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
		CFG_USER_OK = 0,			//��ȷ
		CFG_USER_ERR_USERNAME = E_STR_ID_TIP_06_01,	//�û�������Ϊ�ա����Ѿ��д��û����Ѵ���
		CFG_USER_ERR_PASS = E_STR_ID_TIP_06_02,		//�û�������������
		CFG_USER_ERR_PASS_EMPTY = E_STR_ID_TIP_06_02,//�û����벻��Ϊ��
		CFG_USER_ERR_CONFIRM_PASS = E_STR_ID_TIP_06_03,//�û���������벻һ��
		CFG_USER_ERR_MAC = E_STR_ID_MESSAGE_44,		//�û������MAC��ַ����
	};

public:
	static bool IsAdmin( const std::string& name );

	bool FindUser( const std::string& name, NCFG_INFO_USER& user, int& index);//�û�����Ϊ�ؼ�ֵ
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
	int					m_maxUser;		//����û�
	int					m_numUser;		//��ǰ�û�����
	int					m_maxUserName;	//�û�����󳤶�
	int					m_maxPassword;	//�������󳤶�

	NCFG_INFO_USER*		m_pCfgInfoUser;	//�û��������� sizeof(NCFG_INFO_USER) * m_maxUser
	std::string			m_currSelect;	//��ǰ��Ҫ����ѡ�е��У��û�����Ψһ

	std::string			m_loginName;	//��¼�û���

protected:
	//ֻ��Ψһ��admin�û�ӵ��ɾ�����������Ҳ���ɾ������
	static std::string s_strAdmin;
};

#endif
