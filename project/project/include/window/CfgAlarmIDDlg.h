/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : L
** Date         : 2009-08-14
** Name         : CfgNetworkDlg.h
** Version      : 1.0
** Description  : 
** Modify Record:
1:2010-06-10,把该页面中的DDNS配置分割出来，放在CfgNetMoreConfigDlg.cpp页面中 ZHL
***********************************************************************/

#ifndef _CFG_ALARMID_DLG_H_
#define _CFG_ALARMID_DLG_H_

#include "Dialog.h"
#include "ListCtrl.h"
#include "CfgDlg.h"
#include "CheckCtrl.h"

#include <map>

using namespace GUI;

class CCfgAlarmIdDlg : public CCfgDlg
{
public:
	CCfgAlarmIdDlg();
	~CCfgAlarmIdDlg();

protected:
	void OnInitial();
	void OnDestroy();

	void SetTip();

protected:
	bool ShowData(NCFG_ITEM_ID cfgID, const BYTE* pData, int len, bool bRefresh);
	bool SetData(NCFG_ITEM_ID cfgID, BYTE*& pData, int& len)  const;

	unsigned long CheckValue();
	unsigned long PreCheckValue();
	bool IsValidID(const string &strID);

protected:
	DECLARE_MSG_MAP()
	unsigned long OnClickList(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);
	unsigned long  OnCheck();

protected:// UI
	GUI::CListCtrl		m_cfgLiveList;

	std::map<int, int>  m_rowChnnMap;

	enum LIST_COL
	{
		COL_CHNN,
		COL_CAMERA_ID,
	};

protected:
};

#endif
