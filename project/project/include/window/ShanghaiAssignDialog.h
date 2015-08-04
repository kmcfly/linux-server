/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : zxx
** Date         : 2012-11-19
** Name         : ShanghaiAssignDialog.h
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/


#ifndef __SHANGHAI_ASSIGN_MAN_H__
#define __SHANGHAI_ASSIGN_MAN_H__

#include "Dialog.h"
#include "Button.h"
#include "RadioCtrl.h"
#include "EditCtrl.h"
#include "CheckCtrl.h"
#include <string>
#include <map>
#include "ShanghaiCmd.h"
#include "CharCodeTable.h"
using namespace GUI;

class CShanghaiAssignDialog: public GUI::CDialog
{
public:
	CShanghaiAssignDialog();
	~CShanghaiAssignDialog();

	void GetAssignInfo();
protected:
	void OnInitial();
protected:
	DECLARE_MSG_MAP()
	unsigned long OnClickRadioAssign();//点击切换签到人员单选按钮
	unsigned long OnClickRadioRepair();
private:
	//建立项目类型ctrl
	void CreateTypeCheckCtrl(int index,unsigned short &x,unsigned short &y,unsigned short &xFocus,unsigned short &yFocus,const char *pstr);
	string ChangeChinese(const char *pstr);
protected:
	GUI::CRadioGroup	m_rdGroupAssign;	//签到人员单选按钮群组
	GUI::CRadioCtrl		m_rdAssignKeep;		//保养人员签到单选按钮
	GUI::CRadioCtrl		m_rdAssignRepair;	//维修人员签到单选按钮

	GUI::CStatic		m_typeStatic;		//项目选择
	GUI::CCheckCtrl		m_typeCheck[8];		//项目类型check框
	GUI::CStatic		m_Editstatic;		//记录说明
	GUI::CEditCtrl		m_inputEdit;		//项目说明输入框

	GUI::CRadioGroup	m_rdGroupRepair;	//修理结果按钮群组
	GUI::CRadioCtrl		m_rdRepairNone;		//未修复单选按钮
	GUI::CRadioCtrl		m_rdRepairPart;		//部分修复单选按钮
	GUI::CRadioCtrl		m_rdRepairAll;		//完全修复单选按钮

	
	typedef enum _REPAIR_RESULT_
	{
		REPAIR_NONE =0,	//未修复
		REPAIR_PART,	//部分修复
		REPAIR_FIX,		//完全修复
	}REPAIR_INFO;

	typedef enum __ASSIGN_TYPE__
	{
		ASSIGN_KEEP =0,	//保养签到
		ASSIGN_REPAIR,	//维修签到
	}ASSIGN_TYPE;
	
	unsigned short m_curAssignType;
	unsigned short m_curRepairType;
private:
	std::map<int,std::string> m_checkNameMap;//check框的名字记录集合 
	char m_repairResult[256];
	CCharCodeTable* m_codeTable; 
};


#endif

//end
