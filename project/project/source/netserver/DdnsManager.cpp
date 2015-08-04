#include "DdnsManager.h"
#include "DdnsMeibu.h"
#include "DdnsDvrdydns.h"
#include "DdnsDyndns.h"
#include "DdnsNoip.h"
#include "DdnsVisionica.h"
#include "DdnsTecvoz.h"
#include "DdnsMintAuto.h"
#include "DdnsHiView.h"
#include "DdnsCNB.h"

#include "DdnsTVTIPServerClient.h"


CDdnsManager * CDdnsManager::Instance()
{
	static CDdnsManager ddnsManager;
	return &ddnsManager;
}

CDdnsManager::CDdnsManager()
{
	
}


CDdnsManager::~CDdnsManager()
{

}

//读取配置，获得当前使用的DDNS
bool CDdnsManager::Inital()
{
	memset( &m_DdnsServerType, 0, sizeof(DDNS_SERVER_TYPE));
	memset( &m_DdnsServerInfo, 0, sizeof(DDNS_SERVER_INFO));
	memset( &m_DdnsClientInfo, 0, sizeof( DDNS_INFO ) );
	memset( &m_CheckClientInfoEx, 0 , sizeof( DDNS_INFO_EX ) );

	m_pDdnsClient = new CDdnsClient;
	m_bUseDdns = false;
	GetLocalNetworkInfo( &m_LocalNetInfo );

	m_bUpdateOK = false;
	m_lastCheckUpdateTime = GetCurrTime32();

	m_sleepCount		= 0;

	memset(m_deviceName, 0, sizeof(m_deviceName));
	m_httpPort = 0;
	m_serverPort = 0;

	memset(m_mintDDNSServerName, 0, sizeof(m_mintDDNSServerName));

	m_bLocalInfoChange = false;
	
	return true;
	
}

void CDdnsManager::Quit()
{
	if( NULL != m_pDdnsClient )
	{
		delete m_pDdnsClient;
	}
	m_bUseDdns = false;
}


bool CDdnsManager::Start()
{
	m_hIpUpdateID = PUB_CreateThread( IpUpdateThread, (void *)this, &m_bIpUpdateProc );	
	if( PUB_CREATE_THREAD_FAIL == m_hIpUpdateID )
	{
		PUB_PrintError(__FILE__,__LINE__);
		return false;
	}	 	
	return true;
}
void CDdnsManager::Stop()
{
	if ( PUB_THREAD_ID_NOINIT != m_hIpUpdateID )
	{		
		PUB_ExitThread( &m_hIpUpdateID, &m_bIpUpdateProc );	
	}
}


bool CDdnsManager::ChangeMintDDNSName(const unsigned char *pMintDDNSName)
{
	if (NULL == pMintDDNSName)
	{
		return false;
	}
	else
	{
		m_DdnsLock.Lock();
		strcpy((char *)m_mintDDNSServerName, (const char *)pMintDDNSName);
		if ((DDNS_MINTDNS == m_DdnsClientInfo.type) && (NULL != m_pDdnsClient))
		{
			m_pDdnsClient->SetDDNSServerName((const char *)pMintDDNSName);
		}
		m_DdnsLock.UnLock();
		return true;
	}
}

