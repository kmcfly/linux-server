/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : hz
** Date         : 2009-08-12
** Name         : CfgScheduleEditDlg.cpp 
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/

#include "CfgScheduleEditDlg.h"
#include "CalendarDlg.h"

CCfgScheduleEditDlg::CCfgScheduleEditDlg() : m_scheduleMode(SCHEDULE_HOLIDAY)
, m_initScheduleDay(0)
{

}

CCfgScheduleEditDlg::~CCfgScheduleEditDlg()
{

}

void CCfgScheduleEditDlg::OnInitial()
{
	GUI::CDialog::OnInitial();
	SetImage(BMP_MEM_DLG_LARGE, false);
	SetCaption(m_pStrTable->String(E_STR_ID_TITLE_SCHEDULE), false);
	SetDefaultBtn( DEFAULT_BTN_OK | DEFAULT_BTN_CANCEL );
	SetDefaultBtnCaption( DEFAULT_BTN_CANCEL, m_pStrTable->String( E_STR_ID_EXIT ) );
	//////////////////////////////////////////////////////////////////////////

	unsigned long x = m_length[DLG_OFFSET_LEFT] ;
	unsigned long y = m_length[TITLE_HEIGHT] + m_length[DLG_OFFSET_TOP] + 8;
	unsigned long combo_cx = 120;
	unsigned long sc_cx = 100;
	
	m_scWeek.Create(GetRandWndID(),x,y,sc_cx,m_length[EDIT_HEIGHT],this);
	m_scWeek.SetCaption(m_pStrTable->String(E_STR_ID_WEEK_DAYS),false);
	m_comboDate.Create(GetRandWndID(),x+sc_cx+2,y,combo_cx,m_length[COMBO_HEIGHT],this,0,1);
	for ( int i=0;i<7;i++)
	{
		m_comboDate.AddItem(m_pStrTable->String(static_cast<ENUM_STRING_ID>(E_STR_ID_SUNDAY+i)),i);
	}
	m_comboDate.SetDropDownCount(7);
	m_comboDate.SetCurItemData(m_initScheduleDay,false);

	//////////////////////////////
	y += m_length[COMBO_HEIGHT]+12;
	unsigned long listTime_left = m_length[DLG_OFFSET_LEFT] ;
	unsigned long listTime_top = y;
	unsigned long listTime_width = m_rect.Width() - listTime_left * 2;
	unsigned long listTime_height = 0;
	listTime_height = m_rect.Height() - listTime_top -m_length[DEFAULT_BTN_BOTTOM_YPOS] - m_length[DLG_OFFSET_BOTTOM];

	m_listTime.Create(GetRandWndID(), listTime_left, listTime_top, listTime_width, listTime_height, this, 0, 2);
	m_listTime.EnableSingleSelect(true);
	m_listTime.AddColumn(m_pStrTable->String( E_STR_ID_START_TIME ), listTime_width/2);
	m_listTime.AddColumn(m_pStrTable->String( E_STR_ID_END_TIME ), listTime_width/2);

	//////////////////////////////////////////////////////////////////////////
	int btn_top = m_rect.Height() - m_length[DEFAULT_BTN_BOTTOM_YPOS];
	y = btn_top;

	m_btnTimeAdd.Create(GetRandWndID(), x, y, m_length[BTN_WIDTH], m_length[BTN_HEIGHT], this, 0, 3, m_pStrTable->String(E_STR_ID_ADD));

	x += m_length[BTN_WIDTH] + m_length[DEFAULT_BTN_OFFSET];
	m_btnTimeDel.Create(GetRandWndID(), x, y, m_length[BTN_WIDTH], m_length[BTN_HEIGHT], this, 1, 3, m_pStrTable->String(E_STR_ID_DELETE));

	x = m_rect.Width() - m_length[DLG_OFFSET_LEFT] - m_length[BTN_WIDTH] - combo_cx - m_length[DEFAULT_BTN_OFFSET] - 156;
	y = m_length[TITLE_HEIGHT] + m_length[DLG_OFFSET_TOP] + 8;

	m_scCopy.Create(GetRandWndID(), x, y, 150, m_length[EDIT_HEIGHT], this);
	m_scCopy.SetCaption(m_pStrTable->String(E_STR_ID_APPLY_SETTINGS_TO), false);

	x += 150 + 6;
		
	m_comboCopy.Create(GetRandWndID(),x,y,combo_cx,m_length[COMBO_HEIGHT],this,1,1);
	for ( int i=0;i<7;i++)
	{
		m_comboCopy.AddItem(m_pStrTable->String(static_cast<ENUM_STRING_ID>(E_STR_ID_SUNDAY+i)),i);
	}
	m_comboCopy.AddItem(m_pStrTable->String(E_STR_ID_ALL),7);
	m_comboCopy.SetDropDownCount(8);
	m_comboCopy.SetCurItemData(7, false);

	x += combo_cx + m_length[DEFAULT_BTN_OFFSET];
	m_btnCopy.Create(GetRandWndID(),x,y,m_length[BTN_WIDTH],m_length[BTN_HEIGHT],this,2,1);
	m_btnCopy.SetCaption(m_pStrTable->String(E_STR_ID_COPY),false);

	//////////////////////////////////////////////////////////////////////////////
	ShowSectionTime(false);
}

