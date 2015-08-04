#include "DrawButton.h"

using namespace GUI;

GUI::CDrawButton::CDrawButton() : m_drawType(SQUARE)
{
}

GUI::CDrawButton::~CDrawButton()
{

}

bool CDrawButton::Create(unsigned long ctrlID, 
						  unsigned long x, 
						  unsigned long y, 
						  unsigned long cx,
						  unsigned long cy,
						  CWnd* pParent, 
						  int xFocus, 
						  int yFocus,
						  DRAWBTN_TYPE type/* = SQUARE*/)
{
	m_drawType = type;
	if (GUI::CButton::Create(ctrlID, x, y, cx, cy, pParent, xFocus, yFocus, NULL))
	{
		return true;
	}
	else
	{
		return false;
	}
}

void GUI::CDrawButton::OnPaint(PAINT_STATUS status)
{
	unsigned long color = 0;
	switch (status)
	{
	case PAINT_STATUS_FOCUS:
		color = GUI_RGB(255, 50, 50);
		break;
	case PAINT_STATUS_DISABLE:
		color = GUI_RGB(192, 192, 192);
		break;
	default:
		color = GUI_RGB(100, 100, 100);
		break;
	}
	
	GUI::CRect rect = m_rect;
	switch (m_drawType)
	{
	case SQUARE:
		{
			int size = 12;
			GUI::CWnd::DrawRect(rect.m_left + (rect.Width() - size)/2, rect.m_top + (rect.Height() - size)/2, size, size, color);
		}
		
		break;
	case TRIANGLE_LEFT:
		{
			int top = rect.m_top + rect.Height()/2 - 1;
			int bottom = top;
			for (int x = rect.m_left; x <= rect.m_right; ++x)
			{
				GUI::CWnd::DrawLine(x, top, x, bottom, color);
				top--;
				bottom++;
				if ((top < rect.m_top) || (bottom > rect.m_bottom))
				{
					break;
				}
			}
		}
		break;
	case TRIANGLE_RIGHT:
		{
			int top = rect.m_top + rect.Height()/2 - 1;
			int bottom = top;
			for (int x = rect.m_right; x >= rect.m_left; --x)
			{
				GUI::CWnd::DrawLine(x, top, x, bottom, color);
				top--;
				bottom++;
				if ((top < rect.m_top) || (bottom > rect.m_bottom))
				{
					break;
				}
			}
		}
		break;
	case TRIANGLE_UP:
		{
			int left = rect.m_left + rect.Width()/2 - 1;
			int right = left;
			for (int y = rect.m_top; y <= rect.m_bottom; ++y)
			{
				GUI::CWnd::DrawLine(left, y, right, y, color);
				left--;
				right++;
				if ((left < rect.m_left) || (right > rect.m_right))
				{
					break;
				}
			}
		}
		break;
	case TRIANGLE_DOWN:
		{
			int left = rect.m_left + rect.Width()/2 - 1;
			int right = left;
			for (int y = rect.m_bottom; y >= rect.m_top; --y)
			{
				GUI::CWnd::DrawLine(left, y, right, y, color);
				left--;
				right++;
				if ((left < rect.m_left) || (right > rect.m_right))
				{
					break;
				}
			}
		}
		break;
	default:
		assert(false);
		break;
	}
}

