/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : l
** Date         : 2009-08-13
** Name         : CfgLiveColorDlg.h
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/

#include "CfgDevColorDlg.h"

extern NET_LOGIN_SUCCESS_REPLY *g_p_login_succ;
//////////////////////////////////////////////////////////////////////////
/// Define、Init
//////////////////////////////////////////////////////////////////////////
unsigned short CCfgDevColorDlg::CFG_DLG_LEFT_X = 10;
unsigned short CCfgDevColorDlg::CFG_DLG_RIGHT_X = 5;
unsigned short CCfgDevColorDlg::CFG_DLG_TOP_Y = 10;
unsigned short CCfgDevColorDlg::CFG_DLG_BOTTOM_Y = 8;
unsigned short CCfgDevColorDlg::CFG_DLG_OFF_X = 5;
unsigned short CCfgDevColorDlg::CFG_DLG_OFF_Y = 8;


//////////////////////////////////////////////////////////////////////////
/// 构造函数区
//////////////////////////////////////////////////////////////////////////

CCfgDevColorDlg::CCfgDevColorDlg()
{
	
	memset(m_lastSliderValue, 0, sizeof(int) * 4);

	m_pMsgMan = CMessageMan::Instance();
}

CCfgDevColorDlg::~CCfgDevColorDlg()
{

}

//////////////////////////////////////////////////////////////////////////
/// 公共接口函数区
//////////////////////////////////////////////////////////////////////////

unsigned short CCfgDevColorDlg::GetWidth()
{
	return 200;
}

unsigned short CCfgDevColorDlg::GetHeight()
{
	// 11 是从CCfgLiveColorDlg::OnInitial中计算出来
	unsigned short height = CFG_DLG_TOP_Y + CFG_DLG_BOTTOM_Y + 8 * m_length[COMBO_HEIGHT] + 8 * CFG_DLG_OFF_Y + m_length[BTN_HEIGHT];

	return height < m_height ? height : m_height;
}

void CCfgDevColorDlg::SetDevColorInfo(unsigned long devColor)
{
	m_color = devColor;

}

//////////////////////////////////////////////////////////////////////////
/// 继承函数区
//////////////////////////////////////////////////////////////////////////

