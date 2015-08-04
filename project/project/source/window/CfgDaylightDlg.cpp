/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author			: L
** Date				: 2009-11-12
** Name			: CfgDaylightDlg.cpp
** Version			: 1.0
** Description	: 
** Modify Record:
1:
***********************************************************************/

#include "CfgDaylightDlg.h"

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////

CDstWeekDlg::CDstWeekDlg()
{
	m_dst = NULL;
}

CDstWeekDlg::~CDstWeekDlg()
{

}

void CDstWeekDlg::OnInitial()
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

	//strcpy(str,"Start Month");
	list.AddItem(ROW_START_MONTH,0,m_pStrTable->String(E_STR_ID_FROM));
	GUI::CComboCtrl* pCombo = dynamic_cast<GUI::CComboCtrl*>(list.AddItem(ROW_START_MONTH,1,WND_TYPE_COMBO));
	assert(pCombo);
	for (int i=0;i<12;i++)
	{
		pCombo->AddItem(m_pStrTable->String(static_cast<ENUM_STRING_ID>(E_STR_ID_JANUARY+i)),i);
	}
	pCombo->SetCurItemData(0,false);

	//strcpy(str,"Start Week Of The Month");
	//list.AddItem(ROW_START_WEEKOFMON,0,"");
	pCombo = dynamic_cast<GUI::CComboCtrl*>(list.AddItem(ROW_START_WEEKOFMON,1,WND_TYPE_COMBO));
	assert(pCombo);
	for (int i=0;i<6;i++)
	{
		pCombo->AddItem(m_pStrTable->String(static_cast<ENUM_STRING_ID>(E_STR_ID_DST_WEEK_1ST+i)),i);
	}
	pCombo->SetCurItemData(0,false);

	//strcpy(str,"Start Week");
	//list.AddItem(ROW_START_WEEK,0,m_pStrTable->String(E_STR_ID_DST_START_WEEK));
	pCombo = dynamic_cast<GUI::CComboCtrl*>(list.AddItem(ROW_START_WEEK,1,WND_TYPE_COMBO));
	assert(pCombo);
	for (int i=0;i<7;i++)
	{
		pCombo->AddItem(m_pStrTable->String(static_cast<ENUM_STRING_ID>(E_STR_ID_SUNDAY+i)),i);
	}
	pCombo->SetDropDownCount(7);
	pCombo->SetCurItemData(0,false);

	//strcpy(str,"Start Time");
	//list.AddItem(ROW_START_TIME,0,m_pStrTable->String(E_STR_ID_TIME));
	GUI::CTimeCtrl* pTime = dynamic_cast<GUI::CTimeCtrl*>(list.AddItem(ROW_START_TIME,1,WND_TYPE_TIME,m_length[TIME_CTRL_WIDTH],m_length[TIME_CTRL_HEIGHT]));
	assert(pTime);
	pTime->EnableAddDec(false);
	pTime->AddToNotifyList();
	//pTime->SetTime(GetCurrTime32(),false);

	////

	//strcpy(str,"End Month");
	list.AddItem(ROW_END_MONTH,0,m_pStrTable->String(E_STR_ID_UNTIL));
	pCombo = dynamic_cast<GUI::CComboCtrl*>(list.AddItem(ROW_END_MONTH,1,WND_TYPE_COMBO));
	assert(pCombo);
	for (int i=0;i<12;i++)
	{
		pCombo->AddItem(m_pStrTable->String(static_cast<ENUM_STRING_ID>(E_STR_ID_JANUARY+i)),i);
	}
	pCombo->SetCurItemData(0,false);

	//strcpy(str,"End Week Of The Month");
	//list.AddItem(ROW_END_WEEKOFMON,0,m_pStrTable->String(E_STR_ID_DST_END_WEEKOFM));
	pCombo = dynamic_cast<GUI::CComboCtrl*>(list.AddItem(ROW_END_WEEKOFMON,1,WND_TYPE_COMBO));
	assert(pCombo);
	for (int i=0;i<6;i++)
	{
		pCombo->AddItem(m_pStrTable->String(static_cast<ENUM_STRING_ID>(E_STR_ID_DST_WEEK_1ST+i)),i);
	}
	pCombo->SetCurItemData(0,false);

	//strcpy(str,"End Week");
	//list.AddItem(ROW_END_WEEK,0,m_pStrTable->String(E_STR_ID_DST_END_WEEK));
	pCombo = dynamic_cast<GUI::CComboCtrl*>(list.AddItem(ROW_END_WEEK,1,WND_TYPE_COMBO));
	assert(pCombo);
	for (int i=0;i<7;i++)
	{
		pCombo->AddItem(m_pStrTable->String(static_cast<ENUM_STRING_ID>(E_STR_ID_SUNDAY+i)),i);
	}
	pCombo->SetDropDownCount(7);
	pCombo->SetCurItemData(0,false);

	//strcpy(str,"END Time");
	//list.AddItem(ROW_END_TIME,0,m_pStrTable->String(E_STR_ID_TIME));
	pTime = dynamic_cast<GUI::CTimeCtrl*>(list.AddItem(ROW_END_TIME,1,WND_TYPE_TIME,m_length[TIME_CTRL_WIDTH],m_length[TIME_CTRL_HEIGHT]));
	assert(pTime);
	pTime->EnableAddDec(false);
	pTime->AddToNotifyList();
	//pTime->SetTime(GetCurrTime32(),false);

	list.CreateListFocusMap();
}

