// MessageMan.h: interface for the CMessageMan class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MESSAGEMAN_H__97139952_AFF3_4EDE_B36A_FB43BEE0B1AC__INCLUDED_)
#define AFX_MESSAGEMAN_H__97139952_AFF3_4EDE_B36A_FB43BEE0B1AC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <list>
#include <map>
#include "PUB_common.h"
#include "FrameData.h"
#include "NetProtocol.h"
#include "FBInterface.h"

//#ifndef NDEBUG
#include "RunLogItemDef.h"
#include "RunLog.h"
//#endif

//支持RTP流传输的宏开关, __STREAM_SERVER_RTP__
//支持TUTK流传输的宏开关, __TUTK_SERVER__

typedef	struct _message_data
{
	unsigned long	clientID;
	unsigned long	deviceID;
	char*			pData;
	int				dataLen;
}MESSAGE_DATA;

#ifdef __SUPPORT_OSDTWOLINES__
typedef struct _point_data
{
	unsigned long x;
	unsigned long y;
}POINT;
#endif


typedef enum _chnn_status_type
{
	CHNN_STATUS_MOTION,
	CHNN_STATUS_SENSOR_IN,
	CHNN_STATUS_VIDEO_LOSS,
	CHNN_STATUS_ALARM_OUT,
	CHNN_STATUS_MANUAL_REC,
	CHNN_STATUS_SCHEDULE_REC,
	CHNN_STATUS_MOTION_REC,
	CHNN_STATUS_SENSOR_REC,
}CHNN_STATUS_TYPE;

typedef enum _disk_status_type
{
	DISK_STATUS_NORMAL,
	DISK_STATUS_INSERT_USB,
	DISK_STATUS_REMOVE_USB,
	DISK_STATUS_DISK_FULL,
	DISK_STATUS_INSERT_ESATA,
	DISK_STATUS_REMOVE_ESATA,
}DISK_STATUS_TYPE;

typedef enum _net_server_type
{
	TVT_NET_SERVER,
	GB28181_NET_SERVER,
	RTP_NET_SERVER,
	TUTK_NET_SERVER,
	API_SERVER,
	NET_SERVER_NUM,
}NET_SERVER_TYPE;

typedef struct _msg_to_client_
{
	std::map<long, long>	clientMap;//第一个是客户ID，第二个是NET_SERVER_TYPE
	std::list<MESSAGE_DATA>	msgList;
}MSG_TO_CLIENT;

class CMessageMan  
{
public:
	static CMessageMan * Instance();
	~CMessageMan();

	bool Initial(unsigned char videoInputNum, unsigned char sensorInNum);
	void Quit();

	//////////////////////////////////////////////////////////////////////////
#ifdef __SUPPORT_OSDTWOLINES__
	void SetLiveCamPos(unsigned char chnn, unsigned long x, unsigned long y);
	void GetLiveCamPos(unsigned char chnn, unsigned long& x, unsigned long& y);
#endif


	void SetChannelVisible(bool bVisible){m_bChannelVisible = bVisible;}
	bool VisibleChannel(){return m_bChannelVisible;}

	void SetCheckPassword(bool bCheck){m_bCheckPassword = bCheck;}
	bool CheckPassword(){return m_bCheckPassword;}
	bool RefreshLive();
	void RefreshLiveTrue() { m_bRefreshLive = true;}
	bool ShowTime(){return m_bShowTime;}
	void SetShowTime(bool bShow){m_bShowTime = bShow;}

	void SetPointToMainFrame(unsigned long *pMainFrame);
	bool CheckFreezeScreenSave();

	void RefreshCamName(unsigned char chnn, const char *pName);
	bool GetCamName(unsigned char chnn, std::string & name);
	void SetShowName(unsigned char chnn, bool bShow);
	bool ShowName(unsigned char chnn){return (0 == (m_showName & ((ULONGLONG)(0x01) << chnn))) ? false : true;}
	bool CheckChangeCamName(){return m_bCamNameChange;}
	void SetChangeCamName(bool bChange){ m_bCamNameChange = bChange;}
	void RefreshOSDName(unsigned long chnn);

