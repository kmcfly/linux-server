#include "CfgNetworkPushDlg.h"


CCfgNetworkPushDlg::CCfgNetworkPushDlg()
{

}

CCfgNetworkPushDlg::~CCfgNetworkPushDlg()
{

}

void CCfgNetworkPushDlg::OnInitial()
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

	list.AddItem(ROW_USE_PUSH, 0, m_pStrTable->String(E_STR_ID_PUSH_ENABLE));
	pCheck = dynamic_cast<GUI::CCheckCtrl*>(list.AddItem(ROW_USE_PUSH, 1, WND_TYPE_CHECK));
	assert(pCheck);

	// 	list.AddItem(ROW_PUSH_SERVER_PORT, 0, m_pStrTable->String(E_STR_ID_PORT));
	// 	pNumEdit = dynamic_cast<GUI::CNumEditCtrl*>(list.AddItem(ROW_PUSH_SERVER_PORT, 1, WND_TYPE_NUM_EDIT));
	// 	pNumEdit->SetEnable(false);
	// 
	// 	assert(pNumEdit);
	// 	pNumEdit->SetMaxStringLen( 5 );
	// 	pNumEdit->SetRange(0,65535);

	list.CreateListFocusMap();
}


void CCfgNetworkPushDlg::DoOther()
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

void CCfgNetworkPushDlg::EndOther()
{

}

bool CCfgNetworkPushDlg::ShowData(NCFG_ITEM_ID cfgID, const BYTE* pData, int len, bool bRefresh)
{
	assert(NULL != pData);
	assert(len > 0);

	switch (cfgID)
	{
	case NCFG_ITEM_PUSH_SERVER:	
		{
			assert(len == sizeof(PUSH_SERVER_INFO));
			const PUSH_SERVER_INFO *pNatServerInfo = reinterpret_cast<const PUSH_SERVER_INFO*>(pData);
// 
// 			GUI::CEditCtrl * pEdit = dynamic_cast<GUI::CEditCtrl*>(m_cfgNatList.GetItemWnd(ROW_PUSH_SERVER_IP,1));
// 			assert( NULL != pEdit );
// 
// 			pEdit->SetCaption(pNatServerInfo->szServerAddr, bRefresh);

// 			GUI::CNumEditCtrl* pNumPort = dynamic_cast<GUI::CNumEditCtrl*>(m_cfgNatList.GetItemWnd(ROW_PUSH_SERVER_PORT,1));
// 			assert(NULL!=pNumPort);
// 			pNumPort->SetValue( pNatServerInfo->iServerPort, bRefresh );
		}
		return true;
		break;
	case NCFG_ITEM_USE_PUSH:
		{
			const unsigned long * pUsePush = reinterpret_cast<const unsigned long *>(pData);

			GUI::CCheckCtrl * pCheck = dynamic_cast<GUI::CCheckCtrl*>(m_cfgNatList.GetItemWnd(ROW_USE_PUSH, 1));
			assert( NULL != pCheck );

			if (0 == * pUsePush)
			{
				pCheck->SetCheck(false, bRefresh);
			}
			else
			{
				pCheck->SetCheck(true, bRefresh);
			}
		}
		return true;
		break;
	default:
		return false;
		break;
	}

	return false;
}

bool CCfgNetworkPushDlg::SetData(NCFG_ITEM_ID cfgID, BYTE*& pData, int& len)  const
{
	assert(NULL != pData);
	assert(len > 0);

	switch (cfgID)
	{
	case NCFG_ITEM_PUSH_SERVER:
		{			
			assert(len == (sizeof(PUSH_SERVER_INFO)));
			PUSH_SERVER_INFO *pNatServerInfo = reinterpret_cast<PUSH_SERVER_INFO*>(pData);

// 			GUI::CEditCtrl * pEdit = dynamic_cast<GUI::CEditCtrl*>((GUI::CEditCtrl*)m_cfgNatList.GetItemWnd(ROW_PUSH_SERVER_IP,1));
// 			assert( NULL != pEdit );
// 			strcpy(pNatServerInfo->szServerAddr, pEdit->GetCaption().c_str());

// 			GUI::CNumEditCtrl* pNumPort = dynamic_cast<GUI::CNumEditCtrl*>((GUI::CNumEditCtrl*)m_cfgNatList.GetItemWnd(ROW_PUSH_SERVER_PORT,1));
// 			assert(pNumPort); 	
// 			pNatServerInfo->iServerPort = pNumPort->GetValue();
		}
		return true;
		break;
	case NCFG_ITEM_USE_PUSH:
		{
			assert(len == (sizeof(unsigned long)));
			unsigned long *pUseCheck = reinterpret_cast<unsigned long *>(pData);

			GUI::CCheckCtrl * pCheck = dynamic_cast<GUI::CCheckCtrl*>((GUI::CCheckCtrl*)m_cfgNatList.GetItemWnd(ROW_USE_PUSH,1));
			assert(NULL != pCheck);
			if (pCheck->IsCheck())
			{
				* pUseCheck = 1;
			}
			else
			{
				* pUseCheck = 0;
			}
		}
		return true;
		break;
	default:
		return false;
		break;
	}
	return false;
}

BEGIN_MSG_MAP(CCfgNetworkPushDlg, CCfgDlg)
	ON_MSG_EX(m_cfgNatList .GetID(), KEY_VIRTUAL_MSG_DEFAULT_TAG, OnClickList)
END_MSG_MAP()

unsigned long CCfgNetworkPushDlg::OnClickList(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{

	unsigned short row = (keyID >> 16)&0x7fff;	//行
	unsigned short col = keyID&0xffff;			//列

	if (!IsModify())
	{
		switch (row)
		{
		case ROW_USE_PUSH:
		case ROW_PUSH_SERVER_IP:
		case ROW_PUSH_SERVER_PORT:
			if(IsChange(NCFG_ITEM_PUSH_SERVER) || IsChange(NCFG_ITEM_USE_PUSH))
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
