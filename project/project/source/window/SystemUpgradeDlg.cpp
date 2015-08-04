/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : L
** Date         : 2009-08-19
** Name         : SystemUpgradeDlg.cpp
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/
#include "SystemUpgradeDlg.h"
#include "ListCtrlEx.cpp"
#include "NetProtocol.h"
#include "MessageMan.h"
#include "TimeCtrl.h"
#include "Version.h"
#include "ConfigEx.h"

extern DVR_SYSTEM_STATUS g_system_status;
bool g_upgrade_status = false;
extern bool g_bProIdequeSubProdID;

CSystemUpgradeDlg::CSystemUpgradeDlg()
{
	memset(m_selectFileName, 0, sizeof(m_selectFileName));
}

CSystemUpgradeDlg::~CSystemUpgradeDlg()
{

}

void CSystemUpgradeDlg::OnInitial()
{
	SetStyle(SYSTEM_UPGRADE_DLG);
	CUSBExploreDlg::OnInitial();

#ifdef  __ENVIRONMENT_LINUX__
	if (g_bProIdequeSubProdID)
	{
		#ifdef __CUSTOM_US02_NEW__
			SetCaption(m_pStrTable->String(E_STR_ID_TITLE_UPGRADE_01), false);
		#else
			SetCaption(m_pStrTable->String(E_STR_ID_TITLE_UPGRADE), false);
		#endif
	}
	else
	{
		char szCaption[256] = {0};
		char szTemp[36] = {0};
		CProduct::Instance()->GetHardwarePreVersion(szTemp, 36);
		#ifdef __CUSTOM_US02_NEW__
			snprintf(szCaption, 256, "%s-%s", m_pStrTable->String(E_STR_ID_TITLE_UPGRADE_01), szTemp);
		#else
			snprintf(szCaption, 256, "%s-%s", m_pStrTable->String(E_STR_ID_TITLE_UPGRADE), szTemp);
		#endif
		SetCaption(szCaption, false);		
	}	
#else
	#ifdef __CUSTOM_US02_NEW__
		SetCaption(m_pStrTable->String(E_STR_ID_TITLE_UPGRADE_01), false);
	#else
		SetCaption(m_pStrTable->String(E_STR_ID_TITLE_UPGRADE), false);
	#endif
#endif
	#ifdef __CUSTOM_US02_NEW__
		SetDefaultBtnCaption( DEFAULT_BTN_OK, m_pStrTable->String(E_STR_ID_UPGRADE_01) );
	#else
		SetDefaultBtnCaption( DEFAULT_BTN_OK, m_pStrTable->String(E_STR_ID_UPGRADE) );
	#endif

}

