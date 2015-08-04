/***********************************************************************
** Copyright (C)  Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : L
** Date         : 2010-04-09
** Name         : CfgAlarmBuzzerDlg.h 
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/

#include "CfgAlarmBuzzerDlg.h"
#include "FuncCustom.h"


unsigned long CCfgAlarmBuzzerDlg::s_holdTime[] = {5, 10, 20, 30, 60, 120, -1};

CCfgAlarmBuzzerDlg::CCfgAlarmBuzzerDlg()
{

}

CCfgAlarmBuzzerDlg::~CCfgAlarmBuzzerDlg()
{

}

void CCfgAlarmBuzzerDlg::OnInitial()
{
	CCfgDlg::OnInitial();
	unsigned short x = 0;
	unsigned short y = 0;
	unsigned short width = 0;
	unsigned short height = 0;

	if (!IsSupportAlarmOut())
	{
		SetImage(BMP_MEM_DLG_LARGE, false);
		SetCaption(m_pStrTable->String(E_STR_ID_BUZZER), false);
		SetDefaultBtn( DEFAULT_BTN_APPLY | DEFAULT_BTN_OK | DEFAULT_BTN_CANCEL );
		SetDefaultBtnCaption( DEFAULT_BTN_CANCEL, m_pStrTable->String(E_STR_ID_EXIT));
		SetDefaultBtnCaption( DEFAULT_BTN_OK,m_pStrTable->String(E_STR_ID_APPLY) );
		SetDefaultBtnCaption( DEFAULT_BTN_APPLY,m_pStrTable->String(E_STR_ID_DEFAULT) );
		//////////////////////////////////////////////////////////////////////////
		x = m_length[DLG_OFFSET_LEFT];
		y = m_length[TITLE_HEIGHT] +  2 * m_length[DLG_OFFSET_TOP];

		width = m_rect.Width() - 2 * m_length[DLG_OFFSET_LEFT];
		height = m_rect.Height() - y - m_length[DEFAULT_BTN_BOTTOM_YPOS] - m_length[DLG_OFFSET_BOTTOM];
	}
	else
	{
		x = m_length[DLG_OFFSET_SUB_LEFT];
		y = m_length[DLG_OFFSET_SUB_TOP];
		width = m_rect.Width() - 2 * m_length[DLG_OFFSET_SUB_LEFT];
		height = m_rect.Height() - y - m_length[DLG_OFFSET_SUB_BOTTOM];
	}
	//////////////////////////////////////////////////////////////////////////
	GUI::CListCtrl& list = m_listCtrl;

	int hold_count = sizeof(s_holdTime)/sizeof(s_holdTime[0]);
	char str[32] = {0};

	list.Create(GUI::CWnd::GetRandWndID(), x, y, width, height, this, 0, 0, false);
	list.AddColumn(NULL, width / 2);
	list.AddColumn(NULL, width / 2 - 4 );

	////
	list.AddItem(ROW_BUZZER_ENABLE, 0, m_pStrTable->String(E_STR_ID_BUZZER));
	GUI::CCheckCtrl* pCheck = dynamic_cast<GUI::CCheckCtrl*>(list.AddItem(ROW_BUZZER_ENABLE, 1, WND_TYPE_CHECK));
	assert( pCheck );

	list.AddItem(ROW_BUZZER_HOLD_TIME, 0, m_pStrTable->String(E_STR_ID_BUZZER_HOLD_TIME));
	GUI::CComboCtrl* pCombo = dynamic_cast<GUI::CComboCtrl*>(list.AddItem(ROW_BUZZER_HOLD_TIME, 1, WND_TYPE_COMBO));
	assert(pCombo);
	
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

	list.CreateListFocusMap();

}

bool CCfgAlarmBuzzerDlg::ShowData(NCFG_ITEM_ID cfgID, const BYTE* pData, int len, bool bRefresh)
{
	assert(NULL != pData);
	assert(len > 0);

	GUI::CListCtrl& list = m_listCtrl;
	BYTE* pTempData = NULL;
	unsigned long tempDataLen = 0;

	switch (cfgID)
	{
	case NCFG_ITEM_BUZZER_ENABLE:
		{
			printf("%s:%s:%d, Show data buzzer enable\n", __FUNCTION__, __FILE__, __LINE__);
			assert(len == sizeof(unsigned long));
			const unsigned long* pValue = reinterpret_cast<const unsigned long*>(pData);
			GUI::CCheckCtrl* pCheckBuzzer = dynamic_cast<GUI::CCheckCtrl*>(list.GetItemWnd(ROW_BUZZER_ENABLE, 1));
			assert(NULL != pCheckBuzzer);
			pCheckBuzzer->SetCheck(*pValue, bRefresh);
		}
		return true;
		break;
	case NCFG_ITEM_BUZZER_HOLD_TIME:
		{

			printf("%s:%s:%d, Show data buzzer HOLD time\n", __FUNCTION__, __FILE__, __LINE__);
			assert(len == sizeof(unsigned long));
			const unsigned long* pValue = reinterpret_cast<const unsigned long*>(pData);
			GUI::CComboCtrl* pCombo = dynamic_cast<GUI::CComboCtrl*>(list.GetItemWnd(ROW_BUZZER_HOLD_TIME, 1));
			assert(NULL != pCombo);
			pCombo->SetCurItemData( *pValue, bRefresh );
		}
		return true;
		break;
	default:
		return false;
		break;
	}

	return false;
}

bool CCfgAlarmBuzzerDlg::SetData(NCFG_ITEM_ID cfgID, BYTE*& pData, int& len) const
{
	assert(NULL != pData);
	assert(len > 0);

	const GUI::CListCtrl& list = m_listCtrl;

	switch (cfgID)
	{
	case NCFG_ITEM_BUZZER_ENABLE:
		{
			assert(len == sizeof(unsigned long));
			unsigned long* pValue = reinterpret_cast<unsigned long*>(pData);
			const GUI::CCheckCtrl* pCheckBuzzer = dynamic_cast<const GUI::CCheckCtrl*>(list.GetItemWnd(ROW_BUZZER_ENABLE, 1));
			*pValue = pCheckBuzzer->IsCheck() ? 1 : 0;
		}
		return true;
		break;
	case NCFG_ITEM_BUZZER_HOLD_TIME:
		{
			assert(len == sizeof(unsigned long));
			unsigned long* pValue = reinterpret_cast<unsigned long*>(pData);
			const GUI::CComboCtrl* pCombo = dynamic_cast<const GUI::CComboCtrl*>(list.GetItemWnd(ROW_BUZZER_HOLD_TIME, 1));
			assert( NULL != pCombo );
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

unsigned long CCfgAlarmBuzzerDlg::OnApply()
{
	//Load default
	CCfgDlg* pCfgDlg = this;
	assert(pCfgDlg);
	unsigned long ret = OnLoadDefault(this);

	if( pCfgDlg->IsModify() )
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

BEGIN_MSG_MAP(CCfgAlarmBuzzerDlg, CCfgDlg)
	ON_MSG_EX(m_listCtrl.GetID(), KEY_VIRTUAL_MSG_DEFAULT_TAG, OnClickList)
END_MSG_MAP()

void CCfgAlarmBuzzerDlg::GetItemList(CFGITEM_LIST& itemList) const
{
	if (!IsSupportAlarmOut())
	{
		printf("%s:%s:%d, add %d, and %d\n", __FUNCTION__, __FILE__, __LINE__, NCFG_ITEM_BUZZER_ENABLE, NCFG_ITEM_BUZZER_HOLD_TIME);
		itemList.push_back(NCFG_ITEM_BUZZER_ENABLE);
		itemList.push_back(NCFG_ITEM_BUZZER_HOLD_TIME);
	}
	else
	{
		printf("%s:%s:%d, Not support alarm out\n", __FUNCTION__, __FILE__, __LINE__);
	}
}
unsigned long CCfgAlarmBuzzerDlg::OnClickList(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos )
{
	unsigned short row = (keyID >> 16)&0x7fff;	//行
	unsigned short col = keyID&0xffff;			//列

	if (!IsModify() && 1 == col)
	{
		switch (row)
		{
		case ROW_BUZZER_ENABLE:
			if (IsChange(NCFG_ITEM_BUZZER_ENABLE))
			{
				SetModify(true);
				EnableOkBtn(true, true);
				return KEY_VIRTUAL_MSG_CONTINUE;
			}
			break;
		case ROW_BUZZER_HOLD_TIME:
			if (IsChange(NCFG_ITEM_BUZZER_HOLD_TIME))
			{
				SetModify(true);
				EnableOkBtn(true, true);
				return KEY_VIRTUAL_MSG_CONTINUE;
			}
			break;
		default:
			assert(false);
			break;
		}
	} 

	return KEY_VIRTUAL_MSG_STOP;
}

//end
