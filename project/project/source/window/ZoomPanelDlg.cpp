/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : YSW
** Date         : 2010-07-1
** Name         : ZoomDlg.cpp
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/
#include "ZoomPanelDlg.h"
#include "MessageMan.h"

extern NET_LOGIN_SUCCESS_REPLY *g_p_login_succ;

//////////////////////////////////////////////////////////////////////////
CZoomPanelDlg::CZoomPanelDlg() : m_channel(0), m_offsetX(0), m_offsetY(0), m_authCH(0), m_pbtnCH(NULL)
{
	memset(&m_zoomInfo, 0, sizeof(ZOOM_INFO));
}

CZoomPanelDlg::~CZoomPanelDlg()
{

}

void CZoomPanelDlg::OnInitial()
{
	GUI::CDialog::OnInitial();
	m_style |= WND_NO_TITLE;
	//////////////////////////////////////////////////////////////////////////
	m_pbtnCH = new GUI::CButton [g_p_login_succ->productInfo.videoInputNum];
	assert (NULL != m_pbtnCH);
	assert (0 != m_authCH);
	assert (m_channel < g_p_login_succ->productInfo.videoInputNum);

	unsigned long num = (g_p_login_succ->productInfo.videoInputNum + 1) / 2;
	unsigned long x = (m_rect.Width() - 40), y = 8, cx = 25, cy = 24, xFocus = 0, yFocus = 0;
	char str [8] = {0};


	for (int i=0; i<g_p_login_succ->productInfo.videoInputNum; ++i)
	{
		xFocus = i % num;
		yFocus = ((i/num )> 0) ? 2 : 0;

		sprintf(str, "%d", i+1);
		m_pbtnCH[i].Create(GetRandWndID(), (i%num)*cx+10, (i/num)*cy+6, cx, cy, this, xFocus, yFocus, str, WND_CHILD|WND_VISIBLE|WND_NO_IMAGE_BK);

		if (0 == (m_authCH & (0x01 << i)))
		{
			m_pbtnCH[i].SetEnable(false, false);
		}
	}

	m_pbtnCH[m_channel].SetMark(true);

	m_btnUp.Create(GetRandWndID(), x, y, cx, cy, this, num  + 1, 0);
	m_btnUp.SetImage(PAINT_STATUS_NORMAL, BMP_PTZ_UP, false);
	m_btnUp.SetImage(PAINT_STATUS_FOCUS, BMP_PTZ_UP_F, false);
	x += 15; y += 15;
	m_btnRight.Create(GetRandWndID(), x, y, cx, cy, this, num + 1, 1);
	m_btnRight.SetImage(PAINT_STATUS_NORMAL, BMP_PTZ_RIGHT, false);
	m_btnRight.SetImage(PAINT_STATUS_FOCUS, BMP_PTZ_RIGHT_F, false);
	x -= 15; y += 15;
	m_btnDown.Create(GetRandWndID(), x, y, cx, cy, this, num  + 1, 2);
	m_btnDown.SetImage(PAINT_STATUS_NORMAL, BMP_PTZ_DOWN, false);
	m_btnDown.SetImage(PAINT_STATUS_FOCUS, BMP_PTZ_DOWN_F, false);
	x -= 15; y -= 15;
	m_btnLeft.Create(GetRandWndID(), x, y, cx, cy, this, num, 1);
	m_btnLeft.SetImage(PAINT_STATUS_NORMAL, BMP_PTZ_LEFT, false);
	m_btnLeft.SetImage(PAINT_STATUS_FOCUS, BMP_PTZ_LEFT_F, false);

	m_zoomInfo.x = 0;
	m_zoomInfo.y = 0;
	m_zoomInfo.width = m_width;// / 2;
	m_zoomInfo.height = m_height ;/// 2;
	m_offsetX = m_zoomInfo.width / 16;
	m_offsetY = m_zoomInfo.height / 32;

//	CMessageMan::Instance()->PostMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_ZOOM, (unsigned char *)&m_zoomInfo, sizeof(ZOOM_INFO));
}

