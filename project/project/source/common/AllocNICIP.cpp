#include "AllocNICIP.h"
#include <resolv.h>
extern bool g_curNet3G;

CAllocNICIP::CAllocNICIP()
{
	
}

CAllocNICIP::~CAllocNICIP()
{

}


bool CAllocNICIP::Initial()
{
	memset(&m_networkAddrInfo, 0, sizeof(NETWORK_ADDR_INFO));
	memset(&m_pppoeInfo, 0, sizeof(PPPOE_INFO));
	
	m_firstStaticDNS = 0;
	m_secondStaticDNS = 0;
	m_usePPPOE = 0;
	
	CPPPOEMng::Instance()->Initial();

	return true;
}

void CAllocNICIP::Quit()
{
	if( true == m_networkAddrInfo.bDHCP )
	{
		StopDHCP();

	}

	if( true == m_pppoeInfo.bEnable )
	{
		CPPPOEMng::Instance()->Stop();
	}

}

CAllocNICIP *CAllocNICIP::Instance()
{
	static CAllocNICIP allocNICIP;
	return &allocNICIP;
}

/*****************************************************************
Function:
		CheckPppoeParam( const unsigned char *pGetipModeInfo )
Description:
		���PPPOE�����Ƿ���ȷ��
		1:����Ҫ���ڼ���״̬
		2:PPPOE�û�������Ϊ��
		3:���PPPOE�û����������Ƿ���Ч���û�Ҫ��ֱ֤����modeomֱ�Ӻ���������ṩ������

Calls:

Called by:

Table Accessed:

Table Updated:

Input:

Output:

Return:

Others:
		2010.02.02 TVT zhl
******************************************************************/

NET_CTRL_FAIL CAllocNICIP::CheckPppoeParam( const unsigned char *pGetipModeInfo )
{
	assert( NULL != pGetipModeInfo );
	const PPPOE_INFO *pPppoeInfo = reinterpret_cast< const PPPOE_INFO *>(pGetipModeInfo);

	if( !CNetInterface::Instance()->GetNICRunning() )
	{
		//�������ӿ��Ƿ���UP״̬
		return NETERR_NETPARAMCHECK_ECONNECT;
	}

	if( 0 == strlen( pPppoeInfo->user ) )
	{
		//�û�������Ϊ��
		return NETERR_NETPARAMCHECK_EUSERNAMEEMPTY;
	}
	
	if( pPppoeInfo->bEnable )
	{
		m_changeNicCfgLock.Lock();
		if( !m_pppoeInfo.bEnable )
		{
			//����/etc/resolv.conf�ļ�
			DVRSystem("rm /tmp/resolv.conf");
			DVRSystem("cp /etc/resolv.conf /tmp/");
		}

		CPPPOEMng::Instance()->Stop();

		int pppoeTestStatus = CPPPOEMng::Instance()->TestPppoe( pPppoeInfo->user, pPppoeInfo->password );
		
		NET_CTRL_FAIL retval = NETERR_NETPARAMCHECK_OK;

		switch( pppoeTestStatus )
		{
		case PPPOE_STATUS_UNKNOW:
			{
				assert( false );
			}
		case PPPOE_STATUS_STOP:
		case PPPOE_STATUS_RUNNING:
				retval = NETERR_NETPARAMCHECK_ENAMEPWD;
				break;
		case PPPOE_STATUS_VALID:
			{
				retval = NETERR_NETPARAMCHECK_OK;
			}
			break;
		default:
			break;
		}

		if( m_pppoeInfo.bEnable )
		{
			//���֮ǰPPPOE�������оʹ��´�pppoe
			CPPPOEMng::Instance()->Start( m_pppoeInfo.user, m_pppoeInfo.password);
		}
		else
		{
			DVRSystem( "rm -rf /etc/resolv.conf");
			DVRSystem("mv /tmp/resolv.conf /etc/");
		}

		res_init();

		m_changeNicCfgLock.UnLock();

		return retval;
	}

	return NETERR_NETPARAMCHECK_OK;

}
/*****************************************************************

Function:
		CheckDhcpParam( const unsigned char *pGetipModeInfo )
Description:
		������õ�IP�Ƿ�Ϸ���
		1:����Ƕ�̬��ȡIP��ַ�Ͳ����м��
		2:ʹ�þ�̬IP��ʱ��Ҫ���IP�ĺϷ��Ժ��Ƿ����������������IP��ַ��ͻ

Calls:

Called by:

Table Accessed:

Table Updated:

Input:

Output:

Return:

Others:	
		2010.02.02 TVT zhl
******************************************************************/

