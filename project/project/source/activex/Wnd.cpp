#include "Wnd.h"
#include "CtrlID.h"

using namespace GUI;

int GUI::CWnd::s_randWndID = RAND_CTRLID;//随机窗口ID

GUI::CFBInterface *GUI::CWnd::m_pFBInterface = GUI::CFBInterface::Instance();
unsigned short GUI::CWnd::m_width = 0;
unsigned short GUI::CWnd::m_height = 0;
unsigned short GUI::CWnd::m_length [LENGTH_NUM];
CStringTable	*GUI::CWnd::m_pStrTable = CStringTable::Instance();
GUI::CWnd		*GUI::CWnd::m_pRootWnd = NULL;

GUI::CWnd::CWnd() : m_bShow(false), \
					m_bFocus(false), \
					m_bEnable(true), \
					m_pParent(NULL), \
					m_pActiveCtrl(NULL), \
					m_hCtrlNum(0), \
					m_vCtrlNum(0), \
					m_xFocus(-1), \
					m_yFocus(-1), \
					m_pFocusMap(NULL), \
					m_defaultFocus(DEFUALT_FOCUS_TOP_LEFT), \
					m_windowID(INVALID_ID), \
					m_data(0), \
					m_imageID(BMP_INVALID), \
					m_style(0)
{
}

GUI::CWnd::~CWnd()
{
	m_bEnable = false;
	m_ctrlType = WND_TYPE_INVALID;

	assert(!m_bShow);
	assert(!m_bFocus);
	assert(!m_bEnable);
	if (0 != m_style)
	assert(0 == m_style);
	assert(INVALID_ID == m_windowID);
	assert(WND_TYPE_INVALID == m_ctrlType);
	assert(BMP_INVALID == m_imageID);

	assert(NULL == m_pParent);
	assert(NULL == m_pActiveCtrl);
	assert(0 == m_hCtrlNum);
	assert(0 == m_vCtrlNum);
	assert(-1 == m_xFocus);
	assert(-1 == m_yFocus);
	assert(DEFUALT_FOCUS_TOP_LEFT == m_defaultFocus);

	assert(0 == m_data);
}

