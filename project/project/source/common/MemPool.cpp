#include "MemPool.h"
CMemPool * CMemPool::m_pMemPool = NULL;

CMemPool::CMemPool()
{
	m_totalMemLen = 0;
	m_usedMemLen = 0;
	memset(m_memPool, 0, sizeof(m_memPool));
}

CMemPool::~CMemPool()
{

}

CMemPool * CMemPool::Instance()
{
	if (NULL == m_pMemPool)
	{
		m_pMemPool = new CMemPool;
	}

	return m_pMemPool;
}

bool CMemPool::Initial(unsigned long totalMemLen)
{
	m_totalMemLen = totalMemLen;
	return true;
}

bool CMemPool::AddPool(MEM_TYPE memType, unsigned long length, unsigned long count)
{
	if (MEM_TYPE_END <= memType)
	{
		printf("%s:%s:%d, Running at here, memeType=%d, length=%d, count=%d\n", __FUNCTION__, __FILE__, __LINE__, memType, length, count);
		fflush(stderr);
		return false;
	}

	if (0 < m_memPool[memType].count)
	{
		printf("%s:%s:%d, Running at here, memeType=%d, length=%d, count=%d\n", __FUNCTION__, __FILE__, __LINE__, memType, length, count);
		fflush(stderr);
		return false;
	}

	if ((m_usedMemLen + length * count) > m_totalMemLen)
	{
		printf("%s:%s:%d, Running at here, memeType=%d, length=%d, count=%d\n", __FUNCTION__, __FILE__, __LINE__, memType, length, count);
		fflush(stderr);
		return false;
	}

	m_memPool[memType].count = count;
	m_memPool[memType].memType = memType;
	m_memPool[memType].memLen = length;
	m_memPool[memType].pMemUse = new MEM_USE [count];
	if (NULL == m_memPool[memType].pMemUse)
	{
		printf("%s:%s:%d, Running at here, memeType=%d, length=%d, count=%d\n", __FUNCTION__, __FILE__, __LINE__, memType, length, count);
		fflush(stderr);
		return false;
	}

	for (int i=0; i<count; i++)
	{
		m_memPool[memType].pMemUse[i].bUsed = false;
		m_memPool[memType].pMemUse[i].pMem = new char[length];
		if (NULL != m_memPool[memType].pMemUse[i].pMem)
		{
			memset(m_memPool[memType].pMemUse[i].pMem, 0, length);
		}
		else
		{
			printf("%s:%s:%d, Running at here, memeType=%d, length=%d, count=%d\n", __FUNCTION__, __FILE__, __LINE__, memType, length, count);
			fflush(stderr);
			return false;
		}
	}
	return true;
}

void CMemPool::Quit()
{
	for (int type=0; type<MEM_TYPE_END; type++)
	{
		if (NULL != m_memPool[type].pMemUse)
		{
			for (int i=0; i<m_memPool[type].count; i++)
			{
				if (NULL != m_memPool[type].pMemUse[i].pMem)
				{
					delete m_memPool[type].pMemUse[i].pMem;
				}
				m_memPool[type].pMemUse[i].pMem = NULL;
			}
			delete m_memPool[type].pMemUse;
			m_memPool[type].pMemUse = NULL;
		}
	}
}

void * CMemPool::GetMem(MEM_TYPE memType, unsigned long length)
{
	if (MEM_TYPE_END <= memType)
	{
		printf("%s:%s:%d, Running at here\n", __FUNCTION__, __FILE__, __LINE__);
		fflush(stderr);
		return NULL;
	}

	if (0 == m_memPool[memType].count || NULL == m_memPool[memType].pMemUse || length > m_memPool[memType].memLen || memType != m_memPool[memType].memType)
	{
		printf("%s:%s:%d, Running at here\n", __FUNCTION__, __FILE__, __LINE__);
		printf("count=%d, pMemUse=%p, length=%d, memType=%d\n", 
			m_memPool[memType].count,m_memPool[memType].pMemUse,m_memPool[memType].memLen,m_memPool[memType].memType);
		printf("type=%d, length=%d\n", memType, length);

		fflush(stderr);
		
		return NULL;
	}

	void * pMem = NULL;
	for (int i=0; i<m_memPool[memType].count; i++)
	{
		if (!m_memPool[memType].pMemUse[i].bUsed)
		{
			m_memPool[memType].pMemUse[i].bUsed = true;
			pMem = m_memPool[memType].pMemUse[i].pMem;
			break;
		}
	}

	return pMem;
}

void CMemPool::ReleaseMem(void * pMem)
{
	for (int type=0; type<MEM_TYPE_END; type++)
	{
		if (NULL != m_memPool[type].pMemUse)
		{
			for (int i=0; i<m_memPool[type].count; i++)
			{
				if ((char *)pMem == m_memPool[type].pMemUse[i].pMem)
				{
					m_memPool[type].pMemUse[i].bUsed = false;
					break;
				}
			}
		}
	}
}

