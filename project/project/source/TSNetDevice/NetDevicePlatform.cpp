#include <assert.h>
#include <vector>
#include <string.h>

#include "NetDevicePlatform.h"
#include "TSNetDevice.h"
#include "SWL_Public.h"
#include "TSMulticast.h"
#include "DDPublic.h"
#include "SWL_MultiNetComm.h"
#include "DeviceTypeHeader.h"
#include "MulticastHeader.h"

//////////////////////////////////////////////////////////////////////////

typedef struct _nd_device_info_ex
{
	MULTICAST_INFO_V14		deviceInfo;
	int						lastOnlineTime;		//最后一次被发现在线时间（时间以开机累积时间计，单位ms）
}ND_DEVICE_INFO_EX;

const unsigned int DEVICE_KEEP_LIVE_TIME	= 10 * 1000;  //设备如果10秒没有搜到 就认为此设备已经不在线了
const unsigned int REQUEST_INTERVAL			= 5 * 1000;		//每5秒钟发送一次组播请求

//局域网搜索IPCAM相关
CTSMulticast	g_multicast;
char			*g_pRecvBuffer = NULL;			// 接收网络数据的缓冲区
long			g_bufferSize = 0;
bool			g_bSearching = false;
PUB_thread_t	g_searchThread = PUB_THREAD_ID_NOINIT;

std::vector<ND_DEVICE_INFO_EX>	g_arrDeviceInfo;
CPUB_Lock						g_listLock;

int                             g_iSearchStartCount = 0;

CTS_MultiNetComm				*g_pMultiNetComm;
ND_DEVICE_NETWORK_INFO			* g_pDeviceNetworkInfo;

