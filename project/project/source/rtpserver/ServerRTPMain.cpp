
#include "ServerRTPMan.h"
#include "Types.h"
#include "PS_Packet_Packaging.h"

#ifdef _DEBUG
#define  new DEBUG_NEW
#endif

const unsigned long PS_TMP_BUF_LEN = 1024*10;

CServerRTPMan::CServerRTPMan()
{
	for (int j=0; j<MAX_RTSP_MEDIA_COUNT; j++)
	{
		for (int i=0; i<MAX_STREAM_NUM; i++)
		{
			m_LastFrameDateTime[j][i].seconds = 0;
			m_LastFrameDateTime[j][i].microsecond = 0;
			m_LastFrameTs[j][i] = 0;  //spvmn测试 ps流的时间戳必须小于2^30
			m_bIsPackedH264[i] = false;
		}
	}
	memset(m_bRequestedThisChannel, 0, MAX_STREAM_NUM);
	m_SendTimeOutNum = 0;
	m_MaxFdRTP = 0;
	m_MaxFdRTSP = 0;
	CRTSPStatusCode::Instance()->Initial();

	m_pPsPackt = new CPSPackaging();
	assert(m_pPsPackt);

//
   
	memset(m_Scale, 0, sizeof(m_Scale));
	memset(m_LastFrameTime, 0, sizeof(m_LastFrameTime));
	memset(m_ThisFrameTime, 0, sizeof(m_ThisFrameTime));
	memset(m_LastMsgTime, 0, sizeof(m_LastMsgTime));
	memset(m_FrameCountPerMsg, 0, sizeof(m_FrameCountPerMsg));
	memset(m_LastFrameIndex, 0, sizeof(m_LastFrameIndex));
	memset(m_StarSpeedControl, 0, sizeof(m_StarSpeedControl));
	memset(m_bIsPaused, 0, sizeof(m_bIsPaused));


	m_pPsBuf = new char[PS_TMP_BUF_LEN];//一帧最大不超过1M 
	assert(m_pPsBuf);
	memset(m_pPsBuf, 0, PS_TMP_BUF_LEN);
	m_pPsBufLen = 0;
}

CServerRTPMan::~CServerRTPMan()
{
	if (NULL != m_pRTPPort)
	{
		delete []m_pRTPPort;
		m_pRTPPort = NULL;
	}

	if (m_pPsPackt)
	{
		delete m_pPsPackt;
		m_pPsPackt = NULL;
	}
	
	if (m_pPsBuf)
	{
		delete m_pPsBuf;
		m_pPsBuf = NULL;
	}
	
}

CServerRTPMan* CServerRTPMan::Instance()
{
	static CServerRTPMan m_CServerRTPMan;
	return &m_CServerRTPMan;
}

bool CServerRTPMan::Initial(unsigned short MaxConnectNum, unsigned short rtspPort, unsigned short rtpPort)
{
	m_RTSPPort = rtspPort;
	m_RTPStartPort = rtpPort;
	m_bHasInitial = true;
	m_MaxConnectNum = MaxConnectNum;

	//修改配置后重启，会话信息不保存了
	unsigned short *pTmp = new unsigned short[MaxConnectNum];
	memset(pTmp, 0, MaxConnectNum);
	if (NULL != m_pRTPPort)
	{
		delete []m_pRTPPort;
	}
	m_pRTPPort = pTmp;

	return true;
}

bool CServerRTPMan::Start()
{
	if (!m_bHasInitial)
	{
		assert(false);
		return false;
	}

	if (0 != m_RTSPPort)
	{
// 		m_pListenProc = new CSWL_ListenProcEx(AcceptLinkCallback, this);
// 		if (0 != m_pListenProc->StartListen(m_RTSPPort))
// 		{
// 			delete m_pListenProc;
// 			m_pListenProc = NULL;
// 			return false;
// 		}
	}
	else
	{
		printf("%s:%s:%d, The rtsp port err, port : %d\n", __FUNCTION__, __FILE__, __LINE__, m_RTSPPort);
		assert(false);
		return false;
	}

	m_hSendThreadID = PUB_CreateThread(SendThread, (void *)this, &m_bSendThreadProc );	
	if( PUB_CREATE_THREAD_FAIL == m_hSendThreadID )
	{
		printf("%s:%s:%d, Create rtsp send thread error\n", __FUNCTION__, __FILE__, __LINE__);
		return false;
	}

//  	m_hRecvThreadID = PUB_CreateThread(RecvThread, (void *)this, &m_bRecvThreadProc );	
//  	if( PUB_CREATE_THREAD_FAIL == m_hRecvThreadID )
//  	{
//  		printf("%s:%s:%d, Create rtsp recv thread error\n", __FUNCTION__, __FILE__, __LINE__);
//  		return false;
//  	}

 	m_hRTCPThreadID = PUB_CreateThread(RTCPThread, (void *)this, &m_bRTCPThreadProc );	
 	if( PUB_CREATE_THREAD_FAIL == m_hRTCPThreadID )
 	{
 		printf("%s:%s:%d, Create rtsp RTCP thread error\n", __FUNCTION__, __FILE__, __LINE__);
 		return false;
 	}

	m_hPlayBackThreadID = PUB_CreateThread(PlayBackThread, (void *)this, &m_bPlayBackProc );	
	if( PUB_CREATE_THREAD_FAIL == m_hPlayBackThreadID )
	{
		printf("%s:%s:%d, Create rtsp PlayBack thread error\n", __FUNCTION__, __FILE__, __LINE__);
		return false;
	}
	

	return true;
}

bool CServerRTPMan::Stop()
{
	m_Lock.Lock();
	if (NULL != m_pListenProc)
	{
		m_pListenProc->StopListen();
		delete m_pListenProc;
		m_pListenProc = NULL;		
	}

	CloseAllSession();
	
	std::list<CRTPPacket*>::iterator pIter;
	std::map<unsigned long, std::list<CRTPPacket*>*>::iterator mapIter;
	int key;
	for (mapIter=m_mapRTPPacket.begin();mapIter!=m_mapRTPPacket.end();++mapIter)
	{
		key = mapIter->first;
		for (pIter=mapIter->second->begin(); pIter!=mapIter->second->end(); ++pIter)
		{
			delete (*pIter);
			(*pIter) = NULL;
		}
		delete mapIter->second;
		mapIter->second = NULL;
	}
	for (mapIter=m_mapRTPPacketFree.begin();mapIter!=m_mapRTPPacketFree.end();++mapIter)
	{
		key = mapIter->first;
		for (pIter=mapIter->second->begin(); pIter!=mapIter->second->end(); ++pIter)
		{
			delete (*pIter);
			(*pIter) = NULL;
		}
		delete mapIter->second;
		mapIter->second = NULL;
	}
	m_mapRTPPacket.clear();
	m_mapRTPPacketFree.clear();

	memset(m_Scale, 0, sizeof(m_Scale));
	memset(m_LastFrameTime, 0, sizeof(m_LastFrameTime));
	memset(m_ThisFrameTime, 0, sizeof(m_ThisFrameTime));
	memset(m_LastMsgTime, 0, sizeof(m_LastMsgTime));
	memset(m_FrameCountPerMsg, 0, sizeof(m_FrameCountPerMsg));
	memset(m_LastFrameIndex, 0, sizeof(m_LastFrameIndex));
	memset(m_StarSpeedControl, 0, sizeof(m_StarSpeedControl));
	memset(m_bIsPaused, 0, sizeof(m_bIsPaused));
	m_Lock.UnLock();

	if (PUB_THREAD_ID_NOINIT != m_hSendThreadID)
	{		
		PUB_ExitThread(&m_hSendThreadID, &m_bSendThreadProc);
	}
	if (PUB_THREAD_ID_NOINIT != m_hRTCPThreadID)
	{		
		PUB_ExitThread(&m_hRTCPThreadID, &m_bRTCPThreadProc);
	}
	if (PUB_THREAD_ID_NOINIT != m_hPlayBackThreadID)
	{		
		PUB_ExitThread(&m_hPlayBackThreadID, &m_bPlayBackProc);
	}
	return true;
}


int CServerRTPMan::AcceptLinkCallback(CLIENT_SOCK_INFO *pClientSockInfo, void *pParam1, void *pParam2)
{
	CServerRTPMan *pRTSPMan = reinterpret_cast<CServerRTPMan *>(pParam1);
	return pRTSPMan->OnGetAcceptLink(pClientSockInfo);
}

int	CServerRTPMan::OnGetAcceptLink(CLIENT_SOCK_INFO *pClientSockInfo)
{
	m_Lock.Lock();
	printf("____Current Session Count %d ______\n", m_listSession.size());
	if (m_listSession.size() >= m_MaxConnectNum)
	{
		SWL_CloseSocket(pClientSockInfo->pTcpSock);
		m_Lock.UnLock();
		return -1;
	}
	m_Lock.UnLock();

	struct sockaddr_in clientAddr;
	memset(&clientAddr, 0, sizeof(struct sockaddr_in));

	clientAddr.sin_family		= AF_INET;
	clientAddr.sin_addr.s_addr	= pClientSockInfo->dwIP;
	clientAddr.sin_port			= pClientSockInfo->nPort;

	CServerRTPSession *pRTPSession = new CServerRTPSession(clientAddr, pClientSockInfo->pTcpSock);
	if (NULL == pRTPSession)
	{
		SWL_CloseSocket(pClientSockInfo->pTcpSock);
		return -1;
	}

	//这里待完善，需要传入，流媒体参数
	//pRTPSession->SetMediaInfo(GetRTPServerPort());
	
	AddSession(pRTPSession, pClientSockInfo->pTcpSock, SWL_INVALID_SOCKET);

	return 0;
}


