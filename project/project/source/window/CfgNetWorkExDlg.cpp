#include "CfgNetworkExDlg.h"
//#include "RadioCtrl.h"
extern DVR_SYSTEM_STATUS g_system_status;
extern unsigned long g_IsChangeNetMode;

CCfgNetworkExDlg::CCfgNetworkExDlg()
{
	
}

CCfgNetworkExDlg::~CCfgNetworkExDlg()
{

}

void CCfgNetworkExDlg::OnInitial()
{
	CCfgDlg::OnInitial();

	memset(&m_datainfo,0,sizeof(SETUPDATAINFO));
	GUI::CListCtrl& list = m_cfgNetworkList;
	GUI::CWnd * pWnd = NULL;
	GUI::CEditCtrl*	 pEdit = NULL;
	GUI::CNumEditCtrl* pNumEdit = NULL;
	
	unsigned short x = m_length[DLG_OFFSET_SUB_LEFT], y = m_length[DLG_OFFSET_SUB_TOP];
	unsigned short width = m_rect.Width() - 2 * m_length[DLG_OFFSET_SUB_LEFT];
	unsigned short height = m_length[LIST_ITEM_HEIGHT]*2;;

	unsigned short offsetEdge = 0;
	int maxListRow = height/m_length[LIST_ITEM_HEIGHT]; //List框不带头

	list.Create(GUI::CWnd::GetRandWndID(), x, y, width, height, this, 0, 0, false);

	list.AddColumn(NULL, width * 16 / 32);
	list.AddColumn(NULL, width * 16 / 32 - offsetEdge);


	////
	char str[128] = {0};
	int row = 0;

	// row = 0
	list.AddItem( ROW_HTTP_PORT, 0, m_pStrTable->String( E_STR_ID_HTTP_PORT ) );
	pNumEdit = dynamic_cast<GUI::CNumEditCtrl*>( list.AddItem( ROW_HTTP_PORT, 1, WND_TYPE_NUM_EDIT) );
	assert( pNumEdit );
	pNumEdit->SetMaxStringLen( 5 );
	pNumEdit->SetRange(0,65535);

	row += 1;
	list.AddItem( ROW_SERVER_PORT, 0, m_pStrTable->String(E_STR_ID_SERVER_PORT) );
	pNumEdit = dynamic_cast<GUI::CNumEditCtrl*>( list.AddItem( ROW_SERVER_PORT, 1, WND_TYPE_NUM_EDIT) );
	assert( pNumEdit );
	pNumEdit->SetMaxStringLen( 5 );
	pNumEdit->SetRange(0,(int)65535);

	list.CreateListFocusMap();
	
	m_scInfo.Create(GetRandWndID(),x+4,y+height + (m_length[LIST_ITEM_HEIGHT]-m_length[STATIC_HEIGHT])/2,width/2 - 6,m_length[STATIC_HEIGHT],this);
	m_scInfo.SetCaption(m_pStrTable->String(E_STR_ID_NETWORK_TYPE),false);

	m_rdNetWork.Create(GetRandWndID(),x+width/2+4 ,y+height+(m_length[LIST_ITEM_HEIGHT]-m_length[RADIO_HEIGHT])/2,(width-16)/4,m_length[RADIO_HEIGHT],this,0,1,&m_rdGroup);
	m_rdWireless.Create(GetRandWndID(),x+width/2+4 + (width-16)/4,y+height+(m_length[LIST_ITEM_HEIGHT]-m_length[RADIO_HEIGHT])/2,(width-16)/4 - 2,m_length[RADIO_HEIGHT],this,1,1,&m_rdGroup);

	m_rdGroup.AddRadio(&m_rdNetWork);
	m_rdGroup.AddRadio(&m_rdWireless);

	m_rdNetWork.SetCaption(m_pStrTable->String(E_STR_ID_WIRED_NETWORK),false,false);
	m_rdNetWork.SetData(PAGE_NETWORK);
	m_rdWireless.SetCaption(m_pStrTable->String(E_STR_ID_NETWORK_WIRELESS),false,false);
	m_rdWireless.SetData(PAGE_WIRELESS);

	y += height+m_length[LIST_ITEM_HEIGHT];
	height = m_rect.Height() - y - m_length[DLG_OFFSET_SUB_BOTTOM];
	m_localNetModeDlg.SetDataInfo(m_datainfo);
	m_wirelessModeDlg.SetDataInfo(m_datainfo);

	m_localNetModeDlg.Create(GetRandWndID(),x,y,width,height,this,0,2,false);
	m_wirelessModeDlg.Create(GetRandWndID(),x,y,width,height,this,1,2,false);

	////
	m_curPage = PAGE_NULL;
	//m_rdNetWork.SetCheck(true,false);
	//SetPage(PAGE_NETWORK,false);
	//Repaint();

	m_poeNum = CProduct::Instance()->PoeInputNum();

}
void CCfgNetworkExDlg::OnPaint(PAINT_STATUS status)
{
	CCfgDlg::OnPaint(status);

	////
	unsigned long x = m_rect.m_left + m_length[DLG_OFFSET_SUB_LEFT];
	unsigned long y = m_rect.m_top + m_length[DLG_OFFSET_SUB_TOP] + m_length[LIST_ITEM_HEIGHT]*2;
	unsigned long width = m_rect.Width() - m_length[DLG_OFFSET_SUB_LEFT] * 2;
	unsigned long height = m_length[LIST_ITEM_HEIGHT];

	//左竖
	GUI::CWnd::DrawLine(x,y,x,y+height,COLOR_LINE_GRAY,2);

	//中竖
	GUI::CWnd::DrawLine(x+width/2,y,x+width/2,y+height,COLOR_LINE_GRAY,1);

	//右竖
	GUI::CWnd::DrawLine(x+width-2,y,x+width-2,y+height,COLOR_LINE_GRAY,2);
	
}
void CCfgNetworkExDlg::CheckUI(bool bRefresh)
{
	GUI::CRadioCtrl* pRadio = m_rdGroup.GetCheckRadio();
	if( pRadio )
	{
		switch ( static_cast<PAGE_ITEM>(pRadio->GetData()) )
		{
		case PAGE_NETWORK:
			SetPage(PAGE_NETWORK,bRefresh);
			break;
		case PAGE_WIRELESS:
			SetPage(PAGE_WIRELESS,bRefresh);
			break;
		default:
			assert(false);
		}
	}
	else
	{
		assert(false);
	}
}
void CCfgNetworkExDlg::SetPage(PAGE_ITEM item /*= PAGE_NETWORK*/,bool bRefresh /*= true*/ )
{
	if(m_curPage != item )
	{
		if(m_curPage!=PAGE_NULL)
		{
			OnModifyNetModeData(NCFG_ITEM_USE_3G);
			//只要切换都要重启
			//if(DLG_OK == MessageBox(m_pStrTable->String(E_STR_ID_CHANGE_NET_TYPE), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK | DEFAULT_BTN_CANCEL | DEFAULT_ICON_WARNING))
			{
				m_curPage = item;
				g_IsChangeNetMode = item;
				//g_system_status=DVR_SYSTEM_REBOOT;

			}
			//else
			{
				//m_curPage = item;
			}
		}
		else
		{
			m_curPage = item;
		}
		//
	}
		m_localNetModeDlg.Show(false,false);
		m_wirelessModeDlg.Show(false,false);
		printf("m_curPage=%d\n",m_curPage);
		switch ( m_curPage )
		{
		case PAGE_NETWORK:
			{
				m_rdNetWork.SetCheck(true,false);
				m_localNetModeDlg.Show(true,false);
			}
			break;
		case PAGE_WIRELESS:
			{
				m_rdWireless.SetCheck(true,false);
				m_wirelessModeDlg.Show(true,false);
			}
			break;
		default:
			assert(false);
			break;
		}

		if( bRefresh )
		{
			Repaint();
		}
	
}
bool CCfgNetworkExDlg::ShowData(NCFG_ITEM_ID cfgID, const BYTE* pData, int len, bool bRefresh)
{
	assert(NULL != pData);
	assert(len > 0);
	GUI::CListCtrl& list = m_cfgNetworkList;

	BYTE* pTempData = NULL;
	unsigned long tempDataLen = 0;

	switch (cfgID)
	{
	case NCFG_ITEM_HTTP_PORT:
		{
			assert( len == sizeof(unsigned long));
			const unsigned long* pValue = reinterpret_cast<const unsigned long*>(pData);
			GUI::CNumEditCtrl* pEdit = dynamic_cast<GUI::CNumEditCtrl*>( list.GetItemWnd(ROW_HTTP_PORT, 1) );
			assert( pEdit );
			pEdit->SetValue( *pValue, bRefresh );
		}
		return true;
		break;
	case NCFG_ITEM_SERVER_PORT:
		{
			assert( len == sizeof(unsigned long));
			const unsigned long* pValue = reinterpret_cast<const unsigned long*>(pData);
			GUI::CNumEditCtrl* pEdit = dynamic_cast<GUI::CNumEditCtrl*>( list.GetItemWnd(ROW_SERVER_PORT, 1) );
			assert( pEdit );
			pEdit->SetValue( *pValue, bRefresh );
		}
		return true;
		break;
	
		
	case NCFG_ITEM_NETWORK_ADDR_INFO:
		{
			m_datainfo.cfgID =cfgID;
			assert( len == sizeof(NETWORK_ADDR_INFO) );
			const NETWORK_ADDR_INFO* pValue = reinterpret_cast<const NETWORK_ADDR_INFO*>(pData);
			m_datainfo.network_info = *pValue;
			RefreshView(false);
		}
		return true;
		break;
	case NCFG_ITEM_DNS_1:
		{
			m_datainfo.cfgID =cfgID;
			assert( len == sizeof(unsigned long) );
			const unsigned long* pUlong1 = reinterpret_cast<const unsigned long*>(pData);
			m_datainfo.pUlong1 = *pUlong1;
			RefreshView(false);
		}
		return true;
		break;
	case NCFG_ITEM_DNS_2:
		{
			m_datainfo.cfgID =cfgID;
			assert( len == sizeof(unsigned long) );
			const unsigned long* pUlong2 = reinterpret_cast<const unsigned long*>(pData);
			m_datainfo.pUlong2 = *pUlong2;	
			RefreshView(false);
		}
		return true;
		break;
	case NCFG_ITEM_PPPOE_INFO:
		{
			m_datainfo.cfgID =cfgID;
			assert( len == sizeof(PPPOE_INFO) );
			CCfgDlg::GetConfig(NCFG_ITEM_PPPOE_USER_NAME_MAX_LEN,&pTempData, tempDataLen);
			assert(tempDataLen==sizeof(unsigned long));
			unsigned long maxUserLen = *reinterpret_cast<unsigned long*>(pTempData);

			CCfgDlg::GetConfig(NCFG_ITEM_PPPOE_PASSWORD_MAX_LEN,&pTempData,tempDataLen);
			assert(tempDataLen==sizeof(unsigned long));
			unsigned long maxPassLen = *reinterpret_cast<unsigned long*>(pTempData);
		
			const PPPOE_INFO* pPPPOE = reinterpret_cast<const PPPOE_INFO*>(pData);
			m_datainfo.maxUserLen =maxUserLen;
			m_datainfo.maxPassLen =maxPassLen;
			m_datainfo.pPPPOE = *pPPPOE;
			RefreshView(false);

		}
		return true;
		break;
	case NCFG_ITEM_SWITCH_INFO:
		{
			m_datainfo.cfgID =cfgID;
			if(m_poeNum > 0)
			{
				const NET_SWITCH_INFO *pSwitchInfo = reinterpret_cast<const NET_SWITCH_INFO*>(pData);
				m_datainfo.switchIP = pSwitchInfo->switchIP;
				m_datainfo.netMask = pSwitchInfo->netMask;
				RefreshView(false);
			}
		}
		return true;
	break;
	case NCFG_ITEM_NET_WIRELESS:
		{
			m_datainfo.cfgID =cfgID;
			assert(len == sizeof(MDVR_WIRELESS_CFG));
			const MDVR_WIRELESS_CFG *pValue = reinterpret_cast<const MDVR_WIRELESS_CFG*>(pData);
			m_datainfo.mdvr_wireless = *pValue;
			RefreshView(false);
		}
		return true;
		break;	
	case NCFG_ITEM_USE_3G:
		{
			assert(len == sizeof(unsigned long));
			const unsigned long* pPage =reinterpret_cast<const unsigned long*>(pData);
			if(*pPage == 0)//本地ip设置
			{
				SetPage(PAGE_NETWORK,false);
			}
			else if(*pPage == 1)
			{
				SetPage(PAGE_WIRELESS,false);
			}
			
			return true;
			break;	
		}
	default:
		break;
	}

	return false;
}

