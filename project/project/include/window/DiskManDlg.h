/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : L
** Date         : 2009-08-19
** Name         : DiskManDlg.h
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/

#ifndef _DISK_MAN_DLG_H_
#define _DISK_MAN_DLG_H_

#include "Dialog.h"
#include "ListCtrl.h"
#include "ProgressDlg.h"
#include "PUB_common.h"
#include "USBExploreDlg.h"
#include "DiskInfoDlg.h"

using namespace GUI;


class CDiskManDlg : public GUI::CDialog
{
public:
	CDiskManDlg();
	~CDiskManDlg();

	//static PUB_THREAD_RESULT PUB_THREAD_CALL FormatThread( void* pParam );

	//void FormatProc();

	void SetPercent( int percet );
	void EndProgress();

	bool IsFormating() {return m_bFormating;}

	void EndDialog(DLG_END_VALUE result);

	void SetFixedDiskList(DVR_DISK_INFO_LIST *pList);
protected:
	void OnInitial();
	void OnPostInitial();

	void OnIdle();

#ifdef __DVR_PROFESSIONAL__
	unsigned long OnApply();
#endif
	unsigned long OnOK();
    void RefreshDiskInfo(bool bRefresh = true);

protected:
	DECLARE_MSG_MAP()
	unsigned long OnClickBrowse();
	unsigned long OnClickList(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);
	unsigned long OnRefresh();
	unsigned long OnCkEsata();
	void EnableApplayBtn(bool bEnable, bool bRefresh);
	void EnableFormatBtn(bool bEnable, bool bRefresh);
	void EnableRefreshBtn(bool bEnable, bool bRefresh);
    void EnableExitBtn(bool bEnable, bool bRefresh);
	void EnableBrowseBtn(bool bEnable, bool bRefresh);

	unsigned long OnBtnFormat();
	unsigned long OnBtnApply();

protected:
	GUI::CListCtrl			m_diskManList;
	CProgressDlg			m_progressDlg;
	GUI::CStatic            m_stcMsg;
	GUI::CButton            m_bntRefresh;
	GUI::CButton			m_btnBrowse;
	GUI::CCheckCtrl         m_bEsataCk;
	//CUSBExploreDlg			m_usbExploreDlg;
	//CDiskInfoDlg			m_diskInfoDlg;

	GUI::CButton			m_btnApplay;
	GUI::CButton			m_btnFormat;

protected:
	bool							m_prgShow;
	//bool							m_bFormat;
	//PUB_thread_t					m_hFormatThread;

	DVR_DISK_INFO_LIST     m_diskInfoList;
	POS                   *m_listRowToDiskInfo;
	int                    m_curSelectRow;

	bool                 m_bFormating;
	int                  m_diplayPrecent;

	int					m_finishDiskNum;  //已完成格式化的个数

	DVR_DISK_INFO_LIST   *m_pFixedDiskInfoList;
	enum
	{
		DISK_READ_WRITE = 0,
		DISK_READ_ONLY,
		DISK_REDUNDANCY,
	};

};


#endif


