
#include "CfgSpeedAlmOutputDlg.h"


CCfgSpeedAlmOutputDlg::CCfgSpeedAlmOutputDlg():m_pTriggerInfo(NULL)
{

}

CCfgSpeedAlmOutputDlg::~CCfgSpeedAlmOutputDlg()
{
	if( m_pTriggerInfo )
	{
		delete [] m_pTriggerInfo;
		m_pTriggerInfo = NULL;
	}
}

void CCfgSpeedAlmOutputDlg::OnInitial()
{
	CCfgDlg::OnInitial();
	//////////////////////////////////////////////////////////////////////////

	unsigned short x = m_length[DLG_OFFSET_SUB_LEFT], y = m_length[DLG_OFFSET_SUB_TOP];
	//int height = 340;
	unsigned short width = m_rect.Width() - 2 * m_length[DLG_OFFSET_SUB_LEFT];
	unsigned short height = m_rect.Height() - y - 2 * m_length[LIST_ITEM_HEIGHT] - m_length[DLG_OFFSET_SUB_BOTTOM];

	unsigned short offsetEdge = 0;
	int maxListRow = (height - m_length[LIST_HEAD_HEIGHT])/m_length[LIST_ITEM_HEIGHT]; //List框带头
	//int numChnn = CCfgDlg::s_loginSuccessReply.productInfo.sensorInputNum;
	int numChnn = 2;
	if( numChnn > maxListRow )
	{
		offsetEdge = m_length[SCROLL_WIDTH] + 2;
	}

	m_listCtrl.Create(CTRLID_CFG_SENSOR_LIST, x, y, width, height, this, 0, 0);
	m_listCtrl.AddColumn("", width * 6 / 32 );
	m_listCtrl.AddColumn(m_pStrTable->String(E_STR_ID_HOLD_TIME), width * 12 / 32);
	m_listCtrl.AddColumn(m_pStrTable->String(E_STR_ID_TRIGGER), width * 14 / 32 - offsetEdge);

	char str[32] = {0};
	GUI::CWnd * pWnd = NULL;
	GUI::CComboCtrl * pCombo = NULL;

	m_listCtrl.AddItem(0, COL_CHANNEL, m_pStrTable->String(E_STR_ID_HIGH_SPEED));
	m_listCtrl.AddItem(1, COL_CHANNEL, m_pStrTable->String(E_STR_ID_LOW_SPEED));

	for (int ch = 0; ch < 2; ++ch)
	{
		pCombo = dynamic_cast<GUI::CComboCtrl*>(m_listCtrl.AddItem(ch, COL_HOLD_TIME, WND_TYPE_COMBO));
		assert(NULL != pCombo);
		pCombo->AddItem("5", 5);
		pCombo->AddItem("10", 10);
		pCombo->AddItem("20", 20);
		pCombo->AddItem("30", 30);
		pCombo->AddItem("60", 60);
		pCombo->AddItem("120", 120);
		pCombo->AddItem(m_pStrTable->String(E_STR_ID_ALWAYS), -1);
		pCombo->SetCurItemData(10, false);
		pCombo->SetDropDownCount(7);

		pWnd = m_listCtrl.AddItem(ch, COL_TRIGGER, WND_TYPE_BUTTON);
		pWnd->SetCaption(m_pStrTable->String(E_STR_ID_TRIGGER), false);
	}

	m_listCtrl.CreateListFocusMap();

	if( m_pTriggerInfo )
	{
		delete [] m_pTriggerInfo;
	}

	m_pTriggerInfo = new ALARM_TRIGGER_INFO[2];
	memset(m_pTriggerInfo, 0, sizeof(ALARM_TRIGGER_INFO) * 2);

}


void CCfgSpeedAlmOutputDlg::OnDestroy()
{
	GUI::CDialog::OnDestroy();

	if( m_pTriggerInfo )
	{
		delete [] m_pTriggerInfo;
		m_pTriggerInfo = NULL;
	}
}



