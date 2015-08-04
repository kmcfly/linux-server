/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : L
** Date         : 2009-08-15
** Name         : CfgUserDlg.h 
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/

#ifndef _CFG_USER_DLG_H_
#define _CFG_USER_DLG_H_

#include "NetConfigDefine.h"
#include "Dialog.h"
#include "ListCtrl.h"
#include "CfgDlg.h"
#include "CfgUserPasswordDlg.h"

using namespace GUI;

class CCfgUserDlg : public CCfgDlg
{
public:
	CCfgUserDlg();
	~CCfgUserDlg();

	enum CFG_USER_ERROR
	{
		CFG_USER_OK = 0,			//正确
		CFG_USER_ERR_USERNAME = E_STR_ID_TIP_06_01,	//用户名不能为空、或已经有此用户名已存在
		CFG_USER_ERR_PASS = E_STR_ID_TIP_06_02,		//用户输入的密码错误
		CFG_USER_ERR_PASS_EMPTY = E_STR_ID_TIP_06_02,//用户密码不能为空
		CFG_USER_ERR_CONFIRM_PASS = E_STR_ID_TIP_06_03,//用户输入的密码不一致
		CFG_USER_ERR_MAC = E_STR_ID_MESSAGE_44,		//用户输入的MAC地址有误
	};

public:
	static bool IsAdmin( const std::string& name );

	bool FindUser( const std::string& name, NCFG_INFO_USER& user, int& index);//用户名作为关键值
	bool FindUser( const std::string& name, int& index );
	int  GetUserCount();

protected:
	void OnInitial();
	void OnPostInitial();
	void OnDestroy();

	void GetItemList(CFGITEM_LIST& itemList) const;
	bool ShowData(NCFG_ITEM_ID cfgID, const BYTE* pData, int len, bool bRefresh);
	bool SetData(NCFG_ITEM_ID cfgID, BYTE*& pData, int& len)  const;

	NCFG_INFO_USER* GetCfgInfoUser();
	bool DoAddUser( const NCFG_INFO_USER& user );
	bool DoModifyUser( const NCFG_INFO_USER& user );
	bool DoDeleteUser( const NCFG_INFO_USER& user );
	bool DoDeleteUser( const std::string& name );

protected:
	DECLARE_MSG_MAP()
	unsigned long OnClickList(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);
	unsigned long OnBtnDelete();
	unsigned long OnBtnModify();
	unsigned long OnBtnAdd();
	unsigned long OnBtnModifyPassword();

protected:
	GUI::CListCtrl		m_cfgUserList;

	GUI::CButton		m_btnDelete;
	GUI::CButton		m_btnModify;
	GUI::CButton		m_btnAdd;
	GUI::CButton		m_btnModifyPassword;

	enum LIST_ROW
	{
		ROW_NAME,
		ROW_TYPE,
		ROW_MAC,
	};

protected:
	int					m_maxUser;		//最大用户
	int					m_numUser;		//当前用户个数
	int					m_maxUserName;	//用户名最大长度
	int					m_maxPassword;	//密码框最大长度

	NCFG_INFO_USER*		m_pCfgInfoUser;	//用户配置数据 sizeof(NCFG_INFO_USER) * m_maxUser
	std::string			m_currSelect;	//当前需要保持选中的行，用户名，唯一

	std::string			m_loginName;	//登录用户名

protected:
	//只有唯一的admin用户拥有删除操作，并且不能删除自身
	static std::string s_strAdmin;
};

#endif