void GUI::CWnd::SetSize(unsigned short width, unsigned short height)
{
	m_width = width;
	m_height = height;

	m_length[FONT_WIDTH]		= 20;
	m_length[RECT_WIDTH]		= 640;
	m_length[RECT_HEIGHT]		= 420;
	m_length[TITLE_HEIGHT]		= 40;
	m_length[BTN_WIDTH]			= 88;
	m_length[BTN_HEIGHT]		= 25;
	m_length[STATIC_HEIGHT]		= 24;
	m_length[EDIT_HEIGHT]		= 24;
	m_length[COMBO_ARROW_WIDTH] = 24;
	m_length[COMBO_WIDTH]		= 20;
	m_length[COMBO_HEIGHT]		= 24;
	m_length[CHECK_WIDTH]		= 24;
	m_length[CHECK_HEIGHT]		= 24;
	m_length[LIST_ITEM_HEIGHT]	= 30;
	m_length[LIST_HEAD_HEIGHT]	= 24;
	m_length[LIST_EX_ITEM_HEIGHT]	= 28;
	m_length[RADIO_WIDTH]		= 24;
	m_length[RADIO_HEIGHT]		= 24;
	m_length[SCROLL_WIDTH]		= 18;
	m_length[SLIDER_WIDTH]		= 18;
	m_length[TABLE_HEIGHT]		= 30;
	m_length[FLEX_BTN_WIDTH]	= 10;
	m_length[FLEX_BTN_HEIGHT]	= 78;

	m_length[CALENDAR_WIDTH]	= 252;
	m_length[CALENDAR_HEIGHT]	= 210;
	m_length[CALENDAR_BTN_WIDTH]	= 36;
	m_length[CALENDAR_BTN_HEIGHT]	= 25;
	m_length[CALENDARDLG_WIDTH]		= m_length[CALENDAR_WIDTH] + 6;
	m_length[CALENDARDLG_HEIGHT]	= m_length[CALENDAR_HEIGHT] + 6;

	m_length[TIME_CTRL_WIDTH] = 132,
	m_length[TIME_CTRL_HEIGHT] = 24,
	m_length[DATE_CTRL_WIDTH] = 150,
	m_length[DATE_CTRL_HEIGHT] = 24,

	m_length[IP_CTRL_HEIGHT]  = 24,

	m_length[SCHEDULE_ITEM_HEIGHT]  = 24,
	m_length[SCHEDULE_HEADER_HEIGHT]  = 22,

	m_length[ANCHOR_WIDTH] = 220,
	m_length[ANCHOR_HEIGHT] = 48,

	m_length[TITLE_EXIT_BTN_RIGHT_XPOS] = 60;
	m_length[TITLE_EXIT_BTN_YPOS] = 12;
	m_length[TITLE_EXIT_BTN_WIDTH] = 30;
	m_length[TITLE_EXIT_BTN_HEIGHT] = 24;

	m_length[TITLE_CAPTION_XPOS] = 30;
	m_length[TITLE_CAPTION_YPOS] = 12;

	// 对话框偏移量参考值
	m_length[DLG_OFFSET_LEFT] = 15;
	m_length[DLG_OFFSET_TOP] = 12;
	m_length[DLG_OFFSET_BOTTOM] = 8;
	m_length[DLG_OFFSET_SUB_LEFT] = 10;
	m_length[DLG_OFFSET_SUB_TOP] = 10;
	m_length[DLG_OFFSET_SUB_BOTTOM] = 8;
	m_length[DLG_OFFSET_X] = 10;
	m_length[DLG_OFFSET_Y] = 8;

	m_length[DEFAULT_BTN_RIGHT_XPOS] = m_length[DLG_OFFSET_LEFT]/*15*//* + m_length[BTN_WIDTH]*/;
	m_length[DEFAULT_BTN_BOTTOM_YPOS] = 12 + m_length[BTN_HEIGHT];
	m_length[DEFAULT_BTN_OFFSET] = 12;

	m_length[STATUS_INFO_XPOS] = m_length[DEFAULT_BTN_RIGHT_XPOS];
	m_length[STATUS_INFO_BOTTOM_YPOS] = m_length[DEFAULT_BTN_BOTTOM_YPOS];

	m_length[EDIT_XPOS]			= 3;
	m_length[EDIT_YPOS]			= 2;
	m_length[TABLE_XPOS]		= 5;
	m_length[TABLE_YPOS]		= 5;
	m_length[STATIC_XPOS]		= 0;
	m_length[STATIC_YPOS]		= 2;
	m_length[LIST_ITEM_XPOS]	= 4;
	m_length[LIST_ITEM_YPOS]	= 4;
	m_length[COMBO_XPOS]		= 2;
	m_length[COMBO_YPOS]		= 2;

	m_length[MSG_PICTURE_XPOS]	= 15;
	m_length[MSG_PICTURE_YPOS]	= 60;
	m_length[MSG_TEXT_XPOS]		= 100;
	m_length[MSG_TEXT_YPOS]		= 70;
}

GUI::CWnd* GUI::CWnd::GetRootWnd()
{
	if( NULL == m_pRootWnd )
	{
		m_pRootWnd = new GUI::CWnd();
		m_pRootWnd->m_bShow = true;
		m_pRootWnd->m_rect.SetSize(m_width,m_height);
	}

	return m_pRootWnd;
}

void GUI::CWnd::RefreshLive()
{
	m_pFBInterface->RefreshLive();
}

void GUI::CWnd::Quit()
{
	if( NULL != m_pRootWnd )
	{
		m_pRootWnd->m_bShow = false;
		delete m_pRootWnd;
		m_pRootWnd = NULL;
	}
}

