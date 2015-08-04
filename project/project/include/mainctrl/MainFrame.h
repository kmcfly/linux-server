/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : yuanshiwei
** Date         : 2008-11-28
** Name         : MainFrame.h
** Version      : 1.0
** Description  :
** Modify Record:
1:
***********************************************************************/
#ifndef _MAIN_FRAME_H_
#define _MAIN_FRAME_H_

#include "ConfigEx.h"
#include "UserMan.h"
#include "LocalDevice.h"
#include "DVRTimer.h"
#include "MessageMan.h"
#include "LogMan.h"
#include "ReclogManEx.h"
#include "StreamProc.h"
#include "StreamRecordManEx.h"
#include "DataSerProc.h"
#ifndef __ENVIRONMENT_WIN32__
#include "UpnpMan.h"
#endif
#if !defined(__ASYN_PLAY__)
#include "PlaybackManager.h"
#else
#include "PlaybackManagerAsyn.h"
#endif

#ifdef __NETSERVER_HXHT__
	#include "HxNetReader.h"
	#include "HxConnectMan.h"
#endif

#include "NetReader.h"


#include "BackupMan.h"

#include "MotionMan.h"
#include "SensorInMan.h"
#include "VideoLossMan.h"
#include "AlarmOutMan.h"
#include "AlarmMan.h"
#include "LanguagePack.h"
#include "PinYinInput.h"
#include "DisplayMan.h"

#include "ConfigSetMan.h"
#include "ConfigBlock.h"
#include "PtzMan.h"
#ifdef __ENVIRONMENT_LINUX__
#include "FakeCurise.h"
#endif

#if defined(__DVD_BURN_REC__) && defined (__ENVIRONMENT_LINUX__)
#include "DispatcherMan.h"
#endif


#ifdef __DVR_ULTIMATE__
#include "NdLibMan.h"
#include "NetDeviceManager.h"
#include "RegNetDeviceLog.h"
#include "ConfigNetDeviceMan.h"
#endif

#ifdef __ENVIRONMENT_LINUX__
#include "TZoneDZoneCfg.h"
#include "DiskManager.h"
#include "CDOperationMan.h"
#include "DdnsManager.h"
#include "AllocNICIP.h"
#ifdef __CUSTOM_IPV6__
#include "AllocNICIP6.h"
#endif

#ifdef __DVR_BASIC__
#include "SmtpMan.h"
#include "SnapCapabilityMan.h"
#include "SnapPictureMan.h"
#include "SnapMan.h"
#endif

#else
#include "SmtpMan.h"
#endif

#ifdef __MOBILE_DVR__
#include "SpeedMan.h"
#endif


#include "ExportConfig.h"

#ifdef __SUPPORT_HRSST__
#include "PhoneServer.h"
#endif
//zxx新加入
#include "CharCodeTable.h"

#ifdef __CUSTOM_YUANZHENG__
#include "YZRecord.h"
#include "YuanZhengClinet.h"
#include "MANUALDVRTimer.h"
#include "YZSnapMan.h"
#endif

#ifdef __SUPPORT_SNAPD1__
#include "ShanghaiSnapMan.h"
#include "ShanghaiConnectMan.h"
#endif

#ifdef __ALARM_TO_UPDATE_FTP_VIDEO__
#include "FtpLoadMan.h"
#endif

#include "PushMan.h"

typedef union _digiwater_un
{
	struct  
	{
		unsigned long devicetype;
		unsigned char mac[6];
		unsigned char channel;
		unsigned char resver[5];
	};
	unsigned char water[16];
}DIGIWATERU;

class CMainFrame
{
public:
	//interface
	CMainFrame();
	~CMainFrame();
	bool Initial(const char *pWorkPath, const char *pDiskPath = NULL, unsigned long subProductID=0);
	void Quit();

	bool Start();
	void Stop();

#ifdef __ENVIRONMENT_LINUX__
	void SetResolution(int outdev, unsigned short width, unsigned short height, bool bActive = true);
#else
	void SetDisplayDevInfo(const DISPLAY_DEV_INFO* pDispDevInfo, int num, bool bActive = true);
#endif

	bool CheckFreezeScreenSave();

	void FirstCheckVIOver();

	//////////////////////////////////////////////////////////////////////////
	static int ProcCallBackMsg(void* pObject, int module, int iMsg, void *pParam1, void *pParam2);

	bool SetEncodeAgain();
	void RefreshOSDName(unsigned long chnn)
	{
		if (NULL != m_pConfigSetMan)
		{
			m_pConfigSetMan->RefreshOSDName(chnn);
		}
	}
	unsigned long ConvertVideoSize( unsigned long vs);
protected:
	//methods

