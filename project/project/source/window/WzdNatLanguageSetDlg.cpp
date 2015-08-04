/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : ZXX
** Date         : 2013-11-26
** Name         : WzdNatLanguageSetDlg.cpp
** Version      : 1.0
** Description  : 向导管理对话框
** Modify Record:
***********************************************************************/

#include "WzdNatLanguageSetDlg.h"


CWzdNatLanguageSetDlg::CWzdNatLanguageSetDlg()
{

}

CWzdNatLanguageSetDlg::~CWzdNatLanguageSetDlg()
{

}

void CWzdNatLanguageSetDlg::InitCtrlInfo()
{
	m_selectLanguageCombo.RemoveAllItem(false);
	for(int i = 0; i < m_pNatLanguageWzdInfo->LanguageNum;  ++i)
	{
		unsigned long langID = m_pNatLanguageWzdInfo->Language[i].languageId;
		m_selectLanguageCombo.AddItem(m_pNatLanguageWzdInfo->Language[i].name, (LONGLONG)langID);
	}
	m_selectLanguageCombo.SetCurItemData(m_pNatLanguageWzdInfo->CurrentLanguage, false);
}

void CWzdNatLanguageSetDlg::SetWzdInfo(NAT_WIZARD_INFO* pWzdInfo)
{
	m_pNatLanguageWzdInfo = pWzdInfo;
}

void CWzdNatLanguageSetDlg::GetWzdData(bool bNext)
{
	m_pNatLanguageWzdInfo->CurrentLanguage = m_selectLanguageCombo.GetCurItemData();

}

void CWzdNatLanguageSetDlg::OnInitial()
{
	CWzdNatPageDlg::OnInitial();
	//////////////////////////////////////////////////////////////////////////
	unsigned short x = (m_rect.Width() - 2 * m_length[DLG_OFFSET_SUB_LEFT])/2-2*m_length[BTN_WIDTH];
	unsigned short y = (m_rect.Height() - m_length[DLG_OFFSET_SUB_BOTTOM])/4;
	unsigned short tipWidth = 2*m_length[BTN_WIDTH];

	m_staticTip.Create(GetRandWndID(),x,y,tipWidth,m_length[STATIC_HEIGHT],this);
	// m_staticTip.SetCaption("Select Language:");
	m_staticTip.SetCaption(m_pStrTable->String(E_STR_ID_NAT_WZD_LANGUAGE_04));

	m_selectLanguageCombo.Create(GetRandWndID(),x+tipWidth,y,2*m_length[BTN_WIDTH],m_length[COMBO_HEIGHT],this,0,0);
	unsigned long textX = m_length[DLG_OFFSET_SUB_LEFT];
	unsigned long textY = y+m_length[COMBO_HEIGHT];
	unsigned long cx = m_rect.Width() - 2 * m_length[DLG_OFFSET_SUB_LEFT];
	unsigned long cy = (m_rect.Height() - m_length[DLG_OFFSET_SUB_BOTTOM])/3*2;
	m_richTextCtrl.Create(GetRandWndID(),textX,textY,cx,cy,this);
	string strInfo;
	// strInfo.append("IMPORTANT!");
	strInfo.append(m_pStrTable->String(E_STR_ID_NAT_WZD_LANGUAGE_00));
	strInfo.append("\\n");
	// strInfo.append("If you change the language selection, your system will need to restart.");
	strInfo.append(m_pStrTable->String(E_STR_ID_NAT_WZD_LANGUAGE_01));
	strInfo.append("\\n");
	// strInfo.append("You can then continue with the setup.");
	strInfo.append(m_pStrTable->String(E_STR_ID_NAT_WZD_LANGUAGE_02));
	strInfo.append("\\n");
	// strInfo.append("Please make your settings in all three tabs above, before clicking Next.");
	strInfo.append(m_pStrTable->String(E_STR_ID_NAT_WZD_LANGUAGE_03));
	strInfo.append("\\n");
	m_richTextCtrl.SetText(strInfo,false);
}

void CWzdNatLanguageSetDlg::OnDestroy()
{
	CWzdNatPageDlg::OnDestroy();
}

//语言改变立即生效
BEGIN_MSG_MAP(CWzdNatLanguageSetDlg, CWzdNatPageDlg)
	ON_MSG(m_selectLanguageCombo.GetID(),KEY_VIRTUAL_ENTER,OnChangeLanguage)
END_MSG_MAP()

unsigned long CWzdNatLanguageSetDlg::OnChangeLanguage()
{
	if(m_selectLanguageCombo.GetCurItemData()  != m_pNatLanguageWzdInfo->CurrentLanguage)
	{
		GetWzdData(true);
		return KEY_VIRTUAL_LANGUAGE;
	}
	else
	{
		return KEY_VIRTUAL_MSG_STOP;
	}
}