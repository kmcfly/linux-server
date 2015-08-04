/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : L
** Date         : 2010-02-26
** Name         : WzdPageDlg.cpp
** Version      : 1.0
** Description  : 向导对话框
** Modify Record:
***********************************************************************/

#include "WzdPageDlg.h"


CWzdPageDlg::CWzdPageDlg() : m_pWzdInfo(NULL),
m_bUsed(true)
{
	
}

CWzdPageDlg::~CWzdPageDlg()
{

}

void CWzdPageDlg::SetUsed(bool bUsed)
{
	m_bUsed = bUsed;
}

bool CWzdPageDlg::IsUsed()
{
	return m_bUsed;
}

void CWzdPageDlg::SetWzdInfo(WIZARD_INFO* pWzdInfo)
{
	m_pWzdInfo = pWzdInfo;
}

void CWzdPageDlg::InitCtrlInfo()
{
}
void CWzdPageDlg::ShowWzdData(bool bRefresh)
{

}

void CWzdPageDlg::GetWzdData(bool bNext)
{
	
}

void CWzdPageDlg::OnLeavePage()
{

}

//end
