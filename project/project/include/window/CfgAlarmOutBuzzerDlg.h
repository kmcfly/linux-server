/***********************************************************************
** Copyright (C)  Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : L
** Date         : 2010-04-09
** Name         : CfgAlarmOutBuzzerDlg.h 
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/

#ifndef _CFG_ALARM_OUT_BUZZER_DLG_H_
#define _CFG_ALARM_OUT_BUZZER_DLG_H_

#include "Dialog.h"
#include "CfgDlg.h"
#include "ListCtrl.h"

class CCfgAlarmOutBuzzerDlg : public CCfgDlg
{
public:
	CCfgAlarmOutBuzzerDlg();
	virtual ~CCfgAlarmOutBuzzerDlg();


protected:
	void OnInitial();

protected:
	bool ShowData(NCFG_ITEM_ID cfgID, const BYTE* pData, int len, bool bRefresh);
	bool SetData(NCFG_ITEM_ID cfgID, BYTE*& pData, int& len)  const;

protected:
	DECLARE_MSG_MAP()
	unsigned long OnClickList(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos );
	void GetItemList(CFGITEM_LIST& itemList) const;

protected:

	GUI::CListCtrl m_listCtrl;

	enum LIST_ROW
	{
		ROW_BUZZER_ENABLE,
		ROW_BUZZER_HOLD_TIME,
	};

	static unsigned long s_holdTime[]; 
};

#endif
