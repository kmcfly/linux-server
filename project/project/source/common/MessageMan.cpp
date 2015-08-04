// MessageMan.cpp: implementation of the CMessageMan class.
//
//////////////////////////////////////////////////////////////////////
#include "StdHeader.h"
#include "MessageMan.h"
#include "NetProtocol.h"
#include "MainFrame.h"

extern ULONGLONG g_net_channel;
unsigned long CMessageMan::m_sNextClientID = 1;
CPUB_Lock CMessageMan::m_clientIDLock;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CMessageMan * CMessageMan::Instance()
{
	static CMessageMan s_MessageMan;
	return &s_MessageMan;
}

CMessageMan::CMessageMan()
{
	m_bChannelVisible  = true;
	m_bCheckPassword	= true;
	m_bRefreshLive		= false;
	m_bCamNameChange	= false;	
	m_bShowTime			= true;
	m_bRecycleRec		= false;

	m_bShowSnapTip		= false;
	m_xPosSnapTip		= 20;
	m_yPosSnapTip		= 20;
	m_bSuccSnapTip		= true;

	m_videoInputNum		= 0;

	m_motion	= 0;
	m_sensorIn	= 0;
	m_videoLoss	= 0;
	m_alarmOut	= 0;

	m_recManual		= 0;
	m_recSchedule	= 0;
	m_recMotion		= 0;
	m_recSensor		= 0;

	m_PBShowCamNameCH	= 0;
	m_displayCH			= 0;
	m_ullNetDeviceStatus = 0;
	m_ullPoeOnlineStatus = 0;
	
	m_showName		= 0;
	m_pstrCamName	= NULL;
	m_pStrSensorName= NULL;

	m_currPTZchnn	= INVALID_INDEX(unsigned char);

	m_bShowDiskAlarm = false;
	m_bDiskError = false;

#if defined(__ASYN_PLAY__)
	for (int i = 0; i < 32; i++)
	{
		m_showSpeed[i] = PLAY_SPEED_1X;
	}
#endif
	m_pMainFrame = NULL;
}

CMessageMan::~CMessageMan()
{
}

bool CMessageMan::Initial(unsigned char videoInputNum, unsigned char sensorInNum)
{
	m_videoInputNum = videoInputNum;
	m_sensorInNum = sensorInNum;

	m_pstrCamName = new std::string	[m_videoInputNum];
	m_pStrSensorName = new std::string[m_sensorInNum];

#ifdef __SUPPORT_OSDTWOLINES__
	m_pCamPoint = new POINT[m_videoInputNum];
	memset(m_pCamPoint, 0, sizeof(POINT)*m_videoInputNum);
#endif

	m_pFBInterface = GUI::CFBInterface::Instance();

	return true;
}

void CMessageMan::Quit()
{
	Clear();

	delete [] m_pstrCamName;
	m_pstrCamName = NULL;

	delete [] m_pStrSensorName;
	m_pStrSensorName = NULL;

#ifdef __SUPPORT_OSDTWOLINES__
	delete []m_pCamPoint;
	m_pCamPoint = NULL;
#endif

	m_pFBInterface = NULL;
}

bool CMessageMan::RefreshLive()
{
	if (m_bRefreshLive)
	{
		m_bRefreshLive = false;
		return true;
	}
	else
	{
		return false;
	}
}

void CMessageMan::SetPointToMainFrame(unsigned long *pMainFrame)
{
	m_pMainFrame = pMainFrame;
}

bool CMessageMan::CheckFreezeScreenSave()
{
	assert(m_pMainFrame != NULL);
	return ((CMainFrame*)m_pMainFrame)->CheckFreezeScreenSave();
}

void CMessageMan::RefreshSensorName(unsigned char sensor, const char *pName)
{
	assert (sensor < m_sensorInNum);
	if (sensor < m_sensorInNum)
	{
		m_pStrSensorName[sensor] = pName;
	}
}

bool CMessageMan::GetSensorName(unsigned char sensor,std::string & name)
{
	assert (sensor < m_sensorInNum);
	if (sensor >= m_sensorInNum)
	{
		return false;
	}

	name = m_pStrSensorName[sensor];

	return true;
}

void CMessageMan::RefreshCamName(unsigned char chnn, const char *pName)
{
	assert (chnn < m_videoInputNum);

	m_bRefreshLive = true;
	m_bCamNameChange = true;

	m_pstrCamName[chnn] = pName;
}

bool CMessageMan::GetCamName(unsigned char chnn,std::string & name)
{
	assert (chnn < m_videoInputNum);

	name = m_pstrCamName[chnn];

	return true;
}

void CMessageMan::SetShowName(unsigned char chnn, bool bShow)
{
	if (bShow)
	{
		m_showName |= (((ULONGLONG)0x01) << chnn);
	}
	else
	{
		m_showName &= ~(((ULONGLONG)0x01) << chnn);
	}

	m_bRefreshLive = true;
}
#if defined(__ASYN_PLAY__)
void CMessageMan::PBShowSpeed(PLAYBACK_SPEED_INFO speedInfo)
{
	if (speedInfo.chnn == 0xFF)
	{
		for (int i = 0; i < m_videoInputNum; i++)
		{
			m_showSpeed[i] = speedInfo.speed;
		}
	}
	else if (speedInfo.chnn >=0 && speedInfo.chnn < m_videoInputNum)
	{
		m_showSpeed[speedInfo.chnn] = speedInfo.speed;
	}
	else
	{
		assert(false);
	}
}
#endif
void CMessageMan::SetSnapTipPos(unsigned long x, unsigned long y)
{
	m_xPosSnapTip = x;
	m_yPosSnapTip = y;
}

void CMessageMan::GetSnapTipPos(unsigned long& x, unsigned long& y)
{
	x = m_xPosSnapTip;
	y = m_yPosSnapTip;
}

void CMessageMan::SetShowSnapTip(bool bShow, bool bSucc)
{
	if((m_bShowSnapTip != bShow) || (m_bSuccSnapTip != bSucc))
	{
		RefreshLiveTrue();
	}

	m_bShowSnapTip = bShow;
	m_bSuccSnapTip = bSucc;
}

void CMessageMan::ShowSnapTip(bool& bShow, bool& bSucc)
{
	bShow = m_bShowSnapTip;
	bSucc = m_bSuccSnapTip;
}

void CMessageMan::RefreshDisplay(ULONGLONG CH)
{
	m_displayCH = CH;
	
	m_bRefreshLive = true;
}

