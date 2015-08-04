/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : zxx
** Date         : 2010-07-21
** Name         : CWzdDiskManDlg.h
** Version      : 1.0
** Description  : 
** Modify Record:
����:���̹������
***********************************************************************/
#ifndef _WzdDiskMan_H_
#define _WzdDiskMan_H_

#include "Dialog.h"
#include "WndStructDef.h"
#include "WzdPageDlg.h"
#include "ListCtrl.h"
#include "Static.h"
#include "Button.h"
#include "ProgressDlg.h"
#include "CheckCtrl.h"

using namespace GUI;

class CWzdDiskManDlg:public CWzdPageDlg
{
public:
	CWzdDiskManDlg();
	~CWzdDiskManDlg();

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
	void SetPercent( int percet );
	void EndProgress();
	void RefreshDiskInfo(bool bRefresh = true);

protected:
	virtual void OnInitial();
	void OnIdle();

	DECLARE_MSG_MAP()
	unsigned long OnFormat();
	unsigned long OnRefresh();
	unsigned long OnClickList(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);

protected:
	void EnableFormatBtn(bool bEnable, bool bRefresh);
	void EnableRefreshBtn(bool bEnable, bool bRefresh);
	void EnableDefaultBtn(bool bEnable, bool bRefresh);//��ֹ������һ������һ�����˳�����

protected:
	WIZARD_INFO*		m_pWzdInfo;
	bool				m_bUsed;

	int					m_finishDiskNum;  //����ɸ�ʽ���ĸ���

private:
	GUI::CListCtrl		m_diskManList;
	CProgressDlg		m_progressDlg;
	GUI::CStatic		m_stcMsg;
	GUI::CButton		m_bntRefresh;
	GUI::CButton		m_bntFormat;
	
	
	bool				m_prgShow;
	DVR_DISK_INFO_LIST	m_diskInfoList;
	POS*				m_listRowToDiskInfo;
	int					m_curSelectRow;

	bool				m_bFormating;
	int					m_diplayPrecent;
};

#endif

//end
