/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : Ԭʯά
** Date         : 2009-10-15
** Name         : AlarmMan.cpp
** Version      : 1.0
** Description  :
** Modify Record:
1:
***********************************************************************/
#include "AlarmMan.h"

CAlarmMan * CAlarmMan::Instance()
{
	static CAlarmMan s_AlarmMan;
	return &s_AlarmMan;
}

#ifdef __MOBILE_DVR__
CAlarmMan::CAlarmMan() : m_bDetect(false), m_detect_ID(PUB_THREAD_ID_NOINIT), m_pMotion(NULL), m_pSensorIn(NULL), m_pVideoLoss(NULL), \
m_pAlarmOut(NULL), m_pLocalDevice(NULL),m_pSpeedMan(NULL), m_pOtherAlarm(NULL)
{

}
#else
CAlarmMan::CAlarmMan() : m_bDetect(false), m_detect_ID(PUB_THREAD_ID_NOINIT), m_pMotion(NULL), m_pSensorIn(NULL), m_pVideoLoss(NULL), \
						m_pAlarmOut(NULL), m_pLocalDevice(NULL), m_pOtherAlarm(NULL)
{

}
#endif

CAlarmMan::~CAlarmMan()
{

}
#ifdef __MOBILE_DVR__
bool CAlarmMan::Initial(CMotionMan *pMotionMan, CSensorInMan *pSensorInMan, CVideoLossMan *pVideoLossMan, COtherAlarmMan *pOtherAlarm, CAlarmOutMan *pAlarmOutMan, CLogMan *pLogMan, CMessageMan *pMsgMan,CSpeedMan *pSpeedMan)
{
	assert (NULL != pMotionMan);
	assert (NULL != pSensorInMan);
	assert (NULL != pVideoLossMan);
	assert (NULL != pAlarmOutMan);

	m_pMotion		= pMotionMan;
	m_pSensorIn		= pSensorInMan;
	m_pVideoLoss	= pVideoLossMan;
	m_pOtherAlarm	= pOtherAlarm;
	m_pAlarmOut		= pAlarmOutMan;
	m_pLocalDevice	= CLocalDevice::Instance();
	m_pLogMan       = pLogMan;
	m_pMsgMan       = pMsgMan;
	m_pSpeedMan		= pSpeedMan;

	m_pExceptionAlarm = new CExceptionAlarmMan;

	return true;
}
#else
bool CAlarmMan::Initial(CMotionMan *pMotionMan, CSensorInMan *pSensorInMan, CVideoLossMan *pVideoLossMan, COtherAlarmMan *pOtherAlarm, CAlarmOutMan *pAlarmOutMan, CLogMan *pLogMan, CMessageMan *pMsgMan)
{
	assert (NULL != pMotionMan);
	assert (NULL != pSensorInMan);
	assert (NULL != pVideoLossMan);
	assert (NULL != pAlarmOutMan);

	m_pMotion		= pMotionMan;
	m_pSensorIn		= pSensorInMan;
	m_pVideoLoss	= pVideoLossMan;
	m_pOtherAlarm	= pOtherAlarm;
	m_pAlarmOut		= pAlarmOutMan;
	m_pLocalDevice	= CLocalDevice::Instance();
	m_pLogMan       = pLogMan;
	m_pMsgMan       = pMsgMan;

	m_pExceptionAlarm = new CExceptionAlarmMan;

#ifdef __DVR_ULTIMATE__
	m_pNetDeviceMan = CNetDeviceManager::Instance();
#endif

	return true;
}
#endif

void CAlarmMan::Quit()
{
	m_pMotion		= NULL;
	m_pSensorIn		= NULL;
	m_pVideoLoss	= NULL;
	m_pOtherAlarm	= NULL;
	m_pAlarmOut		= NULL;
	m_pLocalDevice	= NULL;
#ifdef __MOBILE_DVR__
	m_pSpeedMan		= NULL;
#endif

	if(NULL != m_pExceptionAlarm)
	{
		delete m_pExceptionAlarm;
		m_pExceptionAlarm = NULL;
	}
}

