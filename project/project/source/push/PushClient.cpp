#include "PushClient.h"
#ifndef WIN32
#include "NetInterface.h"
#endif

CPushClient::CPushClient()
{
	m_hPushClientID = PUB_THREAD_ID_NOINIT;
	m_bPushClientProc = false;

	memset(&m_providerInfo, 0, sizeof(PROVIDER_TYPE_INFO));
	SetClientStatus(PUSH_CLIENT_CLOSEED);
	m_ltdTime = GetCurrTime32();

	m_sock = SWL_INVALID_SOCKET;

	//接收
	memset(&m_msgHeader, 0, sizeof(PUSH_MSG_HEADER));
	memset(m_recvBuf, 0, PUSH_CLIENT_BUF_LEN);
	m_pRecvMsgInfo = (PUSH_MSG_INFO *)m_recvBuf;
	m_pPDR	= (PUSH_DATA_REPLY *)(m_recvBuf + sizeof(PUSH_MSG_INFO));
	m_recvBufLen = PUSH_CLIENT_BUF_LEN;
	m_recvPos = 0;
	m_bNeedRep = false;

	//发送
	memset(m_sendBuf, 0, PUSH_CLIENT_BUF_LEN);
	m_pSendMsgInfo = (PUSH_MSG_INFO *)(m_sendBuf + sizeof(PUSH_MSG_HEADER));
	m_sendBufLen = PUSH_CLIENT_BUF_LEN;
	m_sendPos = 0;
	m_needSendLen = 0;

	snprintf(m_szServerIP, sizeof(m_szServerIP), "192.168.66.193");
	m_serverPot = 6036;

	m_bGetPushServerInfo = false;
}

CPushClient::~CPushClient()
{

}

bool CPushClient::Initial(const char * pProviderId, unsigned long pidLen, const char * pdvrIP/*=""*/, unsigned short webPort, const char * pCostomID/*=NULL*/, unsigned long ulMaxBufMsg)
{
	strncpy(m_providerInfo.providerId, pProviderId, MAX_PROVIDER_ID_LEN);
	strncpy(m_providerInfo.serverIP, pdvrIP, MAX_IP_LEN);
	m_providerInfo.serverPort = webPort;
	m_maxBufMsg = ulMaxBufMsg;
	
	if (NULL != pCostomID)
	{
		snprintf((char *)m_providerInfo.companyInfo, sizeof(m_providerInfo.companyInfo), "%s", pCostomID);
	}

	return true;
}

void CPushClient::Quit()
{
	ClearPushMsg();

	ClearDTK();
}

bool CPushClient::Start()
{
	m_hPushClientID = PUB_CreateThread(PushClientThread, ((void*)this), &m_bPushClientProc);
	if( PUB_CREATE_THREAD_FAIL == m_hPushClientID )
	{
		printf("%s:%s:%d, start push client thread error\n", __FUNCTION__, __FILE__, __LINE__);
		return false;
	}

	return true;
}

void CPushClient::Stop()
{
	PUB_ExitThread(&m_hPushClientID, &m_bPushClientProc);
}

bool CPushClient::PushData(const unsigned char * pData, unsigned long dataLen)
{
	int msgLen = dataLen;
	char * pMsg = new char [msgLen];
	memcpy(pMsg, pData, dataLen);

	PUSH_MSG_HEADER * pMsgHeader = (PUSH_MSG_HEADER *)pMsg;
	
	m_pushMsgListLock.Lock();
	if (m_maxBufMsg < m_pushMsgList.size())
	{
		ClearPushMsg();
	}
	m_pushMsgList.push_back(pMsgHeader);
	m_pushMsgListLock.UnLock();

	return true;
}

