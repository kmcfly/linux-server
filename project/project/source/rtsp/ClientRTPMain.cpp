#include "ClientRTPMain.h"
#ifndef WIN32
#include <signal.h>
#include <sys/syscall.h>
#endif

#if defined __ENVIRONMENT_LINUX__
#include <unistd.h>
#endif

CClientRTPMain::CClientRTPMain()
{
	m_thread_index_val = 0;
	m_bIsStart = false;
	for( int idx = 0; idx < RTP_THREAD_MAX_COUNT; idx++ )
	{
		m_hRecvThreadID[idx] = PUB_CREATE_THREAD_FAIL;
	}

	m_hSendThreadID = PUB_CREATE_THREAD_FAIL;
	m_bSendThreadProc = false;

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

	m_run_lines = 0;

	for( int i = 0; i < SESSION_DEL_LIST_MAX_CNT; i++ )
	{
		m_pdelSessList[i] = NULL;
	}
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

	for( int idx = 0; idx < RTP_THREAD_MAX_COUNT; idx++ )
	{
		m_hRecvThreadID[idx] = PUB_CreateThread(RecvThread, (void *)this, &m_stThreadCon[idx].m_bRecvThreadProc);
		if( PUB_CREATE_THREAD_FAIL == m_hRecvThreadID[idx] )
		{
			PUB_PrintError(__FILE__,__LINE__);
			return false;
		}
	}


	m_hSendThreadID = PUB_CreateThread(SendThread, (void *)this, &m_bSendThreadProc);
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

	CloseAllSession( 0 );
	DeleteAllExistSessionItem();
	
	for( int idx = 0; idx < RTP_THREAD_MAX_COUNT; idx++ )
	{
		if (PUB_THREAD_ID_NOINIT == m_hRecvThreadID[idx] || false == m_stThreadCon[idx].m_bRecvThreadProc)
		{
			printf("%s %d err\n", __FILE__, __LINE__);
			return false;
		}

		PUB_ExitThread(&m_hRecvThreadID[idx], &m_stThreadCon[idx].m_bRecvThreadProc);
	}

	if (PUB_THREAD_ID_NOINIT == m_hSendThreadID || false == m_bSendThreadProc)
	{
		printf("%s %d err\n", __FILE__, __LINE__);
		return false;
	}
	PUB_ExitThread(&m_hSendThreadID, &m_bSendThreadProc);
	
	m_bIsStart = false;

	return true;
}

bool CClientRTPMain::BindThreadToCpu(int pid, int whichCpu)
{
#ifdef __ENVIRONMENT_LINUX__
	cpu_set_t mask;

	CPU_ZERO(&mask);
	CPU_SET(whichCpu, &mask);
	if (0 != sched_setaffinity((pid_t)pid, sizeof(mask), &mask))
	{
		fprintf(stderr, "%s %s %d\n", strerror(errno), __FILE__, __LINE__);
		return false;
	}
	else
	{
		return true;
	}
#else 
	return false;
#endif
}

