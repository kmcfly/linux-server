
/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : 
** Date         : 2007-11-06
** Name         : 
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/

#include "MotionMan.h"
#if defined(__SUPPORT_SNAPD1__)
#include "ShanghaiConnectMan.h"
#include "ShanghaiCmd.h"
#endif
CMotionMan::CMotionMan() : CAlarmIn()
{
	m_alarmType = PUSH_ALARM_MOTION;
}

CMotionMan::~CMotionMan()
{
#if defined(__SUPPORT_SNAPD1__)
	if(NULL != m_pAlarmUpload)
	{
		delete []m_pAlarmUpload;
		m_pAlarmUpload = NULL;
	}
#endif
}

void CMotionMan::InitialStatus()
{
	for (unsigned char i = 0; i < m_chnnNum; i++)
	{
		m_pAlarmInStatus[i] = ALARMIN_STATUS_NO;
		m_pAlarmInAttrib[i].timerType = DVR_TIMER_MOTION;
	}
#if defined(__SUPPORT_SNAPD1__)
	m_pAlarmUpload = new bool[m_chnnNum];
	memset(m_pAlarmUpload,0,sizeof(bool)*m_chnnNum);
#endif
}

void CMotionMan::DisableTrigerSelf( int alarmOutDeviceNum )
{
	for( BYTE i=0; i<m_chnnNum; i++ )
	{
		m_pAlarmInAttrib[i].toPTZType[i]   = TO_PTZ_NULL;
		m_pAlarmInAttrib[i].toPTZPreset[i] = 0xff;
		m_pAlarmInAttrib[i].toPTZCruise[i] = 0xff;
		m_pAlarmInAttrib[i].toPTZTrack[i]  = 0xff;
	}

}

RECORD_TYPE CMotionMan::GetRecType()
{
	return RECORD_TYPE_MOTION;
}

//移动检测，需要开启的报警有：触发录像，触发继电器报警
void CMotionMan::OpenAlarm(unsigned long chnn)
{
	memset(&m_pEventInfo[chnn], 0, sizeof(EVENT_LOG));
	m_pEventInfo[chnn].chnn = (unsigned short)chnn;
	m_pEventInfo[chnn].startTime = GetCurrTime32();
	m_pEventInfo[chnn].type = EVENT_TYPE_MOTION;
#if defined(__SUPPORT_SNAPD1__)
	if(!m_pAlarmUpload[chnn])
	{
		//已经触发的不再触发，没触发的去触发
		char alarmNote[256] = "GB2312:移动侦测报警";
		CShanghaiConnectMan::Instance()->GetLogCmd(DVRSYSMOTION,chnn+1,alarmNote,strlen(alarmNote),NULL,0,GetCurrTime());
		m_pAlarmUpload[chnn] = true;
	}
#endif
	Record(chnn);
	AlarmOut(chnn);
	toPTZ(chnn);
	BigView(chnn);
	SendEmail(chnn);
	SnapPicture(chnn);
	StartFtpUpdate(chnn);

#if defined(__TVT_PUSH_SOCK__)
	CPushMan::Instance()->PushData(DATA_TYPE_ALARM, &m_alarmType, sizeof(m_alarmType), chnn);
#endif
}

//移动检测，需要关闭的报警
void CMotionMan::CloseAlarm(unsigned long chnn)
{
	StopRecord(chnn);
	StopPTZ(chnn);

#if defined(__CUSTOM_ZNV__) && defined(__ZNV_CN__) && defined(__ZNV_SC__)
	CloseAlarmOut(chnn);
#endif

	m_pEventInfo[chnn].endTime = GetCurrTime32();
	m_pLogMan->WriteEventLog(m_pEventInfo[chnn]);
	memset(&m_pEventInfo[chnn], 0, sizeof(EVENT_LOG));
#if defined(__SUPPORT_SNAPD1__)
	m_pAlarmUpload[chnn] = false;
#endif
	
	StopFtpUpdate(chnn);
}

