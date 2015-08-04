/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : 
** Date         : 2008-12-31
** Name         : 
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/
#ifndef _VIDEO_LOSS_MAN_H_
#define _VIDEO_LOSS_MAN_H_
#include "AlarmIn.h"

class CVideoLossMan : public CAlarmIn
{
public:

	//构造函数
	CVideoLossMan();

	//析构函数
	virtual ~CVideoLossMan();
protected:
	void InitialStatus();
	//该函数返回移动录像类型
	RECORD_TYPE GetRecType();

	inline ALARM_TYPE GetAlarmType(unsigned long chnn) {return ALARM_TYPE_VIDEOLOSS;};

	//移动检测触发的报警有：录像，继电器
	void OpenAlarm(unsigned long chnn);

	//关闭已经触发的报警
	void CloseAlarm(unsigned long chnn);

	void AdjustSnap(ULONGLONG & llSnapChnn, unsigned long alarmChnn);
private:
#if defined(__SUPPORT_SNAPD1__)
	bool *m_pAlarmUpload;
#endif
};
#endif //_VIDEO_LOSS_MAN_H_