PUB_THREAD_RESULT PUB_THREAD_CALL SearchThread(void *pParam)
{
#ifdef	__ENVIRONMENT_LINUX__
	pid_t tid = syscall(__NR_gettid);
	printf("%s, %d, tid=%d\n", __FILE__, __LINE__, tid);
#endif

	unsigned long prevSearchTime = 0;
	std::vector<ND_DEVICE_INFO_EX>::iterator it;

	while(g_bSearching)
	{
		unsigned long curTime = GetTickCount();

		//if( g_iSearchStartCount <= 0 )
		//{
		//	PUB_Sleep(2000);
		//	continue;
		//}
        //
		//g_iSearchStartCount--;
		//prevSearchTime = curTime - REQUEST_INTERVAL;

		//定期发送组播搜索命令
		if((curTime < prevSearchTime) || ((curTime - prevSearchTime) >= REQUEST_INTERVAL))
		{
			//发送组播搜索数据包
			MULTICAST_INFO_V14  info;
			memset(&info, 0, sizeof(MULTICAST_INFO_V14));
			info.mulHead.head	= FLAG_MULTIHEAD;
			info.mulHead.cmd	= MULTICAST_CMD_SEARCH;
			info.mulHead.ver	= MULTICAST_10007_VERSION;
			g_multicast.SendTo(&info, sizeof(MULTICAST_INFO));

			prevSearchTime = curTime;
		}

		//更新在线IPCAM的列表
		g_listLock.Lock();
		for(it = g_arrDeviceInfo.begin(); it != g_arrDeviceInfo.end();)
		{
			if(curTime > (*it).lastOnlineTime && ((curTime - (*it).lastOnlineTime) >= DEVICE_KEEP_LIVE_TIME))
			{
				it = g_arrDeviceInfo.erase(it);
			}
			else
			{
				it++;
			}
		}
		g_listLock.UnLock();

		//接收IPCAM的在线报告
		int nRecv = 0;
		while((nRecv = g_multicast.RecvFrom(g_pRecvBuffer, g_bufferSize)) > 0)
		{
			unsigned long flag;
			memcpy(&flag, g_pRecvBuffer, sizeof(unsigned long));

			if(flag == FLAG_MULTIHEAD)
			{
				if ( nRecv < sizeof(MULTICAST_INFO) )
				{
					continue;
				}

				MULTICAST_INFO_V14 multicastInfo;
				memcpy(&multicastInfo, g_pRecvBuffer, sizeof(MULTICAST_INFO_V14));

				if(multicastInfo.mulHead.cmd != MULTICAST_CMD_INFO)
				{
					if((MULTICAST_CMD_SEARCH == multicastInfo.mulHead.cmd) && (MULTICAST_10007_VERSION == multicastInfo.mulHead.ver))
					{
						prevSearchTime = curTime;
					}
					continue;
				}
				if(multicastInfo.deviceType != DEVICE_TYPE_DEF::TVT_PRODUCT_IPC)
				{
					continue;
				}

				g_listLock.Lock();

				for(it = g_arrDeviceInfo.begin(); it != g_arrDeviceInfo.end(); it++)
				{							
					ND_DEVICE_INFO_EX &devSearchInfo = *it;

					if(memcmp(devSearchInfo.deviceInfo.MacAddr, multicastInfo.MacAddr, sizeof(multicastInfo.MacAddr)) == 0)	//该设备已在列表中，更新该设备的信息
					{
						devSearchInfo.lastOnlineTime = curTime;
						if((devSearchInfo.deviceInfo.ipaddr != multicastInfo.ipaddr) \
							|| (devSearchInfo.deviceInfo.netport != multicastInfo.netport))
						{										
							//删除先前使用此IP地址和网络端口的设备节点
							std::vector<ND_DEVICE_INFO_EX>::iterator it2;
							for(it2 = g_arrDeviceInfo.begin(); it2 != g_arrDeviceInfo.end(); it2++)
							{
								if(((*it2).deviceInfo.ipaddr == multicastInfo.ipaddr) \
									&& ((*it2).deviceInfo.netport == multicastInfo.netport))
								{
									g_arrDeviceInfo.erase(it2);
									break;
								}
							}

							devSearchInfo.deviceInfo = multicastInfo;
						}
						else if(multicastInfo.mulHead.ver >= MULTICAST_10007_VERSION) //如果是10007以上的版本，因为里面包含设备类型名，所以需要更新信息
						{
							devSearchInfo.deviceInfo = multicastInfo;
						}

						break;
					}
				}

				if(g_arrDeviceInfo.end() == it)
				{
					//如果设备不在元组中，加入元组
					ND_DEVICE_INFO_EX devSearchInfo;
					memset(&devSearchInfo, 0, sizeof(ND_DEVICE_INFO_EX));
					devSearchInfo.deviceInfo = multicastInfo;
					devSearchInfo.lastOnlineTime = curTime;
					g_arrDeviceInfo.push_back(devSearchInfo);
				}

				g_listLock.UnLock();
			}
		}

		if (NULL != g_pDeviceNetworkInfo)
		{
			MULTICAST_INFO  info;
			memset(&info, 0, sizeof(info));

			info.mulHead.head	= FLAG_MULTIHEAD;
			info.mulHead.cmd	= MULTICAST_CMD_SET;
			info.mulHead.ver	= MULTICAST_VER;

			info.ipaddr = g_pDeviceNetworkInfo->newIpaddr;
			info.netmask = g_pDeviceNetworkInfo->newNetmask;
			info.route = g_pDeviceNetworkInfo->newGateway;
			memcpy(info.szPasswd, g_pDeviceNetworkInfo->password, sizeof(info.szPasswd));

			int macaddr[6] = {0};
			sscanf(g_pDeviceNetworkInfo->szID, "%x:%x:%x:%x:%x:%x", &macaddr[0], &macaddr[1], 
				&macaddr[2], &macaddr[3], &macaddr[4], &macaddr[5]);
			for(int i = 0; i < 6; i++)
			{
				info.MacAddr[i] = macaddr[i];
			}

			g_multicast.SendTo(&info, sizeof(info));

			delete g_pDeviceNetworkInfo;
			g_pDeviceNetworkInfo = NULL;
			prevSearchTime = 0xffffffff;
		}

		PUB_Sleep(1);

	}

	return 0;
}

int nd_init()
{
	g_pMultiNetComm = NULL;

	if(0 != g_multicast.Init(DEFAULT_LOCAL_PORT, MULTICAST_PORT, MULTICAST_IP))
	{
		return -1;
	}
	g_multicast.AddRecvGroup(MULTICAST_IPTOOL_RECV_IP, MULTICAST_PORT);

	g_bufferSize = 3200;
	g_pRecvBuffer = new char[g_bufferSize];

	g_searchThread = PUB_CreateThread(SearchThread, NULL, &g_bSearching);
	if(PUB_CREATE_THREAD_FAIL == g_searchThread)
	{
		assert(false);
	}

	return 0;
}

