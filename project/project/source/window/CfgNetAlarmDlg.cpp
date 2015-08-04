/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : hz
** Date         : 2009-10-14
** Name         : CfgNetAlarmDlg.cpp 
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/
#include "CfgNetAlarmDlg.h"
#include "CfgTriggerDlg.h"

//////////////////////////////////////////////////////////////////////////

CCfgNetAlarmManDlg::CCfgNetAlarmManDlg()
{

}

CCfgNetAlarmManDlg::~CCfgNetAlarmManDlg()
{

}

void CCfgNetAlarmManDlg::OnPreInitial()
{
	AddCaption(m_pStrTable->String(E_STR_ID_TITLE_NET_ALARM));
	AddCaption(m_pStrTable->String(E_STR_ID_NET_ALARM));
	AddCaption(m_pStrTable->String(E_STR_ID_ALARM_HANDING));
	AddCaption(m_pStrTable->String(E_STR_ID_SCHEDULE));

	CCfgManDlg3<CCfgNetAlarmDlg,CCfgNetAlarmHandingDlg,CCfgScheduleChildDlg>::OnPreInitial();

	GetPage3().SetMaxChnn(s_loginSuccessReply.productInfo.localVideoInputNum, s_loginSuccessReply.productInfo.localVideoInputNum);   
	//GetPage3().SetMaxChnn(s_loginSuccessReply.productInfo.videoInputNum, s_loginSuccessReply.productInfo.videoInputNum);     xian
	GetPage3().SetScheduleCfgID(NCFG_ITEM_NET_ALARM_SCH_WEEK, NCFG_ITEM_NET_ALARM_SCH_HOLIDAY);
}

void CCfgNetAlarmManDlg::GetItemList(CFGITEM_LIST& itemList) const
{
	itemList.push_back(NCFG_ITEM_NET_ALARM_SCH_WEEK);

	itemList.push_back(NCFG_ITEM_SENSOR_IN_ENABLE);
	itemList.push_back(NCFG_ITEM_SENSOR_IN_TYPE);
	itemList.push_back(NCFG_ITEM_SENSOR_IN_NAME);

	itemList.push_back(NCFG_ITEM_SENSOR_IN_HOLD_TIME);
	itemList.push_back(NCFG_ITEM_SENSOR_IN_BUZZ);
	itemList.push_back(NCFG_ITEM_SENSOR_IN_TO_ALARM_OUT);
	itemList.push_back(NCFG_ITEM_SENSOR_IN_TO_RECORD);

	if(s_loginSuccessReply.productInfo.videoInputNum > 0)
	{
		itemList.push_back(NCFG_ITEM_SENSOR_IN_TO_PTZ);
	}

	itemList.push_back(NCFG_ITEM_NET_ALARM_SCH_HOLIDAY);

	//
	itemList.push_back(NCFG_ITEM_SENSOR_IN_NAME_MAX_LEN);
}

//////////////////////////////////////////////////////////////////////////

CCfgNetAlarmDlg::CCfgNetAlarmDlg()
{

}

CCfgNetAlarmDlg::~CCfgNetAlarmDlg()
{

}