PUB_THREAD_RESULT PUB_THREAD_CALL CServerRTPMan::SendThread(void *pParam)
{
#ifndef	WIN32
	printf("%s %d pid = %d, tid = %d,  CServerRTPMan::SendThread\n",__FILE__, __LINE__, getpid(), syscall(__NR_gettid));
#endif

	CServerRTPMan *pServerRTPMan = reinterpret_cast<CServerRTPMan *>(pParam);
	pServerRTPMan->SendProc();

	return 0;
}

PUB_THREAD_RESULT PUB_THREAD_CALL CServerRTPMan::RecvThread(void *pParam)
{
#ifndef	WIN32
	printf("%s %d pid = %d, tid = %d,  CServerRTPMan::RecvThread\n",__FILE__, __LINE__, getpid(), syscall(__NR_gettid));
#endif

	CServerRTPMan *pServerRTPMan = reinterpret_cast<CServerRTPMan *>(pParam);
	pServerRTPMan->RecvProc();

	return 0;
}

PUB_THREAD_RESULT PUB_THREAD_CALL CServerRTPMan::RTCPThread(void *pParam)
{
#ifndef	WIN32
	printf("%s %d pid = %d, tid = %d,  CServerRTPMan::RTCPThread\n",__FILE__, __LINE__, getpid(), syscall(__NR_gettid));
#endif

	CServerRTPMan *pServerRTPMan = reinterpret_cast<CServerRTPMan *>(pParam);
	pServerRTPMan->RTCPProc();

	return 0;
}

PUB_THREAD_RESULT PUB_THREAD_CALL CServerRTPMan::PlayBackThread(void *pParam)
{
#ifndef	WIN32
	printf("%s %d pid = %d, tid = %d,  CServerRTPMan::PlayBackThread\n",__FILE__, __LINE__, getpid(), syscall(__NR_gettid));
#endif

	CServerRTPMan *pServerRTPMan = reinterpret_cast<CServerRTPMan *>(pParam);
	pServerRTPMan->PlayBackProc();

	return 0;
}

void CServerRTPMan::SendProc()
{
	struct timeval timeOut;
	int retVal = 0;
	fd_set				FdSet;;
	SWL_socket_t		MaxFd;

	while(m_bSendThreadProc)
	{
		retVal = 0;
		FD_ZERO(&FdSet);

		timeOut.tv_sec	= 0;
		timeOut.tv_usec = 10000;

		m_Lock.Lock();
		if (!m_listSession.empty())
		{
			//发送RTSP消息
			if (m_MaxFdRTSP != 0)
			{
				FdSet = m_FdSetRTSP;
				MaxFd = m_MaxFdRTSP;
				m_Lock.UnLock();
				retVal = select(MaxFd+1, NULL, &FdSet, NULL, &timeOut);

				if (0 < retVal)
				{
					SendMsg(FdSet, retVal);
					m_SendTimeOutNum = 0;
					m_Lock.Lock();
				}
				else if (0 == retVal)
				{
					if(++m_SendTimeOutNum > 500)//5 s
					{
						printf("Close All Sessions\n");
						m_Lock.Lock();
						CloseAllSession();
						m_Lock.UnLock();
						continue;
					}
				}
				else
				{
					//DelAll
					printf("%s : %d : Close All Sessions\n", __FILE__, __LINE__);
					m_Lock.Lock();
					CloseAllSession();
					m_Lock.UnLock();
					continue;
				}
			}

			//发送RTP包
			//不同的packet涉及到不同的socket，这里假设Video比Audio数据量大，只需select Video的fd
			//m_Lock.Lock();
			if (m_MaxFdRTP != 0)
			{
				FdSet = m_FdSetRTP;
				MaxFd = m_MaxFdRTP;
				m_Lock.UnLock();
				retVal = select(MaxFd+1, NULL, &FdSet, NULL, &timeOut);

				if (0 < retVal)
				{
					SendPacket(FdSet, retVal);
					m_SendTimeOutNum = 0;
					//PUB_Sleep(1);

				}
				else if (0 == retVal)
				{
					if(++m_SendTimeOutNum > 500)//5 s
					{
						printf("%s : %d : Close All Sessions MaxFd : %d\n", __FILE__, __LINE__, MaxFd);
						m_Lock.Lock();
						CloseAllSession();
						m_Lock.UnLock();
					}
					PUB_Sleep(1);
				}
				else
				{
					//DelAll
					printf("%s : %d : Close All Sessions\n", __FILE__, __LINE__);
					m_Lock.Lock();
					CloseAllSession();
					m_Lock.UnLock();
				}

			}
			else
			{
				m_Lock.UnLock();
				PUB_Sleep(10);
			}
		}
		else
		{
			m_Lock.UnLock();
			PUB_Sleep(10);
		}
	}

	
}

void CServerRTPMan::RecvProc()
{
	struct timeval timeOut;
	int retVal = 0;
	fd_set				FdSet;;
	SWL_socket_t		MaxFd;

	while (m_bRecvThreadProc)
	{
		retVal = 0;
		FD_ZERO(&FdSet);

		timeOut.tv_sec	= 0;
		timeOut.tv_usec = 10000;

		m_Lock.Lock();
		if (!m_listSession.empty())
		{
			FdSet = m_FdSetRTSP;
			MaxFd = m_MaxFdRTSP;
			m_Lock.UnLock();
			retVal = select(MaxFd+1, &FdSet, NULL, NULL, &timeOut);

			if (0 < retVal)
			{
				RecvMsg(FdSet, retVal);
			}
			else if (0 == retVal)
			{
				PUB_Sleep(1);
			}
			else
			{
				//DelAll
				printf("%s : %d : Close All Sessions\n", __FILE__, __LINE__);
				m_Lock.Lock();
				CloseAllSession();
				m_Lock.UnLock();
			}
		
			//m_Lock.Lock();
			////释放空间   放在这边，节约其他线程资源
			//CRTPPacket *pRTPPacket = NULL;
			//std::map<unsigned long, std::list<CRTPPacket*>*>::iterator mapIter = m_mapRTPPacket.begin();
			//for (; mapIter!=m_mapRTPPacket.end(); ++mapIter)
			//{
			//	int key = mapIter->first;
			//	int listSize = mapIter->second->size();
			//	for (int j=0; j<listSize; ++j)
			//	{
			//		pRTPPacket = m_mapRTPPacket[key]->front();
			//		if (pRTPPacket->GetCount() != 0)
			//		{
			//			break;
			//		}
			//		m_mapRTPPacket[key]->pop_front();
			//		m_mapRTPPacketFree[key]->push_back(pRTPPacket);
			//	}
			//}
			//m_Lock.UnLock();


		}
		else
		{
			m_Lock.UnLock();
			PUB_Sleep(10);
		}
	}
}

void CServerRTPMan::RTCPProc()
{
	unsigned short sleep = 0;
	unsigned short timeInterVal = 5000;
	unsigned short SessionSeq = 0;
	int i=0;

	std::list<CServerRTPSession*>::iterator it;
	while (m_bRTCPThreadProc)
	{
		m_Lock.Lock();
		if (!m_listSession.empty())
		{
			sleep = timeInterVal / m_listSession.size();
			m_Lock.UnLock();
			PUB_Sleep(sleep);
			
			m_Lock.Lock();
			if (m_listSession.empty())//因为上次解锁到这次解锁经历了sleep，要重新判断session是否为空
			{
				m_Lock.UnLock();
				PUB_Sleep(100);
				continue;
			}
			for (i=0, it=m_listSession.begin(); it!=m_listSession.end() && i<=SessionSeq; ++it, ++i)
			{
			}
			SessionSeq++;
			if (it==m_listSession.end())
			{
				it = m_listSession.begin();
				SessionSeq = 0;
			}
			//(*it)->SendRTCP();
			m_Lock.UnLock();
		}
		else
		{
			m_Lock.UnLock();
			PUB_Sleep(100);
		}
	}
}

int CServerRTPMan::RecvMsg(fd_set &CurSet,unsigned short ValidNum)
{
	unsigned short times = 0;
	m_Lock.Lock();
	std::list<CServerRTPSession*>::iterator iter = m_listSession.begin();
	for (; iter!=m_listSession.end();)
	{
		if (times >= ValidNum)
		{
			break;
		}
		if (FD_ISSET((*iter)->GetSocketFd(), &CurSet))
		{
			times++;
			if(0 > (*iter)->RecvMsg())
			{
				DeleteSession(*iter);
				iter = m_listSession.erase(iter);
				continue;
			}
		}
		++iter;
	}
	m_Lock.UnLock();
	return 0;
}