void CCfgDevColorDlg::OnInitial()
{
	GUI::CDialog::OnInitial();
	m_style |= WND_NO_TITLE;
	//////////////////////////////////////////////////////////////////////////

	unsigned short xLeft = CFG_DLG_LEFT_X, yTop = CFG_DLG_TOP_Y;
	unsigned short xOff = CFG_DLG_OFF_X, yOff = CFG_DLG_OFF_Y;
	unsigned short width = GetWidth() - 2 * xLeft;
	unsigned short height = m_length[COMBO_HEIGHT];
	
	GUI::CStatic*		pStc[4]     = {&m_stcBright, &m_stcChroma, &m_stcSaturation, &m_stcContrast};
	GUI::CStatic*		pScNum[4]   = {&m_neBright,  &m_neChroma,  &m_neSaturation,  &m_neContrast};
	GUI::CSliderCtrl*	pSlider[4]	= {&m_slBright,  &m_slChroma,  &m_slSaturation,  &m_slContrast};
	GUI::CButton*		pBtn[1]		= {&m_btnRecover};
	GUI::CButton*		pBtnAdd[4]  = {&m_btnBrightAdd, &m_btnChromaAdd, &m_btnSaturationAdd, &m_btnContrastAdd};
	GUI::CButton*		pBtnDec[4]  = {&m_btnBrightDec, &m_btnChromaDec, &m_btnSaturationDec, &m_btnContrastDec};
	unsigned long x = xLeft, y = yTop, cx = width, cy = height;
	
	x = xLeft + xOff * 2;
	y += yOff * 2;

#ifdef __TDFH__
	unsigned long yf = (cy + yOff) / 2;
#endif
	unsigned long btnWidth = 30;
	unsigned long btnHeight = 25;

	for ( int i = 0; i < 4; i ++ )
	{

#ifdef __TDFH__
		//FH平台回放时不调色度
		if (i == 1)
		{
			continue;
		}
#endif
		x = xLeft + xOff;
		pStc[i]->Create( GUI::CWnd::GetRandWndID(), x, y+4, cx - 2 * xOff- 60, cy, this);
		x += (cx - 2 * xOff - 60) ;
		pScNum[i]->Create( GUI::CWnd::GetRandWndID(), x, y+4, 60 , cy, this);
		pScNum[i]->SetBKColor(COLOR_DLG_CHILD_BK);
		pScNum[i]->SetAlignText( ALIGN_TEXT_RIGHT );

		x = xLeft + xOff;
		y += cy + yOff;

		pBtnDec[i]->Create( GetRandWndID(), x, y-3, btnWidth, btnHeight, this, 0, i+1);
		pBtnDec[i]->SetImage(PAINT_STATUS_NORMAL, BMP_BTN_DEC, false);
		pBtnDec[i]->SetImage(PAINT_STATUS_FOCUS, BMP_BTN_DEC_FOCUS, false);

		x += btnWidth + 2;

		pSlider[i]->Create( GetRandWndID(), x, y, cx - 2 * xOff - 4 - 2 * btnWidth, this, 1, i+1, true );
		
		x = xLeft + cx - btnWidth - xOff;

		pBtnAdd[i]->Create(GetRandWndID(), x, y-3, btnWidth, btnHeight, this, 2, i+1);
		pBtnAdd[i]->SetImage(PAINT_STATUS_NORMAL, BMP_BTN_ADD, false);
		pBtnAdd[i]->SetImage(PAINT_STATUS_FOCUS, BMP_BTN_ADD_FOCUS, false);

		y += cy;
#ifdef __TDFH__
		y += yf;
#endif
	}

	x = xLeft; 
	y += yOff * 2;	
	pBtn[0]->Create( GUI::CWnd::GetRandWndID(), x, y, width, m_length[BTN_HEIGHT], this, 0, 7 );
	pBtn[0]->SetCaption(m_pStrTable->String(E_STR_ID_DEFAULT), false);


	//// Exit Button
	cx = 35;
	cy = 35;
	x = GetWidth() - cx;
	y = 2;
	m_btnExit.Create( GUI::CWnd::GetRandWndID(), x, y, cx, cy, this, 0, 0 );
	m_btnExit.SetImage( PAINT_STATUS_NORMAL, BMP_BTN_EXIT4, false );
	m_btnExit.SetImage( PAINT_STATUS_FOCUS, BMP_BTN_EXIT4_FOCUS, false );

	//默认初始化 控件内容 
	SetCtrlData();
	
}

void CCfgDevColorDlg::OnDestroy()
{
	GUI::CDialog::OnDestroy();
}

void CCfgDevColorDlg::EndDialog(DLG_END_VALUE result)
{	
	GUI::CDialog::EndDialog(result);
}

//////////////////////////////////////////////////////////////////////////
/// 功能函数区
//////////////////////////////////////////////////////////////////////////

void CCfgDevColorDlg::SetCtrlData()
{
	char str[128] = {0};

    unsigned long color = m_color;

	m_stcBright.SetCaption( m_pStrTable->String(E_STR_ID_BRIGHTNESS), false );
	m_stcChroma.SetCaption( m_pStrTable->String(E_STR_ID_HUE), false );
	m_stcSaturation.SetCaption( m_pStrTable->String(E_STR_ID_SATURATION), false );
	m_stcContrast.SetCaption( m_pStrTable->String(E_STR_ID_CONTRAST), false );

	m_slBright.SetRange( 0 , 255 );
	m_slChroma.SetRange( 0 , 255 );
	m_slSaturation.SetRange( 0 , 255 );
	m_slContrast.SetRange( 0 , 255 );


	SetColorData( ( ( color >> 24 ) & 0xff ), m_slBright, m_neBright, false );
	SetColorData( ( ( color >> 16 ) & 0xff ), m_slContrast, m_neContrast, false );
	SetColorData( ( ( color >>  8 ) & 0xff ), m_slSaturation, m_neSaturation, false );
	SetColorData( ( ( color >>  0 ) & 0xff ), m_slChroma, m_neChroma, false );
}