bool GUI::CWnd::Create( unsigned long ctrlID, 
							 unsigned long x, 
							 unsigned long y, 
							 unsigned long cx, 
							 unsigned long cy, 
							 CWnd* pParent, 
							 unsigned long style,
							 int xFocus,
							 int yFocus)
{
	m_pParent = pParent;
	m_style = style;
	m_windowID = ctrlID;
	m_xFocus = xFocus;
	m_yFocus = yFocus;
	m_bEnable = true;

	if (style & WND_VISIBLE)
	{
		m_bShow = true;
	} 
	else
	{
		m_bShow = false;
	}

	if ((NULL != pParent) && (WND_CHILD & style))
	{	
		//对于具有父窗口，并且是子窗口风格，在父窗的子窗口链表中，加入本窗
		bool ret = pParent->AddChildList(this);
		assert(ret);//与父窗口ID相同，会出现断言
	}

	SetWndPos(x, y, cx, cy);

	OnInitial();
	OnPostInitial();

	if(!CreateFocusMap())
	{
		//assert(false);//建立焦点地图失败
		//TRACE("建立焦点地图失败 \n");//
	}

	return true;
}

void GUI::CWnd::Destroy()
{
	WND_LIST::iterator iter;
	for (iter = m_childList.begin(); iter != m_childList.end(); ++iter)
	{
		(*iter)->DestroyChild();
	}

	if (NULL != m_pParent)
	{
		m_pParent->RemoveChild(this);
	}

	if (NULL != m_pFocusMap)
	{
		delete [] m_pFocusMap;
		m_pFocusMap = NULL;
	}

	m_childList.clear();

	OnDestroy();

	m_bShow = false;
	m_bFocus = false;
	m_style = 0;
	m_windowID = INVALID_ID;
	m_imageID = BMP_INVALID;

	m_pParent = NULL;
	m_pActiveCtrl = NULL;
	m_hCtrlNum = 0;
	m_vCtrlNum = 0;
	m_xFocus = -1;
	m_yFocus = -1;
	m_defaultFocus = DEFUALT_FOCUS_TOP_LEFT;

	m_data = 0;
}

void GUI::CWnd::DestroyChild()
{
	WND_LIST::iterator iter;
	for (iter = m_childList.begin(); iter != m_childList.end(); ++iter)
	{
		(*iter)->DestroyChild();
	}

	if (NULL != m_pFocusMap)
	{
		delete [] m_pFocusMap;
		m_pFocusMap = NULL;
	}

	m_childList.clear();

	OnDestroy();

	m_bShow = false;
	m_bFocus = false;
	m_style = 0;
	m_windowID = INVALID_ID;
	m_imageID = BMP_INVALID;

	m_pParent = NULL;
	m_pActiveCtrl = NULL;
	m_hCtrlNum = 0;
	m_vCtrlNum = 0;
	m_xFocus = -1;
	m_yFocus = -1;
	m_defaultFocus = DEFUALT_FOCUS_TOP_LEFT;

	m_data = 0;
}

void GUI::CWnd::SetCaption(const std::string& strCaption, bool bRefresh /* = true */)
{
	m_strCaption = strCaption;

	if (bRefresh)
	{
		Repaint();
	}
}

void GUI::CWnd::Draw()
{
	//如果该窗体不能显示，那么其所有的子窗体都不应该显示
	//为了防止子窗体和父窗体的显示属性不一致，所以在这次判断处理。
	if (IsShow())
	{
		//先绘制本身的窗体，其实可以在OnPaint里再判断是否显示，
		//但是为了防止遗漏，这里做了加强处理，即先行判断是否需要显示。
		GUI::PAINT_STATUS status = PaintStatus();
		assert(PAINT_STATUS_NULL != status);
		assert(m_bShow);
		OnPaint(status);

		//再调用子窗体的Repaint(),具体子窗体是否显示，显示怎样的状态由子窗体自身决定
		WND_LIST::iterator iter;
		for (iter = m_childList.begin(); iter != m_childList.end(); ++iter)
		{
			CWnd* pWnd = *iter;
			assert(NULL != pWnd);

			pWnd->Draw();
		}
		//清除/改变 子窗口的绘制
		OnErase(status);
	}
}

