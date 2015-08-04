/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : hz
** Date         : 2009-08-01
** Name         : PTZViewDlg.h 
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/
#ifndef _PTZ_VIEW_DLG_H_
#define _PTZ_VIEW_DLG_H_

#include "Dialog.h"
#include "ComboCtrl.h"
#include "PtzControlDlg.h"
#include "MessageMan.h"
#include "PTZLiveCtrl.h"
#include "PTZTrackCtrl.h"
#include "PTZPresetCtrl.h"

using namespace GUI;

class CPTZViewDlg : public GUI::CDialog
{
public:
	CPTZViewDlg();
	~CPTZViewDlg();

	typedef enum _curr_ptzctrldlg_num
	{
		CURR_PTZCTRLDLG_NULL=0,
		CURR_PTZCTRLDLG_LIVE=1,
		CURR_PTZCTRLDLG_TRACK=2,
		CURR_PTZCTRLDLG_PRESET=3,
	}CURR_PTZCTRLDLG_NUM;

	void SetCtrlDlg(GUI::CDialog* pCtrlDlg, int width, int height,CURR_PTZCTRLDLG_NUM curCtrlDlg=CURR_PTZCTRLDLG_NULL );
	GUI::CDialog* GetCtrlDlg() { return m_pCtrlDlg; }
	static unsigned long GetPTZStatus( unsigned long chnn );			//获得PTZ当前的状态
	static ENUM_STRING_ID GetPtzCtrlStrID(unsigned long errPtz);			//获得PTZ失败原因的字符串ID 
	static inline void SetCanCtrlChnn(ULONGLONG chnn){m_ctrlChnn = chnn;}			//设置可以控制的PTZ通道
protected:
	DECLARE_MSG_MAP()
	void OnInitial();
	void OnDestroy();
	void OnPaint(PAINT_STATUS status);
	unsigned long OnBtnHide();
	unsigned long OnBtnExit();

	unsigned long OnZoom();
	unsigned long OnFocus();
	unsigned long OnIris();
	unsigned long OnSpeed();

	unsigned long OnAutoFocus();

	unsigned long OnClickChangeChnnBtn(unsigned long ctrlID);
	unsigned long OnChangeChnnToNum( unsigned long keyVirtualNum);
	unsigned long OnClickChnnAllBtn();
	
	unsigned long OnRefreshBtnCaption(unsigned long keyID);

	void HaveControlPTZ(unsigned long keyID);
	unsigned long OnPTZCtrl( unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos );

	void ChangeBtnCaption(bool bChnnChanged);
	void ChangeToChnn(ULONGLONG CH, VIEW_SPLIT_MODE splitMode);
	void SetChnnBtnMark(unsigned long chnn, unsigned long btnColor, bool brefrash);

	bool GetCurDisplayInfo(NET_DISPLAY_INFO& dispInfo);

	//---------------added 2011.7.30---------------------
	unsigned long OnPreCommand(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);
private:
	enum
	{
		PTZ_CTRL_NULL,
		PTZ_CTRL_ZOOM,
		PTZ_CTRL_FOCUS,
		PTZ_CTRL_IRIS,
		PTZ_CTRL_SPEED
	};

	void ShowPanel(bool bShow, bool bRefresh);

	//
#ifdef __DVR_PROFESSIONAL__
	enum
	{
		PTZ_MOUSE_CTRL_STOP,
		PTZ_MOUSE_CTRL_ING,
		PTZ_MOUSE_CTRL_START
	}; 

	int	m_mouseCtrlStatus;
	unsigned long m_xPosStart;
	unsigned long m_yPosStart;

	PTZ_CMD_TYPE PTZCmdType(unsigned long x1, unsigned long y1, unsigned long x2, unsigned long y2, unsigned long cx, unsigned long cy);
	unsigned long PTZCmdSpeed(unsigned long cx, unsigned long cy);
#endif

	bool			m_bShowPanel;
	unsigned long	m_ptzCtrlMode;

	CPtzControlDlg	m_ptzControlDlg;
	GUI::CButton	m_btnExit;
	GUI::CButton	m_btnHide;
	GUI::CRect		m_rectPanel;
	GUI::CButton    m_ptzAutoFcus;
	GUI::CDialog* m_pCtrlDlg;
	GUI::CRect m_rectCtrlDlg;
	enum { CTRLDLG_MAX_HEIGHT = 100 };
	
	CURR_PTZCTRLDLG_NUM m_CurrPtzCtrlDlgNum;
	bool m_bHaveCtrolPTZ;

	GUI::CButton	*m_pBtnChnn;
	unsigned long	m_channelTemp;
	unsigned long	m_groupCount;
	unsigned long	m_chnnChngWdth;
	static ULONGLONG m_ctrlChnn;

};

#endif//_PTZ_VIEW_DLG_H_

