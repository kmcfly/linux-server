/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : L
** Date         : 2010-12-03
** Name         : BackDoorManDlg.h
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/

#include "BackDoorManDlg.h"
#include "MessageMan.h"
#include "LanguagePack.h"

using namespace GUI;

#define BACKDOORMAN_LOGO_MOUNT_PATH  "/mnt/bdmlogo"
#define BACKDOORMAN_NO_USB           "USB device is not plugged, or no valid file."
#define BACKDOORMAN_SAVE_OK          "Successfully saved"
#define BACKDOORMAN_SAVE_FAIL        "Save failed"
#define BACKDOORMAN_ERROR            "Error"
#define BACKDOORMAN_SAVE             "Save"


#define BACKDOORMAN_WEB_LOGO_DIR_PATH "/mnt/mtd/WebSites/logo/logo.png"
#define BACKDOORMAN_DVR_LOGO_DIR_PATH "/mnt/mtd/ui/icon_logo.bmp"
#define BACKDOORMAN_DVR_LOGO_TRA_PATH "/mnt/mtd/ui/icon_logo_trans.bmp"
#define BACKDOORMAN_WEB_LOGO_NAME	  "ielogo.png"
#define BACKDOORMAN_LOGO_NAME		  "icon_logo.bmp"
#define BACKDOORMAN_LOGO_TRANS_NAME	  "icon_logo_trans.bmp"

#define BACKDOORMAN_DDNS              "Select DDNS type"
#define BACKDOORMAN_DDNS_DEFAULT      "Default"
#define BACKDOORMAN_DDNS_US02         "DDNS for US02"
#define BACKDOORMAN_DDNS_US09SUB1     "DDNS for US09_01"
#define BACKDOORMAN_DDNS_US09SUB2     "DDNS for US09_02"
#define BACKDOORMAN_DDNS_US61         "DDNS for US61"
#define BACKDOORMAN_DDNS_BR27         "DDNS for BR27"
#define BACKDOORMAN_DDNS_TW01         "DDNS for TWO1"
#define BACKDOORMAN_DDNS_TH13         "DDNS for TH13 www.hiview.org"
#define BACKDOORMAN_DDNS_MX15         "DDNS for MX15 mymeriva.com"
#define BACKDOORMAN_DDNS_VN11         "DDNS for VN11 questek.tv"

#define BACKDOORMAN_KEYBOAD_CHANGE           "Select Keyboard"
#define BACKDOORMAN_KEYBOAD_Q1_1UA           "1UA/15UB/1UZ(W/K/J/Y/X)-Q1"
#define BACKDOORMAN_KEYBOAD_Q2_1UB           "1UB-Q2"
#define BACKDOORMAN_KEYBOAD_Q3_1UC           "1UC-Q3"
#define BACKDOORMAN_KEYBOAD_Q4_1UD           "1UD-Q4"
#define BACKDOORMAN_KEYBOAD_Q5_1UE           "1UE-Q5"
#define BACKDOORMAN_KEYBOAD_Q6_1UF           "1UF-Q6"
#define BACKDOORMAN_KEYBOAD_Q7_1UG           "1UG-Q7"
#define BACKDOORMAN_KEYBOAD_Q8_1US_OLD       "1US_OLD-Q8"
#define BACKDOORMAN_KEYBOAD_Q9_1UT           "1UT-Q9"
#define BACKDOORMAN_KEYBOAD_Q10_SUA          "SUA-Q10"
#define BACKDOORMAN_KEYBOAD_Q11_SUB          "SUB-Q11"
#define BACKDOORMAN_KEYBOAD_Q12_SUC          "SUC-Q12"
#define BACKDOORMAN_KEYBOAD_Q13_1UTW         "1UTW-Q13"
#define BACKDOORMAN_KEYBOAD_Q14_2UA          "2UA-Q14"
#define BACKDOORMAN_KEYBOAD_Q15_2UB          "2UB-Q15"
#define BACKDOORMAN_KEYBOAD_Q16_2UC          "2UC-Q16"
#define BACKDOORMAN_KEYBOAD_Q17_2UD          "2UD(2UE,2UF,2UG)-Q17"
#define BACKDOORMAN_KEYBOAD_Q18_1US_NEW      "1US_NEW-Q18"
#define BACKDOORMAN_KEYBOAD_Q19_1_5U         "1.5UA-Q19"
#define BACKDOORMAN_KEYBOAD_Q20_1UV          "1UV--Q20"
#define BACKDOORMAN_KEYBOAD_Q21_1UH          "1UH--Q21"
#define BACKDOORMAN_KEYBOAD_Q22_1UI          "1UI--Q22"
#define BACKDOORMAN_KEYBOAD_Q23_ONE_BTN      "1UA--Q23--guangmianban"		

#define BACKDOORMAN_MAC                       "Modify Mac"
#define BACKDOORMAN_HARDWARE                  "Hardware Version"

#define BACKDOORMAN_CRYTOINFO       "select language and company"
#define BACKDOORMAN__SELECT_LAN     "select language"
#define BACKDOORMAN_SELECT_CMP      "select company"
#define BACKDOORMAN_COMPANY_NAME	"company name"
const char BACKDOORMAN_COMPANY_IN[] =  {0xE5,0x9B,0xBD,0xE9,0x99,0x85,0xE6,0xA0,0x87,0xE5,0x87,0x86,0xE7,0x89,0x88,0x0};
const char BACKDOORMAN_COMPANY_CN[] =  {0xE5,0x9B,0xBD,0xE5,0x86,0x85,0xE7,0x89,0x88,0xE6,0x9C,0xAC,0x0};
const char BACKDOORMAN_COMPANY_SP[] =  {0xE7,0x89,0xB9,0xE6,0xAE,0x8A,0xE7,0x89,0x88,0x0};
#define BACKDOORMAN_COMPANY_US02   "US02"
#define BACKDOORMAN_COMPANY_RU04   "RU04"
#define BACKDOORMAN_COMPANY_BR27   "BR27"
#define BACKDOORMAN_COMPANY_TW01   "TW01"
#define BACKDOORMAN_COMPANY_GB14   "GB14"
#define BACKDOORMAN_COMPANY_IN23   "IN23"
#define BACKDOORMAN_COMPANY_US47   "US47"
#define BACKDOORMAN_COMPANY_IN25   "IN25-ADI"
#define BACKDOORMAN_COMPANY_PL14   "PL14"
#define BACKDOORMAN_COMPANY_IL03   "IL03"
#define BACKDOORMAN_COMPANY_LV01   "LV01"
#define BACKDOORMAN_COMPANY_CBC    "CBC"

#define BACKDOORMAN_COMPANY_GB31	"GB31"
#define BACKDOORMAN_COMPANY_ZNV		"ZNV"

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////

CBackDoorLogoImgDlg::CBackDoorLogoImgDlg()
{
	m_pBackDoorInfo = NULL;
}

CBackDoorLogoImgDlg::~CBackDoorLogoImgDlg()
{


}