void GUI::CWnd::Clear()
{
	if (NULL != m_pParent)
	{
		//先把自身的属性更改过来，然后调用父窗体的刷新接口
		//当父窗体调用该子窗体时，因为其自身属性的原因将不再刷新窗体，因而达到清空其对应窗体区域的目的
		//同时又不影响其兄弟窗体的显示。

		m_pParent->Draw();
	} 
	else
	{
		m_pFBInterface->Clear(m_showRect.m_left, m_showRect.m_top, m_showRect.Width(), m_showRect.Height());
	}
}

void GUI::CWnd::SetEnable(bool bEnable, bool bRefresh /* = true */)
{
	if (m_bEnable != bEnable)
	{
		m_bEnable = bEnable;
		WND_LIST::iterator iter;
		for (iter = m_childList.begin(); iter != m_childList.end(); ++iter)
		{
			(*iter)->SetEnable(bEnable, false);
		}

		if (bRefresh)
		{
			Repaint();
		}
	}
}

bool GUI::CWnd::AddChildList(CWnd* pNode)
{
	assert(0 != pNode);
	if (pNode->GetID() != m_windowID || m_pRootWnd == this )
	{
		m_childList.push_back(pNode);
		return true;
	}
	else
	{
		return false;
	}	
}

bool GUI::CWnd::RemoveChild(GUI::CWnd* pNode)
{
	for (WND_LIST::iterator iter = m_childList.begin(); iter != m_childList.end(); ++iter)
	{
		if ((*iter) == pNode)
		{
			if (pNode == m_pActiveCtrl)
			{
				m_pActiveCtrl = NULL;
			}

			m_childList.erase(iter);
			return true;
		}
	}
	return false;
}

void GUI::CWnd::Show(bool bShow, bool bRefresh/* = true*/)
{
	if (IsShow() && !bShow && !bRefresh)
	{
		m_pFBInterface->Clear(m_showRect.m_left, m_showRect.m_top, m_showRect.Width(), m_showRect.Height());
	}

	m_bShow = bShow;
	
	if (bRefresh)
	{
		Repaint();
	}
	else
	{
		//某些情况下，要求显示该窗体，但是需要等一批都显示完成后再集体刷新，所以增加该动作，只是绘制，但不刷新。由
		//外部控制集体刷新，基本上在DoModal中。
		if (IsShow())
		{
			Draw();
		}
	}
}

bool GUI::CWnd::IsShow() const
{
	if ((m_bShow) && (NULL != m_pParent))
	{
		return m_pParent->IsShow();
	} 
	else
	{
		return m_bShow;
	}
}

void GUI::CWnd::Repaint()
{
	if (this == GetRootWnd())
	{
		m_pFBInterface->RefreshLive();
	}

	if (IsShow())
	{
		Draw();
	//	Sleep(200); //测试用
	} 
	else
	{
		Clear();
	}

	m_pFBInterface->Refresh(m_rect.m_left, m_rect.m_top, m_rect.Width(), m_rect.Height());
}

PAINT_STATUS GUI::CWnd::PaintStatus()
{
	if (!IsShow())
	{
		return PAINT_STATUS_NULL;
	}
	else
	{
		if (!m_bEnable)
		{
			return PAINT_STATUS_DISABLE;
		}
		else
		{
			if (m_bFocus)
			{
				return PAINT_STATUS_FOCUS;
			}
			else
			{
				return PAINT_STATUS_NORMAL;
			}
		}
	}
}

GUI::CRect GUI::CWnd::GetWindowRect() const
{
	if ((NULL != m_pParent) && (WND_CHILD & m_style))
	{
		GUI::CRect rect = m_pParent->GetWindowRect();
		rect.Modify(m_showRect.m_left, m_showRect.m_top, m_showRect.Width(), m_showRect.Height());
		return rect;
	}
	else
	{
		return m_showRect;
	}
}

