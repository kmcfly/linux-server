/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : L
** Date         : 2010-03-01
** Name         : WzdTipDiskDlg.h
** Version      : 1.0
** Description  : �����������ʾ
** Modify Record:
** ҳ�����ã���ʾ������ص���Ϣ
***********************************************************************/

#ifndef _WZD_TIP_DISK_DLG_H_
#define _WZD_TIP_DISK_DLG_H_

#include "Static.h"
#include "RichTextCtrl.h"
#include "WzdPageDlg.h"

using namespace GUI;

class CWzdTipDiskDlg : public CWzdPageDlg
{
public:
	CWzdTipDiskDlg();
	~CWzdTipDiskDlg();

	void SetTipDisk(const std::string& str);
	void AppendTipDisk(const std::string& str);

protected:
	void OnInitial();

protected:
	GUI::CRichTextCtrl m_richTextCtrl;
};

#endif
