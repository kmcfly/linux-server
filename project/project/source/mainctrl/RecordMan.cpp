/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : 袁石维
** Date         : 2007-10-08
** Name         : RecordMan.cpp
** Version      : 1.0
** Description  :
** Modify Record:
1:
***********************************************************************/
#include "RecordMan.h"
#include "ConfigEx.h"
#include "ReclogManEx.h"

//public
CRecordMan::CRecordMan()
{
	m_videoInputNum = 0;
	m_audioInputNum = 0;
	m_initialPreRecordLMask = 0;
	m_initialPreRecordHMask = 0;
	m_preRecordType = PRERECORD_MODE_MEMORY;
}

CRecordMan::~CRecordMan()
{
}


bool CRecordMan::Initial(unsigned char videoInputNum, unsigned char audioInputNum,unsigned long preRecordType, CReclogManEX *pReclogManEx, const char *pFilePath /* = NULL */, 
						 unsigned char preRecStartPar /* = 0 */, unsigned char parNum /* = 0 */, 
						 CMessageMan *pMsgMan /* = NULL */)
{
	m_actionLock.Lock();

	m_videoInputNum = videoInputNum;
	m_audioInputNum = audioInputNum;
	m_initialPreRecordLMask = 0;
	m_initialPreRecordHMask = 0;
	m_preRecordType = PRERECORD_MODE_MEMORY;
	
	m_bAction       = false;
	m_preRecordType = preRecordType;

	m_pRecord		= new unsigned char [m_videoInputNum];
	m_pSchedule		= new unsigned char [m_videoInputNum];
	m_pMotion		= new unsigned char [m_videoInputNum];
	m_pSensor		= new unsigned char [m_videoInputNum];
	m_pManual		= new unsigned char [m_videoInputNum];
	m_pAudio		= new unsigned char [m_videoInputNum];

	m_pPreRecProc	= new unsigned char [m_videoInputNum];
	m_pActionRec	= new unsigned char [m_videoInputNum];

	m_pManualStatus	= new unsigned char [m_videoInputNum];
	m_pMotionStatus	= new unsigned char [m_videoInputNum];
	m_pSensorStatus	= new unsigned char [m_videoInputNum];
	m_pScheduleStatus	= new unsigned char [m_videoInputNum];

	m_pMotionCount	= new short [m_videoInputNum];
	m_pSensorCount	= new short [m_videoInputNum];
	m_pRecHoldTime	= new unsigned long [m_videoInputNum];

	m_pRecordProc	= new CRecordProc[m_videoInputNum];

	m_pScheduleSch		= new CSchedule [m_videoInputNum];
	m_pMotionSch	= new CSchedule [m_videoInputNum];
	m_pSensorSch	= new CSchedule [m_videoInputNum];

	m_pChannelLive	= new unsigned char[m_videoInputNum];

	ResetStatus();
	if (PRERECORD_MODE_DISK == m_preRecordType)
	{
		m_initialPreRecordLMask = 0;
		m_initialPreRecordHMask = 0;

		bool bRet = true;	
		if (0 == parNum)
		{
			bRet = true;
			for (unsigned long i=0; i<m_videoInputNum; ++i)
			{
				if (!m_pRecordProc[i].Initial(PRERECORD_MODE_DISK, i, pReclogManEx, pFilePath, NULL, pMsgMan))
				{
					assert(false);
					bRet = false;
					break;
				}
			}
		}
		else
		{
			bRet = true;
			int fileNumOfPartition = 4;
			if (m_videoInputNum > 16)
			{
				fileNumOfPartition = 8;
			}
			for (unsigned long i=0; i<m_videoInputNum; ++i)
			{	
				int partionIndex = preRecStartPar + i/fileNumOfPartition;
				char szPrerecordPath[64] = {0};
				snprintf(szPrerecordPath, 64, "%s/%02d", pFilePath, partionIndex);
				if (!m_pRecordProc[i].Initial(PRERECORD_MODE_DISK, i, pReclogManEx, pFilePath, szPrerecordPath, pMsgMan))
				{
					assert(false);
					bRet = false;
					break;			
				}

				if (m_pRecordProc[i].IsCanPreDiskRecord())
				{
					if (partionIndex < 64)
					{
						m_initialPreRecordLMask |= (((ULONGLONG)1) << (partionIndex));
						m_initialPreRecordHMask |= 0;
					}
					else
					{
						m_initialPreRecordLMask |= 0;
						m_initialPreRecordHMask |= (((ULONGLONG)1) << (partionIndex));
					}
				}
			}	
		}

		if (!bRet)
		{
			m_initialPreRecordLMask = 0;
			m_initialPreRecordHMask = 0;
			m_actionLock.UnLock();
			return false;
		}
	}
	else
	{
		for (unsigned long i=0; i<m_videoInputNum; ++i)
		{
			if (!m_pRecordProc[i].Initial(PRERECORD_MODE_MEMORY, i, pReclogManEx, pFilePath, NULL, pMsgMan))
			{
				m_actionLock.UnLock();
				assert (false);
				return false;
			}
		}
	}

	
	
	m_pTimer	= CDVRTimer::Instance();

	m_bAction = true;
	m_actionLock.UnLock();
	return true;
}

