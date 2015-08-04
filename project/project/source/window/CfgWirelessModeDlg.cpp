#include "CfgWireLessModeDlg.h"
#include "CfgNetworkExDlg.h"
CCfgWireLessModeDlg::CCfgWireLessModeDlg()
{

}

CCfgWireLessModeDlg::~CCfgWireLessModeDlg()
{

}

void CCfgWireLessModeDlg::OnInitial()
{
	m_style |= WND_NO_TITLE;
	GUI::CDialog::OnInitial();
	//////////////////////////////////////////////////////////////////////////
	GUI::CListCtrl& list = m_cfgNetWirelessList;
	GUI::CWnd * pWnd = NULL;
	GUI::CComboCtrl* pCombo = NULL;
	GUI::CCheckCtrl* pCheck = NULL;
	GUI::CEditCtrl* pEdit = NULL;
	GUI::CButton*	pBtn = NULL;

	unsigned long x = 0,y = 0;
	unsigned long width = m_rect.Width(), height = m_rect.Height();

	unsigned short offsetEdge = 0;

	int maxListRow = height/m_length[LIST_ITEM_HEIGHT]; //List框不带头
	if( ROW_WIRELESS_PWD >= maxListRow )
	{
		offsetEdge = m_length[SLIDER_WIDTH] + 2;
	}


	list.Create(GUI::CWnd::GetRandWndID(), x, y, width, height, this, 0, 0, false);
	list.AddColumn(NULL, width/2);
	list.AddColumn(NULL, width/2 - offsetEdge );




	list.AddItem(ROW_WIRELESS_ENABLE, 0, m_pStrTable->String(E_STR_ID_WIRELESS_ENABLE));
	pCheck = dynamic_cast<GUI::CCheckCtrl*>(list.AddItem(ROW_WIRELESS_ENABLE, 1, WND_TYPE_CHECK));
	assert(pCheck);

	list.AddItem(ROW_WIRELESS_TYPE, 0, m_pStrTable->String(E_STR_ID_WIRELESS_TYPE));
	pCombo = dynamic_cast<GUI::CComboCtrl*>(list.AddItem(ROW_WIRELESS_TYPE, 1, WND_TYPE_COMBO));
	assert(pCombo);
	pCombo->AddItem("WCDMA", WIRELESS_TYPE_WCDMA);
	pCombo->AddItem(m_pStrTable->String(E_STR_ID_TYPE_EVDO), WIRELESS_TYPE_EVDO);
	
	pCombo->SetCurItem( 0, false );

	list.AddItem(ROW_WIRELESS_CONNENT, 0, m_pStrTable->String(E_STR_ID_WIRELESS_CONNECT));
	pEdit = dynamic_cast<GUI::CEditCtrl*>(list.AddItem(ROW_WIRELESS_CONNENT, 1, WND_TYPE_EDIT));
	assert(pEdit);
	pEdit->SetMaxStringLen(67);

	list.AddItem(ROW_WIRELESS_APN, 0, m_pStrTable->String(E_STR_ID_WIRELESS_APN));
	pEdit = dynamic_cast<GUI::CEditCtrl*>(list.AddItem(ROW_WIRELESS_APN, 1, WND_TYPE_EDIT));
	assert(pEdit);
	pEdit->SetMaxStringLen(67);


	list.AddItem(ROW_WIRELESS_USER, 0, m_pStrTable->String(E_STR_ID_USER_NAME));
	pEdit = dynamic_cast<GUI::CEditCtrl*>(list.AddItem(ROW_WIRELESS_USER, 1, WND_TYPE_EDIT));
	assert(pEdit);
	pEdit->SetMaxStringLen(67);


	list.AddItem(ROW_WIRELESS_PWD, 0, m_pStrTable->String(E_STR_ID_PASSWORD));
	pEdit = dynamic_cast<GUI::CEditCtrl*>(list.AddItem(ROW_WIRELESS_PWD, 1, WND_TYPE_EDIT));
	assert(pEdit);
	pEdit->SetMaxStringLen(35);
	pEdit->SetPassword(true);

	list.CreateListFocusMap();
	
}
void CCfgWireLessModeDlg::OnPaint(PAINT_STATUS status)
{

}
void CCfgWireLessModeDlg::RefreshView(bool bRefresh)
{
	if(m_dataInfo.cfgID == NCFG_ITEM_NET_WIRELESS)
	{
		GUI::CCheckCtrl* pCheck = NULL;
		GUI::CComboCtrl* pCombo = NULL;
		GUI::CEditCtrl* pEdit = NULL;

		pCheck = dynamic_cast<GUI::CCheckCtrl*>(m_cfgNetWirelessList.GetItemWnd(ROW_WIRELESS_ENABLE,1));
		assert( NULL != pCheck );
		pCheck->SetCheck(m_dataInfo.mdvr_wireless.enable,bRefresh);

		pCombo = dynamic_cast<GUI::CComboCtrl*>(m_cfgNetWirelessList.GetItemWnd(ROW_WIRELESS_TYPE,1));
		assert(NULL!= pCombo);
		pCombo->SetCurItemData(m_dataInfo.mdvr_wireless.type,bRefresh);

		pEdit = dynamic_cast<GUI::CEditCtrl*>(m_cfgNetWirelessList.GetItemWnd(ROW_WIRELESS_CONNENT,1));
		assert( NULL != pEdit );
		pEdit->SetCaption(m_dataInfo.mdvr_wireless.dialup[m_dataInfo.mdvr_wireless.type - 1].ConnectPoint, bRefresh);

		pEdit = dynamic_cast<GUI::CEditCtrl*>(m_cfgNetWirelessList.GetItemWnd(ROW_WIRELESS_APN,1));
		assert( NULL != pEdit );
		pEdit->SetCaption(m_dataInfo.mdvr_wireless.dialup[m_dataInfo.mdvr_wireless.type - 1].APNNumber, bRefresh);

		pEdit = dynamic_cast<GUI::CEditCtrl*>(m_cfgNetWirelessList.GetItemWnd(ROW_WIRELESS_USER,1));
		assert( NULL != pEdit );
		pEdit->SetCaption(m_dataInfo.mdvr_wireless.dialup[m_dataInfo.mdvr_wireless.type - 1].username, bRefresh);

		pEdit = dynamic_cast<GUI::CEditCtrl*>(m_cfgNetWirelessList.GetItemWnd(ROW_WIRELESS_PWD,1));
		assert( NULL != pEdit );
		pEdit->SetCaption(m_dataInfo.mdvr_wireless.dialup[m_dataInfo.mdvr_wireless.type - 1].password, bRefresh);
	}

}


