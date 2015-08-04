
#ifndef _POE_ONVIF_DEVICE_MAN_H_
#define _POE_ONVIF_DEVICE_MAN_H_

#include "PUB_common.h"
#include "Multicast.h"
#include "MulticastHeader.h"

#ifdef __ENVIRONMENT_WIN32__
const char DHCP_ADDR_TABLE_PATH[] = "d:\\udhcpd.conf";
#else
const char DHCP_ADDR_TABLE_PATH[] = "/etc/udhcpd.conf";
#endif

#define IPC_MAC_ADDR_LENS 6

enum POE_ONVIF_DEVICE_STATUS
{
	POE_ONVIF_OFFLINE,			//没有设备
	POE_ONVIF_ONLINE,			//设备上线
	POE_ONVIF_ETH0_DEVICE,
};

typedef struct _poe_onvif_device_info_
{
	short			channel;
	unsigned short	dataPort;
	unsigned int	dwIP;
	char			sn[64];
	unsigned int	manufacturerId;
	char 			manufacturerName[36];
	char			productModel[36];
}POE_ONVIF_DEVICE_INFO;

typedef struct _poe_ipc_ext_ctrl_param
{
	int     op_type;
	bool    bPoeChnPtoP;
	bool    bEth0Ipc;
	unsigned long dwIPAddr;
	unsigned long delChnMsk;
	char			snID[64];
}POE_IPC_EXT_CTRL_PARAM;

typedef struct _poe_onvif_device_info_ex_ 
{
	POE_ONVIF_DEVICE_INFO	device;
	POE_ONVIF_DEVICE_STATUS	state;				//设备的状态POE_ONVIF_DEVICE_STATUS
	unsigned long search_times;
	unsigned long poe_ipc_status;
	unsigned char IpcmacAddr[IPC_MAC_ADDR_LENS];
}POE_ONVIF_DEVICE_INFO_EX;

typedef struct _poe_mac_ip_map
{
	unsigned char mac_addr[IPC_MAC_ADDR_LENS];
	unsigned char ip_addr[64];
	unsigned char b_has_maps;
}POE_MAC_IP_MAP;

typedef struct _poe_channel_sn_
{
	unsigned long		channel;
	int                 poe_ipc_status;
	char				sn[64];
}POE_CHANNEL_SN;

typedef struct _delay_use_ip_
{
	unsigned int		ip;
	unsigned long		offlineTime;
}DELAY_USE_IP;

typedef struct _tvt_ipc_discover_
{
	unsigned long			discoverTimeTick;
	MultiCastInfo_v14		tvtIpcInfo;
}TVT_IPC_DISCOVER;

typedef void (*ONVIF_ONLINE_CB_FUN)(POE_ONVIF_DEVICE_INFO &deviceInfo, POE_IPC_EXT_CTRL_PARAM& stCtrlParam, void *pUser);
typedef void (*ONVIF_OFFLINE_CB_FUN)(POE_ONVIF_DEVICE_INFO &deviceInfo, POE_IPC_EXT_CTRL_PARAM& stCtrlParam, void *pUser);
typedef void (*GET_IPC_PHY_CHN_BY_MAC)( char* macAddr, int len, bool bOnvifDevice, int* chnn, bool bClearOnvifDeviceMac, void *pUser);

class CPoeOnvifDeviceMan
{
	typedef struct _device_sn_info_
	{
		unsigned long		prevOnlineTick;
		char				sn[64];
	}DEVICE_SN_INFO;

public:
	CPoeOnvifDeviceMan(void);
	~CPoeOnvifDeviceMan(void);

	bool Initial(int nPortNum, POE_CHANNEL_SN *pChannelSn);
	void Quit();

	/***创建搜索线程***/
	bool Start();

	/***停止搜索线程***/
	bool Stop();

	//修改switch地址的时候，需要同步修改DHCP服务器的地址池
	void ChangeSwitchIP(unsigned int newIP, unsigned int netmask);

