//CalendarCtrl.cpp
#include "CalendarCtrl.h"
#include "CtrlID.h"

using namespace GUI;

GUI::CCalendarCtrl::CCalendarCtrl(): m_day(0)
{
	m_ctrlType = GUI::WND_TYPE_CALENDAR;
}

GUI::CCalendarCtrl::~CCalendarCtrl()
{

}

bool GUI::CCalendarCtrl::Create(unsigned long ctrlID, 
								 unsigned long x, 
								 unsigned long y, 
								 unsigned long cx, 
								 unsigned long cy, 
								 CWnd* pParent, 
								 int xFocus, 
								 int yFocus)
{
	cx = (cx < m_length[CALENDAR_WIDTH]) ? m_length[CALENDAR_WIDTH] : cx;
	cy = (cy < m_length[CALENDAR_HEIGHT]) ? m_length[CALENDAR_HEIGHT] : cy;

	return GUI::CWnd::Create(ctrlID, x, y, m_length[CALENDAR_WIDTH], m_length[CALENDAR_HEIGHT], pParent, WND_CHILD | WND_VISIBLE, xFocus, yFocus);
}

unsigned long GUI::CCalendarCtrl::Time()
{
	tm time;
	memset(&time, 0, sizeof(tm));

	time.tm_year	= m_comboYear.GetCurItem() + 2008 - 1900;
	time.tm_mon		= m_comboMonth.GetCurItem();
	time.tm_mday	= m_day + 1;

	return TmToDVRTime32(time);
}

void GUI::CCalendarCtrl::SetDayInfo(unsigned long info)
{
	assert(false);//暂时禁用
	/*for (int i=0; i<31; ++i)
	{
		if (info & (0x01 << i))
		{
			m_btnDay[i].SetBkColor(COLOR_BTN_DEFAULT_MARK);
		}
		else
		{
			m_btnDay[i].SetBkColor(COLOR_BTN_BK);
		}
	}*/
}

//////////////////////////////////////////////////////////////////////////
BEGIN_MSG_MAP(GUI::CCalendarCtrl, GUI::CWnd)
	ON_MSG(CTRLID_CALENDAR_COMBO_YEAR, KEY_VIRTUAL_ENTER, OnRefreshCalendar)
	ON_MSG(CTRLID_CALENDAR_COMBO_MONTH, KEY_VIRTUAL_ENTER,	OnRefreshCalendar)
	ON_MSG(CTRLID_NULL, KEY_VIRTUAL_ADD, OnRefreshCalendar)
	ON_MSG(CTRLID_NULL, KEY_VIRTUAL_DEC,	OnRefreshCalendar)
	ON_MSG_CTRL_RANGE(CTRL_ID_DAY_1, CTRL_ID_DAY_42, KEY_VIRTUAL_ENTER, OnSelectDay);
END_MSG_MAP()

