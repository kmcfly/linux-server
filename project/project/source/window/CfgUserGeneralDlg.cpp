/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : L
** Date         : 2009-08-15
** Name         : CfgUserGeneralDlg.cpp
** Version      : 1.0
** Description  : 
** Modify Record: 2009-08-29
1:
***********************************************************************/

#include "CfgUserGeneralDlg.h"
#include "CfgUserPasswordDlg.h"
#include "CfgUserDlg.h"
#include "CfgUserOperateManDlg.h"

CCfgUserGeneralDlg::CCfgUserGeneralDlg()
{
	m_cfgUserDlgType = CFG_USER_OPERATE_ADD;
	m_maxUserName = 0;
	m_maxPassword = 0;
}

CCfgUserGeneralDlg::~CCfgUserGeneralDlg()
{

}

void CCfgUserGeneralDlg::OnInitial()
{
	GUI::CDialog::OnInitial();
	//////////////////////////////////////////////////////////////////////////

	GUI::CListCtrl& list = m_cfgUserGener;
	GUI::CWnd * pWnd = NULL;
	GUI::CCheckCtrl* pCheck = NULL;
	GUI::CComboCtrl* pCombo = NULL;
	GUI::CEditCtrl*  pEdit = NULL;
	GUI::CButton* pBtn = NULL;

	unsigned short x = m_length[DLG_OFFSET_SUB_LEFT], y = m_length[DLG_OFFSET_SUB_TOP];
	unsigned short xOff = 10, yOff = 0;

	unsigned short width = m_rect.Width() - 2 * m_length[DLG_OFFSET_SUB_LEFT];
	unsigned short height = m_rect.Height() - y - m_length[DLG_OFFSET_SUB_BOTTOM];

	char str[128] = {0};

	/////List
	list.Create(GUI::CWnd::GetRandWndID(), x, y, width, height, this, 0, 0, false);
	list.AddColumn("", width / 2);
	list.AddColumn("", width / 2 - 4 );

	int row = 0;
	int col = 0;

	// row = 0
	row = 0;
	list.AddItem( row, 0, m_pStrTable->String( E_STR_ID_USER_NAME ) );
	pEdit = dynamic_cast<GUI::CEditCtrl*>( list.AddItem( row, 1, WND_TYPE_EDIT ) );
	assert( pEdit );
	pEdit->SetCaption("guest", false );
	pEdit->SetMaxStringLen( m_maxUserName );
	if( CFG_USER_OPERATE_MODIFY == m_cfgUserDlgType ) pEdit->SetEnable( false, false );

	// row = 1
	{
		if( CFG_USER_OPERATE_MODIFY == m_cfgUserDlgType )
		{
			// none
		}
		else
		{
			row += 1;
			list.AddItem( row, 0, m_pStrTable->String( E_STR_ID_PASSWORD ) );
			pEdit = dynamic_cast<GUI::CEditCtrl*>( list.AddItem( row, 1, WND_TYPE_EDIT) );
			assert( pEdit );
			pEdit->SetPassword( true );
			pEdit->SetMaxStringLen( m_maxPassword );

			// row ++
			row += 1;
			list.AddItem( row, 0, m_pStrTable->String( E_STR_ID_COMFIRM_PASSWORD ) );
			pEdit = dynamic_cast<GUI::CEditCtrl*>( list.AddItem( row, 1, WND_TYPE_EDIT) );
			assert( pEdit );
			pEdit->SetMaxStringLen( m_maxPassword );
			pEdit->SetPassword( true );
		}
	}

	//
	row += 1;
	list.AddItem( row, 0, m_pStrTable->String( E_STR_ID_USER_TYPE ) );
	pCombo = dynamic_cast<GUI::CComboCtrl*>( list.AddItem( row, 1, WND_TYPE_COMBO ) );
	assert( pCombo );
	pCombo->AddItem( m_pStrTable->String(E_STR_ID_NORMAL) , USER_GROUP_NORMAL);
	pCombo->AddItem( m_pStrTable->String(E_STR_ID_ADVANCED_USER) , USER_GROUP_ADVANCE);
	pCombo->SetMaxStringLen( 64 );
	pCombo->SetCurItem( 0 , false );
	//
#if !defined(__TW01_RILI__)
	row += 1;
	list.AddItem( row, 0, m_pStrTable->String( E_STR_ID_BIND_PC_MAC_ADDR ));
	pCheck = dynamic_cast<GUI::CCheckCtrl*>( list.AddItem(row, 1, WND_TYPE_CHECK) );


	row += 1;
	list.AddItem( row, 0, m_pStrTable->String( E_STR_ID_PC_MAC_ADDR ) );
	pEdit = dynamic_cast<GUI::CEditCtrl*>( list.AddItem( row, 1, WND_TYPE_EDIT ) );
	assert( pEdit );
	strcpy( str, "00-00-00-00-00-00" );
	pEdit->SetMaxStringLen( 17 );
	pEdit->SetCaption( str, false );
#endif
	list.CreateListFocusMap();
}

