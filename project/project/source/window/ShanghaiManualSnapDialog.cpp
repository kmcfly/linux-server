#include "ShanghaiManualSnapDialog.h"
#include "ShanghaiSnapMan.h"

CShanghaiManualSnapDialog::CShanghaiManualSnapDialog()
{
	m_codeTable = CCharCodeTable::Instance();
}

CShanghaiManualSnapDialog::~CShanghaiManualSnapDialog()
{

}

void CShanghaiManualSnapDialog::SetInitialInfo(unsigned long channelNum,unsigned long choseType)
{
	m_channelNum = channelNum;
	m_choseType = choseType;
}

void CShanghaiManualSnapDialog::OnInitial()
{
	GUI::CDialog::OnInitial();
	
	unsigned short xLeft =  m_rect.Width()/3, yTop = 25;
	unsigned short x = xLeft, y = yTop;
	unsigned short xOff = 5, yOff = 5;
	unsigned short width = 4*m_length[BTN_WIDTH]+50;
	unsigned short rowH = m_length[TIME_CTRL_HEIGHT];
	int totalcount=m_channelNum/4+1;

	x = xLeft;
	std::string outStr;
	outStr.clear();
	outStr = ChangeChinese("抓图通道选择:");
	m_Tip.Create(GetRandWndID(),x,y,2*m_length[BTN_WIDTH],m_length[BTN_HEIGHT],this,outStr.c_str()); 

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
	unsigned short ckW = m_length[BTN_WIDTH]+15;
	unsigned short ckH = m_length[CHECK_HEIGHT] + 2;
	char str[128] = {0};
	for ( int i = 0; i < numChnn; i++ )
	{
		if( i % 4 == 0 )
		{
			x = xLeft;
			y += ckH;
			m_pCKGroup[j++].Create( CTRLID_SEARCH_CDN2_GROUP_0 + j -1 , x, y, m_length[CHECK_WIDTH], ckH - 2, this, 0, j + 3 );
			//x = 40;
			x = x+ m_length[CHECK_WIDTH]+5;
		}

		m_pCKChnn[i].Create( CTRLID_SEARCH_CDN2_CH_0 + i, x, y, ckW, ckH - 2, this, ( i % 4 ) + 1, j + 3 );
		x += m_length[BTN_WIDTH]+15;
		memset(str,0,sizeof(str));
		if(i < numLocalChnn)
		{
			snprintf( str, 128,"%d", i + 1);
		}
		else
		{
			snprintf(str, 128,"D%d", i + 1 - numLocalChnn);
		}
		m_pCKChnn[i].SetCaption( str, false, false );
		m_pCKChnn[i].SetCheck( false, false );
	}

	//////////////////////////////////////////////////////////////////////////
	for ( int i = 0; i < numChnn; i += 4 )
	{
		DoClickChnn( i, false );
	}

	y += 2*m_length[CHECK_HEIGHT];
	x=xLeft;
	
	outStr.clear();
	outStr = ChangeChinese("警号");
	outStr += ":";
	m_policeTip.Create(GetRandWndID(),x,y,1.5*m_length[BTN_WIDTH],m_length[BTN_HEIGHT],this,outStr.c_str());//警号
	m_policeNumEdit.Create(GetRandWndID(),x+m_length[BTN_WIDTH]+25,y,1.5*m_length[BTN_WIDTH],m_length[BTN_HEIGHT],this,totalcount+1,totalcount+1);
	m_policeTip.Show(false,false);
	m_policeNumEdit.Show(false,false);
	if(type_CHECK == m_choseType)
	{
		m_policeTip.Show(true,false);
		m_policeNumEdit.Show(true,false);
	}

}

unsigned long CShanghaiManualSnapDialog::DoClickChnn( unsigned long index, bool bRefresh )
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

BEGIN_MSG_MAP(CShanghaiManualSnapDialog, GUI::CDialog)
	ON_MSG_CTRL_RANGE(CTRLID_SEARCH_CDN2_CH_0, CTRLID_SEARCH_CDN2_CH_47, KEY_VIRTUAL_ENTER, OnClickChnn)
	ON_MSG_CTRL_RANGE(CTRLID_SEARCH_CDN2_GROUP_0, CTRLID_SEARCH_CDN2_GROUP_11, KEY_VIRTUAL_ENTER, OnClickGroup)
END_MSG_MAP()

unsigned long CShanghaiManualSnapDialog::OnClickChnn( unsigned long ctrlID )
{
	return DoClickChnn( ctrlID - CTRLID_SEARCH_CDN2_CH_0, true );
}

unsigned long CShanghaiManualSnapDialog::OnClickGroup( unsigned long ctrlID )
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

bool CShanghaiManualSnapDialog::OnClickBtnUpload()
{
	ULONGLONG curTime = GetCurrTime();
	ULONGLONG chnnbit = CH();
	if(0 == chnnbit)
	{
		return false;
	}

	if( (0 != chnnbit)&&(0 != curTime ) )
	{
		switch(m_choseType)
		{
		case type_TEST:
			{
				CShanghaiSnapMan::GetInstance()->SetManualParam(chnnbit,curTime,TESTUPLOAD_IMAGE);
				return true;
			}
			break;
		case type_KEEP:
			{
				CShanghaiSnapMan::GetInstance()->SetManualParam(chnnbit,curTime,KEEPUPLOAD_IMAGE);
				return  true;
			}
			break;
		case type_CHECK:
			{
				std::string str = m_policeNumEdit.GetCaption();
				string outstr;
				outstr.clear();
				//编辑框文字需要转码，转为gbk看看
				if ( -1 == m_codeTable->Convert(outstr, CCT_CODE_GBK, str.c_str(),CCT_CODE_UTF8) )
				{
					outstr = str;
				}
				printf("%s,%d,str=%s\n",__FILE__,__LINE__,outstr.c_str());
				CShanghaiSnapMan::GetInstance()->SetCheckPiliceNum(outstr.c_str());
				CShanghaiSnapMan::GetInstance()->SetManualParam(chnnbit,curTime,CHECKUPLOAD_IMAGE);
				return true;
			}
			break;
		default:
			break;
		}
		
	}
	return false;
}

string CShanghaiManualSnapDialog::ChangeChinese(const char *pstr)
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

ULONGLONG CShanghaiManualSnapDialog::CH()
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