	void RefreshSensorName(unsigned char chnn, const char *pName);
	bool GetSensorName(unsigned char chnn, std::string & name);

	void SetAudioChnn(unsigned char chnn){m_audioChannel = chnn;}
	unsigned char AudioChnn(){return m_audioChannel;}

	void PBShowCamNameCH(ULONGLONG CH){m_PBShowCamNameCH = CH;}
	bool PBShowCamName(unsigned char chnn){return (0 == (m_PBShowCamNameCH & ((ULONGLONG)(0x01) << chnn))) ? false : true;}

#if defined(__ASYN_PLAY__)	
	void PBShowSpeed(PLAYBACK_SPEED_INFO speedInfo);
	unsigned long GetShowSpeed(unsigned long chnn){return m_showSpeed[chnn];}
#endif

	void RefreshMotion(ULONGLONG CH){m_motionCH = CH;}
	ULONGLONG Motion(){return m_motionCH;}

	void SetSnapTipPos(unsigned long x, unsigned long y);
	void GetSnapTipPos(unsigned long& x, unsigned long& y);
	void SetShowSnapTip(bool bShow = false, bool bSucc = true);
	void ShowSnapTip(bool& bShow, bool& bSucc);

	void RefreshDisplay(ULONGLONG CH);
	void RefreshRecStatus(unsigned char type, unsigned long chnn);
	unsigned long GetRecStatus(unsigned char chnn);
	void RefreshCHStatus(CHNN_STATUS_TYPE type , ULONGLONG value);
	bool GetCHStatus(CHNN_STATUS_TYPE type , ULONGLONG & value);
	inline bool Recording(){return (0 != (m_recManual | m_recSchedule | m_recMotion | m_recSensor));}
	void RefreshDiskStatus(DISK_STATUS_TYPE type);
	DISK_STATUS_TYPE GetDiskStatus();

	void SetRecycleRec(bool bRecycle) {m_bRecycleRec = bRecycle;}
	bool ShowDiskAlarm(){return (!m_bRecycleRec && m_bShowDiskAlarm);}
	void SetShowDiskAlarm(bool bShow) {m_bShowDiskAlarm = bShow;}
	bool IsDiskError(){return m_bDiskError;}
	bool SetDiskError(bool bDiskError){m_bDiskError = bDiskError;}

	void SetCurrPTZChnn(unsigned char chnn) {m_currPTZchnn = chnn;}
	unsigned char CurrPTZChnn() {return m_currPTZchnn;}
	void SetCurrPTZSpeed( BYTE ptzSpeed ){ m_currPTZSpeed = ptzSpeed;}
	BYTE CurrPTZSpeed(){return m_currPTZSpeed;}
	////////////////////////////////////////////////////////
	void RefreshFormatProgress(int allDiskNum, int curDiskNum, int precent);
    void GetFormatProgress(int *pAllDiskNum, int *pCurDiskNum, int *precent);
	void RefreshBackupProgress(int precent);
	void GetBackupProgress(int *precent);
	////////////////////////////////////////////////////////

	//
	void PutFrame(CFrameData * const pFrameData);
	bool GetFrame(CFrameData **ppFrameData, unsigned long stream, long netServerType = TVT_NET_SERVER);
	void ClearFrame(unsigned long stream);
	unsigned long GetFrameNum(unsigned long stream, long netServerType = TVT_NET_SERVER);
	bool IsFrameEmpty();
#ifdef __SUPPORT_WAP__
	bool GetFrameForWebServer(CFrameData **ppFrameData, unsigned long stream);
#endif

	void PutPBFrame(CFrameData * const pFrameData);
	bool GetPBFrame(CFrameData **ppFrameData, long netServerType = TVT_NET_SERVER);
	void ClearPBFrame(long clientID, unsigned long streamID);

