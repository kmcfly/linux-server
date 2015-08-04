#include "ClientRTPMain.h"
#ifndef WIN32
#include <signal.h>
#endif

//#ifdef assert
//#undef assert
//#define assert(a) ((void)0)
//#endif

CClientRTPMain::CClientRTPMain()
{
	FD_ZERO(&m_fdSet);
	m_MaxSocketFd = 0;
	m_bIsStart = false;
	memset(m_ArraySessionID, 0, sizeof(m_ArraySessionID));

#ifndef WIN32
	sigset_t signal_mask;
	sigemptyset (&signal_mask);
	sigaddset (&signal_mask, SIGPIPE);
	int rc = pthread_sigmask (SIG_BLOCK, &signal_mask, NULL);
	if (rc != 0) 
	{
		printf("block sigpipe error\n");
	} 
#endif 
}


CClientRTPMain::~CClientRTPMain()
{

}

CClientRTPMain *CClientRTPMain::Instance()
{
	static CClientRTPMain m_ClientRTPMain;
	return &m_ClientRTPMain;
}

bool CClientRTPMain::Start()
{
	if (m_bIsStart)
	{
		return false;
	}
	m_hRecvThreadID = PUB_CreateThread(RecvThread, (void *)this, &m_bRecvThreadProc);
	m_hSendThreadID = PUB_CreateThread(SendThread, (void *)this, &m_bSendThreadProc);


	if( PUB_CREATE_THREAD_FAIL == m_hRecvThreadID )
	{
		PUB_PrintError(__FILE__,__LINE__);
		return false;
	}
	if( PUB_CREATE_THREAD_FAIL == m_hSendThreadID )
	{
		PUB_PrintError(__FILE__,__LINE__);
		return false;
	}

	m_bIsStart = true;

	return true;
}

bool CClientRTPMain::Stop()
{
	if (!m_bIsStart)
	{
		return false;
	}

	CloseAllSession();
	
	if (PUB_THREAD_ID_NOINIT == m_hRecvThreadID || false == m_bRecvThreadProc)
	{
		assert(false);
		return false;
	}
	if (PUB_THREAD_ID_NOINIT == m_hSendThreadID || false == m_bSendThreadProc)
	{
		assert(false);
		return false;
	}

	PUB_ExitThread(&m_hRecvThreadID, &m_bRecvThreadProc);
	PUB_ExitThread(&m_hSendThreadID, &m_bSendThreadProc);
	
	m_bIsStart = false;

	return true;
}

PUB_THREAD_RESULT PUB_THREAD_CALL CClientRTPMain::RecvThread(void *pParam)
{
#ifndef	WIN32
	printf("%s %d pid = %d, tid = %d,  RTPCLientRecvThread\n",__FILE__, __LINE__, getpid(), syscall(__NR_gettid));
#endif

	CClientRTPMain *pRTP_RTSPClient = reinterpret_cast<CClientRTPMain *>(pParam);
	pRTP_RTSPClient->RecvProc();
	return 0;
}

PUB_THREAD_RESULT PUB_THREAD_CALL CClientRTPMain::SendThread(void *pParam)
{
#ifndef	WIN32
	printf("%s %d pid = %d, tid = %d,  RTPCLientSendThread\n",__FILE__, __LINE__, getpid(), syscall(__NR_gettid));
#endif

	CClientRTPMain *pRTP_RTSPClient = reinterpret_cast<CClientRTPMain *>(pParam);
	pRTP_RTSPClient->SendProc();
	return 0;
}

