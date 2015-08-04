/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : ZXX
** Date         : 2013-11-26
** Name         : WzdNewNatGetNetWorkSetUpDlg.h
** Version      : 1.0
** Description  : 向导管理对话框
** Modify Record:
***********************************************************************/

#ifndef _WZD_NAT_GETNETWORKSETUP_DLG_H_
#define _WZD_NAT_GETNETWORKSETUP_DLG_H_
#include "Dialog.h"
#include "WzdNatPageDlg.h"
#include "ScrollCtrl.h"
#include "ListCtrl.h"
#include "Static.h"
#include "RichTextCtrl.h"
#include "CheckCtrl.h"
#include "RadioCtrl.h"
class CWzdNewNatGetNetWorkSetUpDlg : public CWzdNatPageDlg
{
public:
	CWzdNewNatGetNetWorkSetUpDlg();
	~CWzdNewNatGetNetWorkSetUpDlg();

	//设置向导信息
	void SetWzdInfo(NAT_WIZARD_INFO* pWzdInfo);
	//在获取数据后设置控件信息
	void InitCtrlInfo();
	//显示向导数据
	void ShowWzdData(bool bRefresh);

	//页面完成读取向导信息
	void GetWzdData(bool bNext = true);

	bool GetNatEnaleFlag();
protected:
	void OnInitial();
	void OnDestroy();
private:
	bool GetIp();
	void GetStaticIp(unsigned long ip,char *pIP);//把unsigned long ip组装为字符串型
protected:
	DECLARE_MSG_MAP()
	unsigned long OnCheck();
	unsigned long OnScrollchange(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);
	unsigned long OnClickRadioCtrl();//选择不同的选项后，下一个页面也变得有所不同
protected:
	GUI::CListCtrl		 m_netList;
	GUI::CRichTextCtrl   m_netNoteRichText;
	GUI::CCheckCtrl		 m_checkCtrl;				//下次是否显示向导
	GUI::CScrollCtrl	 m_scroll_v;
	GUI::CRadioGroup	 m_rdGroup;
	GUI::CRadioCtrl		 m_rdNat;
	GUI::CRadioCtrl		 m_rdLocal;


	unsigned short		m_toprow;//滚屏显示的第一行
private:
	NAT_WIZARD_INFO		*m_pNetWzdInfo;
	enum LIST_ROW
	{
		ROW_NET_STATUS,
		ROW_GETIP_STATUS,
		ROW_DVRIP,
	};

	typedef enum _NET_SELECT_
	{
		NET_NAT,
		NET_LOCAL,
	}NET_SELECT;

	bool m_bEnableNatFlag;//是否打开穿透的标志，默认为true;
};

#endif

