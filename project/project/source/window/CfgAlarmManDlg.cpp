/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : hz
** Date         : 2009-08-01
** Name         : CfgAlarmManDlg.cpp
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/
#include "CfgAlarmManDlg.h"
#include "FuncCustom.h"

extern NET_LOGIN_SUCCESS_REPLY *g_p_login_succ;
CCfgAlarmManDlg::CCfgAlarmManDlg()
{

}

CCfgAlarmManDlg::~CCfgAlarmManDlg()
{

}

//////////////////////////////////////////////////////////////////////////
void CCfgAlarmManDlg::OnInitial()
{
	GUI::CDialog::OnInitial();
	SetImage(BMP_MEM_DLG_NORMAL, false);
	SetCaption(m_pStrTable->String(E_STR_ID_TITLE_ALARM), false);
	//////////////////////////////////////////////////////////////////////////

#ifndef __DVR_ANCHOR__
	unsigned short size_btn = 72;
	unsigned short size_text = 150;
	unsigned short offset_text = (size_text - size_btn) / 2;

	unsigned short base_x = 10;
	unsigned short x = base_x;
	unsigned short y = 60;
	unsigned short col = 0;
	if (IsSupportAlarmOut() || (g_p_login_succ->productInfo.netVideoInputNum > 0))
	{
		m_btnSensor.Create(GetRandWndID(), x + offset_text, y, size_btn, size_btn, this, 0, 1, NULL, WND_CHILD | WND_VISIBLE | WND_NO_IMAGE_BK);
		m_btnSensor.SetImage(PAINT_STATUS_NORMAL, BMP_SENSOR, false);
		m_btnSensor.SetImage(PAINT_STATUS_DISABLE, BMP_SENSOR, false);
		m_btnSensor.SetImage(PAINT_STATUS_FOCUS, BMP_SENSOR_FOCUS, false);
		m_stcSensor.Create(CTRLID_STATIC, x, y+size_btn, size_text, m_length[STATIC_HEIGHT], this, m_pStrTable->String(E_STR_ID_MENU_CFG_SENSOR));
		m_stcSensor.SetAlignText(GUI::ALIGN_TEXT_CENTER);

		x += size_text;
		col++;
	}
	m_btnMotion.Create(GetRandWndID(), x + offset_text, y, size_btn, size_btn, this, 1, 1, NULL, WND_CHILD | WND_VISIBLE | WND_NO_IMAGE_BK);
	m_btnMotion.SetImage(PAINT_STATUS_NORMAL, BMP_MOTION, false);
	m_btnMotion.SetImage(PAINT_STATUS_DISABLE, BMP_MOTION, false);
	m_btnMotion.SetImage(PAINT_STATUS_FOCUS, BMP_MOTION_FOCUS, false);
	m_stcMotion.Create(CTRLID_STATIC, x, y+size_btn, size_text, m_length[STATIC_HEIGHT], this, m_pStrTable->String(E_STR_ID_MENU_CFG_MOTION));
	m_stcMotion.SetAlignText(GUI::ALIGN_TEXT_CENTER);

// #ifndef __CUSTOM_SHANGHAI__
// 	if (CCfgDlg::s_loginSuccessReply.productInfo.localVideoInputNum > 0)
// #endif
	{
		x += size_text;
		col++;
		m_btnVideoLoss.Create(GetRandWndID(), x + offset_text, y, size_btn, size_btn, this, 2, 1, NULL, WND_CHILD | WND_VISIBLE | WND_NO_IMAGE_BK);
		m_btnVideoLoss.SetImage(PAINT_STATUS_NORMAL, BMP_VIDEO_LOSS, false);
		m_btnVideoLoss.SetImage(PAINT_STATUS_DISABLE, BMP_VIDEO_LOSS, false);
		m_btnVideoLoss.SetImage(PAINT_STATUS_FOCUS, BMP_VIDEO_LOSS_FOCUS, false);
		m_stcVideoLoss.Create(CTRLID_STATIC, x, y+size_btn, size_text, m_length[STATIC_HEIGHT], this, m_pStrTable->String(E_STR_ID_MENU_CFG_VIDEO_LOSS));
		m_stcVideoLoss.SetAlignText(GUI::ALIGN_TEXT_CENTER);

	}
	x += size_text;
	col++;
	m_btnOtherAlarm.Create(GetRandWndID(), x + offset_text, y, size_btn, size_btn, this, 3, 1, NULL, WND_CHILD | WND_VISIBLE | WND_NO_IMAGE_BK);
	m_btnOtherAlarm.SetImage(PAINT_STATUS_NORMAL, BMP_OTHER_ALARM, false);
	m_btnOtherAlarm.SetImage(PAINT_STATUS_DISABLE, BMP_OTHER_ALARM, false);
	m_btnOtherAlarm.SetImage(PAINT_STATUS_FOCUS, BMP_OTHER_ALARM_FOCUS, false);
	m_stcOtherAlarm.Create(CTRLID_STATIC, x, y+size_btn, size_text, m_length[STATIC_HEIGHT], this, m_pStrTable->String(E_STR_ID_MENU_CFG_OTHER_ALARM));
	m_stcOtherAlarm.SetAlignText(GUI::ALIGN_TEXT_CENTER);

	if((3 == col) && (x != base_x))
	{
		x = base_x;
		y += 120;
	}
	else
	{
		x += size_text;
		col++;
	}

	m_btnAlarmOut.Create(GetRandWndID(), x + offset_text, y, size_btn, size_btn, this, 0, 2, NULL, WND_CHILD | WND_VISIBLE | WND_NO_IMAGE_BK);
	m_btnAlarmOut.SetImage(PAINT_STATUS_NORMAL, BMP_ALARM_OUT, false);
	m_btnAlarmOut.SetImage(PAINT_STATUS_DISABLE, BMP_ALARM_OUT, false);
	m_btnAlarmOut.SetImage(PAINT_STATUS_FOCUS, BMP_ALARM_OUT_FOCUS, false);
	m_stcAlarmOut.Create(CTRLID_STATIC, x , y+size_btn, size_text, m_length[STATIC_HEIGHT], this, m_pStrTable->String(E_STR_ID_MENU_CFG_ALARM_OUT));
	m_stcAlarmOut.SetAlignText(GUI::ALIGN_TEXT_CENTER);

#ifdef __MOBILE_DVR__
	x += size_text;
	col++;
	m_btnSpeedAlarm.Create(GetRandWndID(), x + offset_text, y, size_btn, size_btn, this, 1, 2, NULL, WND_CHILD | WND_VISIBLE | WND_NO_IMAGE_BK);
	m_btnSpeedAlarm.SetImage(PAINT_STATUS_NORMAL, BMP_OTHER_ALARM, false);
	m_btnSpeedAlarm.SetImage(PAINT_STATUS_DISABLE, BMP_OTHER_ALARM, false);
	m_btnSpeedAlarm.SetImage(PAINT_STATUS_FOCUS, BMP_OTHER_ALARM_FOCUS, false);
	m_stcSpeedAlarm.Create(CTRLID_STATIC, x, y+size_btn, size_text, m_length[STATIC_HEIGHT], this, m_pStrTable->String(E_STR_ID_MENU_SPEED_ALARM));
	m_stcSpeedAlarm.SetAlignText(GUI::ALIGN_TEXT_CENTER);
#endif

	if((3 == col) && (x != base_x))
	{
		x = base_x;
		y += 120;
	}
	else
	{
		x += size_text;
		col++;
	}

	m_btnExit.Create(CTRLID_DEFAULT_BTN_CANCEL, x + offset_text, y, size_btn, size_btn, this, 1, 2, NULL, WND_CHILD | WND_VISIBLE | WND_NO_IMAGE_BK);
	m_btnExit.SetImage(PAINT_STATUS_NORMAL, BMP_BTN_EXIT3, false);
	m_btnExit.SetImage(PAINT_STATUS_DISABLE, BMP_BTN_EXIT3, false);
	m_btnExit.SetImage(PAINT_STATUS_FOCUS, BMP_BTN_EXIT3_FOCUS, false);
	m_stcExit.Create(CTRLID_STATIC, x, y+size_btn, size_text, m_length[STATIC_HEIGHT], this, m_pStrTable->String(E_STR_ID_EXIT));
	m_stcExit.SetAlignText(GUI::ALIGN_TEXT_CENTER);
#else

	unsigned short x = 80;
	unsigned short y = m_length[TITLE_HEIGHT] + m_length[DLG_OFFSET_TOP] + 25;
	unsigned short x_off = 60, y_off = 10;
	unsigned short cx = m_length[ANCHOR_WIDTH], cy = m_length[ANCHOR_HEIGHT];

	int yFocus = 1;
	int rowNum = 6;
// #ifndef __CUSTOM_SHANGHAI__
// 	if (CCfgDlg::s_loginSuccessReply.productInfo.localVideoInputNum == 0)
// 	{
// 		rowNum--;
// 	}
// #endif

	if(!IsSupportAlarmOut() && (g_p_login_succ->productInfo.netVideoInputNum == 0))
	{
		rowNum--;
	}

	if(rowNum <= 5)
	{
		x = 200;
		y = m_length[TITLE_HEIGHT] + m_length[DLG_OFFSET_TOP] + 25;
	}

	if (IsSupportAlarmOut() || (g_p_login_succ->productInfo.netVideoInputNum > 0))
	{
		m_anSensor.Create(GUI::CWnd::GetRandWndID(), x, y, cx, cy, this, 0, yFocus++, m_pStrTable->String(E_STR_ID_MENU_CFG_SENSOR));
		m_anSensor.SetImage(BMP_ANCHOR_SENSOR, BMP_ANCHOR_SENSOR_F, BMP_ANCHOR_SENSOR);
	
		y += 60;
	}

	m_anMotion.Create(GUI::CWnd::GetRandWndID(), x, y, cx, cy, this, 0, yFocus++, m_pStrTable->String(E_STR_ID_MENU_CFG_MOTION));
	m_anMotion.SetImage(BMP_ANCHOR_MOTION, BMP_ANCHOR_MOTION_F, BMP_ANCHOR_MOTION);

// #ifndef __CUSTOM_SHANGHAI__
// 	if (CCfgDlg::s_loginSuccessReply.productInfo.localVideoInputNum > 0)
// #endif
	{
		y += 60;
		m_anVideoLoss.Create(GUI::CWnd::GetRandWndID(), x, y, cx, cy, this, 0, yFocus++, m_pStrTable->String(E_STR_ID_MENU_CFG_VIDEO_LOSS));
		m_anVideoLoss.SetImage(BMP_ANCHOR_VIDEO_LOSS, BMP_ANCHOR_VIDEO_LOSS_F, BMP_ANCHOR_VIDEO_LOSS);
	}

	y += 60;
	m_anOtherAlarm.Create(GUI::CWnd::GetRandWndID(), x, y, cx, cy, this, 0, yFocus++, m_pStrTable->String(E_STR_ID_MENU_CFG_OTHER_ALARM));
	m_anOtherAlarm.SetImage(BMP_ANCHOR_OTHER_ALARM, BMP_ANCHOR_OTHER_ALARM_F, BMP_ANCHOR_OTHER_ALARM);

	y += 60;
	m_anAlarmOut.Create(GUI::CWnd::GetRandWndID(), x, y, cx, cy, this, 0, yFocus++, m_pStrTable->String(E_STR_ID_MENU_CFG_ALARM_OUT));
	m_anAlarmOut.SetImage(BMP_ANCHOR_ALARM_OUT, BMP_ANCHOR_ALARM_OUT_F, BMP_ANCHOR_ALARM_OUT);

	//µÚ¶þÁÐ
	if (rowNum > 5)
	{
		x = m_rect.Width() / 2 + 40;
		y = m_length[TITLE_HEIGHT] + m_length[DLG_OFFSET_TOP] + 25 - 60;
	}

#ifdef __MOBILE_DVR__
	y += 60;
	m_anSpeedAlarm.Create(GUI::CWnd::GetRandWndID(), x, y, cx, cy, this, 0, yFocus++, m_pStrTable->String(E_STR_ID_MENU_SPEED_ALARM));
	m_anSpeedAlarm.SetImage(BMP_ANCHOR_CONFIG, BMP_ANCHOR_CONFIG_F, BMP_ANCHOR_CONFIG);
#endif

	y += 60;
	m_anExit.Create(GUI::CWnd::GetRandWndID(), x, y, cx, cy, this, 0, yFocus++, m_pStrTable->String(E_STR_ID_EXIT));
	m_anExit.SetImage(BMP_ANCHOR_EXIT, BMP_ANCHOR_EXIT_F, BMP_ANCHOR_EXIT);
#endif
}

