/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : L
** Date         : 2009-10-07
** Name         : DeviceSearchDlg.cpp
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/

#include "DeviceSearchDlg.h"
#include "ListCtrlEx.cpp"
#include "RegNetDeviceLog.h"
#include "IPAddressCtrl.h"
#include "MessageMan.h"
#include "SetupIPCNetworkDlg.h"
#include "FuncCustom.h"

template <class T>
std::string &  GUI::CListCtrlEx<T>::Text(T &node, unsigned short col)
{
	DISCOVER_DEVICE_INFO & deviceInfo = node;
	char str[512] = {0};

	if( 0 == col )
	{
		strcpy(str, deviceInfo.manufacturerName);
	}

	if( 1== col )
	{
		strcpy(str, deviceInfo.productModel);
	}

	if( 2 == col )
	{
		strcpy(str, deviceInfo.szAddr);
	}

	if( 3 == col )
	{
		sprintf( str, "%d", deviceInfo.netport);
	}

	
	//if( 3 == col )
	//{
	//	sprintf(str, "%.2X-%.2X-%.2X-%.2X-%.2X-%.2X",deviceInfo.mac[0],deviceInfo.mac[1],deviceInfo.mac[2],
	//			deviceInfo.mac[3],deviceInfo.mac[4],deviceInfo.mac[5] );
	//}

	m_str = str;
	return m_str;
}

//此处声明CListCtrlEx窗口指针,是由于上级调用窗口也使用CListCtrlEx类,
//使得模版发生解析冲突,因此使用此变量保存:但此对象仅一例
//GUI::CListCtrlEx<DISCOVER_DEVICE_INFO>* s_pListDeviceList = NULL;
//GUI::CListCtrlEx<DISCOVER_DEVICE_INFO>* ListDeviceInstance()
//{
//	if( NULL == s_pListDeviceList )
//	{
//		s_pListDeviceList = new GUI::CListCtrlEx<DISCOVER_DEVICE_INFO>;
//	}
//
//	return s_pListDeviceList;
//}
//
//// Quit前,必须保证窗口已经被销毁
//void ListDeviceQuit()
//{
//	if( NULL != s_pListDeviceList )
//	{
//		delete s_pListDeviceList;
//		s_pListDeviceList = NULL;
//	}
//}


CDeviceSearchDlg::CDeviceSearchDlg()
{
}

CDeviceSearchDlg::~CDeviceSearchDlg()
{
	m_selDeviceVec.clear();
}

void CDeviceSearchDlg::OnInitial()
{
	GUI::CDialog::OnInitial();
	SetImage(BMP_MEM_DLG_LARGE, false);
	SetCaption(m_pStrTable->String(E_STR_ID_TITLE_ADD_DEVICE), false);

	SetDefaultBtn(DEFAULT_BTN_APPLY | DEFAULT_BTN_OK | DEFAULT_BTN_CANCEL );
	SetDefaultBtnCaption(DEFAULT_BTN_APPLY, m_pStrTable->String(E_STR_ID_REFRESH));
	//////////////////////////////////////////////////////////////////////////
	GUI::CListCtrlEx<DISCOVER_DEVICE_INFO>* pListEx = &m_listCtrlEx;

	unsigned short x = m_length[DLG_OFFSET_LEFT], y = m_length[TITLE_HEIGHT] + m_length[DLG_OFFSET_TOP];
	unsigned short width = m_rect.Width() - 2 * m_length[DLG_OFFSET_LEFT];
	unsigned short height = m_rect.Height() - y - m_length[DEFAULT_BTN_BOTTOM_YPOS];

	pListEx->Create(GUI::CWnd::GetRandWndID(), x, y, width, height, this, 0, 1, true);
	pListEx->AddColumn(m_pStrTable->String(E_STR_ID_IPCAM_MANUFACTURER), width * 6 / 32+100);
	pListEx->AddColumn(m_pStrTable->String(E_STR_ID_IPCAM_PRODUCT_MODEL), width * 6 / 32);
	pListEx->AddColumn(m_pStrTable->String(E_STR_ID_IP_ADDRESS), width * 8 / 32);
	pListEx->AddColumn(m_pStrTable->String(E_STR_ID_PORT), width * 6 / 32-100);
//	pListEx->AddColumn(m_pStrTable->String(E_STR_ID_MAC), width * 12 / 32 );
	pListEx->EnableSingleSelect();


	x = m_length[DLG_OFFSET_LEFT];
	y = m_rect.Height() - m_length[DEFAULT_BTN_BOTTOM_YPOS];
	char str[200] = {0};
	strcpy(str, m_pStrTable->String(E_STR_ID_SETUP));
	m_btnSetupIpc.Create( GUI::CWnd::GetRandWndID(), x, y, m_length[BTN_WIDTH], m_length[BTN_HEIGHT], this, 0, 2, str );
}

void CDeviceSearchDlg::OnPostInitial()
{
	CDialog::OnPostInitial();

	UpdateDataUI( false );

	m_btnSetupIpc.SetEnable(false);

	m_selDeviceVec.clear();
}

