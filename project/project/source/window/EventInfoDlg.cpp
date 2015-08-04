/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : ysw
** Date         : 2009-08-06
** Name         : EventInfoDlg.cpp
** Version      : 1.0
** Description  : 
** Modify Record: 2009-09-01
1:
***********************************************************************/
#include "EventInfoDlg.h"
#include "ListCtrlEx.cpp"
#include "TimeCtrl.h"
#include "FuncCustom.h"

#include "NetProtocol.h"
#include "MessageMan.h"
#include "ExportLogList.h"

extern NET_LOGIN_SUCCESS_REPLY *g_p_login_succ;
//////////////////////////////////////////////////////////////////////////
template <class T>
std::string &  GUI::CListCtrlEx<T>::Text(T &node, unsigned short col)
{
	EVENT_LOG &it = node;
	char str [512] = {0};

	if (0 == col)
	{
		sprintf(str, "%d", it.chnn+1);
	}

	if (1 == col)
	{
		if (EVENT_TYPE_MOTION & it.type)
		{
			snprintf(str, 512, "%s", m_pStrTable->String(E_STR_ID_MOTION));
		}

		if (EVENT_TYPE_SENSOR & it.type)
		{
			snprintf(str, 512, "%s", m_pStrTable->String(E_STR_ID_SENSOR));
		}

		if (EVENT_TYPE_V_LOSS & it.type)
		{
			snprintf(str, 512, "%s", m_pStrTable->String(E_STR_ID_VIDEO_LOSS));
		}
	}

	if (2 == col)
	{
		//tm time = DVRTime32ToTm(it.startTime);
		//sprintf(str, "%d-%02d-%02d %02d:%02d:%02d", time.tm_year+1900, time.tm_mon+1, time.tm_mday, time.tm_hour, time.tm_min, time.tm_sec);
		std::string strtime;
		CTimeCtrl::GetTime32Text(strtime,it.startTime);
		strcpy(str,strtime.c_str());
	}

	if (3 == col)
	{
		//tm time = DVRTime32ToTm(it.endTime);
		//sprintf(str, "%d-%02d-%02d %02d:%02d:%02d", time.tm_year+1900, time.tm_mon+1, time.tm_mday, time.tm_hour, time.tm_min, time.tm_sec);
		std::string strtime;
		CTimeCtrl::GetTime32Text(strtime,it.endTime);
		strcpy(str,strtime.c_str());
	}

	m_str = str;
	return m_str;
}
//////////////////////////////////////////////////////////////////////////
CEventInfoDlg::CEventInfoDlg(): m_bFlexSearchConditionDlg(false)
{
	m_pEventLog = NULL;
	m_EventLogNum = 0;
}

CEventInfoDlg::~CEventInfoDlg()
{
	if(NULL != m_pEventLog)
	{
		delete [] m_pEventLog;
		m_pEventLog = NULL;
	}

	m_EventLogNum = 0;
}

