/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : hz
** Date         : 2009-08-12
** Name         : CfgScheduleTimeDlg.h 
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/

#ifndef _CFG_SCHEDULE_TIME_DLG_H_
#define _CFG_SCHEDULE_TIME_DLG_H_

#include "Dialog.h"
#include "Static.h"
//#include "TimeCtrlEx.h"
#include "ComboCtrl.h"
#include "GroupCtrl.h"
#include "Schedule.h"

using namespace GUI;

class CCfgScheduleTimeDlg : public GUI::CDialog
{
public:
	CCfgScheduleTimeDlg();
	~CCfgScheduleTimeDlg();

	enum
	{
		WIDTH = 370,
		HEIGHT = 160,
	};

	void GetSection(TIME_SEC& section) const
	{
		section = m_section;
	}

protected:
	//DECLARE_MSG_MAP()
	void OnInitial();
	void OnOutOfDlg(unsigned long keyID);
	unsigned long OnOK();

	GUI::CGroupCtrl m_groupCtrl[2];
//	GUI::CTimeCtrlEx m_startTimeCtrl;
//	GUI::CTimeCtrlEx m_endTimeCtrl;
	GUI::CComboCtrl m_comboStartHour;
	GUI::CComboCtrl m_comboStartMinute;
	GUI::CStatic    m_staticStart[2];
	GUI::CComboCtrl m_comboEndHour;
	GUI::CComboCtrl m_comboEndMinute;
	GUI::CStatic    m_staticEnd[2];


	TIME_SEC m_section;

protected:
};

#endif
