// NatListenManager.h: interface for the CNatListenManager class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __NAT_NET_LISTEN_MANAGER_H__
#define __NAT_NET_LISTEN_MANAGER_H__

#include "NatUserDefine.h"

#ifdef NAT_TVT_DVR_4_0
#include "TVT_PubCom.h"
#else
#include "PUB_common.h"
#endif//NAT_TVT_DVR_4_0

#include "SWL_Public.h"
#include "NatSocket.h"
#include "NatPublicTypes.h"
#include "NatListenProc.h"
#include "NatTestProtocol.h"

#include <vector>

/**
 * 穿透侦听所占用的端口数量
 */
static const int NAT_LISTEN_PORT_COUNT = 1000;

typedef struct _nat_listen_manager_cache_
{
	int						iServerCount;		// 服务器个数
	NAT_SERVER_LIST_INFO*	pServerList;		// 服务器列表
	
}NAT_LISTEN_MANAGER_CACHE;

/**
 * 当穿透侦听管理器有改变时，回调
 * param[in] pCache			穿透侦听管理器的缓存
 * param[in] pParam1		填注册回调函数时，外部传进去的参数
 * param[in] pParam2		触发事件时的其它参数信息
 */
typedef int (*NAT_LISTEN_MAN_CACHE_CALLBACK)(const NAT_LISTEN_MANAGER_CACHE *pCache, void *pParam1, void *pParam2);

/**
 * 设备侦听管理器配置
 */
typedef struct _nat_listen_manager_config_
{
	char					deviceNo[64];		/**< 设备的序列号 */
	char					serverIp[256];       /**< NAT服务器IP地址 */
	unsigned short			serverPort;          /**< NAT服务器端口号 */
	unsigned short			localPort;			/**< 本地绑定的端口（穿透） */
	unsigned long			localIp;			/**< 本地绑定的IP地址（穿透） */
	unsigned char			refuseRelay;		/**< 设备是否拒绝中继服务。为0表示不拒绝；为非0表示拒绝。默认为0 */
	char					multicastif[256];	/**< 多播地址，主要用于UPNP，为空表示由内部选择 */
	
	//////////////////////////
	// 以下是扩展信息（ExtraInfo）

#ifdef __NAT_DEVICE_CONFIG_EX__
	// 扩展版
	char			caExtraInfo[512];		/**< 扩展信息，内部的数据只会在设备和客户端上解析，对穿透服务器是透明的 */
#else
	// 标准版
    unsigned long  deviceType;          /**< 设备类型，等价于实际设备中的产品型号 */
    char           deviceVersion[128];  /**< 设备版本号，以"-"为分隔符， 如"1.0.0-XXX-XX"
                                             其中，第1部分为主要版本号 */
    unsigned short deviceWebPort;       /**< 设备的WEB侦听端口号（TCP） */
    unsigned short deviceDataPort;      /**< 设备的数据侦听端口（TCP） */
#endif//__NAT_DEVICE_CONFIG_EX__
}NAT_LISTEN_MANAGER_CONFIG;

class CNatServerListFetcher;
class CNatUpnpMapper;

class CNatListenManager
{
public:
	CNatListenManager(NAT_ACCEPT_LINK_CALLBACK pCallback, void* pParam);
	virtual ~CNatListenManager();
public:
    /**
     * 设置穿透侦听管理器的缓存
     * 该表列是由调用者从缓存中加载出来的
     * @param[in] pCache				缓存
     * @param[in] pServerListCallback   缓存修改通知回调，可以为空
     * @param[in] pParam                回调参数pParam1的传入值
     */
    void SetCache(const NAT_LISTEN_MANAGER_CACHE* pCache, NAT_LISTEN_MAN_CACHE_CALLBACK pCacheCallback, void *pParam);

	void SetCache(const char* fileName);

    /**
     * 开始监听端口，检查是否有网络接入
     * @param[in] pConfig 设备配置
     * @return 如果成功，返回0；否则，返回其它值。
     */
    int StartListen(const NAT_LISTEN_MANAGER_CONFIG* pConfig);

    /**
     * 停止监听端口
     * @return 如果成功，返回0；否则，返回其它值。
     */
    int StopListen();