bool CPushClient::AddDeviceTokenInfo(const DEVICE_TOKEN_INFO_PNTS * pDTKIPnts)
{
	DEVICE_TOKEN_INFO_PNTS * pDTKIPntsTemp = new DEVICE_TOKEN_INFO_PNTS;
	* pDTKIPntsTemp = * pDTKIPnts;
	memcpy(pDTKIPntsTemp->providerId, m_providerInfo.providerId, sizeof(m_providerInfo.providerId));

	m_dtkListLock.Lock();
	std::list<DEVICE_TOKEN_INFO_PNTS *>::iterator iter;
	for (iter=m_dtkList.begin(); iter != m_dtkList.end(); iter++)
	{
		//找到一个相同的token
		if (0 == memcmp((*iter)->deviceTokenInfo.token, pDTKIPntsTemp->deviceTokenInfo.token, (*iter)->deviceTokenInfo.tokenLen))
		{
			delete * iter;
			m_dtkList.erase(iter);
			break;
		}
	}
	m_dtkList.push_back(pDTKIPntsTemp);
	m_dtkListLock.UnLock();

	return true;
}

void CPushClient::ChangeIP(const char * pdvrIP)
{
	strcpy(m_providerInfo.serverIP, pdvrIP);

	UpdateProviderInfo();
}

void CPushClient::ChangePort(unsigned short port)
{
	if (m_providerInfo.serverPort != port)
	{
		m_providerInfo.serverPort = port;

		UpdateProviderInfo();
	}
}

void CPushClient::ChangeServerInfo(const char * pServerIP, unsigned short serverPort)
{
	snprintf(m_szServerIP, sizeof(m_szServerIP), "%s", pServerIP);
	m_serverPot = serverPort;
}

void CPushClient::PushMsg(PUSH_MSG_TYPE msgType, const unsigned char * pMsgData, unsigned long dataLen)
{
	int msgLen = sizeof(PUSH_MSG_INFO) + sizeof(PUSH_MSG_HEADER) + dataLen;
	char * pMsg = new char [msgLen];

	memset(pMsg, 0, msgLen);

	PUSH_MSG_HEADER * pHeader = (PUSH_MSG_HEADER *) pMsg;
	memcpy(pHeader->startFlag, "1111", strlen("1111"));
	pHeader->dataLen = msgLen - sizeof(PUSH_MSG_HEADER);

	PUSH_MSG_INFO * pMsgInfo =  (PUSH_MSG_INFO *)(pMsg + sizeof(PUSH_MSG_HEADER));
	pMsgInfo->isRequest = 1;
	pMsgInfo->msgType = msgType;
	pMsgInfo->msgLen = msgLen - sizeof(PUSH_MSG_HEADER) - sizeof(PUSH_MSG_INFO);

	if (0 < dataLen)
	{
		char * pData = pMsg + sizeof(PUSH_MSG_HEADER) + sizeof(PUSH_MSG_INFO);
		memcpy(pData, pMsgData, dataLen);
	}

	m_pushMsgListLock.Lock();
	if (m_maxBufMsg < m_pushMsgList.size())
	{
		ClearPushMsg();
	}
	m_pushMsgList.push_front(pHeader);
	m_pushMsgListLock.UnLock();
}


int CPushClient::DataProc()
{
	if (IsClosed())
	{
		return 0;
	}

	fd_set			readSet;
	fd_set			writeSet;

	FD_ZERO(&readSet);
	FD_SET(m_sock, &readSet);

	FD_ZERO(&writeSet);
	FD_SET(m_sock, &writeSet);

	struct timeval timeOut = {5, 0};

	int retVal = select(m_sock + 1, &readSet, &writeSet, NULL, &timeOut);

	if (0 < retVal)
	{
		//先接收数据
		if (FD_ISSET(m_sock, &readSet))
		{
			retVal = RecvData() ;
			if (0 > retVal)
			{
				SetClientStatus(PUSH_CLIENT_ERROR);
			}
			else if (0 < retVal)
			{
			}
			else
			{}
		}

		//再发送消息
		if (FD_ISSET(m_sock, &writeSet))
		{
			retVal = SendData();
			if (0 > retVal)
			{
				SetClientStatus(PUSH_CLIENT_ERROR);
			}
			else if (0 < retVal)
			{
			}
			else
			{}
		}
	}
	else if (0 > retVal)
	{
		SetClientStatus(PUSH_CLIENT_ERROR);
	}
	else
	{}

	return 0;
}

