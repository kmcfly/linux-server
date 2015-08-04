/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : L
** Date         : 2009-09-29
** Name         : DeviceManDlg.h
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/

#include "PUB_common.h"
#include "DeviceManDlg.h"
#include "ListCtrlExMulti.cpp"
#include "DeviceInfoDlg.h"
#include "MessageBox.h"
#include "MessageMan.h"
#include "NdLibMan.h"
#include "FuncCustom.h"


#include <vector>


#define ANSI_STRING

#ifdef __ENVIRONMENT_LINUX__
#include "NetInterface.h"
#endif

extern NET_LOGIN_SUCCESS_REPLY *g_p_login_succ;

template <class T>
std::string &  GUI::CListCtrlExMulti<T>::Text(T &node, unsigned short col)
{
	IP_DEVICE_INFO& device = node;
	char str[512] = {0};
	unsigned char invalid = INVALID_INDEX(unsigned char);

	if( 0 == col )
	{
		if( invalid != device.channel )
		{
			sprintf( str, "%d" , device.channel + 1);
		}
		else
		{
			snprintf( str, 512, "%s", m_pStrTable->String(E_STR_ID_NONE) );
		}
	}

	if( 1 == col )
	{
		strcpy(str, device.szServer);
	}

	if( 2 == col )
	{
		strcpy(str, device.manufacturerName);
	}

	if( 3 == col )
	{
		strcpy(str, device.productModel);
	}

	if ( 4 == col )
	{
		if( invalid != device.channel )
		{
//			strcpy( str, m_pStrTable->String(E_STR_ID_ENABLE));
			if(device.status == ND_STATUS_OK)
			{
				strcpy(str, m_pStrTable->String(E_STR_ID_IPCAM_CONNECTED));
			}
			else
			{
				strcpy(str, m_pStrTable->String(E_STR_ID_IPCAM_DISCONNECTED));
			}
		}
		else
		{
			strcpy( str, m_pStrTable->String(E_STR_ID_DISABLE) );
		}
	}

	m_str = str;
	return m_str;
}

//自动补设备名ID
int CDeviceManDlg::m_rndID = 1;

CDeviceManDlg::CDeviceManDlg()
{
	m_pBtnApply = NULL;
	m_pBitStreamValue = NULL;
}

CDeviceManDlg::~CDeviceManDlg()
{

}

void CDeviceManDlg::OnInitial()
{
	GUI::CDialog::OnInitial();
	m_pBitStreamValue = NULL;

	SetImage(BMP_MEM_DLG_LARGE, false);
	SetCaption(m_pStrTable->String(E_STR_ID_TITLE_DEVICE_MAN), false);

	SetDefaultBtn( DEFAULT_BTN_APPLY | DEFAULT_BTN_CANCEL );
	SetDefaultBtnCaption(DEFAULT_BTN_CANCEL, m_pStrTable->String(E_STR_ID_EXIT) );
	//////////////////////////////////////////////////////////////////////////
	GUI::CListCtrlExMulti<IP_DEVICE_INFO>& listEx = m_deviceList;
	char str[256] = {0};

	unsigned short x = m_length[DLG_OFFSET_LEFT], y = m_length[TITLE_HEIGHT] + m_length[DLG_OFFSET_TOP];
	unsigned short width = m_rect.Width() - 2 * m_length[DLG_OFFSET_LEFT];
	unsigned short height = m_rect.Height() - y - m_length[DEFAULT_BTN_BOTTOM_YPOS];

	listEx.Create(GUI::CWnd::GetRandWndID(), x, y, width, height, this, 0, 1);
	listEx.AddColumn(m_pStrTable->String(E_STR_ID_CH), 80);
	listEx.AddColumn(m_pStrTable->String(E_STR_ID_DEVICE_NET_ADDRESS), width -600 );
	listEx.AddColumn(m_pStrTable->String(E_STR_ID_IPCAM_MANUFACTURER), 275);
	listEx.AddColumn(m_pStrTable->String(E_STR_ID_IPCAM_PRODUCT_MODEL), 125);
	listEx.AddColumn(m_pStrTable->String(E_STR_ID_STATUS), 120 );

	//listEx.EnableSingleSelect();

	x = m_length[DLG_OFFSET_LEFT];
	y = m_rect.Height() - m_length[DEFAULT_BTN_BOTTOM_YPOS];
	strcpy( str, m_pStrTable->String( E_STR_ID_ADD ) );
	m_btnAdd.Create( GUI::CWnd::GetRandWndID(), x, y, m_length[BTN_WIDTH], m_length[BTN_HEIGHT], this, 0, 2, str );

	x += m_length[BTN_WIDTH] + 8;
	strcpy( str, m_pStrTable->String( E_STR_ID_SETUP) );
	m_btnModify.Create( GUI::CWnd::GetRandWndID(), x,y,m_length[BTN_WIDTH],m_length[BTN_HEIGHT],this,1,2,str );

	x += m_length[BTN_WIDTH] + 8;
	strcpy( str, m_pStrTable->String( E_STR_ID_DELETE) );
	m_btnDelete.Create( GUI::CWnd::GetRandWndID(), x,y,m_length[BTN_WIDTH],m_length[BTN_HEIGHT],this,2,2,str );

	x += m_length[BTN_WIDTH] + 8;
	strcpy( str, m_pStrTable->String( E_STR_ID_CLEAR_ALL) );
	m_btnDeleteAll.Create( GUI::CWnd::GetRandWndID(), x,y,m_length[BTN_WIDTH],m_length[BTN_HEIGHT],this,3,2,str );
	
	x += m_length[BTN_WIDTH] + 8;
	strcpy( str, m_pStrTable->String( E_STR_ID_SEARCH) );
	m_btnSearch.Create( GUI::CWnd::GetRandWndID(), x,y,m_length[BTN_WIDTH],m_length[BTN_HEIGHT],this,3,2,str );
}

