/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : xkg
** Date         : 2009-05-22
** Name         : ConfigSetMan.h
** Version      : 1.0
** Description  :
** Modify Record:
***********************************************************************/
#include "ConfigSetMan.h"
#include "PUB_common.h"
#include "UserMan.h"
#include "FBInterface.h"
#ifdef __ENVIRONMENT_LINUX__
#include "UpnpMan.h"
#endif

#ifdef __ENVIRONMENT_LINUX__

#ifdef __NETSERVER_TVT__
#include "webserver.h"
#include "NetServer.h"
#include "AlarmMan.h"
#endif


#ifdef __NETSERVER_HXHT__
#include "HxConnectMan.h"
#endif

#ifdef __CUSTOM_IPV6__
#include "AllocNICIP6.h"
#endif

#endif

#if defined (__ENVIRONMENT_LINUX__) && defined (__DVR_BASIC__)
#include "SmtpMan.h"
#include "SnapPictureMan.h"
#include "SnapCapabilityMan.h"
#endif

#ifdef __SUPPORT_HRSST__
#include "PhoneServer.h"
#endif

#ifdef __CUSTOM_YUANZHENG__
#include "YuanZhengClinet.h"
#include "YZSnapMan.h"
#endif

#ifdef __SUPPORT_SNAPD1__
#include "ShanghaiSnapMan.h"
#include "SnapPictureMan.h"
#endif

//ui
#include "TimeCtrl.h"
#include "DateCtrl.h"
#include "KeyboardDlg.h"

#include "WndStructDef.h"

#ifdef __ALARM_TO_UPDATE_FTP_VIDEO__
#include "FTPUpdateMan.h"
#include "FtpLoadMan.h"
#endif

#ifdef __GB28181_SERVER__
#include "Gb28181Server.h"
#endif

#include "NetStreamAllotDevice.h"
#include "FuncCustom.h"
using namespace GUI;

extern DVR_SYSTEM_STATUS g_system_status;
extern VIDEO_FORMAT		g_video_format;
extern LANGUAGE			g_language;
extern VGA_RESOLUTION	g_resolution;
extern unsigned long	g_timeMode;
extern unsigned long	g_dateMode;
extern unsigned long	g_showWizzard;
extern unsigned long	g_screen_save_time;

extern WND_STARTUP_INFO* g_p_wnd_startup_info;
extern WND_STARTUP_INFO* g_p_cur_startup_info;
extern bool              g_curNet3G;  
extern unsigned long g_adjustCvbsVO;
extern int  g_video_out_mode;
extern NET_LOGIN_SUCCESS_REPLY *g_p_login_succ;

const char TIME_STAMP_STR [] = "0123456789 :/-APM.";
//////////////////////////////////////////////////////////////////////////
CConfigSetMan::CConfigSetMan()
{
	m_videoInputNum			= 0;
	m_localVideoInputNum	= 0;
	m_netVideoInputNum		= 0;
	m_sensorInputNum		= 0;
	m_sensorOutputNum		= 0;

	m_pRecMan			= NULL;
	m_pMotionMan		= NULL;
	m_pSensorMan		= NULL;
	m_pVideoLossMan		= NULL;
	m_pAlarmOutMan		= NULL;
	m_pLogMan           = NULL;
	m_pDisplayMan		= NULL;
	m_pNetReader        = NULL;

#ifdef __ENVIRONMENT_LINUX__
	m_pNetInterface		= NULL;
	m_pLocalDeviceMan   = NULL;
	m_pOSDInfoCamName	= NULL;
#endif

#ifdef __MOBILE_DVR__
	m_pspeedMan			= NULL;
#endif

#ifdef __DVR_ULTIMATE__
	m_pNetDeviceMan = NULL;
#endif

	m_bHasInitial		= false;

#ifdef __ENVIRONMENT_LINUX__
	memset(m_currentCover, 0, sizeof(m_currentCover));
	memset(m_motionDetectInfo, 0, sizeof(m_motionDetectInfo));
#endif

	memset(&m_encode, 0, sizeof(ENCODE_INFO) * 64);
	memset(&m_encodeEx, 0, sizeof(ENCODE_INFO_EX) * 64);
	memset(&m_setEncodeCount, 0, sizeof(unsigned long) * 64);
}

CConfigSetMan::~CConfigSetMan()
{

}

#ifdef __MOBILE_DVR__
bool CConfigSetMan::Initial(unsigned char localChnnNum, unsigned char netChnnNum, unsigned char sensorInputNum, unsigned char sensorOutputNum, \
							CRecordMan *pRecMan, CMotionMan *pMotionMan, CSensorInMan *pSensorMan, CVideoLossMan *pVideoLossMan, COtherAlarmMan *pOtherAlarm, \
							CAlarmOutMan *pAlarmOutMan, CReclogManEX *pReclogMan, CLogMan *pLogMan, CLocalDevice *pLocalDeviceMan,\
							CDisplayMan* pDisplayMan, CPtzMan* pPtzMan, CNetReader* pNetReader, CStreamProc *pStreamProc, CSpeedMan *pSpeedMan)
{
	m_videoInputNum	= localChnnNum + netChnnNum;
	m_localVideoInputNum = localChnnNum;
	m_netVideoInputNum	= netChnnNum;

	m_sensorInputNum = sensorInputNum;
	m_sensorOutputNum = sensorOutputNum;
	m_pRecMan = pRecMan;
	m_pMotionMan = pMotionMan;
	m_pVideoLossMan = pVideoLossMan;
	m_pOtherAlarm = pOtherAlarm;
	m_pSensorMan = pSensorMan;
	m_pAlarmOutMan = pAlarmOutMan;
	m_pReclogMan = pReclogMan;
	m_pLogMan = pLogMan;
	m_pDisplayMan = pDisplayMan;
	m_pPtzMan = pPtzMan;
	m_pNetReader = pNetReader;
	m_pStreamProc = pStreamProc;
#ifdef __ENVIRONMENT_LINUX__
	m_pNetInterface = CNetInterface::Instance();
	if (!m_pNetInterface->Initial())
	{
		return false;
	}
	m_pLocalDeviceMan = pLocalDeviceMan;

	m_pOSDInfoCamName =  new OSD_INFO [m_videoInputNum];
#endif
	m_pspeedMan = pSpeedMan;

	return true;
}
#else
#ifdef __DVR_ULTIMATE__
bool CConfigSetMan::Initial(unsigned char localChnnNum, unsigned char netChnnNum, unsigned char localSensorInputNum, unsigned char sensorInputNum, unsigned char sensorOutputNum, \
							CRecordMan *pRecMan, CMotionMan *pMotionMan, CSensorInMan *pSensorMan, CVideoLossMan *pVideoLossMan, COtherAlarmMan *pOtherAlarm, \
							CAlarmOutMan *pAlarmOutMan, CReclogManEX *pReclogMan, CLogMan *pLogMan, CLocalDevice *pLocalDeviceMan,\
							CDisplayMan* pDisplayMan, CPtzMan* pPtzMan, CNetReader* pNetReader, CStreamProc *pStreamProc, CNetDeviceManager* pNetDeviceMan)
#else
bool CConfigSetMan::Initial(unsigned char localChnnNum, unsigned char netChnnNum, unsigned char localSensorInputNum, unsigned char sensorInputNum, unsigned char sensorOutputNum, \
							CRecordMan *pRecMan, CMotionMan *pMotionMan, CSensorInMan *pSensorMan, CVideoLossMan *pVideoLossMan, COtherAlarmMan *pOtherAlarm, \
							CAlarmOutMan *pAlarmOutMan, CReclogManEX *pReclogMan, CLogMan *pLogMan, CLocalDevice *pLocalDeviceMan,\
							CDisplayMan* pDisplayMan, CPtzMan* pPtzMan, CNetReader* pNetReader, CStreamProc *pStreamProc)
#endif
{
	m_videoInputNum	= localChnnNum + netChnnNum;
	m_localVideoInputNum = localChnnNum;
	m_netVideoInputNum	= netChnnNum;

	m_localSensorInputNum = localSensorInputNum;
	m_sensorInputNum = sensorInputNum;
	m_sensorOutputNum = sensorOutputNum;
	m_pRecMan = pRecMan;
	m_pMotionMan = pMotionMan;
	m_pVideoLossMan = pVideoLossMan;
	m_pOtherAlarm = pOtherAlarm;
	m_pSensorMan = pSensorMan;
	m_pAlarmOutMan = pAlarmOutMan;
    m_pReclogMan = pReclogMan;
	m_pLogMan = pLogMan;
	m_pDisplayMan = pDisplayMan;
	m_pPtzMan = pPtzMan;
	m_pNetReader = pNetReader;
	m_pStreamProc = pStreamProc;
#ifdef __ENVIRONMENT_LINUX__
	m_pNetInterface = CNetInterface::Instance();
	if (!m_pNetInterface->Initial())
	{
		return false;
	}
	m_pLocalDeviceMan = pLocalDeviceMan;

	m_pOSDInfoCamName =  new OSD_INFO [m_videoInputNum];
#endif

#ifdef __DVR_ULTIMATE__
	m_pNetDeviceMan = pNetDeviceMan;
#endif

	return true;
}
#endif

void CConfigSetMan::Quit()
{
#ifdef __ENVIRONMENT_LINUX__
	m_pNetInterface->Quit();

	delete [] m_pOSDInfoCamName;
	m_pOSDInfoCamName = NULL;
#endif

	m_videoInputNum			= 0;
	m_localVideoInputNum	= 0;
	m_netVideoInputNum		= 0;

	m_sensorInputNum		= 0;
	m_sensorOutputNum		= 0;

	m_pRecMan		= NULL;
	m_pMotionMan	= NULL;
	m_pVideoLossMan = NULL;
	m_pOtherAlarm	= NULL;
	m_pSensorMan	= NULL;
	m_pAlarmOutMan	= NULL;
	m_pReclogMan	= NULL;
	m_pLogMan		= NULL;
	m_pDisplayMan	= NULL;
	m_pPtzMan		= NULL;
	m_pNetReader	= NULL;
	m_pStreamProc	= NULL;
#ifdef __MOBILE_DVR__
	m_pspeedMan		= NULL;
#endif

#ifdef __DVR_ULTIMATE__
	m_pNetDeviceMan = NULL;
#endif
}

void CConfigSetMan::TakeEffect(const unsigned char *pDataBlock, unsigned long dataLen)
{
	assert(pDataBlock != NULL);
	assert(dataLen > 0);
	
	long itemHeadLen = sizeof(NCFG_ITEM_HEAD);
	const NCFG_BLOCK_HEAD *pBlockHead = reinterpret_cast<const NCFG_BLOCK_HEAD*>(pDataBlock);
	int itemNum = (int)pBlockHead->ItemNum; 
	long readPos = sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD)*itemNum;
	const NCFG_ITEM_HEAD *pItemHead = NULL;

	for (int i =0; i<itemNum; i++)
	{
		pItemHead = reinterpret_cast<const NCFG_ITEM_HEAD*>(pDataBlock + i*itemHeadLen + sizeof(NCFG_BLOCK_HEAD));
		if ((pItemHead->itemID>NCFG_ITEM_SYSTEM_BASE) && (pItemHead->itemID<NCFG_ITEM_SYSTEM_END))
		{
			EffectSystemCfg(pItemHead, pDataBlock+readPos);
		}
		else if ((pItemHead->itemID>NCFG_ITEM_CHANNEL_BASE) && (pItemHead->itemID<NCFG_ITEM_CHANNEL_END))
		{
			EffectChannCfg(pItemHead, pDataBlock+readPos);
		}
		else if ((pItemHead->itemID>NCFG_ITEM_CHANNEL_REC_BASE) && (pItemHead->itemID<NCFG_ITEM_CHANNEL_REC_END))
		{
			EffectChannRecCfg(pItemHead, pDataBlock+readPos);
		}
		else if ((pItemHead->itemID>NCFG_ITEM_SCHEDULE_BASE) && (pItemHead->itemID<NCFG_ITEM_SCHEDULE_END))
		{
			EffectScheduleCfg(pItemHead, pDataBlock+readPos);
		}
		else if ((pItemHead->itemID>NCFG_ITEM_NET_BASE) && (pItemHead->itemID<NCFG_ITEM_NET_END))
		{
			EffectNetBaseCfg(pItemHead, pDataBlock+readPos);
		}
		else if ((pItemHead->itemID>NCFG_ITEM_ALARM_TRIGER_BASE) && (pItemHead->itemID<NCFG_ITEM_ALARM_TRIGER_END))
		{
			EffectAlarmTrigerCfg(pItemHead, pDataBlock+readPos);
		}
		else if ((pItemHead->itemID>NCFG_ITEM_ALARM_TRIGER_SENSOR_BASE) && \
			(pItemHead->itemID<NCFG_ITEM_ALARM_TRIGER_SENSOREND))
		{
			EffectAlarmTrigerSensorCfg(pItemHead, pDataBlock+readPos);
		}
		else if ((pItemHead->itemID>NCFG_ITEM_ALARM_TRIGER_MOTION_BASE) && \
			(pItemHead->itemID<NCFG_ITEM_ALARM_TRIGER_MOTIONEND))
		{
			EffectAlarmTrigerMotionCfg(pItemHead, pDataBlock+readPos);
		}
		else if ((pItemHead->itemID>NCFG_ITEM_ALARM_TRIGER_VLOSS_BASE) && \
			(pItemHead->itemID<NCFG_ITEM_ALARM_TRIGER_VLOSSEND))
		{
			EffectAlarmTrigerVLossCfg(pItemHead, pDataBlock+readPos);
		}
		else if ((pItemHead->itemID>NCFG_ITEM_ALARM_TRIGER_SHELTER_BASE) && \
			(pItemHead->itemID<NCFG_ITEM_ALARM_TRIGER_SHELTEREND))
		{
			EffectAlarmTrigerShelterCfg(pItemHead, pDataBlock+readPos);
		}
		else if ((pItemHead->itemID>NCFG_ITEM_EXCEPTION_BASE) && \
			(pItemHead->itemID<NCFG_ITEM_EXCEPTION_END))
		{
			EffectAlarmExceptionCfg(pItemHead, pDataBlock+readPos);
		}
		else if ((pItemHead->itemID>NCFG_ITEM_ENCODE_BASE) && (pItemHead->itemID<NCFG_ITEM_ENCODE_END))
		{
			EffectEncodeCfg(pItemHead, pDataBlock+readPos);
		}
		else if ((pItemHead->itemID >= NCFG_ITEM_PTZ_INFO_PRESET_INFO_CHBASE)&& 
			(pItemHead->itemID < NCFG_ITEM_PTZ_INFO_PRESET_INFO_CHEND))
		{
			EffectPresetCfg(pItemHead, pDataBlock+readPos);
		}
		else if ((pItemHead->itemID >= NCFG_ITEM_PTZ_INFO_CRUISE_INFO_CHBASE)&& 
			(pItemHead->itemID < NCFG_ITEM_PTZ_INFO_CRUISE_INFO_CHEND))
		{
			EffecCruiseCfg(pItemHead, pDataBlock+readPos);
		}
#ifdef __MOBILE_DVR__
		else if ((pItemHead->itemID >= NCFG_ITEM_MOBILE_BASE)&& 
			(pItemHead->itemID < NCFG_ITEM_MOBILE_END))
		{
			//EffecMobileCfg(pItemHead, pDataBlock+readPos);
		}
		else if ((pItemHead->itemID >= NCFG_ITEM_SPEED_BASE)&& 
			(pItemHead->itemID < NCFG_ITEM_SPEED_END))
		{
			EffecBasicSpeedCfg(pItemHead,pDataBlock+readPos);
		}
		else if ((pItemHead->itemID >= NCFG_ITEM_HIGH_SPEED_BASE)&& 
			(pItemHead->itemID < NCFG_ITEM_HIGH_SPEED_END))
		{
			EffecHighSpeedCfg(pItemHead,pDataBlock+readPos);
		}
		else if ((pItemHead->itemID >= NCFG_ITEM_LOW_SPEED_BASE)&& 
			(pItemHead->itemID < NCFG_ITEM_LOW_SPEED_END))
		{
			EffecLowSpeedCfg(pItemHead,pDataBlock+readPos);
		}
		else if ((pItemHead->itemID >= NCFG_ITEM_WIRELESS_BASE)&& 
			(pItemHead->itemID < NCFG_ITEM_WIRELESS_END))
		{
			
		}
#endif
#ifdef __GB28181_SERVER__
		else if((pItemHead->itemID > NCFG_ITEM_GB28181_BASE) && \
			(pItemHead->itemID<NCFG_ITEM_GB28181_END))
		{
			EffectGB28181Cfg(pItemHead, pDataBlock+readPos);
		}
#endif
		else
		{
			//待设置的ID没有处理
			assert(false);
		}
		readPos += pItemHead->len;
	}
	assert(readPos == dataLen);

	//对网络设备生效
#ifdef __DVR_ULTIMATE__
	m_pNetDeviceMan->SetNetConfig(pDataBlock, dataLen);
#endif
}

