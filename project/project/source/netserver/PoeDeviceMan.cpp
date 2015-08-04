#include "PoeDeviceMan.h"
#include "SWL_Public.h"
#include <algorithm>
#include <string>
#include "NetDeviceDefine.h"

#define POE_WAIT_TIME  1000
#define POE_SLEEP_TIME 5000 
#define POE_TRY_MAX_TIMES  5

using std::string;

CPoeDeviceMan::CPoeDeviceMan(void)
{
	m_pIpcOnvifMacPhyPort = NULL;
}


CPoeDeviceMan::~CPoeDeviceMan(void)
{ 
	if( m_pIpcOnvifMacPhyPort )
	{
		delete [] m_pIpcOnvifMacPhyPort;
		m_pIpcOnvifMacPhyPort = NULL;
	}
}


bool CPoeDeviceMan::Initial(int nPortNum)
{
	if (nPortNum <= 0)
	{
		return false;
	}

	m_pIpcOnvifMacPhyPort = new IPC_ONVIF_MAC_PHY_PORT[IPC_ONVIF_MAX_PORT_NUM];
	if( m_pIpcOnvifMacPhyPort )
	{
		memset( m_pIpcOnvifMacPhyPort, 0, sizeof(IPC_ONVIF_MAC_PHY_PORT)*IPC_ONVIF_MAX_PORT_NUM);
	}
 
	m_waitEvent.CreateWaitEvent();

	m_bConnectRunning = false;
	m_threadConnect = PUB_CREATE_THREAD_FAIL;

	m_bRecvRunning = false;
	m_threadRecv = PUB_CREATE_THREAD_FAIL;

	m_deviceCount = nPortNum;
	m_deviceInfoStore.resize(nPortNum);
	for(int i = 0; i < m_deviceCount; i++)
	{
		POE_DEVICE_INFO &devInfo = m_deviceInfoStore[i];
		memset(&devInfo,0,sizeof(POE_DEVICE_INFO));
	}


	if (m_deviceSearch.Init() < 0 || m_deviceSearch.StartSearch() < 0)
	{
		return false;
	}

	if (m_deviceIPSet.Init(DEFAULT_LOCAL_PORT,MULTICAST_PORT,MULTICAST_IP, POE_ETHER) < 0)
	{
		return false;
	}
	m_deviceIPSet.AddRecvGroup(MULTICAST_IPTOOL_RECV_IP, MULTICAST_PORT);

	return true;
}


void CPoeDeviceMan::Quit()
{
	m_waitEvent.Close();

	m_deviceSearch.EndSearch();
	m_deviceSearch.Exit();

	m_deviceIPSet.Quit();
	m_deviceInfoStore.clear();

	if( m_pIpcOnvifMacPhyPort )
	{
		delete [] m_pIpcOnvifMacPhyPort;
		m_pIpcOnvifMacPhyPort = NULL;
	}

}


bool CPoeDeviceMan::Start()
{
	m_threadConnect = PUB_CreateThread(ConnectThread,this,&m_bConnectRunning);
	m_threadRecv = PUB_CreateThread(RecvThread,this,&m_bRecvRunning);

	
	if ((PUB_CREATE_THREAD_FAIL == m_threadConnect) ||
		(PUB_CREATE_THREAD_FAIL == m_threadRecv))
	{
		Stop();
		return false;
	}

	return true;
}


bool CPoeDeviceMan::Stop()
{
	if (PUB_CREATE_THREAD_FAIL != m_threadConnect)
	{
		PUB_ExitThread(&m_threadConnect,&m_bConnectRunning);
	}

	if (PUB_CREATE_THREAD_FAIL != m_threadRecv)
	{
		PUB_ExitThread(&m_threadRecv,&m_bRecvRunning);
	}

	return true;
}

const char * CPoeDeviceMan::GetDevicePassword(const char *pSn)
{
	for(int i = 0; i < m_deviceCount; i++)
	{
		if(strcmp(m_deviceInfoStore[i].sn, pSn) == 0)
		{
			return m_deviceInfoStore[i].passwd;
		}
	}

	return NULL;
}


