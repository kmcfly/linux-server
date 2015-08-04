
/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : 
** Date         : 2007-11-06
** Name         : 
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/

#include "AlarmIn.h"
#include "SnapPictureMan.h"
#include "Product.h"

#ifdef __SUPPORT_SNAPD1__
#include "ShanghaiSnapMan.h"
#endif

extern bool g_bHaveSnapPic;
extern ULONGLONG g_canSnapPicCH;

CAlarmIn::CAlarmIn() : m_chnnNum (0), m_pTimer(NULL), m_pAlarmOutMan(NULL), m_pLogMan(NULL), m_pStreamRecManEx(NULL), m_pDisplayMan(NULL), m_status(0)
{
	m_pLastSendEmailTime = NULL;
	m_localVideoInputNum = 0;
	m_netVideoInputNum = 0;
}

CAlarmIn::~CAlarmIn()
{

}

bool CAlarmIn::Initial(unsigned char chnnNum, DVR_TIMER_TYPE timerType, CAlarmOutMan *pAlarmOutMan, CLogMan *pLogMan, CStreamRecordManEx *pStreamRecManEx, CDisplayMan *pDisplayMan, unsigned long localVideoInputNum, unsigned long netVideoInputNum)
{
	assert ((NULL != pAlarmOutMan) && (NULL != pLogMan) && (NULL != pStreamRecManEx) && (NULL != pDisplayMan));
	if ((NULL == pAlarmOutMan) || (NULL == pLogMan) || (NULL == pStreamRecManEx) || (NULL == pDisplayMan))
	{
		return false;
	}

	assert ((MAX_CHNN_NUM >= chnnNum) && (chnnNum > 0));
	if ((0 == chnnNum) || (MAX_CHNN_NUM <chnnNum))
	{
		return false;
	}

	WEEK_SCHEDULE defaultWeekSchdule;
	memset( &defaultWeekSchdule, 0xff, sizeof(WEEK_SCHEDULE) );		//把sensor、motion、videoLoss的默认week schdule设置为有效
	
	m_lock.Lock();
	
	m_chnnNum = chnnNum;

	m_pAlarmOut = new bool [m_chnnNum];	//记录需要触发报警
	m_pAlarmInStatus = new int[m_chnnNum]; //记录报警输入的当前状态， （多通道） 
	m_pAlarmInAttrib = new ALARM_ATTRIB[m_chnnNum]; //报警输入的属性，（多通道）
	m_pEventInfo = new EVENT_LOG[m_chnnNum];

	m_status			= 0;
	
	//初始各个通道的状态为：非报警
	memset(m_pAlarmOut, 0, m_chnnNum*sizeof(bool));
	memset(m_pAlarmInAttrib, 0, m_chnnNum*sizeof(ALARM_ATTRIB));
	memset(m_pEventInfo, 0, m_chnnNum * sizeof(EVENT_LOG));

	//舒适化状态，各派生类需要重写该方法。
	InitialStatus();

	m_pTimer		= CDVRTimer::Instance();   
	m_pAlarmOutMan	= pAlarmOutMan;
	m_pLogMan		= pLogMan;
	m_pStreamRecManEx		= pStreamRecManEx;
	m_pDisplayMan		= pDisplayMan;

	m_pSchedule		= new CSchedule[chnnNum];
	for( BYTE i=0; i<chnnNum; i++ )
	{
		m_pSchedule[i].SetWeekSchedule( defaultWeekSchdule );
	}
	m_bAction = true;


	m_pLastSendEmailTime = new LONGLONG[m_chnnNum];
	memset(m_pLastSendEmailTime, 0, sizeof(LONGLONG)*m_chnnNum);

	m_localVideoInputNum = localVideoInputNum;
	m_netVideoInputNum = netVideoInputNum;

	m_lock.UnLock();
	return true;
}

void CAlarmIn::Quit()
{
	m_lock.Lock();

	m_status			= 0;

	//退出前，停止录像，停止计时。
	for (unsigned short i = 0; i < m_chnnNum; i++)
	{
		StopRecord(i);	

		if (DVR_TIMER_NO_ACTION != m_pTimer->CheckTimer(i, m_pAlarmInAttrib[i].timerType))
		{
			m_pTimer->StopTime(i, m_pAlarmInAttrib[i].timerType);
		}
	}

	for (unsigned char i = 0; i < m_chnnNum; i++)
	{
		if (m_pEventInfo[i].startTime > 0)
		{
			m_pEventInfo[i].endTime = GetCurrTime32();
			m_pLogMan->WriteEventLog(m_pEventInfo[i]);
		}
	}

	memset(m_pAlarmOut, 0, m_chnnNum*sizeof(bool));

	for (unsigned char i = 0; i < m_chnnNum; i++)
	{
		m_pAlarmInStatus[i] = ALARMIN_STATUS_NO;
	}

	memset(m_pEventInfo, 0, m_chnnNum * sizeof(EVENT_LOG));	

	m_pTimer	= NULL;

	delete [] m_pAlarmOut;	//记录需要触发报警
	delete [] m_pAlarmInStatus; //记录报警输入的当前状态， （多通道） 
	delete [] m_pAlarmInAttrib; //报警输入的属性，（多通道）
	delete [] m_pEventInfo;
	delete [] m_pSchedule;
	delete [] m_pLastSendEmailTime;

	m_lock.UnLock();
}

