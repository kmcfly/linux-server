// Multicast.cpp: implementation of the CTSMulticast class.
//
//////////////////////////////////////////////////////////////////////
#include <stdio.h>

#ifndef __ENVIRONMENT_WIN32__
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#endif
#include "TSMulticast.h"

#include <assert.h>
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTSMulticast::CTSMulticast()
{
	m_maxFdValue = SWL_INVALID_SOCKET;
	m_MultiCastSocketSend = SWL_INVALID_SOCKET;
	m_bReady = false;
	PUB_InitLock(&m_lock);
	FD_ZERO(&m_recvSet);
}

CTSMulticast::~CTSMulticast()
{
	PUB_DestroyLock(&m_lock);
}

//pMulticastip带结束符的广播地址
int CTSMulticast::Init(unsigned short wLocalPort, unsigned short wRemotePort, const char *pMulticastip, const char *pIfName)
{
#ifndef WIN32
	char szCmd[64] = {0};
	sprintf(szCmd, "route add -net 224.0.0.0 netmask 240.0.0.0 dev %s", pIfName);
	DVRSystem(szCmd);
#endif
	CPUB_LockAction	funLock(&m_lock);
	m_bReady = false;

	strcpy(m_szIfName, pIfName);

	int socklen = sizeof(struct sockaddr_in);
	// 设置发送的端口和IP信息
	memset(&m_sendSockaddrIn, 0, socklen);
	m_sendSockaddrIn.sin_family = AF_INET;
	m_sendSockaddrIn.sin_port = htons(wLocalPort);
	m_sendSockaddrIn.sin_addr.s_addr = INADDR_ANY;

	m_MultiCastSocketSend = CreateSendSock(m_sendSockaddrIn);
	if (SWL_INVALID_SOCKET == m_MultiCastSocketSend) 
	{
		PUB_PrintError(__FILE__,__LINE__);
		return -1;
	}

	struct sockaddr_in recvAddrIn;
	//设置接收的端口及IP信息
	memset(&recvAddrIn, 0, socklen);
	recvAddrIn.sin_family = AF_INET;
	recvAddrIn.sin_port = htons(wRemotePort);

	// 注意这里设置的对方地址是指组播地址，而不是对方的实际IP地址 
	if ((recvAddrIn.sin_addr.s_addr=inet_addr(pMulticastip)) == INADDR_NONE)
	{
		PUB_PrintError(__FILE__,__LINE__);
		SWL_CloseSocket(m_MultiCastSocketSend);
		m_MultiCastSocketSend = SWL_INVALID_SOCKET;
		return -1;
	}

	m_MultiCastPeerAddr = recvAddrIn;

	memset(&m_mreq,0,sizeof(m_mreq));
	//设置发送组播消息的源主机的地址信息 
	SWL_socket_t newRecvSock = CreateReceiveSock(recvAddrIn);
	if(SWL_INVALID_SOCKET == newRecvSock)
	{
		PUB_PrintError(__FILE__,__LINE__);
		SWL_CloseSocket(m_MultiCastSocketSend);
		m_MultiCastSocketSend = SWL_INVALID_SOCKET;
		return -1;
	}

	m_recvSockaddrIn.push_back(recvAddrIn);
	m_MultiCastSocketRecv.push_back(newRecvSock);

	m_maxFdValue = newRecvSock;
	FD_SET(newRecvSock, &m_recvSet);

	m_bReady = true;
	return 0;
}

int CTSMulticast::Quit()
{
	CPUB_LockAction	funLock(&m_lock);

	CloseAllSocket();

	m_bReady = false;
	return 0;
}

