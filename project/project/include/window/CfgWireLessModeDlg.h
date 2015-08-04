/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : zxx
** Date         : 2011-07-15
** Name         : CfgWireLessModeDlg.h//局域网模式对话框
** Version      : 1.0
** Description  : 
** Modify Record:

***********************************************************************/

#ifndef _CFGWIRELESSMODEDLG_H_
#define _CFGWIRELESSMODEDLG_H_

#include "Dialog.h"
#include "ListCtrl.h"
#include "CfgDlg.h"
#include "dvrdvsdef.h"
class CCfgWireLessModeDlg : public GUI::CDialog
{
public:
	CCfgWireLessModeDlg();
	~CCfgWireLessModeDlg();
	void SetDataInfo(const SETUPDATAINFO& DataInfo) { m_dataInfo = DataInfo;}
	unsigned long CheckValue();
	//刷新数据到界面
	void RefreshView(bool bRefresh=true);

protected:
	virtual void OnInitial();
	void OnPaint(PAINT_STATUS status);
	void OnModifyNetModeData(unsigned long cfgId);

	DECLARE_MSG_MAP()
	unsigned long OnClickList(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);
	
	unsigned long GetErrorTextNum(NET_CTRL_FAIL result);

protected:
	GUI::CListCtrl		m_cfgNetWirelessList;
private:
	SETUPDATAINFO m_dataInfo;

	enum LIST_ROW
	{
		ROW_WIRELESS_ENABLE = 0,
		ROW_WIRELESS_TYPE  = 1,
		ROW_WIRELESS_CONNENT = 2,
		ROW_WIRELESS_APN = 3,
		ROW_WIRELESS_USER = 4, 
		ROW_WIRELESS_PWD = 5,
	};
};

#endif
