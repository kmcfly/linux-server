/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : L
** Date         : 2009-08-19
** Name         : CfgAdvanceDlg.cpp
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/
#include "CfgAdvanceDlg.h"
#include "MessageMan.h"
#include "UserMan.h"

CCfgAdvanceDlg::CCfgAdvanceDlg()
{

}

CCfgAdvanceDlg::~CCfgAdvanceDlg()
{

}

void CCfgAdvanceDlg::OnInitial()
{
	GUI::CDialog::OnInitial();
	SetImage(BMP_MEM_DLG_NORMAL, false);
	SetCaption(m_pStrTable->String(E_STR_ID_TITLE_ADVANCE_SETUP), false);
	//////////////////////////////////////////////////////////////////////////
	unsigned short x = 200;
	unsigned short y = m_length[TITLE_HEIGHT] + m_length[DLG_OFFSET_TOP] + 40;
	unsigned short x_off = 60, y_off = 10;
	unsigned short cx = 0, cy = 0;

#ifndef __DVR_ANCHOR__
	m_picReset.Create(CTRLID_PICTURE, x, y, this, BMP_ICON_CONFIG_RESET);
	GUI::CWnd::TextSize(m_pStrTable->String(E_STR_ID_MENU_CFG_RESET), strlen(m_pStrTable->String(E_STR_ID_MENU_CFG_RESET)), cx, cy);
	m_anReset.Create(GUI::CWnd::GetRandWndID(), x+x_off, y+y_off, cx, cy, this, 0, 1, m_pStrTable->String(E_STR_ID_MENU_CFG_RESET));

	y += 60;
	char str [512] = {0};
	snprintf(str, 512, "%s / %s", m_pStrTable->String(E_STR_ID_IMPORT), m_pStrTable->String(E_STR_ID_EXPORT));

	m_picConfigMan.Create(CTRLID_PICTURE, x, y, this, BMP_ICON_CONFIG_MAN);
	GUI::CWnd::TextSize(str, strlen(str), cx, cy);
	m_anConfigMan.Create(GUI::CWnd::GetRandWndID(), x+x_off, y+y_off, cx, cy, this, 0, 2, str);

#if !defined(__TW01_RILI__)	
	y += 60;
	strncpy(str,m_pStrTable->String(E_STR_ID_BLACK_WHITE_LIST), 512);
	m_picBlackList.Create(CTRLID_PICTURE, x, y, this, BMP_ICON_CONFIG_BLACKLIST);
	GUI::CWnd::TextSize(str, strlen(str), cx, cy);
	m_anblackList.Create(GUI::CWnd::GetRandWndID(), x+x_off, y+y_off, cx, cy, this, 0, 3, str);
#endif

#ifdef __NETSERVER_HXHT__
	y += 60;
	m_picHxNetParam.Create(CTRLID_PICTURE, x, y, this, BMP_ICON_EXIT);
	GUI::CWnd::TextSize(m_pStrTable->String(E_STR_ID_MEGA_EYES), strlen(m_pStrTable->String(E_STR_ID_MEGA_EYES)), cx, cy);
	m_anHxNetParam.Create(GUI::CWnd::GetRandWndID(), x+x_off, y+y_off, cx, cy, this, 0, 4, m_pStrTable->String(E_STR_ID_MEGA_EYES));
#endif
	y += 60;
	m_picExit.Create(CTRLID_PICTURE, x, y, this, BMP_ICON_EXIT);
	GUI::CWnd::TextSize(m_pStrTable->String(E_STR_ID_EXIT), strlen(m_pStrTable->String(E_STR_ID_EXIT)), cx, cy);
	m_anExit.Create(GUI::CWnd::GetRandWndID(), x+x_off, y+y_off, cx, cy, this, 0, 4, m_pStrTable->String(E_STR_ID_EXIT));
#else
	cx = m_length[ANCHOR_WIDTH];
	cy = m_length[ANCHOR_HEIGHT];

	m_anReset.Create(GUI::CWnd::GetRandWndID(), x, y, cx, cy, this, 0, 1, m_pStrTable->String(E_STR_ID_MENU_CFG_RESET));
	m_anReset.SetImage(BMP_ANCHOR_RESET, BMP_ANCHOR_RESET_F, BMP_ANCHOR_RESET);

	y += 60;
	char str [512] = {0};
	snprintf(str, 512, "%s / %s", m_pStrTable->String(E_STR_ID_IMPORT), m_pStrTable->String(E_STR_ID_EXPORT));

	m_anConfigMan.Create(GUI::CWnd::GetRandWndID(), x, y, cx, cy, this, 0, 2, str);
	m_anConfigMan.SetImage(BMP_ANCHOR_IMPORT_EXPORT, BMP_ANCHOR_IMPORT_EXPORT_F, BMP_ANCHOR_IMPORT_EXPORT);

	//黑白名单划线按钮
#if !defined(__TW01_RILI__)	
	y += 60;
	strncpy(str, m_pStrTable->String(E_STR_ID_BLACK_WHITE_LIST), 512);
	m_anblackList.Create(GUI::CWnd::GetRandWndID(), x, y, cx, cy, this, 0, 3, str);
	m_anblackList.SetImage(BMP_ANCHOR_BLACKLIST, BMP_ANCHOR_BLACKLIST_F, BMP_ANCHOR_BLACKLIST);
#endif
	
	//互信互通网络配置
#ifdef __NETSERVER_HXHT__
	y += 60;
	m_anHxNetParam.Create(GUI::CWnd::GetRandWndID(), x, y, cx, cy, this, 0, 4, m_pStrTable->String(E_STR_ID_MEGA_EYES));
	m_anHxNetParam.SetImage(BMP_ANCHOR_EXIT, BMP_ANCHOR_EXIT_F, BMP_ANCHOR_EXIT);
#endif

	y += 60;
	m_anExit.Create(GUI::CWnd::GetRandWndID(), x, y, cx, cy, this, 0, 4, m_pStrTable->String(E_STR_ID_EXIT));
	m_anExit.SetImage(BMP_ANCHOR_EXIT, BMP_ANCHOR_EXIT_F, BMP_ANCHOR_EXIT);
#endif
	
	
}

