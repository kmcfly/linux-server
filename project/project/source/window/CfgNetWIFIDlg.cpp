#include "CfgNetWIFIDlg.h"
#include "ListCtrlEx.cpp"
#include "CfgNetWIFIIPInfoDlg.h"

template <class T>
std::string &  GUI::CListCtrlEx<T>::Text(T &node, unsigned short col)
{
	NET_WIFI_MODEM &it = node;
	char str [128] = {0};

	if (0 == col)
	{	
		memset(str, 0, sizeof(str));
		sprintf(str, "%d", it.dwChannel);
		m_str = str;
	}

	if (1 == col)
	{
		memset(str, 0, sizeof(str));
		sprintf(str, "%s", it.ssid);
		m_str = str;
	}

	if (2 == col)
	{
		memset(str, 0, sizeof(str));
		switch(it.dwSignal)
		{
		case 0:
			snprintf(str, 128, "%s",m_pStrTable->String(E_STR_ID_SIGNAL_NONE));
			break;
		case 1:	
			snprintf(str, 128, "%s",m_pStrTable->String(E_STR_ID_SIGNAL_WEAKER));
			break;
		case 2:
			snprintf(str, 128, "%s",m_pStrTable->String(E_STR_ID_SIGNAL_WEAK));
			break;
		case 3:
			snprintf(str, 128, "%s",m_pStrTable->String(E_STR_ID_SIGNAL_GENERAL));
			break;
		case 4:
			snprintf(str, 128, "%s",m_pStrTable->String(E_STR_ID_SIGNAL_BATTER));
			break;
		case 5:
			snprintf(str, 128, "%s",m_pStrTable->String(E_STR_ID_SIGNAL_GOOD));
			break;
		default:
			break;
		}
		m_str = str;
	}

	if (3 == col)
	{
		memset(str, 0, sizeof(str));	
		if((it.dwAuthMode == WIFI_AUTH_OPEN) && (it.dwEncrypType == WIFI_ENCRYP_NONE))
		{
			snprintf(str, 128, "%s",m_pStrTable->String(E_STR_ID_SIGNAL_UNLOCK));
		}
		else
		{
			snprintf(str, 128, "%s",m_pStrTable->String(E_STR_ID_SIGNAL_LOCK));		
		}
		m_str = str;
	}
	
	if(4 == col)
	{
		memset(str, 0, sizeof(str));
		switch(it.dwSignalUseState)
		{
		case 0:
			snprintf(str, 128, "%s",m_pStrTable->String(E_STR_ID_SIGNAL_UNUSE));
			break;
		case 1:
			snprintf(str, 128, "%s",m_pStrTable->String(E_STR_ID_SIGNAL_USING));
			break;
		case 2:
			snprintf(str,128, "%s",m_pStrTable->String(E_STR_ID_SIGNAL_USED));		
			break;
		default:
			break;
		}
		m_str = str;
	}
	
	return m_str;
}

CCfgNetWIFIDlg::CCfgNetWIFIDlg()
{
	m_SelectItemBak = 0;	
	m_pwificfg = NULL;
}

CCfgNetWIFIDlg::~CCfgNetWIFIDlg()
{
	
}