void CRecordMan::GetInitialPreDiskRecordDiskMask(ULONGLONG &heightMask, ULONGLONG &lowMask)
{
	m_actionLock.Lock();
	heightMask = m_initialPreRecordHMask;
	lowMask = m_initialPreRecordLMask;	
	m_actionLock.UnLock();
}

void CRecordMan::Quit()
{
	m_actionLock.Lock();

	for (BYTE i=0; i<m_videoInputNum; ++i)
	{
		m_pRecordProc[i].Quit();
	}

	ResetStatus();

	delete [] m_pRecord;
	delete [] m_pSchedule;
	delete [] m_pMotion;
	delete [] m_pSensor;
	delete [] m_pManual;
	delete [] m_pAudio;

	delete [] m_pPreRecProc;
	delete [] m_pActionRec;

	delete [] m_pManualStatus;
	delete [] m_pMotionStatus;
	delete [] m_pSensorStatus;
	delete [] m_pScheduleStatus;

	delete [] m_pMotionCount;
	delete [] m_pSensorCount;
	delete [] m_pRecHoldTime;

	delete [] m_pRecordProc;

	delete [] m_pScheduleSch;
	delete [] m_pMotionSch;
	delete [] m_pSensorSch;

	delete [] m_pChannelLive;

	m_actionLock.UnLock();
}



void CRecordMan::Action(bool bAction)
{
	assert(m_bAction != bAction);

	m_actionLock.Lock();
	if (!bAction)
	{
		for (BYTE i = 0; i < m_videoInputNum; ++i)
		{
			//printf("%s,%d\n",__FILE__,__LINE__);
			m_pRecordProc[i].Reset();
			//printf("%s,%d\n",__FILE__,__LINE__);
			m_pRecordProc[i].RemoveData();
		}

		memset(m_pPreRecProc, 0, m_videoInputNum);

		memset(m_pManualStatus, 0, m_videoInputNum);
		memset(m_pMotionStatus, 0, m_videoInputNum);
		memset(m_pSensorStatus, 0, m_videoInputNum);
		memset(m_pScheduleStatus, 0, m_videoInputNum);

		memset(m_pMotionCount, 0, sizeof(short)*m_videoInputNum);
		memset(m_pSensorCount, 0, sizeof(short)*m_videoInputNum);
		
		
		for (BYTE i = 0; i < m_videoInputNum; ++i)
		{
			if (DVR_TIMER_NO_ACTION != m_pTimer->CheckTimer(i, DVR_TIMER_RECORD_MOTION))
			{
				m_pTimer->StopTime(i, DVR_TIMER_RECORD_MOTION);
			}

			if (DVR_TIMER_NO_ACTION != m_pTimer->CheckTimer(i, DVR_TIMER_RECORD_SENSOR))
			{
				m_pTimer->StopTime(i, DVR_TIMER_RECORD_SENSOR);
			}			
		}

		//printf("%s,%d\n",__FILE__,__LINE__);
	}
	m_bAction = bAction;

	m_actionLock.UnLock();
}

