/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : zxx
** Date         : 2012-11-19
** Name         : ShanghaiOtherErrDialog.h
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/

#ifndef __SHANGHAI_OTHERERR_DIALOG_H__
#define __SHANGHAI_OTHERERR_DIALOG_H__

#include "Dialog.h"
#include "Button.h"
#include "CheckCtrl.h"
#include "Static.h"
#include "EditCtrl.h"
#include "ComboCtrl.h"
#include "ShanghaiCmd.h"
#include <string>
#include "CharCodeTable.h"
using namespace GUI;

class CShanghaiOtherErrDialog: public GUI::CDialog
{
public:
	CShanghaiOtherErrDialog();
	~CShanghaiOtherErrDialog();

	void GetAssignInfo();
protected:
	void OnInitial();
private:
	//建立故障类型ctrl
	void CreateCheckCtrl(int index,unsigned short &x,unsigned short &y,unsigned short &xFocus,unsigned short &yFocus,const char *pstr);
	string ChangeChinese(const char *pstr);
private:
	GUI::CStatic	m_static;
	GUI::CComboCtrl m_errCombo;

	GUI::CCheckCtrl m_FaultCheck[3];
	GUI::CStatic	m_Editstatic;
	GUI::CEditCtrl  m_editCtrl;

	int m_curSel;
	enum FAULT_TYPE
	{
		FAULT_VIDEOALARM = 0,
		FAULT_AROUNDALARM ,
		FAULT_NETALARM,
		FAULT_LOCALALARM,
		FAULT_INTRUSIONALARM,
		FAULT_BUILDINGINTERCOM,
		FAULT_ACCESSMANAGE,
		FAULT_ENTRANCECONTROL,
		FAULT_ELECTRONICPATROL,
		FAULT_OTHERS,

	};
	
	CCharCodeTable* m_codeTable; 
};
#endif
//end
