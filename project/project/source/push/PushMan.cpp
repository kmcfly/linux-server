#include "PushMan.h"
#include "UserMan.h"
#include "StringTable.h"
#include "AppInfoServManage.h"

//const unsigned long CLIENT_NUM = 1500;
const unsigned long CLIENT_NUM = 1;

CPushMan * CPushMan::m_pPushMan = NULL;

CPushMan::CPushMan()
{
	m_bUsePush = false;
}

CPushMan::~CPushMan()
{
}

CPushMan * CPushMan::Instance()
{
	if (NULL == m_pPushMan)
	{
		m_pPushMan = new CPushMan;
	}

	return m_pPushMan;
}

int CPushMan::AddDeviceTokenInfo(unsigned long clientID, const DEVICE_TOKEN_INFO * deviceTokenInfo)
{
	DEVICE_TOKEN_INFO_PNTS dtkiPnts;
	memset(&dtkiPnts, 0, sizeof(DEVICE_TOKEN_INFO_PNTS));
	dtkiPnts.deviceTokenInfo = * deviceTokenInfo;

	if (CUserMan::Instance()->GetUser(dtkiPnts.userName, sizeof(dtkiPnts.userName), clientID))
	{
		for (int i=0; i<CLIENT_NUM; i++)
		{
			m_pPushClient[i].AddDeviceTokenInfo(&dtkiPnts);
		}
	}

	return 0;
}

void CPushMan::ChangeIP(const char * pdvrIP)
{
	for (int i=0; i<CLIENT_NUM; i++)
	{
		m_pPushClient[i].ChangeIP(pdvrIP);
	}
}

void CPushMan::ChangePort(unsigned short port)
{
	for (int i=0; i<CLIENT_NUM; i++)
	{
		m_pPushClient[i].ChangePort(port);
	}
}

void CPushMan::ChangeServerInfo(const char * pServerIP, unsigned short serverPort)
{
	for (int i=0; i<CLIENT_NUM; i++)
	{
		m_pPushClient[i].ChangeServerInfo(pServerIP, serverPort);
	}
}

bool CPushMan::Initial(const char * pMSN, const char * pCostomID, unsigned long ulMaxBufMsg)
{
	if (NULL == pMSN)
	{
		m_pPushClient = new CPushClient [CLIENT_NUM];
		int id = 0x12345678;
		int subid = 0xAADD; 
		char szID[MAX_PROVIDER_ID_LEN] = {0};

		for (int i=0; i<CLIENT_NUM; i++)
		{
			snprintf(szID, sizeof(szID), "%08x%04x", id, subid);

			id ++;


			char szPID[MAX_PROVIDER_ID_LEN] = {0};

			szPID[0] = szID[0];
			szPID[1] = szID[1];
			szPID[2] = '-';
			szPID[3] = szID[2];
			szPID[4] = szID[3];
			szPID[5] =  '-';
			szPID[6] = szID[4];
			szPID[7] = szID[5];
			szPID[8] =  '-';
			szPID[9] = szID[6];
			szPID[10] = szID[7];
			szPID[11] =  '-';
			szPID[12] = szID[8];
			szPID[13] = szID[9];
			szPID[14] =  '-';
			szPID[15] = szID[10];
			szPID[16] = szID[11];

			m_pPushClient[i].Initial(szPID, MAX_PROVIDER_ID_LEN, "192.168.3.154", 80, pCostomID, ulMaxBufMsg);
		}
	}
	else
	{
		m_pPushClient = new CPushClient [CLIENT_NUM];
		for (int i=0; i<CLIENT_NUM; i++)
		{
			m_pPushClient[i].Initial(pMSN, MAX_PROVIDER_ID_LEN, "192.168.3.154", 80, pCostomID, ulMaxBufMsg);
		}
	}

	m_bGetPushServerInfo = false;
	return true;
}

