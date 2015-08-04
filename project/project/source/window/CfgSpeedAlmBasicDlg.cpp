
#include "CfgSpeedAlmBasicDlg.h"


CCfgSpeedAlmBasicDlg::CCfgSpeedAlmBasicDlg()
{

}

CCfgSpeedAlmBasicDlg::~CCfgSpeedAlmBasicDlg()
{

}

void CCfgSpeedAlmBasicDlg::OnInitial()
{
	CCfgDlg::OnInitial();
	//////////////////////////////////////////////////////////////////////////
	GUI::CListCtrl& list = m_cfgSpeedAlmBasicList;
	GUI::CWnd * pWnd = NULL;
	GUI::CComboCtrl* pCombo = NULL;
	GUI::CEditCtrl*  pEdit = NULL;
	GUI::CCheckCtrl* pCheck = NULL;

	unsigned short x = m_length[DLG_OFFSET_SUB_LEFT], y = m_length[DLG_OFFSET_SUB_TOP];
	unsigned short width = m_rect.Width() - 2 * m_length[DLG_OFFSET_SUB_LEFT];
	unsigned short height = m_rect.Height() - y - m_length[DLG_OFFSET_SUB_BOTTOM];

	list.Create(GUI::CWnd::GetRandWndID(), x, y, width, height, this, 0, 0, false);
	list.AddColumn(NULL, width/2);
	list.AddColumn(NULL, width/2 - 4 );

	////
	//char str[100];

	list.AddItem(ROW_SPEED_SOURCE, 0, m_pStrTable->String(E_STR_ID_SPEED_SOURCE));
	pCombo = dynamic_cast<GUI::CComboCtrl*>(list.AddItem(ROW_SPEED_SOURCE, 1, WND_TYPE_COMBO));
	assert(pCombo);
	pCombo->AddItem(m_pStrTable->String(E_STR_ID_SPEED_FOR_GPS), SPEED_SOURCE_GPS);
	pCombo->AddItem(m_pStrTable->String(E_STR_ID_SPEED_FOR_CAR), SPEED_SOURCE_CAR);
	pCombo->SetCurItem( 0, false );

	list.AddItem(ROW_SPEED_UNIT, 0, m_pStrTable->String(E_STR_ID_SPEED_UNIT));
	pCombo = dynamic_cast<GUI::CComboCtrl*>(list.AddItem(ROW_SPEED_UNIT, 1, WND_TYPE_COMBO));
	assert(pCombo);
	pCombo->AddItem(m_pStrTable->String(E_STR_ID_SPEED_UNIT_KM), SPEED_UNIT_KM_H);
	pCombo->AddItem(m_pStrTable->String(E_STR_ID_SPEED_UNIT_MN), SPEED_UNIT_MN_H);
	pCombo->SetCurItem( 0, false );

	list.AddItem(ROW_HIGH_SPEED_CHECK, 0, m_pStrTable->String(E_STR_ID_HIGH_SPEED));
	pCheck = dynamic_cast<GUI::CCheckCtrl*>(list.AddItem(ROW_HIGH_SPEED_CHECK, 1, WND_TYPE_CHECK));
	assert(pCheck);
	
	list.AddItem(ROW_HIGH_SPEED_VALUE, 0, m_pStrTable->String(E_STR_ID_SPEED_VALUE));
	pEdit = dynamic_cast<GUI::CEditCtrl*>(list.AddItem(ROW_HIGH_SPEED_VALUE, 1, WND_TYPE_EDIT));
	assert( pEdit );

	list.AddItem(ROW_LOW_SPEED_CHECK, 0, m_pStrTable->String(E_STR_ID_LOW_SPEED));
	pCheck = dynamic_cast<GUI::CCheckCtrl*>(list.AddItem(ROW_LOW_SPEED_CHECK, 1, WND_TYPE_CHECK));
	assert(pCheck);


	list.AddItem(ROW_LOW_SPEED_VALUE, 0, m_pStrTable->String(E_STR_ID_SPEED_VALUE));
	pEdit = dynamic_cast<GUI::CEditCtrl*>(list.AddItem(ROW_LOW_SPEED_VALUE, 1, WND_TYPE_EDIT));
	assert( pEdit );


	list.CreateListFocusMap();

}

