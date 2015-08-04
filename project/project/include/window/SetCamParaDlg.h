/*
* Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
* Author       : xian shi wei
* Date         : 2014-10-27
* Name         : SetCamParaDlg.h
* Version      : 1.0
* Description  : 
* Modify Record:
*/

#ifndef __CFG_CAM__PARA_DLG_H__
#define __CFG_CAM__PARA_DLG_H__

#include "CheckCtrl.h"
#include "SliderCtrl.h"
#include "Static.h"
#include "Button.h"
#include "ComboCtrl.h"

#include "Dialog.h"
#include "NetProtocol.h"

typedef struct __camera__paramter_ 
{
	unsigned char   m_mirror;
	unsigned char   m_flip;
	short			m_chnn;
	unsigned long   m_rotato;
	AF_CONTROL_T    m_focus;
	LENS_DISTORTION_CONFIG m_lensDistortion;
}CAMERA_PARAMTER;

//class CCamFlit;
//class CCamMirror;
//class CCamRotaro;
//class CCamFocus;
//class CCamDistortion;

using namespace GUI;
class CSetCamParaDlg: public GUI::CDialog{
public:
	CSetCamParaDlg();
	~CSetCamParaDlg();

public:
	static const unsigned short CFG_DLG_LEFT_X = 10;	//距离窗口左边距离
	static const unsigned short CFG_DLG_RIGHT_X = 5;	//距离窗口右边界距离
	static const unsigned short CFG_DLG_TOP_Y = 15;	//距离窗口顶部距离
	static const unsigned short CFG_DLG_BOTTOM_Y = 8;	//窗口底部厚度
	static const unsigned short CFG_DLG_OFF_X = 5;	    //窗口元素横向偏移量
	static const unsigned short CFG_DLG_OFF_Y = 8;  	//窗口元素纵向偏移量
	static unsigned long  lRotato[4];

	static unsigned short GetWidth()	//本窗口需求宽度
	{
		return 200;
	}
	static unsigned short GetHeight()	//本窗口需求高度
	{
		unsigned short height = CFG_DLG_TOP_Y + 3*CFG_DLG_BOTTOM_Y + 8 * m_length[COMBO_HEIGHT] + 4 * CFG_DLG_OFF_Y + 3 * m_length[BTN_HEIGHT];

		return height < m_height ? height : m_height;
	}

	enum {
		WIDTH =  540,
		HEIGHT = 140,
	};
	enum {
		checkWidth = 20,
		checkhight = 20,
	};
	
public:
	void SetChnnInfo(const CAMERA_PARAMTER *pCamPara,int sel,int netChnn, int localChnn, int controlType = 0);
	void GetChnnPara(CAMERA_PARAMTER * pCamPara,int len);

protected:
	virtual void OnInitial();
	virtual void OnDestroy();
	void EndDialog(DLG_END_VALUE result);
    void SetCtrlData();
	void SetValue(int currChnn);
	void SetValueChangeChnnel(int currChnn);

	void SelectCh(const CAMERA_PARAMTER camPara,int chnn);
	/*void OnPaint(PAINT_STATUS status);
	void ShowPanel(bool bShow, bool bRefresh);*/

//DECLARE_MSG_MAP()
//	unsigned long OnClickChangeChnnBtn(unsigned long ctrlID);
//	unsigned long OnChangeChnnToNum( unsigned long keyVirtualNum);
//	unsigned long OnClickChnnAllBtn();
//
//	bool GetCurDisplayInfo(NET_DISPLAY_INFO& disp );
//	void ChangeToChnn(ULONGLONG CH, VIEW_SPLIT_MODE splitMode);
//	void SetChnnBtnMark(unsigned long chnn, unsigned long btnColor, bool brefrash);
//
//	unsigned long OnBtnHide();
//	unsigned long OnBtnExit();
DECLARE_MSG_MAP()
	unsigned long OnCkMirro();
	unsigned long OnCkFilp();
	unsigned long OnComboRotato();
	unsigned long OnSliderDistortion(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);
	unsigned long OnCheckDistortion();
	unsigned long OnAutoFocus();
	unsigned long OnFocusDec();
	unsigned long OnFocusAdd();
	unsigned long OnStop();
	/*unsigned long OnFocusDec(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);
	unsigned long OnFocusAdd(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);*/
	unsigned long OnFocusMode();
	unsigned long OnAlwaysKeepFocus();

	unsigned long OnZoomDec();
	unsigned long OnZoomAdd();
	
