/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : L
** Date         : 2009-10-12
** Name         : CfgRecordStreamDlg.h
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/

#ifndef _CFG_RECORD_STREAM_H_
#define _CFG_RECORD_STREAM_H_

#include "Dialog.h"
#include "ListCtrl.h"
#include "Static.h"
#include "CfgDlg.h"
#include "RadioCtrl.h"
#include "FrameRate.h"
#include "NetDeviceDefine.h"
#include "IPCFrameEncode.h"

#include <map>

using namespace GUI;

class CCfgRecordStreamDlg : public CCfgDlg
{
public:
	CCfgRecordStreamDlg();
	~CCfgRecordStreamDlg();

protected:
	void OnInitial();
	void OnPostInitial();
	void OnDestroy();
	void SetTip();
	//void OnChangeFocus(unsigned long oldID, unsigned long newID);

public:
	void UpdateView(bool bRefresh);
	void DoOther();
	void EndOther();
protected:
	bool ShowData(NCFG_ITEM_ID cfgID, const BYTE* pData, int len, bool bRefresh);
	bool SetData(NCFG_ITEM_ID cfgID, BYTE*& pData, int& len)  const;

protected:
	DECLARE_MSG_MAP()
	unsigned long OnClickList(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);
	unsigned long OnClickListAll(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);

protected:
	void AddResolutionItem(unsigned long dwResolution, CComboCtrl *pCombo,bool bAddD1);
	void AddQualityItem(unsigned long *pQualityValue, unsigned long count, CComboCtrl *pCombo);
	void AddEncodeModeItem(unsigned long videoEncodeMode, CComboCtrl *pCombo);
	void AddBitStreamItem(unsigned long *pBitValue, unsigned long count, CComboCtrl *pCombo, unsigned long resolution = 0, int channel=-1);
	bool IsInBitStreamRange(unsigned long value);
	bool IsInQualityLevel(unsigned long value);
	void AddMIxBitStreamItem(CComboCtrl *pCombo, unsigned long resolution);
private:
	bool IsMixDvr();


	GUI::CListCtrl				m_cfgListCtrl;
	GUI::CListCtrl				m_cfgListCtrlAll;

	GUI::CStatic				m_cfgScAll;
	GUI::CRadioGroup	m_rdGroup;
	GUI::CRadioCtrl		m_rdEvent;
	GUI::CRadioCtrl		m_rdTime;

	GUI::CStatic				m_cfgScRemaining;
	GUI::CStatic				m_cfgScRemainNum;
protected:
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
	unsigned long 				  m_maxNetChnnRate;
	unsigned long                 m_totalRate;	
	unsigned long                 m_defauChnnRateNum;
	unsigned long                 m_videoResolutionMask;
	unsigned long                 m_defauResolution;
	unsigned long                 m_videoEncodeMode;
	unsigned long                 m_defaVideoEncodeMode;
	unsigned long                 m_qualityLevelNum;
	unsigned long                 *m_pQualityLevelValue;
	unsigned long                 m_defauQualityLevel;
	unsigned long                 m_bitStreamCount;
	unsigned long                 *m_pBitStreamValue;
	unsigned long                 m_defaBitStreamValue;

	unsigned long                 m_bVideoSizeSpecial;
	ULONGLONG                     m_videoSizeCIFChnnBits;
	ULONGLONG                     m_videoSizeD1ChnnBits;
	
	ND_DEV_INFO					  *m_pNetDevInfo;
	int							  m_netDevNum;

	std::map<int, int>				m_rowChnnMap;

	NCFG_IPC_SUPPORT_ENCODE			m_ipcSupportEncode;

	unsigned long					m_orignLowBitRate;
	bool							m_bModified;

	//混合型dvr使用
	CHANNEL_RECP * m_pChnnRecp;
	VS_SUPBRR *m_pVSBit;

	NET_CHANNEL_ENC_INFO			m_netChennelEncInfo[64];

	SUPPORT_ENC_INFO			m_supportEncInfo[128];
	CHECK_ENC_INFO				m_checkEncInfo[128];
};


#endif