bool CCfgSpeedAlmOutputDlg::ShowData(NCFG_ITEM_ID cfgID, const BYTE* pData, int len, bool bRefresh)
{
	assert(NULL != pData);
	assert(len > 0);

	BYTE* pTempData = NULL;
	unsigned long tempDataLen = 0;

	switch (cfgID)
	{
	case NCFG_ITEM_HIGH_SPEED_TO_ALARM_OUT:
		{
			assert(len == sizeof(ULONGLONG));
			const ULONGLONG* pValue = reinterpret_cast<const ULONGLONG*>(pData);

			assert(NULL != m_pTriggerInfo);
			m_pTriggerInfo[0].AlarmOut = *pValue;

		}	
		return true;
	case NCFG_ITEM_HIGH_SPEED_HOLD_TIME:
		{
			assert(len == sizeof(unsigned long));
			const unsigned long* pValue = reinterpret_cast<const unsigned long*>(pData);

			GUI::CComboCtrl* pCtrl = dynamic_cast<GUI::CComboCtrl*>(m_listCtrl.GetItemWnd(0, COL_HOLD_TIME));
			if( *pValue != static_cast<unsigned long>(-1) )
			{
				pCtrl->SetCurItemData(*pValue, bRefresh);
			}
			else
			{
				pCtrl->SetCurItemData(-1, bRefresh);
			}
			
		}	
		return true;
		break;

	case NCFG_ITEM_HIGH_SPEED_BUZZ:
		{
			assert(len == sizeof(unsigned long));
			const unsigned long* pValue = reinterpret_cast<const unsigned long*>(pData);

			assert(NULL != m_pTriggerInfo);

			m_pTriggerInfo[0].Buzzer = *pValue;
			
		}	
		return true;
		break;
	case NCFG_ITEM_HIGH_SPEED_TO_RECORD:
		{
			assert(len == sizeof(ULONGLONG));
			const ULONGLONG* pValue = reinterpret_cast<const ULONGLONG*>(pData);

			assert(NULL != m_pTriggerInfo);

			m_pTriggerInfo[0].Record = *pValue;
	
		}	
		return true;
		break;
	case NCFG_ITEM_HIGH_SPEED_TO_PTZ:
		{
			BYTE video_count = s_loginSuccessReply.productInfo.localVideoInputNum;

			assert(len == sizeof(TO_PTZ) * video_count);
			const TO_PTZ* pValue = reinterpret_cast<const TO_PTZ*>(pData);

			assert(NULL != m_pTriggerInfo);
		
			for (BYTE ch = 0; ch < video_count; ++ch, ++pValue)
			{
				m_pTriggerInfo[0].ToPtz[ch] = *pValue;
			}				
			
		}	
		return true;
		break;

	case NCFG_ITEM_HIGH_SPEED_BIG_VIEW:
		{
			const ULONGLONG *pValue = reinterpret_cast<const ULONGLONG*>(pData);

			m_pTriggerInfo[0].FullScreen = *pValue;
			
		}	
		return true;
		break;
#ifdef __DVR_BASIC__	
	case NCFG_ITEM_HIGH_SPEED_TO_EMAIL_ENABLE:
		{
			assert(len == sizeof(unsigned long));
			const unsigned long* pValue = reinterpret_cast<const unsigned long*>(pData);

			assert(NULL != m_pTriggerInfo);

			m_pTriggerInfo[0].EmailEnable = *pValue;
		}
		return true;
		break;
	case NCFG_ITEM_HIGH_SPEED_TO_EMAIL_ADDR:
		{
			//获得到配置里前3个收件人地址
			unsigned long tempDataLen = 0;
			BYTE* pTempData = NULL;
			CCfgDlg::GetConfig(NCFG_ITEM_EMAIL_RECV_INFO, &pTempData, tempDataLen);
			char *pInfo = reinterpret_cast<char *>(pTempData);
			if(pInfo != NULL)
			{
				memcpy(m_pTriggerInfo[0].AddrInfo, pInfo, sizeof(m_pTriggerInfo[0].AddrInfo));
			}

			assert(len == sizeof(unsigned long));
			const unsigned long* pValue = reinterpret_cast<const unsigned long*>(pData);

			assert(NULL != m_pTriggerInfo);

			m_pTriggerInfo[0].EmailAddr = *pValue;
		}
		return true;
		break;
#endif
		//-----LOW SPEED 
	case NCFG_ITEM_LOW_SPEED_TO_ALARM_OUT:
		{
			assert(len == sizeof(ULONGLONG));
			const ULONGLONG* pValue = reinterpret_cast<const ULONGLONG*>(pData);

			assert(NULL != m_pTriggerInfo);
			m_pTriggerInfo[1].AlarmOut = *pValue;
	
		}	
		return true;
		break;
	case NCFG_ITEM_LOW_SPEED_HOLD_TIME:
		{
			assert(len == sizeof(unsigned long));
			const unsigned long* pValue = reinterpret_cast<const unsigned long*>(pData);

			GUI::CComboCtrl* pCtrl = dynamic_cast<GUI::CComboCtrl*>(m_listCtrl.GetItemWnd(1, COL_HOLD_TIME));
			if( *pValue != static_cast<unsigned long>(-1) )
			{
				pCtrl->SetCurItemData(*pValue, bRefresh);
			}
			else
			{
				pCtrl->SetCurItemData(-1, bRefresh);
			}
			
		}	
		return true;
		break;
	case NCFG_ITEM_LOW_SPEED_BUZZ:
		{
			assert(len == sizeof(unsigned long));
			const unsigned long* pValue = reinterpret_cast<const unsigned long*>(pData);

			assert(NULL != m_pTriggerInfo);

			m_pTriggerInfo[1].Buzzer = *pValue;
	
		}	
		return true;
		break;
	case NCFG_ITEM_LOW_SPEED_TO_RECORD:
		{
			assert(len == sizeof(ULONGLONG));
			const ULONGLONG* pValue = reinterpret_cast<const ULONGLONG*>(pData);

			assert(NULL != m_pTriggerInfo);

			m_pTriggerInfo[1].Record = *pValue;

		}	
		return true;
		break;
	case NCFG_ITEM_LOW_SPEED_TO_PTZ:
		{
			BYTE video_count = s_loginSuccessReply.productInfo.localVideoInputNum;

			assert(len == sizeof(TO_PTZ) * video_count);
			const TO_PTZ* pValue = reinterpret_cast<const TO_PTZ*>(pData);

			assert(NULL != m_pTriggerInfo);

			for (BYTE ch = 0; ch < video_count; ++ch, ++pValue)
			{
				m_pTriggerInfo[1].ToPtz[ch] = *pValue;
			}				
		
		}	
		return true;
		break;
	case NCFG_ITEM_LOW_SPEED_BIG_VIEW:
		{
			const ULONGLONG *pValue = reinterpret_cast<const ULONGLONG*>(pData);

			m_pTriggerInfo[1].FullScreen = *pValue;
		
		}	
		return true;
		break;
#ifdef __DVR_BASIC__	
	case NCFG_ITEM_LOW_SPEED_TO_EMAIL_ENABLE:
		{
			assert(len == sizeof(unsigned long));
			const unsigned long* pValue = reinterpret_cast<const unsigned long*>(pData);

			assert(NULL != m_pTriggerInfo);

			m_pTriggerInfo[1].EmailEnable = *pValue;
		}
		return true;
		break;
	case NCFG_ITEM_LOW_SPEED_TO_EMAIL_ADDR:
		{
			//获得到配置里前3个收件人地址
			unsigned long tempDataLen = 0;
			BYTE* pTempData = NULL;
			CCfgDlg::GetConfig(NCFG_ITEM_EMAIL_RECV_INFO, &pTempData, tempDataLen);
			char *pInfo = reinterpret_cast<char *>(pTempData);
			if(pInfo != NULL)
			{
				memcpy(m_pTriggerInfo[1].AddrInfo, pInfo, sizeof(m_pTriggerInfo[1].AddrInfo));
			}

			assert(len == sizeof(unsigned long));
			const unsigned long* pValue = reinterpret_cast<const unsigned long*>(pData);

			assert(NULL != m_pTriggerInfo);

			m_pTriggerInfo[1].EmailAddr = *pValue;
		}
		return true;
		break;
#endif
	default:
		return false;
		break;
	}
	return false;
}

