/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : hz
** Date         : 2009-08-12
** Name         : CfgScheduleTimeDlg.cpp 
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/

#include "CfgScheduleTimeDlg.h"

CCfgScheduleTimeDlg::CCfgScheduleTimeDlg()
{

}

CCfgScheduleTimeDlg::~CCfgScheduleTimeDlg()
{

}

void CCfgScheduleTimeDlg::OnInitial()
{
	GUI::CDialog::OnInitial();
	m_style |= WND_NO_TITLE;
	SetDefaultBtn( DEFAULT_BTN_OK | DEFAULT_BTN_CANCEL );
	SetDefaultBtnImage(DEFAULT_BTN_OK, BMP_BUTTON_DLG_OK, BMP_BUTTON_DLG_OK_F, BMP_BUTTON_DLG_OK);
	SetDefaultBtnImage(DEFAULT_BTN_CANCEL, BMP_BTN_EXIT4, BMP_BTN_EXIT4_FOCUS, BMP_BTN_EXIT4);
	//////////////////////////////////////////////////////////////////////////
	tm tm_time;
	memset(&tm_time, 0, sizeof(tm_time));

	int base_x = 15;
	int x = base_x;
	int y = 5;
	int combo_cx = 100;
	//int offset_x = 24;
	int titel_cx = 60;
	m_groupCtrl[0].Create(CTRLID_STATIC, base_x - 5, y, m_rect.Width() - x - 5, m_length[STATIC_HEIGHT] + m_length[COMBO_HEIGHT] + 5, this, m_pStrTable->String(E_STR_ID_START_TIME));

	y += m_length[STATIC_HEIGHT];
	m_staticStart[0].Create(CTRLID_STATIC, x, y, titel_cx - 2, m_length[STATIC_HEIGHT], this, m_pStrTable->String(E_STR_ID_HOUR));
	x += titel_cx;
	m_comboStartHour.Create(GetRandWndID(), x, y, combo_cx, m_length[COMBO_HEIGHT], this, 0, 1);
	x += combo_cx + 20;
	m_staticStart[1].Create(CTRLID_STATIC, x, y, titel_cx - 2, m_length[STATIC_HEIGHT], this, m_pStrTable->String(E_STR_ID_MINUTE));
	x += titel_cx;
	m_comboStartMinute.Create(GetRandWndID(), x, y, combo_cx, m_length[COMBO_HEIGHT], this, 1, 1);


	x = base_x;
	y += m_length[COMBO_HEIGHT] + 10;
	m_groupCtrl[1].Create(CTRLID_STATIC, base_x - 5, y, m_rect.Width() - x - 5, m_length[STATIC_HEIGHT] + m_length[COMBO_HEIGHT] + 5, this, m_pStrTable->String(E_STR_ID_END_TIME));
	
	y += m_length[STATIC_HEIGHT];
	m_staticEnd[0].Create(CTRLID_STATIC, x, y, titel_cx - 2, m_length[STATIC_HEIGHT], this, m_pStrTable->String(E_STR_ID_HOUR));
	x += titel_cx;
	m_comboEndHour.Create(GetRandWndID(), x, y, combo_cx, m_length[COMBO_HEIGHT], this, 0, 2);
	x += combo_cx + 20;
	m_staticEnd[1].Create(CTRLID_STATIC, x, y, titel_cx - 2, m_length[STATIC_HEIGHT], this, m_pStrTable->String(E_STR_ID_MINUTE));
	x += titel_cx;
	m_comboEndMinute.Create(GetRandWndID(), x, y, combo_cx, m_length[COMBO_HEIGHT], this, 1, 2);
	//////////////////////////////////////////////////////////////////////////
	char sz[8] = {0};
	for (int i = 0; i < 24; ++i)
	{
		sprintf(sz, "%d", i);
		m_comboStartHour.AddItem(sz, i);
		m_comboEndHour.AddItem(sz, i);
	}
	m_comboStartHour.SetCurItem(0);
	m_comboEndHour.SetCurItem(23);
	for (int i = 0; i < 60; ++i)
	{
		sprintf(sz, "%d", i);
		m_comboStartMinute.AddItem(sz, i);
		m_comboEndMinute.AddItem(sz, i);
	}
	m_comboStartMinute.SetCurItem(0);
	m_comboEndMinute.SetCurItem(59);
}

void CCfgScheduleTimeDlg::OnOutOfDlg(unsigned long keyID)
{
	GUI::CDialog::OnOutOfDlg(keyID);

	if (KEY_VIRTUAL_ENTER == keyID)
	{
		EndDialog(DLG_ESC);
	}
}

unsigned long CCfgScheduleTimeDlg::OnOK()
{
	m_section.startTimeHour = static_cast<unsigned char>(m_comboStartHour.GetCurItemData());
	m_section.startTimeMinute = static_cast<unsigned char>(m_comboStartMinute.GetCurItemData());

	m_section.endTimeHour = static_cast<unsigned char>(m_comboEndHour.GetCurItemData());
	m_section.endTimeMinute = static_cast<unsigned char>(m_comboEndMinute.GetCurItemData());

	if(m_section.startTimeHour > m_section.endTimeHour)
	{
		MessageBox(m_pStrTable->String(E_STR_ID_MESSAGE_51), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK);
		return KEY_VIRTUAL_MSG_CONTINUE;
	}
	else if(m_section.startTimeHour == m_section.endTimeHour)
	{
		if(m_section.startTimeMinute >= m_section.endTimeMinute)
		{
			MessageBox(m_pStrTable->String(E_STR_ID_MESSAGE_51), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK);
			return KEY_VIRTUAL_MSG_CONTINUE;
		}
	}
	return GUI::CDialog::OnOK();
}

