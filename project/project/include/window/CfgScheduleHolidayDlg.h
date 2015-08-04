/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : hz
** Date         : 2009-08-12
** Name         : CfgScheduleHolidayDlg.h
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/

#ifndef _CFG_SCHEDULE_HOLIDAY_DLG_H_
#define _CFG_SCHEDULE_HOLIDAY_DLG_H_

#include "Dialog.h"
#include "ScheduleListCtrl.h"
#include "CfgScheduleEditDlg.h"

using namespace GUI;

class CCfgScheduleHolidayDlg : public GUI::CDialog
{
public:
	CCfgScheduleHolidayDlg();
	~CCfgScheduleHolidayDlg();

	void SetHolidaySchedule(const HOLIDAY_SCHEDULE_LIST& list);
	void GetHolidaySchedule(HOLIDAY_SCHEDULE_LIST& list) const;

protected:
	DECLARE_MSG_MAP()
	void OnInitial();
	void OnDestroy();

	void SetTip();

	unsigned long OnClickSchedule();

	void ShowSchedule(bool bRefresh);

protected:
	GUI::CScheduleListCtrl m_scheduleHolidayCtrl;
	HOLIDAY_SCHEDULE_LIST m_holidayList;
};

#endif