void CCfgWireLessModeDlg::OnModifyNetModeData(unsigned long cfgId)
{
	CCfgNetworkExDlg *pcfgNetWorkExDlg = dynamic_cast<CCfgNetworkExDlg*>(GetParent());
	assert(pcfgNetWorkExDlg);//
	pcfgNetWorkExDlg->SetDataInfo(m_dataInfo);
	pcfgNetWorkExDlg->OnModifyNetModeData(cfgId);
}

unsigned long CCfgWireLessModeDlg::CheckValue()
{
	unsigned char* pData = NULL;
	int dataLen = 0;
	MESSAGE_DATA msgData;
	NET_CTRL_FAIL result;

	pData = new unsigned char[sizeof(MDVR_WIRELESS_CFG)];
	memset(pData, 0, sizeof(MDVR_WIRELESS_CFG));
	dataLen = sizeof(MDVR_WIRELESS_CFG);
	MDVR_WIRELESS_CFG* pWirelessCfg = reinterpret_cast<MDVR_WIRELESS_CFG *>(pData);

	GUI::CCheckCtrl* pCheck = dynamic_cast<GUI::CCheckCtrl*>(m_cfgNetWirelessList.GetItemWnd(ROW_WIRELESS_ENABLE, 1));
	bool bUseWireless = pCheck->IsCheck();
	if (bUseWireless)
	{
		pWirelessCfg->enable =  true;
		GUI::CComboCtrl* pCombo = dynamic_cast<GUI::CComboCtrl*>(m_cfgNetWirelessList.GetItemWnd(ROW_WIRELESS_TYPE, 1));
		pWirelessCfg->type = pCombo->GetCurItemData();
		int type = pWirelessCfg->type - 1;

		GUI::CEditCtrl* pEdit = dynamic_cast<GUI::CEditCtrl*>(m_cfgNetWirelessList.GetItemWnd(ROW_WIRELESS_CONNENT, 1));
		snprintf(pWirelessCfg->dialup[type].ConnectPoint, 68, "%s", pEdit->GetCaption().c_str());

		pEdit = dynamic_cast<GUI::CEditCtrl*>(m_cfgNetWirelessList.GetItemWnd(ROW_WIRELESS_APN, 1));
		snprintf(pWirelessCfg->dialup[type].APNNumber, 68, "%s", pEdit->GetCaption().c_str());

		pEdit = dynamic_cast<GUI::CEditCtrl*>(m_cfgNetWirelessList.GetItemWnd(ROW_WIRELESS_USER, 1));
		snprintf(pWirelessCfg->dialup[type].username, 68, "%s", pEdit->GetCaption().c_str());

		pEdit = dynamic_cast<GUI::CEditCtrl*>(m_cfgNetWirelessList.GetItemWnd(ROW_WIRELESS_PWD, 1));
		snprintf(pWirelessCfg->dialup[type].password, 68, "%s", pEdit->GetCaption().c_str());

		MessageOut( GetTopestWnd(), E_STR_ID_PLEASE_WAIT, TIP_ERR );

		CMessageMan::Instance()->SendMsgDataToServer( LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_NETPARAMCHECK_WIRELESS, pData, dataLen, msgData );
		if( NULL !=  pData )
		{
			delete [] pData;
			pData = NULL;
		}

		assert( msgData.pData );
		PACKCMD *pCmd = reinterpret_cast<PACKCMD *>(msgData.pData);
		if( CMD_REPLY_NETPARAMCHECK_WIRELESS_SUCC == static_cast<NET_PROTOCOL>(pCmd->cmdType) )
		{
			return 0;
		}
		result = *reinterpret_cast< const NET_CTRL_FAIL* >( msgData.pData + sizeof(PACKCMD) );
		CMessageMan::Instance()->ReleaseMsgBuff(msgData);

		return GetErrorTextNum(result);
	}
	else
	{
		return 0;
	}

}

