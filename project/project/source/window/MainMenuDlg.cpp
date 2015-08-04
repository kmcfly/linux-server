/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : hz
** Date         : 2009-08-06
** Name         : MainMenuDlg.cpp
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/
#include "MainMenuDlg.h"
#include "FuncCustom.h"
#include "NetProtocol.h"
#ifdef __SUPPORT_SNAPD1__
#include "ShanghaiPTManDialog.h"
#include "ShanghaiConnectMan.h"
#endif

extern NET_LOGIN_SUCCESS_REPLY *g_p_login_succ;

extern LANGUAGE	g_language;

//////////////////////////////////////////////////////////////////////////
CMainMenuDlg::CMainMenuDlg()
{

}

CMainMenuDlg::~CMainMenuDlg()
{

}

//////////////////////////////////////////////////////////////////////////
void CMainMenuDlg::OnInitial()
{
	GUI::CDialog::OnInitial();
	SetImage(BMP_MEM_DLG_NORMAL, false);
	SetCaption(m_pStrTable->String(E_STR_ID_TITLE_MAIN_MENU), false);

	//////////////////////////////////////////////////////////////////////////
	CreateCtrl();
}

void CMainMenuDlg::OnPostInitial()
{
	GUI::CDialog::OnPostInitial();
	/////////////////////////////////////////////////////////////////////////
	m_btnExit.Create(GetRandWndID(), 
					m_rect.Width() - m_length[TITLE_EXIT_BTN_RIGHT_XPOS], 
					m_length[TITLE_EXIT_BTN_YPOS],
					m_length[TITLE_EXIT_BTN_WIDTH], 
					m_length[TITLE_EXIT_BTN_HEIGHT],
					this, 0, 0);

	m_btnExit.SetImage(PAINT_STATUS_NORMAL, BMP_BUTTON_DLG_EXIT, false);
	m_btnExit.SetImage(PAINT_STATUS_FOCUS, BMP_BUTTON_DLG_EXIT_F, false);
}

