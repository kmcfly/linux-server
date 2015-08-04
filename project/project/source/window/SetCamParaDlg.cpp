/*
* Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
* Author       : xian shi wei
* Date         : 2014-10-27
* Name         : SetCamParaDlg.cpp
* Version      : 1.0
* Description  : 
* Modify Record:
*/

#include "MessageMan.h"
#include "SetCamParaDlg.h"
#include "FuncCustom.h"
#include "StringTable.h"

extern NET_LOGIN_SUCCESS_REPLY * g_p_login_succ;

unsigned long CSetCamParaDlg::lRotato[4] = {0,90,180,270};

CSetCamParaDlg::CSetCamParaDlg(){
	m_curSel = -1;
	m_localChnn =-1;
	m_netChnn =-1;
	m_pSetCamPara = NULL;
	m_pSetCamParaChange = NULL; 
	m_change = false;
}

CSetCamParaDlg::~CSetCamParaDlg()
{
	if ( NULL != m_pSetCamPara)
	{
		delete [] m_pSetCamPara;
	}
	if (NULL != m_pSetCamParaChange)
	{
		delete [] m_pSetCamParaChange;
	}
}

void CSetCamParaDlg::SetChnnInfo(const CAMERA_PARAMTER *pCamPara,int sel,int netChnn,int localChnn,int controlType)
{
	assert(pCamPara);
	assert(sel<netChnn+localChnn);

	if( m_pSetCamPara )
	{
		delete[] m_pSetCamPara;
		m_pSetCamPara = NULL;
	}

	m_pSetCamPara = new CAMERA_PARAMTER [netChnn+1];
	memset(m_pSetCamPara,0,(sizeof(CAMERA_PARAMTER))*(netChnn+1));
	memcpy(m_pSetCamPara,pCamPara,(sizeof( CAMERA_PARAMTER )*(netChnn)) );

	if (m_pSetCamParaChange)
	{
		delete [] m_pSetCamParaChange;
		m_pSetCamParaChange = NULL;
	}

	m_pSetCamParaChange = new CAMERA_PARAMTER [netChnn+1];
	memset( m_pSetCamParaChange,0,(sizeof(CAMERA_PARAMTER))*(netChnn+1));
	memcpy( m_pSetCamParaChange, pCamPara, (sizeof( CAMERA_PARAMTER) * (netChnn+1)) );

	m_localChnn = localChnn;
	m_netChnn = netChnn;
	m_curSel = sel;
	m_controlType = controlType;  // 工具栏进来1    现场进来 0
	printf("%s,%d,%d",__FILE__,__LINE__,m_controlType);

	////在SDK转换时，需要用的 localVedioNum,发送这个消息就是去处理它	
	//pMsgMan->PostMsgDataToServer(LOCAL_CLIENT_ID,LOCAL_DEVICE_ID,CMD_REQUEST_SET_IMAGE_ROI,NULL,0);
}
void CSetCamParaDlg::GetChnnPara(CAMERA_PARAMTER * pCamPara,int len)
{
    assert(m_netChnn == len);
	memcpy(pCamPara, m_pSetCamParaChange,(sizeof(CAMERA_PARAMTER))*len);
}

void CSetCamParaDlg::OnInitial()
{
	assert(m_pSetCamPara);		//必须先设置SetChnnInfo
	assert(m_pSetCamParaChange);
	
	/*m_outModifyAll=false;*/
	
	GUI::CDialog::OnInitial();
	m_style |= WND_NO_TITLE;

	unsigned short xLeft = CFG_DLG_LEFT_X, yTop = CFG_DLG_TOP_Y;
	unsigned short xOff = CFG_DLG_OFF_X, yOff = CFG_DLG_OFF_Y/2;
	unsigned short width = GetWidth() - 2 * xLeft;
	unsigned short height = m_length[COMBO_HEIGHT];
	
	char str[8] = {0};
	GUI::CComboCtrl*    pComboRotato[1] = {&m_comboRotato};

	unsigned long x = xLeft, y = yTop, cx = width, cy = height;

	m_stcChnn.Create( GUI::CWnd::GetRandWndID(), x, y, cx - 40, cy, this);
	y += cy + yOff;  
	m_comboChnn.Create( GUI::CWnd::GetRandWndID(), x, y, cx, cy, this, 0, 1);
	y += cy;

	unsigned long btnWidth = 30;
	unsigned long btnHeight = 25;

	x = xLeft+xOff;
	y += yOff;
	m_ckMirror.Create( GUI::CWnd::GetRandWndID(),x,y,btnWidth,btnHeight, this, 0, 2);
	x += btnWidth + 5;
    m_stMirror.Create( GUI::CWnd::GetRandWndID(), x, y, width/3, m_length[BTN_HEIGHT], this);
	x += (width/3);
	x += 10;

	m_ckFilp.Create( GUI::CWnd::GetRandWndID(),x,y,btnWidth,btnHeight, this, 0, 3);
	x += btnWidth + 5;
	m_stFlip.Create( GUI::CWnd::GetRandWndID(), x, y, width/3, m_length[BTN_HEIGHT], this);

	x = xLeft+xOff;
	y += (m_length[BTN_HEIGHT] + yOff);

	m_neDistortion.Create( GUI::CWnd::GetRandWndID(), x, y, cx - 2 * xOff- 60, cy, this);

    m_valueDistortion.Create( GUI::CWnd::GetRandWndID(), (cx - 2 * xOff - 60)+8, y+4, 60 , cy, this);
	m_valueDistortion.SetBKColor(COLOR_DLG_CHILD_BK);
	m_valueDistortion.SetAlignText(ALIGN_TEXT_RIGHT);
    y += (cy + yOff);
    x = xLeft+xOff;
	
	m_chDistortion.Create(GUI::CWnd::GetRandWndID(),x,y,btnWidth, btnHeight, this, 0, 4);
	m_chDistortion.SetCheck(false,false);
	x += btnWidth + 2;
	m_slDistortion.Create(GUI::CWnd::GetRandWndID(), x, y, cx - 2 * xOff - 4 - 2 * btnWidth, this, 0, 5, true );

	x = xLeft+xOff;
	y += cy + yOff;

	/*m_neTotato.Create( GUI::CWnd::GetRandWndID(), x, y+4, cx - 2 * xOff- 60, cy, this);
	y += cy + yOff;
	x = xLeft+xOff*3/2;

	pComboRotato[0]->Create( GUI::CWnd::GetRandWndID(), x, y, width-10, m_length[BTN_HEIGHT]-5, this, 0, 6);
	for (int i =0; i < 4; i++)
	{
		sprintf(str,"%d",lRotato[i]);
		pComboRotato[0]->AddItem(str,sizeof(long));   //E_STR_ID_IMAGE_ROTATO
	}
	m_comboRotato.SetCurItem(0,false);
	m_comboRotato.SetDropDownCount(4);
	m_comboRotato.SetEnable(false);             //false
	y += (m_length[BTN_HEIGHT] + yOff*2); */
   
	y += yOff*2;
	x = xLeft;
	m_stFocusMode.Create(GUI::CWnd::GetRandWndID(),x,y,cx - 2 * xOff- 60,cy, this);
	m_stFocusMode.SetCaption(m_pStrTable->String(E_STR_ID_CAM_FOCUS_MODE));
    y += yOff+cy;
    x = xLeft;

    m_comFocusMode.Create( GUI::CWnd::GetRandWndID(), x, y, cx, cy, this, 0, 7);
	
	y += xOff*4+(m_length[BTN_HEIGHT]-5);
	x = xLeft+xOff*2;

	m_btnFocusDec.Create(GUI::CWnd::GetRandWndID(), x, y, btnWidth, m_length[BTN_HEIGHT], this, 0, 8);
	m_btnFocusDec.SetImage(PAINT_STATUS_NORMAL, BMP_BTN_DEC, false);
	m_btnFocusDec.SetImage(PAINT_STATUS_FOCUS, BMP_BTN_DEC_FOCUS, false);
	x+=(btnWidth+8);
	m_btnAutoFocus.Create(GUI::CWnd::GetRandWndID(), x, y, 100, m_length[BTN_HEIGHT], this, 0, 9);
	m_btnAutoFocus.SetCaption(m_pStrTable->String(E_STR_ID_IMAGE_AUTO_FOCUS),false);
	x+=108;
	m_btnFocusAdd.Create(GUI::CWnd::GetRandWndID(), x, y, btnWidth, m_length[BTN_HEIGHT], this, 0, 10);
	m_btnFocusAdd.SetImage(PAINT_STATUS_NORMAL, BMP_BTN_ADD, false);
	m_btnFocusAdd.SetImage(PAINT_STATUS_FOCUS, BMP_BTN_ADD_FOCUS, false);

	//y+=70;
	x = xLeft+xOff*2;
	y += (m_length[BTN_HEIGHT] + yOff*2);
	m_btnZoomDec.Create(GUI::CWnd::GetRandWndID(), x, y, btnWidth, m_length[BTN_HEIGHT], this, 0, 11);
	m_btnZoomDec.SetImage(PAINT_STATUS_NORMAL, BMP_BTN_DEC, false);
	m_btnZoomDec.SetImage(PAINT_STATUS_FOCUS, BMP_BTN_DEC_FOCUS, false);
	x+=(btnWidth+8);
	m_stZoom.Create(GUI::CWnd::GetRandWndID(), x, y, 100,  m_length[BTN_HEIGHT], this);
	m_stZoom.SetCaption(m_pStrTable->String(E_STR_ID_CAM_MANUAL_ZOOM));
	x+=108;
	m_btnZoomAdd.Create(GUI::CWnd::GetRandWndID(), x, y, btnWidth, m_length[BTN_HEIGHT], this, 0, 12);
	m_btnZoomAdd.SetImage(PAINT_STATUS_NORMAL, BMP_BTN_ADD, false);
	m_btnZoomAdd.SetImage(PAINT_STATUS_FOCUS, BMP_BTN_ADD_FOCUS, false);

	y+=35;

	//ok button
	x = xLeft; 
	m_btnOK.Create( GUI::CWnd::GetRandWndID(), x, y, width, m_length[BTN_HEIGHT], this, 0, 13);
	m_btnOK.SetCaption(m_pStrTable->String(E_STR_ID_SAVE),true);

	//// Exit Button
	cx = 35;
	cy = 35;
	x = GetWidth() - cx;
	y = 2;
	m_btnExit.Create( GUI::CWnd::GetRandWndID(), x, y, cx, cy, this, 0, 14);
	m_btnExit.SetImage( PAINT_STATUS_NORMAL, BMP_BTN_EXIT4, false );
	m_btnExit.SetImage( PAINT_STATUS_FOCUS, BMP_BTN_EXIT4_FOCUS, false );
	
	//默认初始化 控件内容 
	SetCtrlData();

}

