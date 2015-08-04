#include "ScrollCtrl.h"
#include "CtrlID.h"

using namespace GUI;

GUI::CScrollCtrl::CScrollCtrl() : m_bHorizontal(false)
{
	m_ctrlType = GUI::WND_TYPE_SCROLL;
}

GUI::CScrollCtrl::~CScrollCtrl()
{

}

bool GUI::CScrollCtrl::Create(unsigned long ctrlID, 
						  unsigned long x, 
						  unsigned long y, 
						  unsigned long len, 
						  CWnd* pParent, 
						  int xFocus, 
						  int yFocus,
						  bool bHorizontal /*= true*/)
						  
{
	//设置对话框风格
	unsigned long style = WND_CHILD | WND_VISIBLE;
	//尺寸
	m_bHorizontal = bHorizontal;
	unsigned long cx = len;
	unsigned long cy = m_length[SCROLL_WIDTH];
	if (!bHorizontal)
	{
		cx = m_length[SCROLL_WIDTH];
		cy = len;
	}

	//创建窗口
	if (GUI::CWnd::Create(ctrlID, x, y, cx, cy, pParent, style, xFocus, yFocus))
	{
		return true;
	}
	else
	{
		return false;
	}
}

void GUI::CScrollCtrl::OnInitial()
{
	GUI::CWnd::OnInitial();

	if (m_bHorizontal)
	{
		m_btnFirst.Create(CTRLID_SCROLL_BTN_FIRST, 0, 0, m_length[SCROLL_WIDTH], m_length[SCROLL_WIDTH], this, 0, 0);
		m_btnFirst.SetImage(PAINT_STATUS_NORMAL, BMP_SCROLL_BTN_LEFT, false);
		m_btnFirst.SetImage(PAINT_STATUS_FOCUS, BMP_SCROLL_BTN_LEFT_FOCUS, false);
		m_btnFirst.SetImage(PAINT_STATUS_DISABLE, BMP_SCROLL_BTN_LEFT_DISABLE, false);

		m_btnSecond.Create(CTRLID_SCROLL_BTN_SECOND, m_rect.Width() - m_length[SCROLL_WIDTH], 0, m_length[SCROLL_WIDTH], m_length[SCROLL_WIDTH], this, 2, 0);
		m_btnSecond.SetImage(PAINT_STATUS_NORMAL, BMP_SCROLL_BTN_RIGHT, false);
		m_btnSecond.SetImage(PAINT_STATUS_FOCUS, BMP_SCROLL_BTN_RIGHT_FOCUS, false);
		m_btnSecond.SetImage(PAINT_STATUS_DISABLE, BMP_SCROLL_BTN_RIGHT_DISABLE, false);

		m_slider.Create(CTRLID_SCROLL_SLIDER, m_length[SCROLL_WIDTH], 0, m_rect.Width() - m_length[SCROLL_WIDTH] - m_length[SCROLL_WIDTH], this, 1, 0);
		m_slider.SetSliderBoxImage(BMP_SCROLL_BTN, BMP_SCROLL_BTN_FOCUS, BMP_SCROLL_BTN_DISABLE);
		m_slider.SetBKColor(COLOR_SCROLL_BK);
	} 
	else
	{
		m_btnFirst.Create(CTRLID_SCROLL_BTN_FIRST, 0, 0, m_length[SCROLL_WIDTH], m_length[SCROLL_WIDTH], this, 0, 0);
		m_btnFirst.SetImage(PAINT_STATUS_NORMAL, BMP_SCROLL_BTN_TOP, false);
		m_btnFirst.SetImage(PAINT_STATUS_FOCUS, BMP_SCROLL_BTN_TOP_FOCUS, false);
		m_btnFirst.SetImage(PAINT_STATUS_DISABLE, BMP_SCROLL_BTN_TOP_DISABLE, false);

		m_btnSecond.Create(CTRLID_SCROLL_BTN_SECOND, 0, m_rect.Height() - m_length[SCROLL_WIDTH], m_length[SCROLL_WIDTH], m_length[SCROLL_WIDTH], this, 0, 2);
		m_btnSecond.SetImage(PAINT_STATUS_NORMAL, BMP_SCROLL_BTN_BOTTOM, false);
		m_btnSecond.SetImage(PAINT_STATUS_FOCUS, BMP_SCROLL_BTN_BOTTOM_FOCUS, false);
		m_btnSecond.SetImage(PAINT_STATUS_DISABLE, BMP_SCROLL_BTN_BOTTOM_DISABLE, false);

		m_slider.Create(CTRLID_SCROLL_SLIDER, 0, m_length[SCROLL_WIDTH], m_rect.Height() - m_length[SCROLL_WIDTH] - m_length[SCROLL_WIDTH], this, 0, 1, false);
		m_slider.SetSliderBoxImage(BMP_SCROLL_BTN_V, BMP_SCROLL_BTN_V_FOCUS, BMP_SCROLL_BTN_V_DISABLE);
		m_slider.SetBKColor(COLOR_SCROLL_BK);
	}
}

void GUI::CScrollCtrl::OnPaint(PAINT_STATUS status)
{
	GUI::CWnd::DrawRect(m_rect.m_left, m_rect.m_top, m_rect.Width(), m_rect.Height(), COLOR_SCROLL_BK);
}

BEGIN_MSG_MAP(GUI::CScrollCtrl, GUI::CWnd)
//	ON_MSG(CTRLID_SCROLL_SLIDER, KEY_VIRTUAL_ENTER, OnSliderChange)
//	ON_MSG(CTRLID_SCROLL_SLIDER, KEY_VIRTUAL_DRAG, OnSliderChange)
//	ON_MSG(CTRLID_SCROLL_SLIDER, KEY_VIRTUAL_LEFT, OnSliderChange)
//	ON_MSG(CTRLID_SCROLL_SLIDER, KEY_VIRTUAL_RIGHT, OnSliderChange)
	ON_MSG(CTRLID_SCROLL_BTN_FIRST, KEY_VIRTUAL_ENTER, OnBtnFirst)
	ON_MSG(CTRLID_SCROLL_BTN_SECOND, KEY_VIRTUAL_ENTER, OnBtnSecond)
END_MSG_MAP()

//unsigned long GUI::CScrollCtrl::OnSliderChange()
//{
//	int pos =m_slider.GetPos();
//
//	if ((pos == m_slider.GetRangeMin()) || (pos == m_slider.GetRangeMax()))
//	{
//		return KEY_VIRTUAL_MSG_CONTINUE;//滑块移动到两头，交由上层处理
//	} 
//	else
//	{
//		return KEY_VIRTUAL_MSG_STOP;
//	}
//}

unsigned long GUI::CScrollCtrl::OnBtnFirst()
{
	if (m_bHorizontal)
	{
		return m_slider.OnLeftKey();
	} 
	else
	{
		return m_slider.OnUpKey();
	}
}

unsigned long GUI::CScrollCtrl::OnBtnSecond()
{
	if (m_bHorizontal)
	{
		return m_slider.OnRightKey();
	} 
	else
	{
		return m_slider.OnDownKey();
	}
}

