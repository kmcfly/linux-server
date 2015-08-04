/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : ZXX
** Date         : 2012-09-22
** Name         : WzdUS02DSTDateDlg.cpp
** Version      : 1.0
** Description  : 向导页面对话框
** Modify Record:
***********************************************************************/

#include "WzdUS02DSTDateDlg.h"

CWzdUS02DSTDateDlg::CWzdUS02DSTDateDlg()
{

};

CWzdUS02DSTDateDlg::~CWzdUS02DSTDateDlg()
{

};

void CWzdUS02DSTDateDlg::RefreshView(bool bRefresh)
{
	GUI::CListCtrl& list = m_listCtrl;

	dynamic_cast<GUI::CDateCtrl*>(list.GetItemWnd(ROW_START_DATE,1))->SetDate(m_dst->InYear,m_dst->InMonth,m_dst->InMday,false);
	dynamic_cast<GUI::CTimeCtrl*>(list.GetItemWnd(ROW_START_TIME,1))->SetTime(m_dst->InSecond,false);

	dynamic_cast<GUI::CDateCtrl*>(list.GetItemWnd(ROW_END_DATE,1))->SetDate(m_dst->OutYear,m_dst->OutMonth,m_dst->OutMday,false);
	dynamic_cast<GUI::CTimeCtrl*>(list.GetItemWnd(ROW_END_TIME,1))->SetTime(m_dst->OutSecond,false);

	if( bRefresh )
	{
		Repaint();
	}
}

void CWzdUS02DSTDateDlg::OnInitial()
{
	m_style |= WND_NO_TITLE;
	GUI::CDialog::OnInitial();
	//////////////////////////////////////////////////////////////////////////

	unsigned long x = 0,y = 0;
	unsigned long width = m_rect.Width(), height = m_rect.Height();
	GUI::CListCtrl& list = m_listCtrl;
	char str[128] = {0};

	list.Create(GetRandWndID(),x,y,width,height,this,0,0,false);
	list.AddColumn(NULL,width/2);
	list.AddColumn(NULL,width/2);

	list.AddItem(ROW_START_DATE,0,m_pStrTable->String(E_STR_ID_FROM));
	GUI::CDateCtrl* pDate = dynamic_cast<GUI::CDateCtrl*>(list.AddItem(ROW_START_DATE,1,WND_TYPE_DATE,m_length[DATE_CTRL_WIDTH],m_length[DATE_CTRL_HEIGHT]));
	assert(pDate);
	pDate->AddToNotifyList();

	GUI::CTimeCtrl* pTime = dynamic_cast<GUI::CTimeCtrl*>(list.AddItem(ROW_START_TIME,1,WND_TYPE_TIME,m_length[DATE_CTRL_WIDTH]+4,m_length[TIME_CTRL_HEIGHT]));
	assert(pTime);
	pTime->EnableAddDec(false);
	pTime->AddToNotifyList();
	
	list.AddItem(ROW_END_DATE,0,m_pStrTable->String(E_STR_ID_UNTIL));
	pDate = dynamic_cast<GUI::CDateCtrl*>(list.AddItem(ROW_END_DATE,1,WND_TYPE_DATE,m_length[DATE_CTRL_WIDTH],m_length[DATE_CTRL_HEIGHT]));
	assert(pDate);
	pDate->AddToNotifyList();
	
	pTime = dynamic_cast<GUI::CTimeCtrl*>(list.AddItem(ROW_END_TIME,1,WND_TYPE_TIME,m_length[DATE_CTRL_WIDTH]+4,m_length[TIME_CTRL_HEIGHT]));
	assert(pTime);
	pTime->EnableAddDec(false);
	pTime->AddToNotifyList();
	list.CreateListFocusMap();
}

void CWzdUS02DSTDateDlg::OnPaint(PAINT_STATUS status)
{

}

BEGIN_MSG_MAP(CWzdUS02DSTDateDlg, GUI::CDialog)
	ON_MSG_EX(m_listCtrl.GetID(), KEY_VIRTUAL_MSG_DEFAULT_TAG, OnClickList)
END_MSG_MAP()
unsigned long CWzdUS02DSTDateDlg::OnClickList(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	unsigned short row = (keyID >> 16)&0x7fff;	//行
	unsigned short col = keyID&0xffff;			//列

	GUI::CListCtrl& list = m_listCtrl;
	GUI::CDateCtrl* pDate = NULL;
	GUI::CTimeCtrl* pTime = NULL;

	if( 1 == col )
	{
		switch ( row )
		{
		case ROW_START_DATE:
			{
				pDate = dynamic_cast<GUI::CDateCtrl*>(list.GetItemWnd(ROW_START_DATE,1));
				assert(pDate);

				m_dst->InYear = pDate->GetYear();
				m_dst->InMonth = pDate->GetMonth();
				m_dst->InMday = pDate->GetDay();
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
		case ROW_END_DATE:
			{
				pDate = dynamic_cast<GUI::CDateCtrl*>(list.GetItemWnd(ROW_END_DATE,1));
				assert(pDate);

				m_dst->OutYear = pDate->GetYear();
				m_dst->OutMonth = pDate->GetMonth();
				m_dst->OutMday = pDate->GetDay();
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
