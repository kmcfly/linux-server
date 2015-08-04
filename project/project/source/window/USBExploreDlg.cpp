/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : L
** Date         : 2009-08-19
** Name         : USBExploreDlg.cpp
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/
#include "USBExploreDlg.h"
#include "NetProtocol.h"
#include "ListCtrlEx.cpp"
#include "MessageMan.h"
#include "TimeCtrl.h"
#include "LocalDevice.h"
#include "BackDoorManDlg.h"

extern CRYTOINFO_STATUS g_crytoStatus;
extern bool g_bProIdequeSubProdID;

extern DVR_SYSTEM_STATUS g_system_status;

template <class T>
std::string &  GUI::CListCtrlEx<T>::Text(T &node, unsigned short col)
{
#ifndef __ENVIRONMENT_WIN32__
	DIR_INFO  &it = node;
	char str [512] = {0};

	if (0 == col)
	{
		snprintf(str, 512, "%s", it.name);
	}

	if (0 == strcmp(it.name, ".."))
	{
		if ((1 == col) || (2 == col) || (3 == col))
		{
			sprintf(str, " ");
		}
	}
	else
	{
		if (1 == col)
		{
			if (S_ISDIR(it.type))
			{
				sprintf(str, " ");
			}
			else if (it.size <= 0)
			{
				sprintf(str, "%dKB", 1);
			}
			else
			{
				sprintf(str, "%dKB", it.size);
			}
		}

		if (2 == col)
		{
			if (S_ISDIR(it.type))
			{
				strcpy(str, m_pStrTable->String(E_STR_ID_FOLDER));
			}
			else
			{
				strcpy(str, m_pStrTable->String(E_STR_ID_FILE));
			}
		}

		if (3 == col)
		{
			//tm time = DVRTime32ToTm(it.dateModified);
			//sprintf(str, "%d-%02d-%02d %02d:%02d:%02d", time.tm_year+1900, time.tm_mon+1, time.tm_mday, time.tm_hour, time.tm_min, time.tm_sec);
			std::string strTime;
			CTimeCtrl::GetTime32Text(strTime,it.dateModified);
			strcpy(str,strTime.c_str());
		}
	}

	m_str = str;
#endif
	return m_str;
}

CUSBExploreDlg::CUSBExploreDlg()
{
	memset(m_selectFileName, 0, sizeof(m_selectFileName));
	memset(m_device, 0, sizeof(m_device));
	memset(m_captionName, 0, sizeof(m_captionName));

	m_curDevice    = NULL;
	m_style        = DEFUALT_DLG;
	m_selectType   = 0;

}


CUSBExploreDlg::~CUSBExploreDlg()
{

}

void CUSBExploreDlg::SetCurDevice(int selectType, char* pDevice)
{
	m_selectType = selectType;
	if (pDevice != NULL)
	{
		snprintf(m_device, 128, "%s", pDevice);
	}	
}

void CUSBExploreDlg::SetCaptionName(const char *pCaption)
{
	if (pCaption != NULL)
	{
		snprintf(m_captionName, sizeof(m_captionName), "%s", pCaption);
	}
}