//////////////////////////////////////////////////////////////////////////
void CEventInfoDlg::OnInitial()
{
	GUI::CDialog::OnInitial();
	SetImage(BMP_MEM_DLG_LARGE, false);
	SetCaption(m_pStrTable->String(E_STR_ID_TITLE_EVENT_LIST), false);

#ifdef __DVR_PROFESSIONAL__
	SetDefaultBtn( DEFAULT_BTN_OK | DEFAULT_BTN_CANCEL | DEFAULT_BTN_APPLY );
	SetDefaultBtnCaption( DEFAULT_BTN_OK, m_pStrTable->String(E_STR_ID_SEARCH) );
	SetDefaultBtnCaption( DEFAULT_BTN_CANCEL, m_pStrTable->String(E_STR_ID_EXIT) );
	SetDefaultBtnCaption(DEFAULT_BTN_APPLY,m_pStrTable->String(E_STR_ID_EXPORT));
#else
	SetDefaultBtn( DEFAULT_BTN_OK | DEFAULT_BTN_CANCEL);
	SetDefaultBtnCaption( DEFAULT_BTN_OK, m_pStrTable->String(E_STR_ID_SEARCH) );
	SetDefaultBtnCaption( DEFAULT_BTN_CANCEL, m_pStrTable->String(E_STR_ID_EXIT) );
#endif
	//////////////////////////////////////////////////////////////////////////

	m_channelNum = g_p_login_succ->productInfo.videoInputNum;
	if(0 == g_p_login_succ->productInfo.localVideoInputNum)
	{
		m_channelNum += g_p_login_succ->productInfo.sensorInputNum;
	}
	if( GUI::CWnd::m_width >= 1024 && GUI::CWnd::m_height >= 768 )
	{
		CreateUnfoldCtrl();
	}
	else
	{
		CreateFoldCtrl();
	}

	m_ckMotion.SetCheck(true, false);
	m_ckSensor.SetCheck(true, false);

	if ((0 == g_p_login_succ->productInfo.localVideoInputNum) && (0 < g_p_login_succ->productInfo.netVideoInputNum))
	{
		m_ckVideoLoss.SetCheck(false, false);
		m_ckVideoLoss.Show(false, false);
	}
	else
	{
		m_ckVideoLoss.SetCheck(true, false);
	}


	tm tmTime = DVRTime32ToTm(GetCurrTime32());
	//tmTime.tm_mon = tmTime.tm_mon;
	tmTime.tm_hour = 0;
	tmTime.tm_min  = 0;
	tmTime.tm_sec  = 0;
	if (GUI::CWnd::m_width >= 1024 && GUI::CWnd::m_height >= 768 || m_channelNum <= 16 )
	{
		m_searchCdn2TimeDlg.SetStartTime(tmTime);
	}
	else
	{
		m_searchCdnTimeDlg.SetStartTime(tmTime);
	}
	
	tmTime.tm_hour = 23;
	tmTime.tm_min  = 59;
	tmTime.tm_sec  = 59;
	if (GUI::CWnd::m_width >= 1024 && GUI::CWnd::m_height >= 768 || m_channelNum <= 16 )
	{
		m_searchCdn2TimeDlg.SetEndTime(tmTime);
	}
	else
	{
		m_searchCdnTimeDlg.SetEndTime(tmTime);
	}

	if(NULL != m_pEventLog)
	{
		delete [] m_pEventLog;
		m_pEventLog = NULL;
	}
	m_EventLogNum = 0;
}
void CEventInfoDlg::OnPostInitial()
{
	GUI::CDialog::OnPostInitial();

#ifdef __DVR_PROFESSIONAL__
	GUI::CButton *pBtn = dynamic_cast<GUI::CButton*>(GetCtrl(CTRLID_DEFAULT_BTN_APPLY));
	assert(pBtn != NULL);
	pBtn->SetEnable(false, false);
#endif
}

