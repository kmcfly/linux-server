#include "HxCycleBuffer.h"
#include <assert.h>
#include <stdio.h>
#include "Typedef.h"

CHxCycleBuffer::CHxCycleBuffer(void)
{
	m_bufferSize	= 0;
	m_pBuffer		= NULL;
	m_pReadCursor	= NULL;
	m_pWriteCursor	= NULL;
	m_pUnused		= NULL;
	m_bWaitForQuit	= false;
}

CHxCycleBuffer::~CHxCycleBuffer(void)
{
}

bool CHxCycleBuffer::Initial(long bufferSize)
{
	assert(bufferSize > 0);
	assert(NULL == m_pBuffer);

	if(m_pBuffer != NULL)
	{
		return true;
	}

	m_pBuffer = new unsigned char [bufferSize];
	if(NULL == m_pBuffer)
	{
		return false;
	}

	m_bufferSize = bufferSize;
	m_pWriteCursor = m_pBuffer;

	return true;
}

bool CHxCycleBuffer::Quit()
{
	if(NULL == m_pBuffer)
	{
		return true;
	}

	//有数据还没有读完，等待缓冲区释放
	if(m_pReadCursor != NULL)
	{
		m_bWaitForQuit = true;
		return false;
	}

	delete [] m_pBuffer;

	m_pBuffer		= NULL;
	m_pWriteCursor	= NULL;
	m_pUnused		= NULL;
	m_bufferSize	= 0;
	m_bWaitForQuit	= false;

	return true;
}


unsigned char *CHxCycleBuffer::GetBuffer(long bufferLen)
{
	if(NULL == m_pBuffer)
	{
		return NULL;
	}

	unsigned char *pRetBuffer = NULL;

	if(NULL == m_pReadCursor)	//缓冲区全部空闲
	{
		m_pWriteCursor = m_pBuffer;
		if(m_bufferSize >= bufferLen)
		{
			pRetBuffer = m_pWriteCursor;
			m_pReadCursor = m_pWriteCursor;
			m_pWriteCursor += bufferLen;
		}
		else
		{
			assert(false);
		}
	}
	else if(m_pReadCursor >= m_pWriteCursor)	//缓冲区中间空闲
	{
		if((m_pReadCursor - m_pWriteCursor) >= bufferLen)
		{
			pRetBuffer = m_pWriteCursor;
			m_pWriteCursor += bufferLen;
		}

	}
	else		//缓冲区尾部空闲
	{
		if((m_pBuffer+m_bufferSize-m_pWriteCursor) >= bufferLen)
		{
			pRetBuffer = m_pWriteCursor;
			m_pWriteCursor += bufferLen;
		}
		else
		{
			m_pUnused = m_pWriteCursor;
			m_pWriteCursor = m_pBuffer;
			if((m_pReadCursor - m_pWriteCursor) > bufferLen)
			{
				pRetBuffer = m_pWriteCursor;
				m_pWriteCursor += bufferLen;
			}
			else
			{
				//assert(false);
			}

		}
	}

	if (pRetBuffer > m_pBuffer + m_bufferSize)
	{
		printf("%s:%s:%d, error", __FUNCTION__, __FILE__, __LINE__);
		return NULL;
	}
	return pRetBuffer;
}

void CHxCycleBuffer::ReleaseBuffer(unsigned char *pBuffer, long bufferLen)
{
	if(NULL == m_pReadCursor)
	{
		return;
	}

	if (pBuffer > m_pBuffer + m_bufferSize)
	{
		printf("%s:%s:%d, error", __FUNCTION__, __FILE__, __LINE__);
		assert(false);
	}
	if(m_pReadCursor != pBuffer)
	{
		assert(false);
		return;
	}

	if((m_pReadCursor + bufferLen) > (m_pBuffer + m_bufferSize))
	{
		assert(false);
		return;
	}

	if((m_pWriteCursor > m_pReadCursor) && ((m_pReadCursor + bufferLen) > m_pWriteCursor))
	{
		assert(false);
		return;
	}

	if((NULL != m_pUnused) && ((m_pReadCursor + bufferLen) > m_pUnused))
	{
		assert(false);
		return;
	}

	m_pReadCursor += bufferLen;

	if(m_pReadCursor == m_pWriteCursor)
	{
		m_pReadCursor = NULL;
		m_pWriteCursor = m_pBuffer;
		m_pUnused = NULL;
	}
	else if(m_pReadCursor == m_pUnused)
	{
		m_pUnused = NULL;
		if(m_pWriteCursor == m_pBuffer)
		{
			m_pReadCursor = NULL;
		}
		else
		{
			m_pReadCursor = m_pBuffer;
		}
	}
}


bool CHxCycleBuffer::WaitForQuit()
{
	return m_bWaitForQuit;
}