void CZoomPanelDlg::OnDestroy()
{
	memset(&m_zoomInfo, 0, sizeof(ZOOM_INFO));
	CMessageMan::Instance()->PostMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_ZOOM, (unsigned char *)&m_zoomInfo, sizeof(ZOOM_INFO));

	GUI::CDialog::OnDestroy();

	delete [] m_pbtnCH;
	m_pbtnCH = NULL;
}

BEGIN_MSG_MAP(CZoomPanelDlg, GUI::CDialog)
	ON_MSG_CTRL_RANGE(m_pbtnCH[0].GetID(), m_pbtnCH[g_p_login_succ->productInfo.videoInputNum-1].GetID(), KEY_VIRTUAL_ENTER, OnCH)
	ON_MSG(m_btnUp.GetID(), KEY_VIRTUAL_ENTER, OnUp)
	ON_MSG(m_btnRight.GetID(), KEY_VIRTUAL_ENTER, OnRight)
	ON_MSG(m_btnDown.GetID(), KEY_VIRTUAL_ENTER, OnDown)
	ON_MSG(m_btnLeft.GetID(), KEY_VIRTUAL_ENTER, OnLeft)
	ON_MSG(CTRLID_NULL, KEY_VIRTUAL_UP, OnUp)
	ON_MSG(CTRLID_NULL, KEY_VIRTUAL_RIGHT, OnRight)
	ON_MSG(CTRLID_NULL, KEY_VIRTUAL_DOWN, OnDown)
	ON_MSG(CTRLID_NULL, KEY_VIRTUAL_LEFT, OnLeft)
END_MSG_MAP()

//////////////////////////////////////////////////////////////////////////
void CZoomPanelDlg::OnMove(unsigned long x1, unsigned y1, unsigned long x2, unsigned y2)
{
//	printf("x1:%d, y1:%d, x2:%d, y2:%d\n", x1, y1, x2, y2);
	int x = m_zoomInfo.x;
	if (x1 < x2)
	{
		int left = 0;
		int right = x - ((x2 - x1) / 2);
		x = (left <= right) ? right : left;
	}
	else
	{
		int left = x + ((x1 - x2) / 2);
		int right = m_width - m_zoomInfo.width;
		x = (left <= right) ? left : right;
	}

	int y = m_zoomInfo.y;
	if (y1 < y2)
	{
		int top = 0;
		int bottom = y - ((y2 - y1) / 2);
		y = (top <= bottom) ? bottom : top;
	}
	else
	{
		int top = y + ((y1 - y2) / 2);
		int bottom = m_height - m_zoomInfo.height;
		y = (top <= bottom) ? top : bottom;
	}

	if ((x != m_zoomInfo.x) || (y != m_zoomInfo.y))
	{
	//	printf("x:%d, y:%d, m_zoomInfo.x:%d, m_zoomInfo.y:%d,width=%ld..height=%ld..\n", x, y, m_zoomInfo.x, m_zoomInfo.y,m_zoomInfo.width,m_zoomInfo.height);
		m_zoomInfo.x = x;
		m_zoomInfo.y = y;

		CMessageMan::Instance()->PostMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_ZOOM, (unsigned char *)&m_zoomInfo, sizeof(ZOOM_INFO));
	}
}