bool CCfgSpeedAlmOutputDlg::SetData(NCFG_ITEM_ID cfgID, BYTE*& pData, int& len)  const
{
	assert(NULL != pData);
	assert(len > 0);

	switch (cfgID)
	{
	case NCFG_ITEM_HIGH_SPEED_HOLD_TIME:
		{
			unsigned long* pValue = reinterpret_cast<unsigned long*>(pData);

			const GUI::CComboCtrl* pCombo = NULL;

			pCombo = dynamic_cast<const GUI::CComboCtrl*>( m_listCtrl.GetItemWnd(0, COL_HOLD_TIME) );
			assert( NULL != pCombo );

			unsigned long value = pCombo->GetCurItemData();
			if(value != static_cast<unsigned long>(-1))
			{
				*pValue = value;
			}
			else
			{
				*pValue = -1;
			}
			
		}
		return true;
		break;

	case NCFG_ITEM_HIGH_SPEED_BUZZ:
		{
			unsigned long* pValue = reinterpret_cast<unsigned long*>(pData);

			assert(NULL != m_pTriggerInfo);

			*pValue = m_pTriggerInfo[0].Buzzer;
			
		}
		return true;
		break;

	case NCFG_ITEM_HIGH_SPEED_TO_ALARM_OUT:
		{
			ULONGLONG* pValue = reinterpret_cast<ULONGLONG*>(pData);

			assert(NULL != m_pTriggerInfo);
			
			*pValue = m_pTriggerInfo[0].AlarmOut;
			
		}
		return true;
		break;

	case NCFG_ITEM_HIGH_SPEED_TO_RECORD:
		{

			ULONGLONG* pValue = reinterpret_cast<ULONGLONG*>(pData);

			assert(NULL != m_pTriggerInfo);

			*pValue = m_pTriggerInfo[0].Record;
			
		}
		return true;
		break;

	case NCFG_ITEM_HIGH_SPEED_TO_PTZ:
		{	
			BYTE video_count = s_loginSuccessReply.productInfo.localVideoInputNum;
			TO_PTZ* pValue = reinterpret_cast<TO_PTZ*>(pData);

			assert(NULL != m_pTriggerInfo);

			for (BYTE ch = 0; ch < video_count; ++ch, ++pValue)
			{
				
				*pValue = m_pTriggerInfo[0].ToPtz[ch];
				
			}				
			
		}
		return true;
		break;

	case NCFG_ITEM_HIGH_SPEED_BIG_VIEW:
		{
			assert( NULL != m_pTriggerInfo );
			ULONGLONG *pValue = reinterpret_cast<ULONGLONG *>(pData);

			*pValue = m_pTriggerInfo[0].FullScreen;
		}
		return true;
		break;
#ifdef __DVR_BASIC__	
	case NCFG_ITEM_HIGH_SPEED_TO_EMAIL_ENABLE:
		{
			unsigned long* pValue = reinterpret_cast<unsigned long*>(pData);
			assert(NULL != m_pTriggerInfo);

			*pValue = m_pTriggerInfo[0].EmailEnable;
		}
		return true;
		break;
	case NCFG_ITEM_HIGH_SPEED_TO_EMAIL_ADDR:
		{
			unsigned long* pValue = reinterpret_cast<unsigned long*>(pData);
			assert(NULL != m_pTriggerInfo);

			*pValue = m_pTriggerInfo[0].EmailAddr;
		}
		return true;
		break;
#endif
		//-----low speed
	case NCFG_ITEM_LOW_SPEED_HOLD_TIME:
		{
			unsigned long* pValue = reinterpret_cast<unsigned long*>(pData);

			const GUI::CComboCtrl* pCombo = NULL;
	
			pCombo = dynamic_cast<const GUI::CComboCtrl*>( m_listCtrl.GetItemWnd(1, COL_HOLD_TIME) );
			assert( NULL != pCombo );

			unsigned long value = pCombo->GetCurItemData();
			if(value != static_cast<unsigned long>(-1))
			{
				*pValue = value;
			}
			else
			{
				*pValue = -1;
			}

		}
		return true;
		break;

	case NCFG_ITEM_LOW_SPEED_BUZZ:
		{
			unsigned long* pValue = reinterpret_cast<unsigned long*>(pData);

			assert(NULL != m_pTriggerInfo);
		
			*pValue = m_pTriggerInfo[1].Buzzer;
			
		}
		return true;
		break;

	case NCFG_ITEM_LOW_SPEED_TO_ALARM_OUT:
		{
			ULONGLONG* pValue = reinterpret_cast<ULONGLONG*>(pData);

			assert(NULL != m_pTriggerInfo);

			*pValue = m_pTriggerInfo[1].AlarmOut;

		}
		return true;
		break;

	case NCFG_ITEM_LOW_SPEED_TO_RECORD:
		{
			ULONGLONG* pValue = reinterpret_cast<ULONGLONG*>(pData);

			assert(NULL != m_pTriggerInfo);
			*pValue = m_pTriggerInfo[1].Record;
		}
		return true;
		break;

	case NCFG_ITEM_LOW_SPEED_TO_PTZ:
		{	
			BYTE video_count = s_loginSuccessReply.productInfo.localVideoInputNum;
				TO_PTZ* pValue = reinterpret_cast<TO_PTZ*>(pData);

			assert(NULL != m_pTriggerInfo);

			for (BYTE ch = 0; ch < video_count; ++ch, ++pValue)
			{

				*pValue = m_pTriggerInfo[1].ToPtz[ch];

			}				
		
		}
		return true;
		break;

	case NCFG_ITEM_LOW_SPEED_BIG_VIEW:
		{
			assert( NULL != m_pTriggerInfo );
			ULONGLONG *pValue = reinterpret_cast<ULONGLONG *>(pData);

			*pValue = m_pTriggerInfo[1].FullScreen;
		
		}
		return true;
		break;
#ifdef __DVR_BASIC__	
	case NCFG_ITEM_LOW_SPEED_TO_EMAIL_ENABLE:
		{
			unsigned long* pValue = reinterpret_cast<unsigned long*>(pData);

			assert(NULL != m_pTriggerInfo);

			*pValue = m_pTriggerInfo[1].EmailEnable;

		}
		return true;
		break;
	case NCFG_ITEM_LOW_SPEED_TO_EMAIL_ADDR:
		{
			unsigned long* pValue = reinterpret_cast<unsigned long*>(pData);

			assert(NULL != m_pTriggerInfo);

			*pValue = m_pTriggerInfo[1].EmailAddr;

		}
		return true;
		break;
#endif
	default:
		return false;
		break;
	}
	return false;
}



