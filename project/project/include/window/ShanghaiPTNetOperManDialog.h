/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : zxx
** Date         : 2013-1-5
** Name         : ShanghaiPTNetOperManDialog.h
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/

#ifndef _SHANGHAI_PTOPERMANDIALOG_H_
#define _SHANGHAI_PTOPERMANDIALOG_H_

#include "Dialog.h"
#include "TabCtrl.h"
#include "Button.h"
#include "ShanghaiAssignDialog.h"
#include "ShanghaiOtherErrDialog.h"
#include "ShanghaiManualSnapDialog.h"
#include "CharCodeTable.h"
#include "NetProtocol.h"
#include <string>
using namespace GUI;
class CShanghaiPTNetOperManDialog: public GUI::CDialog
{
public:
	CShanghaiPTNetOperManDialog();
	~CShanghaiPTNetOperManDialog();
	void SetChoseType(unsigned long choseType);
protected:
	void OnInitial();
	unsigned long OnPreCommand(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);
	unsigned long OnOK();//退出
	unsigned long OnApply();//上传
private:
	CShanghaiAssignDialog		m_ShanghaiAssignDialog;
	CShanghaiOtherErrDialog		m_ShanghaiOtherErrDialog;
	CShanghaiManualSnapDialog	m_ShanghaiManualSnapDialog;

	GUI::CTabCtrl	  m_tabCtrl;
	int				  m_choseType;
	CCharCodeTable    *m_codeTable; 

	enum PAGE_ITEM
	{
		PAGE_MANUALSNAP = 0,		//手动抓图
		PAGE_ASSIGNREPAIR,	//签到和报修
	};

	enum chose_type
	{
		type_OPER,
		type_KEEP,
		type_CHECK,
	};
	unsigned long	   m_initPageMask;			//记录已经初始化的页面
	PAGE_ITEM		   m_curPage;
private:
	string ChangeChinese(const char *pstr);
};

#endif

