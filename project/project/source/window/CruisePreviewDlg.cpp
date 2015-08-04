
#include "CruisePreviewDlg.h"

CCruisePreviewDlg::CCruisePreviewDlg()
{

}

CCruisePreviewDlg::~CCruisePreviewDlg()
{

}

//protected:

void CCruisePreviewDlg::OnInitial()
{
	GUI::CDialog::OnInitial();
	m_style |= WND_NO_TITLE;

	CreateFocusMap();
}


void CCruisePreviewDlg::OnPaint(PAINT_STATUS status)
{
	DrawClear( 0 , 0, m_rect.m_right, m_rect.m_bottom, false);
}
