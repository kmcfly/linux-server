/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : hz
** Date         : 2009-08-01
** Name         : PTZViewDlg.cpp 
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/
#include "PTZViewDlg.h"
#include "MessageMan.h"
#include "dvrdvsdef.h"
#include "UserMan.h"
#include "FuncCustom.h"


extern NET_LOGIN_SUCCESS_REPLY *g_p_login_succ;

#ifdef __DVR_PROFESSIONAL__
const unsigned long MOUSE_CTRL_OFFSET = 20;
#endif

ULONGLONG CPTZViewDlg::m_ctrlChnn = 0;

CPTZViewDlg::CPTZViewDlg() : m_bShowPanel(true), m_ptzCtrlMode(PTZ_CTRL_NULL), m_pCtrlDlg(NULL)
{
#ifdef __DVR_PROFESSIONAL__
	m_mouseCtrlStatus = PTZ_MOUSE_CTRL_STOP;
	m_xPosStart = 0;
	m_yPosStart = 0;
#endif
	m_pBtnChnn = NULL;
	m_channelTemp = 0;
	m_groupCount  = 0;
	m_chnnChngWdth = 0;
}

CPTZViewDlg::~CPTZViewDlg()
{
	if(NULL != m_pBtnChnn)
	{
		delete [] m_pBtnChnn;
		m_pBtnChnn = NULL;
	}
}

