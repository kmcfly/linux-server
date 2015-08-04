
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
	memset( &defaultWeekSchdule, 0xff, sizeof(WEEK_SCHEDULE) );		//��sensor��motion��videoLoss��Ĭ��week schdule����Ϊ��Ч
	
	m_lock.Lock();
	
	m_chnnNum = chnnNum;

	m_pAlarmOut = new bool [m_chnnNum];	//��¼��Ҫ��������
	m_pAlarmInStatus = new int[m_chnnNum]; //��¼��������ĵ�ǰ״̬�� ����ͨ���� 
	m_pAlarmInAttrib = new ALARM_ATTRIB[m_chnnNum]; //������������ԣ�����ͨ����
	m_pEventInfo = new EVENT_LOG[m_chnnNum];

	m_status			= 0;
	
	//��ʼ����ͨ����״̬Ϊ���Ǳ���
	memset(m_pAlarmOut, 0, m_chnnNum*sizeof(bool));
	memset(m_pAlarmInAttrib, 0, m_chnnNum*sizeof(ALARM_ATTRIB));
	memset(m_pEventInfo, 0, m_chnnNum * sizeof(EVENT_LOG));

	//���ʻ�״̬������������Ҫ��д�÷�����
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

	//�˳�ǰ��ֹͣ¼��ֹͣ��ʱ��
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

	delete [] m_pAlarmOut;	//��¼��Ҫ��������
	delete [] m_pAlarmInStatus; //��¼��������ĵ�ǰ״̬�� ����ͨ���� 
	delete [] m_pAlarmInAttrib; //������������ԣ�����ͨ����
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
		if (!m_pAlarmOut[chnn]) //m_pAlarmOut[chnn]Ϊ��ʱ����ʾ���ͨ���ڽ�����Ҫ�������������Ǳ�����û�д���
			                    //m_pAlarmOut[chnn]Ϊ��ʱ����ʾ���ͨ����Ҫ����������ոմ����걨��
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
			//���û�п���¼�񣬵�����������Ҫ¼������¼��
			if(!(m_pAlarmInAttrib[chnn].hasRecCH & ((ULONGLONG)0x01 << i)) && (m_pAlarmInAttrib[chnn].toRecCH & ((ULONGLONG)0x01 << i)))
			{
				m_pStreamRecManEx->Record(i, GetRecType());
				m_pAlarmInAttrib[chnn].hasRecCH |= ((ULONGLONG)0x01 << i);
			}

			//����Ѿ�����¼�񣬶�������Ҫ�ر�¼����ر�¼��
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
	//�޸ģ�ֻ�иı��˼̵������Ž��м̵������������
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
	//�����޸ĺ���
	m_pAlarmInAttrib[chnn].toAlarmOut   &= ( (ULONGLONG)(0x01) << relayOutNum );						//����������λ������������λ
	m_pAlarmInAttrib[chnn].toAlarmOut	|= (toAlarmOutCH & ALL_CHNN_MASK(relayOutNum));

	m_lock.UnLock();
}