void CClientRTPMain::RecvProc()
{
	int ret = 0;
	struct timeval timeOut;
	fd_set FD_Set;
	SWL_socket_t MaxSockFd;
	int timeOutTimes = 0;

	while(m_bRecvThreadProc)
	{
		m_Lock.Lock();
		if (m_listSessionInfo.empty())
		{
			m_Lock.UnLock();
			PUB_Sleep(100);
			continue;
		}
		m_Lock.UnLock();

		timeOut.tv_sec = 0;
		timeOut.tv_usec = 100000;
		m_Lock.Lock();
		FD_Set = m_fdSet;
		MaxSockFd = m_MaxSocketFd;
		m_Lock.UnLock();

		ret = select(MaxSockFd+1, &FD_Set, NULL, NULL, &timeOut);
		if (ret > 0)
		{
			RecvAllData();
			timeOutTimes = 0;
		}
		else if(ret < 0)
		{
			CloseAllSession();
		}
		else
		{
			if (timeOutTimes++ > 50)
			{
				timeOutTimes = 0;
				printf("%s %d TimeOut CloseAll\n", __FILE__, __LINE__);
				CloseAllSession();
			}
			PUB_Sleep(10);
		}
	}
}

void CClientRTPMain::SendProc()
{
	std::list<RTP_SESSION_INFO*>::iterator iter;
	unsigned long tmpLen = 0;
	int ret;
	while(m_bSendThreadProc)
	{
		PUB_Sleep(100);

		m_Lock.Lock();
		for(iter=m_listSessionInfo.begin(); iter!=m_listSessionInfo.end();)
		{
			if (0 == (*iter)->pSession->SendData((*iter)->pSendBuf + (*iter)->SendDataLen, tmpLen))//取得要发送的数据
			{
				(*iter)->SendDataLen += tmpLen;
			}
			ret = SendData((*iter));//真正发送
			if (0 > ret)
			{
				printf("%s %d : Client RTP Send Failed\n", __FILE__, __LINE__);
				CloseASession((*iter));
				iter = m_listSessionInfo.erase(iter);
				continue;
			}
			++iter;
		}

		m_Lock.UnLock();

	}
}


int CClientRTPMain::RecvAllData()
{
	std::list<RTP_SESSION_INFO*>::iterator iter;
	int ret;
	bool bExit = false;
	m_Lock.Lock();
	for (iter=m_listSessionInfo.begin(); iter!=m_listSessionInfo.end();)
	{
		ret = RecvData(*iter);
		if (ret > 0)
		{
			ret = (*iter)->pSession->GetData((*iter)->pRecvBuf, (*iter)->RecvDataLen);
			if (ret == (*iter)->RecvDataLen)
			{
				(*iter)->pRecvPos = (*iter)->pRecvBuf;
				(*iter)->RecvDataLen = 0;
			}
			else if (ret > 0)
			{
				assert((*iter)->RecvDataLen > ret);
				memmove((*iter)->pRecvBuf, (*iter)->pRecvBuf + ret, (*iter)->RecvDataLen - ret);//转移数据
				(*iter)->RecvDataLen -= ret;
				(*iter)->pRecvPos -= ret;
			}
			else if(ret < 0)
			{
				bExit = true;
			}
		}
		else if(ret < 0)
		{
			bExit = true;
		}
		if (bExit)
		{
			printf("%s %d : Client RTP Recv Failed\n", __FILE__, __LINE__);
			CloseASession((*iter));
			iter = m_listSessionInfo.erase(iter);
			continue;
		}

		++iter;
	}
	m_Lock.UnLock();

	return 0;
}

int CClientRTPMain::RecvData(RTP_SESSION_INFO *pSessionInfo)
{
	int ret = 0;

	ret = pSessionInfo->pSock->RecvData(pSessionInfo->pRecvPos, RTP_CLIENT_RECV_BUF_LEN - (pSessionInfo->pRecvPos - pSessionInfo->pRecvBuf), 300);
	if (ret > 0)
	{
		pSessionInfo->pRecvPos += ret;
		pSessionInfo->RecvDataLen += ret;
		pSessionInfo->pSession->ClearRecvWaitTime();
	}
	else if (0 > ret)
	{

	}
	else
	{
 		if (pSessionInfo->pSession->GetRecvWaitTime() > 500)
 		{
 			printf("RTSP Recv Wait Time > 500  : %d\n", __LINE__);
 			ret = -1;
 		}
 		pSessionInfo->pSession->RecvWaitTimeInc();
	}

	return ret;
}