	void PutTalkWayFrame(const FRAME_INFO_EX & frameInfo);
	bool GetTalkWayFrame(FRAME_INFO_EX & frameInfo, long netServerType = TVT_NET_SERVER);
	void ClearTalkWayFrame();

	void AddClientID(unsigned long clientID, long netServerType = TVT_NET_SERVER)
	{
		assert(netServerType < NET_SERVER_NUM);

		m_toServerLock.Lock();
		if(clientID != LOCAL_CLIENT_ID)
		{
			m_toClientMsg[netServerType].clientMap[clientID] = netServerType;
		}
		m_toServerLock.UnLock();
	}
	//
	void PutMsgToServer(const MESSAGE_DATA & msgData, long netServerType = TVT_NET_SERVER);
	void PutMsgToClient(const MESSAGE_DATA & msgData);

	bool GetMsgFromServer(MESSAGE_DATA & msgData, bool bNetServer = true, long netServerType = TVT_NET_SERVER);
	bool GetMsgFromClient(MESSAGE_DATA & msgData);

	void Clear();
	void CleanMsg(unsigned long clientID);	//清空指定客户端的所有未处理完的消息

	//发送消息到服务器 ,用此函数发送的消息， 服务器端会返回结果的，等待服务器端得消息。
	void SendMsgDataToServer(unsigned long clientID, unsigned long deviceID, long cmdType, unsigned char *pData, unsigned long dataLen, MESSAGE_DATA &msgData);
	//发送消息到服务器 ,用此函数发送的消息， 服务器端不会返回结果的。
	void PostMsgDataToServer(unsigned long clientID, unsigned long deviceID, long cmdType, unsigned char *pData, unsigned long dataLen);
    //释放空间
	void ReleaseMsgBuff(MESSAGE_DATA & msgData);

	//网络数字信号状态,bit为1表示断线状态
	void RefreshNetDeviceStatus(ULONGLONG ullStatus);
	void SetNetDeviceStatus(bool bOnLine, unsigned long channel);
	ULONGLONG GetNetDeviceStatus();

	//POE设备在线状态，bit为1表示在线
	void RefreshPoeOnlineStatus(ULONGLONG ullStatus);
	void SetNetPoeDeviceStatus(bool bOnLine, unsigned long channel);
	ULONGLONG GetPoeOnlineStatus();

	unsigned long AllocNetClientID();

private:
	CMessageMan();
	//////////////////////////////////////////////////////////////////////////
#ifdef __SUPPORT_OSDTWOLINES__
	POINT					*m_pCamPoint;
#endif
	bool			m_bChannelVisible;
	bool			m_bCheckPassword;
	bool			m_bRefreshLive;		//当通道名称、是否显示名称、录像状态、视频丢失等发生变化时，需要刷新现场。
	bool			m_bCamNameChange;
	bool			m_bShowTime;
	bool			m_bRecycleRec;

	bool			m_bShowSnapTip;
	unsigned long	m_xPosSnapTip;
	unsigned long	m_yPosSnapTip;
	bool			m_bSuccSnapTip;

	unsigned char	m_videoInputNum;
	unsigned char	m_sensorInNum;

	unsigned char	m_audioChannel;

	ULONGLONG	m_motionCH;//及时移动侦测结果，未平滑处理的值
	ULONGLONG	m_motion, m_sensorIn, m_videoLoss, m_alarmOut;
	ULONGLONG	m_recManual, m_recSchedule, m_recMotion, m_recSensor;

	ULONGLONG	m_PBShowCamNameCH;
	ULONGLONG	m_displayCH;

	//网络数字信号设备状态
	ULONGLONG	m_ullNetDeviceStatus;
	ULONGLONG	m_ullPoeOnlineStatus;
	
	ULONGLONG				m_showName;
	std::string				*m_pstrCamName;
	std::string				*m_pStrSensorName;
#if defined(__ASYN_PLAY__)
	unsigned long			m_showSpeed[32];
#endif

