/////////////////////////////////////////////////////////////////////////////
#ifndef _RTP_SOCK_H_
#define _RTP_SOCK_H_

#include "SWL_Public.h"
//#include "TVT_PubCom.h"			//3.0不包含   3.0添加DD_Public 或 PUB_common
#include "PUB_common.h"
#include "SWL_TypeDefine.h"

class CRTPSock
{
public:
	CRTPSock(struct sockaddr_in sockAddr, SWL_socket_t sockfd, int protocol);
	CRTPSock();
	~CRTPSock();

	int SetSockfd(struct sockaddr_in sockAddr, SWL_socket_t sockfd, int protocol);
	int SetPort(unsigned short port);
	int SetPort(unsigned short port, unsigned short RemotePort);
	void SetServerPort(unsigned short port)	{m_port = port;}
	void SetClientPort(unsigned short port)	{m_RemotePort = port;}
	unsigned short GetPort();
	unsigned short GetRemotePort();
	int SetIP(u_long sinAaddr);
	void SetSockfd(SWL_socket_t sockfd){m_socketfd = sockfd;}
	u_long GetIP();
	u_long GetDestIP();
	int CreateSock(int protocol);
	int CreateSock(unsigned short port, u_long sinAddr=INADDR_ANY, int protocol=IPPROTO_TCP);
	int CreateSock(unsigned short port, unsigned short RemotePort, u_long sinAddr=INADDR_ANY, int protocol=IPPROTO_TCP);
	void CloseSock();
	
	//int SendData(const char *pSendData, unsigned long dataLen);
	int SendData(const char *pSendData, unsigned long dataLen, int sendTime);

	int RecvData(char *pRecvBuf, unsigned long bufLen);
	int RecvData(char *pRecvBuf, unsigned long bufLen, int recvTime);

	SWL_socket_t GetSockfd();

private:
	
	int CreateUDPSock();
	int CreateTCPSock();
	
	SWL_socket_t		m_socketfd;
	unsigned short		m_port;
	unsigned short		m_RemotePort;
	u_long				m_DestAddr;
	struct sockaddr_in	m_sockAddr;
	int					m_protocol;
};
#endif /*_RTP_UDP_SOCK_H_*/
/////////////////////////////////////////////////////////////////////////////
