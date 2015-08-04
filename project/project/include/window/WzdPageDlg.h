/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : L
** Date         : 2010-02-26
** Name         : WzdPageDlg.h
** Version      : 1.0
** Description  : �򵼶Ի���
** Modify Record:
***********************************************************************/

#ifndef _WZD_PAGE_DLG_H_
#define _WZD_PAGE_DLG_H_

#include "Dialog.h"
#include "WndStructDef.h"

using namespace GUI;

class CWzdPageDlg : public GUI::CDialog
{
public:
	CWzdPageDlg();
	~CWzdPageDlg();

	//��ҳ���Ƿ�ʹ��
	void SetUsed(bool bUsed = true);
	bool IsUsed();

	//�ڻ�ȡ���ݺ����ÿؼ���Ϣ
	virtual void InitCtrlInfo();
	//��������Ϣ
	virtual void SetWzdInfo(WIZARD_INFO* pWzdInfo);

	//��ʾ������
	virtual void ShowWzdData(bool bRefresh);

	//ҳ����ɶ�ȡ����Ϣ
	virtual void GetWzdData(bool bNext = true);

	//�뿪ĳһҳ��ʱ
	virtual void OnLeavePage();

protected:
	WIZARD_INFO* m_pWzdInfo;
	bool m_bUsed;
};

#endif