int CPushClient::RecvData()
{
	int retVal = 0;

	if (0 == m_msgHeader.dataLen)
	{
		//接收消息头
		retVal = RecvData((const unsigned char *)(&m_msgHeader.startFlag), sizeof(m_msgHeader.startFlag));
		if (0 > retVal)
		{
			memset(&m_msgHeader, 0, sizeof(PUSH_MSG_HEADER));
			return retVal;
		}

		if ((sizeof(m_msgHeader.startFlag) != retVal) || (0 !=  memcmp(m_msgHeader.startFlag, "1111", strlen("1111"))))
		{
			memset(&m_msgHeader, 0, sizeof(PUSH_MSG_HEADER));
			return 0;
		}

		//接收消息体的长度
		retVal = RecvData((unsigned char *)&m_msgHeader.dataLen, sizeof(m_msgHeader.dataLen));
		
		if (0 > retVal)
		{
			memset(&m_msgHeader, 0, sizeof(PUSH_MSG_HEADER));
			return 0;
		}

		if (retVal != sizeof(m_msgHeader.dataLen))
		{
			memset(&m_msgHeader, 0, sizeof(PUSH_MSG_HEADER));
			return 0;
		}
	}

	if ((0 == m_recvPos) && (m_msgHeader.dataLen > m_recvBufLen))
	{
		assert(false);
		return -1;
	}

	if (m_recvPos < m_msgHeader.dataLen)
	{
		retVal = RecvData(m_recvBuf + m_recvPos, m_msgHeader.dataLen - m_recvPos);

		if (0 > retVal)
		{
			memset(&m_msgHeader, 0, sizeof(PUSH_MSG_HEADER));
			m_recvPos = 0;

			return retVal;
		}
		else
		{
			m_recvPos += retVal;
		}
	}


	//收到一个完整消息
	if ((0 < m_recvPos) && (m_recvPos == m_msgHeader.dataLen))
	{
		AnalysisRecvMsg();
		memset(&m_msgHeader, 0, sizeof(PUSH_MSG_HEADER));
		m_recvPos = 0;
	}
	else
	{}

	return 0;
}

int CPushClient::RecvData(const unsigned char * buf, int needRecLen)
{
	int recvLen = 0;

	while (recvLen < needRecLen)
	{
		int retVal = SWL_Recv(m_sock, (void *)(buf + recvLen), needRecLen - recvLen, 0);
		if (0 >= retVal)
		{
			if (SWL_EWOULDBLOCK())
			{
				continue;
			}
			else
			{
				return -1;
			}
		}
		else
		{
			recvLen += retVal;
		}
	}

	return recvLen;
}

int CPushClient::AnalysisRecvMsg()
{
	m_bNeedRep = false;

	assert(0 == m_pRecvMsgInfo->isRequest);

	if (PUSH_FAIL == * m_pPDR)
	{
		//printf("%s:%s:%d, 发送数据异常, 要关闭服务器\n", __FUNCTION__, __FILE__, __LINE__);
		SetClientStatus(PUSH_CLIENT_ERROR);
	}
	else
	{
		//printf("%s:%s:%d, 发送数据成功可以继续发送\n", __FUNCTION__, __FILE__, __LINE__);
	}

	if (PUSH_CMD_PROVIDER_INFO == m_pRecvMsgInfo->msgType)
	{
		//printf("%s:%s:%d, 收到设备信息的应答\n", __FUNCTION__, __FILE__, __LINE__);
		SetClientStatus(PUSH_CLIENT_CONNECTED);
	}
	else if (PUSH_CMD_PUSH_MSG == m_pRecvMsgInfo->msgType)
	{
		//printf("%s:%s:%d, 收到推送消息的应答\n", __FUNCTION__, __FILE__, __LINE__);
	}
	else
	{

	}

	return 0;
}

