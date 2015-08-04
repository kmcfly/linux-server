/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : ysw
** Date         : 2009-07-24
** Name         : ToolBar.cpp
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/
#include "ToolBar.h"
#include "Product.h"
#include "NetProtocol.h"
#include "FuncCustom.h"
#include "MessageMan.h"
#include "CfgDlg.h"

extern NET_LOGIN_SUCCESS_REPLY *g_p_login_succ;
//////////////////////////////////////////////////////////////////////////

GUI::CToolBar::CToolBar()
{
	memset(&m_posInfo, 0, sizeof(NCFG_INFO_TOOLBAR_POS));

	m_lenWndInfo = 0;
	memset(&m_btnWndInfo, 0, sizeof(BTN_WND_INFO) * 32);

	memset(&m_btnWndLayout, 0, sizeof(BTN_WND_LAYOUT));

}

GUI::CToolBar::~CToolBar()
{

}

void GUI::CToolBar::OnInitial()
{
	GUI::CDialog::OnInitial();
	//////////////////////////////////////////////////////////////////////////
	unsigned short x = 0, width = 36, height = 31, y = 5;
	unsigned short ex_width = 10, ex_height = 6;
	int xFocus = 0, xOff = 4;
	unsigned short tmp_width = 0;
	
	m_lenWndInfo = 0;
	memset(&m_btnWndInfo, 0, sizeof(BTN_WND_INFO) * 32);
	memset(&m_btnWndLayout, 0, sizeof(BTN_WND_LAYOUT));

	//int numChnn = g_p_login_succ->productInfo.videoInputNum;
	
	int numChnn = IsSupportHybridDisplay()?g_p_login_succ->productInfo.videoInputNum:g_p_login_succ->productInfo.localVideoInputNum;
	PRODUCT_TYPE productID = CProduct::Instance()->ProductType();
	//////////////////////////////////////////////////////////////////////////

	//主菜单
	m_btnMenu.Create(CTRLID_TOOL_BAR_SYSTEM, x, y, 60, height, this, xFocus++, 0);
	m_btnMenu.SetImage(PAINT_STATUS_NORMAL, BMP_BTN_TOOL_MENU, false);
	m_btnMenu.SetImage(PAINT_STATUS_FOCUS, BMP_BTN_TOOL_MENU_F, false);
	m_btnMenu.Show(false, false);
	
	m_btnWndInfo[m_lenWndInfo++].SetData(BTN_ITEM_MENU, &m_btnMenu, 60 + xOff, height, y);

	//1x1 分割
	if(numChnn >= 1)
	{
		m_btn1X1.Create(CTRLID_TOOL_BAR_1X1, x, y, width, height, this, xFocus++, 0);
		m_btn1X1.SetImage(PAINT_STATUS_NORMAL, BMP_BTN_TOOL_1X1, false);
		m_btn1X1.SetImage(PAINT_STATUS_FOCUS, BMP_BTN_TOOL_1X1_F, false);
		m_btn1X1.Show(false, false);

		tmp_width = (numChnn > 1) ? width : (width + xOff);
		m_btnWndInfo[m_lenWndInfo++].SetData(BTN_ITEM_SPLIT, &m_btn1X1, tmp_width, height, y);
	}

	if(numChnn > 1)
	{
		m_btn1X1Ex.Create(CTRLID_TOOL_BAR_1X1_EX, x, y, ex_width, ex_height, this, xFocus++, 0);
		m_btn1X1Ex.SetStateImage(SPLIT_UP, PAINT_STATUS_NORMAL, BMP_BTN_TOOL_TRIANGLE_UP);
		m_btn1X1Ex.SetStateImage(SPLIT_UP, PAINT_STATUS_FOCUS, BMP_BTN_TOOL_TRIANGLE_UP_F);
		m_btn1X1Ex.SetStateImage(SPLIT_DOWN, PAINT_STATUS_NORMAL, BMP_BTN_TOOL_TRIANGLE_DOWN);
		m_btn1X1Ex.SetStateImage(SPLIT_DOWN, PAINT_STATUS_FOCUS, BMP_BTN_TOOL_TRIANGLE_DOWN_F);
		m_btn1X1Ex.SetCurState(SPLIT_DOWN, false);
		m_btn1X1Ex.Show(false, false);
		
		m_btnWndInfo[m_lenWndInfo++].SetData(BTN_ITEM_SPLIT_EX, &m_btn1X1Ex, ex_width + xOff, ex_height, y);
	}

	//2x2分割
	if(numChnn >= 4)
	{
		m_btn2X2.Create(CTRLID_TOOL_BAR_2X2, x, y, width, height, this, xFocus++, 0);
		m_btn2X2.SetImage(PAINT_STATUS_NORMAL, BMP_BTN_TOOL_2X2, false);
		m_btn2X2.SetImage(PAINT_STATUS_FOCUS, BMP_BTN_TOOL_2X2_F, false);
		m_btn2X2.Show(false, false);

		tmp_width = (numChnn > 4) ? width : (width + xOff);
		m_btnWndInfo[m_lenWndInfo++].SetData(BTN_ITEM_SPLIT, &m_btn2X2, tmp_width, height, y);
	}

	if(numChnn > 4)
	{
		m_btn2X2Ex.Create(CTRLID_TOOL_BAR_2X2_EX, x, y, ex_width, ex_height, this, xFocus++, 0);
		m_btn2X2Ex.SetStateImage(SPLIT_UP, PAINT_STATUS_NORMAL, BMP_BTN_TOOL_TRIANGLE_UP);
		m_btn2X2Ex.SetStateImage(SPLIT_UP, PAINT_STATUS_FOCUS, BMP_BTN_TOOL_TRIANGLE_UP_F);
		m_btn2X2Ex.SetStateImage(SPLIT_DOWN, PAINT_STATUS_NORMAL, BMP_BTN_TOOL_TRIANGLE_DOWN);
		m_btn2X2Ex.SetStateImage(SPLIT_DOWN, PAINT_STATUS_FOCUS, BMP_BTN_TOOL_TRIANGLE_DOWN_F);
		m_btn2X2Ex.SetCurState(SPLIT_DOWN, false);
		m_btn2X2Ex.Show(false, false);

		m_btnWndInfo[m_lenWndInfo++].SetData(BTN_ITEM_SPLIT_EX, &m_btn2X2Ex, ex_width + xOff, ex_height, y);
	}

	//2x3分割
	if(numChnn >= 5)
	{
		m_btn2X3.Create(CTRLID_TOOL_BAR_2X3, x, y, width, height, this, xFocus++, 0);
		m_btn2X3.SetImage(PAINT_STATUS_NORMAL, BMP_BTN_TOOL_2X3, false);
		m_btn2X3.SetImage(PAINT_STATUS_FOCUS, BMP_BTN_TOOL_2X3_F, false);
		m_btn2X3.Show(false, false);

		tmp_width = (numChnn > 6) ? width : (width + xOff);
		m_btnWndInfo[m_lenWndInfo++].SetData(BTN_ITEM_SPLIT, &m_btn2X3, tmp_width, height, y);
	}

	if(numChnn > 6)
	{
		m_btn2X3Ex.Create(CTRLID_TOOL_BAR_2X3_EX, x, y, ex_width, ex_height, this, xFocus++, 0);
		m_btn2X3Ex.SetStateImage(SPLIT_UP, PAINT_STATUS_NORMAL, BMP_BTN_TOOL_TRIANGLE_UP);
		m_btn2X3Ex.SetStateImage(SPLIT_UP, PAINT_STATUS_FOCUS, BMP_BTN_TOOL_TRIANGLE_UP_F);
		m_btn2X3Ex.SetStateImage(SPLIT_DOWN, PAINT_STATUS_NORMAL, BMP_BTN_TOOL_TRIANGLE_DOWN);
		m_btn2X3Ex.SetStateImage(SPLIT_DOWN, PAINT_STATUS_FOCUS, BMP_BTN_TOOL_TRIANGLE_DOWN_F);
		m_btn2X3Ex.SetCurState(SPLIT_DOWN, false);
		m_btn2X3Ex.Show(false, false);

		m_btnWndInfo[m_lenWndInfo++].SetData(BTN_ITEM_SPLIT_EX, &m_btn2X3Ex, ex_width + xOff, ex_height, y);
	}

	//VIEW_SPLIT_1A5 3x3
	if(numChnn >= 6)
	{
		m_btn1A5.Create(CTRLID_TOOL_BAR_1A5, x, y, width, height, this, xFocus++, 0);
		m_btn1A5.SetImage(PAINT_STATUS_NORMAL, BMP_BTN_TOOL_1A5, false);
		m_btn1A5.SetImage(PAINT_STATUS_FOCUS, BMP_BTN_TOOL_1A5_F, false);
		m_btn1A5.Show(false, false);

		tmp_width = (numChnn > 6) ? width : (width + xOff);
		m_btnWndInfo[m_lenWndInfo++].SetData(BTN_ITEM_SPLIT, &m_btn1A5, tmp_width, height, y);
	}

	if(numChnn > 6)
	{
		m_btn1A5Ex.Create(CTRLID_TOOL_BAR_1A5_EX, x, y, ex_width, ex_height, this, xFocus++, 0);
		m_btn1A5Ex.SetStateImage(SPLIT_UP, PAINT_STATUS_NORMAL, BMP_BTN_TOOL_TRIANGLE_UP);
		m_btn1A5Ex.SetStateImage(SPLIT_UP, PAINT_STATUS_FOCUS, BMP_BTN_TOOL_TRIANGLE_UP_F);
		m_btn1A5Ex.SetStateImage(SPLIT_DOWN, PAINT_STATUS_NORMAL, BMP_BTN_TOOL_TRIANGLE_DOWN);
		m_btn1A5Ex.SetStateImage(SPLIT_DOWN, PAINT_STATUS_FOCUS, BMP_BTN_TOOL_TRIANGLE_DOWN_F);
		m_btn1A5Ex.SetCurState(SPLIT_DOWN, false);
		m_btn1A5Ex.Show(false, false);

		m_btnWndInfo[m_lenWndInfo++].SetData(BTN_ITEM_SPLIT_EX, &m_btn1A5Ex, ex_width + xOff, ex_height, y);
	}

	//VIEW_SPLIT_1A7 4x4
	if(numChnn >= 8)
	{
		m_btn1A7.Create(CTRLID_TOOL_BAR_1A7, x, y, width, height, this, xFocus++, 0);
		m_btn1A7.SetImage(PAINT_STATUS_NORMAL, BMP_BTN_TOOL_1A7, false);
		m_btn1A7.SetImage(PAINT_STATUS_FOCUS, BMP_BTN_TOOL_1A7_F, false);
		m_btn1A7.Show(false, false);

		tmp_width = (numChnn > 6) ? width : (width + xOff);
		m_btnWndInfo[m_lenWndInfo++].SetData(BTN_ITEM_SPLIT, &m_btn1A7, tmp_width, height, y);
	}

	if(numChnn > 8)
	{
		m_btn1A7Ex.Create(CTRLID_TOOL_BAR_1A7_EX, x, y, ex_width, ex_height, this, xFocus++, 0);
		m_btn1A7Ex.SetStateImage(SPLIT_UP, PAINT_STATUS_NORMAL, BMP_BTN_TOOL_TRIANGLE_UP);
		m_btn1A7Ex.SetStateImage(SPLIT_UP, PAINT_STATUS_FOCUS, BMP_BTN_TOOL_TRIANGLE_UP_F);
		m_btn1A7Ex.SetStateImage(SPLIT_DOWN, PAINT_STATUS_NORMAL, BMP_BTN_TOOL_TRIANGLE_DOWN);
		m_btn1A7Ex.SetStateImage(SPLIT_DOWN, PAINT_STATUS_FOCUS, BMP_BTN_TOOL_TRIANGLE_DOWN_F);
		m_btn1A7Ex.SetCurState(SPLIT_DOWN, false);
		m_btn1A7Ex.Show(false, false);

		m_btnWndInfo[m_lenWndInfo++].SetData(BTN_ITEM_SPLIT_EX, &m_btn1A7Ex, ex_width + xOff, ex_height, y);
	}

	//3x3分割
	if(numChnn >= 8 )
	{
		m_btn3X3.Create(CTRLID_TOOL_BAR_3X3, x, y, width, height, this, xFocus++, 0);
		m_btn3X3.SetImage(PAINT_STATUS_NORMAL, BMP_BTN_TOOL_3X3, false);
		m_btn3X3.SetImage(PAINT_STATUS_FOCUS, BMP_BTN_TOOL_3X3_F, false);
		m_btn3X3.Show(false, false);

		tmp_width = (numChnn > 8) ? width : (width + xOff);
		m_btnWndInfo[m_lenWndInfo++].SetData(BTN_ITEM_SPLIT, &m_btn3X3, tmp_width, height, y);
	}

	if(numChnn > 9)
	{
		m_btn3X3Ex.Create(CTRLID_TOOL_BAR_3X3_EX, x, y, ex_width, ex_height, this, xFocus++, 0);
		m_btn3X3Ex.SetStateImage(SPLIT_UP, PAINT_STATUS_NORMAL, BMP_BTN_TOOL_TRIANGLE_UP);
		m_btn3X3Ex.SetStateImage(SPLIT_UP, PAINT_STATUS_FOCUS, BMP_BTN_TOOL_TRIANGLE_UP_F);
		m_btn3X3Ex.SetStateImage(SPLIT_DOWN, PAINT_STATUS_NORMAL, BMP_BTN_TOOL_TRIANGLE_DOWN);
		m_btn3X3Ex.SetStateImage(SPLIT_DOWN, PAINT_STATUS_FOCUS, BMP_BTN_TOOL_TRIANGLE_DOWN_F);
		m_btn3X3Ex.SetCurState(SPLIT_DOWN, false);
		m_btn3X3Ex.Show(false, false);

		m_btnWndInfo[m_lenWndInfo++].SetData(BTN_ITEM_SPLIT_EX, &m_btn3X3Ex, ex_width + xOff, ex_height, y);
	}

	//VIEW_SPLIT_1A12 4x4
	if( (numChnn >= 13) && ((CProduct::Instance()->ProductType() != TD_2832NE_B) || (CProduct::Instance()->ProductType() != TD_2816NE_B)))
	{
		m_btn1A12.Create(CTRLID_TOOL_BAR_1A12, x, y, width, height, this, xFocus++, 0);
		m_btn1A12.SetImage(PAINT_STATUS_NORMAL, BMP_BTN_TOOL_1A12, false);
		m_btn1A12.SetImage(PAINT_STATUS_FOCUS, BMP_BTN_TOOL_1A12_F, false);
		m_btn1A12.Show(false, false);

		tmp_width = (numChnn > 6) ? width : (width + xOff);
		m_btnWndInfo[m_lenWndInfo++].SetData(BTN_ITEM_SPLIT, &m_btn1A12, tmp_width, height, y);
	}

	if( (numChnn > 13) && ((CProduct::Instance()->ProductType() != TD_2832NE_B) || (CProduct::Instance()->ProductType() != TD_2816NE_B)))
	{
		m_btn1A12Ex.Create(CTRLID_TOOL_BAR_1A12_EX, x, y, ex_width, ex_height, this, xFocus++, 0);
		m_btn1A12Ex.SetStateImage(SPLIT_UP, PAINT_STATUS_NORMAL, BMP_BTN_TOOL_TRIANGLE_UP);
		m_btn1A12Ex.SetStateImage(SPLIT_UP, PAINT_STATUS_FOCUS, BMP_BTN_TOOL_TRIANGLE_UP_F);
		m_btn1A12Ex.SetStateImage(SPLIT_DOWN, PAINT_STATUS_NORMAL, BMP_BTN_TOOL_TRIANGLE_DOWN);
		m_btn1A12Ex.SetStateImage(SPLIT_DOWN, PAINT_STATUS_FOCUS, BMP_BTN_TOOL_TRIANGLE_DOWN_F);
		m_btn1A12Ex.SetCurState(SPLIT_DOWN, false);
		m_btn1A12Ex.Show(false, false);

		m_btnWndInfo[m_lenWndInfo++].SetData(BTN_ITEM_SPLIT_EX, &m_btn1A12Ex, ex_width + xOff, ex_height, y);
	}

	//4x4分割
	if(numChnn >= 12)
	{
		m_btn4X4.Create(CTRLID_TOOL_BAR_4X4, x, y, width, height, this, xFocus++, 0);
		m_btn4X4.SetImage(PAINT_STATUS_NORMAL, BMP_BTN_TOOL_4X4, false);
		m_btn4X4.SetImage(PAINT_STATUS_FOCUS, BMP_BTN_TOOL_4X4_F, false);
		m_btn4X4.Show(false, false);

		tmp_width = (numChnn > 16) ? width : (width + xOff);
		m_btnWndInfo[m_lenWndInfo++].SetData(BTN_ITEM_SPLIT, &m_btn4X4, tmp_width, height, y);
	}

	if(numChnn > 16)
	{
		m_btn4X4Ex.Create(CTRLID_TOOL_BAR_4X4_EX, x, y, ex_width, ex_height, this, xFocus++, 0);
		m_btn4X4Ex.SetStateImage(SPLIT_UP, PAINT_STATUS_NORMAL, BMP_BTN_TOOL_TRIANGLE_UP);
		m_btn4X4Ex.SetStateImage(SPLIT_UP, PAINT_STATUS_FOCUS, BMP_BTN_TOOL_TRIANGLE_UP_F);
		m_btn4X4Ex.SetStateImage(SPLIT_DOWN, PAINT_STATUS_NORMAL, BMP_BTN_TOOL_TRIANGLE_DOWN);
		m_btn4X4Ex.SetStateImage(SPLIT_DOWN, PAINT_STATUS_FOCUS, BMP_BTN_TOOL_TRIANGLE_DOWN_F);
		m_btn4X4Ex.SetCurState(SPLIT_DOWN, false);
		m_btn4X4Ex.Show(false, false);

		m_btnWndInfo[m_lenWndInfo++].SetData(BTN_ITEM_SPLIT_EX, &m_btn4X4Ex, ex_width + xOff, ex_height, y);
	}

	//5x5分割
	if(numChnn >= 18)
	{
		m_btn5X5.Create(CTRLID_TOOL_BAR_5X5, x, y, width, height, this, xFocus++, 0);
		m_btn5X5.SetImage(PAINT_STATUS_NORMAL, BMP_BTN_TOOL_5X5, false);
		m_btn5X5.SetImage(PAINT_STATUS_FOCUS, BMP_BTN_TOOL_5X5_F, false);
		m_btn5X5.Show(false, false);

		tmp_width = (numChnn > 24) ? width : (width + xOff);
		m_btnWndInfo[m_lenWndInfo++].SetData(BTN_ITEM_SPLIT, &m_btn5X5, tmp_width, height, y);
	}

	if(numChnn > 24)
	{
		m_btn5X5Ex.Create(CTRLID_TOOL_BAR_5X5_EX, x, y, ex_width, ex_height, this, xFocus++, 0);
		m_btn5X5Ex.SetStateImage(SPLIT_UP, PAINT_STATUS_NORMAL, BMP_BTN_TOOL_TRIANGLE_UP);
		m_btn5X5Ex.SetStateImage(SPLIT_UP, PAINT_STATUS_FOCUS, BMP_BTN_TOOL_TRIANGLE_UP_F);
		m_btn5X5Ex.SetStateImage(SPLIT_DOWN, PAINT_STATUS_NORMAL, BMP_BTN_TOOL_TRIANGLE_DOWN);
		m_btn5X5Ex.SetStateImage(SPLIT_DOWN, PAINT_STATUS_FOCUS, BMP_BTN_TOOL_TRIANGLE_DOWN_F);
		m_btn5X5Ex.SetCurState(SPLIT_DOWN, false);
		m_btn5X5Ex.Show(false, false);

		m_btnWndInfo[m_lenWndInfo++].SetData(BTN_ITEM_SPLIT_EX, &m_btn5X5Ex, ex_width + xOff, ex_height, y);
	}

	//6x6分割
	if(numChnn >= 32)
	{
		m_btn6X6.Create(CTRLID_TOOL_BAR_6X6, x, y, width, height, this, xFocus++, 0);
		m_btn6X6.SetImage(PAINT_STATUS_NORMAL, BMP_BTN_TOOL_6X6, false);
		m_btn6X6.SetImage(PAINT_STATUS_FOCUS, BMP_BTN_TOOL_6X6_F, false);
		m_btn6X6.Show(false, false);

		m_btnWndInfo[m_lenWndInfo++].SetData(BTN_ITEM_SPLIT, &m_btn6X6, width + xOff, height, y);
	}

	//画中画
	if (IsSupportPip())
	{
		m_btnPip.Create(CTRLID_TOOL_BAR_PIP, x, y, width, height, this, xFocus++, 0);
		m_btnPip.SetImage(PAINT_STATUS_NORMAL, BMP_BTN_TOOL_PIP, false);
		m_btnPip.SetImage(PAINT_STATUS_FOCUS, BMP_BTN_TOOL_PIP_F, false);
		m_btnPip.Show(false, false);
		m_btnWndInfo[m_lenWndInfo++].SetData(BTN_ITEM_FUNC, &m_btnPip, width + xOff, height, y);
	}

#ifdef __DVR_BASIC__
	//跳台
	m_btnDwell.Create(CTRLID_TOOL_BAR_DWELL, x, y, width, height, this, xFocus++, 0);
	m_btnDwell.SetStateImage(DWELL_OPEN, PAINT_STATUS_NORMAL, BMP_BTN_TOOL_DWELL);
	m_btnDwell.SetStateImage(DWELL_OPEN, PAINT_STATUS_FOCUS, BMP_BTN_TOOL_DWELL_F);
	m_btnDwell.SetStateImage(DWELL_CLOSE, PAINT_STATUS_NORMAL, BMP_BTN_TOOL_DWELL_NO);
	m_btnDwell.SetStateImage(DWELL_CLOSE, PAINT_STATUS_FOCUS, BMP_BTN_TOOL_DWELL_NO_F);
	m_btnDwell.SetCurState(DWELL_CLOSE, false);
	m_btnDwell.Show(false, false);

	m_btnWndInfo[m_lenWndInfo++].SetData(BTN_ITEM_FUNC, &m_btnDwell, width + xOff, height, y);
#endif

	//调节色彩
	if(IsSupportChangeToolBarColor())
	{
		m_btnChangeColor.Create(CTRLID_TOOL_BAR_CHANGE_COLOR, x, y, width, height, this, xFocus++, 0);
		m_btnChangeColor.SetImage(PAINT_STATUS_NORMAL, BMP_BTN_TOOL_SET_COLOR,false);
		m_btnChangeColor.SetImage(PAINT_STATUS_FOCUS, BMP_BTN_TOOL_SET_COLOR_F, false);
		m_btnChangeColor.Show(false, false);

		m_btnWndInfo[m_lenWndInfo++].SetData(BTN_ITEM_FUNC, &m_btnChangeColor, width + xOff, height, y);
	}

	//摄像机参数设置
	if (g_p_login_succ->productInfo.netVideoInputNum > 0)
	{
		m_btnCamPara.Create(CTRLID_TOOL_BAR_CAM_PARA, x, y, width, height, this, xFocus++, 0);
		m_btnCamPara.SetImage(PAINT_STATUS_NORMAL, BMP_BTN_TOOL_SET_CAM_PARA,false);
		m_btnCamPara.SetImage(PAINT_STATUS_FOCUS, BMP_BTN_TOOL_SET_CAM_PARA_F, false);
		m_btnCamPara.Show(false,false);
		m_btnWndInfo[m_lenWndInfo++].SetData(BTN_ITEM_FUNC, &m_btnCamPara, width + xOff, height, y);	
	}
	
	//电子放大
	m_btnZoom.Create(CTRLID_TOOL_BAR_ZOOM, x, y, width, height, this, xFocus++, 0);
	m_btnZoom.SetImage(PAINT_STATUS_NORMAL, BMP_BTN_TOOL_ZOOM, false);
	m_btnZoom.SetImage(PAINT_STATUS_FOCUS, BMP_BTN_TOOL_ZOOM_F, false);
	m_btnZoom.Show(false, false);
	
	m_btnWndInfo[m_lenWndInfo++].SetData(BTN_ITEM_FUNC, &m_btnZoom, width + xOff, height, y);

	//音频
	if ((0 < g_p_login_succ->productInfo.audioInputNum) || (0 < g_p_login_succ->productInfo.netVideoInputNum))
	{
		m_btnAudio.Create(CTRLID_TOOL_BAR_AUDIO, x, y, width, height, this, xFocus++, 0);
		m_btnAudio.SetStateImage(AUDIO_OPEN, PAINT_STATUS_NORMAL, BMP_BTN_TOOL_AUDIO);
		m_btnAudio.SetStateImage(AUDIO_OPEN, PAINT_STATUS_FOCUS, BMP_BTN_TOOL_AUDIO_F);
		m_btnAudio.SetStateImage(AUDIO_MUTE, PAINT_STATUS_NORMAL, BMP_BTN_TOOL_AUDIO_MUTE);
		m_btnAudio.SetStateImage(AUDIO_MUTE, PAINT_STATUS_FOCUS, BMP_BTN_TOOL_AUDIO_MUTE_F);
		m_btnAudio.SetCurState(AUDIO_MUTE, false);
		m_btnAudio.Show(false, false);

		m_btnWndInfo[m_lenWndInfo++].SetData(BTN_ITEM_FUNC, &m_btnAudio, width + xOff, height, y);
	}

#if !defined(__TW01_RILI__)	
	//PTZ
	if(IsSupportPTZ() /*&& (g_p_login_succ->productInfo.localVideoInputNum > 0)*/)
	{
		m_btnPTZ.Create(CTRLID_TOOL_BAR_PTZ, x, y, width, height, this, xFocus++, 0);
		m_btnPTZ.SetImage(PAINT_STATUS_NORMAL, BMP_BTN_TOOL_PTZ, false);
		m_btnPTZ.SetImage(PAINT_STATUS_FOCUS, BMP_BTN_TOOL_PTZ_F, false);
		m_btnPTZ.Show(false, false);

		m_btnWndInfo[m_lenWndInfo++].SetData(BTN_ITEM_FUNC, &m_btnPTZ, width + xOff, height, y);
	}
#endif

	//抓图
	if (IsSupportSnapPic()/* && (CProduct::Instance()->LocalVideoInputNum() > 0)*/)
	{
		m_btnSnap.Create(CTRLID_TOOL_BAR_SNAP, x, y, width, height, this, xFocus++, 0);
		m_btnSnap.SetImage(PAINT_STATUS_NORMAL, BMP_BTN_TOOL_SNAP, false);
		m_btnSnap.SetImage(PAINT_STATUS_FOCUS, BMP_BTN_TOOL_SNAP_F, false);
		m_btnSnap.Show(false, false);

		m_btnWndInfo[m_lenWndInfo++].SetData(BTN_ITEM_FUNC, &m_btnSnap, width + xOff, height, y);
	}
	
	//录像
	m_btnRec.Create(CTRLID_TOOL_BAR_REC, x, y, width, height, this, xFocus++, 0);
	m_btnRec.SetImage(PAINT_STATUS_NORMAL, BMP_BTN_TOOL_REC, false);
	m_btnRec.SetImage(PAINT_STATUS_FOCUS, BMP_BTN_TOOL_REC_F, false);
	m_btnRec.Show(false, false);

	m_btnWndInfo[m_lenWndInfo++].SetData(BTN_ITEM_FUNC, &m_btnRec, width + xOff, height, y);

	//回放
	m_btnPlay.Create(CTRLID_TOOL_BAR_PLAY, x, y, width, height, this, xFocus++, 0);
	m_btnPlay.SetImage(PAINT_STATUS_NORMAL, BMP_BTN_TOOL_PLAY, false);
	m_btnPlay.SetImage(PAINT_STATUS_FOCUS, BMP_BTN_TOOL_PLAY_F, false);
	m_btnPlay.Show(false, false);

	m_btnWndInfo[m_lenWndInfo++].SetData(BTN_ITEM_FUNC, &m_btnPlay, width + xOff, height, y);

	m_btnPlayEx.Create(CTRLID_TOOL_BAR_PLAY_EX, x, y, ex_width, ex_height, this, xFocus++, 0);
	m_btnPlayEx.SetStateImage(SPLIT_UP, PAINT_STATUS_NORMAL, BMP_BTN_TOOL_TRIANGLE_UP);
	m_btnPlayEx.SetStateImage(SPLIT_UP, PAINT_STATUS_FOCUS, BMP_BTN_TOOL_TRIANGLE_UP_F);
	m_btnPlayEx.SetStateImage(SPLIT_DOWN, PAINT_STATUS_NORMAL, BMP_BTN_TOOL_TRIANGLE_DOWN);
	m_btnPlayEx.SetStateImage(SPLIT_DOWN, PAINT_STATUS_FOCUS, BMP_BTN_TOOL_TRIANGLE_DOWN_F);
	m_btnPlayEx.SetCurState(SPLIT_DOWN, false);
	m_btnPlayEx.Show(false, false);

	m_btnWndInfo[m_lenWndInfo++].SetData(BTN_ITEM_FUNC_EX, &m_btnPlayEx, ex_width + xOff, ex_height, y);

#if defined(__DVD_BURN_REC__) && defined (__ENVIRONMENT_LINUX__)
	//刻录
	m_btnBurn.Create(CTRLID_TOOL_BAR_BURN, x, y, width, height, this, xFocus++, 0);
	m_btnBurn.SetImage(PAINT_STATUS_NORMAL, BMP_BTN_TOOL_REC, false);
	m_btnBurn.SetImage(PAINT_STATUS_FOCUS, BMP_BTN_TOOL_REC_F, false);
	m_btnBurn.Show(false, false);

	m_btnWndInfo[m_lenWndInfo++].SetData(BTN_ITEM_FUNC, &m_btnBurn, width + xOff, height, y);

	//刻录通道选择
	m_btnBurnEx.Create(CTRLID_TOOL_BAR_BURN_EX, x, y, ex_width, ex_height, this, xFocus++, 0);
	m_btnBurnEx.SetStateImage(SPLIT_UP, PAINT_STATUS_NORMAL, BMP_BTN_TOOL_TRIANGLE_UP);
	m_btnBurnEx.SetStateImage(SPLIT_UP, PAINT_STATUS_FOCUS, BMP_BTN_TOOL_TRIANGLE_UP_F);
	m_btnBurnEx.SetStateImage(SPLIT_DOWN, PAINT_STATUS_NORMAL, BMP_BTN_TOOL_TRIANGLE_DOWN);
	m_btnBurnEx.SetStateImage(SPLIT_DOWN, PAINT_STATUS_FOCUS, BMP_BTN_TOOL_TRIANGLE_DOWN_F);
	m_btnBurnEx.SetCurState(SPLIT_DOWN, false);
	m_btnBurnEx.Show(false, false);

	m_btnWndInfo[m_lenWndInfo++].SetData(BTN_ITEM_FUNC_EX, &m_btnBurnEx, ex_width + xOff, ex_height, y);
#endif
	//系统信息
#if defined(__CUSTOM_BR27__)
	m_btnSysInfo.Create(CTRLID_TOOL_BAR_SYSINFO,x,y,width,height,this, xFocus++, 0);
	m_btnSysInfo.SetImage(PAINT_STATUS_NORMAL, BMP_BTN_TOOL_SYSINFO, false);
	m_btnSysInfo.SetImage(PAINT_STATUS_FOCUS, BMP_BTN_TOOL_SYSINFO_F, false);
	m_btnSysInfo.Show(false, false);

	m_btnWndInfo[m_lenWndInfo++].SetData(BTN_ITEM_FUNC, &m_btnSysInfo, width + xOff, height, y);

	//工具条状态
	m_btnStateToolBar.Create(CTRLID_TOOL_BAR_STATUS, x, y, width, height, this, xFocus++, 0);
	m_btnStateToolBar.SetImage(PAINT_STATUS_NORMAL, BMP_BTN_TOOL_WINEDIT, false);
	m_btnStateToolBar.SetImage(PAINT_STATUS_FOCUS, BMP_BTN_TOOL_WINEDIT_F, false);
	m_btnStateToolBar.Show(false, false);

	m_btnWndInfo[m_lenWndInfo++].SetData(BTN_ITEM_STATE, &m_btnStateToolBar, width + xOff, height, y);

#else
	//工具条状态
	m_btnStateToolBar.Create(CTRLID_TOOL_BAR_STATUS, x, y, width, height, this, xFocus++, 0);
	m_btnStateToolBar.SetImage(PAINT_STATUS_NORMAL, BMP_BTN_TOOL_EDIT, false);
	m_btnStateToolBar.SetImage(PAINT_STATUS_FOCUS, BMP_BTN_TOOL_EDIT_F, false);
	m_btnStateToolBar.Show(false, false);

	m_btnWndInfo[m_lenWndInfo++].SetData(BTN_ITEM_STATE, &m_btnStateToolBar, width + xOff, height, y);
#endif
	//计算大小
	for(int i = 0; i < m_lenWndInfo; i++)
	{
		if(NULL != m_btnWndInfo[i].pWnd)
		{
			m_btnWndLayout.max_width += m_btnWndInfo[i].width;

			if(m_btnWndInfo[i].type <= BTN_ITEM_SPLIT_EX)
			{
				m_btnWndLayout.split_width += m_btnWndInfo[i].width;
			}
			else
			{
				m_btnWndLayout.func_width += m_btnWndInfo[i].width;
			}
		}
	}

	m_btnWndLayout.split_width += 2; //多预留少许间距
	m_btnWndLayout.func_width += 2;
	m_btnWndLayout.max_width += 4;

	//背景图大小
	CWnd::GetImageSize(BMP_DLG_TOOLBAR, m_bmpBgWidth, m_bmpBgHeight);

	//

	m_x_enter = 0;
	m_y_enter = 0;
	m_x_drag_last = 0;
	m_y_drag_last = 0;

	m_bDrag = false;
	m_bFirstDrag = false;
	m_bShowToolBar = true;

	m_x_pos = 0;
	m_y_pos = 0;
	m_x_pos_last = 0;
	m_y_pos_last = 0;
	m_count = 0;
	m_wait = 0;

	m_top_offset = y + height / 2;
	m_right_offset = width / 2 + 5;

	if(m_width < 800)
	{
		m_b_cvbs = true;
		m_tool_show_rect.SetRect(40, 40, m_width - 40, m_height - 40);
	}
	else
	{
		m_b_cvbs = false;
		m_tool_show_rect.SetRect(0, 0, m_width - 1, m_height - 1);
	}
}

