/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : xzw
** Date         : 2011-04-23
** Name         : SearchCondition2Dlg.cpp
** Version      : 3.0
** Description  : 
** Modify Record:
1:
***********************************************************************/
#include "SearchCondition2Dlg.h"
#include "NetProtocol.h"
#include "FuncCustom.h"

//extern NET_LOGIN_SUCCESS_REPLY *g_p_login_succ;
//////////////////////////////////////////////////////////////////////////
CSearchCondition2Dlg::CSearchCondition2Dlg() : m_channelNum(0), m_authCH(0)
{
}

CSearchCondition2Dlg::~CSearchCondition2Dlg()
{

}

void CSearchCondition2Dlg::SetInitialInfo(unsigned long channelNum, ULONGLONG authCH)
{
	m_channelNum = channelNum;
	m_authCH	= authCH;
}

unsigned long CSearchCondition2Dlg::Time()
{
	GUI::CCalendarCtrl* pCalendar = dynamic_cast<GUI::CCalendarCtrl*>(GetScrollWnd().GetWnd(0));
	if (pCalendar == NULL)
	{
		return 0;
	}
	else
	{
		return pCalendar->Time();
	}
}

ULONGLONG CSearchCondition2Dlg::CH()
{
	ULONGLONG chnnBits = 0;
	unsigned char chnnNum = m_channelNum;
	GUI::CCheckCtrl* pCKCH;
	for (int i=0; i<chnnNum; ++i)
	{
		pCKCH = dynamic_cast<GUI::CCheckCtrl*>(GetScrollWnd().GetWnd(ID_CH + i));
		chnnBits |= ((!pCKCH->IsCheck()) ? 0 : (0x01 << i));
	}

	return chnnBits;
}


//////////////////////////////////////////////////////////////////////////
void CSearchCondition2Dlg::OnInitial()
{
	assert(m_channelNum > 0);

	GUI::CScrollDialog::OnInitial();

	unsigned short x = 5, y = 5, offset = 50, border = 2;

	ULONGLONG CH = 0;
	int localVideoInputNum = 0, netVideoInputNum = 0;
	int maxChnn = 0, effectChnn = 0;

	IsSupportCH(CH, &effectChnn, &localVideoInputNum, &netVideoInputNum);

	GUI::CScrollWnd& wnd = GetScrollWnd();
	GUI::CRect rect(x, y, x + m_length[CALENDAR_WIDTH], y + m_length[CALENDAR_HEIGHT]);

	wnd.SetSizeOffset(m_length[CALENDAR_WIDTH] + x, m_length[CALENDAR_HEIGHT] + y);
	wnd.SetViewMaxSize(m_length[CALENDAR_WIDTH] + x, m_length[CALENDAR_HEIGHT]* 3 + 3 * y );
	
	GUI::CCalendarCtrl* pCalendar = dynamic_cast<GUI::CCalendarCtrl*>(wnd.AddWnd(WND_TYPE_CALENDAR, CTRLID_SEARCH_CALENDAR, rect, 0, 0, ID_CALENDAR));

	//////////////////////////////////////////////////////////////////////////
	unsigned char groupNum = (m_channelNum + 3)/4;
	//m_pCKGroup = new GUI::CCheckCtrl [groupNum];
	GUI::CCheckCtrl* pCKGroup = NULL;
	x = 10; y += (m_length[CALENDAR_HEIGHT] + 8);
	for (int i=0; i<groupNum; ++i)
	{
		rect.SetRect(x, y + i * m_length[CHECK_HEIGHT], x + m_length[CHECK_WIDTH], y + m_length[CHECK_HEIGHT] * (i + 1));
		pCKGroup = dynamic_cast<GUI::CCheckCtrl*>(wnd.AddWnd(WND_TYPE_CHECK, CTRLID_SEARCH_GROUP_0 + i, rect, 0, i, ID_GROUP + i));
	}

	int localChnnNum = CProduct::Instance()->LocalVideoInputNum();
	char ch [] = "32";
	//m_pCKCH = new GUI::CCheckCtrl [m_channelNum];
	GUI::CCheckCtrl* pCKCH;
	x = 40; y = (m_length[CALENDAR_HEIGHT] + 13), offset = 54;
	rect.SetRect(x, y, x + offset, y + m_length[CHECK_HEIGHT]);
	for (int i=0; i<m_channelNum; ++i)
	{
		sprintf(ch, "%d", i+1);
		rect.SetRect(x + (i % 4) * offset, y + (i / 4) * m_length[CHECK_HEIGHT], x + (1 + i % 4) * offset, y + (1 + i / 4) * m_length[CHECK_HEIGHT]);
		//m_pCKCH[i].Create(CTRLID_SEARCH_CH_0+i, x+(i%4)*offset, y+(i/4)*m_length[CHECK_HEIGHT], offset, m_length[CHECK_HEIGHT], this, i%4+1, 1+i/4);
		pCKCH = dynamic_cast<GUI::CCheckCtrl*>(wnd.AddWnd(WND_TYPE_CHECK, CTRLID_SEARCH_CH_0 + i, rect, (i%4) + 1, i/4, ID_CH + i));
		pCKCH->SetCaption(ch, false, false);

		if (m_authCH & ((ULONGLONG)(0x01) << i))
		{
			pCKCH->SetCheck(true, false);
		}
		else
		{
			pCKCH->SetEnable(false, false);
		}
	}


	//////////////////////////////////////////////////////////////////////////
	for ( int i=0; i<groupNum; i++ )
	{
		DoClickChnn( i * 4, false );
	}
}


