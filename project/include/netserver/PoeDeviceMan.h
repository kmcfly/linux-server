#ifndef _POE_DEVICE_MAN_H_
#define _POE_DEVICE_MAN_H_

#include "Netpnp.h"
#include "Typedef.h"
#include "MulticastHeader.h"
#include "Multicast.h"
#include "PUB_common.h"
#include <vector>

using std::vector;

#define IPC_ONVIF_MAX_PORT_NUM 32

enum POE_DEVICE_STATUS
{
	POE_HAS_NO_DEVICE,			//没有设备
	POE_MAC_ADDR_UNSET,         //没有设置MAC地址
	POE_IP_UNSET,				//没有设置IP
	POE_WAIT_FOR_ACK,           //等待确认
	POE_IP_SET,					//设置好了IP
};

typedef struct _poe_device_info_
{
	unsigned char	MacAddr[BROADCAST_MAC_ADDR_LEN];
	char			passwd[BROADCAST_PASSWD_LEN];
	char			sn[64];
}POE_DEVICE_INFO;

typedef vector<POE_DEVICE_INFO> POE_DEVICE_INFO_STORE;
typedef vector<MultiCastInfo_v14> POE_SEARCH_REPLY_INFO_STORE;
typedef vector<BROADCAST_SEARCH_INFO> BROADCAST_SEARCH_INFO_STORE;

class CPoeDeviceMan
{
public:
	CPoeDeviceMan(void);
	~CPoeDeviceMan(void);

	bool Initial(int nPortNum);
	void Quit();

	/***开启POE功能，创建搜索线程***/
	bool Start();

	/***关闭POE功能，停止搜索线程***/
	bool Stop();

	const char * GetDevicePassword(const char *pSn);
	int GetIPCMacChannel(const char *macAddr, int len, bool bOnvifDevice, bool bClearOnvifDeviceMac );

private:
	/***上线下线回调、修改IPC的IP地址功能***/
	static RESULT WINAPI ConnectThread(void * pParam);
	void ConnectProc();
 
	/***接收内核收索的设备信息***/
	static RESULT WINAPI RecvThread(void *pParam);
	void RecvProc();

	/***通过广播设置设备的IP地址***/
	bool SetDeviceDHCP(POE_DEVICE_INFO &deviceInfo);

	/***获取搜索到的设备列表***/
	void GetDevice(BROADCAST_SEARCH_INFO_STORE &searchInfoStore); 
	
	/***更新设备(通过搜索回复的消息)***/
	void UpdateDeviceByResponseMsg();

	/***更新设备(通过连接状态消息)***/
	void UpdateDeviceByLinkStatusMsg();

private:
	CNetPnp	            m_deviceSearch;
	CMulticast          m_deviceIPSet;

	bool				m_bConnectRunning;
	PUB_thread_t		m_threadConnect;
	
	bool                m_bRecvRunning;
	PUB_thread_t        m_threadRecv;

	/***存放设备信息***/
	POE_DEVICE_INFO_STORE	    m_deviceInfoStore;
	unsigned int                m_deviceCount;			//POE网口数目
	
	/***存放广播回复消息***/
	POE_SEARCH_REPLY_INFO_STORE	m_replyInfoStore;

	/***存放内核返回的设备信息***/
	BROADCAST_SEARCH_INFO_STORE m_searchInfoStore;
	CPUB_Lock                   m_searchInfoLock;         

	unsigned short              m_linkStatus;
	CPUB_Lock                   m_linkStatusLock;

	CPUB_Lock                   m_sendMsgToPnp;
	
	CWaitEvent                  m_waitEvent; 

	//保存ONVIF的mac和port的maps
	IPC_ONVIF_MAC_PHY_PORT*     m_pIpcOnvifMacPhyPort;
};

#endif