void CRecordMan::ResetPreDiskRecordPath(unsigned char preRecStartPar, unsigned char parNum, ULONGLONG &heightMask, ULONGLONG &lowMask)
{
	m_actionLock.Lock();
	heightMask = 0;
	lowMask = 0;

	bool bPreRecord = false;
		
	int pathIndex = -1;
	if (0 == parNum)
	{
		pathIndex = -1;
		for (unsigned long i=0; i<m_videoInputNum; ++i)
		{
			m_pRecordProc[i].ResetPreDiskRecordPath(pathIndex, bPreRecord);			
		}
	}
	else
	{
		int fileNumOfPartition = 4;
		if (m_videoInputNum > 16)
		{
			fileNumOfPartition = 8;
		}

		for (unsigned long i=0; i<m_videoInputNum; ++i)
		{			
			pathIndex = preRecStartPar + i/fileNumOfPartition;
			m_pRecordProc[i].ResetPreDiskRecordPath(pathIndex, bPreRecord);	
			if (bPreRecord)
			{
				if (pathIndex < 64)
				{
					lowMask |= (((ULONGLONG)1) << (pathIndex));
					heightMask |= 0;
				}
				else
				{
					lowMask |= 0;
					heightMask |= (((ULONGLONG)1) << (pathIndex));
				}
			}
			else
			{

			}
		}	
	}

	m_actionLock.UnLock();
	return;
}



RECORD_WRITE_STATUS CRecordMan::DateProc(CFrameData & frameData, bool & bKeyFrame)
{
	m_actionLock.Lock();

	if (!m_bAction)
	{
		m_actionLock.UnLock();
		return RECORD_WRITE_FAIL;
	}
	

	//do something
	assert(CHANNEL_ID(frameData.GetFrameInfo().channel) < m_videoInputNum);

	//不开启录像，则直接返回。
	{
		if (!m_pRecord[CHANNEL_ID(frameData.GetFrameInfo().channel)])
		{
			m_actionLock.UnLock();
			return RECORD_WRITE_FAIL;
		}
	}

	//不开启录取音频时过滤音频
	{
		if (!m_pAudio[CHANNEL_ID(frameData.GetFrameInfo().channel)] && (FRAME_TYPE_AUDIO == frameData.GetFrameInfo().frameType))
		{
			m_actionLock.UnLock();
			return RECORD_WRITE_SUCC;
		}
	}

	RECORD_WRITE_STATUS ret = m_pRecordProc[CHANNEL_ID(frameData.GetFrameInfo().channel)].DateProc(frameData, bKeyFrame);

	m_actionLock.UnLock();
	return ret;
}

