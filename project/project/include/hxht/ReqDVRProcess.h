/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : zhl  
** Date         : 2010-07-28
** Name         : RequestDvrProcess .h
** Version      : 1.0
** Description  :
				1����DVR��������������hxht������Ϣ
				2����hxht����������Ϣ����ΪDVR�������DVR���̴߳�������DVR���ص����ݣ�������hxht���������Ӧ��

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


//���Ż�ͨ����ͷ�������ԣ���ע��ʱ��ע�����������
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
	NET_CMD_TYPE_NONE = 0,							//�������������
	NET_CMD_TYPE_NONE_DVR_PROCESS,					//����ҪDVR�����������
	NET_CMD_TYPE_WAIT_DVR_PROCESS,					//��ҪDVR����������DVR������з��ص���� ��ʱ����Ҫ�ȴ�DVR������������Ӧ�������
	NET_CMD_TYPE_NONE_DVR_REPLAY,					//��ҪDVR����������DVR�����û��Ӧ��������Ҫ�ڸ�DVR�ͳ��������������HXHT��Ӧ��
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
	//����DVR��HXHT�����豸��������Ϣ
	bool CreateHxhtRequestMsg(HX_CMD_DATA &requestCmd, const char *pParam=NULL);			

	//��DVR������Ϣ
	bool SendReqMsg(HX_CMD_DATA &reqDvrMsg);

	//��ȡDVR��Ӧ����Ϣ
	bool GetRepMsg(HX_CMD_DATA &dvrRepMsg);	

	//
	bool DelReqInfo(SWL_socket_t sockfd);

	//����Ӧ����Ϣ�Ƿ�ʧ��
	bool IsReplayFail(HX_CMD_DATA &replayCmd);
	SWL_socket_t GetCsSockFromTicket(const char *pTicket);
	SWL_socket_t GetClSockFromTicket(const char *pTicket);
	bool GetVReqInfo(VIDEO_REQUEST_INFO &videoReqInfo, const char *pTicket);
	bool DeleteTicket(VIDEO_REQUEST_INFO &videoReqInfo, const char *pTicket);
	//��ȡ��������
	inline int GetHeartCycle(){return m_hxNetMessageInfo.heartCycle;}
	//��ȡ�ỰID
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
	bool RepReportAlarmToCS(const char *pData, unsigned long dataLen);		//����֪ͨӦ��
	
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

	//����֪ͨ
	bool Fault(char **ppCmdBuf, unsigned long &buffLen, unsigned long &dataLen, const char *pParam);
	////////////////////////////////////////////////ע���״̬�������/////////////////////////////////////////
	//�Ựע������
	bool Register(char **ppCmdBuf, unsigned long &buffLen, unsigned long &dataLen);
	//�洢ע������
	bool StoreRegister(char **ppCmdBuf, unsigned long &buffLen, unsigned long &dataLen);
	
	//����ע���Ӧ��
	bool KeepAlive(char **ppCmdBuf, unsigned long &buffLen, unsigned long &dataLen, const char *pParam);

	//�豸״̬����
	bool DeviceStatusReport(char **ppCmdBuf, unsigned long &buffLen, unsigned long &dataLen, unsigned long deviceType);
	
	//֪ͨ�����������ǰ����״̬
	bool ReportAlarmToCS(char **ppCmdBuf, unsigned long &buffLen, unsigned long &dataLen, const char *pParam);

	//////////////////////////////////////////////////��Ƶ���///////////////////////////////////////////////////
	//��ʼ��Ƶ����
	bool StartVideoRequest(char **ppCmdBuf, unsigned long &buffLen, unsigned long &dataLen, const START_STREAM_PARAM *pStartVideoParam);
	bool CreateVideoHeader(char **ppCmdBuf, unsigned long &buffLen, unsigned long &dataLen, const char *pParam);
	


	//��Ƶ����
	bool VideoRequest(const char *pData, HX_NET_REQUEST_INFO &requestInfo);
	bool ReplayVideoRequest(char **ppCmdBuf, unsigned long &buffLen, unsigned long &dataLen, VIDEO_REQUEST_INFO *pVideoReqParam);
	
	//�ͻ�������ǰ�ˣ�������Ƶ�� ʹ����Ƶ����ʱЭ�̺õ���Կ
	bool ClientConnetForV(const char *pData, HX_NET_REQUEST_INFO &requestInfo);
	bool ReplayClientConnetForV(char **ppCmdBuf, unsigned long &buffLen, unsigned long &dataLen, const VIDEO_REQUEST_INFO *pVideoReqParam);
	
	//�ͻ��˹ر���Ƶ�����Ӧ��
	bool ClientCloseV();
	bool ReplayClientCloseV();


	///////////////////////////////��ʷ��Ƶ//////////////////////////////////////
	//��ʷ��Ƶ���ط�ʽ����
	bool VDLTypeReq(const char *pReqXml, const char *pDeviceID, unsigned long hxSetGetConfig);
	//��ʷ��Ƶ���ط�ʽӦ��
	bool RepVDLType(char **ppCmdBuf, unsigned long &buffLen, unsigned long &dataLen, const char *pDVRRepData, unsigned long dvrRepDataLen);
	//�׸�¼���ļ���������
	bool FirstHVideoFileDL(const char *pData, const char *pDeviceID, unsigned long hxSetGetConfig, HX_NET_REQUEST_INFO &requestInfo);
	bool RepFirstHVideoFileDL(char **ppCmdBuf, unsigned long &buffLen, unsigned long &dataLen);
	//�����ļ���������
	bool NextHVideoFileDL(const char *pData, const char *pDeviceID, unsigned long hxSetGetConfig);
	bool CreatePBVideoHeader(char **ppCmdBuf, unsigned long &buffLen, unsigned long &dataLen, const char *pParam);

	bool DownloadVideo(const char *pData, HX_NET_REQUEST_INFO &requestInfo);

	///////////////////////////////��̨���//////////////////////////////////////
	bool GetPTZCtrlParam(const char *pData, int chnnNum, unsigned long hxSetGetConfig);
	bool GetPTZStatus(int chnnNum, unsigned long hxSetGetConfig);		//��ȡ��̨״̬����̨���������Ϣ
	bool RepGetPTZStatus(char **ppCmdBuf, unsigned long &buffLen, unsigned long &dataLen, int chnnNum, unsigned long hxSetGetConfig);


	////////////////////////////////�����������////////////////////////////////////
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
	bool SaveLinkageInfo(const char *pDevName, const char *pSchemeID, unsigned long commSerial, const char *pSchemeData); // ����������Ϣ
	bool RemoveLinkageInfo(unsigned long commSerial = 0);							//ɾ��������Ϣ

	////////////////////////////////��������Ӧ�����////////////////////////////////////
	void GetWeekSchedule(char *pValue, unsigned long count, WEEK_SCHEDULE *pWeekSchedule);
	bool RepSetWeekSchedule();
	bool RepSetAlarmOutStatus(char **ppCmdBuf, unsigned long &buffLen, unsigned long &dataLen);
	bool RepGetAlarmOutStatus(char **ppCmdBuf, unsigned long &buffLen, unsigned long &dataLen, const HX_NET_REQUEST_INFO *pRequestInfo);
	bool RepGetCameraAlarmInSchedule(char **ppCmdBuf, unsigned long &buffLen, unsigned long &dataLen, unsigned long hxSetGetConfig);
	bool RepSetAlarmOutSchedule();
	bool RepGetAlarmOutSchedule();

	bool RepSetCfg(char **ppCmdBuf, unsigned long &buffLen, unsigned long &dataLen, unsigned long hxSetGetConfig);
	//����ָ����ţ��ѵ�ǰ��ʱ�䵱����š��ѵ�ǰʱ���е���ͺ�����Ϊ��ţ� ���к������ڸ�12λ�������ڵ�20λ


	unsigned long GetXMLContent(TiXmlDocument &xmlDoc, char **ppCmdBuf, unsigned long &bufLen, bool bRequest=true);

	bool EnterConfig();
	bool ExitConfig();

	//hxht�����豸��DVR������
	bool SendRequestToDvr(HX_CMD_DATA clientReqHead, HX_NET_REQUEST_INFO &requestInfo);
	//dvr��HXHT�豸��Ӧ��
	bool GetReplayFromDvr(HX_CMD_DATA &dvrRepMsg);

	//���豸ID ��ȡ�豸��ͨ����
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
	HX_CAMERA_BASIC_INFO	**m_ppAllCameraInfo;			//ע��Ӧ������������ͷ��Ϣ
	HX_SENSOR_BASIC_INFO	**m_ppAllSensorInInfo;			//ע��Ӧ�������д����������豸��Ϣ
	HX_SENSOR_BASIC_INFO	**m_ppAllSensorOutInfo;			//ע��Ӧ���е����б�������豸��Ϣ