void GUI::CToolBar::OnDestroy()
{
	GUI::CDialog::OnDestroy();
}

void GUI::CToolBar::OnPaint(PAINT_STATUS status)
{
	CDrawManager drawMan;
	const unsigned char* pData ;
	pData = drawMan.Stretch(BMP_DLG_TOOLBAR, m_rect.Width(), m_rect.Height(), m_bmpBgWidth / 2, m_bmpBgHeight / 2, NULL, true);

	int len = drawMan.GetDataLen();

	if (NULL != pData)
	{
		GUI::CWnd::DrawImage(pData, len, m_rect.m_left, m_rect.m_top, m_rect.Width(), m_rect.Width(), m_rect.Height());
	}
}

void GUI::CToolBar::OnIdle()
{
	if(m_bDrag)
	{
		return;
	}

	if(m_wait > 0)
	{
		m_wait--;
	}

	bool bShow = IsShow();

	if(bShow)
	{
		if(PtInToolbar(m_x_pos, m_y_pos))
		{
			m_count = 0;
		}
		else
		{
			m_count++;
		}

		if(m_count >= m_posInfo.postDelayTime)
		{
			GUI::CDialog::Show(false, true);

			CMessageMan::Instance()->RefreshLiveTrue();
		}
	}
	else
	{
		if(PtInToolbar(m_x_pos, m_y_pos) && !IsAlmostAbove(m_x_pos, m_y_pos, m_x_pos_last, m_y_pos_last, 200))
		{
			m_count++;
		}
		else
		{
			m_count = 0;
		}

		if(m_bShowToolBar && (m_count >= m_posInfo.preDelayTime) && (m_wait <= 0))
		{
			GUI::CDialog::Show(true, true);

			CMessageMan::Instance()->RefreshLiveTrue();
		}
	}

}