void CBackDoorLogoImgDlg::OnInitial()
{
	GUI::CDialog::OnInitial();
	////////////////////////////////////
	memset(&m_posBtn, 0xFF, sizeof(m_posBtn));

	unsigned short x = m_length[DLG_OFFSET_SUB_LEFT], y = m_length[DLG_OFFSET_SUB_TOP];
	unsigned short width = m_rect.Width() - 2 * m_length[DLG_OFFSET_SUB_LEFT];
	unsigned short height = m_rect.Height() - y - m_length[DLG_OFFSET_SUB_BOTTOM];

	GUI::CButton* pBtn = NULL;
	GUI::CListCtrl& list = m_listCtrl;
	unsigned short offsetEdge = 4;

	list.Create(GUI::CWnd::GetRandWndID(), x, y, width, height, this, 0, 0, false);
	list.AddColumn(NULL, width/3);
	list.AddColumn(NULL, width/3);
	list.AddColumn(NULL, width/3 - offsetEdge);

	int row = 0;

	//开机画面
	if(0 != m_pBackDoorInfo->BootImage)
	{
		list.AddItem(row, 0, "Change boot image");

		list.AddItem(row, 1, "logo.jpg");

		list.AddItem(row, 2, WND_TYPE_BUTTON, m_length[BTN_WIDTH]);
		pBtn = dynamic_cast<GUI::CButton*>(list.GetItemWnd(row, 2));
		pBtn->SetCaption(BACKDOORMAN_SAVE, false);
		
		m_posBtn[BTN_BOOT_IMG].row = row;
		m_posBtn[BTN_BOOT_IMG].col = 2;
		
		row += 2;
	}

	//Logo
	if(0 != m_pBackDoorInfo->Logo)
	{
		list.AddItem(row, 0, "Change Device logo");
		row++;

		list.AddItem(row, 0, "logo display in IE browser");
		list.AddItem(row, 1, BACKDOORMAN_WEB_LOGO_NAME);
		list.AddItem(row, 2, WND_TYPE_BUTTON, m_length[BTN_WIDTH]);
		pBtn = dynamic_cast<GUI::CButton*>(list.GetItemWnd(row, 2));
		pBtn->SetCaption(BACKDOORMAN_SAVE, false);
		m_posBtn[BTN_IELOGO].row = row;
		m_posBtn[BTN_IELOGO].col = 2;
		row++;

		list.AddItem(row, 0, "logo display in DVR");
		list.AddItem(row, 1, BACKDOORMAN_LOGO_NAME);
		list.AddItem(row, 2, WND_TYPE_BUTTON, m_length[BTN_WIDTH]);
		pBtn = dynamic_cast<GUI::CButton*>(list.GetItemWnd(row, 2));
		pBtn->SetCaption(BACKDOORMAN_SAVE, false);
		m_posBtn[BTN_DVRLOGO].row = row;
		m_posBtn[BTN_DVRLOGO].col = 2;
		row++;

		list.AddItem(row, 0, "logo display in DVR");
		list.AddItem(row, 1, BACKDOORMAN_LOGO_TRANS_NAME);
		list.AddItem(row, 2, WND_TYPE_BUTTON, m_length[BTN_WIDTH]);
		pBtn = dynamic_cast<GUI::CButton*>(list.GetItemWnd(row, 2));
		pBtn->SetCaption(BACKDOORMAN_SAVE, false);
		m_posBtn[BTN_DVRLOGOTRANS].row = row;
		m_posBtn[BTN_DVRLOGOTRANS].col = 2;
	}

	list.CreateListFocusMap();
}

bool CBackDoorLogoImgDlg::HasBackDoorInfo(const WND_BACK_DOOR_INFO& info)
{
	if( (0 != info.BootImage) ||
		(0 != info.Logo))
	{
		return true;
	}

	return false;
}

BEGIN_MSG_MAP(CBackDoorLogoImgDlg, GUI::CDialog)
	ON_MSG_EX(m_listCtrl.GetID(), KEY_VIRTUAL_MSG_DEFAULT_TAG, OnClickList)
END_MSG_MAP()

unsigned long CBackDoorLogoImgDlg::OnClickList(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	unsigned short row = (keyID >> 16)&0x7fff;	//行
	unsigned short col = keyID&0xffff;			//列

	MESSAGE_DATA msgData;
	PACKCMD *pCmd = NULL;
	unsigned long cmdType;

	if((m_posBtn[BTN_BOOT_IMG].row == row) && (m_posBtn[BTN_BOOT_IMG].col == col))
	{
		//修改开机画面
		char szBootImgName[256] = {0};
		if (CheckBootImageFile(const_cast<char *>("logo.jpg"), szBootImgName, 256))
		{
			memset(&msgData, 0, sizeof(MESSAGE_DATA));
			CMessageMan::Instance()->SendMsgDataToServer(0, LOCAL_DEVICE_ID, CMD_REQUEST_MODIFY_BOOT_IMAGE, reinterpret_cast<unsigned char *>(&szBootImgName[0]), sizeof(szBootImgName), msgData);
			pCmd = reinterpret_cast<PACKCMD *>(msgData.pData);
			cmdType = (NET_PROTOCOL)pCmd->cmdType;
			if (CMD_REPLY_CTRL_SUCC == cmdType)
			{
				//保存成功
				UmountUsb();
				MessageBox(BACKDOORMAN_SAVE_OK, m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK);										
			}
			else
			{
				//保存失败		
				UmountUsb();
				MessageBox(BACKDOORMAN_SAVE_FAIL, m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK);									
			}

		}
		else
		{
			MessageBox(BACKDOORMAN_NO_USB, m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK);
		}

		return KEY_VIRTUAL_MSG_STOP;
	}

	if((m_posBtn[BTN_IELOGO].row == row) && (m_posBtn[BTN_IELOGO].col == col))
	{
		char szIELogoName[256] = {0};
		if (CheckBootImageFile(const_cast<char *>(BACKDOORMAN_WEB_LOGO_NAME), szIELogoName, 256))
		{
			char strCmd[256] = {0};
			snprintf(strCmd, 256, "cp %s %s", szIELogoName, BACKDOORMAN_WEB_LOGO_DIR_PATH);
			DVRSystem(strCmd);
			UmountUsb();
			MessageBox(BACKDOORMAN_SAVE_OK, m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK);
		}
		else
		{
			MessageBox(BACKDOORMAN_NO_USB, m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK);
		}
		
		return KEY_VIRTUAL_MSG_STOP;
	}

	if((m_posBtn[BTN_DVRLOGO].row == row) && (m_posBtn[BTN_DVRLOGO].col == col))
	{
		char szDVRLogoName[256] = {0};
		if (CheckBootImageFile(const_cast<char *>(BACKDOORMAN_LOGO_NAME), szDVRLogoName, 256))
		{
			char strCmd[256] = {0};
			snprintf(strCmd, 256, "cp %s %s", szDVRLogoName, BACKDOORMAN_DVR_LOGO_DIR_PATH);
			DVRSystem(strCmd);
			UmountUsb();
			MessageBox(BACKDOORMAN_SAVE_OK, m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK);
		}
		else
		{
			MessageBox(BACKDOORMAN_NO_USB, m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK);
		}

		return KEY_VIRTUAL_MSG_STOP;
	}

	if((m_posBtn[BTN_DVRLOGOTRANS].row == row) && (m_posBtn[BTN_DVRLOGOTRANS].col == col))
	{
		char szDVRTransLogoName[256] = {0};
		if (CheckBootImageFile(const_cast<char *>(BACKDOORMAN_LOGO_TRANS_NAME), szDVRTransLogoName, 256))
		{
			char strCmd[256] = {0};
			snprintf(strCmd, 256, "cp %s %s", szDVRTransLogoName, BACKDOORMAN_DVR_LOGO_TRA_PATH);
			DVRSystem(strCmd);
			UmountUsb();
			MessageBox(BACKDOORMAN_SAVE_OK, m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK);
		}
		else
		{
			MessageBox(BACKDOORMAN_NO_USB, m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK);
		}


		return KEY_VIRTUAL_MSG_STOP;
	}

	return KEY_VIRTUAL_MSG_STOP;
}


