/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : 袁石维
** Date         : 2007-10-15
** Name         : DVRTimer.cpp
** Version      : 1.0
** Description  :
** Modify Record:
1:
***********************************************************************/
#include "DVRTimer.h"

//public
CDVRTimer * CDVRTimer::Instance()
{
	static CDVRTimer s_DVRTimer;
	return &s_DVRTimer;
}

void CDVRTimer::StartTime(unsigned short index, unsigned short time, DVR_TIMER_TYPE type)
{
	unsigned short i = Index(type) + index;
	assert(m_maxTimerNum > i);
	assert(DVR_TIMER_NO_ACTION == m_pStatus[i]);
	assert(time >= 0);


	m_lock.Lock();

	m_pStatus[i] = DVR_TIMER_TIMER_ING;

	m_pTime[i]	= time;

	m_lock.UnLock();
}

void CDVRTimer::StopTime(unsigned short index, DVR_TIMER_TYPE type)
{
	unsigned short i = Index(type) + index;
	assert(m_maxTimerNum > i);
	assert(DVR_TIMER_NO_ACTION != m_pStatus[i]);

	m_lock.Lock();

	m_pStatus[i]	= DVR_TIMER_NO_ACTION;

	m_pTime[i]	= 0;

	m_lock.UnLock();
}

void CDVRTimer::Pause(unsigned short index, DVR_TIMER_TYPE type)
{
	unsigned short i = Index(type) + index;
	assert(m_maxTimerNum > i);
	assert(DVR_TIMER_NO_ACTION != m_pStatus[i]);

	m_lock.Lock();

	m_pStatus[i]	= DVR_TIMER_TIMER_PAUSE;

	m_lock.UnLock();
}

void CDVRTimer::ResetTime(unsigned short index, unsigned short time, DVR_TIMER_TYPE type)
{
	unsigned short i = Index(type) + index;
	assert(m_maxTimerNum > i);
	//assert(DVR_TIMER_NO_ACTION != m_pStatus[i]);

	m_lock.Lock();

	m_pTime[i]	= time;	//不修改状态，只修改时间值。

	m_lock.UnLock();
}

DVR_TIMER_STATUS CDVRTimer::CheckTimer(unsigned short index, DVR_TIMER_TYPE type)
{
	unsigned short i = Index(type) + index;
	assert(m_maxTimerNum > i);

	return m_pStatus[i];
}

bool CDVRTimer::Initial(unsigned char videoInputNum, unsigned char sensorInputNum, unsigned char sensorOutputNum, unsigned char otherAlarmNum)
{
	assert ((MAX_CHNN_NUM >= videoInputNum) && (videoInputNum > 0));
	if ((MAX_CHNN_NUM < videoInputNum) || (0 == videoInputNum))
	{
		return false;
	}
	assert ((MAX_CHNN_NUM >= sensorInputNum) && (sensorInputNum > 0));
	if ((MAX_CHNN_NUM < sensorInputNum) || (0 == sensorInputNum))
	{
		return false;
	}
	assert ((MAX_CHNN_NUM >= sensorOutputNum) && (sensorOutputNum >= 0));
	if ((MAX_CHNN_NUM < sensorOutputNum) || (0 > sensorOutputNum))
	{
		return false;
	}

	m_timerRecordMotion		= 0;
	m_timerRecordSensor		= m_timerRecordMotion + videoInputNum;
	m_timerMotion			= m_timerRecordSensor + videoInputNum;
	m_timerVideoLoss		= m_timerMotion	+ videoInputNum;
	m_timerSensorIn			= m_timerVideoLoss + videoInputNum;
	m_timerSensorOut		= m_timerSensorIn + sensorInputNum;
	m_timerBuzzer			= m_timerSensorOut + sensorOutputNum;
	m_timerOtherAlarm		= m_timerBuzzer + 1;
	
	m_maxTimerNum	= m_timerOtherAlarm + otherAlarmNum;

	m_pTime		= new unsigned short [m_maxTimerNum];
	m_pStatus	= new DVR_TIMER_STATUS [m_maxTimerNum];
	//////////////////////////////////////////////////////////////////////////
	m_hTimer = 0;
	m_bTimer = false;
	for (unsigned short i=0; i<m_maxTimerNum; ++i)
	{
		m_pStatus[i] = DVR_TIMER_NO_ACTION;
		m_pTime[i]	= 0;
	}

	return true;
}