void CSearchCondition2Dlg::OnDestroy()
{
	GUI::CDialog::OnDestroy();

}

void CSearchCondition2Dlg::OnPaint(PAINT_STATUS status)
{
	GUI::CScrollDialog::OnPaint(status);

	int cx, cy;
	GetCurScrollPos(cx, cy);

	//画列
	int x = m_rect.m_left + m_length[CHECK_WIDTH] + 12;
	int start = m_rect.m_top + m_length[CALENDAR_HEIGHT] + 10 - cy * (m_length[CALENDAR_HEIGHT] + 5);
	int end = start + ((m_channelNum + 3)/4)*m_length[CHECK_HEIGHT];
	int offset = 5;

	GUI::CWnd::DrawLine(m_rect.m_left+offset, start, m_rect.m_left+offset, end, COLOR_LINE_GRAY);//最左边的竖条
	GUI::CWnd::DrawLine(x, start, x, end, COLOR_LINE_GRAY);//最左边的竖条
	GUI::CWnd::DrawLine(m_rect.m_right-offset - 20, start, m_rect.m_right-offset- 20, end, COLOR_LINE_GRAY);//最左边的竖条

	//画行
	int y = start;// + m_length[CHECK_HEIGHT];
	while (y <= end)
	{
		GUI::CWnd::DrawLine(m_rect.m_left+offset, y, m_rect.m_right-offset -20, y, COLOR_LINE_GRAY);
		y += m_length[CHECK_HEIGHT];
	}

}

void CSearchCondition2Dlg::MarkDay(unsigned short year, unsigned char month, unsigned char day, unsigned long color, bool bRefresh)
{
	GUI::CCalendarCtrl* pCalendar = dynamic_cast<GUI::CCalendarCtrl*>(GetScrollWnd().GetWnd(ID_CALENDAR));
	pCalendar->MarkDay(year, month, day, color, bRefresh);
}

BEGIN_MSG_MAP(CSearchCondition2Dlg, GUI::CScrollDialog)
	ON_MSG_EX(GetScrollWnd().GetID(), KEY_VIRTUAL_MSG_DEFAULT_TAG, OnClickScrollWnd)
END_MSG_MAP()

unsigned long CSearchCondition2Dlg::OnClickScrollWnd(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	int index = -1;
	unsigned long tmpIndex = (keyID >> 20)&0x7FF;
	if( tmpIndex < 0x7FF )
	{
		index = tmpIndex;
	}
	unsigned long id = keyID&0xFFFFF;

	printf("(%s,line=%d)index=%d\n",__FILE__,__LINE__,index);

	if (index > 0)
	{
		if ((index >= ID_CH) && (index <= ID_CH + 32))
		{
			OnClickChnn(index);
		}
		if ((index >= ID_GROUP) && (index <= ID_GROUP + 7))
		{
			OnClickGroup(index);
		}
	}
	return KEY_VIRTUAL_MSG_STOP;

}

unsigned long CSearchCondition2Dlg::OnClickChnn( unsigned long ctrlID )
{
	return DoClickChnn( ctrlID - ID_CH , true );
}

unsigned long CSearchCondition2Dlg::OnClickGroup( unsigned long ctrlID )
{
	unsigned long groupNum = ctrlID - ID_GROUP;
	GUI::CCheckCtrl* pCKGroup = dynamic_cast<GUI::CCheckCtrl*>(GetScrollWnd().GetWnd(ID_GROUP + groupNum));
	GUI::CCheckCtrl* pCKCH;

	bool check = pCKGroup->IsCheck();
	for ( int i = 0; i < 4; i++ )
	{
		pCKCH = dynamic_cast<GUI::CCheckCtrl*>(GetScrollWnd().GetWnd(ID_CH + groupNum * 4 + i));
		if(m_channelNum <= (groupNum * 4 + i))
		{
			break;
		}

		if (m_authCH & ((ULONGLONG)(0x01) << (groupNum * 4 + i)))
		{
			if( pCKCH->IsCheck() != check )
			{
				pCKCH->SetCheck( check, true );
			}
		}
	}

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CSearchCondition2Dlg::DoClickChnn( unsigned long index, bool bRefresh )
{
	int groupNum = index / 4;
	GUI::CCheckCtrl* pCKCH;

	bool check = true;
	for ( int i = 0; i < 4; i++ )
	{
		pCKCH = dynamic_cast<GUI::CCheckCtrl*>(GetScrollWnd().GetWnd(ID_CH + groupNum * 4 + i));
		if( groupNum * 4 + i > m_channelNum - 1 )
		{
			break;
		}

		if( !pCKCH->IsCheck() )
		{
			check = false;
			break;
		}
	}
	GUI::CCheckCtrl* pCKGroup = dynamic_cast<GUI::CCheckCtrl*>(GetScrollWnd().GetWnd(ID_GROUP + groupNum));
	if( pCKGroup->IsCheck() != check )
	{
		pCKGroup->SetCheck( check, bRefresh );
	}

	return KEY_VIRTUAL_MSG_STOP;
}

//end

