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


#include "CfgUserOperateManDlg.h"
#include "CfgUserDlg.h"

extern NET_LOGIN_SUCCESS_REPLY *g_p_login_succ;

CCfgUserOperateManDlg::CCfgUserOperateManDlg()
{
	m_curPage = PAGE_GENERAL;
	m_cfgUserDlgType = CFG_USER_OPERATE_ADD;

	memset( &m_cfgUserDlgType, 0, sizeof(NCFG_INFO_USER) );
	m_maxPassword = 0;
	m_maxUserName = 0;
	m_pParentUserDlg = NULL;
	m_maxChnn = -1;
	m_effectCH = INVALID_INDEX(ULONGLONG);
}

CCfgUserOperateManDlg::~CCfgUserOperateManDlg()
{

}

void CCfgUserOperateManDlg::OnInitial()
{
	char str[64] = {0};

	assert( m_maxChnn > 0 );//设置最大通道数目

	switch ( m_cfgUserDlgType )
	{
	case CFG_USER_OPERATE_ADD:
		snprintf( str, 64, "%s", m_pStrTable->String(E_STR_ID_TITLE_ADD_USER));
		break;
	case CFG_USER_OPERATE_MODIFY:
		snprintf( str, 64, "%s", m_pStrTable->String(E_STR_ID_TITLE_MODIFY_USER) );
		break;
	}

	GUI::CDialog::OnInitial();
	SetImage(BMP_MEM_DLG_LARGE, false);
	SetCaption( str, false);
	SetDefaultBtn( DEFAULT_BTN_APPLY | DEFAULT_BTN_CANCEL);
	SetDefaultBtnCaption( DEFAULT_BTN_CANCEL, m_pStrTable->String(E_STR_ID_EXIT) );
	SetDefaultBtnCaption( DEFAULT_BTN_APPLY, m_pStrTable->String(E_STR_ID_OK) );
	//////////////////////////////////////////////////////////////////////////

	unsigned short x = m_length[DLG_OFFSET_LEFT];
	unsigned short y = m_length[TITLE_HEIGHT] + m_length[TABLE_HEIGHT] + m_length[DLG_OFFSET_TOP];
	unsigned short cx = m_rect.Width() - 2 * m_length[DLG_OFFSET_LEFT];
	unsigned short cy = m_rect.Height() - y- m_length[DLG_OFFSET_BOTTOM] - m_length[DEFAULT_BTN_BOTTOM_YPOS];

	switch( m_cfgUserDlgType )
	{
	case CFG_USER_OPERATE_ADD:
		m_cfgUserGeneralDlg.SetOperateType( CCfgUserGeneralDlg::CFG_USER_OPERATE_ADD );
		break;
	case CFG_USER_OPERATE_MODIFY:
		m_cfgUserGeneralDlg.SetOperateType( CCfgUserGeneralDlg::CFG_USER_OPERATE_MODIFY );
		break;
	}

	m_cfgUserGeneralDlg.SetMaxUserName( m_maxUserName );
	m_cfgUserGeneralDlg.SetMaxPassword( m_maxPassword );
	m_cfgUserGeneralDlg.Create(GUI::CWnd::GetRandWndID(), x, y, cx, cy, this, 0, 2);

#ifdef __DVR_BASIC__
	m_cfgUserAuthorityDlg.SetMaxChnn(m_maxChnn, m_effectCH);
	m_cfgUserAuthorityDlg.Create(GUI::CWnd::GetRandWndID(), x, y, cx, cy, this, 0, 3);
#endif

	m_tabCtrl.Create(GUI::CWnd::GetRandWndID(), x, m_length[TITLE_HEIGHT]+m_length[DLG_OFFSET_TOP], cx, m_length[TABLE_HEIGHT], this, 0, 1);
	m_tabCtrl.AddItem(&m_cfgUserGeneralDlg, m_pStrTable->String(E_STR_ID_GENERAL));
#ifdef __DVR_BASIC__
	m_tabCtrl.AddItem(&m_cfgUserAuthorityDlg, m_pStrTable->String( E_STR_ID_AUTHORITY ) );
#endif

	if( m_cfgUserDlgType == CFG_USER_OPERATE_MODIFY )
	{
		m_cfgUserGeneralDlg.ShowInfoUserGeneral( m_pCfgInfoUser, false );
#ifdef __DVR_BASIC__
		m_cfgUserAuthorityDlg.ShowInfoUserAuth( m_pCfgInfoUser, false );
#endif
	}
	else
	{
		memset(&m_pCfgInfoUser, 0, sizeof(NCFG_INFO_USER));
		ShowDefaultAuth( USER_GROUP_NORMAL, false );
	}

	m_curPage = PAGE_GENERAL;
	m_tabCtrl.SetCurItem( m_curPage , false );

	// create tip
	CreateTipWnd(cx-m_length[DEFAULT_BTN_OFFSET]-2*m_length[BTN_WIDTH]);
	SetTipTextColor(GUI_RGB(220,20,20),TIP_ERR);
}