void CDstWeekDlg::OnPaint(PAINT_STATUS status)
{
	
}

//刷新数据到界面
void CDstWeekDlg::RefreshView(bool bRefresh)
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

void CDstWeekDlg::OnModifyWeekData()
{
	CCfgDaylightDlg* pDaylight = dynamic_cast<CCfgDaylightDlg*>(GetParent());
	assert(pDaylight);//

	pDaylight->OnModifyDstData();
}

BEGIN_MSG_MAP(CDstWeekDlg, GUI::CDialog)
	ON_MSG_EX(m_listCtrl.GetID(), KEY_VIRTUAL_MSG_DEFAULT_TAG, OnClickList)
END_MSG_MAP()

unsigned long CDstWeekDlg::OnClickList(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	assert(m_dst);

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
			OnModifyWeekData();
			return KEY_VIRTUAL_MSG_CONTINUE;
			break;
		case ROW_START_WEEKOFMON:
			{
				pCombo = dynamic_cast<GUI::CComboCtrl*>(list.GetItemWnd(ROW_START_WEEKOFMON,1));
				assert(pCombo);
				m_dst->InWeekIndex = pCombo->GetCurItemData();
			}
			OnModifyWeekData();
			return KEY_VIRTUAL_MSG_CONTINUE;
			break;
		case ROW_START_WEEK:
			{
				pCombo = dynamic_cast<GUI::CComboCtrl*>(list.GetItemWnd(ROW_START_WEEK,1));
				assert(pCombo);
				m_dst->InWday = pCombo->GetCurItemData();
			}
			OnModifyWeekData();
			return KEY_VIRTUAL_MSG_CONTINUE;
			break;
		case ROW_START_TIME:
			{
				pTime = dynamic_cast<GUI::CTimeCtrl*>(list.GetItemWnd(ROW_START_TIME,1));
				assert(pTime);
				m_dst->InSecond = pTime->GetTime();
			}
			OnModifyWeekData();
			return KEY_VIRTUAL_MSG_CONTINUE;
			break;
		case ROW_END_MONTH:
			{
				pCombo = dynamic_cast<GUI::CComboCtrl*>(list.GetItemWnd(ROW_END_MONTH,1));
				assert(pCombo);
				m_dst->OutMonth = pCombo->GetCurItemData();
			}
			OnModifyWeekData();
			return KEY_VIRTUAL_MSG_CONTINUE;
			break;
		case ROW_END_WEEKOFMON:
			{
				pCombo = dynamic_cast<GUI::CComboCtrl*>(list.GetItemWnd(ROW_END_WEEKOFMON,1));
				assert(pCombo);
				m_dst->OutWeekIndex = pCombo->GetCurItemData();
			}
			OnModifyWeekData();
			return KEY_VIRTUAL_MSG_CONTINUE;
			break;
		case ROW_END_WEEK:
			{
				pCombo = dynamic_cast<GUI::CComboCtrl*>(list.GetItemWnd(ROW_END_WEEK,1));
				assert(pCombo);
				m_dst->OutWday = pCombo->GetCurItemData();
			}
			OnModifyWeekData();
			return KEY_VIRTUAL_MSG_CONTINUE;
			break;
		case ROW_END_TIME:
			{
				pTime = dynamic_cast<GUI::CTimeCtrl*>(list.GetItemWnd(ROW_END_TIME,1));
				assert(pTime);
				m_dst->OutSecond = pTime->GetTime();
			}
			OnModifyWeekData();
			return KEY_VIRTUAL_MSG_CONTINUE;
			break;
		default:
			assert(false);
			break;
		}
	}

	return KEY_VIRTUAL_MSG_STOP;
}

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////