void GUI::CCalendarCtrl::OnInitial()
{
	GUI::CWnd::OnInitial();

	unsigned short x = 2, y = 2, offset = 5;
	//////////////////////////////////////////////////////////////////////////
	m_comboYear.Create(CTRLID_CALENDAR_COMBO_YEAR, x, y, m_length[CALENDAR_WIDTH]/2, m_length[COMBO_HEIGHT], this, 0, 0);
	x += (m_length[CALENDAR_WIDTH]/2)+offset;
	offset = 9;
	m_comboMonth.Create(CTRLID_CALENDAR_COMBO_MONTH, x, y, m_length[CALENDAR_WIDTH]/2-offset, m_length[COMBO_HEIGHT], this, 1, 0);

	char year [32] = "2008";
	for (int i=2008; i<=2025; ++i)
	{
		snprintf(year, 32, "%d", i);
		m_comboYear.AddItem(year);
	}

	for (int i=1; i<=12; ++i)
	{
		snprintf(year, 32, "%d", i);
		m_comboMonth.AddItem(year);
	}
	//////////////////////////////////////////////////////////////////////////
	char *pWeek[128] = {(char *)(m_pStrTable->String(E_STR_ID_SUN)),\
						(char *)(m_pStrTable->String(E_STR_ID_MON)),\
						(char *)(m_pStrTable->String(E_STR_ID_TUE)),\
						(char *)(m_pStrTable->String(E_STR_ID_WED)),\
						(char *)(m_pStrTable->String(E_STR_ID_THU)),\
						(char *)(m_pStrTable->String(E_STR_ID_FRI)),\
						(char *)(m_pStrTable->String(E_STR_ID_SAT))};

	x = 3; y = 33;
	for (int i=0; i<7; ++i)
	{
		m_weekDay[i].Create(CTRLID_STATIC, x, y, m_length[CALENDAR_BTN_WIDTH] - 6, 30, this, pWeek[i]);
		x += m_length[CALENDAR_BTN_WIDTH];
	}
	
	//////////////////////////////////////////////////////////////////////////
	char day [32] = "31";

	x = 0;
	y = 60 - m_length[CALENDAR_BTN_HEIGHT];
	for (int i=0; i<42; ++i)
	{
		if(0 == i % 7)
		{
			x = 0;
			y += m_length[CALENDAR_BTN_HEIGHT];
		}

		snprintf(day, 32, "%d", i+1);
		m_btnDay[i].Create(CTRL_ID_DAY_1+i, x, y, m_length[CALENDAR_BTN_WIDTH], m_length[CALENDAR_BTN_HEIGHT], this, i%7, (i/7)+1, day, WND_CHILD | WND_VISIBLE | WND_NO_IMAGE_BK);
		m_btnDay[i].Show( false, false );
	
		x += m_length[CALENDAR_BTN_WIDTH];
	}

	//
	tm time = DVRTime32ToTm(GetCurrTime32());
	m_comboYear.SetCurItem((time.tm_year+1900-2008));
	m_comboMonth.SetCurItem(time.tm_mon);

	RefreshCalendar(time.tm_year+1900, time.tm_mon+1);

	m_day = time.tm_mday-1;
}

void GUI::CCalendarCtrl::OnDestroy()
{
	ClearMarkDay(false);
	SetDefaultBtnBK();
	GUI::CWnd::OnDestroy();
}

void GUI::CCalendarCtrl::OnPaint(PAINT_STATUS status)
{
	GUI::CWnd::DrawRect(m_rect.m_left, m_rect.m_top, m_rect.Width(), m_rect.Height(), COLOR_DLG_CHILD_BK);
	GUI::CWnd::DrawRectBorder(m_rect.m_left, m_rect.m_top, m_rect.Width(), m_rect.Height(), COLOR_LINE_GRAY);

	unsigned short x = m_rect.m_left;
	unsigned short y = m_rect.m_top+30;

	GUI::CWnd::DrawLine(m_rect.m_left, m_rect.m_top+30, m_rect.m_right, m_rect.m_top+30, COLOR_LINE_GRAY);

	y += 30;
	for (int i=0; i<6; ++i)
	{
		GUI::CWnd::DrawLine(m_rect.m_left, y, m_rect.m_right, y, COLOR_LINE_GRAY);
		y += m_length[CALENDAR_BTN_HEIGHT];
	}

	x += m_length[CALENDAR_BTN_WIDTH];
	y = m_rect.m_top+30;
	for (int i=0; i<6; ++i)
	{
		GUI::CWnd::DrawLine(x, y, x, m_rect.m_bottom, COLOR_LINE_GRAY);
		x += m_length[CALENDAR_BTN_WIDTH];
	}

	assert(m_pIndexBtnDay[m_day]);
	m_pIndexBtnDay[m_day]->SetBkColor(COLOR_CAL_TODAYBTN_BK, false);
}

void GUI::CCalendarCtrl::SetMarkDayColor()
{
	for (int i = 0; i < 42; ++i)
	{
		m_btnDay[i].SetMark(false);
	}

	unsigned short cur_year = static_cast<unsigned short>(GetSelYear());
	unsigned char cur_month = static_cast<unsigned char>(GetSelMonth() + 1);

	int markday_count = 0;
	MARKDAY_LIST::const_iterator iter;
	for (iter = m_markDayList.begin(); iter != m_markDayList.end(); ++iter)
	{
		if ((iter->year == cur_year) && (iter->month == cur_month))
		{
			assert(iter->day - 1 < 31);
			assert(m_pIndexBtnDay[iter->day - 1]);
			m_pIndexBtnDay[iter->day - 1]->SetMark(true, iter->color, false);

			if (++markday_count >= 31)
			{
				break;
			}
		}
	}
}