NET_CTRL_FAIL CAllocNICIP::CheckDhcpParam( const unsigned char *pGetipModeInfo )
{
	assert( NULL != pGetipModeInfo );
	const NETWORK_ADDR_INFO *pNetworkAddrInfo = reinterpret_cast<const NETWORK_ADDR_INFO *>( pGetipModeInfo );

	if( pNetworkAddrInfo->bDHCP )
	{
		m_changeNicCfgLock.Lock();
		if( !CNetInterface::Instance()->GetNICRunning() )
		{
			//�������ӿ��Ƿ���UP״̬
			m_changeNicCfgLock.UnLock();
			return NETERR_NETPARAMCHECK_ECONNECT;
		}
		
		if( m_networkAddrInfo.bDHCP )
		{
			//���DHCP���������У���Ҫ��ֹͣDHCP
			printf("%s:%s:%d, stop dhcp\n", __FUNCTION__, __FILE__, __LINE__);
			StopDHCP();
		}
		else
		{			//����/etc/resolv.conf�ļ�
			DVRSystem("rm /tmp/resolv.conf");
			DVRSystem("cp /etc/resolv.conf /tmp/");
		}

		//����DHCP����ʱֻ����5�ξ�ֹͣ�� ��ֹ�����ȴ�
		StartDHCP(const_cast<char *>("-t 5 -n"), "eth0");

		int dhcpTestStatus = ValidDHCPip();
		NET_CTRL_FAIL retNetCtrl = NETERR_NETPARAMCHECK_OK;

		if( DHCP_STATUS_VALID != dhcpTestStatus )
		{
			//����ʧ��
			retNetCtrl = NETERR_NETPARMACHECK_ENETUNREACHD;
		}
		
		StopDHCP();

		if( !m_networkAddrInfo.bDHCP )
		{
			//�ָ�ԭʼ�ľ�̬IP
			if( !m_pppoeInfo.bEnable )
			{
				NETWORK_ADDR_INFO addrInfo = m_networkAddrInfo;
				//��̬��ȡIP��ַ
				GetIPStatic( m_networkAddrInfo, "eth0" );

				CNetInterface::Instance()->SetDNS( m_firstStaticDNS, 1, "eth0" );
				CNetInterface::Instance()->SetDNS( m_secondStaticDNS, 2, "eth0" );
				if(g_curNet3G)
				{
					DVRSystem("cat /etc/ppp/resolv.conf >> /etc/resolv.conf");
				}

			}
			else
			{
				DVRSystem( "rm -rf /etc/resolv.conf");
				DVRSystem("mv /tmp/resolv.conf /etc/");
			}
		}
		else
		{
			StartDHCP(const_cast<char *>("-b"), "eth0");
		}

		char multicastRouteCmd[120] = {0};

		//�����鲥
		sprintf( multicastRouteCmd,"%s%s", _ADD_MULTICAST_ROUTE_CMD ,"eth0" );
		
		DVRSystem( multicastRouteCmd );
		
		res_init();

		m_changeNicCfgLock.UnLock();
		return retNetCtrl;
	}
	else
	{
		IP_CHECK_ERR ipCheckErr = CNetInterface::Instance()->IsValidIP( pNetworkAddrInfo->staticIP, pNetworkAddrInfo->netMask );
		
		switch( ipCheckErr )
		{
		case IP_CHECK_ERR_SUCC:
			{//����Ƿ��ǳ�ͻ��IP��ַ
				if( CNetInterface::Instance()->IPIsCollision( pNetworkAddrInfo->staticIP ) )
				{
					return NETERR_NETPARAMCHECK_EIPCONFLICT;
				}
				else
				{
					return NETERR_NETPARAMCHECK_OK;
				}
			}
			break;
		case IP_CHECK_ERR_IPINVALID:
			//IP��ַ������
			return NETERR_NETPARAMCHECK_EIPORSUBMASK;
			break;
		case IP_CHECK_ERR_NETMASK:
			//�����������
			return NETERR_NETPARAMCHECK_EIPORSUBMASK;
			break;
		default:
			assert(0);
			return NETERR_NETPARAMCHECK_EIPORSUBMASK;
			break;
		}
		
	}
}


