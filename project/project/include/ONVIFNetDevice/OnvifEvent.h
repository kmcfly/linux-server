#ifndef  _ONVIF_EVENT_H_
#define  _ONVIF_EVENT_H_

#include "SWL_Public.h"
#include "DDPublic.h"
#include "OnvifAlarm.h"
#include "eventH.h"
#include "string.h"
#include "ClientRTP.h"
#include <map>
//#include "OnvifEventRecv.h"

#define RTSP_STREAMID_FOR_SNAP_VAL 0x5555

#define SNAP_JPEG_CAPTRUE_USED_DEFAULT 0xfff
#define SNAP_JPEG_STREAM_ID_VALUE 0xffff

#define EVENT_THREAD_PROCESS_CNT 3
#define EVENT_PROCESS_CHNNS_CNT 8

#include"SWL_MultiNetCommEx.h"

class CPackBitsCompress
{
public:
	CPackBitsCompress();
	~CPackBitsCompress();

	int Encode(const unsigned char *src,int srcLen,unsigned char *dest);
	int Decode(unsigned char *decSrc,int decLen,unsigned char *decDest);
	void Decode(unsigned char *decSrc,int decLen,unsigned char *decDest,int &destLen);
private:

protected:
	int  FindSameLargestStr(const unsigned char *src,unsigned int srcLen); 
	int   FindDiffLargestStr(unsigned char *src,unsigned int srcLen); 
	const unsigned char *FindLargestStr(const unsigned char *srcStr,unsigned int srcLen,const unsigned char *keyValue,bool &bSameValue);
};


const int   gChnnTotalNum = 64;
const unsigned int gRenewCount = 10;
const unsigned int gSubCnt = 7;

#define ONVIF_EVENT_PRINTF  //control the printf macro

enum ONVIFEVENTTYPE
{
	TEVENT_BASIC_SUBSCRIBE=0,
	TEVENT_PULLPOINT_SUBSCRIBE,
	TEVENT_END
};

enum EOnvifAlarmType
{
	TEVENT_ALARM_MOTION=0,
	TEVENT_ALARM_SHELTER,
	TEVENT_ALARM_SENSOR,
	TEVENT_ALARM_VIDEO_LOSS,
	TEVENT_ALARM_OTHER,
	TEVENT_ALARM_END
};

enum EOnvifEventChnnStatus
{
	TIDLE_STATUS=0,
	TSUBSCRIBE_STATUS,
	TRENEW_STATUS,
	TCANCEL_STATUS,
	TDEL_STATUS,
	TERR_WAIT_STATUS,
	TEND_STATUS
};

struct OnvifEventChnnInfo
{
	int chnn;
	char localIpAddr[96];
	char userName[32];
	char passKey[32];
	char servAddr[128];

	//subscribe response URI
	char subscribeUri[256];

	char b_renew_subscribe_event;

	//check num
	int checkCount;
	EOnvifEventChnnStatus status;

	bool bCanProcessPullMsg;
	bool bHasRecvAlarmMsg;
	EOnvifEventChnnStatus pullmsg_status;
	int pullmsg_checkcnt;

	char devUtcServAddr[128];
	unsigned int iIpcCtrlType;
};

typedef struct _onvif_event_recv_process
{
	int sock[EVENT_PROCESS_CHNNS_CNT];
	int channel[EVENT_PROCESS_CHNNS_CNT];
	bool brecv[EVENT_PROCESS_CHNNS_CNT];

	int cnt;
	int wr;
	int rd;
}onvif_event_recv_process;


//网络连接资源结构体
typedef struct _onvif_net_link_resource_
{
	SWL_socket_t		sock;
	long				deviceID;
}ONVIF_NET_LINK_RESOURCE;

class COnvifEvent
{
public:
	COnvifEvent();
	~COnvifEvent();

	static COnvifEvent *EventInstance(void);

	void EventOpen(void);
	void EventClose(void);

	bool GetCurDeviceIPText(char *pIPText, unsigned short textLen, const char* name = "eth0", unsigned short len = 4);
	void SetListenAddr(char *ipAddr,int aPort = 50099);
	void SetListenAddrByManual( char *ipAddr, int aPort = 50099);
	bool GetListenAddress( char* ipAddr, int len, const char* eth_name = "eth0", unsigned int eth_len = 4 );

	void AddChnnEvent(int chnn,OnvifEventChnnInfo* pOnvifEvent);
	void DelChnnEvent(int chnn);
	void DelAllEvent(void);

	void ResetChnnEvent(int chnn,char *servAddr);
	void SetDevLinkStatus(int chnn,bool bActive);

	int GetDevAlarm(int chnn,enum EOnvifAlarmType aAlarmType);

	static PUB_THREAD_RESULT PUB_THREAD_CALL OnvifSubscribeProcess(void *param);
	static PUB_THREAD_RESULT PUB_THREAD_CALL OnvifEventProcess(void *param);
	static PUB_THREAD_RESULT PUB_THREAD_CALL OnvifPullMsgGetStatic(void *param);
	static PUB_THREAD_RESULT PUB_THREAD_CALL OnvifRecvEventProcess(void *param);

protected:
	//thread process function
	PUB_THREAD_RESULT ProcessSubscribe(void);
	PUB_THREAD_RESULT ProcessListen(void);
	PUB_THREAD_RESULT ProcessPullMsg(void);
	PUB_THREAD_RESULT ProcessRecvEvent(void);


