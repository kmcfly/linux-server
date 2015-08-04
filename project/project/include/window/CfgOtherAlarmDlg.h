/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : hz
** Date         : 2009-08-01
** Name         : CfgOtherAlarmDlg.h 
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/
#ifndef _CFG_OTHER_ALARM_DLG_H_
#define _CFG_OTHER_ALARM_DLG_H_

#include "Dialog.h"
#include "ListCtrl.h"
#include "CfgDlg.h"
#include "OtherAlarmDlg.h"
#include "WndStructDef.h"

using namespace GUI;

class CCfgOtherAlarmDlg : public CCfgDlg
{
public:
	CCfgOtherAlarmDlg();
	~CCfgOtherAlarmDlg();

	void UpdateView(bool bRefresh);

protected:
	void OnInitial();
	void OnDestroy();

protected:
	void GetItemList(CFGITEM_LIST& itemList) const;
	bool ShowData(NCFG_ITEM_ID cfgID, const BYTE* pData, int len, bool bRefresh);
	bool SetData(NCFG_ITEM_ID cfgID, BYTE*& pData, int& len)  const;

	unsigned long OnApply();

protected:
	DECLARE_MSG_MAP()
	unsigned long OnClickScrollDlg(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);
protected:
	COtherAlarmDlg  m_otherAlarm;

	OTHER_ALARM_INFO* m_pOtherAlarmInfo;
};

#endif//_CFG_OTHER_ALARM_DLG_H_

