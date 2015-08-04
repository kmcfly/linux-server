/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : zhl
** Date         : 2010-07-22
** Name         : HxConnectMan.h
** Version      : 1.0
** Description  :
					主要完成的任务：
					1：交互数据接收和发送。
					2：DVR在线维持
					3：收到DVR协商好音视频传输后发送音视频数据
					
					DVR要和四类套接口交互消息
					1：接入服务器的会话端口6004
					2：接入服务器的存储心跳报告端口6005
					3：主动传输音视频时和接入服务器会话端口协商好的其他网络设备端口，包括HXHT的各种服务器、客户端。由于其IP和端口通过协商决定的，需要临时生成套接口再传输数据,
					   该类套接口主要用与音视频协商过程中DVR和音视频请求方密钥(ticket) 的确认。在协商好数据传输后该套接口就只用于音视频传输功能，
					   此时不应该在该类中管理，要交给音视频控制模块来处理
					4:被动传输的监听端口，包括视频连接端口、呼叫连接端口、升级端口、WEB访问端口

				   数据传输的方式为：
				    1：给每个套接口创建一个SEND_DATA_BUF来存放要发送到该套接口的数据，每次向SEND_DATA_BUF中存放一个HXHT命令的数据。
				    2：有数据从消息管理模块发送到网络时，先查看该套接口的SEND_DATA_BUF是否为空，不为空就等待SEND_DATA_BUF中数据发送完毕后才能把数据放到SEND_DATA_BUF中
				    3：对于某个套接口，如果SEND_DATA_BUF中有数据就发送到网络，SEND_DATA_BUF中的数据可以多次发送
					4：临时创建的套接口的SEND_DATA_BUF可以被随时销毁，但该套接口不一定被关闭
** Modify Record:

***********************************************************************/

#ifndef _DATA_COMMUNICATE_MAN_H_
#define _DATA_COMMUNICATE_MAN_H_

#include "PUB_common.h"
#include "HxGlobal.h"
#include "ReqDVRProcess.h"
#include "SWL_Public.h"
#include "SWL_ListenProcEx.h"
#include "PfHxDefine.h"
#include "HxAVDataSendMan.h"
#include "MessageMan.h"


const int REGISTER_WAIT_TIME	= 30;				//注册时等待30s没有应答就再注册
const int SOCKET_SRBUF_LEN		= 512 * 1024;		//
class CReqDVRProcess;
class CHxConnectMan
{
public:
	static CHxConnectMan *Instance();

	//初始化和接入服务器的会话端口和音视频监听端口
	bool Initial(HX_NET_INIT_INFO hxNetInitInfo);	
	void Quit();

	bool Start();
	void Stop();

	//删除所有缓存的帧	
	void ClearAllFrameData();

	//重新启动HXHT网络模块
	bool Restart(const HXHT_NET_PARAM_INFO *pNewParam, bool bLocalIPChanged);

	~CHxConnectMan();
protected:
private:
	CHxConnectMan();


	//该线程负责主动消息
	static PUB_THREAD_RESULT PUB_THREAD_CALL HxConnectManThread(void *pParam);
	void HxConnectManProc();
	//该线程负责被动消息
	static PUB_THREAD_RESULT PUB_THREAD_CALL NetServerThread(void *pParam);
	void NetServerProc();

	//创建套接口，在进行主动视频传输时需要
	static int AcceptLinkCallback(CLIENT_SOCK_INFO *pClientSockInfo, void *pParam1, void *pParam2);
	bool InitMsgSockInfo(CLIENT_SOCK_INFO *pClientSockInfo);
	void QuitMsgSockInfo();	
	inline std::list<HX_CMD_DATA *>::iterator DelMsgBuf(SWL_socket_t sockfd);


	//向接入服务器发送请求
	bool AddReqToCS(TVTHX_MSG_CMD hxCmdNum, SWL_socket_t clientSockfd, char *pParam, unsigned long paramLen);
	//获取一个请求来处理，每个处理都是阻塞的，也即每个消息的请求和应答都是阻塞的
	bool DecReqToCS(HX_DVR_REQ_CMD_INFO &dvrReqCmdInfo);
	//清除主动消息链表	
	void ClearReqCmdList();
	bool SendRequest(TVTHX_MSG_CMD requestCmd, SWL_socket_t toSockFd, const char *pParam=NULL);		//向接入服务器注册,发送心跳，状态报告等
	bool GetVideoHead(HX_CMD_DATA &hxCmdData);
	bool GetAVData(HX_CMD_DATA &sockCmdBuf);

