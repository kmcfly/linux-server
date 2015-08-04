/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : 袁石维
** Date         : 2009-09-25
** Name         : NetInterface.cpp
** Version      : 1.0
** Description  :
** Modify Record:
1:
***********************************************************************/
#include "NetInterface.h"
#include "PPPoeMng.h"
#include <bits/endian.h>
#include <byteswap.h>
#include <resolv.h>

const unsigned char datagramhead[12] = {0x2b,0x50,0x01,0x00,0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x00};
const unsigned char datagramend[4]   = {0x00,0x01,0x00,0x01};

//////////////////////////////////////////////////////////////////////////
CNetInterface * CNetInterface::Instance()
{
	static CNetInterface s_netInterface;
	return &s_netInterface;
}

CNetInterface::CNetInterface() : m_fd(-1)
{
	memset(&m_localNICStatusInfo, 0, sizeof(NETWORK_STATUS_INFO));
}

CNetInterface::~CNetInterface()
{

}

bool CNetInterface::Initial()
{
	if (-1 != m_fd) 
	{
		assert(false);
		return true;
	}

	m_fd = socket (AF_INET, SOCK_DGRAM, 0);
	if (m_fd < 0) 
	{
		m_fd = -1;
		return false;
	}
	assert(-1 != m_fd);

	return true;
}

void CNetInterface::Quit()
{
	if ( -1 != m_fd )
	{
		close(m_fd);
		m_fd = -1;
	}
}
//////////////////////////////////////////////////////////////////////////
#if 0
unsigned char CNetInterface::EnumInterface()
{
	unsigned char intrface = 0;
	struct ifconf ifc;

	if (m_fd >=0 ) 
	{
		ifc.ifc_len = sizeof(ifreq)*MAX_INTERFACE_NUM;
		ifc.ifc_buf = reinterpret_cast<caddr_t>(m_InterfaceBuf);

		if (!ioctl (m_fd, SIOCGIFCONF, (char *) &ifc)) 
		{
			intrface = static_cast<unsigned char>(ifc.ifc_len / sizeof (struct ifreq));
			return intrface;
		}
		else
		{
			printf("EnumInterface(%s  %d) failed:%s\n", __FILE__,__LINE__, strerror(errno));
			return 0;
		}
	}
	else
	{
		printf("EnumInterface(%s  %d) failed:%s\n", __FILE__,__LINE__, strerror(errno));
		return 0;
	}
}
#endif

//liujiang
#if 0
bool CNetInterface::GetInterfaceName(char *pName, unsigned short len, unsigned char index)
{
	assert (index < MAX_INTERFACE_NUM);
	assert (64 < len);

	if (index < m_interfaceNum)
	{
		strcpy(pName, m_InterfaceBuf[index].ifr_name);
		return true;
	}

	return false;
}
#endif

//liujiang
bool CNetInterface::UpInterface(const char* name, unsigned short len)
{	
	assert (len > 0);
	assert (strlen(name) < IFNAMSIZ);
	assert (strlen(name) <= len);

	struct   ifreq   ifr;  
	memset(&ifr,0,sizeof(ifr));
	strncpy(ifr.ifr_name,   name,   IFNAMSIZ);  
	ifr.ifr_name[IFNAMSIZ- 1]   =   0; 
	ifr.ifr_flags |=  IFF_UP | IFF_RUNNING;

	if (ioctl(m_fd, SIOCSIFFLAGS, &ifr) < 0)    
	{  
		printf("UpInterface(%s  %d) failed:%s\n",__FILE__,__LINE__,strerror(errno));
		return false;
	}  	
	return true;	
}

bool CNetInterface::DownInterface(const char* name, unsigned short len)
{
	assert (len > 0);
	assert (strlen(name) < IFNAMSIZ);
	assert (strlen(name) <= len);

	struct   ifreq   ifr;  
	memset(&ifr,0,sizeof(ifr));
	strncpy(ifr.ifr_name,   name,   IFNAMSIZ);  
	ifr.ifr_name[IFNAMSIZ- 1]   =   0; 
	SetIP(0, name, len);
	ifr.ifr_flags &= ~(IFF_UP | IFF_RUNNING);
	if (ioctl(m_fd, SIOCSIFFLAGS, &ifr) < 0)    
	{  
		printf("DownInterface(%s  %d) failed:%s\n",__FILE__,__LINE__,strerror(errno));
		return false;
	}  		
	return true;
}

#if 0
int CNetInterface::GetIndexByName(const char* name, unsigned short len)
{
	assert (len > 0);
	assert (strlen(name) < IFNAMSIZ);
	assert (strlen(name) <= len);

	//检查名称的合法性
	bool bValidName = false;
	for (unsigned char i=0; i<m_interfaceNum; ++i)
	{
		if (0 == strcmp(name, m_InterfaceBuf[i].ifr_name))
		{
			bValidName = true;
			return i;
		}
	}
	
	return -1;
}
#endif

NET_INTERFACE_STATUS CNetInterface::GetInterfaceStatus(const char* name/* = "eth0"*/, unsigned short len/* = 4*/)
{
	assert (len > 0);
	assert (strlen(name) < IFNAMSIZ);
	assert (strlen(name) <= len);

	struct   ifreq   ifr;  
	memset(&ifr,0,sizeof(ifr));
	strncpy(ifr.ifr_name,   name,   IFNAMSIZ);  
	ifr.ifr_name[IFNAMSIZ- 1] = 0; 

	if (!(ioctl(m_fd, SIOCGIFFLAGS, (char *)&ifr)) )
	{
		if (ifr.ifr_flags & IFF_UP ) 
		{
			return NET_INTERFACE_UP;
		}
		else
		{
			return NET_INTERFACE_DOWN;
		}
	}
	printf("GetInterfaceStatus(%s  %d) failed:%s\n",__FILE__,__LINE__,strerror(errno));
	return NET_INTERFACE_ERROR;
}

bool CNetInterface::IPIsCollision(unsigned long ip, const char* name/* = "eth0"*/, unsigned short len/* = 4*/)
{
	u_long	hostIp = ip;			//记录本地IP
	u_char  hostMac[6] = {0};		//记录本地MAC

	int fd_arp = socket(AF_INET, SOCK_PACKET, htons(0x0806));
	if( fd_arp < 0 )
	{
		printf("(%s %d) Failed: open socket failed\n",__FILE__,__LINE__);
		return false;  
	}

	struct  ifreq   ifr;  
	strcpy(ifr.ifr_name,  name);
	ifr.ifr_addr.sa_family = AF_INET;

	//获取本机IP地址
	if(ioctl(fd_arp, SIOCGIFADDR,  &ifr)<0) 
	{
		printf("(%s %d) Attention: no ip binded to interface %s\n",__FILE__,__LINE__,name);
	} 
	else
	{
		struct  sockaddr_in    *sin_ptr=(struct  sockaddr_in*)&ifr.ifr_addr; 
		hostIp = sin_ptr->sin_addr.s_addr;
	}

	if (hostIp == ip)
	{
		hostIp = 0;
	}

	//获取本机MAC地址  
	if (ioctl(fd_arp, SIOCGIFHWADDR, &ifr)<0)  
	{  
		printf("(%s %d) Error: no mac binded to interface %s\n",__FILE__,__LINE__,name);
		close(fd_arp);
		return false;
	}
	else
	{
		u_char * ptr=(u_char *)&ifr.ifr_ifru.ifru_hwaddr.sa_data[0]; 
		memcpy(hostMac, ptr, 6);
	}
	

	//构造ARP请求报文
	ARP_PACKET arp_packets;
	memset(&arp_packets.target_Mac,0xff,6);
	memcpy(&arp_packets.send_Mac,hostMac,6);
	arp_packets.ethnet_type = htons(0x0806);
	arp_packets.had_Type = htons(0x0001);
	arp_packets.pro_Type = htons(0x0800);
	arp_packets.had_Size = 0x06;
	arp_packets.pro_Size = 0x04;
	arp_packets.operate =  htons(ARP_REQUEST);
	memcpy(&arp_packets.send_Mac1,hostMac,6);
	arp_packets.send_Ip  = hostIp;
	arp_packets.target_ip = ip;
	memset(&arp_packets.target_Mac1,0,6);

	struct sockaddr to;
	memset(&to,0,sizeof(to));
	strcpy(to.sa_data, name); 

	//设置超时时间
	timeval timeout;
	timeout.tv_sec = 1;
	timeout.tv_usec = 0;
	fd_set   rdfd;
	bool isCollision = false;

	for (int timer = 0; timer<5 ;timer++) 
	{
		int len = sendto(fd_arp,&arp_packets,sizeof(arp_packets),0,&to,sizeof(to) );
		if( len < 0 )
		{  
			//printf("(%s %d) Failed: sendto failed\n",__FILE__,__LINE__);
			continue;
		}   

		FD_ZERO(&rdfd);
		FD_SET(fd_arp, &rdfd);
		len = select(fd_arp+1, &rdfd, NULL, NULL, &timeout);
		if( len > 0 )
		{
			socklen_t fromlen = 0;
			ARP_PACKET arp_packet_rev;
			memset(&arp_packet_rev,0,sizeof(arp_packet_rev));		
			len = recvfrom(fd_arp,&arp_packet_rev,sizeof(arp_packet_rev), 0,&to,&fromlen);
			if( ( ARP_REPLAY==ntohs(arp_packet_rev.operate) ) && (arp_packet_rev.send_Ip == ip ) )
			{
				long ipReply = arp_packet_rev.send_Ip;				
				printf("(%s %d) Send    Arp request ip=%lu.%lu.%lu.%lu\n",__FILE__,__LINE__,(ip&0xff),((ip>>8)&0xff),((ip>>16)&0xff),((ip>>24)&0xff));
				printf("(%s %d) Receive Arp reply,  ip=%lu.%lu.%lu.%lu, ",__FILE__,__LINE__,(ipReply&0xff),((ipReply>>8)&0xff),((ipReply>>16)&0xff),((ipReply>>24)&0xff));
				printf(" mac=%02x.%02x.%02x.%02x.%02x.%02x\n",arp_packet_rev.send_Mac1[0],arp_packet_rev.send_Mac1[1],arp_packet_rev.send_Mac1[2],arp_packet_rev.send_Mac1[3],arp_packet_rev.send_Mac1[4],arp_packet_rev.send_Mac1[5]);
				printf("(%s %d) This ip has been used: %lu.%lu.%lu.%lu\n",__FILE__,__LINE__,(ip&0xff),((ip>>8)&0xff),((ip>>16)&0xff),((ip>>24)&0xff));
				isCollision  = true;
				break;
			}
		}
		else
		{
			//printf("(%s %d) Failed: select timeout\n",__FILE__,__LINE__);
		}
	}

	close(fd_arp);	
	return isCollision;
}