void CCfgNetWIFIDlg::OnInitial()
{
	CCfgDlg::OnInitial();
	//////////////////////////////////////////////////////////////////////////
	GUI::CCheckCtrl *pCheck = NULL;
	GUI::CStatic	*pStatic = NULL;
	GUI::CEditCtrl  *pEditCtrl = NULL;
	GUI::CButton	*pButton = NULL;

	unsigned short x = m_length[DLG_OFFSET_SUB_LEFT], y = m_length[DLG_OFFSET_SUB_TOP];
	unsigned short width = m_rect.Width() - 2 * m_length[DLG_OFFSET_SUB_LEFT];
	unsigned short height = m_rect.Height() - y - m_length[DLG_OFFSET_SUB_BOTTOM];
	
	unsigned short offsetEdge = 0;
	int maxListRow = (height - m_length[LIST_HEAD_HEIGHT])/m_length[LIST_ITEM_HEIGHT]; 
	if( 12 > maxListRow )
	{
		offsetEdge = m_length[SCROLL_WIDTH] + 2;
	}

	//enable list	
	GUI::CListCtrl&listEn = m_cfgListEnable;
	listEn.Create( GUI::CWnd::GetRandWndID(), x, y, width, m_length[LIST_ITEM_HEIGHT], this, 0, 1, false );
	listEn.AddColumn(NULL, width/2-80);
	listEn.AddColumn(NULL, width/2-80 );
	listEn.AddColumn(NULL, width/5-30 );

	listEn.AddItem(ROW_WIFI_ENABLE, 0, m_pStrTable->String(E_STR_ID_WIFI_ENABLE));
	pCheck = dynamic_cast<GUI::CCheckCtrl*>(listEn.AddItem(ROW_WIFI_ENABLE, 1, WND_TYPE_CHECK));
	assert(pCheck);
	pButton = dynamic_cast<GUI::CButton*>(listEn.AddItem(ROW_WIFI_ENABLE, 2, WND_TYPE_BUTTON));
	assert(pButton);
	pButton->SetCaption(m_pStrTable->String( E_STR_ID_SEARCH_SIGNAL ), false);
	pButton->SetEnable(false,false);
	listEn.CreateListFocusMap();
	
	//search signal string
	y+=m_length[LIST_ITEM_HEIGHT]+ 8;
	char str[100] = {0};
	m_cfgListStrSearch.Create( GUI::CWnd::GetRandWndID(), x, y, width / 2, m_length[STATIC_HEIGHT], this );
	strcpy( str, m_pStrTable->String( E_STR_ID_SEARCH_SIGNAL ) );
	m_cfgListStrSearch.SetCaption( str, false );

	//search signal list
	unsigned short high = m_rect.Height()- m_length[LIST_ITEM_HEIGHT]*3- m_length[STATIC_HEIGHT]- m_length[BTN_HEIGHT]-38;
	y+=m_length[STATIC_HEIGHT]+3;

	m_cfgListSignal.Create(GUI::CWnd::GetRandWndID(), x,  y, width, high, this, 0, 1);	
	m_cfgListSignal.AddColumn(m_pStrTable->String(E_STR_ID_WIFI_CHANNEL), width * 5 / 32-10 );
	m_cfgListSignal.AddColumn(m_pStrTable->String(E_STR_ID_WIFI_SSID), width * 5 / 32 +110 );
	m_cfgListSignal.AddColumn(m_pStrTable->String(E_STR_ID_WIFI_SIGNAL), width * 5 / 32+40);
	m_cfgListSignal.AddColumn(m_pStrTable->String(E_STR_ID_WIFI_ENCRYPT), width * 5 / 32 + 30);  //只有当AuthMode=OPEN和EncrypType=NONE时，才是不需要输入密码
	m_cfgListSignal.AddColumn(m_pStrTable->String(E_STR_ID_WIFI_USE_STATE), width * 5 / 32+40 );
	m_cfgListSignal.EnableSingleSelect();

	//password
	high+=m_length[LIST_ITEM_HEIGHT];
	GUI::CListCtrl&listPassword = m_cfgListInputPass;
	listPassword.Create( GUI::CWnd::GetRandWndID(), x, y+high, width, m_length[LIST_ITEM_HEIGHT], this, 0, 1, false );
	listPassword.AddColumn(NULL, width/2);
	listPassword.AddColumn(NULL, width/2 - 4 );
	listPassword.AddItem(0, 0, m_pStrTable->String(E_STR_ID_WIFI_PASSWORD));
	pEditCtrl = dynamic_cast<GUI::CEditCtrl*>(listPassword.AddItem(0, 1, WND_TYPE_EDIT));
	assert(pEditCtrl);
	pEditCtrl->SetPassword( true );
	listPassword.CreateListFocusMap();
	
	//setup button
	high+=m_length[LIST_ITEM_HEIGHT]+8;
	x = m_rect.Width()- m_length[BTN_WIDTH]*3;
	m_cfgSetupBtn.Create( GUI::CWnd::GetRandWndID(),x,y+high, m_length[BTN_WIDTH]+15, m_length[BTN_HEIGHT], this, 0, 1, m_pStrTable->String(E_STR_ID_SETUP) );
	
	//test button
	x+=m_length[BTN_WIDTH]+40;
	m_cfgTestBtn.Create( GUI::CWnd::GetRandWndID(),x,y+high, m_length[BTN_WIDTH]+15, m_length[BTN_HEIGHT], this, 0, 1, m_pStrTable->String(E_STR_ID_TEST) );
	
	if(m_pwificfg == NULL)
	{
		m_pwificfg = new WIFI_CONFIG[MAX_WIFI_CFG_NUM];
		memset(m_pwificfg, 0, sizeof(WIFI_CONFIG)*MAX_WIFI_CFG_NUM);
	}
}

