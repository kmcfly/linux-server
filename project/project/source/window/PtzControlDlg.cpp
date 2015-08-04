/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : L
** Date         : 2009-08-17
** Name         : PtzControlDlg.cpp
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/

#include "PtzControlDlg.h"
#include "NetProtocol.h"
#include "PUB_common.h"
#include "PTZViewDlg.h"

CPtzControlDlg::CPtzControlDlg()
{
	
}

CPtzControlDlg::~CPtzControlDlg()
{

}


void CPtzControlDlg::OnInitial()
{
	GUI::CDialog::OnInitial();
	m_style |= WND_NO_TITLE;
	//////////////////////////////////////////////////////////////////////////
	//方向键
	int x = 10;
	int y = 4;
	{
		int size = 15;
		int cx = 0;
		int cy = 0;
		GUI::CWnd::GetImageSize(BMP_PTZ_CTRL_BK, cx, cy);
		m_rectPtzCtrl.SetRect(0, 0, cx, cy);
		m_rectPtzCtrl.MoveToXY(x, y);

		int xOffset = 43, yOffset = 10;
		m_btnUp.Create( CTRLID_PTZCONTROL_UP, m_rectPtzCtrl.m_left + xOffset,	m_rectPtzCtrl.m_top + yOffset, size, size, this, 1, 0);
		m_btnUp.SetImage(PAINT_STATUS_NORMAL, BMP_PTZ_UP, false);
		m_btnUp.SetImage(PAINT_STATUS_FOCUS, BMP_PTZ_UP_F, false);

		xOffset = 65; yOffset = 21;
		m_btnUpRight.Create(CTRLID_PTZCONTROL_UPRIGHT, m_rectPtzCtrl.m_left + xOffset,	m_rectPtzCtrl.m_top + yOffset, size, size, this, 2, 0);
		m_btnUpRight.SetImage(PAINT_STATUS_NORMAL, BMP_PTZ_UP_RIGHT, false);
		m_btnUpRight.SetImage(PAINT_STATUS_FOCUS, BMP_PTZ_UP_RIGHT_F, false);

		xOffset = 76; yOffset = 42;
		m_btnRight.Create(CTRLID_PTZCONTROL_RIGHT, m_rectPtzCtrl.m_left + xOffset,	m_rectPtzCtrl.m_top + yOffset, size, size, this, 2, 1);
		m_btnRight.SetImage(PAINT_STATUS_NORMAL, BMP_PTZ_RIGHT, false);
		m_btnRight.SetImage(PAINT_STATUS_FOCUS, BMP_PTZ_RIGHT_F, false);

		xOffset = 65; yOffset = 65;
		m_btnDownRight.Create(CTRLID_PTZCONTROL_DOWNRIGHT, m_rectPtzCtrl.m_left + xOffset,	m_rectPtzCtrl.m_top + yOffset, size, size, this, 2, 2);
		m_btnDownRight.SetImage(PAINT_STATUS_NORMAL, BMP_PTZ_DOWN_RIGHT, false);
		m_btnDownRight.SetImage(PAINT_STATUS_FOCUS, BMP_PTZ_DOWN_RIGHT_F, false);

		xOffset = 43; yOffset = 76;
		m_btnDown.Create(CTRLID_PTZCONTROL_DOWN, m_rectPtzCtrl.m_left + xOffset,	m_rectPtzCtrl.m_top + yOffset, size, size, this, 1, 2);
		m_btnDown.SetImage(PAINT_STATUS_NORMAL, BMP_PTZ_DOWN, false);
		m_btnDown.SetImage(PAINT_STATUS_FOCUS, BMP_PTZ_DOWN_F, false);

		xOffset = 22; yOffset = 65;
		m_btnDownLeft.Create(CTRLID_PTZCONTROL_DOWNLEFT, m_rectPtzCtrl.m_left + xOffset,	m_rectPtzCtrl.m_top + yOffset, size, size, this, 0, 2);
		m_btnDownLeft.SetImage(PAINT_STATUS_NORMAL, BMP_PTZ_DOWN_LEFT, false);
		m_btnDownLeft.SetImage(PAINT_STATUS_FOCUS, BMP_PTZ_DOWN_LEFT_F, false);

		xOffset = 10; yOffset = 42;
		m_btnLeft.Create(CTRLID_PTZCONTROL_LEFT, m_rectPtzCtrl.m_left + xOffset,	m_rectPtzCtrl.m_top + yOffset, size, size, this, 0, 1);
		m_btnLeft.SetImage(PAINT_STATUS_NORMAL, BMP_PTZ_LEFT, false);
		m_btnLeft.SetImage(PAINT_STATUS_FOCUS, BMP_PTZ_LEFT_F, false);

		xOffset = 22; yOffset = 21;
		m_btnUpLeft.Create(CTRLID_PTZCONTROL_UPLEFT, m_rectPtzCtrl.m_left + xOffset,	m_rectPtzCtrl.m_top + yOffset, size, size, this, 0, 0);
		m_btnUpLeft.SetImage(PAINT_STATUS_NORMAL, BMP_PTZ_UP_LEFT, false);
		m_btnUpLeft.SetImage(PAINT_STATUS_FOCUS, BMP_PTZ_UP_LEFT_F, false);

		xOffset = 42; yOffset = 42;
		m_btnStop.Create(CTRLID_PTZCONTROL_STOP, m_rectPtzCtrl.m_left + xOffset,	m_rectPtzCtrl.m_top + yOffset, size, size, this, 1, 1);
		m_btnStop.SetImage(PAINT_STATUS_NORMAL, BMP_PTZ_STOP, false);
		m_btnStop.SetImage(PAINT_STATUS_FOCUS, BMP_PTZ_STOP_F, false);
	}	
	
	//焦距等
	{
		x = m_rectPtzCtrl.m_right + 10;
		y = 10;
		int cx = 30;
		int cy = 25;
		int text_cx = 70;
		int offset_y = cy + 8;
		int offset_x = cx + text_cx;
		

		m_btnZoomSub.Create(CTRLID_PTZCONTROL_ZOOMIN, x, y, cx, cy, this, 3, 0);
		y += offset_y;
		m_btnFocusSub.Create(CTRLID_PTZCONTROL_FOCUSIN, x, y, cx, cy, this, 3, 1);
		y += offset_y;
		m_btnApertureSub.Create(CTRLID_PTZCONTROL_IRISIN, x, y, cx, cy, this, 3, 2);

		
		////
		y = 10;
		x += offset_x;
		m_btnZoomAdd.Create(CTRLID_PTZCONTROL_ZOOMOUT, x, y, cx, cy, this, 4, 0);
		y += offset_y;
		m_btnFocusAdd.Create(CTRLID_PTZCONTROL_FOCUSOUT, x, y, cx, cy, this, 4, 1);
		y += offset_y;
		m_btnApertureAdd.Create(CTRLID_PTZCONTROL_IRISOUT, x, y, cx, cy, this, 4, 2);
		////Text
		x -= text_cx;
		y = 10;
		m_scZoom.Create(CTRLID_STATIC, x, y, text_cx - 2, m_length[STATIC_HEIGHT], this, m_pStrTable->String(E_STR_ID_ZOOM));
		m_scZoom.SetAlignText(GUI::ALIGN_TEXT_CENTER);
		y += offset_y;
		m_scFocus.Create(CTRLID_STATIC, x, y, text_cx - 2, m_length[STATIC_HEIGHT], this, m_pStrTable->String(E_STR_ID_FOCUS));
		m_scFocus.SetAlignText(GUI::ALIGN_TEXT_CENTER);
		y += offset_y;
		m_scAperture.Create(CTRLID_STATIC, x, y, text_cx - 2, m_length[STATIC_HEIGHT], this, m_pStrTable->String(E_STR_ID_IRIS));
		m_scAperture.SetAlignText(GUI::ALIGN_TEXT_CENTER);



		m_btnZoomSub.SetImage(PAINT_STATUS_NORMAL, BMP_BTN_DEC, false);
		m_btnZoomSub.SetImage(PAINT_STATUS_FOCUS, BMP_BTN_DEC_FOCUS, false);
		m_btnZoomSub.SetImage(PAINT_STATUS_DISABLE, BMP_BTN_DEC, false);

		m_btnFocusSub.SetImage(PAINT_STATUS_NORMAL, BMP_BTN_DEC, false);
		m_btnFocusSub.SetImage(PAINT_STATUS_FOCUS, BMP_BTN_DEC_FOCUS, false);
		m_btnFocusSub.SetImage(PAINT_STATUS_DISABLE, BMP_BTN_DEC, false);

		m_btnApertureSub.SetImage(PAINT_STATUS_NORMAL, BMP_BTN_DEC, false);
		m_btnApertureSub.SetImage(PAINT_STATUS_FOCUS, BMP_BTN_DEC_FOCUS, false);
		m_btnApertureSub.SetImage(PAINT_STATUS_DISABLE, BMP_BTN_DEC, false);

		m_btnZoomAdd.SetImage(PAINT_STATUS_NORMAL, BMP_BTN_ADD, false);
		m_btnZoomAdd.SetImage(PAINT_STATUS_FOCUS, BMP_BTN_ADD_FOCUS, false);
		m_btnZoomAdd.SetImage(PAINT_STATUS_DISABLE, BMP_BTN_ADD, false);

		m_btnFocusAdd.SetImage(PAINT_STATUS_NORMAL, BMP_BTN_ADD, false);
		m_btnFocusAdd.SetImage(PAINT_STATUS_FOCUS, BMP_BTN_ADD_FOCUS, false);
		m_btnFocusAdd.SetImage(PAINT_STATUS_DISABLE, BMP_BTN_ADD, false);

		m_btnApertureAdd.SetImage(PAINT_STATUS_NORMAL, BMP_BTN_ADD, false);
		m_btnApertureAdd.SetImage(PAINT_STATUS_FOCUS, BMP_BTN_ADD_FOCUS, false);
		m_btnApertureAdd.SetImage(PAINT_STATUS_DISABLE, BMP_BTN_ADD, false);
	}

	//速度
	{
		y = m_rect.Height() - m_length[BTN_HEIGHT];
		x = m_rectPtzCtrl.m_left;
		unsigned short text_cx = 0;
		unsigned short text_cy = 0;
		const char* pStr = m_pStrTable->String(E_STR_ID_SPEED);
		std::string str = pStr;
		str += ": 5";
		TextSize(str.c_str(), str.length(), text_cx, text_cy);
		m_scSpeed.Create( CTRLID_STATIC, x, y - 2, text_cx, m_length[STATIC_HEIGHT], this, str.c_str());
		m_scSpeed.SetBKColor(COLOR_DLG_CHILD_BK);
		
		//x += text_cx + 10;
		int slider_cx = m_btnApertureAdd.GetWindowRect().m_right - m_btnZoomSub.GetWindowRect().m_left;
		m_sliderSpeed.Create(GetRandWndID(), m_btnZoomSub.GetWindowRect().m_left - m_rect.m_left, y, slider_cx, this, 0, 4);
		m_sliderSpeed.SetRange(0, 7);
		m_sliderSpeed.SetCurPos(4, false);
		CMessageMan::Instance()->SetCurrPTZSpeed( m_sliderSpeed.GetPos() + 1 );
	}
}

