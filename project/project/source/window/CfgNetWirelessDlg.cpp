
#include "CfgNetWirelessDlg.h"


CCfgNetWirelessDlg::CCfgNetWirelessDlg()
{

}

CCfgNetWirelessDlg::~CCfgNetWirelessDlg()
{

}

void CCfgNetWirelessDlg::OnInitial()
{
	CCfgDlg::OnInitial();
	//////////////////////////////////////////////////////////////////////////
	GUI::CListCtrl& list = m_cfgNetWirelessList;
	GUI::CWnd * pWnd = NULL;
	GUI::CComboCtrl* pCombo = NULL;
	GUI::CCheckCtrl* pCheck = NULL;

	unsigned short x = m_length[DLG_OFFSET_SUB_LEFT], y = m_length[DLG_OFFSET_SUB_TOP];
	unsigned short width = m_rect.Width() - 2 * m_length[DLG_OFFSET_SUB_LEFT];
	unsigned short height = m_rect.Height() - y - m_length[DLG_OFFSET_SUB_BOTTOM];

	list.Create(GUI::CWnd::GetRandWndID(), x, y, width, height, this, 0, 0, false);
	list.AddColumn(NULL, width/2);
	list.AddColumn(NULL, width/2 - 4 );


	list.AddItem(ROW_WIRELESS_ENABLE, 0, m_pStrTable->String(E_STR_ID_WIRELESS_ENABLE));
	pCheck = dynamic_cast<GUI::CCheckCtrl*>(list.AddItem(ROW_WIRELESS_ENABLE, 1, WND_TYPE_CHECK));
	assert(pCheck);

	list.AddItem(ROW_WIRELESS_TYPE, 0, m_pStrTable->String(E_STR_ID_WIRELESS_TYPE));
	pCombo = dynamic_cast<GUI::CComboCtrl*>(list.AddItem(ROW_WIRELESS_TYPE, 1, WND_TYPE_COMBO));
	assert(pCombo);
	pCombo->AddItem(m_pStrTable->String(E_STR_ID_TYPE_EVDO), WIRELESS_TYPE_EVDO);
	//pCombo->AddItem(m_pStrTable->String(E_STR_ID_TYPE_GPRS), WIRELESS_TYPE_GPRS);
	pCombo->SetCurItem( 0, false );

	list.CreateListFocusMap();

}


bool CCfgNetWirelessDlg::ShowData(NCFG_ITEM_ID cfgID, const BYTE* pData, int len, bool bRefresh)
{

	assert(NULL != pData);
	assert(len > 0);

	switch (cfgID)
	{
	case NCFG_ITEM_WIRELESS_COMM:
		{
			assert(len == sizeof(MDVR_WIRELESS_CFG));
			const MDVR_WIRELESS_CFG *pValue = reinterpret_cast<const MDVR_WIRELESS_CFG*>(pData);
			
			GUI::CCheckCtrl* pCheck = NULL;
			GUI::CComboCtrl* pCombo = NULL;
			
			pCheck = dynamic_cast<GUI::CCheckCtrl*>(m_cfgNetWirelessList.GetItemWnd(ROW_WIRELESS_ENABLE,1));
			assert( NULL != pCheck );
			pCheck->SetCheck(pValue->enable,bRefresh);

			pCombo = dynamic_cast<GUI::CComboCtrl*>(m_cfgNetWirelessList.GetItemWnd(ROW_WIRELESS_TYPE,1));
			assert(NULL!= pCombo);
			pCombo->SetCurItemData(pValue->type,bRefresh);
		}
		return true;
		break;
	default:
		return false;
		break;
	}
	return false;
}


bool CCfgNetWirelessDlg::SetData(NCFG_ITEM_ID cfgID, BYTE*& pData, int& len)  const
{
	assert(NULL != pData);
	assert(len > 0);
	
	const GUI::CListCtrl& list = m_cfgNetWirelessList;

	switch (cfgID)
	{
	case NCFG_ITEM_WIRELESS_COMM:
		{
			assert(len == sizeof(MDVR_WIRELESS_CFG));
			MDVR_WIRELESS_CFG *pValue = reinterpret_cast<MDVR_WIRELESS_CFG*>(pData);

			GUI::CCheckCtrl* pCheck = NULL;
			GUI::CComboCtrl* pCbType = NULL;
			
			pCheck = dynamic_cast<GUI::CCheckCtrl*>((GUI::CCheckCtrl*)list.GetItemWnd(ROW_WIRELESS_ENABLE,1));
			pCbType = dynamic_cast<GUI::CComboCtrl*>((GUI::CComboCtrl*)list.GetItemWnd(ROW_WIRELESS_TYPE,1));

			assert(pCheck);
			assert(pCbType);

			pValue->enable = pCheck->IsCheck()?1:0;
			pValue->type = pCbType->GetCurItemData();
		}
		return true;
		break;
	default:
		return false;
		break;
	}
	return false;
}


BEGIN_MSG_MAP(CCfgNetWirelessDlg, CCfgDlg)
	ON_MSG_EX(m_cfgNetWirelessList.GetID(), KEY_VIRTUAL_MSG_DEFAULT_TAG, OnClickList)
END_MSG_MAP()

unsigned long CCfgNetWirelessDlg::OnClickList(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	unsigned short row = (keyID >> 16)&0x7fff;	//ÐÐ
	unsigned short col = keyID&0xffff;			//ÁÐ

	if (!IsModify())
	{

		switch (row)
		{
		case ROW_WIRELESS_ENABLE:
		case ROW_WIRELESS_TYPE:
			if (IsChange(NCFG_ITEM_WIRELESS_COMM))
			{
				SetModify(true);
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