unsigned long CCfgDevColorDlg::DoSliderChange(GUI::CSliderCtrl& slider, GUI::CStatic& numSC)
{
	//检查防止调黑
	bool bPassCheck = true;
	int sliderValue[4] = {0};
	sliderValue[0] = m_slBright.GetPos();		//亮度
	sliderValue[1] = m_slChroma.GetPos();		//色度
	sliderValue[2] = m_slSaturation.GetPos();	//饱和度
	sliderValue[3] = m_slContrast.GetPos();		//对比度

	if((sliderValue[3] < 20) || ((sliderValue[0] * sliderValue[0] + sliderValue[2] * 8 + sliderValue[3] * sliderValue[3] * 2) < 10000))
	{
		//从试验中推算出的大致条件,并不准确
		bPassCheck = false;
	}

	unsigned long id = slider.GetID();
	if(id == m_slBright.GetID())
	{
		if(bPassCheck)
		{
			m_lastSliderValue[0] = slider.GetPos();
		}
		else
		{
			slider.SetCurPos(m_lastSliderValue[0], true);
		}
	}
	else if(id == m_slChroma.GetID())
	{
		if(bPassCheck)
		{
			m_lastSliderValue[1] = slider.GetPos();
		}
		else
		{
			slider.SetCurPos(m_lastSliderValue[1], true);
		}
	}
	else if(id == m_slSaturation.GetID())
	{
		if(bPassCheck)
		{
			m_lastSliderValue[2] = slider.GetPos();
		}
		else
		{
			slider.SetCurPos(m_lastSliderValue[2], true);
		}
	}
	else if(id == m_slContrast.GetID())
	{
		if(bPassCheck)
		{
			m_lastSliderValue[3] = slider.GetPos();
		}
		else
		{
			slider.SetCurPos(m_lastSliderValue[3], true);
		}
	}
	else
	{
		assert(false);
	}

	int pos = slider.GetPos();
	char szValue[32] = { 0 };
	sprintf(szValue, "%d", pos);
	numSC.SetCaption(szValue, true);

	if (!bPassCheck)
	{
		return KEY_VIRTUAL_MSG_CONTINUE;//滑块移动到两头，交由上层处理
	} 
	else
	{
		return KEY_VIRTUAL_MSG_STOP;
	}
}

void CCfgDevColorDlg::SetColorData(unsigned long color,GUI::CSliderCtrl& slider, GUI::CStatic& numSC,bool bRefresh)
{
	char str[32]={0};
	assert(color <= 255);//值[0~255]单颜色值
	slider.SetCurPos(color,bRefresh);

	sprintf(str,"%d",color);
	numSC.SetCaption(str,bRefresh);

	unsigned long id = slider.GetID();
	if(id == m_slBright.GetID())
	{
		m_lastSliderValue[0] = slider.GetPos();
	}
	else if(id == m_slChroma.GetID())
	{
		m_lastSliderValue[1] = slider.GetPos();
	}
	else if(id == m_slSaturation.GetID())
	{
		m_lastSliderValue[2] = slider.GetPos();
	}
	else if(id == m_slContrast.GetID())
	{
		m_lastSliderValue[3] = slider.GetPos();
	}
	else
	{
		assert(false);
	}
}