	//variable

private:
	//methods
	void Repair();
	bool SetupConfig();
	//////////////////////////////////////////////////////////////////////////
	int ProcCallBackMsg(int module, int iMsg, void *pParam1, void *pParam2);

	//////////////////////////////////////////////////////////////////////////
#ifdef __ENVIRONMENT_LINUX__
	void SetDiskManInitInfo(DISK_INIT_INFO &diskInitInfo);
	void GetDiskManInfo(CDiskManager *pDiskMan, DISK_LOG_INFO **pPartitionInfo, int &partionNum, ULONG &diskStatus);
#endif

	//////////////////////////////////////////////////////////////////////////
	int FormatDisk(DVR_DISK_INFO *pDiskInfo, int num);
	static PUB_THREAD_RESULT PUB_THREAD_CALL FormatDiskThread(void *pParam);
	int FormatDiskProc();
	void StopFormatDisk();

	//////////////////////////////////////////////////////////////////////////
	int ChangeDiskProperty(DVR_DISK_INFO *pDiskInfo, int num);
	static PUB_THREAD_RESULT PUB_THREAD_CALL ChangeDiskThread(void *pParam);
	int ChangeDiskProperty();
	void StopChangeDisk();

	//////////////////////////////////////////////////////////////////////////
	static PUB_THREAD_RESULT PUB_THREAD_CALL MessageThread(void *pParam);
	int MessageProc();
	void MessageProc(const MESSAGE_DATA & msgData);
	void NetMessageProc(const MESSAGE_DATA &msgData);
	NET_PROTOCOL Protocol(const MESSAGE_DATA & msgData);
	void MsgData(MESSAGE_DATA &msgData, unsigned long clientID, long cmdType, unsigned char *pData, unsigned long dataLen);
	
	//////////////////////////////////////////////////////////////////////////
	void WriteLog(unsigned long clientID, LOG_TYPE type);
	void WriteDiskErrorLog(DISK_HEALTH_INFO_LIST &warningList);
	void WriteTimeErrorLog(unsigned long errorType);

	//判断是否在要进行配置时区。
	bool IsSetTzoneConfig(const unsigned char *pDataBlock, unsigned long dataLen);
#ifdef __MOBILE_DVR__
	//wifi用来临时测试用的
	int SearchSignalForWIFI(int *pCount, NET_WIFI_MODEM *pWifiModem);
	int TestWIFIConnect(WIFI_CONFIG &wificfg);
	CSpeedMan		*m_pSpeedMan;
#endif

	//看门狗	
	static PUB_THREAD_RESULT PUB_THREAD_CALL FeedDogThread(void *pParam);
	int FeedDogProc();
	static PUB_THREAD_RESULT PUB_THREAD_CALL TimerThread(void *pParam);
	int TimerProc();
	
	//VI自动检测
	static PUB_THREAD_RESULT PUB_THREAD_CALL CheckVIThread(void *pParam);
	int CheckVIProc();

	//磁盘检测
	static PUB_THREAD_RESULT PUB_THREAD_CALL DiskDetectThread(void *pParam);
	int DiskDetectProc();
	//网络检测
	static PUB_THREAD_RESULT PUB_THREAD_CALL NetworkThread(void *pParam);
	int NetworkProc();

	//设置水印
	void SetWaterMark();

	bool CheckNetUpdate(char *pPath, char *pDevName);

