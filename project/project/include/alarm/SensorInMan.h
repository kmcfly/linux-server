

/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : 
** Date         : 2007-11-06
** Name         : 
** Version      : 1.0
** Description  : 传感器检测管理类，
** Modify Record:
1:
***********************************************************************/

#ifndef _SENSOR_IN_MAN_H_
#define _SENSOR_IN_MAN_H_

#include "AlarmIn.h"

#if defined(__SUPPORT_SNAPD1__)
#include "ShanghaiCmd.h"
#include "ShanghaiConnectMan.h"
#endif
//传感器检测管理类
class CSensorInMan  : public  CAlarmIn
{
public:
	
	//构造函数
	CSensorInMan();

	//析构函数
	virtual ~CSensorInMan();

	//设置设备类型，NO这NC
	void SetDeviceType(bool bNO, unsigned char chnn);
protected:
	void InitialStatus();
	void AdjustAlarmIn(ULONGLONG & chnnBits);

	//该函数返回的录像类型为：传感器录像
	RECORD_TYPE GetRecType();

	inline ALARM_TYPE GetAlarmType(unsigned long chnn) {return ALARM_TYPE_SENSOR;};

	//传感器检测管理类触发的报警有：录像，云台，继电器
	void OpenAlarm(unsigned long chnn);

	//关闭已经触发的报警
	void CloseAlarm(unsigned long chnn);

private:

	//该变量用于调整输入的报警信息；
	//该变量中的某位添0，表示在该位上的输入信号是1有效，0无效
	//              添1，表示在该位上的输入信号是1无效，0有效
	//具体操作就是和外部信号做异或处理。
	ULONGLONG m_chnnBits;
#if defined(__SUPPORT_SNAPD1__)
	bool *m_pAlarmUpload;
#endif
};

#endif //_SENSOR_IN_MAN_H_
