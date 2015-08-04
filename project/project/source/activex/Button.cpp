#include "Button.h"
#include "DrawManager.h"

GUI::CButton::CButton() : m_bMark(false)
, m_markColor(COLOR_BTN_DEFAULT_MARK)
, m_alignText(ALIGN_TEXT_CENTER)
, m_bDrawBorder(true)
{
	m_ctrlType = GUI::WND_TYPE_BUTTON;
	m_color = COLOR_BTN_BK;
	m_color_focus = COLOR_SELECT;
}

GUI::CButton::~CButton()
{

}

bool GUI::CButton::Create(unsigned long ctrlID, 
						  unsigned long x, 
						  unsigned long y, 
						  unsigned long cx,
						  unsigned long cy,
						  CWnd* pParent, 
						  int xFocus, 
						  int yFocus, 
						  const char* pCaption /* = NULL */,
						  unsigned long style/* = WND_CHILD | WND_VISIBLE | WND_DEFAULT_BK*/)
{
	if (GUI::CWnd::Create(ctrlID, x, y, cx, cy, pParent, style, xFocus, yFocus))
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

void GUI::CButton::OnPaint(PAINT_STATUS status)
{
	if (WND_NO_IMAGE_BK & m_style)
	{
		if (PAINT_STATUS_FOCUS == status)
		{
			GUI::CWnd::DrawRect(m_rect.m_left, m_rect.m_top, m_rect.Width(), m_rect.Height(), m_color_focus);
		}
		else
		{
			GUI::CWnd::DrawRect(m_rect.m_left, m_rect.m_top, m_rect.Width(), m_rect.Height(), m_color);
		}

		if (m_bDrawBorder)
		{
			//上横线
			GUI::CWnd::DrawLine(m_rect.m_left, m_rect.m_top, m_rect.m_right, m_rect.m_top, COLOR_LINE_GRAY, 1);
			//左竖线
			GUI::CWnd::DrawLine(m_rect.m_left, m_rect.m_top, m_rect.m_left, m_rect.m_bottom, COLOR_LINE_GRAY, 1);
			//右竖线
			GUI::CWnd::DrawLine(m_rect.m_right, m_rect.m_top, m_rect.m_right, m_rect.m_bottom, COLOR_LINE_BLACK, 1);
			//下横线
			GUI::CWnd::DrawLine(m_rect.m_left, m_rect.m_bottom, m_rect.m_right, m_rect.m_bottom, COLOR_LINE_BLACK, 1);
		}			
	}
	else if (WND_DEFAULT_BK & m_style)
	{

		BITMAP_ID bmpID = BMP_INVALID;
		if (PAINT_STATUS_FOCUS == status)
		{
			bmpID = BMP_BUTTON_NORMAL_FOCUS;
		} 
		else
		{
			bmpID = BMP_BUTTON_NORMAL;
		}

		CDrawManager drawMan;
		int len = 0;
		const unsigned char* pData = drawMan.Stretch(bmpID, m_rect.Width(), m_rect.Height(), 7, CDrawManager::INVLAID, &len, true);
		if (NULL != pData)
		{
			GUI::CWnd::DrawImage(pData, len, m_rect.m_left, m_rect.m_top, m_rect.Width(), m_rect.Width(), m_rect.Height());
		}	
	}
	else
	{
		//根据状态查找对应图片
		BTN_IMAGE::const_iterator iter = m_statusImage.find(status);
		if (iter == m_statusImage.end())
		{
			assert(false);
			return;
		}

		BITMAP_ID bmpID = iter->second;
		//绘制图片
		DrawImage(bmpID, m_rect.m_left, m_rect.m_top);
	}

	if (m_strCaption.length() > 0)
	{
		assert (PAINT_STATUS_NULL != status);

		unsigned short xOffSet = 3;
		unsigned short yOffset = 3;
		if (ALIGN_TEXT_CENTER == m_alignText)
		{
			unsigned short textWidth = 0, textHeight = 0;
			if (!GUI::CWnd::TextSize(m_strCaption.c_str(), m_strCaption.length(), textWidth, textHeight))
			{
				assert(false);
			}

			xOffSet = (textWidth < m_rect.Width()) ? ((m_rect.Width() - textWidth) / 2) : 0;
			yOffset = (textHeight < m_rect.Height()) ? ((m_rect.Height() - textHeight) / 2) : 0;
		} 
		else if (ALIGN_TEXT_RIGHT == m_alignText)
		{
			unsigned short textWidth = 0, textHeight = 0;
			if (!GUI::CWnd::TextSize(m_strCaption.c_str(), m_strCaption.length(), textWidth, textHeight))
			{
				assert(false);
			}

			xOffSet = (textWidth < m_rect.Width()) ? (m_rect.Width() - textWidth) : 0;
			yOffset = (textHeight < m_rect.Height()) ? (m_rect.Height() - textHeight) : 0;
		}
		

		if (PAINT_STATUS_NORMAL == status)
		{
			GUI::CWnd::DrawText(m_strCaption.c_str(), m_strCaption.length(), m_rect.m_left + xOffSet, m_rect.m_top + yOffset, m_rect.Width() -  2 * xOffSet, m_rect.Height()-yOffset);
		}
		else if (PAINT_STATUS_FOCUS == status)
		{
			GUI::CWnd::DrawText(m_strCaption.c_str(), m_strCaption.length(), m_rect.m_left + xOffSet + 1, m_rect.m_top + yOffset + 1, m_rect.Width() - 2 * xOffSet, m_rect.Height()-yOffset, COLOR_TEXT_FOCUS);
		}
		else
		{
			GUI::CWnd::DrawText(m_strCaption.c_str(), m_strCaption.length(), m_rect.m_left + xOffSet, m_rect.m_top + yOffset, m_rect.Width()- 2 * xOffSet, m_rect.Height()-yOffset, COLOR_TEXT_DISABLE);
		}
	}

	if (m_bMark)
	{
		GUI::CWnd::DrawRectBorder(m_rect.m_left + 2, m_rect.m_top + 2, m_rect.Width() - 4, m_rect.Height() - 4, m_markColor, 2);
	}
}

void GUI::CButton::SetImage(PAINT_STATUS status, BITMAP_ID id, bool bRefresh /* = true */)
{
	if (BMP_INVALID == id)
	{
		m_style &= ~(WND_DEFAULT_BK);
		m_style |= WND_NO_IMAGE_BK;
	}
	else
	{
		//修改按钮尺寸
		int cx = 0;
		int cy = 0;

		if (GetImageSize(id, cx, cy))
		{
			m_rect.Modify(m_rect.m_left, m_rect.m_top, cx, cy);
			m_showRect = m_rect;

			//设置图片
			m_statusImage[status] = id;

			m_style &= ~(WND_DEFAULT_BK);
			m_style &= ~(WND_NO_IMAGE_BK);

			if (bRefresh)
			{
				Repaint();
			}
		} 
		else
		{
			assert(false);//不存在图片
		}
	}
}

GUI::BITMAP_ID GUI::CButton::GetImage(PAINT_STATUS status) const
{
	BTN_IMAGE::const_iterator iter = m_statusImage.find(status);
	if (iter != m_statusImage.end())
	{
		return iter->second;
	} 
	else
	{
		return GUI::BMP_INVALID;
	}
}

void GUI::CButton::SetBkColor(unsigned color, bool bRefresh/* = false*/)
{
	m_color = color;
	if (bRefresh)
	{
		Repaint();
	}
}

void GUI::CButton::SetBkFocusColor(unsigned color, bool bRefresh/* = false*/)
{
	m_color_focus = color;
	if (bRefresh)
	{
		Repaint();
	}
}

void GUI::CButton::SetMark(bool bMark, unsigned color /* = COLOR_BTN_DEFAULT_MARK */, bool bRefresh /* = false */)
{
	m_bMark = bMark;
	m_markColor = color;

	if (bRefresh)
	{
		Repaint();
	}
}