void CDeviceManDlg::OnPostInitial()
{
	CDialog::OnPostInitial();

	m_maxDevice = 0;
	m_pBtnApply = dynamic_cast<GUI::CButton*>(GetCtrl(CTRLID_DEFAULT_BTN_APPLY));
	assert( m_pBtnApply );

	MESSAGE_DATA msgData;
	NCFG_ITEM_HEAD cfgItemHead[2];
	cfgItemHead[0].itemID = NCFG_ITEM_IPC_SUPPORT_ENCODE;
	cfgItemHead[1].itemID = NCFG_ITEM_MAJOR_SUPPORT_BITRATE_RANGE;
	CMessageMan::Instance()->SendMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_CFG_GET, (unsigned char *)cfgItemHead, sizeof(cfgItemHead), msgData);

	NCFG_ITEM_HEAD *pItemHead = (NCFG_ITEM_HEAD*)(msgData.pData+sizeof(PACKCMD)+sizeof(NCFG_BLOCK_HEAD));
	char *pCfgData = msgData.pData + sizeof(PACKCMD) + sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD)*2;
	memcpy(&m_ipcSupportEncode, pCfgData, sizeof(NCFG_IPC_SUPPORT_ENCODE));

	if(VIDEO_FORMAT_NTSC == g_p_login_succ->productInfo.videoFormat)
	{
// #ifndef __CHIP3520__
// 		m_ipcSupportEncode.defaChnnRate = m_ipcSupportEncode.defaChnnRate * 6 / 5;
// 		m_ipcSupportEncode.maxChnnRate = m_ipcSupportEncode.maxChnnRate * 6 / 5;
// 		m_ipcSupportEncode.maxTotalRateOnVGA = m_ipcSupportEncode.maxTotalRateOnVGA * 6 / 5;
// 		m_ipcSupportEncode.maxChnnRateOnVGA = m_ipcSupportEncode.maxChnnRateOnVGA * 6 / 5;
// #endif
	}

	pCfgData += pItemHead[0].len;
	
	m_bitStreamCount = pItemHead[1].len/sizeof(unsigned long) - 1;
	assert(m_bitStreamCount > 0);
	m_pBitStreamValue = new unsigned long [m_bitStreamCount];
	memcpy(m_pBitStreamValue, pCfgData, sizeof(unsigned long)*m_bitStreamCount);	

	CMessageMan::Instance()->ReleaseMsgBuff(msgData);

	////
	UpdateDataUI( false );

	if(m_deviceList.GetCount() >= g_p_login_succ->productInfo.netVideoInputNum)
	{
		m_btnAdd.SetEnable(false);
	}
	else
	{
		m_btnAdd.SetEnable(true);
	}	
}


unsigned long CDeviceManDlg::OnApply()
{
	if( GetModify() )
	{
		if(!SaveData())
		{
			return KEY_VIRTUAL_MSG_CONTINUE;
		}
		m_deviceList.ClearMultiSelectList();
		UpdateDataUI( true );
		
		CNetDeviceManager::Instance()->RefreshDisplay();
	}

	return KEY_VIRTUAL_MSG_STOP;
}

