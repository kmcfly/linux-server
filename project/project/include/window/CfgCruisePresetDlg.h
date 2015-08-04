/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : hz
** Date         : 2009-08-01
** Name         : CfgCruisePresetDlg.h 
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/
#ifndef _CFG_CRUISE_PRESET_DLG_H_
#define _CFG_CRUISE_PRESET_DLG_H_

#include "Dialog.h"
#include "ListCtrlEx.h"
#include "CruiseEditDlg.h"
//#include "PictureCtrl.h"
#include "Static.h"
#include "EditCtrl.h"
#include "ComboCtrl.h"

#include "CfgCruiseDlg.h"
#include "MessageMan.h"


extern NET_LOGIN_SUCCESS_REPLY *g_p_login_succ;

using namespace GUI;

class CCfgCruisePresetDlg : public GUI::CDialog
{
public:
	CCfgCruisePresetDlg();
	~CCfgCruisePresetDlg();

	void SetCruiseData(const std::vector<CRUISE_INFO>& cruiseList, int curCruise )
	{
		m_curCurise = curCruise;
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
	void EndDialog(DLG_END_VALUE result);

	void SetTip();

	unsigned long OnOK();
	unsigned long OnApply();
	unsigned long OnClickBtnAdd();
	unsigned long OnClickBtnDel();
	unsigned long OnClickBtnModify();
	unsigned long OnClickBtnTop();
	unsigned long OnClickBtnBottom();
	unsigned long OnClickBtnUp();
	unsigned long OnClickBtnDown();
	unsigned long OnChangeCruise();

private:
	DLG_END_VALUE PopEditDlg(CCruiseEditDlg& editDlg, GUI::CButton& btn);
	
	bool SendCruisePreviewCmd( const CRUISE_INFO &cruiseInfo );
	bool StartCruisePreview( const CRUISE_INFO &cruiseInfo );
	bool StopCruisePreview( const CRUISE_INFO &cruiseInfo );
	void PopupCruisePreviewDlg();

	void ShowPresetList(int index, bool bRefresh);


	//GUI::CListCtrl m_listPreset;
	
	GUI::CButton m_btnPresetAdd;
	GUI::CButton m_btnPresetDel;
	GUI::CButton m_btnPresetModify;

	GUI::CButton m_btnPresetTop;
	GUI::CButton m_btnPresetUp;
	GUI::CButton m_btnPresetDown;
	GUI::CButton m_btnPresetBottom;

	GUI::CStatic m_stCruiseName;
	GUI::CEditCtrl m_editName;
	GUI::CStatic m_stIndex;
	GUI::CComboCtrl m_comboIndex;

	GUI::CListCtrlEx <PTZ_CURISE_POINT> m_listCtrlEx;

	std::vector<CRUISE_INFO> m_CruiseList;
	int m_curCurise;

};

#endif//_CFG_CRUISE_DLG_H_

