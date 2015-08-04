/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : L
** Date         : 2010-12-20
** Name         : DiskAdvDlg.h
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/

#ifndef _DISK_ADV_DLG_H_
#define _DISK_ADV_DLG_H_

#include "Dialog.h"
#include "TabCtrl.h"
#include "ComboCtrl.h"
#include "Static.h"
#include "ListCtrl.h"
#include "DiskManDlg.h"
#include "MessageMan.h"


class CDiskAdvDlg : public GUI::CDialog
{
public:
	CDiskAdvDlg();
	virtual ~CDiskAdvDlg();

	void SetFixedDiskList(DVR_DISK_INFO_LIST *pList);

	void RefreshComboInfo(bool bRefresh);

protected:
	void OnInitial();
	void OnPostInitial();

	void RefreshListInfo(bool bRefresh);
	void GetIDName(unsigned char id, char *pStr, int len);

	DECLARE_MSG_MAP()
	unsigned long OnClickCombo();

	

	GUI::CComboCtrl		m_comboCtrl;
	GUI::CListCtrl		m_listCtrl;

	GUI::CStatic		m_scTitle[4];
	GUI::CStatic		m_scInfo[4];

	GUI::CStatic        m_scSmTitle;

	DVR_DISK_INFO_LIST   *m_pFixedDiskInfoList;
};


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////


class CDiskTableManDlg : public GUI::CDialog
{
public:
	CDiskTableManDlg();
	virtual ~CDiskTableManDlg();

	void EnableExitBtn(bool bEnable, bool bRefresh);

protected:
	void OnInitial();
	unsigned long OnPreCommand(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);

	void EndDialog(DLG_END_VALUE result);

	GUI::CTabCtrl	m_tabCtrl;

	CDiskManDlg	m_diskManDlg;
	CDiskAdvDlg m_diskAdvDlg;

	DVR_DISK_INFO_LIST m_fixedDiskInfoList;

	enum PAGE_ITEM
	{
		PAGE_DISKMAN,
		PAGE_DISKADV,
	};
};

#endif

//end
