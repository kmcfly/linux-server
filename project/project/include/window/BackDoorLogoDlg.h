/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : L
** Date         : 2010-12-01
** Name         : BackDoorLogo.h
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/

#ifndef _BACK_DOOR_LOGO_DLG_H_
#define _BACK_DOOR_LOGO_DLG_H_

#include "Wnd.h"
#include "Dialog.h"
#include "Static.h"

class CBackDoorLogoDlg : public GUI::CDialog
{
public:
	CBackDoorLogoDlg();
	~CBackDoorLogoDlg();

	void OnInitial();
	void OnPostInitial();
	void OnDestroy();

protected:
	virtual unsigned long OnOK();

	bool CheckImageFile( );
	void UmountUsb();

	GUI::CStatic m_scWeb[3];
	GUI::CStatic m_scWebDisp;

	GUI::CStatic m_scDvr[3];
	GUI::CStatic m_scDvrDisp;

	GUI::CStatic m_scDvrTrans[3];
	GUI::CStatic m_scDvrTransDisp;

	char* m_pPathWeb;
	char* m_pPathDvr;
	char* m_pPathDvrTrans;
};

#endif
//end
