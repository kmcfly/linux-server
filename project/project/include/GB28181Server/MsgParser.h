#pragma once

#include <map>
#include <utility>
#include <string>

#include "MarkupSTL.h"
#include "Protocol.h"
#include"MessageMan.h"

#include "Gb28181Server.h"

#include "Gb28181Config.h"
#include "Gb28181Ptz.h"
#include "Gb28181Mansrtsp.h"
#define  MAX_ALARM_NUM 16
#define  MIN_SEARCH_TIME   1262275200  //2010-01-10 00:00:00
typedef enum  _search_type
{
	SEARCH_TYPE_TIME = 0, //按时间检索录像文件
	SEARCH_TYPE_ALARM ,   //按报警检索录像文件
	SEARCH_TYPE_MANUAL,   //按手动录像检索文件
	SEARCH_TYPE_ALL,      //检索所有录像文件
}SEARCH_TYPE;

typedef struct _cmd_data
{
	XML_PROTOCOL_TYPE	cmdtype;	//命令的类型
	unsigned long   channelId;      //通道ID
	unsigned long 	SN;         //命令序列号
	char  szDeviceID[32];				//设备ID
	void *pdata;                   //数据
	int datalen;                   //数据长度
}CMD_DATA;


class CGb28181Server;
class CMsgParser
{
public:
	static CMsgParser* Instance();

	~CMsgParser();

	int	Initial();

	//卸载CGb28181Server对象，当不再使用CGb28181Server对象的时候调用此函数
	void Quit();

	//CGb28181Server启动，只有当CGb28181Server启动了，其他的操作才能有效
	int	 Start();

	//CGb28181Server停止
	int	 Stop();

	//上报文件结束,
	void NotifyFileEnd(char *m_cDeviceID);

private:
	//使用单件，所以让构造函数私有
	CMsgParser();

	//禁止使用默认的拷贝构造函数和=运算符
	CMsgParser(const CMsgParser&);

	CMsgParser& operator=(const CMsgParser&);

	void LockCmdList() {m_lCmdListLock.Lock();}
	void UnLockCmdList() {m_lCmdListLock.UnLock();}

	//将sip消息转化成内部配置文件
	int ParseMsg(const SIP_MESSAGE_BODY &sipMsgBody,MESSAGE_DATA &msgData);

	/*解析xml消息*/
	int	ParseXMLMsg(const string & strMsgBody,MESSAGE_DATA & msgData);

	//将内部配置文件转化成sip消息
	int ReverseParseMsg(const MESSAGE_DATA &msgData,SIP_MESSAGE_BODY &sipMsgBody);

	//解析消息体，并发生消息
	int	ParseMsg();

	//反解析消息体，并发送sip消息
	int	ReverseParseMsg();

	//告警上报
	int AlarmSend();

	//心跳
	bool NotifyKeepAlive(); 

	//心跳是否正常
	bool GetHeartBreak();  

	//将消息压入队列，等会messageMan回应构造xml
	bool AddParseMsgToList(const SIP_MESSAGE_BODY &sipMsgBody);  

	//获取队列，构造xml
	bool GetParseMsgFromList(SIP_MESSAGE_BODY &sipMsgBody);

    /*将需要的命令和参数压入队列*/	
	bool AddCmdDataToList(const CMD_DATA &CmdData);

	/*反解析时取出需要的命令和参数*/
	bool GetCmdDataFromList(CMD_DATA &CmdData);

	/*解析相关命令*/
	int ReverParseCmd(CMD_DATA &CmdData);

	/*告警查询*/
	bool  QueryAlarm();

	//告警信息上报
	int NotifyAlarm(CHNN_STATUS_TYPE type ,ULONGLONG & value);
	
	//填充报警的具体信息
	void SetAlarmInfoText(CHNN_STATUS_TYPE type,CNotifyAlarmProtocol *pNotifyAlarmProtocol);

	//释放设置配置
	void ReleaseSetCfg();

