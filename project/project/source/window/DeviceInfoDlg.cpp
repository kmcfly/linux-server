/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : L
** Date         : 2009-09-29
** Name         : DeviceInfoDlg.h
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/

#include "DeviceInfoDlg.h"
#include "Product.h"
#include "RegNetDeviceLog.h"
#include "NetProtocol.h"
#include "IPDataBufferDefine.h"
#include "dvrdvsdefine.h"
#include "NdLibMan.h"
#include "FuncCustom.h"
#include "MessageMan.h"

extern NET_LOGIN_SUCCESS_REPLY *g_p_login_succ;
//////////////////////////////////////////////////////////////////////////
CDeviceInfoDlg::CDeviceInfoDlg()
{
	m_dlgType = DEVICE_ADD;
	memset( &m_deviceInfo, 0, sizeof(IP_DEVICE_INFO) );
	m_deviceInfo.channel = INVALID_INDEX(unsigned char);

	for(int i = 0; i < ROW_NUM; i++)
	{
		m_rows[i] = -1;
	}
}

CDeviceInfoDlg::~CDeviceInfoDlg()
{
	m_vecManufacturer.clear();
}

void CDeviceInfoDlg::OnInitial()
{
	char str[256] = {0};

	switch ( m_dlgType )
	{
	case DEVICE_ADD:
		snprintf( str, 256, "%s", m_pStrTable->String(E_STR_ID_TITLE_ADD_DEVICE) );
		break;
	case DEVICE_MODIFY:
		snprintf( str, 256, "%s", m_pStrTable->String(E_STR_ID_TITLE_MODIFY_DEVICE)  );
		break;
	}

	GUI::CDialog::OnInitial();
	SetImage(BMP_MEM_DLG_LARGE, false);
	SetCaption(str, false);

	SetDefaultBtn( DEFAULT_BTN_OK | DEFAULT_BTN_CANCEL );
	//////////////////////////////////////////////////////////////////////////
	GUI::CListCtrl& list = m_listCtrl;
	GUI::CCheckCtrl* pCheck = NULL;
	GUI::CComboCtrl* pCombo = NULL;
	GUI::CEditCtrl* pEdit = NULL;
	GUI::CNumEditCtrl* pNumEdit = NULL;
	unsigned char invalid = INVALID_INDEX(unsigned char);

	int iSelManufacturer = -1;
	int iSelModel = -1;

	unsigned short x = m_length[DLG_OFFSET_LEFT], y = m_length[TITLE_HEIGHT] + m_length[DLG_OFFSET_TOP];
	unsigned short width = m_rect.Width() - 2 * m_length[DLG_OFFSET_LEFT];
	unsigned short height = m_rect.Height() - y - m_length[DEFAULT_BTN_BOTTOM_YPOS] - m_length[DLG_OFFSET_BOTTOM];

	list.Create( GUI::CWnd::GetRandWndID(), x, y, width, height, this, 0, 1, false );
	list.AddColumn( NULL , width / 2 );
	list.AddColumn( NULL , width / 2 - 2 );

	if( DEVICE_ADD == m_dlgType )
	{
		int j = 0;
		int idx = -1;
		for (j = 0; j < m_deviceCount; j++)
		{
			if(invalid != m_pDeviceInfo[j].channel)
			{
				idx = j;
			}
		}

		if( idx >= 0 )
		{
			memcpy( &m_deviceInfo, &m_pDeviceInfo[idx], sizeof(IP_DEVICE_INFO));
			if (DEVICE_ADD == m_dlgType)
			{
				m_deviceInfo.productModel[0] = '\0';
			}

			m_deviceInfo.bPOEDevice = false;
			m_deviceInfo.channel = invalid;
			memset( m_deviceInfo.szID, 0, sizeof(m_deviceInfo.szID) );
			strcpy( m_deviceInfo.szEtherName, "eth0" );

			for( j = (strlen(m_deviceInfo.szServer)-1); j > 0; j-- )
			{
				if( '.' == m_deviceInfo.szServer[j] || ':' == m_deviceInfo.szServer[j])
				{
					break;
				}
				else
				{
					m_deviceInfo.szServer[j] = '\0';
				}
			}
		}
	}

	int row = 0;

	//////////////////////////////////////////////////////////////////////////
	//是否启用
	m_rows[ROW_ENABLE] = row;
	list.AddItem( row, 0, m_pStrTable->String(E_STR_ID_ENABLE) );
	pCheck = dynamic_cast<GUI::CCheckCtrl*>( list.AddItem( row, 1, WND_TYPE_CHECK ) );
	assert( pCheck );
	pCheck->SetCheck( invalid  != m_deviceInfo.channel );
	row++;

	if(m_deviceInfo.bPOEDevice)
	{
		pCheck->SetEnable(false);
	}
	
	//////////////////////////////////////////////////////////////////////////
	//绑定通道
	m_rows[ROW_CHNN] = row;
	list.AddItem( row, 0, m_pStrTable->String(E_STR_ID_CHANNEL) );
	pCombo = dynamic_cast<GUI::CComboCtrl*>( list.AddItem( row , 1, WND_TYPE_COMBO ) );
	assert( pCombo );
	int comboItemNum = 0;
	int poeNum = CProduct::Instance()->PoeInputNum();
	for ( int i = g_p_login_succ->productInfo.localVideoInputNum; i < g_p_login_succ->productInfo.videoInputNum; i++ )
	{
		if((m_deviceInfo.bPOEDevice) && (i >= (g_p_login_succ->productInfo.localVideoInputNum+poeNum)))
		{
			break;
		}

		int j = 0;
		for (j = 0; j < m_deviceCount; j++)
		{
			if((m_pDeviceInfo[j].channel == i) && (m_deviceInfo.channel != i))
			{
				break;
			}
		}

		if(j == m_deviceCount)
		{
			sprintf( str, "%d", i + 1 /*- g_p_login_succ->productInfo.localVideoInputNum*/ );
			pCombo->AddItem( str, i );
			comboItemNum++;
		}
	}
	
	if(comboItemNum == 0)
	{
		pCheck->SetEnable(false);
	}

	if((m_dlgType == DEVICE_MODIFY) && (invalid != m_deviceInfo.channel))
	{
		pCombo->SetCurItemData(m_deviceInfo.channel, false);
	}
	else
	{
		pCombo->SetCurItem(0, false);
	}

	row++;

	//////////////////////////////////////////////////////////////////////////
	//设备厂商
	m_rows[ROW_MANUFACTURER] = row;
	list.AddItem( row, 0,  m_pStrTable->String(E_STR_ID_IPCAM_MANUFACTURER) );

	if(!GetManufacturerVar())
	{
		list.AddItem( row, 1, m_deviceInfo.manufacturerName);
	}
	else
	{
		pCombo = dynamic_cast<GUI::CComboCtrl*>( list.AddItem( row, 1, WND_TYPE_COMBO ) );
		assert( pCombo );
		pCombo->SetMaxStringLen( 36 );
		
		//请求NVR支持的IPCAM厂家
		MESSAGE_DATA msgData;
		CMessageMan::Instance()->SendMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_SUPPORT_DEVICE, NULL, 0, msgData);
		PACKCMD *pPackCmd = reinterpret_cast<PACKCMD *>(msgData.pData);
		if(CMD_REPLY_NET_SUPPORT_DEVICE == pPackCmd->cmdType)
		{
			unsigned long count =*reinterpret_cast<unsigned long*>( msgData.pData + sizeof(PACKCMD) );
			NVR_SUPPORT_MANUFACTURER *pSupportManuf = reinterpret_cast<NVR_SUPPORT_MANUFACTURER *>(msgData.pData+sizeof(PACKCMD)+sizeof(unsigned long));

			if(!m_vecManufacturer.empty())
			{
				m_vecManufacturer.clear();
			}
			for(unsigned long i = 0; i < count; i++)
			{
				m_vecManufacturer.push_back(pSupportManuf[i]);
			}
		}
		CMessageMan::Instance()->ReleaseMsgBuff(msgData);

		for(int m = 0; m < m_vecManufacturer.size(); m++)
		{
			pCombo->AddItem(m_vecManufacturer[m].manufacturerName, m_vecManufacturer[m].manufacturerId);
		}

		if(!m_vecManufacturer.empty())
		{
			pCombo->SetCurItem( 0 , false );
			iSelManufacturer = 0;

			if(m_dlgType == DEVICE_MODIFY)
			{
				for(int m = 0; m < m_vecManufacturer.size(); m++)
				{
					if(m_vecManufacturer[m].manufacturerId == m_deviceInfo.manufacturerId)
					{
						pCombo->SetCurItem( m , false );
						iSelManufacturer = m;
						break;
					}
				}
	//			if(!m_deviceInfo.bPOEDevice)
				{
					pCombo->SetEnable(false);
				}
			}
			else
			{
				for (int m = 0; m < m_vecManufacturer.size(); m++)
				{
					if (m_vecManufacturer[m].manufacturerId == m_deviceInfo.manufacturerId)
					{
						pCombo->SetCurItem( m , false );
						iSelManufacturer = m;
						break;
					}
				}
			}
		}
	}
	row++;
	
	//////////////////////////////////////////////////////////////////////////
	//设备IP
	m_rows[ROW_IP] = row;
	list.AddItem( row, 0, m_pStrTable->String(E_STR_ID_IP_ADDRESS) );
	pEdit = dynamic_cast<GUI::CEditCtrl*>( list.AddItem(row, 1, WND_TYPE_EDIT) );
	assert( pEdit );
	pEdit->SetMaxStringLen( sizeof(m_deviceInfo.szServer) );
	pEdit->SetCaption( m_deviceInfo.szServer, false );
	row++;

	if(m_deviceInfo.bPOEDevice)
	{
		pEdit->SetEnable(false);
	}

	//////////////////////////////////////////////////////////////////////////
	//设备数据端口
	m_rows[ROW_PORT] = row;
	list.AddItem( row, 0, m_pStrTable->String(E_STR_ID_SERVER_PORT) );
	pNumEdit = dynamic_cast<GUI::CNumEditCtrl*>( list.AddItem( row, 1, WND_TYPE_NUM_EDIT ) );
	assert( pNumEdit );
	pNumEdit->SetRange( 1, 65535 );
	pNumEdit->SetValue( m_deviceInfo.nPort, false );
	row++;

	if(m_deviceInfo.bPOEDevice)
	{
		pNumEdit->SetEnable(false);
	}

	//////////////////////////////////////////////////////////////////////////
	//HTTP端口
	m_rows[ROW_HTTP_PORT] = row;
	list.AddItem( row, 0, m_pStrTable->String(E_STR_ID_HTTP_PORT) );
	pNumEdit = dynamic_cast<GUI::CNumEditCtrl*>( list.AddItem( row, 1, WND_TYPE_NUM_EDIT ) );
	assert( pNumEdit );
	pNumEdit->SetRange( 1, 65535 );
	pNumEdit->SetValue( m_deviceInfo.nHttpPort, false );
	row++;

	//////////////////////////////////////////////////////////////////////////
	//控制端口
	m_rows[ROW_CTRL_PORT] = row;
	list.AddItem( row, 0, m_pStrTable->String(E_STR_ID_IPCAM_CONTROL_PORT) );
	pNumEdit = dynamic_cast<GUI::CNumEditCtrl*>( list.AddItem( row, 1, WND_TYPE_NUM_EDIT ) );
	assert( pNumEdit );
	pNumEdit->SetRange( 1, 65535 );
	pNumEdit->SetValue( m_deviceInfo.nCtrlPort, false );
	row++;

	//////////////////////////////////////////////////////////////////////////
	//用户名
	m_rows[ROW_USERNAME] = row;
	list.AddItem( row, 0, m_pStrTable->String(E_STR_ID_USER_NAME) );
	pEdit = dynamic_cast<GUI::CEditCtrl*>( list.AddItem( row, 1, WND_TYPE_EDIT ) );
	assert( pEdit );
	pEdit->SetMaxStringLen( 36 );
	pEdit->SetCaption( m_deviceInfo.username, false );
	row++;

	if((m_deviceInfo.bPOEDevice) && (MID_TVT == m_deviceInfo.manufacturerId))
	{
		//同为自己的POE设备，使用私有协议，不能修改用户名和密码
		//非同为的POE设备，由于使用ONVIF协议，需要修改用户名和密码
		pEdit->SetEnable(false);
	}

	//////////////////////////////////////////////////////////////////////////
	//用户密码
	m_rows[ROW_PASSWORD] = row;
	list.AddItem( row, 0, m_pStrTable->String(E_STR_ID_PASSWORD) );
	pEdit = dynamic_cast<GUI::CEditCtrl*>( list.AddItem( row, 1, WND_TYPE_EDIT ) );
	assert( pEdit );
	pEdit->SetMaxStringLen( 36 );
	pEdit->SetCaption( m_deviceInfo.password, false );
	pEdit->SetPassword( true );

	if((m_deviceInfo.bPOEDevice) && (MID_TVT == m_deviceInfo.manufacturerId))
	{
		pEdit->SetEnable(false);
	}

	list.CreateListFocusMap();

	if(m_vecManufacturer.empty())
	{
		m_listCtrl.RemoveItem(m_rows[ROW_CTRL_PORT], true);
		m_listCtrl.RemoveItem(m_rows[ROW_HTTP_PORT], true);
	}
	else
	{
		if(!m_vecManufacturer[iSelManufacturer].bNeedHttpPort)
		{
			m_listCtrl.RemoveItem(m_rows[ROW_HTTP_PORT], true);
		}
		if(!m_vecManufacturer[iSelManufacturer].bNeedCtrlPort)
		{
			m_listCtrl.RemoveItem(m_rows[ROW_CTRL_PORT], true);
		}
	}

	//
	DoCheckUI( false );
}

