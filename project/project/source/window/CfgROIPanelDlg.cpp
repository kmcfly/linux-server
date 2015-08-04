/*
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : xian shi wei
** Date         : 2014-10-20
** Name         : CfgROIPanelDlg.cpp 
** Version      : 1.0
** Description  : 
** Modify Record:
1:
*/
#include "CfgROIPanelDlg.h"

CCfgROIPanelDlg::CCfgROIPanelDlg() : m_clickCommand(CLICK_INVALID),m_iEnable(0),m_sensitivity(1){}

CCfgROIPanelDlg::~CCfgROIPanelDlg(){}

void CCfgROIPanelDlg::OnInitial()
{
	GUI::CDialog::OnInitial();
	m_style |= WND_NO_TITLE;
	//m_style |= WND_NO_BORDER;
	//m_style |= WND_NO_IMAGE_BK;
	//m_style |= WND_CHILD;

	int x = 5; 
	int y = 5;//(m_rect.Height() - m_length[STATIC_HEIGHT]) / 2;
	int cx = 120;

	int btn_cx = 0;
	int btn_cy = 0;
	GetImageSize(BMP_SAVE, btn_cx, btn_cy);

	unsigned short senscx = 0;
	unsigned short senscy = 0;
	TextSize(m_pStrTable->String(E_STR_ID_ENABLE),strlen(m_pStrTable->String(E_STR_ID_ENABLE)),senscx,senscy);
	if (senscx > cx)
	{
		senscx = cx;
	}
	m_enableRoi.Create(CTRLID_STATIC, x, y, senscx, m_length[STATIC_HEIGHT], this);
	m_enableRoi.SetBKColor(COLOR_DLG_CHILD_BK);
	x += senscx + 5;
	std::string strEnable = m_pStrTable->String(E_STR_ID_ENABLE);
	m_enableRoi.SetCaption(strEnable.c_str(), true);

	m_chEnableROI.Create(GetRandWndID(),x,y,btn_cx,btn_cy,this, 0, 0);
	m_chEnableROI.SetCheck(false,false);

	x += btn_cx+8;

	TextSize(m_pStrTable->String(E_STR_ID_SENSITIVITY), strlen(m_pStrTable->String(E_STR_ID_SENSITIVITY)), senscx, senscy);
	m_stSensitive.Create(CTRLID_STATIC, x, y, senscx, m_length[STATIC_HEIGHT], this);
	m_stSensitive.SetBKColor(COLOR_DLG_CHILD_BK);
	x += senscx + 5;
	
	cx = 80;
	m_sliderSensitive.Create(GetRandWndID(), x, y, cx, this, 1, 0);
	m_sliderSensitive.SetRange(0, 9);
	m_sensitivity = ( 11 > m_sensitivity )? ((m_sensitivity==0)?m_sensitivity=1:m_sensitivity): 10;
	m_sliderSensitive.SetCurPos((m_sensitivity-1), false);
	SetSensitiveInfo(m_sensitivity, false);

	x += cx + 5;
	cx = 20;
	m_stSensitiveNum.Create(CTRLID_STATIC, x, y, cx, m_length[STATIC_HEIGHT], this);
	m_stSensitiveNum.SetBKColor(COLOR_DLG_CHILD_BK);

	x = 5 + 120 + 5 + 80 + 10 +20 + 10;
	cx = 30;
	x += cx + 40;
	m_btnSave.Create(GetRandWndID(), x, y, btn_cx, btn_cy, this, 2, 0, NULL, WND_CHILD|WND_VISIBLE);
	m_btnSave.SetImage(PAINT_STATUS_NORMAL, BMP_SAVE, false);
	m_btnSave.SetImage(PAINT_STATUS_DISABLE, BMP_SAVE, false);
	m_btnSave.SetImage(PAINT_STATUS_FOCUS, BMP_SAVE_FOCUS, false);

	x += cx + 5;
	m_btnExit.Create(GetRandWndID(), x, y, btn_cx, btn_cy, this, 3, 0);
	m_btnExit.SetImage(PAINT_STATUS_NORMAL, BMP_BTN_EXIT2, false);
	m_btnExit.SetImage(PAINT_STATUS_DISABLE, BMP_BTN_EXIT2, false);
	m_btnExit.SetImage(PAINT_STATUS_FOCUS, BMP_BTN_EXIT2_FOCUS, false);

	/*m_stInfo.Create(CTRLID_STATIC, 5, y + btn_cy, m_rect.Width() - 10 - 3, m_length[STATIC_HEIGHT], this);
	m_stInfo.SetAlignText(GUI::ALIGN_TEXT_LEFT);*/

	//m_stInfo.SetBKColor(COLOR_DLG_CHILD_BK);
}

void CCfgROIPanelDlg::OnDestroy()
{
	GUI::CDialog::OnDestroy();
}

