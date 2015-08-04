/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : zhl   
** Date         : 2010-09-01
** Name         : HxGetSetConfig.h
** Version      : 1.0
** Description  :    
**					hxht�����豸��ȡ����������
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
	
	//����DVR���
	bool SetCtrlDVRInfo(unsigned long ctrlNum, unsigned char *pData, unsigned long dataLen, bool bNeedDvrReplay = true);
	bool NeedCtrlDVR();
	bool CtrlDvr(unsigned long clientId, unsigned long deviceID);

	//��ȡ����
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
	

	//��������
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

	std::list<unsigned int> m_getCfgItemIdList;				//��Ŷ�ȡ���õ�item ID


	bool	m_bNeedGetConfig;
	HX_CONFIG_STATUS m_getConfigStatus;
	std::list<HX_CONFIG_INFO> m_getCfgItemInfoList;			//��Ŷ����õ�item head�� item head ��Ҫ�����ݳ��ȣ��øó������������õĿռ�
	
	bool				m_bNeedSetConfig;	
	HX_CONFIG_STATUS m_setConfigStatus;
	std::list<HX_CONFIG_INFO> m_setCfgItemInfoList;			//���д���õ�item head�� item head ��Ҫ�����ݳ��ȣ��øó������������õĿռ�

	CConfigBlock		m_cfgBlock;

	unsigned long		m_ctrlNum;
	unsigned char		*m_pCtrlData;
	unsigned long		m_ctrlDataLen;
	bool				m_bNeedDvrReplay;
	CONFIG_TEMP_INFO	m_configTempInfo;
	/**************************************zxx����*******************************************************************/
public:
	void SetNeedDefaultCfg(bool bNeed){m_bNeedDefaultConfig = bNeed;}
	void GetFrameParam(HX_NET_INIT_INFO hxNetInfo);//����޸�֡����Ҫ�Ĳ���
private:
	bool m_bNeedDefaultConfig;
	unsigned char m_maxVideo;
private:
	void GetData(unsigned char *tmpData,int dataLen,char *pData,int &realLen);//��ԭ�ȵ����ÿ��ȡ���ã�ȥ��ֻ����ѡ��NCFG_ITEM_ENCODE_MAJOR_SUPPORT_PROPERTY,realLen����ͳ��ʵ�ʵ����ݿ鳤��

public:
	unsigned long m_videoParamType;
};


#endif  /*_HX_GET_SET_CONFIG_*/