bool CBackDoorLogoImgDlg::CheckBootImageFile(char *pFileName, char *pRetFilePathName, int len)
{
#ifndef __ENVIRONMENT_WIN32__
	MESSAGE_DATA msgData;
	PACKCMD *pCmd = NULL;
	unsigned long cmdType;
	DVR_DISK_INFO *pPartionInfo = NULL;
	int partionNum = 0;
	char szCmd[256] = {0};
	CMyList<DVR_DISK_INFO> deviceInfoList;

	//
	assert(pFileName != NULL);
	if (0 == strlen(pFileName))
	{
		return false;
	}

	if (len < 256)
	{
		return false;
	}

	//
	memset(&msgData, 0, sizeof(MESSAGE_DATA));
	CMessageMan::Instance()->SendMsgDataToServer(0, LOCAL_DEVICE_ID, CMD_REQUEST_DISK_GET_REMOVABLE_PARTION, NULL, 0, msgData);
	pPartionInfo = (DVR_DISK_INFO *)(msgData.pData + sizeof(PACKCMD));
	partionNum = (msgData.dataLen - sizeof(PACKCMD))/sizeof(DVR_DISK_INFO);
	if (partionNum > 0)
	{
		for (int i = 0; i < partionNum; ++i)
		{	
			DVR_DISK_INFO &tmp = pPartionInfo[i];
			deviceInfoList.AddTail(tmp);
		}
	}
	CMessageMan::Instance()->ReleaseMsgBuff(msgData);

	//
	if (0 == deviceInfoList.GetCount())
	{
		return false;
	}

	//
	bool bOk = false;
	POS pos = deviceInfoList.GetHeadPosition();
	while (NULL != pos)
	{
		DVR_DISK_INFO &diskInfo = deviceInfoList.GetAt(pos);
		snprintf(szCmd, 256, "mkdir -p %s", BACKDOORMAN_LOGO_MOUNT_PATH);
		DVRSystem(szCmd);
		snprintf(szCmd, 256, "umount %s", BACKDOORMAN_LOGO_MOUNT_PATH);
		DVRSystem(szCmd);		
		snprintf(szCmd, 256, "mount -t vfat %s %s", diskInfo.diskname, BACKDOORMAN_LOGO_MOUNT_PATH);
		if (0 != DVRSystem(szCmd))
		{
			deviceInfoList.GetNext(pos);
			snprintf(szCmd, 256, "rm -rf %s", BACKDOORMAN_LOGO_MOUNT_PATH);
			DVRSystem(szCmd);
			continue;
		}

		if ('/' == pFileName[0])
		{
			snprintf(szCmd, 256, "%s%s", BACKDOORMAN_LOGO_MOUNT_PATH, pFileName);
		}
		else
		{
			snprintf(szCmd, 256, "%s/%s", BACKDOORMAN_LOGO_MOUNT_PATH,pFileName);
		}

		if (0 != access(szCmd, R_OK))
		{
			snprintf(szCmd, 256, "umount %s", BACKDOORMAN_LOGO_MOUNT_PATH);
			DVRSystem(szCmd);
			snprintf(szCmd, 256, "rm -rf %s", BACKDOORMAN_LOGO_MOUNT_PATH);
			DVRSystem(szCmd);
			deviceInfoList.GetNext(pos);
			continue;
		}

		bOk = true;
		break;
	}
	deviceInfoList.RemoveAll();

	if (bOk)
	{
		snprintf(pRetFilePathName, len, "%s", szCmd);
	}

	return bOk;
#else
	return false;
#endif
}



void CBackDoorLogoImgDlg::UmountUsb()
{
	char szCmd[256] = {0};
	snprintf(szCmd, 256, "umount %s", BACKDOORMAN_LOGO_MOUNT_PATH);
	if (0 == DVRSystem(szCmd))
	{
		snprintf(szCmd, 256, "rm -rf %s", BACKDOORMAN_LOGO_MOUNT_PATH);
		DVRSystem(szCmd);
	}
}

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////

CBackDoorDdnsDlg::CBackDoorDdnsDlg()
{

}

CBackDoorDdnsDlg::~CBackDoorDdnsDlg()
{

}

void CBackDoorDdnsDlg::OnInitial()
{
	GUI::CDialog::OnInitial();
	////////////////////////////////////
	memset(&m_posBtn, 0xFF, sizeof(m_posBtn));

	unsigned short x = m_length[DLG_OFFSET_SUB_LEFT], y = m_length[DLG_OFFSET_SUB_TOP];
	unsigned short width = m_rect.Width() - 2 * m_length[DLG_OFFSET_SUB_LEFT];
	unsigned short height = m_rect.Height() - y - m_length[DLG_OFFSET_SUB_BOTTOM];

	GUI::CButton    *pBtn = NULL;
	GUI::CComboCtrl *pCombo = NULL;
	GUI::CListCtrl& list = m_listCtrl;
	unsigned short offsetEdge = 4;
	char szName[48] = {0};

	list.Create(GUI::CWnd::GetRandWndID(), x, y, width, height, this, 0, 0, false);
	list.AddColumn(NULL, width/3);
	list.AddColumn(NULL, width/3);
	list.AddColumn(NULL, width/3 - offsetEdge);

	int row = 0;

	//DDNS
	if(0 != m_pBackDoorInfo->DDNS)
	{
		list.AddItem(row, 0, BACKDOORMAN_DDNS);

		list.AddItem(row, 1, WND_TYPE_COMBO);		
		pCombo = dynamic_cast<GUI::CComboCtrl*>(list.GetItemWnd(row, 1));
		snprintf(szName, sizeof(szName), "%s", BACKDOORMAN_DDNS_DEFAULT);
		pCombo->AddItem(szName, DDNS_COMPANY_DEFAULT);
		snprintf(szName, sizeof(szName), "%s", BACKDOORMAN_DDNS_US02);
		pCombo->AddItem(szName, DDNS_COMPANY_US02);
		snprintf(szName, sizeof(szName), "%s", BACKDOORMAN_DDNS_US09SUB1);
		pCombo->AddItem(szName, DDNS_COMPANY_US09_SUB1);
		snprintf(szName, sizeof(szName), "%s", BACKDOORMAN_DDNS_US09SUB2);
		pCombo->AddItem(szName, DDNS_COMPANY_US09_SUB2);
		snprintf(szName, sizeof(szName), "%s", BACKDOORMAN_DDNS_BR27);
		pCombo->AddItem(szName, DDNS_COMPANY_BR27);
		snprintf(szName, sizeof(szName), "%s", BACKDOORMAN_DDNS_US61);
		pCombo->AddItem(szName, DDNS_COMPANY_US61);
		snprintf(szName, sizeof(szName), "%s", BACKDOORMAN_DDNS_TW01);
		pCombo->AddItem(szName, DDNS_COMPANY_TW01);	
		snprintf(szName, sizeof(szName), "%s", BACKDOORMAN_DDNS_TH13);
		pCombo->AddItem(szName, DDNS_COMPANY_TH13);	

		snprintf(szName, sizeof(szName), "%s", BACKDOORMAN_DDNS_MX15);
		pCombo->AddItem(szName, DDNS_COMPANY_MX15);	

		snprintf(szName, sizeof(szName), "%s", BACKDOORMAN_DDNS_VN11);
		pCombo->AddItem(szName, DDNS_COMPANY_VN11);	

		pCombo->SetCurItem(0, false);

		list.AddItem(row, 2, WND_TYPE_BUTTON, m_length[BTN_WIDTH]);
		pBtn = dynamic_cast<GUI::CButton*>(list.GetItemWnd(row, 2));
		pBtn->SetCaption(BACKDOORMAN_SAVE, false);
		m_posBtn[BTN_DDNS].row = row;
		m_posBtn[BTN_DDNS].col = 2;

		row += 2;

	}

	//Keyboard
	if(0 != m_pBackDoorInfo->Keyboard)
	{
		list.AddItem(row, 0, BACKDOORMAN_KEYBOAD_CHANGE);
		
		list.AddItem(row, 1, WND_TYPE_COMBO);		
		pCombo = dynamic_cast<GUI::CComboCtrl*>(list.GetItemWnd(row, 1));
		snprintf(szName, sizeof(szName), "%s", BACKDOORMAN_KEYBOAD_Q1_1UA);
		pCombo->AddItem(szName, KEYBOARD_1UA);
		snprintf(szName, sizeof(szName), "%s", BACKDOORMAN_KEYBOAD_Q2_1UB);
		pCombo->AddItem(szName, KEYBOARD_1UB);
		snprintf(szName, sizeof(szName), "%s", BACKDOORMAN_KEYBOAD_Q3_1UC);
		pCombo->AddItem(szName, KEYBOARD_1UC);
		snprintf(szName, sizeof(szName), "%s", BACKDOORMAN_KEYBOAD_Q4_1UD);
		pCombo->AddItem(szName, KEYBOARD_1UD);	
		snprintf(szName, sizeof(szName), "%s", BACKDOORMAN_KEYBOAD_Q5_1UE);
		pCombo->AddItem(szName, KEYBOARD_1UE);	
		snprintf(szName, sizeof(szName), "%s", BACKDOORMAN_KEYBOAD_Q6_1UF);
		pCombo->AddItem(szName, KEYBOARD_1UF);
		snprintf(szName, sizeof(szName), "%s", BACKDOORMAN_KEYBOAD_Q7_1UG);
		pCombo->AddItem(szName, KEYBOARD_1UG);
		snprintf(szName, sizeof(szName), "%s", BACKDOORMAN_KEYBOAD_Q8_1US_OLD);
		pCombo->AddItem(szName, KEYBOARD_1US);
		snprintf(szName, sizeof(szName), "%s", BACKDOORMAN_KEYBOAD_Q9_1UT);
		pCombo->AddItem(szName, KEYBOARD_1UT);	
		snprintf(szName, sizeof(szName), "%s", BACKDOORMAN_KEYBOAD_Q10_SUA);
		pCombo->AddItem(szName, KEYBOARD_SUA);	
		snprintf(szName, sizeof(szName), "%s", BACKDOORMAN_KEYBOAD_Q11_SUB);
		pCombo->AddItem(szName, KEYBOARD_SUB);
		snprintf(szName, sizeof(szName), "%s", BACKDOORMAN_KEYBOAD_Q12_SUC);
		pCombo->AddItem(szName, KEYBOARD_SUC);
		snprintf(szName, sizeof(szName), "%s", BACKDOORMAN_KEYBOAD_Q13_1UTW);
		pCombo->AddItem(szName, KEYBOARD_1UTW);
		snprintf(szName, sizeof(szName), "%s", BACKDOORMAN_KEYBOAD_Q18_1US_NEW);
		pCombo->AddItem(szName, KEYBOARD_1USS);	
		snprintf(szName, sizeof(szName), "%s", BACKDOORMAN_KEYBOAD_Q14_2UA);
		pCombo->AddItem(szName, KEYBOARD_2UA);	
		snprintf(szName, sizeof(szName), "%s", BACKDOORMAN_KEYBOAD_Q15_2UB);
		pCombo->AddItem(szName, KEYBOARD_2UB);	
		snprintf(szName, sizeof(szName), "%s", BACKDOORMAN_KEYBOAD_Q16_2UC);
		pCombo->AddItem(szName, KEYBOARD_2UC);
		snprintf(szName, sizeof(szName), "%s", BACKDOORMAN_KEYBOAD_Q17_2UD);
		pCombo->AddItem(szName, KEYBOARD_2UD);	
		snprintf(szName, sizeof(szName), "%s", BACKDOORMAN_KEYBOAD_Q19_1_5U);
		pCombo->AddItem(szName, KEYBOARD_15U);	
		snprintf(szName, sizeof(szName), "%s", BACKDOORMAN_KEYBOAD_Q20_1UV);
		pCombo->AddItem(szName, KEYBOARD_1UV);	
		snprintf(szName, sizeof(szName), "%s", BACKDOORMAN_KEYBOAD_Q21_1UH);
		pCombo->AddItem(szName, KEYBOARD_1UH);	
		snprintf(szName, sizeof(szName), "%s", BACKDOORMAN_KEYBOAD_Q22_1UI);
		pCombo->AddItem(szName, KEYBOARD_1UI);	
		snprintf(szName, sizeof(szName), "%s", BACKDOORMAN_KEYBOAD_Q23_ONE_BTN);
		pCombo->AddItem(szName, KEYBOARD_1U_ONE_BTN);	
		pCombo->SetCurItem(0, false);

		list.AddItem(row, 2, WND_TYPE_BUTTON, m_length[BTN_WIDTH]);
		pBtn = dynamic_cast<GUI::CButton*>(list.GetItemWnd(row, 2));
		pBtn->SetCaption(BACKDOORMAN_SAVE, false);
		m_posBtn[BTN_KEYBOARD].row = row;
		m_posBtn[BTN_KEYBOARD].col = 2;
	}

	list.CreateListFocusMap();
}