GUI::CRect GUI::CToolBar::GetSubBtnWndRect(GUI::CTRLID id)
{
	GUI::CRect rect;
	switch ( id)
	{
	case CTRLID_TOOL_BAR_AUDIO:
		{
			if ((0 < g_p_login_succ->productInfo.audioInputNum) || ((0 == g_p_login_succ->productInfo.localVideoInputNum) &&(0 < g_p_login_succ->productInfo.netVideoInputNum)))
			{
				rect = m_btnAudio.GetWindowRect();
			}
		}
		
		break;
	case CTRLID_TOOL_BAR_1X1:
		rect = m_btn1X1.GetWindowRect();
		break;
	case CTRLID_TOOL_BAR_2X2:
		rect = m_btn2X2.GetWindowRect();
		break;
	case CTRLID_TOOL_BAR_2X3:
		rect = m_btn2X3.GetWindowRect();
		break;
	case CTRLID_TOOL_BAR_1A5:
		rect = m_btn1A5.GetWindowRect();
		break;
	case CTRLID_TOOL_BAR_3X3:
		rect = m_btn3X3.GetWindowRect();
		break;
	case CTRLID_TOOL_BAR_1A7:
		rect = m_btn1A7.GetWindowRect();
		break;
	case CTRLID_TOOL_BAR_1A12:
		rect = m_btn1A12.GetWindowRect();
		break;
	case CTRLID_TOOL_BAR_4X4:
		rect = m_btn4X4.GetWindowRect();
		break;
	case CTRLID_TOOL_BAR_5X5:
		rect = m_btn5X5.GetWindowRect();
		break;
	case CTRLID_TOOL_BAR_6X6:
		rect = m_btn6X6.GetWindowRect();
		break;
	case CTRLID_TOOL_BAR_SNAP:
		rect = m_btnSnap.GetWindowRect();
		break;
	case CTRLID_TOOL_BAR_PLAY:
		rect = m_btnPlay.GetWindowRect();
		break;
	case CTRLID_TOOL_BAR_PIP:
		rect = m_btnPip.GetWindowRect();
		break;
	case CTRLID_TOOL_BAR_BURN:
		rect = m_btnBurn.GetWindowRect();
		break;
	case CTRLID_TOOL_BAR_SYSINFO:
		rect = m_btnSysInfo.GetWindowRect();
		break;
	default:
		assert(false);
		break;
	}

	return rect;
}