CDstDateDlg::CDstDateDlg()
{

}

CDstDateDlg::~CDstDateDlg()
{

}

void CDstDateDlg::OnInitial()
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

	//tm tmpTm = DVRTime32ToTm(GetCurrTime32());

	//strcpy(str,"Start Date");
	list.AddItem(ROW_START_DATE,0,m_pStrTable->String(E_STR_ID_FROM));
	GUI::CDateCtrl* pDate = dynamic_cast<GUI::CDateCtrl*>(list.AddItem(ROW_START_DATE,1,WND_TYPE_DATE,m_length[DATE_CTRL_WIDTH],m_length[DATE_CTRL_HEIGHT]));
	assert(pDate);
	pDate->AddToNotifyList();
	//pDate->SetDate(tmpTm.tm_year,tmpTm.tm_mon,tmpTm.tm_mday,false);

	//strcpy(str,"Start Time");
	//list.AddItem(ROW_START_TIME,0,m_pStrTable->String(E_STR_ID_FROM));
	GUI::CTimeCtrl* pTime = dynamic_cast<GUI::CTimeCtrl*>(list.AddItem(ROW_START_TIME,1,WND_TYPE_TIME,m_length[DATE_CTRL_WIDTH]+4,m_length[TIME_CTRL_HEIGHT]));
	assert(pTime);
	pTime->EnableAddDec(false);
	pTime->AddToNotifyList();
	//pTime->SetTime(tmpTm.tm_hour,tmpTm.tm_min,tmpTm.tm_sec,false);

	//////

	//strcpy(str,"End Date");
	list.AddItem(ROW_END_DATE,0,m_pStrTable->String(E_STR_ID_UNTIL));
	pDate = dynamic_cast<GUI::CDateCtrl*>(list.AddItem(ROW_END_DATE,1,WND_TYPE_DATE,m_length[DATE_CTRL_WIDTH],m_length[DATE_CTRL_HEIGHT]));
	assert(pDate);
	pDate->AddToNotifyList();
	//pDate->SetDate(tmpTm.tm_year,tmpTm.tm_mon,tmpTm.tm_mday,false);

	//strcpy(str,"End Time");
	//list.AddItem(ROW_END_TIME,0,m_pStrTable->String(E_STR_ID_TIME));
	pTime = dynamic_cast<GUI::CTimeCtrl*>(list.AddItem(ROW_END_TIME,1,WND_TYPE_TIME,m_length[DATE_CTRL_WIDTH]+4,m_length[TIME_CTRL_HEIGHT]));
	assert(pTime);
	pTime->EnableAddDec(false);
	pTime->AddToNotifyList();
	//pTime->SetTime(tmpTm.tm_hour,tmpTm.tm_min,tmpTm.tm_sec,false);

	list.CreateListFocusMap();
}

void CDstDateDlg::OnPaint(PAINT_STATUS status)
{

}

//刷新数据到界面
void CDstDateDlg::RefreshView(bool bRefresh)
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

