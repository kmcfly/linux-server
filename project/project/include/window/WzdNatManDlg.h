/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : ZXX
** Date         : 2013-11-25
** Name         : WzdNatManDlg.h
** Version      : 1.0
** Description  : 穿透向导管理对话框
** Modify Record:
***********************************************************************/

#ifndef _WZD_NAT_MAN_DLG_H_
#define _WZD_NAT_MAN_DLG_H_
#include "Static.h"
#include "CheckCtrl.h"
#include "DateCtrl.h"
#include "TimeCtrl.h"
#include "Dialog.h"
#include "CfgDlg.h"
#include <vector>
#include <string>
#include "WzdNatPageDlg.h"

using namespace GUI;

class CWzdNatManDlg : public CCfgDlg
{
public:
	CWzdNatManDlg();
	~CWzdNatManDlg();

	//进入配置,某些操作必须在进配置之后才能进行.
	bool TryEnterConfig();
protected:
	void OnInitial();
	void OnPostInitial();
	void OnDestroy();
	void OnPaint(PAINT_STATUS status);
	void EndDialog(DLG_END_VALUE result);

	unsigned long OnOK(); //下一页
	unsigned long OnCancel(); //退出
	unsigned long OnApply(); //上一页
	unsigned long SaveNatWizard();//保存并退出
	void OnIdle();				//向导定时关闭处理
protected:
	unsigned long CheckValue();
	//配置部分相关
	virtual void GetItemList(CFGITEM_LIST& itemList) const;
	virtual bool SetData(NCFG_ITEM_ID cfgID, BYTE*& pData, int& len) const;

	bool EnterConfig();
	void ExitConfig() const;

protected:
	void CreateNatWizardPage();	//初始化向导页面
	void CreateWizardInfo();		//创建向导变量
	void InitWizardInfo();			//初始化向导变量


	void ShowPage(int index);		//显示某一个页面
	void ShowBtn(int index);		//更新按钮状态
	void OnWzdEnd();			//向导结束生效处理

protected:
	//创建穿透向导欢迎页面（第一页）
	CWzdNatPageDlg* CreateNatStartDlg(unsigned short x, unsigned short y, unsigned short cx, unsigned short cy, bool bshow,int pageIndex);
	
	//创建带tab控件的向导第2页面，包括了语言、时间和dst设置
	CWzdNatPageDlg* CreateNatTabBasicManDlg(unsigned short x, unsigned short y, unsigned short cx, unsigned short cy, bool bshow,int pageIndex);

	//检测IP页面，如果检测不到，不能继续往下
	CWzdNatPageDlg* CreateNatGetNetWorkSetUpDlg(unsigned short x, unsigned short y, unsigned short cx, unsigned short cy, bool bshow,int pageIndex);
	
	//建立穿透和修改密码页面
	CWzdNatPageDlg* CreateNatPassWordDlg(unsigned short x, unsigned short y, unsigned short cx, unsigned short cy, bool bshow,int pageIndex);

	//如果选择了本地连接，该页就是最后页面
	CWzdNatPageDlg* CreateLocalConnectEndDlg(unsigned short x, unsigned short y, unsigned short cx, unsigned short cy, bool bshow,int pageIndex);

	//创建设备选择页面（android,apple,pc）
	CWzdNatPageDlg* CreateNatSelectDevDlg(unsigned short x, unsigned short y, unsigned short cx, unsigned short cy, bool bshow,int pageIndex);
	
	//创建二维码页面
	CWzdNatPageDlg* CreateNatQrCodeDlg(unsigned short x, unsigned short y, unsigned short cx, unsigned short cy, bool bshow,int pageIndex);

	//创建PC提示页面
	CWzdNatPageDlg* CreateNatPCTipDlg(unsigned short x, unsigned short y, unsigned short cx, unsigned short cy, bool bshow,int pageIndex);

	//创建穿透最后页面
	CWzdNatPageDlg *CreateNatEndUpDlg(unsigned short x, unsigned short y, unsigned short cx, unsigned short cy, bool bshow,int pageIndex);
private:
	//根据输入的字符串ID,返回对应的字符串
	void GetTipText(unsigned long strID,char *errText);
protected:
	int m_curIndex;								//当前页面序号

	NAT_WIZARD_INFO*		m_pNatWzdInfo;			//向导数据

	std::vector<CWzdNatPageDlg*>	m_vecWzdPage;	//向导页面

	unsigned long m_sysDiskStatus;				//系统磁盘状态

	bool m_bEnterConfig;						// 是否进入配置
	unsigned long   m_displayResolutionMask;
	LONGLONG	m_curTime;
	LONGLONG	m_lastTime;
	LONGLONG    m_maxTime;
	

	enum WZD_PAGE
	{
		PAGE_NATPASS		= 2,//就是修改密码页面
		PAGE_NETWORK		= 3,
		PAGE_LOCALEND		= 4,//本地结束页面
		PAGE_DEVSELECT		= 5,
		PAGE_QRCODE			= 6,
		PAGE_PCTIP			= 7,
	};

	int	m_devSelect;//选择的设备，默认为DEV_ANDROID

};







#endif