int nd_quit()
{
	if(PUB_THREAD_ID_NOINIT != g_searchThread)
	{
		PUB_ExitThread(&g_searchThread, &g_bSearching);
	}

	if(NULL != g_pRecvBuffer)
	{
		delete [] g_pRecvBuffer;
		g_pRecvBuffer = NULL;
	}
	
	g_multicast.Quit();

	return 0;
}

///
int nd_get_info(ND_LIB_INFO* p_lib_info)
{
	p_lib_info->manufacturerId = MID_TVT;
	p_lib_info->bNeedHttpPort = 0;
	p_lib_info->bNeedCtrlPort = 0;

	return 0;
}

void nd_on_timer(unsigned long curTime)
{

}

int nd_get_discovered_device_num()
{
	g_listLock.Lock();
	int num = g_arrDeviceInfo.size();
	g_listLock.UnLock();

	return num;
}

int nd_get_discovered_device_info(ND_DEVICE_INFO *pDeviceInfo, int deviceNum)
{
	if( NULL == pDeviceInfo || 0 == deviceNum )
	{
		g_iSearchStartCount = 4;
		return 0;
	}
	else if( 0xffff == deviceNum && (0 == strcmp(pDeviceInfo->productModel, "5a5a5a5a")) )
	{
		// 获取IPC的厂家名称
		// 因为接口是统一的，增加接口比较麻烦，所以使用同一接口，根据参数不一样，获取不同的功能。
		return MANU_ONVIF_TVT;
	}
	else if( 0xffff == deviceNum && (0 == strcmp(pDeviceInfo->productModel, "5b5b5b5b")) )
	{
		// 设置当前设备poe通道数目
		// 因为接口是统一的，增加接口比较麻烦，所以使用同一接口，根据参数不一样，获取不同的功能。

		return 0;

	}

	g_listLock.Lock();

	memset(pDeviceInfo, 0, sizeof(ND_DEVICE_INFO)*deviceNum);

	int actualNum = (deviceNum <= g_arrDeviceInfo.size()) ? deviceNum : g_arrDeviceInfo.size();

	for(int i= 0; i < actualNum; i++)
	{
		in_addr ipaddr;
		ipaddr.s_addr = g_arrDeviceInfo[i].deviceInfo.ipaddr;
		strcpy(pDeviceInfo[i].szAddr, inet_ntoa(ipaddr));
		pDeviceInfo[i].netmask = g_arrDeviceInfo[i].deviceInfo.netmask;
		pDeviceInfo[i].gateway = g_arrDeviceInfo[i].deviceInfo.route;
		pDeviceInfo[i].netport = g_arrDeviceInfo[i].deviceInfo.netport;
		pDeviceInfo[i].nHttpPort = g_arrDeviceInfo[i].deviceInfo.nHttpPort;
		unsigned char *pMacAddr = g_arrDeviceInfo[i].deviceInfo.MacAddr;
		sprintf(pDeviceInfo[i].szID, "%02x:%02x:%02x:%02x:%02x:%02x", pMacAddr[0], \
			pMacAddr[1], pMacAddr[2], pMacAddr[3], pMacAddr[4], pMacAddr[5]);
		sprintf(pDeviceInfo[i].firmwareVersion, "%d.%d", \
			(g_arrDeviceInfo[i].deviceInfo.softver >> 16) & 0xffff, g_arrDeviceInfo[i].deviceInfo.softver & 0xffff);

		pDeviceInfo[i].manufacturerId = MID_TVT;

		strcpy(pDeviceInfo[i].manufacturerName, g_arrDeviceInfo[i].deviceInfo.szManufacturery);

		if(g_arrDeviceInfo[i].deviceInfo.mulHead.ver >= MULTICAST_10007_VERSION)
		{
			strncpy(pDeviceInfo[i].productModel, g_arrDeviceInfo[i].deviceInfo.szProductType, 35);
		}
	}

	g_listLock.UnLock();

	return actualNum;
}

void nd_set_discovered_device_info(ND_DEVICE_NETWORK_INFO * p_NetworkInfo)
{
	if (NULL != p_NetworkInfo)
	{
		if (NULL == g_pDeviceNetworkInfo)
		{
			g_pDeviceNetworkInfo = new ND_DEVICE_NETWORK_INFO;
		}
		* g_pDeviceNetworkInfo = * p_NetworkInfo;
	}
}


CNetDevice* nd_create()
{
	return new CTSNetDevice();
}

void nd_destroy(CNetDevice* p_net_device)
{
	if(p_net_device)
	{
		delete p_net_device;
	}
}

//end

