#ifndef _CALENDAR_CTRL_H_
#define _CALENDAR_CTRL_H_

#include <vector>
#include "Wnd.h"
#include "Static.h"
#include "Button.h"
#include "ComboCtrl.h"
#include "Calendar.h"

namespace GUI
{
	class CCalendarCtrl : public GUI::CWnd
	{
	public:
		CCalendarCtrl();
		virtual ~CCalendarCtrl();

		virtual bool Create( unsigned long ctrlID, 
			unsigned long x, 
			unsigned long y,
			unsigned long cx,
			unsigned long cy,
			CWnd* pParent,
			int xFocus,
			int yFocus);

		unsigned long Time();
		void SetDayInfo(unsigned long info);
		void MarkDay(unsigned short year, unsigned char month, unsigned char day, unsigned long color = COLOR_BTN_DEFAULT_MARK, bool bRefresh = false);//月、日由1开始
		void ClearMarkDay(bool bRefresh = false);

		unsigned char GetSelDay() const { return m_day; }
		int GetSelMonth() const { return m_comboMonth.GetCurItem(); }//0开始
		int GetSelYear() const { return m_comboYear.GetCurItem() + 2008; }

	protected:
		DECLARE_MSG_MAP()
		virtual void OnInitial();
		virtual void OnDestroy();
		virtual void OnPaint(PAINT_STATUS status);

		void RefreshCalendar(unsigned short year, unsigned char month);
		unsigned long OnRefreshCalendar();
		unsigned long OnSelectDay(unsigned long ctrlID);

		void SetMarkDayColor();
		void SetDefaultBtnBK(bool bRefresh = false);
	private:
		//////////////////////////////////////////////////////////////////////////
		unsigned char m_day;
		//unsigned long m_dayInfo;
		//////////////////////////////////////////////////////////////////////////
		CCalendar	m_calendar;
		GUI::CStatic m_weekDay[7];
		GUI::CButton m_btnDay[42];		//创建的按钮
		GUI::CButton* m_pIndexBtnDay[31]; //按日期顺序的索引
		GUI::CComboCtrl m_comboYear;
		GUI::CComboCtrl m_comboMonth;
		//////////////////////////////////////////////////////////////////////////
		enum
		{
			CTRL_ID_DAY_1 = 1,
			CTRL_ID_DAY_42 = 42,
		};

		struct MAKR_DAY_INFO 
		{
			unsigned short year;
			unsigned char month;//由1开始
			unsigned char day;//由1开始
			unsigned long color;
		};
		typedef std::vector<MAKR_DAY_INFO> MARKDAY_LIST;
		MARKDAY_LIST m_markDayList;
	};
}

#endif//_CALENDAR_CTRL_H_
