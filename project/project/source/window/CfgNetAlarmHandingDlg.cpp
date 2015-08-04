/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : L
** Date         : 2009-10-14
** Name         : CfgNetAlarmHandingDlg.h 
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/
#include "CfgNetAlarmHandingDlg.h"

CCfgNetAlarmHandingDlg::CCfgNetAlarmHandingDlg() : m_pTriggerInfo(NULL), m_pAllTriggerInfo(NULL)
{

}

CCfgNetAlarmHandingDlg::~CCfgNetAlarmHandingDlg()
{
	if( m_pTriggerInfo )
	{
		delete [] m_pTriggerInfo;
		m_pTriggerInfo = NULL;
	}

	if( m_pAllTriggerInfo )
	{
		delete [] m_pAllTriggerInfo;
		m_pAllTriggerInfo = NULL;
	}
}

//////////////////////////////////////////////////////////////////////////
void CCfgNetAlarmHandingDlg::OnInitial()
{
	CCfgDlg::OnInitial();
	//////////////////////////////////////////////////////////////////////////

	unsigned short x = m_length[DLG_OFFSET_SUB_LEFT], y = m_length[DLG_OFFSET_SUB_TOP];
	//int height = 340;
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
	m_listCtrl.AddColumn("", width * 2 / 32 );
	m_listCtrl.AddColumn(m_pStrTable->String(E_STR_ID_HOLD_TIME), width * 12 / 32);
	m_listCtrl.AddColumn(m_pStrTable->String(E_STR_ID_TRIGGER), width * 18 / 32 - offsetEdge);

	char str[32] = {0};
	GUI::CWnd * pWnd = NULL;
	GUI::CComboCtrl * pCombo = NULL;
	for (int ch = 0; ch < numChnn; ++ch)
	{
		sprintf(str, "%d", ch + 1);
		m_listCtrl.AddItem(ch, COL_CHANNEL, str);

		pCombo = dynamic_cast<GUI::CComboCtrl*>(m_listCtrl.AddItem(ch, COL_HOLD_TIME, WND_TYPE_COMBO));
		assert(NULL != pCombo);
		pCombo->AddItem("5",5);
		pCombo->AddItem("10",10);
		pCombo->AddItem("20",20);
		pCombo->AddItem("30",30);
		pCombo->AddItem("40",40);
		pCombo->SetCurItem(0);

		pWnd = m_listCtrl.AddItem(ch, COL_TRIGGER, WND_TYPE_BUTTON, m_length[BTN_WIDTH], m_length[BTN_HEIGHT]);
		pWnd->SetCaption(m_pStrTable->String(E_STR_ID_SETTING), false);
	}

	m_listCtrl.CreateListFocusMap();

	GUI::CRect rect = m_listCtrl.GetClientRect();
	y += rect.Height() + 4;
	
	m_scAll.Create( GUI::CWnd::GetRandWndID(), x, y + (m_length[LIST_ITEM_HEIGHT]-m_length[STATIC_HEIGHT])/2, width, m_length[STATIC_HEIGHT], this );
	m_scAll.SetCaption( m_pStrTable->String(E_STR_ID_ALL), false );

	y += m_length[LIST_ITEM_HEIGHT];
	m_listAll.Create( GUI::CWnd::GetRandWndID(), x, y, width, m_length[LIST_ITEM_HEIGHT], this, 0, 1, false );
	m_listAll.AddColumn("", width * 2 / 32 );
	m_listAll.AddColumn(m_pStrTable->String(E_STR_ID_HOLD_TIME), width * 12 / 32);
	m_listAll.AddColumn(m_pStrTable->String(E_STR_ID_TRIGGER), width * 18 / 32 - 2);

	{
		int ch = 0;
		pWnd = m_listAll.AddItem(ch, COL_CHANNEL, WND_TYPE_CHECK);

		pCombo = dynamic_cast<GUI::CComboCtrl*>(m_listAll.AddItem(ch, COL_HOLD_TIME, WND_TYPE_COMBO));
		assert(NULL != pCombo);
		pCombo->AddItem("5",5);
		pCombo->AddItem("10",10);
		pCombo->AddItem("20",20);
		pCombo->AddItem("30",30);
		pCombo->AddItem("40",40);
		pCombo->SetCurItem(0);

		pWnd = m_listAll.AddItem(ch, COL_TRIGGER, WND_TYPE_BUTTON);
		pWnd->SetCaption(m_pStrTable->String(E_STR_ID_SETTING), false);
	}
	m_listAll.CreateListFocusMap();

	//////////////////////////////////////////////////////////////////////////
	m_pTriggerInfo = new ALARM_TRIGGER_INFO[s_loginSuccessReply.productInfo.sensorInputNum];
	memset(m_pTriggerInfo, 0, sizeof(ALARM_TRIGGER_INFO) * s_loginSuccessReply.productInfo.sensorInputNum);
	m_pAllTriggerInfo = new ALARM_TRIGGER_INFO[1];
	memset(m_pAllTriggerInfo, 0, sizeof(ALARM_TRIGGER_INFO) * 1);

	//m_pTriggerInfo = CCfgTriggerDlg::CreateTriggerInfo(s_loginSuccessReply.productInfo.sensorInputNum);	
	//m_pAllTriggerInfo = CCfgTriggerDlg::CreateTriggerInfo(1);	
}

