/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : ZXX
** Date         : 2013-11-26
** Name         : WzdNatDSTWeekDlg.cpp
** Version      : 1.0
** Description  : 向导页面对话框
** Modify Record:
***********************************************************************/
#include "WzdNatDSTWeekDlg.h"

CWzdNatDSTWeekDlg::CWzdNatDSTWeekDlg()
{

}

CWzdNatDSTWeekDlg::~CWzdNatDSTWeekDlg()
{

}

void CWzdNatDSTWeekDlg::RefreshView(bool bRefresh)
{
	GUI::CListCtrl& list = m_listCtrl;

	dynamic_cast<GUI::CComboCtrl*>(list.GetItemWnd(ROW_START_MONTH,1))->SetCurItemData(m_dst->InMonth,false);
	dynamic_cast<GUI::CComboCtrl*>(list.GetItemWnd(ROW_START_WEEKOFMON,1))->SetCurItemData(m_dst->InWeekIndex,false);
	dynamic_cast<GUI::CComboCtrl*>(list.GetItemWnd(ROW_START_WEEK,1))->SetCurItemData(m_dst->InWday,false);
	dynamic_cast<GUI::CTimeCtrl*>(list.GetItemWnd(ROW_START_TIME,1))->SetTime(m_dst->InSecond,false);

	dynamic_cast<GUI::CComboCtrl*>(list.GetItemWnd(ROW_END_MONTH,1))->SetCurItemData(m_dst->OutMonth,false);
	dynamic_cast<GUI::CComboCtrl*>(list.GetItemWnd(ROW_END_WEEKOFMON,1))->SetCurItemData(m_dst->OutWeekIndex,false);
	dynamic_cast<GUI::CComboCtrl*>(list.GetItemWnd(ROW_END_WEEK,1))->SetCurItemData(m_dst->OutWday,false);
	dynamic_cast<GUI::CTimeCtrl*>(list.GetItemWnd(ROW_END_TIME,1))->SetTime(m_dst->OutSecond,false);

	if( bRefresh )
	{
		Repaint();
	}
}

void CWzdNatDSTWeekDlg::OnInitial()
{
	assert(m_dst);			//必须先绑定数据区域,CCfgDaylightDlg::SetDstData()

	m_style |= WND_NO_TITLE;
	GUI::CDialog::OnInitial();
	//////////////////////////////////////////////////////////////////////////
	unsigned long x = 0,y = 0;
	unsigned long width = m_rect.Width(), height = m_rect.Height();
	GUI::CListCtrl& list = m_listCtrl;
	char str[128] = {0};

	unsigned short offsetEdge = m_length[SCROLL_WIDTH] + 2;
	if( GUI::CWnd::m_width >= 1024 && GUI::CWnd::m_height >= 768 )
	{
		offsetEdge = 4;
	}

	list.Create(GetRandWndID(),x,y,width,height,this,0,0,false);
	list.AddColumn(NULL,width/2);
	list.AddColumn(NULL,width/2-offsetEdge);

	list.AddItem(ROW_START_MONTH,0,m_pStrTable->String(E_STR_ID_FROM));
	GUI::CComboCtrl* pCombo = dynamic_cast<GUI::CComboCtrl*>(list.AddItem(ROW_START_MONTH,1,WND_TYPE_COMBO));
	assert(pCombo);
	for (int i=0;i<12;i++)
	{
		pCombo->AddItem(m_pStrTable->String(static_cast<ENUM_STRING_ID>(E_STR_ID_JANUARY+i)),i);
	}
	pCombo->SetCurItemData(0,false);

	pCombo = dynamic_cast<GUI::CComboCtrl*>(list.AddItem(ROW_START_WEEKOFMON,1,WND_TYPE_COMBO));
	assert(pCombo);
	for (int i=0;i<6;i++)
	{
		pCombo->AddItem(m_pStrTable->String(static_cast<ENUM_STRING_ID>(E_STR_ID_DST_WEEK_1ST+i)),i);
	}
	pCombo->SetCurItemData(0,false);

	pCombo = dynamic_cast<GUI::CComboCtrl*>(list.AddItem(ROW_START_WEEK,1,WND_TYPE_COMBO));
	assert(pCombo);
	for (int i=0;i<7;i++)
	{
		pCombo->AddItem(m_pStrTable->String(static_cast<ENUM_STRING_ID>(E_STR_ID_SUNDAY+i)),i);
	}
	pCombo->SetDropDownCount(7);
	pCombo->SetCurItemData(0,false);

	GUI::CTimeCtrl* pTime = dynamic_cast<GUI::CTimeCtrl*>(list.AddItem(ROW_START_TIME,1,WND_TYPE_TIME,m_length[TIME_CTRL_WIDTH],m_length[TIME_CTRL_HEIGHT]));
	assert(pTime);
	pTime->EnableAddDec(false);
	pTime->AddToNotifyList();

	list.AddItem(ROW_END_MONTH,0,m_pStrTable->String(E_STR_ID_UNTIL));
	pCombo = dynamic_cast<GUI::CComboCtrl*>(list.AddItem(ROW_END_MONTH,1,WND_TYPE_COMBO));
	assert(pCombo);
	for (int i=0;i<12;i++)
	{
		pCombo->AddItem(m_pStrTable->String(static_cast<ENUM_STRING_ID>(E_STR_ID_JANUARY+i)),i);
	}
	pCombo->SetCurItemData(0,false);

	pCombo = dynamic_cast<GUI::CComboCtrl*>(list.AddItem(ROW_END_WEEKOFMON,1,WND_TYPE_COMBO));
	assert(pCombo);
	for (int i=0;i<6;i++)
	{
		pCombo->AddItem(m_pStrTable->String(static_cast<ENUM_STRING_ID>(E_STR_ID_DST_WEEK_1ST+i)),i);
	}
	pCombo->SetCurItemData(0,false);

	pCombo = dynamic_cast<GUI::CComboCtrl*>(list.AddItem(ROW_END_WEEK,1,WND_TYPE_COMBO));
	assert(pCombo);
	for (int i=0;i<7;i++)
	{
		pCombo->AddItem(m_pStrTable->String(static_cast<ENUM_STRING_ID>(E_STR_ID_SUNDAY+i)),i);
	}
	pCombo->SetDropDownCount(7);
	pCombo->SetCurItemData(0,false);

	pTime = dynamic_cast<GUI::CTimeCtrl*>(list.AddItem(ROW_END_TIME,1,WND_TYPE_TIME,m_length[TIME_CTRL_WIDTH],m_length[TIME_CTRL_HEIGHT]));
	assert(pTime);
	pTime->EnableAddDec(false);
	pTime->AddToNotifyList();

	list.CreateListFocusMap();
}

