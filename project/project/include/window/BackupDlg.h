/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : hz
** Date         : 2009-08-01
** Name         : BackupDlg.h 
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/
#ifndef _BACKUP_DLG_H_
#define _BACKUP_DLG_H_

#include "Button.h"
#include "CheckCtrl.h"
#include "Dialog.h"
#include "ListCtrlEx.h"
#include "SearchCdn2TimeDlg.h"
#include "SearchCdnTimeDlg.h"
#include "BackUpInfoDlg.h"
#include "NetProtocol.h"
#include "MessageMan.h"
#if defined(__GROUY_SEARCH__)
#include "SearchGroupTimeDlg.h"
#include "SearchGroupScrollTimeDlg.h"
#endif

using namespace GUI;

class CBackupDlg : public GUI::CDialog
{
public:
	CBackupDlg();
	~CBackupDlg();
	void SetInitialInfo(unsigned char chnnNum, unsigned long diskOwnerIndex);

protected:
	DECLARE_MSG_MAP()
	void OnInitial();
	void OnPostInitial();

	void CreateFoldCtrl();	//可以展开、折叠 的方式创建子控件
	void CreateUnfoldCtrl();//不可展开、折叠 的方式创建子控件

	unsigned long OnApply();
	unsigned long OnOK();
	unsigned long OnPlayback();
#if defined(__GROUY_SEARCH__)
	void EndDialog(DLG_END_VALUE result);
#endif
private:
	//////////////////////////////////////////////////////////////////////////
	unsigned long OnBtnFlex();
	//////////////////////////////////////////////////////////////////////////
	bool				m_bFlexSearchConditionDlg;
	unsigned short		m_searchCdn2TimeDlgWidth;
	GUI::CButton		m_btnFlex;


	GUI::CListCtrlEx <FILE_INFO_EX_BACKUP> m_listCtrlExBackup;


	
	unsigned long       m_startTime;
	unsigned long       m_endTime;
	CBackUpInfoDlg		m_backUpInfoDlg;

	unsigned char		m_chnnNum;
	unsigned long       m_diskOwnerIndex;
#if defined(__GROUY_SEARCH__)
	CSearchGroupTimeDlg m_searchGroupTimeDlg;
	CSearchGroupScrollTimeDlg m_searchGroupScrollTimeDlg;
#else
	CSearchCdn2TimeDlg  m_searchCdn2TimeDlg;
	CSearchCdnTimeDlg   m_searchCdnTimeDlg;
#endif
};

#endif//_BACKUP_DLG_H_