int CPushClient::SendData()
{
	int retVal = 0;

// 	if (m_bNeedRep)
// 	{
// 		return retVal;
// 	}

	if ((0 == m_needSendLen) || (m_needSendLen == m_sendPos))
	{
		m_needSendLen = 0;
		m_sendPos = 0;
		//
		m_pushMsgListLock.Lock();
		if (!m_pushMsgList.empty())
		{
			PUSH_MSG_HEADER * pushMsgHeader = m_pushMsgList.front();
			m_pushMsgList.pop_front();

			if ((sizeof(PUSH_MSG_HEADER) + pushMsgHeader->dataLen) <= PUSH_CLIENT_BUF_LEN)
			{
				memcpy(m_sendBuf, pushMsgHeader, sizeof(PUSH_MSG_HEADER) + pushMsgHeader->dataLen);
				m_needSendLen = sizeof(PUSH_MSG_HEADER) + pushMsgHeader->dataLen;

				if (0 != memcmp(pushMsgHeader->startFlag, "1111", 4))
				{
					assert(false);
				}

				if (PUSH_CMD_PUSH_MSG == m_pSendMsgInfo->msgType)
				{
					PROVIDER_PUSH_MSG * pMsg = (PROVIDER_PUSH_MSG *) (m_sendBuf + sizeof(PUSH_MSG_HEADER) + sizeof(PUSH_MSG_INFO));
					if (pushMsgHeader->dataLen != sizeof(PUSH_MSG_INFO) + sizeof(PROVIDER_PUSH_MSG) + pMsg->paramLen)
					{
						assert(false);
					}
				}
				else if (PUSH_CMD_PROVIDER_INFO == m_pSendMsgInfo->msgType)
				{
					if (pushMsgHeader->dataLen != sizeof(PROVIDER_TYPE_INFO) + sizeof(PUSH_MSG_INFO))
					{
						assert(false);
					}
				}
				else if (PUSH_DEVICE_TOKEN_INFO == m_pSendMsgInfo->msgType)
				{
					if (pushMsgHeader->dataLen != sizeof(DEVICE_TOKEN_INFO_PNTS) + sizeof(PUSH_MSG_INFO))
					{
						assert(false);
					}
				}
				else
				{
					if (0 != pushMsgHeader->dataLen)
					{
						assert(false);
					}
				}
			}
			else
			{
				assert(false);
			}
			delete pushMsgHeader;
		}
		m_pushMsgListLock.UnLock();
	}

	if (m_sendPos < m_needSendLen)
	{
		retVal = SendData(m_sendBuf + m_sendPos, m_needSendLen - m_sendPos);
		if (0 > retVal)
		{
			return retVal;
		}
		else
		{
			m_sendPos += retVal;
		}
	}

	if ((0 < m_needSendLen) && (m_sendPos == m_needSendLen))
	{
		SendDataOver();
	}

	return retVal;
}

int CPushClient::SendData(const unsigned char * buf, int needSendLen)
{
	int sendLen = 0;

	while (sendLen < needSendLen)
	{
		int retVal = SWL_Send(m_sock, buf + sendLen, needSendLen - sendLen, 0);
		if (0 >= retVal)
		{
			if (SWL_EWOULDBLOCK())
			{
				continue;
			}
			else
			{
				return -1;
			}
		}
		else
		{
			sendLen += retVal;
		}
	}

	return sendLen;
}

