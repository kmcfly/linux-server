////////////////////////////////////////////////////////////////////////////////////////
#include "RTPSock.h"

#ifdef _DEBUG
#define  new DEBUG_NEW
#endif

//#ifdef assert
//#undef assert
//#define assert(a) ((void)0)
//#endif

//#include "Profile.h"

CRTPSock::CRTPSock(struct sockaddr_in sockAddr, SWL_socket_t sockfd, int protocol)
{
	m_sockAddr	= sockAddr;
	m_socketfd	= sockfd;
	m_protocol	= protocol;
	m_DestAddr	= 0;
	m_port		= 0;
	m_RemotePort = 0;
}

CRTPSock::CRTPSock()
{
	memset(&m_sockAddr, 0, sizeof(struct sockaddr_in));
	m_socketfd	= SWL_INVALID_SOCKET;
	m_protocol	= 0;
	m_DestAddr	= 0;
	m_port		= 0;
	m_RemotePort	= 0;
}

CRTPSock::~CRTPSock()
{
	CloseSock();
}

int CRTPSock::SetSockfd(struct sockaddr_in sockAddr, SWL_socket_t sockfd, int protocol)
{
	m_sockAddr	= sockAddr;
	m_socketfd	= sockfd;
	m_protocol	= protocol;
	//m_DestAddr	= 0;
	//m_port		= 0;
	
	return 0;
}

int CRTPSock::SetPort(unsigned short port)
{
	if (SWL_INVALID_SOCKET != m_socketfd)
	{
		return -1;
	}

	m_port = port;

	return 0;
}

int CRTPSock::SetPort(unsigned short port, unsigned short RemotePort)
{
 	if (SWL_INVALID_SOCKET != m_socketfd)
 	{
 		return -1;
 	}

	m_port = port;
	m_sockAddr.sin_port	= htons(m_port);
	m_RemotePort = RemotePort;
	return 0;
}

int CRTPSock::SetIP(u_long sinAaddr)
{
//  	if (SWL_INVALID_SOCKET != m_socketfd)
//  	{
//  		assert(false);
//  		return -1;
//  	}
// 	
	m_DestAddr = sinAaddr;

	return 0;
}

u_long CRTPSock::GetIP()
{
	return m_sockAddr.sin_addr.s_addr;
}

u_long CRTPSock::GetDestIP()
{
	return m_DestAddr;
}

int CRTPSock::CreateSock(int protocol)
{
	if (SWL_INVALID_SOCKET != m_socketfd)
	{
		assert(false);
		return -1;
	}

	return CreateSock(m_port, m_RemotePort, m_DestAddr, protocol);
}


int CRTPSock::CreateSock(unsigned short port, u_long sinAddr/*=INADDR_ANY*/, int protocol/*=IPPROTO_TCP*/)
{
	if (SWL_INVALID_SOCKET != m_socketfd)
	{
		assert(false);
		return -1;
	}

	m_DestAddr = sinAddr;
	m_protocol = protocol;
	m_port = port;
	memset(&m_sockAddr, 0, sizeof(struct sockaddr_in));
	m_sockAddr.sin_port			= htons(port);
	m_sockAddr.sin_family		= AF_INET;

	//创建套接口
	if (IPPROTO_UDP == protocol)
	{
		m_sockAddr.sin_addr.s_addr	= htonl(INADDR_ANY);
		return CreateUDPSock();
	}
	else if (IPPROTO_TCP == protocol)
	{
		if (INADDR_ANY != sinAddr)
		{
			m_sockAddr.sin_addr.s_addr = sinAddr;
			return CreateTCPSock();
		}
	}
	else
	{
	}

	assert(false);
	return -1;
}

int CRTPSock::CreateSock(unsigned short port, unsigned short RemotePort, u_long sinAddr/*=INADDR_ANY*/, int protocol/*=IPPROTO_TCP*/)
{
	m_RemotePort = RemotePort;
	m_port = port;
	return CreateSock(port, sinAddr, protocol);
}

void CRTPSock::CloseSock()
{
	if (SWL_INVALID_SOCKET != m_socketfd)
	{
//#ifdef WIN32
//		shutdown(m_socketfd, SD_BOTH);
//#else
//		shutdown(m_socketfd, SHUT_RDWR);
//#endif
		SWL_CloseSocket(m_socketfd);
		printf("Close Sock\n");
	}

	m_socketfd = SWL_INVALID_SOCKET;
}

