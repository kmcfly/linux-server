/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : hz
** Date         : 2009-08-01
** Name         : CfgAlarmManDlg.cpp
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/
#include "CfgAlarmOutDlg.h"

unsigned long CCfgAlarmOutDlg::s_holdTime[] = {5, 10, 20, 30, 60, 120, -1};

//////////////////////////////////////////////////////////////////////////

CCfgAlarmOutManDlg::CCfgAlarmOutManDlg()
{

}

CCfgAlarmOutManDlg::~CCfgAlarmOutManDlg()
{

}

void CCfgAlarmOutManDlg::OnPreInitial()
{
#ifndef __DVR_ANCHOR__
	AddCaption(m_pStrTable->String(E_STR_ID_TITLE_ALARM_OUT));
	AddCaption(m_pStrTable->String(E_STR_ID_ALARM_OUT));
	AddCaption(m_pStrTable->String(E_STR_ID_SCHEDULE));
	AddCaption(m_pStrTable->String(E_STR_ID_BUZZER));
#else
	AddCaption(m_pStrTable->String(E_STR_ID_TITLE_ALARM_OUT));
	AddCaption(m_pStrTable->String(E_STR_ID_TITLE_ALARM_OUT));
	AddCaption(m_pStrTable->String(E_STR_ID_TITLE_SCHEDULE));
	AddCaption(m_pStrTable->String(E_STR_ID_TITLE_BUZZER));
#endif
	CCfgManDlg3<CCfgAlarmOutDlg, CCfgScheduleChildDlg, CCfgAlarmOutBuzzerDlg>::OnPreInitial();
	GetPage2().SetMaxChnn(s_loginSuccessReply.productInfo.relayOutputNum, CCfgScheduleChildDlg::TYPE_ALL);
	GetPage2().SetScheduleCfgID(NCFG_ITEM_ALARMOUT_SCH_WEEK,NCFG_ITEM_ALARMOUT_SCH_HOLIDAY);
}

void CCfgAlarmOutManDlg::GetItemList(CFGITEM_LIST& itemList) const
{
	itemList.push_back(NCFG_ITEM_ALARMOUT_SCH_WEEK);

	itemList.push_back(NCFG_ITEM_BUZZER_ENABLE);
	itemList.push_back(NCFG_ITEM_BUZZER_HOLD_TIME);
	itemList.push_back(NCFG_ITEM_ALARM_OUT_NAME);
	itemList.push_back(NCFG_ITEM_ALARM_OUT_HOLD_TIME);

	itemList.push_back(NCFG_ITEM_ALARM_OUT_NAME_MAX_LEN);
}

//////////////////////////////////////////////////////////////////////////

CCfgAlarmOutDlg::CCfgAlarmOutDlg()
{

}

CCfgAlarmOutDlg::~CCfgAlarmOutDlg()
{

}

