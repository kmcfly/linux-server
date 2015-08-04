/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : l
** Date         : 2009-08-12
** Name         : CfgRecordDlg.h
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/

#ifndef _CFG_RECORD_DLG_H_
#define _CFG_RECORD_DLG_H_

#include "Dialog.h"
#include "ListCtrl.h"
#include "CfgDlg.h"

using namespace GUI;

class CCfgRecordDlg : public CCfgDlg
{
public:
	CCfgRecordDlg();
	~CCfgRecordDlg();

protected:
	void OnInitial();
	void OnDestroy();

	void SetTip();
	void DoOther();

public:
	void UpdateView(bool bRefresh);

protected:
	bool ShowData(NCFG_ITEM_ID cfgID, const BYTE* pData, int len, bool bRefresh);
	bool SetData(NCFG_ITEM_ID cfgID, BYTE*& pData, int& len)  const;

protected:
	DECLARE_MSG_MAP()
	unsigned long OnClickList(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);
	unsigned long OnClickListAll(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);

protected:
	GUI::CListCtrl m_cfgRecordList;
	GUI::CListCtrl m_cfgRecordListAll;

	GUI::CStatic   m_cfgSCAll;

	std::map<int, int>  m_rowChnnMap;

	enum LIST_COL
	{
		COL_CHNN,
		COL_REC_ENABLE,
		COL_REC_AUDIO,
		COL_REDUNDANT,
	};
};

#endif
