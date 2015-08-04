/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : YSW
** Date         : 2010-06-18
** Name         : Anchor.cpp
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/
#include "Anchor.h"
#include "DrawManager.h"

GUI::CAnchor::CAnchor() : m_textColor(COLOR_TEXT_NORMAL),
m_idNormal(BMP_INVALID),
m_idFocus(BMP_INVALID),
m_idDisable(BMP_INVALID),
m_bmpNot(true)
{
	m_ctrlType = GUI::WND_TYPE_ANCHOR;
}

GUI::CAnchor::~CAnchor()
{

}

bool GUI::CAnchor::Create(unsigned long ctrlID, unsigned long x, unsigned long y, unsigned long cx, \
						  unsigned long cy, CWnd* pParent, int xFocus, int yFocus, const char* pCaption /* = NULL */)
{
	if (GUI::CWnd::Create(ctrlID, x, y, cx, cy, pParent, WND_CHILD | WND_VISIBLE | WND_NO_IMAGE_BK, xFocus, yFocus))
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

void GUI::CAnchor::OnInitial()
{
	CWnd::OnInitial();

	if(BMP_ANCHOR_BK < BMP_INVALID_BELOW)
	{
		CWnd::GetImageSize(BMP_ANCHOR_BK, m_bmpBgWidth, m_bmpBgHeight);
	}
	else
	{
		m_bmpBgWidth = 0;
		m_bmpBgHeight = 0;
	}
}

void GUI::CAnchor::OnPaint(PAINT_STATUS status)
{
	CWnd::OnPaint(status);

	if(m_bmpNot)
	{
		if (!m_strCaption.empty())
		{
			unsigned long color = m_textColor;
			if (PAINT_STATUS_NORMAL == status)
			{
				color = m_textColor;
			}
			else if (PAINT_STATUS_FOCUS == status)
			{
				color = COLOR_SELECT;
			}
			else
			{
				color = COLOR_TEXT_DISABLE;
			}

			GUI::CWnd::DrawText(m_strCaption.c_str(), m_strCaption.length(), m_rect.m_left, m_rect.m_top, m_rect.Width(), m_rect.Height(), color);

			if (PAINT_STATUS_FOCUS == status)
			{
				GUI::CWnd::DrawLine(m_rect.m_left, m_rect.m_bottom, m_rect.m_right, m_rect.m_bottom, COLOR_SELECT, 1);
			}
			else
			{
				GUI::CWnd::DrawLine(m_rect.m_left, m_rect.m_bottom, m_rect.m_right, m_rect.m_bottom, COLOR_DLG_CHILD_BK, 1);
			}
		} 
	}
	else
	{
		unsigned long x = m_rect.m_left, y = m_rect.m_top;

		//»æÖÆ×ó±ßÍ¼Æ¬Óëµ×Í¼
		if(PAINT_STATUS_FOCUS == status)
		{
			if(BMP_INVALID != m_idFocus)
			{
				CWnd::DrawImage(m_idFocus, x, y);
			}
			else
			{
				assert(false);
			}

			if(m_rect.Width() > 180)
			{
				CDrawManager drawMan;
				const unsigned char* pData = drawMan.Stretch(BMP_ANCHOR_BK_F, m_rect.Width() - 60, m_rect.Height(), m_bmpBgWidth / 2, CDrawManager::INVLAID, NULL, true);
				int len = drawMan.GetDataLen();
				if (NULL != pData)
				{
					GUI::CWnd::DrawImage(pData, len, m_rect.m_left + 60, m_rect.m_top, m_rect.Width() - 60, m_rect.Width() - 60, m_rect.Height());
				}
			}
			else
			{
				GUI::CWnd::DrawImage(BMP_ANCHOR_BK_F, m_rect.m_left + 60, m_rect.m_top);
			}
		}
		else
		{
			if(BMP_INVALID != m_idNormal)
			{
				CWnd::DrawImage(m_idNormal, x, y);
			}
			else
			{
				assert(false);
			}

			if(m_rect.Width() > 180)
			{
				CDrawManager drawMan;
				const unsigned char* pData = drawMan.Stretch(BMP_ANCHOR_BK, m_rect.Width() - 60, m_rect.Height(), m_bmpBgWidth / 2, CDrawManager::INVLAID, NULL, true);
				int len = drawMan.GetDataLen();
				if (NULL != pData)
				{
					GUI::CWnd::DrawImage(pData, len, m_rect.m_left + 60, m_rect.m_top, m_rect.Width() - 60, m_rect.Width() - 60, m_rect.Height());
				}
			}
			else
			{
				GUI::CWnd::DrawImage(BMP_ANCHOR_BK, m_rect.m_left + 60, m_rect.m_top);
			}
		}

		//»æÖÆÎÄ×Ö
		if (!m_strCaption.empty())
		{
			x = m_rect.m_left + 66;
			y = m_rect.m_top + (m_rect.Height() - m_length[STATIC_HEIGHT]) / 2;
			unsigned short cx = 0, cy = m_length[STATIC_HEIGHT];
			CWnd::TextSize(m_strCaption.c_str(), m_strCaption.length(), cx, cy);

			unsigned long color = m_textColor;
			if (PAINT_STATUS_NORMAL == status)
			{
				color = m_textColor;
			}
			else if (PAINT_STATUS_FOCUS == status)
			{
				color = COLOR_TEXT_FOCUS;
			}
			else
			{
				color = COLOR_TEXT_DISABLE;
			}

			GUI::CWnd::DrawText(m_strCaption.c_str(), m_strCaption.length(), x, y, m_rect.Width() - 68, cy, color);
		}
	}
}

void GUI::CAnchor::SetImage(BITMAP_ID idNormal, BITMAP_ID idFocus, BITMAP_ID idDisable)
{
	m_idNormal = idNormal;
	m_idFocus = idFocus;
	m_idDisable = idDisable;

	if((BMP_INVALID != idNormal) && (BMP_INVALID != idFocus))
	{
		m_bmpNot = false;
	}
}

//end