bool CMainMenuDlg::CreateCtrl()
{

	ULONGLONG CH = 0;
	int localVideoInputNum = 0, netVideoInputNum = 0;
	int maxChnn = 0, effectChnn = 0;

	IsSupportCH(CH, &effectChnn, &localVideoInputNum, &netVideoInputNum);

	maxChnn = localVideoInputNum + netVideoInputNum;

#ifndef __DVR_ANCHOR__
	unsigned short leftX = 50, topY = 60;
	unsigned short size = 72, xOffset = 78, yOffset = 120;
	unsigned short x = leftX, y = topY, cx = size, cy = size, stH = 25;
	unsigned short xCnt = 0;
	int xFocus = 0, yFocus = 1;

	m_btn[0].Create(CTRLID_MAINMENU_CONFIG, x, y, cx, cy, this, xFocus, yFocus, NULL, WND_CHILD | WND_VISIBLE | WND_NO_IMAGE_BK);
	m_btn[0].SetImage(PAINT_STATUS_NORMAL, BMP_CONFIG, false);
	m_btn[0].SetImage(PAINT_STATUS_FOCUS, BMP_CONFIG_FOCUS, false);
	m_btn[0].SetImage(PAINT_STATUS_DISABLE, BMP_CONFIG, false);
	m_static[0].Create(CTRLID_STATIC, x - size/2, y+size, size*2, stH, this, m_pStrTable->String(E_STR_ID_MENU_CONFIG));
	m_static[0].SetAlignText(GUI::ALIGN_TEXT_CENTER);
	
	++ xCnt;
	x = (0 == (xCnt % 4)) ? leftX : (x + size + xOffset);
	y = (0 == (xCnt % 4)) ? (y + yOffset) : y;
	xCnt = (xCnt % 4);
	
	++ xFocus;
	yFocus = (0 == (xFocus % 4)) ? (yFocus + 1) : yFocus;
	xFocus = (xFocus % 4);
	
	m_btn[1].Create(CTRLID_MAINMENU_SEARCH, x, y, cx, cy, this, xFocus, yFocus, NULL, WND_CHILD | WND_VISIBLE | WND_NO_IMAGE_BK);
	m_btn[1].SetImage(PAINT_STATUS_NORMAL, BMP_SEARCH, false);
	m_btn[1].SetImage(PAINT_STATUS_FOCUS, BMP_SEARCH_FOCUS, false);
	m_btn[1].SetImage(PAINT_STATUS_DISABLE, BMP_SEARCH, false);
	m_static[1].Create(CTRLID_STATIC, x - size/2, y+size, size*2, stH, this, m_pStrTable->String(E_STR_ID_MENU_SEARCH));
	m_static[1].SetAlignText(GUI::ALIGN_TEXT_CENTER);

	++ xCnt;
	x = (0 == (xCnt % 4)) ? leftX : (x + size + xOffset);
	y = (0 == (xCnt % 4)) ? (y + yOffset) : y;
	xCnt = (xCnt % 4);
	
	++ xFocus;
	yFocus = (0 == (xFocus % 4)) ? (yFocus + 1) : yFocus;
	xFocus = (xFocus % 4);

	m_btn[2].Create(CTRLID_MAINMENU_BACKUP, x, y, cx, cy, this, xFocus, yFocus, NULL, WND_CHILD | WND_VISIBLE | WND_NO_IMAGE_BK);
	m_btn[2].SetImage(PAINT_STATUS_NORMAL, BMP_BACKUP, false);
	m_btn[2].SetImage(PAINT_STATUS_FOCUS, BMP_BACKUP_FOCUS, false);
	m_btn[2].SetImage(PAINT_STATUS_DISABLE, BMP_BACKUP, false);
	m_static[2].Create(CTRLID_STATIC, x - size/2, y+size, size*2, stH, this, m_pStrTable->String(E_STR_ID_MENU_BACKUP));
	m_static[2].SetAlignText(GUI::ALIGN_TEXT_CENTER);

	++ xCnt;
	x = (0 == (xCnt % 4)) ? leftX : (x + size + xOffset);
	y = (0 == (xCnt % 4)) ? (y + yOffset) : y;
	xCnt = (xCnt % 4);

	++ xFocus;
	yFocus = (0 == (xFocus % 4)) ? (yFocus + 1) : yFocus;
	xFocus = (xFocus % 4);

	m_btn[3].Create(CTRLID_MAINMENU_INFO, x, y, cx, cy, this, xFocus, yFocus, NULL, WND_CHILD | WND_VISIBLE | WND_NO_IMAGE_BK);
	m_btn[3].SetImage(PAINT_STATUS_NORMAL, BMP_INFO, false);
	m_btn[3].SetImage(PAINT_STATUS_FOCUS, BMP_INFO_FOCUS, false);
	m_btn[3].SetImage(PAINT_STATUS_DISABLE, BMP_INFO, false);
	m_static[3].Create(CTRLID_STATIC, x - size/2, y+size, size*2, stH, this, m_pStrTable->String(E_STR_ID_MENU_INFO));
	m_static[3].SetAlignText(GUI::ALIGN_TEXT_CENTER);


	if (IsSupportAlarmOut())
	{
		++ xCnt;
		x = (0 == (xCnt % 4)) ? leftX : (x + size + xOffset);
		y = (0 == (xCnt % 4)) ? (y + yOffset) : y;
		xCnt = (xCnt % 4);

		++ xFocus;
		yFocus = (0 == (xFocus % 4)) ? (yFocus + 1) : yFocus;
		xFocus = (xFocus % 4);

		m_btn[4].Create(CTRLID_MAINMENU_MANUAL_ALARM, x, y, cx, cy, this, xFocus, yFocus, NULL, WND_CHILD | WND_VISIBLE | WND_NO_IMAGE_BK);
		m_btn[4].SetImage(PAINT_STATUS_NORMAL, BMP_MANUAL_ALARM, false);
		m_btn[4].SetImage(PAINT_STATUS_FOCUS, BMP_MANUAL_ALARM_FOCUS, false);
		m_btn[4].SetImage(PAINT_STATUS_DISABLE, BMP_MANUAL_ALARM, false);
		m_static[4].Create(CTRLID_STATIC, x - size/2, y+size, size*2, stH, this, m_pStrTable->String(E_STR_ID_MENU_MANUAL_ALARM));//手动报警
		m_static[4].SetAlignText(GUI::ALIGN_TEXT_CENTER);
	}


	++ xCnt;
	x = (0 == (xCnt % 4)) ? leftX : (x + size + xOffset);
	y = (0 == (xCnt % 4)) ? (y + yOffset) : y;
	xCnt = (xCnt % 4);

	++ xFocus;
	yFocus = (0 == (xFocus % 4)) ? (yFocus + 1) : yFocus;
	xFocus = (xFocus % 4);

	m_btn[5].Create(CTRLID_MAINMENU_DISK_MAN, x, y, cx, cy, this, xFocus, yFocus, NULL, WND_CHILD | WND_VISIBLE | WND_NO_IMAGE_BK);
	m_btn[5].SetImage(PAINT_STATUS_NORMAL, BMP_DISK, false);
	m_btn[5].SetImage(PAINT_STATUS_FOCUS, BMP_DISK_FOCUS, false);
	m_btn[5].SetImage(PAINT_STATUS_DISABLE, BMP_DISK, false);
	m_static[5].Create(CTRLID_STATIC, x - size/2, y+size, size*2, stH, this, m_pStrTable->String(E_STR_ID_MENU_DISK_MAN));
	m_static[5].SetAlignText(GUI::ALIGN_TEXT_CENTER);

	++ xCnt;
	x = (0 == (xCnt % 4)) ? leftX : (x + size + xOffset);
	y = (0 == (xCnt % 4)) ? (y + yOffset) : y;
	xCnt = (xCnt % 4);

	++ xFocus;
	yFocus = (0 == (xFocus % 4)) ? (yFocus + 1) : yFocus;
	xFocus = (xFocus % 4);

	m_btn[6].Create(CTRLID_MAINMENU_SYSTEM_RESOURCES, x, y, cx, cy, this, xFocus, yFocus, NULL, WND_CHILD | WND_VISIBLE | WND_NO_IMAGE_BK);
	m_btn[6].SetImage(PAINT_STATUS_NORMAL, BMP_RESOURCES, false);
	m_btn[6].SetImage(PAINT_STATUS_FOCUS, BMP_RESOURCES_FOCUS, false);
	m_btn[6].SetImage(PAINT_STATUS_DISABLE, BMP_RESOURCES, false);
	#ifdef __CUSTOM_US02_NEW__
		m_static[6].Create(CTRLID_STATIC, x - size/2, y+size, size*2, stH, this, m_pStrTable->String(E_STR_ID_UPGRADE_01));
	#else
		m_static[6].Create(CTRLID_STATIC, x - size/2, y+size, size*2, stH, this, m_pStrTable->String(E_STR_ID_MENU_UPGRADE));
	#endif
	m_static[6].SetAlignText(GUI::ALIGN_TEXT_CENTER);

	if(netVideoInputNum > 0)
	{
		++ xCnt;
		x = (0 == (xCnt % 4)) ? leftX : (x + size + xOffset);
		y = (0 == (xCnt % 4)) ? (y + yOffset) : y;
		xCnt = (xCnt % 4);

		++ xFocus;
		yFocus = (0 == (xFocus % 4)) ? (yFocus + 1) : yFocus;
		xFocus = (xFocus % 4);

		m_btn[7].Create(CTRLID_MAINMENU_DEVICE_MAN, x, y, cx, cy, this, xFocus, yFocus, NULL, WND_CHILD | WND_VISIBLE | WND_NO_IMAGE_BK);
		m_btn[7].SetImage(PAINT_STATUS_NORMAL, BMP_DEVICE, false);
		m_btn[7].SetImage(PAINT_STATUS_FOCUS, BMP_DEVICE_FOCUS, false);
		m_btn[7].SetImage(PAINT_STATUS_DISABLE, BMP_RESOURCES, false);
		m_static[7].Create(CTRLID_STATIC, x - size/2, y+size, size*2, stH, this, m_pStrTable->String(E_STR_ID_MENU_IP_CAMERA) );
		m_static[7].SetAlignText(GUI::ALIGN_TEXT_CENTER);
	}

	++ xCnt;
	x = (0 == (xCnt % 4)) ? leftX : (x + size + xOffset);
	y = (0 == (xCnt % 4)) ? (y + yOffset) : y;
	xCnt = (xCnt % 4);

	++ xFocus;
	yFocus = (0 == (xFocus % 4)) ? (yFocus + 1) : yFocus;
	xFocus = (xFocus % 4);

	m_btn[8].Create(CTRLID_MAINMENU_LOGOFF, x, y, cx, cy, this, xFocus, yFocus, NULL, WND_CHILD | WND_VISIBLE | WND_NO_IMAGE_BK);
	m_btn[8].SetImage(PAINT_STATUS_NORMAL, BMP_LOGOFF, false);
	m_btn[8].SetImage(PAINT_STATUS_FOCUS, BMP_LOGOFF_FOCUS, false);
	m_btn[8].SetImage(PAINT_STATUS_DISABLE, BMP_SHUTDOWN, false);
	m_static[8].Create(CTRLID_STATIC, x - size/2, y+size, size*2, stH, this, m_pStrTable->String(E_STR_ID_MENU_LOGOFF));
	m_static[8].SetAlignText(GUI::ALIGN_TEXT_CENTER);

#ifdef __SUPPORT_SNAPD1__
	{
			++ xCnt;
			x = (0 == (xCnt % 4)) ? leftX : (x + size + xOffset);
			y = (0 == (xCnt % 4)) ? (y + yOffset) : y;
			xCnt = (xCnt % 4);

			++ xFocus;
			yFocus = (0 == (xFocus % 4)) ? (yFocus + 1) : yFocus;
			xFocus = (xFocus % 4);
		
			m_btnShanghaiMan.Create(GetRandWndID(), x, y, cx, cy, this, xFocus, yFocus, NULL, WND_CHILD | WND_VISIBLE | WND_NO_IMAGE_BK);
			m_btnShanghaiMan.SetImage(PAINT_STATUS_NORMAL, BMP_PEN, false);
			m_btnShanghaiMan.SetImage(PAINT_STATUS_FOCUS, BMP_PEN_FOCUS, false);
			m_btnShanghaiMan.SetImage(PAINT_STATUS_DISABLE, BMP_PEN, false);
			std::string outStr;
			outStr.clear();
			if (-1 == CCharCodeTable::Instance()->Convert(outStr, CCT_CODE_UTF8, "平台管理", CCT_CODE_GBK))
			{
				outStr = "平台管理";
			}
			
			m_staticShanghaiMan.Create(CTRLID_STATIC, x - size/2, y+size, size*2, stH, this, outStr.c_str());
			m_staticShanghaiMan.SetAlignText(GUI::ALIGN_TEXT_CENTER);
	}
#endif
	if(IsSupportShutDown())
	{
		++ xCnt;
		x = (0 == (xCnt % 4)) ? leftX : (x + size + xOffset);
		y = (0 == (xCnt % 4)) ? (y + yOffset) : y;
		xCnt = (xCnt % 4);

		++ xFocus;
		yFocus = (0 == (xFocus % 4)) ? (yFocus + 1) : yFocus;
		xFocus = (xFocus % 4);

		m_btn[9].Create(CTRLID_MAINMENU_SHUTDOWN, x, y, cx, cy, this, xFocus, yFocus, NULL, WND_CHILD | WND_VISIBLE | WND_NO_IMAGE_BK);
		m_btn[9].SetImage(PAINT_STATUS_NORMAL, BMP_SHUTDOWN, false);
		m_btn[9].SetImage(PAINT_STATUS_FOCUS, BMP_SHUTDOWN_FOCUS, false);
		m_btn[9].SetImage(PAINT_STATUS_DISABLE, BMP_SHUTDOWN, false);
		m_static[9].Create(CTRLID_STATIC, x - size/2, y+size, size*2, stH, this, m_pStrTable->String(E_STR_ID_MENU_SHUT_DOWN));
		m_static[9].SetAlignText(GUI::ALIGN_TEXT_CENTER);
	}
#else
	unsigned short x = 80;
	unsigned short y = m_length[TITLE_HEIGHT] + m_length[DLG_OFFSET_TOP] + 25;
	unsigned short x_off = 60, y_off = 10;
	unsigned short cx = m_length[ANCHOR_WIDTH], cy = m_length[ANCHOR_HEIGHT];

	int yFocus = 1;
	int row = 0;

	row++;
	m_btn[0].Create(GUI::CWnd::GetRandWndID(), x, y, cx, cy, this, 0, yFocus++, m_pStrTable->String(E_STR_ID_MENU_CONFIG));
	m_btn[0].SetImage(BMP_ANCHOR_CONFIG, BMP_ANCHOR_CONFIG_F, BMP_ANCHOR_CONFIG);

	row++;
	y += 60;
	m_btn[1].Create(GUI::CWnd::GetRandWndID(), x, y, cx, cy, this, 0, yFocus++, m_pStrTable->String(E_STR_ID_MENU_SEARCH));
	m_btn[1].SetImage(BMP_ANCHOR_SEARCH, BMP_ANCHOR_SEARCH_F, BMP_ANCHOR_SEARCH);

	row++;
	y += 60;
	m_btn[2].Create(GUI::CWnd::GetRandWndID(), x, y, cx, cy, this, 0, yFocus++, m_pStrTable->String(E_STR_ID_MENU_BACKUP));
	m_btn[2].SetImage(BMP_ANCHOR_BACKUP, BMP_ANCHOR_BACKUP_F, BMP_ANCHOR_BACKUP);

	row++;
	y += 60;
	m_btn[3].Create(GUI::CWnd::GetRandWndID(), x, y, cx, cy, this, 0, yFocus++, m_pStrTable->String(E_STR_ID_MENU_INFO));
	m_btn[3].SetImage(BMP_ANCHOR_INFO, BMP_ANCHOR_INFO_F, BMP_ANCHOR_INFO);

	if (IsSupportAlarmOut())
	{
		row++;
		y += 60;
		m_btn[4].Create(GUI::CWnd::GetRandWndID(), x, y, cx, cy, this, 0, yFocus++, m_pStrTable->String(E_STR_ID_MENU_MANUAL_ALARM));
		m_btn[4].SetImage(BMP_ANCHOR_MANUAL, BMP_ANCHOR_MANUAL_F, BMP_ANCHOR_MANUAL);
	}

	if(row >= 5)
	{
		row = 0;
		x = m_rect.Width() / 2 + 40;
		y = m_length[TITLE_HEIGHT] + m_length[DLG_OFFSET_TOP] + 25 - 60;
	}

	row++;
	y += 60;
	m_btn[5].Create(GUI::CWnd::GetRandWndID(), x, y, cx, cy, this, 0, yFocus++, m_pStrTable->String(E_STR_ID_MENU_DISK_MAN));
	m_btn[5].SetImage(BMP_ANCHOR_DISK, BMP_ANCHOR_DISK_F, BMP_ANCHOR_DISK);

	if(row >= 5)
	{
		row = 0;
		x = m_rect.Width() / 2 + 40;
		y = m_length[TITLE_HEIGHT] + m_length[DLG_OFFSET_TOP] + 25 - 60;
	}

	row++;
	y += 60;
	m_btn[6].Create(GUI::CWnd::GetRandWndID(), x, y, cx, cy, this, 0, yFocus++, m_pStrTable->String(E_STR_ID_MENU_UPGRADE));
	m_btn[6].SetImage(BMP_ANCHOR_UPGRADE, BMP_ANCHOR_UPGRADE_F, BMP_ANCHOR_UPGRADE);

	if(netVideoInputNum > 0)
	{
		row++;
		y += 60;
		m_btn[7].Create(GUI::CWnd::GetRandWndID(), x, y, cx, cy, this, 0, yFocus++, m_pStrTable->String(E_STR_ID_MENU_IP_CAMERA));
		m_btn[7].SetImage(BMP_ANCHOR_NET_DEVICE, BMP_ANCHOR_NET_DEVICE_F, BMP_ANCHOR_CONFIG);
	}

	row++;
	y += 60;
	m_btn[8].Create(GUI::CWnd::GetRandWndID(), x, y, cx, cy, this, 0, yFocus++, m_pStrTable->String(E_STR_ID_MENU_LOGOFF));
	m_btn[8].SetImage(BMP_ANCHOR_LOGOFF, BMP_ANCHOR_LOGOFF_F, BMP_ANCHOR_LOGOFF);

	if(IsSupportShutDown())
	{
		row++;
		y += 60;
		m_btn[9].Create(GUI::CWnd::GetRandWndID(), x, y, cx, cy, this, 0, yFocus++, m_pStrTable->String(E_STR_ID_MENU_SHUT_DOWN));
		m_btn[9].SetImage(BMP_ANCHOR_SHUTDOWN, BMP_ANCHOR_SHUTDOWN_F, BMP_ANCHOR_SHUTDOWN);
	}
#endif

#if defined(__DVR_LOGO__) || defined(__ENVIRONMENT_WIN32__)
	int logoCx = 0, logoCy = 0;
	GetImageSize(BMP_ICON_LOGO, logoCx, logoCy);

	x = m_rect.Width() - m_length[DLG_OFFSET_LEFT] - logoCx;
	y = m_rect.Height() - m_length[DLG_OFFSET_BOTTOM] - logoCy - 8;

#ifndef __TW01_RILI__
	m_logoPicCtrl.Create(GetRandWndID(), x, y, this, BMP_ICON_LOGO);
#endif
#endif

	return true;
}