void CDdnsManager::ChangeDDNSClient(const DDNS_INFO &DdnsClientInfo)
{
	m_DdnsLock.Lock();

	memcpy( &m_DdnsClientInfo, &DdnsClientInfo, sizeof( DDNS_INFO ) );

	m_DdnsServerType = static_cast<const DDNS_SERVER_TYPE>(DdnsClientInfo.type);
	assert( m_DdnsServerType >= DDNS_NONE );
	assert( m_DdnsServerType <= DDNS_END );
	
	m_bUseDdns = ( ( DDNS_NONE != m_DdnsServerType ) &&( true == DdnsClientInfo.bEnable ) )?true:false;

	if( NULL != m_pDdnsClient )
	{
		delete m_pDdnsClient;
	}

	switch( m_DdnsServerType )
	{
	case DDNS_MEIBU:
		{
			m_pDdnsClient = new CDdnsMeibu;
			break;
		}
	case DDNS_DVRDYDNS:
		{
			m_pDdnsClient = new CDdnsDvrdydns;
			m_pDdnsClient->SetDDNSServerName("www.dvrdydns.com");
			break;
		}
	case DDNS_MYQSEE:
		{
			m_pDdnsClient = new CDdnsDvrdydns;
			m_pDdnsClient->SetDDNSServerName("myq-see.com");
			break;
		}
	case DDNS_DVRLIS:
		{
			m_pDdnsClient = new CDdnsDvrdydns;
			m_pDdnsClient->SetDDNSServerName("ns1.dvrlists.com");
			break;
		}
	case DDNS_EASTERNDNS:
		{
			m_pDdnsClient = new CDdnsDvrdydns;
			m_pDdnsClient->SetDDNSServerName("www.easterndns.com");
			break;
		}
	case DDNS_NEWDDNS:
		{
			m_pDdnsClient = new CDdnsDvrdydns;
			m_pDdnsClient->SetDDNSServerName("www.newddns.com");
			break;
		}
	case DDNS_DYNDNS:
		{
			m_pDdnsClient = new CDdnsDyndns;
			m_pDdnsClient->SetDDNSServerName("members.dyndns.org");
			m_sleepCount = (30 * 24 * 60 * 60 ) / 10;	//30天更新一次
			break;
		}
	case DDNS_MINTDNS:
		{
			m_pDdnsClient = new CDdnsDvrdydns;
			m_pDdnsClient->SetDDNSServerName((const char *)m_mintDDNSServerName);
			break;
		}
	case DDNS_NOIPDNS:
		{
			m_pDdnsClient = new CDdnsNoip;
			break;
		}
	case DDNS_VISIONICA:
		{
			m_pDdnsClient = new CDdnsVisionica;
			break;
		}
	case DDNS_TVT_IP_SERVER:
		{
			m_pDdnsClient = new CDdnsTVTIPServerClient;
			m_pDdnsClient->SetDeviceName(m_deviceName);
			m_pDdnsClient->SetHttpPort(m_httpPort);
			m_pDdnsClient->SetServerPort(m_serverPort);
			m_sleepCount = (DEV_TIMEOUT * 1000) / SLEEP_LENGTH;

			break;
		}
	case DDNS_MINCO:
		{
			m_pDdnsClient = new CDdnsDyndns;
			m_pDdnsClient->SetDDNSServerName("members.jflddns.com.br");	
		}
		break;
	case DDNS_TECVOZ:
		{
 			m_pDdnsClient = new CDdnsTecvoz;
 			m_pDdnsClient->SetDeviceName(m_deviceName);
 			m_pDdnsClient->SetHttpPort(m_httpPort);
 			m_pDdnsClient->SetServerPort(m_serverPort);
 			m_pDdnsClient->SetDDNSServerName("members.tecvozddns.com.br");

			m_sleepCount = (30 * 24 * 60 * 60 ) / 10;	//30天更新一次
		}
		break;
	case DDNS_WINCO_BR27:
		{
			m_pDdnsClient = new CDdnsDyndns;
			m_pDdnsClient->SetDDNSServerName("members.ddns.com.br");
			char user[132] = {0};
			char domain[132] = {0};
			snprintf(user, sizeof(user), "%s.ddns.com.br", m_DdnsClientInfo.user);
			snprintf(domain, sizeof(domain), "%s.ddns.com.br", m_DdnsClientInfo.domain);

			snprintf(m_DdnsClientInfo.user, sizeof(m_DdnsClientInfo.user), "%s", user);
			snprintf(m_DdnsClientInfo.domain, sizeof(m_DdnsClientInfo.domain), "%s", domain);
		}
		break;
	case DDNS_MINT_AUTO:
		{
			char mac[12] = {0};
			CNetInterface::Instance()->GetMAC(mac);
			char macStr[20] = {0};
			snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);

			m_pDdnsClient = new CDdnsMintAuto;
			m_pDdnsClient->SetMac(macStr);
#if defined(__CUSTOM_IN23__)
			m_pDdnsClient->SetDDNSServerName("www.cpplusddns.com");
#else
			m_pDdnsClient->SetDDNSServerName("www.autoddns.com");
#endif
			
		
			char domain[120] = {0};
#if defined(__CUSTOM_IN23__)
			snprintf(domain, sizeof(domain), "%s.cpplusddns.com", m_DdnsClientInfo.domain);
#else
			snprintf(domain, sizeof(domain), "%s.autoddns.com", m_DdnsClientInfo.domain);
#endif
			snprintf(m_DdnsClientInfo.user, sizeof(m_DdnsClientInfo.user), "admin");
			snprintf(m_DdnsClientInfo.domain, sizeof(m_DdnsClientInfo.domain), "%s", domain);
			snprintf(m_DdnsClientInfo.password, sizeof(m_DdnsClientInfo.password), "password");
		}
		break;
	case DDNS_HIVIEW:
		{
			m_pDdnsClient = new CDdnsHiView;
			
			m_pDdnsClient->SetDDNSServerName("www.hiview.org");
		}
		break;
	case DDNS_MX15:
		{
			m_pDdnsClient = new CDdnsDvrdydns;
			m_pDdnsClient->SetHttpPort(m_httpPort);
			m_pDdnsClient->SetServerPort(m_serverPort);
			m_pDdnsClient->SetDDNSServerName("mymeriva.com");
			break;
		}
	case DDNS_VN11:
		{
			m_pDdnsClient = new CDdnsDvrdydns;
			m_pDdnsClient->SetDDNSServerName("questek.tv");
			break;
		}
	case DDNS_CNB:
		{
			m_pDdnsClient = new CDdnsCNB;
			m_pDdnsClient->SetHttpPort(m_httpPort);
			m_pDdnsClient->SetServerPort(m_serverPort);
			break;
		}
	default:
		{
			m_pDdnsClient = new CDdnsClient;
			break;
		}
	}

	CNetInterface::Instance()->SetbDDNS( m_bUseDdns );
	m_DdnsLock.UnLock();
}