void CCfgNetWIFIDlg::OnDestroy()
{
	if(m_pwificfg!=NULL)
	{
		delete []m_pwificfg;
		m_pwificfg = NULL;
	}
}

bool CCfgNetWIFIDlg::ShowData(NCFG_ITEM_ID cfgID, const BYTE* pData, int len, bool bRefresh)
{

	assert(NULL != pData);
	assert(len > 0);

	switch (cfgID)
	{
	case NCFG_ITEM_WIRELESS_WIFI:
		{
			assert(len == (sizeof(WIFI_CONFIG)*MAX_WIFI_CFG_NUM));
			const WIFI_CONFIG *pValue = reinterpret_cast<const WIFI_CONFIG*>(pData);

			GUI::CCheckCtrl* pCheck = NULL;
			GUI::CButton* pButton = NULL;
			GUI::CEditCtrl*pEdit = NULL;

			//显示wifi enable 状态和搜素按钮
			pCheck = dynamic_cast<GUI::CCheckCtrl*>(m_cfgListEnable.GetItemWnd(ROW_WIFI_ENABLE,1));
			pCheck->SetCheck(pValue[0].enable,bRefresh);
			pButton = dynamic_cast<GUI::CButton*>(m_cfgListEnable.GetItemWnd(ROW_WIFI_ENABLE,2));
			pEdit = dynamic_cast<GUI::CEditCtrl*>(m_cfgListInputPass.GetItemWnd(0,1));			

			if(pCheck->IsCheck())
			{
				pButton->SetEnable(true,false);
				pEdit->EnableInput(true);
				m_cfgSetupBtn.SetEnable(true,false);
				m_cfgTestBtn.SetEnable(true,false);
			}
			else
			{
				pButton->SetEnable(false,false);
				pEdit->EnableInput(false);
				m_cfgSetupBtn.SetEnable(false,false);
				m_cfgTestBtn.SetEnable(false,false);
			}
			
			//根据wifi enable状态显示搜素列表
			if(pValue[0].enable)
			{
				SearchSignal();
				CompareListItem(pData, bRefresh);
			}

			//显示密码
			assert(pEdit);
			pEdit->SetMaxStringLen(36);
			pEdit->SetCaption(pValue[0].key1,bRefresh);
			
			//初始化setup 按钮的值
			memcpy(m_pwificfg, pData, (sizeof(WIFI_CONFIG)*MAX_WIFI_CFG_NUM));
			
		}
		return true;
		break;
	default:
		return false;
		break;
	}
	return false;
}

