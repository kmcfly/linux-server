
#ifndef _NETCFGANDUPMAN_H_ 
#define _NETCFGANDUPMAN_H_ 
#include "SWL_ListenProcEx.h"
#include "SWL_MultiNetComm.h"
#include "MessageMan.h"


#ifdef   __ENVIRONMENT_LINUX__
#include "HxMulticast.h"
#endif


#include <map>
#include <utility>


const long CFGUP_CHANNEL_BUFFER_SIZE = 64*1024;
const long CFGUP_SEND_BUFFER_SIZE = 64*1024;
const int  CFGUP_DEVICE_NAME_MAX_LEN = 20;


typedef struct _net_cfgup_info
{
	unsigned char	dwDeviceType;

	unsigned short	nPort;
	unsigned short  nHttpPort;

	unsigned long	dwProductType;
	unsigned long	dwProductSubID;

	unsigned long	dwDevVer;
	unsigned long	dwSoftVer;
	unsigned long	dwBuildTime;

	char			deviceName [CFGUP_DEVICE_NAME_MAX_LEN];
	char			szMac[8];//只用了前6个字节
}NET_CFGUP_INIT_INFO;

typedef struct _cfgup_client_service_info
{
	long				clientID;
	unsigned long		dwIP;			//连接的客户端的IP地址
	unsigned short		wPort;			//客户端远程机的端口号（主机字节序）
	unsigned char		bBlocked;		//该客户端网络被阻塞
	unsigned char		bValidateClient;
	
	//该客户端发送数据缓冲区
	unsigned char		*pSendBuffer;
	unsigned char		*pDataCusor;
	long				sendBufferSize;
	long				sendDataLen;
	
	// 请求现场或回放码流时的异常信息
	unsigned long		lastStreamError;
}CFGUP_CLIENT_SERVICE_INFO;

class CCfgUpNetServer  
{
public:
	static CCfgUpNetServer* Instance();
	~CCfgUpNetServer();

	//
	int	Initial(const NET_CFGUP_INIT_INFO	*pNetInitInfo, CMessageMan *pMessageMan);

	//
	void Quit();

	//
	int Start();

	//
	int Stop();

	//
	unsigned long CfgUpGetLastStreamError(long clientID);
	
private:
	//使用单件，所以让构造函数私有
	CCfgUpNetServer();

	//禁止使用默认的拷贝构造函数和=运算符
	CCfgUpNetServer(const CCfgUpNetServer&);
	CCfgUpNetServer& operator=(const CCfgUpNetServer&);

	//当有客户端连上来，会调用此函数
	static int CfgUpAcceptLinkCallback(CLIENT_SOCK_INFO *pClientSockInfo, void *pParam1, void *pParam2);
	int	OnCfgUpGetAcceptLink(CLIENT_SOCK_INFO *pClientSockInfo);

	//网络数据发送线程
	static PUB_THREAD_RESULT PUB_THREAD_CALL CfgUpSendDataThread(void *pParam);
	int CfgUpSendDataRun();

	//网络数据接收回调函数
	static int CfgUpRecvDataCallback(long clientID, void* pParam, const char* pBuf, int iLen);
	int CfgUpRecvDataProc(long clientID, const char* pBuf, int iLen);

	void CfgUpSendData(long clientID, void *pData, int dataLen);

	void CfgUpRemoveClient(CFGUP_CLIENT_SERVICE_INFO *pClient);

	//组播线程
#ifdef   __ENVIRONMENT_LINUX__
	static PUB_THREAD_RESULT PUB_THREAD_CALL MulticastThread(void *pParam);
	int MulticastProc();
#endif


private:
	CSWL_ListenProcEx		*m_pCfgUpListenProc;
	CSWL_MultiNetComm		m_cfgUpmultiNetComm;


	PUB_lock_t				m_cfgUpStartLock;		//在开始和停止的时候，要与登录和处理某个断开的客户端互斥


	PUB_thread_t			m_cfgUpsendDataID;
	bool					m_bcfgUpSendDataRun;
	bool					m_bUpgrading;

	NET_CFGUP_INIT_INFO		m_cfgUpNetInitInfo;

	CMyList<CFGUP_CLIENT_SERVICE_INFO *> m_lstcfgUpClientServiceInfo;

	CMessageMan				*m_pMessageMan;
	MESSAGE_DATA			m_messageData;

	//组播线程相关
#ifdef   __ENVIRONMENT_LINUX__
	PUB_thread_t			m_multicastID;
	bool					m_bmulticastRun;
	CMulticast				m_multicastEth0;
	CMulticast				m_multicastWifi;
	unsigned char			m_szMac[6];
#endif

};

#endif 

