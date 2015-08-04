#ifndef _NAT_SEARCH_LAN_DEVICE_H_
#define _NAT_SEARCH_LAN_DEVICE_H_

#include "NatUserDefine.h"

#ifdef NAT_TVT_DVR_4_0
#include "TVT_PubCom.h"
#include "TVT_Protocol.h"
#else
#include "PUB_common.h"
#endif//NAT_TVT_DVR_4_0

#include "NatPublicTypes.h"

typedef struct _nat_device_lan_searcher_info_
{
	unsigned char			MacAddr[6];
	unsigned long			ipaddr;
	unsigned long			netmask;
	unsigned short			netport;
	unsigned short			nHttpPort;
	NAT_DEVICE_VER_TYPE		softver;    //�豸�汾��
}NAT_DEVICE_LAN_SEARCHER_INFO;

/**
 * ���������豸�Ժ󣬵��øú���
 * @param[in] info �豸��Ϣ
 * @param[in] pObject �ص���ض���ָ��
 * @param[in] nParam ����������
 * @return ����ֵδ���塣
 */
typedef int (*NAT_SEARCH_CALLBACKEX)(const NAT_DEVICE_LAN_SEARCHER_INFO &info, void *pObject, void *pParam);

class CNatDeviceLanSearcher
{
public:
	CNatDeviceLanSearcher();
	~CNatDeviceLanSearcher();

	bool Start(unsigned long iTimeOut, char *pSn);
	bool Stop();

	bool IsCompleted();
	bool IsSucceeded();

	void SetSearchCallBack(NAT_SEARCH_CALLBACKEX pCallBack, void* pObject);

	void GetDeviceInfo(NAT_DEVICE_LAN_SEARCHER_INFO &deviceInfo);

	 /**
     * ���ٺ�����
     * @return ����ɹ�������0�����򣬷�������ֵ��
     */  
    int Destroy();

private:
	void InternalStop();

	static PUB_THREAD_RESULT PUB_THREAD_CALL SearchThread(void *pParam);
	void SearchPro();
	void SetDeviceInfo(NAT_DEVICE_LAN_SEARCHER_INFO &deviceInfo);

private:
	CPUB_Lock			   m_syncLock;

	bool                   m_bCompleted; //�ⲿ��ѯ�鿴��־
	bool                   m_bSuccess;

	unsigned long          m_timeout;
	bool			       m_bSearching;
	PUB_thread_t	       m_serachThreadHandle;
	char			       m_deviceSN[TVT_MAX_TITLE_LEN+4];

	NAT_DEVICE_LAN_SEARCHER_INFO    m_deviceInfo;
	CPUB_Lock			            m_deviceLock;

	NAT_SEARCH_CALLBACKEX			m_pSearchCallback;
	void							*m_pSearchCallbackParam;
};


#endif