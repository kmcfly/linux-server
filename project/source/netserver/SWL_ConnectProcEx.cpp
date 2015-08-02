// SWL_ConnectProcEx.cpp: implementation of the CSWL_ConnectProcEx class.
//
//////////////////////////////////////////////////////////////////////
#include "SWL_ConnectProcEx.h"

#include <assert.h>
#include <string.h>

#ifdef	__ENVIRONMENT_LINUX__
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#endif

#include "SWL_Public.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


CSWL_ConnectProcEx::CSWL_ConnectProcEx(const char *pIPAddr, unsigned short nPort):m_nPort(nPort)
{
	//����IP��ַ
	char ipTemp[256] = {0};
	char portTmp[60] = {0};
	snprintf(portTmp, sizeof(portTmp), "%d", nPort);

	struct addrinfo hint;
	memset(&hint, 0, sizeof(struct addrinfo));

	m_pAddrInfo = NULL;
	//IPv6��ַ��ĿǰIPv6�ĵ�ַ��֧��DNS��ʽ����
	if ('[' == pIPAddr[0])
	{
		snprintf(ipTemp, sizeof(ipTemp), "%s", pIPAddr + 1);
		for (int i=0; i<strlen(ipTemp); i++)
		{
			if (']' == ipTemp[i])
			{
				ipTemp[i]='\0';
				break;
			}
		}

		hint.ai_flags = AI_CANONNAME;
		hint.ai_family = PF_INET6;
		hint.ai_socktype = SOCK_STREAM;
		hint.ai_protocol = 0;
	}
	else//IPv4��ַ
	{
		snprintf(ipTemp, sizeof(ipTemp), "%s", pIPAddr);
		hint.ai_flags = AI_CANONNAME;
		hint.ai_family = PF_INET;
		hint.ai_socktype = SOCK_STREAM;
		hint.ai_protocol = 0;
	}

	if (0 != getaddrinfo(ipTemp, portTmp, (const struct addrinfo *)&hint, &m_pAddrInfo))
	{
		assert(false);
		return;
	}

	m_ConnectThreadID = PUB_THREAD_ID_NOINIT;
	m_Sock = SWL_INVALID_SOCKET;
	m_pConnectCallbackEx = NULL;
	m_pConnectCallbackParamEx = NULL;
	m_bConnectThreadRun = false;
	m_bDestroy = false;
	PUB_InitLock(&m_ConnectSynLock);
	PUB_InitLock(&m_ConnectAsynLock);
}

CSWL_ConnectProcEx::~CSWL_ConnectProcEx()
{
	if (m_pAddrInfo)
	{
		freeaddrinfo(m_pAddrInfo);
		m_pAddrInfo = NULL;
	}

	PUB_DestroyLock(&m_ConnectSynLock);
	PUB_DestroyLock(&m_ConnectAsynLock);
}

//CreateMethod
CSWL_ConnectProcEx* CSWL_ConnectProcEx::NewConnectProc(const char *pIPAddr, unsigned short nPort)
{
	return new CSWL_ConnectProcEx(pIPAddr, nPort);
}

int CSWL_ConnectProcEx::Destroy()
{
	m_bDestroy = true;

	//�����һ���첽������������
	if (PUB_THREAD_ID_NOINIT != m_ConnectThreadID)
	{
		PUB_ExitThread(&m_ConnectThreadID, NULL);	
	}

	if(SWL_INVALID_SOCKET != m_Sock)
	{
		SWL_CloseSocket(m_Sock);
	}

	delete this;
	return 0;
}

//����ֵ��  �ɹ� >0
//			ʧ�� SWL_SOCKET_ERROR 
SWL_socket_t CSWL_ConnectProcEx::ConnectSynEx(int timeOut)
{
	CPUB_LockAction	FunctionLock(&m_ConnectSynLock); //������������
	SWL_socket_t sockTmp = SWL_INVALID_SOCKET;

	m_Sock = SWL_CreateSocket(m_pAddrInfo->ai_family, m_pAddrInfo->ai_socktype, m_pAddrInfo->ai_protocol);
	if(SWL_INVALID_SOCKET == m_Sock)
	{
		SWL_PrintError(__FILE__,__LINE__);
		return SWL_SOCKET_ERROR;
	}
	
#if defined(__ENVIRONMENT_WIN32__)
	int iRecvBuffer = 512 * 1024;
#else
	int iRecvBuffer = 128 * 1024;
#endif
	setsockopt(m_Sock, SOL_SOCKET, SO_RCVBUF, (char *)&iRecvBuffer, sizeof(int));

	//���arp����
#ifdef __ENVIRONMENT_LINUX__
	//BUGS:�����ϵ��ļ�ϵͳû�б�����arp�������������Ҫ���������
#elif defined __ENVIRONMENT_WIN32__
	char szCmd[100] = {0};
	sprintf(szCmd, "/C arp -d %s",m_szIP);
//	ShellExecute(NULL, "Open", "Cmd.exe", szCmd, NULL, SW_HIDE);
#endif
	
	int iRet = 0;
	const int waitTime = 1000;	
	int iTimeOut = 0; //һ�γ�ʱ�ĳ�ʱʱ��
	int iCountTotal = 0;    //ѭ���ȴ���ʱ�Ĵ���
	int iLastTime = timeOut % waitTime;
	int iCount = 0;
	iCountTotal = timeOut / waitTime + (( 0 == iLastTime ) ? 0 : 1);
		
	do {
		++iCount;
		if((iCount == iCountTotal) && (0 != iLastTime))
		{
			iTimeOut = iLastTime;	
		}
		else
		{
			iTimeOut = waitTime;			
		}	
		
		//����
		iRet = SWL_Connect(m_Sock, m_pAddrInfo->ai_addr, m_pAddrInfo->ai_addrlen, iTimeOut);
	} while((iCount < iCountTotal) && (0 != iRet) && !m_bDestroy);	

	if(0 != iRet)
	{
		return SWL_SOCKET_ERROR;
	}
	else
	{
		sockTmp = m_Sock;

		//���ӳɹ������socket�ŵ�CSWL_TCPLink�����У����ظ���������
		//��һ���������´���һ���µ�socket
		m_Sock = SWL_INVALID_SOCKET;
		return sockTmp;
	}
}
				
