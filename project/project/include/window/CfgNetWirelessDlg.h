/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : lufen
** Date         : 2010-04-7
** Name         : CfgNetWirelessDlg.h
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/

#ifndef _CFG_NET_WIRELESS_DLG_H_
#define _CFG_NET_WIRELESS_DLG_H_

#include "ListCtrl.h"
#include "CfgDlg.h"

using namespace GUI;

class CCfgNetWirelessDlg : public CCfgDlg
{
public:
	CCfgNetWirelessDlg();
	~CCfgNetWirelessDlg();

protected:
	void OnInitial();

protected: 
	bool ShowData(NCFG_ITEM_ID cfgID, const BYTE* pData, int len, bool bRefresh);
	bool SetData(NCFG_ITEM_ID cfgID, BYTE*& pData, int& len)  const;


	DECLARE_MSG_MAP()
	unsigned long OnClickList(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);

protected:
	GUI::CListCtrl	m_cfgNetWirelessList;

	enum LIST_ROW
	{
		ROW_WIRELESS_ENABLE = 0,
		ROW_WIRELESS_TYPE  = 1,
	};
};




#endif