private:
	std::list<int>			m_videoKey;//����ѡ���ֵ֪��������������ʲô����
	bool					m_bNeedChangeSysTime;
	unsigned long			m_sendKeepAliveTime;

//***************************************************zxx�¼ӵĲ���*************************************************************************************//
private://zxx���ӵĲ���

	//������Ƶ����
	NET_CMD_TYPE SetNewVideoParam(const char *pData, HX_NET_REQUEST_INFO &requestInfo, const char *pDeviceID, unsigned long hxSetGetConfig);
	bool SetLogoTime(TiXmlElement *pElement,HX_CONFIG_INFO &cfgInfo,CHxGetSetConfig *pHxSetGetConfig,int ChnnNum);
	bool SetCover(TiXmlElement *pElement,HX_CONFIG_INFO &cfgInfo,CHxGetSetConfig *pHxSetGetConfig,int ChnnNum);
	bool SetMotion(TiXmlElement *pElement,HX_CONFIG_INFO &cfgInfo,CHxGetSetConfig *pHxSetGetConfig,int ChnnNum);
	NET_CMD_TYPE SetVideoRecordInfo(TiXmlElement *pElement,HX_CONFIG_INFO &cfgInfo,CHxGetSetConfig *pHxSetGetConfig,int ChnnNum);
	NET_CMD_TYPE SetVideoLevel(TiXmlElement *pElement,HX_CONFIG_INFO &cfgInfo,CHxGetSetConfig *pHxSetGetConfig,int ChnnNum);
	NET_CMD_TYPE SetVideoMaxBPS(TiXmlElement *pElement,HX_CONFIG_INFO &cfgInfo,CHxGetSetConfig *pHxSetGetConfig,int ChnnNum);
	NET_CMD_TYPE SetVideoPicFormat(TiXmlElement *pElement,HX_CONFIG_INFO &cfgInfo,CHxGetSetConfig *pHxSetGetConfig,int ChnnNum);
	NET_CMD_TYPE SetVideoVideoPara(TiXmlElement *pElement,HX_CONFIG_INFO &cfgInfo,CHxGetSetConfig *pHxSetGetConfig,int ChnnNum);//����ɫ��
	void GetCameraCfgXY(unsigned short &x,unsigned short &y,int local);//��������������x��y��ֵ.int local���ɻ��Ż�ͨ������λ��ֵ(1~8)
	void GetTimeCfgXY(unsigned short &x,unsigned short &y,int local);//������ʱ���x��y��ֵ
	unsigned long GetCfgRate(int frameRate);//��������������ֵ
	//isX�ж��ǲ��Ǻ�����
	unsigned short GetDVRCoverXY(unsigned short coverValue,bool isX);

	//��ȡ��Ƶ����
	int GetNewVideoParam(const char *pData, HX_NET_REQUEST_INFO &requestInfo, const char *pDeviceID, unsigned long hxSetGetConfig);
	bool ReplayGetVideoRecordInfo(char **ppCmdBuf, unsigned long &buffLen, unsigned long &dataLen,unsigned long hxSetGetConfig);
	bool ReplayGetVideoLevel(char **ppCmdBuf, unsigned long &buffLen, unsigned long &dataLen,unsigned long hxSetGetConfig);
	bool ReplayGetVideoMaxBPS(char **ppCmdBuf, unsigned long &buffLen, unsigned long &dataLen,unsigned long hxSetGetConfig);
	bool ReplayGetVideoPicFormat(char **ppCmdBuf, unsigned long &buffLen, unsigned long &dataLen,unsigned long hxSetGetConfig);
	bool ReplayGetVideoVideoPara(char **ppCmdBuf, unsigned long &buffLen, unsigned long &dataLen,unsigned long hxSetGetConfig);
	bool ReplayGetLogoTime(char **ppCmdBuf, unsigned long &buffLen, unsigned long &dataLen,unsigned long hxSetGetConfig);
	bool ReplayGetMotion(char **ppCmdBuf, unsigned long &buffLen, unsigned long &dataLen,unsigned long hxSetGetConfig);
	bool ReplayGetCover(char **ppCmdBuf, unsigned long &buffLen, unsigned long &dataLen,unsigned long hxSetGetConfig);
	unsigned short ChangeHtHxXY(unsigned short keyValue,bool iskeyValue);//ת��Ϊ�뻥ͨ���ŷֱ���һ�µ�����,isX�ж��ǲ��Ǻ�����
	void GetResolution(unsigned long dwResolution,bool *support);//��ȡʵ��֧�ֵķֱ���
	
	int GetPlace(int x,int y);
	
	//Զ��������Ƶ����������
	int SetServerParam(const char *pData, HX_NET_REQUEST_INFO &requestInfo, const char *pDeviceID, unsigned long hxSetGetConfig);
	bool SetNetWork(TiXmlElement *pElement,HX_CONFIG_INFO &cfgInfo,CHxGetSetConfig *pHxSetGetConfig);//�����������
	void SetReboot();
	//Զ�̻�ȡ��Ƶ����������
	bool RePlayGetVersion(char **ppCmdBuf, unsigned long &buffLen, unsigned long &dataLen,unsigned long hxSetGetConfig);
	bool RePlayGetNetWork(char **ppCmdBuf, unsigned long &buffLen, unsigned long &dataLen,unsigned long hxSetGetConfig);
	bool ReplayGetDiskInfo(char **ppCmdBuf, unsigned long &buffLen, unsigned long &dataLen,unsigned long hxSetGetConfig);
	bool ReplaySucc(char **ppCmdBuf, unsigned long &buffLen, unsigned long &dataLen);//������������óɹ���ķ���
	//��ûظ��汾��Ҫ�ı���ʱ�䣬���ںͰ汾
	//char *software ����汾
	//unsigned long buildDate ��������
	void GetSysInfo(char *software,unsigned long&buildDate);

	//���ս����������������֤��Ϣ������
	int GetCheckTicket(const char *pData, HX_NET_REQUEST_INFO &requestInfo, const char *pDeviceID, unsigned long hxSetGetConfig);
	//�ظ�������������ŵĶ˿ں�
	bool ReplayPort(char **ppCmdBuf, unsigned long &buffLen, unsigned long &dataLen,unsigned long hxSetGetConfig);

	//���ս���������ڶ��η�������֤��Ϣ���͵�һ�δ�ŵ���Ϣ���бȽ�
	int GetCmdTicket(const char *pData, HX_NET_REQUEST_INFO &requestInfo, const char *pDeviceID, unsigned long hxSetGetConfig);

	//��Ӧ��������
	bool ReplayAudioParam(char **ppCmdBuf, unsigned long &buffLen, unsigned long &dataLen,unsigned long hxSetGetConfig);

	void recvAudio(const char *pData, HX_NET_REQUEST_INFO &requestInfo, int audiolen);
private:
	int m_SetServerResult;//����Զ��������Ƶ�Ľ��
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
