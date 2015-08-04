/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : zxx
** Date         : 2012-03-21
** Name         : MANUALDVRTimer.h
** Version      : 1.0
** Description  :
** Modify Record:
1:
***********************************************************************/
#ifndef _MANUAL_DVR_TIMER_H_
#define _MANUAL_DVR_TIMER_H_

#include "Product.h"
#include "PUB_common.h"

typedef enum _manual_dvr_timer_status
{
	MANUAL_DVR_TIMER_NO_ACTION,		//未计时
	MANUAL_DVR_TIMER_TIMER_ING,		//计时中
	MANUAL_DVR_TIMER_TIMER_PAUSE,		//计时暂停
	MANUAL_DVR_TIMER_TIMER_END			//计时完成
}MANUAL_DVR_TIMER_STATUS;

class CMANUALDVRTimer
{
public:
	//interface
	static CMANUALDVRTimer * Instance();
	~CMANUALDVRTimer();
	bool Start();
	void Stop();

	void StartTime(unsigned short index, unsigned short time);
	void StopTime(unsigned short index);
	
	MANUAL_DVR_TIMER_STATUS CheckTimer(unsigned short index);

	bool Initial (unsigned char videoInputNum);
	void Quit();
private:
	//methods
	CMANUALDVRTimer ();
	

	void Timer();

	static	RESULT WINAPI TimerThread(LPVOID lpParameter);

	bool CreateTimerThd();
	void CloseTimerThd();
	//variable
	//////////////////////////////////////////////////////////////////////////
	unsigned short m_maxTimerNum;
	unsigned short *m_pTime;
	MANUAL_DVR_TIMER_STATUS *m_pStatus;

	CPUB_Lock m_lock;

	PUB_thread_t	m_hTimer;
	bool			m_bTimer;	
};

#endif //_MANUAL_DVR_TIMER_H_
