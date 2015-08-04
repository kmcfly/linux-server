// NatConnectProc.h: interface for the CNatConnectProc class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _NAT_CONNECT_PROC_H_
#define _NAT_CONNECT_PROC_H_

#include "NatUserDefine.h"

#ifdef NAT_TVT_DVR_4_0
#include "TVT_PubCom.h"
#else
#include "PUB_common.h"
#endif//NAT_TVT_DVR_4_0

#include "SWL_Public.h"
#include "NatSocket.h"

/**
 * �����ӽ����Ժ󣬵��øú���
 * @param[in] sock �����ӵ��׽��֣�������������-1
 * @param[in] pObject �ص���ض���ָ��
 * @param[in] nParam ����������
 * @return ����ֵδ���塣
 */
typedef int (*NAT_CONNECT_LINK_CALLBACKEX)(NatSocket sock, void *pObject, void *pParam);

/**
 * �ͻ��˵Ĵ�͸���͵�����
 */
typedef struct _nat_client_traversal_config_
{
    char           dwDeviceNo[64];  /*< Ҫ���ӵ��豸�����к� */
    char           pServerIp[256];  /*< NAT������IP��ַ */
    unsigned short nServerPort;     /*< NAT�������˿ں� */
}NAT_CLIENT_TRAVERSAL_CONFIG;

/**
 * �ͻ��˵�ֱ�����͵�����
 */
typedef struct _nat_client_direct_config_
{
    unsigned long dwDeviceIp;            /*< Ҫ���ӵ��豸��IP��ַ */
    unsigned short nDevicePort;  /*< Ҫ���ӵ��豸�Ķ˿ں� */    
}NAT_CLIENT_DIRECT_CONFIG;

typedef enum
{
	TRAVERSAL_CONFIG,
	DIRECT_CONFIG,
	BOTH_CONFIG
} CONFIG_TYPE;

/**
* ��͸ģʽ
* Ӧ��ִ��P2P��͸��Ȼ���ִ��Relay��͸
*/
enum TraversalMode
{
	TRAVERSAL_P2P = 0,
	TRAVERSAL_RELAY
};

/**
 * �ͻ�������
 */
typedef struct _nat_client_config_
{
	CONFIG_TYPE ConfigType;
    NAT_CLIENT_TRAVERSAL_CONFIG TraversalConfig;
    NAT_CLIENT_DIRECT_CONFIG DirectConfig;    
}NAT_CLIENT_CONFIG;

class CNatUdtClientPeer; 

class CNatTraversalClientPeer;

/**
 * ���Ӵ������ӳɹ�����ʧ�ܺ���Ҫ�ͷŵ��Ķ���.
 */
class CNatConnectProc
{

public:
    /**
     * �½����Ӵ���
     * @param[in] config �ͻ������á�
     * @return ����ɹ����½��������ߣ����򷵻�NULL��
     */  
    static CNatConnectProc* NewConnectProc(const NAT_CLIENT_CONFIG *config);

	//�½����Ӵ���ֱ���ķ�ʽ
	static CNatConnectProc* NewConnectProc(const NAT_CLIENT_DIRECT_CONFIG *config);


	//��ǰ���ӵ�ģʽ 0����͸��1��ת��
	void SetConnectTraversalMode(TraversalMode mode);

    /**
     * ���ٺ�����
     * @return ����ɹ�������0�����򣬷�������ֵ��
     */  
    int Destroy();

	/**
	 * ��ͬ���ķ�ʽ��������һ��
	 * @param[in] timeOut ���ӳ�ʱʱ�䣬��λ������
	 * @return ������ط�NULLֵ����ֵ��ʾ�ɹ����ӵ�NatSocket�����򷵻�NULL��ʾʧ�ܡ�
	 */		
	NatSocket ConnectSyn(int timeOut);


	/**
	 * ��ͬ���ķ�ʽ��������һ��
	 * @param[in] timeOut ���ӳ�ʱʱ�䣬��λ������
	 * @param[in] cancel �Ƿ�ȡ�����ӹ���ָ�룬�ɵ����ߴ��롣����ΪNULL����ʾ������ȡ�����̣�
	 *                   ��ΪNULLʱ������ʱӦ��ʼ��Ϊfalse����������Ҫȡ�����ӹ���ʱ��ֻ�轫ָ����ָ������Ϊtrue���ɡ�
	 * @return ������ط�NULLֵ����ֵ��ʾ�ɹ����ӵ�NatSocket�����򷵻�NULL��ʾʧ�ܡ�
	 */
	NatSocket ConnectSyn(int timeOut, bool *cancel);

    /**
    * ��ʼ���첽��ʽִ�����ӡ�
     * @param[in] pCallback ���ӻص�������
     * @param[in] pObject ���������ָ�롣
     * @param[in] iTimeOut ���ӳ�ʱ��
     * @return ����ɹ�������0�����򣬷�������ֵ��
     */  
    int ConnectAsyn(NAT_CONNECT_LINK_CALLBACKEX pCallback, void* pObject, int iTimeOut);	

	/**
	 * ��ȡ���Ӵ���
	 */
	NAT_CLIENT_ERROR GetConnectError();
	/**
	 * ��ȡ���ӳ���ʱ��״̬
	 */
	NAT_CLIENT_STATE GetConnectState();
protected:
    CNatConnectProc(const NAT_CLIENT_CONFIG &config);
    virtual ~CNatConnectProc();
protected:
	void NotifyOnConnect(NatSocket sock, NAT_CLIENT_ERROR iErrorCode);
private:

	int CreateNatSocket(int iTimeOut);

    bool Init();
private:
    /**
     * ��ֹʹ��Ĭ�ϵĿ������캯��
     */
    CNatConnectProc(const CNatConnectProc&);
    /**
     * ��ֹʹ��Ĭ�ϵ�=�����
     */
    CNatConnectProc& operator=(const CNatConnectProc&);

	/**
	 * bug 
     * �첽���ûص�֮����Ҫ����clear�� ����ConnectAsynֻ�ܱ�����һ��
     */  
	int Clear();
private:
    class COnConnectNotifier;
    friend class COnConnectNotifier;
    COnConnectNotifier *m_onConnectNotifier;
private:
    enum ConnectState
    {
        STATE_NONE,
        STATE_CONNECTING,
		STATE_CONTINUE,
        STATE_FINISHED
    };
private:
    ConnectState m_state;
    bool m_isConnectSync;
    NatSocket m_syncSock;
	int								m_iTimeOut;					//�첽���ӵ�ʱ��ĳ�ʱʱ�䣬��Ϊ����Ϊ��λ

	CNatUdtClientPeer				*m_pClientPeer;				//ֱ��
	CNatTraversalClientPeer			*m_pNatTraversalClientPeer; //��͸ģʽ
	TraversalMode					m_connectTraversalMode;		//���ӵ�ģʽ

	NAT_CONNECT_LINK_CALLBACKEX		m_pConnectCallback;
	void							*m_pConnectCallbackParam;

	CPUB_Lock						m_ConnectSynLock; 
    NAT_CLIENT_CONFIG				m_config;
    NAT_CLIENT_ERROR				m_connectError;
	NAT_CLIENT_STATE				m_clientState;
};

#endif//_NAT_CONNECT_PROC_H_