void CEventInfoDlg::CreateFoldCtrl()
{
	// 本页面在小分辨率下，16通道时空间不够
	// 因此针对 小分辨率 && 16通道 做特别处理
	unsigned short offsetY = 10;
	if( m_channelNum < 16 )
	{
		offsetY = 0;
	}
	if( m_channelNum > 16 )
	{
		m_searchConditionDlgWidth = m_length[CALENDAR_WIDTH] + 30;
	}
	else
	{
		m_searchConditionDlgWidth = m_length[CALENDAR_WIDTH] + 10;
	}
	unsigned short xLeft = m_length[DLG_OFFSET_LEFT], yTop = m_length[TITLE_HEIGHT] + m_length[DLG_OFFSET_TOP];
	unsigned short width = m_rect.Width() - 2 * m_length[DLG_OFFSET_LEFT];

	unsigned short x = xLeft, y = yTop;
	unsigned short listH = m_rect.Height() - yTop - m_length[DEFAULT_BTN_BOTTOM_YPOS] + offsetY;

	m_listCtrlEx.Create(GUI::CWnd::GetRandWndID(), x, y, width, listH, this, 0, 1, false );
	m_listCtrlEx.AddColumn(m_pStrTable->String(E_STR_ID_CH), width * 3 / 32);
	m_listCtrlEx.AddColumn(m_pStrTable->String(E_STR_ID_TYPE), width * 7 / 32);
	m_listCtrlEx.AddColumn(m_pStrTable->String(E_STR_ID_START_TIME), width * 11 / 32);
	m_listCtrlEx.AddColumn(m_pStrTable->String(E_STR_ID_END_TIME), width * 11 / 32);

	GUI::CRect rect = m_listCtrlEx.GetClientRect();
	unsigned short yOff = -offsetY / 2;
	if( m_channelNum < 16 )
	{
		yOff = ( m_rect.Height() - y - m_length[DEFAULT_BTN_BOTTOM_YPOS] - rect.Height() ) / 2;
	}
	y += yOff;
	m_listCtrlEx.MoveWnd( m_rect.m_left + x, m_rect.m_top + y );

	x = xLeft+ width - m_searchConditionDlgWidth;
	if( m_channelNum > 16 )
	{
		m_searchCdnTimeDlg.SetInitialInfo(m_channelNum);
		m_searchCdnTimeDlg.Create(GUI::CWnd::GetRandWndID(), x, y, m_searchConditionDlgWidth, rect.Height(), this, 2, 1);
	}
	else
	{
	m_searchCdn2TimeDlg.SetInitialInfo(m_channelNum);
	m_searchCdn2TimeDlg.Create(GUI::CWnd::GetRandWndID(), x, y, m_searchConditionDlgWidth, rect.Height(), this, 2, 1);
	}
	x -= m_length[FLEX_BTN_WIDTH];
	m_btnFlex.Create(GUI::CWnd::GetRandWndID(), x, (m_rect.Height()/2)-(m_length[FLEX_BTN_HEIGHT]/2), m_length[FLEX_BTN_WIDTH], m_length[FLEX_BTN_HEIGHT], this, 1, 1);

	m_listCtrlEx.SetWndSize(false, false, width - m_searchConditionDlgWidth - m_length[FLEX_BTN_WIDTH], rect.Height());

	m_btnFlex.SetImage(PAINT_STATUS_NORMAL, BMP_BUTTON_DLG_FLEX_R, false);
	m_btnFlex.SetImage(PAINT_STATUS_FOCUS, BMP_BUTTON_DLG_FLEX_R_F, false);

	m_bFlexSearchConditionDlg = false;


	rect = m_listCtrlEx.GetClientRect();
	{
		x = xLeft;
		y = m_rect.Height() - m_length[DEFAULT_BTN_BOTTOM_YPOS];
		m_ckMotion.Create(CTRLID_INFO_MOTION, x, y, 100, m_length[CHECK_HEIGHT], this, 0, 2);
		x += 100;

		if (IsSupportAlarmOut() || (g_p_login_succ->productInfo.netVideoInputNum > 0))
		{
			m_ckSensor.Create(CTRLID_INFO_SENSOR, x, y, 100, m_length[CHECK_HEIGHT], this, 1, 2);
			x += 100;
			m_ckVideoLoss.Create(CTRLID_INFO_VIDEO_LOSS, x, y, 120, m_length[CHECK_HEIGHT], this, 2, 2);
		}
		else
		{
			m_ckVideoLoss.Create(CTRLID_INFO_VIDEO_LOSS, x, y, 120, m_length[CHECK_HEIGHT], this, 1, 2);
		}

		m_ckMotion.SetCaption(m_pStrTable->String(E_STR_ID_MOTION), false, false);
		m_ckSensor.SetCaption(m_pStrTable->String(E_STR_ID_SENSOR), false, false);
		m_ckVideoLoss.SetCaption(m_pStrTable->String(E_STR_ID_VIDEO_LOSS), false, false);
	}

	//////////////////////////////////////////////////////////////////////////
}