void CCfgUserGeneralDlg::SetOperateType( CFG_USER_OPERATE_TYPE type )
{
	m_cfgUserDlgType = type;
}


void CCfgUserGeneralDlg::SetMaxUserName( int maxUserName )
{
	m_maxUserName = maxUserName;
}

void CCfgUserGeneralDlg::SetMaxPassword( int maxPassword )
{
	m_maxPassword = maxPassword;
}

void CCfgUserGeneralDlg::ShowInfoUserGeneral( const NCFG_INFO_USER& infoUser, bool bRefresh )
{
	GUI::CListCtrl& list = m_cfgUserGener;
	switch ( m_cfgUserDlgType )
	{
	case CFG_USER_OPERATE_ADD:
		break;
	case CFG_USER_OPERATE_MODIFY:
		{
			GUI::CEditCtrl* pEdit = NULL;
			GUI::CComboCtrl* pCombo = NULL;
			GUI::CCheckCtrl* pCheck = NULL;

			pEdit = dynamic_cast<GUI::CEditCtrl*>(list.GetItemWnd(ROW_NAME_MODIFY, 1));
			assert(pEdit);
			pEdit->SetCaption( infoUser.name, bRefresh );
			
			pCombo = dynamic_cast<GUI::CComboCtrl*>(list.GetItemWnd(ROW_TYPE_MODIFY, 1));
			assert( pCombo );
			pCombo->SetCurItemData( infoUser.group , bRefresh );

#if !defined(__TW01_RILI__)
			pCheck = dynamic_cast<GUI::CCheckCtrl*>(list.GetItemWnd(ROW_MAC_ENABLE_MODIFY, 1));
			assert(pCheck);
			if(0 != infoUser.bindMac)
			{
				pCheck->SetCheck(true, bRefresh);
			}
			else
			{
				pCheck->SetCheck(false, bRefresh);
			}

			//

			pEdit = dynamic_cast<GUI::CEditCtrl*>(list.GetItemWnd(ROW_MAC_MODIFY, 1));
			assert(pEdit);
			char macStr[64] = {NULL};
			sprintf( macStr, "%.2X-%.2X-%.2X-%.2X-%.2X-%.2X", 
							infoUser.MAC[0],infoUser.MAC[1],infoUser.MAC[2],
							infoUser.MAC[3],infoUser.MAC[4],infoUser.MAC[5]);
			pEdit->SetCaption( macStr, bRefresh );
#endif
		}
		break;
	default:
		break;
	}
}


