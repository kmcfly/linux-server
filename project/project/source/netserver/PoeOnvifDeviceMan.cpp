#include "NetInterface.h"
#include "PoeOnvifDeviceMan.h"
#include "NdLibMan.h"
#include "SWL_Public.h"

#define IPC_OFFLINE_SEARCH_TIMES 2

CPoeOnvifDeviceMan::CPoeOnvifDeviceMan(void)
{
	m_bDiscovering			= false;
	m_threadDiscover		= PUB_THREAD_ID_NOINIT;

	m_fOnlineCBFun			= NULL;
	m_pOnlineCBFunParam		= NULL;
	m_fOfflineCBFun			= NULL;
	m_pOfflineCBFunParam	= NULL;

	m_fGetIPCPhyChlByMac    = NULL;
	m_pGetIpcChlFunParam    = NULL;

	m_pDeviceInfo			= NULL;
	m_deviceCount			= 0;

	m_switchIP				= 0;

	m_pTempDeviceInfo = NULL;

	m_bIpcPhyIsRealChn = true;
	m_bDiscoverRunning = true;

}

CPoeOnvifDeviceMan::~CPoeOnvifDeviceMan(void)
{

}

bool CPoeOnvifDeviceMan::Initial(int nPortNum, POE_CHANNEL_SN *pChannelSn)
{
	assert(nPortNum > 0);
	if(nPortNum <= 0)
	{
		return false;
	}

	m_deviceCount = nPortNum;
	m_pDeviceInfo = new POE_ONVIF_DEVICE_INFO_EX[nPortNum];
	memset(m_pDeviceInfo, 0, sizeof(POE_ONVIF_DEVICE_INFO) * nPortNum);

	for(int i = 0; i < m_deviceCount; i++)
	{
		m_pDeviceInfo[i].device.channel = pChannelSn[i].channel;
		strcpy(m_pDeviceInfo[i].device.sn, pChannelSn[i].sn);
		m_pDeviceInfo[i].poe_ipc_status = pChannelSn[i].poe_ipc_status;
		m_pDeviceInfo[i].state = POE_ONVIF_OFFLINE;
	}

#ifndef __ENVIRONMENT_WIN32__
	if (m_deviceSearch.Init(DEFAULT_LOCAL_PORT,MULTICAST_PORT,MULTICAST_IP, POE_ETHER) < 0)
	{
		return false;
	}
	m_deviceSearch.AddRecvGroup(MULTICAST_IPTOOL_RECV_IP, MULTICAST_PORT);
#endif

	return true;
}

void CPoeOnvifDeviceMan::Quit()
{
	if(NULL != m_pDeviceInfo)
	{
		delete [] m_pDeviceInfo;
		m_pDeviceInfo = NULL;
	}
#ifndef __ENVIRONMENT_WIN32__
	m_deviceSearch.Quit();
#endif


}

bool CPoeOnvifDeviceMan::Start()
{
	if(m_bDiscovering)
	{
		return true;
	}

	m_threadDiscover = PUB_CreateThread(DiscoverDeviceThread, this, &m_bDiscovering);
	if(PUB_CREATE_THREAD_FAIL == m_threadDiscover)
	{
		return false;
	}

#ifndef __ENVIRONMENT_WIN32__
// 	m_threadSearchTvtIpc = PUB_CreateThread(SearchTvtIpcThread, this, &m_bSearchTvtIpc);
// 	if(PUB_CREATE_THREAD_FAIL == m_threadSearchTvtIpc)
// 	{
// 		return false;
// 	}
#endif

	return true;
}


bool CPoeOnvifDeviceMan::Stop()
{
	if(PUB_THREAD_ID_NOINIT != m_threadDiscover)
	{
		PUB_ExitThread(&m_threadDiscover, &m_bDiscovering);
	}

#ifndef __ENVIRONMENT_WIN32__
// 	if(PUB_THREAD_ID_NOINIT != m_threadSearchTvtIpc)
// 	{
// 		PUB_ExitThread(&m_threadSearchTvtIpc, &m_bSearchTvtIpc);
// 	}
#endif

	return true;
}

void CPoeOnvifDeviceMan::ChangeSwitchIP(unsigned int newIP, unsigned int netmask)
{
	if(m_switchIP == newIP)
	{
		return;
	}

#ifndef __ENVIRONMENT_WIN32__
	if(m_switchIP != 0)
	{
		DVRSystem("kill -9 `cat /var/run/udhcpd.pid`");
	}
#endif

	m_switchIP = newIP;
	m_switchNetmask = netmask;

	m_minIP = (m_switchIP & m_switchNetmask) + htonl(100);	//x.x.x.100
	m_maxIP = m_minIP + htonl(154);						//x.x.x.254

	CreateDHCPAddrTable();

#ifndef __ENVIRONMENT_WIN32__
	DVRSystem("/mnt/mtd/udhcpd &");
#endif
}

void CPoeOnvifDeviceMan::SetOnlineCallback(ONVIF_ONLINE_CB_FUN fOnlineCBFun, void *pUser)
{
	m_fOnlineCBFun = fOnlineCBFun;
	m_pOnlineCBFunParam = pUser;
}

