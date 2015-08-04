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
	unsigned long OnClickRadioAssign();//����л�ǩ����Ա��ѡ��ť
	unsigned long OnClickRadioRepair();
private:
	//������Ŀ����ctrl
	void CreateTypeCheckCtrl(int index,unsigned short &x,unsigned short &y,unsigned short &xFocus,unsigned short &yFocus,const char *pstr);
	string ChangeChinese(const char *pstr);
protected:
	GUI::CRadioGroup	m_rdGroupAssign;	//ǩ����Ա��ѡ��ťȺ��
	GUI::CRadioCtrl		m_rdAssignKeep;		//������Աǩ����ѡ��ť
	GUI::CRadioCtrl		m_rdAssignRepair;	//ά����Աǩ����ѡ��ť

	GUI::CStatic		m_typeStatic;		//��Ŀѡ��
	GUI::CCheckCtrl		m_typeCheck[8];		//��Ŀ����check��
	GUI::CStatic		m_Editstatic;		//��¼˵��
	GUI::CEditCtrl		m_inputEdit;		//��Ŀ˵�������

	GUI::CRadioGroup	m_rdGroupRepair;	//��������ťȺ��
	GUI::CRadioCtrl		m_rdRepairNone;		//δ�޸���ѡ��ť
	GUI::CRadioCtrl		m_rdRepairPart;		//�����޸���ѡ��ť
	GUI::CRadioCtrl		m_rdRepairAll;		//��ȫ�޸���ѡ��ť

	
	typedef enum _REPAIR_RESULT_
	{
		REPAIR_NONE =0,	//δ�޸�
		REPAIR_PART,	//�����޸�
		REPAIR_FIX,		//��ȫ�޸�
	}REPAIR_INFO;

	typedef enum __ASSIGN_TYPE__
	{
		ASSIGN_KEEP =0,	//����ǩ��
		ASSIGN_REPAIR,	//ά��ǩ��
	}ASSIGN_TYPE;
	
	unsigned short m_curAssignType;
	unsigned short m_curRepairType;
private:
	std::map<int,std::string> m_checkNameMap;//check������ּ�¼���� 
	char m_repairResult[256];
	CCharCodeTable* m_codeTable; 
};


#endif

//end
