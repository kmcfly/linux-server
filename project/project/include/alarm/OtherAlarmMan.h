/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : YSW
** Date         : 2010-07-03
** Name         : OtherAlarmMan.h
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/
#ifndef _OTHER_ALARM_MAN_H_
#define _OTHER_ALARM_MAN_H_

#include "AlarmIn.h"

//其他报警管理类
class COtherAlarmMan  : public  CAlarmIn
{
public:

	//构造函数
	COtherAlarmMan();

	//析构函数
	virtual ~COtherAlarmMan();
protected:
	void InitialStatus();

	//该函数返回移动录像类型
	RECORD_TYPE GetRecType();

	ALARM_TYPE GetAlarmType(unsigned long chnn);

	//其他报警触发的报警有：录像，继电器
	void OpenAlarm(unsigned long chnn);

	//关闭已经触发的报警
	void CloseAlarm(unsigned long chnn);

private:
};

#endif //_OTHER_ALARM_MAN_H_