void CPoeOnvifDeviceMan::SetGetIpcPhyChnByMac(GET_IPC_PHY_CHN_BY_MAC fOnlineCBFun, void *pUser)
{
	m_fGetIPCPhyChlByMac = fOnlineCBFun;
	m_pGetIpcChlFunParam = pUser;
}


void CPoeOnvifDeviceMan::SetOfflineCallback(ONVIF_OFFLINE_CB_FUN fOfflineCBFun, void *pUser)
{
	m_fOfflineCBFun = fOfflineCBFun;
	m_pOfflineCBFunParam = pUser;
}


//pChannelSn的序号是按照图像显示的位置序号顺序的
//图像的位置序号是按照数组索引， 
//pChannelSn[i].poe_ipc_status 意识有三种：一。0表示POE有连接eth1设备， 二。-1表示POE通道没有连接IPC，三。-2表示POE通道上有eth0的IPC设备

//在IPC设备处理界面的时候需要关闭POE线程处理，这样避免复杂的逻辑处理

void CPoeOnvifDeviceMan::ChangeChannelSn(POE_CHANNEL_SN *pChannelSn, bool bUsedChangPoeChnn)
{
	if( !m_pTempDeviceInfo )
	{
		return;
	}

	m_deviceLock.Lock();

	memcpy(m_pTempDeviceInfo, m_pDeviceInfo, sizeof(POE_ONVIF_DEVICE_INFO_EX) * m_deviceCount);

	printf("CPoeOnvifDeviceMan::ChangeChannelSn : %s,%d \n", __FILE__, __LINE__ );
	bool bFindSn = false;
	bool bHasEth0Ipc = false;
	bool bHasRechangPoechannel = false;

	for(int i = 0; i < m_deviceCount; i++)
	{
		//获取相同sn的IPC信息
		bFindSn = false;
		if( strlen( pChannelSn[i].sn ) > 0 )
		{
			for( int j = 0; j < m_deviceCount; j++ )
			{
				if( 0 == strcmp( pChannelSn[i].sn, m_pTempDeviceInfo[j].device.sn) )
				{
					bFindSn = true;
					memcpy( &m_pDeviceInfo[i], &m_pTempDeviceInfo[j], sizeof(POE_ONVIF_DEVICE_INFO_EX) );

					if( i != j )
					{
						bHasRechangPoechannel = true;
					}

					//当期通道号更改
					m_pDeviceInfo[i].device.channel = pChannelSn[i].channel;
					m_pDeviceInfo[i].poe_ipc_status = pChannelSn[i].poe_ipc_status;

					break;
				}
			}
		}

		if( !bFindSn )
		{
			// 如果没有找到则认为没有IPC信息，
			memset( &m_pDeviceInfo[i], 0, sizeof(POE_ONVIF_DEVICE_INFO_EX) );

			//如果是连接eth0上的设备
			m_pDeviceInfo[i].device.channel = pChannelSn[i].channel;
			m_pDeviceInfo[i].poe_ipc_status = pChannelSn[i].poe_ipc_status;
			strcpy(m_pDeviceInfo[i].device.sn, pChannelSn[i].sn);

			//如果POE上接了eth0的IPC设备，则状态为在线状态
			if( -2 == pChannelSn[i].poe_ipc_status )
			{
				m_pDeviceInfo[i].state = POE_ONVIF_ETH0_DEVICE;

				//如果存在eth0设备的IPC在POE通道上，则认为不需要一对一连接

				bHasEth0Ipc = true;
			}
			else
			{
				m_pDeviceInfo[i].state = POE_ONVIF_OFFLINE;
			}

			printf("sn[%d,%d] = %s, no same channel = %d--mode %d\n", i, pChannelSn[i].channel,
				m_pDeviceInfo[i].device.sn, m_pDeviceInfo[i].device.channel, (int)m_bIpcPhyIsRealChn);
		}
		else
		{
			printf("sn[%d] = %s, channel = %d -- mode %d\n", 
				i, m_pDeviceInfo[i].device.sn, m_pDeviceInfo[i].device.channel, (int)m_bIpcPhyIsRealChn );
			if( -2 == pChannelSn[i].poe_ipc_status )
			{
				bHasEth0Ipc = true;
			}
		}
	}

	if( bHasEth0Ipc )
	{
		printf( "poe mode ischange to flash %s,%d\n", __FILE__, __LINE__ );
		m_bIpcPhyIsRealChn = false;
	}
	else
	{
		//因为一设置，应该检测POE是否一对一状态，所以这里是true
		bUsedChangPoeChnn = true;
		if( bUsedChangPoeChnn )
		{
			//if( m_bIpcPhyIsRealChn )
			//{
			//	printf( "poe mode ischange to flash %s,%d\n", __FILE__, __LINE__ );
			//	m_bIpcPhyIsRealChn = false;
			//}
			//else
			{
				//检测当前的POE通道和IPC通道是否对应
				int ipc_ix = -1;
				bool bIPCtoNoPoeChn = false;
				for(int ix = 0; ix < m_deviceCount; ix++)
				{
					if( 0 == m_pDeviceInfo[ix].poe_ipc_status )
					{
						if( MID_TVT == m_pDeviceInfo[ix].device.manufacturerId )
						{
							ipc_ix = GetChannlByMacaddr( (char*)m_pDeviceInfo[ix].IpcmacAddr, IPC_MAC_ADDR_LENS, false, false );
						}
						else
						{
							ipc_ix = GetChannlByMacaddr( (char*)m_pDeviceInfo[ix].IpcmacAddr, IPC_MAC_ADDR_LENS, true, false );
						}
						printf( "cuurent channel %d---poe chan %d\n", ix, ipc_ix );
						if( -1 != ipc_ix)
						{
							if( ipc_ix != ix )
							{
								bIPCtoNoPoeChn = true;
								break;
							}
						}
					}
				}

				// 如果是一对一关系，怎更改POE对应模式
				if( !bIPCtoNoPoeChn )
				{
					m_bIpcPhyIsRealChn = true;
				}
				else
				{
					m_bIpcPhyIsRealChn = false;
				}

				printf( "poe mode ischange to %d %s,%d\n", (int)m_bIpcPhyIsRealChn, __FILE__, __LINE__ );
			}
		}
		else
		{
			printf( "poe mode is used prev value to %d %s,%d\n", (int)m_bIpcPhyIsRealChn, __FILE__, __LINE__ );
		}

	}

	m_deviceLock.UnLock();
}