void CMessageMan::RefreshRecStatus(unsigned char type, unsigned long chnn)
{
	unsigned char localVideoNum = CProduct::Instance()->LocalVideoInputNum();

	if((chnn >= localVideoNum) && !(g_net_channel & ((ULONGLONG)0x01 << chnn)))
	{
		m_recManual &= ~((ULONGLONG)0x01 << chnn);
		m_recSchedule &= ~((ULONGLONG)0x01 << chnn);
		m_recMotion &= ~((ULONGLONG)0x01 << chnn);
		m_recSensor &= ~((ULONGLONG)0x01 << chnn);
		return;
	}

	if (RECORD_TYPE_MANUAL & type)
	{
		m_recManual |= ((ULONGLONG)0x01 << chnn);
	}
	else
	{
		m_recManual &= ~((ULONGLONG)0x01 << chnn);
	}

	if (RECORD_TYPE_SCHEDULE & type)
	{
		m_recSchedule |= ((ULONGLONG)0x01 << chnn);
	}
	else
	{
		m_recSchedule &= ~((ULONGLONG)0x01 << chnn);
	}

	if (RECORD_TYPE_MOTION & type)
	{
		m_recMotion |= ((ULONGLONG)0x01 << chnn);
	}
	else
	{
		m_recMotion &= ~((ULONGLONG)0x01 << chnn);
	}

	if (RECORD_TYPE_SENSOR & type)
	{
		m_recSensor |= ((ULONGLONG)0x01 << chnn);
	}
	else
	{
		m_recSensor &= ~((ULONGLONG)0x01 << chnn);
	}

	m_bRefreshLive = true;
}

unsigned long CMessageMan::GetRecStatus(unsigned char chnn)
{
	unsigned long type = RECORD_TYPE_NULL;
	if (m_recManual & ((ULONGLONG)0x01 << chnn))
	{
		type |= RECORD_TYPE_MANUAL;
	}

	if (m_recSchedule & ((ULONGLONG)0x01 << chnn))
	{
		type |= RECORD_TYPE_SCHEDULE;
	}

	if (m_recMotion & ((ULONGLONG)0x01 << chnn))
	{
		type |= RECORD_TYPE_MOTION;
	}

	if (m_recSensor & ((ULONGLONG)0x01 << chnn))
	{
		type |= RECORD_TYPE_SENSOR;
	}

	return type;
}

void CMessageMan::RefreshCHStatus(CHNN_STATUS_TYPE type , ULONGLONG value)
{
	switch (type)
	{
	case CHNN_STATUS_MOTION:
		{
			//2010-01-26 12:01:00 YSW
			if (m_motion != value)
			{
				m_bRefreshLive = true;
			}
			m_motion	= value;
		}
		break;
	case CHNN_STATUS_SENSOR_IN:
		{
			//2010-01-26 12:01:00 YSW
			//暂时不刷新
			/*if (m_sensorIn != value)
			{
				m_bRefreshLive = true;
			}*/
			m_sensorIn	= value;
		}
		break;
	case CHNN_STATUS_VIDEO_LOSS:
		{
			if (m_videoLoss != value)
			{
				m_bRefreshLive = true;
			}
			m_videoLoss	= value;
		}
		break;
	case CHNN_STATUS_ALARM_OUT:
		m_alarmOut	= value;
		break;
	default:
		assert(false);
		break;
	}
}

bool CMessageMan::GetCHStatus(CHNN_STATUS_TYPE type , ULONGLONG & value)
{
	switch (type)
	{
	case CHNN_STATUS_MOTION:
		value	= m_motion;
		break;
	case CHNN_STATUS_SENSOR_IN:
		value	= m_sensorIn;
		break;
	case CHNN_STATUS_VIDEO_LOSS:
		value	= m_videoLoss;
		break;
	case CHNN_STATUS_ALARM_OUT:
		value	= m_alarmOut;
		break;
	case CHNN_STATUS_MANUAL_REC:
		value	= m_recManual;
		break;
	case CHNN_STATUS_SCHEDULE_REC:
		value	= m_recSchedule;
		break;
	case CHNN_STATUS_MOTION_REC:
		value	= m_recMotion;
		break;
	case CHNN_STATUS_SENSOR_REC:
		value	= m_recSensor;
		break;
	default:
		assert(false);
		return false;
	}

	return true;
}

void CMessageMan::RefreshDiskStatus(DISK_STATUS_TYPE type)
{
	m_diskStatusList.push_back(type);
}

DISK_STATUS_TYPE CMessageMan::GetDiskStatus()
{
	if (0 != m_diskStatusList.size())
	{
		std::list<DISK_STATUS_TYPE>::iterator it;
		it = m_diskStatusList.begin();
		m_diskStatusList.pop_front();
		return (*it);
	}
	else
	{
		return DISK_STATUS_NORMAL;
	}
}

void CMessageMan::PutFrame(CFrameData * const pFrameData)
{
	assert (NULL != pFrameData);

	if (LIVE_FRAME_FIRST_STREAM == pFrameData->GetFrameInfo().frameAttrib)
	{
		m_mainFrameListLock.Lock();
		pFrameData->Add();
		m_mainFrameInfoList.push_back(pFrameData);
#ifdef __STREAM_SERVER_RTP__
		pFrameData->Add();
		m_mainRTPFrameList.push_back(pFrameData);
#endif
#ifdef __TUTK_SERVER__
		pFrameData->Add();
		m_mainTUTKFrameList.push_back(pFrameData);
#endif
#ifdef __API_SERVER__
		pFrameData->Add();
		m_mainDevAPIFrameList.push_back(pFrameData);
#endif
		m_mainFrameListLock.UnLock();
	}
	else if (LIVE_FRAME_SECOND_STREAM == pFrameData->GetFrameInfo().frameAttrib)
	{
#ifndef __NETSERVER_HXHT__
		m_subFrameListLock.Lock();
		pFrameData->Add();
		m_subFrameInfoList.push_back(pFrameData);
#ifdef __STREAM_SERVER_RTP__
		pFrameData->Add();
		m_subRTPFrameList.push_back(pFrameData);
#endif
#ifdef __TUTK_SERVER__
		pFrameData->Add();
		m_subTUTKFrameList.push_back(pFrameData);
#endif
#ifdef __API_SERVER__
		pFrameData->Add();
		m_subDevAPIFrameList.push_back(pFrameData);
#endif
		m_subFrameListLock.UnLock();
#endif
	}
	else if(LIVE_FRAME_TALK == pFrameData->GetFrameInfo().frameAttrib)
	{
		m_talkFrameListLock.Lock();
		pFrameData->Add();
		m_talkFrameInfoList.push_back(pFrameData);
#ifdef __STREAM_SERVER_RTP__
		pFrameData->Add();
		m_talkRTPFrameList.push_back(pFrameData);
#endif
// #ifdef __TUTK_SERVER__
// 		pFrameData->Add();
// 		m_talkTUTKFrameList.push_back(pFrameData);
// #endif
#ifdef __API_SERVER__
		pFrameData->Add();
		m_talkDevAPIFrameList.push_back(pFrameData);
#endif
		m_talkFrameListLock.UnLock();
	}
	else
	{
		assert (LIVE_FRAME_JPEG == pFrameData->GetFrameInfo().frameAttrib);
		m_jpegFrameListLock.Lock();
		pFrameData->Add();
		m_jpegFrameInfoList.push_back(pFrameData);
#ifdef __SUPPORT_WAP__
		pFrameData->Add();
		m_jpegFrameInfoForWebList.push_back(pFrameData);
#endif
#ifdef __STREAM_SERVER_RTP__
		pFrameData->Add();
		m_jpegRTPFrameList.push_back(pFrameData);
#endif
#ifdef __API_SERVER__
		pFrameData->Add();
		m_jpegDevAPIFrameList.push_back(pFrameData);
#endif
// #ifdef __TUTK_SERVER__
// 		pFrameData->Add();
// 		m_jpegTUTKFrameList.push_back(pFrameData);
// #endif
		m_jpegFrameListLock.UnLock();
	}
}