BEGIN_MSG_MAP(CCfgSpeedAlmOutputDlg, CCfgDlg)
	ON_MSG_EX(CTRLID_CFG_SENSOR_LIST, KEY_VIRTUAL_MSG_DEFAULT_TAG, OnClickList)
END_MSG_MAP()

unsigned long CCfgSpeedAlmOutputDlg::OnClickList(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	unsigned short row = static_cast<unsigned short>((keyID >> 16)&0x7fff);
	unsigned short col = static_cast<unsigned short>(keyID&0xffff);


	if( COL_HOLD_TIME == col )
	{
		if(row == 0)
		{
			if( IsChange(NCFG_ITEM_HIGH_SPEED_HOLD_TIME) )
			{
				SetModify(true);
				return KEY_VIRTUAL_MSG_CONTINUE;
			}
		}
		else if(row == 1)
		{
			if( IsChange(NCFG_ITEM_LOW_SPEED_HOLD_TIME) )
			{
				SetModify(true);
				return KEY_VIRTUAL_MSG_CONTINUE;
			}
		}
	}

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
				if(row == 0)
				{
					if (IsChange(NCFG_ITEM_HIGH_SPEED_BIG_VIEW) ||
						IsChange(NCFG_ITEM_HIGH_SPEED_BUZZ) ||
						IsChange(NCFG_ITEM_HIGH_SPEED_TO_ALARM_OUT) ||
						IsChange(NCFG_ITEM_HIGH_SPEED_TO_RECORD) ||
#ifdef __DVR_BASIC__	
						IsChange(NCFG_ITEM_HIGH_SPEED_TO_EMAIL_ENABLE) ||
						IsChange(NCFG_ITEM_HIGH_SPEED_TO_EMAIL_ADDR) ||
#endif
						IsChange(NCFG_ITEM_HIGH_SPEED_TO_PTZ))
					{
						SetModify(true);
						return KEY_VIRTUAL_MSG_CONTINUE;
					}
				}
				else if(row == 1)
				{
					if (IsChange(NCFG_ITEM_LOW_SPEED_BIG_VIEW) ||
						IsChange(NCFG_ITEM_LOW_SPEED_BUZZ) ||
						IsChange(NCFG_ITEM_LOW_SPEED_TO_ALARM_OUT) ||
						IsChange(NCFG_ITEM_LOW_SPEED_TO_RECORD) ||
#ifdef __DVR_BASIC__	
						IsChange(NCFG_ITEM_LOW_SPEED_TO_EMAIL_ENABLE) ||
						IsChange(NCFG_ITEM_LOW_SPEED_TO_EMAIL_ADDR) ||
#endif
						IsChange(NCFG_ITEM_LOW_SPEED_TO_PTZ))
					{
						SetModify(true);
						return KEY_VIRTUAL_MSG_CONTINUE;
					}

				}

			}			
		}
	}
	return KEY_VIRTUAL_MSG_STOP;
}
