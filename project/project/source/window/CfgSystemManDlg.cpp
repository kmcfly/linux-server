/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : l
** Date         : 2009-08-11
** Name         : CfgSystemManDlg.cpp
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/

#include "CfgSystemManDlg.h"
#include "MessageMan.h"
#include "UserMan.h"
#include "FuncCustom.h"


extern LANGUAGE	g_language;

//////////////////////////////////////////////////////////////////////////
/// Define
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
/// 构造函数区
//////////////////////////////////////////////////////////////////////////
CCfgSystemManDlg::CCfgSystemManDlg()
{

}

CCfgSystemManDlg::~CCfgSystemManDlg()
{

}

//////////////////////////////////////////////////////////////////////////
/// 公共接口函数区
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
/// 继承函数区
//////////////////////////////////////////////////////////////////////////
void CCfgSystemManDlg::OnInitial()
{
	GUI::CDialog::OnInitial();
	SetImage(BMP_MEM_DLG_NORMAL, false);
	SetCaption(m_pStrTable->String(E_STR_ID_TITLE_SETUP), false);
	//////////////////////////////////////////////////////////////////////
	//进入配置

	///
	CreateCtrl();
}

void CCfgSystemManDlg::OnDestroy()
{
	GUI::CDialog::OnDestroy();
	//退出配置

}