bool CAlarmMan::Start()
{
	m_detect_ID = PUB_CreateThread(DetectThread, (void *)this, &m_bDetect);
	if( PUB_CREATE_THREAD_FAIL == m_detect_ID )
	{
		printf("Initial alarm detect thread fail!");
		return false;
	}

	m_timer_ID = PUB_CreateThread(TimerThread, (void *)this, &m_bTimer);
	if( PUB_CREATE_THREAD_FAIL == m_timer_ID )
	{
		printf("Initial alarm timer thread fail!");
		return false;
	}

	return true;
}

void CAlarmMan::Stop()
{
	if (PUB_THREAD_ID_NOINIT != m_timer_ID)
	{		
		PUB_ExitThread(&m_timer_ID, &m_bTimer);
	}

	if (PUB_THREAD_ID_NOINIT != m_detect_ID)
	{		
		PUB_ExitThread(&m_detect_ID, &m_bDetect);
	}
}

//////////////////////////////////////////////////////////////////////////
RESULT WINAPI CAlarmMan::DetectThread(LPVOID lpParameter)
{
#ifdef __ENVIRONMENT_LINUX__
	pid_t pid = getpid();
	pid_t tid = syscall(__NR_gettid);
	printf("%s, %d, pid = %d, tid=%d\n", __FILE__, __LINE__, pid, tid);
#endif

	assert(NULL != lpParameter);

	CAlarmMan *pThis = reinterpret_cast<CAlarmMan *>(lpParameter);

	pThis->DetectProc();

	return 0;
}

