/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : L
** Date         : 2009-08-14
** Name         : CfgNetworkDlg.cpp
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/

#include "CfgSipServerDlg.h"

//const char szCaption[] = {0xE5,0x9B,0xBD,0xE6,0xA0,0x87,0xE9,0x85,0x8D,0xE7,0xBD,0xAE,0xE5,0x87,0xBA,0xE9,0x94,0x99,0x0};
//const char LANG_UTF8_XXXX[] = {0xE9,0x98,0xBF,0xE6,0x96,0xAF,0xE8,0x92,0x82,0xE8,0x8A,0xAC,0xE6,0xB3,0x95,0xE5,0xB0,0x94,0x0};

CCfgSipServerDlg::CCfgSipServerDlg()
{
}

CCfgSipServerDlg::~CCfgSipServerDlg()
{
//	GB28181_CONFIG *pGB28181_CONFIG = GB28181_CONFIG::Instance();
//	pGB28181_CONFIG->ReloadConf(SIPSERVER);
}


void CCfgSipServerDlg::OnInitial()
{
	CCfgDlg::OnInitial();
	//////////////////////////////////////////////////////////////////////////

	GUI::CListCtrl &list = m_cfgSipServerList;
	GUI::CWnd * pWnd = NULL;
	GUI::CCheckCtrl* pCheck = NULL;
	GUI::CEditCtrl*	 pEdit = NULL;
	GUI::CNumEditCtrl* pNumEdit = NULL;
	GUI::CIPAddressCtrl* pIP = NULL;
	GUI::CButton* pBtn = NULL;

	unsigned short x = m_length[DLG_OFFSET_SUB_LEFT], y = m_length[DLG_OFFSET_SUB_TOP];
	unsigned short width = m_rect.Width() - 2 * m_length[DLG_OFFSET_SUB_LEFT];
	unsigned short height = m_rect.Height() - y - m_length[DLG_OFFSET_SUB_BOTTOM];

	unsigned short offsetEdge = 0;
	int maxListRow = height/m_length[LIST_ITEM_HEIGHT]; //List框不带头
	if( ROW_NUM > maxListRow )
	{
		offsetEdge = m_length[SLIDER_WIDTH] + 2;
	}
	list.Create(GUI::CWnd::GetRandWndID(), x, y, width, height, this, 0, 0, false);

	list.AddColumn(NULL, width * 16 / 32);
	list.AddColumn(NULL, width * 16 / 32 - offsetEdge);
	////
	char str[128] = {0};
	int row = 0;
	// row = 0

	list.AddItem( ROW_SIP_SERVER_IP, 0, "Sip Server IP" );
	pIP = dynamic_cast<GUI::CIPAddressCtrl*>( list.AddItem( ROW_SIP_SERVER_IP, 1, WND_TYPE_IP) );
	assert( pIP );
	
	// row = 1
	row += 1;

	list.AddItem( ROW_SIP_SERVER_PORT, 0, "Sip Server Port" );
	pNumEdit = dynamic_cast<GUI::CNumEditCtrl*>( list.AddItem( ROW_SIP_SERVER_PORT, 1, WND_TYPE_NUM_EDIT) );
	assert( pNumEdit );
	pNumEdit->SetMaxStringLen( 5 );
	pNumEdit->SetRange(0,65535);

	// row = 2
	row += 1;

	list.AddItem( ROW_SIP_EXPIRES, 0, "Expires" );
	pNumEdit = dynamic_cast<GUI::CNumEditCtrl*>( list.AddItem( ROW_SIP_EXPIRES, 1, WND_TYPE_NUM_EDIT) );
	assert( pNumEdit );
	pNumEdit->SetMaxStringLen( 5 );
	pNumEdit->SetRange(0, (int)65535);


	// row = 3
	row += 1;
	list.AddItem( ROW_SIP_SERVER_ID, 0,"Sip Server ID" );
	pEdit = dynamic_cast<GUI::CEditCtrl*>( list.AddItem( ROW_SIP_SERVER_ID, 1, WND_TYPE_EDIT) );
	assert( pEdit);

	// row = 4
	row += 1;
	list.AddItem( ROW_SIP_USER_NAME, 0, "User Name" );
	pEdit = dynamic_cast<GUI::CEditCtrl*>( list.AddItem( ROW_SIP_USER_NAME, 1, WND_TYPE_EDIT) );
	assert( pEdit );

	// row = 5
	row += 1;
	list.AddItem( ROW_SIP_PASSWORD, 0, "Password" );
	pEdit = dynamic_cast<GUI::CEditCtrl*>( list.AddItem( ROW_SIP_PASSWORD, 1, WND_TYPE_EDIT) );
	assert( pEdit );
	pEdit->SetPassword( true );

	// row = 6
	row += 1;
	list.AddItem( ROW_DVR_ID, 0,"Dvr ID" );
	pEdit = dynamic_cast<GUI::CEditCtrl*>( list.AddItem( ROW_DVR_ID, 1, WND_TYPE_EDIT) );
	assert( pEdit);

	// row = 7
	row += 1;
	list.AddItem( ROW_DVR_PORT, 0, "Dvr Port" );
	pNumEdit = dynamic_cast<GUI::CNumEditCtrl*>( list.AddItem( ROW_DVR_PORT, 1, WND_TYPE_NUM_EDIT) );
	assert( pNumEdit );
	pNumEdit->SetMaxStringLen( 5 );
	pNumEdit->SetRange(0,65535);

	// row = 8
	row += 1;
	list.AddItem( ROW_DVR_DEVOWNER, 0,"Dev Owner" );
	pEdit = dynamic_cast<GUI::CEditCtrl*>( list.AddItem( ROW_DVR_DEVOWNER, 1, WND_TYPE_EDIT) );
	assert( pEdit);

	// row = 9
	row += 1;
	list.AddItem( ROW_DVR_CIVILCODE, 0,"Civil Code" );
	pEdit = dynamic_cast<GUI::CEditCtrl*>( list.AddItem( ROW_DVR_CIVILCODE, 1, WND_TYPE_EDIT) );
	assert( pEdit);

	// row = 10
	row += 1;
	list.AddItem( ROW_DVR_INSTALLADDR, 0,"Install Addr" );
	pEdit = dynamic_cast<GUI::CEditCtrl*>( list.AddItem( ROW_DVR_INSTALLADDR, 1, WND_TYPE_EDIT) );
	assert( pEdit);


	list.CreateListFocusMap();
}