void CEventInfoDlg::CreateUnfoldCtrl()
{
	m_searchConditionDlgWidth = m_length[CALENDAR_WIDTH] + 10;
	unsigned short xLeft = m_length[DLG_OFFSET_LEFT], yTop = m_length[TITLE_HEIGHT] + m_length[DLG_OFFSET_TOP];
	unsigned short width = m_rect.Width() - 2 * m_length[DLG_OFFSET_LEFT];

	unsigned short x = xLeft, y = yTop;
	unsigned short listW = width - m_searchConditionDlgWidth - 4;
	unsigned short listH = m_rect.Height() - yTop - m_length[DEFAULT_BTN_BOTTOM_YPOS];

	m_listCtrlEx.Create(GUI::CWnd::GetRandWndID(), x, y, listW, listH, this, 0, 1, false );
	m_listCtrlEx.AddColumn(m_pStrTable->String(E_STR_ID_CH), listW * 3 / 32);
	m_listCtrlEx.AddColumn(m_pStrTable->String(E_STR_ID_TYPE), listW * 9 / 32);
	m_listCtrlEx.AddColumn(m_pStrTable->String(E_STR_ID_START_TIME), listW * 10 / 32);
	m_listCtrlEx.AddColumn(m_pStrTable->String(E_STR_ID_END_TIME), listW * 10 / 32);

	GUI::CRect rect = m_listCtrlEx.GetClientRect();
	//unsigned short yOff = ( m_rect.Height() - y - m_length[DEFAULT_BTN_BOTTOM_YPOS] - rect.Height() ) / 2;
	//y += yOff;
	//m_listCtrlEx.MoveWnd( m_rect.m_left + x, m_rect.m_top + y );

	x = xLeft+ width - m_searchConditionDlgWidth;
	m_searchCdn2TimeDlg.SetInitialInfo(m_channelNum);
	m_searchCdn2TimeDlg.Create(GUI::CWnd::GetRandWndID(), x, y, m_searchConditionDlgWidth, rect.Height(), this, 2, 1);

	rect = m_listCtrlEx.GetClientRect();
	{
		x = xLeft;
		y = m_rect.Height() - m_length[DEFAULT_BTN_BOTTOM_YPOS];
		m_ckMotion.Create(CTRLID_INFO_MOTION, x, y, 150, m_length[CHECK_HEIGHT], this, 0, 2);
		x += 150;

		if (IsSupportAlarmOut() || (g_p_login_succ->productInfo.netVideoInputNum > 0))
		{
			m_ckSensor.Create(CTRLID_INFO_SENSOR, x, y, 150, m_length[CHECK_HEIGHT], this, 1, 2);
			x += 150;
			m_ckVideoLoss.Create(CTRLID_INFO_VIDEO_LOSS, x, y, 150, m_length[CHECK_HEIGHT], this, 2, 2);
		}
		else
		{
			m_ckVideoLoss.Create(CTRLID_INFO_VIDEO_LOSS, x, y, 150, m_length[CHECK_HEIGHT], this, 1, 2);
		}

		m_ckMotion.SetCaption(m_pStrTable->String(E_STR_ID_MOTION), false, false);
		m_ckSensor.SetCaption(m_pStrTable->String(E_STR_ID_SENSOR), false, false);
		m_ckVideoLoss.SetCaption(m_pStrTable->String(E_STR_ID_VIDEO_LOSS), false, false);
	}

	//////////////////////////////////////////////////////////////////////////
}