void CSetCamParaDlg::OnDestroy()
{
	GUI::CDialog::OnDestroy();
}

void CSetCamParaDlg::EndDialog(DLG_END_VALUE result)
{
	if (DLG_OK != result)
	{
		/*RecoveryChnnColor( m_color, m_pColorChange, m_maxChnn );*/
	}
	GUI::CDialog::EndDialog(result);
}

void CSetCamParaDlg::SetCtrlData()
{
	char str[128] = {0};
	snprintf( str, 128, "%s", m_pStrTable->String(E_STR_ID_CHANNEL) );
	m_stcChnn.SetCaption(str , false);
	m_stFlip.SetCaption( m_pStrTable->String(E_STR_ID_IMAGE_FILP_ON),false);
	m_stMirror.SetCaption( m_pStrTable->String(E_STR_ID_IMAGE_MIRROR_ON),false);

	m_comFocusMode.AddItem(m_pStrTable->String(E_STR_ID_CAM_MANUAL_FOCUS),0);
	//m_comFocusMode.AddItem(m_pStrTable->String(E_STR_ID_CAM_AUTO_FOCUS),1);
	m_comFocusMode.SetCurItem(0,false);

	ULONGLONG CH = 0;
	int localVideoInputNum = 0, netVideoInputNum = 0;
	int maxChnn = 0, effectChnn = 0;

	IsSupportCH(CH, &effectChnn, &localVideoInputNum, &netVideoInputNum);
	m_netChnn = netVideoInputNum;
	m_localChnn = localVideoInputNum;

	if( -1==m_curSel )
	{
		//strcpy(str,"All");
		m_comboChnn.AddItem(m_pStrTable->String(E_STR_ID_ALL), -1);
		m_comboChnn.SetCurItem(0, false);
	}
	else
	{
		for ( int i = m_localChnn; i < m_netChnn+m_localChnn; i ++ )
		{
			if((CH >> i) & 0x1)
	         {
				sprintf(str, "%d", i + 1);
				m_comboChnn.AddItem(str, i);
			}
		}
		m_comboChnn.SetCurItemData( m_curSel, false );
	}
   
	m_neDistortion.SetCaption(m_pStrTable->String(E_STR_ID_IMAGE_DISTORTION),false);
	//m_neTotato.SetCaption(m_pStrTable->String(E_STR_ID_IMAGE_ROTATO),false);
	m_slDistortion.SetRange( 0,255 );

	SetValue(m_curSel);
}