void CCfgROIPanelDlg::OnChangeFocus(unsigned long oldID, unsigned long newID)
{
	if (newID == m_btnSave.GetID())
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

void CCfgROIPanelDlg::SetSensitiveInfo(int value, bool bRefresh)
{
	std::string str = m_pStrTable->String(E_STR_ID_SENSITIVITY);
	m_stSensitive.SetCaption(str.c_str(), bRefresh);

	char sz[4] = {0};
	sprintf(sz, "%d", value);
	m_stSensitiveNum.SetCaption(sz, bRefresh);
}

CCfgROIPanelDlg::CLICK_COMMAND CCfgROIPanelDlg::GetClickCommand()
{
	CLICK_COMMAND res = m_clickCommand;
	m_clickCommand = CLICK_INVALID;

	return res;
}

void CCfgROIPanelDlg::SetInfoCaption(const std::string& str, bool bRefresh)
{
	m_stInfo.SetCaption(str, bRefresh);
}

void CCfgROIPanelDlg::OnPaint(PAINT_STATUS status,unsigned int enable,unsigned int sensitivety)
{
	//GUI::CDialog::OnPaint(PAINT_STATUS_NORMAL);
	m_btnSave.SetMark(true,COLOR_DLG_NORMAL_BK,true);
	m_btnExit.SetMark(true,COLOR_DLG_NORMAL_BK,true);

	m_enableRoi.SetBKColor(COLOR_DLG_CHILD_BK);

	//SetEnableROI(enable);
	if (enable != m_iEnable)
	{
		m_iEnable = enable;
	}
	if (enable >0 ){
		m_chEnableROI.SetCheck(true,false);
	}else{
		m_chEnableROI.SetCheck(false,false);
	}
	/*
	m_chEnableROI.SetCheck(true);
	m_chEnableROI.SetCaption("");
	*/
	m_sliderSensitive.SetSliderBoxImage(BMP_SLIDER_BOX,BMP_SLIDER_BOX_FOUCS,BMP_SLIDER_BK);

	//if ((sensitivety<11)&&(sensitivety>0))
	{
		10>sensitivety?sensitivety:sensitivety=10;
		m_sliderSensitive.SetCurPos((sensitivety-1),true);
		char str[4]={0};
		sprintf(str,"%d",sensitivety);
		m_stSensitiveNum.SetCaption(str,true);
	}
	m_stSensitive.SetBKColor(COLOR_DLG_CHILD_BK);

	//GUI::CWnd::RefreshLive();
	GUI::CWnd::Repaint(); //刷新整个窗口区域
}

void CCfgROIPanelDlg::SetSenstiveValue(unsigned long sensitivity)
{
	m_sensitivity = sensitivity;
	if ((sensitivity<11)&&(sensitivity>=0))
	{
		char str[4]={0};
		m_sliderSensitive.SetCurPos((sensitivity-1),true);
		sprintf(str,"%d",sensitivity);
		m_stSensitiveNum.SetCaption(str,true);
	}
	m_stSensitive.SetBKColor(COLOR_DLG_CHILD_BK);
}

BEGIN_MSG_MAP(CCfgROIPanelDlg, GUI::CDialog)
	ON_MSG( m_chEnableROI.GetID(), KEY_VIRTUAL_ENTER, OnCheckEnable)
	ON_MSG( m_btnSave.GetID(), KEY_VIRTUAL_ENTER, OnBtnSave)
	ON_MSG( m_btnExit.GetID(), KEY_VIRTUAL_ENTER, OnBtnExit )
	ON_MSG_EX(m_sliderSensitive.GetID(), KEY_VIRTUAL_ENTER, OnSensitiveChange)
	ON_MSG_EX(m_sliderSensitive.GetID(), KEY_VIRTUAL_DRAG, OnSensitiveChange)
	ON_MSG_EX(m_sliderSensitive.GetID(), KEY_VIRTUAL_BLUP, OnSensitiveChange)
END_MSG_MAP()


unsigned long CCfgROIPanelDlg::OnCheckEnable()
{
	if (m_chEnableROI.IsCheck()?1:0)
	{
		SetEnableROI(1);
		m_chEnableROI.SetCheck(true);
	}else{
		SetEnableROI(0);
		m_chEnableROI.SetCheck(false);
	}
	
	return KEY_VIRTUAL_MSG_CONTINUE;
}

unsigned long CCfgROIPanelDlg::OnBtnSave()
{
	m_clickCommand = CLICK_SAVE;
	return KEY_VIRTUAL_MSG_CONTINUE;
}

unsigned long CCfgROIPanelDlg::OnBtnExit()
{
	m_clickCommand = CLICK_EXIT;
	return KEY_VIRTUAL_MSG_CONTINUE;
}

unsigned long CCfgROIPanelDlg::OnSensitiveChange(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	int pos = m_sliderSensitive.GetPos();
	9>pos ? pos : pos=9;
	SetSensitiveInfo(pos+1, true);
	m_sensitivity = pos+1;

	m_clickCommand = CLICK_SENSI;
	return KEY_VIRTUAL_MSG_CONTINUE;
}