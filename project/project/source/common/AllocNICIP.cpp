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
		检测PPPOE参数是否正确。
		1:网络要处于激活状态
		2:PPPOE用户名不能为空
		3:检测PPPOE用户名和密码是否有效。用户要保证直接用modeom直接和网络服务提供商连接

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
		//检测网络接口是否处于UP状态
		return NETERR_NETPARAMCHECK_ECONNECT;
	}

	if( 0 == strlen( pPppoeInfo->user ) )
	{
		//用户名不能为空
		return NETERR_NETPARAMCHECK_EUSERNAMEEMPTY;
	}
	
	if( pPppoeInfo->bEnable )
	{
		m_changeNicCfgLock.Lock();
		if( !m_pppoeInfo.bEnable )
		{
			//保存/etc/resolv.conf文件
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
			//如果之前PPPOE在运行中就从新打开pppoe
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
		检测设置的IP是否合法。
		1:如果是动态获取IP地址就不进行检测
		2:使用静态IP的时候要检测IP的合法性和是否和网内其他主机的IP地址冲突

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
			//检测网络接口是否处于UP状态
			m_changeNicCfgLock.UnLock();
			return NETERR_NETPARAMCHECK_ECONNECT;
		}
		
		if( m_networkAddrInfo.bDHCP )
		{
			//如果DHCP处于运行中，需要先停止DHCP
			printf("%s:%s:%d, stop dhcp\n", __FUNCTION__, __FILE__, __LINE__);
			StopDHCP();
		}
		else
		{			//保存/etc/resolv.conf文件
			DVRSystem("rm /tmp/resolv.conf");
			DVRSystem("cp /etc/resolv.conf /tmp/");
		}

		//测试DHCP功能时只发送5次就停止， 防止过长等待
		StartDHCP(const_cast<char *>("-t 5 -n"), "eth0");

		int dhcpTestStatus = ValidDHCPip();
		NET_CTRL_FAIL retNetCtrl = NETERR_NETPARAMCHECK_OK;

		if( DHCP_STATUS_VALID != dhcpTestStatus )
		{
			//测试失败
			retNetCtrl = NETERR_NETPARMACHECK_ENETUNREACHD;
		}
		
		StopDHCP();

		if( !m_networkAddrInfo.bDHCP )
		{
			//恢复原始的静态IP
			if( !m_pppoeInfo.bEnable )
			{
				NETWORK_ADDR_INFO addrInfo = m_networkAddrInfo;
				//静态获取IP地址
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

		//设置组播
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
			{//检测是否是冲突的IP地址
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
			//IP地址不可用
			return NETERR_NETPARAMCHECK_EIPORSUBMASK;
			break;
		case IP_CHECK_ERR_NETMASK:
			//网络掩码错误
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
			1:为网络接口分配IP地址

Calls:

Called by:

Table Accessed:

Table Updated:

Input:
	1:getIPMode, 获得IP的方式，有两种方式，GETIPMODE_PPPOE和GETIPMODE_NPPPOE
	2:pGetipModeInfo,根据不同的getIPMode该参数分别指向NETWORK_ADDR_INFO或者PPPOE_INFO类型的地址
	3:pNICName中存储不同网卡的名称

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
				//动态获取IP地址
				netCtrlResult = GetIPDhcp( *pNetworkAddrInfo, pNICName );
				
			}
			else
			{
				//静态获取IP地址
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
			//IP地址被改变时记录改变后的IP地址
			memcpy( &m_networkAddrInfo, pNetworkAddrInfo, sizeof( NETWORK_ADDR_INFO ) );

			break;
		}
		case GETIPMODE_PPPOE:
		{
			const PPPOE_INFO *pPppoeInfo = reinterpret_cast< const PPPOE_INFO *>( pGetipModeInfo );
			CNetInterface::Instance()->SetbPPPOE( pPppoeInfo->bEnable );
			
			if( pPppoeInfo->bEnable )
			{
				//删除以前网关，否则pppoe不能ping 到外网
				CNetInterface::Instance()->DelGateway( m_networkAddrInfo.gateway, 0,0 );

				netCtrlResult = GetIPPppoe( *pPppoeInfo, pNICName );

				memcpy( &m_pppoeInfo, pPppoeInfo, sizeof( PPPOE_INFO ) );

			}
			else
			{
				if( !m_networkAddrInfo.bDHCP )
				{
#ifdef __RELEASE__
					//在不使用PPPOE时， 并且要使用静态IP，这时候如果静态IP和上次一样就不会从新保存到配置中，也就不会生效，这里需要从新设置静态IP信息
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
		//设置组播路由
		sprintf( multicastRouteCmd,"%s%s", _ADD_MULTICAST_ROUTE_CMD ,pNICName );
	}
	else
	{
		//默认使用eth0
		sprintf( multicastRouteCmd,"%s%s", _ADD_MULTICAST_ROUTE_CMD ,"eth0" );
	}
	DVRSystem( multicastRouteCmd );

	//跟新IP后要从新初始化_res中的变量，因为域名解析的时候要用到该变量中的信息，
	res_init();

	m_changeNicCfgLock.UnLock();
	return netCtrlResult;
}

bool CAllocNICIP::RecoverNetIic(const char *pNicName/*="eth0"*/)
{
	m_changeNicCfgLock.Lock();

	if (m_pppoeInfo.bEnable)
	{
		//使用ppoe方式时出错
		printf("%s:%s:%d, 使用PPPoe获取IP出错\n", __FUNCTION__, __FILE__, __LINE__);
		GetIPPppoe( m_pppoeInfo, pNicName );
	}
	else
	{
		if (m_networkAddrInfo.bDHCP)
		{
			//使用dhcp方式是出错
			printf("%s:%s:%d, 使用dhcp方式是出错\n", __FUNCTION__, __FILE__, __LINE__);
			GetIPDhcp( m_networkAddrInfo, pNicName );
		}
		else
		{
			//设置网关
			printf("%s:%s:%d, 使用静态IP方式是出错\n", __FUNCTION__, __FILE__, __LINE__);
			CNetInterface::Instance()->SetGateway( m_networkAddrInfo.gateway,0, 0, pNicName );
		}
	}

	//设置广播网关地址
	char multicastRouteCmd[128] = {0};
	if( NULL != pNicName )
	{
		//设置组播路由
		sprintf( multicastRouteCmd,"%s%s", _ADD_MULTICAST_ROUTE_CMD ,pNicName );
	}
	else
	{
		//默认使用eth0
		sprintf( multicastRouteCmd,"%s%s", _ADD_MULTICAST_ROUTE_CMD ,"eth0" );
	}
	DVRSystem( multicastRouteCmd );

	//跟新IP后要从新初始化_res中的变量，因为域名解析的时候要用到该变量中的信息，
	res_init();

	m_changeNicCfgLock.UnLock();

	return true;
}

NET_CTRL_FAIL CAllocNICIP::GetDhcpIP(NETWORK_STATUS_INFO &info, const char *pNICName)
{
	NET_CTRL_FAIL netCtrlResult = NETERR_NETPARAMCHECK_OK;
	m_changeNicCfgLock.Lock();
	CNetInterface::Instance()->SetbDHCP(true);
	
	//以后台方式运行udhcpc,否则system() 调用会等待udhcpc的返回而阻塞主线程。
	if( !StartDHCP(const_cast<char *>("-b"), pNICName) )
	{
		return NETERR_NETPARAMCHECK_EUNKNOWN;
	}
	//成功运行后去获取ip;
	PUB_Sleep(50);
	CNetInterface::Instance()->GetLocalNICStatusInfo(info);

	//停止DHCP
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

	//以后台方式运行udhcpc,否则system() 调用会等待udhcpc的返回而阻塞主线程。
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

	//停止pppoe
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
检测当前是否运行的是DHCP方式，并且有有效的IP

*/

int CAllocNICIP::ValidDHCPip()
{
	//检测是否运行了DHCP进程
	if( !IsProcessRunning( const_cast<char *>("udhcpc")) )
	{
		return DHCP_STATUS_STOP;
	}

	//检测是否有有效的DHCP ip地址
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






