#include "CfgNetworkNatDlg.h"


CCfgNetworkNatDlg::CCfgNetworkNatDlg()
{

}

CCfgNetworkNatDlg::~CCfgNetworkNatDlg()
{

}

void CCfgNetworkNatDlg::OnInitial()
{
	CCfgDlg::OnInitial();
	//////////////////////////////////////////////////////////////////////////
	GUI::CListCtrl& list = m_cfgNatList ;
	GUI::CWnd * pWnd = NULL;

	GUI::CEditCtrl*  pEdit = NULL;
	GUI::CCheckCtrl* pCheck = NULL;
	GUI::CButton *pButton = NULL;
	GUI::CNumEditCtrl* pNumEdit = NULL;
	GUI::CComboCtrl  *pCombo = NULL;

	unsigned short x = m_length[DLG_OFFSET_SUB_LEFT], y = m_length[DLG_OFFSET_SUB_TOP];
	unsigned short width = m_rect.Width() - 2 * m_length[DLG_OFFSET_SUB_LEFT];
	unsigned short height = m_rect.Height() - y - m_length[DLG_OFFSET_SUB_BOTTOM];

	unsigned short offsetEdge = 0;
	int maxListRow = height /m_length[LIST_ITEM_HEIGHT]; //List框带头
	if( ROW_COUNT > maxListRow )
	{
		offsetEdge = m_length[SCROLL_WIDTH] + 2;
	}

	list.Create(GUI::CWnd::GetRandWndID(), x, y, width, height, this, 0, 0, false);
	list.AddColumn(NULL, width/2);
	list.AddColumn(NULL, width/2 - offsetEdge );

	list.AddItem(ROW_NAT_ENABLE, 0, m_pStrTable->String(E_STR_ID_NET_ENABLE));
	pCheck = dynamic_cast<GUI::CCheckCtrl*>(list.AddItem(ROW_NAT_ENABLE, 1, WND_TYPE_CHECK));
	assert(pCheck);

#if !defined(__CUSTOM_US02__)
	list.AddItem(ROW_NAT_SERVER_IP, 0, m_pStrTable->String(E_STR_ID_NET_SERVER));
	pEdit = dynamic_cast<GUI::CEditCtrl*>(list.AddItem(ROW_NAT_SERVER_IP, 1, WND_TYPE_EDIT));
	pEdit->SetMaxStringLen(67);
	assert(pEdit);

#if defined(__CUSTOM_CN__) || defined(__CUSTOM_SHANGHAI__) || defined(__CUSTOM_IN__) || defined(__CUSTOM_CNB__)
	pEdit->SetCaption("www.autonat.com", false);
	pEdit->SetEnable(false, false);
#elif defined(__CUSTOM_IL03__)
	pEdit->SetCaption("www.provisionisr-nat.com", false);
	pEdit->SetEnable(false, false);
#else
	pEdit->SetCaption("www.autonat.com", false);
	pEdit->SetEnable(false, false);
#endif

#endif

#if !defined(__CUSTOM_CN__) && !defined(__CUSTOM_SHANGHAI__) && !defined(__CUSTOM_IN__) && !defined(__CUSTOM_CNB__)
	list.AddItem(ROW_NAT_SERVER_PORT, 0, m_pStrTable->String(E_STR_ID_PORT));
	pNumEdit = dynamic_cast<GUI::CNumEditCtrl*>(list.AddItem(ROW_NAT_SERVER_PORT, 1, WND_TYPE_NUM_EDIT));
	pNumEdit->SetEnable(false, false);

	assert(pNumEdit);
	pNumEdit->SetMaxStringLen( 5 );
	pNumEdit->SetRange(0,65535);
#endif

	list.CreateListFocusMap();
}


void CCfgNetworkNatDlg::DoOther()
{
	/*BYTE* pTempData = NULL;
	unsigned long tempDataLen = 0;
	int count = 0;
	GUI::CComboCtrl* pCombo = NULL;
	unsigned long *pTempValue = NULL;
	GUI::CListCtrl& list = m_cfgNatList;
	char str[128] = {0};

	CCfgDlg::GetConfig( NCFG_ITEM_EMAIL_IMG_NUM_RANGE, &pTempData, tempDataLen);
	assert(tempDataLen > 0);
	count = tempDataLen/sizeof(unsigned long);	
	pTempValue = reinterpret_cast<unsigned long *>(pTempData);
	pCombo = dynamic_cast<GUI::CComboCtrl*>(list.GetItemWnd(ROW_IMG_NUM, 1));
	assert(pCombo);
	pCombo->RemoveAllItem(false);			
	for (int i = 0; i < count; ++i)
	{
	if (0 == pTempValue[i])
	{
	snprintf(str, 128, "%s", m_pStrTable->String(E_STR_ID_IMG_NONE));
	}
	else
	{
	snprintf(str, 128, "%d", pTempValue[i]);
	}
	pCombo->AddItem(str, pTempValue[i]);					
	}
	pCombo->SetCurItem(0, false);*/


	//CCfgDlg::GetConfig( NCFG_ITEM_EMAIL_SNAP_TIME_RANGE, &pTempData, tempDataLen);
	//assert(tempDataLen > 0);
	//count = tempDataLen/sizeof(unsigned long);
	//pTempValue = reinterpret_cast<unsigned long *>(pTempData);	
	//pCombo = dynamic_cast<GUI::CComboCtrl*>(list.GetItemWnd(ROW_SNAP_TIME, 1));
	//assert(pCombo);
	//pCombo->RemoveAllItem(false);			
	//for (int i = 0; i < count; ++i)
	//{
	//	sprintf(str, "%d", pTempValue[i]);
	//	pCombo->AddItem(str, pTempValue[i]);					
	//}
	//pCombo->SetCurItem(0, false);
}

