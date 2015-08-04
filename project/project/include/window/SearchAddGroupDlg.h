/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : wn
** Date         : 2011-10-01
** Name         : SearchAddGroupDlg
** Version      : 1.0
** Description  : 
** Modify Record:
** Modify       :zxx
1:  注意： 通道总数有外面传入
***********************************************************************/
#if defined(__GROUY_SEARCH__)
#ifndef _SEARCH_ADD_GROUP_DLG_H_
#define _SEARCH_ADD_GROUP_DLG_H_

#include "Dialog.h"
#include "EditCtrl.h"
#include "CheckCtrl.h"
#include "Product.h"
#include "NetConfigDefine.h"
#include "Button.h"
#include <list>

using namespace GUI;

typedef std::list<NCFG_INFO_GROUP_ITEM> GROUP_ITEM_LIST;

class CSearchAddGroupDlg : public GUI::CDialog
{
public:
	CSearchAddGroupDlg();
	~CSearchAddGroupDlg();
    void SetInitialInfo(unsigned long chnnNum, ULONGLONG authCH,GROUP_ITEM_LIST *pcfgGroup);
	//当点击编辑时候需要的函数
	//参数:bool benable ---获取是否按下编辑按钮,不需要编辑传入false
	//    string curStr ---传入的是上一个页面选择的分组名，不需要编辑传入""
	//    int nItem     ---传入的combo的当前项,不需要编辑传入0
	void GetEditEnable(bool bIsEdit,string curStr,int nItem);
	//发送需要刷新的combo当前值
	int SendCurnItem();
protected:
	DECLARE_MSG_MAP()
	void OnInitial();
	void OnPostInitial();
	unsigned long OnOK();//保存并退出
	unsigned long OnCancel();//退出
	void OnDestroy();
	void OnPaint(PAINT_STATUS status);
	unsigned long OnClickEdit();
	unsigned long OnClickChnn( unsigned long ctrlID );
	unsigned long OnClickGroup( unsigned long ctrlID );
	unsigned long DoClickChnn( unsigned long index, bool bRefresh );
private:
	GUI::CEditCtrl m_editAdd;
	GUI::CCheckCtrl	*m_pCKCH, *m_pCKGroup;
	GUI::CStatic  m_static;
	GUI::CStatic  m_selectStatic;

	std::string m_curStr;
	unsigned long m_chnnNum;//通道数目
	bool m_bIsEdit;//是否是编辑页面
	ULONGLONG m_authCH;
	int m_nItem;//上一个页面combo控件的值
	
	GROUP_ITEM_LIST *m_pcfgList;
private:
	void SetChannel(const ULONGLONG chanbits);
	ULONGLONG GetCurChannel() const;
	bool AddGroup();//修改页面值的时候判断是否增加分组
	int GetLineNum();
	int GetGroupNum();
};

#endif
#endif
