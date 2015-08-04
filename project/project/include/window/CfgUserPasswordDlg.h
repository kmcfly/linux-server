/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : L
** Date         : 2009-08-28
** Name         : CfgUserPasswordDlg.h
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/

#ifndef _CFG_USER_PASSWORD_H_
#define _CFG_USER_PASSWORD_H_

#include "Dialog.h"
#include "Static.h"
#include "EditCtrl.h"
#include "NetConfigDefine.h"

using namespace GUI;

class CCfgUserPasswordDlg : public GUI::CDialog
{
public:
	CCfgUserPasswordDlg();
	~CCfgUserPasswordDlg();

public:
	enum CFG_USER_PASSWORD_TYPE
	{
		CFG_USER_PASSWORD_SET = 0,
		CFG_USER_PASSWORD_MODIFY = 1,
	};

public:
	void SetPasswordType( CFG_USER_PASSWORD_TYPE type = CFG_USER_PASSWORD_SET );
	void SetMaxPassword( int maxPassword );

	//void SetInfoUser( NCFG_INFO_USER& infoUser );
	unsigned long CheckValue();
	//bool GetInfoUser( NCFG_INFO_USER& infoUser );
protected:
	//int GetValue();

protected:
	void OnInitial();
	unsigned long OnApply();

protected:
	//GUI::CStatic		m_stcUserName;
	GUI::CStatic		m_scOldPassword;
	GUI::CStatic		m_scNewPassword;
	GUI::CStatic		m_scConfirmPassword;

	GUI::CEditCtrl		m_editOldPassword;
	GUI::CEditCtrl		m_editNewPassword;
	GUI::CEditCtrl		m_editConfirmPassword;

protected:
	CFG_USER_PASSWORD_TYPE	m_cfgPasswordType;
	int						m_maxPassword;	//密码框最大长度
	//NCFG_INFO_USER			m_infoUser;
};


#endif