int CClientRTPMain::SendData(RTP_SESSION_INFO *pSessionInfo)
{
	if (NULL == pSessionInfo)
	{
		return -1;
	}
	if (0 == pSessionInfo->SendDataLen)
	{
		return 0;
	}

	int ret = 0;

	unsigned long Len = pSessionInfo->SendDataLen;

	ret = pSessionInfo->pSock->SendData(pSessionInfo->pSendPos, Len, 300);
	if (ret < 0)
	{
		//assert(false);
		return -1;
	}
	else if(ret > 0)
	{
		if (ret == Len)
		{
			pSessionInfo->pSendPos = pSessionInfo->pSendBuf;
		}
		else
		{
			pSessionInfo->pSendPos += ret;
		}
		pSessionInfo->SendDataLen -= ret;
	}
	
	return ret;
}

int CClientRTPMain::StartASession(const char *pURL, const char *pUserName, const char *pPassWd, RTP_CALLBACK pCallBack, void *pParam)
{
	//m_Lock.Lock();
	//std::list<RTP_SESSION_INFO*>::iterator iter = m_listSessionInfo.begin();
	//for (; iter!=m_listSessionInfo.end(); ++iter)
	//{
	//	if (0 == (*iter)->RequestURL.compare(pURL) && pParam == (*iter)->pCallBackObj)
	//	{
	//		return 0;
	//	}
	//}
	//m_Lock.UnLock();

	int retID = 0;
	
	for (int i=0; i<RTP_MAX_REQUEST_URL_NUM; ++i)
	{
		if (0 == m_ArraySessionID[i])
		{
			retID = i+1;
			m_ArraySessionID[i] = retID;
			break;
		}
	}

	CClientRTSPSession *pSession = new CClientRTSPSession;
	char pIP[64] = {0};
	char username[128] = {0};
	char passwd[128] = {0};
	int port = 0;
	int ret = 0;
	if (NULL != pUserName)
	{
		strcpy(username, pUserName);
	}
	if (NULL != pPassWd)
	{
		strcpy(passwd, pPassWd);
	}
	ret = pSession->ParseURL(pURL, username, passwd, pIP, port);
	if (ret < 0)
	{
		delete pSession;
		pSession = NULL;
		return ret;
	}
	
	CRTPSock *pRTSPSocket = new CRTPSock;
	if (pRTSPSocket->CreateSock(port, inet_addr(pIP), IPPROTO_TCP) < 0)
	{
		delete pSession;
		delete pRTSPSocket;
		pSession = NULL;
		pRTSPSocket = NULL;
		//assert(false);
		return -1;
	} 

	pSession->SetCallBack(pCallBack, pParam, retID);

	RTP_SESSION_INFO *CurSessionInfo = new RTP_SESSION_INFO;
	CurSessionInfo->pCallBackObj = pParam;
	CurSessionInfo->TVTSessionID = retID;
	CurSessionInfo->pSession = pSession;
	CurSessionInfo->RequestURL = pURL;
	CurSessionInfo->pSock = pRTSPSocket;
	CurSessionInfo->pRecvBuf = new char[RTP_CLIENT_RECV_BUF_LEN];
	CurSessionInfo->pSendBuf = new char[RTP_CLIENT_SEND_BUF_LEN];
	assert(CurSessionInfo->pRecvBuf != NULL);
	assert(CurSessionInfo->pSendBuf != NULL);
	CurSessionInfo->pSendPos = CurSessionInfo->pSendBuf;
	CurSessionInfo->pRecvPos = CurSessionInfo->pRecvBuf;
	CurSessionInfo->RecvDataLen = 0;
	CurSessionInfo->SendDataLen = 0;

	m_Lock.Lock();
	FD_SET(pRTSPSocket->GetSockfd(), &m_fdSet);
	m_MaxSocketFd = m_MaxSocketFd >= pRTSPSocket->GetSockfd() ? m_MaxSocketFd : pRTSPSocket->GetSockfd();
	m_listSessionInfo.push_back(CurSessionInfo);
	m_Lock.UnLock();

	pSession->Start(pURL, username, passwd);

	return retID;
}

