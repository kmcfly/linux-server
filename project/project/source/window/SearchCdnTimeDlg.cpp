/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : xzw
** Date         : 2011-04-25
** Name         : SearchCdnTimeDlg.cpp
** Version      : 3.0
** Description  : 
** Modify Record:
1:
***********************************************************************/

#include "SearchCdnTimeDlg.h"
#include "NetProtocol.h"
#include "FuncCustom.h"

CSearchCdnTimeDlg::CSearchCdnTimeDlg() 
{
	m_channelNum = 0;
	m_authCH = 0;
}

CSearchCdnTimeDlg::~CSearchCdnTimeDlg()
{

}

void CSearchCdnTimeDlg::OnInitial()
{
	assert(m_channelNum>0);

	GUI::CScrollDialog::OnInitial();
	//////////////////////////////////////////////////////////////////////////
	GUI::CRect rect;
	unsigned short xLeft = 10, yTop = 15;
	unsigned short x = xLeft, y = yTop;
	unsigned short xOff = 5, yOff = 5;
	unsigned short width = m_rect.Width() - 2 * xLeft;
	unsigned short rowH = m_length[TIME_CTRL_HEIGHT];

	ULONGLONG CH = 0;
	int localVideoInputNum = 0, netVideoInputNum = 0;
	int maxChnn = 0, effectChnn = 0;

	IsSupportCH(CH, &effectChnn, &localVideoInputNum, &netVideoInputNum);

	GUI::CScrollWnd& wnd = GetScrollWnd();
	wnd.SetSizeOffset(m_rect.Width(), rowH * 1);
	wnd.SetViewMaxSize(m_rect.Width(), rowH * 16);

	rect.SetRect(x, y, x + 90, y + rowH);

	char str[128] = {0};
	GUI::CStatic* pStatic =dynamic_cast<GUI::CStatic*>(wnd.AddWnd(WND_TYPE_STATIC, GUI::CWnd::GetRandWndID(), rect, 0, 0, ID_START_STATIC));
	pStatic->SetCaption(m_pStrTable->String(E_STR_ID_START_TIME), false);
	x = xLeft + 94;
	rect. SetRect(x, y, x + m_length[DATE_CTRL_WIDTH], y + m_length[DATE_CTRL_HEIGHT]);
	GUI::CDateCtrl* pDate = dynamic_cast<GUI::CDateCtrl*>(wnd.AddWnd(WND_TYPE_DATE, GUI::CWnd::GetRandWndID(), rect, 0, 1, ID_DATE_START));
	//m_dateStart.Create(GetRandWndID(),x,y,m_length[DATE_CTRL_WIDTH],m_length[DATE_CTRL_HEIGHT],this,0,1);
	y += m_length[DATE_CTRL_HEIGHT] + yOff;
	rect.SetRect(x, y, x + m_length[DATE_CTRL_WIDTH], y + m_length[TIME_CTRL_HEIGHT]);
	GUI::CTimeCtrl* pTime = dynamic_cast<GUI::CTimeCtrl*>(wnd.AddWnd(WND_TYPE_TIME, GUI::CWnd::GetRandWndID(), rect, 1, 1, ID_TIME_START));
	//m_timeStart.Create(GetRandWndID(),x,y,m_length[DATE_CTRL_WIDTH],m_length[TIME_CTRL_HEIGHT],this,1,1);

	x = xLeft;
	y += rowH + yOff;
	rect.SetRect(x, y, x + 90, y + rowH);
	pStatic = dynamic_cast<GUI::CStatic*>(wnd.AddWnd(WND_TYPE_STATIC, GUI::CWnd::GetRandWndID(), rect, 0, 0, ID_END_STATIC));
	pStatic->SetCaption(m_pStrTable->String(E_STR_ID_END_TIME), false);
	//m_scEnd.Create( GUI::CWnd::GetRandWndID(), x, y, 90, rowH, this, m_pStrTable->String(E_STR_ID_END_TIME) );

	x = xLeft + 94;
	rect.SetRect(x, y, x + m_length[DATE_CTRL_WIDTH], y + m_length[DATE_CTRL_HEIGHT]);
	pDate = dynamic_cast<GUI::CDateCtrl*>(wnd.AddWnd(WND_TYPE_DATE, GUI::CWnd::GetRandWndID(), rect, 0, 2, ID_DATE_END));
	//m_dateEnd.Create(GetRandWndID(),x,y,m_length[DATE_CTRL_WIDTH],m_length[DATE_CTRL_HEIGHT],this,0,2);

	y += m_length[DATE_CTRL_HEIGHT] + yOff;
	rect.SetRect(x, y, x + m_length[DATE_CTRL_WIDTH], y + m_length[DATE_CTRL_HEIGHT]);
	pTime = dynamic_cast<GUI::CTimeCtrl*>(wnd.AddWnd(WND_TYPE_TIME, GUI::CWnd::GetRandWndID(), rect, 1, 2, ID_TIME_END));
	//m_timeEnd.Create( GetRandWndID(), x, y,m_length[DATE_CTRL_WIDTH],m_length[DATE_CTRL_HEIGHT],this,1,2);

	x = xLeft;
	y += yOff + 8;
	unsigned short numChnn = m_channelNum;
	unsigned short numLocalChnn = CProduct::Instance()->LocalVideoInputNum();
	assert (numChnn >= 4);
	GUI::CCheckCtrl*	pCKChnn;
	GUI::CCheckCtrl*	pCKGroup;

	int j = 0;
	unsigned short ckW = (width-m_length[CHECK_WIDTH]) / 4 - 5;
	unsigned short ckH = m_length[CHECK_HEIGHT] + 2;
	for ( int i = 0; i < numChnn; i++ )
	{
		if( i % 4 == 0 )
		{
			x = xLeft;
			y += ckH;
			rect.SetRect(x, y, x + m_length[CHECK_WIDTH], y + ckH - 2);
			pCKGroup = dynamic_cast<GUI::CCheckCtrl*>(wnd.AddWnd(WND_TYPE_CHECK, CTRLID_SEARCH_CDN2_GROUP_0 + j, rect, 0, j + 4, ID_GROUP + j)); 
			j++;
			//m_pCKGroup[j++].Create( CTRLID_SEARCH_CDN2_GROUP_0 + j -1 , x, y, m_length[CHECK_WIDTH], ckH - 2, this, 0, j + 3 );
			x = 40;
		}

		rect.SetRect(x, y, x + ckW, y + ckH - 2);
		pCKChnn = dynamic_cast<GUI::CCheckCtrl*>(wnd.AddWnd(WND_TYPE_CHECK, CTRLID_SEARCH_CDN2_CH_0 + i, rect, (i % 4) + 1, j + 3, ID_CKCH + i));
		//m_pCKChnn[i].Create( CTRLID_SEARCH_CDN2_CH_0 + i, x, y, ckW, ckH - 2, this, ( i % 4 ) + 1, j + 3 );
		x += ckW;
		sprintf( str, "%d", i + 1);

		pCKChnn->SetCaption( str, false, false );

		if (m_authCH & ((ULONGLONG)(0x01) << i))
		{
			pCKChnn->SetCheck( true, false );
		}
		else
		{
			pCKChnn->SetEnable(false, false);
		}
	}


	//////////////////////////////////////////////////////////////////////////
	for ( int i = 0; i < numChnn; i += 4 )
	{
		DoClickChnn( i, false );
	}
}

