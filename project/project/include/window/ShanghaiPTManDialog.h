/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : zxx
** Date         : 2012-12-30
** Name         : ShanghaiPTManDialog.h
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/
#ifndef __SHANGHAI_PTMAN_DIALOG_H__
#define __SHANGHAI_PTMAN_DIALOG_H__

#include "TabCtrl.h"
#include "CfgDlg.h"
#include "CfgConnectSGPTDlg.h"
#include "CfgRecordSnapDlg.h"
#include "ShanghaiCfgDailyImageDialog.h"
#include "ShanghaiAlarmImageDialog.h"
#include "CharCodeTable.h"
#include <string>
using namespace GUI;

class CShanghaiPTManDialog: public CCfgDlg
{
public:
	CShanghaiPTManDialog();
	~CShanghaiPTManDialog();
protected:
	enum PAGE_ITEM
	{
		PAGE_NET = 0,	//平台网络连接
		PAGE_SNAPCFG,	//平台抓图配置
		PAGE_DAILYIMAGE,//平台日常图像配置
		PAGE_ALARMIMAGE,//平台报警图像配置
	};

	virtual void OnInitial();	
	
	void OnPostInitial();

	void OnDestroy();

	void GetItemList(CFGITEM_LIST& itemList) const;

	CCfgDlg * GetPage(PAGE_ITEM item);

	unsigned long OnPreCommand(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);
	
	unsigned long OnApply();

	unsigned long CheckValue();

	bool EnterConfig();

	void ExitConfig() const;
protected:
	DECLARE_MSG_MAP()
	unsigned long OnClickNET();
	unsigned long OnClickRecordSnap();
	unsigned long OnClickDailyImage();
	unsigned long OnClickAlarmImage();

private:
	string ChangeChinese(const char *pstr);
private:
	GUI::CTabCtrl	  m_tabCtrl;

	unsigned long	   m_initPageMask;			//记录已经初始化的页面
	PAGE_ITEM		   m_curPage;

	CCfgConnectSGPTDlg			 m_PTNETCfgDialog;			//平台接入
	CCfgRecordSnapDlg			 m_cfgRecordSnapDlg;		//抓图配置
	CShanghaiCfgDailyimageDialog m_shanghaiDailyImageDialog;//日常图像
	CShanghaiAlarmImageDialog	 m_shanghaiAlarmimageDialog;//报警图像
	CCharCodeTable* m_codeTable;
};

#endif