unsigned long CCfgWireLessModeDlg::GetErrorTextNum(NET_CTRL_FAIL result)
{
	ENUM_STRING_ID strID = E_STR_ID_NULL;

	switch( result )
	{
	case NETERR_NETPARAMCHECK_OK:						//连接通过
		strID = E_STR_ID_MESSAGE_CONNECT_SUCC;
		break;
	case NETERR_NETPARAMCHECK_ENODEVICE:				//无线设备断开
		strID = E_STR_ID_MESSAGE_29;
		break;
	case NETERR_NETPARMACHECK_ENOCARD:					//无线设备没有UIM卡
		strID = E_STR_ID_MESSAGE_30;
		break;
	case NETERR_NETPARMACHECK_ENOSIGNAL:				//没有无线信号
		strID = E_STR_ID_MESSAGE_66;
		break;
	case NETERR_NETPARMACHECK_ENETUNREACHD:				//网络不可达
		strID = E_STR_ID_MESSAGE_58;
		break;
	case NETERR_NETPARAMCHECK_EUNKNOWN:					//未知错误	
		strID = E_STR_ID_MESSAGE_59;
		break;
	case NETERR_NETPARAMCHECK_EPARAM:					//拨号参数错误
		strID = E_STR_ID_TIP_05_04;
	default:
		strID = E_STR_ID_MESSAGE_CONNECT_FAIL;
		break;
	}

	return strID;
}
BEGIN_MSG_MAP(CCfgWireLessModeDlg, GUI::CDialog)
	ON_MSG_EX(m_cfgNetWirelessList.GetID(), KEY_VIRTUAL_MSG_DEFAULT_TAG, OnClickList)
END_MSG_MAP()

