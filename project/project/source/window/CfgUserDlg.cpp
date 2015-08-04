/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : L
** Date         : 2009-08-15
** Name         : CfgUserDlg.cpp
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/

#include "CfgUserDlg.h"
#include "CfgUserOperateManDlg.h"
#include "MessageMan.h"
#include "FuncCustom.h"

#include <cctype>
#include <algorithm>


//只有唯一的admin用户拥有删除操作，并且不能删除自身
std::string CCfgUserDlg::s_strAdmin = "admin";

bool CCfgUserDlg::IsAdmin( const std::string& name )
{
	if( 0 == strcmp( s_strAdmin.c_str(), name.c_str() ) )
	{
		return true;
	}
	else
	{
		return false;
	}
}


CCfgUserDlg::CCfgUserDlg() : 
m_maxUser(0), 
m_maxUserName(0), 
m_maxPassword(0),
m_numUser(0),
m_pCfgInfoUser(NULL)
{
}

CCfgUserDlg::~CCfgUserDlg()
{

}

void CCfgUserDlg::OnInitial()
{
	CCfgDlg::OnInitial();
	SetImage(BMP_MEM_DLG_LARGE, false);
	SetCaption(m_pStrTable->String(E_STR_ID_TITLE_USER_MAN), false);
	SetDefaultBtn( DEFAULT_BTN_CANCEL );
	SetDefaultBtnCaption( DEFAULT_BTN_CANCEL, m_pStrTable->String(E_STR_ID_EXIT) );
	//////////////////////////////////////////////////////////////////////////

	GUI::CListCtrl& list = m_cfgUserList;
	GUI::CWnd * pWnd = NULL;
	GUI::CCheckCtrl* pCheck = NULL;
	GUI::CComboCtrl* pCombo = NULL;

	unsigned short x = m_length[DLG_OFFSET_LEFT], y = m_length[TITLE_HEIGHT] + m_length[DLG_OFFSET_TOP];
	unsigned short xOff = m_length[DLG_OFFSET_X];
	unsigned short ctrlHeight = m_length[BTN_HEIGHT];
	unsigned short ctrlWidth = m_length[BTN_WIDTH];;

	unsigned short width = m_rect.Width() - 2 * m_length[DLG_OFFSET_LEFT];
	unsigned short height = m_rect.Height() - y - m_length[DEFAULT_BTN_BOTTOM_YPOS] - m_length[DLG_OFFSET_BOTTOM];

	unsigned long authority = CCfgDlg::s_loginSuccessReply.authority & 0x0000FFFF;
	bool bOperater = authority & AUTH_ADVANCE_MAN; //是否有删除等操作权限

	/////List
	list.Create(GUI::CWnd::GetRandWndID(), x, y, width, height, this, 0, 1, true);
	list.AddColumn(m_pStrTable->String( E_STR_ID_USER_NAME ), width*15/32);
	list.AddColumn(m_pStrTable->String( E_STR_ID_USER_TYPE ), width*7/32);
#if !defined(__TW01_RILI__)
	list.AddColumn(m_pStrTable->String( E_STR_ID_PC_MAC_ADDR ),width*10/32 - 20 );
#endif

	//调整list 位置.
	GUI::CRect rect = list.GetClientRect();
	unsigned short yOff = (m_rect.Height() - rect.Height() - m_length[DEFAULT_BTN_BOTTOM_YPOS] - y - m_length[DLG_OFFSET_BOTTOM]) / 2;
	y += yOff;
	list.MoveWnd( m_rect.m_left + x, m_rect.m_top + y );

	char str[128] = {0};

	list.EnableSingleSelect( true );
	list.SetCurSingleSelect( 0, false );
	list.CreateListFocusMap();

	/////Other
	GUI::CRect listRect =  list.GetClientRect();
	{
		GUI::CButton*	pBtn[3] = { &m_btnAdd, &m_btnModify, &m_btnDelete };

		unsigned short xx = x, yy = m_rect.Height() - m_length[DEFAULT_BTN_BOTTOM_YPOS];
		
		for( int ii = 0; ii < 3; ii++ )
		{
			//如果用户没有权限，这里直接跳过
			if( !bOperater ) break;

			pBtn[ii]->Create( GUI::CWnd::GetRandWndID(), xx, yy, ctrlWidth, ctrlHeight, this, ii , 2 );
			xx += ctrlWidth + xOff;
		}

		m_btnModifyPassword.Create( GUI::CWnd::GetRandWndID(), xx, yy, m_length[BTN_WIDTH] * 2, ctrlHeight, this, 3, 2 );
		
		pBtn[0]->SetCaption( m_pStrTable->String( E_STR_ID_ADD ), false );
		pBtn[1]->SetCaption( m_pStrTable->String( E_STR_ID_SETUP ), false );
		pBtn[2]->SetCaption( m_pStrTable->String( E_STR_ID_DELETE ), false );

		m_btnModifyPassword.SetCaption( m_pStrTable->String(E_STR_ID_MODIFY_PASSWORD), false );
	}

	/////
	m_maxUser = 0;
	m_maxUserName = 0;
	m_maxPassword = 0;
	m_numUser = 0;
	m_currSelect = "";

	m_loginName = "";
}