	int insert_event_rect_proc( int sock, int chnn, bool brecv );

	//basic subscribe
	int RecvMsg_SetupSubscribe(OnvifEventChnnInfo *aChnnInfo,char *devIp,int port);
	int RecvMsg_RenewSubscribe(OnvifEventChnnInfo *aChnnInfo);

	//subscribe update function
	void RecvMsg_CheckEventUpdate(void);
	void RecvMsg_CheckChnnStatus(void);

	int CancelSubscribe(OnvifEventChnnInfo *aChnnInfo);

	//pull point subscribe
	int PullMsg_SetupSubscribe(OnvifEventChnnInfo *aChnnInfo);
	int PullMsg_GetAlarmMessages(OnvifEventChnnInfo *aChnnInfo);

	void PullMsg_UpdateAlarmEvent(void);

	void UpdateIpAddr(char *oldAddr,char *newAddr);

	void GetEventInfoFromTopic( int channel, char* AlarmTopic,char* AlarmMessage );

	inline char* strlwr_tvt(char* str)
	{
		char* orig = str;
		for (; *str != '\0'; str++)
		{
			*str = tolower(*str);
		}
		return orig;
	}


	//recv message handle
	int BindSock(int port);
	int EventRecvNotify(int sock,unsigned int channel, bool bRecvMsg);
	unsigned int CheckEventIP(char *ip, bool& bRecvMsg);
	void SetHasRecvMsgProc(int chann);
private:
	PUB_thread_t						m_subscribeId;
	bool								m_bSubscribe;

	PUB_thread_t						m_eventRecvId;
	bool								m_bEventRecv;

	PUB_thread_t                        m_eventProcessID[EVENT_THREAD_PROCESS_CNT];
	bool                                m_bEventProcess[EVENT_THREAD_PROCESS_CNT];
	int                                 m_iEventIndex;
	onvif_event_recv_process            m_stEventProcInfo[EVENT_THREAD_PROCESS_CNT];
	CPUB_Lock                           m_lockEventProcInfo[EVENT_THREAD_PROCESS_CNT];

	PUB_thread_t						m_eventPullMsgId;
	bool								m_bPullMsgCreate;

	//
	bool								m_bChnnActive[gChnnTotalNum];

	//alarm signal lock
	CPUB_Lock							m_motionLock;
	CPUB_Lock							m_shelterLock;
	CPUB_Lock							m_sensorLock;
	CPUB_Lock							m_videoLossLock;
	CPUB_Lock							m_otherLock;
	//alarm signal
	unsigned   char						m_motionAlarm[gChnnTotalNum];
	unsigned   char						m_shelterAlarm[gChnnTotalNum];
	unsigned   char						m_sensorAlarm[gChnnTotalNum];
	unsigned   char						m_videoLossAlarm[gChnnTotalNum];
	unsigned   char						m_otherAlarm[gChnnTotalNum];

	//event information list lock
	CPUB_Lock							m_eventInfoLock;
	CPUB_Lock							m_eventSubscriLock;
	CPUB_Lock							m_chnnActiveLock;


	//storage the different channel device information 
	std::list<OnvifEventChnnInfo *>		m_eventChnnInfoList;

	//subscribe tmp list 
	std::list<OnvifEventChnnInfo *>		m_eventUpdateList;

	//terminate time
	char								m_terminationTime[8];

	//listen socket id
	int									m_listenSockID;

	//listen  ip address lock
	CPUB_Lock							m_listenAddrLock;
	//listen addr and port
	int									m_port;
	char								m_listenIp[96];
	bool								m_bIpExist;
	//recv information
	fd_set								m_fdr;

};

const unsigned int  gOnvifSnapBuffLen = 256*1024;
const unsigned int  gOnvifSnapChnnNum = 32; //the total channel num

//enum declare
typedef enum _onvif_media_type_
{
	ONVIF_MEDIA_PIC=0,
	ONVIF_MEDIA_VIDEO,
	ONVIF_MEDIA_AUDIO,
	ONVIF_MEDIA_TEXT,
	ONVIF_MEDIA_END
}ONVIF_MEDIA_TYPE;

typedef enum _onvif_msg_type_
{
	ONVIF_MSG_HTTP = 0,
	ONVIF_MSG_RTSP,
	OVNIF_MSG_RTCP,
	ONVIF_MSG_TEXT,
	ONVIF_MSG_END
}ONVIF_MSG_TYPE;

typedef enum _onvif_snap_opera_status_
{
	ONVIF_OPERA_IDLE = 0,
	ONVIF_OPERA_AUTO,
	ONVIF_OPERA_MANUAL,
	ONVIF_OPERA_AUTO_MANUAL,
	ONVIF_OPERA_END
}ONVIF_SNAP_OPERA_STATUS;

