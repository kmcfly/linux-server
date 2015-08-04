/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : ZXX
** Date         : 2013-12-26
** Name         : WzdNatPassWordDlg.h
** Version      : 1.0
** Description  : 向导修改密码页面
** Modify Record:
***********************************************************************/

#ifndef _WZD_NAT_PASSWORD_DLG_H_
#define _WZD_NAT_PASSWORD_DLG_H_
#include "WzdNatPageDlg.h"
#include "RichTextCtrl.h"
#include "ScrollCtrl.h"
#include "CheckCtrl.h"
#include "ListCtrl.h"
#include <string>
using namespace GUI;

//////////////////////////////////////////////////////////////////////////
// * 向导启用穿透和修改密码页面
//////////////////////////////////////////////////////////////////////////

class CWzdNatPassWordDlg : public CWzdNatPageDlg
{
public:
	CWzdNatPassWordDlg();
	~CWzdNatPassWordDlg();

	//设置向导信息
	void SetWzdInfo(NAT_WIZARD_INFO* pWzdInfo);
	void OnLeavePage();

	//显示向导数据
	void ShowWzdData(bool bRefresh);
	//页面完成读取向导信息
	void GetWzdData(bool bNext);

	bool CheckNewPasswd();
	
protected:
	void OnInitial();
	void InitCtrlInfo();
	void OnDestroy();
	void SetTipInfo();
	void OnPostInitial();//设置用户名和密码输入的最大长度
protected:
	DECLARE_MSG_MAP()
	unsigned long OnCheck();
	unsigned long OnScrollchange(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);
	unsigned long OnClickList(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);

protected:
	GUI::CRichTextCtrl	m_richTextCtrl;
	GUI::CListCtrl		m_listCtrl;
	GUI::CCheckCtrl		m_checkCtrl;
	GUI::CScrollCtrl	m_scroll_v;

	NAT_WIZARD_INFO     * m_pWzdInfo;
	unsigned short		m_toprow;//滚屏显示的第一行

	enum LIST_ROW
	{
		ROW_USER_NAME,
		ROW_ORIGIAN_PASSWORD,
		ROW_NEW_PASSWORD,
		ROW_CONFIRM_PASSWORD,
		ROW_COUNT,
	};
private:
	string			m_modifyName;//需要修改的用户名
	string			m_origianPass;//原先的密码
	string			m_newPass;//新的密码
	string			m_confirmPass;//确认的密码
	bool			m_bChangePassword;
	
};

#endif