void CCfgUserDlg::OnPostInitial()
{
	CCfgDlg::OnPostInitial();

	//获取登录用户名
	MESSAGE_DATA msgData;
	memset(&msgData, 0, sizeof(MESSAGE_DATA));
	CMessageMan::Instance()->SendMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_LOGIN_USER_INFO, NULL, 0, msgData);

	PACKCMD* pCmd = reinterpret_cast<PACKCMD *>(msgData.pData);
	int len = msgData.dataLen - sizeof(PACKCMD);
	if(CMD_REPLY_LOGIN_USER_INFO == pCmd->cmdType && len > 0)
	{
		NET_CLIENT_INFO_EX* pNetClientInfo = reinterpret_cast<NET_CLIENT_INFO_EX*>(msgData.pData + sizeof(PACKCMD));
		m_loginName = pNetClientInfo->User;
	}
	else
	{
		m_loginName = "";
	}

	CMessageMan::Instance()->ReleaseMsgBuff(msgData);
}

void CCfgUserDlg::OnDestroy()
{
	CCfgDlg::OnDestroy();

	if( m_pCfgInfoUser )
	{
		delete [] m_pCfgInfoUser;
		m_pCfgInfoUser = NULL;
	}

	m_loginName = "";
}

void CCfgUserDlg::GetItemList(CFGITEM_LIST& itemList) const
{
	itemList.push_back( NCFG_ITEM_USER );

	///...read only
	itemList.push_back( NCFG_ITEM_USER_MAXNUM );
	itemList.push_back( NCFG_ITEM_DEVUSER_NAME_MAX_LEN );
	itemList.push_back( NCFG_ITEM_DEVUSER_PWD_MAX_LEN );
}