void CDstDateDlg::OnModifyDateData()
{
	CCfgDaylightDlg* pDaylight = dynamic_cast<CCfgDaylightDlg*>(GetParent());
	assert(pDaylight);//

	pDaylight->OnModifyDstData();
}

BEGIN_MSG_MAP(CDstDateDlg, GUI::CDialog)
	ON_MSG_EX(m_listCtrl.GetID(), KEY_VIRTUAL_MSG_DEFAULT_TAG, OnClickList)
END_MSG_MAP()

unsigned long CDstDateDlg::OnClickList(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
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
			OnModifyDateData();
			return KEY_VIRTUAL_MSG_CONTINUE;
			break;
		case ROW_START_TIME:
			{
				pTime = dynamic_cast<GUI::CTimeCtrl*>(list.GetItemWnd(ROW_START_TIME,1));
				assert(pTime);

				m_dst->InSecond = pTime->GetTime();
			}
			OnModifyDateData();
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
			OnModifyDateData();
			return KEY_VIRTUAL_MSG_CONTINUE;
			break;
		case ROW_END_TIME:
			{
				pTime = dynamic_cast<GUI::CTimeCtrl*>(list.GetItemWnd(ROW_END_TIME,1));
				assert(pTime);

				m_dst->OutSecond = pTime->GetTime();
			}
			OnModifyDateData();
			return KEY_VIRTUAL_MSG_CONTINUE;
			break;
		default:
			assert(false);
			break;
		}
	}

	return KEY_VIRTUAL_MSG_STOP;
}

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////

CCfgDaylightDlg::CCfgDaylightDlg()
{
	m_curPage = PAGE_NULL;
	memset(&m_dstData,0,sizeof(DAYLIGHT_INFO));
}

CCfgDaylightDlg::~CCfgDaylightDlg()
{

}