void CPtzControlDlg::SetPTZFuncMode(PTZ_FUNC_MODE mode)
{
	if (PTZ_FUNC_ZOOM == mode)
	{
		m_scZoom.SetTextColor(COLOR_SELECT);
		m_scFocus.SetTextColor(COLOR_LINE_WHITE);
		m_scAperture.SetTextColor(COLOR_LINE_WHITE);
		m_scSpeed.SetTextColor(COLOR_LINE_WHITE);
	}
	else if (PTZ_FUNC_FOCUS == mode)
	{
		m_scFocus.SetTextColor(COLOR_SELECT);
		m_scZoom.SetTextColor(COLOR_LINE_WHITE);
		m_scAperture.SetTextColor(COLOR_LINE_WHITE);
		m_scSpeed.SetTextColor(COLOR_LINE_WHITE);
	}
	else if (PTZ_FUNC_IRIS == mode)
	{
		m_scAperture.SetTextColor(COLOR_SELECT);
		m_scFocus.SetTextColor(COLOR_LINE_WHITE);
		m_scZoom.SetTextColor(COLOR_LINE_WHITE);
		m_scSpeed.SetTextColor(COLOR_LINE_WHITE);
	}
	else if (PTZ_FUNC_SPEED == mode)
	{
		m_scSpeed.SetTextColor(COLOR_SELECT);
		m_scAperture.SetTextColor(COLOR_LINE_WHITE);
		m_scFocus.SetTextColor(COLOR_LINE_WHITE);
		m_scZoom.SetTextColor(COLOR_LINE_WHITE);
	}

	Repaint();
}