void CConfigSetMan::EffectSystemCfg(const NCFG_ITEM_HEAD *pItemHead, const unsigned char *pData)
{
	switch(pItemHead->itemID)
	{
	case NCFG_ITEM_DEVICE_ID:				//设备ID，unsigned long
		{
#ifdef __ENVIRONMENT_LINUX__
			CRemoteControl::Instance()->SetDVRID( *reinterpret_cast<const unsigned long *>(pData) );
			CExternalKeyboard::Instance()->SetDVRID( *reinterpret_cast<const unsigned long *>(pData) );

#ifdef __DVR_BASIC__
			CSmtpMan::Instance()->SetDeviceID(*reinterpret_cast<const unsigned long *>(pData));
#endif
#endif
			if (NULL != g_p_login_succ)
			{
				g_p_login_succ->productInfo.deviceID = *reinterpret_cast<const unsigned long *>(pData);
			}
			
			CProduct::Instance()->SetDeviceID(*reinterpret_cast<const unsigned long *>(pData));
		}
		break;		
	case NCFG_ITEM_DEVICE_NAME:				//设备名称，NCFG_INFO_SHORT_NAME
		{
			const NCFG_INFO_SHORT_NAME *pDeviceInfo = reinterpret_cast<const NCFG_INFO_SHORT_NAME *>(pData);
#ifdef __ENVIRONMENT_LINUX__
			
#ifdef __DVR_BASIC__
			CSmtpMan::Instance()->SetDeviceName(pDeviceInfo->name);
#endif

#ifdef __NETSERVER_TVT__
			CDdnsManager::Instance()->SetDeviceName(pDeviceInfo->name);
#endif
			CUpnpMan::Instance()->SetDeviceName(pDeviceInfo->name);
#endif
			if (NULL != g_p_login_succ)
			{
				snprintf(g_p_login_succ->productInfo.deviceName, sizeof(g_p_login_succ->productInfo.deviceName) - 1, "%s", pDeviceInfo->name);
			}
			
			CProduct::Instance()->SetDeviceName(pDeviceInfo->name);
		}
		break;
	case NCFG_ITEM_VIDEO_FORMAT:			//视频格式，unsigned long ,视频制式
		{
			if (m_bHasInitial)
			{
				g_video_format = static_cast<VIDEO_FORMAT>(*reinterpret_cast<const unsigned long*>(pData));
				char szTmp[8] = {0};
				sprintf(szTmp, "%d", g_video_format);
				CLocalDevice::WriteVideoFormatToFlash(szTmp, 1);

				WriteLog(CConfigEx::Instance()->cfgClient(), LOG_TYPE_CHGE_VIDEO_FORMAT);
				g_system_status = DVR_SYSTEM_REBOOT;
			}
		}
		break;
	case NCFG_ITEM_AUTHORIZATION:			//是否权限检查, unsigned long
		{
			CUserMan::Instance()->SetCheckAuth(*(reinterpret_cast<const unsigned long *>(pData)) ? true : false);
			CMessageMan::Instance()->SetCheckPassword(*(reinterpret_cast<const unsigned long *>(pData)) ? true : false);

			if (*(reinterpret_cast<const unsigned long *>(pData)))
			{
				WriteLog(CConfigEx::Instance()->cfgClient(), LOG_TYPE_PASSWD_ON);
			}
			else
			{
				WriteLog(CConfigEx::Instance()->cfgClient(), LOG_TYPE_PASSWD_OFF);
			}
		}
		break;
	case NCFG_ITEM_LIVE_AUDIO_ENABLE:		//现场音频，unsigned long 
		break;
	case NCFG_ITEM_LIVE_AUDIO_CHNN:			//现场音频通道,unsigned long
		break;
	case NCFG_ITEM_LIVE_AUDIO_VOLUME:
		break;
	case NCFG_ITEM_AUDIO_BROADCAST_ENABLE:	//是否开启了语音广播, unsigned long
		break;
	case NCFG_ITEM_DATE_FORMAT:				//日期格式，unsigned long
		{
			if (m_bHasInitial)
			{
				const unsigned long* pValue = reinterpret_cast<const unsigned long*>(pData);
				unsigned long modeDate = GUI::CDateCtrl::DateMode(*pValue);
				if (modeDate != GUI::CDateCtrl::GetDateMode())
				{
					GUI::CDateCtrl::SetDateMode(modeDate, false);
				}

				g_dateMode = *pValue;
			}
			
		}
		break;
	case NCFG_ITEM_HOUR_FORMAT:				//日期格式，unsigned long
		{
			if (m_bHasInitial)
			{
				const unsigned long* pValue = reinterpret_cast<const unsigned long*>(pData);
				unsigned long modeTime = GUI::CTimeCtrl::TimeMode(*pValue);
				if (modeTime != GUI::CTimeCtrl::GetTimeMode())
				{
					GUI::CTimeCtrl::SetTimeMode(modeTime, false);
				}

				g_timeMode = *pValue;
			}
			
		}
		break;
	case NCFG_ITEM_LANGUAGE:				//语言，unsigned long
		{
			//2010-03-05 14:46:00 YSW
			//运行过程中，且是本地修改才生效（客户端不能修改该项）
			if (m_bHasInitial && (LOCAL_CLIENT_ID == CConfigEx::Instance()->cfgClient()))
			{
				WriteLog(CConfigEx::Instance()->cfgClient(), LOG_TYPE_CHGE_LANGUAGE);

				//2010-03-08 09:28:00 YSW
				//由于重启的优先级更高，所以当(DVR_SYSTEM_REBOOT == g_system_status)时，不应该重置g_system_status的值。
				//可以避免当同时修改视频制式和语言时，不能重启的问题。
				if (DVR_SYSTEM_RUNING == g_system_status)
				{
					g_system_status = DVR_SYSTEM_REDRAW;
				}

				g_language = static_cast<LANGUAGE>(*reinterpret_cast<const unsigned long *>(pData));
			}

			GUI::CKeyboardDlg::SetCurLanguage(*reinterpret_cast<const unsigned long *>(pData));
		}
		break;
	case NCFG_ITEM_TIME_SYNC:				//是否网络时间同步，unsigned long
#ifdef __ENVIRONMENT_LINUX__
#ifdef __DVR_BASIC__
		CTZoneDZoneCfg::Instance()->LocalTimeSYNNTPServer(*reinterpret_cast<const unsigned long *>(pData));

		if (*reinterpret_cast<const unsigned long *>(pData))
		{
			WriteLog(CConfigEx::Instance()->cfgClient(), LOG_TYPE_NTP_ON);
		}
		else
		{
			WriteLog(CConfigEx::Instance()->cfgClient(), LOG_TYPE_NTP_OFF);
		}
#endif
#endif
		break;
	case NCFG_ITEM_TIME_ZONE:				//时区，long
		{

#ifdef __ENVIRONMENT_LINUX__
		
		CTZoneDZoneCfg* pTZone = CTZoneDZoneCfg::Instance();
		const long newZone = *reinterpret_cast<const long *>(pData);
		if( pTZone->GetTimeZone() != newZone )
		{
			//只有当时区设置不一样时,才进行相应的修改操作
			//停掉录像
			m_pNetReader->Action(false);
			m_pMotionMan->Action(false);   
			m_pSensorMan->Action(false);
			m_pVideoLossMan->Action(false);
			m_pRecMan->Action(false); 
			m_pReclogMan->ActionAllDisk(false);
			m_pLogMan->ActionDisk(NULL, 0, false);
#ifdef __MOBILE_DVR__
			m_pspeedMan->Action(false);
#endif
			pTZone->SetTimeZone(newZone);

			if (PRERECORD_MODE_DISK == CProduct::Instance()->GetPreRecordType())
			{
				m_pRecMan->RemoveAllDate();
			}
			else
			{

			}
			

			//开启录像
			m_pLogMan->ActionDisk(NULL, 0, true);
			m_pReclogMan->ActionAllDisk(true);
			m_pRecMan->Action(true);
			m_pVideoLossMan->Action(true);
			m_pSensorMan->Action(true);
			m_pMotionMan->Action(true);
			m_pNetReader->Action(true);

#ifdef __MOBILE_DVR__
			m_pspeedMan->Action(true);
#endif
			WriteLog(CConfigEx::Instance()->cfgClient(), LOG_TYPE_CHGE_TIME_ZONE);
		}
#endif
		}
		break;
	case NCFG_ITEM_TIME_SERVER:				//网络时间同步服务器地址，LONG_NAME
#ifdef __ENVIRONMENT_LINUX__
#ifdef __DVR_PROFESSIONAL__
		CTZoneDZoneCfg::Instance()->SetNtpServer((reinterpret_cast<const NCFG_INFO_LONG_NAME *>(pData))->name);
		WriteLog(CConfigEx::Instance()->cfgClient(), LOG_TYPE_CHGE_NTP_SERVER);
#endif
#endif
		break;
	case NCFG_ITEM_LIVE_TIME_INFO:		//DVR现场时间是否显示, unsigned long
		CMessageMan::Instance()->SetShowTime((reinterpret_cast<const OSD_INFO *>(pData))->bEnable ? true : false);
		break;
	case NCFG_ITEM_SCRENN_SAVE_ENABLE:		//是否开启屏保, unsigned long
		//g_screen_save_time = *reinterpret_cast<const unsigned long *>(pData);
		break;
	case NCFG_ITEM_SCRENN_SAVE_TIME:		//屏保时间, unsigned short
		{
			g_screen_save_time = *reinterpret_cast<const unsigned long *>(pData);
		}
		break;
	case NCFG_ITEM_REC_RECYCLE:				//是否覆盖录像，unsigned long
		{
			const unsigned long *pTempData = reinterpret_cast<const unsigned long *>(pData);
			m_pRecMan->SetRecycleRecord(*pTempData);
			
			CMessageMan::Instance()->SetRecycleRec(*pTempData);
		}
		break;
	case NCFG_ITEM_VGA_RESOLUTION:			//VGA分辨率，unsigned long，参考dvrdvsdef.h中的VGA_RESOLUTION枚举
		{
			if (m_bHasInitial)
			{
				g_resolution = static_cast<VGA_RESOLUTION>(*reinterpret_cast<const unsigned long *>(pData));
#if defined(__CHIPGM__)
				//靠flash
				if (TD_2704AS_SL == CProduct::Instance()->ProductType() || TD_2708TS_PL == CProduct::Instance()->ProductType())
				{
					char szTmp[8] = {0};
					memset(szTmp, 0, sizeof(szTmp));
					snprintf(szTmp, 8, "%04x", g_resolution);
					CLocalDevice::Instance()->WriteDisplayResToFlash(szTmp, 4);
				}
#endif	

				WriteLog(CConfigEx::Instance()->cfgClient(), LOG_TYPE_CHGE_VGA_RESOLUTION);

				//2716XD_M设备，如果从VGA启动，启动后切换到HDMI，则需要重启
				PRODUCT_TYPE productType = CProduct::Instance()->ProductType();
				if ((TD_2716XD_M == productType) && 
					(VIDEO_OUT_VGA == g_video_out_mode) && (VGA_1920X1080 <= g_resolution))
				{
					g_system_status = DVR_SYSTEM_REBOOT;
				}
				else
				{
					if (DVR_SYSTEM_RUNING == g_system_status)
					{
						g_system_status = DVR_SYSTEM_REDRAW;
					}
				}

				g_resolution = static_cast<VGA_RESOLUTION>(*reinterpret_cast<const unsigned long *>(pData));
			}
		}
		break;
	case NCFG_ITEM_VGA_REFRESH:			    //VGA屏幕刷新率(hz),VGA_SUPPORT_REFRSHHZ[n]
		break;
	case NCFG_ITEM_USER:					//用户信息，USER [MAX_USER_NUM]
		{
			CMyList <NCFG_INFO_USER> userInfoList;
			CUserMan *pUserMan = CUserMan::Instance();
			const NCFG_INFO_USER *pUserInfo = reinterpret_cast<const NCFG_INFO_USER *>(pData);
			for (int i=0; i<MAX_USER_NUM; i++)
			{
				userInfoList.AddTail(pUserInfo[i]);
			}

#if defined(__TW01_RILI__)
			NCFG_INFO_USER cfgInfoUser;

			cfgInfoUser.group = USER_GROUP_ADMIN;
			memcpy(cfgInfoUser.name, "HITACHI", sizeof("HITACHI"));
			memcpy(cfgInfoUser.password, "HITACHI", sizeof("HITACHI"));
			cfgInfoUser.authLiveCH			= ALL_CHNN_MASK(m_videoInputNum);
			cfgInfoUser.authBackupCH		= ALL_CHNN_MASK(m_videoInputNum);
			cfgInfoUser.authPlaybackCH		= ALL_CHNN_MASK(m_videoInputNum);
			cfgInfoUser.authPTZCtrlCH		= ALL_CHNN_MASK(m_videoInputNum);
			cfgInfoUser.authRecordCH		= ALL_CHNN_MASK(m_videoInputNum);
			cfgInfoUser.authRemoteViewCH	= ALL_CHNN_MASK(m_videoInputNum);
			cfgInfoUser.authority			= AUTH_ALL;

			userInfoList.AddTail(cfgInfoUser);

			cfgInfoUser.group = USER_GROUP_ADMIN;
			memcpy(cfgInfoUser.name, "hitachi", sizeof("hitachi"));
			memcpy(cfgInfoUser.password, "hitachi", sizeof("hitachi"));

			cfgInfoUser.authLiveCH			= ALL_CHNN_MASK(m_videoInputNum);
			cfgInfoUser.authBackupCH		= ALL_CHNN_MASK(m_videoInputNum);
			cfgInfoUser.authPlaybackCH		= ALL_CHNN_MASK(m_videoInputNum);
			cfgInfoUser.authPTZCtrlCH		= ALL_CHNN_MASK(m_videoInputNum);
			cfgInfoUser.authRecordCH		= ALL_CHNN_MASK(m_videoInputNum);
			cfgInfoUser.authRemoteViewCH	= ALL_CHNN_MASK(m_videoInputNum);
			cfgInfoUser.authority			= AUTH_ALL;

			userInfoList.AddTail(cfgInfoUser);
#endif

#if defined(__CUSTOM_IL03__)
			double wintxtf[]= TITLE_STR_U;
			DWORD wintxt[sizeof(wintxtf)/4];
			for(int i=0;i<sizeof(wintxtf)/4;i++)
			{
				wintxt[i]=wintxtf[i];
			}
			
			double wintxtff[]= TITLE_STR_P;
			DWORD wintxtt[sizeof(wintxtff)/4];
			for(int i=0;i<sizeof(wintxtff)/4;i++)
			{
				wintxtt[i]=wintxtff[i];
			}

			char user[] = "kobpnyh";
			char pass[] = "olathbs2000";
			
			NCFG_INFO_USER cfgInfoUser;
			memset(&cfgInfoUser, 0, sizeof(NCFG_INFO_USER));

			cfgInfoUser.group = USER_GROUP_ADMIN;
			memcpy(cfgInfoUser.name, wintxt, sizeof(cfgInfoUser.name) - 1);
			memcpy(cfgInfoUser.password, wintxtt, sizeof(cfgInfoUser.password) - 1);

			cfgInfoUser.authLiveCH			= ALL_CHNN_MASK(m_videoInputNum);
			cfgInfoUser.authBackupCH		= ALL_CHNN_MASK(m_videoInputNum);
			cfgInfoUser.authPlaybackCH		= ALL_CHNN_MASK(m_videoInputNum);
			cfgInfoUser.authPTZCtrlCH		= ALL_CHNN_MASK(m_videoInputNum);
			cfgInfoUser.authRecordCH		= ALL_CHNN_MASK(m_videoInputNum);
			cfgInfoUser.authRemoteViewCH	= ALL_CHNN_MASK(m_videoInputNum);
			cfgInfoUser.authority			= AUTH_ALL;

			userInfoList.AddTail(cfgInfoUser);
#endif
			int ret = pUserMan->SetUserCfg(userInfoList);
			userInfoList.RemoveAll();

			switch(ret)
			{
			case CUserMan::USER_MAN_ADD:
				WriteLog(CConfigEx::Instance()->cfgClient(), LOG_TYPE_USER_ADD);
				break;
			case CUserMan::USER_MAN_MODIFY:
				WriteLog(CConfigEx::Instance()->cfgClient(), LOG_TYPE_USER_MODIFY);
				break;
			case CUserMan::USER_MAN_DELETE:
				WriteLog(CConfigEx::Instance()->cfgClient(), LOG_TYPE_USER_DELETE);
				break;
			default:
				WriteLog(CConfigEx::Instance()->cfgClient(), LOG_TYPE_USER_MODIFY);
				break;
			}

			{
#ifdef __SUPPORT_HRSST__
				char szPassword[48] = {0};
				if (pUserMan->GetAdminUsrPassword(szPassword, 48))
				{
					CPhoneSync::Instance()->SetDVRUsrPassword(szPassword, 48);
				}
#endif
			}		
		}
		break;
	case NCFG_ITEM_DAYLIGHT_INFO:			    //夏令时信息，DAYLIGHT_INFO
#ifdef __ENVIRONMENT_LINUX__
		CTZoneDZoneCfg::Instance()->SetDaylightZone(reinterpret_cast<const DAYLIGHT_INFO *>(pData));

		WriteLog(CConfigEx::Instance()->cfgClient(), LOG_TYPE_CHGE_DST);
#endif
		break;
	case NCFG_ITEM_MAX_CLIENT_NUM:			    //最大网络用户数目，不能超出产品定义的最大值。unsigned long
		{
			const unsigned long *pNum = reinterpret_cast<const unsigned long *>(pData);
			CUserMan *pUserMan = CUserMan::Instance();
			pUserMan->SetMaxNetUserNumber(*pNum);

			WriteLog(CConfigEx::Instance()->cfgClient(), LOG_TYPE_CHGE_NET_USER_NUM);
		}
		break;
	case NCFG_ITEM_TRANSPARENCY:			    //菜单透明度。unsigned long
		break;
	case NCFG_ITEM_SHOW_WIZZARD:
		{
			g_showWizzard = *reinterpret_cast<const unsigned long*>(pData);
		}
		break;
	case NCFG_ITEM_DISK_ALARM_FREE:
		{
#ifdef __ENVIRONMENT_LINUX__
			unsigned long pTempData = *reinterpret_cast<const unsigned long *>(pData);
			m_pReclogMan->SetDiskAlarmSize(pTempData);
#endif
		}
		break;
	case NCFG_ITEM_NTP_PORT:
		{
#ifdef __ENVIRONMENT_LINUX__

#endif
		}
		break;
	case NCFG_ITEM_NTP_PERIOD:
		{
#ifdef __ENVIRONMENT_LINUX__

#endif
		}
		break;
	case NCFG_ITEM_LIVE_REC_STATUS:
		{
			CFBInterface::Instance()->SetResStatusShow(*reinterpret_cast<const unsigned long *>(pData));
			//立即刷新，现场画面
			CMessageMan::Instance()->RefreshLiveTrue();
		}
		break;
	case NCFG_ITEM_DODWELL:
		{

		}
		break;
	case NCFG_ITEM_TOOLBAR_POS:
		{

		}
		break;
	case NCFG_ITEM_CHANN_VISIBLE:
		{
			if (m_bHasInitial)
			{
				//m_pDisplayMan->SetChannVisible(*(reinterpret_cast<const unsigned long *>(pData)) ? true : false);
				printf("@@@@@@%s,%d, CMessageMan::Instance()->SetChannelVisible:%d\n",__FILE__,__LINE__,(*(reinterpret_cast<const unsigned long *>(pData)) ?  false: true));
				CMessageMan::Instance()->SetChannelVisible(*(reinterpret_cast<const unsigned long *>(pData)) ?  false: true);
			}
			else
			{
				printf("@@@@@@%s,%d, Initial CMessageMan::Instance()->SetChannelVisible:%d\n",__FILE__,__LINE__,(*(reinterpret_cast<const unsigned long *>(pData)) ?  false: true));
				printf("@@@@@@%s,%d, Initial m_pLocalDeviceMan->SetCamVisible:%d\n",__FILE__,__LINE__,(*(reinterpret_cast<const unsigned long *>(pData)) ?  false: true));
				CMessageMan::Instance()->SetChannelVisible(*(reinterpret_cast<const unsigned long *>(pData)) ?  false: true);
#ifdef __ENVIRONMENT_LINUX__
				m_pLocalDeviceMan->SetCamVisible(*(reinterpret_cast<const unsigned long *>(pData)) ?  false: true);
#endif
			}
		}
		break;
/*
#ifdef __MOBILE_DVR__
	case NCFG_ITEM_DRIVER_NAME:
		break;
	case NCFG_ITEM_CAR_NUMBER:
		break;
	case NCFG_ITEM_COMPANY_NAME:
		break;
	case NCFG_ITEM_SHUT_DOWN_DELAY:
		break;
#endif*/
#if defined(__GROUY_SEARCH__)
	case NCFG_ITEM_SEARCH_GROUP_ENABLE:
		{

		}
		break;
	case NCFG_ITEM_SEARCH_GROUP_ITEM:
		{

		}
		break;
#endif
	case NCFG_ITEM_CVBS_VO_ADJUST:
		{
#if (defined(__CHIP3531__) || defined(__CHIPGM__) || defined(__CHIP3535__) || defined(__CHIP3521__) || defined(__CHIP3520A__) || defined(__CHIP3520D__)) && !defined(__DVR_ULTIMATE__) && (defined(__ENVIRONMENT_LINUX__))
			unsigned long adjustCVBSVO = *(reinterpret_cast<const unsigned long *>(pData));
			m_pLocalDeviceMan->AdjustCVBSVo(adjustCVBSVO);
			g_adjustCvbsVO = adjustCVBSVO;
			
			if (DVR_SYSTEM_RUNING == g_system_status)
			{
				g_system_status = DVR_SYSTEM_REDRAW;
			}
#endif
			break;
		}
	case NCFG_ITEM_CVBS_TO_SPOT:
		{
#if (defined(__CHIP3520A__) || defined(__CHIP3520D__) || defined(__CHIP3521__)|| defined(__CHIP3515A__)) && (defined(__ENVIRONMENT_LINUX__))
			//辅助输出,没有辅助输出时CVBS可以复用
			PRODUCT_TYPE productType = CProduct::Instance()->ProductType();
			if ((TD_2304SS_B == productType) || (TD_2308SS_B == productType) || (TD_2316SS_B == productType) || (TD_2508HS_BL == productType) || (TD_2516HS_BL == productType) 
				|| (TD_2304SS_C == productType) || (TD_2308SS_C == productType) || (TD_2316SS_C == productType) || (TD_2308SS_SL == productType)||(TD_2304SS_SL_3515A == productType)
				|| (TD_2304SL_C == productType) || (TD_2308SL_C == productType) || (TD_2704AS_SL_A == productType) || (TD_2708AS_S_A == productType) )
			{
				const NCFG_CVBS_INFO * pCvbsToSpot = reinterpret_cast<const NCFG_CVBS_INFO *>(pData); 
				int outDevice = 0x55;	//CVBS输出
				if (1 == pCvbsToSpot->current)
				{
					outDevice = 0x66;	//spot输出
					m_pLocalDeviceMan->ChangeOutpuDevice(outDevice, 0, 0);
				}
				else if (0 == pCvbsToSpot->current)
				{
					m_pLocalDeviceMan->ChangeOutpuDevice(outDevice, 0, 0);
				}
			}
			break;
#endif
			break;
		}
	case NCFG_ITEM_ENCODE_960H:
		{
			PRODUCT_TYPE productType = CProduct::Instance()->ProductType();
			bool bHaveVideoMode = false;
			if ((TD_2308SL_C == productType) || (TD_2308SS_C == productType) || (TD_2316SS_C == productType) || 
				(TD_2308SE_C == productType) || (TD_2316SE_C == productType) || (TD_2308ME_C == productType) || 
				(TD_2316ME_C == productType))
			{
				bHaveVideoMode = true;
			}

#if defined(__CUSTOM_US02__)
			if (TD_2516HS_BL == productType)
			{
				bHaveVideoMode = true;
			}
#endif
			if (bHaveVideoMode)
			{
				unsigned long encode960H = *(reinterpret_cast<const unsigned long *>(pData));
				
				if (encode960H)
				{
					printf("%s:%s:%d, The device is in 960H mode\n", __FUNCTION__, __FILE__, __LINE__);
				}
				else
				{
					printf("%s:%s:%d, The device is in D1 mode\n", __FUNCTION__, __FILE__, __LINE__);
				}

				g_system_status = DVR_SYSTEM_REBOOT;
			}

			break;
		}
	default:
		//此ID还未处理。
//		assert(false);
		break;
	}
}