void CPoeOnvifDeviceMan::EnablePoeDiscoverThreadRun( bool bRun )
{
	//printf( "enter EnablePoeDiscoverThreadRun %s,%d\n", __FILE__, __LINE__ );
	m_deviceLock.Lock();
	m_bDiscoverRunning = bRun;
	m_deviceLock.UnLock();
	//printf( "leave EnablePoeDiscoverThreadRun %s,%d\n", __FILE__, __LINE__ );
}

RESULT WINAPI CPoeOnvifDeviceMan::DiscoverDeviceThread(void * pParam)
{
#ifdef __ENVIRONMENT_LINUX__
	int pid = GetThisThreadId();
	printf("%s, %d, pid = %d\n", __FILE__, __LINE__, pid);
#endif

	CPoeOnvifDeviceMan *pThis = (CPoeOnvifDeviceMan *)pParam;
	pThis->DiscoverDeviceProc();

	return 0;
}

void CPoeOnvifDeviceMan::DiscoverDeviceProc()
{
	int deviceMaxNum = 500;
	ND_DEVICE_INFO *pDeviceInfo = new ND_DEVICE_INFO[deviceMaxNum];
	m_pTempDeviceInfo = new POE_ONVIF_DEVICE_INFO_EX[m_deviceCount];

	list<DEVICE_SN_INFO> deviceSnInfoList;

	POE_IPC_EXT_CTRL_PARAM stOnvifOnlineCtrlParam;
	POE_IPC_EXT_CTRL_PARAM stOnvifOfflineCtrlParam;

	while(m_bDiscovering)
	{
		if(deviceSnInfoList.size() > (2 * m_deviceCount))
		{
			unsigned long earlistTime = 0xffffffff;
			list<DEVICE_SN_INFO>::iterator itTemp = deviceSnInfoList.begin();
			list<DEVICE_SN_INFO>::iterator it = deviceSnInfoList.begin();
			for(; it != deviceSnInfoList.end(); it++)
			{
				if((*it).prevOnlineTick < earlistTime)
				{
					earlistTime = (*it).prevOnlineTick;
					itTemp = it;
				}
			}

			deviceSnInfoList.erase(itTemp);
		}

		if( !m_bDiscoverRunning )
		{
			PUB_Sleep(1000);
			continue;
		}

		int deviceNum = CNdLibMan::Instance()->GetDiscoveredDeviceNum();
		if(deviceNum > deviceMaxNum)
		{
			deviceMaxNum = deviceNum + 100;
			delete [] pDeviceInfo;
			pDeviceInfo = new ND_DEVICE_INFO[deviceMaxNum];
		}

		deviceNum = CNdLibMan::Instance()->GetDiscoveredDeviceInfo(pDeviceInfo, deviceMaxNum);

#ifndef __ENVIRONMENT_WIN32__
		SearchTvtIpc(m_listTvtIpc);
#endif

		m_deviceLock.Lock();
		if( !m_bDiscoverRunning )
		{
			m_deviceLock.UnLock();
			PUB_Sleep(1000);
			continue;
		}

		memcpy(m_pTempDeviceInfo, m_pDeviceInfo, sizeof(POE_ONVIF_DEVICE_INFO_EX) * m_deviceCount);
		for(int i = 0; i < m_deviceCount; i++)
		{
			m_pTempDeviceInfo[i].state = POE_ONVIF_OFFLINE;
		}

		//把私有协议的IPCAM加入到设备数组
		for(list<MultiCastInfo_v14>::iterator infoIt = m_listTvtIpc.begin(); infoIt != m_listTvtIpc.end(); infoIt++)
		{
			MultiCastInfo_v14 &ipcInfo = *infoIt;
			char strMac[16];
			sprintf(strMac, "%02x%02x%02x%02x%02x%02x", ipcInfo.MacAddr[0], ipcInfo.MacAddr[1], ipcInfo.MacAddr[2], \
				ipcInfo.MacAddr[3], ipcInfo.MacAddr[4], ipcInfo.MacAddr[5]);

			//更新deviceSnInfoList
			list<DEVICE_SN_INFO>::iterator it = deviceSnInfoList.begin();
			for(; it != deviceSnInfoList.end(); it++)
			{
				if(strncmp((*it).sn, strMac, 12) == 0)
				{
					(*it).prevOnlineTick = GetTickCount();
					break;
				}
			}
			if(it == deviceSnInfoList.end())
			{
				DEVICE_SN_INFO deviceSnInfo;
				strcpy(deviceSnInfo.sn, strMac);
				deviceSnInfo.prevOnlineTick = GetTickCount();
				deviceSnInfoList.push_back(deviceSnInfo);
			}

			//根据sn找到当前的序号。
			int index = -1;
			if( m_bIpcPhyIsRealChn )
			{
				//获取通道号
				index = GetChannlByMacaddr( (char*)ipcInfo.MacAddr, IPC_MAC_ADDR_LENS,  false, false );
			}
			else
			{
				index = GetDeviceIndex(strMac, 12);
			}

			if(index != -1)
			{
				m_pTempDeviceInfo[index].state = POE_ONVIF_ONLINE;
				m_pTempDeviceInfo[index].device.channel = index;
				m_pTempDeviceInfo[index].device.dwIP = ipcInfo.ipaddr;
				m_pTempDeviceInfo[index].device.dataPort = ipcInfo.netport;
				m_pTempDeviceInfo[index].device.manufacturerId = MID_TVT;

				strcpy(m_pTempDeviceInfo[index].device.manufacturerName, ipcInfo.szManufacturery);

				strcpy(m_pTempDeviceInfo[index].device.sn, strMac);
				strcpy(m_pTempDeviceInfo[index].device.productModel, ipcInfo.szProductType);

				memcpy( (char*)m_pTempDeviceInfo[index].IpcmacAddr, (char*)ipcInfo.MacAddr, IPC_MAC_ADDR_LENS );
			}
		}

		//处理ONVIF搜索到的设备
		int prod_id_val = MANU_ONVIF_NORMAL;
		bool bIPCIsNotTvt = false;
		for(int m = 0; m < deviceNum; m++)
		{
			if(MID_ONVIF == pDeviceInfo[m].manufacturerId)
			{
				unsigned int ipAddr = inet_addr(pDeviceInfo[m].szAddr);
				char MacAddrValue[IPC_MAC_ADDR_LENS] = {0};
				bIPCIsNotTvt = false;
				if(strcmp(pDeviceInfo[m].szEther, POE_ETHER) == 0)
				{
					//符合eth1标志
					bIPCIsNotTvt = true;
				}

				//确认不是TVT的IPC
				if( bIPCIsNotTvt )
				{
					// 在szID中如果最后有SZ****SZ格式的数据，表示产品的厂家ID标志，需要比较的时候去掉8个字符
					//printf( " pDeviceInfo[m].szID = %s,%s\n", pDeviceInfo[m].szID, pDeviceInfo[m].szAddr );
					int ext_ln = strlen(pDeviceInfo[m].szID);
					if( ext_ln >= 8 )
					{
						if( 'Z' == pDeviceInfo[m].szID[ext_ln-1] && 'S' == pDeviceInfo[m].szID[ext_ln-2] 
						&& 'Z' == pDeviceInfo[m].szID[ext_ln-7] && 'S' == pDeviceInfo[m].szID[ext_ln-8] )
						{
							ext_ln -= 8;
						}
					}

					if(CheckIPInDelayUse(ipAddr))
					{
						continue;
					}

					bool bGetIPmapMac = false;
					int index = -1;

					bGetIPmapMac = GetMacByOldOnvifList( ipAddr, (unsigned char*)MacAddrValue, IPC_MAC_ADDR_LENS );
					if( bGetIPmapMac )
					{
						index = GetChannlByMacaddr( MacAddrValue, IPC_MAC_ADDR_LENS, true, false );
					}

					if( -1 == index )
					{
						bGetIPmapMac = GetMacaddrByIPAddr((unsigned char*)pDeviceInfo[m].szAddr, 64, (unsigned char*)MacAddrValue, IPC_MAC_ADDR_LENS );
						if( bGetIPmapMac )
						{
							index = GetChannlByMacaddr( MacAddrValue, IPC_MAC_ADDR_LENS, true, false );
						}
					}

					//获取不到对应的通道号，则退出
					if( -1 == index )
					{
						continue;
					}

					//把搜索到的同为的IPCAM从ONVIF列表中去掉
					char strMac[16] = {0};
					sprintf(strMac, "%02x%02x%02x%02x%02x%02x", MacAddrValue[0], MacAddrValue[1], MacAddrValue[2], \
						MacAddrValue[3], MacAddrValue[4], MacAddrValue[5]);
					//printf( "onvif strmac ==== %s,%s,%d\n", strMac, __FILE__, __LINE__ );
					if((0 == strncmp( strMac, "0018ae", 6 )) || (0 == strncmp( strMac, "a81b189a", 8)))
					{
						continue;
					}
					else
					{
						int del_dwip_same = 0;
						for( int tdel = 0; tdel < m_deviceCount; tdel++ )
						{
							if( m_pTempDeviceInfo[tdel].device.dwIP == ipAddr 
								&& MID_TVT == m_pTempDeviceInfo[tdel].device.manufacturerId 
								&& POE_ONVIF_ONLINE == m_pTempDeviceInfo[tdel].state )
							{
								del_dwip_same = 1;
								break;
							}
						}

						if( del_dwip_same )
						{
							continue;
						}
					}


					if( m_bIpcPhyIsRealChn )
					{
						index = GetChannlByMacaddr( MacAddrValue, IPC_MAC_ADDR_LENS, true, false );
					}
					else
					{
						index = GetDeviceIndex(pDeviceInfo[m].szID, ext_ln );
					}

					//printf( "get device index == %d,%d\n", index, ipAddr );
					if(index != -1)
					{
						m_pTempDeviceInfo[index].state = POE_ONVIF_ONLINE;
						m_pTempDeviceInfo[index].device.channel = index;
						m_pTempDeviceInfo[index].device.dwIP = ipAddr;
						m_pTempDeviceInfo[index].device.dataPort = pDeviceInfo[m].netport;
						m_pTempDeviceInfo[index].device.manufacturerId = pDeviceInfo[m].manufacturerId;

						strcpy(m_pTempDeviceInfo[index].device.manufacturerName, pDeviceInfo[m].manufacturerName);

						strcpy(m_pTempDeviceInfo[index].device.sn, pDeviceInfo[m].szID);
						strcpy(m_pTempDeviceInfo[index].device.productModel, pDeviceInfo[m].productModel);

						memcpy( (char*)m_pTempDeviceInfo[index].IpcmacAddr, (char*)MacAddrValue, IPC_MAC_ADDR_LENS );
					}
				}
			}
		}

		//检查协议下线的设备
		for(int i = 0; i < m_deviceCount; i++)
		{
			{
				if((m_pDeviceInfo[i].state == POE_ONVIF_ONLINE) && (m_pTempDeviceInfo[i].state == POE_ONVIF_OFFLINE))
				{
					//确定搜索IPC_OFFLINE_SEARCH_TIMES次以上没有搜索到则认为是掉线
					if( m_pDeviceInfo[i].search_times > 0 )
					{
						m_pDeviceInfo[i].search_times--;
					}

					if( 0 == m_pDeviceInfo[i].search_times )
					{
						int ckidx = -1;
						if( MID_TVT == m_pDeviceInfo[i].device.manufacturerId )
						{
							//私有协议再次确认mac地址是否存在
							ckidx = GetChannlByMacaddr( (char*)m_pDeviceInfo[i].IpcmacAddr, IPC_MAC_ADDR_LENS, false, false );
						}
						else
						{
							//onvif的话，删除设备mac对应的通道号
							ckidx = GetChannlByMacaddr( (char*)m_pDeviceInfo[i].IpcmacAddr, IPC_MAC_ADDR_LENS, true, true );
						}

						if( -1 != ckidx )
						{
							printf( "current ipc is live one %s,%s,%d\n", m_pDeviceInfo[i].device.sn, __FILE__, __LINE__ );
							m_pDeviceInfo[i].search_times = 1;
						}
					}

					if( 0 == m_pDeviceInfo[i].search_times )
					{
						m_pDeviceInfo[i].state = POE_ONVIF_OFFLINE;

						DELAY_USE_IP delayUse;
						delayUse.ip = m_pDeviceInfo[i].device.dwIP;
						delayUse.offlineTime = GetTickCount();
						m_delayUseList.push_back(delayUse);

						if(NULL != m_fOfflineCBFun)
						{
							memset( &stOnvifOfflineCtrlParam, 0, sizeof(POE_IPC_EXT_CTRL_PARAM) );
							stOnvifOfflineCtrlParam.op_type = 0;
							stOnvifOfflineCtrlParam.bPoeChnPtoP = m_bIpcPhyIsRealChn;
							stOnvifOfflineCtrlParam.bEth0Ipc = false;

							m_fOfflineCBFun(m_pDeviceInfo[i].device, stOnvifOfflineCtrlParam, m_pOfflineCBFunParam);

							printf( "delete ipc == %d\n", m_pDeviceInfo[i].device.channel );

							//表示当前POE通道没有连接IPC设备
							m_pDeviceInfo[i].poe_ipc_status = -1;
						}
					}
				}
				else if((m_pDeviceInfo[i].state == POE_ONVIF_ONLINE) && (m_pTempDeviceInfo[i].state == POE_ONVIF_ONLINE ))
				{
					//确定搜索IPC_OFFLINE_SEARCH_TIMES次以上没有搜索到则认为是掉线
					if( m_pDeviceInfo[i].search_times < IPC_OFFLINE_SEARCH_TIMES )
					{
						m_pDeviceInfo[i].search_times++;
					}
				}
			}
		}

		//检查上线、掉线、更改设备
		bool bPrintDebugInfo = false;
		bool bAddorChangeIpc = false;
		for(int i = 0; i < m_deviceCount; i++)
		{
			//新上线的设备
			bAddorChangeIpc = false;
			if((POE_ONVIF_ONLINE == m_pTempDeviceInfo[i].state) && (POE_ONVIF_OFFLINE == m_pDeviceInfo[i].state))
			{
				bAddorChangeIpc = true;
			}
			else if( (POE_ONVIF_ONLINE == m_pTempDeviceInfo[i].state) && (POE_ONVIF_ONLINE == m_pDeviceInfo[i].state ) )
			{
				if( (m_pTempDeviceInfo[i].device.manufacturerId != m_pDeviceInfo[i].device.manufacturerId) 
					|| (m_pTempDeviceInfo[i].device.dwIP != m_pDeviceInfo[i].device.dwIP) 
					|| (strcmp(m_pTempDeviceInfo[i].device.sn, m_pDeviceInfo[i].device.sn) != 0) )
				{
					bAddorChangeIpc = true;
				}
			}

			if( bAddorChangeIpc )
			{
				memcpy(&m_pDeviceInfo[i], &m_pTempDeviceInfo[i], sizeof(POE_ONVIF_DEVICE_INFO_EX));
				if(NULL != m_fOnlineCBFun)
				{
					memset( &stOnvifOnlineCtrlParam, 0, sizeof(POE_IPC_EXT_CTRL_PARAM) );
					stOnvifOnlineCtrlParam.delChnMsk = GetSameSnOrIPItem( 
						m_pDeviceInfo[i].device.sn, strlen(m_pDeviceInfo[i].device.sn),
						m_pDeviceInfo[i].device.dwIP, m_pDeviceInfo[i].device.channel );
					printf( "addd ipc == %d\n", m_pDeviceInfo[i].device.channel );
					m_fOnlineCBFun(m_pDeviceInfo[i].device, stOnvifOnlineCtrlParam, m_pOnlineCBFunParam);
					bPrintDebugInfo = true;
				}

				//确定搜索IPC_OFFLINE_SEARCH_TIMES次以上没有搜索到则认为是掉线
				m_pDeviceInfo[i].search_times = IPC_OFFLINE_SEARCH_TIMES;
				m_pDeviceInfo[i].poe_ipc_status = 0;
			}
		}


		if(bPrintDebugInfo)
		{
			printf("CPoeOnvifDeviceMan::ChangeChannelSn : %s,%d \n", __FILE__, __LINE__ );
			for(int i = 0; i < m_deviceCount; i++)
			{
				printf("sn[%d] = %s, channel = %d\n", i, m_pDeviceInfo[i].device.sn, m_pDeviceInfo[i].device.channel);
			}
		}

		m_deviceLock.UnLock();

		PUB_Sleep(1000);
	}

	delete [] pDeviceInfo;
	delete [] m_pTempDeviceInfo;

	m_pTempDeviceInfo = NULL;
}