bool CCfgNetWIFIDlg::SetData(NCFG_ITEM_ID cfgID, BYTE*& pData, int& len)  const
{
	assert(NULL != pData);
	assert(len > 0);

	switch (cfgID)
	{
	case NCFG_ITEM_WIRELESS_WIFI:
		{
			assert(len == (sizeof(WIFI_CONFIG)*MAX_WIFI_CFG_NUM));
			WIFI_CONFIG *pValue = reinterpret_cast<WIFI_CONFIG*>(pData);

			GUI::CCheckCtrl* pCheck = NULL;
			//设置使能
			pCheck = dynamic_cast<GUI::CCheckCtrl*>((GUI::CCheckCtrl*)m_cfgListEnable.GetItemWnd(ROW_WIFI_ENABLE,1));
			assert(pCheck);
			pValue[0].enable = pCheck->IsCheck()?1:0;
			
			//当选择某条信息后保存起来
			if(pCheck->IsCheck())
			{
				long sel = m_cfgListSignal.GetSingleSelect();
				if(sel >=0 )
				{
					//返回的是一个引用方式
					assert(len == (sizeof(WIFI_CONFIG)*MAX_WIFI_CFG_NUM));

					const NET_WIFI_MODEM*pSerachInfo = m_cfgListSignal.GetItem(sel);
					pValue[0].authMode = pSerachInfo->dwAuthMode;
					pValue[0].encrypType = pSerachInfo->dwEncrypType;
					pValue[0].channel = pSerachInfo->dwChannel;
					strcpy(pValue[0].ssid, pSerachInfo->ssid);
				}
			}
			
			//设置密码
			GUI::CEditCtrl*pEdit = NULL;
			pEdit= dynamic_cast<GUI::CEditCtrl*>((GUI::CEditCtrl*)m_cfgListInputPass.GetItemWnd(0,1));	
			assert(pEdit);
			strcpy(pValue[0].key1,pEdit->GetCaption().c_str());
			
			//由于直接拷贝会覆盖刚刚设置的值，所以要先拷贝
			m_pwificfg[0].enable = pValue[0].enable;
			m_pwificfg[0].authMode = pValue[0].authMode;
			m_pwificfg[0].encrypType = pValue[0].encrypType;
			m_pwificfg[0].channel = pValue[0].channel;
			strcpy(m_pwificfg[0].ssid, pValue[0].ssid);
			strcpy(m_pwificfg[0].key1, pValue[0].key1);

			//获得到setup button 的值
			memcpy(pData, m_pwificfg, (sizeof(WIFI_CONFIG)*MAX_WIFI_CFG_NUM));
		}
		return true;
		break;
	default:
		return false;
		break;
	}
	return false;
}

void CCfgNetWIFIDlg::CompareListItem(const BYTE* pData, bool bRefresh)
{
	int CountNum = m_cfgListSignal.GetCount();
	const WIFI_CONFIG *pwificfg = reinterpret_cast<const WIFI_CONFIG*>(pData);

	NET_WIFI_MODEM *pWifiMode = NULL;
	for(int i=0; i<CountNum; i++)
	{
		//返回的是一个引用方式
		pWifiMode = m_cfgListSignal.GetItem(i);
		if((pwificfg[0].authMode == pWifiMode->dwAuthMode) && (pwificfg[0].encrypType == pWifiMode->dwEncrypType) &&
			(pwificfg[0].channel == pWifiMode->dwChannel) && (strcmp(pwificfg[0].ssid, pWifiMode->ssid) ==0))
		{
			//说明当前条是上次用户选择的也是正在使用的
			m_cfgListSignal.SetSingleSelect(i, bRefresh);
			pWifiMode->dwSignalUseState = 1;
			m_cfgListSignal.Repaint();
			m_SelectItemBak = i+1;
		}
	}
}


void CCfgNetWIFIDlg::SearchSignal(void)
{
	MESSAGE_DATA msgData;
	memset( &msgData, 0, sizeof( msgData ) );
	unsigned long dataLen = 0;

	GUI::CCheckCtrl* pCheck = dynamic_cast<GUI::CCheckCtrl*>( m_cfgListEnable.GetItemWnd( ROW_WIFI_ENABLE, 1) );
	bool bwifiEn = pCheck->IsCheck();

	if(bwifiEn)
	{	
#ifdef  __MOBILE_DVR__
		CMessageMan::Instance()->SendMsgDataToServer( LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_NET_WIFI_SEARCH, NULL, 0, msgData );

		assert( msgData.pData );
		PACKCMD *pCmd = reinterpret_cast<PACKCMD *>(msgData.pData);

		switch( static_cast<NET_PROTOCOL>(pCmd->cmdType) )
		{
		case CMD_REPLY_NET_WIFI_SEARCH_SUCC:
			{	
				m_cfgListSignal.PutData(NULL, 0, false);
				unsigned char number = 0;
				NET_WIFI_MODEM *pwifimode = NULL;
				pwifimode = (NET_WIFI_MODEM*)(msgData.pData+sizeof(PACKCMD));
				number = (msgData.dataLen-sizeof(PACKCMD))/sizeof(NET_WIFI_MODEM);			
				NET_WIFI_MODEM *pSignalInfo = new NET_WIFI_MODEM[number];
				for(unsigned int i=0;i<number;i++)
				{
					pSignalInfo[i] = pwifimode[i];		
				}
				m_cfgListSignal.PutData(pSignalInfo,number);

				pSignalInfo = NULL;

				CMessageMan::Instance()->ReleaseMsgBuff(msgData);
			}
			break;
		case CMD_REPLY_NET_WIFI_SEARCH_FALL:
			{
			}
			break;
		default:

			break;
		}
#endif
	}
}

