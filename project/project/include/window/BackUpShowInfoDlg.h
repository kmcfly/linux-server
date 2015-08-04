/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : zxx
** Date         : 2011-12-08
** Name         : BackUpShowInfoDlg.h 
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/
#ifndef _BACKUP_SHOWINFO_DLG_H_
#define _BACKUP_SHOWINFO_DLG_H_
#include "Dialog.h"
#include "ListCtrl.h"
#include "ProgressDlg.h"
#include "MessageMan.h"
#include "USBExploreDlg.h"
#include "Static.h"
#include "ComboCtrl.h"
#include "Button.h"
using namespace GUI;

class CBackUpShowInfoDlg: public GUI::CDialog
{
public:
	CBackUpShowInfoDlg();
	~CBackUpShowInfoDlg();
	void SetInitialInfo(DVR_DISK_INFO_LIST *pDiskInfoList, FILE_INFO_EX_BACKUP *pDataBackup, unsigned long fileNum, unsigned long allSize);
protected:
	DECLARE_MSG_MAP()
	void OnInitial();
	void OnPostInitial();
	unsigned long OnApply();
	unsigned long OnCancel();
	void OnIdle();
	void OnDestroy();
	void EndDialog(DLG_END_VALUE result);
	unsigned long OnClickList(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);
private:
	void SetPercent( int percet );
	void EndProgress();
	void SetButtonStatus(bool bBackuping, DISK_TYPE diskType = NET_DISK_TYPE_REMOVEABLE);
protected:
	GUI::CListCtrl			m_backUpInfoList;//显示上一个页面传过来的东西
	GUI::CListCtrl			m_backUpOperationList;//本页面操作列表
	CProgressDlg			m_progressDlg;
private:
	DVR_DISK_INFO_LIST      *m_pDiskInfoList;

	enum OPER_LIST_ROW
	{
		ROW_FILE_SIZE,
		ROW_STORAGE_TYPE,
		ROW_FREE_SIZE,
		ROW_DISK_CLEAN,
	};

	enum LIST_COL
	{
		COL_SEQ,
		COL_CHNN,
		COL_STARTTIME,
		COL_ENDTIME,
	};

	FILE_INFO_EX_BACKUP    *m_pFileInfoBackup;
	int m_fileNum;
	POS  m_curDevicePos;
	bool m_bBackupIng;
	int  m_precent;
	int  m_displayPrecent;
	unsigned long  m_needSpace;
	bool					m_prgShow;
	PUB_thread_t			m_hBackUpThread;
	char                    *m_pDiskName;
	char                    *m_pMountDir;
private:
	void OnClickDiskCleanUp();
};
#endif
