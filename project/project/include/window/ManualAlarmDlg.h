/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : L
** Date         : 2009-08-19
** Name         : ManualAlarmDlg.h
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/

#ifndef _MANUAL_ALARM_DLG_H_
#define _MANUAL_ALARM_DLG_H_

#include "Dialog.h"
#include "ListCtrl.h"
#include "CheckCtrl.h"
#include "MessageMan.h"
#include "CfgDlg.h"

using namespace GUI;

class CManualAlarmDlg : public CCfgDlg
{
public:
	CManualAlarmDlg();
	~CManualAlarmDlg();

	void UpdateView(bool bRefresh);

protected:
	void OnInitial();

	unsigned long OnApply();
	unsigned long OnOK();
	unsigned long OnCancel();
	unsigned long OnClickAll();

protected:
	void GetItemList(CFGITEM_LIST& itemList) const;
	bool ShowData(NCFG_ITEM_ID cfgID, const BYTE* pData, int len, bool bRefresh);

protected:
	unsigned long CheckUI(bool bCk, bool bRefresh);

	DECLARE_MSG_MAP()
	unsigned long OnClickList(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);

	GUI::CListCtrl			m_manualAlarmList;
	GUI::CCheckCtrl			m_ckAll;

	enum LIST_COL
	{
		COL_INDEX = 0,
		COL_ALARM_NAME,
#if !defined(__TW01_RILI__)
		COL_IP_ADDRESS,
#endif
		COL_TRIGGER
	};
private:
	CMessageMan			*m_pMsgMan;
};

#endif
