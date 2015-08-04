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

#include "CfgLiveColorDlg.h"
#include "FuncCustom.h"
#include "dvrdvsdef.h"

extern NET_LOGIN_SUCCESS_REPLY *g_p_login_succ;
//////////////////////////////////////////////////////////////////////////
/// Define、Init
//////////////////////////////////////////////////////////////////////////
unsigned short CCfgLiveColorDlg::CFG_DLG_LEFT_X = 10;
unsigned short CCfgLiveColorDlg::CFG_DLG_RIGHT_X = 5;
unsigned short CCfgLiveColorDlg::CFG_DLG_TOP_Y = 15;
unsigned short CCfgLiveColorDlg::CFG_DLG_BOTTOM_Y = 8;
unsigned short CCfgLiveColorDlg::CFG_DLG_OFF_X = 5;
unsigned short CCfgLiveColorDlg::CFG_DLG_OFF_Y = 8;


//////////////////////////////////////////////////////////////////////////
/// 构造函数区
//////////////////////////////////////////////////////////////////////////

CCfgLiveColorDlg::CCfgLiveColorDlg()
{
	m_color = NULL;
	m_pColorChange = NULL;
	m_maxChnn = -1;
	m_curSel = -1;
    m_localVideoNum = false;
	memset(m_lastSliderValue, 0, sizeof(int) * 4);

	m_pMsgMan = CMessageMan::Instance();
}

CCfgLiveColorDlg::~CCfgLiveColorDlg()
{
	if( m_color )
	{
		delete [] m_color;
		m_color = NULL;
	}

	if( m_pColorChange )
	{
		delete [] m_pColorChange;
		m_pColorChange = NULL;
	}
	
	m_maxChnn = -1;
	m_curSel = -1;
	m_outsideFlag=0;

}

//////////////////////////////////////////////////////////////////////////
/// 公共接口函数区
//////////////////////////////////////////////////////////////////////////

unsigned short CCfgLiveColorDlg::GetWidth()
{
	return 200;
}

unsigned short CCfgLiveColorDlg::GetHeight()
{
	// 11 是从CCfgLiveColorDlg::OnInitial中计算出来
	unsigned short height = CFG_DLG_TOP_Y + CFG_DLG_BOTTOM_Y + 10 * m_length[COMBO_HEIGHT] + 10 * CFG_DLG_OFF_Y + 2 * m_length[BTN_HEIGHT];

	return height < m_height ? height : m_height;
}

void CCfgLiveColorDlg::SetChnnInfo(const unsigned long*pColor,int max,int sel)
{
	assert(pColor);
	assert(sel<max);

	if( m_color )
	{
		delete[] m_color;
	}
	
	m_color = new unsigned long [max+1];
	memcpy(m_color,pColor,sizeof( unsigned long )*max);

	m_pColorChange = new unsigned long [max+1];
	memcpy( m_pColorChange, pColor, ( sizeof( unsigned long ) * max ) );

	m_maxChnn = max;
	m_curSel = sel;
}


void CCfgLiveColorDlg::SetChnnInfo(const unsigned long*pColor,int max,int sel,int flag)
{
	assert(pColor);
	assert(sel<max);

	if( m_color )
	{
		delete[] m_color;
	}
	
	m_color = new unsigned long [max+1];
	memcpy(m_color,pColor,sizeof( unsigned long )*max);

	m_pColorChange = new unsigned long [max+1];
	memcpy( m_pColorChange, pColor, ( sizeof( unsigned long ) * max ) );

	m_maxChnn = max;
	if(flag==-2)//toolbar
	{
		if( sel != -2)
		{
			m_curSel = sel;
		}
		else
		{
			m_curSel=max;
		}
		m_outsideFlag=-2;
		
	}
	else if(flag==-3 )//第三套界面
	{
		if(sel!=-3)
		{
			m_curSel=sel;
		}
		else
		{
			m_curSel=max;
		}
		m_outsideFlag=-3;
		printf("3m_curSEl=%d\n",m_curSel);
		printf("sel=%d\n",sel);
	}
}
void CCfgLiveColorDlg::GetChnnColor(unsigned long* pColor,int len)
{
	assert(m_maxChnn==len);
	memcpy(pColor,m_pColorChange,sizeof(unsigned long)*len);
}