//////////////////////////////////////////////////////////////////////////
void CPTZViewDlg::OnInitial()
{
	printf("!**!PTZ %s, %s, %d\n", __FUNCTION__, __FILE__, __LINE__);
	GUI::CDialog::OnInitial();
	m_style |= WND_NO_TITLE;
	assert(NULL != m_pCtrlDlg);
	unsigned long channelNum = g_p_login_succ->productInfo.videoInputNum;//localVideoInputNum;//
	unsigned long localChannel = g_p_login_succ->productInfo.localVideoInputNum;
	m_channelTemp = channelNum;
	m_groupCount = 1;

	unsigned short width = 36, height = 37;
	m_chnnChngWdth = 0;						//第一个通道按钮右移长度
	unsigned short changeWidth = 0;			//m_pCtrlDlg对话框右移长度
	
	//////////////////////////////////////////////////////////////////////////
	//计算面板尺寸
	if(16 < channelNum )
	{
		//大于16个通道的时候先默认使用两行按键
		if(channelNum % 2)
		{
			m_channelTemp = channelNum / 2 + 1;
		}
		else
		{
			m_channelTemp = channelNum / 2;
		}
		m_groupCount = 2;
		
	}

	if( ( CPtzControlDlg::WIDTH + m_rectCtrlDlg.Width() ) > (width * ( m_channelTemp + 1) ))
	{
		//4路，8路通道按钮要居中排放，32路的时候要放两排
		m_rectPanel.SetSize(/*540*/CPtzControlDlg::WIDTH + m_rectCtrlDlg.Width(), CPtzControlDlg::HEIGHT + (height + 1) * m_groupCount + 6 );

		m_chnnChngWdth = ( CPtzControlDlg::WIDTH + m_rectCtrlDlg.Width() - width * (m_channelTemp + 1)  - m_channelTemp) / 2;
		changeWidth = 0;
	}
	else
	{
		m_rectPanel.SetSize(/*540*/( (width + 1) * ( m_channelTemp + 1 ) + 2 ), CPtzControlDlg::HEIGHT + (height + 1) * m_groupCount + 6);
		m_chnnChngWdth = 0;
		changeWidth = ( (( width + 1 ) * (m_channelTemp + 1) ) - CPtzControlDlg::WIDTH - m_rectCtrlDlg.Width() ) / 2 - 1;
	}



	unsigned short offset_y = 5;
	if (576 == m_height)
	{
		offset_y += 20; 
	}

	m_rectPanel.MoveToXY((m_width - m_rectPanel.Width())/2, m_height - m_rectPanel.Height() - offset_y);
	//////////////////////////////////////////////////////////////////////////
	int x = m_rectPanel.m_left + m_chnnChngWdth;
	int y = m_rectPanel.m_top + 2;//m_rect.Height() - CPtzControlDlg::HEIGHT;
	m_pBtnChnn = new GUI::CButton [(m_channelTemp + 1) * m_groupCount];

	int i = 0;
	int j = 0;
	char strValue[4] = {0};
	unsigned long ctrlID = CTRLID_CHANNEL_LIVE0;

	for(i=0; i<m_groupCount; i++)
	{
		for(j=0; j<m_channelTemp; j++)
		{
			snprintf(strValue, sizeof(strValue), "%02d", i*m_channelTemp + j + 1);
			m_pBtnChnn[i*(m_channelTemp+1)+j].Create(ctrlID + i*(m_channelTemp)+j, x, y, width, height, this, j+1,i, strValue, WND_CHILD | WND_VISIBLE | WND_NO_IMAGE_BK);
			
			if( !((m_ctrlChnn >> (i*(m_channelTemp)+j)) & 0x0000000000000001))
			{
				//该通道没有控制PTZ权限
				m_pBtnChnn[i*(m_channelTemp+1)+j].SetEnable(false, false);	
			}

			x += width+ 1;
		}
		
		if((m_groupCount-1) > i)
		{
			snprintf(strValue, sizeof(strValue), "%s", "");
			m_pBtnChnn[i*(m_channelTemp+1)+j].Create(CTRLID_NULL, x, y, width, height, this, j+1, i, strValue, WND_CHILD | WND_VISIBLE | WND_NO_IMAGE_BK);
			m_pBtnChnn[i*(m_channelTemp+1)+j].SetEnable(false, false);

			x = m_rectPanel.m_left + m_chnnChngWdth;
			y +=  height + 1;
		}
	}


	snprintf(strValue, sizeof(strValue), "%s", "All");
	m_pBtnChnn[(m_channelTemp + 1) *m_groupCount-1].Create(CTRLID_CHANNEL_LIVE_ALL, x, y, width, height, this, j+1, i-1, strValue, WND_CHILD | WND_VISIBLE | WND_NO_IMAGE_BK);
	
	x = m_rectPanel.m_left;
	y = m_rectPanel.m_top + height * m_groupCount + 9;

	m_ptzControlDlg.Create(GetRandWndID(), x, y, CPtzControlDlg::WIDTH, CPtzControlDlg::HEIGHT, this, 0, i);


	x += CPtzControlDlg::WIDTH + changeWidth;
	m_pCtrlDlg->Create(GetRandWndID(), x, y, m_rectCtrlDlg.Width(), m_rectCtrlDlg.Height(), this, 1, i);

	////
	int offset_x = 40;
	int btn_cx = 0;
	int btn_cy = 0;
	GetImageSize(BMP_BTN_EXIT2, btn_cx, btn_cy);
	y = m_rect.Height() - btn_cy - offset_y - 3;
	x = m_rectPanel.m_right - offset_x;

	m_btnExit.Create(GetRandWndID(), x, y, btn_cx, btn_cy, this, 1, i+1);
	m_btnExit.SetImage(PAINT_STATUS_NORMAL, BMP_BTN_EXIT2, false);
	m_btnExit.SetImage(PAINT_STATUS_FOCUS, BMP_BTN_EXIT2_FOCUS, false);
	m_btnExit.SetImage(PAINT_STATUS_DISABLE, BMP_BTN_EXIT2, false);

	x -= offset_x;
	m_btnHide.Create(GetRandWndID(), x, y, btn_cx, btn_cy, this, 0, i+1);
	m_btnHide.SetImage(PAINT_STATUS_NORMAL, BMP_BTN_PLAY_HIDE, false);
	m_btnHide.SetImage(PAINT_STATUS_FOCUS, BMP_BTN_PLAY_HIDE_FOCUS, false);
	m_btnHide.SetImage(PAINT_STATUS_DISABLE, BMP_BTN_PLAY_HIDE, false);
	
	m_keyMode = KEY_MODE_PTZ;

	/*x -= 5*offset_x;
	m_ptzAutoFcus.Create(GetRandWndID(),x,y,2*btn_cx,btn_cy,this,0,i+1);*/
	

	//zhl 2010-03-03 该对话框为整个屏幕，只有面板框显示，其他地方透明。
	unsigned short left = m_rect.m_left + m_rectPanel.m_left;								//面板框左边界
	unsigned short right = m_rect.m_left + m_rectPanel.m_left + m_rectPanel.Width() - 1;	//面板框右边界
	CreateTipWnd( right - left - CPtzControlDlg::WIDTH - 2 * btn_cx - offset_x, left + CPtzControlDlg::WIDTH + 10 , m_rect.Height() - 30 );
#ifdef __CUSTOM_IL03__
	ChangeTipBKColor(COLOR_DLG_CHILD_BK);
#endif

	printf("!**!PTZ %s, %s, %d\n", __FUNCTION__, __FILE__, __LINE__);

	m_bHaveCtrolPTZ=false;
	SetChnnBtnMark(CMessageMan::Instance()->CurrPTZChnn(),COLOR_SELECT, false );

	//如果是PRESET和TRACK配置界面就大画面到当前选择的通道
	switch( m_CurrPtzCtrlDlgNum )
	{
	case CURR_PTZCTRLDLG_LIVE:
		{
			break;
		}
	case CURR_PTZCTRLDLG_TRACK:
		for (int temp = localChannel+1;temp<=channelNum; temp++)
		{
			m_pBtnChnn[temp].SetEnable(false,false);
		}
	case CURR_PTZCTRLDLG_PRESET:
		{
			ULONGLONG CH = 0x01 << CMessageMan::Instance()->CurrPTZChnn();
			ChangeToChnn( CH, VIEW_SPLIT_1X1 );
			break;
		}
	case CURR_PTZCTRLDLG_NULL:
		break;
	default:
		break;
	}
printf("!**!PTZ %s, %s, %d\n", __FUNCTION__, __FILE__, __LINE__);
}