bool CCfgUserDlg::ShowData(NCFG_ITEM_ID cfgID, const BYTE* pData, int len, bool bRefresh)
{
	assert(NULL != pData);
	assert(len > 0);
	GUI::CListCtrl& list = m_cfgUserList;
	char str[128] = {0};

	BYTE* pTempData = NULL;
	unsigned long tempDataLen = 0;

	switch (cfgID)
	{
	case NCFG_ITEM_USER:
		{
			CCfgDlg::GetConfig( NCFG_ITEM_USER_MAXNUM, &pTempData, tempDataLen );
			assert( tempDataLen == sizeof(unsigned long) );
			m_maxUser = *reinterpret_cast<unsigned long*>(pTempData);

			CCfgDlg::GetConfig( NCFG_ITEM_DEVUSER_NAME_MAX_LEN, &pTempData, tempDataLen );
			assert( tempDataLen == sizeof(unsigned long) );
			m_maxUserName = *reinterpret_cast<unsigned long*>(pTempData);

			CCfgDlg::GetConfig( NCFG_ITEM_DEVUSER_PWD_MAX_LEN, &pTempData, tempDataLen );
			assert( tempDataLen == sizeof(unsigned long) );
			m_maxPassword = *reinterpret_cast<unsigned long*>(pTempData);

			const NCFG_INFO_USER* pInfoUser = reinterpret_cast<const NCFG_INFO_USER*>(pData);
			assert( len == sizeof(NCFG_INFO_USER) * m_maxUser );

			if( m_pCfgInfoUser )
			{
				delete [] m_pCfgInfoUser;
				m_pCfgInfoUser = NULL;
			}
			m_pCfgInfoUser = new NCFG_INFO_USER[m_maxUser];
			memcpy( m_pCfgInfoUser, pInfoUser, sizeof(NCFG_INFO_USER) * m_maxUser );

			list.RemoveAllItem( bRefresh );
			m_numUser = 0;
			int selNum = 0;
			for ( int ii=0; ii<m_maxUser; ii++ )
			{
				if( USER_GROUP_NONE == m_pCfgInfoUser[ii].group ) continue;

				strcpy( str, m_pCfgInfoUser[ii].name );
				if( 0 == m_currSelect.compare( str ) ) 
				{
					selNum = ii;
				}
				list.AddItem( m_numUser, 0, str, bRefresh );

				switch ( m_pCfgInfoUser[ii].group )
				{
				case USER_GROUP_ADMIN:
					strcpy( str, m_pStrTable->String(E_STR_ID_ADMIN) );
					break;
				case USER_GROUP_ADVANCE:
					strcpy( str, m_pStrTable->String(E_STR_ID_ADVANCED_USER) );
					break;
				case USER_GROUP_NORMAL:
					strcpy( str, m_pStrTable->String(E_STR_ID_NORMAL) );
					break;
				default:
					strcpy( str, "" );
					assert( false );
					break;
				}
				list.AddItem( m_numUser, 1, str, bRefresh );
#if !defined(__TW01_RILI__)
				sprintf(str,"%.2X-%.2X-%.2X-%.2X-%.2X-%.2X",m_pCfgInfoUser[ii].MAC[0], m_pCfgInfoUser[ii].MAC[1], m_pCfgInfoUser[ii].MAC[2],
												m_pCfgInfoUser[ii].MAC[3],m_pCfgInfoUser[ii].MAC[4],m_pCfgInfoUser[ii].MAC[5]);
				list.AddItem( m_numUser, 2, str, bRefresh );
#endif
				m_numUser += 1;
			}
			list.SetCurSingleSelect( selNum, bRefresh );

		}
		return true;
		break;
	default:
		break;
	}
	return false;
}

bool CCfgUserDlg::SetData(NCFG_ITEM_ID cfgID, BYTE*& pData, int& len)  const
{
	assert(NULL != pData);
	assert(len > 0);
	const GUI::CListCtrl& list = m_cfgUserList;

	switch (cfgID)
	{
	case NCFG_ITEM_USER:
		{
			NCFG_INFO_USER* pInfoUser = reinterpret_cast<NCFG_INFO_USER*>(pData);
			assert( len == sizeof(NCFG_INFO_USER) * m_maxUser );
			assert( m_pCfgInfoUser );
			if( m_pCfgInfoUser ) 
			{
				memcpy( pInfoUser, m_pCfgInfoUser, sizeof(NCFG_INFO_USER) * m_maxUser );
			}
		}
		return true;
		break;
	default:
		break;
	}
	return false;
}

NCFG_INFO_USER* CCfgUserDlg::GetCfgInfoUser()
{
	return m_pCfgInfoUser;
}