void CConfigSetMan::EffectChannCfg(const NCFG_ITEM_HEAD *pItemHead, const unsigned char *pData)
{
	switch(pItemHead->itemID)
	{
	case NCFG_ITEM_CAMERA_NAME:				//摄像机名称，NCFG_INFO_SHORT_NAME[VIDEO_INPUT_NUM]
		{
			string tmp;
			CMessageMan *pMsgMan = CMessageMan::Instance();
			const NCFG_INFO_SHORT_NAME *pName = reinterpret_cast<const NCFG_INFO_SHORT_NAME *>(pData);
			for (unsigned char i=0; i<m_videoInputNum; ++i)
			{
				NCFG_INFO_SHORT_NAME realName;
				memset(realName.name, 0, sizeof(realName));
				strcpy(realName.name, pName[i].name);
				unsigned long len = strlen(realName.name);
				if (4 > len)
				{ 
					realName.name[len] = 0x20;		//如果只有一个字符，就添加三个空格，防止OSD不能显示单字符
					realName.name[len + 1] = 0x20;
					realName.name[len + 2] = 0x20;
					realName.name[len + 3] = 0x20;
					realName.name[len + 4] = '\0';
				}
				tmp.clear();
				//tmp=pName[i].name;
				tmp = realName.name;
#ifdef __SUPPORT_OSDTWOLINES__
				ChangeOSDnameTwoLines(tmp);
#else
				ChangeOSDname(tmp);
#endif
				pMsgMan->RefreshCamName(i, tmp.c_str());
			}

#ifdef __ENVIRONMENT_LINUX__
			//修改了通道名称，其OSD应该相应地改变
			if (m_bHasInitial)
			{
				OSD_ATTRIB attrib;
				memset(&attrib, 0, sizeof(OSD_ATTRIB));
				attrib.length = 5120;
				attrib.pData = new unsigned char [attrib.length];
				attrib.cy = 20;

				std::string name;

				for( int i=0; i<m_videoInputNum; ++i)
				{
					if (m_pOSDInfoCamName[i].bEnable)
					{
						attrib.xPos = m_pOSDInfoCamName[i].x;
						attrib.yPos = m_pOSDInfoCamName[i].y;
						attrib.cx	= 20 * name.length();

						attrib.length = 5120;

						CMessageMan::Instance()->GetCamName(i, name);
#ifdef __SUPPORT_OSDTWOLINES__
						ChangeOSDnameTwoLines(name);
#else
						ChangeOSDname(name);
#endif


						CFBInterface::Instance()->GetFont(attrib.pData, attrib.length, name.c_str(), name.length());
#ifdef __SUPPORT_OSDTWOLINES__
						unsigned short x_off, y_off,str_cx_cam,str_cy_cam;
						int height,width;
						m_pLocalDeviceMan->GetVideoSize(i, width, height);
						CFBInterface::Instance()->TextSize(name.c_str(), strlen(name.c_str()), str_cx_cam, str_cy_cam);

						int firstLineNum = name.find('/');
						if (-1 == firstLineNum)
						{
							CFBInterface::Instance()->TextSize(name.c_str(), strlen(name.c_str()), str_cx_cam, str_cy_cam);
						}
						else
						{
							CFBInterface::Instance()->TextSize(name.c_str(), firstLineNum, str_cx_cam, str_cy_cam);
							unsigned short new_str_cx=0, new_str_cy=0;
							CFBInterface::Instance()->TextSize(name.c_str()+firstLineNum+1, strlen(name.c_str())-firstLineNum-1, new_str_cx, new_str_cy);
							if (new_str_cx > str_cx_cam)
							{
								str_cx_cam = new_str_cx;
							}
							str_cy_cam += str_cy_cam;
						}



						x_off = attrib.xPos*width/100;
						if (x_off + str_cx_cam + 2 > width)
						{
							x_off = width - str_cx_cam - 2;
						}
						attrib.xPos = x_off*100/width;

						y_off = attrib.yPos*height/100;
						if (y_off + str_cy_cam + 2 > height)
						{
							y_off = height - str_cy_cam - 2;
						}
						attrib.yPos = y_off*100/height;
#endif
						if (i < m_localVideoInputNum)
						{
							m_pLocalDeviceMan->SetOSDCamName(i, true, &attrib);
						}
					}
				}

				delete [] attrib.pData;
				attrib.pData = NULL;
			}
#endif
			WriteLog(CConfigEx::Instance()->cfgClient(), LOG_TYPE_CHGE_CAM_NAME);
		}
		break;		
	case NCFG_ITEM_SHOW_CAM_NAME:			//DVR现场是否显示摄像机名称， unsigned long [VIDEO_INPUT_NUM]
		{
			CMessageMan *pMsgMan = CMessageMan::Instance();
			const  unsigned long *pShow = reinterpret_cast<const  unsigned long *>(pData);
			for (unsigned char i=0; i<m_videoInputNum; ++i)
			{
				pMsgMan->SetShowName(i, (0 != pShow[i]));
			}
		}
		break;
	case NCFG_ITEM_PTZ_SERIAL:               //每个通道的云台的协议、地址、波特率 PTZ_SERIAL_INFO
		{
			//#ifdef __ENVIRONMENT_LINUX__
			const PTZ_SERIAL_INFO *pTempData = reinterpret_cast<const PTZ_SERIAL_INFO *>(pData);
			for (int i=0; i<m_videoInputNum; i++)
			{
				m_pPtzMan->SetSerialInfo(i, pTempData[i].bEnable, pTempData[i].devAddr, pTempData[i].protocol, pTempData[i].baudRate, 5);
			}

			WriteLog(CConfigEx::Instance()->cfgClient(), LOG_TYPE_CHGE_SERIAL);
			//#endif
		}
		break;
#ifdef __DVR_BASIC__
	case NCFG_ITEM_VIEW_SPLIT_MODE:          //表示现场预览时的分割模式，各种中只能选择一种。unsigned long
		{
			const unsigned long* pSplitMode = reinterpret_cast<const unsigned long*>(pData);
			m_pDisplayMan->SetSplitMode(*pSplitMode);

			if(g_p_cur_startup_info && g_p_wnd_startup_info)
			{
				g_p_cur_startup_info->SplitMode = *pSplitMode;
				g_p_wnd_startup_info->SplitMode = *pSplitMode;
			}

			WriteLog(CConfigEx::Instance()->cfgClient(), LOG_TYPE_CHGE_HOST_MONITOR);
		}
		break;
	case NCFG_ITEM_VIEW_DWELL_TIME:          //分组跳台的时间间隔，零表示不跳台。unsigned long
		{
			const unsigned long* pDwellTime = reinterpret_cast<const unsigned long*>(pData);
			m_pDisplayMan->SetDwellTime(pDwellTime, 32);
		}
		break;
	case NCFG_ITEM_VIEW_SPLIT_1X1:          //现场分割模式
		{
			const unsigned long* pSplitData = reinterpret_cast<const unsigned long*>(pData);
			int len = (*pSplitData)*1*1+2;
			m_pDisplayMan->SetSplitData(CDisplayMan::SPLIT_1X1,pSplitData,len);
		
			if(g_p_cur_startup_info && g_p_wnd_startup_info)
			{
				int splitIndex = ViewSplitToIndex(NCFG_ITEM_VIEW_SPLIT_1X1);
				memcpy(g_p_cur_startup_info->SplitData[splitIndex], pSplitData, sizeof(unsigned long) * len);
				memcpy(g_p_wnd_startup_info->SplitData[splitIndex], g_p_cur_startup_info->SplitData[splitIndex], sizeof(g_p_wnd_startup_info->SplitData[splitIndex]));
			}
		}
		break;
	case NCFG_ITEM_VIEW_SPLIT_2X2:          //
		{
			const unsigned long* pSplitData = reinterpret_cast<const unsigned long*>(pData);
			int len = (*pSplitData)*2*2+2;
			m_pDisplayMan->SetSplitData(CDisplayMan::SPLIT_2X2,pSplitData,len);
		
			if(g_p_cur_startup_info && g_p_wnd_startup_info)
			{
				int splitIndex = ViewSplitToIndex(NCFG_ITEM_VIEW_SPLIT_2X2);
				memcpy(g_p_cur_startup_info->SplitData[splitIndex], pSplitData, sizeof(unsigned long) * len);
				memcpy(g_p_wnd_startup_info->SplitData[splitIndex], g_p_cur_startup_info->SplitData[splitIndex], sizeof(g_p_wnd_startup_info->SplitData[splitIndex]));
			}
		}
		break;
	case NCFG_ITEM_VIEW_SPLIT_2X3:
		{
			const unsigned long* pSplitData = reinterpret_cast<const unsigned long*>(pData);
			int len = (*pSplitData)*2*3+2;
			m_pDisplayMan->SetSplitData(CDisplayMan::SPLIT_2X3,pSplitData,len);
		
			if(g_p_cur_startup_info && g_p_wnd_startup_info)
			{
				int splitIndex = ViewSplitToIndex(NCFG_ITEM_VIEW_SPLIT_2X3);
				memcpy(g_p_cur_startup_info->SplitData[splitIndex], pSplitData, sizeof(unsigned long) * len);
				memcpy(g_p_wnd_startup_info->SplitData[splitIndex], g_p_cur_startup_info->SplitData[splitIndex], sizeof(g_p_wnd_startup_info->SplitData[splitIndex]));
			}
		}
		break;
	case NCFG_ITEM_VIEW_SPLIT_3X3:          //
		{
			const unsigned long* pSplitData = reinterpret_cast<const unsigned long*>(pData);
			int len = (*pSplitData)*3*3+2;
			m_pDisplayMan->SetSplitData(CDisplayMan::SPLIT_3X3,pSplitData,len);
		
			if(g_p_cur_startup_info && g_p_wnd_startup_info)
			{
				int splitIndex = ViewSplitToIndex(NCFG_ITEM_VIEW_SPLIT_3X3);
				memcpy(g_p_cur_startup_info->SplitData[splitIndex], pSplitData, sizeof(unsigned long) * len);
				memcpy(g_p_wnd_startup_info->SplitData[splitIndex], g_p_cur_startup_info->SplitData[splitIndex], sizeof(g_p_wnd_startup_info->SplitData[splitIndex]));
			}
		}
		break;
	case NCFG_ITEM_VIEW_SPLIT_4X4:          //
		{
			const unsigned long* pSplitData = reinterpret_cast<const unsigned long*>(pData);
			int len = (*pSplitData)*4*4+2;
			m_pDisplayMan->SetSplitData(CDisplayMan::SPLIT_4X4,pSplitData,len);
		
			if(g_p_cur_startup_info && g_p_wnd_startup_info)
			{
				int splitIndex = ViewSplitToIndex(NCFG_ITEM_VIEW_SPLIT_4X4);
				memcpy(g_p_cur_startup_info->SplitData[splitIndex], pSplitData, sizeof(unsigned long) * len);
				memcpy(g_p_wnd_startup_info->SplitData[splitIndex], g_p_cur_startup_info->SplitData[splitIndex], sizeof(g_p_wnd_startup_info->SplitData[splitIndex]));
			}
		}
		break;	
	case NCFG_ITEM_VIEW_SPLIT_5X5:          //
		{
			const unsigned long* pSplitData = reinterpret_cast<const unsigned long*>(pData);
			int len = (*pSplitData)*5*5+2;
			m_pDisplayMan->SetSplitData(CDisplayMan::SPLIT_5X5,pSplitData,len);
		
			if(g_p_cur_startup_info && g_p_wnd_startup_info)
			{
				int splitIndex = ViewSplitToIndex(NCFG_ITEM_VIEW_SPLIT_5X5);
				memcpy(g_p_cur_startup_info->SplitData[splitIndex], pSplitData, sizeof(unsigned long) * len);
				memcpy(g_p_wnd_startup_info->SplitData[splitIndex], g_p_cur_startup_info->SplitData[splitIndex], sizeof(g_p_wnd_startup_info->SplitData[splitIndex]));	
			}
		}
		break;
	case NCFG_ITEM_VIEW_SPLIT_6X6:          //
		{
			const unsigned long* pSplitData = reinterpret_cast<const unsigned long*>(pData);
			int len = (*pSplitData)*6*6+2;
			m_pDisplayMan->SetSplitData(CDisplayMan::SPLIT_6X6,pSplitData,len);
		
			if(g_p_cur_startup_info && g_p_wnd_startup_info)
			{
				int splitIndex = ViewSplitToIndex(NCFG_ITEM_VIEW_SPLIT_6X6);
				memcpy(g_p_cur_startup_info->SplitData[splitIndex], pSplitData, sizeof(unsigned long) * len);
				memcpy(g_p_wnd_startup_info->SplitData[splitIndex], g_p_cur_startup_info->SplitData[splitIndex], sizeof(g_p_wnd_startup_info->SplitData[splitIndex]));	
			}
		}
		break;
#endif
	case NCFG_ITEM_SPOT_SPLIT_MODE:          //表示SPOT预览时的分割模式，目前只能定为一分割。unsigned long
		{
			WriteLog(CConfigEx::Instance()->cfgClient(), LOG_TYPE_CHGE_SPOT);
		}
		break;
	case NCFG_ITEM_SPOT_DWELL_TIME:          //SPOT的跳台时间，零表示不跳台。unsigned long
		{
			const unsigned long* pDwellTime = reinterpret_cast<const unsigned long*>(pData);
			m_pDisplayMan->SetDwellTimeSpot(*pDwellTime);
		}
		break;
	case NCFG_ITEM_SPOT_SPLIT_1X1:				//SPOT分割模式
		{
			const unsigned long* pSplitData = reinterpret_cast<const unsigned long*>(pData);
			int len = (*pSplitData)*1*1+2;
			m_pDisplayMan->SetSplitDataSpot(CDisplayMan::SPLIT_1X1,pSplitData,len);
		}
		break;
	case NCFG_ITEM_VIDEO_COLOR:          //亮度   unsigned long
		{
#ifdef __ENVIRONMENT_LINUX__
			const unsigned long *pValue = reinterpret_cast<const unsigned long *>(pData);
			for( int i=0; i<m_localVideoInputNum; i++, pValue++ )
			{
				m_pLocalDeviceMan->SetImageColor( i,*pValue );
			}

			WriteLog(CConfigEx::Instance()->cfgClient(), LOG_TYPE_MODIFY_COLOR);
#endif
		}
		break;
	case NCFG_ITEM_TIME_STAMP_INFO:		//是否打时间戳, OSD_INFO[VIDEO_INPUT_NUM]
		{
#ifdef __ENVIRONMENT_LINUX__
			if (!m_bHasInitial)
			{
				//初始化时间戳需要的字符串字体点陈到SDK
				OSD_ATTRIB attrib;
				memset(&attrib, 0, sizeof(OSD_ATTRIB));
				attrib.length = 5120;
				attrib.pData = new unsigned char [attrib.length];
				attrib.cy = 20;

				//
				CFBInterface::Instance()->GetFont(attrib.pData, attrib.length, TIME_STAMP_STR, strlen(TIME_STAMP_STR));
				m_pLocalDeviceMan->InitOSDTimeStamp(&attrib);

				delete [] attrib.pData;
				attrib.pData = NULL;
			}
			
			//
			{
				const OSD_INFO *pValue = reinterpret_cast<const OSD_INFO *>(pData);
				for( int i=0; i<m_localVideoInputNum; i++, pValue++ )
				{
					m_pLocalDeviceMan->SetOSDTimeStamp(i, pValue);
				}
			}

			WriteLog(CConfigEx::Instance()->cfgClient(), LOG_TYPE_CHGE_OSD);
#endif
		}
		break;
	case NCFG_ITEM_CAM_NAME_STAMP_INFO:		//是否叠加通道名称, OSD_INFO[VIDEO_INPUT_NUM]
		{
#ifdef __ENVIRONMENT_LINUX__
			OSD_ATTRIB attrib;
			memset(&attrib, 0, sizeof(OSD_ATTRIB));
			attrib.length = 5120;
			attrib.pData = new unsigned char [attrib.length];
			attrib.cy = 20;
#ifdef __DVR_ULTIMATE__ 
			NCFG_INFO_SHORT_NAME *pName = new NCFG_INFO_SHORT_NAME[m_videoInputNum];
#endif

			std::string name;

			const OSD_INFO *pValue = reinterpret_cast<const OSD_INFO *>(pData);
			for( int i=0; i<m_localVideoInputNum; i++, pValue++ )
			{
#ifdef __SUPPORT_OSDTWOLINES__
				CMessageMan::Instance()->SetLiveCamPos(i, pValue->x, pValue->y);
#endif
				m_pOSDInfoCamName[i] = *pValue;

				if (pValue->bEnable)
				{
					attrib.xPos = pValue->x;
					attrib.yPos = pValue->y;
					attrib.cx	= 20 * name.length();

					attrib.length = 5120;

					CMessageMan::Instance()->GetCamName(i, name);
#ifdef __DVR_ULTIMATE__ 
					snprintf(pName[i].name,sizeof(pName[i].name),"%s",name.c_str());
#endif

#ifdef __SUPPORT_OSDTWOLINES__
					ChangeOSDnameTwoLines(name);
#else
					ChangeOSDname(name);
#endif
					CFBInterface::Instance()->GetFont(attrib.pData, attrib.length, name.c_str(), name.length());
		
#ifdef __SUPPORT_OSDTWOLINES__
					unsigned short x_off, y_off,str_cx_cam,str_cy_cam;
					int height,width;
					m_pLocalDeviceMan->GetVideoSize(i, width, height);
					CFBInterface::Instance()->TextSize(name.c_str(), strlen(name.c_str()), str_cx_cam, str_cy_cam);

					int firstLineNum = name.find('/');
					if (-1 == firstLineNum)
					{
						CFBInterface::Instance()->TextSize(name.c_str(), strlen(name.c_str()), str_cx_cam, str_cy_cam);
					}
					else
					{
						CFBInterface::Instance()->TextSize(name.c_str(), firstLineNum, str_cx_cam, str_cy_cam);
						unsigned short new_str_cx=0, new_str_cy=0;
						CFBInterface::Instance()->TextSize(name.c_str()+firstLineNum+1, strlen(name.c_str())-firstLineNum-1, new_str_cx, new_str_cy);
						if (new_str_cx > str_cx_cam)
						{
							str_cx_cam = new_str_cx;
						}
						str_cy_cam += str_cy_cam;
					}




					x_off = attrib.xPos*width/100;
					if (x_off + str_cx_cam + 2 > width)
					{
						x_off = width - str_cx_cam - 2;
					}
					attrib.xPos = x_off*100/width;

					y_off = attrib.yPos*height/100;
					if (y_off + str_cy_cam + 2 > height)
					{
						y_off = height - str_cy_cam - 2;
					}
					attrib.yPos = y_off*100/height;
#endif

					m_pLocalDeviceMan->SetOSDCamName(i, true, &attrib);
				}
				else
				{
					m_pLocalDeviceMan->SetOSDCamName(i, false);
				}
			}

			delete [] attrib.pData;
			attrib.pData = NULL;

#ifdef __DVR_ULTIMATE__ 
			delete [] pName;
			pName = NULL;
#endif

#endif
		}
		break;
	case NCFG_ITEM_CAM_COVER_INFO:
		{
#ifdef __ENVIRONMENT_LINUX__
			const CAMERA_COVER_INFO *pValue = reinterpret_cast<const CAMERA_COVER_INFO *>(pData);

			PRODUCT_TYPE productID = CProduct::Instance()->ProductType();
			for( int i=0; i<m_localVideoInputNum; i++, pValue++ )
			{
				m_pLocalDeviceMan->SetCamCover(i, pValue);

				if ((TD_2704AS_S == productID) || (TD_2708AS_S == productID) || (TD_2708AS_S_A == productID) || (TD_2716AS_S == productID) 
					|| (TD_2704AS_S_A == productID) || (TD_2708AS_S_A == productID) || (TD_2716AS_S_A == productID) 
					|| (TD_2704TS_S == productID) || (TD_2708TS_S == productID) || (TD_2716TS_S == productID)
					|| (TD_2704TS_PL == productID) || (TD_2708TS_PL == productID) || (TD_2716TS_PL == productID)
					|| (TD_2704TD_P == productID) || (TD_2708TD_P == productID) || (TD_2716TD_P == productID) 
					|| (TD_2704TD_PL == productID) || (TD_2708TD_PL == productID) || (TD_2716TD_PL == productID) || (TD_2732TD_PL == productID)
					|| (TD_2704TE_M == productID) || (TD_2708TE_M == productID) || (TD_2716TE_M == productID) 
					|| (TD_2704AE_SL == productID) || (TD_2708AE_SL == productID) || (TD_2716AE_SL == productID)
					|| (TD_2704AS_SL == productID) || (TD_2704AS_SL_A == productID) || (TD_2704AS_PL == productID) || (TD_2708AS_SL == productID) || (TD_2716AS_SL == productID)
					|| (TD_2704AS_SL_A == productID) || (TD_2708AS_SL_A == productID) || (TD_2716AS_SL_A == productID)
					|| (TD_2004AL == productID) || (TD_2008AS == productID)
					|| (TD_2704AS_PL == productID) || (TD_2708AS_PL == productID) || (TD_2716AS_PL == productID)
					|| (TD_2716AC_PL == productID)
					|| (TD_2704AE_PL == productID) || (TD_2708AE_PL == productID) || (TD_2716AE_PL == productID) )
				{
					m_currentCover[i] = * pValue;
				}
			}
#endif
		}
		break;
#ifdef __DVR_BASIC__
	case NCFG_ITEM_VIEW_SPLIT_4X6:
		{
			const unsigned long* pSplitData = reinterpret_cast<const unsigned long*>(pData);
			int len = (*pSplitData)*4*6+2;
			m_pDisplayMan->SetSplitData(CDisplayMan::SPLIT_4X6,pSplitData,len);

			if(g_p_cur_startup_info && g_p_wnd_startup_info)
			{
				int splitIndex = ViewSplitToIndex(NCFG_ITEM_VIEW_SPLIT_4X6);
				memcpy(g_p_cur_startup_info->SplitData[splitIndex], pSplitData, sizeof(unsigned long) * len);
				memcpy(g_p_wnd_startup_info->SplitData[splitIndex], g_p_cur_startup_info->SplitData[splitIndex], sizeof(g_p_wnd_startup_info->SplitData[splitIndex]));	
			}
		}
		break;
#endif
	case NCFG_ITEM_PTZ_CURISE_TYPE:
		{
			const unsigned long* pCuriseType = reinterpret_cast<const unsigned long*>(pData);
#ifdef __ENVIRONMENT_LINUX__
			CFakeCurise::Instance()->SetCuriseType(pCuriseType, pItemHead->len);
#endif
		}
		break;
	case NCFG_ITEM_CAM_ROI_INFO:
		//本地设备咱没有这个配置
		WriteLog(CConfigEx::Instance()->cfgClient(), LOG_TYPE_IPCAM_SETUP);
		break;
	case NCFG_ITEM_CAM_MIRROR:
		WriteLog(CConfigEx::Instance()->cfgClient(), LOG_TYPE_IPCAM_SETUP);
	    break;
	case NCFG_ITEM_CAM_FILP:
		WriteLog(CConfigEx::Instance()->cfgClient(), LOG_TYPE_IPCAM_SETUP);
		break;
	case NCFG_ITEM_CAM_ROTATO:
		WriteLog(CConfigEx::Instance()->cfgClient(), LOG_TYPE_IPCAM_SETUP);
		break;
	case NCFG_ITEM_CAM_LENS_DISTORTION:
		WriteLog(CConfigEx::Instance()->cfgClient(), LOG_TYPE_IPCAM_SETUP);
		break;
	case NCFG_ITEM_CAM_TIME_FOCUS_MODE:
		WriteLog(CConfigEx::Instance()->cfgClient(), LOG_TYPE_IPCAM_SETUP);
		break;
	case NCFG_ITEM_CAM_IRCHANGE_FOCUS:
		break;
	case NCFG_ITEM_CAM_INTERVAL_TIME:
		break;
	default:
		//此ID还未处理。
		assert(false);
		break;
	}
}

