/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : zhanglei
** Date         : 2010-4-16
** Name         :SmtpMan.cpp
** Version      : 1.0
** Description  :Send Email
** Modify Record:
***********************************************************************/
	
#include "SmtpMan.h"
#include "mylist.cpp"
#include "MessageMan.h"
#include "LocalDevice.h"
#include "StringTable.h"
#include "SnapCapabilityMan.h"

#ifdef __ENVIRONMENT_LINUX__
//CSnapProc/////////////////////////////////////////////////////////////////
CSnapProc  *CSnapProc::m_pInstance = NULL;


CSnapProc *CSnapProc::Instance()
{
	if( NULL == m_pInstance )
	{
		m_pInstance = new CSnapProc;
	}
	return m_pInstance;
}


CSnapProc::CSnapProc()
{
	m_channelNum      = 0;
	m_snapImgNum      = 0;
	m_bSnapImg        = false;
	m_snapPeriod      = 0;
	m_pSnapPeriodChnn = NULL;
	m_pLastJPEGTime   = NULL;

	m_pDataBuffer   = NULL;
	m_DataBufferNum = 0;
	
	m_pChnnSnaptoEmailIdList = NULL;
	
	
	
}

CSnapProc::~CSnapProc()
{


}


bool CSnapProc::Initial()
{
	//
	int channelNum = CProduct::Instance()->VideoInputNum();
	m_channelNum = channelNum;
	m_snapImgNum = 0;
	m_bSnapImg   = false;
	m_snapPeriod = 0;
	if (NULL == m_pSnapPeriodChnn)
	{
		m_pSnapPeriodChnn = new int [channelNum];
	}
	memset(m_pSnapPeriodChnn, 0, sizeof(int)*channelNum);
	if (NULL == m_pLastJPEGTime)
	{
		m_pLastJPEGTime = new LONGLONG [channelNum];
	}
	memset(m_pLastJPEGTime, 0, sizeof(int)*channelNum);

	//
	m_DataBufferNum = channelNum*2;	
	
	m_pDataBuffer = new EMAIL_BUFFER [m_DataBufferNum];
	memset(m_pDataBuffer, 0, sizeof(EMAIL_BUFFER)*m_DataBufferNum);
	for (int i = 0; i < m_DataBufferNum; ++i)
	{
		m_pDataBuffer[i].pDataBuff = new unsigned char [ALARM_EMAIL_MAX_SNAP_IMAGE_COUNT*JPEG_MAX_LENGTH];
		memset(m_pDataBuffer[i].pDataBuff, 0, ALARM_EMAIL_MAX_SNAP_IMAGE_COUNT * JPEG_MAX_LENGTH);
	}
	
	//	
	if (NULL == m_pChnnSnaptoEmailIdList)
	{
		m_pChnnSnaptoEmailIdList = new CMyList<int> [channelNum];
	}
	m_EmailInfoList.RemoveAll();
	
	return true;
}

void CSnapProc::Quit()
{
	//
	m_channelNum      = 0;
	m_snapImgNum      = 0;
	m_bSnapImg        = false;
	m_snapPeriod      = 0;
	if (m_pSnapPeriodChnn != NULL)
	{
		delete [] m_pSnapPeriodChnn;
		m_pSnapPeriodChnn = NULL;
	}
	if (m_pLastJPEGTime != NULL)
	{
		delete [] m_pLastJPEGTime;
		m_pLastJPEGTime = NULL;
	}

	//
	if (m_pDataBuffer != NULL)
	{
		for (int i = 0; i < m_DataBufferNum; ++i)
		{
			delete [] m_pDataBuffer[i].pDataBuff;
		}
		delete [] m_pDataBuffer;
		m_pDataBuffer = NULL;		
	}
	m_DataBufferNum = 0;

	//    
	if (m_pChnnSnaptoEmailIdList != NULL)
	{
		for (int i = 0; i < m_channelNum; ++i)
		{
			m_pChnnSnaptoEmailIdList[i].RemoveAll();
		}		
		delete [] m_pChnnSnaptoEmailIdList;
		m_pChnnSnaptoEmailIdList = NULL;
	}

	POS pos = m_EmailInfoList.GetHeadPosition();
	while (pos != NULL)
	{
		EMAIL_INFO &emailInfo = m_EmailInfoList.GetAt(pos);
		if (emailInfo.pAlarmInfoList != NULL)
		{
			emailInfo.pAlarmInfoList->RemoveAll();
			delete emailInfo.pAlarmInfoList;
			emailInfo.pAlarmInfoList = NULL;
		}
		if (emailInfo.pJPEGInfoList != NULL)
		{
			POS jpegPos = emailInfo.pJPEGInfoList->GetHeadPosition();
			while (jpegPos != NULL)
			{
				EMAIL_CH_JPEG_INFO &chJpegInfo = emailInfo.pJPEGInfoList->GetAt(jpegPos);
				chJpegInfo.pJPEGInfo->RemoveAll();
				delete chJpegInfo.pJPEGInfo;
				chJpegInfo.pJPEGInfo = NULL;
				emailInfo.pJPEGInfoList->GetNext(jpegPos);
			}
			emailInfo.pJPEGInfoList->RemoveAll();
			delete emailInfo.pJPEGInfoList;
			emailInfo.pJPEGInfoList = NULL;			
		}
		m_EmailInfoList.GetNext(pos);
	}
	m_EmailInfoList.RemoveAll();
}

void CSnapProc::Action(bool bEnable)
{
	m_SnapLock.Lock();
	if (!bEnable)
	{
		memset(m_pSnapPeriodChnn, 0, sizeof(int)*m_channelNum);
		memset(m_pLastJPEGTime, 0, sizeof(LONGLONG)*m_channelNum);
		
		POS pos = m_EmailInfoList.GetHeadPosition();
		while (pos != NULL)
		{
			EMAIL_INFO &emailInfo = m_EmailInfoList.GetAt(pos);
			if (emailInfo.pAlarmInfoList != NULL)
			{
				emailInfo.pAlarmInfoList->RemoveAll();
				delete emailInfo.pAlarmInfoList;
				emailInfo.pAlarmInfoList = NULL;
			}
			if (emailInfo.pJPEGInfoList != NULL)
			{
				POS jpegPos = emailInfo.pJPEGInfoList->GetHeadPosition();
				while (jpegPos != NULL)
				{
					EMAIL_CH_JPEG_INFO &chJpegInfo = emailInfo.pJPEGInfoList->GetAt(jpegPos);
					chJpegInfo.pJPEGInfo->RemoveAll();
					delete chJpegInfo.pJPEGInfo;
					chJpegInfo.pJPEGInfo = NULL;
					emailInfo.pJPEGInfoList->GetNext(jpegPos);
				}
				emailInfo.pJPEGInfoList->RemoveAll();
				delete emailInfo.pJPEGInfoList;
				emailInfo.pJPEGInfoList = NULL;			
			}
			m_EmailInfoList.GetNext(pos);
		}
		m_EmailInfoList.RemoveAll();

		if (m_pChnnSnaptoEmailIdList != NULL)
		{
			for (int i = 0; i < m_channelNum; ++i)
			{
				m_pChnnSnaptoEmailIdList[i].RemoveAll();
			}			
		}


		for (int i = 0; i < m_DataBufferNum; ++i)
		{
			m_pDataBuffer[i].bUse = false;
		}
	}
	m_bAction = bEnable;

	m_SnapLock.UnLock();
}

bool CSnapProc::Start()
{
	m_bAction = true;

	return true;
}

void CSnapProc::Stop()
{

}


//void CSnapProc::SendEmail(unsigned long channel, const char *pszChannelName, ALARM_TYPE alarmType, char *pszTime)
//{
//	assert( NULL != pszChannelName );
//	assert( NULL != pszTime );
//
//	m_SnapLock.Lock();
//	if (!m_bAction)
//	{
//		m_SnapLock.UnLock();
//		return;
//	}
//
//	if ((alarmType != ALARM_TYPE_MONTION) 
//		&&(alarmType != ALARM_TYPE_SENSOR)
//		&&(alarmType != ALARM_TYPE_VIDEOLOSS))
//	{
//		EMAIL_INFO info;
//		memset(&info, 0, sizeof(EMAIL_INFO));
//		info.chn = INVALID_INDEX(unsigned long);
//		snprintf(info.szChannelName, sizeof(info.szChannelName), "%s", "channel");
//		info.pAlarmInfoList = new CMyList <EMAIL_ALARM_INFO>;
//		EMAIL_ALARM_INFO temp;
//		temp.alarmType = alarmType;
//		snprintf(temp.szTime, sizeof(temp.szTime), "%s", pszTime);
//		info.pAlarmInfoList->AddTail(temp);
//		info.pJPEGInfoList = NULL;
//		info.bufferIndex = -1;
//		CMessageMan::Instance()->PostMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_SEND_EMAIL, (unsigned char *)(&info), sizeof(EMAIL_INFO));
//		m_SnapLock.UnLock();
//		return;
//	}
//
//	if (0 == m_snapImgNum)
//	{
//		EMAIL_INFO info;
//		memset(&info, 0, sizeof(EMAIL_INFO));
//		info.chn = channel;
//		snprintf(info.szChannelName, sizeof(info.szChannelName), "%s", pszChannelName);
//		info.pAlarmInfoList = new CMyList <EMAIL_ALARM_INFO>;
//		EMAIL_ALARM_INFO temp;
//		temp.alarmType = alarmType;
//		snprintf(temp.szTime, sizeof(temp.szTime), "%s", pszTime);
//		info.pAlarmInfoList->AddTail(temp);
//		info.pJPEGInfoList = NULL;
//		info.bufferIndex = -1;
//		CMessageMan::Instance()->PostMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_SEND_EMAIL, (unsigned char *)(&info), sizeof(EMAIL_INFO));
//		m_SnapLock.UnLock();
//		return;
//	}
//
//	if (0 == m_pSnapChnnStatus[channel])
//	{
//		int bufferIndex = UseEmailBuffer();
//		if (bufferIndex < 0)
//		{
//			EMAIL_INFO info;
//			memset(&info, 0, sizeof(EMAIL_INFO));
//			info.chn = channel;
//			snprintf(info.szChannelName, sizeof(info.szChannelName), "%s", pszChannelName);
//			info.pAlarmInfoList = new CMyList <EMAIL_ALARM_INFO>;
//			EMAIL_ALARM_INFO temp;
//			temp.alarmType = alarmType;
//			snprintf(temp.szTime, sizeof(temp.szTime), "%s", pszTime);
//			info.pAlarmInfoList->AddTail(temp);
//			info.pJPEGInfoList = NULL;
//			info.bufferIndex = -1;
//			CMessageMan::Instance()->PostMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_SEND_EMAIL, (unsigned char *)(&info), sizeof(EMAIL_INFO));
//			m_SnapLock.UnLock();
//			return;
//		}
//		else
//		{
//			printf("%s,%d,star snap\n",__FILE__,__LINE__);
//#ifndef __ALARM_SNAP_JPEG__
//			CLocalDevice::Instance()->SetEmailSnapEnable(channel, true);
//#else
//			CSnapCapabilityMan::Instance()->StarSnapJpeg(channel, false);
//#endif
//			assert(0 == m_pSnapChnnCount[channel]);
//			m_pSnapChnnStatus[channel] = 1;
//			m_pSnapChnnCount[channel] = m_snapImgNum;
//			m_pLastJPEGTime[channel] = 0;
//			m_pSnapPeriodChnn[channel] = m_snapPeriod;
//			m_pEmailInfo[channel].chn = channel;
//			snprintf(m_pEmailInfo[channel].szChannelName, sizeof(m_pEmailInfo[channel].szChannelName), "%s", pszChannelName);
//			m_pEmailInfo[channel].pAlarmInfoList = new CMyList <EMAIL_ALARM_INFO>;
//			EMAIL_ALARM_INFO temp;
//			temp.alarmType = alarmType;
//			snprintf(temp.szTime, sizeof(temp.szTime), "%s", pszTime);
//			m_pEmailInfo[channel].pAlarmInfoList->AddTail(temp);		
//			m_pEmailInfo[channel].pJPEGInfoList = new CMyList <EMAIL_JPEG_INFO>;
//			m_pEmailInfo[channel].bufferIndex = bufferIndex;
//			m_pEmailInfo[channel].curDataLength = 0;
//			m_SnapLock.UnLock();
//			return;
//		}		
//	}
//	else if (1 == m_pSnapChnnStatus[channel])
//	{
//		printf("%s,%d,snaping\n",__FILE__,__LINE__);
//		assert(m_pSnapChnnCount[channel] > 0);
//		assert(m_pEmailInfo[channel].bufferIndex > -1);
//		assert(m_pEmailInfo[channel].pJPEGInfoList != NULL);
//		assert(m_pEmailInfo[channel].pAlarmInfoList != NULL);
//		//正在抓取图片时，这个通道又来了一次报警。处理方法是：对应这种报警，只需保留
//		//报警信息即可，但是也没有必要保留太多。
//		if (m_pEmailInfo[channel].pAlarmInfoList->GetCount() < 16)
//		{
//			EMAIL_ALARM_INFO temp;
//			temp.alarmType = alarmType;
//			snprintf(temp.szTime, sizeof(temp.szTime), "%s", pszTime);
//			m_pEmailInfo[channel].pAlarmInfoList->AddTail(temp);
//		}
//		m_SnapLock.UnLock();
//		return;
//	}
//	else
//	{
//		assert(false);
//		m_SnapLock.UnLock();
//		return;
//	}
//
//
//}

