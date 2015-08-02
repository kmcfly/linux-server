// SWL_ListenProcEx.cpp: implementation of the CSWL_ListenProcEx class.
//
//////////////////////////////////////////////////////////////////////

#include <assert.h>
#include <string.h>

#ifdef   __ENVIRONMENT_LINUX__
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
#include <netdb.h>
#endif

#include "SWL_Public.h"
#include "SWL_MultiNetComm.h"
#include "SWL_ListenProcEx.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSWL_ListenProcEx::CSWL_ListenProcEx(GET_ACCEPT_LINK_CALLBACK pCallback, void* pParam)
{
	//��accept֮��û���ûص������������������Դй¶����˻ص���������Ϊ��
	assert(NULL != pCallback);
	m_pAcceptCallback = pCallback;
	m_pParam = pParam;
	m_SocketListen = SWL_INVALID_SOCKET;
	m_AcceptThreadID = PUB_THREAD_ID_NOINIT;
	m_bAcceptThreadRun = false;
	m_bListenStart = false;
	PUB_InitLock(&m_ListenLock);
}

CSWL_ListenProcEx::~CSWL_ListenProcEx()
{
	PUB_DestroyLock(&m_ListenLock);
}
 
//����socket,����ʼ�����˿�,����accept�̣߳�accept�߳̽�����ʱ��socket������
//return value: 0 �ɹ�  1 ����socketʧ�� 2 ��ȡ���������߻�ȡ����IP��ַʧ��
//				3 ��socketʧ��  4 ����ʧ��  5 ���������߳�ʧ��
//				6 �Ѿ���ʼ�˼���
int CSWL_ListenProcEx::StartListen(unsigned short int nPort)
{
	//������StartListen�ķ��ؽ��
	int iReturn = 0;
	int iLoop = 0;
	CPUB_LockAction   FunctionLock(&m_ListenLock); //������������

	//����Ѿ���ʼ�˼�������ֱ�ӷ���
	if (m_bListenStart)
	{
		iReturn = 6;
		goto StartListen_end;
	}

#ifdef __CUSTOM_IPV6__
	m_SocketListen = CreateIPV6Socket(nPort);
#else
	m_SocketListen = CreateIPV4Socket(nPort);
#endif
	if (SWL_INVALID_SOCKET == m_SocketListen)
	{
		iReturn = 1;
		goto StartListen_end;
	}
	
	//����
	
	while(0 != SWL_Listen(m_SocketListen, 5/*SOMAXCONN*/))
	{
		++iLoop;
		if (iLoop < 12) 
		{
			SWL_PrintError(__FILE__,__LINE__);	
			PUB_Sleep(10000);
		}
		else
		{
			SWL_PrintError(__FILE__,__LINE__);		
			iReturn = 4;
			goto StartListen_end_close_sock;
		}	
	}

	//����accept���߳�
	m_AcceptThreadID = PUB_CreateThread(StartAccept, (void *)this, &m_bAcceptThreadRun);
	if( PUB_CREATE_THREAD_FAIL == m_AcceptThreadID )
	{
		PUB_PrintError(__FILE__,__LINE__);
		iReturn = 5;
		goto StartListen_end_close_sock;
	}

	//���������ɹ���
	iReturn = 0;
	m_bListenStart = true;
	goto StartListen_end;

	//�ڴ���socket�󣬺���Ľ׶γ�����Ҫ�Ѵ�����socket����
StartListen_end_close_sock:    
	SWL_CloseSocket(m_SocketListen);
	m_SocketListen = SWL_INVALID_SOCKET;
	
StartListen_end:
	return iReturn;
}

//ֹͣ�����˿�,����socket,socket�����ٷ�����accept�̵߳Ľ���
//return value: 0 �ɹ�
//				1 �Ѿ�ֹͣ
int CSWL_ListenProcEx::StopListen()
{
	CPUB_LockAction   FunctionLock(&m_ListenLock);
	
	//����Ѿ�ֹͣ�˼�������ֱ�ӷ���
	if (!m_bListenStart) 
	{
		return 1;
	}

	//�˳�accept�߳�
	PUB_ExitThread(&m_AcceptThreadID, &m_bAcceptThreadRun);
	m_bListenStart = false;
	return 0;
}