unsigned long CPtzControlDlg::ResponseHideCommond( unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	return OnCommand( ctrlID, keyID, xPos, yPos);
}



void CPtzControlDlg::OnPaint(PAINT_STATUS status)
{
//	GUI::CDialog::OnPaint(status);

	GUI::CWnd::DrawImage(BMP_PTZ_CTRL_BK, m_rect.m_left + m_rectPtzCtrl.m_left, m_rect.m_top + m_rectPtzCtrl.m_top);
}


BEGIN_MSG_MAP(CPtzControlDlg, GUI::CDialog)
//	ON_MSG( m_btnExit.GetID(), KEY_VIRTUAL_ENTER, OnBtnExit )
	ON_MSG_EX(m_sliderSpeed.GetID(), KEY_VIRTUAL_ENTER, OnSpeedChange)
	ON_MSG_EX(m_sliderSpeed.GetID(), KEY_VIRTUAL_DRAG, OnSpeedChange)
	ON_MSG_EX(m_sliderSpeed.GetID(), KEY_VIRTUAL_BLUP, OnSpeedChange)


	//处理鼠标左键
	ON_MSG_EX(m_btnLeft.GetID(), KEY_VIRTUAL_ENTER, OnLeft)
	ON_MSG_EX(m_btnRight.GetID(), KEY_VIRTUAL_ENTER, OnRight)
	ON_MSG_EX(m_btnUp.GetID(), KEY_VIRTUAL_ENTER, OnUp)
	ON_MSG_EX(m_btnDown.GetID(), KEY_VIRTUAL_ENTER, OnDown)

	ON_MSG_EX(m_btnUpRight.GetID(), KEY_VIRTUAL_ENTER, OnUpRight)
	ON_MSG_EX(m_btnDownRight.GetID(), KEY_VIRTUAL_ENTER, OnDownRight)
	ON_MSG_EX(m_btnUpLeft.GetID(), KEY_VIRTUAL_ENTER, OnUpLeft)
	ON_MSG_EX(m_btnDownLeft.GetID(), KEY_VIRTUAL_ENTER, OnDownLeft)

	ON_MSG_EX(m_btnFocusSub.GetID(), KEY_VIRTUAL_ENTER, OnNear)
	ON_MSG_EX(m_btnFocusAdd.GetID(), KEY_VIRTUAL_ENTER, OnFar)
	ON_MSG_EX(m_btnZoomAdd.GetID(), KEY_VIRTUAL_ENTER, OnZoomOut)
	ON_MSG_EX(m_btnZoomSub.GetID(), KEY_VIRTUAL_ENTER, OnZoomIn)

	ON_MSG_EX(m_btnApertureAdd.GetID(), KEY_VIRTUAL_ENTER, OnIrisOpen)
	ON_MSG_EX(m_btnApertureSub.GetID(), KEY_VIRTUAL_ENTER, OnIrisClose)

	ON_MSG(m_btnStop.GetID(), KEY_VIRTUAL_ENTER, OnStop)
	ON_MSG_CTRL_RANGE( CTRLID_PTZCONTROL_UP, CTRLID_PTZCONTROL_IRISOUT, KEY_VIRTUAL_BLUP, OnMouseLftBttnUp )



	//处理按键板、485键盘、遥控的按键
	ON_MSG_EX(GUI::CTRLID_NULL, KEY_VIRTUAL_PTZ_LEFT, OnLeft)
	ON_MSG_EX(GUI::CTRLID_NULL, KEY_VIRTUAL_PTZ_RIGHT, OnRight)
	ON_MSG_EX(GUI::CTRLID_NULL, KEY_VIRTUAL_PTZ_UP, OnUp)
	ON_MSG_EX(GUI::CTRLID_NULL, KEY_VIRTUAL_PTZ_DOWN, OnDown)

	ON_MSG_EX(GUI::CTRLID_NULL, KEY_VIRTUAL_PTZ_RIGHTDOWN, OnDownRight)
	ON_MSG_EX(GUI::CTRLID_NULL, KEY_VIRTUAL_PTZ_LEFTDOWN, OnDownLeft)
	ON_MSG_EX(GUI::CTRLID_NULL, KEY_VIRTUAL_PTZ_RIGHTUP, OnUpRight)
	ON_MSG_EX(GUI::CTRLID_NULL, KEY_VIRTUAL_PTZ_LEFTUP, OnUpLeft)

	ON_MSG_EX(GUI::CTRLID_NULL, KEY_VIRTUAL_FOCUS_ADD, OnFar)
	ON_MSG_EX(GUI::CTRLID_NULL, KEY_VIRTUAL_FOCUS_DEC, OnNear)
	ON_MSG_EX(GUI::CTRLID_NULL, KEY_VIRTUAL_ZOOM_ADD, OnZoomOut)
	ON_MSG_EX(GUI::CTRLID_NULL, KEY_VIRTUAL_ZOOM_DEC, OnZoomIn)

	ON_MSG_EX(GUI::CTRLID_NULL, KEY_VIRTUAL_IRIS_ADD, OnIrisOpen)
	ON_MSG_EX(GUI::CTRLID_NULL, KEY_VIRTUAL_IRIS_DEC, OnIrisClose)
	
	ON_MSG(GUI::CTRLID_NULL, KEY_VIRTUAL_SPEED_ADD,OnPTZSpeedAdd)
	ON_MSG(GUI::CTRLID_NULL, KEY_VIRTUAL_SPEED_DEC,OnPTZSpeedDec)

	ON_MSG(GUI::CTRLID_NULL, KEY_VIRTUAL_PTZ_STOP, OnStop)
