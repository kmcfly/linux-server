// NatServerListFetcher.h: interface for the CNatServerListFetcher class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __NAT_SERVER_LIST_FETCHER__
#define __NAT_SERVER_LIST_FETCHER__

#include "NatPublicTypes.h"
#include "tinyxml.h"
#include "TVTHttp.h"
#include <vector>

class CNatServerListFetcher
{
public:
	static const int TEST_SPEED_TIMEOUT = 10 * 1000; // ms
public:
	CNatServerListFetcher();
	virtual ~CNatServerListFetcher();
public:
    /**
     * 设置默认服务器地址，通常为域名
	 * @param[in] addr				服务器地址
     */
    void SetDefaultServerAddr(const char* addr);
    /**
     * 设置服务器列表缓存
	 * @param[in] pServerList 服务器列表
	 * @param[in] iCount 服务器数量
	 * @param[in] iUpdatedCount 用于更新的服务器数量，从第0个开始一次使用；为0表示所有服务器都可使用
								如果都更新失败，则使用默认的服务器更新
     */
    void SetServerList(const NAT_SERVER_LIST_INFO* pServerList, int iCount, int iUpdatedCount = 0);
    /**
     * 设置是否启用测速，默认为 false
     */
    void SetTestSpeed(bool bTestSpeed);

    /**
     * 开始执行获取过程
     * 内部会启动处理线程
     */
    bool Start();

    /**
     * 是否已完成
     * 初始状态为true；当执行Start()后，为false；在处理完成后，为true，此时内部线程会自动退出
     */
    bool IsCompleted();

	/**
	 * 是否成功获取服务器列表
	 * 如果不成功，则仍可以获取得到更新之前的列表
	 */
	bool IsSucceeded();

    /**
     * 取消执行过程，并等待真正完成后返回
     */
    void Cancel();
    
	/**
	 * 获取服务器数量
	 * 应满足 IsCompleted() 以后，才可以获取
	 */
	int GetServerCount();
    /**
     * 获取服务器列表项
     * 应满足 IsCompleted() 以后，才可以获取
	 * @param[in] pServerInfo	服务器信息，由调用者传入，内部填写信息
	 * @param[in] index			索引号
	 * @return 如果成功，返回true；否则，返回false
     */
    bool GetServerInfo(NAT_SERVER_LIST_INFO *pServerInfo, int index);

	/**
	 * 获取已成功测速的服务器数量
	 * 在Start()成功后，就可以定时轮询获取
	 */
	int GetSpeededServerCount();

	/**
	 * 获取已成功测速的服务器列表项
	 * 在Start()成功后，就可以定时轮询获取
	 * @param[in] pServerInfo	服务器信息，由调用者传入，内部填写信息
	 * @param[in] index			索引号
	 * @return 如果成功，返回true；否则，返回false
	 */
	bool GetSpeededServerInfo(NAT_SERVER_LIST_INFO *pServerInfo, int index);

	/**
	 * 是否启用测速
	 */
    bool IsTestSpeed();

private:

    static PUB_THREAD_RESULT PUB_THREAD_CALL WorkThreadFunc(void *pParam);
    int RunWork();
private:
	typedef struct __SERVER_SPEED_INFO__
	{
		NAT_SERVER_LIST_INFO serverInfo;
		unsigned long remoteIp;
		unsigned long lastSendTime;
		int sendCount;
		bool hasReceived;
	}SERVER_SPEED_INFO;

	typedef std::vector<NAT_SERVER_LIST_INFO> NatServerVector;
	typedef std::list<SERVER_SPEED_INFO> NatServerList;

	static const int RAND_BIND_TIMES_MAX = 10;
	static const int SEND_TEST_SPEED_COUNT = 3;
	static const int SEND_TEST_SPEED_INTERVAL = 1000; // ms
private:
	bool InitSpeedTestList(NatServerVector &serverVector);
	//下载服务器列表
	bool FetchListFromServ(NatServerVector &serverVector, const char *pServerAddr);
	//解析服务器列表
	bool ParseServList(NatServerVector &serverVector, const char* pXmlSrc, int nXmlType = 0);
	//解析服务器信息
	bool ParseServInfo(TiXmlElement* pXmlDoc, NAT_SERVER_LIST_INFO* pServInfo);
	//清空
	void Clear();

	void ProcTestSpeed();
private:
	bool m_isCompleted;
	bool m_isSucceeded;
	int m_cacheUpdatedCount;
	//结果服务器列表
	NatServerVector m_speedServerVector;
	//临时服务器列表
	NatServerVector m_cacheServerVector;
	// 下载服务器列表
	NatServerVector m_downloadServerVector;
	// 测速列表
	NatServerList   m_speedTestList;
	CTVTHttp *m_pHttp;
	CPUB_Lock m_lock;
	char m_szDefaultServerAddr[256];
    bool m_isTestSpeed;

    PUB_thread_t m_workThreadID;
    bool m_workThreadRunning;

};
#endif//__NAT_SERVER_LIST_FETCHER__