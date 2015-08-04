/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : L
** Date         : 2010-02-26
** Name         : WzdManDlg.h
** Version      : 1.0
** Description  : 向导管理对话框
** Modify Record:
***********************************************************************/

#ifndef _WZD_MAN_DLG_H_
#define _WZD_MAN_DLG_H_

#include "Static.h"
#include "CheckCtrl.h"
#include "Dialog.h"
#include "WndStructDef.h"
#include "WzdPageDlg.h"
#include "CfgDlg.h"
#include "WzdTipNetDlg.h"
#include "WzdRecordStreamDlg.h"
#include "WzdDiskManDlg.h"
#include <vector>

using namespace GUI;

class CWzdManDlg : public CCfgDlg
{
public:
	CWzdManDlg();
	~CWzdManDlg();

	void SetDiskStatus(unsigned long status);

	//进入配置,某些操作必须在进配置之后才能进行.
	bool TryEnterConfig();

protected:
	void OnInitial();
	void OnPostInitial();
	void OnDestroy();
	void OnPaint(PAINT_STATUS status);
	void EndDialog(DLG_END_VALUE result);
	
	unsigned long OnOK(); //上一页
	unsigned long OnCancel(); //下一页
	unsigned long OnApply(); //上一页
	unsigned long SaveWizard();//保存并退出

protected:
	//配置部分相关
	virtual void GetItemList(CFGITEM_LIST& itemList) const;
	virtual bool SetData(NCFG_ITEM_ID cfgID, BYTE*& pData, int& len) const;

	bool EnterConfig();
	void ExitConfig() const;

protected:
	void CreateWizardInfo();	//创建向导变量
	void InitWizardInfo();		//初始化向导变量

	void CreateWizardPage();	//初始化向导页面

	void ShowPage(int index);	//显示某一个页面

	void ShowBtn(int index);	//更新按钮状态

	void OnWzdEnd();			//向导结束生效处理

	void OnIdle();				//向导定时关闭处理

protected:
	//创建磁盘提示页面
	CWzdPageDlg* CreateTipDiskDlg(unsigned short x, unsigned short y, unsigned short cx, unsigned short cy, bool show );
	//创建网络提示页面
	CWzdTipNetDlg * CreateTipNetDlg(unsigned short x, unsigned short y, unsigned short cx, unsigned short cy, bool show );
	//创建录像提示页面
	CWzdRecordStreamDlg *CreateTipRecordDlg(unsigned short x, unsigned short y, unsigned short cx, unsigned short cy, bool show );
	//创建磁盘管理提示页面
	CWzdDiskManDlg *CreateDiskManDlg(unsigned short x, unsigned short y, unsigned short cx, unsigned short cy, bool show );

protected:
	int m_curIndex;								//当前页面序号

	WIZARD_INFO*		m_pWizardInfo;			//向导数据

	std::vector<CWzdPageDlg*>	m_vecWzdPage;	//向导页面

	unsigned long m_sysDiskStatus;				//系统磁盘状态

	bool m_bEnterConfig;						// 是否进入配置
	unsigned long   m_displayResolutionMask;
	LONGLONG	m_curTime;
	LONGLONG	m_lastTime;
	LONGLONG    m_maxTime;

};

#endif