unsigned long CDeviceInfoDlg::OnOK()
{
	DLG_ERROR err = DoCheckValue();
	if( DEVICE_INFO_OK == err )
	{
		DoGetValue( );
		return CDialog::OnOK();
	}
	else
	{
		DoMessageError( err );
	}

	return KEY_VIRTUAL_MSG_STOP; 
}

void CDeviceInfoDlg::SetDlgType( DLG_TYPE type /*= DEVICE_ADD*/ )
{
	m_dlgType = type;
}

void CDeviceInfoDlg::SetDeviceInfo( const IP_DEVICE_INFO& deviceInfo )
{
	m_deviceInfo = deviceInfo;
}

void CDeviceInfoDlg::GetDeviceInfo( IP_DEVICE_INFO& deviceInfo )
{
	deviceInfo = m_deviceInfo;
}

void CDeviceInfoDlg::SetAllDeviceInfo(IP_DEVICE_INFO *pNetLinkInfo, long count)
{
	m_pDeviceInfo = pNetLinkInfo;
	m_deviceCount = count;
}

CDeviceInfoDlg::DLG_ERROR CDeviceInfoDlg::DoCheckValue()
{
	GUI::CListCtrl& list = m_listCtrl;

// 	if(m_rows[ROW_MODEL] != -1)
// 	{
// 		GUI::CComboCtrl *pCombo = dynamic_cast<GUI::CComboCtrl*>( list.GetItemWnd(m_rows[ROW_MODEL],1) );
// 		assert( pCombo );
// 		if(pCombo->GetCurItem() < 0)
// 		{
// 			return DEVICE_INFO_PRODUCT;
// 		}
// 	}

	GUI::CEditCtrl* pIP = dynamic_cast<GUI::CEditCtrl*>( list.GetItemWnd(m_rows[ROW_IP],1) );
	assert( pIP );
	if(pIP->GetCaption() == "")
	{
		return DEVICE_INFO_IP;
	}

	GUI::CEditCtrl* pEdit = dynamic_cast<GUI::CEditCtrl*>( list.GetItemWnd(m_rows[ROW_USERNAME],1) );
	assert( pEdit );
	if(pEdit->GetCaption() == "")
	{
		return DEVICE_INFO_USER_PASS;
	}

	return DEVICE_INFO_OK;
}

