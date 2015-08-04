/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : 袁石维
** Date         : 2009-10-15
** Name         : AlarmMan.h
** Version      : 1.0
** Description  :
** Modify Record:
1:
***********************************************************************/
#ifndef _ALARM_MAN_H_
#define _ALARM_MAN_H_

#include "SensorInMan.h"
#include "MotionMan.h"
#include "VideoLossMan.h"
#include "OtherAlarmMan.h"
#include "AlarmOutMan.h"
#include "LocalDevice.h"
#include "MessageMan.h"
#include "RecordMan.h"
#include "LogMan.h"
#ifdef __CUSTOM_YUANZHENG__
#include "YuanZhengClinet.h"
#endif
#include "ExceptionAlarmMan.h"
#ifdef __MOBILE_DVR__
#include "SpeedMan.h"
#endif

#ifdef __DVR_ULTIMATE__
#include "NetDeviceManager.h"
#endif

class CAlarmMan
{
public:
	static CAlarmMan * Instance();
	virtual ~CAlarmMan();

	//////////////////////////////////////////////////////////////////////////
#ifdef __MOBILE_DVR__
	bool Initial(CMotionMan *pMotionMan, CSensorInMan *pSensorInMan, CVideoLossMan *pVideoLossMan, COtherAlarmMan *pOtherAlarm, CAlarmOutMan *pAlarmOutMan, CLogMan *pLogMan, CMessageMan *pMsgMan,CSpeedMan *pSpeedMan);
#else
	bool Initial(CMotionMan *pMotionMan, CSensorInMan *pSensorInMan, CVideoLossMan *pVideoLossMan, COtherAlarmMan *pOtherAlarm, CAlarmOutMan *pAlarmOutMan, CLogMan *pLogMan, CMessageMan *pMsgMan);
#endif	
	void Quit();

	bool Start();
	void Stop();
	//////////////////////////////////////////////////////////////////////////
protected:
private:
	CAlarmMan();

	static	RESULT WINAPI DetectThread(LPVOID lpParameter);
	void DetectProc();

	static	RESULT WINAPI TimerThread(LPVOID lpParameter);
	void Timer();
	//////////////////////////////////////////////////////////////////////////
	bool			m_bDetect;					//
	PUB_thread_t	m_detect_ID;				//线程ID
	bool			m_bTimer;					//
	PUB_thread_t	m_timer_ID;				//线程ID

	CMotionMan		*m_pMotion;
	CSensorInMan	*m_pSensorIn;
	CVideoLossMan	*m_pVideoLoss;
	COtherAlarmMan	*m_pOtherAlarm;
	CExceptionAlarmMan *m_pExceptionAlarm;
	CAlarmOutMan	*m_pAlarmOut;
	CLocalDevice	*m_pLocalDevice;
	CLogMan         *m_pLogMan;
	CMessageMan     *m_pMsgMan;

#ifdef __MOBILE_DVR__
	CSpeedMan		*m_pSpeedMan;
#endif

#ifdef __DVR_ULTIMATE__
	CNetDeviceManager *m_pNetDeviceMan;
#endif
};

#endif //_ALARM_MAN_H_