void CSetCamParaDlg::SetValue(int currChnn)
{
	int chnn;
	//assert((0 <= chnn)&&(chnn < m_netChnn));
    if (currChnn < 0)
    {
		chnn = 0;
	}
	else
	{
		chnn = currChnn-m_localChnn;
		assert((chnn>=0)&&(128>chnn));
	}

	//mirror
	if (0 == m_pSetCamPara[chnn].m_mirror)
	{
		m_ckMirror.SetCheck(false);
	}else if (1==m_pSetCamPara[chnn].m_mirror)
	{
		m_ckMirror.SetCheck(true);
	}
	{
		char charMirro = m_pSetCamPara[chnn].m_mirror;
		unsigned long lMirro = 0;

		lMirro |= charMirro;
		if(( 0 <= m_curSel )&&( m_curSel < m_localChnn+m_netChnn ))
		{
			lMirro|= (m_curSel<<8);
		}
		else if( -1 == m_curSel )
		{
			//用通道的最大值来代表所有通道
			lMirro|=((m_localChnn+m_netChnn)<<8);
		}
		CMessageMan::Instance()->PostMsgDataToServer( LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_SET_IMAGE_MIRROR, (unsigned char *)&lMirro, sizeof( unsigned long ) );
		m_pSetCamParaChange[chnn].m_mirror = m_pSetCamPara[chnn].m_mirror;
	}

	//FLIP
	if (0 == m_pSetCamPara[chnn].m_flip)
	{
		m_ckFilp.SetCheck(false);
	}else if (1 == m_pSetCamPara[chnn].m_flip)
	{
		m_ckFilp.SetCheck(true);
	}
	{
		unsigned char charFlip = m_pSetCamPara[chnn].m_flip;
		unsigned long lFlip = 0;		
		MESSAGE_DATA msgData;
		CMessageMan* pMsgMan = CMessageMan::Instance();

		lFlip |= charFlip;
		if(( 0 <= m_curSel )&&( m_curSel < m_localChnn+m_netChnn ))
		{
			lFlip|= (m_curSel<<8);
		}
		else if( -1 == m_curSel )
		{
			//用通道的最大值来代表所有通道
			lFlip|=((m_localChnn+m_netChnn)<<8);
		}
		CMessageMan::Instance()->PostMsgDataToServer( LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_SET_IMAGE_FILP, (unsigned char *)&lFlip, sizeof( unsigned long ) );
		m_pSetCamParaChange[chnn].m_flip = m_pSetCamPara[chnn].m_flip;
	}

	if (0 == m_pSetCamPara[chnn].m_lensDistortion.ucLensDistortion){
		m_chDistortion.SetCheck(false);
	}else if (1== m_pSetCamPara[chnn].m_lensDistortion.ucLensDistortion){
		m_chDistortion.SetCheck(true);
	}
	unsigned int value = m_pSetCamPara[chnn].m_lensDistortion.ucLensDistortionValue;
	char valueStr[6]={0};
	if (255<value)
	{
		value = 255;
	}
	sprintf(valueStr,"%d",value);
	m_slDistortion.SetCurPos(value,false);
	m_valueDistortion.SetCaption(valueStr,true);

	{
		LENS_DISTORTION distortion;
		memset(&distortion,0,sizeof(LENS_DISTORTION));

		distortion.ucLensDistortionValue = m_pSetCamPara[chnn].m_lensDistortion.ucLensDistortionValue;
		distortion.ucLensDistortion = m_pSetCamPara[chnn].m_lensDistortion.ucLensDistortion;
		distortion.noUse[0] = m_pSetCamPara[chnn].m_lensDistortion.noUse[0];
		distortion.noUse[1] = m_pSetCamPara[chnn].m_lensDistortion.noUse[1];

		if(( 0 <= m_curSel )&&( m_curSel < m_localChnn+m_netChnn ))
		{
			distortion.chnn = m_curSel;
		}
		else if( -1 == m_curSel ) 
		{
			//用通道的最大值来代表所有通道
			distortion.chnn = m_localChnn+m_netChnn;
		}
		CMessageMan::Instance()->PostMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID,CMD_REQUEST_SET_IMAGE_DISTORTION,(unsigned char *)&distortion,sizeof(LENS_DISTORTION));
		m_pSetCamParaChange[chnn].m_lensDistortion.ucLensDistortionValue= m_pSetCamPara[chnn].m_lensDistortion.ucLensDistortionValue;
		m_pSetCamParaChange[chnn].m_lensDistortion.ucLensDistortion = m_pSetCamPara[chnn].m_lensDistortion.ucLensDistortion;
		m_pSetCamParaChange[chnn].m_lensDistortion.noUse[0] = m_pSetCamPara[chnn].m_lensDistortion.noUse[0];
		m_pSetCamParaChange[chnn].m_lensDistortion.noUse[1] = m_pSetCamPara[chnn].m_lensDistortion.noUse[1];
	}
	m_comboRotato.SetCurItem(m_pSetCamPara[chnn].m_rotato,false);
}

void CSetCamParaDlg::SetValueChangeChnnel(int currChnn)
{
	int chnn;
	//assert((0 <= chnn)&&(chnn < m_netChnn));
	if (currChnn < 0)
	{
		chnn = 0;
	}
	else
	{
		chnn = currChnn-m_localChnn;
		assert((chnn>=0)&&(128>chnn));
	}

	//mirror
	if (0 == m_pSetCamPara[chnn].m_mirror)
	{
		m_ckMirror.SetCheck(false);
	}else if (1==m_pSetCamPara[chnn].m_mirror)
	{
		m_ckMirror.SetCheck(true);
	}
	if (m_pSetCamParaChange[chnn].m_mirror != m_pSetCamPara[chnn].m_mirror)
	{
		char charMirro = m_pSetCamPara[chnn].m_mirror;
		unsigned long lMirro = 0;

		lMirro |= charMirro;
		if(( 0 <= m_curSel )&&( m_curSel < m_localChnn+m_netChnn ))
		{
			lMirro|= (m_curSel<<8);
		}
		else if( -1 == m_curSel )
		{
			//用通道的最大值来代表所有通道
			lMirro|=((m_localChnn+m_netChnn)<<8);
		}
		CMessageMan::Instance()->PostMsgDataToServer( LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_SET_IMAGE_MIRROR, (unsigned char *)&lMirro, sizeof( unsigned long ) );
		m_pSetCamParaChange[chnn].m_mirror = m_pSetCamPara[chnn].m_mirror;
	}

	//FLIP
	if (0 == m_pSetCamPara[chnn].m_flip)
	{
		m_ckFilp.SetCheck(false);
	}else if (1 == m_pSetCamPara[chnn].m_flip)
	{
		m_ckFilp.SetCheck(true);
	}
	if (m_pSetCamParaChange[chnn].m_flip != m_pSetCamPara[chnn].m_flip)
	{
		unsigned char charFlip = m_pSetCamPara[chnn].m_flip;
		unsigned long lFlip = 0;		
		MESSAGE_DATA msgData;
		CMessageMan* pMsgMan = CMessageMan::Instance();

		lFlip |= charFlip;
		if(( 0 <= m_curSel )&&( m_curSel < m_localChnn+m_netChnn ))
		{
			lFlip|= (m_curSel<<8);
		}
		else if( -1 == m_curSel )
		{
			//用通道的最大值来代表所有通道
			lFlip|=((m_localChnn+m_netChnn)<<8);
		}
		CMessageMan::Instance()->PostMsgDataToServer( LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_SET_IMAGE_FILP, (unsigned char *)&lFlip, sizeof( unsigned long ) );
		m_pSetCamParaChange[chnn].m_flip = m_pSetCamPara[chnn].m_flip;
	}

    //distortion
	if (0 == m_pSetCamPara[chnn].m_lensDistortion.ucLensDistortion){
		m_chDistortion.SetCheck(false);
	}else if (1== m_pSetCamPara[chnn].m_lensDistortion.ucLensDistortion){
		m_chDistortion.SetCheck(true);
	}
	unsigned int value = m_pSetCamPara[chnn].m_lensDistortion.ucLensDistortionValue;	
	char valueStr[6]={0};
	if (value>255)
	{
		value = 255;	
	}
	sprintf(valueStr,"%d",value);
	m_slDistortion.SetCurPos(value,false);
	m_valueDistortion.SetCaption(valueStr,true);
	printf("%s,%d,%s,%d",valueStr, value, __FILE__, __LINE__);
	if ( memcmp( &(m_pSetCamParaChange[chnn].m_lensDistortion), &(m_pSetCamPara[chnn].m_lensDistortion),sizeof(LENS_DISTORTION_CONFIG)) )
	{
		LENS_DISTORTION distortion;
		memset(&distortion,0,sizeof(LENS_DISTORTION));

		distortion.ucLensDistortionValue = m_pSetCamPara[chnn].m_lensDistortion.ucLensDistortionValue;
		distortion.ucLensDistortion = m_pSetCamPara[chnn].m_lensDistortion.ucLensDistortion;
		distortion.noUse[0] = m_pSetCamPara[chnn].m_lensDistortion.noUse[0];
		distortion.noUse[1] = m_pSetCamPara[chnn].m_lensDistortion.noUse[1];

		if(( 0 <= m_curSel )&&( m_curSel < m_localChnn+m_netChnn ))
		{
			distortion.chnn = m_curSel;
		}
		else if( -1 == m_curSel ) 
		{
			//用通道的最大值来代表所有通道
			distortion.chnn = m_localChnn+m_netChnn;
		}
		CMessageMan::Instance()->PostMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID,CMD_REQUEST_SET_IMAGE_DISTORTION,(unsigned char *)&distortion,sizeof(LENS_DISTORTION));
		m_pSetCamParaChange[chnn].m_lensDistortion.ucLensDistortionValue= m_pSetCamPara[chnn].m_lensDistortion.ucLensDistortionValue;
		m_pSetCamParaChange[chnn].m_lensDistortion.ucLensDistortion = m_pSetCamPara[chnn].m_lensDistortion.ucLensDistortion;
		m_pSetCamParaChange[chnn].m_lensDistortion.noUse[0] = m_pSetCamPara[chnn].m_lensDistortion.noUse[0];
		m_pSetCamParaChange[chnn].m_lensDistortion.noUse[1] = m_pSetCamPara[chnn].m_lensDistortion.noUse[1];
	}

	m_comboRotato.SetCurItem(m_pSetCamPara[chnn].m_rotato,false);
}