void GUI::CWnd::ClientToScreen(unsigned long & x, unsigned long & y) const
{
	unsigned long left = m_rect.m_left;
	unsigned long top = m_rect.m_top;

	x += left;
	y += top;
}

void GUI::CWnd::ScreenToClient(int & x, int & y) const
{
	assert(m_rect.PtInRect(x, y));

	x -= m_rect.m_left;
	y -= m_rect.m_top;
}

void GUI::CWnd::ScreenToClient(CRect& rect) const
{
	ScreenToClient(rect.m_left, rect.m_top);
	ScreenToClient(rect.m_right, rect.m_bottom);
}

unsigned long GUI::CWnd::OnKeyProc(unsigned long keyID, unsigned long xPoint, unsigned long yPoint)
{	
	if (!IsShow() || !m_bEnable)
	{
		return keyID;
	}

	if (((xPoint < m_width) && (yPoint < m_height)) && !IsInTheRect(xPoint, yPoint))
	{
		return keyID;
	}

	//处理活动（具有焦点）窗口的按键响应
	if (NULL != m_pActiveCtrl)
	{
		//活动窗口处理按键消息

		//由于把一个消息送给子窗体前，父窗体需要处理一些相关业务，所以
		//增加了一个处理函数，继承类需要重写该函数来处理其特殊业务，一般情况下直接返回。
		if (KEY_VIRTUAL_MSG_STOP == OnPreCommand(m_pActiveCtrl->GetID(), keyID, xPoint, yPoint))
		{
			return KEY_VIRTUAL_MSG_STOP;
		}

		unsigned long msg = m_pActiveCtrl->OnKeyProc(keyID, xPoint, yPoint);
		if (KEY_VIRTUAL_MSG_STOP != msg)
		{
			if ((KEY_VIRTUAL_MSG_CONTINUE == msg) || (msg == keyID))
			{
				return OnCommand(m_pActiveCtrl->GetID(), keyID, xPoint, yPoint);//本窗口响应按键消息
			}
			else
			{
				return OnCommand(m_pActiveCtrl->GetID(), msg, xPoint, yPoint);//本窗口响应按键消息
			}
		}
		else
		{
			return KEY_VIRTUAL_MSG_STOP;
		}
	}
	else //没有活动窗口
	{
		if (KEY_VIRTUAL_MSG_STOP == OnPreCommand(m_windowID, keyID, xPoint, yPoint))
		{
			return KEY_VIRTUAL_MSG_STOP;
		}
		else
		{
			return OnCommand(m_windowID, keyID, xPoint, yPoint);
		}
	}
}

GUI::CWnd* GUI::CWnd::InWhichActiceCtrl(unsigned long xPoint, unsigned long yPoint)
{
	if (m_childList.empty())
	{
		return NULL;
	}

	//遍历所有控件
	WND_LIST::iterator iter;
	for (iter = m_childList.begin(); iter != m_childList.end(); ++iter)
	{
		CWnd* pCtrl = *iter;
		if ( pCtrl->IsInTheRect(xPoint, yPoint) && pCtrl->CanSetFocus() ) //能设置焦点，表明该控件是活动的
		{
			return pCtrl;
		}
	}

	return NULL;
}

bool GUI::CWnd::IsInTheRect(unsigned long xPoint, unsigned long yPoint) const
{
	return m_showRect.PtInRect(xPoint, yPoint);
}

//bool GUI::CWnd::DrawImage(const std::string& strFile, int x, int y)
//{
//	GUI::CImage image;
//	if (!image.LoadImage(strFile.c_str()))
//	{
//		assert(false);
//		return false;
//	}
//
//	int cx = image.Width();
//	int cy = image.Height();
//	int len = image.Size();
//	if (len <= 0)
//	{
//		assert(false);
//		return false;
//	}
//
//	GUI::CWnd::DrawImage(image.GetDate(), len, x, y, cx, (cx < m_rect.Width()) ? cx : m_rect.Width(), (cy < m_rect.Height()) ? cy : m_rect.Height());
//	return true;
//}