void CCfgScheduleEditDlg::SetWeekSchedule(const WEEK_SCHEDULE& weekSchedule)
{
	m_scheduleMode = SCHEDULE_WEEK;
	m_schedule.clear();
	SCHEDULE_EDIT_INFO info;
	for (int day = 0; day < 7; ++day)
	{
		info.week = day;
		info.section_list.clear();
		CSchedule::ScheduleToTimeSec(info.section_list, weekSchedule.week[day]);
		m_schedule.push_back(info);
	}
}

void CCfgScheduleEditDlg::GetWeekSchedule(WEEK_SCHEDULE& weekSchedule) const
{
	assert(m_schedule.size() == 7);
	for (SCHEDULE_EDITINFO_LIST::const_iterator iter = m_schedule.begin(); iter != m_schedule.end(); ++iter)
	{
		CSchedule::TimeSecToSchedule(iter->section_list, weekSchedule.week[iter->week]);
	}
}

void CCfgScheduleEditDlg::SetHolidaySchedule(const HOLIDAY_SCHEDULE_LIST& list)
{
	m_scheduleMode = SCHEDULE_HOLIDAY;

	m_schedule.clear();
	SCHEDULE_EDIT_INFO info;
	for (HOLIDAY_SCHEDULE_LIST::const_iterator iter = list.begin(); iter != list.end(); ++iter)
	{
		info.year = iter->year;
		info.month = iter->month;
		info.day = iter->day;

		info.section_list.clear();
		CSchedule::ScheduleToTimeSec(info.section_list, iter->date);
		m_schedule.push_back(info);
	}
}

void CCfgScheduleEditDlg::GetHolidaySchedule(HOLIDAY_SCHEDULE_LIST& list) const
{
	list.clear();
	HOLIDAY_SCHEDULE date;
	memset(&date, 0, sizeof(date));
	for (SCHEDULE_EDITINFO_LIST::const_iterator iter = m_schedule.begin(); iter != m_schedule.end(); ++iter)
	{
		date.year = iter->year;
		date.month = iter->month;
		date.day = iter->day;
		CSchedule::TimeSecToSchedule(iter->section_list, date.date);
		list.push_back(date);
	}
}

void CCfgScheduleEditDlg::SetInitScheduleDay(int day)
{
	assert(day >= 0);
	assert(day < 7);
	m_initScheduleDay = day;
}

BEGIN_MSG_MAP(CCfgScheduleEditDlg, GUI::CDialog)
	ON_MSG(m_btnTimeAdd.GetID(), KEY_VIRTUAL_ENTER, OnClickBtnTimeAdd)
	ON_MSG(m_btnTimeDel.GetID(), KEY_VIRTUAL_ENTER, OnClickBtnTimeDel)
	ON_MSG(m_comboDate.GetID(),KEY_VIRTUAL_ENTER,OnClickComboDate)
	ON_MSG(m_btnCopy.GetID(),KEY_VIRTUAL_ENTER,OnBtnCopy)
