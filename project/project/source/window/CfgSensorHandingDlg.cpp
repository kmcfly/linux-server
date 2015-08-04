/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : hz
** Date         : 2009-08-01
** Name         : CfgSensorHandingDlg.cpp 
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/
#include "CfgSensorHandingDlg.h"
#include "FuncCustom.h"

CCfgSensorHandingDlg::CCfgSensorHandingDlg() : m_pTriggerInfo(NULL), m_pAllTriggerInfo(NULL)
{

}

CCfgSensorHandingDlg::~CCfgSensorHandingDlg()
{
	if( m_pTriggerInfo )
	{
		delete [] m_pTriggerInfo;
		m_pTriggerInfo = NULL;
	}

	if( m_pAllTriggerInfo )
	{
		delete m_pAllTriggerInfo;
		m_pAllTriggerInfo = NULL;
	}
}

//////////////////////////////////////////////////////////////////////////
void CCfgSensorHandingDlg::OnInitial()
{
	CCfgDlg::OnInitial();
	//////////////////////////////////////////////////////////////////////////

	unsigned short x = m_length[DLG_OFFSET_SUB_LEFT], y = m_length[DLG_OFFSET_SUB_TOP];
	//int height = 340;
	unsigned short width = m_rect.Width() - 2 * m_length[DLG_OFFSET_SUB_LEFT];
	unsigned short height = m_rect.Height() - y - 2 * m_length[LIST_ITEM_HEIGHT] - m_length[DLG_OFFSET_SUB_BOTTOM];

	unsigned short offsetEdge = 0;
	int maxListRow = (height - m_length[LIST_HEAD_HEIGHT])/m_length[LIST_ITEM_HEIGHT]; //List框带头

	ULONGLONG CH = 0;
	int localVideoInputNum = 0, netVideoInputNum = 0;
	int localSensorInNum = s_loginSuccessReply.productInfo.sensorInputNum;
	int effectChnn = 0;

	IsSupportCH(CH, &effectChnn, &localVideoInputNum, &netVideoInputNum);

	CH >>= localVideoInputNum;
	CH <<= localSensorInNum;
	CH |= ALL_CHNN_MASK(localSensorInNum);

	effectChnn += localSensorInNum;
	effectChnn -= localVideoInputNum;
	
	int sensorInNum = localSensorInNum + netVideoInputNum;

	int row = 0;
	for(int i = 0; i < sensorInNum; i++)
	{
		if((CH >> i) & 0x1)
		{
			m_rowChnnMap.insert(std::make_pair(row++, i));
		}
	}

	if(effectChnn > maxListRow)
	{
		offsetEdge = m_length[SCROLL_WIDTH] + 2;
	}

	m_listCtrl.Create(CTRLID_CFG_SENSOR_LIST, x, y, width, height, this, 0, 0);
	m_listCtrl.AddColumn(m_pStrTable->String(E_STR_ID_CH), width * 3 / 32 );
#if !(defined(__CUSTOM_US02__) && defined(__CUSTOM_US02_NEW__))
	m_listCtrl.AddColumn(m_pStrTable->String(E_STR_ID_HOLD_TIME), width * 12 / 32);
#endif
	m_listCtrl.AddColumn(m_pStrTable->String(E_STR_ID_TRIGGER), width * 17 / 32 - offsetEdge);

	char str[32] = {0};
	GUI::CWnd * pWnd = NULL;
	GUI::CComboCtrl * pCombo = NULL;
	for (int ch = 0; ch < effectChnn; ++ch)
	{
		int chnnIndex = m_rowChnnMap.find(ch)->second;
		if(chnnIndex < localVideoInputNum)
		{
			sprintf(str, "%d", chnnIndex + 1);
		}
		else
		{
			sprintf(str, "%d", chnnIndex + 1 /*- localVideoInputNum*/);
		}
		m_listCtrl.AddItem(ch, COL_CHANNEL, str);

#if !(defined(__CUSTOM_US02__) && defined(__CUSTOM_US02_NEW__))
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
#endif

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
	m_listAll.AddColumn(m_pStrTable->String(E_STR_ID_CH), width * 3 / 32 );
#if !(defined(__CUSTOM_US02__) && defined(__CUSTOM_US02_NEW__))
	m_listAll.AddColumn(m_pStrTable->String(E_STR_ID_HOLD_TIME), width * 12 / 32);
#endif
	m_listAll.AddColumn(m_pStrTable->String(E_STR_ID_TRIGGER), width * 17 / 32 - 2);


	{
		int ch = 0;
		pWnd = m_listAll.AddItem(ch, COL_CHANNEL, WND_TYPE_CHECK);

#if !(defined(__CUSTOM_US02__) && defined(__CUSTOM_US02_NEW__))
		pCombo = dynamic_cast<GUI::CComboCtrl*>(m_listAll.AddItem(ch, COL_HOLD_TIME, WND_TYPE_COMBO));
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
#endif
		pWnd = m_listAll.AddItem(ch, COL_TRIGGER, WND_TYPE_BUTTON, m_length[BTN_WIDTH], m_length[BTN_HEIGHT]);
		pWnd->SetCaption(m_pStrTable->String(E_STR_ID_SETTING), false);
		pWnd->SetEnable(false, false);
	}
	m_listAll.CreateListFocusMap();

	//////////////////////////////////////////////////////////////////////////
	if( m_pTriggerInfo )
	{
		delete [] m_pTriggerInfo;
	}

	if( m_pAllTriggerInfo )
	{
		delete m_pAllTriggerInfo;
	}

	m_pTriggerInfo = new ALARM_TRIGGER_INFO[sensorInNum];
	memset(m_pTriggerInfo, 0, sizeof(ALARM_TRIGGER_INFO) * sensorInNum);
	m_pAllTriggerInfo = new ALARM_TRIGGER_INFO;
	memset(m_pAllTriggerInfo, 0, sizeof(ALARM_TRIGGER_INFO));
}

void CCfgSensorHandingDlg::OnDestroy()
{
	GUI::CDialog::OnDestroy();

	if( m_pTriggerInfo )
	{
		delete [] m_pTriggerInfo;
		m_pTriggerInfo = NULL;
	}

	if( m_pAllTriggerInfo )
	{
		delete m_pAllTriggerInfo;
		m_pAllTriggerInfo = NULL;
	}

	m_rowChnnMap.clear();
}

void CCfgSensorHandingDlg::UpdateView(bool bRefresh)
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

bool CCfgSensorHandingDlg::ShowData(NCFG_ITEM_ID cfgID, const BYTE* pData, int len, bool bRefresh)
{
	assert(NULL != pData);
	assert(len > 0);

	ULONGLONG CH = 0;
	int localSensorInNum = s_loginSuccessReply.productInfo.sensorInputNum;
	int localVideoInputNum = 0, netVideoInputNum = 0;
	int effectChnn = 0;

	IsSupportCH(CH, &effectChnn, &localVideoInputNum, &netVideoInputNum);

	effectChnn -= localVideoInputNum;
	effectChnn += localSensorInNum;

	int sensorInNum = localSensorInNum + netVideoInputNum;

	switch (cfgID)
	{
#if !(defined(__CUSTOM_US02__) && defined(__CUSTOM_US02_NEW__))
	case NCFG_ITEM_SENSOR_IN_HOLD_TIME:
		{
			assert(len == sizeof(unsigned long) * sensorInNum);
			const unsigned long* pValue = reinterpret_cast<const unsigned long*>(pData);

			for(int ch = 0; ch < effectChnn; ch++)
			{
				int index = m_rowChnnMap.find(ch)->second;
				if(index < sensorInNum)
				{
					GUI::CComboCtrl* pCtrl = dynamic_cast<GUI::CComboCtrl*>(m_listCtrl.GetItemWnd(ch, COL_HOLD_TIME));
					if( pValue[index] != static_cast<unsigned long>(-1) )
					{
						pCtrl->SetCurItemData(pValue[index], bRefresh);
					}
					else
					{
						pCtrl->SetCurItemData(-1, bRefresh);
					}
				}
			}
		}
		return true;
		break;
#endif
	case NCFG_ITEM_SENSOR_IN_BUZZ:
		{
			assert(len == sizeof(unsigned long) * sensorInNum);
			const unsigned long* pValue = reinterpret_cast<const unsigned long*>(pData);

			assert(NULL != m_pTriggerInfo);
			for (BYTE i = 0; i < effectChnn; i++)
			{
				int index = m_rowChnnMap.find(i)->second;
				if(index < sensorInNum)
				{
					m_pTriggerInfo[index].Buzzer = pValue[index];
				}
			}
		}
		return true;
		break;
	case NCFG_ITEM_SENSOR_IN_TO_ALARM_OUT:
		{
			assert(len == sizeof(ULONGLONG) * sensorInNum);
			const ULONGLONG* pValue = reinterpret_cast<const ULONGLONG*>(pData);

			assert(NULL != m_pTriggerInfo);
			for (BYTE i = 0; i < effectChnn; i++)
			{
				int index = m_rowChnnMap.find(i)->second;
				if(index < sensorInNum)
				{
					m_pTriggerInfo[index].AlarmOut = pValue[index];
				}
			}
		}
		return true;
		break;
	case NCFG_ITEM_SENSOR_IN_TO_RECORD:
		{
			assert(len == sizeof(ULONGLONG) * sensorInNum);
			const ULONGLONG* pValue = reinterpret_cast<const ULONGLONG*>(pData);

			assert(NULL != m_pTriggerInfo);
			for (BYTE i = 0; i < effectChnn; i++)
			{
				int index = m_rowChnnMap.find(i)->second;
				if(index < sensorInNum)
				{
					m_pTriggerInfo[index].Record = pValue[index];
				}
			}
		}
		return true;
		break;
	case NCFG_ITEM_SENSOR_IN_TO_PTZ:
		{
			BYTE video_count = localVideoInputNum + netVideoInputNum;

			assert(len == sizeof(TO_PTZ) * sensorInNum * video_count);
			const TO_PTZ* pValue = reinterpret_cast<const TO_PTZ*>(pData);

			assert(NULL != m_pTriggerInfo);
			for (BYTE i = 0; i < effectChnn; i++)
			{
				int index = m_rowChnnMap.find(i)->second;
				for (BYTE ch = 0; ch < video_count; ch++)
				{
					m_pTriggerInfo[index].ToPtz[ch] = pValue[index * video_count + ch];
				}
			}
		}
		return true;
		break;
	case NCFG_ITEM_SENSOR_IN_TO_SNAP:
		{
			assert(len == sizeof(ULONGLONG) * sensorInNum);
			const ULONGLONG* pValue = reinterpret_cast<const ULONGLONG*>(pData);
			
			for(BYTE i = 0; i < effectChnn; i++)
			{
				int index = m_rowChnnMap.find(i)->second;
				m_pTriggerInfo[index].SnapCH = pValue[index];
			}
		}
		return true;
		break;
	case NCFG_ITEM_SENSOR_TO_BIG_VIEW:
		{
			const ULONGLONG *pValue = reinterpret_cast<const ULONGLONG*>(pData);
			for(BYTE i = 0; i < effectChnn; i++)
			{
				int index = m_rowChnnMap.find(i)->second;
				m_pTriggerInfo[index].FullScreen = pValue[index];
			}
		}
		return true;
		break;
#ifdef __DVR_BASIC__	
	case NCFG_ITEM_SENSOR_IN_TO_EMAIL_ENABLE:
		{
			assert(len == sizeof(unsigned long) * sensorInNum);
			const unsigned long* pValue = reinterpret_cast<const unsigned long*>(pData);

			assert(NULL != m_pTriggerInfo);
			for (BYTE i = 0; i < effectChnn; i++)
			{
				int index = m_rowChnnMap.find(i)->second;
				m_pTriggerInfo[index].EmailEnable = pValue[index];
			}
		}
		return true;
		break;
	case NCFG_ITEM_SENSOR_TO_EMAIL_ADDR:
		{
			//获得到配置里前3个收件人地址
			unsigned long tempDataLen = 0;
			BYTE* pTempData = NULL;
			CCfgDlg::GetConfig(NCFG_ITEM_EMAIL_RECV_INFO, &pTempData, tempDataLen);
			char *pInfo = reinterpret_cast<char *>(pTempData);
			if(pInfo != NULL)
			{
				for(BYTE i = 0; i < sensorInNum; ++i)	
				{
					memcpy(m_pTriggerInfo[i].AddrInfo, pInfo, sizeof(m_pTriggerInfo[0].AddrInfo));
				}
			}

			assert(len == sizeof(unsigned long) * sensorInNum);
			const unsigned long* pValue = reinterpret_cast<const unsigned long*>(pData);

			assert(NULL != m_pTriggerInfo);
			for (BYTE i = 0; i < effectChnn; i++)
			{
				int index = m_rowChnnMap.find(i)->second;
				m_pTriggerInfo[index].EmailAddr = pValue[index];
			}
		}

		return true;
		break;
#ifdef __ALARM_TO_UPDATE_FTP_VIDEO__
	case NCFG_ITEM_SENSOR_IN_TO_FTP_ENABLE:
		{
			assert(len == sizeof(ULONGLONG) * sensorInNum);
			const ULONGLONG* pValue = reinterpret_cast<const ULONGLONG*>(pData);

			assert(NULL != m_pTriggerInfo);
			for(BYTE i = 0; i < effectChnn; i++)
			{
				m_pTriggerInfo[i].ftpVideo = pValue[i];
			}
		}
		return true;
		break;
#endif
#endif
	default:
		return false;
		break;
	}

	return false;
}

bool CCfgSensorHandingDlg::SetData(NCFG_ITEM_ID cfgID, BYTE*& pData, int& len)  const
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

	ULONGLONG CH = 0;
	int localSensorInNum = s_loginSuccessReply.productInfo.sensorInputNum;
	int localVideoInputNum = 0, netVideoInputNum = 0;
	int effectChnn = 0;

	IsSupportCH(CH, &effectChnn, &localVideoInputNum, &netVideoInputNum);

    int maxChnn = localVideoInputNum + netVideoInputNum;
	int sensorInNum = localSensorInNum + netVideoInputNum;

	effectChnn -= localVideoInputNum;
	effectChnn += localSensorInNum;

	switch (cfgID)
	{
#if !(defined(__CUSTOM_US02__) && defined(__CUSTOM_US02_NEW__))
	case NCFG_ITEM_SENSOR_IN_HOLD_TIME:
		{
			assert(len == sizeof(unsigned long) * sensorInNum);
			unsigned long* pValue = reinterpret_cast<unsigned long*>(pData);

			const GUI::CComboCtrl* pCombo = NULL;
			for(int ch = 0; ch < effectChnn; ch++)
			{
				int index = m_rowChnnMap.find(ch)->second;

				if(index < sensorInNum)
				{
					if( bAll )
					{
						pCombo = dynamic_cast<const GUI::CComboCtrl*>( listAll.GetItemWnd(0, COL_HOLD_TIME) );
					}
					else
					{
						pCombo = dynamic_cast<const GUI::CComboCtrl*>( m_listCtrl.GetItemWnd(ch, COL_HOLD_TIME) );
					}
					assert( NULL != pCombo );
					unsigned long value = pCombo->GetCurItemData();

					if(value != static_cast<unsigned long>(-1))
					{
						pValue[index] = value;
					}
					else
					{
						pValue[index] = -1;
					}
				}
			}
		}
		return true;
		break;
#endif
	case NCFG_ITEM_SENSOR_IN_BUZZ:
		{
			assert(len == sizeof(unsigned long) * sensorInNum);
			unsigned long* pValue = reinterpret_cast<unsigned long*>(pData);

			assert(NULL != m_pTriggerInfo);
			for (BYTE i = 0; i < effectChnn; i++)
			{
				int index = m_rowChnnMap.find(i)->second;

				if(index < sensorInNum)
				{
					if( bAll )
					{
						pValue[index] = m_pAllTriggerInfo->Buzzer;
					}
					else
					{
						pValue[index] = m_pTriggerInfo[index].Buzzer;
					}
				}
			}
		}
		return true;
		break;
	case NCFG_ITEM_SENSOR_IN_TO_ALARM_OUT:
		{
			assert(len == sizeof(ULONGLONG) * sensorInNum);
			ULONGLONG* pValue = reinterpret_cast<ULONGLONG*>(pData);

			assert(NULL != m_pTriggerInfo);
			for (BYTE i = 0; i < effectChnn; i++)
			{
				int index = m_rowChnnMap.find(i)->second;

				if(index < sensorInNum)
				{
					if( bAll )
					{
						pValue[index] = m_pAllTriggerInfo->AlarmOut;
					}
					else
					{
						pValue[index] = m_pTriggerInfo[index].AlarmOut;
					}
				}
			}
		}
		return true;
		break;
	case NCFG_ITEM_SENSOR_IN_TO_RECORD:
		{
			assert(len == sizeof(ULONGLONG) * sensorInNum);
			ULONGLONG* pValue = reinterpret_cast<ULONGLONG*>(pData);

			assert(NULL != m_pTriggerInfo);
			for (BYTE i = 0; i < effectChnn; i++)
			{
				int index = m_rowChnnMap.find(i)->second;

				if(index < sensorInNum)
				{
					if( bAll )
					{
						pValue[index] = m_pAllTriggerInfo->Record;
					}
					else
					{
						pValue[index] = m_pTriggerInfo[index].Record;
					}
				}
			}
		}
		return true;
		break;
	case NCFG_ITEM_SENSOR_IN_TO_PTZ:
		{

			BYTE video_count = maxChnn;//maxChnn; //temprory
			assert(len == sizeof(TO_PTZ) * sensorInNum * video_count);
			TO_PTZ* pValue = reinterpret_cast<TO_PTZ*>(pData);

			assert(NULL != m_pTriggerInfo);
			for (BYTE i = 0; i < effectChnn; ++i)
			{
				int index = m_rowChnnMap.find(i)->second;
				for (BYTE ch = 0; ch < video_count; ch++)
				{
					if( bAll )
					{
						pValue[index * video_count + ch] = m_pAllTriggerInfo->ToPtz[ch];
					}
					else
					{
						pValue[index * video_count + ch] = m_pTriggerInfo[index].ToPtz[ch];
					}
				}				
			}
		}
		return true;
		break;
	case NCFG_ITEM_SENSOR_IN_TO_SNAP:
		{
			assert(len == sizeof(ULONGLONG) * sensorInNum);
			ULONGLONG* pValue = reinterpret_cast<ULONGLONG*>(pData);

			for(BYTE i = 0; i < effectChnn; i++)
			{
				int index = m_rowChnnMap.find(i)->second;

				if(index < sensorInNum)
				{
					if(bAll)
					{
						pValue[index] = m_pAllTriggerInfo->SnapCH;
					}
					else
					{
						pValue[index] = m_pTriggerInfo[index].SnapCH;
					}
				}
			}
		}
		return true;
		break;
	case NCFG_ITEM_SENSOR_TO_BIG_VIEW:
		{
			assert( NULL != m_pTriggerInfo );
			assert( len == ( sizeof(ULONGLONG) * sensorInNum ) );
			ULONGLONG *pValue = reinterpret_cast<ULONGLONG *>(pData);

			for(BYTE i = 0; i < effectChnn; i++)
			{
				int index = m_rowChnnMap.find(i)->second;

				if(index < sensorInNum)
				{
					if( bAll )
					{
						pValue[index] = m_pAllTriggerInfo->FullScreen;
					}
					else
					{
						pValue[index] = m_pTriggerInfo[index].FullScreen;
					}
				}
			}
		}
		return true;
		break;
#ifdef __DVR_BASIC__	
	case NCFG_ITEM_SENSOR_IN_TO_EMAIL_ENABLE:
		{
			assert(len == sizeof(unsigned long) * sensorInNum);
			unsigned long* pValue = reinterpret_cast<unsigned long*>(pData);

			assert(NULL != m_pTriggerInfo);
			for(BYTE i = 0; i < effectChnn; i++)
			{
				int index = m_rowChnnMap.find(i)->second;

				if(index < sensorInNum)
				{
					if( bAll )
					{
						pValue[index] = m_pAllTriggerInfo->EmailEnable;
					}
					else
					{
						pValue[index] = m_pTriggerInfo[index].EmailEnable;
					}
				}
			}
		}
		return true;
		break;
	case NCFG_ITEM_SENSOR_TO_EMAIL_ADDR:
		{
			assert(len == sizeof(unsigned long) * sensorInNum);
			unsigned long* pValue = reinterpret_cast<unsigned long*>(pData);

			assert(NULL != m_pTriggerInfo);
			for(BYTE i = 0; i < effectChnn; i++)
			{
				int index = m_rowChnnMap.find(i)->second;

				if(index < sensorInNum)
				{
					if( bAll )
					{
						pValue[index] = m_pAllTriggerInfo->EmailAddr;
					}
					else
					{
						pValue[index] = m_pTriggerInfo[index].EmailAddr;
					}
				}
			}
		}
		return true;
		break;
#endif

#ifdef __ALARM_TO_UPDATE_FTP_VIDEO__
	case NCFG_ITEM_SENSOR_IN_TO_FTP_ENABLE:
		{
			ULONGLONG * pValue = reinterpret_cast<ULONGLONG *>(pData);

			assert(NULL != m_pTriggerInfo);
			for (BYTE i = 0; i < effectChnn; ++i)
			{
				int index = m_rowChnnMap.find(i)->second;
				if( bAll )
				{
					pValue[index] = m_pAllTriggerInfo->ftpVideo;
				}
				else
				{
					pValue[index] = m_pTriggerInfo[index].ftpVideo;
				}
			}
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

BEGIN_MSG_MAP(CCfgSensorHandingDlg, CCfgDlg)
	ON_MSG_EX(CTRLID_CFG_SENSOR_LIST, KEY_VIRTUAL_MSG_DEFAULT_TAG, OnClickList)
	ON_MSG_EX(m_listAll.GetID(), KEY_VIRTUAL_MSG_DEFAULT_TAG, OnClickListAll)
END_MSG_MAP()

unsigned long CCfgSensorHandingDlg::OnClickList(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	unsigned short row = static_cast<unsigned short>((keyID >> 16)&0x7fff);
	unsigned short col = static_cast<unsigned short>(keyID&0xffff);

	if(
#if !(defined(__CUSTOM_US02__) && defined(__CUSTOM_US02_NEW__))
		COL_HOLD_TIME == col ||
#endif
		COL_TRIGGER == col )
	{
		//2010-03-03 09:23:00 操作习惯 : 有操作非All部分内容, 就将All选项取消
		GUI::CCheckCtrl* pCheckAll = dynamic_cast<GUI::CCheckCtrl*>(m_listAll.GetItemWnd(0, COL_CHANNEL));
		assert(pCheckAll);
		if( pCheckAll->IsCheck() )
		{
			pCheckAll->SetCheck(false, true);

			GUI::CButton* pButtonAll = dynamic_cast<GUI::CButton*>(m_listAll.GetItemWnd(0, COL_TRIGGER));
			assert(NULL != pButtonAll);
			pButtonAll->SetEnable(pCheckAll->IsCheck(), true);
		}
	}

#if !(defined(__CUSTOM_US02__) && defined(__CUSTOM_US02_NEW__))
	if( COL_HOLD_TIME == col )
	{
		if( IsChange(NCFG_ITEM_SENSOR_IN_HOLD_TIME) )
		{
			SetModify(true);
			return KEY_VIRTUAL_MSG_CONTINUE;
		}
	}
#endif

	if (COL_TRIGGER == col)
	{
		int index = m_rowChnnMap.find(row)->second;

		CCfgTriggerDlg dlg;
		m_pTriggerInfo[index].PtzSafeChnn = -1;
		dlg.SetTriggerInfo(m_pTriggerInfo[index], index);
		if (DLG_OK == dlg.DoModal(this))
		{
			dlg.GetTriggerInfo(m_pTriggerInfo[index]);
			if (!IsModify())
			{
				if (IsChange(NCFG_ITEM_SENSOR_TO_BIG_VIEW) ||
					IsChange(NCFG_ITEM_SENSOR_IN_BUZZ) ||
					IsChange(NCFG_ITEM_SENSOR_IN_TO_ALARM_OUT) ||
					IsChange(NCFG_ITEM_SENSOR_IN_TO_RECORD) ||
#ifdef __DVR_BASIC__	
					IsChange(NCFG_ITEM_SENSOR_IN_TO_EMAIL_ENABLE)||
					IsChange(NCFG_ITEM_SENSOR_TO_EMAIL_ADDR)||
#endif					
					IsChange(NCFG_ITEM_SENSOR_IN_TO_FTP_ENABLE) ||
					IsChange(NCFG_ITEM_SENSOR_IN_TO_PTZ) ||
					IsChange(NCFG_ITEM_SENSOR_IN_TO_SNAP))
				{
					SetModify(true);
					return KEY_VIRTUAL_MSG_CONTINUE;
				}
			}			
		}
	}
	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CCfgSensorHandingDlg::OnClickListAll(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
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
			m_pAllTriggerInfo->PtzSafeChnn = -1;
			dlg.SetTriggerInfo(*m_pAllTriggerInfo, INVALID_INDEX(unsigned char));
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
			if( COL_TRIGGER == col
#if !(defined(__CUSTOM_US02__) && defined(__CUSTOM_US02_NEW__))
				|| COL_HOLD_TIME == col 
#endif
			)
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