void CSearchCdnTimeDlg::OnPaint(PAINT_STATUS status)
{
	GUI::CScrollDialog::OnPaint(status);
	int cx, cy;
	GetCurScrollPos(cx, cy);

	unsigned short borderWidth = 23;
	unsigned short xLeft = 10, yTop = 15;
	unsigned short x = xLeft, y = yTop;
	unsigned short xOff = 5, yOff = 5;
	unsigned short width = m_rect.Width() - 2 * xLeft;
	unsigned short rowH = m_length[TIME_CTRL_HEIGHT];

	unsigned short rcLeft = m_rect.m_left + xLeft - xOff;
	unsigned short rcTop = m_rect.m_top + yTop - yOff - cy * rowH;
	unsigned short rcWidth = width + 2 * xOff - borderWidth;

	unsigned short tmH = 4 * rowH + 4 * yOff + 4;
	//// 上部分
	{
		x = rcLeft;
		y = rcTop;

		GUI::CWnd::DrawLine(x, y, x, y + tmH, COLOR_LINE_GRAY);//左竖
		GUI::CWnd::DrawLine(x, y, x + rcWidth, y, COLOR_LINE_GRAY);//上横
		GUI::CWnd::DrawLine(x + rcWidth, y, x + rcWidth, y + tmH, COLOR_LINE_GRAY); //右竖
		GUI::CWnd::DrawLine(x, y + tmH, x + rcWidth, y + tmH, COLOR_LINE_GRAY);//下横

	}

	unsigned short numChnn = m_channelNum;
	unsigned short numRow = numChnn / 4;
	if( numChnn % 4 != 0 ) numRow += 1;
	unsigned short ckH = m_length[CHECK_HEIGHT] + 2;
	rcLeft = m_rect.m_left + xLeft - xOff;
	rcTop = m_rect.m_top + yTop + 4 * rowH + 5 * yOff - cy * rowH;
	rcWidth = width + 2 * xOff - borderWidth; 
	//// 下部分
	{
		x = rcLeft;
		y = rcTop;
		unsigned short chnnH = numRow * ckH + 2;

		GUI::CWnd::DrawLine(x, y, x, y + chnnH, COLOR_LINE_GRAY ); // 左竖
		GUI::CWnd::DrawLine(x, y, x + rcWidth, y, COLOR_LINE_GRAY);//上横
		GUI::CWnd::DrawLine(x + rcWidth, y, x + rcWidth, y + chnnH, COLOR_LINE_GRAY); //右竖
		GUI::CWnd::DrawLine(x, y + chnnH, x + rcWidth, y + chnnH, COLOR_LINE_GRAY);//下横

		x = m_rect.m_left + m_length[CHECK_WIDTH] + 12;
		GUI::CWnd::DrawLine(x, y, x, y + chnnH, COLOR_LINE_GRAY ); // 中竖


		x = rcLeft;
		// 行横线
		for ( int i = 1; i < numRow; i ++ )
		{
			y += ckH;
			GUI::CWnd::DrawLine(x, y + 2, x + rcWidth, y + 2, COLOR_LINE_GRAY);//上横
		}
	}
}