bool GUI::CWnd::DrawImage(BITMAP_ID id, int x, int y)
{
	CImage* pImage = CImageList::Instance()->GetImage(id);
	if (NULL != pImage)
	{
		int cx = pImage->Width();
		int cy = pImage->Height();
		int len = pImage->Size();
		if (len <= 0)
		{
			assert(false);
			return false;
		}

		GUI::CWnd::DrawImage(pImage->GetDate(), len, x, y, cx, (cx < m_rect.Width()) ? cx : m_rect.Width(), (cy < m_rect.Height()) ? cy : m_rect.Height());
		return true;
	}
	else
	{
		return false;
	}
}

bool GUI::CWnd::GetImageSize(BITMAP_ID id, int& cx, int& cy) const
{
	CImage* pImage = CImageList::Instance()->GetImage(id);
	if (NULL != pImage)
	{
		cx = pImage->Width();
		cy = pImage->Height();

		return true;
	}
	else
	{
		return false;
	}
}

//////////////////////////////////////////////////////////////////////////
void GUI::CWnd::DrawClear(unsigned short x, unsigned short y, unsigned short cx, unsigned short cy, bool bImmediate /* = false */)
{
	GUI::CRect rect = m_showRect;

	if ((NULL != m_pParent) && (WND_CHILD & m_style))
	{
		GUI::CRect parentRect = m_pParent->GetWindowRect();
		rect.Modify(parentRect.m_left, parentRect.m_top, parentRect.Width(), parentRect.Height());
	}

	rect.Modify(x, y, cx, cy);

	if ((rect.m_left <= rect.m_right) && (rect.m_top <= rect.m_bottom))
	{
		if ((x <= rect.m_right) && (y <= rect.m_bottom))
		{
			m_pFBInterface->Clear(rect.m_left, rect.m_top, rect.Width(), rect.Height());
		}

		if (bImmediate)
		{
			m_pFBInterface->Refresh(m_rect.m_left, m_rect.m_top, m_rect.Width(), m_rect.Height());
		}
	}
}

void GUI::CWnd::DrawRect(unsigned short x, unsigned short y, unsigned short cx, unsigned short cy, unsigned long color, bool bImmediate /* = false */)
{
	GUI::CRect rect = m_showRect;

	if ((NULL != m_pParent) && (WND_CHILD & m_style))
	{
		GUI::CRect parentRect = m_pParent->GetWindowRect();
		rect.Modify(parentRect.m_left, parentRect.m_top, parentRect.Width(), parentRect.Height());
	}

	rect.Modify(x, y, cx, cy);

	if ((rect.m_left <= rect.m_right) && (rect.m_top <= rect.m_bottom))
	{
		if ((x <= rect.m_right) && (y <= rect.m_bottom))
		{
			m_pFBInterface->DrawRect(rect.m_left, rect.m_top, rect.Width(), rect.Height(), color);
		}

		if (bImmediate)
		{
			m_pFBInterface->Refresh(m_rect.m_left, m_rect.m_top, m_rect.Width(), m_rect.Height());
		}
	}
}

void GUI::CWnd::DrawRectBorder(unsigned short x, unsigned short y, unsigned short cx, unsigned short cy, unsigned long color, \
							   unsigned short height /* = 1 */, unsigned short width /* = 0 */, bool bImmediate /* = false */)
{
	GUI::CRect rect = m_showRect;

	if ((NULL != m_pParent) && (WND_CHILD & m_style))
	{
		GUI::CRect parentRect = m_pParent->GetWindowRect();
		rect.Modify(parentRect.m_left, parentRect.m_top, parentRect.Width(), parentRect.Height());
	}

	rect.Modify(x, y, cx, cy);

	if ((rect.m_left <= rect.m_right) && (rect.m_top <= rect.m_bottom))
	{
		if ((x <= rect.m_right) && (y <= rect.m_bottom))
		{
			width = (0 == width) ? height : width;
			m_pFBInterface->DrawRectBorder(rect.m_left, rect.m_top, rect.Width(), rect.Height(), color, height, width);
		}

		if (bImmediate)
		{
			m_pFBInterface->Refresh(m_rect.m_left, m_rect.m_top, m_rect.Width(), m_rect.Height());
		}
	}
}