bool CBackDoorDdnsDlg::HasBackDoorInfo(const WND_BACK_DOOR_INFO& info)
{
	if( (0 != info.DDNS) ||
		(0 != info.Keyboard))
	{
		return true;
	}

	return false;
}

BEGIN_MSG_MAP(CBackDoorDdnsDlg, GUI::CDialog)
	ON_MSG_EX(m_listCtrl.GetID(), KEY_VIRTUAL_MSG_DEFAULT_TAG, OnClickList)
END_MSG_MAP()

unsigned long CBackDoorDdnsDlg::OnClickList(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	unsigned short row = (keyID >> 16)&0x7fff;	//行
	unsigned short col = keyID&0xffff;			//列

	GUI::CListCtrl& list = m_listCtrl;
	MESSAGE_DATA msgData;
	PACKCMD *pCmd = NULL;
	unsigned long cmdType;

	if((m_posBtn[BTN_DDNS].row == row) && (m_posBtn[BTN_DDNS].col == col))
	{

		GUI::CComboCtrl *pCombo = dynamic_cast<GUI::CComboCtrl*>(list.GetItemWnd(row, 1) );
		int companyDdnsNum = pCombo->GetCurItemData();

		char szNum[7] = {0};
		snprintf(szNum, sizeof(szNum), "%d", companyDdnsNum);
		memset(&msgData, 0, sizeof(MESSAGE_DATA));
		CMessageMan::Instance()->SendMsgDataToServer(0, LOCAL_DEVICE_ID, CMD_REQUEST_MODIFY_DDNS_TYPE, reinterpret_cast<unsigned char *>(szNum), 7, msgData);
		pCmd = reinterpret_cast<PACKCMD *>(msgData.pData);
		cmdType = (NET_PROTOCOL)pCmd->cmdType;
		if (CMD_REPLY_CTRL_SUCC == cmdType)
		{			
			DVRSystem("rm -rf /mnt/mtd/config/* ");
			DVRSystem("rm -rf /mnt/BackUpNetConfig.dat");	
			CProduct::Instance()->writeDefaultResolution();
			MessageBox(BACKDOORMAN_SAVE_OK, m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK);
		}
		else
		{			
			MessageBox(BACKDOORMAN_SAVE_FAIL, m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK);	
		}

		return KEY_VIRTUAL_MSG_STOP;
	}

	if((m_posBtn[BTN_KEYBOARD].row == row) && (m_posBtn[BTN_KEYBOARD].col == col))
	{
		char str[50] = {0};
		GUI::CComboCtrl *pCombo = dynamic_cast<GUI::CComboCtrl*>(list.GetItemWnd(row, 1) );
		int keyboardNum = pCombo->GetCurItemData();

		bool bKbExist = true;
		switch(keyboardNum)
		{
		case KEYBOARD_1UA:
			snprintf(str, 6, "%s", "Q1");
			break;
		case KEYBOARD_1UB:
			snprintf(str, 6, "%s", "Q2");
			break;
		case KEYBOARD_1UC:
			snprintf(str, 6, "%s", "Q3");
			break;
		case KEYBOARD_1UD:
			snprintf(str, 6, "%s", "Q4");
			break;
		case KEYBOARD_1UE:
			snprintf(str, 6, "%s", "Q5");
			break;
		case KEYBOARD_1UF:
			snprintf(str, 6, "%s", "Q6");
			break;
		case KEYBOARD_1UG:
			snprintf(str, 6, "%s", "Q7");
			break;
		case KEYBOARD_1US:
			snprintf(str, 6, "%s", "Q8");
			break;
		case KEYBOARD_1UT:
			snprintf(str, 6, "%s", "Q9");
			break;
		case KEYBOARD_SUA:
			snprintf(str, 6, "%s", "Q10");
			break;
		case KEYBOARD_SUB:
			snprintf(str, 6, "%s", "Q11");
			break;
		case KEYBOARD_SUC:
			snprintf(str, 6, "%s", "Q12");
			break;
		case KEYBOARD_1UTW:
			snprintf(str, 6, "%s", "Q13");
			break;
		case KEYBOARD_1USS:
			snprintf(str, 6, "%s", "Q18");
			break;
		case KEYBOARD_2UA:
			snprintf(str, 6, "%s", "Q14");
			break;
		case KEYBOARD_2UB:
			snprintf(str, 6, "%s", "Q15");
			break;
		case KEYBOARD_2UC:
			snprintf(str, 6, "%s", "Q16");
			break;
		case KEYBOARD_2UD:
			snprintf(str, 6, "%s", "Q17");
			break;
		case KEYBOARD_15U:
			snprintf(str, 6, "%s", "Q19");
			break;
		case KEYBOARD_1UV:
			snprintf(str, 6, "%s", "Q20");
			break;
		case KEYBOARD_1UH:
			snprintf(str, 6, "%s", "Q21");
			break;
		case KEYBOARD_1UI:
			snprintf(str, 6, "%s", "Q22");
			break;
		case KEYBOARD_1U_ONE_BTN:
			snprintf(str, 6, "%s", "Q23");
			break;
		default:
			bKbExist = false;
			break;
		}

		if (bKbExist)
		{
			memset(&msgData, 0, sizeof(MESSAGE_DATA));
			CMessageMan::Instance()->SendMsgDataToServer(0, LOCAL_DEVICE_ID, CMD_REQUEST_SET_KEYBOARD_NAME, reinterpret_cast<unsigned char *>(str), 7, msgData);
			pCmd = reinterpret_cast<PACKCMD *>(msgData.pData);
			cmdType = (NET_PROTOCOL)pCmd->cmdType;
			if (CMD_REPLY_CTRL_SUCC == cmdType)
			{
				DVRSystem("rm -rf /mnt/mtd/config/* ");
				CProduct::Instance()->writeDefaultResolution();
				MessageBox(BACKDOORMAN_SAVE_OK, m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK);
			}
			else
			{
				MessageBox(BACKDOORMAN_SAVE_FAIL, m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK);	

			}			
		}
		else
		{
			MessageBox(BACKDOORMAN_ERROR,  m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK);
		}

		return KEY_VIRTUAL_MSG_STOP;

	}

	return KEY_VIRTUAL_MSG_STOP;
}