BEGIN_MSG_MAP(CSetCamParaDlg, GUI::CDialog)
ON_MSG(m_ckMirror.GetID(), KEY_VIRTUAL_ENTER, OnCkMirro)
ON_MSG(m_ckFilp.GetID(), KEY_VIRTUAL_ENTER, OnCkFilp)
//ON_MSG(m_comboRotato.GetID(), KEY_VIRTUAL_ENTER, OnComboRotato)
ON_MSG_EX(m_slDistortion.GetID(), KEY_VIRTUAL_ENTER, OnSliderDistortion)
ON_MSG_EX(m_slDistortion.GetID(), KEY_VIRTUAL_DRAG, OnSliderDistortion)
ON_MSG_EX(m_slDistortion.GetID(), KEY_VIRTUAL_BLUP, OnSliderDistortion)
ON_MSG(m_chDistortion.GetID(),KEY_VIRTUAL_ENTER, OnCheckDistortion)
ON_MSG(m_btnFocusDec.GetID(),KEY_VIRTUAL_ENTER,OnFocusDec)
ON_MSG(m_btnFocusDec.GetID(),KEY_VIRTUAL_BLUP,OnStop)

ON_MSG(m_btnFocusAdd.GetID(),KEY_VIRTUAL_ENTER,OnFocusAdd)
ON_MSG(m_btnFocusAdd.GetID(),KEY_VIRTUAL_BLUP,OnStop)

ON_MSG(m_btnAutoFocus.GetID(),KEY_VIRTUAL_ENTER,OnAutoFocus)

ON_MSG(m_btnZoomDec.GetID(),KEY_VIRTUAL_ENTER,OnZoomDec)
ON_MSG(m_btnZoomDec.GetID(),KEY_VIRTUAL_BLUP,OnStop)

ON_MSG(m_btnZoomAdd.GetID(),KEY_VIRTUAL_ENTER,OnZoomAdd)
ON_MSG(m_btnZoomAdd.GetID(),KEY_VIRTUAL_BLUP,OnStop)

ON_MSG(m_comboChnn.GetID(),KEY_VIRTUAL_ENTER,OnChnnComboChange)
ON_MSG(m_btnOK.GetID(), KEY_VIRTUAL_ENTER, OnBtnOK)
ON_MSG(m_btnExit.GetID(), KEY_VIRTUAL_ENTER, OnBtnExit)
END_MSG_MAP()