//////////////////////////////////////////////////////////////////////////
/// 消息响应区
//////////////////////////////////////////////////////////////////////////
BEGIN_MSG_MAP(CCfgDevColorDlg, GUI::CDialog)
	ON_MSG_EX(m_slBright.GetID(), KEY_VIRTUAL_ENTER, OnSliderBrightChange)
	ON_MSG_EX(m_slBright.GetID(), KEY_VIRTUAL_DRAG, OnSliderBrightChange)
	ON_MSG_EX(m_slBright.GetID(), KEY_VIRTUAL_BLUP, OnSliderBrightChange)
	ON_MSG_EX(m_slChroma.GetID(), KEY_VIRTUAL_ENTER, OnSliderChromaChange)
	ON_MSG_EX(m_slChroma.GetID(), KEY_VIRTUAL_DRAG, OnSliderChromaChange)
	ON_MSG_EX(m_slChroma.GetID(), KEY_VIRTUAL_BLUP, OnSliderChromaChange)
	ON_MSG_EX(m_slSaturation.GetID(), KEY_VIRTUAL_ENTER, OnSliderSaturationChange)
	ON_MSG_EX(m_slSaturation.GetID(), KEY_VIRTUAL_DRAG, OnSliderSaturationChange)
	ON_MSG_EX(m_slSaturation.GetID(), KEY_VIRTUAL_BLUP, OnSliderSaturationChange)
	ON_MSG_EX(m_slContrast.GetID(), KEY_VIRTUAL_ENTER, OnSliderContrastChange)
	ON_MSG_EX(m_slContrast.GetID(), KEY_VIRTUAL_DRAG, OnSliderContrastChange)
	ON_MSG_EX(m_slContrast.GetID(), KEY_VIRTUAL_BLUP, OnSliderContrastChange)

	ON_MSG(m_btnBrightAdd.GetID(), KEY_VIRTUAL_ENTER, OnBtnBrightAdd)
	ON_MSG(m_btnBrightDec.GetID(), KEY_VIRTUAL_ENTER, OnBtnBrightDec)
	ON_MSG(m_btnChromaAdd.GetID(), KEY_VIRTUAL_ENTER, OnBtnChromaAdd)
	ON_MSG(m_btnChromaDec.GetID(), KEY_VIRTUAL_ENTER, OnBtnChromaDec)
	ON_MSG(m_btnSaturationAdd.GetID(), KEY_VIRTUAL_ENTER, OnBtnSaturationAdd)
	ON_MSG(m_btnSaturationDec.GetID(), KEY_VIRTUAL_ENTER, OnBtnSaturationDec)
	ON_MSG(m_btnContrastAdd.GetID(), KEY_VIRTUAL_ENTER, OnBtnContrastAdd)
	ON_MSG(m_btnContrastDec.GetID(), KEY_VIRTUAL_ENTER, OnBtnContrastDec)

	ON_MSG(m_btnExit.GetID(), KEY_VIRTUAL_ENTER, OnBtnExit)
	ON_MSG(m_btnRecover.GetID(), KEY_VIRTUAL_ENTER, OnBtnRecover)
END_MSG_MAP()

unsigned long CCfgDevColorDlg::OnSliderBrightChange(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	unsigned long retVal = DoSliderChange( m_slBright, m_neBright );
	
	if(retVal == KEY_VIRTUAL_MSG_STOP)
	{
		ChangeChnnColor();
	}
	return  retVal;
}

unsigned long CCfgDevColorDlg::OnSliderChromaChange(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	unsigned long retVal = DoSliderChange( m_slChroma, m_neChroma );
	
	if(retVal == KEY_VIRTUAL_MSG_STOP)
	{
		ChangeChnnColor();
	}
	return retVal;
}

unsigned long CCfgDevColorDlg::OnSliderSaturationChange(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	unsigned long retVal = DoSliderChange( m_slSaturation, m_neSaturation );
	
	if(retVal == KEY_VIRTUAL_MSG_STOP)
	{
		ChangeChnnColor();
	}
	return retVal;
}

unsigned long CCfgDevColorDlg::OnSliderContrastChange(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	unsigned long retVal = DoSliderChange( m_slContrast, m_neContrast );

	if(retVal == KEY_VIRTUAL_MSG_STOP)
	{
		ChangeChnnColor();
	}
	return retVal;
}