//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////

CBackDoorMacDlg::CBackDoorMacDlg()
{

}

CBackDoorMacDlg::~CBackDoorMacDlg()
{

}

void CBackDoorMacDlg::OnInitial()
{
	GUI::CDialog::OnInitial();
	////////////////////////////////////
	memset(&m_posBtn, 0xFF, sizeof(m_posBtn));

	unsigned short x = m_length[DLG_OFFSET_SUB_LEFT], y = m_length[DLG_OFFSET_SUB_TOP];
	unsigned short width = m_rect.Width() - 2 * m_length[DLG_OFFSET_SUB_LEFT];
	unsigned short height = m_rect.Height() - y - m_length[DLG_OFFSET_SUB_BOTTOM];

	GUI::CEditCtrl* pEdit = NULL;
	GUI::CButton *pBtn = NULL;
	GUI::CListCtrl& list = m_listCtrl;
	unsigned short offsetEdge = 4;

	list.Create(GUI::CWnd::GetRandWndID(), x, y, width, height, this, 0, 0, false);
	list.AddColumn(NULL, width/3);
	list.AddColumn(NULL, width/3);
	list.AddColumn(NULL, width/3 - offsetEdge);

	int row = 0;

	//MAC
	if(0 != m_pBackDoorInfo->Mac)
	{
		list.AddItem(row, 0, BACKDOORMAN_MAC);

		pEdit = dynamic_cast<GUI::CEditCtrl*>(list.AddItem(row, 1, WND_TYPE_EDIT));
		pEdit->SetMaxStringLen( 12 );		

		pBtn = dynamic_cast<GUI::CButton*>(list.AddItem(row, 2, WND_TYPE_BUTTON, m_length[BTN_WIDTH]));
		pBtn->SetCaption(BACKDOORMAN_SAVE, false);		
		m_posBtn[BTN_MAC].row = row;
		m_posBtn[BTN_MAC].col = 2;

		row += 2;
	}

	//HARDWARE
	if(0 != m_pBackDoorInfo->Hardware)
	{
		list.AddItem(row, 0, BACKDOORMAN_HARDWARE);

		pEdit = dynamic_cast<GUI::CEditCtrl*>(list.AddItem(row, 1, WND_TYPE_EDIT));
		pEdit->SetMaxStringLen( 6 );

		pBtn =  dynamic_cast<GUI::CButton*>(list.AddItem(row, 2, WND_TYPE_BUTTON, m_length[BTN_WIDTH]));
		pBtn->SetCaption(BACKDOORMAN_SAVE, false);	
		m_posBtn[BTN_HARDWARE].row = row;
		m_posBtn[BTN_HARDWARE].col = 2;
	}

	list.CreateListFocusMap();
}

bool CBackDoorMacDlg::HasBackDoorInfo(const WND_BACK_DOOR_INFO& info)
{
	if( (0 != info.Mac) ||
		(0 != info.Hardware))
	{
		return true;
	}

	return false;
}

BEGIN_MSG_MAP(CBackDoorMacDlg, GUI::CDialog)
	ON_MSG_EX(m_listCtrl.GetID(), KEY_VIRTUAL_MSG_DEFAULT_TAG, OnClickList)
END_MSG_MAP()

unsigned long CBackDoorMacDlg::OnClickList(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	unsigned short row = (keyID >> 16)&0x7fff;	//行
	unsigned short col = keyID&0xffff;			//列

	GUI::CListCtrl& list = m_listCtrl;
	GUI::CEditCtrl* pEdit = NULL;
	GUI::CButton* pBtn = NULL;
	MESSAGE_DATA msgData;
	PACKCMD *pCmd = NULL;
	unsigned long cmdType;

	if((m_posBtn[BTN_MAC].row == row) && (m_posBtn[BTN_MAC].col == col))
	{
		char str[50] = {0};
		unsigned char macaddr[6] = {0};
		pEdit = dynamic_cast<GUI::CEditCtrl*>(list.GetItemWnd(row, 1) );		
		snprintf(str, 13, "%s", pEdit->GetCaption().c_str());
		if (GetMac(str, macaddr))
		{
			memset(&msgData, 0, sizeof(MESSAGE_DATA));
			CMessageMan::Instance()->SendMsgDataToServer(0, LOCAL_DEVICE_ID, CMD_REQUEST_MODIFY_MAC, reinterpret_cast<unsigned char *>(macaddr), sizeof(macaddr),msgData);
			pCmd = reinterpret_cast<PACKCMD *>(msgData.pData);
			cmdType = (NET_PROTOCOL)pCmd->cmdType;
			if (CMD_REPLY_CTRL_SUCC == cmdType)
			{
				DVRSystem("rm -rf /mnt/mtd/config/* ");
				CProduct::Instance()->writeDefaultResolution();
				MessageBox(BACKDOORMAN_SAVE_OK, m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK);
			}
			else
			{
				MessageBox(BACKDOORMAN_SAVE_FAIL, m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK);		
			}
		}
		else
		{
			pEdit->SetCaption("");			
			MessageBox(BACKDOORMAN_ERROR,  m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK);
		}	
		return KEY_VIRTUAL_MSG_STOP;
	}

	if((m_posBtn[BTN_HARDWARE].row == row) && (m_posBtn[BTN_HARDWARE].col == col))
	{
		char str[50] = {0};
		pEdit = dynamic_cast<GUI::CEditCtrl*>(list.GetItemWnd(row, 1) );		
		snprintf(str, 7, "%s", pEdit->GetCaption().c_str());
		if (CheckHardwareVersion(str))
		{
			memset(&msgData, 0, sizeof(MESSAGE_DATA));
			CMessageMan::Instance()->SendMsgDataToServer(0, LOCAL_DEVICE_ID, CMD_REQUEST_MODIFY_HARDWARE_VER, reinterpret_cast<unsigned char *>(str), 7, msgData);
			pCmd = reinterpret_cast<PACKCMD *>(msgData.pData);
			cmdType = (NET_PROTOCOL)pCmd->cmdType;
			if (CMD_REPLY_CTRL_SUCC == cmdType)
			{
				DVRSystem("rm -rf /mnt/mtd/config/* ");
				CProduct::Instance()->writeDefaultResolution();
				MessageBox(BACKDOORMAN_SAVE_OK, m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK);
			}
			else
			{
				MessageBox(BACKDOORMAN_SAVE_FAIL, m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK);	
			}
		}
		else
		{
			pEdit->SetCaption("");			
			MessageBox(BACKDOORMAN_ERROR,  m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK);
		}		
		return KEY_VIRTUAL_MSG_STOP;
	}

	return KEY_VIRTUAL_MSG_STOP;
}