#ifndef __ENVIRONMENT_WIN32__
RESULT WINAPI CPoeOnvifDeviceMan::SearchTvtIpcThread(void *pParam)
{
	CPoeOnvifDeviceMan *pThis = (CPoeOnvifDeviceMan *)pParam;
	pThis->SearchTvtIpcProc();

	return 0;
}

void CPoeOnvifDeviceMan::SearchTvtIpcProc()
{
	list<MultiCastInfo_v14> tempIpcList;

	while(m_bSearchTvtIpc)
	{
		SearchTvtIpc(tempIpcList);

 		m_listIpcLock.Lock();
 		m_listTvtIpc.clear();
 		if(!tempIpcList.empty())
 		{
 			copy(tempIpcList.begin(), tempIpcList.end(), back_inserter(m_listTvtIpc));
 		}
 		m_listIpcLock.UnLock();

		PUB_Sleep(500);
	}
}

void CPoeOnvifDeviceMan::SearchTvtIpc(list<MultiCastInfo_v14> &tvtIpcList)
{
	tvtIpcList.clear();

	MultiCastInfo_v14 tvtIpcInfo;
	memset(&tvtIpcInfo, 0, sizeof(MultiCastInfo_v14));
	tvtIpcInfo.mulHead.head = FLAG_MULTIHEAD;
	tvtIpcInfo.mulHead.ver  = MULTICAST_10007_VERSION;
	tvtIpcInfo.mulHead.cmd = MULTICAST_CMD_SEARCH;

	if(0 != m_deviceSearch.SendTo(&tvtIpcInfo,sizeof(MultiCastInfo)))
	{
		printf("......................SearchTVTIPC failed....\n");
		return;
	}

	memset(&tvtIpcInfo, 0, sizeof(MultiCastInfo_v14));
	int ret = 0;
	int tryTimes = 20;

	while(tryTimes > 0)
	{
		ret = m_deviceSearch.RecvFrom(&tvtIpcInfo, sizeof(MultiCastInfo_v14));

		//PUB_Sleep(1000);

		if ((ret > 0) && (ret >= sizeof(MultiCastInfo_v14)))
		{
			if (tvtIpcInfo.mulHead.cmd == MULTICAST_CMD_INFO)
			{
				list<MultiCastInfo_v14>::iterator it = tvtIpcList.begin();
				for(; it != tvtIpcList.end(); it++)
				{
					if(tvtIpcInfo.ipaddr == (*it).ipaddr)
					{
						memcpy(&(*it), &tvtIpcInfo, sizeof(MultiCastInfo_v14));
						break;
					}
				}

				if(it == tvtIpcList.end())
				{
					tvtIpcList.push_back(tvtIpcInfo);
				}
			}
		}
		else
		{
			tryTimes--;
			PUB_Sleep(100);
		}
	}
}
#endif