//////////////////////////////////////////////////////////////////////////
/// 继承函数区
//////////////////////////////////////////////////////////////////////////

void CCfgLiveColorDlg::OnInitial()
{
	assert(m_color);		//必须先设置SetChnnInfo
	assert(m_maxChnn>0);
	
	m_outModifyAll=false;
	
	GUI::CDialog::OnInitial();
	m_style |= WND_NO_TITLE;
	//////////////////////////////////////////////////////////////////////////

	unsigned short xLeft = CFG_DLG_LEFT_X, yTop = CFG_DLG_TOP_Y;
	unsigned short xOff = CFG_DLG_OFF_X, yOff = CFG_DLG_OFF_Y;
	unsigned short width = GetWidth() - 2 * xLeft;
	unsigned short height = m_length[COMBO_HEIGHT];
	
	GUI::CStatic*		pStc[5] = {&m_stcChnn, &m_stcBright, &m_stcChroma, &m_stcSaturation, &m_stcContrast};
	GUI::CComboCtrl*	pCombo[1] = {&m_comboChnn};
	GUI::CStatic*		pScNum[5] = { NULL,  &m_neBright,  &m_neChroma,  &m_neSaturation,  &m_neContrast};
	GUI::CSliderCtrl*	pSlider[5]	= { NULL,  &m_slBright,  &m_slChroma,  &m_slSaturation,  &m_slContrast};
	GUI::CButton*		pBtn[2]		= { &m_btnRecover, &m_btnOK };
	GUI::CButton*		pBtnAdd[5] = { NULL, &m_btnBrightAdd, &m_btnChromaAdd, &m_btnSaturationAdd, &m_btnContrastAdd};
	GUI::CButton*		pBtnDec[5] = { NULL, &m_btnBrightDec, &m_btnChromaDec, &m_btnSaturationDec, &m_btnContrastDec};
	unsigned long x = xLeft, y = yTop, cx = width, cy = height;

	pStc[0]->Create( GUI::CWnd::GetRandWndID(), x, y, cx - 40, cy, this);
	y += cy + yOff;
	pCombo[0]->Create( GUI::CWnd::GetRandWndID(), x, y, cx, cy, this, 0, 1);
	y += cy;

	x = xLeft + xOff * 2;
	y += yOff * 2;

	unsigned long btnWidth = 30;
	unsigned long btnHeight = 25;

	for ( int i = 1; i < 5; i ++ )
	{
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
	}

	x = xLeft; 
	y += yOff * 2;
	pBtn[0]->Create( GUI::CWnd::GetRandWndID(), x, y, width, m_length[BTN_HEIGHT], this, 0, 6 );
	y += m_length[BTN_HEIGHT] + yOff ;
	pBtn[1]->Create( GUI::CWnd::GetRandWndID(), x, y, width, m_length[BTN_HEIGHT], this, 0, 7 );
	pBtn[1]->SetCaption(m_pStrTable->String(E_STR_ID_FILE),true);


	//// Exit Button
	cx = 35;
	cy = 35;
	x = GetWidth() - cx;
	y = 2;
	m_btnExit.Create( GUI::CWnd::GetRandWndID(), x, y, cx, cy, this, 0, 0 );
	m_btnExit.SetImage( PAINT_STATUS_NORMAL, BMP_BTN_EXIT4, false );
	m_btnExit.SetImage( PAINT_STATUS_FOCUS, BMP_BTN_EXIT4_FOCUS, false );

	m_btnRecover.SetCaption( m_pStrTable->String(E_STR_ID_DEFAULT), false );
	
	//如果是外面进来的，先把按钮改为保存
	if(( -2 == m_outsideFlag )||( -3 == m_outsideFlag ))
	{
		m_btnOK.SetCaption(m_pStrTable->String(E_STR_ID_SAVE),false);
	}
	else
	{
		m_btnOK.SetCaption( m_pStrTable->String(E_STR_ID_OK), false );
	}

	//默认初始化 控件内容 
	SetCtrlData();
	
}

