/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : zxx
** Date         : 2012-11-22
** Name         : ShanghaiConnectMan.h
** Version      : 1.0
** Description  :
** Modify Record:
***********************************************************************/

#ifndef _SHANGHAI_CONNECTMAN_H_
#define _SHANGHAI_CONNECTMAN_H_

#include "PUB_common.h"
#include "SWL_Public.h"
#include "SWL_ListenProcEx.h"
#include "tinyxml.h" 
#include "DiskManager.h"
#include "ReclogManEx.h"
#include "ShanghaiCmd.h"
#include "NetProtocol.h"
#include "LogMan.h"
#include <list>
#include <string>
#include <map>
using namespace std;

const int  recvSendBufLen = 1024;
const char pShanghaiXmltitle[] = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\r\n";
const unsigned long maxShanghaiDataSize = 2048;  
const unsigned long maxCmdCount = 100;//最多保存100个指令 
#pragma pack(1)
typedef struct msgHead
{
	char heart[6];//QWCMD
	unsigned long headlen;//消息长度，含头

	char resver[10];//保留字节
}MSG_HEAD;
#pragma pack()

typedef struct _delay_dailyinfo_
{
	unsigned long		operType;//消息的操作类型
	unsigned long		chnn;	//消息通道
	char			    triggerTime[20];//触发时间
}DELAY_DAILYINFO;

typedef struct _heart_info_
{
	ULONGLONG TotalSpace;//所有磁盘加起来的总容量和，以G为单位
	ULONGLONG FreeSpace;//所有磁盘剩余容量的总和，以G为单位
}HEARTINFO;

typedef enum net_err
{
	ERR_SUCC,
	ERR_OPERTYPE,
	ERR_PICTOOBIG,
	ERR_NETERR,
	ERR_PTENABLE,//设置参数平台关闭
}NET_ERRINFO;

class CShanghaiConnectMan
{
public:
	static CShanghaiConnectMan *Instance();
	~CShanghaiConnectMan();

	void Initial();
	void Quit();

	bool Start();
	void Stop();

	void SetPTEnabel(bool benable);//启用平台
	bool GetPTEnable();
	void SetMotionEnable(bool benable);//移动侦测启用
	void SetStartHeartProc();//开启发送心跳

	void SetAgentId(const char *pAgentId);//设置代理id
	void Setport(unsigned long port);//设置服务器端口
	void SetIp(const char *pIp);//设置代理ip

	void GetHeartInfo(CReclogManEX	*pReclogMan);//每次心跳都去获取磁盘消息和dvr标识
	//获取日志的操作,添加入操作列表并发送
	void GetLogCmd(unsigned long type,unsigned long chnn,char *ptext,int textLen,void *pPicBuf,int picBufLen,ULONGLONG time);
	
	void GetMotionAlarmSnap(OPER_INFO snapInfo);//获取移动侦测报警截图
	void GetSensorAlarmSnap(OPER_INFO snapInfo);//获取外部触发报警截图

	//Base64算法
	int to64frombits(unsigned char *out, const unsigned char *in, int inlen);

	void SetMotionHead(bool bhead);//添加移动侦测的头
	void SetSensorHead(bool bhead);//添加外部触发的头
	bool GetMotionHead();
	bool GetSensorHead();

	void SetMotionAlarmStartTime(unsigned long time);
	void SetSensorAlarmStartTime(unsigned long time);

	LONGLONG GetTriggerTime();
        LONGLONG GetChannelBits();
	
protected:
private:
	CShanghaiConnectMan();
	/****************************线程部分*****************************************/
	//该线程负责心跳、信息、图片的发送
	static PUB_THREAD_RESULT PUB_THREAD_CALL HeartThread(void *pParam);
	void HeartProc();

	/****************************线程部分*****************************************/