//////////////////////////////////////////////////////////////////////////
void CCfgAlarmOutDlg::OnInitial()
{
	CCfgDlg::OnInitial();
	//////////////////////////////////////////////////////////////////////////

	unsigned short x = m_length[DLG_OFFSET_SUB_LEFT], y = m_length[DLG_OFFSET_SUB_TOP];
	unsigned short width = m_rect.Width() - 2 * m_length[DLG_OFFSET_SUB_LEFT];
	unsigned short height = m_rect.Height() - y - m_length[DLG_OFFSET_SUB_BOTTOM] - 2 * m_length[LIST_ITEM_HEIGHT];

	char str[32] = {0};
	int chnn_count = s_loginSuccessReply.productInfo.relayOutputNum;
	int hold_count = sizeof(s_holdTime)/sizeof(s_holdTime[0]);
	GUI::CWnd * pWnd = NULL;
	GUI::CComboCtrl* pCombo = NULL;
	
	//////////////////////////////////////////////////////////////////////////

	m_listCtrl.Create(GetRandWndID(), x, y, width, height, this, 0, 1);
	m_listCtrl.AddColumn(m_pStrTable->String(E_STR_ID_CH), width * 3 / 32);
	m_listCtrl.AddColumn(m_pStrTable->String(E_STR_ID_RELAY_NAME), width * 18 / 32);
	m_listCtrl.AddColumn(m_pStrTable->String(E_STR_ID_HOLD_TIME), width * 11 / 32);

	for (int ch = 0; ch < chnn_count; ++ch)
	{
		sprintf(str, "%d", ch + 1);
		m_listCtrl.AddItem(ch, COL_CHANNEL, str);

		pCombo = reinterpret_cast<GUI::CComboCtrl*>(m_listCtrl.AddItem(ch, COL_HOLD_TIME, WND_TYPE_COMBO));
		assert(NULL != pCombo);
		for (int time = 0; time < hold_count; ++time)
		{
			if (-1 != s_holdTime[time])
			{
				sprintf(str, "%d", s_holdTime[time]);
				pCombo->AddItem(str, s_holdTime[time]);
			} 
			else
			{
				pCombo->AddItem(m_pStrTable->String(E_STR_ID_ALWAYS), s_holdTime[time]);
			}			
		}
		pCombo->SetDropDownCount(7);
		
		pWnd = m_listCtrl.AddItem(ch, COL_NAME, WND_TYPE_EDIT);
	}

	m_listCtrl.CreateListFocusMap();

	GUI::CRect rect = m_listCtrl.GetWindowRect();
	y += rect.Height() + 4;

	m_scAll.Create( GUI::CWnd::GetRandWndID(), x, y + (m_length[LIST_ITEM_HEIGHT]-m_length[STATIC_HEIGHT])/2, width, m_length[STATIC_HEIGHT], this );
	m_scAll.SetCaption( m_pStrTable->String(E_STR_ID_ALL), false );

	y += m_length[LIST_ITEM_HEIGHT];
	m_listAll.Create( GUI::CWnd::GetRandWndID(), x, y, width, m_length[LIST_ITEM_HEIGHT], this, 0, 2, false );
	m_listAll.AddColumn("", width * 3 / 32);
	m_listAll.AddColumn("", width * 18 / 32);
	m_listAll.AddColumn(m_pStrTable->String(E_STR_ID_HOLD_TIME), width * 11 / 32);

	{
		int ch = 0;

		pWnd = m_listAll.AddItem(ch, COL_CHANNEL, WND_TYPE_CHECK );

		pCombo = reinterpret_cast<GUI::CComboCtrl*>(m_listAll.AddItem(ch, COL_HOLD_TIME, WND_TYPE_COMBO));
		assert(NULL != pCombo);
		for (int time = 0; time < hold_count; ++time)
		{
			if (-1 != s_holdTime[time])
			{
				sprintf(str, "%d", s_holdTime[time]);
				pCombo->AddItem(str, s_holdTime[time]);
			} 
			else
			{
				pCombo->AddItem(m_pStrTable->String(E_STR_ID_ALWAYS), s_holdTime[time]);
			}			
		}
		pCombo->SetCurItem(1, false);
		pCombo->SetDropDownCount(7);
	}
	m_listAll.CreateListFocusMap();
}

void CCfgAlarmOutDlg::UpdateView(bool bRefresh)
{
	GUI::CCheckCtrl* pCheckAll = dynamic_cast<GUI::CCheckCtrl*>(m_listAll.GetItemWnd(0, COL_CHANNEL));
	if( pCheckAll )
	{
		pCheckAll->SetCheck( false, bRefresh );
	}

	CCfgDlg::UpdateView( bRefresh );
}