IP_SET_ERR CNetInterface::SetIP(unsigned long ip, const char* name/* = "eth0"*/, unsigned short len/* = 4*/)
{
	if (IPIsCollision(ip, name, len))
	{
		return IP_SET_ERR_COLLISION;
	}
	else
	{
		assert (len > 0);
		assert (strlen(name) < IFNAMSIZ);
		assert (strlen(name) <= len);

		struct   ifreq   ifr;  
		memset(&ifr,0,sizeof(ifr));
		strncpy(ifr.ifr_name,   name,   IFNAMSIZ);  
		ifr.ifr_name[IFNAMSIZ- 1]   =   0; 

		ioctl(m_fd, SIOCGIFFLAGS, &ifr);	
		if(!(ifr.ifr_flags & IFF_UP))	
		{		
			ifr.ifr_flags = IFF_UP;		
			if(0 != ioctl(m_fd, SIOCSIFFLAGS, &ifr))		
			{		
				return IP_SET_ERR_NO_DEVICE;		
			}			
		}

		struct   sockaddr_in   sin;  
		memset(&sin,   0,   sizeof(sin));  
		sin.sin_family   =   AF_INET;  
		sin.sin_addr.s_addr   =  ip;  
		memcpy(&ifr.ifr_addr,   &sin,   sizeof(sin));  

		if (ioctl(m_fd, SIOCSIFADDR,   &ifr) < 0)  
		{
			printf("SetIpAddress(%s  %d) failed:%s\n",__FILE__,__LINE__,strerror(errno));
			return IP_SET_ERR_INVALID;
		}  

		return IP_SET_ERR_SUCC;
	}
}

bool CNetInterface::GetIPAddress(unsigned long &ip, const char* name/* = "eth0"*/, unsigned short len/* = 4*/)
{
	assert (len > 0);
	assert (strlen(name) < IFNAMSIZ);
	assert (strlen(name) <= len);

	struct   ifreq   ifr;  
	memset(&ifr,0,sizeof(ifr));
	strncpy(ifr.ifr_name,   name,   IFNAMSIZ);  
	ifr.ifr_name[IFNAMSIZ- 1] = 0; 

	if (!(ioctl(m_fd, SIOCGIFFLAGS, (char *)&ifr)))
	{
		if(IFF_UP & ifr.ifr_flags)
		{
			if (!(ioctl(m_fd, SIOCGIFADDR, (char *)&ifr)) )
			{
				ip = ((sockaddr_in*)&ifr.ifr_addr)->sin_addr.s_addr;
				return true;
			}
			else
			{
				return false;						
			}
		}
		else
		{
			return false;
		}
	}		
	//	printf("GetIpaddress(%s  %d) failed:%s\n",__FILE__,__LINE__,strerror(errno));
	return false;
}

bool CNetInterface::GetIPText(char *pIPText, unsigned short textLen, const char* name/* = "eth0"*/, unsigned short len/* = 4*/)
{
	assert (NULL != pIPText);
	assert (textLen > 15);
	assert (len > 0);
	assert (strlen(name) < IFNAMSIZ);
	assert (strlen(name) <= len);
	struct   ifreq   ifr;  
	memset(&ifr,0,sizeof(ifr));
	strncpy(ifr.ifr_name,   name,   IFNAMSIZ);  
	ifr.ifr_name[IFNAMSIZ- 1] = 0; 

	if (!(ioctl(m_fd, SIOCGIFFLAGS, (char *)&ifr)))
	{
		if(IFF_UP & ifr.ifr_flags)
		{
			if (!(ioctl(m_fd, SIOCGIFADDR, (char *)&ifr)) )
			{
				unsigned long ip = ((sockaddr_in*)&ifr.ifr_addr)->sin_addr.s_addr;
#if defined(__TDFH__) || defined(__CHIPTI__) || defined(__CHIPGM__)
				sprintf(pIPText, "%d.%d.%d.%d", (ip&0xff), (ip>>8)&0xff, (ip>>16)&0xff, (ip>>24)&0xff);
#else
				sprintf(pIPText, "%03d.%03d.%03d.%03d", (ip&0xff), (ip>>8)&0xff, (ip>>16)&0xff, (ip>>24)&0xff);
#endif
				return true;				
			}
			else
			{
				return false;						
			}
		}
		else
		{
			return false;
		}
	}	
	//	printf("GetIpaddress(%s  %d) failed:%s\n",__FILE__,__LINE__,strerror(errno));
	return false;	
}

bool  CNetInterface::SetIPMask(unsigned long ipMask, const char* name/* = "eth0"*/, unsigned short len/* = 4*/)
{
	assert (len > 0);
	assert (strlen(name) < IFNAMSIZ);
	assert (strlen(name) <= len);


	struct   ifreq   ifr;  
	memset(&ifr, 0, sizeof(ifr));

	strcpy(ifr.ifr_name, name);

	struct   sockaddr_in   sin;  
	memset(&sin,   0,   sizeof(sin));  
	sin.sin_family   =   AF_INET;  
	sin.sin_addr.s_addr   =  ipMask;  
	memcpy(&ifr.ifr_addr,   &sin,   sizeof(sin));  

	if (ioctl(m_fd, SIOCSIFNETMASK,   &ifr) < 0)  
	{
		printf("SetIpaddressMask(%s  %d) failed:%s\n",__FILE__,__LINE__,strerror(errno));
		return false;
	}  

	return true;

}

bool CNetInterface::GetIPMask(char *pIPMaskText, unsigned short textLen, const char* name/* = "eth0"*/, unsigned short len/* = 4*/)
{
	assert (NULL != pIPMaskText);
	assert (textLen > 15);
	assert (len > 0);
	assert (strlen(name) < IFNAMSIZ);
	assert (strlen(name) <= len);

	struct   ifreq   ifr;  
	memset(&ifr,0,sizeof(ifr));
	strncpy(ifr.ifr_name,   name,   IFNAMSIZ);  
	ifr.ifr_name[IFNAMSIZ- 1] = 0; 
	if (!(ioctl(m_fd, SIOCGIFNETMASK, (char *)&ifr)) )
	{
		unsigned long ip = ((sockaddr_in*)&ifr.ifr_addr)->sin_addr.s_addr;
#if defined (__TDFH__) || defined(__CHIPTI__) || defined(__CHIPGM__)
		sprintf(pIPMaskText, "%d.%d.%d.%d", (ip&0xff), (ip>>8)&0xff, (ip>>16)&0xff, (ip>>24)&0xff);
#else
		sprintf(pIPMaskText, "%03d.%03d.%03d.%03d", (ip&0xff), (ip>>8)&0xff, (ip>>16)&0xff, (ip>>24)&0xff);
#endif
		return true;
	}

	printf("GetIpaddressMask(%s  %d) failed:%s\n",__FILE__,__LINE__,strerror(errno));
	return false;
}


