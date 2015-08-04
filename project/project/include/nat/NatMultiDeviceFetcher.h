// NatDeviceFetcher.h: interface for the CNatDeviceFetcher class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _NAT_MULTI_DEVICE_FETCHER_H_
#define _NAT_MULTI_DEVICE_FETCHER_H_

#include "NatUserDefine.h"

#ifdef NAT_TVT_DVR_4_0
#include "TVT_PubCom.h"
#else
#include "PUB_common.h"
#endif//NAT_TVT_DVR_4_0

#include "SWL_Public.h"
#include "NatSocket.h"
#include "NatPublicTypes.h"
#include "NatDeviceFetcher.h"
#include <vector>

/**
 * �豸��ע��ķ����������Ϣ
 */
typedef struct _nat_dev_reg_serv_info_
{
	int						iServerInfoCount;	/**< ��͸�����������棩���������Ϊ0�������pServerInfoList��iRegServerIndex */
	int						iRegServNum;		/**< pServerInfoList�ǰ��ɿ쵽�������ȼ�����ģ�ǰiRegServNum��server���豸ע�ᵽ��server */
	NAT_SERVER_LIST_INFO	*pServerInfoList;	/**< ��͸�������б�ָ�룬���ΪNULL����ʾ������ */
	NAT_SERVER_LIST_INFO	*pRegServerInfo;	/**< �豸��ע������ķ�������Ϣ */
}NAT_DEV_REG_SERV_INFO;

/**
 * ����ȡ�豸���ؽ���󣬵��øú���
 * @param[in] pDeviceInfo ��ȡ���豸��Ϣ��������������NULL
 * @param[in] pRegServerInfo �豸��ע��ķ����������Ϣ�����ΪNULL��������
							 ���е�pServerInfoList��pRegServerInfo�����ڲ�������ͷŵ�
 * @param[in] pObject �ص���ض���ָ��
 * @param[in] nParam ����������
 * @return ����ֵδ���塣
 */
typedef int (*NAT_MULTI_FETCH_DEVICE_CALLBACKEX)(const NAT_DEVICE_INFO *pDeviceInfo, NAT_DEV_REG_SERV_INFO *pRegServerInfo, bool state, void *pObject, void *pParam);

class CNatMultiDeviceFetcherWorker;

/**
 * �豸��Ϣ��ȡ��
 */
class CNatMultiDeviceFetcher
{
    friend class CNatMultiDeviceFetcherWorker;
public:
    /**
     * �½����Ӵ���
     * @param[in] config �ͻ������á�
     * @return ����ɹ����½��������ߣ����򷵻�NULL��
     */  
    static CNatMultiDeviceFetcher* NewDeviceFetcher(const NAT_DEVICE_FETCHER_CONFIG *config, NAT_DEV_REG_SERV_INFO *pRegServerInfo = NULL);

    /**
     * ���ٺ�����
     * @return ����ɹ�������0�����򣬷�������ֵ��
     */  
    int Destroy();

	/**
     * ��ͬ�����õķ�ʽ��ȡ�豸��Ϣ��
     * �������ظ����á�
     * @param[in] timeout ��ʱʱ�䣬��λ������
     * @param[in] pDeviceInfo ���ػ�ȡ���豸��Ϣ��ָ�룬���벻��ΪNULL��
	 * @param[in] pRegServerInfo �����豸��ע��ķ����������Ϣ������������سɹ�����ֵ��ΪNULL��
								 ��ָ�������ڲ�������ͷŵ�
     * @param[in] bWaitForAllServer �Ƿ�ȴ����з��������ؽ������λ������
     * @return �����ȡ�ɹ�������0�����򣬷�������ֵ
	 */
    int FetchSyn(int iTimeOut, NAT_DEVICE_INFO *pDeviceInfo, NAT_DEV_REG_SERV_INFO **pRegServerInfo, bool bWaitForAllServer = false);

    /**
     * ��ʼ
	 * ���첽��ʽ��ȡ�豸��Ϣ��
     * @param[in] iTimeOut ��ʱʱ�䣬��λ�����롣
     * @return ����ɹ�������0�����򣬷�������ֵ��
	 * !!!!!ʹ���첽��ʽ���ص�֮��һ��Ҫ���ø����stop�� ���߸���Ҫ�ĳ��̳߳�פ����ʽ
     */  
    int Start(int iTimeOut, bool bWaitForAllServer = false);

	/**
	 * ֹͣ
	 * ֹͣ�첽��ʽ��ȡ�豸��Ϣ
	 */
	void Stop();

	/**
	 * �Ƿ����
	 */
	bool IsCompleted();

	/**
	 * �Ƿ�ɹ�
	 * �� IsCompleted() Ϊtrueʱ����Ч
	 */
	bool IsSucceeded();

	/**
	 * ��ȡ�豸��Ϣ
	 * �� IsCompleted() Ϊtrueʱ����Ч
	 */
	NAT_DEVICE_INFO* GetDeviceInfo();

	/**
	 * ��ȡ�豸ע�����������Ϣ
	 * �� IsCompleted() Ϊtrueʱ����Ч
	 */
	NAT_DEV_REG_SERV_INFO* GetRegServerInfo();


    /**
     * ��ȡ�������
     * �����ȡ�豸��Ϣ����ʹ�øú������Ի��ʧ�ܵĴ������
     */
	NAT_CLIENT_ERROR GetError();

	typedef enum
	{
		DF_STATE_NONE,
		DF_STATE_FAILED,
		DF_STATE_COMPLETE,
		DF_STATE_GET_LIST,
		DF_STATE_GET_DEVICE,
		DF_STATE_CONTINUE_GET_DEVICE,
	}NatDeviceFetcherState;

protected:
    CNatMultiDeviceFetcher(const NAT_DEVICE_FETCHER_CONFIG *config);
    virtual ~CNatMultiDeviceFetcher();
    bool Init(NAT_DEV_REG_SERV_INFO *pRegServerInfo=NULL);
	void ChangeState(NatDeviceFetcherState state);
	NatDeviceFetcherState GetState();
	void SortRegServInfo();
private:
	NatDeviceFetcherState					m_state;
    CNatMultiDeviceFetcherWorker			*m_worker;
    NAT_CLIENT_ERROR						m_error;
    NAT_DEVICE_FETCHER_CONFIG				m_config;
    int										m_iTimeout;
	unsigned long							m_StartTime;
    CPUB_Lock								m_syncLock;
    NAT_DEVICE_INFO							m_deviceInfo;
	NAT_DEV_REG_SERV_INFO					m_devRegServInfo;
	std::vector<NAT_SERVER_LIST_INFO>		m_devRegServInfoSort;

	bool									m_bWaitForAllServer;
};


#endif//_NAT_DEVICE_FETCHER_H_