//////////////////////////////////////////////////////////////////////////
/// 功能函数区
//////////////////////////////////////////////////////////////////////////
bool CCfgSystemManDlg::CreateCtrl()
{
#ifndef __DVR_ANCHOR__
	unsigned short size = 72, xOffset = 150, yOffset = 120;
	unsigned short leftX = 50;
	unsigned short topY = 60;
	unsigned short x = leftX, y = topY, cx = size, cy = size, stH = m_length[STATIC_HEIGHT];
	int index = 0;

	m_btn[SYSTEM_MAN_BTN_BASIC].Create(CTRLID_CFG_SYSTEM_MAN_BASIC_BTN, x, y, cx, cy, this, 1, 1, NULL, WND_CHILD | WND_VISIBLE | WND_NO_IMAGE_BK);
	m_btn[SYSTEM_MAN_BTN_BASIC].SetImage(PAINT_STATUS_NORMAL, BMP_BASIC_CONFIG, false);
	m_btn[SYSTEM_MAN_BTN_BASIC].SetImage(PAINT_STATUS_FOCUS, BMP_BASIC_CONFIG_FOCUS, false);
	m_btn[SYSTEM_MAN_BTN_BASIC].SetImage(PAINT_STATUS_DISABLE, BMP_BASIC_CONFIG, false);
	m_static[SYSTEM_MAN_BTN_BASIC].Create(CTRLID_STATIC, x - size/2, y+size, size*2, stH, this, m_pStrTable->String(E_STR_ID_MENU_CFG_BASIC));
	m_static[SYSTEM_MAN_BTN_BASIC].SetAlignText(GUI::ALIGN_TEXT_CENTER);

	x += xOffset;
	index++;
	m_btn[SYSTEM_MAN_BTN_LIVE].Create(CTRLID_CFG_SYSTEM_MAN_LIVE_BTN, x, y, cx, cy, this, 2, 1, NULL, WND_CHILD | WND_VISIBLE | WND_NO_IMAGE_BK);
	m_btn[SYSTEM_MAN_BTN_LIVE].SetImage(PAINT_STATUS_NORMAL, BMP_LIVE, false);
	m_btn[SYSTEM_MAN_BTN_LIVE].SetImage(PAINT_STATUS_FOCUS, BMP_LIVE_FOCUS, false);
	m_btn[SYSTEM_MAN_BTN_LIVE].SetImage(PAINT_STATUS_DISABLE, BMP_LIVE, false);
	m_static[SYSTEM_MAN_BTN_LIVE].Create(CTRLID_STATIC, x - size/2, y+size, size*2, stH, this, m_pStrTable->String(E_STR_ID_MENU_CFG_LIVE));
	m_static[SYSTEM_MAN_BTN_LIVE].SetAlignText(GUI::ALIGN_TEXT_CENTER);

	x += xOffset;
	index++;
	m_btn[SYSTEM_MAN_BTN_RECORD].Create(CTRLID_CFG_SYSTEM_MAN_RECORD_BTN, x, y, cx, cy, this, 3, 1, NULL, WND_CHILD | WND_VISIBLE | WND_NO_IMAGE_BK);
	m_btn[SYSTEM_MAN_BTN_RECORD].SetImage(PAINT_STATUS_NORMAL, BMP_RECORD, false);
	m_btn[SYSTEM_MAN_BTN_RECORD].SetImage(PAINT_STATUS_FOCUS, BMP_RECORD_FOCUS, false);
	m_btn[SYSTEM_MAN_BTN_RECORD].SetImage(PAINT_STATUS_DISABLE, BMP_RECORD, false);
	m_static[SYSTEM_MAN_BTN_RECORD].Create(CTRLID_STATIC, x - size/2, y+size, size*2, stH, this, m_pStrTable->String(E_STR_ID_MENU_CFG_RECORD));
	m_static[SYSTEM_MAN_BTN_RECORD].SetAlignText(GUI::ALIGN_TEXT_CENTER);

	x += xOffset;
	index++;
	m_btn[SYSTEM_MAN_BTN_SCHEDULE].Create(CTRLID_CFG_SYSTEM_MAN_SCHEDULE_BTN, x, y, cx, cy, this, 4, 1, NULL, WND_CHILD | WND_VISIBLE | WND_NO_IMAGE_BK);
	m_btn[SYSTEM_MAN_BTN_SCHEDULE].SetImage(PAINT_STATUS_NORMAL, BMP_SCHEDULE, false);
	m_btn[SYSTEM_MAN_BTN_SCHEDULE].SetImage(PAINT_STATUS_FOCUS, BMP_SCHEDULE_FOCUS, false);
	m_btn[SYSTEM_MAN_BTN_SCHEDULE].SetImage(PAINT_STATUS_DISABLE, BMP_SCHEDULE, false);
	#ifdef __CUSTOM_US02_NEW__
		if(g_language == LANGUAGE_GERMAN || g_language == LANGUAGE_RUSSIAN)
		{
			m_static[SYSTEM_MAN_BTN_SCHEDULE].Create(CTRLID_STATIC, x - size/2, y+size, size*2, stH, this, m_pStrTable->String(E_STR_ID_MENU_CFG_SCHEDULE));
		}
		else
		{
			m_static[SYSTEM_MAN_BTN_SCHEDULE].Create(CTRLID_STATIC, x - size, y+size, size*3, stH, this, m_pStrTable->String(E_STR_ID_MENU_CFG_SCHEDULE_01));			
		}
#else
		m_static[SYSTEM_MAN_BTN_SCHEDULE].Create(CTRLID_STATIC, x - size/2, y+size, size*2, stH, this, m_pStrTable->String(E_STR_ID_MENU_CFG_SCHEDULE));
	#endif
	m_static[SYSTEM_MAN_BTN_SCHEDULE].SetAlignText(GUI::ALIGN_TEXT_CENTER);


	x += xOffset;
	index++;
	if(0 == (index % 4))
	{
		index = 0;
		x = leftX;
		y += yOffset;
	}
	m_btn[SYSTEM_MAN_BTN_ALARM].Create(CTRLID_CFG_SYSTEM_MAN_ALARM_BTN, x, y, cx, cy, this, 1, 2, NULL, WND_CHILD | WND_VISIBLE | WND_NO_IMAGE_BK);
	m_btn[SYSTEM_MAN_BTN_ALARM].SetImage(PAINT_STATUS_NORMAL, BMP_ALARM, false);
	m_btn[SYSTEM_MAN_BTN_ALARM].SetImage(PAINT_STATUS_FOCUS, BMP_ALARM_FOCUS, false);
	m_btn[SYSTEM_MAN_BTN_ALARM].SetImage(PAINT_STATUS_DISABLE, BMP_ALARM, false);
	m_static[SYSTEM_MAN_BTN_ALARM].Create(CTRLID_STATIC, x - size/2, y+size, size*2, stH, this, m_pStrTable->String(E_STR_ID_MENU_CFG_ALARM));
	m_static[SYSTEM_MAN_BTN_ALARM].SetAlignText(GUI::ALIGN_TEXT_CENTER);

#if !defined(__TW01_RILI__)
	x += xOffset;
	index++;
	if(0 == (index % 4))
	{
		index = 0;
		x = leftX;
		y += yOffset;
	}
	m_btn[SYSTEM_MAN_BTN_NETWORK].Create(CTRLID_CFG_SYSTEM_MAN_NETWORK_BTN, x, y, cx, cy, this, 2, 2, NULL, WND_CHILD | WND_VISIBLE | WND_NO_IMAGE_BK);
	m_btn[SYSTEM_MAN_BTN_NETWORK].SetImage(PAINT_STATUS_NORMAL, BMP_NETWORK, false);
	m_btn[SYSTEM_MAN_BTN_NETWORK].SetImage(PAINT_STATUS_FOCUS, BMP_NETWORK_FOCUS, false);
	m_btn[SYSTEM_MAN_BTN_NETWORK].SetImage(PAINT_STATUS_DISABLE, BMP_NETWORK, false);
	m_static[SYSTEM_MAN_BTN_NETWORK].Create(CTRLID_STATIC, x - size/2, y+size, size*2, stH, this, m_pStrTable->String(E_STR_ID_MENU_CFG_NETWORK));
	m_static[SYSTEM_MAN_BTN_NETWORK].SetAlignText(GUI::ALIGN_TEXT_CENTER);
#endif


	x += xOffset;
	index++;
	if(0 == (index % 4))
	{
		index = 0;
		x = leftX;
		y += yOffset;
	}
	m_btn[SYSTEM_MAN_BTN_USERS].Create(CTRLID_CFG_SYSTEM_MAN_USERS_BTN, x, y, cx, cy, this, 3, 2, NULL, WND_CHILD | WND_VISIBLE | WND_NO_IMAGE_BK);
	m_btn[SYSTEM_MAN_BTN_USERS].SetImage(PAINT_STATUS_NORMAL, BMP_USER, false);
	m_btn[SYSTEM_MAN_BTN_USERS].SetImage(PAINT_STATUS_FOCUS, BMP_USER_FOCUS, false);
	m_btn[SYSTEM_MAN_BTN_USERS].SetImage(PAINT_STATUS_DISABLE, BMP_USER, false);
	m_static[SYSTEM_MAN_BTN_USERS].Create(CTRLID_STATIC, x - size/2, y+size, size*2, stH, this, m_pStrTable->String(E_STR_ID_MENU_CFG_USERS));
	m_static[SYSTEM_MAN_BTN_USERS].SetAlignText(GUI::ALIGN_TEXT_CENTER);

#if !defined(__TW01_RILI__)
	if(IsSupportPTZ() && (CCfgDlg::s_loginSuccessReply.productInfo.videoInputNum > 0))
	{
		x += xOffset;
		index++;
		if(0 == (index % 4))
		{
			index = 0;
			x = leftX;
			y += yOffset;
		}
		m_btn[SYSTEM_MAN_BTN_PTZ].Create(CTRLID_CFG_SYSTEM_MAN_PTZ_BTN, x, y, cx, cy, this, 4, 2, NULL, WND_CHILD | WND_VISIBLE | WND_NO_IMAGE_BK);
		m_btn[SYSTEM_MAN_BTN_PTZ].SetImage(PAINT_STATUS_NORMAL, BMP_PTZ, false);
		m_btn[SYSTEM_MAN_BTN_PTZ].SetImage(PAINT_STATUS_FOCUS, BMP_PTZ_FOCUS, false);
		m_btn[SYSTEM_MAN_BTN_PTZ].SetImage(PAINT_STATUS_DISABLE, BMP_PTZ, false);
		m_static[SYSTEM_MAN_BTN_PTZ].Create(CTRLID_STATIC, x - size/2, y+size, size*2, stH, this, m_pStrTable->String(E_STR_ID_MENU_CFG_PTZ));
		m_static[SYSTEM_MAN_BTN_PTZ].SetAlignText(GUI::ALIGN_TEXT_CENTER);
	}
#endif

	x += xOffset;
	index++;
	if(0 == (index % 4))
	{
		index = 0;
		x = leftX;
		y += yOffset;
	}
	m_btn[SYSTEM_MAN_BTN_ADVANCE].Create(CTRLID_CFG_SYSTEM_MAN_ADVANCE_BTN, x, y, cx, cy, this, 1, 3, NULL, WND_CHILD | WND_VISIBLE | WND_NO_IMAGE_BK);
	m_btn[SYSTEM_MAN_BTN_ADVANCE].SetImage(PAINT_STATUS_NORMAL, BMP_ADVANCE, false);
	m_btn[SYSTEM_MAN_BTN_ADVANCE].SetImage(PAINT_STATUS_FOCUS, BMP_ADVANCE_FOCUS, false);
	m_btn[SYSTEM_MAN_BTN_ADVANCE].SetImage(PAINT_STATUS_DISABLE, BMP_ADVANCE, false);
	#if defined(__KR24_IN__) || defined(__KOREAN__)
		if(g_language == LANGUAGE_CHINESE_S)
		{
			char hanyu[] = {0xea,0xb3,0xa0,0xea,0xb8,0x89,0xec,0x84,0xa4,0xec,0xa0,0x95,0x00};
			m_static[SYSTEM_MAN_BTN_ADVANCE].Create(CTRLID_STATIC, x - size/2, y+size, size*2, stH, this, hanyu);
		}
		else
		{
			m_static[SYSTEM_MAN_BTN_ADVANCE].Create(CTRLID_STATIC, x - size/2, y+size, size*2, stH, this, m_pStrTable->String(E_STR_ID_MENU_CFG_ADVANCED));		
		}
	#else
		m_static[SYSTEM_MAN_BTN_ADVANCE].Create(CTRLID_STATIC, x - size/2, y+size, size*2, stH, this, m_pStrTable->String(E_STR_ID_MENU_CFG_ADVANCED));
	#endif
	m_static[SYSTEM_MAN_BTN_ADVANCE].SetAlignText(GUI::ALIGN_TEXT_CENTER);

#ifdef __GB28181_SERVER__
	x += xOffset;
	index++;
	if(0 == (index % 4))
	{
		index = 0;
		x = leftX;
		y += yOffset;
	}

	m_btn[SYSTEM_MAN_BTN_GB28181].Create(CTRLID_CFG_SYSTEM_MAN_GB28181_BTN, x, y, cx, cy, this, 1, 1, NULL, WND_CHILD | WND_VISIBLE | WND_NO_IMAGE_BK);
	m_btn[SYSTEM_MAN_BTN_GB28181].SetImage(PAINT_STATUS_NORMAL, BMP_BASIC_CONFIG, false);
	m_btn[SYSTEM_MAN_BTN_GB28181].SetImage(PAINT_STATUS_FOCUS, BMP_BASIC_CONFIG_FOCUS, false);
	m_btn[SYSTEM_MAN_BTN_GB28181].SetImage(PAINT_STATUS_DISABLE, BMP_BASIC_CONFIG, false);
	m_static[SYSTEM_MAN_BTN_GB28181].Create(CTRLID_STATIC, x - size/2, y+size, size*2, stH, this, "GB28181");
	m_static[SYSTEM_MAN_BTN_GB28181].SetAlignText(GUI::ALIGN_TEXT_CENTER);
#endif
#else

	unsigned short x = 80;
	unsigned short y = m_length[TITLE_HEIGHT] + m_length[DLG_OFFSET_TOP] + 25;
	unsigned short x_off = 60, y_off = 10;
	unsigned short cx = m_length[ANCHOR_WIDTH], cy = m_length[ANCHOR_HEIGHT];

	int yFocus = 1;

	m_btn[0].Create(GUI::CWnd::GetRandWndID(), x, y, cx, cy, this, 0, yFocus++, m_pStrTable->String(E_STR_ID_MENU_CFG_BASIC));
	m_btn[0].SetImage(BMP_ANCHOR_BASIC, BMP_ANCHOR_BASIC_F, BMP_ANCHOR_BASIC);

	y += 60;
	m_btn[1].Create(GUI::CWnd::GetRandWndID(), x, y, cx, cy, this, 0, yFocus++, m_pStrTable->String(E_STR_ID_MENU_CFG_LIVE));
	m_btn[1].SetImage(BMP_ANCHOR_LIVE, BMP_ANCHOR_LIVE_F, BMP_ANCHOR_LIVE);

	y += 60;
	m_btn[SYSTEM_MAN_BTN_RECORD].Create(GUI::CWnd::GetRandWndID(), x, y, cx, cy, this, 0, yFocus++, m_pStrTable->String(E_STR_ID_MENU_CFG_RECORD));
	m_btn[SYSTEM_MAN_BTN_RECORD].SetImage(BMP_ANCHOR_RECORD, BMP_ANCHOR_RECORD_F, BMP_ANCHOR_RECORD);

	y += 60;
	m_btn[SYSTEM_MAN_BTN_SCHEDULE].Create(GUI::CWnd::GetRandWndID(), x, y, cx, cy, this, 0, yFocus++, m_pStrTable->String(E_STR_ID_MENU_CFG_SCHEDULE));
	m_btn[SYSTEM_MAN_BTN_SCHEDULE].SetImage(BMP_ANCHOR_SCHEDULE, BMP_ANCHOR_SCHEDULE_F, BMP_ANCHOR_SCHEDULE);

	y += 60;
	m_btn[SYSTEM_MAN_BTN_ALARM].Create(GUI::CWnd::GetRandWndID(), x, y, cx, cy, this, 0, yFocus++, m_pStrTable->String(E_STR_ID_MENU_CFG_ALARM));
	m_btn[SYSTEM_MAN_BTN_ALARM].SetImage(BMP_ANCHOR_ALARM, BMP_ANCHOR_ALARM_F, BMP_ANCHOR_ALARM);

	//第二列

	x = m_rect.Width() / 2 + 40;
	y = m_length[TITLE_HEIGHT] + m_length[DLG_OFFSET_TOP] + 25 - 60;

	y += 60;
	m_btn[SYSTEM_MAN_BTN_NETWORK].Create(GUI::CWnd::GetRandWndID(), x, y, cx, cy, this, 0, yFocus++, m_pStrTable->String(E_STR_ID_MENU_CFG_NETWORK));
	m_btn[SYSTEM_MAN_BTN_NETWORK].SetImage(BMP_ANCHOR_NETWORK, BMP_ANCHOR_NETWORK_F, BMP_ANCHOR_NETWORK);

	y += 60;
	m_btn[SYSTEM_MAN_BTN_USERS].Create(GUI::CWnd::GetRandWndID(), x, y, cx, cy, this, 0, yFocus++, m_pStrTable->String(E_STR_ID_MENU_CFG_USERS));
	m_btn[SYSTEM_MAN_BTN_USERS].SetImage(BMP_ANCHOR_USER, BMP_ANCHOR_USER_F, BMP_ANCHOR_USER);

	if(IsSupportPTZ() && (CCfgDlg::s_loginSuccessReply.productInfo.videoInputNum > 0))
	{
		y += 60;
		m_btn[SYSTEM_MAN_BTN_PTZ].Create(GUI::CWnd::GetRandWndID(), x, y, cx, cy, this, 0, yFocus++, m_pStrTable->String(E_STR_ID_MENU_CFG_PTZ));
		m_btn[SYSTEM_MAN_BTN_PTZ].SetImage(BMP_ANCHOR_PTZ, BMP_ANCHOR_PTZ_F, BMP_ANCHOR_PTZ);
	}

	y += 60;
	m_btn[SYSTEM_MAN_BTN_ADVANCE].Create(GUI::CWnd::GetRandWndID(), x, y, cx, cy, this, 0, yFocus++, m_pStrTable->String(E_STR_ID_MENU_CFG_ADVANCED));
	m_btn[SYSTEM_MAN_BTN_ADVANCE].SetImage(BMP_ANCHOR_ADVANCED, BMP_ANCHOR_ADVANCED_F, BMP_ANCHOR_ADVANCED);

//#ifdef __GB28181_SERVER__
//	y += 60;
//	m_btn[SYSTEM_MAN_BTN_GB28181].Create(GUI::CWnd::GetRandWndID(), x, y, cx, cy, this, 0, yFocus++, "GB28181");
//	m_btn[SYSTEM_MAN_BTN_GB28181].SetImage(BMP_ANCHOR_BASIC, BMP_ANCHOR_BASIC_F, BMP_ANCHOR_BASIC);
//#endif
#endif

	return true;
}