void CCfgLiveColorDlg::OnDestroy()
{
	GUI::CDialog::OnDestroy();
}

void CCfgLiveColorDlg::EndDialog(DLG_END_VALUE result)
{
	if (DLG_OK != result)
	{
		RecoveryChnnColor( m_color, m_pColorChange, m_maxChnn );
	}
	GUI::CDialog::EndDialog(result);
}

//////////////////////////////////////////////////////////////////////////
/// 功能函数区
//////////////////////////////////////////////////////////////////////////

void CCfgLiveColorDlg::SetCtrlData()
{
	char str[128] = {0};

	snprintf( str, 128, "%s", m_pStrTable->String(E_STR_ID_CHANNEL) );
	m_stcChnn.SetCaption(str , false);

	ULONGLONG CH = 0;
	int localVideoInputNum = 0, netVideoInputNum = 0;
	int maxChnn = 0, effectChnn = 0;

	IsSupportCH(CH, &effectChnn, &localVideoInputNum, &netVideoInputNum);

	maxChnn = localVideoInputNum + netVideoInputNum;

	m_localVideoNum = localVideoInputNum;

	int numChnn = m_maxChnn;
	unsigned long color = 0;
	unsigned long startChnn = 0;

	PRODUCT_TYPE product = CProduct::Instance()->ProductType();
	if (TD_2304SS_SDI == product)
	{
		startChnn = 1;
	}
	else if (TD_2308SS_SDI == product)
	{
		startChnn = 2;
	}
	else if (TD_2508HE_SDI == product)
	{
		startChnn = 2;
	}

	assert(numChnn > startChnn);

	if( -2 == m_outsideFlag )//外面调节色彩(toolbar)
	{
		for ( int i = 0; i < numChnn - startChnn; i ++ )
		{
			memset(str,0,sizeof(str));
			if((CH >> i) & 0x1)
			{
				sprintf(str, "%d", i + startChnn + 1);

				m_comboChnn.AddItem( str , i + startChnn);
			}
		}
		m_comboChnn.AddItem(m_pStrTable->String(E_STR_ID_ALL), numChnn);
		
		//假如没错，应该是下面的样子
		if(m_curSel== m_maxChnn)//全通道
		{
			printf("mycursel=%d\n",m_curSel);
			m_comboChnn.SetCurItemData(numChnn, false);
			m_curSel=m_maxChnn;
		}
		else
		{
			printf("mycursel=%d\n",m_curSel);
			m_comboChnn.SetCurItemData(m_curSel, false);
		}
	}
	else if( -3 == m_outsideFlag)//第二套界面右键菜单调色彩
	{
		for ( int i = 0; i < numChnn - startChnn; i ++ )
		{
			if ((CH >> i) & 0x1)
			{
				sprintf(str, "%d", i + startChnn + 1);
				m_comboChnn.AddItem(str, i + startChnn);
			}
		}
		m_comboChnn.AddItem(m_pStrTable->String(E_STR_ID_ALL), numChnn);
		if((m_curSel != m_maxChnn) && ((CH >> m_curSel) & 0x01))
		{
			m_comboChnn.SetCurItemData(m_curSel, false);
		}
		else
		{
			m_comboChnn.SetCurItemData(numChnn, false);
		}
	}
	else
	{
		if( -1==m_curSel )
		{
			//strcpy(str,"All");
			m_comboChnn.AddItem(m_pStrTable->String(E_STR_ID_ALL), -1);
			m_comboChnn.SetCurItem(0, false);
		}
		else
		{
			for ( int i = 0; i < numChnn - startChnn; i ++ )
			{
				if((CH >> i) & 0x1)
				{
					sprintf(str, "%d", i + startChnn + 1);

					m_comboChnn.AddItem(str, i + startChnn);
				}
			}
			m_comboChnn.SetCurItemData( m_curSel, false );
		}

	}

	if( -2 == m_outsideFlag)
	{		
		//只要显示当前的颜色
		if( m_curSel == m_maxChnn)
		{
			printf("color all\n");
			color = ((g_p_login_succ->productInfo.defBrightness << 24) + (g_p_login_succ->productInfo.defContrast << 16) +\
				(g_p_login_succ->productInfo.defSaturation << 8) + g_p_login_succ->productInfo.defHue);

			for (unsigned char i=0; i < m_maxChnn; ++i)
			{
				m_pColorChange [i] = color;
			}
		}
		else
		{
			printf("not color all\n");
			color = m_color[m_curSel];
		}
		
	}
	else if( -3 == m_outsideFlag)
	{
		if( m_curSel != m_maxChnn)
		{
			color = m_color[m_curSel];
		}
		else
		{
			color = ((g_p_login_succ->productInfo.defBrightness << 24) + (g_p_login_succ->productInfo.defContrast << 16) +\
				(g_p_login_succ->productInfo.defSaturation << 8) + g_p_login_succ->productInfo.defHue);

			for (unsigned char i=0; i < m_maxChnn; ++i)
			{
				m_pColorChange [i] = color;
			}
		}
		
	}
	else
	{
		if( -1 == m_curSel)
		{
			//选择所有的时候采用默认值
			color = ((g_p_login_succ->productInfo.defBrightness << 24) + (g_p_login_succ->productInfo.defContrast << 16) +\
				(g_p_login_succ->productInfo.defSaturation << 8) + g_p_login_succ->productInfo.defHue);

			for (unsigned char i=0; i < m_maxChnn; ++i)
			{
				m_pColorChange [i] = color;
			}
		}
		else
		{
			color = m_color[m_curSel];
		}

	}

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

	//如果当前选择的是所有通道，色彩实际值和界面上显示的就有可能有差异，这时候要刷新色彩实际值
	if( -2 == m_outsideFlag)
	{		
		//只要显示当前的颜色
		if( m_curSel == m_maxChnn)
		{
			RecoveryChnnColor(m_pColorChange, m_color, m_maxChnn);
		}
	}
	else if( -3 == m_outsideFlag)
	{
		if( m_curSel == m_maxChnn)
		{
			RecoveryChnnColor( m_pColorChange, m_color, m_maxChnn );
		}
	}
	else
	{
		if( -1 == m_curSel)
		{
			RecoveryChnnColor( m_pColorChange, m_color, m_maxChnn );
		}
	}
}

