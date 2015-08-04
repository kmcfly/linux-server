/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : L
** Date         : 2010-11-04
** Name         : DiskInfoDlg.h
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/

#ifndef _DISK_INFO_DLG_H_
#define _DISK_INFO_DLG_H_

#include "Dialog.h"
#include "ListCtrl.h"

using namespace GUI;

class CDiskInfoDlg : public GUI::CDialog
{
public:
	CDiskInfoDlg();
	~CDiskInfoDlg();
	void SetDiskInfo(int row, DVR_DISK_INFO diskInfo){m_seq = row; m_diskInfo = diskInfo;};

protected:
	void OnInitial();
	void OnPostInitial();
	DECLARE_MSG_MAP()
	unsigned long OnClickList(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);
	

protected:
	GUI::CListCtrl		m_diskInfoList;
	DVR_DISK_INFO		m_diskInfo;
	int					m_seq;

	enum LIST_ROW
	{
		ROW_DISK_ID,
		ROW_SATA_TYPE,
		ROW_TOTAL_SPACE,
		ROW_FREE_SPACE,
		ROW_DISK_STATUS,
		ROW_DISK_ATTR,
		ROW_DISK_SRC,
		ROW_PIC_INFO = 9,
		ROW_PIC_SPACE ,
		ROW_EXIST_PIC,
		ROW_PIC_CLEAR,
	};

};

#endif