bool CRecordMan::Record(unsigned char chnn, RECORD_TYPE type)
{
	m_actionLock.Lock();

	if (!m_bAction)
	{
		m_actionLock.UnLock();
		return false;
	}

	//do something
	assert(chnn < m_videoInputNum);

	//对于schedu,手工录像事件可以直接向通道发送录像信号
	//而对于motion，sensor事件要考虑到：某个通道被交叉触发的情况
	bool ret = true;
	if (RECORD_TYPE_MANUAL == type)
	{
		m_pManual[chnn] = true;
	}
	else if (RECORD_TYPE_SENSOR == type)
	{
		ret = true;
		switch(m_pSensorStatus[chnn]) 
		{
		case RECORD_WAITING :
			{
				if (!(m_pSensorSch[chnn].CheckTime(GetCurrTime32()) && m_pRecord[chnn]))
				{
					m_pSensorCount[chnn]++;
				}
				else
				{					
					ret = m_pRecordProc[chnn].Record(type);
					if (ret)
					{
						++ m_pSensorCount[chnn];
						m_pSensorStatus[chnn] = RECORD_DOING;
					}
				}
			}
			break;
		case RECORD_DOING :
			{
				m_pSensorCount[chnn]++;
			}
			break;
		case RECORD_DELAY :
			{
				assert(0 == m_pSensorCount[chnn]);
				m_pTimer->StopTime(chnn, DVR_TIMER_RECORD_SENSOR);
				m_pSensorStatus[chnn] = RECORD_DOING;
				m_pSensorCount[chnn]++;
			}
			break;

		}			

	}
	else if (RECORD_TYPE_MOTION == type)
	{
		ret = true;
		switch(m_pMotionStatus[chnn]) 
		{
		case RECORD_WAITING :
			{
				if (!(m_pMotionSch[chnn].CheckTime(GetCurrTime32()) && m_pRecord[chnn]))
				{
					m_pMotionCount[chnn]++;
					//printf("%ld is not in motion time\n",chnn);
				}
				else
				{
					ret = m_pRecordProc[chnn].Record(type);
					if (ret)
					{
						++ m_pMotionCount[chnn];
						m_pMotionStatus[chnn] = RECORD_DOING;
						//printf("%ld start to record type:motion\n", chnn);
					}
				}				
			}
			break;
		case RECORD_DOING :
			{
				m_pMotionCount[chnn]++;
				//printf("%ld is recording type: motion\n",chnn);
			}
			break;
		case RECORD_DELAY :
			{
				assert(0 == m_pMotionCount[chnn]);
				m_pTimer->StopTime(chnn, DVR_TIMER_RECORD_MOTION);
				m_pMotionStatus[chnn] = RECORD_DOING;
				m_pMotionCount[chnn]++;
				//printf("%ld go into recording motion from delay\n",chnn);
			}
			break;
		}//end of switch
	}
	else
	{
		assert(false);
	}

	m_actionLock.UnLock();

	return ret;
}

bool CRecordMan::StopRecord(unsigned char chnn, RECORD_TYPE type)
{
	m_actionLock.Lock();

	if (!m_bAction)
	{
		m_actionLock.UnLock();
		return false;
	}


	//do something
	assert(chnn < m_videoInputNum);

	bool ret = true;
	if (RECORD_TYPE_MANUAL == type)
	{
		m_pManual[chnn] = false;
	}
	else if (RECORD_TYPE_SENSOR == type)
	{
		ret = true;
		switch(m_pSensorStatus[chnn])
		{
		case RECORD_WAITING:
			{
				assert (m_pSensorCount[chnn] > 0);
				if (m_pSensorCount[chnn] > 0)
				{
					-- m_pSensorCount[chnn];
				}
			}
			break;
		case RECORD_DOING:
			{
				assert (m_pSensorCount[chnn] > 0);
				-- m_pSensorCount[chnn];

				if (0 == m_pSensorCount[chnn])
				{
					m_pTimer->StartTime(chnn, m_pRecHoldTime[chnn], DVR_TIMER_RECORD_SENSOR);
					m_pSensorStatus[chnn] = RECORD_DELAY;

				}					
			}
			break;
		case RECORD_DELAY:
			{
				assert(false);
			}
			break;

		}
	}	
	else if (RECORD_TYPE_MOTION == type)
	{
		ret = true;
		switch(m_pMotionStatus[chnn])
		{
		case RECORD_WAITING:
			{
				assert (m_pMotionCount[chnn] > 0);
				-- m_pMotionCount[chnn];
			}
			break;
		case RECORD_DOING:
			{
				assert (m_pMotionCount[chnn] > 0);
				-- m_pMotionCount[chnn];

				if (0 == m_pMotionCount[chnn])
				{
					m_pTimer->StartTime(chnn, m_pRecHoldTime[chnn], DVR_TIMER_RECORD_MOTION);
					m_pMotionStatus[chnn] = RECORD_DELAY;
					//printf("%ld into record delay type:motion\n",chnn);
				}					
			}
			break;
		case RECORD_DELAY:
			{
				assert(false);
			}
			break;

		}
	}
	else
	{
		assert(false);
	}

	m_actionLock.UnLock();

	return ret;
}

