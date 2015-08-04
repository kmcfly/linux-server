/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : L
** Date         : 2009-08-17
** Name         : PTZPresetCtrl.h 
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/

#ifndef _PTZ_PRESET_CTRL_H_
#define _PTZ_PRESET_CTRL_H_

#include "Dialog.h"
#include "ComboCtrl.h"
#include "Static.h"

using namespace GUI;

class CPTZPresetCtrl : public GUI::CDialog
{
public:
	CPTZPresetCtrl();
	~CPTZPresetCtrl();
	void SetPTZConfig(std::vector<PTZ_SERIAL_INFO> ptzConfig){m_ptzConfig = ptzConfig;}
	unsigned long ResponseHideCommond( unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);
	void SetPresetInfo(int preset_index, const std::vector<PTZ_PRESET>& presetList);
	//bool GetPresetInfo(std::vector<PTZ_PRESET>& presetList) const;

	enum
	{
		WIDTH = 300,
		HEIGHT = 100,
	};

	void ChangeBtnCaption(bool bChnnChanged);
protected:
	DECLARE_MSG_MAP()
	void OnInitial();
	void OnPaint(PAINT_STATUS status);
	//void OnDestroy();
	
	unsigned long OnSave();
	unsigned long OnClickBrush();
	unsigned long OnClickLamp();
	unsigned long OnClickComboPreset();
	void GoToPreset();

	void RefalseBtnCaption( bool bRefresh = true);
	GUI::CStatic	m_scNumber;
	GUI::CComboCtrl	m_comboIndex;
	//GUI::CStatic	m_scName;
	//GUI::CEditCtrl	m_editName;
	GUI::CButton	m_btnSave;

	unsigned long	m_protocl;
	bool m_bSave;
	int m_curPreset;

	GUI::CButton	m_btnBrush;
	GUI::CButton	m_btnLamp;
	std::vector<PTZ_SERIAL_INFO> m_ptzConfig;
	std::vector<PTZ_PRESET> m_presetList;
};


#endif