unsigned long CCfgUserGeneralDlg::CheckValue()
{
	//常规检查
	const GUI::CListCtrl& list = m_cfgUserGener;

	switch ( m_cfgUserDlgType )
	{
	case CFG_USER_OPERATE_ADD:
		{
			const GUI::CEditCtrl* pEdit = dynamic_cast<const GUI::CEditCtrl*>(list.GetItemWnd( ROW_NAME_ADD, 1 ));
			assert( pEdit );
			if( pEdit->GetCaption().empty() ) 
			{
				return CCfgUserDlg::CFG_USER_ERR_USERNAME;
			}

			pEdit = dynamic_cast<const GUI::CEditCtrl*>(list.GetItemWnd(ROW_PASS_ADD, 1));
			assert( pEdit );
			std::string strPass1 = pEdit->GetCaption();

			pEdit = dynamic_cast<const GUI::CEditCtrl*>(list.GetItemWnd(ROW_CONFIRM_PASS_ADD, 1));
			assert( pEdit );
			std::string strPass2 = pEdit->GetCaption();

			if( 0 != strPass1.compare( strPass2 ) ) 
			{
				return CCfgUserDlg::CFG_USER_ERR_CONFIRM_PASS;
			}
#if !defined(__TW01_RILI__)
			pEdit = dynamic_cast<const GUI::CEditCtrl*>(list.GetItemWnd(ROW_MAC_ADD, 1) );
			assert( pEdit );
			std::string macStr = pEdit->GetCaption();
			unsigned char testMac[8] = {0};
			if( !GetMac(macStr, testMac) )
			{
				return CCfgUserDlg::CFG_USER_ERR_MAC;
			}
#endif
			return CCfgUserDlg::CFG_USER_OK;
		}
		break;
	case CFG_USER_OPERATE_MODIFY:
		{
#if !defined(__TW01_RILI__)
			const GUI::CEditCtrl* pEdit = dynamic_cast<const GUI::CEditCtrl*>(list.GetItemWnd(ROW_MAC_MODIFY, 1) );
			assert( pEdit );
			std::string macStr = pEdit->GetCaption();

			unsigned char testMac[8] = {0};
			if( !GetMac(macStr, testMac) )
			{
				return CCfgUserDlg::CFG_USER_ERR_MAC;
			}
#endif
			return CCfgUserDlg::CFG_USER_OK;
		}
		break;
	default:
		assert( false );
		return CCfgUserDlg::CFG_USER_OK;
	}
	return CCfgUserDlg::CFG_USER_OK;
}

bool CCfgUserGeneralDlg::GetMac( const std::string& str, unsigned char pMac[] ) const 
{
	if( str.size() < 17 ) return false;
	char tmpStr[3] = {0};
	unsigned char result[6] = {0};

	//只要mac地址中有一位不为0，则认为是绑定mac地址

	for ( int i=0; i<6; i++ )
	{
		tmpStr[0] = str.at( 3 * i );
		if( !isxdigit(tmpStr[0]) ) return false;

		tmpStr[1] = str.at( 3 * i + 1 );
		if( !isxdigit(tmpStr[1]) ) return false;
		if( i < 5 ) 
		{
			char seg = str.at( 3 * i + 2 );
			switch( seg )
			{
			case '-':
				break;
			case ' ':
				break;
			default:
				return false;
				break;
			}
		}

		result[i] = strtoul( tmpStr, NULL, 16 );
	}

	memcpy( pMac, &result, 6 * sizeof(unsigned char) );

	return true;
}

unsigned long CCfgUserGeneralDlg::GetBindingMac(const unsigned char pMac[]) const
{
	unsigned long binding = 0;
	for ( int i = 0; i < 6; i++ )
	{
		if( pMac[i] != 0 )
		{
			binding = 1;
			break;
		}
	}

	return binding;
}

