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
	NAT_DEVICE_VER_TYPE		softver;    //设备版本号
}NAT_DEVICE_LAN_SEARCHER_INFO;

/**
 * 当搜索到设备以后，调用该函数
 * @param[in] info 设备信息
 * @param[in] pObject 回调相关对象指针
 * @param[in] nParam 保留参数。
 * @return 返回值未定义。
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
     * 销毁函数。
     * @return 如果成功，返回0；否则，返回其它值。
     */  
    int Destroy();

private:
	void InternalStop();

	static PUB_THREAD_RESULT PUB_THREAD_CALL SearchThread(void *pParam);
	void SearchPro();
	void SetDeviceInfo(NAT_DEVICE_LAN_SEARCHER_INFO &deviceInfo);

private:
	CPUB_Lock			   m_syncLock;

	bool                   m_bCompleted; //外部轮询查看标志
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