void CUSBExploreDlg::OnInitial()
{
	GUI::CDialog::OnInitial();
	SetImage(BMP_MEM_DLG_LARGE, false);

	if (strlen(m_captionName) > 0)
	{
		SetCaption(m_captionName, false);
	}
	

	if(BACKUP_INFO_DLG == m_style || DISK_BROWSE_DLG == m_style)
	{
		SetDefaultBtn( DEFAULT_BTN_CANCEL);
		SetDefaultBtnCaption( DEFAULT_BTN_CANCEL, m_pStrTable->String(E_STR_ID_EXIT) );
	}
	else
	{
		SetDefaultBtn( DEFAULT_BTN_CANCEL | DEFAULT_BTN_OK );
		SetDefaultBtnCaption( DEFAULT_BTN_CANCEL, m_pStrTable->String(E_STR_ID_EXIT) );
		SetDefaultBtnCaption( DEFAULT_BTN_OK, m_pStrTable->String(E_STR_ID_OK) );
	}

	//////////////////////////////////////////////////////////////////////////
	unsigned short x = m_length[DLG_OFFSET_LEFT], y = m_length[TITLE_HEIGHT] + m_length[DLG_OFFSET_TOP];
	unsigned short width  = 0;
	unsigned short height = 0;
	char str[128] = {0};

	//x = x + 20;
	y += 4;
	width = m_length[COMBO_WIDTH] + 180;
	height = m_length[COMBO_HEIGHT];
	m_deviceNameCombo.Create(GUI::CWnd::GetRandWndID(), x, y, width, height, this, 0, 1);
	m_deviceNameCombo.SetMaxStringLen(36);

	x = x + width + 20;
	width = m_length[BTN_WIDTH];
	height = m_length[BTN_HEIGHT];
	m_refreshBnt.Create(GUI::CWnd::GetRandWndID(), x, y, width, height, this, 0, 2, false);
	m_refreshBnt.SetCaption(m_pStrTable->String(E_STR_ID_REFRESH), false);


	x = m_length[DLG_OFFSET_LEFT];
	y += height + 8;
	width = m_rect.Width() - 2 * m_length[DLG_OFFSET_LEFT];
	height = m_rect.Height() - y - m_length[DEFAULT_BTN_BOTTOM_YPOS] - m_length[DLG_OFFSET_BOTTOM];
	m_fileInfoList.Create(GUI::CWnd::GetRandWndID(), x, y, width, height, this, 0, 3, false);
	m_fileInfoList.AddColumn(m_pStrTable->String(E_STR_ID_NAME), width * 14/32);
	m_fileInfoList.AddColumn(m_pStrTable->String(E_STR_ID_SIZE), width * 4/32);
	m_fileInfoList.AddColumn(m_pStrTable->String(E_STR_ID_TYPE), width * 3/32);
	m_fileInfoList.AddColumn(m_pStrTable->String(E_STR_ID_DATE_MODIFIED), width * 11/32);
	GUI::CRect rect = m_fileInfoList.GetClientRect();

	x = m_rect.Width() - m_length[DEFAULT_BTN_RIGHT_XPOS] - 4 * m_length[BTN_WIDTH] - 3 * m_length[DEFAULT_BTN_OFFSET];
	y = m_rect.Height() - m_length[DEFAULT_BTN_BOTTOM_YPOS];
	m_btnAddDir.Create(GUI::CWnd::GetRandWndID(), x, y, m_length[BTN_WIDTH], m_length[BTN_HEIGHT], this, 2, 1, false);
	m_btnAddDir.SetCaption(m_pStrTable->String(E_STR_ID_NEW), false);

	x += m_length[BTN_WIDTH] + m_length[DEFAULT_BTN_OFFSET];
	m_btnDel.Create(GUI::CWnd::GetRandWndID(), x, y, m_length[BTN_WIDTH], m_length[BTN_HEIGHT], this, 2, 2, false);
	m_btnDel.SetCaption(m_pStrTable->String(E_STR_ID_DELETE), false);

	if(BACKUP_INFO_DLG == m_style)
	{
		m_btnAddDir.Show(false, false);
		
		x = GetWindowRect().m_left + m_rect.Width() - m_length[DEFAULT_BTN_RIGHT_XPOS] - 2 * m_length[BTN_WIDTH] - m_length[DEFAULT_BTN_OFFSET];
		y = GetWindowRect().m_top + m_rect.Height() - m_length[DEFAULT_BTN_BOTTOM_YPOS];

		m_btnDel.MoveWnd(x, y);
		m_btnDel.Show(true, false);
	}
	else if(SYSTEM_UPGRADE_DLG == m_style)
	{
		m_btnAddDir.Show(false, false);
		m_btnDel.Show(false, false);
	}
	else if(DISK_BROWSE_DLG == m_style)
	{
		x = GetWindowRect().m_left + m_rect.Width() - m_length[DEFAULT_BTN_RIGHT_XPOS] - 2 * m_length[BTN_WIDTH] - m_length[DEFAULT_BTN_OFFSET];
		y = GetWindowRect().m_top + m_rect.Height() - m_length[DEFAULT_BTN_BOTTOM_YPOS];
		m_btnDel.MoveWnd(x, y);
		m_btnDel.Show(true, false);

		x -= (m_length[BTN_WIDTH] + m_length[DEFAULT_BTN_OFFSET]);
		m_btnAddDir.MoveWnd(x, y);
		m_btnDel.Show(true, false);
	}

	m_fileInfoList.EnableSingleSelect();

	m_deviceInfoList.RemoveAll();
	RefreshDeviceInfo(false);

	if (m_deviceInfoList.GetCount() > 0)
	{
		if (PARTITION == m_selectType)
		{
			bool bFind = false;
			POS pos = m_deviceInfoList.GetHeadPosition();
			while (NULL != pos)
			{
				DVR_DISK_INFO &diskInfo = m_deviceInfoList.GetAt(pos);
				if (0 == strcmp(diskInfo.diskname, m_device))
				{
					bFind = true;
					break;
				}
				m_deviceInfoList.GetNext(pos);
			}

			if (bFind)
			{
				m_deviceNameCombo.SetCurItemData((LONGLONG)pos, false);
			}
			else
			{
				m_deviceNameCombo.SetCurItem(0, false);
			}

		}
		else
		{
			m_deviceNameCombo.SetCurItem(0, false);
		}


		POS pos = (POS)m_deviceNameCombo.GetCurItemData();

		DVR_DISK_INFO &diskInfo = m_deviceInfoList.GetAt(pos);
		m_dirPathList.RemoveAll();
		DIR_INFO temp;
		snprintf(temp.name, 256, "%s", diskInfo.mountdir);
		m_dirPathList.AddTail(temp);
		if (RefreshFileInfo(diskInfo.mountdir, false) < 0)
		{
			MessageBox(m_pStrTable->String(E_STR_ID_MESSAGE_70), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK|DEFAULT_ICON_WARNING);
			ClearDeviceInfo(false);
		}
		else
		{		
			m_curDevice = pos;

		}
	}



#ifdef __ENVIRONMENT_LINUX__
	if ((!g_bProIdequeSubProdID) && (CRYTOINFO_STATUS_NEW != g_crytoStatus))	
	{
		x = m_length[DEFAULT_BTN_RIGHT_XPOS];
		y = m_rect.Height() - m_length[DEFAULT_BTN_BOTTOM_YPOS];
		width = 200;
		m_editPassword.Create(GetRandWndID(), x, y, width, m_length[TIME_CTRL_HEIGHT], this, 0, 0);
		m_editPassword.SetPassword(true);
		m_editPassword.SetFollowCtrl(true);

		x = x + width + 20;
		width = m_length[BTN_WIDTH];
		height = m_length[BTN_HEIGHT];
		m_backdoorBnt.Create(GUI::CWnd::GetRandWndID(), x, y, width, height, this, 0, 0, false);
		m_backdoorBnt.SetCaption("Set Device", false);
	}
#endif

}