bool CCfgSystemManDlg::EnterConfig()
{
	//进入配置
	CMessageMan* pMsgMan = CMessageMan::Instance();

	MESSAGE_DATA msgData;
	memset(&msgData, 0, sizeof(MESSAGE_DATA));
	pMsgMan->SendMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_CFG_ENTER, NULL, 0, msgData);

	assert(NULL != msgData.pData);
	PACKCMD *pCmd = reinterpret_cast<PACKCMD *>(msgData.pData);
	if (CMD_REPLY_CFG_SUCC == static_cast<NET_PROTOCOL>(pCmd->cmdType))
	{
		pMsgMan->ReleaseMsgBuff( msgData );
		return true;
	}
	else
	{
		pMsgMan->ReleaseMsgBuff( msgData );
		GUI::CDialog::MessageBox(m_pStrTable->String(E_STR_ID_MESSAGE_26), m_pStrTable->String(E_STR_ID_TITLE_FAILED), DEFAULT_BTN_OK|DEFAULT_ICON_ERROR);
		return false;
	}
}

void CCfgSystemManDlg::ExitConfig() const
{
	//退出配置
	CMessageMan* pMsgMan = CMessageMan::Instance();
	pMsgMan->PostMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_CFG_LEAVE, NULL, 0);
}

bool CCfgSystemManDlg::EnterPtz()
{
	//进入云台配置
	CMessageMan* pMsgMan = CMessageMan::Instance();

	MESSAGE_DATA msgData;
	memset(&msgData, 0, sizeof(MESSAGE_DATA));
	pMsgMan->SendMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_PTZ_START, NULL, 0, msgData);

	assert(NULL != msgData.pData);
	PACKCMD *pCmd = reinterpret_cast<PACKCMD *>(msgData.pData);
	if (CMD_REPLY_PTZ_START_SUCC == static_cast<NET_PROTOCOL>(pCmd->cmdType))
	{
		pMsgMan->ReleaseMsgBuff( msgData );
		return true;
	}
	else
	{
		pMsgMan->ReleaseMsgBuff( msgData );
		GUI::CDialog::MessageBox(m_pStrTable->String(E_STR_ID_MESSAGE_112), m_pStrTable->String(E_STR_ID_TITLE_FAILED), DEFAULT_BTN_OK|DEFAULT_ICON_ERROR);
		return false;
	}
}

