// NetServer.h: interface for the CNetServer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NETSERVER_H__C59538D3_3A21_4917_B62B_FA9DD19926DA__INCLUDED_)
#define AFX_NETSERVER_H__C59538D3_3A21_4917_B62B_FA9DD19926DA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SWL_ListenProcEx.h"
#include "SWL_MultiNetComm.h"
#include "NatMultiNetNat.h"
#include "MessageMan.h"
#include "AVDataSendMan.h"
#include "SWL_Public.h"
#include "NatListenProc.h"
#include "NatListenManager.h"

#ifdef   __ENVIRONMENT_LINUX__
#include "Multicast.h"
#endif

#include <map>
#include <utility>

class CFrameData;
class CFrameBuffer;
class CAVDataSendMan;

#if defined(__PACKID_14__) || defined(__PACKID_27__)
const long CHANNEL_BUFFER_SIZE = 512*1024;
const long SEND_BUFFER_SIZE = 512*1024;
#else
const long CHANNEL_BUFFER_SIZE = 256*1024;
const long SEND_BUFFER_SIZE = 256*1024;
#endif

const int DEVICE_NAME_MAX_LEN = 20;

#define     MSG_MULTIPLE_CHANGE_IP  3   //广播改变IP
#define		MSG_SET_MAC				4	//设置MAC地址
#define		SERVERJS				"./WebServer/server.js"	//liuhao 2010-0203 存放目录已经修改

//当客户端处传给外部的消息，调用此回调函数
//pObject	注册回调函数的时候传进来的参数
//iMsg		消息名
//pParam1	参数1
//pParam2	参数2
typedef int (*SERVER_MESSAGE_CALLBACK)(void* pObject, int iMsg, void *pParam1, void *pParam2);

typedef std::map<unsigned long, ULONGLONG>	STREAM_CH_MAP;
typedef std::pair<unsigned long, ULONGLONG> STREAM_CH_PAIR;

typedef struct _net_init_info
{
	unsigned char	dwMaxVideo;
	unsigned char	dwMaxAudio;
	unsigned char	dwMaxLink;
	unsigned char	dwDeviceType;

	unsigned short	nPort;
	unsigned short  nHttpPort;

	unsigned long	dwProductType;
	unsigned long	dwProductSubID;

	unsigned long	dwDevVer;
	unsigned long	dwSoftVer;
	unsigned long	dwBuildTime;

	char			deviceName [DEVICE_NAME_MAX_LEN];
	char			szMac[8];//只用了前6个字节
}NET_INIT_INFO;

typedef struct _client_service_info
{
	long				clientID;
	unsigned long		connectType;	//连接类型
	unsigned long		dwIP;			//连接的客户端的IP地址
	unsigned short		wPort;			//客户端远程机的端口号（主机字节序）
	unsigned char		bBlocked;		//该客户端网络被阻塞
	unsigned char		bValidateClient;
	unsigned long		bTalking;		//是否在对讲
	CAVDataSendMan		*pAVDataSendMan;
	ULONGLONG			authRViewBits;	//授权远程现场通道掩码（位为0表示不能查看网络数据）
	ULONGLONG			authPlayback;	//回放权限
	ULONGLONG			authBackup;		//备份权限
	unsigned long		dwLiveStreamID;

	//该客户端发送数据缓冲区
	unsigned char		*pSendBuffer;
	unsigned char		*pDataCusor;
	long				sendBufferSize;
	long				sendDataLen;
	
	ULONGLONG			ullMasterVideo;	//当前该客户端请求的主码流情况
	ULONGLONG			ullSecondVideo;	//
	ULONGLONG			ullAudio;
	STREAM_CH_MAP		*pStreamCHMap;
	ULONGLONG			ullJpegStream;

	//设备状态信息
	NET_REC_STATUS		netRecStatus;	//录像状态信息
	ULONGLONG			ullMotionState;//CHNN_STATUS_MOTION, 移动报警状态，状态位为1表示报警
	ULONGLONG			ullSensorInState;//CHNN_STATUS_SENSOR_IN，传感器报警状态，状态位为1表示报警
	ULONGLONG			ullVideoLoss;	//CHNN_STATUS_VIDEO_LOSS，视频丢失，状态位为1表示视频丢失
	ULONGLONG			ullNetLogin;	//网络网络用户登录

	// 请求现场或回放码流时的异常信息
	unsigned long		lastStreamError;
	list<unsigned long>	*pLastStreamErrorList;

	unsigned long		loginMark;
	CSWL_MultiNet		* pMultiNet;
}CLIENT_SERVICE_INFO;

