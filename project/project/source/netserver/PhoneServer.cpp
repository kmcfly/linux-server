
#include "PhoneServer.h"

CPhoneSync::CPhoneSync()
{
	m_pMsg = NULL;
}

CPhoneSync::~CPhoneSync()
{

}

CPhoneSync* CPhoneSync::Instance()
{
	static CPhoneSync s_PhoneSynServer;
	return &s_PhoneSynServer;
}


int CPhoneSync::Init(unsigned long productType)
{
	m_productType = productType;

	m_phoneServerIp   = 0;
	m_phoneServerPort = 0;

	m_dvrHttpPort = 0;
	m_dvrServerPort  = 0;
	memset(m_szMac, 0, sizeof(m_szMac));
	memset(m_szUsrPassword, 0, sizeof(m_szUsrPassword));

	memset(&m_msgHead, 0, sizeof(m_msgHead));
	m_msgHead.msg_type = PHONE_MSG_HEARTBEAT;
	m_msgHead.msg_subtype = 0;
	m_msgHead.msg_size = sizeof(m_sendRegInfo);
	memset(&m_sendRegInfo, 0, sizeof(m_sendRegInfo));	
	m_msgLen = sizeof(PHONE_MSG_HEAD_T) + sizeof(m_sendRegInfo);
	m_pMsg = new char [m_msgLen];
	m_bSendInfoOk = false;

	m_bChangeMsg = false;

	return true;
}

void CPhoneSync::Quit()
{
	if (NULL != m_pMsg)
	{
		delete [] m_pMsg;
		m_pMsg = NULL;
	}
}

//
void CPhoneSync::SetPhoneServerIp(unsigned long ip)
{
	m_lock.Lock();

	m_phoneServerIp = ip;
	SendContent();
	m_bChangeMsg = true;

	m_lock.UnLock();
}


void CPhoneSync::SetPhoneServerPort(unsigned long usServerPort)
{
	m_lock.Lock();

	m_phoneServerPort = usServerPort;
	SendContent();
	m_bChangeMsg = true;



	m_lock.UnLock();
}

//
void CPhoneSync::SetDVRHttpPort(unsigned short usHttpPort)
{

	m_lock.Lock();
	
	m_dvrHttpPort = usHttpPort;

	SendContent();


	m_lock.UnLock();

}

//
void CPhoneSync::SetDVRServerPort(unsigned short usServerPort)
{
	m_lock.Lock();

	m_dvrServerPort = usServerPort;

	SendContent();
	
	m_lock.UnLock();
}

bool CPhoneSync::SetMac(char *pMac, int len)
{
	m_lock.Lock();

	if (len >= 64)
	{
		m_lock.UnLock();
		return false;
	}
	memset(m_szMac, 0, sizeof(m_szMac));
	memcpy(m_szMac, pMac, len);

	SendContent();

	m_lock.UnLock();

	return true;
}
//
bool CPhoneSync::SetDVRUsrPassword(char *pPassword, int len)
{
	m_lock.Lock();
	
	if (len >= 64)
	{
		m_lock.UnLock();
		return false;
	}
	if (0 != strcmp(m_szUsrPassword, pPassword))
	{
		memset(m_szUsrPassword, 0, sizeof(m_szUsrPassword));
		memcpy(m_szUsrPassword, pPassword, len);

		SendContent();
	}
	
	m_lock.UnLock();

	return true;
}

bool CPhoneSync::SetDVRVersion(char *pVersion, int len)
{
	m_lock.Lock();

	if (len >= 64)
	{
		m_lock.UnLock();
		return false;
	}
	memset(m_szVersion, 0, sizeof(m_szVersion));
	memcpy(m_szVersion, pVersion, len);

	SendContent();

	m_lock.UnLock();
	return true;
}


//
bool CPhoneSync::Start()
{
	m_sendDataID = PUB_CreateThread(SendPhoneRegInfoThread, this, &m_bSendDataRun);
	if(m_sendDataID == PUB_CREATE_THREAD_FAIL)
	{
		PUB_PrintError(__FILE__,__LINE__);
		return false;
	}

	return true;
	
}

//
void CPhoneSync::Stop()
{
	if (m_sendDataID != PUB_CREATE_THREAD_FAIL)
	{
		PUB_ExitThread(&m_sendDataID, &m_bSendDataRun);
	}
}