void CPTZViewDlg::OnDestroy()
{
	GUI::CDialog::OnDestroy();

	m_bShowPanel = true;
}

void CPTZViewDlg::OnPaint(PAINT_STATUS status)
{
	GUI::CWnd::RefreshLive();

	if (m_bShowPanel)
	{
		unsigned short left = m_rect.m_left + m_rectPanel.m_left;
		unsigned short right = m_rect.m_left + m_rectPanel.m_left + m_rectPanel.Width() - 1;
		unsigned short top = m_rect.m_top + m_rectPanel.m_top;
		unsigned short bottom = m_rect.m_top + m_rectPanel.m_top + m_rectPanel.Height() - 1;

		GUI::CWnd::DrawRect(left, top, m_rectPanel.Width(), m_rectPanel.Height(), COLOR_DLG_CHILD_BK);

		//上横线
		GUI::CWnd::DrawLine(left, top, right, top, COLOR_LINE_GRAY, 1);
		//左竖线
		GUI::CWnd::DrawLine(left, top, left, bottom, COLOR_LINE_GRAY, 1);
		//右竖线
		GUI::CWnd::DrawLine(right, top, right, bottom, COLOR_LINE_BLACK, 1);
		//下横线
		GUI::CWnd::DrawLine(left, bottom, right, bottom, COLOR_LINE_BLACK, 1);
		//中间横线
		GUI::CWnd::DrawLine(left, bottom-30, right, bottom-30, COLOR_LINE_GRAY, 1);

		//
		GUI::CWnd::DrawLine(left, top+38 * m_groupCount + 4, right, top + 38 * m_groupCount + 4, COLOR_LINE_GRAY, 1);
		if( m_chnnChngWdth > 6 )
		{
			GUI::CWnd::DrawLine(left + m_chnnChngWdth - 3, top,  left + m_chnnChngWdth - 3, top + 38 * m_groupCount + 4 , COLOR_LINE_GRAY, 1);
			GUI::CWnd::DrawLine(right - m_chnnChngWdth + 2 , top, right- m_chnnChngWdth + 2 , top + 38 * m_groupCount + 4, COLOR_LINE_GRAY, 1);
		}

	}
}

void CPTZViewDlg::ShowPanel(bool bShow, bool bRefresh)
{
	for (WND_LIST::iterator iter = m_childList.begin(); iter != m_childList.end(); ++iter)
	{
		if( !bShow )		//隐藏时隐藏所有
		{
			(*iter)->Show(bShow, false);
		}
		else				//显示时不刷新处TIP框
		{
			if( ( &m_picTip != *iter ) && ( &m_scTip != *iter ) )			//TIP提示图标和提示框不刷新出来
			{
				(*iter)->Show(bShow, false);
			}
		}

	}

	m_bShowPanel = bShow;
	if (bRefresh)
	{
		Repaint();
	}

}

void CPTZViewDlg::SetCtrlDlg(GUI::CDialog* pCtrlDlg, int width, int height ,CURR_PTZCTRLDLG_NUM curCtrlDlgNum)
{
	m_pCtrlDlg = pCtrlDlg;
	if (height > CTRLDLG_MAX_HEIGHT)
	{
		height = CTRLDLG_MAX_HEIGHT;
	}
	m_rectCtrlDlg.SetRect(0, 0, width, height);

	m_CurrPtzCtrlDlgNum = curCtrlDlgNum;
}

/**************************************************************************************************
Function:GetPTZStatus( unsigned long chnn ) 获取通道chnn中云台的状态
参数：chnn 通道号 0-15
返回值:通道chnn云台的当前状态
作者：ZHL 2010-02-24


***************************************************************************************************/
unsigned long CPTZViewDlg::GetPTZStatus( unsigned long chnn )
{
	unsigned long status = 0;

	MESSAGE_DATA msgData;
	memset(&msgData, 0, sizeof(MESSAGE_DATA));
	CMessageMan::Instance()->SendMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_PTZ_STATUS, reinterpret_cast< unsigned char *>( &chnn ), sizeof( unsigned long ), msgData);

	assert( NULL != msgData.pData );
	PACKCMD *pPackcmd = reinterpret_cast< PACKCMD * >( msgData.pData );
	if( CMD_REPLY_PTZ_STATUS_SUC == pPackcmd->cmdType )
	{
		status = *reinterpret_cast< unsigned long * >( msgData.pData + sizeof( PACKCMD ) );
	}
	CMessageMan::Instance()->ReleaseMsgBuff( msgData );

	return status;
	
}


