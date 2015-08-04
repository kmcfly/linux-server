#ifndef _DDNS_MANAGER_H_
#define _DDNS_MANAGER_H_

#include "dvrdvsdef.h"
#include "PUB_common.h"
#include "DdnsClient.h"
#include "NetConfigDefine.h"
#include "NetInterface.h"
#include "NetProtocol.h"



#define MAXINTERFACES				5

#define CANNOTUSEDDNS				0x1						//��ʱ����ʹ��DDNS������DVR������ʹ����DDNS����
#define CANUSEDDNS					0x11					//��ʱ����ʹ��DDNS������DVR

const int meibuTestHoldTime					= 60 * 5;			//����meibu���ܳɹ����5�����ڵ��������Զ�Ĭ�ϳɹ�
const unsigned long DEFAULT_SLEEP_COUNT		= 120;				//�����������ʱĬ������ʱ��
const unsigned long SLEEP_LENGTH			= 10000;			//ÿ�����ߵ�ʱ�䣬 ��λms��ÿ��10s�ͼ������IP������IP�Ƿ�ı䣬����ı��ֱ�������������

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

	//��ȡ��ǰ�豸��IP��MAC��ַ
	bool GetLocalNetworkInfo( LOCOALNETWORKINFO *pLocalNetworkInfo );

	
	DDNS_SERVER_TYPE	m_DdnsServerType;
	DDNS_SERVER_INFO	m_DdnsServerInfo;
	DDNS_INFO			m_DdnsClientInfo;
	CDdnsClient			*m_pDdnsClient;

	PUB_thread_t		m_hIpUpdateID;
	bool				m_bIpUpdateProc;
	CPUB_Lock			m_DdnsLock;

	bool				m_bUseDdns;								//DDNS�����Ͳ�ΪDDNS_NONE,����ʹ����DDNS���ܵ�ʱ���ֵΪtrue

	LOCOALNETWORKINFO	m_LocalNetInfo;
	char				m_externalIP[20];

	unsigned long		m_lastCheckUpdateTime;					//��¼�ϴβ��Գɹ���ʱ��
	bool				m_bUpdateOK;							//���Գɹ�
	DDNS_INFO_EX		m_CheckClientInfoEx;					//�ϴβ��Գɹ�ʱ�Ĳ�����Ϣ

	unsigned long		m_sleepCount;

	char				m_deviceName[64];
	unsigned short		m_httpPort;
	unsigned short      m_serverPort;
	unsigned char		m_mintDDNSServerName[132];
	bool				m_bLocalInfoChange;
};
#endif

