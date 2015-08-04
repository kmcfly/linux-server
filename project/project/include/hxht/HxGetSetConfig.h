/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : zhl   
** Date         : 2010-09-01
** Name         : HxGetSetConfig.h
** Version      : 1.0
** Description  :    
**					hxht网络设备获取和设置配置
***********************************************************************/

#ifndef _HX_GET_SET_CONFIG_
#define _HX_GET_SET_CONFIG_

#include <list>
#include "PfHxDefine.h"
#include "HxGlobal.h"
#include "NetConfigDefine.h"
#include "ConfigBlock.h"
#include "dvrdvsdef.h"
#include "FrameRate.h"
typedef enum _hx_config_status_
{
	HX_CONFIG_STATUS_NONE	= 0,
	HX_CONFIG_STATUS_ENTER	= 1,
	HX_CONFIG_STATUS_READ	= 2,
	HX_CONFIG_STATUS_WRITE	= 3,
	HX_CONFIG_STATUS_LEAVE	= 4,
}HX_CONFIG_STATUS;

typedef struct _cofig_temp_info_
{
	NCFG_ITEM_ID ncfgItemID;
	char *pData;
	unsigned long dataLen;
	ULONGLONG lastReadTime;
}CONFIG_TEMP_INFO;

class CHxGetSetConfig
{
public:
	CHxGetSetConfig();
	~CHxGetSetConfig();
	
	//操作DVR相关
	bool SetCtrlDVRInfo(unsigned long ctrlNum, unsigned char *pData, unsigned long dataLen, bool bNeedDvrReplay = true);
	bool NeedCtrlDVR();
	bool CtrlDvr(unsigned long clientId, unsigned long deviceID);

	//获取配置
	bool AddGetConfigInfo(HX_CONFIG_INFO hxConfigInfo);
	void ReleaseHxGetCfgInfo();
	inline void SetNeedReadCfg(HX_CONFIG_STATUS cfgStatus){m_getConfigStatus = cfgStatus;}
	inline bool NeedReadConfig(){return ((HX_CONFIG_STATUS_READ == m_getConfigStatus) ? true : false);}
	inline bool IsGettingConfig(){return ((HX_CONFIG_STATUS_READ == m_getConfigStatus) ? true : false);}
	
	std::list<HX_CONFIG_INFO> &GetCfgItemInfoList();
	bool GetConfig(unsigned long clientId, unsigned long deviceID);
	bool GetCfgDataFromDVR(char *pData, unsigned long dataLen);
	unsigned char *GetItemData(unsigned long cfgItemID, unsigned long &dataLen);
	bool GetCfgOver();
	bool IsGetCfgOK();
	

	//设置配置
	bool AddHxSetConfigInfo(HX_CONFIG_INFO hxConfigInfo);
	void ReleaseHxSetCfgInfo();
	inline void SetNeedSetCfg(bool bNeed){m_bNeedSetConfig = bNeed;}
	inline bool NeedSetConfig(){return m_bNeedSetConfig;}
	
	bool EnterConfig(unsigned long clientId, unsigned long deviceID);
	void LeaveCfg(unsigned long clientId, unsigned long deviceID);
	bool SetConfig(unsigned long clientId, unsigned long deviceID, char *pData, unsigned long dataLen);
	bool SetCfgOver();
	bool IsSetCfgOK();
	inline bool IsSettingConfig(){return ((HX_CONFIG_STATUS_NONE != m_setConfigStatus) ? true : false);}
	

protected:
	
private:
	bool AddItem(unsigned int cfgItemId);

	std::list<unsigned int> m_getCfgItemIdList;				//存放读取配置的item ID


	bool	m_bNeedGetConfig;
	HX_CONFIG_STATUS m_getConfigStatus;
	std::list<HX_CONFIG_INFO> m_getCfgItemInfoList;			//存放读配置的item head， item head 中要有数据长度，用该长度来申请配置的空间
	
	bool				m_bNeedSetConfig;	
	HX_CONFIG_STATUS m_setConfigStatus;
	std::list<HX_CONFIG_INFO> m_setCfgItemInfoList;			//存放写配置的item head， item head 中要有数据长度，用该长度来申请配置的空间

	CConfigBlock		m_cfgBlock;

	unsigned long		m_ctrlNum;
	unsigned char		*m_pCtrlData;
	unsigned long		m_ctrlDataLen;
	bool				m_bNeedDvrReplay;
	CONFIG_TEMP_INFO	m_configTempInfo;
	/**************************************zxx增加*******************************************************************/
public:
	void SetNeedDefaultCfg(bool bNeed){m_bNeedDefaultConfig = bNeed;}
	void GetFrameParam(HX_NET_INIT_INFO hxNetInfo);//获得修改帧率需要的参数
private:
	bool m_bNeedDefaultConfig;
	unsigned char m_maxVideo;
private:
	void GetData(unsigned char *tmpData,int dataLen,char *pData,int &realLen);//从原先的配置块获取配置，去除只读的选项NCFG_ITEM_ENCODE_MAJOR_SUPPORT_PROPERTY,realLen用来统计实际的数据块长度

public:
	unsigned long m_videoParamType;
};


#endif  /*_HX_GET_SET_CONFIG_*/