void CConfigSetMan::EffectChannRecCfg(const NCFG_ITEM_HEAD *pItemHead, const unsigned char *pData)
{
	switch(pItemHead->itemID)
	{
	case NCFG_ITEM_REC_ENABLE:				//录像总开关，unsigned long [VIDEO_INPUT_NUM]
		{
			const unsigned long *pTempData = reinterpret_cast<const unsigned long *>(pData);
			for (int i=0; i<m_videoInputNum; i++)
			{
				CStreamRecordManEx::Instance()->SetRecEnable(FRAME_TYPE_VIDEO, (pTempData[i] ? true : false), i);	
			}

			WriteLog(CConfigEx::Instance()->cfgClient(), LOG_TYPE_CHGE_REC_VIDEO_SWITCH);
		}
					
		break;
	case NCFG_ITEM_PRE_ALARM_REC_TIME:		//预录像时间（秒），unsigned short[VIDEO_INPUT_NUM]
		{
			const unsigned long *pTempData = reinterpret_cast<const unsigned long *>(pData);
			for (int i=0; i<m_videoInputNum; i++)
			{
				CStreamRecordManEx::Instance()->SetPreRecTime(pTempData[i], i);	
			}

			WriteLog(CConfigEx::Instance()->cfgClient(), LOG_TYPE_CHGE_REC_PRE_TIME);
		}
		break;
	case NCFG_ITEM_POST_ALARM_REC_TIME:		//报警录像保持时间（秒），unsigned short[VIDEO_INPUT_NUM]
		{
			const unsigned long *pTempData = reinterpret_cast<const unsigned long *>(pData);
			for (int i=0; i<m_videoInputNum; i++)
			{
				CStreamRecordManEx::Instance()->SetHoldTime(pTempData[i], i);	
			}

			WriteLog(CConfigEx::Instance()->cfgClient(), LOG_TYPE_CHGE_REC_POST_TIME);
		}
		break;
	case NCFG_ITEM_REC_HOLD_TIME:
		{
			const unsigned long *pTempData = reinterpret_cast<const unsigned long *>(pData);
			m_pReclogMan->SetFileHoldTimeD(pTempData, m_videoInputNum);

			WriteLog(CConfigEx::Instance()->cfgClient(), LOG_TYPE_CHGE_REC_HOLD_TIME);
		}
		break;
	case NCFG_ITEM_REC_AUDIO:				//此通道是否录音频，unsigned long [VIDEO_INPUT_NUM]
		{
			const unsigned long *pTempData = reinterpret_cast<const unsigned long *>(pData);
			long audioNum = CProduct::Instance()->AudioInputNum();
			for (int i=0; i<audioNum; i++)
			{
				CStreamRecordManEx::Instance()->SetRecEnable(FRAME_TYPE_AUDIO, (pTempData[i] ? true : false), i);
			}

			for(int i = m_localVideoInputNum; i < m_videoInputNum; i++)
			{
				CStreamRecordManEx::Instance()->SetRecEnable(FRAME_TYPE_AUDIO, (pTempData[i-m_localVideoInputNum+audioNum] ? true : false), i);
			}

			WriteLog(CConfigEx::Instance()->cfgClient(), LOG_TYPE_CHGE_REC_AUDIO_SWITCH);
		}
		
		break;
	case NCFG_ITEM_REC_REDUNDANT:				//此通道是否开启冗余录像，unsigned long [VIDEO_INPUT_NUM]
		{
			const unsigned long *pTempData = reinterpret_cast<const unsigned long *>(pData);
			for (int i=0; i<m_videoInputNum; i++)
			{
				CStreamRecordManEx::Instance()->SetRedundancyRecord((pTempData[i] ? true : false), i);
			}

			WriteLog(CConfigEx::Instance()->cfgClient(), LOG_TYPE_CHGE_REC_REDU_SWITCH);
		}
		break;
	default:
		//此ID还未处理。
		assert(false);
		break;
	}
}

void CConfigSetMan::EffectScheduleCfg(const NCFG_ITEM_HEAD *pItemHead, const unsigned char *pData)
{
	const WEEK_SCHEDULE *pWeekSch = reinterpret_cast<const WEEK_SCHEDULE*>(pData);

	if((pItemHead->itemID == NCFG_ITEM_REC_SCH_SCHEDULE_HOLIDAY) || \
		(pItemHead->itemID == NCFG_ITEM_REC_SCH_MOTION_HOLIDAY) || \
		(pItemHead->itemID == NCFG_ITEM_REC_SCH_SENSOR_HOLIDAY) || \
		(pItemHead->itemID == NCFG_ITEM_SENSOR_IN_SCH_HOLIDAY) || \
		(pItemHead->itemID == NCFG_ITEM_MOTION_SCH_HOLIDAY) || \
		(pItemHead->itemID == NCFG_ITEM_ALARMOUT_SCH_HOLIDAY) || \
		(pItemHead->itemID == NCFG_ITEM_NET_ALARM_SCH_HOLIDAY))
	{			
		EffecScheduleHolidayCfg(pItemHead, pData);
	}
	else
	{
		switch(pItemHead->itemID)
		{
		case NCFG_ITEM_REC_SCH_SCHEDULE_WEEK:	//排程录像周计划， WEEK_SCHEDULE[VIDEO_INPUT_NUM]
			{
				for (int i=0; i<m_videoInputNum; i++)
				{
					CStreamRecordManEx::Instance()->SetWeekSchedule(RECORD_TYPE_SCHEDULE, pWeekSch[i], i);
				}

				WriteLog(CConfigEx::Instance()->cfgClient(), LOG_TYPE_CHGE_SCH_SCHEDULE);
			}
			
			break;
		case NCFG_ITEM_REC_SCH_MOTION_WEEK:		//移动侦测录像周计划， WEEK_SCHEDULE[VIDEO_INPUT_NUM]
			{
				for (int i=0; i<m_videoInputNum; i++)
				{
					CStreamRecordManEx::Instance()->SetWeekSchedule(RECORD_TYPE_MOTION, pWeekSch[i], i);
				}

				WriteLog(CConfigEx::Instance()->cfgClient(), LOG_TYPE_CHGE_SCH_MOTION);
			}
			
			break;
		case  NCFG_ITEM_REC_SCH_SENSOR_WEEK:		//传感器报警录像周计划，WEEK_SCHEDULE[VIDEO_INPUT_NUM]
			{
				for (int i=0; i<m_videoInputNum; i++)
				{
					CStreamRecordManEx::Instance()->SetWeekSchedule(RECORD_TYPE_SENSOR, pWeekSch[i], i);
				}

				WriteLog(CConfigEx::Instance()->cfgClient(), LOG_TYPE_CHGE_SCH_ALARM);
			}
			
			break;
		case NCFG_ITEM_SENSOR_IN_SCH_WEEK:
			{
				for( int i=0; i<m_sensorInputNum; i++ )
				{
					m_pSensorMan->SetWeekSchedule( i, pWeekSch[i] );
				}

				WriteLog(CConfigEx::Instance()->cfgClient(), LOG_TYPE_CHGE_SENSOR_SCH);
			}
			
			break;
		case NCFG_ITEM_ALARMOUT_SCH_WEEK:
			{
				for( int i=0; i<m_sensorOutputNum; i++ )
				{
					m_pAlarmOutMan->SetWeekSchedule( i, pWeekSch[i] );
				}

				WriteLog(CConfigEx::Instance()->cfgClient(), LOG_TYPE_CHGE_RELAY_SCH);
			}
			
			break;
		case NCFG_ITEM_MOTION_SCH_WEEK:
			{
				for( int i=0; i<m_videoInputNum; i++ )
				{
					m_pMotionMan->SetWeekSchedule( i, pWeekSch[i] );
				}

				WriteLog(CConfigEx::Instance()->cfgClient(), LOG_TYPE_CHGE_MOTION_SCH);
			}
			
			break;
		case NCFG_ITEM_NET_ALARM_SCH_WEEK:
			break;
		default:
			//此ID还未处理。
			assert(false);
			break;
		}
	}
}

void CConfigSetMan::EffectNetBaseCfg(const NCFG_ITEM_HEAD *pItemHead, const unsigned char *pData)
{

	switch(pItemHead->itemID)
	{
#ifdef __ENVIRONMENT_LINUX__
	case NCFG_ITEM_UPNP_ENABLE:				//是否启动UPNP功能, unsigned long
		{
			CUpnpMan::Instance()->EnableUpnp(*reinterpret_cast<const unsigned long *>(pData));
		}
		break;
	case NCFG_ITEM_UPNP_FRIENDLY_NAME:		//UPNP设备显示的名字, LONG_NAME
		break;
	case NCFG_ITEM_HTTP_PORT:				//HTTP 端口, unsigned short
		{
			CNetInterface::Instance()->SetHttpPort(*reinterpret_cast<const unsigned short *>(pData));
#ifdef __NETSERVER_TVT__
			CNetServer::Instance()->ChangeHttpPort(*reinterpret_cast<const unsigned short *>(pData));
			CWebServer::Instance()->ChangeServerHttpPort( *reinterpret_cast<const unsigned short *>( pData ) );
			CDdnsManager::Instance()->SetHttpPort(*reinterpret_cast<const unsigned short *>(pData));
#endif
#ifdef __SUPPORT_HRSST__
			CPhoneSync::Instance()->SetDVRHttpPort(*reinterpret_cast<const unsigned short *>(pData));
#endif

			CUpnpMan::Instance()->AddPortMap("eth0", *reinterpret_cast<const unsigned short *>(pData), PORT_TYPE_WEB);

#if defined(__TVT_PUSH_SOCK__)
			CPushMan::Instance()->ChangePort(*reinterpret_cast<const unsigned short *>(pData));
#endif 			
			WriteLog(CConfigEx::Instance()->cfgClient(), LOG_TYPE_CHGE_HTTP_PORT);
		}
		break;
	case NCFG_ITEM_SERVER_PORT:				//网络服务端口， unsigned short
		{
			CNetInterface::Instance()->SetServerPort(*reinterpret_cast<const unsigned short *>(pData));
#ifdef __NETSERVER_TVT__
			CNetServer::Instance()->ChangeListenPort( *reinterpret_cast<const unsigned short *>( pData ));
			CDdnsManager::Instance()->SetServerPort(*reinterpret_cast<const unsigned short *>( pData ));
#endif
#ifdef __SUPPORT_HRSST__
			CPhoneSync::Instance()->SetDVRServerPort(*reinterpret_cast<const unsigned short *>(pData));
#endif

#ifdef __CUSTOM_YUANZHENG__
			CYuanZhengMan::Instance()->SetServerPort(*reinterpret_cast<const unsigned short *>(pData));
#endif

			CUpnpMan::Instance()->AddPortMap("eth0", *reinterpret_cast<const unsigned short *>(pData), PORT_TYPE_DATA);
			
			WriteLog(CConfigEx::Instance()->cfgClient(), LOG_TYPE_CHGE_SER_PORT);

		}
		break;
#ifdef __RELEASE__
	case NCFG_ITEM_NETWORK_ADDR_INFO:
		{
#ifdef __SUPPORT_3G__
			//if (!g_curNet3G)
			{

				CAllocNICIP::Instance()->allocNICIP(CAllocNICIP::GETIPMODE_NPPPOE, pData );

#ifdef __NETSERVER_HXHT__
				if (m_bHasInitial)
				{
					CHxConnectMan::Instance()->Restart(NULL, true);
				}
#endif
				CUpnpMan::Instance()->IPChanged("eth0");
				WriteLog(CConfigEx::Instance()->cfgClient(), LOG_TYPE_CHGE_IP);

			}
/*			else
			{
				CAllocNICIP::Instance()->allocNICIP(CAllocNICIP::GETIPMODE_NPPPOE, pData );

#ifdef __NETSERVER_HXHT__
				if (m_bHasInitial)
				{
					CHxConnectMan::Instance()->Restart(NULL, true);
				}
#endif
				CUpnpMan::Instance()->IPChanged("eth0");
				WriteLog(CConfigEx::Instance()->cfgClient(), LOG_TYPE_CHGE_IP);
			}*/
#else
			CAllocNICIP::Instance()->allocNICIP(CAllocNICIP::GETIPMODE_NPPPOE, pData );

#ifdef __NETSERVER_HXHT__
			if (m_bHasInitial)
			{
				CHxConnectMan::Instance()->Restart(NULL, true);
			}
#endif
			CUpnpMan::Instance()->IPChanged("eth0");
			WriteLog(CConfigEx::Instance()->cfgClient(), LOG_TYPE_CHGE_IP);
#endif

#if defined(__CHIPGM__)
			if (TD_2708TS_PL == CProduct::Instance()->ProductType())
			{
#if defined(__ENVIRONMENT_LINUX__)
				DVRSystem("ifconfig eth0 up");
#endif
			}
#endif
		}
		break;
#endif
	case NCFG_ITEM_DNS_1:					//unsigned long
		{
#ifdef __SUPPORT_3G__
			if (!g_curNet3G)
			{
				CNetInterface::Instance()->SetDNS(*reinterpret_cast<const unsigned long *>(pData),1, "eth0");
				CAllocNICIP::Instance()->SetDnsInfo( *reinterpret_cast<const unsigned long *>(pData), 1 );
			}
#else
			CNetInterface::Instance()->SetDNS(*reinterpret_cast<const unsigned long *>(pData),1, "eth0");
			CAllocNICIP::Instance()->SetDnsInfo( *reinterpret_cast<const unsigned long *>(pData), 1 );
#endif
		}
		break;
	case NCFG_ITEM_DNS_2:					//unsigned long
		{
#ifdef __SUPPORT_3G__
			if (!g_curNet3G)
			{
				CNetInterface::Instance()->SetDNS(*reinterpret_cast<const unsigned long *>(pData), 2, "eth0");
				CAllocNICIP::Instance()->SetDnsInfo( *reinterpret_cast<const unsigned long *>(pData), 2 );
			}
#else
			CNetInterface::Instance()->SetDNS(*reinterpret_cast<const unsigned long *>(pData), 2, "eth0");
			CAllocNICIP::Instance()->SetDnsInfo( *reinterpret_cast<const unsigned long *>(pData), 2 );
#endif
		}
		break;
	case NCFG_ITEM_PPPOE_INFO:
		{
#ifdef __SUPPORT_3G__
			if (!g_curNet3G)
			{
				CAllocNICIP::Instance()->allocNICIP(CAllocNICIP::GETIPMODE_PPPOE, pData );
#ifdef __NETSERVER_HXHT__
				if (m_bHasInitial)
				{
					CHxConnectMan::Instance()->Restart(NULL, true);
				}
#endif
				WriteLog(CConfigEx::Instance()->cfgClient(), LOG_TYPE_CHGE_PPPOE);
			}
#else
			CAllocNICIP::Instance()->allocNICIP(CAllocNICIP::GETIPMODE_PPPOE, pData );
#ifdef __NETSERVER_HXHT__
			if (m_bHasInitial)
			{
				CHxConnectMan::Instance()->Restart(NULL, true);
			}
#endif
			WriteLog(CConfigEx::Instance()->cfgClient(), LOG_TYPE_CHGE_PPPOE);
#endif
		}
		break;
	case NCFG_ITEM_DDNS_INFO:              //DDNS是否开启, unsigned long
		{
			CDdnsManager::Instance()->ChangeDDNSClient(*reinterpret_cast<const DDNS_INFO*>(pData));

			WriteLog(CConfigEx::Instance()->cfgClient(), LOG_TYPE_CHGE_DDNS);
		}
		break;
#ifdef __DVR_BASIC__
	case NCFG_ITEM_EMAIL_SEND_INFO:
		{			
			CSmtpMan::Instance()->SetSendPara(reinterpret_cast<const MAIL_SEND_INFO*>(pData));
			WriteLog(CConfigEx::Instance()->cfgClient(), LOG_TYPE_CHGE_SEND_EMAIL);
		}
		break;
	case NCFG_ITEM_EMAIL_RECV_INFO:
		{			
			CSmtpMan::Instance()->SetReceiverPara(reinterpret_cast<const MAIL_RECV_INFO*>(pData), MAX_MAIL_RECV_NUM);
			WriteLog(CConfigEx::Instance()->cfgClient(), LOG_TYPE_CHGE_RECV_EMAIL);
		}
		break;
	case NCFG_ITEM_EMAIL_PIC_ACC:
		{
			CSnapProc::Instance()->SetSnapImg(*reinterpret_cast<const unsigned long*>(pData));
		}
		break;
#endif
	case NCFG_ITEM_MULTI_NET_ADDR:
		{

		}
		break;
	case NCFG_ITEM_DDNS_UPDATE_CYCLE:
		{	
			CDdnsManager::Instance()->ChangeUpdateCycle(*reinterpret_cast<const unsigned long *>(pData));
			WriteLog(CConfigEx::Instance()->cfgClient(), LOG_TYPE_CHGE_DDNS);
		}
		break;
	case NCFG_ITEM_BAN_ALLOW_TYPE:
		{	
#ifdef __NETSERVER_TVT__
			CNetServer::Instance()->SetBanAllowType(*reinterpret_cast<const BAN_ALLOW_TYPE *>(pData));
#endif
		}
		break;
	case NCFG_ITEM_BAN_ALLOW_LIST:
		{	
#ifdef __NETSERVER_TVT__
			CNetServer::Instance()->SetBanAllowList(reinterpret_cast<const IP_SECTION *>(pData), MAX_BAN_ALLOW_LIST_NUM);
#endif
		}
		break;
	case NCFG_ITEM_HRSST_SERVER_IP:
		{
#ifdef __SUPPORT_HRSST__			
			CPhoneSync::Instance()->SetPhoneServerIp(*reinterpret_cast<const unsigned long *>(pData));
#endif
		}
		break;
	case NCFG_ITEM_HRSST_SERVER_PORT:
		{
#ifdef __SUPPORT_HRSST__
			CPhoneSync::Instance()->SetPhoneServerPort(*reinterpret_cast<const unsigned long *>(pData));
#endif
		}
		break;
	case NCFG_ITEM_HXHT_NET_PARAM:
		{
#ifdef __NETSERVER_HXHT__
			if (m_bHasInitial)
			{
				CHxConnectMan::Instance()->Restart((const HXHT_NET_PARAM_INFO *)(pData), false);
			}
#endif
		}
		break;
	case NCFG_ITEM_NET_WIRELESS:
		{
#ifdef __SUPPORT_3G__
			bool bStartWireless = CWirelessManage::Instance()->GetWirelessThdState();
			const MDVR_WIRELESS_CFG* G3Info = reinterpret_cast<const MDVR_WIRELESS_CFG *>(pData);
			//char cmd[100]={0};

			if (G3Info->enable)
			{
				//snprintf(cmd, 100, "ifconfig eth0 down");
				//DVRSystem(cmd);
				g_curNet3G = true;
				printf("g_curNet3G: %d\n", g_curNet3G);	
				if (!bStartWireless)
				{
					CWirelessManage::Instance()->SetWirelessPara(G3Info);
					CWirelessManage::Instance()->StartWireless();
				}
				else
				{
					CWirelessManage::Instance()->Stop();
					CWirelessManage::Instance()->SetWirelessPara(G3Info);
					CWirelessManage::Instance()->StartWireless();
				}
				DVRSystem("route del -net default dev eth0");
				DVRSystem("route add -net default dev ppp0");

			}
			else
			{
				//snprintf(cmd, 100, "ifconfig eth0 up");
				//DVRSystem(cmd);

				if (bStartWireless)
				{
					CWirelessManage::Instance()->Stop();
				}
				DVRSystem("route");
				DVRSystem("ifconfig ppp0 down");
				DVRSystem("route");
				g_curNet3G = false;
			}
#endif
		}
		break;

	case NCFG_ITEM_AUTO_REPORT:
		{
			if (m_bHasInitial)
			{
				CNetServer::Instance()->ChangeAutoCms(*reinterpret_cast<const NET_AUTO_REPORT *>(pData));
			}
		}
		break;
	case NCFG_ITEM_MINT_DDNS_SERVER_NAME:
		{
			CDdnsManager::Instance()->ChangeMintDDNSName(pData);
		}
		break;
	case NCFG_ITEM_USE_3G:
		{
			
		}
		break;
	case NCFG_ITEM_NETWORK_ADDR_INFO_IPV6:
		{
			//
#ifdef __CUSTOM_IPV6__
			CAllocNicIP6::Instance()->AllocNICIP(reinterpret_cast<const NETWORK_ADDR_INFO_IPv6 *>(pData));
#endif
		}
		break;
	case NCFG_ITEM_NET_YZ_PARAM:
		{
#ifdef __CUSTOM_YUANZHENG__
			YUANZHENG_PARAM yuanz_param=*reinterpret_cast<const YUANZHENG_PARAM *>(pData);
			CYuanZhengMan::Instance()->SetYuanZhengParam(yuanz_param);
			CYZSnapMan::Instance()->SetYuanZhengParam(yuanz_param);
#endif
		}
		break;
	case NCFG_ITEM_NET_YZ_SNAP_PARAM:
		{
#ifdef __CUSTOM_YUANZHENG__
			YUANZHENG_SNAP_PARAM snap_param=*reinterpret_cast<const YUANZHENG_SNAP_PARAM *>(pData);
			CYZSnapMan::Instance()->SetYZSnapPara(snap_param);
#endif
		}
		break;
#ifdef __SUPPORT_SNAPD1__
	case NCFG_ITEM_NET_SHANGHAI_DAILY_IMAGE:
		{
		
			CShanghaiSnapMan::GetInstance()->GetDailyCfg(reinterpret_cast<const SNAP_INFOEX*>(pData), MAX_DAILY_SNAP_NUM);
		}
		break;
	case NCFG_ITEM_NET_SHANGHAI_PRE_ALARM_SNAP_TIME:
		{
			CShanghaiSnapMan::GetInstance()->GetPreAlarmCfg(reinterpret_cast<const unsigned long*>(pData));
		}
		break;
	case NCFG_ITEM_NET_SHANGHAI_POST_ALARM_SNAP_TIME:
		{
			CShanghaiSnapMan::GetInstance()->GetPostAlarmCfg(reinterpret_cast<const unsigned long*>(pData));
		}
		break;
	case NCFG_ITEM_NET_SHANGHAI_MOTIONUPLOAD_ENABLE:
		{
			const unsigned long param=*reinterpret_cast<const unsigned long *>(pData);
			if(0 == param)
			{
				CShanghaiConnectMan::Instance()->SetMotionEnable(false);
			}
			else if(1 == param)
			{
				CShanghaiConnectMan::Instance()->SetMotionEnable(true);
			}	
		}
		break;
	case NCFG_ITEM_NET_SHANGHAI_PTENABLE:
		{
			const unsigned long param=*reinterpret_cast<const unsigned long *>(pData);
			if(0 == param)
			{
				CShanghaiConnectMan::Instance()->SetPTEnabel(false);
			}
			else if(1 == param)
			{
				CShanghaiConnectMan::Instance()->SetPTEnabel(true);
			}	
		}
		break;
	case NCFG_ITEM_NET_SHANGHAI_AGENTID:
		{
			const char *pAgent = reinterpret_cast<const char *>(pData);
			CShanghaiConnectMan::Instance()->SetAgentId(pAgent);
		}
		break;
	case NCFG_ITEM_NET_SHANGHAI_IP:
		{
			const char *pIp = reinterpret_cast<const char *>(pData);
			CShanghaiConnectMan::Instance()->SetIp(pIp);
		}
		break;
	case NCFG_ITEM_NET_SHANGHAI_PORT:
		{
			const unsigned long param=*reinterpret_cast<const unsigned long *>(pData);
			CShanghaiConnectMan::Instance()->Setport(param);
		}
		break;
#endif

#else
	case NCFG_ITEM_HTTP_PORT:				//HTTP 端口, unsigned short
		{
#if defined(__TVT_PUSH_SOCK__)
			CPushMan::Instance()->ChangePort(*reinterpret_cast<const unsigned short *>(pData));
#endif 
			WriteLog(CConfigEx::Instance()->cfgClient(), LOG_TYPE_CHGE_HTTP_PORT);
		}
		break;
	case NCFG_ITEM_AUTO_REPORT:
		{
			//if (m_bHasInitial)
			{
				CNetServer::Instance()->ChangeAutoCms(*reinterpret_cast<const NET_AUTO_REPORT *>(pData));
			}
		}
		break;
#endif
	case NCFG_ITEM_NET_FTP:
		{
			const TVT_FTP_CONFIG * pFtpConfig = reinterpret_cast<const TVT_FTP_CONFIG *>(pData);
#ifdef __ALARM_TO_UPDATE_FTP_VIDEO__
			CFtpUpdateMan::Instance()->SetupFtpUpdateInfo(pFtpConfig);
			//设置到报警服
			CFtpLoadMan::Instance()->SetUpFtpConfig(* pFtpConfig);
			//设置到FTP模块
#endif
		}
		break;
	case NCFG_ITEM_NAT_SERVER:
		{
			const NAT_SERVER_INFO * pNatServerInfo = reinterpret_cast<const NAT_SERVER_INFO *>(pData);
			CNetServer::Instance()->ChangeNatServerInfo(pNatServerInfo);
		}
		break;
	case NCFG_ITEM_PUSH_SERVER:
		{
#if defined(__TVT_PUSH_SOCK__)
			const PUSH_SERVER_INFO * pPushServerInfo = reinterpret_cast<const PUSH_SERVER_INFO *>(pData);
			CPushMan::Instance()->ChangeServerInfo(pPushServerInfo->szServerAddr, pPushServerInfo->iServerPort);
#endif
		}
		break;
#ifdef __DVR_ULTIMATE__
	case NCFG_ITEM_SWITCH_INFO:
		{
			const NET_SWITCH_INFO *pSwitchInfo = reinterpret_cast<const NET_SWITCH_INFO*>(pData);
			CNetDeviceManager::Instance()->ChangeSwitchIP(pSwitchInfo->switchIP, pSwitchInfo->netMask);
 			if(m_bHasInitial)
 			{
 				g_system_status = DVR_SYSTEM_REBOOT;
 			}
		}
		break;
#endif
	case NCFG_ITEM_USE_PUSH:
		{
#if defined(__TVT_PUSH_SOCK__)
			const unsigned long * pUsePush = reinterpret_cast<const unsigned long *>(pData);
			if (0 == * pUsePush)
			{
				CPushMan::Instance()->UsePush(false);
			}
			else
			{
				CPushMan::Instance()->UsePush(true);
			}
#endif
		}
		break;
	default:
		//此ID还未处理。
		//assert(false);
		break;
	}

}

