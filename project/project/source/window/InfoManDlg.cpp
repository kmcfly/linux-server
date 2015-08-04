/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : ysw
** Date         : 2009-08-06
** Name         : InfoManDlg.cpp
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/
#include "InfoManDlg.h"
#include "UserMan.h"
#include "LocalDevice.h"
#ifdef __SUPPORT_SHOW_QRCODE__
#include "QRCode.h"
#include "NetProtocol.h"
#include "MessageMan.h"
#include "UserMan.h"
#endif
CInfoManDlg::CInfoManDlg()
{

}

CInfoManDlg::~CInfoManDlg()
{

}

//////////////////////////////////////////////////////////////////////////
void CInfoManDlg::OnInitial()
{
	GUI::CDialog::OnInitial();
	SetImage(BMP_MEM_DLG_NORMAL, false);
	SetCaption(m_pStrTable->String(E_STR_ID_TITLE_INFORMATION), false);
	//////////////////////////////////////////////////////////////////////////
	int xFocus = 0;
	int yFocus = 0;
#ifndef __DVR_ANCHOR__
	unsigned short size_btn = 72;
	unsigned short size_text = 150;
	unsigned short offset_text = (size_text - size_btn) / 2;

	unsigned short base_x = 10;
	unsigned short x = base_x;
	unsigned short y = 60;
	m_btnSystem.Create(CTRLID_INFO_BTN_SYSTEM, x + offset_text, y, size_btn, size_btn, this, 0, 1, NULL, WND_CHILD | WND_VISIBLE | WND_NO_IMAGE_BK);
	m_btnSystem.SetImage(PAINT_STATUS_NORMAL, BMP_SYSTEM, false);
	m_btnSystem.SetImage(PAINT_STATUS_DISABLE, BMP_SYSTEM, false);
	m_btnSystem.SetImage(PAINT_STATUS_FOCUS, BMP_SYSTEM_FOCUS, false);
	m_stcSystem.Create(CTRLID_STATIC, x, y+size_btn, size_text, m_length[STATIC_HEIGHT], this, m_pStrTable->String(E_STR_ID_MENU_INFO_SYSTEM));
	m_stcSystem.SetAlignText(GUI::ALIGN_TEXT_CENTER);

	x += size_text;
	m_btnEvent.Create(CTRLID_INFO_BTN_EVENT, x + offset_text, y, size_btn, size_btn, this, 1, 1, NULL, WND_CHILD | WND_VISIBLE | WND_NO_IMAGE_BK);
	m_btnEvent.SetImage(PAINT_STATUS_NORMAL, BMP_EVENT, false);
	m_btnEvent.SetImage(PAINT_STATUS_DISABLE, BMP_EVENT, false);
	m_btnEvent.SetImage(PAINT_STATUS_FOCUS, BMP_EVENT_FOCUS, false);
	m_stcEvent.Create(CTRLID_STATIC, x, y+size_btn, size_text, m_length[STATIC_HEIGHT], this, m_pStrTable->String(E_STR_ID_MENU_INFO_EVENT));
	m_stcEvent.SetAlignText(GUI::ALIGN_TEXT_CENTER);

	x += size_text;
	m_btnLog.Create(CTRLID_INFO_BTN_LOG, x + offset_text, y, size_btn, size_btn, this, 2, 1, NULL, WND_CHILD | WND_VISIBLE | WND_NO_IMAGE_BK);
	m_btnLog.SetImage(PAINT_STATUS_NORMAL, BMP_LOG, false);
	m_btnLog.SetImage(PAINT_STATUS_DISABLE, BMP_LOG, false);
	m_btnLog.SetImage(PAINT_STATUS_FOCUS, BMP_LOG_FOCUS, false);
	m_stcLog.Create(CTRLID_STATIC, x, y+size_btn, size_text, m_length[STATIC_HEIGHT], this, m_pStrTable->String(E_STR_ID_MENU_INFO_LOG));
	m_stcLog.SetAlignText(GUI::ALIGN_TEXT_CENTER);

#if !defined(__TW01_RILI__)	
	x += size_text;
	m_btnNetwork.Create(CTRLID_INFO_BTN_NETWORK, x + offset_text, y, size_btn, size_btn, this, 3, 1, NULL, WND_CHILD | WND_VISIBLE | WND_NO_IMAGE_BK);
	m_btnNetwork.SetImage(PAINT_STATUS_NORMAL, BMP_NETWORK, false);
	m_btnNetwork.SetImage(PAINT_STATUS_DISABLE, BMP_NETWORK, false);
	m_btnNetwork.SetImage(PAINT_STATUS_FOCUS, BMP_NETWORK_FOCUS, false);
	m_stcNetwork.Create(CTRLID_STATIC, x, y+size_btn, size_text, m_length[STATIC_HEIGHT], this, m_pStrTable->String(E_STR_ID_MENU_INFO_NETWORK));
	m_stcNetwork.SetAlignText(GUI::ALIGN_TEXT_CENTER);

	x = base_x;
	y += 120;
	m_btnOnlineUser.Create(CTRLID_INFO_BTN_ONLINE_USER, x + offset_text, y, size_btn, size_btn, this, 0, 2, NULL, WND_CHILD | WND_VISIBLE | WND_NO_IMAGE_BK);
	m_btnOnlineUser.SetImage(PAINT_STATUS_NORMAL, BMP_ONLINE_USER, false);
	m_btnOnlineUser.SetImage(PAINT_STATUS_DISABLE, BMP_ONLINE_USER, false);
	m_btnOnlineUser.SetImage(PAINT_STATUS_FOCUS, BMP_ONLINE_USER_FOCUS, false);
	m_stcOnlineUser.Create(CTRLID_STATIC, x , y+size_btn, size_text, m_length[STATIC_HEIGHT], this, m_pStrTable->String(E_STR_ID_MENU_INFO_ONLINE_USER));
	m_stcOnlineUser.SetAlignText(GUI::ALIGN_TEXT_CENTER);
#endif

	x += size_text;
	m_btnDevice.Create(CTRLID_INFO_BTN_DEVICE, x + offset_text, y, size_btn, size_btn, this, 1, 2, NULL, WND_CHILD | WND_VISIBLE | WND_NO_IMAGE_BK);
	m_btnDevice.SetImage(PAINT_STATUS_NORMAL, BMP_REC_INFO, false);
	m_btnDevice.SetImage(PAINT_STATUS_DISABLE, BMP_REC_INFO, false);
	m_btnDevice.SetImage(PAINT_STATUS_FOCUS, BMP_REC_INFO_FOCUS, false);
	m_stcDevice.Create(CTRLID_STATIC, x, y+size_btn, size_text, m_length[STATIC_HEIGHT], this, m_pStrTable->String(E_STR_ID_MENU_CFG_RECORD));
	m_stcDevice.SetAlignText(GUI::ALIGN_TEXT_CENTER);

	xFocus = 2;
	yFocus = 2;	
#ifdef __SUPPORT_SHOW_QRCODE__
	x += size_text;
	m_btnqrCode.Create(GetRandWndID(), x + offset_text, y, size_btn, size_btn, this, 2, 2, NULL, WND_CHILD | WND_VISIBLE | WND_NO_IMAGE_BK);
	m_btnqrCode.SetImage(PAINT_STATUS_NORMAL, BMP_QRCODE, false);
	m_btnqrCode.SetImage(PAINT_STATUS_DISABLE, BMP_QRCODE, false);
	m_btnqrCode.SetImage(PAINT_STATUS_FOCUS, BMP_QRCODE_FOCUS, false);
	m_stcqrCode.Create(CTRLID_STATIC, x, y+size_btn, size_text, m_length[STATIC_HEIGHT], this,"QRCODE");
	m_stcqrCode.SetAlignText(GUI::ALIGN_TEXT_CENTER);
	xFocus +=1;
#endif

#ifdef __SUPPORT_APP_INFO_QRCODE__
	m_appInfoQrCodeDlg = CAppInfoQrCodeDlg::Instance();
	x += size_text;
	m_btnAppInfo.Create(GetRandWndID(), x + offset_text, y, size_btn, size_btn, this, xFocus, yFocus, NULL, WND_CHILD | WND_VISIBLE | WND_NO_IMAGE_BK);
	m_btnAppInfo.SetImage(PAINT_STATUS_NORMAL, BMP_APP_INFO_QRCODE, false);
	m_btnAppInfo.SetImage(PAINT_STATUS_DISABLE, BMP_APP_INFO_QRCODE, false);
	m_btnAppInfo.SetImage(PAINT_STATUS_FOCUS, BMP_APP_INFO_QRCODE_FOCUS, false);
	m_stcAppInfoQrCode.Create(CTRLID_STATIC, x, y+size_btn, size_text, m_length[STATIC_HEIGHT], this, "AppInfo");
	m_stcAppInfoQrCode.SetAlignText(GUI::ALIGN_TEXT_CENTER);
	xFocus +=1;
#endif
	if(xFocus > 3)
	{
		xFocus = 0;
		yFocus +=1;
		x = base_x;
		y += 120;
	}
	else
	{
		x += size_text;
		xFocus++;
	}
	m_btnExit.Create(CTRLID_INFO_BTN_EXIT, x + offset_text, y, size_btn, size_btn, this,xFocus, yFocus, NULL, WND_CHILD | WND_VISIBLE | WND_NO_IMAGE_BK);
	m_btnExit.SetImage(PAINT_STATUS_NORMAL, BMP_BTN_EXIT3, false);
	m_btnExit.SetImage(PAINT_STATUS_DISABLE, BMP_BTN_EXIT3, false);
	m_btnExit.SetImage(PAINT_STATUS_FOCUS, BMP_BTN_EXIT3_FOCUS, false);
	m_stcExit.Create(CTRLID_STATIC, x, y+size_btn, size_text, m_length[STATIC_HEIGHT], this, m_pStrTable->String(E_STR_ID_EXIT));
	m_stcExit.SetAlignText(GUI::ALIGN_TEXT_CENTER);
	
#else
	unsigned short x = 80;
	unsigned short y = m_length[TITLE_HEIGHT] + m_length[DLG_OFFSET_TOP] + 25;
	unsigned short x_off = 60, y_off = 10;
	unsigned short cx = m_length[ANCHOR_WIDTH], cy = m_length[ANCHOR_HEIGHT];

	yFocus = 1;

	m_btnSystem.Create(GUI::CWnd::GetRandWndID(), x, y, cx, cy, this, 0, yFocus++, m_pStrTable->String(E_STR_ID_MENU_INFO_SYSTEM));
	m_btnSystem.SetImage(BMP_ANCHOR_SYSTEM, BMP_ANCHOR_SYSTEM_F, BMP_ANCHOR_SYSTEM);

	y += 60;
	m_btnEvent.Create(GUI::CWnd::GetRandWndID(), x, y, cx, cy, this, 0, yFocus++, m_pStrTable->String(E_STR_ID_MENU_INFO_EVENT));
	m_btnEvent.SetImage(BMP_ANCHOR_EVENT, BMP_ANCHOR_EVENT_F, BMP_ANCHOR_EVENT);

	y += 60;
	m_btnLog.Create(GUI::CWnd::GetRandWndID(), x, y, cx, cy, this, 0, yFocus++, m_pStrTable->String(E_STR_ID_MENU_INFO_LOG));
	m_btnLog.SetImage(BMP_ANCHOR_LOG, BMP_ANCHOR_LOG_F, BMP_ANCHOR_LOG);

#if !defined(__TW01_RILI__)	
	y += 60;
	m_btnNetwork.Create(GUI::CWnd::GetRandWndID(), x, y, cx, cy, this, 0, yFocus++, m_pStrTable->String(E_STR_ID_MENU_INFO_NETWORK));
	m_btnNetwork.SetImage(BMP_ANCHOR_NETWORK, BMP_ANCHOR_NETWORK_F, BMP_ANCHOR_NETWORK);

	y += 60;
	m_btnOnlineUser.Create(GUI::CWnd::GetRandWndID(), x, y, cx, cy, this, 0, yFocus++, m_pStrTable->String(E_STR_ID_MENU_INFO_ONLINE_USER));
	m_btnOnlineUser.SetImage(BMP_ANCHOR_ONLINE_USERS, BMP_ANCHOR_ONLINE_USERS_F, BMP_ANCHOR_ONLINE_USERS);

	//第二列

	x = m_rect.Width() / 2 + 40;
	y = m_length[TITLE_HEIGHT] + m_length[DLG_OFFSET_TOP] + 25 - 60;
#endif

	y += 60;
	m_btnDevice.Create(GUI::CWnd::GetRandWndID(), x, y, cx, cy, this, 0, yFocus++, m_pStrTable->String(E_STR_ID_MENU_CFG_RECORD));
	m_btnDevice.SetImage(BMP_ANCHOR_RECORD, BMP_ANCHOR_RECORD_F, BMP_ANCHOR_RECORD);

#ifdef __SUPPORT_SHOW_QRCODE__
	y += 60;

	m_btnqrCode.Create(GUI::CWnd::GetRandWndID(), x, y, cx, cy, this, 0, yFocus++, "Qrcode");
	m_btnqrCode.SetImage(BMP_QRCODE,  BMP_QRCODE_FOCUS, BMP_QRCODE);
#endif

#ifdef __SUPPORT_APP_INFO_QRCODE__
	y += 60;
	m_btnAppInfo.Create(GUI::CWnd::GetRandWndID(), x, y, cx, cy, this, 0, yFocus++, "AppInfo");
	m_btnAppInfo.SetImage(BMP_APP_INFO_QRCODE, BMP_APP_INFO_QRCODE_FOCUS, BMP_APP_INFO_QRCODE);	
#endif

	y += 60;
	m_btnExit.Create(GUI::CWnd::GetRandWndID(), x, y, cx, cy, this, 0, yFocus++, m_pStrTable->String(E_STR_ID_EXIT));
	m_btnExit.SetImage(BMP_ANCHOR_EXIT, BMP_ANCHOR_EXIT_F, BMP_ANCHOR_EXIT);

#endif
}

