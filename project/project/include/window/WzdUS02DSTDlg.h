/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : ZXX
** Date         : 2012-09-18
** Name         : WzdUS02DSTDlg.h
** Version      : 1.0
** Description  : ��ҳ��Ի���
** Modify Record:
***********************************************************************/

#ifndef _WZD_US02_DST_DLG_H_
#define _WZD_US02_DST_DLG_H_
#include "Dialog.h"
#include "WzdUS02PageDlg.h"
#include "ListCtrl.h"
#include "WzdUS02DSTWeekDlg.h"
#include "WzdUS02DSTDateDlg.h"

class CWzdUS02DSTDlg : public CWzdUS02PageDlg
{
public:
	CWzdUS02DSTDlg();
	~CWzdUS02DSTDlg();

	unsigned long CheckValue();
	//�ڻ�ȡ���ݺ����ÿؼ���Ϣ
	void InitCtrlInfo();
	//��������Ϣ
	void SetWzdInfo(US02_WzdInfo* pWzdInfo);
	//��ʾ������
	void ShowWzdData(bool bRefresh);
	//��ȡ������
	void GetWzdData(bool bNext);
	enum PAGE_ITEM
	{
		PAGE_NULL	= -1,
		PAGE_WEEK	= DAYLIGHT_TYPE_WEEK,
		PAGE_DATE	= DAYLIGHT_TYPE_DAY,
	};
protected:
	void OnInitial();
	void OnDestroy();
	void OnPaint(PAINT_STATUS status);
protected:
	void SetPage(PAGE_ITEM item = PAGE_WEEK,bool bRefresh = true );
	
	void CheckUI(bool bRefresh);
	//ˢ�����ݵ�����
	void RefreshView(bool bRefresh=true);
protected:
	DECLARE_MSG_MAP()
	unsigned long OnClickList(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);
	unsigned long OnClickRadio();
protected:
	GUI::CListCtrl	m_listDSTCtrl;

	GUI::CRadioGroup	m_rdGroup;
	GUI::CRadioCtrl		m_rdWeek;
	GUI::CRadioCtrl		m_rdDate;
	GUI::CStatic		m_scInfo;
	enum LIST_ROW
	{
		ROW_ENABLE_DST,
		ROW_TIME_OFFSET,
	};

	CWzdUS02DSTWeekDlg m_dstWeekDlg;
	CWzdUS02DSTDateDlg m_dstDateDlg;
private:
	US02_WzdInfo* m_pWzdInfo;
	PAGE_ITEM	m_curPage;
	DAYLIGHT_INFO		m_dstData;//����ID��Ŀ���ڱ��ڴ�����

};

#endif
