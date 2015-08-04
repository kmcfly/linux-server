/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : L
** Date         : 2009-08-28
** Name         : CfgUserPasswordDlg.cpp
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/

#include "CfgUserPasswordDlg.h"
#include "MessageMan.h"


CCfgUserPasswordDlg::CCfgUserPasswordDlg()
{
	m_cfgPasswordType = CFG_USER_PASSWORD_SET;
	m_maxPassword = 0;
}

CCfgUserPasswordDlg::~CCfgUserPasswordDlg()
{

}

void CCfgUserPasswordDlg::OnInitial()
{
	GUI::CDialog::OnInitial();

	char str[64];
	switch ( m_cfgPasswordType )
	{
	case CFG_USER_PASSWORD_SET:
		snprintf( str, 64, "%s", m_pStrTable->String(E_STR_ID_SET_PASSWORD) );
		break;
	case CFG_USER_PASSWORD_MODIFY:
		snprintf( str, 64, "%s", m_pStrTable->String(E_STR_ID_MODIFY_PASSWORD) );
		break;
	}

	SetImage(BMP_MEM_DLG_SMALL, false);
	SetCaption(str, false);

	SetDefaultBtn( DEFAULT_BTN_APPLY | DEFAULT_BTN_CANCEL );
	SetDefaultBtnCaption( DEFAULT_BTN_CANCEL, m_pStrTable->String( E_STR_ID_EXIT) );
	SetDefaultBtnCaption(DEFAULT_BTN_APPLY,m_pStrTable->String(E_STR_ID_OK));
	//////////////////////////////////////////////////////////////////////////

	unsigned short xLeft = m_length[DEFAULT_BTN_RIGHT_XPOS];
	unsigned short width = m_rect.Width() - 2 * xLeft;
	unsigned short row = 4;
	switch ( m_cfgPasswordType )
	{
	case CFG_USER_PASSWORD_SET:
		row = 4;
		break;
	case CFG_USER_PASSWORD_MODIFY:
		row = 6;
		break;
	}

	unsigned short xOff = 10, yOff = 2;
	unsigned short height = m_length[EDIT_HEIGHT] * (row/2) + m_length[STATIC_HEIGHT] * (row/2+1) + (yOff * row);
	unsigned short yTop = m_length[TITLE_HEIGHT] + xOff;//(m_rect.Height() - height - m_length[TITLE_HEIGHT] - m_length[DEFAULT_BTN_BOTTOM_YPOS]) * 3 / 4;

	unsigned short x = xLeft, y = yTop;

	GUI::CStatic*	pStatic[3] = { NULL };
	GUI::CEditCtrl* pEdit[3] = { NULL };

	switch ( m_cfgPasswordType )
	{
	case CFG_USER_PASSWORD_SET:
		{
			pStatic[0] = &m_scNewPassword;
			pStatic[1] = &m_scConfirmPassword;

			pEdit[0] = &m_editNewPassword;
			pEdit[1] = &m_editConfirmPassword;
		}
		break;
	case CFG_USER_PASSWORD_MODIFY:
		{
			pStatic[0] = &m_scOldPassword;
			pStatic[1] = &m_scNewPassword;
			pStatic[2] = &m_scConfirmPassword;

			pEdit[0] = &m_editOldPassword;
			pEdit[1] = &m_editNewPassword;
			pEdit[2] = &m_editConfirmPassword;

			//m_stcUserName.Create(GUI::CWnd::GetRandWndID(), x, y, width, m_length[STATIC_HEIGHT], this, CStringTable::Instance()->String(E_STR_ID_TIP_06_07));
			//m_stcUserName.SetTextColor(GUI_RGB(255, 0, 0));

			y += (m_length[STATIC_HEIGHT] + yOff + 5);
		}
		break;
	default:
		assert(false);
		return;
	}

	// create ui
	for ( int i = 0; i < 3; i++ )
	{
		x = xLeft;
		if( pStatic[i] ) pStatic[i]->Create( GUI::CWnd::GetRandWndID(), x, y, width, m_length[STATIC_HEIGHT], this );

		y += (m_length[STATIC_HEIGHT] + yOff);
		if( pEdit[i] ) pEdit[i]->Create( GUI::CWnd::GetRandWndID(), x, y, width, m_length[EDIT_HEIGHT], this, 0, i + 1 );

		y += (m_length[EDIT_HEIGHT] + yOff);
	}

	y += yOff;
	CreateTipWnd(width,x,y);
	SetTipTextColor(GUI_RGB(220,20,20),TIP_ERR);

	m_scOldPassword.SetCaption( m_pStrTable->String(E_STR_ID_OLD_PASSWORD), false );
	m_scNewPassword.SetCaption( m_pStrTable->String( E_STR_ID_NEW_PASSWORD ), false );
	m_scConfirmPassword.SetCaption( m_pStrTable->String( E_STR_ID_COMFIRM_PASSWORD ), false );

	m_editOldPassword.SetMaxStringLen( m_maxPassword );
	m_editOldPassword.SetPassword( true );
	m_editNewPassword.SetMaxStringLen( m_maxPassword );
	m_editNewPassword.SetPassword( true );
	m_editConfirmPassword.SetMaxStringLen( m_maxPassword );
	m_editConfirmPassword.SetPassword( true );
}