void  CAlarmIn::SetToBigViewCH(ULONGLONG toBigViewCH, unsigned char chnn)
{
	assert (chnn < m_chnnNum);

	m_lock.Lock();
	
	//�޸ģ�ֻ�иı��ˣ��Ž��б�����
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

	//�޸ģ�ֻ�иı��˷��������Ž��з�����������
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

	//�����޸ĺ�Ľ��
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
			1:���ñ��������������̨�Ĳ���

Calls:

Called by:

Table Accessed:

Table Updated:

Input:
			1:*pToPTZ��ű�������ʱҪ��������̨������Ϣ
			2:externDeviceNum��ʾ���ӵ��ⲿ����������Ϣ�� 
			  ���������16�����������룬16����Ƶ����(motion��videoloss)
			3:localVInput��ʾ����DVR�Ͽνӵ���̨����

			��ӵ������һ������������Դ������е���̨����������һ����������������Դ���������̨�Ķ�����
			ͬ��һ���ƶ�����¼�Ҳ���Դ���������̨�Ķ���
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
			//����Ϊ�������κ�PTZ����
			m_pAlarmInAttrib[alarmInput].toPTZType[ch] = TO_PTZ_NULL;
			m_pAlarmInAttrib[alarmInput].toPTZPreset[ch] = 0xff;
			m_pAlarmInAttrib[alarmInput].toPTZCruise[ch] = 0xff;
			m_pAlarmInAttrib[alarmInput].toPTZTrack[ch]  = 0xff;
			
			if( pToPTZ[index].cIsEnablePreset )
			{
				//��������Ԥ�õ�
				m_pAlarmInAttrib[alarmInput].toPTZType[ch] = TO_PTZ_PRESET;
				m_pAlarmInAttrib[alarmInput].toPTZPreset[ch] = pToPTZ[index].ucToPreset;

			}
			else if( pToPTZ[index].cIsEnableCruise )
			{
				//��������Ѳ��
				m_pAlarmInAttrib[alarmInput].toPTZType[ch] = TO_PTZ_CRUISE;
				m_pAlarmInAttrib[alarmInput].toPTZCruise[ch] = pToPTZ[index].ucToCruise;

			}
			else if( pToPTZ[index].cIsEnableTrack )
			{
				//���������켣
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

	//�޸ģ�ֻ�иı��ˣ��Ž��б�����
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

	//�޸ģ�ֻ�иı��ˣ��Ž��б�����
	
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
				//����ǿ����������ʹ��ϴ�FTP
#ifdef __ALARM_TO_UPDATE_FTP_VIDEO__
				CFtpUpdateMan::Instance()->StartUpdate(i, GetRecType());
#endif
			}
			else
			{
				//����ǹرձ������͹ر�FTP�ϴ�
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

	//�ֱ���ÿ��ͨ���ı�������
	for (int i = 0; i < m_chnnNum; ++i)
	{
		if (!m_pAlarmInAttrib[i].bDetection)
		{
			//˵����ͨ�������澯�����ź�
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
				//��ǰ״̬�ǣ��Ǹ澯
				if (bIsZero)
				{
					//�����ȡ�ź�Ϊ��0����ô�������֣��Ǹ澯״̬
					//do nothing					
				}
				else
				{
					//�����ȡ�ź�Ϊ��1, ��ôҪ�������������ҽ��뱨��״̬	
					//printf("channl %d into Alarmin_Yes\n",i);
					//�ñ����Ҫ�������������滻��ʱ��������Timer��ȥ���m_pAlarmOut��״̬
					m_pAlarmOut[i] = true;
					m_pAlarmInStatus[i] = ALARMIN_STATUS_YES;				
				}
				
			}
			break;
		case ALARMIN_STATUS_YES:
			{
				//��ǰ״̬�ǣ�����
				if (bIsZero)
				{
					//�����ȡ�ź�Ϊ��0, ��ôҪ������ʱ�����������ҽ��룺�����ӳ�״̬
					//printf("channl %d into Alarmin_Delay\n",i);
					m_pTimer->StartTime(i, m_pAlarmInAttrib[i].holdTime, m_pAlarmInAttrib[i].timerType);
					m_pAlarmInStatus[i] = ALARMIN_STATUS_DELAY;
				}
				else
				{
					//�����ȡ�ź�Ϊ��1, ��ôҪ�������֣�����״̬
					//do nothing
				}

			}
			break;
		case ALARMIN_STATUS_DELAY:
			{
				//��ǰ״̬�ǣ������ӳٽ׶�
				if (bIsZero)
				{
					//�����ȡ���ź�Ϊ��0����ôҪ���������ӳٽ׶�
					//do nothing
					//��timer�н�������
				}
				else
				{
					//�����ȡ���ź�Ϊ�� 1����ôҪֹͣ�����ӳټ����������ҽ��뱨��״̬
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
				//˵�������ӳٽ���			
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
	//ֻ�ܿ���������������ܹرգ��ر���AlarmOutMan��ʱ��ɹرա�
	if (0 != m_pAlarmInAttrib[chnn].toAlarmOut)
	{
		m_pAlarmOutMan->AlarmOut(m_pAlarmInAttrib[chnn].toAlarmOut);
	}
}

void CAlarmIn::CloseAlarmOut(unsigned long chnn)
{
	//ֻ�ܿ���������������ܹرգ��ر���AlarmOutMan��ʱ��ɹرա�
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