bool CCfgUserDlg::FindUser( const std::string& name, NCFG_INFO_USER& user, int& index)
{
	NCFG_INFO_USER* pInfoUser = CCfgUserDlg::GetCfgInfoUser();

	for ( int i = 0; i < m_maxUser; i++ )
	{
		if( USER_GROUP_NONE != pInfoUser[i].group )
		{
			if( 0 == strcmp( pInfoUser[i].name, name.c_str() ) )
			{
				memcpy( &user, &(pInfoUser[i]), sizeof(NCFG_INFO_USER) );
				index = i;
				return true;
			}
		}
	}

	return false;
}

bool CCfgUserDlg::FindUser( const std::string& name, int& index )
{
	NCFG_INFO_USER* pInfoUser = CCfgUserDlg::GetCfgInfoUser();

	for ( int i = 0; i < m_maxUser; i++ )
	{
		if( USER_GROUP_NONE != pInfoUser[i].group )
		{
			if( 0 == strcmp( pInfoUser[i].name, name.c_str() ) )
			{
				index = i;
				return true;
			}
		}
	}

	return false;
}

int CCfgUserDlg::GetUserCount()
{
	NCFG_INFO_USER* pInfoUser = CCfgUserDlg::GetCfgInfoUser();

	int count = 0;
	for ( int i = 0; i < m_maxUser; i++ )
	{
		if( USER_GROUP_NONE != pInfoUser[i].group )
		{
			count += 1;
		}
	}

	return count;
}


bool CCfgUserDlg::DoAddUser( const NCFG_INFO_USER& user )
{
	NCFG_INFO_USER* pInfoUser = CCfgUserDlg::GetCfgInfoUser();

	for ( int i = 0; i < m_maxUser; i++ )
	{
		if( USER_GROUP_NONE != pInfoUser[i].group )
		{
			continue;
		}
		else
		{
			memcpy( &(pInfoUser[i]), &user, sizeof(NCFG_INFO_USER) );
			SetModify(true);
			return true;
		}
	}
	return false;
}

bool CCfgUserDlg::DoModifyUser( const NCFG_INFO_USER& user )
{
	NCFG_INFO_USER* pInfoUser = CCfgUserDlg::GetCfgInfoUser();
	std::string strName( user.name );

	int index = 0;
	if( FindUser( strName, index ) )
	{
		memcpy( &(pInfoUser[index]), &user, sizeof(NCFG_INFO_USER) );
		SetModify(true);
		return true;
	}

	return false;
}

bool CCfgUserDlg::DoDeleteUser( const NCFG_INFO_USER& user )
{
	std::string name( user.name );
	return DoDeleteUser( name );
}

bool CCfgUserDlg::DoDeleteUser( const std::string& name )
{
	NCFG_INFO_USER* pInfoUser = CCfgUserDlg::GetCfgInfoUser();

	int index = 0;
	if( FindUser( name, index ) )
	{
		for ( int i=index; i< m_maxUser - 1; i++ )
		{
			//删除某用户后，依次移动用户序列
			memcpy( &(pInfoUser[i]), &(pInfoUser[i+1]), sizeof(NCFG_INFO_USER) );
		}
		memset( &(pInfoUser[m_maxUser-1]), 0, sizeof(NCFG_INFO_USER) );
		SetModify(true);
		return true;
	}

	return false;
}


BEGIN_MSG_MAP(CCfgUserDlg, GUI::CDialog)
	ON_MSG_EX( m_cfgUserList.GetID(), KEY_VIRTUAL_MSG_DEFAULT_TAG, OnClickList )
	ON_MSG( m_btnDelete.GetID(), KEY_VIRTUAL_ENTER, OnBtnDelete )
	ON_MSG( m_btnModify.GetID(), KEY_VIRTUAL_ENTER, OnBtnModify )
	ON_MSG( m_btnAdd.GetID(), KEY_VIRTUAL_ENTER, OnBtnAdd )
	ON_MSG( m_btnModifyPassword.GetID(), KEY_VIRTUAL_ENTER, OnBtnModifyPassword )
	ON_MSG( m_cfgUserList.GetID(), KEY_VIRTUAL_DBCLICK, OnBtnModifyPassword )
