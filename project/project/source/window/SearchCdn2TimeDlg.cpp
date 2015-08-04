/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : L
** Date         : 2009-08-25
** Name         : SearchCdn2TimeDlg.h
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/


#include "SearchCdn2TimeDlg.h"
#include "NetProtocol.h"
#include "FuncCustom.h"
//////////////////////////////////////////////////////////////////////////

CSearchCdn2TimeDlg::CSearchCdn2TimeDlg() : m_pCKChnn(NULL), m_pCKGroup(NULL)
{
	m_channelNum = 0;
	m_authCH = 0;
}

CSearchCdn2TimeDlg::~CSearchCdn2TimeDlg()
{

}

void CSearchCdn2TimeDlg::OnInitial()
{
	assert(m_channelNum>0);

	GUI::CDialog::OnInitial();
	//////////////////////////////////////////////////////////////////////////

	unsigned short xLeft = 10, yTop = 15;
	unsigned short x = xLeft, y = yTop;
	unsigned short xOff = 5, yOff = 5;
	unsigned short width = m_rect.Width() - 2 * xLeft;
	unsigned short rowH = m_length[TIME_CTRL_HEIGHT];

	ULONGLONG CH = 0;
	int localVideoInputNum = 0, netVideoInputNum = 0;
	int maxChnn = 0, effectChnn = 0;

	IsSupportCH(CH, &effectChnn, &localVideoInputNum, &netVideoInputNum);

	maxChnn = localVideoInputNum + netVideoInputNum;

	char str[128] = {0};
	m_scStart.Create( GUI::CWnd::GetRandWndID(), x, y, 90, rowH, this, m_pStrTable->String(E_STR_ID_START_TIME) );
	//x += width - rowH;
	//m_btnStart.Create( GUI::CWnd::GetRandWndID(), x, y, rowH, rowH, this, 1, 1 );
	//m_btnStart.SetImage( PAINT_STATUS_NORMAL, BMP_BTN_PLAY, false );
	//m_btnStart.SetImage( PAINT_STATUS_FOCUS, BMP_BTN_PLAY_FOCUS, false );
	//m_btnStart.SetImage( PAINT_STATUS_DISABLE, BMP_BTN_PLAY_DISABLE, false );

	x = xLeft + 94;
	//y += rowH;
	//m_timeStart.Create( GUI::CWnd::GetRandWndID(), x, y, width - 4 * xOff, rowH, this, 1, 1 );
	m_dateStart.Create(GetRandWndID(),x,y,m_length[DATE_CTRL_WIDTH],m_length[DATE_CTRL_HEIGHT],this,0,1);
	y += m_length[DATE_CTRL_HEIGHT] + yOff;
	m_timeStart.Create(GetRandWndID(),x,y,m_length[DATE_CTRL_WIDTH],m_length[TIME_CTRL_HEIGHT],this,1,1);

	x = xLeft;
	y += rowH + yOff + 4;
	m_scEnd.Create( GUI::CWnd::GetRandWndID(), x, y, 90, rowH, this, m_pStrTable->String(E_STR_ID_END_TIME) );
	//x += width - rowH;
	//m_btnEnd.Create( GUI::CWnd::GetRandWndID(), x, y, rowH, rowH, this, 1, 1 );
	//m_btnEnd.SetImage( PAINT_STATUS_NORMAL, BMP_BTN_PLAY, false );
	//m_btnEnd.SetImage( PAINT_STATUS_FOCUS, BMP_BTN_PLAY_FOCUS, false );
	//m_btnEnd.SetImage( PAINT_STATUS_DISABLE, BMP_BTN_PLAY_DISABLE, false );

	x = xLeft + 94;
	//y += rowH;

	m_dateEnd.Create(GetRandWndID(),x,y,m_length[DATE_CTRL_WIDTH],m_length[DATE_CTRL_HEIGHT],this,0,2);
	
	y += m_length[DATE_CTRL_HEIGHT] + yOff;
	m_timeEnd.Create( GetRandWndID(), x, y,m_length[DATE_CTRL_WIDTH],m_length[DATE_CTRL_HEIGHT],this,1,2);
	
	//y += rowH + yOff;
	x = xLeft;
	y += yOff + 8;
	unsigned short numChnn = m_channelNum;
	unsigned short numLocalChnn = CProduct::Instance()->LocalVideoInputNum();
	assert (numChnn >= 4);
	m_pCKChnn = new GUI::CCheckCtrl [numChnn];
	m_pCKGroup = new GUI::CCheckCtrl [(numChnn+3)/4];

	int j = 0;
	unsigned short ckW = (width-m_length[CHECK_WIDTH]) / 4;
	unsigned short ckH = m_length[CHECK_HEIGHT] + 2;
	for ( int i = 0; i < numChnn; i++ )
	{
		if( i % 4 == 0 )
		{
			x = xLeft;
			y += ckH;
			m_pCKGroup[j++].Create( CTRLID_SEARCH_CDN2_GROUP_0 + j -1 , x, y, m_length[CHECK_WIDTH], ckH - 2, this, 0, j + 3 );
			x = 40;
		}

		m_pCKChnn[i].Create( CTRLID_SEARCH_CDN2_CH_0 + i, x, y, ckW, ckH - 2, this, ( i % 4 ) + 1, j + 3 );
		x += ckW;
		sprintf( str, "%d", i + 1);

		m_pCKChnn[i].SetCaption( str, false, false );

		if (m_authCH & ((ULONGLONG)(0x01) << i))
		{
			m_pCKChnn[i].SetCheck( true, false );
		}
		else
		{
			m_pCKChnn[i].SetEnable(false, false);
		}
	}


	//////////////////////////////////////////////////////////////////////////
	for ( int i = 0; i < numChnn; i += 4 )
	{
		if ((!m_pCKChnn[i].IsEnable())&&(!m_pCKChnn[i+1].IsEnable())&&(!m_pCKChnn[i+2].IsEnable())&&(!m_pCKChnn[i+3].IsEnable()))
		{
			m_pCKGroup[i/4].SetEnable(false,false);
		}
		DoClickChnn( i, false );
	}
}

