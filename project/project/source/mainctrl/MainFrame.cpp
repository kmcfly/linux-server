/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : yuanshiwei
** Date         : 2008-11-28
** Name         : MainFrame.cpp
** Version      : 1.0
** Description  :
** Modify Record:
1:
***********************************************************************/
#include "MainFrame.h"
#include "gui.h"
#include "TimeCtrl.h"
#include "dvrdvsdef.h"
#include "device.h"

#include "EncodeCheck.h"

#ifdef __DVR_ULTIMATE__
#include "IPCDecDisplayMan.h"
#endif

#if defined(__ALARM_TO_UPDATE_FTP_VIDEO__)
#include "FTPUpdateMan.h"
#endif

#ifdef __TUTK_SERVER__
#include "TVT_IOTCDevice.h"
#endif
#include "AppInfoServManage.h"

#include <string>
#include "FuncCustom.h"

#ifdef __ENVIRONMENT_LINUX__
#include"PtzAux.h"
#include "NetInterface.h"
#endif

using namespace GUI;
//public
extern bool	g_bCloseWzd;
extern DVR_SYSTEM_STATUS g_system_status;
extern VIDEO_FORMAT g_video_format;
extern bool g_bProIdequeSubProdID;
extern bool g_upgrade_status;
extern bool g_b_rw_cfg;
extern bool g_bHaveSnapPic;
extern bool g_bLcdScreemOn;
extern bool g_curNet3G;
extern ULONGLONG g_net_channel;
extern VIDEO_SIZE_BITRATE_INFO g_vsBitRateInfo[];

CMainFrame::CMainFrame()
{
	m_pDiskPath		= NULL;
	m_pConfig		= NULL;
	m_pDVRTimer		= NULL;
#ifdef __CUSTOM_YUANZHENG__
	m_pYZRec		= NULL;
	m_pYZDVRTimer   = NULL;
	m_pYZSnapMan    = NULL;
#endif
	m_pUserMan		= NULL;
	m_pMsgMan		= NULL;
	m_pDataSerProc  = NULL;
	m_pBackupMan    = NULL;

	m_pMotionMan	= NULL;
	m_pSensorInMan	= NULL;
	m_pVideoLossMan	= NULL;
	m_pOtherAlarm	= NULL;
	m_pAlarmOutMan	= NULL;
	m_pAlarmMan		= NULL;
#ifdef __MOBILE_DVR__
	m_pSpeedMan		=NULL;
#endif

	m_pConfigSetMan = NULL;

#ifdef __DVR_ULTIMATE__
	m_pNetDeviceMan	= NULL;
	m_pRegNetDeviceLog = NULL;
	m_pCfgNetDeviceMan = NULL;
#endif

#ifdef __ENVIRONMENT_LINUX__
	//m_pPTZControl	= NULL;
	m_pDiskMan      = NULL;
	m_pCdOperMan    = NULL;
	m_pLocalDeviceMan = NULL;
#endif

#ifdef __SUPPORT_HRSST__
	m_pPhoneSync = NULL;
#endif

	//////////////////////////////////////////////////////////////////////////
	m_localVideoInputNum	= 0;
	m_videoInputNum			= 0;
	m_audioInputNum			= 0;
	m_localSensorInputNum	= 0;
	m_sensorInputNum		= 0;
	m_sensorOutNum			= 0;
	m_maxClientNum			= 0;
	m_videoOutNum			= 0;


	m_netVideoInputNum		= 0;
	m_netAudioInputNum		= 0;

	//m_width					= 800;
	//m_height				= 600;	

	//////////////////////////////////////////////////////////////////////////
	m_msgProcID		= PUB_THREAD_ID_NOINIT;
	m_bMsgProc		= false;

	memset(&m_displayMode, 0, sizeof(NET_DISPLAY_MODE_INFO));
    
	///////////////////////////////////////////////
	m_pDiskInfoForFormat = NULL;
	m_diskNumForFormat   = 0;
	m_formatDiskProcID   = PUB_THREAD_ID_NOINIT;
	m_bFormatDiskProc    = false;
	///////////////////////////////////////////////
	m_pDiskInfoForChange = NULL;
	m_diskNumForChange   = 0;
	m_changeDiskProcID   = PUB_THREAD_ID_NOINIT;
	m_bChangeDiskProc    = false;
	//////////////////////////////////////////////////////////////////////////
	m_diskDetectProcID		= PUB_THREAD_ID_NOINIT;
	m_bDiskDetectProc		= false;
	m_networkProcID			= PUB_THREAD_ID_NOINIT;
	m_bNetworkProc			= false;
    ///////////////////////////////////////////////
	m_systemDiskStatus   = 0;

	//////////////////////////////////////////////
	m_pVideoPlayerPath   = NULL;

	//memset(&m_operatorLog, 0, sizeof(OPERATOR_LOG));

	m_bOperForFormating = false;
	m_bOperForLocalBackuping = false;
	m_bOperForLocalPlayBackup = false;
	m_lOperForNetReading = 0;
	m_bOperForUpdating = false;


	m_todayTimeL = 0;
	m_bTodayHaveDel = false;

	m_bHaveReadFlash = false;

	m_feedDogProcID		= PUB_THREAD_ID_NOINIT;
	m_bFeedDogProc		= false;
	m_timerProcID		= PUB_THREAD_ID_NOINIT;
	m_bTimerProc		= false;
	
	m_checkVIProcID		= PUB_THREAD_ID_NOINIT;
	m_bCheckVIProc		= false;

	m_bRecording		= false;
	m_lastSyncRTCTime	= 0;
	m_lastShowTime		= 0;
	m_showTime			= 0;

	m_preRecordLMask    = 0;
	m_preRecordHMask    = 0;

	m_bPlayback			= false;
	m_bBackup			= false;
	m_playChNum			= 0;
	
	m_preRecordType     = PRERECORD_MODE_MEMORY;
#ifdef __DVR_ULTIMATE__
	strcpy(m_szManufacturerName[MID_UNKNOWN], "unknown");
	strcpy(m_szManufacturerName[MID_TVT], "");
	strcpy(m_szManufacturerName[MID_ONVIF], "ONVIF");
#endif

	m_bCanCheckVI = false;
	m_bFirstCheckVIOver = false;
	m_bHaveDiskLost = false;
}

CMainFrame::~CMainFrame()
{

}

bool CMainFrame::Initial(const char *pWorkPath, const char *pDiskPath/* = NULL*/, unsigned long subProductID)
{
#if defined(__TW01_RILI__)
	m_lastAdjTmTime = DVRTimeToTm(GetCurrTime());
	
	printf("%s:%s:%d: %d-%d-%d %d:%d:%d\n", __FUNCTION__, __FILE__, __LINE__, m_lastAdjTmTime.tm_year, m_lastAdjTmTime.tm_mon, m_lastAdjTmTime.tm_mday, m_lastAdjTmTime.tm_hour, m_lastAdjTmTime.tm_min, m_lastAdjTmTime.tm_sec);
#endif

	//zxx新加入
	m_pCodeTable=CCharCodeTable::Instance();
	m_pCodeTable->Initial(pWorkPath);

// #ifdef __ENVIRONMENT_LINUX__
// 	mkfifo(CLIENT_FIFO_NAME, 0660);
// #endif
    assert(pWorkPath != NULL);

	m_pDiskPath = new char [512];
	memset(m_pDiskPath, 0, 512);
	if (NULL != pDiskPath)
	{
		strcpy(m_pDiskPath, pDiskPath);
	}

	m_pProduct = CProduct::Instance();
	m_localVideoInputNum = m_pProduct->LocalVideoInputNum();
	m_videoInputNum		= m_pProduct->VideoInputNum();
	m_audioInputNum		= m_pProduct->AudioInputNum();
	m_sensorOutNum		= m_pProduct->SensorOutputNum();
	m_videoOutNum		= m_pProduct->VideoOutputNum();

	m_netVideoInputNum	= m_pProduct->NetVideoInputNum();
	m_netAudioInputNum	= m_netVideoInputNum;

	m_localSensorInputNum = m_pProduct->SensorInputNum();
	m_sensorInputNum	= m_localSensorInputNum+m_netVideoInputNum;

	//////////////////////////////////////////////////////////////////////////
	m_pConfig		= CConfigEx::Instance();
	m_pDVRTimer		= CDVRTimer::Instance();
#ifdef __CUSTOM_YUANZHENG__
	m_pYZDVRTimer   = CMANUALDVRTimer::Instance();
	m_pYZRec		= CYZRecord::Instance();
	m_pYZSnapMan    = CYZSnapMan::Instance();	
#endif
	m_pReclogMan	= new CReclogManEX;
	m_pLogMan		= CLogMan::Instance();
	m_pUserMan		= CUserMan::Instance();
	m_pMsgMan		= CMessageMan::Instance();
	m_pStreamRecManEx		=  CStreamRecordManEx::Instance();
	m_pRecMan		= new CRecordMan();
#if defined(__DVD_BURN_REC__) && defined (__ENVIRONMENT_LINUX__)
	m_pDvdWriteMan	= CDispatcherMan::Instance();
#endif
	m_pStreamProc	= new CStreamProc;
    m_pDataSerProc  = new CDataSerProc;
#if !defined(__ASYN_PLAY__)
    m_pPlaybackMan  = new CPlaybackManager;
#else
	m_pPlaybackManAsyn  = new CPlaybackManagerAsyn;
#endif

#ifdef __NETSERVER_HXHT__
	//m_pHxNetReader	= new CHxNetReader ;
	m_pHxNetReader	= CHxNetReader::Instance();
#endif
	m_pNetReader    = new CNetReader;
    m_pBackupMan    = new CBackupMan;

#if defined (__ENVIRONMENT_LINUX__) && defined (__DVR_BASIC__)
	m_pSmtpMan		= CSmtpMan::Instance();
	m_pSnapProc     = CSnapProc::Instance();
	m_pSnapCapabilityMan = CSnapCapabilityMan::Instance();
#if defined(__SUPPORT_SNAPD1__)
	m_pShanghaiSnapMan = CShanghaiSnapMan::GetInstance();
	m_pShanghaiConnectMan = CShanghaiConnectMan::Instance();
#else
	m_pSnapPictureMan    = CSnapPictureMan::GetInstance();
#endif
	m_pSnapMan           = CSnapMan::GetInstance();
#else
	m_pSmtpMan		= CSmtpMan::Instance();
#endif

	m_pMotionMan	= new CMotionMan;
	m_pSensorInMan	= new CSensorInMan;
	m_pVideoLossMan	= new CVideoLossMan;
	m_pOtherAlarm	= new COtherAlarmMan;
	m_pAlarmOutMan	= new CAlarmOutMan;
	m_pAlarmMan		= CAlarmMan::Instance();
#ifdef __MOBILE_DVR__
	m_pSpeedMan		= new CSpeedMan;
#endif
	m_pPtzMan		= CPtzMan::Instance();
#ifdef __ENVIRONMENT_LINUX__
	m_pFakeCurise	= CFakeCurise::Instance();
	m_pUpnpMan		= CUpnpMan::Instance();
#endif

	m_pConfigSetMan = new CConfigSetMan;

#ifdef __ALARM_TO_UPDATE_FTP_VIDEO__
	m_pFtpLoadMan	= CFtpLoadMan::Instance();
#endif

	m_pPushMan		= CPushMan::Instance();

#ifdef __DVR_ULTIMATE__
	if(m_netVideoInputNum > 0)
	{
		m_pRegNetDeviceLog = CRegNetDeviceLog::Instance();
		m_pCfgNetDeviceMan = CConfigNetDeviceMan::Instance();
		m_pNetDeviceMan	= CNetDeviceManager::Instance();
	}
#endif

#ifdef __ENVIRONMENT_LINUX__
	m_pDiskMan        = CDiskManager::Instance();
	//m_pPTZControl	= CPTZControl::Instance();

	m_pCdOperMan      = CDOperationMan::Instance();
	m_pDdnsManager	  = CDdnsManager::Instance();
	m_pallocNICIP	  = CAllocNICIP::Instance();
#ifdef __CUSTOM_IPV6__
	m_pallocNICIP6	  = CAllocNicIP6::Instance();
#endif
#endif

	m_pLocalDeviceMan = CLocalDevice::Instance();

	m_pDisplayMan = CDisplayMan::Instance();

#ifdef __SUPPORT_HRSST__
	m_pPhoneSync = CPhoneSync::Instance();
#endif

	//启动水印功能
	//先屏蔽到水印功能
	SetWaterMark();
	printf("Setup warter mark \n");

	//////////////////////////////////////////////////////////////////////////
	if (!m_pDVRTimer->Initial(m_videoInputNum, m_sensorInputNum, m_sensorOutNum, OTHER_ALARM_TYPE_NUM))
	{
		printf("Inital DVRT timer fail\n");
		return false;
	}
	
	bool bSupportRedun = m_pProduct->SupportRedunRec();
#ifdef __ENVIRONMENT_LINUX__
	//初始化cd设备
	if (!m_pCdOperMan->Initial(pWorkPath))
	{
		printf("Initial CD-Rom man fail\n");
		return false;
	}
	else
	{
		printf("Initial CD-Rom man succ\n");
	}

	//初始化硬盘和U盘设备
	DISK_INIT_INFO diskInitInfo;
    SetDiskManInitInfo(diskInitInfo);
	if (!m_pDiskMan->Initial(&diskInitInfo))
	{
		printf("Inital diskmanager fail\n");
		return false;
	}
	else
	{
		printf("Initial diskmanager succ\n");
	}

	DISK_LOG_INFO *pPartitionStatus = NULL;
	int allMountedPartitionNum = 0;
	m_systemDiskStatus = 0;
	GetDiskManInfo(m_pDiskMan, &pPartitionStatus, allMountedPartitionNum, m_systemDiskStatus);

	if (!m_pReclogMan->Initial(pPartitionStatus, allMountedPartitionNum, bSupportRedun, pDiskPath))
	{
		printf("Inital reclog manager fail\n");
		return false;
	}
	else
	{				
		m_pReclogMan->CreateDateList();	
		Repair();

		printf("Initial reclog manager succ\n");
	}
	
#ifdef __DVR_BASIC__
   if (g_bHaveSnapPic)
   {
	   //初始化抓图。
	   if (!m_pSnapMan->Initial(pPartitionStatus, allMountedPartitionNum, pDiskPath))
	   {
		   printf("Initial SnapMan fail file = %s, line = %d\n", __FILE__, __LINE__);
	   }
   }

#endif

	if (!m_pLogMan->Initial(pPartitionStatus, allMountedPartitionNum, pDiskPath))
	{
		printf("Inital log manager fail\n");
		return false;
	}
	else
	{
		printf("Initial log manager succ\n");
	}

	//修改硬盘状态等
	if (0 < allMountedPartitionNum)
	{
		m_pLocalDeviceMan->SetLEDStatus(LED_STATUS_HDD, true);
	}

#ifndef NDEBUG
	if (!CRunLog::Instance()->Initial(pPartitionStatus, allMountedPartitionNum, pDiskPath))
	{
		printf("Inital Run log fail\n");
		//return false;  //存在失败的情况，比如所有的硬盘都是只读属性。
	}
	else
	{
		printf("Initial Run log succ\n");
	}
#endif

	if (NULL != pPartitionStatus)
	{
		delete [] pPartitionStatus;
		pPartitionStatus = NULL;
	}

	
	//if (!m_pPTZControl->Initial(m_localVideoInputNum))
	//{
	//	printf("Inital ptz control fail\n");
	//	return false;
	//}
	//else
	//{
	//	printf("Initial ptz control succ\n");
	//}

#else
    
	int allMountedPartitionNum = 4;
	DISK_LOG_INFO *pPartitionStatus = new DISK_LOG_INFO [allMountedPartitionNum];
	for (int i = 0; i < allMountedPartitionNum; ++i)
	{
		pPartitionStatus[i].diskIndex = i;
		pPartitionStatus[i].diskOwnedBy = 0;
		pPartitionStatus[i].diskProperty = 0;
	}
	
	if (!m_pReclogMan->Initial(pPartitionStatus, allMountedPartitionNum, bSupportRedun, pDiskPath))
	{
		printf("Inital reclog manager fail\n");
		return false;
	}
	else
	{  
		m_pReclogMan->CreateDateList();
		Repair();
	}
	
	if (!m_pLogMan->Initial(pPartitionStatus, allMountedPartitionNum, pDiskPath))
	{
		printf("Inital log manager fail\n");
		return false;
	}
	

#ifndef NDEBUG
	if (!CRunLog::Instance()->Initial(pPartitionStatus, allMountedPartitionNum, pDiskPath))
	{
		printf("Inital Run log fail\n");
		//return false;  //存在失败的情况，比如所有的硬盘都是只读属性。
	}
	else
	{
		printf("Initial Run log succ\n");
	}
#endif


#endif

	if(!m_pStreamRecManEx->Initial(m_videoInputNum,m_netVideoInputNum,m_audioInputNum+m_netAudioInputNum,m_pRecMan))
	{
		printf("Inital liveStreamRecord manager fail\n");
		return false;
	}
	else
	{
		printf("Inital liveStreamRecord manager succ\n");
	}
	unsigned char virtualNetVideoInputNum = 0 ;
	if(m_pStreamRecManEx->IsSupportMinorStreamRecord())
	{
		virtualNetVideoInputNum = m_netVideoInputNum;
	}
	
	if (PRERECORD_MODE_DISK == CProduct::Instance()->GetPreRecordType())
	{
		m_preRecordType = PRERECORD_MODE_DISK;
		unsigned char prerecordStartIndex = 0;
		unsigned char prerecordParNum = 0;
#if defined (__ENVIRONMENT_LINUX__)
		m_pDiskMan->GetOneRWDiskPartionNum(prerecordStartIndex, prerecordParNum);
#endif
		printf("%s,%d,prerecordStartIndex:%d,prerecordParNum:%d,pDiskPath:%s\n",__FILE__,__LINE__,prerecordStartIndex,prerecordParNum,pDiskPath);
		if (!m_pRecMan->Initial(m_videoInputNum+virtualNetVideoInputNum, m_audioInputNum+m_netAudioInputNum,PRERECORD_MODE_DISK, m_pReclogMan, pDiskPath, prerecordStartIndex, prerecordParNum, m_pMsgMan))
		{
			printf("Inital record manager fail\n");
			return false;
		}
		else
		{
			printf("Inital record manager succ\n");
		}
		
		m_preRecordLMask = 0;
		m_preRecordHMask = 0;
		m_pRecMan->GetInitialPreDiskRecordDiskMask(m_preRecordHMask, m_preRecordLMask);	
	}
	else
	{
		m_preRecordType = PRERECORD_MODE_MEMORY;
		if (!m_pRecMan->Initial(m_videoInputNum+virtualNetVideoInputNum, m_audioInputNum+m_netAudioInputNum,PRERECORD_MODE_MEMORY, m_pReclogMan, pDiskPath, 0, 0, m_pMsgMan))
		{
			printf("Inital record manager fail\n");
			return false;
		}
		else
		{
			printf("Inital record manager succ\n");
		}
	}
	

#if defined(__DVD_BURN_REC__) && defined (__ENVIRONMENT_LINUX__)
	if (m_pDvdWriteMan->Initial(m_videoInputNum))
	{
		printf("Inital dvd dispatcher manager succ\n");
	}
	else
	{
		printf("Inital dvd dispatcher manager fail\n");
		return false;
	}
#endif

#ifdef __CUSTOM_YUANZHENG__
	if(!m_pYZDVRTimer->Initial(m_videoInputNum))
	{
		printf("Inital m_pYZDVRTimer fail\n");
		return false;
	}
	if (!m_pYZRec->Initial(m_videoInputNum,m_pRecMan))
	{
		printf("Inital YZREC fail\n");
		return false;
	}

#endif

#if defined (__ENVIRONMENT_LINUX__) && defined (__DVR_BASIC__)
	if(!m_pSnapProc->Initial())
	{
		printf("Initial m_pSnapProc fail \n");
		return false;
	}
	else
	{
		printf("Initial smtp manager succ\n");
	}
	
	if (g_bHaveSnapPic)
	{
#ifdef __SUPPORT_SNAPD1__
		if (!m_pShanghaiSnapMan->Initial(m_videoInputNum))
		{
			printf("Initial m_pSnapPictureMan fail \n");
			return false;
		}
		else
		{
			printf("Initial m_pSnapPictureMan manager succ\n");
		}

#else
		if (!m_pSnapPictureMan->Initial(m_videoInputNum))
		{
			printf("Initial m_pSnapPictureMan fail \n");
			return false;
		}
		else
		{
			printf("Initial m_pSnapPictureMan manager succ\n");
		}
#endif

	}	
#endif

	//SG平台连接
#if defined(__SUPPORT_SNAPD1__)
	m_pShanghaiConnectMan->Initial();
#endif

	if (!m_pStreamProc->Initial(m_videoInputNum, m_netVideoInputNum, m_audioInputNum, m_pReclogMan, m_pRecMan,m_pStreamRecManEx))
	{
		printf("Inital stream proc fail\n");
		return false;
	}
	else
	{
		printf("Inital stream proc succ\n");
	}

#ifdef __TUTK_SERVER__
	CTVTIOTCDevice::Instance()->SetReclogMan(m_pReclogMan);
#endif


#ifdef __DVR_ULTIMATE__
	if(m_netVideoInputNum > 0)
	{
		if( !m_pRegNetDeviceLog->Initial( pWorkPath ) )
		{
			printf("Initial net device log fail\n");
			return false;
		}
		else
		{
			printf("Initial net device log succ\n");
		}

		if(!m_pCfgNetDeviceMan->Initial(m_pConfig, m_videoInputNum, m_videoInputNum - m_localVideoInputNum, m_sensorOutNum, m_audioInputNum, m_localSensorInputNum))
		{
			printf("Initial config net device manager fail");
			return false;
		}
		else
		{
			printf("Initial config net device manager succ");
		}

		if (!m_pNetDeviceMan->Initial(m_localVideoInputNum, m_videoInputNum - m_localVideoInputNum, m_localSensorInputNum))
		{
			printf("Inital net device manager fail\n");
			return false;
		}
		else
		{
			printf("Inital net device manager succ\n");
		}

		//更新网络通道标记位g_net_channel
		{
			std::list<NET_DEVICE_LINK_INFO> tmpList;
			m_pNetDeviceMan->GetNetDeviceEffect(tmpList);

			std::list<NET_DEVICE_LINK_INFO>::iterator iter;
			for(iter = tmpList.begin(); iter !=  tmpList.end();iter++)
			{
				g_net_channel |= ((ULONGLONG)(0x1) << iter->channel);
			}

			if(g_net_channel != 0)
			{
				m_pStreamProc->SetSecondSteamRateLimit(CProduct::Instance()->GetNetLimitMaxRate());
			}

			printf("net video input num = %d. binding mask = 0x%X. \n", m_netVideoInputNum, g_net_channel);
		}
	}
#endif

#ifdef __ENVIRONMENT_LINUX__ 
	if (!m_pDataSerProc->Initial(m_pMsgMan, m_pReclogMan, m_pLogMan, m_pDiskMan))
	{
		printf("Inital data server proc fail\n");
		return false;
	}
	else
	{
		printf("Inital data server proc succ\n");
	}
#else
	if (!m_pDataSerProc->Initial(m_pMsgMan, m_pReclogMan, m_pLogMan, m_videoInputNum))
	{
		printf("Inital data server proc fail\n");
		return false;
	}
#endif

#ifdef __NETSERVER_HXHT__
	if (!m_pHxNetReader->Initial(m_pReclogMan, m_pMsgMan, m_pDiskPath))
	{
		printf("Inital HxNetReader fail\n");
		return false;
	}
	else
	{
		printf("Inital HxNetReader succ\n");
	}
#endif

	if (!m_pNetReader->Initail(m_pDiskPath, m_pReclogMan, m_pMsgMan))
	{
		printf("Inital netReader fail\n");
		return false;
	}
	else
	{
		printf("Inital netReader succ\n");
	}

	if (!m_pBackupMan->Initail())
	{
		printf("Initial backupman fail\n");
		return false;
	}
	else
	{
		printf("Initial backupman succ\n");
	}

#if /*defined (__ENVIRONMENT_LINUX__) && */defined (__DVR_BASIC__)
	if(!m_pSmtpMan->Initial())
	{
		printf("Initial smtp manager fail \n");
		return false;
	}
	else
	{
		printf("Initial smtp manager succ\n");
	}
#endif

	if (!m_pAlarmOutMan->Initial(m_sensorOutNum))
	{
		printf("Initial alarm out manager fail\n");
		return false;
	}
	else
	{
		printf("Initial alarm out manager succ\n");
	}

	if (!m_pMotionMan->Initial(m_videoInputNum, DVR_TIMER_MOTION, m_pAlarmOutMan, m_pLogMan, m_pStreamRecManEx, m_pDisplayMan, m_localVideoInputNum, m_netVideoInputNum))
	{
		printf("Initial motion manager fail\n");
		return false;
	}
	else
	{
		printf("Initial motion manager succ\n");
	}

	if (!m_pSensorInMan->Initial(m_sensorInputNum, DVR_TIMER_SENSOR_IN, m_pAlarmOutMan, m_pLogMan, m_pStreamRecManEx, m_pDisplayMan, m_localVideoInputNum, m_netVideoInputNum))
	{
		printf("Initial sensor in manager fail\n");
		return false;
	}
	else
	{
		printf("Initial sensor in manager succ\n");
	}

	if (!m_pVideoLossMan->Initial(m_videoInputNum, DVR_TIMER_VIDEO_LOSS, m_pAlarmOutMan, m_pLogMan, m_pStreamRecManEx, m_pDisplayMan, m_localVideoInputNum, m_netVideoInputNum))
	{
		printf("Initial video loss manager fail\n");
		return false;
	}
	else
	{
		printf("Initial video loss manager succ\n");
	}

	if (!m_pOtherAlarm->Initial(OTHER_ALARM_TYPE_NUM, DVR_TIMER_OTHER_ALARM, m_pAlarmOutMan, m_pLogMan, m_pStreamRecManEx, m_pDisplayMan, m_localVideoInputNum, m_netVideoInputNum))
	{
		printf("Initial video loss manager fail\n");
		return false;
	}
	else
	{
		printf("Initial video loss manager succ\n");
	}

#ifdef __MOBILE_DVR__
	if (!m_pSpeedMan->Initial(2, DVR_TIMER_SENSOR_IN, m_pAlarmOutMan, m_pLogMan, m_pStreamRecManEx, m_pDisplayMan))
	{
		printf("Initial speed manager fail\n");
		return false;
	}
	else
	{
		printf("Initial speed succ\n");
	}
#endif
#ifdef __MOBILE_DVR__
	if (!m_pAlarmMan->Initial(m_pMotionMan, m_pSensorInMan, m_pVideoLossMan, m_pOtherAlarm, m_pAlarmOutMan, m_pLogMan, m_pMsgMan,m_pSpeedMan))
	{
		printf("Initial alarm manager fail\n");
		return false;
	}
	else
	{
		printf("Initial alarm manager succ\n");
	}
#else
	if (!m_pAlarmMan->Initial(m_pMotionMan, m_pSensorInMan, m_pVideoLossMan, m_pOtherAlarm, m_pAlarmOutMan, m_pLogMan, m_pMsgMan))
	{
		printf("Initial alarm manager fail\n");
		return false;
	}
	else
	{
		printf("Initial alarm manager succ\n");
	}

#endif
	if( !m_pPtzMan->Initial() )
	{
		printf("Initial ptz manager fail\n");
		return false;
	}
	else
	{
		printf("Initial ptz manager succ\n");
	}
#ifdef __ENVIRONMENT_LINUX__
	if (!m_pFakeCurise->Initial(m_localVideoInputNum))
	{
		printf("Initial fake curise fail\n");
		return false;
	}
	else
	{
		printf("Initial fake curise succ\n");
	}
#endif
	
// 	if (!CReadDdns::Instance()->Initial())
// 	{
// 		printf("Initial ReadDDdns error\n");
// 	}
// 	else
// 	{
// 		printf("Initial ReadDDdns ok\n");
// 	}

	if(!CPinYinInput::Instance()->Initial(pWorkPath))
	{
		printf("Initial pinyin input fail\n");
	}
	else
	{
		printf("Initial pinyin input succ\n");
	}

	if (!m_pDisplayMan->Initial(m_localVideoInputNum,m_videoInputNum,m_videoOutNum,m_netVideoInputNum))
	{
		printf("Initial display man fail\n");
		return false;
	}
	else
	{
		printf("Initial display man succ\n");
	}
#ifdef __ENVIRONMENT_LINUX__
	if (!m_pUpnpMan->Initial())
	{
		printf("Initial UpnpMan man fail\n");
		return false;
	}
	else
	{
		printf("Initial UpnpMan man succ\n");
	}
#endif


#ifdef __MOBILE_DVR__
	if (!m_pConfigSetMan->Initial(m_localVideoInputNum, m_netVideoInputNum, m_sensorInputNum, m_sensorOutNum, \
		m_pRecMan, m_pMotionMan, m_pSensorInMan, m_pVideoLossMan, m_pOtherAlarm, \
		m_pAlarmOutMan, m_pReclogMan, m_pLogMan, m_pLocalDeviceMan, \
		m_pDisplayMan, m_pPtzMan, m_pNetReader, m_pStreamProc,m_pSpeedMan))
	{
		printf("Initial config set manager fail\n");
		return false;
	}
	else
	{
		printf("Initial config set manager succ\n");
	}
#else
#ifdef __DVR_ULTIMATE__
	if (!m_pConfigSetMan->Initial(m_localVideoInputNum, m_netVideoInputNum, m_localSensorInputNum, m_sensorInputNum, m_sensorOutNum, \
								  m_pRecMan, m_pMotionMan, m_pSensorInMan, m_pVideoLossMan, m_pOtherAlarm,\
								  m_pAlarmOutMan, m_pReclogMan, m_pLogMan, m_pLocalDeviceMan, \
								  m_pDisplayMan, m_pPtzMan, m_pNetReader, m_pStreamProc,
								  m_pNetDeviceMan))
#else
	if (!m_pConfigSetMan->Initial( m_localVideoInputNum, m_netVideoInputNum, m_localSensorInputNum, m_sensorInputNum, m_sensorOutNum, \
									m_pRecMan, m_pMotionMan, m_pSensorInMan, m_pVideoLossMan, m_pOtherAlarm,\
									m_pAlarmOutMan, m_pReclogMan, m_pLogMan, m_pLocalDeviceMan, \
									m_pDisplayMan, m_pPtzMan, m_pNetReader, m_pStreamProc))
#endif
	{
		printf("Initial config set manager fail\n");
		return false;
	}
	else
	{
		printf("Initial config set manager succ\n");
	}
#endif

	unsigned long ulMaxBufMsg = m_videoInputNum * 3;
	
#ifdef __ENVIRONMENT_LINUX__
	{
		char macSN[40] = {0};
		char mac[20] = {0};
		CNetInterface::Instance()->GetMAC(mac);
		snprintf(macSN, sizeof(macSN), "%02x%02x%02x%02x%02x%02x", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);

		printf("%s:%s:%d, my mac=%s\n", __FUNCTION__, __FILE__, __LINE__, macSN);

		string sCostomID;

		GetCustomEncrypt(subProductID, sCostomID);
		m_pPushMan->Initial(macSN, sCostomID.c_str(), ulMaxBufMsg);
	}
#else
	string sCostomID;

	GetCustomEncrypt(subProductID, sCostomID);

	m_pPushMan->Initial(NULL, sCostomID.c_str(), ulMaxBufMsg);
#endif

#ifdef __ENVIRONMENT_LINUX__
	if( !m_pallocNICIP->Initial() )
	{
		printf( "%s():%s:%d, initial the alloc NIC IP error\n", __FUNCTION__, __FILE__, __LINE__ );
		return false;
	}
	else
	{
		printf( "%s():%s:%d, initial the alloc NIC IP succ\n", __FUNCTION__, __FILE__, __LINE__ );
	}

#ifdef __CUSTOM_IPV6__
	if( !m_pallocNICIP6->Initial() )
	{
		printf( "%s():%s:%d, initial the alloc NIC IPv6 error\n", __FUNCTION__, __FILE__, __LINE__ );
		return false;
	}
	else
	{
		printf( "%s():%s:%d, initial the alloc NIC IPv6 succ\n", __FUNCTION__, __FILE__, __LINE__ );
	}


#endif

	if( !m_pDdnsManager->Inital() )
	{
		printf( "%s():%s:%d, initial the DDNS manager error\n", __FUNCTION__, __FILE__, __LINE__ );
		return false;
	}
	else
	{
		printf( "%s():%s:%d, initial the DDNS manager succ\n", __FUNCTION__, __FILE__, __LINE__ );
	}
#endif
	

	//设置抓jpeg帧率
	//m_pLocalDeviceMan->SetSnapCapability(m_localVideoInputNum, CProduct::Instance()->SnapJpegRate(), CProduct::Instance()->SnapJpegRateTime(), DVRVIDEO_SIZE_CIF);

#if defined (__ENVIRONMENT_LINUX__) && defined (__DVR_BASIC__)
#if defined(__SUPPORT_HONGDI__)
	m_pSnapCapabilityMan->Initial(m_localVideoInputNum, CProduct::Instance()->SnapJpegRate(), CProduct::Instance()->SnapJpegRateTime(), DVRVIDEO_SIZE_HD1080);
#else
	m_pSnapCapabilityMan->Initial(m_localVideoInputNum, CProduct::Instance()->SnapJpegRate(), CProduct::Instance()->SnapJpegRateTime(), DVRVIDEO_SIZE_CIF);
#endif
#endif

#ifdef __SUPPORT_HRSST__
	m_pPhoneSync->Init(m_pProduct->ProductType());
#endif

#ifdef __CUSTOM_YUANZHENG__
	CYuanZhengMan::Instance()->Inital();
	m_pYZSnapMan->Initial(m_localVideoInputNum);
#endif
	
	//初始化配置，往各模块设入配置参数
	if (!SetupConfig())
	{
		printf("Setup config fail\n");
		return false;
	}
	else
	{
		m_pConfigSetMan->SetHasIntial();
		printf("Setup config succ\n");
	}


	//获取播放器的路径，用于备份
	{
		char szTmp[128] = {0};
		m_pVideoPlayerPath = new char [128];
		memset(m_pVideoPlayerPath, 0, 128);
		strcpy(szTmp, pWorkPath);
		int len = strlen(szTmp);
		if ('/' == szTmp[len - 1])
		{
			szTmp[len - 1] = '\0';
		}

		sprintf(m_pVideoPlayerPath, "/mnt/mtd/%s", BACKUP_STORE_VIDEOPLAY_PATH);
	}

	//初始化一些变量
	{
		m_bOperForFormating = false;
		m_bOperForLocalBackuping = false;
		m_bOperForLocalPlayBackup = false;
		m_lOperForNetReading = 0;

		m_todayTimeL = GetZeroTime32OfDate(GetCurrTime32());
		m_bTodayHaveDel = false;
	}
	{
		m_bHaveReadFlash = false;
		memset(m_szHardwareVersion, 0, sizeof(m_szHardwareVersion));
		memset(m_szKernelVersion, 0, sizeof(m_szKernelVersion));
		memset(m_szMCUVersion, 0, sizeof(m_szMCUVersion));
	}
	{
#ifdef __ENVIRONMENT_LINUX__
		m_pLastVGADisp  = NULL;
		m_pCVBSMainDisp = NULL;
		m_pCVBSMinDisp  = NULL;		
#else
		m_pCurMainDispWin32 = NULL;
		m_pCurMinDispWin32  = NULL;
#endif
	}

	{
#ifdef __SUPPORT_HRSST__
		char szMacTemp[32] = {0};
		char szMac[32] = {0};		
		//unsigned long phoneSereverIP = IP_ADDRESS(122,11,22,227);
		//m_pPhoneSync->SetPhoneServerIp(phoneSereverIP);
		//m_pPhoneSync->SetPhoneServerPort(7001);
		CNetInterface::Instance()->GetMAC(szMacTemp);
		snprintf(szMac, 32, "%02x-%02x-%02x-%02x-%02x-%02x", szMacTemp[0],szMacTemp[1],szMacTemp[2],szMacTemp[3],szMacTemp[4],szMacTemp[5]);
		m_pPhoneSync->SetMac(szMac, 32);

		char szVersion[32] = {0};
		snprintf(szVersion, 32, "%s", &(DVR_SOFT_VERSION_M[16]));
		m_pPhoneSync->SetDVRVersion(szVersion, 32);
#endif
	}

	m_bSendFirstStream = false;


	CMessageMan::Instance()->SetPointToMainFrame((unsigned long*)this);

	//SetUpChannelVisible();
	//
	WriteLog(LOCAL_CLIENT_ID, LOG_TYPE_BOOT);


	return true;
}

void CMainFrame::Quit()
{
	//
	WriteLog(LOCAL_CLIENT_ID, LOG_TYPE_SHUTDOWN);

	//zxx新加入
	m_pCodeTable->Quit();

#ifdef __SUPPORT_HRSST__
	m_pPhoneSync->Quit();
#endif


	m_pPtzMan->Quit();
	m_pAlarmMan->Quit();
	m_pMotionMan->Quit();
	m_pSensorInMan->Quit();
	m_pVideoLossMan->Quit();
	m_pOtherAlarm->Quit();
	m_pDisplayMan->Quit();
#ifdef __MOBILE_DVR__
	m_pSpeedMan->Quit();
#endif

	m_pAlarmOutMan->Quit();

	m_pConfigSetMan->Quit();

	m_pPushMan->Quit();

#ifdef __ENVIRONMENT_LINUX__
	m_pUpnpMan->Quit();
#endif
	m_pBackupMan->Quit();
#ifdef __NETSERVER_HXHT__
	m_pHxNetReader->Quit();
#endif

	m_pNetReader->Quit();
	//m_pPlaybackMan->Quit();
	m_pDataSerProc->Quit();
	m_pStreamProc->Quit();
#if defined(__DVD_BURN_REC__) && defined (__ENVIRONMENT_LINUX__)
	m_pDvdWriteMan->Quit();
#endif

	m_pRecMan->Quit();
	m_pLogMan->Quit();
#if defined(__SUPPORT_SNAPD1__)
	m_pShanghaiConnectMan->Quit();//必须在m_pReclogMan之前Quit掉
#endif
	m_pReclogMan->Quit();

#if defined (__ENVIRONMENT_LINUX__) && defined (__DVR_BASIC__)
	m_pSnapProc->Quit();
	m_pSmtpMan->Quit();
	m_pSnapCapabilityMan->Quit();
	m_pSnapMan->Quit();
#ifdef __SUPPORT_SNAPD1__
	m_pShanghaiSnapMan->Quit();
#else
	m_pSnapPictureMan->Quite();		
#endif
#else
	m_pSmtpMan->Quit();
#endif

#ifdef __DVR_ULTIMATE__
	if(m_netVideoInputNum > 0)
	{
		m_pNetDeviceMan->Quit();
		m_pRegNetDeviceLog->Quit();
		m_pCfgNetDeviceMan->Quit();
	}
#endif

#ifdef __ENVIRONMENT_LINUX__
	//m_pPTZControl->Quit();
	m_pDiskMan->Quit();
	m_pCdOperMan->Quit();
	m_pDdnsManager->Quit();

#ifdef __CUSTOM_IPV6__
	m_pallocNICIP6->Quit();
#endif
	m_pallocNICIP->Quit();
#endif
	m_pDVRTimer->Quit();
#ifdef __CUSTOM_YUANZHENG__
	m_pYZRec->Quit();
	if(m_pYZRec!=NULL)
	{
		delete m_pYZRec;
		m_pYZRec=NULL;
	}
	m_pYZDVRTimer->Quit();
	{
		if(m_pYZDVRTimer!=NULL)
		{
			delete m_pYZDVRTimer;
			m_pYZDVRTimer=NULL;
		}
	}
#endif

#ifndef NDEBUG
	CRunLog::Instance()->Quit();
#endif

#ifdef __CUSTOM_YUANZHENG__
	CYuanZhengMan::Instance()->Quit();
	m_pYZSnapMan->Quit();
#endif


#ifdef __SUPPORT_HRSST__
	delete m_pPhoneSync;
#endif


	delete m_pReclogMan;
	//delete m_pLogMan;
	delete m_pStreamProc;
	delete m_pRecMan;
	delete m_pDataSerProc;
#if !defined(__ASYN_PLAY__)
	delete m_pPlaybackMan;
#else
	delete m_pPlaybackManAsyn;
#endif
#ifdef __NETSERVER_HXHT__
	delete m_pHxNetReader;
#endif

	delete m_pNetReader;
	delete m_pBackupMan;
	
	delete m_pMotionMan;
	delete m_pSensorInMan;
	delete m_pVideoLossMan;
	delete m_pOtherAlarm;
#ifdef __MOBILE_DVR__
	delete m_pSpeedMan;
#endif	
	delete m_pAlarmOutMan;

	delete m_pConfigSetMan;


#if defined (__ENVIRONMENT_LINUX__) && defined (__DVR_BASIC__)
	delete m_pSnapProc;
	delete m_pSmtpMan;
	delete m_pSnapCapabilityMan;
#if !defined(__SUPPORT_SNAPD1__)
	delete m_pSnapPictureMan;
#endif
	delete m_pSnapMan;	
#else
	delete m_pSmtpMan;
#endif

#ifdef __CUSTOM_YUANZHENG__
    delete m_pYZSnapMan;
#endif

	delete [] m_pDiskPath;

	m_pDiskPath = NULL;

	if (NULL != m_pVideoPlayerPath)
	{
		delete [] m_pVideoPlayerPath;
		m_pVideoPlayerPath = NULL;
	}

	if (NULL != m_pDiskInfoForFormat)
	{
		delete [] m_pDiskInfoForFormat;
		m_pDiskInfoForFormat = NULL;
	}

	if (PUB_THREAD_ID_NOINIT != m_formatDiskProcID)
	{		
		PUB_ExitThread(&m_formatDiskProcID, &m_bFormatDiskProc);
	}

	if (NULL != m_pDiskInfoForChange)
	{
		delete [] m_pDiskInfoForChange;
		m_pDiskInfoForChange = NULL;
	}

	if (PUB_THREAD_ID_NOINIT != m_changeDiskProcID)
	{		
		PUB_ExitThread(&m_changeDiskProcID, &m_bChangeDiskProc);
	}

	{
		m_bOperForFormating = false;
		m_bOperForLocalBackuping = false;
		m_bOperForLocalPlayBackup = false;
	}
	m_bHaveReadFlash = false;

#ifdef __ENVIRONMENT_LINUX__
	if (NULL != m_pLastVGADisp)
	{
		delete m_pLastVGADisp;
		m_pLastVGADisp = NULL;
	}
	if (NULL != m_pCVBSMainDisp)
	{
		delete m_pCVBSMainDisp;
		m_pCVBSMainDisp = NULL;
	}
	if (NULL != m_pCVBSMinDisp)
	{
		delete m_pCVBSMinDisp;
		m_pCVBSMinDisp = NULL;
	}
#else
	if (NULL != m_pCurMainDispWin32)
	{
		delete m_pCurMainDispWin32;
		m_pCurMainDispWin32 = NULL;
	}
	if (NULL != m_pCurMinDispWin32)
	{
		delete m_pCurMinDispWin32;
		m_pCurMinDispWin32 = NULL;
	}

#endif
}

bool CMainFrame::Start()
{
	m_msgProcID = PUB_CreateThreadEx(MessageThread, this, &m_bMsgProc, 40);
	if(m_msgProcID == PUB_CREATE_THREAD_FAIL)
	{
		PUB_PrintError(__FILE__,__LINE__);
		return false;
	}

	if (!m_pDVRTimer->Start())
	{
		PUB_PrintError(__FILE__,__LINE__);
		return false;
	}

	if (!m_pRecMan->Start())
	{
		PUB_PrintError(__FILE__,__LINE__);
		return false;
	}

#if defined(__DVD_BURN_REC__) && defined (__ENVIRONMENT_LINUX__)
	if (!m_pDvdWriteMan->Start())
	{
		PUB_PrintError(__FILE__,__LINE__);
		return false;
	}
#endif

#if defined (__ENVIRONMENT_LINUX__) && defined (__DVR_BASIC__)
	if (!m_pSnapProc->Start())
	{
		PUB_PrintError(__FILE__,__LINE__);
		return false;
	}
#endif

	if (!m_pStreamProc->Start())
	{
		PUB_PrintError(__FILE__,__LINE__);
		return false;
	}

	if (!m_pDataSerProc->Start())
	{
		PUB_PrintError(__FILE__,__LINE__);
		return false;
	}

#ifdef __NETSERVER_HXHT__
	if (!m_pHxNetReader->Start())
	{
		PUB_PrintError(__FILE__,__LINE__);
		return false;
	}
#endif

	if (!m_pNetReader->Start())
	{
		PUB_PrintError(__FILE__,__LINE__);
		return false;
	}

#ifdef __DVR_ULTIMATE__
	if(m_netVideoInputNum > 0)
	{
		if (!m_pNetDeviceMan->Start())
		{
			PUB_PrintError(__FILE__,__LINE__);
			return false;
		}
	}
#endif

#if /*defined (__ENVIRONMENT_LINUX__) && */defined (__DVR_BASIC__)
	m_pSmtpMan->Start();
#endif

#ifdef __ENVIRONMENT_LINUX__
	if (!m_pFakeCurise->Start())
	{
		PUB_PrintError(__FILE__,__LINE__);
		return false;
	}
#endif

	if (!m_pAlarmMan->Start())
	{
		PUB_PrintError(__FILE__,__LINE__);
		return false;
	}

	if( !m_pDisplayMan->Start() )
	{
		PUB_PrintError(__FILE__,__LINE__);
		return false;
	}

#ifdef __ENVIRONMENT_LINUX__
	m_pDdnsManager->Start();
#endif

	m_timerProcID = PUB_CreateThread(TimerThread, this, &m_bTimerProc);
	if(m_timerProcID == PUB_CREATE_THREAD_FAIL)
	{
		PUB_PrintError(__FILE__,__LINE__);
		return false;
	}

	m_diskDetectProcID = PUB_CreateThread(DiskDetectThread, this, &m_bDiskDetectProc);
	if(m_diskDetectProcID == PUB_CREATE_THREAD_FAIL)
	{
		PUB_PrintError(__FILE__,__LINE__);
		return false;
	}

	m_networkProcID = PUB_CreateThread(NetworkThread, this, &m_bNetworkProc);
	if(m_networkProcID == PUB_CREATE_THREAD_FAIL)
	{
		PUB_PrintError(__FILE__,__LINE__);
		return false;
	}

#ifdef __CUSTOM_YUANZHENG__
	if (!CYuanZhengMan::Instance()->Start())
	{
		PUB_PrintError(__FILE__,__LINE__);
		return false;
	}
	if(!m_pYZDVRTimer->Start())
	{
		PUB_PrintError(__FILE__,__LINE__);
		return false;
	}
	if(!m_pYZRec->start())
	{
		PUB_PrintError(__FILE__,__LINE__);
		return false;
	}
	if (!m_pYZSnapMan->Start())
	{
		PUB_PrintError(__FILE__,__LINE__);
		return false;
	}
	m_pYZSnapMan->Action(true);
#endif
	

	//创建看门狗线程
//#ifdef __RELEASE__
	m_feedDogProcID = PUB_CreateThread(FeedDogThread, this, &m_bFeedDogProc);
	if(m_feedDogProcID == PUB_CREATE_THREAD_FAIL)
	{
		PUB_PrintError(__FILE__,__LINE__);
		return false;
	}
//#endif

#ifdef __ENVIRONMENT_LINUX__
	if(!m_pUpnpMan->Start())
	{
		printf("%s:%s:%d, Start upnpMan error\n", __FUNCTION__, __FILE__, __LINE__);	
		return false;
	}
	else
	{
		printf("%s:%s:%d, Start upnpMan sucss\n", __FUNCTION__, __FILE__, __LINE__);	
	}
#endif

#if defined(__SUPPORT_SNAPD1__)
	m_pShanghaiConnectMan->GetHeartInfo(m_pReclogMan);
	if (!m_pShanghaiConnectMan->Start())
	{
		PUB_PrintError(__FILE__,__LINE__);
		return false;
	}

#endif

#ifdef __ALARM_TO_UPDATE_FTP_VIDEO__
	m_pFtpLoadMan->StartLoadStream();
#endif

#if defined(__TVT_PUSH_SOCK__)
	m_pPushMan->Start();
#endif

#if defined(__CHIP3520A__) || defined(__CHIP3520D__) || defined(__CHIP3521__) || defined(__CHIPGM__) || (defined(__CHIP3531__) && defined(__HISI_SDK_0BX__))
	//创建视频输入自动检测线程
	PRODUCT_TYPE productID = CProduct::Instance()->ProductType();
	
	if ((TD_2704TS_PL == productID) || (TD_2708TS_PL == productID) || (TD_2716TS_PL == productID) || (TD_2716TS_S == productID) || (TD_2716AS_SL == productID) || (TD_2704AS_S == productID) || (TD_2708AS_S == productID) || (TD_2708AS_S_A == productID) || (TD_2704AS_SL == productID)
		|| (TD_2704TD_P == productID) || (TD_2708TD_P == productID) || (TD_2716TD_P == productID) || (TD_2704TD_PL == productID) || (TD_2708TD_PL == productID) || (TD_2716TD_PL == productID) || (TD_2732TD_PL == productID)
		|| (TD_2704TE_M == productID) || (TD_2708TE_M == productID) || (TD_2716TE_M == productID)
		|| (TD_2704AE_SL == productID) || (TD_2708AE_SL == productID) || (TD_2716AE_SL == productID)
		|| (TD_2704AS_SL == productID) || (TD_2704AS_SL_A == productID) || (TD_2704AS_PL == productID) || (TD_2708AS_SL == productID) || (TD_2716AS_SL == productID)
		|| (TD_2704AS_SL_A == productID) || (TD_2708AS_SL_A == productID) || (TD_2716AS_SL_A == productID)
		|| (TD_2004AL == productID) || (TD_2008AS == productID)
		|| (TD_2704AS_S_A == productID) || (TD_2708AS_S_A == productID)
		|| (TD_2704AS_PL == productID) || (TD_2708AS_PL == productID) || (TD_2716AS_PL == productID)
		|| (TD_2716AC_PL == productID)
		|| (TD_2704AE_PL == productID) || (TD_2708AE_PL == productID) || (TD_2716AE_PL == productID)
		)
	{
		m_checkVIProcID = PUB_CreateThread(CheckVIThread, this, &m_bCheckVIProc);
		if(m_checkVIProcID == PUB_CREATE_THREAD_FAIL)
		{
			PUB_PrintError(__FILE__,__LINE__);
			return false;
		}
		else
		{
			m_bFirstCheckVIOver = false;
			PUB_Sleep(4000);
		}
	}

#endif

	return true;
}

void CMainFrame::Stop()
{
#if defined(__CHIP3520A__) || defined(__CHIP3520D__) || defined(__CHIP3521__) || (defined(__CHIP3531__) && defined(__HISI_SDK_0BX__))
	if (PUB_THREAD_ID_NOINIT != m_checkVIProcID)
	{
		PUB_ExitThread(&m_checkVIProcID, &m_bCheckVIProc);
	}
#endif

#ifndef __ENVIRONMENT_WIN32__
	m_pUpnpMan->Stop();


	ext_capture_stop(VIDEO_STREAM_TYPE_PRIMARY);
	ext_capture_stop(VIDEO_STREAM_TYPE_NETWORK);
	ext_capture_stop(VIDEO_STREAM_TYPE_AUDIO);
#endif
	if (PUB_THREAD_ID_NOINIT != m_networkProcID)
	{		
		PUB_ExitThread(&m_networkProcID, &m_bNetworkProc);
	}

	if (PUB_THREAD_ID_NOINIT != m_diskDetectProcID)
	{		
		PUB_ExitThread(&m_diskDetectProcID, &m_bDiskDetectProc);
	}

	if (PUB_THREAD_ID_NOINIT != m_timerProcID)
	{		
		PUB_ExitThread(&m_timerProcID, &m_bTimerProc);
	}
	
#if defined(__TVT_PUSH_SOCK__)
	m_pPushMan->Stop();
#endif

#ifdef __ALARM_TO_UPDATE_FTP_VIDEO__
	m_pFtpLoadMan->StopLoadStream();
#endif

#ifdef __CUSTOM_YUANZHENG__
	CYuanZhengMan::Instance()->Stop();
	m_pYZRec->stop();
	m_pYZDVRTimer->Stop();
#endif

#ifdef __DVR_ULTIMATE__
	m_pNetDeviceMan->Stop();
	CIPCDecDisplayMan::Instance()->Stop();
#endif

	m_pDisplayMan->Stop();
	
	m_pAlarmMan->Stop();

#ifdef __ENVIRONMENT_LINUX__
	m_pFakeCurise->Stop();
#endif

#if /*defined (__ENVIRONMENT_LINUX__) && */defined (__DVR_BASIC__)
	m_pSmtpMan->Stop();
#endif

#ifdef __NETSERVER_HXHT__
	m_pHxNetReader->Stop();
#endif

	m_pNetReader->Stop();
	m_pDataSerProc->Stop();
	m_pStreamProc->Stop();
#if defined (__ENVIRONMENT_LINUX__) && defined (__DVR_BASIC__)
	m_pSnapProc->Stop();
#endif

#if defined(__DVD_BURN_REC__) && defined (__ENVIRONMENT_LINUX__)
	m_pDvdWriteMan->Stop();
#endif

	m_pRecMan->Stop();
	m_pDVRTimer->Stop();

#ifdef __SUPPORT_SNAPD1__
	m_pShanghaiConnectMan->Stop();
#endif

#ifdef __CUSTOM_YUANZHENG__
	m_pYZSnapMan->Stop();
#endif

	if (PUB_THREAD_ID_NOINIT != m_msgProcID)
	{		
		PUB_ExitThread(&m_msgProcID, &m_bMsgProc);
	}

#ifdef __ENVIRONMENT_LINUX__
	m_pDdnsManager->Stop();
#endif

//#ifdef __RELEASE__
	if (PUB_THREAD_ID_NOINIT != m_feedDogProcID)
	{		
		PUB_ExitThread(&m_feedDogProcID, &m_bFeedDogProc);
	}
//#endif

	
}

#ifdef __ENVIRONMENT_LINUX__
void CMainFrame::SetResolution(int outdev, unsigned short width, unsigned short height, bool bActive)
{
	if (bActive)
	{
		DISPLAY_DEV_INFO temp[2];
		if ((VIDEO_OUT_VGA == outdev) || (VIDEO_OUT_HDMI == outdev))
		{
			temp[0].bValid = 1;
			temp[0].devId = 0;
			temp[0].videoType = outdev;
			temp[0].devMode = DISPLAY_DEV_MAJOR;
			temp[0].width = width;
			temp[0].height = height;

			VGA_RESOLUTION resolution = (VIDEO_FORMAT_NTSC != g_video_format) ? VGA_720X576 : VGA_720X480;
			GetVGAResolution(resolution, temp[1].width, temp[1].height);
			temp[1].bValid = 1;
			temp[1].devId = 0;
			temp[1].videoType = VIDEO_OUT_CVBS;
			temp[1].devMode = DISPLAY_DEV_MINOR;
			
			m_pDisplayMan->SetDisplayDevInfo(temp, 2);
			m_pLocalDeviceMan->SetCurMainDevice(outdev);

			if (NULL == m_pLastVGADisp)
			{
				m_pLastVGADisp = new DISPLAY_DEV_INFO;				
			}
			if (NULL != m_pLastVGADisp)
			{
				*m_pLastVGADisp = temp[0];
			}
			if (m_pCVBSMainDisp != NULL)
			{
				delete m_pCVBSMainDisp;
				m_pCVBSMainDisp = NULL;
			}
			if (m_pCVBSMinDisp != NULL)
			{
				delete m_pCVBSMinDisp;
				m_pCVBSMinDisp = NULL;
			}

		}
		else if ((VIDEO_OUT_CVBS == outdev))
		{
			//注意两中情况：1，从VGA切换到CVBS；2，从CVBS切换到CVBS，比如：在CVBS下，修改语言。
			if ((m_pCVBSMainDisp != NULL) && (m_pCVBSMinDisp != NULL))
			{
				temp[0] = *m_pCVBSMainDisp;
				temp[1] = *m_pCVBSMinDisp;
				m_pDisplayMan->SetDisplayDevInfo(temp, 2);				
			}
			else
			{
				temp[0].bValid = 1;
				temp[0].devId = 0;
				temp[0].videoType = outdev;
				temp[0].devMode = DISPLAY_DEV_MAJOR;
				temp[0].width = width;
				temp[0].height = height;
				
				if (m_pLastVGADisp != NULL)
				{
					temp[1] = *m_pLastVGADisp;
					temp[1].devMode = DISPLAY_DEV_MINOR;					
				}
				else
				{
					temp[1].bValid = 1;
					temp[1].devId = 0;
					temp[1].videoType = VIDEO_OUT_VGA;
					temp[1].devMode = DISPLAY_DEV_MINOR;
					temp[1].width = 1024;
					temp[1].height = 768;
					PRODUCT_TYPE productTypeTmp = m_pProduct->ProductType();
					if ((TD_7008SB == productTypeTmp) || (TD_7004SB == productTypeTmp))
					{						
						temp[1].width = 800;
						temp[1].height = 600;
					}
					
					if ((productTypeTmp >= TD_2704HD) && (productTypeTmp <= TD_2716XD_L))
					{
						bool bGetOk = false;
						int device=CLocalDevice::Instance()->GetDisplayModFromFlash( );//获得设备
						int resolution=CLocalDevice::Instance()->GetDisplayResFromFlash();//获得分辨率
						if (VIDEO_OUT_VGA == device)
						{
							if((resolution==VGA_800X600) || (resolution==VGA_1024X768) || (resolution==VGA_1280X960)|| (resolution==VGA_1280X1024))
							{
								bGetOk = true;
							}
							else
							{
								bGetOk = false;
							}
						}
						else if (VIDEO_OUT_HDMI == device)
						{
							if (resolution==VGA_1920X1080)
							{
								bGetOk = true;
							}
							else
							{
								bGetOk = false;
							}
						}
						else
						{
							bGetOk = false;
						}

						if (bGetOk)
						{
							printf("###%s,%d, get DISPLAY_DEV_MINOR ok\n",__FILE__,__LINE__);
							temp[1].videoType = device;
							GetVGAResolution((VGA_RESOLUTION)resolution, temp[1].width, temp[1].height);
							if ((TD_2704HD == m_pProduct->ProductType()) && (!m_pProduct->Is2704HDNewHarWare()))
							{
								printf("is 2708 old, not change\n");
							}
							else
							{
								printf("is not 2708 old, change to 1920 X 1080\n");
								temp[1].width = 1920;
								temp[1].height = 1080;
							}							
							printf("###%s,%d, get DISPLAY_DEV_MINOR ok,outdevice:%d,width:%d height:%d\n",__FILE__,__LINE__,temp[1].videoType,temp[1].width,temp[1].height);
						}
						else
						{
							printf("##%s,%d,get DISPLAY_DEV_MINOR faild\n",__FILE__,__LINE__);
							if ((TD_2704HD == m_pProduct->ProductType()) && (!m_pProduct->Is2704HDNewHarWare()))
							{
								printf("is  2708 old \n");
								temp[1].videoType = VIDEO_OUT_VGA;
								temp[1].width = 1024;
								temp[1].height = 768;
							}
							else
							{
								printf("is not 2708 old\n");
								temp[1].videoType = VIDEO_OUT_HDMI;
								temp[1].width = 1920;
								temp[1].height = 1080;
							}
						}				
					}
					
					printf("################%s,%d,outdevice:%d,width:%d height:%d\n",__FILE__,__LINE__,temp[1].videoType,temp[1].width,temp[1].height);
				}

				m_pDisplayMan->SetDisplayDevInfo(temp, 2);
				m_pLocalDeviceMan->SetCurMainDevice(outdev);
				
				if (NULL == m_pCVBSMainDisp)
				{
					m_pCVBSMainDisp = new DISPLAY_DEV_INFO;					
				}
				if (NULL != m_pCVBSMainDisp)
				{
					*m_pCVBSMainDisp = temp[0];
				}
				if (NULL == m_pCVBSMinDisp)
				{
					m_pCVBSMinDisp = new DISPLAY_DEV_INFO;				
				}
				if (NULL != m_pCVBSMinDisp)
				{
					*m_pCVBSMinDisp = temp[1];
				}
				if (m_pLastVGADisp != NULL)
				{
					delete m_pLastVGADisp;
					m_pLastVGADisp = NULL;
				}
			}	
		}
	}
	else
	{
		m_pDisplayMan->SetUnInitial();
	}
}
#else
void CMainFrame::SetDisplayDevInfo(const DISPLAY_DEV_INFO* pDispDevInfo, int num, bool bActive)
{
	if(bActive)
	{
		m_pDisplayMan->SetDisplayDevInfo(pDispDevInfo, num);
		m_pLocalDeviceMan->SetCurMainDevice(2);

		for (int i = 0; i < num; i++)
		{
			if(0 != pDispDevInfo[i].bValid && DISPLAY_DEV_MAJOR == pDispDevInfo[i].devMode)
			{
				if(NULL == m_pCurMainDispWin32)
				{
					m_pCurMainDispWin32 = new DISPLAY_DEV_INFO;
				}
				memcpy(m_pCurMainDispWin32, &pDispDevInfo[i], sizeof(DISPLAY_DEV_INFO));
			}
			
			if(0 != pDispDevInfo[i].bValid && DISPLAY_DEV_MINOR == pDispDevInfo[i].devMode)
			{
				if(NULL == m_pCurMinDispWin32)
				{
					m_pCurMinDispWin32 = new DISPLAY_DEV_INFO;
				}
				memcpy(m_pCurMinDispWin32, &pDispDevInfo[i], sizeof(DISPLAY_DEV_INFO));
			}
		}
	}
	else
	{
		m_pDisplayMan->SetUnInitial();
	}
}
#endif

bool CMainFrame::CheckFreezeScreenSave()
{
	if ((m_bOperForFormating || m_bOperForLocalBackuping || m_bOperForLocalPlayBackup))
	{
		return true;
	}

	return false;	
}

void CMainFrame::FirstCheckVIOver()
{
	int waitCount = 12;

	while (waitCount)
	{
		if (m_bFirstCheckVIOver && 6 > waitCount)
		{
			break;
		}

		PUB_Sleep(1000);
		waitCount--;
	}
}

//protected

//private
PUB_THREAD_RESULT PUB_THREAD_CALL CMainFrame::MessageThread(void *pParam)
{
#ifdef __ENVIRONMENT_LINUX__
	pid_t pid = getpid();
	pid_t tid = syscall(__NR_gettid);
	printf("%s, %d, pid = %d, tid=%d\n", __FILE__, __LINE__, pid, tid);
#endif

	CMainFrame *pThis = reinterpret_cast<CMainFrame *>(pParam);
	pThis->MessageProc();

	return 0;
}
  
int CMainFrame::MessageProc()
{
	CMessageMan *pMsgMan = CMessageMan::Instance();
	MESSAGE_DATA msgData;

	while (m_bMsgProc)
	{
		if (pMsgMan->GetMsgFromClient(msgData))
		{
			//对于一些耗时比较长的命令放到DataSerProc中处理 .在DataSerProc中会释放msgData.pData
			NET_PROTOCOL cmdType = Protocol(msgData);
			if ((CMD_REQUEST_DATA_SEARCH == cmdType)
			  ||(CMD_REQUEST_DATA_LOCK == cmdType)
			  ||(CMD_REQUEST_DATA_UNLOCK == cmdType)
			  ||(CMD_REQUEST_DATA_DELETE == cmdType)
			  ||(CMD_REQUEST_PIC_LOCK == cmdType)
			  ||(CMD_REQUEST_PIC_UNLOCK == cmdType)
			  ||(CMD_REQUEST_PIC_DELETE == cmdType))
			{
				//（2010-01-20 14:27:00 YSW)
				//由于m_pDataSerProc->PutMsg只是放入一个列表，等待其内部一个线程再去处理。
				//这个分支的释放要靠其内部完成。
				m_pDataSerProc->PutMsg(msgData);
				NET_DATA_SEARCH dataInfo = *reinterpret_cast<NET_DATA_SEARCH *>(msgData.pData + sizeof(PACKCMD));
				if(dataInfo.searchType == DATA_SEARCH_TYPE_EVENT)
				{
					WriteLog(msgData.clientID,LOG_TYPE_SEARCH_EVENT);
				}
				else if(dataInfo.searchType ==DATA_SEARCH_TYPE_FILE)
				{
					WriteLog(msgData.clientID,LOG_TYPE_SEARCH_FILE_MAN);
				}
			}
			else
			{
				if (LOCAL_DEVICE_ID == msgData.deviceID)
				{
					MessageProc(msgData);
				}
				else
				{
					//发给网络设备
					NetMessageProc(msgData);
				}

				//（2010-01-20 14:27:00 YSW)
				//存着在MessageProc中提前返回的情况，而释放却在最后，这样会导致内存泄漏，所以移到此处保证释放
				pMsgMan->ReleaseMsgBuff(msgData);
			}
#if !defined(__CHIP3531__) && !defined(__CHIP3535__) || defined(__CHIPGM__) 
//			PUB_Sleep(20);
#endif
		}
		else
		{
			PUB_Sleep(40);
		}
	}

	return 0;
}

void CMainFrame::MessageProc(const MESSAGE_DATA & msgData)
{   
	bool _judge__ = false;
	assert (0 == msgData.deviceID);

	NET_PROTOCOL cmdType = Protocol(msgData);
	
	if (CMD_REQUEST_LOGIN == cmdType)
	{
		
		printf("%s,%d,g_bProIdequeSubProdID:%d\n",__FILE__,__LINE__,g_bProIdequeSubProdID);
#ifdef  __ENVIRONMENT_LINUX__
		if (!g_bProIdequeSubProdID)
		{			
			NET_CTRL_FAIL ret = NETERR_LOGIN_FAIL_USERPWD_ERR;
			MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_LONGIN_FAIL, reinterpret_cast<unsigned char *>(&ret), sizeof(NET_CTRL_FAIL));			
		}
		else
#endif
		{
			//检查用户名和密码
			NET_LOGIN_INFO *pLogInInfo = reinterpret_cast<NET_LOGIN_INFO *>(msgData.pData + sizeof(PACKCMD));


			bool bAuto = false;
			bAuto = ((LOCAL_IP == pLogInInfo->IP) && (0 != pLogInInfo->connectType));

			if (CONNECTTYPE_ACTIVE_CONNECT == pLogInInfo->connectType)
			{
				strcpy(pLogInInfo->username, "admin");
#ifndef __CUSTOM_BR27__
				strcpy(pLogInInfo->password, "123456");
#else
				strcpy(pLogInInfo->password, "1");
#endif

#if defined(__CUSTOM_ZNV__)
				strcpy(pLogInInfo->password, "12345");
#endif

#if defined(__CUSTOM_CNB__)
				strcpy(pLogInInfo->username, "root");
				strcpy(pLogInInfo->password, "admin");
#endif

#if defined(__CUSTOM_ZNV__) && defined(__ZNV_CN__) && defined(__ZNV_SC__)
				strcpy(pLogInInfo->password, "admins");
#endif

			}		

			LOGIN_RET logInRet = m_pUserMan->LogIn(pLogInInfo->username, pLogInInfo->password, pLogInInfo->MAC, pLogInInfo->connectType, msgData.clientID, pLogInInfo->IP, bAuto);
			
			if (LOGIN_OK == logInRet)
			{
				NET_LOGIN_SUCCESS_REPLY logInReply;
				memset(&logInReply, 0, sizeof(NET_LOGIN_SUCCESS_REPLY));				

				//读取MAC地址加入到登录返回值中 liuhao 2010-02-03
				char macTmp[8] = {0};
#ifdef __ENVIRONMENT_LINUX__
				CNetInterface::Instance()->GetMAC(macTmp);
#endif
				if (m_bHaveReadFlash)
				{
					//snprintf(logInReply.productInfo.hardwareVersion, 36, "%s", m_szHardwareVersion);
					//snprintf(logInReply.productInfo.kernelVersion, 64, "%s", m_szKernelVersion);
					//snprintf(logInReply.productInfo.MCUVersion, 36, "%s", m_szMCUVersion);
				}
				else
				{
					char szTemp1[36] = {0};
					char szTemp2[36] = {0};
					CProduct::Instance()->GetHardwarePreVersion(szTemp1, 36);
					CLocalDevice::GetHardWareVersion(szTemp2, 36);				
					snprintf(m_szHardwareVersion, 36, "%s-%s", szTemp1, szTemp2);
					CLocalDevice::GetKernelVersion(m_szKernelVersion, 64);
					m_pLocalDeviceMan->GetMCUVersion(m_szMCUVersion, 36);
					//snprintf(logInReply.productInfo.hardwareVersion, 36, "%s", m_szHardwareVersion);
					//snprintf(logInReply.productInfo.kernelVersion, 64, "%s", m_szKernelVersion);
					//snprintf(logInReply.productInfo.MCUVersion, 36, "%s", m_szMCUVersion);
					m_bHaveReadFlash = true;
				}
				CProduct::Instance()->GetProductInfo(logInReply.productInfo, macTmp, m_szHardwareVersion, m_szKernelVersion, m_szMCUVersion);

				//检查权限的总开关，每次登录时属于。客户端已经登录的用户，在退出之前都是用登录
				//时使用的开关信息。中途如果有人改变开关属性，客户端将不受影响。
				//本地因为能访问到m_pMsgMan，所以在抓取输入（按键等控制）线程中及时更新了开关信息。
				logInReply.productInfo.bPasswordCheck = m_pMsgMan->CheckPassword();

				//用于的具体权限设置。需要根据总开关来定，如果总开关开启，则需要检查，否则可以不检查。
				USER_INFO userInfo;
				if (m_pUserMan->GetUser(userInfo, msgData.clientID))
				{
					logInReply.authLiveCH			= userInfo.authLiveCH;
					logInReply.authBackupCH			= userInfo.authBackupCH;
					logInReply.authPlaybackCH		= userInfo.authPlaybackCH;
					logInReply.authPTZCtrlCH		= userInfo.authPTZCtrlCH;
					logInReply.authRecordCH			= userInfo.authRecordCH;
					logInReply.authRemoteViewCH		= userInfo.authRemoteViewCH;
					logInReply.authority			= userInfo.authority;
				}
				else
				{
					assert (false);
				}

				MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_LONGIN_SUCC, reinterpret_cast<unsigned char *>(&logInReply), sizeof(NET_LOGIN_SUCCESS_REPLY));

				WriteLog(msgData.clientID, LOG_TYPE_LOG_IN);

				//修改网络连接状态灯
				if (LOCAL_CLIENT_ID != msgData.clientID)
				{
					m_pLocalDeviceMan->SetLEDStatus(LED_STATUS_NET, true);
				}

				//登录设置通道可见
				if (LOCAL_CLIENT_ID == msgData.clientID)
				{
					printf("%s,%d, LOCAL_CLIENT_ID logo in m_pLocalDeviceMan->SetCamVisible(true)\n",__FILE__,__LINE__);
					m_pLocalDeviceMan->SetCamVisible(true);
				}

				g_bCloseWzd = true;
			}
			else
			{
				NET_CTRL_FAIL ret = NETERR_LOGIN_FAIL_USERPWD_ERR;
				switch (logInRet)
				{
				case LOGIN_PWD_ERROR:
				case LOGIN_NO_USER:
					ret = NETERR_LOGIN_FAIL_USERPWD_ERR;
					break;
				case LOGIN_USER_OVERFILL:
					ret = NETERR_LOGIN_FAIL_OVERFLOW;
					break;
				case LOGIN_NO_AUTH:
				case LOGIN_MAC_ERROR:
					ret = NETERR_LOGIN_FAIL_REFUSE;
					break;
				default:
					assert(false);
					break;
				}

				MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_LONGIN_FAIL, reinterpret_cast<unsigned char *>(&ret), sizeof(NET_CTRL_FAIL));
			}
		}

		m_pMsgMan->PutMsgToClient(m_localMsgData); 
	}
	else if (CMD_REQUEST_LOGOUT == cmdType)
	{
		//云台配置退出
		if(msgData.clientID == m_pPtzMan->PtzClient())
		{
			m_pPtzMan->ExitPtz();
			WriteLog(msgData.clientID, LOG_TYPE_PTZ_EXIT);
		}

		//配置退出
		if (msgData.clientID == m_pConfig->cfgClient())
		{
			m_pConfig->ExitConfig();
		}

#ifdef __DVR_ULTIMATE__
		//设备管理退出
		if(msgData.clientID == m_pRegNetDeviceLog->GetClientID())
		{
			m_pRegNetDeviceLog->ExitIPCamConfig();
		}
#endif
		
		//回放及备份退出
		m_pNetReader->Stop(msgData.clientID);

		WriteLog(msgData.clientID, LOG_TYPE_LOG_OFF);

		//实际登出动作，要等完成一些退出动作之后。比如写日志。
		m_pUserMan->LogOff(msgData.clientID);

		//修改网络连接状态灯
		if (LOCAL_CLIENT_ID != msgData.clientID)
		{
			CMyList <USER_INFO> userList;
			if (0 == m_pUserMan->GetNetUser(userList))
			{
				m_pLocalDeviceMan->SetLEDStatus(LED_STATUS_NET, false);
			}
		}
		
		if (LOCAL_CLIENT_ID == msgData.clientID)
		{
			if (!m_pMsgMan->VisibleChannel() && m_pMsgMan->CheckPassword())//2011.7.6-如果设置未登录不可见且需要检查权限时，则登出时重新通道不可见
			{
				printf("@@@@@@@%s,%d,m_pLocalDeviceMan->SetCamVisible(false);\n ",__FILE__,__LINE__);
				m_pLocalDeviceMan->SetCamVisible(false);
			}
			else
			{
				printf("@@@@@@@%s,%d,CamVisible == true;\n ",__FILE__,__LINE__);
			}
		}		
	
		MsgData(m_localMsgData, msgData.clientID, CMD_REQUEST_LOGOUT, NULL, 0);
		m_pMsgMan->PutMsgToClient(m_localMsgData);	
	}
	else if (CMD_REQUEST_CFG_ENTER == cmdType)
	{
		if (INVALID_CLIENT_ID != m_pConfig->cfgClient())
		{
			NET_CTRL_FAIL ret = NETERR_OPERATE_FAIL_OPERATING;
			MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_CFG_FAIL, reinterpret_cast<unsigned char *>(&ret), sizeof(NET_CTRL_FAIL));
		}
		else
		{
			m_pConfig->EnterConfig(msgData.clientID);
			/*unsigned char *pData = new unsigned char [4+sizeof(NET_CFG_PAGE_INFO)*64];
			*(reinterpret_cast<unsigned long *>(pData)) = m_pConfig->GetConfigPage(reinterpret_cast<NET_CFG_PAGE_INFO *>(pData+4));
			assert (*(reinterpret_cast<unsigned long *>(pData)) > 0);
			MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_CFG_SUCC, pData, 4+sizeof(NET_CFG_PAGE_INFO)*64);
			delete [] pData;*/
			MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_CFG_SUCC, NULL, 0);
		}

		m_pMsgMan->PutMsgToClient(m_localMsgData); 
	}
	else if (CMD_REQUEST_CFG_GET == cmdType)
	{
// 		if (msgData.clientID != m_pConfig->cfgClient())
// 		{
// 			NET_CTRL_FAIL ret = NETERR_MUST_ENTER_CFG;
// 			MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_CFG_FAIL, reinterpret_cast<unsigned char *>(&ret), sizeof(NET_CTRL_FAIL));
// 		}
// 		else
		{
			unsigned long dataLen = 0;
			unsigned char *pCfgData = NULL;
			if (m_pConfig->GetConfig(reinterpret_cast<unsigned char *>(msgData.pData+sizeof(PACKCMD)), msgData.dataLen-sizeof(PACKCMD), &pCfgData, dataLen))
			{
				MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_CFG_DATA, pCfgData, dataLen);

				delete [] pCfgData;
			}
			else
			{
				NET_CTRL_FAIL ret = NETERR_FAIL;
				MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_CFG_FAIL, reinterpret_cast<unsigned char *>(&ret), sizeof(NET_CTRL_FAIL));
			}
		}

		m_pMsgMan->PutMsgToClient(m_localMsgData); 
#if defined(__SUPPORT_SNAPD1__)
		char tip[256]="GB2312:获取DVR参数配置";
		if(NULL != m_pShanghaiConnectMan)
		{
			m_pShanghaiConnectMan->GetLogCmd(DVRCFGSET,0,tip,strlen(tip),NULL,0,GetCurrTime());
		}
		
#endif
	}
	else if (CMD_REQUEST_CFG_SET == cmdType)
	{
		if (msgData.clientID != m_pConfig->cfgClient())
		{
			NET_CTRL_FAIL ret = NETERR_MUST_ENTER_CFG;
			MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_CFG_FAIL, reinterpret_cast<unsigned char *>(&ret), sizeof(NET_CTRL_FAIL));
			m_pMsgMan->PutMsgToClient(m_localMsgData);
		}
		else
		{
			//格式化硬盘时，不能配置时区。
			if ((m_bOperForFormating || m_bOperForLocalBackuping || m_bOperForLocalPlayBackup) \
				&& (IsSetTzoneConfig(reinterpret_cast<unsigned char *>(msgData.pData+sizeof(PACKCMD)), msgData.dataLen-sizeof(PACKCMD))) )
			{
				NET_CTRL_FAIL ret = NETERR_FAIL;
				MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_CFG_FAIL, reinterpret_cast<unsigned char *>(&ret), sizeof(NET_CTRL_FAIL));
				m_pMsgMan->PutMsgToClient(m_localMsgData);
			}
			else
			{
				//解析消息的数据内容
				{
					
					unsigned long dataLen = msgData.dataLen-sizeof(PACKCMD);//包含blockhead和itemhead的数据长度
					const NCFG_BLOCK_HEAD *pBlockHead = reinterpret_cast<const NCFG_BLOCK_HEAD*>(msgData.pData+sizeof(PACKCMD));
					unsigned long readPos = sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD)*pBlockHead->ItemNum;
					unsigned writePos = 0;
					
					const NCFG_ITEM_HEAD *pItemHead = NULL;
					PRODUCT_TYPE productID = CProduct::Instance()->ProductType();
					
					for (unsigned long i =0; i<pBlockHead->ItemNum; i++)
					{
						pItemHead = reinterpret_cast<const NCFG_ITEM_HEAD *>(msgData.pData+sizeof(PACKCMD) + i*sizeof(NCFG_ITEM_HEAD) + sizeof(NCFG_BLOCK_HEAD));

						if ((INVALID_CLIENT_ID != m_pConfig->cfgClient()) && (LOCAL_DEVICE_ID != m_pConfig->cfgClient()) && 
							((TD_2704AS_SL_A == productID) || (TD_2708AS_S_A == productID)
							|| (TD_2704AS_PL == productID)|| (TD_2708AS_PL == productID) || (TD_2716AS_PL == productID)
							|| (TD_2704TS_PL == productID)|| (TD_2708TS_PL == productID) || (TD_2716TS_PL == productID)))
						{

							if (NCFG_ITEM_ENCODE_MAJOR == pItemHead->itemID)
							{
								ENCODE_INFO *pEncode = (ENCODE_INFO *)(msgData.pData + sizeof(PACKCMD) + readPos);
								for (int i=0; i<m_localVideoInputNum; i++)
								{
									printf("%s:%s:%d, channel=%d,resolution =%d\n", __FUNCTION__, __FILE__, __LINE__, pEncode[i].resolution);
									pEncode[i].resolution = CLocalDevice::Instance()->GetVideoSize(i);
									printf("%s:%s:%d, channel=%d,resolution =%d\n", __FUNCTION__, __FILE__, __LINE__, pEncode[i].resolution);
								}
							}

							if(NCFG_ITEM_ENCODE_MAJOR_EX == pItemHead->itemID)
							{
								ENCODE_INFO_EX *pEncode = (ENCODE_INFO_EX *)(msgData.pData + sizeof(PACKCMD) + readPos);
								for (int i=0; i<m_localVideoInputNum; i++)
								{
									printf("%s:%s:%d, channel=%d,resolution =%d\n", __FUNCTION__, __FILE__, __LINE__, pEncode[i].resolution);
									pEncode[i].resolution = CLocalDevice::Instance()->GetVideoSize(i);
									printf("%s:%s:%d, channel=%d,resolution =%d\n", __FUNCTION__, __FILE__, __LINE__, pEncode[i].resolution);
								}
							}
						}

						if (NCFG_ITEM_ENCODE_MAJOR == pItemHead->itemID)
						{
							CreateEncodeCfg(pItemHead,reinterpret_cast<unsigned char *>(msgData.pData+sizeof(PACKCMD)),readPos,pItemHead->itemID);
						}
						else if(NCFG_ITEM_ENCODE_MAJOR_EX == pItemHead->itemID)
						{
							CreateEncodeCfg(pItemHead,reinterpret_cast<unsigned char *>(msgData.pData+sizeof(PACKCMD)),readPos,pItemHead->itemID,msgData.clientID);
						}
						else if (NCFG_ITEM_ENCODE_MINOR == pItemHead->itemID)
						{
							CreateEncodeCfg(pItemHead,reinterpret_cast<unsigned char *>(msgData.pData+sizeof(PACKCMD)),readPos,pItemHead->itemID);
						}
						else if (NCFG_ITEM_ENCODE_MINOR_EX == pItemHead->itemID)
						{
							CreateEncodeCfg(pItemHead,reinterpret_cast<unsigned char *>(msgData.pData+sizeof(PACKCMD)),readPos,pItemHead->itemID,msgData.clientID);
						}
						else
						{
							printf("pItemHead->itemID=%x\n",pItemHead->itemID);
						}
						readPos += pItemHead->len;
					}

				}
				if (!m_pConfig->SetConfig(reinterpret_cast<unsigned char *>(msgData.pData+sizeof(PACKCMD)), msgData.dataLen-sizeof(PACKCMD)))
				{
					NET_CTRL_FAIL ret = NETERR_FAIL;
					MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_CFG_FAIL, reinterpret_cast<unsigned char *>(&ret), sizeof(NET_CTRL_FAIL));
					m_pMsgMan->PutMsgToClient(m_localMsgData);
				}
				else
				{
					//保持成功，则开始生效处理。
					m_pConfigSetMan->TakeEffect(reinterpret_cast<unsigned char *>(msgData.pData+sizeof(PACKCMD)), msgData.dataLen-sizeof(PACKCMD));
					MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_CFG_SUCC, NULL, 0);
					m_pMsgMan->PutMsgToClient(m_localMsgData);
				}
				
			}
		} 
	}
	else if (CMD_REQUEST_CFG_DEF_DATA == cmdType)
	{
		unsigned long dataLen = 0;
		unsigned char *pCfgData = NULL;
		bool bChangeToCVBS = false;
		PRODUCT_TYPE productTypeTmp = CProduct::Instance()->ProductType();
		if ((productTypeTmp >= TD_2704HD) && (productTypeTmp <= TD_2716XD_L))
		{
			if (VIDEO_OUT_CVBS == CLocalDevice::Instance()->GetCurMainDevice())
			{
				bChangeToCVBS = true;
			}
		}
		if (m_pConfig->GetDefaultData(reinterpret_cast<unsigned char *>(msgData.pData+sizeof(PACKCMD)), msgData.dataLen-sizeof(PACKCMD), &pCfgData, dataLen, bChangeToCVBS))
		{			
			MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_CFG_DATA, pCfgData, dataLen);

			delete [] pCfgData;
		}
		else
		{
			NET_CTRL_FAIL ret = NETERR_FAIL;
			MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_CFG_FAIL, reinterpret_cast<unsigned char *>(&ret), sizeof(NET_CTRL_FAIL));
		}

		m_pMsgMan->PutMsgToClient(m_localMsgData); 
	}
	else if (CMD_REQUEST_CFG_DEFAULT == cmdType)
	{
		{
			NET_CTRL_FAIL ret = NETERR_FAIL;
			MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_CFG_FAIL, reinterpret_cast<unsigned char *>(&ret), sizeof(NET_CTRL_FAIL));
		}

		m_pMsgMan->PutMsgToClient(m_localMsgData); 
	}
	else if (CMD_REQUEST_RESTOR_ADMIN_PASS == cmdType)
	{
		bool bSucc = false;
		NCFG_INFO_SHORT_NAME admin;
		NCFG_INFO_SHORT_NAME newPass;
		
		memset(&admin, 0, sizeof(admin));
		memset(&newPass, 0, sizeof(newPass));
		strcpy(admin.name, "admin");
#ifndef __CUSTOM_BR27__
			strcpy(newPass.name, "123456");
#else
			strcpy(newPass.name, "admin");
#endif
		if (LOCAL_CLIENT_ID == msgData.clientID)
		{
			//读取配置用户信息
			CMyList <NCFG_ITEM_ID> itemList;
			itemList.AddTail(NCFG_ITEM_USER);

			unsigned char* pData = NULL;
			unsigned long len = 0;
			if (m_pConfig->GetConfigEx(itemList, &pData, len))
			{
				CConfigBlock cfgBlock;
				cfgBlock.CreateBlock(pData, len);
				
				unsigned char *pCfgData = NULL;
				cfgBlock.GetConfig(NCFG_ITEM_USER, &pCfgData, len);
				assert(len == sizeof(NCFG_INFO_USER) * MAX_USER_NUM);
				NCFG_INFO_USER* pUser = reinterpret_cast<NCFG_INFO_USER*>(pCfgData);

				assert(sizeof(admin.name) == sizeof(pUser[0].name)); //确保长度一致
				assert(sizeof(newPass.name) == sizeof(pUser[0].name));
				for ( int i = 0; i < MAX_USER_NUM; i++ )
				{
					//查找对应用户
					if( USER_GROUP_NONE != pUser[i].group && 0 == strncmp(admin.name, pUser[i].name, sizeof(pUser[i].name)) )
					{
						strncpy(pUser[i].password, newPass.name, sizeof(pUser[i].password));
						
						//保存密码
						std::list<NCFG_ITEM_HEAD_EX> cfgItemList = cfgBlock.GetConfigItemList();
						
						//头信息
						NCFG_BLOCK_HEAD blockHead;
						blockHead.netcfgver = NCFG_VERSION;
						blockHead.biSize = sizeof(NCFG_BLOCK_HEAD);
						blockHead.ItemNum = cfgItemList.size();

						int header_len = blockHead.biSize + blockHead.ItemNum * sizeof(NCFG_ITEM_HEAD);
						int dataLen = cfgBlock.DataLen() - header_len;
						int setDataLen = header_len + dataLen;
						unsigned char* pSaveData = new unsigned char[setDataLen];

						//包装Block头和Item头信息
						memcpy( pSaveData, &blockHead, sizeof(NCFG_BLOCK_HEAD) );
						NCFG_ITEM_HEAD* pItemHeader = reinterpret_cast<NCFG_ITEM_HEAD*>(pSaveData + sizeof(NCFG_BLOCK_HEAD));
						for(std::list<NCFG_ITEM_HEAD_EX>::const_iterator iter = cfgItemList.begin(); iter != cfgItemList.end(); ++iter)
						{
							*pItemHeader = iter->item_head;
							++pItemHeader;
						}

						//填充数据
						memcpy(pSaveData + header_len, cfgBlock.Data(), dataLen);

						//尝试保存
						if( m_pConfig->SetConfig(pSaveData, setDataLen) )
						{
							//保存密码成功
							bSucc = true;							
							WriteLog(msgData.clientID, LOG_TYPE_CHANGE_PASSWD);
						}

						delete [] pSaveData;
						break;
					}
					
				}
				
				delete [] pData;
				pData = NULL;
				cfgBlock.DestroyBlock();
			}			
		}

		if (bSucc)
		{
			MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_CTRL_SUCC, NULL, 0);	
		}
		else
		{
			MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_CTRL_FAIL, NULL, 0);	
		}
		m_pMsgMan->PutMsgToClient(m_localMsgData);
	}
	else if(CMD_REQUEST_CFG_MODIFY_PASS == cmdType)
	{
		NET_CTRL_FAIL ret = NETERR_MUST_ENTER_CFG;

		if (msgData.clientID == m_pConfig->cfgClient())
		{
			//提取信息 : [登录ID] [旧密码] [新密码]
			unsigned long clientID = *reinterpret_cast<unsigned long*>(msgData.pData+sizeof(PACKCMD));
			NCFG_INFO_SHORT_NAME* pPass = reinterpret_cast<NCFG_INFO_SHORT_NAME*>(msgData.pData+sizeof(PACKCMD)+sizeof(unsigned long));
			assert(sizeof(PACKCMD)+sizeof(unsigned long)+sizeof(NCFG_INFO_SHORT_NAME) * 2 == msgData.dataLen );

			USER_INFO userInfo;
			memset(&userInfo, 0, sizeof(USER_INFO));
			if( m_pUserMan->GetUser(userInfo, msgData.clientID) ) //读取登录ID的用户信息
			{
				//读取配置用户信息
				CMyList <NCFG_ITEM_ID> itemList;
				itemList.AddTail(NCFG_ITEM_USER);

				unsigned char* pData = NULL;
				unsigned long len = 0;
				if (m_pConfig->GetConfigEx(itemList, &pData, len))
				{
					//在配置中,没有找到用户信息
					ret = NETERR_MODIFY_PASS_UFUSER;

					CConfigBlock cfgBlock;
					cfgBlock.CreateBlock(pData, len);

					unsigned char *pCfgData = NULL;
					cfgBlock.GetConfig(NCFG_ITEM_USER, &pCfgData, len);
					assert(len == sizeof(NCFG_INFO_USER) * MAX_USER_NUM);
					NCFG_INFO_USER* pUser = reinterpret_cast<NCFG_INFO_USER*>(pCfgData);

					assert(sizeof(userInfo.name) == sizeof(pUser[0].name)); //确保长度一致
					assert(sizeof(pPass[0].name) == sizeof(pUser[0].name));
					for ( int i = 0; i < MAX_USER_NUM; i++ )
					{
						//查找对应用户
						if( USER_GROUP_NONE != pUser[i].group && 0 == strncmp(userInfo.name, pUser[i].name, sizeof(pUser[i].name)) )
						{
							//比较旧密码
							if( 0 == strncmp(pPass[0].name, pUser[i].password, sizeof(pUser[i].password)) )
							{
								strncpy(pUser[i].password, pPass[1].name, sizeof(pUser[i].password));

								//保存密码
								std::list<NCFG_ITEM_HEAD_EX> cfgItemList = cfgBlock.GetConfigItemList();

								//头信息
								NCFG_BLOCK_HEAD blockHead;
								blockHead.netcfgver = NCFG_VERSION;
								blockHead.biSize = sizeof(NCFG_BLOCK_HEAD);
								blockHead.ItemNum = cfgItemList.size();

								int header_len = blockHead.biSize + blockHead.ItemNum * sizeof(NCFG_ITEM_HEAD);
								int dataLen = cfgBlock.DataLen() - header_len;
								int setDataLen = header_len + dataLen;
								unsigned char* pSaveData = new unsigned char[setDataLen];

								//包装Block头和Item头信息
								memcpy( pSaveData, &blockHead, sizeof(NCFG_BLOCK_HEAD) );
								NCFG_ITEM_HEAD* pItemHeader = reinterpret_cast<NCFG_ITEM_HEAD*>(pSaveData + sizeof(NCFG_BLOCK_HEAD));
								for(std::list<NCFG_ITEM_HEAD_EX>::const_iterator iter = cfgItemList.begin(); iter != cfgItemList.end(); ++iter)
								{
									*pItemHeader = iter->item_head;
									++pItemHeader;
								}
								
								//填充数据
								memcpy(pSaveData + header_len, cfgBlock.Data(), dataLen);

								//尝试保存
								if( m_pConfig->SetConfig(pSaveData, setDataLen) )
								{
									m_pConfigSetMan->TakeEffect(pSaveData, setDataLen);
									//保存密码成功
									ret = NETERR_SUCCESS;

									WriteLog(msgData.clientID, LOG_TYPE_CHANGE_PASSWD);
								}
								else
								{
									//保存密码到配置文件失败
									ret = NETERR_MODIFY_PASS_ERR_SAVE;
								}

								delete [] pSaveData;
							}
							else
							{
								//密码不正确
								ret = NETERR_MODIFY_PASS_ERR;
							}

							break; //找到了对应的用户跳出
						}
					}

					delete [] pData;
					pData = NULL;
					cfgBlock.DestroyBlock();
				}
			}
			else
			{
				//在登录用户中,没有找到用户信息
				ret = NETERR_MODIFY_PASS_UFUSER;
			}
		}
		
		if( NETERR_SUCCESS == ret )
		{
			MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_CFG_MODIFY_PASS_SUCC, reinterpret_cast<unsigned char *>(&ret), sizeof(NET_CTRL_FAIL));	
		}
		else
		{
			MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_CFG_MODIFY_PASS_FAIL, reinterpret_cast<unsigned char *>(&ret), sizeof(NET_CTRL_FAIL));	
		}
		m_pMsgMan->PutMsgToClient(m_localMsgData);
	}
#ifdef __DVR_ULTIMATE__
	else if (CMD_REQUEST_CFG_ENTER_IPCAM == cmdType)
	{
		if (INVALID_CLIENT_ID != m_pRegNetDeviceLog->GetClientID())
		{
			NET_CTRL_FAIL ret = NETERR_OPERATE_FAIL_OPERATING;
			MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_CFG_FAIL, reinterpret_cast<unsigned char *>(&ret), sizeof(NET_CTRL_FAIL));
		}
		else
		{
			m_pRegNetDeviceLog->EnterIPCamConfig(msgData.clientID);
			MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_CFG_SUCC, NULL, 0);
		}

		m_pMsgMan->PutMsgToClient(m_localMsgData); 
	}
	else if (CMD_REQUEST_CFG_LEAVE_IPCAM == cmdType)
	{
		assert ((msgData.clientID == m_pRegNetDeviceLog->GetClientID()) || (msgData.clientID == LOCAL_DEVICE_ID));
		m_pRegNetDeviceLog->ExitIPCamConfig();
	}
#endif
	else if(CMD_REQUEST_CFG_EXPORT == cmdType)
	{
#ifdef __ENVIRONMENT_LINUX__
		unsigned char *pExportCount = NULL;
		unsigned long countLen = 0;
		int retval = CExportConfig::Instance()->ExportConfig(&pExportCount, countLen);
		if( NETERR_EXPORT_CONFIG_OK == retval )
		{
			//printf("%s:%s:%d, export file Length=%d\n", __FUNCTION__, __FILE__, __LINE__, countLen);
			MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_CFG_EXPROT_SUCC, pExportCount, countLen);	
			
			//导出配置成时才写日志
			WriteLog(msgData.clientID, LOG_TYPE_EXPORT_CONFIG);
		}
		else
		{
			MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_CFG_EXPROT_FAIL, reinterpret_cast<unsigned char *>(&retval), sizeof(int));	
		}		
		
		if (NULL != pExportCount)
		{
			delete [] pExportCount;
			pExportCount = NULL;
		}
#else
		int retval = NETERR_EXPORT_CONFIG_ERR;
		MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_CFG_EXPROT_FAIL, reinterpret_cast<unsigned char *>(&retval), sizeof(int));	
		WriteLog(msgData.clientID, LOG_TYPE_EXPORT_CONFIG);
#endif


		m_pMsgMan->PutMsgToClient(m_localMsgData);

		
	}
	else if(CMD_REQUEST_CFG_IMPORT == cmdType)
	{
#ifdef __ENVIRONMENT_LINUX__
		unsigned long dataLen = (reinterpret_cast<PACKCMD *>(msgData.pData))->dataLen;
		int retval = CExportConfig::Instance()->ImportConfig(reinterpret_cast<char *>(msgData.pData + sizeof(PACKCMD)), dataLen);
		if( NETERR_EXPORT_CONFIG_OK == retval )
		{
			bool bHaveVideoMode = false;
			PRODUCT_TYPE productID = CProduct::Instance()->ProductType();
			if ((TD_2308SL_C == productID) || (TD_2308SS_C == productID) || (TD_2316SS_C == productID) || (TD_2308SE_C == productID) || (TD_2316SE_C == productID) || (TD_2308ME_C == productID) || (TD_2316ME_C == productID))
			{
				bHaveVideoMode = true;
			}
#if defined(__CUSTOM_US02__)
			if (TD_2516HS_BL == productID)
			{
				bHaveVideoMode = true;
			}
#endif
			CConfigEx	*pConfig = CConfigEx::Instance();

			CMyList <NCFG_ITEM_ID> itemList;
			itemList.AddTail(NCFG_ITEM_VIDEO_FORMAT);
			itemList.AddTail(NCFG_ITEM_ENCODE_960H);

			unsigned char *pConfigData = NULL;
			unsigned long dataLen = 0;

			if (pConfig->GetConfigEx(itemList, &pConfigData, dataLen))
			{
				CConfigBlock cfgBlock;
				cfgBlock.CreateBlock(pConfigData, dataLen);

				unsigned char *pData = NULL;
				if (bHaveVideoMode)
				{
					cfgBlock.GetConfig(NCFG_ITEM_ENCODE_960H, &pData, dataLen);
					unsigned long encode960H = *reinterpret_cast<const unsigned long *>(pData);
					unsigned long flashEncode = CLocalDevice::Instance()->GetEncodeSize();
					if(encode960H != flashEncode)
					{
						//把配置的值写入flash
						CLocalDevice::Instance()->SetEncodeSize(encode960H);
					}
				}
				
				//把视频制式写到flash中
				cfgBlock.GetConfig(NCFG_ITEM_VIDEO_FORMAT, &pData, dataLen);
				VIDEO_FORMAT videoFormatTemp = static_cast<VIDEO_FORMAT>(*reinterpret_cast<const unsigned long*>(pData));
				if ((VIDEO_FORMAT_PAL == videoFormatTemp) || ((VIDEO_FORMAT_NTSC == videoFormatTemp)))
				{
					char szTmp[8] = {0};
					sprintf(szTmp, "%d", videoFormatTemp);
					CLocalDevice::WriteVideoFormatToFlash(szTmp, 1);
				}
			}


			MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_CFG_IMPORT_SUCC, reinterpret_cast<unsigned char *>(&retval), sizeof(int));	
			
			//导入配置成时才写日志
			WriteLog(msgData.clientID, LOG_TYPE_IMPORT_CONFIG);
		}
		else
		{
			MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_CFG_IMPORT_FAIL, reinterpret_cast<unsigned char *>(&retval), sizeof(int));	
		}
#else
		int retval = NETERR_EXPORT_CONFIG_ERR;
		MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_CFG_IMPORT_FAIL, reinterpret_cast<unsigned char *>(&retval), sizeof(int));	
		WriteLog(msgData.clientID, LOG_TYPE_IMPORT_CONFIG);
#endif
		m_pMsgMan->PutMsgToClient(m_localMsgData);

		
	}
	else if (CMD_REQUEST_CFG_LEAVE == cmdType)
	{
		assert ((msgData.clientID == m_pConfig->cfgClient()) || (msgData.clientID == LOCAL_DEVICE_ID));

		m_pConfig->ExitConfig();
	}
	else if (CMD_REQUEST_DISK_NUM_STATUS == cmdType)
	{
		MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_DISK_INFO, (unsigned char *)(&m_systemDiskStatus), sizeof(m_systemDiskStatus));
		m_pMsgMan->PutMsgToClient(m_localMsgData);	
	}
	else if (CMD_REQUEST_DISK_START == cmdType)
	{
#ifdef __ENVIRONMENT_LINUX__
		int deviceNum = 0;
		DVR_DISK_INFO_LIST dvrDiskInfoList;
		DVR_DISK_INFO *pDvrDiskInfo = NULL;

		if (m_bOperForFormating)
		{
			//只有在格式化，才会失败
			NET_CTRL_FAIL ret = NETERR_FORMAT_DISK_FORMATING;
			MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_DISK_FAIL, (unsigned char *)&ret, sizeof(NET_CTRL_FAIL));
		}
		else
		{
			//查找录像硬盘
			m_pDiskMan->GetDeviceDiskInfo(NET_DISK_TYPE_FIXEDDISK, NULL, dvrDiskInfoList);
			//计算录像硬盘的剩余空间
			{
				POS pos = dvrDiskInfoList.GetHeadPosition();
				while (NULL != pos)
				{
					DVR_DISK_INFO &diskInfo = dvrDiskInfoList.GetAt(pos);
					if (NET_DISK_NO_FORMAT == diskInfo.diskStatus)
					{
						diskInfo.freeSpace = diskInfo.totalSpace;
					}
					else if (NET_DISK_NORMAL == diskInfo.diskStatus)
					{
						diskInfo.freeSpace = m_pReclogMan->GetDiskFreeSpace(diskInfo.diskpara, diskInfo.partitionNum);
					}
					dvrDiskInfoList.GetNext(pos);
				}
			}

			//查找热拔插设备
			m_pDiskMan->GetDeviceDiskInfo(NET_DISK_TYPE_REMOVEABLE, NULL, dvrDiskInfoList);

			//查找光驱
			m_pCdOperMan->GetDeviceDiskInfo(dvrDiskInfoList);

			deviceNum = dvrDiskInfoList.GetCount();		
			if (deviceNum > 0)
			{
				pDvrDiskInfo = new DVR_DISK_INFO[deviceNum];
				int i = 0;
				POS pos = dvrDiskInfoList.GetHeadPosition();
				while (NULL != pos)
				{
					DVR_DISK_INFO &diskInfo = dvrDiskInfoList.GetAt(pos);
					pDvrDiskInfo[i] = diskInfo;
					dvrDiskInfoList.GetNext(pos);
					++i;
				}

				MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_DISK_INFO, reinterpret_cast<unsigned char *>(pDvrDiskInfo), sizeof(DVR_DISK_INFO)*deviceNum);
			}
			else
			{
				MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_DISK_INFO, NULL, 0);
			}
		}
		
		if (pDvrDiskInfo != NULL)
		{
			delete [] pDvrDiskInfo;
			pDvrDiskInfo = NULL;
		}

		dvrDiskInfoList.RemoveAll();

		m_pMsgMan->PutMsgToClient(m_localMsgData);	
		
#else
        DVR_DISK_INFO diskInfo;
		memset(&diskInfo, 0 , sizeof(DVR_DISK_INFO));
		strncpy(diskInfo.diskname, m_pDiskPath, 35);
		MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_DISK_INFO, reinterpret_cast<unsigned char *>(&diskInfo), sizeof(DVR_DISK_INFO));
		m_pMsgMan->PutMsgToClient(m_localMsgData);	
#endif

	}
	else if (CMD_REQUEST_DISK_MOUNT_REMOVABLE == cmdType)
	{
#ifdef __ENVIRONMENT_LINUX__

		DVR_DISK_INFO *pDvrDiskInfo = reinterpret_cast<DVR_DISK_INFO *>(msgData.pData + sizeof(PACKCMD));
		int count = (msgData.dataLen - sizeof(PACKCMD))/sizeof(DVR_DISK_INFO);
		assert(1 == count);
		int index = 0;
		if (m_pDiskMan->Mount((DISK_TYPE)(pDvrDiskInfo[index].diskType), (DISK_INTERFACE_TYPE)(pDvrDiskInfo[index].diskInterface), pDvrDiskInfo[index].diskname, pDvrDiskInfo[index].totalSpace) < 0)
		{
			MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_DISK_MOUNT_REMOVABLE_FAIL, NULL, 0);
		}
		else
		{
			MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_DISK_MOUNT_REMOVABLE_SUC, NULL, 0);
		}
		m_pMsgMan->PutMsgToClient(m_localMsgData);	
#endif

	}
	else if (CMD_REQUEST_DISK_GET_REMOVABLE_PARTION == cmdType)
	{
#ifdef __ENVIRONMENT_LINUX__
	
		POS pos = NULL;
		int count = 0;
		DVR_DISK_INFO *pPartionInfo = NULL;
		DVR_DISK_INFO_LIST partionInfoList;

		m_pDiskMan->GetDevicePartionInfo(NET_DISK_TYPE_REMOVEABLE, NULL, partionInfoList);
		count = partionInfoList.GetCount();
		if (count > 0)
		{
			pPartionInfo = new DVR_DISK_INFO [count];
			pos = partionInfoList.GetHeadPosition();
			count = 0;
			while (NULL != pos)
			{
				DVR_DISK_INFO &partionInfo = partionInfoList.GetAt(pos);
				pPartionInfo[count] = partionInfo;
				count++;
				partionInfoList.GetNext(pos);
			}

			MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_DISK_INFO, reinterpret_cast<unsigned char *>(pPartionInfo), sizeof(DVR_DISK_INFO)*count);			
		}
		else
		{
			MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_DISK_INFO, NULL, 0);
		}
		
		partionInfoList.RemoveAll();
		if (NULL != pPartionInfo)
		{
			delete [] pPartionInfo;
			pPartionInfo = NULL;
		}

		m_pMsgMan->PutMsgToClient(m_localMsgData);	
#endif

	}
	else if (CMD_REQUEST_DISK_UNMOUNT_REMOVABLE == cmdType)
	{
#ifdef __ENVIRONMENT_LINUX__
		DVR_DISK_INFO *pDvrDiskInfo = reinterpret_cast<DVR_DISK_INFO *>(msgData.pData + sizeof(PACKCMD));
		int count = (msgData.dataLen - sizeof(PACKCMD))/sizeof(DVR_DISK_INFO);
		assert(1 == count);
		int index = 0;

		m_pDiskMan->UnMount((DISK_TYPE)(pDvrDiskInfo[index].diskType), pDvrDiskInfo[index].diskname);
#endif
	}
	else if (CMD_REQUEST_DISK_FORMAT_START == cmdType)
	{
#ifdef __ENVIRONMENT_LINUX__
		DVR_DISK_INFO *pDiskInfo = reinterpret_cast<DVR_DISK_INFO *>(msgData.pData + sizeof(PACKCMD));
		int num = (msgData.dataLen - sizeof(PACKCMD))/sizeof(DVR_DISK_INFO);
		bool bRet = false;
		DVR_DISK_INFO *pDiskInfoTemp  = NULL;
		NET_CTRL_FAIL ret = NETERR_FAIL;

		do 
		{
			/*if (num != 1)
			{
				break;
			}*/

			if (m_bOperForUpdating)
			{
				break;
			}

			if (m_bOperForFormating)
			{
				ret = NETERR_FORMAT_DISK_FORMATING;
				break;
			}

			if ((m_bOperForLocalBackuping) || (m_bOperForLocalPlayBackup))
			{
				ret = NETERR_FORMAT_DISK_BACKUP_PLAY;
				break;
			}

			if (m_pReclogMan->CheckReadDisk())
			{
				ret = NETERR_FORMAT_DISK_BACKUP_PLAY;
				break;
			}			
			
			int i = 0;
			for (i = 0; i < num; ++i)
			{
				if (NET_DISK_TYPE_FIXEDDISK != pDiskInfo[i].diskType)
				{					
					break;
				}
			}
			if (i != num)
			{
				break;
			}
			
			DVR_DISK_INFO_LIST tempList;
			for (i = 0; i < num; ++i)
			{				
				m_pDiskMan->GetDeviceDiskInfo((DISK_TYPE)pDiskInfo[i].diskType, pDiskInfo[i].diskname, tempList);
			}
			if (tempList.GetCount() != num)
			{
				break;
			}
			
			int count = 0;
			pDiskInfoTemp = new DVR_DISK_INFO [num];
			POS pos = tempList.GetHeadPosition();
			while (pos != NULL)
			{
				DVR_DISK_INFO &node = tempList.GetNext(pos);
				memcpy(&pDiskInfoTemp[count], &node, sizeof(DVR_DISK_INFO));
				count++;
			}
			tempList.RemoveAll();
			bRet = true;

		} while(0);
		
		if (bRet)
		{
			CMessageMan::Instance()->RefreshFormatProgress(0, 0, 0);
			FormatDisk(pDiskInfoTemp, num);
			MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_FORMAT_SUCC, NULL, 0);
			m_bOperForFormating = true;	
#ifdef __DVR_ULTIMATE__
			m_pNetDeviceMan->DeviceAction(false);
#endif
		}
		else
		{
			MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_FORMAT_FAIL, (unsigned char *)(&ret), sizeof(NET_CTRL_FAIL));
		}

		if (pDiskInfoTemp != NULL)
		{
			delete [] pDiskInfoTemp;
			pDiskInfoTemp = NULL;
		}
		m_pMsgMan->PutMsgToClient(m_localMsgData);

#else 
		DVR_DISK_INFO *pDiskInfo = reinterpret_cast<DVR_DISK_INFO *>(msgData.pData + sizeof(PACKCMD));
		int num = (msgData.dataLen - sizeof(PACKCMD))/sizeof(DVR_DISK_INFO);

		FormatDisk(pDiskInfo, num);

		MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_FORMAT_SUCC, NULL, 0);
		m_pMsgMan->PutMsgToClient(m_localMsgData);

		m_bOperForFormating = true;
#endif
	}
	else if (CMD_REQUEST_DISK_FORMAT_PERCENT == cmdType)
	{
		int allDiskNum = 0;
		int curDiskNum = 0;
		int precent = 0;
		unsigned long ret = 0; 
		CMessageMan::Instance()->GetFormatProgress(&allDiskNum, &curDiskNum, &precent);
		ret = precent;

		//第一快硬盘的百分比是0-100
		//第二快硬盘的百分比是0-110
		//第三快硬盘的百分比是0-120
		if ((precent == 100) && (curDiskNum >= 1))
		{
			ret = 100+(curDiskNum - 1)*10;
		}

		MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_FORMAT_PERCENT, (unsigned char *)(&ret), sizeof(unsigned long));
		m_pMsgMan->PutMsgToClient(m_localMsgData);						
	}
	else if (CMD_REQUEST_DISK_FORMAT_STOP == cmdType)
	{
		if (m_bOperForFormating)
		{
			StopFormatDisk();
			m_bOperForFormating = false;

			WriteLog(msgData.clientID, LOG_TYPE_FORMAT_SUCC);

			m_pLocalDeviceMan->SetLEDStatus(LED_STATUS_HDD, true);
#ifdef __DVR_ULTIMATE__
			m_pNetDeviceMan->DeviceAction(true);
#endif
		}
	}
	else if (CMD_REQUEST_DISK_GET_CD_INFO == cmdType)
	{
#ifdef __ENVIRONMENT_LINUX__
		int deviceNum = 0;
		DVR_DISK_INFO *pDiskInfo = NULL;
		DVR_DISK_INFO_LIST diskInfoList;

		m_pCdOperMan->GetDeviceDiskInfo(diskInfoList);
		deviceNum = diskInfoList.GetCount();
		if (deviceNum > 0)
		{
			pDiskInfo = new DVR_DISK_INFO[deviceNum];
			int i = 0;
			POS pos = diskInfoList.GetHeadPosition();
			while (NULL != pos)
			{
				DVR_DISK_INFO &diskInfo = diskInfoList.GetAt(pos);
				pDiskInfo[i] = diskInfo;
				diskInfoList.GetNext(pos);
				++i;

			}
			MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_DISK_INFO, reinterpret_cast<unsigned char *>(pDiskInfo), sizeof(DVR_DISK_INFO)*deviceNum);
		}
		else
		{
			MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_DISK_INFO, NULL, 0);
		}


		if (pDiskInfo != NULL)
		{
			delete [] pDiskInfo;
			pDiskInfo = NULL;
		}

		diskInfoList.RemoveAll();

		m_pMsgMan->PutMsgToClient(m_localMsgData);	
#else 
		MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_DISK_INFO, NULL, 0);
		m_pMsgMan->PutMsgToClient(m_localMsgData);	
#endif


	}
	else if (CMD_REQUEST_DISK_STOP == cmdType)
	{

	}
	else if (CMD_REQUEST_DISK_CHANGE_PROPERTY == cmdType)
	{
#ifdef __ENVIRONMENT_LINUX__
		DVR_DISK_INFO *pDiskInfo = reinterpret_cast<DVR_DISK_INFO *>(msgData.pData + sizeof(PACKCMD));
		int num = (msgData.dataLen - sizeof(PACKCMD))/sizeof(DVR_DISK_INFO);
		DVR_DISK_INFO *pDiskInfoTemp = NULL;
		NET_CTRL_FAIL ret = NETERR_SUCCESS;
		
		do 
		{
			if (m_bOperForFormating)
			{
				ret = NETERR_FORMAT_DISK_FORMATING;
				break;
			}

			if ((m_bOperForLocalBackuping) || (m_bOperForLocalPlayBackup))
			{
				ret = NETERR_FORMAT_DISK_BACKUP_PLAY;
				break;
			}

			if (m_pReclogMan->CheckReadDisk())
			{
				ret = NETERR_FORMAT_DISK_BACKUP_PLAY;
				break;
			}

			int i = 0;
			DVR_DISK_INFO_LIST tempList;
			for (i = 0; i < num; ++i)
			{				
				m_pDiskMan->GetDeviceDiskInfo((DISK_TYPE)pDiskInfo[i].diskType, pDiskInfo[i].diskname, tempList);
			}
			if (tempList.GetCount() != num)
			{
				ret = NETERR_FAIL;
				break;			
			}

			ret = NETERR_SUCCESS; 
			int index = 0;
			int count = 0;
			pDiskInfoTemp = new DVR_DISK_INFO [num];
			POS pos = tempList.GetHeadPosition();
			while (pos != NULL)
			{
				DVR_DISK_INFO &node = tempList.GetAt(pos);
				if (0 != strcmp(node.diskname, pDiskInfo[count].diskname))
				{
					printf("#####%s,%d,error\n",__FILE__,__LINE__);
				}
				else
				{
					if ((NET_DISK_OWNED_BY_THIS == node.diskOwnedBy)
						&&(NET_DISK_TYPE_FIXEDDISK == node.diskType) 
						&&(NET_DISK_NORMAL == node.diskStatus))
					{
						if (pDiskInfo[count].disktemp == node.diskProperty)
						{

						}
						else
						{
							memcpy(&pDiskInfoTemp[index], &node, sizeof(DVR_DISK_INFO));
							pDiskInfoTemp[index].disktemp = pDiskInfo[count].disktemp;
							index++;
						}											
					}
				}				
                
				tempList.GetNext(pos);
				count++;
			}

			num = index;				
			tempList.RemoveAll();
		} while(0);

		if ((NETERR_SUCCESS == ret) && (num > 0))
		{
			if (ChangeDiskProperty(pDiskInfoTemp, num) < 0)
			{
				ret = NETERR_DISK_IO_ERR;
			}
		}
		
		if (NETERR_SUCCESS == ret)
		{
			MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_DISK_CHANGE_PROPERTY_SUC, NULL, 0);			
		}
		else
		{
			MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_DISK_CHANGE_PROPERTY_FAIL, (unsigned char *)&ret, sizeof(NET_CTRL_FAIL));
		}
		m_pMsgMan->PutMsgToClient(m_localMsgData);

		if (pDiskInfoTemp != NULL)
		{
			delete [] pDiskInfoTemp;
			pDiskInfoTemp = NULL;
		}
#else 
		MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_DISK_CHANGE_PROPERTY_SUC, NULL, 0);
		m_pMsgMan->PutMsgToClient(m_localMsgData);	
#endif
	}
	else if (CMD_REQUEST_DISK_GET_OWNER == cmdType)
	{
#ifdef __ENVIRONMENT_LINUX__

		DVR_DISK_OWNER_LIST diskOwnerList;
		int num = m_pDiskMan->GetDiskOwner(NET_DISK_TYPE_FIXEDDISK, diskOwnerList);
		DVR_DISK_OWNER *pDiskOwner = NULL;
		if (num > 0)
		{
			pDiskOwner = new DVR_DISK_OWNER[num];
			unsigned long channelNum = 0;
			int i = 0;
			POS pos = diskOwnerList.GetHeadPosition();
			while (NULL != pos)
			{
				DVR_DISK_OWNER &owner = diskOwnerList.GetAt(pos);
				pDiskOwner[i] = owner;
				if (NET_DISK_OWNED_BY_THIS != pDiskOwner[i].dvrIndex)
				{
					channelNum = m_pReclogMan->GetDiskChannelNum(pDiskOwner[i].dvrPara);					
					pDiskOwner[i].dvrPara = channelNum;
				}
				diskOwnerList.GetNext(pos);
				++i;
			}
		}
		diskOwnerList.RemoveAll();

		MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_DISK_GET_OWNER_SUC, reinterpret_cast<unsigned char *>(pDiskOwner), num * sizeof(DVR_DISK_OWNER));			
		m_pMsgMan->PutMsgToClient(m_localMsgData);

		delete [] pDiskOwner;
		pDiskOwner = NULL;
#else
		DVR_DISK_OWNER owner;
		owner.dvrIndex = 0;
		owner.dvrPara = m_videoInputNum;
		MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_DISK_GET_OWNER_SUC, reinterpret_cast<unsigned char *>(&owner), sizeof(DVR_DISK_OWNER));			
		m_pMsgMan->PutMsgToClient(m_localMsgData);
#endif
	}
	else if (CMD_REQUEST_PTZ_START == cmdType)
	{
		if (INVALID_CLIENT_ID != m_pPtzMan->PtzClient())
		{
			unsigned long ret = PTZ_ERR_OPERATE_FAIL;
			MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_PTZ_START_FAIL, reinterpret_cast<unsigned char *>(&ret), sizeof(unsigned long));
		}
		else
		{
			m_pPtzMan->EnterPtz(msgData.clientID);
			MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_PTZ_START_SUCC, NULL, 0);
		

			WriteLog(msgData.clientID, LOG_TYPE_PTZ_ENTER);
		}

		m_pMsgMan->PutMsgToClient(m_localMsgData); 
	}
	else if (CMD_REQUEST_PTZ_STOP == cmdType)
	{
		assert(msgData.clientID == m_pPtzMan->PtzClient());
		m_pPtzMan->ExitPtz();

		WriteLog(msgData.clientID, LOG_TYPE_PTZ_EXIT);
	}
	else if (CMD_REQUEST_PTZ_CTRL == cmdType)
	{
		unsigned long PTZCtrlRtrnValue = PTZ_ERR_OPERATE_FAIL;

		if (INVALID_CLIENT_ID == m_pPtzMan->PtzClient() ||
			msgData.clientID == m_pPtzMan->PtzClient())
		{
			PTZCtrlRtrnValue = PTZ_ERR_OK;
			PTZ_CTRL *pPTZCtrl = reinterpret_cast<PTZ_CTRL *>(msgData.pData + sizeof(PACKCMD));

			if(pPTZCtrl->chnn < m_localVideoInputNum)
			{
				PTZCtrlRtrnValue = m_pPtzMan->Action(pPTZCtrl->chnn, pPTZCtrl->cmdType, pPTZCtrl->cmdValue, pPTZCtrl->speed);
			}
			else
			{
#ifdef __DVR_ULTIMATE__
				if ((pPTZCtrl->cmdType == PTZ_CMD_PRESET_SET)||(pPTZCtrl->cmdType == PTZ_CMD_PRESET_GO)||(pPTZCtrl->cmdType == PTZ_CMD_PRESET_DEL))
				{	
					if (m_pPtzMan->PresetIsEnable(pPTZCtrl->chnn,pPTZCtrl->cmdValue))
					{
						if(!m_pNetDeviceMan->PTZAction(pPTZCtrl->chnn, pPTZCtrl->cmdType, pPTZCtrl->cmdValue, pPTZCtrl->speed, 0))
						{
							printf("!**!PTZ %s, %s, %d\n", __FUNCTION__, __FILE__, __LINE__);
							PTZCtrlRtrnValue = PTZ_ERR_OPERATE_FAIL;
						}
					}else{
						PTZCtrlRtrnValue = PTZ_ERR_PRESET_DISABLE;
					}
				}else{
					if(!m_pNetDeviceMan->PTZAction(pPTZCtrl->chnn, pPTZCtrl->cmdType, pPTZCtrl->cmdValue, pPTZCtrl->speed, 0))
					{
						printf("!**!PTZ %s, %s, %d\n", __FUNCTION__, __FILE__, __LINE__);
						PTZCtrlRtrnValue = PTZ_ERR_OPERATE_FAIL;
					}
				}
#endif
			}
		}

		printf("!**!PTZ %s, %s, %d\n", __FUNCTION__, __FILE__, __LINE__);

		if (PTZ_ERR_OK == PTZCtrlRtrnValue)
		{
			MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_PTZ_SEND_CMD_SUC,reinterpret_cast< unsigned char *>( &PTZCtrlRtrnValue ), sizeof( unsigned long ) );
		}
		else
		{
			MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_PTZ_SEND_CMD_FAIL, reinterpret_cast< unsigned char *>( &PTZCtrlRtrnValue ), sizeof( unsigned long ) );
		}
		m_pMsgMan->PutMsgToClient(m_localMsgData);

		WriteLog(msgData.clientID, LOG_TYPE_PTZ_CTRL);
	}
	else if (CMD_REQUEST_PTZ_STATUS == cmdType)
	{
		unsigned long chnn = *reinterpret_cast<unsigned long *>( msgData.pData + sizeof( PACKCMD ) );
		unsigned long status = PTZ_ST_NULL;
		
		if(chnn < m_localVideoInputNum)
		{
			status = m_pPtzMan->GetPtzState( chnn );
		}

		MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_PTZ_STATUS_SUC, reinterpret_cast< unsigned char * >( &status ), sizeof( unsigned long ) );
		m_pMsgMan->PutMsgToClient(m_localMsgData);

	}
	else if( CMD_REQUEST_PTZ_STOP_CURRENT_STATUS == cmdType )
	{
		unsigned long ptzRetValue = PTZ_ERR_OPERATE_FAIL;

		if (INVALID_CLIENT_ID == m_pPtzMan->PtzClient() ||
			msgData.clientID == m_pPtzMan->PtzClient())
		{
			unsigned long chnn = *reinterpret_cast<unsigned long *>( msgData.pData + sizeof( PACKCMD ) );
			ptzRetValue = PTZ_ERR_OK;

			ptzRetValue = m_pPtzMan->Action(chnn, PTZ_CMD_RESET, 0);
		}

		if (PTZ_ERR_OK == ptzRetValue)
		{
			MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_PTZ_SEND_CMD_SUC,reinterpret_cast< unsigned char *>( &ptzRetValue ), sizeof( unsigned long ) );
		}
		else
		{
			MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_PTZ_SEND_CMD_FAIL, reinterpret_cast< unsigned char *>( &ptzRetValue ), sizeof( unsigned long ) );
		}

		m_pMsgMan->PutMsgToClient(m_localMsgData);
	}
	else if( CMD_REQUEST_PTZ_CRUISE_PREVIEW == cmdType )			//请求巡航线预览
	{
		assert( NULL != msgData.pData );
		assert( sizeof( PACKCMD ) <= msgData.dataLen );

		unsigned long ptzRetValue = PTZ_ERR_OPERATE_FAIL;
		if (INVALID_CLIENT_ID == m_pPtzMan->PtzClient() ||
			msgData.clientID == m_pPtzMan->PtzClient())
		{
			if(m_pPtzMan->SetCruisePreviewInfo( reinterpret_cast< const unsigned char * >( msgData.pData + sizeof( PACKCMD ) ) ))
			{
				ptzRetValue = PTZ_ERR_OK;
			}
		}

		if (PTZ_ERR_OK == ptzRetValue)
		{
			MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_PTZ_CRUISE_PREVIEW_SUC, reinterpret_cast< unsigned char * >( &ptzRetValue ), sizeof( unsigned long ) );
		}
		else
		{
			MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_PTZ_CRUISE_PREVIEW_FAIL, reinterpret_cast< unsigned char * >( &ptzRetValue ), sizeof( unsigned long ) );
		}

		m_pMsgMan->PutMsgToClient(m_localMsgData);
	}
	else if( CMD_REQUEST_PTZ_CRUISE_PREVIEW_RECOVER == cmdType )	//恢复预览后没有保存的巡航信息
	{
		assert( NULL != msgData.pData );
		assert( sizeof( PACKCMD ) <= msgData.dataLen );

		unsigned long ptzRetValue = PTZ_ERR_OPERATE_FAIL;
		if (INVALID_CLIENT_ID == m_pPtzMan->PtzClient() ||
			msgData.clientID == m_pPtzMan->PtzClient())
		{
			m_pPtzMan->RecoverCruisePreviewInfo( *reinterpret_cast< unsigned long * >( msgData.pData + sizeof( PACKCMD ) ) );
		}
		else
		{
			assert(false);
		}
	}
	else if(CMD_REQUEST_MOTION_AREA_SET == cmdType)
	{
		NET_MOTION_INFO* pNetMotionInfo = reinterpret_cast<NET_MOTION_INFO*>(msgData.pData + sizeof(PACKCMD));

		unsigned long num = (msgData.dataLen - sizeof(PACKCMD))/sizeof(NET_MOTION_INFO);
		assert ((0 < num) && (num <= m_videoInputNum));

		num = num<m_localVideoInputNum?num:m_localVideoInputNum;
		for(int i = 0; i < num; i++)
		{
			//[2010-04-23 16:00 L] 需设置的通道在NET_MOTION_INFO中指定
			if (pNetMotionInfo->chnn < m_localVideoInputNum)
			{
				m_pLocalDeviceMan->SetMotionInfo(pNetMotionInfo->chnn, &pNetMotionInfo[i].detectInfo);
			}
		}
	}
	else if (CMD_REQUEST_MANUAL_REC == cmdType)
	{
		//注意静态
		static ULONGLONG CH = 0x00;
		static bool bStart = true;

		ULONGLONG authCH = *reinterpret_cast<ULONGLONG *>(msgData.pData + sizeof(PACKCMD));
		for (unsigned char i=0; i<m_videoInputNum; ++i)
		{
			//具备权限，才改变。
			if (authCH & ((ULONGLONG)0x01 << i))
			{
				if (bStart)
				{
					m_pStreamRecManEx->Record(i, RECORD_TYPE_MANUAL);
					CH |= ((ULONGLONG)0x01 << i);
				}
				else
				{
					m_pStreamRecManEx->StopRecord(i, RECORD_TYPE_MANUAL);
					CH &= ~((ULONGLONG)0x01 << i);
				}
			}			
		}

		if (bStart)
		{
			WriteLog(msgData.clientID, LOG_TYPE_MANUAL_START);
		}
		else
		{
			WriteLog(msgData.clientID, LOG_TYPE_MANUAL_STOP);
		}

		//是否为开启，则每次都修改。
		bStart = !bStart;
		{
			unsigned long dataLen = sizeof(ULONGLONG) + sizeof( NCFG_BLOCK_HEAD ) + sizeof( NCFG_ITEM_HEAD );
			unsigned char * pData = new unsigned char [dataLen];

			NCFG_BLOCK_HEAD *pBlockHead = reinterpret_cast<NCFG_BLOCK_HEAD *>( pData );
			pBlockHead->biSize		= sizeof(NCFG_BLOCK_HEAD);
			pBlockHead->ItemNum		= 1;
			pBlockHead->netcfgver	= NCFG_VERSION;

			NCFG_ITEM_HEAD *pItemHead = reinterpret_cast< NCFG_ITEM_HEAD * >( pData + sizeof( NCFG_BLOCK_HEAD ) );
			pItemHead->itemID = NCFG_ITEM_MANUAL_REC;
			pItemHead->len	  = sizeof(ULONGLONG);
			pItemHead->num	  = 1;
			pItemHead->subLen = sizeof(ULONGLONG);

			memcpy(pData+sizeof(NCFG_BLOCK_HEAD)+sizeof(NCFG_ITEM_HEAD), &CH, sizeof(ULONGLONG));

			if (!m_pConfig->SetConfig( pData, dataLen ))
			{
				assert (false);
			}

			delete [] pData;
			pData = NULL;
		}		
	}
	else if(CMD_REQUEST_GB81_REC_CTRL == cmdType)
	{
		ULONGLONG CHGB81 = 0x00;
		MANUALRECSTATUS recInfo = *reinterpret_cast<MANUALRECSTATUS *>(msgData.pData + sizeof(PACKCMD));
		if(msgData.deviceID != LOCAL_DEVICE_ID)
		{
			//检查权限
			ULONGLONG auth = CUserMan::Instance()->CheckAuthority(AUTH_RECORD,msgData.clientID);
			recInfo.authCH = recInfo.authCH & auth;
		}

		for (unsigned char i=0; i<m_videoInputNum; ++i)
		{
			//具备权限，才改变。
			if (recInfo.authCH & ((ULONGLONG)0x01 << i))
			{
				if (recInfo.bStart)
				{
					m_pRecMan->Record(i, RECORD_TYPE_MANUAL);
					CHGB81 |= ((ULONGLONG)0x01 << i);
				}
				else
				{
					m_pRecMan->StopRecord(i, RECORD_TYPE_MANUAL);
					CHGB81 &= ~((ULONGLONG)0x01 << i);
				}
			}			
		}

		if (recInfo.bStart)
		{
			WriteLog(msgData.clientID, LOG_TYPE_MANUAL_START);
		}
		else
		{
			WriteLog(msgData.clientID, LOG_TYPE_MANUAL_STOP);
		}

		////是否为开启，则每次都修改。
		{
			unsigned long dataLen = sizeof(ULONGLONG) + sizeof( NCFG_BLOCK_HEAD ) + sizeof( NCFG_ITEM_HEAD );
			unsigned char * pData = new unsigned char [dataLen];

			NCFG_BLOCK_HEAD *pBlockHead = reinterpret_cast<NCFG_BLOCK_HEAD *>( pData );
			pBlockHead->biSize		= sizeof(NCFG_BLOCK_HEAD);
			pBlockHead->ItemNum		= 1;
			pBlockHead->netcfgver	= NCFG_VERSION;

			NCFG_ITEM_HEAD *pItemHead = reinterpret_cast< NCFG_ITEM_HEAD * >( pData + sizeof( NCFG_BLOCK_HEAD ) );
			pItemHead->itemID = NCFG_ITEM_MANUAL_REC;
			pItemHead->len	  = sizeof(ULONGLONG);
			pItemHead->num	  = 1;
			pItemHead->subLen = sizeof(ULONGLONG);

			memcpy(pData+sizeof(NCFG_BLOCK_HEAD)+sizeof(NCFG_ITEM_HEAD), &CHGB81, sizeof(ULONGLONG));

			if (!m_pConfig->SetConfig( pData, dataLen ))
			{
				assert (false);
			}

			delete [] pData;
			pData = NULL;
		}		
	}
	else if (CMD_REQUEST_CHANGE_DISPLAY_MODE == cmdType)
	{
		NET_DISPLAY_INFO curDisp;
		memset(&curDisp, 0, sizeof(NET_DISPLAY_INFO));
		if (sizeof(PACKCMD) != msgData.dataLen)
		{
			unsigned long defaultTimeOut = 30;//默认延时时间
			NET_DISPLAY_INFO* pDisplay = reinterpret_cast<NET_DISPLAY_INFO *>(msgData.pData + sizeof(PACKCMD));
			if(pDisplay->type == NET_DISPLAY_CHANGE_DATA_PACK)
			{
				unsigned long timeout = (pDisplay->time_out == static_cast<unsigned short>(-1)) ? DISPLAY_FREEZE : pDisplay->time_out;
				timeout = (timeout == 0) ? defaultTimeOut : timeout;
#ifdef __DVR_ULTIMATE__
				CIPCDecDisplayMan::Instance()->Stop();
#endif
				m_pDisplayMan->ChangeDisplay(pDisplay->split_mode, DISPLAY_LAYER_MAX, pDisplay->data, pDisplay->data_len, timeout, pDisplay->rec, 0, false, true);
#ifdef __DVR_ULTIMATE__
				CIPCDecDisplayMan::Instance()->Start();
				ULONGLONG displayCH, displayCHMinor;
				CIPCDecDisplayMan::Instance()->GetDisplayCH(displayCH, displayCHMinor);
				m_pNetDeviceMan->KeyFrame(displayCH, 0, 0);
				m_pNetDeviceMan->RequestMinorStream(displayCHMinor);
#endif
			}
			else if(pDisplay->type == NET_DISPLAY_CHANGE_XY_PT)
			{
				unsigned long timeout = (pDisplay->time_out == static_cast<unsigned short>(-1) ) ? DISPLAY_FREEZE : pDisplay->time_out;
				timeout = (timeout == 0) ? defaultTimeOut : timeout;
#ifdef __DVR_ULTIMATE__
				CIPCDecDisplayMan::Instance()->Stop();
#endif
				m_pDisplayMan->ChangeDisplay(pDisplay->x, pDisplay->y, timeout, pDisplay->rec);
#ifdef __DVR_ULTIMATE__
				CIPCDecDisplayMan::Instance()->Start();
				ULONGLONG displayCH, displayCHMinor;
				CIPCDecDisplayMan::Instance()->GetDisplayCH(displayCH, displayCHMinor);
				m_pNetDeviceMan->KeyFrame(displayCH, 0, 0);
				m_pNetDeviceMan->RequestMinorStream(displayCHMinor);
#endif			
			}
			else if(pDisplay->type == NET_DISPLAY_FREEZE)
			{
				m_pDisplayMan->FreezeDisplay();
			}
			else if(pDisplay->type == NET_DISPLAY_UNFREEZE)
			{
#ifdef __DVR_ULTIMATE__
				CIPCDecDisplayMan::Instance()->Stop();
#endif
				m_pDisplayMan->PopDisplay();
#ifdef __DVR_ULTIMATE__
				CIPCDecDisplayMan::Instance()->Start();
				ULONGLONG displayCH, displayCHMinor;
				CIPCDecDisplayMan::Instance()->GetDisplayCH(displayCH, displayCHMinor);
				m_pNetDeviceMan->KeyFrame(displayCH, 0, 0);
				m_pNetDeviceMan->RequestMinorStream(displayCHMinor);
#endif
				CMessageMan::Instance()->RefreshDisplay(0);	
			}
			else if(pDisplay->type == NET_DISPLAY_EXCHANGE_TWO_PT)
			{
				if(pDisplay->data_len >= 4)
				{
					unsigned long x1 = pDisplay->data[0];
					unsigned long y1 = pDisplay->data[1];
					unsigned long x2 = pDisplay->data[2];
					unsigned long y2 = pDisplay->data[3];

					unsigned long timeout = (pDisplay->time_out == static_cast<unsigned short>(-1)) ? DISPLAY_FREEZE : pDisplay->time_out;
					timeout = (timeout == 0) ? defaultTimeOut : timeout;

					unsigned char chnn1 = INVALID_INDEX(unsigned char);
					unsigned char chnn2 = INVALID_INDEX(unsigned char);
					unsigned char index1 = INVALID_INDEX(unsigned char);
					unsigned char index2 = INVALID_INDEX(unsigned char);

					unsigned long hitsChnn[MAX_CHNN_NUM] = {0};
					int hits = m_pDisplayMan->HitChnn(x1, y1, hitsChnn, MAX_CHNN_NUM);
					for (int i = 0; i < hits; i++)
					{
						if( (DISPLAY_SIGNAL_ANALOG == DISPLAY_GET_SIGNAL(hitsChnn[i])) || \
							(DISPLAY_SIGNAL_NET == DISPLAY_GET_SIGNAL(hitsChnn[i])) )
						{
							if(DISPLAY_INVALID != DISPLAY_GET_CHNN(hitsChnn[i]))
							{
								chnn1 = DISPLAY_GET_CHNN(hitsChnn[i]);
								index1 = DISPLAY_GET_INDEX(hitsChnn[i]);
								break;
							}
						}
					}
					hits = m_pDisplayMan->HitChnn(x2, y2, hitsChnn, MAX_CHNN_NUM);
					for (int i = 0; i < hits; i++)
					{
						if( (DISPLAY_SIGNAL_ANALOG == DISPLAY_GET_SIGNAL(hitsChnn[i])) || \
							(DISPLAY_SIGNAL_NET == DISPLAY_GET_SIGNAL(hitsChnn[i])) )
						{
							if(DISPLAY_INVALID != DISPLAY_GET_CHNN(hitsChnn[i]))
							{
								chnn2 = DISPLAY_GET_CHNN(hitsChnn[i]);
								index2 = DISPLAY_GET_INDEX(hitsChnn[i]);
								break;
							}
						}
					}

					unsigned long bDifferentChnn = true;
					if((chnn1 == INVALID_INDEX(unsigned char)) || \
						(chnn2 == INVALID_INDEX(unsigned char)) || ((chnn1 == chnn2) && (index1 == index2)))
					{
						bDifferentChnn = false;
					}

					if(bDifferentChnn)
					{
#ifdef __DVR_ULTIMATE__
						CIPCDecDisplayMan::Instance()->Stop();
#endif
						m_pDisplayMan->ChangeDisplay(x1, y1, x2, y2, timeout, pDisplay->rec);
#ifdef __DVR_ULTIMATE__
						CIPCDecDisplayMan::Instance()->Start();
						ULONGLONG displayCH, displayCHMinor;
						CIPCDecDisplayMan::Instance()->GetDisplayCH(displayCH, displayCHMinor);
						m_pNetDeviceMan->KeyFrame(displayCH, 0, 0);
						m_pNetDeviceMan->RequestMinorStream(displayCHMinor);
#endif
					}
				}
				else
				{
					printf("Change Display. Param Error. type = %d.data len = %d.\n", pDisplay->type, pDisplay->data_len);
					assert(false);
				}
			}
			else if(pDisplay->type == NET_DISPLAY_SET_MINIWND_POS)
			{
				if(pDisplay->data_len >= 4)
				{
					unsigned long x = pDisplay->data[0];
					unsigned long y = pDisplay->data[1];
					unsigned long cx = pDisplay->data[2];
					unsigned long cy = pDisplay->data[3];

#ifdef __DVR_ULTIMATE__
					CIPCDecDisplayMan::Instance()->Stop();
#endif
					m_pDisplayMan->SetMiniWndPos(x, y, cx, cy);
#ifdef __DVR_ULTIMATE__
					CIPCDecDisplayMan::Instance()->Start();
					ULONGLONG displayCH, displayCHMinor;
					CIPCDecDisplayMan::Instance()->GetDisplayCH(displayCH, displayCHMinor);
					m_pNetDeviceMan->KeyFrame(displayCH, 0, 0);
					m_pNetDeviceMan->RequestMinorStream(displayCHMinor);
#endif
				}
				else
				{
					printf("Change Display. Param Error. type = %d.data len = %d.\n", pDisplay->type, pDisplay->data_len);
					assert(false);
				}
			}
			else if(pDisplay->type == NET_DISPLAY_FILTER)
			{
				unsigned long timeout = (pDisplay->time_out == static_cast<unsigned short>(-1)) ? DISPLAY_FREEZE : pDisplay->time_out;
				timeout = (timeout == 0) ? defaultTimeOut : timeout;
#ifdef __DVR_ULTIMATE__
				CIPCDecDisplayMan::Instance()->Stop();
#endif
				m_pDisplayMan->ChangeDisplay(pDisplay->split_mode, DISPLAY_LAYER_MAX, pDisplay->data, pDisplay->data_len, timeout, pDisplay->rec, 0, true);
#ifdef __DVR_ULTIMATE__
				CIPCDecDisplayMan::Instance()->Start();
				ULONGLONG displayCH, displayCHMinor;
				CIPCDecDisplayMan::Instance()->GetDisplayCH(displayCH, displayCHMinor);
				m_pNetDeviceMan->KeyFrame(displayCH, 0, 0);
				m_pNetDeviceMan->RequestMinorStream(displayCHMinor);
#endif
			}
			else
			{
				printf("Change Display. Param Error. type = 0x%X.\n", pDisplay->type);
				assert(false);
			}

			m_pDisplayMan->GetDisplayInfo(curDisp);
		}
		else
		{
			//恢复冻结
			m_pDisplayMan->PopDisplay();

#ifdef __DVR_ULTIMATE__
			CIPCDecDisplayMan::Instance()->Start();
			ULONGLONG displayCH, displayCHMinor;
			CIPCDecDisplayMan::Instance()->GetDisplayCH(displayCH, displayCHMinor);
			m_pNetDeviceMan->KeyFrame(displayCH, 0, 0);
			m_pNetDeviceMan->RequestMinorStream(displayCHMinor);
#endif

			CMessageMan::Instance()->RefreshDisplay(0);

			m_pDisplayMan->GetDisplayInfo(curDisp);
		}

		MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_CHANAGE_DISPLAY_SUCC, (unsigned char*)(&curDisp), sizeof(NET_DISPLAY_INFO));

		m_pMsgMan->PutMsgToClient(m_localMsgData);
	}
	else if(CMD_REQUEST_GET_DISPLAY_INFO == cmdType)
	{
		NET_DISPLAY_INFO dispInfo;
		memset(&dispInfo, 0, sizeof(NET_DISPLAY_INFO));

		if (sizeof(PACKCMD) != msgData.dataLen)
		{

			NET_DISPLAY_INFO* pDisplay = reinterpret_cast<NET_DISPLAY_INFO *>(msgData.pData + sizeof(PACKCMD));
		
			if(pDisplay->type == NET_DISPLAY_GET_INFO)
			{
				m_pDisplayMan->GetDisplayInfo(dispInfo);
			}
			else if(pDisplay->type == NET_DISPLAY_GET_INFO_XY)
			{
				m_pDisplayMan->GetDisplayInfo(dispInfo, pDisplay->x, pDisplay->y, true);
			}
			else
			{
				m_pDisplayMan->GetDisplayInfo(dispInfo);

				printf("Change Display. Param Error. type = 0x%X.\n", pDisplay->type);
				assert(false);
			}
			
			MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_CTRL_SUCC, reinterpret_cast<unsigned char*>(&dispInfo), sizeof(NET_DISPLAY_INFO));
		}
		else
		{
			m_pDisplayMan->GetDisplayInfo(dispInfo);

			MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_CTRL_SUCC, reinterpret_cast<unsigned char*>(&dispInfo), sizeof(NET_DISPLAY_INFO));
		}

		m_pMsgMan->PutMsgToClient(m_localMsgData);
	}
	else if (CMD_REQUEST_DODWELL_START == cmdType)
	{
		m_pDisplayMan->SetEnable(true);

		WriteLog(msgData.clientID, LOG_TYPE_DWELL_ENABLE);
	}
	else if(CMD_REQUEST_DODWELL_STOP == cmdType)
	{
		m_pDisplayMan->SetEnable(false);

		WriteLog(msgData.clientID, LOG_TYPE_DWELL_DISENABLE);
	}
	else if (CMD_REQUEST_OPEN_ALARM == cmdType)
	{
		ULONGLONG alarm = *reinterpret_cast<ULONGLONG *>(msgData.pData + sizeof(PACKCMD));

		for (int i=0; i<m_sensorOutNum; ++i)
		{
			if (alarm & ((ULONGLONG)(0x01) << i))
			{
				m_pLocalDeviceMan->SetSensorOutStatus(i, true);
			}
		}

		m_pLocalDeviceMan->SetBuzzerStatus(true);

		WriteLog(msgData.clientID, LOG_TYPE_OPEN_ALARM);
	}
	else if (CMD_REQUEST_CLEAN_ALARM == cmdType)
	{
		ULONGLONG alarm = *reinterpret_cast<ULONGLONG *>(msgData.pData + sizeof(PACKCMD));

		for (int i=0; i<m_sensorOutNum; ++i)
		{
			if (alarm & ((ULONGLONG)(0x01) << i))
			{
				m_pLocalDeviceMan->SetSensorOutStatus(i, false);
			}
		}

		m_pLocalDeviceMan->SetBuzzerStatus(false);

		WriteLog(msgData.clientID, LOG_TYPE_CLEAR_ALARM);
	}
	else if (CMD_REQUEST_EVICT_SOMEONE == cmdType)
	{
		unsigned long clientID = *reinterpret_cast<unsigned long *>(msgData.pData + sizeof(PACKCMD));

		//先加入屏蔽名单
		m_pUserMan->AddDenyClientID(clientID, 300);

		//先发送断开命令给指定的客户端
		MsgData(m_localMsgData, clientID, CMD_REPLY_DISCONNECT, NULL, 0);
		m_pMsgMan->PutMsgToClient(m_localMsgData);

		//然后回复断开结果给操作客户端
		MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_CTRL_SUCC, NULL, 0);
		m_pMsgMan->PutMsgToClient(m_localMsgData);
	}
	else if (CMD_REQUEST_ZOOM == cmdType)
	{
		ZOOM_INFO *pZoom = reinterpret_cast<ZOOM_INFO *>(msgData.pData + sizeof(PACKCMD));

		m_pLocalDeviceMan->Zoom((pZoom->width > 0) ? true : false, pZoom->x, pZoom->y, pZoom->width, pZoom->height);
	}
	else if (CMD_REQUEST_CHANGE_TIME == cmdType)
	{
		//CMD_REQUEST_CHANGE_TIME   网络客户端请求修改时间，是带返回值的	
		//CMD_REQUEST_L_CHANGE_TIME 本地客户端请求修改时间，是不带返回值的	
		//分开的原因是：立即同步时间功能，用的是send CMD_REQUEST_SYN_TIME_NOW，
		//在处理这个命令时，又要修改时间，如果用 send CMD_REQUEST_CHANGE_TIME
		//会导致死锁。如果用post CMD_REQUEST_L_CHANGE_TIME命令，不会死锁，但是
		//这个并没有修改时间，而等CMD_REQUEST_SYN_TIME_NOW执行完后，在去执行CMD_REQUEST_L_CHANGE_TIME
		//所以会出现立即同步时间提示成功，但是实际并没有修改时间.
		//
		//目前先保留这个两个修改时间的命令。
		//以后要去掉命令CMD_REQUEST_L_CHANGE_TIME

#ifdef __ENVIRONMENT_LINUX__
		if ((!m_bOperForFormating) && (!m_bOperForLocalBackuping) && (!m_bOperForLocalPlayBackup))
		{		
			unsigned long time = *reinterpret_cast<unsigned long *>(msgData.pData + sizeof(PACKCMD));
			tm tmpTm = DVRTime32ToTm(time);
			if(tmpTm.tm_year+1900 > 2025)
			{
				MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_CHANAGE_TIME_FAIL, NULL, 0);
			}
			else
			{
			//停掉录像
			m_pNetReader->Action(false);
			m_pMotionMan->Action(false);   
			m_pSensorInMan->Action(false);
			m_pVideoLossMan->Action(false);
			m_pOtherAlarm->Action(false);
#ifdef __MOBILE_DVR__
			m_pSpeedMan->Action(false);
#endif
			m_pRecMan->Action(false); 
			m_pReclogMan->ActionAllDisk(false);
			m_pLogMan->ActionDisk(NULL, 0, false);
#ifdef __DVR_BASIC__
			m_pSmtpMan->Action(false);
			m_pSnapProc->Action(false);
#endif

#if defined(__ALARM_TO_UPDATE_FTP_VIDEO__)
			CFtpUpdateMan::Instance()->Action(false);
#endif
			m_pStreamProc->Action(false);  //只有在清空缓存的帧时，才用。
			//如果只是停录像，就不需要该函数。
#ifdef __DVR_ULTIMATE__
			m_pNetDeviceMan->DeviceAction(false);
			m_pNetDeviceMan->ClearDataBuffer();
			ULONGLONG ullDisplayCH, ullDisplayCHMinor;
			CIPCDecDisplayMan::Instance()->GetDisplayCH(ullDisplayCH, ullDisplayCHMinor);
			CIPCDecDisplayMan::Instance()->Stop();
#endif

			//修改时间
			unsigned long time = *reinterpret_cast<unsigned long *>(msgData.pData + sizeof(PACKCMD));
			m_pLocalDeviceMan->SetSystemTime(time);
			//成功修改时间后，需要更新m_todayTimeL变量
			m_todayTimeL = GetZeroTime32OfDate(GetCurrTime32());

#ifdef __DVR_ULTIMATE__
			m_pNetDeviceMan->SyncTimeAll();
#endif

			//清空一些资源
			m_pMsgMan->ClearFrame(LIVE_FRAME_FIRST_STREAM | LIVE_FRAME_SECOND_STREAM | LIVE_FRAME_JPEG);
#ifdef __NETSERVER_TVT__
			CNetServer::Instance()->ClearData(LIVE_FRAME_FIRST_STREAM | LIVE_FRAME_SECOND_STREAM);	//本行与上一行交换，防止数据清空不完全lh_modify20100416
#endif
			m_pRecMan->RemoveAllDate();
#ifdef __DVR_BASIC__
			m_pSnapProc->RemoveAllData();
#endif
			m_pStreamProc->ClearData();
			m_pLocalDeviceMan->ClearBuffer();

#ifdef __DVR_ULTIMATE__
			if((ullDisplayCH != 0) ||(ullDisplayCHMinor != 0))
			{
				printf("ullDisplayCH = %x, ullDisplayCHMinor = %x, %s, %d\n", (unsigned long)ullDisplayCH, (unsigned long)ullDisplayCHMinor, __FILE__, __LINE__);
				CIPCDecDisplayMan::Instance()->SetDisplayCH(ullDisplayCH, ullDisplayCHMinor);
			}
			CIPCDecDisplayMan::Instance()->Start();
			m_pNetDeviceMan->DeviceAction(true);
			m_pNetDeviceMan->KeyFrame(ullDisplayCH, 0, 0);
			m_pNetDeviceMan->RequestMinorStream(ullDisplayCHMinor);
#endif
			
			//开启录像	
			m_pStreamProc->Action(true);

#if defined(__ALARM_TO_UPDATE_FTP_VIDEO__)
			CFtpUpdateMan::Instance()->Action(true);
#endif


#ifdef __DVR_BASIC__
			m_pSnapProc->Action(true);
			m_pSmtpMan->Action(true);
#endif
			m_pLogMan->ActionDisk(NULL, 0, true);
			m_pReclogMan->ActionAllDisk(true);
			m_pRecMan->Action(true);
			m_pOtherAlarm->Action(true);
			m_pVideoLossMan->Action(true);
			m_pSensorInMan->Action(true);
			m_pMotionMan->Action(true);
			m_pNetReader->Action(true);
#ifdef __MOBILE_DVR__
			m_pSpeedMan->Action(true);
#endif
			MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_CHANAGE_TIME_SUCC, NULL, 0);
			WriteLog(msgData.clientID, LOG_TYPE_CHANGE_TIME);
			}
		}
 		else
 		{
 			MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_CHANAGE_TIME_FAIL, NULL, 0);
 		}
#else
	#ifdef __DVR_ULTIMATE__
		m_pNetDeviceMan->SyncTimeAll();
	#endif
		MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_CHANAGE_TIME_FAIL, NULL, 0);
#endif
		m_pMsgMan->PutMsgToClient(m_localMsgData);
	}
	else if (CMD_REQUEST_L_CHANGE_TIME == cmdType)
	{
#ifdef __ENVIRONMENT_LINUX__
		if ((!m_bOperForFormating) && (!m_bOperForLocalBackuping) && (!m_bOperForLocalPlayBackup))
		{		
			//停掉录像
			m_pNetReader->Action(false);
			m_pMotionMan->Action(false);   
			m_pSensorInMan->Action(false);
			m_pVideoLossMan->Action(false);
			m_pOtherAlarm->Action(false);
#ifdef __MOBILE_DVR__
			m_pSpeedMan->Action(false);
#endif
			m_pRecMan->Action(false); 
			m_pReclogMan->ActionAllDisk(false);
			m_pLogMan->ActionDisk(NULL, 0, false);
#ifdef __DVR_BASIC__
			m_pSmtpMan->Action(false);
			m_pSnapProc->Action(false);
#endif

#if defined(__ALARM_TO_UPDATE_FTP_VIDEO__)
			CFtpUpdateMan::Instance()->Action(false);
#endif

			m_pStreamProc->Action(false);  //只有在清空缓存的帧时，才用。
			//如果只是停录像，就不需要该函数。

#ifdef __DVR_ULTIMATE__
			m_pNetDeviceMan->DeviceAction(false);
			m_pNetDeviceMan->ClearDataBuffer();
			ULONGLONG displayCH, displayCHMinor;
			CIPCDecDisplayMan::Instance()->GetDisplayCH(displayCH, displayCHMinor);
			CIPCDecDisplayMan::Instance()->Stop();
#endif

			//修改时间
			unsigned long time = *reinterpret_cast<unsigned long *>(msgData.pData + sizeof(PACKCMD));
			m_pLocalDeviceMan->SetSystemTime(time);
			//成功修改时间后，需要更新m_todayTimeL变量
			m_todayTimeL = GetZeroTime32OfDate(GetCurrTime32());

#ifdef __DVR_ULTIMATE__
			m_pNetDeviceMan->SyncTimeAll();
#endif


			//清空一些资源
			m_pMsgMan->ClearFrame(LIVE_FRAME_FIRST_STREAM | LIVE_FRAME_SECOND_STREAM | LIVE_FRAME_JPEG);
#ifdef __NETSERVER_TVT__
			CNetServer::Instance()->ClearData(LIVE_FRAME_FIRST_STREAM | LIVE_FRAME_SECOND_STREAM);	//本行与上一行交换，防止数据清空不完全lh_modify20100416
#endif
#ifdef __NETSERVER_HXHT__
			CHxConnectMan::Instance()->ClearAllFrameData();
#endif
			m_pRecMan->RemoveAllDate();
#ifdef __DVR_BASIC__
			m_pSnapProc->RemoveAllData();
#endif
			m_pStreamProc->ClearData();
			m_pLocalDeviceMan->ClearBuffer();

#ifdef __DVR_ULTIMATE__
			if((displayCH != 0) || (displayCHMinor != 0))
			{
				printf("displayCH = %x, displayCHMinor = %x, %s, %d\n", (unsigned long)displayCH, (unsigned long)displayCHMinor, __FILE__, __LINE__);
				CIPCDecDisplayMan::Instance()->SetDisplayCH(displayCH, displayCHMinor);
			}
			CIPCDecDisplayMan::Instance()->Start();
			m_pNetDeviceMan->DeviceAction(true);
			m_pNetDeviceMan->KeyFrame(displayCH, 0, 0);
			m_pNetDeviceMan->RequestMinorStream(displayCHMinor);
#endif

			//开启录像	
			m_pStreamProc->Action(true);


#if defined(__ALARM_TO_UPDATE_FTP_VIDEO__)
			CFtpUpdateMan::Instance()->Action(true);
#endif

#ifdef __DVR_BASIC__
			m_pSnapProc->Action(true);
			m_pSmtpMan->Action(true);
#endif
			m_pLogMan->ActionDisk(NULL, 0, true);
			m_pReclogMan->ActionAllDisk(true);
			m_pRecMan->Action(true);
			m_pOtherAlarm->Action(true);
			m_pVideoLossMan->Action(true);
			m_pSensorInMan->Action(true);
			m_pMotionMan->Action(true);
			m_pNetReader->Action(true);
#ifdef __MOBILE_DVR__
			m_pSpeedMan->Action(true);
#endif
			WriteLog(msgData.clientID, LOG_TYPE_CHANGE_TIME);
		}
#else
#ifdef __DVR_ULTIMATE__
		m_pNetDeviceMan->SyncTimeAll();
#endif
#endif

#if defined(__TW01_RILI__)
		m_lastAdjTmTime = DVRTimeToTm(GetCurrTime());
#endif
	}
	else if( CMD_REQUEST_L_CHANGE_ZONE_TIME == cmdType )
	{
#ifdef __ENVIRONMENT_LINUX__
		if ((!m_bOperForFormating) && (!m_bOperForLocalBackuping) && (!m_bOperForLocalPlayBackup))
		{
			ZONE_TIME_INFO* pZoneTime = reinterpret_cast<ZONE_TIME_INFO*>(msgData.pData + sizeof(PACKCMD));

			CTZoneDZoneCfg* pTZone = CTZoneDZoneCfg::Instance();
			long zone = static_cast<long>(pZoneTime->TimeZone);
			bool bSetZone = (0 != pZoneTime->UseZone && pTZone->GetTimeZone() != zone);
			bool bSetTime = (0 != pZoneTime->UseTime);
			if( bSetZone || bSetTime )
			{
				//停掉录像
				m_pNetReader->Action(false);
				m_pMotionMan->Action(false);   
				m_pSensorInMan->Action(false);
				m_pVideoLossMan->Action(false);
				m_pOtherAlarm->Action(false);
#ifdef __MOBILE_DVR__
				m_pSpeedMan->Action(false);
#endif
				m_pRecMan->Action(false); 
				m_pReclogMan->ActionAllDisk(false);
				m_pLogMan->ActionDisk(NULL, 0, false);
#ifdef __DVR_BASIC__
				m_pSmtpMan->Action(false);
				m_pSnapProc->Action(false);
#endif

#if defined(__ALARM_TO_UPDATE_FTP_VIDEO__)
				CFtpUpdateMan::Instance()->Action(false);
#endif
				m_pStreamProc->Action(false);  //只有在清空缓存的帧时，才用。
				//如果只是停录像，就不需要该函数。

#ifdef __DVR_ULTIMATE__
				m_pNetDeviceMan->DeviceAction(false);
				m_pNetDeviceMan->ClearDataBuffer();
				ULONGLONG displayCH, displayCHMinor;
				CIPCDecDisplayMan::Instance()->GetDisplayCH(displayCH, displayCHMinor);
				CIPCDecDisplayMan::Instance()->Stop();
#endif

				//修改时区
				if( bSetZone )
				{
					pTZone->SetTimeZone(zone);
				}
				//修改时间
				if( bSetTime )
				{
					tm tmpTm;
					tmpTm.tm_year = static_cast<int>(pZoneTime->year) - 1900;
					tmpTm.tm_mon = static_cast<int>(pZoneTime->month);
					tmpTm.tm_mday = static_cast<int>(pZoneTime->mday);
					tmpTm.tm_hour = static_cast<int>(pZoneTime->hour);
					tmpTm.tm_min = static_cast<int>(pZoneTime->min);
					tmpTm.tm_sec = static_cast<int>(pZoneTime->sec);
					m_pLocalDeviceMan->SetSystemTime(TmToDVRTime32(tmpTm));

#ifdef __DVR_ULTIMATE__
					m_pNetDeviceMan->SyncTimeAll();
#endif
				}

				//清空一些资源
				m_pMsgMan->ClearFrame(LIVE_FRAME_FIRST_STREAM | LIVE_FRAME_SECOND_STREAM | LIVE_FRAME_JPEG);
#ifdef __NETSERVER_TVT__
				CNetServer::Instance()->ClearData(LIVE_FRAME_FIRST_STREAM | LIVE_FRAME_SECOND_STREAM);	//本行与上一行交换，防止数据清空不完全lh_modify20100416
#endif
				m_pRecMan->RemoveAllDate();
#ifdef __DVR_BASIC__
				m_pSnapProc->RemoveAllData();
#endif
				m_pStreamProc->ClearData();
				m_pLocalDeviceMan->ClearBuffer();

#ifdef __DVR_ULTIMATE__
				if((displayCH != 0) || (displayCHMinor))
				{
					printf("displayCH = %x, displayCHMinor = %x, %s, %d\n", (unsigned long)displayCH, (unsigned long)displayCHMinor, __FILE__, __LINE__);
					CIPCDecDisplayMan::Instance()->SetDisplayCH(displayCH, displayCHMinor);
				}
				CIPCDecDisplayMan::Instance()->Start();
				m_pNetDeviceMan->DeviceAction(true);
				m_pNetDeviceMan->KeyFrame(displayCH, 0, 0);
				m_pNetDeviceMan->RequestMinorStream(displayCHMinor);
#endif

				//开启录像	
				m_pStreamProc->Action(true);

#if defined(__ALARM_TO_UPDATE_FTP_VIDEO__)
				CFtpUpdateMan::Instance()->Action(true);
#endif

#ifdef __DVR_BASIC__
				m_pSnapProc->Action(true);
				m_pSmtpMan->Action(true);
#endif
				m_pLogMan->ActionDisk(NULL, 0, true);
				m_pReclogMan->ActionAllDisk(true);
				m_pRecMan->Action(true);
				m_pOtherAlarm->Action(true);
				m_pVideoLossMan->Action(true);
				m_pSensorInMan->Action(true);
				m_pMotionMan->Action(true);
				m_pNetReader->Action(true);
#ifdef __MOBILE_DVR__
				m_pSpeedMan->Action(true);
#endif
				if( bSetZone )
				{
					WriteLog(msgData.clientID, LOG_TYPE_CHGE_TIME_ZONE);
				}

				if( bSetTime )
				{
					WriteLog(msgData.clientID, LOG_TYPE_CHANGE_TIME);
				}
			}
		}
#endif
	}
	else if( CMD_REQUEST_SYN_TIME_NOW == cmdType )			//NTP即时同步，需要返回值表示是否同步成功
	{
		bool bUpdateNow = false;
#ifdef __ENVIRONMENT_LINUX__
		if ((!m_bOperForFormating) && (!m_bOperForLocalBackuping) && (!m_bOperForLocalPlayBackup))
		{
			char *pNtpServerName = reinterpret_cast<char *>(msgData.pData + sizeof(PACKCMD));
			bUpdateNow = CTZoneDZoneCfg::Instance()->UpdateTimeNow( pNtpServerName );
		}
#endif
		if( bUpdateNow )
		{
			WriteLog(msgData.clientID, LOG_TYPE_NTP_MANUAL);
			MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_SYN_TIME_NOW_SUCC, NULL, 0);
		}
		else
		{
			MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_SYN_TIME_NOW_FAIL,NULL, 0);
		}
		m_pMsgMan->PutMsgToClient(m_localMsgData);
	}
	else if (CMD_REQUEST_CONTROL_LIVE_AUDIO == cmdType)
	{
//#ifdef __ENVIRONMENT_LINUX__
		NET_LIVEAUDIO_CTRL_INFO audioCtrlInfo = *reinterpret_cast<NET_LIVEAUDIO_CTRL_INFO *>(msgData.pData + sizeof(PACKCMD));
		if (NET_LIVE_AUDIO_MUTE == audioCtrlInfo.type)
		{
			m_pLocalDeviceMan->SetAudioLiveChannel(false, audioCtrlInfo.audiochnnel);

			WriteLog(msgData.clientID, LOG_TYPE_MUTE_ENABLE);
		}
		else if (NET_LIVE_AUDIO_OPEN == audioCtrlInfo.type)
		{
			m_pLocalDeviceMan->SetAudioLiveChannel(true, audioCtrlInfo.audiochnnel);
			
			//WriteLog(msgData.clientID, LOG_TYPE_AUDIO_CH_CHANGE);

		}
		else if (NET_LIVE_AUDIO_AUTO == audioCtrlInfo.type)
		{
			if (0 == audioCtrlInfo.audiochnnel)
			{
				m_pLocalDeviceMan->SetAudioAuto(false);
			}
			else
			{
				m_pLocalDeviceMan->SetAudioAuto(true);
			}
		}
		else if (NET_LIVE_AUDIO_RECOVER == audioCtrlInfo.type)
		{
			m_pLocalDeviceMan->RecoverAudioLiveChannel();
		}
		else
		{
			assert(false);
		}
//#endif		
	}
	else if (CMD_REQUEST_MODIFY_AUDIO_VOLUME == cmdType)
	{
		unsigned long volume = *reinterpret_cast<unsigned long *>(msgData.pData + sizeof(PACKCMD));
		
		if( volume != INVALID_INDEX(unsigned long))
		{
			m_pLocalDeviceMan->SetAudioVolume(volume);	
		}
		else
		{
			//在调节音量时，拖动slider结束时才写“调整音量”到日志文件
			WriteLog(msgData.clientID, LOG_TYPE_VOLUME_ADJUST);
		}
	}
	else if (CMD_REQUEST_LOCAL_CLIENT_PLAYBACK_PLAY == cmdType)
	{
		//回放调用顺序：
		//1  playbackStar；
		//2  changdisplyMode (到回放模式)
		//3  playbackGetbuf <-> playbackSetbuf
		//4  playbackStop
		//5  changedisplyMode（恢复到现场）
		ULONGLONG outCH = 0;
		bool bInitialSuc = false;
		bool bPlaySuc = false;
		LOCAL_CLINET_PLAYBACK_INFO playbackInfo = *reinterpret_cast<LOCAL_CLINET_PLAYBACK_INFO *>(msgData.pData + sizeof(PACKCMD));
        DVR_DISK_OWNER diskOwnerInfo;
		unsigned long channelNum = 0;

#ifdef __DVR_ULTIMATE__
		ULONGLONG displayCH, displayCHMinor;
		CIPCDecDisplayMan::Instance()->GetDisplayCH(displayCH, displayCHMinor);
		//启用回放前,先关闭网络流
		CIPCDecDisplayMan::Instance()->Stop();
#endif


		//一进回放就要限制子码流的帧率，而不是在回放开启成功后，再去设置。因为刘志高的sdk做了限制，只要进入回放，设置子码流的帧率就不在生效了。
		{
			//本地回放时，限制子码流的帧率
			int playChnnNum = 0;
			for(int bit = 0; bit < 64; bit++)
			{
				if(PUB_HasBit64(playbackInfo.chnnBits, bit))
				{
					playChnnNum++;
				}
			}

			m_bPlayback = true;
			m_playChNum = playChnnNum;

			//进入回放时需要限制码流
			SetStreamLimit(false);
		}

		do 
		{
#ifdef __ENVIRONMENT_LINUX__
			m_pDiskMan->GetOwnerInfo(playbackInfo.diskOwnerIndex, diskOwnerInfo);
			assert(playbackInfo.diskOwnerIndex == diskOwnerInfo.dvrIndex);
			if (NET_DISK_OWNED_BY_THIS == diskOwnerInfo.dvrIndex)
			{
				channelNum = diskOwnerInfo.dvrPara;
			}
			else
			{
				channelNum = m_pReclogMan->GetDiskChannelNum(diskOwnerInfo.dvrPara);
			}
			assert(channelNum > 0);
#else
			channelNum = m_videoInputNum;
#endif
			unsigned long actualChnnNum = channelNum;
			ADAPT_TO_VIRTUAL_CHNN_NUM(actualChnnNum,channelNum);
			ADAPT_TO_VIRTUAL_PLAYBACK_ULL_CHNN_BITS(playbackInfo.chnnBits);
			
			
#if !defined(__ASYN_PLAY__)
			if (!m_pPlaybackMan->Initial(channelNum,  CProduct::Instance()->VideoOutputNum(), CProduct::Instance()->MaxFFSpeed(), m_pDiskPath, m_pReclogMan, playbackInfo.diskOwnerIndex))
			{
				printf("Inital playback manager fail\n");
				break;
			}
            bInitialSuc = true;

			if (PLAYBACK_ON_TIME == playbackInfo.playbackType)
			{
				bPlaySuc = m_pPlaybackMan->Play(playbackInfo.starTime, playbackInfo.endTime, playbackInfo.curTime, playbackInfo.chnnBits,(MESSAGE_CALLBACK)playbackInfo.pMsgCallBack, (void *)playbackInfo.pCallBackObj, outCH);
			}
			else
			{
				bPlaySuc = m_pPlaybackMan->Play(reinterpret_cast<FILE_INFO_EX_LIST *>(playbackInfo.pFileList), playbackInfo.curTime, (MESSAGE_CALLBACK)playbackInfo.pMsgCallBack, (void *)playbackInfo.pCallBackObj, outCH);
			}
#else
			if (!m_pPlaybackManAsyn->Initial(channelNum,  CProduct::Instance()->LocalVideoInputNum(), CProduct::Instance()->MaxFFSpeed(), m_pDiskPath, m_pReclogMan, playbackInfo.diskOwnerIndex, playbackInfo.bSync))
			{
				printf("Inital playback manager fail\n");
				break;
			}
			bInitialSuc = true;

			if (PLAYBACK_ON_TIME == playbackInfo.playbackType)
			{
				bPlaySuc = m_pPlaybackManAsyn->Play(playbackInfo.starTime, playbackInfo.endTime, playbackInfo.curTime, playbackInfo.chnnBits,(MESSAGE_CALLBACK)playbackInfo.pMsgCallBack, (void *)playbackInfo.pCallBackObj, outCH);
			}
			else
			{
				//bPlaySuc = m_pPlaybackMan->Play(reinterpret_cast<FILE_INFO_EX_LIST *>(playbackInfo.pFileList), playbackInfo.curTime, (MESSAGE_CALLBACK)playbackInfo.pMsgCallBack, (void *)playbackInfo.pCallBackObj, outCH);
			}
#endif
		} while(0);

		if(bInitialSuc && bPlaySuc)
		{
			unsigned long dataChnn[64] = {0};

			int index = 0;
			//
			ADAPT_TO_ACTUAL_OUTPUT_ULL_CHNN(outCH);
			//
			for (int i = 0; i < MAX_CHNN_NUM; i++)
			{
				if(outCH & (static_cast<ULONGLONG>(0x1) << i))
				{
					if(i < m_localVideoInputNum)
					{
						dataChnn[index] = DISPLAY_PACK(DISPLAY_LAYER_DIGIT, DISPLAY_SIGNAL_DIGIT, index, i+m_netVideoInputNum);
					}
					else
					{
						dataChnn[index] = DISPLAY_PACK(DISPLAY_LAYER_DIGIT, DISPLAY_SIGNAL_DIGIT, index, i-m_localVideoInputNum);
					}
					
					index++;
				}
			}

			unsigned long splitMode[8] = {0};
			splitMode[DISPLAY_LAYER_DIGIT] = playbackInfo.splitMode;
			m_pDisplayMan->ChangeDisplay(splitMode, 8, dataChnn, index, DISPLAY_FREEZE, DISPLAY_RECORD, DISPLAY_NOTIFY);

#ifdef __DVR_ULTIMATE__
			if(index <= m_pProduct->PBChannelNumForLimitFirstStream())
			{
				m_pNetDeviceMan->SetFirstStreamLimitBitRate(INVALID_INDEX(unsigned long));
			}
#endif
		}
		
		if(bInitialSuc && bPlaySuc)
		{
#if !defined(__ASYN_PLAY__)
			m_pPlaybackMan->Run();
#else
			m_pPlaybackManAsyn->Run();
#endif
#if defined(__NETSERVER_TVT__) && defined (__SUPPORT_3G__)
			CNetServer::Instance()->SetPBStatus(true);
#endif
		}

		if (bInitialSuc && bPlaySuc)
		{
			MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_LOCAL_CLIENT_PLAYBACK_PLAY_SUC,(unsigned char *)&outCH, sizeof(ULONGLONG));
#if defined(__SUPPORT_SNAPD1__)
			char tip[256]="GB2312:录像本地回放";
			for(int i=0;i<m_videoInputNum;i++)
			{
				if(outCH & (0x01<<i))
				{
					CShanghaiConnectMan::Instance()->GetLogCmd(DVRLOCALREPLAY,i+1,tip,strlen(tip),NULL,0,GetCurrTime());
				}
			}
#endif
			WriteLog(msgData.clientID, LOG_TYPE_PLAYBACK_PLAY);
			m_bOperForLocalPlayBackup = true;

			m_pLocalDeviceMan->SetLEDStatus(LED_STATUS_PLAY, true);

			m_pMsgMan->PBShowCamNameCH(0);
#if defined(__ASYN_PLAY__)
			PLAYBACK_SPEED_INFO speedinfo;
			speedinfo.speed =PLAY_SPEED_1X;
			speedinfo.chnn  = 0xFF;//不管同步和异步，都是在全部通道停止的情况下，才能启动回放，因此启动时全通道都是1倍速度播放
			m_pMsgMan->PBShowSpeed(speedinfo);
			m_pMsgMan->RefreshLiveTrue();
#endif
			m_pLocalDeviceMan->SetMainDeviceColor();
		}
		else
		{
			if (bInitialSuc)
			{
#if !defined(__ASYN_PLAY__)
				m_pPlaybackMan->Quit();
#else
				m_pPlaybackManAsyn->Quit();
#endif
#if defined(__NETSERVER_TVT__) && defined (__SUPPORT_3G__)
				CNetServer::Instance()->SetPBStatus(false);
#endif
			}
			
			m_bPlayback = false;
			m_playChNum = 0;
			//回放失败，要设回子码流的帧率
			SetStreamLimit(false);

#ifdef __DVR_ULTIMATE__
			printf("displayCH = %x, displayCHMinor = %x, %s, %d\n", (unsigned long)displayCH, (unsigned long)displayCHMinor, __FILE__, __LINE__);
			CIPCDecDisplayMan::Instance()->SetDisplayCH(displayCH, displayCHMinor);
			CIPCDecDisplayMan::Instance()->Start();
			m_pNetDeviceMan->KeyFrame(displayCH, 0, 0);
			m_pNetDeviceMan->RequestMinorStream(displayCHMinor);
#endif

			MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_LOCAL_CLIENT_PLAYBACK_PLAY_FAIL, NULL, 0);
		}
		
		m_pMsgMan->PutMsgToClient(m_localMsgData);

	}
	else if (CMD_REQUEST_LOCAL_CLIENT_PLAYBACK_STOP == cmdType)
	{
		if (m_bPlayback)
		{
#if !defined(__ASYN_PLAY__)
			m_pPlaybackMan->Stop();
			m_pPlaybackMan->Quit();
#else
			m_pPlaybackManAsyn->Stop();
			m_pPlaybackManAsyn->Quit();
#endif
#if defined(__NETSERVER_TVT__) && defined (__SUPPORT_3G__)
			CNetServer::Instance()->SetPBStatus(false);
#endif
#if defined(__ASYN_PLAY__)
			PLAYBACK_SPEED_INFO speedinfo;
			speedinfo.speed =PLAY_SPEED_NONE;
			speedinfo.chnn  = 0xFF;
			m_pMsgMan->PBShowSpeed(speedinfo);
			m_pMsgMan->RefreshLiveTrue();
#endif
		}
		
		MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_LOCAL_CLIENT_PLAYBACK_STOP_SUC, NULL, 0);
		m_pMsgMan->PutMsgToClient(m_localMsgData);

		//WriteLog(msgData.clientID, LOG_TYPE_PLAYBACK_STOP);

		if (m_bPlayback)
		{
			WriteLog(msgData.clientID, LOG_TYPE_PLAYBACK_STOP);

			m_bOperForLocalPlayBackup = false;

			m_pLocalDeviceMan->SetLEDStatus(LED_STATUS_PLAY, false);

			m_pLocalDeviceMan->RecoverMainDeviceColor();

			m_bPlayback = false;
			m_playChNum = 0;
			//停止回放时，要取消码流限制
			SetStreamLimit(false);
		}
	}
	else if (CMD_REQUEST_LOCAL_CLIENT_PLAYBACK_PAUSE == cmdType)
	{
#if !defined(__ASYN_PLAY__)
		m_pPlaybackMan->Pause();
#else
		CURCHNNINFO chnnInfo = *reinterpret_cast<CURCHNNINFO *>(msgData.pData + sizeof(PACKCMD));
		
		PLAYBACK_SPEED_INFO speedinfo;
		if(chnnInfo.speed!=PLAY_SPEED_1X)
		{
			speedinfo.speed =PLAY_SPEED_PAUSE;
			m_pPlaybackManAsyn->Pause(chnnInfo.clickchnn,true);
		}
		else
		{
			speedinfo.speed=PLAY_SPEED_1X;
			m_pPlaybackManAsyn->Pause(chnnInfo.clickchnn,false);
		}

		if(chnnInfo.bsync)
		{
			speedinfo.chnn  = 0xff;
			
		}
		else
		{
			speedinfo.chnn  = chnnInfo.clickchnn;
		}
		m_pMsgMan->PBShowSpeed(speedinfo);
		m_pMsgMan->RefreshLiveTrue();
#endif
		MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_LOCAL_CLIENT_PLAYBACK_PAUSE_SUC, NULL, 0);
		m_pMsgMan->PutMsgToClient(m_localMsgData);

		WriteLog(msgData.clientID, LOG_TYPE_PLAYBACK_PAUSE);
	}
	else if (CMD_REQUEST_LOCAL_CLIENT_PLAYBACK_RESUM == cmdType)
	{
#if !defined(__ASYN_PLAY__)
		m_pPlaybackMan->Resume();
#else
		CURCHNNINFO chnnInfo = *reinterpret_cast<CURCHNNINFO *>(msgData.pData + sizeof(PACKCMD));
		if(chnnInfo.bsync)
		{
			m_pPlaybackManAsyn->Resume(PLAYBACK_ALL_CHNN);
		}
		else
		{
			m_pPlaybackManAsyn->Resume(chnnInfo.clickchnn);
		}
		PLAYBACK_SPEED_INFO speedinfo;
		speedinfo.speed=PLAY_SPEED_1X;
		if(chnnInfo.bsync)
		{
			speedinfo.chnn  = 0xff;

		}
		else
		{
			speedinfo.chnn  = chnnInfo.clickchnn;
		}
		m_pMsgMan->PBShowSpeed(speedinfo);
		m_pMsgMan->RefreshLiveTrue();
#endif
		MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_LOCAL_CLIENT_PLAYBACK_RESUM_SUC, NULL, 0);
		m_pMsgMan->PutMsgToClient(m_localMsgData);

		WriteLog(msgData.clientID, LOG_TYPE_PLAYBACK_RESUME);
	}
	else if (CMD_REQUEST_LOCAL_CLIENT_PLAYBACK_CHG_CHNN == cmdType)
	{
		printf("0-CMD_REQUEST_LOCAL_CLIENT_PLAYBACK_CHG_CHNN-splitMode\n");
		ULONGLONG outCH = 0;
		LOCAL_CLINET_PB_CHG_CH_INFO *pInfo = reinterpret_cast<LOCAL_CLINET_PB_CHG_CH_INFO *>(msgData.pData + sizeof(PACKCMD));
		//begin:在这里做特殊处理，奇异分割时，大画面用大码流回放，小画面用小码流回放
		if(pInfo->splitMode==VIEW_SPLIT_1A5 || pInfo->splitMode==VIEW_SPLIT_1A7 || pInfo->splitMode==VIEW_SPLIT_1A12)
		{
			printf("1-CMD_REQUEST_LOCAL_CLIENT_PLAYBACK_CHG_CHNN-splitMode:0x%x\n",pInfo->splitMode);
			if(pInfo->splitMode==VIEW_SPLIT_1A5 || pInfo->splitMode==VIEW_SPLIT_1A7)
			{
				m_pPlaybackMan->SetSpecSplit(true, 0);
			}
			else if(pInfo->splitMode==VIEW_SPLIT_1A12)
			{
				//m_pPlaybackMan->SetSpecSplit(true, 5);
				m_pPlaybackMan->SetSpecSplit(false, 0);
			}
		}
		else
		{
			m_pPlaybackMan->SetSpecSplit(false, 0);
		}
		//end
#if !defined(__ASYN_PLAY__)
		bool bRet = m_pPlaybackMan->SetCurChannelMask(pInfo->chnnBits, outCH);
		ADAPT_TO_ACTUAL_OUTPUT_ULL_CHNN(outCH);
#else
		bool bRet = m_pPlaybackManAsyn->SetCurChannelMask(pInfo->chnnBits, outCH);
#endif
		if(!bRet)
		{
			outCH = 0;
		}

		unsigned long dataChnn[64] = {0};
		int index = 0;
		for (int i = 0; i < MAX_CHNN_NUM; i++)
		{
			if(outCH & (static_cast<ULONGLONG>(0x1) << i))
			{
				if(i < m_localVideoInputNum)
				{
					dataChnn[index] = DISPLAY_PACK(DISPLAY_LAYER_DIGIT, DISPLAY_SIGNAL_DIGIT, index, i+m_netVideoInputNum);
				}
				else
				{
					dataChnn[index] = DISPLAY_PACK(DISPLAY_LAYER_DIGIT, DISPLAY_SIGNAL_DIGIT, index, i-m_localVideoInputNum);
				}
				index++;
			}
		}

#ifdef __DVR_ULTIMATE__
		if(index > m_pProduct->PBChannelNumForLimitFirstStream())
		{
			m_pNetDeviceMan->SetFirstStreamLimitBitRate(CProduct::Instance()->FirstStreamLimitBitRate());
		}
		else
		{
			m_pNetDeviceMan->SetFirstStreamLimitBitRate(INVALID_INDEX(unsigned long));
		}
#endif

		unsigned long splitMode[8] = {0};
		splitMode[DISPLAY_LAYER_DIGIT] = pInfo->splitMode;

		m_pDisplayMan->ChangeDisplay(splitMode, 8, dataChnn, index, DISPLAY_FREEZE, DISPLAY_RECORD, DISPLAY_NOTIFY);

		MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_LOCAL_CLIENT_PLAYBACK_CHG_CHNN_SUCC, (unsigned char *)&outCH, sizeof(ULONGLONG));
		m_pMsgMan->PutMsgToClient(m_localMsgData);
	}
	else if (CMD_REQUEST_LOCAL_CLIENT_PLAYBACK_CLICK == cmdType)
	{
		unsigned long pos = *reinterpret_cast<unsigned long *>(msgData.pData + sizeof(PACKCMD));
		unsigned short x = (pos & 0xffff);
		unsigned short y = ((pos >> 16) & 0xffff);

		unsigned long chnn = INVALID_INDEX(unsigned long);

		unsigned long hitsChnn[MAX_CHNN_NUM] = {0};
		int hits = m_pDisplayMan->HitChnn(x, y, hitsChnn, MAX_CHNN_NUM);
		for (int i = 0; i < hits; i++)
		{
			if( (DISPLAY_SIGNAL_DIGIT == DISPLAY_GET_SIGNAL(hitsChnn[i])) &&
				(DISPLAY_INVALID != DISPLAY_GET_CHNN(hitsChnn[i])))
			{
				chnn = DISPLAY_GET_CHNN(hitsChnn[i]);
				break;
			}
		}

#ifdef __DVR_ULTIMATE__
		m_pNetDeviceMan->SetFirstStreamLimitBitRate(INVALID_INDEX(unsigned long));
#endif

		ULONGLONG outCH = 0;
		if (INVALID_INDEX(unsigned long) == chnn)
		{
			MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_LOCAL_CLIENT_PLAYBACK_CHG_CHNN_FAIL, (unsigned char *)&outCH, sizeof(ULONGLONG));
		}
		else
		{
			if(chnn < m_netVideoInputNum)
			{
				chnn += m_localVideoInputNum;
			}
			else
			{
				chnn -= m_netVideoInputNum;
			}
			ULONGLONG CH = (ULONGLONG)(0x01) << chnn;
#if !defined(__ASYN_PLAY__)
			if(!m_pPlaybackMan->SetCurChannelMask(CH, outCH))
			{
				outCH = 0;
			}
#else
			if(!m_pPlaybackManAsyn->SetCurChannelMask(CH, outCH))
			{
				outCH = 0;
			}
#endif

			unsigned long dataChnn[64] = {0};
			int index = 0;
			for (int i = 0; i < MAX_CHNN_NUM; i++)
			{
				if(outCH & (static_cast<ULONGLONG>(0x1) << i))
				{
					if(i < m_localVideoInputNum)
					{
						dataChnn[index] = DISPLAY_PACK(DISPLAY_LAYER_DIGIT, DISPLAY_SIGNAL_DIGIT, index, i+m_netVideoInputNum);
					}
					else
					{
						dataChnn[index] = DISPLAY_PACK(DISPLAY_LAYER_DIGIT, DISPLAY_SIGNAL_DIGIT, index, i-m_localVideoInputNum);
					}
					index++;
				}
			}

			m_pDisplayMan->ChangeDisplay(NULL, 0, dataChnn, index, DISPLAY_FREEZE, DISPLAY_RECORD, DISPLAY_NOTIFY);

			MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_LOCAL_CLIENT_PLAYBACK_CHG_CHNN_SUCC, (unsigned char *)&outCH, sizeof(ULONGLONG));
		}
		
		m_pMsgMan->PutMsgToClient(m_localMsgData);
	}
	else if(CMD_REQUEST_LOCAL_CLIENT_PLAYBACK_HIT == cmdType)
	{
		unsigned long pos = *reinterpret_cast<unsigned long *>(msgData.pData + sizeof(PACKCMD));
		unsigned short x = (pos & 0xffff);
		unsigned short y = ((pos >> 16) & 0xffff);

		unsigned long chnn = INVALID_INDEX(unsigned long);

		unsigned long hitsChnn[MAX_CHNN_NUM] = {0};
		int hits = m_pDisplayMan->HitChnn(x, y, hitsChnn, MAX_CHNN_NUM);

		for (int i = 0; i < hits; i++)
		{
			if( (DISPLAY_SIGNAL_DIGIT == DISPLAY_GET_SIGNAL(hitsChnn[i])) &&
				(DISPLAY_INVALID != DISPLAY_GET_CHNN(hitsChnn[i])))
			{
				chnn = DISPLAY_GET_CHNN(hitsChnn[i]);
				break;
			}
		}

		ULONGLONG outCH = 0;
		if (INVALID_INDEX(unsigned long) == chnn)
		{
			MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_LOCAL_CLIENT_PLAYBACK_HIT_FAIL, (unsigned char *)&outCH, sizeof(ULONGLONG));
		}
		else
		{
			outCH = (ULONGLONG)(0x01) << chnn;
			MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_LOCAL_CLIENT_PLAYBACK_HIT_SUC, (unsigned char *)&outCH, sizeof(ULONGLONG));
		}
		m_pMsgMan->PutMsgToClient(m_localMsgData);
	}

	else if(CMD_REQUEST_LOCAL_CLIENT_PLAYBACK_CHNINIT == cmdType)
	{
		unsigned long initchnn = *reinterpret_cast<unsigned long *>(msgData.pData + sizeof(PACKCMD));;
		m_pDisplayMan->SendInitChnn(initchnn);
		m_pDisplayMan->SendClickPos(INVALID_INDEX(unsigned long),INVALID_INDEX(unsigned long));//初始化时候传送特殊值给fb
		CMessageMan::Instance()->RefreshLiveTrue();
		MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_LOCAL_CLIENT_PLAYBACK_INIT_CHNN_SUCC, (unsigned char *)&initchnn, sizeof(unsigned long));
		m_pMsgMan->PutMsgToClient(m_localMsgData);

	}
	else if(CMD_REQUEST_LOCAL_CLIENT_PLAYBACK_UNINIT == cmdType)
	{
		unsigned long uninitChnn = INVALID_INDEX(unsigned long);
		m_pDisplayMan->SendInitChnn(uninitChnn);
		m_pDisplayMan->SendClickPos(INVALID_INDEX(unsigned long),INVALID_INDEX(unsigned long));//反初始化时候传送特殊值给fb
		CMessageMan::Instance()->RefreshLiveTrue();
		MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_LOCAL_CLIENT_PLAYBACK_UNINIT_CHNN_SUCC, (unsigned char *)&uninitChnn, sizeof(unsigned long));
		m_pMsgMan->PutMsgToClient(m_localMsgData);

	}
	else if(CMD_REQUEST_LOCAL_CLIENT_PLAYBACK_CHOSECHN == cmdType)
	{
		unsigned long pos = *reinterpret_cast<unsigned long *>(msgData.pData + sizeof(PACKCMD));
		unsigned short x = (pos & 0xffff);
		unsigned short y = ((pos >> 16) & 0xffff);

		unsigned long chnn = INVALID_INDEX(unsigned long);

		unsigned long hitsChnn[MAX_CHNN_NUM] = {0};
		int hits = m_pDisplayMan->HitChnn(x, y, hitsChnn, MAX_CHNN_NUM);

		for (int i = 0; i < hits; i++)
		{
			if( (DISPLAY_SIGNAL_DIGIT == DISPLAY_GET_SIGNAL(hitsChnn[i])) &&
				(DISPLAY_INVALID != DISPLAY_GET_CHNN(hitsChnn[i])))
			{
				chnn = DISPLAY_GET_CHNN(hitsChnn[i]);
				break;
			}
		}

		if (chnn != INVALID_INDEX(unsigned long))
		{
			//给displayMan发送点击的x,y值和通道号
			m_pDisplayMan->SendClickPos(x,y);
			m_pDisplayMan->	SendInitChnn(chnn);
			CMessageMan::Instance()->RefreshLiveTrue();
			MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_LOCAL_CLIENT_PLAYBACK_CHOSE_CHNN_SUCC, (unsigned char *)&chnn, sizeof(unsigned long));
		}
		else
		{
			MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_LOCAL_CLIENT_PLAYBACK_CHOSE_CHNN_FAIL, 0, sizeof(unsigned long));
		}
		
		m_pMsgMan->PutMsgToClient(m_localMsgData);
	}
	else if (CMD_REQUEST_LOCAL_CLIENT_PLAYBACK_CHG_TIME == cmdType)
	{
#if !defined(__ASYN_PLAY__)
		ULONGLONG outCH = 0;
		unsigned long time = *reinterpret_cast<unsigned long *>(msgData.pData + sizeof(PACKCMD));
		bool bRet = m_pPlaybackMan->SetCurTime(time, outCH);
		ADAPT_TO_ACTUAL_OUTPUT_ULL_CHNN(outCH);
#else
		ULONGLONG outCH = 0;
		PLAYBACK_CHANGETIME_INFO timeInfo = *reinterpret_cast<PLAYBACK_CHANGETIME_INFO *>(msgData.pData + sizeof(PACKCMD));
		if(timeInfo.bpause)
		{
			//刷新到回放
			PLAYBACK_SPEED_INFO speedinfo;
			speedinfo.speed=PLAY_SPEED_1X;//快进点上下段后在点击播放按钮应该刷新为播放状态
			if(timeInfo.bsync)
			{
				speedinfo.chnn=0xff;
			}
			else
			{
				speedinfo.chnn  = timeInfo.chnn;
			}
			
			m_pMsgMan->PBShowSpeed(speedinfo);
			m_pMsgMan->RefreshLiveTrue();
		}
		bool bRet = m_pPlaybackManAsyn->SetCurTime(timeInfo.time, outCH, timeInfo.chnn);
#endif
		if(!bRet)
		{
			outCH = 0;
		}
		MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_LOCAL_CLIENT_PLAYBACK_CHG_TIME_SUCC, (unsigned char *)&outCH, sizeof(ULONGLONG));
		m_pMsgMan->PutMsgToClient(m_localMsgData);
	}
	else if (CMD_REQUEST_LOCAL_CLIENT_PLAYBACK_NEXTFRAME == cmdType)
	{
#if !defined(__ASYN_PLAY__)
		m_pPlaybackMan->PlayNextFrame(true);
#else
		unsigned long chnn = *reinterpret_cast<unsigned long *>(msgData.pData + sizeof(PACKCMD));
		m_pPlaybackManAsyn->PlayNextFrame(true, chnn);
#endif		
		MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_LOCAL_CLIENT_PLAYBACK_NEXTFRAME_SUC, NULL, 0);
		m_pMsgMan->PutMsgToClient(m_localMsgData);
	}
	else if (CMD_REQUEST_LOCAL_CLIENT_PLAYBACK_FF == cmdType)
	{
#if !defined(__ASYN_PLAY__)
		unsigned long speed = *reinterpret_cast<unsigned long *>(msgData.pData + sizeof(PACKCMD));
		m_pPlaybackMan->SetPlaySpeed(true, (PLAY_SPEED)speed);
		printf("m_pPlaybackMan->SetPlaySpeed()\n");
#else
		PLAYBACK_SPEED_INFO speedinfo = *reinterpret_cast<PLAYBACK_SPEED_INFO *>(msgData.pData + sizeof(PACKCMD));
		m_pPlaybackManAsyn->SetPlaySpeed(true, (PLAY_SPEED)(speedinfo.speed), speedinfo.chnn);
		speedinfo.speed |= (0x1 << 16);
		m_pMsgMan->PBShowSpeed(speedinfo);
		m_pMsgMan->RefreshLive();
#endif
		MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_LOCAL_CLIENT_PLAYBACK_FF_SUC, NULL, 0);
		m_pMsgMan->PutMsgToClient(m_localMsgData);

		WriteLog(msgData.clientID, LOG_TYPE_PLAYBACK_FF);
	}
	else if (CMD_REQUEST_LOCAL_CLIENT_PLAYBACK_REW == cmdType)
	{
#if !defined(__ASYN_PLAY__)
		unsigned long speed = *reinterpret_cast<unsigned long *>(msgData.pData + sizeof(PACKCMD));
		m_pPlaybackMan->SetPlaySpeed(false, (PLAY_SPEED)speed);
#else
		PLAYBACK_SPEED_INFO speedinfo = *reinterpret_cast<PLAYBACK_SPEED_INFO *>(msgData.pData + sizeof(PACKCMD));
		m_pPlaybackManAsyn->SetPlaySpeed(false, (PLAY_SPEED)(speedinfo.speed), speedinfo.chnn);
		m_pMsgMan->PBShowSpeed(speedinfo);
		m_pMsgMan->RefreshLive();
#endif
		MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_LOCAL_CLIENT_PLAYBACK_REW_SUC, NULL, 0);
		m_pMsgMan->PutMsgToClient(m_localMsgData);

		WriteLog(msgData.clientID, LOG_TYPE_PLAYBACK_REW);
	}
	else if (CMD_REQUEST_LOCAL_CLIENT_PLAYBACK_CHG_AUDIO == cmdType)
	{
		unsigned long chnn = *reinterpret_cast<unsigned long *>(msgData.pData + sizeof(PACKCMD));
#if !defined(__ASYN_PLAY__)
		m_pPlaybackMan->SetAudioChannel(chnn);
#else
		m_pPlaybackManAsyn->SetAudioChannel(chnn);
#endif
	}
	else if (CMD_REQUEST_LOCAL_CLIENT_PLAYBACK_LIVE == cmdType)
	{		
#if !defined(__ASYN_PLAY__)
		m_pPlaybackMan->Pause();
		ULONGLONG outCH = 0;
		LIVESTRUCT *pStruct = reinterpret_cast<LIVESTRUCT *>(msgData.pData + sizeof(PACKCMD));

		bool bRet = m_pPlaybackMan->SetCurChannelMask(pStruct->chnnBits, outCH);
		if(!bRet)
		{
			//失败
			outCH = 0;
		}
		else
		{
			unsigned long defaultTimeOut = 30;//默认延时时间
			unsigned long timeout = (pStruct->curDisp.time_out == static_cast<unsigned short>(-1)) ? DISPLAY_FREEZE : pStruct->curDisp.time_out;
			timeout = (timeout == 0) ? defaultTimeOut : timeout;
			m_pDisplayMan->ChangeDisplay(pStruct->curDisp.split_mode,DISPLAY_LAYER_MAX, pStruct->curDisp.data, pStruct->curDisp.data_len, timeout, pStruct->curDisp.rec);
		}

		m_pPlaybackMan->Resume();
		MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_LOCAL_CLIENT_PLAYBACK_CLICKIVE_SUC, (unsigned char *)&outCH, sizeof(ULONGLONG));
		m_pMsgMan->PutMsgToClient(m_localMsgData);
#else
		m_pPlaybackManAsyn->Pause(PLAYBACK_ALL_CHNN,false);
		ULONGLONG outCH = 0;
		LIVESTRUCT *pStruct = reinterpret_cast<LIVESTRUCT *>(msgData.pData + sizeof(PACKCMD));

		bool bRet = m_pPlaybackManAsyn->SetCurChannelMask(pStruct->chnnBits, outCH);
		if(!bRet)
		{
			//失败
			outCH = 0;
		}
		else
		{
			unsigned long defaultTimeOut = 30;//默认延时时间
			unsigned long timeout = (pStruct->curDisp.time_out == static_cast<unsigned short>(-1)) ? DISPLAY_FREEZE : pStruct->curDisp.time_out;
			timeout = (timeout == 0) ? defaultTimeOut : timeout;
			m_pDisplayMan->ChangeDisplay(pStruct->curDisp.split_mode,DISPLAY_LAYER_MAX, pStruct->curDisp.data, pStruct->curDisp.data_len, timeout, pStruct->curDisp.rec);
		}
		printf("%s,%d\n",__FILE__,__LINE__);
		m_pPlaybackManAsyn->Resume(PLAYBACK_ALL_CHNN);
		MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_LOCAL_CLIENT_PLAYBACK_CLICKIVE_SUC, (unsigned char *)&outCH, sizeof(ULONGLONG));
		m_pMsgMan->PutMsgToClient(m_localMsgData);
#endif
	}
	else if (CMD_REQUEST_LOCAL_CLIENT_PLAYBACK_CAM_NAME == cmdType)
	{
		ULONGLONG showCH = *reinterpret_cast<ULONGLONG *>(msgData.pData + sizeof(PACKCMD));
		m_pMsgMan->PBShowCamNameCH(showCH);

		MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_LOCAL_CLIENT_PLAYBACK_CAM_NAME, NULL, 0);
		m_pMsgMan->PutMsgToClient(m_localMsgData);
	}
	else if (CMD_REQUEST_DATA_READ == cmdType)
	{
		
#ifdef __NETSERVER_HXHT__
		NET_HX_DATA_SEARCH dataInfo = *reinterpret_cast<NET_HX_DATA_SEARCH *>(msgData.pData + sizeof(PACKCMD));
		m_pHxNetReader->StartRead(msgData.clientID, dataInfo.chnnNum, dataInfo.startTime, dataInfo.endTime, dataInfo.dataOwnerby);
#endif

#ifdef __NETSERVER_TVT__
		ULONGLONG outCH = 0;
		DVR_DISK_OWNER diskOwnerInfo;
		unsigned long  channelNum = 0;
		NET_DATA_SEARCH dataInfo = *reinterpret_cast<NET_DATA_SEARCH *>(msgData.pData + sizeof(PACKCMD));
		if(dataInfo.dataType == NET_DATA_SEARCH_PLAYBACK)
		{
			printf("playback\n");
			
			WriteLog(msgData.clientID,LOG_TYPE_PLAYBACK_PLAY);
		}
		else if(dataInfo.dataType == NET_DATA_SEARCH_BACKUP)
		{
			printf("backup\n");
			
			WriteLog(msgData.clientID,LOG_TYPE_BACKUP_START);
		}
		memset(&diskOwnerInfo, 0, sizeof(DVR_DISK_OWNER));
		diskOwnerInfo.dvrIndex = dataInfo.dataOwnerby;
#ifdef __ENVIRONMENT_LINUX__        
		m_pDiskMan->GetOwnerInfo(dataInfo.dataOwnerby, diskOwnerInfo);
		assert(diskOwnerInfo.dvrIndex == dataInfo.dataOwnerby);
        if (NET_DISK_OWNED_BY_THIS == dataInfo.dataOwnerby)
        {
			channelNum = diskOwnerInfo.dvrPara;
        }
		else
		{
			channelNum = m_pReclogMan->GetDiskChannelNum(diskOwnerInfo.dvrPara);
		}
#else
		channelNum = m_videoInputNum;      
#endif
		m_pNetReader->StartRead(msgData.clientID, dataInfo.streamID, dataInfo.dataType, dataInfo.videoCH,
 			                    dataInfo.audioCH, dataInfo.startTime, dataInfo.endTime, 
								outCH, channelNum, diskOwnerInfo.dvrIndex);
		//远程回放时限制码流
		SetStreamLimit(false);
#endif

	}
	else if (CMD_REQUEST_DATA_PAUSE == cmdType)
	{
		assert(false);
		ULONGLONG outCH = 0;
		NET_DATA_SEARCH dataInfo = *reinterpret_cast<NET_DATA_SEARCH *>(msgData.pData + sizeof(PACKCMD));
		m_pNetReader->Pause(msgData.clientID, dataInfo.streamID, dataInfo.videoCH);		
		WriteLog(msgData.clientID, LOG_TYPE_PLAYBACK_PAUSE);
	}
	else if (CMD_REQUEST_DATA_RESUME == cmdType)
	{
		//printf("%s, %d\n", __FILE__, __LINE__);
//		assert(false);
		NET_DATA_SEARCH dataInfo = *reinterpret_cast<NET_DATA_SEARCH *>(msgData.pData + sizeof(PACKCMD));
		m_pNetReader->Play(msgData.clientID, dataInfo.streamID);
		WriteLog(msgData.clientID, LOG_TYPE_PLAYBACK_RESUME);
	}
	else if (CMD_REQUEST_DATA_FF == cmdType)
	{
		NET_DATA_SEARCH dataInfo = *reinterpret_cast<NET_DATA_SEARCH *>(msgData.pData + sizeof(PACKCMD));
		NET_PLAY_SPEED speed = (NET_PLAY_SPEED)dataInfo.dataType;
		//printf("(%s, %d)FF:%d\n", __FILE__, __LINE__, speed);
		if (speed < NET_PLAY_SPEED_8X)
		{
			
		}
		else
		{
			m_pNetReader->FF(msgData.clientID, dataInfo.streamID, dataInfo.videoCH, dataInfo.dataType);
		}
		WriteLog(msgData.clientID, LOG_TYPE_PLAYBACK_FF);
	}
	else if (CMD_REQUEST_DATA_REW == cmdType)
	{
		NET_DATA_SEARCH dataInfo = *reinterpret_cast<NET_DATA_SEARCH *>(msgData.pData + sizeof(PACKCMD));
		NET_PLAY_SPEED speed = (NET_PLAY_SPEED)dataInfo.dataType;
		//printf("(%s, %d)REW:%d\n", __FILE__, __LINE__, speed);
		if (speed < NET_PLAY_SPEED_8X)
		{
			assert(false);
		}
		else 
		{
			m_pNetReader->REW(msgData.clientID, dataInfo.streamID, dataInfo.videoCH, speed);
		}
		WriteLog(msgData.clientID, LOG_TYPE_PLAYBACK_REW);
	}
	else if (CMD_REQUEST_DATA_STOP == cmdType)
	{
		//printf("(%s, %d)stop\n", __FILE__, __LINE__);
		NET_DATA_SEARCH dataInfo = *reinterpret_cast<NET_DATA_SEARCH *>(msgData.pData + sizeof(PACKCMD));

#ifdef __NETSERVER_HXHT__
		m_pHxNetReader->StopRead(msgData.clientID);
#endif

#ifdef __NETSERVER_TVT__		
		int stoptype = m_pNetReader->Stop(msgData.clientID, dataInfo.streamID);
		if(stoptype == -1)
		{

		}
		else if(stoptype ==  NET_DATA_SEARCH_PLAYBACK)
		{
			WriteLog(msgData.clientID, LOG_TYPE_PLAYBACK_STOP);
		}
		else if(stoptype ==  NET_DATA_SEARCH_BACKUP)
		{
			WriteLog(msgData.clientID, LOG_TYPE_BACKUP_COMPLETE);
		}
		else
		{

		}
#endif
		CMessageMan::Instance()->ClearPBFrame(msgData.clientID, dataInfo.streamID);
#ifdef __NETSERVER_TVT__
		CNetServer::Instance()->ClearPBData(msgData.clientID, dataInfo.streamID);
#endif
#ifdef __NETSERVER_TVT__
		//远程回放时取消码流限制
		SetStreamLimit(false);
#endif
	}
	else if (CMD_REQUEST_DATA_INDEX == cmdType)
	{
		NET_DATA_SEARCH dataInfo = *reinterpret_cast<NET_DATA_SEARCH *>(msgData.pData + sizeof(PACKCMD));
		 m_pNetReader->RelayIndex(msgData.clientID, dataInfo.streamID, dataInfo.dataType);
	}
	else if(CMD_REQUEST_CHANGE_AUDIO == cmdType)
	{
		NET_DATA_SEARCH dataInfo = *reinterpret_cast<NET_DATA_SEARCH *>(msgData.pData + sizeof(PACKCMD));
		m_pNetReader->ChangeAudio(msgData.clientID, dataInfo.streamID, dataInfo.audioCH);
	}
	else if (CMD_REQUEST_LOCAL_CLIENT_BACKUP_STARTREAD == cmdType)
	{
		ULONGLONG secondStreamCH = 0;
		ULONGLONG secondStreamCH_mobile = 0;
		unsigned long secondSendAllNum = 0;

		LOCAL_CLIENT_BACKUP_INFO backupInfo = *reinterpret_cast<LOCAL_CLIENT_BACKUP_INFO *>(msgData.pData + sizeof(PACKCMD));
		BACKUP_INITIAL_INFO backupInitialInfo;
		backupInitialInfo.pFileInfoBackup   = (FILE_INFO_EX_BACKUP*)backupInfo.pFileInfoBackup;
		backupInitialInfo.fileNum      = backupInfo.fileNum;
		backupInitialInfo.startTime    = backupInfo.startTime;
		backupInitialInfo.endTime      = backupInfo.endTime;
		backupInitialInfo.pDiskName    = (char *)backupInfo.pDiskName;
		backupInitialInfo.pMountDir    = (char *)backupInfo.pMountDir;
		backupInitialInfo.deviceType   = (DISK_TYPE)backupInfo.diskType;
		backupInitialInfo.dataType     = (BACKUP_DATA_TYPE)backupInfo.dataType;
		backupInitialInfo.callBack     = (MESSAGE_CALLBACK_EX)ProcCallBackMsg;
		backupInitialInfo.pCallBackObj = (void *)this;
		backupInitialInfo.pReclogMan   = m_pReclogMan;
		backupInitialInfo.channelNum   = m_videoInputNum;
		backupInitialInfo.pSrcFilePath  = m_pDiskPath;
		backupInitialInfo.bHaveVideoPlay = (bool)backupInfo.bHaveVideoPlay;
		backupInitialInfo.pSrcPlayerPath = m_pVideoPlayerPath;
		backupInitialInfo.diskOwnerby = backupInfo.diskOwnerby;
		if (0 == backupInfo.bCombined)
		{
			backupInitialInfo.bCombined = false;
		}
		else
		{
			backupInitialInfo.bCombined = true;
		}
		
		if (0 == m_pBackupMan->StartBackup(backupInitialInfo))
		{
			MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_LOCAL_CLIENT_BACKUP_STARTREAD_SUC, NULL, 0);

			WriteLog(msgData.clientID, LOG_TYPE_BACKUP_START);
			m_bOperForLocalBackuping = true;

			m_bBackup = true;
			if (!m_bOperForLocalPlayBackup)
			{
				//备份时，限制子码流的帧率
				SetStreamLimit(false);
			}
									
			m_pLocalDeviceMan->SetLEDStatus(LED_STATUS_BACKUP, true);
		}
		else
		{
			MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_LOCAL_CLIENT_BACKUP_STARTREAD_FAIL, NULL, 0);
			WriteLog(msgData.clientID, LOG_TYPE_BACKUP_FAIL);
		}

		m_pMsgMan->PutMsgToClient(m_localMsgData);		
	}
	else if (CMD_REQUEST_LOCAL_CLIENT_BACKUP_BACKUPING == cmdType)
	{
		if (0 == m_pBackupMan->Backup())
		{
			MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_LOCAL_CLIENT_BACKUP_BACKUPING_SUC, NULL, 0);
			//WriteLog(msgData.clientID, LOG_TYPE_BACKUP_COMPLETE);
		}
		else
		{
			MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_LOCAL_CLIENT_BACKUP_BACKUPING_FAIL, NULL, 0);
			WriteLog(msgData.clientID, LOG_TYPE_BACKUP_FAIL);
		}
		m_pMsgMan->PutMsgToClient(m_localMsgData);		

	}
	else if (CMD_REQUEST_LOCAL_CLIENT_BACKUP_STOP == cmdType)
	{
		ULONGLONG secondStreamCH = 0;
		ULONGLONG secondStreamCH_mobile = 0;
		unsigned long secondSendAllNum = 0;

		if (m_bOperForLocalBackuping)
		{
			m_pBackupMan->StopBackup();

			MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_LOCAL_CLIENT_BACKUP_STOP_SUC, NULL, 0);
			//WriteLog(msgData.clientID, LOG_TYPE_BACKUP_CANCEL);
			WriteLog(msgData.clientID, LOG_TYPE_BACKUP_COMPLETE);
			m_bOperForLocalBackuping = false;
			m_pLocalDeviceMan->SetLEDStatus(LED_STATUS_BACKUP, false);

			m_bBackup = false;
			if (!m_bOperForLocalPlayBackup)
			{
				//停止备份时，取消码流限制
				SetStreamLimit(false);
			}
		}
		else
		{
			MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_LOCAL_CLIENT_BACKUP_STOP_SUC, NULL, 0);						
		}
		m_pMsgMan->PutMsgToClient(m_localMsgData);
	}
#ifdef __DVR_ULTIMATE__
	else if(CMD_REQUEST_NET_DEVICE_LOG_MAX == cmdType )
	{
		if (m_netVideoInputNum > 0)
		{
			unsigned long netDeviceMax = m_pNetDeviceMan->GetNetDeviceMax();
			MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_NET_DEVICE_LOG_MAX, reinterpret_cast<unsigned char *>(&netDeviceMax), sizeof(unsigned long));
			m_pMsgMan->PutMsgToClient(m_localMsgData);
		}
		
	}
	else
	{
		_judge__ = true;
	}

	if ( false == _judge__)
	{
		return;
	}
	if (CMD_REQUEST_NET_DEVICE_LOG_GET == cmdType )
	{
		if (m_netVideoInputNum > 0)
		{
			if(NULL != m_pRegNetDeviceLog )
			{
				unsigned long netDeviceCount = m_pNetDeviceMan->GetNetDeviceCount();

				unsigned long len = netDeviceCount * sizeof(IP_DEVICE_INFO) + sizeof( unsigned long );
				unsigned char* pData = new unsigned char[len];
				memset(pData, 0, len);

				IP_DEVICE_INFO* pNetLinkInfo = reinterpret_cast<IP_DEVICE_INFO*>( pData + sizeof(unsigned long) );
				unsigned long retCount = m_pNetDeviceMan->GetNetDevice( pNetLinkInfo, netDeviceCount );
				*reinterpret_cast<unsigned long*>(pData) = retCount;

				#if 0
				for(unsigned long i = 0; i < retCount; i++)
				{
					strcpy(pNetLinkInfo[i].manufacturerName, GetIPCManufacturerName(pNetLinkInfo[i].manufacturerId));
				}
				#endif

				for(int i = 0; i < retCount; i++)
				{
					if(!isalnum(pNetLinkInfo[i].manufacturerName[0]))
					{
						strcpy(pNetLinkInfo[i].manufacturerName, "IP Camera");
					}
				}
				MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_NET_DEVICE_LOG_GET_SUCC, pData, sizeof(IP_DEVICE_INFO) * retCount + sizeof(unsigned long) );
				m_pMsgMan->PutMsgToClient(m_localMsgData);
				delete [] pData;
			}
			else
			{
				MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_NET_DEVICE_LOG_GET_FAIL, NULL, 0 );
				m_pMsgMan->PutMsgToClient(m_localMsgData);
			}
		}
	}
	else if(CMD_REQUEST_NET_DEVICE_LOG_SET == cmdType )
	{
		if (m_netVideoInputNum > 0)
		{
			IP_DEVICE_INFO*  pNetLinkInfo = reinterpret_cast<IP_DEVICE_INFO *>(msgData.pData + sizeof(PACKCMD) + sizeof(unsigned long));
			unsigned long count = *reinterpret_cast<unsigned long*>( msgData.pData + sizeof(PACKCMD) );

			if(m_pNetDeviceMan->SetNetDevice(pNetLinkInfo, count))
			{
				ULONGLONG ullCH = 0;
				for(int i = 0; i < count; i++)
				{
					if(pNetLinkInfo[i].channel != INVALID_INDEX(unsigned char))
					{
						ullCH |= (ULONGLONG)0x01 << pNetLinkInfo[i].channel;
					}
				}
				for(int ch = m_localVideoInputNum; ch < m_videoInputNum; ch++)
				{
					if(!((ullCH >> ch) & 0x01))
					{
						m_pStreamRecManEx->SetChannelLive(ch, false);
					}
				}
				MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_NET_DEVICE_LOG_SET_SUCC, NULL, 0);
				m_pNetDeviceMan->NotifyRegDeviceChanged();
			}
			else
			{
				MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_NET_DEVICE_LOG_SET_FAIL, NULL, 0);
			}

			//更新网络通道标记位g_net_channel
			{
				g_net_channel = 0;

				std::list<NET_DEVICE_LINK_INFO> tmpList;
				m_pNetDeviceMan->GetNetDeviceEffect(tmpList);

				std::list<NET_DEVICE_LINK_INFO>::iterator iter;
				for(iter = tmpList.begin(); iter !=  tmpList.end();iter++)
				{
					g_net_channel |= ((ULONGLONG)(0x1) << iter->channel);
				}

				printf("net video, binding mask = 0x%X. \n", g_net_channel);
#if 0
				//如果有数字信号，则需要对网络子码流降帧，否则要恢复网络子码流的帧率
				ULONGLONG secondCH, thirdCH, mobileHigh;
				unsigned long secondSendAllNum;
				if(g_net_channel != 0)
				{
					m_pStreamProc->SetSecondSteamRateLimit(CProduct::Instance()->GetNetLimitMaxRate());
					m_pUserMan->SecondStreamCH(secondCH, thirdCH, secondSendAllNum, mobileHigh);
					if(secondCH | thirdCH)
					{
						m_pStreamProc->ChangeSecondStreamCH(secondCH, thirdCH, secondSendAllNum, mobileHigh);
					}
				}
				else
				{
					m_pStreamProc->SetSecondSteamRateLimit(INVALID_INDEX(unsigned long));
					m_pUserMan->SecondStreamCH(secondCH, thirdCH, secondSendAllNum, mobileHigh);
					if(secondCH | thirdCH)
					{
						m_pStreamProc->ChangeSecondStreamCH(secondCH, thirdCH, secondSendAllNum, mobileHigh);
					}
				}
#endif
			}

			m_pMsgMan->PutMsgToClient(m_localMsgData);
		}
	}
	else if ( CMD_REQUEST_NET_DEVICE_SEARCH == cmdType )
	{
		if(m_netVideoInputNum > 0)
		{
			long netCount = CNdLibMan::Instance()->GetDiscoveredDeviceNum();
			unsigned long ip = 0;
			unsigned long netmask = 0;
#ifndef WIN32
			CNetInterface::Instance()->GetIPAddress(ip);
			CNetInterface::Instance()->GetIPMaskUL(netmask);
#endif
			unsigned long len = netCount * sizeof( DISCOVER_DEVICE_INFO ) + sizeof( unsigned long );

			ND_DEVICE_INFO* pDeviceInfo = new ND_DEVICE_INFO[netCount];
			netCount = CNdLibMan::Instance()->GetDiscoveredDeviceInfo( pDeviceInfo, netCount );

			//对搜索的IP排序
			ND_DEVICE_INFO netDeviceInfo;
			unsigned long ip1;
			unsigned long ip2;

			for (int j=0; j<netCount-1; j++)
			{
				for (int i=0; i<netCount-j-1; i++)
				{
					ip1 = inet_addr(pDeviceInfo[i].szAddr);
					ip1 = ntohl(ip1);
					ip2 = inet_addr(pDeviceInfo[i + 1].szAddr);
					ip2 = ntohl(ip2);

					if (ip1 > ip2)
					{
						netDeviceInfo = pDeviceInfo[i];
						pDeviceInfo[i] = pDeviceInfo[i + 1];
						pDeviceInfo[i + 1] = netDeviceInfo;
					}
				}
			}

			unsigned char* pRealData = new unsigned char[len];
			DISCOVER_DEVICE_INFO* pRealDeviceInfo = reinterpret_cast<DISCOVER_DEVICE_INFO *>( pRealData + sizeof(unsigned long) );

			long realNetCount = 0;
			int prod_id_val = MANU_ONVIF_NORMAL;

			for (int i = 0; i < netCount; i++)
			{
				//这个设备是ONVIF设备，但不是eth0上的设备
				if((pDeviceInfo[i].manufacturerId == MID_ONVIF) && (strcmp(pDeviceInfo[i].szEther, POE_ETHER) == 0))
				{
					continue;
				}

				//这个设备已经添加
				if (m_pNetDeviceMan->IsNetDeviceHaveConnect(pDeviceInfo + i))
				{
					continue;
				}

				if( MID_ONVIF == pDeviceInfo[i].manufacturerId )
				{
					//再次确认如果是TVT设备，则退出
					//prod_id_val = CNdLibMan::Instance()->GetOnvifIpcManufactID( &pDeviceInfo[i] );
					//if( MANU_ONVIF_TVT == prod_id_val )
					//{
					//	continue;
					//}
				}

				strcpy(pRealDeviceInfo[realNetCount].szAddr, pDeviceInfo[i].szAddr);
				pRealDeviceInfo[realNetCount].netmask = pDeviceInfo[i].netmask;
				pRealDeviceInfo[realNetCount].gateway = pDeviceInfo[i].gateway;
				pRealDeviceInfo[realNetCount].netport = pDeviceInfo[i].netport;
				pRealDeviceInfo[realNetCount].nHttpPort = pDeviceInfo[i].nHttpPort;
				pRealDeviceInfo[realNetCount].nCtrlPort = pDeviceInfo[i].nCtrlPort;
				pRealDeviceInfo[realNetCount].manufacturerId = pDeviceInfo[i].manufacturerId;
				strcpy(pRealDeviceInfo[realNetCount].szID, pDeviceInfo[i].szID);
				memcpy(pRealDeviceInfo[realNetCount].productModel, pDeviceInfo[i].productModel, sizeof(pRealDeviceInfo[realNetCount].productModel));
				//strcpy(pRealDeviceInfo[realNetCount].manufacturerName, GetIPCManufacturerName(pDeviceInfo[i].manufacturerId));

				//保存IPC设备厂家的类型
				if( MID_ONVIF == pDeviceInfo[i].manufacturerId )
				{
					pRealDeviceInfo[realNetCount].manufacturerName[35] = prod_id_val;
				}

				if(
					/*!isalpha(pDeviceInfo[i].manufacturerName[0])*/
					std::string(pDeviceInfo[i].manufacturerName).empty()
				)
				{
					char cmname[37] = {'\0'};
					strncpy(cmname,  GetIPCManufacturerName(pDeviceInfo[i].manufacturerId), 32);
					if(!std::string(cmname).empty())
					{
						strcpy(pRealDeviceInfo[realNetCount].manufacturerName, cmname);
					}
					else
					{
						strcpy(pRealDeviceInfo[realNetCount].manufacturerName, "IP Camera");
					}
				}
				else
				{
					strcpy(pRealDeviceInfo[realNetCount].manufacturerName, pDeviceInfo[i].manufacturerName);
				}
				
				realNetCount++;
			}

			*reinterpret_cast<unsigned long *>(pRealData) = realNetCount;

			MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_NET_DEVICE_SEARCH, pRealData, sizeof(DISCOVER_DEVICE_INFO)*realNetCount + sizeof(unsigned long));
			m_pMsgMan->PutMsgToClient(m_localMsgData);

			delete [] pDeviceInfo;
			delete [] pRealData;
		}
	}
	else if(CMD_REQUEST_NET_DEVICE_INFO == cmdType)
	{
		if(m_netVideoInputNum > 0)
		{
			long count = m_pNetDeviceMan->GetActiveCount(false);
			
			if(count > 0)
			{
				ND_DEV_INFO* pDevInfo = new ND_DEV_INFO[count];

				int index = 0;
				for(int i = m_localVideoInputNum; i < m_videoInputNum; i++)
				{
					CNetDevice* pNetDevice = m_pNetDeviceMan->GetNetDeviceByChnn(i, false);
					if(pNetDevice)
					{
						memset( &pDevInfo[index], 0, sizeof(ND_DEV_INFO) );
						pNetDevice->GetDevInfo(0, pDevInfo[index]);
						pDevInfo[index].videoSize = DDVideoSizeToVideoSize(pDevInfo[index].videoSize);
						pDevInfo[index++].chnn = i;
					}
				}

				assert(index <= count);
				MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_NET_DEVICE_INFO, (unsigned char*)pDevInfo, sizeof(ND_DEV_INFO) * count);
			}
			else
			{
				MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_NET_DEVICE_INFO, NULL, 0);
			}

			m_pMsgMan->PutMsgToClient(m_localMsgData);
		}
	}
	else if(CMD_REQUEST_SET_NET_DEVICE_INFO == cmdType)
	{
		if(m_netVideoInputNum > 0)
		{
			ND_DEVICE_NETWORK_INFO *  pNetDeviceNetworkInfo = reinterpret_cast<ND_DEVICE_NETWORK_INFO *>(msgData.pData + sizeof(PACKCMD));
			CNdLibMan::Instance()->SetDiscoveredDeviceInfo(pNetDeviceNetworkInfo);
		}
	}
	else if(CMD_REQUEST_SUPPORT_DEVICE == cmdType)
	{
		vector<NVR_SUPPORT_MANUFACTURER> supportManufacturer;
		long manufacturerNum = CNdLibMan::Instance()->GetSupportManufacturer(supportManufacturer);

		long dataLen = sizeof(long) + sizeof(NVR_SUPPORT_MANUFACTURER) * manufacturerNum;
		unsigned char *pData = new unsigned char[dataLen];

		*((long *)pData) = manufacturerNum;
		NVR_SUPPORT_MANUFACTURER *pSupportManufacturer = (NVR_SUPPORT_MANUFACTURER*)(pData + sizeof(long));

		int num = 0;
		while(!supportManufacturer.empty())
		{
			vector<NVR_SUPPORT_MANUFACTURER>::iterator it1 = supportManufacturer.begin(), it2;
			for(it2 = it1; it2 != supportManufacturer.end(); it2++)
			{
				if((*it2).manufacturerId < (*it1).manufacturerId)
				{
					it1 = it2;
				}
			}
			pSupportManufacturer[num] = *it1;
			strcpy(pSupportManufacturer[num].manufacturerName, GetIPCManufacturerName(pSupportManufacturer[num].manufacturerId));
			num++;
			supportManufacturer.erase(it1);
		}

		MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_NET_SUPPORT_DEVICE, pData, dataLen);
		m_pMsgMan->PutMsgToClient(m_localMsgData);
	}
#endif
	else if (CMD_REQUEST_CLIENT_INFO == cmdType)
	{
		CMyList <USER_INFO> userList;
		m_pUserMan->GetNetUser(userList);

		unsigned long len = userList.GetCount()*sizeof(NET_CLIENT_INFO_EX) + sizeof(unsigned long);
		unsigned char *pData = new unsigned char [len];
		memset(pData, 0, len);
		*reinterpret_cast<unsigned long *>(pData) = userList.GetCount();

		if (userList.GetCount() > 0)
		{
			NET_CLIENT_INFO_EX *pUser = reinterpret_cast<NET_CLIENT_INFO_EX *>(pData+sizeof(unsigned long));
			unsigned char i = 0;
			POS pos = userList.GetHeadPosition();
			while(NULL != pos)
			{
				USER_INFO & node = userList.GetNext(pos);

				//超级用户不显示
				#ifdef __CUSTOM_IL03__
					#define TITLE_STR_U {0x70626f6b, 0x0068796e,0}
					double wintxtf[]= TITLE_STR_U;
					DWORD wintxt[sizeof(wintxtf)/4];
					for(int i=0;i<sizeof(wintxtf)/4;i++)
					{
						wintxt[i]=wintxtf[i];
					}

					if(strcmp(node.name, (char *)wintxt) == 0)
					{
						(*pData)--;
						len--;
						continue;
					}
				#endif

				pUser[i].clientType = node.clientType;
				pUser[i].IP			= node.IP;
				pUser[i].status		= 1;
				pUser[i].port		= 0;
				pUser[i].clientID	= node.ID;

				strcpy(pUser[i].User, node.name);

				i += 1;
			}
		}

		MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_CLIENT_INFO, pData, len);

		m_pMsgMan->PutMsgToClient(m_localMsgData);
		delete [] pData;

		WriteLog(msgData.clientID, LOG_TYPE_VIEW_ONLINE_USER);
	}
	else if (CMD_REQUEST_EVENT_INFO == cmdType)
	{
		NET_EVENT_SEARCH *pSearch = reinterpret_cast<NET_EVENT_SEARCH *>( msgData.pData + sizeof(PACKCMD) );

		EVENT_LOT_LIST list;
		unsigned long num = m_pLogMan->GetEventLog(pSearch->startTime, pSearch->endTime, pSearch->type, pSearch->chnnBits, list);
#if 0
		EVENT_LOG eventLog;
		for(int i = 0; i < 30; i++)
		{
			eventLog.chnn = i%16;
			eventLog.startTime = GetCurrTime32() - 100000;
			eventLog.endTime = GetCurrTime32();
			eventLog.type = 0x01 << (i%3);
			list.AddTail(eventLog);
		}
		num = list.GetCount();
#endif
		if (num > 0)
		{
			unsigned long len = num*sizeof(EVENT_LOG) + sizeof(NET_EVENT_INFO);
			unsigned char *pData = new unsigned char [len];
			reinterpret_cast<NET_EVENT_INFO *>(pData)->eventNum = num;
			reinterpret_cast<NET_EVENT_INFO *>(pData)->end = true;
			
			EVENT_LOG *pLog = reinterpret_cast<EVENT_LOG *>(pData+sizeof(NET_EVENT_INFO));
			unsigned long i = 0;
			POS pos = list.GetTailPosition();
			while(NULL != pos)
			{
				EVENT_LOG & node = list.GetPrev(pos);
				pLog[i++] = node;
			}

			MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_EVENT_INFO, pData, len);

			m_pMsgMan->PutMsgToClient(m_localMsgData);

			delete [] pData;
		}
		else
		{
			unsigned long len = sizeof(NET_EVENT_INFO);
			unsigned char *pData = new unsigned char [len];
			reinterpret_cast<NET_EVENT_INFO *>(pData)->eventNum = 0;
			reinterpret_cast<NET_EVENT_INFO *>(pData)->end = true;

			MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_EVENT_INFO, pData, len);

			m_pMsgMan->PutMsgToClient(m_localMsgData);

			delete [] pData;
		}

		WriteLog(msgData.clientID, LOG_TYPE_VIEW_EVENT);
	}
	else if (CMD_REQUEST_LOG_INFO == cmdType)
	{
		NET_LOG_SEARCH *pSearch = reinterpret_cast<NET_LOG_SEARCH *>( msgData.pData + sizeof(PACKCMD) );
		OPERATOR_LOG_LIST list;	
		unsigned long num = m_pLogMan->GetOperatorLog(pSearch->startTime, pSearch->endTime, pSearch->content, list);
		if (num > 0)
		{
			unsigned long len = num*sizeof(OPERATOR_LOG) + sizeof(NET_LOG_INFO);
			unsigned char *pData = new unsigned char [len];
			reinterpret_cast<NET_LOG_INFO *>(pData)->logNum = num;
			reinterpret_cast<NET_LOG_INFO *>(pData)->end = true;

			OPERATOR_LOG *pLog = reinterpret_cast<OPERATOR_LOG *>(pData+sizeof(NET_LOG_INFO));
			unsigned long i = 0;
			POS pos = list.GetTailPosition();
			while(NULL != pos)
			{
				OPERATOR_LOG & node = list.GetPrev(pos);
				pLog[i++] = node;
			}

			MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_LOG_INFO, pData, len);

			m_pMsgMan->PutMsgToClient(m_localMsgData);

			delete [] pData;
		}
		else
		{
			unsigned long len = sizeof(NET_LOG_INFO);
			unsigned char *pData = new unsigned char [len];
			reinterpret_cast<NET_LOG_INFO *>(pData)->logNum = 0;
			reinterpret_cast<NET_LOG_INFO *>(pData)->end = true;

			MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_LOG_INFO, pData, len);

			m_pMsgMan->PutMsgToClient(m_localMsgData);

			delete [] pData;
		}

		WriteLog(msgData.clientID, LOG_TYPE_VIEW_LOG);
	}
	else if(CMD_REQUEST_EXPORT_LOG == cmdType)
	{
		//该命令只记录日志导出操作，数据已经由CMD_REQUEST_LOG_INFO命令传给了客户端
		//存在问题，当前的日志导出操作在本次导出的日志中看不到
		WriteLog(msgData.clientID, LOG_TYPE_VIEW_EXPORT_LOG);
	}
	else if(CMD_REQUEST_EXPORT_EVENT == cmdType)
	{
		//该命令只记录事件导出操作，数据已经由CMD_REQUEST_EVENT_INFO命令传给了客户端
		WriteLog(msgData.clientID, LOG_TYPE_VIEW_EXPORT_EVENT);
	}
	else if(CMD_REQUEST_STREAM_START == cmdType)
	{
		NET_LIVESTREAM_CTRL_INFO *pStreamCtrl = reinterpret_cast<NET_LIVESTREAM_CTRL_INFO *>( msgData.pData + sizeof(PACKCMD) );
		for(unsigned char channel = 0; channel < m_localVideoInputNum; channel++)
		{

			if(pStreamCtrl->fastVideoCH & ((ULONGLONG)0x01 << channel))
			{
				m_pLocalDeviceMan->KeyFrame(channel, true);
			}

			if(pStreamCtrl->secondVideoCH & ((ULONGLONG)0x01 << channel))
			{
				m_pLocalDeviceMan->KeyFrame(channel, false);
			}
		}

		//unsigned long secondSendAllNum = 0;
		m_pUserMan->ChangeSecondStreamCH(pStreamCtrl->secondVideoCH, msgData.clientID);
		//m_pUserMan->SecondStreamCH(pStreamCtrl->secondVideoCH, pStreamCtrl->threeVideoCH, secondSendAllNum);

		//远程预览时进行码流限制
		SetStreamLimit(true);

		//针对2504HE_M做特殊限制：如果连接了主码流，预录像降到5秒
		if (PRERECORD_MODE_DISK == m_preRecordType)
		{
			unsigned long productID = CLocalDevice::GetProductID();
			if (TD_2504HE_M == productID)
			{
				if (m_bSendFirstStream)
				{
					if (CNetServer::Instance()->GetFirstStreamSendNum() > 0)
					{

					}
					else
					{
						m_pRecMan->RecoverPreDiskRecTime();
						m_bSendFirstStream = false;
					}
				}
				else
				{
					if (CNetServer::Instance()->GetFirstStreamSendNum() > 0)
					{
						m_pRecMan->SetPreRecTime(5);
						m_bSendFirstStream = true;
					}
				}
			}
		}
	
	}
	else if(CMD_REQUEST_STREAM_CHANGE == cmdType)
	{
		if (msgData.clientID != 0)
		{
			NET_LIVESTREAM_CTRL_INFO *pStreamCtrl = reinterpret_cast<NET_LIVESTREAM_CTRL_INFO *>( msgData.pData + sizeof(PACKCMD) );
			for(unsigned char channel = 0; channel < m_localVideoInputNum; channel++)
			{
				if(pStreamCtrl->fastVideoCH & ((ULONGLONG)0x01 << channel))
				{
					m_pLocalDeviceMan->KeyFrame(channel, true);
				}

				if(pStreamCtrl->secondVideoCH & ((ULONGLONG)0x01 << channel))
				{
					m_pLocalDeviceMan->KeyFrame(channel, false);
				}
			}

			//unsigned long secondSendAllNum = 0;

			m_pUserMan->ChangeSecondStreamCH(pStreamCtrl->secondVideoCH, msgData.clientID);
			//m_pUserMan->SecondStreamCH(pStreamCtrl->secondVideoCH, pStreamCtrl->threeVideoCH, secondSendAllNum);//注意主码流代表次码流，次码流代表手机流

			//改变远程预览通道时进行码流限制
			SetStreamLimit(true);
		}
		else
		{
			//修改问题：重新设置了码流，但是没有立即生效，要等到客户端切换画面时才生效。
			SetStreamLimit(true);
		}

		if (PRERECORD_MODE_DISK == m_preRecordType)
		{
			unsigned long productID = CLocalDevice::GetProductID();
			if (TD_2504HE_M == productID)
			{
				if (m_bSendFirstStream)
				{
					if (CNetServer::Instance()->GetFirstStreamSendNum() > 0)
					{

					}
					else
					{
						m_pRecMan->RecoverPreDiskRecTime();
						m_bSendFirstStream = false;
					}
				}
				else
				{
					if (CNetServer::Instance()->GetFirstStreamSendNum() > 0)
					{
						m_pRecMan->SetPreRecTime(5);
						m_bSendFirstStream = true;
					}
				}
			}
		}

	}
	else if(CMD_REQUEST_STREAM_STOP == cmdType)
	{
		NET_LIVESTREAM_CTRL_INFO *pStreamCtrl = reinterpret_cast<NET_LIVESTREAM_CTRL_INFO *>( msgData.pData + sizeof(PACKCMD) );
		
		//unsigned long secondSendAllNum = 0;
		m_pUserMan->ChangeSecondStreamCH(pStreamCtrl->secondVideoCH, msgData.clientID);
		//m_pUserMan->SecondStreamCH(pStreamCtrl->secondVideoCH, pStreamCtrl->threeVideoCH, secondSendAllNum);//注意主码流代表次码流，次码流代表手机流

		//停止远程预览时取消码流限制
		SetStreamLimit(true);

		if (PRERECORD_MODE_DISK == m_preRecordType)
		{
			unsigned long productID = CLocalDevice::GetProductID();
			if (TD_2504HE_M == productID)
			{
				if (m_bSendFirstStream)
				{
					if (CNetServer::Instance()->GetFirstStreamSendNum() > 0)
					{

					}
					else
					{
						m_pRecMan->RecoverPreDiskRecTime();
						m_bSendFirstStream = false;
					}
				}
				else
				{
					if (CNetServer::Instance()->GetFirstStreamSendNum() > 0)
					{
						m_pRecMan->SetPreRecTime(5);
						m_bSendFirstStream = true;
					}
				}
			}
		}

	}
	else if(CMD_REQUEST_KEYFRAME == cmdType)
	{
		NET_LIVESTREAM_CTRL_INFO *pStreamCtrl = reinterpret_cast<NET_LIVESTREAM_CTRL_INFO *>( msgData.pData + sizeof(PACKCMD) );
		for(unsigned char channel = 0; channel < m_localVideoInputNum; channel++)
		{
			if(pStreamCtrl->fastVideoCH & ((ULONGLONG)0x01 << channel))
			{
				m_pLocalDeviceMan->KeyFrame(channel, true);
			}
			if(pStreamCtrl->secondVideoCH & ((ULONGLONG)0x01 << channel))
			{
				m_pLocalDeviceMan->KeyFrame(channel, false);
			}
		}
	}
	else if( CMD_REQUEST_NETWORK_INFO == cmdType )
	{
		unsigned char *pData = new unsigned char [sizeof(NETWORK_STATUS_INFO)];
		memset(pData, 0, sizeof(NETWORK_STATUS_INFO));

#ifdef __ENVIRONMENT_LINUX__
		CNetInterface::Instance()->GetLocalNICStatusInfo(*reinterpret_cast<NETWORK_STATUS_INFO*>(pData));
#else
		NETWORK_STATUS_INFO *pNetStatus = reinterpret_cast<NETWORK_STATUS_INFO *>(pData);
		pNetStatus->bDHCP = true;
		pNetStatus->bDDNS = true;
		pNetStatus->DNS1 = IP_ADDRESS(192, 168, 11, 1);
		pNetStatus->gateway = IP_ADDRESS(192, 168, 11, 1);
		pNetStatus->IP = IP_ADDRESS(192, 168, 11, 61);
		pNetStatus->subMask = IP_ADDRESS(255, 255, 255, 0);
		pNetStatus->serverPort = 2000;
		pNetStatus->httpPort = 80;
#endif

		MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_NETWORK_INFO, pData, sizeof(NETWORK_STATUS_INFO));
		m_pMsgMan->PutMsgToClient(m_localMsgData);
		delete [] pData;

		WriteLog(msgData.clientID, LOG_TYPE_VIEW_NETWORK);
	}
	else if( CMD_REQUEST_DTNETWORK == cmdType )
	{
		NETWORK_STATUS_INFO dtNetInfo;
		memset(&dtNetInfo,0,sizeof(NETWORK_STATUS_INFO));
#ifdef __ENVIRONMENT_LINUX__
		NET_CTRL_FAIL netCtrlResult = CAllocNICIP::Instance()->GetDhcpIP(dtNetInfo);//打开dhcp
#endif
		MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_DTNETWORK, reinterpret_cast<unsigned char*>(&dtNetInfo), sizeof(NETWORK_STATUS_INFO));
		m_pMsgMan->PutMsgToClient(m_localMsgData);

		WriteLog(msgData.clientID, LOG_TYPE_VIEW_NETWORK);
	}
	else if (CMD_REQUEST_FIRST_SUPPORT_INFO == cmdType)
	{
		unsigned long dataLen = sizeof(SUPPORT_ENC_INFO) * 128;
		SUPPORT_ENC_INFO * pSupportEncInfo = new SUPPORT_ENC_INFO [128];
		memset(pSupportEncInfo, 0, dataLen);

		if (0 == CEncodeCheck::Instance()->GetEncodeInfo(pSupportEncInfo, dataLen))
		{
			MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_FIRST_SUPPORT_INFO, reinterpret_cast<unsigned char*>(pSupportEncInfo), dataLen);
		}
		else
		{
			MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_FIRST_SUPPORT_INFO, NULL, 0);
		}

		m_pMsgMan->PutMsgToClient(m_localMsgData);

		delete [] pSupportEncInfo;
		pSupportEncInfo = NULL;
	}
	else if (CMD_REQUEST_FIRST_CHECK_ENC_INFO == cmdType)
	{
		unsigned long channel = * reinterpret_cast<unsigned long *>( msgData.pData + sizeof(PACKCMD));
		CHECK_ENC_INFO * pCheckEncInfoOther = reinterpret_cast<CHECK_ENC_INFO *>( msgData.pData + sizeof(PACKCMD) + sizeof(unsigned long));

		CHECK_ENC_INFO * pCheckEncInfo = new CHECK_ENC_INFO [m_localVideoInputNum];
		memcpy(pCheckEncInfo, pCheckEncInfoOther, sizeof(CHECK_ENC_INFO) * m_localVideoInputNum);

		if (true == CEncodeCheck::Instance()->CheckEncodeParam(channel, pCheckEncInfo))
		{
			MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_FIRST_CHECK_ENC_INFO, reinterpret_cast<unsigned char*>(pCheckEncInfo), sizeof(CHECK_ENC_INFO) * m_localVideoInputNum);
		}
		else
		{
			MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_FIRST_CHECK_ENC_INFO, NULL, 0);
		}

		m_pMsgMan->PutMsgToClient(m_localMsgData);

		delete [] pCheckEncInfo;
		pCheckEncInfo = NULL;
	}
	else if (CMD_REQUEST_NAT_INFO == cmdType)
	{
		NAT_SERVER_INFO * pNatServerInfo = new NAT_SERVER_INFO;
		memset(pNatServerInfo, 0, sizeof(NAT_SERVER_INFO));
		CNetServer::Instance()->GetNatServerInfo(pNatServerInfo);
		char sServer[128] = {0};
		
#if defined(__CUSTOM_US02__)
		snprintf(sServer, sizeof(sServer), "www.%s", "qtview.com");
#elif defined(__CUSTOM_CN__) || defined(__CUSTOM_SHANGHAI__) || defined(__CUSTOM_IN__) || defined(__CUSTOM_CNB__)
		snprintf(pNatServerInfo->szServerAddr, sizeof(pNatServerInfo->szServerAddr), "autonat.com");
		snprintf(sServer, sizeof(sServer), "www.%s", pNatServerInfo->szServerAddr);
#elif defined(__CUSTOM_IL03__)
		snprintf(sServer, sizeof(sServer), "%s", "www.provisionisr-cloud.com");
#else
		snprintf(pNatServerInfo->szServerAddr, sizeof(pNatServerInfo->szServerAddr), "autonat.com");
		snprintf(sServer, sizeof(sServer), "www.%s", pNatServerInfo->szServerAddr);
#endif

		snprintf(pNatServerInfo->szServerAddr, sizeof(pNatServerInfo->szServerAddr), "%s", sServer);

		MsgData(m_localMsgData, msgData.clientID, CMD_REQUEST_NAT_INFO, reinterpret_cast<unsigned char*>(pNatServerInfo), sizeof(NAT_SERVER_INFO));
		m_pMsgMan->PutMsgToClient(m_localMsgData);

		delete [] pNatServerInfo;
		pNatServerInfo = NULL;
	}
	else if( CMD_REQUEST_WIRELESS_INFO == cmdType )
	{
		unsigned char* pData = new unsigned char[sizeof(WIRELESS_STATUS_INFO)];
		memset(pData, 0, sizeof(WIRELESS_STATUS_INFO));
#ifdef __SUPPORT_3G__
		{
			WIRELESS_STATUS_INFO *pWirelessStatus = reinterpret_cast<WIRELESS_STATUS_INFO*>(pData);
#ifdef __ENVIRONMENT_LINUX__
			CNetInterface::Instance()->GetIPAddress(pWirelessStatus->IP, "ppp0");			
			pWirelessStatus->wirelessstatus = CWirelessManage::Instance()->GetWirelessState();
#endif
		}
#endif
		MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_WIRELESS_INFO, pData, sizeof(WIRELESS_STATUS_INFO));
		m_pMsgMan->PutMsgToClient(m_localMsgData);
		delete [] pData;
	}
	else if( CMD_REQUEST_LOGIN_USER_INFO == cmdType )
	{
		USER_INFO userInfo;
		memset(&userInfo, 0, sizeof(USER_INFO));
		if(m_pUserMan->GetUser(userInfo, msgData.clientID))
		{
			NET_CLIENT_INFO_EX netClientInfo;
			memset(&netClientInfo, 0, sizeof(NET_CLIENT_INFO_EX));

			netClientInfo.clientType	= userInfo.clientType;
			netClientInfo.IP			= userInfo.IP;
			netClientInfo.status		= 1;
			netClientInfo.port			= 0;
			netClientInfo.clientID		= msgData.clientID;

			strcpy(netClientInfo.User, userInfo.name);

			MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_LOGIN_USER_INFO, (unsigned char*)(&netClientInfo), sizeof(NET_CLIENT_INFO_EX));
		}
		else
		{
			//如果没有找到本登录用户信息,回复空;
			//正常情况下不可能走本分支
			MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_LOGIN_USER_INFO, NULL, 0);
		}
		m_pMsgMan->PutMsgToClient(m_localMsgData);
	}
	else if( CMD_REQUEST_NETPARAMCHECK_PPPOE == cmdType )
	{
#ifdef __SUPPORT_3G__
		if (!g_curNet3G)
		{
			NET_CTRL_FAIL netCtrlResult = NETERR_NETPARAMCHECK_OK; 
#ifdef __ENVIRONMENT_LINUX__
			netCtrlResult = CAllocNICIP::Instance()->CheckPppoeParam( reinterpret_cast< const unsigned char * >( msgData.pData + sizeof( PACKCMD ) ) );
#endif
			if( NETERR_NETPARAMCHECK_OK == netCtrlResult )
			{
				MsgData( m_localMsgData, msgData.clientID, CMD_REPLY_NETPARAMCHECK_PPPOE_SUCC, reinterpret_cast< unsigned char *>( &netCtrlResult), sizeof( NET_CTRL_FAIL ) );
			}
			else
			{
				MsgData( m_localMsgData, msgData.clientID, CMD_REPLY_NETPARAMCHECK_PPPOE_FAIL, reinterpret_cast< unsigned char *>( &netCtrlResult), sizeof( NET_CTRL_FAIL ) );
			}
		}
		else
		{
			NET_CTRL_FAIL netCtrlResult = NETERR_NETPARAMCHECK_EUNKNOWN; 
			MsgData( m_localMsgData, msgData.clientID, CMD_REPLY_NETPARAMCHECK_PPPOE_FAIL, reinterpret_cast< unsigned char *>( &netCtrlResult), sizeof( NET_CTRL_FAIL ) );
		}
#else
		NET_CTRL_FAIL netCtrlResult = NETERR_NETPARAMCHECK_OK; 
#ifdef __ENVIRONMENT_LINUX__
		netCtrlResult = CAllocNICIP::Instance()->CheckPppoeParam( reinterpret_cast< const unsigned char * >( msgData.pData + sizeof( PACKCMD ) ) );
#endif
		if( NETERR_NETPARAMCHECK_OK == netCtrlResult )
		{
			MsgData( m_localMsgData, msgData.clientID, CMD_REPLY_NETPARAMCHECK_PPPOE_SUCC, reinterpret_cast< unsigned char *>( &netCtrlResult), sizeof( NET_CTRL_FAIL ) );
		}
		else
		{
			MsgData( m_localMsgData, msgData.clientID, CMD_REPLY_NETPARAMCHECK_PPPOE_FAIL, reinterpret_cast< unsigned char *>( &netCtrlResult), sizeof( NET_CTRL_FAIL ) );
		}
#endif
		
		m_pMsgMan->PutMsgToClient( m_localMsgData );
	}
	else if( CMD_REQUEST_NETPARAMCHECK_WIRELESS == cmdType)
	{
#ifdef __SUPPORT_3G__
		NET_CTRL_FAIL netCtrlResult = NETERR_SUCCESS; 
#ifdef __ENVIRONMENT_LINUX__
		//char cmd[100]={0};
		//snprintf(cmd, 100, "ifconfig eth0 down");
		//DVRSystem(cmd);
		CWirelessManage::Instance()->SetWirelessPara(reinterpret_cast< const MDVR_WIRELESS_CFG *>( msgData.pData + sizeof( PACKCMD )));
		netCtrlResult = CWirelessManage::Instance()->CheckWireless() ;
#endif
		if( NETERR_SUCCESS == netCtrlResult )
		{
			MsgData( m_localMsgData, msgData.clientID, CMD_REPLY_NETPARAMCHECK_WIRELESS_SUCC, reinterpret_cast< unsigned char *>( &netCtrlResult), sizeof( NET_CTRL_FAIL ) );
		}
		else
		{
#ifdef __ENVIRONMENT_LINUX__
			//snprintf(cmd, 100, "ifconfig eth0 up");
			//DVRSystem(cmd);
#endif
			MsgData( m_localMsgData, msgData.clientID, CMD_REPLY_NETPARAMCHECK_WIRELESS_FAIL, reinterpret_cast< unsigned char *>( &netCtrlResult), sizeof( NET_CTRL_FAIL ) );
		}
#else
		NET_CTRL_FAIL netCtrlResult = NETERR_NETPARAMCHECK_ENODEVICE; 
		MsgData( m_localMsgData, msgData.clientID, CMD_REPLY_NETPARAMCHECK_WIRELESS_FAIL, reinterpret_cast< unsigned char *>( &netCtrlResult), sizeof( NET_CTRL_FAIL ) );
#endif
		m_pMsgMan->PutMsgToClient( m_localMsgData );
	}
	else if( CMD_REQUEST_NETPARAMCHECK_DDNS == cmdType )
	{
		NET_CTRL_FAIL netCtrlResult = NETERR_NETPARAMCHECK_OK; 
		DDNS_INFO_EX *pDdnsInfoEx = reinterpret_cast<DDNS_INFO_EX *>( msgData.pData + sizeof( PACKCMD ) );
#ifdef __ENVIRONMENT_LINUX__
		netCtrlResult = CDdnsManager::Instance()->CheckDDNSClientInfo( *pDdnsInfoEx );
#endif
#if defined(__CUSTOM_IN23__)
		if( NETERR_NETPARAMCHECK_OK == netCtrlResult )
		{
			MsgData( m_localMsgData, msgData.clientID, CMD_REPLY_NETPARAMCHECK_DDNS_SUCC, reinterpret_cast<unsigned char * >( &netCtrlResult ), sizeof( NET_CTRL_FAIL ) );
		}
		else if(NETERR_NETPARAMCHECK_DOMAIN_CREATE_OK == netCtrlResult)
		{
			MsgData( m_localMsgData, msgData.clientID, CMD_REPLY_IN23_DOMAIN_CREATE_SUCC, reinterpret_cast<unsigned char * >( &netCtrlResult ), sizeof( NET_CTRL_FAIL ) );
		}
		else if(NETERR_NETPARAMCHECK_DOMAIN_REPLACE_OK == netCtrlResult)
		{
			MsgData( m_localMsgData, msgData.clientID, CMD_REPLY_IN23_DOMAIN_REPLACE_SUCC, reinterpret_cast<unsigned char * >( &netCtrlResult ), sizeof( NET_CTRL_FAIL ) );
		}
		else if(NETERR_NETPARAMCHECK_DOMAIN_ALREADY_USE == netCtrlResult)
		{
			MsgData( m_localMsgData, msgData.clientID, CMD_REPLY_IN23_DOMAIN_ALREADY_USE, reinterpret_cast<unsigned char * >( &netCtrlResult ), sizeof( NET_CTRL_FAIL ) );
		}
		else
		{
			MsgData( m_localMsgData, msgData.clientID, CMD_REPLY_NETPARAMCHECK_DDNS_FAIL, reinterpret_cast<unsigned char * >( &netCtrlResult ), sizeof( NET_CTRL_FAIL ) );
		}

#else
	   switch(netCtrlResult)
	   {
			case NETERR_NETPARAMCHECK_OK:
			case NETERR_NETPARAMCHECK_DOMAIN_CREATE_OK:
			case NETERR_NETPARAMCHECK_DOMAIN_REPLACE_OK:
				{
					MsgData( m_localMsgData, msgData.clientID, CMD_REPLY_NETPARAMCHECK_DDNS_SUCC, reinterpret_cast<unsigned char * >( &netCtrlResult ), sizeof( NET_CTRL_FAIL ) );
				}
				break;
			default:
				{
					MsgData( m_localMsgData, msgData.clientID, CMD_REPLY_NETPARAMCHECK_DDNS_FAIL, reinterpret_cast<unsigned char * >( &netCtrlResult ), sizeof( NET_CTRL_FAIL ) );
				}
				break;
	   }
#endif
		
		m_pMsgMan->PutMsgToClient( m_localMsgData );

	}
	else if( CMD_REQUEST_NETPARAMCHECK_DHCP == cmdType )
	{	
#ifdef __SUPPORT_3G__
		//if (!g_curNet3G)
		{
			NET_CTRL_FAIL netCtrlResult = NETERR_NETPARAMCHECK_OK;
#ifdef __ENVIRONMENT_LINUX__
			netCtrlResult = CAllocNICIP::Instance()->CheckDhcpParam( reinterpret_cast< const unsigned char * >( msgData.pData + sizeof( PACKCMD ) ) );
#endif
			if( NETERR_NETPARAMCHECK_OK == netCtrlResult  )
			{
				MsgData( m_localMsgData, msgData.clientID, CMD_REPLY_NETPARAMCHECK_DHCP_SUCC, reinterpret_cast< unsigned char *>( &netCtrlResult), sizeof( NET_CTRL_FAIL ) );

			}
			else
			{
				MsgData( m_localMsgData, msgData.clientID, CMD_REPLY_NETPARAMCHECK_DHCP_FAIL, reinterpret_cast< unsigned char *>( &netCtrlResult), sizeof( NET_CTRL_FAIL ) );
			}
		}
		/*else
		{
			NET_CTRL_FAIL netCtrlResult = NETERR_NETPARAMCHECK_EUNKNOWN;
			MsgData( m_localMsgData, msgData.clientID, CMD_REPLY_NETPARAMCHECK_DHCP_FAIL, reinterpret_cast< unsigned char *>( &netCtrlResult), sizeof( NET_CTRL_FAIL ) );
		}*/
#else
		NET_CTRL_FAIL netCtrlResult = NETERR_NETPARAMCHECK_OK;
#ifdef __ENVIRONMENT_LINUX__
		netCtrlResult = CAllocNICIP::Instance()->CheckDhcpParam( reinterpret_cast< const unsigned char * >( msgData.pData + sizeof( PACKCMD ) ) );
#endif
		if( NETERR_NETPARAMCHECK_OK == netCtrlResult  )
		{
			MsgData( m_localMsgData, msgData.clientID, CMD_REPLY_NETPARAMCHECK_DHCP_SUCC, reinterpret_cast< unsigned char *>( &netCtrlResult), sizeof( NET_CTRL_FAIL ) );

		}
		else
		{
			MsgData( m_localMsgData, msgData.clientID, CMD_REPLY_NETPARAMCHECK_DHCP_FAIL, reinterpret_cast< unsigned char *>( &netCtrlResult), sizeof( NET_CTRL_FAIL ) );
		}
#endif		
		m_pMsgMan->PutMsgToClient( m_localMsgData );
	}
	else if (CMD_REQUEST_NETPARAMCHECK_EMAIL == cmdType)
	{
		const MAIL_SEND_INFO *pNewSendInfo = reinterpret_cast< const MAIL_SEND_INFO * >( msgData.pData + sizeof( PACKCMD)); 
		const MAIL_RECV_INFO *pNewRecvInfo = reinterpret_cast< const MAIL_RECV_INFO * >( msgData.pData + sizeof( PACKCMD) + sizeof(MAIL_SEND_INFO)); 
		int  newRecvNum = (msgData.dataLen - sizeof(PACKCMD) - sizeof(MAIL_SEND_INFO))/sizeof(MAIL_RECV_INFO);
		MAIL_SEND_INFO oldSendInfo;
		MAIL_RECV_INFO *pOldRecvInfo = new MAIL_RECV_INFO [MAX_MAIL_RECV_NUM];
		memset(pOldRecvInfo, 0, sizeof(MAIL_RECV_INFO)*MAX_MAIL_RECV_NUM);
		int oldRecvNum = 0;
		char szTime[48] = {0};
		tm time = DVRTime32ToTm(GetCurrTime32());
		snprintf(szTime, 48, "%d-%d-%d %d:%d:%d", time.tm_year + 1900, time.tm_mon + 1, time.tm_mday, time.tm_hour, time.tm_min, time.tm_sec);
		std::string name;
		m_pMsgMan->GetCamName(0, name);	
#if /*defined (__ENVIRONMENT_LINUX__) && */defined (__DVR_BASIC__)
		m_pSmtpMan->GetSendPara(&oldSendInfo);
		m_pSmtpMan->GetReceiverPara(pOldRecvInfo, &oldRecvNum);
		m_pSmtpMan->SetSendPara(pNewSendInfo);
		m_pSmtpMan->SetReceiverPara(pNewRecvInfo, newRecvNum);
		int ret = m_pSmtpMan->TestSendEmailProcess(ALARM_TYPE_TEST, szTime);
		m_pSmtpMan->SetSendPara(&oldSendInfo);
		m_pSmtpMan->SetReceiverPara(pOldRecvInfo, oldRecvNum);
//#else
//		int ret = 1;
#endif

		if (ret >= 0)
		{
			MsgData( m_localMsgData, msgData.clientID, CMD_REPLY_NETPARAMCHECK_EMAIL_SUCC, NULL, 0);
		}
		else
		{
			MsgData( m_localMsgData, msgData.clientID, CMD_REPLY_NETPARAMCHECK_EMAIL_FAIL, NULL,0);
		}
		m_pMsgMan->PutMsgToClient( m_localMsgData );
	}
	else if (CMD_REQUEST_SEND_EMAIL == cmdType)
	{
#if /*defined (__ENVIRONMENT_LINUX__) && */defined (__DVR_BASIC__)
		const EMAIL_INFO *pEmailInfo = reinterpret_cast< const EMAIL_INFO * >( msgData.pData + sizeof( PACKCMD));
        EMAIL_INFO temp;		
		memcpy(&temp, pEmailInfo, sizeof(EMAIL_INFO));
		m_pSmtpMan->SendEmailProcess(temp);	
#endif
	}
	else if(CMD_REQUEST_NETPARAMCHECK_UPNP == cmdType)
	{
#if defined (__ENVIRONMENT_LINUX__) && defined (__CUSTOM_US02__)
		const unsigned long *plocalIP = reinterpret_cast< const unsigned long * >( msgData.pData + sizeof( PACKCMD));
		int ret = m_pUpnpMan->CheckPortMap(*plocalIP);
		if(0 == ret)
		{
			MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_NETPARAMCHECK_UPNP_SUCC, (unsigned char*)(&ret), sizeof(int));
		}
		else
		{
			MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_NETPARAMCHECK_UPNP_FAIL, (unsigned char*)(&ret), sizeof(int));
		}
#endif
#ifdef WIN32
			MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_NETPARAMCHECK_UPNP_SUCC, 0, sizeof(int));
#endif
		m_pMsgMan->PutMsgToClient( m_localMsgData );
	}
	else if( CMD_REQUEST_COLOR_GET == cmdType )
	{
		//向客户端发送现场颜色信息， 客户端每次只请求一个通道 ,该通道的颜色信息存放在chnnColor中
//#ifdef __ENVIRONMENT_LINUX__
		bool bGetSingle = true;
		NET_CHNN_COLOR* pChnnColor = NULL;
		
		const NET_CHNN_COLOR *pClntRqstChnnClr = reinterpret_cast<const NET_CHNN_COLOR *>(msgData.pData + sizeof(PACKCMD));
		if(m_videoInputNum > pClntRqstChnnClr->chnn)
		{
			bGetSingle = true;
			pChnnColor = new NET_CHNN_COLOR;
			memset(pChnnColor, 0, sizeof(NET_CHNN_COLOR));
			pChnnColor->chnn = pClntRqstChnnClr->chnn;
		}
		else
		{
			bGetSingle = false;
			pChnnColor = new NET_CHNN_COLOR[m_videoInputNum];
			memset(pChnnColor, 0, sizeof(NET_CHNN_COLOR) * m_videoInputNum);
		}
		
		unsigned long dataLen = 0;
		unsigned char *pCfgData = NULL;
		
		NCFG_ITEM_HEAD itemHead;
		memset(&itemHead, 0, sizeof(NCFG_ITEM_HEAD));
		itemHead.itemID = NCFG_ITEM_VIDEO_COLOR;
		
		if (m_pConfig->GetConfig(reinterpret_cast<unsigned char *>(&itemHead), sizeof(NCFG_ITEM_HEAD), &pCfgData, dataLen))
		{	
			const unsigned long *pClr = reinterpret_cast<const unsigned long *>(pCfgData); 
			assert(dataLen == (sizeof(unsigned long) * m_videoInputNum) + sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD));
			if(dataLen == (sizeof(unsigned long) * m_videoInputNum) + sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD))
			{
				assert(NULL != pClr);
				if(bGetSingle)
				{
					pClr += ((sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD))/4 + pChnnColor->chnn);
					memcpy(&(pChnnColor->color), pClr, sizeof(unsigned long));
				}
				else
				{
					pClr += ((sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD))/4);
					for (int i = 0; i < m_videoInputNum; i++)
					{
						memcpy(&(pChnnColor[i].color), pClr, sizeof(unsigned long));
						pChnnColor[i].chnn = i;
						pClr += 1;
					}
				}
			}

			delete [] pCfgData;
		}
		else
		{			
			printf("%s:%s:%d, get data, error\n\n", __FUNCTION__, __FILE__, __LINE__);
		}

		if(bGetSingle)
		{
			MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_CHNN_CTRL_COLOR, reinterpret_cast<unsigned char *>(pChnnColor), sizeof(NET_CHNN_COLOR ));
			delete pChnnColor;
			pChnnColor = NULL;
		}
		else
		{
			MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_CHNN_CTRL_COLOR, reinterpret_cast<unsigned char *>(pChnnColor), sizeof(NET_CHNN_COLOR) * m_videoInputNum);
			delete [] pChnnColor;
			pChnnColor = NULL;
		}
		m_pMsgMan->PutMsgToClient(m_localMsgData);
//#endif
	}
	else if( CMD_REQUEST_COLOR_SET == cmdType )
	{
		const NET_CHNN_COLOR chnnColor = *reinterpret_cast< const NET_CHNN_COLOR * >( msgData.pData + sizeof( PACKCMD ) );
		
		if(m_videoInputNum > chnnColor.chnn)
		{
			if(m_localVideoInputNum > chnnColor.chnn)
			{
				m_pLocalDeviceMan->SetImageColor(chnnColor.chnn, chnnColor.color);
			}
			else
			{
#ifdef __DVR_ULTIMATE__
				m_pNetDeviceMan->SetVideoColor(chnnColor.chnn, ((chnnColor.color >> 24) & 0xFF), (chnnColor.color & 0xFF), ((chnnColor.color >> 8) & 0xFF), ((chnnColor.color >> 16) & 0xFF));
#endif
			}
		}
		else
		{
			for(int i = 0; i < m_localVideoInputNum; i++)
			{
				m_pLocalDeviceMan->SetImageColor( i, chnnColor.color );
			}

#ifdef __DVR_ULTIMATE__
			for(int i = m_localVideoInputNum; i < m_videoInputNum; i++)
			{
				m_pNetDeviceMan->SetVideoColor(i, ((chnnColor.color >> 24) & 0xFF), (chnnColor.color & 0xFF), ((chnnColor.color >> 8) & 0xFF), ((chnnColor.color >> 16) & 0xFF));
			}
#endif
		}
	}
	else if( CMD_REQUEST_COLOR_SAVE == cmdType )
	{
		bool bSetSingle = true;
		const NET_CHNN_COLOR *pChnnColor = reinterpret_cast< const NET_CHNN_COLOR * >( msgData.pData + sizeof( PACKCMD ) );

		if(m_videoInputNum > pChnnColor->chnn)
		{
			bSetSingle = true;
		}
		else
		{
			bSetSingle = false;
		}

		unsigned long dataLen = 0;
		unsigned char *pCfgData = NULL;

		NCFG_ITEM_HEAD itemHead;
		memset( &itemHead, 0, sizeof( NCFG_ITEM_HEAD ) );
		itemHead.itemID = NCFG_ITEM_VIDEO_COLOR;

		//读出所有通道的颜色信息
		if (m_pConfig->GetConfig(reinterpret_cast<unsigned char *>(&itemHead), sizeof( NCFG_ITEM_HEAD ), &pCfgData, dataLen))
		{	
			assert( dataLen == ( ( sizeof( unsigned long ) * m_videoInputNum ) + sizeof( NCFG_BLOCK_HEAD ) + sizeof( NCFG_ITEM_HEAD ) ) );
			
			NCFG_BLOCK_HEAD *pBlockHead = reinterpret_cast<NCFG_BLOCK_HEAD *>( pCfgData );
			assert( pBlockHead->biSize == sizeof( NCFG_BLOCK_HEAD ) );
			assert( pBlockHead->ItemNum == 1 );
			assert( pBlockHead->netcfgver == NCFG_VERSION );

			NCFG_ITEM_HEAD *pItemHead = reinterpret_cast< NCFG_ITEM_HEAD * >( pCfgData + sizeof( NCFG_BLOCK_HEAD ) );
			assert( pItemHead->itemID == NCFG_ITEM_VIDEO_COLOR );
			assert( pItemHead->len	  == ( sizeof( unsigned long ) * m_videoInputNum ) );
			assert( pItemHead->num	  == m_videoInputNum );
			assert( pItemHead->subLen == sizeof( unsigned long ) );
			
			unsigned long *pAllChnnClr = reinterpret_cast<unsigned long *>(pCfgData + sizeof( NCFG_BLOCK_HEAD ) + sizeof(NCFG_ITEM_HEAD) );
			
			if(bSetSingle)
			{
				//刷新相关通道的颜色配置
				pAllChnnClr[pChnnColor->chnn] = pChnnColor->color;
			}
			else
			{
				for (int i = 0; i < m_videoInputNum; i++)
				{
					pAllChnnClr[i] = pChnnColor->color;
				}
			}

			if( !m_pConfig->SetConfig( pCfgData, dataLen ) )
			{
				NET_CTRL_FAIL ret = NETERR_FAIL;
				MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_CFG_FAIL, reinterpret_cast<unsigned char *>(&ret), sizeof(NET_CTRL_FAIL));
			}
			else
			{		
				MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_CFG_SUCC, NULL, 0);
				WriteLog(msgData.clientID,LOG_TYPE_MODIFY_COLOR);
			}

#ifdef __DVR_ULTIMATE__
			m_pNetDeviceMan->SetNetConfig(pCfgData, dataLen);
#endif

			m_pMsgMan->PutMsgToClient(m_localMsgData);

			delete [] pCfgData;
		}
		else
		{
			assert(0);
			printf("%s:%s:%d, get the all channel color error\n", __FILE__, __FUNCTION__, __LINE__);
			NET_CTRL_FAIL ret = NETERR_FAIL;
			MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_CFG_FAIL, reinterpret_cast<unsigned char *>(&ret), sizeof(NET_CTRL_FAIL));
			m_pMsgMan->PutMsgToClient(m_localMsgData);
		}
		
	}
	else if (CMD_REQUEST_MODIFY_MAC == cmdType)
	{
		unsigned char * pMacaddr = reinterpret_cast<unsigned char *>(msgData.pData + sizeof(PACKCMD));
		assert(6 == (msgData.dataLen - sizeof(PACKCMD)));
		char macstr[64] = {0};
		snprintf(macstr, 18, "%02x:%02x:%02x:%02x:%02x:%02x\n", 
			    pMacaddr[0], pMacaddr[1], pMacaddr[2], pMacaddr[3], pMacaddr[4], pMacaddr[5]);
		printf("macstr:%s\n",macstr);
		if (CLocalDevice::WriteMacToFlash(macstr, 17))
		{
			MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_CTRL_SUCC, NULL, 0);
		}
		else
		{
			MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_CTRL_FAIL, NULL, 0);
		}
		m_pMsgMan->PutMsgToClient(m_localMsgData);

	}
	else if (CMD_REQUEST_MODIFY_HARDWARE_VER == cmdType)
	{
		char * pVersion = reinterpret_cast<char *>(msgData.pData + sizeof(PACKCMD));
		assert(7 == (msgData.dataLen - sizeof(PACKCMD)));		
		if (CLocalDevice::WriteHardWareVersion(pVersion, 6))
		{
			MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_CTRL_SUCC, NULL, 0);
		}
		else
		{
			MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_CTRL_FAIL, NULL, 0);
		}
		m_pMsgMan->PutMsgToClient(m_localMsgData);
	}
	else if (CMD_REQUEST_MODIFY_PROD_SUB_ID == cmdType)
	{
		char *pId = reinterpret_cast<char *>(msgData.pData + sizeof(PACKCMD));
		assert(7 == (msgData.dataLen - sizeof(PACKCMD)));
		if (CLocalDevice::WriteProductSubID(pId, 6))
		{
			MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_CTRL_SUCC, NULL, 0);
		}
		else
		{
			MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_CTRL_FAIL, NULL, 0);
		}
		m_pMsgMan->PutMsgToClient(m_localMsgData);
	}
	else if (CMD_REQUEST_MODIFY_BOOT_IMAGE == cmdType)
	{
		char *pFileName = reinterpret_cast<char *>(msgData.pData + sizeof(PACKCMD));
		if (CLocalDevice::SetBootImage(pFileName))
		{
			MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_CTRL_SUCC, NULL, 0);
		}
		else
		{
			MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_CTRL_FAIL, NULL, 0);
		}
		m_pMsgMan->PutMsgToClient(m_localMsgData);
	}
	else if (CMD_REQUEST_MODIFY_DDNS_TYPE == cmdType)
	{
		char *pType = reinterpret_cast<char *>(msgData.pData + sizeof(PACKCMD));
		assert(7 == (msgData.dataLen - sizeof(PACKCMD)));
		if (CLocalDevice::WriteDDNSTypeToFlash(pType, 6))
		{
			MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_CTRL_SUCC, NULL, 0);
		}
		else
		{
			MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_CTRL_FAIL, NULL, 0);
		}
		m_pMsgMan->PutMsgToClient(m_localMsgData);
	}
	else if (CMD_REQUEST_MODIFY_CRYPTO_INFO == cmdType)
	{
		const CRYTOINFO *pInfo = reinterpret_cast<const CRYTOINFO *>(msgData.pData + sizeof(PACKCMD));
		if (CLocalDevice::WriteCrytoInfoNew(pInfo->languageBits, pInfo->productSubID))
		{
			MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_CTRL_SUCC, NULL, 0);
		}
		else
		{
			MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_CTRL_FAIL, NULL, 0);
		}
		m_pMsgMan->PutMsgToClient(m_localMsgData);
	}
	else if(CMD_REQUEST_MODIFY_COMPANY_NAME == cmdType)
	{
		char *pCompanyName = (char *)(msgData.pData + sizeof(PACKCMD));
		if(CLocalDevice::WriteCompanyNameToFlash(pCompanyName, strlen(pCompanyName)))
		{
			MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_CTRL_SUCC, NULL, 0);
		}
		else
		{
			MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_CTRL_FAIL, NULL, 0);
		}
		m_pMsgMan->PutMsgToClient(m_localMsgData);
	}
#ifdef __ENVIRONMENT_LINUX__
	else if(CMD_REQUEST_EXFACTORY_SETUP == cmdType)
	{
		DVRSystem("rm -rf /mnt/mtd/config/* ");
		CProduct::Instance()->writeDefaultResolution();
		g_system_status = DVR_SYSTEM_REBOOT;

		g_b_rw_cfg = false;

		WriteLog(msgData.clientID, LOG_TYPE_LOAD_DEFAULT);
	}
	else if (CMD_REQUEST_SET_KEYBOARD_NAME == cmdType)
	{
		char *pName = reinterpret_cast<char *>(msgData.pData + sizeof(PACKCMD));
		assert(7 == (msgData.dataLen - sizeof(PACKCMD)));
		if (CLocalDevice::WriteKeyBoardNameToFlash(pName, 7))
		{
			MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_CTRL_SUCC, NULL, 0);
		}
		else
		{
			MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_CTRL_FAIL, NULL, 0);
		}
		m_pMsgMan->PutMsgToClient(m_localMsgData);
	}
#endif
#ifdef __MOBILE_DVR__	
	else if (CMD_REQUEST_NET_WIFI_SEARCH == cmdType)
	{
		NET_WIFI_MODEM wifiMode[12];
		memset(wifiMode, 0, sizeof(NET_WIFI_MODEM)*12);
		int count = 12;		
		int ret = SearchSignalForWIFI(&count,wifiMode);
		if(ret>0)
		{
			MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_NET_WIFI_SEARCH_SUCC,reinterpret_cast<unsigned char *>(&wifiMode), sizeof(NET_WIFI_MODEM)*count);
		}
		else
		{
			MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_NET_WIFI_SEARCH_SUCC,reinterpret_cast<unsigned char *>(&wifiMode), sizeof(NET_WIFI_MODEM)*count);
		}
		m_pMsgMan->PutMsgToClient( m_localMsgData );
	}
	else if(CMD_REQUEST_NET_WIFI_CONNECT == cmdType)
	{
		WIFI_CONFIG *pwificfg = reinterpret_cast<WIFI_CONFIG *>( msgData.pData + sizeof( PACKCMD ));
		int ret = TestWIFIConnect(*pwificfg);
		if(ret == 0)
		{
			MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_NET_WIFI_CONNECT_SUCC,NULL,0);
		}
		else
		{
			MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_NER_WIFI_CONNECT_FAIL,NULL,0);
		}
		m_pMsgMan->PutMsgToClient(m_localMsgData);
	}
#endif
	else if(CMD_REQUEST_LAST_STREAM_ERROR == cmdType)
	{
		unsigned long lastError = 0;
#ifdef __NETSERVER_TVT__
		lastError = CNetServer::Instance()->GetLastStreamError(msgData.clientID);
#endif
		MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_LAST_STREAM_ERROR, (unsigned char *)&lastError, sizeof(unsigned long));

		m_pMsgMan->PutMsgToClient(m_localMsgData);
	}
#ifdef __ENVIRONMENT_LINUX__
	else if(CMD_REQUEST_TALK_START == cmdType)
	{
		bool bRet=false;

#ifdef __NETSERVER_HXHT__
		bRet = m_pStreamProc->StartHxhtTalk();//互信互通的对讲
#else
		bRet = m_pStreamProc->StartTalk();
#endif
		if(bRet)
		{
			MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_TALK_START_SUCC, NULL, 0);
		}
		else
		{
			NET_CTRL_FAIL ret = NETERR_FAIL;
			MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_TALK_START_FAIL, reinterpret_cast<unsigned char *>(&ret), sizeof(NET_CTRL_FAIL));
		}
		m_pMsgMan->PutMsgToClient(m_localMsgData);
	}
	else if(CMD_REQUEST_TALK_STOP == cmdType)
	{
#ifdef __NETSERVER_HXHT__
		m_pStreamProc->WouldStopHxhtTalk();
		m_pMsgMan->ClearHxhtTalkWayFrame();
		m_pStreamProc->StopHxhtTalk();
#else
		m_pStreamProc->WouldStopTalk();
		m_pMsgMan->ClearTalkWayFrame();
		m_pStreamProc->StopTalk();
#endif
	}
	else if(CMD_REQUEST_REBOOT == cmdType)
	{
		g_system_status = DVR_SYSTEM_REBOOT;
	}
	else if(CMD_REQUEST_JPEG_START == cmdType)
	{
#if defined (__ENVIRONMENT_LINUX__) && defined (__DVR_BASIC__)
		NET_JPEG_STREAM_INFO *pJpegInfo = reinterpret_cast<NET_JPEG_STREAM_INFO *>( msgData.pData + sizeof( PACKCMD ));
		printf("Start jpeg stream %s %d,pJpegInfo->jpegCH:", __FILE__, __LINE__,pJpegInfo->jpegCH);
		PrintfBit64(pJpegInfo->jpegCH);
		ULONGLONG openCH = 0;
		m_pUserMan->SetJPEGStreamCH(pJpegInfo->jpegCH, openCH, msgData.clientID);
		
		for (int chnn = 0; chnn < m_localVideoInputNum; ++chnn)
		{
			if (openCH & ((ULONGLONG)0x1 << chnn))
			{
				//m_pLocalDeviceMan->SetPhoneSnapChannel(chnn, true);
#if !defined(__SUPPORT_HONGDI__)
				CSnapCapabilityMan::Instance()->StarSnapJpeg(chnn, true);
#endif
			}
		}
#endif
	}
	else if(CMD_REQUEST_JPEG_CHANGE == cmdType)
	{
#if defined (__ENVIRONMENT_LINUX__) && defined (__DVR_BASIC__)
		NET_JPEG_STREAM_INFO *pJpegInfo = reinterpret_cast<NET_JPEG_STREAM_INFO *>( msgData.pData + sizeof( PACKCMD ));
		ULONGLONG oldJpegStreamCH = 0;
		ULONGLONG openCH = 0;
		ULONGLONG closeCH = 0;
		int chnn = 0;
		printf("Change jpeg stream %s %d,%x\n", __FILE__, __LINE__,pJpegInfo->jpegCH);

		m_pUserMan->GetJPEGStreamCH(oldJpegStreamCH, msgData.clientID);
		m_pUserMan->CancelJPEGStreamCH(oldJpegStreamCH, closeCH, msgData.clientID);
		for (chnn = 0; chnn < m_localVideoInputNum; ++chnn)
		{
			if (closeCH & ((ULONGLONG)0x1 << chnn))
			{
				//m_pLocalDeviceMan->SetPhoneSnapChannel(chnn, false);
				CSnapCapabilityMan::Instance()->StopSnapJpeg(chnn, true);
			}
		}

		m_pUserMan->SetJPEGStreamCH(pJpegInfo->jpegCH, openCH, msgData.clientID);
		for (chnn = 0; chnn < m_localVideoInputNum; ++chnn)
		{
			if (openCH & ((ULONGLONG)0x1 << chnn))
			{
				//m_pLocalDeviceMan->SetPhoneSnapChannel(chnn, true);
#if !defined(__SUPPORT_HONGDI__)
				CSnapCapabilityMan::Instance()->StarSnapJpeg(chnn, true);
#endif
			}
		}
#endif
	}
	else if(CMD_REQUEST_JPEG_STOP == cmdType)
	{
#if defined (__ENVIRONMENT_LINUX__) && defined (__DVR_BASIC__)
		printf("Stop jpeg stream %s %d\n", __FILE__, __LINE__);
		NET_JPEG_STREAM_INFO *pJpegInfo = reinterpret_cast<NET_JPEG_STREAM_INFO *>( msgData.pData + sizeof( PACKCMD ));
		ULONGLONG oldJpegStreamCH = 0;
		ULONGLONG closeCH = 0;
		int chnn = 0;

		m_pUserMan->GetJPEGStreamCH(oldJpegStreamCH, msgData.clientID);
		m_pUserMan->CancelJPEGStreamCH(oldJpegStreamCH, closeCH, msgData.clientID);
		for (chnn = 0; chnn < m_localVideoInputNum; ++chnn)
		{
			if (closeCH & ((ULONGLONG)0x1 << chnn))
			{
				//m_pLocalDeviceMan->SetPhoneSnapChannel(chnn, false);
				CSnapCapabilityMan::Instance()->StopSnapJpeg(chnn, true);
			}
		}
#endif
	}
	else if(CMD_REQUEST_SET_FUNC == cmdType)
	{
		char *pFunc = reinterpret_cast<char *>(msgData.pData + sizeof(PACKCMD));
		assert((msgData.dataLen - sizeof(PACKCMD)) >= 6);
		if (CLocalDevice::WriteFuncCustomToFlash(pFunc, 6))
		{
			MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_CTRL_SUCC, NULL, 0);
		}
		else
		{
			MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_CTRL_FAIL, NULL, 0);
		}
		m_pMsgMan->PutMsgToClient(m_localMsgData);
	}
	else if (CMD_REQUEST_UPDATE_START == cmdType)
	{
		printf("%s,%d,##########CMD_REQUEST_UPDATE_START",__FILE__,__LINE__);
		NET_CTRL_FAIL ret = NETERR_FAIL;
		bool bOk = false;

#ifdef __ENVIRONMENT_LINUX__
		do 
		{
			if (0 == m_pUserMan->CheckAuthority(AUTH_ADVANCE_MAN, msgData.clientID))
			{
				ret = NETERR_FAIL_NO_AUTHORITY;
				break;
			}

			if (m_bOperForUpdating)
			{
				ret = NETERR_UPDATE_FAIL_UPDATING;
				break;
			}
			if (m_bOperForFormating)
			{
				ret = NETERR_UPDATE_FAIL_FORMAT_DISK;
				break;
			}

			unsigned char startParIndex = 0;
			unsigned char parNum = 0;
			memset(m_partionNameForUpdate, 0, sizeof(m_partionNameForUpdate));
#ifdef __ENVIRONMENT_LINUX__
			m_pDiskMan->GetOneRWDiskPartionNum(startParIndex, parNum, m_partionNameForUpdate);
#endif
			if (parNum <= 0)
			{
				ret = NETERR_UPDATE_FAIL_NO_DISK;
				break;
			}

			bOk = true;
			m_bOperForUpdating = true;
			m_bOperForUpdateClinetID = msgData.clientID;
			m_partionIndexForUpdate = startParIndex;
		} while(0);	
#endif

		if (bOk)
		{		
			printf("%s,%d,##########ok\n",__FILE__,__LINE__);
			MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_CAN_UPDATE, NULL, 0);
		}
		else
		{
			printf("%s,%d,##########error\n",__FILE__,__LINE__);
			MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_NOT_UPDATE, reinterpret_cast<unsigned char *>(&ret), sizeof(NET_CTRL_FAIL));			
		}
		m_pMsgMan->PutMsgToClient(m_localMsgData);
	}
	else if (CMD_REQUEST_UPDATE_DATA == cmdType)
	{
		//printf("%s,%d,##########CMD_REQUEST_UPDATE_DATA",__FILE__,__LINE__);
		NET_UPDATE_INFO *pUpdateInfo = reinterpret_cast<NET_UPDATE_INFO *>(msgData.pData + sizeof(PACKCMD));
		NET_CTRL_FAIL ret = NETERR_FAIL;

		bool bOk = false;
		do 
		{
			if (!m_bOperForUpdating)
			{				
				bOk = false;
				break;
			}
			if (m_bOperForUpdateClinetID != msgData.clientID)
			{
				bOk = false;
				break;
			}

			if ((pUpdateInfo->index <= 0) || (pUpdateInfo->index > 100))
			{
				bOk = false;
				break;
			}

			if (1 == pUpdateInfo->index)
			{
				char szPath[32] = {0};
				snprintf(szPath, 32, "/mnt/%02d/upfile.tar", m_partionIndexForUpdate);
				FILE *fp = fopen(szPath, "wb");
				if (NULL == fp)
				{
					bOk = false;
					break;
				}

				unsigned char *pdata = reinterpret_cast<unsigned char *>(msgData.pData + sizeof(PACKCMD)+sizeof(NET_UPDATE_INFO));
				if (pUpdateInfo->currPageLen != fwrite(pdata, sizeof(unsigned char), pUpdateInfo->currPageLen, fp))
				{
					bOk = false;
					fclose(fp);
					break;
				}
				
				bOk = true;
				fclose(fp);
				break;
			}
			else if (pUpdateInfo->index <= 100)
			{
				char szPath[32] = {0};
				snprintf(szPath, 32, "/mnt/%02d/upfile.tar", m_partionIndexForUpdate);
				FILE *fp = fopen(szPath, "rb+");
				if (NULL == fp)
				{
					bOk = false;
					break;
				}
				fseek(fp, 0, SEEK_END);
				unsigned char *pdata = reinterpret_cast<unsigned char *>(msgData.pData + sizeof(PACKCMD)+sizeof(NET_UPDATE_INFO));
				if (pUpdateInfo->currPageLen != fwrite(pdata, sizeof(unsigned char), pUpdateInfo->currPageLen, fp))
				{
					bOk = false;
					fclose(fp);
					break;
				}
				fclose(fp);

				if (pUpdateInfo->index <= 99)
				{
					bOk = true;
					break;
				}
				else
				{					
					if (CheckNetUpdate(szPath, m_partionNameForUpdate))
					{
						bOk = true;
					}
					else
					{
						ret = NETERR_UPDATE_FAIL_INVAID_FILE;
						bOk = false;
					} 
					break;
				}				
			}			
		} while(0);

		if (!bOk)
		{
			//printf("%s,%d,error\n",__FILE__,__LINE__);
			MsgData( m_localMsgData, msgData.clientID, CMD_REPLY_UPDATE_FAIL, reinterpret_cast<unsigned char *>(&ret), sizeof(NET_CTRL_FAIL));			
			if (m_bOperForUpdating)
			{
				char szCmd[64] = {0};
				snprintf(szCmd, 64, "rm -rf /mnt/%02d/upfile.tar", m_partionIndexForUpdate);
				DVRSystem(szCmd);
				m_bOperForUpdating = false;
			}

		}
		else
		{ 
			//printf("%s,%d,ok%d\n",__FILE__,__LINE__,pUpdateInfo->index);
			MsgData( m_localMsgData, msgData.clientID, CMD_REPLY_UPDATE_PERCENT, reinterpret_cast<unsigned char *>(&pUpdateInfo->index), sizeof(unsigned long));						
		}
	
		m_pMsgMan->PutMsgToClient( m_localMsgData );
	}
	else if (CMD_REQUEST_UPDATE_UPDATE == cmdType )
	{
		printf("%s,%d,##########CMD_REQUEST_UPDATE_UPDATE",__FILE__,__LINE__);
		if ((m_bOperForUpdating) && (m_bOperForUpdateClinetID == msgData.clientID))
		{
			{
				PRODUCT_TYPE productTypeUpdate = CProduct::Instance()->ProductType();
				if ((productTypeUpdate >= TD_2704HD) && (productTypeUpdate <= TD_2716XD_L))
				{
					char *filePath;
					filePath = "/upgrade/preupgrade.sh";
					if (FindStringInFile(filePath, "#cp -f /mnt/mtd/config/* /mnt/Bkconfig/"))
					{
						printf("is not bkc\n");
						CProduct::Instance()->writeDefaultResolution();
					}
					else
					{
						UpdateBKC();
						printf("is bkc\n");
					}
				}
			}			
			printf("reboot\n");
			g_system_status = DVR_SYSTEM_REBOOT;
			g_upgrade_status = true;
			WriteLog(msgData.clientID, LOG_TYPE_UPGRADE_SUCC);
		}
		else
		{
			printf("no update,%d,%d,%d,\n",m_bOperForUpdating,m_bOperForUpdateClinetID,msgData.clientID);
		}
	}
	else if (CMD_REQUEST_UPDATE_CANCEL == cmdType)
	{		
		if (m_bOperForUpdating)
		{
			printf("%s,%d,##########CMD_REQUEST_UPDATE_CANCEL ",__FILE__,__LINE__);
			if (m_bOperForUpdateClinetID != msgData.clientID)
			{
				printf("no update,%d,%d\n",m_bOperForUpdateClinetID,msgData.clientID);
			} 
			else
			{
				char szCmd[64] = {0};
				snprintf(szCmd, 64, "rm -rf /mnt/mtd/Update.dat");
				DVRSystem(szCmd);
				snprintf(szCmd, 64, "rm -rf /mnt/%02d/upfile.tar", m_partionIndexForUpdate);
				DVRSystem(szCmd);
#if defined(__CHIPGM__)
				if (TD_2704AS_SL != CProduct::Instance()->ProductType() && TD_2708TS_PL != CProduct::Instance()->ProductType())
				{
					snprintf(szCmd, 64, "rm -rf /upgrade");
					DVRSystem(szCmd);
				}
#else
				snprintf(szCmd, 64, "rm -rf /upgrade");
				DVRSystem(szCmd);
#endif

				snprintf(szCmd, 64, "rm -rf /mnt/mtd/UpFile.dat");
				DVRSystem(szCmd);
				
				m_bOperForUpdating = false;
			}		
		}		
	}
	else if (CMD_REQUEST_GET_DEV_COLOR == cmdType)
	{
		unsigned long ret = m_pLocalDeviceMan->GetCurMainDeviceColor();
		MsgData( m_localMsgData, msgData.clientID, CMD_REPLY_DEV_COLOR, reinterpret_cast<unsigned char *>(&ret), sizeof(unsigned long));			
		m_pMsgMan->PutMsgToClient( m_localMsgData );
	}
	else if (CMD_REQUEST_DEV_COLOR_SET == cmdType)
	{
		unsigned long pdata = *reinterpret_cast<unsigned long *>(msgData.pData + sizeof(PACKCMD));
		m_pLocalDeviceMan->SetMainDeviceColor(pdata);

	}
	else if (CMD_REQUEST_DEV_DEFA_COLOR == cmdType)
	{
		unsigned long ret = m_pLocalDeviceMan->GetCurMainDeviceDefaColor();
		MsgData( m_localMsgData, msgData.clientID, CMD_REPLY_DEV_COLOR, reinterpret_cast<unsigned char *>(&ret), sizeof(unsigned long));			
		m_pMsgMan->PutMsgToClient( m_localMsgData );
	}
	else if (CMD_REQUEST_MANUAL_SNAP == cmdType)
	{
		unsigned long sleepTime = 0;
		bool bSucc = false;
#if defined (__ENVIRONMENT_LINUX__) && defined (__DVR_BASIC__)
		if (g_bHaveSnapPic)
		{			
			sleepTime = 0;
			bSucc = true;
#ifdef __SUPPORT_SNAPD1__
			sleepTime = CShanghaiSnapMan::GetInstance()->GetSnapTime();
#else
			sleepTime = CSnapPictureMan::GetInstance()->GetSnapTime();
#endif
			if ((m_pSnapMan->IsCanSavePicture()) && (sleepTime > 0))
			{
				for (int i = 0; i < m_videoInputNum; i++)
				{
#ifdef __SUPPORT_SNAPD1__
					if (!CShanghaiSnapMan::GetInstance()->SetSnapPicture(i, 1))
#else
					if (!CSnapPictureMan::GetInstance()->SetSnapPicture(i, 1))
#endif
					{
						bSucc = false;
						break;
					}
#ifdef __SUPPORT_SNAPD1__
					else
					{
						CShanghaiSnapMan::GetInstance()->SetScreenSnapParam(i);
					}
#endif
				}			
			}
			else
			{
				bSucc = false;
			}
		}		
#endif
		if (bSucc)
		{
			MsgData( m_localMsgData, msgData.clientID, CMD_REPLY_CTRL_SUCC, (unsigned char*)(&sleepTime), sizeof(unsigned long));			
			WriteLog(msgData.clientID, LOG_TYPE_MANUAL_SNAP_SUCC);
		}
		else
		{
			MsgData( m_localMsgData, msgData.clientID, CMD_REPLY_CTRL_FAIL, NULL, 0);			
			WriteLog(msgData.clientID, LOG_TYPE_MANUAL_SNAP_FAIL);
		}
		m_pMsgMan->PutMsgToClient( m_localMsgData );
		
	}
	else if (CMD_REQUEST_DISK_PIC_INFO == cmdType)
	{
		unsigned long picInfo[2];
		picInfo[0] = 0;
		picInfo[1] = 0;
#if defined (__ENVIRONMENT_LINUX__) && defined (__DVR_BASIC__)
		if (g_bHaveSnapPic)
		{
			unsigned long *pDiskIndex = (unsigned long *)(msgData.pData + sizeof(PACKCMD));
			int num = (msgData.dataLen - sizeof(PACKCMD))/sizeof(unsigned long);

			for (int i = 0; i < num; ++i)
			{
				unsigned long allNum  = 0;
				unsigned long usedNum = 0;
				m_pSnapMan->GetDiskPictureNum(pDiskIndex[i], allNum, usedNum);
				picInfo[0] += allNum;
				picInfo[1] += usedNum;
			}
		}		
#endif
		MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_DISK_PIC_INFO, (unsigned char*)(&(picInfo[0])), sizeof(picInfo));
		m_pMsgMan->PutMsgToClient(m_localMsgData);
	}
	else if (CMD_REQUEST_DEL_DISK_PIC == cmdType)
	{		
#if defined (__ENVIRONMENT_LINUX__) && defined (__DVR_BASIC__)
		if (g_bHaveSnapPic)
		{
			unsigned long *pDiskIndex = (unsigned long *)(msgData.pData + sizeof(PACKCMD));
			int num = (msgData.dataLen - sizeof(PACKCMD))/sizeof(unsigned long);
			for (int i = 0; i < num; ++i)
			{		
				m_pSnapMan->DeleteOneDiskPicture(pDiskIndex[i]);			
			}
		}		
#endif
		MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_DATA_NULL, NULL, 0);
		m_pMsgMan->PutMsgToClient(m_localMsgData);
		WriteLog(msgData.clientID, LOG_TYPE_DELETE_PICTURE);
	}
	else if (CMD_REQUEST_DISK_SMART_INFO == cmdType)
	{
#if defined (__ENVIRONMENT_LINUX__)
		char *pDiskName = (char *)(msgData.pData + sizeof(PACKCMD));
		printf("%s,%d,diskName:%s\n",__FILE__,__LINE__,pDiskName);

		NET_DISK_SMART_INFO smartInfo;
		if (m_pDiskMan->GetDiskSmartInfo(pDiskName, smartInfo))
		{
			printf("###################disk:%s,smart info###################\n",pDiskName);
			printf("ID        CURValue            WorstValue      Threshold     RawValue\n");
			for (int i = 0; i < 20; i++)
			{
				if (smartInfo.smartInfo[i].nIndex != 0)
				{
					printf("%x        %d                  %d               %d           %d\n", 
						smartInfo.smartInfo[i].nIndex, smartInfo.smartInfo[i].nCurrent, smartInfo.smartInfo[i].nWorest,
						smartInfo.smartInfo[i].nThreshold,smartInfo.smartInfo[i].nRawValue);
				}
			}		

			MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_DISK_SMART_SUCC, (unsigned char *)(&smartInfo), sizeof(NET_DISK_SMART_INFO));
		}
		else
		{
			MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_DISK_SMART_FAIL, NULL, 0);
		}

#else
		NET_DISK_SMART_INFO smartInfo;
		memset(&smartInfo, 0, sizeof(NET_DISK_SMART_INFO));
		strcpy((char*)(smartInfo.szModelNumber), "3250310AS");
		strcpy((char*)(smartInfo.szSerialNumber), "6RYBCWKL");
		strcpy((char*)(smartInfo.szFirmwareRev), "3.AAF");
		smartInfo.smartInfo[0].nIndex = 1;
		smartInfo.smartInfo[1].nIndex = 2;
		smartInfo.smartInfo[2].nIndex = 3;

		MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_DISK_SMART_SUCC, (unsigned char *)(&smartInfo), sizeof(NET_DISK_SMART_INFO));
		//MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_DISK_SMART_FAIL, NULL, 0);
#endif
		m_pMsgMan->PutMsgToClient(m_localMsgData);
	}
	else if(CMD_REQUEST_SET_NETWORK == cmdType)
	{
#if defined(__X_LINUX__)
#else
		long dataLen = sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD) * 2 + sizeof(NETWORK_ADDR_INFO) + sizeof(HXHT_NET_PARAM_INFO);
		unsigned char *pData = new unsigned char[dataLen];
		NCFG_BLOCK_HEAD *pBlockHead = (NCFG_BLOCK_HEAD *)pData;
		NCFG_ITEM_HEAD *pItemHead = (NCFG_ITEM_HEAD *)(pData + sizeof(NCFG_BLOCK_HEAD));
		pBlockHead->biSize = sizeof(NCFG_BLOCK_HEAD);
		pBlockHead->ItemNum = 2;
		pBlockHead->netcfgver = NCFG_VERSION;
		pItemHead[0].itemID = NCFG_ITEM_NETWORK_ADDR_INFO;
		pItemHead[0].len = sizeof(NETWORK_ADDR_INFO);
		pItemHead[0].num = 1;
		pItemHead[0].subLen = sizeof(NETWORK_ADDR_INFO);
		pItemHead[1].itemID = NCFG_ITEM_HXHT_NET_PARAM;
		pItemHead[1].len = sizeof(HXHT_NET_PARAM_INFO);
		pItemHead[1].num = 1;
		pItemHead[1].subLen = sizeof(HXHT_NET_PARAM_INFO);

		memcpy(pData+sizeof(NCFG_BLOCK_HEAD)+sizeof(NCFG_ITEM_HEAD)*2, msgData.pData+sizeof(PACKCMD), sizeof(NETWORK_ADDR_INFO));

		HXHT_NET_PARAM_INFO *pHxhtParamInfo = (HXHT_NET_PARAM_INFO *)(pData+sizeof(NCFG_BLOCK_HEAD)+sizeof(NCFG_ITEM_HEAD)*2+sizeof(NETWORK_ADDR_INFO));
		memcpy(pHxhtParamInfo, msgData.pData+sizeof(PACKCMD)+sizeof(NETWORK_ADDR_INFO), sizeof(HXHT_NET_PARAM_INFO));
		
		NCFG_ITEM_HEAD itemHead;
		itemHead.itemID = NCFG_ITEM_HXHT_NET_PARAM;
		unsigned char *pConfig;
		unsigned long configLen;

		if(m_pConfig->GetConfig((unsigned char *)&itemHead, sizeof(itemHead), &pConfig, configLen))
		{
			HXHT_NET_PARAM_INFO *pHxhtOldInfo = (HXHT_NET_PARAM_INFO *)(pConfig + sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD));
			pHxhtParamInfo->usVDevDataPort = pHxhtOldInfo->usVDevDataPort;
			delete [] pConfig;
		}

		if (m_pConfig->SetConfig(pData, dataLen))
		{
			//保存成功，则开始生效处理。
			m_pConfigSetMan->TakeEffect(pData, dataLen);
		}
#endif
	}
#endif
//	else if(CMD_REQUEST_YUANZHENG_MANUAL_REC == cmdType)
//	{
//#ifdef __CUSTOM_YUANZHENG__
//		//首先处理结构体的状态
//		YUANZHENG_MANUAL_REC_PARAM yzRecParam = *reinterpret_cast<YUANZHENG_MANUAL_REC_PARAM *>(msgData.pData + sizeof(PACKCMD));
//		
//		//判断对应通道的状态
//		unsigned char   chnn = yzRecParam.channel;
//		if(yzRecParam.recStatus==YZ_REC_START)//开始录像
//		{
//			m_pYZRec->StartRec(chnn,yzRecParam.recTime,yzRecParam.recStatus,msgData.clientID);
//			WriteLog(msgData.clientID, LOG_TYPE_MANUAL_START);
//		}
//		else if(yzRecParam.recStatus==YZ_REC_STOP)
//		{
//			m_pYZRec->StopRec(chnn,yzRecParam.recStatus);
//			WriteLog(msgData.clientID, LOG_TYPE_MANUAL_STOP);
//		}
//		
//#endif
//	}
//	else if (CMD_REQUEST_SET_YUANZHENG_PARAM == cmdType)
//	{
//#ifdef __CUSTOM_YUANZHENG__
//		YUANZHENG_PARAM param = *reinterpret_cast<YUANZHENG_PARAM *>(msgData.pData + sizeof(PACKCMD));
//		CYuanZhengMan::Instance()->SetYuanZhengParam(param);
//#endif
//	}
	else if(CMD_REQUEST_EFFECT_NETWORK == cmdType)
	{
		unsigned char *pData = NULL;
		unsigned long dataLen = 0;
		MESSAGE_DATA 	temp_localMsgData;
		temp_localMsgData.clientID = LOCAL_CLIENT_ID;
		temp_localMsgData.deviceID = LOCAL_DEVICE_ID;
		temp_localMsgData.pData    = NULL;
		temp_localMsgData.dataLen  = 0;

		CMyList <NCFG_ITEM_ID> temp_itemList;

		temp_itemList.AddTail(NCFG_ITEM_NETWORK_ADDR_INFO);
		if(!g_curNet3G)
		{
			temp_itemList.AddTail(NCFG_ITEM_DNS_1);
			temp_itemList.AddTail(NCFG_ITEM_DNS_2);
		}

		if (m_pConfig->GetConfigEx(temp_itemList, &pData, dataLen))
		{
			m_pConfigSetMan->TakeEffect(pData, dataLen);
			temp_itemList.RemoveAll();
			delete [] pData;
			pData = NULL;
		}
		else
		{
			
		}

		m_pMsgMan->PutMsgToClient( temp_localMsgData );
		printf("effect network !!!!!!!!\n");
	}
	else if(CMD_REQUEST_YUANZHENG_MANUAL_REC == cmdType)
	{
#ifdef __CUSTOM_YUANZHENG__
		//首先处理结构体的状态
		YUANZHENG_MANUAL_REC_PARAM yzRecParam = *reinterpret_cast<YUANZHENG_MANUAL_REC_PARAM *>(msgData.pData + sizeof(PACKCMD));
		
		//判断对应通道的状态
		unsigned char   chnn = yzRecParam.channel;
		if(yzRecParam.recStatus==YZ_REC_START)//开始录像
		{
			m_pYZRec->StartRec(chnn,yzRecParam.recTime,yzRecParam.recStatus,msgData.clientID);
			WriteLog(msgData.clientID, LOG_TYPE_MANUAL_START);
		}
		else if(yzRecParam.recStatus==YZ_REC_STOP)
		{
			m_pYZRec->StopRec(chnn,yzRecParam.recStatus);
			WriteLog(msgData.clientID, LOG_TYPE_MANUAL_STOP);
		}
		
#endif
	}
	else if (CMD_REQUEST_SET_YUANZHENG_PARAM == cmdType)
	{
#ifdef __CUSTOM_YUANZHENG__
		YUANZHENG_PARAM param = *reinterpret_cast<YUANZHENG_PARAM *>(msgData.pData + sizeof(PACKCMD));
		CYuanZhengMan::Instance()->SetYuanZhengParam(param);
#endif
	}
	else
	{
		//编译器限制：块嵌套太深，移到这里
		switch (cmdType)
		{
		case CMD_REQUEST_ROUTERIP:
			{
#if defined (__ENVIRONMENT_LINUX__) && defined (__CUSTOM_US02__)
				char pIP[32]={0};
				int ret=m_pUpnpMan->GetWanIP(pIP);

				if( (0==ret) && (0 != strcmp(pIP,"0.0.0.0")) )
				{
					MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_GETROUTERIP_SUCC, (unsigned char*)(&pIP), sizeof(pIP));
				}
				else
				{
					MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_GETROUTERIP_FAIL, (unsigned char*)(&pIP), sizeof(pIP));
				}

#endif

#ifdef WIN32
				char pIP[32]="192.168.3.115";
				MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_GETROUTERIP_SUCC, (unsigned char*)(pIP), sizeof(pIP));
#endif
				m_pMsgMan->PutMsgToClient( m_localMsgData);

				break;
			}
#ifdef __ENVIRONMENT_LINUX__
		case CMD_REQUEST_REMOVEDDNS:
			{
				bool bsucc = true;
#ifndef WIN32
				bsucc = CDdnsManager::Instance()->DDNSAutoRegRemove();
#endif
				if(bsucc)
				{
					MsgData( m_localMsgData, msgData.clientID, CMD_REPLY_REMOVEDDNS_SUCC, NULL,0 );
				}
				else
				{
					MsgData( m_localMsgData, msgData.clientID, CMD_REPLY_REMOVEDDNS_FAIL, NULL,0 );
				}
				m_pMsgMan->PutMsgToClient(m_localMsgData);
				break;
			}
#endif
		case CMD_REQUEST_SET_BURN_INFO:
			{
				unsigned char * pBurnChnnInfo = reinterpret_cast<unsigned char *>(msgData.pData + sizeof(PACKCMD));
				unsigned long retVal = 0;
#if defined(__DVD_BURN_REC__) && defined(__ENVIRONMENT_LINUX__)
				retVal = CDispatcherMan::Instance()->ReSetRecChnn((char *)pBurnChnnInfo, m_videoInputNum);
#endif
				MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_CTRL_SUCC, (unsigned char *)&retVal, sizeof(unsigned long));
				m_pMsgMan->PutMsgToClient(m_localMsgData);

				break;
			}

		case CMD_REQUEST_SET_BURN_REC:
			{
				unsigned long burnRec = *reinterpret_cast<unsigned long *>(msgData.pData + sizeof(PACKCMD));
				bool bRetVal = false;

#if defined(__DVD_BURN_REC__) && defined(__ENVIRONMENT_LINUX__)
				if (burnRec)
				{
					printf("%s:%s:%d, start burn\n", __FUNCTION__, __FILE__, __LINE__);
					CDispatcherMan::Instance()->RecStartAll();
				}
				else
				{
					printf("%s:%s:%d, Stop burn\n", __FUNCTION__, __FILE__, __LINE__);
					CDispatcherMan::Instance()->RecStopAll();
				}
#endif
				MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_CTRL_SUCC, NULL, 0);

				m_pMsgMan->PutMsgToClient(m_localMsgData);

				break;
			}
		case CMD_REQUEST_SET_IMAGE_MIRROR:   //xian
			{//1个字节
#ifdef __DVR_ULTIMATE__
				unsigned long  * pOther = reinterpret_cast<unsigned long * >(msgData.pData + sizeof(PACKCMD));
				unsigned char charOther = (*pOther)&0xFF;
				unsigned int chnn = ((*pOther)&0xFF00)>>8;
				if ((m_videoInputNum>chnn) && (chnn >= m_localVideoInputNum))
				{
					 m_pNetDeviceMan->SetVideoOther(chnn, &charOther,CMD_REQUEST_SET_IMAGE_MIRROR);
				}		    
#endif
				break;
			}
		case CMD_REQUEST_SET_IMAGE_FILP:
			{//1个字节
#ifdef __DVR_ULTIMATE__
				unsigned long  * pOther = reinterpret_cast<unsigned long * >(msgData.pData + sizeof(PACKCMD));
				unsigned char charOther = (*pOther)&0xFF;
				unsigned int chnn = ((*pOther)&0xFF00)>>8;
				if ((m_videoInputNum>chnn) && (chnn >= m_localVideoInputNum))
				{
					m_pNetDeviceMan->SetVideoOther(chnn, &charOther,CMD_REQUEST_SET_IMAGE_FILP); 
				}		    
#endif
				break;
			}
		case CMD_REQUEST_SET_IMAGE_ROTATO:
			{  //4个字节
#ifdef __DVR_ULTIMATE__
				unsigned long  * pOther = reinterpret_cast<unsigned long * >(msgData.pData + sizeof(PACKCMD));
				unsigned long lOther = (*pOther)&0xFF;
				unsigned int chnn = ((*pOther)&0xFF00)>>8;
				if ((m_videoInputNum>chnn) && (chnn >= m_localVideoInputNum))
				{
					m_pNetDeviceMan->SetVideoOther(chnn, (unsigned char *)&lOther,CMD_REQUEST_SET_IMAGE_ROTATO); 
				}		    
#endif
				break;
			}
		case CMD_REQUEST_SET_IMAGE_DISTORTION:
			{
#ifdef __DVR_ULTIMATE__
			    LENS_DISTORTION * pDistortion = reinterpret_cast<LENS_DISTORTION *>(msgData.pData + sizeof(PACKCMD));
				/*unsigned int chnn = ((*pDistortion)&0xFF0000)>>16;*/
				LENS_DISTORTION_CONFIG stDistortion;
				unsigned int chnn = pDistortion->chnn;
				memset(&stDistortion,0,sizeof(LENS_DISTORTION_CONFIG));
				/*stDistortion.ucLensDistortion = (*pDistortion)&0xFF;
				stDistortion.ucLensDistortionValue = ((*pDistortion)&0xFF00)>>8;*/
				if ( ( m_videoInputNum>chnn ) && ( chnn >= m_localVideoInputNum ) )
				{
					/*stDistortion.ucLensDistortion = (*pDistortion)&0xFF;
					stDistortion.ucLensDistortionValue = ((*pDistortion)>>8)&0xFF;*/
					stDistortion.ucLensDistortion = pDistortion->ucLensDistortion;
					stDistortion.ucLensDistortionValue = pDistortion->ucLensDistortionValue;
					m_pNetDeviceMan->SetVideoOther(chnn,(unsigned char *)&stDistortion,CMD_REQUEST_SET_IMAGE_DISTORTION);
				}
#endif
				break;
			}
		case CMD_REQUEST_SET_AUTOFOCUS:
			{
#ifdef  __DVR_ULTIMATE__
				bool returnSet=true;
				AF_CONTROL_T * pAF = reinterpret_cast<AF_CONTROL_T *>(msgData.pData + sizeof(PACKCMD));
				AF_CONTROL af_ctrl;
				memset(&(af_ctrl),0,sizeof(AF_CONTROL));
			    if ( ( m_videoInputNum>pAF->chnn ) && ( pAF->chnn >= m_localVideoInputNum ) )
				{
					af_ctrl.afType = pAF->afType;
					af_ctrl.nValue = pAF->nValue;
					m_pNetDeviceMan->SetVideoOther(pAF->chnn,(unsigned char *)&af_ctrl,CMD_REQUEST_SET_AUTOFOCUS);
				}
				if (true == returnSet)
				{
					MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_SET_AUTOFOCUS_SUCC,reinterpret_cast< unsigned char *>( &returnSet ), sizeof( bool ) );
				}
				else
				{
					MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_SET_AUTOFOCUS_FAIL, reinterpret_cast< unsigned char *>( &returnSet ), sizeof( bool ) );
				}
				m_pMsgMan->PutMsgToClient(m_localMsgData);
#endif
				break;
			}
		case CMD_REQUEST_SET_IMAGE_ROI:
			{
#ifdef  __DVR_ULTIMATE__
				unsigned int chnn  = m_localVideoInputNum;
				m_pNetDeviceMan->SetVideoOther(chnn,NULL,CMD_REQUEST_SET_IMAGE_ROI);
#endif
				break;
			}
		case CMD_REQUEST_SAVE_IMAGE_MIRROR:
			{
#ifdef  __DVR_ULTIMATE__
				bool bSetSingle = true;
				const NET_CHNN_MIRROR *pMirror = reinterpret_cast< const NET_CHNN_MIRROR * >( msgData.pData + sizeof( PACKCMD ) );

				if(m_netVideoInputNum > pMirror->chnn){
					bSetSingle = true;
				}else{
					bSetSingle = false;
				}

				unsigned long dataLen = 0;
				unsigned char *pCfgData = NULL;

				NCFG_ITEM_HEAD itemHead;
				memset( &itemHead, 0, sizeof( NCFG_ITEM_HEAD ) );
				itemHead.itemID = NCFG_ITEM_CAM_MIRROR;
				//读出所有通道的镜像配置
				if (m_pConfig->GetConfig(reinterpret_cast<unsigned char *>(&itemHead), sizeof( NCFG_ITEM_HEAD ), &pCfgData, dataLen))
				{	
					assert( dataLen == ( ( sizeof( unsigned char ) * m_netVideoInputNum ) + sizeof( NCFG_BLOCK_HEAD ) + sizeof( NCFG_ITEM_HEAD ) ) );

					NCFG_BLOCK_HEAD *pBlockHead = reinterpret_cast<NCFG_BLOCK_HEAD *>( pCfgData );
					assert( pBlockHead->biSize == sizeof( NCFG_BLOCK_HEAD ) );
					assert( pBlockHead->ItemNum == 1 );
					assert( pBlockHead->netcfgver == NCFG_VERSION );

					NCFG_ITEM_HEAD *pItemHead = reinterpret_cast< NCFG_ITEM_HEAD * >( pCfgData + sizeof( NCFG_BLOCK_HEAD ) );
					assert( pItemHead->itemID == NCFG_ITEM_CAM_MIRROR );
					assert( pItemHead->len	  == ( sizeof( unsigned char ) * m_netVideoInputNum ) );
					assert( pItemHead->num	  == m_netVideoInputNum );
					assert( pItemHead->subLen == sizeof( unsigned char ) );

					unsigned char *pAllChnn = reinterpret_cast<unsigned char *>(pCfgData + sizeof( NCFG_BLOCK_HEAD ) + sizeof(NCFG_ITEM_HEAD) );

					if(bSetSingle)
					{
						//刷新相关通道的镜像配置
						pAllChnn[pMirror->chnn] = pMirror->mirror;
					}
					else
					{
						/*for (unsigned int i = 0; i < m_netVideoInputNum; i++)
						{
							pAllChnn[i] = pMirror->mirror;
						}*/
					}

					if( !m_pConfig->SetConfig( pCfgData, dataLen ) )
					{
						NET_CTRL_FAIL ret = NETERR_FAIL;
						MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_CFG_FAIL, reinterpret_cast<unsigned char *>(&ret), sizeof(NET_CTRL_FAIL));
					}
					else
					{		
						MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_CFG_SUCC, NULL, 0);
					}

#ifdef __DVR_ULTIMATE__
					m_pNetDeviceMan->SetNetConfig(pCfgData, dataLen);
#endif

					m_pMsgMan->PutMsgToClient(m_localMsgData);

					delete [] pCfgData;
				}
				else
				{
					assert(0);
					printf("%s:%s:%d, get the all channel mirror error\n", __FILE__, __FUNCTION__, __LINE__);
					NET_CTRL_FAIL ret = NETERR_FAIL;
					MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_CFG_FAIL, reinterpret_cast<unsigned char *>(&ret), sizeof(NET_CTRL_FAIL));
					m_pMsgMan->PutMsgToClient(m_localMsgData);
				}
#endif
			}
			break;
		case CMD_REQUEST_SAVE_IMAGE_FILP:
			{
#ifdef  __DVR_ULTIMATE__
				bool bSetSingle = true;
				const NET_CHNN_FLIP *pFlip = reinterpret_cast< const NET_CHNN_FLIP * >( msgData.pData + sizeof( PACKCMD ) );

				if(m_netVideoInputNum > pFlip->chnn){
					bSetSingle = true;
				}else{
					bSetSingle = false;
				}

				unsigned long dataLen = 0;
				unsigned char *pCfgData = NULL;

				NCFG_ITEM_HEAD itemHead;
				memset( &itemHead, 0, sizeof( NCFG_ITEM_HEAD ) );
				itemHead.itemID = NCFG_ITEM_CAM_FILP;
				//读出所有通道的翻转配置
				if (m_pConfig->GetConfig(reinterpret_cast<unsigned char *>(&itemHead), sizeof( NCFG_ITEM_HEAD ), &pCfgData, dataLen))
				{	
					assert( dataLen == ( ( sizeof( unsigned char ) * m_netVideoInputNum ) + sizeof( NCFG_BLOCK_HEAD ) + sizeof( NCFG_ITEM_HEAD ) ) );

					NCFG_BLOCK_HEAD *pBlockHead = reinterpret_cast<NCFG_BLOCK_HEAD *>( pCfgData );
					assert( pBlockHead->biSize == sizeof( NCFG_BLOCK_HEAD ) );
					assert( pBlockHead->ItemNum == 1 );
					assert( pBlockHead->netcfgver == NCFG_VERSION );

					NCFG_ITEM_HEAD *pItemHead = reinterpret_cast< NCFG_ITEM_HEAD * >( pCfgData + sizeof( NCFG_BLOCK_HEAD ) );
					assert( pItemHead->itemID == NCFG_ITEM_CAM_FILP );
					assert( pItemHead->len	  == ( sizeof( unsigned char ) * m_netVideoInputNum ) );
					assert( pItemHead->num	  == m_netVideoInputNum );
					assert( pItemHead->subLen == sizeof( unsigned char ) );

					unsigned char *pAllChnn = reinterpret_cast<unsigned char *>(pCfgData + sizeof( NCFG_BLOCK_HEAD ) + sizeof(NCFG_ITEM_HEAD) );

					if(bSetSingle)
					{
						//刷新相关通道的翻转配置
						pAllChnn[pFlip->chnn] = pFlip->flip;
					}
					else
					{
						/*for (unsigned int i = 0; i < m_netVideoInputNum; i++)
						{
							pAllChnn[i] = pFlip->flip;
						}*/
					}

					if( !m_pConfig->SetConfig( pCfgData, dataLen ) )
					{
						NET_CTRL_FAIL ret = NETERR_FAIL;
						MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_CFG_FAIL, reinterpret_cast<unsigned char *>(&ret), sizeof(NET_CTRL_FAIL));
					}
					else
					{		
						MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_CFG_SUCC, NULL, 0);
					}

#ifdef __DVR_ULTIMATE__
					m_pNetDeviceMan->SetNetConfig(pCfgData, dataLen);
#endif

					m_pMsgMan->PutMsgToClient(m_localMsgData);

					delete [] pCfgData;
				}
				else
				{
					assert(0);
					printf("%s:%s:%d, get the all channel flip error\n", __FILE__, __FUNCTION__, __LINE__);
					NET_CTRL_FAIL ret = NETERR_FAIL;
					MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_CFG_FAIL, reinterpret_cast<unsigned char *>(&ret), sizeof(NET_CTRL_FAIL));
					m_pMsgMan->PutMsgToClient(m_localMsgData);
				}
#endif
			}
			break;
		case CMD_REQUEST_SAVE_IMAGE_ROTATO:
			{
#ifdef  __DVR_ULTIMATE__
				bool bSetSingle = true;
				const NET_CHNN_ROTATO *pRotato = reinterpret_cast< const NET_CHNN_ROTATO * >( msgData.pData + sizeof( PACKCMD ) );

				if(m_netVideoInputNum > pRotato->chnn){
					bSetSingle = true;
				}else{
					bSetSingle = false;
				}

				unsigned long dataLen = 0;
				unsigned char *pCfgData = NULL;

				NCFG_ITEM_HEAD itemHead;
				memset( &itemHead, 0, sizeof( NCFG_ITEM_HEAD ) );
				itemHead.itemID = NCFG_ITEM_CAM_ROTATO;
				//读出所有通道的走廊模式配置
				if (m_pConfig->GetConfig(reinterpret_cast<unsigned char *>(&itemHead), sizeof( NCFG_ITEM_HEAD ), &pCfgData, dataLen))
				{	
					assert( dataLen == ( ( sizeof( unsigned long ) * m_netVideoInputNum ) + sizeof( NCFG_BLOCK_HEAD ) + sizeof( NCFG_ITEM_HEAD ) ) );

					NCFG_BLOCK_HEAD *pBlockHead = reinterpret_cast<NCFG_BLOCK_HEAD *>( pCfgData );
					assert( pBlockHead->biSize == sizeof( NCFG_BLOCK_HEAD ) );
					assert( pBlockHead->ItemNum == 1 );
					assert( pBlockHead->netcfgver == NCFG_VERSION );

					NCFG_ITEM_HEAD *pItemHead = reinterpret_cast< NCFG_ITEM_HEAD * >( pCfgData + sizeof( NCFG_BLOCK_HEAD ) );
					assert( pItemHead->itemID == NCFG_ITEM_CAM_ROTATO );
					assert( pItemHead->len	  == ( sizeof( unsigned long ) * m_netVideoInputNum ) );
					assert( pItemHead->num	  == m_netVideoInputNum );
					assert( pItemHead->subLen == sizeof( unsigned long ) );

					unsigned long *pAllChnn = reinterpret_cast<unsigned long *>(pCfgData + sizeof( NCFG_BLOCK_HEAD ) + sizeof(NCFG_ITEM_HEAD) );

					if(bSetSingle)
					{
						//刷新相关通道的走廊模式配置
						pAllChnn[pRotato->chnn] = pRotato->rotato;
					}
					else
					{
						/*for (unsigned int i = 0; i < m_netVideoInputNum; i++)
						{
							pAllChnn[i] = pRotato->rotato;
						}*/
					}

					if( !m_pConfig->SetConfig( pCfgData, dataLen ) )
					{
						NET_CTRL_FAIL ret = NETERR_FAIL;
						MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_CFG_FAIL, reinterpret_cast<unsigned char *>(&ret), sizeof(NET_CTRL_FAIL));
					}
					else
					{		
						MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_CFG_SUCC, NULL, 0);
					}

#ifdef __DVR_ULTIMATE__
					m_pNetDeviceMan->SetNetConfig(pCfgData, dataLen);
#endif

					m_pMsgMan->PutMsgToClient(m_localMsgData);

					delete [] pCfgData;
				}
				else
				{
					assert(0);
					printf("%s:%s:%d, get the all channel rotato error\n", __FILE__, __FUNCTION__, __LINE__);
					NET_CTRL_FAIL ret = NETERR_FAIL;
					MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_CFG_FAIL, reinterpret_cast<unsigned char *>(&ret), sizeof(NET_CTRL_FAIL));
					m_pMsgMan->PutMsgToClient(m_localMsgData);
				}
#endif
			}
			break;
		case CMD_REQUEST_SAVE_IMAGE_DISTORTION:
			{
#ifdef  __DVR_ULTIMATE__
				bool bSetSingle = true;
				const NET_CHNN_LENS_DISTORTION *pLesDistortion = reinterpret_cast< const NET_CHNN_LENS_DISTORTION * >( msgData.pData + sizeof( PACKCMD ) );

				if(m_netVideoInputNum > pLesDistortion->chnn){
					bSetSingle = true;
				}else{
					bSetSingle = false;
				}

				unsigned long dataLen = 0;
				unsigned char *pCfgData = NULL;

				NCFG_ITEM_HEAD itemHead;
				memset( &itemHead, 0, sizeof( NCFG_ITEM_HEAD ) );
				itemHead.itemID = NCFG_ITEM_CAM_LENS_DISTORTION;
				//读出所有通道的畸变校正配置
				if (m_pConfig->GetConfig(reinterpret_cast<unsigned char *>(&itemHead), sizeof( NCFG_ITEM_HEAD ), &pCfgData, dataLen))
				{	
					assert( dataLen == ( ( sizeof( LENS_DISTORTION_CONFIG ) * m_netVideoInputNum ) + sizeof( NCFG_BLOCK_HEAD ) + sizeof( NCFG_ITEM_HEAD ) ) );

					NCFG_BLOCK_HEAD *pBlockHead = reinterpret_cast<NCFG_BLOCK_HEAD *>( pCfgData );
					assert( pBlockHead->biSize == sizeof( NCFG_BLOCK_HEAD ) );
					assert( pBlockHead->ItemNum == 1 );
					assert( pBlockHead->netcfgver == NCFG_VERSION );

					NCFG_ITEM_HEAD *pItemHead = reinterpret_cast< NCFG_ITEM_HEAD * >( pCfgData + sizeof( NCFG_BLOCK_HEAD ) );
					assert( pItemHead->itemID == NCFG_ITEM_CAM_LENS_DISTORTION );
					assert( pItemHead->len	  == ( sizeof( LENS_DISTORTION_CONFIG ) * m_netVideoInputNum ) );
					assert( pItemHead->num	  == m_netVideoInputNum );
					assert( pItemHead->subLen == sizeof( LENS_DISTORTION_CONFIG ) );

					LENS_DISTORTION_CONFIG *pAllChnn = reinterpret_cast<LENS_DISTORTION_CONFIG *>(pCfgData + sizeof( NCFG_BLOCK_HEAD ) + sizeof(NCFG_ITEM_HEAD) );

					if(bSetSingle)
					{
						//刷新相关通道的
						pAllChnn[pLesDistortion->chnn].ucLensDistortion = pLesDistortion->lensDistortion.ucLensDistortion;
						pAllChnn[pLesDistortion->chnn].ucLensDistortionValue = pLesDistortion->lensDistortion.ucLensDistortionValue;
						pAllChnn[pLesDistortion->chnn].noUse[0] = pLesDistortion->lensDistortion.noUse[0];
						pAllChnn[pLesDistortion->chnn].noUse[1] = pLesDistortion->lensDistortion.noUse[1];
					}
					else
					{
						/*for (unsigned int i = 0; i < m_netVideoInputNum; i++)
						{
							pAllChnn[i].ucLensDistortion = (pLesDistortion->lensDistortion).ucLensDistortion;
							pAllChnn[i].ucLensDistortionValue = (pLesDistortion->lensDistortion).ucLensDistortionValue;
							pAllChnn[i].noUse[0] = (pLesDistortion->lensDistortion).noUse[0];
							pAllChnn[i].noUse[1] = (pLesDistortion->lensDistortion).noUse[1];
							++pLesDistortion->lensDistortion;
						}*/
					}

					if( !m_pConfig->SetConfig( pCfgData, dataLen ) )
					{
						NET_CTRL_FAIL ret = NETERR_FAIL;
						MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_CFG_FAIL, reinterpret_cast<unsigned char *>(&ret), sizeof(NET_CTRL_FAIL));
					}
					else
					{		
						MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_CFG_SUCC, NULL, 0);
					}

#ifdef __DVR_ULTIMATE__
					m_pNetDeviceMan->SetNetConfig(pCfgData, dataLen);
#endif

					m_pMsgMan->PutMsgToClient(m_localMsgData);

					delete [] pCfgData;
				}
				else
				{
					assert(0);
					printf("%s:%s:%d, get the all channel lens distortion error\n", __FILE__, __FUNCTION__, __LINE__);
					NET_CTRL_FAIL ret = NETERR_FAIL;
					MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_CFG_FAIL, reinterpret_cast<unsigned char *>(&ret), sizeof(NET_CTRL_FAIL));
					m_pMsgMan->PutMsgToClient(m_localMsgData);
				}
#endif
			}
			break;
		case CMD_REQUEST_GET_IMAGE_MIRROR:
			{
#ifdef  __DVR_ULTIMATE__
				bool bGetSingle = true;
				NET_CHNN_MIRROR* pMirror = NULL;
				const NET_CHNN_MIRROR *pRqstMirror = reinterpret_cast<const NET_CHNN_MIRROR *>(msgData.pData + sizeof(PACKCMD));
				if(m_netVideoInputNum > pRqstMirror->chnn){
					bGetSingle = true;
					pMirror = new NET_CHNN_MIRROR;
					memset(pMirror,0,sizeof(NET_CHNN_MIRROR));
					pMirror->chnn = pRqstMirror->chnn;
				}else{
					bGetSingle = false;
					pMirror = new NET_CHNN_MIRROR[m_netVideoInputNum];
					memset(pMirror,0,sizeof(NET_CHNN_MIRROR)*m_netVideoInputNum);
				}

				unsigned long dataLen = 0;
				unsigned char *pCfgData = NULL;

				NCFG_ITEM_HEAD itemHead;
				memset(&itemHead, 0, sizeof(NCFG_ITEM_HEAD));
				itemHead.itemID = NCFG_ITEM_CAM_MIRROR;

				if (m_pConfig->GetConfig(reinterpret_cast<unsigned char *>(&itemHead), sizeof(NCFG_ITEM_HEAD), &pCfgData, dataLen))
				{	
					const unsigned char *pTemp = reinterpret_cast<const unsigned char *>(pCfgData); 
					assert(dataLen == (sizeof(unsigned char) * m_netVideoInputNum) + sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD));
					if(dataLen == (sizeof(unsigned char) * m_netVideoInputNum) + sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD))
					{
						assert(NULL != pTemp);
						if(bGetSingle)
						{
							pTemp += (sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD) + pMirror->chnn);
							memcpy(&(pMirror->mirror), pTemp, sizeof(unsigned char));
						}
						else
						{
							pTemp += (sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD));
							for (int i = 0; i < m_netVideoInputNum; i++)
							{
								memcpy(&(pMirror[i].mirror), pTemp, sizeof(unsigned char));
								pMirror[i].chnn = i;
								pTemp += 1;
							}
						}
					}
					delete [] pCfgData;
				}else{			
					printf("%s:%s:%d, get data, error\n\n", __FUNCTION__, __FILE__, __LINE__);
				}

				if(bGetSingle){
					MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_GET_IMAGE_MIRROR, reinterpret_cast<unsigned char *>(pMirror), sizeof(NET_CHNN_MIRROR ));
					delete pMirror;
					pMirror = NULL;
				}else{
					MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_GET_IMAGE_MIRROR, reinterpret_cast<unsigned char *>(pMirror), sizeof(NET_CHNN_MIRROR) * m_netVideoInputNum);
					delete [] pMirror;
					pMirror = NULL;
				}
				m_pMsgMan->PutMsgToClient(m_localMsgData);
#endif
			}
			break;
		case CMD_REQUEST_GET_IMAGE_FILP:
			{
#ifdef  __DVR_ULTIMATE__
				bool bGetSingle = true;
				NET_CHNN_FLIP* pFlip = NULL;
				const NET_CHNN_FLIP* pRqstFlip = reinterpret_cast<const NET_CHNN_FLIP *>(msgData.pData + sizeof(PACKCMD));
				if(m_netVideoInputNum > pRqstFlip->chnn){   
					bGetSingle = true;
					pFlip = new NET_CHNN_FLIP;
					memset(pFlip,0,sizeof(NET_CHNN_FLIP));
					pFlip->chnn = pRqstFlip->chnn;
				}else{
					bGetSingle = false;
					pFlip = new NET_CHNN_FLIP[m_netVideoInputNum];
					memset(pFlip,0,sizeof(NET_CHNN_FLIP)*m_netVideoInputNum);
				}

				unsigned long dataLen = 0;
				unsigned char *pCfgData = NULL;

				NCFG_ITEM_HEAD itemHead;
				memset(&itemHead, 0, sizeof(NCFG_ITEM_HEAD));
				itemHead.itemID = NCFG_ITEM_CAM_FILP;

				if (m_pConfig->GetConfig(reinterpret_cast<unsigned char *>(&itemHead), sizeof(NCFG_ITEM_HEAD), &pCfgData, dataLen))
				{	
					const unsigned char *pTemp = reinterpret_cast<const unsigned char *>(pCfgData); 
					assert(dataLen == (sizeof(unsigned char) * m_netVideoInputNum) + sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD));
					if(dataLen == (sizeof(unsigned char) * m_netVideoInputNum) + sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD))
					{
						assert(NULL != pTemp);
						if(bGetSingle)
						{
							pTemp += (sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD) + pFlip->chnn);
							memcpy(&(pFlip->flip), pTemp, sizeof(unsigned char));
						}
						else
						{
							pTemp += (sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD));
							for (int i = 0; i < m_netVideoInputNum; i++)
							{
								memcpy(&(pFlip[i].flip), pTemp, sizeof(unsigned char));
								pFlip[i].chnn = i;
								pTemp += 1;
							}
						}
					}
					delete [] pCfgData;
				}else{			
					printf("%s:%s:%d, get data, error\n\n", __FUNCTION__, __FILE__, __LINE__);
				}

				if(bGetSingle){
					MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_GET_IMAGE_FILP, reinterpret_cast<unsigned char *>(pFlip), sizeof(NET_CHNN_FLIP ));
					delete pFlip;
					pFlip = NULL;
				}else{
					MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_GET_IMAGE_FILP, reinterpret_cast<unsigned char *>(pFlip), sizeof(NET_CHNN_FLIP) * m_netVideoInputNum);
					delete [] pFlip;
					pFlip = NULL;
				}
				m_pMsgMan->PutMsgToClient(m_localMsgData);
#endif
			}
			break;
		case CMD_REQUEST_GET_IMAGE_ROTATO:
			{
#ifdef  __DVR_ULTIMATE__
				bool bGetSingle = true;
				NET_CHNN_ROTATO* pRotato = NULL;
				const NET_CHNN_ROTATO* pRqstRotato = reinterpret_cast<const NET_CHNN_ROTATO *>(msgData.pData + sizeof(PACKCMD));
				if(m_netVideoInputNum > pRqstRotato->chnn){   
					bGetSingle = true;
					pRotato = new NET_CHNN_ROTATO;
					memset(pRotato,0,sizeof(NET_CHNN_ROTATO));
					pRotato->chnn = pRqstRotato->chnn;
				}else{
					bGetSingle = false;
					pRotato = new NET_CHNN_ROTATO[m_netVideoInputNum];
					memset(pRotato,0,sizeof(NET_CHNN_ROTATO)*m_netVideoInputNum);
				}

				unsigned long dataLen = 0;
				unsigned char *pCfgData = NULL;

				NCFG_ITEM_HEAD itemHead;
				memset(&itemHead, 0, sizeof(NCFG_ITEM_HEAD));
				itemHead.itemID = NCFG_ITEM_CAM_ROTATO;

				if (m_pConfig->GetConfig(reinterpret_cast<unsigned char *>(&itemHead), sizeof(NCFG_ITEM_HEAD), &pCfgData, dataLen))
				{	
					const unsigned long *pTemp = reinterpret_cast<const unsigned long *>(pCfgData); 
					assert(dataLen == (sizeof(unsigned long) * m_netVideoInputNum) + sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD));
					if(dataLen == (sizeof(unsigned long) * m_netVideoInputNum) + sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD))
					{
						assert(NULL != pTemp);
						if(bGetSingle)
						{
							pTemp += (sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD) + pRotato->chnn);
							memcpy(&(pRotato->rotato), pTemp, sizeof(unsigned long));
						}
						else
						{
							pTemp += (sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD));
							for (int i = 0; i < m_netVideoInputNum; i++)
							{
								memcpy(&(pRotato[i].rotato), pTemp, sizeof(unsigned long));
								pRotato[i].chnn = i;
								pTemp += 1;
							}
						}
					}
					delete [] pCfgData;
				}else{			
					printf("%s:%s:%d, get data, error\n\n", __FUNCTION__, __FILE__, __LINE__);
				}

				if(bGetSingle){
					MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_GET_IMAGE_ROTATO, reinterpret_cast<unsigned char *>(pRotato), sizeof(NET_CHNN_ROTATO ));
					delete pRotato;
					pRotato = NULL;
				}else{
					MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_GET_IMAGE_ROTATO, reinterpret_cast<unsigned char *>(pRotato), sizeof(NET_CHNN_ROTATO) * m_netVideoInputNum);
					delete [] pRotato;
					pRotato = NULL;
				}
				m_pMsgMan->PutMsgToClient(m_localMsgData);
#endif
			}
			break;
		case CMD_REQUEST_GET_IMAGE_DISTORTION:
			{
#ifdef  __DVR_ULTIMATE__
				bool bGetSingle = true;
				NET_CHNN_LENS_DISTORTION* pLensDistortion = NULL;
				const NET_CHNN_LENS_DISTORTION* pRqstLensDistortion = reinterpret_cast<const NET_CHNN_LENS_DISTORTION *>(msgData.pData + sizeof(PACKCMD));
				if(m_netVideoInputNum > pRqstLensDistortion->chnn){   
					bGetSingle = true;
					pLensDistortion = new NET_CHNN_LENS_DISTORTION;
					memset(pLensDistortion,0,sizeof(NET_CHNN_LENS_DISTORTION));
					pLensDistortion->chnn = pRqstLensDistortion->chnn;
				}else{
					bGetSingle = false;
					pLensDistortion = new NET_CHNN_LENS_DISTORTION[m_netVideoInputNum];
					memset(pLensDistortion,0,sizeof(NET_CHNN_LENS_DISTORTION)*m_netVideoInputNum);
				}

				unsigned long dataLen = 0;
				unsigned char *pCfgData = NULL;
				unsigned char *pData    = NULL;
				const LENS_DISTORTION_CONFIG *pTemp = NULL;

				NCFG_ITEM_HEAD itemHead;
				memset(&itemHead, 0, sizeof(NCFG_ITEM_HEAD));
				itemHead.itemID = NCFG_ITEM_CAM_LENS_DISTORTION;

				if (m_pConfig->GetConfig(reinterpret_cast<unsigned char *>(&itemHead), sizeof(NCFG_ITEM_HEAD), &pCfgData, dataLen))
				{	
					pData = pCfgData;
					assert(dataLen == (sizeof(LENS_DISTORTION_CONFIG) * m_netVideoInputNum) + sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD));
					if(dataLen == (sizeof(LENS_DISTORTION_CONFIG) * m_netVideoInputNum) + sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD))
					{
						assert(NULL != pData);
						pData +=(sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD));
						pTemp = reinterpret_cast<const LENS_DISTORTION_CONFIG *>(pData);
						if(bGetSingle)
						{
							pTemp += pLensDistortion->chnn;
							pLensDistortion->lensDistortion.ucLensDistortion = pTemp->ucLensDistortion;
							pLensDistortion->lensDistortion.ucLensDistortionValue = pTemp->ucLensDistortionValue;
							pLensDistortion->lensDistortion.noUse[0] = pTemp->noUse[0];
							pLensDistortion->lensDistortion.noUse[1] = pTemp->noUse[1];
						}
						else
						{
							for (int i = 0; i < m_netVideoInputNum; i++)
							{
								pLensDistortion[i].lensDistortion.ucLensDistortion = pTemp[i].ucLensDistortion;
								pLensDistortion[i].lensDistortion.ucLensDistortionValue = pTemp[i].ucLensDistortionValue;
								pLensDistortion[i].lensDistortion.noUse[0] = pTemp[i].noUse[0];
								pLensDistortion[i].lensDistortion.noUse[1] = pTemp[i].noUse[1];
								pLensDistortion[i].chnn = i;
							}
						}
					}
					delete [] pCfgData;
					pCfgData = NULL;
					pTemp = NULL;
					pData = NULL;
				}else{			
					printf("%s:%s:%d, get data, error\n\n", __FUNCTION__, __FILE__, __LINE__);
				}

				if(bGetSingle){
					MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_GET_IMAGE_DISTORTION, reinterpret_cast<unsigned char *>(pLensDistortion), sizeof(NET_CHNN_LENS_DISTORTION));
					delete pLensDistortion;
					pLensDistortion = NULL;
				}else{
					MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_GET_IMAGE_DISTORTION, reinterpret_cast<unsigned char *>(pLensDistortion), sizeof(NET_CHNN_LENS_DISTORTION) * m_netVideoInputNum);
					delete [] pLensDistortion;
					pLensDistortion = NULL;
				}
				m_pMsgMan->PutMsgToClient(m_localMsgData);
#endif
			}
			break;
		case CMD_REQUEST_ADD_IPHONE_TOKEN:
			{
				DEVICE_TOKEN_INFO * pDtki = reinterpret_cast<DEVICE_TOKEN_INFO *>(msgData.pData + sizeof(PACKCMD));
				printf("%s:%s:%d, add device token info\n", __FUNCTION__, __FILE__, __LINE__);
				int retVal = m_pPushMan->AddDeviceTokenInfo(msgData.clientID, pDtki);
				printf("%s:%s:%d, add device token info\n", __FUNCTION__, __FILE__, __LINE__);
				MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_ADD_IPHONE_TOKEN, (unsigned char *)&retVal, sizeof(int));

				m_pMsgMan->PutMsgToClient(m_localMsgData);

				break;
			}
		case CMD_REQUEST_CHANNEL_CRRENT_VIDEO_ENCODE_INFO:
			{
#ifdef __DVR_ULTIMATE__
				unsigned long * pULChannel = reinterpret_cast<unsigned long *>(msgData.pData + sizeof(PACKCMD));

				int iDataLen = sizeof(CHN_VIDEO_ENCODE_INFO) + sizeof(CHN_VIDEO_ENCODE_INFO_DETAIL) * 50;
				char * pEncodeData = new char [iDataLen];
				memset(pEncodeData, 0, iDataLen);

				int dataLen = m_pNetDeviceMan->GetEncodeInfo(*pULChannel, pEncodeData, iDataLen);

				if (0 >= dataLen)
				{
					CHN_VIDEO_ENCODE_INFO * pVideoEncodeInfo = (CHN_VIDEO_ENCODE_INFO *) pEncodeData;
					memset(pVideoEncodeInfo, 0, sizeof(CHN_VIDEO_ENCODE_INFO));

					pVideoEncodeInfo->bValid = 0;
					pVideoEncodeInfo->channel = * pULChannel;

					MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_CHANNEL_CRRENT_VIDEO_ENCODE_INFO, (unsigned char *)pEncodeData,	sizeof(CHN_VIDEO_ENCODE_INFO));
				}
				else
				{
					MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_CHANNEL_CRRENT_VIDEO_ENCODE_INFO, (unsigned char *)pEncodeData,		dataLen);
				}

				if (NULL != pEncodeData)
				{
					delete [] pEncodeData;
					pEncodeData = NULL;
				}

				m_pMsgMan->PutMsgToClient(m_localMsgData);
#endif
				break;
			}
		case CMD_REQUEST_MODIFY_CHANNEL_VIDEO_ENCODE_INFO:
			{
#ifdef __DVR_ULTIMATE__
				PCHN_VIDEO_ENCODE_INFO pEncodeInfo = reinterpret_cast<PCHN_VIDEO_ENCODE_INFO>(msgData.pData + sizeof(PACKCMD));
				PCHN_VIDEO_ENCODE_INFO_DETAIL pEncodeInfoDetail = reinterpret_cast<PCHN_VIDEO_ENCODE_INFO_DETAIL>(pEncodeInfo + 1);

				int ret = m_pNetDeviceMan->SetEncodeInfo(pEncodeInfo->channel, pEncodeInfoDetail->stream, (char *)pEncodeInfoDetail->resolution, pEncodeInfoDetail->rate, pEncodeInfoDetail->encodeType, pEncodeInfoDetail->quality, pEncodeInfoDetail->lBitRateStream[0]);

				if (0 != ret)
				{
					MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_MODIFY_CHANNEL_VIDEO_ENCODE_INFO_FAIL, NULL, 0);
				}
				else
				{
					int iDataLen = sizeof(CHN_VIDEO_ENCODE_INFO) + sizeof(CHN_VIDEO_ENCODE_INFO_DETAIL);
					char * pEncodeData = new char [iDataLen];
					memset(pEncodeData, 0, iDataLen);

					ret = m_pNetDeviceMan->GetCurrentEncodeInfo(pEncodeInfo->channel, pEncodeData, iDataLen);
					if (ret != iDataLen)
					{
						MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_MODIFY_CHANNEL_VIDEO_ENCODE_INFO_FAIL, NULL, 0);
					}
					else
					{
						MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_MODIFY_CHANNEL_VIDEO_ENCODE_INFO_SUCC, (unsigned char *)pEncodeData, iDataLen);

						delete [] pEncodeData;
						pEncodeData = NULL;
					}
				}

				m_pMsgMan->PutMsgToClient(m_localMsgData);
#endif
				break;
			}
		case CMD_REQUEST_NET_CHANNEL_SUPPORT_VIDEO_ENCODE_INFO:
			{
#ifdef __DVR_ULTIMATE__
				unsigned long  * pStreamType = reinterpret_cast<unsigned long * >(msgData.pData + sizeof(PACKCMD));
				
				NET_CHANNEL_ENC_INFO * pNetChannelEncInfo = new NET_CHANNEL_ENC_INFO[m_netVideoInputNum];
				memset(pNetChannelEncInfo, 0, sizeof(NET_CHANNEL_ENC_INFO) * m_netVideoInputNum);

				int ret = m_pNetDeviceMan->GetNetEncodeInfo(* pStreamType, pNetChannelEncInfo);

				if (0 != ret)
				{
					MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_NET_CHANNEL_SUPPORT_VIDEO_ENCODE_INFO, NULL, 0);
				}
				else
				{
					//对分辨率进行排序
					for (int chnn=0; chnn<m_netVideoInputNum; chnn++)
					{
						NET_CHANNEL_ENC_INFO * pChannelEncInfo = pNetChannelEncInfo + chnn;
						if (1 == pChannelEncInfo->online)
						{
							for (int j=0; DVRVIDEO_SIZE_NONE != pChannelEncInfo->encInfo[j].videoSize; j++)
							{
								int k = 0;
								int minPos = j;
								int jIndex = GetDVRVideoIndex((VIDEO_SIZE)pChannelEncInfo->encInfo[minPos].videoSize);
								
								for (k=j+1; DVRVIDEO_SIZE_NONE != pChannelEncInfo->encInfo[k].videoSize; k++)
								{
									if (jIndex > GetDVRVideoIndex((VIDEO_SIZE)pChannelEncInfo->encInfo[k].videoSize))
									{
										jIndex = GetDVRVideoIndex((VIDEO_SIZE)pChannelEncInfo->encInfo[k].videoSize);
										minPos = k;
									}
								}
								
								if (DVRVIDEO_SIZE_NONE != pChannelEncInfo->encInfo[minPos].videoSize)
								{
									unsigned long	videoSize	= pChannelEncInfo->encInfo[minPos].videoSize;
									unsigned char	ucRate		= pChannelEncInfo->encInfo[minPos].ucRate;
									unsigned long	maxBitRate	= pChannelEncInfo->encInfo[minPos].maxBitRate;

									pChannelEncInfo->encInfo[minPos].videoSize	= pChannelEncInfo->encInfo[j].videoSize;
									pChannelEncInfo->encInfo[minPos].ucRate		= pChannelEncInfo->encInfo[j].ucRate;
									pChannelEncInfo->encInfo[minPos].maxBitRate	= pChannelEncInfo->encInfo[j].maxBitRate;

									pChannelEncInfo->encInfo[j].videoSize	= videoSize;
									pChannelEncInfo->encInfo[j].ucRate		= ucRate;
									pChannelEncInfo->encInfo[j].maxBitRate	= maxBitRate;
								}
							}
						}
					}
					
					MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_NET_CHANNEL_SUPPORT_VIDEO_ENCODE_INFO, (unsigned char *)pNetChannelEncInfo, sizeof(NET_CHANNEL_ENC_INFO) * m_netVideoInputNum);
				}

				delete [] pNetChannelEncInfo;
				
				m_pMsgMan->PutMsgToClient(m_localMsgData);
#else
				MsgData( m_localMsgData, msgData.clientID, CMD_REPLY_NO_SUPPORT, NULL, 0);
				m_pMsgMan->PutMsgToClient( m_localMsgData );
#endif
				break;
			}
		case CMD_REQUEST_SECOND_SUPPORT_INFO:
			{
				unsigned long resNum =  CCalculateSubEncode::Instance()->GetResNum();
				unsigned long subLen = resNum*m_localVideoInputNum;
				assert(subLen>0);
				SUPPORT_SUB_ENC_INFO * pSupportSubEncInfo = new SUPPORT_SUB_ENC_INFO[subLen];

				memset(pSupportSubEncInfo,0,subLen*sizeof(SUPPORT_SUB_ENC_INFO));

				if ( 0 == CCalculateSubEncode::Instance()->GetEncodeAllInfo(pSupportSubEncInfo,subLen) )
				{
					MsgData(m_localMsgData, msgData.clientID,CMD_REPLY_SECOND_SUPPORT_INFO, reinterpret_cast<unsigned char*>(pSupportSubEncInfo), subLen);
				}
				else
				{
					MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_SECOND_SUPPORT_INFO, NULL, 0);
				}

				m_pMsgMan->PutMsgToClient(m_localMsgData);

				delete [] pSupportSubEncInfo;
				pSupportSubEncInfo = NULL;
				break;
			}
		case CMD_REQUEST_SECOND_CHECK_ENC_INFO:
			{
				unsigned long channel = * reinterpret_cast<unsigned long *>( msgData.pData + sizeof(PACKCMD));
				CHECK_ENC_INFO * pCheckEncInfoOther = reinterpret_cast<CHECK_ENC_INFO *>( msgData.pData + sizeof(PACKCMD) + sizeof(unsigned long));

				CHECK_ENC_INFO * pCheckEncInfo = new CHECK_ENC_INFO [m_localVideoInputNum];
				memcpy(pCheckEncInfo, pCheckEncInfoOther, sizeof(CHECK_ENC_INFO) * m_localVideoInputNum);
				if (true == CCalculateSubEncode::Instance()->CheckEncodeParam(channel, pCheckEncInfo))
				{
					MsgData(m_localMsgData, msgData.clientID,CMD_REPLY_SECOND_CHECK_ENC_INFO, reinterpret_cast<unsigned char*>(pCheckEncInfo), sizeof(CHECK_ENC_INFO) * m_localVideoInputNum);
				}
				else
				{
					MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_SECOND_CHECK_ENC_INFO, NULL, 0);
				}

				m_pMsgMan->PutMsgToClient(m_localMsgData);

				delete [] pCheckEncInfo;
					pCheckEncInfo = NULL;
				break;
			}
		/*case CMD_REPLY_SECOND_SUB_CODE_ALL:
			{
			}*/
		case CMD_REQUEST_APP_DOWNLOAD_LINK_INFO:
		{
			string  InfoStr;
			CAppInfoServeManage *pAppInfoServMange = new CAppInfoServeManage;
			bool bRet = pAppInfoServMange->RequestServerInfo(InfoStr,REQUEST_APP_LINK_INFO);
			///
			VECTOR_APP_INFO vectorAppInfo;
			pAppInfoServMange->GetAppDownlinkInfo(InfoStr,vectorAppInfo);
			char *pData = NULL;
			if(vectorAppInfo.size() ==0)
			{
				bRet = 0;
			}
			
			APP_DOWNLOAD_INFO *pappDownlink = new APP_DOWNLOAD_INFO[vectorAppInfo.size()];
			memset(pappDownlink,0,sizeof(APP_DOWNLOAD_INFO)*vectorAppInfo.size());
			unsigned int i=0;
			for (VECTOR_APP_INFO::iterator it=vectorAppInfo.begin();it!=vectorAppInfo.end();it++)
			{
				if((*it).osPlateform.length() < (sizeof(pappDownlink[i].osPlateform)-1))
					memcpy(pappDownlink[i].osPlateform,(*it).osPlateform.c_str(),(*it).osPlateform.length());
				if((*it).appPlateForm.length() < (sizeof(pappDownlink[i].appPlateForm)-1))
					memcpy(pappDownlink[i].appPlateForm,(*it).appPlateForm.c_str(),(*it).appPlateForm.length());
				if((*it).AppName.length() < (sizeof(pappDownlink[i].AppName)-1))
					memcpy(pappDownlink[i].AppName,(*it).AppName.c_str(),(*it).AppName.length());
				if((*it).QrDownUrl.length() < (sizeof(pappDownlink[i].QrDownUrl)-1))
					memcpy(pappDownlink[i].QrDownUrl,(*it).QrDownUrl.c_str(),(*it).QrDownUrl.length());
				i++;
			}
			////
			pData = reinterpret_cast<char *>(pappDownlink);
			if(bRet)
			{
				MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_APP_DOWNLOAD_LINK_INFO_SUCC, reinterpret_cast<unsigned char *>(pData), sizeof(APP_DOWNLOAD_INFO)*vectorAppInfo.size());
			}
			else
			{
				MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_APP_DOWNLOAD_LINK_INFO_FAIL, NULL, 0);
			}
			m_pMsgMan->PutMsgToClient(m_localMsgData);
			delete pAppInfoServMange;
			delete [] pappDownlink;
			break;
		}
		case CMD_REQUEST_TOKEN_SERVER_INFO:
		{
			string  InfoStr;
			CAppInfoServeManage *pAppInfoServMange = new CAppInfoServeManage;
			bool bRet = pAppInfoServMange->RequestServerInfo(InfoStr,REQUEST_TOKEN_SERVER_INFO);
			TOKEN_SERVER_INFO tokenServerInfo;
			if(bRet)
			{
				memset(&tokenServerInfo,0,sizeof(tokenServerInfo));
				bRet = pAppInfoServMange->GetTokenServerInfo(InfoStr,tokenServerInfo);
			}
			char *pData = tokenServerInfo.tokenServerIp;
			if(bRet)
			{
				MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_TOKEN_SERVER_INFO_SUCC, reinterpret_cast<unsigned char *>(pData), sizeof(tokenServerInfo));
			}
			else
			{
				MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_TOKEN_SERVER_INFO_FAIL, NULL, 0);
			}
			m_pMsgMan->PutMsgToClient(m_localMsgData);
			delete pAppInfoServMange;
			break;
		}
		case CMD_REQUEST_NET_CHANNEL_VIDEO_DEF_BITSTREAM:
			{
				if (IsStaticFrame())
				{
					int iCount = 0;
					for (iCount=0; ; iCount++)
					{
						if (0 == g_vsBitRateInfo[iCount].bitRate)
						{
							break;
						}
					}
					MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_NET_CHANNEL_VIDEO_DEF_BITSTREAM, (unsigned char *)&g_vsBitRateInfo, sizeof(VIDEO_SIZE_BITRATE_INFO) * iCount);
				}
				else
				{
					MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_NET_CHANNEL_VIDEO_DEF_BITSTREAM, NULL, 0);
				}

				m_pMsgMan->PutMsgToClient(m_localMsgData);

				break;
			}
		case CMD_REQUEST_CHANGE_CVBS:
			{
				NCFG_CVBS_INFO *pTempCvbs = reinterpret_cast<NCFG_CVBS_INFO *>( msgData.pData + sizeof(PACKCMD));
				if ((2==pTempCvbs->last)&&((0 == pTempCvbs->current)||(1 == pTempCvbs->current)))
				{
					UpdateCVBS();
				} 
				MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_CFG_SUCC, NULL, 0);
				m_pMsgMan->PutMsgToClient(m_localMsgData);
				if ( ((2==pTempCvbs->last)&&((0 == pTempCvbs->current)||(1 == pTempCvbs->current))) ||
					 ((2==pTempCvbs->current)&&((0 == pTempCvbs->last)||(1==pTempCvbs->last)) ) 
					)
				{
					g_system_status = DVR_SYSTEM_REBOOT;
				}
				break;
			}
	default:
			{
				printf("%s:%s:%d, Get error cmd=0x%08x\n", __FUNCTION__, __FILE__, __LINE__, cmdType);
				//没有处理的ID，都认为是不支持的功能，所以返回不支持。
				MsgData( m_localMsgData, msgData.clientID, CMD_REPLY_NO_SUPPORT, NULL, 0);
				m_pMsgMan->PutMsgToClient( m_localMsgData );
			}
		}	
	}
}

#ifdef  __MOBILE_DVR__
//wifi用来临时测试用的
int CMainFrame::SearchSignalForWIFI(int *pCount, NET_WIFI_MODEM *pWifiModem)
{
	char strBuf[1024] = {0}; 
	unsigned char num = 0;
	int iRSSI = 0;				//ssid
	char networkType[12]={0};
	char mac[24]= {0};
	char authMode[24]={0};		//认证模式
	char encrypType[24]={0};	//加密类型
	char *FilePath = "E:\\work\\wifitest.txt";
	FILE*pFile = fopen(FilePath,"r");
	
	if(pFile == NULL)
	{
		printf("file open err\n");
		return -1;
	}
	if(fgets(strBuf,sizeof(strBuf),pFile) == NULL)
	{
		printf("open err!\n");
		fclose(pFile);
		pFile = NULL;
		return -1;
	}
	while(num < *pCount)
	{
		memset(strBuf, 0, sizeof(strBuf));
		memset(authMode, 0, sizeof(authMode));
		memset(encrypType,0, sizeof(encrypType));
		
		if(fgets(strBuf,sizeof(strBuf),pFile) == NULL)
		{
			break;
		}
		else
		{
			if((strBuf[0]== 10) && (strBuf[1] == 0))
			{
				break;
			}
		}		
		sscanf(strBuf, "%lu", &pWifiModem[num].dwChannel);
		sscanf(strBuf+8,"%d", &iRSSI);
		memset(pWifiModem[num].ssid, 0, sizeof(pWifiModem[num].ssid));
		if (0 == memcmp(strBuf+16,"                                    ",36))
		{
			memset(pWifiModem[num].ssid, 0, sizeof(pWifiModem[num].ssid));
		}
		else
		{
			sscanf(strBuf+16,"%s",pWifiModem[num].ssid);
		}

		memset(mac, 0, sizeof(mac));
		memcpy(mac, strBuf + 16 + 36, 17);

		memset(encrypType, 0, sizeof(encrypType));
		memcpy(encrypType, strBuf + 16 + 36 + 18, 12);

		memset(authMode, 0, sizeof(authMode));
		memcpy(authMode, strBuf + 16 + 36 + 18 + 12, 20);

		memset(networkType, 0, sizeof(networkType));
		memcpy(networkType, strBuf + 16 + 36 + 18 + 12 + 20, 5);

		if (-90 >= iRSSI)
		{
			pWifiModem[num].dwSignal = 0;
		}
		else if ( -81 >= iRSSI)
		{
			pWifiModem[num].dwSignal = 1;
		}
		else if(-71 >= iRSSI)
		{
			pWifiModem[num].dwSignal = 2;
		}
		else if (-67 >= iRSSI)
		{
			pWifiModem[num].dwSignal = 3;
		}
		else if(-57 >= iRSSI)
		{
			pWifiModem[num].dwSignal = 4;
		}
		else
		{
			pWifiModem[num].dwSignal = 5;
		}

		if (NULL != strstr(authMode, "OPEN"))
		{
			pWifiModem[num].dwAuthMode = WIFI_AUTH_OPEN;
		}
		else if (NULL != strstr(authMode, "SHARED"))
		{
			pWifiModem[num].dwAuthMode = WIFI_AUTH_SHARE;
		}
		else if (NULL != strstr(authMode, "WPA-PSK"))
		{
			pWifiModem[num].dwAuthMode = WIFI_AUTH_WPA_PSK;
		}
		else if ((NULL != strstr(authMode, "WPA2-PSK")) || (NULL != strstr(authMode, "WPA(2)-PSK")))
		{
			pWifiModem[num].dwAuthMode = WIFI_AUTH_WPA2_PSK;
		}
		else
		{
			assert(false);
		}
		
		if(NULL != strstr(encrypType, "NONE"))
		{
			pWifiModem[num].dwEncrypType = WIFI_ENCRYP_NONE;
		}
		else if(NULL != strstr(encrypType, "WEP"))
		{
			pWifiModem[num].dwEncrypType = WIFI_ENCRYP_WEP;
		}
		else if(NULL != strstr(encrypType, "AES"))
		{
			pWifiModem[num].dwEncrypType = WIFI_ENCRYP_AES;	
		}
		else if(NULL != strstr(encrypType, "TKIP"))
		{
			pWifiModem[num].dwEncrypType = WIFI_ENCRYP_TKIP;			
		}
		else
		{
			assert(false);
		}
		++num;
		if(num == *pCount)
		{
			break;
		} 
	}

	fclose(pFile);
	pFile = NULL;
	
	*pCount = num;
	return num;

}

//临时测试使用
int CMainFrame::TestWIFIConnect(WIFI_CONFIG &wificfg)
{
	return -1;
}
#endif

void CMainFrame::NetMessageProc(const MESSAGE_DATA &msgData)
{
	assert (0 != msgData.deviceID);

	NET_PROTOCOL cmdType = Protocol(msgData);
	
	if (CMD_REQUEST_CFG_ENTER == cmdType)
	{
		if (INVALID_CLIENT_ID != m_pConfig->cfgClient())
		{
			NET_CTRL_FAIL ret = NETERR_OPERATE_FAIL_OPERATING;
			MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_CFG_FAIL, reinterpret_cast<unsigned char *>(&ret), sizeof(NET_CTRL_FAIL));
		}
		else
		{
			m_pConfig->EnterConfig(msgData.clientID);
			/*unsigned char *pData = new unsigned char [4+sizeof(NET_CFG_PAGE_INFO)*64];
			*(reinterpret_cast<unsigned long *>(pData)) = m_pConfig->GetConfigPage(reinterpret_cast<NET_CFG_PAGE_INFO *>(pData+4));
			assert (*(reinterpret_cast<unsigned long *>(pData)) > 0);
			MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_CFG_SUCC, pData, 4+sizeof(NET_CFG_PAGE_INFO)*64);
			delete [] pData;*/
			MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_CFG_SUCC, NULL, 0);
		}

		m_pMsgMan->PutMsgToClient(m_localMsgData); 
	}
	else if (CMD_REQUEST_CFG_GET == cmdType)
	{
		if (msgData.clientID != m_pConfig->cfgClient())
		{
			NET_CTRL_FAIL ret = NETERR_MUST_ENTER_CFG;
			MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_CFG_FAIL, reinterpret_cast<unsigned char *>(&ret), sizeof(NET_CTRL_FAIL));
		}
		else
		{
			unsigned long dataLen = 0;
			unsigned char *pCfgData = NULL;
			if (m_pConfig->GetConfig(reinterpret_cast<unsigned char *>(msgData.pData+sizeof(PACKCMD)), msgData.dataLen-sizeof(PACKCMD), &pCfgData, dataLen))
			{
				MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_CFG_DATA, pCfgData, dataLen);

				delete [] pCfgData;
			}
			else
			{
				NET_CTRL_FAIL ret = NETERR_FAIL;
				MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_CFG_FAIL, reinterpret_cast<unsigned char *>(&ret), sizeof(NET_CTRL_FAIL));
			}
		}

		m_pMsgMan->PutMsgToClient(m_localMsgData); 
	}
	else if (CMD_REQUEST_CFG_SET == cmdType)
	{
		if (msgData.clientID != m_pConfig->cfgClient())
		{
			NET_CTRL_FAIL ret = NETERR_MUST_ENTER_CFG;
			MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_CFG_FAIL, reinterpret_cast<unsigned char *>(&ret), sizeof(NET_CTRL_FAIL));
			m_pMsgMan->PutMsgToClient(m_localMsgData);
		}
		else
		{
			if (!m_pConfig->SetConfig(reinterpret_cast<unsigned char *>(msgData.pData+sizeof(PACKCMD)), msgData.dataLen-sizeof(PACKCMD)))
			{
				NET_CTRL_FAIL ret = NETERR_FAIL;
				MsgData(m_localMsgData, msgData.clientID, CMD_REPLY_CFG_FAIL, reinterpret_cast<unsigned char *>(&ret), sizeof(NET_CTRL_FAIL));
				m_pMsgMan->PutMsgToClient(m_localMsgData);
			}
		} 
	}
	else if (CMD_REQUEST_CFG_LEAVE == cmdType)
	{
		assert (msgData.clientID == m_pConfig->cfgClient());

		m_pConfig->ExitConfig();
	}
	else if(CMD_REQUEST_COLOR_GET == cmdType)
	{
		NET_CHNN_COLOR chnnColor;
		memset(&chnnColor, 0, sizeof(NET_CHNN_COLOR));

		const NET_CHNN_COLOR *pReqChnnClr = reinterpret_cast<const NET_CHNN_COLOR *>(msgData.pData + sizeof(PACKCMD));

		chnnColor.chnn = pReqChnnClr->chnn;

		MsgData(m_netMsgData, msgData.clientID, CMD_REPLY_CHNN_CTRL_COLOR, reinterpret_cast<unsigned char *>(&chnnColor), sizeof(NET_CHNN_COLOR ));
		m_netMsgData.deviceID = NET_DEVICE_ID;

		m_pMsgMan->PutMsgToClient(m_netMsgData);
	}
	else if(CMD_REQUEST_COLOR_SET == cmdType)
	{
		const NET_CHNN_COLOR *pReqChnnClr = reinterpret_cast<const NET_CHNN_COLOR *>(msgData.pData + sizeof(PACKCMD));
#ifdef __DVR_ULTIMATE__
		//if(!m_pNetDeviceMan->SetVideoColor(*pReqChnnClr))
		//{
		//	;
		//}
#endif
	}
	else if(CMD_REQUEST_COLOR_SAVE == cmdType)
	{
		const NET_CHNN_COLOR *pReqChnnClr = reinterpret_cast<const NET_CHNN_COLOR *>(msgData.pData + sizeof(PACKCMD));

		MsgData(m_netMsgData, msgData.clientID, CMD_REPLY_CFG_SUCC, NULL, 0);
		m_netMsgData.deviceID = NET_DEVICE_ID;
		m_pMsgMan->PutMsgToClient(m_netMsgData);
	}
	else if(CMD_REQUEST_COLOR_DEFAULT == cmdType)
	{
		NET_CHNN_COLOR chnnColor;
		memset(&chnnColor, 0, sizeof(NET_CHNN_COLOR));

		const NET_CHNN_COLOR *pReqChnnClr = reinterpret_cast<const NET_CHNN_COLOR *>(msgData.pData + sizeof(PACKCMD));
#ifdef __DVR_ULTIMATE__
		//if(!m_pNetDeviceMan->GetDefaultVideoColor(pReqChnnClr->chnn, chnnColor))
		//{
		//	chnnColor.color = 0;
		//}
#endif
		chnnColor.chnn = pReqChnnClr->chnn;

		MsgData(m_netMsgData, msgData.clientID, CMD_REPLY_CHNN_CTRL_COLOR, reinterpret_cast<unsigned char *>(&chnnColor), sizeof(NET_CHNN_COLOR ));
		m_netMsgData.deviceID = NET_DEVICE_ID;

		m_pMsgMan->PutMsgToClient(m_netMsgData);
	}
	else if(CMD_REQUEST_PTZ_CTRL == cmdType)
	{
		;
	}
	else
	{
		printf("Msg = %x, %s, %d\n", cmdType, __FILE__, __LINE__);
		//没有处理的ID，都认为是不支持的功能，所以返回不支持。
		MsgData( m_localMsgData, msgData.clientID, CMD_REPLY_NO_SUPPORT, NULL, 0);
		m_pMsgMan->PutMsgToClient( m_localMsgData );
	}
}

NET_PROTOCOL CMainFrame::Protocol(const MESSAGE_DATA & msgData)
{
	if (NULL != msgData.pData)
	{
		PACKCMD *pCmd = reinterpret_cast<PACKCMD *>(msgData.pData);
		return (NET_PROTOCOL)pCmd->cmdType;
	}
	else
	{
		return CMD_REQUEST_LOGOUT;
	}
}

void CMainFrame::MsgData(MESSAGE_DATA &msgData, unsigned long clientID, long cmdType, unsigned char *pData, unsigned long dataLen)
{
	m_cmd.cmdType	= cmdType;
	m_cmd.cmdVer	= NET_PROTOCOL_VER;
	m_cmd.dataLen	= dataLen;

	msgData.clientID = clientID;
	msgData.deviceID = 0;
	msgData.dataLen = sizeof(PACKCMD) + dataLen;

	msgData.pData = new char [msgData.dataLen];

	memcpy(msgData.pData, &m_cmd, sizeof(PACKCMD));

	if ((NULL != pData) && (0 < dataLen))
	{
		memcpy(msgData.pData + sizeof(PACKCMD), pData, dataLen);
	}
}

//////////////////////////////////////////////////////////////////////////
void CMainFrame::Repair()
{
	assert (NULL != m_pReclogMan);

	LONGLONG endTime = GetCurrTime();
	FILE_INFO_EX_LIST fileInfoList;
	m_pReclogMan->GetUnfinishedFile(fileInfoList);

	if (fileInfoList.GetCount() > 0)
	{
		char path [512] = {0};

		POS pos = fileInfoList.GetHeadPosition();
		while (NULL != pos)
		{
			FILE_INFO_EX & node = fileInfoList.GetNext(pos);

#ifdef __ENVIRONMENT_WIN32__
			if (1 < strlen(m_pDiskPath))
			{
				sprintf(path, "%s\\%02d\\%08d.dat", m_pDiskPath, node.diskIndex, node.fileIndex);
			}
			else
			{
				assert (1 == strlen(m_pDiskPath));
				sprintf(path, "%c:\\%08d.dat", m_pDiskPath[0]+node.diskIndex, node.fileIndex);
			}
#else
			sprintf(path, "%s/%02d/%08d.dat", m_pDiskPath, node.diskIndex, node.fileIndex);
#endif

#if defined(__PRIMARY_WRDISK__)
			if (CPrimaryRecReader::Repair(path, &endTime))
#else
			if (CRecReader::Repair(path, &endTime))
#endif
			{
				//获得最后一帧的时间，用于修复。				
				m_pReclogMan->Repair(node, endTime/1000000);
			}
			else
			{
				//修复失败，则把该文件删除掉。
				if (!m_pReclogMan->DeleteFile(node))
				{
					printf("Delete repair fail file %s fail\n", path);
					assert(false);
				}				
			}
		}
	}
}

bool CMainFrame::SetupConfig()
{
	unsigned char *pData = NULL;
	unsigned long dateLen = 0;

	CMyList <NCFG_ITEM_ID> itemList;

	//
	//for (unsigned long i=NCFG_ITEM_DEVICE_ID; i<NCFG_ITEM_SYSTEM_END; ++i)
	//{
	//	itemList.AddTail(static_cast<NCFG_ITEM_ID>(i));
	//}
	for (unsigned long i=NCFG_ITEM_DEVICE_ID; i<NCFG_ITEM_TIME_ZONE; ++i)
	{
		itemList.AddTail(static_cast<NCFG_ITEM_ID>(i));
	}
	
	for (unsigned long i=NCFG_ITEM_TIME_SERVER; i<NCFG_ITEM_DAYLIGHT_INFO; ++i)
	{
		itemList.AddTail(static_cast<NCFG_ITEM_ID>(i));
	}
	for (unsigned long i=NCFG_ITEM_MAX_CLIENT_NUM; i<NCFG_ITEM_SYSTEM_END; ++i)
	{
		itemList.AddTail(static_cast<NCFG_ITEM_ID>(i));
	}

	if (m_pConfig->GetConfigEx(itemList, &pData, dateLen))
	{
		m_pConfigSetMan->TakeEffect(pData, dateLen);
		itemList.RemoveAll();
		delete [] pData;
		pData = NULL;
	}
	else
	{
		return false;
	}

	//
	for (unsigned long i=NCFG_ITEM_CAMERA_NAME; i<NCFG_ITEM_CHANNEL_END; ++i)
	{
		itemList.AddTail(static_cast<NCFG_ITEM_ID>(i));
	}

	if (m_pConfig->GetConfigEx(itemList, &pData, dateLen))
	{
		m_pConfigSetMan->TakeEffect(pData, dateLen);
		itemList.RemoveAll();
		delete [] pData;
		pData = NULL;
	}
	else
	{
		return false;
	}

	//
	for (unsigned long i=NCFG_ITEM_REC_ENABLE; i<NCFG_ITEM_CHANNEL_REC_END; ++i)
	{
		itemList.AddTail(static_cast<NCFG_ITEM_ID>(i));
	}

	if (m_pConfig->GetConfigEx(itemList, &pData, dateLen))
	{
		m_pConfigSetMan->TakeEffect(pData, dateLen);
		itemList.RemoveAll();
		delete [] pData;
		pData = NULL;
	}
	else
	{
		return false;
	}

	//
	for (unsigned long i=NCFG_ITEM_ENCODE_MAJOR; i<NCFG_ITEM_ENCODE_END; ++i)
	{
		itemList.AddTail(static_cast<NCFG_ITEM_ID>(i));
	}

	if (m_pConfig->GetConfigEx(itemList, &pData, dateLen))
	{
		m_pConfigSetMan->TakeEffect(pData, dateLen);
		itemList.RemoveAll();
		delete [] pData;
		pData = NULL;
	}
	else
	{
		return false;
	}

	//
	for (unsigned long i=NCFG_ITEM_REC_SCH_SCHEDULE_WEEK; i<NCFG_ITEM_SCHEDULE_END; ++i)
	{
		itemList.AddTail(static_cast<NCFG_ITEM_ID>(i));
	}

	if (m_pConfig->GetConfigEx(itemList, &pData, dateLen))
	{
		m_pConfigSetMan->TakeEffect(pData, dateLen);
		itemList.RemoveAll();
		delete [] pData;
		pData = NULL;
	}
	else
	{
		return false;
	}

	//
	for (unsigned long i=NCFG_ITEM_UPNP_ENABLE; i<NCFG_ITEM_NET_END; ++i)
	{
		itemList.AddTail(static_cast<NCFG_ITEM_ID>(i));
	}

	if (m_pConfig->GetConfigEx(itemList, &pData, dateLen))
	{
		m_pConfigSetMan->TakeEffect(pData, dateLen);
		itemList.RemoveAll();
		delete [] pData;
		pData = NULL;
	}
	else
	{
		return false;
	}

	for (unsigned long i=NCFG_ITEM_BUZZER_ENABLE; i<NCFG_ITEM_ALARM_TRIGER_END; ++i)
	{
		itemList.AddTail(static_cast<NCFG_ITEM_ID>(i));
	}

	if (m_pConfig->GetConfigEx(itemList, &pData, dateLen))
	{
		m_pConfigSetMan->TakeEffect(pData, dateLen);
		itemList.RemoveAll();
		delete [] pData;
		pData = NULL;
	}
	else
	{
		return false;
	}

	for (unsigned long i=NCFG_ITEM_SENSOR_IN_ENABLE; i<NCFG_ITEM_ALARM_TRIGER_SENSOREND; ++i)
	{
		itemList.AddTail(static_cast<NCFG_ITEM_ID>(i));
	}

	if (m_pConfig->GetConfigEx(itemList, &pData, dateLen))
	{
		m_pConfigSetMan->TakeEffect(pData, dateLen);
		itemList.RemoveAll();
		delete [] pData;
		pData = NULL;
	}
	else
	{
		return false;
	}

	for (unsigned long i=NCFG_ITEM_MOTION_ENABLE; i<NCFG_ITEM_ALARM_TRIGER_MOTIONEND; ++i)
	{
		itemList.AddTail(static_cast<NCFG_ITEM_ID>(i));
	}

	if (m_pConfig->GetConfigEx(itemList, &pData, dateLen))
	{
		m_pConfigSetMan->TakeEffect(pData, dateLen);
		itemList.RemoveAll();
		delete [] pData;
		pData = NULL;
	}
	else
	{
		return false;
	}

	//video loss
	for (unsigned long i=NCFG_ITEM_VL_TO_ALARM_OUT; i<NCFG_ITEM_ALARM_TRIGER_VLOSSEND; ++i)
	{
		itemList.AddTail(static_cast<NCFG_ITEM_ID>(i));
	}

	if (m_pConfig->GetConfigEx(itemList, &pData, dateLen))
	{
		m_pConfigSetMan->TakeEffect(pData, dateLen);
		itemList.RemoveAll();
		delete [] pData;
		pData = NULL;
	}
	else
	{
		return false;
	}

	//other alarm
	for (unsigned long i=NCFG_ITEM_DISK_FULL_BUZZER; i<NCFG_ITEM_EXCEPTION_END; ++i)
	{
		itemList.AddTail(static_cast<NCFG_ITEM_ID>(i));
	}

	if (m_pConfig->GetConfigEx(itemList, &pData, dateLen))
	{
		m_pConfigSetMan->TakeEffect(pData, dateLen);
		itemList.RemoveAll();
		delete [] pData;
		pData = NULL;
	}
	else
	{
		return false;
	}

	if(m_videoInputNum > 0)
	{
		for( unsigned long i = 0; i < m_videoInputNum; ++i)//temporary
		{
			itemList.AddTail(static_cast<NCFG_ITEM_ID>(NCFG_ITEM_PTZ_INFO_PRESET_INFO_CHBASE + i));
		}

		if (m_pConfig->GetConfigEx(itemList, &pData, dateLen))
		{
			m_pConfigSetMan->TakeEffect(pData, dateLen);
			itemList.RemoveAll();
			delete [] pData;
			pData = NULL;
		}
		else
		{
			return false;
		}
	}

	//
	if(m_videoInputNum > 0)
	{
		for(unsigned long i = 0; i < m_videoInputNum; ++i)//temporary
		{
			itemList.AddTail(static_cast<NCFG_ITEM_ID>(NCFG_ITEM_PTZ_INFO_CRUISE_INFO_CHBASE + i));
		}

		if (m_pConfig->GetConfigEx(itemList, &pData, dateLen))
		{
			m_pConfigSetMan->TakeEffect(pData, dateLen);
			itemList.RemoveAll();
			delete [] pData;
			pData = NULL;
		}
		else
		{
			return false;
		}
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////
int CMainFrame::FormatDisk(DVR_DISK_INFO *pDiskInfo, int num)
{
#ifdef __ENVIRONMENT_LINUX__

	assert(NULL == m_pDiskInfoForFormat);
	assert(NULL != pDiskInfo);
	assert(PUB_THREAD_ID_NOINIT == m_formatDiskProcID);
	
	if (NULL != m_pDiskInfoForFormat)
	{
		delete [] m_pDiskInfoForFormat;
		m_pDiskInfoForFormat = NULL;
	}
    
	m_diskNumForFormat = num;
	m_pDiskInfoForFormat = new DVR_DISK_INFO [m_diskNumForFormat];
	if (NULL == m_pDiskInfoForFormat)
	{
		return -1;
	}
	memcpy(m_pDiskInfoForFormat, pDiskInfo, num*sizeof(DVR_DISK_INFO));


	if (PUB_THREAD_ID_NOINIT != m_formatDiskProcID)
	{
		PUB_ExitThread(&m_formatDiskProcID, &m_bFormatDiskProc);
	}
	m_formatDiskProcID = PUB_CreateThread(FormatDiskThread, this, &m_bFormatDiskProc);
	if(m_formatDiskProcID == PUB_CREATE_THREAD_FAIL)
	{
		PUB_PrintError(__FILE__,__LINE__);
		delete [] m_pDiskInfoForFormat;
		m_pDiskInfoForFormat = NULL;
		m_diskNumForFormat   = 0;
		return -1;
	}


	return 0;
#else
	char fileName [512] = {0};

	for (unsigned long i=0; i<4; ++i)
	{
		printf("format disk\n");
		sprintf(fileName, "%s\\%02d\\eventlog.bin", m_pDiskPath, i);
		CLog::CreateLogFile(fileName, sizeof(EVENT_LOG), 1000);

		sprintf(fileName, "%s\\%02d\\operlog.bin", m_pDiskPath, i);
		CLog::CreateLogFile(fileName, sizeof(OPERATOR_LOG), 1000);

		sprintf(fileName, "%s\\%02d\\reclog.bin", m_pDiskPath, i);
		CReclogEx::CreateLogFile(fileName, 512, m_videoInputNum);
	}
	return 0;
#endif
}


PUB_THREAD_RESULT PUB_THREAD_CALL CMainFrame::FormatDiskThread(void *pParam)
{
#ifdef __ENVIRONMENT_LINUX__
	pid_t pid = getpid();
	pid_t tid = syscall(__NR_gettid);
	printf("%s, %d, pid = %d, tid=%d\n", __FILE__, __LINE__, pid, tid);
#endif

	CMainFrame *pThis = reinterpret_cast<CMainFrame *>(pParam);
	pThis->FormatDiskProc();

	return 0;
}

int CMainFrame::FormatDiskProc()
{
#ifndef __ENVIRONMENT_LINUX__
	
	return 0;
#else

	DISK_LOG_INFO *pDiskStatus = NULL;
	bool bSucc = true;
	bool bStopRead = true;
	bool bStopRecord = true;
	bool bIsFormated = true;
	 
	while (m_bFormatDiskProc)
	{
#ifndef NDEBUG
		//在格式化之前退出CRunLog模块
		{
			CRunLog::Instance()->Quit();
			printf("%s,%d:CRunLog::Instance()->Quit()\n",__FILE__,__LINE__);
		}
#endif
		for (int i = 0; i < m_diskNumForFormat; ++i)
		{
			m_curDiskNumForFormat = i + 1;
			if (NET_DISK_TYPE_FIXEDDISK == m_pDiskInfoForFormat[i].diskType)
			{
				if (NET_DISK_NO_FORMAT != m_pDiskInfoForFormat[i].diskStatus)
				{
					if ((NET_DISK_READ_WRITE == m_pDiskInfoForFormat[i].diskProperty)
					  ||(NET_DISK_REDUNDANCY == m_pDiskInfoForFormat[i].diskProperty))
					{
						assert(NET_DISK_OWNED_BY_THIS == m_pDiskInfoForFormat[i].diskOwnedBy);
						assert(m_pDiskInfoForFormat[i].partitionNum > 0);
						bStopRead = true;
						bStopRecord = true;
						bIsFormated = true;
					}
					else if (NET_DISK_ONLY_READ == m_pDiskInfoForFormat[i].diskProperty)
					{
						assert(m_pDiskInfoForFormat[i].partitionNum > 0);
						bStopRead = true;
						bStopRecord = false;
						bIsFormated = true;
					}
				}
				else
				{
					//未格式过的硬盘
					assert(NET_DISK_OWNED_BY_THIS == m_pDiskInfoForFormat[i].diskOwnedBy);
					bStopRead = false;
					bStopRecord = false;
					bIsFormated = false;
				}
			}
			else
			{
				//不支持对非固定硬盘的格式化
				assert(false);
			}

			//准备格式化
			DVR_DISK_INFO_LIST diskInfoLstTmp;
			if (bStopRead)
			{
				//检查是否在读，如果在读，就停掉读取动作或给出错误提示
				m_pNetReader->Action(false);
			}
			if (bStopRecord)
			{
				m_pMotionMan->Action(false);
				m_pSensorInMan->Action(false);
				m_pVideoLossMan->Action(false);
				m_pOtherAlarm->Action(false);
				m_pRecMan->Action(false);
#ifdef __MOBILE_DVR__
				m_pSpeedMan->Action(false);
#endif
			}
			if (bIsFormated)
			{
				pDiskStatus = new DISK_LOG_INFO [m_pDiskInfoForFormat[i].partitionNum];
				for (int reclogIndex = 0; reclogIndex < m_pDiskInfoForFormat[i].partitionNum; ++reclogIndex)
				{
					pDiskStatus[reclogIndex].diskOwnedBy  = m_pDiskInfoForFormat[i].diskOwnedBy;
					pDiskStatus[reclogIndex].diskIndex    = m_pDiskInfoForFormat[i].diskpara + reclogIndex;
					pDiskStatus[reclogIndex].diskProperty = m_pDiskInfoForFormat[i].diskProperty;
				}									
				if (!m_pReclogMan->ActionDisk(pDiskStatus, m_pDiskInfoForFormat[i].partitionNum, false))
				{
					bSucc = false;
					break;
				}
				
				if (g_bHaveSnapPic)
				{
					if (!CSnapMan::GetInstance()->ActionDisk(pDiskStatus, m_pDiskInfoForFormat[i].partitionNum, false))
					{
						printf("CSnapMan action Disk fail status = %d\n", false);
						bSucc = false;
						break;
					}
				}

				m_pLogMan->ActionDisk(pDiskStatus, m_pDiskInfoForFormat[i].partitionNum, false);				
				if (NULL != pDiskStatus)
				{
					delete [] pDiskStatus;
					pDiskStatus = NULL;
				}
			}

			//开始格式化
			if (0 != m_pDiskMan->FormatDisk((DISK_TYPE)m_pDiskInfoForFormat[i].diskType, m_pDiskInfoForFormat[i].diskname, (MESSAGE_CALLBACK_EX)ProcCallBackMsg, this))
			{
				bSucc = false;
				break;
			}
			
			//结束格式化
			{
				if (PRERECORD_MODE_DISK == m_preRecordType)
				{
					unsigned char prerecordPartIndex = 0;
					unsigned char prerecordPartNum = 0;
					m_preRecordLMask    = 0;
					m_preRecordHMask    = 0;
					m_pDiskMan->GetOneRWDiskPartionNum(prerecordPartIndex, prerecordPartNum);
					m_pRecMan->ResetPreDiskRecordPath(prerecordPartIndex, prerecordPartNum, m_preRecordHMask, m_preRecordLMask);
				}
			}
			m_pDiskMan->GetDeviceDiskInfo((DISK_TYPE)m_pDiskInfoForFormat[i].diskType, m_pDiskInfoForFormat[i].diskname, diskInfoLstTmp);
			DVR_DISK_INFO &diskInfoTmp = diskInfoLstTmp.GetHead();
			pDiskStatus = new DISK_LOG_INFO [diskInfoTmp.partitionNum];
			for (int reclogIndex = 0; reclogIndex < diskInfoTmp.partitionNum; ++reclogIndex)
			{
				pDiskStatus[reclogIndex].diskOwnedBy  = diskInfoTmp.diskOwnedBy;
				pDiskStatus[reclogIndex].diskIndex    = diskInfoTmp.diskpara + reclogIndex;
				pDiskStatus[reclogIndex].diskProperty = diskInfoTmp.diskProperty;
			}
			if (bIsFormated)
			{
				if (!m_pLogMan->ActionDisk(pDiskStatus, diskInfoTmp.partitionNum, true))
				{
					bSucc = false;
					break;
				}
				if (!m_pReclogMan->ActionDisk(pDiskStatus, diskInfoTmp.partitionNum, true))
				{
					bSucc = false;
					break;
				}
				if (g_bHaveSnapPic)
				{
					if (!CSnapMan::GetInstance()->ActionDisk(pDiskStatus, diskInfoTmp.partitionNum, true))
					{
						printf("CSnapMan action Disk fail status = %d\n", true);
						bSucc = false;
						break;
					}
					if (!CSnapMan::GetInstance()->AddDisk(pDiskStatus, diskInfoTmp.partitionNum))
					{
						printf("CSnapMan add Disk fail\n");
						bSucc = false;
						break;
					}
				}
			}
			else
			{
				if (g_bHaveSnapPic)
				{

					if (!CSnapMan::GetInstance()->AddDisk(pDiskStatus, diskInfoTmp.partitionNum))
					{
						printf("CSnapMan add Disk fail\n");
						bSucc = false;
						break;
					}
				}
				if (!m_pLogMan->AddDisk(pDiskStatus, diskInfoTmp.partitionNum))
				{
					bSucc = false;
					break;
				}
				if (!m_pReclogMan->AddDisk(pDiskStatus, diskInfoTmp.partitionNum)) //格式化后的分区个数
				{
					bSucc = false;
					break;
				}
			}
			if (bStopRecord)
			{
				m_pRecMan->Action(true);
				m_pVideoLossMan->Action(true);
				m_pSensorInMan->Action(true);
				m_pMotionMan->Action(true);
				m_pOtherAlarm->Action(true);
#ifdef __MOBILE_DVR__
				m_pSpeedMan->Action(true);
#endif
			}
			if (bStopRead)
			{
				m_pNetReader->Action(true);
			}
			if (NULL != pDiskStatus)
			{
				delete [] pDiskStatus;
				pDiskStatus = NULL;
			}			
			diskInfoLstTmp.RemoveAll();			
		}

		if (NULL != pDiskStatus)
		{
			delete [] pDiskStatus;
			pDiskStatus = NULL;
		}
		m_bFormatDiskProc = false;

#ifndef NDEBUG
		//在格式化完所有的硬盘后，重新初始化CRunLog模块
		{	
			DISK_LOG_INFO *pPartitionStatus = NULL;
			int allMountedPartitionNum = 0;
			ULONG systemDiskStatus = 0;
			GetDiskManInfo(m_pDiskMan, &pPartitionStatus, allMountedPartitionNum, systemDiskStatus);
			if (!CRunLog::Instance()->Initial(pPartitionStatus, allMountedPartitionNum, m_pDiskPath))
			{
				printf("(%s,%d) inital Run log fail\n",__FILE__, __LINE__);				
			}
			else
			{
				printf("(%s,%d) initial Run log succ\n", __FILE__, __LINE__);
			}
			if (NULL != pPartitionStatus)
			{
				delete [] pPartitionStatus;
				pPartitionStatus = NULL;
			}
		}
#endif

	}

	assert(NULL == pDiskStatus);
	if (NULL != pDiskStatus)
	{
		delete [] pDiskStatus;
		pDiskStatus = NULL;
	}

	if (NULL != m_pDiskInfoForFormat)
	{
		delete [] m_pDiskInfoForFormat;
		m_pDiskInfoForFormat= NULL;
	}

	m_diskNumForFormat = 0;

	if (!bSucc)
	{
		ProcCallBackMsg(this, CALL_BACK_MODULE_DISK_MAN, CALL_BACK_MSG_FAIL, NULL, NULL);
	}
	else
	{
		ProcCallBackMsg(this, CALL_BACK_MODULE_DISK_MAN, CALL_BACK_MSG_FINISH, NULL, NULL);
	}
	
	return 0;

#endif

}


void CMainFrame::StopFormatDisk()
{
	//printf("%s,%d\n",__FILE__,__LINE__);
	if (PUB_THREAD_ID_NOINIT != m_formatDiskProcID)
	{
		PUB_ExitThread(&m_formatDiskProcID, &m_bFormatDiskProc);
	}

	if (NULL != m_pDiskInfoForFormat)
	{
		delete [] m_pDiskInfoForFormat;
		m_pDiskInfoForFormat= NULL;
	}

	m_diskNumForFormat = 0;


}





int CMainFrame::ChangeDiskProperty(DVR_DISK_INFO *pDiskInfo, int num)
{
#ifdef __ENVIRONMENT_LINUX__

	assert(NULL == m_pDiskInfoForChange);
	assert(NULL != pDiskInfo);
	assert(PUB_THREAD_ID_NOINIT == m_changeDiskProcID);

	if (NULL != m_pDiskInfoForChange)
	{
		delete [] m_pDiskInfoForChange;
		m_pDiskInfoForChange = NULL;
	}

	m_diskNumForChange = num;
	m_pDiskInfoForChange = new DVR_DISK_INFO [m_diskNumForChange];
	if (NULL == m_pDiskInfoForChange)
	{
		return -1;
	}
	memcpy(m_pDiskInfoForChange, pDiskInfo, m_diskNumForChange*sizeof(DVR_DISK_INFO));


	if (PUB_THREAD_ID_NOINIT != m_changeDiskProcID)
	{
		PUB_ExitThread(&m_changeDiskProcID, &m_bChangeDiskProc);
	}
	
#if 0
	m_changeDiskProcID = PUB_CreateThread(ChangeDiskThread, this, &m_bChangeDiskProc);
	if(m_changeDiskProcID == PUB_CREATE_THREAD_FAIL)
	{
		PUB_PrintError(__FILE__,__LINE__);
		delete [] m_pDiskInfoForChange;
		m_pDiskInfoForChange = NULL;
		m_diskNumForChange   = 0;
		return -1;
	}
#else
	int ret = ChangeDiskProperty();
	StopChangeDisk();
	return ret;
#endif
	


	return 0;
#endif

	return 0;

}


PUB_THREAD_RESULT PUB_THREAD_CALL CMainFrame::ChangeDiskThread(void *pParam)
{
#ifdef __ENVIRONMENT_LINUX__
	pid_t pid = getpid();
	pid_t tid = syscall(__NR_gettid);
	printf("%s, %d, pid = %d, tid=%d\n", __FILE__, __LINE__, pid, tid);
#endif

	CMainFrame *pThis = reinterpret_cast<CMainFrame *>(pParam);
	pThis->ChangeDiskProperty();

	return 0;
}


int CMainFrame::ChangeDiskProperty( )
{
#ifdef __ENVIRONMENT_LINUX__
	DISK_LOG_INFO *pDiskStatus = NULL;
	bool bSucc = true;

#ifndef NDEBUG
	//在修改硬盘属性之前退出CRunLog模块
	{
		CRunLog::Instance()->Quit();
		printf("%s,%d:CRunLog::Instance()->Quit()\n",__FILE__,__LINE__);
	}
#endif


	for (int index = 0; index < m_diskNumForChange; ++index)
	{    
		assert(NET_DISK_OWNED_BY_THIS == m_pDiskInfoForChange[index].diskOwnedBy);
		assert(NET_DISK_TYPE_FIXEDDISK == m_pDiskInfoForChange[index].diskType);
		assert(NET_DISK_NORMAL == m_pDiskInfoForChange[index].diskStatus);
		assert(m_pDiskInfoForChange[index].diskProperty != m_pDiskInfoForChange[index].disktemp);
		
		if ((NET_DISK_READ_WRITE == m_pDiskInfoForChange[index].diskProperty) || (NET_DISK_REDUNDANCY == m_pDiskInfoForChange[index].diskProperty))
		{
			assert(m_pDiskInfoForChange[index].partitionNum > 0);
			DVR_DISK_INFO_LIST diskInfoLstTmp;

			//准备 需要停录像
			m_pNetReader->Action(false);
			m_pMotionMan->Action(false);   
			m_pSensorInMan->Action(false);
			m_pVideoLossMan->Action(false);
			m_pOtherAlarm->Action(false);
#ifdef __MOBILE_DVR__
			m_pSpeedMan->Action(false);
#endif
			m_pRecMan->Action(false);                 				
			pDiskStatus = new DISK_LOG_INFO [m_pDiskInfoForChange[index].partitionNum];
			for (int reclogIndex = 0; reclogIndex < m_pDiskInfoForChange[index].partitionNum; ++reclogIndex)
			{
				pDiskStatus[reclogIndex].diskOwnedBy  = m_pDiskInfoForChange[index].diskOwnedBy;
				pDiskStatus[reclogIndex].diskIndex    = m_pDiskInfoForChange[index].diskpara + reclogIndex;
				pDiskStatus[reclogIndex].diskProperty = m_pDiskInfoForChange[index].diskProperty;
			}
			if (!m_pReclogMan->ActionDisk(pDiskStatus, m_pDiskInfoForChange[index].partitionNum, false))
			{
				bSucc = false;
				break;
			}

			if (g_bHaveSnapPic)
			{
				if (!CSnapMan::GetInstance()->ActionDisk(pDiskStatus, m_pDiskInfoForChange[index].partitionNum, false))
				{
					printf("CSnapMan action Disk fail status = %d\n", false);
					bSucc = false;
					break;
				}
			}

			if (!m_pLogMan->ActionDisk(pDiskStatus, m_pDiskInfoForChange[index].partitionNum, false))
			{
				bSucc = false;
				break;
			}
			if (NULL != pDiskStatus)
			{
				delete [] pDiskStatus;
				pDiskStatus = NULL;
			}

			//开始
			if (m_pDiskMan->ChangeDiskProperty((DISK_TYPE)m_pDiskInfoForChange[index].diskType, m_pDiskInfoForChange[index].diskname, m_pDiskInfoForChange[index].disktemp) < 0)
			{
				bSucc = false;
				break;
			}

			//结束
			{
				if (PRERECORD_MODE_DISK == m_preRecordType)
				{
					unsigned char prerecordPartIndex = 0;
					unsigned char prerecordPartNum = 0;
					m_preRecordLMask    = 0;
					m_preRecordHMask    = 0;
					m_pDiskMan->GetOneRWDiskPartionNum(prerecordPartIndex, prerecordPartNum);
					m_pRecMan->ResetPreDiskRecordPath(prerecordPartIndex, prerecordPartNum, m_preRecordHMask, m_preRecordLMask);
				}
			}
			m_pDiskMan->GetDeviceDiskInfo((DISK_TYPE)m_pDiskInfoForChange[index].diskType, m_pDiskInfoForChange[index].diskname, diskInfoLstTmp);
			DVR_DISK_INFO &diskInfoTmp = diskInfoLstTmp.GetHead();
			pDiskStatus = new DISK_LOG_INFO [diskInfoTmp.partitionNum];
			for (int reclogIndex = 0; reclogIndex < diskInfoTmp.partitionNum; ++reclogIndex)
			{
				pDiskStatus[reclogIndex].diskOwnedBy  = diskInfoTmp.diskOwnedBy;
				pDiskStatus[reclogIndex].diskIndex    = diskInfoTmp.diskpara + reclogIndex;
				pDiskStatus[reclogIndex].diskProperty = diskInfoTmp.diskProperty;
			}
			if (!m_pLogMan->ActionDisk(pDiskStatus, diskInfoTmp.partitionNum, true))
			{
				bSucc = false;
				break;
			}
			if (!m_pReclogMan->ActionDisk(pDiskStatus, diskInfoTmp.partitionNum, true))
			{
				bSucc = false;
				break;
			}
			if (g_bHaveSnapPic)
			{
				if (!CSnapMan::GetInstance()->ActionDisk(pDiskStatus, diskInfoTmp.partitionNum, true))
				{
					printf("CSnapMan action Disk fail status = %d\n", true);
					bSucc = false;
					break;
				}
			}
			m_pRecMan->Action(true);
			m_pVideoLossMan->Action(true);
			m_pSensorInMan->Action(true);
			m_pMotionMan->Action(true);
			m_pOtherAlarm->Action(true);
#ifdef __MOBILE_DVR__
			m_pSpeedMan->Action(true);
#endif
			m_pNetReader->Action(true);
			if (NULL != pDiskStatus)
			{
				delete [] pDiskStatus;
				pDiskStatus = NULL;
			}
			diskInfoLstTmp.RemoveAll();

		}
		else if (NET_DISK_ONLY_READ == m_pDiskInfoForChange[index].diskProperty) 
		{
			DVR_DISK_INFO_LIST diskInfoLstTmp;

			//准备  不需要停录像
			m_pNetReader->Action(false);
			pDiskStatus = new DISK_LOG_INFO [m_pDiskInfoForChange[index].partitionNum];
			for (int reclogIndex = 0; reclogIndex < m_pDiskInfoForChange[index].partitionNum; ++reclogIndex)
			{ 
				pDiskStatus[reclogIndex].diskOwnedBy  = m_pDiskInfoForChange[index].diskOwnedBy;
				pDiskStatus[reclogIndex].diskIndex    = m_pDiskInfoForChange[index].diskpara + reclogIndex;
				pDiskStatus[reclogIndex].diskProperty = m_pDiskInfoForChange[index].diskProperty;
			}
			if (!m_pLogMan->ActionDisk(pDiskStatus, m_pDiskInfoForChange[index].partitionNum, false))
			{
				bSucc = false;
				break;
			}
			if (!m_pReclogMan->ActionDisk(pDiskStatus, m_pDiskInfoForChange[index].partitionNum, false))
			{
				bSucc = false;
				break;
			}
			if (g_bHaveSnapPic)
			{
				if (!CSnapMan::GetInstance()->ActionDisk(pDiskStatus, m_pDiskInfoForChange[index].partitionNum, false))
				{
					printf("CSnapMan action Disk fail status = %d\n", false);
					bSucc = false;
					break;
				}
			}
			if (NULL != pDiskStatus)
			{
				delete [] pDiskStatus;
				pDiskStatus = NULL;
			}

			//开始
			if (m_pDiskMan->ChangeDiskProperty((DISK_TYPE)m_pDiskInfoForChange[index].diskType, m_pDiskInfoForChange[index].diskname, m_pDiskInfoForChange[index].disktemp) < 0)
			{
				bSucc = false;
				break;
			}

			//结束
			{
				if (PRERECORD_MODE_DISK == m_preRecordType)
				{
					unsigned char prerecordPartIndex = 0;
					unsigned char prerecordPartNum = 0;
					m_preRecordLMask    = 0;
					m_preRecordHMask    = 0;
					m_pDiskMan->GetOneRWDiskPartionNum(prerecordPartIndex, prerecordPartNum);
					m_pRecMan->ResetPreDiskRecordPath(prerecordPartIndex, prerecordPartNum, m_preRecordHMask, m_preRecordLMask);
				}
			}
			m_pDiskMan->GetDeviceDiskInfo((DISK_TYPE)m_pDiskInfoForChange[index].diskType, m_pDiskInfoForChange[index].diskname, diskInfoLstTmp);
			DVR_DISK_INFO &diskInfoTmp = diskInfoLstTmp.GetHead();
			pDiskStatus = new DISK_LOG_INFO [diskInfoTmp.partitionNum];
			for (int reclogIndex = 0; reclogIndex < diskInfoTmp.partitionNum; ++reclogIndex)
			{
				pDiskStatus[reclogIndex].diskOwnedBy  = diskInfoTmp.diskOwnedBy;
				pDiskStatus[reclogIndex].diskIndex    = diskInfoTmp.diskpara + reclogIndex;
				pDiskStatus[reclogIndex].diskProperty = diskInfoTmp.diskProperty;
			}
			if (!m_pLogMan->ActionDisk(pDiskStatus, diskInfoTmp.partitionNum, true))
			{
				bSucc = false;
				break;
			}
			if (!m_pReclogMan->ActionDisk(pDiskStatus, diskInfoTmp.partitionNum, true))
			{
				bSucc = false;
				break;
			}

			if (g_bHaveSnapPic)
			{
				if (!CSnapMan::GetInstance()->ActionDisk(pDiskStatus, diskInfoTmp.partitionNum, true))
				{
					printf("CSnapMan action Disk fail status = %d\n", true);
					bSucc = false;
					break;
				}
			}
			m_pNetReader->Action(true);
			if (NULL != pDiskStatus)
			{
				delete [] pDiskStatus;
				pDiskStatus = NULL;
			}
			diskInfoLstTmp.RemoveAll();

		}
		else
		{
			assert(false);
		}
	}
#ifndef NDEBUG
	//在修改完所有硬盘的属性后，重新初始化CRunLog模块
	{	
		DISK_LOG_INFO *pPartitionStatus = NULL;
		int allMountedPartitionNum = 0;
		ULONG systemDiskStatus = 0;
		GetDiskManInfo(m_pDiskMan, &pPartitionStatus, allMountedPartitionNum, systemDiskStatus);
		if (!CRunLog::Instance()->Initial(pPartitionStatus, allMountedPartitionNum, m_pDiskPath))
		{
			printf("(%s,%d) inital Run log fail\n",__FILE__, __LINE__);				
		}
		else
		{
			printf("(%s,%d) initial Run log succ\n", __FILE__, __LINE__);
		}
		if (NULL != pPartitionStatus)
		{
			delete [] pPartitionStatus;
			pPartitionStatus = NULL;
		}
	}
#endif

	if (NULL != pDiskStatus)
	{
		delete [] pDiskStatus;
		pDiskStatus = NULL;
	}

	if (!bSucc)
	{
		return -1;
	}

#endif
	return 0;
	
}


void CMainFrame::StopChangeDisk()
{
	if (PUB_THREAD_ID_NOINIT != m_changeDiskProcID)
	{
		PUB_ExitThread(&m_changeDiskProcID, &m_bChangeDiskProc);
	}

	if (NULL != m_pDiskInfoForChange)
	{
		delete [] m_pDiskInfoForChange;
		m_pDiskInfoForChange= NULL;
	}

	m_diskNumForChange = 0;
}

//////////////////////////////////////////////////////////////////////////////
int CMainFrame::ProcCallBackMsg(void* pObject, int module, int iMsg, void *pParam1, void *pParam2)
{
	CMainFrame * pPlayBack = (CMainFrame*)pObject;
	return pPlayBack->ProcCallBackMsg(module, iMsg, pParam1, pParam1);
}

int CMainFrame::ProcCallBackMsg(int module, int iMsg, void *pParam1, void *pParam2)
{
	if (CALL_BACK_MODULE_DISK_MAN == module) // 磁盘管理模块
	{
		if (CALL_BACK_MSG_PROGRESS == iMsg) //进度
		{
			m_pMsgMan->RefreshFormatProgress(m_diskNumForFormat, m_curDiskNumForFormat, *((int *)pParam1));
		}
		else if(CALL_BACK_MSG_FINISH == iMsg) //结束
		{
			CMessageMan::Instance()->PostMsgDataToServer(0, LOCAL_DEVICE_ID, CMD_REQUEST_DISK_FORMAT_STOP, NULL, 0);
		}
		else if (CALL_BACK_MSG_FAIL == iMsg) //失败
		{
			m_pMsgMan->RefreshFormatProgress(m_diskNumForFormat, m_curDiskNumForFormat,101);
			CMessageMan::Instance()->PostMsgDataToServer(0, LOCAL_DEVICE_ID, CMD_REQUEST_DISK_FORMAT_STOP, NULL, 0);
		}
	}
	else if (CALL_BACK_MODULE_BACKUP_MAN == module)
	{
		if (CALL_BACK_MSG_PROGRESS == iMsg)
		{
			if (*((int *)pParam1) < 100)
			{
				m_pMsgMan->RefreshBackupProgress(*((int *)pParam1));
			}			
		}
		else if (CALL_BACK_MSG_FINISH == iMsg)
		{
			m_pMsgMan->RefreshBackupProgress(100);
		}
		else if (CALL_BACK_MSG_FAIL == iMsg)
		{
			m_pMsgMan->RefreshBackupProgress(101);
		}
	}
	else
	{
		assert(false);
	}

	return 0;
}

void CMainFrame::WriteLog(unsigned long clientID, LOG_TYPE type)
{
	OPERATOR_LOG		operatorLog;
	memset(&operatorLog, 0, sizeof(OPERATOR_LOG));
	if ((LOG_TYPE_BOOT == type) || (LOG_TYPE_SHUTDOWN == type) || ((LOG_TYPE_ERROR_INFO < type) && (type <= LOG_TYPE_UNKNOWN_ERR)))
	{
		operatorLog.IP		= IP_ADDRESS(127, 0, 0, 1);
		strcpy(operatorLog.name, "system");
		operatorLog.contentID = GetContentID(type);
		operatorLog.type = (type & 0xffff);
		operatorLog.time = GetCurrTime32();
		m_pLogMan->WriteOperatorLog(operatorLog);
	}
	else if (LOG_TYPE_ERR_9A9A9A9A == type)
	{
		type = LOG_TYPE_UNKNOWN_ERR;
		operatorLog.IP		= IP_ADDRESS(127, 0, 0, 1);
		strcpy(operatorLog.name, "system:9a9a");
		operatorLog.contentID = GetContentID(type);
		operatorLog.type = (type & 0xffff);
		operatorLog.time = GetCurrTime32();
		m_pLogMan->WriteOperatorLog(operatorLog);

	}
	else if (LOG_TYPE_ERR_9A000001 == type)
	{
		type = LOG_TYPE_UNKNOWN_ERR;
		operatorLog.IP		= IP_ADDRESS(127, 0, 0, 1);
		strcpy(operatorLog.name, "system:9a01");
		operatorLog.contentID = GetContentID(type);
		operatorLog.type = (type & 0xffff);
		operatorLog.time = GetCurrTime32();
		m_pLogMan->WriteOperatorLog(operatorLog);
	}
	else if (LOG_TYPE_ERR_9A000002 == type)
	{
		type = LOG_TYPE_UNKNOWN_ERR;
		operatorLog.IP		= IP_ADDRESS(127, 0, 0, 1);
		strcpy(operatorLog.name, "system:9a02");
		operatorLog.contentID = GetContentID(type);
		operatorLog.type = (type & 0xffff);
		operatorLog.time = GetCurrTime32();
		m_pLogMan->WriteOperatorLog(operatorLog);
	}
	////zxx新加，硬盘掉线日志
	else if(LOG_TYE_DISK_DISCONNECT == type)
	{
		operatorLog.IP		= IP_ADDRESS(127, 0, 0, 1);
		strcpy(operatorLog.name, "system");
		operatorLog.contentID = GetContentID(type);
		operatorLog.type = (type & 0xffff);
		operatorLog.time = GetCurrTime32();
#if defined(__SUPPORT_SNAPD1__)
		char tip[256]="GB2312:硬盘故障";
		if(NULL != m_pShanghaiConnectMan)
		{
			CShanghaiConnectMan::Instance()->GetLogCmd(DVRDISKERR,0,tip,strlen(tip),NULL,0,GetCurrTime());
		}
		
#endif
		if(!m_pLogMan->WriteOperatorLog(operatorLog))
		{
			printf("write log err 6709\n");
		}

	}
	else if(LOG_TYE_DISK_FULL == type)
	{
		operatorLog.IP		= IP_ADDRESS(127, 0, 0, 1);
		strcpy(operatorLog.name, "system");
		operatorLog.contentID = GetContentID(type);
		operatorLog.type = (type & 0xffff);
		operatorLog.time = GetCurrTime32();
#if defined(__SUPPORT_SNAPD1__)
		char tip[256]="GB2312:硬盘故障";
		if(NULL != m_pShanghaiConnectMan)
		{
			CShanghaiConnectMan::Instance()->GetLogCmd(DVRDISKERR,0,tip,strlen(tip),NULL,0,GetCurrTime());
		}
#endif
		if(!m_pLogMan->WriteOperatorLog(operatorLog))
		{
			printf("write log err 6709\n");
		}
	}
	else
	{
		USER_INFO user;
		if (m_pUserMan->GetUser(user, clientID))
		{
			operatorLog.IP		= user.IP;
			strcpy(operatorLog.name, user.name);

			operatorLog.contentID = GetContentID(type);
			operatorLog.type = (type & 0xffff);

			operatorLog.time = GetCurrTime32();

			m_pLogMan->WriteOperatorLog(operatorLog);
		}
	}
}

#ifdef __ENVIRONMENT_LINUX__
void CMainFrame::SetDiskManInitInfo(DISK_INIT_INFO &diskInitInfo)
{
	diskInitInfo.videoInputNum = m_videoInputNum;
	diskInitInfo.bHaveEsata = m_pProduct->SupportESATA();
	if (diskInitInfo.bHaveEsata)
	{
		diskInitInfo.bHaveEsata = m_pProduct->GetUseEsata();
	}
	diskInitInfo.nEsataInterface = m_pProduct->GetESATAIndex();
	diskInitInfo.nPciToEsataNum = m_pProduct->GetPciToEsataNum();
	memset(diskInitInfo.pciToEsataIndex, 0, sizeof(diskInitInfo.pciToEsataIndex));
	for (int pciCount = 0; pciCount < diskInitInfo.nPciToEsataNum; ++pciCount)
	{
		diskInitInfo.pciToEsataIndex[pciCount] = m_pProduct->GetPciToEsataIndex(pciCount);
	}
	memset(diskInitInfo.szUsbToEsataPath, 0, sizeof(diskInitInfo.szUsbToEsataPath));
	if (m_pProduct->SupportUsbToEsata())
	{
		m_pProduct->GetUsbToEsataPath(diskInitInfo.szUsbToEsataPath);
	}



	//获取DVR的mac地址，这里不应该使用下面的方法获取，应该采用类CNetInterface
	//中的GetMACaddress函数。但是CNetInterface还没有。等有了类CNetInterface后，
	//在修改这里。091108
	memset(diskInitInfo.dvrId, 0, 8);
	int fd = socket (AF_INET, SOCK_DGRAM, 0);
	if (fd >= 0)
	{
		struct ifreq  ifr; 
		memset(&ifr, 0, sizeof(ifr));
		strcpy(ifr.ifr_name, "eth0" );
		if (!(ioctl (fd, SIOCGIFHWADDR, (char *)&ifr)) )
		{
			sockaddr *pSocketAddress = &ifr.ifr_hwaddr;
			char* macAddressTemp = (char*)(pSocketAddress->sa_data);
			memcpy(diskInitInfo.dvrId,  macAddressTemp, 6);
			//printf("%02x-%02x-%02x-%02x-%02x-%02x",diskInitInfo.dvrId[0],diskInitInfo.dvrId[1],diskInitInfo.dvrId[2],diskInitInfo.dvrId[3],diskInitInfo.dvrId[4],diskInitInfo.dvrId[5]);
		}		
		close(fd);
	}
	else
	{
		memset(diskInitInfo.dvrId, '0xa', 6);
	}

	diskInitInfo.fileSizeM = MAX_RECORD_FILE_LEN_M;
	
	diskInitInfo.partitionNum = 4;
	diskInitInfo.productId = CProduct::Instance()->ProductType();
	strcpy(diskInitInfo.szDiskMountPath, m_pDiskPath);

}

void CMainFrame::GetDiskManInfo(CDiskManager *pDiskMan, DISK_LOG_INFO **pPartitionInfo, int &partionNum, ULONG &diskStatus)
{
	int partitionIndex = 0;
	int allMountedPartitionNum = 0;
	int allReadWritePartitionNum = 0;
	int allReadOnlyPartitionNum = 0;
	int allRedundancyPartitionNum = 0;
	int allUnFormatedDiskNum = 0;
	DVR_DISK_INFO_LIST partitionInfoList;
	DISK_LOG_INFO *pPartitionInfoTmp = NULL;

	pDiskMan->GetDevicePartionInfo(NET_DISK_TYPE_FIXEDDISK, NULL, partitionInfoList);
	POS pos = partitionInfoList.GetHeadPosition();
	while (NULL != pos)
	{
		DVR_DISK_INFO &info = partitionInfoList.GetAt(pos);

		if (NET_DISK_NORMAL == info.diskStatus)
		{
			//统计所有挂载成功的分区数
			allMountedPartitionNum++;

			if (NET_DISK_OWNED_BY_THIS == info.diskOwnedBy)
			{
				if (NET_DISK_READ_WRITE == info.diskProperty)
				{
					allReadWritePartitionNum++;
				}
				else if (NET_DISK_ONLY_READ == info.diskProperty)
				{
					allReadOnlyPartitionNum++;
				}
				else if (NET_DISK_REDUNDANCY == info.diskProperty)
				{
					allRedundancyPartitionNum++;					
				}
				else 
				{
					assert(false);
				}
			}
			else
			{
				assert(NET_DISK_ONLY_READ == info.diskProperty);
				allReadOnlyPartitionNum++;
			}
		}
		else if (NET_DISK_NO_FORMAT == info.diskStatus)
		{
			allUnFormatedDiskNum++;
		}
		else
		{
			assert(false);
		}
		partitionInfoList.GetNext(pos);
	}

	//设置系统硬盘状态
	diskStatus = 0;
	if (allReadWritePartitionNum >0)
	{
		diskStatus |= NET_DISK_HAVE_READEWRITE_DISK; 
	}
	if (allRedundancyPartitionNum > 0)
	{
		diskStatus |= NET_DISK_HAVE_REDUN_DISK; 
	}
	if (allReadOnlyPartitionNum > 0)
	{
		diskStatus |= NET_DISK_HAVE_ONLYREADE_DISK;
	}
	if (allUnFormatedDiskNum > 0)
	{
		diskStatus |= NET_DISK_HAVE_UNFORMATED_DISK;
	}


	if (allMountedPartitionNum > 0)
	{
		partitionIndex = 0;
		pPartitionInfoTmp = new DISK_LOG_INFO [allMountedPartitionNum];
		pos = partitionInfoList.GetHeadPosition();
		while (NULL != pos)
		{
			DVR_DISK_INFO &info = partitionInfoList.GetAt(pos);
			if (NET_DISK_NORMAL == info.diskStatus)
			{
				pPartitionInfoTmp[partitionIndex].diskOwnedBy = info.diskOwnedBy;
				pPartitionInfoTmp[partitionIndex].diskIndex = info.diskpara;
				pPartitionInfoTmp[partitionIndex].diskProperty = info.diskProperty;
				partitionIndex++;				
			}
			else if (NET_DISK_NO_FORMAT == info.diskStatus)
			{

			}
			else
			{
				assert(false);
			}
			partitionInfoList.GetNext(pos);
		}
	}

	partionNum = allMountedPartitionNum;
	(*pPartitionInfo) = pPartitionInfoTmp;

	partitionInfoList.RemoveAll();

}
#endif

bool CMainFrame::IsSetTzoneConfig(const unsigned char *pDataBlock, unsigned long dataLen)
{
	assert(pDataBlock != NULL);
	assert(dataLen > 0);
     
	bool bRet = false;
	long itemHeadLen = sizeof(NCFG_ITEM_HEAD);
	const NCFG_BLOCK_HEAD *pBlockHead = reinterpret_cast<const NCFG_BLOCK_HEAD*>(pDataBlock);
	int itemNum = (int)pBlockHead->ItemNum; 
	long readPos = sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD)*itemNum;
	const NCFG_ITEM_HEAD *pItemHead = NULL;

	for (int i =0; i<itemNum; i++)
	{
		pItemHead = reinterpret_cast<const NCFG_ITEM_HEAD*>(pDataBlock + i*itemHeadLen + sizeof(NCFG_BLOCK_HEAD));
		if (NCFG_ITEM_TIME_ZONE == pItemHead->itemID)
		{
			bRet = true;
			break;
		}
	}
	return bRet;
}


PUB_THREAD_RESULT PUB_THREAD_CALL CMainFrame::FeedDogThread(void *pParam)
{
#ifdef __ENVIRONMENT_LINUX__
	pid_t pid = getpid();
	pid_t tid = syscall(__NR_gettid);
	printf("%s, %d, feed dog pid = %d, tid=%d\n", __FILE__, __LINE__, pid, tid);
#endif

	CMainFrame *pThis = reinterpret_cast<CMainFrame *>(pParam);
	pThis->FeedDogProc();

	return 0;
}


int CMainFrame::FeedDogProc()
{
	//下面的喂狗方法有问题：比如通过修改时间的方法，使时间不变，就会不问狗
	/*bool bOk = false;
	LONGLONG curTime = 0;
	LONGLONG lastFeedDog = 0;
	while (m_bFeedDogProc)
	{
#ifdef __ENVIRONMENT_WIN32__
#else
		curTime = GetCurrTime();
		if (((curTime - lastFeedDog) > static_cast<LONGLONG>(29000000)) || (curTime < lastFeedDog) || (!bOk))
		{
			lastFeedDog = curTime;
#ifdef __ENVIRONMENT_LINUX__
			bOk = m_pLocalDeviceMan->FeedDog();
#endif
		}
#endif
		PUB_Sleep(1000);
	}*/

	bool bLcdScreemOn = false;

	int decodeError = 0;
	bool bOk = false;
	int count = 0;	
	PRODUCT_TYPE productID = CProduct::Instance()->ProductType();

	while (m_bFeedDogProc)
	{
		//磁盘掉线，停止喂狗,重启机器
		if (m_bHaveDiskLost)
		{
			PUB_Sleep(1000);
			continue;
		}

#ifdef __ENVIRONMENT_WIN32__

#else

#ifdef __CHIP3515__
		if ((count > 9) || (!bOk))
#elif defined(__CHIPTI__)
		if ((count > 5) || (!bOk))
#else
		if ((count > 9) || (!bOk))
#endif
		{
			bOk = m_pLocalDeviceMan->FeedDog() && m_pLocalDeviceMan->FeedDog();

			decodeError = m_pLocalDeviceMan->CheckDecodeError();
			if (0x9a9a9a9a == decodeError)
			{
				printf("system wait reboot\n");
				WriteLog(LOCAL_CLIENT_ID, LOG_TYPE_ERR_9A9A9A9A);
				break;
			}
			else if (0x9b9b9b9b == decodeError)
			{
				printf("system wait reboot\n");
				WriteTimeErrorLog(decodeError);
				break;
			}
			else if (0x9b9b9b9c == decodeError)
			{
				printf("system wait reboot\n");
				WriteTimeErrorLog(decodeError);
				break;
			}
			else if (0x9b9b9b9d == decodeError)
			{
				printf("system wait reboot\n");
				WriteTimeErrorLog(decodeError);
				break;
			}
			else if (0x9a000001 == decodeError)
			{
				WriteLog(LOCAL_CLIENT_ID, LOG_TYPE_ERR_9A000001);
			}
			else if (0x9a000002 == decodeError)
			{
				WriteLog(LOCAL_CLIENT_ID, LOG_TYPE_ERR_9A000002);

				//网络出问题，恢复网络接口配置相关信息
				CAllocNICIP::Instance()->RecoverNetIic("eth0");
			}
			else
			{
				//CAllocNICIP::Instance()->RecoverNetIic("eth0");
			}

			count = 0;
		}
		else
		{
			count++;
			if (g_bLcdScreemOn != bLcdScreemOn)
			{
				CLocalDevice::Instance()->SetLcdScreemStatus(g_bLcdScreemOn);
				printf("##############%s,%d, SetLcdScreemStatus:%d\n",__FILE__,__LINE__,g_bLcdScreemOn);
				bLcdScreemOn = g_bLcdScreemOn;
			}
		}		
#endif
		PUB_Sleep(1000);
	}


	return 0;
}

PUB_THREAD_RESULT PUB_THREAD_CALL CMainFrame::TimerThread(void *pParam)
{
#ifdef __ENVIRONMENT_LINUX__
	pid_t pid = getpid();
	pid_t tid = syscall(__NR_gettid);
	printf("%s, %d, timer pid = %d, tid = %d\n", __FILE__, __LINE__, pid, tid);
#endif

	CMainFrame *pThis = reinterpret_cast<CMainFrame *>(pParam);
	pThis->TimerProc();

	return 0;
}


int CMainFrame::TimerProc()
{
#ifndef NDEBUG
	unsigned char day = 0;
#endif

#if defined(__TW01_RILI__)
	static bool bAdjTime = false;
	unsigned long waitAdjTimeOk = 0;
	struct tm curTmTime;
#endif

	while (m_bTimerProc)
	{
		m_showTime = GetCurrTime();
			
#ifdef __ENVIRONMENT_LINUX__

#if defined(__CHIP3515__) || defined(__TDFH__)
		if (((m_showTime - m_lastShowTime) > static_cast<LONGLONG>(500000)) || (m_showTime < m_lastShowTime))
#else
		if (((m_showTime - m_lastShowTime) > static_cast<LONGLONG>(250000)) || (m_showTime < m_lastShowTime))
#endif
		{
			m_lastShowTime = m_showTime;

			GUI::CTimeCtrl::GetTime32Text(m_strTime, static_cast<unsigned long>(m_showTime/1000000));
			m_pLocalDeviceMan->RefreshTimeStamp(m_strTime);

			//修改录像状态等
			if (m_bRecording != m_pMsgMan->Recording())
			{
				m_bRecording = m_pMsgMan->Recording();

				m_pLocalDeviceMan->SetLEDStatus(LED_STATUS_RECORD, m_bRecording);
			}
		}

		//RTC时间同步
		if (((m_showTime - m_lastSyncRTCTime) > (static_cast<LONGLONG>(86400)*1000000)) || (m_showTime < m_lastSyncRTCTime))
		{
			m_lastSyncRTCTime = m_showTime;

			m_pLocalDeviceMan->SetSystemTime(static_cast<unsigned long>(m_showTime/1000000), false);

#ifndef NDEBUG
			++ day;

			//非发布版本，允许15天后将系统关机
			if (15 < day)
			{
				printf("Test version,I will Shut Down\n");
				g_system_status = DVR_SYSTEM_EXIT;
			}
#endif
		}

		PUB_Sleep(200);
#else
		PUB_Sleep(1000);
#endif

//#if defined(__TW01_RILI__)
#if 0
		if (true == bAdjTime)
		{
			if (waitAdjTimeOk < 2000)
			{
				waitAdjTimeOk ++;
			}
			else
			{
				bAdjTime = false;
			}
		}
		else
		{
			curTmTime = DVRTimeToTm(m_showTime);

			if ((0 == curTmTime.tm_wday) && (0 == curTmTime.tm_hour) && (0 == curTmTime.tm_min))
			{
				//如果进入星期天就调整一次时间
				//向后调整设备时间
				//经测试，7天会快5s左右
				unsigned long adjTime = 5 * (7 - m_lastAdjTmTime.tm_wday % 7) / 7;
				adjTime %= 6;

				unsigned long time = ((m_showTime / 1000000) - adjTime);

				printf("%s:%s:%d, 开始调整时间\n", __FUNCTION__, __FILE__, __LINE__);
				CMessageMan::Instance()->PostMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_L_CHANGE_TIME, (unsigned char *)&time, sizeof(unsigned long));
				printf("%s:%s:%d, 调整时间结束\n", __FUNCTION__, __FILE__, __LINE__);

				bAdjTime = true;			//设置为已经调整，防止重复调整
				waitAdjTimeOk = 0;
			}
		}
#endif
	}

	return 0;
}

PUB_THREAD_RESULT PUB_THREAD_CALL CMainFrame::CheckVIThread(void *pParam)
{
#ifdef __ENVIRONMENT_LINUX__
	pid_t pid = getpid();
	pid_t tid = syscall(__NR_gettid);
	printf("%s, %d, check vi pid = %d, tid=%d\n", __FILE__, __LINE__, pid, tid);
#endif

	CMainFrame *pThis = reinterpret_cast<CMainFrame *>(pParam);
	pThis->CheckVIProc();

	return 0;
}


int CMainFrame::CheckVIProc()
{
	PRODUCT_TYPE productID = CProduct::Instance()->ProductType();

	while (m_bCheckVIProc)
	{
#if defined(__CHIP3520A__) || defined(__CHIP3520D__) || defined(__CHIP3521__) || defined(__CHIPGM__) || (defined(__CHIP3531__) && defined(__HISI_SDK_0BX__))
		if ((TD_2704TS_PL == productID) || (TD_2708TS_PL == productID) || (TD_2716TS_PL == productID) || (TD_2716AS_SL == productID) || (TD_2716TS_S == productID) || (TD_2704AS_S == productID) || (TD_2708AS_S == productID) || (TD_2708AS_S_A == productID) || (TD_2704AS_SL == productID) || (TD_2704TD_P == productID) || (TD_2708TD_P == productID) || (TD_2716TD_P == productID) || (TD_2704TD_PL == productID) || (TD_2708TD_PL == productID) || (TD_2716TD_PL == productID) || (TD_2732TD_PL == productID)
			|| (TD_2704TE_M == productID) || (TD_2708TE_M == productID) || (TD_2716TE_M == productID)
			|| (TD_2704AE_SL == productID) || (TD_2708AE_SL == productID) || (TD_2716AE_SL == productID)
			|| (TD_2704AS_SL == productID) || (TD_2704AS_SL_A == productID) || (TD_2708AS_SL == productID) || (TD_2716AS_SL == productID)
			|| (TD_2704AS_SL_A == productID) || (TD_2708AS_SL_A == productID) || (TD_2716AS_SL_A == productID)
			|| (TD_2004AL == productID) || (TD_2008AS == productID)
			|| (TD_2704AS_S_A == productID) || (TD_2708AS_S_A == productID)
			|| (TD_2704AS_PL == productID) || (TD_2708AS_PL == productID) || (TD_2716AS_PL == productID)
			|| (TD_2716AC_PL == productID)
			|| (TD_2704AE_PL == productID) || (TD_2708AE_PL == productID) || (TD_2716AE_PL == productID)
			)
		{
			if (m_bCanCheckVI)
			{
				CheckVI();
			}
		}
#endif
		PUB_Sleep(100);
	}

	return 0;
}

//磁盘检测
PUB_THREAD_RESULT PUB_THREAD_CALL CMainFrame::DiskDetectThread(void *pParam)
{
#ifdef __ENVIRONMENT_LINUX__
	pid_t pid = getpid();
	pid_t tid = syscall(__NR_gettid);
	printf("%s, %d, pid = %d, tid=%d\n", __FILE__, __LINE__, pid, tid);
#endif

	CMainFrame *pThis = reinterpret_cast<CMainFrame *>(pParam);
	pThis->DiskDetectProc();

	return 0;
}

int CMainFrame::DiskDetectProc()
{
	ULONGLONG diskMask[2] = {0, 0};		
	unsigned long freeFileNum = 0;
	bool bHaveWorkDisk = false;
	DVR_DISK_INFO_LIST dvrDiskInfoList;
	bool bIsAlarm = false;
	unsigned long diskSleepCnt = 0, diskDelFileCnt = 0, diskFullCnt = 0, diskHealthCnt = 0;
	unsigned long diskcheck=0;//先写在外面，感觉可以利用diskFullCnt
	bool bCheckAlarm = false;
	int num=0;
	bool bHaveDiskLost=false;//硬盘掉线蜂鸣器开关

	unsigned long timeTmp=0;
	unsigned long preTime=0;
	while (m_bDiskDetectProc)
	{
#ifdef __ENVIRONMENT_LINUX__

		//磁盘休眠
		{
			if (diskSleepCnt > 600)
			{
				diskSleepCnt = 0;
				diskMask[0] = 0;
				diskMask[1] = 0;
				m_pReclogMan->GetUsingPartitionMask(diskMask[1], diskMask[0]);
				m_pLogMan->GetUsingPartitionMask(diskMask[1], diskMask[0]);	

				if (PRERECORD_MODE_DISK == m_preRecordType)
				{
					diskMask[1] |= m_preRecordHMask;
					diskMask[0] |= m_preRecordLMask;
				}
				if (g_bHaveSnapPic)
				{
					ULONGLONG SnapDiskHMask = 0;
					ULONGLONG SnapDiskLMask = 0;
					if (m_pSnapMan->GetUsingPartitionMask(SnapDiskHMask, SnapDiskLMask))
					{
						diskMask[1] |= SnapDiskHMask;
						diskMask[0] |= SnapDiskLMask;
					}				
				}

#ifndef NDEBUG
				CRunLog::Instance()->GetUsingPartitionMask(diskMask[1], diskMask[0]);		
#endif
				if ((!m_bOperForFormating) && (!m_bOperForLocalBackuping))
				{
					m_pDiskMan->ProcessDiskSleep(diskMask[1], diskMask[0]);
				}
			}
			else
			{
				++ diskSleepCnt;
			}
		}

		//删除过期数据
		{
			if (diskDelFileCnt > 840)
			{
				diskDelFileCnt = 0;
				unsigned long ltime = GetZeroTime32OfDate(GetCurrTime32());
				if (m_todayTimeL != ltime)
				{
					if ((!m_bOperForFormating) && (!m_bOperForLocalBackuping))
					{
						m_pReclogMan->DeleteOverdueFiles();
						m_todayTimeL = ltime;
						printf("Delete data succ\n");
					}
					else
					{
						printf("Delete data failed\n");
					}
				}
			}
			else
			{
				++ diskDelFileCnt;
			}
		}

		//处理磁盘满报警
		{
			if (diskFullCnt > 240)
			{
				diskFullCnt = 0;
				freeFileNum = 0;
				bHaveWorkDisk = false;
				bIsAlarm = false;

				if (!m_bOperForFormating)
				{
					dvrDiskInfoList.RemoveAll();

					//查找录像硬盘
					m_pDiskMan->GetDeviceDiskInfo(NET_DISK_TYPE_FIXEDDISK, NULL, dvrDiskInfoList);

					//计算录像硬盘的剩余空间		
					POS pos = dvrDiskInfoList.GetHeadPosition();
					while (NULL != pos)
					{
						DVR_DISK_INFO &diskInfo = dvrDiskInfoList.GetAt(pos);
						if (NET_DISK_NORMAL == diskInfo.diskStatus)
						{
							bHaveWorkDisk = m_pReclogMan->GetWorkDiskFreeFileNum(diskInfo.diskpara, diskInfo.partitionNum, freeFileNum);				
						}
						dvrDiskInfoList.GetNext(pos);
					}

					if (bHaveWorkDisk > 0)
					{
						bIsAlarm = m_pReclogMan->CheckDiskAlarm(freeFileNum);
					}
					else
					{
						bIsAlarm = false;
					}

					m_pMsgMan->SetShowDiskAlarm(bIsAlarm);
#ifdef __CUSTOM_YUANZHENG__
					if(bIsAlarm)
					{
						CYuanZhengMan::Instance()->SetYZAlarm(YZ_ALARM_DISK_FULL,true,0);
					}
					else
					{
						CYuanZhengMan::Instance()->SetYZAlarm(YZ_ALARM_DISK_FULL,false,0);
					}
					
#endif
					if (bIsAlarm)
					{
						WriteLog(LOCAL_CLIENT_ID, LOG_TYE_DISK_FULL);
					}

					m_pLocalDeviceMan->SetOtherAlarm(OTHER_ALARM_TYPE_DISK_FULL, bIsAlarm);
				}
			}
			else
			{
				++ diskFullCnt;
			}
		}

		//检查磁盘健康状态
		{
			if (diskHealthCnt > 7200)
			{
				diskHealthCnt = 0;
				
				if (!m_bOperForFormating)
				{
					bool bHealthAlarm = false;
					DISK_HEALTH_INFO_LIST warningList;
					m_pDiskMan->ProcessDiskHealth(warningList);
					if (warningList.GetCount() > 0)
					{
						bHealthAlarm = true;
						WriteDiskErrorLog(warningList);
					}
					else
					{
						bHealthAlarm = false;
					}
					
					m_pLocalDeviceMan->SetOtherAlarm(OTHER_ALARM_TYPE_DISK_WARNING, bHealthAlarm);
#if defined(__TW01_RILI__)
					m_pMsgMan->SetDiskError(bHealthAlarm);
#endif
				}
			}
			else
			{
				++ diskHealthCnt;
			}	

		}

		bCheckAlarm = false;
		//zxx新加
		{
			timeTmp =  GetCurrTime32();
			if(timeTmp - preTime > 10)
			{
				preTime = timeTmp;
				num=m_pDiskMan->GetDiskCount();
				if(num<m_pDiskMan->GetFirstDiskNum())//拔下硬盘
				{
					bHaveDiskLost=true;
					//磁盘掉线，停止喂狗
					m_bHaveDiskLost = true;
					bCheckAlarm=true;
					m_pDiskMan->SetFirstDiskNum(num);
					WriteLog(LOCAL_CLIENT_ID, LOG_TYE_DISK_DISCONNECT);	
					
				}
				else
				{ 
					bCheckAlarm = false;
				}
#ifdef __CUSTOM_YUANZHENG__
				if(bCheckAlarm)
				{
					CYuanZhengMan::Instance()->SetYZAlarm(YZ_ALARM_DISK_DISCONNET,true,0);
				}
				else
				{
					CYuanZhengMan::Instance()->SetYZAlarm(YZ_ALARM_DISK_DISCONNET,false,0);
				}

#endif
				m_pLocalDeviceMan->SetOtherAlarm(OTHER_ALARM_TYPE_DISK_DISCONNET, bCheckAlarm);
#if defined(__TW01_RILI__)
				m_pMsgMan->SetDiskError(bCheckAlarm);
#endif
// 				if(bHaveDiskLost)
// 				{
// 					m_pLocalDeviceMan->SetBuzzerStatus(true);
// 				}
			}
		}
		

#endif

		PUB_Sleep(500);
	}

	return 0;
}

//网络检测
PUB_THREAD_RESULT PUB_THREAD_CALL CMainFrame::NetworkThread(void *pParam)
{
#ifdef __ENVIRONMENT_LINUX__
	pid_t pid = getpid();
	pid_t tid = syscall(__NR_gettid);
	printf("%s, %d, pid = %d, tid=%d\n", __FILE__, __LINE__, pid, tid);
#endif

	CMainFrame *pThis = reinterpret_cast<CMainFrame *>(pParam);
	pThis->NetworkProc();

	return 0;
}

int CMainFrame::NetworkProc()
{
#ifdef __ENVIRONMENT_LINUX__
	CNetInterface *pNetInter = CNetInterface::Instance();
	unsigned long ip = 0, cnt = 0;
	bool bIpCollision = false, bDisconnect = false;

	while (m_bNetworkProc)
	{
		if (cnt >= 5)
		{
			cnt = 0;

			if (pNetInter->GetNICRunning())
			{
				if (pNetInter->GetIPAddress(ip))
				{
					if (pNetInter->IPIsCollision(ip))
					{
						m_pLocalDeviceMan->SetOtherAlarm(OTHER_ALARM_TYPE_IP_CONFLICT, true);
						if (!bIpCollision)
						{
							bIpCollision = true; 
							WriteLog(LOCAL_CLIENT_ID, LOG_TYPE_IP_CONFLICT);
						}
					}
					else
					{
						bIpCollision = false;
						m_pLocalDeviceMan->SetOtherAlarm(OTHER_ALARM_TYPE_IP_CONFLICT, false);
					}

					if (bDisconnect)
					{
						m_pLocalDeviceMan->SetOtherAlarm(OTHER_ALARM_TYPE_DISCONNET, false);
						bDisconnect = false;
					}
					
					continue;
				}
			}

			{
				m_pLocalDeviceMan->SetOtherAlarm(OTHER_ALARM_TYPE_DISCONNET, true);
				if (!bDisconnect)
				{
					WriteLog(LOCAL_CLIENT_ID, LOG_TYPE_NETWORK_ERR);
					bDisconnect = true;
				}
			}
		}
		else
		{
			++ cnt;
		}

		PUB_Sleep(1000);
	}
#else
	PUB_Sleep(1000);
#endif

	return 0;
}

void CMainFrame::SetWaterMark()
{
#ifdef __ENVIRONMENT_LINUX__
	printf("%s,%d,start dog\n",__FILE__,__LINE__);
	m_pLocalDeviceMan->FeedDog();
	unsigned char mac[7] = {0};
	unsigned char key[9] = "tvtsplay";
	PRODUCT_TYPE pType = CProduct::Instance()->ProductType();

	memset(&m_waterMark, '\0x20', sizeof(DIGIWATERU));
	int fd = socket (AF_INET, SOCK_DGRAM, 0);
	if (fd >= 0)
	{
		struct ifreq  ifr; 
		memset(&ifr, 0, sizeof(ifr));
		strcpy(ifr.ifr_name, "eth0" );
		if (!(ioctl (fd, SIOCGIFHWADDR, (char *)&ifr)) )
		{
			sockaddr *pSocketAddress = &ifr.ifr_hwaddr;
			char* macAddressTemp = (char*)(pSocketAddress->sa_data);
			memcpy(mac,  macAddressTemp, 6);	
		}		
		close(fd);
	}
	else
	{
		memset(mac, '0xa', 6);
	}
	memcpy(m_waterMark.mac, mac, 6);
	m_waterMark.devicetype = pType;
	for (int i = 0; i<m_localVideoInputNum; i++)
	{
		m_waterMark.channel = i;
		printf("%s,%d,set chnn:%d, waterMack\n",__FILE__,__LINE__,i);
		CLocalDevice::Instance()->SetWaterMark(i, key, 8, m_waterMark.water, 16);
	}	
	printf("%s,%d,kill dog\n",__FILE__,__LINE__);
	m_pLocalDeviceMan->KillDog();
#endif
}

#if defined(__TDFH__)

 bool CMainFrame::CheckNetUpdate(char *pPath, char *pDevName)
{
#ifdef __ENVIRONMENT_LINUX__
	char cmd[512] = {0};
	snprintf(cmd, 512, "rm -rf /mnt/upgrade");
	DVRSystem(cmd);

	snprintf(cmd, 512, "mkdir /mnt/upgrade");
	DVRSystem(cmd);

	//先检查压缩包是否正确	
	bool bOk = false;
	do
	{
		snprintf(cmd, 512, "cd /mnt/upgrade; tar xzvf %s preupgrade.sh", pPath);
		if (0 != DVRSystem(cmd))
		{
			break;
		}
		snprintf(cmd, 512, "rm -rf /mnt/upgrade/preupgrade.sh");
		DVRSystem(cmd);

		snprintf(cmd, 512, "cd /mnt/upgrade; tar xzvf %s netupgrade.sh", pPath);	
		if (0 != DVRSystem(cmd))
		{
			snprintf(cmd, 512, "/mnt/mtd/netupgrade.sh");
			if (access(cmd , R_OK) != 0)
			{
				break;
			}
			snprintf(cmd, 512, "cp /mnt/mtd/netupgrade.sh /mnt/upgrade/ -rf");
			DVRSystem(cmd);
		}

		snprintf(cmd, 512, "cd /mnt/upgrade; rm -rf preupgrade.sh");
		if (0 != DVRSystem(cmd))
		{
			break;
		}

		snprintf(cmd, 512, "cd /mnt/upgrade; mv netupgrade.sh  preupgrade.sh");
		if (0 != DVRSystem(cmd))
		{
			break;
		}

		snprintf(cmd, 512, "cd /mnt/upgrade; tar xzvf %s productcheck", pPath);
		if (0 != DVRSystem(cmd))
		{
			break;			
		}

		snprintf(cmd, 512, "/mnt/upgrade/productcheck");
		if (0 != DVRSystem(cmd))
		{
			break;			
		}

		snprintf(cmd, 512, "/mnt/upgrade/check.dat");
		if (access(cmd , R_OK) != 0)
		{
			break;
		}
		bOk = true;
	}while(0);

	if (!bOk)
	{
		snprintf(cmd, 512, "rm -rf /mnt/upgrade");
		DVRSystem(cmd);
		return false;
	}

	//检查是否可以升级
	snprintf(cmd, 512, "/mnt/upgrade/check.dat");
	if (!CheckVersion(cmd))
	{
		snprintf(cmd, 512, "rm -rf /mnt/upgrade");
		DVRSystem(cmd);
		return false;
	}

	snprintf(cmd, 512, "rm -rf /mnt/mtd/UpFile.dat");
	DVRSystem(cmd);

	snprintf(cmd, 512, "echo %s > /mnt/mtd/UpFile.dat", pDevName);
	if (0 != DVRSystem(cmd))
	{
		snprintf(cmd, 512, "rm -rf /mnt/upgrade");
		DVRSystem(cmd);
		return false;
	}

	//设置升级标志
	if (!SetUpdateStatus(true))
	{
		snprintf(cmd, 512, "rm -rf /mnt/upgrade");
		DVRSystem(cmd);
		snprintf(cmd, 512, "rm -rf /mnt/mtd/UpFile.dat");
		DVRSystem(cmd);
		return false;
	}
#endif
	return true;

}

#else

bool CMainFrame::CheckNetUpdate(char *pPath, char *pDevName)
{
#ifdef __ENVIRONMENT_LINUX__
	char *filePath;
#if defined(__CHIPGM__)
	if ((TD_2704AS_SL == CProduct::Instance()->ProductType()) || (TD_2708TS_PL == CProduct::Instance()->ProductType()))
	{
		filePath = "/mnt";
	}
	else
	{
		filePath = "/upgrade";
	}
#else
	filePath = "/upgrade";
#endif

	char cmd[512] = {0};
#if defined(__CHIPGM__)
	if (TD_2704AS_SL != CProduct::Instance()->ProductType() && TD_2708TS_PL != CProduct::Instance()->ProductType())
	{
		snprintf(cmd, 512, "rm -rf /upgrade");
		DVRSystem(cmd);
		snprintf(cmd, 512, "mkdir /upgrade");
		DVRSystem(cmd);
	}
#else
	snprintf(cmd, 512, "rm -rf /upgrade");
	DVRSystem(cmd);
	snprintf(cmd, 512, "mkdir /upgrade");
	DVRSystem(cmd);
#endif
	

	//先检查压缩包是否正确	
	bool bOk = false;
	do
	{
		snprintf(cmd, 512, "cd %s; tar xzvf %s preupgrade.sh", filePath, pPath);
		if (0 != DVRSystem(cmd))
		{
			break;
		}
		snprintf(cmd, 512, "rm -rf %s/preupgrade.sh", filePath);
		DVRSystem(cmd);

		snprintf(cmd, 512, "cd %s; tar xzvf %s netupgrade.sh", filePath, pPath);
		if (0 != DVRSystem(cmd))
		{
			snprintf(cmd, 512, "/mnt/mtd/netupgrade.sh");
			if (access(cmd , R_OK) != 0)
			{
				break;
			}
			snprintf(cmd, 512, "cp /mnt/mtd/netupgrade.sh %s/ -rf", filePath);
			DVRSystem(cmd);
		}
		snprintf(cmd, 512, "cd %s; rm -rf preupgrade.sh", filePath);
		if (0 != DVRSystem(cmd))
		{
			break;
		}   
		snprintf(cmd, 512, "cd %s; mv netupgrade.sh  preupgrade.sh", filePath);
		if (0 != DVRSystem(cmd))
		{
			break;
		}


		snprintf(cmd, 512, "cd %s; tar xzvf %s productcheck", filePath, pPath);
		if (0 != DVRSystem(cmd))
		{
			break;			
		}
		snprintf(cmd, 512, "%s/productcheck", filePath);
		if (0 != DVRSystem(cmd))
		{
			break;			
		}
		snprintf(cmd, 512, "%s/check.dat", filePath);
		if (access(cmd , R_OK) != 0)
		{
			break;
		}
		bOk = true;
	}while(0);

	if (!bOk)
	{
#if defined(__CHIPGM__)
		if (TD_2704AS_SL != CProduct::Instance()->ProductType() && TD_2708TS_PL != CProduct::Instance()->ProductType())
		{
			snprintf(cmd, 512, "rm -rf %s", filePath);
			DVRSystem(cmd);
		}
#else
		snprintf(cmd, 512, "rm -rf %s", filePath);
		DVRSystem(cmd);
#endif
		return false;
	}


	//检查是否可以升级
	snprintf(cmd, 512, "%s/check.dat", filePath);
	if (!CheckVersion(cmd))
	{
#if defined(__CHIPGM__)
		if (TD_2704AS_SL != CProduct::Instance()->ProductType() && TD_2708TS_PL != CProduct::Instance()->ProductType())
		{
			snprintf(cmd, 512, "rm -rf %s", filePath);
			DVRSystem(cmd);
		}
#else
		snprintf(cmd, 512, "rm -rf %s", filePath);
		DVRSystem(cmd);
#endif
		return false;
	}


	snprintf(cmd, 512, "rm -rf /mnt/mtd/UpFile.dat");
	DVRSystem(cmd);

	snprintf(cmd, 512, "echo %s > /mnt/mtd/UpFile.dat", pDevName);
	if (0 != DVRSystem(cmd))
	{
#if defined(__CHIPGM__)
		if (TD_2704AS_SL != CProduct::Instance()->ProductType() && TD_2708TS_PL != CProduct::Instance()->ProductType())
		{
			snprintf(cmd, 512, "rm -rf %s", filePath);
			DVRSystem(cmd);
		}
#else
		snprintf(cmd, 512, "rm -rf %s", filePath);
		DVRSystem(cmd);
#endif
		return false;
	}

	//设置升级标志
	if (!SetUpdateStatus(true))
	{
#if defined(__CHIPGM__)
		if (TD_2704AS_SL != CProduct::Instance()->ProductType() && TD_2708TS_PL != CProduct::Instance()->ProductType())
		{
			snprintf(cmd, 512, "rm -rf %s", filePath);
			DVRSystem(cmd);
		}
#else
		snprintf(cmd, 512, "rm -rf %s", filePath);
		DVRSystem(cmd);
#endif
		snprintf(cmd, 512, "rm -rf /mnt/mtd/UpFile.dat");
		DVRSystem(cmd);
		return false;
	}
#endif
	return true;

}

#endif

void CMainFrame::SetStreamLimit(bool bRequestStream)
{
	ULONGLONG secondStreamCH = 0;
	ULONGLONG secondStreamCH_mobile = 0;
	unsigned long secondSendAllNum = 0;

	if(m_bPlayback || m_bBackup)
	{
#ifdef __DVR_ULTIMATE__
		m_pNetDeviceMan->SetSecondStreamLimitRate(m_pProduct->GetNetLimitMaxRate());

		if((m_playChNum > m_pProduct->PBChannelNumForLimitFirstStream()) || m_bBackup)
		{
			if(g_net_channel != 0)
			{
				m_pNetDeviceMan->SetFirstStreamLimitBitRate(m_pProduct->FirstStreamLimitBitRate());
			}
		}
#else
		PRODUCT_TYPE productID = CProduct::Instance()->ProductType();
		if (m_bBackup && ((TD_2704XS_SL == productID) || (TD_2704AS_SL == productID) || (TD_2704TS_S == productID) || (TD_2704AS_SL_A == productID) || (TD_2704AS_PL == productID)))
		{
			unsigned long limitFrameRate = (VIDEO_FORMAT_PAL == g_video_format) ? 6 : 7;
			m_pStreamProc->SetFirstStreamLimit(limitFrameRate, 2048);
		}
#endif
	}
	else
	{
#ifdef __DVR_ULTIMATE__
		m_pNetDeviceMan->SetSecondStreamLimitRate(CProduct::Instance()->GetSecondStreamCopies());
		m_pNetDeviceMan->SetFirstStreamLimitBitRate(INVALID_INDEX(unsigned long));
#else
		PRODUCT_TYPE productID = CProduct::Instance()->ProductType();

		if ((TD_2704XS_SL == productID) || (TD_2704AS_SL == productID) || (TD_2704TS_S == productID) || (TD_2704AS_SL_A == productID) || (TD_2704AS_PL == productID))
		{
			m_pStreamProc->SetFirstStreamLimit(INVALID_INDEX(unsigned long), INVALID_INDEX(unsigned long));
		}
#endif
	}

	//以下为调整联网情况下调整子码流的代码
	ULONGLONG mobileHigh = 0;
	m_pUserMan->SecondStreamCH(secondStreamCH, secondStreamCH_mobile, secondSendAllNum, mobileHigh);//注意主码流代表次码流，次码流代表手机流
#ifdef __NETSERVER_TVT__
	if (m_pProduct->IsLimitSecondStreamForFirstStreamNum())
	{
		unsigned long firstSendAllNum = CNetServer::Instance()->GetFirstStreamSendNum();
		if (firstSendAllNum > m_pProduct->FirstStreamNumForLimitSecondStream())
		{
			m_pStreamProc->SecondStreamMaxRateForFirstStreamNum(m_pProduct->LimitSecondRateForFirstStreamNum(), CProduct::Instance()->LimitSecondAllRateForFirstStreamNum());
#ifdef __DVR_ULTIMATE__
			m_pNetDeviceMan->SecondStreamMaxRateForFirstStreamNum(m_pProduct->LimitSecondRateForFirstStreamNum());
#endif
		}
		else
		{
			m_pStreamProc->SecondStreamMaxRateForFirstStreamNum(INVALID_INDEX(unsigned long), INVALID_INDEX(unsigned long));
#ifdef __DVR_ULTIMATE__
			m_pNetDeviceMan->SecondStreamMaxRateForFirstStreamNum(INVALID_INDEX(unsigned long));
#endif
		}
	}
#endif
	m_pStreamProc->ChangeSecondStreamCH(secondStreamCH, secondStreamCH_mobile, secondSendAllNum, mobileHigh);
#ifdef __DVR_ULTIMATE__
	if(bRequestStream || (CProduct::Instance()->GetSecondStreamCopies() != CProduct::Instance()->GetNetLimitMaxRate()))
	{
		m_pNetDeviceMan->ChangeSecondStreamCH(secondStreamCH | CNetServer::Instance()->GetFirstStreamCH(), secondStreamCH_mobile, secondSendAllNum+CNetServer::Instance()->GetFirstStreamSendNum());
	}
#endif
}

void CMainFrame::CheckVI()
{
	//获取是否有分辨率改变
#if defined(__ENVIRONMENT_LINUX__)
	bool bChange = false;

	ext_dvr_video_tpye_chg_info video_tpye_chg_info[64];
	memset(video_tpye_chg_info, 0, sizeof(video_tpye_chg_info));
	
	int changeNum = 0;

#if  defined(__CHIP3520A__) ||  defined(__CHIP3520D__) ||defined(__CHIP3521__) || defined(__CHIPGM__) || (defined(__CHIP3531__) && defined(__HISI_SDK_0BX__))

	if (0 == ext_get_video_type_info(video_tpye_chg_info, &changeNum))
	{
		if (0 < changeNum)
		{
			printf("%s:%s:%d, vi video size changed changeNum=%d\n", __FUNCTION__, __FILE__, __LINE__, changeNum);
			bChange = true;
		}
	}
#endif

	if (!bChange)
	{
		return;
	}
#else
	static bool bChange = false;
	if (bChange)
	{
		return;
	}
	else
	{
		bChange = true;
	}
#endif

	//获取配置
	CMyList <NCFG_ITEM_ID> itemList;
	itemList.AddTail(NCFG_ITEM_ENCODE_MAJOR);
	itemList.AddTail(NCFG_ITEM_ENCODE_MINOR);

	unsigned char *pConfigData = NULL;
	ENCODE_INFO * pEncodeInfo = NULL;
	unsigned long dataLen = 0;

	unsigned char *pConfigDataMirror = NULL;
	ENCODE_INFO * pEncodeInfoMirror = NULL;
	unsigned long dataLenMirror = 0;

	if (m_pConfig->GetConfigEx(itemList, &pConfigData, dataLen))
	{
		CConfigBlock cfgBlock;
		cfgBlock.CreateBlock(pConfigData, dataLen);

		unsigned char *pItemData = NULL;
		unsigned long itemLen;
		cfgBlock.GetConfig(NCFG_ITEM_ENCODE_MAJOR, &pItemData, itemLen);
		pEncodeInfo = reinterpret_cast<ENCODE_INFO *>(pItemData);

		unsigned char *pItemDataMirror = NULL;
		unsigned long itemLenMirror;
		cfgBlock.GetConfig(NCFG_ITEM_ENCODE_MINOR,&pItemDataMirror,itemLenMirror);
		pEncodeInfoMirror = reinterpret_cast<ENCODE_INFO *>(pItemDataMirror);
	}

	//修改配置
#if defined(__ENVIRONMENT_LINUX__)
	CProduct * pProduct = CProduct::Instance();

	unsigned long videoSizeMaxk = pProduct->LocalVideoSizeMask();
	unsigned long defVideoSize = pProduct->LocalDefaVideoSize();
	unsigned long hBitStream = pProduct->LocalDefaBitRateValue();
	unsigned long encodeType = pProduct->LocalDefauVideoEncodeMode();
	unsigned long quality = pProduct->LocalDefQuiltyValue();
	unsigned long defRate = pProduct->LocalDefaChnnRateNumNtsc();
	if (VIDEO_FORMAT_PAL == g_video_format)
	{
		defRate = pProduct->LocalDefaChnnRateNumPal();
	}

	if (DVRVIDEO_SIZE_HD1080 == defVideoSize)
	{
		defRate += 1;
		defRate = (defRate << 1);
		if (VIDEO_FORMAT_PAL == g_video_format)
		{
			defRate = (defRate > 25) ? 25 : defRate;
		}
		else
		{
			defRate = (defRate > 30) ? 30 : defRate;
		}
	}

/*
	VIDEO_FORMAT videoFormatTemp = static_cast<VIDEO_FORMAT>(CLocalDevice::GetVideoFormatFromFlash()); //读取flash里的视频制式信息
	if (VIDEO_FORMAT_PAL == videoFormatTemp)
	{
		//如果是恢复默认配置，则根据当前的制式来定。注意NTSC已经不需要重新赋值。
		rate = CProduct::Instance()->LocalDefaChnnRateNumPal();
	}
*/

	unsigned long videoSize = 0;
	int  rate = 0;
	unsigned long bitStream = 8192;

	bool bVIChanged = false;
	bool bVIChangedMirror = false;

	for (int index=0; index < changeNum; index++)
	{
#if defined(__CHIPGM__)
		if (TD_2704AS_SL == CProduct::Instance()->ProductType())
		{
			defRate = pProduct->LocalDefaChnnRateNumNtsc();
			if (VIDEO_FORMAT_PAL == g_video_format)
			{
				defRate = pProduct->LocalDefaChnnRateNumPal();
			}
		}
#endif

		int channel = video_tpye_chg_info[index].channel;
		switch (video_tpye_chg_info[index].video_type)
		{
		case VIDEO_TYPE_720P60:
			{
				rate = 60;
				videoSize = DVRVIDEO_SIZE_HD720;
				bitStream = 4096;
				break;
			}
		case VIDEO_TYPE_720P50:
			{
				rate = 50;
				videoSize = DVRVIDEO_SIZE_HD720;
				bitStream = 4096;
				break;
			}
		case VIDEO_TYPE_1080P30:
			{
				rate = 60;
				videoSize = DVRVIDEO_SIZE_HD1080;
				bitStream = 8192;
				break;
			}
		case VIDEO_TYPE_1080P25:
			{
				rate = 50;
				videoSize = DVRVIDEO_SIZE_HD1080;
				bitStream = 8192;
				break;
			}
		case VIDEO_TYPE_720P30:
			{
				rate = 30;
				videoSize = DVRVIDEO_SIZE_HD720;
				bitStream = 4096;
				break;
			}
		case VIDEO_TYPE_720P25:
			{
				rate = 25;
				videoSize = DVRVIDEO_SIZE_HD720;
				bitStream = 4096;
				break;
			}
		case VIDEO_TYPE_960H30:
			{
				rate = 30;
				videoSize = DVRVIDEO_SIZE_960H;
				bitStream = 2048;
				if (DVRVIDEO_SIZE_HD1080 == defVideoSize)
				{
					defRate += 1;
					defRate = (defRate << 4);
				}
				else if (DVRVIDEO_SIZE_HD720 == defVideoSize)
				{
					defRate += 1;
					defRate = (defRate << 2);
				}


				if (VIDEO_FORMAT_PAL == g_video_format)
				{
					defRate = (defRate > 25) ? 25 : defRate;
				}
				else
				{
					defRate = (defRate > 30) ? 30 : defRate;
				}

				break;
			}
		case VIDEO_TYPE_960H25:
			{
				rate = 25;
				videoSize = DVRVIDEO_SIZE_960H;
				bitStream = 2048;

				if (DVRVIDEO_SIZE_HD1080 == defVideoSize)
				{
					defRate += 1;
					defRate = (defRate << 4);
				}
				else if (DVRVIDEO_SIZE_HD720 == defVideoSize)
				{
					defRate += 1;
					defRate = (defRate << 2);
				}


				if (VIDEO_FORMAT_PAL == g_video_format)
				{
					defRate = (defRate > 25) ? 25 : defRate;
				}
				else
				{
					defRate = (defRate > 30) ? 30 : defRate;
				}

				break;
			}
		case VIDEO_TYPE_HDCIF30:
			{
				rate = 30;
				videoSize = DVRVIDEO_SIZE_CIF;
				bitStream = 512;
				break;
			}
		case VIDEO_TYPE_HDCIF25:
			{
				rate = 25;
				videoSize = DVRVIDEO_SIZE_CIF;
				bitStream = 512;
				break;
			}
		case VIDEO_TYPE_720P60_AHD:
			{
				rate = 60;
				videoSize = DVRVIDEO_SIZE_HD720;
				bitStream = 4096;
				break;
			}
		case VIDEO_TYPE_720P50_AHD:
			{
				rate = 50;
				videoSize = DVRVIDEO_SIZE_HD720;
				bitStream = 4096;
				break;
			}
		case VIDEO_TYPE_1080P30_AHD:
			{
				rate = 60;
				videoSize = DVRVIDEO_SIZE_HD1080;
				bitStream = 8192;
				break;
			}
		case VIDEO_TYPE_1080P25_AHD:
			{
				rate = 50;
				videoSize = DVRVIDEO_SIZE_HD1080;
				bitStream = 8192;
				break;
			}
		case VIDEO_TYPE_720P30_AHD:
			{
				rate = 30;
				videoSize = DVRVIDEO_SIZE_HD720;
				bitStream = 4096;
				break;
			}
		case VIDEO_TYPE_720P25_AHD:
			{
				rate = 25;
				videoSize = DVRVIDEO_SIZE_HD720;
				bitStream = 4096;
				break;
			}
		case VIDEO_TYPE_960H30_AHD:
			{
				rate = 30;
				videoSize = DVRVIDEO_SIZE_960H;
				bitStream = 2048;
				if (DVRVIDEO_SIZE_HD1080 == defVideoSize)
				{
					defRate += 1;
					defRate = (defRate << 4);
				}
				else if (DVRVIDEO_SIZE_HD720 == defVideoSize)
				{
					defRate += 1;
					defRate = (defRate << 2);
				}

				if (VIDEO_FORMAT_PAL == g_video_format)
				{
					defRate = (defRate > 25) ? 25 : defRate;
				}
				else
				{
					defRate = (defRate > 30) ? 30 : defRate;
				}

				break;
			}
		case VIDEO_TYPE_960H25_AHD:
			{
				rate = 25;
				videoSize = DVRVIDEO_SIZE_960H;
				bitStream = 2048;

				if (DVRVIDEO_SIZE_HD1080 == defVideoSize)
				{
					defRate += 1;
					defRate = (defRate << 4);
				}
				else if (DVRVIDEO_SIZE_HD720 == defVideoSize)
				{
					defRate += 1;
					defRate = (defRate << 2);
				}


				if (VIDEO_FORMAT_PAL == g_video_format)
				{
					defRate = (defRate > 25) ? 25 : defRate;
				}
				else
				{
					defRate = (defRate > 30) ? 30 : defRate;
				}

				break;
			}
		case VIDEO_TYPE_HDCIF30_AHD:
			{
				rate = 30;
				videoSize = DVRVIDEO_SIZE_CIF;
				bitStream = 512;
				break;
			}
		case VIDEO_TYPE_HDCIF25_AHD:
			{
				rate = 25;
				videoSize = DVRVIDEO_SIZE_CIF;
				bitStream = 512;
				break;
			}
		default:
			printf("%s:%s:%d, no this video size %d \n", __FUNCTION__, __FILE__, __LINE__, video_tpye_chg_info[index].video_type);
			return;
			break;
		}

		CCalculateSubEncode::Instance()->SetChannelResMask(channel,videoSize);
		CPtzAux::Instance()->SetResolutionType(channel,video_tpye_chg_info[index].video_type);

		if (videoSize & videoSizeMaxk)
		{
			printf("%s:%s:%d, video size=0x%08x, rate=%d, defRate=%d\n", __FUNCTION__, __FILE__, __LINE__, videoSize, rate, defRate);
			if (rate > defRate)
			{
				rate = defRate;
			}

			if (DVRVIDEO_SIZE_HD1080 == videoSize)
			{
				rate = (rate >> 1);
				if (0 >= rate)
				{
					rate = 1;
				}
			}

			printf("%s:%s:%d, video size=0x%08x, rate=%d\n", __FUNCTION__, __FILE__, __LINE__, videoSize, rate);
			unsigned long mirrorConfigSize =0;
			unsigned long mirrorCur =0;
			unsigned long mirrorRate =0;
#if defined(__CHIPGM__)
			if (TD_2704AS_SL == CProduct::Instance()->ProductType())
			{
				pEncodeInfo[channel].resolution = videoSize;
				pEncodeInfo[channel].rate = rate;
				pEncodeInfo[channel].hBitStream = bitStream;
				bVIChanged = true;
				if (pEncodeInfoMirror[channel].resolution != videoSize)
				{
					if (VIDEO_FORMAT_PAL == g_video_format)
					{
						if (pEncodeInfoMirror[channel].rate >12)
						{
							pEncodeInfoMirror[channel].rate =12;
						}
					}else{
						if (pEncodeInfoMirror[channel].rate >15)
						{
							pEncodeInfoMirror[channel].rate =15;
						}
					}
					bVIChangedMirror = true;
				}else{
					bVIChangedMirror = true;
				}
			}
			else
			{
				if (pEncodeInfo[channel].resolution != videoSize)
				{
					pEncodeInfo[channel].resolution = videoSize;
					pEncodeInfo[channel].rate = rate;
					pEncodeInfo[channel].hBitStream = bitStream;
					bVIChanged = true;
				}
				else
				{
					bVIChanged = true;
				}

				if (pEncodeInfoMirror[channel].resolution != videoSize)
				{
					if (VIDEO_FORMAT_PAL == g_video_format)
					{
						if (pEncodeInfoMirror[channel].rate >12)
						{
							pEncodeInfoMirror[channel].rate =12;
						}
					}else{
						if (pEncodeInfoMirror[channel].rate >15)
						{
							pEncodeInfoMirror[channel].rate =15;
						}
					}
					bVIChangedMirror = true;
				}else{
					bVIChangedMirror = true;
				}
			}
#else
			if (pEncodeInfo[channel].resolution != videoSize)
			{
				pEncodeInfo[channel].resolution = videoSize;
				pEncodeInfo[channel].rate = rate;
				pEncodeInfo[channel].hBitStream = bitStream;
				bVIChanged = true;
			}
			else
			{
				bVIChanged = true;
			}
			
			if (pEncodeInfoMirror[channel].resolution != videoSize)
			{
				mirrorCur = ConvertVideoSize(videoSize);
				mirrorConfigSize = ConvertVideoSize(pEncodeInfoMirror[channel].resolution);
				
				if (mirrorConfigSize > mirrorCur)
				{
					pEncodeInfoMirror[channel].resolution = videoSize;
					pEncodeInfoMirror[channel].rate = rate;
					pEncodeInfoMirror[channel].hBitStream = bitStream/2;
				}
				if (DVRVIDEO_SIZE_HD1080 == videoSize)
				{
					if (VIDEO_FORMAT_PAL == g_video_format)
					{
						if (pEncodeInfoMirror[channel].rate >12)
						{
							pEncodeInfoMirror[channel].rate =12;
						}
					}else{
						if (pEncodeInfoMirror[channel].rate >15)
						{
							pEncodeInfoMirror[channel].rate =15;
						}
					}
				}
				bVIChangedMirror = true;
				printf("%s,%d,%d,%d,%d\n",__FILE__,__LINE__,pEncodeInfoMirror[channel].resolution,pEncodeInfoMirror[channel].rate,pEncodeInfoMirror[channel].hBitStream);
			}else{
				bVIChangedMirror = true;
			}
#endif
		}
	}

	if ((!bVIChanged)&&(!bVIChangedMirror))
	{
		delete [] pConfigData;

		return;
	}
#else
for (int chnn=0; chnn < m_localVideoInputNum; chnn++)
{
	pEncodeInfo[chnn].rate = chnn + 1;
	pEncodeInfo[chnn].hBitStream = 512;
}
#endif

	//////////////////////////////////////////////////////////////////////////////////////
	//分配空间
	dataLen = sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD) + sizeof(ENCODE_INFO) * m_localVideoInputNum;
	BYTE * pData = new BYTE[dataLen+1];
	memset(pData, 0, dataLen+1);

	//组装头信息
	NCFG_BLOCK_HEAD * pblockHead = reinterpret_cast<NCFG_BLOCK_HEAD *>(pData);
	pblockHead->netcfgver = NCFG_VERSION;
	pblockHead->biSize = sizeof(NCFG_BLOCK_HEAD);
	pblockHead->ItemNum = 1;

	//获取每一个项的信息
	NCFG_ITEM_HEAD *pItemInfo = reinterpret_cast<NCFG_ITEM_HEAD *>(pData + sizeof(NCFG_BLOCK_HEAD));
	pItemInfo->itemID = NCFG_ITEM_ENCODE_MAJOR;
	pItemInfo->num = 1;
	pItemInfo->len = sizeof(ENCODE_INFO) * m_localVideoInputNum;
	pItemInfo->subLen =  sizeof(ENCODE_INFO);
	memcpy(pData + sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD), pEncodeInfo, sizeof(ENCODE_INFO) * m_localVideoInputNum);

	bool bChangeVencOk = false;
	
	//关掉客户端

	bool bNeedCloseClient = false;

	int tryCount = 0;

	char gateway[40] = {0};
	char target[40] = {0};
	char targetMask[40] = {0};

	do
	{
		MESSAGE_DATA msgData;
		//进入配置
		CMessageMan::Instance()->SendMsgDataToServer(0, LOCAL_DEVICE_ID, CMD_REQUEST_CFG_ENTER, NULL, 0, msgData);
		PACKCMD * pCmd = reinterpret_cast<PACKCMD *>(msgData.pData);
		if (CMD_REPLY_CFG_SUCC == static_cast<NET_PROTOCOL>(pCmd->cmdType))
		{
			CMessageMan::Instance()->ReleaseMsgBuff(msgData);

			//保存信息
			CMessageMan::Instance()->SendMsgDataToServer(0, LOCAL_DEVICE_ID, CMD_REQUEST_CFG_SET, pData, dataLen, msgData);
			pCmd = reinterpret_cast<PACKCMD *>(msgData.pData);
			if (CMD_REPLY_CFG_SUCC == static_cast<NET_PROTOCOL>(pCmd->cmdType))
			{
				bChangeVencOk = true;
			}
			else
			{
				PUB_Sleep(100);

				tryCount ++;
				//如果设置10次不成功就重启设备
				if (10 < tryCount)
				{
					g_system_status = DVR_SYSTEM_REBOOT;
				}
				printf("%s:%s:%d, VI changed but can not change venc\n", __FUNCTION__, __FILE__, __LINE__);
			}

			CMessageMan::Instance()->ReleaseMsgBuff(msgData);

			//退出配置
			CMessageMan::Instance()->PostMsgDataToServer(0, LOCAL_DEVICE_ID, CMD_REQUEST_CFG_LEAVE, NULL, 0);
		}
		else
		{
			CMessageMan::Instance()->ReleaseMsgBuff(msgData);
			unsigned long clientID = m_pConfig->cfgClient();

			if (INVALID_CLIENT_ID != clientID)
			{
				if (LOCAL_CLIENT_ID != clientID)
				{
#if defined(__ENVIRONMENT_LINUX__)
					//获取网关
					CNetInterface::Instance()->GetGateway(gateway, target, targetMask);

					DVRSystem("ifconfig eth0 down");
					bNeedCloseClient = true;
#endif
				}
				else
				{
					if (DVR_SYSTEM_RUNING == g_system_status)
					{
						g_system_status = DVR_SYSTEM_REDRAW;
					}
				}

				PUB_Sleep(100);
			}
		}
#if defined(__CHIPGM__)
if (TD_2708TS_PL == CProduct::Instance()->ProductType())
{
	PUB_Sleep(200);
}
#endif
	} while (!bChangeVencOk);
	
	BYTE * pDataMirror = NULL;
#if defined(__ENVIRONMENT_LINUX__)
	if (true == bVIChangedMirror )
	{
#endif
		//分配空间
		dataLenMirror = sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD) + sizeof(ENCODE_INFO) * m_localVideoInputNum;
		pDataMirror = new BYTE[dataLenMirror+1];
		memset(pDataMirror, 0, dataLenMirror+1);

		//组装头信息
		NCFG_BLOCK_HEAD * pblockHeadMirror = reinterpret_cast<NCFG_BLOCK_HEAD *>(pDataMirror);
		pblockHeadMirror->netcfgver = NCFG_VERSION;
		pblockHeadMirror->biSize = sizeof(NCFG_BLOCK_HEAD);
		pblockHeadMirror->ItemNum = 1;

		//获取每一个项的信息
		NCFG_ITEM_HEAD *pItemInfoMirror = reinterpret_cast<NCFG_ITEM_HEAD *>(pDataMirror + sizeof(NCFG_BLOCK_HEAD));
		pItemInfoMirror->itemID = NCFG_ITEM_ENCODE_MINOR;
		pItemInfoMirror->num = 1;
		pItemInfoMirror->len = sizeof(ENCODE_INFO) * m_localVideoInputNum;
		pItemInfoMirror->subLen =  sizeof(ENCODE_INFO);
		memcpy(pDataMirror + sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD), pEncodeInfoMirror, sizeof(ENCODE_INFO) * m_localVideoInputNum);

		bool bChangeVencOkMirror = false;
		int tryCountMirror = 0;

		do 
		{
			MESSAGE_DATA msgDataMirror;
			//进入配置
			CMessageMan::Instance()->SendMsgDataToServer(0, LOCAL_DEVICE_ID, CMD_REQUEST_CFG_ENTER, NULL, 0, msgDataMirror);
			PACKCMD * pCmdMirror = reinterpret_cast<PACKCMD *>(msgDataMirror.pData);
			if (CMD_REPLY_CFG_SUCC == static_cast<NET_PROTOCOL>(pCmdMirror->cmdType))
			{
				CMessageMan::Instance()->ReleaseMsgBuff(msgDataMirror);

				//保存信息
				CMessageMan::Instance()->SendMsgDataToServer(0, LOCAL_DEVICE_ID, CMD_REQUEST_CFG_SET, pDataMirror, dataLenMirror, msgDataMirror);
				pCmdMirror = reinterpret_cast<PACKCMD *>(msgDataMirror.pData);
				if (CMD_REPLY_CFG_SUCC == static_cast<NET_PROTOCOL>(pCmdMirror->cmdType))
				{
					printf("%s:%s:%d, VI changed but can not change mirror venc\n", __FUNCTION__, __FILE__, __LINE__);
					bChangeVencOkMirror = true;
				}
				else
				{
					PUB_Sleep(100);

					tryCountMirror ++;
					//如果设置10次不成功就重启设备
					if (10 < tryCount)
					{
						g_system_status = DVR_SYSTEM_REBOOT;
					}
					printf("%s:%s:%d, VI changed but can not change mirror venc\n", __FUNCTION__, __FILE__, __LINE__);
				}

				CMessageMan::Instance()->ReleaseMsgBuff(msgDataMirror);

				//退出配置
				CMessageMan::Instance()->PostMsgDataToServer(0, LOCAL_DEVICE_ID, CMD_REQUEST_CFG_LEAVE, NULL, 0);
			}
			else
			{
				CMessageMan::Instance()->ReleaseMsgBuff(msgDataMirror);
				unsigned long clientID = m_pConfig->cfgClient();

				if (INVALID_CLIENT_ID != clientID)
				{
					if (LOCAL_CLIENT_ID != clientID)
					{
#if defined(__ENVIRONMENT_LINUX__)
						DVRSystem("ifconfig eth0 down");
						bNeedCloseClient = true;
#endif
					}
					else
					{
						if (DVR_SYSTEM_RUNING == g_system_status)
						{
							g_system_status = DVR_SYSTEM_REDRAW;
						}
					}

					PUB_Sleep(100);
				}
			}
#if defined(__CHIPGM__)
			if (TD_2708TS_PL == CProduct::Instance()->ProductType())
			{
				PUB_Sleep(200);
			}
#endif
		} while (!bChangeVencOkMirror);
#if defined(__ENVIRONMENT_LINUX__)
	}
#endif
	//保存配置
	//1：关客户端
	//2：关UI
	//3：保存配置
	//5：起客户端

	if (bNeedCloseClient)
	{
#if defined(__ENVIRONMENT_LINUX__)
		DVRSystem("ifconfig eth0 up");

		//恢复网关配置
		int iGateway = inet_addr(gateway);
		int iTarget = inet_addr(target);
		int iTargetMask = inet_addr(targetMask);

		CNetInterface::Instance()->SetGateway(iGateway, iTarget, iTargetMask);
#endif
	}

	if (NULL != pData)
	{
		delete [] pData;
	}
	
	if (NULL != pDataMirror)
	{
		delete [] pDataMirror;
	}

	if (NULL != pConfigData)
	{
		delete [] pConfigData;
	}
	
	if (!m_bFirstCheckVIOver)
	{
		m_bFirstCheckVIOver = true;
	}
}

unsigned long CMainFrame::ConvertVideoSize( unsigned long vs)
{
	unsigned long mirrorSize;
	if (DVRVIDEO_SIZE_CIF == vs )
	{
		mirrorSize = 352*288;
	} 
	else if ( DVRVIDEO_SIZE_D1 == vs )
	{
		mirrorSize = 704*576;
	} 
	else if (DVRVIDEO_SIZE_HD720 == vs)
	{
		mirrorSize = 1280*720;
	}
	else if (DVRVIDEO_SIZE_960H == vs)
	{
		mirrorSize = 1280*960;
	} 
	else if (DVRVIDEO_SIZE_HD1080 == vs)
	{
		mirrorSize = 1920*1080;
	}
	return mirrorSize;
}

bool CMainFrame::SetEncodeAgain()
{
	unsigned char *pData = NULL;
	unsigned long dateLen = 0;

	CMyList <NCFG_ITEM_ID> itemList;


	//
	for (unsigned long i=NCFG_ITEM_ENCODE_MAJOR; i < NCFG_ITEM_ENCODE_PHONE; ++i)
	{
		itemList.AddTail(static_cast<NCFG_ITEM_ID>(i));
	}

	if (m_pConfig->GetConfigEx(itemList, &pData, dateLen))
	{
		m_pConfigSetMan->TakeEffect(pData, dateLen);
		itemList.RemoveAll();
		delete [] pData;
		pData = NULL;
	}
	else
	{
		m_bCanCheckVI = true;
		return false;
	}


	m_bCanCheckVI = true;

	return true;

}

void CMainFrame::WriteDiskErrorLog(DISK_HEALTH_INFO_LIST &warningList)
{
	if (warningList.IsEmpty())
	{
		return;
	}

	POS pos = warningList.GetHeadPosition();
	while (pos != NULL)
	{
		DISK_HEALTH_INFO &diskHealthInfo = warningList.GetNext(pos);
		OPERATOR_LOG		operatorLog;
		memset(&operatorLog, 0, sizeof(OPERATOR_LOG));
		LOG_TYPE type = LOG_TYE_DISK_WARNING;
		operatorLog.IP		= IP_ADDRESS(127, 0, 0, 1);
		snprintf(operatorLog.name, 36, "%s", diskHealthInfo.diskSmartInfo.szSerialNumber);
		operatorLog.contentID = GetContentID(type);
		operatorLog.type = (type & 0xffff);
		operatorLog.time = GetCurrTime32();
		m_pLogMan->WriteOperatorLog(operatorLog);
#if defined(__SUPPORT_SNAPD1__)
		char tip[256]="GB2312:硬盘故障";
		if(NULL != m_pShanghaiConnectMan)
		{
			m_pShanghaiConnectMan->GetLogCmd(DVRDISKERR,0,tip,strlen(tip),NULL,0,GetCurrTime());
		}
	
#endif
	}
}

void CMainFrame::WriteTimeErrorLog(unsigned long errorType)
{
	OPERATOR_LOG		operatorLog;
	LOG_TYPE type = LOG_TYPE_UNKNOWN_ERR;
	memset(&operatorLog, 0, sizeof(OPERATOR_LOG));
	if (0x9b9b9b9b == errorType)
	{
		type = LOG_TYPE_UNKNOWN_ERR;
		operatorLog.IP		= IP_ADDRESS(127, 0, 0, 1);
		strcpy(operatorLog.name, "system:9b9b");
		operatorLog.contentID = GetContentID(type);
		operatorLog.type = (type & 0xffff);
		operatorLog.time = GetCurrTime32();
		m_pLogMan->WriteOperatorLog(operatorLog);
	}
	else if (0x9b9b9b9c == errorType)
	{
		type = LOG_TYPE_UNKNOWN_ERR;
		operatorLog.IP		= IP_ADDRESS(127, 0, 0, 1);
		strcpy(operatorLog.name, "system:9b9c");
		operatorLog.contentID = GetContentID(type);
		operatorLog.type = (type & 0xffff);
		operatorLog.time = GetCurrTime32();
		m_pLogMan->WriteOperatorLog(operatorLog);
	}
	else if (0x9b9b9b9d == errorType)
	{
		type = LOG_TYPE_UNKNOWN_ERR;
		operatorLog.IP		= IP_ADDRESS(127, 0, 0, 1);
		strcpy(operatorLog.name, "system:9b9d");
		operatorLog.contentID = GetContentID(type);
		operatorLog.type = (type & 0xffff);
		operatorLog.time = GetCurrTime32();
		m_pLogMan->WriteOperatorLog(operatorLog);
	}

}

#ifdef __DVR_ULTIMATE__
const char *CMainFrame::GetIPCManufacturerName(unsigned long mid)
{
	assert(mid < MID_COUNT);
	if(mid >= MID_COUNT)
	{
		return m_szManufacturerName[MID_UNKNOWN];
	}

	if((MID_TVT == mid) && (strlen(m_szManufacturerName[mid]) == 0))
	{
		//read from flash
		CLocalDevice::GetCompanyNameFromFlash(m_szManufacturerName[mid], 32);
		m_szManufacturerName[mid][31] = '\0';
	}

	return m_szManufacturerName[mid];
}
#endif

bool CMainFrame::CreateEncodeCfg(const NCFG_ITEM_HEAD *pItemHead,unsigned char *pBlockData,unsigned long readPos,unsigned long itemId,unsigned long clientID)
{
	bool bsucc = false;
	
	if(NCFG_ITEM_ENCODE_MAJOR ==itemId)
	{
		//从少（major）转到多(amjor_ex),读取底面配置并合成过去
		unsigned char *pCfgData = NULL;
		ENCODE_INFO_EX *pExInfo = NULL;
		//读取底面配置ENCODE_INFO_EX
		CMyList <NCFG_ITEM_ID> itemList;
		itemList.AddTail(NCFG_ITEM_ENCODE_MAJOR_EX);
		unsigned char* preadData = NULL;
		unsigned long readlen = 0;
		if (m_pConfig->GetConfigEx(itemList, &preadData, readlen))
		{
			CConfigBlock cfgBlock;
			cfgBlock.CreateBlock(preadData, readlen);

			unsigned long dataLen = 0;
			cfgBlock.GetConfig(NCFG_ITEM_ENCODE_MAJOR_EX, &pCfgData, dataLen);
			assert(dataLen == sizeof(ENCODE_INFO_EX)*m_videoInputNum);

			pExInfo = (ENCODE_INFO_EX *)(pCfgData);
			ENCODE_INFO *pEncode = (ENCODE_INFO *)(pBlockData+readPos);
			for (int i=0; i<m_localVideoInputNum; i++)//Ex超过m_localVideoInputNum部分是乱码,需要读取pExInfo的进行合成
			{
				pExInfo[i].resolution = pEncode[i].resolution;
				pExInfo[i].rate       = pEncode[i].rate;
				pExInfo[i].encodeType = pEncode[i].encodeType;
				pExInfo[i].quality    = pEncode[i].quality;
				pExInfo[i].lBitStream = pEncode[i].lBitStream;
				pExInfo[i].hBitStream = pEncode[i].hBitStream;
				/*printf("%s,%d,pExInfo[i].resolution=%d,pExInfo[i].rate=%d,pExInfo[i].encodeType=%d,pExInfo[i].quality=%d,pExInfo[i].lBitStream=%d,pExInfo[i].hBitStream=%d\n",\
				__FILE__,__LINE__,pExInfo[i].resolution,pExInfo[i].rate,pExInfo[i].encodeType,pExInfo[i].quality,pExInfo[i].lBitStream,pExInfo[i].hBitStream);*/
			}

			NCFG_BLOCK_HEAD blockHead;
			blockHead.netcfgver = NCFG_VERSION;
			blockHead.biSize = sizeof(NCFG_BLOCK_HEAD);
			blockHead.ItemNum = 1;
			int header_len = blockHead.biSize + blockHead.ItemNum * sizeof(NCFG_ITEM_HEAD);
			int setDataLen = 0;
			setDataLen = header_len + m_videoInputNum *sizeof(ENCODE_INFO_EX);

			unsigned char *pData = new unsigned char[setDataLen];
			NCFG_ITEM_HEAD itemHeadEx;
			itemHeadEx.itemID = NCFG_ITEM_ENCODE_MAJOR_EX;
			itemHeadEx.len = m_videoInputNum *sizeof(ENCODE_INFO_EX);
			itemHeadEx.num = m_videoInputNum;
			itemHeadEx.subLen = sizeof(ENCODE_INFO_EX);
			ENCODE_INFO_EX *pNewEncode = (ENCODE_INFO_EX *)(pData + header_len);
			memcpy( pData, &blockHead, sizeof(NCFG_BLOCK_HEAD) );//NCFG_BLOCK_HEAD
			memcpy(pData+blockHead.biSize,&itemHeadEx, blockHead.ItemNum * sizeof(NCFG_ITEM_HEAD));//NCFG_ITEM_HEAD
			memcpy(pNewEncode,pExInfo,sizeof(ENCODE_INFO_EX)*m_videoInputNum);

			bsucc= m_pConfig->SetConfig(pData, setDataLen);
			if(NULL != pData)
			{
				delete []pData;
				pData =NULL;
			}

			if(NULL != preadData)
			{
				delete []preadData;
				preadData =NULL;
			}
			cfgBlock.DestroyBlock();
		}
		else
		{
			printf("read cfg ERR,%s,%d\n",__FILE__,__LINE__);
			assert(false);
			return bsucc;
		}
	}
	else if(NCFG_ITEM_ENCODE_MAJOR_EX ==itemId)
	{
		NCFG_BLOCK_HEAD blockHead;
		blockHead.netcfgver = NCFG_VERSION;
		blockHead.biSize = sizeof(NCFG_BLOCK_HEAD);
		blockHead.ItemNum = 1;
		int header_len = blockHead.biSize + blockHead.ItemNum * sizeof(NCFG_ITEM_HEAD);
		int setDataLen = 0;
		setDataLen = header_len + m_localVideoInputNum *sizeof(ENCODE_INFO);

		//从多（major_ex）转到少(major),截取部分配置
		unsigned char *pData = new unsigned char[setDataLen];
		NCFG_ITEM_HEAD itemHeadEx;
		itemHeadEx.itemID = NCFG_ITEM_ENCODE_MAJOR;
		itemHeadEx.len = m_localVideoInputNum *sizeof(ENCODE_INFO);
		itemHeadEx.num = m_localVideoInputNum;
		itemHeadEx.subLen = sizeof(ENCODE_INFO);
		ENCODE_INFO *pEncode = (ENCODE_INFO *)(pData + header_len);
		ENCODE_INFO_EX *pExInfo = (ENCODE_INFO_EX *)(pBlockData+readPos);
		for (int i=0; i<m_localVideoInputNum; i++)//从多余的ENCODE_INFO_EX部分截取出m_localVideoInputNum部分并存放入ENCODE_INFO
		{
			pEncode[i].resolution = pExInfo[i].resolution;//
			pEncode[i].rate       = pExInfo[i].rate;
			pEncode[i].encodeType = pExInfo[i].encodeType;
			pEncode[i].quality    = pExInfo[i].quality;
			pEncode[i].lBitStream = pExInfo[i].lBitStream;
			pEncode[i].hBitStream = pExInfo[i].hBitStream;
			/*printf("%s,%d,pExInfo[i].resolution=%d,pExInfo[i].rate=%d,pExInfo[i].encodeType=%d,pExInfo[i].quality=%d,pExInfo[i].lBitStream=%d,pExInfo[i].hBitStream=%d\n",\
			__FILE__,__LINE__,pExInfo[i].resolution,pExInfo[i].rate,pExInfo[i].encodeType,pExInfo[i].quality,pExInfo[i].lBitStream,pExInfo[i].hBitStream);*/
		}
		memcpy( pData, &blockHead, sizeof(NCFG_BLOCK_HEAD) );//NCFG_BLOCK_HEAD
		memcpy(pData+blockHead.biSize,&itemHeadEx, blockHead.ItemNum * sizeof(NCFG_ITEM_HEAD));//NCFG_ITEM_HEAD
		bsucc= m_pConfig->SetConfig(pData, setDataLen);
		if(clientID != LOCAL_DEVICE_ID)//网络来的消息需要做本地生效处理
		{
			//保持成功，则开始生效处理。
			m_pConfigSetMan->TakeEffect(pData,setDataLen);

		}
		pEncode = NULL;
		if(pData != NULL)
		{
			delete []pData;
			pData = NULL;
		}
		
	}
	else if(NCFG_ITEM_ENCODE_MINOR ==itemId)
	{
		//从少（major）转到多(amjor_ex),读取底面配置并合成过去
		
		//读取底面配置ENCODE_INFO_EX
		CMyList <NCFG_ITEM_ID> itemList;
		itemList.AddTail(NCFG_ITEM_ENCODE_MINOR_EX);
		unsigned char * preadData = NULL;
		unsigned long	readlen = 0;
		if (m_pConfig->GetConfigEx(itemList, &preadData, readlen))
		{
			CConfigBlock cfgBlock;
			cfgBlock.CreateBlock(preadData, readlen);

			unsigned long dataLen = 0;
			unsigned char *pCfgData = NULL;
			cfgBlock.GetConfig(NCFG_ITEM_ENCODE_MINOR_EX, &pCfgData, dataLen);
			assert(dataLen == sizeof(ENCODE_INFO_EX)*m_videoInputNum);

			ENCODE_INFO_EX *pExInfo = (ENCODE_INFO_EX *)(pCfgData);
			ENCODE_INFO *pEncode = (ENCODE_INFO *)(pBlockData+readPos);
			for (int i=0; i<m_localVideoInputNum; i++)
			{
				pExInfo[i].resolution = pEncode[i].resolution;
				pExInfo[i].rate       = pEncode[i].rate;
				pExInfo[i].encodeType = pEncode[i].encodeType;
				pExInfo[i].quality    = pEncode[i].quality;
				pExInfo[i].lBitStream = pEncode[i].lBitStream;
				pExInfo[i].hBitStream = pEncode[i].hBitStream;
			}

			NCFG_BLOCK_HEAD blockHead;
			blockHead.netcfgver = NCFG_VERSION;
			blockHead.biSize = sizeof(NCFG_BLOCK_HEAD);
			blockHead.ItemNum = 1;
			int header_len = blockHead.biSize + blockHead.ItemNum * sizeof(NCFG_ITEM_HEAD);
			int setDataLen = 0;
			setDataLen = header_len + m_videoInputNum *sizeof(ENCODE_INFO_EX);

			unsigned char *pData = new unsigned char[setDataLen];
			NCFG_ITEM_HEAD itemHeadEx;
			itemHeadEx.itemID = NCFG_ITEM_ENCODE_MINOR_EX;
			itemHeadEx.len = m_videoInputNum * sizeof(ENCODE_INFO_EX);
			itemHeadEx.num = m_videoInputNum;
			itemHeadEx.subLen = sizeof(ENCODE_INFO_EX);
			ENCODE_INFO_EX *pNewEncode = (ENCODE_INFO_EX *)(pData + header_len);
			memcpy( pData, &blockHead, sizeof(NCFG_BLOCK_HEAD) );//NCFG_BLOCK_HEAD
			memcpy(pData+blockHead.biSize,&itemHeadEx, blockHead.ItemNum * sizeof(NCFG_ITEM_HEAD));//NCFG_ITEM_HEAD
			memcpy(pNewEncode,pExInfo,sizeof(ENCODE_INFO_EX)*m_videoInputNum);

			bsucc= m_pConfig->SetConfig(pData, setDataLen);
			if(NULL != pData)
			{
				delete []pData;
				pData =NULL;
			}

			if(NULL != preadData)
			{
				delete []preadData;
				preadData =NULL;
			}
			cfgBlock.DestroyBlock();
		}
		else
		{
			printf("read cfg ERR,%s,%d\n",__FILE__,__LINE__);
			assert(false);
			return bsucc;
		}
	}
	else if(NCFG_ITEM_ENCODE_MINOR_EX == itemId)
	{
		NCFG_BLOCK_HEAD blockHead;
		blockHead.netcfgver = NCFG_VERSION;
		blockHead.biSize = sizeof(NCFG_BLOCK_HEAD);
		blockHead.ItemNum = 1;
		int header_len = blockHead.biSize + blockHead.ItemNum * sizeof(NCFG_ITEM_HEAD);
		int setDataLen = 0;
		setDataLen = header_len + m_localVideoInputNum *sizeof(ENCODE_INFO);

		//从多（major_ex）转到少(major),截取部分配置
		unsigned char *pData = new unsigned char[setDataLen];
		NCFG_ITEM_HEAD itemHeadEx;
		itemHeadEx.itemID = NCFG_ITEM_ENCODE_MINOR;
		itemHeadEx.len = m_localVideoInputNum * sizeof(ENCODE_INFO);
		itemHeadEx.num = m_localVideoInputNum;
		itemHeadEx.subLen = sizeof(ENCODE_INFO);
		ENCODE_INFO *pEncode = (ENCODE_INFO *)(pData + header_len);
		ENCODE_INFO_EX *pExInfo = (ENCODE_INFO_EX *)(pBlockData+readPos);
		for (int i=0; i<m_localVideoInputNum; i++)
		{
			pEncode[i].resolution = pExInfo[i].resolution;//
			pEncode[i].rate       = pExInfo[i].rate;
			pEncode[i].encodeType = pExInfo[i].encodeType;
			pEncode[i].quality    = pExInfo[i].quality;
			pEncode[i].lBitStream = pExInfo[i].lBitStream;
			pEncode[i].hBitStream = pExInfo[i].hBitStream;
		}
		memcpy( pData, &blockHead, sizeof(NCFG_BLOCK_HEAD) );//NCFG_BLOCK_HEAD
		memcpy(pData+blockHead.biSize,&itemHeadEx, blockHead.ItemNum * sizeof(NCFG_ITEM_HEAD));//NCFG_ITEM_HEAD
		bsucc= m_pConfig->SetConfig(pData, setDataLen);
		if(clientID != LOCAL_DEVICE_ID)//网络来的消息需要做本地生效处理
		{
			//保持成功，则开始生效处理。
			m_pConfigSetMan->TakeEffect(pData,setDataLen);
		}
		pEncode = NULL;
		if(pData != NULL)
		{
			delete []pData;
			pData = NULL;
		}

	}
	return bsucc;
}
//bool CMainFrame::SetUpChannelVisible()
//{
//	unsigned char *pData = NULL;
//	unsigned long dateLen = 0;
//
//	CMyList <NCFG_ITEM_ID> itemList;
//
//	itemList.AddTail(static_cast<NCFG_ITEM_ID>(NCFG_ITEM_CHANN_VISIBLE));
//
//	if (m_pConfig->GetConfigEx(itemList, &pData, dateLen))
//	{
//		assert(pData != NULL);
//		assert(dateLen > 0);
//
//		const NCFG_BLOCK_HEAD *pBlockHead = reinterpret_cast<const NCFG_BLOCK_HEAD*>(pData);
//		int itemNum = (int)pBlockHead->ItemNum;
//		assert(1 == itemNum);
//		long readPos = sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD);
//		const unsigned long *pVisible = reinterpret_cast<const unsigned long *>(pData+readPos);
//		if (*pVisible)
//		{
//			m_pLocalDeviceMan->SetCamVisible(false);
//		}
//		else
//		{
//			m_pLocalDeviceMan->SetCamVisible(true);
//		}
//		itemList.RemoveAll();
//		delete [] pData;
//		pData = NULL;
//	}
//	else
//	{
//		return false;
//	}
//	return true;
//}

//////////////////////////////////////////////////////////////////////////
//end

void CMainFrame::UpdateBKC()
{
	unsigned char *pCfgMajorData = NULL;
	unsigned char *pCfgExData = NULL;
	ENCODE_INFO_EX *pExInfo = NULL;
	ENCODE_INFO *pEncode = NULL;
	//读取底面配置ENCODE_INFO
	CMyList <NCFG_ITEM_ID> itemList;
	itemList.AddTail(NCFG_ITEM_ENCODE_MAJOR);
	itemList.AddTail(NCFG_ITEM_ENCODE_MAJOR_EX);
	unsigned char* preadData = NULL;
	unsigned long readlen = 0;

	if (m_pConfig->GetConfigEx(itemList, &preadData, readlen))
	{
		CConfigBlock cfgBlock;
		cfgBlock.CreateBlock(preadData, readlen);

		unsigned long dataLen = 0;
		cfgBlock.GetConfig(NCFG_ITEM_ENCODE_MAJOR, &pCfgMajorData, dataLen);
		assert(dataLen == sizeof(ENCODE_INFO)*m_videoInputNum);
		pEncode = (ENCODE_INFO *)(pCfgMajorData);
		
		//读取底面配置ENCODE_INFO_EX并把pMajor的值赋给他
		cfgBlock.GetConfig(NCFG_ITEM_ENCODE_MAJOR_EX, &pCfgExData, dataLen);
		assert(dataLen == sizeof(ENCODE_INFO_EX)*m_videoInputNum);
		pExInfo = (ENCODE_INFO_EX *)(pCfgExData);

		for (int i=0; i<m_localVideoInputNum; i++)//Ex超过m_localVideoInputNum部分是乱码,需要读取pExInfo的进行合成
		{
			pExInfo[i].resolution = pEncode[i].resolution;
			pExInfo[i].rate       = pEncode[i].rate;
			pExInfo[i].encodeType = pEncode[i].encodeType;
			pExInfo[i].quality    = pEncode[i].quality;
			pExInfo[i].lBitStream = pEncode[i].lBitStream;
			pExInfo[i].hBitStream = pEncode[i].hBitStream;
		}

		NCFG_BLOCK_HEAD blockHead;
		blockHead.netcfgver = NCFG_VERSION;
		blockHead.biSize = sizeof(NCFG_BLOCK_HEAD);
		blockHead.ItemNum = 1;
		int header_len = blockHead.biSize + blockHead.ItemNum * sizeof(NCFG_ITEM_HEAD);
		int setDataLen = 0;
		setDataLen = header_len + m_videoInputNum *sizeof(ENCODE_INFO_EX);

		unsigned char *pData = new unsigned char[setDataLen];
		NCFG_ITEM_HEAD itemHeadEx;
		itemHeadEx.itemID = NCFG_ITEM_ENCODE_MAJOR_EX;
		itemHeadEx.len = m_videoInputNum *sizeof(ENCODE_INFO_EX);
		itemHeadEx.num = m_videoInputNum;
		itemHeadEx.subLen = sizeof(ENCODE_INFO_EX);
		ENCODE_INFO_EX *pNewEncode = (ENCODE_INFO_EX *)(pData + header_len);
		memcpy( pData, &blockHead, sizeof(NCFG_BLOCK_HEAD) );//NCFG_BLOCK_HEAD
		memcpy(pData+blockHead.biSize,&itemHeadEx, blockHead.ItemNum * sizeof(NCFG_ITEM_HEAD));//NCFG_ITEM_HEAD
		memcpy(pNewEncode,pExInfo,sizeof(ENCODE_INFO_EX)*m_videoInputNum);

		m_pConfig->SetConfig(pData, setDataLen);
		if(NULL != pData)
		{
			delete []pData;
			pData =NULL;
		}

		if(NULL != preadData)
		{
			delete []preadData;
			preadData =NULL;
		}
		cfgBlock.DestroyBlock();
		
	}
}

void CMainFrame::UpdateCVBS()
{
	unsigned char *pCfgMajorData = NULL;
	unsigned char *pCfgExData = NULL;
	ENCODE_INFO_EX *pExInfo = NULL;
	ENCODE_INFO *pEncode = NULL;
	//读取底面配置ENCODE_INFO
	CMyList <NCFG_ITEM_ID> itemList;
	itemList.AddTail(NCFG_ITEM_ENCODE_MAJOR);
	itemList.AddTail(NCFG_ITEM_ENCODE_MAJOR_EX);
	unsigned char* preadData = NULL;
	unsigned long readlen = 0;

	if (m_pConfig->GetConfigEx(itemList, &preadData, readlen))
	{
		CConfigBlock cfgBlock;
		cfgBlock.CreateBlock(preadData, readlen);

		unsigned long dataLen = 0;
		cfgBlock.GetConfig(NCFG_ITEM_ENCODE_MAJOR, &pCfgMajorData, dataLen);
		assert(dataLen == sizeof(ENCODE_INFO)*m_localVideoInputNum);
		pEncode = (ENCODE_INFO *)(pCfgMajorData);

		//读取底面配置ENCODE_INFO_EX并把pMajor的值赋给他
		cfgBlock.GetConfig(NCFG_ITEM_ENCODE_MAJOR_EX, &pCfgExData, dataLen);
		assert(dataLen == sizeof(ENCODE_INFO_EX)*m_videoInputNum);
		pExInfo = (ENCODE_INFO_EX *)(pCfgExData);

		for (int i=0; i<m_localVideoInputNum; i++)//Ex超过m_localVideoInputNum部分是乱码,需要读取pExInfo的进行合成
		{
			if (pEncode[i].rate >15)
			{
				if (VIDEO_FORMAT_PAL == g_video_format)
				{
					pEncode[i].rate = 12;
					pExInfo[i].rate = 12;
				}
				if (VIDEO_FORMAT_NTSC == g_video_format)
				{
					pEncode[i].rate = 15;
					pExInfo[i].rate = 15;
				}
			}
		}

		NCFG_BLOCK_HEAD blockHead;
		blockHead.netcfgver = NCFG_VERSION;
		blockHead.biSize = sizeof(NCFG_BLOCK_HEAD);
		blockHead.ItemNum = 1;
		int header_len = blockHead.biSize + blockHead.ItemNum * sizeof(NCFG_ITEM_HEAD);
		int setDataLen = 0;
		setDataLen = header_len + m_videoInputNum *sizeof(ENCODE_INFO_EX);

		unsigned char *pData = new unsigned char[setDataLen];
		NCFG_ITEM_HEAD itemHeadEx;
		itemHeadEx.itemID = NCFG_ITEM_ENCODE_MAJOR_EX;
		itemHeadEx.len = m_videoInputNum *sizeof(ENCODE_INFO_EX);
		itemHeadEx.num = m_videoInputNum;
		itemHeadEx.subLen = sizeof(ENCODE_INFO_EX);
		ENCODE_INFO_EX *pNewEncode = (ENCODE_INFO_EX *)(pData + header_len);
		memcpy( pData, &blockHead, sizeof(NCFG_BLOCK_HEAD) );//NCFG_BLOCK_HEAD
		memcpy(pData+blockHead.biSize,&itemHeadEx, blockHead.ItemNum * sizeof(NCFG_ITEM_HEAD));//NCFG_ITEM_HEAD
		memcpy(pNewEncode,pExInfo,sizeof(ENCODE_INFO_EX)*m_videoInputNum);

		m_pConfig->SetConfig(pData, setDataLen);
		if(NULL != pData)
		{
			delete []pData;
			pData =NULL;
		}
		//
		NCFG_BLOCK_HEAD blockHead_MAJOR;
		blockHead_MAJOR.netcfgver = NCFG_VERSION;
		blockHead_MAJOR.biSize = sizeof(NCFG_BLOCK_HEAD);
		blockHead_MAJOR.ItemNum = 1;
		int header_len_major = blockHead_MAJOR.biSize + blockHead_MAJOR.ItemNum * sizeof(NCFG_ITEM_HEAD);
		int setData = 0;
		setData = header_len_major + m_localVideoInputNum *sizeof(ENCODE_INFO);

		unsigned char *pDataMajor = new unsigned char[setData];
		NCFG_ITEM_HEAD itemHeadEx_major;
		itemHeadEx_major.itemID = NCFG_ITEM_ENCODE_MAJOR;
		itemHeadEx_major.len = m_localVideoInputNum *sizeof(ENCODE_INFO);
		itemHeadEx_major.num = m_localVideoInputNum;
		itemHeadEx_major.subLen = sizeof(ENCODE_INFO);
		ENCODE_INFO *pNewEncodeMajor = (ENCODE_INFO *)(pDataMajor + header_len_major);
		memcpy( pDataMajor, &blockHead_MAJOR, sizeof(NCFG_BLOCK_HEAD) );//NCFG_BLOCK_HEAD
		memcpy(pDataMajor+blockHead_MAJOR.biSize,&itemHeadEx_major, blockHead_MAJOR.ItemNum * sizeof(NCFG_ITEM_HEAD));//NCFG_ITEM_HEAD
		memcpy(pNewEncodeMajor,pEncode,sizeof(ENCODE_INFO)*m_localVideoInputNum);

		m_pConfig->SetConfig(pDataMajor, setData);
		if(NULL != pDataMajor)
		{
			delete []pDataMajor;
			pDataMajor =NULL;
		}

		if(NULL != preadData)
		{
			delete []preadData;
			preadData =NULL;
		}
		cfgBlock.DestroyBlock();

	}
}