typedef enum _onvif_snap_status_
{
	ONVIF_SNAP_WAITING= 0,
	ONVIF_SNAP_GET_PIC,
	ONVIF_SNAP_IDLE,
	ONVIF_SNAP_FAILED,
	ONVIF_SNAP_SUCCESS,
	ONVIF_SNAP_SEND_FAILED,
	ONVIF_SNAP_SEND_SUCCESS,
	ONVIF_SNAP_RECVING,
	ONVIF_SNAP_RECV_SUCCESS,
	ONVIF_SNAP_STATUS_END,
}ONVIF_SNAP_STATUS;


//struct declare
typedef struct _onvif_media_
{
	//device channel information
	int							chnnNum; //>=0,valid
	//socket information
	unsigned char				sockType;//0:TCP,1:UDP
	SWL_socket_t					sockFd;

	//media type and url
	ONVIF_MEDIA_TYPE				mediaType; //picture,video, audio,or text
	char							mediaUrl[256]; //media service url

	//device connect status
	ONVIF_SNAP_STATUS			status;

	//send msg parameter
	unsigned short               picInterval;
	unsigned short               picNum; //get picture num in picInterval seconds
	unsigned short               cap_jpeg_number;
	//probe count
	unsigned int                 probeCount; //probeCount = 0,modify status

	//snap operation type
	ONVIF_SNAP_OPERA_STATUS      operaType;
	unsigned  int                currManualNum; //manual snap picture number already
}ONVIF_MEDIA;

typedef struct _onvif_snap_pic_recv_
{  
	bool                      bActive;
	int                       totalPicLen;
	int                       recvPicLen;
	int                       prevHttpLen;
	int                       maxBufLen;
	unsigned   char           *picBuf;

	//device parameter
	//recv data buffer
	RTSP_DATA_FRAME             *picData;
	RTP_CALLBACK                cbFun;
	void                        *devParam;
}ONVIF_SNAP_PIC_RECV;

//class declare

typedef int (*ONVIF_SNAP_CALLBACK)(RTSP_DATA_FRAME *pFrameData, int SeesionID, void *pParam);

enum
{
	HTTP_CMD_OK				= 200,
	HTTP_CMD_UNAUTHORIZED	= 401
};

class COnvifSnapJpeg
{
public:
	~COnvifSnapJpeg();

	static COnvifSnapJpeg *Instance();
	bool   Start();
	bool   Stop();

	int     AddDevSnap(int channel);
	void    DelDevSnap(int channel);
	void    DelAllSnap();

	int     StartSnap(int channel, char *pSnapUri, ONVIF_SNAP_CALLBACK CallBackFun, void *pParam, char* pServerAddress);
	int     StopSnap(int channel);

	int     SetSnapPeriodParam(int channel, unsigned short period, unsigned short num, unsigned int capture_number);

	typedef struct
	{
		ONVIF_SNAP_CALLBACK			callBack;
		void						*pParam;
		bool						bStart;
		bool						bConnected;
		unsigned int				snapInterval;
		unsigned int                defaultInterval;
		unsigned int                captureNumber;
		unsigned int				startTick;
		SWL_socket_t				sock;
		char						*pSendBuf;
		char						*pRecvBuf;
		char						*pRecvRes;
		unsigned int				sendLen;
		unsigned int				currSendLen;
		unsigned int				recvBufLen;
		unsigned int				currRecvLen;
		unsigned int				contentLen;
		unsigned long				ip;
		unsigned short				port;
	}SnapInfo;

private:
	COnvifSnapJpeg();

	static PUB_THREAD_RESULT PUB_THREAD_CALL SendThread(void *pParam);
	static PUB_THREAD_RESULT PUB_THREAD_CALL RecvThread(void *pParam);
	void SendProc();
	void RecvProc();

	void ConnectToServer();
	bool Connect(SWL_socket_t &sockFd, unsigned long ip, unsigned short port);
	void SendCmd();
	int SendCmd(SWL_socket_t sockFd, const char *pData, unsigned int len);
	void Recv(fd_set &fdSet);
	int Recv(SWL_socket_t sockFd, char *pBuf, unsigned int len);
	bool ParseUri(const char *pUri, const char* pServerAddress, char *pRequestCmd, unsigned long &ip, unsigned short &port);
	int ParseCmd(const char *pData, unsigned int len, unsigned int &contentLen);
	void CloseConnect(SnapInfo *pSnapInfo);
	void CloseSock(SWL_socket_t &sockFd);
	void CloseAll();
	void CallCb(SnapInfo* pSnapInfo, unsigned int channel);
	void SnapIntervalCtrl();
private:
	bool				m_bIsStart;
	PUB_thread_t		m_hRecvThreadID;
	PUB_thread_t		m_hSendThreadID;
	bool				m_bRecvThreadProc;
	bool				m_bSendThreadProc;
	CPUB_Lock			m_Lock;
	fd_set				m_fdsetRead;
	SWL_socket_t		m_MaxSockFd;

	std::map<int, SnapInfo*>		m_mapSnapInfo;
};

#endif