void CDeviceManDlg::EndDialog(DLG_END_VALUE result)
{
	if( GetModify() )
	{
		Show( false, true );
		GUI::CMessageBox box( m_pStrTable->String( E_STR_ID_MESSAGE_38),	m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK | DEFAULT_BTN_CANCEL | DEFAULT_ICON_QUESTION);
		if( DLG_OK == box.DoModal(this) )
		{
			SaveData();
		}
	}

	if(NULL != m_pBitStreamValue)
	{
		delete [] m_pBitStreamValue;
		m_pBitStreamValue = NULL;
	}
	m_deviceList.ClearMultiSelectList();
	GUI::CDialog::EndDialog( result );
}

bool CDeviceManDlg::DoAdd( const IP_DEVICE_INFO& deviceInfo, bool bRefresh )
{
	//如果已经有此设备(比较mac地址) 或最大上限 返回false
	if( m_maxDevice < (m_deviceList.GetCount()+1) )
	{
		return false;
	}

	if(m_deviceList.GetCount() >= m_maxDevice)
	{
		m_btnAdd.SetEnable(false);
	}

	//const IP_DEVICE_INFO* pNetLinkInfo = m_deviceList.GetListData();
	//for ( int i=0; i<m_deviceList.GetCount(); i++ )
	//{
		//if( (pNetLinkInfo[i].linkInfo.IP == deviceInfo.linkInfo.IP) \
		//	&& (pNetLinkInfo[i].linkInfo.nPort == deviceInfo.linkInfo.nPort) )
		//{
		//	return false;
		//}
	//}

	m_deviceList.AddItem( deviceInfo, false );
	//m_deviceList.SetSingleSelect( m_deviceList.GetCount() -1,  bRefresh );
	m_deviceList.Repaint();
	return true;
}

bool CDeviceManDlg::DoModify( const IP_DEVICE_INFO& deviceInfo, long index)
{
	//long sel = m_deviceList.GetSingleSelect();

	if( /*sel*/index >= 0 )
	{
		//修改
		IP_DEVICE_INFO* pDeviceInfo = m_deviceList.GetItem( /*sel*/index );	//返回值属于引用方式
		if( 0 == memcmp(pDeviceInfo, &deviceInfo, sizeof(IP_DEVICE_INFO) ) )
		{
			return false; //没有做什么修改
		}

		bool bUseDefaultCfg = false;
		if(0 != strcmp(pDeviceInfo->szServer, deviceInfo.szServer) \
			|| (pDeviceInfo->nCtrlPort != deviceInfo.nCtrlPort) \
			|| (pDeviceInfo->channel != deviceInfo.channel)
			|| (strcmp(pDeviceInfo->username, deviceInfo.username))
			|| (memcmp(pDeviceInfo->password, deviceInfo.password, sizeof(pDeviceInfo->password))))
		{
			if(!pDeviceInfo->bPOEDevice)
			{
				bUseDefaultCfg = true;
			}
		}
		*pDeviceInfo = deviceInfo;
		pDeviceInfo->status = ND_STATUS_DISCONNECT;
		pDeviceInfo->bUseDefaultCfg = bUseDefaultCfg;

		m_deviceList.Repaint();
	}
	else
	{
		assert( false );
		return false;
	}

	return true;
}

bool CDeviceManDlg::DoDelete( const long oldSel  )
{
	IP_DEVICE_INFO* pDataInfo = m_deviceList.GetListData();
	int count = m_deviceList.GetCount();
	if( oldSel < 0 || count < 1 || oldSel >=count ) 
	{
		assert( false );
		//return false;
	}

	//GUI::CListCtrlEx接管了内存块,不需要另外处理
	IP_DEVICE_INFO* pNewDevice = new IP_DEVICE_INFO[count -1];

	int i = 0, j = 0;
	//int newSel = -1;
	while( j < count )
	{
		if( j != oldSel )
		{
			pNewDevice[i] = pDataInfo[j];
		}
		else
		{
			j += 1;
			//newSel = i - 1;		//新选择的，位于删除项的上一项
			continue;
		}

		i +=1;
		j += 1;
	};

	m_deviceList.PutData( pNewDevice, count -1, false );

	/*if( newSel < 0 ) newSel = 0;
	if( newSel >= 0 && m_deviceList.GetCount() > newSel )
	{
		m_deviceList.SetSingleSelect( newSel, false );

		if(pNewDevice[newSel].bPOEDevice)
		{
			m_btnDelete.SetEnable(false);
			m_btnModify.SetEnable(false);
		}
	}*/

	m_deviceList.SetSingleSelect( -1, false );

	m_btnDelete.SetEnable(false);
	m_btnModify.SetEnable(false);

	m_btnAdd.SetEnable(true);

	m_deviceList.Repaint();

	return true;
}

