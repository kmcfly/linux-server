#ifndef __FRAME_DATA_POOL_H__
#define __FRAME_DATA_POOL_H__

#include "FrameData.h"

#include <deque>

class CFrameDataPool
{
public:
	CFrameDataPool(unsigned long count=20 * 1024);
	~CFrameDataPool();

	inline CFrameData * Get(const FRAME_INFO_EX & frameInfo)
	{
		m_usePos --;
		CFrameData * pFrameData = m_ppFrameBuff[m_usePos];
		if (0 == m_usePos)
		{
			NewFrameBuff(1024);
		}
		
		pFrameData->SetFrameInfo(frameInfo);

		return pFrameData;
	}

	inline void Release(CFrameData * pFrameData)
	{
		m_ppFrameBuff[m_usePos] = pFrameData;
		m_usePos ++;
	}
protected:
private:
	void NewFrameBuff(unsigned long count);
private:
	unsigned long			m_bufCount;
	unsigned long			m_frameCount;
	CFrameData			**	m_ppFrameBuff;
	unsigned long			m_usePos;
	std::list<CFrameData *> m_frameBuffList;
};
#endif

