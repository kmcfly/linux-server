/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : L
** Date         : 2009-08-25
** Name         : BackUpInfoDlg.h 
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/

#ifndef _BACK_UP_INFO_H_
#define _BACK_UP_INFO_H_

#include "Dialog.h"
#include "ListCtrl.h"
#include "ProgressDlg.h"
#include "PUB_common.h"
#include "mylist.cpp"
#include "NetProtocol.h"
#include "MessageMan.h"
#include "USBExploreDlg.h"


typedef CMyList<DVR_DISK_INFO> DVR_DISK_INFO_LIST;

using namespace GUI;

class CBackUpInfoDlg : public GUI::CDialog
{
public:
	CBackUpInfoDlg();
	~CBackUpInfoDlg();
	void SetInitialInfo(DVR_DISK_INFO_LIST *pDiskInfoList, FILE_INFO_EX_BACKUP *pDataBackup, unsigned long fileNum, unsigned long startTime, unsigned long endTime,unsigned long  diskOwnerby);

protected:
	void OnInitial();
	void OnPostInitial();
	unsigned long OnOK();
	unsigned long OnClickDiskMan();
	unsigned long OnCancel();
	void OnIdle();
	void OnDestroy();
    void EndDialog(DLG_END_VALUE result);

protected:
	DECLARE_MSG_MAP()
	unsigned long OnClickList(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);

private:
	void SetPercent( int percet );
	void EndProgress();
	void SetButtonStatus(bool bBackuping, DISK_TYPE diskType = NET_DISK_TYPE_REMOVEABLE); 
	unsigned long CalNeedSpaceSize(unsigned long fileNum, DISK_TYPE deviceType, BACKUP_DATA_TYPE dataSaveType, bool bHavePlayer);

protected:
	GUI::CListCtrl			m_backUpInfoList;
	GUI::CButton			m_diskManBtn;
	CProgressDlg			m_progressDlg;

protected:

	enum LIST_ROW
	{
		ROW_START_TIME = 0,
		ROW_END_TIME = 1,
		ROW_FILE_NUM = 2,
		ROW_FILE_SIZE = 3,
		ROW_STORAGE_TYPE = 4,
		ROW_FREE_SIZE = 5,
		ROW_PLAYER = 6,
		ROW_BACKUP_TYPE = 7,
	};

private:
	bool					m_prgShow;
	PUB_thread_t			m_hBackUpThread;

	DVR_DISK_INFO_LIST      *m_pDiskInfoList;
	unsigned long           m_startTime;
	unsigned long           m_endTime;
	FILE_INFO_EX_BACKUP    *m_pFileInfoBackup;
	unsigned long           m_fileNum;
	unsigned long           m_needSpace;
	POS                     m_curDevicePos;
	bool                    m_bHavePlayer;
	BACKUP_DATA_TYPE        m_dataSaveType;



	char                    *m_pDiskName;
	char                    *m_pMountDir;
	bool                    m_bBackupIng;
	int                     m_precent;
	int                     m_displayPrecent;
	unsigned long           m_diskOwnerby;
}; 



#endif