void CSnapProc::SendEmail(unsigned long channel, const char *pszChannelName, ALARM_TYPE alarmType, char *pszTime, ULONGLONG toSnapCH)
{
	
	assert( NULL != pszChannelName );
	assert( NULL != pszTime );

	m_SnapLock.Lock();
	if (!m_bAction)
	{
		m_SnapLock.UnLock();
		return;
	}

	if ((alarmType != ALARM_TYPE_MONTION) 
	  &&(alarmType != ALARM_TYPE_SENSOR)
	  &&(alarmType != ALARM_TYPE_VIDEOLOSS))
	{
		EMAIL_INFO info;
		info.id = -1;
		info.chn = INVALID_INDEX(unsigned long);
		snprintf(info.szChannelName, sizeof(info.szChannelName), "%s", "channel");
		info.pAlarmInfoList = new CMyList <EMAIL_ALARM_INFO>;
		EMAIL_ALARM_INFO temp;
		temp.alarmType = alarmType;
		snprintf(temp.szTime, sizeof(temp.szTime), "%s", pszTime);
		info.pAlarmInfoList->AddTail(temp);
		info.pJPEGInfoList = NULL;		
		CMessageMan::Instance()->PostMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_SEND_EMAIL, (unsigned char *)(&info), sizeof(EMAIL_INFO));
		m_SnapLock.UnLock();
		return;
	}

	if ((!m_bSnapImg)||(0 == m_snapImgNum) || (0 == toSnapCH))
	{
		//printf("%s,%d,chnn:%d,no pic email,m_snapImgNum=%d,toSnapCH=%d\n",__FILE__,__LINE__,channel,m_snapImgNum,toSnapCH);
		EMAIL_INFO info;
		info.id = -1;
		info.chn = channel;
		snprintf(info.szChannelName, sizeof(info.szChannelName), "%s", pszChannelName);
		info.pAlarmInfoList = new CMyList <EMAIL_ALARM_INFO>;
		EMAIL_ALARM_INFO temp;
		temp.alarmType = alarmType;
		snprintf(temp.szTime, sizeof(temp.szTime), "%s", pszTime);
		info.pAlarmInfoList->AddTail(temp);
		info.pJPEGInfoList = NULL;		
		CMessageMan::Instance()->PostMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_SEND_EMAIL, (unsigned char *)(&info), sizeof(EMAIL_INFO));
        m_SnapLock.UnLock();
		return;
	}

	{
		EMAIL_INFO info;		
		info.chn = channel;
		snprintf(info.szChannelName, sizeof(info.szChannelName), "%s", pszChannelName);
		info.pAlarmInfoList = new CMyList <EMAIL_ALARM_INFO>;
		EMAIL_ALARM_INFO temp;
		temp.alarmType = alarmType;
		snprintf(temp.szTime, sizeof(temp.szTime), "%s", pszTime);
		info.pAlarmInfoList->AddTail(temp);
		info.pJPEGInfoList = new CMyList <EMAIL_CH_JPEG_INFO>;	
		if (((ULONGLONG)(0x01) << channel) & toSnapCH)
		{
			EMAIL_CH_JPEG_INFO chnJpegInfo;
			chnJpegInfo.channel = channel;
			chnJpegInfo.curDataLength = 0;			
			int bufferIndex = UseEmailBuffer();
			if (bufferIndex < 0)
			{
				chnJpegInfo.snapCount = 0;
				chnJpegInfo.bufferIndex = -1;
				chnJpegInfo.pJPEGInfo = NULL;
			}
			else
			{
				chnJpegInfo.snapCount = m_snapImgNum;
				chnJpegInfo.bufferIndex = bufferIndex;
				chnJpegInfo.pJPEGInfo = new CMyList <EMAIL_JPEG_INFO>;
				info.pJPEGInfoList->AddTail(chnJpegInfo);
			}
		}	
		for (int i = 0; i < m_channelNum; ++i)
		{
			if (i != channel)
			{
				if (((ULONGLONG)(0x01) << i) & toSnapCH)
				{
					EMAIL_CH_JPEG_INFO chnJpegInfo;
					chnJpegInfo.channel = i;
					chnJpegInfo.curDataLength = 0;
					int bufferIndex = UseEmailBuffer();
					if (bufferIndex < 0)
					{
						chnJpegInfo.snapCount = 0;
						chnJpegInfo.bufferIndex = -1;
						chnJpegInfo.pJPEGInfo = NULL;
						break;
					}
					else
					{
						chnJpegInfo.snapCount = m_snapImgNum;
						chnJpegInfo.bufferIndex = bufferIndex;
						chnJpegInfo.pJPEGInfo = new CMyList <EMAIL_JPEG_INFO>;
						info.pJPEGInfoList->AddTail(chnJpegInfo);
					}

				}
			}		
		}
		if (info.pJPEGInfoList->IsEmpty())
		{
			//printf("%s,%d,chn:%d,no pig because of no buffer\n",__FILE__,__LINE__,channel);
			delete info.pJPEGInfoList;
			info.pJPEGInfoList = NULL;
			info.id = -1;
			CMessageMan::Instance()->PostMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_SEND_EMAIL, (unsigned char *)(&info), sizeof(EMAIL_INFO));
			m_SnapLock.UnLock();
			return;
		}
		else
		{
			info.id = info.pJPEGInfoList->GetHead().bufferIndex;
		}
		
		//printf("%s,%d,chnn%d send email\n",__FILE__,__LINE__,channel);
		POS pos = info.pJPEGInfoList->GetHeadPosition();
		while (pos != NULL)
		{
			EMAIL_CH_JPEG_INFO &chnJpegInfoTemp = info.pJPEGInfoList->GetAt(pos);
			if (m_pChnnSnaptoEmailIdList[chnJpegInfoTemp.channel].IsEmpty())
			{
				//printf("%s,%d,channel:%d,snaping\n",__FILE__,__LINE__,chnJpegInfoTemp.channel);
				//CLocalDevice::Instance()->SetEmailSnapEnable(chnJpegInfoTemp.channel, true);

#if !defined(__SUPPORT_HONGDI__)
				CSnapCapabilityMan::Instance()->StarSnapJpeg(chnJpegInfoTemp.channel, false);
#endif
				//printf("                  chn:%d,snap pig use buffid:%d\n",chnJpegInfoTemp.channel,chnJpegInfoTemp.bufferIndex);
				m_pLastJPEGTime[chnJpegInfoTemp.channel] = 0;
				m_pSnapPeriodChnn[chnJpegInfoTemp.channel] = m_snapPeriod;

			}
			m_pChnnSnaptoEmailIdList[chnJpegInfoTemp.channel].AddTail(info.id);
			info.pJPEGInfoList->GetNext(pos);
		}

		m_EmailInfoList.AddTail(info);
		
		m_SnapLock.UnLock();
		return;
	}
}
	



void CSnapProc::SetSnapTimeS(const unsigned long timeS)
{
	m_SnapLock.Lock();
	m_snapPeriod = timeS;
	m_SnapLock.UnLock();
}

void CSnapProc::SetSnapImgNum(unsigned long imgNum)
{
	m_SnapLock.Lock();	
	if (imgNum > ALARM_EMAIL_MAX_SNAP_IMAGE_COUNT)
	{
		imgNum = ALARM_EMAIL_MAX_SNAP_IMAGE_COUNT;
	}
	
	m_snapImgNum = imgNum;
	printf("########%s,%d,m_snapImgNum:%d\n",__FILE__,__LINE__,m_snapImgNum);
	m_SnapLock.UnLock();
}

void CSnapProc::SetSnapImg(unsigned long bSnapImg)
{
	m_SnapLock.Lock();

	if (bSnapImg > 0)
	{
		m_bSnapImg = true;
	}
	else
	{
		m_bSnapImg = false;
	}
	printf("########%s,%d,m_bSnapImg:%d\n",__FILE__,__LINE__,m_bSnapImg);
	m_SnapLock.UnLock();
}

