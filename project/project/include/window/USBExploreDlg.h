/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : ZHL
** Date         : 2009-08-19
** Name         : USBExploreDlg.h
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/
#ifndef _USB_EXPLORE_DLG_H_
#define _USB_EXPLORE_DLG_H_

#ifndef __ENVIRONMENT_WIN32__
#include <sys/stat.h>
#include <dirent.h>
#endif

#include "Dialog.h"
#include "Static.h"
#include "MessageBox.h"
#include "ProgressDlg.h"
#include "PUB_common.h"
#include "ComboCtrl.h"
#include "mylist.cpp"
#include "ListCtrlEx.h"
#include "KeyboardDlg.h"
const char _extern_usb_device_mount_path_[]="/mnt/usb";
const char _extern_usbDisk_device_mount_path_[]="/mnt/usbDisk";

typedef struct _dir_info 
{	
	char name[256];
	unsigned long size;
	unsigned short type;
	time_t dateModified;		
}DIR_INFO;

enum DLG_STYLE{
	CONFIG_MAN_DLG,
	BACKUP_INFO_DLG,
	EXPORT_LOG_LIST_DLG,
	LOG_INFO_DLG,
	SYSTEM_UPGRADE_DLG,
	DISK_BROWSE_DLG,
	DEFUALT_DLG
};

using namespace GUI;

class CUSBExploreDlg : public GUI::CDialog
{

public:
	enum SELECT_TYPE
	{
		NONE,
		PARTITION,
		DEVICENAME,
	};

public:
	CUSBExploreDlg();
	~CUSBExploreDlg();
	void SetCurDevice(int selectType, char* pDevice);
	void SetStyle(DLG_STYLE style){m_style = style;};
	void SetCaptionName(const char *pCaption);
protected:
	void OnInitial();
	
	unsigned long OnOK();
	
	void EndDialog(DLG_END_VALUE result);

	int RefreshDeviceInfo(bool bRefresh = true);
	int RefreshFileInfo(char *pDir, bool bRefresh = true);
	void ClearDeviceInfo(bool bRefresh = true);
	void Refresh(bool bRefresh = true);
	int EnterDirPath(char *pDirPath, bool bRefresh = true);
	int EnterDirName(char *pDirName, bool bRefresh = true);
	int FindDirName(const char* name);

	virtual void ClearMore(){;}
protected:
	DECLARE_MSG_MAP()
	unsigned long OnClickCombo();
	unsigned long OnClickList(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);
	unsigned long OnDoubleClickList(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);
	unsigned long OnClickRefresh();
	unsigned long OnClickAddDir();
	unsigned long OnClickDel();
	unsigned long OnClickSetDevice();
protected:
	GUI::CComboCtrl     m_deviceNameCombo;
	GUI::CButton        m_refreshBnt;
	GUI::CButton		m_btnAddDir;
	GUI::CButton		m_btnDel;
	DVR_DISK_INFO_LIST  m_deviceInfoList;
	POS                 m_curDevice;
	int					m_selectType;
	char				m_device[128];

	GUI::CListCtrlEx <DIR_INFO> m_fileInfoList;
	CMyList <DIR_INFO> m_dirPathList;
	char                m_selectFileName[256];
	DLG_STYLE			m_style;


	char                m_captionName[128];
	GUI::CEditCtrl      m_editPassword;
	GUI::CButton        m_backdoorBnt;
};


#endif //_USB_EXPLORE_DLG_H_