void CSearchCdnTimeDlg::SetInitialInfo(unsigned long channelNum, ULONGLONG authCH)
{
	m_channelNum = channelNum;
	m_authCH = authCH;
}

unsigned long CSearchCdnTimeDlg::TimeStart()
{
	tm tmpTm;
	memset(&tmpTm,0,sizeof(tm));
	GUI::CDateCtrl* pDateStart = dynamic_cast<GUI::CDateCtrl*>(GetScrollWnd().GetWnd(ID_DATE_START));
	GUI::CTimeCtrl* pTimeStart = dynamic_cast<GUI::CTimeCtrl*>(GetScrollWnd().GetWnd(ID_TIME_START));
	pDateStart->GetDate(tmpTm.tm_year,tmpTm.tm_mon,tmpTm.tm_mday);
	pTimeStart->GetTime(tmpTm.tm_hour,tmpTm.tm_min,tmpTm.tm_sec);

	tmpTm.tm_year -= 1900;

	return TmToDVRTime32(tmpTm);
}

unsigned long CSearchCdnTimeDlg::TimeEnd()
{
	tm tmpTm;
	memset(&tmpTm,0,sizeof(tm));
	GUI::CDateCtrl* pDateEnd = dynamic_cast<GUI::CDateCtrl*>(GetScrollWnd().GetWnd(ID_DATE_END));
	GUI::CTimeCtrl* pTimeEnd = dynamic_cast<GUI::CTimeCtrl*>(GetScrollWnd().GetWnd(ID_TIME_END));
	pDateEnd->GetDate(tmpTm.tm_year,tmpTm.tm_mon,tmpTm.tm_mday);
	pTimeEnd->GetTime(tmpTm.tm_hour,tmpTm.tm_min,tmpTm.tm_sec);

	tmpTm.tm_year -= 1900;

	return TmToDVRTime32(tmpTm);
}

ULONGLONG CSearchCdnTimeDlg::CH()
{
	GUI::CCheckCtrl* pCKChnn;
	ULONGLONG bit = 0;
	for ( int i=0; i< m_channelNum; i++ )
	{
		pCKChnn = dynamic_cast<GUI::CCheckCtrl*>(GetScrollWnd().GetWnd(ID_CKCH + i));
		if(pCKChnn->IsCheck() )
		{
			bit |= (ULONGLONG)0x1 << i;
		}
	}
	return bit;
}

