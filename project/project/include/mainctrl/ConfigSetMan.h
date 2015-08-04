/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : xkg
** Date         : 2009-05-22
** Name         : ConfigSetMan.h
** Version      : 1.0
** Description  :
** Modify Record:
***********************************************************************/

#ifndef _CONFIG_SET_MAN_H_
#define _CONFIG_SET_MAN_H_
#include "NetConfigDefine.h"
#include "RecordMan.h"
#include "MotionMan.h"
#include "SensorInMan.h"
#include "VideoLossMan.h"
#include "OtherAlarmMan.h"
#include "AlarmOutMan.h"
#include "ReclogManEx.h"
#include "LogMan.h"
#include "DisplayMan.h"
#include "PtzMan.h"
#ifdef __ENVIRONMENT_LINUX__
#include "FakeCurise.h"
#endif
#include "NetReader.h"
#include "StreamProc.h"

#ifdef __DVR_ULTIMATE__
#include "NetDeviceManager.h"
#endif

#include "WirelessManage.h"

#ifdef __ENVIRONMENT_LINUX__
#include "NetInterface.h"
#include "DdnsManager.h"
#include "RemoteControl.h"
#include "ExternalKeyboard.h"
#include "TZoneDZoneCfg.h"
#include "AllocNICIP.h"
#include "LocalDevice.h"
#endif

#ifdef __MOBILE_DVR__
#include "SpeedMan.h"
#endif

class CConfigSetMan
{
public:
	CConfigSetMan();
	~CConfigSetMan();

#ifdef __MOBILE_DVR__
	bool Initial(unsigned char localChnnNum, unsigned char netChnnNum, unsigned char sensorInputNum, unsigned char sensorOutputNum, \
				CRecordMan *pRecMan, CMotionMan *pMotionMan, CSensorInMan *pSensorMan, CVideoLossMan *pVideoLossMan, COtherAlarmMan *pOtherAlarm, CAlarmOutMan *pAlarmOutMan,\
				CReclogManEX *pReclogMan, CLogMan *pLogMan, CLocalDevice *pLocalDeviceMan,CDisplayMan* pDisplayMan, CPtzMan* pPtzMan, \
				CNetReader* pNetReader, CStreamProc *pStreamProc, CSpeedMan *pSpeedMan);
#elif defined(__DVR_ULTIMATE__)
	bool Initial(unsigned char localChnnNum, unsigned char netChnnNum, unsigned char localSensorInputNum, unsigned char sensorInputNum, unsigned char sensorOutputNum, CRecordMan *pRecMan, CMotionMan *pMotionMan,\
		         CSensorInMan *pSensorMan, CVideoLossMan *pVideoLossMan, COtherAlarmMan *pOtherAlarm, CAlarmOutMan *pAlarmOutMan, CReclogManEX *pReclogMan, CLogMan *pLogMan, CLocalDevice *pLocalDeviceMan,\
				 CDisplayMan* pDisplayMan, CPtzMan* pPtzMan, CNetReader* pNetReader, CStreamProc *pStreamProc, CNetDeviceManager* pCfgNetDeviceMan);
#else
	bool Initial(unsigned char localChnnNum, unsigned char netChnnNum, unsigned char localSensorInputNum, unsigned char sensorInputNum, unsigned char sensorOutputNum, CRecordMan *pRecMan, CMotionMan *pMotionMan,\
				CSensorInMan *pSensorMan, CVideoLossMan *pVideoLossMan, COtherAlarmMan *pOtherAlarm, CAlarmOutMan *pAlarmOutMan, CReclogManEX *pReclogMan, CLogMan *pLogMan, CLocalDevice *pLocalDeviceMan,\
				CDisplayMan* pDisplayMan, CPtzMan* pPtzMan, CNetReader* pNetReader, CStreamProc *pStreamProc);
#endif