void CDdnsManager::SetDeviceName(const char *pDeviceName)
{
	strcpy(m_deviceName, pDeviceName);
	
	if (NULL != m_pDdnsClient)
	{
		m_pDdnsClient->SetDeviceName(pDeviceName);
	}
}

void CDdnsManager::SetHttpPort(unsigned short httpPort)
{
	m_httpPort = httpPort;
	
	if (NULL != m_pDdnsClient)
	{
		m_pDdnsClient->SetHttpPort(httpPort);
		m_bLocalInfoChange = true;
	}
}

void CDdnsManager::SetServerPort(unsigned short serverPort)
{
	m_serverPort = serverPort;

	if (NULL != m_pDdnsClient)
	{
		m_pDdnsClient->SetServerPort(serverPort);
		m_bLocalInfoChange = true;
	}
}
/******************************************************************************************************

Function:CheckDDNSClientInfo(const DDNS_INFO &DdnsClientInfo)

Description:
			1:在DDNS信息保存到配置文件前检查这些信息的正确性
Calls:

Called by:

Table Accessed:

Table Updated:

Input:

Output:

Return:

Others:
********************************************************************************************************/
NET_CTRL_FAIL CDdnsManager::CheckDDNSClientInfo(DDNS_INFO_EX &DdnsClientInfo)
{
	if( ( !DdnsClientInfo.bEnable ) || ( DDNS_NONE == DdnsClientInfo.type ) )
	{
		m_bUpdateOK = false;
		return NETERR_NETPARAMCHECK_OK;
	}
	
	//检测网络接口是否处于UP状态
#ifdef __SUPPORT_3G__
	if (!CNetInterface::Instance()->GetNICRunning("ppp0"))
	{
		if( !CNetInterface::Instance()->GetNICRunning() )
		{
			m_bUpdateOK = false;
			return NETERR_NETPARAMCHECK_ECONNECT;
		}
	}
#else
	if( !CNetInterface::Instance()->GetNICRunning() )
	{
		m_bUpdateOK = false;
		return NETERR_NETPARAMCHECK_ECONNECT;
	}
#endif

	//用户名不能为空
	if( (DDNS_MINT_AUTO != DdnsClientInfo.type) && (0 == strlen( DdnsClientInfo.user )) )
	{
		m_bUpdateOK = false;
		return NETERR_NETPARAMCHECK_EUSERNAMEEMPTY;
	}

	//更换了测试协议、用户名、密码或者域名
	if(memcmp(&DdnsClientInfo, &m_CheckClientInfoEx, sizeof(DDNS_INFO)))
	{
		m_bUpdateOK = false;
		
		//printf("%s:%s:%d, 更改了DDNS协议, 老的协议为:%d, 新的协议为:%d\n", __FUNCTION__, __FILE__, __LINE__,m_CheckClientInfo.type, DdnsClientInfo.type);
		//printf("%s:%s:%d, 更改了DDNS用户名, 老的用户名为:%s, 新的用户名为:%s\n", __FUNCTION__, __FILE__, __LINE__,m_CheckClientInfo.user, DdnsClientInfo.user);
		//printf("%s:%s:%d, 更改了DDNS密码, 老的密码为:%s, 新的密码为:%s\n", __FUNCTION__, __FILE__, __LINE__,m_CheckClientInfo.password, DdnsClientInfo.password);
		//printf("%s:%s:%d, 更改了DDNS域名, 老的域名为:%s, 新的域名为:%s\n", __FUNCTION__, __FILE__, __LINE__,m_CheckClientInfo.domain, DdnsClientInfo.domain);
		m_CheckClientInfoEx = DdnsClientInfo;
	}

	NET_CTRL_FAIL netCtrlResult = NETERR_NETPARAMCHECK_OK;

	switch( DdnsClientInfo.type )
	{
	case DDNS_MEIBU:
		{

			if( !m_bUpdateOK || ( GetCurrTime32() > ( m_lastCheckUpdateTime + meibuTestHoldTime ) ) )
			{
				//数据更新或者超时就需要从新上报服务器
				CDdnsMeibu ddnsMeibu;
				int ret = ddnsMeibu.DdnsIPupdate( DdnsClientInfo.user, DdnsClientInfo.password, DdnsClientInfo.domain );
				
				if( 0 == ret )
				{
					m_bUpdateOK = true;
					m_lastCheckUpdateTime = GetCurrTime32();
				}
				else
				{
					m_bUpdateOK = false;
				}

				netCtrlResult = static_cast< NET_CTRL_FAIL >(  ret  + NETERR_NETPARAMCHECK_OK );
			}

			

		}
		break;
	case DDNS_DYNDNS:
		{
			//限制频繁上测试DYDDNS，在成功后的5分钟内都反回成功
			if( !m_bUpdateOK || ( GetCurrTime32() > ( m_lastCheckUpdateTime + meibuTestHoldTime ) ) )
			{
				CDdnsDyndns ddnsDyndns;
				ddnsDyndns.SetDDNSServerName("members.dyndns.org");
				int ret = ddnsDyndns.DdnsIPupdate( DdnsClientInfo.user, DdnsClientInfo.password, DdnsClientInfo.domain );

				if( 0 == ret )
				{
					m_bUpdateOK = true;
					m_lastCheckUpdateTime = GetCurrTime32();
				}
				else
				{
					m_bUpdateOK = false;
				}

				netCtrlResult = static_cast< NET_CTRL_FAIL >(  ret  + NETERR_NETPARAMCHECK_OK );
			}
		}
		break;
	case DDNS_DVRDYDNS:
		{
			CDdnsDvrdydns ddnsDvrdydns;
			if(ddnsDvrdydns.SetDDNSServerName("www.dvrdydns.com"))
			{
				int ret = ddnsDvrdydns.DdnsIPupdate( DdnsClientInfo.user, DdnsClientInfo.password, DdnsClientInfo.domain );

				netCtrlResult = static_cast< NET_CTRL_FAIL >(  ret  + NETERR_NETPARAMCHECK_OK );

				m_bUpdateOK = false;
			}
		}
		break;
	case DDNS_MINTDNS:
		{
			CDdnsDvrdydns ddnsDvrdydns;
			if(ddnsDvrdydns.SetDDNSServerName(DdnsClientInfo.serverDNS))
			{
				int ret = ddnsDvrdydns.DdnsIPupdate( DdnsClientInfo.user, DdnsClientInfo.password, DdnsClientInfo.domain );

				netCtrlResult = static_cast< NET_CTRL_FAIL >(  ret  + NETERR_NETPARAMCHECK_OK );

				m_bUpdateOK = false;
			}
			else
			{
				netCtrlResult = NETERR_NETPARAMCHECK_REGISTER_FAIL;
			}
		}
		break;
	case DDNS_MYQSEE:
		{
			CDdnsDvrdydns ddnsMyqSee;
			if(ddnsMyqSee.SetDDNSServerName("myq-see.com"))
			{
				int ret = ddnsMyqSee.DdnsIPupdate( DdnsClientInfo.user, DdnsClientInfo.password, DdnsClientInfo.domain );

				netCtrlResult = static_cast< NET_CTRL_FAIL >(  ret  + NETERR_NETPARAMCHECK_OK );

				m_bUpdateOK = false;
			}
		}
		break;
	case DDNS_DVRLIS:
		{
			CDdnsDvrdydns ddnsMyqSee;
			if(ddnsMyqSee.SetDDNSServerName("ns1.dvrlists.com"))
			{
				int ret = ddnsMyqSee.DdnsIPupdate( DdnsClientInfo.user, DdnsClientInfo.password, DdnsClientInfo.domain );

				netCtrlResult = static_cast< NET_CTRL_FAIL >(  ret  + NETERR_NETPARAMCHECK_OK );

				m_bUpdateOK = false;
			}
		}
		break;
	case DDNS_EASTERNDNS:
		{
			CDdnsDvrdydns ddnsMyqSee;
			if(ddnsMyqSee.SetDDNSServerName("www.easterndns.com"))
			{
				int ret = ddnsMyqSee.DdnsIPupdate( DdnsClientInfo.user, DdnsClientInfo.password, DdnsClientInfo.domain );

				netCtrlResult = static_cast< NET_CTRL_FAIL >(  ret  + NETERR_NETPARAMCHECK_OK );

				m_bUpdateOK = false;
			}
		}
		break;
	case DDNS_NEWDDNS:
		{
			CDdnsDvrdydns ddnsNewDdns;
			if(ddnsNewDdns.SetDDNSServerName("www.newddns.com"))
			{
				int ret = ddnsNewDdns.DdnsIPupdate( DdnsClientInfo.user, DdnsClientInfo.password, DdnsClientInfo.domain );

				netCtrlResult = static_cast< NET_CTRL_FAIL >(  ret  + NETERR_NETPARAMCHECK_OK );

				m_bUpdateOK = false;
			}
		}
		break;
	case DDNS_NOIPDNS:
		{
			CDdnsNoip ddnsNoip;
			{
				int ret = ddnsNoip.DdnsIPupdate( DdnsClientInfo.user, DdnsClientInfo.password, DdnsClientInfo.domain );

				netCtrlResult = static_cast< NET_CTRL_FAIL >(  ret  + NETERR_NETPARAMCHECK_OK );

				m_bUpdateOK = false;
			}
		}
		break;
	case DDNS_VISIONICA:
		{
			CDdnsVisionica ddnsVisionica;
			int ret = ddnsVisionica.RegisterAccount(DdnsClientInfo.user, DdnsClientInfo.domain);

			netCtrlResult = static_cast< NET_CTRL_FAIL >(  ret  + NETERR_NETPARAMCHECK_OK );

			m_bUpdateOK = false;
		}
		break;
	case DDNS_TVT_IP_SERVER:
		{
			CDdnsTVTIPServerClient tvtIPServer;
			tvtIPServer.SetDeviceName(m_deviceName);
			tvtIPServer.SetHttpPort(m_httpPort);
			tvtIPServer.SetServerPort(m_serverPort);

			int ret = tvtIPServer.DdnsIPupdate( DdnsClientInfo.user, DdnsClientInfo.password, DdnsClientInfo.domain );

			netCtrlResult = static_cast< NET_CTRL_FAIL >(  ret  + NETERR_NETPARAMCHECK_OK );

			m_bUpdateOK = false;
		}
		break;
	case DDNS_MINCO:
		{
			CDdnsDyndns ddnsNewDdns;
			if(ddnsNewDdns.SetDDNSServerName("members.jflddns.com.br"))
			{
				int ret = ddnsNewDdns.DdnsIPupdate(DdnsClientInfo.user, DdnsClientInfo.password, DdnsClientInfo.domain);

				netCtrlResult = static_cast< NET_CTRL_FAIL >(  ret  + NETERR_NETPARAMCHECK_OK );

				m_bUpdateOK = false;
			}
		}
		break;
	case DDNS_TECVOZ:
		{
			CDdnsTecvoz ddnsTecvozDdns;
			if(ddnsTecvozDdns.SetDDNSServerName("members.tecvozddns.com.br"))
			{
				int ret = ddnsTecvozDdns.DdnsIPupdate(DdnsClientInfo.user, DdnsClientInfo.password, DdnsClientInfo.domain);

				netCtrlResult = static_cast< NET_CTRL_FAIL >(  ret  + NETERR_NETPARAMCHECK_OK );

				m_bUpdateOK = false;
			}
		}
		break;
	case DDNS_WINCO_BR27:
		{
			CDdnsDyndns ddnsNewDdns;
			if(ddnsNewDdns.SetDDNSServerName("members.ddns.com.br"))
			{
				char user[120] = {0};
				char domain[120] = {0};
				snprintf(user, sizeof(user), "%s.ddns.com.br", DdnsClientInfo.user);
				snprintf(domain, sizeof(domain), "%s.ddns.com.br", DdnsClientInfo.domain);

				int ret = ddnsNewDdns.DdnsIPupdate(user, DdnsClientInfo.password, domain);

				netCtrlResult = static_cast< NET_CTRL_FAIL >(  ret  + NETERR_NETPARAMCHECK_OK );

				m_bUpdateOK = false;
			}
		}
		break;
	case DDNS_MINT_AUTO:
		{
			char mac[12] = {0};
			CNetInterface::Instance()->GetMAC(mac);
			char macStr[20] = {0};
			snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
			CDdnsMintAuto ddnsMintAuto;
			ddnsMintAuto.SetMac(macStr);
#if defined(__CUSTOM_IN23__)
			ddnsMintAuto.SetDDNSServerName("www.cpplusddns.com");
#else
			ddnsMintAuto.SetDDNSServerName("www.autoddns.com");
#endif	
			int ret = ddnsMintAuto.DdnsAutoReg(DdnsClientInfo.domain);
			
			netCtrlResult = static_cast< NET_CTRL_FAIL >(  ret  + NETERR_NETPARAMCHECK_OK );

			m_bUpdateOK = false;
		}
		break;
	case DDNS_HIVIEW:
		{
			CDdnsHiView hiview;

			hiview.SetDDNSServerName("www.hiview.org");

			int ret = hiview.DdnsIPupdate(DdnsClientInfo.user, DdnsClientInfo.password, DdnsClientInfo.domain);

			netCtrlResult = static_cast< NET_CTRL_FAIL >(  ret  + NETERR_NETPARAMCHECK_OK );

			m_bUpdateOK = false;
		}
		break;
	case DDNS_MX15:
		{
			CDdnsDvrdydns ddnmx15;
			if(ddnmx15.SetDDNSServerName("mymeriva.com"))
			{
				int ret = ddnmx15.DdnsIPupdate( DdnsClientInfo.user, DdnsClientInfo.password, DdnsClientInfo.domain );

				netCtrlResult = static_cast< NET_CTRL_FAIL >(  ret  + NETERR_NETPARAMCHECK_OK );

				m_bUpdateOK = false;
			}
			break;
		}
	case DDNS_VN11:
		{
			CDdnsDvrdydns ddnsVn11;
			if(ddnsVn11.SetDDNSServerName("questek.tv"))
			{
				int ret = ddnsVn11.DdnsIPupdate( DdnsClientInfo.user, DdnsClientInfo.password, DdnsClientInfo.domain );

				netCtrlResult = static_cast< NET_CTRL_FAIL >(  ret  + NETERR_NETPARAMCHECK_OK );

				m_bUpdateOK = false;
			}
			break;
		}
	case DDNS_CNB:
		{
			CDdnsCNB ddnsCNB;
			ddnsCNB.SetHttpPort(m_httpPort);
			ddnsCNB.SetServerPort(m_serverPort);
			int ret = ddnsCNB.DdnsAutoReg(DdnsClientInfo.user, DdnsClientInfo.password, DdnsClientInfo.domain);

			netCtrlResult = static_cast< NET_CTRL_FAIL >(  ret  + NETERR_NETPARAMCHECK_OK );

			m_bUpdateOK = false;
		}
		break;	
	default:
		assert( false );
		break;
	}

	return netCtrlResult;

}

