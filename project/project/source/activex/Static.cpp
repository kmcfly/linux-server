#include "Static.h"

GUI::CStatic::CStatic() : m_lineStyle(NO_LINE), m_alignText(ALIGN_TEXT_LEFT), m_bAllRect(true), m_bkColor(INVALID_COLOR), m_textColor(COLOR_TEXT_NORMAL)
{
	m_ctrlType = GUI::WND_TYPE_STATIC;
}

GUI::CStatic::~CStatic()
{

}

bool GUI::CStatic::Create(unsigned long ctrlID, unsigned long x, unsigned long y, unsigned long cx, \
						  unsigned long cy, CWnd* pParent, const char* pCaption /* = NULL */)
{
	if (GUI::CWnd::Create(ctrlID, x, y, cx, cy, pParent, WND_CHILD | WND_VISIBLE | WND_NO_IMAGE_BK | WND_DISABLE_FOCUS, 0, 0))
	{
		if (NULL != pCaption)
		{
			SetCaption(pCaption, false);
		}
		return true;
	}
	else
	{
		return false;
	}
}

void GUI::CStatic::OnPaint(PAINT_STATUS status)
{
	unsigned short str_cx = 0;
	unsigned short str_cy = 0;
	if (!m_strCaption.empty())
	{
		if ((INVALID_COLOR != m_bkColor) || (NO_LINE != m_lineStyle) || (ALIGN_TEXT_LEFT != m_alignText))
		{
			GUI::CWnd::TextSize(m_strCaption.c_str(), m_strCaption.length(), str_cx, str_cy);
		}

		int text_x = m_rect.m_left;
		int text_y = m_rect.m_top;
		if (ALIGN_TEXT_CENTER == m_alignText)
		{
			text_x = m_rect.m_left + (m_rect.Width() - str_cx) / 2;
			text_y = m_rect.m_top + (m_rect.Height() - str_cy) / 2;
		}
		else if (ALIGN_TEXT_RIGHT == m_alignText)
		{
			text_x = m_rect.m_right - str_cx;
		}

		//如果需要绘制背景，先画背景
		if (INVALID_COLOR != m_bkColor)
		{
			if (m_bAllRect)
			{
				GUI::CWnd::DrawRect(m_rect.m_left, m_rect.m_top, m_rect.Width(), m_rect.Height(), m_bkColor);
			}
			else
			{
				GUI::CWnd::DrawRect(text_x, text_y, str_cx, str_cy, m_bkColor);
			}
		}

		if(g_langRightOrder && ALIGN_TEXT_CENTER == m_alignText)
		{
			GUI::CWnd::DrawText(m_strCaption.c_str(), m_strCaption.length(), text_x, text_y, str_cx, m_rect.Height(), m_textColor);
		}
		else
		{
			GUI::CWnd::DrawText(m_strCaption.c_str(), m_strCaption.length(), text_x, text_y, m_rect.Width(), m_rect.Height(), m_textColor);
		}
	} 

	if (NO_LINE != m_lineStyle)
	{
		if (HORZ_LINE == m_lineStyle)
		{
			int y = m_rect.m_top + m_rect.Height()/2;
			GUI::CWnd::DrawLine(m_rect.m_left - str_cx - 4, y, m_rect.m_right, y, COLOR_LINE_GRAY);
		} 
		else
		{
			int x = m_rect.m_left + m_rect.Width()/2;
			GUI::CWnd::DrawLine(x, m_rect.m_top, x, m_rect.m_bottom, COLOR_LINE_GRAY);
		}
	} 
}
