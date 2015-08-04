/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : ZXX
** Date         : 2013-11-27
** Name         : WzdNatEndDlg.cpp
** Version      : 1.0
** Description  : 穿透向导设备结束页面对话框
** Modify Record:
***********************************************************************/

#include "WzdNatEndDlg.h"
#include "DrawManager.h"
CWzdNatEndDlg::CWzdNatEndDlg()
{
	m_toprow = 0;
}

CWzdNatEndDlg::~CWzdNatEndDlg()
{

}

void CWzdNatEndDlg::SetWzdInfo(NAT_WIZARD_INFO* pWzdInfo)
{
	m_pWzdInfo = pWzdInfo;
}

void CWzdNatEndDlg::ShowWzdData(bool bRefresh)
{
	if(1 == m_pWzdInfo->ShowWzd)
	{
		m_checkCtrl.SetCheck(true,false);
	}
	else
	{
		m_checkCtrl.SetCheck(false,false);
	}
}

void CWzdNatEndDlg::GetWzdData(bool bNext)
{

}
void CWzdNatEndDlg::OnLeavePage()
{

}

void CWzdNatEndDlg::SetTipInfo(const std::string& str)
{
	m_richTextCtrl.SetText(str, false);
	unsigned short maxTextRow = m_richTextCtrl.GetTextRow()-1;
	if (800 >= m_width)//800x600以上分辨率不需要滚动条
	{
		m_scroll_v.SetRange(0,maxTextRow);
		m_scroll_v.SetCurPos(0,false);
	}
}


void CWzdNatEndDlg::OnInitial()
{
	CWzdNatPageDlg::OnInitial();
	unsigned long x = m_length[DLG_OFFSET_SUB_LEFT];
	unsigned long y = m_length[DLG_OFFSET_SUB_TOP];
	unsigned long height = m_rect.Height() - 2 * y - m_length[CHECK_HEIGHT];
	unsigned long cx = m_rect.Width() -  2 * x - CScrollCtrl::m_length[SCROLL_WIDTH] - 5;
	unsigned long cy = (m_rect.Height() - 2 * y)/2+2*m_length[STATIC_HEIGHT]+5-m_length[BTN_WIDTH];
	m_richTextCtrl.Create(GetRandWndID(), x, y, cx, cy, this);
	if (800 >= m_width)//800x600以上分辨率不需要滚动条
	{
		m_richTextCtrl.SetScrollShow(true);
		m_scroll_v.Create(GetRandWndID(), x+cx,y,cy-5,this,0,1,false);
	}


	m_drawX = x+15;
	m_drawY = y+cy+m_length[BTN_WIDTH];

	x = m_length[DLG_OFFSET_SUB_LEFT];
	y = m_length[DLG_OFFSET_SUB_TOP]+height;
	cx = m_rect.Width() -  2 * x;
	cy = m_length[CHECK_HEIGHT];
	m_checkCtrl.Create(GetRandWndID(),x,y,cx,cy,this,0,2);
	m_checkCtrl.SetCaption(m_pStrTable->String(E_STR_ID_US02_WZD_OPENAGAIN),false,false);
}

void CWzdNatEndDlg::OnDestroy()
{
	CWzdNatPageDlg::OnDestroy();
}
void CWzdNatEndDlg::InitCtrlInfo()
{

}