void CCfgDaylightDlg::OnInitial()
{
	CCfgDlg::OnInitial();
	//////////////////////////////////////////////////////////////////////////
	unsigned long x = m_length[DLG_OFFSET_SUB_LEFT];
	unsigned long y = m_length[DLG_OFFSET_SUB_TOP];
	unsigned long width = m_rect.Width() - m_length[DLG_OFFSET_SUB_LEFT] * 2;
	unsigned long height = m_length[LIST_ITEM_HEIGHT]*2;
	char str[128] = {0};

	m_listCtrl.Create(GetRandWndID(),x,y,width,height,this,0,0,false);
	m_listCtrl.AddColumn(NULL,width / 2 );
	m_listCtrl.AddColumn(NULL,width / 2-4);
	m_listCtrl.DrawLine(ROW_ENABLE_DST,2);
	
	m_listCtrl.AddItem(ROW_ENABLE_DST,0,m_pStrTable->String(E_STR_ID_DST_FULL));
	GUI::CCheckCtrl* pCheck = dynamic_cast<GUI::CCheckCtrl*>(m_listCtrl.AddItem(ROW_ENABLE_DST,1,WND_TYPE_CHECK));
	assert(pCheck);

	//strcpy(str,"Time Offset");
	m_listCtrl.AddItem(ROW_TIME_OFFSET,0,m_pStrTable->String(E_STR_ID_DST_TIME_OFFSET));
	GUI::CComboCtrl* pCombo = dynamic_cast<GUI::CComboCtrl*>(m_listCtrl.AddItem(ROW_TIME_OFFSET,1,WND_TYPE_COMBO));
	assert(pCombo);
	//pCombo->AddItem("0",( 0 * ( 60 * 60 ) ) );
	pCombo->AddItem("1",( 1 * ( 60 * 60 ) ) );
	pCombo->AddItem("2",( 2 * ( 60 * 60 ) ) );
	pCombo->SetCurItemData(60 * 60,false);

	/*
	m_listCtrl.AddItem(ROW_WEEK_DAY,0,"Starting Method");
	GUI::CRadioCtrl* pRadioWeek = dynamic_cast<GUI::CRadioCtrl*>(m_listCtrl.AddItem(ROW_WEEK_DAY,1,WND_TYPE_RADIO));
	assert(pRadioWeek);
	pRadioWeek->SetData(PAGE_WEEK);	//使用数据0标记
	pRadioWeek->SetCaption(m_pStrTable->String(E_STR_ID_WEEK),false,false);
	m_rdGroup.AddRadio(pRadioWeek);
	pRadioWeek->AddGroup(&m_rdGroup);
	GUI::CRadioCtrl* pRadioDay = dynamic_cast<GUI::CRadioCtrl*>(m_listCtrl.AddItem(ROW_WEEK_DAY,2,WND_TYPE_RADIO));
	assert(pRadioDay);
	pRadioDay->SetData(PAGE_DATE);	//使用数据1标记
	pRadioDay->SetCaption(m_pStrTable->String(E_STR_ID_DATE),false,false);
	m_rdGroup.AddRadio(pRadioDay);
	pRadioDay->AddGroup(&m_rdGroup);
	*/
	m_listCtrl.CreateListFocusMap();

	//check
	m_scInfo.Create(GetRandWndID(),x+4,y+height + (m_length[LIST_ITEM_HEIGHT]-m_length[STATIC_HEIGHT])/2,width/2 - 6,m_length[STATIC_HEIGHT],this);
	m_scInfo.SetCaption(m_pStrTable->String(E_STR_ID_MODE),false);
	
	m_rdWeek.Create(GetRandWndID(),x+width/2+4 ,y+height+(m_length[LIST_ITEM_HEIGHT]-m_length[RADIO_HEIGHT])/2,(width-16)/4,m_length[RADIO_HEIGHT],this,0,1,&m_rdGroup);
	m_rdDate.Create(GetRandWndID(),x+width/2+4 + (width-16)/4,y+height+(m_length[LIST_ITEM_HEIGHT]-m_length[RADIO_HEIGHT])/2,(width-16)/4 - 2,m_length[RADIO_HEIGHT],this,1,1,&m_rdGroup);

	m_rdGroup.AddRadio(&m_rdWeek);
	m_rdGroup.AddRadio(&m_rdDate);

	m_rdWeek.SetCaption(m_pStrTable->String(E_STR_ID_WEEK),false,false);
	m_rdWeek.SetData(PAGE_WEEK);
	m_rdDate.SetCaption(m_pStrTable->String(E_STR_ID_DATE),false,false);
	m_rdDate.SetData(PAGE_DATE);

	////
	y += height+m_length[LIST_ITEM_HEIGHT];
	height = m_rect.Height() - y - m_length[DLG_OFFSET_SUB_BOTTOM];
	m_dstWeekDlg.SetDstData(&m_dstData);
	m_dstDateDlg.SetDstData(&m_dstData);
	m_dstWeekDlg.Create(GetRandWndID(),x,y,width,height,this,0,2,false);
	m_dstDateDlg.Create(GetRandWndID(),x,y,width,height,this,1,2,false);

	////
	m_curPage = PAGE_NULL;
	//pRadioWeek->SetCheck(true,false);
	m_rdWeek.SetCheck(true,false);
	CheckUI(false);
}

void CCfgDaylightDlg::OnDestroy()
{
	CCfgDlg::OnDestroy();
}

void CCfgDaylightDlg::OnPaint(PAINT_STATUS status)
{
	CCfgDlg::OnPaint(status);

	////
	unsigned long x = m_rect.m_left + m_length[DLG_OFFSET_SUB_LEFT];
	unsigned long y = m_rect.m_top + m_length[DLG_OFFSET_SUB_TOP] + m_length[LIST_ITEM_HEIGHT]*2;
	unsigned long width = m_rect.Width() - m_length[DLG_OFFSET_SUB_LEFT] * 2;
	unsigned long height = m_length[LIST_ITEM_HEIGHT];

	//左竖
	GUI::CWnd::DrawLine(x,y,x,y+height,COLOR_LINE_GRAY,2);

	//中竖
	GUI::CWnd::DrawLine(x+width/2,y,x+width/2,y+height,COLOR_LINE_GRAY,1);

	//右竖
	GUI::CWnd::DrawLine(x+width-2,y,x+width-2,y+height,COLOR_LINE_GRAY,2);

}