int CPoeDeviceMan::GetIPCMacChannel(const char *macAddr, int len, bool bOnvifDevice, bool bClearOnvifDeviceMac )
{
	int chnn = -1;

	if( bOnvifDevice )
	{
		if( !m_pIpcOnvifMacPhyPort )
		{
			return -1;
		}

		if( bClearOnvifDeviceMac )
		{
			for( int idx = 0; idx < IPC_ONVIF_MAX_PORT_NUM; idx++ )
			{
				if( m_pIpcOnvifMacPhyPort[idx].macChannel > 0 )
				{
					if( 0 == memcmp( macAddr, m_pIpcOnvifMacPhyPort[idx].MacAddr, BROADCAST_MAC_ADDR_LEN) )
					{
						memset( &m_pIpcOnvifMacPhyPort[idx], 0, sizeof(IPC_ONVIF_MAC_PHY_PORT) );
						break;
					}
				}
			}

			chnn = -1;
		}
		else
		{
			for( int idx = 0; idx < IPC_ONVIF_MAX_PORT_NUM; idx++ )
			{
				if( m_pIpcOnvifMacPhyPort[idx].macChannel > 0 )
				{
					if( 0 == memcmp( macAddr, m_pIpcOnvifMacPhyPort[idx].MacAddr, BROADCAST_MAC_ADDR_LEN) )
					{
						chnn = idx;
						break;
					}
				}
			}

			if( -1 == chnn )
			{
				m_sendMsgToPnp.Lock();
				m_deviceSearch.GetPortNumByMac( (unsigned char*)macAddr, len );
				m_sendMsgToPnp.UnLock();
			}
		}
	}
	else
	{
		m_linkStatusLock.Lock();
		for(int i = 0; i < m_deviceCount; i++)
		{
			if( 0 == memcmp(m_deviceInfoStore[i].MacAddr, macAddr, BROADCAST_MAC_ADDR_LEN))
			{
				chnn = i;
				break;
			}
		}
		m_linkStatusLock.UnLock();
	}

	return chnn;
}


RESULT WINAPI CPoeDeviceMan::ConnectThread(void * pParam)
{
	if (NULL == pParam)
	{
		return 0;
	}
	
	CPoeDeviceMan *pPoeDeviceMan = reinterpret_cast<CPoeDeviceMan *>(pParam);
	pPoeDeviceMan->ConnectProc();
	return 0 ;
}


void CPoeDeviceMan::ConnectProc()
{
#if defined(__CHIP3531__) || defined(__CHIP3535__)
	unsigned long recvCount = 32;
#else
	unsigned long recvCount = 10;
#endif
	while(m_bConnectRunning)
	{
		m_waitEvent.WaitForCond(POE_SLEEP_TIME);
		if (m_deviceSearch.GetMsgCount() > recvCount)
		{
			UpdateDeviceByResponseMsg();
		}

		UpdateDeviceByLinkStatusMsg();
	}
}


RESULT WINAPI CPoeDeviceMan::RecvThread(void *pParam)
{
	if (NULL == pParam)
	{
		return 0;
	}
	
	CPoeDeviceMan *pPoeDeviceMan = reinterpret_cast<CPoeDeviceMan *>(pParam);
	pPoeDeviceMan->RecvProc();
	return 0;
}


void CPoeDeviceMan::RecvProc()
{
	while(m_bRecvRunning)
	{
		BROADCAST_SEARCH_INFO searchInfo;
		if (m_deviceSearch.GetDevice(searchInfo) >= 0)
		{

			if (BROADCAST_CODE_RESPONSE_SEARCH == searchInfo.cmdCode)
			{
				m_searchInfoLock.Lock();
				m_searchInfoStore.push_back(searchInfo);
				m_searchInfoLock.UnLock();
			}
			else if( BROADCAST_CODE_RESPONSE_MAC_PORT == searchInfo.cmdCode )
			{
				if( searchInfo.macChannel >= 0 && searchInfo.macChannel < IPC_ONVIF_MAX_PORT_NUM )
				{
					if( m_pIpcOnvifMacPhyPort )
					{
						memcpy( m_pIpcOnvifMacPhyPort[searchInfo.macChannel].MacAddr, searchInfo.MacAddr, BROADCAST_MAC_ADDR_LEN );
						m_pIpcOnvifMacPhyPort[searchInfo.macChannel].macChannel = searchInfo.macChannel+1;
					}
				}
			}

			if( BROADCAST_CODE_RESPONSE_MAC_PORT != searchInfo.cmdCode )
			{
				/***实时更新链路状态***/
				m_linkStatusLock.Lock();
				m_linkStatus = searchInfo.PhyLinkStatus;
				m_linkStatusLock.UnLock();

				m_waitEvent.SetEvent();
			}
		}
		else
		{
			/***在不支持POE的设备上面需要Sleep否则CPU使用率很高***/
			PUB_Sleep(50);
		}
	}
}