unsigned long CSetCamParaDlg::OnCkMirro()
{
	unsigned char charMirro;
	unsigned long lMirro = 0;
	std::string str;

	assert((0<=(m_curSel-m_localChnn)) && (m_curSel-m_localChnn<m_netChnn));
	if (m_ckMirror.IsCheck())
	{
		m_pSetCamParaChange[m_curSel-m_localChnn].m_mirror = 1;
		charMirro = 1;
	}else{
		m_pSetCamParaChange[m_curSel-m_localChnn].m_mirror = 0;
		charMirro = 0;
	}
	lMirro |= charMirro;
	if(( 0 <= m_curSel )&&( m_curSel < m_localChnn+m_netChnn ))
	{
		lMirro|= (m_curSel<<8);
	}
	else if( -1 == m_curSel )
	{
		//用通道的最大值来代表所有通道
		lMirro|=((m_localChnn+m_netChnn)<<8);
	}

	CMessageMan::Instance()->PostMsgDataToServer( LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_SET_IMAGE_MIRROR, (unsigned char *)&lMirro, sizeof( unsigned long ) );
	return KEY_VIRTUAL_MSG_STOP;
}
unsigned long CSetCamParaDlg::OnCkFilp()
{
	unsigned char charFlip;
	unsigned long lFlip = 0;

	assert((0<=(m_curSel-m_localChnn)) && (m_curSel-m_localChnn<m_netChnn));
	if (m_ckFilp.IsCheck())
	{
		charFlip = 1;
		m_pSetCamParaChange[m_curSel-m_localChnn].m_flip = 1;
	} else{

		charFlip = 0;
		m_pSetCamParaChange[m_curSel-m_localChnn].m_flip = 0;
	}

	lFlip |= charFlip;
	if(( 0 <= m_curSel )&&( m_curSel < m_localChnn+m_netChnn ))
	{
		lFlip|= (m_curSel<<8);
	}
	else if( -1 == m_curSel )
	{
		//用通道的最大值来代表所有通道
		lFlip|=((m_localChnn+m_netChnn)<<8);
	}
	CMessageMan::Instance()->PostMsgDataToServer( LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_SET_IMAGE_FILP, (unsigned char *)&lFlip, sizeof( unsigned long ) );

	return KEY_VIRTUAL_MSG_STOP;
}
unsigned long CSetCamParaDlg::OnCheckDistortion()
{
	unsigned int value = 0;
	assert((0<=(m_curSel-m_localChnn)) && (m_curSel-m_localChnn<m_netChnn));
	m_chDistortion.IsCheck()?value =1:value =0;
	m_pSetCamParaChange[m_curSel-m_localChnn].m_lensDistortion.ucLensDistortion = value;
   /* if (0==value)
    {*/
	char szValue[16] = { 0 };
	int pos = m_slDistortion.GetPos();
	sprintf(szValue, "%d", pos);
	m_valueDistortion.SetCaption(szValue, true); 
	m_ucLensDistortionValue = pos;
	//m_valueDistortion.SetCaption(" ",true);
    /*}*/
	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CSetCamParaDlg::OnSliderDistortion(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	LENS_DISTORTION distortion;
	int Chnn = m_curSel- m_localChnn;
	assert( (0<=Chnn) && ( Chnn<m_netChnn ));
	memset(&distortion,0,sizeof(LENS_DISTORTION));
	char szValue[16] = { 0 };
	int pos = m_slDistortion.GetPos();
	if ( m_pSetCamParaChange[Chnn].m_lensDistortion.ucLensDistortion )
	{
		if ( m_ucLensDistortionValue != pos )
		{
			m_pSetCamParaChange[Chnn].m_lensDistortion.ucLensDistortionValue = pos;
			m_ucLensDistortionValue = pos;
			sprintf(szValue, "%d", pos);
			m_valueDistortion.SetCaption(szValue, true); 

			distortion.ucLensDistortionValue = m_pSetCamParaChange[Chnn].m_lensDistortion.ucLensDistortionValue;
			distortion.ucLensDistortion = m_pSetCamParaChange[Chnn].m_lensDistortion.ucLensDistortion;

			if(( 0 <= m_curSel )&&( m_curSel < m_localChnn+m_netChnn ))
			{
				distortion.chnn = m_curSel;
			}
			else if( -1 == m_curSel ) 
			{
				//用通道的最大值来代表所有通道
				distortion.chnn = m_localChnn+m_netChnn;
			}
			CMessageMan::Instance()->PostMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID,CMD_REQUEST_SET_IMAGE_DISTORTION,(unsigned char *)&distortion,sizeof(LENS_DISTORTION));
		}
	}
	else
	{
		sprintf(szValue, "%d", pos);
		m_valueDistortion.SetCaption(szValue, true); 
		m_ucLensDistortionValue = pos;
	}
	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CSetCamParaDlg::OnComboRotato()   //(旋转-——走廊模式)0-0,1-90,
{
	//保存当前页面值  
	int sel = m_comboRotato.GetCurItem();
	int rotatoChnn = m_curSel-m_localChnn;
	assert((0<=rotatoChnn) && (rotatoChnn<m_netChnn));
	if ( sel < 0)
	{
		return KEY_VIRTUAL_MSG_STOP;
	}
	
	unsigned char charRotato = sel;
    m_pSetCamParaChange[rotatoChnn].m_rotato = sel;

	unsigned long lRotato = 0;

	lRotato |= charRotato;
	if(0 <= m_curSel)
	{
		lRotato|= (m_curSel<<8);
	}
	else if( -1 == m_curSel )
	{
		//用通道的最大值来代表所有通道
		lRotato|=(m_curSel+m_localChnn<<8);
	}
	CMessageMan::Instance()->PostMsgDataToServer( LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_SET_IMAGE_ROTATO, (unsigned char *)&lRotato, sizeof( unsigned long ) );

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CSetCamParaDlg::OnAutoFocus()
{
	int focusChnn = m_curSel-m_localChnn;
	AF_CONTROL_T af_ctrl;
	af_ctrl.chnn = m_curSel;
	af_ctrl.afType = ONETIME_FOCUS;
	af_ctrl.nValue = 0;
    assert((0<=focusChnn) && (focusChnn<m_netChnn));
	m_pSetCamParaChange[focusChnn].m_focus.chnn = m_curSel;
	m_pSetCamParaChange[focusChnn].m_focus.afType = ONETIME_FOCUS;
	m_pSetCamParaChange[focusChnn].m_focus.nValue = 0;
	/*CMessageMan::Instance()->PostMsgDataToServer( LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_SET_AUTOFOCUS, (unsigned char *)&af_ctrl, sizeof( AF_CONTROL_T ) );*/
	MESSAGE_DATA msgData;
	memset(&msgData, 0, sizeof(MESSAGE_DATA));
	CMessageMan::Instance()->SendMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_SET_AUTOFOCUS, (unsigned char *)&af_ctrl, sizeof(AF_CONTROL_T), msgData);
	ReleaseMsgBuff( msgData );
	return KEY_VIRTUAL_MSG_STOP;
}

bool CSetCamParaDlg::ReleaseMsgBuff(MESSAGE_DATA &msgData)
{
	assert( NULL != msgData.pData );
	PACKCMD * pPackCmd = reinterpret_cast<PACKCMD*>(msgData.pData);
	if( CMD_REPLY_SET_AUTOFOCUS_SUCC == pPackCmd->cmdType )
	{
		CMessageMan::Instance()->ReleaseMsgBuff( msgData );
		return true;
	}
	else if( CMD_REPLY_SET_AUTOFOCUS_FAIL == pPackCmd->cmdType )
	{
		bool ptzValue = *reinterpret_cast<bool *>(msgData.pData + sizeof(PACKCMD));
		/*ENUM_STRING_ID strID = GetPtzCtrlStrID(ptzValue);
		MessageOut( GetTopestWnd(), strID, TIP_MSG );*/
		CMessageMan::Instance()->ReleaseMsgBuff( msgData );
		return false;
	}
}
unsigned long CSetCamParaDlg::OnFocusDec()
{
	AF_CONTROL_T af_ctrl;
	af_ctrl.chnn = m_curSel;
	af_ctrl.afType = MANUAL_FOCUS;
	af_ctrl.nValue = 0;
	//CMessageMan::Instance()->PostMsgDataToServer( LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_SET_AUTOFOCUS, (unsigned char *)&af_ctrl, sizeof( AF_CONTROL_T ) );
	MESSAGE_DATA msgData;
	memset(&msgData, 0, sizeof(MESSAGE_DATA));
	CMessageMan::Instance()->SendMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_SET_AUTOFOCUS, (unsigned char *)&af_ctrl, sizeof(AF_CONTROL_T), msgData);
	ReleaseMsgBuff( msgData );

	return KEY_VIRTUAL_MSG_STOP;
}
unsigned long CSetCamParaDlg::OnFocusAdd()
{
	AF_CONTROL_T af_ctrl;
	af_ctrl.chnn = m_curSel;
	af_ctrl.afType = MANUAL_FOCUS;
	af_ctrl.nValue = 1;
	//CMessageMan::Instance()->PostMsgDataToServer( LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_SET_AUTOFOCUS, (unsigned char *)&af_ctrl, sizeof( AF_CONTROL_T ) );
	MESSAGE_DATA msgData;
	memset(&msgData, 0, sizeof(MESSAGE_DATA));
	CMessageMan::Instance()->SendMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_SET_AUTOFOCUS, (unsigned char *)&af_ctrl, sizeof(AF_CONTROL_T), msgData);
	ReleaseMsgBuff( msgData );
	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CSetCamParaDlg::OnZoomDec()
{
	AF_CONTROL_T af_ctrl;
	af_ctrl.chnn = m_curSel;
	af_ctrl.afType = MANUAL_ZOOM;
	af_ctrl.nValue = 0;
	//CMessageMan::Instance()->PostMsgDataToServer( LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_SET_AUTOFOCUS, (unsigned char *)&af_ctrl, sizeof( AF_CONTROL_T ) );
	MESSAGE_DATA msgData;
	memset(&msgData, 0, sizeof(MESSAGE_DATA));
	CMessageMan::Instance()->SendMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_SET_AUTOFOCUS, (unsigned char *)&af_ctrl, sizeof(AF_CONTROL_T), msgData);
	ReleaseMsgBuff( msgData );
	return KEY_VIRTUAL_MSG_STOP;
}
unsigned long CSetCamParaDlg::OnZoomAdd()
{
	AF_CONTROL_T af_ctrl;
	af_ctrl.chnn = m_curSel;
	af_ctrl.afType = MANUAL_ZOOM;
	af_ctrl.nValue = 1;
	//CMessageMan::Instance()->PostMsgDataToServer( LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_SET_AUTOFOCUS, (unsigned char *)&af_ctrl, sizeof( AF_CONTROL_T ) );
	MESSAGE_DATA msgData;
	memset(&msgData, 0, sizeof(MESSAGE_DATA));
	CMessageMan::Instance()->SendMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_SET_AUTOFOCUS, (unsigned char *)&af_ctrl, sizeof(AF_CONTROL_T), msgData);
	ReleaseMsgBuff( msgData );
	return KEY_VIRTUAL_MSG_STOP;
}
unsigned long CSetCamParaDlg::OnStop()
{
	AF_CONTROL_T af_ctrl;
	af_ctrl.chnn = m_curSel;
	af_ctrl.afType = MANUAL_STOP;
	af_ctrl.nValue = 0;
	//CMessageMan::Instance()->PostMsgDataToServer( LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_SET_AUTOFOCUS, (unsigned char *)&af_ctrl, sizeof( AF_CONTROL_T ) );
	MESSAGE_DATA msgData;
	memset(&msgData, 0, sizeof(MESSAGE_DATA));
	CMessageMan::Instance()->SendMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_SET_AUTOFOCUS, (unsigned char *)&af_ctrl, sizeof(AF_CONTROL_T), msgData);
	ReleaseMsgBuff( msgData );
	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CSetCamParaDlg::OnChangeValue()
{
	m_change = true;
	return KEY_VIRTUAL_MSG_STOP;
}
void CSetCamParaDlg::SelectCh(const CAMERA_PARAMTER camPara,int chnn)//切换通道后颜色和显示模式的更改
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

	maxChnn = localVideoInputNum + netVideoInputNum;

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


	//设值
	SetValue(chnn);

	CMessageMan* pMsgMan = CMessageMan::Instance();

	//发送修改显示模式
	MESSAGE_DATA messageData;
	memset(&messageData, 0, sizeof(MESSAGE_DATA));
	pMsgMan->SendMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_CHANGE_DISPLAY_MODE, (unsigned char *)&displayInfo, sizeof(NET_DISPLAY_INFO), messageData );
	pMsgMan->ReleaseMsgBuff(messageData);

	GetRootWnd()->Repaint();
}

