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
	POE_HAS_NO_DEVICE,			//û���豸
	POE_MAC_ADDR_UNSET,         //û������MAC��ַ
	POE_IP_UNSET,				//û������IP
	POE_WAIT_FOR_ACK,           //�ȴ�ȷ��
	POE_IP_SET,					//���ú���IP
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

	/***����POE���ܣ����������߳�***/
	bool Start();

	/***�ر�POE���ܣ�ֹͣ�����߳�***/
	bool Stop();

	const char * GetDevicePassword(const char *pSn);
	int GetIPCMacChannel(const char *macAddr, int len, bool bOnvifDevice, bool bClearOnvifDeviceMac );

private:
	/***�������߻ص����޸�IPC��IP��ַ����***/
	static RESULT WINAPI ConnectThread(void * pParam);
	void ConnectProc();
 
	/***�����ں��������豸��Ϣ***/
	static RESULT WINAPI RecvThread(void *pParam);
	void RecvProc();

	/***ͨ���㲥�����豸��IP��ַ***/
	bool SetDeviceDHCP(POE_DEVICE_INFO &deviceInfo);

	/***��ȡ���������豸�б�***/
	void GetDevice(BROADCAST_SEARCH_INFO_STORE &searchInfoStore); 
	
	/***�����豸(ͨ�������ظ�����Ϣ)***/
	void UpdateDeviceByResponseMsg();

	/***�����豸(ͨ������״̬��Ϣ)***/
	void UpdateDeviceByLinkStatusMsg();

private:
	CNetPnp	            m_deviceSearch;
	CMulticast          m_deviceIPSet;

	bool				m_bConnectRunning;
	PUB_thread_t		m_threadConnect;
	
	bool                m_bRecvRunning;
	PUB_thread_t        m_threadRecv;

	/***����豸��Ϣ***/
	POE_DEVICE_INFO_STORE	    m_deviceInfoStore;
	unsigned int                m_deviceCount;			//POE������Ŀ
	
	/***��Ź㲥�ظ���Ϣ***/
	POE_SEARCH_REPLY_INFO_STORE	m_replyInfoStore;

	/***����ں˷��ص��豸��Ϣ***/
	BROADCAST_SEARCH_INFO_STORE m_searchInfoStore;
	CPUB_Lock                   m_searchInfoLock;         

	unsigned short              m_linkStatus;
	CPUB_Lock                   m_linkStatusLock;

	CPUB_Lock                   m_sendMsgToPnp;
	
	CWaitEvent                  m_waitEvent; 

	//����ONVIF��mac��port��maps
	IPC_ONVIF_MAC_PHY_PORT*     m_pIpcOnvifMacPhyPort;
};

#endif
