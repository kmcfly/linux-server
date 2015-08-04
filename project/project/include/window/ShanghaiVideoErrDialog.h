/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : zxx
** Date         : 2012-11-19
** Name         : ShanghaiVideoErrDialog.h
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/

#ifndef __SHANGHAIVIDEOERR_DIALOG_H__
#define __SHANGHAIVIDEOERR_DIALOG_H__

#include "Dialog.h"
#include "Button.h"
#include "CheckCtrl.h"
#include "Static.h"
#include "EditCtrl.h"
#include <string>
#include <map>
#include "ShanghaiCmd.h"
#include "CharCodeTable.h"
using namespace GUI;

class CShanghaiVideoErrDialog: public GUI::CDialog
{
public:
	CShanghaiVideoErrDialog();
	~CShanghaiVideoErrDialog();

	void GetAssignInfo();
protected:
	void OnInitial();
private:
	//建立需要的checkctrl
	void CreateCheckCtrl(int index,unsigned short &x,unsigned short &y,unsigned short &xFocus,unsigned short &yFocus,const char *pstr);
	string ChangeChinese(const char *pstr);
private:
	GUI::CCheckCtrl m_FaultCheck[12];
	GUI::CStatic	m_static[4];
	GUI::CEditCtrl  m_editCtrl;
	GUI::CButton	m_btnUpLoad;
	std::map<int,std::string> m_checkNameMap;//check框的名字记录集合 
	CCharCodeTable* m_codeTable; 
};

#endif
//end