	std::list<DISK_STATUS_TYPE>	m_diskStatusList;

	std::list<CFrameData *>	m_mainFrameInfoList;
	std::list<CFrameData *>	m_subFrameInfoList;
	std::list<CFrameData *>	m_pbFrameInfoList;
	std::list<CFrameData *>	m_jpegFrameInfoList;
#ifdef __SUPPORT_WAP__
	std::list<CFrameData *>	m_jpegFrameInfoForWebList;
#endif
	std::list<CFrameData *> m_talkFrameInfoList;
	std::list<FRAME_INFO_EX>	m_talkWayFrameList;
#ifdef __STREAM_SERVER_RTP__
	std::list<CFrameData *>	m_mainRTPFrameList;
	std::list<CFrameData *>	m_subRTPFrameList;
	std::list<CFrameData *>	m_pbRTPFrameList;
	std::list<CFrameData *>	m_jpegRTPFrameList;
	std::list<CFrameData *> m_talkRTPFrameList;
#endif
#ifdef __TUTK_SERVER__
	std::list<CFrameData *>	m_mainTUTKFrameList;
	std::list<CFrameData *>	m_subTUTKFrameList;
// 	std::list<CFrameData *>	m_pbTUTKFrameList;
// 	std::list<CFrameData *>	m_jpegTUTKFrameList;
// 	std::list<CFrameData *> m_talkTUTKFrameList;
#endif
#ifdef __API_SERVER__
	std::list<CFrameData *>	m_mainDevAPIFrameList;
	std::list<CFrameData *>	m_subDevAPIFrameList;
	std::list<CFrameData *>	m_pbDevAPIFrameList;
	std::list<CFrameData *>	m_jpegDevAPIFrameList;
	std::list<CFrameData *> m_talkDevAPIFrameList;
#endif
	CPUB_Lock				m_mainFrameListLock;
	CPUB_Lock				m_subFrameListLock;
	CPUB_Lock				m_pbFrameListLock;
	CPUB_Lock				m_jpegFrameListLock;
	CPUB_Lock				m_talkFrameListLock;
	CPUB_Lock				m_talkWayFrameListLock;

	CPUB_Lock				m_toServerLock;
	CPUB_Lock				m_toClientLock;
	CPUB_Lock				m_localClientLock;

	std::list<MESSAGE_DATA>	m_toServerMsgList;
	std::list<MESSAGE_DATA>	m_toUIMsgList;
	MSG_TO_CLIENT			m_toClientMsg[NET_SERVER_NUM];

	GUI::CFBInterface *m_pFBInterface;
	////////////////////////////////////////////////////////////////
	int  m_allDiskNumForFormat;
	int  m_curDiskNumForFormat;
	int  m_precentOfFormat;
	int  m_precentOfBackup;
	////////////////////////////////////////////////////////////////
	unsigned char m_currPTZchnn;
	BYTE		  m_currPTZSpeed;

	bool          m_bShowDiskAlarm;
	bool		  m_bDiskError;

	unsigned long *m_pMainFrame;

	//给网络客户端统一分配客户ID
	static unsigned long		m_sNextClientID;
	static CPUB_Lock			m_clientIDLock;

/*******************************zxx新加hxht语音对讲需要函数************************************************/
public:
	void PutHxhtTalkWayFrame(const FRAME_INFO_EX & frameInfo);
	bool GetHxhtTalkWayFrame(FRAME_INFO_EX & frameInfo);
	void ClearHxhtTalkWayFrame();
private:
	CPUB_Lock	m_talkWayHxhtListLock;
	std::list<FRAME_INFO_EX>	m_talkWayHxhtList;
/*********************************************************************************************************/

};

#endif // !defined(AFX_MESSAGEMAN_H__97139952_AFF3_4EDE_B36A_FB43BEE0B1AC__INCLUDED_)


////////////////////////////////////////////////////