int CTSMulticast::AddRecvGroup(const char *pMulticastip, unsigned short wRecvPort)
{
	struct sockaddr_in recvAddrIn;
	//设置接收的端口及IP信息
	memset(&recvAddrIn, 0, sizeof(struct sockaddr_in));
	recvAddrIn.sin_family = AF_INET;
	recvAddrIn.sin_port = htons(wRecvPort);

	// 注意这里设置的对方地址是指组播地址，而不是对方的实际IP地址 
	if ((recvAddrIn.sin_addr.s_addr=inet_addr(pMulticastip)) == INADDR_NONE)
	{
		PUB_PrintError(__FILE__,__LINE__);
		return -1;
	}

	SWL_socket_t newRecvSock = CreateReceiveSock(recvAddrIn);
	if(SWL_INVALID_SOCKET == newRecvSock)
	{
		PUB_PrintError(__FILE__,__LINE__);
		return -1;
	}

	m_recvSockaddrIn.push_back(recvAddrIn);
	m_MultiCastSocketRecv.push_back(newRecvSock);

	FD_SET(newRecvSock, &m_recvSet);
	if(newRecvSock > m_maxFdValue)
	{
		m_maxFdValue = newRecvSock;
	}

	return 0;
}

int CTSMulticast::SendTo(const void *pBuff, int iLen)
{
	CPUB_LockAction	funLock(&m_lock);
	if(!m_bReady)
	{
		if(ReCreateAllSocket())
		{
			m_bReady = true;
		}
		else
		{
			return -2;
		}
	}

	if (sendto(m_MultiCastSocketSend, (const char *)pBuff, iLen, 0, (struct sockaddr *)&m_MultiCastPeerAddr,
		sizeof(struct sockaddr)) < 0) 
	{
		PUB_PrintError(__FILE__,__LINE__);
		CloseAllSocket();
		m_bReady = false;
		return -1;
	}

	return 0;
}

//0 关闭 >0 收到数据 -1 收数据失败 -2 出错
int CTSMulticast::RecvFrom(void *pBuff, int iLen)
{
	CPUB_LockAction	funLock(&m_lock);
	if(!m_bReady)
	{
		return -2;
	}

	fd_set readSet = m_recvSet;
	struct timeval timeout = {0, 10000};
	SWL_socklen_t ret = select(m_maxFdValue+1, &readSet, NULL, NULL, &timeout);

	if(ret <= 0)
	{
		return -1;
	}

	struct sockaddr recvAddr;
	SWL_socklen_t socklen = sizeof(struct sockaddr);
	int n = -1;
	
	for(int i = 0; i < m_MultiCastSocketRecv.size(); i++)
	{
		if(!FD_ISSET(m_MultiCastSocketRecv[i], &readSet))
		{
			continue;
		}

		n = recvfrom(m_MultiCastSocketRecv[i], (char *)pBuff, iLen, 0, &recvAddr, &socklen);

		if (-1 == n)
		{
			if (!SWL_EWOULDBLOCK()) 
			{
				PUB_PrintError(__FILE__,__LINE__);
				CloseAllSocket();
				m_bReady = false;
				return -2;
			}
		}
		else if(n > 0)
		{
			return n;
		}
	}

	return n;
}

SWL_socket_t CTSMulticast::CreateSendSock(sockaddr_in sockAddrIn)
{
	SWL_socket_t sock = SWL_CreateSocket(AF_INET, SOCK_DGRAM, 0);
	if (sock == SWL_INVALID_SOCKET) 
	{
		PUB_PrintError(__FILE__,__LINE__);
		return SWL_INVALID_SOCKET;
	}

#ifndef  WIN32
	setsockopt(sock, SOL_SOCKET, SO_BINDTODEVICE, m_szIfName, strlen(m_szIfName) + 1);
#endif
	return sock;
}

