/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : L
** Date         : 2009-01-12
** Name         : SetPositionDlg.cpp
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/

#include "SetPositionDlg.h"
#include "MessageMan.h"

#undef USE_DRAG_SET_POSITION
#define USE_DRAG_SET_POSITION 1
extern VIDEO_FORMAT	g_video_format;
//////////////////////////////////////////////////////////////////////////

#ifdef __SUPPORT_OSDTWOLINES__
 CSetPtItem::CSetPtItem() : m_bBlink(false)
 , m_bDrawBorder(true)
 , m_count(0)
 , m_bCamName(false)
 {

 }
#else
CSetPtItem::CSetPtItem() : m_bBlink(false)
, m_bDrawBorder(true)
, m_count(0)
{

}

#endif

CSetPtItem::~CSetPtItem()
{

}

#ifdef __SUPPORT_OSDTWOLINES__
bool CSetPtItem::Create( unsigned long ctrlID, unsigned long x, unsigned long y, unsigned long cx, unsigned long cy, bool bCamName, CWnd* pParent, const char* pCaption)
{
	m_bDrawBorder = true;
	m_count = 0;
	m_bCamName = bCamName;

	if (GUI::CWnd::Create(ctrlID, x, y, cx, cy, pParent, WND_CHILD | WND_VISIBLE | WND_NO_IMAGE_BK, 0, 0))
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
#else
bool CSetPtItem::Create( unsigned long ctrlID, unsigned long x, unsigned long y, unsigned long cx, unsigned long cy, CWnd* pParent, const char* pCaption)
{
	m_bDrawBorder = true;
	m_count = 0;

	if (GUI::CWnd::Create(ctrlID, x, y, cx, cy, pParent, WND_CHILD | WND_VISIBLE | WND_NO_IMAGE_BK, 0, 0))
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

#endif

void CSetPtItem::OnPaint(PAINT_STATUS status)
{
	//GUI::CWnd::DrawClear(m_rect.m_left, m_rect.m_top, m_rect.Width(), m_rect.Height());
	
	OnDrawBlink(true);

	OnDrawText();
}

void CSetPtItem::OnDrawText()
{
	unsigned short str_cx = 0;
	unsigned short str_cy = 0;
	if (!m_strCaption.empty())
	{
		int text_x = m_rect.m_left + 4;
		int text_y = m_rect.m_top + 2;

#ifdef __SUPPORT_OSDTWOLINES__
		//2011-06-15 ZYX 改
		int firstLineNum = m_strCaption.find('/');
		if (m_bCamName && -1 != firstLineNum)
		{
			GUI::CWnd::DrawText(m_strCaption.c_str(), firstLineNum, text_x, text_y, m_rect.Width(), m_rect.Height(), COLOR_TEXT_NORMAL);
			GUI::CWnd::DrawText(m_strCaption.c_str()+firstLineNum+1, m_strCaption.length()-firstLineNum-1, text_x, text_y+m_rect.Height()/2, m_rect.Width(), m_rect.Height(), COLOR_TEXT_NORMAL);
		}
		else
		{
			GUI::CWnd::DrawText(m_strCaption.c_str(), m_strCaption.length(), text_x, text_y, m_rect.Width(), m_rect.Height(), COLOR_TEXT_NORMAL);
		}
		//
#else
		GUI::CWnd::DrawText(m_strCaption.c_str(), m_strCaption.length(), text_x, text_y, m_rect.Width(), m_rect.Height(), COLOR_TEXT_NORMAL);
#endif
	} 
}

void CSetPtItem::OnDrawBlink(bool erase)
{
	if(m_bBlink)
	{
		if(m_bDrawBorder)
		{
			GUI::CWnd::DrawRectBorder(m_rect.m_left, m_rect.m_top, m_rect.Width(), m_rect.Height(), COLOR_SELECT);
		}
		else
		{
			if(erase)
			{
				//去除4条边线
				GUI::CWnd::DrawClear(m_rect.m_left, m_rect.m_top, m_rect.Width(), 1);
				GUI::CWnd::DrawClear(m_rect.m_left, m_rect.m_bottom, m_rect.Width(), 1);
				GUI::CWnd::DrawClear(m_rect.m_left, m_rect.m_top, 1, m_rect.Height());
				GUI::CWnd::DrawClear(m_rect.m_right, m_rect.m_top, 1, m_rect.Height());
			}
		}

		m_bDrawBorder = !m_bDrawBorder;
	}
	else
	{
		GUI::CWnd::DrawRectBorder(m_rect.m_left, m_rect.m_top, m_rect.Width(), m_rect.Height(), COLOR_SELECT);
	}
}

bool CSetPtItem::SetBlink(bool enable)
{
	bool ret = (m_bBlink != enable);
	m_bBlink = enable;
	return ret;
}

void CSetPtItem::OnIdle()
{
	if(m_bBlink)
	{
		m_count++;
#ifdef __ENVIRONMENT_WIN32__
		if(m_count >= 20)
#else
		if(m_count >= 20)
#endif
		{
			m_count= 0;

			//由于可能存在遮盖关系,绘制代码交给父窗口处理
			//Repaint();

			CSetPositionDlg* pSetDlg = dynamic_cast<CSetPositionDlg*>(GetParent());
			assert(pSetDlg);
			pSetDlg->OnDrawItem(this);
		}
	}
}

//////////////////////////////////////////////////////////////////////////

CSetPositionDlg::CSetPositionDlg() : m_bSetCamera(false)
,m_bSetTime(false)
{
	m_rectCamera.SetRect(10, 10, 120, 10 + m_length[STATIC_HEIGHT]);
	m_rectTime.SetRect(10, 10, 120, 10 + m_length[STATIC_HEIGHT]);
}

CSetPositionDlg::~CSetPositionDlg()
{

}

void CSetPositionDlg::OnInitial()
{
	GUI::CDialog::OnInitial();
	m_style |= WND_NO_TITLE;
	//////////////////////////////////////////////////////////////////////////

	m_bModify = false;
	m_bDragCamera = false;
	m_bDragTime = false;
	m_bFirstDrag = false;
	m_x_enter =0, m_y_enter = 0;

#ifdef __SUPPORT_OSDTWOLINES__
	m_scCamera.Create(GetRandWndID(),m_rectCamera.m_left,m_rectCamera.m_top,m_rectCamera.Width(),m_rectCamera.Height(), true, this);
	m_scTime.Create(GetRandWndID(),m_rectTime.m_left,m_rectTime.m_top,m_rectTime.Width(),m_rectTime.Height(),false, this);
#else
	m_scCamera.Create(GetRandWndID(),m_rectCamera.m_left,m_rectCamera.m_top,m_rectCamera.Width(),m_rectCamera.Height(),this);
	m_scTime.Create(GetRandWndID(),m_rectTime.m_left,m_rectTime.m_top,m_rectTime.Width(),m_rectTime.Height(),this);
#endif
	m_scCamera.SetData(ITEM_CAM);
	m_scCamera.Show(m_bSetCamera,false);

	m_scTime.SetData(ITEM_TIME);
	m_scTime.Show(m_bSetTime,false);

	////
	if(m_bSetCamera)
	{
		SetSelItem(ITEM_CAM, false);
	}
	else
	{
		SetSelItem(ITEM_TIME, false);
	}
	
	m_offset_len = ((m_width / 50) * (m_width / 50)) + ((m_height / 50) * (m_height / 50));
	if (VIDEO_FORMAT_NTSC == g_video_format)
	{
		m_offset_pos = (m_height + 11) / 12;
	}
	else
	{
		m_offset_pos = (m_height + 14) / 15;
	}
	/////////////////////////////////////////////////////////////////////////
	CreateFocusMap();
}

void CSetPositionDlg::OnDestroy()
{
	CDialog::OnDestroy();
}

void CSetPositionDlg::OnPaint(PAINT_STATUS status)
{
	GUI::CWnd::DrawClear(m_rect.m_left,m_rect.m_top,m_rect.Width(),m_rect.Height());
}

void CSetPositionDlg::EndDialog(DLG_END_VALUE result)
{
	m_bSetCamera = false;
	m_bSetTime = false;

	CDialog::EndDialog(result);
}

void CSetPositionDlg::OnIdle()
{
	GUI::CDialog::OnIdle();

	WND_LIST::iterator iter;
	for (iter = m_childList.begin(); iter != m_childList.end(); ++iter)
	{
		CSetPtItem* pItem = dynamic_cast<CSetPtItem*>(*iter);
		if (NULL != pItem)
		{
			pItem->OnIdle();
		}
	}
}

unsigned long CSetPositionDlg::OnFocusProcEx(unsigned long keyID, unsigned long xPoint, unsigned long yPoint)
{
	//如果焦点地图建立失败，不会走本函数

	if(keyID == KEY_VIRTUAL_MOVE)
	{
		static unsigned long s_count = 0;
		s_count++;

		//鼠标移动抓键值,比较快,忽略部分
#ifdef __ENVIRONMENT_WIN32__
		if(s_count >= 5)
		{
			s_count = 0;
			OnIdle();
		}
#else
		if(s_count >= 5)
		{
			s_count = 0;
			OnIdle();
		}
#endif
		return KEY_VIRTUAL_MSG_STOP;
	}
	else if (keyID == KEY_VIRTUAL_DRAG)
	{
		// ??
		return OnMouseMove(CTRLID_NULL, keyID, xPoint, yPoint);
	}

	return GUI::CDialog::OnFocusProcEx(keyID, xPoint, yPoint);
}

void CSetPositionDlg::SeEnableCamera(int x, int y, int cx, int cy, const char* pCamera, bool enable)
{
	m_rectCamera.SetRect(x,y,x+cx,y+cy);
	m_scCamera.SetCaption(pCamera,false);
	m_bSetCamera = enable;
}

void CSetPositionDlg::SetEnableTime(int x, int y, int cx, int cy, const char* pTime, bool enable)
{
	m_rectTime.SetRect(x,y,x+cx,y+cy);
	m_scTime.SetCaption(pTime,false);
	m_bSetTime = enable;
}

void CSetPositionDlg::GetPosCamera(int& x, int& y)
{
	x = m_rectCamera.m_left;
	y = m_rectCamera.m_top;
}

void CSetPositionDlg::GetPosTime(int& x, int& y)
{
	x = m_rectTime.m_left;
	y = m_rectTime.m_top;
}

bool CSetPositionDlg::IsAlmostEqual(unsigned long x1, unsigned long y1, unsigned long x2, unsigned long y2, unsigned long distance)
{
	unsigned long x = (x1 >= x2) ? (x1-x2) : (x2-x1);
	unsigned long y = (y1 >= y2) ? (y1-y2) : (y2-y1);

	if(((x*x) + (y*y)) <= distance)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool CSetPositionDlg::IsAlmostAbove(unsigned long x1, unsigned long y1, unsigned long x2, unsigned long y2, unsigned long distance)
{
	unsigned long x = (x1>=x2)?(x1-x2):(x2-x1);
	unsigned long y = (y1>=y2)?(y1-y2):(y2-y1);

	if(((x*x) + (y*y)) >= distance)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void CSetPositionDlg::SetModify(bool modify)
{
	m_bModify = modify;
}

void CSetPositionDlg::SetSelItem(ITEM_SEL sel, bool bRefresh)
{
	m_selItem = sel;
	UpdateSelItem(bRefresh);
}

void CSetPositionDlg::UpdateSelItem(bool bRefresh)
{
	if(m_scCamera.SetBlink(m_selItem == ITEM_CAM) && bRefresh)
	{
		m_scCamera.SetDrawBlink(true);
		m_scCamera.Repaint();
	}

	if(m_scTime.SetBlink(m_selItem == ITEM_TIME) && bRefresh)
	{
		m_scTime.SetDrawBlink(true);
		m_scTime.Repaint();
	}
}

void CSetPositionDlg::OnDrawItem(CSetPtItem* pItem)
{
	ITEM_SEL item = static_cast<ITEM_SEL>(pItem->GetData());
	 switch (item)
	 {
	 case ITEM_CAM:
		 {
			if(m_rectCamera.IntersectRect(m_rectTime))
			{
				GUI::CRect rect = m_rectCamera.UnionRect(m_rectTime);
				GUI::CWnd::DrawClear(rect.m_left, m_rect.m_top, m_rect.Width(), m_rect.Height());

				m_scTime.OnDrawBlink(false);
				m_scTime.OnDrawText();
				m_scCamera.OnDrawBlink(false);
				m_scCamera.OnDrawText();

				GUI::CWnd::Refresh(rect.m_left, m_rect.m_top, m_rect.Width(), m_rect.Height());
			}
			else
			{
				//m_scTime.Repaint();
				m_scCamera.Repaint();
			}
		 }
		 break;
	 case ITEM_TIME:
		 {
			 if(m_rectTime.IntersectRect(m_rectCamera))
			 {
				 GUI::CRect rect = m_rectCamera.UnionRect(m_rectTime);
				 GUI::CWnd::DrawClear(rect.m_left, m_rect.m_top, m_rect.Width(), m_rect.Height());

				 m_scCamera.OnDrawBlink(false);
				 m_scCamera.OnDrawText();
				 m_scTime.OnDrawBlink(false);
				 m_scTime.OnDrawText();

				 GUI::CWnd::Refresh(rect.m_left, m_rect.m_top, m_rect.Width(), m_rect.Height());
			 }
			 else
			 {
				//m_scCamera.Repaint();
				m_scTime.Repaint();
			 }
		 }
		 break;
	 default:
		 break;
	 }
}

BEGIN_MSG_MAP(CSetPositionDlg, GUI::CDialog)
#if USE_DRAG_SET_POSITION
	ON_MSG_EX(GetID(),KEY_VIRTUAL_DRAG,OnMouseMove)
#else
	ON_MSG_EX(GetID(), KEY_VIRTUAL_MOVE, OnMouseMove)
#endif
	ON_MSG_EX(GetID(), KEY_VIRTUAL_ENTER, OnEnter)
END_MSG_MAP()

unsigned long CSetPositionDlg::OnEnter(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
#if USE_DRAG_SET_POSITION
	if(m_bDragCamera && !m_rectCamera.PtInRect(xPos,yPos))
	{
		m_bDragCamera = false;
	}

	if(m_bDragTime && !m_rectTime.PtInRect(xPos,yPos))
	{
		m_bDragTime = false;
	}
#endif

	if( !m_bDragCamera && !m_bDragTime)
	{
		if(m_bSetCamera && m_rectCamera.PtInRect(xPos,yPos))
		{
			m_bDragCamera = true;
			m_bFirstDrag = true;
			m_x_enter = xPos;
			m_y_enter = yPos;

			SetSelItem(ITEM_CAM, true);

			return KEY_VIRTUAL_MSG_STOP;
		}
			
		if(m_bSetTime && m_rectTime.PtInRect(xPos,yPos))
		{
			m_bDragTime = true;
			m_bFirstDrag = true;
			m_x_enter = xPos;
			m_y_enter = yPos;

			SetSelItem(ITEM_TIME, true);

			return KEY_VIRTUAL_MSG_STOP;
		}

		return KEY_VIRTUAL_MSG_STOP;
	}

#if !(USE_DRAG_SET_POSITION)
	if( m_bDragCamera )
	{
		OnMouseMove(0,0,xPos,yPos);
		m_bDragCamera = false;
		return KEY_VIRTUAL_MSG_STOP;
	}

	if( m_bDragTime )
	{
		OnMouseMove(0,0,xPos,yPos);
		m_bDragTime = false;
		return KEY_VIRTUAL_MSG_STOP;
	}
#endif

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CSetPositionDlg::OnMouseMove(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	static unsigned long old_x_pos = 0;
	static unsigned long old_y_pos = 0;

	//处理通道名 
	if( m_bDragCamera )
	{
		if(m_bFirstDrag)
		{
			old_x_pos = m_x_enter;
			old_y_pos = m_y_enter;
			m_bFirstDrag = false;
		}
#if !(USE_DRAG_SET_POSITION)
		//当两点距离平方和过大时,认为属于结束操作
		if(IsAlmostAbove(old_x_pos,old_y_pos,xPos,yPos,3200))
		{
			m_bDragCamera = false;
			return KEY_VIRTUAL_MSG_STOP;
		}
#endif

		//只有在两点距离平方和大于N时,才重绘
		if(!IsAlmostEqual(old_x_pos, old_y_pos, xPos, yPos, m_offset_len))
		{
			//2010-06-11 12:00:00 YSW
			unsigned long top = (m_rectTime.m_top <= m_offset_pos) ? 0 : (m_rectTime.m_top - m_offset_pos);
			unsigned long bottom = ((m_rectTime.m_top + m_offset_pos) <= m_height) ? (m_rectTime.m_top + m_offset_pos) : m_height;
			unsigned long yy = 0;
			if (yPos <= old_y_pos)
			{
				yy = (m_rectCamera.m_top <= (old_y_pos - yPos)) ? 0 : m_rectCamera.m_top - (old_y_pos - yPos);
			}
			else
			{
				yy = (m_rectCamera.m_bottom <= (m_height - (yPos - old_y_pos))) ? m_rectCamera.m_top + (yPos - old_y_pos) : m_height - m_rectCamera.Height();
			}

			if ((top <= yy) && (yy <= bottom))
			{
				return KEY_VIRTUAL_MSG_STOP;
			}

			int x = xPos - old_x_pos;
			int y = yPos - old_y_pos;

			bool refreshTime = true;
			GUI::CRect oldRect = m_rectCamera;

			//清除原来区域
			GUI::CWnd::DrawClear(m_rectCamera.m_left,m_rectCamera.m_top,m_rectCamera.Width(),m_rectCamera.Height(),false);

			//修正x,y以免超出屏幕范围
			if(m_rectCamera.m_left+x <= 0) x = -m_rectCamera.m_left;
#ifdef __SUPPORT_OSDTWOLINES__
			//2011-06-15 ZYX
			if(m_rectCamera.m_left+x >= m_rect.Width()-m_rectCamera.Width()) x = m_rect.Width()-m_rectCamera.Width() - m_rectCamera.m_left;
#else
			if(m_rectCamera.m_left+x >= m_rect.Width()) x = m_rect.Width()-m_rectCamera.m_left;
#endif
			if(m_rectCamera.m_top+y <= 0) y = - m_rectCamera.m_top;
			if(m_rectCamera.m_top+y >= m_rect.Height()-m_rectCamera.Height()) y = m_rect.Height()-m_rectCamera.Height()-m_rectCamera.m_top;

			//移动窗口位置
			SetModify(true);
			m_rectCamera.Offset(x,y);
			m_scCamera.MoveWnd(m_rectCamera.m_left,m_rectCamera.m_top);

			refreshTime = ( m_rectTime.IntersectRect(oldRect) || m_rectTime.IntersectRect(m_rectCamera) );
			if(m_bSetTime&&refreshTime) 
			{
				m_scTime.Show(true,false);
			}
			if(m_bSetCamera)
			{
				m_scCamera.Show(true,false);
			}

			//一次性统一刷新屏幕
			GUI::CRect newRect = m_rectCamera.UnionRect(oldRect);
			if( refreshTime )
			{
				GUI::CWnd::Refresh(m_rectTime.m_left,m_rectTime.m_top,m_rectTime.Width(),m_rectTime.Height());
			}
			GUI::CWnd::Refresh(newRect.m_left,newRect.m_top,newRect.Width(),newRect.Height());

			old_x_pos = xPos;
			old_y_pos = yPos;
		}

		return KEY_VIRTUAL_MSG_STOP;
	}
	
	//处理时间
	if( m_bDragTime )
	{
		if(m_bFirstDrag)
		{
			old_x_pos = m_x_enter;
			old_y_pos = m_y_enter;
			m_bFirstDrag = false;
		}

#if !(USE_DRAG_SET_POSITION)
		//当两点距离平方和过大时,认为属于结束操作
		if(IsAlmostAbove(old_x_pos,old_y_pos,xPos,yPos,3200))
		{
			m_bDragTime = false;
			return KEY_VIRTUAL_MSG_STOP;
		}
#endif

		//只有在两点距离平方和大于N时,才重绘
		if(!IsAlmostEqual(old_x_pos, old_y_pos, xPos, yPos, m_offset_len))
		{
			//2010-06-11 12:00:00 YSW
			unsigned long top = (m_rectCamera.m_top <= m_offset_pos) ? 0 : (m_rectCamera.m_top - m_offset_pos);
			unsigned long bottom = ((m_rectCamera.m_top + m_offset_pos) <= m_height) ? (m_rectCamera.m_top + m_offset_pos) : m_height;
			unsigned long yy = 0;
			if (yPos <= old_y_pos)
			{
				yy = (m_rectTime.m_top <= (old_y_pos - yPos)) ? 0 : m_rectTime.m_top - (old_y_pos - yPos);
			}
			else
			{
				yy = (m_rectTime.m_bottom <= (m_height - (yPos - old_y_pos))) ? m_rectTime.m_top + (yPos - old_y_pos) : m_height - m_rectTime.Height();
			}

			if ((top <= yy) && (yy < bottom))
			{
				return KEY_VIRTUAL_MSG_STOP;
			}

			int x = xPos - old_x_pos;
			int y = yPos - old_y_pos;

			bool refreshCamera = true;
			GUI::CRect oldRect = m_rectTime;

			//清除原来区域
			DrawClear(m_rect.m_left+m_rectTime.m_left,m_rect.m_top+m_rectTime.m_top,m_rectTime.Width(),m_rectTime.Height(),false);

			//修正x,y以免超出屏幕范围
			if(m_rectTime.m_left+x <= 0) x = -m_rectTime.m_left;
			if(m_rectTime.m_left+x >= m_rect.Width()) x = m_rect.Width()-m_rectTime.m_left;
			if(m_rectTime.m_top+y <= 0) y = - m_rectTime.m_top;
			if(m_rectTime.m_top+y >= m_rect.Height()-m_rectTime.Height()) y = m_rect.Height()-m_rectTime.Height()-m_rectTime.m_top;

			//移动窗口位置
			SetModify(true);
			m_rectTime.Offset(x,y);
			m_scTime.MoveWnd(m_rectTime.m_left,m_rectTime.m_top);

			refreshCamera = ( m_rectCamera.IntersectRect(oldRect) || m_rectCamera.IntersectRect(m_rectTime) );
			if(m_bSetCamera && refreshCamera)
			{
				m_scCamera.Show(true,false);
			}
			if(m_bSetTime)
			{
				m_scTime.Show(true,false);
			}

			//一次性统一刷新屏幕
			GUI::CRect newRect = m_rectTime.UnionRect(oldRect);
			if( refreshCamera )
			{
				GUI::CWnd::Refresh(m_rectCamera.m_left,m_rectCamera.m_top,m_rectCamera.Width(),m_rectCamera.Height());
			}
			GUI::CWnd::Refresh(newRect.m_left,newRect.m_top,newRect.Width(),newRect.Height());

			old_x_pos = xPos;
			old_y_pos = yPos;
		}

		return KEY_VIRTUAL_MSG_STOP;
	}

	return KEY_VIRTUAL_MSG_STOP;
}

//end
