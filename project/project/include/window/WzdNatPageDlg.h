/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : ZXX
** Date         : 2013-11-25
** Name         : WzdNatPageDlg.h
** Version      : 1.0
** Description  : ��͸�򵼼̳жԻ���
** Modify Record:
***********************************************************************/

#ifndef _WZD_NAT_PAGE_DLG_H_
#define _WZD_NAT_PAGE_DLG_H_

#include "Dialog.h"
#include "WndStructDef.h"

using namespace GUI;

typedef enum _DEV_SELECT_
{
	DEV_ANDROID = 0,
	DEV_APPLE	= 1,
	DEV_PC		= 2,
	DEV_MOBILE  = 3,
	NEXT_PC		= 4,
	NEXT_MOBILE = 5,
}DEV_SELECT_INDEX;

class CWzdNatPageDlg : public GUI::CDialog
{
public:
	CWzdNatPageDlg();
	~CWzdNatPageDlg();

	//��ҳ���Ƿ�ʹ��
	void SetUsed(bool bUsed = true);
	bool IsUsed();

	//�ڻ�ȡ���ݺ����ÿؼ���Ϣ
	virtual void InitCtrlInfo();
	//��������Ϣ
	virtual void SetWzdInfo(NAT_WIZARD_INFO* pWzdInfo);

	//��ʾ������
	virtual void ShowWzdData(bool bRefresh);

	//ҳ����ɶ�ȡ����Ϣ
	virtual void GetWzdData(bool bNext = true);

	//�뿪ĳһҳ��ʱ
	virtual void OnLeavePage();

	virtual unsigned long CheckValue();
	
protected:
	NAT_WIZARD_INFO* m_pWzdInfo;
	bool m_bUsed;
};

#endif
//end
