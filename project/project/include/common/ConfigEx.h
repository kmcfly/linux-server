/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : 袁石维
** Date         : 2009-03-26
** Name         : ConfigEx.h
** Version      : 1.0
** Description  :
** Modify Record:
1:
***********************************************************************/
#ifndef _CONFIG_EX_H_
#define _CONFIG_EX_H_

#include "Product.h"
#include "PUB_common.h"
#include "NetConfigDefine.h"
#include "mylist.h"
#include "NetProtocol.h"
#include "CruiseConfig.h"
#include "PresetConfig.h"
#include "ScheduleConfig.h"
#include "LanguagePack.h"
#include "ReadDdns.h"
#if defined(__SUPPORT_SNAPD1__)
#include "ShanghaiCmd.h"
#include "ShanghaiConnectMan.h"
#endif
typedef struct _config_cfg_head
{
	unsigned long   version;				//配置定义版本号
	unsigned long   headLen;				//本结构体长度
	unsigned long   productID;				//产品ID
	unsigned long   subProductID;			//产品子ID
	unsigned long   firmwareVersion;		//固件版本号

	unsigned long	fileLength;				//文件长度
	unsigned long	itemnum;				//配置项数目

	unsigned long	videoInputNum;
	unsigned long	localVideoInputNum;
	unsigned long	netVideoInputNum;

	unsigned long	audioInputNum;
	unsigned long	sensorInputNum;
	unsigned long	sensorOutputNum;
}CONFIG_CFG_HEAD;

const unsigned long CONFIG_DATA_POS = sizeof(CONFIG_CFG_HEAD) + 32;

typedef enum _default_cfg
{
	DEFAULT_CFG_NULL					= 0x0,

	DEFAULT_CFG_SYSTEM_BASE				= (0x01<<0),//此范围ID存储系统基本配置的杂项
	DEFAULT_CFG_CHANNEL_BASE			= (0x01<<1),//通道配置
	DEFAULT_CFG_CHANNEL_REC_BASE		= (0x01<<2),//通道录像配置
	DEFAULT_CFG_ENCODE_BASE				= (0x01<<3),//通道编码配置
	DEFAULT_CFG_SCHEDULE_BASE			= (0x01<<4),//计划
	DEFAULT_CFG_NET_BASE				= (0x01<<5),//网络功能
	DEFAULT_CFG_ALARM_TRIGER_BASE		= (0x01<<6),//报警联动功能
	DEFAULT_CFG_ALARM_TRIGER_SENSORBASE	= (0x01<<7),
	DEFAULT_CFG_ALARM_TRIGER_MOTIONBASE	= (0x01<<8),
	DEFAULT_CFG_ALARM_TRIGER_VLOSSBASE	= (0x01<<9),
	DEFAULT_CFG_ALARM_TRIGER_SHELTERBASE= (0x01<<10),

	//车载相关
	DEFAULT_CFG_MOBILE_BASE				= (0x01<<11),
	DEFAULT_CFG_SPEED_BASE				= (0x01<<12),
	DEFAULT_CFG_HIGH_SPEED_BASE			= (0x01<<13),
	DEFAULT_CFG_LOW_SPEED_BASE			= (0x01<<14),
	DEFAULT_CFG_WALLOP_BASE				= (0x01<<15),
	DEFAULT_CFG_WIRELESS_BASE			= (0x01<<16),
	//车载相关-end

	DEFAULT_CFG_EXCEPTION_BASE			= (0x01<<17), //其他异常报警

	DEFAULT_CFG_PRESET					= (0x01<<28), //预制点
	DEFAULT_CFG_CRUISE					= (0x01<<29), //巡航线
	DEFAULT_CFG_HOLIDAY_SCHEDULE		= (0x01<<30), //假日排程
}DEFAULT_CFG;

class CConfigEx
{
public:
	~CConfigEx ();
	static CConfigEx* Instance();
	bool Initial (const char *pFilePath = NULL);
	void Quit ();
	bool IsCreatNewFile() { return m_bCreatNewFile;};

	//////////////////////////////////////////////////////////////////////////
	void EnterConfig(unsigned long clientID = LOCAL_DEVICE_ID);
	void ExitConfig();
	inline unsigned long cfgClient(){return m_clientID;};

	bool GetConfig(unsigned char *pItemHead, unsigned long headLen, unsigned char **pDataBlock,unsigned long &dataLen);
	bool GetConfigEx(CMyList <NCFG_ITEM_ID> &itemList, unsigned char **pDataBlock, unsigned long &dataLen);
	bool GetDefaultData(unsigned char *pItemHead, unsigned long headLen, unsigned char **pDataBlock,unsigned long &dataLen, bool bChangtoCVBS);
	bool SetConfig(const unsigned char *pDataBlock, unsigned long dataLen);
	bool LoadDefault(unsigned long param);
protected:
private:
	//methods
	CConfigEx ();