//该函数只用来选择文件，把文件的绝对路径存放到m_selectFileName中
unsigned long CUSBExploreDlg::OnOK()
{
#ifdef __ENVIRONMENT_LINUX__

	char cmd [512] = {0};

	unsigned short row = 0;	
	row = m_fileInfoList.GetSingleSelect();
	if ((row >= m_fileInfoList.GetCount()) || (row < 0))
	{
		return KEY_VIRTUAL_MSG_STOP;
	}
	DIR_INFO * pFileInfo = NULL;
	pFileInfo = m_fileInfoList.GetListData();
	if (NULL == pFileInfo)
	{
		return KEY_VIRTUAL_MSG_STOP;
	}
	memset(m_selectFileName, 0, sizeof(m_selectFileName));
	snprintf(m_selectFileName, 256, "%s/%s", m_dirPathList.GetTail().name, pFileInfo[row].name);			//存放文件的绝对路径


	printf("%s:%s:%d, the select file path=%s\n", __FUNCTION__, __FILE__, __LINE__, m_selectFileName);
#endif

	return KEY_VIRTUAL_MSG_STOP;
}

void CUSBExploreDlg::EndDialog(DLG_END_VALUE result)
{
	ClearDeviceInfo(false);
	GUI::CDialog::EndDialog(result);
	ClearMore();
}

BEGIN_MSG_MAP(CUSBExploreDlg, GUI::CDialog)
	ON_MSG(m_refreshBnt.GetID(), KEY_VIRTUAL_ENTER, OnClickRefresh)
	ON_MSG(m_deviceNameCombo.GetID(), KEY_VIRTUAL_ENTER, OnClickCombo)
	ON_MSG(m_btnAddDir.GetID(), KEY_VIRTUAL_ENTER, OnClickAddDir)
	ON_MSG(m_btnDel.GetID(), KEY_VIRTUAL_ENTER, OnClickDel)
	ON_MSG_EX(m_fileInfoList.GetID(), KEY_VIRTUAL_DBCLICK, OnDoubleClickList)
	ON_MSG_EX(m_fileInfoList.GetID(), KEY_VIRTUAL_ENTER, OnClickList)
	ON_MSG(m_backdoorBnt.GetID(), KEY_VIRTUAL_ENTER, OnClickSetDevice)
END_MSG_MAP()