/*****************************************************************

Function:allocNICIP(const GETIPMODE &getIPMode,const unsigned char *pGetipModeInfo, const char *pNICName)

Description:
			1:Ϊ����ӿڷ���IP��ַ

Calls:

Called by:

Table Accessed:

Table Updated:

Input:
	1:getIPMode, ���IP�ķ�ʽ�������ַ�ʽ��GETIPMODE_PPPOE��GETIPMODE_NPPPOE
	2:pGetipModeInfo,���ݲ�ͬ��getIPMode�ò����ֱ�ָ��NETWORK_ADDR_INFO����PPPOE_INFO���͵ĵ�ַ
	3:pNICName�д洢��ͬ����������

Output:

Return:

Others:
******************************************************************/


NET_CTRL_FAIL CAllocNICIP::allocNICIP(const GETIPMODE &getIPMode,const unsigned char *pGetipModeInfo, const char *pNICName )
{
	assert( NULL != pGetipModeInfo );
	assert( NULL != pNICName );

	NET_CTRL_FAIL netCtrlResult = NETERR_NETPARAMCHECK_OK;

	m_changeNicCfgLock.Lock();

	switch( getIPMode )
	{
		case GETIPMODE_NPPPOE:
		{
			const NETWORK_ADDR_INFO *pNetworkAddrInfo = reinterpret_cast< const NETWORK_ADDR_INFO *>( pGetipModeInfo );

			CNetInterface::Instance()->SetbDHCP( pNetworkAddrInfo->bDHCP );

			if( true == pNetworkAddrInfo->bDHCP )
			{
				//��̬��ȡIP��ַ
				netCtrlResult = GetIPDhcp( *pNetworkAddrInfo, pNICName );
				
			}
			else
			{
				//��̬��ȡIP��ַ
				netCtrlResult = GetIPStatic( *pNetworkAddrInfo, pNICName );

				#ifdef __SUPPORT_3G__
					if(!g_curNet3G)
				#else

				#endif
					{
						CNetInterface::Instance()->SetDNS( m_firstStaticDNS, 1, "eth0" );
						CNetInterface::Instance()->SetDNS( m_secondStaticDNS, 2, "eth0" );
					}

			}

			memset( &m_pppoeInfo, 0, sizeof( PPPOE_INFO ) );
			//IP��ַ���ı�ʱ��¼�ı���IP��ַ
			memcpy( &m_networkAddrInfo, pNetworkAddrInfo, sizeof( NETWORK_ADDR_INFO ) );

			break;
		}
		case GETIPMODE_PPPOE:
		{
			const PPPOE_INFO *pPppoeInfo = reinterpret_cast< const PPPOE_INFO *>( pGetipModeInfo );
			CNetInterface::Instance()->SetbPPPOE( pPppoeInfo->bEnable );
			
			if( pPppoeInfo->bEnable )
			{
				//ɾ����ǰ���أ�����pppoe����ping ������
				CNetInterface::Instance()->DelGateway( m_networkAddrInfo.gateway, 0,0 );

				netCtrlResult = GetIPPppoe( *pPppoeInfo, pNICName );

				memcpy( &m_pppoeInfo, pPppoeInfo, sizeof( PPPOE_INFO ) );

			}
			else
			{
				if( !m_networkAddrInfo.bDHCP )
				{
#ifdef __RELEASE__
					//�ڲ�ʹ��PPPOEʱ�� ����Ҫʹ�þ�̬IP����ʱ�������̬IP���ϴ�һ���Ͳ�����±��浽�����У�Ҳ�Ͳ�����Ч��������Ҫ�������þ�̬IP��Ϣ
					GetIPStatic( m_networkAddrInfo, pNICName );

					CNetInterface::Instance()->SetDNS( m_firstStaticDNS, 1, "eth0" );
					CNetInterface::Instance()->SetDNS( m_secondStaticDNS, 2, "eth0" );

					memset( &m_pppoeInfo, 0, sizeof( PPPOE_INFO ) ); 
#endif
				}
				
			}

			break; 
		}
		default:
		{
			assert(false);
			break;	
		}
	}


	char multicastRouteCmd[120] = {0};
	if( NULL != pNICName )
	{
		//�����鲥·��
		sprintf( multicastRouteCmd,"%s%s", _ADD_MULTICAST_ROUTE_CMD ,pNICName );
	}
	else
	{
		//Ĭ��ʹ��eth0
		sprintf( multicastRouteCmd,"%s%s", _ADD_MULTICAST_ROUTE_CMD ,"eth0" );
	}
	DVRSystem( multicastRouteCmd );

	//����IP��Ҫ���³�ʼ��_res�еı�������Ϊ����������ʱ��Ҫ�õ��ñ����е���Ϣ��
	res_init();

	m_changeNicCfgLock.UnLock();
	return netCtrlResult;
}

