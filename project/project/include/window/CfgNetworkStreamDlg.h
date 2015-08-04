/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : l
** Date         : 2009-08-14
** Name         : CfgNetworkStreamDlg.h
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/

#ifndef _CFG_NETWORK_STREAM_DLG_H_
#define _CFG_NETWORK_STREAM_DLG_H_

#include "Dialog.h"
#include "ListCtrl.h"
#include "CfgDlg.h"
#include "FrameRate.h"
#include "IPCFrameEncode.h"
#include <map>

using namespace GUI;

class CCfgNetworkStreamDlg : public CCfgDlg
{
public:
	CCfgNetworkStreamDlg();
	~CCfgNetworkStreamDlg();

protected:
	void OnInitial();
	void OnPostInitial();
	void OnDestroy();
	void DoOther();
	void EndOther();
public:
	void UpdateView(bool bRefresh);

protected:
	bool ShowData(NCFG_ITEM_ID cfgID, const BYTE* pData, int len, bool bRefresh);
	bool SetData(NCFG_ITEM_ID cfgID, BYTE*& pData, int& len)  const;

protected:
	DECLARE_MSG_MAP()
	unsigned long OnClickList(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);
	unsigned long OnClickListAll(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);
private:	
	void AddResolutionItem(unsigned long dwResolution, CComboCtrl *pCombo);

	void AddSubResolutionItem(SUPPORT_SUB_ENC_INFO * pSupportSubEncInfo, CComboCtrl *pCombo);
	bool AddSubMixBitStreamItem(CComboCtrl *pCombo, unsigned int channel, unsigned long resolution,unsigned long *pMaxBitStream=NULL);

	void AddQualityItem(unsigned long *pQualityValue, unsigned long count, CComboCtrl *pCombo);
	void AddEncodeModeItem(unsigned long videoEncodeMode, CComboCtrl *pCombo);
	void AddBitStreamItem(unsigned long *pBitValue, unsigned long count, CComboCtrl *pCombo, unsigned long ulMaxBirRate=768);
	void AddBitStreamLocalItem(unsigned long *pBitValue, unsigned long count, CComboCtrl *pCombo, unsigned long resolution);
	bool IsInBitStreamRange(unsigned long value);
	bool IsInQualityLevel(unsigned long value);
	void ChangeNetEncodeInfo(ULONGLONG &CH, unsigned long ulToChnn, unsigned long &toVideoSize, bool bRefresh);
protected:
	GUI::CListCtrl			m_cfgNetworkStreamList;
	GUI::CListCtrl			m_cfgListAll;
	GUI::CStatic				m_cfgScAll;
	GUI::CStatic				m_cfgScRemaining;
	GUI::CStatic				m_cfgScRemainNum;
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
	//CIPCFrameEncode				  m_ipcFrameEncode;
	unsigned long                 m_videoFormat;
	unsigned long                 m_maxChnnRate;
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
	unsigned long					m_localVideoInputNum;
	unsigned long					m_netVideoInputNum;

	unsigned long				  m_reslutionNum;

	//NCFG_IPC_SUPPORT_ENCODE			m_ipcSupportEncode;

	std::map<int, int>				m_rowChnnMap;

	NET_CHANNEL_ENC_INFO			m_netChennelEncInfo[128];
	CHECK_ENC_INFO					m_checkEncInfo[128];
	SUPPORT_SUB_ENC_INFO				*m_supportSubEncInfo;
};


#endif