/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : ysw
** Date         : 2009-08-01
** Name         : TimeSearchDlg.h
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/
#ifndef _TIME_SEARCH_DLG_H_
#define _TIME_SEARCH_DLG_H_
#include "Button.h"
#include "StateButton.h"
#include "RadioCtrl.h"
#include "CheckCtrl.h"
#include "Dialog.h"
#include "Static.h"
#include "DateCtrl.h"
#include "TimeCtrl.h"
#include "GroupCtrl.h"
#include "SelectChnnDlg.h"
#include "ScheduleListCtrl.h"
#include "MessageMan.h"
#include "DrawManager.h"

using namespace GUI;

class CTimeSearchDlg : public GUI::CDialog
{
public:
	CTimeSearchDlg();
	~CTimeSearchDlg();

	void SetInitialInfo(unsigned long channelNum, unsigned long videoOutNum, unsigned long diskOwnerIndex, ULONGLONG authCH);

	void OnSearch(unsigned long startTime, unsigned long endTime, ULONGLONG  chnnBits, unsigned long dataOwnerby, bool bRefresh);
    unsigned long GetPlaybackInfo(unsigned long &playbackStarTime, unsigned long &playbackEndTime, unsigned long &playbackPlayTime, ULONGLONG &chnnBits);
protected:
	void OnInitial();
	void OnPostInitial();

	//////////////////////////////////////////////////////////////////////////
	enum TIME_SEARCH_SEL_TYPE
	{
		TIME_SEARCH_SEL_TYPE_1X1 = 0,
		TIME_SEARCH_SEL_TYPE_2X2,
		TIME_SEARCH_SEL_TYPE_2X3,
		TIME_SEARCH_SEL_TYPE_3X3,
		TIME_SEARCH_SEL_TYPE_4X4 ,
	};

	TIME_SEARCH_SEL_TYPE	m_selType;
	ULONGLONG				m_selChnn;

	unsigned long			m_playbackStartTime;
	unsigned long			m_playbackEndTime;
	unsigned long			m_playbackPlayTime;
	unsigned long			m_channelNum;
	unsigned long			m_videoOuputNum;
	unsigned long			m_diskOwnerIndex;

	ULONGLONG				m_authCH;
	ULONGLONG				m_scheduleCH;

	//////////////////////////////////////////////////////////////////////////
	DECLARE_MSG_MAP()

	//
	unsigned long OnBtnChnn1X1();
	unsigned long OnBtnChnn2X2();
	unsigned long OnBtnChnn2X3();
	unsigned long OnBtnChnn3X3();
	unsigned long OnBtnChnn4X4();
	unsigned long OnBtnPlayback();
	unsigned long OnClickSchedule(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);

private:
	void RefreshBtnSplit(TIME_SEARCH_SEL_TYPE type);
	/////////////////////////////////////////////////////////////////////////
	enum
	{
		CHNN_SELECT_NO,
		CHNN_SELECT_YES
	};

	GUI::CStateButton		m_btnChnn[5];

	GUI::CStatic			m_scStartTm;
	GUI::CDateCtrl			m_dateStart;
	GUI::CTimeCtrl			m_tmStart;
	GUI::CButton			m_btnPaly;

	CSelectChnnDlg			m_selectChnnDlg;

	GUI::CScheduleListCtrl	m_scheduleList;	
};
#endif //_TIME_SEARCH_DLG_H_
