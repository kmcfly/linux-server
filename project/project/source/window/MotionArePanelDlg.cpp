/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : hz
** Date         : 2009-08-01
** Name         : MotionArePanelDlg.cpp 
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/
#include "MotionArePanelDlg.h"

CMotionArePanelDlg::CMotionArePanelDlg() : m_clickCommand(CLICK_INVALID)
{
	m_bEnableTest = true;
}

CMotionArePanelDlg::~CMotionArePanelDlg()
{

}

//////////////////////////////////////////////////////////////////////////
void CMotionArePanelDlg::OnInitial()
{
	GUI::CDialog::OnInitial();
	m_style |= WND_NO_TITLE;
	//////////////////////////////////////////////////////////////////////////
	int x = 5; 
	int y = 5;//(m_rect.Height() - m_length[STATIC_HEIGHT]) / 2;
	int cx = 120;

	int btn_cx = 0;
	int btn_cy = 0;
	GetImageSize(BMP_SAVE, btn_cx, btn_cy);

	unsigned short senscx = 0;
	unsigned short senscy = 0;
	TextSize(m_pStrTable->String(E_STR_ID_SENSITIVITY), strlen(m_pStrTable->String(E_STR_ID_SENSITIVITY)), senscx, senscy);
	if (senscx > cx)
	{
		senscx = cx;
	}
	
	m_stSensitive.Create(CTRLID_STATIC, x, y, senscx, m_length[STATIC_HEIGHT], this);
	m_stSensitive.SetBKColor(COLOR_DLG_CHILD_BK);

	x += senscx + 5;
	cx = 80;
	m_sliderSensitive.Create(GetRandWndID(), x, y, cx, this, 0, 0);
	m_sliderSensitive.SetRange(0, 7);
	m_sensitivity = ( 7 > m_sensitivity )?m_sensitivity:7;
	m_sliderSensitive.SetCurPos(m_sensitivity, false);
	SetSensitiveInfo(m_sensitivity, false);

	x += cx + 5;
	cx = 10;
	m_stSensitiveNum.Create(CTRLID_STATIC, x, y, cx, m_length[STATIC_HEIGHT], this);
	m_stSensitiveNum.SetBKColor(COLOR_DLG_CHILD_BK);

	x = 5 + 120 + 5 + 80 + 5 +10 + 5;
	cx = 24;
	m_btnAll.Create(GetRandWndID(), x, y, btn_cx, btn_cy, this, 1, 0);
	m_btnAll.SetImage(PAINT_STATUS_NORMAL, BMP_ALL, false);
	m_btnAll.SetImage(PAINT_STATUS_DISABLE, BMP_ALL, false);
	m_btnAll.SetImage(PAINT_STATUS_FOCUS, BMP_ALL_FOCUS, false);

	x += cx + 10;
	m_btnClear.Create(GetRandWndID(), x, y, btn_cx, btn_cy, this, 2, 0);
	m_btnClear.SetImage(PAINT_STATUS_NORMAL, BMP_CLEAR, false);
	m_btnClear.SetImage(PAINT_STATUS_DISABLE, BMP_CLEAR, false);
	m_btnClear.SetImage(PAINT_STATUS_FOCUS, BMP_CLEAR_FOCUS, false);

	if(m_bEnableTest)
	{
		x += cx + 10;
		m_btnTest.Create(GetRandWndID(), x, y, btn_cx, btn_cy, this, 3, 0);
		m_btnTest.SetImage(PAINT_STATUS_NORMAL, BMP_CUSTOM, false);
		m_btnTest.SetImage(PAINT_STATUS_DISABLE, BMP_CUSTOM, false);
		m_btnTest.SetImage(PAINT_STATUS_FOCUS, BMP_CUSTOM_FOCUS, false);
	}
	
	x += cx + 10;
	m_btnSave.Create(GetRandWndID(), x, y, btn_cx, btn_cy, this, 4, 0, NULL, WND_CHILD|WND_VISIBLE);
	m_btnSave.SetImage(PAINT_STATUS_NORMAL, BMP_SAVE, false);
	m_btnSave.SetImage(PAINT_STATUS_DISABLE, BMP_SAVE, false);
	m_btnSave.SetImage(PAINT_STATUS_FOCUS, BMP_SAVE_FOCUS, false);

	x += cx + 10;
	m_btnExit.Create(GetRandWndID(), x, y, btn_cx, btn_cy, this, 5, 0);
	m_btnExit.SetImage(PAINT_STATUS_NORMAL, BMP_BTN_EXIT2, false);
	m_btnExit.SetImage(PAINT_STATUS_DISABLE, BMP_BTN_EXIT2, false);
	m_btnExit.SetImage(PAINT_STATUS_FOCUS, BMP_BTN_EXIT2_FOCUS, false);


	m_stInfo.Create(CTRLID_STATIC, 5, y + btn_cy, m_rect.Width() - 10 - 3, m_length[STATIC_HEIGHT], this);
	m_stInfo.SetAlignText(GUI::ALIGN_TEXT_LEFT);
	m_stInfo.SetBKColor(COLOR_DLG_CHILD_BK);

}

void CMotionArePanelDlg::OnDestroy()
{
	GUI::CDialog::OnDestroy();

}