void CCfgSipServerDlg::SetTip()
{
	//GUI::CWnd* pWnd = m_cfgSipServerList.GetItemWnd(ROW_NET_TEST, COLUMN_DATA);
	//if( NULL != pWnd )
	//{
	//	AddTip(pWnd->GetID(), E_STR_ID_TIP_05_01);
	//}
	//else
	//{
	//	printf("%s:%s:%d, GetItemWnd() error\n", __FUNCTION__, __FILE__, __LINE__);
	//	assert(false);
	//}

	//设置显示静态tip
	//GUI::CListCtrl& list = m_cfgSipServerList;
	//AddTip(list.GetItemWnd(ROW_SIP_SERVER_PORT,1)->GetID(),E_STR_ID_TIP_01_37);
	//AddTip(list.GetItemWnd(ROW_VIDEO_FORMAT,1)->GetID(),E_STR_ID_TIP_01_37);
	//AddTip(list.GetItemWnd(GetRealRow(ROW_LANGUAGE), 1)->GetID(), E_STR_ID_TIP_01_38);
	//AddTip(list.GetItemWnd(ROW_VGA_RESOLUTION,1)->GetID(),E_STR_ID_TIP_01_38);

}

bool CCfgSipServerDlg::ShowData(NCFG_ITEM_ID cfgID, const BYTE* pData, int len, bool bRefresh)
{
	assert(NULL != pData);
	assert(len > 0);
	GUI::CListCtrl& list = m_cfgSipServerList;

	BYTE* pTempData = NULL;
	unsigned long tempDataLen = 0;

	switch (cfgID)
	{
	case NCFG_ITEM_GB28181_SERVER_INFO:
		{
			assert( len == sizeof(GB28181_SIP_SERVER));
			const GB28181_SIP_SERVER* pValue = reinterpret_cast<const GB28181_SIP_SERVER *>(pData);

			GUI::CIPAddressCtrl* pIP = dynamic_cast<GUI::CIPAddressCtrl*>(list.GetItemWnd(ROW_SIP_SERVER_IP,COLUMN_DATA));
			assert(pIP);
			pIP->SetIP(pValue->server_ip,bRefresh);

			GUI::CNumEditCtrl* pEdit = dynamic_cast<GUI::CNumEditCtrl*>( list.GetItemWnd(ROW_SIP_SERVER_PORT, COLUMN_DATA) );
			assert( pEdit );
			pEdit->SetValue(pValue->server_port, bRefresh );

			GUI::CNumEditCtrl* pExpiresEdit = dynamic_cast<GUI::CNumEditCtrl*>( list.GetItemWnd(ROW_SIP_EXPIRES, COLUMN_DATA) );
			assert( pExpiresEdit );
			pExpiresEdit->SetValue(pValue->expires, bRefresh );

			GUI::CEditCtrl* pServerIDEdit = dynamic_cast<GUI::CEditCtrl*>( list.GetItemWnd(ROW_SIP_SERVER_ID, COLUMN_DATA) );
			assert( pServerIDEdit );
			pServerIDEdit->SetMaxStringLen(32);
			pServerIDEdit->SetCaption((char *)pValue->server_id,bRefresh);

			GUI::CEditCtrl* pUserEdit = dynamic_cast<GUI::CEditCtrl*>( list.GetItemWnd(ROW_SIP_USER_NAME, COLUMN_DATA) );
			assert( pUserEdit );
			pUserEdit->SetMaxStringLen(32);
			pUserEdit->SetCaption((char *)pValue->user,bRefresh);

			GUI::CEditCtrl* pPwdEdit = dynamic_cast<GUI::CEditCtrl*>( list.GetItemWnd(ROW_SIP_PASSWORD, COLUMN_DATA) );
			assert( pPwdEdit );
			pPwdEdit->SetMaxStringLen(32);
			pPwdEdit->SetCaption((char *)pValue->passwd,bRefresh);

			GUI::CEditCtrl* pDvrIDEdit = dynamic_cast<GUI::CEditCtrl*>( list.GetItemWnd(ROW_DVR_ID, COLUMN_DATA) );
			assert( pDvrIDEdit );
			pDvrIDEdit->SetMaxStringLen(32);
			pDvrIDEdit->SetCaption((char *)pValue->dvr_id,bRefresh);

			GUI::CNumEditCtrl* pDvrPortEdit = dynamic_cast<GUI::CNumEditCtrl*>( list.GetItemWnd(ROW_DVR_PORT, COLUMN_DATA) );
			assert( pDvrPortEdit );
			pDvrPortEdit->SetValue(pValue->dvr_port, bRefresh );

			GUI::CEditCtrl* pDevOwnerEdit = dynamic_cast<GUI::CEditCtrl*>( list.GetItemWnd(ROW_DVR_DEVOWNER, COLUMN_DATA) );
			assert( pDevOwnerEdit );
			pDevOwnerEdit->SetMaxStringLen(64);
			pDevOwnerEdit->SetCaption((char *)pValue->devowner,bRefresh);

			GUI::CEditCtrl* pCivilCodeEdit = dynamic_cast<GUI::CEditCtrl*>( list.GetItemWnd(ROW_DVR_CIVILCODE, COLUMN_DATA) );
			assert( pCivilCodeEdit );
			pCivilCodeEdit->SetMaxStringLen(64);
			pCivilCodeEdit->SetCaption((char *)pValue->civilcode,bRefresh);

			GUI::CEditCtrl* pInstallAddrEdit = dynamic_cast<GUI::CEditCtrl*>( list.GetItemWnd(ROW_DVR_INSTALLADDR, COLUMN_DATA) );
			assert( pInstallAddrEdit );
			pInstallAddrEdit->SetMaxStringLen(64);
			pInstallAddrEdit->SetCaption((char *)pValue->installaddr,bRefresh);

		}
		return true;
		break;
	default:
		break;
	}
	return false;
}

