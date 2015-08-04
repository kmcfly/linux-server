#include "CfgNetPhoneDlg.h"

CCfgNetPhoneDlg::CCfgNetPhoneDlg()
{

}
CCfgNetPhoneDlg::~CCfgNetPhoneDlg()
{

}

void CCfgNetPhoneDlg::OnInitial()
{
	CCfgDlg::OnInitial();
	//////////////////////////////////////////////////////////////////////////
	GUI::CListCtrl& list = m_cfgPhoneList ;
	GUI::CWnd * pWnd = NULL;

	GUI::CEditCtrl*  pEdit = NULL;
	GUI::CCheckCtrl* pCheck = NULL;
	GUI::CButton *pButton = NULL;
	GUI::CNumEditCtrl* pNumEdit = NULL;
	GUI::CComboCtrl  *pCombo = NULL;
	GUI::CIPAddressCtrl *pIP =NULL;

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


	list.AddItem(ROW_PHONE_IP, 0, "Phone Service IP");
	pIP = dynamic_cast<GUI::CIPAddressCtrl*>(list.AddItem(ROW_PHONE_IP, 1, WND_TYPE_IP));
	assert(pIP);

	list.AddItem(ROW_PHONE_PORT, 0, m_pStrTable->String(E_STR_ID_PORT));
	pNumEdit = dynamic_cast<GUI::CNumEditCtrl*>(list.AddItem(ROW_PHONE_PORT, 1, WND_TYPE_NUM_EDIT));
	assert(pNumEdit);
	pNumEdit->SetMaxStringLen( 5 );
	pNumEdit->SetRange(0,65535);

	list.CreateListFocusMap();
}


bool CCfgNetPhoneDlg::ShowData(NCFG_ITEM_ID cfgID, const BYTE* pData, int len, bool bRefresh)
{
	assert(NULL != pData);
	assert(len > 0);
	GUI::CListCtrl& list = m_cfgPhoneList;

	BYTE* pTempData = NULL;
	unsigned long tempDataLen = 0;

	switch (cfgID)
	{
	case NCFG_ITEM_HRSST_SERVER_IP:
		{
			assert( len == sizeof(unsigned long) );
			const unsigned long* pUlong = reinterpret_cast<const unsigned long*>(pData);
			GUI::CIPAddressCtrl* pIp = dynamic_cast<GUI::CIPAddressCtrl*>( list.GetItemWnd(ROW_PHONE_IP, 1) );
			assert( pIp );
			pIp->SetIP( *pUlong, bRefresh );
		}
		return true;
		break;
	case NCFG_ITEM_HRSST_SERVER_PORT:
		{
			assert( len == sizeof(unsigned long));
			const unsigned long* pValue = reinterpret_cast<const unsigned long*>(pData);
			GUI::CNumEditCtrl* pEdit = dynamic_cast<GUI::CNumEditCtrl*>( list.GetItemWnd(ROW_PHONE_PORT, 1) );
			assert( pEdit );
			pEdit->SetValue( *pValue, bRefresh );
		}
		return true;
		break;
	default:
		break;
	}

	return false;
}



bool CCfgNetPhoneDlg::SetData(NCFG_ITEM_ID cfgID, BYTE*& pData, int& len)  const
{
	assert(NULL != pData);
	assert(len > 0);
	const GUI::CListCtrl& list = m_cfgPhoneList;

	switch (cfgID)
	{
	case NCFG_ITEM_HRSST_SERVER_IP:
		{
			assert( len == sizeof(unsigned long) );
			unsigned long* pUlongLong = reinterpret_cast<unsigned long*>(pData);
			const GUI::CIPAddressCtrl* pIp = dynamic_cast<const GUI::CIPAddressCtrl*>( list.GetItemWnd(ROW_PHONE_IP, 1) );
			assert( pIp );
			*pUlongLong = pIp->GetIP();
		}
		return true;
		break;
	case NCFG_ITEM_HRSST_SERVER_PORT:
		{
			assert( len == sizeof(unsigned long));
			unsigned long* pValue = reinterpret_cast<unsigned long*>(pData);
			const GUI::CNumEditCtrl* pEdit = dynamic_cast<const GUI::CNumEditCtrl*>( list.GetItemWnd(ROW_PHONE_PORT, 1) );
			assert( pEdit );
			*pValue = pEdit->GetValue();			
		}
		return true;
		break;
	default:
		break;
	}

	return false;
}


BEGIN_MSG_MAP(CCfgNetPhoneDlg, GUI::CDialog)
	ON_MSG_EX(m_cfgPhoneList.GetID(), KEY_VIRTUAL_MSG_DEFAULT_TAG, OnClickList)
END_MSG_MAP()


unsigned long CCfgNetPhoneDlg::OnClickList(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	unsigned short row = (keyID >> 16)&0x7fff;	//行
	unsigned short col = keyID&0xffff;			//列

	if (!IsModify())
	{
		switch (row)
		{
		case ROW_PHONE_IP:
			if (IsChange(NCFG_ITEM_HRSST_SERVER_IP))
			{
				SetModify(true);
			}
			return KEY_VIRTUAL_MSG_CONTINUE;
			break;
		case ROW_PHONE_PORT:
			if(IsChange( NCFG_ITEM_HRSST_SERVER_PORT ))
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