bool CMessageMan::GetFrame(CFrameData **ppFrameData, unsigned long stream, long netServerType /* = TVT_NET_SERVER*/)
{
	assert (NULL != ppFrameData);

	if (LIVE_FRAME_FIRST_STREAM == stream)
	{
		m_mainFrameListLock.Lock();

		std::list<CFrameData*> *pFrameList = NULL;
		if(TVT_NET_SERVER == netServerType)
		{
			pFrameList = &m_mainFrameInfoList;
		}
#ifdef __STREAM_SERVER_RTP__
		if(RTP_NET_SERVER == netServerType)
		{
			pFrameList = &m_mainRTPFrameList;
		}
#endif
#ifdef __TUTK_SERVER__
		if(TUTK_NET_SERVER == netServerType)
		{
			pFrameList = &m_mainTUTKFrameList;
		}
#endif
#ifdef __API_SERVER__
		if(API_SERVER == netServerType)
		{
			pFrameList = &m_mainDevAPIFrameList;
		}
#endif
		if ((NULL != pFrameList) && (pFrameList->size() != 0))
		{
			*ppFrameData = pFrameList->front();
			pFrameList->pop_front();
			m_mainFrameListLock.UnLock();
			return true;
		}
		else
		{
			m_mainFrameListLock.UnLock();
			return false;
		}
	}
	else if (LIVE_FRAME_SECOND_STREAM == stream)
	{
		m_subFrameListLock.Lock();

		std::list<CFrameData*> *pFrameList = NULL;
		if(TVT_NET_SERVER == netServerType)
		{
			pFrameList = &m_subFrameInfoList;
		}
#ifdef __STREAM_SERVER_RTP__
		if(RTP_NET_SERVER == netServerType)
		{
			pFrameList = &m_subRTPFrameList;
		}
#endif
#ifdef __TUTK_SERVER__
		if(TUTK_NET_SERVER == netServerType)
		{
			pFrameList = &m_subTUTKFrameList;
		}
#endif
#ifdef __API_SERVER__
		if(API_SERVER == netServerType)
		{
			pFrameList = &m_subDevAPIFrameList;
		}
#endif
		if (0 != pFrameList->size())
		{
			*ppFrameData = pFrameList->front();
			pFrameList->pop_front();
			m_subFrameListLock.UnLock();
			return true;
		}
		else
		{
			m_subFrameListLock.UnLock();
			return false;
		}
	}
	else if(LIVE_FRAME_TALK == stream)
	{
		m_talkFrameListLock.Lock();

		std::list<CFrameData*> *pFrameList = NULL;
		if(TVT_NET_SERVER == netServerType)
		{
			pFrameList = &m_talkFrameInfoList;
		}
#ifdef __STREAM_SERVER_RTP__
		if(RTP_NET_SERVER == netServerType)
		{
			pFrameList = &m_talkRTPFrameList;
		}
#endif
// #ifdef __TUTK_SERVER__
// 		if(TUTK_NET_SERVER == netServerType)
// 		{
// 			pFrameList = &m_talkTUTKFrameList;
// 		}
// #endif
#ifdef __API_SERVER__
		if(API_SERVER == netServerType)
		{
			pFrameList = &m_talkDevAPIFrameList;
		}
#endif
		if(0 != pFrameList->size())
		{
			*ppFrameData = pFrameList->front();
			pFrameList->pop_front();
			m_talkFrameListLock.UnLock();
			return true;
		}
		else
		{
			m_talkFrameListLock.UnLock();
			return false;
		}
	}
	else
	{
		m_jpegFrameListLock.Lock();

		std::list<CFrameData*> *pFrameList = NULL;
		if(TVT_NET_SERVER == netServerType)
		{
			pFrameList = &m_jpegFrameInfoList;
		}
#ifdef __STREAM_SERVER_RTP__
		if(RTP_NET_SERVER == netServerType)
		{
			pFrameList = &m_jpegRTPFrameList;
		}
#endif
// #ifdef __TUTK_SERVER__
// 		if(TUTK_NET_SERVER == netServerType)
// 		{
// 			pFrameList = &m_jpegTUTKFrameList;
// 		}
// #endif
#ifdef __API_SERVER__
		if(API_SERVER == netServerType)
		{
			pFrameList = &m_jpegDevAPIFrameList;
		}
#endif
		if (0 != pFrameList->size())
		{
			*ppFrameData = pFrameList->front();
			pFrameList->pop_front();
			m_jpegFrameListLock.UnLock();
			return true;
		}
		else
		{
			m_jpegFrameListLock.UnLock();
			return false;
		}
	}
}

#ifdef __SUPPORT_WAP__
bool CMessageMan::GetFrameForWebServer(CFrameData **ppFrameData, unsigned long stream)
{
	assert (NULL != ppFrameData);
	assert(LIVE_FRAME_JPEG == stream);

	if (LIVE_FRAME_JPEG == stream)
	{
		m_jpegFrameListLock.Lock();
		if (!m_jpegFrameInfoForWebList.empty())
		{
			*ppFrameData = m_jpegFrameInfoForWebList.front();
			m_jpegFrameInfoForWebList.pop_front();
			m_jpegFrameListLock.UnLock();
			return true;
		}
		else
		{
			m_jpegFrameListLock.UnLock();
			return false;
		}
	}

	return false;
}
#endif