bool CCfgUserGeneralDlg::GetValue(NCFG_INFO_USER& infoUser )
{
	const GUI::CListCtrl& list = m_cfgUserGener;

	switch ( m_cfgUserDlgType )
	{
	case CFG_USER_OPERATE_ADD:
		{
			const GUI::CEditCtrl* pEdit = NULL;
			
			pEdit = dynamic_cast<const GUI::CEditCtrl*>(list.GetItemWnd( ROW_NAME_ADD, 1 ));
			assert( pEdit );
			strcpy( infoUser.name, pEdit->GetCaption().c_str() );

			pEdit = dynamic_cast<const GUI::CEditCtrl*>(list.GetItemWnd(ROW_PASS_ADD, 1));
			assert( pEdit );
			strcpy( infoUser.password, pEdit->GetCaption().c_str() );

			const GUI::CComboCtrl* pCombo = NULL;
			pCombo = dynamic_cast<const GUI::CComboCtrl*>(list.GetItemWnd(ROW_TYPE_ADD, 1) );
			assert( pCombo );
			infoUser.group = pCombo->GetCurItemData();

			// ... to do MAC
#if !defined(__TW01_RILI__)
			pEdit = dynamic_cast<const GUI::CEditCtrl*>(list.GetItemWnd(ROW_MAC_ADD, 1) );
			assert( pEdit );
			std::string macStr = pEdit->GetCaption();

			memset(infoUser.MAC, 0, sizeof(infoUser.MAC));
			assert(sizeof(infoUser.MAC) >= 8);
			GetMac(macStr, infoUser.MAC );

			//
			const GUI::CCheckCtrl* pCheck = dynamic_cast<const GUI::CCheckCtrl*>(list.GetItemWnd(ROW_MAC_ENABLE_ADD, 1));
			assert(pCheck);
			if(pCheck->IsCheck())
			{
				infoUser.bindMac = 1;
			}
			else
			{
				infoUser.bindMac = 0;
			}
#else
			memset(infoUser.MAC, 0, sizeof(infoUser.MAC));
			infoUser.bindMac = 0;
#endif
			
		}
		break;
	case CFG_USER_OPERATE_MODIFY:
		{
			const GUI::CComboCtrl* pCombo = NULL;
			pCombo = dynamic_cast<const GUI::CComboCtrl*>(list.GetItemWnd(ROW_TYPE_MODIFY, 1) );
			assert( pCombo );
			infoUser.group = pCombo->GetCurItemData();

			// ... to do MAC
#if !defined(__TW01_RILI__)
			const GUI::CEditCtrl* pEdit = dynamic_cast<const GUI::CEditCtrl*>(list.GetItemWnd(ROW_MAC_MODIFY, 1) );
			assert( pEdit );
			std::string macStr = pEdit->GetCaption();

			memset(infoUser.MAC, 0, sizeof(infoUser.MAC));
			assert(sizeof(infoUser.MAC) >= 8);

			GetMac(macStr, infoUser.MAC );

			const GUI::CCheckCtrl* pCheck = dynamic_cast<const GUI::CCheckCtrl*>(list.GetItemWnd(ROW_MAC_ENABLE_MODIFY, 1));
			assert(pCheck);
			if(pCheck->IsCheck())
			{
				infoUser.bindMac = 1;
			}
			else
			{
				infoUser.bindMac = 0;
			}
#else
			memset(infoUser.MAC, 0, sizeof(infoUser.MAC));
#endif
		}
		break;
	default:
		assert( false );
		return false;
	}
	return true;
}

bool CCfgUserGeneralDlg::GetInfoUserGeneral( NCFG_INFO_USER& infoUser )
{
	//检查数据的正确性由上层窗口来进行

	if( GetValue(infoUser) )
	{
		return true;
	}
	return false;
}

BEGIN_MSG_MAP(CCfgUserGeneralDlg, GUI::CDialog)
	ON_MSG_EX(m_cfgUserGener.GetID(), KEY_VIRTUAL_MSG_DEFAULT_TAG, OnClickList)
END_MSG_MAP()

unsigned long CCfgUserGeneralDlg::OnClickList(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	unsigned short row = (keyID >> 16)&0x7fff;	//行
	unsigned short col = keyID&0xffff;			//列

	switch ( m_cfgUserDlgType )
	{
	case CFG_USER_OPERATE_ADD:
		{
			if( ROW_TYPE_ADD == row && 1 == col )
			{
				CCfgUserOperateManDlg* pUserManDlg = dynamic_cast<CCfgUserOperateManDlg*>( this->GetParent() );
				assert( pUserManDlg );
				
				GUI::CComboCtrl* pCombo = dynamic_cast<GUI::CComboCtrl*>(m_cfgUserGener.GetItemWnd( row, col ));
				assert( pCombo );

				pUserManDlg->ShowDefaultAuth( pCombo->GetCurItemData(), false );
			}
		}
		break;
	case CFG_USER_OPERATE_MODIFY:
		{
			if( ROW_TYPE_MODIFY == row && 1 == col )
			{
				CCfgUserOperateManDlg* pUserManDlg = dynamic_cast<CCfgUserOperateManDlg*>( this->GetParent() );
				assert( pUserManDlg );

				GUI::CComboCtrl* pCombo = dynamic_cast<GUI::CComboCtrl*>(m_cfgUserGener.GetItemWnd( row, col ));
				assert( pCombo );
				
				pUserManDlg->ShowDefaultAuth( pCombo->GetCurItemData(), false );
			}
		}
		break;
	default:
		assert(false);
		break;
	}

	return KEY_VIRTUAL_MSG_STOP;
}