END_MSG_MAP()

//unsigned long CPtzControlDlg::OnBtnExit()
//{
//	return OnOK();
//}

unsigned long CPtzControlDlg::OnSpeedChange(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	SpeedChange();

	return KEY_VIRTUAL_MSG_STOP;
}


///////////////////////////////////////////////////////////////////////////////
unsigned long CPtzControlDlg::OnUp(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	PTZ_CTRL ptzCtrl;
	ptzCtrl.chnn = CMessageMan::Instance()->CurrPTZChnn();
	ptzCtrl.cmdType = PTZ_CMD_UP;
	ptzCtrl.speed = m_sliderSpeed.GetPos() + 1;
	
	MESSAGE_DATA msgData;
	memset(&msgData, 0, sizeof(MESSAGE_DATA));
	CMessageMan::Instance()->SendMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_PTZ_CTRL, (unsigned char *)&ptzCtrl, sizeof(PTZ_CTRL), msgData);
	
	ReleaseMsgBuff( msgData );

	if ((INVALID_INDEX(unsigned long) == xPos) || (INVALID_INDEX(unsigned long) == yPos))
	{
		PUB_Sleep(50);
		OnStop();
	}

	return KEY_VIRTUAL_PTZ_UP;
}

unsigned long CPtzControlDlg::OnDown(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	PTZ_CTRL ptzCtrl;
	ptzCtrl.chnn = CMessageMan::Instance()->CurrPTZChnn();
	ptzCtrl.cmdType = PTZ_CMD_DOWN;
	ptzCtrl.speed = m_sliderSpeed.GetPos() + 1;
	
	MESSAGE_DATA msgData;
	memset(&msgData, 0, sizeof(MESSAGE_DATA));
	CMessageMan::Instance()->SendMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_PTZ_CTRL, (unsigned char *)&ptzCtrl, sizeof(PTZ_CTRL), msgData);

	ReleaseMsgBuff( msgData );
	
	if ((INVALID_INDEX(unsigned long) == xPos) || (INVALID_INDEX(unsigned long) == yPos))
	{
		PUB_Sleep(50);
		OnStop();
	}

	return KEY_VIRTUAL_PTZ_DOWN;
}