void CSnapProc::ReleaseBuffer(int bufferIndex)
{
	m_SnapLock.Lock();
	assert((bufferIndex > -1) && (bufferIndex < m_DataBufferNum));
	assert(m_pDataBuffer[bufferIndex].bUse);
	m_pDataBuffer[bufferIndex].bUse = false;
	//printf("%s,%d,release buffer id:%d\n",__FILE__,__LINE__,bufferIndex);
	m_SnapLock.UnLock();
}

void CSnapProc::RemoveAllData()
{
	m_SnapLock.Lock();
	{

	}
	m_SnapLock.UnLock();
	return;
}


void CSnapProc::SnapJPEGStream(CFrameData *pFrameData)
{
	m_SnapLock.Lock();

	if (!m_bAction)
	{
		m_SnapLock.UnLock();
		return;
	}

	FRAME_INFO_EX &frameInfo = pFrameData->GetFrameInfo();
	int channel = frameInfo.channel;

	if (m_pChnnSnaptoEmailIdList[channel].IsEmpty())
	{

	}
	else
	{
		bool bSnapJPEG = true;
		if (frameInfo.time <= m_pLastJPEGTime[channel])
		{
			bSnapJPEG = true;
		}
		else
		{
			LONGLONG d = (frameInfo.time - m_pLastJPEGTime[channel])/1000000;
			if (d >= m_pSnapPeriodChnn[channel])
			{
				bSnapJPEG = true;
				m_pLastJPEGTime[channel] = frameInfo.time;
			}
			else
			{
				bSnapJPEG = false;
			}
		}

		if (bSnapJPEG)
		{
			POS chnnSnapToEmailPosPre = NULL;
			POS chnnSnapToEmailPos = m_pChnnSnaptoEmailIdList[channel].GetHeadPosition();
			while (chnnSnapToEmailPos != NULL)
			{
				chnnSnapToEmailPosPre = chnnSnapToEmailPos;
				int emailId = m_pChnnSnaptoEmailIdList[channel].GetNext(chnnSnapToEmailPos);
				bool bFind = false;
				POS emailPos = NULL;
				POS emailPosTemp = m_EmailInfoList.GetHeadPosition();				
				while(emailPosTemp != NULL)
				{
					EMAIL_INFO &emailInfoTemp = m_EmailInfoList.GetAt(emailPosTemp);
					if (emailInfoTemp.id == emailId)
					{
						emailPos = emailPosTemp;
						bFind = true;
						break;
					}
					m_EmailInfoList.GetNext(emailPosTemp);
				}
				if(bFind)
				{
					EMAIL_INFO &emailInfo = m_EmailInfoList.GetAt(emailPos);
					assert(emailInfo.pJPEGInfoList != NULL);
					if ((NULL != emailInfo.pJPEGInfoList) && (emailInfo.pJPEGInfoList->GetCount() > 0))
					{
						bool bFindChJpeg = false;
						POS chnJpegPos = NULL;
						POS chJpegPosTemp = emailInfo.pJPEGInfoList->GetHeadPosition();
						while (chJpegPosTemp != NULL)
						{
							EMAIL_CH_JPEG_INFO &chJpegInfoTemp = emailInfo.pJPEGInfoList->GetAt(chJpegPosTemp);
							if (chJpegInfoTemp.channel == channel)
							{
								bFindChJpeg = true;
								chnJpegPos = chJpegPosTemp;
								break;
							}
							emailInfo.pJPEGInfoList->GetNext(chJpegPosTemp);
						}
						if (bFindChJpeg)
						{
							EMAIL_CH_JPEG_INFO &chJpegInfo = emailInfo.pJPEGInfoList->GetAt(chnJpegPos);

							//对于NVR，如果要抓图的通道断开连接了，则没办法继续抓图，从而会导致邮件一直发不出去
							//这里做个特殊处理，当设备断开时，送一个空帧过来，停止继续等待抓图，并把邮件发送出去
							if(NULL == frameInfo.pData)
							{
								chJpegInfo.snapCount = 0;
							}
							else if (chJpegInfo.curDataLength + frameInfo.length > (ALARM_EMAIL_MAX_SNAP_IMAGE_COUNT*JPEG_MAX_LENGTH))
							{
								assert(false);
								chJpegInfo.snapCount = 0;
							}
							else
							{
								EMAIL_JPEG_INFO JPEGInfo;
								JPEGInfo.pData = m_pDataBuffer[chJpegInfo.bufferIndex].pDataBuff + chJpegInfo.curDataLength;
								memcpy(JPEGInfo.pData, frameInfo.pData, frameInfo.length);
								JPEGInfo.length = frameInfo.length;
								JPEGInfo.time = frameInfo.time;				
								chJpegInfo.pJPEGInfo->AddTail(JPEGInfo);
								chJpegInfo.curDataLength += frameInfo.length;
								chJpegInfo.snapCount--;						
							}

							if (0 == chJpegInfo.snapCount)
							{
								//printf("%s,%d,chnn:%d,emaillist:%d\n",__FILE__,__LINE__,channel,m_pChnnSnaptoEmailIdList[channel].GetCount());
								m_pChnnSnaptoEmailIdList[channel].RemoveAt(chnnSnapToEmailPosPre);
								bool bSendEmail = true;
								POS posTemp = emailInfo.pJPEGInfoList->GetHeadPosition();
								while (posTemp != NULL)
								{
									EMAIL_CH_JPEG_INFO &Sendtemp = emailInfo.pJPEGInfoList->GetAt(posTemp);
									if (0 != Sendtemp.snapCount)
									{
										bSendEmail = false;
										break;
									}
									emailInfo.pJPEGInfoList->GetNext(posTemp);
								}
								if (bSendEmail)
								{
									CMessageMan::Instance()->PostMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_SEND_EMAIL, (unsigned char *)(&emailInfo), sizeof(EMAIL_INFO));
									m_EmailInfoList.RemoveAt(emailPos);
									//printf("%s,%d,emailnum:%d\n",__FILE__,__LINE__,m_EmailInfoList.GetCount());
									emailPos = NULL;
								}								
							}
						}
						else
						{
							assert(false);
							m_pChnnSnaptoEmailIdList[channel].RemoveAt(chnnSnapToEmailPosPre);
							bool bSendEmail = true;
							POS posTemp = emailInfo.pJPEGInfoList->GetHeadPosition();
							while (posTemp != NULL)
							{
								EMAIL_CH_JPEG_INFO &Sendtemp = emailInfo.pJPEGInfoList->GetNext(posTemp);
								if (0 != Sendtemp.snapCount)
								{
									bSendEmail = false;
									break;
								}
							}
							if (bSendEmail)
							{
								CMessageMan::Instance()->PostMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_SEND_EMAIL, (unsigned char *)(&emailInfo), sizeof(EMAIL_INFO));
								m_EmailInfoList.RemoveAt(emailPos);
								emailPos = NULL;
							}

						}
					}
					else
					{
						assert(false);
						m_pChnnSnaptoEmailIdList[channel].RemoveAt(chnnSnapToEmailPosPre);
						if (emailInfo.pJPEGInfoList != NULL)
						{
							emailInfo.pJPEGInfoList->RemoveAll();
							delete emailInfo.pJPEGInfoList;
							emailInfo.pJPEGInfoList = NULL;
						}
						if (emailInfo.pAlarmInfoList != NULL)
						{
							emailInfo.pAlarmInfoList->RemoveAll();
							delete emailInfo.pAlarmInfoList;
							emailInfo.pAlarmInfoList = NULL;
						}
						m_EmailInfoList.RemoveAt(emailPos);
					}

				}
				else
				{
					assert(false);
					m_pChnnSnaptoEmailIdList[channel].RemoveAt(chnnSnapToEmailPosPre);
				}
			}

			if (m_pChnnSnaptoEmailIdList[channel].IsEmpty())
			{
				//printf("%s,%d,channel %d end snap\n",__FILE__,__LINE__,channel);

				//CLocalDevice::Instance()->SetEmailSnapEnable(channel, false);
#if !defined(__SUPPORT_HONGDI__)
				CSnapCapabilityMan::Instance()->StopSnapJpeg(channel, false);
#endif


			}			

		}
	}

	m_SnapLock.UnLock();
	return;	
}

int CSnapProc::UseEmailBuffer()
{
	for (int i = 0; i < m_DataBufferNum; ++i)
	{
		if (!m_pDataBuffer[i].bUse)
		{
			m_pDataBuffer[i].bUse = true;
			return i;
		}		
	}
	return -1;
}


LONGLONG CSnapProc::GetSendEmailPeriod()
{
	m_SnapLock.Lock();

	LONGLONG ret = m_snapImgNum * m_snapPeriod;

	m_SnapLock.UnLock();
	return ret;
}

//CSnapProc/////////////////////////////////////////////////////////////////
#endif

//CSmtpMan/////////////////////////////////////////////////////////////////

CSmtpMan *CSmtpMan::m_pInstance = NULL;

CSmtpMan *CSmtpMan::Instance()
{
    if( NULL == m_pInstance )
    {
        m_pInstance = new CSmtpMan;
    }
    return m_pInstance;
}

CSmtpMan::CSmtpMan()
{

}

CSmtpMan::~CSmtpMan()
{

}

bool CSmtpMan::Initial()
{
	m_bAction = true;

	m_sendMailProc_ID = PUB_THREAD_ID_NOINIT;
	m_bSendMailProc = false;

	m_DeveiceID = 0;
	memset(m_DeviceName, 0, sizeof(m_DeviceName));
	m_pSendInfo = NULL;
	m_bCheckSendInfo = false;

	memset(m_subjectDsp1, 0, sizeof(m_subjectDsp1));
	memset(m_subjectDsp2, 0, sizeof(m_subjectDsp2));
	memset(m_deviceIDDsp, 0, sizeof(m_deviceIDDsp));
	memset(m_deviceNameDsp, 0, sizeof(m_deviceNameDsp));
	memset(m_channelIDDsp, 0, sizeof(m_channelIDDsp));
	memset(m_channelNameDsp, 0, sizeof(m_channelNameDsp));
	memset(m_alarmTypeDsp, 0, sizeof(m_alarmTypeDsp));
	memset(m_alarmTimeDsp, 0, sizeof(m_alarmTimeDsp));
	memset(m_alarmTypeMotion, 0, sizeof(m_alarmTypeMotion));
	memset(m_alarmTypeSensor, 0, sizeof(m_alarmTypeSensor));
	memset(m_alarmTypeMotion, 0, sizeof(m_alarmTypeMotion));
	memset(m_alarmTypeLoss, 0, sizeof(m_alarmTypeLoss));
	memset(m_alarmTypeTest, 0, sizeof(m_alarmTypeTest));
	memset(m_alarmTypeDiskFull, 0, sizeof(m_alarmTypeDiskFull));
	memset(m_alarmTypeIpConflict, 0, sizeof(m_alarmTypeIpConflict));
	memset(m_alarmTypeDisconnet, 0, sizeof(m_alarmTypeDisconnet));
	memset(m_alarmTypeDisconnet, 0, sizeof(m_alarmTypeDiskError));
	memset(m_alarmTypeOther, 0, sizeof(m_alarmTypeOther));
	memset(m_sensorAlarmTypeName, 0, sizeof(m_sensorAlarmTypeName));

	InitSSL();

	return true;
}

