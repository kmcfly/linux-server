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
     * ����Ĭ�Ϸ�������ַ��ͨ��Ϊ����
	 * @param[in] addr				��������ַ
     */
    void SetDefaultServerAddr(const char* addr);
    /**
     * ���÷������б���
	 * @param[in] pServerList �������б�
	 * @param[in] iCount ����������
	 * @param[in] iUpdatedCount ���ڸ��µķ������������ӵ�0����ʼһ��ʹ�ã�Ϊ0��ʾ���з���������ʹ��
								���������ʧ�ܣ���ʹ��Ĭ�ϵķ���������
     */
    void SetServerList(const NAT_SERVER_LIST_INFO* pServerList, int iCount, int iUpdatedCount = 0);
    /**
     * �����Ƿ����ò��٣�Ĭ��Ϊ false
     */
    void SetTestSpeed(bool bTestSpeed);

    /**
     * ��ʼִ�л�ȡ����
     * �ڲ������������߳�
     */
    bool Start();

    /**
     * �Ƿ������
     * ��ʼ״̬Ϊtrue����ִ��Start()��Ϊfalse���ڴ�����ɺ�Ϊtrue����ʱ�ڲ��̻߳��Զ��˳�
     */
    bool IsCompleted();

	/**
	 * �Ƿ�ɹ���ȡ�������б�
	 * ������ɹ������Կ��Ի�ȡ�õ�����֮ǰ���б�
	 */
	bool IsSucceeded();

    /**
     * ȡ��ִ�й��̣����ȴ�������ɺ󷵻�
     */
    void Cancel();
    
	/**
	 * ��ȡ����������
	 * Ӧ���� IsCompleted() �Ժ󣬲ſ��Ի�ȡ
	 */
	int GetServerCount();
    /**
     * ��ȡ�������б���
     * Ӧ���� IsCompleted() �Ժ󣬲ſ��Ի�ȡ
	 * @param[in] pServerInfo	��������Ϣ���ɵ����ߴ��룬�ڲ���д��Ϣ
	 * @param[in] index			������
	 * @return ����ɹ�������true�����򣬷���false
     */
    bool GetServerInfo(NAT_SERVER_LIST_INFO *pServerInfo, int index);

	/**
	 * ��ȡ�ѳɹ����ٵķ���������
	 * ��Start()�ɹ��󣬾Ϳ��Զ�ʱ��ѯ��ȡ
	 */
	int GetSpeededServerCount();

	/**
	 * ��ȡ�ѳɹ����ٵķ������б���
	 * ��Start()�ɹ��󣬾Ϳ��Զ�ʱ��ѯ��ȡ
	 * @param[in] pServerInfo	��������Ϣ���ɵ����ߴ��룬�ڲ���д��Ϣ
	 * @param[in] index			������
	 * @return ����ɹ�������true�����򣬷���false
	 */
	bool GetSpeededServerInfo(NAT_SERVER_LIST_INFO *pServerInfo, int index);

	/**
	 * �Ƿ����ò���
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
	//���ط������б�
	bool FetchListFromServ(NatServerVector &serverVector, const char *pServerAddr);
	//�����������б�
	bool ParseServList(NatServerVector &serverVector, const char* pXmlSrc, int nXmlType = 0);
	//������������Ϣ
	bool ParseServInfo(TiXmlElement* pXmlDoc, NAT_SERVER_LIST_INFO* pServInfo);
	//���
	void Clear();

	void ProcTestSpeed();
private:
	bool m_isCompleted;
	bool m_isSucceeded;
	int m_cacheUpdatedCount;
	//����������б�
	NatServerVector m_speedServerVector;
	//��ʱ�������б�
	NatServerVector m_cacheServerVector;
	// ���ط������б�
	NatServerVector m_downloadServerVector;
	// �����б�
	NatServerList   m_speedTestList;
	CTVTHttp *m_pHttp;
	CPUB_Lock m_lock;
	char m_szDefaultServerAddr[256];
    bool m_isTestSpeed;

    PUB_thread_t m_workThreadID;
    bool m_workThreadRunning;

};
#endif//__NAT_SERVER_LIST_FETCHER__