// int CServerRTPMan::SendPacketAndMsg(fd_set &CurSet,unsigned short ValidNum)
// {
// 	unsigned short times = 0;
// 	int ret;
// 	m_Lock.Lock();
// 	std::list<CServerRTPSession*>::iterator iter = m_listSession.begin();
// 	for (; iter!=m_listSession.end();)
// 	{
// 		//if (times >= ValidNum)
// 		//{
// 		//	break;
// 		//}
// 		if (FD_ISSET((*iter)->GetSocketFd(), &CurSet))
// 		{
// 			//times++;
// 
// 			//发RTSP消息
// 			ret = (*iter)->SendMsg();
// 			if (ret < 0)
// 			{
// 				DeleteSession(*iter);
// 				iter = m_listSession.erase(iter);
// 				continue;
// 			}
// 
// 			//发RTP包
// 			ret = (*iter)->SendRTPPacket();
// 			if (ret < 0)
// 			{
// 				DeleteSession(*iter);
// 				iter = m_listSession.erase(iter);
// 				continue;
// 			}
// 			else if (ret > 0)
// 			{
// 				if ((*iter)->GetSendWaitTime() > 20000)// >2s
// 				{
// 					printf("RTSP Send Wait Time > 20000  : %d\n", __LINE__);
// 					DeleteSession(*iter);
// 					iter = m_listSession.erase(iter);
// 					continue;
// 				}
// 			}
// 			else//发送正常
// 			{
// 			
// 			}
// 
// 		}
// 		else
// 		{
// 			(*iter)->SendWaitTimeInc();
// 			if ((*iter)->GetSendWaitTime() > 100000)
// 			{
// 				printf("RTSP Send Wait Time > 100000  : %d\n", __LINE__);
// 				DeleteSession(*iter);
// 				iter = m_listSession.erase(iter);
// 				continue;
// 			}
// 		}
// 		++iter;
// 	}
// 	m_Lock.UnLock();
// 	return 0;
// }


int CServerRTPMan::SendPacket(fd_set &CurSet,unsigned short ValidNum)
{
	unsigned short times = 0;
	int ret;
	m_Lock.Lock();
	std::list<CServerRTPSession*>::iterator iter = m_listSession.begin();
	for (; iter!=m_listSession.end();)
	{
		if (times >= ValidNum)
		{
			break;
		}
		if (FD_ISSET((*iter)->GetRTPSocketFd(), &CurSet))
		{
			times++;

			//发RTP包
			ret = (*iter)->SendRTPPacket();
			if (ret < 0)
			{
				DeleteSession(*iter);
				iter = m_listSession.erase(iter);
				continue;
			}
			else if (ret > 0)
			{
				if ((*iter)->GetSendWaitTime() > 20000)// >2s
				{
					printf("RTSP Send Wait Time > 20000  : %d\n", __LINE__);
					DeleteSession(*iter);
					iter = m_listSession.erase(iter);
					continue;
				}
			}
			else//发送正常
			{

			}
		}
		else
		{
			(*iter)->SendWaitTimeInc();
			if ((*iter)->GetSendWaitTime() > 100000)
			{
				printf("RTP Send Wait Time > 100000  : %d\n", __LINE__);
				DeleteSession(*iter);
				iter = m_listSession.erase(iter);
				continue;
			}
		}
		++iter;
	}
	m_Lock.UnLock();
	return 0;
}

int CServerRTPMan::SendMsg(fd_set &CurSet,unsigned short ValidNum)
{
	unsigned short times = 0;
	int ret;
	m_Lock.Lock();
	std::list<CServerRTPSession*>::iterator iter = m_listSession.begin();
	for (; iter!=m_listSession.end();)
	{
		//if (times >= ValidNum)
		//{
		//	break;
		//}
		if (FD_ISSET((*iter)->GetSocketFd(), &CurSet))
		{
			//times++;

			//发RTSP消息
			SWL_socket_t fd = SWL_INVALID_SOCKET;

			ret = (*iter)->SendMsg(fd);//若session是UDP，取得其RTPSocket
			if (ret < 0)
			{
				DeleteSession(*iter);
				iter = m_listSession.erase(iter);
				continue;
			}
			if (SWL_INVALID_SOCKET != fd)
			{
				m_bRequestedThisChannel[(*iter)->GetChannel()] = true;
				m_MaxFdRTP = (m_MaxFdRTP >= fd) ? m_MaxFdRTP : fd;
				FD_SET(fd, &m_FdSetRTP);
			}
		}
		else
		{
			(*iter)->SendWaitTimeInc();
			if ((*iter)->GetSendWaitTime() > 100000)
			{
				printf("RTSP Send Wait Time > 100000  : %d\n", __LINE__);
				DeleteSession(*iter);
				iter = m_listSession.erase(iter);
				continue;
			}
		}
		++iter;
	}
	m_Lock.UnLock();
	return 0;
}


bool CServerRTPMan::AddSession(CServerRTPSession* pServerRTPSession, SWL_socket_t rtspSocketFd, SWL_socket_t rtpSocketFd)
{
	m_Lock.Lock();
	m_listSession.push_back(pServerRTPSession);
	if (SWL_INVALID_SOCKET != rtspSocketFd)
	{
		m_MaxFdRTSP = (m_MaxFdRTSP >= rtspSocketFd) ? m_MaxFdRTSP : rtspSocketFd;
		FD_SET(rtspSocketFd, &m_FdSetRTSP);
	}
	if (SWL_INVALID_SOCKET != rtpSocketFd)
	{
		m_MaxFdRTP = (m_MaxFdRTP >= rtpSocketFd) ? m_MaxFdRTP : rtpSocketFd;
		FD_SET(rtpSocketFd, &m_FdSetRTP);
	}
	if (pServerRTPSession->GetChannel() < MAX_STREAM_NUM)
	{
		m_bRequestedThisChannel[pServerRTPSession->GetChannel()] = true;
	}
	m_Lock.UnLock();
	return true;
}

bool CServerRTPMan::DeleteSession(CServerRTPSession* pServerRTPSession)
{
	printf("DeleteSession %d\n", pServerRTPSession->GetSessionID());
	std::list<CServerRTPSession*>::iterator iter = m_listSession.begin();
	bool bRTSP = false;
	if (m_MaxFdRTSP != 0)
	{
		bRTSP = true;
	}
	m_MaxFdRTSP = 0;
	m_MaxFdRTP = 0;
	for (; iter != m_listSession.end(); ++iter)
	{
		if (pServerRTPSession == (*iter))
		{
			ClearRTPServerPort((*iter)->GetServerRTPPort());
			if (bRTSP)
			{
				FD_CLR((*iter)->GetSocketFd(), &m_FdSetRTSP);
			}
			if (pServerRTPSession->IsSendOverUDP())
			{
				FD_CLR((*iter)->GetRTPSocketFd(), &m_FdSetRTP);
			}
			pServerRTPSession->Quit();
			delete (*iter);
			(*iter) = NULL;
		}
		else
		{
			if (bRTSP)
			{
				m_MaxFdRTSP = (m_MaxFdRTSP >= (*iter)->GetSocketFd()) ? m_MaxFdRTSP : (*iter)->GetSocketFd();
			}
			if ((*iter)->IsSendOverUDP())
			{
				m_MaxFdRTP = (m_MaxFdRTP >= (*iter)->GetRTPSocketFd()) ? m_MaxFdRTP : (*iter)->GetRTPSocketFd();
			}
		}
	}

	return true;
}

void CServerRTPMan::CloseAllSession()
{
	std::list<CServerRTPSession*>::iterator iter = m_listSession.begin();
	for (; iter!=m_listSession.end();)
	{
		if ((*iter)->GetSocketFd() != SWL_INVALID_SOCKET && m_MaxFdRTSP != 0)
		{
			FD_CLR((*iter)->GetSocketFd(), &m_FdSetRTSP);
		}
		
		if ((*iter)->IsSendOverUDP())
		{
			FD_CLR((*iter)->GetRTPSocketFd(), &m_FdSetRTP);
		}
		(*iter)->Quit();
		delete (*iter);
		(*iter) = NULL;
		iter = m_listSession.erase(iter);
	}

	if (m_pRTPPort)
	{
		memset(m_pRTPPort, 0, m_MaxConnectNum);
	}
	m_MaxFdRTSP = 0;
	m_MaxFdRTP = 0;
	FD_ZERO(&m_FdSetRTSP);
	FD_ZERO(&m_FdSetRTP);
	memset(m_bRequestedThisChannel, 0, MAX_STREAM_NUM);
	m_SendTimeOutNum = 0;

//回放
	memset(m_Scale, 0, sizeof(m_Scale));
	memset(m_LastFrameTime, 0, sizeof(m_LastFrameTime));
	memset(m_ThisFrameTime, 0, sizeof(m_ThisFrameTime));
	memset(m_LastMsgTime, 0, sizeof(m_LastMsgTime));
	memset(m_FrameCountPerMsg, 0, sizeof(m_FrameCountPerMsg));
	memset(m_LastFrameIndex, 0, sizeof(m_LastFrameIndex));
	memset(m_StarSpeedControl, 0, sizeof(m_StarSpeedControl));
	memset(m_bIsPaused, 0, sizeof(m_bIsPaused));

	for (int j=0; j<MAX_RTSP_MEDIA_COUNT; j++)
	{
		for (int i=0; i<MAX_STREAM_NUM; i++)
		{
			m_LastFrameDateTime[j][i].seconds = 0;
			m_LastFrameDateTime[j][i].microsecond = 0;
			m_LastFrameTs[j][i] = 0;  //spvmn测试 ps流的时间戳必须小于2^30
			m_bIsPackedH264[i] = false;
		}
	}

	printf("Close All Sessions End\n");
}