	bool InitialConfig ();	//假如没有Config.dat文件存在，则需要调用该方法创建。
	bool LoadConfig (const char *pFilePath);		//假如有Config.dat文件存在，则需要调用该方法加载数据。
	//////////////////////////////////////////////////////////////////////////
	bool InitialHead();		//初始化文件头
	bool InitialBody();		//初始化配置体
	bool InitialPTZ();

	bool SetConfigBlock(const unsigned char *pDataBlock,  unsigned long num, unsigned long dataLen, bool bReWrite=false, bool bAddInConfig=false); //id有序，头和数据间隔存储
	bool GetConfigBlock(CMyList <NCFG_ITEM_HEAD> & itemHeadList, unsigned char **pDataBlock,unsigned long &Len, unsigned short &itemNum);

	bool LoadDefaultConfig(unsigned long param, bool bReWrite =false, bool bAddInConfig = false);
	bool WriteConfig(unsigned char *pConfig, unsigned long len, size_t pos);
	bool InitialConfigItemHead(); //初始化配置索引链表。

	bool OnlyReadInfoSize(unsigned short itemID, unsigned short &len, unsigned short &subNum);
	bool OnlyReadInfo(unsigned short itemID, unsigned char *pData, unsigned short len);
	bool GetPresetConfigInfo(unsigned itemID, unsigned char *PData, unsigned short &dataLen, unsigned short &subNum);
	bool SetPresetConfigInfo(unsigned itemID, const unsigned char *PData,unsigned short subNum);
	bool GetCruiseConfigInfo(unsigned itemID, unsigned char *PData, unsigned short &dataLen, unsigned short &subNum);
	bool SetCruiseConfigInfo(unsigned itemID, const unsigned char *PData,unsigned short subNum);
	bool GetHolidayScheCfgInfo(unsigned itemID, unsigned char *PData, unsigned short &dataLen, unsigned short &subNum);
	bool SetHolidayScheCfgInfo(unsigned itemID, const unsigned char *PData,unsigned short subNum);

