// NatListenProc.h: interface for the CNatListenProc class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _NAT_LISTEN_PROC_H_
#define _NAT_LISTEN_PROC_H_

#include "NatUserDefine.h"

#ifdef NAT_TVT_DVR_4_0
#include "TVT_PubCom.h"
#else
#include "PUB_common.h"
#endif//NAT_TVT_DVR_4_0

#include "SWL_Public.h"
#include <string.h>
#include <list>
#include "NatSocket.h"
#include "NatPublicTypes.h"

/**
 * �豸����
 * ��׼��
 */
typedef struct _nat_device_config_
{
	int						serverCount;		/**< NAT���������� */
	NAT_SERVER_LIST_INFO	*serverList;		/**< NAT�������б� */
	char					deviceNo[64];		/**< �豸�����к� */
	unsigned short			localPort;			/**< ���ذ󶨵Ķ˿ڣ���͸�� */
	unsigned long			localIp;			/**< ���ذ󶨵�IP��ַ����͸�� */
	unsigned char			refuseRelay;		/**< �豸�Ƿ�ܾ��м̷���Ϊ0��ʾ���ܾ���Ϊ��0��ʾ�ܾ���Ĭ��Ϊ0 */
	unsigned char			ucNatType;			/**< �豸���������NAT���ͣ�Ϊ0��ʾδ֪���ͣ�Ĭ��Ϊ0 */
	unsigned short			usUpnpPort;			/**< �豸����UDT�˿ڵ�UPNPӳ��˿ڣ�Ϊ0��ʾ��֧��UPNP��Ĭ��Ϊ0 */
	
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

}NAT_DEVICE_CONFIG;


/**
 * �豸�ķ�����״̬
 */
enum NAT_DEVICE_SERVER_STATE
{
	NAT_STATE_NONE,             /*��ʼ̬*/
    NAT_STATE_CONNECTING,       /**< �������ӷ����� */
    NAT_STATE_CONNECTED,        /**< �����ӷ����� */
    NAT_STATE_DISCONNECTED      /**< ���ӷ�����ʧ�� */
};

/**
 * �豸�ķ���������
 */
enum NAT_DEVICE_SERVER_ERROR
{
    NAT_ERR_OK,                /**< �޴���  */  
    NAT_ERR_UNKNOWN,           /**< δ֪���� */ 
    NAT_ERR_CONNECT_FAILED,    /**< ���ӷ�����ʧ�� */ 
    NAT_ERR_REGISTER_FAILED,   /**< �������ע��ʧ�� */ 
};

/**
 * �������ӻص�����
 * ���пͻ������Ӻ�accept���Ժ���øú���
 * @param[in] pClientSockInfo �ͻ��˵���Ϣ
 * @param[in] pParam1 δ����
 * @param[in] pParam2 δ����
 * @return ����ֵδ����
 */
typedef int (*NAT_ACCEPT_LINK_CALLBACK)(CLIENT_SOCK_INFO * pClientSockInfo, void *pParam1, void *pParam2);

class  CNatDevicePeer;

class CNatListenProc 
{
public:
    /**
     * ���캯��
     * pCallback����Ҫ�����췵�أ���Ϊ����Ҫ����listen
     * ���pCallback��û�а����ɵ�socket�ӹ�����ᵼ����Դй¶
     * @param[in] pCallback �������ӻص�����
     * @param[in] pParam ���������
     */    
    CNatListenProc(NAT_ACCEPT_LINK_CALLBACK pCallback, void* pParam);
    ~CNatListenProc();

    /**
     * ��ʼ�����˿ڣ�����Ƿ����������
     * @param[in] pConfig �豸����
     * @return ����ɹ�������0�����򣬷�������ֵ��
     */
    int StartListen(const NAT_DEVICE_CONFIG* pConfig);

    /**
     * ֹͣ�����˿�
     * @return ����ɹ�������0�����򣬷�������ֵ��
     */
    int StopListen();

    /**
     * �Ƿ��ѿ�ʼ����
     */
    bool IsStarted();

    /**
     * ���������
     * ֻ���������������ͨ�ţ���Ӱ�����еĿͻ��˴�͸����
     * @param[in] szServerIp     ����IP��ַ
     * @param[in] usServerPort   �������˿ں�
	 */
	void ResetServer(const char *serverIp, unsigned short serverPort);

	void ResetServer(const NAT_SERVER_LIST_INFO *serverInfo, int index);

	void SetUpnpPort(unsigned short natUpnpPort);

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
     * ��ȡ�豸�ķ�����״̬
     */
    NAT_DEVICE_SERVER_STATE GetServerState(int index = 0);

    /**
     * ��ȡ�豸�ķ���������
     */
    NAT_DEVICE_SERVER_ERROR GetServerError(int index = 0);

	/**
	 * ��ȡ�豸�Ĺ���IP
	 * ���� GetServerState() == NAT_STATE_CONNECTED ʱ��Ч
	 */
	unsigned long GetPublicIp(int index = 0);

	/**
	 * ��ȡ�豸�Ĺ����˿�
	 * ���� GetServerState() == NAT_STATE_CONNECTED ʱ��Ч
	 */
	unsigned short GetPublicPort(int index = 0);
protected:
	void OnConnect(NatSocket sock, unsigned long iIP, unsigned short nPort);
private:
    void Clear();
private:
    class COnConnectNotifier;
    friend class COnConnectNotifier;
    COnConnectNotifier *m_onConnectNotifier;
private:
    /**
     * ��ֹʹ��Ĭ�ϵĿ������캯��
     */
    CNatListenProc(const CNatListenProc&);
    /**
     * ��ֹʹ��Ĭ�ϵ�=�����
     */
    CNatListenProc& operator=(const CNatListenProc&);

#ifndef __NAT_DEVICE_CONFIG_EX__   
private:
	struct DEV_EXTRA_INFO;
	void UpdateExtraInfo();
	DEV_EXTRA_INFO		*m_extraInfo;
#endif//!__NAT_DEVICE_CONFIG_EX__
private:
	bool				m_isStarted;     	//�Ƿ��ѿ�ʼ����
	NAT_ACCEPT_LINK_CALLBACK m_pAcceptCallback;    //�пͻ���connect������accept����ô˻ص�����
	void				*m_pParam;             //�ص�������
	PUB_lock_t			m_listenLock;      	//��ʼ������ֹͣ��������
	CLIENT_SOCK_INFO	m_clientSockInfo;
	CNatDevicePeer      *m_pNatDevicePeer;   //Nat�豸�������
};

#endif//_NAT_LISTEN_PROC_H_