void CSmtpMan::Quit()
{
	if (PUB_THREAD_ID_NOINIT != m_sendMailProc_ID)
	{
		PUB_ExitThread(&m_sendMailProc_ID, &m_bSendMailProc);
	}

	//地址链表内存释放
	m_ToAddrList.RemoveAll( );	

	if( NULL != m_pSendInfo )
	{
		delete m_pSendInfo;
		m_pSendInfo = NULL;
	}
}

void CSmtpMan::ChangeLanguage()
{
	m_lock.Lock();

	snprintf(m_subjectDsp1, sizeof(m_subjectDsp1), "%s", CStringTable::Instance()->String(E_STR_ID_EMAIL_SUBJECT1));
	snprintf(m_subjectDsp2, sizeof(m_subjectDsp2), "%s", CStringTable::Instance()->String(E_STR_ID_EMAIL_SUBJECT2));

	snprintf(m_deviceIDDsp, sizeof(m_deviceIDDsp), "%s", CStringTable::Instance()->String(E_STR_ID_DEV_ID));
	snprintf(m_deviceNameDsp, sizeof(m_deviceNameDsp), "%s", CStringTable::Instance()->String(E_STR_ID_DEV_NAME));

	snprintf(m_channelIDDsp, sizeof(m_channelIDDsp), "%s", CStringTable::Instance()->String(E_STR_ID_EMAIL_CH_ID));
	snprintf(m_channelNameDsp, sizeof(m_channelNameDsp), "%s", CStringTable::Instance()->String(E_STR_ID_EMAIL_CH_NAME));

	snprintf(m_alarmTypeDsp, sizeof(m_alarmTypeDsp), "%s", CStringTable::Instance()->String(E_STR_ID_EMAIL_ALARM_TYPE));
	snprintf(m_alarmTimeDsp, sizeof(m_alarmTypeDsp), "%s", CStringTable::Instance()->String(E_STR_ID_EMAIL_ALARM_TIME));

	snprintf(m_alarmTypeMotion, sizeof(m_alarmTimeDsp), "%s", CStringTable::Instance()->String(E_STR_ID_EMAIL_MOTION_ALARM));
	snprintf(m_alarmTypeSensor, sizeof(m_alarmTypeMotion), "%s", CStringTable::Instance()->String(E_STR_ID_EMAIL_SENSOR_ALARM));
	snprintf(m_alarmTypeLoss, sizeof(m_alarmTypeLoss), "%s", CStringTable::Instance()->String(E_STR_ID_EMAIL_LOSS_ALARM));
	snprintf(m_alarmTypeTest, sizeof(m_alarmTypeTest), "%s", CStringTable::Instance()->String(E_STR_ID_EMAIL_TEST_ALARM));
	snprintf(m_alarmTypeDiskFull, sizeof(m_alarmTypeDiskFull), "%s", CStringTable::Instance()->String(E_STR_ID_EMAIL_DISKFULL_ALARM));
	snprintf(m_alarmTypeIpConflict, sizeof(m_alarmTypeIpConflict), "%s", CStringTable::Instance()->String(E_STR_ID_EMAIL_IPCONF_ALARM));
	snprintf(m_alarmTypeDisconnet, sizeof(m_alarmTypeDisconnet), "%s", CStringTable::Instance()->String(E_STR_ID_EMAIL_DISCONNET_ALARM));
	snprintf(m_alarmTypeDiskError, sizeof(m_alarmTypeDiskError), "%s", CStringTable::Instance()->String(E_STR_ID_LOG_DISK_WARNING));
	snprintf(m_alarmTypeDiskDisConnect, sizeof(m_alarmTypeDiskDisConnect), "%s", CStringTable::Instance()->String(E_STR_ID_LOG_DISK_DISCONNECT));

	snprintf(m_alarmTypeOther, sizeof(m_alarmTypeOther), "%s", CStringTable::Instance()->String(E_STR_ID_EMAIL_OTHER_ALARM));
	snprintf(m_sensorAlarmTypeName, sizeof(m_sensorAlarmTypeName), "%s", CStringTable::Instance()->String(E_STR_ID_SENSOR_NAME));

	m_lock.UnLock();
	return;
}



bool CSmtpMan::Start()
{
	m_sendMailProc_ID = PUB_CreateThread(SendMailThread, this, &m_bSendMailProc);

	if( PUB_CREATE_THREAD_FAIL == m_sendMailProc_ID )
	{
		return false;
	}

	return true;
}

void CSmtpMan::Stop()
{
	if (PUB_THREAD_ID_NOINIT != m_sendMailProc_ID)
	{
		PUB_ExitThread(&m_sendMailProc_ID, &m_bSendMailProc);
	}
}

void CSmtpMan::Action(bool bEnable)
{
    m_lock.Lock();

	if (!bEnable)
	{
		POS pos = m_EmailList.GetHeadPosition();
		while (pos != NULL)
		{
			EMAIL_INFO &node = m_EmailList.GetNext(pos);
			ReleaseEmailInfo(&node);
		}
		m_EmailList.RemoveAll();		
	}	
	m_bAction = bEnable;

    m_lock.UnLock();
    return ;
}


//设备ID
void CSmtpMan::SetDeviceID(unsigned long DeviceID)
{
    m_lock.Lock();
    m_DeveiceID = DeviceID;
    m_lock.UnLock();
    return;
}

//设备名称
void CSmtpMan::SetDeviceName(const char *pDeviceName)
{
    assert ( NULL != pDeviceName );

    m_lock.Lock();
    snprintf(m_DeviceName, STRINGLEN, "%s", pDeviceName);
    m_lock.UnLock();
    return;
}

//发件人信息
void CSmtpMan::SetSendPara(const MAIL_SEND_INFO *pSendInfo)
{
    m_lock.Lock();
	m_bCheckSendInfo = false;
    if ( NULL == m_pSendInfo )
    {
        m_pSendInfo = new MAIL_SEND_INFO;
    }
	memset(m_pSendInfo, 0, sizeof(MAIL_SEND_INFO));
	memcpy(m_pSendInfo, pSendInfo, sizeof(MAIL_SEND_INFO));
	m_bCheckSendInfo = CheckSendInfo();
    m_lock.UnLock();
}

void CSmtpMan::GetSendPara(MAIL_SEND_INFO *pSendInfo)
{
	m_lock.Lock();
	memset(pSendInfo, 0, sizeof(MAIL_SEND_INFO));
	if ( NULL != m_pSendInfo )
	{
		memcpy(pSendInfo, m_pSendInfo, sizeof(MAIL_SEND_INFO));
	}
	m_lock.UnLock();
}

//收件人信息
void CSmtpMan::SetReceiverPara(const MAIL_RECV_INFO *pReceviceInfo, int num)
{
    m_lock.Lock();
    m_ToAddrList.RemoveAll();    
    for(int i = 0; i < num; i++)
    {
		if (strlen(pReceviceInfo[i].recvname) > 0)
		{
			m_ToAddrList.AddTail(pReceviceInfo[i]);
		}
	}
    m_lock.UnLock();  
}

void CSmtpMan::GetReceiverPara(MAIL_RECV_INFO *pReceviceInfo, int *pRetNum)
{
	m_lock.Lock();
	*pRetNum = 0;
	int i = 0;
	POS pos = m_ToAddrList.GetHeadPosition();
	while (pos != NULL)
	{
		MAIL_RECV_INFO &node = m_ToAddrList.GetNext(pos);
		sprintf(pReceviceInfo[i++].recvname, "%s", node.recvname);
	}
	*pRetNum = i;
	m_lock.UnLock();
}

//测试发送邮件
int  CSmtpMan::TestSendEmailProcess(ALARM_TYPE alarmType, char *pszTime)
{ 
    assert( NULL != pszTime );

	m_lock.Lock();	
	
	int ret = -1;
    if (!m_bAction)
    {
        m_lock.UnLock();
        return ret;
    }
	if (!m_bCheckSendInfo)
	{
		m_lock.UnLock();
		return ret;
	}
	if (m_ToAddrList.GetCount() <= 0)
	{
		m_lock.UnLock();
		return ret;
	}
     
	//
	EMAIL_INFO emailTemp;
	memset(&emailTemp,  0, sizeof(EMAIL_INFO));	
	emailTemp.id = -1;
	emailTemp.chn = INVALID_INDEX(unsigned long);
	snprintf(emailTemp.szChannelName, STRINGLEN,  "%s", "channel");
	emailTemp.pAlarmInfoList = new CMyList<EMAIL_ALARM_INFO>;
	EMAIL_ALARM_INFO alarmInfo;	
	alarmInfo.alarmType = alarmType;
	snprintf(alarmInfo.szTime, STRINGLEN, "%s", pszTime);
	emailTemp.pAlarmInfoList->AddTail(alarmInfo);
	emailTemp.pJPEGInfoList = NULL;
	CMyList <EMAIL_INFO> EmailInfoList;
	EmailInfoList.AddTail(emailTemp);	

	//
	unsigned char *pOut = new unsigned char [1024];
	memset(pOut, 0, 1024);
	int iOutLen = SmtpEmailContent(&EmailInfoList, 1, (char *)pOut);
	delete emailTemp.pAlarmInfoList;
	EmailInfoList.RemoveAll();

	//
	MAIL_SEND_INFO *pSendInfo = new MAIL_SEND_INFO;
	CMyList<MAIL_RECV_INFO> *pToAddrList = new CMyList<MAIL_RECV_INFO>;
	SEND_USE_INFO sendInfo;
	memcpy(pSendInfo, m_pSendInfo, sizeof(MAIL_SEND_INFO));
	pToAddrList->RemoveAll();
	POS pos = m_ToAddrList.GetHeadPosition();
	while (pos != NULL)
	{
		MAIL_RECV_INFO &node = m_ToAddrList.GetNext(pos);
		pToAddrList->AddTail(node);
	}	
	sendInfo.pOutBuff = pOut;
	sendInfo.len  = iOutLen;
	sendInfo.pBio = NULL;
	sendInfo.pCtx = NULL;
	sendInfo.pSsl = NULL;
	sendInfo.sock = SWL_INVALID_SOCKET;
	sendInfo.Authmethod = SASL_LOGIN;
	sendInfo.pSendInfo = pSendInfo;
	sendInfo.pToAddrList = pToAddrList;
	if (SendEmailProcessImp(sendInfo))
	{
		ret = 0;
	}
	if (pOut != NULL)
	{
		delete [] pOut;
		pOut = NULL;
	}
	if (pSendInfo != NULL)
	{
		delete pSendInfo;
		pSendInfo = NULL;
	}
	if (pToAddrList != NULL)
	{
		delete pToAddrList;
		pToAddrList = NULL;
	}	

	m_lock.UnLock();
	PUB_Sleep(1000);  
    return ret;
}