//////////////////////////////////////////////////////////////////////////
void CCfgNetAlarmDlg::OnInitial()
{
	CCfgDlg::OnInitial();
	//////////////////////////////////////////////////////////////////////////

	unsigned short x = m_length[DLG_OFFSET_SUB_LEFT], y = m_length[DLG_OFFSET_SUB_TOP];
	//int height = 340;
	//int height = m_length[LIST_ITEM_HEIGHT] * 7;
	unsigned short width = m_rect.Width() - 2 * m_length[DLG_OFFSET_SUB_LEFT];
	unsigned short height = m_rect.Height() - y - 2 * m_length[LIST_ITEM_HEIGHT] - m_length[DLG_OFFSET_SUB_BOTTOM];

	unsigned short offsetEdge = 0;
	int maxListRow = (height - m_length[LIST_HEAD_HEIGHT])/m_length[LIST_ITEM_HEIGHT]; //List框带头
	int numChnn = CCfgDlg::s_loginSuccessReply.productInfo.sensorInputNum;
	if( numChnn > maxListRow )
	{
		offsetEdge = m_length[SCROLL_WIDTH] + 2;
	}

	m_listCtrl.Create(CTRLID_CFG_SENSOR_LIST, x, y, width, height, this, 0, 0);
	m_listCtrl.AddColumn("", width * 2 / 32);
	m_listCtrl.AddColumn(m_pStrTable->String(E_STR_ID_ENABLE), width * 4 / 32);
	m_listCtrl.AddColumn(m_pStrTable->String(E_STR_ID_IP_ADDRESS), width * 10 / 32);
	m_listCtrl.AddColumn(m_pStrTable->String(E_STR_ID_NAME), width * 16 / 32 - offsetEdge);

	char str[32] = {0};
	GUI::CWnd * pWnd = NULL;
	GUI::CComboCtrl * pCombo = NULL;
	for (int ch = 0; ch < numChnn; ++ch)
	{
		sprintf(str, "%d", ch + 1);
		m_listCtrl.AddItem(ch, COL_CHANNEL, str);

		pWnd = m_listCtrl.AddItem(ch, COL_ENABLE, WND_TYPE_CHECK);

		GUI::CIPAddressCtrl* pIP = reinterpret_cast<GUI::CIPAddressCtrl*>(m_listCtrl.AddItem(ch, COL_IP, WND_TYPE_IP));
		assert(pIP);
		pIP->SetIP(192,168,1,ch+1,false);

		pWnd = m_listCtrl.AddItem(ch, COL_NAME, WND_TYPE_EDIT);
		pWnd->SetCaption(str, false);
	}

	m_listCtrl.CreateListFocusMap();

	GUI::CRect rect = m_listCtrl.GetClientRect();
	y += rect.Height() + 4;
	
	m_scAll.Create( GUI::CWnd::GetRandWndID(), x, y + (m_length[LIST_ITEM_HEIGHT] - m_length[STATIC_HEIGHT])/2, width, m_length[STATIC_HEIGHT], this );
	m_scAll.SetCaption( m_pStrTable->String(E_STR_ID_ALL), false );

	y += m_length[LIST_ITEM_HEIGHT];
	m_listAll.Create( GUI::CWnd::GetRandWndID(), x, y, width, m_length[LIST_ITEM_HEIGHT], this, 0, 1, false );
	m_listAll.AddColumn("", width * 2 / 32);
	m_listAll.AddColumn(m_pStrTable->String(E_STR_ID_ENABLE), width * 4 / 32);
	m_listAll.AddColumn(m_pStrTable->String(E_STR_ID_IP_ADDRESS), width * 10 / 32);
	m_listAll.AddColumn(m_pStrTable->String(E_STR_ID_NAME), width * 16 / 32 - 2);

	{
		int ch = 0;
		pWnd = m_listAll.AddItem(ch, COL_CHANNEL, WND_TYPE_CHECK);

		pWnd = m_listAll.AddItem(ch, COL_ENABLE, WND_TYPE_CHECK);

		pWnd = m_listAll.AddItem(ch, COL_IP, WND_TYPE_IP);
		(reinterpret_cast<GUI::CIPAddressCtrl*>(pWnd))->SetIP(192,168,1,1,false);
	}
	m_listAll.CreateListFocusMap();
}
void CCfgNetAlarmDlg::UpdateView(bool bRefresh)
{
	GUI::CCheckCtrl* pCheckAll = dynamic_cast<GUI::CCheckCtrl*>(m_listAll.GetItemWnd(0, COL_CHANNEL));
	if( pCheckAll )
	{
		pCheckAll->SetCheck( false, bRefresh );
	}

	CCfgDlg::UpdateView( bRefresh );
}


