
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

	//设置超速单位
	void SetSpeedUnit(unsigned long SpeedUnit);
	
	//设置超速来源 (GPS/car)
	void SetSpeedSource(unsigned long SpeedSource);
	
	//设置超速值
	void SetSpeedValue(char *speedValue, unsigned int ch);

	//判断那一种超速
	BYTE CompareSpeed(void);

protected:
	void InitialStatus();

	//返回的录像类型为:超速录像
	RECORD_TYPE GetRecType();

	inline ALARM_TYPE GetAlarmType(unsigned long chnn) {return ALARM_TYPE_SPEED;};

	//超速触发的报警有:录像,大画面,云台,继电器
	void OpenAlarm(unsigned long chnn);

	//关闭已经触发的报警
	void CloseAlarm(unsigned long chnn);

private:
	static CSpeedMan *m_pInstance;

	unsigned long	m_speedUnit;
	unsigned long	m_speedSource;
	char			m_speedValue[2][13];

};

#endif 