unsigned long CCfgLiveColorDlg::DoSliderChange(GUI::CSliderCtrl& slider, GUI::CStatic& numSC)
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

void CCfgLiveColorDlg::SetColorData(unsigned long color,GUI::CSliderCtrl& slider, GUI::CStatic& numSC,bool bRefresh)
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
BEGIN_MSG_MAP(CCfgLiveColorDlg, GUI::CDialog)
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

	ON_MSG(m_comboChnn.GetID(),KEY_VIRTUAL_ENTER,OnChnnComboChange)
	ON_MSG(m_btnRecover.GetID(), KEY_VIRTUAL_ENTER, OnBtnRecover)
	ON_MSG(m_btnOK.GetID(), KEY_VIRTUAL_ENTER, OnBtnOK)
	ON_MSG(m_btnExit.GetID(), KEY_VIRTUAL_ENTER, OnBtnExit)
END_MSG_MAP()

unsigned long CCfgLiveColorDlg::OnSliderBrightChange(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	unsigned long retVal = DoSliderChange( m_slBright, m_neBright );
	
	/*if (keyID == KEY_VIRTUAL_ENTER)
	{
		m_sendTime = GetCurrTime();
	}
	else if (KEY_VIRTUAL_DRAG == keyID)
	{
		LONGLONG currentTime = GetCurrTime();
		if (currentTime - m_sendTime < 500000)
		{
			return retVal;
		}
		else
		{
			m_sendTime = currentTime;
		}
	}*/
	
	if ((m_curSel >= m_localVideoNum) && (KEY_VIRTUAL_BLUP != keyID))
	{
		return KEY_VIRTUAL_MSG_STOP;
	}
	
	if(retVal == KEY_VIRTUAL_MSG_STOP)
	{
		ChangeChnnColor();
	}
	
	return  retVal;
}