//accept�̣߳�����AcceptThreadRun
//BUGS�������̵߳��˳�״̬û��������
PUB_THREAD_RESULT CSWL_ListenProcEx::StartAccept(void *pParam)
{
#ifdef	__ENVIRONMENT_LINUX__
	pid_t pid = getpid();
	pid_t tid = syscall(__NR_gettid);
	printf("%s, %d, pid = %d, tid=%d\n", __FILE__, __LINE__, pid, tid);
#endif
	assert(NULL != pParam);
	CSWL_ListenProcEx *pListenProc = reinterpret_cast<CSWL_ListenProcEx*>(pParam);
#ifdef __CUSTOM_IPV6__
	pListenProc->AcceptThread6Run();
#else
	pListenProc->AcceptThreadRun();
#endif
	
	return 0;
}

//accept�߳�,accept��run����
//return value:0 �߳�����
//			   1 �߳��˳�ʱ�ر�socketʧ��
//			   2 accept����
int CSWL_ListenProcEx::AcceptThreadRun()
{	
	SWL_socket_t  sClientLinkSock;
	struct sockaddr struAddr;
	SWL_socklen_t len = sizeof(struAddr);

	while(m_bAcceptThreadRun)
	{		
		sClientLinkSock = SWL_Accept(m_SocketListen, (struct sockaddr *)&struAddr, &len);

		if(SWL_INVALID_SOCKET == sClientLinkSock) 
		{
			if(SWL_EWOULDBLOCK())
			{
#ifdef __CHIP3515__
				PUB_Sleep(100);
#else
				PUB_Sleep(100);
#endif
				continue;
			}
			else
			{
				SWL_PrintError(__FILE__, __LINE__);
				assert(false);//xg ����������ô����˳����̺߳��µĿͻ��˾ͽ�������
				return 2;
			}
		}

		m_clientSockInfo.pTcpSock = sClientLinkSock;
		struct sockaddr_in * sockAddr = (struct sockaddr_in*)&struAddr;
		m_clientSockInfo.dwIP = sockAddr->sin_addr.s_addr;
		m_clientSockInfo.nPort = sockAddr->sin_port;

		//���ö�����ʱע��Ļص�����
		if (0 != m_pAcceptCallback(&m_clientSockInfo, m_pParam, NULL))
		{
			SWL_CloseSocket(m_clientSockInfo.pTcpSock);
		}
	}

	if ( 0 != SWL_CloseSocket(m_SocketListen) )
	{
		return 1;
	}
	return 0;
}

int CSWL_ListenProcEx::AcceptThread6Run()
{
	SWL_socket_t  sClientLinkSock;
	struct sockaddr_in6 structAddrIn6;
	SWL_socklen_t len = sizeof(structAddrIn6);

	memset(&m_clientSockInfo, 0, sizeof(CLIENT_SOCK_INFO));

	while(m_bAcceptThreadRun)
	{		
		sClientLinkSock = SWL_Accept(m_SocketListen, (struct sockaddr *)&structAddrIn6, &len);

		if(SWL_INVALID_SOCKET == sClientLinkSock) 
		{
			if(SWL_EWOULDBLOCK())
			{
#ifdef __CHIP3515__
				PUB_Sleep(100);
#else
				PUB_Sleep(100);
#endif
				continue;
			}
			else
			{
				SWL_PrintError(__FILE__, __LINE__);
				assert(false);//xg ����������ô����˳����̺߳��µĿͻ��˾ͽ�������
				return 2;
			}
		}

		m_clientSockInfo.pTcpSock = sClientLinkSock;
		//m_clientSockInfo.dwIP = structAddrIn6.sin6_addr;
		m_clientSockInfo.nPort = structAddrIn6.sin6_port;

		//�����IPV4��IP��ַ����¼IP��Ϣ������IPV6�ĵ�ַ����Ϊ255.255.255.255
 		unsigned char * pIpAddr = structAddrIn6.sin6_addr.s6_addr;
 		if (0x00 == pIpAddr[0] && 0x00 == pIpAddr[1] && 0x00 == pIpAddr[2] &&
 			0x00 == pIpAddr[3] && 0x00 == pIpAddr[4] && 0x00 == pIpAddr[5] &&
 			0x00 == pIpAddr[6] && 0x00 == pIpAddr[7] && 0x00 == pIpAddr[8] &&
 			0x00 == pIpAddr[9] && 0xFF == pIpAddr[10] && 0xFF == pIpAddr[11])
 		{
 			//��ʾ���Ǹ�IPV4�Ŀͻ���
 			char * pIp = (char *)&m_clientSockInfo.dwIP;
 			pIp[0] = pIpAddr[12];
 			pIp[1] = pIpAddr[13];
 			pIp[2] = pIpAddr[14];
 			pIp[3] = pIpAddr[15];
 
 			struct in_addr in;
 			memset(&in, 0, sizeof(struct in_addr));
 			in.s_addr = m_clientSockInfo.dwIP;
 		}
		else
		{
			memset(&m_clientSockInfo.dwIP, 0xff, sizeof(m_clientSockInfo.dwIP));
		}

		printf("%s:%s:%d, get accept for ipv6 client\n", __FUNCTION__, __FILE__, __LINE__);
		//���ö�����ʱע��Ļص�����
		if (!m_pAcceptCallback(&m_clientSockInfo, m_pParam, NULL))
		{
			SWL_CloseSocket(m_clientSockInfo.pTcpSock);
		}
	}

	if ( 0 != SWL_CloseSocket(m_SocketListen) )
	{
		return 1;
	}
	return 0;
}

