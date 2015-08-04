/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : yuanshiwei
** Date         : 2009-06-12
** Name         : DataSerProc.cpp
** Version      : 1.0
** Description  :
** Modify Record:
1:
***********************************************************************/

#include "DataSerProc.h"
#include "UserMan.h"
#include "StreamRecordManEx.h"
extern bool g_bHaveSnapPic;

//public
CDataSerProc::CDataSerProc()
{
	m_msgProcID		= PUB_THREAD_ID_NOINIT;
	m_bMsgProc		= false;
}

CDataSerProc::~CDataSerProc()
{
	Quit();
}

#ifdef __ENVIRONMENT_LINUX__ 
bool CDataSerProc::Initial(CMessageMan *pMsgMan, CReclogManEX *pReclogMan, CLogMan *pLogMan, CDiskManager *pDiskMan)
{
	m_pMsgMan    = pMsgMan;
	m_pReclogMan = pReclogMan;
    m_pLogMan    = pLogMan;
	m_pDiskMan   = pDiskMan;
	//m_localVideoInputNum = localVideoInputNum;
	//m_netVideoInputNum = netVideoInputNum;
	m_pSnapPicMan = CSnapMan::GetInstance();
	return true;
}
#else
bool CDataSerProc::Initial(CMessageMan *pMsgMan, CReclogManEX *pReclogMan, CLogMan *pLogMan, unsigned long channelNum)
{
	m_pMsgMan    = pMsgMan;
	m_pReclogMan = pReclogMan;
	m_pLogMan    = pLogMan;
	m_channelNum = channelNum;
	return true;
}
#endif
void CDataSerProc::Quit()
{
    m_msgListLock.Lock();
	std::list<MESSAGE_DATA>::iterator it;
	for (it = m_msgList.begin(); it != m_msgList.end(); ++it) 
	{
		if ((*it).pData != NULL)
		{
			delete[] (*it).pData;
			(*it).pData = NULL;
		}		
	}
	m_msgList.clear();
	m_msgListLock.UnLock();

}
	
bool CDataSerProc::Start()
{
	m_msgProcID = PUB_CreateThread(MessageThread, (void *)this, &m_bMsgProc);
	if( PUB_CREATE_THREAD_FAIL == m_msgProcID )
	{
		PUB_PrintError(__FILE__,__LINE__);
		return false;
	}

	return true;
}

void CDataSerProc::Stop()
{
	if (PUB_THREAD_ID_NOINIT != m_msgProcID)
	{		
		PUB_ExitThread(&m_msgProcID, &m_bMsgProc);
	}
}

void CDataSerProc::PutMsg(const MESSAGE_DATA & msgData)
{
	m_msgListLock.Lock();
	m_msgList.push_back(msgData);
	m_msgListLock.UnLock();
}

//protected:

//private:

bool CDataSerProc::GetMsg(MESSAGE_DATA &msgData)
{
	m_msgListLock.Lock();

	if(0 == m_msgList.size())
	{
		m_msgListLock.UnLock();
		return false;
	}

	std::list<MESSAGE_DATA>::iterator it = m_msgList.begin(); 
	
	msgData.deviceID = (*it).deviceID;
	msgData.clientID = (*it).clientID;
	msgData.pData = (*it).pData;
	msgData.dataLen = (*it).dataLen;
	m_msgList.erase(it);
	m_msgListLock.UnLock();
	return true;	
}


PUB_THREAD_RESULT PUB_THREAD_CALL CDataSerProc::MessageThread(void *pParam)
{
#ifdef __ENVIRONMENT_LINUX__
	pid_t pid = getpid();
	pid_t tid = syscall(__NR_gettid);
	printf("%s, %d, pid = %d, tid=%d\n", __FILE__, __LINE__, pid, tid);
#endif

	CDataSerProc *pThis = reinterpret_cast<CDataSerProc *>(pParam);
	pThis->MessageProc();

	return 0;

}