unsigned long CCfgLiveColorDlg::OnSliderChromaChange(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	unsigned long retVal = DoSliderChange( m_slChroma, m_neChroma );
	
	if ((m_curSel >= m_localVideoNum) && (KEY_VIRTUAL_BLUP != keyID))
	{
		return KEY_VIRTUAL_MSG_STOP;
	}
		
	if(retVal == KEY_VIRTUAL_MSG_STOP)
	{
		ChangeChnnColor();
	}
	return retVal;
}

unsigned long CCfgLiveColorDlg::OnSliderSaturationChange(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	unsigned long retVal = DoSliderChange( m_slSaturation, m_neSaturation );

	if ((m_curSel >= m_localVideoNum) && (KEY_VIRTUAL_BLUP != keyID))
	{
		return KEY_VIRTUAL_MSG_STOP;
	}
		
	if(retVal == KEY_VIRTUAL_MSG_STOP)
	{
		ChangeChnnColor();
	}
	return retVal;
}

unsigned long CCfgLiveColorDlg::OnSliderContrastChange(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	unsigned long retVal = DoSliderChange( m_slContrast, m_neContrast );

	if ((m_curSel >= m_localVideoNum) && (KEY_VIRTUAL_BLUP != keyID))
	{
		return KEY_VIRTUAL_MSG_STOP;
	}
	
	if(retVal == KEY_VIRTUAL_MSG_STOP)
	{
		ChangeChnnColor();
	}
	return retVal;
}

unsigned long CCfgLiveColorDlg::OnBtnBrightAdd()
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

unsigned long CCfgLiveColorDlg::OnBtnChromaAdd()
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

unsigned long CCfgLiveColorDlg::OnBtnSaturationAdd()
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

unsigned long CCfgLiveColorDlg::OnBtnContrastAdd()
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

unsigned long CCfgLiveColorDlg::OnBtnBrightDec()
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

unsigned long CCfgLiveColorDlg::OnBtnChromaDec()
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

unsigned long CCfgLiveColorDlg::OnBtnSaturationDec()
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

unsigned long CCfgLiveColorDlg::OnBtnContrastDec()
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