bool CCfgNetworkExDlg::SetData(NCFG_ITEM_ID cfgID, BYTE*& pData, int& len)  const
{
	assert(NULL != pData);
	assert(len > 0);
	const GUI::CListCtrl& list = m_cfgNetworkList;

	switch (cfgID)
	{
	case NCFG_ITEM_HTTP_PORT:
		{
			assert( len == sizeof(unsigned long));
			unsigned long* pValue = reinterpret_cast<unsigned long*>(pData);
			const GUI::CNumEditCtrl* pEdit = dynamic_cast<const GUI::CNumEditCtrl*>( list.GetItemWnd(ROW_HTTP_PORT, 1) );
			assert( pEdit );
			*pValue = pEdit->GetValue();
		}
		return true;
		break;
	case NCFG_ITEM_SERVER_PORT:
		{
			assert( len == sizeof(unsigned long));
			unsigned long* pValue = reinterpret_cast<unsigned long*>(pData);
			const GUI::CNumEditCtrl* pEdit = dynamic_cast<const GUI::CNumEditCtrl*>( list.GetItemWnd(ROW_SERVER_PORT, 1) );
			assert( pEdit );
			*pValue = pEdit->GetValue();
		}
		return true;
		break;
	case NCFG_ITEM_NETWORK_ADDR_INFO:
		{
			assert( len == sizeof(NETWORK_ADDR_INFO) );
			NETWORK_ADDR_INFO* pValue = reinterpret_cast<NETWORK_ADDR_INFO*>(pData);
			*pValue =m_datainfo.network_info;
			
		}
		return true;
		break;
	case NCFG_ITEM_DNS_1:
		{
			assert( len == sizeof(unsigned long) );
			unsigned long* pUlongLong = reinterpret_cast<unsigned long*>(pData);
			*pUlongLong =m_datainfo.pUlong1;
		}
		return true;
		break;
	case NCFG_ITEM_DNS_2:
		{
			assert( len == sizeof(unsigned long) );
			unsigned long* pUlongLong = reinterpret_cast<unsigned long*>(pData);
			*pUlongLong = m_datainfo.pUlong2;
		}
		return true;
		break;
	case NCFG_ITEM_PPPOE_INFO:
		{
			assert( len == sizeof(PPPOE_INFO) );
			PPPOE_INFO* pValue = reinterpret_cast<PPPOE_INFO*>(pData);
			*pValue = m_datainfo.pPPPOE;
		}
		return true;
		break;
	case NCFG_ITEM_SWITCH_INFO:
		{
			assert( len == sizeof(NET_SWITCH_INFO) );
			NET_SWITCH_INFO* pValue = reinterpret_cast<NET_SWITCH_INFO*>(pData);
			pValue->switchIP = m_datainfo.switchIP;
			pValue->netMask = m_datainfo.netMask;
		}
		return true;
		break;
	case NCFG_ITEM_NET_WIRELESS:
		{
			assert(len == sizeof(MDVR_WIRELESS_CFG));
			MDVR_WIRELESS_CFG *pValue = reinterpret_cast<MDVR_WIRELESS_CFG*>(pData);
			*pValue =m_datainfo.mdvr_wireless;

		}
		return true;
		break;
	case NCFG_ITEM_USE_3G:
		{
			assert(len == sizeof(unsigned long));
			unsigned long *pValue =reinterpret_cast<unsigned long*>(pData);
			if(m_curPage == PAGE_NETWORK)
			{
				*pValue =0;
			}
			else if(m_curPage == PAGE_WIRELESS)
			{
				*pValue =1;
			}
			else
			{
				assert(false);
			}		
		}	
		return true;
		break;	
	default:
		break;
	}

	return false;
}

