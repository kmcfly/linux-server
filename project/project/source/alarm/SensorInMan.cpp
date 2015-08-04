
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

#include "SensorInMan.h"
#include "AlarmOutMan.h"

CSensorInMan::CSensorInMan() : CAlarmIn()
{
	m_alarmType = PUSH_ALARM_SENSOR;
}

CSensorInMan::~CSensorInMan()
{
#if defined(__SUPPORT_SNAPD1__)
	if(NULL != m_pAlarmUpload)
	{
		delete []m_pAlarmUpload;
		m_pAlarmUpload = NULL;
	}
#endif

}

void CSensorInMan::InitialStatus()
{
	m_chnnBits = 0;
	for (unsigned char i = 0; i < m_chnnNum; i++)
	{
		m_pAlarmInStatus[i] = ALARMIN_STATUS_NO;
		m_pAlarmInAttrib[i].timerType = DVR_TIMER_SENSOR_IN;
	}
#if defined(__SUPPORT_SNAPD1__)
	m_pAlarmUpload = new bool[m_chnnNum];
	memset(m_pAlarmUpload,0,sizeof(bool)*m_chnnNum);
#endif
}

void CSensorInMan::SetDeviceType(bool bNO, unsigned char chnn)
{
	if (bNO)
	{
		//�����NO����ʾ�ڸ�λ�ϵ������ź���1��Ч��0��Ч
		m_chnnBits |= ((ULONGLONG)0x01 << chnn);
	}
	else
	{
		//�������෴
		m_chnnBits &= ~((ULONGLONG)0x01 << chnn);
	}
}

void CSensorInMan::AdjustAlarmIn(ULONGLONG & chnnBits)
{
	chnnBits = (chnnBits ^ m_chnnBits);
}

RECORD_TYPE CSensorInMan::GetRecType()
{
	return RECORD_TYPE_SENSOR;
}

//��������⣬��Ҫ�����ı����У�����¼�񣬴����̵���������������̨
void CSensorInMan::OpenAlarm(unsigned long chnn)
{
	memset(&m_pEventInfo[chnn], 0, sizeof(EVENT_LOG));
	m_pEventInfo[chnn].chnn = (unsigned short)chnn;
	m_pEventInfo[chnn].startTime = GetCurrTime32();
	m_pEventInfo[chnn].type = EVENT_TYPE_SENSOR;

#if defined(__SUPPORT_SNAPD1__)
	if(!m_pAlarmUpload[chnn])
	{
		//�Ѿ������Ĳ��ٴ�����û������ȥ����
		char alarmNote[256] = "GB2312:�ⲿ��������";
		CShanghaiConnectMan::Instance()->GetLogCmd(DVROUTSIDEALARM,chnn+1,alarmNote,strlen(alarmNote),NULL,0,GetCurrTime());
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

//��������⣬��Ҫ�رյı���
void CSensorInMan::CloseAlarm(unsigned long chnn)
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



