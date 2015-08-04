/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : zhl
** Date         : 2010-10-08
** Name         : CfgHxhtNetParamCfgDlg.h
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/

#ifndef _CFG_HXHT_NET_PARAM_DLG_
#define _CFG_HXHT_NET_PARAM_DLG_

#include "Dialog.h"
#include "ListCtrl.h"
#include "CfgDlg.h"
using namespace GUI;


class CCfgHxhtNetParamCfgDlg : public CCfgDlg
{
public:
	CCfgHxhtNetParamCfgDlg();
	~CCfgHxhtNetParamCfgDlg();

protected:
	virtual void OnInitial();
	//unsigned long OnOK();
	unsigned long OnApply();

	void GetItemList(CFGITEM_LIST& itemList) const;
	bool ShowData(NCFG_ITEM_ID cfgID, const BYTE* pData, int len, bool bRefresh);
	bool SetData(NCFG_ITEM_ID cfgID, BYTE*& pData, int& len)  const;

	DECLARE_MSG_MAP()
	unsigned long OnClickList(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);
protected:
	enum LIST_ROW
	{
		ROW_VIDEO_SERVER_DEVICE_ID = 0,
		ROW_VIDEO_SERVER_DATA_PORT = 1,

		ROW_CONNECT_SERVER_IP = 3,
		ROW_CONNECT_SERVER_MESSAGE_PORT = 4,
		ROW_CONNECT_SERVER_STORT_PORT = 5,

		ROW_NUM = 6,
	};

	GUI::CListCtrl			m_cfgHxhtNetParamList;
};


#endif
