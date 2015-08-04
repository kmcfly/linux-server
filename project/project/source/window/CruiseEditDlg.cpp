/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : hz
** Date         : 2009-08-12
** Name         : CruiseEditDlg.cpp 
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/

#include "CruiseEditDlg.h"

std::vector<PTZ_PRESET> CCruiseEditDlg::m_presetList;

CCruiseEditDlg::CCruiseEditDlg()
{
	m_curisePoint.dwellSpeed = 1;
	m_curisePoint.dwellTime = 5;
	m_curisePoint.preset = 0;
}

CCruiseEditDlg::~CCruiseEditDlg()
{

}

void CCruiseEditDlg::OnInitial()
{
	GUI::CDialog::OnInitial();
	m_style |= WND_NO_TITLE;
	SetDefaultBtn( DEFAULT_BTN_OK | DEFAULT_BTN_CANCEL );
	SetDefaultBtnImage(DEFAULT_BTN_OK, BMP_BUTTON_DLG_OK, BMP_BUTTON_DLG_OK_F, BMP_BUTTON_DLG_OK);
	SetDefaultBtnImage(DEFAULT_BTN_CANCEL, BMP_BTN_EXIT4, BMP_BTN_EXIT4_FOCUS, BMP_BTN_EXIT4);
	//////////////////////////////////////////////////////////////////////////
	char sz[16] = {0};
	int title_cx = 70;
	int combo_cx = 130;
	int cy = m_length[COMBO_HEIGHT] + 10;
	int x = 10;
	int y = 5;

	m_stPreset.Create(CTRLID_STATIC, x, y, title_cx - 2, m_length[STATIC_HEIGHT], this, m_pStrTable->String(E_STR_ID_PRESET));
	m_comboPreset.Create(GetRandWndID(), x + title_cx, y, combo_cx, m_length[COMBO_HEIGHT], this, 0, 0);
	for (std::vector<PTZ_PRESET>::const_iterator iter = m_presetList.begin(); iter != m_presetList.end(); ++iter)
	{
		sprintf(sz, "%d", iter->index + 1);
		m_comboPreset.AddItem(sz, iter->index);
	}
	if( !m_presetList.empty())
	{
		m_comboPreset.SetCurItemData(m_curisePoint.preset, false);
	}

	y += cy;
	m_editPresetName.Create(GetRandWndID(), x + title_cx, y, combo_cx, m_length[EDIT_HEIGHT], this, 0, 1);
	m_editPresetName.SetEnable(false, false);
	for (std::vector<PTZ_PRESET>::const_iterator iter = m_presetList.begin(); iter != m_presetList.end(); ++iter)
	{
		if (iter->index == m_curisePoint.preset)
		{
			m_editPresetName.SetCaption(iter->name, false);
		}
	}

	y += cy;
	m_stSpeed.Create(CTRLID_STATIC, x, y, title_cx - 2, m_length[STATIC_HEIGHT], this, m_pStrTable->String(E_STR_ID_SPEED));
	m_comboSpeed.Create(GetRandWndID(), x + title_cx, y, combo_cx, m_length[COMBO_HEIGHT], this, 0, 2);
	for (int i = 1; i <= 8; ++i)
	{
		sprintf(sz, "%d", i);
		m_comboSpeed.AddItem(sz, i);
	}
	m_comboSpeed.SetCurItemData(m_curisePoint.dwellSpeed, false);

	y += cy;
	m_stTime.Create(CTRLID_STATIC, x, y, title_cx - 2, m_length[STATIC_HEIGHT], this, m_pStrTable->String(E_STR_ID_TIME));
	m_comboTime.Create(GetRandWndID(), x + title_cx, y, combo_cx, m_length[COMBO_HEIGHT], this, 0, 3);
	m_comboTime.AddItem("5 sec", 5);
	m_comboTime.AddItem("10 sec", 10);
	m_comboTime.AddItem("15 sec", 15);
	m_comboTime.AddItem("20 sec", 20);
	m_comboTime.AddItem("30 sec", 30);
	m_comboTime.AddItem("60 sec", 60);
	m_comboTime.SetCurItemData(m_curisePoint.dwellTime, false);
}

void CCruiseEditDlg::OnOutOfDlg(unsigned long keyID)
{
	GUI::CDialog::OnOutOfDlg(keyID);

	if (KEY_VIRTUAL_ENTER == keyID)
	{
		EndDialog(DLG_ESC);
	}
}

unsigned long CCruiseEditDlg::OnOK()
{
	m_curisePoint.preset = m_comboPreset.GetCurItemData();
	m_curisePoint.dwellSpeed = m_comboSpeed.GetCurItemData();
	m_curisePoint.dwellTime = m_comboTime.GetCurItemData();

	return GUI::CDialog::OnOK();
}

void CCruiseEditDlg::SetPresetList(const std::vector<PTZ_PRESET>& presetList)
{
	m_presetList = presetList;
}

BEGIN_MSG_MAP(CCruiseEditDlg, GUI::CDialog)
	ON_MSG(m_comboPreset.GetID(), KEY_VIRTUAL_ENTER, OnClickPreset)
END_MSG_MAP()

unsigned long CCruiseEditDlg::OnClickPreset()
{
	int sel = m_comboPreset.GetCurItem();
//	char sz[64] = {0};
//	sprintf(sz, "Preset%d", sel + 1);
	if (sel >= 0)
	{
		m_editPresetName.SetCaption(m_presetList[sel].name, true);
	}

	return KEY_VIRTUAL_MSG_STOP;
}

