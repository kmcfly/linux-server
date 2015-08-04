/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : zhl  
** Date         : 2010-07-28
** Name         : RequestDvrProcess .h
** Version      : 1.0
** Description  :
				1：把DVR的请求命令生成hxht网络消息
				2：把hxht网络请求消息解析为DVR命令并发给DVR主线程处理，解析DVR返回的数据，并生成hxht网络请求的应答

** Modify Record: 

***********************************************************************/

#ifndef _REQUEST_DVR_PROCESS_H_
#define _REQUEST_DVR_PROCESS_H_

#include "HxConnectMan.h"
#include "HxGlobal.h"
#include "PfHxDefine.h"
#include "tinyxml.h"
#include "MessageMan.h"
#include "HxGetSetConfig.h"
#include <map>
#include <string.h>
using namespace std;

#include "CharCodeTable.h"

const char SchemeInfoPath[] = "./schemeInfo";
const char SchemeFileNames[] = "schemeFileNames.xml";


//互信互通摄像头基本属性，在注册时由注册服务器返回
typedef struct _hx_camera_basic_info
{
	char szID[64];
	char szDeviceID[64];
	char szDomainName[64];
	char szName[128];
	int	chnnNum;
	int hasPan;
	int IsIpCamera;
	unsigned long port;
	unsigned long number;
}HX_CAMERA_BASIC_INFO;

typedef struct _hx_sensor_basic_info_
{
	char szID[64];
	char szDeviceID[64];
	char szName[128];
	int	 chnnNum;
}HX_SENSOR_BASIC_INFO;

typedef enum _net_cmd_type_
{
	NET_CMD_TYPE_NONE = 0,							//错误的网络命令
	NET_CMD_TYPE_NONE_DVR_PROCESS,					//不需要DVR来处理的命令
	NET_CMD_TYPE_WAIT_DVR_PROCESS,					//需要DVR来处理，并且DVR处理后有返回的命令， 这时候需要等待DVR处理后才能生成应答的命令
	NET_CMD_TYPE_NONE_DVR_REPLAY,					//需要DVR来处理，并且DVR处理后没有应答的命令，需要在给DVR送出请求后立即生成HXHT的应答
}NET_CMD_TYPE;

typedef enum _hx_alarm_device_type_
{
	HX_ALARM_DEVICE_TYPE_CAMERA = 0,
	HX_ALARM_DEVICE_TYPE_SENSOR_IN = 1,
	HX_ALARM_DEVICE_TYPE_SENSOR_OUT = 2,
}HX_ALARM_DEVICE_TYPE;

class CReqDVRProcess
{
public:
	static CReqDVRProcess *Instance();

	bool Initial(HX_NET_INIT_INFO hxNetInfo);
	void Quit();
	//生成DVR对HXHT网络设备的请求消息
	bool CreateHxhtRequestMsg(HX_CMD_DATA &requestCmd, const char *pParam=NULL);			

	//向DVR请求消息
	bool SendReqMsg(HX_CMD_DATA &reqDvrMsg);

	//获取DVR的应答消息
	bool GetRepMsg(HX_CMD_DATA &dvrRepMsg);	

	//
	bool DelReqInfo(SWL_socket_t sockfd);

	//检测回应的消息是否失败
	bool IsReplayFail(HX_CMD_DATA &replayCmd);
	SWL_socket_t GetCsSockFromTicket(const char *pTicket);
	SWL_socket_t GetClSockFromTicket(const char *pTicket);
	bool GetVReqInfo(VIDEO_REQUEST_INFO &videoReqInfo, const char *pTicket);
	bool DeleteTicket(VIDEO_REQUEST_INFO &videoReqInfo, const char *pTicket);
	//获取心跳周期
	inline int GetHeartCycle(){return m_hxNetMessageInfo.heartCycle;}
	//获取会话ID
	inline char* GetMessageID(){return m_hxNetMessageInfo.szMessageID;}
	inline int GetSHeartCycle(){return m_hxNetStortInfo.stortHeartCycle;}
	inline const char *GetRebootTime(){return m_hxNetMessageInfo.szSysRebootTime;}
	inline int GetRebootCycle(){return m_hxNetMessageInfo.sysRebootCycle;}
	inline const char *GetCSTime(){return m_hxNetMessageInfo.curDateTime;}
	unsigned long GetSerialNum();
	inline int GetChnnNum(const char *pCameraID);

