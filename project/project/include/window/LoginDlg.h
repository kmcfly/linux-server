/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : hz
** Date         : 2009-08-12
** Name         : LoginDlg.h 
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/

#ifndef _LOGIN_DLG_H_
#define _LOGIN_DLG_H_

#include "Dialog.h"
#include "Static.h"
#include "EditCtrl.h"
#include "MessageMan.h"
//#include "BackDoorDlg.h"
//#include "BackDoorLogoDlg.h"

using namespace GUI;

class CLoginDlg : public GUI::CDialog
{
public:
	CLoginDlg();
	~CLoginDlg();

	bool IsLogin() const { return m_bLogIn; }

 	bool GetLoginSuccessReply(NET_LOGIN_SUCCESS_REPLY& replyInfo) const;
	bool GetErrorCode(NET_CTRL_FAIL& errorCode) const;
	
	bool LogIn(const char *pName, const char *pPassword, bool bAuto = true);
	void Logoff();

protected:
	//DECLARE_MSG_MAP()
	void OnInitial();
	unsigned long OnOK();

	bool GetMaxLen(unsigned long& nameLen, unsigned long& passLen);

	bool LoginProc();
	void SetErrorText();

	GUI::CStatic m_static[2];
	GUI::CEditCtrl m_editUser;
	GUI::CEditCtrl m_editPassword;
	GUI::CStatic m_stLoginInfo;

	bool m_bLogIn;
	NET_CTRL_FAIL			m_loginErrorCode;

	//用户名、密码长度
	unsigned long m_lenName;
	unsigned long m_lenPass;

	static bool s_bInitFirst;
};

#endif//_LOGIN_DLG_H_
