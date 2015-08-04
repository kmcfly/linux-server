/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : L
** Date         : 2009-08-19
** Name         : SystemUpgradeDlg.h
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/
#ifndef _SYSTEM_UPGRADE_DLG_H_
#define _SYSTEM_UPGRADE_DLG_H_
#ifndef __ENVIRONMENT_WIN32__
#include <sys/stat.h>
#include <dirent.h>
#endif
#include "Dialog.h"
#include "Static.h"
#include "MessageBox.h"
#include "ProgressDlg.h"
#include "PUB_common.h"
#include "ComboCtrl.h"
#include "ListCtrlEx.h"
#include "mylist.cpp"
#include "USBExploreDlg.h"


using namespace GUI;

class CSystemUpgradeDlg : public CUSBExploreDlg
{
public:
	CSystemUpgradeDlg();
	~CSystemUpgradeDlg();



protected:
	void OnInitial();
	unsigned long OnOK();
	void EndDialog(DLG_END_VALUE result);



};

#endif //_SYSTEM_UPGRADE_DLG_H_




