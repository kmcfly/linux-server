
/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : 
** Date         : 2007-11-07
** Name         : 
** Version      : 1.0
** Description  : ����������࣬�����̵���������Buzzer����
** Modify Record:
1:
***********************************************************************/

#ifndef  _ALARMOUTMAN_H_
#define  _ALARMOUTMAN_H_

#include "DVRTimer.h"
#include "LocalDevice.h"
#include "Schedule.h"

//�����������
typedef struct _AlarmOut_Attrib
{
	unsigned long bOpen;     //�ñ����Ƿ��
	unsigned long holdTime;
	DVR_TIMER_TYPE timeType;
}ALARMOUT_ATTRIB;

//�������������
class CAlarmOutMan  
{
public:
	//�����ǽӿڲ���
	//���캯��
	CAlarmOutMan();

	//��������
	virtual ~CAlarmOutMan();

	//��ʼ������
	bool Initial(unsigned char chnnNum);
	//������Դ	
	void Quit();

	//����������źţ�
	//�����ڷǱ���״̬ʱ���б������ָ����򿪱�����
	//�����ڱ���״̬ʱ���б������ָ�������¼�ʱ��
	void AlarmOut(ULONGLONG chnnBits);
	void CloseAlarmOut(ULONGLONG chnnBits);
	void Timer();

	//�ر����еļ̵���
	bool KillAllAlarmOut();
	
	//�������µı������������Ϣ
	void SetEnable(bool bEnable, unsigned char chnn);
	void SetAllEnable(bool bEnable);
	void SetBuzzerEnable(bool bEnable);
	//���bAll == true��������chnn��Ч���������ͨ����
	void SetHoldTime(unsigned long holdTime, unsigned char chnn, bool bAll = true);
	void SetBuzzerHoldTime(unsigned short holdTime);
	void SetWeekSchedule( int chnn, WEEK_SCHEDULE weekSchedule ){ m_pSchedule[chnn].SetWeekSchedule( weekSchedule);}
protected:

private:
	//////////////////////////////////////////////////////////////////////////
	//������chnn���̵���
	void OpenAlarmOut(unsigned char chnn);

	//�رյ�chnn���̵���
	void KillAlarmOut(unsigned char chnn);
	//////////////////////////////////////////////////////////////////////////
	unsigned char	m_chnnNum;
	CDVRTimer		*m_pTimer;	//�����������
	CLocalDevice	*m_pLocalDevice;
	//////////////////////////////////////////////////////////////////////////
	//״̬�� 0 ���رգ���1��������
	int				*m_pAlarmOutStatus;	//�����������ͨ����״̬
	ALARMOUT_ATTRIB *m_pAlarmOutAttrib;	//������������ԣ�������̵�����Buzzer��
	//////////////////////////////////////////////////////////////////////////
	CSchedule		*m_pSchedule;
	enum
	{
		ALARM_OUT_OFF,
		ALARM_OUT_ON,
	};

	CPUB_Lock		m_alarmOutCountLock;
	int				m_alarmOutCount[32];
};

#endif 