BEGIN_MSG_MAP(CCfgAlarmManDlg, GUI::CDialog)
#ifndef __DVR_ANCHOR__
	ON_MSG(m_btnSensor.GetID(), KEY_VIRTUAL_ENTER, OnSensor)
	ON_MSG(m_btnMotion.GetID(), KEY_VIRTUAL_ENTER, OnMotion)
	ON_MSG(m_btnVideoLoss.GetID(), KEY_VIRTUAL_ENTER, OnVideoLoss)
	ON_MSG(m_btnOtherAlarm.GetID(), KEY_VIRTUAL_ENTER, OnOtherAlarm)
	ON_MSG(m_btnAlarmOut.GetID(), KEY_VIRTUAL_ENTER, OnAlarmOut)
#ifdef __MOBILE_DVR__
	ON_MSG(m_btnSpeedAlarm.GetID(), KEY_VIRTUAL_ENTER, OnSpeedAlarm)
#endif
#else
	ON_MSG(m_anSensor.GetID(), KEY_VIRTUAL_ENTER, OnSensor)
	ON_MSG(m_anMotion.GetID(), KEY_VIRTUAL_ENTER, OnMotion)
	ON_MSG(m_anVideoLoss.GetID(), KEY_VIRTUAL_ENTER, OnVideoLoss)
	ON_MSG(m_anOtherAlarm.GetID(), KEY_VIRTUAL_ENTER, OnOtherAlarm)
	ON_MSG(m_anAlarmOut.GetID(), KEY_VIRTUAL_ENTER, OnAlarmOut)
