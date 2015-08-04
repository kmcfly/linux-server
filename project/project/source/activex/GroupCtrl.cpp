#include "GroupCtrl.h"

GUI::CGroupCtrl::CGroupCtrl() 
{
	m_ctrlType = GUI::WND_TYPE_GROUP;
}

GUI::CGroupCtrl::~CGroupCtrl()
{

}

bool GUI::CGroupCtrl::Create(unsigned long ctrlID, unsigned long x, unsigned long y, unsigned long cx, \
						  unsigned long cy, CWnd* pParent, const char* pCaption /* = NULL */, bool bCheck/* = false*/)
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

void GUI::CGroupCtrl::OnPaint(PAINT_STATUS status)
{
	unsigned long color = COLOR_LINE_GRAY;
	unsigned short width = 1;

	int text_x = m_rect.m_left + 15;
	int text_y = m_rect.m_top;
	unsigned short text_cx = 0;
	unsigned short text_cy = 0;
	if (m_strCaption.length() > 0)
	{
		TextSize(m_strCaption.c_str(), m_strCaption.length(), text_cx, text_cy);
		GUI::CWnd::DrawText(m_strCaption.c_str(), m_strCaption.length(), text_x, text_y, text_cx, m_rect.Height());

		GUI::CRect rcGroup(m_rect.m_left, text_y + m_length[STATIC_HEIGHT] / 2, m_rect.m_right, m_rect.m_bottom);

		GUI::CWnd::DrawLine(rcGroup.m_left, rcGroup.m_top, text_x - 3, rcGroup.m_top, color, width);
		GUI::CWnd::DrawLine(text_x + text_cx + 3, rcGroup.m_top, rcGroup.m_right, rcGroup.m_top, color, width);
		GUI::CWnd::DrawLine(rcGroup.m_right, rcGroup.m_top, rcGroup.m_right, rcGroup.m_bottom, color, width);
		GUI::CWnd::DrawLine(rcGroup.m_right, rcGroup.m_bottom, rcGroup.m_left, rcGroup.m_bottom, color, width);
		GUI::CWnd::DrawLine(rcGroup.m_left, rcGroup.m_bottom, rcGroup.m_left, rcGroup.m_top, color, width);
	}
	else
	{
		GUI::CWnd::DrawRectBorder(m_rect.m_left, text_y, m_rect.Width(), m_rect.Height(), color, width);
	}
}
