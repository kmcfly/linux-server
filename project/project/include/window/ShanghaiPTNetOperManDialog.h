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
	unsigned long OnOK();//�˳�
	unsigned long OnApply();//�ϴ�
private:
	CShanghaiAssignDialog		m_ShanghaiAssignDialog;
	CShanghaiOtherErrDialog		m_ShanghaiOtherErrDialog;
	CShanghaiManualSnapDialog	m_ShanghaiManualSnapDialog;

	GUI::CTabCtrl	  m_tabCtrl;
	int				  m_choseType;
	CCharCodeTable    *m_codeTable; 

	enum PAGE_ITEM
	{
		PAGE_MANUALSNAP = 0,		//�ֶ�ץͼ
		PAGE_ASSIGNREPAIR,	//ǩ���ͱ���
	};

	enum chose_type
	{
		type_OPER,
		type_KEEP,
		type_CHECK,
	};
	unsigned long	   m_initPageMask;			//��¼�Ѿ���ʼ����ҳ��
	PAGE_ITEM		   m_curPage;
private:
	string ChangeChinese(const char *pstr);
};

#endif

