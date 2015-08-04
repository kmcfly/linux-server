/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : YSW
** Date         : 2010-07-03
** Name         : OtherAlarmMan.cpp
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/

#include "OtherAlarmMan.h"

COtherAlarmMan::COtherAlarmMan() : CAlarmIn()
{

}

COtherAlarmMan::~COtherAlarmMan()
{

}

void COtherAlarmMan::InitialStatus()
{
	for (unsigned char i = 0; i < m_chnnNum; i++)
	{
		m_pAlarmInStatus[i] = ALARMIN_STATUS_NO;
		m_pAlarmInAttrib[i].timerType = DVR_TIMER_OTHER_ALARM;

		m_pAlarmInAttrib[i].bDetection	= true;
		m_pAlarmInAttrib[i].holdTime		= 30;
	}
}

RECORD_TYPE COtherAlarmMan::GetRecType()
{
	return RECORD_TYPE_NULL;
}

ALARM_TYPE COtherAlarmMan::GetAlarmType(unsigned long chnn)
{
	switch(chnn)
	{
	case OTHER_ALARM_TYPE_DISK_FULL:
		return ALARM_TYPE_DISK_FULL;
	case OTHER_ALARM_TYPE_IP_CONFLICT:
		return ALARM_TYPE_IP_CONFLICT;
	case OTHER_ALARM_TYPE_DISCONNET:
		return ALARM_TYPE_DISCONNET;
	case OTHER_ALARM_TYPE_DISK_WARNING:
		return ALARM_TYPE_DISK_WARNING;
	case OTHER_ALARM_TYPE_DISK_DISCONNET:
		return ALARM_TYPE_DISK_DISCONNET;
	default:
		return ALARM_TYPE_NULL;
	}
}

//������������Ҫ�����ı����У������̵����������������������������ʼ���
void COtherAlarmMan::OpenAlarm(unsigned long chnn)
{
	AlarmOut(chnn);
	SendEmail(chnn);
}

//�ƶ���⣬��Ҫ�رյı���
void COtherAlarmMan::CloseAlarm(unsigned long chnn)
{
#if defined(__CUSTOM_ZNV__) && defined(__ZNV_CN__) && defined(__ZNV_SC__)
	CloseAlarmOut(chnn);
#endif
}