void CAlarmMan::DetectProc()
{
	 ULONGLONG alarmIn = 0, alarmOut = 0, alarmInNet = 0;
	 PRODUCT_TYPE productTypeTemp = CProduct::Instance()->ProductType();

	int time = 0;
	int sensorTime = 0;
	while (m_bDetect)
	{
		//Motion
		alarmIn = 0, alarmOut = 0, alarmInNet = 0;
		m_pLocalDevice->GetMotionStatus(alarmIn);
#ifdef __DVR_ULTIMATE__
		m_pNetDeviceMan->GetMotionStatus(alarmInNet);
		alarmIn |= alarmInNet;
#endif
		alarmOut = m_pMotion->AlarmIn(alarmIn);
		m_pMsgMan->RefreshCHStatus(CHNN_STATUS_MOTION, alarmOut);	//lh_modify 2010.03.17
		m_pMsgMan->RefreshMotion(alarmIn);

		//Sensor
		alarmIn = 0, alarmOut = 0, alarmInNet = 0;
		if (((productTypeTemp >= TD_2704HD) && (productTypeTemp <= TD_2716XE_S))||((productTypeTemp >= TD_2704XD_S) && (productTypeTemp <= TD_2716XD_S)))
		{
			m_pLocalDevice->GetSensorInStatus(alarmIn);	
			if (sensorTime > 5)
			{
#ifdef __DVR_ULTIMATE__
				m_pNetDeviceMan->GetSensorInStatus(alarmInNet);
				alarmIn |= alarmInNet;
#endif
				alarmOut = m_pSensorIn->AlarmIn(alarmIn);
				m_pMsgMan->RefreshCHStatus(CHNN_STATUS_SENSOR_IN, alarmOut);	//lh_modify 2010.03.17
				sensorTime = 6;
			}
			else
			{
				sensorTime++;
			}
		}
		else
		{
			//
			m_pLocalDevice->GetSensorInStatus(alarmIn);	
#ifdef __DVR_ULTIMATE__
		alarmIn &= (((ULONGLONG)0x01 << CProduct::Instance()->SensorInputNum()) - 1);
			m_pNetDeviceMan->GetSensorInStatus(alarmInNet);
			alarmIn |= alarmInNet;
#endif
			alarmOut = m_pSensorIn->AlarmIn(alarmIn);
			m_pMsgMan->RefreshCHStatus(CHNN_STATUS_SENSOR_IN, alarmOut);	//lh_modify 2010.03.17
		}

		//Video loss
		alarmIn = 0, alarmOut = 0, alarmInNet = 0;
#if defined(__CHIP3515__) || defined(__TDFH__)
		time++;
		if (time > 1)
		{
			m_pLocalDevice->GetVideoLossStatus(alarmIn);
#ifdef __DVR_ULTIMATE__
			m_pNetDeviceMan->GetVideoLossStatus(alarmInNet);
			alarmIn |= alarmInNet;
#endif
			alarmOut = m_pVideoLoss->AlarmIn(alarmIn);
#ifdef __CUSTOM_YUANZHENG__
			if(alarmOut!=0)
			{
				CYuanZhengMan::Instance()->SetYZAlarm(YZ_ALARM_VIDEOLOSS,true,alarmOut);
			}
			else
			{
				CYuanZhengMan::Instance()->SetYZAlarm(YZ_ALARM_VIDEOLOSS,false,0);
			}

#endif
			m_pMsgMan->RefreshCHStatus(CHNN_STATUS_VIDEO_LOSS, alarmOut);	//lh_modify 2010.03.17
			time = 0;
		}
#else
		m_pLocalDevice->GetVideoLossStatus(alarmIn);
#ifdef __DVR_ULTIMATE__
		m_pNetDeviceMan->GetVideoLossStatus(alarmInNet);
		alarmIn |= alarmInNet;
//#ifdef __CUSTOM_SHANGHAI__
		//alarmIn |= m_pMsgMan->GetNetDeviceStatus();
//#endif
#endif
		alarmOut = m_pVideoLoss->AlarmIn(alarmIn);
#ifdef __CUSTOM_YUANZHENG__
		if(alarmOut!=0)
		{
			CYuanZhengMan::Instance()->SetYZAlarm(YZ_ALARM_VIDEOLOSS,true,alarmOut);
		}
		else
		{
			CYuanZhengMan::Instance()->SetYZAlarm(YZ_ALARM_VIDEOLOSS,false,0);
		}

#endif
		m_pMsgMan->RefreshCHStatus(CHNN_STATUS_VIDEO_LOSS, alarmOut);	//lh_modify 2010.03.17
#endif


		//
		m_pLocalDevice->GetOtherAlarm(alarmIn);
#ifdef __DVR_ULTIMATE__
		m_pNetDeviceMan->GetOtherAlarm(alarmInNet);
		alarmIn |= alarmInNet;
#endif
		alarmOut = m_pOtherAlarm->AlarmIn(alarmIn);

#ifdef __DVR_ULTIMATE__
		ULONGLONG netDisconnectStatus = m_pMsgMan->GetNetDeviceStatus();
		ULONGLONG localVideoLossStatus = (ULONGLONG)(m_pLocalDevice->GetVideoLossStatus());
		netDisconnectStatus |= localVideoLossStatus;
		m_pLocalDevice->SetLEDStatusEx(LED_STATUS_VIDEO_LOSS, netDisconnectStatus);
#endif

#ifdef __DVR_ULTIMATE__
		m_pNetDeviceMan->GetExceptionAlarm(alarmInNet);
		m_pExceptionAlarm->AlarmIn(alarmInNet);
#endif

#ifdef __MOBILE_DVR__
		byte ret = m_pSpeedMan->CompareSpeed();
		alarmOut = m_pSpeedMan->AlarmIn(ret);
		m_pMsgMan->RefreshCHStatus(CHNN_STATUS_SENSOR_IN, alarmOut);	
#endif

#if defined(__CHIP3515__) || defined(__TDFH__) || defined(__CHIPGM__) 
		PUB_Sleep(1000);
#else
		PUB_Sleep(500);
#endif
	}
}

RESULT WINAPI CAlarmMan::TimerThread(LPVOID lpParameter)
{
#ifdef __ENVIRONMENT_LINUX__
	pid_t pid = getpid();
	pid_t tid = syscall(__NR_gettid);
	printf("%s, %d, pid = %d, tid=%d\n", __FILE__, __LINE__, pid, tid);
#else
	DWORD pid = GetCurrentThreadId();
#endif

	assert(NULL != lpParameter);

	CAlarmMan *pThis = reinterpret_cast<CAlarmMan *>(lpParameter);

	pThis->Timer();

	return 0;
}

void CAlarmMan::Timer()
{
	while (m_bTimer)
	{
		//
		m_pMotion->Timer();

		m_pSensorIn->Timer();

		m_pVideoLoss->Timer();

		m_pOtherAlarm->Timer();

#ifdef __MOBILE_DVR__
		m_pSpeedMan->Timer();
#endif
		m_pAlarmOut->Timer();

#if defined(__CHIP3515__) || defined(__TDFH__)
		PUB_Sleep(1000);
#else
		PUB_Sleep(500);
#endif
	}
}

//end
