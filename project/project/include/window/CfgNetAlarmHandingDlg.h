/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : L
** Date         : 2009-10-14
** Name         : CfgNetAlarmHandingDlg.h 
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/
#ifndef _CFG_NET_ALARM_HANDING_DLG_H_
#define _CFG_NET_ALARM_HANDING_DLG_H_

#include "CfgDlg.h"
#include "ListCtrl.h"

#include "CfgTriggerDlg.h"

using namespace GUI;

class CCfgNetAlarmHandingDlg : public CCfgDlg
{
public:
	CCfgNetAlarmHandingDlg();
	~CCfgNetAlarmHandingDlg();

protected:
	DECLARE_MSG_MAP()
	void OnInitial();
	void OnDestroy();

	void UpdateView(bool bRefresh);

	unsigned long OnClickList(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);
	unsigned long OnClickListAll(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);

	enum COL_INDEX
	{
		COL_CHANNEL,
		COL_HOLD_TIME,
		COL_TRIGGER,
	};

	//void CreateTriggerInfo();
	//void DestroyTriggerInfo();
	//ÖØÐ´
	//void GetItemList(CFGITEM_LIST& itemList) const;
	bool ShowData(NCFG_ITEM_ID cfgID, const BYTE* pData, int len, bool bRefresh);
	bool SetData(NCFG_ITEM_ID cfgID, BYTE*& pData, int& len)  const;


private:
	GUI::CListCtrl m_listCtrl;
	GUI::CListCtrl m_listAll;
	GUI::CStatic   m_scAll;

	ALARM_TRIGGER_INFO* m_pTriggerInfo;
	ALARM_TRIGGER_INFO* m_pAllTriggerInfo;
};

#endif//_CFG_SENSOR_ADVANCE_DLG_H_

