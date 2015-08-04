/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : hz
** Date         : 2009-08-12
** Name         : CruiseEditDlg.h 
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/

#ifndef _CFG_SCHEDULE_TIME_DLG_H_
#define _CFG_SCHEDULE_TIME_DLG_H_

#include "Dialog.h"
#include "Static.h"
#include "ComboCtrl.h"
#include "EditCtrl.h"

using namespace GUI;

class CCruiseEditDlg : public GUI::CDialog
{
public:
	CCruiseEditDlg();
	~CCruiseEditDlg();

	void SetCurisePoint(const PTZ_CURISE_POINT point) { m_curisePoint = point; }
	void GetCurisePoint(PTZ_CURISE_POINT& point) { point = m_curisePoint; }

	static void SetPresetList(const std::vector<PTZ_PRESET>& presetList);

protected:
	DECLARE_MSG_MAP()
	void OnInitial();
	void OnOutOfDlg(unsigned long keyID);
	unsigned long OnOK();

	unsigned long OnClickPreset();

protected:
	GUI::CStatic m_stPreset;
	GUI::CComboCtrl m_comboPreset;
	GUI::CEditCtrl m_editPresetName;
	GUI::CStatic m_stSpeed;
	GUI::CComboCtrl m_comboSpeed;
	GUI::CStatic m_stTime;
	GUI::CComboCtrl m_comboTime;

	PTZ_CURISE_POINT m_curisePoint;

	static std::vector<PTZ_PRESET> m_presetList;
};

#endif