void CAlarmIn::SetEnable(bool bEnable, unsigned char chnn)
{
	assert (chnn < m_chnnNum);

	m_lock.Lock();

	m_pAlarmInAttrib[chnn].bDetection = bEnable;

	if (!bEnable && (ALARMIN_STATUS_NO != m_pAlarmInStatus[chnn]))
	{
		m_pAlarmInStatus[chnn] = ALARMIN_STATUS_NO;
		if (!m_pAlarmOut[chnn]) //m_pAlarmOut[chnn]为真时，表示这个通道在将来需要触发报警，但是报警还没有触发
			                    //m_pAlarmOut[chnn]为假时，表示这个通道不要触发报警或刚刚触发完报警
		{
			CloseAlarm(chnn);
		}
		
		if (DVR_TIMER_NO_ACTION != m_pTimer->CheckTimer(chnn, m_pAlarmInAttrib[chnn].timerType))
		{
			m_pTimer->StopTime(chnn, m_pAlarmInAttrib[chnn].timerType);
		}		
	}

	m_lock.UnLock();
}

void CAlarmIn::Action(bool bEnable)
{
	m_lock.Lock();

	assert(bEnable != m_bAction);
	if (!bEnable)
	{
		for (int chnn = 0; chnn < m_chnnNum; ++chnn)
		{
			if (ALARMIN_STATUS_NO != m_pAlarmInStatus[chnn])
			{
				m_pAlarmInStatus[chnn] = ALARMIN_STATUS_NO;
				if (!m_pAlarmOut[chnn])
				{
					CloseAlarm(chnn);
				}

				if (DVR_TIMER_NO_ACTION != m_pTimer->CheckTimer(chnn, m_pAlarmInAttrib[chnn].timerType))
				{
					m_pTimer->StopTime(chnn, m_pAlarmInAttrib[chnn].timerType);
				}		
			}
		}

		memset(m_pLastSendEmailTime, 0, sizeof(LONGLONG)*m_chnnNum);
	}

	m_bAction = bEnable;

	m_lock.UnLock();

}



void CAlarmIn::SetHoldTime(unsigned short holdTime, unsigned char chnn)
{
	assert (chnn < m_chnnNum);

	m_lock.Lock();

	m_pAlarmInAttrib[chnn].holdTime = holdTime;
	m_pTimer->ResetTime(chnn, holdTime, m_pAlarmInAttrib[chnn].timerType);

	m_lock.UnLock();
}

void CAlarmIn::SetToRecCH(ULONGLONG toRecCH, unsigned char chnn)
{
	assert (chnn < m_chnnNum);

	m_lock.Lock();

	m_pAlarmInAttrib[chnn].toRecCH	= toRecCH;

	if (ALARMIN_STATUS_NO != m_pAlarmInStatus[chnn])
	{
		int videoInputNum = CProduct::Instance()->VideoInputNum();
		for (unsigned char i=0; i<videoInputNum; ++i)
		{
			//如果没有开启录像，但是新配置需要录像，则开启录像。
			if(!(m_pAlarmInAttrib[chnn].hasRecCH & ((ULONGLONG)0x01 << i)) && (m_pAlarmInAttrib[chnn].toRecCH & ((ULONGLONG)0x01 << i)))
			{
				m_pStreamRecManEx->Record(i, GetRecType());
				m_pAlarmInAttrib[chnn].hasRecCH |= ((ULONGLONG)0x01 << i);
			}

			//如果已经开启录像，而新配置要关闭录像，则关闭录像。
			if((m_pAlarmInAttrib[chnn].hasRecCH & ((ULONGLONG)0x01 << i)) && !(m_pAlarmInAttrib[chnn].toRecCH & ((ULONGLONG)0x01 << i)))
			{
				m_pStreamRecManEx->StopRecord(i, GetRecType());
				m_pAlarmInAttrib[chnn].hasRecCH &= ~((ULONGLONG)0x01 << i);
			}
		}
	}

	m_lock.UnLock();
}