bool CNetInterface::GetIPMaskUL(unsigned long  &ipmask, const char* name /*= "eth0"*/, unsigned short len /*= 4*/)
{
	assert (strlen(name) < IFNAMSIZ);
	assert (strlen(name) <= len);

	struct   ifreq   ifr;  
	memset(&ifr, 0, sizeof(ifr));

	strncpy(ifr.ifr_name, name, IFNAMSIZ);
	ifr.ifr_name[IFNAMSIZ-1]='\0';

	if (!(ioctl(m_fd, SIOCGIFNETMASK, (char *)&ifr)) )
	{
		ipmask = ((sockaddr_in*)&ifr.ifr_netmask)->sin_addr.s_addr;
		return true;
	}


	printf("GetIpaddressMask(%s  %d) failed:%s\n",__FILE__,__LINE__,strerror(errno));
	return false;

}

//buglj pMac不是字符串则这个函数正确
bool CNetInterface::SetMAC(char* pMac, const char* name/* = "eth0"*/, unsigned short len/* = 4*/)
{
	assert (len > 0);
	assert (strlen(name) < IFNAMSIZ);
	assert (strlen(name) <= len);

	struct   ifreq   ifr;  
	memset(&ifr,0,sizeof(ifr));
	strncpy(ifr.ifr_name,   name,   IFNAMSIZ);  
	ifr.ifr_name[IFNAMSIZ- 1]   =   0; 

	struct   sockaddr   sin;  
	memset(&sin,   0,   sizeof(sin));  
	sin.sa_family   =   ARPHRD_ETHER;  
	memcpy(sin.sa_data,pMac,6);
	memcpy(&ifr.ifr_hwaddr,   &sin,   sizeof(sin));  

	if (ioctl(m_fd, SIOCSIFHWADDR,   &ifr) < 0)  
	{
		printf("SetMacAddress(%s  %d) failed:%s\n",__FILE__,__LINE__,strerror(errno));
		return false;
	} 

	return true;
}

//buglj pMac不是字符串则这个函数正确
bool CNetInterface::GetMAC(char* pMac, const char* name/* = "eth0"*/, unsigned short len/* = 4*/)
{
	assert (len > 0);
	assert (strlen(name) < IFNAMSIZ);
	assert (strlen(name) <= len);

	struct   ifreq   ifr;  
	memset(&ifr,0,sizeof(ifr));
	strncpy(ifr.ifr_name,   name,   IFNAMSIZ);  
	ifr.ifr_name[IFNAMSIZ- 1] = 0; 

	if (!(ioctl (m_fd, SIOCGIFHWADDR, (char *)&ifr)) )
	{
		sockaddr *pSocketAddress = &ifr.ifr_hwaddr;
		char* macAddressTemp = (char*)(pSocketAddress->sa_data);
		memcpy(pMac,macAddressTemp,6);
		//		printf("%02x-%02x-%02x-%02x-%02x-%02x",macAddressTemp[0],macAddressTemp[1],macAddressTemp[2],macAddressTemp[3],macAddressTemp[4],macAddressTemp[5]);
		return true;
	}

	printf("GetMACaddress(%s  %d) failed:%s\n",__FILE__,__LINE__,strerror(errno));
	return false;
}

bool  CNetInterface::SetGateway(unsigned long  gateway, unsigned long  target, unsigned long  targetMask, const char* name/* = "eth0"*/)
{
	struct rtentry rm;
	memset(&rm, 0, sizeof(rm));

	struct sockaddr_in ic_gateway ;	
	ic_gateway.sin_family = AF_INET; 
	ic_gateway.sin_addr.s_addr = gateway;
	ic_gateway.sin_port = 0; 
	(( struct sockaddr_in*)&rm.rt_dst)->sin_family = AF_INET;

	if (0!=target) 
	{
		(( struct sockaddr_in*)&rm.rt_dst)->sin_addr.s_addr = target;
	}

	(( struct sockaddr_in*)&rm.rt_dst)->sin_port = 0;
	(( struct sockaddr_in*)&rm.rt_genmask)->sin_family = AF_INET;

	if (0!=targetMask) 
	{
		(( struct sockaddr_in*)&rm.rt_genmask)->sin_addr.s_addr = targetMask;
	}

	(( struct sockaddr_in*)&rm.rt_genmask)->sin_port = 0;
	memcpy((void *) &rm.rt_gateway, &ic_gateway, sizeof(ic_gateway));
	rm.rt_flags = RTF_UP | RTF_GATEWAY;
	rm.rt_dev = (char *)name;

	//先删除默认网关
	if (ioctl(m_fd, SIOCDELRT, &rm) < 0)  
	{
	}

	//添加默认网关
	if (ioctl(m_fd, SIOCADDRT, &rm) < 0)  
	{
		printf("SetDefaultRoute(%s  %d) failed:%s\n",__FILE__,__LINE__,strerror(errno));
		return false;
	}  

	return true;
}

bool  CNetInterface::DelGateway(unsigned long gateway, unsigned long target, unsigned long targetMask)
{
	struct rtentry rm;
	memset(&rm, 0, sizeof(rm));

	struct sockaddr_in ic_gateway ;	
	ic_gateway.sin_family = AF_INET; 
	ic_gateway.sin_addr.s_addr = gateway;
	ic_gateway.sin_port = 0; 

	(( struct sockaddr_in*)&rm.rt_dst)->sin_family = AF_INET;

	if (0!=target) 
	{
		(( struct sockaddr_in*)&rm.rt_dst)->sin_addr.s_addr = target;
	}

	(( struct sockaddr_in*)&rm.rt_dst)->sin_port = 0;
	(( struct sockaddr_in*)&rm.rt_genmask)->sin_family = AF_INET;

	if (0!=targetMask) 
	{
		(( struct sockaddr_in*)&rm.rt_genmask)->sin_addr.s_addr = targetMask;
	}

	(( struct sockaddr_in*)&rm.rt_genmask)->sin_port = 0;
	memcpy((void *) &rm.rt_gateway, &ic_gateway, sizeof(ic_gateway));
	rm.rt_flags = RTF_UP | RTF_GATEWAY;

	if (ioctl(m_fd, SIOCDELRT, &rm) < 0)  
	{
		printf("DelDefaultRoute(%s  %d) failed:%s\n",__FILE__,__LINE__,strerror(errno));
		return false;
	}  

	return true;
}

bool CNetInterface::GetGateway(char*  pGateway, char*  pTarget, char* pTargetMask, const char* name/* = "eth0"*/)
{
	char buffer[200] = {0};

	unsigned long defaultRoutePara[4] = {0};
	FILE* p_fd = fopen(_PATH_ROUTE, "r");
	if (NULL==p_fd)
	{
		return false;
	}

	while(fgets(buffer, 200, p_fd))
	{
		sscanf(buffer, "%*s %x %x %x %*x %*x %*x %x %*x %*x %*x\n",(unsigned int *)&defaultRoutePara[1], (unsigned int *)&defaultRoutePara[0], (unsigned int *)&defaultRoutePara[3],(unsigned int *)&defaultRoutePara[2]);

		if (NULL!=strstr(buffer, name)) 
		{
			//如果FLAG标志中有 RTF_GATEWAY
			if (defaultRoutePara[3] & RTF_GATEWAY) 
			{
				assert(NULL!=pGateway);
				unsigned long ip = defaultRoutePara[0];
#if defined(__TDFH__) || defined(__CHIPTI__) || defined(__CHIPGM__)
				sprintf(pGateway, "%d.%d.%d.%d", (ip&0xff), (ip>>8)&0xff, (ip>>16)&0xff, (ip>>24)&0xff);
#else
				sprintf(pGateway, "%03d.%03d.%03d.%03d", (ip&0xff), (ip>>8)&0xff, (ip>>16)&0xff, (ip>>24)&0xff);
#endif
				
				if (NULL != pTarget) 
				{
					ip = defaultRoutePara[1];
#if defined(__TDFH__) || defined(__CHIPTI__) || defined(__CHIPGM__)
					sprintf(pTarget, "%d.%d.%d.%d", (ip&0xff), (ip>>8)&0xff, (ip>>16)&0xff, (ip>>24)&0xff);
#else
					sprintf(pTarget, "%03d.%03d.%03d.%03d", (ip&0xff), (ip>>8)&0xff, (ip>>16)&0xff, (ip>>24)&0xff);
#endif
					
				}

				if (NULL != pTargetMask) 
				{
					ip = defaultRoutePara[2];
#if defined(__TDFH__) || defined(__CHIPTI__) || defined(__CHIPGM__)
					sprintf(pTargetMask, "%d.%d.%d.%d", (ip&0xff), (ip>>8)&0xff, (ip>>16)&0xff, (ip>>24)&0xff);
#else
					sprintf(pTargetMask, "%03d.%03d.%03d.%03d", (ip&0xff), (ip>>8)&0xff, (ip>>16)&0xff, (ip>>24)&0xff);
#endif
					
				}

				fclose(p_fd);
				return true;
			}
		}

		memset(buffer,0,200);			
	}

	fclose(p_fd);
	return false;
}



