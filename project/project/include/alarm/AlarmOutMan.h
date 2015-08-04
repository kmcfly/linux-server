
/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : 
** Date         : 2007-11-07
** Name         : 
** Version      : 1.0
** Description  : 管理报警输出类，包括继电器报警和Buzzer报警
** Modify Record:
1:
***********************************************************************/

#ifndef  _ALARMOUTMAN_H_
#define  _ALARMOUTMAN_H_

#include "DVRTimer.h"
#include "LocalDevice.h"
#include "Schedule.h"

//报警输出属性
typedef struct _AlarmOut_Attrib
{
	unsigned long bOpen;     //该报警是否打开
	unsigned long holdTime;
	DVR_TIMER_TYPE timeType;
}ALARMOUT_ATTRIB;

//报警输出管理类
class CAlarmOutMan  
{
public:
	//下面是接口部分
	//构造函数
	CAlarmOutMan();

	//析构函数
	virtual ~CAlarmOutMan();

	//初始化函数
	bool Initial(unsigned char chnnNum);
	//清理资源	
	void Quit();

	//处理报警输出信号，
	//当处于非报警状态时，有报警输出指令则打开报警；
	//当处于报警状态时，有报警输出指令则重新计时。
	void AlarmOut(ULONGLONG chnnBits);
	void CloseAlarmOut(ULONGLONG chnnBits);
	void Timer();

	//关闭所有的继电器
	bool KillAllAlarmOut();
	
	//设置最新的报警输出配置信息
	void SetEnable(bool bEnable, unsigned char chnn);
	void SetAllEnable(bool bEnable);
	void SetBuzzerEnable(bool bEnable);
	//如果bAll == true成立，则chnn无效，否则代表通道。
	void SetHoldTime(unsigned long holdTime, unsigned char chnn, bool bAll = true);
	void SetBuzzerHoldTime(unsigned short holdTime);
	void SetWeekSchedule( int chnn, WEEK_SCHEDULE weekSchedule ){ m_pSchedule[chnn].SetWeekSchedule( weekSchedule);}
protected:

private:
	//////////////////////////////////////////////////////////////////////////
	//开启第chnn个继电器
	void OpenAlarmOut(unsigned char chnn);

	//关闭第chnn个继电器
	void KillAlarmOut(unsigned char chnn);
	//////////////////////////////////////////////////////////////////////////
	unsigned char	m_chnnNum;
	CDVRTimer		*m_pTimer;	//管理计数器类
	CLocalDevice	*m_pLocalDevice;
	//////////////////////////////////////////////////////////////////////////
	//状态有 0 （关闭），1（开启）
	int				*m_pAlarmOutStatus;	//报警输出各个通道的状态
	ALARMOUT_ATTRIB *m_pAlarmOutAttrib;	//报警输出的属性，（多个继电器和Buzzer）
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