void CRecordMan::SetChannelLive(unsigned char chnn, bool bLive)
{
	m_actionLock.Lock();

	//do something
	assert(chnn < m_videoInputNum);

	m_pChannelLive[chnn] = bLive;

	m_actionLock.UnLock();
}

bool CRecordMan::Start()
{
	m_timerTheadID = PUB_CreateThread(TimerThread, this, &m_bTimer);
	if (PUB_CREATE_THREAD_FAIL == m_timerTheadID)
	{
		PUB_PrintError(__FILE__,__LINE__);
		return false;
	}

	return true;
}

void CRecordMan::Stop()
{
	if (PUB_THREAD_ID_NOINIT != m_timerTheadID)
	{
		PUB_ExitThread(&m_timerTheadID, &m_bTimer);
	}
}

void CRecordMan::RemoveAllDate()
{
	m_actionLock.Lock();

	for (BYTE chnn = 0; chnn < m_videoInputNum; ++chnn)
	{
		m_pRecordProc[chnn].RemoveData();
	}

	m_actionLock.UnLock();
}



void CRecordMan::SetRecEnable(FRAME_TYPE type, bool bEnable, unsigned char chnn)
{
	assert (chnn < m_videoInputNum);
	assert ((FRAME_TYPE_AUDIO == type) || (FRAME_TYPE_VIDEO == type));

	m_actionLock.Lock();

	if (!m_bAction)
	{
		m_actionLock.UnLock();
		return ;
	}


	if (FRAME_TYPE_AUDIO == type)
	{
		m_pAudio[chnn]	= bEnable;
	}
	else	//其它的都当成为视频
	{
		m_pRecord[chnn]	= bEnable;
	}

	if (!bEnable)
	{
		m_pRecordProc[chnn].RemoveData();

		//关闭录像，把数据从缓冲区刷新到磁盘
		m_pRecordProc[chnn].FlushRecDataToDisk();
	}

	m_actionLock.UnLock();	
}

void CRecordMan::SetWeekSchedule(RECORD_TYPE type, const WEEK_SCHEDULE & weekSch, unsigned char chnn)
{
	assert (chnn < m_videoInputNum);
	assert ((RECORD_TYPE_SCHEDULE == type) || (RECORD_TYPE_MOTION == type) || (RECORD_TYPE_SENSOR == type));

	m_actionLock.Lock();

	if (!m_bAction)
	{
		m_actionLock.UnLock();
		return ;
	}


	if (RECORD_TYPE_SCHEDULE == type)
	{
		m_pScheduleSch[chnn].SetWeekSchedule(weekSch);
	}
	else if (RECORD_TYPE_MOTION == type)
	{
		m_pMotionSch[chnn].SetWeekSchedule(weekSch);
	}
	else
	{
		m_pSensorSch[chnn].SetWeekSchedule(weekSch);
	}

	m_actionLock.UnLock();
}

void CRecordMan::SetHolidaySchedule(RECORD_TYPE type, const CMyList<HOLIDAY_SCHEDULE> &holidayList, unsigned char chnn)
{
	assert (chnn < m_videoInputNum);
	assert ((RECORD_TYPE_SCHEDULE == type) || (RECORD_TYPE_MOTION == type) || (RECORD_TYPE_SENSOR == type));

	m_actionLock.Lock();

	if (!m_bAction)
	{
		m_actionLock.UnLock();
		return ;
	}


	if (RECORD_TYPE_SCHEDULE == type)
	{
		m_pScheduleSch[chnn].SetHolidaySchedule(holidayList);
	}
	else if (RECORD_TYPE_MOTION == type)
	{
		m_pMotionSch[chnn].SetHolidaySchedule(holidayList);
	}
	else
	{
		m_pSensorSch[chnn].SetHolidaySchedule(holidayList);
	}

	m_actionLock.UnLock();
}