ENUM_STRING_ID CPTZViewDlg::GetPtzCtrlStrID(unsigned long errPtz)
{
	ENUM_STRING_ID strID = E_STR_ID_NULL;
	switch(errPtz)
	{
		//PTZ_ERR_OK = 0,
	case	PTZ_ERR_WRITE_COM:		//写串口失败
		strID = E_STR_ID_MESSAGE_92;
		break;
	case	PTZ_ERR_UF_PROTOCOL:	//没有找到协议集
		strID = E_STR_ID_MESSAGE_93;
		break;
	case	PTZ_ERR_CHNN_LOCK:		//通道被锁定
		strID = E_STR_ID_MESSAGE_94;
		break;
	case	PTZ_ERR_CHNN_UN_INTERRUPT://不能打断通道操作
		strID = E_STR_ID_MESSAGE_95;
		break;
	case	PTZ_ERR_CHNN_UN_SET:	//没有找到相关通道设置
		strID = E_STR_ID_MESSAGE_96;
		break;
	case	PTZ_ERR_CHNN_UN_ENABLE: //未启用通道
		strID = E_STR_ID_MESSAGE_97;
		break;
	case	PTZ_ERR_CHNN_WAITING: //通道处于等待中
		strID = E_STR_ID_MESSAGE_98;
		break;
	case	PTZ_ERR_PRESET_DISABLE:	//预置点没有使用
		strID = E_STR_ID_MESSAGE_107;
		break;
	case	PTZ_ERR_OPERATE_FAIL: //有人在设置PTZ不能进行操作
		strID = E_STR_ID_MESSAGE_112;
		break;
	default:
		break;

	}
	
	return strID;
}


BEGIN_MSG_MAP(CPTZViewDlg, GUI::CDialog)
	ON_MSG( m_btnHide.GetID(), KEY_VIRTUAL_ENTER, OnBtnHide )
	ON_MSG( m_btnExit.GetID(), KEY_VIRTUAL_ENTER, OnBtnExit )
	ON_MSG( GUI::CTRLID_NULL, KEY_VIRTUAL_HIDE, OnBtnHide )

	ON_MSG( GUI::CTRLID_NULL, KEY_VIRTUAL_ZOOM, OnZoom)
	ON_MSG( GUI::CTRLID_NULL, KEY_VIRTUAL_FOCUS, OnFocus)
	ON_MSG( GUI::CTRLID_NULL, KEY_VIRTUAL_IRIS, OnIris)
	ON_MSG( GUI::CTRLID_NULL, KEY_VIRTUAL_SPEED, OnSpeed)

	//当隐藏该对话框时响应云台控制
	ON_MSG_EX( GUI::CTRLID_NULL, keyID, OnPTZCtrl )
	//当操做云台基本控制时刷新现场中云台控制中控件上的字符串，这仅使用于PTZLive.cpp界面
	ON_MSG_KEY_RANGE(GUI::CTRLID_NULL, KEY_VIRTUAL_FOCUS_ADD, KEY_VIRTUAL_PTZ_RIGHTDOWN,OnRefreshBtnCaption)

	//改变现场通道显示
	ON_MSG_CTRL_RANGE(GUI::CTRLID_CHANNEL_LIVE0, GUI::CTRLID_CHANNEL_LIVE31,KEY_VIRTUAL_ENTER, OnClickChangeChnnBtn)
	ON_MSG_KEY_RANGE(GUI::CTRLID_NULL, KEY_VIRTUAL_0, KEY_VIRTUAL_32,OnChangeChnnToNum)
	ON_MSG( GUI::CTRLID_CHANNEL_LIVE_ALL, KEY_VIRTUAL_ENTER, OnClickChnnAllBtn)

END_MSG_MAP()