int CPushClient::SendDataOver()
{
	if (PUSH_CMD_REQUEST_PROVIDER_INFO == m_pRecvMsgInfo->msgType)
	{
		//printf("%s:%s:%d, 收到设备信息请求\n", __FUNCTION__, __FILE__, __LINE__);
	}
	else if (PUSH_CMD_PROVIDER_INFO == m_pRecvMsgInfo->msgType)
	{
		//printf("%s:%s:%d, 发送设备信息成功\n", __FUNCTION__, __FILE__, __LINE__);
	}
	else if (PUSH_CMD_PUSH_MSG == m_pRecvMsgInfo->msgType)
	{
		//printf("%s:%s:%d, 发送推送信息成功\n", __FUNCTION__, __FILE__, __LINE__);
	}
	else if (PUSH_CMD_PUSH_MSG == m_pRecvMsgInfo->msgType)
	{
		//printf("%s:%s:%d, 发送终端token成功\n", __FUNCTION__, __FILE__, __LINE__);
	}
	else
	{
	}

	m_ltdTime = GetCurrTime32();

	m_needSendLen = 0;
	m_sendPos = 0;
	m_bNeedRep = true;

	return 0;
}

bool CPushClient::UpdateProviderInfo()
{
	if (IsConnectting() || IsConnected())
	{
#ifndef WIN32
		unsigned long ip = 0;
		CNetInterface::Instance()->GetIPAddress(ip);
		snprintf(m_providerInfo.serverIP, sizeof(m_providerInfo.serverIP), "%d.%d.%d.%d", ip & 0xff, (ip >> 8) & 0xff, (ip >> 16) & 0xff, (ip >> 24) & 0xff);
#endif
		PushMsg(PUSH_CMD_PROVIDER_INFO, (unsigned char *)&m_providerInfo, sizeof(PROVIDER_TYPE_INFO));
		return true;
	}
	else
	{
		return false;
	}
}

bool CPushClient::UpdateToken()
{
	if (IsConnected())
	{
		m_dtkListLock.Lock();

		std::list<DEVICE_TOKEN_INFO_PNTS *>::iterator iter;
		for (iter = m_dtkList.begin(); iter != m_dtkList.end();)
		{
			PushMsg(PUSH_DEVICE_TOKEN_INFO, (unsigned char *)(*iter), sizeof(DEVICE_TOKEN_INFO_PNTS));

			delete *iter;
			iter = m_dtkList.erase(iter);
		}
		m_dtkListLock.UnLock();

		return true;
	}
	else
	{
		return true;
	}
}

bool CPushClient::CheckStatus()
{
	switch (GetClientStatus())
	{
	case PUSH_CLIENT_CLOSEED:
		break;
	case PUSH_CLIENT_CONNECTTING:
	case PUSH_CLIENT_CONNECTED:
		{
			unsigned long curTime = GetCurrTime32();
			unsigned long det = curTime - m_ltdTime;

			if ((curTime < m_ltdTime) || (60 * 10 < (curTime - m_ltdTime)))
			{
				m_ltdTime = curTime;
				break;
			}
			else if (PUSH_CLIENT_IDLE_TIME < curTime - m_ltdTime)
			{
				SetClientStatus(PUSH_CLIENT_ERROR);
			}
			else
			{
				break;
			}
		}
	case PUSH_CLIENT_ERROR:
		{
			if (SWL_INVALID_SOCKET != m_sock)
			{
				SWL_CloseSocket(m_sock);
				m_sock = SWL_INVALID_SOCKET;
			}
			SetClientStatus(PUSH_CLIENT_CLOSEED);
		}
		break;
	}

	return true;
}

