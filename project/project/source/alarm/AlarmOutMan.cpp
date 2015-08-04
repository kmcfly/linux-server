
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

#include "AlarmOutMan.h"
#include "ConfigEx.h"

#define BUUZER_ID(num) (num -1)

CAlarmOutMan::CAlarmOutMan():m_chnnNum (0), m_pTimer(NULL), m_pLocalDevice(NULL)
{
	memset(m_alarmOutCount, 0, sizeof(unsigned long) * 32);
}

CAlarmOutMan::~CAlarmOutMan()
{
}

bool CAlarmOutMan::Initial(unsigned char chnnNum)
{  
	m_chnnNum = chnnNum + 1;//���ӵ�һ��ΪBuzzer����Դ��

	m_pAlarmOutStatus	= new int [m_chnnNum];
	m_pAlarmOutAttrib	= new ALARMOUT_ATTRIB [m_chnnNum];

	memset(m_pAlarmOutStatus, 0, (m_chnnNum * sizeof(int)));
	memset(m_pAlarmOutAttrib, 0, (m_chnnNum * sizeof(ALARMOUT_ATTRIB)));

	m_pLocalDevice = CLocalDevice::Instance();

	for (char i=0; i<m_chnnNum; ++i)
	{
		m_pAlarmOutAttrib[i].bOpen	= true;//����û�����ø��Ĭ�϶��򿪡�
		m_pAlarmOutAttrib[i].timeType = DVR_TIMER_SENSOR_OUT;
		KillAlarmOut(i);
	}
	m_pAlarmOutAttrib[BUUZER_ID(m_chnnNum)].timeType = DVR_TIMER_BUZZER;

	m_pTimer = CDVRTimer::Instance();

	m_pSchedule = new CSchedule[chnnNum];

	return true;
}

void CAlarmOutMan::Quit()
{
	for (unsigned char i = 0; i < m_chnnNum; i++)
	{
		if (ALARM_OUT_ON == m_pAlarmOutStatus[i])
		{
			KillAlarmOut(i);
			if (DVR_TIMER_NO_ACTION != m_pTimer->CheckTimer(i, m_pAlarmOutAttrib[i].timeType))
			{
				m_pTimer->StopTime(i, m_pAlarmOutAttrib[i].timeType);
			}			
		}
	}


	memset(m_pAlarmOutStatus, 0, (m_chnnNum * sizeof(int)));
	memset(m_pAlarmOutAttrib, 0, (m_chnnNum * sizeof(ALARMOUT_ATTRIB)));

	m_pTimer = NULL;
	delete [] m_pAlarmOutStatus;
	delete [] m_pAlarmOutAttrib;
	delete [] m_pSchedule;
}

void CAlarmOutMan::AlarmOut(ULONGLONG chnnBits)
{
	time_t currentTime = GetCurrTime32();
	//�ֱ���ÿ���̵����ı������
	for (unsigned char i = 0; i < m_chnnNum; ++i)
	{
		if (!((chnnBits >> i) & ((ULONGLONG)1)))
		{
			//�����0���Ͳ�����
			continue;			
		}

		m_alarmOutCountLock.Lock();
		m_alarmOutCount[i] ++;
		m_alarmOutCountLock.UnLock();

		if (!m_pAlarmOutAttrib[i].bOpen)	//���豸δ�򿪣�����������û�б�Ҫ�ˡ�
		{
			continue;
		}
		
		if( ( i != BUUZER_ID( m_chnnNum)) && ( !m_pSchedule[i].CheckTime( currentTime ) ))  //��������ʱû�����ƻ�
		{
			continue;
		}

		switch (m_pAlarmOutStatus[i])
		{
		case ALARM_OUT_OFF:
			{
				//��ǰ�ǹر�״̬��������ܵ��ź�1��
				//��ô��Ҫ�����̵���,�������������ҽ���̵�������״̬
				OpenAlarmOut(i);
				m_pTimer->StartTime(i, m_pAlarmOutAttrib[i].holdTime, m_pAlarmOutAttrib[i].timeType);
			}
			break;
		case ALARM_OUT_ON:
			{
				//��ǰ�ǿ���״̬��������ܵ��ź�1��
				//��ôҪ���¼�ʱ
#if defined(__CUSTOM_TW01__) || defined(__CUSTOM_TW01TS__) || defined(__CUSTOM_CBC__)
				OpenAlarmOut(i);
#endif

//#if defined(__CUSTOM_ZNV__)
				//OpenAlarmOut(i);
//#endif

				m_pTimer->ResetTime(i, m_pAlarmOutAttrib[i].holdTime, m_pAlarmOutAttrib[i].timeType);
			}
			break;
		}
	}
}

void CAlarmOutMan::CloseAlarmOut(ULONGLONG chnnBits)
{
	for (unsigned char i = 0; i < m_chnnNum; ++i)
	{
		m_alarmOutCountLock.Lock();
		m_alarmOutCount[i] --;

		if (0 >= m_alarmOutCount[i])
		{
			m_alarmOutCount[i] = 0;
			if (ALARM_OUT_ON == m_pAlarmOutStatus[i])
			{
				KillAlarmOut(i);
				m_pTimer->StopTime(i, m_pAlarmOutAttrib[i].timeType);
			}
		}
		m_alarmOutCountLock.UnLock();
	}
}

void CAlarmOutMan::OpenAlarmOut(unsigned char chnn)
{
	if (m_pAlarmOutAttrib[chnn].bOpen)
	{
		if (chnn != BUUZER_ID(m_chnnNum))
		{
			m_pLocalDevice->SetSensorOutStatus(chnn, true);
			m_pAlarmOutStatus[chnn] = ALARM_OUT_ON;
		}	
		else 
		{
			m_pLocalDevice->SetBuzzerStatus(true);
			m_pAlarmOutStatus[BUUZER_ID(m_chnnNum)] = ALARM_OUT_ON;
		}	
	}
}

