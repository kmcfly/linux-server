/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : zxx
** Date         : 2010-07-20
** Name         : CWzdRecordStreamDlg.cpp
** Version      : 1.0
** Description  : 
** Modify Record:
作用:录像码流的向导
***********************************************************************/
#ifndef _WZD_RECORD_STREAMDLG_H_
#define _WZD_RECORD_STREAMDLG_H_
#include "Dialog.h"
#include "WndStructDef.h"
#include "WzdPageDlg.h"
#include "ListCtrl.h"
#include "Static.h"
#include "FrameRate.h"
#include "NetDeviceDefine.h"
#include "IPCFrameEncode.h"

#include <map>


using namespace GUI;
class CWzdRecordStreamDlg:public CWzdPageDlg
{
public:
	CWzdRecordStreamDlg();
	~CWzdRecordStreamDlg();

	//此页面是否使用
	void SetUsed(bool bUsed = true);
	bool IsUsed();

	//在获取数据后设置控件信息
	virtual void InitCtrlInfo();

	//设置向导信息
	virtual void SetWzdInfo(WIZARD_INFO* pWzdInfo);

	//显示向导数据
	virtual void ShowWzdData(bool bRefresh);

	//页面完成读取向导信息
	virtual void GetWzdData(bool bNext = true);
	
protected:
	virtual void OnInitial();
	void OnPostInitial();

	void OnDestroy();
protected:
	DECLARE_MSG_MAP()
	unsigned long OnClickList(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);
	unsigned long OnClickListAll(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);


protected:
	WIZARD_INFO* m_pWzdInfo;
	bool m_bUsed;
private:
	
	GUI::CListCtrl	m_cfgList;
	GUI::CStatic	m_cfgScAll;
	GUI::CListCtrl	m_cfgListCtrlAll;

	GUI::CStatic	m_cfgScRemaining;
	GUI::CStatic	m_cfgScRemainNum;
	enum LIST_COL
	{
		COL_CHNN = 0,
		COL_RESOLUTION,
		COL_RATE,
#ifdef __DVR_PROFESSIONAL__
		COL_STREAM_TYPE,
#endif
		COL_QUALITY,
#ifdef __DVR_PROFESSIONAL__
		COL_MAX_BIT_STREAM,
#endif
	};
private:
	unsigned long                 m_channelNum;
	CFrameRate		              m_frameRate;
	CIPCFrameEncode				  m_ipcFrameEncode;
	unsigned long                 m_videoFormat;
	unsigned long                 m_maxChnnRate;
	unsigned long                 m_totalRate;	
	unsigned long                 m_defauChnnRateNum;
	unsigned long                 m_videoResolutionMask;
	unsigned long                 m_defauResolution;
	unsigned long                 m_videoEncodeMode;

	unsigned long                 m_bVideoSizeSpecial;
	ULONGLONG                     m_videoSizeCIFChnnBits;
	ULONGLONG                     m_videoSizeD1ChnnBits;
private:
	void AddQualityItem(unsigned long *pQualityValue, unsigned long count, CComboCtrl *pCombo);
	void AddResolutionItem(unsigned long dwResolution, CComboCtrl *pCombo, bool bAddD1);
	void AddEncodeModeItem(unsigned long videoEncodeMode, CComboCtrl *pCombo);
	void AddBitStreamItem(unsigned long *pBitValue, unsigned long count, CComboCtrl *pCombo,unsigned long resolution);
	bool IsInQualityLevel(unsigned long value);
	bool IsInBitStreamRange(unsigned long value);
	bool IsMixDvr();
	void AddMIxBitStreamItem(CComboCtrl *pCombo, unsigned long resolution);
private:
	ENCODE_INFO_EX m_infoEx[32];

	ND_DEV_INFO		*m_pNetDevInfo;
	int							m_netDevNum;

	std::map<int, int>				m_rowChnnMap;

	NCFG_IPC_SUPPORT_ENCODE		m_ipcSupportEncode;

	SUPPORT_ENC_INFO			m_supportEncInfo[128];
	CHECK_ENC_INFO				m_checkEncInfo[128];
};
#endif
