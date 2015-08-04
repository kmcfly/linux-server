
/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : 
** Date         : 2007-11-06
** Name         : 
** Version      : 1.0
** Description  : 移动检测管理类，
** Modify Record:
1:
***********************************************************************/

#ifndef _MOTIONMAN_H_
#define _MOTIONMAN_H_

#include "AlarmIn.h"

//移动检测管理类
class CMotionMan  : public  CAlarmIn
{
public:
	
	//构造函数
	CMotionMan();

	//析构函数
	virtual ~CMotionMan();
protected:
	void InitialStatus();
	void DisableTrigerSelf( int alarmOutDeviceNum );

	//该函数返回移动录像类型
	RECORD_TYPE GetRecType();

	inline ALARM_TYPE GetAlarmType(unsigned long chnn) {return ALARM_TYPE_MONTION;};

	//移动检测触发的报警有：录像，继电器
	void OpenAlarm(unsigned long chnn);

	//关闭已经触发的报警
	void CloseAlarm(unsigned long chnn);

private:
#if defined(__SUPPORT_SNAPD1__)
	bool *m_pAlarmUpload;
#endif
};

#endif


