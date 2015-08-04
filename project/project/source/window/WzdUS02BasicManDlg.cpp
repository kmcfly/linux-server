/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : zxx
** Date         : 2012-09-18
** Name         : WzdUS02BasicManDlg.h
** Version      : 1.0
** Description  : 向导页面对话框
** Modify Record:
***********************************************************************/

#include "WzdUS02BasicManDlg.h"


CWzdUS02BasicManDlg::CWzdUS02BasicManDlg(): m_curPage(PAGE_TIME),m_pWzdInfo(NULL)
{
	
}

CWzdUS02BasicManDlg::~CWzdUS02BasicManDlg()
{

}

unsigned long CWzdUS02BasicManDlg::CheckValue()
{
	if(PAGE_DST == m_tabCtrl.GetFocusItem())
	{
		return m_wzdUS02DSTDlg.CheckValue();
	}
	return 0;
}
void CWzdUS02BasicManDlg::SetWzdInfo(US02_WzdInfo* pWzdInfo)
{
	m_pWzdInfo = pWzdInfo;
}

void CWzdUS02BasicManDlg::InitCtrlInfo()
{
	m_wzdUS02TimeDlg.InitCtrlInfo();
	m_wzdUS02DSTDlg.InitCtrlInfo();
	m_wzdUS02LanguageSetDlg.InitCtrlInfo();
}

void CWzdUS02BasicManDlg::ShowWzdData(bool bRefresh)
{
	m_wzdUS02TimeDlg.ShowWzdData(false);
	if(1 == m_pWzdInfo->ShowWzd)
	{
		m_checkCtrl.SetCheck(true,false);
	}
	else
	{
		m_checkCtrl.SetCheck(false,false);
	}
}

void CWzdUS02BasicManDlg::GetWzdData(bool bNext)
{
	m_wzdUS02TimeDlg.GetWzdData(bNext);
	m_wzdUS02DSTDlg.GetWzdData(bNext);
	m_wzdUS02LanguageSetDlg.GetWzdData(bNext);
}

void CWzdUS02BasicManDlg::OnLeavePage()
{
	m_wzdUS02TimeDlg.OnLeavePage();
}

bool CWzdUS02BasicManDlg::GetCurrZoneTimeInfo(ZONE_TIME_INFO& zoneTimeInfo, int zone)
{
	return m_wzdUS02TimeDlg.GetCurrZoneTimeInfo(zoneTimeInfo,zone);
}

void CWzdUS02BasicManDlg::OnInitial()
{
	CWzdUS02PageDlg::OnInitial();
	unsigned short x = m_length[DLG_OFFSET_LEFT]-10;
	unsigned short y = m_length[TITLE_HEIGHT] +m_length[DLG_OFFSET_TOP];
	unsigned short offset = m_length[DLG_OFFSET_BOTTOM];
	unsigned short cx = m_rect.Width() - 2 * x;
	unsigned short cy = m_rect.Height() - y- offset- 2*m_length[LIST_ITEM_HEIGHT];
	
	CWzdUS02DSTDlg sass;
	//先建立m_wzdUS02TimeDlg
	m_wzdUS02LanguageSetDlg.Create(GetRandWndID(),x,y,cx,cy,this,0,1);
	assert(m_pWzdInfo!=NULL);
	if(NULL != m_pWzdInfo)
	{
		m_wzdUS02LanguageSetDlg.SetWzdInfo(m_pWzdInfo);
	}
	m_wzdUS02TimeDlg.Create(GetRandWndID(), x, y, cx, cy, this, 0, 2);
	assert(m_pWzdInfo!=NULL);
	if(NULL != m_pWzdInfo)
	{
		m_wzdUS02TimeDlg.SetWzdInfo(m_pWzdInfo);
	}
	
	m_wzdUS02DSTDlg.Create(GetRandWndID(),x, y, cx, cy, this, 0, 3);
	if(NULL != m_pWzdInfo)
	{
		m_wzdUS02DSTDlg.SetWzdInfo(m_pWzdInfo);
	}
	m_tabCtrl.Create(GetRandWndID(), x, m_length[TITLE_HEIGHT]/2, cx, m_length[TABLE_HEIGHT], this, 0, 1);
	m_tabCtrl.AddItem(&m_wzdUS02LanguageSetDlg,m_pStrTable->String(E_STR_ID_LANGUAGE));
	m_tabCtrl.AddItem(&m_wzdUS02TimeDlg, m_pStrTable->String(E_STR_ID_DATE_AND_TIME) );
	m_tabCtrl.AddItem(&m_wzdUS02DSTDlg, m_pStrTable->String(E_STR_ID_DST));
	
	m_curPage = PAGE_LANGUAGE;
	m_tabCtrl.SetCurItem(m_curPage , false );

	x = m_length[DLG_OFFSET_SUB_LEFT];
	y = m_rect.Height() - 3 * m_length[DLG_OFFSET_SUB_TOP];

	cx = m_rect.Width() -  2 * x;
	cy = m_length[CHECK_HEIGHT];

	m_checkCtrl.Create(GetRandWndID(),x,y,cx,cy,this,0,1);
	m_checkCtrl.SetCaption(m_pStrTable->String(E_STR_ID_US02_WZD_OPENAGAIN),false,false);
	
}

void CWzdUS02BasicManDlg::OnDestroy()
{

}


BEGIN_MSG_MAP(CWzdUS02BasicManDlg, CWzdUS02PageDlg)
	ON_MSG(m_checkCtrl.GetID(), KEY_VIRTUAL_ENTER, OnCheck)
END_MSG_MAP()

unsigned long  CWzdUS02BasicManDlg::OnCheck()
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