void CCfgDaylightDlg::OnModifyDstData()
{
	if( (!IsModify())&&IsChange(NCFG_ITEM_DAYLIGHT_INFO) )
	{
		SetModify(true);
	}
}

void CCfgDaylightDlg::SetPage(PAGE_ITEM item /*= PAGE_WEEK*/,bool bRefresh /*= true*/ )
{
	if( m_curPage != item )
	{
		m_curPage = item;

		m_dstWeekDlg.Show(false,false);
		m_dstDateDlg.Show(false,false);

		switch ( m_curPage )
		{
		case PAGE_WEEK:
			{
				m_dstWeekDlg.Show(true,false);
			}
			break;
		case PAGE_DATE:
			{
				m_dstDateDlg.Show(true,false);
			}
			break;
		default:
			assert(false);
			break;
		}

		if( bRefresh )
		{
			Repaint();
		}
	}
}

unsigned long CCfgDaylightDlg::CheckValue()
{
	//在日期模式时结束时间必须大于开始时间offset
	if(m_dstData.enable && (DAYLIGHT_TYPE_DAY==m_dstData.type))
	{
		//开始年份不能大于结束年份
		if(m_dstData.InYear>m_dstData.OutYear)
		{
			return E_STR_ID_MESSAGE_47;
		}

		//同年时开始月份不能大于结束月份
		if(m_dstData.InYear==m_dstData.OutYear)
		{
			if(m_dstData.InMonth>m_dstData.OutMonth)
			{
				return E_STR_ID_MESSAGE_47;
			}
			
			if(m_dstData.InMonth==m_dstData.OutMonth)
			{
				if(m_dstData.InMday>m_dstData.OutMday)
				{
					return E_STR_ID_MESSAGE_47;
				}
				
				if(m_dstData.InMday==m_dstData.OutMday)
				{
					if((m_dstData.InSecond+m_dstData.offSet) > m_dstData.OutSecond)
					{
						return E_STR_ID_MESSAGE_47;
					}
				}
			}

		}
		
	}

	if(m_dstData.enable && (DAYLIGHT_TYPE_WEEK==m_dstData.type))
	{
		if((m_dstData.InMonth==m_dstData.OutMonth) && (m_dstData.InWeekIndex==m_dstData.OutWeekIndex) && (m_dstData.InWday==m_dstData.OutWday ))
		{
			if((m_dstData.InSecond+m_dstData.offSet) > m_dstData.OutSecond)
			{
				return E_STR_ID_MESSAGE_47;
			}
		}
	}

	return 0;
}

void CCfgDaylightDlg::CheckUI(bool bRefresh)
{
	GUI::CRadioCtrl* pRadio = m_rdGroup.GetCheckRadio();
	if( pRadio )
	{
		switch ( static_cast<PAGE_ITEM>(pRadio->GetData()) )
		{
		case PAGE_WEEK:
			SetPage(PAGE_WEEK,bRefresh);
			break;
		case PAGE_DATE:
			SetPage(PAGE_DATE,bRefresh);
			break;
		default:
			assert(false);
		}
	}
	else
	{
		assert(false);
	}
}

unsigned long CCfgDaylightDlg::OnOK()
{
	return CCfgDlg::OnOK();
}

bool CCfgDaylightDlg::ShowData(NCFG_ITEM_ID cfgID, const BYTE* pData, int len, bool bRefresh)
{
	assert(NULL != pData);
	assert(len > 0);
	GUI::CListCtrl& list = m_listCtrl;

	switch (cfgID)
	{
	case NCFG_ITEM_DAYLIGHT_INFO:
		{
			assert(len==sizeof(DAYLIGHT_INFO));
			const DAYLIGHT_INFO* pInfo = reinterpret_cast<const DAYLIGHT_INFO*>(pData);

			memcpy(&m_dstData,pInfo,sizeof(DAYLIGHT_INFO));

			//刷新界面
			RefreshView(false);
		}
		return true;
		break;
	default:
		return false;
		break;
	}

	return false;
}

