/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : zxx
** Date         : 2012-03-21
** Name         : YZRecord.cpp
** Version      : 1.0
** Description  :
** Modify Record:
1:
***********************************************************************/
#include "YZRecord.h"

CYZRecord * CYZRecord::Instance()
{
	static CYZRecord s_yzRec;
	return &s_yzRec;
}

CYZRecord::CYZRecord ()
{
	m_pYzRecStatus    =NULL;
	m_pRecMan = NULL;
	m_pmaunalDVRTimer = NULL;
	m_videoInputNum =0;
	m_clientID =0;
}

CYZRecord::~CYZRecord()
{

}

bool CYZRecord::Initial (unsigned char videoInputNum,CRecordMan *pRecMan)
{

	if(pRecMan!=NULL)
	{
		m_pRecMan = pRecMan;
	}
	else
	{		
		return false;
	}

	m_videoInputNum = videoInputNum;
	m_pYzRecStatus = new int[videoInputNum];
	for(int i=0;i<videoInputNum;i++)
	{
		m_pYzRecStatus[i]=YZ_REC_END;
	}
	m_pmaunalDVRTimer= CMANUALDVRTimer::Instance();
	return true;
}


bool CYZRecord::CreateYZCheckTimerThd()
{
	assert(0 == m_checkYzTimer); //只能建立一次线程

	m_checkYzTimer = PUB_CreateThread(YZTimerThread, (void *)this, &m_bYzTimer);
	if( PUB_CREATE_THREAD_FAIL == m_checkYzTimer )
	{
		PUB_PrintError(__FILE__,__LINE__);
		return false;
	}

	return  true;
}

RESULT WINAPI CYZRecord::YZTimerThread(LPVOID lpParameter)
{
	assert(NULL != lpParameter);

#ifdef __ENVIRONMENT_LINUX__
	pid_t pid = getpid();
	pid_t tid = syscall(__NR_gettid);

	printf("%s, %d, Timer thread pid = %d, tid=%d\n", __FILE__, __LINE__, pid, tid);
#endif

	CYZRecord *pThis = (CYZRecord *) lpParameter;

	pThis->YZTimer();

	return 0;
}


void CYZRecord::YZTimer()
{
	while(m_bYzTimer)
	{
		m_lock.Lock();
		for (int i=0; i<m_videoInputNum; ++i)
		{
			if(YZ_REC_ING == m_pYzRecStatus[i])
			{
				if(MANUAL_DVR_TIMER_TIMER_END == m_pmaunalDVRTimer->CheckTimer(i))
				{
					/*unsigned long nowTime =GetCurrTime32();
					printf("%s,%d,auto stoprec\n",__FILE__,__LINE__);
					PrintfBit32Time(nowTime);*/
					m_pYzRecStatus[i]=YZ_REC_END;
					//停止录像
					m_pRecMan->StopRecord(i,RECORD_TYPE_MANUAL);
					//写入日志
					OPERATOR_LOG		operatorLog;
					memset(&operatorLog, 0, sizeof(OPERATOR_LOG));
					USER_INFO user;
					memset(&user,0,sizeof(USER_INFO));
					if(CUserMan::Instance()->GetUser(user,m_clientID))
					{
						operatorLog.IP		= user.IP;
						strcpy(operatorLog.name, user.name);

						operatorLog.contentID = GetContentID(LOG_TYPE_MANUAL_STOP);
						operatorLog.type = (LOG_TYPE_MANUAL_STOP & 0xffff);

						operatorLog.time = GetCurrTime32();
						CLogMan::Instance()->WriteOperatorLog(operatorLog);
					}


				}
			}
		}
		m_lock.UnLock();

		PUB_Sleep(1000);
	}
}
void CYZRecord::CloseYZTimerThd()
{
	if (PUB_THREAD_ID_NOINIT != m_checkYzTimer)
	{		
		PUB_ExitThread(&m_checkYzTimer, &m_bYzTimer);		
	}
}


void CYZRecord::Quit()
{
	if(m_pYzRecStatus!=NULL)
	{
		delete []m_pYzRecStatus;
		m_pYzRecStatus = NULL;
	}
}

void CYZRecord::StartRec(unsigned char chnn,unsigned short recTime,unsigned char recStatus,unsigned long clientID)
{
	m_lock.Lock();
	if(m_pYzRecStatus!=NULL)
	{
		if(recStatus==YZ_REC_START)//开始录像
		{
			switch(m_pYzRecStatus[chnn])
			{
				case YZ_REC_ING:
					break;
				case YZ_REC_END:
					{
						//unsigned long nowTime =GetCurrTime32();
						//printf("starttime:\n");
						//PrintfBit32Time(nowTime);
						////传给定时器startTime
						m_pmaunalDVRTimer->StartTime(chnn,recTime);
						m_pYzRecStatus[chnn]=YZ_REC_ING;
						//开启录像
						m_clientID=clientID;
						m_pRecMan->Record(chnn,RECORD_TYPE_MANUAL);
						break;
					}
				default:
					assert(false);
					break;

			}
		}
	}
	m_lock.UnLock();
}

void CYZRecord::StopRec(unsigned char chnn,unsigned char recStatus)
{
	m_lock.Lock();
	if(m_pYzRecStatus!=NULL)
	{
		if(recStatus==YZ_REC_STOP)//结束录像
		{
			switch(m_pYzRecStatus[chnn])
			{
			case YZ_REC_ING://必须结束录像，停止计时器
				{
					/*unsigned long nowTime =GetCurrTime32();
					printf("manual endtime:\n");
					PrintfBit32Time(nowTime);*/
					m_pRecMan->StopRecord(chnn,RECORD_TYPE_MANUAL);
					m_pmaunalDVRTimer->StopTime(chnn);
					m_pYzRecStatus[chnn]=YZ_REC_END;
					break;
				}
			case YZ_REC_END://录像已经停止
				break;
			default:
				assert(false);
				break;
			}
		}
	}
	m_lock.UnLock();
	
}
bool CYZRecord::start()//开启线程
{
	//开启线程检查计时器
	if(!CreateYZCheckTimerThd())
	{
		if(m_pYzRecStatus!=NULL)
		{
			delete []m_pYzRecStatus;
			m_pYzRecStatus = NULL;
		}
		return false;
	}
	return true;
}

void CYZRecord::stop()//停止线程
{
	CloseYZTimerThd();
}
//end
