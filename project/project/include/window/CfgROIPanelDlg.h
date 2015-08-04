/*
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : xian shi wei
** Date         : 2014-10-20
** Name         : CfgROIPanelDlg.h 
** Version      : 1.0
** Description  : 
** Modify Record:
1:
*/
#ifndef __CFG_ROI_PANEL_DLG_H__
#define __CFG_ROI_PANEL_DLG_H__

#include "Dialog.h"
#include "ComboCtrl.h"
#include "SliderCtrl.h"
#include "Static.h"
#include "CheckCtrl.h"

using namespace GUI;

class CCfgROIPanelDlg : public GUI::CDialog
{
public:
	CCfgROIPanelDlg();
	~CCfgROIPanelDlg();

	enum{
		WIDTH = 390,
		HEIGHT = 30,
	};

	enum CLICK_COMMAND{
		CLICK_SENSI,
		CLICK_SAVE,
		CLICK_EXIT,

		CLICK_INVALID = -1,
	};
	void SetInfoCaption(const std::string& str, bool bRefresh = true);

	CLICK_COMMAND GetClickCommand();
	
	unsigned long GetSensitiveValue(){ 
		return m_sensitivity;
	}
	void SetSenstiveValue(unsigned long sensitivity);

	void SetEnableROI( unsigned long enable){ 
		m_iEnable=enable;
		if (enable>0)
		{
			m_chEnableROI.SetCheck(true,false);
		}else{
			m_chEnableROI.SetCheck(false,false);
		}
	}
	unsigned long GetEnableROI(){ 
		return m_iEnable;
	}
	virtual void OnPaint(PAINT_STATUS status,unsigned int enable,unsigned int sensitivety);
	
	void SetValue(unsigned int enable, unsigned int sensitivety)
	{
		m_iEnable = enable;
		m_sensitivity = sensitivety;
	}
	void SetParamter(unsigned int& enabel,unsigned int& sensitivety)
	{
		enabel = m_iEnable;
		sensitivety = m_sensitivity;
	}

DECLARE_MSG_MAP()
	void OnInitial();
	void OnDestroy();
	void OnChangeFocus(unsigned long oldID, unsigned long newID);

	unsigned long OnBtnSave();
	unsigned long OnBtnExit();
	unsigned long OnCheckEnable();

	unsigned long OnSensitiveChange(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);

private:
	void SetSensitiveInfo(int value, bool bRefresh);

	GUI::CButton m_btnSave;
	GUI::CButton m_btnExit;

	GUI::CStatic m_stInfo;

	GUI::CSliderCtrl m_sliderSensitive;
	GUI::CStatic m_stSensitive;
	GUI::CStatic m_stSensitiveNum;
	GUI::CStatic m_enableRoi;

	GUI::CCheckCtrl m_chEnableROI;

	CLICK_COMMAND m_clickCommand;
	unsigned long m_sensitivity;

	unsigned long m_iEnable;
};

#endif