bool CPoeDeviceMan::SetDeviceDHCP(POE_DEVICE_INFO &deviceInfo)
{
	MultiCastInfo_v13 multicastInfo;

	/***设置IP地址***/

	memset(&multicastInfo,0,sizeof(MultiCastInfo));

	multicastInfo.mulHead.head = FLAG_MULTIHEAD;
	multicastInfo.mulHead.ver  = MULTICAST_10005_VERSION;
	multicastInfo.mulHead.cmd  = MULTICAST_CMD_SET;
	memcpy(multicastInfo.MacAddr,deviceInfo.MacAddr,BROADCAST_MAC_ADDR_LEN);
	memcpy(multicastInfo.szPasswd,deviceInfo.passwd,BROADCAST_PASSWD_LEN);
	multicastInfo.ucIPMode = 1;												//修改成DHCP模式

	return (m_deviceIPSet.SendTo(&multicastInfo,sizeof(MultiCastInfo)) == 0);
}

void CPoeDeviceMan::GetDevice(BROADCAST_SEARCH_INFO_STORE &searchInfoStore)
{
	searchInfoStore.clear();

	m_sendMsgToPnp.Lock();
	m_deviceSearch.SearchDevice();
	m_sendMsgToPnp.UnLock();

	m_searchInfoLock.Lock();
	if (m_searchInfoStore.empty())
	{
		m_searchInfoLock.UnLock();
		return;
	}

	copy(m_searchInfoStore.begin(),m_searchInfoStore.end(),back_inserter(searchInfoStore));

	m_searchInfoStore.clear();
	m_searchInfoLock.UnLock();
}

void CPoeDeviceMan::UpdateDeviceByResponseMsg()
{
	BROADCAST_SEARCH_INFO_STORE tempSearchInfoStore;
	bool bMustSetDHCPFlag[128];
	int i;

	GetDevice(tempSearchInfoStore);

	if (tempSearchInfoStore.empty())
	{
		return;
	}

	BROADCAST_SEARCH_INFO_STORE::iterator iter = tempSearchInfoStore.begin(),
		end = tempSearchInfoStore.end();

	for( i = 0; i < 128; i++ )
	{
		bMustSetDHCPFlag[i] = false;
	}

	m_linkStatusLock.Lock();
	for (; iter!=end; iter++)
	{
		BROADCAST_SEARCH_INFO &deviceInfo = (*iter);
		POE_DEVICE_INFO &poeDeviceInfo = m_deviceInfoStore[deviceInfo.macChannel];

		if(memcmp(poeDeviceInfo.MacAddr, deviceInfo.MacAddr, sizeof(poeDeviceInfo.MacAddr)) != 0)
		{
			memcpy(poeDeviceInfo.MacAddr, deviceInfo.MacAddr, sizeof(poeDeviceInfo.MacAddr));
			memcpy(poeDeviceInfo.passwd,deviceInfo.szPasswd,BROADCAST_PASSWD_LEN);
			sprintf(poeDeviceInfo.sn, "%02x%02x%02x%02x%02x%02x", deviceInfo.MacAddr[0], deviceInfo.MacAddr[1], deviceInfo.MacAddr[2], \
				deviceInfo.MacAddr[3], deviceInfo.MacAddr[4], deviceInfo.MacAddr[5]);

			bMustSetDHCPFlag[ deviceInfo.macChannel ] = true;
		}
	}
	m_linkStatusLock.UnLock();

	for (i = 0; i< m_deviceCount; i++)
	{
		if( bMustSetDHCPFlag[i] )
		{
			POE_DEVICE_INFO &poeDeviceInfo = m_deviceInfoStore[i];
			SetDeviceDHCP(poeDeviceInfo);
		}
	}
}


void CPoeDeviceMan::UpdateDeviceByLinkStatusMsg()
{
	m_linkStatusLock.Lock();

	for (int i = 0; i< m_deviceCount; i++)
	{
		if (0 == (m_linkStatus & (0x1<<i)))
		{
			POE_DEVICE_INFO &poeInfo = m_deviceInfoStore[i];
			memset(&poeInfo, 0, sizeof(POE_DEVICE_INFO));

			if( m_pIpcOnvifMacPhyPort )
			{
				memset( &m_pIpcOnvifMacPhyPort[i], 0, sizeof(IPC_ONVIF_MAC_PHY_PORT) );
			}
		}
	}

	m_linkStatusLock.UnLock();
}
