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
 * 设备所注册的服务器相关信息
 */
typedef struct _nat_dev_reg_serv_info_
{
	int						iServerInfoCount;	/**< 穿透服务器（缓存）个数，如果为0，则忽略pServerInfoList和iRegServerIndex */
	int						iRegServNum;		/**< pServerInfoList是按由快到慢的优先级排序的，前iRegServNum个server是设备注册到的server */
	NAT_SERVER_LIST_INFO	*pServerInfoList;	/**< 穿透服务器列表指针，如果为NULL，表示无数据 */
	NAT_SERVER_LIST_INFO	*pRegServerInfo;	/**< 设备所注册的最快的服务器信息 */
}NAT_DEV_REG_SERV_INFO;

/**
 * 当获取设备返回结果后，调用该函数
 * @param[in] pDeviceInfo 获取的设备信息，如果出错，则等于NULL
 * @param[in] pRegServerInfo 设备所注册的服务器相关信息；如果为NULL，不处理；
							 其中的pServerInfoList和pRegServerInfo是由内部分配和释放的
 * @param[in] pObject 回调相关对象指针
 * @param[in] nParam 保留参数。
 * @return 返回值未定义。
 */
typedef int (*NAT_MULTI_FETCH_DEVICE_CALLBACKEX)(const NAT_DEVICE_INFO *pDeviceInfo, NAT_DEV_REG_SERV_INFO *pRegServerInfo, bool state, void *pObject, void *pParam);

class CNatMultiDeviceFetcherWorker;

/**
 * 设备信息获取者
 */
class CNatMultiDeviceFetcher
{
    friend class CNatMultiDeviceFetcherWorker;
public:
    /**
     * 新建连接处理。
     * @param[in] config 客户端配置。
     * @return 如果成功，新建的连接者；否则返回NULL。
     */  
    static CNatMultiDeviceFetcher* NewDeviceFetcher(const NAT_DEVICE_FETCHER_CONFIG *config, NAT_DEV_REG_SERV_INFO *pRegServerInfo = NULL);

    /**
     * 销毁函数。
     * @return 如果成功，返回0；否则，返回其它值。
     */  
    int Destroy();

	/**
     * 以同步调用的方式获取设备信息；
     * 不允许重复调用。
     * @param[in] timeout 超时时间，单位：毫秒
     * @param[in] pDeviceInfo 返回获取的设备信息的指针，传入不能为NULL。
	 * @param[in] pRegServerInfo 返回设备所注册的服务器相关信息；如果函数返回成功，该值不为NULL；
								 该指针是由内部分配和释放的
     * @param[in] bWaitForAllServer 是否等待所有服务器返回结果，单位：毫秒
     * @return 如果获取成功，返回0；否则，返回其它值
	 */
    int FetchSyn(int iTimeOut, NAT_DEVICE_INFO *pDeviceInfo, NAT_DEV_REG_SERV_INFO **pRegServerInfo, bool bWaitForAllServer = false);

    /**
     * 开始
	 * 以异步方式获取设备信息。
     * @param[in] iTimeOut 超时时间，单位：毫秒。
     * @return 如果成功，返回0；否则，返回其它值。
	 * !!!!!使用异步方式，回调之后一定要调用该类的stop， 或者该类要改成线程常驻的形式
     */  
    int Start(int iTimeOut, bool bWaitForAllServer = false);

	/**
	 * 停止
	 * 停止异步方式获取设备信息
	 */
	void Stop();

	/**
	 * 是否完成
	 */
	bool IsCompleted();

	/**
	 * 是否成功
	 * 在 IsCompleted() 为true时才有效
	 */
	bool IsSucceeded();

	/**
	 * 获取设备信息
	 * 在 IsCompleted() 为true时才有效
	 */
	NAT_DEVICE_INFO* GetDeviceInfo();

	/**
	 * 获取设备注册服务器的信息
	 * 在 IsCompleted() 为true时才有效
	 */
	NAT_DEV_REG_SERV_INFO* GetRegServerInfo();


    /**
     * 获取错误代码
     * 如果获取设备信息出错，使用该函数可以获得失败的错误代码
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