void GUI::CToolBar::EnableDwell(bool bOpen, bool bRefresh/* = true*/)
{
#ifdef __DVR_BASIC__
	if (bOpen)
	{
		m_btnDwell.SetCurState(DWELL_OPEN, bRefresh);
	}
	else
	{
		m_btnDwell.SetCurState(DWELL_CLOSE, bRefresh);
	}
#endif
}

void GUI::CToolBar::EnableAudio(bool bOpen, bool bRefresh/* = true*/)
{
	if ((0 < g_p_login_succ->productInfo.audioInputNum) || (g_p_login_succ->productInfo.netVideoInputNum > 0))
	{
		if (bOpen)
		{
			m_btnAudio.SetCurState(AUDIO_OPEN, bRefresh);
		}
		else
		{
			m_btnAudio.SetCurState(AUDIO_MUTE, bRefresh);
		}
	}
}

bool GUI::CToolBar::GetCfgPos(NCFG_INFO_TOOLBAR_POS& posInfo)
{
	memcpy(&posInfo, &m_posInfo, sizeof(NCFG_INFO_TOOLBAR_POS));
	
	return true;
}

bool GUI::CToolBar::PtInToolbar(int xPos, int yPos)
{
	return m_btnWndLayout.rect.PtInRect(xPos, yPos);
}