SWL_socket_t CSWL_ListenProcEx::CreateIPV4Socket(unsigned short port)
{
	SWL_socket_t sockfd = SWL_INVALID_SOCKET;
	//����һ��socket�����������е�socket���Ƿ�������
	sockfd = SWL_CreateSocket(AF_INET, SOCK_STREAM, 0);
	if(SWL_INVALID_SOCKET == sockfd)
	{
		printf("%s:%s:%d, Create socket error\n", __FUNCTION__, __FILE__, __LINE__);
		assert(false);
		return sockfd;
	}

	struct sockaddr_in sockaddrIn4;
	memset(&sockaddrIn4, 0, sizeof(struct sockaddr_in));

	SWL_socklen_t  sockAddrLen = sizeof(struct sockaddr_in);

	sockaddrIn4.sin_family = AF_INET;
	sockaddrIn4.sin_addr.s_addr = htonl(INADDR_ANY);;
	sockaddrIn4.sin_port = htons(port); 		//����Ҫת��������
	memset(&(sockaddrIn4.sin_zero), '\0', 8);	// zero the rest of the struct

	int reuse = 1;
	setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (char *)&reuse, sizeof(int));

	int iSendBuffer = 256 * 1024;
	setsockopt(m_SocketListen, SOL_SOCKET, SO_SNDBUF, (char *)&iSendBuffer, sizeof(int));

	int iSendLowat = 128 * 1024;
	setsockopt(m_SocketListen, SOL_SOCKET, SO_SNDLOWAT, (char *)&iSendLowat, sizeof(int));


	//��socket
	int	iLoop = 0;
	while(0 != SWL_Bind(sockfd, (sockaddr*)&sockaddrIn4, sizeof(sockaddr_in)))
	{
		++iLoop;
		if (iLoop < 60) 
		{
			SWL_PrintError(__FILE__,__LINE__);				
			PUB_Sleep(2000);
		}
		else
		{
			SWL_PrintError(__FILE__,__LINE__);
			SWL_CloseSocket(sockfd);
			sockfd = SWL_INVALID_SOCKET;
			break;
		}			
	}

	return sockfd;
}

SWL_socket_t CSWL_ListenProcEx::CreateIPV6Socket(unsigned short port)
{
	SWL_socket_t  sockfd = SWL_INVALID_SOCKET;
	sockfd = SWL_CreateSocket(PF_INET6, SOCK_STREAM, 0);
	if (SWL_INVALID_SOCKET == sockfd)
	{
		printf("%s:%s:%d, Create socket error\n", __FUNCTION__, __FILE__, __LINE__);
		assert(false);
		return SWL_INVALID_SOCKET;
	}

	struct sockaddr_in6 sockaddrIn6;
	memset(&sockaddrIn6, 0, sizeof(struct sockaddr_in6));
	sockaddrIn6.sin6_family = PF_INET6;
	sockaddrIn6.sin6_port   = htons(port);
	//sockaddrIn6.sin6_addr	= in6addr_any;

	int reuse = 1;
	setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (char *)&reuse, sizeof(int));

	//reuse = 1;
	//setsockopt(sockfd, IPPROTO_IPV6, IPV6_V6ONLY, &reuse, sizeof(int));
	//setsockopt(sockfd, IPPROTO_IPV6, 26, &reuse, sizeof(int));

	int	iLoop = 0;
	while(0 != SWL_Bind(sockfd, (sockaddr*)&sockaddrIn6, sizeof(struct sockaddr_in6)))
	{
		++iLoop;
		if (iLoop < 60) 
		{
			SWL_PrintError(__FILE__,__LINE__);				
			PUB_Sleep(2000);
		}
		else
		{
			SWL_PrintError(__FILE__,__LINE__);
			SWL_CloseSocket(sockfd);
			sockfd = SWL_INVALID_SOCKET;
			break;
		}			
	}

	return sockfd;
}