void CMotionArePanelDlg::OnChangeFocus(unsigned long oldID, unsigned long newID)
{
	if (newID == m_btnAll.GetID())
	{
		m_stInfo.SetCaption(m_pStrTable->String(E_STR_ID_ALL));
	} 
	else if ((newID == m_btnTest.GetID()) && m_bEnableTest)
	{
		m_stInfo.SetCaption(m_pStrTable->String(E_STR_ID_TEST));
	}
	else if (newID == m_btnClear.GetID())
	{
		m_stInfo.SetCaption(m_pStrTable->String(E_STR_ID_CLEAR));
	}
	else if (newID == m_btnSave.GetID())
	{
		m_stInfo.SetCaption(m_pStrTable->String(E_STR_ID_SAVE));
	}
	else if (newID == m_btnExit.GetID())
	{
		m_stInfo.SetCaption(m_pStrTable->String(E_STR_ID_EXIT));
	}
	else 
	{
		m_stInfo.SetCaption(" ");
	}
}

void CMotionArePanelDlg::SetSensitiveInfo(int value, bool bRefresh)
{
	//char sz[8] = {0};
	//sprintf(sz, ": %d", value + 1);
	std::string str = m_pStrTable->String(E_STR_ID_SENSITIVITY);
	//str += sz;
	m_stSensitive.SetCaption(str.c_str(), bRefresh);

	char sz[8] = {0};
	sprintf(sz, "%d", value + 1);
	m_stSensitiveNum.SetCaption(sz, bRefresh);
}

CMotionArePanelDlg::CLICK_COMMAND CMotionArePanelDlg::GetClickCommand()
{
	CLICK_COMMAND res = m_clickCommand;
	m_clickCommand = CLICK_INVALID;

	return res;
}

void CMotionArePanelDlg::SetInfoCaption(const std::string& str, bool bRefresh)
{
	m_stInfo.SetCaption(str, bRefresh);
}

void CMotionArePanelDlg::EnableTest(bool bEnable)
{
	m_bEnableTest = bEnable;
}

//void CMotionArePanelDlg::EndDialog(DLG_END_VALUE result)
//{
//	OnBtnHide();
//}


//void CMotionArePanelDlg::OnPaint(PAINT_STATUS status)
//{
//		GUI::CWnd::DrawRect(m_rect.m_left + m_rectPanel.m_left, m_rect.m_top + m_rectPanel.m_top, m_rectPanel.Width(), m_rectPanel.Height(), COLOR_DLG_CHILD_BK);
//
//		GUI::CWnd::DrawRectBorder(m_rect.m_left + m_rectPanel.m_left, m_rect.m_top + m_rectPanel.m_top, m_rectPanel.Width(), m_rectPanel.Height(), GUI_RGB(80, 80, 80), 3);
//		GUI::CWnd::DrawRectBorder(m_rect.m_left + m_rectPanel.m_left, m_rect.m_top + m_rectPanel.m_top, m_rectPanel.Width(), m_rectPanel.Height(), GUI_RGB(128, 128, 128), 1);
//}

BEGIN_MSG_MAP(CMotionArePanelDlg, GUI::CDialog)
	ON_MSG( m_btnAll.GetID(), KEY_VIRTUAL_ENTER, OnBtnAll)
	ON_MSG( m_btnTest.GetID(), KEY_VIRTUAL_ENTER, OnBtnTest )
	ON_MSG( m_btnSave.GetID(), KEY_VIRTUAL_ENTER, OnBtnSave)
	ON_MSG( m_btnClear.GetID(), KEY_VIRTUAL_ENTER, OnBtnClear )
	ON_MSG( m_btnExit.GetID(), KEY_VIRTUAL_ENTER, OnBtnExit )
	ON_MSG_EX(m_sliderSensitive.GetID(), KEY_VIRTUAL_ENTER, OnSensitiveChange)
	ON_MSG_EX(m_sliderSensitive.GetID(), KEY_VIRTUAL_DRAG, OnSensitiveChange)
	ON_MSG_EX(m_sliderSensitive.GetID(), KEY_VIRTUAL_BLUP, OnSensitiveChange)
END_MSG_MAP()

unsigned long CMotionArePanelDlg::OnSensitiveChange(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	int pos = m_sliderSensitive.GetPos();
	SetSensitiveInfo(pos, true);
	m_sensitivity = pos;

	m_clickCommand = CLICK_SENSI;
	return KEY_VIRTUAL_MSG_CONTINUE;
}

unsigned long CMotionArePanelDlg::OnBtnAll()
{
	m_clickCommand = CLICK_ALL;
	return KEY_VIRTUAL_MSG_CONTINUE;
}

unsigned long CMotionArePanelDlg::OnBtnTest()
{
	m_clickCommand = CLICK_TEST;
	return KEY_VIRTUAL_MSG_CONTINUE;
}

unsigned long CMotionArePanelDlg::OnBtnSave()
{
	m_clickCommand = CLICK_SAVE;
	return KEY_VIRTUAL_MSG_CONTINUE;
}

unsigned long CMotionArePanelDlg::OnBtnClear()
{
	m_clickCommand = CLICK_CLEAR;
	return KEY_VIRTUAL_MSG_CONTINUE;
}

unsigned long CMotionArePanelDlg::OnBtnExit()
{
	m_clickCommand = CLICK_EXIT;
	return KEY_VIRTUAL_MSG_CONTINUE;
}

