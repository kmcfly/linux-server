/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : ZXX
** Date         : 2012-09-17
** Name         : WzdUS02ManDlg.h
** Version      : 1.0
** Description  : 向导管理对话框
** Modify Record:
***********************************************************************/

#ifndef _WZD_US02_MAN_DLG_H_
#define _WZD_US02_MAN_DLG_H_
#include "Static.h"
#include "CheckCtrl.h"
#include "Dialog.h"
#include "CfgDlg.h"
#include "WzdUS02PageDlg.h"
#include <vector>
#include <string>
using namespace GUI;

class CWzdUS02ManDlg : public CCfgDlg
{
public:
	CWzdUS02ManDlg();
	~CWzdUS02ManDlg();

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
	unsigned long SaveUS02Wizard();//保存并退出
	void OnIdle();				//向导定时关闭处理
protected:
	unsigned long CheckValue();
	//配置部分相关
	virtual void GetItemList(CFGITEM_LIST& itemList) const;
	virtual bool SetData(NCFG_ITEM_ID cfgID, BYTE*& pData, int& len) const;

	bool EnterConfig();
	void ExitConfig() const;

protected:
	void CreateUS02WizardPage();	//初始化向导页面
	void CreateWizardInfo();		//创建向导变量
	void InitWizardInfo();			//初始化向导变量


	void ShowPage(int index);		//显示某一个页面
	void ShowBtn(int index);		//更新按钮状态
	void OnWzdEnd();			//向导结束生效处理

protected:
	//创建第一页的向导欢迎页面
	CWzdUS02PageDlg* CreateUS02TipStartDlg(unsigned short x, unsigned short y, unsigned short cx, unsigned short cy, bool show,int pageIndex);
	//创建带tab控件的向导第2页面，包括了语言、时间和dst设置
	CWzdUS02PageDlg* CreateUS02TabBasicManDlg(unsigned short x, unsigned short y, unsigned short cx, unsigned short cy, bool show,int pageIndex);
	//创建网络连接前的提示页面
	CWzdUS02PageDlg* CreateUS02ConntctTipDlg(unsigned short x, unsigned short y, unsigned short cx, unsigned short cy, bool show,int pageIndex);
	//获取IP等网络设置的页面
	CWzdUS02PageDlg* CreateUS02GetNetWorkSetUpDlg(unsigned short x, unsigned short y, unsigned short cx, unsigned short cy, bool show,int pageIndex);
	//创建检查upnp功能的页面
	CWzdUS02PageDlg *CreateUS02CheckUpnpDlg(unsigned short x, unsigned short y, unsigned short cx, unsigned short cy, bool show,int pageIndex);
	//创建打开端口页面
	CWzdUS02PageDlg *CreateUS02GetPortDlg(unsigned short x, unsigned short y, unsigned short cx, unsigned short cy, bool show,int pageIndex);
	//创建获取路由ip页面
	CWzdUS02PageDlg *CreateUS02GetRouterIpDlg(unsigned short x, unsigned short y, unsigned short cx, unsigned short cy, bool show,int pageIndex);
	//创建移动设备选择页面
	CWzdUS02PageDlg *CreateUS02SelectMobileDlg(unsigned short x, unsigned short y, unsigned short cx, unsigned short cy, bool show,int pageIndex);
	//创建移动设备提示页面
	CWzdUS02PageDlg *CreateUS02MobileTipDlg(unsigned short x, unsigned short y, unsigned short cx, unsigned short cy, bool show,int pageIndex);
	//创建DDNS使用提示页面
	CWzdUS02PageDlg *CreateUS02DDNSTipDlg(unsigned short x, unsigned short y, unsigned short cx, unsigned short cy, bool show,int pageIndex);
	//创建DDNS连接提示页面
	CWzdUS02PageDlg *CreateUS02DDNSConnectTipDlg(unsigned short x, unsigned short y, unsigned short cx, unsigned short cy, bool show,int pageIndex);
	//创建二维码显示页面
	CWzdUS02PageDlg *CreateUS02QRShowDlg(unsigned short x, unsigned short y, unsigned short cx, unsigned short cy, bool show,int pageIndex);
	//创建最后页面
	CWzdUS02PageDlg *CreateUS02EndUpDlg(unsigned short x, unsigned short y, unsigned short cx, unsigned short cy, bool show,int pageIndex);
private:
	//根据输入的字符串ID,返回对应的字符串
	void GetTipText(unsigned long strID,char *errText);
protected:
	int m_curIndex;								//当前页面序号

	US02_WzdInfo*		m_pUS02WzdInfo;			//向导数据

	std::vector<CWzdUS02PageDlg*>	m_vecWzdPage;	//向导页面

	unsigned long m_sysDiskStatus;				//系统磁盘状态

	bool m_bEnterConfig;						// 是否进入配置
	unsigned long   m_displayResolutionMask;
	LONGLONG	m_curTime;
	LONGLONG	m_lastTime;
	LONGLONG    m_maxTime;

	enum WZD_PAGE
	{
		PAGE_NETWORK		= 3,
		PAGE_CHECKUPNP		= 4,
		PAGE_PORT			= 5,
		PAGE_ROUTERIP		= 6,
		PAGE_MOBIELESELECT	= 7,
	};


		//############监听语言改变
	DECLARE_MSG_MAP()
	unsigned long  OnChangeLanguage();
};







#endif