PUB_THREAD_RESULT PUB_THREAD_CALL CPhoneSync::SendPhoneRegInfoThread(void *pParam)
{
#ifdef __ENVIRONMENT_LINUX__
	pid_t pid = getpid();
	pid_t tid = syscall(__NR_gettid);
	printf("%s, %d, pid = %d, tid=%d\n", __FILE__, __LINE__, pid, tid);
#endif

	CPhoneSync *pThis = reinterpret_cast<CPhoneSync *>(pParam);
	pThis->SendPhoneRegInfoRun();

	return 0;
}



int CPhoneSync::SendPhoneRegInfoRun()
{
	SWL_socket_t sockSend = SWL_INVALID_SOCKET;
	bool bConnect = true;

	char *pRecvBuf = new char [sizeof(PHONE_MSG_HEAD_T)];
	int pRecvLen = sizeof(PHONE_MSG_HEAD_T);
	printf("%s,%d,pRecvLen:%d\n",__FILE__,__LINE__, pRecvLen);

	while (m_bSendDataRun)
	{
		m_lock.Lock();

		if (!m_bSendInfoOk)
		{
			printf("%s,%d,###m_bSendInfoOk:false\n",__FILE__,__LINE__);
			m_lock.UnLock();
			PUB_Sleep(30000);
			continue;
		}

		if (SWL_INVALID_SOCKET == sockSend)
		{
			printf("%s,%d,###SWL_INVALID_SOCKET == sockSend\n",__FILE__,__LINE__);
			sockSend = SWL_CreateSocket(AF_INET, SOCK_STREAM, 0);
			if(SWL_INVALID_SOCKET == sockSend)
			{				
				printf("%s,%d,###SWL_CreateSocket error\n",__FILE__,__LINE__);
				m_lock.UnLock();
				PUB_Sleep(30000);
				continue;
			}
			
			bConnect = true;
		}

		if (bConnect)
		{
			printf("%s,%d,###bConnect:false\n",__FILE__,__LINE__);
			struct sockaddr_in struServerAddr;
			struServerAddr.sin_family = AF_INET;
			struServerAddr.sin_addr.s_addr = m_phoneServerIp;
			struServerAddr.sin_port = htons(m_phoneServerPort); 
			memset(&(struServerAddr.sin_zero), '\0', 8); // zero the rest of the struct
			SWL_socklen_t addrLen = sizeof(struServerAddr);

				//连接
		     int iRet = SWL_Connect(sockSend, reinterpret_cast<struct sockaddr*>(&struServerAddr), addrLen, 8000);

			 if (0 != iRet)
			 {
				 printf("%s,%d,###SWL_Connect error\n",__FILE__,__LINE__);
				 m_lock.UnLock();
				 PUB_Sleep(30000);
				 continue;
			 }

			 printf("%s,%d,###SWL_Connect ok\n",__FILE__,__LINE__);
			 bConnect = false;	
			 m_bChangeMsg = false;
		}

		{
			bool bSendSuc = true;
			int iRet = 0;
			int iLeft = m_msgLen;    
			const char *pTmp = static_cast<const char*>(m_pMsg);

			while ( 0 != iLeft )
			{
				assert(0 < iLeft);
				assert(iLeft <= m_msgLen);
				//assert(pTmp >= pBuf);
				assert(pTmp <= (static_cast<const char*>(m_pMsg) + m_msgLen));

				iRet = SWL_Send(sockSend, pTmp , iLeft, 0);

				if ( 0 < iRet)
				{
					pTmp += iRet;
					iLeft -= iRet;
					printf("%s,%d,###%d\n",__FILE__,__LINE__,iRet);
				}
				else if( SWL_SOCKET_ERROR == iRet)
				{
					bSendSuc = false;
					break;					
				}
				else	// 0 == iRet
				{
					bSendSuc = false;
					break;
				}
			}

			if (bSendSuc)
			{
				int recvRet = RecvMsg(sockSend, pRecvBuf, pRecvLen, true);
				printf("%s,%d,revRetLen:%d\n",__FILE__,__LINE__,recvRet);
				if (recvRet <= 0)
				{
					printf("%s,%d,###bRecvMsg:false\n",__FILE__,__LINE__);
					SWL_CloseSocket(sockSend);
					sockSend = SWL_INVALID_SOCKET;
					bConnect = true;
					m_lock.UnLock();
					PUB_Sleep(25000);
					continue;
				}
			}
			


			if (m_bChangeMsg)
			{
				printf("%s,%d,###m_bChangeMsg:true\n",__FILE__,__LINE__);
				SWL_CloseSocket(sockSend);
				sockSend = SWL_INVALID_SOCKET;
				bConnect = true;
				m_bChangeMsg = false;
				m_lock.UnLock();
				PUB_Sleep(25000);
				continue;
			}

			if (!bSendSuc)
			{
				printf("%s,%d,###bSendSuc:false\n",__FILE__,__LINE__);
				SWL_CloseSocket(sockSend);
				sockSend = SWL_INVALID_SOCKET;
				bConnect = true;
				m_lock.UnLock();
				PUB_Sleep(25000);
				continue;
			}
			else
			{
				 printf("%s,%d,###bSendSuc:ok\n",__FILE__,__LINE__);
				m_lock.UnLock();
				PUB_Sleep(25000);
				continue;
			}
		}


	}

}