//
void CMessageMan::ClearFrame(unsigned long stream)
{
	if (LIVE_FRAME_FIRST_STREAM & stream)
	{
		m_mainFrameListLock.Lock();

		for (std::list<CFrameData *>::iterator it = m_mainFrameInfoList.begin(); it != m_mainFrameInfoList.end();)
		{
			if (FRAME_TYPE_VIDEO_FORMAT != (*it)->GetFrameInfo().frameType && FRAME_TYPE_AUDIO_FORMAT != (*it)->GetFrameInfo().frameType)
			{
				//音频和视频格式帧不清除
				(*it)->Dec();
				it = m_mainFrameInfoList.erase(it);
			}
			else
			{
				it++;
			}
		}

#ifdef __STREAM_SERVER_RTP__
		for (std::list<CFrameData *>::iterator it = m_mainRTPFrameList.begin(); it != m_mainRTPFrameList.end();)
		{
			if (FRAME_TYPE_VIDEO_FORMAT != (*it)->GetFrameInfo().frameType && FRAME_TYPE_AUDIO_FORMAT != (*it)->GetFrameInfo().frameType)
			{
				//音频和视频格式帧不清除
				(*it)->Dec();
				it = m_mainRTPFrameList.erase(it);
			}
			else
			{
				it++;
			}
		}
#endif
#ifdef __TUTK_SERVER__
		for (std::list<CFrameData *>::iterator it = m_mainTUTKFrameList.begin(); it != m_mainTUTKFrameList.end();)
		{
			if (FRAME_TYPE_VIDEO_FORMAT != (*it)->GetFrameInfo().frameType && FRAME_TYPE_AUDIO_FORMAT != (*it)->GetFrameInfo().frameType)
			{
				//音频和视频格式帧不清除
				(*it)->Dec();
				it = m_mainTUTKFrameList.erase(it);
			}
			else
			{
				it++;
			}
		}
#endif
#ifdef __API_SERVER__
		for (std::list<CFrameData *>::iterator it = m_mainDevAPIFrameList.begin(); it != m_mainDevAPIFrameList.end();)
		{
			if (FRAME_TYPE_VIDEO_FORMAT != (*it)->GetFrameInfo().frameType && FRAME_TYPE_AUDIO_FORMAT != (*it)->GetFrameInfo().frameType)
			{
				//音频和视频格式帧不清除
				(*it)->Dec();
				it = m_mainDevAPIFrameList.erase(it);
			}
			else
			{
				it++;
			}
		}
#endif

		m_mainFrameListLock.UnLock();
	}
	
	if (LIVE_FRAME_SECOND_STREAM & stream)
	{
		m_subFrameListLock.Lock();

		for (std::list<CFrameData *>::iterator it = m_subFrameInfoList.begin(); it != m_subFrameInfoList.end();)
		{
			if (FRAME_TYPE_VIDEO_FORMAT != (*it)->GetFrameInfo().frameType && FRAME_TYPE_AUDIO_FORMAT != (*it)->GetFrameInfo().frameType)
			{
				//音频和视频格式帧不清除
				(*it)->Dec();
				it = m_subFrameInfoList.erase(it);
			}
			else
			{
				it++;
			}
		}

#ifdef __STREAM_SERVER_RTP__
		for (std::list<CFrameData *>::iterator it = m_subRTPFrameList.begin(); it != m_subRTPFrameList.end();)
		{
			if (FRAME_TYPE_VIDEO_FORMAT != (*it)->GetFrameInfo().frameType && FRAME_TYPE_AUDIO_FORMAT != (*it)->GetFrameInfo().frameType)
			{
				//音频和视频格式帧不清除
				(*it)->Dec();
				it = m_subRTPFrameList.erase(it);
			}
			else
			{
				it++;
			}
		}
#endif
#ifdef __TUTK_SERVER__
		for (std::list<CFrameData *>::iterator it = m_subTUTKFrameList.begin(); it != m_subTUTKFrameList.end();)
		{
			if (FRAME_TYPE_VIDEO_FORMAT != (*it)->GetFrameInfo().frameType && FRAME_TYPE_AUDIO_FORMAT != (*it)->GetFrameInfo().frameType)
			{
				//音频和视频格式帧不清除
				(*it)->Dec();
				it = m_subTUTKFrameList.erase(it);
			}
			else
			{
				it++;
			}
		}
#endif
#ifdef __API_SERVER__
		for (std::list<CFrameData *>::iterator it = m_subDevAPIFrameList.begin(); it != m_subDevAPIFrameList.end();)
		{
			if (FRAME_TYPE_VIDEO_FORMAT != (*it)->GetFrameInfo().frameType && FRAME_TYPE_AUDIO_FORMAT != (*it)->GetFrameInfo().frameType)
			{
				//音频和视频格式帧不清除
				(*it)->Dec();
				it = m_subDevAPIFrameList.erase(it);
			}
			else
			{
				it++;
			}
		}
#endif

		m_subFrameListLock.UnLock();
	}

	if (LIVE_FRAME_JPEG & stream)
	{
		m_jpegFrameListLock.Lock();

		std::list<CFrameData *>::iterator it;
		while (m_jpegFrameInfoList.size() > 0)
		{
			it = m_jpegFrameInfoList.begin();
			(*it)->Dec();
			m_jpegFrameInfoList.pop_front();				
		}
#ifdef __STREAM_SERVER_RTP__
		while (m_jpegRTPFrameList.size() > 0)
		{
			it = m_jpegRTPFrameList.begin();
			(*it)->Dec();
			m_jpegRTPFrameList.pop_front();				
		}
#endif
// #ifdef __TUTK_SERVER__
// 		while (m_jpegTUTKFrameList.size() > 0)
// 		{
// 			it = m_jpegTUTKFrameList.begin();
// 			(*it)->Dec();
// 			m_jpegTUTKFrameList.pop_front();				
// 		}
// #endif
#ifdef __API_SERVER__
		while (m_jpegDevAPIFrameList.size() > 0)
		{
			it = m_jpegDevAPIFrameList.begin();
			(*it)->Dec();
			m_jpegDevAPIFrameList.pop_front();				
		}
#endif
#ifdef __SUPPORT_WAP__
		while (m_jpegFrameInfoForWebList.size() > 0)
		{
			it = m_jpegFrameInfoForWebList.begin();
			(*it)->Dec();
			m_jpegFrameInfoForWebList.pop_front();				
		}
#endif
		m_jpegFrameListLock.UnLock();
	}

	if(LIVE_FRAME_TALK & stream)
	{
		m_talkFrameListLock.Lock();

		std::list<CFrameData *>::iterator it;
		while(m_talkFrameInfoList.size() > 0)
		{
			it = m_talkFrameInfoList.begin();
			(*it)->Dec();
			m_talkFrameInfoList.pop_front();
		}
#ifdef __STREAM_SERVER_RTP__
		while(m_talkRTPFrameList.size() > 0)
		{
			it = m_talkRTPFrameList.begin();
			(*it)->Dec();
			m_talkRTPFrameList.pop_front();
		}
#endif
// #ifdef __TUTK_SERVER__
// 		while(m_talkTUTKFrameList.size() > 0)
// 		{
// 			it = m_talkTUTKFrameList.begin();
// 			(*it)->Dec();
// 			m_talkTUTKFrameList.pop_front();
// 		}
// #endif
 #ifdef __API_SERVER__
 		while(m_talkDevAPIFrameList.size() > 0)
 		{
 			it = m_talkDevAPIFrameList.begin();
 			(*it)->Dec();
 			m_talkDevAPIFrameList.pop_front();
 		}
 #endif
		m_talkFrameListLock.UnLock();
	}
}

