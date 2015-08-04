/***********************************************************************
** Copyright (C)  Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : L
** Date         : 2010-04-10
** Name         : CfgRecordRecycleDlg.h 
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/

#ifndef _CFG_RECORD_RECYCLE_DLG_H_
#define _CFG_RECORD_RECYCLE_DLG_H_

#include "Dialog.h"
#include "CfgDlg.h"
#include "ListCtrl.h"

class CCfgRecordRecycleDlg : public CCfgDlg
{
public:
	CCfgRecordRecycleDlg();
	virtual ~CCfgRecordRecycleDlg();

	virtual unsigned long PreCheckValue();

protected:
	void OnInitial();

protected:
	bool ShowData(NCFG_ITEM_ID cfgID, const BYTE* pData, int len, bool bRefresh);
	bool SetData(NCFG_ITEM_ID cfgID, BYTE*& pData, int& len)  const;

protected:
	DECLARE_MSG_MAP()
	unsigned long OnClickList(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);

protected:
	GUI::CListCtrl m_listCtrl;

	enum LIST_ROW
	{
		ROW_RECORD_RECYCLE,
	};
};

#endif
