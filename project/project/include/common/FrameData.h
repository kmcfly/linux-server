/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : Ԭʯά
** Date         : 2007-12-04
** Name         : FrameData.h
** Version      : 1.0
** Description  :
** Modify Record:
1:
***********************************************************************/
#ifndef _FRAME_DATA_H_
#define _FRAME_DATA_H_

#include "dvrdvsdef.h"
#include "PUB_common.h"

extern unsigned long PRE_RECORD_MAX_LEN;
extern unsigned long PRE_RECORD_MAX_TIME;	//Ԥ¼��ʱ�䣨�룩

const unsigned long INVALID_DATA_ID = 0xffffffff;

class CFrameData
{
public:
	//interface
	CFrameData();
	CFrameData(const FRAME_INFO_EX & frameInfo, unsigned long style = 0, unsigned long ulProtocal=0, unsigned long ulDataID=INVALID_DATA_ID);
	~CFrameData();

	inline void SetFrameInfo(const FRAME_INFO_EX & frameInfo)
	{
		m_style		= 1;
		m_frameInfo = frameInfo;
		m_dataID	= INVALID_DATA_ID;
		m_count		= 0;
		m_protocal	= 0;
	}

	void SetProtocol(unsigned long protocol)
	{
		m_dataLock.Lock();

		m_protocal = protocol;

		m_dataLock.UnLock();
	}

	unsigned long GetProtocol();
	
	void SetDataId(unsigned long dataID)
	{
		m_dataLock.Lock();

		m_dataID = dataID;

		m_dataLock.UnLock();
	}

	unsigned long GetDataId();
	bool IsKeyFrame()
	{
		return m_frameInfo.keyFrame;
	}
// 	unsigned long Add();
// 	unsigned long Dec();
// 	
// 	FRAME_INFO_EX & GetFrameInfo();
// 	unsigned long GetStatus();

	unsigned long Add()
	{
		m_dataLock.Lock();

		++ m_count;

		m_dataLock.UnLock();

		return m_count;
	}

	unsigned long Dec()
	{
		int iCount = 0;
		m_dataLock.Lock();

		assert (0 < m_count);

		-- m_count;

		if (0 == m_style)
		{
			iCount = m_count;
			if (0 == m_count) 
			{
				m_dataLock.UnLock();
				delete this;
				return iCount;
			}		
		}
		else
		{
			iCount = m_count;
		}

		m_dataLock.UnLock();
		return iCount;
	}

	FRAME_INFO_EX & GetFrameInfo()
	{
		return m_frameInfo;
	}

	unsigned long GetStatus()
	{
		m_dataLock.Lock();

		unsigned long ret = m_count;

		m_dataLock.UnLock();

		return ret;
	}
	
	FRAME_TYPE FrameType()
	{
		return (FRAME_TYPE)m_frameInfo.frameType;
	}
	
	unsigned long FrameLen()
	{
		return m_frameInfo.length;
	}
protected:
	

private:
	//methods

	//variable
	FRAME_INFO_EX	m_frameInfo;
	unsigned long	m_style;
	unsigned long	m_dataID;
	unsigned long	m_count;
	unsigned long	m_protocal;
	CPUB_Lock		m_dataLock;
};
#endif //_FRAME_DATA_H_
