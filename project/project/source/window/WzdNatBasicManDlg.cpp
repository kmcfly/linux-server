/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : zxx
** Date         : 2013-11-26
** Name         : WzdNatBasicManDlg.cpp
** Version      : 1.0
** Description  : 向导页面对话框
** Modify Record:
***********************************************************************/

#include "WzdNatBasicManDlg.h"


CWzdNatBasicManDlg::CWzdNatBasicManDlg(): m_curPage(PAGE_TIME),m_pWzdInfo(NULL)
{

}

CWzdNatBasicManDlg::~CWzdNatBasicManDlg()
{

}

unsigned long CWzdNatBasicManDlg::CheckValue()
{
	if(PAGE_DST == m_tabCtrl.GetFocusItem())
	{
		return m_wzdNatDSTDlg.CheckValue();
	}
	return 0;
}

void CWzdNatBasicManDlg::SetWzdInfo(NAT_WIZARD_INFO* pWzdInfo)
{
	m_pWzdInfo = pWzdInfo;
}

void CWzdNatBasicManDlg::InitCtrlInfo()
{
	m_wzdNatTimeDlg.InitCtrlInfo();
	m_wzdNatDSTDlg.InitCtrlInfo();
	m_wzdNatLanguageSetDlg.InitCtrlInfo();
}

void CWzdNatBasicManDlg::ShowWzdData(bool bRefresh)
{
	m_wzdNatTimeDlg.ShowWzdData(false);
	if(1 == m_pWzdInfo->ShowWzd)
	{
		m_checkCtrl.SetCheck(true,false);
	}
	else
	{
		m_checkCtrl.SetCheck(false,false);
	}
}

void CWzdNatBasicManDlg::GetWzdData(bool bNext)
{
	m_wzdNatTimeDlg.GetWzdData(bNext);
	m_wzdNatDSTDlg.GetWzdData(bNext);
	m_wzdNatLanguageSetDlg.GetWzdData(bNext);
}

void CWzdNatBasicManDlg::OnLeavePage()
{
	m_wzdNatTimeDlg.OnLeavePage();
}

bool CWzdNatBasicManDlg::GetCurrZoneTimeInfo(ZONE_TIME_INFO& zoneTimeInfo, int zone)
{
	return m_wzdNatTimeDlg.GetCurrZoneTimeInfo(zoneTimeInfo,zone);
}

void CWzdNatBasicManDlg::OnInitial()
{
	CWzdNatPageDlg::OnInitial();
	unsigned short x = m_length[DLG_OFFSET_LEFT]-10;
	unsigned short y = m_length[TITLE_HEIGHT] +m_length[DLG_OFFSET_TOP];
	unsigned short offset = m_length[DLG_OFFSET_BOTTOM];
	unsigned short cx = m_rect.Width() - 2 * x;
	unsigned short cy = m_rect.Height() - y- offset- 2*m_length[LIST_ITEM_HEIGHT];
	unsigned long  height =  m_rect.Height() - 2 * m_length[DLG_OFFSET_SUB_TOP] - m_length[CHECK_HEIGHT];

	CWzdNatDSTDlg sass;
	//先建立m_wzdNatTimeDlg
	m_wzdNatLanguageSetDlg.Create(GetRandWndID(),x,y,cx,cy,this,0,1);
	assert(m_pWzdInfo!=NULL);
	if(NULL != m_pWzdInfo)
	{
		m_wzdNatLanguageSetDlg.SetWzdInfo(m_pWzdInfo);
	}
	m_wzdNatTimeDlg.Create(GetRandWndID(), x, y, cx, cy, this, 0, 2);
	assert(m_pWzdInfo!=NULL);
	if(NULL != m_pWzdInfo)
	{
		m_wzdNatTimeDlg.SetWzdInfo(m_pWzdInfo);
	}

	m_wzdNatDSTDlg.Create(GetRandWndID(),x, y, cx, cy, this, 0, 3);
	if(NULL != m_pWzdInfo)
	{
		m_wzdNatDSTDlg.SetWzdInfo(m_pWzdInfo);
	}
	m_tabCtrl.Create(GetRandWndID(), x, m_length[TITLE_HEIGHT]/2, cx, m_length[TABLE_HEIGHT], this, 0, 4);
	m_tabCtrl.AddItem(&m_wzdNatLanguageSetDlg,m_pStrTable->String(E_STR_ID_LANGUAGE));
	m_tabCtrl.AddItem(&m_wzdNatTimeDlg, m_pStrTable->String(E_STR_ID_DATE_AND_TIME) );
	m_tabCtrl.AddItem(&m_wzdNatDSTDlg, m_pStrTable->String(E_STR_ID_DST));

	m_curPage = PAGE_LANGUAGE;
	m_tabCtrl.SetCurItem(m_curPage , false );

	x = m_length[DLG_OFFSET_SUB_LEFT];
	y = m_length[DLG_OFFSET_SUB_TOP]+height;
	cx = m_rect.Width() -  2 * x;
	cy = m_length[CHECK_HEIGHT];
	m_checkCtrl.Create(GetRandWndID(),x,y,cx,cy,this,0,5);
	m_checkCtrl.SetCaption(m_pStrTable->String(E_STR_ID_US02_WZD_OPENAGAIN),false,false);
}

void CWzdNatBasicManDlg::OnDestroy()
{

}


BEGIN_MSG_MAP(CWzdNatBasicManDlg, CWzdNatPageDlg)
	ON_MSG(m_checkCtrl.GetID(), KEY_VIRTUAL_ENTER, OnCheck)
END_MSG_MAP()

unsigned long  CWzdNatBasicManDlg::OnCheck()
{
	if(m_checkCtrl.IsCheck())
	{
		m_pWzdInfo->ShowWzd = 1;
	}
	else
	{
		m_pWzdInfo->ShowWzd = 0;
	}
	return KEY_VIRTUAL_STOP;
}

//end