unsigned long CZoomPanelDlg::OnCH(int ctrlID)
{
	unsigned char chnn = ctrlID - m_pbtnCH[0].GetID();
	if (m_channel != chnn)
	{
		m_pbtnCH[m_channel].SetMark(false);

		m_channel = chnn;
		m_pbtnCH[m_channel].SetMark(true);

		assert (m_channel < g_p_login_succ->productInfo.videoInputNum);

		//先退出电子放大
		unsigned short x = m_zoomInfo.x;
		unsigned short y = m_zoomInfo.y;
		unsigned short width = m_zoomInfo.width;
		unsigned short height = m_zoomInfo.height;
		
		memset(&m_zoomInfo, 0, sizeof(ZOOM_INFO));
		CMessageMan::Instance()->PostMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_ZOOM, (unsigned char *)&m_zoomInfo, sizeof(ZOOM_INFO));
		
		//切换到单通道大画面
		NET_DISPLAY_INFO disp;
		memset(&disp, 0, sizeof(NET_DISPLAY_INFO));
		disp.type = NET_DISPLAY_CHANGE_DATA_PACK;
		disp.rec = 0;
		disp.time_out = -1;
		disp.split_mode[DISPLAY_LAYER_ANALOG] = VIEW_SPLIT_1X1;
		disp.split_mode[DISPLAY_LAYER_NET] = VIEW_SPLIT_1X1;

		int localVideoInputNum = g_p_login_succ->productInfo.localVideoInputNum;
		if(m_channel < localVideoInputNum)
		{
			disp.data[0] = DISPLAY_PACK(DISPLAY_LAYER_ANALOG, DISPLAY_SIGNAL_ANALOG, 0, m_channel);
		}
		else
		{
			disp.data[0] = DISPLAY_PACK(DISPLAY_LAYER_NET, DISPLAY_SIGNAL_NET, 0, m_channel - localVideoInputNum);
		}
		disp.data_len = 1;

		MESSAGE_DATA msgData;
		memset(&msgData, 0, sizeof(MESSAGE_DATA));
		CMessageMan::Instance()->SendMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_CHANGE_DISPLAY_MODE, (unsigned char*)&disp, sizeof(NET_DISPLAY_INFO), msgData);	
		CMessageMan::Instance()->ReleaseMsgBuff(msgData);

		//开启电子放大
		m_zoomInfo.x = x;
		m_zoomInfo.y = y;
		m_zoomInfo.width = width;
		m_zoomInfo.height = height;
		m_offsetX = m_zoomInfo.width / 16;
		m_offsetY = m_zoomInfo.height / 32;
		CMessageMan::Instance()->PostMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_ZOOM, (unsigned char *)&m_zoomInfo, sizeof(ZOOM_INFO));

		GetRootWnd()->Repaint();
	}

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CZoomPanelDlg::OnUp()
{
	if ((m_offsetY <= m_zoomInfo.y)&&(m_zoomInfo.height != m_height))
	{
		m_zoomInfo.y -= m_offsetY;
		CMessageMan::Instance()->PostMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_ZOOM, (unsigned char *)&m_zoomInfo, sizeof(ZOOM_INFO));
	}

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CZoomPanelDlg::OnRight()
{
	if ((m_zoomInfo.width != m_width)&&(m_zoomInfo.x+m_zoomInfo.width+m_offsetX<m_width))
	{
		m_zoomInfo.x += m_offsetX;
		CMessageMan::Instance()->PostMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_ZOOM, (unsigned char *)&m_zoomInfo, sizeof(ZOOM_INFO));
	}

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CZoomPanelDlg::OnDown()
{
	if ((m_zoomInfo.height != m_height)&&(m_zoomInfo.y+m_zoomInfo.height+m_offsetY <m_height))
	{
		m_zoomInfo.y += m_offsetY;
		CMessageMan::Instance()->PostMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_ZOOM, (unsigned char *)&m_zoomInfo, sizeof(ZOOM_INFO));
	}

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CZoomPanelDlg::OnLeft()
{
	if ((m_offsetX <= m_zoomInfo.x)&&(m_zoomInfo.width != m_width))
	{
		m_zoomInfo.x -= m_offsetX;
		CMessageMan::Instance()->PostMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_ZOOM, (unsigned char *)&m_zoomInfo, sizeof(ZOOM_INFO));
	}

	return KEY_VIRTUAL_MSG_STOP;
}
//end
