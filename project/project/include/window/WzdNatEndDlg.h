/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : ZXX
** Date         : 2013-11-27
** Name         : WzdNatEndDlg.h
** Version      : 1.0
** Description  : 穿透向导页面结束对话框
** Modify Record:
***********************************************************************/

#ifndef _WZD_NAT_END_DLG_H_
#define _WZD_NAT_END_DLG_H_
#include "Dialog.h"
#include "WzdNatPageDlg.h"
#include "ScrollCtrl.h"
#include "RichTextCtrl.h"
#include "CheckCtrl.h"
using namespace GUI;

//////////////////////////////////////////////////////////////////////////
// * 向导 结束页面
//////////////////////////////////////////////////////////////////////////

class CWzdNatEndDlg : public CWzdNatPageDlg
{
public:
	CWzdNatEndDlg();
	~CWzdNatEndDlg();

	//设置向导信息
	void SetWzdInfo(NAT_WIZARD_INFO* pWzdInfo);
	void ShowWzdData(bool bRefresh);
	//页面完成读取向导信息
	void GetWzdData(bool bNext);
	void OnLeavePage();
	void SetTipInfo(const std::string& str);
protected:
	void OnInitial();
	void OnDestroy();
	virtual void InitCtrlInfo();
	void OnPaint(PAINT_STATUS status);
protected:
	DECLARE_MSG_MAP()
	unsigned long OnCheck();
	unsigned long OnScrollchange(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);
private:
	void DrawTriangle(int x,int y);//画三角形
protected:
	GUI::CRichTextCtrl	m_richTextCtrl;
	GUI::CCheckCtrl		m_checkCtrl;
	GUI::CScrollCtrl	m_scroll_v;
	NAT_WIZARD_INFO     *m_pWzdInfo;

	int					m_drawX;//自绘图像开始X坐标
	int					m_drawY;//自绘图像开始Y坐标
	unsigned short		m_toprow;//滚屏显示的第一行

};

#endif