//���ӣ��������ϵ��ûص�����
//return value: 0 �ɹ�
//				1 ʧ�ܣ���һ���첽������������
//				2 ʧ�ܣ������첽�����߳�ʧ��
// int CSWL_ConnectProcEx::ConnectAsyn(GET_CONNECT_LINK_CALLBACKEX pCallback, 
// 								  void* pObject, int iTimeOut)
// {
// 	assert( (NULL != pCallback) && (NULL != pObject) );
// 	CPUB_LockAction	FunctionLock(&m_ConnectAsynLock); //������������
// 
// 	//�����һ���첽������������
// 	if (m_bConnectThreadRun)
// 	{
// 		return 1;
// 	}
// 	else if (PUB_THREAD_ID_NOINIT != m_ConnectThreadID) //�߳��˳��ˣ���Ҫjoin
// 	{
// 		//����ڶ���������NULL��m_bConnectThreadRun�����ԣ���Ϊ�ڱ��߳��˳�ʱ��
// 		//�Ѿ���m_bConnectThreadRun�����false
// 		PUB_ExitThread(&m_ConnectThreadID, NULL);	
// 	}
// 
// 	m_pConnectCallback = pCallback;
// 	m_pConnectCallbackParam = pObject;
// 	m_iTimeOut = iTimeOut;
// 	m_ConnectThreadID = PUB_CreateThread(StartConnect, (void *)this, &m_bConnectThreadRun);
// 	if( PUB_CREATE_THREAD_FAIL == m_ConnectThreadID )
// 	{
// 		PUB_PrintError(__FILE__,__LINE__);
// 		return 2;
// 	}
// 	
// 	return 0;
// }

//���ӣ��������ϵ��ûص�����
//return value: 0 �ɹ�
//				1 ʧ�ܣ���һ���첽������������
//				2 ʧ�ܣ������첽�����߳�ʧ��
int CSWL_ConnectProcEx::ConnectAsynEx(GET_CONNECT_LINK_CALLBACKEX pCallback, void* pObject, int iTimeOut)
{
	assert( (NULL != pCallback));
	CPUB_LockAction	FunctionLock(&m_ConnectAsynLock); //������������

	//�����һ���첽������������
	if (m_bConnectThreadRun)
	{
		return 1;
	}
	else if (PUB_THREAD_ID_NOINIT != m_ConnectThreadID) //�߳��˳��ˣ���Ҫjoin
	{
		//����ڶ���������NULL��m_bConnectThreadRun�����ԣ���Ϊ�ڱ��߳��˳�ʱ��
		//�Ѿ���m_bConnectThreadRun�����false
		PUB_ExitThread(&m_ConnectThreadID, NULL);	
	}

	m_pConnectCallbackEx = pCallback;
	m_pConnectCallbackParamEx = pObject;
	m_iTimeOut = iTimeOut;
	m_ConnectThreadID = PUB_CreateThread(StartConnectEx, (void *)this, &m_bConnectThreadRun);
	if( PUB_CREATE_THREAD_FAIL == m_ConnectThreadID )
	{
		PUB_PrintError(__FILE__,__LINE__);
		return 2;
	}
	
	return 0;
}

//connect�̣߳�����ConnectThreadRun
//BUGS�������̵߳��˳�״̬û��������
// PUB_THREAD_RESULT CSWL_ConnectProcEx::StartConnect(void *pParam)
// {
// #ifdef	__ENVIRONMENT_LINUX__
// 	PUB_PRINTF_INFO("%s %d pid = %d\n",__FILE__, __LINE__, getpid());
// #endif
// 	assert(NULL != pParam);
// 	CSWL_ConnectProcEx *pConnectProc = static_cast<CSWL_ConnectProcEx*>(pParam);
// 	pConnectProc->ConnectThreadRun();
// 	return 0;
// }

PUB_THREAD_RESULT CSWL_ConnectProcEx::StartConnectEx(void *pParam)
{
#ifdef	__ENVIRONMENT_LINUX__
	pid_t pid = getpid();
	pid_t tid = syscall(__NR_gettid);
	printf("%s, %d, pid = %d, tid=%d\n", __FILE__, __LINE__, pid, tid);
#endif
	assert(NULL != pParam);
	CSWL_ConnectProcEx *pConnectProc = static_cast<CSWL_ConnectProcEx*>(pParam);
	pConnectProc->ConnectThreadRunEx();
	return 0;
}

int CSWL_ConnectProcEx::ConnectThreadRunEx()
{
	SWL_socket_t sock = SWL_INVALID_SOCKET;

	sock = ConnectSynEx(m_iTimeOut);	

	//�õ�һ�����ӣ��������Ӳ��ϣ������ص������������˳���ѭ��
	m_pConnectCallbackEx(sock, m_pConnectCallbackParamEx, NULL);
		
	//�˳�ǰ�ȸ����������ֵfalse������ConnectAsyn�ж�
	m_bConnectThreadRun = false;
	return 0;
}


