#include "ProgressCtrl.h"
#include "DrawManager.h"

using namespace GUI;

GUI::CProgressCtrl::CProgressCtrl() : m_percent(0), m_bShowPercent(false)
{
	m_ctrlType = GUI::WND_TYPE_PROGRESS;
}

GUI::CProgressCtrl::~CProgressCtrl()
{

}

bool GUI::CProgressCtrl::Create(unsigned long ctrlID, 
							unsigned long x, 
							unsigned long y,
							unsigned long cx, 
							unsigned long cy,
							CWnd* pParent,
							bool bVisible/* = true*/,
							bool bShowPercent/* = true*/)
{
	//设置对话框风格
	unsigned long style = WND_CHILD | WND_DISABLE_FOCUS;
	if (bVisible)
	{
		style |= WND_VISIBLE;
	} 

	m_bShowPercent = bShowPercent;
	//创建窗口
	if (GUI::CWnd::Create(ctrlID, x, y, cx, cy, pParent, style, 0, 0))
	{
		return true;
	}
	else
	{
		return false;
	}
}

void GUI::CProgressCtrl::OnPaint(PAINT_STATUS status)
{
	GUI::CWnd::OnPaint(status);

	const int PROGRESS_BORDER_WIDTH = 14;

	CDrawManager drawMan;
	int len = 0;
	const unsigned char* pData = drawMan.Stretch(BMP_PROGRESS_1, m_rect.Width(), m_rect.Height(), PROGRESS_BORDER_WIDTH, CDrawManager::INVLAID, &len, true);
	if (NULL != pData)
	{
		DrawImage(pData, len, m_rect.m_left, m_rect.m_top, m_rect.Width(), m_rect.Width(), m_rect.Height());
	}	

	if (m_percent > 0)
	{
		int draw_width = m_rect.Width() * m_percent / 100;

		const unsigned char* pData = NULL;
		//Left Border
		{
			int width = (draw_width > PROGRESS_BORDER_WIDTH ? PROGRESS_BORDER_WIDTH : draw_width);
			pData = drawMan.CopyHorizontal(BMP_PROGRESS_2, 0, 0, width, m_rect.Height(), &len, true);
			assert(NULL != pData);
			DrawImage(pData, len, m_rect.m_left, m_rect.m_top, width, width, m_rect.Height());
		}
		
		//Center
		if (draw_width > PROGRESS_BORDER_WIDTH)
		{
			int width = (draw_width > (m_rect.Width() - PROGRESS_BORDER_WIDTH) ? (m_rect.Width() - PROGRESS_BORDER_WIDTH * 2) : (draw_width - PROGRESS_BORDER_WIDTH));
			pData = drawMan.FillHorizontal(BMP_PROGRESS_2, PROGRESS_BORDER_WIDTH, width, m_rect.Height(), &len, true);
			assert(NULL != pData);
			DrawImage(pData, len, m_rect.m_left + PROGRESS_BORDER_WIDTH, m_rect.m_top, width, width, m_rect.Height());
		} 

		//Right
		if (draw_width > m_rect.Width() - PROGRESS_BORDER_WIDTH)
		{
			int width = PROGRESS_BORDER_WIDTH - (m_rect.Width() - draw_width);
			assert(width > 0);
			pData = drawMan.CopyHorizontal(BMP_PROGRESS_2, PROGRESS_BORDER_WIDTH, 0, width, m_rect.Height(), &len, true);
			assert(NULL != pData);
			DrawImage(pData, len, m_rect.m_right - PROGRESS_BORDER_WIDTH + 1, m_rect.m_top, width, width, m_rect.Height());
		}
	}

	//Show percent
	if (m_bShowPercent)
	{
		char str[32] = {0};
		snprintf(str, 32, "%d %%", m_percent);
		std::string strPercent = str;

		if (strPercent.length() > 0)
		{
			assert (PAINT_STATUS_NULL != status);

			unsigned short textWidth = 0, textHeight = 0;
			if (!GUI::CWnd::TextSize(strPercent.c_str(), strPercent.length(), textWidth, textHeight))
			{
				assert(false);
			}

			unsigned short xOffSet = (textWidth < m_rect.Width()) ? ((m_rect.Width() - textWidth) / 2) : 0;
			unsigned short yOffset = (textHeight < m_rect.Height()) ? ((m_rect.Height() - textHeight) / 2) : 0;

			GUI::CWnd::DrawText(strPercent.c_str(), strPercent.length(), m_rect.m_left + xOffSet, m_rect.m_top + yOffset, textWidth + 8, m_rect.Height()-yOffset);
		}
	}
}

void GUI::CProgressCtrl::SetPercent(int percet, bool bRefresh /* = true */)
{
	m_percent = percet;
	if (bRefresh)
	{
		Repaint();
	}
}