bool CCfgSipServerDlg::SetData(NCFG_ITEM_ID cfgID, BYTE*& pData, int& len)  const
{
	printf("CCfgSipServerDlg::SetData called!\n");
	assert(NULL != pData);
	assert(len > 0);
	const GUI::CListCtrl& list = m_cfgSipServerList;

	switch (cfgID)
	{
	case NCFG_ITEM_GB28181_SERVER_INFO:
		{
			assert( len == sizeof(GB28181_SIP_SERVER));
			GB28181_SIP_SERVER * pValue = reinterpret_cast<GB28181_SIP_SERVER*>(pData);

			const GUI::CIPAddressCtrl* pIP = dynamic_cast<const GUI::CIPAddressCtrl*>(list.GetItemWnd(ROW_SIP_SERVER_IP,COLUMN_DATA));
			assert(pIP);
			pValue->server_ip = pIP->GetIP();


			const GUI::CNumEditCtrl* pEdit = dynamic_cast<const GUI::CNumEditCtrl*>( list.GetItemWnd(ROW_SIP_SERVER_PORT, COLUMN_DATA) );
			assert( pEdit );
			pValue->server_port = pEdit->GetValue();

			const GUI::CNumEditCtrl* pExpiresEdit = dynamic_cast<const GUI::CNumEditCtrl*>( list.GetItemWnd(ROW_SIP_EXPIRES, COLUMN_DATA) );
			assert( pExpiresEdit );
			pValue->expires = pExpiresEdit->GetValue();


			const GUI::CEditCtrl* pSipServerEdit = dynamic_cast<const GUI::CEditCtrl*>( list.GetItemWnd(ROW_SIP_SERVER_ID, COLUMN_DATA) );
			assert( pSipServerEdit );
			strcpy((char *)pValue->server_id,pSipServerEdit->GetCaption().c_str());

			const GUI::CEditCtrl* pUserEdit = dynamic_cast<const GUI::CEditCtrl*>( list.GetItemWnd(ROW_SIP_USER_NAME, COLUMN_DATA) );
			assert( pUserEdit );
			strcpy((char *)pValue->user,pUserEdit->GetCaption().c_str());


			const GUI::CEditCtrl* pPwdEdit = dynamic_cast<const GUI::CEditCtrl*>( list.GetItemWnd(ROW_SIP_PASSWORD, COLUMN_DATA) );
			assert( pPwdEdit );
			strcpy((char *)pValue->passwd,pPwdEdit->GetCaption().c_str());


			const GUI::CEditCtrl* pDvrIdEdit = dynamic_cast<const GUI::CEditCtrl*>( list.GetItemWnd(ROW_DVR_ID, COLUMN_DATA) );
			assert( pDvrIdEdit );
			strcpy((char *)pValue->dvr_id,pDvrIdEdit->GetCaption().c_str());


			const GUI::CNumEditCtrl* pDvrPortEdit = dynamic_cast<const GUI::CNumEditCtrl*>( list.GetItemWnd(ROW_DVR_PORT, COLUMN_DATA) );
			assert( pDvrPortEdit );
			pValue->dvr_port = pDvrPortEdit->GetValue();


			const GUI::CEditCtrl* pDevOwnerEdit = dynamic_cast<const GUI::CEditCtrl*>( list.GetItemWnd(ROW_DVR_DEVOWNER, COLUMN_DATA) );
			assert( pDevOwnerEdit );
			strcpy((char *)pValue->devowner,pDevOwnerEdit->GetCaption().c_str());

			const GUI::CEditCtrl* pCivilCodeEdit = dynamic_cast<const GUI::CEditCtrl*>( list.GetItemWnd(ROW_DVR_CIVILCODE, COLUMN_DATA) );
			assert( pCivilCodeEdit );
			strcpy((char *)pValue->civilcode,pCivilCodeEdit->GetCaption().c_str());

			const GUI::CEditCtrl* pInstallAddrEdit = dynamic_cast<const GUI::CEditCtrl*>( list.GetItemWnd(ROW_DVR_INSTALLADDR, COLUMN_DATA) );
			assert( pInstallAddrEdit );
			strcpy((char *)pValue->installaddr,pInstallAddrEdit->GetCaption().c_str());
		}
		return true;
		break;
	default:
		break;
	}

	return false;
}