void CCfgNetWIFIDlg::SetErrorText(unsigned char checkResult)
{
	ENUM_STRING_ID strID = E_STR_ID_NULL;

	switch(checkResult)
	{
		case WIFI_CONNECT_SUCC:
			strID = E_STR_ID_MESSAGE_CONNECT_SUCC;
			break;
		case WIFI_CONNECT_FAIL: 
			strID = E_STR_ID_MESSAGE_CONNECT_FAIL;
			break;
		default:
			strID = E_STR_ID_MESSAGE_59;		//未知错误
			break;
	}
	MessageOut( GetTopestWnd(), strID, TIP_ERR );
}

BEGIN_MSG_MAP(CCfgNetWIFIDlg, CCfgDlg)
	ON_MSG_EX(m_cfgListEnable.GetID(), KEY_VIRTUAL_MSG_DEFAULT_TAG, OnClickEnableList)
	//ON_MSG_EX(m_cfgListSignal.GetID(), KEY_VIRTUAL_MSG_DEFAULT_TAG, OnClickSearchList)
	ON_MSG_EX(m_cfgListInputPass.GetID(), KEY_VIRTUAL_MSG_DEFAULT_TAG, OnClickInputPasswod)
	ON_MSG( m_cfgSetupBtn.GetID(), KEY_VIRTUAL_ENTER, OnBtnSetup)
	ON_MSG( m_cfgTestBtn.GetID(), KEY_VIRTUAL_ENTER, OnBtnTest)
END_MSG_MAP()

unsigned long CCfgNetWIFIDlg::OnClickEnableList(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	unsigned short row = (keyID >> 16)&0x7fff;	//行
	unsigned short col = keyID&0xffff;			//列
	
	if((row == ROW_WIFI_ENABLE) && (col == 1))
	{
		GUI::CCheckCtrl* pCheck = NULL;
		GUI::CButton* pButton = NULL;
		GUI::CEditCtrl*pEdit =NULL;

		pButton = dynamic_cast<GUI::CButton*>((GUI::CButton*)m_cfgListEnable.GetItemWnd(ROW_WIFI_ENABLE,2));
		assert(pButton);
		pCheck = dynamic_cast<GUI::CCheckCtrl*>((GUI::CCheckCtrl*)m_cfgListEnable.GetItemWnd(ROW_WIFI_ENABLE,1));
		assert(pCheck);	
		pEdit = dynamic_cast<GUI::CEditCtrl*>((GUI::CEditCtrl*)m_cfgListInputPass.GetItemWnd(0,1));
		assert(pEdit);	

		if(pCheck->IsCheck())
		{
			pButton->SetEnable(true,true);
			pEdit->EnableInput(true);
			m_cfgSetupBtn.SetEnable(true,true);
			m_cfgTestBtn.SetEnable(true,true);
		}
		else
		{
			pButton->SetEnable(false,true);
			pEdit->EnableInput(false);
			m_cfgSetupBtn.SetEnable(false,true);
			m_cfgTestBtn.SetEnable(false,true);
			
			m_cfgListSignal.PutData(NULL, 0, true);
		}

	}

	if((row == ROW_WIFI_ENABLE)&& (col == 2) )
	{
		SearchSignal();
		//显示搜素列表
		//RefreshList();
		
		if(m_SelectItemBak != 0)
		{
			NET_WIFI_MODEM *pWifiMode;
			m_cfgListSignal.SetSingleSelect(m_SelectItemBak-1, true);
			pWifiMode = m_cfgListSignal.GetItem(m_SelectItemBak-1);
			pWifiMode->dwSignalUseState = 1;
		}
		Repaint();	  
	}

	if (!IsModify())
	{
		switch (row)
		{
		case ROW_WIFI_ENABLE:
			{
				if(col == 1)
				{
					if(IsChange(NCFG_ITEM_WIRELESS_WIFI))	
					{	
						SetModify(true);
						return KEY_VIRTUAL_MSG_CONTINUE;
					}
				}
			}		
			break;

		default:
			assert(false);
			break;
		}

		return KEY_VIRTUAL_MSG_CONTINUE;
	} 
	else
	{
		return KEY_VIRTUAL_MSG_STOP;
	}
	
}