void CCfgUserOperateManDlg::OnDestroy()
{
	CDialog::OnDestroy();
}

unsigned long CCfgUserOperateManDlg::OnApply()
{
	unsigned long ret = m_cfgUserGeneralDlg.CheckValue();

	if( CCfgUserDlg::CFG_USER_OK == ret &&
		CFG_USER_OPERATE_ADD == m_cfgUserDlgType )
	{
		NCFG_INFO_USER tmpUser;
		memcpy( &tmpUser, &m_pCfgInfoUser, sizeof(NCFG_INFO_USER) );
		m_cfgUserGeneralDlg.GetInfoUserGeneral( tmpUser );

		int index = 0;
		assert( m_pParentUserDlg );
		if( m_pParentUserDlg && m_pParentUserDlg->FindUser( tmpUser.name, index ) )
		{
			ret = CCfgUserDlg::CFG_USER_ERR_USERNAME;
		}
	}

	if( ret != CCfgUserDlg::CFG_USER_OK )
	{
		MessageOut(static_cast<ENUM_STRING_ID>(ret),TIP_ERR,true,DEFAULT_BTN_OK|DEFAULT_ICON_ERROR);
		
		m_curPage = PAGE_GENERAL;
		m_tabCtrl.SetCurItem( m_curPage , true );

		return KEY_VIRTUAL_MSG_STOP;
	}
	else
	{
		GetValue();
		return OnOK();
	}
}

bool CCfgUserOperateManDlg::GetValue()
{
	NCFG_INFO_USER tmpUser;
	memcpy( &tmpUser, &m_pCfgInfoUser, sizeof(NCFG_INFO_USER) );
	m_cfgUserGeneralDlg.GetInfoUserGeneral( tmpUser );

#ifdef __DVR_BASIC__
	m_cfgUserAuthorityDlg.GetInfoUserAuth( tmpUser );
#else
	ShowDefaultAuthEx(tmpUser.group, tmpUser);
#endif

	memcpy( &m_pCfgInfoUser, &tmpUser, sizeof(NCFG_INFO_USER) );
	return true;
}

void CCfgUserOperateManDlg::SetOperateType( CFG_USER_OPERATE_TYPE type /*= CFG_USER_OPERATE_ADD*/ )
{
	m_cfgUserDlgType = type;
}

void CCfgUserOperateManDlg::SetMaxUserName( int maxUserName )
{
	m_maxUserName = maxUserName;
}

void CCfgUserOperateManDlg::SetMaxPassword( int maxPassword )
{
	m_maxPassword = maxPassword;
}

void CCfgUserOperateManDlg::SetParentUserDlg( CCfgUserDlg* parent )
{
	assert( parent );
	m_pParentUserDlg = parent;
}

void CCfgUserOperateManDlg::SetInfoUser(NCFG_INFO_USER& infoUser )
{
	memcpy( &m_pCfgInfoUser, &infoUser, sizeof(NCFG_INFO_USER) );
}

void CCfgUserOperateManDlg::GetInfoUser( NCFG_INFO_USER& infoUser )
{
	memcpy( &infoUser, &m_pCfgInfoUser, sizeof(NCFG_INFO_USER) );
}