void CConfigSetMan::EffectAlarmTrigerCfg(const NCFG_ITEM_HEAD *pItemHead, const unsigned char *pData)
{
	switch(pItemHead->itemID)
	{
	case NCFG_ITEM_BUZZER_ENABLE:			//蜂鸣器开关，unsigned long
		{
			const unsigned long *pTempData = reinterpret_cast<const unsigned long *>(pData);
			m_pAlarmOutMan->SetBuzzerEnable((*pTempData) ? true : false);

			if (*pTempData)
			{
				WriteLog(CConfigEx::Instance()->cfgClient(), LOG_TYPE_BUZZER_ON);
			}
			else
			{
				WriteLog(CConfigEx::Instance()->cfgClient(), LOG_TYPE_BUZZER_OFF);
			}
		}
		break;
	case NCFG_ITEM_BUZZER_HOLD_TIME:			//蜂鸣器保持时间，unsigned short,
		{
			const unsigned long *pTempData = reinterpret_cast<const unsigned long *>(pData);
			m_pAlarmOutMan->SetBuzzerHoldTime(*pTempData);
		}
		break;
	case NCFG_ITEM_ALARM_OUT_NAME:			//继电器输出名称，SHORT_NAME[SENSOR_OUTPUT_NUM]
		break;
	case NCFG_ITEM_ALARM_OUT_HOLD_TIME:		//继电器输出保持时间，unsigned long [SENSOR_OUTPUT_NUM]
		{
			const unsigned long *pHoldTime = reinterpret_cast<const unsigned long *>(pData);
			for (int i=0; i<m_sensorOutputNum; i++)
			{
				m_pAlarmOutMan->SetHoldTime((unsigned short)(pHoldTime[i]), i, false);
			}
		}
		break;
	default:
		//此ID还未处理。
		assert(false);
		break;
	}
}

void CConfigSetMan::EffectAlarmTrigerSensorCfg(const NCFG_ITEM_HEAD *pItemHead, const unsigned char *pData)
{
	switch(pItemHead->itemID)
	{
	case NCFG_ITEM_SENSOR_IN_ENABLE:			//传感器开关，unsigned long [SENSOR_INPUT_NUM]
		{
			const unsigned long *pTempData = reinterpret_cast<const unsigned long *>(pData);
			for (int i=0; i<m_sensorInputNum; i++)
			{
				m_pSensorMan->SetEnable((pTempData[i] ? true : false), i);
			}

			WriteLog(CConfigEx::Instance()->cfgClient(), LOG_TYPE_CHGE_SENSOR_SWITCH);
		}
		
		break;
	case NCFG_ITEM_SENSOR_IN_TYPE:			//传感器设备类型，unsigned long [SENSOR_INPUT_NUM]
		{
			const unsigned long *pTempData = reinterpret_cast<const unsigned long *>(pData);
			for (int i=0; i<m_localSensorInputNum; i++)
			{
				m_pSensorMan->SetDeviceType((pTempData[i] ? false : true ), i);
			}

			WriteLog(CConfigEx::Instance()->cfgClient(), LOG_TYPE_CHGE_SENSOR_TYPE);
		}
		
		break;
	case NCFG_ITEM_SENSOR_IN_NAME:			//传感器名称，SHORT_NAME [SENSOR_INPUT_NUM]
		{
			const NCFG_INFO_SHORT_NAME *pName = reinterpret_cast<const NCFG_INFO_SHORT_NAME *>(pData);
			for (unsigned char i=0; i<m_sensorInputNum; ++i)
			{
				CMessageMan::Instance()->RefreshSensorName(i, pName[i].name);
			}
		}
		break;
	case NCFG_ITEM_SENSOR_IN_HOLD_TIME://传感器保持时间, unsigned long [SENSOR_INPUT_NUM]
		{
			const unsigned long *pHoldTime = reinterpret_cast<const unsigned long*>(pData);
#if !(defined(__CUSTOM_US02__) && defined(__CUSTOM_US02_NEW__))
			for (int i=0; i<m_sensorInputNum; i++)
			{
				m_pSensorMan->SetHoldTime(pHoldTime[i], i);
			}
#else
			for(int i=0; i<m_sensorInputNum; i++)
			{
				m_pSensorMan->SetHoldTime(5, i);
			}
#endif
// 			for(int i = m_localSensorInputNum; i < m_sensorInputNum; i++)
// 			{
// 				m_pSensorMan->SetHoldTime(0, i);
// 			}
		}
		
		break;
	case NCFG_ITEM_SENSOR_IN_BUZZ:          //传感器触发蜂鸣器, unsigned long [SENSOR_INPUT_NUM]
		{
			const unsigned long *pToBuzzer = reinterpret_cast<const unsigned long *>(pData);
			for( int i=0; i<m_sensorInputNum; i++)
				m_pSensorMan->SetToBuzzer((*(pToBuzzer+i) << m_sensorOutputNum), i, m_sensorOutputNum );

		}
		break;
	case NCFG_ITEM_SENSOR_IN_TO_ALARM_OUT:	//传感器触发报警输出，ULONGLONG [SENSOR_INPUT_NUM];
		{
			const ULONGLONG *pToAlarmOut = reinterpret_cast<const ULONGLONG *>(pData);
			for (int i=0; i<m_sensorInputNum; i++)
			{
				m_pSensorMan->SetToAlarmOutCH(pToAlarmOut[i], i, m_sensorOutputNum);
			}
		}
		
		break;
	case NCFG_ITEM_SENSOR_IN_TO_RECORD:		//传感器触发录像，ULONGLONG [SENSOR_INPUT_NUM];
		{
			const ULONGLONG *pToRecord = reinterpret_cast<const ULONGLONG *>(pData);
			for (int i=0; i<m_sensorInputNum; i++)
			{
				m_pSensorMan->SetToRecCH(pToRecord[i], i);
			}
		}
		
		break;
	case NCFG_ITEM_SENSOR_TO_BIG_VIEW:		//传感器触发大画面，ULONGLONG [SENSOR_INPUT_NUM];
		{
			const ULONGLONG *pToBigView = reinterpret_cast<const ULONGLONG *>(pData);
			for( int i=0; i<m_sensorInputNum; i++ )
			{
				m_pSensorMan->SetToBigViewCH(pToBigView[i], i);
			}
		}

		break;
	case NCFG_ITEM_SENSOR_IN_TO_EMAIL_ENABLE://传感器能够触发EMAIL，unsigned long
		{
			const unsigned long *pToEmail = reinterpret_cast<const unsigned long *>(pData);
			for( int i=0; i<m_sensorInputNum; i++ )
			{
				m_pSensorMan->SetToEmail(pToEmail[i], i);
			}
		}
		break;
	case NCFG_ITEM_SENSOR_TO_EMAIL_ADDR:		//传感器触发MAIL地址列表，unsigned long[n]
		break;
	case NCFG_ITEM_SENSOR_IN_TO_FTP_ENABLE:	//传感器触发触发FTP，unsigned long
		{
#ifdef __ALARM_TO_UPDATE_FTP_VIDEO__
			const ULONGLONG * pFtpUpdateChannel  = reinterpret_cast<const ULONGLONG *>(pData);
			for( int i=0; i<m_localVideoInputNum; i++ )
			{
				m_pSensorMan->SetToFTPVideo(pFtpUpdateChannel[i], i);
			}
#endif
		}
		break;
	case NCFG_ITEM_SENSOR_IN_TO_FTP_ADDR:	//传感器触发FTP地址列表，和MAIL处理方式一样unsigned long[n] 
		break;
	case NCFG_ITEM_SENSOR_IN_TO_PTZ:			//传感器触发云台预制点，TO_PTZ [SENSOR_INPUT_NUM][VIDEO_INPUT_NUM]
		{
			if(m_videoInputNum > 0)
			{
				m_pSensorMan->SetToPTZ( reinterpret_cast<const TO_PTZ*>(pData),m_sensorInputNum ,m_videoInputNum );		
			}
			
		}
		break;
	case NCFG_ITEM_SENSOR_IN_TO_SNAP:		//传感器触发抓图的通道ULONGLONG [SENSOR_INPUT_NUM]
		{
			const ULONGLONG *pToSnap = reinterpret_cast<const ULONGLONG *>(pData);
			for (int i=0; i<m_sensorInputNum; i++)
			{
				m_pSensorMan->SetToSnapCH(pToSnap[i], i);
			}
		}
		break;
	case NCFG_ITEM_SENSOR_TO_FTP_SNAP:		//传感器触发FTP抓图，ULONGLONG[VIDEO_INPUT_NUM]
		break;
	case NCFG_ITEM_SENSOR_TO_EMAIL_SNAP:		//传感器触发EMAIL抓图，ULONGLONG[VIDEO_INPUT_NUM]
		break;
	default:
		//此ID还未处理。
		assert(false);
		break;
	}

	if ((NCFG_ITEM_SENSOR_IN_BUZZ <= pItemHead->itemID) && (pItemHead->itemID <= NCFG_ITEM_SENSOR_TO_EMAIL_SNAP))
	{
		WriteLog(CConfigEx::Instance()->cfgClient(), LOG_TYPE_CHGE_SENSOR_TRIGGER);
	}
}

void CConfigSetMan::EffectAlarmTrigerMotionCfg(const NCFG_ITEM_HEAD *pItemHead, const unsigned char *pData)
{
	switch(pItemHead->itemID)
	{
	case NCFG_ITEM_MOTION_ENABLE:			//移动侦测开关，unsigned long [VIDEO_INPUT_NUM]
		{
			const unsigned long *pTempData = reinterpret_cast<const unsigned long *>(pData);
			for (int i=0; i<m_videoInputNum; i++)
			{
				m_pMotionMan->SetEnable((pTempData[i] ? true : false), i);
			}

			WriteLog(CConfigEx::Instance()->cfgClient(), LOG_TYPE_CHGE_MOTION_SWITCH);
		}
		
		break;
	case NCFG_ITEM_MOTION_AREA_INFO:         //区域的信息，MOTION_DETECT_INFO[VIDEO_INPUT_NUM]
		{
			CLocalDevice *pLocalDevice = CLocalDevice::Instance();
			const MOTION_DETECT_INFO *pTempData = reinterpret_cast<const MOTION_DETECT_INFO *>(pData);
			PRODUCT_TYPE productID = CProduct::Instance()->ProductType();
			for (int i=0; i<m_localVideoInputNum; ++i)
			{
				pLocalDevice->SetMotionInfo(i, &pTempData[i]);
#ifdef __ENVIRONMENT_LINUX__
				if ((TD_2704AS_S == productID) || (TD_2708AS_S == productID) || (TD_2716AS_S == productID)
					|| (TD_2704AS_S_A == productID) || (TD_2708AS_S_A == productID) || (TD_2716AS_S_A == productID)
					|| (TD_2704TS_S == productID) || (TD_2708TS_S == productID) || (TD_2716TS_S == productID)
					|| (TD_2704TS_PL == productID) || (TD_2708TS_PL == productID) || (TD_2716TS_PL == productID)
					|| (TD_2704TD_P == productID) || (TD_2708TD_P == productID) || (TD_2716TD_P == productID) 
					|| (TD_2704TD_PL == productID) || (TD_2708TD_PL == productID) || (TD_2716TD_PL == productID) || (TD_2732TD_PL == productID)
					|| (TD_2704TE_M == productID) || (TD_2708TE_M == productID) || (TD_2716TE_M == productID)
					|| (TD_2704AE_SL == productID) || (TD_2708AE_SL == productID) || (TD_2716AE_SL == productID)
					|| (TD_2704AS_SL == productID) || (TD_2704AS_SL_A == productID) || (TD_2704AS_PL == productID) || (TD_2708AS_SL == productID) || (TD_2716AS_SL == productID)
					|| (TD_2704AS_SL_A == productID) || (TD_2708AS_SL_A == productID) || (TD_2716AS_SL_A == productID)
					|| (TD_2004AL == productID) || (TD_2008AS == productID)
					|| (TD_2704AS_PL == productID) || (TD_2708AS_PL == productID) || (TD_2716AS_PL == productID)
					|| (TD_2716AC_PL == productID)
					|| (TD_2704AE_PL == productID) || (TD_2708AE_PL == productID) || (TD_2716AE_PL == productID)
					|| (TD_2704AS_S_A == productID) || (TD_2708AS_S_A == productID) || (TD_2716AS_S_A == productID))

				{
					m_motionDetectInfo[i] = pTempData[i];
				}
#endif
			}

			WriteLog(CConfigEx::Instance()->cfgClient(), LOG_TYPE_CHGE_MOTION_AREA);
		}
		break;
	case NCFG_ITEM_MOTION_HOLD_TIME:	//移动侦测保持时间,unsigned long
		{
			const unsigned long *pHoldTime = reinterpret_cast<const unsigned long*>(pData);
			for (int i=0; i<m_videoInputNum; i++)
			{
				m_pMotionMan->SetHoldTime(pHoldTime[i], i);
			}
// 			for(int i = m_localVideoInputNum; i < m_videoInputNum; i++)
// 			{
// 				m_pMotionMan->SetHoldTime(0, i);
// 			}
		}
		
		break;
	case NCFG_ITEM_MOTION_TO_ALARM_OUT:		//移动侦测触发报警输出，ULONGLONG [VIDEO_INPUT_NUM]
		{
			const ULONGLONG *pToAlarmOut = reinterpret_cast<const ULONGLONG *>(pData);
			for (int i=0; i<m_videoInputNum; i++)
			{
				m_pMotionMan->SetToAlarmOutCH(pToAlarmOut[i], i, m_sensorOutputNum);
			}
		}
		
		break;
	case NCFG_ITEM_MOTION_TO_RECORD:			//移动侦测触发录像，ULONGLONG [VIDEO_INPUT_NUM]
		{
			const ULONGLONG *pToRecord = reinterpret_cast<const ULONGLONG *>(pData);
			for (int i=0; i<m_videoInputNum; i++)
			{
				m_pMotionMan->SetToRecCH(pToRecord[i], i);
			}
		}
		
		break;
	case NCFG_ITEM_MOTION_TO_BIG_VIEW:			//移动侦测触发大画面，ULONGLONG [VIDEO_INPUT_NUM]
		{
			const ULONGLONG *pToBigView = reinterpret_cast<const ULONGLONG *>(pData);
			for( int i=0; i<m_videoInputNum; i++ )
			{
				m_pMotionMan->SetToBigViewCH(pToBigView[i], i);
			}
		}

		break;
	case NCFG_ITEM_MOTION_TO_EMAIL_ENABLE:	//移动侦测能够触发EMAIL，unsigned long
		{
			const unsigned long *pToEmail = reinterpret_cast<const unsigned long *>(pData);
			for( int i=0; i<m_videoInputNum; i++ )
			{
				m_pMotionMan->SetToEmail(pToEmail[i], i);
			}
		}
		break;
	case NCFG_ITEM_MOTION_TO_EMAIL_ADDR:		//传感器触发MAIL地址列表，unsigned long[n]
		break;
	case NCFG_ITEM_MOTION_TO_PTZ:			//移动侦测触发云台，TO_PTZ [VIDEO_INPUT_NUM][VIDEO_INPUT_NUM]
		if(m_videoInputNum > 0)
		{
		    m_pMotionMan->SetToPTZ(reinterpret_cast<const TO_PTZ *>(pData), m_videoInputNum, m_videoInputNum);
		}
		break;
	case NCFG_ITEM_MOTION_TO_FTP_ENABLE:		//移动侦测触发FTP，unsigned long
		{
#ifdef __ALARM_TO_UPDATE_FTP_VIDEO__		
			const ULONGLONG * pFtpUpdateChannel  = reinterpret_cast<const ULONGLONG *>(pData);
			for( int i=0; i<m_localVideoInputNum; i++ )
			{
				m_pMotionMan->SetToFTPVideo(pFtpUpdateChannel[i], i);
			}
#endif
		}
		break;
	case NCFG_ITEM_MOTION_TO_FTP_ADDR:		//移动侦测触发FTP地址列表，和MAIL处理方式一样unsigned long[n] 
		break;
	case NCFG_ITEM_MOTION_TO_BUZZ:           //移动侦测触发BUZZ unsigned long[VIDEO_INPUT_NUM]
		{
			const unsigned long *pToBuzzer = reinterpret_cast<const unsigned long *>(pData);
			for( int i=0; i<m_videoInputNum; i++)
				m_pMotionMan->SetToBuzzer((*(pToBuzzer+i) << m_sensorOutputNum), i, m_sensorOutputNum );

		}
		break;
	case NCFG_ITEM_MOTION_TO_SNAP:           //移动侦测触发抓拍 ULONGLONG[VIDEO_INPUT_NUM]
		{
			const ULONGLONG *pToSnap = reinterpret_cast<const ULONGLONG *>(pData);
			for (int i=0; i<m_videoInputNum; i++)
			{
				m_pMotionMan->SetToSnapCH(pToSnap[i], i);
			}
		}		
		break;
	case NCFG_ITEM_MOTION_TO_FTP_SNAP:		//移动侦测触发FTP抓图，ULONGLONG[VIDEO_INPUT_NUM]
		break;
	case NCFG_ITEM_MOTION_TO_EMAIL_SNAP:		//移动侦测触发EMAIL抓图，ULONGLONG[VIDEO_INPUT_NUM]
		break;
#ifdef __NETSERVER_HXHT__
	case NCFG_ITEM_HXHT_MOTION_AREA_INFO:       //互信互通矩形区域配置
		break;
#endif
	default:
		//此ID还未处理。
		assert(false);
		break;
	}

	if ((NCFG_ITEM_MOTION_TO_ALARM_OUT <= pItemHead->itemID) && (pItemHead->itemID <= NCFG_ITEM_MOTION_TO_EMAIL_SNAP))
	{
		WriteLog(CConfigEx::Instance()->cfgClient(), LOG_TYPE_CHGE_MOTION_TRIGGER);
	}
}

