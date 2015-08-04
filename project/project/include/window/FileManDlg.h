/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : ysw
** Date         : 2009-08-01
** Name         : FileManDlg.h
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/
#ifndef _FILE_MAN_DLG_H_
#define _FILE_MAN_DLG_H_
#include "Button.h"
#include "Dialog.h"
#include "ListCtrlEx.h"
#include "MessageMan.h"

using namespace GUI;

class CFileManDlg : public GUI::CDialog
{
public:
	CFileManDlg();
	~CFileManDlg();
	void SetInitialInfo(unsigned long channelNum, unsigned long diskOwnerIndex);

	void OnSearch(unsigned long startTime, unsigned long endTime, ULONGLONG chnnBits, unsigned long dataOwnerby);
	unsigned long GetPlaybackInfo(FILE_INFO_EX & fileInfo);
protected:
	DECLARE_MSG_MAP()
	void OnInitial();
    void EndDialog(DLG_END_VALUE result);
	unsigned long OnClickLock();
	unsigned long OnClickDel();
	unsigned long OnClickList(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);
private:
	GUI::CButton m_btnDelete;
	GUI::CButton m_btnLock;
	GUI::CListCtrlEx <FILE_INFO_EX> m_listCtrlEx;

	unsigned long m_channelNum;
	unsigned long m_diskOwnerIndex;

	FILE_INFO_EX  m_selFileInfo;
	NET_DATA_SEARCH m_curSearchCond;
};
#endif //_FILE_MAN_DLG_H_