unsigned long CMessageMan::GetFrameNum(unsigned long stream, long netServerType /* = TVT_NET_SERVER*/)
{
	unsigned long retNum = 0;
	if (LIVE_FRAME_FIRST_STREAM & stream)
	{
		
	}

	if (LIVE_FRAME_SECOND_STREAM & stream)
	{
		m_subFrameListLock.Lock();
		retNum = m_subFrameInfoList.size();
		m_subFrameListLock.UnLock();
	}

	if (LIVE_FRAME_JPEG & stream)
	{
		
	}

	if(LIVE_FRAME_TALK & stream)
	{
		
	}

	return retNum;

}

bool CMessageMan::IsFrameEmpty()
{
	bool bIsFrameEmpty = true;

	m_mainFrameListLock.Lock();
	if (!m_mainFrameInfoList.empty())
	{
		bIsFrameEmpty = false;
	}
	m_mainFrameListLock.UnLock();

	if (bIsFrameEmpty)
	{
		m_subFrameListLock.Lock();
		if (!m_subFrameInfoList.empty())
		{
			bIsFrameEmpty = false;
		}
		m_subFrameListLock.UnLock();
	}

	return bIsFrameEmpty;
}


void CMessageMan::PutPBFrame(CFrameData * const pFrameData)
{
	m_pbFrameListLock.Lock();
	pFrameData->Add();
	m_pbFrameInfoList.push_back(pFrameData);
#ifdef __STREAM_SERVER_RTP__
	pFrameData->Add();
	m_pbRTPFrameList.push_back(pFrameData);
#endif
// #ifdef __TUTK_SERVER__
// 	pFrameData->Add();
// 	m_pbTUTKFrameList.push_back(pFrameData);
// #endif
#ifdef __API_SERVER__
	pFrameData->Add();
	m_pbDevAPIFrameList.push_back(pFrameData);
#endif
	m_pbFrameListLock.UnLock();
}

bool CMessageMan::GetPBFrame(CFrameData **ppFrameData, long netServerType /*=TVT_NET_SERVER*/)
{
	m_pbFrameListLock.Lock();

	std::list<CFrameData *> *pFrameList = NULL;

	if(TVT_NET_SERVER == netServerType)
	{
		pFrameList = &m_pbFrameInfoList;
	}

#ifdef __STREAM_SERVER_RTP__
	if(RTP_NET_SERVER == netServerType)
	{
		pFrameList = &m_pbRTPFrameList;
	}
#endif
// #ifdef __TUTK_SERVER__
// 	if(TUTK_NET_SERVER == netServerType)
// 	{
// 		pFrameList = &m_pbTUTKFrameList;
// 	}
// #endif
#ifdef __API_SERVER__
	if(API_SERVER == netServerType)
	{
		pFrameList = &m_pbDevAPIFrameList;
	}
#endif
	if ((NULL != pFrameList) && !pFrameList->empty())
	{
		*ppFrameData = pFrameList->front();
		pFrameList->pop_front();
		m_pbFrameListLock.UnLock();

		(*ppFrameData)->Add();
		(*ppFrameData)->Dec();

		return true;
	}
	else
	{
		m_pbFrameListLock.UnLock();
		return false;
	}
}

void CMessageMan::ClearPBFrame(long clientID, unsigned long streamID)
{
	m_pbFrameListLock.Lock();

	if (m_pbFrameInfoList.size() > 0)
	{
		std::list<CFrameData *>::iterator it;
		for (it = m_pbFrameInfoList.begin(); it != m_pbFrameInfoList.end();)
		{
			if ((streamID == (*it)->GetFrameInfo().streamID) && ((*it)->GetDataId() == clientID))
			{
				(*it)->Dec();
				m_pbFrameInfoList.erase(it ++);//此处it作为参数传递给了erase，然后自增，接着才是erase的执行。
			}
			else
			{
				++ it;
			}
		}
	}

	m_pbFrameListLock.UnLock();
}

void CMessageMan::PutTalkWayFrame(const FRAME_INFO_EX & frameInfo)
{
	m_talkWayFrameListLock.Lock();
	m_talkWayFrameList.push_back(frameInfo);
	m_talkWayFrameListLock.UnLock();
}

bool CMessageMan::GetTalkWayFrame(FRAME_INFO_EX & frameInfo, long netServerType /* = TVT_NET_SERVER*/)
{
	assert (NULL != frameInfo.pData);

	m_talkWayFrameListLock.Lock();

	if (0 < m_talkWayFrameList.size())
	{
		std::list<FRAME_INFO_EX>::iterator it;
		it = m_talkWayFrameList.begin();

		frameInfo.frameType = (*it).frameType;
		frameInfo.length = (*it).length;
		frameInfo.time = (*it).time;

		//由于语音对讲的数据缓冲区是从SDK获取的，所以在此需要把数据
		//复制一次，然后把原来的释放掉。
		memcpy(frameInfo.pData, (*it).pData, (*it).length);
		delete [] (*it).pData;

		m_talkWayFrameList.pop_front();
		m_talkWayFrameListLock.UnLock();
		return true;
	}
	else
	{
		m_talkWayFrameListLock.UnLock();
		return false;
	}
}

void CMessageMan::ClearTalkWayFrame()
{
	m_talkWayFrameListLock.Lock();

	std::list<FRAME_INFO_EX>::iterator it;
	while (m_talkWayFrameList.size() > 0)
	{
		it = m_talkWayFrameList.begin();
		delete [] (*it).pData;
		m_talkWayFrameList.pop_front();				
	}

	m_talkWayFrameListLock.UnLock();
}

void CMessageMan::PutMsgToServer(const MESSAGE_DATA & msgData, long netServerType /* = TVT_NET_SERVER*/)
{
	assert(netServerType < NET_SERVER_NUM);

	m_toServerLock.Lock();
	m_toServerMsgList.push_back(msgData);
	if(msgData.clientID != LOCAL_CLIENT_ID)
	{
		m_toClientMsg[netServerType].clientMap[msgData.clientID] = netServerType;
	}

	m_toServerLock.UnLock();
}