END_MSG_MAP()


void CCfgScheduleEditDlg::ShowSectionTime(bool bRefresh)
{
	int sel = m_comboDate.GetCurItemData();
	if (sel >= 0)
	{
		m_listTime.RemoveAllItem(false);
		
		const SECTION_LIST& sectionList = m_schedule[sel].section_list;
		char sz[32] = {0};
		int item = 0;
		for (SECTION_LIST::const_iterator iter = sectionList.begin(); iter != sectionList.end(); ++iter)
		{
			sprintf(sz, "%.2d:%.2d", iter->startTimeHour, iter->startTimeMinute/*, iter->start_sec*/);
			m_listTime.AddItem(item, 0, sz, false);

			sprintf(sz, "%.2d:%.2d", iter->endTimeHour, iter->endTimeMinute/*, iter->end_sec*/);
			m_listTime.AddItem(item, 1, sz, false);

			++item;
		}

		if (item > 0)
		{
			m_listTime.SetCurSingleSelect(0, false);
		}

		if (bRefresh)
		{
			m_listTime.Repaint();
		}		
	}
}


unsigned long CCfgScheduleEditDlg::OnClickBtnTimeAdd()
{
	int sel = m_comboDate.GetCurItemData();
	if (sel >= 0)
	{
		GUI::CRect rect = m_btnTimeAdd.GetWindowRect();
		int top = rect.m_top - CCfgScheduleTimeDlg::HEIGHT;
		int left = rect.m_left;

		CCfgScheduleTimeDlg dlg;
		if (DLG_OK == dlg.DoModal(this, left, top, CCfgScheduleTimeDlg::WIDTH, CCfgScheduleTimeDlg::HEIGHT))
		{
			TIME_SEC section;
			dlg.GetSection(section);
			
			m_schedule[sel].section_list.push_back(section);
			
			ShowSectionTime(true);
		}
	}

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CCfgScheduleEditDlg::OnClickBtnTimeDel()
{
	int day = m_comboDate.GetCurItemData();
	if (day >= 0)
	{
		int section = m_listTime.GetCurSingleSelect();
		SECTION_LIST::iterator iter = m_schedule[day].section_list.begin();
		for (int i = 0; iter != m_schedule[day].section_list.end(), i < section; ++iter, ++i)
		{

		}

		if (iter != m_schedule[day].section_list.end())
		{
			m_schedule[day].section_list.erase(iter);
		}

		ShowSectionTime(true);
				
	}
	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CCfgScheduleEditDlg::OnSelectDateList()
{
	ShowSectionTime(true);
	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CCfgScheduleEditDlg::OnClickComboDate()
{
	ShowSectionTime(true);

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CCfgScheduleEditDlg::OnBtnCopy()
{
	int selSor = m_comboDate.GetCurItemData();
	int selCopyto = m_comboCopy.GetCurItemData();

	if( selCopyto != selSor && selCopyto <7 )
	{
		m_schedule[selCopyto].section_list.clear();
		SECTION_LIST::iterator iter;
		for (iter=m_schedule[selSor].section_list.begin();iter!=m_schedule[selSor].section_list.end();iter++)
		{
			m_schedule[selCopyto].section_list.push_back(*iter);
		}
	}

	if( selCopyto >= 7 )
	{
		for ( int i=0;i<7;i++ )
		{
			if( i == selSor )
			{
				continue;
			}
			
			m_schedule[i].section_list.clear();
			SECTION_LIST::iterator iter;
			for ( iter=m_schedule[selSor].section_list.begin();iter!=m_schedule[selSor].section_list.end();iter++)
			{
				m_schedule[i].section_list.push_back(*iter);
			}
		}
	}

	return KEY_VIRTUAL_MSG_STOP;
}