void CAlarmIn::SetToAlarmOutCH(ULONGLONG toAlarmOutCH, unsigned char chnn, unsigned char relayOutNum)
{
	assert (chnn < m_chnnNum);

	m_lock.Lock();
	//修改：只有改变了继电器，才进行继电器报警输出。
	ULONGLONG newToAlarmOut = 0;
	newToAlarmOut |= (toAlarmOutCH & ALL_CHNN_MASK(relayOutNum));
	ULONGLONG oldToAlarmOut = m_pAlarmInAttrib[chnn].toAlarmOut;
	oldToAlarmOut &= ALL_CHNN_MASK(relayOutNum);
	if (newToAlarmOut != oldToAlarmOut)
	{
		ULONGLONG temp = m_pAlarmInAttrib[chnn].toAlarmOut;
		m_pAlarmInAttrib[chnn].toAlarmOut = newToAlarmOut;
		if (ALARMIN_STATUS_NO != m_pAlarmInStatus[chnn])
		{
#if defined(__CUSTOM_ZNV__) && defined(__ZNV_CN__) && defined(__ZNV_SC__)
			CloseAlarmOut(chnn);
#endif
			AlarmOut(chnn);
		}
		m_pAlarmInAttrib[chnn].toAlarmOut = temp;
	}
	//保存修改后结果
	m_pAlarmInAttrib[chnn].toAlarmOut   &= ( (ULONGLONG)(0x01) << relayOutNum );						//保留蜂鸣器位清除传感器输出位
	m_pAlarmInAttrib[chnn].toAlarmOut	|= (toAlarmOutCH & ALL_CHNN_MASK(relayOutNum));

	m_lock.UnLock();
}

void  CAlarmIn::SetToBigViewCH(ULONGLONG toBigViewCH, unsigned char chnn)
{
	assert (chnn < m_chnnNum);

	m_lock.Lock();
	
	//修改：只有改变了，才进行报警。
	if (toBigViewCH != m_pAlarmInAttrib[chnn].toBigViewCH)
	{
		m_pAlarmInAttrib[chnn].toBigViewCH	= toBigViewCH;
		if (ALARMIN_STATUS_NO != m_pAlarmInStatus[chnn])
		{
			BigView(chnn);
		}
	}

	m_lock.UnLock();
}

void CAlarmIn::SetToBuzzer(bool bToBuzzer, unsigned char chnn, unsigned char relayOutNum)
{
	assert (chnn < m_chnnNum);

	m_lock.Lock();

	//修改：只有改变了蜂鸣器，才进行蜂鸣器报警。
	ULONGLONG newBoBuzzer = 0;
	if (bToBuzzer)
	{
		newBoBuzzer = ((ULONGLONG)(0x01) << relayOutNum);
	}
	ULONGLONG oldBoBuzzer = m_pAlarmInAttrib[chnn].toAlarmOut;
	oldBoBuzzer &= ((ULONGLONG)(0x01) << relayOutNum);
	if (newBoBuzzer != oldBoBuzzer)
	{
		ULONGLONG temp = m_pAlarmInAttrib[chnn].toAlarmOut;
		m_pAlarmInAttrib[chnn].toAlarmOut = newBoBuzzer;
		if (ALARMIN_STATUS_NO != m_pAlarmInStatus[chnn])
		{
			AlarmOut(chnn);
		}
		m_pAlarmInAttrib[chnn].toAlarmOut = temp;
	}

	//保存修改后的结果
	if (bToBuzzer)
	{
		m_pAlarmInAttrib[chnn].toAlarmOut |= ((ULONGLONG)(0x01) << relayOutNum);
	}
	else
	{
		m_pAlarmInAttrib[chnn].toAlarmOut &= ALL_CHNN_MASK(relayOutNum);
	}

	m_lock.UnLock();
}

