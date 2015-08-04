/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : ysw
** Date         : 2009-08-08
** Name         : VersionInfoDlg.cpp
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/
#include "VersionInfoDlg.h"
#include "NetProtocol.h"
#include "TimeCtrl.h"
#include "ConfigEx.h"

extern NET_LOGIN_SUCCESS_REPLY *g_p_login_succ;
//////////////////////////////////////////////////////////////////////////
CVersionInfoDlg::CVersionInfoDlg()
{

}

CVersionInfoDlg::~CVersionInfoDlg()
{

}

//////////////////////////////////////////////////////////////////////////
void CVersionInfoDlg::OnInitial()
{
	GUI::CDialog::OnInitial();
	SetCaption(m_pStrTable->String(E_STR_ID_TITLE_SYSTEM), false);
	//////////////////////////////////////////////////////////////////////////
	unsigned short x = m_length[DLG_OFFSET_SUB_LEFT], y = m_length[DLG_OFFSET_SUB_TOP];
	unsigned short width = m_rect.Width() -  2 * m_length[DLG_OFFSET_SUB_LEFT];
	unsigned short height = m_rect.Height() - y - m_length[DLG_OFFSET_SUB_BOTTOM];

	m_versionInfoList.Create(GUI::CWnd::GetRandWndID(), x, y, width, height, this, 0, 0, false);
	m_versionInfoList.AddColumn(NULL, width/2);
	m_versionInfoList.AddColumn(NULL, width/2);

	////
	char str[512] = {0};
	
	m_versionInfoList.AddItem(COL_DEVICE_NAME, 0, m_pStrTable->String(E_STR_ID_DEV_NAME));
	strcpy(str, g_p_login_succ->productInfo.deviceName);
	m_versionInfoList.AddItem(COL_DEVICE_NAME, 1, str);

#if defined(__CUSTOM_CNB__)
	m_versionInfoList.AddItem(COL_DEVICE_TYPE, 0, m_pStrTable->String(E_STR_ID_PRODUCT_MODEL));
	std::string productName;
	CProduct::Instance()->GetProductIDName(productName);
	sprintf(str, "%s", productName.c_str());
	m_versionInfoList.AddItem(COL_DEVICE_TYPE, 1, str);
#endif
	m_versionInfoList.AddItem(COL_DEVICE_ID, 0, m_pStrTable->String(E_STR_ID_DEV_ID));
	sprintf(str, "%d", g_p_login_succ->productInfo.deviceID);
	m_versionInfoList.AddItem(COL_DEVICE_ID, 1, str);

	m_versionInfoList.AddItem(COL_HARDWARE_VERSION, 0, m_pStrTable->String(E_STR_ID_HARDWARE_VERSION));
	strcpy(str, g_p_login_succ->productInfo.hardwareVersion);
	m_versionInfoList.AddItem(COL_HARDWARE_VERSION, 1, str);

	m_versionInfoList.AddItem(COL_MCU_VERSION, 0, m_pStrTable->String(E_STR_ID_MCU_VERSION));
	strcpy(str, g_p_login_succ->productInfo.MCUVersion);
	m_versionInfoList.AddItem(COL_MCU_VERSION, 1, str);

	m_versionInfoList.AddItem(COL_KERNEL_VERSION, 0, m_pStrTable->String(E_STR_ID_KERNEL_VERSION));
	strcpy(str, g_p_login_succ->productInfo.kernelVersion);
	m_versionInfoList.AddItem(COL_KERNEL_VERSION, 1, str);

	m_versionInfoList.AddItem(COL_FIRMWARE_VERSION, 0, m_pStrTable->String(E_STR_ID_FIRMWARE_VERSION));
	strcpy(str, g_p_login_succ->productInfo.firmwareVersion);
	m_versionInfoList.AddItem(COL_FIRMWARE_VERSION, 1, str);

	m_versionInfoList.AddItem(COL_LAUNCH_DATE, 0, m_pStrTable->String(E_STR_ID_LAUNCH_DATE));

	char timetext [64] = {0};
	sprintf(timetext, "%d-%02d-%02d %02d:%02d:%02d", g_p_login_succ->productInfo.buildDate>>16, (0x000ff & g_p_login_succ->productInfo.buildDate>>8), \
		(0x000ff & g_p_login_succ->productInfo.buildDate), g_p_login_succ->productInfo.buildTime>>16, (0x000ff & g_p_login_succ->productInfo.buildTime>>8), \
		(0x000ff & g_p_login_succ->productInfo.buildTime));
	m_versionInfoList.AddItem(COL_LAUNCH_DATE, 1, timetext);

	m_versionInfoList.AddItem(COL_VIDEO_FORMAT, 0, m_pStrTable->String(E_STR_ID_VIDEO_FORMAT));
	NCFG_ITEM_HEAD itemHead;
	memset(&itemHead, 0, sizeof(NCFG_ITEM_HEAD));
	itemHead.itemID = NCFG_ITEM_VIDEO_FORMAT;
	unsigned long dataLen = 0;
	unsigned char *pData = NULL;

	if(CConfigEx::Instance()->GetConfig((unsigned char *)&itemHead, sizeof(itemHead), &pData, dataLen))
	{
		assert(pData != NULL);
		assert(dataLen > 0);

		unsigned long *pVideoFormat = reinterpret_cast<unsigned long *>( pData + sizeof(NCFG_ITEM_HEAD) + sizeof(NCFG_BLOCK_HEAD));
		if (*pVideoFormat == 0)
		{
			m_versionInfoList.AddItem(COL_VIDEO_FORMAT, 1, m_pStrTable->String(E_STR_ID_NTSC));
		}
		else
		{
			m_versionInfoList.AddItem(COL_VIDEO_FORMAT, 1, m_pStrTable->String(E_STR_ID_PAL));
		}
	}

	
}
