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
	static unsigned short GetWidth();	//������������
	static unsigned short GetHeight();	//����������߶�

public:
	static unsigned short DLG_LEFT_X;		//���봰����߾���
	static unsigned short DLG_RIGHT_X;		//���봰���ұ߽����
	static unsigned short DLG_TOP_Y;		//���봰�ڶ�������
	static unsigned short DLG_BOTTOM_Y;		//���ڵײ����
	static unsigned short DLG_OFF_X;		//����Ԫ�غ���ƫ����
	static unsigned short DLG_OFF_Y;		//����Ԫ������ƫ����
	static unsigned short DLG_TOOLBAR_HEIGHT;//�������߶�

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




