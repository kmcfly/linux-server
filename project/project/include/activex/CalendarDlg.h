#ifndef _CALENDAR_DLG_H_
#define _CALENDAR_DLG_H_

#include "Dialog.h"
#include "CalendarCtrl.h"


namespace GUI
{
	class CCalendarDlg : public GUI::CDialog
	{
	public:
		CCalendarDlg();
		virtual ~CCalendarDlg();

		GUI::CCalendarCtrl* GetCalCtrl() { return &m_calendarCtrl;}

	protected:
		DECLARE_MSG_MAP()
		void OnInitial();
		void OnOutOfDlg(unsigned long keyID);

		unsigned long OnClickCal(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);

		
		GUI::CCalendarCtrl m_calendarCtrl;
	private:
	

	};
}

#endif//_CALENDAR_DLG_H_