void CPushMan::Quit()
{
	for (int i=0; i<CLIENT_NUM; i++)
	{
		m_pPushClient[i].Quit();
	}
	
	delete [] m_pPushClient;
	
	m_pPushClient = NULL;
	m_bGetPushServerInfo = false;
}

bool CPushMan::Start()
{
 	m_hPushThreadID = PUB_CreateThread(PushThread, ((void*)this), &m_bPushThreadProc);
 	if( PUB_CREATE_THREAD_FAIL == m_hPushThreadID )
 	{
 		printf("%s:%s:%d, start push thread error\n", __FUNCTION__, __FILE__, __LINE__);
 		return false;
 	}
 
 	for (int i=0; i<CLIENT_NUM; i++)
 	{
 		m_pPushClient[i].Start();
 	}

	return true;
}

void CPushMan::Stop()
{
	for (int i=0; i<CLIENT_NUM; i++)
	{
		m_pPushClient[i].Stop();
	}

	PUB_ExitThread(&m_hPushThreadID, &m_bPushThreadProc);
	m_bGetPushServerInfo = false;
}

void CPushMan::PushData(PUSH_DATA_TYPE type, void * pData, int len, unsigned long channel/*=0xffffffff*/)
{
	if (!m_bUsePush)
	{
		return;
	}

	int msgLen = sizeof(PUSH_MSG_INFO) + sizeof(PUSH_MSG_HEADER) + sizeof(PROVIDER_PUSH_MSG) + len;
	char * pMsgData = new char [msgLen];

	memset(pMsgData, 0, msgLen);

	PUSH_MSG_HEADER * pHeader = (PUSH_MSG_HEADER *) pMsgData;
	memcpy(pHeader->startFlag, "1111", strlen("1111"));
	pHeader->dataLen = msgLen - sizeof(PUSH_MSG_HEADER);

	PUSH_MSG_INFO * pMsgInfo =  (PUSH_MSG_INFO *)(pMsgData + sizeof(PUSH_MSG_HEADER));
	pMsgInfo->isRequest = 1;
	pMsgInfo->msgType = PUSH_CMD_PUSH_MSG;
	pMsgInfo->msgLen = msgLen - sizeof(PUSH_MSG_HEADER) - sizeof(PUSH_MSG_INFO);

	PROVIDER_PUSH_MSG * pPushMsg = (PROVIDER_PUSH_MSG *)(pMsgData + sizeof(PUSH_MSG_HEADER) + sizeof(PUSH_MSG_INFO));

	char tipTime[20] = {0};
	pPushMsg->time = GetCurrTime32();
	struct tm tmTime = DVRTime32ToTm(pPushMsg->time);
	snprintf(tipTime, sizeof(tipTime), "%02d/%02d %02d:%02d:%02d", tmTime.tm_mon + 1, tmTime.tm_mday, tmTime.tm_hour, tmTime.tm_min, tmTime.tm_sec);

	char tipMsg[30] = {0};
	int tipLen = sizeof(tipMsg);
	GetTipMsg(tipMsg, tipLen, type, pData, len);

	snprintf(pPushMsg->msg, sizeof(pPushMsg->msg), "%s %s", tipTime, tipMsg);


	pPushMsg->msgLen = (int)strlen(pPushMsg->msg);
	pPushMsg->badage = 1;
	//pPushMsg->time = GetCurrTime32();
	pPushMsg->msgType = type;
	//pPushMsg->msgValue = PUSH_ALARM_MOTION;
	GetMsgValue(pPushMsg->msgValue, type, pData, len);
	pPushMsg->paramLen = sizeof(unsigned long);
	memcpy(pPushMsg + 1, &channel, sizeof(unsigned long));

	m_pushMsgListLock.Lock();
	m_pushInfoList.push_back(pHeader);
	m_pushMsgListLock.UnLock();
}