BEGIN_MSG_MAP(CInfoManDlg, GUI::CDialog)
	ON_MSG(m_btnSystem.GetID(), KEY_VIRTUAL_ENTER, OnSystem)
	ON_MSG(m_btnEvent.GetID(), KEY_VIRTUAL_ENTER,	OnEvent)
	ON_MSG(m_btnLog.GetID(), KEY_VIRTUAL_ENTER, OnLog)
#if !defined(__TW01_RILI__)	
	ON_MSG(m_btnNetwork.GetID(), KEY_VIRTUAL_ENTER,	OnNetwork)
	ON_MSG(m_btnOnlineUser.GetID(), KEY_VIRTUAL_ENTER, OnOnlineUser)
#endif
	ON_MSG(m_btnDevice.GetID(), KEY_VIRTUAL_ENTER, OnDevice)
	ON_MSG(m_btnExit.GetID(), KEY_VIRTUAL_ENTER,	OnExit)
#ifdef __SUPPORT_SHOW_QRCODE__
		ON_MSG(m_btnqrCode.GetID(), KEY_VIRTUAL_ENTER,	OnShowQrCode)
#endif

#ifdef __SUPPORT_APP_INFO_QRCODE__
	ON_MSG(m_btnAppInfo.GetID(),KEY_VIRTUAL_ENTER,OnShowAppInfoQrCode)