void CSmtpMan::SendEmailProcess(EMAIL_INFO EmailInfo)
{
	//把邮件缓存起来，有独立的线程发送
	m_lock.Lock();

	if (!m_bAction)
	{
		ReleaseEmailInfo(&EmailInfo);
		m_lock.UnLock();
		return;
	}

	m_EmailList.AddTail(EmailInfo);

	m_lock.UnLock();
	return;  
}


RESULT WINAPI CSmtpMan::SendMailThread(void*  lpParameter)
{
#ifdef __ENVIRONMENT_LINUX__
	pid_t pid = getpid();
	pid_t tid = syscall(__NR_gettid);

	printf("%s,%d,SendMailThread pid=%d, tid=%d\n",__FILE__,__LINE__,pid, tid);
#endif
	assert(NULL != lpParameter);

	CSmtpMan *pThis = reinterpret_cast<CSmtpMan *>(lpParameter);
	pThis->SendMailThreadProcess();

	return 0;
}

//发送邮件线程
void  CSmtpMan::SendMailThreadProcess( )
{
	int channelNum = CProduct::Instance()->VideoInputNum();
	int mergeCharCount = 16;
	int mergePicCount = 4;
	int alarmMaxSize = channelNum * ALARM_EMAIL_PICTURE_CONTENT_LENGTH+ALARM_EMAIL_TEXT_CONTENT_LENGTH;
	unsigned char *pOutBuff = new unsigned char[alarmMaxSize];
	memset(pOutBuff, 0, alarmMaxSize);

	MAIL_SEND_INFO *pSendInfo = new MAIL_SEND_INFO;
	CMyList<MAIL_RECV_INFO> *pToAddrList = new CMyList<MAIL_RECV_INFO>;
	if (channelNum <= 8)
	{
		mergePicCount = 2;
	}
	else
	{
		mergePicCount = 4;
	}


	while (m_bSendMailProc)
	{
		m_lock.Lock();

		if( m_EmailList.GetCount( ) <= 0 )
		{
			m_lock.UnLock();
			PUB_Sleep(2000);  
			continue;
		}

		if (!m_bAction)
		{
			{
				POS posTmp = m_EmailList.GetHeadPosition();
				while (posTmp != NULL)
				{
					EMAIL_INFO &node = m_EmailList.GetAt(posTmp);
					ReleaseEmailInfo(&node);
					m_EmailList.RemoveAt(posTmp);
					posTmp = m_EmailList.GetHeadPosition();					
				}
			}
			m_lock.UnLock();
			PUB_Sleep(2000);  
			continue;
		}

		if (!m_bCheckSendInfo)
		{
			{
				POS posTmp = m_EmailList.GetHeadPosition();
				while (posTmp != NULL)
				{
					EMAIL_INFO &node = m_EmailList.GetAt(posTmp);
					ReleaseEmailInfo(&node);
					m_EmailList.RemoveAt(posTmp);
					posTmp = m_EmailList.GetHeadPosition();					
				}
			}
			m_lock.UnLock();
			PUB_Sleep(2000);  
			continue;
		}

		if (m_ToAddrList.GetCount() <= 0)
		{
			{
				POS posTmp = m_EmailList.GetHeadPosition();
				while (posTmp != NULL)
				{
					EMAIL_INFO &node = m_EmailList.GetAt(posTmp);
					ReleaseEmailInfo(&node);
					m_EmailList.RemoveAt(posTmp);
					posTmp = m_EmailList.GetHeadPosition();					
				}
			}
			m_lock.UnLock();
			PUB_Sleep(2000);  
			continue;
		}

		
		//如果是报警中只是文字，没有图片，就把这些报警组成一个邮件发送出去。
		//如果是报警中有图片，就把这个报警单独组成一个邮件发送出去。
		bool bMergePic = false;
		int MergeCount = 16;
		int sendNum = 0;
		EMAIL_INFO &firstNode = m_EmailList.GetHead();
		if (firstNode.pJPEGInfoList != NULL)
		{
			bMergePic = true;
			MergeCount = mergePicCount;
		}
		else
		{
			bMergePic = false;
			MergeCount = mergeCharCount;
		}
		float length = ALARM_EMAIL_TEXT_CONTENT_LENGTH;
		POS pos = m_EmailList.GetHeadPosition();
		while ((pos != NULL) && (sendNum < MergeCount))
		{
			EMAIL_INFO &node = m_EmailList.GetNext(pos);
			if (bMergePic)
			{
				if (NULL == node.pJPEGInfoList)
				{
					break;
				}
				else
				{
					POS chJpegPos = node.pJPEGInfoList->GetHeadPosition();
					while (chJpegPos != NULL)
					{
						EMAIL_CH_JPEG_INFO &chJpegInfo = node.pJPEGInfoList->GetNext(chJpegPos);
						length += (chJpegInfo.curDataLength)*1.4;
					}
					if (length >= (float)alarmMaxSize)
					{
						break;
					}
				}
			}
			else
			{
				if (node.pJPEGInfoList != NULL)
				{
					break;
				}
			}			
			sendNum++;
		}

		if (sendNum <= 0)
		{
			{
				POS posTmp = m_EmailList.GetHeadPosition();
				while (posTmp != NULL)
				{
					EMAIL_INFO &node = m_EmailList.GetAt(posTmp);
					ReleaseEmailInfo(&node);
					m_EmailList.RemoveAt(posTmp);
					posTmp = m_EmailList.GetHeadPosition();					
				}
			}
			m_lock.UnLock();
			PUB_Sleep(2000);  
			continue;
		}

		memset(pOutBuff, 0, alarmMaxSize);
		int iOutLen = SmtpEmailContent(&m_EmailList, sendNum, (char *)pOutBuff);

		//		
		int count = 0;
		pos = m_EmailList.GetHeadPosition();
		while ((pos != NULL) && (count < sendNum))
		{
			EMAIL_INFO &node = m_EmailList.GetAt(pos);
			ReleaseEmailInfo(&node);
			m_EmailList.RemoveAt(pos);
			pos = m_EmailList.GetHeadPosition();
			count++;
		}
		assert(count == sendNum);

		//
		SEND_USE_INFO sendInfo;
		{
			memcpy(pSendInfo, m_pSendInfo, sizeof(MAIL_SEND_INFO));
			pToAddrList->RemoveAll();
			POS pos = m_ToAddrList.GetHeadPosition();
			while (pos != NULL)
			{
				MAIL_RECV_INFO &node = m_ToAddrList.GetNext(pos);
				pToAddrList->AddTail(node);
			}
			
			sendInfo.pOutBuff = pOutBuff;
			sendInfo.len  = iOutLen;
			sendInfo.pBio = NULL;
			sendInfo.pCtx = NULL;
			sendInfo.pSsl = NULL;
			sendInfo.sock = SWL_INVALID_SOCKET;
			sendInfo.Authmethod = SASL_LOGIN;
			sendInfo.pSendInfo = pSendInfo;
			sendInfo.pToAddrList = pToAddrList;
		}
		
		m_lock.UnLock();		

		//发信
		SendEmailProcessImp(sendInfo);
		
		PUB_Sleep(2000); 	
	}

	//公共内存的释放
	if(pOutBuff != NULL)
	{
		delete [] pOutBuff;
		pOutBuff = NULL;
	}

	if (pSendInfo != NULL)
	{
		delete pSendInfo;
		pSendInfo = NULL;
	}

	if (pToAddrList != NULL)
	{
		pToAddrList->RemoveAll();
		delete pToAddrList;
		pToAddrList = NULL;
	}

}


//发送主调函数
bool CSmtpMan::SendEmailProcessImp(SEND_USE_INFO &sendInfo)
{
	//连接SMTP 邮件服务器
	if(!ConnectMailServer(sendInfo))
	{
		goto SendMail_err;
	}
	//smtp 协议交互过程
	if(!SmtpReady(sendInfo ) )
	{
		goto SendMail_err;
	}
	if(!SmtpEhlo(sendInfo) )
	{
		goto SendMail_err;
	}
	if(!SmtpAuth(sendInfo))
	{
		goto SendMail_err;
	}
	if(!SmtpFrom(sendInfo ))
	{
		goto SendMail_err;
	}
	if(!SmtpRcpt( sendInfo))
	{
		goto SendMail_err;
	}
	if(!SmtpData(sendInfo ))
	{
		goto SendMail_err;
	}
	if(!SmtpContent(sendInfo))
	{
		goto SendMail_err;
	}
	if(!SmtpQuit(sendInfo ))
	{
		goto SendMail_err;
	}
	SmtpRelease( sendInfo); 

	printf("%s,%d,send email ok\n",__FILE__,__LINE__);
	return true;
SendMail_err:

	printf("%s,%d,send email error\n",__FILE__,__LINE__);
	SmtpRelease(sendInfo);
	return false;

}