PUB_THREAD_RESULT PUB_THREAD_CALL CPushMan::PushThread(void *pParam)
{
#ifndef	WIN32
	printf("%s %d pid = %d, tid = %d,  CPushMan::PushThread\n",__FILE__, __LINE__, getpid(), syscall(__NR_gettid));
#endif

	CPushMan *pObj = reinterpret_cast<CPushMan *>(pParam);

	pObj->PushProc();

	return 0;
}

void CPushMan::PushProc()
{
	CAppInfoServeManage *pAppInfoServMange = new CAppInfoServeManage;
	while (m_bPushThreadProc)
	{
		if(!m_bGetPushServerInfo)
		{
			string  InfoStr;
			bool bRet = pAppInfoServMange->RequestServerInfo(InfoStr,REQUEST_TOKEN_SERVER_INFO);
			TOKEN_SERVER_INFO tokenServerInfo;
			if(bRet)
			{
				memset(&tokenServerInfo,0,sizeof(tokenServerInfo));
				m_bGetPushServerInfo = pAppInfoServMange->GetTokenServerInfo(InfoStr,tokenServerInfo);
			}
			if(m_bGetPushServerInfo)
			{
				printf("get tokenserver ip= %s...port = %d.....%d....%s..\n",tokenServerInfo.tokenServerIp,atoi(tokenServerInfo.tokenServerPort),__LINE__,__FILE__);
				ChangeServerInfo(tokenServerInfo.tokenServerIp,atoi(tokenServerInfo.tokenServerPort));
				for (int i=0; i<CLIENT_NUM; i++)
			 	{
			 		m_pPushClient[i].SetPushServerInfo(m_bGetPushServerInfo);
			 	}
			}
			else
			{
				PUB_Sleep(5000);
				continue;
			}
			delete pAppInfoServMange;
		}
		m_pushMsgListLock.Lock();
		if (m_pushInfoList.empty())
		{
			m_pushMsgListLock.UnLock();
			PUB_Sleep(1000);
			continue;
		}

		//有要推送的消息
		DispatchPushMsg();

		m_pushMsgListLock.UnLock();

		PUB_Sleep(50);
	}
}

void CPushMan::DispatchPushMsg()
{
	if (!m_pushInfoList.empty())
	{
		PUSH_MSG_HEADER * pushMsgHeader = m_pushInfoList.front();

		bool bRet = false;

		for (int i=0; i<CLIENT_NUM; i++)
		{
			bRet |= m_pPushClient[i].PushData((unsigned char *)pushMsgHeader, pushMsgHeader->dataLen + sizeof(PUSH_MSG_HEADER));
		}

 		if (bRet)
 		{
 			m_pushInfoList.pop_front();
 
 			delete pushMsgHeader;
 		}
	}
}


