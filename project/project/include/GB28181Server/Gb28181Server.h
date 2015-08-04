#pragma once

#include "MessageMan.h"
#include "Gb28181Config.h"

#include <map>
#include <utility>
#include <string>

#define GB_SUCCESS           0x00000000
#define GB_FAILURE           0xFFFFFFFF

typedef enum _manscdp_type
{
	UnknownType = 0,		//无类型
	CONTROL,				//表示一个控制的动作
	QUERY,					//表示一个查询的动作
	NOTIFY,					//表示一个通知的动作
	RESPONSE,				//表示一个请求动作的应答
}MANSCDP_TYPE;


typedef enum _body_type
{
	Unknown = 0,     //无类型
	SDP,			 //SDP消息体
	MANSCDP_XML,     //MANSCDP+XML消息体
	MANSRTSP,		 //MANSRTSP 消息体
	DVR_CMD_REPLY_CFG_SUCC,//请求设置成功
	DVR_CMD_REPLY_CFG_DATA,//获取配置
	DVR_CMD_REPLY_CFG_FAIL,//请求设置失败
	DVR_CMD_REPLY_CHANAGE_TIME_SUCC,//设置系统时间成功
	DVR_CMD_REPLY_CHANAGE_TIME_FAIL,//设置系统时间失败
	DVR_CMD_REPLY_PTZ_SEND_CMD_SUC,  //向云台发送命令成功
	DVR_CMD_REPLY_DATA_INFO,		//回复的搜索结果数据，附带数据
}BODY_TYPE;

typedef struct _sip_message_body
{
	BODY_TYPE		body_type;
	MANSCDP_TYPE	manscdp_type;
	std::string		strMsgBody;
	unsigned int	uiMsgLen;
}SIP_MESSAGE_BODY;

class CSipServer;
class CMsgParser;
class CGb28181Server  
{
public:
	static CGb28181Server* Instance();
	
	~CGb28181Server();
	      
	//初始化CGb28181Server对象，在每次使用CGb28181Server对象前要调用此函数
	int	 Initial(CMessageMan *pMessageMan);
	
	//更新配置，需重启sipServer
	void SetSipServerConfig(const GB28181_SIP_SERVER & Gb28181SipServer);

	//设置配置
	void SetGB28181Cfg(const GB28181_SIP_SERVER & Gb28181SipServer);

	void SetGB28181Info(const Gb28181_INIT_INFO & Gb28181Info);

	void SetGb28181ChannelIDConfig(const GB28181_CHANNEL_ID & Gb28181ChannelID);

	void SetGb28181AlarmIDConfig(const GB28181_ALARM_ID & Gb28181AlarmID);

	void SetGb28181CatalogInfo(const GB28181_CHANNEL_ID & Gb28181ChannelID,const GB28181_ALARM_ID & Gb28181AlarmID);

	void SetGb28181DeviceStatusInfo(const GB28181_CHANNEL_ID & Gb28181ChannelID,const GB28181_ALARM_ID & Gb28181AlarmID);


	//卸载CGb28181Server对象，当不再使用CGb28181Server对象的时候调用此函数
	void Quit();
	
	//CGb28181Server启动，只有当CGb28181Server启动了，其他的操作才能有效
	int	 Start();
	
	//CGb28181Server停止
	int	 Stop();

	//获取客户端ID
	unsigned long  GetClientID();

	//向内部发送消息
	void PutMsgToServer(const MESSAGE_DATA & msgData);

	//从内部获取消息
	bool GetMsgFromServer(MESSAGE_DATA & msgData);

	//添加一个消息到接收消息队列中
	bool AddMessageToRevList(const SIP_MESSAGE_BODY &sipMsgBody);

	//从发送的消息队列中获取一个消息
	bool GetMessageFromSendList(SIP_MESSAGE_BODY &sipMsgBody);

	//从接受消息队列中获取一个消息
	bool GetMessageFromRevList(SIP_MESSAGE_BODY &sipMsgBody);
	
	//添加一个消息到即将发送出去sip队列中
	bool AddMessageToSendList(const SIP_MESSAGE_BODY &sipMsgBody);
	
	//释放消息内存函数
	inline void ReleaseMsgBuff(MESSAGE_DATA &msgData) {this->m_pMessageMan->ReleaseMsgBuff(msgData);}

	//注册成功后，同步当前系统时间
	int SetDvrTime(struct tm &tm_time); 


	//bool TrySendListLock(){ return  m_listSendMsgLock.TryLock();}
	void LockSendList() {m_listSendMsgLock.Lock();}
	void UnLockSendList() {m_listSendMsgLock.UnLock();}


	void LockRevList() {m_listRevMsgLock.Lock();}
	void UnLockRevList() {m_listRevMsgLock.UnLock();}
	
private:

	//添加一个消息到消息队列中
	bool AddMessageToList(const SIP_MESSAGE_BODY &sipMsgBody,std::list<SIP_MESSAGE_BODY> & listMessage);

	//从消息队列中获取一个消息
	bool GetMessageFromList(SIP_MESSAGE_BODY &sipMsgBody,std::list<SIP_MESSAGE_BODY> & listMessage);

	//使用单件，所以让构造函数私有
	CGb28181Server();
	
	//禁止使用默认的拷贝构造函数和=运算符
	CGb28181Server(const CGb28181Server&);
	
	CGb28181Server& operator=(const CGb28181Server&);

private:

	PUB_lock_t					m_Gb28181Lock;  
	bool						m_bGb28181Quit;		//退出标志
	bool						m_bIsStartSipServer;//是否要重启sipserver
	unsigned long				m_iClientID;		//客户端ID
	CPUB_Lock					m_listRevMsgLock;			//接受Sip消息链表锁
	CPUB_Lock					m_listSendMsgLock;			//发送消息链表锁
	
	std::list<SIP_MESSAGE_BODY>	m_listRevMessage;			//用来存放接收到的Sip消息
	std::list<SIP_MESSAGE_BODY>	m_listSendMessage;			//用来存放准备发送的Sip消息

	CSipServer					*m_pSipServer;			//用来控制SipServer
	CMsgParser					*m_pParser;				//用来访问Parser
	CMessageMan					*m_pMessageMan;
	GB28181_CONFIG				*m_pGB28181_CONFIG;		//配置
private:
	GB28181_CHANNEL_ID          m_Gb28181ChannelID;   //临时保存通道配置的结构
	GB28181_ALARM_ID			m_Gb28181AlarmID;     //临时报警配置的结构
};