void CSearchCdnTimeDlg::SetStartTime(tm time)
{
	GUI::CDateCtrl* pDateStart = dynamic_cast<GUI::CDateCtrl*>(GetScrollWnd().GetWnd(ID_DATE_START));
	GUI::CTimeCtrl* pTimeStart = dynamic_cast<GUI::CTimeCtrl*>(GetScrollWnd().GetWnd(ID_TIME_START));
	pDateStart->SetDate(time.tm_year+1900,time.tm_mon,time.tm_mday,false);
	pTimeStart->SetTime(time.tm_hour,time.tm_min,time.tm_sec,false);
}

void CSearchCdnTimeDlg::SetEndTime(tm time)
{
	GUI::CDateCtrl* pDateEnd = dynamic_cast<GUI::CDateCtrl*>(GetScrollWnd().GetWnd(ID_DATE_END));
	GUI::CTimeCtrl* pTimeEnd = dynamic_cast<GUI::CTimeCtrl*>(GetScrollWnd().GetWnd(ID_TIME_END));
	pDateEnd->SetDate(time.tm_year+1900,time.tm_mon,time.tm_mday,false);
	pTimeEnd->SetTime(time.tm_hour,time.tm_min,time.tm_sec,false);
}


BEGIN_MSG_MAP(CSearchCdnTimeDlg, GUI::CScrollDialog)
	ON_MSG_EX(GetScrollWnd().GetID(), KEY_VIRTUAL_MSG_DEFAULT_TAG, OnClickScrollWnd)
	//ON_MSG_CTRL_RANGE(CTRLID_SEARCH_CDN2_CH_0, CTRLID_SEARCH_CDN2_CH_31, KEY_VIRTUAL_ENTER, OnClickChnn)
	//ON_MSG_CTRL_RANGE(CTRLID_SEARCH_CDN2_GROUP_0, CTRLID_SEARCH_CDN2_GROUP_7, KEY_VIRTUAL_ENTER, OnClickGroup)
END_MSG_MAP()


unsigned long CSearchCdnTimeDlg::OnClickScrollWnd(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	int index = -1;
	unsigned long tmpIndex = (keyID >> 20)&0x7FF;
	if( tmpIndex < 0x7FF )
	{
		index = tmpIndex;
	}
	unsigned long id = keyID&0xFFFFF;

	if ( index > 0)
	{
		if (index >= ID_CKCH && index <= ID_CKCH + 31)
		{
			OnClickChnn(index);
		}
		else if (index >=  ID_GROUP && index <= ID_GROUP + 7)
		{
			OnClickGroup(index);
		}
	}
	
	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CSearchCdnTimeDlg::OnClickChnn( unsigned long ctrlID )
{
	return DoClickChnn( ctrlID - ID_CKCH, true );
}

unsigned long CSearchCdnTimeDlg::OnClickGroup( unsigned long ctrlID )
{
	unsigned long groupNum = ctrlID - ID_GROUP;
	GUI::CCheckCtrl* pCKChnn;
	GUI::CCheckCtrl* pCKGroup = dynamic_cast<GUI::CCheckCtrl*>(GetScrollWnd().GetWnd(ctrlID));
	bool check = pCKGroup->IsCheck();
	for ( int i = 0; i < 4; i++ )
	{
		if( groupNum * 4 + i > m_channelNum - 1 )
		{
			break;
		}
		pCKChnn = dynamic_cast<GUI::CCheckCtrl*>(GetScrollWnd().GetWnd(ID_CKCH + groupNum * 4 + i));
		if( pCKChnn->IsCheck() != check )
		{
			pCKChnn->SetCheck( check, true );
		}
	}

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CSearchCdnTimeDlg::DoClickChnn( unsigned long index, bool bRefresh )
{
	int groupNum = index / 4;
	
	GUI::CCheckCtrl* pCKChnn;
	GUI::CCheckCtrl* pCKGroup;
	bool check = true;
	for ( int i = 0; i < 4; i++ )
	{
		if( groupNum * 4 + i > m_channelNum - 1 )
		{
			break;
		}
		pCKChnn = dynamic_cast<GUI::CCheckCtrl*>(GetScrollWnd().GetWnd(ID_CKCH + groupNum * 4 + i));
		if( !pCKChnn->IsCheck() )
		{
			check = false;
			break;
		}
	}
	pCKGroup =  dynamic_cast<GUI::CCheckCtrl*>(GetScrollWnd().GetWnd(ID_GROUP + groupNum ));
	if( pCKGroup->IsCheck() != check )
	{
		pCKGroup->SetCheck( check, bRefresh );
	}

	return KEY_VIRTUAL_MSG_STOP;
}