unsigned long CEventInfoDlg::OnOK()
{
	if (((GUI::CWnd::m_width >= 1024) && (GUI::CWnd::m_height >= 768)) || (m_channelNum <= 16) )
	{
		if (0 == m_searchCdn2TimeDlg.CH())
		{
			MessageBox(m_pStrTable->String(E_STR_ID_MESSAGE_34), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK | DEFAULT_ICON_WARNING);
			return KEY_VIRTUAL_MSG_STOP;
		}
	}
	else
	{
		if (0 == m_searchCdnTimeDlg.CH())
		{
			MessageBox(m_pStrTable->String(E_STR_ID_MESSAGE_34), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK | DEFAULT_ICON_WARNING);
			return KEY_VIRTUAL_MSG_STOP;
		}
	}


	if ((!m_ckMotion.IsCheck()) && (!m_ckSensor.IsCheck()) && !m_ckVideoLoss.IsCheck())
	{
		MessageBox(m_pStrTable->String(E_STR_ID_MESSAGE_35), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK | DEFAULT_ICON_WARNING);
		return KEY_VIRTUAL_MSG_STOP;
	}

	MESSAGE_DATA msgData;
	NET_EVENT_SEARCH searchCond;
	memset(&searchCond, 0, sizeof(NET_EVENT_SEARCH));

	if (m_ckMotion.IsCheck())
	{
		searchCond.type |= EVENT_TYPE_MOTION;
	}

	if (m_ckSensor.IsCheck())
	{
		searchCond.type |= EVENT_TYPE_SENSOR;
	}

	if (m_ckVideoLoss.IsCheck())
	{
		searchCond.type |= EVENT_TYPE_V_LOSS;
	}

	if (((GUI::CWnd::m_width >= 1024) && (GUI::CWnd::m_height >= 768)) || (m_channelNum <= 16) )
	{
		searchCond.startTime = m_searchCdn2TimeDlg.TimeStart();
		searchCond.endTime = m_searchCdn2TimeDlg.TimeEnd();
		searchCond.chnnBits = m_searchCdn2TimeDlg.CH();
	}
	else
	{
		searchCond.startTime = m_searchCdnTimeDlg.TimeStart();
		searchCond.endTime = m_searchCdnTimeDlg.TimeEnd();
		searchCond.chnnBits = m_searchCdnTimeDlg.CH();
	}


	CMessageMan::Instance()->SendMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_EVENT_INFO, (unsigned char *)&searchCond, sizeof(NET_EVENT_SEARCH), msgData);

	NET_EVENT_INFO *pInfo = reinterpret_cast<NET_EVENT_INFO *>(msgData.pData + sizeof(PACKCMD));

	EVENT_LOG *pDate = reinterpret_cast<EVENT_LOG *>(msgData.pData + sizeof(PACKCMD) + sizeof(NET_EVENT_INFO));
	

	if(NULL != m_pEventLog)
	{
		delete [] m_pEventLog;
		m_pEventLog = NULL;
	}
	m_EventLogNum = 0;

	if (pInfo->eventNum <= 0)
	{
#ifdef __DVR_PROFESSIONAL__
		GUI::CButton *pBtn = dynamic_cast<GUI::CButton*>(GetCtrl(CTRLID_DEFAULT_BTN_APPLY));
		assert(pBtn != NULL);
		pBtn->SetEnable(false, true);
#endif
		MessageBox(m_pStrTable->String(E_STR_ID_MESSAGE_36), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK | DEFAULT_ICON_WARNING);
	}
	else
	{
		assert ((pInfo->eventNum*sizeof(EVENT_LOG) + sizeof(NET_EVENT_INFO) + sizeof(PACKCMD)) <= msgData.dataLen);

		EVENT_LOG *pEvent = new EVENT_LOG[pInfo->eventNum];
		m_pEventLog	= new EVENT_LOG[pInfo->eventNum];
		memcpy(pEvent, pDate, pInfo->eventNum*sizeof(EVENT_LOG));
		memcpy(m_pEventLog, pDate, pInfo->eventNum*sizeof(EVENT_LOG));
		m_EventLogNum = pInfo->eventNum;

#ifdef __DVR_PROFESSIONAL__
		GUI::CButton *pBtn = dynamic_cast<GUI::CButton*>(GetCtrl(CTRLID_DEFAULT_BTN_APPLY));
		assert(pBtn != NULL);
		pBtn->SetEnable(true);
#endif
		m_listCtrlEx.PutData(pEvent, pInfo->eventNum);
		pEvent = NULL;
	}
	
	CMessageMan::Instance()->ReleaseMsgBuff(msgData);	

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CEventInfoDlg::OnApply()
{
	if((NULL != m_pEventLog) && (0 != m_EventLogNum))
	{
		//有事件数据，并且数据个数不为0的时候才允许生成xml文件
		char headTitle[132]={0};
		char *pElememtTitle[5]={NULL};
		pElememtTitle[0]=new char [132];
		pElememtTitle[1]=new char [132];
		pElememtTitle[2]=new char [132];
		pElememtTitle[3]=new char [132];

		strcpy(headTitle,m_pStrTable->String(E_STR_ID_TITLE_EVENT_LIST));
		strcpy(pElememtTitle[0], m_pStrTable->String(E_STR_ID_CH));
		strcpy(pElememtTitle[1], m_pStrTable->String(E_STR_ID_TYPE));
		strcpy(pElememtTitle[2], m_pStrTable->String(E_STR_ID_START_TIME));
		strcpy(pElememtTitle[3], m_pStrTable->String(E_STR_ID_END_TIME));
		PopupExportCtrlDlg(this, reinterpret_cast<char *>(&headTitle), reinterpret_cast<char **>(&pElememtTitle), reinterpret_cast<char *>(m_pEventLog),  m_EventLogNum*sizeof(EVENT_LOG), 1);

		delete [] pElememtTitle[0];
		delete [] pElememtTitle[1];
		delete [] pElememtTitle[2];
		delete [] pElememtTitle[3];
		delete [] pElememtTitle[4];
	}
	else
	{
		MessageBox(m_pStrTable->String(E_STR_ID_MESSAGE_36), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK | DEFAULT_ICON_WARNING);
	}

	return KEY_VIRTUAL_MSG_STOP;
}


//////////////////////////////////////////////////////////////////////////
BEGIN_MSG_MAP(CEventInfoDlg, GUI::CDialog)
	ON_MSG(m_btnFlex.GetID(), KEY_VIRTUAL_ENTER, OnBtnFlex)
	ON_MSG(CTRLID_NULL, KEY_VIRTUAL_SEARCH, OnOK)
	ON_MSG(m_ckSensor.GetID(), KEY_VIRTUAL_ENTER, OnCheckSensor)
END_MSG_MAP()