	 /*设备控制*/
	int	ControlDevice(CControlProtocol* pControlProtocol ,MESSAGE_DATA & msgData,SIP_MESSAGE_BODY &sipMsgBodyResp);

	/*将时间字符串转换为无符号整形数*/
	unsigned long ConvertTimeToULong(char *time);

	/*将无符号整形数时间转换为字符串%d-%d-%dT%d:%d:%d 形式*/
	int ConvertTimeToStr(unsigned  long ulTime,char *buf);

	/*录像文件检索*/
	int ProcReplyDataInfo(const MESSAGE_DATA &msgData,SIP_MESSAGE_BODY &sipMsgBody);

	//获取录像文件的检索方式
	int GetSearchType(char *m_szType);

	//设置录像文件的搜索类型
	void SetSearchType(unsigned long  lSearchType,char *m_szType);

	//填充录像文件搜索条件
	void SetSearchDataInfo(int type,NET_DATA_SEARCH *pdataInfo);

	//处理从MessageMan获取的配置
	int ProcReplyCfgData(const MESSAGE_DATA &msgData);

	//处理反解析消息
	int ProcRespMsg(const MESSAGE_DATA &msgData,SIP_MESSAGE_BODY &sipRespMsg);

	//解析sdp或xml消息体并转化内部配置并发送消息线程
	static PUB_THREAD_RESULT PUB_THREAD_CALL ParseMsgProc(void *pParam);

	//反解析消息，并组装xml或sdp协议发送出去
	static PUB_THREAD_RESULT PUB_THREAD_CALL ReverseParseMsgProc(void *pParam);

	//告警查询上报线程
	static PUB_THREAD_RESULT PUB_THREAD_CALL AlarmSendProc(void *pParam);

	//构造控制命令回复的response消息
	int XmlFormControlResp(CBasicProtocol* pBasicProtocol,SIP_MESSAGE_BODY & sipMsgBodyResp);

	//构造获取设备状态的xml
	int XmlFormDeviceStatusResp(CBasicProtocol* pBasicProtocol,SIP_MESSAGE_BODY & sipMsgBodyResp);
	
	//构造获取设备信息的xml
	int XmlFormDeviceInfoResp(CBasicProtocol* pBasicProtocol,SIP_MESSAGE_BODY & sipMsgBodyResp);

	//构造获取目录的xml
	int XmlFormCatalogResp(CBasicProtocol* pBasicProtocol,SIP_MESSAGE_BODY & sipMsgBodyResp);

	//录像目录查询
	int XmlFormRecordInfoResp(CResponseRecordInfoProtocol & m_ResponseRecordInfoProtocol,SIP_MESSAGE_BODY & sipMsgBodyResp);

	//订阅alarm事件回应
	int XmlFormAlarmResp(CBasicProtocol* pBasicProtocol,SIP_MESSAGE_BODY & sipMsgBodyResp);
	
private:

	std::list<SIP_MESSAGE_BODY>	m_listParseMessage;			//用来存放准备发送的消息
	std::list<CMD_DATA>         m_listPackCmd;              //反解析需要用的数据

	CPUB_Lock					m_lCmdListLock;			//Command  list lock

	CGb28181Server				*m_pGb28181Server;			//访问Gb28181Server接口
	CMessageMan					*m_pMessageMan;             //访问MessageMan接口
	GB28181_CONFIG				*m_pGb28181_Config;					//配置文件
	
	bool                        m_bHeartBreak;               //心跳是否正常
	bool						m_bParseMsgRun;            
	bool						m_bReverseParseMsgRun;
	bool                        m_bAlarmSendRun;
	bool                        m_bAlarmGuard[32];                   //是否开启布防
	bool                        m_bRecord;                   //是否开启布防

	PUB_thread_t				m_ParseMsgID;				//消息接收线程
	PUB_thread_t				m_ReverseParsemsgID;		//消息接收线程
	PUB_thread_t				m_AlarmSendID;		//告警上报线程
};
