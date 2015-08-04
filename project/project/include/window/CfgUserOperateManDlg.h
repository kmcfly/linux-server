/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : L
** Date         : 2009-08-29
** Name         : CfgUserOperateManDlg.h
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/

#ifndef _CFG_USER_OPERATE_MAN_DLG_
#define _CFG_USER_OPERATE_MAN_DLG_

#include "NetConfigDefine.h"
#include "Dialog.h"
#include "TabCtrl.h"

#include "CfgUserDlg.h"
#include "CfgUserGeneralDlg.h"

#ifdef __DVR_BASIC__
#include "CfgUserAuthorityDlg.h"
#endif

using namespace GUI;

class CCfgUserOperateManDlg : public GUI::CDialog
{
public:
	CCfgUserOperateManDlg();
	~CCfgUserOperateManDlg();

public:
	enum CFG_USER_OPERATE_TYPE
	{
		CFG_USER_OPERATE_ADD = 0,
		CFG_USER_OPERATE_MODIFY = 1,
	};

public:
	void SetOperateType( CFG_USER_OPERATE_TYPE type = CFG_USER_OPERATE_ADD );
	void SetMaxUserName( int maxUserName );
	void SetMaxPassword( int maxPassword );
	void SetParentUserDlg( CCfgUserDlg* parent );
	void SetInfoUser(NCFG_INFO_USER& infoUser );
	void GetInfoUser( NCFG_INFO_USER& infoUser );
	void ShowDefaultAuth( unsigned long group , bool bRefresh );

	void SetMaxChnn(int max, ULONGLONG effectCH = INVALID_INDEX(ULONGLONG));

protected:
	void OnInitial();
	void OnDestroy();
	
	unsigned long OnApply();

protected:
	bool GetValue();
    
	void ShowDefaultAuthEx(unsigned long group , NCFG_INFO_USER &user);

	DECLARE_MSG_MAP()
	unsigned long OnClickTab();

protected:
	GUI::CTabCtrl			m_tabCtrl;
#ifdef __DVR_BASIC__
	CCfgUserAuthorityDlg	m_cfgUserAuthorityDlg;
#endif
	CCfgUserGeneralDlg		m_cfgUserGeneralDlg;

	enum PAGE_ITEM
	{
		PAGE_GENERAL,
#ifdef __DVR_BASIC__
		PAGE_AUTHORITY,
#endif
	};
	PAGE_ITEM m_curPage;

protected:
	CFG_USER_OPERATE_TYPE	m_cfgUserDlgType;//操作的类型：添加、删除
	NCFG_INFO_USER			m_pCfgInfoUser; //用户输出信息
	int						m_maxUserName;	//用户名最大长度
	int						m_maxPassword;	//密码框最大长度
	int						m_maxChnn;			//通道数目最大值
	ULONGLONG				m_effectCH;		//有效通道
	CCfgUserDlg*			m_pParentUserDlg;//父窗口，需要检查数据
};



#endif