SWL_socket_t CTSMulticast::CreateReceiveSock(sockaddr_in sockAddrIn)
{
	// 创建接收 socket 
	SWL_socket_t newRecvSock = SWL_CreateSocket(AF_INET, SOCK_DGRAM, 0);
	if (newRecvSock == SWL_INVALID_SOCKET)
	{
		PUB_PrintError(__FILE__,__LINE__);
		return SWL_INVALID_SOCKET;
	}

#ifndef WIN32
	setsockopt(newRecvSock, SOL_SOCKET, SO_BINDTODEVICE, m_szIfName, strlen(m_szIfName) + 1);
#endif

	// 设置要加入组播的地址 
	m_mreq.imr_multiaddr.s_addr = sockAddrIn.sin_addr.s_addr;
	// 把本机加入组播地址，即本机网卡作为组播成员，只有加入组才能收到组播消息 
	if (setsockopt(newRecvSock, IPPROTO_IP, IP_ADD_MEMBERSHIP, (const char *)&m_mreq,sizeof(struct ip_mreq)) == -1)
	{
		PUB_PrintError(__FILE__,__LINE__);
		SWL_CloseSocket(newRecvSock);
		return SWL_INVALID_SOCKET;
	}

	int bMultipleApps = true;		
	if(setsockopt(newRecvSock, SOL_SOCKET, SO_REUSEADDR, (const char *)&bMultipleApps, sizeof(bMultipleApps)) == -1)
	{
		PUB_PrintError(__FILE__,__LINE__);
		setsockopt(newRecvSock, IPPROTO_IP, IP_DROP_MEMBERSHIP, (const char *)&m_mreq, sizeof(m_mreq));
		SWL_CloseSocket(newRecvSock);
		return SWL_INVALID_SOCKET;
	}

	sockAddrIn.sin_addr.s_addr = htonl(INADDR_ANY);
	if (SWL_Bind(newRecvSock, (struct sockaddr *) &sockAddrIn,sizeof(struct sockaddr_in)) == -1) 
	{
		PUB_PrintError(__FILE__,__LINE__);
		setsockopt(newRecvSock, IPPROTO_IP, IP_DROP_MEMBERSHIP, (const char *)&m_mreq, sizeof(m_mreq));
		SWL_CloseSocket(newRecvSock);
		return SWL_INVALID_SOCKET;
	}

	return newRecvSock;
}

void CTSMulticast::CloseAllSocket()
{
	if (SWL_INVALID_SOCKET != m_MultiCastSocketSend)
	{
		SWL_CloseSocket(m_MultiCastSocketSend);
		m_MultiCastSocketSend = SWL_INVALID_SOCKET;
	}

	for(int i = 0; i < m_MultiCastSocketRecv.size(); i++)
	{
		setsockopt(m_MultiCastSocketRecv[i], IPPROTO_IP, IP_DROP_MEMBERSHIP, (const char *)&m_mreq, sizeof(m_mreq));
		SWL_CloseSocket(m_MultiCastSocketRecv[i]);
		FD_CLR(m_MultiCastSocketRecv[i], &m_recvSet);
	}
	m_MultiCastSocketRecv.clear();
}

bool CTSMulticast::ReCreateAllSocket()
{
#ifndef WIN32
	char szCmd[64] = {0};
	sprintf(szCmd, "route add -net 224.0.0.0 netmask 240.0.0.0 dev %s", m_szIfName);
	DVRSystem(szCmd);
#endif

	m_MultiCastSocketSend = CreateSendSock(m_sendSockaddrIn);
	if (SWL_INVALID_SOCKET == m_MultiCastSocketSend) 
	{
		PUB_PrintError(__FILE__,__LINE__);
		return false;
	}

	m_maxFdValue = 0;
	for(int i = 0; i < m_recvSockaddrIn.size(); i++)
	{
		struct sockaddr_in &recvAddrIn = m_recvSockaddrIn[i];
		SWL_socket_t newRecvSock = CreateReceiveSock(recvAddrIn);
		if((SWL_INVALID_SOCKET == newRecvSock) && (0 == i))
		{
			PUB_PrintError(__FILE__,__LINE__);
			SWL_CloseSocket(m_MultiCastSocketSend);
			m_MultiCastSocketSend = SWL_INVALID_SOCKET;
			return false;
		}

		if(newRecvSock != SWL_INVALID_SOCKET)
		{
			m_MultiCastSocketRecv.push_back(newRecvSock);
			m_maxFdValue = (m_maxFdValue > newRecvSock) ? m_maxFdValue : newRecvSock;
			FD_SET(newRecvSock, &m_recvSet);
		}
	}

	return true;
}