bool CServerRTPMan::RellocPacLst(unsigned char Channel)
{
	printf("Begin Relloc Pac Lst , Channel : %d\n", Channel);
	if (Channel >= MAX_STREAM_NUM)
	{
		return false;
	}

	CRTPPacket *pRTPPacket = NULL;
	for (int j=0; j<PACKET_POOL_RELLOC_SIZE; ++j)
	{
		pRTPPacket = new CRTPPacket;
		if (NULL == pRTPPacket)
		{
			assert(false);
			return false;
		}
		m_mapRTPPacketFree[Channel]->push_back(pRTPPacket);
	}
	printf("End Relloc Pac Lst , Channel : %d\n", Channel);

	return true;
}

int CServerRTPMan::GetFrameData(CFrameData *pFrameData, bool IsPlayBack)
{
	unsigned char channel = pFrameData->GetFrameInfo().channel;
	assert(channel < MAX_CH_NUM);

	m_Lock.Lock();

	if (m_listSession.empty())
	{
		m_Lock.UnLock();
		return 0;
	}

	//检查客户端是否请求了该通道
	if (IsPlayBack)
	{
		if (!m_bRequestedThisChannel[channel + MAX_CH_NUM])
		{
			m_Lock.UnLock();
			return 0;
		}
	}
	else
	{
		if (!m_bRequestedThisChannel[channel])
		{
			m_Lock.UnLock();
			return 0;
		}
	}


	if (IsPlayBack)
	{
		//printf("GetFrameData IsPlayBack ch:%d\n", channel);

		if (m_LastMsgTime[channel].seconds == 0)
		{
			printf("m_LastMsgTime[channel].seconds == 0\n");
			m_LastMsgTime[channel] = RTSPGetCurrTime();
		}
		if (pFrameData->GetFrameInfo().frameType == FRAME_TYPE_VIDEO)
		{
			m_FrameCountPerMsg[channel]++;
			//printf("m_FrameCountPerMsg:%d : %lu  index:%lu\n", channel, m_FrameCountPerMsg[channel], pFrameData->GetFrameInfo().frameIndex);
			m_LastFrameIndex[channel] = pFrameData->GetFrameInfo().frameIndex;
			m_LastFrameTime[channel] = m_ThisFrameTime[channel];
			m_ThisFrameTime[channel] = pFrameData->GetFrameInfo().time;
		}

		if (pFrameData->GetFrameInfo().time == 0)
		{
			printf("pFrameData time err ,index %lu\n", pFrameData->GetFrameInfo().bufIndex);
		}

		channel += MAX_CH_NUM;
	}


	if (pFrameData->GetFrameInfo().frameType == FRAME_TYPE_VIDEO_FORMAT)//忽略这种帧
	{
		printf("FRAME_TYPE_VIDEO_FORMAT\n");
		m_Lock.UnLock();
		return 0;
	}
	if (pFrameData->GetFrameInfo().frameType == FRAME_TYPE_END)//回放流结束
	{
		printf("FRAME_TYPE_END\n");
		int SessionID = -1;
		std::list<CServerRTPSession*>::iterator iter = m_listSession.begin();
		for (; iter!=m_listSession.end(); ++iter)
		{
			if (channel == (*iter)->GetChannel())
			{
				SessionID = (*iter)->GetSessionID();
				(*iter)->CallCallBack(SessionID);
				printf("Close Session : %d\n", SessionID);
				break;
			}
		}
		if (SessionID != -1)
		{
// 			int len = 0;
// 			m_pPsPackt->Packet_DSM_CC(m_pPsBuf, len, m_LastFrameTs[STREAM_LIVE_VIDEO][channel], DSM_CC_STOP, false, false);
// 			CreateRFC3550Packet(channel, (unsigned char*)m_pPsBuf, len, false, m_LastFrameTs[STREAM_LIVE_VIDEO][channel], true, 96);
// 			m_Lock.UnLock();
// 			PUB_Sleep(1000);
// 			m_Lock.Lock();
			StopTransRTP(SessionID);
		}
		m_Lock.UnLock();
		

		return 0;
	}

	ReleasePacket(channel);	

	if (m_bIsPackedH264[channel])
	{
		FrameDataToRTPPacket(pFrameData);
	}
	else
	{
		FrameDataToRTPPacketRefBy3550(pFrameData, IsPlayBack);
	}

	m_Lock.UnLock();

	return 0;
}

int CServerRTPMan::FrameDataToRTPPacket(CFrameData *pFrameData)
{
	static float VIDEOTSINC = RTP_VIDEO_CAPTURE_FREQUENCY * 0.000001;
	static float AUDIOTSINC = RTP_AUDIO_CAPTURE_FREQUENCY * 0.000001;


	FRAME_INFO_EX dataFrame = pFrameData->GetFrameInfo();
	int streamType = -1;
	const unsigned char *pData = dataFrame.pData;
	unsigned long channel = dataFrame.channel;
	unsigned long dataLen = dataFrame.length;
	bool bIsKeyFrame = dataFrame.keyFrame;
	printf("bKeyFrame  %d\n", bIsKeyFrame);
	int ret = 0;

	unsigned int FrameSec = dataFrame.time / 1000000;
	unsigned int FrameMicroSec = dataFrame.time % 1000000;

	if (FRAME_TYPE_VIDEO == dataFrame.frameType)
	{
		streamType = STREAM_LIVE_VIDEO;

		if (m_LastFrameDateTime[streamType][channel].microsecond != 0)
		{
			m_LastFrameTs[streamType][channel] += VIDEOTSINC * ((FrameSec - m_LastFrameDateTime[streamType][channel].seconds)*1000000 +\
				(FrameMicroSec - m_LastFrameDateTime[streamType][channel].microsecond));
		}
		else
		{
 			if (bIsKeyFrame != true)//从关键帧开始传
 			{
 				return 0;
 			}
		}
	}
	else if(FRAME_TYPE_AUDIO == dataFrame.frameType)
	{
		streamType = STREAM_LIVE_AUDIO;

		if (m_LastFrameDateTime[streamType][channel].microsecond != 0)
		{
			m_LastFrameTs[streamType][channel] += AUDIOTSINC * ((FrameSec - m_LastFrameDateTime[streamType][channel].seconds)*1000000 +\
				(FrameMicroSec - m_LastFrameDateTime[streamType][channel].microsecond));
		}
	}
	else
	{
		assert(false);
		return -1;
	}	
	m_LastFrameDateTime[streamType][channel].seconds = FrameSec;
	m_LastFrameDateTime[streamType][channel].microsecond = FrameMicroSec;

	//若该通道未预分配，预分配空间
	PreAllocPacLst(channel);
	if (m_mapRTPPacketFree[channel]->size() <= (dataLen / MAX_RTP_PAYLOAD_DATA_LEN))
	{
		RellocPacLst(channel);
	}

	if (STREAM_LIVE_VIDEO == streamType)
	{
		//发送视频数据
		if (!IsStartCode(pData))
		{
			assert(false);
			return -1;
		}

		const unsigned char *pCurPos	= pData;
		const unsigned char *pReadPos	= pCurPos;
		const NALU_HEADER *pNaluHeader	= NULL;

		if (MAX_RTP_PAYLOAD_DATA_LEN > dataLen)
		{
			//不需要分包的视频数据
			ret = CreateRTPPacket(channel, pCurPos + START_CODE_LEN, dataLen - START_CODE_LEN, bIsKeyFrame, m_LastFrameTs[streamType][channel]);
			if ( 0 > ret )
			{
				return ret;
			}
		}
		else
		{
			//需要对这个视频帧分到多个RTP数据报中发送
			do
			{
				if (!IsStartCode(pCurPos))
				{
					assert(false);
					return -1;
				}

				pCurPos		+= START_CODE_LEN;
				pNaluHeader	= (const NALU_HEADER *)pCurPos;
				pCurPos		+= sizeof(char);
				pReadPos	= pCurPos;

				switch (pNaluHeader->type)
				{
				case NAL_UNIT_TYPE_ZERO:
					{
						break;
					}
				case SLICE_LAYER_WITHOUT_PARTITIONING_RBSP:
				case SLICE_DATA_PARTION_A_LAYER_RBSP:
				case SLICE_DATA_PARTION_B_LAYER_RBSP:
				case SLICE_DATA_PARTION_C_LAYER_RBSP:
				case SLICE_LAYER_WITH_PARTITIONING_RBSP:
				case ACCESS_UNIT_DELIMITER_RBSP:
				case END_OF_SEQ_RBSP:
				case END_OF_STREAM_RBSP:
				case FILLER_DATA_RBSP:
					{
						pReadPos += (dataLen - (pCurPos - pData));
						//生成RTP数据包
						ret = CreateRTPFUPacket(channel, pNaluHeader, pCurPos, pReadPos - pCurPos, bIsKeyFrame, m_LastFrameTs[streamType][channel]);
						if ( 0 > ret )
						{
							return ret;
						}
						break;
					}
				case SEI_RBSP:
				case SEQ_PARAMETER_SET_RBSP:
				case PIC_PARAMETER_SET_RBSP:
					{
						pReadPos = NextStartCode(pCurPos, dataLen - (pCurPos - pData));
						if (NULL == pReadPos)
						{
							assert(false);
							return -1;
						}

						//生成RTP数据包
						ret = CreateRTPspsPacket(channel, pCurPos - 1, pReadPos - pCurPos + 1, bIsKeyFrame, m_LastFrameTs[streamType][channel]);
						if ( 0 > ret )
						{
							return ret;
						}
						break;
					}
				default:
					break;
				}

				if (pReadPos < pData + dataLen)
				{
					pCurPos = pReadPos;
				}
				else
				{
					//分包结束
					break;
				}

			}while(1);
		}
	}
	else if (STREAM_LIVE_AUDIO == streamType) //
	{
		if ( 1600 != dataLen)
		{
			assert(false);
			return 0;
		}
		const unsigned char *pPos = pData;
		for (int i=0; i<5; i++)
		{
			if (m_mapRTPPacketFree[channel]->empty())
			{
				RellocPacLst(channel);
			}

			ret = CreateAudioPacket(channel, pPos, 320, m_LastFrameTs[streamType][channel]);
			if ( 0 > ret)
			{
				assert(false);
				return ret;
			}
			pPos += 320;
		}
	}
	else
	{
		assert(false);
		return 0;
	}

	return 0;
}

