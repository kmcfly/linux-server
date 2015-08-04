

#include "CfgDriverInfoDlg.h"


CCfgDriverInfoDlg::CCfgDriverInfoDlg()
{

}

CCfgDriverInfoDlg::~CCfgDriverInfoDlg()
{

}

void CCfgDriverInfoDlg::OnInitial()
{
	CCfgDlg::OnInitial();
	//////////////////////////////////////////////////////////////////////////
	GUI::CListCtrl& list = m_cfgDriverInfoList;
	GUI::CWnd * pWnd = NULL;
	GUI::CComboCtrl* pCombo = NULL;
	GUI::CEditCtrl*  pEdit = NULL;

	unsigned short x = m_length[DLG_OFFSET_SUB_LEFT], y = m_length[DLG_OFFSET_SUB_TOP];
	unsigned short width = m_rect.Width() - 2 * m_length[DLG_OFFSET_SUB_LEFT];
	unsigned short height = m_rect.Height() - y - m_length[DLG_OFFSET_SUB_BOTTOM];

	list.Create(GUI::CWnd::GetRandWndID(), x, y, width, height, this, 0, 0, false);
	list.AddColumn(NULL, width/2);
	list.AddColumn(NULL, width/2 - 4 );

	////
	char str[100];

	list.AddItem(ROW_DRIVER_NAME, 0, m_pStrTable->String(E_STR_ID_DRIVER_NAME));
	pEdit = dynamic_cast<GUI::CEditCtrl*>(list.AddItem(ROW_DRIVER_NAME, 1, WND_TYPE_EDIT));
	assert( pEdit );

	list.AddItem(ROW_DEVICE_NUM, 0, m_pStrTable->String(E_STR_ID_DRIVER_NUM));
	pEdit = dynamic_cast<GUI::CEditCtrl*>(list.AddItem(ROW_DEVICE_NUM, 1, WND_TYPE_EDIT));
	assert( pEdit );

	list.AddItem(ROW_COMPANY_NAME, 0, m_pStrTable->String(E_STR_ID_COMPANY_NAME));
	pEdit = dynamic_cast<GUI::CEditCtrl*>(list.AddItem(ROW_COMPANY_NAME, 1, WND_TYPE_EDIT));
	assert( pEdit );

	list.AddItem(ROW_SHUTDOWNDELAY, 0, m_pStrTable->String(E_STR_ID_SHUTDOWN_DELAY));
	pCombo = dynamic_cast<GUI::CComboCtrl*>(list.AddItem(ROW_SHUTDOWNDELAY, 1, WND_TYPE_COMBO));
	assert(pCombo);
	m_DelayTime = 60;
	for (int i = 10;i <= m_DelayTime; )
	{
		sprintf(str,"%d",i);
		pCombo->AddItem(str,i);
		i+=10;
	}	
	pCombo->SetCurItemData( m_DelayTime, false );
	//pCombo->SetMaxStringLen( 32 );

	list.CreateListFocusMap();

}

bool CCfgDriverInfoDlg::ShowData(NCFG_ITEM_ID cfgID, const BYTE* pData, int len, bool bRefresh)
{
	assert(NULL != pData);
	assert(len > 0);

	BYTE* pTempData = NULL;
	unsigned long tempDataLen = 0;

	switch (cfgID)
	{

	case NCFG_ITEM_COMPANY_NAME:
		{
			assert(len == sizeof(NCFG_INFO_LONG_NAME));

			CCfgDlg::GetConfig( NCFG_ITEM_DEVICE_NAME_MAX_LEN, &pTempData, tempDataLen );
			assert( tempDataLen == sizeof(unsigned long) );
			unsigned long max_name_len = *(reinterpret_cast<unsigned long*>(pTempData));

			const NCFG_INFO_LONG_NAME* pShortName = reinterpret_cast<const NCFG_INFO_LONG_NAME*>(pData);

			GUI::CEditCtrl* pEdit = dynamic_cast<GUI::CEditCtrl*>( m_cfgDriverInfoList.GetItemWnd( ROW_COMPANY_NAME, 1 ) );
			assert(NULL != pEdit );
			pEdit->SetMaxStringLen( max_name_len);
			pEdit->SetCaption( pShortName->name, bRefresh );
		}
		return true;
		break;
	case NCFG_ITEM_DRIVER_NAME:
		{
			//assert(len == sizeof(NCFG_INFO_SHORT_NAME));

			CCfgDlg::GetConfig( NCFG_ITEM_DEVICE_NAME_MAX_LEN, &pTempData, tempDataLen );
			assert( tempDataLen == sizeof(unsigned long) );
			unsigned long max_name_len = *(reinterpret_cast<unsigned long*>(pTempData));

			const NCFG_INFO_SHORT_NAME* pShortName = reinterpret_cast<const NCFG_INFO_SHORT_NAME*>(pData);

			GUI::CEditCtrl* pEdit = dynamic_cast<GUI::CEditCtrl*>( m_cfgDriverInfoList.GetItemWnd( ROW_DRIVER_NAME, 1 ) );
			assert(NULL != pEdit );
			pEdit->SetMaxStringLen( max_name_len);
			pEdit->SetCaption( pShortName->name, bRefresh );
		}
		return true;
		break;
	case NCFG_ITEM_CAR_NUMBER:
		{
			//assert(len == sizeof(NCFG_INFO_SHORT_NAME));

			CCfgDlg::GetConfig( NCFG_ITEM_DEVICE_NAME_MAX_LEN, &pTempData, tempDataLen );
			assert( tempDataLen == sizeof(unsigned long) );
			unsigned long max_name_len = *(reinterpret_cast<unsigned long*>(pTempData));

			const NCFG_INFO_SHORT_NAME* pShortName = reinterpret_cast<const NCFG_INFO_SHORT_NAME*>(pData);

			GUI::CEditCtrl* pEdit = dynamic_cast<GUI::CEditCtrl*>( m_cfgDriverInfoList.GetItemWnd( ROW_DEVICE_NUM, 1 ) );
			assert(NULL != pEdit );
			pEdit->SetMaxStringLen( max_name_len);
			pEdit->SetCaption( pShortName->name, bRefresh );
		}
		return true;
		break;
	case NCFG_ITEM_SHUT_DOWN_DELAY:
		{
			assert(len == sizeof(unsigned long));
			const unsigned long* pLong = reinterpret_cast<const unsigned long*>(pData);

			GUI::CComboCtrl* pCombo = dynamic_cast<GUI::CComboCtrl*>( m_cfgDriverInfoList.GetItemWnd( ROW_SHUTDOWNDELAY, 1 ) );
			assert(NULL != pCombo );
			pCombo->SetCurItemData( *pLong, bRefresh );
		}
		return true;
		break;
	default:
		return false;
		break;
	}
	return false;
}