bool CBackDoorMacDlg::GetMac(char *pInput, unsigned char *pMacaddr)
{
	assert(pInput != NULL);

	bool bRet = false;
	if(12 != strlen(pInput))
	{
		return false;
	}

	unsigned char tmpvalue = 0;
	for(int i = 0; i < 12; i++)
	{
		//三种合法值中的一种
		if((pInput[i] >= '0') && (pInput[i] <= '9'))
		{
			tmpvalue = (unsigned char)pInput[i] - '0';
			bRet = true;
		}
		else if((pInput[i] >= 'a') && (pInput[i] <= 'f'))
		{
			tmpvalue = (unsigned char)pInput[i] - 'a' + 10;
			bRet = true;
		}
		else if((pInput[i] >= 'A') && (pInput[i] <= 'F'))
		{
			tmpvalue = (unsigned char)pInput[i] - 'A' + 10;
			bRet = true;
		}
		else
		{
			bRet = false;
			break;
		}

		if(0 == (i % 2))
		{
			//高4位
			pMacaddr[i/2] += tmpvalue * 0x10;
		}
		else
		{
			//低4位
			pMacaddr[i/2] += tmpvalue;
		}
	}

	return bRet;
}

bool CBackDoorMacDlg::CheckHardwareVersion(char *pVersion)
{
	assert(pVersion != NULL);

	bool bRet = false;
	bool hasDot = false;

	if (strlen(pVersion) >= 7)
	{
		return false;
	}

	for(int i = 0; i < strlen(pVersion); i++)
	{
		//三种合法值中的一种
		if((pVersion[i] >= '0') && (pVersion[i] <= '9'))
		{
			hasDot = false;
			bRet = true;
		}
		else if((pVersion[i] == '.'))
		{
			//如果第一个就为点号,输入错误
			if (0==i) 
			{
				bRet = false;
				break;
			}
			bRet = true;

			//如果连续有两个点号,输入错误
			if (hasDot) 
			{
				bRet = false;
				break;
			}
			hasDot = true;
		}
		else if((pVersion[i] >= 'a') && (pVersion[i] <= 'z'))
		{
			hasDot = false;
			bRet = true;			
		}
		else if((pVersion[i] >= 'A') && (pVersion[i] <= 'Z'))
		{
			hasDot = false;
			bRet = true;
		}
		else
		{
			//存在非法值，直接清空，重新输入
			bRet = false;
			break;
		}
	}

	return bRet;

}

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
CBackDoorLangDlg::CBackDoorLangDlg()
{

}

CBackDoorLangDlg::~CBackDoorLangDlg()
{

}

void CBackDoorLangDlg::OnInitial()
{
	GUI::CDialog::OnInitial();
	////////////////////////////////////
	memset(&m_posBtn, 0xFF, sizeof(m_posBtn));

	unsigned short x = m_length[DLG_OFFSET_SUB_LEFT], y = m_length[DLG_OFFSET_SUB_TOP];
	unsigned short width = m_rect.Width() - 2 * m_length[DLG_OFFSET_SUB_LEFT];
	unsigned short height = m_rect.Height() - y - m_length[DLG_OFFSET_SUB_BOTTOM];

	char szName[48] = {0};
	GUI::CListCtrl& list = m_listCtrl;
	GUI::CComboCtrl *pCombo = NULL;
	GUI::CButton *pBtn = NULL;
	GUI::CCheckCtrl* pCheck = NULL;
	unsigned short offsetEdge = 4;

	list.Create(GUI::CWnd::GetRandWndID(), x, y, width, height, this, 0, 0, false);
	list.AddColumn(NULL, width/3);
	list.AddColumn(NULL, width/3);
	list.AddColumn(NULL, width/3 - offsetEdge);


	if ((0 != m_pBackDoorInfo->CompanyID) && (0 != m_pBackDoorInfo->Lang))
	{
		list.AddItem(ROW_CRYTO_INFO, 0, BACKDOORMAN_CRYTOINFO);
	}
	
	//BTN_COMPANY_ID
	if(0 != m_pBackDoorInfo->CompanyID)
	{
		list.AddItem(ROW_COMPANY, 0, BACKDOORMAN_SELECT_CMP);

		pCombo = dynamic_cast<GUI::CComboCtrl*>(list.AddItem(ROW_COMPANY, 1, WND_TYPE_COMBO));
		snprintf(szName, 48, "IN-%s", BACKDOORMAN_COMPANY_IN);
		pCombo->AddItem(szName, TD_TVT_IN);
		snprintf(szName, 48, "%s", BACKDOORMAN_COMPANY_CN);
		pCombo->AddItem(szName, TD_TVT_CN);
		snprintf(szName, 48, "%s", BACKDOORMAN_COMPANY_SP);
		pCombo->AddItem(szName, TD_TVT_SP);
		snprintf(szName, 48, "%s", BACKDOORMAN_COMPANY_US02);
		pCombo->AddItem(szName, TD_TVT_US02);	
		snprintf(szName, 48, "%s", BACKDOORMAN_COMPANY_RU04);
		pCombo->AddItem(szName, TD_TVT_RU04);	
		snprintf(szName, 48, "%s", BACKDOORMAN_COMPANY_BR27);
		pCombo->AddItem(szName, TD_TVT_BR27);	
		snprintf(szName, 48, "%s", BACKDOORMAN_COMPANY_TW01);
		pCombo->AddItem(szName, TD_TVT_TW01);	
		snprintf(szName, 48, "%s", BACKDOORMAN_COMPANY_GB14);
		pCombo->AddItem(szName, TD_TVT_GB14);
		snprintf(szName, 48, "%s", BACKDOORMAN_COMPANY_IN23);
		pCombo->AddItem(szName, TD_TVT_IN23);	
		snprintf(szName, 48, "%s", BACKDOORMAN_COMPANY_US47);
		pCombo->AddItem(szName, TD_TVT_US47);
		snprintf(szName, 48, "%s", BACKDOORMAN_COMPANY_IN25);
		pCombo->AddItem(szName, TD_TVT_IN25);	
		snprintf(szName, 48, "%s", BACKDOORMAN_COMPANY_PL14);
		pCombo->AddItem(szName, TD_TVT_PL14);
		snprintf(szName, 48, "%s", BACKDOORMAN_COMPANY_IL03);
		pCombo->AddItem(szName, TD_TVT_IL03);
		snprintf(szName, 48, "%s", BACKDOORMAN_COMPANY_LV01);
		pCombo->AddItem(szName, TD_TVT_LV01);
		snprintf(szName, 48, "%s", BACKDOORMAN_COMPANY_CBC);
		pCombo->AddItem(szName, TD_TVT_CBC);
		snprintf(szName, 48, "%s", BACKDOORMAN_COMPANY_GB31);
		pCombo->AddItem(szName, TD_TVT_GB31);
		snprintf(szName, 48, "%s", BACKDOORMAN_COMPANY_ZNV);
		pCombo->AddItem(szName, TD_TVT_ZNV);

		unsigned long subId = CProduct::Instance()->GetProductSubIDInHardware();
		if (subId > TD_TVT_ZNV)
		{
			pCombo->SetCurItem(0);
		}
		else
		{
			pCombo->SetCurItemData(subId, false);
		}
	}

	//Lang
	int languageNum = MAX_LANGUAGE_PACK_NUM;
	if(0 != m_pBackDoorInfo->Lang)
	{
		list.AddItem(ROW_LANGUAGE, 0, BACKDOORMAN__SELECT_LAN);

		unsigned long languageBits = LANGUAGE_ALL;
		for (int count = 0; count < languageNum; ++count)
		{
			NCFG_INFO_LANGUAGE Language;
			memset(&Language, 0, sizeof(Language));
			pCheck = dynamic_cast<GUI::CCheckCtrl*>(list.AddItem(ROW_LANGUAGE+count, 1, WND_TYPE_CHECK));
			Language.languageId = ((unsigned long)(0x01)) << count;
			CLanguagePack::GetLanguageNameForBackDoor(Language.languageId, Language.name, 64);
			pCheck->SetCaption(Language.name, false, false);
			pCheck->SetCheck(true, false);
		}

		pBtn = dynamic_cast<GUI::CButton*>(list.AddItem(ROW_LANGUAGE + 1, 0,  WND_TYPE_BUTTON, m_length[BTN_WIDTH] + 50, m_length[BTN_HEIGHT]));
		pBtn->SetCaption("V", false);
		m_posBtn[BTN_SEL_LAN_ALL].row = ROW_LANGUAGE + 1;
		m_posBtn[BTN_SEL_LAN_ALL].col = 0;	

		pBtn = dynamic_cast<GUI::CButton*>(list.AddItem(ROW_LANGUAGE + 2, 0,  WND_TYPE_BUTTON, m_length[BTN_WIDTH] + 50, m_length[BTN_HEIGHT]));
		pBtn->SetCaption("X", false);	
		m_posBtn[BTN_SEL_LAN_CAL].row = ROW_LANGUAGE + 2;
		m_posBtn[BTN_SEL_LAN_CAL].col = 0;	
	}

	if ((0 != m_pBackDoorInfo->Lang) || (0 != m_pBackDoorInfo->CompanyID))
	{
		int row = ROW_LANGUAGE + languageNum;		
		pBtn = dynamic_cast<GUI::CButton*>(list.AddItem(row, 2, WND_TYPE_BUTTON, m_length[BTN_WIDTH] + 50, m_length[BTN_HEIGHT]) );
		pBtn->SetCaption(BACKDOORMAN_SAVE, false);	
		m_posBtn[BTN_SAVE].row = row;
		m_posBtn[BTN_SAVE].col = 2;	
	}

	list.CreateListFocusMap();
}