//使用RFC3550的打包方式
int CServerRTPMan::FrameDataToRTPPacketRefBy3550(CFrameData *pFrameData, bool IsPlayBack)
{
	static float VIDEOTSINC = RTP_VIDEO_CAPTURE_FREQUENCY * 0.000001;
	static float AUDIOTSINC = RTP_AUDIO_CAPTURE_FREQUENCY * 0.000001;
	float PBFix = 1;

	FRAME_INFO_EX dataFrame = pFrameData->GetFrameInfo();
	int streamType = -1;
	const unsigned char *pData = dataFrame.pData;
	unsigned long channel = dataFrame.channel;
	unsigned long dataLen = dataFrame.length;
	bool bIsKeyFrame = dataFrame.keyFrame;
	//printf("bKeyFrame  %d\n", bIsKeyFrame);
	int ret = 0;

	unsigned int FrameSec = dataFrame.time / 1000000;
	unsigned int FrameMicroSec = dataFrame.time % 1000000;

	bool bIsDiscardAudio = false;
	bool bIsPacketPause = false;

	if (IsPlayBack)
	{
 		if (m_Scale[channel] > 0 )
 		{
 			PBFix = 1.0 / abs(m_Scale[channel]);
 		}
 		else if(m_Scale[channel] < 0 )
 		{
 			PBFix = abs(m_Scale[channel]);
 		}
		else
		{
			//printf("m_Scale 0\n");
			if (!m_bIsPaused[channel])
			{
				printf("m_bIsPaused false\n");
				bIsPacketPause = true;
				if (dataFrame.frameType == FRAME_TYPE_VIDEO)
				{
					m_bIsPaused[channel] = true;
				}
			}
		}

		if (PBFix > 1 || (PBFix < 1 && PBFix != 0))
		{
			bIsDiscardAudio = true;
		}

		channel += MAX_CH_NUM;
	}

	if (FRAME_TYPE_VIDEO == dataFrame.frameType)
	{
		streamType = STREAM_LIVE_VIDEO;

		if (m_LastFrameDateTime[streamType][channel].microsecond != 0)
		{
  			m_LastFrameTs[streamType][channel] += PBFix * (VIDEOTSINC * ((FrameSec - m_LastFrameDateTime[streamType][channel].seconds)*1000000 +\
  				(FrameMicroSec - m_LastFrameDateTime[streamType][channel].microsecond)));

// 			unsigned long temp_ts = VIDEOTSINC * ((FrameSec - m_LastFrameDateTime[streamType][channel].seconds)*1000000 +\
// 				(FrameMicroSec - m_LastFrameDateTime[streamType][channel].microsecond));
			//static unsigned long tmpcount = 0;
			//printf("real ts:%d  count:%d  time:%d\n", temp_ts, tmpcount++, RTSPGetCurrTime().seconds);
			
			//test
			//m_LastFrameTs[streamType][channel] += 3600;
		}
		else
		{
			if (bIsKeyFrame != true)//从关键帧开始传
			{
				//printf("still not keyframe\n");
				return 0;
			}
		}
	}
	else if(FRAME_TYPE_AUDIO == dataFrame.frameType)
	{
		if (bIsDiscardAudio)
		{
			return 0;
		}

		//printf("FRAME_TYPE_AUDIO :%d  len :%lu\n", dataFrame.frameType, dataFrame.length);
 		streamType = STREAM_LIVE_AUDIO;
 
		//视频没传 不传音频
		if (m_LastFrameDateTime[STREAM_LIVE_VIDEO][channel].microsecond == 0)
		{
			return 0;
		}
 		if (m_LastFrameDateTime[streamType][channel].microsecond != 0)
 		{
   			m_LastFrameTs[streamType][channel] += PBFix *(AUDIOTSINC * ((FrameSec - m_LastFrameDateTime[streamType][channel].seconds)*1000000 +\
   				(FrameMicroSec - m_LastFrameDateTime[streamType][channel].microsecond)));
 		}
	}
	else if (FRAME_TYPE_AUDIO_FORMAT == dataFrame.frameType)
	{
		return 0;
	}
	else
	{
		printf("frame type err :%d\n", dataFrame.frameType);
		//assert(false);
		return -1;
	}
	m_LastFrameDateTime[streamType][channel].seconds = FrameSec;
	m_LastFrameDateTime[streamType][channel].microsecond = FrameMicroSec;


	//若该通道未预分配，预分配空间
	PreAllocPacLst(channel);
	
	if (m_mapRTPPacketFree[channel]->size() <= (dataLen / MAX_RTP_PAYLOAD_DATA_LEN) + 5)
	{
		RellocPacLst(channel);
	}
	
	int iOutLen = dataLen;
	unsigned char* pcOutBuf = (unsigned char*)pData;

 	assert(dataLen <= 1024*1024);
 	static unsigned char s_buf[1024*1024];
 	pcOutBuf = s_buf;
 	memset(pcOutBuf,0,1024*1024);
 	int iRet = -1;
 
 	if (streamType == STREAM_LIVE_VIDEO)
 	{
 		if (bIsKeyFrame)
 		{
 			iRet = m_pPsPackt->Packet_I_frame( (char *)pData, dataLen, (char* )pcOutBuf, iOutLen, m_LastFrameTs[streamType][channel], m_LastFrameTs[streamType][channel]);
 		}
 		else 
 		{
 			iRet = m_pPsPackt->Packet_P_frame((char *)pData, dataLen, (char*)pcOutBuf, iOutLen, m_LastFrameTs[streamType][channel], m_LastFrameTs[streamType][channel]);
 		}
 	}
	else if (streamType == STREAM_LIVE_AUDIO)
 	{
		if (dataLen == 1620)
		{
			for (int i=0; i<5; i++)
			{
				pData += 4;
				if (m_pPsBufLen + 50 > PS_TMP_BUF_LEN)
				{
					printf("m_pPsBufLen  Err : %lu\n", m_pPsBufLen);
					return -1;
				}
				iRet = m_pPsPackt->Packet_Audio_frame((char *)pData, 320, m_pPsBuf + m_pPsBufLen, iOutLen, m_LastFrameTs[STREAM_LIVE_VIDEO][channel]);
				pData += 320;
				m_pPsBufLen += iOutLen;
			}
		}
		else if(dataLen == 324)
		{
			pData += 4;
			if (m_pPsBufLen + 50 > PS_TMP_BUF_LEN)
			{
				printf("m_pPsBufLen  Err : %lu\n", m_pPsBufLen);
				return -1;
			}
			iRet = m_pPsPackt->Packet_Audio_frame((char *)pData, 320, m_pPsBuf + m_pPsBufLen, iOutLen, m_LastFrameTs[STREAM_LIVE_VIDEO][channel]);
			pData += 320;
			m_pPsBufLen += iOutLen;
		}
		else
		{
			printf("STREAM_LIVE_AUDIO dataLen Err : %lu\n", dataLen);
			return -1;
		}

 	}

 	if (iRet < 0)
 	{
 		__trip;
 		return -1;
 	}

#if 0
	static FILE *fp =NULL;
	if(fp == NULL)
	{
		fp=fopen("/mnt/nfs/1.ps","a+");
	}else
	{
		fwrite(pcOutBuf,1,iOutLen,fp);
	}
#endif

	if (streamType == STREAM_LIVE_VIDEO)//视频帧才发  音频帧先缓存着
	{
		//把音频帧加在视频后面
		if (m_pPsBufLen != 0)
		{
// 			static int dp=0;
// 			if (dp++ % 100 == 0)
// 			{
// 				dump(m_pPsBuf, m_pPsBufLen);
// 			}

			memcpy(pcOutBuf + iOutLen, m_pPsBuf, m_pPsBufLen);
			iOutLen += m_pPsBufLen;
			m_pPsBufLen = 0;
		}

		if (bIsPacketPause)
		{
			printf("Pack DSM_CC\n");
			int tempLen = 0;
			m_pPsPackt->Packet_DSM_CC((char*)(pcOutBuf + iOutLen), tempLen, m_LastFrameTs[STREAM_LIVE_VIDEO][channel], DSM_CC_PAUSE);
			iOutLen += tempLen;

			//dump(pcOutBuf + iOutLen - tempLen, tempLen);
		}

		while(iOutLen > 0)
		{
			if (iOutLen > MAX_RTP_PAYLOAD_DATA_LEN)
			{
				CreateRFC3550Packet(channel, pcOutBuf, MAX_RTP_PAYLOAD_DATA_LEN, bIsKeyFrame, m_LastFrameTs[streamType][channel], false, 96);
				iOutLen -= MAX_RTP_PAYLOAD_DATA_LEN;
				pcOutBuf += MAX_RTP_PAYLOAD_DATA_LEN;
			}
			else
			{
				if (bIsPacketPause)
				{
					CreateRFC3550Packet(channel, pcOutBuf, iOutLen, bIsKeyFrame, m_LastFrameTs[streamType][channel], true, 96, true);
				}
				else
				{
					CreateRFC3550Packet(channel, pcOutBuf, iOutLen, bIsKeyFrame, m_LastFrameTs[streamType][channel], true, 96);
				}
				iOutLen = 0;
				pData = NULL;
			}
		}

 		if (bIsPacketPause)//打完暂停包，需要有resume包
 		{
 			iRet = m_pPsPackt->Packet_DSM_CC((char*)pcOutBuf, iOutLen, m_LastFrameTs[STREAM_LIVE_VIDEO][channel], DSM_CC_PLAY);
 			assert(iOutLen < MAX_RTP_PAYLOAD_DATA_LEN);
 			
 			CreateRFC3550Packet(channel, pcOutBuf, iOutLen, bIsKeyFrame, m_LastFrameTs[streamType][channel], false, 96);
 		}
	}


	return 0;
}