#if defined (__TDFH__)
 unsigned long CSystemUpgradeDlg::OnOK()
{
#ifdef __ENVIRONMENT_LINUX__
	CUSBExploreDlg::OnOK();
	char cmd [512] = {0};
	printf("%s,%d:the select file:%s\n", __FILE__, __LINE__, m_selectFileName);

	//是否有选择的文件
	if (0 == strlen(m_selectFileName))
	{
		MessageBox(m_pStrTable->String(E_STR_ID_MESSAGE_07), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK|DEFAULT_ICON_WARNING);
		return KEY_VIRTUAL_MSG_STOP;  
	}

	//确定是否要升级
	if (DLG_OK != MessageBox(m_pStrTable->String(E_STR_ID_MESSAGE_17), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK|DEFAULT_BTN_CANCEL|DEFAULT_ICON_WARNING))
	{
		return KEY_VIRTUAL_MSG_STOP; 
	}
	
	//
	snprintf(cmd, 512, "rm -rf /mnt/upgrade");
	DVRSystem(cmd);

	snprintf(cmd, 512, "mkdir /mnt/upgrade");
	if (0 != DVRSystem(cmd))
	{
		MessageBox(m_pStrTable->String(E_STR_ID_MESSAGE_24), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK|DEFAULT_ICON_WARNING);
		return KEY_VIRTUAL_MSG_STOP;  
	}
	
	//先检查压缩包是否正确
	bool bOk = false;
	do
	{
		snprintf(cmd, 512, "cd /mnt/upgrade; tar xzvf %s preupgrade.sh", m_selectFileName);
		if (0 != DVRSystem(cmd))
		{
			break;
		}

		snprintf(cmd, 512, "cd /mnt/upgrade; tar xzvf %s productcheck", m_selectFileName);		
		if (0 != DVRSystem(cmd))
		{
			break;			
		}

		snprintf(cmd, 512, "/mnt/upgrade/productcheck");
	    if (0 != DVRSystem(cmd))
		{
			break;			
		}

		snprintf(cmd, 512, "/mnt/upgrade/check.dat");		
		if (access(cmd , R_OK) != 0)
		{
			break;
		}
		bOk = true;
	}while(0);

	if (!bOk)
	{

		snprintf(cmd, 512, "rm -rf /mnt/upgrade");		
		DVRSystem(cmd);
		MessageBox(m_pStrTable->String(E_STR_ID_MESSAGE_07), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK|DEFAULT_ICON_WARNING);
		return KEY_VIRTUAL_MSG_STOP; 
	}
	
	//检查是否可以升级
	snprintf(cmd, 512, "/mnt/upgrade/check.dat");
	if (!CheckVersion(cmd))
	{
		snprintf(cmd, 512, "rm -rf /mnt/upgrade");		
		DVRSystem(cmd);
		MessageBox(m_pStrTable->String(E_STR_ID_MESSAGE_07), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK|DEFAULT_ICON_WARNING);
		return KEY_VIRTUAL_MSG_STOP;  
	}

	snprintf(cmd, 512, "rm -rf /mnt/mtd/UpFile.dat");
	DVRSystem(cmd);
	
	int length = sizeof(_extern_usb_device_mount_path_) + 2;

	snprintf(cmd, 512, "echo %s > /mnt/mtd/UpFile.dat", &(m_selectFileName[length]));
	if (0 != DVRSystem(cmd))
	{
		snprintf(cmd, 512, "rm -rf /mnt/upgrade");		
		DVRSystem(cmd);
		MessageBox(m_pStrTable->String(E_STR_ID_MESSAGE_24), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK|DEFAULT_ICON_WARNING);
		return KEY_VIRTUAL_MSG_STOP; 
	}
	
	//设置升级标志
	if (!SetUpdateStatus(true))
	{
		snprintf(cmd, 512, "rm -rf /mnt/upgrade");		
		DVRSystem(cmd);
		snprintf(cmd, 512, "rm -rf /mnt/mtd/UpFile.dat");
		DVRSystem(cmd);
		MessageBox(m_pStrTable->String(E_STR_ID_MESSAGE_24), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK|DEFAULT_ICON_WARNING);
		return KEY_VIRTUAL_MSG_STOP;  
	}
	//升级前先备份好网络相关配置
	ClearDeviceInfo(false);
	g_system_status = DVR_SYSTEM_REBOOT;
	g_upgrade_status = true;

#endif

	return KEY_VIRTUAL_MSG_STOP;
}

#else