int CRTPSock::SendData(const char *pSendData, unsigned long dataLen, int sendTime)
{
	//static FunctionProfile __funcProfile(__FUNCTION__);
	//ProfilerStackFrame __funcFrame(&__funcProfile);

	int retVal = 0;
	
	if (0 == sendTime)
	{
		if (IPPROTO_TCP == m_protocol)
		{
			retVal = SWL_Send(m_socketfd, pSendData, dataLen, 0);
		}
		else
		{
			struct sockaddr_in	sockAddr;
			memset(&sockAddr, 0, sizeof(struct sockaddr_in));
			sockAddr.sin_addr.s_addr = m_DestAddr;
			sockAddr.sin_port = htons(m_RemotePort);
			sockAddr.sin_family = AF_INET;

			retVal = sendto(m_socketfd, pSendData, dataLen, 0, (const struct sockaddr *)(&sockAddr), sizeof(struct sockaddr_in));
		}
		return retVal;
	}

	fd_set wset;
	struct timeval timeOut;
	unsigned long hasSendLen = 0;

	while (sendTime > 0)
	{
		timeOut.tv_sec	= 0;
		timeOut.tv_usec = 50;

		FD_ZERO(&wset);
		FD_SET(m_socketfd, &wset);
		retVal = select(m_socketfd + 1, NULL, &wset, NULL, &timeOut);

		if (1 == retVal)
		{
			if (IPPROTO_TCP == m_protocol)
			{
				retVal = SWL_Send(m_socketfd, pSendData + hasSendLen, dataLen - hasSendLen, 0);
			}
			else
			{
				struct sockaddr_in	sockAddr;
				memset(&sockAddr, 0, sizeof(struct sockaddr_in));
				sockAddr.sin_addr.s_addr = m_DestAddr;
				sockAddr.sin_port = htons(m_RemotePort);
				sockAddr.sin_family = AF_INET;
// 				if (0 != m_RemotePort%2)
// 				{
// 					printf("RTCP SendFrom %ul\n", ntohs(m_sockAddr.sin_port));
// 					printf("RTCP SendTo %ul\n", m_RemotePort);
// 				}
				
				retVal = sendto(m_socketfd, pSendData + hasSendLen, dataLen - hasSendLen, 0, (const struct sockaddr *)(&sockAddr), sizeof(struct sockaddr_in));
			}
			
			if (0 < retVal)
			{
				hasSendLen += retVal;
			}
			else
			{
				if (0 > retVal)
				{
#ifdef WIN32
					int err = WSAGetLastError();
#endif
					return retVal;
				}
				else//retVal == 0
				{
					continue;			
				}
			}

			if (hasSendLen >= dataLen)
			{
				return hasSendLen;
			}
		}
		else if (-1 == retVal)
		{
#ifdef WIN32
			int err = WSAGetLastError();
#endif
			//WSAENOTSOCK
			assert(false);
			return -1;
		}

		sendTime -= 50;
	}

	//assert(false);
	return hasSendLen;
}

int CRTPSock::RecvData(char *pRecvBuf, unsigned long bufLen)
{
	int retVal = 0;

	if (IPPROTO_TCP == m_protocol)
	{
		retVal = SWL_Recv(m_socketfd, pRecvBuf, bufLen, 0);
		return retVal;
	}
	else if (IPPROTO_UDP == m_protocol)
	{
		struct sockaddr_in sockAddr;
#ifndef WIN32
		socklen_t sockLen = sizeof(struct sockaddr_in);
#else
		int sockLen = sizeof(struct sockaddr_in);
#endif
		memset(&sockAddr, 0, sockLen);

		retVal = recvfrom(m_socketfd, pRecvBuf, bufLen, 0, (struct sockaddr *)&sockAddr, &sockLen);
		assert(0 < retVal);

		return retVal;
	}
	else
	{
		assert(false);
		return -1;
	}
}

int CRTPSock::RecvData(char *pRecvBuf, unsigned long bufLen, int recvTime)
{
	//接收RTSP应答
	fd_set rset;
	struct timeval timeOut;
	int retVal = 0;

	while (recvTime > 0)
	{
		timeOut.tv_sec	= 0;
		timeOut.tv_usec = 50;

		FD_ZERO(&rset);
		FD_SET(m_socketfd, &rset);
		retVal = select(m_socketfd + 1, &rset, NULL, NULL, &timeOut);

		if (1 == retVal)
		{
			if (IPPROTO_TCP == m_protocol)
			{
				retVal = SWL_Recv(m_socketfd, pRecvBuf, bufLen, 0);
			}
			else if (IPPROTO_UDP == m_protocol)
			{
				struct sockaddr_in sockAddr;
#ifndef WIN32
				socklen_t sockLen = sizeof(struct sockaddr_in);
#else
				int sockLen = sizeof(struct sockaddr_in);
#endif
				memset(&sockAddr, 0, sockLen);
				
				retVal = recvfrom(m_socketfd, pRecvBuf, bufLen, 0, (struct sockaddr *)&sockAddr, &sockLen);
			}
			else
			{
				assert(false);
				return -1;
			}
			
			return retVal;
		}
		else if (-1 == retVal)
		{
			assert(false);
			return -1;
		}

		recvTime -= 50;
	}

	//assert(false);
	return 0;
}

SWL_socket_t CRTPSock::GetSockfd()
{
	return m_socketfd;
}

unsigned short CRTPSock::GetPort()
{
	return m_port;
}

unsigned short CRTPSock::GetRemotePort()
{
	return m_RemotePort;
}

int CRTPSock::CreateUDPSock()
{
	m_socketfd = SWL_CreateSocket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (SWL_INVALID_SOCKET == m_socketfd)
	{
		assert(false);
		return -1;
	}

	if (0 != bind(m_socketfd, (const struct sockaddr *)&m_sockAddr, sizeof(struct sockaddr_in)))
	{
		SWL_CloseSocket(m_socketfd);
		m_socketfd = SWL_INVALID_SOCKET;

		assert(false);
		return -1;
	}
	
	return 0;
}

int CRTPSock::CreateTCPSock()
{
	m_socketfd = SWL_CreateSocket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (SWL_INVALID_SOCKET == m_socketfd)
	{
		assert(false);
		return -1;
	}

	if (0 != SWL_Connect(m_socketfd, (const struct sockaddr *)&m_sockAddr, sizeof(struct sockaddr_in), 10))
	{
		SWL_CloseSocket(m_socketfd);
		m_socketfd = SWL_INVALID_SOCKET;

		assert(false);
		return -1;
	}

	return 0;
}