	bool ReleaseMsgBuff(MESSAGE_DATA &msgData);

	unsigned long OnChnnComboChange();
	unsigned long OnBtnOK();
	unsigned long OnBtnExit();

	unsigned long OnChangeValue();

protected:// UI 控件
	GUI::CStatic		m_stcChnn;
	GUI::CComboCtrl		m_comboChnn;

	GUI::CStatic    m_neDistortion;
	GUI::CStatic    m_valueDistortion;
	GUI::CStatic    m_neTotato;
	GUI::CStatic    m_stMirror;
	GUI::CStatic    m_stFlip;
	GUI::CStatic    m_stFocusMode;
	GUI::CStatic    m_stZoom;
	GUI::CStatic    m_stFocusTimeInterval;

	GUI::CButton		m_btnOK;
	GUI::CButton        m_btnRecover;
	GUI::CButton		m_btnExit;

	GUI::CCheckCtrl     m_ckMirror;
	GUI::CCheckCtrl     m_ckFilp;
	GUI::CButton        m_btnAutoFocus;
	GUI::CButton        m_btnFocusAdd;
	GUI::CButton        m_btnFocusDec;
	GUI::CButton        m_btnZoomAdd;
	GUI::CButton        m_btnZoomDec;
	GUI::CComboCtrl     m_comboRotato;
	GUI::CSliderCtrl    m_slDistortion;
	GUI::CCheckCtrl     m_chDistortion;

	GUI::CComboCtrl     m_comFocusMode;
	GUI::CComboCtrl     m_comFocusTimeInterval;

	LENS_DISTORTION     m_distortionOld;
	LENS_DISTORTION     m_distortionCur;

	unsigned char       m_ucLensDistortion;
	unsigned char       m_ucLensDistortionValue;
	unsigned char       m_curDistortionValue;

	bool				m_change;

protected:
	int    m_curSel;
	int    m_localChnn;
    int    m_netChnn;
	int    m_controlType;

	//保存原始现场的信息值	
	CAMERA_PARAMTER * m_pSetCamPara;       
	/*保存 临时改变的现场值  在该界面点击确认时把数据返回给上级目录，
	当直接退出时用来比较和m_pSetCamPara的差异，并恢复现场值到m_pSetCamPara中存储的状态 */
	CAMERA_PARAMTER * m_pSetCamParaChange;  
	                                       
	/*unsigned char   m_Mirror*/
};

//class CCamFlit{
//	CCamFlit():m_flit=0,m_flitChnn=-1{}
//public:
//	unsigned char GetFlit() const { 
//		return m_flit;
//	}
//	void SetFlit( unsigned char chTemp){ 
//		m_flit = chTemp;
//	}
//
//	unsigned short GetFlitChnn() const { 
//		return m_flitChnn;
//	}
//	void SetFlitChnn( short shTemp){ 
//		m_flitChnn= shTemp;
//	}
//private:
//	unsigned char	m_flit;
//	short  m_flitChnn;
//};
//
//class CCamMirror{
//	CCamMirror():m_Mirror=0,m_MirrorChnn=-1{}
//public:
//	unsigned char GetMirror() const { 
//		return m_Mirror;
//	}
//	void SetMirror( unsigned char chTemp){ 
//		m_Mirror = chTemp;
//	}
//
//	unsigned short GetMirrorChnn() const { 
//		return m_MirrorChnn;
//	}
//	void SetMirrorChnn( short shTemp){ 
//		m_MirrorChnn= shTemp;
//	}
//private:
//	unsigned char	m_Mirror;
//	short  m_MirrorChnn;
//};
//
//class CCamRotaro{
//
//};
//
//class CCamFocus{
//
//};
//
//class CCamDistortion{
//	CCamDistortion():memset(&m_distortion,0,sizeof(LENS_DISTORTION)){}
//public:
//	LENS_DISTORTION * GetDistortion() const{
//		return &m_distortion;
//	}
//	void * SetDistortion( LENS_DISTORTION * pLensTemp){
//		assert(NULL != pLensTemp);
//		m_distortion.chnn = pLensTemp->chnn;
//		strcpy(m_distortion.noUse,pLensTemp->noUse);
//		m_distortion.ucLensDistortion = pLensTemp->ucLensDistortion;
//		m_distortion.ucLensDistortionValue = pLensTemp->ucLensDistortionValue;
//	}
//private:
//	LENS_DISTORTION m_distortion;
//};

#endif