bool CCfgNetAlarmDlg::ShowData(NCFG_ITEM_ID cfgID, const BYTE* pData, int len, bool bRefresh)
{
	assert(NULL != pData);
	assert(len > 0);

	BYTE* pTempData = NULL;
	unsigned long tempDataLen = 0;

	switch (cfgID)
	{
	case NCFG_ITEM_SENSOR_IN_ENABLE:
		{
			assert(len == sizeof(unsigned long) * s_loginSuccessReply.productInfo.sensorInputNum);
			const unsigned long* pValue = reinterpret_cast<const unsigned long*>(pData);
			
			GUI::CCheckCtrl* pCtrl = NULL;
			for (int ch = 0; ch < s_loginSuccessReply.productInfo.sensorInputNum; ++ch, ++pValue)
			{
				pCtrl = dynamic_cast<GUI::CCheckCtrl*>(m_listCtrl.GetItemWnd(ch, COL_ENABLE));
				assert( NULL != pCtrl );
				pCtrl->SetCheck(*pValue, bRefresh);
			}
			
		}
		return true;
		break;
	//case NCFG_ITEM_SENSOR_IN_TYPE:
	//	{
	//		assert(len == sizeof(unsigned long) * s_loginSuccessReply.productInfo.sensorInputNum);
	//		const unsigned long* pValue = reinterpret_cast<const unsigned long*>(pData);

	//		GUI::CComboCtrl* pCtrl = NULL;
	//		for (int ch = 0; ch < s_loginSuccessReply.productInfo.sensorInputNum; ++ch, ++pValue)
	//		{
	//			pCtrl = dynamic_cast<GUI::CComboCtrl*>( m_listCtrl.GetItemWnd(ch, COL_TYPE));
	//			assert(NULL != pCtrl);

	//			if (!pCtrl->SetCurItemData(static_cast<LONGLONG>(*pValue), bRefresh))
	//			{
	//				assert(false);
	//			}
	//		}
	//	}
	//	return true;
	//	break;
	case NCFG_ITEM_SENSOR_IN_NAME:
		{
			assert(len == sizeof(NCFG_INFO_SHORT_NAME) * s_loginSuccessReply.productInfo.sensorInputNum);
			CCfgDlg::GetConfig(NCFG_ITEM_SENSOR_IN_NAME_MAX_LEN,&pTempData,tempDataLen);
			assert(tempDataLen==sizeof(unsigned long));
			unsigned long maxName = *reinterpret_cast<unsigned long*>(pTempData);
			const NCFG_INFO_SHORT_NAME* pValue = reinterpret_cast<const NCFG_INFO_SHORT_NAME*>(pData);

			GUI::CEditCtrl* pCtrl = NULL;
			for (int ch = 0; ch < s_loginSuccessReply.productInfo.sensorInputNum; ++ch, ++pValue)
			{
				pCtrl = dynamic_cast<GUI::CEditCtrl*>( m_listCtrl.GetItemWnd(ch, COL_NAME));
				assert(NULL != pCtrl );
				pCtrl->SetMaxStringLen(maxName);
				pCtrl->SetCaption(pValue->name, bRefresh);
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

bool CCfgNetAlarmDlg::SetData(NCFG_ITEM_ID cfgID, BYTE*& pData, int& len)  const
{
	assert(NULL != pData);
	assert(len > 0);

	const GUI::CListCtrl& listAll = m_listAll;

	bool bAll = false;
	{
		const GUI::CCheckCtrl* pCheck = dynamic_cast<const GUI::CCheckCtrl*>( listAll.GetItemWnd( 0, COL_CHANNEL ) );
		assert( pCheck );
		bAll = pCheck->IsCheck();
	}

	switch (cfgID)
	{
	case NCFG_ITEM_SENSOR_IN_ENABLE:
		{
			assert(len == sizeof(unsigned long) * s_loginSuccessReply.productInfo.sensorInputNum);
			unsigned long* pValue = reinterpret_cast<unsigned long*>(pData);

			const GUI::CCheckCtrl* pCtrl = NULL;
			for (int ch = 0; ch < s_loginSuccessReply.productInfo.sensorInputNum; ++ch, ++pValue)
			{
				if( bAll )
				{
					pCtrl = dynamic_cast<const GUI::CCheckCtrl*>( listAll.GetItemWnd(0, COL_ENABLE) );
				}else{
					pCtrl = dynamic_cast<const GUI::CCheckCtrl*>( m_listCtrl.GetItemWnd(ch, COL_ENABLE) );
				}
				assert( NULL != pCtrl );
				*pValue = pCtrl->IsCheck() ? 1 : 0;
			}
		}
		return true;
		break;
	//case NCFG_ITEM_SENSOR_IN_TYPE:
	//	{
	//		assert(len == sizeof(unsigned long) * s_loginSuccessReply.productInfo.sensorInputNum);
	//		unsigned long* pValue = reinterpret_cast<unsigned long*>(pData);

	//		const GUI::CComboCtrl* pCtrl = NULL;
	//		for (int ch = 0; ch < s_loginSuccessReply.productInfo.sensorInputNum; ++ch, ++pValue)
	//		{
	//			if( bAll )
	//			{
	//				pCtrl = dynamic_cast<const GUI::CComboCtrl*>( listAll.GetItemWnd(0, COL_TYPE) );
	//			}else{
	//				pCtrl = dynamic_cast<const GUI::CComboCtrl*>( m_listCtrl.GetItemWnd(ch, COL_TYPE) );
	//			}
	//			assert(NULL != pCtrl );

	//			int sel = pCtrl->GetCurItem();
	//			assert(sel >= 0);
	//			*pValue = pCtrl->GetItemData(sel);
	//		}
	//	}
	//	return true;
	//	break;
	case NCFG_ITEM_SENSOR_IN_NAME:
		{
			assert(len == sizeof(NCFG_INFO_SHORT_NAME) * s_loginSuccessReply.productInfo.sensorInputNum);
			NCFG_INFO_SHORT_NAME* pValue = reinterpret_cast<NCFG_INFO_SHORT_NAME*>(pData);

			const GUI::CEditCtrl* pCtrl = NULL;
			for (int ch = 0; ch < s_loginSuccessReply.productInfo.sensorInputNum; ++ch, ++pValue)
			{
				pCtrl = dynamic_cast<const GUI::CEditCtrl*>( m_listCtrl.GetItemWnd(ch, COL_NAME) );
				assert(NULL != pCtrl);
				strcpy(pValue->name, pCtrl->GetCaption().c_str());
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



BEGIN_MSG_MAP(CCfgNetAlarmDlg, CCfgDlg)
	ON_MSG_EX(CTRLID_CFG_SENSOR_LIST, KEY_VIRTUAL_MSG_DEFAULT_TAG, OnClickList)
	ON_MSG_EX(m_listAll.GetID(), KEY_VIRTUAL_MSG_DEFAULT_TAG, OnClickListAll)
END_MSG_MAP()

unsigned long CCfgNetAlarmDlg::OnClickList(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	if (!IsModify())
	{
		unsigned short row = (keyID >> 16)&0x7fff;	//行
		unsigned short col = keyID&0xffff;			//列

		switch (col)
		{
		case COL_ENABLE:
			if (IsChange(NCFG_ITEM_SENSOR_IN_ENABLE))
			{
				SetModify(true);
			}
			break;
		//case COL_TYPE:
		//	if (IsChange(NCFG_ITEM_SENSOR_IN_TYPE))
		//	{
		//		SetModify(true);
		//	}
		//	break;
		case COL_IP:
			break;
		case COL_NAME:
			if (IsChange(NCFG_ITEM_SENSOR_IN_NAME))
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

unsigned long CCfgNetAlarmDlg::OnClickListAll(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	unsigned short row = static_cast<unsigned short>((keyID >> 16)&0x7fff);
	unsigned short col = static_cast<unsigned short>(keyID&0xffff);

	GUI::CCheckCtrl* pCheckAll = dynamic_cast<GUI::CCheckCtrl*>(m_listAll.GetItemWnd(0, COL_CHANNEL));
	CCfgDlg* pCfgDlg = dynamic_cast<CCfgDlg*>( this->GetParent() );
	assert( pCheckAll );
	assert( pCfgDlg );
	if( pCheckAll->IsCheck() )
	{
		pCfgDlg->SetRefreshView( true );
		if (!IsModify())
		{
			if( COL_ENABLE == col )
			{
				SetModify(true);
				return KEY_VIRTUAL_MSG_CONTINUE;
			}
		}
	}else{
		pCfgDlg->SetRefreshView( false );
	}

	// 点选All 直接认为做了修改
	if (!IsModify())
	{
		if( COL_CHANNEL == col )
		{
			SetModify(true);
			return KEY_VIRTUAL_MSG_CONTINUE;
		}
	}
	return KEY_VIRTUAL_MSG_STOP;
}