void CWzdNatDSTWeekDlg::OnPaint(PAINT_STATUS status)
{

}

BEGIN_MSG_MAP(CWzdNatDSTWeekDlg, GUI::CDialog)
	ON_MSG_EX(m_listCtrl.GetID(), KEY_VIRTUAL_MSG_DEFAULT_TAG, OnClickList)
END_MSG_MAP()

unsigned long CWzdNatDSTWeekDlg::OnClickList(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	assert(NULL != m_dst);

	unsigned short row = (keyID >> 16)&0x7fff;	//行
	unsigned short col = keyID&0xffff;			//列

	GUI::CListCtrl& list = m_listCtrl;
	GUI::CComboCtrl* pCombo = NULL;
	GUI::CTimeCtrl* pTime = NULL;

	if( 1 == col )
	{
		switch (row)
		{
		case ROW_START_MONTH:
			{
				pCombo = dynamic_cast<GUI::CComboCtrl*>(list.GetItemWnd(ROW_START_MONTH,1));
				assert(pCombo);
				m_dst->InMonth = pCombo->GetCurItemData();
			}
			return KEY_VIRTUAL_MSG_CONTINUE;
			break;
		case ROW_START_WEEKOFMON:
			{
				pCombo = dynamic_cast<GUI::CComboCtrl*>(list.GetItemWnd(ROW_START_WEEKOFMON,1));
				assert(pCombo);
				m_dst->InWeekIndex = pCombo->GetCurItemData();
			}
			return KEY_VIRTUAL_MSG_CONTINUE;
			break;
		case ROW_START_WEEK:
			{
				pCombo = dynamic_cast<GUI::CComboCtrl*>(list.GetItemWnd(ROW_START_WEEK,1));
				assert(pCombo);
				m_dst->InWday = pCombo->GetCurItemData();
			}
			return KEY_VIRTUAL_MSG_CONTINUE;
			break;
		case ROW_START_TIME:
			{
				pTime = dynamic_cast<GUI::CTimeCtrl*>(list.GetItemWnd(ROW_START_TIME,1));
				assert(pTime);
				m_dst->InSecond = pTime->GetTime();
			}
			return KEY_VIRTUAL_MSG_CONTINUE;
			break;
		case ROW_END_MONTH:
			{
				pCombo = dynamic_cast<GUI::CComboCtrl*>(list.GetItemWnd(ROW_END_MONTH,1));
				assert(pCombo);
				m_dst->OutMonth = pCombo->GetCurItemData();
			}
			return KEY_VIRTUAL_MSG_CONTINUE;
			break;
		case ROW_END_WEEKOFMON:
			{
				pCombo = dynamic_cast<GUI::CComboCtrl*>(list.GetItemWnd(ROW_END_WEEKOFMON,1));
				assert(pCombo);
				m_dst->OutWeekIndex = pCombo->GetCurItemData();
			}
			return KEY_VIRTUAL_MSG_CONTINUE;
			break;
		case ROW_END_WEEK:
			{
				pCombo = dynamic_cast<GUI::CComboCtrl*>(list.GetItemWnd(ROW_END_WEEK,1));
				assert(pCombo);
				m_dst->OutWday = pCombo->GetCurItemData();
			}
			return KEY_VIRTUAL_MSG_CONTINUE;
			break;
		case ROW_END_TIME:
			{
				pTime = dynamic_cast<GUI::CTimeCtrl*>(list.GetItemWnd(ROW_END_TIME,1));
				assert(pTime);
				m_dst->OutSecond = pTime->GetTime();
			}
			return KEY_VIRTUAL_MSG_CONTINUE;
			break;
		default:
			assert(false);
			break;
		}
	}

	return KEY_VIRTUAL_MSG_STOP;
}

//end
