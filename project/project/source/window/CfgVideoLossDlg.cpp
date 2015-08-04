/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : hz
** Date         : 2009-08-01
** Name         :CfgVideoLossDlg.cpp 
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/
#include "CfgVideoLossDlg.h"

#include "FuncCustom.h"

CCfgVideoLossDlg::CCfgVideoLossDlg() : m_pAllTriggerInfo(NULL)
, m_pTriggerInfo(NULL)
{

}

CCfgVideoLossDlg::~CCfgVideoLossDlg()
{
	if( m_pTriggerInfo )
	{
		delete [] m_pTriggerInfo;
	}

	if( m_pAllTriggerInfo )
	{
		delete m_pAllTriggerInfo;
	}
}

//////////////////////////////////////////////////////////////////////////
void CCfgVideoLossDlg::OnInitial()
{
	CCfgDlg::OnInitial();
	SetImage(BMP_MEM_DLG_LARGE, false);
	SetCaption(m_pStrTable->String(E_STR_ID_TITLE_VIDEO_LOSS), false);

	SetDefaultBtn( DEFAULT_BTN_APPLY | DEFAULT_BTN_OK | DEFAULT_BTN_CANCEL);
	SetDefaultBtnCaption( DEFAULT_BTN_CANCEL, m_pStrTable->String(E_STR_ID_EXIT) );
	SetDefaultBtnCaption(DEFAULT_BTN_OK,m_pStrTable->String(E_STR_ID_APPLY));
	SetDefaultBtnCaption(DEFAULT_BTN_APPLY,m_pStrTable->String(E_STR_ID_DEFAULT));
	//////////////////////////////////////////////////////////////////////////

	unsigned short x = m_length[DLG_OFFSET_LEFT], y = m_length[TITLE_HEIGHT] +  2 * m_length[DLG_OFFSET_TOP];
	unsigned short width = m_rect.Width() - 2 * m_length[DLG_OFFSET_LEFT];
	unsigned short height = m_rect.Height() - y - 2 * m_length[LIST_ITEM_HEIGHT] - m_length[DEFAULT_BTN_BOTTOM_YPOS] - m_length[DLG_OFFSET_BOTTOM];


	ULONGLONG CH = 0;
	int localVideoInputNum = 0, netVideoInputNum = 0;
	int maxChnn = 0, effectChnn = 0;

	IsSupportCH(CH, &effectChnn, &localVideoInputNum, &netVideoInputNum);

	maxChnn = localVideoInputNum + netVideoInputNum;

// #if !defined(__CUSTOM_SHANGHAI__)
// 	maxChnn = localVideoInputNum;
// 	effectChnn = localVideoInputNum;
// #endif
	//localVideoInputNum ? (maxChnn = localVideoInputNum+ netVideoInputNum):(maxChnn = localVideoInputNum),;          

	int row = 0;
	for(int i = 0; i < maxChnn; i++)
	{
		if((CH >> i) & 0x1)
		{
			m_rowChnnMap.insert(std::make_pair(row++, i));
		}
	}

	m_listCtrl.Create(GetRandWndID(), x, y, width, height, this, 0, 1, false);
	m_listCtrl.AddColumn("", width * 2 / 32 );
	m_listCtrl.AddColumn("", width * 8 / 32 );

	char str[32] = {0};
	int chnn_count = maxChnn;  //s_loginSuccessReply.productInfo.localVideoInputNum;
	GUI::CWnd * pWnd = NULL;
	for (int ch = 0; ch < effectChnn; ++ch)
	{
		int chnnIndex = m_rowChnnMap.find(ch)->second;
		
		sprintf(str, "%d", chnnIndex + 1);
		m_listCtrl.AddItem(ch, COL_CHANNEL, str);

		pWnd = m_listCtrl.AddItem(ch, COL_TRIGGER, WND_TYPE_BUTTON);
		pWnd->SetCaption(m_pStrTable->String(E_STR_ID_TRIGGER), false);
	}

	m_listCtrl.CreateListFocusMap();

	GUI::CRect rect = m_listCtrl.GetClientRect();
	y += rect.Height() + 4;

	m_scAll.Create( GUI::CWnd::GetRandWndID(), x, y + (m_length[LIST_ITEM_HEIGHT]-m_length[STATIC_HEIGHT])/2, width, m_length[STATIC_HEIGHT], this );
	m_scAll.SetCaption( m_pStrTable->String(E_STR_ID_ALL), false );

	y += m_length[LIST_ITEM_HEIGHT];
	m_listAll.Create( GUI::CWnd::GetRandWndID(), x, y, width, m_length[LIST_ITEM_HEIGHT], this, 0, 2, false );
	m_listAll.AddColumn("", width * 2 / 32 );
	m_listAll.AddColumn("", width * 8 / 32 );
	{
		int ch = 0;
		m_listAll.AddItem(ch, COL_CHANNEL, WND_TYPE_CHECK);

		pWnd = m_listAll.AddItem(ch, COL_TRIGGER, WND_TYPE_BUTTON);
		pWnd->SetCaption(m_pStrTable->String(E_STR_ID_TRIGGER), false);
	}
	m_listAll.CreateListFocusMap();

	//////////////////////////////////////////////////////////////////////////
	m_pTriggerInfo = new ALARM_TRIGGER_INFO[maxChnn];	
	m_pAllTriggerInfo = new ALARM_TRIGGER_INFO;
	memset(m_pTriggerInfo, 0, sizeof(ALARM_TRIGGER_INFO) * maxChnn);
	memset(m_pAllTriggerInfo, 0, sizeof(ALARM_TRIGGER_INFO));

	CreateTipWnd();
}