/*
unsigned long CCfgSpeedAlmBasicDlg::PreCheckValue()
{
if(IsChange(NCFG_ITEM_VIDEO_FORMAT)) 
{
return E_STR_ID_TIP_01_37;
}

if(IsChange(NCFG_ITEM_VGA_RESOLUTION))
{
return E_STR_ID_TIP_01_40;
}

//[2010-03-02] 现在修改语言选项不需要提示重启
//if(IsChange(NCFG_ITEM_LANGUAGE))
//{
//	return E_STR_ID_TIP_01_41;
//}

return 0;
}

void CCfgSpeedAlmBasicDlg::SetTip()
{
//设置显示静态tip
GUI::CListCtrl& list = m_cfgSystemList;
AddTip(list.GetItemWnd(ROW_DEVICE_ID,1)->GetID(),E_STR_ID_TIP_01_36);
AddTip(list.GetItemWnd(ROW_VIDEO_FORMAT,1)->GetID(),E_STR_ID_TIP_01_37);
//AddTip(list.GetItemWnd(ROW_AUTHORIZATION,1)->GetID(),E_STR_ID_TIP_01_38);
//AddTip(list.GetItemWnd(ROW_LIVE_TIME_ENABLE,1)->GetID(),E_STR_ID_TIP_01_39);
AddTip(list.GetItemWnd(ROW_VGA_RESOLUTION,1)->GetID(),E_STR_ID_TIP_01_40);
//AddTip(list.GetItemWnd(ROW_LANGUAGE,1)->GetID(),E_STR_ID_TIP_01_41);
}

*/