/*********************************************************************************************************************

Function:SetToPTZ(const TO_PTZ *pToPTZ, int externDeviceNum, int localVInputNum)

Description:
			1:设置报警输入关联到云台的操作

Calls:

Called by:

Table Accessed:

Table Updated:

Input:
			1:*pToPTZ存放报警产生时要触发的云台动作信息
			2:externDeviceNum表示连接的外部报警输入信息， 
			  比如可能有16个传感器输入，16个视频输入(motion和videoloss)
			3:localVInput表示的是DVR上课接的云台个数

			最复杂的情况下一个报警输入可以触发所有的云台动作，比如一个传感器的输入可以触发所有云台的动作，
			同理一个移动侦测事件也可以触发所有云台的动作
Output:

Return:

Others:
************************************************************************************************************************/
void CAlarmIn::SetToPTZ(const TO_PTZ *pToPTZ, int alarmInputDeviceNum, int videoInputNum)
{
	assert(NULL != pToPTZ);
	assert((0 < alarmInputDeviceNum) && (alarmInputDeviceNum <= m_chnnNum));
	assert ((0 < videoInputNum) && (videoInputNum <= MAX_CHNN_NUM));

	m_lock.Lock();

	for(int alarmInput=0; alarmInput < alarmInputDeviceNum; alarmInput++)
	{
		for(int ch = 0; ch < videoInputNum; ch++ )
		{
			int index =  (alarmInput * videoInputNum) + ch;
			//先设为不触发任何PTZ操作
			m_pAlarmInAttrib[alarmInput].toPTZType[ch] = TO_PTZ_NULL;
			m_pAlarmInAttrib[alarmInput].toPTZPreset[ch] = 0xff;
			m_pAlarmInAttrib[alarmInput].toPTZCruise[ch] = 0xff;
			m_pAlarmInAttrib[alarmInput].toPTZTrack[ch]  = 0xff;
			
			if( pToPTZ[index].cIsEnablePreset )
			{
				//报警触发预置点
				m_pAlarmInAttrib[alarmInput].toPTZType[ch] = TO_PTZ_PRESET;
				m_pAlarmInAttrib[alarmInput].toPTZPreset[ch] = pToPTZ[index].ucToPreset;

			}
			else if( pToPTZ[index].cIsEnableCruise )
			{
				//报警触发巡航
				m_pAlarmInAttrib[alarmInput].toPTZType[ch] = TO_PTZ_CRUISE;
				m_pAlarmInAttrib[alarmInput].toPTZCruise[ch] = pToPTZ[index].ucToCruise;

			}
			else if( pToPTZ[index].cIsEnableTrack )
			{
				//报警触发轨迹
				m_pAlarmInAttrib[alarmInput].toPTZType[ch] = TO_PTZ_TRACK;
				m_pAlarmInAttrib[alarmInput].toPTZTrack[ch] = pToPTZ[index].ucToTrack;

			}
		}
	}

	DisableTrigerSelf( videoInputNum );
	m_lock.UnLock();
}


void  CAlarmIn::SetToEmail(unsigned long toEmail, unsigned char chnn)
{
	assert (chnn < m_chnnNum);

	m_lock.Lock();

	//修改：只有改变了，才进行报警。
	if (toEmail != m_pAlarmInAttrib[chnn].toEmail)
	{
		m_pAlarmInAttrib[chnn].toEmail	= toEmail;
		if (ALARMIN_STATUS_NO != m_pAlarmInStatus[chnn])
		{
			m_pLastSendEmailTime[chnn] = 0;
			SendEmail(chnn);
		}
	}

	m_lock.UnLock();
}

void CAlarmIn::SetToSnapCH(ULONGLONG toSnapCH, unsigned char chnn)
{
	assert (chnn < m_chnnNum);
	
	m_lock.Lock();

	AdjustSnap(toSnapCH, chnn);

	//修改：只有改变了，才进行报警。
	
	if (toSnapCH != m_pAlarmInAttrib[chnn].toSnapCH)
	{
		m_pAlarmInAttrib[chnn].toSnapCH	= toSnapCH;
		if (ALARMIN_STATUS_NO != m_pAlarmInStatus[chnn])
		{
			m_pLastSendEmailTime[chnn] = 0;
			SendEmail(chnn);
			SnapPicture(chnn);
		}
	}

	m_lock.UnLock();
}
void CAlarmIn::SetToFTPVideo(ULONGLONG toFtpCH, unsigned char chnn)
{
	assert (chnn < m_chnnNum);

	m_lock.Lock();
	m_pAlarmInAttrib[chnn].toFtpCH	= toFtpCH;
	if (ALARMIN_STATUS_NO != m_pAlarmInStatus[chnn])
	{
		int videoInputNum = CProduct::Instance()->VideoInputNum();
		for (unsigned char i=0; i<videoInputNum; ++i)
		{
			if(m_pAlarmInAttrib[chnn].toFtpCH & ((ULONGLONG)0x01 << i))
			{
				//如果是开启报警，就打开上传FTP
#ifdef __ALARM_TO_UPDATE_FTP_VIDEO__
				CFtpUpdateMan::Instance()->StartUpdate(i, GetRecType());
#endif
			}
			else
			{
				//如果是关闭报警，就关闭FTP上传
#ifdef __ALARM_TO_UPDATE_FTP_VIDEO__
				if (m_pAlarmInAttrib[chnn].hasUpdateCH & ((ULONGLONG)0x01 << i))
				{
					CFtpUpdateMan::Instance()->StopUpdate(i, GetRecType());
				}
#endif
			}
		}
	}
	m_lock.UnLock();
}