unsigned long CEventInfoDlg::OnBtnFlex()
{
	if( m_channelNum > 16 )
	{
		m_searchConditionDlgWidth = m_length[CALENDAR_WIDTH] + 30;
	}
	else
	{
		m_searchConditionDlgWidth = m_length[CALENDAR_WIDTH] + 10;
	}
	unsigned short xLeft = m_length[DLG_OFFSET_LEFT], yTop = m_length[TITLE_HEIGHT] + m_length[DLG_OFFSET_TOP];
	unsigned short width = m_rect.Width() - 2 * m_length[DLG_OFFSET_LEFT];
	
	unsigned short x = xLeft;
	unsigned short y = yTop;

	if (m_bFlexSearchConditionDlg)
	{
		int w = m_rect.Width()-m_searchConditionDlgWidth-m_length[FLEX_BTN_WIDTH]-(m_length[DLG_OFFSET_LEFT]*2);
		int h = m_listCtrlEx.GetClientRect().Height();
		m_listCtrlEx.SetWndSize(false, false, w, h );

		if( m_channelNum > 16 )
		{
			m_searchCdnTimeDlg.Show(true, false);
		}
		else
		{
			m_searchCdn2TimeDlg.Show(true, false);
		}

		x = m_rect.m_left + xLeft + width - m_searchConditionDlgWidth - m_length[FLEX_BTN_WIDTH];
		y = m_rect.m_top + m_rect.Height()/2 - m_length[FLEX_BTN_HEIGHT]/2;
		m_btnFlex.MoveWnd( x, y );

		m_btnFlex.SetImage(PAINT_STATUS_NORMAL, BMP_BUTTON_DLG_FLEX_R, false);
		m_btnFlex.SetImage(PAINT_STATUS_FOCUS, BMP_BUTTON_DLG_FLEX_R_F, false);
	}
	else
	{
		m_listCtrlEx.SetWndSize(true, false);

		if( m_channelNum > 16 )
		{
			m_searchCdnTimeDlg.Show(false, false);
		}
		else
		{
			m_searchCdn2TimeDlg.Show(false, false);
		}

		x = m_rect.m_left + xLeft + width;
		y = m_rect.m_top + m_rect.Height()/2 - m_length[FLEX_BTN_HEIGHT]/2;
		m_btnFlex.MoveWnd( x, y );

		m_btnFlex.SetImage(PAINT_STATUS_NORMAL, BMP_BUTTON_DLG_FLEX_L, false);
		m_btnFlex.SetImage(PAINT_STATUS_FOCUS, BMP_BUTTON_DLG_FLEX_L_F, false);
	}

	m_bFlexSearchConditionDlg = !m_bFlexSearchConditionDlg;

	Repaint();
	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CEventInfoDlg::OnCheckSensor()
{
	if(g_p_login_succ->productInfo.localVideoInputNum > 0)
	{
		return KEY_VIRTUAL_MSG_STOP;
	}

	if(0 == g_p_login_succ->productInfo.sensorInputNum)
	{
		return KEY_VIRTUAL_MSG_STOP;
	}

	long prevChannel = m_channelNum;
	m_channelNum = g_p_login_succ->productInfo.videoInputNum;
	if(m_ckSensor.IsCheck())
	{
		m_channelNum += g_p_login_succ->productInfo.sensorInputNum;
	}

	m_btnFlex.Destroy();
	m_listCtrlEx.Destroy();
	m_ckSensor.Destroy();
	m_ckMotion.Destroy();
	m_ckVideoLoss.Destroy();

	if( GUI::CWnd::m_width >= 1024 && GUI::CWnd::m_height >= 768 )
	{
		m_searchCdn2TimeDlg.Destroy();
		CreateUnfoldCtrl();
	}
	else
	{
		if(prevChannel <= 16)
		{
			m_searchCdn2TimeDlg.Destroy();
		}
		else
		{
			m_searchCdnTimeDlg.Destroy();
		}
		CreateFoldCtrl();
	}

	if ((0 == g_p_login_succ->productInfo.localVideoInputNum) && (0 < g_p_login_succ->productInfo.netVideoInputNum))
	{
		m_ckVideoLoss.SetCheck(false, false);
		m_ckVideoLoss.Show(false, false);
	}
	else
	{
		m_ckVideoLoss.SetCheck(true, false);
	}
	
	Repaint();
	return KEY_VIRTUAL_MSG_STOP;
}

//end