bool CNetInterface::GetPPPoEGateway(char*  pGateway, char*  pTarget, char* pTargetMask)
{
	char buffer[200] = {0};

	unsigned long defaultRoutePara[4] = {0};
	FILE* p_fd = fopen(_PATH_ROUTE, "r");
	if (NULL==p_fd)
	{
		return false;
	}

	while(fgets(buffer, 200, p_fd))
	{
		sscanf(buffer, "%*s %x %x %x %*x %*x %*x %x %*x %*x %*x\n",(unsigned int *)&defaultRoutePara[1], (unsigned int *)&defaultRoutePara[0], (unsigned int *)&defaultRoutePara[3],(unsigned int *)&defaultRoutePara[2]);

		if (NULL!=strstr(buffer, "ppp0")) 
		{
			//如果FLAG标志中有 RTF_GATEWAY
			if (  defaultRoutePara[3] & RTF_HOST )
			{
				unsigned long ip = defaultRoutePara[0];

				if( NULL != pGateway )
				{
#if defined(__TDFH__) || defined(__CHIPTI__) || defined(__CHIPGM__)
					sprintf(pGateway, "%d.%d.%d.%d", (ip&0xff), (ip>>8)&0xff, (ip>>16)&0xff, (ip>>24)&0xff);
#else
					sprintf(pGateway, "%03d.%03d.%03d.%03d", (ip&0xff), (ip>>8)&0xff, (ip>>16)&0xff, (ip>>24)&0xff);
#endif
					
				}

				if (NULL != pTarget) 
				{
					ip = defaultRoutePara[1];
#if defined(__TDFH__) || defined(__CHIPTI__) || defined(__CHIPGM__)
					sprintf(pTarget, "%d.%d.%d.%d", (ip&0xff), (ip>>8)&0xff, (ip>>16)&0xff, (ip>>24)&0xff);
#else
					sprintf(pTarget, "%03d.%03d.%03d.%03d", (ip&0xff), (ip>>8)&0xff, (ip>>16)&0xff, (ip>>24)&0xff);
#endif
					
				}

				if (NULL != pTargetMask) 
				{
					ip = defaultRoutePara[2];
#if defined(__TDFH__) || defined(__CHIPTI__) || defined(__CHIPGM__)
					sprintf(pTargetMask, "%d.%d.%d.%d", (ip&0xff), (ip>>8)&0xff, (ip>>16)&0xff, (ip>>24)&0xff);
#else
					sprintf(pTargetMask, "%03d.%03d.%03d.%03d", (ip&0xff), (ip>>8)&0xff, (ip>>16)&0xff, (ip>>24)&0xff);
#endif
					
				}

				fclose(p_fd);
				return true;
			}
		}

		memset(buffer,0,200);			
	}

	fclose(p_fd);
	return false;
}
/******************************************************************************************************

Function:
			SetDNS(unsigned long dnsServer, unsigned long num, const char *pNetIf)

Description:
			1：使用域名解析IP的时候需要DNS服务器的IP地址，系统中该IP存放在/etc/resolv.conf文件中.
			2：该函数把从配置界面获得的DNS服务器信息写到文件resolv.conf文件中
			3：该文件必须存在，并且在只修改某一个dns 服务器IP的时候保证另外一个不变

Calls:

Called by:

Table Accessed:

Table Updated:

Input:
		dnsServer 要存储的dns
		num		  1表示主dns，2表示次dns
		pNetIf	  相关网络接口

Output:

Return:

Others:
		1：zhl 2010-01-10 修改
********************************************************************************************************/


bool CNetInterface::SetDNS(unsigned long dnsServer, unsigned long num , const char *pNetIf)
{
	char szNetIf[32];
	memset( szNetIf, 0 , sizeof( szNetIf ) );
	char newNetIf[32];
	memset( newNetIf, 0 , sizeof( newNetIf ) );
	char dns1[60];
	memset( dns1, 0, sizeof( dns1 ) );
	char dns2[60] = {0};
	memset( dns2, 0, sizeof( dns2 ) );
	FILE *pFile = NULL;


	if( m_localNICStatusInfo.bDHCP || m_localNICStatusInfo.bPPPoE )
	{
		return true;
	}
	if ( ( pFile = fopen(_PATH_DNS, "r+" ) ) != NULL )
	{
		//读出DNS信息
		fscanf( pFile, "%s\r\nnameserver %s\r\nnameserver %s\r\n", szNetIf, dns1, dns2 );
		fclose( pFile );
		pFile = NULL;	
	}

	if( ( pFile = fopen(_PATH_DNS, "w+") ) == NULL )			//把文件清空
	{
		return false;
	}


	sprintf(newNetIf, "%s%s", "#", pNetIf);
	if( 0 == strncmp( szNetIf, newNetIf, strlen( newNetIf ) ) )
	{
		switch( num )
		{
		case 1:
			fprintf( pFile,"%s\r\nnameserver %lu.%lu.%lu.%lu\r\nnameserver %s\r\n\r\n",newNetIf, (dnsServer&0xff),(dnsServer>>8)&0xff,(dnsServer>>16)&0xff,(dnsServer>>24)&0xff, dns2);
			m_localNICStatusInfo.DNS1 = dnsServer;
			break;
		case 2:
			fprintf( pFile,"%s\r\nnameserver %s\r\nnameserver %lu.%lu.%lu.%lu\r\n",newNetIf,dns1, (dnsServer&0xff),(dnsServer>>8)&0xff,(dnsServer>>16)&0xff,(dnsServer>>24)&0xff);
			m_localNICStatusInfo.DNS2 = dnsServer;
			break;
		default:
			assert( false );
			break;
		}
	}
	else
	{
		switch( num )
		{
		case 1:
			fprintf( pFile,"%s\r\nnameserver %lu.%lu.%lu.%lu\r\nnameserver 0.0.0.0\r\n",newNetIf, (dnsServer&0xff),(dnsServer>>8)&0xff,(dnsServer>>16)&0xff,(dnsServer>>24)&0xff);
			m_localNICStatusInfo.DNS1 = dnsServer;
			break;
		case 2:
			fprintf( pFile,"%s\r\nnameserver 0.0.0.0\r\nnameserver %lu.%lu.%lu.%lu\r\n",newNetIf, (dnsServer&0xff),(dnsServer>>8)&0xff,(dnsServer>>16)&0xff,(dnsServer>>24)&0xff);
			m_localNICStatusInfo.DNS2 = dnsServer;
			break;
		default:
			assert( false );
			break;
		}
		
	}

	if( NULL != pFile )
	{
		fclose(pFile);
		pFile = NULL;
	}
	
	//跟新DNS后要从新初始化_res中的变量，因为域名解析的时候要用到该变量中的信息
	res_init();
	

	return true;	
}