void CSearchCdn2TimeDlg::OnDestroy()
{
	GUI::CDialog::OnDestroy();

	m_scStart.Destroy();
	m_scEnd.Destroy();
	m_dateStart.Destroy();
	m_timeStart.Destroy();
	m_dateEnd.Destroy();
	m_timeEnd.Destroy();

	int j = 0;
	for ( int i = 0; i < m_channelNum; i++ )
	{
		if( i % 4 == 0 )
		{
			m_pCKGroup[j++].Destroy();
		}

		m_pCKChnn[i].Destroy();
	}

	delete [] m_pCKGroup;
	delete [] m_pCKChnn;
	m_pCKGroup = NULL;
	m_pCKChnn = NULL;
}
void CSearchCdn2TimeDlg::OnPaint(PAINT_STATUS status)
{
	GUI::CDialog::OnPaint(status);

	unsigned short xLeft = 10, yTop = 15;
	unsigned short x = xLeft, y = yTop;
	unsigned short xOff = 5, yOff = 5;
	unsigned short width = m_rect.Width() - 2 * xLeft;
	unsigned short rowH = m_length[TIME_CTRL_HEIGHT];

	unsigned short rcLeft = m_rect.m_left + xLeft - xOff;
	unsigned short rcTop = m_rect.m_top + yTop - yOff;
	unsigned short rcWidth = width + 2 * xOff;
	
	unsigned short tmH = 4 * rowH + 4 * yOff + 8;
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
	rcTop = m_rect.m_top + yTop + 4 * rowH + 6 * yOff;
	rcWidth = width + 2 * xOff;
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

void CSearchCdn2TimeDlg::SetInitialInfo(unsigned long channelNum, ULONGLONG authCH)
{
	m_channelNum = channelNum;
	m_authCH = authCH;
}

unsigned long CSearchCdn2TimeDlg::TimeStart()
{
	tm tmpTm;
	memset(&tmpTm,0,sizeof(tm));
	m_dateStart.GetDate(tmpTm.tm_year,tmpTm.tm_mon,tmpTm.tm_mday);
	m_timeStart.GetTime(tmpTm.tm_hour,tmpTm.tm_min,tmpTm.tm_sec);
	
	tmpTm.tm_year -= 1900;

	return TmToDVRTime32(tmpTm);
}

unsigned long CSearchCdn2TimeDlg::TimeEnd()
{
	tm tmpTm;
	memset(&tmpTm,0,sizeof(tm));
	m_dateEnd.GetDate(tmpTm.tm_year,tmpTm.tm_mon,tmpTm.tm_mday);
	m_timeEnd.GetTime(tmpTm.tm_hour,tmpTm.tm_min,tmpTm.tm_sec);

	tmpTm.tm_year -= 1900;

	return TmToDVRTime32(tmpTm);
}

ULONGLONG CSearchCdn2TimeDlg::CH()
{
	ULONGLONG bit = 0;
	for ( int i=0; i< m_channelNum; i++ )
	{
		if( m_pCKChnn[i].IsCheck() )
		{
			bit |= (ULONGLONG)0x1 << i;
		}
	}
	return bit;
}

void CSearchCdn2TimeDlg::SetStartTime(tm time)
{
	m_dateStart.SetDate(time.tm_year+1900,time.tm_mon,time.tm_mday,false);
	m_timeStart.SetTime(time.tm_hour,time.tm_min,time.tm_sec,false);
}

void CSearchCdn2TimeDlg::SetEndTime(tm time)
{
	m_dateEnd.SetDate(time.tm_year+1900,time.tm_mon,time.tm_mday,false);
	m_timeEnd.SetTime(time.tm_hour,time.tm_min,time.tm_sec,false);
}


BEGIN_MSG_MAP(CSearchCdn2TimeDlg, GUI::CDialog)
	//ON_MSG_EX(m_btnStart.GetID(), KEY_VIRTUAL_ENTER, OnTimeStart)
	//ON_MSG_EX(m_btnEnd.GetID(), KEY_VIRTUAL_ENTER, OnTimeEnd)
	ON_MSG_CTRL_RANGE(CTRLID_SEARCH_CDN2_CH_0, CTRLID_SEARCH_CDN2_CH_47, KEY_VIRTUAL_ENTER, OnClickChnn)
	ON_MSG_CTRL_RANGE(CTRLID_SEARCH_CDN2_GROUP_0, CTRLID_SEARCH_CDN2_GROUP_11, KEY_VIRTUAL_ENTER, OnClickGroup)
END_MSG_MAP()

//unsigned long CSearchCdn2TimeDlg::OnTimeStart(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
//{
//	GUI::CRect rect = m_btnStart.GetWindowRect();
//	unsigned short cx = m_length[CALENDARDLG_WIDTH];
//	unsigned short cy = m_length[CALENDARDLG_HEIGHT];
//	unsigned short x = rect.m_right - cx;
//	unsigned short y = rect.m_bottom;
//
//	m_calendarDlg.DoModal( NULL,  x, y, cx, cy, false );
//
//	assert( GetParent() );
//	GetParent()->Repaint();
//
//	unsigned long time = m_calendarDlg.GetCalCtrl()->Time();
//	tm tmTime = DVRTime32ToTm(time);
//	tmTime.tm_mon = tmTime.tm_mon + 1;
//	tmTime.tm_year = tmTime.tm_year + 1900;
//	tmTime.tm_year = (tmTime.tm_year < 2000) ? 2000 : tmTime.tm_year;
//	tmTime.tm_hour = 0;
//	tmTime.tm_min  = 0;
//	tmTime.tm_sec  = 0;
//	m_timeStart.SetTime(tmTime);
//
//	return KEY_VIRTUAL_MSG_STOP;
//}

//unsigned long CSearchCdn2TimeDlg::OnTimeEnd(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
//{
//	GUI::CRect rect = m_btnEnd.GetWindowRect();
//	unsigned short cx = m_length[CALENDARDLG_WIDTH];
//	unsigned short cy = m_length[CALENDARDLG_HEIGHT];
//	unsigned short x = rect.m_right - cx;
//	unsigned short y = rect.m_bottom;
//	m_calendarDlg.DoModal( NULL,  x, y, cx, cy, false );
//
//	assert( GetParent() );
//	GetParent()->Repaint();
//
//	unsigned long time = m_calendarDlg.GetCalCtrl()->Time();
//	tm tmTime = DVRTime32ToTm(time);
//	tmTime.tm_mon = tmTime.tm_mon + 1;
//	tmTime.tm_year = tmTime.tm_year + 1900;
//	tmTime.tm_year = (tmTime.tm_year < 2000) ? 2000 : tmTime.tm_year;
//	tmTime.tm_hour = 23;
//	tmTime.tm_min  = 59;
//	tmTime.tm_sec  = 59;
//	m_timeEnd.SetTime(tmTime);
//
//	
//	return KEY_VIRTUAL_MSG_STOP;
//}

unsigned long CSearchCdn2TimeDlg::OnClickChnn( unsigned long ctrlID )
{
	return DoClickChnn( ctrlID - CTRLID_SEARCH_CDN2_CH_0, true );
}

unsigned long CSearchCdn2TimeDlg::OnClickGroup( unsigned long ctrlID )
{
	unsigned long groupNum = ctrlID - CTRLID_SEARCH_CDN2_GROUP_0;

	bool check = m_pCKGroup[groupNum].IsCheck();
	for ( int i = 0; i < 4; i++ )
	{
		if( groupNum * 4 + i > m_channelNum - 1 )
		{
			break;
		}

		if( m_pCKChnn[groupNum * 4 + i].IsCheck() != check )
		{
			m_pCKChnn[groupNum * 4 + i].SetCheck( check, true );
		}
	}

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CSearchCdn2TimeDlg::DoClickChnn( unsigned long index, bool bRefresh )
{
	int groupNum = index / 4;

	bool check = true;
	for ( int i = 0; i < 4; i++ )
	{
		if( groupNum * 4 + i > m_channelNum - 1 )
		{
			break;
		}

		if( m_pCKChnn[groupNum * 4 + i].IsEnable() && !m_pCKChnn[groupNum * 4 + i].IsCheck() )
		{
			check = false;
			break;
		}
	}

	if( m_pCKGroup[groupNum].IsCheck() != check )
	{
		m_pCKGroup[groupNum].SetCheck( check, bRefresh );
	}

	return KEY_VIRTUAL_MSG_STOP;
}
