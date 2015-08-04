/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : ZXX
** Date         : 2013-11-25
** Name         : WzdNatQrCodeDlg.h
** Version      : 1.0
** Description  : 穿透向导页面二维码对话框
** Modify Record:
***********************************************************************/

#ifndef _WZD_NAT_QRCODE_DLG_H_
#define _WZD_NAT_QRCODE_DLG_H_
#include "Dialog.h"
#include "WzdNatPageDlg.h"
#include "RichTextCtrl.h"
#include "CheckCtrl.h"
#include "ScrollCtrl.h"
using namespace GUI;

//////////////////////////////////////////////////////////////////////////
// * 向导 二维码页面
//////////////////////////////////////////////////////////////////////////
class CWzdNatQrCodeDlg : public CWzdNatPageDlg
{
public:
	CWzdNatQrCodeDlg();
	~CWzdNatQrCodeDlg();

	//设置向导信息
	virtual void SetWzdInfo(NAT_WIZARD_INFO* pWzdInfo);
	virtual void ShowWzdData(bool bRefresh);
	virtual void GetWzdData(bool bNext);
	virtual void OnLeavePage();	
	//根据不同的设备代码显示不同的页面
	virtual void SetDevIndex(int index);
protected:
	void OnInitial();
	virtual void InitCtrlInfo();
	void OnPaint(PAINT_STATUS status);
	void OnDestroy();
protected:
	DECLARE_MSG_MAP()
	unsigned long OnCheck();
	unsigned long OnScrollchange(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);
private:
	void SetMaix(unsigned char * pMaix,unsigned long w,unsigned long h);
protected:
	GUI::CRichTextCtrl	m_phoneRichTextCtrl;
	GUI::CCheckCtrl		m_checkCtrl;
	NAT_WIZARD_INFO     * m_pWzdInfo;

	unsigned char * m_pMaix;
	unsigned long m_maxW;//二维码绘图宽度
	unsigned long m_maxH;//二维码绘图高度
	int			  m_drawStartX;//二维码绘图开始X坐标
	int			  m_drawStartY;//二维码绘图开始Y坐标
	int			  m_devIndex;

	unsigned short		m_toprow;//滚屏显示的第一行
	GUI::CScrollCtrl	m_scroll_vPhone;
};

#endif


