/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : YSW
** Date         : 2010-02-02
** Name         : DebugSetupDlg.h
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/
#ifndef _DEBUG_SETUP_DLG_H_
#define _DEBUG_SETUP_DLG_H_

#include "Dialog.h"
#include "Static.h"
#include "NumEditCtrl.h"
#include "CheckCtrl.h"

using namespace GUI;

class CDebugSetupDlg : public GUI::CDialog
{
public:
	CDebugSetupDlg();
	~CDebugSetupDlg();

	//
protected:
	void OnInitial();
private:
	//
	unsigned long OnApply();
	unsigned long OnOK();
	//
	GUI::CStatic	m_stcType;
	GUI::CStatic	m_stcTime;
	GUI::CStatic	m_stcNumber;

	//
	GUI::CCheckCtrl	m_ckLive;
	GUI::CCheckCtrl	m_ckRecord;
	GUI::CCheckCtrl	m_ckSearch;
	GUI::CCheckCtrl	m_ckPlay;
	GUI::CCheckCtrl	m_ckInformation;

	//
	GUI::CNumEditCtrl	m_neTimeLive;
	GUI::CNumEditCtrl	m_neTimeRecord;
	GUI::CNumEditCtrl	m_neTimeSearch;
	GUI::CNumEditCtrl	m_neTimePlay;
	GUI::CNumEditCtrl	m_neTimeInfo;

	//
	GUI::CNumEditCtrl	m_neNumLive;
	GUI::CNumEditCtrl	m_neNumRecord;
	GUI::CNumEditCtrl	m_neNumSearch;
	GUI::CNumEditCtrl	m_neNumPlay;
	GUI::CNumEditCtrl	m_neNumInfo;
};
#endif //_DEBUG_SETUP_DLG_H_