void CAlarmIn::SetToFTPPic(ULONGLONG toFtpCH, unsigned char chnn)
{

}

ULONGLONG CAlarmIn::AlarmIn(ULONGLONG chnnBits)
{
	m_lock.Lock();

	if (!m_bAction)
	{
		m_lock.UnLock();
		return 0;
	}
	
	time_t currentTime = GetCurrTime32();

	AdjustAlarmIn(chnnBits);

	//分别处理每个通道的报警输入
	for (int i = 0; i < m_chnnNum; ++i)
	{
		if (!m_pAlarmInAttrib[i].bDetection)
		{
			//说明该通道不检测告警输入信号
			continue;
		}

		bool bIsZero = false;
		if ((chnnBits >> i) & ((ULONGLONG)1))
		{
			bIsZero = false;
		}
		else
		{
			bIsZero = true;
		}
		
		bIsZero |= ( !m_pSchedule[i].CheckTime( currentTime) );

		switch (m_pAlarmInStatus[i])
		{
		case ALARMIN_STATUS_NO:
			{ 
				//当前状态是：非告警
				if (bIsZero)
				{
					//如果获取信号为：0，那么继续保持：非告警状态
					//do nothing					
				}
				else
				{
					//如果获取信号为：1, 那么要开启报警，并且进入报警状态	
					//printf("channl %d into Alarmin_Yes\n",i);
					//用标记需要触发报警输入替换即时触发，在Timer再去检查m_pAlarmOut的状态
					m_pAlarmOut[i] = true;
					m_pAlarmInStatus[i] = ALARMIN_STATUS_YES;				
				}
				
			}
			break;
		case ALARMIN_STATUS_YES:
			{
				//当前状态是：报警
				if (bIsZero)
				{
					//如果获取信号为：0, 那么要开启延时计数器，并且进入：报警延迟状态
					//printf("channl %d into Alarmin_Delay\n",i);
					m_pTimer->StartTime(i, m_pAlarmInAttrib[i].holdTime, m_pAlarmInAttrib[i].timerType);
					m_pAlarmInStatus[i] = ALARMIN_STATUS_DELAY;
				}
				else
				{
					//如果获取信号为：1, 那么要继续保持：报警状态
					//do nothing
				}

			}
			break;
		case ALARMIN_STATUS_DELAY:
			{
				//当前状态是：报警延迟阶段
				if (bIsZero)
				{
					//如果获取的信号为：0，那么要继续保持延迟阶段
					//do nothing
					//在timer中结束报警
				}
				else
				{
					//如果获取的信号为： 1，那么要停止报警延迟计数器，并且进入报警状态
					//printf("channl %d into Alarmin_Yes\n",i);	
					m_pTimer->StopTime(i,m_pAlarmInAttrib[i].timerType);
					m_pAlarmInStatus[i] = ALARMIN_STATUS_YES;								
				}
				
			}			
			break;
		
		} //end switch
		
	}//end for
	
	m_status = 0;
	for (int i = 0; i < m_chnnNum; ++i)
	{
		if ((ALARMIN_STATUS_YES == m_pAlarmInStatus[i]) || (ALARMIN_STATUS_DELAY == m_pAlarmInStatus[i]))
		{
			m_status |= ((ULONGLONG)0x01 << i);
#if defined(__CUSTOM_TW01__) || defined(__CUSTOM_TW01TS__) || defined(__CUSTOM_CBC__)
			AlarmOut(i);
#endif
		}
	}

	m_lock.UnLock();

	return m_status;
}
	