unsigned long CCfgDevColorDlg::OnBtnBrightAdd()
{
	int max = m_slBright.GetRangeMax();
	int value = m_slBright.GetPos();
	if( value + 1 <= max )
	{
		value += 1;
		m_slBright.SetCurPos(value, true);
		if(KEY_VIRTUAL_MSG_STOP == DoSliderChange(m_slBright, m_neBright))
		{
			ChangeChnnColor();
		}
	}

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CCfgDevColorDlg::OnBtnChromaAdd()
{
	int max = m_slChroma.GetRangeMax();
	int value = m_slChroma.GetPos();
	if( value + 1 <= max )
	{
		value += 1;
		m_slChroma.SetCurPos(value, true);
		if(KEY_VIRTUAL_MSG_STOP == DoSliderChange(m_slChroma, m_neChroma))
		{
			ChangeChnnColor();
		}
	}

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CCfgDevColorDlg::OnBtnSaturationAdd()
{
	int max = m_slSaturation.GetRangeMax();
	int value = m_slSaturation.GetPos();
	if( value + 1 <= max )
	{
		value += 1;
		m_slSaturation.SetCurPos(value, true);
		if(KEY_VIRTUAL_MSG_STOP == DoSliderChange(m_slSaturation, m_neSaturation))
		{
			ChangeChnnColor();
		}
	}

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CCfgDevColorDlg::OnBtnContrastAdd()
{
	int max = m_slContrast.GetRangeMax();
	int value = m_slContrast.GetPos();
	if( value + 1 <= max )
	{
		value += 1;
		m_slContrast.SetCurPos(value, true);
		if(KEY_VIRTUAL_MSG_STOP == DoSliderChange(m_slContrast, m_neContrast))
		{
			ChangeChnnColor();
		}
	}

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CCfgDevColorDlg::OnBtnBrightDec()
{
	int min = m_slBright.GetRangeMin();
	int value = m_slBright.GetPos();
	if( value - 1 >= min )
	{
		value -= 1;
		m_slBright.SetCurPos(value, true);
		if(KEY_VIRTUAL_MSG_STOP == DoSliderChange(m_slBright, m_neBright))
		{
			ChangeChnnColor();
		}
	}

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CCfgDevColorDlg::OnBtnChromaDec()
{
	int min = m_slChroma.GetRangeMin();
	int value = m_slChroma.GetPos();
	if( value - 1 >= min )
	{
		value -= 1;
		m_slChroma.SetCurPos(value, true);
		if(KEY_VIRTUAL_MSG_STOP == DoSliderChange(m_slChroma, m_neChroma))
		{
			ChangeChnnColor();
		}
	}

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CCfgDevColorDlg::OnBtnSaturationDec()
{
	int min = m_slSaturation.GetRangeMin();
	int value = m_slSaturation.GetPos();
	if( value - 1 >= min )
	{
		value -= 1;
		m_slSaturation.SetCurPos(value, true);
		if(KEY_VIRTUAL_MSG_STOP == DoSliderChange(m_slSaturation, m_neSaturation))
		{
			ChangeChnnColor();
		}
	}

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CCfgDevColorDlg::OnBtnContrastDec()
{
	int min = m_slContrast.GetRangeMin();
	int value = m_slContrast.GetPos();
	if( value - 1 >= min )
	{
		value -= 1;
		m_slContrast.SetCurPos(value, true);
		if(KEY_VIRTUAL_MSG_STOP == DoSliderChange(m_slContrast, m_neContrast))
		{
			ChangeChnnColor();
		}
	}

	return KEY_VIRTUAL_MSG_STOP;
}


unsigned long CCfgDevColorDlg::OnBtnRecover()
{
	MESSAGE_DATA msgData;
	memset(&msgData, 0, sizeof(MESSAGE_DATA));
	CMessageMan::Instance()->SendMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_DEV_DEFA_COLOR, NULL, 0, msgData);
	unsigned long color = *reinterpret_cast<unsigned long *>(msgData.pData + sizeof(PACKCMD));
	CMessageMan::Instance()->ReleaseMsgBuff(msgData);

	m_slBright.SetCurPos( ((color & 0xff000000) >> 24) , true );
	DoSliderChange( m_slBright, m_neBright );
	m_slChroma.SetCurPos( ((color & 0x000000ff) >> 0), true );
	DoSliderChange( m_slChroma, m_neChroma );
	m_slContrast.SetCurPos( ((color & 0x00ff0000) >> 16), true );
	DoSliderChange( m_slContrast, m_neContrast );
	m_slSaturation.SetCurPos( ((color & 0x0000ff00) >> 8), true );
	DoSliderChange( m_slSaturation, m_neSaturation );


	ChangeChnnColor();

	return KEY_VIRTUAL_MSG_STOP;
}


unsigned long CCfgDevColorDlg::OnBtnExit()
{	
	return OnCancel();
}
 

//在界面拖动slider的时候临时改变现场通道颜色
void CCfgDevColorDlg::ChangeChnnColor()
{
	
	unsigned long bright=m_slBright.GetPos();
	unsigned long chroma=m_slChroma.GetPos();
	unsigned long saturation=m_slSaturation.GetPos();
	unsigned long contrast=m_slContrast.GetPos();
	unsigned long color = (bright<<24) | (contrast<<16) | (saturation<<8) | (chroma);
		
	m_pMsgMan->PostMsgDataToServer( LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_DEV_COLOR_SET, ( unsigned char * )&color, sizeof( unsigned long ) );
	
}