//这个处理主要是和NetDeviceManager的一致
int CPoeOnvifDeviceMan::GetSameSnOrIPItem( char* sn, int sn_len, int dwip, int channel )
{
	int index = -1;
	int n = 0;
	int chl_msk = 0;

	for( n = 0; n < m_deviceCount; n++)
	{
		if( m_pDeviceInfo[n].device.channel != channel )
		{
			//如果有相同的sn或者dwIP这关闭
			if( (strncmp(sn, m_pDeviceInfo[n].device.sn, sn_len ) == 0)
				|| (m_pDeviceInfo[n].device.dwIP == dwip) )
			{
				m_pDeviceInfo[n].device.sn[0] = '\0';
				m_pDeviceInfo[n].device.dwIP = 0;
				m_pDeviceInfo[n].search_times = 0;
				m_pDeviceInfo[n].poe_ipc_status = -1;
				m_pDeviceInfo[n].state = POE_ONVIF_OFFLINE;

				chl_msk |= (1<<n);
			}
		}
	}

	return chl_msk;
}

//根据sn检查是否有同一个IPC的，如果有则记录通道号
int CPoeOnvifDeviceMan::GetChannlByMacaddr( char* macAddr, int len, bool bOnvifDevice, bool bClearOnvifDeviceMac )
{
	int chnn = -1;

	if( m_fGetIPCPhyChlByMac && m_pGetIpcChlFunParam )
	{
		m_fGetIPCPhyChlByMac( macAddr, len, bOnvifDevice, &chnn, bClearOnvifDeviceMac, m_pGetIpcChlFunParam );
	}

	return chnn;
}