unsigned long CPTZViewDlg::OnBtnHide()
{
	if (m_bShowPanel)
	{
		ShowPanel(false, true);
	} 
	else
	{
		ShowPanel(true, true);
	}

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CPTZViewDlg::OnBtnExit()
{
	CDialog::EndDialog(DLG_ESC);
	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CPTZViewDlg::OnZoom()
{
	m_ptzCtrlMode = PTZ_CTRL_ZOOM;
	m_ptzControlDlg.SetPTZFuncMode(PTZ_FUNC_ZOOM);
	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CPTZViewDlg::OnFocus()
{
	m_ptzCtrlMode = PTZ_CTRL_FOCUS;
	m_ptzControlDlg.SetPTZFuncMode(PTZ_FUNC_FOCUS);
	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CPTZViewDlg::OnIris()
{
	m_ptzCtrlMode = PTZ_CTRL_IRIS;
	m_ptzControlDlg.SetPTZFuncMode(PTZ_FUNC_IRIS);
	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CPTZViewDlg::OnSpeed()
{
	m_ptzCtrlMode = PTZ_CTRL_SPEED;
	m_ptzControlDlg.SetPTZFuncMode(PTZ_FUNC_SPEED);
	return KEY_VIRTUAL_MSG_STOP;
}


unsigned long CPTZViewDlg::OnClickChangeChnnBtn(unsigned long ctrlID)
{
	if( (ctrlID >= CTRLID_CHANNEL_LIVE0) && (CTRLID_CHANNEL_LIVE31 >= ctrlID) && (NULL != m_pBtnChnn) )
	{
		unsigned char chnn = ctrlID - CTRLID_CHANNEL_LIVE0;
		ULONGLONG CH = (0x01 << chnn);

		NET_DISPLAY_INFO  curDisplayInfo;
		if (GetCurDisplayInfo(curDisplayInfo))
		{
			if ( (chnn != CMessageMan::Instance()->CurrPTZChnn()) || (VIEW_SPLIT_1X1 != curDisplayInfo.split_mode[DISPLAY_LAYER_ANALOG]) )
			{
				//改变当前控制的通道
				CMessageMan::Instance()->SetCurrPTZChnn(chnn);

				//切换到该通道的大画面
				ChangeToChnn( CH, VIEW_SPLIT_1X1 );

				//修改控件上的字符串
				ChangeBtnCaption(true);

				//设置当前通道button状态
				SetChnnBtnMark(chnn, COLOR_SELECT, true);

				GetRootWnd()->Repaint();
			}

		}
		
	}
	else
	{
		printf("%s:%s:%d, 通道号错误 channel = %d\n", __FUNCTION__, __FILE__, __LINE__, ctrlID - CTRLID_CHANNEL_LIVE0);
		assert(false);
	}

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CPTZViewDlg::OnChangeChnnToNum( unsigned long keyVirtualNum)
{
	unsigned long chnn = ( keyVirtualNum & 0x3f );
	if (0 == chnn)
	{
		chnn = (10 - 1);
	}
	else
	{
		chnn -= 1;
	}

	ULONGLONG CH = 0x01 << chnn;

	if(0 == ((g_p_login_succ->authPTZCtrlCH) & CH) || !m_pBtnChnn[chnn].IsEnable())
	{
		return KEY_VIRTUAL_MSG_STOP;
	}

	//2011-03-02 过滤当输入按键板值大于当前通道数
	if((0x00 <= chnn) && (chnn < g_p_login_succ->productInfo.videoInputNum) && (NULL != m_pBtnChnn))
	{
		NET_DISPLAY_INFO  curDisplayInfo;
		if (GetCurDisplayInfo(curDisplayInfo))
		{
			if ( (chnn != CMessageMan::Instance()->CurrPTZChnn()) || (VIEW_SPLIT_1X1 != curDisplayInfo.split_mode[DISPLAY_LAYER_ANALOG]))
			{
				CMessageMan::Instance()->SetCurrPTZChnn(chnn);

				ChangeToChnn( CH, VIEW_SPLIT_1X1 );

				ChangeBtnCaption(true);

				SetChnnBtnMark(chnn, COLOR_SELECT, false);

				GetRootWnd()->Repaint();
			}
		}

	}
	else
	{
		printf("%s:%s:%d, 通道号错误 channel = %d\n", __FUNCTION__, __FILE__, __LINE__, chnn);
		//assert(false);
	}

	return KEY_VIRTUAL_MSG_STOP;
}
unsigned long CPTZViewDlg::OnClickChnnAllBtn()
{
	unsigned short videoInputNum = IsSupportHybridDisplay()?g_p_login_succ->productInfo.videoInputNum:g_p_login_succ->productInfo.localVideoInputNum;
	ULONGLONG	CH = ALL_CHNN_MASK(videoInputNum);
	
	VIEW_SPLIT_MODE splitMode;
	if( 4 >= videoInputNum)
	{
		splitMode = VIEW_SPLIT_2X2;
	}
	else if (5 >= videoInputNum)
	{
		splitMode = VIEW_SPLIT_2X3;
	}
	else if( 9 >= videoInputNum)
	{
		splitMode = VIEW_SPLIT_3X3;
	}
	else if( 16 >= videoInputNum)
	{
		splitMode =  VIEW_SPLIT_4X4;
	}
	else if( 24 >= videoInputNum)
	{
		splitMode = VIEW_SPLIT_4X6;
	}
	else if( 25 >= videoInputNum)
	{
		splitMode = VIEW_SPLIT_5X5;
	}
	else if( 32 >= videoInputNum)
	{
		splitMode = VIEW_SPLIT_6X6;
	}

	ChangeToChnn( CH, splitMode);
	GetRootWnd()->Repaint();
	
	return KEY_VIRTUAL_MSG_STOP;
}

void CPTZViewDlg::ChangeToChnn(ULONGLONG CH, VIEW_SPLIT_MODE splitMode)
{
	NET_DISPLAY_INFO displayInfo;
	memset(&displayInfo,0,sizeof(NET_DISPLAY_INFO));
	displayInfo.type = NET_DISPLAY_CHANGE_DATA_PACK;
	displayInfo.rec = 0;
	displayInfo.time_out = -1;
	
	int index = 0;
	int i = 0;
	for (; i < g_p_login_succ->productInfo.localVideoInputNum; i++)
	{
		if(CH & (static_cast<ULONGLONG>(0x1) << i))
		{
			displayInfo.split_mode[DISPLAY_LAYER_ANALOG] = splitMode;
			displayInfo.data[index] = DISPLAY_PACK(DISPLAY_LAYER_ANALOG, DISPLAY_SIGNAL_ANALOG, index, i);
			index++;
		}
	}

	for(; i < g_p_login_succ->productInfo.videoInputNum; i++)
	{
		if(CH & (static_cast<ULONGLONG>(0x1) << i))
		{
			displayInfo.split_mode[DISPLAY_LAYER_NET] = splitMode;
			displayInfo.data[index] = DISPLAY_PACK(DISPLAY_LAYER_NET, DISPLAY_SIGNAL_NET, index, i-g_p_login_succ->productInfo.localVideoInputNum);
			index++;
		}
	}

	displayInfo.data_len = index;

	MESSAGE_DATA msgData;
	memset(&msgData, 0, sizeof(MESSAGE_DATA));
	CMessageMan::Instance()->SendMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_CHANGE_DISPLAY_MODE, (unsigned char*)&displayInfo, sizeof(NET_DISPLAY_INFO), msgData);	
	CMessageMan::Instance()->ReleaseMsgBuff(msgData);

}

void CPTZViewDlg::SetChnnBtnMark(unsigned long chnn, unsigned long btnColor, bool brefrash)
{
	int i = 0, j = 0;
	for(int i=0; i<(m_channelTemp + 1) * m_groupCount; i++)
	{
		m_pBtnChnn[i].SetMark(false);
	}

	int group = 1;
	do
	{
		
		if(chnn < (m_channelTemp+1)*group-1)
		{
			//是第一排通道按钮
			break;
		}
		else
		{
			//不是第一排通道按钮
			group++;
			assert(group<=m_groupCount);
		}
		
	}while(group<=m_groupCount);
	
	
	m_pBtnChnn[(m_channelTemp+1)*(group-1)+chnn%m_channelTemp].SetMark(true, btnColor,brefrash);
}

unsigned long CPTZViewDlg::OnRefreshBtnCaption(unsigned long keyID)
{
	ChangeBtnCaption(false);

	return KEY_VIRTUAL_MSG_STOP;
}

void CPTZViewDlg::ChangeBtnCaption(bool bChnnChanged)
{
	switch( m_CurrPtzCtrlDlgNum )
	{
	case CURR_PTZCTRLDLG_LIVE:
		{
			CPTZLiveCtrl *pPTZLiveCtrl = dynamic_cast<CPTZLiveCtrl *>( m_pCtrlDlg );
			if( NULL != pPTZLiveCtrl )
			{
				pPTZLiveCtrl->ChangeBtnCaption(bChnnChanged);
			}
			break;
		}
	case CURR_PTZCTRLDLG_TRACK:
		{
			CPTZTrackCtrl *pPTZTrackCtrl = dynamic_cast<CPTZTrackCtrl *>(m_pCtrlDlg);
			if(NULL != pPTZTrackCtrl)
			{
				pPTZTrackCtrl->ChangeBtnCaption(bChnnChanged);
			}
			break;
		}
	case CURR_PTZCTRLDLG_PRESET:
		{
			CPTZPresetCtrl *pPTZPresetCtrl = dynamic_cast<CPTZPresetCtrl *>(m_pCtrlDlg);
			if(NULL != pPTZPresetCtrl)
			{
				pPTZPresetCtrl->ChangeBtnCaption(bChnnChanged);
			}
			break;
		}
	case CURR_PTZCTRLDLG_NULL:
		break;
	default:
		break;
	}
}

void CPTZViewDlg::HaveControlPTZ(unsigned long keyID)
{
	if(((KEY_VIRTUAL_FOCUS_ADD <= keyID) && ( keyID < KEY_VIRTUAL_PTZ_STOP)) || ((KEY_VIRTUAL_PTZ_LEFTUP <= keyID)&&(keyID < KEY_VIRTUAL_PTZ_SET_PRESET)) )
	{
		m_bHaveCtrolPTZ = true;
	}
}


unsigned long CPTZViewDlg::OnPTZCtrl( unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos )
{
	//记录是否需要在拖动鼠标后停止云台
	HaveControlPTZ(keyID);

	if(!m_bShowPanel/* && (KEY_VIRTUAL_MOVE != keyID)*/)
	{
#ifdef __DVR_PROFESSIONAL__
		//鼠标控制
		if ((xPos < m_width) && (yPos < m_height))
		{
			if ((KEY_VIRTUAL_ENTER == keyID) && (PTZ_MOUSE_CTRL_STOP == m_mouseCtrlStatus))
			{
				m_xPosStart = xPos;
				m_yPosStart = yPos;
				m_mouseCtrlStatus = PTZ_MOUSE_CTRL_START;

				return KEY_VIRTUAL_MSG_STOP;
			}
			else if ((KEY_VIRTUAL_DRAG == keyID) && (PTZ_MOUSE_CTRL_STOP != m_mouseCtrlStatus))
			{
				unsigned long cx = (m_xPosStart < xPos) ? (xPos - m_xPosStart) : (m_xPosStart - xPos);
				unsigned long cy = (m_yPosStart < yPos) ? (yPos - m_yPosStart) : (m_yPosStart - yPos);

				if ((MOUSE_CTRL_OFFSET < cx) || (MOUSE_CTRL_OFFSET < cy))
				{
					PTZ_CTRL ptzCtrl;
					ptzCtrl.chnn = CMessageMan::Instance()->CurrPTZChnn();
					ptzCtrl.cmdType = PTZCmdType(m_xPosStart, m_yPosStart, xPos, yPos, cx, cy);
					ptzCtrl.speed = PTZCmdSpeed(cx, cy);

					MESSAGE_DATA msgData;
					memset(&msgData, 0, sizeof(MESSAGE_DATA));
					CMessageMan::Instance()->SendMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_PTZ_CTRL, (unsigned char *)&ptzCtrl, sizeof(PTZ_CTRL), msgData);

					CMessageMan::Instance()->ReleaseMsgBuff( msgData );

					m_mouseCtrlStatus = PTZ_MOUSE_CTRL_ING;
				}

				return KEY_VIRTUAL_MSG_STOP;
			}
			else
			{
				//
				if (KEY_VIRTUAL_ADD == keyID)
				{
					PTZ_CTRL ptzCtrl;
					ptzCtrl.chnn = CMessageMan::Instance()->CurrPTZChnn();
					ptzCtrl.cmdType = PTZ_CMD_ZOOM_OUT;
					ptzCtrl.speed = 3;

					MESSAGE_DATA msgData;
					memset(&msgData, 0, sizeof(MESSAGE_DATA));
					CMessageMan::Instance()->SendMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_PTZ_CTRL, (unsigned char *)&ptzCtrl, sizeof(PTZ_CTRL), msgData);

					CMessageMan::Instance()->ReleaseMsgBuff( msgData );
					m_mouseCtrlStatus = PTZ_MOUSE_CTRL_ING;
					PUB_Sleep(50);
				}

				if (KEY_VIRTUAL_DEC == keyID)
				{
					PTZ_CTRL ptzCtrl;
					ptzCtrl.chnn = CMessageMan::Instance()->CurrPTZChnn();
					ptzCtrl.cmdType = PTZ_CMD_ZOOM_IN;
					ptzCtrl.speed = 3;

					MESSAGE_DATA msgData;
					memset(&msgData, 0, sizeof(MESSAGE_DATA));
					CMessageMan::Instance()->SendMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_PTZ_CTRL, (unsigned char *)&ptzCtrl, sizeof(PTZ_CTRL), msgData);

					CMessageMan::Instance()->ReleaseMsgBuff( msgData );
					m_mouseCtrlStatus = PTZ_MOUSE_CTRL_ING;
					PUB_Sleep(50);
				}

				//如果正在运行过程中，则停止云台
				if (PTZ_MOUSE_CTRL_ING == m_mouseCtrlStatus)
				{
					PTZ_CTRL ptzCtrl;
					ptzCtrl.chnn = CMessageMan::Instance()->CurrPTZChnn();
					ptzCtrl.cmdType = PTZ_CMD_STOP;

					MESSAGE_DATA msgData;
					memset(&msgData, 0, sizeof(MESSAGE_DATA));
					CMessageMan::Instance()->SendMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_PTZ_CTRL, (unsigned char *)&ptzCtrl, sizeof(PTZ_CTRL), msgData);

					CMessageMan::Instance()->ReleaseMsgBuff( msgData );
				}

				m_mouseCtrlStatus = PTZ_MOUSE_CTRL_STOP;

				return KEY_VIRTUAL_MSG_STOP;
			}
		}
		else
#endif
		{
			if ((KEY_VIRTUAL_ADD == keyID) || (KEY_VIRTUAL_DEC == keyID))
			{
				switch(m_ptzCtrlMode)
				{
				case PTZ_CTRL_ZOOM:
					keyID = (KEY_VIRTUAL_ADD == keyID) ? KEY_VIRTUAL_ZOOM_ADD : KEY_VIRTUAL_ZOOM_DEC;
					break;
				case PTZ_CTRL_FOCUS:
					keyID = (KEY_VIRTUAL_ADD == keyID) ? KEY_VIRTUAL_FOCUS_ADD : KEY_VIRTUAL_FOCUS_DEC;
					break;
				case PTZ_CTRL_IRIS:
					keyID = (KEY_VIRTUAL_ADD == keyID) ? KEY_VIRTUAL_IRIS_ADD : KEY_VIRTUAL_IRIS_DEC;
					break;
				case PTZ_CTRL_SPEED:
					keyID = (KEY_VIRTUAL_ADD == keyID) ? KEY_VIRTUAL_SPEED_ADD : KEY_VIRTUAL_SPEED_DEC;
					break;
				default:
					break;
				}
			}

			m_ptzControlDlg.ResponseHideCommond( ctrlID, keyID, xPos, yPos);
			m_pCtrlDlg->ResponseHideCommond( ctrlID, keyID, xPos, yPos);

			/*if(((KEY_VIRTUAL_FOCUS_ADD <= keyID) && ( keyID < KEY_VIRTUAL_PTZ_STOP)) || \
				((KEY_VIRTUAL_PTZ_LEFTUP <= keyID) && (keyID < KEY_VIRTUAL_PTZ_SET_PRESET)) )
			{
				PUB_Sleep(50);
				m_ptzControlDlg.ResponseHideCommond(GUI::CTRLID_NULL, KEY_VIRTUAL_PTZ_STOP, xPos, yPos);
				m_pCtrlDlg->ResponseHideCommond(GUI::CTRLID_NULL, KEY_VIRTUAL_PTZ_STOP, xPos, yPos);
			}*/
		}
	}
	else
	{
		//操作云台后拖动鼠标时不能响应鼠标弹起事件，这会造成弹起鼠标时不能停止云台转动
		if(KEY_VIRTUAL_BLUP == keyID)
		{
			if( m_bHaveCtrolPTZ )
			{
				keyID = KEY_VIRTUAL_ENTER;
				m_ptzControlDlg.ResponseHideCommond( CTRLID_PTZCONTROL_STOP, keyID, xPos, yPos);
				m_bHaveCtrolPTZ = false;
			}
		}

		//云台停止时清除m_bHaveCtrolPTZ标志，防止在操作完方向后操作track时由于鼠标左键的弹起而发送停止云台命令，从而导致tarck失败
		if(KEY_VIRTUAL_PTZ_STOP == keyID)
		{
			m_bHaveCtrolPTZ = false;
		}
	}
	
	return keyID;
}

#ifdef __DVR_PROFESSIONAL__
PTZ_CMD_TYPE CPTZViewDlg::PTZCmdType(unsigned long x1, unsigned long y1, unsigned long x2, unsigned long y2, unsigned long cx, unsigned long cy)
{
	if ((cy << 1) <= cx)
	{
		//left or right
		if (x1 < x2)
		{
			return PTZ_CMD_LEFT;
		}
		else
		{
			return PTZ_CMD_RIGHT;
		}
	}
	else if ((cx < (cy << 1)) && (cy < (cx << 1)))
	{
		//left_up,left_down,right_down,right_up
		if ((x1 < x2) && (y1 < y2))
		{
			return PTZ_CMD_LEFT_UP;
		}
		else if ((x1 < x2) && (y1 > y2))
		{
			return PTZ_CMD_LEFT_DOWN;
		}
		else if ((x1 > x2) && (y1 > y2))
		{
			return PTZ_CMD_RIGHT_DOWN;
		}
		else
		{
			return PTZ_CMD_RIGHT_UP;
		}
	}
	else
	{
		//up or down
		if (y1 < y2)
		{
			return PTZ_CMD_UP;
		}
		else
		{
			return PTZ_CMD_DOWN;
		}
	}
}

unsigned long CPTZViewDlg::PTZCmdSpeed(unsigned long cx, unsigned long cy)
{
	unsigned long xSpeed = (cx / (m_width >> 4)) + 1;
	unsigned long ySpeed = (cy / (m_height >> 4)) + 1;

	xSpeed = (8 < xSpeed) ? 8 : xSpeed;
	ySpeed = (8 < ySpeed) ? 8 : ySpeed;

	return (xSpeed > ySpeed) ? xSpeed : ySpeed;
}
#endif

bool CPTZViewDlg::GetCurDisplayInfo(NET_DISPLAY_INFO& disp )
{
	bool ret = false;

	MESSAGE_DATA msgData;
	memset(&msgData, 0, sizeof(MESSAGE_DATA));
	CMessageMan::Instance()->SendMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_GET_DISPLAY_INFO, NULL, 0, msgData);
	assert(NULL != msgData.pData);
	PACKCMD *pCmd = reinterpret_cast<PACKCMD *>(msgData.pData);
	if (CMD_REPLY_CTRL_SUCC == static_cast<NET_PROTOCOL>(pCmd->cmdType))
	{
		//获取当前显示的通道信息
		NET_DISPLAY_INFO* pDispInfo = reinterpret_cast<NET_DISPLAY_INFO*>(msgData.pData + sizeof(PACKCMD));

		memcpy(&disp, pDispInfo, sizeof(NET_DISPLAY_INFO));

		ret = true;
	}
	CMessageMan::Instance()->ReleaseMsgBuff(msgData);

	return ret;
}

/***************************************************************************************
函数：OnPreCommand()
功能：拦截按键消息，判断当前用户是否有权限控制某通道的云台。
主要用在按键板和遥控器按键的判断(数字键)
wn	2011-07-30
***************************************************************************************/
unsigned long CPTZViewDlg::OnPreCommand(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	//--------------2011.7.16-------------
	ULONGLONG authCH = CUserMan::Instance()->CheckAuthority(AUTH_PTZ_CTRL);
	if ((KEY_VIRTUAL_1 <= keyID) && (KEY_VIRTUAL_32 >= keyID))
	{
		unsigned long nTemp = static_cast<unsigned long>(keyID & 0x00FF);

		if (!(static_cast<unsigned long>(authCH>>(nTemp - 1)) & 0x0001))
		{
			return KEY_VIRTUAL_MSG_STOP;
		}
	}
	return GUI::CDialog::OnPreCommand(ctrlID, keyID, xPos, yPos);
}

//end




