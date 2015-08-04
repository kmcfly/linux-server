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

#define     MSG_MULTIPLE_CHANGE_IP  3   //�㲥�ı�IP
#define		MSG_SET_MAC				4	//����MAC��ַ
#define		SERVERJS				"./WebServer/server.js"	//liuhao 2010-0203 ���Ŀ¼�Ѿ��޸�

//���ͻ��˴������ⲿ����Ϣ�����ô˻ص�����
//pObject	ע��ص�������ʱ�򴫽����Ĳ���
//iMsg		��Ϣ��
//pParam1	����1
//pParam2	����2
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
	char			szMac[8];//ֻ����ǰ6���ֽ�
}NET_INIT_INFO;

typedef struct _client_service_info
{
	long				clientID;
	unsigned long		connectType;	//��������
	unsigned long		dwIP;			//���ӵĿͻ��˵�IP��ַ
	unsigned short		wPort;			//�ͻ���Զ�̻��Ķ˿ںţ������ֽ���
	unsigned char		bBlocked;		//�ÿͻ������类����
	unsigned char		bValidateClient;
	unsigned long		bTalking;		//�Ƿ��ڶԽ�
	CAVDataSendMan		*pAVDataSendMan;
	ULONGLONG			authRViewBits;	//��ȨԶ���ֳ�ͨ�����루λΪ0��ʾ���ܲ鿴�������ݣ�
	ULONGLONG			authPlayback;	//�ط�Ȩ��
	ULONGLONG			authBackup;		//����Ȩ��
	unsigned long		dwLiveStreamID;

	//�ÿͻ��˷������ݻ�����
	unsigned char		*pSendBuffer;
	unsigned char		*pDataCusor;
	long				sendBufferSize;
	long				sendDataLen;
	
	ULONGLONG			ullMasterVideo;	//��ǰ�ÿͻ�����������������
	ULONGLONG			ullSecondVideo;	//
	ULONGLONG			ullAudio;
	STREAM_CH_MAP		*pStreamCHMap;
	ULONGLONG			ullJpegStream;

	//�豸״̬��Ϣ
	NET_REC_STATUS		netRecStatus;	//¼��״̬��Ϣ
	ULONGLONG			ullMotionState;//CHNN_STATUS_MOTION, �ƶ�����״̬��״̬λΪ1��ʾ����
	ULONGLONG			ullSensorInState;//CHNN_STATUS_SENSOR_IN������������״̬��״̬λΪ1��ʾ����
	ULONGLONG			ullVideoLoss;	//CHNN_STATUS_VIDEO_LOSS����Ƶ��ʧ��״̬λΪ1��ʾ��Ƶ��ʧ
	ULONGLONG			ullNetLogin;	//���������û���¼

	// �����ֳ���ط�����ʱ���쳣��Ϣ
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
	//�ص�����Ĭ��Ϊ��
	//��ʼ��CNetServer������ÿ��ʹ��CNetServer����ǰҪ���ô˺���
	int	Initial(const NET_INIT_INFO	*pNetInitInfo, CMessageMan *pMessageMan, long maxSupportFastVideo=1, bool bSupportDoubleStream = true);
	int AutoReportInitial(bool bSupport, const NET_AUTO_REPORT *pNetAutoReport);


	//
	void SetMaxFirstStreamSendNum(bool bLimitSendNum, unsigned char maxPlayBackChnnNum);

	void SetMaxIPCTransferChnnNum(unsigned long maxIPCTransferChnnNum);

	//�ı�����˿�
	int ChangeListenPort(short int nPort);

	//�ı�㲥ʱ���͵�http�˿�
	int ChangeHttpPort(short int nPort);

	//�ı���������������
	int ChangeMaxLink(unsigned char dwLink);

	int ChangeAutoCms(NET_AUTO_REPORT cmsInfo);
	int ChangeNatServerInfo(const NAT_SERVER_INFO * pNetServerInfo);
	void GetNatServerInfo(NAT_SERVER_INFO * pNetServerInfo){* pNetServerInfo = m_natSeverInfo;}
#ifdef __SUPPORT_3G__
	void SetPBStatus(bool bStartup){m_bStartPlayback = bStartup;};
#endif

	//ж��CNetServer���󣬵�����ʹ��CNetServer�����ʱ����ô˺���
	void Quit();

	//NetServer������ֻ�е�NetServer�����ˣ������Ĳ���������Ч
	int Start();

	//NetServerֹͣ
	int Stop();

    //����NAT��
    int NatStart();

    //NATģ���Ƿ���
    int NatIsStarted();

     //ֹͣNATģ��
    int NatStop();

	//��������ڷ���ģ���е���������
	void ClearData(unsigned long streamType);

	//��������ڷ���ģ���еĻط�����
	void ClearPBData(long clientID, unsigned long streamID);

	unsigned long GetLastStreamError(long clientID);

	//�õ�ָ��ͨ���������ݵ�����
	unsigned long GetStreamEncodedByteRate(long clientID, long channel, unsigned long frameType);

	//�õ�ָ��ͨ��ʵ�����緢�͵�����
	unsigned long GetStreamSentByteRate(long clientID, long channel, unsigned long frameType);

	//����ͳ�����
	void ResetStreamStatistics(long clientID = -1, long channel = ALL_CHANNELS);

	void EnableMasterStream(bool bEnable);

	//����������ʺڰ�����
	void SetBanAllowType(BAN_ALLOW_TYPE type);
	void SetBanAllowList(const IP_SECTION *pIP, unsigned char num);

	unsigned long GetFirstStreamSendNum( );

	ULONGLONG GetFirstStreamCH();
private:
	//ʹ�õ����������ù��캯��˽��
	CNetServer();

	//��ֹʹ��Ĭ�ϵĿ������캯����=�����
	CNetServer(const CNetServer&);
	CNetServer& operator=(const CNetServer&);

	//���пͻ���������������ô˺���
	static int AcceptLinkCallback(CLIENT_SOCK_INFO *pClientSockInfo, void *pParam1, void *pParam2);
	int	OnGetAcceptLink(CLIENT_SOCK_INFO *pClientSockInfo, unsigned long sockType);

	//�������ݷ����߳�
	static PUB_THREAD_RESULT PUB_THREAD_CALL SendDataThread(void *pParam);
	int SendDataRun();

	//�������ݽ��ջص�����
	static int RecvDataCallback(long clientID, void* pParam, const char* pBuf, int iLen);
	int RecvDataProc(long clientID, const char* pBuf, int iLen);

	//��鷢��״̬��Ϣ
	int CheckSendStatusData(long clientID, ULONGLONG &ullOldStatus, ULONGLONG ullNewStatus);
	int CheckSendRecStatusData(long clientID, NET_REC_STATUS &oldRecStatus, NET_REC_STATUS &newRecStatus);

	int RewriteServerJS( unsigned short newStreamPort );

	void SendData(long clientID, void *pData, int dataLen);

	void DispatchTalkStream();
	void DispatchLiveStream(unsigned long stream, BITMAPINFOHEADER *pBmpInfo, WAVEFORMATEX *pWaveInfo);
	void DispatchPBStream();
	void DispatchJpegStream();

	void RemoveClient(CLIENT_SERVICE_INFO *pClient);

	//�鲥�߳�
#ifdef   __ENVIRONMENT_LINUX__
	static PUB_THREAD_RESULT PUB_THREAD_CALL MulticastThread(void *pParam);
	int MulticastProc();
#endif

	//�������ݷ����߳�
	static PUB_THREAD_RESULT PUB_THREAD_CALL ConectCmsThread(void *pParam);
	int ConectCmsRun();

	bool HasAnySecondStreamOfOther(CLIENT_SERVICE_INFO *pClient);

private:
	CSWL_ListenProcEx		*m_pListenProc;
	CNatListenManager		*m_pNatListenManager;
	CSWL_MultiNetComm		m_multiNetComm;
	CSWL_MultiNetNat		m_multiNetNat;
    bool                  m_NatIsStarted;

	unsigned long			m_dwMaxLink;		//����Ŀͻ�������¼��

	PUB_lock_t				m_StartLock;		//�ڿ�ʼ��ֹͣ��ʱ��Ҫ���¼�ʹ���ĳ���Ͽ��Ŀͻ��˻���

 	SERVER_MESSAGE_CALLBACK	m_pMsgCallback;		//��Ϣ����
 	void*					m_pCallbackObject;	//���ûص�����ʱ�Ĳ���

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
	
	BITMAPINFOHEADER		*m_pBitmapInfo_F;	//��������ʽ
	BITMAPINFOHEADER		*m_pBitmapInfo_S;	//��������ʽ
	WAVEFORMATEX			*m_pWaveFormatEx;

	//��ÿ��ͨ��������������
	long					m_maxSupportFastVideo;
	unsigned char			*m_pFastVideoNum;

	//�����������ܣ�����ģ����������ʱ��û��������ܣ�ֻ���ж�ÿ��ͨ�������������ķ����������ƣ�����ÿ��ͨ���ܹ����Է��ͼ�������������
	//����Ĺ���������2516FDʱ�ӵġ�����Ĺ����ǣ�������DVR�����������ķ��������ơ�
	//������������������ֳ���������Զ�̻ط�������Զ�̱�������
	bool                    m_bLimitFirstStreamSendNum;  //�Ƿ����Ʒ����������ķ�����ͨ������������������͹ر��������
	int                     m_maxFirstStreamSendNum;
	int                     m_curFirstStreamSendNum;

	//����ͨ����ͻ��˵����ת��·��
	unsigned long			m_maxIPCTransferChnnNum;
	unsigned long			m_curIPCTransferChnnNum;

	//�Ƿ�������������������
	bool					m_bEnableMasterStream;

	//��������ÿ�ηַ���Ƶ���ݳ��ȼ�ʱ��
	long					*m_pDispatchFrameNum;
	LONGLONG				*m_pFirstDispatchFrameTime;

	//�Խ���ʽ
	WAVEFORMATEX			m_talkAudioFormat;

	//�鲥�߳����
#ifdef   __ENVIRONMENT_LINUX__
	PUB_thread_t			m_multicastID;
	bool					m_bmulticastRun;
	CMulticast				m_multicastEth0;
	CMulticast				m_multicastWifi;
#endif

	//�Ƿ�֧��˫����
	bool                    m_bSupportDoubleStream;
	
	//�ڰ�����
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