void GUI::CWnd::DrawLine(unsigned short x1, unsigned short y1, unsigned short x2, unsigned short y2, unsigned long color,\
						 unsigned short width /* = 1 */, bool bImmediate /* = false */)
{
	GUI::CRect rect = m_showRect;

	if ((NULL != m_pParent) && (WND_CHILD & m_style))
	{
		GUI::CRect parentRect = m_pParent->GetWindowRect();
		rect.Modify(parentRect.m_left, parentRect.m_top, parentRect.Width(), parentRect.Height());
	}

	if (x1 > x2)
	{
		unsigned short temp = x1;
		x1 = x2;
		x2 = temp;
	}

	if (y1 > y2)
	{
		unsigned short temp = y1;
		y1 = y2;
		y2 = temp;
	}

	if (x1 == x2)
	{
		rect.Modify(x1, y1, width, (y2-y1+1));

		if (rect.m_top <= rect.m_bottom)
		{
			if ((rect.m_left < (x1+width)) && (x1 <= rect.m_right))
			{
				unsigned short x = (x1 < rect.m_left) ? rect.m_left : x1;
				width = ((rect.m_right-x+1) >= width) ? width : (rect.m_right-x+1); 
				m_pFBInterface->DrawLine(x, rect.m_top, x, rect.m_bottom, color, width);
			}
		}
	}

	if (y1 == y2)
	{
		rect.Modify(x1, y1, (x2-x1+1), width);

		if (rect.m_left <= rect.m_right)
		{
			if ((rect.m_top < (y1+width)) && (y1 <= rect.m_bottom))
			{
				unsigned short y = (y1 < rect.m_top) ? rect.m_top : y1;
				width = ((rect.m_bottom-y+1) >= width) ? width : (rect.m_bottom-y+1); 
				m_pFBInterface->DrawLine(rect.m_left, y, rect.m_right, y, color, width);
			}
		}
	}

	if (bImmediate)
	{
		m_pFBInterface->Refresh(m_rect.m_left, m_rect.m_top, m_rect.Width(), m_rect.Height());
	}
}

unsigned long GUI::CWnd::DrawText(const char *pStr, unsigned long len, unsigned short x, unsigned short y, \
								  unsigned short cx, unsigned short cy, unsigned long color /* = 0xffffffff */, bool bImmediate /* = false */, unsigned char zoom/* = 1*/, bool bResort /*= true*/)
{
	GUI::CRect rect = m_showRect;

	if ((NULL != m_pParent) && (WND_CHILD & m_style))
	{
		GUI::CRect parentRect = m_pParent->GetWindowRect();
		rect.Modify(parentRect.m_left, parentRect.m_top, parentRect.Width(), parentRect.Height());
	}

	rect.Modify(x, y, cx, cy);

	if ((rect.m_left <= rect.m_right) && (rect.m_top <= rect.m_bottom))
	{
		if ((x <= rect.m_right) && (y <= rect.m_bottom))
		{
			unsigned long res = m_pFBInterface->DrawText(pStr, len, rect.m_left, rect.m_top, rect.Width(), rect.Height(), color, zoom, false, bResort);
			if (bImmediate)
			{
				m_pFBInterface->Refresh(m_rect.m_left, m_rect.m_top, m_rect.Width(), m_rect.Height());
			}
			return res;
		}
		else
		{
			return 0;
		}
	}
	else
	{
		return 0;
	}
}