bool CServerRTPMan::IsStartCode(const unsigned char *pData)
{
	if (0x00 != *pData || 0x00 != *(pData + 1) || 0x00 != *(pData + 2) || 0x01 != *(pData + 3))
	{
		return false;
	}

	return true;
}

const unsigned char *CServerRTPMan::NextStartCode(const unsigned char *pData, unsigned long dataLen)
{
	if (NULL == pData)
	{
		return NULL;
	}

	const unsigned char *pNextStartCode = pData;

	while (1)
	{
		if (IsStartCode(pNextStartCode))
		{
			return pNextStartCode;
		}
		else
		{
			pNextStartCode++;
			if (dataLen - (pNextStartCode - pData) < 4)
			{
				break;
			}
		}
	}

	return NULL;
}


int CServerRTPMan::CreateRFC3550Packet(unsigned short channel, const unsigned char *pRTPData, unsigned long dataLen, \
									   bool bIsKeyFrame, unsigned long TimeStamp, bool mark, unsigned short payloadType, bool bIsPaused)
{
	CRTPPacket *pRTPPacket = m_mapRTPPacketFree[channel]->front();
	m_mapRTPPacketFree[channel]->pop_front();

	pRTPPacket->NewRTPPacket(channel, ++m_VSequenceNum[channel], TimeStamp, \
		RTSPGetCurrTime(), pRTPData, dataLen, bIsKeyFrame, mark, payloadType, bIsPaused);

	m_mapRTPPacket[channel]->push_back(pRTPPacket);
	DistributePacket(pRTPPacket, channel);

	return 0;
}

int CServerRTPMan::CreateRTPPacket(unsigned short channel, const unsigned char *pRTPData, unsigned long dataLen, bool bIsKeyFrame, unsigned long TimeStamp)
{
// 	if (m_mapRTPPacketFree[channel]->empty())
// 	{
// 		RellocPacLst(channel);
// 	}
	CRTPPacket *pRTPPacket = m_mapRTPPacketFree[channel]->front();
	m_mapRTPPacketFree[channel]->pop_front();

	pRTPPacket->NewRTPPacket(channel, ++m_VSequenceNum[channel], TimeStamp, \
		RTSPGetCurrTime(), pRTPData, dataLen, bIsKeyFrame);
	m_mapRTPPacket[channel]->push_back(pRTPPacket);

	DistributePacket(pRTPPacket, channel);

	return 0;
}

int CServerRTPMan::CreateRTPspsPacket(unsigned short channel, const unsigned char *pRTPData, unsigned long dataLen, bool bIsKeyFrame, unsigned long TimeStamp)
{
// 	if (m_mapRTPPacketFree[channel]->empty())
// 	{
// 		RellocPacLst(channel);
// 	}
	CRTPPacket *pRTPPacket = m_mapRTPPacketFree[channel]->front();
	m_mapRTPPacketFree[channel]->pop_front();

	pRTPPacket->NewRTPPacket(channel, ++m_VSequenceNum[channel], TimeStamp, \
		RTSPGetCurrTime(), pRTPData, dataLen, bIsKeyFrame);
	m_mapRTPPacket[channel]->push_back(pRTPPacket);
	DistributePacket(pRTPPacket, channel);

	return 0;
}

int CServerRTPMan::CreateRTPFUPacket(unsigned short channel, const NALU_HEADER *pNaluHeader, \
									 const unsigned char *pNaluData, unsigned long dataLen, bool bIsKeyFrame, unsigned long TimeStamp)
{

	CRTPPacket *pRTPPacket = NULL;
	unsigned long lastDataLen = dataLen;
	const unsigned char *pCurPos = pNaluData;
	unsigned long realDataLen = 0;

	NALU_HEADER fuIndicator = *pNaluHeader;
	fuIndicator.type = FRAGMENTATION_UNIT_A;

	FU_HEADER fuHeader;
	fuHeader.type = pNaluHeader->type;
	fuHeader.start_bit = 1;
	fuHeader.end_bit = 0;
	fuHeader.reserve_bit = 0;

	bool bMarker = false;

	//分片的数据要重新处理nalu头
	while (lastDataLen > 0)
	{
// 		if (m_mapRTPPacketFree[channel]->empty())
// 		{
// 			RellocPacLst(channel);
// 		}
		pRTPPacket = m_mapRTPPacketFree[channel]->front();
		m_mapRTPPacketFree[channel]->pop_front();

		if (lastDataLen > MAX_RTP_PAYLOAD_DATA_LEN)
		{
			realDataLen = MAX_RTP_PAYLOAD_DATA_LEN;
		}
		else
		{
			realDataLen = lastDataLen;
			fuHeader.end_bit = 1;
			bMarker = true;
		}

		pRTPPacket->NewRTPPacket(channel, ++m_VSequenceNum[channel], bMarker, TimeStamp, \
			RTSPGetCurrTime(), &fuIndicator, &fuHeader, pCurPos, realDataLen, bIsKeyFrame);
		m_mapRTPPacket[channel]->push_back(pRTPPacket);

		DistributePacket(pRTPPacket, channel);

		fuHeader.start_bit = 0;
		lastDataLen -= realDataLen;
		pCurPos += realDataLen;
		pRTPPacket = NULL;
	}

	return 0;
}

int CServerRTPMan::CreateAudioPacket(unsigned short channel, const unsigned char *pData, unsigned long dataLen, unsigned long TimeStamp)
{
// 	if (m_mapRTPPacketFree[channel]->empty())
// 	{
// 		RellocPacLst(channel);
// 	}
	CRTPPacket *pRTPPacket = m_mapRTPPacketFree[channel]->front();
	m_mapRTPPacketFree[channel]->pop_front();

	pRTPPacket->NewRTPAPacket(channel, ++m_ASequenceNum[channel], TimeStamp, \
		RTSPGetCurrTime(), pData, dataLen, 0x01);
	m_mapRTPPacket[channel]->push_back(pRTPPacket);
	DistributePacket(pRTPPacket, channel);

	return 0;
}

void CServerRTPMan::DistributePacket(CRTPPacket *pPacket, unsigned long channel)
{
	unsigned short streamType = (pPacket->ISAudio()) ? STREAM_LIVE_AUDIO : STREAM_LIVE_VIDEO;
	std::list<CServerRTPSession*>::iterator iter = m_listSession.begin();
	for (; iter!=m_listSession.end(); ++iter)
	{
		if (channel == (*iter)->GetChannel())
		{
			(*iter)->GetRTPPacket(pPacket, streamType);
		}
	}
}

bool CServerRTPMan::PreAllocPacLst(unsigned char channel)
{
	std::map<unsigned long, std::list<CRTPPacket*>*>::iterator it;
	it = m_mapRTPPacketFree.find(channel);
	if(it == m_mapRTPPacketFree.end())
	{
		printf("PreAllocPacLst %d\n", channel);
		m_mapRTPPacketFree[channel] = new std::list<CRTPPacket *>();
		CRTPPacket *pRTPPacket = NULL;
		for (int j=0; j<PACKET_POOL_SIZE; ++j)
		{
			pRTPPacket = new CRTPPacket;
			if (NULL == pRTPPacket)
			{
				assert(false);
				m_Lock.UnLock();
				return false;
			}
			m_mapRTPPacketFree[channel]->push_back(pRTPPacket);
		}
	}

	it = m_mapRTPPacket.find(channel);
	if(it == m_mapRTPPacket.end())
	{
		m_mapRTPPacket[channel] = new std::list<CRTPPacket *>();
	}
	
	return true;
}

bool CServerRTPMan::SaveFrameData(CFrameData *pFrameData, unsigned short channel)
{
	//丢数据
	//if (m_FrameDataLimitSize[channel] >= FRAME_DATE_LIMIT_SIZE)
	//{
	//	printf("channel %d FrameData Full\n", channel);
	//}

	pFrameData->Add();
	m_listFrameData.push_back(pFrameData);


	return true;
}

