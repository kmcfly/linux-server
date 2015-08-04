/***********************************************************************
** Copyright (C)  Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : xzw
** Date         : 2010-11-2
** Name         : CfgRecordSnapDlg.h 
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/

#ifndef _CFG_RECORD_SNAP_DLG_H_
#define _CFG_RECORD_SNAP_DLG_H_

#include "Dialog.h"
#include "CfgDlg.h"
#include "ListCtrl.h"

class CCfgRecordSnapDlg : public CCfgDlg
{
public:
	CCfgRecordSnapDlg();
	virtual ~CCfgRecordSnapDlg();
	void SetShowSnapNum(bool bShow);

protected:
	void OnInitial();

protected:
	void DoOther();
	void EndOther();
	#ifdef __CUSTOM_US02_NEW__
	void SetTip();
	void TipAll(WND_LIST child, ENUM_STRING_ID strId);
	#endif

protected:
	bool ShowData(NCFG_ITEM_ID cfgID, const BYTE* pData, int len, bool bRefresh);
	bool SetData(NCFG_ITEM_ID cfgID, BYTE*& pData, int& len)  const;

protected:
	DECLARE_MSG_MAP()
	unsigned long OnClickList(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos );

protected:
	GUI::CListCtrl m_listCtrl;

	enum LIST_ROW
	{
		ROW_RESOLUTION,
		ROW_QUALITY,
		ROW_INTERVAL,
		ROW_MAXNUM,
	};
	bool m_bShowSnapNum;
};

#endif