unsigned long CPtzControlDlg::OnLeft(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	PTZ_CTRL ptzCtrl;
	ptzCtrl.chnn = CMessageMan::Instance()->CurrPTZChnn();
	ptzCtrl.cmdType = PTZ_CMD_LEFT;
	ptzCtrl.speed = m_sliderSpeed.GetPos() + 1;
	
	MESSAGE_DATA msgData;
	memset(&msgData, 0, sizeof(MESSAGE_DATA));
	CMessageMan::Instance()->SendMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_PTZ_CTRL, (unsigned char *)&ptzCtrl, sizeof(PTZ_CTRL), msgData);

	ReleaseMsgBuff( msgData );

	if ((INVALID_INDEX(unsigned long) == xPos) || (INVALID_INDEX(unsigned long) == yPos))
	{
		PUB_Sleep(50);
		OnStop();
	}

	return KEY_VIRTUAL_PTZ_LEFT;
}

unsigned long CPtzControlDlg::OnRight(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	PTZ_CTRL ptzCtrl;
	ptzCtrl.chnn = CMessageMan::Instance()->CurrPTZChnn();
	ptzCtrl.cmdType = PTZ_CMD_RIGHT;
	ptzCtrl.speed = m_sliderSpeed.GetPos() + 1;
	
	MESSAGE_DATA msgData;
	memset(&msgData, 0, sizeof(MESSAGE_DATA));
	CMessageMan::Instance()->SendMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_PTZ_CTRL, (unsigned char *)&ptzCtrl, sizeof(PTZ_CTRL), msgData);

	ReleaseMsgBuff( msgData );

	if ((INVALID_INDEX(unsigned long) == xPos) || (INVALID_INDEX(unsigned long) == yPos))
	{
		PUB_Sleep(50);
		OnStop();
	}

	return KEY_VIRTUAL_PTZ_RIGHT;
}