unsigned long CSystemUpgradeDlg::OnOK()
{
#ifdef __ENVIRONMENT_LINUX__
	char *filePath;
#if defined(__CHIPGM__)
	if ((TD_2704AS_SL == CProduct::Instance()->ProductType()) || (TD_2708TS_PL == CProduct::Instance()->ProductType()))
	{
		filePath = "/mnt";
	}
	else
	{
		filePath = "/upgrade";
	}
#else
	filePath = "/upgrade";
#endif

	CUSBExploreDlg::OnOK();
	char cmd [512] = {0};
	printf("%s,%d:the select file:%s\n", __FILE__, __LINE__, m_selectFileName);

	//是否有选择的文件
	if (0 == strlen(m_selectFileName))
	{
		MessageBox(m_pStrTable->String(E_STR_ID_MESSAGE_07), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK|DEFAULT_ICON_WARNING);
		return KEY_VIRTUAL_MSG_STOP;  
	}

	//确定是否要升级
	if (DLG_OK != MessageBox(m_pStrTable->String(E_STR_ID_MESSAGE_17), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK|DEFAULT_BTN_CANCEL|DEFAULT_ICON_WARNING))
	{
		return KEY_VIRTUAL_MSG_STOP; 
	}
	
	//
#if defined(__CHIPGM__)
	if ((TD_2704AS_SL != CProduct::Instance()->ProductType()) && (TD_2708TS_PL != CProduct::Instance()->ProductType()))
	{
		snprintf(cmd, 512, "rm -rf %s", filePath);
		DVRSystem(cmd);
		snprintf(cmd, 512, "mkdir %s", filePath);
		if (0 != DVRSystem(cmd))
		{
			MessageBox(m_pStrTable->String(E_STR_ID_MESSAGE_24), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK|DEFAULT_ICON_WARNING);
			return KEY_VIRTUAL_MSG_STOP;
		}
	}
#else
	snprintf(cmd, 512, "rm -rf %s", filePath);
	DVRSystem(cmd);
	snprintf(cmd, 512, "mkdir %s", filePath);
	if (0 != DVRSystem(cmd))
	{
		MessageBox(m_pStrTable->String(E_STR_ID_MESSAGE_24), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK|DEFAULT_ICON_WARNING);
		return KEY_VIRTUAL_MSG_STOP;  
	}
#endif
	
	//先检查压缩包是否正确
	bool bOk = false;
	do
	{
		snprintf(cmd, 512, "cd %s; tar xzvf %s preupgrade.sh", filePath, m_selectFileName);
		if (0 != DVRSystem(cmd))
		{
			break;
		}
		snprintf(cmd, 512, "cd %s; tar xzvf %s productcheck", filePath, m_selectFileName);
		if (0 != DVRSystem(cmd))
		{
			break;			
		}
		snprintf(cmd, 512, "%s/productcheck", filePath);
	    if (0 != DVRSystem(cmd))
		{
			break;			
		}
		snprintf(cmd, 512, "%s/check.dat", filePath);
		if (access(cmd , R_OK) != 0)
		{
			break;
		}
		bOk = true;
	}while(0);

	if (!bOk)
	{
#if defined(__CHIPGM__)
		if ((TD_2704AS_SL != CProduct::Instance()->ProductType()) && (TD_2708TS_PL != CProduct::Instance()->ProductType()))
		{
			snprintf(cmd, 512, "rm -rf %s", filePath);
			DVRSystem(cmd);
		}
#else
		snprintf(cmd, 512, "rm -rf %s", filePath);
		DVRSystem(cmd);
#endif
		MessageBox(m_pStrTable->String(E_STR_ID_MESSAGE_07), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK|DEFAULT_ICON_WARNING);
		return KEY_VIRTUAL_MSG_STOP; 
	}
	
	//检查是否可以升级
	snprintf(cmd, 512, "%s/check.dat", filePath);
	if (!CheckVersion(cmd))
	{
#if defined(__CHIPGM__)
		if ((TD_2704AS_SL != CProduct::Instance()->ProductType()) && (TD_2708TS_PL != CProduct::Instance()->ProductType()))
		{
			snprintf(cmd, 512, "rm -rf %s", filePath);
			DVRSystem(cmd);
		}
#else
		snprintf(cmd, 512, "rm -rf %s", filePath);
		DVRSystem(cmd);
#endif
		MessageBox(m_pStrTable->String(E_STR_ID_MESSAGE_07), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK|DEFAULT_ICON_WARNING);
		return KEY_VIRTUAL_MSG_STOP;  
	}

	snprintf(cmd, 512, "rm -rf /mnt/mtd/UpFile.dat");
	DVRSystem(cmd);
	
	int length = sizeof(_extern_usb_device_mount_path_) + 2;

	snprintf(cmd, 512, "echo %s > /mnt/mtd/UpFile.dat", &(m_selectFileName[length]));
	if (0 != DVRSystem(cmd))
	{
#if defined(__CHIPGM__)
		if ((TD_2704AS_SL != CProduct::Instance()->ProductType()) && (TD_2708TS_PL != CProduct::Instance()->ProductType()))
		{
			snprintf(cmd, 512, "rm -rf %s", filePath);
			DVRSystem(cmd);
		}
#else
		snprintf(cmd, 512, "rm -rf %s", filePath);
		DVRSystem(cmd);
#endif
		MessageBox(m_pStrTable->String(E_STR_ID_MESSAGE_24), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK|DEFAULT_ICON_WARNING);
		return KEY_VIRTUAL_MSG_STOP; 
	}
	
	//设置升级标志
	if (!SetUpdateStatus(true))
	{
#if defined(__CHIPGM__)
		if ((TD_2704AS_SL != CProduct::Instance()->ProductType()) && (TD_2708TS_PL != CProduct::Instance()->ProductType()))
		{
			snprintf(cmd, 512, "rm -rf %s", filePath);
			DVRSystem(cmd);
		}
#else
		snprintf(cmd, 512, "rm -rf %s", filePath);
		DVRSystem(cmd);
#endif
		snprintf(cmd, 512, "rm -rf /mnt/mtd/UpFile.dat");
		DVRSystem(cmd);
		MessageBox(m_pStrTable->String(E_STR_ID_MESSAGE_24), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK|DEFAULT_ICON_WARNING);
		return KEY_VIRTUAL_MSG_STOP;  
	}
	//升级前先备份好网络相关配置
	ClearDeviceInfo(false);

	{
		PRODUCT_TYPE productTypeUpdate = CProduct::Instance()->ProductType();
		if ((productTypeUpdate >= TD_2704HD) && (productTypeUpdate <= TD_2716XD_L))
		{
			if (FindStringInFile("/upgrade/preupgrade.sh", "#cp -f /mnt/mtd/config/* /mnt/Bkconfig/"))
			{
				printf("is not bkc\n");
				CProduct::Instance()->writeDefaultResolution();
			}
			else
			{
				printf("is bkc\n");
			}
		}
	}		
	g_system_status = DVR_SYSTEM_REBOOT;
	g_upgrade_status = true;

#endif

	return KEY_VIRTUAL_MSG_STOP;
}

#endif
void CSystemUpgradeDlg::EndDialog(DLG_END_VALUE result)
{
	CUSBExploreDlg::EndDialog(result);	
}

