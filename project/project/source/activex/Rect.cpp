/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : hz
** Date         : 2009-07-10
** Name         : Rect.cpp
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/
#include "Rect.h"

GUI::CRect::CRect(void) : m_left(0)
, m_right(0)
, m_top(0)
, m_bottom(0)
{
}

GUI::CRect::CRect(int left, int top, int right, int bottom) : m_left(left)
, m_right(right)
, m_top(top)
, m_bottom(bottom)
{
}

GUI::CRect::~CRect(void)
{
}

void GUI::CRect::SetRect(int left, int top, int right, int bottom)
{
	m_left = left;
	m_top = top;
	m_right = right;
	m_bottom = bottom;
}

int GUI::CRect::Width() const
{ 
	return m_right - m_left + 1; 
}

int GUI::CRect::Height() const
{ 
	return m_bottom - m_top + 1; 
}

bool GUI::CRect::PtInRect(int x, int y) const
{ 
	if ((m_left <= x) && (x <= m_right) &&
		(m_top <= y) && (y <= m_bottom))
	{
		return true;
	}
	else
	{
		return false;
	}
}

void GUI::CRect::Offset(int x, int y)
{ 
	m_left += x;
	m_top += y;
	m_right += x;
	m_bottom += y;
}

void GUI::CRect::Infrate(int x, int y)
{ 
	m_left -= x;
	m_top -= y;
	m_right += x;
	m_bottom += y;
}

void GUI::CRect::Defrate(int x, int y)
{ 
	Infrate(-x, -y);
}


void GUI::CRect::MoveToY(int y)
{ 
	m_bottom = Height() + y - 1; 
	m_top = y;
}

void GUI::CRect::MoveToX(int x)
{ 
	m_right = Width() + x - 1; 
	m_left = x; 
}

void GUI::CRect::MoveToXY(int x, int y)
{ 
	MoveToX(x); 
	MoveToY(y);
}

void GUI::CRect::Modify(int x, int y, int cx, int cy)
{
	if (m_left >= (x + cx))
	{
		m_left = (x + cx - 1);
		m_right = m_left - 1;
	}
	else if (m_top >= (y + cy))
	{
		m_top = (y + cy - 1);
		m_bottom = m_top - 1;
	}
	else
	{
		if (m_left < x)
		{
			m_left = (x < m_right) ? x : m_right;
		}

		if (m_top < y)
		{
			m_top = (y < m_bottom) ? y : m_bottom;
		}

		if ((x + cx) < m_right)
		{
			m_right = (x + cx - 1);
		}

		if ((y + cy) < m_bottom)
		{
			m_bottom = (y + cy - 1);
		}
	}
}

GUI::CRect GUI::CRect::GetCenterRect(int cx, int cy) const
{
	int left = (Width() - cx)/2;
	int top = (Height() - cy)/2;

	return CRect(left, top, left + cx - 1, top + cy - 1);
}

void GUI::CRect::SetSize(int cx, int cy)
{
	m_right = m_left + cx - 1;
	m_bottom = m_top + cy - 1;
}

bool GUI::CRect::IntersectRect(const CRect& cmp, CRect* outRect)
{
	int x_left = (m_left >= cmp.m_left) ? m_left : cmp.m_left;
	int y_top = (m_top >= cmp.m_top) ? m_top : cmp.m_top;
	int x_right = (m_right >= cmp.m_right) ? cmp.m_right : m_right;
	int y_bottom = (m_bottom >= cmp.m_bottom) ? cmp.m_bottom : m_bottom;

	if(x_right >= x_left && y_bottom >= y_top)
	{
		if(NULL != outRect)
		{
			outRect->SetRect(x_left, y_top, x_right, y_bottom);
		}

		return true;
	}
	else
	{
		return false;
	}
}

GUI::CRect GUI::CRect::UnionRect(const CRect& cmp)
{
	int left = (m_left >= cmp.m_left) ? cmp.m_left : m_left;
	int top = (m_top >= cmp.m_top) ? cmp.m_top : m_top;
	int right = (m_right >= cmp.m_right) ? m_right : cmp.m_right;
	int bottom = (m_bottom >= cmp.m_bottom) ? m_bottom : cmp.m_bottom;

	return GUI::CRect(left,top,right,bottom);
}

//end