void CCfgLiveColorDlg::SelectCh(unsigned long color)//切换通道后颜色和显示模式的更改
{
	NET_DISPLAY_INFO displayInfo;
	memset(&displayInfo, 0, sizeof(NET_DISPLAY_INFO));
	displayInfo.type		= NET_DISPLAY_CHANGE_DATA_PACK;
	displayInfo.time_out	= -1;
	displayInfo.rec			= 0;

	ULONGLONG CH = 0;
	int localVideoInputNum = 0, netVideoInputNum = 0;
	int maxChnn = 0, effectChnn = 0;

	IsSupportCH(CH, &effectChnn, &localVideoInputNum, &netVideoInputNum);

	maxChnn = IsSupportHybridDisplay()?(localVideoInputNum + netVideoInputNum):localVideoInputNum;

	if(m_curSel == m_maxChnn)//全通道
	{
		unsigned long splitMode = VIEW_SPLIT_1X1;
		if ( 4 == maxChnn )
		{
			splitMode= VIEW_SPLIT_2X2;
		}
		else if ( 8 == maxChnn )
		{
			splitMode= VIEW_SPLIT_3X3;
		}
		else if (5 == maxChnn)
		{
			splitMode= VIEW_SPLIT_2X3;
		}
		else if (9 == maxChnn)
		{
			splitMode= VIEW_SPLIT_3X3;
		}
		else if ((12 == maxChnn) || (16 == maxChnn))
		{
			splitMode= VIEW_SPLIT_4X4;
		}
		else if (( 16 < maxChnn ) && ( maxChnn <= 25))
		{
			splitMode= VIEW_SPLIT_5X5;
		}
		else
		{
			splitMode= VIEW_SPLIT_6X6;
		}

		displayInfo.split_mode[DISPLAY_LAYER_ANALOG] = splitMode;
		displayInfo.split_mode[DISPLAY_LAYER_NET] = splitMode;
		for(int i = 0; i < maxChnn; i++)
		{
			if(i < localVideoInputNum)
			{
				displayInfo.data[i] = DISPLAY_PACK(DISPLAY_LAYER_ANALOG, DISPLAY_SIGNAL_ANALOG, i, i);
			}
			else
			{
				displayInfo.data[i] = DISPLAY_PACK(DISPLAY_LAYER_NET, DISPLAY_SIGNAL_NET, i, i - localVideoInputNum);
			}
		}
		displayInfo.data_len = maxChnn;
	}
	else//单通道
	{
		displayInfo.split_mode[DISPLAY_LAYER_ANALOG] = VIEW_SPLIT_1X1;
		displayInfo.split_mode[DISPLAY_LAYER_NET] = VIEW_SPLIT_1X1;
		
		if(m_curSel < localVideoInputNum)
		{
			displayInfo.data[0] = DISPLAY_PACK(DISPLAY_LAYER_ANALOG, DISPLAY_SIGNAL_ANALOG, 0, m_curSel);
		}
		else
		{
			displayInfo.data[0] = DISPLAY_PACK(DISPLAY_LAYER_NET, DISPLAY_SIGNAL_NET, 0, m_curSel - localVideoInputNum);
		}

		displayInfo.data_len = 1;
	}

	//设置颜色值
	SetColorData( ( ( color >> 24 ) & 0xFF ), m_slBright, m_neBright, false );
	SetColorData( ( ( color >> 16 ) & 0xFF ), m_slContrast, m_neContrast, false );
	SetColorData( ( ( color >> 8  ) & 0xff ), m_slSaturation, m_neSaturation, false );
	SetColorData( ( ( color >> 0  ) & 0xFF ), m_slChroma, m_neChroma, false );

	CMessageMan* pMsgMan = CMessageMan::Instance();

	//发送修改显示模式
	MESSAGE_DATA messageData;
	memset(&messageData, 0, sizeof(MESSAGE_DATA));
	pMsgMan->SendMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_CHANGE_DISPLAY_MODE, (unsigned char *)&displayInfo, sizeof(NET_DISPLAY_INFO), messageData );
	pMsgMan->ReleaseMsgBuff(messageData);

	GetRootWnd()->Repaint();
}

