/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : hz
** Date         : 2009-08-01
** Name         : MotionArePanelDlg.h 
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/
#ifndef _MOTION_AREA_PANEL_DLG_H_
#define _MOTION_AREA_PANEL_DLG_H_

#include "Dialog.h"
#include "ComboCtrl.h"
#include "SliderCtrl.h"
#include "Static.h"

using namespace GUI;

class CMotionArePanelDlg : public GUI::CDialog
{
public:
	CMotionArePanelDlg();
	~CMotionArePanelDlg();

	enum
	{
		WIDTH = 400,
		HEIGHT = 58,
	};

	enum CLICK_COMMAND
	{
		CLICK_ALL,
		CLICK_CLEAR,
		CLICK_TEST,
		CLICK_SAVE,
		CLICK_EXIT,
		CLICK_SENSI,

		CLICK_INVALID = -1,
	};
	void SetInfoCaption(const std::string& str, bool bRefresh = true);
	void EnableTest(bool bEnable);

	CLICK_COMMAND GetClickCommand();
	void SetSenstiveValue(unsigned long sensitivity){ m_sensitivity = sensitivity;}
	unsigned long GetSensitiveValue(){ return m_sensitivity;}
protected:
	DECLARE_MSG_MAP()
	void OnInitial();
	void OnDestroy();
	void OnChangeFocus(unsigned long oldID, unsigned long newID);

	unsigned long OnBtnAll();
	unsigned long OnBtnTest();
	unsigned long OnBtnSave();
	unsigned long OnBtnClear();
	unsigned long OnBtnExit();

	unsigned long OnSensitiveChange(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);

private:
	void SetSensitiveInfo(int value, bool bRefresh);

	GUI::CButton m_btnAll;
	GUI::CButton m_btnTest;
	GUI::CButton m_btnClear;
	GUI::CButton m_btnSave;
	GUI::CButton m_btnExit;

	GUI::CStatic m_stInfo;

	GUI::CSliderCtrl m_sliderSensitive;
	GUI::CStatic m_stSensitive;
	GUI::CStatic m_stSensitiveNum;

	CLICK_COMMAND m_clickCommand;
	unsigned long m_sensitivity;

	bool		  m_bEnableTest;
};

#endif//_MOTION_AREA_PANEL_DLG_H_