int CPoeOnvifDeviceMan::GetDeviceIndex(const char *sn, int sn_cmp_len)
{
	int index = -1;
	int n = 0;

	//首先搜索是否有相同的sn，如果有这认为是同一个IPC
	for( n = 0; n < m_deviceCount; n++)
	{
		if(strncmp(sn, m_pTempDeviceInfo[n].device.sn, sn_cmp_len ) == 0)
		{
			index = n;
			return index;
		}
	}

	//如果没有搜索到sn，则认为是新IPC，先搜索没有连接IPC的POE通道
	for( n = 0; n < m_deviceCount; n++)
	{
		if(-1 == m_pTempDeviceInfo[n].poe_ipc_status)
		{
			index = n;
			return index;
		}
	}

	return index;
}

bool CPoeOnvifDeviceMan::CheckIPInDelayUse(unsigned int ip)
{
	list<DELAY_USE_IP>::iterator it = m_delayUseList.begin();
	for(; it != m_delayUseList.end(); it++)
	{
		if((*it).ip == ip)
		{
			//离线15秒钟了，可以再使用了
			if(GetTickCount() - (*it).offlineTime > 15000)
			{
				m_delayUseList.erase(it);
				break;
			}
			else	//离线时间太短，还残留在搜索列表中，暂不使用这个IP
			{
				return true;
			}
		}
	}

	return false;
}