BEGIN_MSG_MAP(CMainMenuDlg, GUI::CDialog)
	ON_MSG(m_btn[0].GetID(), KEY_VIRTUAL_ENTER, OnBtnConfig)
	ON_MSG(m_btn[1].GetID(), KEY_VIRTUAL_ENTER, OnBtnSearch)
	ON_MSG(m_btn[2].GetID(), KEY_VIRTUAL_ENTER, OnBtnBackup)
	ON_MSG(m_btn[3].GetID(), KEY_VIRTUAL_ENTER, OnBtnInfo)
	ON_MSG(m_btn[4].GetID(), KEY_VIRTUAL_ENTER, OnBtnManualAlarm)
	ON_MSG(m_btn[5].GetID(), KEY_VIRTUAL_ENTER, OnBtnDiskMan)
	ON_MSG(m_btn[6].GetID(), KEY_VIRTUAL_ENTER, OnBtnSysResources)
	ON_MSG(m_btn[7].GetID(), KEY_VIRTUAL_ENTER, OnBtnDeviceMan)
	ON_MSG(m_btn[8].GetID(), KEY_VIRTUAL_ENTER, OnBtnLogoff)
	ON_MSG(m_btn[9].GetID(), KEY_VIRTUAL_ENTER, OnBtnShutdown)
	ON_MSG(m_btnExit.GetID(), KEY_VIRTUAL_ENTER, OnBtnExit)