void CAlarmIn::Timer()
{
	m_lock.Lock();

	for (unsigned char i = 0; i < m_chnnNum; i++)
	{
		if((ALARMIN_STATUS_YES == m_pAlarmInStatus[i]) || (ALARMIN_STATUS_DELAY == m_pAlarmInStatus[i]))
		{
			if (m_pAlarmOut[i])
			{
				OpenAlarm(i);
				m_pAlarmOut[i] = false;
			}			
		
			if (DVR_TIMER_TIMER_END == m_pTimer->CheckTimer(i, m_pAlarmInAttrib[i].timerType))
			{
				//说明报警延迟结束			
				CloseAlarm(i);
				m_pAlarmInStatus[i] = ALARMIN_STATUS_NO;
				m_pTimer->StopTime(i, m_pAlarmInAttrib[i].timerType);
			}
		}
		else
		{
			//ALARMIN_STATUS_NO
			//do nothing
		}
	}

	m_lock.UnLock();	
}
//////////////////////////////////////////////////////////////////////////
void CAlarmIn::Record(unsigned long chnn)
{
	if (0 != m_pAlarmInAttrib[chnn].toRecCH)
	{
		m_pAlarmInAttrib[chnn].hasRecCH = (ULONGLONG)0;
		int videoInputNum = CProduct::Instance()->VideoInputNum();
		for (BYTE  i = 0; i < videoInputNum; i++)
		{
			if (((m_pAlarmInAttrib[chnn].toRecCH) >> i) & ((ULONGLONG)1))
			{
				if (m_pStreamRecManEx->Record(i, GetRecType()))
				{
					m_pAlarmInAttrib[chnn].hasRecCH |= (((ULONGLONG)1) << i);
				}
			}
		}
	}
}

void CAlarmIn::StopRecord(unsigned long chnn)
{

	if ((0 != m_pAlarmInAttrib[chnn].toRecCH) && (0 != m_pAlarmInAttrib[chnn].hasRecCH))
	{
		int videoInputNum = CProduct::Instance()->VideoInputNum();
		for (BYTE  i = 0; i < videoInputNum; i++)
		{
			if ( ((m_pAlarmInAttrib[chnn].hasRecCH) >> i) & ((ULONGLONG)1))
			{	
				m_pStreamRecManEx->StopRecord(i, GetRecType());
			}
		}

		m_pAlarmInAttrib[chnn].hasRecCH = (ULONGLONG)0;
	}
}

void CAlarmIn::toPTZ(unsigned long chnn)
{
#ifdef __ENVIRONMENT_LINUX__
	CPtzMan *pPTZMan = CPtzMan::Instance();
	CNetDeviceManager *pNetDeviceMan = CNetDeviceManager::Instance();
	int videoInputNum = CProduct::Instance()->VideoInputNum();
	int loccalVideoInputNum =CProduct::Instance()->LocalVideoInputNum();

	for (int i = 0; i < videoInputNum; i++)
	{
		
		if (TO_PTZ_PRESET == m_pAlarmInAttrib[chnn].toPTZType[i])
		{
			if (INVALID_INDEX(unsigned char) != m_pAlarmInAttrib[chnn].toPTZPreset[i])
			{
				if (i < loccalVideoInputNum)
				{
					pPTZMan->Action(i, PTZ_CMD_PRESET_GO, m_pAlarmInAttrib[chnn].toPTZPreset[i]);
				}
				else
				{
					pNetDeviceMan->PTZAction(i,PTZ_CMD_PRESET_GO,m_pAlarmInAttrib[chnn].toPTZPreset[i],5,0);
				}
			}
		}
		else if (TO_PTZ_CRUISE == m_pAlarmInAttrib[chnn].toPTZType[i])	
		{
			if (INVALID_INDEX(unsigned char) != m_pAlarmInAttrib[chnn].toPTZCruise[i])
			{
				if (i < loccalVideoInputNum)
				{
					pPTZMan->Action(i, PTZ_CMD_CRUISE_RUN, m_pAlarmInAttrib[chnn].toPTZCruise[i]);
				}
				else
				{
					pNetDeviceMan->PTZAction(i,PTZ_CMD_CRUISE_RUN,m_pAlarmInAttrib[chnn].toPTZCruise[i],5,0);
				}
			}	
		}
		else if (TO_PTZ_TRACK == m_pAlarmInAttrib[chnn].toPTZType[i])
		{
			if (INVALID_INDEX(unsigned char) != m_pAlarmInAttrib[chnn].toPTZTrack[i])
			{
				if (i < loccalVideoInputNum)
				{
					pPTZMan->Action(i, PTZ_CMD_TRACK_START, m_pAlarmInAttrib[chnn].toPTZTrack[i]);
				}
				else
				{
					pNetDeviceMan->PTZAction(i,PTZ_CMD_TRACK_START,m_pAlarmInAttrib[chnn].toPTZTrack[i],5,0);
				}
			}
		}
	}
#endif
}