void CCfgSystemManDlg::ExitPtz() const
{	
	//退出配置云台
	CMessageMan* pMsgMan = CMessageMan::Instance();
	pMsgMan->PostMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_PTZ_STOP, NULL, 0);
}

bool CCfgSystemManDlg::CheckAuthoritySetup()
{
	if( 0 != CUserMan::Instance()->CheckAuthority(AUTH_SYS_SETUP) )
	{
		return true;
	}
	else
	{
		return false;
	}
}

//////////////////////////////////////////////////////////////////////////
/// 消息响应区
//////////////////////////////////////////////////////////////////////////
BEGIN_MSG_MAP(CCfgSystemManDlg, GUI::CDialog)
	ON_MSG(m_btn[SYSTEM_MAN_BTN_BASIC].GetID(), KEY_VIRTUAL_ENTER, OnBtnCfgBasic)
	ON_MSG(m_btn[SYSTEM_MAN_BTN_LIVE].GetID(), KEY_VIRTUAL_ENTER, OnBtnCfgLive)
	ON_MSG(m_btn[SYSTEM_MAN_BTN_RECORD].GetID(), KEY_VIRTUAL_ENTER, OnBtnCfgRecord)
	ON_MSG(m_btn[SYSTEM_MAN_BTN_SCHEDULE].GetID(), KEY_VIRTUAL_ENTER, OnBtnCfgSchedule)
	ON_MSG(m_btn[SYSTEM_MAN_BTN_ALARM].GetID(), KEY_VIRTUAL_ENTER, OnBtnCfgAlarm)