// void CServerRTPMan::CheckFrameRate(CFrameData * pFrameData)
// {
// 	TVT_DATE_TIME CurTime = pFrameData->GetFrameInfo().frame.time;
// 	int channel = pFrameData->GetFrameInfo().frame.channel;
// 
// 	if (m_LastFrameDateTime[channel].seconds == 0)
// 	{
// 		m_LastFrameDateTime[channel] = pFrameData->GetFrameInfo().frame.time;
// 		return;
// 	}
// 
// 	//时间差 不在一秒范围内			0.5S是随便设的一个阀值
//  	int DefSecond, DefMicroSecond;
//  	DefSecond = CurTime.seconds - m_LastFrameDateTime[channel].seconds;
//  	DefMicroSecond = CurTime.microsecond - m_LastFrameDateTime[channel].microsecond;
//  	if (((DefSecond != 0) &&  DefMicroSecond>= 500000) || DefMicroSecond <= 0)
//  	{
//  		assert(false);
//  		Stop();
//  		return;
//  	}
//  	unsigned short framerate = 0x100000000 / DefMicroSecond;
//  	if (m_aRTPTimestamp[channel].GetFrameRate() != framerate)
//  	{
//  		
//  	}
//  	m_LastFrameDateTime[channel] = CurTime;
// }


unsigned short CServerRTPMan::GetRTPServerPort()
{
	m_Lock.Lock();
	if (NULL != m_pRTPPort)
	{
		for (int i=0; i< m_MaxConnectNum; i++)
		{
			if (m_pRTPPort[i] == 0)
			{
				m_pRTPPort[i] = m_RTPStartPort + 4*i;
				m_Lock.UnLock();
				return m_pRTPPort[i];
			}
		}
	}
	m_Lock.UnLock();
	return 0;
}

void CServerRTPMan::ClearRTPServerPort(unsigned short port)
{
	m_Lock.Lock();
	if (NULL != m_pRTPPort)
	{
		for (int i=0; i< m_MaxConnectNum; i++)
		{
			if (m_pRTPPort[i] == port)
			{
				m_pRTPPort[i] = 0;
				break;
			}
		}
	}
	m_Lock.UnLock();
}


int CServerRTPMan::StartTransRTP(RTPMediaInfo *pMediaInfo)
{
	printf("StartTransRTP\n");
	printf("clientID: %lu  Channel %d\n", pMediaInfo->ClientID, pMediaInfo->Channel);

 	m_Lock.Lock();

	memset(m_pPsBuf, 0, 4096);
	m_pPsBufLen = 0;


	printf("____Current Session Count %d ______\n", m_listSession.size());
	if (m_listSession.size() >= m_MaxConnectNum)
	{
		printf("Too Much Session! Count %d ______\n", m_listSession.size());
		m_Lock.UnLock();
		return -1;
	}

	if (!pMediaInfo)
	{
		m_Lock.UnLock();
		return -1;
	}

	//是否启动过该流
	std::list<CServerRTPSession*>::iterator iter = m_listSession.begin();
	for (; iter != m_listSession.end(); ++iter)
	{
		if ((*iter)->GetSessionID() == pMediaInfo->SessionID)
		{
			printf("Already requested this session\n");
			m_Lock.UnLock();
			return -1;
		}
		if (pMediaInfo->StreamType == 1 || pMediaInfo->StreamType == 2)
		{
			if ((*iter)->GetChannel() - MAX_CH_NUM == pMediaInfo->Channel)
			{
				printf("Already requested this channel\n");
				m_Lock.UnLock();
				return -1;
			}
		}
		else
		{
			if ((*iter)->GetChannel() == pMediaInfo->Channel)
			{
				printf("Already requested this channel\n");
				m_Lock.UnLock();
				return -1;
			}
		}
	}


	if (pMediaInfo->StreamType == 1 || pMediaInfo->StreamType == 2)
	{
		m_StarSpeedControl[pMediaInfo->Channel] = 2;//test
		m_bIsPaused[pMediaInfo->Channel] = false;
		m_bIsPackedH264[pMediaInfo->Channel + MAX_CH_NUM] = false;
		m_Scale[pMediaInfo->Channel] = pMediaInfo->PlaybackScale;
		printf("_________Play Back_______Scale : %d  clientID : %lu  channel:%d\n", \
			pMediaInfo->PlaybackScale, pMediaInfo->ClientID, pMediaInfo->Channel);
	}
	else
	{
		m_bIsPackedH264[pMediaInfo->Channel] = false;
	}

	m_Lock.UnLock();
#ifdef TEST_JRTP
	m_Lock.Lock();
	int ret;
	unsigned long destip, destport;
	//unsigned long srcip = htonl((unsigned long)192<<24 | (unsigned long)168<<16 | (unsigned long)3<<8 | (unsigned long)202);
	std::string ipstr = "192.168.3.201";
	destip = inet_addr(ipstr.c_str());
	destip = ntohl(destip);
	jrtplib::RTPUDPv4TransmissionParams transparams;  
	jrtplib::RTPSessionParams sessparams; 

	sessparams.SetOwnTimestampUnit(1.0/90000.0);
	transparams.SetPortbase(8000); 
	printf("protocal:%d\n", transparams.GetTransmissionProtocol());
	ret = m_RTPSession.Create(sessparams, &transparams);
	printf("m_RTPSession.Create ret : %d\n", ret);
	if (ret < 0)
	{
		m_Lock.UnLock();
		return 0;
	}
	jrtplib::RTPIPv4Address addr(destip, 6000);
	m_RTPSession.AddDestination(addr);
	m_RTPSession.SetDefaultTimestampIncrement(3600);
	m_RTPSession.SetDefaultPayloadType(98);
	m_RTPSession.SetDefaultMark(false);
	m_RTPSession.SetMaximumPacketSize(1500);
	m_Lock.UnLock();
#endif

//test
// 	pMediaInfo->Channel = 1;
//    	pMediaInfo->LocalRTPPort = 7000;
//    	pMediaInfo->LocalRTCPPort = 7001;
//    	pMediaInfo->MediaType = 98;
//    	pMediaInfo->RemoteIp = htonl((unsigned long)192<<24 | (unsigned long)168<<16 | (unsigned long)3<<8 | (unsigned long)201);
//    	pMediaInfo->RemoteRTPPort = 6000;
//    	pMediaInfo->RemoteRTCPPort = 6001;

	CServerRTPSession *pRTPSession = new CServerRTPSession(pMediaInfo);
	if (NULL == pRTPSession)
	{
		return -1;
	}
	AddSession(pRTPSession, SWL_INVALID_SOCKET, pRTPSession->GetRTPSocketFd());	

	printf("Start RTPSession End\n");
	return 0;
}

int CServerRTPMan::StopTransRTP(int SessionID)
{
	printf("StopTransRTP begin\n");
	unsigned short ch=255;
	m_Lock.Lock();
	std::list<CServerRTPSession*>::iterator iter = m_listSession.begin();
	for (; iter!=m_listSession.end(); ++iter)
	{
		if (SessionID == (*iter)->GetSessionID())
		{
			ch = (*iter)->GetChannel();
			DeleteSession(*iter);
			iter = m_listSession.erase(iter);
			break;
		}
	}

	if (ch != 255)
	{
  		for (int j=0; j<MAX_RTSP_MEDIA_COUNT; j++)
  		{
  			m_LastFrameDateTime[j][ch].seconds = 0;
  			m_LastFrameDateTime[j][ch].microsecond = 0;
			m_LastFrameTs[j][ch] = 0;
  		}
		m_bIsPackedH264[ch] = false;

 		m_bRequestedThisChannel[ch] = false;
		if (ch >= MAX_CH_NUM)
		{
			printf("StopTransRTP PlayBack channel : %d\n", ch);
			ch -= MAX_CH_NUM;
		}
		m_Scale[ch] = 0;
		m_LastFrameTime[ch] = 0;
		m_ThisFrameTime[ch] = 0;
		m_LastMsgTime[ch].seconds = 0;
		m_LastMsgTime[ch].microsecond = 0;
		m_FrameCountPerMsg[ch] = 0;
		m_LastFrameIndex[ch] = 0;
		m_StarSpeedControl[ch] = 0;
		m_bIsPaused[ch] = false;
	}
	m_SendTimeOutNum = 0;
	m_Lock.UnLock();

	printf("StopTransRTP end\n");
	return 0;
}

void CServerRTPMan::ReleasePacket(unsigned char channel)
{
	//m_Lock.Lock();
	CRTPPacket *pRTPPacket = NULL;

	if (m_mapRTPPacket.find(channel) != m_mapRTPPacket.end())
	{
		int listSize = m_mapRTPPacket[channel]->size();
		for (int j=0; j<listSize; ++j)
		{
			pRTPPacket = m_mapRTPPacket[channel]->front();
			if (pRTPPacket->GetCount() != 0)
			{
				break;
			}
			m_mapRTPPacket[channel]->pop_front();
			m_mapRTPPacketFree[channel]->push_back(pRTPPacket);
		}
	}
	
	//m_Lock.UnLock();
}

int CServerRTPMan::SundaySearch(char *pSrc, char *pPattern, unsigned long PatternLen, unsigned long SrcLen)
{
	//Sunday算法
	int i=0,j=0;//分别为目标串和模式串当前匹配的位置
	unsigned long pe = PatternLen - 1;//目标串后定位

	while(i<SrcLen && j<PatternLen)
	{
		if (pSrc[i] == pPattern[j])
		{
			i++;
			j++;
		}
		else//计算NEXT值
		{
			int k = PatternLen - 1;
			while(k>=0 && pSrc[pe + 1] != pPattern[k])//查找模式串中是否有pe
			{
				k--;
			}
			int gap = PatternLen - k;
			i += gap;
			pe = i + PatternLen - 1;
			j = 0;
		}
	}
	if (j == PatternLen)
	{
		return i;
	}

	return -1;

}