unsigned long CSetCamParaDlg::OnChnnComboChange()
{
	//还原之前通道的值
	SetValueChangeChnnel(m_curSel);

	int sel = m_comboChnn.GetCurItemData();
	m_curSel = sel;
	CAMERA_PARAMTER camPara;
	memset(&camPara,0,sizeof(CAMERA_PARAMTER));
	int chnn = m_curSel-m_localChnn;
	assert(0<=chnn && chnn<m_netChnn);
	strcpy((char *)&camPara, (char *)(&(m_pSetCamParaChange[m_curSel-m_localChnn])));

	SelectCh(camPara,m_curSel);
	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CSetCamParaDlg::OnBtnOK()
{
	if (0==m_controlType)
	{
		return OnOK();
		printf("%s,%d",__FILE__,__LINE__);
	} 
	else if (1==m_controlType)
	{
		int chnn = m_curSel-m_localChnn;
		assert(0<=chnn && chnn<m_netChnn);
		assert((NULL != m_pSetCamPara)&&( NULL != m_pSetCamParaChange));
		/*for (unsigned int i=0;i<m_netChnn;i++)
		{*/
		if ( (m_pSetCamPara[chnn].m_chnn == m_pSetCamParaChange[chnn].m_chnn))
		{
			//FLIP
			if (m_pSetCamParaChange[chnn].m_flip != m_pSetCamPara[chnn].m_flip)
			{
				NET_CHNN_FLIP chnnFlip;
				memset( &chnnFlip, 0, sizeof( NET_CHNN_FLIP ) );
				MESSAGE_DATA msgData;
				CMessageMan* pMsgMan = CMessageMan::Instance();

				chnnFlip.chnn=chnn;
				chnnFlip.flip=m_pSetCamParaChange[chnn].m_flip;
				pMsgMan->SendMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_SAVE_IMAGE_FILP,(unsigned char *)&chnnFlip, sizeof(NET_CHNN_FLIP), msgData);
				pMsgMan->ReleaseMsgBuff( msgData );

				m_pSetCamPara[chnn].m_flip = m_pSetCamParaChange[chnn].m_flip;
			}
			//mirror
			if (m_pSetCamParaChange[chnn].m_mirror != m_pSetCamPara[chnn].m_mirror)
			{
				NET_CHNN_MIRROR chnnMirror;
				memset( &chnnMirror, 0, sizeof( NET_CHNN_MIRROR ) );
				MESSAGE_DATA msgData;
				CMessageMan* pMsgMan = CMessageMan::Instance();

				chnnMirror.chnn=chnn;
				chnnMirror.mirror=m_pSetCamParaChange[chnn].m_mirror;
				pMsgMan->SendMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_SAVE_IMAGE_MIRROR,(unsigned char *)&chnnMirror, sizeof(NET_CHNN_MIRROR), msgData);
				pMsgMan->ReleaseMsgBuff( msgData );

				m_pSetCamPara[chnn].m_mirror = m_pSetCamParaChange[chnn].m_mirror;
			}
			/*
			//rotato
			if (m_pSetCamParaChange[chnn].m_rotato != m_pSetCamPara[chnn].m_rotato)
			{
				NET_CHNN_ROTATO chnnRotato;
				memset( &chnnRotato, 0, sizeof( NET_CHNN_MIRROR ) );
				MESSAGE_DATA msgData;
				CMessageMan* pMsgMan = CMessageMan::Instance();

				chnnRotato.chnn=chnn;
				chnnRotato.rotato=m_pSetCamParaChange[chnn].m_rotato;
				pMsgMan->SendMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_SAVE_IMAGE_ROTATO,(unsigned char *)&chnnRotato, sizeof(NET_CHNN_ROTATO), msgData);
				pMsgMan->ReleaseMsgBuff( msgData );

				m_pSetCamPara[chnn].m_rotato = m_pSetCamParaChange[chnn].m_rotato;
			} */
			//dirtortion
			if ( memcmp( &(m_pSetCamParaChange[chnn].m_lensDistortion), &(m_pSetCamPara[chnn].m_lensDistortion),sizeof(LENS_DISTORTION_CONFIG)) )
			{
				NET_CHNN_LENS_DISTORTION chnnLensDistortion;
				memset( &chnnLensDistortion, 0, sizeof( NET_CHNN_LENS_DISTORTION ) );
				MESSAGE_DATA msgData;
				CMessageMan* pMsgMan = CMessageMan::Instance();

				chnnLensDistortion.chnn=chnn;
				chnnLensDistortion.lensDistortion.ucLensDistortion=m_pSetCamParaChange[chnn].m_lensDistortion.ucLensDistortion;
				chnnLensDistortion.lensDistortion.ucLensDistortionValue=m_pSetCamParaChange[chnn].m_lensDistortion.ucLensDistortionValue;
				chnnLensDistortion.lensDistortion.noUse[0]=m_pSetCamParaChange[chnn].m_lensDistortion.noUse[0];
				chnnLensDistortion.lensDistortion.noUse[1]=m_pSetCamParaChange[chnn].m_lensDistortion.noUse[1];

				m_pSetCamPara[chnn].m_lensDistortion.ucLensDistortion = m_pSetCamParaChange[chnn].m_lensDistortion.ucLensDistortion;
				m_pSetCamPara[chnn].m_lensDistortion.ucLensDistortionValue = m_pSetCamParaChange[chnn].m_lensDistortion.ucLensDistortionValue;
				m_pSetCamPara[chnn].m_lensDistortion.noUse[0] = m_pSetCamParaChange[chnn].m_lensDistortion.noUse[0];
				m_pSetCamPara[chnn].m_lensDistortion.noUse[1] = m_pSetCamParaChange[chnn].m_lensDistortion.noUse[1];

				pMsgMan->SendMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_SAVE_IMAGE_DISTORTION,(unsigned char *)&chnnLensDistortion, sizeof(NET_CHNN_LENS_DISTORTION), msgData);
				pMsgMan->ReleaseMsgBuff( msgData );

			}
			
		}
		printf("%s,%d",__FILE__,__LINE__);
		return 1;
	}
}

