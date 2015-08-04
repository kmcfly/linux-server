/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : ZXX
** Date         : 2012-09-17
** Name         : WzdUS02PageDlg.h
** Version      : 1.0
** Description  : 向导对话框
** Modify Record:
***********************************************************************/

#include "WzdUS02PageDlg.h"

CWzdUS02PageDlg::CWzdUS02PageDlg() : m_pWzdInfo(NULL),
m_bUsed(true)
{

}

CWzdUS02PageDlg::~CWzdUS02PageDlg()
{

}

void CWzdUS02PageDlg::SetUsed(bool bUsed)
{
	m_bUsed = bUsed;
}

bool CWzdUS02PageDlg::IsUsed()
{
	return m_bUsed;
}

//bool SetRefreshWzdLastTime()
//{
//	if()  
//}

void CWzdUS02PageDlg::SetWzdInfo(US02_WzdInfo* pWzdInfo)
{
	m_pWzdInfo = pWzdInfo;
}

void CWzdUS02PageDlg::InitCtrlInfo()
{
}
void CWzdUS02PageDlg::ShowWzdData(bool bRefresh)
{

}

void CWzdUS02PageDlg::GetWzdData(bool bNext)
{

}

void CWzdUS02PageDlg::OnLeavePage()
{

}

 unsigned long CWzdUS02PageDlg::CheckValue()
 {
	return 0;
 }


//end

