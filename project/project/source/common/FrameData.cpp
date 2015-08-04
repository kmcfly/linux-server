/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : Ԭʯά
** Date         : 2007-12-04
** Name         : FrameData.cpp
** Version      : 1.0
** Description  :
** Modify Record:
1:
***********************************************************************/
#include "FrameData.h"
//public

CFrameData::CFrameData() : m_style(0xffffffff), m_dataID(INVALID_DATA_ID), m_count(0), m_protocal(0)
{
}

CFrameData::CFrameData(const FRAME_INFO_EX & frameInfo, unsigned long style, unsigned long ulProtocal, unsigned long ulDataID) : 
m_frameInfo(frameInfo), m_style(style), m_dataID(ulDataID), m_count(0), m_protocal(ulProtocal)
{
//	if (FRAME_TYPE_NONE != frameInfo.frameType)
//	{
//		assert(NULL != frameInfo.pData);
// 	}
}

CFrameData::~CFrameData()
{
	if ((0 == m_style) && (NULL != m_frameInfo.pData))
	{
		delete [] m_frameInfo.pData;
		m_frameInfo.pData = NULL;
	}
}

// void CFrameData::SetProtocol(unsigned long protocol)
// {
// 	m_dataLock.Lock();
// 
// 	m_protocal = protocol;
// 
// 	m_dataLock.UnLock();
// }

unsigned long CFrameData::GetProtocol()
{
	m_dataLock.Lock();

	unsigned long protocol = m_protocal;

	m_dataLock.UnLock();

	return protocol;
}

// void CFrameData::SetDataId(unsigned long dataID)
// {
// 	m_dataLock.Lock();
// 	
// 	m_dataID = dataID;
// 	
// 	m_dataLock.UnLock();
// }

unsigned long CFrameData::GetDataId()
{
	m_dataLock.Lock();
	
	unsigned long dataId = m_dataID;
	
	m_dataLock.UnLock();
	
	return dataId;
}

//protected

//private

//end
