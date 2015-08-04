/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : L
** Date         : 2009-08-15
** Name         : CfgUserGeneralDlg.h
** Version      : 1.0
** Description  : 
** Modify Record: 2009-08-29
1:
***********************************************************************/

#ifndef _CFG_USER_GENERAL_DLG_H_
#define _CFG_USER_GENERAL_DLG_H_

#include "NetConfigDefine.h"
#include "Dialog.h"
#include "ListCtrl.h"

using namespace GUI;

class CCfgUserGeneralDlg : public GUI::CDialog
{
public:
	CCfgUserGeneralDlg();
	~CCfgUserGeneralDlg();
	
public:
	enum CFG_USER_OPERATE_TYPE{
		CFG_USER_OPERATE_ADD = 0,
		CFG_USER_OPERATE_MODIFY = 1,
	};

public:
	void SetOperateType( CFG_USER_OPERATE_TYPE type = CFG_USER_OPERATE_ADD );
	void SetMaxUserName( int maxUserName );
	void SetMaxPassword( int maxPassword );

	void ShowInfoUserGeneral( const NCFG_INFO_USER& infoUser, bool bRefresh );
	unsigned long CheckValue();
	bool GetInfoUserGeneral( NCFG_INFO_USER& infoUser );
protected:
	void OnInitial();

	//str 17位 分割符"-"，" "  unsigned char MAC[6]
	bool GetMac( const std::string& str, unsigned char pMac[] ) const;
	unsigned long GetBindingMac(const unsigned char pMac[]) const;
	bool GetValue(NCFG_INFO_USER& infoUser );

protected:
	DECLARE_MSG_MAP()
	unsigned long OnClickList(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);

protected:
	GUI::CListCtrl			m_cfgUserGener;

protected:
	CFG_USER_OPERATE_TYPE	m_cfgUserDlgType;


	enum LIST_ROW
	{
		ROW_NAME_ADD = 0,
		ROW_PASS_ADD = 1,
		ROW_CONFIRM_PASS_ADD = 2,
		ROW_TYPE_ADD = 3,
		ROW_MAC_ENABLE_ADD = 4,
		ROW_MAC_ADD = 5,

		ROW_NAME_MODIFY = 0,
		ROW_TYPE_MODIFY = 1,
		ROW_MAC_ENABLE_MODIFY = 2,
		ROW_MAC_MODIFY = 3,
	};

protected:
	int						m_maxUserName;	//用户名最大长度
	int						m_maxPassword;	//密码框最大长度
};

#endif