class CNetServer  
{
public:
	static CNetServer* Instance();
	~CNetServer();
	//回调函数默认为空
	//初始化CNetServer对象，在每次使用CNetServer对象前要调用此函数
	int	Initial(const NET_INIT_INFO	*pNetInitInfo, CMessageMan *pMessageMan, long maxSupportFastVideo=1, bool bSupportDoubleStream = true);
	int AutoReportInitial(bool bSupport, const NET_AUTO_REPORT *pNetAutoReport);


	//
	void SetMaxFirstStreamSendNum(bool bLimitSendNum, unsigned char maxPlayBackChnnNum);

	void SetMaxIPCTransferChnnNum(unsigned long maxIPCTransferChnnNum);

	//改变监听端口
	int ChangeListenPort(short int nPort);

	//改变广播时发送的http端口
	int ChangeHttpPort(short int nPort);

	//改变允许的最大连接数
	int ChangeMaxLink(unsigned char dwLink);

	int ChangeAutoCms(NET_AUTO_REPORT cmsInfo);
	int ChangeNatServerInfo(const NAT_SERVER_INFO * pNetServerInfo);
	void GetNatServerInfo(NAT_SERVER_INFO * pNetServerInfo){* pNetServerInfo = m_natSeverInfo;}
#ifdef __SUPPORT_3G__
	void SetPBStatus(bool bStartup){m_bStartPlayback = bStartup;};
#endif

	//卸载CNetServer对象，当不再使用CNetServer对象的时候调用此函数
	void Quit();

	//NetServer启动，只有当NetServer启动了，其他的操作才能有效
	int Start();

	//NetServer停止
	int Stop();

    //启动NAT，
    int NatStart();

    //NAT模块是否开启
    int NatIsStarted();

     //停止NAT模块
    int NatStop();

	//清除缓存在发送模块中的所有数据
	void ClearData(unsigned long streamType);

	//清除缓存在发送模块中的回放数据
	void ClearPBData(long clientID, unsigned long streamID);

	unsigned long GetLastStreamError(long clientID);

	//得到指定通道请求数据的码率
	unsigned long GetStreamEncodedByteRate(long clientID, long channel, unsigned long frameType);

	//得到指定通道实际网络发送的码率
	unsigned long GetStreamSentByteRate(long clientID, long channel, unsigned long frameType);

	//重置统计零点
	void ResetStreamStatistics(long clientID = -1, long channel = ALL_CHANNELS);

	void EnableMasterStream(bool bEnable);

	//设置网络访问黑白名单
	void SetBanAllowType(BAN_ALLOW_TYPE type);
	void SetBanAllowList(const IP_SECTION *pIP, unsigned char num);

	unsigned long GetFirstStreamSendNum( );

	ULONGLONG GetFirstStreamCH();
private:
	//使用单件，所以让构造函数私有
	CNetServer();

	//禁止使用默认的拷贝构造函数和=运算符
	CNetServer(const CNetServer&);
	CNetServer& operator=(const CNetServer&);

	//当有客户端连上来，会调用此函数
	static int AcceptLinkCallback(CLIENT_SOCK_INFO *pClientSockInfo, void *pParam1, void *pParam2);
	int	OnGetAcceptLink(CLIENT_SOCK_INFO *pClientSockInfo, unsigned long sockType);

	//网络数据发送线程
	static PUB_THREAD_RESULT PUB_THREAD_CALL SendDataThread(void *pParam);
	int SendDataRun();

	//网络数据接收回调函数
	static int RecvDataCallback(long clientID, void* pParam, const char* pBuf, int iLen);
	int RecvDataProc(long clientID, const char* pBuf, int iLen);

	//检查发送状态信息
	int CheckSendStatusData(long clientID, ULONGLONG &ullOldStatus, ULONGLONG ullNewStatus);
	int CheckSendRecStatusData(long clientID, NET_REC_STATUS &oldRecStatus, NET_REC_STATUS &newRecStatus);

	int RewriteServerJS( unsigned short newStreamPort );

	void SendData(long clientID, void *pData, int dataLen);

	void DispatchTalkStream();
	void DispatchLiveStream(unsigned long stream, BITMAPINFOHEADER *pBmpInfo, WAVEFORMATEX *pWaveInfo);
	void DispatchPBStream();
	void DispatchJpegStream();