void CPhoneSync::SendContent()
{
	m_bSendInfoOk = false;

	if (0 == m_phoneServerIp)
	{
		return;
	}

	if (0 == m_phoneServerPort)
	{
		return;
	}

	if (0 == m_dvrHttpPort)
	{
		return;
	}
	if (0 == m_dvrServerPort)
	{
		return;
	}
	if (0 == strlen(m_szMac))
	{
		return;
	}	
	if (0 == strlen(m_szVersion))
	{
		return; 
	}

	memset(&m_sendRegInfo, 0, sizeof(m_sendRegInfo));
	snprintf(m_sendRegInfo.reg_info, PHONE_MSG_REG_Info_LEN, "type:dvr%d;mac:%s;httpport:%d;tcpport:%d;udpport:7000;rtspport:554;username:admin;password:%s;vsersion:%s;",  
		                                                      m_productType, m_szMac, m_dvrHttpPort, m_dvrServerPort,m_szUsrPassword, m_szVersion);
	printf("##########%s,%d, %s\n", __FILE__,__LINE__, m_sendRegInfo.reg_info);
	memset(m_pMsg, 0, sizeof(m_msgLen));
    memcpy(m_pMsg, &m_msgHead, sizeof(m_msgHead));
	memcpy(m_pMsg + sizeof(m_msgHead), &m_sendRegInfo, sizeof(m_sendRegInfo));

	m_bSendInfoOk = true;	

	m_bChangeMsg = true;
}


int CPhoneSync::RecvMsg(SWL_socket_t sock, char *pBuf, int iLen, bool bBlock)
{
	int iRet = 0;
	int iLeft = iLen;    //还要接收的数据长度
	char *pTmp = reinterpret_cast<char*>(pBuf);

	bool bContinue = false;	//在非阻塞情况下，是否需要再等待数据
	time_t startTime = time(NULL);
	time_t nowTime;
	time_t disTime;

	while ( 0 != iLeft ) 
	{	
		assert(0 < iLeft);
		assert(iLeft <= iLen);
		assert(pTmp >= pBuf);
		assert(pTmp < (static_cast<const char*>(pBuf) + iLen));

		iRet = SWL_Recv(sock, pTmp, iLeft, 0);
		if (0 < iRet)
		{
			pTmp += iRet;
			iLeft -= iRet;
			bContinue = true;
		}
		else if( SWL_SOCKET_ERROR == iRet)
		{
			//如果是暂时没有数据,判断是否已经超时
			if(SWL_EWOULDBLOCK())
			{
				nowTime = time(NULL);
				disTime = nowTime - startTime;
				if (disTime >= 120)	//BUGS 除了正常的超时外，系统把时间改快也可能导致
				{
					printf("#####%s,%d\n",__FILE__,__LINE__);
					return iLen - iLeft;
				}
				else if(disTime >= 0)	//防止系统时间调慢后导致差值为负数，甚至导致需要一个太长时间才能超时，如几十年
				{
					if(bBlock || bContinue) //阻塞，或非阻塞接收了部分数据，
					{
						PUB_Sleep(0);
						continue;
					}
					else
					{
						printf("#####%s,%d\n",__FILE__,__LINE__);
						return iLen - iLeft;
					}
				}
				else	//系统将时间调慢将导致此情况，简单的处理方法为重新记时
				{
					startTime = time(NULL);
				}
			}
			else
			{
				printf("#####%s,%d\n",__FILE__,__LINE__);
				return -1;
			}			
		}
		else //0 == iRet
		{
			printf("#####%s,%d\n",__FILE__,__LINE__);
			return -1;
		}
	}

	printf("%s,%d\n",__FILE__,__LINE__);
	return iLen - iLeft;
}