void CMessageMan::PutMsgToClient(const MESSAGE_DATA & msgData)
{
	m_toClientLock.Lock();
	if(msgData.clientID != LOCAL_CLIENT_ID)
	{
		for(long i = 0; i < NET_SERVER_NUM; i++)
		{
			if(m_toClientMsg[i].clientMap.find(msgData.clientID) != m_toClientMsg[i].clientMap.end())
			{
				m_toClientMsg[i].msgList.push_back(msgData);
				break;
			}
		}
	}
	else
	{
		m_toUIMsgList.push_back(msgData);
	}
	m_toClientLock.UnLock();
}

bool CMessageMan::GetMsgFromServer(MESSAGE_DATA & msgData, bool bNetServer /* = true */, long netServer /* = TVT_NET_SERVER*/)
{	
	m_toClientLock.Lock();

	if(!bNetServer)
	{
		if(0 == m_toUIMsgList.size())
		{
			m_toClientLock.UnLock();
			return false;
		}

		msgData = m_toUIMsgList.front();
		m_toUIMsgList.pop_front();
	}
	else
	{
		if(0 == m_toClientMsg[netServer].msgList.size())
		{
			m_toClientLock.UnLock();
			return false;
		}

		msgData = m_toClientMsg[netServer].msgList.front();
		m_toClientMsg[netServer].msgList.pop_front();
	}

	m_toClientLock.UnLock();
	return true;
}

bool CMessageMan::GetMsgFromClient(MESSAGE_DATA & msgData)
{
	m_toServerLock.Lock();

	if(0 == m_toServerMsgList.size())
	{
		m_toServerLock.UnLock();
		return false;
	}

	std::list<MESSAGE_DATA>::iterator it;
	it = m_toServerMsgList.begin();
	msgData.deviceID = (*it).deviceID;
	msgData.clientID = (*it).clientID;
	msgData.pData = (*it).pData;
	msgData.dataLen = (*it).dataLen;
	m_toServerMsgList.pop_front();

	m_toServerLock.UnLock();
	return true;
}

void CMessageMan::Clear()
{
	{
		std::list<MESSAGE_DATA>::iterator it;
		m_toServerLock.Lock();

		for (it = m_toServerMsgList.begin(); it != m_toServerMsgList.end(); ++it) 
		{
			delete[] (*it).pData;
			(*it).pData = NULL;
		}
		m_toServerMsgList.clear();

		m_toServerLock.UnLock();
	}

	//
	{
		std::list<MESSAGE_DATA>::iterator it;
		m_toClientLock.Lock();

		for (it = m_toUIMsgList.begin(); it != m_toUIMsgList.end(); ++it) 
		{
			delete[] (*it).pData;
			(*it).pData = NULL;
		}
		m_toUIMsgList.clear();

		for(long i = 0; i < NET_SERVER_NUM; i++)
		{
			m_toClientMsg[i].clientMap.clear();

			for (it = m_toClientMsg[i].msgList.begin(); it != m_toClientMsg[i].msgList.end(); ++it) 
			{
				delete[] (*it).pData;
				(*it).pData = NULL;
			}
			m_toClientMsg[i].msgList.clear();
		}

		m_toClientLock.UnLock();
	}

	m_diskStatusList.clear();

	{
		m_mainFrameListLock.Lock();
		while (m_mainFrameInfoList.size() > 0)
		{
			std::list<CFrameData *>::iterator it;
			it = m_mainFrameInfoList.begin();
			(*it)->Dec();
			m_mainFrameInfoList.pop_front();
		}
#ifdef __STREAM_SERVER_RTP__
		while (m_mainRTPFrameList.size() > 0)
		{
			std::list<CFrameData *>::iterator it;
			it = m_mainRTPFrameList.begin();
			(*it)->Dec();
			m_mainRTPFrameList.pop_front();
		}
#endif
#ifdef __TUTK_SERVER__
		while (m_mainTUTKFrameList.size() > 0)
		{
			std::list<CFrameData *>::iterator it;
			it = m_mainTUTKFrameList.begin();
			(*it)->Dec();
			m_mainTUTKFrameList.pop_front();
		}
#endif
#ifdef __API_SERVER__
		while (m_mainDevAPIFrameList.size() > 0)
		{
			std::list<CFrameData *>::iterator it;
			it = m_mainDevAPIFrameList.begin();
			(*it)->Dec();
			m_mainDevAPIFrameList.pop_front();
		}
#endif
		m_mainFrameListLock.UnLock();
	}

	{
		m_subFrameListLock.Lock();
		while (m_subFrameInfoList.size() > 0)
		{
			std::list<CFrameData *>::iterator it;
			it = m_subFrameInfoList.begin();
			(*it)->Dec();
			m_subFrameInfoList.pop_front();
		}
#ifdef __STREAM_SERVER_RTP__
		while (m_subRTPFrameList.size() > 0)
		{
			std::list<CFrameData *>::iterator it;
			it = m_subRTPFrameList.begin();
			(*it)->Dec();
			m_subRTPFrameList.pop_front();
		}
#endif
#ifdef __TUTK_SERVER__
		while (m_subTUTKFrameList.size() > 0)
		{
			std::list<CFrameData *>::iterator it;
			it = m_subTUTKFrameList.begin();
			(*it)->Dec();
			m_subTUTKFrameList.pop_front();
		}
#endif
#ifdef __API_SERVER__
		while (m_subDevAPIFrameList.size() > 0)
		{
			std::list<CFrameData *>::iterator it;
			it = m_subDevAPIFrameList.begin();
			(*it)->Dec();
			m_subDevAPIFrameList.pop_front();
		}
#endif
		m_subFrameListLock.UnLock();
	}

	{
		m_jpegFrameListLock.Lock();
		while (m_jpegFrameInfoList.size() > 0)
		{
			std::list<CFrameData *>::iterator it;
			it = m_jpegFrameInfoList.begin();
			(*it)->Dec();
			m_jpegFrameInfoList.pop_front();
		}
#ifdef __STREAM_SERVER_RTP__
		while (m_jpegRTPFrameList.size() > 0)
		{
			std::list<CFrameData *>::iterator it;
			it = m_jpegRTPFrameList.begin();
			(*it)->Dec();
			m_jpegRTPFrameList.pop_front();
		}
#endif
// #ifdef __TUTK_SERVER__
// 		while (m_jpegTUTKFrameList.size() > 0)
// 		{
// 			std::list<CFrameData *>::iterator it;
// 			it = m_jpegTUTKFrameList.begin();
// 			(*it)->Dec();
// 			m_jpegTUTKFrameList.pop_front();
// 		}
// #endif
#ifdef __API_SERVER__
		while (m_jpegDevAPIFrameList.size() > 0)
		{
			std::list<CFrameData *>::iterator it;
			it = m_jpegDevAPIFrameList.begin();
			(*it)->Dec();
			m_jpegDevAPIFrameList.pop_front();
		}
#endif
		m_jpegFrameListLock.UnLock();
	}

	{
		m_talkFrameListLock.Lock();
		std::list<CFrameData *>::iterator it;
		while(m_talkFrameInfoList.size() > 0)
		{
			it = m_talkFrameInfoList.begin();
			(*it)->Dec();
			m_talkFrameInfoList.pop_front();
		}
#ifdef __STREAM_SERVER_RTP__
		while(m_talkRTPFrameList.size() > 0)
		{
			it = m_talkRTPFrameList.begin();
			(*it)->Dec();
			m_talkRTPFrameList.pop_front();
		}
#endif
// #ifdef __TUTK_SERVER__
// 		while(m_talkTUTKFrameList.size() > 0)
// 		{
// 			it = m_talkTUTKFrameList.begin();
// 			(*it)->Dec();
// 			m_talkTUTKFrameList.pop_front();
// 		}
// #endif
#ifdef __API_SERVER__
		while(m_talkDevAPIFrameList.size() > 0)
		{
			it = m_talkDevAPIFrameList.begin();
			(*it)->Dec();
			m_talkDevAPIFrameList.pop_front();
		}
#endif
		m_talkFrameListLock.UnLock();
	}

	{
		m_pbFrameListLock.Lock();
		while (m_pbFrameInfoList.size() > 0)
		{
			std::list<CFrameData *>::iterator it;
			it = m_pbFrameInfoList.begin();
			(*it)->Dec();
			m_pbFrameInfoList.pop_front();
		}
		m_pbFrameListLock.UnLock();
	}

	{
		m_talkWayFrameListLock.Lock();

		std::list<FRAME_INFO_EX>::iterator it;
		while (m_talkWayFrameList.size() > 0)
		{
			it = m_talkWayFrameList.begin();
			delete [] (*it).pData;
			m_talkWayFrameList.pop_front();				
		}

		m_talkWayFrameListLock.UnLock();
	}
	{
		m_talkWayHxhtListLock.Lock();

		std::list<FRAME_INFO_EX>::iterator it;
		while (m_talkWayHxhtList.size() > 0)
		{
			it = m_talkWayHxhtList.begin();
			delete [] (*it).pData;
			m_talkWayHxhtList.pop_front();				
		}

		m_talkWayHxhtListLock.UnLock();

	}
}