bool CCfgSpeedAlmBasicDlg::ShowData(NCFG_ITEM_ID cfgID, const BYTE* pData, int len, bool bRefresh)
{
	assert(NULL != pData);
	assert(len > 0);

	BYTE* pTempData = NULL;
	unsigned long tempDataLen = 0;

	switch (cfgID)
	{
		case NCFG_ITEM_SPEED_SOURCE:
		{
			assert(len == sizeof(unsigned long));
			const unsigned long* pValue = reinterpret_cast<const unsigned long*>(pData);

			GUI::CComboCtrl* pCombo = dynamic_cast<GUI::CComboCtrl*>( m_cfgSpeedAlmBasicList.GetItemWnd( ROW_SPEED_SOURCE, 1 ) );
			assert(NULL != pCombo );
			pCombo->SetCurItemData( *pValue, bRefresh );
		}
			return true;
			break;
		case NCFG_ITEM_SPEED_UNIT:
		{
			assert(len == sizeof(unsigned long));
			const unsigned long* pValue = reinterpret_cast<const unsigned long*>(pData);

			GUI::CComboCtrl* pCombo = dynamic_cast<GUI::CComboCtrl*>( m_cfgSpeedAlmBasicList.GetItemWnd( ROW_SPEED_UNIT, 1 ) );
			assert(NULL != pCombo );
			pCombo->SetCurItemData( *pValue, bRefresh );
		}
			return true;
			break;
		case NCFG_ITEM_HIGH_SPEED_ENABLE:
		{
			assert(len == sizeof(unsigned long));
			const unsigned long* pValue = reinterpret_cast<const unsigned long*>(pData);

			GUI::CCheckCtrl* pCheck = dynamic_cast<GUI::CCheckCtrl*>( m_cfgSpeedAlmBasicList.GetItemWnd( ROW_HIGH_SPEED_CHECK, 1 ) );
			assert(NULL != pCheck );
			pCheck->SetCheck( *pValue, bRefresh );
		}
			return true;
			break;
		case NCFG_ITEM_HIGH_SPEED_VALUE:
		{
			assert(len == sizeof(MDVR_SPEED));

			CCfgDlg::GetConfig( NCFG_ITEM_SPEED_VALUE_MUX_LEN, &pTempData, tempDataLen );

			assert( tempDataLen == sizeof(unsigned long) );
			unsigned long max_name_len = *(reinterpret_cast<unsigned long*>(pTempData));

			const MDVR_SPEED* pShortName = reinterpret_cast<const MDVR_SPEED*>(pData);

			GUI::CEditCtrl* pEdit = dynamic_cast<GUI::CEditCtrl*>( m_cfgSpeedAlmBasicList.GetItemWnd( ROW_HIGH_SPEED_VALUE, 1 ) );
			assert(NULL != pEdit );
			pEdit->SetMaxStringLen( max_name_len);
			pEdit->SetCaption( pShortName->speed, bRefresh );
		}
			return true;
			break;
		case NCFG_ITEM_LOW_SPEED_ENABLE:
		{
			assert(len == sizeof(unsigned long));
			const unsigned long* pValue = reinterpret_cast<const unsigned long*>(pData);

			GUI::CCheckCtrl* pCheck = dynamic_cast<GUI::CCheckCtrl*>( m_cfgSpeedAlmBasicList.GetItemWnd( ROW_LOW_SPEED_CHECK, 1 ) );
			assert(NULL != pCheck );
			pCheck->SetCheck( *pValue, bRefresh );
		}	
			return true;
			break;
		case NCFG_ITEM_LOW_SPEED_VALUE:
		{
			assert(len == sizeof(MDVR_SPEED));

			CCfgDlg::GetConfig( NCFG_ITEM_SPEED_VALUE_MUX_LEN, &pTempData, tempDataLen );

			assert( tempDataLen == sizeof(unsigned long) );
			unsigned long max_name_len = *(reinterpret_cast<unsigned long*>(pTempData));

			const MDVR_SPEED* pShortName = reinterpret_cast<const MDVR_SPEED*>(pData);

			GUI::CEditCtrl* pEdit = dynamic_cast<GUI::CEditCtrl*>( m_cfgSpeedAlmBasicList.GetItemWnd( ROW_LOW_SPEED_VALUE, 1 ) );
			assert(NULL != pEdit );
			pEdit->SetMaxStringLen( max_name_len);
			pEdit->SetCaption( pShortName->speed, bRefresh );
		}	
			return true;
			break;

		default:
			return false;
			break;
	}
	return false;
}