	bool ReplayKeepAlive(char *pData, unsigned long dataLen);
	bool ReplayRegister(char *pData, unsigned long dataLen);
	bool SendMessage(unsigned long clientID, unsigned long deviceID, long cmdType, unsigned char *pData, unsigned long dataLen, MESSAGE_DATA &msgData);
	bool RepStoreRegister(const char *pData, unsigned long dataLen);
	bool RepReportAlarmToCS(const char *pData, unsigned long dataLen);		//报警通知应答
	
	bool ReplayFault(unsigned long errNum);

	inline bool AddVConnect();
	inline void DecVConnect();

	void setTalkenable(bool talkenable);
	void clearticket();

	~CReqDVRProcess();
protected:
private:
	CReqDVRProcess();

	unsigned long MakeReqMsgInfo(const char *pData, unsigned long dataLen, HX_NET_REQUEST_INFO &requestInfo, unsigned long hxSetGetConfig = 0);
	bool MakeRepCmd(const TVTHX_REQUEST_MSG_HEAD *pReHead, HX_CMD_DATA &dvrReplay, char *pRequestData, unsigned long dataLen=0);

	//故障通知
	bool Fault(char **ppCmdBuf, unsigned long &buffLen, unsigned long &dataLen, const char *pParam);
	////////////////////////////////////////////////注册和状态报告相关/////////////////////////////////////////
	//会话注册请求
	bool Register(char **ppCmdBuf, unsigned long &buffLen, unsigned long &dataLen);
	//存储注册请求
	bool StoreRegister(char **ppCmdBuf, unsigned long &buffLen, unsigned long &dataLen);
	
	//心跳注册和应答
	bool KeepAlive(char **ppCmdBuf, unsigned long &buffLen, unsigned long &dataLen, const char *pParam);

	//设备状态报告
	bool DeviceStatusReport(char **ppCmdBuf, unsigned long &buffLen, unsigned long &dataLen, unsigned long deviceType);
	
	//通知接入服务器当前报警状态
	bool ReportAlarmToCS(char **ppCmdBuf, unsigned long &buffLen, unsigned long &dataLen, const char *pParam);

	//////////////////////////////////////////////////视频相关///////////////////////////////////////////////////
	//开始视频请求
	bool StartVideoRequest(char **ppCmdBuf, unsigned long &buffLen, unsigned long &dataLen, const START_STREAM_PARAM *pStartVideoParam);
	bool CreateVideoHeader(char **ppCmdBuf, unsigned long &buffLen, unsigned long &dataLen, const char *pParam);
	


	//视频请求
	bool VideoRequest(const char *pData, HX_NET_REQUEST_INFO &requestInfo);
	bool ReplayVideoRequest(char **ppCmdBuf, unsigned long &buffLen, unsigned long &dataLen, VIDEO_REQUEST_INFO *pVideoReqParam);
	
	//客户端连接前端，请求视频， 使用视频请求时协商好的密钥
	bool ClientConnetForV(const char *pData, HX_NET_REQUEST_INFO &requestInfo);
	bool ReplayClientConnetForV(char **ppCmdBuf, unsigned long &buffLen, unsigned long &dataLen, const VIDEO_REQUEST_INFO *pVideoReqParam);
	
	//客户端关闭视频请求和应答
	bool ClientCloseV();
	bool ReplayClientCloseV();


	///////////////////////////////历史视频//////////////////////////////////////
	//历史视频下载方式请求
	bool VDLTypeReq(const char *pReqXml, const char *pDeviceID, unsigned long hxSetGetConfig);
	//历史视频下载方式应答
	bool RepVDLType(char **ppCmdBuf, unsigned long &buffLen, unsigned long &dataLen, const char *pDVRRepData, unsigned long dvrRepDataLen);
	//首个录像文件下载请求
	bool FirstHVideoFileDL(const char *pData, const char *pDeviceID, unsigned long hxSetGetConfig, HX_NET_REQUEST_INFO &requestInfo);
	bool RepFirstHVideoFileDL(char **ppCmdBuf, unsigned long &buffLen, unsigned long &dataLen);
	//后续文件下载请求
	bool NextHVideoFileDL(const char *pData, const char *pDeviceID, unsigned long hxSetGetConfig);
	bool CreatePBVideoHeader(char **ppCmdBuf, unsigned long &buffLen, unsigned long &dataLen, const char *pParam);

