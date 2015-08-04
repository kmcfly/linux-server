/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : ZYX
** Date         : 2011-05-18
** Name         : CfgNetAutoReportDlg.h
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/

#ifndef _CFG_NETAUTOREPORT_DLG_H_
#define _CFG_NETAUTOREPORT_DLG_H_

#include "ListCtrl.h"
#include "CfgDlg.h"
#include "CharCodeTable.h"
using namespace GUI;

class CCfgNetAutoReportDlg : public CCfgDlg
{
public:
	CCfgNetAutoReportDlg();
	~CCfgNetAutoReportDlg();

protected:
	void OnInitial();

protected: 
	bool ShowData(NCFG_ITEM_ID cfgID, const BYTE* pData, int len, bool bRefresh);
	bool SetData(NCFG_ITEM_ID cfgID, BYTE*& pData, int& len)  const;


	DECLARE_MSG_MAP()
	unsigned long OnClickList(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);

protected:
	GUI::CListCtrl	m_cfgNetAutoReportList;

	enum LIST_ROW
	{
		ROW_USE         = 0,
		ROW_SERVER_HOST = 1,
		ROW_SERVER_PORT = 2,
		ROW_DEV_ID      = 3,
	};
};




#endif