void CConfigSetMan::EffectAlarmTrigerVLossCfg(const NCFG_ITEM_HEAD *pItemHead, const unsigned char *pData)
{
	switch(pItemHead->itemID)
	{
	case NCFG_ITEM_VL_TO_ALARM_OUT:			//视频丢失触发报警输出，ULONGLONG [VIDEO_INPUT_NUM]
		{
			const ULONGLONG *pToAlarmOut = reinterpret_cast<const ULONGLONG *>(pData);
			for (int i=0; i<m_videoInputNum; i++)
			{
				m_pVideoLossMan->SetToAlarmOutCH(pToAlarmOut[i], i, m_sensorOutputNum);
			}
		}
		
		break;
	case NCFG_ITEM_VL_TO_RECORD:			//视频丢失触发录像，ULONGLONG [VIDEO_INPUT_NUM]
		{
			const ULONGLONG *pToRecord = reinterpret_cast<const ULONGLONG *>(pData);
			for (int i=0; i<m_videoInputNum; i++)
			{
				m_pVideoLossMan->SetToRecCH(pToRecord[i], i);
			}
		}

		break;
	case NCFG_ITEM_VL_TO_BIG_VIEW:			//视频丢失触发大画面，ULONGLONG [VIDEO_INPUT_NUM]
		{
			const ULONGLONG *pToBigView = reinterpret_cast<const ULONGLONG *>(pData);
			for( int i=0; i<m_videoInputNum; i++ )
			{
				m_pVideoLossMan->SetToBigViewCH(pToBigView[i], i);
			}
		}

		break;
	case NCFG_ITEM_VL_TO_PTZ:
 		{
// 			const unsigned long *pToBuzzer = reinterpret_cast<const unsigned long *>(pData);
// 			for( int i=0; i<m_localVideoInputNum; i++)
			//if(m_localVideoInputNum > 0)
			{
				m_pVideoLossMan->SetToPTZ(reinterpret_cast<const TO_PTZ *>(pData), m_videoInputNum, m_videoInputNum );
			}
		}
		break;
	case NCFG_ITEM_VL_TO_EMAIL_ENABLE:		//视频丢失能够触发MAIL
		{
			const unsigned long *pToEmail = reinterpret_cast<const unsigned long *>(pData);
			for( int i=0; i<m_videoInputNum; i++ )
			{
				m_pVideoLossMan->SetToEmail(pToEmail[i], i);
			}
		}
		break;
	case NCFG_ITEM_VL_TO_EMAIL_ADDR:			//传感器触发MAIL地址列表，unsigned long[n]
		break;
	case NCFG_ITEM_VL_TO_FTP_ENABLE:			//视频丢失触发FTP，unsigned long
		{
//#ifdef __ALARM_TO_UPDATE_FTP_VIDEO__
//			const ULONGLONG * pFtpUpdateChannel  = reinterpret_cast<const ULONGLONG *>(pData);
//			for( int i=0; i<m_localVideoInputNum; i++ )
//			{
//				m_pVideoLossMan->SetToFTPVideo(pFtpUpdateChannel[i], i);
//			}
//#endif
		}
		break;
	case NCFG_ITEM_VL_TO_FTP_ADDR:			//视频丢失触发FTP地址列表，和MAIL处理方式一样unsigned long[n] 
		{
// #ifdef __ALARM_TO_UPDATE_FTP_VIDEO__
// 			const ULONGLONG * pFtpUpdateChannel  = reinterpret_cast<const ULONGLONG *>(pData);
// 			for( int i=0; i<m_localVideoInputNum; i++ )
// 			{
// 				m_pMotionMan->SetToFTPVideo(pFtpUpdateChannel[i], i);
// 			}
// #endif
		}
		break;
	case NCFG_ITEM_VL_TO_BUZZ:               //视频丢失触发BUZZ unsigned long[VIDEO_INPUT_NUM]
		{
			const unsigned long *pToBuzzer = reinterpret_cast<const unsigned long *>(pData);
			for( int i=0; i<m_videoInputNum; i++)
				m_pVideoLossMan->SetToBuzzer((*(pToBuzzer+i) << m_sensorOutputNum), i, m_sensorOutputNum );

		}
		break;
	case NCFG_ITEM_VL_TO_SNAP:
		{
			const ULONGLONG *pToSnap = reinterpret_cast<const ULONGLONG *>(pData);
			for (int i=0; i<m_videoInputNum; i++)
			{
				m_pVideoLossMan->SetToSnapCH(pToSnap[i], i);
			}
		}		
		break;
	case NCFG_ITEM_VL_TO_FTP_SNAP:
		break;
	case NCFG_ITEM_VL_TO_EMAIL_SNAP:
		break;
	default:
		//此ID还未处理。
		assert(false);
		break;
	}

	if ((NCFG_ITEM_VL_TO_ALARM_OUT <= pItemHead->itemID) && (pItemHead->itemID <= NCFG_ITEM_VL_TO_EMAIL_SNAP))
	{
		WriteLog(CConfigEx::Instance()->cfgClient(), LOG_TYPE_CHGE_VL_TRIGGER);
	}
}

void CConfigSetMan::EffectAlarmTrigerShelterCfg(const NCFG_ITEM_HEAD *pItemHead, const unsigned char *pData)
{
	switch(pItemHead->itemID)
	{
	case NCFG_ITEM_SHELTER_TO_ALARM_OUT:		//遮挡触发报警输出，ULONGLONG [VIDEO_INPUT_NUM]
		break;
	case NCFG_ITEM_SHELTER_TO_RECORD:		//遮挡触发录像，ULONGLONG [VIDEO_INPUT_NUM]
		break;
	case NCFG_ITEM_SHELTER_TO_BIG_VIEW:		//遮挡触发大画面，ULONGLONG [VIDEO_INPUT_NUM]
		break;
#ifdef __DVR_BASIC__	
	case NCFG_ITEM_SHELTER_TO_EMAIL_ENABLE:	//遮挡能够触发EMAIL，unsigned long
		break;
	case NCFG_ITEM_SHELTER_TO_EMAIL_ADDR:	//遮挡触发EMAIL地址列表,unsigned long
		break;
	case NCFG_ITEM_SHELTER_TO_FTP_ENABLE:	//遮挡触发FTP，unsigned long
		break;
#endif
	case NCFG_ITEM_SHELTER_TO_BUZZ:          //遮挡触发BUZZ unsigned long[VIDEO_INPUT_NUM]
		break;
	case NCFG_ITEM_SHELTER_TO_PTZ:			//遮挡触发云台，TO_PTZ [VIDEO_INPUT_NUM][VIDEO_INPUT_NUM]
		break;
	case NCFG_ITEM_SHELTER_TO_SNAP:
		break;
	case NCFG_ITEM_SHELTER_TO_FTP_SNAP:
		break;
	case NCFG_ITEM_SHELTER_TO_EMAIL_SNAP:
		break;
	default:
		//此ID还未处理。
		assert(false);
		break;
	}
}

void CConfigSetMan::EffectAlarmExceptionCfg(const NCFG_ITEM_HEAD *pItemHead, const unsigned char *pData)
{
	switch(pItemHead->itemID)
	{
	case NCFG_ITEM_DISK_FULL_BUZZER:			//磁盘满触发蜂鸣器报警 unsigned long
		{
			const unsigned long toBuzzer = *reinterpret_cast<const unsigned long *>(pData);
			m_pOtherAlarm->SetToBuzzer((0 != toBuzzer), OTHER_ALARM_TYPE_DISK_FULL, m_sensorOutputNum );
		}
		break;
	case NCFG_ITEM_DISK_FULL_TO_ALARM_OUT:		//磁盘满触发报警输出 ULONGLONG
		{
			const ULONGLONG *pToAlarmOut = reinterpret_cast<const ULONGLONG *>(pData);
			m_pOtherAlarm->SetToAlarmOutCH(*pToAlarmOut, OTHER_ALARM_TYPE_DISK_FULL, m_sensorOutputNum );
		}
		break;
	case NCFG_ITEM_DISK_FULL_TO_EMAIL:			//磁盘满触发邮件 unsigned long
		{
			const unsigned long toEmail = *reinterpret_cast<const unsigned long *>(pData);
			m_pOtherAlarm->SetToEmail(toEmail, OTHER_ALARM_TYPE_DISK_FULL);
		}
		break;
	case NCFG_ITEM_IP_CONFLICT_BUZZER:			//网络地址冲突触发蜂鸣器报警 unsigned long
		{
			const unsigned long toBuzzer = *reinterpret_cast<const unsigned long *>(pData);
			m_pOtherAlarm->SetToBuzzer((0 != toBuzzer), OTHER_ALARM_TYPE_IP_CONFLICT, m_sensorOutputNum );
		}
		break;
	case NCFG_ITEM_IP_CONFLICT_TO_ALARM_OUT:	//网络地址冲突触发报警输出 ULONGLONG
		{
			const ULONGLONG *pToAlarmOut = reinterpret_cast<const ULONGLONG *>(pData);
			m_pOtherAlarm->SetToAlarmOutCH(*pToAlarmOut, OTHER_ALARM_TYPE_IP_CONFLICT, m_sensorOutputNum );
		}
		break;
	case NCFG_ITEM_IP_CONFLICT_TO_EMAIL:		//网络地址冲突触发邮件 unsigned long
		{
			const unsigned long toEmail = *reinterpret_cast<const unsigned long *>(pData);
			m_pOtherAlarm->SetToEmail(toEmail, OTHER_ALARM_TYPE_IP_CONFLICT);
		}
		break;
	case NCFG_ITEM_DISCONNECT_BUZZER:			//网络断开触发蜂鸣器报警 unsigned long
		{
			const unsigned long toBuzzer = *reinterpret_cast<const unsigned long *>(pData);
			m_pOtherAlarm->SetToBuzzer((0 != toBuzzer), OTHER_ALARM_TYPE_DISCONNET, m_sensorOutputNum );
		}
		break;
	case NCFG_ITEM_DISCONNECT_TO_ALARM_OUT:		//网络断开触发报警输出 ULONGLONG
		{
			const ULONGLONG *pToAlarmOut = reinterpret_cast<const ULONGLONG *>(pData);
			m_pOtherAlarm->SetToAlarmOutCH(*pToAlarmOut, OTHER_ALARM_TYPE_DISCONNET, m_sensorOutputNum );
		}
		break;
	case NCFG_ITEM_DISCONNECT_TO_EMAIL:			//网络断开触发邮件 unsigned long
		{
			const unsigned long toEmail = *reinterpret_cast<const unsigned long *>(pData);
			m_pOtherAlarm->SetToEmail(toEmail, OTHER_ALARM_TYPE_DISCONNET);
		}
		break;
	case NCFG_ITEM_DISK_WARNING_BUZZER:			//硬盘衰减开触发蜂鸣器 unsigned long
		{
			const unsigned long toBuzzer = *reinterpret_cast<const unsigned long *>(pData);
			m_pOtherAlarm->SetToBuzzer((0 != toBuzzer), OTHER_ALARM_TYPE_DISK_WARNING, m_sensorOutputNum );
		}
		break;
	case NCFG_ITEM_DISK_WARNING_TO_ALARM_OUT:			//硬盘衰减触发报警输出 ULONGLONG
		{			
			const ULONGLONG *pToAlarmOut = reinterpret_cast<const ULONGLONG *>(pData);
			m_pOtherAlarm->SetToAlarmOutCH(*pToAlarmOut, OTHER_ALARM_TYPE_DISK_WARNING, m_sensorOutputNum );
		}
		break;
	case NCFG_ITEM_DISK_WARNING_TO_EMAIL:			 //硬盘衰减触发邮件 unsigned long
		{
			const unsigned long toEmail = *reinterpret_cast<const unsigned long *>(pData);
			m_pOtherAlarm->SetToEmail(toEmail, OTHER_ALARM_TYPE_DISK_WARNING);
		}
		break;
	//zxx新加硬盘掉线
	case NCFG_ITEM_DISK_DISCONNECT_BUZZER:			//硬盘掉线触发蜂鸣器 unsigned long
		{
			
			const unsigned long toBuzzer = *reinterpret_cast<const unsigned long *>(pData);
			m_pOtherAlarm->SetToBuzzer((0 != toBuzzer), OTHER_ALARM_TYPE_DISK_DISCONNET, m_sensorOutputNum );

		}
		break;
	case NCFG_ITEM_DISK_DISCONNECT_TO_ALARM_OUT:	//硬盘掉线触发报警输出 ULONGLONG
		{
			const ULONGLONG *pToAlarmOut = reinterpret_cast<const ULONGLONG *>(pData);
			m_pOtherAlarm->SetToAlarmOutCH(*pToAlarmOut, OTHER_ALARM_TYPE_DISK_DISCONNET, m_sensorOutputNum );

		}
		break;
	case NCFG_ITEM_DISK_DISCONNECT_TO_EMAIL:			//硬盘掉线触发邮件 unsigned long
		{
			const unsigned long toEmail = *reinterpret_cast<const unsigned long *>(pData);
			m_pOtherAlarm->SetToEmail(toEmail, OTHER_ALARM_TYPE_DISK_DISCONNET);

		}
		break;
	default:
		assert(false);
		break;
	}
}