BEGIN_MSG_MAP(CCfgAdvanceDlg, GUI::CDialog)
	ON_MSG(m_anReset.GetID(), KEY_VIRTUAL_ENTER, OnReset)
	ON_MSG(m_anConfigMan.GetID(), KEY_VIRTUAL_ENTER, OnConfigMan)
	ON_MSG(m_anExit.GetID(), KEY_VIRTUAL_ENTER, OnExit)
#if !defined(__TW01_RILI__)	
	ON_MSG(m_anblackList.GetID(), KEY_VIRTUAL_ENTER, OnBanList)
#endif

#ifdef __NETSERVER_HXHT__
	ON_MSG(m_anHxNetParam.GetID(), KEY_VIRTUAL_ENTER, OnCfgHxNetParam)
#endif
END_MSG_MAP()

unsigned long CCfgAdvanceDlg::OnReset()
{
	if (0 != CUserMan::Instance()->CheckAuthority(AUTH_ALL))
	{
		if( DLG_OK == MessageBox(m_pStrTable->String(E_STR_ID_MESSAGE_32), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK | DEFAULT_BTN_CANCEL| DEFAULT_ICON_WARNING) )
		{
			CMessageMan::Instance()->PostMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_EXFACTORY_SETUP, NULL, 0);
		}
	}
	else
	{
		MessageBox(m_pStrTable->String(E_STR_ID_MESSAGE_84), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK | DEFAULT_ICON_WARNING);
	}

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CCfgAdvanceDlg::OnConfigMan()
{
	m_configMan.DoModal(this, CENTER_POS, CENTER_POS, MAX_SCREEN_X, MAX_SCREEN_Y, false);

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CCfgAdvanceDlg::OnExit()
{
	EndDialog(DLG_ESC);
	return KEY_VIRTUAL_MSG_STOP;
}
unsigned long CCfgAdvanceDlg::OnBanList()
{


	m_banlistDlg.DoModal(NULL, CENTER_POS, CENTER_POS, MAX_SCREEN_X, MAX_SCREEN_Y, false);//先改为NULL


	return KEY_VIRTUAL_MSG_STOP;
	//end
}
#ifdef __NETSERVER_HXHT__
unsigned long CCfgAdvanceDlg::OnCfgHxNetParam()
{
	m_cfgHxNetParamCfgDlg.DoModal(this);

	return KEY_VIRTUAL_MSG_STOP;
}
#endif
//end