	bool DownloadVideo(const char *pData, HX_NET_REQUEST_INFO &requestInfo);

	///////////////////////////////云台相关//////////////////////////////////////
	bool GetPTZCtrlParam(const char *pData, int chnnNum, unsigned long hxSetGetConfig);
	bool GetPTZStatus(int chnnNum, unsigned long hxSetGetConfig);		//获取云台状态和云台配置相关信息
	bool RepGetPTZStatus(char **ppCmdBuf, unsigned long &buffLen, unsigned long &dataLen, int chnnNum, unsigned long hxSetGetConfig);


	////////////////////////////////报警请求相关////////////////////////////////////
	void SetWeekSchedule(const char *pValue, unsigned long valueLen, WEEK_SCHEDULE *pWeekSchedule);
	bool SetAlarmOutStatus(const char *pData, HX_NET_REQUEST_INFO &requestInfo, unsigned long hxSetGetConfig);
	bool GetAlarmOutStatus(const char *pData, HX_NET_REQUEST_INFO &requestInfo, unsigned int chnnNum);
	bool SetCameraAlarmInSchedule(const char *pData, HX_NET_REQUEST_INFO &requestInfo, const char *pDeviceID, unsigned long hxSetGetConfig);
	bool GetCameraAlarmInSchedule(const char *pData, HX_NET_REQUEST_INFO &requestInfo, const char *pDeviceID, unsigned long hxSetGetConfig);
	bool SetAlarmOutSchedule(const char *pData, HX_NET_REQUEST_INFO &requestInfo, unsigned long hxSetGetConfig);
	bool GetAlarmOutSchedule(const char *pData, HX_NET_REQUEST_INFO &requestInfo, unsigned long hxSetGetConfig);
	bool SetLinkage(const char *pData, unsigned long commSerial, const char *pDeviceID, unsigned long hxSetGetConfig);
	bool DelLinkage(const char *pData, const char *pDeviceID, unsigned long hxSetGetConfig);
	bool SetLinkageInfo(const char *pSchemeData, const char *pDeviceID, unsigned long hxSetGetConfig, bool bSet=true);
	bool SaveLinkageInfo(const char *pDevName, const char *pSchemeID, unsigned long commSerial, const char *pSchemeData); // 保存联动信息
	bool RemoveLinkageInfo(unsigned long commSerial = 0);							//删除联动信息

	////////////////////////////////报警请求应答相关////////////////////////////////////
	void GetWeekSchedule(char *pValue, unsigned long count, WEEK_SCHEDULE *pWeekSchedule);
	bool RepSetWeekSchedule();
	bool RepSetAlarmOutStatus(char **ppCmdBuf, unsigned long &buffLen, unsigned long &dataLen);
	bool RepGetAlarmOutStatus(char **ppCmdBuf, unsigned long &buffLen, unsigned long &dataLen, const HX_NET_REQUEST_INFO *pRequestInfo);
	bool RepGetCameraAlarmInSchedule(char **ppCmdBuf, unsigned long &buffLen, unsigned long &dataLen, unsigned long hxSetGetConfig);
	bool RepSetAlarmOutSchedule();
	bool RepGetAlarmOutSchedule();

	bool RepSetCfg(char **ppCmdBuf, unsigned long &buffLen, unsigned long &dataLen, unsigned long hxSetGetConfig);
	//生成指令序号，把当前的时间当做序号。把当前时间中的秒和毫妙作为序号， 其中毫秒存放在高12位，秒存放在低20位


	unsigned long GetXMLContent(TiXmlDocument &xmlDoc, char **ppCmdBuf, unsigned long &bufLen, bool bRequest=true);

	bool EnterConfig();
	bool ExitConfig();

	//hxht网络设备对DVR的请求
	bool SendRequestToDvr(HX_CMD_DATA clientReqHead, HX_NET_REQUEST_INFO &requestInfo);
	//dvr对HXHT设备的应答
	bool GetReplayFromDvr(HX_CMD_DATA &dvrRepMsg);

	//从设备ID 获取设备的通道号
	int GetChnnNumFromID(const char *pID, HX_ALARM_DEVICE_TYPE &deviceType);
	int GetChnnNumFromName(const char *pName, HX_ALARM_DEVICE_TYPE &deviceType);
	int GetChnnNumFromName(const char *pName);