void CCfgNetAlarmHandingDlg::OnDestroy()
{
	GUI::CDialog::OnDestroy();

	if( m_pTriggerInfo )
	{
		delete [] m_pTriggerInfo;
		m_pTriggerInfo = NULL;
	}

	if( m_pAllTriggerInfo )
	{
		delete [] m_pAllTriggerInfo;
		m_pAllTriggerInfo = NULL;
	}
}

void CCfgNetAlarmHandingDlg::UpdateView(bool bRefresh)
{
	GUI::CCheckCtrl* pCheckAll = dynamic_cast<GUI::CCheckCtrl*>(m_listAll.GetItemWnd(0, COL_CHANNEL));
	if( pCheckAll )
	{
		pCheckAll->SetCheck( false, bRefresh );
	}

	GUI::CButton* pButtonAll = dynamic_cast<GUI::CButton*>(m_listAll.GetItemWnd(0, COL_TRIGGER));
	if( pButtonAll )
	{
		pButtonAll->SetEnable(false, bRefresh );
	}

	CCfgDlg::UpdateView( bRefresh );
}
bool CCfgNetAlarmHandingDlg::ShowData(NCFG_ITEM_ID cfgID, const BYTE* pData, int len, bool bRefresh)
{
	assert(NULL != pData);
	assert(len > 0);

	BYTE count = s_loginSuccessReply.productInfo.sensorInputNum;

	switch (cfgID)
	{
	case NCFG_ITEM_SENSOR_IN_HOLD_TIME:
		{
			assert(len == sizeof(unsigned long)*count);
			const unsigned long* pValue = reinterpret_cast<const unsigned long*>(pData);

			GUI::CComboCtrl* pCombo = NULL;
			for (int ch = 0; ch < count; ++ch, ++pValue)
			{
				pCombo = dynamic_cast<GUI::CComboCtrl*>(m_listCtrl.GetItemWnd(ch, COL_HOLD_TIME));
				assert( NULL != pCombo );
				pCombo->SetCurItemData(*pValue, bRefresh);
			}

		}
		return true;
		break;
	case NCFG_ITEM_SENSOR_IN_BUZZ:
		{
			assert(len == sizeof(unsigned long) * count);
			const unsigned long* pValue = reinterpret_cast<const unsigned long*>(pData);

			assert(NULL != m_pTriggerInfo);
			for (BYTE i = 0; i < count; ++i, ++pValue)
			{
				m_pTriggerInfo[i].Buzzer = *pValue;
			}
		}
		return true;
		break;
	case NCFG_ITEM_SENSOR_IN_TO_ALARM_OUT:
		{
			assert(len == sizeof(ULONGLONG) * count);
			const ULONGLONG* pValue = reinterpret_cast<const ULONGLONG*>(pData);

			assert(NULL != m_pTriggerInfo);
			for (BYTE i = 0; i < count; ++i, ++pValue)
			{
				m_pTriggerInfo[i].AlarmOut = *pValue;
			}
		}
		return true;
		break;
	case NCFG_ITEM_SENSOR_IN_TO_RECORD:
		{
			assert(len == sizeof(ULONGLONG) * count);
			const ULONGLONG* pValue = reinterpret_cast<const ULONGLONG*>(pData);

			assert(NULL != m_pTriggerInfo);
			for (BYTE i = 0; i < count; ++i, ++pValue)
			{
				m_pTriggerInfo[i].Record = *pValue;
			}
		}
		return true;
		break;
	case NCFG_ITEM_SENSOR_IN_TO_PTZ:
		{
			BYTE video_count = s_loginSuccessReply.productInfo.videoInputNum;

			assert(len == sizeof(TO_PTZ) * count * video_count);
			const TO_PTZ* pValue = reinterpret_cast<const TO_PTZ*>(pData);

			assert(NULL != m_pTriggerInfo);
			for (BYTE i = 0; i < count; ++i)
			{
				for (BYTE ch = 0; ch < video_count; ++ch, ++pValue)
				{
					m_pTriggerInfo[i].ToPtz[ch] = *pValue;
				}				
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

bool CCfgNetAlarmHandingDlg::SetData(NCFG_ITEM_ID cfgID, BYTE*& pData, int& len)  const
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

	BYTE count = s_loginSuccessReply.productInfo.sensorInputNum;
	
	switch (cfgID)
	{
	case NCFG_ITEM_SENSOR_IN_HOLD_TIME:
		{
			assert(len == sizeof(unsigned long) * s_loginSuccessReply.productInfo.localVideoInputNum);
			unsigned long* pValue = reinterpret_cast<unsigned long*>(pData);

			const GUI::CComboCtrl* pCombo = NULL;
			for (int ch = 0; ch < count; ++ch, ++pValue)
			{
				if( bAll )
				{
					pCombo = dynamic_cast<const GUI::CComboCtrl*>( listAll.GetItemWnd(0, COL_HOLD_TIME) );
				}
				else
				{
					pCombo = dynamic_cast<const GUI::CComboCtrl*>( m_listCtrl.GetItemWnd(ch, COL_HOLD_TIME) );
				}
				assert(NULL != pCombo );

				*pValue = pCombo->GetCurItemData();
			}
		}
		return true;
		break;
	case NCFG_ITEM_SENSOR_IN_BUZZ:
		{
			assert(len == sizeof(unsigned long) * count);
			unsigned long* pValue = reinterpret_cast<unsigned long*>(pData);

			assert(NULL != m_pTriggerInfo);
			for (BYTE i = 0; i < count; ++i, ++pValue)
			{
				if( bAll )
				{
					*pValue = m_pAllTriggerInfo[0].Buzzer;
				}
				else
				{
					*pValue = m_pTriggerInfo[i].Buzzer;
				}

			}
		}
		return true;
		break;
	case NCFG_ITEM_SENSOR_IN_TO_ALARM_OUT:
		{
			assert(len == sizeof(ULONGLONG) * count);
			ULONGLONG* pValue = reinterpret_cast<ULONGLONG*>(pData);

			assert(NULL != m_pTriggerInfo);
			for (BYTE i = 0; i < count; ++i, ++pValue)
			{
				if( bAll )
				{
					*pValue = m_pAllTriggerInfo[0].AlarmOut;
				}
				else
				{
					*pValue = m_pTriggerInfo[i].AlarmOut;
				}
			}
		}
		return true;
		break;
	case NCFG_ITEM_SENSOR_IN_TO_RECORD:
		{
			assert(len == sizeof(ULONGLONG) * count);
			ULONGLONG* pValue = reinterpret_cast<ULONGLONG*>(pData);

			assert(NULL != m_pTriggerInfo);
			for (BYTE i = 0; i < count; ++i, ++pValue)
			{
				if( bAll )
				{
					*pValue = m_pAllTriggerInfo[0].Record;
				}
				else
				{
					*pValue = m_pTriggerInfo[i].Record;
				}
			}
		}
		return true;
		break;
	case NCFG_ITEM_SENSOR_IN_TO_PTZ:
		{
			BYTE video_count = s_loginSuccessReply.productInfo.videoInputNum;
			assert(len == sizeof(TO_PTZ) * count * video_count);
			TO_PTZ* pValue = reinterpret_cast<TO_PTZ*>(pData);

			assert(NULL != m_pTriggerInfo);
			for (BYTE i = 0; i < count; ++i)
			{
				for (BYTE ch = 0; ch < video_count; ++ch, ++pValue)
				{
					if( bAll )
					{
						*pValue = m_pAllTriggerInfo[0].ToPtz[ch];
					}
					else
					{
						*pValue = m_pTriggerInfo[i].ToPtz[ch];
					}
				}				
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

//void CCfgNetAlarmHandingDlg::GetItemList(CFGITEM_LIST& itemList) const
//{
//	itemList.push_back(NCFG_ITEM_SENSOR_IN_ENABLE);
//	itemList.push_back(NCFG_ITEM_SENSOR_IN_TYPE);
//	itemList.push_back(NCFG_ITEM_SENSOR_IN_NAME);
//}


BEGIN_MSG_MAP(CCfgNetAlarmHandingDlg, CCfgDlg)
	ON_MSG_EX(CTRLID_CFG_SENSOR_LIST, KEY_VIRTUAL_MSG_DEFAULT_TAG, OnClickList)
	ON_MSG_EX(m_listAll.GetID(), KEY_VIRTUAL_MSG_DEFAULT_TAG, OnClickListAll)
END_MSG_MAP()

unsigned long CCfgNetAlarmHandingDlg::OnClickList(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	unsigned short row = static_cast<unsigned short>((keyID >> 16)&0x7fff);
	unsigned short col = static_cast<unsigned short>(keyID&0xffff);

	if (COL_TRIGGER == col)
	{
		CCfgTriggerDlg dlg;
		m_pTriggerInfo[row].PtzSafeChnn = -1;
		dlg.SetTriggerInfo(m_pTriggerInfo[row], row);
		if (DLG_OK == dlg.DoModal(this))
		{
			dlg.GetTriggerInfo(m_pTriggerInfo[row]);
			if (!IsModify())
			{
				if (IsChange(NCFG_ITEM_SENSOR_IN_BUZZ) ||
					IsChange(NCFG_ITEM_SENSOR_IN_TO_ALARM_OUT) ||
					IsChange(NCFG_ITEM_SENSOR_IN_TO_RECORD) ||
					IsChange(NCFG_ITEM_SENSOR_IN_TO_PTZ))
				{
					SetModify(true);
					return KEY_VIRTUAL_MSG_CONTINUE;
				}
			}			
		}
	}
	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CCfgNetAlarmHandingDlg::OnClickListAll(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	unsigned short row = static_cast<unsigned short>((keyID >> 16)&0x7fff);
	unsigned short col = static_cast<unsigned short>(keyID&0xffff);

	GUI::CCheckCtrl* pCheckAll = dynamic_cast<GUI::CCheckCtrl*>(m_listAll.GetItemWnd(0, COL_CHANNEL));
	if (COL_CHANNEL == col)
	{
		GUI::CButton* pButtonAll = dynamic_cast<GUI::CButton*>(m_listAll.GetItemWnd(0, COL_TRIGGER));
		assert(NULL != pButtonAll);
		pButtonAll->SetEnable(pCheckAll->IsCheck(), true);
	}

	CCfgDlg* pCfgDlg = dynamic_cast<CCfgDlg*>( this->GetParent() );
	assert( pCheckAll );
	assert( pCfgDlg );
	if( pCheckAll->IsCheck() )
	{
		if (COL_TRIGGER == col)
		{
			CCfgTriggerDlg dlg;
			m_pAllTriggerInfo[0].PtzSafeChnn = -1;
			dlg.SetTriggerInfo(m_pAllTriggerInfo[0], INVALID_INDEX(unsigned char));
			if (DLG_OK == dlg.DoModal(this))
			{
				dlg.GetTriggerInfo(m_pAllTriggerInfo[0]);
			}
			else
			{
				return KEY_VIRTUAL_MSG_STOP;
			}
		}

		pCfgDlg->SetRefreshView( true );
		if (!IsModify())
		{
			if( COL_TRIGGER == col ||
				COL_HOLD_TIME == col )
			{
				SetModify(true);
				return KEY_VIRTUAL_MSG_CONTINUE;
			}
		}
		
	}
	else
	{
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