unsigned long CCfgWireLessModeDlg::OnClickList(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	unsigned short row = (keyID >> 16)&0x7fff;	//行
	unsigned short col = keyID&0xffff;			//列

	GUI::CCheckCtrl* pCheck = NULL;
	GUI::CComboCtrl* pCbType = NULL;
	GUI::CEditCtrl*  pEdit = NULL;
	
	switch (row)
	{
	case ROW_WIRELESS_ENABLE:
		{
			m_dataInfo.cfgID = NCFG_ITEM_NET_WIRELESS;
			
			pCheck = dynamic_cast<GUI::CCheckCtrl*>( m_cfgNetWirelessList.GetItemWnd(ROW_WIRELESS_ENABLE,1) );
			m_dataInfo.mdvr_wireless.enable = pCheck->IsCheck()?1:0;
			OnModifyNetModeData(NCFG_ITEM_NET_WIRELESS);
			return KEY_VIRTUAL_MSG_CONTINUE;
			break;
		}
	case ROW_WIRELESS_TYPE:
		{
			m_dataInfo.cfgID = NCFG_ITEM_NET_WIRELESS;
			pCbType = dynamic_cast<GUI::CComboCtrl*>(m_cfgNetWirelessList.GetItemWnd(ROW_WIRELESS_TYPE,1));
			m_dataInfo.mdvr_wireless.type = pCbType->GetCurItemData();
			GUI::CEditCtrl* pEdit = NULL;
			//切换配置的话,去下面去读取出来
			//if(m_dataInfo.mdvr_wireless.type == 2)//evdo
			{
				//加载默认配置1的选项
				pEdit = dynamic_cast<GUI::CEditCtrl*>(m_cfgNetWirelessList.GetItemWnd(ROW_WIRELESS_CONNENT,1));
				assert( NULL != pEdit );
				pEdit->SetCaption(m_dataInfo.mdvr_wireless.dialup[m_dataInfo.mdvr_wireless.type - 1].ConnectPoint, false);

				pEdit = dynamic_cast<GUI::CEditCtrl*>(m_cfgNetWirelessList.GetItemWnd(ROW_WIRELESS_APN,1));
				assert( NULL != pEdit );
				pEdit->SetCaption(m_dataInfo.mdvr_wireless.dialup[m_dataInfo.mdvr_wireless.type - 1].APNNumber, false);

				pEdit = dynamic_cast<GUI::CEditCtrl*>(m_cfgNetWirelessList.GetItemWnd(ROW_WIRELESS_USER,1));
				assert( NULL != pEdit );
				pEdit->SetCaption(m_dataInfo.mdvr_wireless.dialup[m_dataInfo.mdvr_wireless.type - 1].username, false);

				pEdit = dynamic_cast<GUI::CEditCtrl*>(m_cfgNetWirelessList.GetItemWnd(ROW_WIRELESS_PWD,1));
				assert( NULL != pEdit );
				pEdit->SetCaption(m_dataInfo.mdvr_wireless.dialup[m_dataInfo.mdvr_wireless.type - 1].password, false);

				Repaint();

			}
			OnModifyNetModeData(NCFG_ITEM_NET_WIRELESS);
			return KEY_VIRTUAL_MSG_CONTINUE;
			break;
		}
	case ROW_WIRELESS_CONNENT:
		{
			m_dataInfo.cfgID = NCFG_ITEM_NET_WIRELESS;
			pEdit = dynamic_cast<GUI::CEditCtrl*>(m_cfgNetWirelessList.GetItemWnd(ROW_WIRELESS_CONNENT,1));
			assert(pEdit);
			strcpy(m_dataInfo.mdvr_wireless.dialup[m_dataInfo.mdvr_wireless.type - 1].ConnectPoint,pEdit->GetCaption().c_str());
			OnModifyNetModeData(NCFG_ITEM_NET_WIRELESS);
			return KEY_VIRTUAL_MSG_CONTINUE;
			break;
		}
	case ROW_WIRELESS_APN:
		{
			m_dataInfo.cfgID = NCFG_ITEM_NET_WIRELESS;
			pEdit = dynamic_cast<GUI::CEditCtrl*>(m_cfgNetWirelessList.GetItemWnd(ROW_WIRELESS_APN,1));
			assert(pEdit);
			strcpy(m_dataInfo.mdvr_wireless.dialup[m_dataInfo.mdvr_wireless.type - 1].APNNumber,pEdit->GetCaption().c_str());
			OnModifyNetModeData(NCFG_ITEM_NET_WIRELESS);
			return KEY_VIRTUAL_MSG_CONTINUE;
			break;
		}
	case ROW_WIRELESS_USER:
		{
			m_dataInfo.cfgID = NCFG_ITEM_NET_WIRELESS;
			pEdit = dynamic_cast<GUI::CEditCtrl*>(m_cfgNetWirelessList.GetItemWnd(ROW_WIRELESS_USER,1));
			assert(pEdit);
			strcpy(m_dataInfo.mdvr_wireless.dialup[m_dataInfo.mdvr_wireless.type - 1].username,pEdit->GetCaption().c_str());
			OnModifyNetModeData(NCFG_ITEM_NET_WIRELESS);
			return KEY_VIRTUAL_MSG_CONTINUE;
			break;
		}
	case ROW_WIRELESS_PWD:
		{
			m_dataInfo.cfgID = NCFG_ITEM_NET_WIRELESS;
			pEdit = dynamic_cast<GUI::CEditCtrl*>(m_cfgNetWirelessList.GetItemWnd(ROW_WIRELESS_PWD,1));
			assert(pEdit);
			strcpy(m_dataInfo.mdvr_wireless.dialup[m_dataInfo.mdvr_wireless.type - 1].password,pEdit->GetCaption().c_str());
			OnModifyNetModeData(NCFG_ITEM_NET_WIRELESS);
			return KEY_VIRTUAL_MSG_CONTINUE;
			break;
			
		}

	default:
		break;
	}

	return KEY_VIRTUAL_MSG_STOP;
}