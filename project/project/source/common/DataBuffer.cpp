/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : YSW
** Date         : 2012-03-14
** Name         : DataBuffer.cpp
** Version      : 1.0
** Description  :
** Modify Record:
1:
***********************************************************************/
#include "DataBuffer.h"
#ifdef __DEBUG_NEW__
#include "debug_new.h"
#define new DEBUG_NEW
#endif


/***********************************************************************
四种情况：
1、free
2、used | free
3、free | used | free
4、used | free | used | free
***********************************************************************/
CDataBuffer::CDataBuffer():m_pBuffer(NULL),m_bufLength(0), m_bLocked(false)
{
}

CDataBuffer::~CDataBuffer()
{
	if (NULL != m_pBuffer)
	{
		delete [] m_pBuffer;
		m_pBuffer	= NULL;
		m_bufLength = 0;
	}
}

int CDataBuffer::Initial(unsigned int bufLength/* = DEFAULT_BUFFER_LENGTH*/)
{
	assert (NULL == m_pBuffer);
	assert (0 == m_bufLength);
	if (NULL != m_pBuffer)
	{
		delete [] m_pBuffer;
		m_pBuffer = NULL;
	}

	m_bufLength	= bufLength;
	assert((0 < m_bufLength) && (m_bufLength <= 8*1024*1024));
	m_bLocked = false;

	m_pBuffer	= new unsigned char [m_bufLength];
	assert (NULL != m_pBuffer);

	BUFFER_BLOCK	block;
	block.disable	= false;
	block.start		= 0;
	block.end		= m_bufLength;

	m_freeList.push_back(block);

	return 0;
}

void CDataBuffer::Quit()
{
	if (NULL != m_pBuffer)
	{
		delete [] m_pBuffer;
		m_pBuffer	= NULL;
		m_bufLength = 0;
	}
}

int CDataBuffer::GetBuffer(unsigned char **ppBuf, unsigned int & bufLength)
{
	assert (!m_bLocked);
	if (m_bLocked)
	{
		return 1;
	}

	assert (NULL != ppBuf);
	assert((0 < bufLength) && (bufLength <= m_bufLength));
	if ((NULL == ppBuf) || (0 == bufLength) || (m_bufLength < bufLength))
	{
		return 2;
	}

	assert (0 < m_freeList.size());

	BUFFER_BLOCK & freeFront = m_freeList.front();

	if (!freeFront.disable && (bufLength <= (freeFront.end - freeFront.start)))
	{
		*ppBuf	= (m_pBuffer + freeFront.start);

		m_bLocked = true;

		return 0;
	}

	BUFFER_BLOCK & freeBack = m_freeList.back();
	assert (!freeBack.disable);

	if (bufLength <= (freeBack.end - freeBack.start))
	{
		*ppBuf	= (m_pBuffer + freeBack.start);

		m_bLocked = true;

		return 0;
	}

	return 1;
}

void CDataBuffer::UnlockBuffer()
{
	assert(m_bLocked);
	m_bLocked = false;
}

void CDataBuffer::UsedBuffer(unsigned char *pBuf, unsigned int bufLength)
{
	assert ((m_pBuffer <= pBuf) && (pBuf < (m_pBuffer + m_bufLength)));
	assert ((pBuf + bufLength) <= (m_pBuffer +m_bufLength));

	if ((m_pBuffer <= pBuf) && (pBuf < (m_pBuffer + m_bufLength)))
	{
		unsigned int num = m_usedList.size();

		if (0 == num)
		{
			BUFFER_BLOCK & free = m_freeList.front();
			assert (0 == free.start);
			assert (m_bufLength == free.end);

			BUFFER_BLOCK used;
			used.start	= free.start;
			used.end	= (used.start + bufLength);
			free.start	= used.end;

			m_usedList.push_back(used);
		}
		else
		{
			BUFFER_BLOCK & used = m_usedList.front();

			if((m_pBuffer + used.end) == pBuf)
			{
				BUFFER_BLOCK & free = m_freeList.front();
				assert (used.end == free.start);
				assert (m_bufLength == free.end);

				used.end += bufLength;
				free.start = used.end;

				assert (free.start <= free.end);

				if (free.start == free.end)
				{
					m_freeList.pop_front();
				}
			}
			else if (1 == num)
			{
				assert (m_pBuffer == pBuf);
				assert (2 == m_freeList.size());

				BUFFER_BLOCK & used		= m_usedList.front();
				BUFFER_BLOCK & freeFront = m_freeList.front();
				BUFFER_BLOCK & freeBack = m_freeList.back();

				assert (0 == freeBack.start);
				assert (freeBack.end == used.start);

				BUFFER_BLOCK newUsed;
				newUsed.start	= freeBack.start;
				newUsed.end		= (newUsed.start + bufLength);
				freeBack.start		= newUsed.end;

				assert (freeBack.start <= freeBack.end);

				m_usedList.push_back(newUsed);

				assert (used.end == freeFront.start);
				assert (m_bufLength == freeFront.end);

				freeFront.disable = true;
			}
			else if (2 == num)
			{
				BUFFER_BLOCK & used = m_usedList.back();
				BUFFER_BLOCK & free = m_freeList.back();

				assert (0 == used.start);
				assert (used.end == free.start);
				assert ((m_pBuffer + used.end) == pBuf);

				used.end += bufLength;
				free.start = used.end;

				assert (free.start <= free.end);
			}
		}
	}
}

