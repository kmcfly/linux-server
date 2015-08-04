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
#ifndef _EXCEPTION_ALARM_MAN_H_
#define _EXCEPTION_ALARM_MAN_H_

#include "AlarmIn.h"

//其他报警管理类
class CExceptionAlarmMan
{
public:

	//构造函数
	CExceptionAlarmMan();

	//析构函数
	virtual ~CExceptionAlarmMan();

	void AlarmIn(ULONGLONG chnnBits);
protected:
private:

	ULONGLONG			m_prevChnnBits;
};

#endif //_EXCEPTION_ALARM_MAN_H_