void CCfgVideoLossDlg::OnDestroy()
{
	CCfgDlg::OnDestroy();

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

unsigned long CCfgVideoLossDlg::OnApply()
{
	//Load default
	unsigned long ret = OnLoadDefault(this);

	if( IsModify() )
	{
		EnableOkBtn(true,false);
	}
	else
	{
		EnableOkBtn(false,false);
	}

	ENUM_STRING_ID strID = ret ? static_cast<ENUM_STRING_ID>(ret) : E_STR_ID_TIP_00_03;
	MessageOut(strID,TIP_MSG,false);

	//一次重绘全部
	Repaint();

	return KEY_VIRTUAL_MSG_STOP;
}


void CCfgVideoLossDlg::UpdateView(bool bRefresh)
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

void CCfgVideoLossDlg::GetItemList(CFGITEM_LIST& itemList) const
{

#ifdef __DVR_BASIC__
	itemList.push_back(NCFG_ITEM_EMAIL_RECV_INFO);
#endif

	itemList.push_back(NCFG_ITEM_VL_TO_ALARM_OUT);
	itemList.push_back(NCFG_ITEM_VL_TO_BIG_VIEW);
#ifdef __DVR_BASIC__	
	itemList.push_back(NCFG_ITEM_VL_TO_EMAIL_ENABLE);
	itemList.push_back(NCFG_ITEM_VL_TO_EMAIL_ADDR);
#endif
	//if(s_loginSuccessReply.productInfo.localVideoInputNum > 0)   // localVideoInputNum 改为 videoInputNum   xian
	{
		itemList.push_back(NCFG_ITEM_VL_TO_PTZ);
#ifdef __ALARM_TO_UPDATE_FTP_VIDEO__
		itemList.push_back(NCFG_ITEM_VL_TO_FTP_ENABLE);
#endif
	}
	itemList.push_back(NCFG_ITEM_VL_TO_BUZZ);
	itemList.push_back(NCFG_ITEM_VL_TO_SNAP);
}

bool CCfgVideoLossDlg::ShowData(NCFG_ITEM_ID cfgID, const BYTE* pData, int len, bool bRefresh)
{
	assert(NULL != pData);
	assert(len > 0);

	ULONGLONG CH = 0;
	int localVideoInputNum = 0, netVideoInputNum = 0;
	int maxChnn = 0, effectChnn = 0;

	IsSupportCH(CH, &effectChnn, &localVideoInputNum, &netVideoInputNum);
	maxChnn = localVideoInputNum + netVideoInputNum;

// #if !defined(__CUSTOM_SHANGHAI__)
// 	maxChnn = localVideoInputNum;
// 	effectChnn = localVideoInputNum;
// #endif

	int count = maxChnn;  //s_loginSuccessReply.productInfo.localVideoInputNum;

	switch (cfgID)
	{
	case NCFG_ITEM_VL_TO_BUZZ:
		{
			assert(len == sizeof(unsigned long) * count);
			const unsigned long* pValue = reinterpret_cast<const unsigned long*>(pData);

			assert(NULL != m_pTriggerInfo);
			for (BYTE i = 0; i < effectChnn; i++)
			{
				int index = m_rowChnnMap.find(i)->second;
				m_pTriggerInfo[index].Buzzer = pValue[index];
			}
		}
		return true;
		break;
	case NCFG_ITEM_VL_TO_ALARM_OUT:
		{
			assert(len == sizeof(ULONGLONG) * count);
			const ULONGLONG* pValue = reinterpret_cast<const ULONGLONG*>(pData);

			assert(NULL != m_pTriggerInfo);
			for (BYTE i = 0; i < effectChnn; i++)
			{
				int index = m_rowChnnMap.find(i)->second;
				m_pTriggerInfo[index].AlarmOut = pValue[index];
			}
		}
		return true;
		break;
	case NCFG_ITEM_VL_TO_RECORD:
		{
			assert(len == sizeof(ULONGLONG) * count);
			const ULONGLONG* pValue = reinterpret_cast<const ULONGLONG*>(pData);

			assert(NULL != m_pTriggerInfo);
			for (BYTE i = 0; i < effectChnn; i++)
			{
				int index = m_rowChnnMap.find(i)->second;
				m_pTriggerInfo[index].Record = pValue[index];
			}
		}
		return true;
		break;
	case NCFG_ITEM_VL_TO_PTZ:
		{
			BYTE video_count = count; //s_loginSuccessReply.productInfo.localVideoInputNum;

			assert(len == sizeof(TO_PTZ) * count * video_count);
			const TO_PTZ* pValue = reinterpret_cast<const TO_PTZ*>(pData);

			assert(NULL != m_pTriggerInfo);
			for (BYTE i = 0; i < effectChnn; ++i)
			{
				int index = m_rowChnnMap.find(i)->second;
				for (BYTE ch = 0; ch < video_count; ch++)
				{
					m_pTriggerInfo[index].ToPtz[ch] = pValue[video_count * index + ch];
				}				
			}
		}
		return true;
		break;
#ifdef __ALARM_TO_UPDATE_FTP_VIDEO__
	case NCFG_ITEM_VL_TO_FTP_ENABLE:
		{
			unsigned char localVideoInputNum = s_loginSuccessReply.productInfo.localVideoInputNum;
			assert(len == sizeof(ULONGLONG) * localVideoInputNum);
			const ULONGLONG* pValue = reinterpret_cast<const ULONGLONG*>(pData);

			assert(NULL != m_pTriggerInfo);
			for (BYTE i = 0; i < localVideoInputNum; ++i, ++pValue)
			{
				m_pTriggerInfo[i].ftpVideo = *pValue;
			}
		}
		return true;
		break;
#endif
	case NCFG_ITEM_VL_TO_SNAP:
		{
			assert(len == sizeof(ULONGLONG) * count);

			const ULONGLONG* pValue = reinterpret_cast<const ULONGLONG*>(pData);

			for(BYTE i = 0; i < effectChnn; i++)
			{
				int index = m_rowChnnMap.find(i)->second;
				m_pTriggerInfo[index].SnapCH = pValue[index];
			}
		}
		return true;
		break;
	case NCFG_ITEM_VL_TO_BIG_VIEW:
		{
			assert( sizeof( ULONGLONG ) * count == len );
			const ULONGLONG *pValue = reinterpret_cast<const ULONGLONG*>(pData);
			assert( NULL != m_pTriggerInfo );
			for( BYTE i = 0; i < effectChnn; i++)
			{
				int index = m_rowChnnMap.find(i)->second;
				m_pTriggerInfo[index].FullScreen = pValue[index];
			}
		}
		return true;
		break;
#ifdef __DVR_BASIC__	
	case NCFG_ITEM_VL_TO_EMAIL_ENABLE:
		{
			assert(len == sizeof(unsigned long) * count);
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
	case NCFG_ITEM_VL_TO_EMAIL_ADDR:
		{
			//获得到配置里前3个收件人地址
			unsigned long tempDataLen = 0;
			BYTE* pTempData = NULL;
			CCfgDlg::GetConfig(NCFG_ITEM_EMAIL_RECV_INFO, &pTempData, tempDataLen);
			char *pInfo = reinterpret_cast<char *>(pTempData);
			if(pInfo != NULL)
			{
				for(BYTE i = 0; i < effectChnn; i++)	
				{
					memcpy(m_pTriggerInfo[m_rowChnnMap.find(i)->second].AddrInfo, pInfo, sizeof(m_pTriggerInfo[0].AddrInfo));
				}
			}

			assert(len == sizeof(unsigned long) * count);
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
#endif
	default:
		return false;
		break;
	}

	return false;
}

bool CCfgVideoLossDlg::SetData(NCFG_ITEM_ID cfgID, BYTE*& pData, int& len)  const
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
	int localVideoInputNum = 0, netVideoInputNum = 0;
	int maxChnn = 0, effectChnn = 0;

	IsSupportCH(CH, &effectChnn, &localVideoInputNum, &netVideoInputNum);

	maxChnn = localVideoInputNum + netVideoInputNum;

// #if !defined(__CUSTOM_SHANGHAI__)
// 	maxChnn = localVideoInputNum;
// 	effectChnn = localVideoInputNum;
// #endif

	int count = maxChnn;  //s_loginSuccessReply.productInfo.localVideoInputNum;

	switch (cfgID)
	{
	case NCFG_ITEM_VL_TO_BUZZ:
		{
			assert(len == sizeof(unsigned long) * count);
			unsigned long* pValue = reinterpret_cast<unsigned long*>(pData);

			if (bAll)
			{
				assert(NULL != m_pAllTriggerInfo);
				for (BYTE i = 0; i < effectChnn; i++)
				{
					pValue[m_rowChnnMap.find(i)->second] = m_pAllTriggerInfo->Buzzer;
				}
			} 
			else
			{
				assert(NULL != m_pTriggerInfo);
				for (BYTE i = 0; i < effectChnn; i++)
				{
					int index = m_rowChnnMap.find(i)->second;
					pValue[index] = m_pTriggerInfo[index].Buzzer;
				}
			}	
		}
		return true;
		break;
	case NCFG_ITEM_VL_TO_ALARM_OUT:
		{
			assert(len == sizeof(ULONGLONG) * count);
			ULONGLONG* pValue = reinterpret_cast<ULONGLONG*>(pData);

			if (bAll)
			{
				assert(NULL != m_pAllTriggerInfo);
				for (BYTE i = 0; i < effectChnn; i++)
				{
					pValue[m_rowChnnMap.find(i)->second] = m_pAllTriggerInfo->AlarmOut;
				}
			} 
			else
			{
				assert(NULL != m_pTriggerInfo);
				for (BYTE i = 0; i < effectChnn; i++)
				{
					int index = m_rowChnnMap.find(i)->second;
					pValue[index] = m_pTriggerInfo[index].AlarmOut;
				}
			}	
		}
		return true;
		break;
	case NCFG_ITEM_VL_TO_RECORD:
		{
			assert(len == sizeof(ULONGLONG) * count);
			ULONGLONG* pValue = reinterpret_cast<ULONGLONG*>(pData);

			if (bAll)
			{
				assert(NULL != m_pAllTriggerInfo);
				for (BYTE i = 0; i < effectChnn; i++)
				{
					pValue[m_rowChnnMap.find(i)->second] = m_pAllTriggerInfo->Record;
				}
			} 
			else
			{
				assert(NULL != m_pTriggerInfo);
				for (BYTE i = 0; i < effectChnn; i++)
				{
					int index = m_rowChnnMap.find(i)->second;
					pValue[index] = m_pTriggerInfo[index].Record;
				}
			}	
		}
		return true;
		break;
	case NCFG_ITEM_VL_TO_PTZ:
		{
			BYTE video_count = count;  //s_loginSuccessReply.productInfo.localVideoInputNum;
			assert(len == sizeof(TO_PTZ) * count * video_count);
			TO_PTZ* pValue = reinterpret_cast<TO_PTZ*>(pData);

			if (bAll)
			{
				assert(NULL != m_pTriggerInfo);
				for (BYTE i = 0; i < effectChnn; ++i)
				{
					int index = m_rowChnnMap.find(i)->second;
					for (BYTE ch = 0; ch < video_count; ch++)
					{
						pValue[video_count * index + ch] = m_pAllTriggerInfo->ToPtz[ch];
					}				
				}
			} 
			else
			{
				assert(NULL != m_pTriggerInfo);
				for (BYTE i = 0; i < effectChnn; ++i)
				{
					int index = m_rowChnnMap.find(i)->second;
					for (BYTE ch = 0; ch < video_count; ch++)
					{
						pValue[video_count * index + ch] = m_pTriggerInfo[index].ToPtz[ch];
					}				
				}
			}
		}
		return true;
		break;
#ifdef __ALARM_TO_UPDATE_FTP_VIDEO__
	case NCFG_ITEM_VL_TO_FTP_ENABLE:
		{
			ULONGLONG * pValue = reinterpret_cast<ULONGLONG *>(pData);

			assert(NULL != m_pTriggerInfo);
			for (BYTE i = 0; i < s_loginSuccessReply.productInfo.localVideoInputNum; ++i)
			{
				if( bAll )
				{
					pValue[i] = m_pAllTriggerInfo->ftpVideo;
				}
				else
				{
					pValue[i] = m_pTriggerInfo[i].ftpVideo;
				}
			}
		}
		return true;
		break;
#endif
	case NCFG_ITEM_VL_TO_SNAP:
		{
			assert(len == sizeof(ULONGLONG) * count);
			ULONGLONG* pValue = reinterpret_cast<ULONGLONG*>(pData);

			for(BYTE i = 0; i < effectChnn; i++)
			{
				int index = m_rowChnnMap.find(i)->second;
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
		return true;
		break;
	case NCFG_ITEM_VL_TO_BIG_VIEW:
		{
			assert( sizeof( ULONGLONG ) * count == len );
			assert( NULL != m_pTriggerInfo );
			ULONGLONG *pValue = reinterpret_cast<ULONGLONG*>(pData);
			for( BYTE i = 0; i < effectChnn; i++)
			{
				int index = m_rowChnnMap.find(i)->second;
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
		return true;
		break;
#ifdef __DVR_BASIC__	
	case NCFG_ITEM_VL_TO_EMAIL_ENABLE:
		{
			assert(len == sizeof(unsigned long) * count);
			unsigned long* pValue = reinterpret_cast<unsigned long*>(pData);

			if (bAll)
			{
				assert(NULL != m_pAllTriggerInfo);
				for (BYTE i = 0; i < effectChnn; i++)
				{
					pValue[m_rowChnnMap.find(i)->second] = m_pAllTriggerInfo->EmailEnable;
				}
			} 
			else
			{
				assert(NULL != m_pTriggerInfo);
				for (BYTE i = 0; i < effectChnn; i++)
				{
					int index = m_rowChnnMap.find(i)->second;
					pValue[index] = m_pTriggerInfo[index].EmailEnable;
				}
			}	
		}
		return true;
		break;
	case NCFG_ITEM_VL_TO_EMAIL_ADDR:
		{
			assert(len == sizeof(unsigned long) * count);
			unsigned long* pValue = reinterpret_cast<unsigned long*>(pData);

			if (bAll)
			{
				assert(NULL != m_pAllTriggerInfo);
				for(BYTE i = 0; i < effectChnn; i++)
				{
					pValue[m_rowChnnMap.find(i)->second] = m_pAllTriggerInfo->EmailAddr;
				}
			}
			else
			{
				assert(NULL != m_pTriggerInfo);
				for(BYTE i = 0; i < effectChnn; i++)
				{
					int index = m_rowChnnMap.find(i)->second;
					pValue[index] = m_pTriggerInfo[index].EmailAddr;
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


BEGIN_MSG_MAP(CCfgVideoLossDlg, CCfgDlg)
	ON_MSG_EX(m_listCtrl.GetID(), KEY_VIRTUAL_MSG_DEFAULT_TAG, OnClickList)
	ON_MSG_EX(m_listAll.GetID(), KEY_VIRTUAL_MSG_DEFAULT_TAG, OnClickListAll)
END_MSG_MAP()


unsigned long CCfgVideoLossDlg::OnClickList(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	unsigned short row = static_cast<unsigned short>((keyID >> 16)&0x7fff);
	unsigned short col = static_cast<unsigned short>(keyID&0xffff);

	//2010-03-03 09:23:00 操作习惯 : 有操作非All部分内容, 就将All选项取消
	GUI::CCheckCtrl* pCheckAll = dynamic_cast<GUI::CCheckCtrl*>(m_listAll.GetItemWnd(0, COL_CHANNEL));
	assert(pCheckAll);
	if( pCheckAll->IsCheck() )
	{
		pCheckAll->SetCheck(false, true);

		GUI::CButton* pBtnTrgAll = dynamic_cast<GUI::CButton*>(m_listAll.GetItemWnd(0, COL_TRIGGER));
		assert(pBtnTrgAll);
		pBtnTrgAll->SetEnable(pCheckAll->IsCheck(), true);
	}

	if (COL_TRIGGER == col)
	{
		int index = m_rowChnnMap.find(row)->second;

		CCfgTriggerDlg dlg;
		dlg.SetShowPage(CCfgTriggerDlg::PAGE_ALL_SHOW & (~CCfgTriggerDlg::PAGE_TO_RECORD_SHOW) & (~CCfgTriggerDlg::PAGE_TO_FTP_SHOW));
		m_pTriggerInfo[index].PtzSafeChnn = -1;
		dlg.SetTriggerInfo(m_pTriggerInfo[index], index);
		if (DLG_OK == dlg.DoModal(this))
		{
			dlg.GetTriggerInfo(m_pTriggerInfo[index]);
			if (!IsModify())
			{
				if (IsChange(NCFG_ITEM_VL_TO_BIG_VIEW)  ||
					IsChange(NCFG_ITEM_VL_TO_ALARM_OUT) ||
					IsChange(NCFG_ITEM_VL_TO_BUZZ) ||
					(IsChange(NCFG_ITEM_VL_TO_RECORD)) ||
#ifdef __DVR_BASIC__	
					(IsChange(NCFG_ITEM_VL_TO_EMAIL_ENABLE)) ||
					(IsChange(NCFG_ITEM_VL_TO_EMAIL_ADDR)) ||
#endif
					(IsChange(NCFG_ITEM_VL_TO_PTZ)) ||
					(IsChange(NCFG_ITEM_VL_TO_FTP_ENABLE)) ||
					(IsChange(NCFG_ITEM_VL_TO_SNAP)))
				{
					SetModify(true);
					EnableOkBtn(true, true);
				}
			}
		}
	}

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CCfgVideoLossDlg::OnClickListAll(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
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

	assert( pCheckAll );
	if( pCheckAll->IsCheck() )
	{
		if (COL_TRIGGER == col)
		{
			CCfgTriggerDlg dlg;
			dlg.SetShowPage(CCfgTriggerDlg::PAGE_ALL_SHOW & (~CCfgTriggerDlg::PAGE_TO_RECORD_SHOW) );
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

		if (!IsModify())
		{
			if( COL_TRIGGER == col)
			{
				SetModify(true);
				return KEY_VIRTUAL_MSG_CONTINUE;
			}
		}
	}

	// 点选All 直接认为做了修改
	if (!IsModify())
	{
		if( COL_CHANNEL == col )
		{
			SetModify(true);
			EnableOkBtn(true, true);
			return KEY_VIRTUAL_MSG_CONTINUE;
		}
	}

	return KEY_VIRTUAL_MSG_STOP;
}