void CCfgNetworkNatDlg::EndOther()
{

}

bool CCfgNetworkNatDlg::ShowData(NCFG_ITEM_ID cfgID, const BYTE* pData, int len, bool bRefresh)
{
	assert(NULL != pData);
	assert(len > 0);

	switch (cfgID)
	{
	case NCFG_ITEM_NAT_SERVER:	
		{
			assert(len == sizeof(NAT_SERVER_INFO));
			const NAT_SERVER_INFO *pNatServerInfo = reinterpret_cast<const NAT_SERVER_INFO*>(pData);

			GUI::CCheckCtrl * pCheck = dynamic_cast<GUI::CCheckCtrl*>(m_cfgNatList.GetItemWnd(ROW_NAT_ENABLE,1));
			pCheck->SetCheck(pNatServerInfo->bStartNat,bRefresh);

#if !defined(__CUSTOM_US02__)
			GUI::CEditCtrl * pEdit = dynamic_cast<GUI::CEditCtrl*>(m_cfgNatList.GetItemWnd(ROW_NAT_SERVER_IP,1));
			assert( NULL != pEdit );
#endif

#if !defined(__CUSTOM_CN__) && !defined(__CUSTOM_SHANGHAI__) && !defined(__CUSTOM_IN__) && !defined(__CUSTOM_CNB__)
			GUI::CNumEditCtrl* pNumPort = dynamic_cast<GUI::CNumEditCtrl*>(m_cfgNatList.GetItemWnd(ROW_NAT_SERVER_PORT,1));
			assert(NULL!=pNumPort);
			pNumPort->SetValue( pNatServerInfo->iServerPort, bRefresh );
#endif
		}
		return true;
		break;
	default:
		return false;
		break;
	}

	return false;
}

bool CCfgNetworkNatDlg::SetData(NCFG_ITEM_ID cfgID, BYTE*& pData, int& len)  const
{
	assert(NULL != pData);
	assert(len > 0);

	switch (cfgID)
	{
	case NCFG_ITEM_NAT_SERVER:
		{			
			assert(len == (sizeof(NAT_SERVER_INFO)));
			NAT_SERVER_INFO *pNatServerInfo = reinterpret_cast<NAT_SERVER_INFO*>(pData);

			GUI::CCheckCtrl * pCheck =  dynamic_cast<GUI::CCheckCtrl*>((GUI::CCheckCtrl*)m_cfgNatList.GetItemWnd(ROW_NAT_ENABLE,1));
			pNatServerInfo->bStartNat = pCheck->IsCheck();

#if !defined(__CUSTOM_US02__)
			GUI::CEditCtrl * pEdit = dynamic_cast<GUI::CEditCtrl*>((GUI::CEditCtrl*)m_cfgNatList.GetItemWnd(ROW_NAT_SERVER_IP,1));
			assert( NULL != pEdit );
#endif

#if !defined(__CUSTOM_CN__) && !defined(__CUSTOM_SHANGHAI__) && !defined(__CUSTOM_IN__) && !defined(__CUSTOM_CNB__)
			GUI::CNumEditCtrl* pNumPort = dynamic_cast<GUI::CNumEditCtrl*>((GUI::CNumEditCtrl*)m_cfgNatList.GetItemWnd(ROW_NAT_SERVER_PORT,1));
			assert(pNumPort); 	
			pNatServerInfo->iServerPort = pNumPort->GetValue();
#endif
		}
		return true;
		break;
	default:
		return false;
		break;
	}
	return false;
}

BEGIN_MSG_MAP(CCfgNetworkNatDlg, CCfgDlg)
	ON_MSG_EX(m_cfgNatList .GetID(), KEY_VIRTUAL_MSG_DEFAULT_TAG, OnClickList)
END_MSG_MAP()

unsigned long CCfgNetworkNatDlg::OnClickList(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{

	unsigned short row = (keyID >> 16)&0x7fff;	//行
	unsigned short col = keyID&0xffff;			//列

	if (!IsModify())
	{
		switch (row)
		{	
		case ROW_NAT_ENABLE:
			{
				GUI::CCheckCtrl * pCheck = dynamic_cast<GUI::CCheckCtrl*>(m_cfgNatList.GetItemWnd(ROW_NAT_ENABLE,1));
				if(pCheck->IsCheck())
				{
					if( DLG_OK == MessageBox(m_pStrTable->String(E_STR_ID_MESSAGE_140), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK | DEFAULT_BTN_CANCEL| DEFAULT_ICON_WARNING) )
					{
						pCheck->SetCheck(true,false);
					}
					else
					{
						pCheck->SetCheck(false,true);
					}
				}
				if(IsChange(NCFG_ITEM_NAT_SERVER))
				{
					SetModify(true);
				}
			}
			break;
#if !defined(__CUSTOM_US02__)
		case ROW_NAT_SERVER_IP:
#endif
		case ROW_NAT_SERVER_PORT:
			if(IsChange(NCFG_ITEM_NAT_SERVER))
			{
				SetModify(true);
			}
			break;
		default:		
			break;
		}
		return KEY_VIRTUAL_MSG_CONTINUE;
	}
	else
	{
		return KEY_VIRTUAL_MSG_STOP;
	}
}