	HX_NET_INIT_INFO		m_hxNetInfo;
	HX_NET_MESSAGE_INFO		m_hxNetMessageInfo;
	HX_NET_STORE_INFO		m_hxNetStortInfo;

	unsigned long			m_hxDeviceID;
	unsigned long			m_hxClientID;

	unsigned long			m_curVideoConnectNum;

	CPUB_Lock				m_videoReqInfoListLock;
	std::list<VIDEO_REQUEST_INFO> m_videoReqInfoList;
 
	CPUB_Lock				m_reqHeadListLock;
	std::list<HX_CMD_DATA>  m_reqHeadList;

	CPUB_Lock				m_repCmdListLock;
	std::list<HX_CMD_DATA>	m_repCmdList;

	unsigned long			m_sequenceNum;
	HX_CAMERA_BASIC_INFO	**m_ppAllCameraInfo;			//注册应答中所有摄像头信息
	HX_SENSOR_BASIC_INFO	**m_ppAllSensorInInfo;			//注册应答中所有传感器输入设备信息
	HX_SENSOR_BASIC_INFO	**m_ppAllSensorOutInfo;			//注册应答中的所有报警输出设备信息
private:
	std::list<int>			m_videoKey;//根据选择的值知道发来的命令是什么参数
	bool					m_bNeedChangeSysTime;
	unsigned long			m_sendKeepAliveTime;

//***************************************************zxx新加的部分*************************************************************************************//
private://zxx增加的部分

	//设置视频参数
	NET_CMD_TYPE SetNewVideoParam(const char *pData, HX_NET_REQUEST_INFO &requestInfo, const char *pDeviceID, unsigned long hxSetGetConfig);
	bool SetLogoTime(TiXmlElement *pElement,HX_CONFIG_INFO &cfgInfo,CHxGetSetConfig *pHxSetGetConfig,int ChnnNum);
	bool SetCover(TiXmlElement *pElement,HX_CONFIG_INFO &cfgInfo,CHxGetSetConfig *pHxSetGetConfig,int ChnnNum);
	bool SetMotion(TiXmlElement *pElement,HX_CONFIG_INFO &cfgInfo,CHxGetSetConfig *pHxSetGetConfig,int ChnnNum);
	NET_CMD_TYPE SetVideoRecordInfo(TiXmlElement *pElement,HX_CONFIG_INFO &cfgInfo,CHxGetSetConfig *pHxSetGetConfig,int ChnnNum);
	NET_CMD_TYPE SetVideoLevel(TiXmlElement *pElement,HX_CONFIG_INFO &cfgInfo,CHxGetSetConfig *pHxSetGetConfig,int ChnnNum);
	NET_CMD_TYPE SetVideoMaxBPS(TiXmlElement *pElement,HX_CONFIG_INFO &cfgInfo,CHxGetSetConfig *pHxSetGetConfig,int ChnnNum);
	NET_CMD_TYPE SetVideoPicFormat(TiXmlElement *pElement,HX_CONFIG_INFO &cfgInfo,CHxGetSetConfig *pHxSetGetConfig,int ChnnNum);
	NET_CMD_TYPE SetVideoVideoPara(TiXmlElement *pElement,HX_CONFIG_INFO &cfgInfo,CHxGetSetConfig *pHxSetGetConfig,int ChnnNum);//设置色彩
	void GetCameraCfgXY(unsigned short &x,unsigned short &y,int local);//获得配置里摄像机x和y的值.int local是由互信互通发来的位置值(1~8)
	void GetTimeCfgXY(unsigned short &x,unsigned short &y,int local);//配置里时间的x和y的值
	unsigned long GetCfgRate(int frameRate);//获得配置里的码流值
	//isX判断是不是横坐标
	unsigned short GetDVRCoverXY(unsigned short coverValue,bool isX);