#if !defined(__TW01_RILI__)
	ON_MSG(m_btn[SYSTEM_MAN_BTN_NETWORK].GetID(), KEY_VIRTUAL_ENTER, OnBtnCfgNetwork)
#endif
	ON_MSG(m_btn[SYSTEM_MAN_BTN_USERS].GetID(), KEY_VIRTUAL_ENTER, OnBtnCfgUsers)
#if !defined(__TW01_RILI__)
	ON_MSG(m_btn[SYSTEM_MAN_BTN_PTZ].GetID(), KEY_VIRTUAL_ENTER, OnBtnCfgPTZ)
#endif
	ON_MSG(m_btn[SYSTEM_MAN_BTN_ADVANCE].GetID(), KEY_VIRTUAL_ENTER, OnBtnCfgAdvance)
#ifdef __GB28181_SERVER__
	ON_MSG(m_btn[SYSTEM_MAN_BTN_GB28181].GetID(), KEY_VIRTUAL_ENTER, OnBtnCfgGb28181)
#endif
END_MSG_MAP()

unsigned long CCfgSystemManDlg::OnBtnCfgBasic()
{
	if( CheckAuthoritySetup() )
	{
		if (EnterConfig())
		{
			m_cfgBasicManDlg.DoModal( this, CENTER_POS, CENTER_POS, MAX_SCREEN_X, MAX_SCREEN_Y, false );
			ExitConfig();
		} 
	}
	else
	{
		MessageBox(m_pStrTable->String(E_STR_ID_MESSAGE_84), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK | DEFAULT_ICON_WARNING);
	}

	return KEY_VIRTUAL_MSG_STOP;
}
#ifdef __GB28181_SERVER__
unsigned long CCfgSystemManDlg::OnBtnCfgGb28181()
{
	if( CheckAuthoritySetup() )
	{
		if (EnterConfig())
		{
			m_cfgGb28181Dlg.DoModal( this, CENTER_POS, CENTER_POS, MAX_SCREEN_X, MAX_SCREEN_Y, false );
			ExitConfig();
		} 
	}
	else
	{
		MessageBox(m_pStrTable->String(E_STR_ID_MESSAGE_84), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK | DEFAULT_ICON_WARNING);
	}

	return KEY_VIRTUAL_MSG_STOP;
}
#endif
unsigned long CCfgSystemManDlg::OnBtnCfgLive()
{
	if( CheckAuthoritySetup() )
	{
		if (EnterConfig())
		{
			m_cfgLiveManDlg.DoModal( this ,CENTER_POS,CENTER_POS,MAX_SCREEN_X,MAX_SCREEN_Y,false);
			ExitConfig();
		} 
	}
	else
	{
		MessageBox(m_pStrTable->String(E_STR_ID_MESSAGE_84), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK | DEFAULT_ICON_WARNING);
	}

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CCfgSystemManDlg::OnBtnCfgRecord()
{
	if( CheckAuthoritySetup() )
	{
		if (EnterConfig())
		{
			m_cfgRecordManDlg.DoModal( this, CENTER_POS, CENTER_POS, MAX_SCREEN_X, MAX_SCREEN_Y, false  );
			ExitConfig();
		} 
	}
	else
	{
		MessageBox(m_pStrTable->String(E_STR_ID_MESSAGE_84), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK | DEFAULT_ICON_WARNING);
	}

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CCfgSystemManDlg::OnBtnCfgSchedule()
{
	if( CheckAuthoritySetup() )
	{
		if (EnterConfig())
		{
			m_cfgScheduleDlg.DoModal( this, CENTER_POS, CENTER_POS, MAX_SCREEN_X, MAX_SCREEN_Y, false  );
			ExitConfig();
		} 
	}
	else
	{
		MessageBox(m_pStrTable->String(E_STR_ID_MESSAGE_84), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK | DEFAULT_ICON_WARNING);
	}

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CCfgSystemManDlg::OnBtnCfgAlarm()
{
	if( CheckAuthoritySetup() )
	{
		if (EnterConfig())
		{
			m_cfgAlarmManDlg.DoModal( this, CENTER_POS, CENTER_POS, MAX_SCREEN_X, MAX_SCREEN_Y, false  );
			ExitConfig();
		}
	}
	else
	{
		MessageBox(m_pStrTable->String(E_STR_ID_MESSAGE_84), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK | DEFAULT_ICON_WARNING);
	}

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CCfgSystemManDlg::OnBtnCfgNetwork()
{
	if( CheckAuthoritySetup() )
	{
		if (EnterConfig())
		{
			m_cfgNetworkManDlg.DoModal( this, CENTER_POS, CENTER_POS, MAX_SCREEN_X, MAX_SCREEN_Y, false  );
			ExitConfig();
		}
	}
	else
	{
		MessageBox(m_pStrTable->String(E_STR_ID_MESSAGE_84), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK | DEFAULT_ICON_WARNING);
	}

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CCfgSystemManDlg::OnBtnCfgUsers()
{
	if (EnterConfig())
	{
		m_cfgUserDlg.DoModal( this, CENTER_POS, CENTER_POS, MAX_SCREEN_X, MAX_SCREEN_Y, false);
		ExitConfig();
	}

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CCfgSystemManDlg::OnBtnCfgPTZ()
{
	if( CheckAuthoritySetup() )
	{
		if (EnterConfig() && EnterPtz())
		{
			m_cfgPtzDlg.DoModal( this, CENTER_POS, CENTER_POS, MAX_SCREEN_X, MAX_SCREEN_Y, false );
			ExitPtz();
			ExitConfig();
		}
	}
	else
	{
		MessageBox(m_pStrTable->String(E_STR_ID_MESSAGE_84), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK | DEFAULT_ICON_WARNING);
	}

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CCfgSystemManDlg::OnBtnCfgAdvance()
{
	if( CheckAuthoritySetup() )
	{
		if (EnterConfig())
		{
			m_cfgAdvanceDlg.DoModal( this, CENTER_POS, CENTER_POS, MAX_SCREEN_X, MAX_SCREEN_Y, false  );
			ExitConfig();
		}
	}
	else
	{
		MessageBox(m_pStrTable->String(E_STR_ID_MESSAGE_84), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK | DEFAULT_ICON_WARNING);
	}

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CCfgSystemManDlg::OnExit()
{
	EndDialog(DLG_ESC);
	return KEY_VIRTUAL_MSG_STOP;
}

//end