	void Quit();
	void SetHasIntial(){m_bHasInitial = true;};
	void TakeEffect(const unsigned char *pDataBlock, unsigned long dataLen); 
	void RefreshOSDName(unsigned long chnn);
protected:
	
private:
	void EffectSystemCfg(const NCFG_ITEM_HEAD *pItemHead, const unsigned char *pData);
	void EffectChannCfg(const NCFG_ITEM_HEAD *pItemHead, const unsigned char *pData);
	void EffectChannRecCfg(const NCFG_ITEM_HEAD *pItemHead, const unsigned char *pData);
	void EffectScheduleCfg(const NCFG_ITEM_HEAD *pItemHead, const unsigned char *pData);
	void EffectNetBaseCfg(const NCFG_ITEM_HEAD *pItemHead, const unsigned char *pData);
	void EffectAlarmTrigerCfg(const NCFG_ITEM_HEAD *pItemHead, const unsigned char *pData);
	void EffectAlarmTrigerSensorCfg(const NCFG_ITEM_HEAD *pItemHead, const unsigned char *pData);
	void EffectAlarmTrigerMotionCfg(const NCFG_ITEM_HEAD *pItemHead, const unsigned char *pData);
	void EffectAlarmTrigerVLossCfg(const NCFG_ITEM_HEAD *pItemHead, const unsigned char *pData);
	void EffectAlarmTrigerShelterCfg(const NCFG_ITEM_HEAD *pItemHead, const unsigned char *pData);
	void EffectAlarmExceptionCfg(const NCFG_ITEM_HEAD *pItemHead, const unsigned char *pData);
	void EffectEncodeCfg(const NCFG_ITEM_HEAD *pItemHead, const unsigned char *pData);

	void EffectPresetCfg(const NCFG_ITEM_HEAD *pItemHead, const unsigned char *pData);
	void EffecCruiseCfg(const NCFG_ITEM_HEAD *pItemHead, const unsigned char *pData);
	void EffecScheduleHolidayCfg(const NCFG_ITEM_HEAD *pItemHead, const unsigned char *pData);

#ifdef __GB28181_SERVER__
	void EffectGB28181Cfg(const NCFG_ITEM_HEAD *pItemHead, const unsigned char *pData);
#endif

#ifdef __MOBILE_DVR__
	void EffecBasicSpeedCfg(const NCFG_ITEM_HEAD *pItemHead, const unsigned char *pData);
	void EffecHighSpeedCfg(const NCFG_ITEM_HEAD *pItemHead, const unsigned char *pData);
	void EffecLowSpeedCfg(const NCFG_ITEM_HEAD *pItemHead, const unsigned char *pData);
	CSpeedMan		*m_pspeedMan;
#endif
	//////////////////////////////////////////////////////////////////////////
	void WriteLog(unsigned long clientID, LOG_TYPE type);
	//////////////////////////////////////////////////////////////////////////
	//data
	bool			m_bHasInitial;

	unsigned char	m_videoInputNum;
	unsigned char	m_localVideoInputNum;
	unsigned char	m_netVideoInputNum;
	unsigned char	m_localSensorInputNum;
	unsigned char	m_sensorInputNum;
	unsigned char	m_sensorOutputNum;
	CRecordMan		*m_pRecMan;
	CMotionMan		*m_pMotionMan;
	CSensorInMan	*m_pSensorMan;
	CVideoLossMan	*m_pVideoLossMan;
	COtherAlarmMan	*m_pOtherAlarm;
	CAlarmOutMan	*m_pAlarmOutMan;
	CReclogManEX    *m_pReclogMan;
	CLogMan         *m_pLogMan;	
	CDisplayMan		*m_pDisplayMan;
	CPtzMan			*m_pPtzMan;
	CNetReader      *m_pNetReader;
	CStreamProc		*m_pStreamProc;
#ifdef __ENVIRONMENT_LINUX__
	CNetInterface	*m_pNetInterface;
	CLocalDevice    *m_pLocalDeviceMan;
	OSD_INFO		*m_pOSDInfoCamName;
#endif

#ifdef __DVR_ULTIMATE__
	CNetDeviceManager *m_pNetDeviceMan;
#endif
	
private:

	void ChangeOSDname(std::string &name);
#ifdef __SUPPORT_OSDTWOLINES__
	void ChangeOSDnameTwoLines(std::string &name);
#endif
#ifdef __ENVIRONMENT_LINUX__
	CAMERA_COVER_INFO m_currentCover[64];
	MOTION_DETECT_INFO m_motionDetectInfo[64];
#endif

	ENCODE_INFO		m_encode[64];
	ENCODE_INFO_EX	m_encodeEx[64];
	unsigned long	m_setEncodeCount[64];
};

#endif
