/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : ZXX
** Date         : 2013-11-25
** Name         : WzdNatPageDlg.h
** Version      : 1.0
** Description  : 穿透向导继承对话框
** Modify Record:
***********************************************************************/

#include "WzdNatPageDlg.h"

CWzdNatPageDlg::CWzdNatPageDlg() : m_pWzdInfo(NULL),
m_bUsed(true)
{

}

CWzdNatPageDlg::~CWzdNatPageDlg()
{

}

void CWzdNatPageDlg::SetUsed(bool bUsed)
{
	m_bUsed = bUsed;
}

bool CWzdNatPageDlg::IsUsed()
{
	return m_bUsed;
}


void CWzdNatPageDlg::SetWzdInfo(NAT_WIZARD_INFO* pWzdInfo)
{
	m_pWzdInfo = pWzdInfo;
}

void CWzdNatPageDlg::InitCtrlInfo()
{
}
void CWzdNatPageDlg::ShowWzdData(bool bRefresh)
{

}

void CWzdNatPageDlg::GetWzdData(bool bNext)
{

}

void CWzdNatPageDlg::OnLeavePage()
{

}

unsigned long CWzdNatPageDlg::CheckValue()
{
	return 0;
}

//end