GUI::CRect GUI::CToolBar::AutoLayout(NCFG_INFO_TOOLBAR_POS* pPosInfo, bool bRefresh, unsigned long xPos, unsigned long yPos)
{
	if(pPosInfo)
	{
		memcpy(&m_posInfo, pPosInfo, sizeof(NCFG_INFO_TOOLBAR_POS));

		m_btnWndLayout.style = m_posInfo.style;
	}

	int require_width = 0, require_height = 0;

	//分多行
	if(m_btnWndLayout.max_width + 80 > m_tool_show_rect.Width())
	{
		printf("AutoLayout 654\n");
		m_btnWndLayout.num_row = 2;

		require_width = (m_btnWndLayout.func_width > m_btnWndLayout.split_width) ? m_btnWndLayout.func_width : m_btnWndLayout.split_width;
		require_width += m_length[BTN_WIDTH]/2;

		require_height = 74;
	}
	else
	{
		m_btnWndLayout.num_row = 1;

		require_width = m_btnWndLayout.max_width + 5;
		require_height = 40;
	}

	//计算位置
	switch(m_btnWndLayout.style)
	{
	case TOOL_BOTTOM:
		{
			if(!m_b_cvbs)
			{
				m_btnWndLayout.rect.SetRect(0, m_height - require_height - 1, m_width - 1, m_height - 1);
			}
			else
			{
				int x_cvbs = m_tool_show_rect.m_left + (m_tool_show_rect.Width() - require_width) / 2;
				int y_cvbs = m_tool_show_rect.m_bottom - require_height;

				m_btnWndLayout.rect.SetRect(x_cvbs, y_cvbs, x_cvbs + require_width - 1, y_cvbs + require_height - 1);
			}
		}
		break;
	case TOOL_TOP:
		{
			if(!m_b_cvbs)
			{
				m_btnWndLayout.rect.SetRect(0, 0, m_width - 1, require_height);
			}
			else
			{
				int x_cvbs = m_tool_show_rect.m_left + (m_tool_show_rect.Width() - require_width) / 2;
				int y_cvbs = m_tool_show_rect.m_top;

				m_btnWndLayout.rect.SetRect(x_cvbs, y_cvbs, x_cvbs + require_width - 1, y_cvbs + require_height - 1);
			}
		}
		break;
	case TOOL_PT:
		{
			int pt_x = 0, pt_y = 0;

			if(	(xPos > m_tool_show_rect.m_right) || (yPos > m_tool_show_rect.m_bottom))
			{
				if(m_posInfo.xPos > 100)
				{
					//居中
					pt_x = m_tool_show_rect.m_left + (m_tool_show_rect.Width() - require_width) / 2;
				}
				else if(m_posInfo.xPos == 100)
				{
					//居右
					pt_x = m_tool_show_rect.m_right - require_width - 1;
				}
				else
				{
					//0，局左
					pt_x = m_tool_show_rect.m_left + m_tool_show_rect.Width() * m_posInfo.xPos / 100;
				}

				if(m_posInfo.yPos >= 100)
				{
					//居底
					pt_y = m_tool_show_rect.m_bottom - require_height - 1;
				}
				else
				{
					pt_y = m_tool_show_rect.m_top + m_tool_show_rect.Height() * m_posInfo.yPos / 100;
				}
			}
			else
			{
				pt_x = xPos + m_right_offset - require_width;
				pt_y = yPos - m_top_offset;
			}

			if(pt_x <= m_tool_show_rect.m_left)
			{
				pt_x = m_tool_show_rect.m_left;
			}

			if(pt_x + require_width >= m_tool_show_rect.m_right)
			{
				pt_x = m_tool_show_rect.m_right - require_width - 1;
			}

			if(pt_y <= m_tool_show_rect.m_top)
			{
				pt_y = m_tool_show_rect.m_top;
			}

			if(pt_y + require_height >= m_tool_show_rect.m_bottom)
			{
				pt_y = m_tool_show_rect.m_bottom - require_height - 1;
			}

			m_btnWndLayout.rect.SetRect(pt_x, pt_y, pt_x + require_width, pt_y + require_height);
	
		}
		break;
	default:
		{
			//m_btnWndLayout.rect.SetRect(0, m_height - require_height, m_width, m_height);

			if(!m_b_cvbs)
			{
				m_btnWndLayout.rect.SetRect(0, m_height - require_height - 1, m_width - 1, m_height - 1);
			}
			else
			{
				int x_cvbs = m_tool_show_rect.m_left + (m_tool_show_rect.Width() - require_width) / 2;
				int y_cvbs = m_tool_show_rect.m_bottom - require_height;

				m_btnWndLayout.rect.SetRect(x_cvbs, y_cvbs, x_cvbs + require_width - 1, y_cvbs + require_height - 1);
			}
			printf("(%s.%d)error toolbar style = %d.\n", __FILE__, __LINE__, m_btnWndLayout.style);
		}
		break;
	}

	//重置窗口位置
	GUI::CWnd::SetWndPos(m_btnWndLayout.rect.m_left, m_btnWndLayout.rect.m_top, m_btnWndLayout.rect.Width(), m_btnWndLayout.rect.Height());

	//布局子窗口
#if defined(__CUSTOM_BR27__)
	if((TOOL_BOTTOM == m_btnWndLayout.style)||(TOOL_TOP == m_btnWndLayout.style))
	{
		OnBottomLayout();
	}
	else
	{
		LayoutSubWnd();
	}
#else
	LayoutSubWnd();
#endif
	

	return m_btnWndLayout.rect;
}

bool GUI::CToolBar::MoveToolWnd(int x, int y)
{
	GUI::CRect rect = m_btnWndLayout.rect;

	if(TOOL_TOP == m_btnWndLayout.style)
	{
		//顶部
		if(y - m_top_offset <= (10 + m_tool_show_rect.m_top))
		{
			return false;
		}

		m_btnWndLayout.style = TOOL_PT;
		AutoLayout(NULL, false, x, y);

		CalculatePosInfo(m_posInfo, m_btnWndLayout.rect, 0, 0, true);
	}
	else if(TOOL_PT == m_btnWndLayout.style)
	{
		//中间
		int pos_x = x + m_right_offset - rect.Width();
		int pos_y = y - m_top_offset;

		if(pos_x <= m_tool_show_rect.m_left)
		{
			pos_x = m_tool_show_rect.m_left;
		}

		if(pos_x + rect.Width() >= m_tool_show_rect.m_right)
		{
			pos_x = m_tool_show_rect.m_right - rect.Width();
		}

		if(pos_y <= (m_tool_show_rect.m_top + 10))
		{
			m_btnWndLayout.style = TOOL_TOP;
			AutoLayout(NULL, false);
		}
		else if(pos_y + rect.Height() + 10 >= m_tool_show_rect.m_bottom)
		{
			m_btnWndLayout.style = TOOL_BOTTOM;
			AutoLayout(NULL, false);
		}
		else
		{
			rect.MoveToXY(pos_x, pos_y);
			m_btnWndLayout.rect = rect;
			GUI::CWnd::SetWndPos(m_btnWndLayout.rect.m_left, m_btnWndLayout.rect.m_top, m_btnWndLayout.rect.Width(), m_btnWndLayout.rect.Height());

			CalculatePosInfo(m_posInfo, rect, 0, 0, true);

			LayoutSubWnd();
		}
	}
	else
	{
		//底部
		if(y - m_top_offset + m_btnWndLayout.rect.Height() + 10 >= m_tool_show_rect.m_bottom)
		{
			return false;
		}

		m_btnWndLayout.style = TOOL_PT;
		AutoLayout(NULL, false, x, y);

		CalculatePosInfo(m_posInfo, m_btnWndLayout.rect, 0, 0, true);
	}

	m_posInfo.style = m_btnWndLayout.style;

	return true;
}

void GUI::CToolBar::LayoutSubWnd()
{
	//计算各个按钮位置
	unsigned short tool_width = m_btnWndLayout.rect.Width(), tool_height = m_btnWndLayout.rect.Height();
	unsigned short x_left = m_btnWndLayout.rect.m_left, y_top = m_btnWndLayout.rect.m_top;
	unsigned short xOff = 2;

	unsigned short x = x_left + tool_width - 3;
	
	unsigned short y = y_top + 5;
	unsigned short ex_yoff = 5;
	unsigned short tmp_x = 0, tmp_y = 0;
	int last_type = BTN_ITEM_STATE;
	for(int i = m_lenWndInfo - 1; i > 0; i--)
	{
		if(NULL != m_btnWndInfo[i].pWnd)
		{
			x -= m_btnWndInfo[i].width;

			tmp_y = (m_btnWndInfo[i].type == BTN_ITEM_SPLIT_EX || m_btnWndInfo[i].type == BTN_ITEM_FUNC_EX) ? (y + ex_yoff) : y;

			if(m_btnWndLayout.num_row > 1)
			{
				tmp_y = ((m_btnWndInfo[i].type > BTN_ITEM_SPLIT_EX) && (CTRLID_TOOL_BAR_PIP != m_btnWndInfo[i].pWnd->GetID())) ? tmp_y : (tmp_y + 34);

				//在CVBS下，支持画中画的机器从画中画按钮开始分行，不支持的从画中画的下一个按钮(分割模式)开始分行
				if(last_type > BTN_ITEM_SPLIT_EX)
				{
					if ((m_btnWndInfo[i].pWnd->GetID() == CTRLID_TOOL_BAR_PIP) 
						|| ((m_btnWndInfo[i].type <= BTN_ITEM_SPLIT_EX) && (m_btnWndInfo[i + 1].pWnd->GetID() != CTRLID_TOOL_BAR_PIP)))
					{
						x = x_left + tool_width - 3 - m_btnWndInfo[i].width;
					}
				}
			}
			
			if (CTRLID_TOOL_BAR_SYSINFO == m_btnWndInfo[i].pWnd->GetID()) 
			{
				m_btnWndInfo[i].pWnd->MoveWnd(x + 15, tmp_y);
			}
			else if(CTRLID_TOOL_BAR_STATUS == m_btnWndInfo[i].pWnd->GetID())
			{
				m_btnWndInfo[i].pWnd->MoveWnd(x + xOff*2, tmp_y);
			}
			else
			{
				m_btnWndInfo[i].pWnd->MoveWnd(x + xOff, tmp_y);
			}
			
			if(!m_btnWndInfo[i].pWnd->IsShow())
			{
				m_btnWndInfo[i].pWnd->Show(true, false);
			}

			last_type = m_btnWndInfo[i].type;
		}
	}

	tmp_x = x_left + 3;
	tmp_y = (m_btnWndLayout.num_row > 1) ? (y + 34) : y;

	m_btnWndInfo[0].pWnd->MoveWnd(tmp_x + xOff, tmp_y);
	if(!m_btnWndInfo[0].pWnd->IsShow())
	{
		m_btnWndInfo[0].pWnd->Show(true, false);
	}
}