	/***当有新的设备上线时执行这里的回调函数***/
	void SetOnlineCallback(ONVIF_ONLINE_CB_FUN fOnlineCBFun, void *pUser);

	/***当根据超时判断断开时，执行这里的回调函数***/
	void SetOfflineCallback(ONVIF_OFFLINE_CB_FUN fOfflineCBFun, void *pUser);

	void SetGetIpcPhyChnByMac(GET_IPC_PHY_CHN_BY_MAC fGetPhyChl, void *pUser );

	void ChangeChannelSn(POE_CHANNEL_SN *pChannelSn, bool bUsedChangPoeChnn );

	void EnablePoeDiscoverThreadRun( bool bRun );

	//获取POE使用的模式，true 表示一对一模式
	bool GetPoeUsedMode() {return m_bIpcPhyIsRealChn;}
	void SetPoeUsedMode( bool bPoeUsedMode ){m_bIpcPhyIsRealChn=bPoeUsedMode;}

private:
	/***定时搜索设备，并通知上线或下线***/
	static RESULT WINAPI DiscoverDeviceThread(void * pParam);
	void DiscoverDeviceProc();

#ifndef __ENVIRONMENT_WIN32__
	static RESULT WINAPI SearchTvtIpcThread(void *pParam);
	void SearchTvtIpcProc();
	void SearchTvtIpc(list<MultiCastInfo_v14> &tvtIpcList);
#endif

	int GetDeviceIndex(const char *sn, int sn_cmp_len );
	int GetChannlByMacaddr( char* macAddr, int len, bool bOnvifDevice, bool bClearOnvifDeviceMac );
	int GetSameSnOrIPItem( char* sn, int sn_len, int dwip, int channnel );
	bool CheckIPInDelayUse(unsigned int ip);

	void CreateDHCPAddrTable();

	//从dhcp文件中获取mac地址
	bool GetMacFormDHCPFile( unsigned char* szIpAddr, int ip_len, unsigned char* szMacAddr, int mac_len );
	bool GetMacFromArp(unsigned char* szIpAddr, int ip_len, unsigned char* szMacAddr, int mac_len);
	bool GetMacaddrByIPAddr( unsigned char* szIpAddr, int ip_len, unsigned char* szMacAddr, int mac_len );
	bool GetMacByOldOnvifList( unsigned int ipAddr, unsigned char* szMacAddr, int mac_len );

private:
	ONVIF_ONLINE_CB_FUN			m_fOnlineCBFun;
	void						*m_pOnlineCBFunParam;
	ONVIF_OFFLINE_CB_FUN		m_fOfflineCBFun;
	void						*m_pOfflineCBFunParam;

	GET_IPC_PHY_CHN_BY_MAC      m_fGetIPCPhyChlByMac;
	void                        *m_pGetIpcChlFunParam;

	bool						m_bDiscovering;
	PUB_thread_t				m_threadDiscover;

	bool						m_bSearchTvtIpc;
	PUB_thread_t				m_threadSearchTvtIpc;

	list<MultiCastInfo_v14>		m_listTvtIpc;
	CPUB_Lock					m_listIpcLock;

	/***存放设备信息***/
	POE_ONVIF_DEVICE_INFO_EX	*m_pDeviceInfo;
	POE_ONVIF_DEVICE_INFO_EX	*m_pTempDeviceInfo;
	int							m_deviceCount;			//POE网口数目
	CPUB_Lock					m_deviceLock;

	list<DELAY_USE_IP>			m_delayUseList;

	unsigned int				m_switchIP;
	unsigned int				m_switchNetmask;

	//组播搜索TVT的IPCAM
	CMulticast					m_deviceSearch;

	//DHCP地址范围
	unsigned int				m_minIP;
	unsigned int				m_maxIP;

	//ture， 使用一对一模式
	bool                        m_bIpcPhyIsRealChn;

	//在IPC设备处理界面的时候需要关闭POE线程处理，这样避免复杂的逻辑处理
	bool                        m_bDiscoverRunning;
};

#endif
