#include "PictureCtrl.h"

GUI::CPictureCtrl::CPictureCtrl() 
{
	m_ctrlType = GUI::WND_TYPE_PICTURE;
}

GUI::CPictureCtrl::~CPictureCtrl()
{

}

bool GUI::CPictureCtrl::Create(unsigned long ctrlID, unsigned long x, unsigned long y, CWnd* pParent, BITMAP_ID bmpID, bool bVisible/* = true*/)
{
	int cx = 0;
	int cy = 0;
	if (BMP_INVALID != bmpID)
	{
		GUI::CWnd::GetImageSize(bmpID, cx, cy);
	}

	unsigned long style = WND_CHILD;
	if (bVisible)
	{
		style |= WND_VISIBLE;
	}

	if (GUI::CWnd::Create(ctrlID, x, y, cx, cy, pParent, style, 0, 0))
	{
		SetImage(bmpID, false);
		return true;
	}
	else
	{
		return false;
	}
}

void GUI::CPictureCtrl::OnPaint(PAINT_STATUS status)
{
	if (PAINT_STATUS_FOCUS != status)
	{
		if (BMP_INVALID != m_imageID)
		{
			DrawImage(m_imageID, m_rect.m_left, m_rect.m_top);
		} 	
	}
}

void GUI::CPictureCtrl::SetImage(BITMAP_ID id, bool bRefresh)
{
	int cx = 0;
	int cy = 0;
	if (GetImageSize(id, cx, cy))
	{
		m_rect.SetSize(cx, cy);
	} 
	else
	{
		assert(false);
		return;
	}

	GUI::CWnd::SetImage(id, bRefresh);
}