bool CAllocNICIP::RecoverNetIic(const char *pNicName/*="eth0"*/)
{
	m_changeNicCfgLock.Lock();

	if (m_pppoeInfo.bEnable)
	{
		//ʹ��ppoe��ʽʱ����
		printf("%s:%s:%d, ʹ��PPPoe��ȡIP����\n", __FUNCTION__, __FILE__, __LINE__);
		GetIPPppoe( m_pppoeInfo, pNicName );
	}
	else
	{
		if (m_networkAddrInfo.bDHCP)
		{
			//ʹ��dhcp��ʽ�ǳ���
			printf("%s:%s:%d, ʹ��dhcp��ʽ�ǳ���\n", __FUNCTION__, __FILE__, __LINE__);
			GetIPDhcp( m_networkAddrInfo, pNicName );
		}
		else
		{
			//��������
			printf("%s:%s:%d, ʹ�þ�̬IP��ʽ�ǳ���\n", __FUNCTION__, __FILE__, __LINE__);
			CNetInterface::Instance()->SetGateway( m_networkAddrInfo.gateway,0, 0, pNicName );
		}
	}

	//���ù㲥���ص�ַ
	char multicastRouteCmd[128] = {0};
	if( NULL != pNicName )
	{
		//�����鲥·��
		sprintf( multicastRouteCmd,"%s%s", _ADD_MULTICAST_ROUTE_CMD ,pNicName );
	}
	else
	{
		//Ĭ��ʹ��eth0
		sprintf( multicastRouteCmd,"%s%s", _ADD_MULTICAST_ROUTE_CMD ,"eth0" );
	}
	DVRSystem( multicastRouteCmd );

	//����IP��Ҫ���³�ʼ��_res�еı�������Ϊ����������ʱ��Ҫ�õ��ñ����е���Ϣ��
	res_init();

	m_changeNicCfgLock.UnLock();

	return true;
}

NET_CTRL_FAIL CAllocNICIP::GetDhcpIP(NETWORK_STATUS_INFO &info, const char *pNICName)
{
	NET_CTRL_FAIL netCtrlResult = NETERR_NETPARAMCHECK_OK;
	m_changeNicCfgLock.Lock();
	CNetInterface::Instance()->SetbDHCP(true);
	
	//�Ժ�̨��ʽ����udhcpc,����system() ���û�ȴ�udhcpc�ķ��ض��������̡߳�
	if( !StartDHCP(const_cast<char *>("-b"), pNICName) )
	{
		return NETERR_NETPARAMCHECK_EUNKNOWN;
	}
	//�ɹ����к�ȥ��ȡip;
	PUB_Sleep(50);
	CNetInterface::Instance()->GetLocalNICStatusInfo(info);

	//ֹͣDHCP
	CNetInterface::Instance()->SetbDHCP(false);
	StopDHCP();

	m_changeNicCfgLock.UnLock();
	return netCtrlResult;
}

