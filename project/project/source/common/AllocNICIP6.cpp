#include "AllocNICIP6.h"

#include <net/if.h>
#include <net/if_arp.h>
#include <netinet/in.h>
//#include <net/ipv6.h>

CAllocNicIP6 * CAllocNicIP6::m_pAllocNicIPv6 = NULL;
CAllocNicIP6::CAllocNicIP6()
{

}

CAllocNicIP6::~CAllocNicIP6()
{

}


CAllocNicIP6 * CAllocNicIP6::Instance()
{
	if (NULL == m_pAllocNicIPv6)
	{
		m_pAllocNicIPv6 = new CAllocNicIP6;
	}

	return m_pAllocNicIPv6;
}

bool CAllocNicIP6::Initial()
{
	memset(&m_networkAddrInfo, 0, sizeof(m_networkAddrInfo));

	return true;
}

void CAllocNicIP6::Quit()
{

}

int CAllocNicIP6::AllocNICIP(const NETWORK_ADDR_INFO_IPv6 * pNetworkAddrInfo)
{
	if (0 == GetIPDhcp(pNetworkAddrInfo) || 0 == GetIPStatic(pNetworkAddrInfo))
	{
		printf("%s:%s:%d, Running at here:%s:%d\n", __FUNCTION__, __FILE__, __LINE__, pNetworkAddrInfo->IP, pNetworkAddrInfo->subnetMask);
		if (0 < strlen((const char *)m_networkAddrInfo.IP) && 0 != strcmp((const char *)pNetworkAddrInfo->IP, (const char *)m_networkAddrInfo.IP))
		{
			//删除原来的地址， IPV6可以配置4个ip地址
			DelIPStatic(&m_networkAddrInfo);
		}

		m_networkAddrInfo = * pNetworkAddrInfo;
		
		return 0;
	}

	return -1;
}

int CAllocNicIP6::TestNetwork(const NETWORK_ADDR_INFO_IPv6 * pNetworkAddrInfo)
{
	if (0 == GetIPDhcp(pNetworkAddrInfo) || 0 == GetIPStatic(pNetworkAddrInfo))
	{
		return 0;
	}
	
	return -1;
}

int CAllocNicIP6::GetIPDhcp(const NETWORK_ADDR_INFO_IPv6 * pNetworkAddrInfo)
{
	if (NULL == pNetworkAddrInfo || !pNetworkAddrInfo->useDHCP)
	{
		return -1;
	}

	return -1;
}

int CAllocNicIP6::GetIPStatic(const NETWORK_ADDR_INFO_IPv6 * pNetworkAddrInfo)
{
	if (0 == ConfigIPStatic(pNetworkAddrInfo, SIOCSIFADDR))
	{
		//设置网关
		ConfigGetway(pNetworkAddrInfo->gateway);
		return 0;
	}

	return -1;
}


int CAllocNicIP6::DelIPStatic(const NETWORK_ADDR_INFO_IPv6 * pNetworkAddrInfo)
{
	return ConfigIPStatic(pNetworkAddrInfo, SIOCDIFADDR);
}