END_MSG_MAP()

unsigned long CCfgUserDlg::OnClickList(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	unsigned short row = (keyID >> 16)&0x7fff;	//行
	unsigned short col = keyID&0xffff;			//列

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CCfgUserDlg::OnBtnDelete()
{
	int row = m_cfgUserList.GetCurSingleSelect();

	if( row >= 0 )
	{
		std::string name = m_cfgUserList.GetItemText( row, 0 );

		//... 检查能否被删除
		if( IsAdmin(name)  )
		{
			GUI::CDialog::MessageBox( m_pStrTable->String(E_STR_ID_MESSAGE_39), m_pStrTable->String(E_STR_ID_TITLE_ERROR), DEFAULT_BTN_OK | DEFAULT_ICON_ERROR);
			return KEY_VIRTUAL_MSG_STOP;
		}

		if( DLG_OK == GUI::CDialog::MessageBox( m_pStrTable->String(E_STR_ID_MESSAGE_40), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK | DEFAULT_BTN_CANCEL | DEFAULT_ICON_QUESTION) )
		{
			if( DoDeleteUser( name ) )
			{
				SetRefreshView( true );
				return OnOK();
			}
			else
			{
				assert( false );
			}
		}
	}
	else
	{
		GUI::CDialog::MessageBox( m_pStrTable->String(E_STR_ID_MESSAGE_41), m_pStrTable->String(E_STR_ID_TITLE_ERROR), DEFAULT_BTN_OK | DEFAULT_ICON_ERROR);
	}

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CCfgUserDlg::OnBtnModify()
{
	NCFG_INFO_USER user;
	memset( &user, 0, sizeof(NCFG_INFO_USER) );

	int row = m_cfgUserList.GetCurSingleSelect();
	if( row >= 0 )
	{
		std::string name = m_cfgUserList.GetItemText( row, 0 );

		//... 检查是否需要被修改
		if( IsAdmin(name) )
		{
			GUI::CDialog::MessageBox( m_pStrTable->String(E_STR_ID_MESSAGE_42), m_pStrTable->String(E_STR_ID_TITLE_ERROR), DEFAULT_BTN_OK | DEFAULT_ICON_ERROR );
			return KEY_VIRTUAL_MSG_STOP;
		}

		int index = 0;
		if( !FindUser( name, user, index ) )
		{
			assert( false );
			return KEY_VIRTUAL_MSG_STOP;
		}
	}
	else
	{
		GUI::CDialog::MessageBox( m_pStrTable->String(E_STR_ID_MESSAGE_41), m_pStrTable->String(E_STR_ID_TITLE_ERROR), DEFAULT_BTN_OK | DEFAULT_ICON_ERROR );
		return KEY_VIRTUAL_MSG_STOP;
	}

	ULONGLONG CH = 0;
	int localVideoInputNum = 0, netVideoInputNum = 0;
	int maxChnn = 0, effectChnn = 0;

	IsSupportCH(CH, &effectChnn, &localVideoInputNum, &netVideoInputNum);

	if ((0 == CH) && (0 == localVideoInputNum))
	{//NVR开启远程全通道
		for (int i = 0; i < netVideoInputNum; i++)
		{
			CH |= (0x1 << i);
		}
	}
	else if ((0 < localVideoInputNum) && (0 < netVideoInputNum))
	{
		for (int i = localVideoInputNum; i < localVideoInputNum + netVideoInputNum; i++)
		{
			CH |= (0x1 <<i);
		}
	}
	maxChnn = localVideoInputNum + netVideoInputNum;

	CCfgUserOperateManDlg dlg;
	dlg.SetOperateType( CCfgUserOperateManDlg::CFG_USER_OPERATE_MODIFY );
	dlg.SetMaxUserName( m_maxUserName );
	dlg.SetMaxPassword( m_maxPassword );
	dlg.SetMaxChnn(maxChnn, CH);
	dlg.SetInfoUser( user );
	dlg.SetParentUserDlg( this );
	if( DLG_OK == dlg.DoModal(this) )
	{
		dlg.GetInfoUser( user );

		if( DoModifyUser(user) )
		{
			m_currSelect = user.name;
			SetRefreshView( true );
			return OnOK();
		}
		else
		{
			assert( false );
		}
	}
	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CCfgUserDlg::OnBtnAdd()
{
	if( GetUserCount() >= m_maxUser )
	{
		GUI::CDialog::MessageBox( m_pStrTable->String(E_STR_ID_MESSAGE_43), m_pStrTable->String(E_STR_ID_TITLE_ERROR), DEFAULT_BTN_OK | DEFAULT_ICON_ERROR );
		return KEY_VIRTUAL_MSG_STOP;
	}

	ULONGLONG CH = 0;
	int localVideoInputNum = 0, netVideoInputNum = 0;
	int maxChnn = 0, effectChnn = 0;

	IsSupportCH(CH, &effectChnn, &localVideoInputNum, &netVideoInputNum);
	if ((0 == CH) && (0 == localVideoInputNum))
	{//NVR开启远程全通道
		for (int i = 0; i < netVideoInputNum; i++)
		{
			CH |= (0x1 << i);
		}
	}
	else if ((0 < localVideoInputNum) && (0 < netVideoInputNum))
	{
		for (int i = localVideoInputNum; i < localVideoInputNum + netVideoInputNum; i++)
		{
			CH |= (0x1 <<i);
		}
	}

	maxChnn = localVideoInputNum + netVideoInputNum;

	NCFG_INFO_USER user;
	memset( &user, 0, sizeof(NCFG_INFO_USER) );

	CCfgUserOperateManDlg dlg;
	dlg.SetOperateType( CCfgUserOperateManDlg::CFG_USER_OPERATE_ADD );
	dlg.SetMaxUserName( m_maxUserName );
	dlg.SetMaxPassword( m_maxPassword );
	dlg.SetMaxChnn(maxChnn, CH);
	dlg.SetParentUserDlg( this );
	if( DLG_OK == dlg.DoModal(this) )
	{
		dlg.GetInfoUser( user );

		if( DoAddUser(user) )
		{
			m_currSelect = user.name;
			SetRefreshView( true );
			return OnOK();
		}
		else
		{
			assert( false );
		}
	}
	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CCfgUserDlg::OnBtnModifyPassword()
{
	////
	int row = m_cfgUserList.GetCurSingleSelect();
	if( row < 0 )
	{
		GUI::CDialog::MessageBox( m_pStrTable->String(E_STR_ID_MESSAGE_41), m_pStrTable->String(E_STR_ID_TITLE_ERROR), DEFAULT_BTN_OK | DEFAULT_ICON_ERROR );
		return KEY_VIRTUAL_MSG_STOP;
	}

	std::string name = m_cfgUserList.GetItemText(row, 0);

	if(m_loginName.empty() || m_loginName != name)
	{
		GUI::CDialog::MessageBox( m_pStrTable->String(E_STR_ID_TIP_06_07), m_pStrTable->String(E_STR_ID_TITLE_ERROR), DEFAULT_BTN_OK | DEFAULT_ICON_ERROR );
		return KEY_VIRTUAL_MSG_STOP;
	}

	//修改密码指修改登录者自身密码
#if defined(WIN32) ||defined(__CUSTOM_US02__)

	if (DLG_OK == MessageBox(m_pStrTable->String(E_STR_ID_MESSAGE_128), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK | DEFAULT_BTN_CANCEL | DEFAULT_ICON_WARNING))
#endif
	{
		CCfgUserPasswordDlg dlg;
		dlg.SetPasswordType( CCfgUserPasswordDlg::CFG_USER_PASSWORD_MODIFY);
		dlg.SetMaxPassword( m_maxPassword );
		dlg.DoModal(this);
	}

	return KEY_VIRTUAL_MSG_STOP;
}