void CDdnsManager::ChangeUpdateCycle(unsigned long updateCycle)
{
	//updateCycle 的单位是分钟

	m_DdnsLock.Lock();
	
	if (DDNS_TVT_IP_SERVER == m_DdnsServerType)
	{
	}
	else if (DDNS_DYNDNS == m_DdnsServerType)
	{
	}
	else if(DDNS_TECVOZ == m_DdnsServerType)
	{
	}
	else
	{
		m_sleepCount = (updateCycle * 60 * 1000) / SLEEP_LENGTH;
	}
	m_DdnsLock.UnLock();
}

bool CDdnsManager::DDNSAutoRegRemove()
{
#if defined(__CUSTOM_IN23__)
	return m_pDdnsClient->RemoveDDNS();
#endif
	return false;
}

PUB_THREAD_RESULT PUB_THREAD_CALL CDdnsManager::IpUpdateThread(void *pParam)
{
#ifdef	__ENVIRONMENT_LINUX__
	pid_t pid = getpid();
	pid_t tid = syscall(__NR_gettid);
	printf("%s, %d, pid = %d, tid=%d\n", __FILE__, __LINE__, pid, tid);
#endif

	CDdnsManager *pDdnsManager = reinterpret_cast<CDdnsManager *>(pParam);
	pDdnsManager->IpUpdateProc();
	return 0;
}