bool CBackDoorLangDlg::HasBackDoorInfo(const WND_BACK_DOOR_INFO& info)
{
	if( (0 != info.CompanyID) ||
		(0 != info.Lang))
	{
		return true;
	}

	return false;
}

BEGIN_MSG_MAP(CBackDoorLangDlg, GUI::CDialog)
	ON_MSG_EX(m_listCtrl.GetID(), KEY_VIRTUAL_MSG_DEFAULT_TAG, OnClickList)
END_MSG_MAP()

unsigned long CBackDoorLangDlg::OnClickList(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	unsigned short row = (keyID >> 16)&0x7fff;	//行
	unsigned short col = keyID&0xffff;			//列
	GUI::CListCtrl& list = m_listCtrl;

	MESSAGE_DATA msgData;
	PACKCMD *pCmd = NULL;
	unsigned long cmdType;

	int languageNum = MAX_LANGUAGE_PACK_NUM;

	if(0 != m_pBackDoorInfo->Lang)
	{
		if((m_posBtn[BTN_SEL_LAN_ALL].row == row) && (m_posBtn[BTN_SEL_LAN_ALL].col == col))
		{
			for (int i = ROW_LANGUAGE; i < ROW_LANGUAGE + languageNum; ++i)
			{
				GUI::CCheckCtrl *pCheck = dynamic_cast<GUI::CCheckCtrl*>(list.GetItemWnd(i, 1) );
				pCheck->SetCheck(true);
			}

			return KEY_VIRTUAL_MSG_STOP;
		}

		if((m_posBtn[BTN_SEL_LAN_CAL].row == row) && (m_posBtn[BTN_SEL_LAN_CAL].col == col))
		{
			for (int i = ROW_LANGUAGE; i < ROW_LANGUAGE + languageNum; ++i)
			{
				GUI::CCheckCtrl *pCheck = dynamic_cast<GUI::CCheckCtrl*>(list.GetItemWnd(i, 1) );
				pCheck->SetCheck(false);
			}

			return KEY_VIRTUAL_MSG_STOP;
		}
	}

	
	if((m_posBtn[BTN_SAVE].row == row) && (m_posBtn[BTN_SAVE].col == col))
	{
		bool bOk = false;
		unsigned long languageBits = 0;
		unsigned char companyID = 0;

		if(0 != m_pBackDoorInfo->Lang)
		{
			for (int i = ROW_LANGUAGE; i < ROW_LANGUAGE + languageNum; ++i)
			{
				GUI::CCheckCtrl *pCheck = dynamic_cast<GUI::CCheckCtrl*>(list.GetItemWnd(i, 1) );
				if (pCheck->IsCheck())
				{
					unsigned long languageId = ((unsigned long)(0x01)) << (i - ROW_LANGUAGE);
					languageBits |= languageId;
				}
			}
		}

		if (languageBits != 0)
		{
			GUI::CComboCtrl *pCombo = dynamic_cast<GUI::CComboCtrl*>(list.GetItemWnd(ROW_COMPANY, 1) );
			if(0 != m_pBackDoorInfo->CompanyID)
			{
				companyID = pCombo->GetCurItemData();
			}
			else
			{
				companyID = CProduct::Instance()->GetProductSubIDInHardware();
			}

			memset(&msgData, 0, sizeof(MESSAGE_DATA));
			CRYTOINFO info;
			info.languageBits = languageBits;
			info.productSubID = companyID;
			CMessageMan::Instance()->SendMsgDataToServer(0, LOCAL_DEVICE_ID, CMD_REQUEST_MODIFY_CRYPTO_INFO, reinterpret_cast<unsigned char *>(&info), sizeof(CRYTOINFO), msgData);
			pCmd = reinterpret_cast<PACKCMD *>(msgData.pData);
			cmdType = (NET_PROTOCOL)pCmd->cmdType;
			if (CMD_REPLY_CTRL_SUCC == cmdType)
			{
				bOk = true;
			}
			
		}

		if (bOk)
		{				
			DVRSystem("rm -rf /mnt/mtd/config/* ");
			CProduct::Instance()->writeDefaultResolution();
			MessageBox(BACKDOORMAN_SAVE_OK, m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK);
		}
		else
		{
			MessageBox(BACKDOORMAN_SAVE_FAIL, m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK);
		}

		return KEY_VIRTUAL_MSG_STOP;
	}
	
	return KEY_VIRTUAL_MSG_STOP;
}


//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////

CBackDoorManufacturerDlg::CBackDoorManufacturerDlg()
{

}

CBackDoorManufacturerDlg::~CBackDoorManufacturerDlg()
{

}