unsigned long CSetCamParaDlg::OnBtnExit()
{
	int chnn = m_curSel-m_localChnn;
	assert(0<=chnn && chnn<m_netChnn);
	assert((NULL != m_pSetCamPara)&&( NULL != m_pSetCamParaChange));
	if ( (m_pSetCamPara[chnn].m_chnn == m_pSetCamParaChange[chnn].m_chnn))
		{
			//FLIP
			if (m_pSetCamParaChange[chnn].m_flip != m_pSetCamPara[chnn].m_flip)
			{
				unsigned char charFlip = m_pSetCamPara[chnn].m_flip;
				unsigned long lFlip = 0;		
				MESSAGE_DATA msgData;
				CMessageMan* pMsgMan = CMessageMan::Instance();

				lFlip |= charFlip;
				if(( 0 <= m_curSel )&&( m_curSel < m_localChnn+m_netChnn ))
				{
					lFlip|= (m_curSel<<8);
				}
				else if( -1 == m_curSel )
				{
					//用通道的最大值来代表所有通道
					lFlip|=((m_localChnn+m_netChnn)<<8);
				}
				CMessageMan::Instance()->PostMsgDataToServer( LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_SET_IMAGE_FILP, (unsigned char *)&lFlip, sizeof( unsigned long ) );
				m_pSetCamParaChange[chnn].m_flip = m_pSetCamPara[chnn].m_flip;
			}
			//mirror
			if (m_pSetCamParaChange[chnn].m_mirror != m_pSetCamPara[chnn].m_mirror)
			{
				char charMirro = m_pSetCamPara[chnn].m_mirror;
				unsigned long lMirro = 0;

				lMirro |= charMirro;
				if(( 0 <= m_curSel )&&( m_curSel < m_localChnn+m_netChnn ))
				{
					lMirro|= (m_curSel<<8);
				}
				else if( -1 == m_curSel )
				{
					//用通道的最大值来代表所有通道
					lMirro|=((m_localChnn+m_netChnn)<<8);
				}
				CMessageMan::Instance()->PostMsgDataToServer( LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_SET_IMAGE_MIRROR, (unsigned char *)&lMirro, sizeof( unsigned long ) );
				m_pSetCamParaChange[chnn].m_mirror = m_pSetCamPara[chnn].m_mirror;
			}
			/*
			//rotato
			if (m_pSetCamParaChange[chnn].m_rotato != m_pSetCamPara[chnn].m_rotato)
			{
				NET_CHNN_ROTATO chnnRotato;
				memset( &chnnRotato, 0, sizeof( NET_CHNN_MIRROR ) );
				MESSAGE_DATA msgData;
				CMessageMan* pMsgMan = CMessageMan::Instance();

				chnnRotato.chnn=chnn;
				chnnRotato.rotato=m_pSetCamParaChange[chnn].m_rotato;
				pMsgMan->SendMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_SAVE_IMAGE_ROTATO,(unsigned char *)&chnnRotato, sizeof(NET_CHNN_ROTATO), msgData);
				pMsgMan->ReleaseMsgBuff( msgData );

				m_pSetCamPara[chnn].m_rotato = m_pSetCamParaChange[chnn].m_rotato;
			} */
			//dirtortion
			if ( memcmp( &(m_pSetCamParaChange[chnn].m_lensDistortion), &(m_pSetCamPara[chnn].m_lensDistortion),sizeof(LENS_DISTORTION_CONFIG)) )
			{
				LENS_DISTORTION distortion;
				memset(&distortion,0,sizeof(LENS_DISTORTION));
				
				distortion.ucLensDistortionValue = m_pSetCamPara[chnn].m_lensDistortion.ucLensDistortionValue;
				distortion.ucLensDistortion = m_pSetCamPara[chnn].m_lensDistortion.ucLensDistortion;
				distortion.noUse[0] = m_pSetCamPara[chnn].m_lensDistortion.noUse[0];
				distortion.noUse[1] = m_pSetCamPara[chnn].m_lensDistortion.noUse[1];

				if(( 0 <= m_curSel )&&( m_curSel < m_localChnn+m_netChnn ))
				{
					distortion.chnn = m_curSel;
				}
				else if( -1 == m_curSel ) 
				{
					//用通道的最大值来代表所有通道
					distortion.chnn = m_localChnn+m_netChnn;
				}
				CMessageMan::Instance()->PostMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID,CMD_REQUEST_SET_IMAGE_DISTORTION,(unsigned char *)&distortion,sizeof(LENS_DISTORTION));
				m_pSetCamParaChange[chnn].m_lensDistortion.ucLensDistortionValue= m_pSetCamPara[chnn].m_lensDistortion.ucLensDistortionValue;
				m_pSetCamParaChange[chnn].m_lensDistortion.ucLensDistortion = m_pSetCamPara[chnn].m_lensDistortion.ucLensDistortion;
				m_pSetCamParaChange[chnn].m_lensDistortion.noUse[0] = m_pSetCamPara[chnn].m_lensDistortion.noUse[0];
				m_pSetCamParaChange[chnn].m_lensDistortion.noUse[1] = m_pSetCamPara[chnn].m_lensDistortion.noUse[1];
			}
		}
	return OnCancel();
}