void CCfgUserOperateManDlg::ShowDefaultAuth( unsigned long group , bool bRefresh )
{
	NCFG_INFO_USER user;
	memset( &user, 0, sizeof(NCFG_INFO_USER) );
	ShowDefaultAuthEx(group, user);

#ifdef __DVR_BASIC__
	m_cfgUserAuthorityDlg.ShowInfoUserAuth( user, bRefresh );
#endif
}

void CCfgUserOperateManDlg::ShowDefaultAuthEx(unsigned long group , NCFG_INFO_USER &user)
{
	switch ( static_cast<USER_GROUP>(group) )
	{
	case USER_GROUP_NORMAL:
		{
#if defined(__TW01_RILI__)
			user.authority = (AUTH_BASIC | AUTH_LIVE | AUTH_RECORD | AUTH_PLAYBACK | AUTH_BACKUP);
#else
			user.authority = (AUTH_BASIC | AUTH_LIVE | AUTH_RECORD | AUTH_PLAYBACK | AUTH_BACKUP | AUTH_PTZ_CTRL | AUTH_REMOTE_VIEW | AUTH_AUDIO_TALK | AUTH_REMOTE_LOGON);
#endif

			user.authLiveCH			= ALL_CHNN_MASK(m_maxChnn);
			user.authRecordCH		= ALL_CHNN_MASK(m_maxChnn);
			user.authPlaybackCH		= ALL_CHNN_MASK(m_maxChnn);
			user.authBackupCH		= ALL_CHNN_MASK(m_maxChnn);
			user.authPTZCtrlCH		= ALL_CHNN_MASK(m_maxChnn/* - g_p_login_succ->productInfo.netVideoInputNum*/);
#if defined(__TW01_RILI__)
			user.authRemoteViewCH	= ~(ALL_CHNN_MASK(m_maxChnn));
#else
			user.authRemoteViewCH	= ALL_CHNN_MASK(m_maxChnn);
#endif
		}

		break;
	case USER_GROUP_ADVANCE:
		{
#if defined(__TW01_RILI__)
			user.authority = (AUTH_BASIC | AUTH_LIVE | AUTH_RECORD | AUTH_PLAYBACK | AUTH_BACKUP | AUTH_SYS_SETUP | AUTH_DATA_MAN | AUTH_DISK_MAN);
#else
			user.authority = (AUTH_BASIC | AUTH_LIVE | AUTH_RECORD | AUTH_PLAYBACK | AUTH_BACKUP | AUTH_PTZ_CTRL |\
				AUTH_REMOTE_VIEW | AUTH_LOG_VIEW | AUTH_SYS_SETUP | AUTH_AUDIO_TALK | AUTH_DATA_MAN | AUTH_DISK_MAN | AUTH_REMOTE_LOGON | AUTH_NET_DEVICE_MAN);
#endif
			user.authLiveCH			= ALL_CHNN_MASK(m_maxChnn);
			user.authRecordCH		= ALL_CHNN_MASK(m_maxChnn);
			user.authPlaybackCH		= ALL_CHNN_MASK(m_maxChnn);
			user.authBackupCH		= ALL_CHNN_MASK(m_maxChnn);
			user.authPTZCtrlCH		= ALL_CHNN_MASK(m_maxChnn);
#if defined(__TW01_RILI__)
			user.authRemoteViewCH	= ~(ALL_CHNN_MASK(m_maxChnn));
#else
			user.authRemoteViewCH	= ALL_CHNN_MASK(m_maxChnn);
#endif
		}
		break;
	default:
		assert( false );
		break;
	}
}

void CCfgUserOperateManDlg::SetMaxChnn(int max, ULONGLONG effectCH)
{
	m_maxChnn = max;
	m_effectCH = effectCH;
}

BEGIN_MSG_MAP(CCfgUserOperateManDlg,GUI::CDialog)
	ON_MSG(m_tabCtrl.GetID(), KEY_VIRTUAL_ENTER, OnClickTab)
END_MSG_MAP()

unsigned long CCfgUserOperateManDlg::OnClickTab()
{
	if (m_curPage != m_tabCtrl.GetCurItem())
	{
		m_curPage = static_cast<PAGE_ITEM>(m_tabCtrl.GetCurItem());
	}	

	return KEY_VIRTUAL_MSG_STOP;
}