#ifdef __SUPPORT_SNAPD1__
	ON_MSG(m_btnShanghaiMan.GetID(),KEY_VIRTUAL_ENTER, OnBtnShanghaiPTMan)
#endif
END_MSG_MAP()

unsigned long CMainMenuDlg::OnBtnConfig()
{
	return KEY_VIRTUAL_MSG_DEFAULT_TAG | CTRLID_MAINMENU_CONFIG;
}

unsigned long CMainMenuDlg::OnBtnSearch()
{
	return KEY_VIRTUAL_SEARCH;
}

unsigned long CMainMenuDlg::OnBtnBackup()
{
	return KEY_VIRTUAL_BACKUP;
}

unsigned long CMainMenuDlg::OnBtnInfo()
{
	return KEY_VIRTUAL_INFO;
}

unsigned long CMainMenuDlg::OnBtnManualAlarm()
{
	return KEY_VIRTUAL_MSG_DEFAULT_TAG | CTRLID_MAINMENU_MANUAL_ALARM;
}

unsigned long CMainMenuDlg::OnBtnDiskMan()
{
	return KEY_VIRTUAL_MSG_DEFAULT_TAG | CTRLID_MAINMENU_DISK_MAN;
}

unsigned long CMainMenuDlg::OnBtnSysResources()
{
	return KEY_VIRTUAL_MSG_DEFAULT_TAG | CTRLID_MAINMENU_SYSTEM_RESOURCES;
}

unsigned long CMainMenuDlg::OnBtnDeviceMan()
{
	return KEY_VIRTUAL_MSG_DEFAULT_TAG | CTRLID_MAINMENU_DEVICE_MAN;
}

unsigned long CMainMenuDlg::OnBtnLogoff()
{
	return KEY_VIRTUAL_MSG_DEFAULT_TAG | CTRLID_MAINMENU_LOGOFF;
}

unsigned long CMainMenuDlg::OnBtnShutdown()
{

	return KEY_VIRTUAL_POWER;
}

unsigned long CMainMenuDlg::OnBtnExit()
{
	return KEY_VIRTUAL_ESC;
}

#ifdef __SUPPORT_SNAPD1__
unsigned long CMainMenuDlg::OnBtnShanghaiPTMan()
{
	CShanghaiPTManDialog pTDialog;
	pTDialog.DoModal();
	return KEY_VIRTUAL_MSG_STOP;
}
#endif
//end
