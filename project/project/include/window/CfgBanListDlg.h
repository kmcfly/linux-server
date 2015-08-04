/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : zxx
** Date         : 2010-07-28
** Name         : CCfgBanListDlg.h
** Version      : 1.0
** Description  : 向导对话框
** Modify Record:
***********************************************************************/
#ifndef _CCFGBANLISTDLG_H_
#define _CCFGBANLISTDLG_H_
#include "Dialog.h"
#include "RadioCtrl.h"
#include "Static.h"
#include "Button.h"
#include "IPAddressCtrl.h"
#include "CheckCtrl.h"
#include "ListCtrl.h"
#include "CfgDlg.h"
using namespace GUI;


class CCfgBanListDlg:public CCfgDlg
{
public:
	CCfgBanListDlg();
	virtual ~CCfgBanListDlg();
protected:
	void OnInitial();
	void GetItemList(CFGITEM_LIST& itemList) const;
	bool ShowData(NCFG_ITEM_ID cfgID, const BYTE* pData, int len, bool bRefresh);
	bool SetData(NCFG_ITEM_ID cfgID, BYTE*& pData, int& len) const;
	unsigned long OnOK();
	unsigned long OnCancel();
	unsigned long OnApply();

protected:
	GUI::CStatic		m_scInfo;
	GUI::CRadioGroup	m_rdGroup;
	GUI::CRadioCtrl		m_rdBlack;
	GUI::CRadioCtrl		m_rdWhite;
	GUI::CListCtrl		m_ipList;
	GUI::CButton	    m_btnadd;
	GUI::CButton        m_btndel;
protected:
	DECLARE_MSG_MAP()
	unsigned long OnClickRadio();
	unsigned long OnClickList(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);
private:
	enum LIST_COL
	{
		ROW_STARTUP,
		ROW_CHNN,
		ROW_STARTIP,
		ROW_ENDIP,
	};
	enum PAGE_ITEM
	{
		PAGE_NULL	= -1,
		PAGE_BLACK,
		PAGE_WHITE,
	};
	PAGE_ITEM	m_curPage;
private:
	void SetPage(PAGE_ITEM page,bool bRefresh);
	
};
#endif