void CDeviceManDlg::SetModify( bool modify /*= true*/ )
{
	if( m_bModify != modify && m_pBtnApply )
	{
		m_pBtnApply->SetEnable( modify, true );
	}

	m_bModify = modify;
}

bool CDeviceManDlg::GetModify()
{
	return m_bModify;
}

void CDeviceManDlg::UpdateDataUI( bool bRefresh )
{
	m_bModify = false;

	// max device
	{
		MESSAGE_DATA msgData;
		memset(&msgData, 0, sizeof(MESSAGE_DATA));
		CMessageMan::Instance()->SendMsgDataToServer(0, LOCAL_DEVICE_ID, CMD_REQUEST_NET_DEVICE_LOG_MAX, NULL,0, msgData);

		//m_maxDevice = *((unsigned long *)(msgData.pData + sizeof(PACKCMD) ));
		m_maxDevice = g_p_login_succ->productInfo.netVideoInputNum;
		CMessageMan::Instance()->ReleaseMsgBuff(msgData);	
	}

	// device data
	{
		MESSAGE_DATA msgData;
		memset(&msgData, 0, sizeof(MESSAGE_DATA));
		CMessageMan::Instance()->SendMsgDataToServer(0, LOCAL_DEVICE_ID, CMD_REQUEST_NET_DEVICE_LOG_GET, NULL,0, msgData);
		PACKCMD *pCmd = reinterpret_cast<PACKCMD *>(msgData.pData);

		if( CMD_REPLY_NET_DEVICE_LOG_GET_SUCC == (NET_PROTOCOL)pCmd->cmdType )
		{
			IP_DEVICE_INFO*  pNetLinkInfo = reinterpret_cast<IP_DEVICE_INFO *>(msgData.pData + sizeof(PACKCMD) + sizeof(unsigned long));
			unsigned long count =*reinterpret_cast<unsigned long*>( msgData.pData + sizeof(PACKCMD) );
			
			assert( count <=  m_maxDevice );
			count = ( count > m_maxDevice ) ? m_maxDevice : count;
			
			if( count > 0 )
			{
				IP_DEVICE_INFO* pNewLinkInfo = new IP_DEVICE_INFO[count];
				memcpy( pNewLinkInfo, pNetLinkInfo, sizeof(IP_DEVICE_INFO) * count );
				m_deviceList.PutData( pNewLinkInfo, count, false);
			}
			else
			{
				m_deviceList.PutData( NULL, 0, false);
			}
		}
		else
		{
			m_deviceList.PutData( NULL, 0, false);
		}

		CMessageMan::Instance()->ReleaseMsgBuff(msgData);	
	}

	if( m_pBtnApply )
	{
		m_pBtnApply->SetEnable( m_bModify, bRefresh );
	}

	if( bRefresh )
	{
		m_deviceList.Repaint();
	}
}