int CServerRTPMan::ChangePlaybackScale(int SessionID, int PlaybackScale)
{
	m_Lock.Lock();
	std::list<CServerRTPSession*>::iterator iter = m_listSession.begin();
	for (; iter!=m_listSession.end(); ++iter)
	{
		if (SessionID == (*iter)->GetSessionID())
		{
			if ((*iter)->IsPlayBack())
			{
				printf("ChangeScale : %d\n", PlaybackScale);

 				if (PlaybackScale == 0)
 				{
 					m_bIsPaused[(*iter)->GetChannel() - MAX_CH_NUM] = false;
 				}
				else
				{
					(*iter)->SetPause(false);
// 					if ((m_Scale[(*iter)->GetChannel() - MAX_CH_NUM] > 1) &&  (PlaybackScale == 1))
// 					{
// 						m_StarSpeedControl[(*iter)->GetChannel() - MAX_CH_NUM] = 3;
// 					}
					if ((m_Scale[(*iter)->GetChannel() - MAX_CH_NUM] < 0) && (PlaybackScale == 1))
					{
						m_StarSpeedControl[(*iter)->GetChannel() - MAX_CH_NUM] = 3;
					}
					if ((m_Scale[(*iter)->GetChannel() - MAX_CH_NUM] == 0) && (PlaybackScale != 0))
					{
						m_StarSpeedControl[(*iter)->GetChannel() - MAX_CH_NUM] = 5;
					}
				}
				m_Scale[(*iter)->GetChannel() - MAX_CH_NUM] = PlaybackScale;
				(*iter)->SetScale(PlaybackScale);
			}
			break;
		}
	}

	m_Lock.UnLock();

	return 0;
}

bool CServerRTPMan::GetSessionInfo(unsigned short channel, unsigned long &ClientID, int &StreamID)
{
	bool ret = false;

	if (channel < MAX_CH_NUM)
	{
		return ret;
	}

	//m_Lock.Lock();
	std::list<CServerRTPSession*>::iterator iter = m_listSession.begin();
	for (; iter!=m_listSession.end(); ++iter)
	{
		if (channel == (*iter)->GetChannel())
		{
			ClientID = (*iter)->GetClientID();
			StreamID = (*iter)->GetMessageID();
			ret = true;
			break;
		}
	}
	//m_Lock.UnLock();
	return ret;
}


void CServerRTPMan::PlayBackProc()
{

	CMessageMan *pMessageMan = CMessageMan::Instance();

	int i = 0;
	RTSP_DATE_TIME CurTime;
	memset(&CurTime, 0, sizeof(RTSP_DATE_TIME));
	unsigned long TimeCompare = 0;
	unsigned long TimeDiff = 0;
	unsigned long FrameTimeDiff = 0;
	unsigned long FrameTimeDiffSec = 0;
	//unsigned long FrameTimeDiffMicS = 0;
	unsigned long index = 0;
	unsigned long sleep_count = 0;
	unsigned long ClientID = 0;
	int StreamID = 0;

	while(m_bPlayBackProc)
	{
 		m_Lock.Lock();
 		for (i=0; i<MAX_CH_NUM; i++)
 		{
 			if (m_LastFrameTime[i] != 0)//若通道不被请求，则该值为0 
 			{
  				if (m_StarSpeedControl[i] != 0)//刚启动的时候控制发快一点
  				{
  					m_StarSpeedControl[i]--;
  					m_FrameCountPerMsg[i] = 0;
  					if (!GetSessionInfo(i + MAX_CH_NUM, ClientID, StreamID))
  					{
  						printf("GetSessionInfo Err\n");
  					}
					printf("Speed Control : index : %lu\n", m_LastFrameIndex[i]);
  					PutMsgToServer(ClientID, StreamID, m_LastFrameIndex[i]);
  					m_LastMsgTime[i] = RTSPGetCurrTime();
					m_Lock.UnLock();
					PUB_Sleep(50);
  					continue;
  				}

 				CurTime = RTSPGetCurrTime();

 				TimeDiff = (CurTime.seconds - m_LastMsgTime[i].seconds) * 1000000 + \
 					(CurTime.microsecond - m_LastMsgTime[i].microsecond);

				FrameTimeDiffSec = m_ThisFrameTime[i] / 1000000 - m_LastFrameTime[i] / 1000000;
  				if (FrameTimeDiffSec == 0)
  				{
  					FrameTimeDiff = (m_ThisFrameTime[i] % 1000000 - m_LastFrameTime[i] % 1000000);
  				}
  				else if(FrameTimeDiffSec == 1)
  				{
  					FrameTimeDiff = (m_ThisFrameTime[i] % 1000000 + 1000000 - m_LastFrameTime[i] % 1000000);
  				}
  				else
  				{
  					//assert(false);
					printf("FrameTimeDiff err: %lu !!!\n", FrameTimeDiff);
					FrameTimeDiff = 40000;
  				}
						
 				//帧率*倍速*Put消息的时间差 > 已发包数   (应发的包数 大于 已发的包数)
 				if (m_FrameCountPerMsg[i] > 1)
 				{
					if (FrameTimeDiff == 0)
					{
						printf("FrameTimeDiff == 0\n");
						FrameTimeDiff = 40000;
					}
					if (m_Scale[i] >= 0)
					{
						if ((m_bIsPaused[i] == false) && m_Scale[i] == 0)
						{
							TimeCompare = (unsigned long)(TimeDiff) / FrameTimeDiff;
						}
						else
						{
							TimeCompare = (unsigned long)(m_Scale[i] * TimeDiff) / FrameTimeDiff;
						}
					}
					else
					{
						TimeCompare =  (unsigned long)(TimeDiff / (abs (m_Scale[i]))) / FrameTimeDiff;
					}
	
 					if (TimeCompare >= m_FrameCountPerMsg[i] - 1)
 					{
  						//printf("result: %lu  FrameTimeDiff %lu\n", TimeCompare, FrameTimeDiff);

 						m_FrameCountPerMsg[i] = 0;

						if (!GetSessionInfo(i + MAX_CH_NUM, ClientID, StreamID))
						{
							printf("GetSessionInfo Err : channel : %D  ClientID %lu  StreamID %d\n", i + MAX_CH_NUM, ClientID, StreamID);
						}

// 						printf("fScale %d  TimeDiff %lu   FrameCountPerMsg %lu\n", \
// 							m_Scale[i], TimeDiff, m_FrameCountPerMsg[i]);
 						PutMsgToServer(ClientID, StreamID, m_LastFrameIndex[i]);
 						//printf("PutMsgToServer : Index : %lu  ClientID %lu   StreamID %d\n", m_LastFrameIndex[i], ClientID, StreamID);

						m_LastMsgTime[i] = CurTime;
 					}
 				}
 			}
 		}
//  		sleep_count++;
//  		if (sleep_count % 1000 == 0)
//  		{
//  
//   			printf("PlayBackProc still alive m_LastFrameTime[0] %llu\n", m_LastFrameTime[0]);
//   			printf("result: %lu  FrameTimeDiff %lu\n", TimeCompare, FrameTimeDiff);
//    			printf("m_LastFrameTime %llu fScale %d  FrameTimeDiff %lu  TimeDiff %lu   FrameCountPerMsg %lu TimeCompare %lu\n", \
//    				m_LastFrameTime[0], m_Scale[0], FrameTimeDiff, TimeDiff, m_FrameCountPerMsg[0], TimeCompare);
//  		
//  		}

		m_Lock.UnLock();
		PUB_Sleep(1);
	}
}

void CServerRTPMan::dump(const void *p, unsigned long Len)
{
	printf("Dump:");
	const char *Pos = (const char *)p;
	for (int i=0; i<Len; ++i)
	{
		if (i % 16 == 0)
		{
			printf("\n");
		}
		printf("%2x ", *Pos);
		Pos++;
	}
	printf("\n");
}

int CServerRTPMan::PutMsgToServer(unsigned long ClientID, int SteamID, unsigned long DataType)
{
	PACKCMD				*PackCmd;
	NET_DATA_SEARCH		*NetDataSerch;
	MESSAGE_DATA		MsgData;

	//回放控制
	MsgData.clientID = ClientID;
	MsgData.deviceID = 0;
	MsgData.dataLen = sizeof(PACKCMD) + sizeof(NET_DATA_SEARCH);
	MsgData.pData = new char[MsgData.dataLen];
	assert(MsgData.pData != NULL);
	memset(MsgData.pData, 0, MsgData.dataLen);

	PackCmd = (PACKCMD *)MsgData.pData;
	NetDataSerch = reinterpret_cast<NET_DATA_SEARCH *>(MsgData.pData + sizeof(PACKCMD));

	//回放请求
	PackCmd->cmdType = CMD_REQUEST_DATA_INDEX;
	PackCmd->cmdID   = 0;
	PackCmd->cmdVer = NET_PROTOCOL_VER;
	PackCmd->dataLen = sizeof(NET_DATA_SEARCH);
	memset(NetDataSerch, 0, sizeof(NET_DATA_SEARCH));
	NetDataSerch->dataType = DataType;
	NetDataSerch->streamID = SteamID;

	CMessageMan::Instance()->PutMsgToServer(MsgData, GB28181_NET_SERVER);

	//dump(MsgData.pData, sizeof(PACKCMD) + sizeof(NET_DATA_SEARCH));

	return 0;
}