unsigned long CPtzControlDlg::OnUpRight(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	PTZ_CTRL ptzCtrl;
	ptzCtrl.chnn = CMessageMan::Instance()->CurrPTZChnn();
	ptzCtrl.cmdType = PTZ_CMD_RIGHT_UP;
	ptzCtrl.speed = m_sliderSpeed.GetPos() + 1;
	
	MESSAGE_DATA msgData;
	memset(&msgData, 0, sizeof(MESSAGE_DATA));
	CMessageMan::Instance()->SendMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_PTZ_CTRL, (unsigned char *)&ptzCtrl, sizeof(PTZ_CTRL), msgData);

	ReleaseMsgBuff( msgData );

	if ((INVALID_INDEX(unsigned long) == xPos) || (INVALID_INDEX(unsigned long) == yPos))
	{
		PUB_Sleep(50);
		OnStop();
	}

	return KEY_VIRTUAL_PTZ_RIGHTUP;
}



unsigned long CPtzControlDlg::OnDownRight(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	PTZ_CTRL ptzCtrl;
	ptzCtrl.chnn = CMessageMan::Instance()->CurrPTZChnn();
	ptzCtrl.cmdType = PTZ_CMD_RIGHT_DOWN;
	ptzCtrl.speed = m_sliderSpeed.GetPos() + 1;

	MESSAGE_DATA msgData;
	memset(&msgData, 0, sizeof(MESSAGE_DATA));
	CMessageMan::Instance()->SendMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_PTZ_CTRL, (unsigned char *)&ptzCtrl, sizeof(PTZ_CTRL), msgData);

	ReleaseMsgBuff( msgData );

	if ((INVALID_INDEX(unsigned long) == xPos) || (INVALID_INDEX(unsigned long) == yPos))
	{
		PUB_Sleep(50);
		OnStop();
	}

	return KEY_VIRTUAL_PTZ_RIGHTDOWN;
}


unsigned long CPtzControlDlg::OnUpLeft(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	PTZ_CTRL ptzCtrl;
	ptzCtrl.chnn = CMessageMan::Instance()->CurrPTZChnn();
	ptzCtrl.cmdType = PTZ_CMD_LEFT_UP;
	ptzCtrl.speed = m_sliderSpeed.GetPos() + 1;

	MESSAGE_DATA msgData;
	memset(&msgData, 0, sizeof(MESSAGE_DATA));
	CMessageMan::Instance()->SendMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_PTZ_CTRL, (unsigned char *)&ptzCtrl, sizeof(PTZ_CTRL), msgData);

	ReleaseMsgBuff( msgData );

	if ((INVALID_INDEX(unsigned long) == xPos) || (INVALID_INDEX(unsigned long) == yPos))
	{
		PUB_Sleep(50);
		OnStop();
	}

	return KEY_VIRTUAL_PTZ_LEFTUP;
}

unsigned long CPtzControlDlg::OnDownLeft(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	PTZ_CTRL ptzCtrl;
	ptzCtrl.chnn = CMessageMan::Instance()->CurrPTZChnn();
	ptzCtrl.cmdType = PTZ_CMD_LEFT_DOWN;
	ptzCtrl.speed = m_sliderSpeed.GetPos() + 1;

	MESSAGE_DATA msgData;
	memset(&msgData, 0, sizeof(MESSAGE_DATA));
	CMessageMan::Instance()->SendMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_PTZ_CTRL, (unsigned char *)&ptzCtrl, sizeof(PTZ_CTRL), msgData);

	ReleaseMsgBuff( msgData );

	if ((INVALID_INDEX(unsigned long) == xPos) || (INVALID_INDEX(unsigned long) == yPos))
	{
		PUB_Sleep(50);
		OnStop();
	}

	return KEY_VIRTUAL_PTZ_LEFTDOWN;
}