void GUI::CToolBar::OnBottomLayout()
{
	if(m_width <= 800)
	{
		//计算各个按钮位置
		unsigned short tool_width = m_btnWndLayout.rect.Width(), tool_height = m_btnWndLayout.rect.Height();
		unsigned short x_left = m_btnWndLayout.rect.m_left, y_top = m_btnWndLayout.rect.m_top;
		unsigned short xOff = 2;

		unsigned short x = (x_left + tool_width - 3)/2;
		int toolLen = 0;
		for(int i = 1; i < m_lenWndInfo; i++)
		{
			if (CTRLID_TOOL_BAR_SYSINFO == m_btnWndInfo[i].pWnd->GetID())
			{
				break;
			}
			toolLen += m_btnWndInfo[i].width;//除去最后2个按钮的工具条长度
		}
		int startx= x- toolLen/4;
		x = startx;
		
		unsigned short y = y_top + 5;
		unsigned short ex_yoff = 5;
		unsigned short tmp_x = 0, tmp_y = 0;
		int last_type = BTN_ITEM_STATE;

		for(int i = 1; i < m_lenWndInfo-1; i++)
		{
			if(NULL != m_btnWndInfo[i].pWnd)
			{
				tmp_y = (m_btnWndInfo[i].type == BTN_ITEM_SPLIT_EX || m_btnWndInfo[i].type == BTN_ITEM_FUNC_EX) ? (y + ex_yoff) : y;
				

				if(m_btnWndLayout.num_row > 1)
				{
					tmp_y = ((m_btnWndInfo[i].type > BTN_ITEM_SPLIT_EX) && (CTRLID_TOOL_BAR_PIP != m_btnWndInfo[i].pWnd->GetID())) ? tmp_y : (tmp_y + 34);
					if(m_btnWndInfo[i].pWnd->GetID() == CTRLID_TOOL_BAR_DWELL)
					{
						x = startx;
					}
				}

				m_btnWndInfo[i].pWnd->MoveWnd(x + xOff, tmp_y);
				if(!m_btnWndInfo[i].pWnd->IsShow())
				{
					m_btnWndInfo[i].pWnd->Show(true, false);
				}
				x += m_btnWndInfo[i].width;

				last_type = m_btnWndInfo[i].type;
			}
		}

		for(int i = m_lenWndInfo - 1; i > 0; i--)
		{
			tmp_y = ((m_btnWndInfo[i].type > BTN_ITEM_SPLIT_EX) && (CTRLID_TOOL_BAR_PIP != m_btnWndInfo[i].pWnd->GetID())) ? tmp_y : (tmp_y + 34);
			if (CTRLID_TOOL_BAR_SYSINFO == m_btnWndInfo[i].pWnd->GetID()) 
			{
				x=x_left + tool_width - 5 - m_btnWndInfo[i].width-m_btnWndInfo[i+1].width;
				m_btnWndInfo[i].pWnd->MoveWnd(x, tmp_y);
			}
			else if(CTRLID_TOOL_BAR_STATUS == m_btnWndInfo[i].pWnd->GetID())
			{
				x = x_left + tool_width - 5 - m_btnWndInfo[i].width;
				m_btnWndInfo[i].pWnd->MoveWnd(x, tmp_y);
			}
			else
			{
				break;
			}

			if(!m_btnWndInfo[i].pWnd->IsShow())
			{
				m_btnWndInfo[i].pWnd->Show(true, false);
			}
			
		}
		tmp_x = x_left + 3;
		tmp_y = (m_btnWndLayout.num_row > 1) ? (y + 34) : y;

		m_btnWndInfo[0].pWnd->MoveWnd(tmp_x + xOff, tmp_y);
		if(!m_btnWndInfo[0].pWnd->IsShow())
		{
			m_btnWndInfo[0].pWnd->Show(true, false);
		}
	}
	else
	{
		//计算各个按钮位置
		unsigned short tool_width = m_btnWndLayout.rect.Width(), tool_height = m_btnWndLayout.rect.Height();
		unsigned short x_left = m_btnWndLayout.rect.m_left, y_top = m_btnWndLayout.rect.m_top;
		unsigned short xOff = 2;

		unsigned short x = (x_left + tool_width - 3)/2;
		int toolLen = 0;
		for(int i = 1; i < m_lenWndInfo; i++)
		{
			if (CTRLID_TOOL_BAR_SYSINFO == m_btnWndInfo[i].pWnd->GetID())
			{
				break;
			}
			toolLen += m_btnWndInfo[i].width;//工具条长度
		}
		
		int startx= x- toolLen/2;
		x = startx;
		unsigned short y = y_top + 5;
		unsigned short ex_yoff = 5;
		unsigned short tmp_x = 0, tmp_y = 0;
		int last_type = BTN_ITEM_STATE;

		for(int i = 1; i < m_lenWndInfo-1; i++)
		{
			if(NULL != m_btnWndInfo[i].pWnd)
			{
				tmp_y = (m_btnWndInfo[i].type == BTN_ITEM_SPLIT_EX || m_btnWndInfo[i].type == BTN_ITEM_FUNC_EX) ? (y + ex_yoff) : y;
				m_btnWndInfo[i].pWnd->MoveWnd(x + xOff, tmp_y);
			
				if(!m_btnWndInfo[i].pWnd->IsShow())
				{
					m_btnWndInfo[i].pWnd->Show(true, false);
				}
				x += m_btnWndInfo[i].width;

				last_type = m_btnWndInfo[i].type;
			}
		}

		for(int i = m_lenWndInfo - 1; i > 0; i--)
		{
			tmp_y = (m_btnWndInfo[i].type == BTN_ITEM_SPLIT_EX || m_btnWndInfo[i].type == BTN_ITEM_FUNC_EX) ? (y + ex_yoff) : y;
			if (CTRLID_TOOL_BAR_SYSINFO == m_btnWndInfo[i].pWnd->GetID()) 
			{
				x=x_left + tool_width - 3 - m_btnWndInfo[i].width-m_btnWndInfo[i+1].width;
				m_btnWndInfo[i].pWnd->MoveWnd(x, tmp_y);
			}
			else if(CTRLID_TOOL_BAR_STATUS == m_btnWndInfo[i].pWnd->GetID())
			{
				x = x_left + tool_width - 3 - m_btnWndInfo[i].width;
				m_btnWndInfo[i].pWnd->MoveWnd(x, tmp_y);
			}
			else
			{
				break;
			}
			if(!m_btnWndInfo[i].pWnd->IsShow())
			{
				m_btnWndInfo[i].pWnd->Show(true, false);
			}

		}
		tmp_x = x_left + 3;
		tmp_y = (m_btnWndLayout.num_row > 1) ? (y + 34) : y;

		m_btnWndInfo[0].pWnd->MoveWnd(tmp_x + xOff, tmp_y);
		if(!m_btnWndInfo[0].pWnd->IsShow())
		{
			m_btnWndInfo[0].pWnd->Show(true, false);
		}

	}
	
}
bool GUI::CToolBar::IsAlmostEqual(unsigned long x1, unsigned long y1, unsigned long x2, unsigned long y2, unsigned long distance)
{
	unsigned long x = (x1 >= x2) ? (x1-x2) : (x2-x1);
	unsigned long y = (y1 >= y2) ? (y1-y2) : (y2-y1);

	if(((x*x) + (y*y)) <= distance)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool GUI::CToolBar::IsAlmostAbove(unsigned long x1, unsigned long y1, unsigned long x2, unsigned long y2, unsigned long distance)
{
	unsigned long x = (x1>=x2)?(x1-x2):(x2-x1);
	unsigned long y = (y1>=y2)?(y1-y2):(y2-y1);

	if(((x*x) + (y*y)) >= distance)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool GUI::CToolBar::CalculatePosInfo(NCFG_INFO_TOOLBAR_POS& posInfo, const GUI::CRect& rect, int xoffset, int yoffset, bool bAutoCenter)
{
	int pos_x = rect.m_left + xoffset;
	int pos_y = rect.m_top + yoffset;

	posInfo.xPos = (pos_x > 0) ? (pos_x * 100 / m_width) : 0;
	posInfo.yPos = (pos_y > 0) ? (pos_y * 100 / m_height) : 0;

	//x轴居左
	if(pos_x <= 10)
	{
		posInfo.xPos = 0;
	}
	
	//x轴居右
	if(pos_x + rect.Width() + 10 >= m_width)
	{
		posInfo.xPos = 100;
	}

	//x轴居中
	if( bAutoCenter &&
		((pos_x + rect.Width() / 2) >= (m_width / 2 - 20)) &&
		((pos_x + rect.Width() / 2) <= (m_width / 2 + 20)))
	{
		posInfo.xPos = INVALID_INDEX(unsigned short);
	}

	//y轴居顶
	if(pos_y <= 10)
	{
		posInfo.yPos = 0;
	}

	//y轴居底
	if(pos_y + rect.Height() + 10 >= m_height)
	{
		posInfo.yPos = INVALID_INDEX(unsigned short);
	}

	return true;
}

void GUI::CToolBar::Show(bool bShow, bool bRefresh)
{
	m_count = 0;

	if(bShow)
	{
		m_wait = 0;
	}
	else
	{
		m_wait = m_posInfo.postDelayTime / 2;
	}

	//m_bShowToolBar = bShow;
	GUI::CDialog::Show(bShow, bRefresh);
}

void GUI::CToolBar::OnChangeFocus(unsigned long oldID, unsigned long newID)
{
	if(GetCtrl(static_cast<GUI::CTRLID>(newID)))
	{
		m_count = 0;
	}
	
	GUI::CDialog::OnChangeFocus(oldID, newID);
}

/////////
BEGIN_MSG_MAP(GUI::CToolBar, GUI::CDialog)
	ON_MSG(CTRLID_TOOL_BAR_6X6, KEY_VIRTUAL_ENTER, OnClick6X6)
	ON_MSG(CTRLID_TOOL_BAR_5X5, KEY_VIRTUAL_ENTER, OnClick5X5)
	ON_MSG(CTRLID_TOOL_BAR_4X4, KEY_VIRTUAL_ENTER, OnClick4X4)
	ON_MSG(CTRLID_TOOL_BAR_1A12, KEY_VIRTUAL_ENTER, OnClick1A12)
	ON_MSG(CTRLID_TOOL_BAR_1A7, KEY_VIRTUAL_ENTER, OnClick1A7)
	ON_MSG(CTRLID_TOOL_BAR_3X3, KEY_VIRTUAL_ENTER, OnClick3X3)
	ON_MSG(CTRLID_TOOL_BAR_1A5, KEY_VIRTUAL_ENTER, OnClick1A5)
	ON_MSG(CTRLID_TOOL_BAR_2X3, KEY_VIRTUAL_ENTER, OnClick2X3)
	ON_MSG(CTRLID_TOOL_BAR_2X2, KEY_VIRTUAL_ENTER, OnClick2X2)
	ON_MSG(CTRLID_TOOL_BAR_1X1, KEY_VIRTUAL_ENTER, OnClick1X1)

	#ifdef __CUSTOM_US02_NEW__
	ON_MSG(CTRLID_TOOL_BAR_6X6, KEY_VIRTUAL_MOVE, OnFocus)
	ON_MSG(CTRLID_TOOL_BAR_5X5, KEY_VIRTUAL_MOVE, OnFocus)
	ON_MSG(CTRLID_TOOL_BAR_4X4, KEY_VIRTUAL_MOVE, OnFocus)
	ON_MSG(CTRLID_TOOL_BAR_1A12, KEY_VIRTUAL_MOVE, OnFocus)
	ON_MSG(CTRLID_TOOL_BAR_1A7, KEY_VIRTUAL_MOVE, OnFocus)
	ON_MSG(CTRLID_TOOL_BAR_3X3, KEY_VIRTUAL_MOVE, OnFocus)
	ON_MSG(CTRLID_TOOL_BAR_1A5, KEY_VIRTUAL_MOVE, OnFocus)
	ON_MSG(CTRLID_TOOL_BAR_2X3, KEY_VIRTUAL_MOVE, OnFocus)
	ON_MSG(CTRLID_TOOL_BAR_2X2, KEY_VIRTUAL_MOVE, OnFocus)
	ON_MSG(CTRLID_TOOL_BAR_1X1, KEY_VIRTUAL_MOVE, OnFocus)
	#endif
//*******************

	ON_MSG(CTRLID_TOOL_BAR_6X6_EX, KEY_VIRTUAL_ENTER, OnClick6X6Ex)
	ON_MSG(CTRLID_TOOL_BAR_5X5_EX, KEY_VIRTUAL_ENTER, OnClick5X5Ex)
	ON_MSG(CTRLID_TOOL_BAR_4X4_EX, KEY_VIRTUAL_ENTER, OnClick4X4Ex)
	ON_MSG(CTRLID_TOOL_BAR_1A12_EX, KEY_VIRTUAL_ENTER, OnClick1A12Ex)
	ON_MSG(CTRLID_TOOL_BAR_1A7_EX, KEY_VIRTUAL_ENTER, OnClick1A7Ex)
	ON_MSG(CTRLID_TOOL_BAR_3X3_EX, KEY_VIRTUAL_ENTER, OnClick3X3Ex)
	ON_MSG(CTRLID_TOOL_BAR_1A5_EX, KEY_VIRTUAL_ENTER, OnClick1A5Ex)
	ON_MSG(CTRLID_TOOL_BAR_2X3_EX, KEY_VIRTUAL_ENTER, OnClick2X3Ex)
	ON_MSG(CTRLID_TOOL_BAR_2X2_EX, KEY_VIRTUAL_ENTER, OnClick2X2Ex)
	ON_MSG(CTRLID_TOOL_BAR_1X1_EX, KEY_VIRTUAL_ENTER, OnClick1X1Ex)

	ON_MSG(CTRLID_TOOL_BAR_SYSTEM, KEY_VIRTUAL_ENTER, OnClickSystem)
	ON_MSG(CTRLID_TOOL_BAR_SEARCH, KEY_VIRTUAL_ENTER, OnClickSearch)
	ON_MSG(CTRLID_TOOL_BAR_BACKUP, KEY_VIRTUAL_ENTER, OnClickBackup)
	ON_MSG(CTRLID_TOOL_BAR_INFO, KEY_VIRTUAL_ENTER, OnClickInformation)

	#ifdef __CUSTOM_US02_NEW__
	ON_MSG(CTRLID_TOOL_BAR_SYSTEM, KEY_VIRTUAL_MOVE, OnFocus)
	ON_MSG(CTRLID_TOOL_BAR_SEARCH, KEY_VIRTUAL_MOVE, OnFocus)
	ON_MSG(CTRLID_TOOL_BAR_BACKUP, KEY_VIRTUAL_MOVE, OnFocus)
	ON_MSG(CTRLID_TOOL_BAR_INFO, KEY_VIRTUAL_MOVE, OnFocus)
	#endif

//******************************

	ON_MSG(CTRLID_TOOL_BAR_DWELL, KEY_VIRTUAL_ENTER, OnClickDwell)
	ON_MSG(CTRLID_TOOL_BAR_ZOOM, KEY_VIRTUAL_ENTER, OnClickZoom)
	ON_MSG(CTRLID_TOOL_BAR_AUDIO, KEY_VIRTUAL_ENTER, OnClickAudio)
	ON_MSG(CTRLID_TOOL_BAR_PTZ, KEY_VIRTUAL_ENTER, OnClickPTZ)
	ON_MSG(CTRLID_TOOL_BAR_REC, KEY_VIRTUAL_ENTER, OnClickRec)
	ON_MSG(CTRLID_TOOL_BAR_PLAY, KEY_VIRTUAL_ENTER, OnClickPlay)
	ON_MSG(CTRLID_TOOL_BAR_PLAY_EX, KEY_VIRTUAL_ENTER, OnClickPlayEx)
	ON_MSG(CTRLID_TOOL_BAR_SHUTDOWN, KEY_VIRTUAL_ENTER, OnClickShutDown)
	ON_MSG(CTRLID_TOOL_BAR_CHANGE_COLOR,KEY_VIRTUAL_ENTER,OnClickChange)
	ON_MSG(CTRLID_TOOL_BAR_SNAP,KEY_VIRTUAL_ENTER,OnClickSnap)
	ON_MSG(CTRLID_TOOL_BAR_PIP, KEY_VIRTUAL_ENTER, OnClickPIP)
	ON_MSG(CTRLID_TOOL_BAR_CAM_PARA,KEY_VIRTUAL_ENTER,OnClickCamPara)

	#ifdef __CUSTOM_US02_NEW__
	ON_MSG(CTRLID_TOOL_BAR_DWELL, KEY_VIRTUAL_MOVE, OnFocus)
	ON_MSG(CTRLID_TOOL_BAR_ZOOM, KEY_VIRTUAL_MOVE, OnFocus)
	ON_MSG(CTRLID_TOOL_BAR_AUDIO, KEY_VIRTUAL_MOVE, OnFocus)
	ON_MSG(CTRLID_TOOL_BAR_PTZ, KEY_VIRTUAL_MOVE, OnFocus)
	ON_MSG(CTRLID_TOOL_BAR_REC, KEY_VIRTUAL_MOVE, OnFocus)
	ON_MSG(CTRLID_TOOL_BAR_PLAY, KEY_VIRTUAL_MOVE, OnFocus)
	ON_MSG(CTRLID_TOOL_BAR_PLAY_EX, KEY_VIRTUAL_MOVE, OnFocus)
	ON_MSG(CTRLID_TOOL_BAR_SHUTDOWN, KEY_VIRTUAL_MOVE, OnFocus)
	ON_MSG(CTRLID_TOOL_BAR_CHANGE_COLOR,KEY_VIRTUAL_MOVE,OnFocus)
	ON_MSG(CTRLID_TOOL_BAR_SNAP,KEY_VIRTUAL_MOVE,OnFocus)
	ON_MSG(CTRLID_TOOL_BAR_CAM_PARA,KEY_VIRTUAL_MOVE,OnFocus)
	ON_MSG(CTRLID_TOOL_BAR_PIP, KEY_VIRTUAL_MOVE, OnFocus)
	ON_MSG(CTRLID_TOOL_BAR_CAM_PARA,KEY_VIRTUAL_MOVE,OnFocus)
	#endif



	//***************************************************

	ON_MSG_EX(CTRLID_TOOL_BAR_STATUS, KEY_VIRTUAL_ENTER, OnClickToolEdit)
	ON_MSG(CTRLID_TOOL_BAR_BURN, KEY_VIRTUAL_ENTER, OnClickBurn)
	ON_MSG(CTRLID_TOOL_BAR_BURN_EX, KEY_VIRTUAL_ENTER, OnClickBurnEx)
	ON_MSG(CTRLID_TOOL_BAR_SYSINFO, KEY_VIRTUAL_ENTER, OnClickSysInfo)

	#ifdef __CUSTOM_US02_NEW__
	ON_MSG_EX(CTRLID_TOOL_BAR_STATUS, KEY_VIRTUAL_MOVE, OnFocus)
	ON_MSG(CTRLID_TOOL_BAR_BURN, KEY_VIRTUAL_MOVE, OnFocus)
	ON_MSG(CTRLID_TOOL_BAR_BURN_EX, KEY_VIRTUAL_MOVE, OnFocus)
	ON_MSG(CTRLID_TOOL_BAR_SYSINFO, KEY_VIRTUAL_MOVE, OnFocus)
	#endif



//toolbar
	#ifdef __CUSTOM_US02_NEW__
	ON_MSG(CTRLID_TOOL_BAR_DLG,KEY_VIRTUAL_MOVE, OnFocus)
	ON_MSG(CTRLID_TOOL_BAR_PTZ,KEY_VIRTUAL_MOVE, OnFocus)
	#endif
	//ON_MSG(CTRLID_TOOL_BAR_STATUS, KEY_VIRTUAL_BLUP, OnLostFocus)


END_MSG_MAP()
//////////////////////////////////////////////////////////////////////////
unsigned long GUI::CToolBar::OnClickPIP()
{

	return KEY_VIRTUAL_PIP;
}
unsigned long GUI::CToolBar::OnClick6X6()
{
	return KEY_VIRTUAL_6X6;
}

unsigned long GUI::CToolBar::OnClick5X5()
{
	return KEY_VIRTUAL_5X5;
}

unsigned long GUI::CToolBar::OnClick4X4()
{
	return KEY_VIRTUAL_4X4;
}

unsigned long GUI::CToolBar::OnClick1A12()
{
	return KEY_VIRTUAL_MSG_DEFAULT_TAG | CTRLID_TOOL_BAR_1A12;
}

unsigned long GUI::CToolBar::OnClick1A7()
{
	return KEY_VIRTUAL_MSG_DEFAULT_TAG | CTRLID_TOOL_BAR_1A7;
}

unsigned long GUI::CToolBar::OnClick3X3()
{
	return KEY_VIRTUAL_3X3;
}

unsigned long GUI::CToolBar::OnClick1A5()
{
	return KEY_VIRTUAL_MSG_DEFAULT_TAG | CTRLID_TOOL_BAR_1A5;
}

unsigned long GUI::CToolBar::OnClick2X3()
{
	return KEY_VIRTUAL_2X3;
}

unsigned long GUI::CToolBar::OnClick2X2()
{
	return KEY_VIRTUAL_2X2;
}

unsigned long GUI::CToolBar::OnClick1X1()
{
	return KEY_VIRTUAL_1X1;
}

#ifdef __CUSTOM_US02_NEW__
unsigned long GUI::CToolBar::OnFocus()
{
	CWnd* active = GetActiveCtrl();
	if(active != NULL)
	{
		return KEY_VIRTUAL_OVER;
	}
	else
	{
		return KEY_VIRTUAL_OUT;
	}

}
#endif


#ifdef __CUSTOM_US02_NEW__
unsigned long GUI::CToolBar::OnFocus(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	CWnd* active = GetActiveCtrl();
	if(active != NULL)
	{
		return KEY_VIRTUAL_OVER;
	}
	else
	{
		return KEY_VIRTUAL_OUT;
	}
}
#endif


unsigned long GUI::CToolBar::OnClick6X6Ex()
{
	return KEY_VIRTUAL_6X6_EX;
}

unsigned long GUI::CToolBar::OnClick5X5Ex()
{
	return KEY_VIRTUAL_5X5_EX;
}

unsigned long GUI::CToolBar::OnClick4X4Ex()
{
	return KEY_VIRTUAL_4X4_EX;
}

unsigned long GUI::CToolBar::OnClick1A12Ex()
{
	return KEY_VIRTUAL_MSG_DEFAULT_TAG | CTRLID_TOOL_BAR_1A12_EX;
}

unsigned long GUI::CToolBar::OnClick1A7Ex()
{
	return KEY_VIRTUAL_MSG_DEFAULT_TAG | CTRLID_TOOL_BAR_1A7_EX;
}

unsigned long GUI::CToolBar::OnClick3X3Ex()
{
	return KEY_VIRTUAL_3X3_EX;
}

unsigned long GUI::CToolBar::OnClick1A5Ex()
{
	return KEY_VIRTUAL_MSG_DEFAULT_TAG | CTRLID_TOOL_BAR_1A5_EX;
}

unsigned long GUI::CToolBar::OnClick2X3Ex()
{
	return KEY_VIRTUAL_2X3_EX;
}

unsigned long GUI::CToolBar::OnClick2X2Ex()
{
	return KEY_VIRTUAL_2X2_EX;
}

unsigned long GUI::CToolBar::OnClick1X1Ex()
{
	return KEY_VIRTUAL_1X1_EX;
}

unsigned long GUI::CToolBar::OnClickSystem()
{
	return KEY_VIRTUAL_MENU;
}

unsigned long GUI::CToolBar::OnClickSearch()
{
	return KEY_VIRTUAL_SEARCH;
}

unsigned long GUI::CToolBar::OnClickBackup()
{
	return KEY_VIRTUAL_BACKUP;
}

unsigned long GUI::CToolBar::OnClickInformation()
{
	return KEY_VIRTUAL_INFO;
}

unsigned long GUI::CToolBar::OnClickDwell()
{
	return KEY_VIRTUAL_SEQ;
}

unsigned long GUI::CToolBar::OnClickZoom()
{
	return KEY_VIRTUAL_ZOOM;
}

unsigned long GUI::CToolBar::OnClickAudio()
{
	return KEY_VIRTUAL_AUDIO;
}

unsigned long GUI::CToolBar::OnClickPTZ()
{
	return KEY_VIRTUAL_PTZ;
}

unsigned long GUI::CToolBar::OnClickRec()
{
	return KEY_VIRTUAL_RECORD;
}

unsigned long GUI::CToolBar::OnClickPlay()
{
	return KEY_VIRTUAL_PLAY;
}

unsigned long GUI::CToolBar::OnClickPlayEx()
{
	return KEY_VIRTUAL_PLAY_EX;
}

unsigned long GUI::CToolBar::OnClickShutDown()
{
	return KEY_VIRTUAL_POWER;
}

unsigned long GUI::CToolBar::OnClickStatus()
{
	return KEY_VIRTUAL_MSG_DEFAULT_TAG | CTRLID_TOOL_BAR_STATUS;
}

unsigned long GUI::CToolBar::OnClickChange()
{
	return KEY_VIRTUAL_MSG_DEFAULT_TAG | CTRLID_TOOL_BAR_CHANGE_COLOR;
}

unsigned long GUI::CToolBar::OnClickCamPara()
{
	return KEY_VIRTUAL_MSG_DEFAULT_TAG | CTRLID_TOOL_BAR_CAM_PARA;
}

unsigned long GUI::CToolBar::OnClickSnap()
{
	return KEY_VIRTUAL_MSG_DEFAULT_TAG | CTRLID_TOOL_BAR_SNAP;
}

unsigned long GUI::CToolBar::OnClickToolEdit(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	if(PtInToolbar(xPos, yPos))
	{
		m_bDrag = true;
		m_bFirstDrag = true;

		m_x_enter = xPos;
		m_y_enter = yPos;
	}

	return KEY_VIRTUAL_MSG_STOP;
}


unsigned long GUI::CToolBar::OnClickBurn()
{
	return KEY_VIRTUAL_BURN;
}

unsigned long GUI::CToolBar::OnClickBurnEx()
{
	return KEY_VIRTUAL_BURN_EX;
}

unsigned long GUI::CToolBar::OnClickSysInfo()
{
	return KEY_VIRTUAL_SYSINFO;
}
//////////////////////////////////////////////////////////////////////////

void GUI::CToolBar::OnEnter(unsigned long xPos, unsigned long yPos)
{
	if(PtInToolbar(xPos, yPos))
	{
		m_count = 0;
		m_wait = 50;
	}
}

void GUI::CToolBar::OnDBClick(unsigned long xPos, unsigned long yPos)
{
	if(PtInToolbar(xPos, yPos))
	{
		//单双击, 重置计时
		m_count = 0;
		m_wait = 50;
	}
}

void GUI::CToolBar::OnMouseMove(unsigned long xPos, unsigned long yPos)
{
	m_x_pos_last = m_x_pos;
	m_y_pos_last = m_y_pos;

	m_x_pos = xPos;
	m_y_pos = yPos;
}

GUI::CRect GUI::CToolBar::OnMouseDrag(unsigned long xPos, unsigned long yPos, bool bRefresh)
{
	GUI::CRect retRect(0, 0, 0, 0);

	if(m_bDrag)
	{
		if(m_bFirstDrag)
		{
			m_x_drag_last = m_x_enter;
			m_y_drag_last = m_y_enter;

			m_bFirstDrag = false;
		}

		GUI::CRect oldRect = m_btnWndLayout.rect;

		unsigned long offset_len = ((m_width / 50) * (m_width / 50)) + ((m_height / 50) * (m_height / 50));
		//unsigned long offset_len = 200;
		if(!IsAlmostEqual(m_x_drag_last, m_y_drag_last, xPos, yPos, offset_len))
		{
			if(MoveToolWnd(xPos, yPos))
			{
				retRect = oldRect.UnionRect(m_btnWndLayout.rect);

				m_x_drag_last = xPos;
				m_y_drag_last = yPos;
			}

		}
	}

	return retRect;
}

unsigned long GUI::CToolBar::OnMouseLeftUp()
{
	if(m_bDrag)
	{
		m_bDrag = false;
		m_bFirstDrag = false;

		return KEY_VIRTUAL_MSG_CONTINUE;
	}

	return KEY_VIRTUAL_MSG_STOP;
}

//////////////////////////////////////////////////////////////////////////