void CDataBuffer::ReleaseBuf(unsigned char *pBuf, unsigned int bufLength)
{
	assert ((m_pBuffer <= pBuf) && (pBuf < (m_pBuffer + m_bufLength)));
	assert ((pBuf + bufLength) <= (m_pBuffer +m_bufLength));

	if ((m_pBuffer <= pBuf) || (pBuf < (m_pBuffer + m_bufLength)))
	{
		BUFFER_BLOCK & used		= m_usedList.front();
		BUFFER_BLOCK & freeBack = m_freeList.back();

		if (freeBack.end == used.start)
		{
			assert ((m_pBuffer + used.start) == pBuf);
			assert (bufLength <= (used.end - used.start));

			freeBack.end += bufLength;
			used.start = freeBack.end;

			//问题：第一次调用GetBuffer，UsedBuffer，UnlockBuffer后，
			//      接着第二次调用GetBuffer，在还没有第二次调用UsedBuffer，UnlockBuffer时，
			//      先调用了第一次ReleaseBuffer,这时会满足条件：bufLength == freeBack.start，
			//      于是会执行下面的操作，导致第二次调用ReleaseBuffer时，
			//      会断言在上面一行：assert (m_pBuffer == pBuf);
			//解决办法：加上判断条件!m_bLocked
			if ((used.start == used.end) && !m_bLocked)
			{
				m_usedList.pop_front();

				if (2 == m_freeList.size())
				{
					BUFFER_BLOCK & freeFront = m_freeList.front();

					//assert (freeFront.disable);
					assert (freeBack.end == freeFront.start);
					assert (m_bufLength == freeFront.end);

					freeFront.start = freeBack.start;
					freeFront.disable = false;

					m_freeList.pop_back();
				}
				else
				{
					assert (!freeBack.disable);
					assert (m_bufLength == freeBack.end);
				}
			}
		}
		else
		{
			assert (1 == m_usedList.size());
			assert (1 == m_freeList.size());

			assert (0 == used.start);
			assert (used.end == freeBack.start);
			assert (freeBack.end == m_bufLength);
			assert (m_pBuffer == pBuf);
			assert (bufLength <= used.end);

			//问题：第一次调用GetBuffer，UsedBuffer，UnlockBuffer后，
			//      接着第二次调用GetBuffer，在还没有第二次调用UsedBuffer，UnlockBuffer时，
			//      先调用了第一次ReleaseBuffer,这时会满足条件：bufLength == freeBack.start，
			//      于是会执行下面的操作，导致第二次调用ReleaseBuffer时，
			//      会断言在上面一行：assert (m_pBuffer == pBuf);
			//解决办法：加上判断条件!m_bLocked
			if ((bufLength == freeBack.start) && !m_bLocked)
			{
				freeBack.start = 0;

				m_usedList.pop_front();
			}
			else
			{
				used.start = bufLength;

				BUFFER_BLOCK newFree;

				newFree.disable = false;
				newFree.start	= 0;
				newFree.end	= bufLength;
				m_freeList.push_back(newFree);
			}
		}
	}
	else
	{
		assert(false);
	}
}

//end