void CMessageMan::CleanMsg(unsigned long clientID)
{
	m_toServerLock.Lock();
	std::list<MESSAGE_DATA>::iterator it;
	for (it = m_toServerMsgList.begin(); it != m_toServerMsgList.end();) 
	{
		MESSAGE_DATA & msgData = *it;
		if (msgData.clientID == clientID) 
		{
			delete[] msgData.pData;
			msgData.pData = NULL;
			it = m_toServerMsgList.erase(it);
		}
		else
		{
			++it;
		}
	}

	m_toServerLock.UnLock();
}


void CMessageMan::PostMsgDataToServer(unsigned long clientID, unsigned long deviceID, long cmdType, unsigned char *pData, unsigned long dataLen)
{
	assert(clientID == LOCAL_CLIENT_ID);
	PACKCMD cmd;
	MESSAGE_DATA msgData;

	cmd.cmdType	= cmdType;
	cmd.cmdVer	= NET_PROTOCOL_VER;
	cmd.dataLen	= dataLen;

	msgData.clientID = clientID;
	msgData.deviceID = deviceID;
	msgData.dataLen = sizeof(PACKCMD) + dataLen;

	msgData.pData = new char [msgData.dataLen];

	memcpy(msgData.pData, &cmd, sizeof(PACKCMD));

	if ((NULL != pData) && (0 < dataLen))
	{
		memcpy(msgData.pData + sizeof(PACKCMD), pData, dataLen);
	}

	PutMsgToServer(msgData);

}

void CMessageMan::SendMsgDataToServer(unsigned long clientID, unsigned long deviceID, long cmdType, unsigned char *pData, unsigned long dataLen, MESSAGE_DATA &msgData)
{
	assert(clientID == LOCAL_CLIENT_ID);
	PACKCMD cmd;

	cmd.cmdType	= cmdType;
	cmd.cmdVer	= NET_PROTOCOL_VER;
	cmd.dataLen	= dataLen;

	msgData.clientID = clientID;
	msgData.deviceID = deviceID;
	msgData.dataLen = sizeof(PACKCMD) + dataLen;

	msgData.pData = new char [msgData.dataLen];

	memcpy(msgData.pData, &cmd, sizeof(PACKCMD));

	if ((NULL != pData) && (0 < dataLen))
	{
		memcpy(msgData.pData + sizeof(PACKCMD), pData, dataLen);
	}

#if (__CHIP3521__) || defined(__CHIPGM__) || (defined(__CHIP3531__) && defined(__HISI_SDK_0BX__))
	if (LOCAL_CLIENT_ID == clientID)
	{
		m_localClientLock.Lock();
	}
#endif

	PutMsgToServer(msgData);

	unsigned long time = GetCurrTime32();
	bool bShowWaitting = false;

	while (true)
	{
		//超过两秒钟，则显示等待标记，显示后不再显示。
		if ( !((CMD_BASENUM_PTZ < cmdType) && (cmdType < CMD_END_PTZ)) && \
			!((CMD_REQUEST_LOCAL_CLIENT_PLAYBACK_PAUSE <= cmdType) && (cmdType <= CMD_REQUEST_LOCAL_CLIENT_PLAYBACK_CAM_NAME))&&\
			!((CMD_REQUEST_SET_IMAGE_MIRROR <= cmdType)&&(cmdType <= CMD_REQUEST_GET_IMAGE_ROI)) )
		{
			if (!bShowWaitting && (time <= (GetCurrTime32() - 1)))
			{
				//
				m_pFBInterface->WaitProc(true);

				bShowWaitting = true;
			}
		}
		
		m_toClientLock.Lock();

		if(0 == m_toUIMsgList.size())
		{
			m_toClientLock.UnLock();
			PUB_Sleep(10);
			continue;
		}
		else
		{
			bool bGet = false;
			std::list<MESSAGE_DATA>::iterator it;
			for (it = m_toUIMsgList.begin(); it != m_toUIMsgList.end(); ++it) 
			{
				if ((clientID == ((*it).clientID)) && (deviceID == ((*it).deviceID)))
				{
					msgData.deviceID = (*it).deviceID;
					msgData.clientID = (*it).clientID;
					msgData.pData = (*it).pData;
					msgData.dataLen = (*it).dataLen;

					m_toUIMsgList.erase(it);
					bGet = true;
					break;
				}
			}
			m_toClientLock.UnLock();

			if(bGet)
			{
				break;
			}
		}
	}

	if (bShowWaitting)
	{
		m_pFBInterface->WaitProc(false);
	}

#if (__CHIP3521__) || defined(__CHIPGM__) || (defined(__CHIP3531__) && defined(__HISI_SDK_0BX__))
	if (LOCAL_CLIENT_ID == clientID)
	{
		m_localClientLock.UnLock();
	}
#endif
}

