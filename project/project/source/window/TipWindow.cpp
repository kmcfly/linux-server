//FileName:	TipWindow.cpp


#include "TipWindow.h"
#include "FuncCustom.h"
#include <algorithm>

#include "Button.h"
#include "DrawManager.h"

// GUI::CTipWindow::CTipWindow() : m_bMark(false)
// , m_markColor(COLOR_BTN_DEFAULT_MARK)
// , m_alignText(ALIGN_TEXT_CENTER)
// , m_bDrawBorder(true)
// {
// 	m_ctrlType = GUI::WND_TYPE_STATIC;
// 	m_color = COLOR_BTN_BK;
// 	m_color_focus = COLOR_SELECT;
// }

GUI::CTipWindow::CTipWindow()
{
	
}

GUI::CTipWindow::~CTipWindow()
{

}

unsigned long  GUI::CTipWindow::GetShowTime()
{
	return m_showTime;
}

void GUI::CTipWindow::SetShowTime(unsigned long time)
{
	m_showTime = time;
}

void GUI::CTipWindow::SetRect(int left, int top, int width, int height)
{
	m_oldLeft	= left;
	m_oldTop	= top;
	m_oldWidth	= width;
	m_oldHeight	= height;
}

bool GUI::CTipWindow::isRectEq(int left, int top, int width, int height)
{
	return (left == m_oldLeft && top == m_oldTop && width == m_oldWidth && height == m_oldHeight);
}
//end