	//获取视频参数
	int GetNewVideoParam(const char *pData, HX_NET_REQUEST_INFO &requestInfo, const char *pDeviceID, unsigned long hxSetGetConfig);
	bool ReplayGetVideoRecordInfo(char **ppCmdBuf, unsigned long &buffLen, unsigned long &dataLen,unsigned long hxSetGetConfig);
	bool ReplayGetVideoLevel(char **ppCmdBuf, unsigned long &buffLen, unsigned long &dataLen,unsigned long hxSetGetConfig);
	bool ReplayGetVideoMaxBPS(char **ppCmdBuf, unsigned long &buffLen, unsigned long &dataLen,unsigned long hxSetGetConfig);
	bool ReplayGetVideoPicFormat(char **ppCmdBuf, unsigned long &buffLen, unsigned long &dataLen,unsigned long hxSetGetConfig);
	bool ReplayGetVideoVideoPara(char **ppCmdBuf, unsigned long &buffLen, unsigned long &dataLen,unsigned long hxSetGetConfig);
	bool ReplayGetLogoTime(char **ppCmdBuf, unsigned long &buffLen, unsigned long &dataLen,unsigned long hxSetGetConfig);
	bool ReplayGetMotion(char **ppCmdBuf, unsigned long &buffLen, unsigned long &dataLen,unsigned long hxSetGetConfig);
	bool ReplayGetCover(char **ppCmdBuf, unsigned long &buffLen, unsigned long &dataLen,unsigned long hxSetGetConfig);
	unsigned short ChangeHtHxXY(unsigned short keyValue,bool iskeyValue);//转换为与互通互信分辨率一致的坐标,isX判断是不是横坐标
	void GetResolution(unsigned long dwResolution,bool *support);//读取实际支持的分辨率
	
	int GetPlace(int x,int y);
	
	//远程设置视频服务器参数
	int SetServerParam(const char *pData, HX_NET_REQUEST_INFO &requestInfo, const char *pDeviceID, unsigned long hxSetGetConfig);
	bool SetNetWork(TiXmlElement *pElement,HX_CONFIG_INFO &cfgInfo,CHxGetSetConfig *pHxSetGetConfig);//设置网络参数
	void SetReboot();
	//远程获取视频服务器参数
	bool RePlayGetVersion(char **ppCmdBuf, unsigned long &buffLen, unsigned long &dataLen,unsigned long hxSetGetConfig);
	bool RePlayGetNetWork(char **ppCmdBuf, unsigned long &buffLen, unsigned long &dataLen,unsigned long hxSetGetConfig);
	bool ReplayGetDiskInfo(char **ppCmdBuf, unsigned long &buffLen, unsigned long &dataLen,unsigned long hxSetGetConfig);
	bool ReplaySucc(char **ppCmdBuf, unsigned long &buffLen, unsigned long &dataLen);//接入服务器设置成功后的返回
	//获得回复版本需要的编译时间，日期和版本
	//char *software 软件版本
	//unsigned long buildDate 编译日期
	void GetSysInfo(char *software,unsigned long&buildDate);

	//接收接入服务器发来的验证信息并保存
	int GetCheckTicket(const char *pData, HX_NET_REQUEST_INFO &requestInfo, const char *pDeviceID, unsigned long hxSetGetConfig);
	//回复接入服务器开放的端口号
	bool ReplayPort(char **ppCmdBuf, unsigned long &buffLen, unsigned long &dataLen,unsigned long hxSetGetConfig);

	//接收接入服务器第二次发来的验证信息并和第一次存放的信息进行比较
	int GetCmdTicket(const char *pData, HX_NET_REQUEST_INFO &requestInfo, const char *pDeviceID, unsigned long hxSetGetConfig);

	//回应语音参数
	bool ReplayAudioParam(char **ppCmdBuf, unsigned long &buffLen, unsigned long &dataLen,unsigned long hxSetGetConfig);

	void recvAudio(const char *pData, HX_NET_REQUEST_INFO &requestInfo, int audiolen);
private:
	int m_SetServerResult;//返回远程设置视频的结果
private:
	string  m_ticket;
	TVTHX_STREAM_HEAD pAudioParam;
	char *m_frameBuf;
	int m_frameLen;
	bool m_talkenable;
};

bool CReqDVRProcess::AddVConnect()
{
	if (m_curVideoConnectNum < m_hxNetInfo.dwMaxLink)
	{
		m_curVideoConnectNum++;
		return true;
	}
	else
	{
		printf("%s:%s:%d, m_curVideoConnectNum=%d, m_hxNetInfo.dwMaxLink=%d\n", __FUNCTION__, __FILE__, __LINE__, m_curVideoConnectNum, m_hxNetInfo.dwMaxLink);
		return true;
	}
}

void CReqDVRProcess::DecVConnect()
{
	if (0 < m_curVideoConnectNum)
	{
		m_curVideoConnectNum--;

	}
}

#endif /*_REQUEST_DVR_PROCESS_H_*/