void CBackDoorManufacturerDlg::OnInitial()
{
	GUI::CDialog::OnInitial();
	////////////////////////////////////
	memset(&m_posBtn, 0xFF, sizeof(m_posBtn));

	unsigned short x = m_length[DLG_OFFSET_SUB_LEFT], y = m_length[DLG_OFFSET_SUB_TOP];
	unsigned short width = m_rect.Width() - 2 * m_length[DLG_OFFSET_SUB_LEFT];
	unsigned short height = m_rect.Height() - y - m_length[DLG_OFFSET_SUB_BOTTOM];

	GUI::CEditCtrl* pEdit = NULL;
	GUI::CButton *pBtn = NULL;
	GUI::CListCtrl& list = m_listCtrl;
	unsigned short offsetEdge = 4;

	list.Create(GUI::CWnd::GetRandWndID(), x, y, width, height, this, 0, 0, false);
	list.AddColumn(NULL, width/3);
	list.AddColumn(NULL, width/3);
	list.AddColumn(NULL, width/3 - offsetEdge);

	int row = 0;

	if(0 != m_pBackDoorInfo->CompanyName)
	{
		list.AddItem(row, 0, BACKDOORMAN_COMPANY_NAME);

		pEdit = dynamic_cast<GUI::CEditCtrl*>(list.AddItem(row, 1, WND_TYPE_EDIT));
		pEdit->SetMaxStringLen( 24 );

		pBtn =  dynamic_cast<GUI::CButton*>(list.AddItem(row, 2, WND_TYPE_BUTTON, m_length[BTN_WIDTH]));
		pBtn->SetCaption(BACKDOORMAN_SAVE, false);	
		m_posBtn[BTN_MANUFACTURER].row = row;
		m_posBtn[BTN_MANUFACTURER].col = 2;
	}

	list.CreateListFocusMap();
}

bool CBackDoorManufacturerDlg::HasBackDoorInfo(const WND_BACK_DOOR_INFO& info)
{
	if(0 != info.CompanyName)
	{
		return true;
	}

	return false;
}

BEGIN_MSG_MAP(CBackDoorManufacturerDlg, GUI::CDialog)
	ON_MSG_EX(m_listCtrl.GetID(), KEY_VIRTUAL_MSG_DEFAULT_TAG, OnClickList)
END_MSG_MAP()

unsigned long CBackDoorManufacturerDlg::OnClickList(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	unsigned short row = (keyID >> 16)&0x7fff;	//行
	unsigned short col = keyID&0xffff;			//列

	GUI::CListCtrl& list = m_listCtrl;
	GUI::CEditCtrl* pEdit = NULL;
	GUI::CButton* pBtn = NULL;
	MESSAGE_DATA msgData;
	PACKCMD *pCmd = NULL;
	unsigned long cmdType;

	if((m_posBtn[BTN_MANUFACTURER].row == row) && (m_posBtn[BTN_MANUFACTURER].col == col))
	{
		char str[32] = {0};
		pEdit = dynamic_cast<GUI::CEditCtrl*>(list.GetItemWnd(row, 1) );
		snprintf(str, 31, "%s", pEdit->GetCaption().c_str());

		memset(&msgData, 0, sizeof(MESSAGE_DATA));
		CMessageMan::Instance()->SendMsgDataToServer(0, LOCAL_DEVICE_ID, CMD_REQUEST_MODIFY_COMPANY_NAME, (unsigned char *)str, 32, msgData);
		pCmd = reinterpret_cast<PACKCMD *>(msgData.pData);
		cmdType = (NET_PROTOCOL)pCmd->cmdType;
		if (CMD_REPLY_CTRL_SUCC == cmdType)
		{
			MessageBox(BACKDOORMAN_SAVE_OK, m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK);
		}
		else
		{
			MessageBox(BACKDOORMAN_SAVE_FAIL, m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK);	
		}

		return KEY_VIRTUAL_MSG_STOP;
	}

	return KEY_VIRTUAL_MSG_STOP;
}

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
CBackDoorManDlg::CBackDoorManDlg()
{
	memset(&m_backDoorInfo, 0, sizeof(WND_BACK_DOOR_INFO));
}

CBackDoorManDlg::~CBackDoorManDlg()
{

}

void CBackDoorManDlg::OnInitial()
{
	GUI::CDialog::OnInitial();
	SetImage(BMP_MEM_DLG_LARGE, false);
	SetDefaultBtn(DEFAULT_BTN_CANCEL);
	SetDefaultBtnCaption(DEFAULT_BTN_CANCEL, m_pStrTable->String(E_STR_ID_EXIT));
	SetCaption("BACK DOOR");
	////////////////////////////////////

	unsigned short x = m_length[DLG_OFFSET_LEFT];
	unsigned short y = m_length[TITLE_HEIGHT] + m_length[TABLE_HEIGHT] + m_length[DLG_OFFSET_TOP];
	unsigned short offset = m_length[DLG_OFFSET_BOTTOM];
	unsigned short cx = m_rect.Width() - 2 * m_length[DLG_OFFSET_LEFT];
	unsigned short cy = m_rect.Height() - y- offset - m_length[DEFAULT_BTN_BOTTOM_YPOS];

	int index = 2;

	if(CBackDoorLogoImgDlg::HasBackDoorInfo(m_backDoorInfo))
	{
		m_pageLogoDlg.SetBackDoorInfo(&m_backDoorInfo);
		m_pageLogoDlg.Create(GetRandWndID(), x, y, cx, cy, this, 0, index++);
	}

	if(CBackDoorDdnsDlg::HasBackDoorInfo(m_backDoorInfo))
	{
		m_pageDdnsDlg.SetBackDoorInfo(&m_backDoorInfo);
		m_pageDdnsDlg.Create(GetRandWndID(), x, y, cx, cy, this, 0, index++);
	}

	if(CBackDoorMacDlg::HasBackDoorInfo(m_backDoorInfo))
	{
		m_pageMacDlg.SetBackDoorInfo(&m_backDoorInfo);
		m_pageMacDlg.Create(GetRandWndID(), x, y, cx, cy, this, 0, index++);
	}

	if(CBackDoorLangDlg::HasBackDoorInfo(m_backDoorInfo))
	{
		m_pageLangDlg.SetBackDoorInfo(&m_backDoorInfo);
		m_pageLangDlg.Create(GetRandWndID(), x, y, cx, cy, this, 0, index++);
	}


	if(CBackDoorManufacturerDlg::HasBackDoorInfo(m_backDoorInfo))
	{
		m_pageComanyDlg.SetBackDoorInfo(&m_backDoorInfo);
		m_pageComanyDlg.Create(GetRandWndID(), x, y, cx, cy, this, 0, index++);
	}

	m_tabCtrl.Create(GetRandWndID(), x, m_length[TITLE_HEIGHT] + m_length[DLG_OFFSET_TOP], cx, m_length[TABLE_HEIGHT], this, 0, 1);
	
	if(CBackDoorLogoImgDlg::HasBackDoorInfo(m_backDoorInfo))
	{
		m_tabCtrl.AddItem(&m_pageLogoDlg, "Logo");
	}

	if(CBackDoorDdnsDlg::HasBackDoorInfo(m_backDoorInfo))
	{
		m_tabCtrl.AddItem(&m_pageDdnsDlg, "DDNS & Keyboard");
	}

	if(CBackDoorMacDlg::HasBackDoorInfo(m_backDoorInfo))
	{
		m_tabCtrl.AddItem(&m_pageMacDlg, "MAC & Hardware");
	}

	if(CBackDoorLangDlg::HasBackDoorInfo(m_backDoorInfo))
	{
		m_tabCtrl.AddItem(&m_pageLangDlg, "Language");
	}

	if(CBackDoorManufacturerDlg::HasBackDoorInfo(m_backDoorInfo))
	{
		m_tabCtrl.AddItem(&m_pageComanyDlg, "Company name");
	}

	m_tabCtrl.SetCurItem(0, false);
}

void CBackDoorManDlg::SetBackDoorInfo(const WND_BACK_DOOR_INFO& info)
{
	memcpy(&m_backDoorInfo, &info, sizeof(WND_BACK_DOOR_INFO));
}

//end
