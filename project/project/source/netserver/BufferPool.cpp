// BufferPool.cpp: implementation of the CBufferPool class.
//
//////////////////////////////////////////////////////////////////////
#include "BufferPool.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBufferPool::CBufferPool()
{
	PUB_InitLock(&m_lock);
}

CBufferPool::~CBufferPool()
{
	PUB_DestroyLock(&m_lock);
}

bool CBufferPool::Init(long size)
{
	CPUB_LockAction functionLock(&m_lock);

	m_pBuffer			= new char[size];
	if(m_pBuffer == NULL)
	{
		assert(false);
		return false;
	}

	m_nBufferSize		= size;
	m_iReadCursor		= 0;
	m_iWriteCursor		= 0;

	return true;
}

void CBufferPool::Destroy()
{
	CPUB_LockAction functionLock(&m_lock);

	if(m_pBuffer != NULL)
	{
		delete [] m_pBuffer;
	}
	m_nBufferSize		= 0;
	m_iReadCursor		= 0;
	m_iWriteCursor		= 0;
}

void CBufferPool::Empty()
{
	CPUB_LockAction functionLock(&m_lock);

	m_iReadCursor		= 0;
	m_iWriteCursor		= 0;
}


//基本属性操作
long CBufferPool::GetTotalSize()
{
	CPUB_LockAction functionLock(&m_lock);

	if(m_pBuffer == NULL)
	{
		return 0;
	}
	long totalSize = m_nBufferSize-1;
	return totalSize;	
}

long CBufferPool::GetDataSize()
{
	CPUB_LockAction functionLock(&m_lock);

	if(m_pBuffer == NULL)
	{
		return 0;
	}
	long dataSize = (m_iWriteCursor + m_nBufferSize - m_iReadCursor) % m_nBufferSize;
	return dataSize;
}

long CBufferPool::GetFreeSize()
{
	CPUB_LockAction functionLock(&m_lock);

	if(m_pBuffer == NULL)
	{
		return 0;
	}
	long freeSize = (m_iReadCursor + m_nBufferSize - 1 - m_iWriteCursor) % m_nBufferSize;

	return freeSize;
}

bool CBufferPool::IncreaseSize(long size)
{
	CPUB_LockAction functionLock(&m_lock);

	char *pTemp = m_pBuffer;
	m_pBuffer = new char[m_nBufferSize + size];
	if(m_pBuffer == NULL)
	{
		assert(false);
		return false;
	}

	if(pTemp != NULL)
	{
		if(m_iWriteCursor > m_iReadCursor)
		{
			memcpy(m_pBuffer, pTemp+m_iReadCursor, m_iWriteCursor-m_iReadCursor);
		}
		else if(m_iWriteCursor < m_iReadCursor)
		{
			memcpy(m_pBuffer, pTemp+m_iReadCursor, m_nBufferSize-m_iReadCursor);
			memcpy(m_pBuffer+m_nBufferSize-m_iReadCursor, pTemp, m_iWriteCursor);
		}
		else
		{
			//没有数据
		}
		m_iWriteCursor = (m_iWriteCursor+m_nBufferSize-m_iReadCursor) % m_nBufferSize;
		m_iReadCursor = 0;
		m_nBufferSize += size;
		delete [] pTemp;
	}
	else
	{
		m_iWriteCursor = 0;
		m_iReadCursor = 0;
		m_nBufferSize = size;
	}

	return true;
}


bool CBufferPool::PushData(const void *pData, long len)
{
	CPUB_LockAction functionLock(&m_lock);

	if(GetFreeSize() < len)
	{
		return false;
	}

	long nTailFreeSize = m_nBufferSize - m_iWriteCursor;
	if(nTailFreeSize < len)
	{
		memcpy(m_pBuffer+m_iWriteCursor, static_cast<const char *>(pData), nTailFreeSize);
		memcpy(m_pBuffer, static_cast<const char *>(pData)+nTailFreeSize, len-nTailFreeSize);
		m_iWriteCursor = len - nTailFreeSize;
	}
	else
	{
		memcpy(m_pBuffer+m_iWriteCursor, static_cast<const char *>(pData), len);
		m_iWriteCursor = (m_iWriteCursor+len) % m_nBufferSize;
	}

	return true;
}

long CBufferPool::PopData(char *pData, long len)
{
	CPUB_LockAction functionLock(&m_lock);

	if(m_pBuffer == NULL)
	{
		return 0;
	}

	if(GetDataSize() >= len)
	{
		return len;
	}
	else
	{
		return GetDataSize();
	}
}
	
char *CBufferPool::GetFreeBuffer(long len)
{
	CPUB_LockAction functionLock(&m_lock);

	if(m_pBuffer == NULL)
	{
		return NULL;
	}
	
	if(GetFreeSize() < len)
	{
		return NULL;
	}

	//保证以返回地址为首地址的空间有len这么大
	if((m_iWriteCursor + len) > m_nBufferSize)
	{
		m_iWriteCursor = 0;
		return GetFreeBuffer(len);
	}

	return m_pBuffer + m_iWriteCursor;
}

char *CBufferPool::GetDataBuffer(long len)
{
	CPUB_LockAction functionLock(&m_lock);

	if(m_pBuffer == NULL)
	{
		return NULL;
	}

	if(GetDataSize() < len)
	{
		return NULL;
	}

	//保证以返回地址为首地址的空间有len这么大,否则进行数据位置调整
	if((m_iReadCursor + len) > m_nBufferSize)
	{
		char *pTemp = new char[m_nBufferSize];
		memcpy(pTemp, m_pBuffer, m_nBufferSize);
		memcpy(m_pBuffer, pTemp + m_iReadCursor, m_nBufferSize - m_iReadCursor);
		memcpy(m_pBuffer + m_nBufferSize - m_iReadCursor, pTemp, m_iWriteCursor);
		m_iWriteCursor = m_nBufferSize - m_iReadCursor + m_iWriteCursor;
		m_iReadCursor = 0;
		delete [] pTemp;

		return GetDataBuffer(len);
	}

	return m_pBuffer+m_iReadCursor;
}

char *CBufferPool::GetBufferHead()
{
	CPUB_LockAction functionLock(&m_lock);

	if(m_pBuffer == NULL)
	{
		return NULL;
	}
	return m_pBuffer;
}

char *CBufferPool::GetBufferTail()
{
	CPUB_LockAction functionLock(&m_lock);

	if(m_pBuffer == NULL)
	{
		return NULL;
	}
	return m_pBuffer+m_nBufferSize;
}

void CBufferPool::FreeBuffer(long len)
{
	CPUB_LockAction functionLock(&m_lock);

	assert(GetDataSize() >= len);
	m_iReadCursor = (m_iReadCursor + len) % m_nBufferSize;
}

void CBufferPool::FillBuffer(long len)
{
	CPUB_LockAction functionLock(&m_lock);

	assert(GetFreeSize() >= len);
	m_iWriteCursor = (m_iWriteCursor + len) % m_nBufferSize;
}