bool CCfgDriverInfoDlg::SetData(NCFG_ITEM_ID cfgID, BYTE*& pData, int& len)  const
{
	assert(NULL != pData);
	assert(len > 0);

	switch (cfgID)
	{
	case NCFG_ITEM_COMPANY_NAME:
		{
			assert(len == sizeof(NCFG_INFO_LONG_NAME));
			NCFG_INFO_LONG_NAME* pShortName = reinterpret_cast<NCFG_INFO_LONG_NAME*>(pData);

			const GUI::CEditCtrl* pEdit = dynamic_cast<const GUI::CEditCtrl*>( m_cfgDriverInfoList.GetItemWnd( ROW_COMPANY_NAME, 1 ) );
			assert(NULL != pEdit );
			strcpy(pShortName->name, pEdit->GetCaption().c_str());		
		}
		return true;
		break;
	case NCFG_ITEM_DRIVER_NAME:
		{
			assert(len == sizeof(NCFG_INFO_SHORT_NAME));
			NCFG_INFO_SHORT_NAME* pShortName = reinterpret_cast<NCFG_INFO_SHORT_NAME*>(pData);

			const GUI::CEditCtrl* pEdit = dynamic_cast<const GUI::CEditCtrl*>( m_cfgDriverInfoList.GetItemWnd( ROW_DRIVER_NAME, 1 ) );
			assert(NULL != pEdit );
			strcpy(pShortName->name, pEdit->GetCaption().c_str());		
		}
		return true;
		break;
	case NCFG_ITEM_CAR_NUMBER:
		{
			assert(len == sizeof(NCFG_INFO_SHORT_NAME));
			NCFG_INFO_SHORT_NAME* pShortName = reinterpret_cast<NCFG_INFO_SHORT_NAME*>(pData);

			const GUI::CEditCtrl* pEdit = dynamic_cast<const GUI::CEditCtrl*>( m_cfgDriverInfoList.GetItemWnd( ROW_DEVICE_NUM, 1 ) );
			assert(NULL != pEdit );
			strcpy(pShortName->name, pEdit->GetCaption().c_str());			
		}
		return true;
		break;

	case NCFG_ITEM_SHUT_DOWN_DELAY:
		{
			assert(len == sizeof(unsigned long));
			unsigned long* pValue = reinterpret_cast<unsigned long*>(pData);

			const GUI::CComboCtrl* pCombo = dynamic_cast<const GUI::CComboCtrl*>( m_cfgDriverInfoList.GetItemWnd( ROW_SHUTDOWNDELAY, 1) );
			assert(NULL != pCombo );
			*pValue = static_cast<unsigned long>(pCombo->GetCurItemData());
		}
		return true;
		break;

	default:
		return false;
		break;
	}
	return false;
}


BEGIN_MSG_MAP(CCfgDriverInfoDlg, CCfgDlg)
	ON_MSG_EX(m_cfgDriverInfoList.GetID(), KEY_VIRTUAL_MSG_DEFAULT_TAG, OnClickList)
END_MSG_MAP()

unsigned long CCfgDriverInfoDlg::OnClickList(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	if (!IsModify())
	{
		unsigned short row = (keyID >> 16)&0x7fff;	//ÐÐ
		unsigned short col = keyID&0xffff;			//ÁÐ

		switch (row)
		{
		case ROW_DRIVER_NAME:
			if (IsChange(NCFG_ITEM_DRIVER_NAME))
			{
				SetModify(true);
			}
			break;
		case ROW_DEVICE_NUM:
			if (IsChange(NCFG_ITEM_CAR_NUMBER))
			{
				SetModify(true);
			}
			break;
		case ROW_COMPANY_NAME:
			if (IsChange(NCFG_ITEM_COMPANY_NAME))
			{
				SetModify(true);
			}
			break;
		case ROW_SHUTDOWNDELAY:
			if (IsChange(NCFG_ITEM_SHUT_DOWN_DELAY))
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
