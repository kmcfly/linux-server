/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : ZXX
** Date         : 2012-10-17
** Name         : WzdUS02SelectDeviceDlg.h
** Version      : 1.0
** Description  : 向导页面对话框
** Modify Record:
***********************************************************************/

#ifndef _WZD_US02_SELECTDEVICE_DLG_H_
#define _WZD_US02_SELECTDEVICE_DLG_H_
#include "Dialog.h"
#include "WzdUS02PageDlg.h"
#include "RichTextCtrl.h"
#include "RadioCtrl.h"
#include "CheckCtrl.h"
using namespace GUI;

class CWzdUS02SelectDeviceDlg:public CWzdUS02PageDlg
{
public:
	CWzdUS02SelectDeviceDlg();
	~CWzdUS02SelectDeviceDlg();

	void SetWzdInfo(US02_WzdInfo* pWzdInfo);
	void GetWzdData(bool bNext);
	void ShowWzdData(bool bRefresh);
protected:
	void OnInitial();
	void OnDestroy();
protected:
	DECLARE_MSG_MAP()
	unsigned long OnClickRadio();
	unsigned long OnCheck();
private:
	GUI::CRichTextCtrl  m_richTip;
	GUI::CRichTextCtrl	m_richAnotherTip;
	GUI::CRadioGroup	m_rdGroup;
	GUI::CRadioCtrl		m_rdApple;
	GUI::CRadioCtrl		m_rdAndroid;
	GUI::CCheckCtrl		m_checkCtrl;				//下次是否显示向导

	unsigned short		m_curMobileSelect;
	US02_WzdInfo		*m_pWzdInfo;
	enum RADIO_ROW
	{
		ROW_APPLE,
		ROW_ANDROID,
	};
	enum Mobile_TYPE
	{
		MOBILE_APPLE,
		MOBILE_ANDROID,
	};

};

#endif