bool CCfgSpeedAlmBasicDlg::SetData(NCFG_ITEM_ID cfgID, BYTE*& pData, int& len)  const
{
	assert(NULL != pData);
	assert(len > 0);

	switch (cfgID)
	{
		case NCFG_ITEM_SPEED_SOURCE:
		{
			assert(len == sizeof(unsigned long));
			unsigned long* pValue = reinterpret_cast<unsigned long*>(pData);

			const GUI::CComboCtrl* pCombo = dynamic_cast<const GUI::CComboCtrl*>( m_cfgSpeedAlmBasicList.GetItemWnd( ROW_SPEED_SOURCE, 1) );
			assert(NULL != pCombo );
			*pValue = static_cast<unsigned long>(pCombo->GetCurItemData());

		}
			return true;
			break;
		case NCFG_ITEM_SPEED_UNIT:
		{
			assert(len == sizeof(unsigned long));
			unsigned long* pValue = reinterpret_cast<unsigned long*>(pData);

			const GUI::CComboCtrl* pCombo = dynamic_cast<const GUI::CComboCtrl*>( m_cfgSpeedAlmBasicList.GetItemWnd( ROW_SPEED_UNIT, 1) );
			assert(NULL != pCombo );
			*pValue = static_cast<unsigned long>(pCombo->GetCurItemData());
		}
			return true;
			break;
		case NCFG_ITEM_HIGH_SPEED_ENABLE:
		{
			assert(len == sizeof(unsigned long));
			unsigned long* pValue = reinterpret_cast<unsigned long*>(pData);

			const GUI::CCheckCtrl* pCheck = dynamic_cast<const GUI::CCheckCtrl*>( m_cfgSpeedAlmBasicList.GetItemWnd( ROW_HIGH_SPEED_CHECK, 1 ) );
			assert(NULL != pCheck );

			if (pCheck->IsCheck())
			{
				*pValue = 1;
			} 
			else
			{
				*pValue = 0;
			}
	
		}
			return true;
			break;
		case NCFG_ITEM_HIGH_SPEED_VALUE:
		{
			assert(len == sizeof(MDVR_SPEED));
			MDVR_SPEED* pShortName = reinterpret_cast<MDVR_SPEED*>(pData);

			const GUI::CEditCtrl* pEdit = dynamic_cast<const GUI::CEditCtrl*>( m_cfgSpeedAlmBasicList.GetItemWnd( ROW_HIGH_SPEED_VALUE, 1 ) );
			assert(NULL != pEdit );
			strcpy(pShortName->speed, pEdit->GetCaption().c_str());			
		}
			return true;
			break;
		case NCFG_ITEM_LOW_SPEED_ENABLE:
		{
			assert(len == sizeof(unsigned long));
			unsigned long* pValue = reinterpret_cast<unsigned long*>(pData);

			const GUI::CCheckCtrl* pCheck = dynamic_cast<const GUI::CCheckCtrl*>( m_cfgSpeedAlmBasicList.GetItemWnd( ROW_LOW_SPEED_CHECK, 1 ) );
			assert(NULL != pCheck );

			if (pCheck->IsCheck())
			{
				*pValue = 1;
			} 
			else
			{
				*pValue = 0;
			}			
		}
			return true;
			break;
		case NCFG_ITEM_LOW_SPEED_VALUE:
		{
			assert(len == sizeof(MDVR_SPEED));
			MDVR_SPEED* pShortName = reinterpret_cast<MDVR_SPEED*>(pData);

			const GUI::CEditCtrl* pEdit = dynamic_cast<const GUI::CEditCtrl*>( m_cfgSpeedAlmBasicList.GetItemWnd( ROW_LOW_SPEED_VALUE, 1 ) );
			assert(NULL != pEdit );
			strcpy(pShortName->speed, pEdit->GetCaption().c_str());			
		}
			return true;
			break;
		default:
		return false;
		break;
	}
	return false;
}


BEGIN_MSG_MAP(CCfgSpeedAlmBasicDlg, CCfgDlg)
	ON_MSG_EX(m_cfgSpeedAlmBasicList.GetID(), KEY_VIRTUAL_MSG_DEFAULT_TAG, OnClickList)
END_MSG_MAP()

unsigned long CCfgSpeedAlmBasicDlg::OnClickList(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	if (!IsModify())
	{
		unsigned short row = (keyID >> 16)&0x7fff;	//行
		unsigned short col = keyID&0xffff;			//列

		switch (row)
		{	
		case ROW_SPEED_SOURCE:
			if(IsChange(NCFG_ITEM_SPEED_SOURCE))
			{
				SetModify(true);
			}
			break;
		case ROW_SPEED_UNIT:
			if(IsChange(NCFG_ITEM_SPEED_UNIT))
			{
				SetModify(true);
			}
			break;
		case ROW_HIGH_SPEED_CHECK:
			if(IsChange(NCFG_ITEM_HIGH_SPEED_ENABLE))
			{
				SetModify(true);
			}
			break;
		case ROW_HIGH_SPEED_VALUE:
			if(IsChange(NCFG_ITEM_HIGH_SPEED_VALUE))
			{
				SetModify(true);
			}
			break;
		case ROW_LOW_SPEED_CHECK:
			if(IsChange(NCFG_ITEM_LOW_SPEED_ENABLE))
			{
				SetModify(true);
			}
			break;
		case ROW_LOW_SPEED_VALUE:
			if(IsChange(NCFG_ITEM_LOW_SPEED_VALUE))
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