void CRecordMan::SetHoldTime(unsigned long holdTime, unsigned char chnn)
{
	assert (holdTime > 0);//不限定持续时间

	m_actionLock.Lock();

	if (!m_bAction)
	{
		m_actionLock.UnLock();
		return ;
	}



	assert (chnn < m_videoInputNum);
	m_pRecHoldTime[chnn]	= holdTime;

	if (RECORD_DELAY == m_pMotionStatus[chnn])
	{
		m_pTimer->ResetTime(chnn, m_pRecHoldTime[chnn], DVR_TIMER_RECORD_MOTION);
	}

	if (RECORD_DELAY == m_pSensorStatus[chnn])
	{
		m_pTimer->ResetTime(chnn, m_pRecHoldTime[chnn], DVR_TIMER_RECORD_SENSOR);
	}

	m_actionLock.UnLock();
}

void CRecordMan::SetPreRecTime(unsigned long time, unsigned char chnn)
{
	assert (chnn < m_videoInputNum);

	m_actionLock.Lock();

	if (!m_bAction)
	{
		m_actionLock.UnLock();
		return ;
	}


	m_pRecordProc[chnn].SetPreRecTime(time);

	m_actionLock.UnLock();
}

void CRecordMan::SetPreRecTime(unsigned long time)
{

	m_actionLock.Lock();

	if (!m_bAction)
	{
		m_actionLock.UnLock();
		return ;
	}


	for (unsigned char chnn = 0; chnn < m_videoInputNum; ++chnn)
	{
		m_pRecordProc[chnn].SetPreRecTime(time);
	}	

	m_actionLock.UnLock();
}

void CRecordMan::SetPreRecDataLen(unsigned char chnn, unsigned long maxPreRecDataLen)
{
	assert (chnn < m_videoInputNum);

	m_actionLock.Lock();

	if (!m_bAction)
	{
		m_actionLock.UnLock();
		return ;
	}


	m_pRecordProc[chnn].SetPreRecDataLen(maxPreRecDataLen);

	m_actionLock.UnLock();
}

void CRecordMan::RecoverPreDiskRecTime()
{
	m_actionLock.Lock();

	if (!m_bAction)
	{
		m_actionLock.UnLock();
		return ;
	}

	for (unsigned char chnn = 0; chnn < m_videoInputNum; ++chnn)
	{
		m_pRecordProc[chnn].RecoverPreDiskRecTime();
	}	

	m_actionLock.UnLock();

}

void CRecordMan::SetRecycleRecord(bool bEnable)
{
	m_actionLock.Lock();
   
    if (!m_bAction)
    {
		m_actionLock.UnLock();
		return;
    }
	
	for (unsigned char chnn = 0; chnn < m_videoInputNum; ++chnn)
	{
		m_pRecordProc[chnn].SetRecycleRecord(bEnable);
	}	

	m_actionLock.UnLock();
	return;
}

void CRecordMan::SetRedundancyRecord(bool bEnable, unsigned char chnn)
{
#ifdef __ENVIRONMENT_LINUX__
	assert(chnn < m_videoInputNum);
	m_actionLock.Lock();

	if (!m_bAction)
	{
		m_actionLock.UnLock();
		return;
	}

	m_pRecordProc[chnn].SetRedundancyRecord(bEnable);

	m_actionLock.UnLock();
#endif

	return;

}
//protected