//刷新数据到界面
void CCfgNetworkExDlg::RefreshView(bool bRefresh)
{
	m_localNetModeDlg.SetDataInfo(m_datainfo);
	m_localNetModeDlg.RefreshView(false);
	m_wirelessModeDlg.SetDataInfo(m_datainfo);

	m_wirelessModeDlg.RefreshView(false);
	if( bRefresh )
	{
		Repaint();
	}
}

void  CCfgNetworkExDlg::OnModifyNetModeData(unsigned long cfgId)
{
	if( (!IsModify()))
	{
		switch(cfgId)
		{
			case NCFG_ITEM_NETWORK_ADDR_INFO:
			case NCFG_ITEM_DNS_1:
			case NCFG_ITEM_DNS_2:
			case NCFG_ITEM_PPPOE_INFO:
			case NCFG_ITEM_SWITCH_INFO:
			case NCFG_ITEM_NET_WIRELESS:
			// case NCFG_ITEM_USE_3G:
				SetModify(true);
				break;
			default:
				break;
		}
		
	}

}
unsigned long CCfgNetworkExDlg::CheckValue()
{
	int ret = 0;
	if(m_curPage ==PAGE_NETWORK)
	{
		ret =m_localNetModeDlg.CheckValue();
		if(ret!=0)
		{
			return ret;
		}
	}
	else if(m_curPage == PAGE_WIRELESS)
	{
		ret =m_wirelessModeDlg.CheckValue();
		if(ret!=0)
		{
			return ret;
		}
	}
	else
	{
		assert(false);
	}
	//如果改变了HTTP或者SERVER的端口，需要检查端口是否有效。
	//端口无效的情况为：1两个端口相同 2:使用了系统默认的端口
	if(IsChange(NCFG_ITEM_HTTP_PORT) || IsChange(NCFG_ITEM_SERVER_PORT)) 
	{
		unsigned long httpPort = dynamic_cast<const GUI::CNumEditCtrl*>( m_cfgNetworkList.GetItemWnd(ROW_HTTP_PORT, 1) )->GetValue();
		unsigned long serverPort = dynamic_cast<const GUI::CNumEditCtrl*>( m_cfgNetworkList.GetItemWnd(ROW_SERVER_PORT, 1) )->GetValue();
		if( (httpPort == serverPort))
		{
			ret =E_STR_ID_MESSAGE_28;
		}
	}
	return ret;
	//检查httpport
}