int CDataSerProc::MessageProc()
{
	MESSAGE_DATA msgData;
	while (m_bMsgProc)
	{
		if (GetMsg(msgData))
		{
			MessageProc(msgData);

			//（2010-01-20 14:27:00 YSW)
			//存着在MessageProc中提前返回的情况，而释放却在最后，这样会导致内存泄漏，所以移到此处保证释放
			m_pMsgMan->ReleaseMsgBuff(msgData);	
		}
		else
		{
			PUB_Sleep(200);
		}
	}

	return 0;

}

void CDataSerProc::MessageProc(const MESSAGE_DATA & msgData)
{
	NET_PROTOCOL cmdType = Protocol(msgData);
	if (CMD_REQUEST_DATA_SEARCH == cmdType)
	{
		NET_DATA_SEARCH *pDataSearchCond = (NET_DATA_SEARCH *)(msgData.pData + sizeof(PACKCMD));
		assert(pDataSearchCond->dataOwnerby >= 0);
		DVR_DISK_OWNER dataOwnerInfo;
		unsigned long channelNum = 0;
#ifdef	__ENVIRONMENT_LINUX__ 
		m_pDiskMan->GetOwnerInfo(pDataSearchCond->dataOwnerby, dataOwnerInfo);
		if (NET_DISK_OWNED_BY_THIS == pDataSearchCond->dataOwnerby)
		{
			channelNum = dataOwnerInfo.dvrPara;
		}
		else
		{
			channelNum = m_pReclogMan->GetDiskChannelNum(dataOwnerInfo.dvrPara);
		}	
#else
		channelNum = m_channelNum;
#endif

		//printf("search cond dateOwner:%d, channnelNum:%d\n", pDataSearchCond->dataOwnerby, channelNum);
		assert(channelNum > 0);

		if(pDataSearchCond->searchType == DATA_SEARCH_TYPE_YEAR)
		{
			CMyList<DVR_DATE> dateInfoList;
			m_pReclogMan->GetDateInfo(dateInfoList, pDataSearchCond->dataOwnerby);
			m_localMsgData.deviceID = LOCAL_DEVICE_ID;
			m_localMsgData.clientID = msgData.clientID;

			if(!dateInfoList.IsEmpty())
			{				
				m_localMsgData.dataLen = sizeof(PACKCMD) + sizeof(NET_DATA_INFO) + sizeof(DVR_DATE)*dateInfoList.GetCount();
				m_localMsgData.pData = new char[m_localMsgData.dataLen];
				assert(m_localMsgData.pData != NULL);
				PACKCMD *pPackCmd = (PACKCMD *)m_localMsgData.pData;
				pPackCmd->cmdType = CMD_REPLY_DATA_INFO;
				pPackCmd->cmdVer = NET_PROTOCOL_VER;
				pPackCmd->dataLen = m_localMsgData.dataLen - sizeof(PACKCMD);
				NET_DATA_INFO *pDataInfo = (NET_DATA_INFO *)(m_localMsgData.pData+sizeof(PACKCMD));
				pDataInfo->dataType = DATA_SEARCH_TYPE_YEAR;
				DVR_DATE *pDvrDate = (DVR_DATE *)(pDataInfo+1);
				while(!dateInfoList.IsEmpty())
				{
					*pDvrDate = dateInfoList.RemoveHead();
					//printf("year-month-day : %d-%d-%d\n", pDvrDate->year, pDvrDate->month, pDvrDate->mday);
					pDvrDate++;
				}
			}
			else
			{
				m_localMsgData.dataLen = sizeof(PACKCMD)+sizeof(NET_DATA_INFO);
				m_localMsgData.pData = new char[m_localMsgData.dataLen];
				assert(m_localMsgData.pData != NULL);
				PACKCMD *pPackCmd = (PACKCMD *)m_localMsgData.pData;
				pPackCmd->cmdType = CMD_REPLY_DATA_NULL;
				pPackCmd->cmdVer = NET_PROTOCOL_VER;
				pPackCmd->dataLen = m_localMsgData.dataLen - sizeof(PACKCMD);
				NET_DATA_INFO *pDataInfo = (NET_DATA_INFO *)(m_localMsgData.pData+sizeof(PACKCMD));
				pDataInfo->dataType = DATA_SEARCH_TYPE_YEAR;
			}
		}
		else if(pDataSearchCond->searchType == DATA_SEARCH_TYPE_TIME)
		{
			SECTION_INFO_EX sectionInfo;
			NET_SECTION_INFO *pNetSectionInfo = NULL;
			PACKCMD *pPackCmd = NULL;
			NET_DATA_INFO *pDataInfo = NULL;
			unsigned long sectionNum = 0;
			unsigned long index = 0;
			SECTION_INFO_EX_LIST *pSectionInfoList = NULL;
			pSectionInfoList = new SECTION_INFO_EX_LIST[channelNum];

			if (0 < pDataSearchCond->videoCH)
			{
				m_pReclogMan->GetSectionInfo(pDataSearchCond->videoCH, pDataSearchCond->startTime, pDataSearchCond->endTime, pSectionInfoList, channelNum, pDataSearchCond->dataOwnerby, true);
			}

			for(char channel = 0; channel < channelNum; channel++)
			{
				sectionNum += pSectionInfoList[channel].GetCount();
			}
			m_localMsgData.deviceID = LOCAL_DEVICE_ID;
			m_localMsgData.clientID = msgData.clientID;
			m_localMsgData.dataLen = sizeof(PACKCMD) + sizeof(NET_DATA_INFO) + sectionNum * sizeof(NET_SECTION_INFO);
			m_localMsgData.pData = new char[m_localMsgData.dataLen];
			assert(m_localMsgData.pData != NULL);
			pPackCmd = (PACKCMD *)m_localMsgData.pData;
			pPackCmd->cmdType = CMD_REPLY_DATA_INFO;
			pPackCmd->cmdVer = NET_PROTOCOL_VER;
			pPackCmd->dataLen = m_localMsgData.dataLen - sizeof(PACKCMD);
			pDataInfo = (NET_DATA_INFO *)(m_localMsgData.pData+sizeof(PACKCMD));
			pDataInfo->dataType = DATA_SEARCH_TYPE_TIME;
			pNetSectionInfo = (NET_SECTION_INFO *)(pDataInfo+1);
			index = 0;
			if (0 != sectionNum)
			{
				for(char channel = 0; channel < channelNum; channel++)
				{
					if (0 == pSectionInfoList[channel].GetCount())
					{
						continue;
					}

					while(!pSectionInfoList[channel].IsEmpty())
					{
						sectionInfo = pSectionInfoList[channel].RemoveHead();
						pNetSectionInfo[index].chnnnel   = channel;
						pNetSectionInfo[index].starTime = sectionInfo.startTime;
						pNetSectionInfo[index].endTime   = sectionInfo.endTime;
						index++;
					}

				}
			}			
			delete [] pSectionInfoList;
			WriteLog(msgData.clientID, LOG_TYPE_SEARCH_TIME);
		}
		else if(pDataSearchCond->searchType == DATA_SEARCH_TYPE_EVENT)
		{
			RECLOG_INFO_EX_LIST *pReclogInfoList = new RECLOG_INFO_EX_LIST[channelNum];
			long eventNum = m_pReclogMan->GetReclog(pDataSearchCond->videoCH, pDataSearchCond->RecordTypeMASK, \
				pDataSearchCond->startTime, pDataSearchCond->endTime, 
				pReclogInfoList, channelNum, pDataSearchCond->dataOwnerby, (LOCAL_CLIENT_ID != msgData.clientID));

			m_localMsgData.deviceID = LOCAL_DEVICE_ID;
			m_localMsgData.clientID = msgData.clientID;
			if(eventNum > 0)
			{
				m_localMsgData.dataLen = sizeof(PACKCMD) + sizeof(NET_DATA_INFO) + sizeof(RECLOG_INFO_EX)*eventNum;
				m_localMsgData.pData = new char[m_localMsgData.dataLen];
				assert(m_localMsgData.pData != NULL);
				PACKCMD *pPackCmd = (PACKCMD *)m_localMsgData.pData;
				pPackCmd->cmdType = CMD_REPLY_DATA_INFO;
				pPackCmd->cmdVer = NET_PROTOCOL_VER;
				pPackCmd->dataLen = m_localMsgData.dataLen - sizeof(PACKCMD);
				NET_DATA_INFO *pDataInfo = (NET_DATA_INFO *)(m_localMsgData.pData+sizeof(PACKCMD));
				pDataInfo->dataType = DATA_SEARCH_TYPE_EVENT;
				RECLOG_INFO_EX *pReclogInfo = (RECLOG_INFO_EX *)(pDataInfo+1);

				for(char channelIndex = 0; channelIndex < channelNum; channelIndex++)
				{
					while(!pReclogInfoList[channelIndex].IsEmpty())
					{
						*pReclogInfo = pReclogInfoList[channelIndex].RemoveTail();
						pReclogInfo++;
					}
				}
			}
			else
			{
				m_localMsgData.dataLen = sizeof(PACKCMD)+sizeof(NET_DATA_INFO);
				m_localMsgData.pData = new char[m_localMsgData.dataLen];
				assert(m_localMsgData.pData != NULL);
				PACKCMD *pPackCmd = (PACKCMD *)m_localMsgData.pData;
				pPackCmd->cmdType = CMD_REPLY_DATA_NULL;
				pPackCmd->cmdVer = NET_PROTOCOL_VER;
				pPackCmd->dataLen = m_localMsgData.dataLen - sizeof(PACKCMD);
				NET_DATA_INFO *pDataInfo = (NET_DATA_INFO *)(m_localMsgData.pData+sizeof(PACKCMD));
				pDataInfo->dataType = DATA_SEARCH_TYPE_EVENT;
			}
			delete [] pReclogInfoList;
			//WriteLog(msgData.clientID, LOG_TYPE_SEARCH_EVENT);
		}
		else if(pDataSearchCond->searchType == DATA_SEARCH_TYPE_FILE)
		{
			
			FILE_INFO_EX_LIST *pFileInfoList = new FILE_INFO_EX_LIST[channelNum];
			long fileNum = m_pReclogMan->GetFileInfo(pDataSearchCond->videoCH, \
				pDataSearchCond->startTime, pDataSearchCond->endTime, \
				pFileInfoList, channelNum, pDataSearchCond->dataOwnerby, (LOCAL_CLIENT_ID != msgData.clientID));

			tm startTime = DVRTime32ToTm(pDataSearchCond->startTime);
			tm endTime = DVRTime32ToTm(pDataSearchCond->endTime);

			m_localMsgData.deviceID = LOCAL_DEVICE_ID;
			m_localMsgData.clientID = msgData.clientID;

			if(fileNum > 0)
			{
				m_localMsgData.dataLen = sizeof(PACKCMD) + sizeof(NET_DATA_INFO) + sizeof(FILE_INFO_EX)*fileNum;
				m_localMsgData.pData = new char[m_localMsgData.dataLen];
				assert(m_localMsgData.pData != NULL);
				PACKCMD *pPackCmd = (PACKCMD *)m_localMsgData.pData;
				pPackCmd->cmdType = CMD_REPLY_DATA_INFO;
				pPackCmd->cmdVer = NET_PROTOCOL_VER;
				pPackCmd->dataLen = m_localMsgData.dataLen - sizeof(PACKCMD);
				NET_DATA_INFO *pDataInfo = (NET_DATA_INFO *)(m_localMsgData.pData+sizeof(PACKCMD));
				pDataInfo->dataType = DATA_SEARCH_TYPE_FILE;
				FILE_INFO_EX *pFileInfo = (FILE_INFO_EX *)(pDataInfo+1);
				for(char channelIndex = 0; channelIndex < channelNum; channelIndex++)
				{
					while(!pFileInfoList[channelIndex].IsEmpty())
					{
						*pFileInfo = pFileInfoList[channelIndex].RemoveTail();
						pFileInfo++;
					}
				}
			}
			else
			{
				m_localMsgData.dataLen = sizeof(PACKCMD)+sizeof(NET_DATA_INFO);
				m_localMsgData.pData = new char[m_localMsgData.dataLen];
				assert(m_localMsgData.pData != NULL);
				PACKCMD *pPackCmd = (PACKCMD *)m_localMsgData.pData;
				pPackCmd->cmdType = CMD_REPLY_DATA_NULL;
				pPackCmd->cmdVer = NET_PROTOCOL_VER;
				pPackCmd->dataLen = m_localMsgData.dataLen - sizeof(PACKCMD);
				NET_DATA_INFO *pDataInfo = (NET_DATA_INFO *)(m_localMsgData.pData+sizeof(PACKCMD));
				pDataInfo->dataType = DATA_SEARCH_TYPE_FILE;
			}
			delete [] pFileInfoList;
			//WriteLog(msgData.clientID, LOG_TYPE_SEARCH_FILE_MAN);
		}
		else if (DATA_SEARCH_TYPE_PIC == pDataSearchCond->searchType)
		{
#if defined (__ENVIRONMENT_LINUX__) && defined (__DVR_BASIC__)
			if (g_bHaveSnapPic)
			{
				unsigned long *pChannel = new unsigned long [channelNum];
				int chnnNum = 0;
				for (int i = 0; i < channelNum; ++i)
				{
					if (pDataSearchCond->videoCH & (((ULONGLONG)(0x01)) << i))
					{
						pChannel[chnnNum] = i;
						chnnNum++;
					}				
				}

				unsigned long picType = 1;

				OUT_PICTURE_INFO_LIST list;
				m_pSnapPicMan->GetPicture(pChannel, chnnNum, &picType, 1, ((LONGLONG)(pDataSearchCond->startTime))*1000000,((LONGLONG)(pDataSearchCond->endTime))*1000000, list, pDataSearchCond->dataOwnerby);

				delete [] pChannel;
				pChannel = NULL;

				m_localMsgData.deviceID = LOCAL_DEVICE_ID;
				m_localMsgData.clientID = msgData.clientID;

				int picNum = list.GetCount();
				if (list.GetCount() > 0)
				{
					m_localMsgData.dataLen = sizeof(PACKCMD) + sizeof(NET_DATA_INFO) + sizeof(SNAP_PIC_PRORITY)*picNum;
					m_localMsgData.pData = new char[m_localMsgData.dataLen];
					assert(m_localMsgData.pData != NULL);
					PACKCMD *pPackCmd = (PACKCMD *)m_localMsgData.pData;
					pPackCmd->cmdType = CMD_REPLY_DATA_INFO;
					pPackCmd->cmdVer = NET_PROTOCOL_VER;
					pPackCmd->dataLen = m_localMsgData.dataLen - sizeof(PACKCMD);
					NET_DATA_INFO *pDataInfo = (NET_DATA_INFO *)(m_localMsgData.pData+sizeof(PACKCMD));
					pDataInfo->dataType = DATA_SEARCH_TYPE_PIC;
					SNAP_PIC_PRORITY *pPicInfo = (SNAP_PIC_PRORITY *)(pDataInfo+1);

					int index = 0;
					while(!list.IsEmpty())
					{
						OUT_PICTURE_INFO outPic = list.RemoveHead();
						pPicInfo[index].diskIndex = outPic.diskIndex;
						pPicInfo[index].nLogPos   = outPic.nLogPos;
						pPicInfo[index].nPos      = outPic.nPos;
						pPicInfo[index].snapTime  = outPic.attribute.time;
						pPicInfo[index].nChannel  = outPic.attribute.nChannel;
						pPicInfo[index].nType     = outPic.attribute.nType;					
						pPicInfo[index].bLock     = outPic.attribute.bLock;
						pPicInfo[index].bFill     = outPic.attribute.bFill;
						pPicInfo[index].nLen      = outPic.attribute.nLen;
						index++;
					}
				}
				else
				{
					m_localMsgData.dataLen = sizeof(PACKCMD)+sizeof(NET_DATA_INFO);
					m_localMsgData.pData = new char[m_localMsgData.dataLen];
					assert(m_localMsgData.pData != NULL);
					PACKCMD *pPackCmd = (PACKCMD *)m_localMsgData.pData;
					pPackCmd->cmdType = CMD_REPLY_DATA_NULL;
					pPackCmd->cmdVer = NET_PROTOCOL_VER;
					pPackCmd->dataLen = m_localMsgData.dataLen - sizeof(PACKCMD);
					NET_DATA_INFO *pDataInfo = (NET_DATA_INFO *)(m_localMsgData.pData+sizeof(PACKCMD));
					pDataInfo->dataType = DATA_SEARCH_TYPE_PIC;
				}
			}
#else
			m_localMsgData.dataLen = sizeof(PACKCMD)+sizeof(NET_DATA_INFO);
			m_localMsgData.pData = new char[m_localMsgData.dataLen];
			assert(m_localMsgData.pData != NULL);
			PACKCMD *pPackCmd = (PACKCMD *)m_localMsgData.pData;
			pPackCmd->cmdType = CMD_REPLY_DATA_NULL;
			pPackCmd->cmdVer = NET_PROTOCOL_VER;
			pPackCmd->dataLen = m_localMsgData.dataLen - sizeof(PACKCMD);
			NET_DATA_INFO *pDataInfo = (NET_DATA_INFO *)(m_localMsgData.pData+sizeof(PACKCMD));
			pDataInfo->dataType = DATA_SEARCH_TYPE_PIC;
#endif			
		}
		else
		{
			assert(false);
		}
		m_pMsgMan->PutMsgToClient(m_localMsgData);

	}
	else if (CMD_REQUEST_DATA_LOCK == cmdType)
	{
		if(!CStreamRecordManEx::Instance()->IsSupportMinorStreamRecord())
		{
			FILE_INFO_EX *pFileInfo = (FILE_INFO_EX *)(msgData.pData + sizeof(PACKCMD));
			int num = (msgData.dataLen - sizeof(PACKCMD))/sizeof(FILE_INFO_EX);
			for (int i = 0; i < num; ++i)
			{
				if (RECORD_FILE_IS_ONLYREAD(pFileInfo[i].type))
				{
					continue;
				}
				if (RECORD_FILE_IS_WRITING(pFileInfo[i].type))
				{
					continue;
				}
				if (RECORD_FILE_IS_LOCK(pFileInfo[i].type))
				{
					continue;
				}
				m_pReclogMan->LockFileEx(pFileInfo[i], true);
			}
		}

		MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_DATA_NULL, NULL, 0);
		m_pMsgMan->PutMsgToClient(m_localMsgData);
		WriteLog(msgData.clientID, LOG_TYPE_LOCK_FILE);
	}
	else if (CMD_REQUEST_DATA_UNLOCK == cmdType)
	{
		if(!CStreamRecordManEx::Instance()->IsSupportMinorStreamRecord())
		{
			FILE_INFO_EX *pFileInfo = (FILE_INFO_EX *)(msgData.pData + sizeof(PACKCMD));
			int num = (msgData.dataLen - sizeof(PACKCMD))/sizeof(FILE_INFO_EX);
			for (int i = 0; i < num; ++i)
			{
				if (RECORD_FILE_IS_ONLYREAD(pFileInfo[i].type))
				{
					continue;
				}
				if (RECORD_FILE_IS_WRITING(pFileInfo[i].type))
				{
					continue;
				}
				if (!RECORD_FILE_IS_LOCK(pFileInfo[i].type))
				{
					continue;
				}
				m_pReclogMan->LockFileEx(pFileInfo[i], false);
			}
		}

		MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_DATA_NULL, NULL, 0);
		m_pMsgMan->PutMsgToClient(m_localMsgData);
		WriteLog(msgData.clientID, LOG_TYPE_UNLOCK_FILE);
	}
	else if (CMD_REQUEST_DATA_DELETE == cmdType)
	{
		if(!CStreamRecordManEx::Instance()->IsSupportMinorStreamRecord())
		{
			FILE_INFO_EX *pFileInfo = (FILE_INFO_EX *)(msgData.pData + sizeof(PACKCMD));
			int num = (msgData.dataLen - sizeof(PACKCMD))/sizeof(FILE_INFO_EX);
	        
			for (int i = 0; i < num; ++i)
			{		
				if (RECORD_FILE_IS_ONLYREAD(pFileInfo[i].type))
				{
					continue;
				}
				if (RECORD_FILE_IS_WRITING(pFileInfo[i].type))
				{
					continue;
				}
				if (RECORD_FILE_IS_LOCK(pFileInfo[i].type))
				{
					continue;
				}
				if (RECORD_FILE_IS_READING(pFileInfo[i].type))
				{
					continue;
				}
				
				m_pReclogMan->DeleteFileEx(pFileInfo[i]);
			}
		}
		

		MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_DATA_NULL, NULL, 0);
		m_pMsgMan->PutMsgToClient(m_localMsgData);
		WriteLog(msgData.clientID, LOG_TYPE_DELETE_FILE);
	}
	else if (CMD_REQUEST_PIC_LOCK == cmdType)
	{
#ifdef __ENVIRONMENT_LINUX__
		SNAP_PIC_PRORITY *pPicInfo = (SNAP_PIC_PRORITY *)(msgData.pData + sizeof(PACKCMD));
		int num = (msgData.dataLen - sizeof(PACKCMD))/sizeof(SNAP_PIC_PRORITY);
		for (int i = 0; i < num; ++i)
		{
			if (SNAP_PIC_IS_ONLYREAD(pPicInfo[i].nType))
			{
				continue;
			}
			if (pPicInfo[i].bLock)
			{
				continue;
			}

			OUT_PICTURE_INFO outPic;
			outPic.diskIndex           = pPicInfo[i].diskIndex;
			outPic.nLogPos             = pPicInfo[i].nLogPos;
			outPic.nPos                = pPicInfo[i].nPos;
			outPic.attribute.time      = pPicInfo[i].snapTime;
			outPic.attribute.nChannel  = pPicInfo[i].nChannel;
			outPic.attribute.nType     = pPicInfo[i].nType;					
			outPic.attribute.bLock     = pPicInfo[i].bLock;
			outPic.attribute.bFill     = pPicInfo[i].bFill;
			outPic.attribute.nLen      = pPicInfo[i].nLen;

			m_pSnapPicMan->SetLockStatus(outPic, true);
		}
#endif
		MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_DATA_NULL, NULL, 0);
		m_pMsgMan->PutMsgToClient(m_localMsgData);
		WriteLog(msgData.clientID, LOG_TYPE_LOCK_PICTURE);
	}
	else if (CMD_REQUEST_PIC_UNLOCK == cmdType)
	{
#ifdef __ENVIRONMENT_LINUX__
		SNAP_PIC_PRORITY *pPicInfo = (SNAP_PIC_PRORITY *)(msgData.pData + sizeof(PACKCMD));
		int num = (msgData.dataLen - sizeof(PACKCMD))/sizeof(SNAP_PIC_PRORITY);
		for (int i = 0; i < num; ++i)
		{
			if (SNAP_PIC_IS_ONLYREAD(pPicInfo[i].nType))
			{
				continue;
			}
			if (!pPicInfo[i].bLock)
			{
				continue;
			}

			OUT_PICTURE_INFO outPic;
			outPic.diskIndex           = pPicInfo[i].diskIndex;
			outPic.nLogPos             = pPicInfo[i].nLogPos;
			outPic.nPos                = pPicInfo[i].nPos;
			outPic.attribute.time      = pPicInfo[i].snapTime;
			outPic.attribute.nChannel  = pPicInfo[i].nChannel;
			outPic.attribute.nType     = pPicInfo[i].nType;					
			outPic.attribute.bLock     = pPicInfo[i].bLock;
			outPic.attribute.bFill     = pPicInfo[i].bFill;
			outPic.attribute.nLen      = pPicInfo[i].nLen;

			m_pSnapPicMan->SetLockStatus(outPic, false);		
		}
#endif
		MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_DATA_NULL, NULL, 0);
		m_pMsgMan->PutMsgToClient(m_localMsgData);
		WriteLog(msgData.clientID, LOG_TYPE_UNLOCK_PICTURE);

	}
	else if (CMD_REQUEST_PIC_DELETE == cmdType)
	{
#ifdef __ENVIRONMENT_LINUX__
		SNAP_PIC_PRORITY *pPicInfo = (SNAP_PIC_PRORITY *)(msgData.pData + sizeof(PACKCMD));
		int num = (msgData.dataLen - sizeof(PACKCMD))/sizeof(SNAP_PIC_PRORITY);
		for (int i = 0; i < num; ++i)
		{
			if (SNAP_PIC_IS_ONLYREAD(pPicInfo[i].nType))
			{
				continue;
			}
			if (pPicInfo[i].bLock)
			{
				continue;
			}

			OUT_PICTURE_INFO outPic;
			outPic.diskIndex           = pPicInfo[i].diskIndex;
			outPic.nLogPos             = pPicInfo[i].nLogPos;
			outPic.nPos                = pPicInfo[i].nPos;
			outPic.attribute.time      = pPicInfo[i].snapTime;
			outPic.attribute.nChannel  = pPicInfo[i].nChannel;
			outPic.attribute.nType     = pPicInfo[i].nType;					
			outPic.attribute.bLock     = pPicInfo[i].bLock;
			outPic.attribute.bFill     = pPicInfo[i].bFill;
			outPic.attribute.nLen      = pPicInfo[i].nLen;

			m_pSnapPicMan->DeleteOne(outPic);			
		}
#endif

		MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_DATA_NULL, NULL, 0);
		m_pMsgMan->PutMsgToClient(m_localMsgData);
		WriteLog(msgData.clientID, LOG_TYPE_DELETE_PICTURE);
	}
	else
	{
		//没有处理的ID，都认为是不支持的功能，所以返回不支持。
		MsgData( m_localMsgData, msgData.clientID, CMD_REPLY_NO_SUPPORT, NULL, 0);
		m_pMsgMan->PutMsgToClient( m_localMsgData );
	}
}

