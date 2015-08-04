/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : L
** Date         : 2011-04-12
** Name         : TSCfgToIpc.h
** Version      : 1.0
** Description  : 向IPC配置转换
** Modify Record:
1:创建
***********************************************************************/

#ifndef _TSCFG_TO_IPC_H_
#define _TSCFG_TO_IPC_H_

#include "NetDevicePlatform.h"
#include "DDPublic.h"
#include "ipc_v3/platform_ipc_v3.h"


class CTSNetDevice;
class CTSNdParam;

class CTSCfgToIpc
{
public:
	CTSCfgToIpc();
	virtual ~CTSCfgToIpc();

	bool Initial(CTSNetDevice* pNetDevice, CTSNdParam* pNdParam);
	void Quit();

	long SetConfig(unsigned long itemID, const unsigned char* pData, unsigned long dataLen);
	unsigned long FlushConfig();

	//清楚配置列
	void ClearCfgList();

	//获取需要发送的配置数据
	bool GetBuff(unsigned char*& pBuff, unsigned long& dataLen);

	int GetVideoStreamIndex(){return 0;/*return m_videoStreamIndex;*/}
	void SetVideoStreamIndex(int streamIndex){m_videoStreamIndex = streamIndex;}

	int GetSubStreamIndex(){return 1;}

	int GetIpcResolution(){return m_ipcCurresolution;}

protected:
	void ClearIpcList();
	unsigned long PackCmd();
	long ConvertConfig();

protected:
	typedef struct _ts_cfg_item
	{
		unsigned long	itemID;
		unsigned long	dataLen;
		unsigned char*	pData;
	}TS_CFG_ITEM;

	typedef struct _ts_cfg_ipc_v3
	{
		IPC_V3::NCFG_ITEM_HEAD  itemHead;
		unsigned char*			pData;
	}TS_CFG_IPC_V3;

	typedef std::list<TS_CFG_ITEM>	 TS_CFG_ITEM_LIST;
	typedef std::list<TS_CFG_IPC_V3> TS_CFG_LIST_IPC_V2;

	TS_CFG_ITEM_LIST			m_cfgItemList;			//临时存储需要保存的配置项
	TS_CFG_LIST_IPC_V2			m_ipcItemList_v2;		//向IPC需要保存的配置项

	unsigned char*				m_pIpcBuff;				//
	unsigned long				m_ipcDataLen;			//
	unsigned long				m_ipcBuffLen;			//

	CTSNetDevice*				m_pNetDevice;			//
	CTSNdParam*					m_pNdParam;

	int							m_videoStreamIndex;

	long						m_timeZone[DD_TIME_ZONE_NUM];

	int                         m_ipcCurresolution;
};

#endif

//end