unsigned long CDeviceSearchDlg::OnOK()
{
	GUI::CListCtrlEx<DISCOVER_DEVICE_INFO>* pListEx = &m_listCtrlEx;

	m_selDeviceVec.clear();
	unsigned long count = 0;
	DISCOVER_DEVICE_INFO* pDeviceInfo = pListEx->GetSelectData( count );
	for ( unsigned long i=0; i<count; i++ )
	{
		m_selDeviceVec.push_back( pDeviceInfo[i] );
	}

	if( pDeviceInfo )
	{
		delete [] pDeviceInfo;
	}

	return GUI::CDialog::OnOK();
}

unsigned long CDeviceSearchDlg::OnApply()
{
	UpdateDataUI( true );

	return GUI::CDialog::OnApply();
}

void CDeviceSearchDlg::UpdateDataUI( bool bRefresh )
{
	GUI::CListCtrlEx<DISCOVER_DEVICE_INFO>* pListEx = &m_listCtrlEx;

	MESSAGE_DATA msgData;
	memset(&msgData, 0, sizeof(MESSAGE_DATA));
	CMessageMan::Instance()->SendMsgDataToServer(0, LOCAL_DEVICE_ID, CMD_REQUEST_NET_DEVICE_SEARCH, NULL,0, msgData);

	DISCOVER_DEVICE_INFO*  pDeviceInfo = reinterpret_cast<DISCOVER_DEVICE_INFO *>(msgData.pData + sizeof(PACKCMD) + sizeof(unsigned long));
	unsigned long count = *reinterpret_cast<unsigned long*>(msgData.pData + sizeof(PACKCMD));

	if( count > 0 )
	{
		DISCOVER_DEVICE_INFO* pNewInfo = new DISCOVER_DEVICE_INFO[count];
		memcpy( pNewInfo, pDeviceInfo, sizeof(DISCOVER_DEVICE_INFO)*count );

		pListEx->PutData( pNewInfo, count, bRefresh );
	}
	else
	{
		pListEx->PutData( NULL, 0, bRefresh );
	}

	CMessageMan::Instance()->ReleaseMsgBuff(msgData);	
}

std::vector<DISCOVER_DEVICE_INFO>& CDeviceSearchDlg::GetSelDeviceInfo()
{
	return m_selDeviceVec;
}

BEGIN_MSG_MAP(CDeviceSearchDlg, GUI::CDialog)
	ON_MSG_EX(m_listCtrlEx.GetID(), KEY_VIRTUAL_DBCLICK, OnDoubleClickList)
	ON_MSG_EX(m_listCtrlEx.GetID(), KEY_VIRTUAL_ENTER, OnClickList)
	ON_MSG(m_btnSetupIpc.GetID(), KEY_VIRTUAL_ENTER, OnEditIPCNetwork)
END_MSG_MAP()


unsigned long CDeviceSearchDlg::OnClickList(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	unsigned short row = 0;	
	row = m_listCtrlEx.GetSingleSelect();
	if ((row >= m_listCtrlEx.GetCount()) || (row < 0))
	{
		m_btnSetupIpc.SetEnable(false);
		return KEY_VIRTUAL_MSG_STOP;
	}

	DISCOVER_DEVICE_INFO * pNdDeviceInfo = m_listCtrlEx.GetItem(row);
	if (NULL == pNdDeviceInfo)
	{
		m_btnSetupIpc.SetEnable(false);
		return KEY_VIRTUAL_MSG_STOP;
	}

	if(pNdDeviceInfo->manufacturerId != MID_TVT)
	{
		m_btnSetupIpc.SetEnable(false);
		return KEY_VIRTUAL_MSG_STOP;
	}

	m_btnSetupIpc.SetEnable(true);

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CDeviceSearchDlg::OnDoubleClickList(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	return EditIPCNetwork();
}

unsigned long CDeviceSearchDlg::OnEditIPCNetwork()
{
	return EditIPCNetwork();
}

unsigned long CDeviceSearchDlg::EditIPCNetwork()
{
	unsigned short row = m_listCtrlEx.GetSingleSelect();
	if ((row >= m_listCtrlEx.GetCount()) || (row < 0))
	{
		return KEY_VIRTUAL_MSG_STOP;
	}

	DISCOVER_DEVICE_INFO * pNdDeviceInfo = m_listCtrlEx.GetItem(row);
	if (NULL == pNdDeviceInfo)
	{
		return KEY_VIRTUAL_MSG_STOP;
	}

	if(pNdDeviceInfo->manufacturerId != MID_TVT)
	{
		return KEY_VIRTUAL_MSG_STOP;
	}

	CSetupIpcNetworkDlg setupIPCNetwordDlg(*pNdDeviceInfo);
	setupIPCNetwordDlg.DoModal(this);

	OnApply();

	return KEY_VIRTUAL_MSG_STOP;
}