	void SetStreamLimit(bool bRequestStream);
	void CheckVI();

#ifdef __DVR_ULTIMATE__
	//由厂家ID获取IPC厂家名称
	const char *GetIPCManufacturerName(unsigned long mid);
#endif
//	bool SetUpChannelVisible();
private:
	//利用客户端传来的ENCODE_INFO_EX（或ENCODE_INFO）,建立ENCODE_INFO（或ENCODE_INFO_EX）并保存
	//参数： const NCFG_ITEM_HEAD *pItemHead  -- 解析出的itemhead
	//		 pBlockData -- 消息头（msgData+sizeof(PACKCMD)）
	//		 readPos  -- 消息中数据的读取位标  itemId -- 消息的id
	bool CreateEncodeCfg(const NCFG_ITEM_HEAD *pItemHead,unsigned char *pBlockData,unsigned long readPos,unsigned long itemId,unsigned long clientID = LOCAL_DEVICE_ID);//接收从客户端发来的EX或者
	void UpdateBKC();//升级保留配置，读取原major的东西合成到Major_Ex中
	void UpdateCVBS();//从无状态转换到CVBS,SPOT时，可能会修改帧率（major，Major_Ex）
	//////////////////////////////////////////////////////////////////////////
	char			*m_pDiskPath;
	CProduct		*m_pProduct;
	CConfigEx		*m_pConfig;
	CDVRTimer		*m_pDVRTimer;
	CUserMan		*m_pUserMan;
	CMessageMan		*m_pMsgMan;
	CReclogManEX	*m_pReclogMan;
	CLogMan			*m_pLogMan;
	CRecordMan		*m_pRecMan;
	CStreamRecordManEx *m_pStreamRecManEx;
#if defined(__DVD_BURN_REC__) && defined (__ENVIRONMENT_LINUX__)
	CDispatcherMan	*m_pDvdWriteMan;
#endif
	CStreamProc		*m_pStreamProc;
	CDataSerProc    *m_pDataSerProc;
#if !defined(__ASYN_PLAY__)
	CPlaybackManager  *m_pPlaybackMan;
#else
	CPlaybackManagerAsyn  *m_pPlaybackManAsyn;
#endif

#ifdef __NETSERVER_HXHT__
	CHxNetReader	*m_pHxNetReader;
#endif

	CNetReader       *m_pNetReader;

	CBackupMan       *m_pBackupMan;

	CMotionMan		*m_pMotionMan;
	CSensorInMan	*m_pSensorInMan;
	CVideoLossMan	*m_pVideoLossMan;
	COtherAlarmMan	*m_pOtherAlarm;
	CAlarmOutMan	*m_pAlarmOutMan;
	CAlarmMan		*m_pAlarmMan;

	CPtzMan			*m_pPtzMan;
#ifdef __ENVIRONMENT_LINUX__
	CFakeCurise		*m_pFakeCurise;
#endif
#ifdef __ENVIRONMENT_LINUX__
	CUpnpMan		* m_pUpnpMan;
#endif
	CConfigSetMan	*m_pConfigSetMan;
#if defined (__ENVIRONMENT_LINUX__) && defined (__DVR_BASIC__)
	CSmtpMan		*m_pSmtpMan;
	CSnapProc       *m_pSnapProc;
	CSnapCapabilityMan  *m_pSnapCapabilityMan;
	CSnapPictureMan     *m_pSnapPictureMan;
	CSnapMan            *m_pSnapMan;
#else
	CSmtpMan		*m_pSmtpMan;
#endif

#ifdef __DVR_ULTIMATE__
	CNetDeviceManager	*m_pNetDeviceMan;
	CRegNetDeviceLog	*m_pRegNetDeviceLog;
	CConfigNetDeviceMan *m_pCfgNetDeviceMan;
#endif

#ifdef __ENVIRONMENT_LINUX__
	//CPTZControl		*m_pPTZControl;
	CDiskManager    *m_pDiskMan;
	CDOperationMan  *m_pCdOperMan;
	CDdnsManager	*m_pDdnsManager;
	CAllocNICIP		*m_pallocNICIP;
#ifdef __CUSTOM_IPV6__
	CAllocNicIP6	*m_pallocNICIP6;
#endif
#endif


	CLocalDevice    *m_pLocalDeviceMan;

	CDisplayMan*		  m_pDisplayMan;
#ifdef __SUPPORT_HRSST__
	CPhoneSync      *m_pPhoneSync;
#endif
	
#ifdef __ALARM_TO_UPDATE_FTP_VIDEO__
	CFtpLoadMan		* m_pFtpLoadMan;
#endif

	CPushMan		* m_pPushMan;

	//
	unsigned char	m_localVideoInputNum;
	unsigned char	m_videoInputNum;
	unsigned char	m_audioInputNum;
	unsigned char	m_localSensorInputNum;
	unsigned char	m_sensorInputNum;
	unsigned char	m_sensorOutNum;
	unsigned char	m_maxClientNum;
	unsigned char	m_videoOutNum;
	
	unsigned char	m_netVideoInputNum;
	unsigned char	m_netAudioInputNum;

	//unsigned short	m_width;
	//unsigned short	m_height;
	//////////////////////////////////////////////////////////////////////////
	PUB_thread_t			m_msgProcID;
	bool					m_bMsgProc;
	
	//
	MESSAGE_DATA m_localMsgData;
	MESSAGE_DATA m_netMsgData;
	PACKCMD m_cmd;
	NET_DISPLAY_MODE_INFO m_displayMode;

