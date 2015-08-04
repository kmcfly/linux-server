/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : hz
** Date         : 2009-08-12
** Name         : CfgPtzPresetDlg.h 
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/

#ifndef _CFG_PTZ_PRESET_DLG_H_
#define _CFG_PTZ_PRESET_DLG_H_

#include "Dialog.h"
#include "ListCtrl.h"
#include "PTZViewDlg.h"
#include "PTZPresetCtrl.h"

using namespace GUI;

class CCfgPtzPresetDlg : public GUI::CDialog
{

public:
	CCfgPtzPresetDlg();
	~CCfgPtzPresetDlg();

	void SetPTZConfig(std::vector<PTZ_SERIAL_INFO> ptzConfig);
	void SetChnn(int chnn);
	void SetPresetList(const std::vector<PTZ_PRESET> & presetList);
	void GetPresetList(std::vector<PTZ_PRESET> & presetList) const;

protected:
	DECLARE_MSG_MAP()
	void OnInitial();
	void OnDestroy();


	unsigned long OnClickList(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);

	void ShowPresset(bool bRefresh);

	void InitialPresetID();
	unsigned long GetPresetID();

	int m_curChnn;

	GUI::CListCtrl	m_listCtrl;

	CPTZViewDlg m_ptzViewDlg;
	CPTZPresetCtrl m_ptzPresetDlg;

	std::vector<PTZ_PRESET>  m_presetList;
	BYTE * m_pPresetID;
	enum { INVALID_PRESET_ID = 255 };

	enum _COL_INDEX_
	{
		COL_PRESET_NUM,
		COL_PRESET_ENABLE,
		COL_PRESET_NAME,
		COL_PRESET_MODIFY
	};

protected:
};

#endif