int CAllocNicIP6::ConfigIPStatic(const NETWORK_ADDR_INFO_IPv6 * pNetworkAddrInfo, unsigned long ioctlCMD)
{
	struct sockaddr_in6 sin6;
	struct in6_ifreq ifr6;

	sin6.sin6_family = AF_INET6;
	int sockfd6 = socket(AF_INET6, SOCK_DGRAM, IPPROTO_UDP);
	if (sockfd6 < 0)
	{
		printf("%s:%s:%d, create ipv6 socket error\n", __FUNCTION__, __FILE__, __LINE__);
		return -1;
	}

	if (0 >= inet_pton(AF_INET6, (const char * )pNetworkAddrInfo->IP, (char *)&ifr6.ifr6_addr))
	{
		perror("inet_pton()");
		printf("%s:%s:%d, get ip error IP=%s\n", __FUNCTION__, __FILE__, __LINE__,pNetworkAddrInfo->IP);
		return -1;
	}

	struct ifreq ifr;
	memset(&ifr, 0, sizeof(struct ifreq));
	strncpy(ifr.ifr_name, "eth0", sizeof("eth0"));
	if (0 != ioctl(sockfd6, SIOGIFINDEX, &ifr))
	{
		perror("ioctl");
		printf("%s:%s:%d, Get interface information eror\n", __FUNCTION__, __FILE__, __LINE__);
		return -1;
	}

	ifr6.ifr6_ifindex = ifr.ifr_ifindex;
	ifr6.ifr6_prefixlen = pNetworkAddrInfo->subnetMask;//子网掩码长度;

	if (0 != ioctl(sockfd6, ioctlCMD, &ifr6))
	{
		perror("ioctl");
		printf("%s:%s:%d, set ipv6 addr error\n", __FUNCTION__, __FILE__, __LINE__);
		return -1;
	}

	printf("%s:%s:%d, Running at here:%s:%d\n", __FUNCTION__, __FILE__, __LINE__, pNetworkAddrInfo->IP, pNetworkAddrInfo->subnetMask);
	return 0;
}


int CAllocNicIP6::ConfigGetway(const unsigned char * pGateway)
{
	struct sockaddr_in6 sa6;
	memset(&sa6, 0, sizeof(sa6));

	struct in6_rtmsg rt;
	memset(&rt, 0, sizeof(rt));

	//struct in6_rtmsg
	//{
	//	struct in6_addr rtmsg_dst;
	//	struct in6_addr rtmsg_src;
	//	struct in6_addr rtmsg_gateway;
	//	u_int32_t rtmsg_type;
	//	u_int16_t rtmsg_dst_len;
	//	u_int16_t rtmsg_src_len;
	//	u_int32_t rtmsg_metric;
	//	unsigned long int rtmsg_info;
	//	u_int32_t rtmsg_flags;
	//	int rtmsg_ifindex;
	//};

	//路由默认目的地为0
	memcpy(&rt.rtmsg_dst, sa6.sin6_addr.s6_addr, sizeof(struct in6_addr));
	rt.rtmsg_dst_len	= 0;	//默认路由子网掩码不配置

	rt.rtmsg_flags		= RTF_UP;
	//rt.rtmsg_metric	= 1;

	if (0 >= inet_pton(AF_INET6, (const char * )pGateway, (char *)&sa6.sin6_addr))
	{
		printf("%s:%s:%d, get ip error\n", __FUNCTION__, __FILE__, __LINE__);
		return -1;
	}
	memcpy(&rt.rtmsg_gateway, &sa6.sin6_addr,sizeof(struct in6_addr));
	rt.rtmsg_flags |= RTF_GATEWAY;

	/* Create a socket to the INET6 kernel. */
	int skfd = socket(AF_INET6, SOCK_DGRAM, 0);
	if (0 >= skfd)
	{
		perror("socket");
		printf("%s:%s:%d, create socket error\n", __FUNCTION__, __FILE__, __LINE__);
		return -1;
	}
	
	struct ifreq ifr;
	memset(&ifr, 0, sizeof(ifr));
	strncpy(ifr.ifr_name, "eth0", strlen("eth0"));
	if (0 != ioctl(skfd, SIOGIFINDEX, &ifr))
	{
		perror("ioctl:");
		printf("%s:%s:%d ioctl error\n", __FUNCTION__, __FILE__, __LINE__);
		close(skfd);
		return -1;
	}
	rt.rtmsg_ifindex = ifr.ifr_ifindex;

	//添加路由
	if (0 != ioctl(skfd, SIOCADDRT, &rt))
	{
		perror("ioctl:");
		printf("%s:%s:%d ioctl error\n", __FUNCTION__, __FILE__, __LINE__);
		close(skfd);
		return -1;
	}

	//删除路由
	//ioctl(skfd, SIOCDELRT, &rt);

	close(skfd);

	return 0;
}

