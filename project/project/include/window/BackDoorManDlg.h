/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : L
** Date         : 2010-12-03
** Name         : BackDoorManDlg.h
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/

#ifndef _BACK_DOOR_MAN_DLG_H_
#define _BACK_DOOR_MAN_DLG_H_

#include "Dialog.h"
#include "TabCtrl.h"
#include "ListCtrl.h"
#include "Button.h"
#include "WndStructDef.h"
#include "LocalDevice.h"

typedef struct _row_col_pos_ 
{
	unsigned short row;
	unsigned short col;
}ROW_COL_POS;

//////////////////////////////////////////////////////////////////////////

class CBackDoorLogoImgDlg : public GUI::CDialog
{
public:
	CBackDoorLogoImgDlg();
	virtual ~CBackDoorLogoImgDlg();

	static bool HasBackDoorInfo(const WND_BACK_DOOR_INFO& info);

	void SetBackDoorInfo(WND_BACK_DOOR_INFO* pInfo) {m_pBackDoorInfo = pInfo;}

protected:
	void OnInitial();

	bool CheckBootImageFile(char *pFileName, char *pRetFilePathName, int len);
	void UmountUsb();

	DECLARE_MSG_MAP()
	unsigned long OnClickList(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);

	WND_BACK_DOOR_INFO* m_pBackDoorInfo;

	GUI::CListCtrl		m_listCtrl;

	enum BTN_INDEX
	{
		BTN_BOOT_IMG,
		BTN_IELOGO,
		BTN_DVRLOGO,
		BTN_DVRLOGOTRANS,
		BTN_COUNT,
	};

	ROW_COL_POS			m_posBtn[BTN_COUNT];
};

//////////////////////////////////////////////////////////////////////////

class CBackDoorDdnsDlg :  public GUI::CDialog
{
public:
	CBackDoorDdnsDlg();
	virtual ~CBackDoorDdnsDlg();

	static bool HasBackDoorInfo(const WND_BACK_DOOR_INFO& info);

	void SetBackDoorInfo(WND_BACK_DOOR_INFO* pInfo) {m_pBackDoorInfo = pInfo;}

protected:
	void OnInitial();

	DECLARE_MSG_MAP()
	unsigned long OnClickList(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);

	WND_BACK_DOOR_INFO* m_pBackDoorInfo;

	GUI::CListCtrl		m_listCtrl;

	enum DDNS_COMPANY
	{
		DDNS_COMPANY_NULL		= 0,
		DDNS_COMPANY_DEFAULT	= 1,
		DDNS_COMPANY_US02		= 2,
		DDNS_COMPANY_US09_SUB1	= 9,
		DDNS_COMPANY_US09_SUB2	= 10,
		DDNS_COMPANY_BR27		= 27,
		DDNS_COMPANY_US61		= 61,
		DDNS_COMPANY_TW01		= 21,
		DDNS_COMPANY_TH13		= 13,
		DDNS_COMPANY_MX15		= 100,
		DDNS_COMPANY_VN11		= 101,
	};

	enum BTN_INDEX
	{
		BTN_DDNS,
		BTN_KEYBOARD,
		BTN_COUNT,
	};
	ROW_COL_POS			m_posBtn[BTN_COUNT];
};

//////////////////////////////////////////////////////////////////////////

class CBackDoorMacDlg : public GUI::CDialog
{
public:
	CBackDoorMacDlg();
	virtual ~CBackDoorMacDlg();

	static bool HasBackDoorInfo(const WND_BACK_DOOR_INFO& info);

	void SetBackDoorInfo(WND_BACK_DOOR_INFO* pInfo) {m_pBackDoorInfo = pInfo;}

protected:
	void OnInitial();

	bool CheckHardwareVersion(char *pVersion);
	bool GetMac(char *pInput, unsigned char *pMacaddr);

	DECLARE_MSG_MAP()
	unsigned long OnClickList(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);

	WND_BACK_DOOR_INFO* m_pBackDoorInfo;

	GUI::CListCtrl		m_listCtrl;



	enum BTN_INDEX
	{
		BTN_MAC,
		BTN_HARDWARE,
		BTN_COUNT,
	};

	ROW_COL_POS			m_posBtn[BTN_COUNT];
};

//////////////////////////////////////////////////////////////////////////

class CBackDoorLangDlg : public GUI::CDialog
{
public:
	CBackDoorLangDlg();
	virtual ~CBackDoorLangDlg();

	static bool HasBackDoorInfo(const WND_BACK_DOOR_INFO& info);

	void SetBackDoorInfo(WND_BACK_DOOR_INFO* pInfo) {m_pBackDoorInfo = pInfo;}

protected:
	void OnInitial();

	DECLARE_MSG_MAP()
	unsigned long OnClickList(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);

	WND_BACK_DOOR_INFO* m_pBackDoorInfo;

	GUI::CListCtrl		m_listCtrl;

	enum LIST_ROW
	{
		ROW_CRYTO_INFO = 0,
		ROW_COMPANY = 1,
		ROW_LANGUAGE = 2,		
	};

	enum BTN_INDEX
	{
		BTN_SEL_LAN_ALL,
		BTN_SEL_LAN_CAL,
		BTN_SAVE,		
		BTN_COUNT,
	};
	ROW_COL_POS			m_posBtn[BTN_COUNT];
};

//////////////////////////////////////////////////////////////////////////

class CBackDoorManufacturerDlg : public GUI::CDialog
{
public:
	CBackDoorManufacturerDlg();
	virtual ~CBackDoorManufacturerDlg();

	static bool HasBackDoorInfo(const WND_BACK_DOOR_INFO& info);

	void SetBackDoorInfo(WND_BACK_DOOR_INFO* pInfo) {m_pBackDoorInfo = pInfo;}

protected:
	void OnInitial();

	DECLARE_MSG_MAP()
	unsigned long OnClickList(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);

	WND_BACK_DOOR_INFO* m_pBackDoorInfo;

	GUI::CListCtrl		m_listCtrl;



	enum BTN_INDEX
	{
		BTN_MANUFACTURER,
		BTN_COUNT,
	};

	ROW_COL_POS			m_posBtn[BTN_COUNT];
};

//////////////////////////////////////////////////////////////////////////

class CBackDoorManDlg : public GUI::CDialog
{
public:
	CBackDoorManDlg();
	virtual ~CBackDoorManDlg();

	void SetBackDoorInfo(const WND_BACK_DOOR_INFO& info);

protected:
	void OnInitial();

	WND_BACK_DOOR_INFO  m_backDoorInfo;

	GUI::CTabCtrl		m_tabCtrl;

	CBackDoorLogoImgDlg	m_pageLogoDlg;
	CBackDoorDdnsDlg	m_pageDdnsDlg;
	CBackDoorMacDlg		m_pageMacDlg;
	CBackDoorLangDlg	m_pageLangDlg;
	CBackDoorManufacturerDlg	m_pageComanyDlg;
};


#endif

//end