//private
void CRecordMan::Timer()
{
	unsigned char chnn=0;
	unsigned long time = 0;

	while(m_bTimer)
	{
		m_actionLock.Lock();

		//add 2010-01-20 14:05 xwm 既然recordMan的Action状态为无效，所以没有必要在进行检测是否有录像到来。
		if (!m_bAction)
		{
			m_actionLock.UnLock();
			PUB_Sleep(5000);
			continue;
		}
		//end

		time = GetCurrTime32();

		for (chnn=0; chnn<m_videoInputNum; ++chnn)
		{
			bool ret = false;

			//处理手动录像
			if (RECORD_WAITING == m_pManualStatus[chnn])
			{
				if (m_pManual[chnn] && m_pRecord[chnn] && m_pChannelLive[chnn])
				{
					ret = m_pRecordProc[chnn].Record(RECORD_TYPE_MANUAL);
					if (ret)
					{
						//printf("%ld start to record type:mannual\n", chnn);
						m_pManualStatus[chnn] = RECORD_DOING;
					}
				}
			}
			else
			{
				if (!(m_pManual[chnn] && m_pRecord[chnn] && m_pChannelLive[chnn]))
				{
					ret = m_pRecordProc[chnn].StopRecord(RECORD_TYPE_MANUAL);
					//printf("%ld stop to record type:mannual\n", chnn);
					m_pManualStatus[chnn] = RECORD_WAITING;
				}
			}

			//判断motion是否在排程时间内
			if (m_pMotionSch[chnn].CheckTime(time) && m_pRecord[chnn])
			{
				if ((RECORD_WAITING == m_pMotionStatus[chnn]) && (m_pMotionCount[chnn] > 0))
				{
					ret = m_pRecordProc[chnn].Record(RECORD_TYPE_MOTION);
					if (ret)
					{
						//printf("%ld start to record type:motion\n", chnn);
						m_pMotionStatus[chnn] = RECORD_DOING;						
					}
				}
				else if (RECORD_DELAY == m_pMotionStatus[chnn])
				{
					assert(0 == m_pMotionCount[chnn]);

					if (DVR_TIMER_TIMER_END == m_pTimer->CheckTimer(chnn, DVR_TIMER_RECORD_MOTION))
					{
						m_pRecordProc[chnn].StopRecord(RECORD_TYPE_MOTION);
						//printf("%ld stop record type: motion succ\n",chnn);
						m_pTimer->StopTime(chnn, DVR_TIMER_RECORD_MOTION);

						m_pMotionStatus[chnn] = RECORD_WAITING;
					}

				}

			}
			else
			{
				if (RECORD_DOING == m_pMotionStatus[chnn])
				{
					assert(m_pMotionCount[chnn] > 0);

					m_pRecordProc[chnn].StopRecord(RECORD_TYPE_MOTION);
					//printf("%ld stop record type: motion succ\n",chnn);

					m_pMotionStatus[chnn] = RECORD_WAITING;
				}
				else if (RECORD_DELAY == m_pMotionStatus[chnn])
				{
					assert(0 == m_pMotionCount[chnn]);

					m_pRecordProc[chnn].StopRecord(RECORD_TYPE_MOTION);
					//printf("%ld stop record type: motion succ\n",chnn);
					m_pTimer->StopTime(chnn, DVR_TIMER_RECORD_MOTION);

					m_pMotionStatus[chnn] = RECORD_WAITING;
				}
			}

			//判断sensor是否在排程时间内
			if (m_pSensorSch[chnn].CheckTime(time) && m_pRecord[chnn])
			{
				if ((RECORD_WAITING == m_pSensorStatus[chnn]) && (m_pSensorCount[chnn] > 0))
				{
					ret = m_pRecordProc[chnn].Record(RECORD_TYPE_SENSOR);
					if (ret)
					{
						//printf("%ld start to record type:sensor succ\n", chnn);
						m_pSensorStatus[chnn] = RECORD_DOING;						
					}
				}
				else if (RECORD_DELAY == m_pSensorStatus[chnn])
				{
					if (DVR_TIMER_TIMER_END == m_pTimer->CheckTimer(chnn, DVR_TIMER_RECORD_SENSOR))
					{
						assert(0 == m_pSensorCount[chnn]);
						m_pRecordProc[chnn].StopRecord(RECORD_TYPE_SENSOR);
						//printf("%ld stop record type: sensor succ\n",chnn);
						m_pTimer->StopTime(chnn, DVR_TIMER_RECORD_SENSOR);

						m_pSensorStatus[chnn] = RECORD_WAITING;
					}
				}

			}
			else
			{
				if (RECORD_DOING == m_pSensorStatus[chnn])
				{
					assert(m_pSensorCount[chnn] > 0);

					m_pRecordProc[chnn].StopRecord(RECORD_TYPE_SENSOR);
					//printf("%ld stop record type: sensor succ\n",chnn);
					m_pSensorStatus[chnn] = RECORD_WAITING;
				}
				else if (RECORD_DELAY == m_pSensorStatus[chnn])
				{
					assert(0 == m_pSensorCount[chnn]);

					m_pRecordProc[chnn].StopRecord(RECORD_TYPE_SENSOR);
					//printf("%ld stop record type: sensor succ\n",chnn);
					m_pTimer->StopTime(chnn, DVR_TIMER_RECORD_SENSOR);

					m_pSensorStatus[chnn] = RECORD_WAITING;
				}
			}

			//处理scheme
			if (RECORD_WAITING == m_pScheduleStatus[chnn])
			{
				if (m_pScheduleSch[chnn].CheckTime(time) && m_pRecord[chnn] && m_pChannelLive[chnn])
				{
					if (m_pRecordProc[chnn].Record(RECORD_TYPE_SCHEDULE))
					{
						//printf("%ld record  schedul succ\n",chnn);
						m_pScheduleStatus[chnn] = RECORD_DOING;
					}
					else
					{
						//printf("%ld record  schedul fail\n",chnn);
					}
				}
			}
			else
			{
				if (!(m_pScheduleSch[chnn].CheckTime(time) && m_pRecord[chnn] && m_pChannelLive[chnn]))
				{
					if (m_pRecordProc[chnn].StopRecord(RECORD_TYPE_SCHEDULE))
					{
						//printf("%ld stop  schedul succ\n",chnn);
					}
					else
					{
						//printf("%ld stop  schedul fail\n",chnn);
					}
					m_pScheduleStatus[chnn] = RECORD_WAITING;
				}

			}//end of scheme

		}//end of for

		m_actionLock.UnLock();
		PUB_Sleep(4000);
	}
}