bool CPushClient::ConnectPtns()
{
	if (IsClosed())
	{
		bool bHaveNewDTK = false;
		m_dtkListLock.Lock();
		if (!m_dtkList.empty())
		{
			bHaveNewDTK = true;
		}
		m_dtkListLock.UnLock();

		bool bHaveMsg = false;
		m_pushMsgListLock.Lock();
		if (!m_pushMsgList.empty())
		{
			bHaveMsg = true;
		}
		m_pushMsgListLock.UnLock();
		
		if (bHaveNewDTK || bHaveMsg)
		{
			struct in_addr serverIp;
			serverIp.s_addr = inet_addr(m_szServerIP);
			if (INADDR_NONE == serverIp.s_addr)
			{
#if USE_LOCAL_DNSRESOLVE
				struct hostent *pHost  = CNetInterface::Instance()->GetHostByName( m_szServerIP );
#else
				struct hostent *pHost  = gethostbyname( m_szServerIP );	 
#endif
				if (NULL != pHost)
				{
					serverIp = *(struct in_addr *)pHost->h_addr_list[0];
#if USE_LOCAL_DNSRESOLVE
					CNetInterface::Instance()->ReleaseHost(pHost);
					pHost = NULL;
#endif
				}
				else
				{
					return false;
				}
			}
			
			char szSIP[40] = {0};
			struct in_addr in;
			in = serverIp;

#if defined(WIN32) || defined(__CHIP3515__) || defined(__CHIP3520__)
			char * pSip = inet_ntoa(in);
#else
#if defined(__X_LINUX__)
			char * pSip = inet_ntoa(in);
#else
			char * pSip = inet_ntoa_r(in, szSIP);
#endif
#endif

			if (NULL == pSip)
			{
				return false;
			}

			if (SWL_INVALID_SOCKET != m_sock)
			{
				SWL_CloseSocket(m_sock);
				m_sock = SWL_INVALID_SOCKET;
				assert(false);
			}

			m_sock = CreateSock(0, m_serverPot, serverIp.s_addr);
			if (SWL_INVALID_SOCKET != m_sock)
			{
				m_ltdTime = GetCurrTime32();

				SetClientStatus(PUSH_CLIENT_CONNECTTING);

				if (UpdateProviderInfo())
				{
					return true;
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
		}
	}

	return true;
}

SWL_socket_t CPushClient::CreateSock(unsigned short localPort, unsigned short remotePort, unsigned long remoteInAddr)
{
	SWL_socket_t sockFd = SWL_CreateSocket(AF_INET, SOCK_STREAM, 0);
	if (SWL_INVALID_SOCKET == sockFd)
	{
		assert(false);
		return sockFd;
	}

	if (0 != localPort)
	{
		struct sockaddr_in sockAddr;
		memset(&sockAddr, 0, sizeof(struct sockaddr_in));
		sockAddr.sin_addr.s_addr = htonl(INADDR_ANY);
		sockAddr.sin_family = AF_INET;
		sockAddr.sin_port = htons(localPort);
		SWL_Bind(sockFd, (const struct sockaddr *)&sockAddr, sizeof(struct sockaddr_in));
	}

	struct sockaddr_in	remoteSockAddr;
	memset(&remoteSockAddr, 0, sizeof(struct sockaddr_in));
	remoteSockAddr.sin_port = htons(remotePort);
	remoteSockAddr.sin_family	= AF_INET;
	remoteSockAddr.sin_addr.s_addr = remoteInAddr;

	if (0 != SWL_Connect(sockFd, (const struct sockaddr *)&remoteSockAddr, sizeof(struct sockaddr_in), 1000))
	{
		SWL_CloseSocket(sockFd);
		sockFd = SWL_INVALID_SOCKET;
	}

	return sockFd;
}


PUB_THREAD_RESULT PUB_THREAD_CALL CPushClient::PushClientThread(void *pParam)
{
#ifndef WIN32
	printf("%s %d pid = %d, tid = %d,  CPushClient::PushClientThread\n",__FILE__, __LINE__, getpid(), syscall(__NR_gettid));
#endif

	CPushClient *pObj = reinterpret_cast<CPushClient *>(pParam);

	pObj->PushClientProc();

	return 0;
}

void CPushClient::PushClientProc()
{
	while (m_bPushClientProc)
	{
		if(!m_bGetPushServerInfo)
		{
			PUB_Sleep(1000);
			continue;
		}
		ConnectPtns();

		DataProc();

		UpdateToken();

		CheckStatus();

		PUB_Sleep(100);
	}
}