void GUI::CWnd::DrawImage(const unsigned char *pImage, unsigned long len, unsigned short x, unsigned short y, \
						  unsigned short width, unsigned short cx, unsigned short cy, bool bImmediate /* = false */)
{
	GUI::CRect rect = m_showRect;

	if ((NULL != m_pParent) && (WND_CHILD & m_style))
	{
		GUI::CRect parentRect = m_pParent->GetWindowRect();
		rect.Modify(parentRect.m_left, parentRect.m_top, parentRect.Width(), parentRect.Height());
	}

	rect.Modify(x, y, cx, cy);

	if ((rect.m_left <= rect.m_right) && (rect.m_top <= rect.m_bottom))
	{
		if ((x <= rect.m_right) && (y <= rect.m_bottom))
		{
			m_pFBInterface->DrawImage(pImage, len, rect.m_left, rect.m_top, width, rect.Width(), rect.Height());
		}

		if (bImmediate)
		{
			m_pFBInterface->Refresh(m_rect.m_left, m_rect.m_top, m_rect.Width(), m_rect.Height());
		}
	}
}

bool GUI::CWnd::TextSize(const char *pStr, unsigned long len, unsigned short &cx, unsigned short &cy, unsigned char zoom/* = 1*/)
{
	return m_pFBInterface->TextSize(pStr, len, cx, cy, zoom);
}

void GUI::CWnd::Refresh(unsigned short x, unsigned short y, unsigned short cx, unsigned short cy)
{
	m_pFBInterface->Refresh(x, y, cx, cy);
}

bool GUI::CWnd::ReSortText(char* pText, const char* pSor, int len)
{
	return m_pFBInterface->ReSortText(pText, pSor, len);
}

//////////////////////////////////////////////////////////////////////////
GUI::CWnd* GUI::CWnd::GetTopestWnd()
{
	CWnd* pWnd = NULL;
	CWnd* pParent = this;
	while (NULL != pParent)
	{
		pWnd = pParent;
		pParent = pParent->GetParent();
	}

	return pWnd;
}

void GUI::CWnd::GetFocusCountInMap(int& x, int & y) const
{
	assert(NULL != m_pFocusMap);
	x = m_hCtrlNum;
	y = m_vCtrlNum;
}


void GUI::CWnd::SetWndPos(int x, int y, int cx, int cy)
{
	CRect rect(x, y, x + cx - 1, y + cy - 1);
	SetWndPos(rect);
}

void GUI::CWnd::SetWndPos(const GUI::CRect& rect)
{
	m_rect = rect;
	if (NULL != m_pParent)
	{
		GUI::CRect rect = m_pParent->GetWindowRect();
		m_rect.Offset(rect.m_left, rect.m_top);
	}

	m_showRect = m_rect;
}

void GUI::CWnd::SetImage(BITMAP_ID id, bool bRefresh)
{
	m_imageID = id;
	if (bRefresh)
	{
		Repaint();
	}
}

void GUI::CWnd::MoveWnd(int x, int y)
{
	
	//遍历所有控件
	int offset_x = 0;
	int offset_y = 0;
	GUI::CRect rcChild;
	WND_LIST::iterator iter;
	for (iter = m_childList.begin(); iter != m_childList.end(); ++iter)
	{
		rcChild = (*iter)->GetWindowRect();
		offset_x = rcChild.m_left - m_rect.m_left;
		offset_y = rcChild.m_top - m_rect.m_top;

		(*iter)->MoveWnd(x + offset_x, y + offset_y);
	}
	
	m_rect.MoveToXY(x, y);
	m_showRect.MoveToXY(x, y);
}

void GUI::CWnd::SetWndSize(bool bOriginal/* = true*/, bool bRefresh/* = true*/, int cx/* = 0*/, int cy/* = 0*/)
{
	if (bOriginal)
	{
		m_showRect = m_rect;
	}
	else
	{
		assert ((0 < cx) && (cx <= m_width));
		assert ((0 < cy) && (cy <= m_height));

		m_showRect.SetSize(cx, cy);
	}

	if (bRefresh)
	{
		Repaint();
	}
}

GUI::CWnd* GUI::CWnd::GetCtrl(CTRLID ctrlID)
{
	for (WND_LIST::iterator iter = m_childList.begin(); iter != m_childList.end(); ++iter)
	{
		if ((*iter)->GetID() == ctrlID)
		{
			return *iter;
		}
	}
	return NULL;
}