bool CCfgAlarmOutDlg::ShowData(NCFG_ITEM_ID cfgID, const BYTE* pData, int len, bool bRefresh)
{
	assert(NULL != pData);
	assert(len > 0);

	BYTE* pTempData = NULL;
	unsigned long tempDataLen = 0;
	GUI::CComboCtrl* pCombo = NULL;

	int count = s_loginSuccessReply.productInfo.relayOutputNum;
	switch (cfgID)
	{
	case NCFG_ITEM_ALARM_OUT_NAME:
		{
			assert(len == sizeof(NCFG_INFO_SHORT_NAME) * count);
			const NCFG_INFO_SHORT_NAME* pValue = reinterpret_cast<const NCFG_INFO_SHORT_NAME*>(pData);

			if(!GetConfig(NCFG_ITEM_ALARM_OUT_NAME_MAX_LEN, &pTempData, tempDataLen))
			{
				assert(false);
			}
			assert(tempDataLen == sizeof(unsigned long));
			unsigned long max_len = *(reinterpret_cast<unsigned long*>(pTempData));
			assert(max_len > 0);

			GUI::CEditCtrl * pEditCtrl = NULL;
			for (int i = 0; i < count; ++i, ++pValue)
			{
				pEditCtrl = dynamic_cast<GUI::CEditCtrl*>(m_listCtrl.GetItemWnd(i, COL_NAME));
				assert(NULL != pEditCtrl);
				pEditCtrl->SetMaxStringLen(max_len);
				pEditCtrl->SetCaption(pValue->name, false);
			}
		}
		return true;
		break;
	case NCFG_ITEM_ALARM_OUT_HOLD_TIME:
		{
			assert(len == sizeof(unsigned long) * count);
			const unsigned long* pValue = reinterpret_cast<const unsigned long*>(pData);

			GUI::CComboCtrl* pCombo = NULL;
			for (BYTE i = 0; i < count; ++i, ++pValue)
			{
				pCombo = dynamic_cast<GUI::CComboCtrl*>(m_listCtrl.GetItemWnd(i, COL_HOLD_TIME));
				assert(NULL != pCombo);
				pCombo->SetCurItemData(*pValue, bRefresh);
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

bool CCfgAlarmOutDlg::SetData(NCFG_ITEM_ID cfgID, BYTE*& pData, int& len)  const
{
	assert(NULL != pData);
	assert(len > 0);
	const GUI::CComboCtrl* pCombo = NULL;

	const GUI::CListCtrl& listAll = m_listAll;

	bool bAll = false;
	{
		const GUI::CCheckCtrl* pCheck = dynamic_cast<const GUI::CCheckCtrl*>( listAll.GetItemWnd( 0, COL_CHANNEL ) );
		assert( pCheck );
		bAll = pCheck->IsCheck();
	}

	int count = s_loginSuccessReply.productInfo.relayOutputNum;

	switch (cfgID)
	{
	case NCFG_ITEM_ALARM_OUT_NAME:
		{
			assert(len == sizeof(NCFG_INFO_SHORT_NAME) * count);
			NCFG_INFO_SHORT_NAME* pValue = reinterpret_cast<NCFG_INFO_SHORT_NAME*>(pData);

			const GUI::CEditCtrl * pEditCtrl = NULL;
			for (int i = 0; i < count; ++i, ++pValue)
			{
				pEditCtrl = dynamic_cast<const GUI::CEditCtrl*>(m_listCtrl.GetItemWnd(i, COL_NAME));
				assert(NULL != pEditCtrl);
				strcpy(pValue->name, pEditCtrl->GetCaption().c_str());
			}
		}
		return true;
		break;
	case NCFG_ITEM_ALARM_OUT_HOLD_TIME:
		{
			assert(len == sizeof(unsigned long) * count);
			unsigned long* pValue = reinterpret_cast<unsigned long*>(pData);

			const GUI::CComboCtrl* pCombo = NULL;

			if (bAll)
			{
				for (BYTE i = 0; i < count; ++i, ++pValue)
				{
					pCombo = dynamic_cast<const GUI::CComboCtrl*>(listAll.GetItemWnd(0, COL_HOLD_TIME));
					assert(NULL != pCombo);
					*pValue = pCombo->GetCurItemData();
				}
			} 
			else
			{
				for (BYTE i = 0; i < count; ++i, ++pValue)
				{
					pCombo = dynamic_cast<const GUI::CComboCtrl*>(m_listCtrl.GetItemWnd(i, COL_HOLD_TIME));
					assert(NULL != pCombo);
					*pValue = pCombo->GetCurItemData();
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

BEGIN_MSG_MAP(CCfgAlarmOutDlg, CCfgDlg)
	ON_MSG_EX(m_listCtrl.GetID(), KEY_VIRTUAL_MSG_DEFAULT_TAG, OnClickList)
	ON_MSG_EX(m_listAll.GetID(), KEY_VIRTUAL_MSG_DEFAULT_TAG, OnClickListAll)
END_MSG_MAP()


unsigned long CCfgAlarmOutDlg::OnClickList(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	unsigned short row = (keyID >> 16)&0x7fff;	//行
	unsigned short col = keyID&0xffff;			//列

	//2010-03-03 09:23:00 操作习惯 : 有操作非All部分内容, 就将All选项取消
	GUI::CCheckCtrl* pCheckAll = dynamic_cast<GUI::CCheckCtrl*>(m_listAll.GetItemWnd(0, COL_CHANNEL));
	assert(pCheckAll);
	if( pCheckAll->IsCheck() )
	{
		pCheckAll->SetCheck(false, true);
	}

	if (!IsModify())
	{
		switch (col)
		{
		case COL_HOLD_TIME:
			if (IsChange(NCFG_ITEM_ALARM_OUT_HOLD_TIME))
			{
				SetModify(true);
				return KEY_VIRTUAL_MSG_CONTINUE;
			}
			break;
		case COL_NAME:
			if (IsChange(NCFG_ITEM_ALARM_OUT_NAME))
			{
				SetModify(true);
				return KEY_VIRTUAL_MSG_CONTINUE;
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

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CCfgAlarmOutDlg::OnClickListAll(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	unsigned short row = static_cast<unsigned short>((keyID >> 16)&0x7fff);
	unsigned short col = static_cast<unsigned short>(keyID&0xffff);

	GUI::CCheckCtrl* pCheckAll = dynamic_cast<GUI::CCheckCtrl*>(m_listAll.GetItemWnd(0, COL_CHANNEL));
	CCfgDlg* pCfgDlg = dynamic_cast<CCfgDlg*>( GetParent() );

	assert( pCheckAll );
	if( pCheckAll->IsCheck() )
	{
		pCfgDlg->SetRefreshView( true );
		if (!IsModify())
		{
			if( COL_HOLD_TIME == col)
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



//////////////////////////////////////////////////////////////////////////
// __DVR_ANCHOR__
//////////////////////////////////////////////////////////////////////////

CCfgAlarmOutManAnchorDlg::CCfgAlarmOutManAnchorDlg()
{

}

CCfgAlarmOutManAnchorDlg::~CCfgAlarmOutManAnchorDlg()
{

}

void CCfgAlarmOutManAnchorDlg::OnInitial()
{
	GUI::CDialog::OnInitial();
	SetImage(BMP_MEM_DLG_NORMAL, false);
	SetCaption(m_pStrTable->String(E_STR_ID_TITLE_ALARM_OUT), false);
	//////////////////////////////////////////////////////////////////////////
	unsigned short x = 200;
	unsigned short y = m_length[TITLE_HEIGHT] + m_length[DLG_OFFSET_TOP] + 25;
	unsigned short x_off = 60, y_off = 10;
	unsigned short cx = m_length[ANCHOR_WIDTH], cy = m_length[ANCHOR_HEIGHT];

	int yFocus = 1;

	m_anAlarmOut.Create(GUI::CWnd::GetRandWndID(), x, y, cx, cy, this, 0, yFocus++, m_pStrTable->String(E_STR_ID_ALARM_OUT));
	m_anAlarmOut.SetImage(BMP_ANCHOR_ALARM_OUT, BMP_ANCHOR_ALARM_OUT_F, BMP_ANCHOR_ALARM_OUT);

	y += 60;
	m_anSchedule.Create(GUI::CWnd::GetRandWndID(), x, y, cx, cy, this, 0, yFocus++, m_pStrTable->String(E_STR_ID_SCHEDULE));
	m_anSchedule.SetImage(BMP_ANCHOR_SCHEDULE, BMP_ANCHOR_SCHEDULE_F, BMP_ANCHOR_SCHEDULE);

	y += 60;
	m_anBuzzer.Create(GUI::CWnd::GetRandWndID(), x, y, cx, cy, this, 0, yFocus++, m_pStrTable->String(E_STR_ID_BUZZER));
	m_anBuzzer.SetImage(BMP_ANCHOR_BUZZER, BMP_ANCHOR_BUZZER_F, BMP_ANCHOR_BUZZER);

	y += 60;
	m_anExit.Create(GUI::CWnd::GetRandWndID(), x, y, cx, cy, this, 0, yFocus++, m_pStrTable->String(E_STR_ID_EXIT));
	m_anExit.SetImage(BMP_ANCHOR_EXIT, BMP_ANCHOR_EXIT_F, BMP_ANCHOR_EXIT);
}

BEGIN_MSG_MAP(CCfgAlarmOutManAnchorDlg, GUI::CDialog)
	ON_MSG(m_anAlarmOut.GetID(), KEY_VIRTUAL_ENTER, OnAnchorAlarmOut)
	ON_MSG(m_anSchedule.GetID(), KEY_VIRTUAL_ENTER, OnAnchorSchedule)
	ON_MSG(m_anBuzzer.GetID(), KEY_VIRTUAL_ENTER, OnAnchorBuzzer)
	ON_MSG(m_anExit.GetID(), KEY_VIRTUAL_ENTER, OnExit)
END_MSG_MAP()

unsigned long CCfgAlarmOutManAnchorDlg::OnAnchorAlarmOut()
{
	m_cfgAlarmOutManDlg.SetAnchorPage(CCfgManDlg3<CCfgAlarmOutDlg, CCfgScheduleChildDlg, CCfgAlarmOutBuzzerDlg>::PAGE_1);
	m_cfgAlarmOutManDlg.DoModal(this, CENTER_POS, CENTER_POS, MAX_SCREEN_X, MAX_SCREEN_Y, false);

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CCfgAlarmOutManAnchorDlg::OnAnchorSchedule()
{
	m_cfgAlarmOutManDlg.SetAnchorPage(CCfgManDlg3<CCfgAlarmOutDlg, CCfgScheduleChildDlg, CCfgAlarmOutBuzzerDlg>::PAGE_2);
	m_cfgAlarmOutManDlg.DoModal(this, CENTER_POS, CENTER_POS, MAX_SCREEN_X, MAX_SCREEN_Y, false);

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CCfgAlarmOutManAnchorDlg::OnAnchorBuzzer()
{
	m_cfgAlarmOutManDlg.SetAnchorPage(CCfgManDlg3<CCfgAlarmOutDlg, CCfgScheduleChildDlg, CCfgAlarmOutBuzzerDlg>::PAGE_3);
	m_cfgAlarmOutManDlg.DoModal(this, CENTER_POS, CENTER_POS, MAX_SCREEN_X, MAX_SCREEN_Y, false);

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CCfgAlarmOutManAnchorDlg::OnExit()
{
	EndDialog(DLG_ESC);
	return KEY_VIRTUAL_MSG_STOP;
}

//end