void CAlarmIn::StopPTZ(unsigned long chnn)
{
#ifdef __ENVIRONMENT_LINUX__
	CPtzMan *pPTZMan = CPtzMan::Instance();
	CNetDeviceManager *pNetDeviceMan = CNetDeviceManager::Instance();
	int videoInputNum = CProduct::Instance()->VideoInputNum();
	int loccalVideoInputNum =CProduct::Instance()->LocalVideoInputNum();

	for (int i = 0; i < videoInputNum; i++)
	{
		if (TO_PTZ_CRUISE == m_pAlarmInAttrib[chnn].toPTZType[i])	
		{
			if (INVALID_INDEX(unsigned char) != m_pAlarmInAttrib[chnn].toPTZCruise[i])
			{
				if (i < loccalVideoInputNum)
				{
					pPTZMan->Action(i, PTZ_CMD_CRUISE_STOP, m_pAlarmInAttrib[chnn].toPTZCruise[i]);
				}
				else
				{
					pNetDeviceMan->PTZAction(i,PTZ_CMD_CRUISE_STOP,m_pAlarmInAttrib[chnn].toPTZCruise[i],5,0);
				}
			}	
		}
		else if (TO_PTZ_TRACK == m_pAlarmInAttrib[chnn].toPTZType[i])
		{
			if (INVALID_INDEX(unsigned char) != m_pAlarmInAttrib[chnn].toPTZTrack[i])
			{
				if (i < loccalVideoInputNum)
				{
					pPTZMan->Action(i, PTZ_CMD_TRACK_STOP, m_pAlarmInAttrib[chnn].toPTZTrack[i]);
				}
				else
				{
					pNetDeviceMan->PTZAction(i,PTZ_CMD_TRACK_STOP,m_pAlarmInAttrib[chnn].toPTZTrack[i],5,0);
				}
			}
		}
	}
#endif
}

void CAlarmIn::AlarmOut(unsigned long chnn)
{
	//只管开启报警输出，不管关闭，关闭由AlarmOutMan延时完成关闭。
	if (0 != m_pAlarmInAttrib[chnn].toAlarmOut)
	{
		m_pAlarmOutMan->AlarmOut(m_pAlarmInAttrib[chnn].toAlarmOut);
	}
}

void CAlarmIn::CloseAlarmOut(unsigned long chnn)
{
	//只管开启报警输出，不管关闭，关闭由AlarmOutMan延时完成关闭。
	if (0 != m_pAlarmInAttrib[chnn].toAlarmOut)
	{
		m_pAlarmOutMan->CloseAlarmOut(m_pAlarmInAttrib[chnn].toAlarmOut);
	}
}

void CAlarmIn::BigView(unsigned long chnn)
{
	if (0 != m_pAlarmInAttrib[chnn].toBigViewCH)
	{
		unsigned long splitMode[8] = {0};
		splitMode[DISPLAY_LAYER_ANALOG] = VIEW_SPLIT_1X1;
		splitMode[DISPLAY_LAYER_NET] = VIEW_SPLIT_1X1;
		unsigned long dataChnn[64] = {0};

		int numChnn = m_pDisplayMan->GetVideoInputNum();
		int localNumChnn = m_pDisplayMan->GetLocalVideoInputNum();

		int index = 0;
		for(int i = 0; i < numChnn; i++)
		{
			if(m_pAlarmInAttrib[chnn].toBigViewCH & (static_cast<ULONGLONG>(0x1) << i))
			{
				if(i < localNumChnn)
				{
					dataChnn[index] = DISPLAY_PACK(DISPLAY_LAYER_ANALOG, DISPLAY_SIGNAL_ANALOG, 0, i);
				}
				else
				{
					dataChnn[index] = DISPLAY_PACK(DISPLAY_LAYER_NET, DISPLAY_SIGNAL_NET, 0, i - localNumChnn);
				}
				index++;
				break;
			}
		}

		m_pDisplayMan->ChangeDisplay(splitMode, 8, dataChnn, index, m_pAlarmInAttrib[chnn].holdTime, DISPLAY_UNRECORD, DISPLAY_NOTIFY);
				
	}
}