void CDeviceInfoDlg::DoGetValue()
{
	GUI::CListCtrl& list = m_listCtrl;
	GUI::CComboCtrl* pCombo = NULL;

	if(m_rows[ROW_ENABLE] != -1)
	{
		GUI::CCheckCtrl* pCheck = dynamic_cast<GUI::CCheckCtrl*>( list.GetItemWnd(m_rows[ROW_ENABLE],1) );
		assert(pCheck);
		pCombo = dynamic_cast<GUI::CComboCtrl*>( list.GetItemWnd(m_rows[ROW_CHNN],1) );
		assert(pCombo);

		if( pCheck->IsCheck() )
		{
			m_deviceInfo.channel = pCombo->GetCurItemData();
		}
		else
		{
			m_deviceInfo.channel = INVALID_INDEX(unsigned char);
		}
	}

	if(GetManufacturerVar())
	{
		if(m_rows[ROW_MANUFACTURER] != -1)
		{
			pCombo = dynamic_cast<GUI::CComboCtrl*>( list.GetItemWnd(m_rows[ROW_MANUFACTURER],1) );
			assert( pCombo );
			m_deviceInfo.manufacturerId = pCombo->GetItemData(pCombo->GetCurItem());
			strcpy(m_deviceInfo.manufacturerName, pCombo->GetItemText(pCombo->GetCurItem()));
		}
	}
	
// 	if(m_rows[ROW_MODEL] != -1)
// 	{
// 		pCombo = dynamic_cast<GUI::CComboCtrl*>( list.GetItemWnd(m_rows[ROW_MODEL],1) );
// 		assert( pCombo );
// 		m_deviceInfo.productId = pCombo->GetItemData(pCombo->GetCurItem());
// 		strcpy(m_deviceInfo.productModel, pCombo->GetItemText(pCombo->GetCurItem()));
// 	}
	
	GUI::CEditCtrl* pIP = dynamic_cast<GUI::CEditCtrl*>( list.GetItemWnd(m_rows[ROW_IP],1) );
	assert( pIP );
	strcpy( m_deviceInfo.szServer, pIP->GetCaption().c_str() );

	GUI::CNumEditCtrl* pNumEdit = dynamic_cast<GUI::CNumEditCtrl*>( list.GetItemWnd(m_rows[ROW_PORT],1) );
	assert( pNumEdit );
	m_deviceInfo.nPort = pNumEdit->GetValue();

	pNumEdit = dynamic_cast<GUI::CNumEditCtrl*>( list.GetItemWnd(m_rows[ROW_HTTP_PORT],1) );
	if(pNumEdit)
	{
		m_deviceInfo.nHttpPort = pNumEdit->GetValue();
	}

	pNumEdit = dynamic_cast<GUI::CNumEditCtrl*>( list.GetItemWnd(m_rows[ROW_CTRL_PORT],1) );
	if(pNumEdit)
	{
		m_deviceInfo.nCtrlPort = pNumEdit->GetValue();
	}

	GUI::CEditCtrl* pEdit = dynamic_cast<GUI::CEditCtrl*>( list.GetItemWnd(m_rows[ROW_USERNAME],1) );
	assert( pEdit );
	strcpy( m_deviceInfo.username, pEdit->GetCaption().c_str() );

	pEdit = dynamic_cast<GUI::CEditCtrl*>( list.GetItemWnd(m_rows[ROW_PASSWORD],1) );
	assert( pEdit );
	strcpy( m_deviceInfo.password, pEdit->GetCaption().c_str() );
}