void CAlarmOutMan::KillAlarmOut(unsigned char chnn)
{
	if (chnn != BUUZER_ID(m_chnnNum))
	{
		m_pLocalDevice->SetSensorOutStatus(chnn, false);
		m_pAlarmOutStatus[chnn] = ALARM_OUT_OFF;
	}	
	else 
	{
		m_pLocalDevice->SetBuzzerStatus(false);
		m_pAlarmOutStatus[BUUZER_ID(m_chnnNum)] = ALARM_OUT_OFF;
	}
}

bool CAlarmOutMan::KillAllAlarmOut()
{
	//����ֵ�����ж��Ƿ������Ľ��й��رն���

	bool bKill = false;
	for (unsigned char i = 0; i < m_chnnNum; ++i)
	{
		if (ALARM_OUT_ON == m_pAlarmOutStatus[i])
		{
			KillAlarmOut(i);
			m_pTimer->StopTime(i, m_pAlarmOutAttrib[i].timeType);
			bKill = true;		
		}
	}

	return bKill;
}

void CAlarmOutMan::Timer()
{
	for (unsigned char i = 0; i < m_chnnNum; i++)
	{				
		if (ALARM_OUT_OFF == m_pAlarmOutStatus[i])//�ر�״̬����Ҫ����
		{
			continue;
		}

		if (DVR_TIMER_TIMER_END == m_pTimer->CheckTimer(i, m_pAlarmOutAttrib[i].timeType))
		{			
			KillAlarmOut(i);
			m_pAlarmOutStatus[i] = ALARM_OUT_OFF;
			m_pTimer->StopTime(i, m_pAlarmOutAttrib[i].timeType);
		}
	}	
}

//�������µı������������Ϣ
void CAlarmOutMan::SetEnable(bool bEnable, unsigned char chnn)
{
	assert (chnn < m_chnnNum);

	m_pAlarmOutAttrib[chnn].bOpen = bEnable;

	//ִ�йرն���������Ժ�����Ҫ�ڴ��������Ļ������ٴ򿪿�����ʱ��Ӧ�Ļ����ټ��뿪��������
	if (!m_pAlarmOutAttrib[chnn].bOpen && (ALARM_OUT_ON == m_pAlarmOutStatus[chnn]))
	{
		KillAlarmOut(chnn);
		m_pAlarmOutStatus[chnn] = ALARM_OUT_OFF;
		if (DVR_TIMER_NO_ACTION != m_pTimer->CheckTimer(chnn, m_pAlarmOutAttrib[chnn].timeType))
		{
			m_pTimer->StopTime(chnn, m_pAlarmOutAttrib[chnn].timeType);
		}	
	}
}

void CAlarmOutMan::SetAllEnable(bool bEnable)
{
	for (int chnn = 0; chnn < m_chnnNum; ++chnn)
	{
		m_pAlarmOutAttrib[chnn].bOpen = bEnable;

		//ִ�йرն���������Ժ�����Ҫ�ڴ��������Ļ������ٴ򿪿�����ʱ��Ӧ�Ļ����ټ��뿪��������
		if (!m_pAlarmOutAttrib[chnn].bOpen && (ALARM_OUT_ON == m_pAlarmOutStatus[chnn]))
		{
			KillAlarmOut(chnn);
			m_pAlarmOutStatus[chnn] = ALARM_OUT_OFF;
			if (DVR_TIMER_NO_ACTION != m_pTimer->CheckTimer(chnn, m_pAlarmOutAttrib[chnn].timeType))
			{
				m_pTimer->StopTime(chnn, m_pAlarmOutAttrib[chnn].timeType);
			}	
		}
	}	
}

void CAlarmOutMan::SetBuzzerEnable(bool bEnable)
{
	m_pAlarmOutAttrib[BUUZER_ID(m_chnnNum)].bOpen = bEnable;

	if (!bEnable && (ALARM_OUT_ON == m_pAlarmOutStatus[BUUZER_ID(m_chnnNum)]))
	{
		KillAlarmOut(BUUZER_ID(m_chnnNum));
		m_pAlarmOutStatus[BUUZER_ID(m_chnnNum)] = ALARM_OUT_OFF;
		if (DVR_TIMER_NO_ACTION != m_pTimer->CheckTimer(BUUZER_ID(m_chnnNum), m_pAlarmOutAttrib[BUUZER_ID(m_chnnNum)].timeType))
		{
			m_pTimer->StopTime(BUUZER_ID(m_chnnNum), m_pAlarmOutAttrib[BUUZER_ID(m_chnnNum)].timeType);
		}
	}
}

//���bAll == true��������chnn��Ч���������ͨ����
void CAlarmOutMan::SetHoldTime(unsigned long holdTime, unsigned char chnn, bool bAll/* = true*/)
{
	if (bAll)
	{
		for (unsigned char i=0; i<BUUZER_ID(m_chnnNum); ++i)
		{
			m_pAlarmOutAttrib[i].holdTime = holdTime;
		}
	}
	else
	{
		assert (chnn < BUUZER_ID(m_chnnNum));
		m_pAlarmOutAttrib[chnn].holdTime = holdTime;
	}
}

void CAlarmOutMan::SetBuzzerHoldTime(unsigned short holdTime)
{
	m_pAlarmOutAttrib[BUUZER_ID(m_chnnNum)].holdTime = holdTime;
}
