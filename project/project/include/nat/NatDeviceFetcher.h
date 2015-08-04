// NatDeviceFetcher.h: interface for the CNatDeviceFetcher class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _NAT_DEVICE_FETCHER_H_
#define _NAT_DEVICE_FETCHER_H_

#include "NatUserDefine.h"

#ifdef NAT_TVT_DVR_4_0
#include "TVT_PubCom.h"
#else
#include "PUB_common.h"
#endif//NAT_TVT_DVR_4_0

#include "SWL_Public.h"
#include "NatSocket.h"
#include "NatPublicTypes.h"

/**
 * �豸��Ϣ���ɿͻ��˴�NAT�������ϻ�ȡ������
 */
typedef struct _nat_device_info
{
    unsigned long   dwDevicePeerIp;         /**< DevicePeer��IP��ַ */
    unsigned short  usDevicePeerPort;       /**< DevicePeer�Ķ˿ں� */
	unsigned char   ucCanRelay;             /**< �豸�Ƿ����ʹ���м����� */
	unsigned char   ucNatType;              /**< �豸���������NAT���ͣ�Ϊ0��ʾδ֪���ͣ�����Ϊ�գ���ʾ0��Ĭ��Ϊ0 */
	unsigned short  usUpnpPort;	            /**< �豸����UDT�˿ڵ�UPNPӳ��˿ڣ�Ϊ0��ʾ��֧��UPNP������Ϊ�գ���ʾ0��Ĭ��Ϊ0 */

	//////////////////////////
	// ��������չ��Ϣ��ExtraInfo��

#ifdef __NAT_DEVICE_CONFIG_EX__
	// ��չ��
	char			caExtraInfo[512];		/**< ��չ��Ϣ���ڲ�������ֻ�����豸�Ϳͻ����Ͻ������Դ�͸��������͸���� */
#else
	// ��׼��
    unsigned long   dwDeviceType;           /**< �豸���ͣ��ȼ���ʵ���豸�еĲ�Ʒ�ͺ� */
    char            caDeviceVersion[128];   /**< �豸�汾�ţ���"-"Ϊ�ָ����� ��"1.0.0-XXX-XX"
                                                 ���У���1����Ϊ��Ҫ�汾�� */
    unsigned short  usDeviceWebPort;        /**< �豸����WEB����˿ں� */
    unsigned short  usDeviceDataPort;       /**< �豸����WEB���ݶ˿ں� */
#endif//__NAT_DEVICE_CONFIG_EX__
}NAT_DEVICE_INFO;

/**
 * �豸�汾��Ϣ
 */
typedef struct _nat_device_ver_info_
{
    NAT_DEVICE_VER_TYPE verType;    /**< �豸�汾���� */
    // �����Ҫ�����Ը��ݰ汾�����ں��������ֶ�
}NAT_DEVICE_VER_INFO;

/**
 * �����豸�汾��Ϣ
 * @param[in] pVerStr �汾��Ϣ�ı���
 * @param[in] pVerInfo �豸�汾��Ϣ
 */
void NAT_ParseDeviceVer(const char* pVerStr, NAT_DEVICE_VER_INFO* pVerInfo);


/**
 * ����ȡ�豸���ؽ���󣬵��øú���
 * @param[in] pDeviceInfo ��ȡ���豸��Ϣ��������������NULL
 * @param[in] pObject �ص���ض���ָ��
 * @param[in] nParam ����������
 * @return ����ֵδ���塣
 */
typedef int (*NAT_FETCH_DEVICE_CALLBACKEX)(const NAT_DEVICE_INFO *pDeviceInfo, void *pObject, void *pParam);


/**
 * Device Fetcher ������
 */
typedef struct _nat_device_fetcher_config_
{
    char           dwDeviceNo[64];  /*< Ҫ���ӵ��豸�����к� */
    char           pServerIp[256];  /*< NAT������IP��ַ */
    unsigned short nServerPort;     /*< NAT�������˿ں� */
}NAT_DEVICE_FETCHER_CONFIG;


class CNatDeviceFetcherWorker;

/**
 * �豸��Ϣ��ȡ��
 */
class CNatDeviceFetcher
{
    friend class CNatDeviceFetcherWorker;
public:
    /**
     * �½����Ӵ���
     * @param[in] config �ͻ������á�
     * @return ����ɹ����½��������ߣ����򷵻�NULL��
     */  
    static CNatDeviceFetcher* NewDeviceFetcher(const NAT_DEVICE_FETCHER_CONFIG *config);

    /**
     * ���ٺ�����
     * @return ����ɹ�������0�����򣬷�������ֵ��
     */  
    int Destroy();

    // ��ͬ�����õķ�ʽ��ȡ�豸��Ϣ��
    // �������ظ����á�
    // @param[in] timeout ��ʱʱ�䣬��λ������
    // @param[in] pDeviceInfo ���ػ�ȡ���豸��Ϣ��ָ�룬���벻��ΪNULL��
    // @return �����ȡ�ɹ�������0�����򣬷�������ֵ
    int FetchSyn(int iTimeOut, NAT_DEVICE_INFO *pDeviceInfo);

    /**
     * ��ʼ���첽��ʽ��ȡ�豸��Ϣ��
     * ���صĽ����pCallback�����ص����������ظ����á�
     * @param[in] pCallback ����ص�������
     * @param[in] pObject ���������ָ�롣
     * @param[in] iTimeOut ��ʱʱ�䣬��λ�����롣
     * @return ����ɹ�������0�����򣬷�������ֵ��
     */  
    int FetchAsyn(NAT_FETCH_DEVICE_CALLBACKEX pCallback, void* pObject, int iTimeOut);	

    /**
     * ��ȡ�������
     * �����ȡ�豸��Ϣ����ʹ�øú������Ի��ʧ�ܵĴ������
     */
	NAT_CLIENT_ERROR GetError();
protected:
    CNatDeviceFetcher(const NAT_DEVICE_FETCHER_CONFIG *config);
    virtual ~CNatDeviceFetcher();
    bool Init();
    void NotifyCallback(NAT_DEVICE_INFO *pDeviceInfo);
private:
    CNatDeviceFetcherWorker *m_worker;
    NAT_CLIENT_ERROR m_error;
    NAT_DEVICE_FETCHER_CONFIG m_config;
    int m_iTimeout;
    bool m_isSync;
    bool m_syncCompleted;
    CPUB_Lock m_syncLock;
    NAT_DEVICE_INFO m_deviceInfo;

    NAT_FETCH_DEVICE_CALLBACKEX m_pCallback;
    void* m_pObject;
};


const static char* NAT_DEVICE_VER_PREFIX_3_0		= "3.";
const static char* NAT_DEVICE_VER_PREFIX_4_0		= "4.";
const static char* NAT_DEVICE_VER_PREFIX_IPC		= "i.";
const static char* NAT_DEVICE_VER_PREFIX_NVMS_9000	= "n.";

#endif//_NAT_DEVICE_FETCHER_H_