void GUI::CCalendarCtrl::RefreshCalendar(unsigned short year, unsigned char month)
{
	for (int i = 0; i < 31; i++)
	{
		m_pIndexBtnDay[i] = NULL;
	}

	unsigned char dayNum = m_calendar.GetMonthDays(year, month);
	unsigned char weekDay = m_calendar.GetWeekday(year, month, 1);

	//修正当前日期
	if(m_day >= dayNum)
	{
		m_day = 0;
	}

	unsigned char day = 1; //日期
	char str[32] = {0};
	for (int i = 0; i < 42; ++i)
	{
		if(i < weekDay)
		{
			m_btnDay[i].SetData(static_cast<ULONGLONG>(-1));
			if(m_btnDay[i].IsShow())
			{
				m_btnDay[i].Show(false, false);
			}
			continue;
		}
		else if(i == weekDay)
		{
			if(day <= dayNum)
			{
				snprintf(str, 32, "%d", day);
				m_btnDay[i].SetData(day);
				m_btnDay[i].SetCaption(str, false);
				if(!m_btnDay[i].IsShow())
				{
					m_btnDay[i].Show(true, false);
				}

				m_pIndexBtnDay[day - 1] = &m_btnDay[i];
			}
			else
			{
				m_btnDay[i].SetData(static_cast<ULONGLONG>(-1));
				if(m_btnDay[i].IsShow())
				{
					m_btnDay[i].Show(false, false);
				}
			}

			++ weekDay;
			++ day;
		}
		else
		{
			assert(false);
		}
	}

	SetMarkDayColor();

	SetDefaultBtnBK();
}

unsigned long GUI::CCalendarCtrl::OnRefreshCalendar()
{
	unsigned short year = m_comboYear.GetCurItem() + 2008;
	unsigned char month = m_comboMonth.GetCurItem() + 1;

	RefreshCalendar(year, month);

	Repaint();

	return KEY_VIRTUAL_MSG_CONTINUE;
}

unsigned long GUI::CCalendarCtrl::OnSelectDay(unsigned long ctrlID)
{
	int index = -1;
	for (int i = 0; i < 31; i++)
	{
		if(m_pIndexBtnDay[i] && m_pIndexBtnDay[i]->GetID() == ctrlID)
		{
			index = i;
		}
	}
	
	if(index < 0)
	{
		//无效值
		assert(false);
		return KEY_VIRTUAL_MSG_STOP;
	}

	assert(m_pIndexBtnDay[m_day]);
	m_pIndexBtnDay[m_day]->SetBkColor(COLOR_BTN_BK, true);
	m_day = static_cast<unsigned char>(index);
	m_pIndexBtnDay[m_day]->SetBkColor(COLOR_CAL_TODAYBTN_BK, true);

	unsigned long year = GetSelYear();
	unsigned long mon = GetSelMonth();
	unsigned long day = GetSelDay();

	//前三位表示特定含义 + 日(27~22) + 月(21~16) + 年(15~0)
	return KEY_VIRTUAL_MSG_DEFAULT_TAG |
				(static_cast<unsigned long>(0x1)<<28) |
				(day<<22) | (mon<<16) | year;

	//return KEY_VIRTUAL_MSG_CONTINUE;
}

void GUI::CCalendarCtrl::MarkDay(unsigned short year, unsigned char month, unsigned char day, unsigned long color /* = COLOR_BTN_DEFAULT_MARK */, bool bRefresh /* = false */)
{
	MAKR_DAY_INFO info;
	info.year = year;
	info.month = month;
	info.day = day;
	info.color = color;
	m_markDayList.push_back(info);

	unsigned short cur_year = static_cast<unsigned short>(GetSelYear());
	unsigned char cur_month = static_cast<unsigned char>(GetSelMonth() + 1);
	if ((year == cur_year) && (cur_month == month))
	{
		assert(day - 1 < 31);
		assert(m_pIndexBtnDay[day - 1]);
		m_pIndexBtnDay[day - 1]->SetMark(true, color, bRefresh);
	}

	if (bRefresh)
	{
		Repaint();
	}
}

void GUI::CCalendarCtrl::ClearMarkDay(bool bRefresh /* = false */)
{
	m_markDayList.clear();
	for (int i = 0; i < 42; ++i)
	{
		m_btnDay[i].SetMark(false);
	}

	if (bRefresh)
	{
		Repaint();
	}
}

void GUI::CCalendarCtrl::SetDefaultBtnBK(bool bRefresh /* = false */)
{
	for (int i = 0; i < 42; ++i)
	{
		m_btnDay[i].SetBkColor(COLOR_BTN_BK, bRefresh);
	}
}
//end


