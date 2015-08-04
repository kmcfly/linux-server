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
 * ��͸������ռ�õĶ˿�����
 */
static const int NAT_LISTEN_PORT_COUNT = 1000;

typedef struct _nat_listen_manager_cache_
{
	int						iServerCount;		// ����������
	NAT_SERVER_LIST_INFO*	pServerList;		// �������б�
	
}NAT_LISTEN_MANAGER_CACHE;

/**
 * ����͸�����������иı�ʱ���ص�
 * param[in] pCache			��͸�����������Ļ���
 * param[in] pParam1		��ע��ص�����ʱ���ⲿ����ȥ�Ĳ���
 * param[in] pParam2		�����¼�ʱ������������Ϣ
 */
typedef int (*NAT_LISTEN_MAN_CACHE_CALLBACK)(const NAT_LISTEN_MANAGER_CACHE *pCache, void *pParam1, void *pParam2);

/**
 * �豸��������������
 */
typedef struct _nat_listen_manager_config_
{
	char					deviceNo[64];		/**< �豸�����к� */
	char					serverIp[256];       /**< NAT������IP��ַ */
	unsigned short			serverPort;          /**< NAT�������˿ں� */
	unsigned short			localPort;			/**< ���ذ󶨵Ķ˿ڣ���͸�� */
	unsigned long			localIp;			/**< ���ذ󶨵�IP��ַ����͸�� */
	unsigned char			refuseRelay;		/**< �豸�Ƿ�ܾ��м̷���Ϊ0��ʾ���ܾ���Ϊ��0��ʾ�ܾ���Ĭ��Ϊ0 */
	char					multicastif[256];	/**< �ಥ��ַ����Ҫ����UPNP��Ϊ�ձ�ʾ���ڲ�ѡ�� */
	
	//////////////////////////
	// ��������չ��Ϣ��ExtraInfo��

#ifdef __NAT_DEVICE_CONFIG_EX__
	// ��չ��
	char			caExtraInfo[512];		/**< ��չ��Ϣ���ڲ�������ֻ�����豸�Ϳͻ����Ͻ������Դ�͸��������͸���� */
#else
	// ��׼��
    unsigned long  deviceType;          /**< �豸���ͣ��ȼ���ʵ���豸�еĲ�Ʒ�ͺ� */
    char           deviceVersion[128];  /**< �豸�汾�ţ���"-"Ϊ�ָ����� ��"1.0.0-XXX-XX"
                                             ���У���1����Ϊ��Ҫ�汾�� */
    unsigned short deviceWebPort;       /**< �豸��WEB�����˿ںţ�TCP�� */
    unsigned short deviceDataPort;      /**< �豸�����������˿ڣ�TCP�� */
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
     * ���ô�͸�����������Ļ���
     * �ñ������ɵ����ߴӻ����м��س�����
     * @param[in] pCache				����
     * @param[in] pServerListCallback   �����޸�֪ͨ�ص�������Ϊ��
     * @param[in] pParam                �ص�����pParam1�Ĵ���ֵ
     */
    void SetCache(const NAT_LISTEN_MANAGER_CACHE* pCache, NAT_LISTEN_MAN_CACHE_CALLBACK pCacheCallback, void *pParam);

	void SetCache(const char* fileName);

    /**
     * ��ʼ�����˿ڣ�����Ƿ����������
     * @param[in] pConfig �豸����
     * @return ����ɹ�������0�����򣬷�������ֵ��
     */
    int StartListen(const NAT_LISTEN_MANAGER_CONFIG* pConfig);

    /**
     * ֹͣ�����˿�
     * @return ����ɹ�������0�����򣬷�������ֵ��
     */
    int StopListen();

    /**
     * �Ƿ��ѿ�ʼ����
     */
    bool IsStarted() ;

	
     /* ���贩͸������
     * ֻ���������������ͨ�ţ���Ӱ�����еĿͻ��˴�͸����
     * @param[in] szServerIp     ����IP��ַ
     * @param[in] usServerPort   �������˿ں�
     */
    void ResetServer(const char *pServerIp, unsigned short sServerPort) ;

#ifdef __NAT_DEVICE_CONFIG_EX__   
	/**
	 * �����豸����չ��Ϣ
	 * ��չ��
	 * @param[in] extraInfo �豸��չ��Ϣ
	 */
	void SetDeviceExtraInfo(const char* extraInfo);
#else
    /**
     * �����豸��WEB�����˿ںţ�����׼��֧��
     * @param[in] deviceWebPort  �豸��WEB�����˿ںţ�TCP��
     */
    void SetDeviceWebPort(unsigned short deviceWebPort);
 
    /**
     * �����豸�����������˿ڣ�TCP��������׼��֧��
     * @param[in] deviceDataPort �豸�����������˿ڣ�TCP��
     */
    void SetDeviceDataPort(unsigned short deviceDataPort);

#endif
    /**
     * ��ȡ�豸ע�ᵽ��͸������״̬
     */
    NAT_DEVICE_SERVER_STATE GetServerState() ;

    /**
     * ��ȡ�豸ע�ᵽ��͸�������Ĵ���
     */
    NAT_DEVICE_SERVER_ERROR GetServerError();

	/**
	 * ��ȡ�豸�Ĺ���IP
	 * ���� GetServerState() == NAT_STATE_CONNECTED ʱ��Ч
	 */
	unsigned long GetPublicIp();

	/**
	 * ��ȡ�豸�Ĺ����˿�
	 * ���� GetServerState() == NAT_STATE_CONNECTED ʱ��Ч
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
		bool				isChanging;		// ���ڸı䣬��ȴ� CNatServerListFetcher ��ɺ�����Ϊfalse��������cache
		NatServerList		serverList;		// �������б�

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
	//���Է������ٶ�
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

	NAT_ACCEPT_LINK_CALLBACK	m_pAcceptCallback;			// �пͻ���connect������accept����ô˻ص�����
	void						*m_pAcceptCallbackParam;	// �ص������Ĳ���
	
	NAT_LISTEN_MAN_CACHE_CALLBACK	m_pCacheCallback;
	void							*m_pCacheCallbackParam; 

	bool m_workThreadRunning;
	PUB_thread_t m_workThreadID;

	int m_curServerIndex;	// ��ǰδʹ�õķ�����������
	CNatListenProc		*m_listenProc;
};

#endif//__NAT_NET_LISTEN_MANAGER_H__