RESULT WINAPI CRecordMan::TimerThread(LPVOID lpParameter)
{
#ifdef __ENVIRONMENT_LINUX__
	pid_t pid = getpid();
	pid_t tid = syscall(__NR_gettid);
	printf("%s, %d, pid = %d, tid=%d\n", __FILE__, __LINE__, pid, tid);
#endif

	assert(NULL != lpParameter);
	CRecordMan *pThis = (CRecordMan *)lpParameter;

	pThis->Timer();

	return 0;
}

void CRecordMan::ResetStatus()
{
	memset(m_pRecord, 0, m_videoInputNum);
	memset(m_pSchedule, 0, m_videoInputNum);
	memset(m_pMotion, 0, m_videoInputNum);
	memset(m_pSensor, 0, m_videoInputNum);
	memset(m_pManual, 0, m_videoInputNum);
	memset(m_pAudio, 0, m_videoInputNum);

	memset(m_pPreRecProc, 0, m_videoInputNum);
	memset(m_pActionRec, 0, m_videoInputNum);

	memset(m_pManualStatus, 0, m_videoInputNum);
	memset(m_pMotionStatus, 0, m_videoInputNum);
	memset(m_pSensorStatus, 0, m_videoInputNum);
	memset(m_pScheduleStatus, 0, m_videoInputNum);

	memset(m_pMotionCount, 0, sizeof(short)*m_videoInputNum);
	memset(m_pSensorCount, 0, sizeof(short)*m_videoInputNum);

	for(unsigned char chnn = 0; chnn < m_videoInputNum; chnn++)
	{
		m_pChannelLive[chnn] = true;
	}
}
//end