unsigned long CCfgUserPasswordDlg::OnApply()
{
	unsigned long ret = CheckValue();

	if( 0 == ret )
	{
		unsigned long len = sizeof(unsigned long) + sizeof(NCFG_INFO_SHORT_NAME) * 2;
		unsigned char* pData = new unsigned char[len];
		memset(pData, 0, len);

		*reinterpret_cast<unsigned long*>(pData) = LOCAL_CLIENT_ID; //本地
		NCFG_INFO_SHORT_NAME* pUserPass = reinterpret_cast<NCFG_INFO_SHORT_NAME*>(pData + sizeof(unsigned long));

		//
		assert(sizeof(pUserPass[0].name)>m_maxPassword);
		strncpy(pUserPass[0].name, m_editOldPassword.GetCaption().c_str(), m_maxPassword);
		strncpy(pUserPass[1].name, m_editNewPassword.GetCaption().c_str(), m_maxPassword);

		MESSAGE_DATA msgData;
		memset(&msgData, 0, sizeof(MESSAGE_DATA));
		CMessageMan::Instance()->SendMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_CFG_MODIFY_PASS, pData, len, msgData);	
	
		assert(NULL != msgData.pData);
		PACKCMD *pCmd = reinterpret_cast<PACKCMD *>(msgData.pData);
		if (CMD_REPLY_CFG_MODIFY_PASS_SUCC == static_cast<NET_PROTOCOL>(pCmd->cmdType))
		{
			ret = 0;
		}
		else
		{
			ret = E_STR_ID_TIP_00_07;
			NET_CTRL_FAIL netret = *reinterpret_cast<NET_CTRL_FAIL*>(msgData.pData + sizeof(PACKCMD));
			switch (netret)
			{
			case NETERR_MODIFY_PASS_ERR:
				ret = E_STR_ID_TIP_06_02; //正常情况下,出错只走本分支
				break;
			case NETERR_MODIFY_PASS_UFUSER:
				//[2010-03-03 L] 当本地用此用户登录,后来IE端将此用户删除后.再修改本用户密码会走本分支
				//assert(false);
				ret = E_STR_ID_TIP_06_04;
				break;
			case NETERR_MODIFY_PASS_ERR_SAVE:
				assert(false);
				ret = E_STR_ID_TIP_06_05;
				break;
			default:
				assert(false);
				break;
			}
		}

		CMessageMan::Instance()->ReleaseMsgBuff(msgData);
		delete [] pData;
	}
	
	if( 0 != ret )
	{
		MessageOut(static_cast<ENUM_STRING_ID>(ret), TIP_ERR, true, DEFAULT_BTN_OK | DEFAULT_ICON_ERROR);
	}
	else
	{
		return OnOK();
	}

	return KEY_VIRTUAL_MSG_STOP;
}


void CCfgUserPasswordDlg::SetPasswordType( CFG_USER_PASSWORD_TYPE type /*= CFG_USER_PASSWORD_SET*/ )
{
	m_cfgPasswordType = type;
}

void CCfgUserPasswordDlg::SetMaxPassword( int maxPassword )
{
	m_maxPassword = maxPassword;
}

//void CCfgUserPasswordDlg::SetInfoUser( NCFG_INFO_USER& infoUser )
//{
//	memcpy( &m_infoUser, &infoUser, sizeof(NCFG_INFO_USER) );
//}

unsigned long CCfgUserPasswordDlg::CheckValue()
{
	if( 0 != m_editNewPassword.GetCaption().compare( m_editConfirmPassword.GetCaption() ) )
	{
		return E_STR_ID_TIP_06_03;
	}

	//std::string strUser(m_infoUser.password);
	//if( 0 != strUser.compare( m_editOldPassword.GetCaption() ) )
	//{
	//	return CCfgUserDlg::CFG_USER_ERR_PASS;
	//}

	return 0;
}

//bool CCfgUserPasswordDlg::GetInfoUser( NCFG_INFO_USER& infoUser )
//{
//	memcpy( &infoUser, &m_infoUser, sizeof(NCFG_INFO_USER) );
//	return true;
//}

//int CCfgUserPasswordDlg::GetValue()
//{
//	strcpy( m_infoUser.password, m_editNewPassword.GetCaption().c_str() );
//	return true;
//}
