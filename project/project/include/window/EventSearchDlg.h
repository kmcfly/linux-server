/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : ysw
** Date         : 2009-08-01
** Name         : EventSearchDlg.h
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/
#ifndef _EVENT_SEARCH_DLG_H_
#define _EVENT_SEARCH_DLG_H_
#include "Button.h"
#include "CheckCtrl.h"
#include "Dialog.h"
#include "ListCtrlEx.h"
#include "MessageMan.h"

using namespace GUI;

class CEventSearchDlg : public GUI::CDialog
{
public:
	CEventSearchDlg();
	~CEventSearchDlg();
	void SetInitialInfo(unsigned long channelNum);
	unsigned long GetPlaybackInfo(FILE_INFO_EX & fileInfo);
	void OnSearch(unsigned long startTime, unsigned long endTime, ULONGLONG chnnBits, unsigned long dataOwnerby);
protected:
	void OnInitial();
	void EndDialog(DLG_END_VALUE result);

protected:
	DECLARE_MSG_MAP()
	unsigned long OnClickList(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);

	unsigned long OnCkMotion();
	unsigned long OnCkSensor();
	unsigned long OnCkAll();

private:
	//////////////////////////////////////////////////////////////////////////
	GUI::CCheckCtrl	m_ckMotion;
	GUI::CCheckCtrl	m_ckSensor;
	GUI::CCheckCtrl	m_ckAll;

	GUI::CListCtrlEx <RECLOG_INFO_EX> m_listCtrlEx;

	RECLOG_INFO_EX  m_selRelogInfo;

	unsigned long   m_channelNum;
};
#endif //_EVENT_SEARCH_DLG_H_
