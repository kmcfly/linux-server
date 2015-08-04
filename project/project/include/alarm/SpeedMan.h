
/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : lufen
** Date         : 2010-4-22
** Name         : 
** Version      : 1.0
** Description  :
** Modify Record:
1:
***********************************************************************/

#ifndef _SPEED_ALM_MAN_H_
#define _SPEED_ALM_MAN_H_

#include "AlarmIn.h"

class CSpeedMan  : public  CAlarmIn
{
public:

	CSpeedMan();

	virtual ~CSpeedMan();

	static CSpeedMan * Instance();

	//���ó��ٵ�λ
	void SetSpeedUnit(unsigned long SpeedUnit);
	
	//���ó�����Դ (GPS/car)
	void SetSpeedSource(unsigned long SpeedSource);
	
	//���ó���ֵ
	void SetSpeedValue(char *speedValue, unsigned int ch);

	//�ж���һ�ֳ���
	BYTE CompareSpeed(void);

protected:
	void InitialStatus();

	//���ص�¼������Ϊ:����¼��
	RECORD_TYPE GetRecType();

	inline ALARM_TYPE GetAlarmType(unsigned long chnn) {return ALARM_TYPE_SPEED;};

	//���ٴ����ı�����:¼��,����,��̨,�̵���
	void OpenAlarm(unsigned long chnn);

	//�ر��Ѿ������ı���
	void CloseAlarm(unsigned long chnn);

private:
	static CSpeedMan *m_pInstance;

	unsigned long	m_speedUnit;
	unsigned long	m_speedSource;
	char			m_speedValue[2][13];

};

#endif 