bool CDeviceManDlg::SaveData()
{
	IP_DEVICE_INFO* pDeviceInfo = m_deviceList.GetListData();
	unsigned long count = m_deviceList.GetCount();
	assert( count <=  m_maxDevice );
	count = ( count > m_maxDevice ) ? m_maxDevice : count;

	unsigned long len = count * sizeof( IP_DEVICE_INFO ) + sizeof(unsigned long);
	unsigned char* pData = new unsigned char[len];
	*reinterpret_cast<unsigned long*>(pData) = count;
	IP_DEVICE_INFO* pSaveDevice = reinterpret_cast<IP_DEVICE_INFO*>( pData + sizeof(unsigned long) );
	memcpy( pSaveDevice, pDeviceInfo, sizeof(IP_DEVICE_INFO)*count );

	MESSAGE_DATA msgData;
	memset(&msgData, 0, sizeof(MESSAGE_DATA));

	NCFG_ITEM_HEAD cfgItemHead;
	cfgItemHead.itemID = NCFG_ITEM_ENCODE_MAJOR_EX;
	CMessageMan::Instance()->SendMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_CFG_GET, (unsigned char *)&cfgItemHead, sizeof(NCFG_ITEM_HEAD), msgData);

	m_ipcFrameEncode.Initial(g_p_login_succ->productInfo.netVideoInputNum, \
		m_ipcSupportEncode.maxTotalBitRate, m_ipcSupportEncode.maxTotalRateOnVGA, \
		m_pBitStreamValue, m_bitStreamCount, m_ipcSupportEncode.defaResolution, \
		m_ipcSupportEncode.defaChnnRate, m_ipcSupportEncode.defaBitRate);

	ENCODE_INFO_EX *pEncodeInfo = (ENCODE_INFO_EX *)(msgData.pData + sizeof(PACKCMD) + sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD));

	bool bOverload = false;
	unsigned short localVideoNum = g_p_login_succ->productInfo.localVideoInputNum;

	//把所有添加的通道的编码配置加入到编码参数调配结构中，有可能已经超出了总的编码限制，但这里不处理
	for(unsigned long index = 0; index < count; index++)
	{
		if(pSaveDevice[index].channel != INVALID_INDEX(unsigned char))
		{
			if(pSaveDevice[index].bUseDefaultCfg)
			{
				m_ipcFrameEncode.AddEncode(pSaveDevice[index].channel-localVideoNum, \
					m_ipcSupportEncode.defaChnnRate, \
					static_cast<VIDEO_SIZE>(m_ipcSupportEncode.defaResolution), \
					m_ipcSupportEncode.defaBitRate);
			}
			else
			{
				m_ipcFrameEncode.AddEncode(pSaveDevice[index].channel-localVideoNum, \
					pEncodeInfo[pSaveDevice[index].channel].rate, \
					static_cast<VIDEO_SIZE>(pEncodeInfo[pSaveDevice[index].channel].resolution), \
					pEncodeInfo[pSaveDevice[index].channel].hBitStream);
			}
			
		}
	}

	//在以上基础上，修改其中第一个更改或添加的设备的编码参数，如果超出限制，则整体超出
	unsigned char frate;
	VIDEO_SIZE videoSize;
	for(unsigned long index = 0; index < count; index++)
	{
		if(pSaveDevice[index].channel != INVALID_INDEX(unsigned char))
		{
			if(pSaveDevice[index].bUseDefaultCfg)
			{
				m_ipcFrameEncode.ModifyEncode(pSaveDevice[index].channel-localVideoNum, \
					0, DVRVIDEO_SIZE_VGA, 0);

				frate = m_ipcSupportEncode.defaChnnRate;
				videoSize = static_cast<VIDEO_SIZE>(m_ipcSupportEncode.defaResolution);
				bOverload = !m_ipcFrameEncode.TryEncode(pSaveDevice[index].channel-localVideoNum, \
					frate, videoSize, \
					m_ipcSupportEncode.defaBitRate);
				break;
			}
		}
	}
	CMessageMan::Instance()->ReleaseMsgBuff(msgData);

	m_ipcFrameEncode.Quit();

	if(bOverload)
	{
		MessageBox(m_pStrTable->String(E_STR_ID_MESSAGE_141), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK);
		delete [] pData;
		return false;
	}

	memset(&msgData, 0, sizeof(MESSAGE_DATA));
	CMessageMan::Instance()->SendMsgDataToServer(0, LOCAL_DEVICE_ID, CMD_REQUEST_NET_DEVICE_LOG_SET, pData, sizeof(IP_DEVICE_INFO)*count + sizeof(unsigned long), msgData);
	
	PACKCMD *pCmd = reinterpret_cast<PACKCMD *>(msgData.pData);

	if( CMD_REPLY_NET_DEVICE_LOG_SET_FAIL == (NET_PROTOCOL)pCmd->cmdType )
	{
		//这里等待返回结果，是因后续需要重新请求
	}

	delete [] pData;
	CMessageMan::Instance()->ReleaseMsgBuff(msgData);

	return true;

}

int CDeviceManDlg::GetCurrentValidChnNum(bool bPOEDev, int channel )
{
	IP_DEVICE_INFO *pDeviceInfo = m_deviceList.GetListData();
	long count = m_deviceList.GetCount();
	int poeNum = CProduct::Instance()->PoeInputNum();
	for ( int i = g_p_login_succ->productInfo.localVideoInputNum; i < g_p_login_succ->productInfo.videoInputNum; i++ )
	{
		if((bPOEDev) && (i >= (g_p_login_succ->productInfo.localVideoInputNum+poeNum)))
		{
			break;
		}

		int j = 0;
		for (j = 0; j < count; j++)
		{
			if((pDeviceInfo[j].channel == i) && (channel != i))
			{
				break;
			}
		}

		if(j == count)
		{
			return i;
		}
	}
	
	return INVALID_INDEX(unsigned char);
}

BEGIN_MSG_MAP(CDeviceManDlg, GUI::CDialog)
	ON_MSG( m_btnAdd.GetID(), KEY_VIRTUAL_ENTER, OnAdd )
	ON_MSG( m_btnModify.GetID(), KEY_VIRTUAL_ENTER, OnModify )
	ON_MSG( m_btnDelete.GetID(), KEY_VIRTUAL_ENTER, OnDelete )
	ON_MSG( m_btnSearch.GetID(), KEY_VIRTUAL_ENTER, OnSearch )
	ON_MSG(m_btnDeleteAll.GetID(), KEY_VIRTUAL_ENTER, OnDeleteAll)
	ON_MSG_EX(m_deviceList.GetID(), KEY_VIRTUAL_ENTER, OnClickList)
