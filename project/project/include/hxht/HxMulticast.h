// Multicast.h: interface for the CMulticast class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MULTICAST_H__C89EFE69_453F_44FD_9D49_C4C51CE78EF2__INCLUDED_)
#define AFX_MULTICAST_H__C89EFE69_453F_44FD_9D49_C4C51CE78EF2__INCLUDED_

#include <netinet/in.h>
#include "PUB_common.h"
#include "Typedef.h"

#include "HxMulticastHeader.h"

const int MULTICAST_INFOLEN=sizeof(MULTICAST_INFO);

class CMulticast  
{
public:
	CMulticast();
	~CMulticast();

	int Init(unsigned short wSendPort, unsigned short wRecvPort, const char *pMulticastip, const char *pIfName = "eth0");
	int Quit();
	
	int SendTo(const void *pBuff, int iLen);
	int RecvFrom(void *pBuff, int iLen);

private:
	struct sockaddr_in	m_MultiCastPeerAddr;
	struct ip_mreqn		m_mreq;
	int m_MultiCastSocketSend;
	int m_MultiCastSocketRecv;
	char m_MultiCastBuf[1024];
	MULTICAST_INFO m_MultiCastInfo;
	PUB_lock_t			m_lock;
	bool m_bReady;
};

#endif // !defined(AFX_MULTICAST_H__C89EFE69_453F_44FD_9D49_C4C51CE78EF2__INCLUDED_)