BEGIN_MSG_MAP(CCfgNetworkExDlg, CCfgDlg)
	ON_MSG(m_rdNetWork.GetID(),KEY_VIRTUAL_ENTER,OnClickRadio)
	ON_MSG(m_rdWireless.GetID(),KEY_VIRTUAL_ENTER,OnClickRadio)
	ON_MSG_EX(m_cfgNetworkList.GetID(), KEY_VIRTUAL_MSG_DEFAULT_TAG, OnClickList)
END_MSG_MAP()
unsigned long CCfgNetworkExDlg::OnClickRadio()
{
	CheckUI(true);
	return KEY_VIRTUAL_MSG_CONTINUE;
}

unsigned long CCfgNetworkExDlg::OnClickList(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	unsigned short row = (keyID >> 16)&0x7fff;	//行
	unsigned short col = keyID&0xffff;			//列
	if (!IsModify())
	{
		switch (row)
		{
		case ROW_HTTP_PORT:
			if (IsChange(NCFG_ITEM_HTTP_PORT))
			{
				SetModify(true);
			}
			return KEY_VIRTUAL_MSG_CONTINUE;
			break;
		case ROW_SERVER_PORT:
			if(IsChange( NCFG_ITEM_SERVER_PORT ))
			{
				SetModify(true);
			}
			return KEY_VIRTUAL_MSG_CONTINUE;
			break;
		default:
			break;
		}
	}
	return KEY_VIRTUAL_MSG_STOP;
}