bool CClientRTPMain::StopASession(int TVTSeesionID)
{
	if (0 >= TVTSeesionID || TVTSeesionID > RTP_MAX_REQUEST_URL_NUM)
	{
		return false;
	}

	char TearDown[512] = {0};
	std::list<RTP_SESSION_INFO*>::iterator iter;

	m_Lock.Lock();
	for (iter = m_listSessionInfo.begin(); iter!=m_listSessionInfo.end(); ++iter)
	{
		if (TVTSeesionID == (*iter)->TVTSessionID)
		{
			printf("%s %d Stop A Session : %d\n", __FILE__, __LINE__, TVTSeesionID);
			CloseASession((*iter));

			m_listSessionInfo.erase(iter);
			
			break;
		}
	}
	m_Lock.UnLock();
	return true;
}

void CClientRTPMain::CloseAllSession()
{
	char TearDown[512] = {0};
	std::list<RTP_SESSION_INFO*>::iterator iter;
	m_Lock.Lock();
	for (iter = m_listSessionInfo.begin(); iter!=m_listSessionInfo.end();)
	{
		(*iter)->pSession->HandleConnectionBreak();
		(*iter)->pSession->SendTearDown(TearDown);
		(*iter)->pSock->SendData(TearDown, strlen(TearDown), 100000);
		(*iter)->pSock->CloseSock();
		(*iter)->pSession->Stop();

		delete (*iter)->pRecvBuf;
		delete (*iter)->pSendBuf;
		delete (*iter)->pSession;
		delete (*iter)->pSock;

		(*iter)->pSession = NULL;
		(*iter)->pRecvBuf = NULL;
		(*iter)->pRecvPos = NULL;
		(*iter)->pSendBuf = NULL;
		(*iter)->pSendPos = NULL;
		(*iter)->pSock = NULL;

		delete (*iter);
		(*iter) = NULL;

		iter = m_listSessionInfo.erase(iter);
	}
	for (int i=0; i<RTP_MAX_REQUEST_URL_NUM; ++i)
	{
		m_ArraySessionID[i] = 0;
	}
	m_MaxSocketFd = 0;
	FD_ZERO(&m_fdSet);
	m_Lock.UnLock();
}

void CClientRTPMain::CloseASession(RTP_SESSION_INFO *pSessionInf)
{
	char TearDown[512] = {0};

	FD_CLR(pSessionInf->pSock->GetSockfd(), &m_fdSet);

	pSessionInf->pSession->HandleConnectionBreak();
	pSessionInf->pSession->SendTearDown(TearDown);
	pSessionInf->pSock->SendData(TearDown, strlen(TearDown), 100000);
	pSessionInf->pSock->CloseSock();
	pSessionInf->pSession->Stop();

	delete pSessionInf->pRecvBuf;
	delete pSessionInf->pSendBuf;
	delete pSessionInf->pSession;
	delete pSessionInf->pSock;

	pSessionInf->pSession = NULL;
	pSessionInf->pRecvBuf = NULL;
	pSessionInf->pRecvPos = NULL;
	pSessionInf->pSendBuf = NULL;
	pSessionInf->pSendPos = NULL;
	pSessionInf->pSock = NULL;

	//释放ID
	for (int i=0; i<RTP_MAX_REQUEST_URL_NUM; ++i)
	{
		if (pSessionInf->TVTSessionID == i+1)
		{
			m_ArraySessionID[i] = 0;
			break;
		}
	}	

	delete pSessionInf;
	pSessionInf = NULL;

}