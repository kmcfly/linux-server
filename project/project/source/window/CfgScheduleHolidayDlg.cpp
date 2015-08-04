/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : hz
** Date         : 2009-08-12
** Name         : CfgScheduleHolidayDlg.cpp
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/

#include "CfgScheduleHolidayDlg.h"

CCfgScheduleHolidayDlg::CCfgScheduleHolidayDlg() 
{

}

CCfgScheduleHolidayDlg::~CCfgScheduleHolidayDlg()
{

}

void CCfgScheduleHolidayDlg::OnInitial()
{
	GUI::CDialog::OnInitial();
	SetImage(BMP_MEM_DLG_LARGE, false);
	SetCaption(m_pStrTable->String(E_STR_ID_TITLE_HOLIDAY_SCHEDULE), false);
	SetDefaultBtn( DEFAULT_BTN_OK | DEFAULT_BTN_CANCEL);
	SetDefaultBtnCaption( DEFAULT_BTN_CANCEL, m_pStrTable->String(E_STR_ID_EXIT) );
	//////////////////////////////////////////////////////////////////////////

	int x = m_length[DLG_OFFSET_LEFT];
	int y = m_length[TITLE_HEIGHT] + m_length[DLG_OFFSET_TOP] + 4;
	int cx = m_rect.Width() - 2 * m_length[DLG_OFFSET_LEFT];
	int cy = m_rect.Height() - y - m_length[DEFAULT_BTN_BOTTOM_YPOS] - m_length[DLG_OFFSET_BOTTOM];

	int maxRow = (cy - m_length[SCHEDULE_HEADER_HEIGHT])/m_length[SCHEDULE_ITEM_HEIGHT];
	maxRow -= 1;

	m_scheduleHolidayCtrl.Create(GetRandWndID(), x, y, cx, cy, this, 0, 1, 95, NULL, maxRow);

	ShowSchedule(false);

	/////////
	CreateTipWnd();
}

void CCfgScheduleHolidayDlg::OnDestroy()
{
	GUI::CDialog::OnDestroy();
}

void CCfgScheduleHolidayDlg::SetTip()
{
	AddTip(m_scheduleHolidayCtrl.GetID(),E_STR_ID_TIP_04_06);
}

void CCfgScheduleHolidayDlg::SetHolidaySchedule(const HOLIDAY_SCHEDULE_LIST& list)
{
	m_holidayList = list;

	ShowSchedule(false);
}

void CCfgScheduleHolidayDlg::GetHolidaySchedule(HOLIDAY_SCHEDULE_LIST& list) const
{
	list = m_holidayList;
}


void CCfgScheduleHolidayDlg::ShowSchedule(bool bRefresh)
{
	m_scheduleHolidayCtrl.RemoveAllItem(false);

	char sz[32] = {0};
	int day = 0;
	for (HOLIDAY_SCHEDULE_LIST::const_iterator iter = m_holidayList.begin(); iter != m_holidayList.end(); ++iter, ++day)
	{
		sprintf(sz, "%.4d-%.2d-%.2d", iter->year, iter->month, iter->day);
		m_scheduleHolidayCtrl.AddItem(day, sz, false);

		std::list<TIME_SEC> timeSecList;
		CSchedule::ScheduleToTimeSec(timeSecList, iter->date);

		for (std::list<TIME_SEC>::const_iterator iter_sec = timeSecList.begin(); iter_sec != timeSecList.end(); ++iter_sec)
		{
			m_scheduleHolidayCtrl.AddData(day, (*iter_sec).startTimeHour, (*iter_sec).startTimeMinute, 0, (*iter_sec).endTimeHour, (*iter_sec).endTimeMinute, 0);
		}
	}
}

BEGIN_MSG_MAP(CCfgScheduleHolidayDlg, GUI::CDialog)
	ON_MSG(m_scheduleHolidayCtrl.GetID(), KEY_VIRTUAL_ENTER, OnClickSchedule)
END_MSG_MAP()

unsigned long CCfgScheduleHolidayDlg::OnClickSchedule()
{
	CCfgScheduleEditDlg holidayEditDlg;
	holidayEditDlg.SetHolidaySchedule(m_holidayList);
	if (DLG_OK == holidayEditDlg.DoModal(this))
	{
		holidayEditDlg.GetHolidaySchedule(m_holidayList);

		ShowSchedule(true);
	}
	return KEY_VIRTUAL_MSG_CONTINUE;
}