PUB_THREAD_RESULT PUB_THREAD_CALL CClientRTPMain::RecvThread(void *pParam)
{
#ifndef	WIN32
	pid_t pid = getpid();
	pid_t tid = syscall(__NR_gettid);
	printf("%s %d pid = %d, tid = %d,  RTPCLientRecvThread\n",__FILE__, __LINE__, pid, tid);

#if defined(__CHIP3535__)
	char szCommond[128] = {0};
	snprintf(szCommond, sizeof(szCommond), "taskset -p 0x2 %d", tid);
	printf("%s:%s:%d, The commond is %s\n", __FUNCTION__, __FILE__, __LINE__, szCommond);
	system(szCommond);
	PUB_Sleep(100);
	printf("%s:%s:%d, The commond is %s\n", __FUNCTION__, __FILE__, __LINE__, szCommond);
	system(szCommond);
#elif defined(__CHIP3531__)
	CClientRTPMain::BindThreadToCpu(tid, 1);
#endif
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

//#define RTP_RECV_SLEEP_PROC_TYPE 1
void CClientRTPMain::RecvProc()
{
	int ret = 0;
	struct timeval timeOut;
	fd_set FD_Set;
	fd_set FD_WriteSet;
	fd_set FD_ErrorSet;
	SWL_socket_t MaxSockFd;
	int timeOutTimes = 0;
	unsigned int iSleepTimes = 0;

	CRtpThreadContext* p_thread_contex = NULL;

	m_thread_index_lock.Lock();
	p_thread_contex = &m_stThreadCon[m_thread_index_val];
	p_thread_contex->m_iSessidValueStart = m_thread_index_val*RTP_THREAD_SESSID_NUM;
	p_thread_contex->m_iSessidMaxCount = RTP_THREAD_SESSID_NUM;
	m_thread_index_val++;
	m_thread_index_lock.UnLock();

	while(p_thread_contex->m_bRecvThreadProc)
	{
		p_thread_contex->m_Lock.Lock();
		if (p_thread_contex->m_listSessionInfo.empty())
		{
			p_thread_contex->m_Lock.UnLock();

			PUB_Sleep(100);
			continue;
		}

#ifdef RTP_RECV_SLEEP_PROC_TYPE
		timeOut.tv_sec = 0;
		timeOut.tv_usec = 100000;
#endif

		FD_Set = p_thread_contex->m_fdSet;
		FD_WriteSet = p_thread_contex->m_fdSet;
		FD_ErrorSet = p_thread_contex->m_fdSet;
		MaxSockFd = p_thread_contex->m_MaxSocketFd;

		p_thread_contex->m_Lock.UnLock();

#ifdef RTP_RECV_SLEEP_PROC_TYPE
		ret = select(MaxSockFd+1, &FD_Set, &FD_WriteSet, &FD_ErrorSet, &timeOut);
#else
		ret = select(MaxSockFd+1, &FD_Set, &FD_WriteSet, &FD_ErrorSet, NULL);
#endif
		if (ret > 0)
		{
			p_thread_contex->m_iSpsRecvDataLenToSleep = 0;
			RecvAllData(p_thread_contex,FD_Set, ret);
			timeOutTimes = 0;

#ifdef RTP_RECV_SLEEP_PROC_TYPE
#else
			iSleepTimes++;
			if( iSleepTimes > 2 )
			{
				//if( p_thread_contex->m_iSpsRecvDataLenToSleep > (ret>>2) )
				{
					PUB_Sleep(20);
					iSleepTimes = 0;
				}
			}
#endif
		}
		else if(ret < 0)
		{
			//CloseSockErrorSession( p_thread_contex, FD_ErrorSet, 1 );
			CloseAllSession( 1 );
			timeOutTimes++;

#ifdef RTP_RECV_SLEEP_PROC_TYPE
#else
			PUB_Sleep(20);
			iSleepTimes = 0;
#endif
		}
		else
		{
			if (timeOutTimes++ > 500)
			{
				timeOutTimes = 0;
				CloseAllSession( 2 );
			}

#ifdef RTP_RECV_SLEEP_PROC_TYPE
#else
			PUB_Sleep(20);
			iSleepTimes = 0;
#endif
		}
	}
}

void CClientRTPMain::SendProc()
{
	std::list<RTP_SESSION_INFO*>::iterator iter;
	unsigned long tmpLen = 0;
	int ret, DataSendCount = 0;
	bool bSleep = false;
	int index;
	CRtpThreadContext* pThreadCon = NULL;
	
	while(m_bSendThreadProc)
	{
		DataSendCount = 0;
		bSleep = false;

		for( index = 0; index < RTP_THREAD_MAX_COUNT; index++ )
		{
			pThreadCon = &m_stThreadCon[index];

			pThreadCon->m_Lock.Lock();

			if( pThreadCon->m_listSessionInfo.size() > 0 )
			{
				for(iter=pThreadCon->m_listSessionInfo.begin(); iter!=pThreadCon->m_listSessionInfo.end();)
				{
					if( (*iter)->SendDataLen <= 0 )
					{
						if (0 == (*iter)->pSession->SendData((*iter)->pSendBuf + (*iter)->SendDataLen, tmpLen))//取得要发送的数据
						{
							(*iter)->SendDataLen += tmpLen;
						}
					}

					if( (*iter)->SendDataLen > 0 )
					{
						ret = SendData((*iter));//真正发送
						if (0 > ret || (*iter)->rtspSendErrorCnt > 3000 )
						{
							printf("%s %d : %d,%d Client RTP xxx Send Failed 3 (%d,%d)\n", __FILE__, __LINE__, 
								(*iter)->TVTSessionID, (*iter)->chnnNumber, ret, (*iter)->rtspSendErrorCnt );

							CloseASession(pThreadCon,(*iter), true);

							iter = pThreadCon->m_listSessionInfo.erase(iter);

							continue;
						}
						else if(0 < ret)
						{
							DataSendCount++;
						}
					}
					++iter;
				}

			}
			pThreadCon->m_Lock.UnLock();

		}

		if( DataSendCount <= 0 )
		{
			PUB_Sleep(200);
		}
	}
}

int CClientRTPMain::CloseSockErrorSession(CRtpThreadContext* pThreadCon, fd_set &CurSet, int count)
{
	std::list<RTP_SESSION_INFO*>::iterator iter;
	pThreadCon->m_Lock.Lock();
	for (iter=pThreadCon->m_listSessionInfo.begin(); iter!=pThreadCon->m_listSessionInfo.end();)
	{
		if (FD_ISSET((*iter)->pSock->GetSockfd(), &CurSet))
		{
			printf("%s %d : %d,%d Socket Error close sessiong position = %d\n", __FILE__, __LINE__, 
				(*iter)->TVTSessionID, (*iter)->chnnNumber, count);

			CloseASession(pThreadCon,(*iter), true );
			iter = pThreadCon->m_listSessionInfo.erase(iter);

			continue;
		}
		++iter;
	}
	pThreadCon->m_Lock.UnLock();

	return 0;

}

int CClientRTPMain::RecvAllData(CRtpThreadContext* pThreadCon, fd_set &CurSet, int count)
{
	std::list<RTP_SESSION_INFO*>::iterator iter;
	int ret, times = 0;
	bool bExit = false;
	pThreadCon->m_Lock.Lock();
	for (iter=pThreadCon->m_listSessionInfo.begin(); iter!=pThreadCon->m_listSessionInfo.end();)
	{
		bExit = false;
		
		if (times >= count)
		{
			break;
		}

		if (FD_ISSET((*iter)->pSock->GetSockfd(), &CurSet))
		{
			times++;

			ret = RecvData(*iter);
			if (ret > 0)
			{
				if( (*iter)->RecvDataLen < (RTP_CLIENT_RECV_BUF_LEN>>1))
				{
					pThreadCon->m_iSpsRecvDataLenToSleep++;
				}

				ret = (*iter)->pSession->GetData((*iter)->pRecvBuf, (*iter)->RecvDataLen, RTP_CLIENT_RECV_BUF_LEN );
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
					printf("%s %d : Client RTP Handle Date Failed, %d,%d\n", 
						__FILE__, __LINE__, (*iter)->TVTSessionID, (*iter)->chnnNumber);
				}
			}
			else if(ret < 0)
			{
				bExit = true;
				printf("%s %d : Client RTP Recv Date Failed, %d,%d\n", 
					__FILE__, __LINE__, (*iter)->TVTSessionID, (*iter)->chnnNumber);
#ifdef WIN32
				int err = WSAGetLastError();
				printf("%s %d : Client RTP Recv Failed.err=%d\n", __FILE__, __LINE__,err);
#endif

			}

			if (bExit)
			{
				printf("%s %d : %d,%d Client RTP Send Failed 2\n", __FILE__, __LINE__, 
					(*iter)->TVTSessionID, (*iter)->chnnNumber);
				CloseASession(pThreadCon,(*iter), true );
				iter = pThreadCon->m_listSessionInfo.erase(iter);
				continue;
			}
		}
		else//FD_ISSET  false
		{
			
		}

		++iter;
	}
	pThreadCon->m_Lock.UnLock();

	return 0;
}

