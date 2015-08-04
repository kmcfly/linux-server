#include "FrameDataPool.h"
#include <assert.h>

CFrameDataPool::CFrameDataPool(unsigned long count)
{
	m_ppFrameBuff = new CFrameData * [128 * 1024];
	m_bufCount = 128 * 1024;
	m_frameCount = 0;
	m_usePos = 0;
	if (count < 1024)
	{
		count = 1024;
	}

	NewFrameBuff(count);
}

CFrameDataPool::~CFrameDataPool()
{
	for (std::list<CFrameData *>::iterator iter = m_frameBuffList.begin(); iter != m_frameBuffList.end(); iter++)
	{
		delete [] (* iter);
	}
	m_frameBuffList.clear();
}

void CFrameDataPool::NewFrameBuff(unsigned long count)
{
	CFrameData * pFrameData = new CFrameData [count];
	m_frameCount += count;
	if (m_frameCount > m_bufCount)
	{
		assert(false);

	}

	m_frameBuffList.push_back(pFrameData);

	for (int i = 0; i< count; i++)
	{
		m_ppFrameBuff[m_usePos] = pFrameData + i;
		m_usePos ++;
	}
}