void CWzdNatEndDlg::OnPaint(PAINT_STATUS status)
{
	GUI::CDialog::OnPaint(status);

	unsigned long ArrowX = (m_rect.Width() -  2 * m_length[DLG_OFFSET_SUB_LEFT])/2;
	unsigned long x = m_rect.m_left+m_length[DLG_OFFSET_LEFT]+m_drawX+ArrowX;
	unsigned long y  = m_rect.m_top+m_length[DLG_OFFSET_TOP]+m_drawY;
	unsigned long ArrowY = 0;
#ifdef __DVR_ANCHOR__
	ArrowY = y-15;
#else
	ArrowY = y;
#endif
	//图片INFO
	GUI::CRect picInfoRect(x-24-m_length[BTN_WIDTH],y-m_length[BTN_WIDTH]/2,m_length[BTN_WIDTH],m_length[BTN_WIDTH]);
#ifdef __DVR_ANCHOR__
	CWnd::DrawImage(BMP_ANCHOR_INFO, picInfoRect.m_left, picInfoRect.m_top);
#else
	CWnd::DrawImage(BMP_INFO, picInfoRect.m_left, picInfoRect.m_top);
#endif	

	//第一个箭头
	DrawTriangle(x,ArrowY-m_length[BTN_WIDTH]/2);
	//图片BMP_QRCODE
#ifdef __SUPPORT_SHOW_QRCODE__
	GUI::CRect qrRect(x,y-m_length[BTN_WIDTH],m_length[BTN_WIDTH],m_length[BTN_WIDTH]);
	CWnd::DrawImage(BMP_QRCODE, qrRect.m_left, qrRect.m_top);
#endif

	//第二个箭头
	DrawTriangle(x,ArrowY+m_length[BTN_WIDTH]/2);
	//图片BMP_NETWORK
	GUI::CRect netWorkRect(x,y,m_length[BTN_WIDTH],m_length[BTN_WIDTH]);
#ifdef __DVR_ANCHOR__
	CWnd::DrawImage(BMP_ANCHOR_NETWORK, netWorkRect.m_left, netWorkRect.m_top);
#else
	CWnd::DrawImage(BMP_NETWORK, netWorkRect.m_left, netWorkRect.m_top);
#endif

}

void CWzdNatEndDlg::DrawTriangle(int x,int y)
{
	//箭头三角形上半部分,由小矩形拼成
	int i=0,j=0;
	for(i=0;i<12;i++)
	{
		for(j=0;j<i+1;j++)
		{
			GUI::CWnd::DrawRect(x-1*i-12, y-1*j, 1, 1, COLOR_LINE_WHITE);
		}
	}

	//箭头三角形下半部分
	for(i=0;i<12;i++)
	{
		for(j=0;j<i+1;j++)
		{
			GUI::CWnd::DrawRect(x-1*i-12, y+1*j, 1, 1, COLOR_LINE_WHITE);
		}
	}

	//补充箭头的后半部分矩形
	GUI::CWnd::DrawRect(x-36, y-3, 12, 6, COLOR_LINE_WHITE);
}

BEGIN_MSG_MAP(CWzdNatEndDlg, CWzdNatPageDlg)
	ON_MSG(m_checkCtrl.GetID(), KEY_VIRTUAL_ENTER, OnCheck)
	ON_MSG_EX(m_scroll_v.GetID(), KEY_VIRTUAL_MSG_DEFAULT_TAG, OnScrollchange)
	ON_MSG_EX(m_scroll_v.GetID(),KEY_VIRTUAL_ENTER, OnScrollchange)
	ON_MSG_EX(m_scroll_v.GetID(),KEY_VIRTUAL_DRAG, OnScrollchange)
	ON_MSG_EX(m_scroll_v.GetID(),KEY_VIRTUAL_BLUP, OnScrollchange)
END_MSG_MAP()

unsigned long  CWzdNatEndDlg::OnCheck()
{
	if(m_checkCtrl.IsCheck())
	{
		m_pWzdInfo->ShowWzd = 1;
	}
	else
	{
		m_pWzdInfo->ShowWzd = 0;
	}
	return KEY_VIRTUAL_STOP;
}

unsigned long CWzdNatEndDlg::OnScrollchange(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	int pos = m_scroll_v.GetPos();
	if (pos != m_toprow)
	{
		m_toprow = pos;
		m_richTextCtrl.SetCurRow(m_toprow);
		Repaint();
	}

	return KEY_VIRTUAL_MSG_STOP; 
}