/*****************************************************************

Function:IpUpdateProc()

Description:控制和DDNS服务器的连接。
			在下面两种情况下更新DDNS  
			1：本地IP地址改变的时候
			2：每过10分钟不管IP地址是否改变都更新DDNS

Calls:

Called by:

Table Accessed:

Table Updated:

Input:

Output:

Return:

Others:
******************************************************************/
//
//
void CDdnsManager::IpUpdateProc()
{
	LOCOALNETWORKINFO localNetworkInfo;
	char externalIP[20];
	int notUpdateTime = 0;							//当连续5次(一个小时)没连上DDNS就表示不可用DDNS了 
	int count = 0;

	m_DdnsLock.Lock();
	if (DDNS_TVT_IP_SERVER == m_DdnsServerType)
	{
		m_sleepCount = 2;
	}
	else if (DDNS_DYNDNS == m_DdnsServerType)
	{
		m_sleepCount = (30 * 24 * 60 * 60 ) / 10;	//30天更新一次
		count = DEFAULT_SLEEP_COUNT / 2;
	}
	else
	{
		if(DEFAULT_SLEEP_COUNT > m_sleepCount)
		{
			m_sleepCount = DEFAULT_SLEEP_COUNT;
			count = DEFAULT_SLEEP_COUNT / 2;
		}
	}

	m_DdnsLock.UnLock();

	//每隔10s查看一次IP是否改变，
	while( m_bIpUpdateProc )
	{
		m_DdnsLock.Lock();
		
		//没有开启DDNS功能的时候该线程什么也不做
		if( false == m_bUseDdns )
		{	
			m_DdnsLock.UnLock();
			notUpdateTime = 0;
			CNetInterface::Instance()->SetbDDNS( ( true == m_DdnsClientInfo.bEnable )?CANNOTUSEDDNS:0x0 );
			PUB_Sleep(3000);
			continue;
		}
		
		//检测网线是否插上
#ifdef __SUPPORT_3G__
		if (!CNetInterface::Instance()->GetNICRunning("ppp0"))
		{
			if( !CNetInterface::Instance()->GetNICRunning() )
			{
				m_DdnsLock.UnLock();
				PUB_Sleep( 10000 );
				notUpdateTime = 0;
				CNetInterface::Instance()->SetbDDNS( CANNOTUSEDDNS );
				continue;
			}
		}
#else
		if( !CNetInterface::Instance()->GetNICRunning() )
		{
			m_DdnsLock.UnLock();
			PUB_Sleep( 10000 );
			notUpdateTime = 0;
			CNetInterface::Instance()->SetbDDNS( CANNOTUSEDDNS );
			continue;
		}
#endif

		//检测本地IP地址是否改变
		if( false == GetLocalNetworkInfo( &localNetworkInfo ) )
		{
			m_DdnsLock.UnLock();
			PUB_Sleep( 10000 );
			continue;
		}

		if( ( m_LocalNetInfo.LocalIP != localNetworkInfo.LocalIP ) || ( 0 == count ) || m_bLocalInfoChange)
		{
			if( DDNS_IPUPDATE_OK != m_pDdnsClient->DdnsIPupdate( m_DdnsClientInfo.user,m_DdnsClientInfo.password,m_DdnsClientInfo.domain ) )
			{
				if( ( 0 == count ) && ( 6 == ( ++notUpdateTime ) ) )
				{
					CNetInterface::Instance()->SetbDDNS( CANNOTUSEDDNS );
				}

				//外网IP改变，或者到更新时间，但只这次没更新成功
				m_DdnsLock.UnLock();
				if (0 == count)
				{
					PUB_Sleep(2000);
				}
				else
				{
					PUB_Sleep(1000);
				}

				continue;
			}
			else
			{
				notUpdateTime = 0;
				CNetInterface::Instance()->SetbDDNS( CANUSEDDNS );

				if( m_LocalNetInfo.LocalIP != localNetworkInfo.LocalIP )
				{
					memcpy( &m_LocalNetInfo, &localNetworkInfo, sizeof( LOCOALNETWORKINFO));
				}
				
				if (m_bLocalInfoChange)
				{
					m_bLocalInfoChange = false;
				}
			}
		}
	
		//检测外网IP地址是否改变
		m_DdnsLock.UnLock();
		if( -1 == m_pDdnsClient->GetExternalIp( externalIP ) )
		{	
			count++;
			count %= m_sleepCount;
			PUB_Sleep(10000);
			continue;
		}
		//m_DdnsLock.Lock();
		if( 0 != strcmp( externalIP, m_externalIP ) )
		{
			count = -1;
			memset(m_externalIP, 0, sizeof(m_externalIP));
			strcpy( m_externalIP, externalIP);
		}
		count++;
		count %= m_sleepCount;

		PUB_Sleep(10000);

	}
	
}




