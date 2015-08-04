/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : ZXX
** Date         : 2012-09-17
** Name         : WzdUS02PageDlg.h
** Version      : 1.0
** Description  : �򵼶Ի���
** Modify Record:
***********************************************************************/

#ifndef _WZD_US02_PAGE_DLG_H_
#define _WZD_US02_PAGE_DLG_H_

#include "Dialog.h"
#include "WndStructDef.h"

using namespace GUI;

class CWzdUS02PageDlg : public GUI::CDialog
{
public:
	CWzdUS02PageDlg();
	~CWzdUS02PageDlg();

	//��ҳ���Ƿ�ʹ��
	void SetUsed(bool bUsed = true);
	bool IsUsed();

	//�ڻ�ȡ���ݺ����ÿؼ���Ϣ
	virtual void InitCtrlInfo();
	//��������Ϣ
	virtual void SetWzdInfo(US02_WzdInfo* pWzdInfo);

	//��ʾ������
	virtual void ShowWzdData(bool bRefresh);

	//ҳ����ɶ�ȡ����Ϣ
	virtual void GetWzdData(bool bNext = true);

	//�뿪ĳһҳ��ʱ
	virtual void OnLeavePage();
	
	virtual unsigned long CheckValue();
protected:
	US02_WzdInfo* m_pWzdInfo;
	bool m_bUsed;
};

#endif