#endif

END_MSG_MAP()

//////////////////////////////////////////////////////////////////////////
unsigned long CInfoManDlg::OnSystem()
{
	//m_systemDlg.DoModal(this, CENTER_POS, CENTER_POS, m_length[RECT_WIDTH], m_length[RECT_HEIGHT]);
	m_systemDlg.DoModal(this);
	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CInfoManDlg::OnEvent()
{
	if (0 == CUserMan::Instance()->CheckAuthority(AUTH_LOG_VIEW))
	{
		MessageBox(m_pStrTable->String(E_STR_ID_MESSAGE_79), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK | DEFAULT_ICON_WARNING);
	}
	else
	{
		m_eventDlg.DoModal(this);	
	}

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CInfoManDlg::OnLog()
{
	if (0 == CUserMan::Instance()->CheckAuthority(AUTH_LOG_VIEW))
	{
		MessageBox(m_pStrTable->String(E_STR_ID_MESSAGE_79), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK | DEFAULT_ICON_WARNING);
	}
	else
	{
		m_logDlg.DoModal(this);
	}
	
	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CInfoManDlg::OnNetwork()
{
	//m_networkDlg.DoModal(this, CENTER_POS, CENTER_POS, m_length[RECT_WIDTH], m_length[RECT_HEIGHT]);
	m_networkDlg.DoModal(this);
	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CInfoManDlg::OnOnlineUser()
{
	//m_onlineUserDlg.DoModal(this, CENTER_POS, CENTER_POS, m_length[RECT_WIDTH], m_length[RECT_HEIGHT]);
	m_onlineUserDlg.DoModal(this);
	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CInfoManDlg::OnDevice()
{
	m_infoDeviceDlg.DoModal(this);

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CInfoManDlg::OnExit()
{
	EndDialog(DLG_ESC);

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CInfoManDlg::OnShowQrCode()
{
#ifdef __SUPPORT_SHOW_QRCODE__

	unsigned long len =0;
	char user[36] = "admin";
	char passWord[36]={0};//颠倒顺序
	CQRCode codeMan;
	QrCodeOutputData qrMaix;
	qrMaix.dataMatrix =NULL;
	qrMaix.width = 0;

	//读取系统信息并组装字符串
	NETWORK_STATUS_INFO networkStatusInfo;
	memset( &networkStatusInfo, 0, sizeof(NETWORK_STATUS_INFO));
#ifdef __ENVIRONMENT_LINUX__
	MESSAGE_DATA msgData;
	CMessageMan::Instance()->SendMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_NETWORK_INFO, NULL, 0, msgData);

	networkStatusInfo = *reinterpret_cast<NETWORK_STATUS_INFO *>(msgData.pData+sizeof(PACKCMD));

	CMessageMan::Instance()->ReleaseMsgBuff(msgData);
#endif

	MESSAGE_DATA msgDataNat;
	CMessageMan::Instance()->SendMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_NAT_INFO, NULL, 0, msgDataNat);
	NAT_SERVER_INFO natServerInfo = * reinterpret_cast<NAT_SERVER_INFO *>(msgDataNat.pData+sizeof(PACKCMD));
	CMessageMan::Instance()->ReleaseMsgBuff(msgDataNat);
	
	char str[128]={0};
	
	#if defined (__CUSTOM_ZNV__) && (!defined (__ZNV_SC__) || defined(__ZNV_WNRL__))
		#ifdef	__ENVIRONMENT_LINUX__
			CLocalDevice::Instance()->GetSNFromFlash(str);
			m_printQRCodeDlg.SetDesc(str);
		#endif
	#else
		snprintf(str,128,"<sn>%02X%02X%02X%02X%02X%02X</sn>\n",networkStatusInfo.MAC[0],networkStatusInfo.MAC[1],networkStatusInfo.MAC[2],networkStatusInfo.MAC[3],networkStatusInfo.MAC[4],networkStatusInfo.MAC[5]);
		snprintf(str+strlen(str),128-strlen(str), "<user>%s</user>\n",user);
		snprintf(str+strlen(str),128-strlen(str), "<ip>%03d.%03d.%03d.%03d</ip>\n",(networkStatusInfo.IP&0xff), (networkStatusInfo.IP>>8)&0xff, (networkStatusInfo.IP>>16)&0xff, (networkStatusInfo.IP>>24)&0xff);
		snprintf(str+strlen(str),128-strlen(str), "<port>%d</port>\n",networkStatusInfo.httpPort);
		snprintf(str+strlen(str),128-strlen(str), "<ver>%d</ver>\n",verNum);

		char sTitle[128] = {0};
		snprintf(sTitle,128,"%02X%02X%02X%02X%02X%02X",networkStatusInfo.MAC[0],networkStatusInfo.MAC[1],networkStatusInfo.MAC[2],networkStatusInfo.MAC[3],networkStatusInfo.MAC[4],networkStatusInfo.MAC[5]);
		m_printQRCodeDlg.SetDesc(sTitle);
		m_printQRCodeDlg.SetDescUrl(natServerInfo.szServerAddr);
		
	#endif
	codeMan.GetQRCodeMatrixData((unsigned char *)str,strlen(str),&qrMaix);

	m_printQRCodeDlg.SetMaix((unsigned char *)qrMaix.dataMatrix,qrMaix.width,qrMaix.width);
	codeMan.FreeQrCodeMatrix(&qrMaix);
	m_printQRCodeDlg.DoModal(this);
#endif
	return KEY_VIRTUAL_MSG_STOP;
}
unsigned long CInfoManDlg::OnShowAppInfoQrCode()
{
	#ifdef __SUPPORT_APP_INFO_QRCODE__
	#if 0
	MESSAGE_DATA msgData;
	CMessageMan::Instance()->SendMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_TOKEN_SERVER_INFO, NULL, 0, msgData);

	
	PACKCMD *packCmd = reinterpret_cast<PACKCMD *>(msgData.pData);
	TOKEN_SERVER_INFO tokenserverinfo;
	if(packCmd->cmdType == CMD_REPLY_TOKEN_SERVER_INFO_SUCC)

	{
		tokenserverinfo = *reinterpret_cast<TOKEN_SERVER_INFO *> (msgData.pData+sizeof(PACKCMD));
	}
	//m_appInfoQrCodeDlg->SetAppInfoQrCodeStr(VectorAppInfo);
	CMessageMan::Instance()->ReleaseMsgBuff(msgData);
	//m_appInfoQrCodeDlg->DoModal(this);
	#else
	MESSAGE_DATA msgData;
	CMessageMan::Instance()->SendMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_APP_DOWNLOAD_LINK_INFO, NULL, 0, msgData);
		
	VECTOR_APP_INFO VectorAppInfo;
	VectorAppInfo.clear();
	PACKCMD *packCmd = reinterpret_cast<PACKCMD *>(msgData.pData);
	if(packCmd->cmdType == CMD_REPLY_APP_DOWNLOAD_LINK_INFO_SUCC)
	{
		int size = packCmd->dataLen/sizeof(APP_DOWNLOAD_INFO);

		APP_DOWNLOAD_INFO *pDownInfo = reinterpret_cast<APP_DOWNLOAD_INFO *>(msgData.pData+sizeof(PACKCMD));
		RESP_APP_INFO restAppInfo;
		for(int i=0;i<size;i++)
		{
			restAppInfo.osPlateform.clear();
			restAppInfo.appPlateForm.clear();
			restAppInfo.AppName.clear();
			restAppInfo.QrDownUrl.clear();
			restAppInfo.osPlateform += pDownInfo->osPlateform;
			restAppInfo.appPlateForm+= pDownInfo->appPlateForm;
			restAppInfo.AppName+= pDownInfo->AppName;
			restAppInfo.QrDownUrl += pDownInfo->QrDownUrl;
			
			VectorAppInfo.push_back(restAppInfo);
			pDownInfo++;
		}
	}
	CAppInfoServeManage lappInfoServerMange;	
	m_appInfoQrCodeDlg->SetAppInfoQrCodeStr(VectorAppInfo);
	CMessageMan::Instance()->ReleaseMsgBuff(msgData);
	m_appInfoQrCodeDlg->DoModal(this);
	#endif
	#endif
	return KEY_VIRTUAL_MSG_STOP;
	
}

//end

