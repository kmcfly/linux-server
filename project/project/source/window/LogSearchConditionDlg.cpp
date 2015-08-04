/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : ysw
** Date         : 2009-08-08
** Name         : LogSearchConditionDlg.cpp
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/
#include "LogSearchConditionDlg.h"

//////////////////////////////////////////////////////////////////////////
CLogSearchConditionDlg::CLogSearchConditionDlg()
{

}

CLogSearchConditionDlg::~CLogSearchConditionDlg()
{

}

//////////////////////////////////////////////////////////////////////////
void CLogSearchConditionDlg::OnInitial()
{
	GUI::CDialog::OnInitial();

	//////////////////////////////////////////////////////////////////////////

	unsigned short xLeft = 8, yTop = 8;
	unsigned short x = xLeft+4, y = yTop+4;
	unsigned short xOff = 4, yOff = 4;
	unsigned short width = m_rect.Width() - 2 * xLeft;
	unsigned short height = m_rect.Height() - 2 * yTop-4*m_length[LIST_ITEM_HEIGHT];

	m_scStart.Create( GUI::CWnd::GetRandWndID(), x, y, 88, m_length[LIST_ITEM_HEIGHT], this, m_pStrTable->String(E_STR_ID_START_TIME) );
	m_dateStart.Create(GetRandWndID(),x+90,y,m_length[DATE_CTRL_WIDTH],m_length[DATE_CTRL_HEIGHT],this,0,1);
	m_timeStart.Create(GetRandWndID(),x+90,y+m_length[LIST_ITEM_HEIGHT],m_length[DATE_CTRL_WIDTH],m_length[TIME_CTRL_HEIGHT],this,0,2);

	y += 2*m_length[LIST_ITEM_HEIGHT];

	m_scEnd.Create( GUI::CWnd::GetRandWndID(), x, y, 88, m_length[LIST_ITEM_HEIGHT], this, m_pStrTable->String(E_STR_ID_END_TIME) );
	m_dateEnd.Create(GetRandWndID(),x+90,y,m_length[DATE_CTRL_WIDTH],m_length[DATE_CTRL_HEIGHT],this,0,3);
	m_timeEnd.Create(GetRandWndID(),x+90,y+m_length[LIST_ITEM_HEIGHT],m_length[DATE_CTRL_WIDTH],m_length[TIME_CTRL_HEIGHT],this,0,4);

	y += 2*m_length[LIST_ITEM_HEIGHT];
	x = xLeft;
	GUI::CListCtrl& list = m_listCtrl;

	list.Create(GetRandWndID(),x,y,width,height,this,0,5,false);
	list.AddColumn("", 32);
	list.AddColumn("", width -32);

	//std::string str[] = {"System Control","System Setup","Playback","Backup",
	//							"Search","View Information","Error"};

	for (int i=0;i<7;i++)
	{
		GUI::CCheckCtrl* pCheck = dynamic_cast<GUI::CCheckCtrl*>(list.AddItem(i,0,WND_TYPE_CHECK));
		assert(pCheck);
		pCheck->SetCheck(true,false);

		list.AddItem(i,1,m_pStrTable->String(static_cast<ENUM_STRING_ID>(E_STR_ID_LOG_SYSTEM_CONTROL+i)));
	}
	list.CreateListFocusMap();
}

unsigned long CLogSearchConditionDlg::GetCondition()
{
	GUI::CListCtrl& list = m_listCtrl;
	//GUI::CCheckCtrl* pCheck = NULL;
	unsigned long condition = 0;

	//for(int i=0;i<7;i++)
	//{
	//	pCheck = dynamic_cast<GUI::CCheckCtrl*>(list.GetItemWnd(i,0));
	//	assert(pCheck);
	//	if( pCheck->IsCheck() )
	//	{
	//		condition |= ( static_cast<unsigned long>(LOG_TYPE_SYSTEM_CTRL) << i );
	//	}
	//}

	if( dynamic_cast<GUI::CCheckCtrl*>(list.GetItemWnd(0,0))->IsCheck() )
	{
		condition |= LOG_TYPE_SYSTEM_CTRL;
	}

	if( dynamic_cast<GUI::CCheckCtrl*>(list.GetItemWnd(1,0))->IsCheck() )
	{
		condition |= LOG_TYPE_CONFIG;
	}

	if( dynamic_cast<GUI::CCheckCtrl*>(list.GetItemWnd(2,0))->IsCheck() )
	{
		condition |= LOG_TYPE_PLAYBACK;
	}

	if( dynamic_cast<GUI::CCheckCtrl*>(list.GetItemWnd(3,0))->IsCheck() )
	{
		condition |= LOG_TYPE_BACKUP;
	}

	if( dynamic_cast<GUI::CCheckCtrl*>(list.GetItemWnd(4,0))->IsCheck() )
	{
		condition |= LOG_TYPE_SEARCH;
	}

	if( dynamic_cast<GUI::CCheckCtrl*>(list.GetItemWnd(5,0))->IsCheck() )
	{
		condition |= LOG_TYPE_VIEW_INFO;
	}

	if( dynamic_cast<GUI::CCheckCtrl*>(list.GetItemWnd(6,0))->IsCheck() )
	{
		condition |= LOG_TYPE_ERROR_INFO;
	}

	return (condition >> 16);
}

unsigned long CLogSearchConditionDlg::TimeStart()
{
	tm tmpTm;
	memset(&tmpTm,0,sizeof(tmpTm));
	m_dateStart.GetDate(tmpTm.tm_year,tmpTm.tm_mon,tmpTm.tm_mday);
	m_timeStart.GetTime(tmpTm.tm_hour,tmpTm.tm_min,tmpTm.tm_sec);
	tmpTm.tm_year-=1900;
	return TmToDVRTime32(tmpTm);
}

unsigned long CLogSearchConditionDlg::TimeEnd()
{
	tm tmpTm;
	memset(&tmpTm,0,sizeof(tmpTm));
	m_dateEnd.GetDate(tmpTm.tm_year,tmpTm.tm_mon,tmpTm.tm_mday);
	m_timeEnd.GetTime(tmpTm.tm_hour,tmpTm.tm_min,tmpTm.tm_sec);
	tmpTm.tm_year-=1900;
	return TmToDVRTime32(tmpTm);
}

void CLogSearchConditionDlg::SetStartTime(tm time)
{
	m_dateStart.SetDate(time.tm_year+1900,time.tm_mon,time.tm_mday,false);
	m_timeStart.SetTime(time.tm_hour,time.tm_min,time.tm_sec,false);
}

void CLogSearchConditionDlg::SetEndTime(tm time)
{
	m_dateEnd.SetDate(time.tm_year+1900,time.tm_mon,time.tm_mday,false);
	m_timeEnd.SetTime(time.tm_hour,time.tm_min,time.tm_sec,false);
}

void CLogSearchConditionDlg::OnPaint(PAINT_STATUS status)
{
	GUI::CDialog::OnPaint(status);

	unsigned short xLeft = 8, yTop = 8;
	unsigned short x = m_rect.m_left+xLeft, y = m_rect.m_top+yTop;
	unsigned short width = m_rect.Width() - 2 * xLeft-2;

	unsigned short height = 4*m_length[LIST_ITEM_HEIGHT]+4;

	GUI::CWnd::DrawLine(x,y,x+width,y,COLOR_LINE_GRAY,2);
	GUI::CWnd::DrawLine(x,y,x,y+height,COLOR_LINE_GRAY,2);
	GUI::CWnd::DrawLine(x+width,y,x+width,y+height,COLOR_LINE_GRAY,2);
}
