/***********************************************************************
** Copyright (C)  Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : L
** Date         : 2010-03-25
** Name         : SetCoverDlg.cpp
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/

#include "SetCoverDlg.h"
#include "MessageMan.h"

extern unsigned char g_coverAreaCountLimit;

CSetCoverDlg::CSetCoverDlg()
{
	memset(&m_coverInfo, 0, sizeof(CAMERA_COVER_INFO));
	m_state = STATE_NULL;
	m_idSel = -1;
}

CSetCoverDlg::~CSetCoverDlg()
{

}

void CSetCoverDlg::OnInitial()
{
	GUI::CDialog::OnInitial();
	m_style |= WND_NO_TITLE;
	//////////////////////////////////////////////////////////////////////////

	m_areaMax = g_coverAreaCountLimit;

	m_areaVec.clear();
	m_idSel = -1;
	m_state = STATE_NULL;
	SetModify(false);

	for(int i = 0; i < m_areaMax; i++ )
	{
		COVER_AREA& area = m_coverInfo.area[i];
		if(0 != area.bCover)
		{
			RectCover rcCover;
			rcCover.bCover = area.bCover;
			rcCover.color = area.color;

			memcpy(&(rcCover.area), &area, sizeof(COVER_AREA));
			rcCover.old = 1;

			//四舍五入, 减少误差
			unsigned long x = (double(area.x * m_width) / 100 + double(0.5));
			unsigned long y = (double(area.y * m_height) / 100 + double(0.5));
			unsigned long cx = (double(area.cx * m_width) / 100 + double(0.5));
			unsigned long cy = (double(area.cy * m_height) / 100 + double(0.5));

			rcCover.id = (static_cast<unsigned long>(x) << 16) | static_cast<unsigned long>(y);
			rcCover.rect.SetRect(x, y, x + cx, y + cy);
		
			m_areaVec.push_back(rcCover);
		}
	}
}

void CSetCoverDlg::OnDestroy()
{
	GUI::CDialog::OnDestroy();

	m_areaVec.clear();
	m_idSel = -1;
}

void CSetCoverDlg::OnPaint(PAINT_STATUS status)
{
	GUI::CWnd::DrawClear(m_rect.m_left,m_rect.m_top,m_rect.Width(),m_rect.Height());

	std::vector<RectCover>::iterator iter;
	for (iter = m_areaVec.begin(); iter != m_areaVec.end(); iter++ )
	{
		DrawRectPt2(iter->rect.m_left, iter->rect.m_top, iter->rect.m_right, iter->rect.m_bottom, true);
		if ( m_idSel == iter->id)
		{
			DrawRectSel(iter->rect, true, false);
		}
	}
}

void CSetCoverDlg::EndDialog(DLG_END_VALUE result)
{
	if(RefreshCoverInfo())
	{
		result = DLG_OK;
	}
	else
	{
		result = DLG_CANCEL;
	}

	GUI::CDialog::EndDialog(result);
}

bool CSetCoverDlg::RefreshCoverInfo()
{
	if(IsModify())
	{
		memset(&m_coverInfo, 0, sizeof(CAMERA_COVER_INFO));

		std::vector<RectCover>::iterator iter;
		int index = 0;
		for (iter = m_areaVec.begin(); iter != m_areaVec.end(); iter++ )
		{
			if(index >= m_areaMax)
			{
				break;
			}

			if(!iter->old)
			{
				//四舍五入, 减少误差
				unsigned long x = (double(iter->rect.m_left) * 100 / m_width + double(0.5));
				unsigned long y = (double(iter->rect.m_top) * 100 / m_height + double(0.5));
				unsigned long cx = (double(iter->rect.Width()) * 100 / m_width + double(0.5));
				unsigned long cy = (double(iter->rect.Height()) * 100 / m_height + double(0.5));

				//值域cx,cy > 0

				if(x >= 100)
				{
					x = 99;
					cx = 1;
				}

				if(cx <= 0)
				{
					cx = 1;
				}

				if(x + cx >= 100)
				{
					cx = 100 - x;
				}

				if(y >= 100)
				{
					y = 99;
					cy = 1;
				}

				if(cy <= 0)
				{
					cy = 1;
				}

				if(y + cy >= 100)
				{
					cy = 100 - y;
				}

				COVER_AREA area;
				memset(&area, 0, sizeof(COVER_AREA));

				area.bCover = 1;
				area.x = x;
				area.y = y;
				area.cx = cx;
				area.cy = cy;

				memcpy(&m_coverInfo.area[index], &area, sizeof(COVER_AREA));
			}
			else
			{
				memcpy(&m_coverInfo.area[index], &(iter->area), sizeof(COVER_AREA));
			}

			index += 1;
		}

		return true;
	}
	else
	{
		return false;
	}
}

void CSetCoverDlg::SetCoverInfo(const CAMERA_COVER_INFO& coverInfo)
{
	memcpy(&m_coverInfo, &coverInfo, sizeof(CAMERA_COVER_INFO));
}

void CSetCoverDlg::GetCoverInfo(CAMERA_COVER_INFO& coverInfo)
{
	memcpy(&coverInfo, &m_coverInfo, sizeof(CAMERA_COVER_INFO));
}

bool CSetCoverDlg::IsModify()
{
	return m_bModify;
}

void CSetCoverDlg::SetModify(bool modify)
{
	m_bModify = modify;
}

bool CSetCoverDlg::IsAlmostEqual(unsigned long x1, unsigned long y1, unsigned long x2, unsigned long y2, unsigned long distance)
{
	unsigned long x = (x1>=x2)?(x1-x2):(x2-x1);
	unsigned long y = (y1>=y2)?(y1-y2):(y2-y1);

	if(((x*x) + (y*y)) <= distance)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool CSetCoverDlg::HitAreaTest(unsigned long xPos, unsigned long yPos, unsigned long& idHit)
{
	std::vector<RectCover>::iterator iter;
	for (iter = m_areaVec.begin(); iter != m_areaVec.end(); iter++ )
	{
		if(iter->rect.PtInRect(xPos, yPos))
		{
			idHit = iter->id;
			return true;
		}
	}
	return false;
}

bool CSetCoverDlg::FindAreaById(unsigned long id, GUI::CRect& rect)
{
	std::vector<RectCover>::iterator iter;
	for (iter = m_areaVec.begin(); iter != m_areaVec.end(); iter++ )
	{
		if(iter->id == id)
		{
			rect = iter->rect;
			return true;
		}
	}
	return false;
}

void CSetCoverDlg::DrawRectPt2(unsigned long x1, unsigned long y1, unsigned long x2, unsigned long y2, bool bDraw)
{
	unsigned long x, y, cx, cy;
	x = (x1 >= x2) ? x2 : x1;
	y = (y1 >= y2) ? y2 : y1;
	cx = (x1 >= x2) ? (x1 - x2) : (x2 - x1);
	cy = (y1 >= y2) ? (y1 - y2) : (y2 - y1);

	if(cx == 0 || cy == 0)
	{
		return;
	}

	if(bDraw)
	{
#ifdef __ENVIRONMENT_WIN32__
		GUI::CWnd::DrawRect(x, y, cx, cy, COLOR_LINE_WHITE, false);
#else
		GUI::CWnd::DrawRect(x, y, cx, cy, COLOR_LINE_GRAY, false);
#endif
	}
	else
	{
		GUI::CWnd::DrawClear(x, y, cx, cy, false);
	}
}

void CSetCoverDlg::RefreshPt2(unsigned long x1, unsigned long y1, unsigned long x2, unsigned long y2)
{
	unsigned long x, y, cx, cy;
	x = (x1 >= x2) ? x2 : x1;
	y = (y1 >= y2) ? y2 : y1;
	cx = (x1 >= x2) ? (x1 - x2) : (x2 - x1);
	cy = (y1 >= y2) ? (y1 - y2) : (y2 - y1);

	if(cx == 0 || cy == 0)
	{
		return;
	}

	GUI::CWnd::Refresh(x, y, cx, cy);
}

void CSetCoverDlg::DrawRectSel(const GUI::CRect& rect, bool bDraw, bool bImmediate)
{
	if(bDraw)
	{
		GUI::CWnd::DrawRectBorder(rect.m_left, rect.m_top, rect.Width() - 1, rect.Height() - 1, COLOR_SELECT,2, 2, bImmediate);
	}
	else
	{
#ifdef __ENVIRONMENT_WIN32__
		GUI::CWnd::DrawRectBorder(rect.m_left, rect.m_top, rect.Width() - 1, rect.Height() - 1, COLOR_LINE_WHITE, 2, 2, bImmediate);
#else
		GUI::CWnd::DrawRectBorder(rect.m_left, rect.m_top, rect.Width() - 1, rect.Height() - 1, COLOR_LINE_GRAY, 2, 2, bImmediate);
#endif
	}
}

void CSetCoverDlg::DrawRestore(const GUI::CRect& rect,  unsigned long idMiss, bool bImmediate)
{
	//清除屏幕区域后,可能属于已有的矩形,需要还原操作
	std::vector<RectCover>::iterator iter;
	for (iter = m_areaVec.begin(); iter != m_areaVec.end(); iter++ )
	{
		GUI::CRect interRect;
		if(iter->id != idMiss && iter->rect.IntersectRect(rect, &interRect))
		{
			DrawRectPt2(interRect.m_left, interRect.m_top, interRect.m_right, interRect.m_bottom, true);
			if(bImmediate)
			{
				RefreshPt2(interRect.m_left, interRect.m_top, interRect.m_right, interRect.m_bottom);
			}
		}
	}
}

void CSetCoverDlg::DrawRestore(unsigned long x1, unsigned long y1, unsigned long x2, unsigned long y2, unsigned long idMiss, bool bImmediate)
{
	unsigned long x, y, cx, cy;
	x = (x1 >= x2) ? x2 : x1;
	y = (y1 >= y2) ? y2 : y1;
	cx = (x1 >= x2) ? (x1 - x2) : (x2 - x1);
	cy = (y1 >= y2) ? (y1 - y2) : (y2 - y1);

	DrawRestore(GUI::CRect(x, y, x + cx, y + cy), idMiss, bImmediate);
}

BEGIN_MSG_MAP(CSetCoverDlg, GUI::CDialog)
	ON_MSG_EX(GetID(), KEY_VIRTUAL_ENTER, OnClick)
	ON_MSG_EX(GetID(), KEY_VIRTUAL_DBCLICK, OnDoubleClick)
	ON_MSG_EX(GetID(), KEY_VIRTUAL_DRAG, OnMouseDrag)
END_MSG_MAP()

unsigned long CSetCoverDlg::OnClick(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	if ((m_width <= xPos) && (m_height <= yPos))
	{
		//按键板的输入
		return KEY_VIRTUAL_MSG_STOP;
	}

	unsigned long idHit;
	if (HitAreaTest(xPos, yPos, idHit))
	{
		if(m_idSel != idHit)
		{
			//切换选中
			GUI::CRect rect;
			if(FindAreaById(m_idSel, rect))
			{
				DrawRectSel(rect, false, true);
			}

			if(FindAreaById(idHit, rect))
			{
				DrawRectSel(rect, true, true);
			}

			m_idSel = idHit;
		}
		else
		{
			GUI::CRect rect;

			if(FindAreaById(idHit, rect))
			{
				DrawRectSel(rect, true, true);
			}
		}

		m_state = STATE_NULL;

		//只要是在某一个已定区域内部，则不能记录可拖动绘制，保证不重叠。
		return KEY_VIRTUAL_MSG_STOP;
	}
	else
	{
		m_state = STATE_ADD; 
	}

	m_bFirstDrag = true;
	m_x_enter = xPos;
	m_y_enter = yPos;

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CSetCoverDlg::OnDoubleClick(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	if ((m_width <= xPos) && (m_height <= yPos))
	{
		//按键板的输入
		return KEY_VIRTUAL_MSG_STOP;
	}

	unsigned long idHit;
	if (HitAreaTest(xPos, yPos, idHit))
	{
		//删除
		GUI::CRect delRect;
		std::vector<RectCover>::iterator iter;
		for (iter = m_areaVec.begin(); iter != m_areaVec.end(); iter++ )
		{
			if(iter->id == idHit)
			{
				delRect = iter->rect;

				DrawRectPt2(iter->rect.m_left, iter->rect.m_top, iter->rect.m_right, iter->rect.m_bottom, false);
				RefreshPt2(iter->rect.m_left, iter->rect.m_top, iter->rect.m_right, iter->rect.m_bottom);

				m_areaVec.erase(iter);
				SetModify(true);

				break;
			}
		}

		DrawRestore(delRect, -1, true);

		m_idSel = -1;
	}

	return KEY_VIRTUAL_MSG_STOP;
}


unsigned long CSetCoverDlg::OnMouseDrag(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	static unsigned long old_x_pos = 0;
	static unsigned long old_y_pos = 0;

	if ((m_width <= xPos) && (m_height <= yPos))
	{
		//按键板的输入
		return KEY_VIRTUAL_MSG_STOP;
	}

	if(STATE_ADD != m_state)
	{
		return KEY_VIRTUAL_MSG_STOP;
	}

	if(m_bFirstDrag)
	{
		old_x_pos = m_x_enter;
		old_y_pos = m_y_enter;

		m_bFirstDrag = false;
	}

	if(xPos >= m_x_enter + 4 && yPos >= m_y_enter + 4 && !IsAlmostEqual(old_x_pos, old_y_pos, xPos, yPos, 100))
	{
		//add or modify
		unsigned long id = (m_x_enter << 16) | m_y_enter;
		bool bFind = false;
		std::vector<RectCover>::iterator iter;
		for (iter = m_areaVec.begin(); iter != m_areaVec.end(); iter++ )
		{
			if(iter->id == id)
			{
				iter->rect.SetRect(m_x_enter, m_y_enter, xPos, yPos);
				bFind = true;
				SetModify(true);
				break;
			}
		}

		if(!bFind)
		{
			if(m_areaVec.size() >= m_areaMax)
			{
				//满了之后不能再添加
				return KEY_VIRTUAL_MSG_STOP;
			}

			RectCover rcCover;
			memset(&rcCover, 0, sizeof(RectCover));
			rcCover.bCover = 1;
			rcCover.id = id;
			rcCover.rect.SetRect(m_x_enter, m_y_enter, xPos, yPos);

			m_areaVec.push_back(rcCover);
			SetModify(true);
		}

		//页面刷新

		if(yPos > old_y_pos)
		{
			DrawRectPt2(m_x_enter, old_y_pos, old_x_pos, yPos, true);
		}
		else if(yPos < old_y_pos)
		{
			DrawRectPt2(m_x_enter, old_y_pos, old_x_pos, yPos, false);
			DrawRestore(m_x_enter, old_y_pos, old_x_pos, yPos, id, false);
		}

		if(xPos > old_x_pos)
		{
			DrawRectPt2(old_x_pos, m_y_enter, xPos, old_y_pos, true);
		}
		else if(xPos < old_x_pos)
		{
			DrawRectPt2(old_x_pos, m_y_enter, xPos, old_y_pos, false);
			DrawRestore(old_x_pos, m_y_enter, xPos, old_y_pos, id, false);
		}

		if(xPos > old_x_pos && yPos > old_y_pos)
		{
			DrawRectPt2(old_x_pos, old_y_pos, xPos, yPos, true);
		}
		else
		{
			DrawRectPt2(old_x_pos, old_y_pos, xPos, yPos, false);
			DrawRestore(old_x_pos, old_y_pos, xPos, yPos, id, false);
		}

		//刷新屏幕
		RefreshPt2(m_x_enter, old_y_pos, xPos, yPos);
		RefreshPt2(old_x_pos, m_y_enter, xPos, old_y_pos);

		old_x_pos = xPos;
		old_y_pos = yPos;
	}
	return KEY_VIRTUAL_MSG_STOP;
}

//end
