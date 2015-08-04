
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
	POE_ONVIF_OFFLINE,			//û���豸
	POE_ONVIF_ONLINE,			//�豸����
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
	POE_ONVIF_DEVICE_STATUS	state;				//�豸��״̬POE_ONVIF_DEVICE_STATUS
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

	/***���������߳�***/
	bool Start();

	/***ֹͣ�����߳�***/
	bool Stop();

	//�޸�switch��ַ��ʱ����Ҫͬ���޸�DHCP�������ĵ�ַ��
	void ChangeSwitchIP(unsigned int newIP, unsigned int netmask);

	/***�����µ��豸����ʱִ������Ļص�����***/
	void SetOnlineCallback(ONVIF_ONLINE_CB_FUN fOnlineCBFun, void *pUser);

	/***�����ݳ�ʱ�ж϶Ͽ�ʱ��ִ������Ļص�����***/
	void SetOfflineCallback(ONVIF_OFFLINE_CB_FUN fOfflineCBFun, void *pUser);

	void SetGetIpcPhyChnByMac(GET_IPC_PHY_CHN_BY_MAC fGetPhyChl, void *pUser );

	void ChangeChannelSn(POE_CHANNEL_SN *pChannelSn, bool bUsedChangPoeChnn );

	void EnablePoeDiscoverThreadRun( bool bRun );

	//��ȡPOEʹ�õ�ģʽ��true ��ʾһ��һģʽ
	bool GetPoeUsedMode() {return m_bIpcPhyIsRealChn;}
	void SetPoeUsedMode( bool bPoeUsedMode ){m_bIpcPhyIsRealChn=bPoeUsedMode;}

private:
	/***��ʱ�����豸����֪ͨ���߻�����***/
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

	//��dhcp�ļ��л�ȡmac��ַ
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

	/***����豸��Ϣ***/
	POE_ONVIF_DEVICE_INFO_EX	*m_pDeviceInfo;
	POE_ONVIF_DEVICE_INFO_EX	*m_pTempDeviceInfo;
	int							m_deviceCount;			//POE������Ŀ
	CPUB_Lock					m_deviceLock;

	list<DELAY_USE_IP>			m_delayUseList;

	unsigned int				m_switchIP;
	unsigned int				m_switchNetmask;

	//�鲥����TVT��IPCAM
	CMulticast					m_deviceSearch;

	//DHCP��ַ��Χ
	unsigned int				m_minIP;
	unsigned int				m_maxIP;

	//ture�� ʹ��һ��һģʽ
	bool                        m_bIpcPhyIsRealChn;

	//��IPC�豸��������ʱ����Ҫ�ر�POE�̴߳����������⸴�ӵ��߼�����
	bool                        m_bDiscoverRunning;
};

#endif