/*****************************************************************

Function:GetLocalAddr( unsigned int *localIP, char *pLocalMAC )

Description:获得本地MAC地址和IP地址

Calls:

Called by:

Table Accessed:

Table Updated:

Input:

Output:

Return: 返回0表示成功获得地址信息，获得的地址信息保存在pLocalNetworkInfo

Others:
******************************************************************/
bool CDdnsManager::GetLocalNetworkInfo( LOCOALNETWORKINFO *pLocalNetworkInfo )
{
	assert( NULL != pLocalNetworkInfo );

	int ifCount;
	int i;

	struct ifreq ifreqBuf[MAXINTERFACES];
	struct ifconf ifconfig;
	struct sockaddr_in *pSockaddr;

	int sockfd = socket( AF_INET, SOCK_STREAM, 0 );
	if( -1 == sockfd )
	{
		perror( "socket() " );
		return false;
	}

	ifconfig.ifc_len = sizeof( struct ifreq ) * MAXINTERFACES;
	ifconfig.ifc_buf = ( caddr_t )ifreqBuf;
	if( -1 == ioctl( sockfd, SIOCGIFCONF , ( char *)&ifconfig ) )
	{
		perror("ioctl()");
		close( sockfd );
		return false;
	}

	ifCount = ifconfig.ifc_len / sizeof( struct ifreq );

	for( i=0; i<ifCount; i++ )
	{
		char ifname[10];
		sprintf(ifname, "%s", ifreqBuf[i].ifr_name );
		if( strcmp( "eth0", ifname )  == 0 )
		{
			if( -1 == ioctl( sockfd, SIOCGIFFLAGS, ( char *)&ifreqBuf[i] ) )
			{
				perror("ioctl()");
				close( sockfd );
				return false;
			}

			//if( IFF_UP & ifreqBuf[i].ifr_flags )
			//{
			//	//printf("%s():%s:%d, the interface is up\n", __FUNCTION__, __FILE__, __LINE__);
			//}
			//else
			//{
			//	//printf("%s():%s:%D, the interface is down\n", __FUNCTION__, __FILE__, __LINE__);
			//}

			//获得设备IP
			if( -1 == ioctl( sockfd, SIOCGIFADDR, ( char * )&ifreqBuf[i] ) )
			{
				perror( "ioctl()" );
				close( sockfd );
				return false;
			}
			pSockaddr = NULL;
			pSockaddr = (struct sockaddr_in *)&ifreqBuf[i].ifr_addr;
			memcpy( &pLocalNetworkInfo->LocalIP, &pSockaddr->sin_addr.s_addr, sizeof( unsigned int ) );
			

			//获得设备MAC
			if( -1 == ioctl( sockfd, SIOCGIFHWADDR, (char *)&ifreqBuf[i] ) )
			{
				perror("ioctl()");
				close( sockfd );
				return false;
			}
			memset( pLocalNetworkInfo->MACAddr, 0, sizeof( pLocalNetworkInfo->MACAddr ) );
			sprintf( pLocalNetworkInfo->MACAddr, "%02x:%02x:%02x:%02x:%02x:%02x\n",\
				ifreqBuf[i].ifr_hwaddr.sa_data[0],\
				ifreqBuf[i].ifr_hwaddr.sa_data[1],\
				ifreqBuf[i].ifr_hwaddr.sa_data[2],\
				ifreqBuf[i].ifr_hwaddr.sa_data[3],\
				ifreqBuf[i].ifr_hwaddr.sa_data[4],\
				ifreqBuf[i].ifr_hwaddr.sa_data[5]\
				);			
			pLocalNetworkInfo->MACAddr[strlen( pLocalNetworkInfo->MACAddr)] = '\0';

			//获得IP地址的子网掩码
			if( -1 == ioctl( sockfd, SIOCGIFNETMASK, (char*)&ifreqBuf[i] ) )
			{
				perror("ioctl()");
				close( sockfd );
				return false;
			}
			pSockaddr = NULL;
			pSockaddr = (struct sockaddr_in *)&ifreqBuf[i].ifr_netmask;
			memcpy( &pLocalNetworkInfo->NetMask, &pSockaddr->sin_addr.s_addr, sizeof( unsigned int ) );

			//获得广播地址
			if( -1 == ioctl( sockfd, SIOCGIFBRDADDR, (char *)&ifreqBuf[i] ) )
			{
				perror("ioctl()");
				close( sockfd );
				return false;
			}
			pSockaddr = NULL;
			pSockaddr = (struct sockaddr_in *)&ifreqBuf[i].ifr_broadaddr;
			memcpy( &pLocalNetworkInfo->BroadAddr, &pSockaddr->sin_addr.s_addr, sizeof( unsigned int ) );

		}
	}
	close( sockfd );

	return true;
}