void CPushMan::GetTipMsg(char * pTipMsg, int &tipLen, PUSH_DATA_TYPE type, void * pData, int len)
{
	int tipMsgLen = tipLen;
	memset(pTipMsg, 0, tipMsgLen);
	static CStringTable * pST = CStringTable::Instance();

	if (DATA_TYPE_ALARM == type)
	{
		PUSH_ARARM_TYPE * pAlarmType = (PUSH_ARARM_TYPE *)pData;
		switch (*pAlarmType)
		{
		case PUSH_ALARM_VIDEOLOSS:		//视频丢失报警
			{
				//snprintf(pTipMsg, tipMsgLen - 1, "%s");
				snprintf(pTipMsg, tipMsgLen - 1, "%s %s", pST->String(E_STR_ID_ALARM), pST->String(E_STR_ID_VIDEO_LOSS));
				printf("%s:%s:%d, alarm video loss\n", __FUNCTION__, __FILE__, __LINE__);
				break;
			}
		case PUSH_ALARM_MOTION:			//动态侦测报警
			{
				//snprintf(pTipMsg, tipMsgLen - 1, "%s");
				snprintf(pTipMsg, tipMsgLen - 1, "%s %s", pST->String(E_STR_ID_ALARM), pST->String(E_STR_ID_MOTION));
				printf("%s:%s:%d, alarm motion\n", __FUNCTION__, __FILE__, __LINE__);
				break;
			}
		case PUSH_ALARM_SENSOR:			//传感器报警
			{
				//snprintf(pTipMsg, tipMsgLen - 1, "%s");
				snprintf(pTipMsg, tipMsgLen - 1, "%s %s", pST->String(E_STR_ID_ALARM), pST->String(E_STR_ID_SENSOR));
				printf("%s:%s:%d, alarm video sensor\n", __FUNCTION__, __FILE__, __LINE__);
				break;
			}
		case PUSH_ALARM_DISKFULL:		//磁盘满报警
			{
				//snprintf(pTipMsg, tipMsgLen - 1, "%s");
				snprintf(pTipMsg, tipMsgLen - 1, "%s %s", pST->String(E_STR_ID_ALARM), pST->String(E_STR_ID_DISK_FULL));
				printf("%s:%s:%d, alarm video disk full\n", __FUNCTION__, __FILE__, __LINE__);
				break;
			}
		case PUSH_ALARM_DISKWARNING:		//磁盘衰减报警
			{
				//snprintf(pTipMsg, tipMsgLen - 1, "%s");
				snprintf(pTipMsg, tipMsgLen - 1, "%s %s", pST->String(E_STR_ID_ALARM), pST->String(E_STR_ID_LOG_DISK_WARNING));
				printf("%s:%s:%d, alarm disk warning\n", __FUNCTION__, __FILE__, __LINE__);
				break;
			}
		default:
			{
				assert(false);
				snprintf(pTipMsg, tipMsgLen - 1, "NULL");
				printf("%s:%s:%d, alarm NULL\n", __FUNCTION__, __FILE__, __LINE__);
				break;
			}
		}
	}
	else
	{
		assert(false);
		snprintf(pTipMsg, tipMsgLen - 1, "NULL");
		printf("%s:%s:%d, alarm NULL\n", __FUNCTION__, __FILE__, __LINE__);
	}

	tipLen = strlen(pTipMsg);
}

void CPushMan::GetMsgValue(int &msgValue, PUSH_DATA_TYPE type, void * pData, int len)
{
	if (DATA_TYPE_ALARM == type)
	{
		PUSH_ARARM_TYPE * pAlarmType = (PUSH_ARARM_TYPE *)pData;
		msgValue = * pAlarmType;
		switch (*pAlarmType)
		{
		case PUSH_ALARM_VIDEOLOSS:		//视频丢失报警
			{
				printf("%s:%s:%d, alarm video loss\n", __FUNCTION__, __FILE__, __LINE__);
				break;
			}
		case PUSH_ALARM_MOTION:			//动态侦测报警
			{
				printf("%s:%s:%d, alarm motion\n", __FUNCTION__, __FILE__, __LINE__);
				break;
			}
		case PUSH_ALARM_SENSOR:			//传感器报警
			{
				printf("%s:%s:%d, alarm video sensor\n", __FUNCTION__, __FILE__, __LINE__);
				break;
			}
		case PUSH_ALARM_DISKFULL:		//磁盘满报警
			{
				printf("%s:%s:%d, alarm video disk full\n", __FUNCTION__, __FILE__, __LINE__);
				break;
			}
		case PUSH_ALARM_DISKWARNING:		//磁盘衰减报警
			{
				printf("%s:%s:%d, alarm disk warning\n", __FUNCTION__, __FILE__, __LINE__);
				break;
			}
		default:
			{
				assert(false);
				printf("%s:%s:%d, alarm NULL\n", __FUNCTION__, __FILE__, __LINE__);
				msgValue = PUSH_ALARM_VIDEOLOSS; 
				break;
			}
		}
	}
	else
	{
		assert(false);
		printf("%s:%s:%d, alarm NULL\n", __FUNCTION__, __FILE__, __LINE__);
	}
}

	