unsigned long CCfgNetWIFIDlg::OnClickInputPasswod(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	unsigned short row = (keyID >> 16)&0x7fff;	//行
	unsigned short col = keyID&0xffff;			//列
	
	if (!IsModify())
	{
		switch (row)
		{
		case 0:
			{
				if(col == 1)
				{
					if(IsChange(NCFG_ITEM_WIRELESS_WIFI))	
					{	
						SetModify(true);
						return KEY_VIRTUAL_MSG_CONTINUE;
					}
				}
			}		
			break;

		default:
			assert(false);
			break;
		}

		return KEY_VIRTUAL_MSG_CONTINUE;
	} 
	else
	{
		return KEY_VIRTUAL_MSG_STOP;
	}
}

unsigned long CCfgNetWIFIDlg::OnBtnSetup()
{
	CCfgNetWIFIIpInfoDlg IPdlg;
	IPdlg.SetwifiIpInfo(m_pwificfg);
	if(DLG_OK == IPdlg.DoModal(this))
	{
		IPdlg.GetwifiIpInfo(m_pwificfg);
		if(!IsModify())
		{
			if(IsChange(NCFG_ITEM_WIRELESS_WIFI))
			{
				SetModify(true);
				return KEY_VIRTUAL_MSG_CONTINUE;
			}
		}
	}
	
	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CCfgNetWIFIDlg::OnBtnTest()
{
	MESSAGE_DATA msgData;
	memset( &msgData, 0, sizeof( msgData ) );
	unsigned char ret = 0;

	GUI::CCheckCtrl*pCheck = dynamic_cast<GUI::CCheckCtrl*>((GUI::CCheckCtrl*)m_cfgListEnable.GetItemWnd(ROW_WIFI_ENABLE,1));
	
	//重新获得当前的值
	WIFI_CONFIG wificfgtmp;
	wificfgtmp.enable = pCheck->IsCheck()?1:0;
	if(pCheck->IsCheck())
	{
		long sel = m_cfgListSignal.GetSingleSelect();
		if(sel >=0 )
		{
			//返回的是一个引用方式
			const NET_WIFI_MODEM*pSerachInfo = m_cfgListSignal.GetItem(sel);
			wificfgtmp.authMode = pSerachInfo->dwAuthMode;
			wificfgtmp.encrypType = pSerachInfo->dwEncrypType;
			wificfgtmp.channel = pSerachInfo->dwChannel;
			strcpy(wificfgtmp.ssid, pSerachInfo->ssid);
		}
	}
	GUI::CEditCtrl*pEdit =dynamic_cast<GUI::CEditCtrl*>((GUI::CEditCtrl*)m_cfgListInputPass.GetItemWnd(0,1));	
	assert(pEdit);
	strcpy(wificfgtmp.key1,pEdit->GetCaption().c_str());
	wificfgtmp.IPMode = m_pwificfg[0].IPMode;
	wificfgtmp.gateway  = m_pwificfg[0].gateway;
	wificfgtmp.staticIP = m_pwificfg[0].staticIP;
	wificfgtmp.subMask = m_pwificfg[0].subMask;
	wificfgtmp.DNSServer = m_pwificfg[0].DNSServer;

	if(pCheck->IsCheck())
	{
#ifdef  __MOBILE_DVR__
		CMessageMan::Instance()->SendMsgDataToServer( LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_NET_WIFI_CONNECT,  reinterpret_cast<unsigned char*>(&wificfgtmp), sizeof(WIFI_CONFIG), msgData);
		assert( msgData.pData );
		PACKCMD *pCmd = reinterpret_cast<PACKCMD *>(msgData.pData);

		switch( static_cast<NET_PROTOCOL>(pCmd->cmdType) )
		{
		case CMD_REPLY_NET_WIFI_CONNECT_SUCC:
			{
				ret = 1;
			}
			break;
		case CMD_REPLY_NER_WIFI_CONNECT_FAIL:
			{
				ret = 2;
			}
			break;
		default:
			break;

		}
#endif
	}

	SetErrorText(ret);
	
	return KEY_VIRTUAL_MSG_STOP;
}

