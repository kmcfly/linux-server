/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : hz
** Date         : 2009-08-01
** Name         : CfgVideoLossDlg.h 
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/
#ifndef _CFG_VIDEOLOSS_DLG_H_
#define _CFG_VIDEOLOSS_DLG_H_

#include "CfgDlg.h"
#include "ListCtrl.h"
#include "CfgTriggerDlg.h"

#include <map>

using namespace GUI;

class CCfgVideoLossDlg : public CCfgDlg
{
public:
	CCfgVideoLossDlg();
	~CCfgVideoLossDlg();

protected:
	DECLARE_MSG_MAP()
	void OnInitial();
	void OnDestroy();

	unsigned long OnApply();

	unsigned long OnClickList(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);
	unsigned long OnClickListAll(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);

	enum COL_INDEX
	{
		COL_CHANNEL,
		COL_TRIGGER,
	};
	//ÖØÐ´
	void UpdateView(bool bRefresh);
	bool ShowData(NCFG_ITEM_ID cfgID, const BYTE* pData, int len, bool bRefresh);
	bool SetData(NCFG_ITEM_ID cfgID, BYTE*& pData, int& len)  const;
	void GetItemList(CFGITEM_LIST& itemList) const;

private:
	GUI::CListCtrl m_listCtrl;
	GUI::CListCtrl m_listAll;
	GUI::CStatic   m_scAll;

	ALARM_TRIGGER_INFO* m_pTriggerInfo;
	ALARM_TRIGGER_INFO* m_pAllTriggerInfo;

	std::map<int, int>  m_rowChnnMap;
};

#endif//_CFG_VIDEOLOSS_DLG_H_