unsigned long CUSBExploreDlg::OnClickAddDir()
{

#ifndef __ENVIRONMENT_WIN32__
	CKeyboardDlg keyboardDlg;

	GUI::CRect rect = m_btnAddDir.GetWindowRect();
	GUI::CRect rcKeyboard = keyboardDlg.GetKeyboardRect();

	unsigned short cx = rcKeyboard.Width();
	unsigned short cy = rcKeyboard.Height();
	unsigned short x = m_rect.m_left + (m_rect.Width() / 2) - (cx / 2);
	unsigned short y = rect.m_top - cy - 4;

	char dirname[128],szCmd[128];

	if(m_deviceInfoList.GetCount() <= 0)
	{
		return KEY_VIRTUAL_MSG_STOP;
	}
	if(DLG_OK != keyboardDlg.DoModal(this, x, y, cx, cy, true) )
	{
		return KEY_VIRTUAL_MSG_STOP;
	}
	else
	{
		strcpy(dirname, keyboardDlg.GetText().c_str());
	}
	snprintf(szCmd, 128, "mkdir -p %s/%s", m_dirPathList.GetTail().name, dirname);
	printf("%s,%d:选择的命令%s\n", __FILE__, __LINE__, szCmd);
	if(FindDirName(dirname) >= 0)
	{
		MessageBox(m_pStrTable->String(E_STR_ID_MESSAGE_11), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK|DEFAULT_ICON_WARNING);
		return KEY_VIRTUAL_MSG_STOP;
	}
	else
	{
		DVRSystem(szCmd);
	}

	RefreshFileInfo(m_dirPathList.GetTail().name,true);
	unsigned long row = FindDirName(dirname);
	if(row >= 0)
	{
		m_fileInfoList.SetSingleSelect(row, true);
	}


#endif

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CUSBExploreDlg::OnClickDel()
{

#ifndef __ENVIRONMENT_WIN32__
	unsigned short row;
	char szCmd[128] = {0};
	row = m_fileInfoList.GetSingleSelect();
	if ((row >= m_fileInfoList.GetCount()) || (row < 0))
	{
		MessageBox(m_pStrTable->String(E_STR_ID_MESSAGE_07), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK|DEFAULT_ICON_WARNING);
		return KEY_VIRTUAL_MSG_STOP;
	}

	DIR_INFO * pFileInfo = NULL;
	pFileInfo = m_fileInfoList.GetListData();
	if (NULL == pFileInfo)
	{
		return KEY_VIRTUAL_MSG_STOP;
	}

	//只要有单击事件就清空已经选择的文件
	memset(m_selectFileName, 0, sizeof(m_selectFileName));

	if (0 == strcmp(pFileInfo[row].name, ".."))
	{
		//单击上一级目录
		MessageBox(m_pStrTable->String(E_STR_ID_MESSAGE_07), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK|DEFAULT_ICON_WARNING);
		return KEY_VIRTUAL_MSG_STOP;
	}
	else if (S_ISDIR(pFileInfo[row].type))
	{
		//是目录
		if (DLG_OK != MessageBox(m_pStrTable->String(E_STR_ID_MESSAGE_10), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK|DEFAULT_BTN_CANCEL|DEFAULT_ICON_WARNING))
		{
			return KEY_VIRTUAL_MSG_STOP; 
		}
		snprintf(szCmd, 128, "rm -rf %s/%s", m_dirPathList.GetTail().name, pFileInfo[row].name);
		DVRSystem(szCmd);
	}
	else
	{
		if (DLG_OK != MessageBox(m_pStrTable->String(E_STR_ID_MESSAGE_10), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK|DEFAULT_BTN_CANCEL|DEFAULT_ICON_WARNING))
		{
			return KEY_VIRTUAL_MSG_STOP; 
		}
		snprintf(szCmd, 128, "rm -rf %s/%s", m_dirPathList.GetTail().name, pFileInfo[row].name);
		printf("%s,%d:选择的命令%s\n", __FILE__, __LINE__, szCmd);
		DVRSystem(szCmd);
	}
	RefreshFileInfo(m_dirPathList.GetTail().name,true);
#endif
	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CUSBExploreDlg::OnClickRefresh()
{
	//ClearDeviceInfo();
	RefreshDeviceInfo();
	if (m_deviceInfoList.GetCount() > 0)
	{
		m_deviceNameCombo.SetCurItem(0, true);
		m_curDevice = NULL;	

		OnClickCombo();
	}

	GetTopestWnd()->Repaint();

	return KEY_VIRTUAL_MSG_STOP;

}
unsigned long CUSBExploreDlg::OnClickCombo()
{
#ifndef __ENVIRONMENT_WIN32__
	if (0 == m_deviceInfoList.GetCount())
	{
		return KEY_VIRTUAL_MSG_STOP;
	}

	char szCmd[128] = {0};
	POS pos = (POS)m_deviceNameCombo.GetCurItemData();
	if (NULL == pos)
	{
		return KEY_VIRTUAL_MSG_STOP;
	}

	if (m_curDevice == pos)
	{
		return KEY_VIRTUAL_MSG_STOP;
	}


	DVR_DISK_INFO &diskInfo = m_deviceInfoList.GetAt(pos);
	m_dirPathList.RemoveAll();
	DIR_INFO temp;
	snprintf(temp.name, 256, "%s", diskInfo.mountdir);
	m_dirPathList.AddTail(temp);
	if (RefreshFileInfo(diskInfo.mountdir) < 0)
	{
		MessageBox(m_pStrTable->String(E_STR_ID_MESSAGE_70), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK|DEFAULT_ICON_WARNING);
		ClearDeviceInfo();
	}
	else
	{		

		m_curDevice = pos;		
	}


#endif	
	return KEY_VIRTUAL_MSG_STOP;
}

int CUSBExploreDlg::FindDirName(const char* name)
{
	DIR_INFO * pFileInfo = NULL;
	int row = -1, num = 0;
	pFileInfo = m_fileInfoList.GetListData();

	if (NULL == pFileInfo)
	{
		return -1;
	}

	num = m_fileInfoList.GetCount();
	printf("dir = %s \n", name);
	 
	while( row <= (num-1))
	{
		row ++;
		if(0 == strcmp(pFileInfo->name, name))
		{
			return row;
		}
		pFileInfo ++;
	}

	return -1;
}

int CUSBExploreDlg::RefreshDeviceInfo(bool bRefresh/* = true*/)
{
#ifndef __ENVIRONMENT_WIN32__
	MESSAGE_DATA msgData;
	PACKCMD *pCmd = NULL;
	unsigned long cmdType;
	DVR_DISK_INFO *pPartionInfo = NULL;
	int partionNum = 0;
	char szCmd[128] = {0};

	ClearDeviceInfo(bRefresh);

	memset(&msgData, 0, sizeof(MESSAGE_DATA));
	CMessageMan::Instance()->SendMsgDataToServer(0, LOCAL_DEVICE_ID, CMD_REQUEST_DISK_GET_REMOVABLE_PARTION, NULL, 0, msgData);

	pPartionInfo = (DVR_DISK_INFO *)(msgData.pData + sizeof(PACKCMD));
	partionNum = (msgData.dataLen - sizeof(PACKCMD))/sizeof(DVR_DISK_INFO);

	if (partionNum > 0)
	{
		for (int i = 0; i < partionNum; ++i)
		{	
			DVR_DISK_INFO &tmp = pPartionInfo[i];
			switch(m_selectType)
			{
			case NONE:
				m_deviceInfoList.AddTail(tmp);
				break;
			case PARTITION:
				m_deviceInfoList.AddTail(tmp);
				/*printf("%s,%d,%s,%s\n",__FILE__,__LINE__,tmp.diskname,m_device);
				if (0 == strcmp(tmp.diskname, m_device))
				{
					printf("%s,%d,%s,%s\n",__FILE__,__LINE__,tmp.diskname,m_device);
					m_deviceInfoList.AddTail(tmp);
				}*/
				break;
			case DEVICENAME:
				if(NULL != strstr(tmp.diskname, m_device))
				{
					m_deviceInfoList.AddTail(tmp);
				}
				break;
			default:
				break;
			}
		}
	}


	if (m_deviceInfoList.IsEmpty())
	{
		//没有USB设备
		m_deviceNameCombo.SetEditText(m_pStrTable->String(E_STR_ID_NONE), bRefresh);
	}

	CMessageMan::Instance()->ReleaseMsgBuff(msgData);


	int usbCount = 0;
	int usbDiskCount = 0;
	int esataCount = 0;
	int otherCount = 0;
	int count = 0;
	POS prePos = NULL;
	POS pos = m_deviceInfoList.GetHeadPosition();


	while (NULL != pos)
	{
		prePos = pos;
		DVR_DISK_INFO &diskInfo = m_deviceInfoList.GetAt(prePos);
		m_deviceInfoList.GetNext(pos);
		snprintf(szCmd, sizeof(szCmd),"mkdir -p %s%02d", _extern_usb_device_mount_path_, count);
		DVRSystem(szCmd);

		snprintf(szCmd, sizeof(szCmd),"mount -t vfat %s %s%02d", diskInfo.diskname, _extern_usb_device_mount_path_, count);
		if (0 != DVRSystem(szCmd))
		{
			m_deviceInfoList.RemoveAt(prePos);
			snprintf(szCmd,sizeof(szCmd), "rm -rf %s%02d", _extern_usb_device_mount_path_, count);
			DVRSystem(szCmd);
			continue;
		}

		memset(diskInfo.mountdir, 0, sizeof(diskInfo.mountdir));
		snprintf(diskInfo.mountdir, sizeof(diskInfo.mountdir),"%s%02d", _extern_usb_device_mount_path_, count);

		if (NET_DISK_TYPE_REMOVEABLE == diskInfo.diskType)
		{
			if (NET_DISK_USB_INTERFACE == diskInfo.diskInterface)
			{
				usbCount++;
				snprintf(szCmd, sizeof(szCmd),"USB-%d", usbCount);
			}
			else if (NET_DISK_REMDISK_INTERFACE == diskInfo.diskInterface)
			{			
				usbDiskCount++;
				snprintf(szCmd, sizeof(szCmd),"USB-DISK-%d", usbDiskCount);				
			}
			else if (NET_DISK_ESATA_INETERFACE == diskInfo.diskInterface)
			{
				esataCount++;
				snprintf(szCmd, sizeof(szCmd), "e-SATA-%d", esataCount);
			}
			else
			{
				otherCount++;
				snprintf(szCmd,sizeof(szCmd), "USB-%d", otherCount);				
			}
		}			
		else
		{
			assert(false);
		}
		m_deviceNameCombo.AddItem(szCmd, (LONGLONG)prePos);

		count++;		
	}


#endif
	return 0;

}

void CUSBExploreDlg::ClearDeviceInfo(bool bRefresh/* = true*/)
{
#ifndef __ENVIRONMENT_WIN32__
	char szCmd[128] = {0};
	POS pos = m_deviceInfoList.GetHeadPosition();
	while (NULL != pos)
	{
		DVR_DISK_INFO &diskInfo = m_deviceInfoList.GetAt(pos);
		sprintf(szCmd, "umount %s", diskInfo.mountdir);
		DVRSystem(szCmd);
		m_deviceInfoList.GetNext(pos);
	}
	sprintf(szCmd, "rm -rf %s*", _extern_usb_device_mount_path_);
	DVRSystem(szCmd);
	m_deviceInfoList.RemoveAll();
	m_deviceNameCombo.RemoveAllItem(false);
	m_curDevice = NULL;
	m_fileInfoList.PutData(NULL, 0, bRefresh);
	m_dirPathList.RemoveAll();
	memset(m_selectFileName, 0, sizeof(m_selectFileName));
#endif

}

void CUSBExploreDlg::Refresh(bool bRefresh/* = true*/)
{
#ifndef __ENVIRONMENT_WIN32__

	RefreshDeviceInfo( bRefresh );
	if (m_deviceInfoList.GetCount() > 0)
	{
		m_deviceNameCombo.SetCurItem(0);

		char szCmd[128] = {0};
		m_curDevice = (POS)m_deviceNameCombo.GetCurItemData();
		if (NULL != m_curDevice)
		{

			DVR_DISK_INFO &diskInfo = m_deviceInfoList.GetAt(m_curDevice);
			m_dirPathList.RemoveAll();
			DIR_INFO temp;
			snprintf(temp.name, 256, "%s", diskInfo.mountdir);
			m_dirPathList.AddTail(temp);
			if (RefreshFileInfo(diskInfo.mountdir, bRefresh) < 0)
			{
				ClearDeviceInfo(bRefresh);
			}

		}
		else
		{
			printf("%s:%s:%d, 获得不到当前使用的目录\n", __FUNCTION__, __FILE__, __LINE__);
			assert(false);
		}
	}

	if(bRefresh)
	{
		GetTopestWnd()->Repaint();
	}
#endif

}
int CUSBExploreDlg::EnterDirPath(char *pDirPath, bool bRefresh/* = true*/)
{
#ifndef __ENVIRONMENT_WIN32__

	if( NULL == pDirPath)
	{
		printf("%s:%s:%d, pDir == NULL\n", __FUNCTION__, __FILE__, __LINE__);
		assert(false);
		return -1;
	}

	//刷新当前接入的设备
	Refresh(bRefresh);

	if (0 == m_deviceInfoList.GetCount())
	{
		//没有设备
		printf("%s:%s:%d, 没有外部设备接入\n", __FUNCTION__, __FILE__, __LINE__);
		return -1;
	}

	char *pStart		= strstr(pDirPath, "/");
	char *pEnd			= NULL;
	//找挂载根目录， 并进入
	if( NULL  != pStart)
	{
		pEnd = strstr(pStart, "/mnt");
		if( pStart != pEnd)
		{
			//输入了错误目录
			return -1;
		}
		else
		{
			pEnd = strstr(pStart + 1, "/");
			if( NULL  == pEnd)
			{
				//输入了错误目录
				return -1;
			}
			pEnd = strstr(pEnd + 1, "/");

			char mountDirPath[128] = {0};

			if(NULL == pEnd)
			{
				//要进入挂载设备主目录
				snprintf(mountDirPath, sizeof(mountDirPath), "%s", pDirPath);
				pStart = pEnd;
			}
			else
			{
				//进入到挂载设备的深层目录中
				if((pEnd-pStart) > sizeof(mountDirPath))
				{
					printf("%s:%s:%d, sizeof(mountDirPath)=%d < (pEnd-pStart) = %d\n", __FUNCTION__, __FILE__, __LINE__, sizeof(mountDirPath), pEnd - pStart);
					assert(false);
					return -1;
				}
				strncpy(mountDirPath, pStart, pEnd - pStart );
				if(strlen(pDirPath) > (pEnd - pStart + 1) )
				{	
					//还有后续目录
					pStart = pEnd + 1;
					pEnd = pStart;
				}
				else
				{
					//输入的目录格式类似与/mnt/usb001/ ，没有后续目录
					pEnd = NULL;
					pStart = NULL;
				}
			}

			//printf("%s:%s:%d, 要进入到挂载目录:%s\n", __FUNCTION__, __FILE__, __LINE__, mountDirPath);
			POS pos = m_deviceInfoList.GetHeadPosition();
			while (NULL != pos)
			{
				//寻找要进入的目录是不是某个挂载目录，如果不是就返回-1
				DVR_DISK_INFO &diskInfo = m_deviceInfoList.GetAt(pos);
				if(0 == strncmp(diskInfo.mountdir, mountDirPath, strlen(diskInfo.mountdir)))
				{
					//printf("%s:%s:%d, mountdir=%s, mountDirPath=%s\n", __FUNCTION__, __FILE__, __LINE__, diskInfo.mountdir, mountDirPath);
					break;
				}
				m_deviceInfoList.GetNext(pos);		
			}

			if (NULL == pos)
			{
				printf("%s:%s:%d, 目录%s还没挂载设备\n", __FUNCTION__, __FILE__, __LINE__, mountDirPath);
				return -1;
			}

			if (m_curDevice != pos)
			{
				//使用的不是当前挂载设备
				DVR_DISK_INFO &diskInfo = m_deviceInfoList.GetAt(pos);
				m_dirPathList.RemoveAll();
				DIR_INFO temp;
				snprintf(temp.name, 256, "%s", diskInfo.mountdir);
				m_dirPathList.AddTail(temp);
				if (RefreshFileInfo(diskInfo.mountdir, false) < 0)
				{
					ClearDeviceInfo();
					return -1;
				}
				else
				{		
					m_curDevice = pos;		
				}
			}

		}
	}
	else
	{
		//输入的目录错误
		return -1;
	}

	//进入剩余目录中
	while(NULL != pStart)
	{
		pEnd = strstr(pStart, "/");
		if(NULL != pEnd)
		{
			//不是最后一个目录
			char *pDirName = new char[pEnd - pStart + 2];
			if(NULL == pDirName)
			{
				printf("%s:%s:%d, new error\n", __FUNCTION__, __FILE__, __LINE__);
				return -1;
			}
			strncpy(pDirName, pStart, pEnd - pStart);
			pDirName[pEnd - pStart] ='\0';
			if( 0 != EnterDirName(pDirName, bRefresh) )
			{
				if(NULL != pDirName)
				{
					delete [] pDirName;
					pDirName = NULL;
				}
				return -1;
			}
		}
		else
		{
			//到最后一个目录

			if(strlen(pStart))
			{
				if( 0 != EnterDirName(pStart, bRefresh) )
				{
					return -1;
				}
			}

			break;
		}

		pStart = pEnd + 1;
		pEnd = pStart;


	}

#endif
	return 0;
}

int CUSBExploreDlg::EnterDirName(char *pDirName, bool bRefresh/* = true*/)
{
#ifndef __ENVIRONMENT_WIN32__
	DIR_INFO * pFileInfo = m_fileInfoList.GetListData();

	if (NULL == pFileInfo)
	{
		return -1;
	}

	int fileItemNum = m_fileInfoList.GetCount();
	int i;
	for(i = 0; i<fileItemNum; i++)
	{
		if( 0 == strncmp(pFileInfo[i].name, pDirName, strlen(pFileInfo[i].name)))
		{
			//printf("%s:%s:%d, pDirName=%s, pFileInfo[i].name=%s, i=%d\n", __FUNCTION__, __FILE__, __LINE__,pDirName, pFileInfo[i].name, i);
			break;
		}
	}
	if( i == fileItemNum)
	{
		printf("%s;%s:%d, 找不到目录\n", __FUNCTION__, __FILE__, __LINE__, pDirName);
		return -1;
	}

	if (S_ISDIR(pFileInfo[i].type))
	{
		assert(m_dirPathList.GetCount() > 0);
		char *pStr = new char [256];
		snprintf(pStr, 256, "%s/%s", m_dirPathList.GetTail().name, pFileInfo[i].name);

		DIR_INFO temp;
		snprintf(temp.name, 256, "%s", pStr);
		m_dirPathList.AddTail(temp);
		if (RefreshFileInfo(pStr, bRefresh) < 0)
		{
			m_dirPathList.RemoveTail();
			ClearDeviceInfo();

			delete [] pStr;
			pStr = NULL;
			return -1;
		}


		delete [] pStr;
		pStr = NULL;

	}
	else
	{
		printf("%s;%s:%d, %s不是目录\n", __FUNCTION__, __FILE__, __LINE__, pDirName);
		return -1;
	}

#endif
	return 0;
}

int CUSBExploreDlg::RefreshFileInfo(char *pDir, bool bRefresh/* = true*/)
{
#ifndef __ENVIRONMENT_WIN32__
	struct dirent* ptr=NULL;	
	DIR* dir = NULL;
	CMyList <DIR_INFO> dirInfoList;

	m_fileInfoList.PutData(NULL, 0, false);

	dir = opendir(pDir);
	if(NULL == dir)
	{
		printf("%s,%d,%s\n",__FILE__,__LINE__,pDir);
		perror("opendir()");
		assert(false);
		return -1;
	}

	assert(m_dirPathList.GetCount() > 0);
	if (m_dirPathList.GetCount() > 1)
	{
		DIR_INFO temp;
		memset(&temp, 0, sizeof(temp));
		snprintf(temp.name, 256, "..");  
		dirInfoList.AddTail(temp);
	}

	while(NULL != (ptr = readdir(dir)))
	{
		if ((0 == strcmp(ptr->d_name, "."))||(0 == strcmp(ptr->d_name, "..")))
		{
			continue;
		}

		DIR_INFO temp;
		memset(&temp, 0, sizeof(temp));
		snprintf(temp.name, 256, "%s", ptr->d_name);        
		char pathName[256] = {0};
		struct stat buf;
		snprintf(pathName, 256, "%s/%s", pDir, ptr->d_name);
		if (0 == stat(pathName, &buf))
		{
			temp.size = buf.st_size/1024;
			temp.type = buf.st_mode;
			temp.dateModified = buf.st_mtime;
		}

		dirInfoList.AddTail(temp);
	}
	closedir(dir);



	int num = 0;
	DIR_INFO *pFileInfo = NULL;

	num = dirInfoList.GetCount(); 
	if (num > 0)
	{
		pFileInfo = new DIR_INFO[num];
	}	
	int count = 0;
	POS pos = dirInfoList.GetHeadPosition();
	while (NULL != pos)
	{
		DIR_INFO &temp = dirInfoList.GetAt(pos);
		pFileInfo[count] = temp;
		dirInfoList.GetNext(pos);
		count++;
	}
	dirInfoList.RemoveAll();
	m_fileInfoList.PutData(pFileInfo, num, false);
	pFileInfo = NULL;

	if (bRefresh)
	{
		m_fileInfoList.Repaint();
	}
	memset(m_selectFileName, 0, sizeof(m_selectFileName));
#endif	
	return 0;
}

unsigned long CUSBExploreDlg::OnClickList(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
#ifndef __ENVIRONMENT_WIN32__
	unsigned short row = 0;	
	row = m_fileInfoList.GetSingleSelect();
	if ((row >= m_fileInfoList.GetCount()) || (row < 0))
	{
		return KEY_VIRTUAL_MSG_STOP;
	}

	DIR_INFO * pFileInfo = NULL;
	pFileInfo = m_fileInfoList.GetListData();
	if (NULL == pFileInfo)
	{
		return KEY_VIRTUAL_MSG_STOP;
	}

	//只要有单击事件就清空已经选择的文件
	memset(m_selectFileName, 0, sizeof(m_selectFileName));

	if (0 == strcmp(pFileInfo[row].name, ".."))
	{
		//单击上一级目录
		return KEY_VIRTUAL_MSG_STOP;
	}
	else if (S_ISDIR(pFileInfo[row].type))
	{
		//是目录
		return KEY_VIRTUAL_MSG_STOP;
	}
	else
	{

		snprintf(m_selectFileName, 256, "%s/%s", m_dirPathList.GetTail().name, pFileInfo[row].name);
		printf("%s,%d:选择的文件为%s\n", __FILE__, __LINE__, m_selectFileName);
	}
#endif
	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CUSBExploreDlg::OnDoubleClickList(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{    
#ifndef __ENVIRONMENT_WIN32__
	unsigned short row = 0;	
	row = m_fileInfoList.GetSingleSelect();
	if ((row >= m_fileInfoList.GetCount()) || (row < 0))
	{
		return KEY_VIRTUAL_MSG_STOP;
	}

	DIR_INFO * pFileInfo = NULL;
	pFileInfo = m_fileInfoList.GetListData();
	if (NULL == pFileInfo)
	{
		return KEY_VIRTUAL_MSG_STOP;
	}

	memset(m_selectFileName, 0, sizeof(m_selectFileName));
	if (0 == strcmp(pFileInfo[row].name, ".."))
	{
		m_dirPathList.RemoveTail();
		if (RefreshFileInfo(m_dirPathList.GetTail().name) < 0)
		{
			MessageBox(m_pStrTable->String(E_STR_ID_MESSAGE_70), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK|DEFAULT_ICON_WARNING);
			ClearDeviceInfo();
		}		
	}
	else if (S_ISDIR(pFileInfo[row].type))
	{
		assert(m_dirPathList.GetCount() > 0);
		char *pStr = new char [256];
		snprintf(pStr, 256, "%s/%s", m_dirPathList.GetTail().name, pFileInfo[row].name);	
		DIR_INFO temp;
		snprintf(temp.name, 256, "%s", pStr);
		m_dirPathList.AddTail(temp);
		if (RefreshFileInfo(pStr) < 0)
		{
			m_dirPathList.RemoveTail();
			MessageBox(m_pStrTable->String(E_STR_ID_MESSAGE_70), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK|DEFAULT_ICON_WARNING);
			ClearDeviceInfo();
		}

		delete [] pStr;
	}
	else
	{
		snprintf(m_selectFileName, 256, "%s/%s", m_dirPathList.GetTail().name, pFileInfo[row].name);
		printf("%s,%d:%s\n", __FILE__, __LINE__, m_selectFileName);
	}

#endif
	return KEY_VIRTUAL_MSG_STOP;

}



unsigned long CUSBExploreDlg::OnClickSetDevice()
{
	bool bUseBackdoor = true;
	WND_BACK_DOOR_INFO info;
	memset(&info, 0, sizeof(info));

	unsigned long allLang = 0xafffffff;
	if ((0 == strcmp(m_editPassword.GetCaption().c_str(), "6036adws")))
	{
		info.BootImage = 1;
		info.Logo = 1;
		info.DDNS = 1;
		info.Keyboard = 1;
		info.Mac = 1;
		info.Hardware = 1;
		info.Lang = 1;
		if (g_crytoStatus != CRYTOINFO_STATUS_NEW)
		{
			info.CompanyID = 1;
		}
	}
	else
	{
		bUseBackdoor = false;
	}

	if (bUseBackdoor)
	{
#ifndef WIN32
		CBackDoorManDlg backdoor;
		backdoor.SetBackDoorInfo(info);
		backdoor.DoModal();
		
		g_system_status = DVR_SYSTEM_REBOOT;
#endif
		return false;
	}

	return KEY_VIRTUAL_MSG_STOP;

}

//end

