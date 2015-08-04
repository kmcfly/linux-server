/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : ZXX
** Date         : 2012-09-22
** Name         : WzdUS02DSTWeekDlg.h
** Version      : 1.0
** Description  : ��ҳ��Ի���
** Modify Record:
***********************************************************************/

#ifndef _WZD_US02_DST_WEEKDLG_H_
#define _WZD_US02_DST_WEEKDLG_H_
#include "Dialog.h"
#include "ListCtrl.h"
using namespace GUI;

class CWzdUS02DSTWeekDlg:public GUI::CDialog
{
public:
	CWzdUS02DSTWeekDlg();
	~CWzdUS02DSTWeekDlg();

	void SetDstData(DAYLIGHT_INFO* pDst) { m_dst = pDst;}
	//ˢ�����ݵ�����
	void RefreshView(bool bRefresh=true);
protected:
	void OnInitial();
	void OnPaint(PAINT_STATUS status);

protected:
	DECLARE_MSG_MAP()
	unsigned long OnClickList(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);

protected:
	GUI::CListCtrl			m_listCtrl;

	enum LIST_ROW
	{
		ROW_START_MONTH,
		ROW_START_WEEKOFMON,
		ROW_START_WEEK,
		ROW_START_TIME,

		ROW_END_MONTH,
		ROW_END_WEEKOFMON,
		ROW_END_WEEK,
		ROW_END_TIME,
	};

protected:
	DAYLIGHT_INFO*		m_dst;
};


#endif