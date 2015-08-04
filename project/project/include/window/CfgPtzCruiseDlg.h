/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : hz
** Date         : 2009-08-12
** Name         : CfgPtzCruiseDlg.h 
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/

#ifndef _CFG_PTZ_CRUISE_DLG_H_
#define _CFG_PTZ_CRUISE_DLG_H_

#include "Dialog.h"
#include "ListCtrlEx.h"
#include "CfgCruiseDlg.h"

using namespace GUI;

class CCfgPtzCruiseDlg : public GUI::CDialog
{

public:
	CCfgPtzCruiseDlg();
	~CCfgPtzCruiseDlg();

	void SetCruiseData(const std::vector<CRUISE_INFO>& cruiseList)
	{
		m_CruiseList = cruiseList;
	}
	void GetCruiseData(std::vector<CRUISE_INFO>& cruiseList) const
	{
		cruiseList = m_CruiseList;
	}


protected:
	DECLARE_MSG_MAP()
	void OnInitial();
	void OnDestroy();

	unsigned long OnClickBtnAdd();
	unsigned long OnClickBtnDel();
	unsigned long OnClickBtnModify();
	unsigned long OnClickBtnClearAll(); 
	unsigned long OnOK();

	void ShowCruise(bool bRefresh);
	void InitialCruiseID();
	unsigned long GetCruiseID();

	GUI::CListCtrlEx <CRUISE_INFO> m_listCtrlEx;

	GUI::CButton m_btnAdd;
	GUI::CButton m_btnModify;
	GUI::CButton m_btnDel;
	GUI::CButton m_btnClear;

	CRUISE_INFO_LIST m_CruiseList;
	BYTE * m_pCruiseID;
	enum { INVALID_CRUISE_ID = 255 };

protected:
};

#endif