	/*********************************xml建立部分*******************************/
	//建立Agent
	TiXmlElement * CreateAgent(TiXmlDocument *document,char *pdvrType);
	//心跳的时候建立heart
	void CreateShanghaiHeartXml(TiXmlElement *pElement,char *pdvrType,HEARTINFO hear_info);
	//补充opercmd
	TiXmlElement * AddOperXml(TiXmlElement *pElement,OPER_INFO operInfo);
	/*********************************xml建立部分*******************************/

	/***********************************socket部分******************************/
	bool Connectheart();//建立并连接心跳
	
	//心跳获取接收到的xml的服务器时间和下次接收心跳的时间间隔
	bool GetHeartRecvTimeAndLeap(char *pSrc,int &TimeValue);
	
	void ProcessGetImageXml(char *xmlbuf);

	//发送和接收socket信息
	bool SendAndRecvHeart(char *pXmlbuf,int &recvTimeLeap,bool bheart);
	/***********************************socket部分******************************/

	/****************************************相关函数******************************/
	bool GetDYRSysErrExitLog();//查找上次关机是否异常，异常为true,否则为false
	
	void gettmreciveTime(tm &serverTm);//获取接收到的时间
	void ChangeSystime();//改变系统时间
	void GetOperCmd(OPER_INFO Opercmd);//获得操作的消息命令
	/****************************************相关函数******************************/

	/*****************************************线程操作函数**************************/
	int HeartOnlyFunction(int &recvTimeLeap);//心跳操作
	int CmdOnlyFunction(int type);//发送指令和图片

	void CMDMotionAlarm();//移动侦测报警处理
	void CMDSensorAlarm();//外部触发报警处理
	/*****************************************线程操作函数**************************/
private:
	PUB_thread_t		m_heartID;
	bool				m_bheartProc;
	SWL_socket_t		m_heartsock;//只有一个套接口
	
	PUB_thread_t		m_cmdID;
	bool				m_bCmdProc;

	std::list<OPER_INFO> m_listCmd;				//命令操作列表和手动以及日常图片
	std::list<OPER_INFO> m_listMotionPic;		//移动侦测图片列表
	std::list<OPER_INFO> m_listsensorPic;		//外部触发（传感器）图片列表
		
	HEARTINFO m_hear_info;//
	
	char m_recvTime[256];
	
	CReclogManEX	*m_pReclogMan;//该处的指针是从MainFrame获取而来，目的是为了获取心跳时候所需要的剩余磁盘容量
	
	int m_DVrStart;		   //日志查找启动标记
	int m_recvTimeLeap;//心跳接收到的时间间隔
	unsigned long m_heartTime;//发送心跳的时刻

	//MOtion列表的头消息，发送xml时候需要头+图片+图片。。。图片发送完毕才能关闭对应套接口
	bool	  m_bmotionHead;//是否有列表头

	//SENSOR列表的头消息,发送xml时候需要头+图片+图片。。。图片发送完毕才能关闭对应套接口
	bool	  m_bsensorHead;
	
	enum cmd_type
	{
		CMD_OPER=0,
		CMD_MOTION,
		CMD_SENSOR,
	};


	bool m_benable;//平台是否启用
	bool m_bmotionEnable;//移动侦测报警上传是否启用
	bool m_bstartHeart;//是否允许发送心跳

	char m_agentId[13];	//代理id
	unsigned long m_port;//代理端口
	char m_ip[20];//代理ip
	
	CPUB_Lock m_PTLock;//停用的时候要删除列表内的所有内容，用来同步停用和发送时候的列表
	unsigned long m_sensorpicCount;//发送传感器图片张数
	unsigned long m_motionpicCount;//发送移动侦测图片张数
	unsigned long m_sensorAlarmStartTime;//传感器报警触发抓图开始时间
	unsigned long m_motionAlarmStartTime;//移动侦测报警触发抓图开始时间
	unsigned long m_maxpicdelayTime;//图片发送延迟最大时间,当前时间-触发报警时间>发送延迟最大时间则停止发送

	LONGLONG m_triggerTime;
	LONGLONG m_chnnBits;
};


#endif