//连接邮箱服务器
bool CSmtpMan::ConnectMailServer( SEND_USE_INFO &sendInfo)
{   
    struct sockaddr_in addr;

    char szAddr[STRINGLEN] = {0};

    struct in_addr in;
    unsigned long time = 0 ;//超时控制

	bool bHaveIp = false;
	{
		struct hostent *pServp = NULL;
		struct in_addr  addrTemp;
#ifdef __ENVIRONMENT_LINUX__
		if(INADDR_ANY == inet_aton((sendInfo.pSendInfo)->server, &addrTemp))
		{
			CNetInterface* pNetIf = CNetInterface::Instance();
			pServp= pNetIf->GetHostByName((sendInfo.pSendInfo)->server);
			if( NULL == pServp )
			{
				bHaveIp = false;
			}
			else
			{
				if ( NULL == pServp->h_addr )
				{
					bHaveIp = false;
				}
				else
				{
					bHaveIp = true;
					in.s_addr = *((unsigned long*)pServp->h_addr);
				}
				pNetIf->ReleaseHost(pServp);
			}
		}
		else
		{
			bHaveIp = true;
			in = addrTemp;
		}
#else
		WSADATA wsaData;
		if(SOCKET_ERROR == WSAStartup(0x202, &wsaData)) 
		{
			printf("errno=%d %s[%d]\n", WSAGetLastError(), __FILE__, __LINE__);
			return false;
		}
		addrTemp.S_un.S_addr = inet_addr((sendInfo.pSendInfo)->server);
		pServp = gethostbyname((sendInfo.pSendInfo)->server);
		if( NULL == pServp )
		{
			bHaveIp = false;
		}
		else
		{
			if (NULL == pServp->h_addr)
			{
				bHaveIp = false;
			}
			else
			{
				bHaveIp = true;
				in.s_addr = *((unsigned long*)pServp->h_addr);
			}
		}
		WSACleanup( );
#endif
	}
	if (!bHaveIp)
	{
		return false;
	}
	

    if ((sendInfo.pSendInfo)->bSSL)
    {        
        strncpy (szAddr ,inet_ntoa(in),STRINGLEN );
		sprintf(szAddr, "%s:%d", szAddr, (sendInfo.pSendInfo)->port);

        sendInfo.pCtx = SSL_CTX_new(SSLv23_client_method());
        sendInfo.pBio = BIO_new_ssl_connect(sendInfo.pCtx);
        BIO_get_ssl(sendInfo.pBio, &sendInfo.pSsl);

        // try connecting
        BIO_set_conn_hostname(sendInfo.pBio, szAddr);//"74.125.127.109:465");//""smtp.gmail.com:25"/**/
        BIO_set_nbio(sendInfo.pBio, 1);

        time = GetTickCount( );
		while(BIO_do_connect(sendInfo.pBio) <= 0)
        {
            if(!BIO_should_retry(sendInfo.pBio) || IsTimeOut(time, SMTP_TIMEOUT))
            {
                SSL_CTX_free(sendInfo.pCtx);
                sendInfo.pCtx = NULL;
                BIO_free_all(sendInfo.pBio);
                sendInfo.pBio = NULL;
                return false;
            }
			PUB_Sleep(2000);
        }		
        return true;
    }
    else
    {
        if( SWL_SOCKET_ERROR == (sendInfo.sock = SWL_CreateSocket(AF_INET, SOCK_STREAM, 0)))
        {         
            return false;
        }

        addr.sin_family = AF_INET;
	   addr.sin_port = htons((sendInfo.pSendInfo)->port);
	   addr.sin_addr = in;

        memset(&(addr.sin_zero), '\0', 8);
        
        // try connecting
        if ( SWL_SOCKET_ERROR == SWL_Connect(sendInfo.sock, (struct sockaddr*)&addr, sizeof(struct sockaddr), SMTP_TIMEOUT) )
        {
            SWL_CloseSocket(sendInfo.sock);
            return false;
        }
        return true;

    }
}

bool CSmtpMan::SmtpReady( SEND_USE_INFO &sendInfo)
{
    char buf[MAXDATASIZE] = {0};

    if(SmtpRead(buf, sizeof(buf), sendInfo) < 0 )
    {
        return false;
    }

    if( 0 == strncmp(SERVICE_READY, buf, strlen(SERVICE_READY)))
    {
        return true;
    }
    else
    {
        return false;
    }
}