int CClientRTPMain::RecvData(RTP_SESSION_INFO *pSessionInfo)
{
	int ret = 0;

	ret = pSessionInfo->pSock->RecvData(pSessionInfo->pRecvPos, RTP_CLIENT_RECV_BUF_LEN - (pSessionInfo->pRecvPos - pSessionInfo->pRecvBuf));
	if (ret > 0)
	{
		pSessionInfo->pRecvPos += ret;
		pSessionInfo->RecvDataLen += ret;
		pSessionInfo->pSession->ClearRecvWaitTime();
	}
	else if (0 > ret)
	{
		//printf("!**! connection is gracefully closed.....\n");
	}
	else
	{
 		if (pSessionInfo->pSession->GetRecvWaitTime() > 500)
 		{
 			printf("RTSP Recv Wait Time > 500  chnn : %d--%d,%d --- %d\n", 
				__LINE__, pSessionInfo->chnnNumber, pSessionInfo->TVTSessionID,
				(pSessionInfo->pRecvPos - pSessionInfo->pRecvBuf) );
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
	bool bRtspIsPlaying = false;

	ret = pSessionInfo->pSock->SendData(pSessionInfo->pSendPos, Len, 0);
	if (ret < 0)
	{
		bRtspIsPlaying = pSessionInfo->pSession->GetRtspIsPlaying();
		if( bRtspIsPlaying )
		{
			pSessionInfo->rtspSendErrorCnt++;
			ret = Len; 
		}
		else
		{
			//printf("%s %d Send Failed\n", __FILE__, __LINE__);
			return -1;
		}
	}
	else if( 0 == ret )
	{
		bRtspIsPlaying = pSessionInfo->pSession->GetRtspIsPlaying();
		if( bRtspIsPlaying )
		{
			pSessionInfo->rtspSendErrorCnt++;
			ret = Len;
		}
	}
	else
	{
		pSessionInfo->rtspSendErrorCnt = 0;
	}
	
	if(ret > 0)
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

int CClientRTPMain::StartASession(const char *pURL, const char *pUserName, const char *pPassWd, 
								  RTP_CALLBACK pCallBack, void *pParam, PGET_IPC_USED_INFO pIpcUsedInfo,
								  int chl_num, int sub_chn, bool bAudioEnable,
								  unsigned long* pErrorCallback, PRTP_FUNC_PROC_CALLBACK pFuncProcCallback )
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
	CRtpThreadContext* pThreadCon = &m_stThreadCon[chl_num%RTP_THREAD_MAX_COUNT];
	
	pThreadCon->m_Lock.Lock();
	for (int i=0; i<pThreadCon->m_iSessidMaxCount; ++i)
	{
		if (0 == pThreadCon->m_ArraySessionID[i])
		{
			retID = pThreadCon->m_iSessidValueStart + i + 1;
			pThreadCon->m_ArraySessionID[i] = retID;
			break;
		}
	}
	pThreadCon->m_Lock.UnLock();

	if (retID == 0)
	{
		printf( "current channel %d,%d no Session ID number\n", chl_num, sub_chn );
		return 0;
	}

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

	RTP_SESSION_INFO *CurSessionInfo = NULL;
	CClientRTSPSession *pSession = NULL;
	CRTPSock *pRTSPSocket = NULL;

	CurSessionInfo = DelListGetItem( true );
	if( CurSessionInfo )
	{
		pSession = CurSessionInfo->pSession;

		pSession->ResetObject();
		pSession->ClearRecvWaitTime();

		pSession->SetChannelNumber(chl_num, sub_chn );
		pSession->SetAudioEnable( bAudioEnable );
		pSession->SetCallBackErrorNumAddr(pErrorCallback);

		ret = pSession->ParseURL(pURL, username, passwd, pIP, port);
		if (ret < 0)
		{
			pThreadCon->m_Lock.Lock();
			pThreadCon->m_ArraySessionID[retID-1-pThreadCon->m_iSessidValueStart] = 0;
			pThreadCon->m_Lock.UnLock();

			if( !DelListInsertItem(CurSessionInfo ) )
			{
				DeleteSessionItemMem( CurSessionInfo );
			}

			printf( "current ss channel %d,%d ParseURL error %s,%d\n", chl_num, sub_chn, __FILE__, __LINE__ );
			return -12;
		}


		pRTSPSocket = CurSessionInfo->pSock;
		pRTSPSocket->ResetObject();
		if (pRTSPSocket->CreateSock(port, inet_addr(pIP), IPPROTO_TCP) < 0)
		{
			pThreadCon->m_Lock.Lock();
			pThreadCon->m_ArraySessionID[retID-1-pThreadCon->m_iSessidValueStart] = 0;
			pThreadCon->m_Lock.UnLock();

			if( !DelListInsertItem(CurSessionInfo ) )
			{
				DeleteSessionItemMem( CurSessionInfo );
			}

			//printf("current channel %d,%d, create sock error %s %d err\n", chl_num, sub_chn, __FILE__, __LINE__);
			return -3;
		} 

		pSession->SetCallBack(pCallBack, pParam, retID);
		pSession->SetGetIpcUsedInfoCallback(pIpcUsedInfo, pParam );
		pSession->SetFuncProcCallback( pFuncProcCallback, pParam );

		CurSessionInfo->pCallBackObj = pParam;
		CurSessionInfo->TVTSessionID = retID;
		CurSessionInfo->chnnNumber = chl_num;

		memset( CurSessionInfo->pRecvBuf, 0, RTP_CLIENT_RECV_BUF_LEN );
		memset( CurSessionInfo->pSendBuf, 0, RTP_CLIENT_SEND_BUF_LEN );

		CurSessionInfo->pSendPos = CurSessionInfo->pSendBuf;
		CurSessionInfo->pRecvPos = CurSessionInfo->pRecvBuf;
		CurSessionInfo->RecvDataLen = 0;
		CurSessionInfo->SendDataLen = 0;

		CurSessionInfo->rtspSendErrorCnt = 0;
	}
	else
	{
		pSession = new CClientRTSPSession;
		assert(pSession);

		pSession->SetChannelNumber(chl_num, sub_chn );
		pSession->SetAudioEnable( bAudioEnable );
		pSession->SetCallBackErrorNumAddr(pErrorCallback);

		ret = pSession->ParseURL(pURL, username, passwd, pIP, port);
		if (ret < 0)
		{
			pThreadCon->m_Lock.Lock();
			pThreadCon->m_ArraySessionID[retID-1-pThreadCon->m_iSessidValueStart] = 0;
			pThreadCon->m_Lock.UnLock();

			delete pSession;
			pSession = NULL;
			printf( "current channel %d,%d ParseURL error %s,%d\n", chl_num, sub_chn, __FILE__, __LINE__ );
			return -2;
		}


		pRTSPSocket = new CRTPSock;
		assert(pRTSPSocket);
		if (pRTSPSocket->CreateSock(port, inet_addr(pIP), IPPROTO_TCP) < 0)
		{
			pThreadCon->m_Lock.Lock();
			pThreadCon->m_ArraySessionID[retID-1-pThreadCon->m_iSessidValueStart] = 0;
			pThreadCon->m_Lock.UnLock();

			delete pSession;
			delete pRTSPSocket;
			pSession = NULL;
			pRTSPSocket = NULL;
			//printf("current channel %d,%d, create sock error %s %d err\n", chl_num, sub_chn, __FILE__, __LINE__);
			return -3;
		} 

		pSession->SetCallBack(pCallBack, pParam, retID);
		pSession->SetGetIpcUsedInfoCallback(pIpcUsedInfo, pParam );
		pSession->SetFuncProcCallback( pFuncProcCallback, pParam );

		CurSessionInfo = new RTP_SESSION_INFO;
		assert(CurSessionInfo);
		CurSessionInfo->pCallBackObj = pParam;
		CurSessionInfo->TVTSessionID = retID;
		CurSessionInfo->chnnNumber = chl_num;
		CurSessionInfo->pSession = pSession;
		//CurSessionInfo->RequestURL = pURL;
		CurSessionInfo->pSock = pRTSPSocket;
		CurSessionInfo->pRecvBuf = new char[RTP_CLIENT_RECV_BUF_LEN];
		CurSessionInfo->pSendBuf = new char[RTP_CLIENT_SEND_BUF_LEN];
		assert(CurSessionInfo->pRecvBuf != NULL);
		assert(CurSessionInfo->pSendBuf != NULL);

		memset( CurSessionInfo->pRecvBuf, 0, RTP_CLIENT_RECV_BUF_LEN );
		memset( CurSessionInfo->pSendBuf, 0, RTP_CLIENT_SEND_BUF_LEN );

		CurSessionInfo->pSendPos = CurSessionInfo->pSendBuf;
		CurSessionInfo->pRecvPos = CurSessionInfo->pRecvBuf;
		CurSessionInfo->RecvDataLen = 0;
		CurSessionInfo->SendDataLen = 0;

		CurSessionInfo->rtspSendErrorCnt = 0;
	}


	pThreadCon->m_Lock.Lock();
	pSession->Start(pURL, username, passwd);
	FD_SET(pRTSPSocket->GetSockfd(), &pThreadCon->m_fdSet);
	pThreadCon->m_MaxSocketFd = pThreadCon->m_MaxSocketFd >= pRTSPSocket->GetSockfd() ? pThreadCon->m_MaxSocketFd : pRTSPSocket->GetSockfd();
	pThreadCon->m_listSessionInfo.push_back(CurSessionInfo);
	pThreadCon->m_Lock.UnLock();


	printf( "StartASession success channel %d,%d retid %d\n", chl_num, sub_chn, retID );

	return retID;
}

bool CClientRTPMain::StopASession(int TVTSeesionID)
{
	//if (0 >= TVTSeesionID || TVTSeesionID > RTP_MAX_REQUEST_URL_NUM)
	//{
	//	return false;
	//}


	char TearDown[512] = {0};
	std::list<RTP_SESSION_INFO*>::iterator iter;

	int pos = TVTSeesionID/RTP_THREAD_SESSID_NUM;
	if( pos < 0 || pos >= RTP_THREAD_MAX_COUNT )
	{
		return false;
	}

	CRtpThreadContext* pThreadCon = &m_stThreadCon[pos];

	pThreadCon->m_Lock.Lock();
	for (iter = pThreadCon->m_listSessionInfo.begin(); iter!=pThreadCon->m_listSessionInfo.end(); ++iter)
	{
		if (TVTSeesionID == (*iter)->TVTSessionID)
		{
			printf("%s %d : %d,%d stop sesssion not send error 1\n", __FILE__, __LINE__, 
				(*iter)->TVTSessionID, (*iter)->chnnNumber);
			CloseASession(pThreadCon,(*iter), false);
			pThreadCon->m_listSessionInfo.erase(iter);
			break;
		}
	}

	for (int i=0; i<pThreadCon->m_iSessidMaxCount; ++i)
	{
		if (TVTSeesionID == pThreadCon->m_iSessidValueStart+i+1)
		{
			pThreadCon->m_ArraySessionID[i] = 0;
			break;
		}
	}

	pThreadCon->m_Lock.UnLock();
	return true;
}

void CClientRTPMain::CloseAllSession(int run_pos)
{
	printf("%s %d TimeOut CloseAll++++++++run position %d\n", __FILE__, __LINE__, run_pos );

	char TearDown[512] = {0};
	std::list<RTP_SESSION_INFO*>::iterator iter;

	for( int inx = 0; inx < RTP_THREAD_MAX_COUNT; inx++ )
	{
		CRtpThreadContext* pThreadCon = &m_stThreadCon[inx];

		pThreadCon->m_Lock.Lock();
		for (iter = pThreadCon->m_listSessionInfo.begin(); iter!=pThreadCon->m_listSessionInfo.end();)
		{
			(*iter)->pSession->HandleConnectionBreak();
			(*iter)->pSession->SendTearDown(TearDown);
			(*iter)->pSock->SendData(TearDown, strlen(TearDown), 100);
			(*iter)->pSock->CloseSock();
			(*iter)->pSession->Stop();

			delete [](*iter)->pRecvBuf;
			delete [](*iter)->pSendBuf;
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

			iter = pThreadCon->m_listSessionInfo.erase(iter);
		}

		//modifoed  让上层清ID
		// 	for (int i=0; i<RTP_MAX_REQUEST_URL_NUM; ++i)
		// 	{
		// 		m_ArraySessionID[i] = 0;
		// 	}

		pThreadCon->m_MaxSocketFd = 0;
		FD_ZERO(&pThreadCon->m_fdSet);
		pThreadCon->m_Lock.UnLock();

	}


}

void CClientRTPMain::CloseASession(CRtpThreadContext* pThreadCon,RTP_SESSION_INFO *pSessionInf, bool bIsNotifyCaller)
{
	char TearDown[512] = {0};

	FD_CLR(pSessionInf->pSock->GetSockfd(), &pThreadCon->m_fdSet);

	if (bIsNotifyCaller)
	{
		pSessionInf->pSession->HandleConnectionBreak();
	}

	memset( TearDown, 0, 512 );
	pSessionInf->pSession->SendTearDown(TearDown);
	pSessionInf->pSock->SendData(TearDown, strlen(TearDown), 100);
	pSessionInf->pSock->CloseSock();
	pSessionInf->pSession->Stop();

	bool bInsertDelList = DelListInsertItem( pSessionInf );
	if( !bInsertDelList )
	{
		delete []pSessionInf->pRecvBuf;
		delete []pSessionInf->pSendBuf;
		delete pSessionInf->pSession;
		delete pSessionInf->pSock;

		pSessionInf->pSession = NULL;
		pSessionInf->pRecvBuf = NULL;
		pSessionInf->pRecvPos = NULL;
		pSessionInf->pSendBuf = NULL;
		pSessionInf->pSendPos = NULL;
		pSessionInf->pSock = NULL;

		delete pSessionInf;
		pSessionInf = NULL;
	}
	else
	{
		pSessionInf = NULL;
	}

}

void CClientRTPMain::HandleSystemTimeChanged(int TVTSeesionID)
{
	int pos = TVTSeesionID/RTP_THREAD_SESSID_NUM;
	if( pos < 0 || pos >= RTP_THREAD_MAX_COUNT )
	{
		return;
	}

	CRtpThreadContext* pThreadCon = &m_stThreadCon[pos];

	std::list<RTP_SESSION_INFO*>::iterator iter;
	pThreadCon->m_Lock.Lock();
	for (iter = pThreadCon->m_listSessionInfo.begin(); iter!=pThreadCon->m_listSessionInfo.end(); ++iter)
	{
		if (TVTSeesionID == (*iter)->TVTSessionID)
		{
			(*iter)->pSession->HandleSystemTimeChanged();
			break;
		}
	}
	pThreadCon->m_Lock.UnLock();
}

bool CClientRTPMain::GetIsMustResyncSystem(int TVTSeesionID)
{
	std::list<RTP_SESSION_INFO*>::iterator iter;
	bool bResyncTime = false;
	int pos = TVTSeesionID/RTP_THREAD_SESSID_NUM;
	if( pos < 0 || pos >= RTP_THREAD_MAX_COUNT )
	{
		return false;
	}

	CRtpThreadContext* pThreadCon = &m_stThreadCon[pos];

	pThreadCon->m_Lock.Lock();
	for (iter = pThreadCon->m_listSessionInfo.begin(); iter!=pThreadCon->m_listSessionInfo.end(); ++iter)
	{
		if (TVTSeesionID == (*iter)->TVTSessionID)
		{
			bResyncTime = (*iter)->pSession->GetIsMustResyncSystem();
			break;
		}
	}
	pThreadCon->m_Lock.UnLock();

	return bResyncTime;
}

RTP_SESSION_INFO* CClientRTPMain::DelListGetItem( bool bGetSmallMem )
{
	RTP_SESSION_INFO* pSessInfo = NULL;

	m_delListLock.Lock();

	for( int i = 0; i < SESSION_DEL_LIST_MAX_CNT; i++ )
	{
		if( m_pdelSessList[i] )
		{
			pSessInfo = m_pdelSessList[i];
			m_pdelSessList[i] = NULL;
			break;
		}
	}
	m_delListLock.UnLock();

	return pSessInfo;
}

bool CClientRTPMain::DelListInsertItem( RTP_SESSION_INFO* pSessItem )
{
	bool bInsert = false;

	return bInsert;

	m_delListLock.Lock();
	for( int i = 0; i < SESSION_DEL_LIST_MAX_CNT; i++ )
	{
		if( NULL == m_pdelSessList[i] )
		{
			m_pdelSessList[i] = pSessItem;
			bInsert = true;
			break;
		}
	}
	m_delListLock.UnLock();

	return bInsert;

}

void CClientRTPMain::DeleteSessionItemMem( RTP_SESSION_INFO* pSessionInf )
{
	delete []pSessionInf->pRecvBuf;
	delete []pSessionInf->pSendBuf;
	delete pSessionInf->pSession;
	delete pSessionInf->pSock;

	pSessionInf->pSession = NULL;
	pSessionInf->pRecvBuf = NULL;
	pSessionInf->pRecvPos = NULL;
	pSessionInf->pSendBuf = NULL;
	pSessionInf->pSendPos = NULL;
	pSessionInf->pSock = NULL;

	delete pSessionInf;
}

void CClientRTPMain::DeleteAllExistSessionItem()
{
	RTP_SESSION_INFO* pSessInfo = NULL;

	m_delListLock.Lock();

	for( int i = 0; i < SESSION_DEL_LIST_MAX_CNT; i++ )
	{
		if( m_pdelSessList[i] )
		{
			pSessInfo = m_pdelSessList[i];
			DeleteSessionItemMem( pSessInfo );
			m_pdelSessList[i] = NULL;
		}
	}
	m_delListLock.UnLock();
}

void CClientRTPMain::PrintRunPos()
{
	printf("m_run_lines rtmain=%d %d\n", m_run_lines, m_run_recv_lines);

#ifdef __ENVIRONMENT_LINUX__
	if( -1 != access( "/mnt/mtd/ts_pri", F_OK ) )
#else
	if( 0 )
#endif
	{
		std::list<RTP_SESSION_INFO*>::iterator iter;
		CRtpThreadContext* pThreadCon = NULL;

		for( int i = 0; i < RTP_THREAD_MAX_COUNT; i++ )
		{
			pThreadCon = &m_stThreadCon[i];
			for (iter = pThreadCon->m_listSessionInfo.begin(); iter!=pThreadCon->m_listSessionInfo.end(); ++iter)
			{
				printf( "next run pos is channel ID = %d\n", (*iter)->TVTSessionID );
				(*iter)->pSession->PrintRunPos();
			}
		}
	}
}