// Multicast.h: interface for the CMulticast class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MULTICAST_H__C89EFE69_453F_44FD_9D49_C4C51CE78EF2__INCLUDED_)
#define AFX_MULTICAST_H__C89EFE69_453F_44FD_9D49_C4C51CE78EF2__INCLUDED_

#include "PUB_common.h"
#include "SWL_Public.h"
#include <vector>

using std::vector;

class CMulticast  
{
public:
	CMulticast();
	~CMulticast();

	int Init(unsigned short wSendPort, unsigned short wRecvPort, const char *pMulticastip, const char *pIfName = "eth0");
	int Quit();
	int AddRecvGroup(const char *pMulticastip, unsigned short wRecvPort);
	
	int SendTo(const void *pBuff, int iLen);
	int RecvFrom(void *pBuff, int iLen);

private:
	SWL_socket_t CreateReceiveSock(sockaddr_in &sockAddrIn);
	SWL_socket_t CreateSendSock(sockaddr_in &sockAddrIn);
	void CloseAllSocket();
	bool ReCreateAllSocket();

private:
	struct sockaddr_in		m_MultiCastPeerAddr;
	struct ip_mreq			m_mreq;
	SWL_socket_t			m_MultiCastSocketSend;
	sockaddr_in				m_sendSockaddrIn;
	vector<SWL_socket_t>	m_MultiCastSocketRecv;
	vector<sockaddr_in>		m_recvSockaddrIn;

	char					m_MultiCastBuf[1024];
	PUB_lock_t				m_lock;
	bool					m_bReady;
	char					m_szIfName[16];
	SWL_socket_t			m_maxFdValue;

	fd_set					m_recvSet;
};

#endif // !defined(AFX_MULTICAST_H__C89EFE69_453F_44FD_9D49_C4C51CE78EF2__INCLUDED_)
