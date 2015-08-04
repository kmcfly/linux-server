/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : L
** Date         : 2009-08-19
** Name         : CfgAdvanceDlg.h
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/


#ifndef _CFG_ADVANCE_DLG_H_
#define _CFG_ADVANCE_DLG_H_

#include "Dialog.h"
#include "Anchor.h"
#include "PictureCtrl.h"
#include "CfgConfigManDlg.h"
#include "CfgBanListDlg.h"

#ifdef __NETSERVER_HXHT__
#include "CfgHxhtNetParamCfgDlg.h"
#endif

using namespace GUI;

class CCfgAdvanceDlg : public GUI::CDialog
{
public:
	CCfgAdvanceDlg();
	~CCfgAdvanceDlg();
protected:
	DECLARE_MSG_MAP()
	void OnInitial();
	unsigned long OnBanList();
	unsigned long OnReset();
	unsigned long OnConfigMan();
	unsigned long OnExit();
#ifdef __NETSERVER_HXHT__
	unsigned long OnCfgHxNetParam();
#endif

private:
	GUI::CPictureCtrl	m_picReset;
	GUI::CPictureCtrl	m_picConfigMan;
	GUI::CPictureCtrl	m_picExit;
	//²¹³äµÄºÚ°×Ãûµ¥Í¼Æ¬
	GUI::CPictureCtrl	m_picBlackList;

	GUI::CAnchor		m_anReset;
	GUI::CAnchor		m_anConfigMan;
	GUI::CAnchor		m_anExit;
	GUI::CAnchor		m_anblackList;

	CCfgConfigManDlg	m_configMan;
	CCfgBanListDlg		m_banlistDlg;

#ifdef __NETSERVER_HXHT__
	GUI::CPictureCtrl	m_picHxNetParam;
	GUI::CAnchor		m_anHxNetParam;
	CCfgHxhtNetParamCfgDlg m_cfgHxNetParamCfgDlg;
#endif

};


#endif
