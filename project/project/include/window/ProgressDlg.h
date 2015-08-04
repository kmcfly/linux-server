/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : L
** Date         : 2009-08-19
** Name         : ProgressDlg.h
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/

#ifndef _PROGRESS_DLG_H_
#define _PROGRESS_DLG_H_

#include "Dialog.h"
#include "ProgressCtrl.h"

using namespace GUI;

class CProgressDlg : public GUI::CDialog
{
public:
	CProgressDlg();
	~CProgressDlg();

public:
	void SetDlgStyle( );

public:
	void SetPercent( int percet , bool bRefresh );
	
	void EndProgress();
protected:
	void OnInitial();
	void OnPaint(PAINT_STATUS status);

protected:
	GUI::CProgressCtrl			m_progress;

protected:
	unsigned short m_xOffLeft;			//æ‡¿Î◊Û±ﬂΩÁæ‡¿Î
	unsigned short m_xOffRight;			//æ‡¿Î”“±ﬂΩÁæ‡¿Î
};

#endif