//socket编程，SMTP协议
bool CSmtpMan::SmtpEhlo(SEND_USE_INFO &sendInfo)
{
    char buf[MAXDATASIZE] = {0};

	if (0 == strstr((sendInfo.pSendInfo)->name, "@"))
	{
		return false;
	}

    strncpy(buf, "EHLO ",MAXDATASIZE);
    strcat(buf, strstr((sendInfo.pSendInfo)->name, "@") + 1);
    strcat(buf, "\r\n");

    if(SmtpSend(buf, strlen(buf), sendInfo) < 0 )
    {
        return false;
    }

    if(SmtpRead((char *)buf, sizeof(buf), sendInfo) < 0 )
    {
        return false;
    }
    if(0 == strncmp(REPLY_OKAY, buf, strlen(REPLY_OKAY)))
    {
        if( NULL != strstr(buf, "LOGIN") )
        {
           sendInfo.Authmethod = SASL_LOGIN;
        }
        else if( NULL != strstr(buf, "PLAIN") )
        {
            sendInfo.Authmethod = SASL_PLAIN;
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }

    return true;
}

//smtp 认证登陆
bool CSmtpMan::SmtpAuth(SEND_USE_INFO &sendInfo)
{
	if (0 == strlen((sendInfo.pSendInfo)->passwd))
	{
		//不需要认证
		return true;
	}

    char local_in_buf[MAXDATASIZE] = {0};
    char local_out_buf[MAXDATASIZE] = {0};
    char local_tmp_buf[MAXDATASIZE] = {0};

    if(SASL_LOGIN == sendInfo.Authmethod)
    {
        strcpy(local_out_buf, "AUTH LOGIN");
        strcat(local_out_buf, "\r\n");

        if( SmtpSend(local_out_buf, strlen(local_out_buf), sendInfo) < 0 )
        {
            return false;
        }
    
        if(SmtpRead(local_in_buf, sizeof(local_in_buf), sendInfo) < 0 )
        {
            return false;
        }
    
        if( strncmp(local_in_buf, "3", 1) != 0 )
        {
            return false;
        }
    
        memset(local_out_buf, 0, sizeof(local_out_buf));
        to64frombits((unsigned char *)local_out_buf, (const unsigned char*)((sendInfo.pSendInfo)->name), strlen((sendInfo.pSendInfo)->name));
        strcat(local_out_buf, "\r\n");
    
        if( SmtpSend(local_out_buf, strlen(local_out_buf), sendInfo) < 0 )
        {
            return false;
        }
    
        if(SmtpRead(local_in_buf, sizeof(local_in_buf), sendInfo) < 0 )
        {
            return false;
        }
    
        if( strncmp(local_in_buf, "3", 1) != 0 )
        {
            return false;
        }
    
        memset(local_out_buf, 0, sizeof(local_out_buf));
        to64frombits((unsigned char *)local_out_buf, (const unsigned char*)((sendInfo.pSendInfo)->passwd), strlen((sendInfo.pSendInfo)->passwd));
        strcat(local_out_buf, "\r\n");
    
        if( SmtpSend(local_out_buf, strlen(local_out_buf), sendInfo) < 0 )
        {
            return false;
        }
    
        if(SmtpRead(local_in_buf, sizeof(local_in_buf), sendInfo) < 0 )
        {
            return false;
        }
    
        if( strncmp(local_in_buf, "2", 1) != 0 )
        {
            return false;
        }
    }
    else if ( SASL_PLAIN == sendInfo.Authmethod)
    {
        strcpy(local_tmp_buf, "^");
        strncat(local_tmp_buf, (sendInfo.pSendInfo)->name,strlen((sendInfo.pSendInfo)->name));
        strcat(local_tmp_buf, "^");
        strncat(local_tmp_buf, (sendInfo.pSendInfo)->passwd,strlen((sendInfo.pSendInfo)->passwd));

        int len = strlen(local_out_buf);

        for ( int i = len-1 ; i >= 0 ; i-- )
        {
            if (local_tmp_buf[i]=='^')
            {
                local_tmp_buf[i]='\0';
            }
        }

        strcpy(local_out_buf, "AUTH PLAIN ");
        to64frombits(reinterpret_cast<unsigned char *>(local_out_buf + strlen(local_out_buf)),
                     reinterpret_cast<unsigned char *>(local_tmp_buf), len);
        strcat(local_out_buf, "\r\n");

        if(SmtpSend(local_out_buf, strlen(local_out_buf), sendInfo) < 0 )
        {
            return false;
        }

        if(SmtpRead(local_in_buf, sizeof(local_in_buf), sendInfo) < 0 )
        {
            return false;
        }

        if( strncmp(local_in_buf, "2", 1) != 0 )
        {
            return false;
        }
    }
    else
    {
        return false;
    }

    return true;
}

//from 地址
bool CSmtpMan::SmtpFrom( SEND_USE_INFO &sendInfo)
{
    char buf[MAXDATASIZE] = {0};

    strcpy(buf, "MAIL FROM:<");
    strcat(buf, (sendInfo.pSendInfo)->name);
    strcat(buf, ">\r\n");
    if( SmtpSend(buf, strlen(buf), sendInfo) < 0 )
    {
        return false;
    }

    if( SmtpRead(buf, sizeof(buf), sendInfo) < 0 )
    {
        return false;
    }

    if(0 == strncmp(REPLY_OKAY, buf, strlen(REPLY_OKAY)))
    {
        return true;
    }
    else
    {
        return false;
    }
}

//to  地址
bool CSmtpMan::SmtpRcpt( SEND_USE_INFO &sendInfo)
{
	char buf[MAXDATASIZE] = {0};
	POS pos = (sendInfo.pToAddrList)->GetHeadPosition();
	while (pos != NULL)
	{
		MAIL_RECV_INFO &node = (sendInfo.pToAddrList)->GetNext(pos);
		snprintf(buf, MAXDATASIZE, "RCPT TO:<%s>\r\n", node.recvname);

		if(SmtpSend(buf, strlen(buf), sendInfo) < 0 )
		{
			return false;
		}

		if(SmtpRead(buf, sizeof(buf), sendInfo) < 0 )
		{		
			return false;
		}		

		if((0 == strncmp(REPLY_OKAY, buf, strlen(REPLY_OKAY))))
		{
			//	return 0;
		}
		else
		{
			//对收件人的合法性不做要求,发送不成功但是不要影响其他的收件人			
		}
	}
  
    return true;
}

//smtp: "DATA" 添加进SMTP协议
bool CSmtpMan::SmtpData(SEND_USE_INFO &sendInfo )
{
    char buf[MAXDATASIZE] = {0};

    strcpy(buf, "DATA\r\n");
    if(SmtpSend(buf, strlen(buf), sendInfo) < 0 )
    {
        return false;
    }

    if(SmtpRead(buf, sizeof(buf), sendInfo) < 0 )
    {
        return false;
    }

    if(0 != strncmp("3", buf, strlen("3")))
    {
        return false;
    }

    return true;
}

bool CSmtpMan::SmtpContent(SEND_USE_INFO &sendInfo)
{
	char buf[MAXDATASIZE] = {0};
    
    if(SmtpSend((char *)sendInfo.pOutBuff, sendInfo.len, sendInfo) < 0 )
    {
        return false;
    }
    
    strcpy((char *)buf, "\r\n.\r\n");
    if(SmtpSend((char *)buf, strlen((char *)buf), sendInfo) < 0 )
    {
        return false;
    }
    if(SmtpRead((char *)buf, sizeof(buf), sendInfo) < 0 )
    {
        return false;
    }

    if(0 != strncmp(REPLY_OKAY, (char *)buf, strlen(REPLY_OKAY)))
    {
        return false;
    }

    return true;

}

//smtp:添加QUIT到SMTP协议
bool CSmtpMan::SmtpQuit(SEND_USE_INFO &sendInfo)
{
    char buf[MAXDATASIZE] = {0};

    strcpy(buf, "QUIT\r\n");
    if(SmtpSend(buf, strlen(buf), sendInfo) < 0 )
    {
        return false;
    }

    if(SmtpRead( buf, sizeof(buf), sendInfo) < 0 )
    {
        return false;
    }

    return true;
}

//资源释放
void CSmtpMan::SmtpRelease(SEND_USE_INFO &sendInfo)
{
    if (0 == (sendInfo.pSendInfo)->bSSL)
    {
        if(SWL_INVALID_SOCKET != sendInfo.sock)
        {
            SWL_Shutdown(sendInfo.sock, SWL_SHUT_BOTH);
            SWL_CloseSocket(sendInfo.sock);
            sendInfo.sock = SWL_INVALID_SOCKET;
        }
    }
    else
    {
        if (NULL != sendInfo.pCtx)
        {
            SSL_CTX_free(sendInfo.pCtx);
            sendInfo.pCtx = NULL;
        }

        if(NULL != sendInfo.pBio)
        {
            BIO_reset(sendInfo.pBio);
            BIO_free_all(sendInfo.pBio);
            sendInfo.pBio = NULL;
        }
    }
    return;
}


//socket 编程，从网络读长度为iLen的数据到pbuf
int CSmtpMan::SmtpRead(char *pBuf, int iLen, SEND_USE_INFO &sendInfo)
{
	int len = 0;
	int iRecved = 0;
	unsigned long time = 0;
	unsigned long dwStart = GetTickCount();
	unsigned long dwDistance = SMTP_TIMEOUT * 2;//要SSL认证的会慢一点
	struct timeval timeout_tv;
	fd_set sockfds;

	if ((sendInfo.pSendInfo)->bSSL)
	{
		do 
		{
			len = BIO_read(sendInfo.pBio, pBuf + iRecved, iLen - iRecved - 1);
			if(0 == len)
			{
				return -1;
			}
			else if( len < 0 )
			{				
				if(!BIO_should_retry(sendInfo.pBio) || IsTimeOut(dwStart, dwDistance))
				{
					return -1;
				}
				PUB_Sleep(2000);
			}
			else
			{
				iRecved += len;
				pBuf[iRecved]='\0';
				if (CheckReplyEnd(pBuf, iRecved))
				{					
					return iRecved;	
				}
			}

		} while(!IsTimeOut(dwStart, dwDistance));		
	}
	else
	{
		dwDistance = SMTP_TIMEOUT;
		do 
		{
			len = recv(sendInfo.sock, pBuf + iRecved, iLen - iRecved - 1, 0);
			if (len == 0)
			{
				return -1;
			}
			else if (len < 0)
			{
				if (SWL_EWOULDBLOCK())
				{
					PUB_Sleep(0);
				}
				else
				{
					return -1;
				}
			}
			else
			{
				iRecved += len;
				pBuf[iRecved] = '\0';
				if (CheckReplyEnd(pBuf, iRecved))
				{
					return iRecved;
				}
			}
		} while(!IsTimeOut(dwStart, dwDistance));	
		
	}
	return -1;
}

//socket 编程，把长度为iLen的pBuf发送到网络
int CSmtpMan::SmtpSend(const char *pBuf, int iLen, SEND_USE_INFO &sendInfo)
{
	int total = 0;          // how many bytes we sent
	int bytesleft = iLen;   // how many we have left to send
	int len = 0;

	if ((sendInfo.pSendInfo)->bSSL)
	{
		while(total < iLen)
		{
			len = BIO_write(sendInfo.pBio, pBuf + total, bytesleft);
			if (len < 0)
			{
				if(!BIO_should_retry(sendInfo.pBio))
				{
					return -1;
				}
				else
				{
					PUB_Sleep(0);
					continue;
				}
			}

			total += len;
			bytesleft -= len;
		}
	}
	else
	{
		while(total < iLen)
		{
			len = send(sendInfo.sock, pBuf + total, bytesleft, 0);
			if (len < 0)
			{
#ifdef __ENVIRONMENT_LINUX__
				if(EAGAIN == errno || EWOULDBLOCK == errno)
				{
					PUB_Sleep(0);
					continue;
				}
				else
				{
					return -1;
				}
#endif
			}

			total += len;
			bytesleft -= len;
		}
	}
	return 0;
}



//邮件内容
int CSmtpMan::SmtpEmailContent(CMyList<EMAIL_INFO> *pEmailInfoList, int Num, char *pOutBuff)
{
	//邮件正文填写进pOutBuff
	char *pStart = pOutBuff;
	int iFileLen = 0;	

	sprintf( pOutBuff, "Message-ID: <%s>\r\n", "DVR");
	pOutBuff += strlen(pOutBuff);	
	
	sprintf( pOutBuff, "Reply-To: <%s>\r\n", m_pSendInfo->name);
	pOutBuff += strlen(pOutBuff);

	sprintf( pOutBuff, "From: <%s>\r\n", m_pSendInfo->name);
	pOutBuff += strlen(pOutBuff);

	bool bBegin = true;
	POS pos = m_ToAddrList.GetHeadPosition();
	while (pos != NULL)
	{
		MAIL_RECV_INFO &node = m_ToAddrList.GetNext(pos);
		if (bBegin)
		{
			sprintf(pOutBuff, "To: <%s>", node.recvname);
			bBegin = false;
		}
		else
		{
			sprintf(pOutBuff,",\r\n\t<%s>", node.recvname);
		}
		pOutBuff += strlen(pOutBuff);
	}
	sprintf(pOutBuff, "\r\n");
	pOutBuff += strlen(pOutBuff);

	if (strlen(m_DeviceName) <= 0)
	{
		sprintf(m_DeviceName, "%s", "DVR");
	}
	EMAIL_ALARM_INFO &tempInfo = ((pEmailInfoList->GetHead()).pAlarmInfoList)->GetHead();
	if (ALARM_TYPE_TEST != tempInfo.alarmType)
	{
		char szSubject[128] = {0};
		snprintf(szSubject, 128, m_subjectDsp1, m_DeviceName);
		sprintf( pOutBuff, "Subject: %s\r\n", szSubject);		
	}
	else
	{
		char szSubject[128] = {0};
		snprintf(szSubject, 128, m_subjectDsp2, m_DeviceName);
		sprintf( pOutBuff, "Subject: %s\r\n", szSubject);			
	}
	pOutBuff += strlen(pOutBuff);
	
	sprintf( pOutBuff, "Date: %s\r\n ", tempInfo.szTime);
	pOutBuff += strlen(pOutBuff);

	sprintf( pOutBuff, "MIME-Version: 1.0\r\n");
	pOutBuff += strlen(pOutBuff);

	sprintf( pOutBuff, "Content-Type:multipart/mixed;\r\n\tboundary=\"%s\"\r\n",MAIL_BOUNDARY2);
	pOutBuff += strlen(pOutBuff);

	sprintf( pOutBuff, "This is a multi-part message in MIME format.\r\n\r\n");
	pOutBuff += strlen(pOutBuff);

	//文本
	sprintf( pOutBuff, "--%s\r\n", MAIL_BOUNDARY2);
	pOutBuff += strlen(pOutBuff);

	sprintf( pOutBuff, "Content-Type: text/plain;\r\n\tcharset=\"utf-8\"\r\n");
	pOutBuff += strlen(pOutBuff);

	sprintf( pOutBuff, "Content-Transfer-Encoding: quoted-printable\r\n\r\n");
	pOutBuff += strlen(pOutBuff);
	
	sprintf( pOutBuff, "%s: %d, %s: %s\r\n", m_deviceIDDsp, m_DeveiceID, m_deviceNameDsp, m_DeviceName);//设备ID
	pOutBuff += strlen(pOutBuff);

	sprintf( pOutBuff, "\r\n\r\n");
	pOutBuff += strlen(pOutBuff);
	
	int EmailInfoCount = 0;
	POS EmailInfoPos = pEmailInfoList->GetHeadPosition();
	while ((EmailInfoPos != NULL) && (EmailInfoCount < Num))
	{
		EMAIL_INFO &EmailInfoNode = pEmailInfoList->GetNext(EmailInfoPos);

#if defined(__CUSTOM_IL03__)
		 if (INVALID_INDEX(unsigned long) != EmailInfoNode.chn)
		{
			POS pos = (EmailInfoNode.pAlarmInfoList)->GetHeadPosition();
			while (NULL != pos)
			{
				char szAlarmName[32] = {0};
				EMAIL_ALARM_INFO &info = (EmailInfoNode.pAlarmInfoList)->GetNext(pos);
				bool bGetSensorName = false;
				std::string strSensorName;
				if (ALARM_TYPE_SENSOR == info.alarmType)
				{
					if (CMessageMan::Instance()->GetSensorName(EmailInfoNode.chn, strSensorName))
					{
						bGetSensorName = true;
					}
				}
				
				if (bGetSensorName)
				{			
					sprintf( pOutBuff, "%s: %d, %s:%s, %s:%s\r\n", m_channelIDDsp, EmailInfoNode.chn + 1, m_sensorAlarmTypeName, strSensorName.c_str(), m_channelNameDsp, EmailInfoNode.szChannelName);//通道号
					pOutBuff += strlen(pOutBuff);
				}
				else
				{
					sprintf( pOutBuff, "%s: %d, %s:%s\r\n", m_channelIDDsp, EmailInfoNode.chn + 1, m_channelNameDsp, EmailInfoNode.szChannelName);//通道号
					pOutBuff += strlen(pOutBuff);
				}
				GetAlarmTypeName(info.alarmType, szAlarmName, 32);
				sprintf( pOutBuff, "%s:%s, %s:%s\r\n", m_alarmTypeDsp, szAlarmName, m_alarmTimeDsp,info.szTime); 
				pOutBuff += strlen(pOutBuff);
			}
		}		
		else
		{
			POS pos = (EmailInfoNode.pAlarmInfoList)->GetHeadPosition();
			while (NULL != pos)
			{
				char szAlarmName[32] = {0};
				EMAIL_ALARM_INFO &info = (EmailInfoNode.pAlarmInfoList)->GetNext(pos);
				GetAlarmTypeName(info.alarmType, szAlarmName, 32);
				sprintf( pOutBuff, "%s:%s, %s:%s\r\n", m_alarmTypeDsp, szAlarmName, m_alarmTimeDsp,info.szTime); 
				pOutBuff += strlen(pOutBuff);
			}
		}
#else
		if (INVALID_INDEX(unsigned long) != EmailInfoNode.chn)
		{
			sprintf( pOutBuff, "%s: %d, %s:%s\r\n", m_channelIDDsp, EmailInfoNode.chn + 1, m_channelNameDsp, EmailInfoNode.szChannelName);//通道号
			pOutBuff += strlen(pOutBuff);
		}		
		
		{
			POS pos = (EmailInfoNode.pAlarmInfoList)->GetHeadPosition();
			while (NULL != pos)
			{
				char szAlarmName[32] = {0};
				EMAIL_ALARM_INFO &info = (EmailInfoNode.pAlarmInfoList)->GetNext(pos);
				GetAlarmTypeName(info.alarmType, szAlarmName, 32);
				sprintf( pOutBuff, "%s:%s, %s:%s\r\n", m_alarmTypeDsp, szAlarmName, m_alarmTimeDsp,info.szTime); 
				pOutBuff += strlen(pOutBuff);
			}
		}
#endif

		sprintf( pOutBuff, "\r\n\r\n");
		pOutBuff += strlen(pOutBuff);
		EmailInfoCount++;
	}



	EmailInfoCount = 0;
	EmailInfoPos = pEmailInfoList->GetHeadPosition();
	while ((EmailInfoPos != NULL) && (EmailInfoCount < Num))
	{
		EMAIL_INFO &EmailInfoNode = pEmailInfoList->GetNext(EmailInfoPos);		
		if (EmailInfoNode.pJPEGInfoList != NULL)
		{
			assert(EmailInfoNode.id > -1);
			POS chJpegPos = EmailInfoNode.pJPEGInfoList->GetHeadPosition();
			while (chJpegPos != NULL)
			{
				EMAIL_CH_JPEG_INFO &chJpegInfo = EmailInfoNode.pJPEGInfoList->GetNext(chJpegPos);

				//图片	
				POS pos = chJpegInfo.pJPEGInfo->GetHeadPosition();
				while (NULL != pos)
				{

					EMAIL_JPEG_INFO &JPEGNode = chJpegInfo.pJPEGInfo->GetNext(pos);

					sprintf( pOutBuff, "--%s\r\n",MAIL_BOUNDARY2);
					pOutBuff += strlen(pOutBuff);					

					char szName[48] = {0};
					tm time = DVRTimeToTm(JPEGNode.time);
					
					if (EmailInfoNode.chn != INVALID_INDEX(unsigned long))
					{
						if (Num > 1)
						{
							snprintf(szName, 48, "CH%02d_%02d%02d%02d%02d%02d%02d_%d",chJpegInfo.channel+1,time.tm_year + 1900 - 2000, time.tm_mon + 1, time.tm_mday, time.tm_hour, time.tm_min, time.tm_sec, EmailInfoCount+1);
						}
						else
						{
							snprintf(szName, 48, "CH%02d_%02d%02d%02d%02d%02d%02d",   chJpegInfo.channel+1,time.tm_year + 1900 - 2000, time.tm_mon + 1, time.tm_mday, time.tm_hour, time.tm_min, time.tm_sec);
						}			
					}
					else
					{
						if (Num > 1)
						{
							snprintf(szName, 48, "%02d%02d%02d%02d%02d%02d_%d",       chJpegInfo.channel+1,time.tm_year + 1900 - 2000, time.tm_mon + 1, time.tm_mday, time.tm_hour, time.tm_min, time.tm_sec, EmailInfoCount+1);
						}
						else
						{
							snprintf(szName, 48, "%02d%02d%02d%02d%02d%02d",          chJpegInfo.channel+1,time.tm_year + 1900 - 2000, time.tm_mon + 1, time.tm_mday, time.tm_hour, time.tm_min, time.tm_sec);
						}
					}


					//邮件内容方式显示图片附件
					sprintf(pOutBuff, "Content-Type: image/jpeg;\r\n\tname=\"%s.jpg\"\r\n", szName);
					pOutBuff += strlen(pOutBuff);

					sprintf( pOutBuff,"Content-Disposition: attachement;\r\n\tfilename=\"%s.jpg\"\r\n",szName);
					pOutBuff += strlen(pOutBuff);

					sprintf( pOutBuff, "Content-Transfer-Encoding: base64\r\n\r\n");
					pOutBuff += strlen(pOutBuff);

					//解决：邮件在iOS5上，图片显示错乱的问题。
					//如果要使用Content-ID，那么就要用一个随机数。
					//或者不使用此域。
					//sprintf( pOutBuff, "Content-ID: <%lu>\r\n\r\n", 1);
					//pOutBuff += strlen(pOutBuff);
					

					int base64Len = to64frombits( (unsigned char *)pOutBuff, (const unsigned char*)(JPEGNode.pData), JPEGNode.length, true);
					pOutBuff += base64Len;

					sprintf( pOutBuff, "\r\n\r\n");
					pOutBuff += strlen(pOutBuff);		

				}

			}
			
		} 

		EmailInfoCount++;
	}
	
	sprintf( pOutBuff, "\r\n\r\n");
	pOutBuff += strlen(pOutBuff);	
	
	sprintf( pOutBuff, "--%s--\r\n", MAIL_BOUNDARY2);  
	pOutBuff += strlen(pOutBuff);	

	return ( pOutBuff - pStart );
}


//64BASE  编码算法
int CSmtpMan::to64frombits(unsigned char *out, const unsigned char *in, int inlen, bool bAttach)
{
	static const char base64digits[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	int iLen = inlen;
	int iCnt = 0;
	for (; inlen >= 3; inlen -= 3)
	{
		*out++ = base64digits[in[0] >> 2];
		*out++ = base64digits[((in[0] << 4) & 0x30) | (in[1] >> 4)];
		*out++ = base64digits[((in[1] << 2) & 0x3c) | (in[2] >> 6)];
		*out++ = base64digits[in[2] & 0x3f];
		in += 3;
		++iCnt;
		if (bAttach && iCnt % 19 == 0)
		{
			*out++ = '\r';
			*out++ = '\n';
		}
	}

	if (inlen > 0)
	{
		unsigned char fragment;

		*out++ = base64digits[in[0] >> 2];
		fragment = (in[0] << 4) & 0x30;

		if (inlen > 1)
			fragment |= in[1] >> 4;

		*out++ = base64digits[fragment];
		*out++ = (inlen < 2) ? '=' : base64digits[(in[1] << 2) & 0x3c];
		*out++ = '=';
	}
	*out   = '\0';
	return (iLen / 3 + ((inlen % 3 == 0) ? 0 : 1)) * 4 + iCnt / 19 * 2;
}


int CSmtpMan::InitSSL()
{
	SSL_load_error_strings();                /* readable error messages */
	ERR_load_BIO_strings();		
	SSL_library_init();                      /* initialize library */	
	return 0;
}


bool CSmtpMan::CheckSendInfo()
{
	bool bRet = false;
	
	if (NULL == m_pSendInfo)
	{
		return bRet;
	}

	if ((strlen(m_pSendInfo->server) <= 0) || (strlen(m_pSendInfo->name) <= 0))
	{
		return bRet;
	}

	return true;
}


void CSmtpMan::ReleaseEmailInfo(EMAIL_INFO *pEmailInfo)
{
#ifdef __ENVIRONMENT_LINUX__ 
	if (pEmailInfo->pJPEGInfoList != NULL)
	{
		assert(pEmailInfo->id > -1);
		POS pos = pEmailInfo->pJPEGInfoList->GetHeadPosition();
		while (pos != NULL)
		{
			EMAIL_CH_JPEG_INFO &chJpegInfo = pEmailInfo->pJPEGInfoList->GetAt(pos);
			if (chJpegInfo.bufferIndex != -1)
			{
				CSnapProc::Instance()->ReleaseBuffer(chJpegInfo.bufferIndex);
				chJpegInfo.bufferIndex = -1;
			}
			if (chJpegInfo.pJPEGInfo != NULL)
			{
				chJpegInfo.pJPEGInfo->RemoveAll();
				delete chJpegInfo.pJPEGInfo;
				chJpegInfo.pJPEGInfo = NULL;
			}
			pEmailInfo->pJPEGInfoList->GetNext(pos);
		}

		pEmailInfo->pJPEGInfoList->RemoveAll();
		delete pEmailInfo->pJPEGInfoList;
		pEmailInfo->pJPEGInfoList = NULL;		
	}
#endif

	if (pEmailInfo->pAlarmInfoList != NULL)
	{
		pEmailInfo->pAlarmInfoList->RemoveAll();
		delete pEmailInfo->pAlarmInfoList;
		pEmailInfo->pAlarmInfoList = NULL;
	}	
}


void CSmtpMan::GetAlarmTypeName(ALARM_TYPE alarmType, char *szName, int len)
{
	switch(alarmType)
	{
	case ALARM_TYPE_MONTION:
		{
			snprintf(szName, len, "%s", m_alarmTypeMotion);
		}
		break;
	case ALARM_TYPE_SENSOR:
		{
			snprintf(szName, len, "%s", m_alarmTypeSensor);
		}
		break;
	case ALARM_TYPE_VIDEOLOSS:
		{
			snprintf(szName, len, "%s", m_alarmTypeLoss);
		}
		break;
	case ALARM_TYPE_TEST:
		{
			snprintf(szName, len, "%s", m_alarmTypeTest);
		}
		break;
	case ALARM_TYPE_DISK_FULL:
		{
			snprintf(szName, len, "%s", m_alarmTypeDiskFull);
		}
		break;
	case ALARM_TYPE_IP_CONFLICT:
		{
			snprintf(szName, len, "%s", m_alarmTypeIpConflict);
		}
		break;
	case ALARM_TYPE_DISCONNET:
		{
			snprintf(szName, len, "%s", m_alarmTypeDisconnet);
		}
		break;
	case ALARM_TYPE_DISK_WARNING:
		{
			snprintf(szName, len, "%s", m_alarmTypeDiskError);
		}
		break;
	case ALARM_TYPE_DISK_DISCONNET:
		{
			snprintf(szName, len, "%s", m_alarmTypeDiskDisConnect);
           
		}
        break;
	default:
		{
			snprintf(szName, len, "%s", m_alarmTypeOther);
		}
		break;
	}	 
}


/*注意：如下
C:
EHLO tvt.net.cn

S:
250-XFORWARD NAME ADDR PROTO HELO SOURCE
250-263.net
250-SIZE 47185920
250-ETRN
250-AUTH LOGIN
250-AUTH=LOGIN
250 8BITMIME

最后一个250后面跟了一个空格，这条回复才算结束，有些服务器可能分多次发过来，我们要做判断：
（1）最后为"\r\n"
（2）最后一行的记录第[3]个下标的数值为空格
（3）前三个为数字
*/
bool CSmtpMan::CheckReplyEnd(const char *pBuff, int iLen)
{
	//3个数字 + 空格 + 最少1个字节提示信息 + 回车换行
	if (iLen < 7) 
	{
		return false;
	}

	if(0 != memcmp(&(pBuff[iLen - 2]), "\r\n", 2))
	{
		return false;
	}

	const char *pLastLRCF = NULL;
	for (int i = iLen - 3; i >= 0; --i) 
	{
		if(0 == memcmp(pBuff + i, "\r\n", 2))
		{			
			pLastLRCF = pBuff + i;
			break;
		}
	}

	if (pLastLRCF == NULL) {
		if(isdigit(pBuff[0]) && isdigit(pBuff[1]) && isdigit(pBuff[2]) && (pBuff[3] == ' '))
		{
			return true;
		}
		//printf("pBuff = %s\n", pBuff);

	}
	else{
		if (isdigit(pLastLRCF[2]) && isdigit(pLastLRCF[3]) && isdigit(pLastLRCF[4]) && (pLastLRCF[5] == ' '))
		{
			return true;
		}
		//printf("pLastLRCF = %s\n", pLastLRCF);
	}
	return false;
}

