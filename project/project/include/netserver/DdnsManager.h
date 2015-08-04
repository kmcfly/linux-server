#ifndef _DDNS_MANAGER_H_
#define _DDNS_MANAGER_H_

#include "dvrdvsdef.h"
#include "PUB_common.h"
#include "DdnsClient.h"
#include "NetConfigDefine.h"
#include "NetInterface.h"
#include "NetProtocol.h"



#define MAXINTERFACES				5

#define CANNOTUSEDDNS				0x1						//次时不能使用DDNS来访问DVR，但是使用了DDNS功能
#define CANUSEDDNS					0x11					//此时可以使用DDNS来访问DVR

const int meibuTestHoldTime					= 60 * 5;			//测试meibu功能成功后的5分钟内的连续测试都默认成功
const unsigned long DEFAULT_SLEEP_COUNT		= 120;				//向服务器更新时默认休眠时间
const unsigned long SLEEP_LENGTH			= 10000;			//每次休眠的时间， 单位ms，每隔10s就检测外网IP，本地IP是否改变，如果改变就直接向服务器更新

class CDdnsManager
{
public:
	static CDdnsManager *Instance();
	
	bool Inital();
	void Quit();

	bool Start();
	void Stop();
	
	bool ChangeMintDDNSName(const unsigned char *pMintDDNSName);
	void ChangeDDNSClient(const DDNS_INFO &DdnsClientInfo);
	void SetDeviceName(const char *pDeviceName);
	void SetHttpPort(unsigned short httpPort);
	void SetServerPort(unsigned short serverPort);

	NET_CTRL_FAIL CheckDDNSClientInfo(DDNS_INFO_EX &DdnsClientInfo);
	void ChangeUpdateCycle(unsigned long updateCycle);

	bool DDNSAutoRegRemove();
	~CDdnsManager();

protected:
private:
	CDdnsManager();
	static	PUB_THREAD_RESULT PUB_THREAD_CALL IpUpdateThread(void *pParam);
	void IpUpdateProc();

	typedef struct _local_network_info
	{
		unsigned int LocalIP;
		unsigned int NetMask;
		unsigned int BroadAddr;
		unsigned int Gateway;
		unsigned int FirstDNS;
		unsigned int SecondDNS;

		char MACAddr[20];		
	}LOCOALNETWORKINFO;

	//获取当前设备的IP和MAC地址
	bool GetLocalNetworkInfo( LOCOALNETWORKINFO *pLocalNetworkInfo );

	
	DDNS_SERVER_TYPE	m_DdnsServerType;
	DDNS_SERVER_INFO	m_DdnsServerInfo;
	DDNS_INFO			m_DdnsClientInfo;
	CDdnsClient			*m_pDdnsClient;

	PUB_thread_t		m_hIpUpdateID;
	bool				m_bIpUpdateProc;
	CPUB_Lock			m_DdnsLock;

	bool				m_bUseDdns;								//DDNS的类型不为DDNS_NONE,并且使用了DDNS功能的时候该值为true

	LOCOALNETWORKINFO	m_LocalNetInfo;
	char				m_externalIP[20];

	unsigned long		m_lastCheckUpdateTime;					//记录上次测试成功的时间
	bool				m_bUpdateOK;							//测试成功
	DDNS_INFO_EX		m_CheckClientInfoEx;					//上次测试成功时的测试信息

	unsigned long		m_sleepCount;

	char				m_deviceName[64];
	unsigned short		m_httpPort;
	unsigned short      m_serverPort;
	unsigned char		m_mintDDNSServerName[132];
	bool				m_bLocalInfoChange;
};
#endif