//void CSetCamParaDlg::OnPaint(PAINT_STATUS status)
//{
//	GUI::CWnd::RefreshLive();
//
//	if (m_bShowPanel)
//	{
//		unsigned short left = m_rect.m_left + m_rectPanel.m_left;
//		unsigned short right = m_rect.m_left + m_rectPanel.m_left + m_rectPanel.Width() - 1;
//		unsigned short top = m_rect.m_top + m_rectPanel.m_top;
//		unsigned short bottom = m_rect.m_top + m_rectPanel.m_top + m_rectPanel.Height() - 1;
//
//		GUI::CWnd::DrawRect(left, top, m_rectPanel.Width(), m_rectPanel.Height(), COLOR_DLG_CHILD_BK);
//
//		//上横线
//		GUI::CWnd::DrawLine(left, top, right, top, COLOR_LINE_GRAY, 1);
//		//左竖线
//		GUI::CWnd::DrawLine(left, top, left, bottom, COLOR_LINE_GRAY, 1);
//		//右竖线
//		GUI::CWnd::DrawLine(right, top, right, bottom, COLOR_LINE_BLACK, 1);
//		//下横线
//		GUI::CWnd::DrawLine(left, bottom, right, bottom, COLOR_LINE_BLACK, 1);
//		//中间横线
//		GUI::CWnd::DrawLine(left, bottom-30, right, bottom-30, COLOR_LINE_GRAY, 1);
//
//		//
//		GUI::CWnd::DrawLine(left, top+38 * m_groupCount + 4, right, top + 38 * m_groupCount + 4, COLOR_LINE_GRAY, 1);
//		if( m_chnnChngWdth > 6 )
//		{
//			GUI::CWnd::DrawLine(left + m_chnnChngWdth - 3, top,  left + m_chnnChngWdth - 3, top + 38 * m_groupCount + 4 , COLOR_LINE_GRAY, 1);
//			GUI::CWnd::DrawLine(right - m_chnnChngWdth + 2 , top, right- m_chnnChngWdth + 2 , top + 38 * m_groupCount + 4, COLOR_LINE_GRAY, 1);
//		}
//
//	}
//}
//
//void CSetCamParaDlg::ShowPanel(bool bShow, bool bRefresh)
//{
//	for (WND_LIST::iterator iter = m_childList.begin(); iter != m_childList.end(); ++iter)
//	{
//		if( !bShow )		//隐藏时隐藏所有
//		{
//			(*iter)->Show(bShow, false);
//		}
//		else				//显示时不刷新处TIP框
//		{
//			if( ( &m_picTip != *iter ) && ( &m_scTip != *iter ) )			//TIP提示图标和提示框不刷新出来
//			{
//				(*iter)->Show(bShow, false);
//			}
//		}
//	}
//
//	m_bShowPanel = bShow;
//	if (bRefresh)
//	{
//		Repaint();
//	}
//}
//
//bool CSetCamParaDlg::GetCurDisplayInfo(NET_DISPLAY_INFO& disp )
//{
//	bool ret = false;
//
//	MESSAGE_DATA msgData;
//	memset(&msgData, 0, sizeof(MESSAGE_DATA));
//	CMessageMan::Instance()->SendMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_GET_DISPLAY_INFO, NULL, 0, msgData);
//	assert(NULL != msgData.pData);
//	PACKCMD *pCmd = reinterpret_cast<PACKCMD *>(msgData.pData);
//	if (CMD_REPLY_CTRL_SUCC == static_cast<NET_PROTOCOL>(pCmd->cmdType))
//	{
//		//获取当前显示的通道信息
//		NET_DISPLAY_INFO* pDispInfo = reinterpret_cast<NET_DISPLAY_INFO*>(msgData.pData + sizeof(PACKCMD));
//
//		memcpy(&disp, pDispInfo, sizeof(NET_DISPLAY_INFO));
//
//		ret = true;
//	}
//	CMessageMan::Instance()->ReleaseMsgBuff(msgData);
//
//	return ret;
//}
//
//BEGIN_MSG_MAP(CSetCamParaDlg, GUI::CDialog)
//	//改变现场通道显示
//	ON_MSG_CTRL_RANGE(GUI::CTRLID_CHANNEL_LIVE0, GUI::CTRLID_CHANNEL_LIVE31,KEY_VIRTUAL_ENTER, OnClickChangeChnnBtn)
//	ON_MSG_KEY_RANGE(GUI::CTRLID_NULL, KEY_VIRTUAL_0, KEY_VIRTUAL_32,OnChangeChnnToNum)
//	ON_MSG( GUI::CTRLID_CHANNEL_LIVE_ALL, KEY_VIRTUAL_ENTER, OnClickChnnAllBtn)
//END_MSG_MAP()
//
//
//unsigned long CSetCamParaDlg::OnClickChangeChnnBtn(unsigned long ctrlID)
//{
//	if( (ctrlID >= CTRLID_CHANNEL_LIVE0) && (CTRLID_CHANNEL_LIVE31 >= ctrlID) && (NULL != m_pBtnChnn) )
//	{
//		unsigned char chnn = ctrlID - CTRLID_CHANNEL_LIVE0;
//		ULONGLONG CH = (0x01 << chnn);
//
//		NET_DISPLAY_INFO  curDisplayInfo;
//		if (GetCurDisplayInfo(curDisplayInfo))
//		{
//			if ( (chnn != CMessageMan::Instance()->CurrPTZChnn()) || (VIEW_SPLIT_1X1 != curDisplayInfo.split_mode[DISPLAY_LAYER_ANALOG]) )
//			{
//				//改变当前控制的通道
//				CMessageMan::Instance()->SetCurrPTZChnn(chnn);
//
//				//切换到该通道的大画面
//				ChangeToChnn( CH, VIEW_SPLIT_1X1 );
//
//				////修改控件上的字符串
//				//ChangeBtnCaption(true);
//
//				////设置当前通道button状态
//				//SetChnnBtnMark(chnn, COLOR_SELECT, true);
//
//				GetRootWnd()->Repaint();
//			}
//
//		}
//
//	}
//	else
//	{
//		printf("%s:%s:%d, 通道号错误 channel = %d\n", __FUNCTION__, __FILE__, __LINE__, ctrlID - CTRLID_CHANNEL_LIVE0);
//		assert(false);
//	}
//
//	return KEY_VIRTUAL_MSG_STOP;
//}
//
//
//
//unsigned long CSetCamParaDlg::OnChangeChnnToNum( unsigned long keyVirtualNum)
//{
//	unsigned long chnn = ( keyVirtualNum & 0x3f );
//	if (0 == chnn)
//	{
//		chnn = (10 - 1);
//	}
//	else
//	{
//		chnn -= 1;
//	}
//
//	ULONGLONG CH = 0x01 << chnn;
//
//	if(0 == ((g_p_login_succ->authPTZCtrlCH) & CH) || !m_pBtnChnn[chnn].IsEnable())
//	{
//		return KEY_VIRTUAL_MSG_STOP;
//	}
//
//	//2011-03-02 过滤当输入按键板值大于当前通道数
//	if((0x00 <= chnn) && (chnn < g_p_login_succ->productInfo.videoInputNum) && (NULL != m_pBtnChnn))
//	{
//		NET_DISPLAY_INFO  curDisplayInfo;
//		if (GetCurDisplayInfo(curDisplayInfo))
//		{
//			if ( (chnn != CMessageMan::Instance()->CurrPTZChnn()) || (VIEW_SPLIT_1X1 != curDisplayInfo.split_mode[DISPLAY_LAYER_ANALOG]))
//			{
//				CMessageMan::Instance()->SetCurrPTZChnn(chnn);
//
//				ChangeToChnn( CH, VIEW_SPLIT_1X1 );
//
//				/*ChangeBtnCaption(true);*/
//
//				SetChnnBtnMark(chnn, COLOR_SELECT, false);
//
//				GetRootWnd()->Repaint();
//			}
//		}
//	}
//	else
//	{
//		printf("%s:%s:%d, 通道号错误 channel = %d\n", __FUNCTION__, __FILE__, __LINE__, chnn);
//		//assert(false);
//	}
//
//	return KEY_VIRTUAL_MSG_STOP;
//}
//unsigned long CSetCamParaDlg::OnClickChnnAllBtn()
//{
//	ULONGLONG	CH = ALL_CHNN_MASK(g_p_login_succ->productInfo.videoInputNum);
//
//	VIEW_SPLIT_MODE splitMode;
//	if( 4 >= g_p_login_succ->productInfo.videoInputNum)
//	{
//		splitMode = VIEW_SPLIT_2X2;
//	}
//	else if( 9 >= g_p_login_succ->productInfo.videoInputNum)
//	{
//		splitMode = VIEW_SPLIT_3X3;
//	}
//	else if( 16 >= g_p_login_succ->productInfo.videoInputNum)
//	{
//		splitMode =  VIEW_SPLIT_4X4;
//	}
//	else if( 24 >= g_p_login_succ->productInfo.videoInputNum)
//	{
//		splitMode = VIEW_SPLIT_4X6;
//	}
//	else if( 25 >= g_p_login_succ->productInfo.videoInputNum)
//	{
//		splitMode = VIEW_SPLIT_5X5;
//	}
//	else if( 32 >= g_p_login_succ->productInfo.videoInputNum)
//	{
//		splitMode = VIEW_SPLIT_6X6;
//	}
//
//	ChangeToChnn( CH, splitMode);
//	GetRootWnd()->Repaint();
//
//	return KEY_VIRTUAL_MSG_STOP;
//}
//
//void CSetCamParaDlg::ChangeToChnn(ULONGLONG CH, VIEW_SPLIT_MODE splitMode)
//{
//	NET_DISPLAY_INFO displayInfo;
//	memset(&displayInfo,0,sizeof(NET_DISPLAY_INFO));
//	displayInfo.type = NET_DISPLAY_CHANGE_DATA_PACK;
//	displayInfo.rec = 0;
//	displayInfo.time_out = -1;
//
//	int index = g_p_login_succ->productInfo.localVideoInputNum;
//	for(int i = index; i < g_p_login_succ->productInfo.videoInputNum; i++)
//	{
//		if(CH & (static_cast<ULONGLONG>(0x1) << i))
//		{
//			displayInfo.split_mode[DISPLAY_LAYER_NET] = splitMode;
//			displayInfo.data[index] = DISPLAY_PACK(DISPLAY_LAYER_NET, DISPLAY_SIGNAL_NET, index, i-g_p_login_succ->productInfo.localVideoInputNum);
//			index++;
//		}
//	}
//
//	displayInfo.data_len = index;
//
//	MESSAGE_DATA msgData;
//	memset(&msgData, 0, sizeof(MESSAGE_DATA));
//	CMessageMan::Instance()->SendMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_CHANGE_DISPLAY_MODE, (unsigned char*)&displayInfo, sizeof(NET_DISPLAY_INFO), msgData);	
//	CMessageMan::Instance()->ReleaseMsgBuff(msgData);
//
//}
//
//void CSetCamParaDlg::SetChnnBtnMark(unsigned long chnn, unsigned long btnColor, bool brefrash)
//{
//	int i = 0, j = 0;
//	for(int i=0; i<(m_chnnNumTemp + 1) * m_groupCount; i++)
//	{
//		m_pBtnChnn[i].SetMark(false);
//	}
//
//	int group = 1;
//	do{
//		if(chnn < (m_chnnNumTemp+1)*group-1)
//		{
//			//是第一排通道按钮
//			break;
//		}
//		else
//		{
//			//不是第一排通道按钮
//			group++;
//			assert(group<=m_groupCount);
//		}
//
//	}while(group<=m_groupCount);
//}
//
//unsigned long CSetCamParaDlg::OnBtnHide()
//{
//	if (m_bShowPanel)
//	{
//		ShowPanel(false, true);
//	} 
//	else
//	{
//		ShowPanel(true, true);
//	}
//
//	return KEY_VIRTUAL_MSG_STOP;
//}
//
//unsigned long CSetCamParaDlg::OnBtnExit()
//{
//	CDialog::EndDialog(DLG_ESC);
//	return KEY_VIRTUAL_MSG_STOP;
//}