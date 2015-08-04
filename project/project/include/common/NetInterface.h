/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : 袁石维
** Date         : 2009-09-25
** Name         : NetInterface.h
** Version      : 1.0
** Description  :
** Modify Record:
1:
***********************************************************************/
#ifndef _NET_INTERFACE_H_
#define _NET_INTERFACE_H_

#include <stdlib.h>  
#include <stdio.h>
#ifdef __ENVIRONMENT_LINUX__
#include <sys/types.h>
#include <sys/param.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <net/if.h>
#include <netinet/in.h>
#include <net/if_arp.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <errno.h>
#include <net/route.h>
#include <fcntl.h>
#include <dirent.h>
#include <ctype.h>
#include <netdb.h>
#include <netinet/ip_icmp.h>
#endif
#include "dvrdvsdef.h"


#define IN_LOOPBACK(a)	((((long int )(a))&0xff000000)==0x7f000000)

#define USE_LOCAL_DNSRESOLVE		1

const unsigned char MAX_INTERFACE_NUM = 3;
const char _PATH_ROUTE[]	= "/proc/net/route";

//#if defined(__CHIPGM__)
//	const char _PATH_DNS[]		= "/tmp/dns.conf";
//#else
	const char _PATH_DNS[]		= "/etc/resolv.conf";
//#endif

const char _PATH_DNS_PPPOE[] = "/etc/resolv.conf";
const char _ICMP_OPTION[] = "haha, give me a icmp reply!";


typedef enum _net_interface_status
{
	NET_INTERFACE_ERROR	= 0,
	NET_INTERFACE_UP,
	NET_INTERFACE_DOWN,
}NET_INTERFACE_STATUS;		//网络接口状态

typedef enum _ip_set_err
{
	IP_SET_ERR_SUCC,
	IP_SET_ERR_COLLISION,
	IP_SET_ERR_INVALID,
	IP_SET_ERR_NO_DEVICE
}IP_SET_ERR;

typedef enum _ip_check_err
{
	IP_CHECK_ERR_SUCC,			//IP地址可用
	IP_CHECK_ERR_IPINVALID,		//IP地址不可用
	IP_CHECK_ERR_NETMASK		//网络掩码错误
}IP_CHECK_ERR;

#define ARP_REQUEST 0x01	//ARP请救
#define ARP_REPLAY  0x02	//ARP应答

#pragma pack(1)
//ARP报文,包含以太网帧头
typedef struct arp_packet                    
{
	unsigned	char    target_Mac[6]; 
	unsigned	char    send_Mac[6];
	unsigned	short   ethnet_type;	//上面是以太网帧头, 之所以没有分开， 是因为arm gcc编译器设置1字节对齐无效

	unsigned	short	had_Type;
	unsigned	short	pro_Type;
	unsigned	char	had_Size; 
	unsigned	char	pro_Size; 
	unsigned	short	operate; 
	unsigned	char	send_Mac1[6];  
	unsigned	long	send_Ip;  
	unsigned	char	target_Mac1[6];  
	unsigned	long	target_ip;		//以上才是真正的ARP报文
} ARP_PACKET;



#pragma pack()

#define DNS_PORT 53
#define isDot(x) x=='.'

class CNetInterface
{
public:
	static CNetInterface * Instance();
	~CNetInterface();

	bool Initial();
	void Quit();
	//////////////////////////////////////////////////////////////////////////
	unsigned char EnumInterface();	//返回设备数目
#if 0
	bool GetInterfaceName(char *pName, unsigned short len, unsigned char index);
#endif
	bool UpInterface(const char* name, unsigned short len);
	bool DownInterface(const char* name, unsigned short len);
#if 0 
	int GetIndexByName(const char* name, unsigned short len);
#endif
	NET_INTERFACE_STATUS GetInterfaceStatus(const char* name = "eth0", unsigned short len = 4);	//获得接口状态

	bool IPIsCollision(unsigned long ip, const char* name = "eth0", unsigned short len = 4);
	IP_SET_ERR SetIP(unsigned long ip, const char* name = "eth0", unsigned short len = 4);
	bool GetIPAddress(unsigned long &ip, const char* name = "eth0", unsigned short len = 4);
	bool GetIPText(char *pIPText, unsigned short textLen, const char* name = "eth0", unsigned short len = 4);
	bool SetIPMask(unsigned long ipMask, const char* name = "eth0", unsigned short len = 4);
	bool GetIPMask(char *pIPMaskText, unsigned short textLen, const char* name = "eth0", unsigned short len = 4);
	bool GetIPMaskUL(unsigned long  &ipmask, const char* name = "eth0", unsigned short len = 4);

	bool SetMAC(char* pMac, const char* name = "eth0", unsigned short len = 4);
	bool GetMAC(char* pMac, const char* name = "eth0", unsigned short len = 4);

	bool SetGateway(unsigned long  gateway, unsigned long  target, unsigned long  targetMask, const char* name = "eth0");
	bool DelGateway(unsigned long gateway, unsigned long target, unsigned long targetMask);
	bool GetGateway(char*  pGateway, char*  pTarget, char* pTargetMask, const char* name = "eth0");

	bool SetDNS(unsigned long dnsServer1, unsigned long dnsServer2 , const char *pNetIf);
	int GetDNS(unsigned long *pDNSServer, int iCount, const char* name="eth0");

	bool ParseDomainName(const char* pDomainname,char* preturnValue, int& length);
	struct hostent* GetHostByName(const char* pDomainname, char* dnsName);
	struct hostent* GetHostByName(const char* pDomainname);
	void ReleaseHost(struct hostent *pHost);

	void GetLocalNICStatusInfo(NETWORK_STATUS_INFO &nicStatusInfo, const char *pNICName="eth0");
	bool GetPPPoEGateway(char*  pGateway, char*  pTarget, char* pTargetMask);
	void SetbDHCP(const bool buseDHCP){m_localNICStatusInfo.bDHCP = buseDHCP;}
	void SetbPPPOE(const bool busePPPOE){m_localNICStatusInfo.bPPPoE = busePPPOE;}
	void SetbDDNS( const char buseDDNS){m_localNICStatusInfo.bDDNS = buseDDNS;}
	void SetbWifi( const bool buseWiFi){m_localNICStatusInfo.bWiFi = buseWiFi;}
	void SetHttpPort( const unsigned short usHttpPort){m_localNICStatusInfo.httpPort = usHttpPort;}
	void SetServerPort( const unsigned short usServerPort){m_localNICStatusInfo.serverPort = usServerPort;}
	unsigned short GetServerPort() {return m_localNICStatusInfo.serverPort;}

	bool GetNICRunning( const char *pNICName="eth0" );
	IP_CHECK_ERR  IsValidIP( unsigned long IPAddr, unsigned long netmask );
	bool CanReachTheHost( const char *pHost );

protected:
	int MakeEchoPack( char *pIcmpData, int dataLen );
#ifdef __ENVIRONMENT_LINUX__
	u_int16_t CountIcmpDataChecksum( const char *pIcmpData, int dataLen );
#endif
	int SndRecIcmpPack( const char *pIcmpSendData, char *pIcmpRecvData, int *pDataLen,struct in_addr sin_addr);
private:
	CNetInterface();


	//////////////////////////////////////////////////////////////////////////
	int			m_fd;
	//unsigned char m_interfaceNum;
	//struct ifreq m_InterfaceBuf[MAX_INTERFACE_NUM];

	NETWORK_STATUS_INFO	m_localNICStatusInfo;						//存放本地网卡状态
};
#endif //_NET_INTERFACE_H_