void CConfigSetMan::EffectEncodeCfg(const NCFG_ITEM_HEAD *pItemHead, const unsigned char *pData)
{
#ifdef __ENVIRONMENT_LINUX__
	
	CLocalDevice *pLocalDevice = CLocalDevice::Instance();

	switch(pItemHead->itemID)
	{
	case NCFG_ITEM_ENCODE_MAJOR:		//主流分辨率，ENCODE_INFO [VIDEO_INPUT_NUM]，每个通道不同
		{
#if defined(__ALARM_TO_UPDATE_FTP_VIDEO__)
			CFtpUpdateMan::Instance()->Action(false);
#endif
			unsigned char maxRate = 0;
			if (VIDEO_FORMAT_PAL == g_video_format)
			{
				maxRate = CProduct::Instance()->LocalMaxChannelRatePal();
			}
			else if (VIDEO_FORMAT_NTSC == g_video_format)
			{
				maxRate = CProduct::Instance()->LocalMaxChannelRateNtsc();
			}
			else
			{
				assert(false);
			}

			unsigned long bSpec = CProduct::Instance()->LocalVideoSizeSpecial();
			ULONGLONG specChnnBits = CProduct::Instance()->LocalVideoSizeD1ChnnBits();

			OSD_ATTRIB attrib;
			memset(&attrib, 0, sizeof(OSD_ATTRIB));
			attrib.length = 5120;
			attrib.pData = new unsigned char [attrib.length];
			attrib.cy = 20;

			std::string name;

			unsigned char rate = 0;
			
			const ENCODE_INFO *pEncode_temp = reinterpret_cast<const ENCODE_INFO *>(pData);
			ENCODE_INFO *pEncode = const_cast<ENCODE_INFO *>(pEncode_temp);
			PRODUCT_TYPE productID = CProduct::Instance()->ProductType();

			for (int i=0; i<m_localVideoInputNum; i++)
			{
				if(pEncode[i].hBitStream>=GetMainStreamMaxBitRate(pEncode[i].resolution))
				{
					pEncode[i].hBitStream = GetMainStreamMaxBitRate(pEncode[i].resolution);
				}
				if (0 == memcmp(m_encode + i, pEncode + i, sizeof(ENCODE_INFO)))
				{
					if (4 <= m_setEncodeCount[i])
					{
						continue;
					}
				}
				else
				{
					m_encode[i] = pEncode[i];
				}

				/*if ((TD_2704AS_S == productID) || (TD_2708AS_S == productID) || (TD_2716AS_S == productID) || (TD_2704TS_S == productID) || (TD_2708TS_S == productID) || (TD_2716TS_S == productID) || (TD_2704TS_PL == productID) || (TD_2708TS_PL == productID) || (TD_2716TS_PL == productID)
					|| (TD_2704AS_S == productID) || (TD_2708AS_S == productID) || (TD_2716AS_S == productID) || (TD_2704TD_P == productID) || (TD_2708TD_P == productID) || (TD_2716TD_P == productID) || (TD_2704TD_PL == productID) || (TD_2708TD_PL == productID) || (TD_2716TD_PL == productID)
					|| (TD_2704TE_M == productID) || (TD_2708TE_M == productID) || (TD_2716TE_M == productID)
					|| (TD_2704AE_SL == productID) || (TD_2708AE_SL == productID) || (TD_2716AE_SL == productID) || (TD_2716AS_SL == productID)
					)*/
				{
					if (5 > m_setEncodeCount[i])
					{
						m_setEncodeCount[i] ++;
					}
				}

				if (bSpec && (((ULONGLONG)0x01 << i) & specChnnBits))
				{
					rate = pEncode[i].rate;
				}
				else
				{
					if (UsedRate(pEncode[i].rate, (VIDEO_SIZE)(pEncode[i].resolution)) > maxRate)
					{
						rate = RealRate(maxRate, (VIDEO_SIZE)(pEncode[i].resolution));
					}
					else
					{
						rate = pEncode[i].rate;
					}
				}

				unsigned char encodeTypeTmp = VIDEO_ENCODE_MODE_VBR;
				unsigned long hBitStreamTmp = 0;
#ifdef __DVR_PROFESSIONAL__
				encodeTypeTmp = pEncode[i].encodeType;
				hBitStreamTmp = pEncode[i].hBitStream;
#else
				//当maxstream等于零时，采用画质档次来处理
				hBitStreamTmp = 0;
#endif			

				//pLocalDevice->SetFrameRate(i, rate);
				//pLocalDevice->SetVideoQuality(i, static_cast<VIDEO_ENCODE_MODE>(pEncode[i].encodeType), pEncode[i].hBitStream, pEncode[i].quality);
				
				//TD_2716TS_S产品，修改视频输入时，需要重新修改移动侦测和遮挡区域
#ifdef __ENVIRONMENT_LINUX__
				if ((TD_2704AS_S == productID) || (TD_2708AS_S == productID) || (TD_2708AS_S_A == productID) || (TD_2716AS_S == productID)
					|| (TD_2704AS_S_A == productID) || (TD_2708AS_S_A == productID) || (TD_2716AS_S_A == productID)
					|| (TD_2704TS_S == productID) || (TD_2708TS_S == productID) || (TD_2716TS_S == productID)
					|| (TD_2704TS_PL == productID) || (TD_2708TS_PL == productID) || (TD_2716TS_PL == productID)
					|| (TD_2704TD_P == productID) || (TD_2708TD_P == productID) || (TD_2716TD_P == productID)
					|| (TD_2704TD_PL == productID) || (TD_2708TD_PL == productID) || (TD_2716TD_PL == productID) || (TD_2732TD_PL == productID)
					|| (TD_2704TE_M == productID) || (TD_2708TE_M == productID) || (TD_2716TE_M == productID)
					|| (TD_2704AE_SL == productID) || (TD_2708AE_SL == productID) || (TD_2716AE_SL == productID)
					|| (TD_2704AS_SL == productID) || (TD_2704AS_SL_A == productID) || (TD_2704AS_PL == productID) || (TD_2708AS_SL == productID) || (TD_2716AS_SL == productID)
					|| (TD_2704AS_SL_A == productID) || (TD_2708AS_SL_A == productID) || (TD_2716AS_SL_A == productID)
					|| (TD_2004AL == productID) || (TD_2008AS == productID)
					|| (TD_2704AS_PL == productID) || (TD_2708AS_PL == productID) || (TD_2716AS_PL == productID)
					|| (TD_2716AC_PL == productID)
					|| (TD_2704AE_PL == productID) || (TD_2708AE_PL == productID) || (TD_2716AE_PL == productID))
				{
					m_pLocalDeviceMan->SetCamCover(i, m_currentCover + i);
				}
#endif

				pLocalDevice->SetVideoSize(i, pEncode[i].resolution);
				pLocalDevice->SetFrameRateQuality(i, rate, static_cast<VIDEO_ENCODE_MODE>(encodeTypeTmp), hBitStreamTmp, pEncode[i].quality);

#ifdef __ENVIRONMENT_LINUX__
				if ((TD_2704AS_S == productID) || (TD_2708AS_S == productID) || (TD_2708AS_S_A == productID) || (TD_2716AS_S == productID)
					|| (TD_2704AS_S_A == productID) || (TD_2708AS_S_A == productID) || (TD_2716AS_S_A == productID)
					|| (TD_2704TS_S == productID) || (TD_2708TS_S == productID) || (TD_2716TS_S == productID)
					|| (TD_2704TS_PL == productID) || (TD_2708TS_PL == productID) || (TD_2716TS_PL == productID)
					|| (TD_2704TD_P == productID) || (TD_2708TD_P == productID) || (TD_2716TD_P == productID)
					|| (TD_2704TD_PL == productID) || (TD_2708TD_PL == productID) || (TD_2716TD_PL == productID) || (TD_2732TD_PL == productID)
					|| (TD_2704TE_M == productID) || (TD_2708TE_M == productID) || (TD_2716TE_M == productID)
					|| (TD_2704AE_SL == productID) || (TD_2708AE_SL == productID) || (TD_2716AE_SL == productID)
					|| (TD_2704AS_SL == productID) || (TD_2704AS_SL_A == productID) || (TD_2704AS_PL == productID) || (TD_2708AS_SL == productID) || (TD_2716AS_SL == productID)
					|| (TD_2704AS_SL_A == productID) || (TD_2708AS_SL_A == productID) || (TD_2716AS_SL_A == productID)
					|| (TD_2004AL == productID) || (TD_2008AS == productID)
					|| (TD_2704AS_PL == productID) || (TD_2708AS_PL == productID) || (TD_2716AS_PL == productID)
					|| (TD_2716AC_PL == productID)
					|| (TD_2704AE_PL == productID) || (TD_2708AE_PL == productID) || (TD_2716AE_PL == productID))
				{
					m_pLocalDeviceMan->SetMotionInfo(i, m_motionDetectInfo + i);
				}
#endif

				//2010-03-09 15:34:00 YSW
				//修改了图像分辨率后，需要重新设置通道名称叠加。子码流改变不需要操作。

				if (m_pOSDInfoCamName[i].bEnable)
				{
					attrib.xPos = m_pOSDInfoCamName[i].x;
					attrib.yPos = m_pOSDInfoCamName[i].y;
					attrib.cx	= 20 * name.length();

					attrib.length = 5120;

					CMessageMan::Instance()->GetCamName(i, name);
#ifdef __SUPPORT_OSDTWOLINES__
					ChangeOSDnameTwoLines(name);
#else
					ChangeOSDname(name);
#endif
					CFBInterface::Instance()->GetFont(attrib.pData, attrib.length, name.c_str(), name.length());

#ifdef __SUPPORT_OSDTWOLINES__
					unsigned short x_off, y_off,str_cx_cam,str_cy_cam;
					int height,width;
					m_pLocalDeviceMan->GetVideoSize(i, width, height);
					CFBInterface::Instance()->TextSize(name.c_str(), strlen(name.c_str()), str_cx_cam, str_cy_cam);

					int firstLineNum = name.find('/');
					if (-1 == firstLineNum)
					{
						CFBInterface::Instance()->TextSize(name.c_str(), strlen(name.c_str()), str_cx_cam, str_cy_cam);
					}
					else
					{
						CFBInterface::Instance()->TextSize(name.c_str(), firstLineNum, str_cx_cam, str_cy_cam);
						unsigned short new_str_cx=0, new_str_cy=0;
						CFBInterface::Instance()->TextSize(name.c_str()+firstLineNum+1, strlen(name.c_str())-firstLineNum-1, new_str_cx, new_str_cy);
						if (new_str_cx > str_cx_cam)
						{
							str_cx_cam = new_str_cx;
						}
						str_cy_cam += str_cy_cam;
					}

					x_off = attrib.xPos*width/100;
					if (x_off + str_cx_cam + 2 > width)
					{
						x_off = width - str_cx_cam - 2;
					}
					attrib.xPos = x_off*100/width;

					y_off = attrib.yPos*height/100;
					if (y_off + str_cy_cam + 2 > height)
					{
						y_off = height - str_cy_cam - 2;
					}
					attrib.yPos = y_off*100/height;
#endif

					m_pLocalDeviceMan->SetOSDCamName(i, true, &attrib);
				}
				else
				{
					m_pLocalDeviceMan->SetOSDCamName(i,false);
				}
			}
			delete [] attrib.pData;
			attrib.pData = NULL;

#if defined(__ALARM_TO_UPDATE_FTP_VIDEO__)
			CFtpUpdateMan::Instance()->Action(true);
#endif
			//WriteLog(CConfigEx::Instance()->cfgClient(), LOG_TYPE_CHGE_LOCAL_ENCODE);
		}
		break;
	case NCFG_ITEM_ENCODE_MAJOR_EX:
		{
						
			const ENCODE_INFO_EX *pEncode_temp = reinterpret_cast<const ENCODE_INFO_EX *>(pData);
			ENCODE_INFO_EX *pEncode = const_cast<ENCODE_INFO_EX *>(pEncode_temp);
			for (int i=0; i<m_localVideoInputNum; i++)
			{
				//对配置的最大码率做限制
				if(pEncode[i].hBitStream>=GetMainStreamMaxBitRate(pEncode[i].resolution))
				{
					pEncode[i].hBitStream = GetMainStreamMaxBitRate(pEncode[i].resolution);
				}
				m_pStreamProc->SetFirstStreamEncode(i, pEncode[i]);
			}

			for(int i = 0; i < m_netVideoInputNum; i++)
			{
				if(pEncode[i+m_localVideoInputNum].hBitStream>=GetMainStreamMaxBitRate(pEncode[i+m_localVideoInputNum].resolution))
				{
					pEncode[i+m_localVideoInputNum].hBitStream = GetMainStreamMaxBitRate(pEncode[i+m_localVideoInputNum].resolution);
				}
				m_pStreamProc->SetNetVideoFRate(i, pEncode[i+m_localVideoInputNum].rate, pEncode[i+m_localVideoInputNum].hBitStream);
			}
			
			WriteLog(CConfigEx::Instance()->cfgClient(), LOG_TYPE_CHGE_LOCAL_ENCODE);
		}
		break;
	case NCFG_ITEM_ENCODE_MINOR: //网络子码流流画质, ENCODE_INFO [VIDEO_INPUT_NUM],VBR指等级(数字越大画质越好)CBR指码流
		{		
			if (m_bHasInitial)
			{
				const ENCODE_INFO *pEncode_temp = reinterpret_cast<const ENCODE_INFO *>(pData);
				ENCODE_INFO *pEncode = const_cast<ENCODE_INFO *>(pEncode_temp);
				for (int i=0; i<m_localVideoInputNum; i++)
				{	
					if(pEncode[i].hBitStream>=GetSubStreamMaxBitRate(pEncode[i].resolution))
					{
						pEncode[i].hBitStream = GetSubStreamMaxBitRate(pEncode[i].resolution);
					}
					ENCODE_INFO temp = pEncode[i];
#ifndef __DVR_PROFESSIONAL__
					temp.hBitStream = 0;
#endif
					if (TD_2704HD == CProduct::Instance()->ProductType())
					{
						pLocalDevice->SetVideoSize(i, pEncode[i].resolution, false);
					}
				
					m_pStreamProc->SetSecondStreamEncode(i, temp);							
				}
				
				//修改问题：重新设置了码流，但是没有立即生效，要等到客户端切换画面时才生效。
				CMessageMan::Instance()->PostMsgDataToServer(0, 0, CMD_REQUEST_STREAM_CHANGE, NULL, 0);

			}
			else
			{
				unsigned char maxRate = 0;
				if (VIDEO_FORMAT_PAL == g_video_format)
				{
					maxRate = CProduct::Instance()->NetMaxChannelRatePal();
				}
				else if (VIDEO_FORMAT_NTSC == g_video_format)
				{
					maxRate = CProduct::Instance()->NetMaxChannelRateNtsc();
				}
				else
				{
					assert(false);
				}

				unsigned char rate = 0;
				const ENCODE_INFO *pEncode_temp = reinterpret_cast<const ENCODE_INFO *>(pData);
				ENCODE_INFO *pEncode = const_cast<ENCODE_INFO *>(pEncode_temp);
				for (int i=0; i<m_localVideoInputNum; i++)
				{
					if(pEncode[i].hBitStream>=GetSubStreamMaxBitRate(pEncode[i].resolution))
					{
						pEncode[i].hBitStream = GetSubStreamMaxBitRate(pEncode[i].resolution);
					}
					//只能通过这个接口去生效（AllotLocalSubStream），子码流编码配置
					//重启后默认给，CIF  1帧
					//unsigned long size = pEncode[i].resolution;
					unsigned long size = DVRVIDEO_SIZE_CIF;
					
					CNetStreamAllotDevice::Instance()->GetUsedNetVideoSize(size);

					pLocalDevice->SetVideoSize(i, size, false);

					/*if (UsedRate(pEncode[i].rate, (VIDEO_SIZE)(pEncode[i].resolution)) > maxRate)
					{
						rate = RealRate(maxRate, (VIDEO_SIZE)(pEncode[i].resolution));
					}
					else
					{
						rate = pEncode[i].rate;
					}*/
					rate = 1;

					ENCODE_INFO temp = pEncode[i];
#ifndef __DVR_PROFESSIONAL__
					temp.hBitStream = 0;
#endif

					//pLocalDevice->SetFrameRate(i, rate, false);
					m_pStreamProc->SetSecondStreamEncode(i, temp);

					unsigned char encodeTypeTmp = VIDEO_ENCODE_MODE_CBR;
					unsigned long hBitStreamTmp = 0;
#ifdef __DVR_PROFESSIONAL__
					encodeTypeTmp = pEncode[i].encodeType;
					hBitStreamTmp = pEncode[i].hBitStream;
#else
					//当maxstream等于零时，采用画质档次来处理
					hBitStreamTmp = 0;
#endif	

					//pLocalDevice->SetVideoQuality(i, static_cast<VIDEO_ENCODE_MODE>(pEncode[i].encodeType), pEncode[i].hBitStream, pEncode[i].quality, false);
					pLocalDevice->SetFrameRateQuality(i, rate, static_cast<VIDEO_ENCODE_MODE>(encodeTypeTmp), hBitStreamTmp, pEncode[i].quality, false);
				}
			}

			WriteLog(CConfigEx::Instance()->cfgClient(), LOG_TYPE_NET_STREAM_CFG);
		}
		break;
	case NCFG_ITEM_ENCODE_PHONE:
		break;
	case NCFG_ITEM_ENCODE_MAJOR_EVENT:
		break;
#ifdef __DVR_BASIC__
	case NCFG_ITEM_JPEG_PERIOD:
		{
			const unsigned long *pValue = reinterpret_cast<const unsigned long *>(pData);
			CSnapProc::Instance()->SetSnapTimeS(pValue[0]);
#if defined(__SUPPORT_SNAPD1__)
			{
				CShanghaiSnapMan::GetInstance()->SetSnapTimeS(pValue[0]);
			}					
#else
			CSnapPictureMan::GetInstance()->SetSnapTimeS(pValue[0]);
#endif
			printf("%s,%d,NCFG_ITEM_JPEG_PERIOD:%d\n",__FILE__,__LINE__,pValue[0]);
			WriteLog(CConfigEx::Instance()->cfgClient(), LOG_TYPE_CHGE_SNAP_SETTING);
		}
		break;
	case NCFG_ITEM_JPEG_SIZE:
		{
			const unsigned long *pValue = reinterpret_cast<const unsigned long *>(pData);
			printf("%s,%d,NCFG_ITEM_JPEG_SIZE:%d\n",__FILE__,__LINE__,pValue[0]);

			if(m_localVideoInputNum > 0)
			{
				CSnapCapabilityMan::Instance()->ChangeJPEGVideoSize((VIDEO_SIZE)pValue[0]);
			}

			WriteLog(CConfigEx::Instance()->cfgClient(), LOG_TYPE_CHGE_SNAP_SETTING);
		}
		break;
	case NCFG_ITEM_JPEG_QUALITY:
		{
			const unsigned long *pValue = reinterpret_cast<const unsigned long *>(pData);

			//  pValue 中的值                           下面的CSnapCapabilityMan的对应的值
			//  IMAGE_QUALITY_LOWER		= 0x02,                 5
			//	IMAGE_QUALITY_LOW		= 0x03,                 4
			//	IMAGE_QUALITY_MEDIUM	= 0x04,                 3
			//	IMAGE_QUALITY_HEIGHTER	= 0x05,                 2
			//	IMAGE_QUALITY_HEIGHTEST	= 0x06                  1

            unsigned long JpegQuality = 7 - pValue[0];
			if (JpegQuality <= 0)
			{
				JpegQuality = 1;
			}
			else if (JpegQuality > 5)
			{
				JpegQuality = 5;
			}
			printf("%s,%d,NCFG_ITEM_JPEG_QUALITY:%d ==> %d\n",__FILE__,__LINE__,pValue[0], JpegQuality);
			CSnapCapabilityMan::Instance()->ChangeJPEGVideoLevel(JpegQuality);
			WriteLog(CConfigEx::Instance()->cfgClient(), LOG_TYPE_CHGE_SNAP_SETTING);
		}
		break;
	case NCFG_ITEM_JPEG_SNAP_NUM:
		{
			const unsigned long *pValue = reinterpret_cast<const unsigned long *>(pData);
			printf("%s,%d,NCFG_ITEM_JPEG_SNAP_NUM:%d\n",__FILE__,__LINE__,pValue[0]);
			CSnapProc::Instance()->SetSnapImgNum(pValue[0]);
#if defined(__SUPPORT_SNAPD1__)
			CShanghaiSnapMan::GetInstance()->SetSnapImgNum(pValue[0]);
#else
			CSnapPictureMan::GetInstance()->SetSnapImgNum(pValue[0]);
#endif
			WriteLog(CConfigEx::Instance()->cfgClient(), LOG_TYPE_CHGE_SNAP_SETTING);
		}
		break;
#endif
	case NCFG_ITEM_ENCODE_MAJOR_SPEC:
		break;
	case NCFG_ITEM_IPC_SUPPORT_ENCODE:
		break;
	case NCFG_ITEM_ENCODE_MINOR_EX:
		{
			const ENCODE_INFO_EX *pEncode_temp = reinterpret_cast<const ENCODE_INFO_EX *>(pData);
			ENCODE_INFO_EX *pEncode = const_cast<ENCODE_INFO_EX *>(pEncode_temp);
			for (int i=0; i<m_localVideoInputNum; i++)
			{
				//对配置的最大码率做限制
				if(pEncode[i].hBitStream>=GetSubStreamMaxBitRate(pEncode[i].resolution))
				{
					pEncode[i].hBitStream = GetSubStreamMaxBitRate(pEncode[i].resolution);
				}
			}

			for(int i = 0; i < m_netVideoInputNum; i++)
			{
				if(pEncode[i+m_localVideoInputNum].hBitStream>=GetSubStreamMaxBitRate(pEncode[i+m_localVideoInputNum].resolution))
				{
					pEncode[i+m_localVideoInputNum].hBitStream = GetSubStreamMaxBitRate(pEncode[i+m_localVideoInputNum].resolution);
				}
			}
		}
		break;
	case NCFG_ITEM_BITRANGE_FOR_VIDEOSIZE:
		break;
	case NCFG_ITEM_MAJOR_CHANNEL_RECORD_PARAM_EX:
		break;
	default:
		//此ID还未处理。
		printf("ID(%d) not processed %s, %d\n", pItemHead->itemID, __FILE__, __LINE__);
		assert(false);
		break;
	}
#else
	switch(pItemHead->itemID)
	{
	case NCFG_ITEM_ENCODE_MAJOR:
		{
			//主码流编码参数改为NCFG_ITEM_ENCODE_MAJOR_EX
		}
		break;
	case NCFG_ITEM_ENCODE_MINOR:	//网络子码流流画质, ENCODE_INFO [VIDEO_INPUT_NUM],VBR指等级(数字越大画质越好)CBR指码流
		{		
			if (m_bHasInitial)
			{
				const ENCODE_INFO *pEncode_temp = reinterpret_cast<const ENCODE_INFO *>(pData);
				ENCODE_INFO *pEncode = const_cast<ENCODE_INFO *>(pEncode_temp);
				for (int i=0; i<m_localVideoInputNum; i++)
				{
					//对配置的最大码率做限制
					if(pEncode[i].hBitStream>=GetSubStreamMaxBitRate(pEncode[i].resolution))
					{
						pEncode[i].hBitStream = GetSubStreamMaxBitRate(pEncode[i].resolution);
					}
					m_pStreamProc->SetSecondStreamEncode(i, pEncode[i]);
				}
			}
			else
			{
				unsigned char maxRate = 0;
				if (VIDEO_FORMAT_PAL == g_video_format)
				{
					maxRate = 100;
				}
				else if (VIDEO_FORMAT_NTSC == g_video_format)
				{
					maxRate = 120;
				}
				else
				{
					assert(false);
				}

				unsigned char rate = 0;
				const ENCODE_INFO *pEncode_temp = reinterpret_cast<const ENCODE_INFO *>(pData);
				ENCODE_INFO *pEncode = const_cast<ENCODE_INFO *>(pEncode_temp);
				for (int i=0; i<m_localVideoInputNum; i++)
				{
					//对配置的最大码率做限制
					if(pEncode[i].hBitStream>=GetSubStreamMaxBitRate(pEncode[i].resolution))
					{
						pEncode[i].hBitStream = GetSubStreamMaxBitRate(pEncode[i].resolution);
					}
//					pLocalDevice->SetVideoSize(i, pEncode[i].resolution, false);

					if (UsedRate(pEncode[i].rate, (VIDEO_SIZE)(pEncode[i].resolution)) > maxRate)
					{
						rate = RealRate(maxRate, (VIDEO_SIZE)(pEncode[i].resolution));
					}
					else
					{
						rate = pEncode[i].rate;
					}

//					pLocalDevice->SetFrameRate(i, rate, false);
					m_pStreamProc->SetSecondStreamEncode(i, pEncode[i]);

//					pLocalDevice->SetVideoQuality(i, static_cast<VIDEO_ENCODE_MODE>(pEncode[i].encodeType), pEncode[i].hBitStream, pEncode[i].quality, false);
				}

				WriteLog(CConfigEx::Instance()->cfgClient(), LOG_TYPE_NET_STREAM_CFG);
			}
		}
		break;
	case NCFG_ITEM_ENCODE_PHONE:
		break;
	case NCFG_ITEM_ENCODE_MAJOR_EVENT:
		break;
	case NCFG_ITEM_JPEG_PERIOD:
		break;
	case NCFG_ITEM_JPEG_SIZE:
		break;
	case NCFG_ITEM_JPEG_QUALITY:
		break;
	case NCFG_ITEM_JPEG_SNAP_NUM:
		break;
	case NCFG_ITEM_ENCODE_MAJOR_SPEC:
		break;
	case NCFG_ITEM_ENCODE_MAJOR_EX:
		{
			const ENCODE_INFO_EX *pEncode_temp = reinterpret_cast<const ENCODE_INFO_EX *>(pData);
			ENCODE_INFO_EX *pEncode = const_cast<ENCODE_INFO_EX *>(pEncode_temp);
			for (int i=0; i<m_localVideoInputNum; i++)
			{
				if(pEncode[i].hBitStream>=GetMainStreamMaxBitRate(pEncode[i].resolution))
				{
					pEncode[i].hBitStream = GetMainStreamMaxBitRate(pEncode[i].resolution);
				}
				m_pStreamProc->SetFirstStreamEncode(i, pEncode[i]);
			}
			for(int i = 0; i < m_netVideoInputNum; i++)
			{
				if(pEncode[i+m_localVideoInputNum].hBitStream>=GetMainStreamMaxBitRate(pEncode[i+m_localVideoInputNum].resolution))
				{
					pEncode[i+m_localVideoInputNum].hBitStream = GetMainStreamMaxBitRate(pEncode[i+m_localVideoInputNum].resolution);
				}
				m_pStreamProc->SetNetVideoFRate(i, pEncode[i+m_localVideoInputNum].rate, pEncode[i+m_localVideoInputNum].hBitStream);
			}

		}
		break;
	case NCFG_ITEM_IPC_SUPPORT_ENCODE:
		break;
	case NCFG_ITEM_ENCODE_MINOR_EX:
		break;
	case NCFG_ITEM_BITRANGE_FOR_VIDEOSIZE:
		break;
	case NCFG_ITEM_MAJOR_CHANNEL_RECORD_PARAM_EX:
		break;
	default:
		printf("ID(%d) not processed %s, %d\n", pItemHead->itemID, __FILE__, __LINE__);
		assert(false);
		break;
	}
#endif
}

