/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : hz
** Date         : 2009-08-12
** Name         : CfgScheduleEditDlg.h 
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/

#ifndef _CFG_SCHEDULE_EDIT_DLG_H_
#define _CFG_SCHEDULE_EDIT_DLG_H_

#include <vector>
#include "Dialog.h"
#include "ListCtrl.h"
#include "CfgScheduleTimeDlg.h"
#include "Schedule.h"

using namespace GUI;

typedef std::list<HOLIDAY_SCHEDULE> HOLIDAY_SCHEDULE_LIST;

class CCfgScheduleEditDlg : public GUI::CDialog
{
public:
	CCfgScheduleEditDlg();
	~CCfgScheduleEditDlg();

	enum SCHEDULE_MODE
	{
		SCHEDULE_WEEK,
		SCHEDULE_HOLIDAY,
	};

	void SetWeekSchedule(const WEEK_SCHEDULE& weekSchedule);
	void GetWeekSchedule(WEEK_SCHEDULE& weekSchedule) const;

	void SetHolidaySchedule(const HOLIDAY_SCHEDULE_LIST& list);
	void GetHolidaySchedule(HOLIDAY_SCHEDULE_LIST& list) const;

	void SetInitScheduleDay(int day);

protected:
	DECLARE_MSG_MAP()
	void OnInitial();

	unsigned long OnClickBtnTimeAdd();
	unsigned long OnClickBtnTimeDel();
	unsigned long OnSelectDateList();

	unsigned long OnClickComboDate();
	unsigned long OnBtnCopy();

	void ShowSectionTime(bool bRefresh);

	GUI::CListCtrl m_listTime;
	GUI::CButton m_btnTimeAdd;
	GUI::CButton m_btnTimeDel;

	GUI::CStatic	m_scWeek;
	GUI::CComboCtrl m_comboDate;

	GUI::CStatic	m_scCopy;
	GUI::CComboCtrl m_comboCopy;
	GUI::CButton	m_btnCopy;

	typedef std::list<TIME_SEC> SECTION_LIST;

	struct SCHEDULE_EDIT_INFO 
	{
		SECTION_LIST section_list;
		//SCHEDULE_HOLIDAY有效
		unsigned short year;
		unsigned char month;//0 ~
		unsigned char day;//0 ~
		//SCHEDULE_WEEK有效
		unsigned short week;//星期天 0 
	};

	typedef std::vector<SCHEDULE_EDIT_INFO> SCHEDULE_EDITINFO_LIST;
	SCHEDULE_EDITINFO_LIST m_schedule;

	
	SCHEDULE_MODE m_scheduleMode;

	int m_initScheduleDay;

protected:
};

#endif