void CDeviceInfoDlg::DoMessageError( DLG_ERROR err )
{
	std::string strOut;

	switch ( err )
	{
	case DEVICE_INFO_PRODUCT:
		strOut = CStringTable::Instance()->String(E_STR_ID_IPCAM_NO_PRODUCT_SEL);
		break;
	case DEVICE_INFO_IP:
		strOut = CStringTable::Instance()->String(E_STR_ID_IPCAM_IP_INVALID);
		break;
	case DEVICE_INFO_USER_PASS:
		strOut = CStringTable::Instance()->String(E_STR_ID_IPCAM_USER_NAME_EMPTY);
		break;
	default:
		assert( false );
	}

	CDialog::MessageBox( strOut.c_str(), m_pStrTable->String(E_STR_ID_TITLE_ERROR), DEFAULT_BTN_OK | DEFAULT_ICON_ERROR);
}

void CDeviceInfoDlg::DoCheckUI( bool bRefresh )
{
	//此对话框元素之依赖、互斥关系少，简单处理
	GUI::CListCtrl& list = m_listCtrl;

	if(m_rows[ROW_ENABLE] != -1)
	{
		printf("%d, %s, %s, %d\n", m_rows[ROW_ENABLE], __FILE__, __FUNCTION__, __LINE__);
		GUI::CCheckCtrl* pCheck = dynamic_cast<GUI::CCheckCtrl*>( list.GetItemWnd(m_rows[ROW_ENABLE],1) );
		assert( pCheck );
		GUI::CComboCtrl* pCombo = dynamic_cast<GUI::CComboCtrl*>( list.GetItemWnd(m_rows[ROW_CHNN],1) );
		assert( pCombo );

		if(!m_deviceInfo.bPOEDevice)
		{
			pCombo->SetEnable( pCheck->IsCheck(), bRefresh );
		}
	}
}