void CMessageMan::ReleaseMsgBuff(MESSAGE_DATA & msgData)
{
	if (msgData.pData != NULL)
	{
		delete [] msgData.pData;
		msgData.pData = NULL;
	}

	memset(&msgData, 0, sizeof(MESSAGE_DATA));
}

void CMessageMan::RefreshNetDeviceStatus(ULONGLONG ullStatus)
{
	if(m_ullNetDeviceStatus != ullStatus)
	{
		m_bRefreshLive = true;
	}
	m_ullNetDeviceStatus = ullStatus;
}

void CMessageMan::SetNetDeviceStatus(bool bOnLine, unsigned long channel)
{
	static ULONGLONG ullStatus;
	if (bOnLine)
	{
		m_ullNetDeviceStatus &= ~((ULONGLONG)0x01 << channel);
	}
	else
	{
		m_ullNetDeviceStatus |= ((ULONGLONG)0x01 << channel);
	}
	if(ullStatus != m_ullNetDeviceStatus)
	{
		ullStatus = m_ullNetDeviceStatus;
		m_bRefreshLive = true;
	}
}

ULONGLONG CMessageMan::GetNetDeviceStatus()
{
	return m_ullNetDeviceStatus;
}

void CMessageMan::RefreshPoeOnlineStatus(ULONGLONG ullStatus)
{
	if(m_ullPoeOnlineStatus != ullStatus)
	{
		m_bRefreshLive = true;
	}
	m_ullPoeOnlineStatus = ullStatus;
}

void CMessageMan::SetNetPoeDeviceStatus(bool bOnLine, unsigned long channel)
{
	static ULONGLONG ullStatus;
	if (bOnLine)
	{
		m_ullPoeOnlineStatus &= ~((ULONGLONG)0x01 << channel);
	}
	else
	{
		m_ullPoeOnlineStatus |= ((ULONGLONG)0x01 << channel);
	}
	if(ullStatus != m_ullPoeOnlineStatus)
	{
		ullStatus = m_ullPoeOnlineStatus;
		m_bRefreshLive = true;
	}
}

ULONGLONG CMessageMan::GetPoeOnlineStatus()
{
	return m_ullPoeOnlineStatus;
}

void CMessageMan::RefreshFormatProgress(int allDiskNum, int curDiskNum, int precent)
{
	m_allDiskNumForFormat = allDiskNum;
	m_curDiskNumForFormat = curDiskNum;
	m_precentOfFormat     = precent;
}


void CMessageMan::GetFormatProgress(int *pAllDiskNum, int *pCurDiskNum, int *precent)
{
	*pAllDiskNum = m_allDiskNumForFormat;
	*pCurDiskNum = m_curDiskNumForFormat;
	*precent     = m_precentOfFormat;
}


void CMessageMan::RefreshBackupProgress(int precent)
{
	m_precentOfBackup = precent;
}

void CMessageMan::GetBackupProgress(int *precent)
{
	*precent = m_precentOfBackup;
}

unsigned long CMessageMan::AllocNetClientID()
{
	unsigned long newClientID = 0;
	m_clientIDLock.Lock();
	newClientID = m_sNextClientID++;
	m_clientIDLock.UnLock();

	return newClientID;
}

/************************************zxx新加语音对讲参数*********************************************/
void CMessageMan::PutHxhtTalkWayFrame(const FRAME_INFO_EX & frameInfo)
{
	m_talkWayHxhtListLock.Lock();
	m_talkWayHxhtList.push_back(frameInfo);
	m_talkWayHxhtListLock.UnLock();
}

bool CMessageMan::GetHxhtTalkWayFrame(FRAME_INFO_EX & frameInfo)
{
	assert (NULL != frameInfo.pData);

	m_talkWayHxhtListLock.Lock();

	if (0 < m_talkWayHxhtList.size())
	{
		std::list<FRAME_INFO_EX>::iterator it;
		it = m_talkWayHxhtList.begin();//从头取

		frameInfo.frameType = (*it).frameType;
		frameInfo.length = (*it).length;
		frameInfo.time = (*it).time;

		//由于语音对讲的数据缓冲区是从SDK获取的，所以在此需要把数据
		//复制一次，然后把原来的释放掉。
		memcpy(frameInfo.pData, (*it).pData, (*it).length);

		delete [] (*it).pData;

		m_talkWayHxhtList.pop_front();
		m_talkWayHxhtListLock.UnLock();
		return true;
	}
	else
	{
		m_talkWayHxhtListLock.UnLock();
		return false;
	}

	return true;


}

void CMessageMan::ClearHxhtTalkWayFrame()
{
	m_talkWayHxhtListLock.Lock();

	std::list<FRAME_INFO_EX>::iterator it;
	while (m_talkWayHxhtList.size() > 0)
	{
		it = m_talkWayHxhtList.begin();
		delete [] (*it).pData;
		m_talkWayHxhtList.pop_front();				
	}

	m_talkWayHxhtListLock.UnLock();
}

#ifdef __SUPPORT_OSDTWOLINES__
void CMessageMan::SetLiveCamPos(unsigned char chnn, unsigned long x, unsigned long y)
{
	assert (chnn < m_videoInputNum);

	m_pCamPoint[chnn].x = x;
	m_pCamPoint[chnn].y = y;
}

void CMessageMan::GetLiveCamPos(unsigned char chnn, unsigned long& x, unsigned long& y)
{
	assert (chnn < m_videoInputNum);

	x = m_pCamPoint[chnn].x;
	y = m_pCamPoint[chnn].y;
}
#endif
void CMessageMan::RefreshOSDName(unsigned long chnn)
{
	if (NULL != m_pMainFrame)
	{
		CMainFrame * pMainFrame = (CMainFrame*)m_pMainFrame;
		pMainFrame->RefreshOSDName(chnn);
	}
}