BEGIN_MSG_MAP(CCfgSipServerDlg, GUI::CDialog)
	ON_MSG_EX(m_cfgSipServerList.GetID(), KEY_VIRTUAL_MSG_DEFAULT_TAG, OnClickList)
END_MSG_MAP()

unsigned long CCfgSipServerDlg::OnClickList(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	unsigned short row = (keyID >> 16)&0x7fff;	//行
	unsigned short col = keyID&0xffff;			//列

	if (!IsModify() && 1 == col)
	{
		switch (row)
		{
		case ROW_SIP_SERVER_IP:
			if (IsChange(NCFG_ITEM_GB28181_SERVER_INFO))
			{
				SetModify(true);
				EnableOkBtn(true, true);
				return KEY_VIRTUAL_MSG_CONTINUE;
			}
			break;
		case ROW_SIP_SERVER_PORT:
			if (IsChange(NCFG_ITEM_GB28181_SERVER_INFO))
			{
				SetModify(true);
				EnableOkBtn(true, true);
				return KEY_VIRTUAL_MSG_CONTINUE;
			}
			break;
		case ROW_SIP_EXPIRES:
			if (IsChange(NCFG_ITEM_GB28181_SERVER_INFO))
			{
				SetModify(true);
				EnableOkBtn(true, true);
				return KEY_VIRTUAL_MSG_CONTINUE;
			}
			break;
		case ROW_SIP_SERVER_ID:
			if (IsChange(NCFG_ITEM_GB28181_SERVER_INFO))
			{
				SetModify(true);
				EnableOkBtn(true, true);
				return KEY_VIRTUAL_MSG_CONTINUE;
			}
			break;
		case ROW_SIP_USER_NAME:
			if (IsChange(NCFG_ITEM_GB28181_SERVER_INFO))
			{
				SetModify(true);
				EnableOkBtn(true, true);
				return KEY_VIRTUAL_MSG_CONTINUE;
			}
			break;
		case ROW_SIP_PASSWORD:
			if (IsChange(NCFG_ITEM_GB28181_SERVER_INFO))
			{
				SetModify(true);
				EnableOkBtn(true, true);
				return KEY_VIRTUAL_MSG_CONTINUE;
			}
		case ROW_DVR_ID:
			if (IsChange(NCFG_ITEM_GB28181_SERVER_INFO))
			{
				SetModify(true);
				EnableOkBtn(true, true);
				return KEY_VIRTUAL_MSG_CONTINUE;
			}
		case ROW_DVR_PORT:
			if (IsChange(NCFG_ITEM_GB28181_SERVER_INFO))
			{
				SetModify(true);
				EnableOkBtn(true, true);
				return KEY_VIRTUAL_MSG_CONTINUE;
			}
		case ROW_DVR_DEVOWNER:
			if (IsChange(NCFG_ITEM_GB28181_SERVER_INFO))
			{
				SetModify(true);
				EnableOkBtn(true, true);
				return KEY_VIRTUAL_MSG_CONTINUE;
			}
		case ROW_DVR_CIVILCODE:
			if (IsChange(NCFG_ITEM_GB28181_SERVER_INFO))
			{
				SetModify(true);
				EnableOkBtn(true, true);
				return KEY_VIRTUAL_MSG_CONTINUE;
			}
		case ROW_DVR_INSTALLADDR:
			if (IsChange(NCFG_ITEM_GB28181_SERVER_INFO))
			{
				SetModify(true);
				EnableOkBtn(true, true);
				return KEY_VIRTUAL_MSG_CONTINUE;
			}
		//default:
		//	assert(false);
			break;
		}
	} 

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CCfgSipServerDlg::PreCheckValue()
{
	if(IsChange(NCFG_ITEM_GB28181_SERVER_INFO)) 
	{
		return E_STR_ID_TIP_03_01;
	}

	return 0;
}


bool CCfgSipServerDlg::IsValidID(const string &strID)
{
	if (strID == "")
	{
		return true;
	}
	char p;
	int i=0;
	for (;i<strID.length();i++)
	{
		p = strID[i];
		if ( p <'0' || p> '9')
		{
			return false;
		}
	}
	if ( i != 20)
	{
		return false;
	}
	return true;
}

const char szCaption[] = {0xE5,0x9B,0xBD,0xE6,0xA0,0x87,0xE9,0x85,0x8D,0xE7,0xBD,0xAE,0xE5,0x87,0xBA,0xE9,0x94,0x99,0x0};			//国标配置出错
const char LANG_UTF8_00[] = {0xE7,0xAB,0xAF,0xE5,0x8F,0xA3,0xE5,0x8F,0xB7,0xE4,0xB8,0x8D,0xE8,0x83,0xBD,0xE4,0xB8,0xBA,0x30,0x0};	//端口号不能为0
const char LANG_UTF8_01[] = {0xE7,0x94,0xA8,0xE6,0x88,0xB7,0xE5,0x90,0x8D,0xE4,0xB8,0x8D,0xE8,0x83,0xBD,0xE4,0xB8,0xBA,0xE7,0xA9,0xBA,0x0};//用户名不能为空
const char LANG_UTF8_02[] = {0xE5,0xAF,0x86,0xE7,0xA0,0x81,0xE4,0xB8,0x8D,0xE8,0x83,0xBD,0xE4,0xB8,0xBA,0xE7,0xA9,0xBA,0x0};		//密码不能为空
const char LANG_UTF8_03[] = {0x49,0x44,0xE4,0xB8,0x8D,0xE7,0xAC,0xA6,0xE5,0x90,0x88,0xE8,0xA7,0x84,0xE8,0x8C,0x83,0x0};				//ID不符合规范


unsigned long CCfgSipServerDlg::CheckValue()
{
	if(IsChange(NCFG_ITEM_GB28181_SERVER_INFO)) 
	{
		unsigned long ulSipServerPort = dynamic_cast<const GUI::CNumEditCtrl*>( m_cfgSipServerList.GetItemWnd(ROW_SIP_SERVER_PORT, COLUMN_DATA) )->GetValue();
		unsigned long ulDvrPort = dynamic_cast<const GUI::CNumEditCtrl*>( m_cfgSipServerList.GetItemWnd(ROW_DVR_PORT, COLUMN_DATA) )->GetValue();
		if(ulSipServerPort == 0 || ulDvrPort == 0)
		{
			MessageBox(LANG_UTF8_00,szCaption,DEFAULT_BTN_OK);
			return E_STR_ID_TIP_00_07;
		}
		string pUserEdit = dynamic_cast<const GUI::CEditCtrl*>( m_cfgSipServerList.GetItemWnd(ROW_SIP_USER_NAME, COLUMN_DATA))->GetCaption().c_str();
		if (pUserEdit == "")
		{
			MessageBox(LANG_UTF8_01,szCaption,DEFAULT_BTN_OK);
			return E_STR_ID_TIP_00_07;
		}
		string pPwdEdit = dynamic_cast<const GUI::CEditCtrl*>( m_cfgSipServerList.GetItemWnd(ROW_SIP_PASSWORD, COLUMN_DATA))->GetCaption().c_str();
		if (pPwdEdit == "")
		{
			MessageBox(LANG_UTF8_02,szCaption,DEFAULT_BTN_OK);
			return E_STR_ID_TIP_00_07;
		}
		string pSipServerID = dynamic_cast<const GUI::CEditCtrl*>( m_cfgSipServerList.GetItemWnd(ROW_SIP_SERVER_ID, COLUMN_DATA))->GetCaption().c_str();
		if (!IsValidID(pSipServerID))
		{
			MessageBox(LANG_UTF8_03,szCaption,DEFAULT_BTN_OK);
			return E_STR_ID_TIP_00_07;
		}

		string pDvrID = dynamic_cast<const GUI::CEditCtrl*>( m_cfgSipServerList.GetItemWnd(ROW_DVR_ID, COLUMN_DATA))->GetCaption().c_str();
		if (!IsValidID(pDvrID))
		{
			MessageBox(LANG_UTF8_03,szCaption,DEFAULT_BTN_OK);
			return E_STR_ID_TIP_00_07;
		}

	}
	return 0;
}