int CNetInterface::GetDNS(unsigned long *pDNSServer, int iCount, const char *name )
{
	assert( NULL != pDNSServer );
	assert( NULL != name );

	FILE *pf = 0;
	char *pLine = NULL;
	size_t len = 0;
	char buffer[20] = {0};
	unsigned char index = 0;	

	if( 0 == strcmp( name, "eth0") )
	{
		if ( (pf = fopen(_PATH_DNS, "r")) == NULL)
		{
			perror("fopen");
			return 0;
		}
	}
	else if( 0 == strcmp( name, "pppoe") )
	{

		if ( (pf = fopen(_PATH_DNS_PPPOE, "r")) == NULL)
		{
			perror("fopen");
			return 0;
		}
	}
	else
	{
		assert( false );
		return 0;
	}


	while( -1 != ( getline(&pLine, &len, pf) ) )
	{
		if (iCount > 0)
		{
			if (strstr (pLine, "nameserver") != NULL)
			{
				memset(buffer, 0, sizeof(buffer));
				sscanf(pLine,"nameserver %s",buffer);
				in_addr_t inaddr = inet_addr(buffer);
				if( INADDR_NONE == inaddr )
				{
					perror("get the dns number");
				}
				pDNSServer[index] = inaddr;

				index++;
				iCount--;
			}
		}	
	}

	if(pLine)
		free(pLine);

	fclose(pf);
	return index;
}

bool CNetInterface::ParseDomainName(const char* pDomainname,char* preturnValue, int& length)
{
	assert(NULL!=preturnValue);
	const char *pCurrent = pDomainname;
	const char *pLast  = pDomainname;
	char *pSource = preturnValue;
	unsigned char mark = 0;
	length = 0;

	//只要当前字符不为字符串的结束标志
	while (*pCurrent) 
	{
		//如果当前字符是 .
		if (isDot(*pCurrent)) 
		{
			//如果连着两个都是 . , 输入格式错, 返回错误
			if (isDot(*(pCurrent-1))) 
			{
				return false;
			}
			//写入长度
			*pSource = mark;

			//拷贝域字符
			memcpy(pSource+1,pLast,mark);

			length = length + mark+1;


			//略过字符 .
			pCurrent++;

			//下一次的起点
			pLast = pCurrent;
			pSource = pSource+mark+1;
			//计数清0
			mark = 0;
		}
		else
		{
			pCurrent ++;	//指针后移
			mark ++;		//计数
		}
	}

	//写入长度	
	*pSource = mark;

	//拷贝域字符
	memcpy(pSource+1,pLast,mark);

	//写入结束字符
	*(pSource+mark+1) = 0;

	length = length + mark +2;

	return true;
}

//BUG 直接只是改了老的函数里面不能支持多线程，并且判断地址错误的BUG，但是这个仍然是个不完整的
//函数例如主机别名等几个变量没填，也没判断获取的别名和地址的个数
struct hostent* CNetInterface::GetHostByName(const char* pDomainname, char* dnsName)
{	
	int iDomainLen = strlen(pDomainname);
	char *pParseName = new char[iDomainLen + 2];//用于存储解析后的域名
	memset(pParseName, 0, iDomainLen + 2);
	int lengthParseName = 0;		//解析后字符串长度

	//解析域名	
	bool ret = ParseDomainName(pDomainname,pParseName,lengthParseName);
	if (!ret) 
	{
		delete []pParseName;		
		return NULL;
	}
	assert( ( iDomainLen + 2 ) == lengthParseName );

	//建构DNS报文头部信息	 格式为[标示(2字节)][标志(2字节)][问题数(2字节)][应答资源数(2字节)][授权资源数][额外资源数]{查询问题}{应答}{授权}{额外}
	unsigned int lengthDatagram = 12 + lengthParseName + 4;
	char *dadagram = new char[lengthDatagram];
	memcpy(dadagram,datagramhead,12);				//考贝协议头
	memcpy(dadagram+12,pParseName,lengthParseName);			//考贝要解析的域名
	memcpy(dadagram+12+lengthParseName,datagramend,4);		//考贝协议尾
	delete []pParseName;	

	//获得DNS服务器的IP地址
	struct sockaddr_in their_addr;	
	memset(&their_addr, 0 , sizeof(their_addr));
	if (0 == inet_aton(dnsName, &their_addr.sin_addr))
	{
		printf("(%s  %d) failed: no dns server ip address!\n",__FILE__,__LINE__);
		delete []dadagram;
		return NULL;
	}

	//创建socket和DNS服务器通信
	int sockfd = -1;	
	their_addr.sin_family = AF_INET;
	their_addr.sin_port = htons(DNS_PORT);    
	socklen_t addr_len = sizeof(their_addr);		
	sockfd = socket(AF_INET,SOCK_DGRAM,0);
	if ( sockfd<0 ) 
	{
		printf("(%s %d) Failed: open socket failed,%s!\n",__FILE__,__LINE__,strerror(errno));
		delete []dadagram;
		return NULL;
	}

	timeval timeout;
	timeout.tv_sec = 5;	//超时时间为1s
	timeout.tv_usec = 0;
	if ( setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) ) 
	{
		close(sockfd);	
		printf("(%s %d) Failed: setsockopt failed,%s\n",__FILE__,__LINE__,strerror(errno));
		delete []dadagram;
		return NULL;
	}

	//发送DNS数据并接收服务器回应
	char revBuffer[512] = {0};
	int  revLength = 0;

	struct hostent *pHost = new hostent;
	pHost->h_addr_list = new char*[4];
	for(int index1 = 0; index1 < 4; ++index1)
	{
		pHost->h_addr_list[index1] = new char[20];
		memset(pHost->h_addr_list[index1], 0, 20);
	}

	ret =  false;
	//进行5次通信
	for (int timer = 0; !ret && timer < 3; timer ++) 
	{
		revLength = sendto(sockfd, (const char*)dadagram, lengthDatagram, 0, (struct sockaddr *)&their_addr, sizeof(struct sockaddr));
		if (0 > revLength)
		{
			//printf("%s:%s:%d, Running at here, get ip error\n", __FUNCTION__, __FILE__, __LINE__);
			continue;
		}

		memset(revBuffer,0,sizeof(revBuffer));
		revLength = recvfrom(sockfd,revBuffer,sizeof(revBuffer),0,(sockaddr*)&their_addr, &addr_len);	
		if (0 < revLength) 
		{
			//比较DNS id
			if (0 == memcmp(revBuffer,datagramhead,2)) 
			{
				//在回应报文中可能有多个返回地址, 得到其个数
				short temp = 0;
				memcpy(&temp, revBuffer + 6, sizeof(short));
				short nameNum = ntohs(temp);

				char * pAnser = revBuffer;

				pAnser = pAnser + 12;				//12为DNS协议的头长度
				pAnser = pAnser + lengthParseName;	//问题的长度，也就是域名的长度
				pAnser = pAnser + 4;				//4为query字段的:type, class ,各为一个short类型

				int index = 0;

/*
在收到下面的数据的时候出现死机问题
0x2b	0x50	0x81	0x80	0x00	0x01	0x00	0x05	0x00	0x00	0x00	0x00
0x07	0x63	0x68	0x65	0x63	0x6b	0x69	0x70	0x06	0x64	0x79	0x6e	
0x64	0x6e	0x73	0x03	0x6f	0x72	0x67	0x00	0x00	0x01	0x00	0x01	
0x07	0x63	0x68	0x65	0x63	0x6b	0x69	0x70	0x06	0x44	0x59	0x4e	0x44	0x4e	0x53	0xc0	0x1b
0x00	0x05	0x00	0x01	0x00	0x00	0x02	0x01	0x00	0x14
0x07	0x63	0x68	0x65	0x63	0x6b	0x69	0x70	0x06	0x64	0x79	0x6e	0x64	0x6e	0x73	0x03	0x63	0x6f	0x6d	0x00	
0xc0	0x3f	0x00	0x01	0x00	0x01	0x00	0x00	0x02	0x01	0x00	0x04	0x5b	0xc6	0x16	0x46	
0xc0	0x3f	0x00	0x01	0x00	0x01	0x00	0x00	0x02	0x01	0x00	0x04	0xd8	0x92	0x26	0x46	
0xc0	0x3f	0x00	0x01	0x00	0x01	0x00	0x00	0x02	0x01	0x00	0x04	0xd8	0x92	0x2b	0x46	
0xc0	0x3f	0x00	0x01	0x00	0x01	0x00	0x00	0x02	0x01	0x00	0x04	0xd8	0x92	0x27	0x46
0x00	0x00	0x00
*/
				//这个地方开始是应答数据的位置
				while (nameNum > 0 && pAnser < revBuffer + revLength)
				{
					//域名部分，第一个字节的最高两位为1，则表示第二个字节为一个指向域名的指针。否则为资源记录的RR信息。
					if (0xc0 == (0xc0 & pAnser[0]))
					{
						pAnser += 2;
					}
					else
					{
						int iNameLen = pAnser[0];

						while (0 != iNameLen)
						{
							pAnser += 1;
							pAnser += iNameLen;
							iNameLen = pAnser[0];

							if (0xc0 == 0xc0 & pAnser[0])
							{
								pAnser+= 1;
								break;
							}
						}

						pAnser += 1;
					}

					//得到了类型的位置
					short sNum = 0;
					memcpy(&sNum, pAnser, sizeof(short));
					short nameType = ntohs(sNum);

					pAnser += 2;

					//类
					pAnser += 2;

					//生存时间
					pAnser += 4;

					//长度
					memcpy(&sNum, pAnser, sizeof(short));
					short nameLength = ntohs(sNum);

					pAnser += 2;

					//如果是IP地址, 返回
					if (1==nameType) 
					{
						memcpy(&pHost->h_addr_list[index][3],(pAnser+3),1);
						memcpy(&pHost->h_addr_list[index][2],(pAnser+2),1);
						memcpy(&pHost->h_addr_list[index][1],(pAnser+1),1);
						memcpy(&pHost->h_addr_list[index][0],(pAnser+0),1);
						pHost->h_addrtype =AF_INET;
						index ++;
						ret  = true;
						if (2 == index)
						{
							break;
						}
					}
					else
					{
						//printf("%s:%s:%d, name type=%d\n", __FUNCTION__, __FILE__, __LINE__, nameType);
					}

					pAnser += nameLength;

					nameNum --;
				}
			}
			else
			{
				//printf("%s:%s:%d, Running at here, get ip error\n", __FUNCTION__, __FILE__, __LINE__);
			}
		}
		else
		{
			//printf("%s:%s:%d, Running at here, get ip error, recv len=%d\n", __FUNCTION__, __FILE__, __LINE__, revLength);
		}
	}
	close(sockfd);	
	delete []dadagram;
	if (ret) 
	{		
		return pHost;
	}
	if (NULL != pHost)
	{
		ReleaseHost(pHost);
	}

	return NULL;
}