END_MSG_MAP()


unsigned long CDeviceManDlg::OnAdd()
{
	ULONGLONG auth = CUserMan::Instance()->CheckAuthority(AUTH_NET_DEVICE_MAN);
	if (0 == auth)
	{
		MessageBox(m_pStrTable->String(E_STR_ID_MESSAGE_80), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK | DEFAULT_ICON_WARNING);
	}
	else
	{
		IP_DEVICE_INFO *pDeviceInfo = m_deviceList.GetListData();
		long count = m_deviceList.GetCount();

		IP_DEVICE_INFO deviceInfo;
		CDeviceInfoDlg dlg;
		dlg.SetDlgType( CDeviceInfoDlg::DEVICE_ADD );
		dlg.SetAllDeviceInfo(pDeviceInfo, count);
		dlg.SetManufacturerVar(true);
		if ( DLG_OK == dlg.DoModal(this) )
		{
			dlg.GetDeviceInfo(deviceInfo);
			deviceInfo.deviceID = INVALID_INDEX(unsigned long);
			deviceInfo.status = ND_STATUS_DISCONNECT;
#if 1
			deviceInfo.bUseDefaultCfg = true;
#endif

			if( DoAdd(deviceInfo, true) )
			{
				SetModify( true );
			}
		}
	}

	if (m_maxDevice < (m_deviceList.GetCount() + 1))
	{
		m_btnAdd.SetEnable(false);
	}

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CDeviceManDlg::OnModify()
{
	IP_DEVICE_INFO deviceInfo;
	memset( &deviceInfo, 0, sizeof(IP_DEVICE_INFO) );

	//long sel = m_deviceList.GetSingleSelect();
	std::vector<unsigned long> tmp_multiSelect = m_deviceList.GetMultiSelect();
	if( /*sel >= 0*/tmp_multiSelect.size()  > 0 )
	{
		ULONGLONG auth = CUserMan::Instance()->CheckAuthority(AUTH_NET_DEVICE_MAN);
		if (0 == auth)
		{
			MessageBox(m_pStrTable->String(E_STR_ID_MESSAGE_80), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK | DEFAULT_ICON_WARNING);
			return KEY_VIRTUAL_MSG_STOP;
		}

		//deviceInfo = *m_deviceList.GetItem(sel);
		deviceInfo = *m_deviceList.GetItem(tmp_multiSelect[0]);
	}
	else
	{
		CDialog::MessageBox( m_pStrTable->String(E_STR_ID_MESSAGE_52), m_pStrTable->String(E_STR_ID_TITLE_ERROR), DEFAULT_BTN_OK | DEFAULT_ICON_ERROR);
		return KEY_VIRTUAL_MSG_STOP;
	}

	IP_DEVICE_INFO *pDeviceInfo = m_deviceList.GetListData();
	long count = m_deviceList.GetCount();

	CDeviceInfoDlg dlg;
	dlg.SetDlgType( CDeviceInfoDlg::DEVICE_MODIFY );
	dlg.SetDeviceInfo( deviceInfo );
	dlg.SetAllDeviceInfo(pDeviceInfo, count);
	dlg.SetManufacturerVar(false);
	if ( DLG_OK == dlg.DoModal(this) )
	{
		dlg.GetDeviceInfo( deviceInfo );
		if( DoModify(deviceInfo, tmp_multiSelect[0]) )
		{
			SetModify( true );
		}
	}
	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CDeviceManDlg::OnDelete()
{
	std::vector<unsigned long> tmp_multiSelect = m_deviceList.GetMultiSelect();
	if(tmp_multiSelect.size() > 0)
	{
		ULONGLONG auth = CUserMan::Instance()->CheckAuthority(AUTH_NET_DEVICE_MAN);
		if (0 == auth)
		{
			MessageBox(m_pStrTable->String(E_STR_ID_MESSAGE_80), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK | DEFAULT_ICON_WARNING);
			return KEY_VIRTUAL_MSG_STOP;
		}

		if( DLG_OK != CDialog::MessageBox( m_pStrTable->String( E_STR_ID_MESSAGE_53 ), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK | DEFAULT_BTN_CANCEL | DEFAULT_ICON_QUESTION) )
		{
			return KEY_VIRTUAL_MSG_STOP;
		}
	}
	else
	{
		CDialog::MessageBox( m_pStrTable->String( E_STR_ID_MESSAGE_52 ), m_pStrTable->String(E_STR_ID_TITLE_ERROR), DEFAULT_BTN_OK | DEFAULT_ICON_ERROR);
		return KEY_VIRTUAL_MSG_STOP;
	}

		IP_DEVICE_INFO* pDataInfo = m_deviceList.GetListData();
		for (int i=static_cast< std::vector<unsigned long> >(m_deviceList.GetMultiSelect()).size() ; i > 0; i--)
		{
			if(!pDataInfo[m_deviceList.m_multiSelectList[i-1]].bPOEDevice)
			{
				m_deviceList.DelItem(m_deviceList.m_multiSelectList[i-1], false);
			}
		}
		m_deviceList.ClearMultiSelectList();
		m_btnAdd.SetEnable(true);
		m_btnDelete.SetEnable(false);
		m_deviceList.Repaint();
		SetModify( true );

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CDeviceManDlg::OnSearch()
{
	if( m_maxDevice < (m_deviceList.GetCount() + 1) )
	{
		GUI::CDialog::MessageBox(m_pStrTable->String(E_STR_ID_MESSAGE_55),m_pStrTable->String(E_STR_ID_TITLE_ERROR),DEFAULT_BTN_OK | DEFAULT_ICON_ERROR);
		return KEY_VIRTUAL_MSG_STOP;
	}

	std::vector<DISCOVER_DEVICE_INFO> deviceInfoVec;
	if( DLG_OK == PopupDeviceSearchDlg(this,deviceInfoVec) )
	{
		ULONGLONG auth = CUserMan::Instance()->CheckAuthority(AUTH_NET_DEVICE_MAN);
		if ((0 < deviceInfoVec.size()) && (0 == auth))
		{
			MessageBox(m_pStrTable->String(E_STR_ID_MESSAGE_80), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK | DEFAULT_ICON_WARNING);
			return KEY_VIRTUAL_MSG_STOP;
		}

		MESSAGE_DATA msgData;
		NCFG_ITEM_HEAD cfgItemHead;
		cfgItemHead.itemID = NCFG_ITEM_USER;
		CMessageMan::Instance()->SendMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_CFG_GET, (unsigned char *)&cfgItemHead, sizeof(cfgItemHead), msgData);
		NCFG_INFO_USER *pUserInfo = (NCFG_INFO_USER *)(msgData.pData + sizeof(PACKCMD) + sizeof(NCFG_BLOCK_HEAD) + sizeof(NCFG_ITEM_HEAD));

		std::vector<DISCOVER_DEVICE_INFO>::iterator it;
		for( it=deviceInfoVec.begin(); it != deviceInfoVec.end(); it++ )
		{
			IP_DEVICE_INFO netLinkInfo;
			unsigned int manu_id_val;
			char prod_number[16];
			int xlen;
			bool bValidNum;

			memset( &netLinkInfo, 0, sizeof(IP_DEVICE_INFO) );

			netLinkInfo.manufacturerId = it->manufacturerId;
			strcpy(netLinkInfo.manufacturerName, it->manufacturerName);
			strcpy(netLinkInfo.productModel, it->productModel);
			netLinkInfo.nPort = it->netport;
			netLinkInfo.nHttpPort = it->nHttpPort;
			netLinkInfo.nCtrlPort = it->nCtrlPort;
			strcpy(netLinkInfo.szServer, it->szAddr);
			strcpy(netLinkInfo.szID, it->szID);

			if(MID_TVT == netLinkInfo.manufacturerId)
			{
#if defined(__CUSTOM_PL14__)
				strcpy(netLinkInfo.username, "root");
				strcpy(netLinkInfo.password, "pass");
#elif defined(__CUSTOM_CNB__)
				strcpy(netLinkInfo.username, pUserInfo[0].name);
				strcpy(netLinkInfo.password, "admin");
#else
				strcpy(netLinkInfo.username, pUserInfo[0].name);
				strcpy(netLinkInfo.password, "123456");
#endif
			}
			else
			{
				manu_id_val = it->manufacturerName[35];
				memset( prod_number, 0, 16 );
				xlen = 0; //strlen( it->szID );
				if( xlen >= 8 )
				{
					strncpy( prod_number, it->szID + (xlen-8), 8 );
					if( 'S' == prod_number[0] && 'Z' == prod_number[1]
						&& 'S' == prod_number[6] && 'Z' == prod_number[7] )
					{
						bValidNum = false;
						for( xlen = 2; xlen < 6; xlen++ )
						{
							if( prod_number[xlen] < '0'
								|| prod_number[xlen] > '9' )
							{
								bValidNum = true;
								break;
							}
						}

						if( !bValidNum )
						{
							prod_number[6] = '\0';
							prod_number[7] = '\0';
							manu_id_val = atoi(prod_number+2);
						}
					}
				}

				if( MANU_ONVIF_HIKVISION == manu_id_val )
				{
					strcpy( netLinkInfo.username, "admin" );
					strcpy( netLinkInfo.password, "12345" );
				}
				else if( MANU_ONVIF_DAHUA == manu_id_val )
				{
					strcpy( netLinkInfo.username, "admin" );
					strcpy( netLinkInfo.password, "admin" );
				}
				else if( MANU_ONVIF_XIONGMAI == manu_id_val )
				{
					strcpy( netLinkInfo.username, "admin" );
					strcpy( netLinkInfo.password, "123456" );
				}
				else if( MANU_ONVIF_ZNV == manu_id_val )
				{
					strcpy( netLinkInfo.username, "admin" );
					strcpy( netLinkInfo.password, "12345" );
				}
				else if( MANU_ONVIF_TIANSHITONG == manu_id_val )
				{
					strcpy( netLinkInfo.username, "admin" );
					strcpy( netLinkInfo.password, "12345" );
				}
				else if( MANU_ONVIF_ZHONGWEI == manu_id_val )
				{
					strcpy( netLinkInfo.username, "admin" );
					strcpy( netLinkInfo.password, "" );
				}
			}

			if(!netLinkInfo.bPOEDevice)
			{
				netLinkInfo.bUseDefaultCfg = true;
			}
			netLinkInfo.channel = GetCurrentValidChnNum(netLinkInfo.bPOEDevice,INVALID_INDEX(unsigned char));
			netLinkInfo.deviceID = INVALID_INDEX(unsigned long);
			netLinkInfo.status = ND_STATUS_DISCONNECT;

			if( DoAdd( netLinkInfo, false ) )
			{
				SetModify( true );
			}
		}

		CMessageMan::Instance()->ReleaseMsgBuff(msgData);

		m_deviceList.Repaint();

		if (m_maxDevice < (m_deviceList.GetCount() + 1))
		{
			m_btnAdd.SetEnable(false);
		}
	}

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CDeviceManDlg::OnDeleteAll()
{
	int iCount = m_deviceList.GetCount();
	
	if (0 >= iCount)
	{
		return KEY_VIRTUAL_MSG_STOP;
	}
	
	ULONGLONG auth = CUserMan::Instance()->CheckAuthority(AUTH_NET_DEVICE_MAN);
	if (0 == auth)
	{
		MessageBox(m_pStrTable->String(E_STR_ID_MESSAGE_80), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK | DEFAULT_ICON_WARNING);
		return KEY_VIRTUAL_MSG_STOP;
	}

	if( DLG_OK != CDialog::MessageBox( m_pStrTable->String( E_STR_ID_MESSAGE_53 ), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK | DEFAULT_BTN_CANCEL | DEFAULT_ICON_QUESTION) )
	{
		return KEY_VIRTUAL_MSG_STOP;
	}

	IP_DEVICE_INFO * pDataInfo = NULL;

	for (int i=0,j=0; i<iCount; i++)
	{
		pDataInfo = m_deviceList.GetItem(j);
		if((NULL != pDataInfo) && !pDataInfo->bPOEDevice)
		{
			m_deviceList.DelItem(j, false);
		}
		else
		{
			j++;
		}
	}

	m_deviceList.ClearMultiSelectList();

	m_btnAdd.SetEnable(true);
	
	m_deviceList.Repaint();
	
	SetModify( true );

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CDeviceManDlg::OnClickList(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	IP_DEVICE_INFO* pDataInfo = m_deviceList.GetListData();
	for (int i=static_cast< std::vector<unsigned long> >(m_deviceList.GetMultiSelect()).size() ; i > 0; i--)
	{
		if(!pDataInfo[m_deviceList.m_multiSelectList[i-1]].bPOEDevice)
		{
			m_btnDelete.SetEnable(true);
			m_btnDeleteAll.SetEnable(true);
			m_btnModify.SetEnable(true);
		}
	}
	
	/*IP_DEVICE_INFO* pDataInfo = m_deviceList.GetListData();
	if(pDataInfo[sel].bPOEDevice)
	{
		m_btnDelete.SetEnable(false);
// 		if(MID_TVT == pDataInfo[sel].manufacturerId)
// 		{
// 			m_btnModify.SetEnable(false);
// 		}
// 		else
		{
			m_btnModify.SetEnable(true);
		}
	}
	else
	{
		m_btnDelete.SetEnable(true);
		m_btnModify.SetEnable(true);
	}*/

	return KEY_VIRTUAL_STOP;
}