NET_CTRL_FAIL CAllocNICIP::GetIPDhcp( const NETWORK_ADDR_INFO &networkAddrInfo, const char *pNICName )
{
	if( m_networkAddrInfo.bDHCP )
	{
		StopDHCP();
	}

	if( m_pppoeInfo.bEnable )
	{
		CPPPOEMng::Instance()->Stop();
	}

	//�Ժ�̨��ʽ����udhcpc,����system() ���û�ȴ�udhcpc�ķ��ض��������̡߳�
	if( !StartDHCP(const_cast<char *>("-b"), pNICName) )
	{
		return NETERR_NETPARAMCHECK_EUNKNOWN;
	}

	return NETERR_NETPARAMCHECK_OK;
}

NET_CTRL_FAIL CAllocNICIP::GetIPStatic( const NETWORK_ADDR_INFO &networkAddrInfo, const char *pNICName )
{
	if( m_networkAddrInfo.bDHCP )
	{
		StopDHCP();
	}

	if( m_pppoeInfo.bEnable )
	{
		CPPPOEMng::Instance()->Stop();
	}

	IP_SET_ERR ipSetRetval = CNetInterface::Instance()->SetIP( networkAddrInfo.staticIP, pNICName );

	NET_CTRL_FAIL netCtrlResult = NETERR_NETPARAMCHECK_OK;

	switch( ipSetRetval )
	{
	case IP_SET_ERR_SUCC:
		netCtrlResult = NETERR_NETPARAMCHECK_OK;
		break;
	case IP_SET_ERR_COLLISION:
		netCtrlResult = NETERR_NETPARAMCHECK_EIPCONFLICT;
		break;
	case IP_SET_ERR_INVALID:
		netCtrlResult = NETERR_NETPARAMCHECK_EIPORSUBMASK;
		break;
	case IP_SET_ERR_NO_DEVICE:
		netCtrlResult = NETERR_NETPARAMCHECK_ENONICDEVICE;
		break;
	default:
		break;
	}
	
	CNetInterface::Instance()->SetIPMask(networkAddrInfo.netMask, pNICName);

#ifdef __SUPPORT_3G__
	if(!g_curNet3G)
#endif
	{
		CNetInterface::Instance()->SetGateway( networkAddrInfo.gateway, 0, 0, pNICName );
	}

	return netCtrlResult;

}

NET_CTRL_FAIL CAllocNICIP::GetIPPppoe( const PPPOE_INFO &pppoeInfo, const char *pNICName)
{
#ifndef __RELEASE__
	return NETERR_NETPARAMCHECK_OK;
#endif

	//ֹͣpppoe
	if( m_pppoeInfo.bEnable )
	{
		CPPPOEMng::Instance()->Stop();
	}

	if( m_networkAddrInfo.bDHCP )
	{
		StopDHCP();
	}

	if( true == pppoeInfo.bEnable )
	{
		CPPPOEMng::Instance()->Start( pppoeInfo.user, pppoeInfo.password);
	}
	
	return NETERR_NETPARAMCHECK_OK;
}

/*
��⵱ǰ�Ƿ����е���DHCP��ʽ����������Ч��IP

*/

int CAllocNICIP::ValidDHCPip()
{
	//����Ƿ�������DHCP����
	if( !IsProcessRunning( const_cast<char *>("udhcpc")) )
	{
		return DHCP_STATUS_STOP;
	}

	//����Ƿ�����Ч��DHCP ip��ַ
	unsigned long dhcpIP = 0;
	if( CNetInterface::Instance()->GetIPAddress( dhcpIP ) )
	{
		if( !( IN_LOOPBACK( ntohl( dhcpIP ) ) ) )
		{
			return DHCP_STATUS_VALID;
		}
	}

	return DHCP_STATUS_RUNNING;
	
}