bool CPoeOnvifDeviceMan::GetMacFromArp(unsigned char* szIpAddr, int ip_len, unsigned char* szMacAddr, int mac_len)
{
#ifndef __ENVIRONMENT_WIN32__
	u_char  hostMac[6] = {0};		//记录本地MAC
	unsigned int ipAddr = inet_addr( (char*)szIpAddr );
	u_long	hostIp = ipAddr;			//记录本地IP
	in_addr in_ip_addr;
	char name[] = "eth1";

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

	if (hostIp == ipAddr)
	{
		printf( "hostip ==== ipaddr %d,%d\n", hostIp, ipAddr );
		close(fd_arp);
		return false;
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
		memcpy( hostMac,ifr.ifr_hwaddr.sa_data,6 );
	}

	//printf( "host mac %02x.%02x.%02x.%02x.%02x.%02x,%d,%d,=== %d\n", 
	//	hostMac[0], hostMac[1], hostMac[2], hostMac[3], hostMac[4], hostMac[5],
	//	hostIp, ipAddr, inet_addr("10.151.151.1") );

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
	arp_packets.target_ip = ipAddr;
	memset(&arp_packets.target_Mac1,0,6);

	struct sockaddr to;
	memset(&to,0,sizeof(to));
	strcpy(to.sa_data, name); 

	//设置超时时间
	timeval timeout;
	timeout.tv_sec = 1;
	timeout.tv_usec = 0;
	fd_set   rdfd;
	bool bGetMac  = false;

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
			if( ( ARP_REPLAY==ntohs(arp_packet_rev.operate) ) && (arp_packet_rev.send_Ip == ipAddr ) )
			{
				//printf(" mac=%02x.%02x.%02x.%02x.%02x.%02x\n",arp_packet_rev.send_Mac1[0],arp_packet_rev.send_Mac1[1],arp_packet_rev.send_Mac1[2],arp_packet_rev.send_Mac1[3],arp_packet_rev.send_Mac1[4],arp_packet_rev.send_Mac1[5]);
				memcpy( szMacAddr, arp_packet_rev.send_Mac1,IPC_MAC_ADDR_LENS );
				bGetMac  = true;
				break;
			}
		}
	}

	close(fd_arp);	
	return bGetMac;