unsigned long CCfgLiveColorDlg::OnChnnComboChange()
{
	//保存当前页面值
	int sel = m_comboChnn.GetCurItemData();
	unsigned long color=0;

	if(( -2 ==m_outsideFlag )||( -3 ==m_outsideFlag))// 外面进来的
	{
		if( sel ==m_maxChnn)//最大通道号，如果没有修改all,不做改变
			{
				if( !m_outModifyAll)
				{
					color = ((g_p_login_succ->productInfo.defBrightness << 24) + (g_p_login_succ->productInfo.defContrast << 16) +\
							(g_p_login_succ->productInfo.defSaturation << 8) + g_p_login_succ->productInfo.defHue);
					m_curSel =m_maxChnn;
				}
				else
				{	
					color=m_pColorChange[m_maxChnn];
					m_curSel =m_maxChnn;
				}
			}
		else//单通道画面
			{
				m_curSel = sel;
				color=m_pColorChange[m_curSel];
			}
	}
	else//不是外面进来的
	{
		if( -1 == m_curSel )
		{
			//啥也不做
			return KEY_VIRTUAL_MSG_STOP;
		}
		else
		{
			m_curSel = sel;
			color=m_pColorChange[m_curSel];
		}
	}
	SelectCh(color);
	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CCfgLiveColorDlg::OnBtnRecover()
{
	m_slBright.SetCurPos( g_p_login_succ->productInfo.defBrightness, true );
	DoSliderChange( m_slBright, m_neBright );
	m_slChroma.SetCurPos( g_p_login_succ->productInfo.defHue, true );
	DoSliderChange( m_slChroma, m_neChroma );
	m_slContrast.SetCurPos( g_p_login_succ->productInfo.defContrast, true );
	DoSliderChange( m_slContrast, m_neContrast );
	m_slSaturation.SetCurPos( g_p_login_succ->productInfo.defSaturation, true );
	DoSliderChange( m_slSaturation, m_neSaturation );


	unsigned long color = ((g_p_login_succ->productInfo.defBrightness << 24) + (g_p_login_succ->productInfo.defContrast << 16) +\
		(g_p_login_succ->productInfo.defSaturation << 8) + g_p_login_succ->productInfo.defHue);
	if( -2 == m_outsideFlag)
	{
		int sel = m_comboChnn.GetCurItemData();
		if( m_maxChnn == sel)
		{
			for (unsigned char i=0; i<m_maxChnn; ++i)
			{
				m_pColorChange [i] = color;
			}
		}
		else
		{
			assert ((0 <= sel) && (sel < m_maxChnn));
			m_pColorChange[sel] = color;
		}
	}
	else
	{
		if( -1 == m_curSel )
		{
			for (unsigned char i=0; i<m_maxChnn; ++i)
			{
				m_pColorChange [i] = color;
			}
		}
		else
		{
			assert ((0 <= m_curSel) && (m_curSel < m_maxChnn));

			m_pColorChange[m_curSel] = color;
		}
	}

	ChangeChnnColor();

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CCfgLiveColorDlg::OnBtnOK()
{
	if(( -2 ==m_outsideFlag )||( -3 ==m_outsideFlag ))
	{
		SaveLiveColor();
	}
	else
	{
		return OnOK();
	}
	
	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CCfgLiveColorDlg::OnBtnExit()
{
	//回复改变的通道color
	//RecoveryChnnColor();
	return OnCancel();
}



//在界面拖动slider的时候临时改变现场通道颜色
void CCfgLiveColorDlg::ChangeChnnColor()
{
	assert( NULL != m_color );
	assert( NULL != m_pColorChange );

	NET_CHNN_COLOR chnnColor;
	memset( &chnnColor, 0, sizeof( NET_CHNN_COLOR ) );
	
	unsigned long bright=m_slBright.GetPos();
	unsigned long chroma=m_slChroma.GetPos();
	unsigned long saturation=m_slSaturation.GetPos();
	unsigned long contrast=m_slContrast.GetPos();
	unsigned long color = (bright<<24) | (contrast<<16) | (saturation<<8) | (chroma);
	
	if( ( -2 == m_outsideFlag) || ( -3 == m_outsideFlag) )//toolbar或者右键菜单
	{
		if( m_curSel == m_maxChnn )//调节all的时候，其他都要改变
		{
			unsigned long *pColorIndex = m_pColorChange;
			//所有通道的颜色都改变
			for( int i=0; i<=m_maxChnn; i++, pColorIndex++ )
			{
				*pColorIndex = color;
			}
			m_pColorChange[m_maxChnn]=color;
			m_outModifyAll=true;
		}
		else//不是all
		{
			m_pColorChange[m_curSel] = color;
		}
	}
	else//live对话框调色彩
	{
		if( -1 == m_curSel )
		{
			unsigned long *pColorIndex = m_pColorChange;
			//所有通道的颜色都改变
			for( int i=0; i<m_maxChnn; i++, pColorIndex++ )
			{
				*pColorIndex = color;
			}
		}
		else
		{
			//只改变某个通道
			m_pColorChange[m_curSel] = color;
		}
	}
	
	if(( 0 <= m_curSel )&&( m_curSel < m_maxChnn ))
	{
		chnnColor.chnn	= m_curSel;
	}
	else if( -1 == m_curSel )
	{
		//用通道的最大值来代表所有通道
		chnnColor.chnn	= m_maxChnn;
	}
	else if(( -2 == m_outsideFlag)&&(m_curSel == m_maxChnn))
	{
		chnnColor.chnn	= m_maxChnn;
	}
	else if(( -3 == m_outsideFlag)&&(m_curSel == m_maxChnn))
	{
		chnnColor.chnn	= m_maxChnn;
	}
	else
	{
		assert(0);
	}
	
	//设置通道号
	chnnColor.hide = 0;
	chnnColor.color = color;
	CMessageMan::Instance()->PostMsgDataToServer( LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_COLOR_SET, ( unsigned char * )&chnnColor, sizeof( NET_CHNN_COLOR ) );
	
}

void CCfgLiveColorDlg::SaveLiveColor()
{
	//点击ok的时候调用
	//保存所有m_pColorChange的值
	NET_CHNN_COLOR chnnColor;
	memset( &chnnColor, 0, sizeof( NET_CHNN_COLOR ) );
	MESSAGE_DATA msgData;

	CMessageMan* pMsgMan = CMessageMan::Instance();

	if(( m_curSel >= 0)&&( m_curSel <m_maxChnn))
	{
		chnnColor.hide=0;
		chnnColor.chnn=m_curSel;
		chnnColor.color=m_pColorChange[m_curSel];

		pMsgMan->PostMsgDataToServer( LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_COLOR_SET, ( unsigned char * )&chnnColor, sizeof( NET_CHNN_COLOR ) );
		pMsgMan->SendMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_COLOR_SAVE,(unsigned char *)&chnnColor, sizeof(NET_CHNN_COLOR), msgData);
		pMsgMan->ReleaseMsgBuff( msgData );

		m_color[m_curSel]=chnnColor.color;
		
	}
	else if(m_curSel==m_maxChnn) //全通道
	{
		//获得当前通道的值作为全通道值
		unsigned long bright=m_slBright.GetPos();
		unsigned long chroma=m_slChroma.GetPos();
		unsigned long saturation=m_slSaturation.GetPos();
		unsigned long contrast=m_slContrast.GetPos();
		unsigned long color = (bright<<24) | (contrast<<16) | (saturation<<8) | (chroma);

		chnnColor.hide=0;
		chnnColor.chnn=-1;//保存全通道的配置
		chnnColor.color=color;
		pMsgMan->PostMsgDataToServer( LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_COLOR_SET, ( unsigned char * )&chnnColor, sizeof( NET_CHNN_COLOR ) );
		pMsgMan->SendMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_COLOR_SAVE,(unsigned char *)&chnnColor, sizeof(NET_CHNN_COLOR), msgData);
		pMsgMan->ReleaseMsgBuff( msgData );
		for(int i=0; i<m_maxChnn; i++)
		{
			m_color[i]=chnnColor.color;
		}

	}
	GetRootWnd()->Repaint();

}

//当新配置的各通道颜色信息没有保存m_color中的时候要回复通道颜色到进入该界面的时候
void CCfgLiveColorDlg::RecoveryChnnColor( const unsigned long *pClrChnn, const unsigned long *pClrChnnChng, int maxChnnNum )
{
	assert( NULL != pClrChnn );
	assert( NULL != pClrChnnChng );
	if( ( -2 == m_outsideFlag) || ( -3 == m_outsideFlag) )
	{
		if( 0 == ( memcmp( pClrChnn, pClrChnnChng , ( sizeof( unsigned long ) * maxChnnNum ) ) ) )
		{
			return;
		}

		NET_CHNN_COLOR chnnColor;
		memset( &chnnColor, 0, sizeof( NET_CHNN_COLOR ) );	

		for( int i=0; i<maxChnnNum; i++ )
		{
			if( *pClrChnn != *pClrChnnChng )
			{
				chnnColor.chnn = i;
				chnnColor.hide = 0;
				chnnColor.color = *pClrChnn;
				CMessageMan::Instance()->PostMsgDataToServer( LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_COLOR_SET, ( unsigned char * )&chnnColor, sizeof( NET_CHNN_COLOR ) );
			}
			pClrChnn++;
			pClrChnnChng++;
		}
	}else{
		NET_CHNN_COLOR chnnColor;
		memset( &chnnColor, 0, sizeof( NET_CHNN_COLOR ) );	
		for( int i=0; i<maxChnnNum; i++ )
		{
			if( *pClrChnn != *pClrChnnChng )
			{
				chnnColor.chnn = i;
				chnnColor.hide = 0;
				chnnColor.color = *pClrChnn;
				CMessageMan::Instance()->PostMsgDataToServer( LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_COLOR_SET, ( unsigned char * )&chnnColor, sizeof( NET_CHNN_COLOR ) );
			}

			pClrChnn++;
			pClrChnnChng++;
		}
	}
	
}