	int SendOneCmdData(HX_CMD_DATA &hxCmdData);
	int SendDataNoblock(HX_CMD_DATA &hxCmdData);


	int RecvData(SWL_socket_t fd, char *pData, int dataLen);
	int BlockRecvData(SWL_socket_t fd, char *pData, int dataLen);
	int GetOneCmdData(HX_CMD_DATA &hxRepCmdData);
	int GetOneCmdDataNoblock(HX_CMD_DATA &hxRepCmdData);

	inline int SelectFd(fd_set * pFdSet, bool bRead);
	bool GetReplayFromNet(HX_CMD_DATA &replayCmdData,  const char *pParam=NULL);

	
	bool ChangeVideoStatus(HX_CMD_DATA &hxCmdData);
	SWL_socket_t ConnectHxNetDev(char *pIP, unsigned short port);

	void GetFrame();
	void GetPBFrame();
	void StartStreamOK(const START_STREAM_PARAM *pStartStreamParam);
	void StopStreamOK(const START_STREAM_PARAM *pStartStreamParam);

	//检测报警状态是否改变
	bool CheckAlarm(ALARM_INFO &alarmInfo);
	void SendAlarmStatus();

	void SendFaultStatus();
	bool CheckVLFault();
	bool CheckDiskFullFault();

	void GetRebootTime(const char *pRebootTime, int rebootCycle);
	void ChangeSysTime(const char *pCSTime);

	PUB_thread_t		m_HxConnectManID;
	volatile bool		m_bHxConnectManProc;

	PUB_thread_t		m_netServerID;
	bool				m_bNetServerProc;

	SWL_socket_t		m_storeSockfd;						//向接入服务器注册的连接
	SWL_socket_t		m_registerSockfd;					//向存储服务器注册的连接

	SWL_socket_t		m_maxMsgSockfd;						//记录互信互通客户端连接时产生的最大文件描述符
	unsigned long		m_sockCount;
	fd_set				m_msgSockSet;						
	std::list<SWL_socket_t> m_msgSockfdList;
	CPUB_Lock			m_msgSockSetLock;



	HX_NET_INIT_INFO	m_HxNetInitInfo;

	volatile int		m_haveRegisted;
	volatile int		m_heartCycle;
	volatile int		m_haveSRegisted;					//存储注册
	volatile int		m_SHeartCycle;						//存储注册心跳周期

	char				*m_pMessageID;
	unsigned long		m_lastKeepLiveTime;
	unsigned long		m_lastRegisteTime;
	unsigned long		m_lastSRegisteTime;

	CSWL_ListenProcEx	*m_pHxListenProc;
	CReqDVRProcess		*m_pReqDVRProcess;
	CMessageMan			*m_pMsgMan;

	CPUB_Lock			m_dvrReqCmdListLock;
	std::list<HX_DVR_REQ_CMD_INFO> m_dvrReqCmdList;

	CPUB_Lock			m_msgBufListLock;
	std::list<HX_CMD_DATA *> m_msgBufList;				//每个连接到消息端口的socket都用一个缓冲区来接收、发送命令， 在接收一个套接口的命令时不能阻塞其他的
	
	
	TVTHX_STREAM_HEAD	*m_pStreamHead;						//现场流视频头，每个通道一个头信息， 在获取音视频格式后填充
	TVTHX_STREAM_HEAD	*m_pPBStreamHead;					//回放流视频头，每个通道一个头信息， 在获取音视频格式后填充	

	ULONGLONG			m_ullStatusMotion;
	ULONGLONG			m_ullStatusSensorIn;
	ULONGLONG			m_ullStatusVideoLoss;
	ULONGLONG			m_ullStatusAlarmOut;

	unsigned long		m_sysRebootTime;					//系统重启相关
	bool				m_bHaveSynTime;

	FAULT_INFO			*m_pVLFaultInfo;					//视频丢失通知信息
	FAULT_INFO			m_diskFaultInfo;					//硬盘满的通知信息

	bool				m_bHasStoreServer;					//
};
#endif /*_DATA_COMMUNICATE_MAN_H_*/