	//用于格式化硬盘
	DVR_DISK_INFO        *m_pDiskInfoForFormat;
	int                  m_diskNumForFormat;
	PUB_thread_t		 m_formatDiskProcID;
	bool				 m_bFormatDiskProc;
	int                  m_curDiskNumForFormat;
	
	//用于改变硬盘的属性
	DVR_DISK_INFO        *m_pDiskInfoForChange;
	int                  m_diskNumForChange;
	PUB_thread_t		 m_changeDiskProcID;
	bool				 m_bChangeDiskProc;
	int                  m_curDiskNumForChange;

	//用于检测硬盘状态：是否满、是否有错误等
	PUB_thread_t		 m_diskDetectProcID;
	bool				 m_bDiskDetectProc;
	//用于检测网络状态：是否有冲突、是否断开了等
	PUB_thread_t		 m_networkProcID;
	bool				 m_bNetworkProc;

	//用于保存系统硬盘数量状态
	ULONG                m_systemDiskStatus;

	//用于保存播放器的保存路径
	char                *m_pVideoPlayerPath;
	
	//用于写日志
	//OPERATOR_LOG		m_operatorLog;		

	//用于记录当前DVR的操作
	bool                m_bOperForFormating;
	bool                m_bOperForLocalBackuping;
	bool                m_bOperForLocalPlayBackup;
	unsigned long       m_lOperForNetReading;
	bool                m_bOperForUpdating;
	unsigned long       m_bOperForUpdateClinetID;
	unsigned char       m_partionIndexForUpdate;
	char                m_partionNameForUpdate[64];

	//
	unsigned long       m_todayTimeL;
	bool                m_bTodayHaveDel;
	
	//记录是否已经读过flash中的信息。如果没有读过，就读取flash，并且保存。
	//如果读过，就从保存中获取.这样做的目的是：防止多次读取flash
	bool                m_bHaveReadFlash; 
	char                m_szHardwareVersion[36];
	char                m_szKernelVersion[64];
	char   	            m_szMCUVersion[36];

	//看门狗线程
	PUB_thread_t			m_feedDogProcID;
	bool					m_bFeedDogProc;
	PUB_thread_t			m_timerProcID;
	bool					m_bTimerProc;
	
	PUB_thread_t			m_checkVIProcID;
	bool					m_bCheckVIProc;

	bool			m_bRecording;				//表示本地正在录像
	LONGLONG		m_lastSyncRTCTime;
	LONGLONG		m_lastShowTime;
	LONGLONG		m_showTime;
	std::string		m_strTime;

#ifdef __ENVIRONMENT_LINUX__
	//DISPLAY_DEV_INFO    *m_pCurMainDisp;
	//DISPLAY_DEV_INFO    *m_pCurMinDisp;
	DISPLAY_DEV_INFO      *m_pLastVGADisp;
	DISPLAY_DEV_INFO      *m_pCVBSMainDisp;
	DISPLAY_DEV_INFO      *m_pCVBSMinDisp;
#else
	DISPLAY_DEV_INFO    *m_pCurMainDispWin32;
	DISPLAY_DEV_INFO    *m_pCurMinDispWin32;
#endif
	DIGIWATERU m_waterMark;    //当前水印信息

	//保存预录像用的是哪个分区
	ULONGLONG  m_preRecordLMask;
	ULONGLONG  m_preRecordHMask;

	//记录当前发送主码流的个数
	bool m_bSendFirstStream;

//zxx新加入
	CCharCodeTable *m_pCodeTable;

	unsigned long m_preRecordType;
#ifdef __CUSTOM_YUANZHENG__
	CYZRecord *m_pYZRec;
	CMANUALDVRTimer *m_pYZDVRTimer;//远征录像定时器
	CYZSnapMan *m_pYZSnapMan;
#endif
#ifdef __SUPPORT_SNAPD1__
	CShanghaiSnapMan *m_pShanghaiSnapMan;
	CShanghaiConnectMan *m_pShanghaiConnectMan;
#endif

//liuhao 20120420
//记录当前回放及备份状态
	bool			m_bPlayback;
	bool			m_bBackup;
	int				m_playChNum;
#ifdef __DVR_ULTIMATE__
	char			m_szManufacturerName[MID_COUNT][32];//用于指定IPCAM的厂商名
#endif

#if defined(__TW01_RILI__)
	struct tm m_lastAdjTmTime;
#endif

	bool m_bCanCheckVI;
	bool m_bFirstCheckVIOver;
	bool m_bHaveDiskLost;
};
#endif // _MAIN_FRAME_H_