unsigned long CPtzControlDlg::OnNear(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	PTZ_CTRL ptzCtrl;
	ptzCtrl.chnn = CMessageMan::Instance()->CurrPTZChnn();
	ptzCtrl.cmdType = PTZ_CMD_NEAR;
	ptzCtrl.speed = m_sliderSpeed.GetPos() + 1;	

	MESSAGE_DATA msgData;
	memset(&msgData, 0, sizeof(MESSAGE_DATA));
	CMessageMan::Instance()->SendMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_PTZ_CTRL, (unsigned char *)&ptzCtrl, sizeof(PTZ_CTRL), msgData);

	ReleaseMsgBuff( msgData );

	if ((INVALID_INDEX(unsigned long) == xPos) || (INVALID_INDEX(unsigned long) == yPos))
	{
		PUB_Sleep(50);
		OnStop();
	}

	return KEY_VIRTUAL_FOCUS_ADD;
}

unsigned long CPtzControlDlg::OnFar(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	PTZ_CTRL ptzCtrl;
	ptzCtrl.chnn = CMessageMan::Instance()->CurrPTZChnn();
	ptzCtrl.cmdType = PTZ_CMD_FAR;
	ptzCtrl.speed = m_sliderSpeed.GetPos() + 1;

	MESSAGE_DATA msgData;
	memset(&msgData, 0, sizeof(MESSAGE_DATA));
	CMessageMan::Instance()->SendMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_PTZ_CTRL, (unsigned char *)&ptzCtrl, sizeof(PTZ_CTRL), msgData);

	ReleaseMsgBuff( msgData );

	if ((INVALID_INDEX(unsigned long) == xPos) || (INVALID_INDEX(unsigned long) == yPos))
	{
		PUB_Sleep(50);
		OnStop();
	}

	return KEY_VIRTUAL_FOCUS_DEC;
}

unsigned long CPtzControlDlg::OnZoomOut(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	PTZ_CTRL ptzCtrl;
	ptzCtrl.chnn = CMessageMan::Instance()->CurrPTZChnn();
	ptzCtrl.cmdType = PTZ_CMD_ZOOM_OUT;
	ptzCtrl.speed = m_sliderSpeed.GetPos() + 1;

	MESSAGE_DATA msgData;
	memset(&msgData, 0, sizeof(MESSAGE_DATA));
	CMessageMan::Instance()->SendMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_PTZ_CTRL, (unsigned char *)&ptzCtrl, sizeof(PTZ_CTRL), msgData);

	ReleaseMsgBuff( msgData );

	if ((INVALID_INDEX(unsigned long) == xPos) || (INVALID_INDEX(unsigned long) == yPos))
	{
		PUB_Sleep(50);
		OnStop();
	}

	return KEY_VIRTUAL_ZOOM_ADD;
}

unsigned long CPtzControlDlg::OnZoomIn(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	PTZ_CTRL ptzCtrl;
	ptzCtrl.chnn = CMessageMan::Instance()->CurrPTZChnn();
	ptzCtrl.cmdType = PTZ_CMD_ZOOM_IN;
	ptzCtrl.speed = m_sliderSpeed.GetPos() + 1;

	MESSAGE_DATA msgData;
	memset(&msgData, 0, sizeof(MESSAGE_DATA));
	CMessageMan::Instance()->SendMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_PTZ_CTRL, (unsigned char *)&ptzCtrl, sizeof(PTZ_CTRL), msgData);

	ReleaseMsgBuff( msgData );

	if ((INVALID_INDEX(unsigned long) == xPos) || (INVALID_INDEX(unsigned long) == yPos))
	{
		PUB_Sleep(50);
		OnStop();
	}

	return KEY_VIRTUAL_ZOOM_DEC;
}


unsigned long CPtzControlDlg::OnIrisOpen(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	PTZ_CTRL ptzCtrl;
	ptzCtrl.chnn = CMessageMan::Instance()->CurrPTZChnn();
	ptzCtrl.cmdType = PTZ_CMD_IRIS_OPEN;
	ptzCtrl.speed = m_sliderSpeed.GetPos() + 1;

	MESSAGE_DATA msgData;
	memset(&msgData, 0, sizeof(MESSAGE_DATA));
	CMessageMan::Instance()->SendMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_PTZ_CTRL, (unsigned char *)&ptzCtrl, sizeof(PTZ_CTRL), msgData);

	ReleaseMsgBuff( msgData );

	if ((INVALID_INDEX(unsigned long) == xPos) || (INVALID_INDEX(unsigned long) == yPos))
	{
		PUB_Sleep(50);
		OnStop();
	}

	return KEY_VIRTUAL_IRIS_ADD;
}

