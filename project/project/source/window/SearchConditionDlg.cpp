/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : ysw
** Date         : 2009-08-01
** Name         : SearchConditionDlg.cpp
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/
#include "SearchConditionDlg.h"
#include "NetProtocol.h"
#include "FuncCustom.h"


//extern NET_LOGIN_SUCCESS_REPLY *g_p_login_succ;
//////////////////////////////////////////////////////////////////////////
CSearchConditionDlg::CSearchConditionDlg() : m_pCKCH(NULL), m_channelNum(0), m_authCH(0)
{
}

CSearchConditionDlg::~CSearchConditionDlg()
{

}

void CSearchConditionDlg::SetInitialInfo(unsigned long channelNum, ULONGLONG authCH)
{
	m_channelNum = channelNum;
	m_authCH	= authCH;
}

unsigned long CSearchConditionDlg::Time()
{
	return m_calendar.Time();
}

ULONGLONG CSearchConditionDlg::CH()
{
	ULONGLONG chnnBits = 0;
	unsigned char chnnNum = m_channelNum;
	for (int i=0; i<chnnNum; ++i)
	{
		chnnBits |= ((!m_pCKCH[i].IsCheck()) ? 0 : (0x01 << i));
	}

	return chnnBits;
}


//////////////////////////////////////////////////////////////////////////
void CSearchConditionDlg::OnInitial()
{
	assert(m_channelNum > 0);

	GUI::CDialog::OnInitial();
	
	unsigned short x = 5, y = 5, offset = 50;

	ULONGLONG CH = 0;
	int localVideoInputNum = 0, netVideoInputNum = 0;
	int maxChnn = 0, effectChnn = 0;

	IsSupportCH(CH, &effectChnn, &localVideoInputNum, &netVideoInputNum);

	maxChnn = localVideoInputNum + netVideoInputNum;
	m_calendar.Create(CTRLID_SEARCH_CALENDAR, x, y, m_length[CALENDAR_WIDTH], m_length[CALENDAR_HEIGHT], this, 0, 0);

	//////////////////////////////////////////////////////////////////////////
	unsigned char groupNum = (m_channelNum + 3)/4;
	m_pCKGroup = new GUI::CCheckCtrl [groupNum];
	x = 10; y += (m_length[CALENDAR_HEIGHT] + 8);
	for (int i=0; i<groupNum; ++i)
	{
		m_pCKGroup[i].Create(CTRLID_SEARCH_GROUP_0 + i, x, y, m_length[CHECK_WIDTH], m_length[CHECK_HEIGHT], this, 0, i+1);
		y += m_length[CHECK_HEIGHT];
	}

	int localChnnNum = localVideoInputNum; //CProduct::Instance()->LocalVideoInputNum();
	char ch [32] = "32";
	m_pCKCH = new GUI::CCheckCtrl [m_channelNum];
	x = 40; y = (m_length[CALENDAR_HEIGHT] + 13), offset = 54;
	for (int i=0; i<m_channelNum; ++i)
	{
		sprintf(ch, "%d", i+1);
		m_pCKCH[i].Create(CTRLID_SEARCH_CH_0+i, x+(i%4)*offset, y+(i/4)*m_length[CHECK_HEIGHT], offset, m_length[CHECK_HEIGHT], this, i%4+1, 1+i/4);
		m_pCKCH[i].SetCaption(ch, false, false);

		if (m_authCH & ((ULONGLONG)(0x01) << i))
		{
			m_pCKCH[i].SetCheck(true, false);
		}
		else
		{
			m_pCKCH[i].SetEnable(false, false);
		}
	}


	//////////////////////////////////////////////////////////////////////////
	for ( int i=0; i<groupNum; i++ )
	{
		if ((!m_pCKCH[i*4].IsEnable())&&(!m_pCKCH[i*4+1].IsEnable())&&(!m_pCKCH[i*4+2].IsEnable())&&(!m_pCKCH[i*4+3].IsEnable()))
		{
			m_pCKGroup[i].SetEnable(false,false);
		}
		DoClickChnn( i * 4, false );
	}
}


void CSearchConditionDlg::OnDestroy()
{
	GUI::CDialog::OnDestroy();

	if (NULL != m_pCKCH)
	{
		delete [] m_pCKCH;
		m_pCKCH = NULL;
	}

	if(NULL != m_pCKGroup )
	{
		delete [] m_pCKGroup;
		m_pCKGroup = NULL;
	}
}

void CSearchConditionDlg::OnPaint(PAINT_STATUS status)
{
	GUI::CDialog::OnPaint(status);

	//画列
	int x = m_rect.m_left + m_length[CHECK_WIDTH] + 12;
	int start = m_rect.m_top + m_length[CALENDAR_HEIGHT] + 10;
	int end = start + ((m_channelNum + 3)/4)*m_length[CHECK_HEIGHT];
	int offset = 5;

	GUI::CWnd::DrawLine(m_rect.m_left+offset, start, m_rect.m_left+offset, end, COLOR_LINE_GRAY);//最左边的竖条
	GUI::CWnd::DrawLine(x, start, x, end, COLOR_LINE_GRAY);//最左边的竖条
	GUI::CWnd::DrawLine(m_rect.m_right-offset, start, m_rect.m_right-offset, end, COLOR_LINE_GRAY);//最左边的竖条

	//画行
	int y = start;// + m_length[CHECK_HEIGHT];
	while (y <= end)
	{
		GUI::CWnd::DrawLine(m_rect.m_left+offset, y, m_rect.m_right-offset, y, COLOR_LINE_GRAY);
		y += m_length[CHECK_HEIGHT];
	}

}

void CSearchConditionDlg::MarkDay(unsigned short year, unsigned char month, unsigned char day, unsigned long color, bool bRefresh)
{
	m_calendar.MarkDay(year, month, day, color, bRefresh);
}

BEGIN_MSG_MAP(CSearchConditionDlg, GUI::CDialog)
	ON_MSG_CTRL_RANGE(CTRLID_SEARCH_CH_0, CTRLID_SEARCH_CH_31, KEY_VIRTUAL_ENTER, OnClickChnn)
	ON_MSG_CTRL_RANGE(CTRLID_SEARCH_GROUP_0, CTRLID_SEARCH_GROUP_7, KEY_VIRTUAL_ENTER, OnClickGroup)
END_MSG_MAP()

unsigned long CSearchConditionDlg::OnClickChnn( unsigned long ctrlID )
{
	return DoClickChnn( ctrlID - CTRLID_SEARCH_CH_0 , true );
}

unsigned long CSearchConditionDlg::OnClickGroup( unsigned long ctrlID )
{
	unsigned long groupNum = ctrlID - CTRLID_SEARCH_GROUP_0;

	bool check = m_pCKGroup[groupNum].IsCheck();
	for ( int i = 0; i < 4; i++ )
	{
		if(m_channelNum <= (groupNum * 4 + i))
		{
			break;
		}
		
		if (m_authCH & ((ULONGLONG)(0x01) << (groupNum * 4 + i)))
		{
			if( m_pCKCH[groupNum * 4 + i].IsCheck() != check )
			{
				m_pCKCH[groupNum * 4 + i].SetCheck( check, true );
			}
		}
	}

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CSearchConditionDlg::DoClickChnn( unsigned long index, bool bRefresh )
{
	int groupNum = index / 4;

	bool check = true;
	for ( int i = 0; i < 4; i++ )
	{
		if( groupNum * 4 + i > m_channelNum - 1 )
		{
			break;
		}

		if( !m_pCKCH[groupNum * 4 + i].IsCheck() )
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

//end

