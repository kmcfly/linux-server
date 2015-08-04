/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author		: L
** Date			: 2009-10-19
** Name			: DwellModeDlgNXN.cpp
** Version		: 1.0
** Description	: 
** Modify Record:
1:
***********************************************************************/

#include "DwellModeDlgNXN.h"

//////////////////////////////////////////////////////////////////////////
// NXN
//////////////////////////////////////////////////////////////////////////
CDwellModeDlgNXN::CDwellModeDlgNXN()
{
	m_layoutType = TYPE_1X1;
	m_maxChnn = 0;

	for ( int i=0;i<72;i++)
	{
		m_comboChnn[i] = NULL;
	}

}

CDwellModeDlgNXN::~CDwellModeDlgNXN()
{

}

void CDwellModeDlgNXN::OnInitial()
{
	m_style |= WND_CHILD;
	GUI::CScrollDialog::OnInitial();
	//////////////////////////////////////////////////////////////////////////

	unsigned short width  = m_rect.Width();
	unsigned short height = m_rect.Height();

	GUI::CScrollWnd& wnd = GetScrollWnd();

	wnd.SetSizeOffset(width,height);
	wnd.SetViewMaxSize(width,height);

	for (int i=0; i < 36; i++)
	{
		GUI::CRect rect(0, 0, 120, m_length[COMBO_HEIGHT]);
		m_comboChnn[i] = dynamic_cast<GUI::CComboCtrl*>(wnd.AddWnd(WND_TYPE_COMBO, GetRandWndID(), rect, i + 1, 0, i));
		wnd.ShowSubWnd(i, false);
		InitComboChnn(*m_comboChnn[i]);
	}

	for (int i=36; i< 72; i++)
	{
		if(i == 71)
		{
			i = 71;
		}
		GUI::CRect rect(140, 0, 140 + 90, m_length[COMBO_HEIGHT]);
		m_comboChnn[i] = dynamic_cast<GUI::CComboCtrl*>(wnd.AddWnd(WND_TYPE_COMBO, GetRandWndID(), rect, i+1, 0, i));
		wnd.ShowSubWnd(i, false);
		InitCombo2Chnn(*m_comboChnn[i]);
	}
	//AutoLayout(TYPE_1X1);
}

void CDwellModeDlgNXN::OnPaint(PAINT_STATUS status)
{
	GUI::CScrollDialog::OnPaint(status);
}

void CDwellModeDlgNXN::OnChangeFocus(unsigned long oldID, unsigned long newID)
{

}

void CDwellModeDlgNXN::SetLayoutType(LAYOUT_TYPE type, bool bRefresh)
{
	m_layoutType = type;

	AutoLayout(type);

	SetCurScrollPos(0, 0, false);

	if(bRefresh)
	{
		Repaint();
	}
}

void CDwellModeDlgNXN::SetChnnData(unsigned long* pChnn,int numChnn,bool bRefresh)
{
	//assert(numChnn<=m_maxChnn);
	for (int i = 0; i < numChnn; i++)
	{
		if(pChnn[i] >= m_maxChnn)
		{
			m_comboChnn[i]->SetCurItemData(-1, bRefresh);
			m_comboChnn[i + 36]->SetCurItemData(-1, bRefresh);
		}
		else
		{
			m_comboChnn[i]->SetCurItemData(pChnn[i], bRefresh);
			m_comboChnn[i + 36]->SetCurItemData(pChnn[i], bRefresh);
		}
	}
}

void CDwellModeDlgNXN::InitComboChnn(GUI::CComboCtrl& combo )
{
	char str[64]={0};
	for ( int i=0;i<m_maxChnn;i++)
	{
		snprintf(str, 64, "%s %d", m_pStrTable->String(E_STR_ID_CHANNEL), i + 1);
		combo.AddItem( str,i);
	}

	combo.AddItem(m_pStrTable->String(E_STR_ID_NONE), -1);

	combo.SetCurItemData(0, false);
}

void CDwellModeDlgNXN::InitCombo2Chnn(GUI::CComboCtrl& combo )
{
	char str[64]={0};
	for ( int i=0;i<m_maxChnn;i++)
	{
		snprintf(str, 64, "    %d", i + 1);

		combo.AddItem( str,i);
	}

	combo.AddItem(m_pStrTable->String(E_STR_ID_NONE), -1);

	combo.SetCurItemData(0, false);
}

void CDwellModeDlgNXN::AutoLayout(LAYOUT_TYPE type)
{
	/*
		重新布局页面元素位置
	*/

	GUI::CScrollWnd& wnd = GetScrollWnd();

	//设置所有页面元素不显示
	for (int i = 0; i < 72; i++)
	{
		wnd.ShowSubWnd(i, false);
	}

	//清除所有绘制边框
	ClearAllDrawBorderPos();

	switch (type)
	{
	case TYPE_1X1:
		{
			AutoSetNXN(1, 1);
		}
		break;
	case TYPE_2X2:
		{
			AutoSetNXN(2, 2);
		}
		break;
	case TYPE_2X3:
		{
			AutoSetNXN(2, 3);
		}
		break;
	case TYPE_3X3:
		{
			AutoSetNXN(3, 3);
		}
		break;
	case TYPE_4X4:
		{
			AutoSetNXN(4, 4);
		}
		break;
	case TYPE_5X5:
		{
			AutoSetNXN(5, 5);
		}
		break;
	case TYPE_6X6:
		{
			AutoSetNXN(6, 6);
		}
		break;
	default:
		assert(false);
		break;
	}
}

void CDwellModeDlgNXN::AutoSetNXN(int row, int col)
{
	GUI::CScrollWnd& wnd = GetScrollWnd();

	unsigned short width  = m_rect.Width() / col;
	unsigned short height = m_rect.Height() / row;

#ifndef __DVR_ANCHOR__
	wnd.SetSizeOffset(width, height);
#else
	wnd.SetSizeOffset(width, height + 1);
#endif
	wnd.SetViewMaxSize(width * col, height * row);

	
	if (width < 125)
	{
		for (int i = 0; i < row * col; i++)
		{
			unsigned long x, y, cx, cy;
			GUI::CRect rect(140, 0, 140 + 90, m_length[COMBO_HEIGHT]);
			wnd.GetViewPos(i % col, i / col, x, y);
			wnd.GetViewSize(i % col, i / col, cx, cy);
			rect.Offset(x + (cx - 90) / 2 - 140, y + (cy - m_length[COMBO_HEIGHT]) / 2);
			wnd.ResetWndRect(i+36, rect);
			wnd.ShowSubWnd(i+ 36, true);

			AddDrawBorderPos(i % col, i / col);
		}
	}
	else
	{
		for (int i = 0; i < row * col; i++)
		{
			unsigned long x, y, cx, cy;
			GUI::CRect rect(0, 0, 120, m_length[COMBO_HEIGHT]);
			wnd.GetViewPos(i % col, i / col, x, y);
			wnd.GetViewSize(i % col, i / col, cx, cy);
			rect.Offset(x + (cx - 120) / 2, y + (cy - m_length[COMBO_HEIGHT]) / 2);
			wnd.ResetWndRect(i, rect);
			wnd.ShowSubWnd(i, true);

			AddDrawBorderPos(i % col, i / col);
		}
	}
}

// end