bool CCfgDaylightDlg::SetData(NCFG_ITEM_ID cfgID, BYTE*& pData, int& len)  const
{
	assert(NULL != pData);
	assert(len > 0);

	switch (cfgID)
	{
	case NCFG_ITEM_DAYLIGHT_INFO:
		{
			assert(len==sizeof(DAYLIGHT_INFO));
			DAYLIGHT_INFO* pInfo = reinterpret_cast<DAYLIGHT_INFO*>(pData);

			memcpy(pInfo,&m_dstData,sizeof(DAYLIGHT_INFO));
		}
		return true;
		break;
	default:
		return false;
		break;
	}

	return false;
}

//刷新数据到界面
void CCfgDaylightDlg::RefreshView(bool bRefresh)
{
	GUI::CListCtrl& list=m_listCtrl;
	DAYLIGHT_INFO& dst=m_dstData;

	GUI::CCheckCtrl* pCkEnable = dynamic_cast<GUI::CCheckCtrl*>(list.GetItemWnd(ROW_ENABLE_DST,1));
	assert(pCkEnable);
	pCkEnable->SetCheck(dst.enable,false);

	GUI::CComboCtrl* pCbOffset = dynamic_cast<GUI::CComboCtrl*>(list.GetItemWnd(ROW_TIME_OFFSET,1));
	assert(pCbOffset);
	pCbOffset->SetCurItemData(dst.offSet,false);

	if( DAYLIGHT_TYPE_WEEK == dst.type )
	{
		m_rdWeek.SetCheck(true,bRefresh);
	}
	else
	{
		m_rdDate.SetCheck(true,bRefresh);
	}

	//
	m_dstWeekDlg.RefreshView(false);
	m_dstDateDlg.RefreshView(false);

	CheckUI(false);

	if( bRefresh )
	{
		Repaint();
	}
}

BEGIN_MSG_MAP(CCfgDaylightDlg, CCfgDlg)
	ON_MSG_EX(m_listCtrl.GetID(), KEY_VIRTUAL_MSG_DEFAULT_TAG, OnClickList)
	ON_MSG(m_rdWeek.GetID(),KEY_VIRTUAL_ENTER,OnClickRadio)
	ON_MSG(m_rdDate.GetID(),KEY_VIRTUAL_ENTER,OnClickRadio)
END_MSG_MAP()

unsigned long CCfgDaylightDlg::OnClickList(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	unsigned short row = (keyID >> 16)&0x7fff;	//行
	unsigned short col = keyID&0xffff;			//列

	GUI::CListCtrl& list = m_listCtrl;

	if( ROW_ENABLE_DST == row && 1 == col )
	{
		GUI::CCheckCtrl* pCheck = dynamic_cast<GUI::CCheckCtrl*>(list.GetItemWnd(ROW_ENABLE_DST,1));
		assert(pCheck);
		m_dstData.enable = pCheck->IsCheck()?1:0;
		OnModifyDstData();
		return KEY_VIRTUAL_MSG_CONTINUE;
	}

	if( ROW_TIME_OFFSET == row && 1 == col )
	{
		GUI::CComboCtrl* pCombo = dynamic_cast<GUI::CComboCtrl*>(list.GetItemWnd(ROW_TIME_OFFSET,1));
		assert(pCombo);
		m_dstData.offSet = pCombo->GetCurItemData();
		OnModifyDstData();
		return KEY_VIRTUAL_MSG_CONTINUE;
	}
	
	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CCfgDaylightDlg::OnClickRadio()
{
	CheckUI(true);

	GUI::CRadioCtrl* pRadio = m_rdGroup.GetCheckRadio();
	assert(pRadio);
	switch ( static_cast<PAGE_ITEM>(pRadio->GetData()) )
	{
	case PAGE_WEEK:
		m_dstData.type = DAYLIGHT_TYPE_WEEK;
		break;
	case PAGE_DATE:
		m_dstData.type = DAYLIGHT_TYPE_DAY;
		break;
	default:
		assert(false);
	}
	OnModifyDstData();

	return KEY_VIRTUAL_MSG_CONTINUE;
}