bool CDeviceInfoDlg::GetMac( const std::string& str, unsigned char pMac[] ) const
{
	if( str.size() < 17 ) return false;
	char tmpStr[3] = {0};
	unsigned char result[6] = {0};

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


BEGIN_MSG_MAP(CDeviceInfoDlg, GUI::CDialog)
	ON_MSG_EX(m_listCtrl.GetID(), KEY_VIRTUAL_MSG_DEFAULT_TAG, OnClickList)
END_MSG_MAP()

unsigned long CDeviceInfoDlg::OnClickList(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	unsigned short row = (keyID >> 16)&0x7fff;	//行
	unsigned short col = keyID&0xffff;			//列

	if( m_rows[ROW_ENABLE] == row && 1 == col )
	{
		DoCheckUI( true );
	}

	if((m_rows[ROW_MANUFACTURER] == row) && 1== col)
	{
		GUI::CComboCtrl *pCombo = NULL;

		do 
		{
			//读取当前选择的设备厂商名
			pCombo = dynamic_cast<GUI::CComboCtrl*>( m_listCtrl.GetItemWnd(m_rows[ROW_MANUFACTURER],1) );
			assert( pCombo );
			if(pCombo->GetCurItem() < 0)
			{
				break;
			}

			unsigned long iSel = pCombo->GetCurItem();
			if(m_vecManufacturer.empty())
			{
				break;
			}


			if(m_vecManufacturer[iSel].bNeedHttpPort)
			{
				m_listCtrl.AddItem(m_rows[ROW_HTTP_PORT], 0, m_pStrTable->String(E_STR_ID_HTTP_PORT) );
				GUI::CNumEditCtrl* pNumEdit = dynamic_cast<GUI::CNumEditCtrl*>(m_listCtrl.AddItem(m_rows[ROW_HTTP_PORT], 1, WND_TYPE_NUM_EDIT ) );
				assert( pNumEdit );
				pNumEdit->SetRange( 1, 65535 );
				pNumEdit->SetValue( m_deviceInfo.nHttpPort, false );
			}
			else
			{
				m_listCtrl.RemoveItem(m_rows[ROW_HTTP_PORT], true);
			}

			if(m_vecManufacturer[iSel].bNeedCtrlPort)
			{
				m_listCtrl.AddItem(m_rows[ROW_CTRL_PORT], 0, m_pStrTable->String(E_STR_ID_IPCAM_CONTROL_PORT) );
				GUI::CNumEditCtrl* pNumEdit = dynamic_cast<GUI::CNumEditCtrl*>(m_listCtrl.AddItem( row, 1, WND_TYPE_NUM_EDIT ) );
				assert( pNumEdit );
				pNumEdit->SetRange( 1, 65535 );
				pNumEdit->SetValue( m_deviceInfo.nCtrlPort, false );
			}
			else
			{
				m_listCtrl.RemoveItem(m_rows[ROW_CTRL_PORT], true);
			}
		} while(0);
	}

	return KEY_VIRTUAL_MSG_STOP;
}