void CAlarmIn::SendEmail(unsigned long chnn)
{
	if (0 != m_pAlarmInAttrib[chnn].toEmail)
	{
		bool bSend = true;
#if defined (__ENVIRONMENT_LINUX__) && defined (__DVR_BASIC__)
		LONGLONG curTime = GetCurrTime32();
		if (curTime < m_pLastSendEmailTime[chnn])
		{
			m_pLastSendEmailTime[chnn] = curTime;
			bSend = true;
		}
		else if (m_pLastSendEmailTime[chnn] + CSnapProc::Instance()->GetSendEmailPeriod() <= curTime)
		{
			m_pLastSendEmailTime[chnn] = curTime;
			bSend = true;
		}
		else
		{
			bSend = false;
		}		
#else
		bSend = false;
#endif		
		if (bSend)
		{
			std::string name;
			char szAlarmType[48] = {0};
			char szTime[48] = {0};

			ALARM_TYPE alarmTypeTemp = GetAlarmType(chnn);
			if ((ALARM_TYPE_MONTION == alarmTypeTemp) 
			  ||(ALARM_TYPE_VIDEOLOSS == alarmTypeTemp))
			{
				CMessageMan::Instance()->GetCamName(chnn, name);
			}
			else if(ALARM_TYPE_SENSOR == alarmTypeTemp)
			{
				CMessageMan::Instance()->GetSensorName(chnn, name);
			}
			else
			{
				name = "channel";
			}
			

			tm time = DVRTime32ToTm(GetCurrTime32());
			snprintf(szTime, 48, "%d-%d-%d %02d:%02d:%02d", time.tm_year + 1900, time.tm_mon + 1, time.tm_mday, time.tm_hour, time.tm_min, time.tm_sec);

#if defined (__ENVIRONMENT_LINUX__) && defined (__DVR_BASIC__)
			long localChannelNum = CProduct::Instance()->LocalVideoInputNum();
			CSnapProc::Instance()->SendEmail(chnn, name.c_str(), GetAlarmType(chnn), szTime, m_pAlarmInAttrib[chnn].toSnapCH & (ALL_CHNN_MASK(localChannelNum) | g_canSnapPicCH));
#endif
		}
	}
}


void CAlarmIn::SnapPicture(unsigned long chnn)
{
#if defined (__ENVIRONMENT_LINUX__) && defined (__DVR_BASIC__)
	if (g_bHaveSnapPic)
	{
		if (m_pAlarmInAttrib[chnn].toSnapCH > 0)
		{
			int sensorNum = 0;
#ifdef __DVR_ULTIMATE__
			sensorNum = CProduct::Instance()->SensorInputNum()+CProduct::Instance()->VideoInputNum();
#else
			sensorNum = CProduct::Instance()->VideoInputNum();
#endif
			for (int i = 0; i < sensorNum; i++)
			{
				if (((m_pAlarmInAttrib[chnn].toSnapCH) >> i) & ((ULONGLONG)1))
				{
					
#ifdef __SUPPORT_SNAPD1__
					bool bsendPic =false;
					ALARM_TYPE alarmTypeTemp = GetAlarmType(i);
					switch(alarmTypeTemp)
					{
					case ALARM_TYPE_MONTION:
						{
							CShanghaiSnapMan::GetInstance()->GetAlarmType(DVRSYSMOTION,i);
							bsendPic = true;
						}
						break;
					case ALARM_TYPE_SENSOR:
						{	
							CShanghaiSnapMan::GetInstance()->GetAlarmType(DVROUTSIDEALARM,i);
							bsendPic = true;
						}
						break;
					default:
						break;
					}
					if(bsendPic)
					{
						CShanghaiSnapMan::GetInstance()->GetAlarmTime(i);
						CShanghaiSnapMan::GetInstance()->SetSnapPicture(i, 1);
					}
#else
					CSnapPictureMan::GetInstance()->SetSnapPicture(i, 1);
#endif
				}
			}		
		}	
	}
#endif
}

void CAlarmIn::StartFtpUpdate(unsigned long chnn)
{
	if (m_pAlarmInAttrib[chnn].toFtpCH)
	{
		m_pAlarmInAttrib[chnn].hasUpdateCH = (ULONGLONG)0;
		int videoInputNum = CProduct::Instance()->VideoInputNum();
		for (int i = 0; i < videoInputNum; i++)
		{
			if (((m_pAlarmInAttrib[chnn].toFtpCH) >> i) & ((ULONGLONG)1))
			{
#ifdef __ALARM_TO_UPDATE_FTP_VIDEO__
				CFtpUpdateMan::Instance()->StartUpdate(i, GetRecType());
#endif
				m_pAlarmInAttrib[chnn].hasUpdateCH |= (((ULONGLONG)0x01) << i);
			}
		}
	}
}

void CAlarmIn::StopFtpUpdate(unsigned long chnn)
{
	if (m_pAlarmInAttrib[chnn].toFtpCH)
	{
		int videoInputNum = CProduct::Instance()->VideoInputNum();
		for (int i = 0; i < videoInputNum; i++)
		{
			if (((m_pAlarmInAttrib[chnn].toFtpCH) >> i) & ((ULONGLONG)1))
			{
#ifdef __ALARM_TO_UPDATE_FTP_VIDEO__
				CFtpUpdateMan::Instance()->StopUpdate(i, GetRecType());
#endif
			}
		}

		m_pAlarmInAttrib[chnn].hasUpdateCH = (ULONGLONG)0;
	}
}


