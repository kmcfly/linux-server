/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : L
** Date         : 2009-08-18
** Name         : AudioControlDlg.h
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/

#ifndef _AUDIO_CONTROL_DLG_
#define _AUDIO_CONTROL_DLG_

#include "Dialog.h"
#include "Static.h"
#include "Button.h"
#include "ComboCtrl.h"
#include "SliderCtrl.h"
#include "CheckCtrl.h"
#include "ConfigEx.h"
#include "ConfigBlock.h"
#include "NetDeviceDefine.h"

using namespace GUI;

class CAudioControlDlg : public GUI::CDialog
{
public:
	CAudioControlDlg();
	~CAudioControlDlg();

	bool isMute(){return m_bMute;}
public:
	static unsigned short GetX( unsigned long xPos );
	static unsigned short GetY( unsigned long yPos );
	static unsigned short GetWidth();	//本窗口需求宽度
	static unsigned short GetHeight();	//本窗口需求高度

public:
	static unsigned short DLG_LEFT_X;		//距离窗口左边距离
	static unsigned short DLG_RIGHT_X;		//距离窗口右边界距离
	static unsigned short DLG_TOP_Y;		//距离窗口顶部距离
	static unsigned short DLG_BOTTOM_Y;		//窗口底部厚度
	static unsigned short DLG_OFF_X;		//窗口元素横向偏移量
	static unsigned short DLG_OFF_Y;		//窗口元素纵向偏移量
	static unsigned short DLG_TOOLBAR_HEIGHT;//工具条高度

protected:
	void OnInitial();
	void OnDestroy();
	void OnChangeFocus(unsigned long oldID, unsigned long newID);
	void OnOutOfDlg(unsigned long keyID);
	void OnPaint(PAINT_STATUS status);
	int LoadAudioCfg();
	int SaveAudioCfg();
	void GetItemList(std::vector<NCFG_ITEM_ID>& itemList) const;
	bool ShowData(NCFG_ITEM_ID cfgID, const BYTE* pData, int len, bool bRefresh);
	bool SetData(NCFG_ITEM_ID cfgID, BYTE*& pData, unsigned long& len)  const;
protected:
	DECLARE_MSG_MAP()
	unsigned long OnBtnExit();
	unsigned long OnClickComboForChangeChnn();
	unsigned long OnCkForMute();
	unsigned long OnCkForAuto();
	unsigned long OnSlider(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);

	void EndDialog(DLG_END_VALUE result);

protected:
	GUI::CStatic		m_scVolume;
	GUI::CStatic		m_scChnn;

	GUI::CSliderCtrl	m_sliderVolume;
	GUI::CComboCtrl		m_comboChnn;

	GUI::CCheckCtrl		m_ckMute;
	GUI::CCheckCtrl		m_ckAuto;

	GUI::CButton		m_btnExit;
	//GUI::CStatic		m_scMute;

	bool                m_bModify;
	bool				m_bMute;

	unsigned long		m_volume;

	static bool         m_bAutoDefa;
	int						m_netDevNum;
	ND_DEV_INFO	*m_pNetDevInfo;
};


#endif