#else
	return false;
#endif
}


bool CPoeOnvifDeviceMan::GetMacFormDHCPFile( unsigned char* szIpAddr, int ip_len, unsigned char* szMacAddr, int mac_len )
{
	bool bGetMac = false;

#ifndef __ENVIRONMENT_WIN32__

	const char DHCP_LEASE_FILE_PATH[] = "/var/udhcpd0.leases";

	FILE *pFile = NULL;//fopen(DHCP_LEASE_FILE_PATH, "rb");
	char lines[160];
	char ip_tmp[32];
	char* pTmpRead = lines;
	char* pMacIpAddr = NULL;
	unsigned idx = 0;
	char macAddr[IPC_MAC_ADDR_LENS];


	pFile = fopen(DHCP_LEASE_FILE_PATH, "rb");
	if( pFile )
	{
		idx = 0;

		while( !feof( pFile ) )
		{
			pTmpRead = lines;
			memset( pTmpRead, 0, 160 );
			pMacIpAddr = fgets( (char*)pTmpRead, 160, pFile );
			if( pMacIpAddr )
			{
				//printf( "cuurent ip lines %s,%s,%d\n", pTmpRead, __FILE__, __LINE__ );
				memset( ip_tmp, 0, 32 );
				memset( macAddr, 0, IPC_MAC_ADDR_LENS );

				sscanf( &pTmpRead[18], "%s", ip_tmp );
				sscanf( pTmpRead, "%02x:%02x:%02x:%02x:%02x:%02x", &macAddr[0], &macAddr[1], &macAddr[2], &macAddr[3], &macAddr[4], &macAddr[5] );

				if( 0 == strcmp( (char*)ip_tmp, (char*)szIpAddr ) )
				{
					memcpy( szMacAddr, macAddr,IPC_MAC_ADDR_LENS );
					bGetMac = true;
					break;
				}
			}
			else
			{
				break;
			}
		}

		fclose( pFile );
		pFile = NULL;
	}

#endif
	return bGetMac;

}

bool CPoeOnvifDeviceMan::GetMacByOldOnvifList( unsigned int ipAddr, unsigned char* szMacAddr, int mac_len )
{
	bool bGetMac = false;
	for(int i = 0; i < m_deviceCount; i++)
	{
		if( MID_TVT != m_pDeviceInfo[i].device.manufacturerId )
		{
			if( m_pDeviceInfo[i].device.dwIP == ipAddr )
			{
				memcpy( szMacAddr, m_pDeviceInfo[i].IpcmacAddr, IPC_MAC_ADDR_LENS);
				bGetMac = true;
				break;
			}
		}
	}

	return bGetMac;
}

bool CPoeOnvifDeviceMan::GetMacaddrByIPAddr( unsigned char* szIpAddr, int ip_len, unsigned char* szMacAddr, int mac_len )
{

	bool bGetMac = false;

	bGetMac = GetMacFormDHCPFile( szIpAddr, ip_len, szMacAddr, mac_len );
	if( !bGetMac )
	{
		bGetMac = GetMacFromArp( szIpAddr, ip_len, szMacAddr, mac_len );
	}

	return bGetMac;
}

void CPoeOnvifDeviceMan::CreateDHCPAddrTable()
{
	FILE *pFile = fopen(DHCP_ADDR_TABLE_PATH, "wb");
	in_addr ipAddr;

	if(NULL != pFile)
	{
		ipAddr.s_addr = m_minIP;
		fprintf(pFile, "start\t\t%s\n", inet_ntoa(ipAddr));
		ipAddr.s_addr = m_maxIP;
		fprintf(pFile, "end\t\t%s\n", inet_ntoa(ipAddr));
		fprintf(pFile, "interface\t%s\n", POE_ETHER);
		fprintf(pFile, "pidfile\t%s\n", "/var/run/udhcpd.pid");
		ipAddr.s_addr = m_switchIP;
		fprintf(pFile, "opt\tdns\t%s\n", inet_ntoa(ipAddr));
		ipAddr.s_addr = m_switchNetmask;
		fprintf(pFile, "option\tsubnet\t%s\n", inet_ntoa(ipAddr));
		ipAddr.s_addr = m_switchIP;
		fprintf(pFile, "opt\trouter\t%s\n", inet_ntoa(ipAddr));
		fprintf(pFile, "option\tdomain\t%s\n", "local");
		fprintf(pFile, "option\tlease\t%d\t\t# 86400 seconds\n", 86400);

		fclose(pFile);
	}
}