	void CalculateSystemCfgSize(unsigned short &itemNum, unsigned long &bufLen);
	void CalculateChannCfgSize(unsigned short &itemNum, unsigned long &bufLen);
	void CalculateChannRecCfgSize(unsigned short &itemNum, unsigned long &bufLen);
	void CalculateScheduleCfgSize(unsigned short &itemNum, unsigned long &bufLen);
	void CalculateNetBaseCfgSize(unsigned short &itemNum, unsigned long &bufLen);
	void CalculateAlarmTrigerCfgSize(unsigned short &itemNum, unsigned long &bufLen);
	void CalculateAlarmTrigerSensorCfgSize(unsigned short &itemNum, unsigned long &bufLen);
	void CalculateAlarmTrigerMotionCfgSize(unsigned short &itemNum, unsigned long &bufLen);
	void CalculateAlarmTrigerVLossCfgSize(unsigned short &itemNum, unsigned long &bufLen);
	void CalculateAlarmTrigerShelterCfgSize(unsigned short &itemNum, unsigned long &bufLen);
	void CalculateAlarmExceptionCfgSize(unsigned short &itemNum, unsigned long &bufLen);
	void CalculateEncodeCfgSize(unsigned short &itemNum, unsigned long &bufLen);
#ifdef __GB28181_SERVER__
	void CalculateGB28181CfgSize(unsigned short &itemNum, unsigned long &bufLen);
#endif

#ifdef __MOBILE_DVR__
	void CalculateMobileCfgSize(unsigned short &itemNum, unsigned long &bufLen);
	void CalculateSpeedCfgSize(unsigned short &itemNum, unsigned long &bufLen);
	void CalculateHighSpeedCfgSize(unsigned short &itemNum, unsigned long &bufLen);
	void CalculateLowSpeedCfgSize(unsigned short &itemNum, unsigned long &bufLen);
	void CalculateWallopCfgSize(unsigned short &itemNum, unsigned long &bufLen);
	void CalculateWirelessCfgSize(unsigned short &itemNum, unsigned long &bufLen);
#endif
	bool LoadBackupConfig(unsigned char **ppBackupConfig, unsigned long & len);
	void LoadSystemCfg(unsigned char **ppConfig, unsigned long & len, unsigned short & num, unsigned char *ppBackupConfig, unsigned long backupLen, unsigned long* pos,bool bChangtoCVBS);
	void LoadChannCfg(unsigned char **ppConfig, unsigned long & len, unsigned short & num, unsigned char *ppBackupConfig = NULL, unsigned long backupLen = 0, unsigned long* pos = NULL);
	void LoadChannRecCfg(unsigned char **ppConfig, unsigned long & len, unsigned short & num, unsigned char *ppBackupConfig = NULL, unsigned long backupLen = 0, unsigned long* pos = NULL);
	void LoadScheduleCfg(unsigned char **ppConfig, unsigned long & len, unsigned short & num, unsigned char *ppBackupConfig = NULL, unsigned long backupLen = 0, unsigned long* pos = NULL);
	void LoadNetBaseCfg(unsigned char **ppConfig, unsigned long & len, unsigned short & num, unsigned char *ppBackupConfig = NULL, unsigned long backupLen = 0, unsigned long *pos = NULL);
	bool GetItemDataFromBackupConfig(unsigned char* pConfig, unsigned char* pBackupConfig, unsigned long oldConfigLen,  unsigned long *pos);
	void LoadAlarmTrigerCfg(unsigned char **ppConfig, unsigned long & len, unsigned short & num, unsigned char *ppBackupConfig = NULL, unsigned long backupLen = 0, unsigned long* pos = NULL);
	void LoadAlarmTrigerSensorCfg(unsigned char **ppConfig, unsigned long & len, unsigned short & num, unsigned char *ppBackupConfig = NULL, unsigned long backupLen = 0, unsigned long* pos = NULL);
	void LoadAlarmTrigerMotionCfg(unsigned char **ppConfig, unsigned long & len, unsigned short & num, unsigned char *ppBackupConfig = NULL, unsigned long backupLen = 0, unsigned long* pos = NULL);
	void LoadAlarmTrigerVLossCfg(unsigned char **ppConfig, unsigned long & len, unsigned short & num, unsigned char *ppBackupConfig = NULL, unsigned long backupLen = 0, unsigned long* pos = NULL);
	void LoadAlarmTrigerShelterCfg(unsigned char **ppConfig, unsigned long & len, unsigned short & num, unsigned char *ppBackupConfig = NULL, unsigned long backupLen = 0, unsigned long* pos = NULL);
	void LoadAlarmExceptionCfg(unsigned char **ppConfig, unsigned long &len, unsigned short &num, unsigned char *ppBackupConfig = NULL, unsigned long backupLen = 0, unsigned long* pos = NULL);
	void LoadEncodeCfg(unsigned char **ppConfig, unsigned long & len, unsigned short & num, bool bDefaultData = false, unsigned char *ppBackupConfig = NULL, unsigned long backupLen = 0, unsigned long* pos = NULL);
	void LoadPTZPresetCfg( unsigned char **ppConfig, unsigned long &len, unsigned short &num);
	void LoadPTZCruiseCfg( unsigned char **ppConfig, unsigned long &len, unsigned short &num);
#ifdef __GB28181_SERVER__
	void LoadGB28181Cfg(unsigned char **ppConfig, unsigned long & len, unsigned short & num, bool bDefaultData = false, unsigned char *ppBackupConfig = NULL, unsigned long backupLen = 0, unsigned long* pos = NULL);
#endif

#ifdef __MOBILE_DVR__
	void LoadMobileCfg(unsigned char **ppConfig, unsigned long & len, unsigned short & num);
	void LoadSpeedCfg(unsigned char **ppConfig, unsigned long & len, unsigned short & num);
	void LoadHighSpeedCfg(unsigned char **ppConfig, unsigned long & len, unsigned short & num);
	void LoadLowSpeedCfg(unsigned char **ppConfig, unsigned long & len, unsigned short & num);
	void LoadWallopCfg(unsigned char **ppConfig, unsigned long & len, unsigned short & num);
	void LoadWirelessCfg(unsigned char **ppConfig, unsigned long & len, unsigned short & num);
#endif

	unsigned long getHolidayGroupNum(unsigned short itemID);
	bool bisMixDVR();//判断是不是混合型dvr
	//////////////////////////////////////////////////////////////////////////
	//variable
	//////////////////////////////////////////////////////////////////////////
	unsigned long	m_clientID;
	FILE			*m_pFile;

	unsigned char	m_videoInputNum;
	unsigned char	m_localVideoInputNum;
	unsigned char	m_audioInputNum;
	unsigned char	m_localSensorInputNum;
	unsigned char	m_sensorInputNum;
	unsigned char	m_sensorOutputNum;
	unsigned char	m_maxClientNum;
	unsigned char	m_netVideoInputNum;

	CONFIG_CFG_HEAD	m_cfgHead;
	CPresetConfig	m_presetConfig;
	CCruiseConfig	m_cruiseConfig;
	CScheduleConfig	m_scheduleHolidayCfg;
	//////////////////////////////////////////////////////////////////////////
	CMyList <NCFG_ITEM_HEAD_EX> m_itemList;

	CPUB_Lock		m_lock;
	//////////////////////////////////////////////////////////////////////////

	bool m_bCreatNewFile;
	bool m_bHaveCheckRate;
};
#endif //_CONFIG_EX_H_
