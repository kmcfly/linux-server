/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : zxx
** Date         : 2007-10-15
** Name         : NEWDVRTimer.cpp
** Version      : 1.0
** Description  :
** Modify Record:
1:
***********************************************************************/
#include "MANUALDVRTimer.h"

//public
CMANUALDVRTimer * CMANUALDVRTimer::Instance()
{
	static CMANUALDVRTimer s_MANUALDVRTimer;
	return &s_MANUALDVRTimer;
}

CMANUALDVRTimer::CMANUALDVRTimer()
{

}

CMANUALDVRTimer::~CMANUALDVRTimer()
{

}

void CMANUALDVRTimer::StartTime(unsigned short index, unsigned short time)
{
	assert(m_maxTimerNum > index);
	assert(MANUAL_DVR_TIMER_NO_ACTION == m_pStatus[index]);
	assert(time > 0);

	m_lock.Lock();

	m_pStatus[index] = MANUAL_DVR_TIMER_TIMER_ING;

	m_pTime[index]	= time;

	m_lock.UnLock();
}

void CMANUALDVRTimer::StopTime(unsigned short index)
{
	
	assert(m_maxTimerNum > index);
	assert(MANUAL_DVR_TIMER_NO_ACTION != m_pStatus[index]);

	m_lock.Lock();

	m_pStatus[index]	= MANUAL_DVR_TIMER_NO_ACTION;

	m_pTime[index]	= 0;

	m_lock.UnLock();
}

MANUAL_DVR_TIMER_STATUS CMANUALDVRTimer::CheckTimer(unsigned short index)
{
	assert(m_maxTimerNum > index);
	//printf("%s,%d,index=%d,status=%d\n",__FILE__,__LINE__,index,m_pStatus[index]);
	return m_pStatus[index];
}

bool CMANUALDVRTimer::Initial(unsigned char videoInputNum)
{
	assert ((MAX_CHNN_NUM >= videoInputNum) && (videoInputNum > 0));
	if ((MAX_CHNN_NUM < videoInputNum) || (0 == videoInputNum))
	{
		return false;
	}
	

	m_maxTimerNum	= videoInputNum;

	m_pTime		= new unsigned short [m_maxTimerNum];
	m_pStatus	= new MANUAL_DVR_TIMER_STATUS [m_maxTimerNum];
	//////////////////////////////////////////////////////////////////////////
	m_hTimer = 0;
	m_bTimer = false;
	for (unsigned short i=0; i<m_maxTimerNum; ++i)
	{
		m_pStatus[i] = MANUAL_DVR_TIMER_NO_ACTION;
		m_pTime[i]	= 0;
	}

	return true;
}


bool CMANUALDVRTimer::Start()
{
	return CreateTimerThd();
}

void CMANUALDVRTimer::Stop()
{
	CloseTimerThd();
}

void CMANUALDVRTimer::Quit()
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



void CMANUALDVRTimer::Timer()
{
	unsigned short i=0;
	while(m_bTimer)
	{
		m_lock.Lock();

		for (i=0; i<m_maxTimerNum; ++i)
		{
			if ((m_pTime[i] > 0) && (MANUAL_DVR_TIMER_TIMER_PAUSE != m_pStatus[i]))
			{
				-- m_pTime[i];
			}

			if ((0 == m_pTime[i]) && (MANUAL_DVR_TIMER_TIMER_ING == m_pStatus[i]))
			{
				m_pStatus [i] = MANUAL_DVR_TIMER_TIMER_END;
			}
		}

		m_lock.UnLock();

		PUB_Sleep(1000);
	}
}

RESULT WINAPI CMANUALDVRTimer::TimerThread(LPVOID lpParameter)
{
	assert(NULL != lpParameter);

#ifdef __ENVIRONMENT_LINUX__
	pid_t pid = getpid();
	pid_t tid = syscall(__NR_gettid);

	printf("%s, %d, Timer thread pid = %d, tid=%d\n", __FILE__, __LINE__, pid, tid);
#endif

	CMANUALDVRTimer *pThis = (CMANUALDVRTimer *) lpParameter;

	pThis->Timer();

	return 0;
}

bool CMANUALDVRTimer::CreateTimerThd()
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

void CMANUALDVRTimer::CloseTimerThd()
{
	if (PUB_THREAD_ID_NOINIT != m_hTimer)
	{		
		PUB_ExitThread(&m_hTimer, &m_bTimer);		
	}
}

