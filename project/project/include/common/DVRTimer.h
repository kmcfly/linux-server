/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : 袁石维
** Date         : 2007-10-13
** Name         : DVRTimer.h
** Version      : 1.0
** Description  :
** Modify Record:
1:
***********************************************************************/
#ifndef _DVR_TIMER_H_
#define _DVR_TIMER_H_

#include "Product.h"
#include "PUB_common.h"

typedef enum _dvr_timer_status
{
	DVR_TIMER_NO_ACTION,		//未计时
	DVR_TIMER_TIMER_ING,		//计时中
	DVR_TIMER_TIMER_PAUSE,		//计时暂停
	DVR_TIMER_TIMER_END			//计时完成
}DVR_TIMER_STATUS;

typedef enum _dvr_timer_type
{
	DVR_TIMER_NULL,
	DVR_TIMER_RECORD_MOTION,
	DVR_TIMER_RECORD_SENSOR,
	DVR_TIMER_MOTION,
	DVR_TIMER_VIDEO_LOSS,
	DVR_TIMER_SENSOR_IN,
	DVR_TIMER_SENSOR_OUT,
	DVR_TIMER_BUZZER,
	DVR_TIMER_OTHER_ALARM,
}DVR_TIMER_TYPE;

class CDVRTimer
{
public:
	//interface
	static CDVRTimer * Instance();
	bool Start();
	void Stop();

	void StartTime(unsigned short index, unsigned short time, DVR_TIMER_TYPE type);
	void StopTime(unsigned short index, DVR_TIMER_TYPE type);
	void Pause(unsigned short index, DVR_TIMER_TYPE type);
	void ResetTime(unsigned short index, unsigned short time, DVR_TIMER_TYPE type);

	DVR_TIMER_STATUS CheckTimer(unsigned short index, DVR_TIMER_TYPE type);

	bool Initial (unsigned char videoInputNum, unsigned char sensorInputNum, unsigned char sensorOutputNum, unsigned char otherAlarmNum);
	void Quit();
protected:
	//methods

	//variable

private:
	//methods
	CDVRTimer ();
	virtual ~CDVRTimer();

	void Timer();

	static	RESULT WINAPI TimerThread(LPVOID lpParameter);

	bool CreateTimerThd();
	void CloseTimerThd();

	unsigned short Index(DVR_TIMER_TYPE type);
	//variable
	//////////////////////////////////////////////////////////////////////////
	unsigned short m_maxTimerNum;

	unsigned short m_timerRecordMotion;
	unsigned short m_timerRecordSensor;
	unsigned short m_timerMotion;
	unsigned short m_timerVideoLoss;
	unsigned short m_timerSensorIn;
	unsigned short m_timerSensorOut;
	unsigned short m_timerBuzzer;
	unsigned short m_timerOtherAlarm;
	//////////////////////////////////////////////////////////////////////////
	unsigned short *m_pTime;
	DVR_TIMER_STATUS *m_pStatus;

	CPUB_Lock m_lock;

	PUB_thread_t	m_hTimer;
	bool			m_bTimer;	
};
#endif //_DVR_TIMER_H_