struct hostent* CNetInterface::GetHostByName(const char* pDomainname)
{
	//获得DNS服务器的IP地址
	hostent* pHost = NULL;
	unsigned long dnsName[10] = {0};	
	char szDnsName[30] = {0};
	struct in_addr in;
	int iCount = GetDNS(dnsName, 10);
	int iIndex = 0;
	while (iCount > 0)
	{
		memset(szDnsName, 0, sizeof(szDnsName));			
		in.s_addr = dnsName[iIndex++];
		/*inet_ntoa_r(in, szDnsName);*/
		unsigned long ip = in.s_addr;
#if defined (__TDFH__) || defined(__CHIPTI__) || defined(__CHIPGM__)
		sprintf(szDnsName, "%d.%d.%d.%d", (ip&0xff), (ip>>8)&0xff, (ip>>16)&0xff, (ip>>24)&0xff);
#else
		sprintf(szDnsName, "%03d.%03d.%03d.%03d", (ip&0xff), (ip>>8)&0xff, (ip>>16)&0xff, (ip>>24)&0xff);
#endif
		pHost = GetHostByName(pDomainname,szDnsName);
		if (NULL != pHost) 
		{
			break;
		}
		else
		{
			//printf("%s:%s:%d, Running at here, get ip error\n", __FUNCTION__, __FILE__, __LINE__);
		}
		iCount--;
	}

	return pHost;
}

