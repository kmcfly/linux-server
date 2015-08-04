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
#include "ZoomDlg.h"
#include "MessageMan.h"

extern NET_LOGIN_SUCCESS_REPLY *g_p_login_succ;

//////////////////////////////////////////////////////////////////////////
CZoomDlg::CZoomDlg() : m_x(0), m_y(0), m_bHideZoomPanel(false)
{
	m_bIsInZoom = false;
	m_zoomParm = 0;
}

CZoomDlg::~CZoomDlg()
{

}

void CZoomDlg::OnInitial()
{
	GUI::CDialog::OnInitial();
	m_style |= WND_NO_TITLE;

	//////////////////////////////////////////////////////////////////////////
	int cx = ((g_p_login_succ->productInfo.videoInputNum + 1 )/2)*25+80, cy = 60;
	int x = m_width - cx - 5, y = m_height - cy - 5;
	if (m_width < 800)
	{
		x -= 40;
		y -= 20;
	}

	m_panel.Create(GetRandWndID(), x, y, cx, cy, this, 0, 0);
	m_panel.Show(!m_bHideZoomPanel, false);

	m_bIsInZoom = true;
}

void CZoomDlg::OnDestroy()
{
	GUI::CDialog::OnDestroy();
	
	m_bIsInZoom = false;
	m_zoomParm = 0;
}

BEGIN_MSG_MAP(CZoomDlg, GUI::CDialog)
	ON_MSG_EX(GetID(), KEY_VIRTUAL_DRAG, OnMove)
	ON_MSG_EX(GetID(), KEY_VIRTUAL_ENTER, OnEnter)
	
	ON_MSG(GUI::CTRLID_NULL, KEY_VIRTUAL_ADD,	OnMouseWheelAdd)
	ON_MSG(GUI::CTRLID_NULL, KEY_VIRTUAL_DEC,	OnMouseWheelDec)	
END_MSG_MAP()


//////////////////////////////////////////////////////////////////////////
void CZoomDlg::OnPaint(GUI::PAINT_STATUS status)
{
	//拦截绘制，保证不把现场分割线等刷掉。
}

void CZoomDlg::SetCHInfo(unsigned char chnn, ULONGLONG authCH, bool bHidePanel)
{
	m_panel.SetCHInfo(chnn, authCH);

	m_bHideZoomPanel = bHidePanel;
}

unsigned long CZoomDlg::OnEnter(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	//鼠标控制
	if ((xPos < m_width) && (yPos < m_height))
	{
		m_x = xPos;
		m_y = yPos;
	}

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CZoomDlg::OnMove(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	//鼠标控制
	if ((xPos < m_width) && (yPos < m_height) && ((m_x != xPos) || (m_y != yPos)))
	{
		if (IsAlmostAbove(m_x, m_y, xPos, yPos, 400))
		{
			m_panel.OnMove(m_x, m_y, xPos, yPos);

			m_x = xPos;
			m_y = yPos;
		}
	}

	return KEY_VIRTUAL_MSG_STOP;
}

bool CZoomDlg::IsAlmostAbove(unsigned long x1, unsigned long y1, unsigned long x2, unsigned long y2, unsigned long distance)
{
	unsigned long x = (x1 >= x2) ? (x1 - x2) : (x2 - x1);
	unsigned long y = (y1 >= y2) ? (y1 - y2) : (y2 - y1);

	if(distance <= ((x*x) + (y*y)))
	{
		return true;
	}
	else
	{
		return false;
	}
}
unsigned long CZoomDlg::OnMouseWheelAdd()
{
	DealWheelZoomMsg(true);
	return 0;
}
unsigned long CZoomDlg::OnMouseWheelDec()
{
	DealWheelZoomMsg(false);
	return 0;
}
void CZoomDlg::DealWheelZoomMsg(bool bZoomOut)
{
	ZOOM_INFO zoomInfo;
	m_panel.GetZoomXYinfo(zoomInfo);
	
	if(bZoomOut)
	{	
		if((m_zoomParm<=3.900000)&&(m_zoomParm>=1))
		{
			m_zoomParm +=0.1;
		}
		else if(m_zoomParm <1.00000)
		{
			m_zoomParm =1;
		}
		else
		return;
	}
	else
	{
		if(m_zoomParm>=1.10000)
		{
			m_zoomParm -=0.1;
			if(zoomInfo.x +m_width/ m_zoomParm > m_width)
			{
				zoomInfo.x  = m_width -m_width/ m_zoomParm;
			}
			if(zoomInfo.y + m_height/ m_zoomParm > m_height)
			{
				zoomInfo.y = m_height-m_height/m_zoomParm;
			}
		}
		else 
		return;
	}
	zoomInfo.width = m_width/ m_zoomParm;
	zoomInfo.height = m_height / m_zoomParm;
	if((m_zoomParm >= 1.0)&&(m_zoomParm<1.1))
	{
		m_zoomParm =1.0;
		zoomInfo.x=0;
		zoomInfo.y=0;
		zoomInfo.width = m_width;
		zoomInfo.height = m_height;
	}
	m_panel.SetZoomWHinfo(zoomInfo);
	CMessageMan::Instance()->PostMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_ZOOM, (unsigned char *)&zoomInfo, sizeof(ZOOM_INFO));
}
//end