    /**
     * 是否已开始侦听
     */
    bool IsStarted() ;

	
     /* 重设穿透服务器
     * 只是重设与服务器的通信，不影响现有的客户端穿透连接
     * @param[in] szServerIp     服务IP地址
     * @param[in] usServerPort   服务器端口号
     */
    void ResetServer(const char *pServerIp, unsigned short sServerPort) ;

#ifdef __NAT_DEVICE_CONFIG_EX__   
	/**
	 * 设置设备的扩展信息
	 * 扩展版
	 * @param[in] extraInfo 设备扩展信息
	 */
	void SetDeviceExtraInfo(const char* extraInfo);
#else
    /**
     * 设置设备的WEB侦听端口号，仅标准版支持
     * @param[in] deviceWebPort  设备的WEB侦听端口号（TCP）
     */
    void SetDeviceWebPort(unsigned short deviceWebPort);
 
    /**
     * 设置设备的数据侦听端口（TCP），仅标准版支持
     * @param[in] deviceDataPort 设备的数据侦听端口（TCP）
     */
    void SetDeviceDataPort(unsigned short deviceDataPort);

#endif
    /**
     * 获取设备注册到穿透服务器状态
     */
    NAT_DEVICE_SERVER_STATE GetServerState() ;

    /**
     * 获取设备注册到穿透服务器的错误
     */
    NAT_DEVICE_SERVER_ERROR GetServerError();

	/**
	 * 获取设备的公网IP
	 * 仅在 GetServerState() == NAT_STATE_CONNECTED 时有效
	 */
	unsigned long GetPublicIp();

	/**
	 * 获取设备的公网端口
	 * 仅在 GetServerState() == NAT_STATE_CONNECTED 时有效
	 */
	unsigned short GetPublicPort();

private:
	static PUB_THREAD_RESULT PUB_THREAD_CALL WorkThreadFunc(void *pParam);
	int RunWork();

	void Clear();

	void ProcRegToServers(unsigned long curTickCount, bool &runMore, bool &allNeedGetServer);
	void ProcCacheChanging(unsigned long curTickCount, bool &runMore);

private:
	static const int REG_SERV_COUNT = 2;
	static const int FETCH_SERV_LIST_TIMEOUT = 3 * 60 * 1000;

	static const int REG_SERV_ERROR_TIMEOUT = 10 * 60 * 1000;
	static const int UPNP_INIT_TIMEOUT = 3 * 1000;
	static const int LISTEN_START_INTERVAL = 10 * 1000;

	enum ListenManState
	{
		STATE_NONE,
		STATE_INITED,
		STATE_MAPPING_PORT,
		STATE_LISTENING,
		STATE_ERROR
	};

	class CListenContext;
	class CUpnpHelper;

	friend class CUpnpHelper;

	typedef std::vector<NAT_SERVER_LIST_INFO> NatServerList;

	class CNatServerCache
	{
	public:
		bool				isChanging;		// 正在改变，需等待 CNatServerListFetcher 完成后，设置为false，并保存cache
		NatServerList		serverList;		// 服务器列表

		CNatServerCache()
			:
			isChanging(false),
			serverList()
		{
		};
		~CNatServerCache()
		{
			serverList.clear();
		};

	};


private:
	bool ReadNatServerCache(CNatServerCache &cache, const char* fileName);
	bool WriteNatServerCache(const CNatServerCache &cache, const char* fileName);
private:
	//测试服务器速度
	NAT_LISTEN_MANAGER_CONFIG m_config;
	CPUB_Lock m_lock;
	CNatServerListFetcher *m_servListFetcher;
	CNatUpnpMapper *m_upnpMapper;
	CUpnpHelper		*m_upnpHelper;
	CNatServerCache m_cache;
	char			m_cacheFileName[256];
	CListenContext  *m_listenContexts[REG_SERV_COUNT];	
	ListenManState m_state;
	unsigned long m_startTime;
	unsigned long m_lastErrorTime;

	NAT_ACCEPT_LINK_CALLBACK	m_pAcceptCallback;			// 有客户端connect上来，accept后调用此回调函数
	void						*m_pAcceptCallbackParam;	// 回调函数的参数
	
	NAT_LISTEN_MAN_CACHE_CALLBACK	m_pCacheCallback;
	void							*m_pCacheCallbackParam; 

	bool m_workThreadRunning;
	PUB_thread_t m_workThreadID;

	int m_curServerIndex;	// 当前未使用的服务器索引号
	CNatListenProc		*m_listenProc;
};

#endif//__NAT_NET_LISTEN_MANAGER_H__
