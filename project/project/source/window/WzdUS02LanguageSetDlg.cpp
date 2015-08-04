/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : ZXX
** Date         : 2012-10-16
** Name         : WzdUS02LanguageSetDlg.cpp
** Version      : 1.0
** Description  : 向导管理对话框
** Modify Record:
***********************************************************************/

#include "WzdUS02LanguageSetDlg.h"


CWzdUS02LanguageSetDlg::CWzdUS02LanguageSetDlg()
{

}

CWzdUS02LanguageSetDlg::~CWzdUS02LanguageSetDlg()
{

}

void CWzdUS02LanguageSetDlg::InitCtrlInfo()
{
	m_selectLanguageCombo.RemoveAllItem(false);
	for(int i = 0; i < m_pUS02LanguageWzdInfo->LanguageNum;  ++i)
	{
		unsigned long langID = m_pUS02LanguageWzdInfo->Language[i].languageId;
		m_selectLanguageCombo.AddItem(m_pUS02LanguageWzdInfo->Language[i].name, (LONGLONG)langID);
	}
	m_selectLanguageCombo.SetCurItemData(m_pUS02LanguageWzdInfo->CurrentLanguage, false);
}

void CWzdUS02LanguageSetDlg::SetWzdInfo(US02_WzdInfo* pWzdInfo)
{
	m_pUS02LanguageWzdInfo = pWzdInfo;
}

void CWzdUS02LanguageSetDlg::GetWzdData(bool bNext)
{
	m_pUS02LanguageWzdInfo->CurrentLanguage = m_selectLanguageCombo.GetCurItemData();

}

void CWzdUS02LanguageSetDlg::OnInitial()
{
	CWzdUS02PageDlg::OnInitial();
	//////////////////////////////////////////////////////////////////////////
	unsigned short x = (m_rect.Width() - 2 * m_length[DLG_OFFSET_SUB_LEFT])/2-2*m_length[BTN_WIDTH];
	unsigned short y = (m_rect.Height() - m_length[DLG_OFFSET_SUB_BOTTOM])/2;
	unsigned short tipWidth = 2*m_length[BTN_WIDTH];

	m_staticTip.Create(GetRandWndID(),x,y,tipWidth,m_length[STATIC_HEIGHT],this);
	m_staticTip.SetCaption("Select Language:");

	m_selectLanguageCombo.Create(GetRandWndID(),x+tipWidth,y,2*m_length[BTN_WIDTH],m_length[COMBO_HEIGHT],this,0,0);

}

void CWzdUS02LanguageSetDlg::OnDestroy()
{
	CWzdUS02PageDlg::OnDestroy();
}

//语言改变立即生效
BEGIN_MSG_MAP(CWzdUS02LanguageSetDlg, CWzdUS02PageDlg)
	ON_MSG(m_selectLanguageCombo.GetID(),KEY_VIRTUAL_ENTER,OnChangeLanguage)
END_MSG_MAP()

unsigned long CWzdUS02LanguageSetDlg::OnChangeLanguage()
{
	if(m_selectLanguageCombo.GetCurItemData()  != m_pUS02LanguageWzdInfo->CurrentLanguage)
	{
		GetWzdData(true);
		return KEY_VIRTUAL_LANGUAGE;
	}
	else
	{
		return KEY_VIRTUAL_MSG_STOP;
	}
}