void CNetInterface::ReleaseHost(struct hostent *pHost)
{
	delete[] pHost->h_addr_list[0];
	delete[] pHost->h_addr_list[1];
	delete[] pHost->h_addr_list[2];
	delete[] pHost->h_addr_list[3];
	delete[] pHost->h_addr_list;
	delete pHost;
}
//////////////////////////////////////////////////////////////////////////
#if 0
int main(int argc, char** argv)
{
	CNetInterface *pNetwork = CNetInterface::Instance();
	
	if (!pNetwork->Initial())
	{
		printf("Initial network interface fail\n");
		exit(0);
	}
	else
	{
		printf("Initial network interface succ\n");
	}

	unsigned char num = pNetwork->EnumInterface();

	char name [128] = {0};
	for (unsigned char i=0; i<num; ++i)
	{
		if (pNetwork->GetInterfaceName(name, 128, i))
		{
			printf("Interface name :%s\n", name);
		}
		else
		{
			assert (false);
		}
	}
	
	printf("eth0 is index %d\n", pNetwork->GetIndexByName("eth0", 4));

	NET_INTERFACE_STATUS status = pNetwork->GetInterfaceStatus("eth0", 4);
	if (NET_INTERFACE_ERROR != status)
	{
		if (NET_INTERFACE_UP == status)
		{
			printf("eth0 is up\n");
		}
		else
		{
			printf("eth0 is down\n");
		}
	}
	
	if (pNetwork->IPIsCollision(IP_ADDRESS(192, 168, 3, 63)))
	{
		printf("Ip collision\n");
	}

	char ipText [16] = {0};
	printf("Before GetIPText:%s, %d\n", __FILE__, __LINE__);
	if (pNetwork->GetIPText(ipText, 16))
	{
		printf("device ip address:%s\n", ipText);
	}

	if (pNetwork->GetIPMask(ipText, 16))
	{
		printf("device ip address mask:%s\n", ipText);
	}

	char mac [6];
	if (pNetwork->GetMAC(mac))
	{
		printf("device mac:%02x:%02x:%02x:%02x:%02x:%02x\n", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
	}

	if (!pNetwork->SetGateway(IP_ADDRESS(192, 168, 3, 1), 0/*IP_ADDRESS(192, 168, 3, 1)*/, IP_ADDRESS(255, 255, 255, 0)))
	{
		printf("Set Gateway fail\n");
	}

	char target [16], targetMask [16];
	if (pNetwork->GetGateway(ipText, target, targetMask, "eth0"))
	{
		printf("Gateway:%s\n", ipText);
		printf("Target:%s\n", target);
		printf("Target mask:%s\n", targetMask);
	}

	pNetwork->Quit();
	exit(0);
	return 0;
}
#endif


void CNetInterface::GetLocalNICStatusInfo(NETWORK_STATUS_INFO &nicStatusInfo, const char *pNICName)
{
	NETWORK_STATUS_INFO	NICStatusInfoTmp;
	memset( &NICStatusInfoTmp, 0, sizeof( NETWORK_STATUS_INFO ) );

	//MAC地址
	GetMAC( m_localNICStatusInfo.MAC);

	memcpy( &NICStatusInfoTmp, &m_localNICStatusInfo, sizeof( NETWORK_STATUS_INFO ) );

	//IP地址
	if( NICStatusInfoTmp.bPPPoE )
	{
		if( PPPOE_STATUS_VALID != CPPPOEMng::Instance()->IsPppoeRunning() )
		{
			NICStatusInfoTmp.IP = 0;
			NICStatusInfoTmp.netstatus = 0;
			NICStatusInfoTmp.subMask = 0;
			NICStatusInfoTmp.gateway = 0;
			NICStatusInfoTmp.DNS1 = 0;
			NICStatusInfoTmp.DNS2 = 0;
			NICStatusInfoTmp.bDDNS = false;
			memcpy( &nicStatusInfo, &NICStatusInfoTmp, sizeof( NETWORK_STATUS_INFO ) );
			return;

		}
		else
		{
			GetIPAddress( NICStatusInfoTmp.IP, "ppp0" );
		}
	}
	else if( NICStatusInfoTmp.bDHCP )
	{
		unsigned long dhcpIP = 0;
		if( GetIPAddress( dhcpIP ) )
		{
			if( IN_LOOPBACK( ntohl( dhcpIP ) ) )
			{
				NICStatusInfoTmp.IP = 0;
				NICStatusInfoTmp.netstatus = 0;
				NICStatusInfoTmp.subMask = 0;
				NICStatusInfoTmp.gateway = 0;
				NICStatusInfoTmp.DNS1 = 0;
				NICStatusInfoTmp.DNS2 = 0;
				NICStatusInfoTmp.bDDNS = false;
				memcpy( &nicStatusInfo, &NICStatusInfoTmp, sizeof( NETWORK_STATUS_INFO ) );
				return;
				
			}			
			else
			{
				NICStatusInfoTmp.IP = dhcpIP;

			}
		}
		else
		{
			NICStatusInfoTmp.IP = 0;
			NICStatusInfoTmp.netstatus = 0;
			NICStatusInfoTmp.subMask = 0;
			NICStatusInfoTmp.gateway = 0;
			NICStatusInfoTmp.DNS1 = 0;
			NICStatusInfoTmp.DNS2 = 0;
			NICStatusInfoTmp.bDDNS = false;
			memcpy( &nicStatusInfo, &NICStatusInfoTmp, sizeof( NETWORK_STATUS_INFO ) );

			return;
		}

	}
	else
	{
		GetIPAddress( NICStatusInfoTmp.IP );
	}


	//网关
	char Getway[20];
	memset( Getway, 0, sizeof( Getway ) );
	bool bGetGetwayOk = false;
	if( NICStatusInfoTmp.bPPPoE )
	{
		GetIPMaskUL(NICStatusInfoTmp.subMask, "ppp0");
		bGetGetwayOk = GetPPPoEGateway( NULL, Getway, NULL );
	}
	else
	{
		GetIPMaskUL(NICStatusInfoTmp.subMask);
		bGetGetwayOk = GetGateway(Getway, NULL, NULL);
	}

	struct in_addr inAddrGetway;
	memset( &inAddrGetway, 0, sizeof( struct in_addr ) );
	if( bGetGetwayOk )
	{
		if( 0 == inet_aton(Getway,&inAddrGetway) )
		{
			perror("inet_aton() error\n");
			assert(0);
		}

	}
	memcpy(&NICStatusInfoTmp.gateway, &inAddrGetway.s_addr, sizeof(int));
	
	//dns
	unsigned long ulDNS[2];
	memset( ulDNS, 0, sizeof( unsigned long ) * 2 );
	if( NICStatusInfoTmp.bPPPoE )
	{
		GetDNS(ulDNS, 2, "pppoe");

		NICStatusInfoTmp.DNS1 = ulDNS[0];
		NICStatusInfoTmp.DNS2 = ulDNS[1];

	}
	else if( NICStatusInfoTmp.bDHCP )
	{
		GetDNS(ulDNS, 2 );

		NICStatusInfoTmp.DNS1 = ulDNS[0];
		NICStatusInfoTmp.DNS2 = ulDNS[1];

	}
	else
	{
		//dnsCount = GetDNS(ulDNS, 2 );
	}


	//检测网络是否有效， 在未插网线的时候不检测
	//静态IP的时候检查是否有IP冲突
	//PPPOE和DHCP时检测网关是否有ICMP回应
	if( GetNICRunning() )
	{
		if( !NICStatusInfoTmp.bPPPoE && !NICStatusInfoTmp.bDHCP )
		{
			NICStatusInfoTmp.netstatus = ( IPIsCollision(NICStatusInfoTmp.IP) )?0:1;
		}
		else
		{
			char host[20] = {0};
			struct in_addr inAddr;
			inAddr.s_addr = NICStatusInfoTmp.gateway;
			sprintf( host, "%s", inet_ntoa(inAddr));
			NICStatusInfoTmp.netstatus = ( CanReachTheHost( host ) )?1:0;
		}	
	}
	else
	{
		NICStatusInfoTmp.netstatus = 0;
	}

	memset( &nicStatusInfo, 0, sizeof( NETWORK_STATUS_INFO ) );
	memcpy( &nicStatusInfo, &NICStatusInfoTmp, sizeof( NETWORK_STATUS_INFO ) );


	//ddns 信息
	nicStatusInfo.bDDNS = ( m_localNICStatusInfo.bDDNS & nicStatusInfo.netstatus );
	

}


bool CNetInterface::GetNICRunning( const char *pNICName )
{

	assert (strlen(pNICName) < IFNAMSIZ);

	struct   ifreq   ifr;  
	memset(&ifr, 0, sizeof(ifr));

	strcpy(ifr.ifr_name, pNICName);

	if (!(ioctl(m_fd, SIOCGIFFLAGS, (char *)&ifr)))
	{
		if((IFF_UP & ifr.ifr_flags) && ( IFF_RUNNING & ifr.ifr_flags ))
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	//printf("GetNICRunning(%s  %d) failed:%s\n",__FILE__,__LINE__,strerror(errno));
	return false;
}

/******************************************************************************************************

Function:
			int　IsValidIP( unsigned long IPAddr, unsigned long netmask )

Description:
			配置DVR IP地址前要检测IP的有效性， 只允许配置A类，B类，C类的IP地址。
			下面的IP是无效的
			1：全是0的IP地址
			2：全是1的ip地址或全是1的网络掩码
			3：以127.开始的地址
			4：主机部分全是0的IP(代表本网络号，不能作为主机有效IP地址)
			5：主机部分全是1的IP(代表本网络上的广播地址，不能作为主机的有效IP地址)
			6：网络号全是0的IP地址
			7：未划分子网时netmask中网络掩码的长度小于默认的网络掩码长度(例如C类网络掩码长度是24，当netmask中网络掩码小于24时该IP被视为无效)
			8：网络掩码和子网掩码中任何一位都不能为0

Calls:

Called by:

Table Accessed:

Table Updated:

Input:		
			1：IPAddr,要检测的IP地址， 可为任意值
			2：netmask=0，表示使用默认的网络掩码，没有子网划分
			   netmask!=0, 表示使用了子网功能

Output:		

Return:
			返回0表示该IP地址可用于主机IP
			返回1表示该IP地址不可用于主机IP

Others:
********************************************************************************************************/
IP_CHECK_ERR CNetInterface::IsValidIP( unsigned long IPAddr, unsigned long netmask )
{
	unsigned long inNetMask;
	unsigned long inHostMask;
	unsigned long inHostLength;
	unsigned long inNetHead;
	unsigned long ipTemp;
	unsigned long subnetmask;

	//DVR中存放IP地址方式是BIG_ENDIAN.
	//先转换IP和子网掩码为平台相关字节序
	if( __BYTE_ORDER == __LITTLE_ENDIAN )
	{
		IPAddr = bswap_32( IPAddr );
		netmask = bswap_32( netmask );
	}

	//不能使用的IP地址 0.0.0.0 、255.255.255.255、127.0.0.0
	if( ( INADDR_ANY == IPAddr ) || ( INADDR_BROADCAST == IPAddr ) || ( IN_LOOPBACK( IPAddr ) ) )
	{
		return IP_CHECK_ERR_IPINVALID;
	}

	//不能使用的网络掩码0.0.0.0 和255.255.255.255
	if( ( INADDR_ANY == netmask ) || ( INADDR_BROADCAST == netmask ) )
	{
		return IP_CHECK_ERR_NETMASK;
	}

	//只能在网络中存在三类IP地址 A类、B类和C类IP地址
	if( IN_CLASSA( IPAddr ) )
	{
		inNetMask = IN_CLASSA_NET;
		inHostMask = IN_CLASSA_HOST;
		inHostLength = IN_CLASSA_NSHIFT;
		inNetHead = ( 0x0 << ( 32 - inHostLength ) );

	}
	else if( IN_CLASSB( IPAddr ) )
	{
		inNetMask = IN_CLASSB_NET;
		inHostMask = IN_CLASSB_HOST;
		inHostLength = IN_CLASSB_NSHIFT;
		inNetHead = ( 0x80 << ( 32 - inHostLength ) );
	}
	else if( IN_CLASSC( IPAddr ) )
	{
		inNetMask = IN_CLASSC_NET;
		inHostMask = IN_CLASSC_HOST;
		inHostLength = IN_CLASSC_NSHIFT;
		inNetHead = ( 0xc0 << ( 32 - inHostLength ) );

	}
	else
	{
		return IP_CHECK_ERR_IPINVALID;
	}


	if( INADDR_ANY != ( netmask & inHostMask ) )
	{
		//进行子网划分时要检测网络掩码和子网掩码部分全为1

		if( INADDR_ANY != ( inNetMask ^ ( inNetMask & netmask ) ) )
		{	//网络掩码中不全是1
			return IP_CHECK_ERR_NETMASK;
		}

		int i = 0;
		while( ( 0x1 ^ ( ( netmask >> i ) & 0x1 ) ) )
		{
			i++;
		}


		subnetmask = ( ~ ( ( 0x1 << i ) -1  ) );						//网络掩码和子网掩码都是1
		if( netmask ^ subnetmask )
		{	//子网掩码中不全是1
			return IP_CHECK_ERR_NETMASK;
		}

		inHostMask = ( 0xffffffff & ( ~subnetmask ) );					//带子网掩码时主机部分的掩码
	}
	else
	{
		//不进行子网划分时检测是否是组建超网。检测方法为：只要网络掩码中第一个1前面都是1就表示为正常的网络掩码

		//查找第一个1的位置
		int i = 0;
		while( ( 0x1 ^ ( ( netmask >> i ) & 0x1 ) ) )
		{
			i++;
		}

		subnetmask = ( ~ ( ( 0x1 << i ) -1  ) );						//真正的网络掩码
		if( netmask ^ subnetmask )
		{	//网掩码中不全是1
			return IP_CHECK_ERR_NETMASK;
		}

		inHostMask = ( 0xffffffff & ( ~subnetmask ) );					//真正的主机掩码
	}


	//到此处说明netmask是正常的网络掩码（划分子网时包括子网掩码，组建超网时是超网的掩码）
	//IP地址主机部分不能全为0或全为1
	if(  ( INADDR_ANY == ( IPAddr & inHostMask ) )  ||  ( inHostMask == ( IPAddr & inHostMask  ) ) )
	{
		return IP_CHECK_ERR_IPINVALID;
	}
	
	//网络部分全是0
	//if( inNetHead == ( IPAddr & inNetMask  ) )
	//{
	//	return IP_CHECK_ERR_IPINVALID;
	//}

	return IP_CHECK_ERR_SUCC;

}
/*
Function:		CanReachTheHost()
Description:	测试目标主机是否能连通。向主机pHost发送ICMP请求报文，如果主机有回应表示可以连通到主机
Input:			pHost, 主机点分格式的IP或者主机域名
Output:			
Return:			返回true 表示能连到目标主机
				返回false表示不能连接到目标主机
*/
bool CNetInterface::CanReachTheHost( const char *pHost )
{
	char icmpSendData[100]={0};
	char icmpRecvData[100]={0};
	struct in_addr sin_addr;
	memset( &sin_addr, 0, sizeof( struct in_addr ) );

//#ifdef __USE_BSD
	strncpy( icmpSendData+sizeof(struct icmp), _ICMP_OPTION, strlen(_ICMP_OPTION));

	if( 0 == MakeEchoPack( icmpSendData, sizeof( struct icmp ) + strlen(_ICMP_OPTION)))
	{
		if( 0 == inet_aton( pHost, &sin_addr))
		{
#if	USE_LOCAL_DNSRESOLVE
			struct hostent *phe = GetHostByName( pHost );
#else
			struct hostent *phe = gethostbyname( pHost );
#endif
			if( NULL != phe )
			{
				sin_addr = *(struct in_addr *)phe->h_addr;
#if	USE_LOCAL_DNSRESOLVE
				ReleaseHost( phe );
#endif
			}
			else
			{
				return false;
			}

		}

		int dataLen = sizeof( struct icmp ) + strlen(_ICMP_OPTION);
		if( 0 == SndRecIcmpPack( icmpSendData, icmpRecvData, &dataLen, sin_addr ) )
		{
			struct ip *pIpHead = ( struct ip *)( icmpRecvData);
			struct icmp *pRecvIcmp = (struct icmp *)( icmpRecvData +  4 * pIpHead->ip_hl );

			if( ( ICMP_ECHOREPLY != pRecvIcmp->icmp_type ) || memcmp( &sin_addr, &pIpHead->ip_src, sizeof(struct in_addr) ) )
			{
				//不是ICMP请求的应答
				//不是从目标主机发来的ICMP应答,为网络或者主机不可达
				printf("(%s:%s:%d),不能到达目标网络或者主机\n", __FUNCTION__, __FILE__, __LINE__);
				printf("\n\n\n\n");
				return false;
			}
			//printf("(%s:%s:%d) get ICMP reply message, length=%d\n", __FUNCTION__, __FILE__, __LINE__, dataLen);
			//printf("(%s:%s:%d),ip head length=%d, ip total length=%d, version=%d, ttl=%d\n", __FUNCTION__, __FILE__, __LINE__, pIpHead->ip_hl,pIpHead->ip_len,pIpHead->ip_v,pIpHead->ip_ttl);	
			//printf("(%s:%s:%d),icmp type=%d, code=%d, id=%d, seq=%d, checksum=0x%04x ", __FUNCTION__, __FILE__, __LINE__, pRecvIcmp->icmp_type, pRecvIcmp->icmp_code, pRecvIcmp->icmp_id, pRecvIcmp->icmp_seq, pRecvIcmp->icmp_cksum);
			//printf("icmp option=%s\n", (char *)(pRecvIcmp)+sizeof(struct icmp));
			//
			//printf("(%s:%s:%d), can reach the host %s\n", __FUNCTION__, __FILE__, __LINE__, inet_ntoa( sin_addr ));
			//printf("\n\n\n\n");
			return true;
		}

	}
	
	printf("(%s:%s:%d), can not reach the host %s\n", __FUNCTION__, __FILE__, __LINE__, inet_ntoa( sin_addr ));
	return false;

//#endif
}

int CNetInterface::MakeEchoPack( char *pIcmpData, int dataLen )
{
	assert( NULL != pIcmpData );
	assert( dataLen >= sizeof(struct icmp ) );

	if( NULL == pIcmpData ) 
	{
		printf("%s:%s:%d, parameter is empty\n", __FUNCTION__, __FILE__, __LINE__);
		return -1;
	}

	struct icmp *pIcmpPack = ( struct icmp * )(pIcmpData);

	memset(pIcmpPack, 0, sizeof(struct icmp));
	pIcmpPack->icmp_type = ICMP_ECHO;
	pIcmpPack->icmp_code = 0;
	//pIcmpPack->icmp_id=(u_int16_t)pthread_self();
	pIcmpPack->icmp_id=getpid();
	pIcmpPack->icmp_seq=0;
	//printf("icmp type=%d, code=%d, id=%d, seq=%d ", pIcmpPack->icmp_type, pIcmpPack->icmp_code, pIcmpPack->icmp_id, pIcmpPack->icmp_seq);
	pIcmpPack->icmp_cksum = CountIcmpDataChecksum( pIcmpData, dataLen );
	//printf("icmp option=%s\n", pIcmpData+sizeof(struct icmp));
	return 0;
}

u_int16_t CNetInterface::CountIcmpDataChecksum( const char *pIcmpData, int dataLen )
{
	assert( NULL != pIcmpData );
	int count = 0;
	int len=dataLen;
	u_int16_t checksum=0;
	const char *pIndex = pIcmpData;

	while( len > 1 )
	{
		count += *(u_int16_t*)pIndex;
		pIndex +=2;
		len -=2;	
	}
	if( 1 == len )
	{
		checksum = (u_int16_t)(*pIndex);
	}
	count += checksum;
	checksum=(count>>16) + (count & 0xffff);
	checksum = ~checksum;

	return checksum;
	
}

int CNetInterface::SndRecIcmpPack( const char *pIcmpSendData, char *pIcmpRecvData, int *pDataLen,struct in_addr sin_addr)
{
#if 0
	struct protoent *protocol = getprotobyname("icmp");
	if( NULL == protocol )
	{
		perror("getprotobyname()");
		printf("get icmp protocol error\n");
		return -1;	
	}
#endif
	int sockFd = socket( AF_INET, SOCK_RAW ,/*protocol->p_proto*/IPPROTO_ICMP);
	if( -1 == sockFd )
	{
		perror("socket()");
		return -1;
	}

	struct sockaddr_in sockaddrDest;
	bzero( &sockaddrDest, sizeof( struct sockaddr_in));
	sockaddrDest.sin_family = AF_INET;
	sockaddrDest.sin_addr = sin_addr;

	if( -1 != sendto( sockFd, pIcmpSendData, *pDataLen, 0, (const struct sockaddr*)&sockaddrDest, sizeof( struct sockaddr_in)) )
	{
		fd_set rdset;
		for( int retry=0; retry<3; retry++)
		{
			FD_ZERO( &rdset );
			FD_SET( sockFd, &rdset );

			struct timeval  timeOut = { 1, 0 };
			int retval = select( sockFd+1, &rdset, NULL, NULL, &timeOut);
			if( 0< retval )
			{
				if( FD_ISSET( sockFd, &rdset ) )
				{
					*pDataLen = recv( sockFd, pIcmpRecvData, 100, 0);
					if( -1 == *pDataLen )
					{
						perror("recv()");
						close( sockFd );
						return -1;
					}
					else
					{
						close( sockFd );
						return 0;
					}
				}
				else
				{
					close( sockFd );
					assert( false );
					return -1;
				}
			}
			else if( 0 == retval )
			{
				//超时的时候再发一次请求
				sendto( sockFd, pIcmpSendData, *pDataLen, 0, (const struct sockaddr*)&sockaddrDest, sizeof( struct sockaddr_in));
				continue;
			}
			else 
			{
				perror("select()");
				close( sockFd );
				return -1;
			}
		}
	}

	close( sockFd );
	return -1;

}
//end
