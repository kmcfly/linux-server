#ifndef _ALLOC_NIC_IP6_H_
#define _ALLOC_NIC_IP6_H_

#include "dvrdvsdef.h"
#include "NetProtocol.h"
#include "NetInterface.h"
#include "PUB_common.h"
#include "PPPoeMng.h"

struct in6_ifreq {
	struct in6_addr ifr6_addr;
	unsigned long ifr6_prefixlen;
	unsigned long ifr6_ifindex;
};

class CAllocNicIP6
{
public:
	static CAllocNicIP6 * Instance();
	~CAllocNicIP6();

	bool Initial();
	void Quit();
	int AllocNICIP(const NETWORK_ADDR_INFO_IPv6 * pNetworkAddrInfo);
	int TestNetwork(const NETWORK_ADDR_INFO_IPv6 * pNetworkAddrInfo);
private:
	CAllocNicIP6();

	int GetIPDhcp(const NETWORK_ADDR_INFO_IPv6 * pNetworkAddrInfo);
	int GetIPStatic(const NETWORK_ADDR_INFO_IPv6 * pNetworkAddrInfo);
	int DelIPStatic(const NETWORK_ADDR_INFO_IPv6 * pNetworkAddrInfo);
	int ConfigIPStatic(const NETWORK_ADDR_INFO_IPv6 * pNetworkAddrInfo, unsigned long ioctlCMD);
	int ConfigGetway(const unsigned char * pGateway);
private:
	static CAllocNicIP6 * m_pAllocNicIPv6;

	NETWORK_ADDR_INFO_IPv6 m_networkAddrInfo;
};


#endif
