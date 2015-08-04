#include "CalendarDlg.h"

using namespace GUI;

CCalendarDlg::CCalendarDlg()
{

}

CCalendarDlg::~CCalendarDlg()
{

}

void CCalendarDlg::OnInitial()
{
	GUI::CDialog::OnInitial();

	m_style |= WND_NO_TITLE;

	m_calendarCtrl.Create(GetRandWndID(), 3, 3, m_length[CALENDAR_WIDTH], m_length[CALENDAR_HEIGHT], this, 0, 0);
}


BEGIN_MSG_MAP(CCalendarDlg, GUI::CDialog)
	ON_MSG_EX(m_calendarCtrl.GetID(), KEY_VIRTUAL_MSG_DEFAULT_TAG, OnClickCal)
END_MSG_MAP()


unsigned long CCalendarDlg::OnClickCal(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	unsigned type = (keyID&(~KEY_VIRTUAL_MSG_DEFAULT_TAG))>>28;
	
	if( type == 1 )
	{
		EndDialog(DLG_OK);
	}

	return KEY_VIRTUAL_MSG_CONTINUE;
}

void GUI::CCalendarDlg::OnOutOfDlg(unsigned long keyID)
{
	GUI::CDialog::OnOutOfDlg(keyID);

	if (KEY_VIRTUAL_ENTER == keyID)
	{
		EndDialog(DLG_ESC);
	}
}

