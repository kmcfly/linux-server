/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : ZXX
** Date         : 2013-11-26
** Name         : WzdNatDSTDlg.cpp
** Version      : 1.0
** Description  : 向导页面对话框
** Modify Record:
***********************************************************************/
#include "WzdNatDSTDlg.h"


CWzdNatDSTDlg::CWzdNatDSTDlg()
{
	m_pWzdInfo = NULL;
	m_curPage = PAGE_NULL;
	memset(&m_dstData,0,sizeof(DAYLIGHT_INFO));
}

CWzdNatDSTDlg::~CWzdNatDSTDlg()
{

}

unsigned long CWzdNatDSTDlg::CheckValue()
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

void CWzdNatDSTDlg::InitCtrlInfo()
{
	memcpy(&m_dstData,&m_pWzdInfo->dstInfo,sizeof(DAYLIGHT_INFO));
	m_dstWeekDlg.SetDstData(&m_dstData);
	m_dstDateDlg.SetDstData(&m_dstData);
	//刷新界面
	RefreshView(false);
}

void CWzdNatDSTDlg::SetWzdInfo(NAT_WIZARD_INFO* pWzdInfo)
{
	m_pWzdInfo = pWzdInfo;
}

void CWzdNatDSTDlg::ShowWzdData(bool bRefresh)
{

}

void CWzdNatDSTDlg::GetWzdData(bool bNext)
{
	if(CheckValue() != 0)
	{
		return;
	}
	memcpy(&(m_pWzdInfo->dstInfo),&m_dstData,sizeof(DAYLIGHT_INFO));
}

void CWzdNatDSTDlg::OnInitial()
{
	CWzdNatPageDlg::OnInitial();

	//////////////////////////////////////////////////////////////////////////
	unsigned long x = m_length[DLG_OFFSET_SUB_LEFT];
	unsigned long y = m_length[DLG_OFFSET_SUB_TOP];
	unsigned long width = m_rect.Width() - m_length[DLG_OFFSET_SUB_LEFT] * 2;
	unsigned long height = m_length[LIST_ITEM_HEIGHT]*2;
	char str[128] = {0};

	m_listDSTCtrl.Create(GetRandWndID(),x,y,width,height,this,0,0,false);
	m_listDSTCtrl.AddColumn(NULL,width / 2 );
	m_listDSTCtrl.AddColumn(NULL,width / 2-4);
	m_listDSTCtrl.DrawLine(ROW_ENABLE_DST,2);

	m_listDSTCtrl.AddItem(ROW_ENABLE_DST,0,m_pStrTable->String(E_STR_ID_DST_FULL));
	GUI::CCheckCtrl* pCheck = dynamic_cast<GUI::CCheckCtrl*>(m_listDSTCtrl.AddItem(ROW_ENABLE_DST,1,WND_TYPE_CHECK));
	assert(pCheck);

	//strcpy(str,"Time Offset");
	m_listDSTCtrl.AddItem(ROW_TIME_OFFSET,0,m_pStrTable->String(E_STR_ID_DST_TIME_OFFSET));
	GUI::CComboCtrl* pCombo = dynamic_cast<GUI::CComboCtrl*>(m_listDSTCtrl.AddItem(ROW_TIME_OFFSET,1,WND_TYPE_COMBO));
	assert(pCombo);
	pCombo->AddItem("1",( 1 * ( 60 * 60 ) ) );
	pCombo->AddItem("2",( 2 * ( 60 * 60 ) ) );
	pCombo->SetCurItemData(60 * 60,false);

	m_listDSTCtrl.CreateListFocusMap();

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

	m_dstWeekDlg.Create(GetRandWndID(),x,y,width,height,this,0,2,false);
	m_dstDateDlg.Create(GetRandWndID(),x,y,width,height,this,1,2,false);

	//
	m_curPage = PAGE_NULL;
	m_rdWeek.SetCheck(true,false);
	CheckUI(false);

}

void CWzdNatDSTDlg::OnDestroy()
{

}

void CWzdNatDSTDlg::OnPaint(PAINT_STATUS status)
{
	CWzdNatPageDlg::OnPaint(status);

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

void CWzdNatDSTDlg::SetPage(PAGE_ITEM item /*= PAGE_WEEK*/,bool bRefresh /*= true*/ )
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

void CWzdNatDSTDlg::CheckUI(bool bRefresh)
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

void CWzdNatDSTDlg::RefreshView(bool bRefresh)
{
	GUI::CListCtrl& list=m_listDSTCtrl;
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

BEGIN_MSG_MAP(CWzdNatDSTDlg, GUI::CDialog)
	ON_MSG_EX(m_listDSTCtrl.GetID(), KEY_VIRTUAL_MSG_DEFAULT_TAG, OnClickList)
	ON_MSG(m_rdWeek.GetID(),KEY_VIRTUAL_ENTER,OnClickRadio)
	ON_MSG(m_rdDate.GetID(),KEY_VIRTUAL_ENTER,OnClickRadio)
END_MSG_MAP()

unsigned long CWzdNatDSTDlg::OnClickList(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	unsigned short row = (keyID >> 16)&0x7fff;	//行
	unsigned short col = keyID&0xffff;			//列
	GUI::CListCtrl& list = m_listDSTCtrl;

	if(1 == col)
	{
		switch(row)
		{
		case ROW_ENABLE_DST:
			{
				GUI::CCheckCtrl* pCheck = dynamic_cast<GUI::CCheckCtrl*>(list.GetItemWnd(ROW_ENABLE_DST,1));
				assert(pCheck);
				m_dstData.enable = pCheck->IsCheck()?1:0;
				return KEY_VIRTUAL_MSG_CONTINUE;
			}
		case ROW_TIME_OFFSET:
			{
				GUI::CComboCtrl* pCombo = dynamic_cast<GUI::CComboCtrl*>(list.GetItemWnd(ROW_TIME_OFFSET,1));
				assert(pCombo);
				m_dstData.offSet = pCombo->GetCurItemData();
				return KEY_VIRTUAL_MSG_CONTINUE;
			}
			break;
		default:
			break;
		}
	}

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CWzdNatDSTDlg::OnClickRadio()
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
	return KEY_VIRTUAL_MSG_CONTINUE;
}
//end
