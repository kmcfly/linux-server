/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : ysw
** Date         : 2009-08-01
** Name         : SearchConditionDlg.h
** Version      : 1.0
** Description  : 
** Modify Record:
1:  注意： 通道总数有外面传入
***********************************************************************/
#ifndef _SEARCH_CONDITION_DLG_H_
#define _SEARCH_CONDITION_DLG_H_
#include "Button.h"
#include "CheckCtrl.h"
#include "Dialog.h"
#include "CalendarCtrl.h"

using namespace GUI;

class CSearchConditionDlg : public GUI::CDialog
{
public:
	CSearchConditionDlg();
	~CSearchConditionDlg();

	//////////////////////////////////////////////////////////////////////////
	void SetInitialInfo(unsigned long channelNum, ULONGLONG authCH);
	unsigned long Time();
	ULONGLONG CH();	
	void MarkDay(unsigned short year, unsigned char month, unsigned char day, unsigned long color = COLOR_BTN_DEFAULT_MARK, bool bRefresh = true);
protected:
	void OnInitial();
	void OnDestroy();
	void OnPaint(PAINT_STATUS status);

protected:
	DECLARE_MSG_MAP()
	unsigned long OnClickChnn( unsigned long ctrlID );
	unsigned long OnClickGroup( unsigned long ctrlID );
    unsigned long OnClickCombo();
protected:
	unsigned long DoClickChnn( unsigned long index, bool bRefresh );


	//////////////////////////////////////////////////////////////////////////
	GUI::CCheckCtrl				*m_pCKCH, *m_pCKGroup;
	GUI::CCalendarCtrl			m_calendar;
	unsigned long				m_channelNum;	
	ULONGLONG					m_authCH;
};
#endif //_SEARCH_CONDITION_DLG_H_