#ifdef __MOBILE_DVR__
	ON_MSG(m_anSpeedAlarm.GetID(), KEY_VIRTUAL_ENTER, OnSpeedAlarm)
#endif
	ON_MSG(m_anExit.GetID(), KEY_VIRTUAL_ENTER, OnExit)
#endif
END_MSG_MAP()

unsigned long CCfgAlarmManDlg::OnSensor()
{
	m_sensorDlg.DoModal(this);
	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CCfgAlarmManDlg::OnMotion()
{
	m_motionManDlg.DoModal(this);
	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CCfgAlarmManDlg::OnVideoLoss()
{
	m_videoLossDlg.DoModal(this);
	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CCfgAlarmManDlg::OnOtherAlarm()
{
	m_otherAlarmDlg.DoModal(this);

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CCfgAlarmManDlg::OnAlarmOut()
{
	if (IsSupportAlarmOut())
	{
		m_alarmOutManDlg.DoModal(this);
	}
	else
	{
		//m_alarmOutBuzDlg.DoModal(this);
		m_alarmBuzzerDlg.DoModal(this);
	}
	return KEY_VIRTUAL_MSG_STOP;
}

#ifdef __MOBILE_DVR__

unsigned long CCfgAlarmManDlg::OnSpeedAlarm()
{
	m_SpeedAlarmDlg.DoModal(this);
	return KEY_VIRTUAL_MSG_STOP;
}

#endif

unsigned long CCfgAlarmManDlg::OnExit()
{
	EndDialog(DLG_ESC);
	return KEY_VIRTUAL_MSG_STOP;
}

//end
