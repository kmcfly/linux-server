#pragma once

//#include "SWL_ListenProcEx.h"
//#include "SWL_MultiNetComm.h"
//#include "AVDataSendMan.h"
#include <eXosip2/eXosip.h>
#include <osip2/osip_mt.h>
#include <eXosip2/eX_setup.h>
#include "eXosip2/eX_register.h"

#include <map>
#include <utility>
#include <string>

#include"ServerRTPMan.h"
#include "ServerRTPSession.h"

#include "Gb28181Server.h"
#include"Gb28181sdp.h"

#include "Gb28181Config.h"

#define GB_SUCCESS           0x00000000
#define GB_FAILURE           0xFFFFFFFF

#define ERR_INVALID_ARG		-2
#define NO_RESP				-3

#define MAX_ID_LEN		32			// 最大的ID长度
#define MAX_CODE_LEN	36			// 最大的域长度


#define MAX_PARA_STR_LENGTH		2048
#define MAX_RECV_MESSAGE_LENGTH  4096


typedef enum _register_state
{
	GB_REGISTER_IDLE= 0,
	GB_REGISTER_FRIST,    //frist  register
	GB_REGISTER_SECOND,  //second   register
	GB_REGISTER_SUCCESS, //register  success
	GB_REGISTER_LOGINOUT,  //注销
	GB_REFISTER_COUNT,
}REGISTER_STATE;

//给RTPServer回调，通知回放或者下载结束
void FileStreamEnd(int SessionID);

class CGb28181Server;
class CSipServer
{
public:
	static CSipServer* Instance();

	~CSipServer();

	//初始化CSipServer对象，在每次使用CSipServer对象前要调用此函数
	int	 Initial();

	//Osip初始化
	int OsipInit();
	
	//Osip反初始化
	int  OsipUnInit();   

	void SetParam(); //设置默认参数


	//卸载CSipServer对象，当不再使用CSipServer对象的时候调用此函数
	void Quit();

	//CSipServer启动，只有当CSipServer启动了，其他的操作才能有效
	int	 Start();

	//CSipServer停止
	int	 Stop();

	//注销
	int DvrLoginOut(bool bNeedLoginOut=true);

	//start时，调用对sipserver的状态初始化
	void InitSipServerState();

	//发送消息
	int	SendMassage(char *xmlmsg,size_t xmlmsg_length);

	//心跳保持
	void HeartKeepLive();

	//获取注册的状态
	REGISTER_STATE GetRegisterState();

	//回放自动结束时调用
	void ReleaseStream(int SessionID);

	//通过channel ID获取session ID
	int GetSessionID(unsigned long ulChannelID);
	
	//清楚所有连接
	bool RemoveAll();
private:

	//使用单件，所以让构造函数私有
	CSipServer();

	//invate 处理
	int	InviteProc(eXosip_event_t *evt);

	//判断该通道的视频流是否被请求过
	bool StreamIsRequested(unsigned long ulChannelID);

	int AckProc(eXosip_event_t *evt);

	//判断播放类型，传输视频流
	int Gb28181StartStream(CALLINFO *pCallInfo);

	//开始传输流
	int StartStream(CALLINFO *pCallInfo);

	 //请求读取回放视频流
	void  RequestDataRead(CALLINFO *pCallInfo);
	
	//停止请求回放流
	void StopRequestDataRead(CALLINFO *pCallInfo);

    // 回放流控制消息处理
	int InfoMsgProc(eXosip_event_t *evt);

	//解析MANsRtsp消息，并回复
	int  MansRtspParseAnswer(eXosip_event_t *evt, osip_message_t * info);

	//关闭视频流
	int StreamByeProc(eXosip_event_t *evt);

		//sdp解析和sdp构造
	int SdpParseAnswer (eXosip_event_t *evt, char *buf);

	//注册线程
	void SipServerRegister();

	//消息接收
	void RecvMsg();

	/**/
	void SendSipMsg();

	//获取xml缓存
	int	GetMessageBody(eXosip_event_t *evt,char *xmlmsg,size_t *xmlmsg_length);

	//向服务器发起第一次注册
	int	RegisterFirst(int &regid,bool IsLoginOut=false); 

    //重复注册使用
	int RegisterAgain(int regid);


	//增加验证信息,向服务器发起第二次注册
	int RegisterSecond(int regid,bool IsLoginOut=false);

	//注册成功后，同步当前系统时间
	int ParseServerTime(osip_message_t *message,struct tm &tm_time);

	//发送request请求
	int MsgSendRequest(char *msg_method,char *to, char *from,char *content,char *conten_type);

	//响应请求 200ok
	int MsgSendAnswer(eXosip_event_t *evt,int status);

	//设置注册的状态
	void SetRegisterState(REGISTER_STATE  RegisterState);  

	//将解析后的SDP信息存入invate链表
	bool AddInvateMsgToList(CALLINFO *pCallInfo);


	//从invate链表中读出SDP信息
	bool GetInvateMsgFromList(CALLINFO* &pCallInfo);

	int PaseRemoteSdp(sdp_message_t *remote_sdp,CALLINFO*pCallInfo);

	/*构造sdp消息*/
	int formSdpInfo(char *buf,CALLINFO *pCallInfo);


	void LockStreamList() {m_lStreamLock.Lock();}
	void UnLockStreamList() {m_lStreamLock.UnLock();}

	static PUB_THREAD_RESULT PUB_THREAD_CALL RegisterProc(void *pParam);

	//消息接收线程
	static PUB_THREAD_RESULT PUB_THREAD_CALL RecvMsgProc(void *pParam);
	
	//消息发送线程
	static PUB_THREAD_RESULT PUB_THREAD_CALL SendMsgProc(void *pParam);

private:

	GB28181_CONFIG				*m_pGb28181_Config;					//配置文件

	REGISTER_STATE				RegisterState;						//注册的状态
	char						m_strSipserverIP[MAX_ID_LEN];		//sip服务器IP地址
	unsigned short				m_iSipserverPort;					//sip服务器会话端口
	unsigned short				m_iSipDvrPort;						//dvr sip会话端口

	bool                        m_SipserverQuit;                   //sipserver 退出标志
	bool                        m_bRegisterRun;                      //消息线程是否正在运行
	bool                        m_bRcvMsgRun;                      //消息线程是否正在运行
	bool                        m_bSndMsgRun;                      //消息发送线程是否正在运行
	bool                        m_bRegister;                        //是否发起重复注册
	
	bool                        m_bLoginOut;                       //注销

	char						m_strSipServerId[MAX_ID_LEN];		//sip服务器ID
	char						m_strDvrip[MAX_ID_LEN];				//DVR ip
	char						m_strDvrId[MAX_ID_LEN];						//设备Id
	char						m_strUserID[MAX_ID_LEN];			//用户名
	char						m_strRegisterPasswd[MAX_ID_LEN];	//注册密码
	unsigned int				m_uiExpires;						//过期时间
	unsigned int                m_uiSessionID;     //音视频流的会话ID
	
	PUB_thread_t				m_registerID;						//注册接收线程
	PUB_thread_t				m_recvmsgID;						//消息接收线程
	PUB_thread_t				m_sendmsgID;						//消息发送线程
	CGb28181Server				*m_pGb28181Server;					//访问Gb28181Server接口          
	CServerRTPMan				*m_pRtpServer;						//用来控制RtpServer
	std::list<CALLINFO*>    m_listCallInfo;             //invate sdp 解析
	
	CPUB_Lock					m_lStreamLock;			//stream list lock
};