unsigned long CPtzControlDlg::OnIrisClose(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	PTZ_CTRL ptzCtrl;
	ptzCtrl.chnn = CMessageMan::Instance()->CurrPTZChnn();
	ptzCtrl.cmdType = PTZ_CMD_IRIS_CLOSE;
	ptzCtrl.speed = m_sliderSpeed.GetPos() + 1;

	MESSAGE_DATA msgData;
	memset(&msgData, 0, sizeof(MESSAGE_DATA));
	CMessageMan::Instance()->SendMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_PTZ_CTRL, (unsigned char *)&ptzCtrl, sizeof(PTZ_CTRL), msgData);

	ReleaseMsgBuff( msgData );

	if ((INVALID_INDEX(unsigned long) == xPos) || (INVALID_INDEX(unsigned long) == yPos))
	{
		PUB_Sleep(50);
		OnStop();
	}

	return KEY_VIRTUAL_IRIS_DEC;
}


unsigned long CPtzControlDlg::OnStop()
{
	PTZ_CTRL ptzCtrl;
	ptzCtrl.chnn = CMessageMan::Instance()->CurrPTZChnn();
	ptzCtrl.cmdType = PTZ_CMD_STOP;

	MESSAGE_DATA msgData;
	memset(&msgData, 0, sizeof(MESSAGE_DATA));
	CMessageMan::Instance()->SendMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_PTZ_CTRL, (unsigned char *)&ptzCtrl, sizeof(PTZ_CTRL), msgData);
	
	ReleaseMsgBuff( msgData );

	return KEY_VIRTUAL_PTZ_STOP;
}


//响应摇杆左旋,增加云台速度
unsigned long CPtzControlDlg::OnPTZSpeedAdd()
{
	int maxSpeed = m_sliderSpeed.GetRangeMax();
	int curSpeed = m_sliderSpeed.GetPos();

	curSpeed++;

	if( curSpeed <= maxSpeed )
	{
		m_sliderSpeed.SetCurPos( curSpeed, true );
	}

	SpeedChange();

	return KEY_VIRTUAL_MSG_STOP;
}

//响应摇杆右旋，减少云台速度
unsigned long CPtzControlDlg::OnPTZSpeedDec()
{
	int minSpeed = m_sliderSpeed.GetRangeMin();
	int curSpeed = m_sliderSpeed.GetPos();

	curSpeed--;
	if( curSpeed >= minSpeed )
	{
		m_sliderSpeed.SetCurPos( curSpeed, true );
	}

	SpeedChange();

	return KEY_VIRTUAL_MSG_STOP;
}

void CPtzControlDlg::SpeedChange()
{
	int speed = m_sliderSpeed.GetPos();
	const char* pStr = m_pStrTable->String(E_STR_ID_SPEED);
	char sz[8] = {0};
	sprintf(sz, ": %d", speed + 1);
	std::string str = pStr;
	str += sz;
	m_scSpeed.SetCaption(str.c_str(), true);
	CMessageMan::Instance()->SetCurrPTZSpeed( m_sliderSpeed.GetPos() + 1 );
}

//操作云台时鼠标弹起要发停止命令
unsigned long CPtzControlDlg::OnMouseLftBttnUp( unsigned long ctrlID )
{
	return OnStop();

}

bool CPtzControlDlg::ReleaseMsgBuff(MESSAGE_DATA &msgData)
{
	assert( NULL != msgData.pData );
	PACKCMD * pPackCmd = reinterpret_cast<PACKCMD*>(msgData.pData);
	if( CMD_REPLY_PTZ_SEND_CMD_SUC == pPackCmd->cmdType )
	{
		CMessageMan::Instance()->ReleaseMsgBuff( msgData );
		return true;
	}
	else if( CMD_REPLY_PTZ_SEND_CMD_FAIL )
	{
		unsigned long ptzValue = *reinterpret_cast<unsigned long *>(msgData.pData + sizeof(PACKCMD));
		ENUM_STRING_ID strID = CPTZViewDlg::GetPtzCtrlStrID(ptzValue);
		MessageOut( GetTopestWnd(), strID, TIP_MSG );

		CMessageMan::Instance()->ReleaseMsgBuff( msgData );
		return false;
	}
}
//end




