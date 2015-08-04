/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : L
** Date         : 2010-03-25
** Name         : CfgCoverDlg.h
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/

#ifndef _CFG_COVER_DLG_H_
#define _CFG_COVER_DLG_H_

#include "Wnd.h"
#include "Dialog.h"
#include "ListCtrl.h"
#include "Button.h"

#include "CfgDlg.h"
#include "SetCoverDlg.h"

#include <map>

using namespace GUI;

class CCfgCoverDlg : public CCfgDlg
{
public:
	CCfgCoverDlg();
	~CCfgCoverDlg();

protected:
	void OnInitial();
	void OnDestroy();

	void SetTip();

protected:
	bool ShowData(NCFG_ITEM_ID cfgID, const BYTE* pData, int len, bool bRefresh);
	bool SetData(NCFG_ITEM_ID cfgID, BYTE*& pData, int& len)  const;
	
protected:
	DLG_END_VALUE PopupSetCoverDlg(int chnn, CAMERA_COVER_INFO& coverInfo);

protected:
	DECLARE_MSG_MAP()
	unsigned long OnClickList(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);

protected:
	GUI::CListCtrl		m_listCtrl;

	CAMERA_COVER_INFO*	m_pCamCoverInfo;
	int					m_buffLen;

	CSetCoverDlg		m_setCoverDlg;

	std::map<int, int>  m_rowChnnMap;

	enum LIST_COL
	{
		COL_CHNN,
		COL_CAMERA_COVER,
	};
};

#endif