void CConfigSetMan::EffectPresetCfg(const NCFG_ITEM_HEAD *pItemHead, const unsigned char *pData)
{
	unsigned long chnn = pItemHead->itemID - NCFG_ITEM_PTZ_INFO_PRESET_INFO_CHBASE;
	unsigned long count = pItemHead->num;
	m_pPtzMan->SetPresetConfig(chnn, reinterpret_cast<const PTZ_PRESET*>(pData), count);
}

/*************************************************************************************************

函数：	EffecCruiseCfg(const NCFG_ITEM_HEAD *pItemHead, const unsigned char *pData)
功能：	配置云台的巡航线信息，包括添加巡航线、删除巡航线、改变巡航线
参数：	
		pItemHead 存放通道号和要改变的巡航线个数等信息
		pData存放具体的巡航数据，数据格式如下
		-----------------------------
		|	NCFG_INFO_CRUISE_HEAD	|
		|	.....................	|
		|	.....................	|
		|	NCFG_INFO_CRUISE_HEAD	|
		-----------------------------
		|	PTZ_CURISE_POINT		|
		|	................		|
		|	................		|
		|	PTZ_CURISE_POINT		|
		-----------------------------
**************************************************************************************************/
void CConfigSetMan::EffecCruiseCfg(const NCFG_ITEM_HEAD *pItemHead, const unsigned char *pData)
{
	printf("%s:%s:%d, set the curise \n", __FUNCTION__, __FILE__, __LINE__);
//#ifdef __ENVIRONMENT_LINUX__
	//CPTZControl *pPTZControl = CPTZControl::Instance();

	unsigned long chnn = pItemHead->itemID - NCFG_ITEM_PTZ_INFO_CRUISE_INFO_CHBASE;
	unsigned long curiseCount = pItemHead->num;				//巡航线条数 大于0
	unsigned long curiseNum   = 0;							//巡航线序号 从0到(MAX_CRUISE_NUM-1)
	unsigned long cruisepointNum=0;							//每条巡航线的预置点个数
	const NCFG_INFO_CRUISE_HEAD *pCuriseHead = reinterpret_cast<const NCFG_INFO_CRUISE_HEAD *>(pData);
	const PTZ_CURISE_POINT *pCurisePoint = reinterpret_cast< const PTZ_CURISE_POINT* >(pData + sizeof(NCFG_INFO_CRUISE_HEAD) * curiseCount) ;

	unsigned long count = 0;
	unsigned char* pCuriseParam = NULL;
	unsigned long paramLen = 0;

	for( int i=0; i<curiseCount; i++,pCuriseHead++ )
	{
		curiseNum = pCuriseHead->index;
		cruisepointNum = pCuriseHead->cruisepointnum;
		PTZ_CURISE curise;
		memset(&curise, 0, sizeof(PTZ_CURISE));
		curise.index = pCuriseHead->index;
		curise.presetNum = pCuriseHead->cruisepointnum;
		memcpy(curise.name, pCuriseHead->name, sizeof(curise.name));

		PTZ_CURISE_POINT* pCurisePt = (PTZ_CURISE_POINT*)(pCurisePoint + count);

		if( curise.presetNum > 0 )
		{
			//如果该条巡航线的预置点为0则要删除该条巡航线
			m_pPtzMan->SetCruiseConfig(chnn, curise.index, &curise, pCurisePt, curise.presetNum);
		}
		else
		{
			m_pPtzMan->SetCruiseConfig(chnn, curise.presetNum, &curise, NULL, 0);
		}

		//设置生效,第一次启动只读取配置值
		if (m_bHasInitial)
		{
			unsigned long len = sizeof(unsigned long) * 2 + sizeof(PTZ_CURISE_POINT) * curise.presetNum;
			if( len > paramLen )
			{
				if( pCuriseParam )
				{
					delete [] pCuriseParam;
				}
				pCuriseParam = new unsigned char[len];
				paramLen = len;
			}
			memset(pCuriseParam, 0, paramLen);
			unsigned long* pLong = reinterpret_cast<unsigned long*>(pCuriseParam);
			*pLong = curise.index;
			pLong += 1;
			*pLong = curise.presetNum;

			unsigned char* pPt = pCuriseParam + sizeof(unsigned long) * 2;
			if( curise.presetNum > 0 )
			{
				memcpy(pPt, pCurisePt, sizeof(PTZ_CURISE_POINT) * curise.presetNum);
			}

			//向云台中设置巡航线
			printf("%s:%s:%d, set thr curise \n", __FUNCTION__, __FILE__, __LINE__);
			m_pPtzMan->Action(chnn, PTZ_CMD_CRUISE_CFG, 0, 0, 0, pCuriseParam);
		}
		else
		{
#ifdef __ENVIRONMENT_LINUX__
			if( curise.presetNum > 0 )
			{
				CFakeCurise::Instance()->InitChnnFakeCurise(chnn, &curise, pCurisePt);
			}
			else
			{
				CFakeCurise::Instance()->InitChnnFakeCurise(chnn, &curise, NULL);
			}
#endif
		}

		count += curise.presetNum;
	}

	if( pCuriseParam )
	{
		delete [] pCuriseParam;
		pCuriseParam = NULL;
	}

	if (m_bHasInitial)
	{
		WriteLog(CConfigEx::Instance()->cfgClient(), LOG_TYPE_CRUISE_MODIFY);
	}
//#endif

}

void CConfigSetMan::EffecScheduleHolidayCfg(const NCFG_ITEM_HEAD *pItemHead, const unsigned char *pData)
{
	CMyList <HOLIDAY_SCHEDULE> list;	 
	HOLIDAY_SCHEDULE holiday;
	NCFG_INFO_HOLIDAY_SCHE_HEAD holidaySchHead;

	unsigned long schGroupNum = 0;
	if((pItemHead->itemID == NCFG_ITEM_REC_SCH_SCHEDULE_HOLIDAY) || \
		(pItemHead->itemID == NCFG_ITEM_REC_SCH_MOTION_HOLIDAY) || \
		(pItemHead->itemID == NCFG_ITEM_REC_SCH_SENSOR_HOLIDAY) || \
		(pItemHead->itemID == NCFG_ITEM_MOTION_SCH_HOLIDAY) || \
		(pItemHead->itemID == NCFG_ITEM_NET_ALARM_SCH_HOLIDAY))
	{
		schGroupNum = m_videoInputNum;
	}
	else if(pItemHead->itemID == NCFG_ITEM_SENSOR_IN_SCH_HOLIDAY)
	{
		schGroupNum = m_sensorInputNum;
	}
	else if(pItemHead->itemID == NCFG_ITEM_ALARMOUT_SCH_HOLIDAY)
	{
		schGroupNum = m_sensorOutputNum;
	}
	else
	{
		schGroupNum = 0;
	}

	for (int i=0; i<schGroupNum; i++)
	{
		memcpy(&holidaySchHead, (pData+i*sizeof(NCFG_INFO_HOLIDAY_SCHE_HEAD)), sizeof(NCFG_INFO_HOLIDAY_SCHE_HEAD));
		for (ULONG j=0; j<holidaySchHead.holidaynum; j++)
		{
			memcpy(&holiday, pData+(holidaySchHead.offset+j*sizeof(HOLIDAY_SCHEDULE)), sizeof(HOLIDAY_SCHEDULE));
			list.AddTail(holiday);
		}
		
		switch(pItemHead->itemID)
		{
		case NCFG_ITEM_REC_SCH_SCHEDULE_HOLIDAY:
			CStreamRecordManEx::Instance()->SetHolidaySchedule(RECORD_TYPE_SCHEDULE, list, i);
			break;
		case NCFG_ITEM_REC_SCH_MOTION_HOLIDAY:
			CStreamRecordManEx::Instance()->SetHolidaySchedule(RECORD_TYPE_MOTION, list, i);
			break;
		case NCFG_ITEM_REC_SCH_SENSOR_HOLIDAY:
			CStreamRecordManEx::Instance()->SetHolidaySchedule(RECORD_TYPE_SENSOR, list, i);
		    break;
		case NCFG_ITEM_SENSOR_IN_SCH_HOLIDAY:
			break;
		case NCFG_ITEM_ALARMOUT_SCH_HOLIDAY:
			break;
		case NCFG_ITEM_MOTION_SCH_HOLIDAY:
			break;
		case NCFG_ITEM_NET_ALARM_SCH_HOLIDAY:
			break;
		default:
		    break;
		}

		//销毁链表
		list.RemoveAll();
	}
}

#ifdef __GB28181_SERVER__
void CConfigSetMan::EffectGB28181Cfg(const NCFG_ITEM_HEAD *pItemHead, const unsigned char *pData)
{
	switch(pItemHead->itemID)
	{
	case NCFG_ITEM_GB28181_SERVER_INFO:
		CGb28181Server::Instance()->SetSipServerConfig(*(GB28181_SIP_SERVER *)pData);
		break;
	case NCFG_ITEM_GB28181_CHANNEL_ID:
		CGb28181Server::Instance()->SetGb28181ChannelIDConfig(*(GB28181_CHANNEL_ID *)pData);
		break;
	case NCFG_ITEM_GB28181_ALARM_ID:
		CGb28181Server::Instance()->SetGb28181AlarmIDConfig(*(GB28181_ALARM_ID *)pData);
		break;
	default:
		break;
	}
}
#endif

#ifdef __MOBILE_DVR__
void CConfigSetMan::EffecBasicSpeedCfg(const NCFG_ITEM_HEAD *pItemHead, const unsigned char *pData)
{
	switch(pItemHead->itemID)
	{
	case NCFG_ITEM_SPEED_SOURCE:
		{
			m_pspeedMan->SetSpeedSource(*reinterpret_cast< const unsigned long*>(pData));
		}
		break;
	case NCFG_ITEM_SPEED_UNIT:
		{
			m_pspeedMan->SetSpeedUnit(*reinterpret_cast< const unsigned long*>(pData));
		}
		break;
	default:
		break;
	}
}

void CConfigSetMan::EffecHighSpeedCfg(const NCFG_ITEM_HEAD *pItemHead, const unsigned char *pData)
{
	switch (pItemHead->itemID)
	{
	case NCFG_ITEM_HIGH_SPEED_ENABLE:
		{
			const unsigned long *pTempData = reinterpret_cast<const unsigned long *>(pData);
			m_pspeedMan->SetEnable((*pTempData ? true : false), 0);
			//暂不写日志，还没有定义类型，后面需要补上
			//WriteLog(CConfigEx::Instance()->cfgClient(), LOG_TYPE_CHGE_SENSOR_SWITCH);
		}
		break;
	case NCFG_ITEM_HIGH_SPEED_VALUE:
		{
			//char *pTempData = reinterpret_cast<char*>(pData);
			m_pspeedMan->SetSpeedValue((char *)pData, 0);
		}	
		break;
	case NCFG_ITEM_HIGH_SPEED_HOLD_TIME:
		{
			const unsigned long *pHoldTime = reinterpret_cast<const unsigned long*>(pData);
			m_pspeedMan->SetHoldTime(*pHoldTime, 0);
		}
		break;
	case NCFG_ITEM_HIGH_SPEED_BUZZ:
		{
			const unsigned long *pToBuzzer = reinterpret_cast<const unsigned long *>(pData);
			m_pspeedMan->SetToBuzzer((*(pToBuzzer) << m_sensorOutputNum), 0, m_sensorOutputNum );		
		}
		break;
	case NCFG_ITEM_HIGH_SPEED_TO_ALARM_OUT:
		{
			const ULONGLONG *pToAlarmOut = reinterpret_cast<const ULONGLONG *>(pData);
			m_pspeedMan->SetToAlarmOutCH(*pToAlarmOut, 0, m_sensorOutputNum);
		}
		break;
	case NCFG_ITEM_HIGH_SPEED_TO_RECORD:
		{
			const ULONGLONG *pToRecord = reinterpret_cast<const ULONGLONG *>(pData);
			m_pspeedMan->SetToRecCH(*pToRecord, 0);
		}
		break;
	case NCFG_ITEM_HIGH_SPEED_BIG_VIEW:
		{
			const ULONGLONG *pToBigView = reinterpret_cast<const ULONGLONG *>(pData);
			m_pspeedMan->SetToBigViewCH(*pToBigView, 0);
		}
		break;
	case NCFG_ITEM_HIGH_SPEED_TO_PTZ:
		{
			if(m_localVideoInputNum > 0)
			{
				m_pspeedMan->SetToPTZ(reinterpret_cast<const TO_PTZ *>(pData), 2, m_localVideoInputNum );
			}
		}
		break;
#ifdef __DVR_BASIC__	
	case NCFG_ITEM_HIGH_SPEED_TO_EMAIL_ENABLE:
		{
		}
		break;
	case NCFG_ITEM_HIGH_SPEED_TO_EMAIL_ADDR:
		{
		}
		break;
#endif
	default:
		break;
	}
}

void CConfigSetMan::EffecLowSpeedCfg(const NCFG_ITEM_HEAD *pItemHead, const unsigned char *pData)
{
	switch(pItemHead->itemID)
	{
	case NCFG_ITEM_LOW_SPEED_ENABLE:
		{
			const unsigned long *pTempData = reinterpret_cast<const unsigned long *>(pData);
			m_pspeedMan->SetEnable((*pTempData ? true : false), 1);
			//暂不写日志，还没有定义类型，后面需要补上
			//WriteLog(CConfigEx::Instance()->cfgClient(), LOG_TYPE_CHGE_SENSOR_SWITCH);
		}
		break;
	case NCFG_ITEM_LOW_SPEED_VALUE:
		{
			//char *pTempData = reinterpret_cast<const unsigned char *>(pData);
			m_pspeedMan->SetSpeedValue((char*)pData, 1);
		}	
		break;
	case NCFG_ITEM_LOW_SPEED_HOLD_TIME:
		{
			const unsigned long *pHoldTime = reinterpret_cast<const unsigned long*>(pData);
			m_pspeedMan->SetHoldTime(*pHoldTime, 1);
		}
		break;
	case NCFG_ITEM_LOW_SPEED_BUZZ:
		{
			const unsigned long *pToBuzzer = reinterpret_cast<const unsigned long *>(pData);
			m_pspeedMan->SetToBuzzer((*(pToBuzzer) << m_sensorOutputNum), 1, m_sensorOutputNum );		
		}
		break;
	case NCFG_ITEM_LOW_SPEED_TO_ALARM_OUT:
		{
			const ULONGLONG *pToAlarmOut = reinterpret_cast<const ULONGLONG *>(pData);
			m_pspeedMan->SetToAlarmOutCH(*pToAlarmOut, 1, m_sensorOutputNum);
		}
		break;
	case NCFG_ITEM_LOW_SPEED_TO_RECORD:
		{
			const ULONGLONG *pToRecord = reinterpret_cast<const ULONGLONG *>(pData);
			m_pspeedMan->SetToRecCH(*pToRecord, 1);
		}
		break;
	case NCFG_ITEM_LOW_SPEED_BIG_VIEW:
		{
			const ULONGLONG *pToBigView = reinterpret_cast<const ULONGLONG *>(pData);
			m_pspeedMan->SetToBigViewCH(*pToBigView, 1);
		}
		break;
	case NCFG_ITEM_LOW_SPEED_TO_PTZ:
		{
			if(m_localVideoInputNum > 0)
			{
				m_pspeedMan->SetToPTZ(reinterpret_cast<const TO_PTZ *>(pData), 2, m_localVideoInputNum );
			}
		}
		break;
#ifdef __DVR_BASIC__	
	case NCFG_ITEM_LOW_SPEED_TO_EMAIL_ENABLE:
		{
		}
		break;
	case NCFG_ITEM_LOW_SPEED_TO_EMAIL_ADDR:
		{
		}
		break;
#endif
	default:
		break;
	}
}
#endif

void CConfigSetMan::WriteLog(unsigned long clientID, LOG_TYPE type)
{
	if (m_bHasInitial)
	{
		//初始化好才写日志。
		USER_INFO user;
		if (CUserMan::Instance()->GetUser(user, clientID))
		{
			OPERATOR_LOG log;
			log.IP		= user.IP;
			strcpy(log.name, user.name);

			log.contentID = GetContentID(type);
			log.type = (type & 0xffff);

			log.time = GetCurrTime32();

			m_pLogMan->WriteOperatorLog(log);
		}
	}
}
void CConfigSetMan::ChangeOSDname(std::string &name)
{	
	int delPos = name.find('/');	
	while(-1 != delPos)
	{
		name = name.erase(delPos,1);
		delPos = name.find('/', delPos);
	}

}


#ifdef __SUPPORT_OSDTWOLINES__
void CConfigSetMan::ChangeOSDnameTwoLines(std::string &name)
{
	int count = 0;
	for(int i = 0; i < name.size();i++)
	{
		if(name[i] != '/')
		{			
			break;
		}
		else
		{
			count++;
		}
	}
	if (count > 0)
	{
		name.erase(0,count);	
	}

	int firest = name.find('/');
	if (firest != -1)
	{
		int delPos = name.find('/', firest+1);
		while(-1 != delPos)
		{
			name = name.erase(delPos, 1);			
			delPos = name.find('/', firest+1);
		}
	}

	{
		int size = name.size();
		if (name[size -1] == '/')
		{
			name.erase(size -1,1);
		}

	}
}
#endif


void CConfigSetMan::RefreshOSDName(unsigned long chnn)
{
#ifdef __ENVIRONMENT_LINUX__
	//修改了通道名称，其OSD应该相应地改变
	if (m_bHasInitial)
	{
		OSD_ATTRIB attrib;
		memset(&attrib, 0, sizeof(OSD_ATTRIB));
		attrib.length = 5120;
		attrib.pData = new unsigned char [attrib.length];
		attrib.cy = 20;

		std::string name;

		if(chnn < m_videoInputNum)
		{
			if (m_pOSDInfoCamName[chnn].bEnable)
			{
				attrib.xPos = m_pOSDInfoCamName[chnn].x;
				attrib.yPos = m_pOSDInfoCamName[chnn].y;
				attrib.cx	= 20 * name.length();

				attrib.length = 5120;

				CMessageMan::Instance()->GetCamName(chnn, name);
#ifdef __SUPPORT_OSDTWOLINES__
				ChangeOSDnameTwoLines(name);
#else
				ChangeOSDname(name);
#endif


				CFBInterface::Instance()->GetFont(attrib.pData, attrib.length, name.c_str(), name.length());
#ifdef __SUPPORT_OSDTWOLINES__
				unsigned short x_off, y_off,str_cx_cam,str_cy_cam;
				int height,width;
				m_pLocalDeviceMan->GetVideoSize(chnn, width, height);
				CFBInterface::Instance()->TextSize(name.c_str(), strlen(name.c_str()), str_cx_cam, str_cy_cam);

				int firstLineNum = name.find('/');
				if (-1 == firstLineNum)
				{
					CFBInterface::Instance()->TextSize(name.c_str(), strlen(name.c_str()), str_cx_cam, str_cy_cam);
				}
				else
				{
					CFBInterface::Instance()->TextSize(name.c_str(), firstLineNum, str_cx_cam, str_cy_cam);
					unsigned short new_str_cx=0, new_str_cy=0;
					CFBInterface::Instance()->TextSize(name.c_str()+firstLineNum+1, strlen(name.c_str())-firstLineNum-1, new_str_cx, new_str_cy);
					if (new_str_cx > str_cx_cam)
					{
						str_cx_cam = new_str_cx;
					}
					str_cy_cam += str_cy_cam;
				}



				x_off = attrib.xPos*width/100;
				if (x_off + str_cx_cam + 2 > width)
				{
					x_off = width - str_cx_cam - 2;
				}
				attrib.xPos = x_off*100/width;

				y_off = attrib.yPos*height/100;
				if (y_off + str_cy_cam + 2 > height)
				{
					y_off = height - str_cy_cam - 2;
				}
				attrib.yPos = y_off*100/height;
#endif

				m_pLocalDeviceMan->SetOSDCamName(chnn, true, &attrib);
			}
		}

		delete [] attrib.pData;
		attrib.pData = NULL;
	}
#endif
}

//end
