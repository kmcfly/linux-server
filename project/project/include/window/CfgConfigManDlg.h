/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : L
** Date         : 2009-08-19
** Name         : CfgConfigManDlg.h
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/

#ifndef _CFG_CONFIG_MAN_DLG_
#define _CFG_CONFIG_MAN_DLG_

#include "Dialog.h"
#include "USBExploreDlg.h"


using namespace GUI;

const unsigned long MAX_FILE_LENGTH = 1024 * 1024;			//限制导出的配置文件不能太大

class CCfgConfigManDlg : public CUSBExploreDlg
{
public:
	CCfgConfigManDlg();
	~CCfgConfigManDlg();

protected:
	void OnInitial();
	void OnPostInitial();
	unsigned long OnOK();
	unsigned long OnApply();


	void SetErrTip( int errNum );
protected:
};


#endif
