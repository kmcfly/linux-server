/*****************************************************************
Copyright (C),2009-2012, tongwei video Tech. Co., Ltd.

File name:AllocNICIP.h

Author: TVT-ZHL-146  Version:	Date:

Description:
			1:���豸��������IP��ַ
			2:ϵͳ�������ַ���IP��ַ�ķ�ʽ���ֱ�Ϊ��̬����IP��
			  ͨ��DHCP����IP��ͨ��PPPOE����IP��ַ
			3:

Others:

Function List:

History:

1.Date:

Author:

Modification:	
2.
******************************************************************/

#ifndef ALLOC_NIC_IP_H_
#define ALLOC_NIC_IP_H_

#include "dvrdvsdef.h"
#include "NetProtocol.h"
#include "NetInterface.h"
#include "PUB_common.h"
#include "PPPoeMng.h"


const char _PATH_DHCP_STATUS_INFO_FILE[] = "/tmp/dhcpcd-status.info";
const char _ADD_MULTICAST_ROUTE_CMD[] = "route add -net 224.0.0.0 netmask 240.0.0.0 dev ";

class CAllocNICIP{
public:
	
	typedef enum  _getipmode_									//�������IP�ķ�ʽ
	{
		GETIPMODE_NPPPOE,										//ͨ����PPPOE��ʽ�������IP��ַ��������̬IP��DHCP
		GETIPMODE_PPPOE

	}GETIPMODE;

	static CAllocNICIP *Instance();
	~CAllocNICIP();

	bool Initial();
	void Quit();

	NET_CTRL_FAIL CheckPppoeParam( const unsigned char *pGetipModeInfo );
	NET_CTRL_FAIL CheckDhcpParam( const unsigned char *pGetipModeInfo );
	NET_CTRL_FAIL allocNICIP(const GETIPMODE &getIPMode,const unsigned char *pGetipModeInfo,const char *pNICName="eth0" );
	inline void   SetDnsInfo( unsigned long dns, int whichOne );

	bool RecoverNetIic(const char *pNicName="eth0");
	NET_CTRL_FAIL GetDhcpIP(NETWORK_STATUS_INFO &info, const char *pNicName="eth0");
private:
	CAllocNICIP();
	
	NET_CTRL_FAIL GetIPDhcp( const NETWORK_ADDR_INFO &networkAddrInfo, const char *pNICName );
	NET_CTRL_FAIL GetIPStatic( const NETWORK_ADDR_INFO &networkAddrInfo, const char *pNICName );
	NET_CTRL_FAIL GetIPPppoe( const PPPOE_INFO &pppoeInfo, const char *pNICName);

	inline bool StartDHCP( char *pParam,const char *pNICName="eth0" );
	inline bool StopDHCP();
	int		ValidDHCPip();

	
	enum _dhcp_status_
	{
		DHCP_STATUS_STOP = 0,			//DHCPû������
		DHCP_STATUS_RUNNING = 1,		//DHCP�������У�����û�з��䵽��Ч��IP
		DHCP_STATUS_VALID = 2,			//DHCP�������У����ҷ�������Ч��ip
	};



	NETWORK_ADDR_INFO m_networkAddrInfo;
	PPPOE_INFO		  m_pppoeInfo;
	unsigned long	  m_firstStaticDNS;						//���dns
	unsigned long	  m_secondStaticDNS;
	int				  m_usePPPOE;
	CPUB_Lock		  m_changeNicCfgLock;
};

bool CAllocNICIP::StartDHCP(char *pParam, const char *pNICName)
{
	char dhcpCmd[120];
	memset( dhcpCmd, 0, sizeof(dhcpCmd ) );

	if( NULL != pNICName )
	{
		//ѡ��ĳ������ʹ��DHCP����
		sprintf( dhcpCmd, "udhcpc %s -i %s > %s", pParam, pNICName, _PATH_DHCP_STATUS_INFO_FILE );
	}
	else
	{
		//Ĭ��ʹ��eth0
		sprintf( dhcpCmd, "udhcpc %s > %s", pParam, _PATH_DHCP_STATUS_INFO_FILE);
	}
	
	if( 0 == DVRSystem(dhcpCmd) ) 
	{
		return true;
	}
	else
	{
		return false;
	}


}

bool CAllocNICIP::StopDHCP()
{
	if( -1 == killProcessByName(const_cast<char *>("udhcpc"), NULL) )
	{
		return false;
	}

	return true;
}

inline void CAllocNICIP::SetDnsInfo( unsigned long dns, int whichOne )
{
	switch( whichOne )
	{
	case 1:
		m_firstStaticDNS = dns;
		break;
	case 2:
		m_secondStaticDNS = dns;
		break;
	default:
		break;
	}
}
#endif