bool CDVRTimer::Start()
{
	return CreateTimerThd();
}

void CDVRTimer::Stop()
{
	CloseTimerThd();
}

void CDVRTimer::Quit()
{
	if(m_pTime!=NULL)
	{
		delete [] m_pTime;
	}
	if(m_pStatus!=NULL)
	{
		delete [] m_pStatus;
	}
	m_pTime		= NULL;
	m_pStatus	= NULL;
}
//protected


//private
CDVRTimer::CDVRTimer()
{
}

CDVRTimer::~CDVRTimer()
{
}

void CDVRTimer::Timer()
{
	unsigned short i=0;
	while(m_bTimer)
	{
		m_lock.Lock();
		
		for (i=0; i<m_maxTimerNum; ++i)
		{
			if ((m_pTime[i] > 0) && (DVR_TIMER_TIMER_PAUSE != m_pStatus[i]))
			{
				-- m_pTime[i];
			}
			
			if ((0 == m_pTime[i]) && (DVR_TIMER_TIMER_ING == m_pStatus[i]))
			{
				m_pStatus [i] = DVR_TIMER_TIMER_END;
			}
		}
		
		m_lock.UnLock();
		
		PUB_Sleep(1000);
	}
}

RESULT WINAPI CDVRTimer::TimerThread(LPVOID lpParameter)
{
	assert(NULL != lpParameter);

#ifdef __ENVIRONMENT_LINUX__
	pid_t pid = getpid();
	pid_t tid = syscall(__NR_gettid);

	printf("%s, %d, Timer thread pid = %d, tid=%d\n", __FILE__, __LINE__, pid, tid);
#endif

	CDVRTimer *pThis = (CDVRTimer *) lpParameter;

	pThis->Timer();

	return 0;
}

bool CDVRTimer::CreateTimerThd()
{
	assert(0 == m_hTimer); //只能建立一次线程

	m_hTimer = PUB_CreateThread(TimerThread, (void *)this, &m_bTimer);
	if( PUB_CREATE_THREAD_FAIL == m_hTimer )
	{
		PUB_PrintError(__FILE__,__LINE__);
		return false;
	}

	return  true;
}

void CDVRTimer::CloseTimerThd()
{
	if (PUB_THREAD_ID_NOINIT != m_hTimer)
	{		
		PUB_ExitThread(&m_hTimer, &m_bTimer);		
	}
}

unsigned short CDVRTimer::Index(DVR_TIMER_TYPE type)
{
	switch(type)
	{
	case DVR_TIMER_NULL:
		assert (false);
		return 0;
	case DVR_TIMER_RECORD_MOTION:
		return m_timerRecordMotion;
	case DVR_TIMER_RECORD_SENSOR:
		return m_timerRecordSensor;
	case DVR_TIMER_MOTION:
		return m_timerMotion;
	case DVR_TIMER_VIDEO_LOSS:
		return m_timerVideoLoss;
	case DVR_TIMER_SENSOR_IN:
		return m_timerSensorIn;
	case DVR_TIMER_SENSOR_OUT:
		return m_timerSensorOut;
	case DVR_TIMER_BUZZER:
		//由于Buzzer被当成了报警输出的最后一个通道，所以这里返回报警输出的序号，外部再加上偏移可以得到真实地buzzer的通道号。
		return m_timerSensorOut;//m_timerBuzzer;
	case DVR_TIMER_OTHER_ALARM:
		return m_timerOtherAlarm;
	default:
		return 0;
	}
}
//end