NET_PROTOCOL CDataSerProc::Protocol(const MESSAGE_DATA & msgData)
{
	assert(NULL != msgData.pData);
	PACKCMD *pCmd = reinterpret_cast<PACKCMD *>(msgData.pData);
	return (NET_PROTOCOL)pCmd->cmdType;		
}

void CDataSerProc::MsgData(MESSAGE_DATA &msgData, unsigned long clientID, long cmdType, unsigned char *pData, unsigned long dataLen)
{
	m_cmd.cmdType	= cmdType;
	m_cmd.cmdVer	= NET_PROTOCOL_VER;
	m_cmd.dataLen	= dataLen;

	msgData.clientID = clientID;
	msgData.deviceID = 0;
	msgData.dataLen = sizeof(PACKCMD) + dataLen;

	msgData.pData = new char [msgData.dataLen];

	memcpy(msgData.pData, &m_cmd, sizeof(PACKCMD));

	if ((NULL != pData) && (0 < dataLen))
	{
		memcpy(msgData.pData + sizeof(PACKCMD), pData, dataLen);
	}
}

void CDataSerProc::WriteLog(unsigned long clientID, LOG_TYPE type)
{
	USER_INFO user;
	if (CUserMan::Instance()->GetUser(user, clientID))
	{
		OPERATOR_LOG log;
		log.IP		= user.IP;
		strcpy(log.name, user.name);

		log.contentID = GetContentID(type);
		log.type = (type & 0xffff);

		log.time = GetCurrTime32();

		m_pLogMan->WriteOperatorLog(log);
	}
}
//end


