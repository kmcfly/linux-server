#include "ShanghaiCfgDailySelectDialog.h"
#include "ShanghaiSnapMan.h"

CShanghaiCfgDailySelectDialog::CShanghaiCfgDailySelectDialog()
{
	m_codeTable = CCharCodeTable::Instance();
}

CShanghaiCfgDailySelectDialog::~CShanghaiCfgDailySelectDialog()
{

}

void CShanghaiCfgDailySelectDialog::SetInitialInfo(unsigned long channelNum)
{
	m_channelNum = channelNum;
}

unsigned long CShanghaiCfgDailySelectDialog::TimeStart()
{
	tm tmpTm,curTm;
	unsigned long curtime = 0;

	memset(&tmpTm,0,sizeof(tm));
	memset(&curTm,0,sizeof(tm));
	curtime = GetCurrTime32();
	curTm = DVRTime32ToTm(curtime);

	m_timeStart.GetTime(tmpTm.tm_hour,tmpTm.tm_min,tmpTm.tm_sec);

	tmpTm.tm_year = curTm.tm_year;
	tmpTm.tm_mon = curTm.tm_mon;
	tmpTm.tm_mday = curTm.tm_mday;
	return TmToDVRTime32(tmpTm);
}

ULONGLONG CShanghaiCfgDailySelectDialog::CH()
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

void CShanghaiCfgDailySelectDialog::SetStartTime(tm time)
{
	m_timeStart.SetTime(time.tm_hour,time.tm_min,time.tm_sec,false);
}

void CShanghaiCfgDailySelectDialog::OnInitial()
{
	GUI::CDialog::OnInitial();
	//////////////////////////////////////////////////////////////////////////

	unsigned short xLeft = 10, yTop = 15;
	unsigned short x = xLeft, y = yTop;
	unsigned short xOff = 5, yOff = 5;
	unsigned short width = m_rect.Width() - 2 * xLeft;
	unsigned short rowH = m_length[TIME_CTRL_HEIGHT];
	int totalcount=m_channelNum/4+1;

	char str[128] = {0};
	m_scStart.Create( GUI::CWnd::GetRandWndID(), x, y, 90, rowH, this, m_pStrTable->String(E_STR_ID_START_TIME) );
	x = xLeft + 94;
	m_timeStart.Create(GetRandWndID(),x,y,m_length[DATE_CTRL_WIDTH],m_length[TIME_CTRL_HEIGHT],this,1,1);

	x = xLeft;
	y += yOff + 8;
	unsigned short numChnn = m_channelNum;
	unsigned short numLocalChnn = 0;
#ifdef __DVR_ULTIMATE__
	numLocalChnn = CProduct::Instance()->NetVideoInputNum();
#else
	numLocalChnn = CProduct::Instance()->LocalVideoInputNum();
#endif
	assert (numChnn >= 4);
	m_pCKChnn = new GUI::CCheckCtrl [numChnn];
	m_pCKGroup = new GUI::CCheckCtrl [numChnn/4];

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
		if(i < numLocalChnn)
		{
			sprintf( str, "%d", i + 1);
		}
		else
		{
			sprintf(str, "D%d", i + 1 - numLocalChnn);
		}
		m_pCKChnn[i].SetCaption( str, false, false );
		m_pCKChnn[i].SetCheck( true, false );
	}

	//////////////////////////////////////////////////////////////////////////
	for ( int i = 0; i < numChnn; i += 4 )
	{
		DoClickChnn( i, false );
	}

}

void CShanghaiCfgDailySelectDialog::OnPaint(PAINT_STATUS status)
{
	GUI::CDialog::OnPaint(status);

	unsigned short xLeft = 10, yTop = 15;
	unsigned short x = xLeft, y = yTop;
	unsigned short xOff = 5, yOff = 5;
	unsigned short width = m_rect.Width() - 2 * xLeft;
	unsigned short rowH = m_length[TIME_CTRL_HEIGHT];

	unsigned short rcLeft = 0;
	unsigned short rcTop = 0;
	unsigned short rcWidth = 0;

	unsigned short numChnn = m_channelNum;
	unsigned short numRow = numChnn / 4;
	if( numChnn % 4 != 0 ) numRow += 1;
	unsigned short ckH = m_length[CHECK_HEIGHT] + 2;
	rcLeft = m_rect.m_left + xLeft - xOff;
	rcTop = m_rect.m_top + yTop + 1 * rowH + 2 * yOff;
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

BEGIN_MSG_MAP(CShanghaiCfgDailySelectDialog, GUI::CDialog)
	ON_MSG_CTRL_RANGE(CTRLID_SEARCH_CDN2_CH_0, CTRLID_SEARCH_CDN2_CH_47, KEY_VIRTUAL_ENTER, OnClickChnn)
	ON_MSG_CTRL_RANGE(CTRLID_SEARCH_CDN2_GROUP_0, CTRLID_SEARCH_CDN2_GROUP_11, KEY_VIRTUAL_ENTER, OnClickGroup)
END_MSG_MAP()

unsigned long CShanghaiCfgDailySelectDialog::OnClickChnn( unsigned long ctrlID )
{
	return DoClickChnn( ctrlID - CTRLID_SEARCH_CDN2_CH_0, true );
}

unsigned long CShanghaiCfgDailySelectDialog::OnClickGroup( unsigned long ctrlID )
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

unsigned long CShanghaiCfgDailySelectDialog::DoClickChnn( unsigned long index, bool bRefresh )
{
	int groupNum = index / 4;

	bool check = true;
	for ( int i = 0; i < 4; i++ )
	{
		if( groupNum * 4 + i > m_channelNum - 1 )
		{
			break;
		}

		if( !m_pCKChnn[groupNum * 4 + i].IsCheck() )
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

string CShanghaiCfgDailySelectDialog::ChangeChinese(const char *pstr)
{
	string outStr;
	if(NULL != m_codeTable)
	{
		if (-1 == m_codeTable->Convert(outStr, CCT_CODE_UTF8, pstr, CCT_CODE_GBK))
		{
			outStr = pstr;
		}
	}
	else
	{
		outStr = pstr;
	}
	return outStr;
}

