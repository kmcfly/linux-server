/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : ysw
** Date         : 2009-08-08
** Name         : LogSearchConditionDlg.h
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/
#ifndef _LOG_SEARCH_CONDITION_DLG_H_
#define _LOG_SEARCH_CONDITION_DLG_H_

#include "Static.h"
#include "Button.h"
#include "Dialog.h"
#include "ListCtrl.h"
#include "TimeCtrl.h"
#include "DateCtrl.h"

using namespace GUI;

class CLogSearchConditionDlg : public GUI::CDialog
{
public:
	CLogSearchConditionDlg();
	~CLogSearchConditionDlg();

	unsigned long GetCondition();

	unsigned long TimeStart();
	unsigned long TimeEnd();

	void SetStartTime(tm time);
	void SetEndTime(tm time);

protected:
	//DECLARE_MSG_MAP()
	void OnInitial();
	void OnPaint(PAINT_STATUS status);

protected:
	GUI::CListCtrl			m_listCtrl;

	GUI::CStatic		m_scStart;
	GUI::CStatic		m_scEnd;

	GUI::CDateCtrl		m_dateStart;
	GUI::CTimeCtrl		m_timeStart;
	GUI::CDateCtrl		m_dateEnd;
	GUI::CTimeCtrl		m_timeEnd;

};
#endif //_LOG_SEARCH_CONDITION_DLG_H_