	void RemoveClient(CLIENT_SERVICE_INFO *pClient);

	//组播线程
#ifdef   __ENVIRONMENT_LINUX__
	static PUB_THREAD_RESULT PUB_THREAD_CALL MulticastThread(void *pParam);
	int MulticastProc();
#endif

	//网络数据发送线程
	static PUB_THREAD_RESULT PUB_THREAD_CALL ConectCmsThread(void *pParam);
	int ConectCmsRun();

	bool HasAnySecondStreamOfOther(CLIENT_SERVICE_INFO *pClient);

private:
	CSWL_ListenProcEx		*m_pListenProc;
	CNatListenManager		*m_pNatListenManager;
	CSWL_MultiNetComm		m_multiNetComm;
	CSWL_MultiNetNat		m_multiNetNat;
    bool                  m_NatIsStarted;

	unsigned long			m_dwMaxLink;		//允许的客户端最大登录数

	PUB_lock_t				m_StartLock;		//在开始和停止的时候，要与登录和处理某个断开的客户端互斥

 	SERVER_MESSAGE_CALLBACK	m_pMsgCallback;		//消息处理
 	void*					m_pCallbackObject;	//调用回调函数时的参数

	PUB_thread_t			m_sendDataID;
	bool					m_bSendDataRun;

	NET_INIT_INFO			m_netInitInfo;

	CMyList<CLIENT_SERVICE_INFO *> m_lstClientServiceInfo;

	CMessageMan				*m_pMessageMan;
	MESSAGE_DATA			m_messageData;

	unsigned char			*m_pStatusData;
	int						m_statusDataLen;

	unsigned char			*m_pRecStatusData;
	int						m_recStatusDataLen;
	
	BITMAPINFOHEADER		*m_pBitmapInfo_F;	//快码流格式
	BITMAPINFOHEADER		*m_pBitmapInfo_S;	//慢码流格式
	WAVEFORMATEX			*m_pWaveFormatEx;

	//对每个通道主码流的限制
	long					m_maxSupportFastVideo;
	unsigned char			*m_pFastVideoNum;

	//下面的这个功能，网络模块最初在设计时是没有这个功能，只是有对每个通道发送主码流的份数做了限制（即：每个通道总共可以发送几个主码流）。
	//下面的功能是在做2516FD时加的。下面的功能是：对整个DVR发送主码流的份数做限制。
	//这里的主码流包括：现场主码流，远程回放码流，远程备份码流
	bool                    m_bLimitFirstStreamSendNum;  //是否限制发送主码流的份数。通过这个变量可以启动和关闭这个功能
	int                     m_maxFirstStreamSendNum;
	int                     m_curFirstStreamSendNum;

	//数字通道向客户端的最大转发路数
	unsigned long			m_maxIPCTransferChnnNum;
	unsigned long			m_curIPCTransferChnnNum;

	//是否启用主码流传输网络
	bool					m_bEnableMasterStream;

	//用来控制每次分发视频数据长度及时间
	long					*m_pDispatchFrameNum;
	LONGLONG				*m_pFirstDispatchFrameTime;

	//对讲格式
	WAVEFORMATEX			m_talkAudioFormat;

	//组播线程相关
#ifdef   __ENVIRONMENT_LINUX__
	PUB_thread_t			m_multicastID;
	bool					m_bmulticastRun;
	CMulticast				m_multicastEth0;
	CMulticast				m_multicastWifi;
#endif

	//是否支持双码流
	bool                    m_bSupportDoubleStream;
	
	//黑白名单
	BAN_ALLOW_TYPE			m_ban_allow_type;
	std::list<IP_SECTION>	m_ban_allow_list;


	NET_AUTO_REPORT			m_netAutoReport;
	PUB_thread_t			m_autoReportThreadID;
	bool					m_bAutoReportRun;
	bool                    m_bConectOk;
	bool                    m_bSupportAutoReport;
	CPUB_Lock               m_netAutoLock;
	
	NAT_LISTEN_MANAGER_CONFIG	m_natDeviceCfg;
	NAT_SERVER_INFO			m_natSeverInfo;
#ifdef __SUPPORT_3G__
	bool					m_bStartPlayback;
#endif
};

#endif // !defined(AFX_NETSERVER_H__C59538D3_3A21_4917_B62B_FA9DD19926DA__INCLUDED_)
