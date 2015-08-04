/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : l
** Date         : 2009-08-13
** Name         : CfgLiveColorDlg.h
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/

#ifndef _CFG_DEV_COLOR_DLG_
#define _CFG_DEV_COLOR_DLG_

#include "Dialog.h"
#include "Static.h"
#include "ComboCtrl.h"
#include "SliderCtrl.h"
#include "Button.h"
#include "NumEditCtrl.h"
#include "MessageMan.h"
#include "NetProtocol.h"

using namespace GUI;

class CCfgDevColorDlg : public GUI::CDialog
{
public:
	CCfgDevColorDlg();
	~CCfgDevColorDlg();

public:
	static unsigned short GetWidth();	//本窗口需求宽度
	static unsigned short GetHeight();	//本窗口需求高度
public:
	static unsigned short CFG_DLG_LEFT_X;		//距离窗口左边距离
	static unsigned short CFG_DLG_RIGHT_X;		//距离窗口右边界距离
	static unsigned short CFG_DLG_TOP_Y;		//距离窗口顶部距离
	static unsigned short CFG_DLG_BOTTOM_Y;		//窗口底部厚度
	static unsigned short CFG_DLG_OFF_X;		//窗口元素横向偏移量
	static unsigned short CFG_DLG_OFF_Y;		//窗口元素纵向偏移量

public:
	//
	void SetDevColorInfo(unsigned long devColor);

	void GetDevColorInfo(unsigned long &devColor, int &dev);

	//void RecoveryChnnColor( const unsigned long *pClrChnn, const unsigned long *pClrChnnChng, int maxChnn );
	
protected:
	virtual void OnInitial();			//初始化
	virtual void OnDestroy();
	void EndDialog(DLG_END_VALUE result);

protected:
	void SetCtrlData();					//设置控件内容

protected:
	unsigned long DoSliderChange(GUI::CSliderCtrl& slider, GUI::CStatic& numSC);
	void SetColorData(unsigned long color,GUI::CSliderCtrl& slider, GUI::CStatic& numSC,bool bRefresh=false);

protected:// msg
	DECLARE_MSG_MAP()

	unsigned long OnSliderBrightChange(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);
	unsigned long OnSliderChromaChange(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);
	unsigned long OnSliderSaturationChange(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);
	unsigned long OnSliderContrastChange(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);

	unsigned long OnBtnBrightAdd();
	unsigned long OnBtnChromaAdd();
	unsigned long OnBtnSaturationAdd();
	unsigned long OnBtnContrastAdd();

	unsigned long OnBtnBrightDec();
	unsigned long OnBtnChromaDec();
	unsigned long OnBtnSaturationDec();
	unsigned long OnBtnContrastDec();

	unsigned long OnChnnComboChange();
	unsigned long OnBtnRecover();
	unsigned long OnBtnOK();
	unsigned long OnBtnExit();

	void ChangeChnnColor();
	void SaveLiveColor();

	void SelectCh(unsigned long color); //切换通道的时候切换显示模式并做颜色改变

protected:// UI 控件

	GUI::CStatic		m_stcBright;
	GUI::CStatic		m_stcChroma;
	GUI::CStatic		m_stcSaturation;
	GUI::CStatic		m_stcContrast;

	GUI::CStatic	m_neBright;
	GUI::CStatic	m_neChroma;
	GUI::CStatic	m_neSaturation;
	GUI::CStatic	m_neContrast;

	GUI::CSliderCtrl	m_slBright;
	GUI::CSliderCtrl	m_slChroma;
	GUI::CSliderCtrl	m_slSaturation;
	GUI::CSliderCtrl	m_slContrast;

	GUI::CButton		m_btnRecover;
	GUI::CButton		m_btnOK;

	GUI::CButton		m_btnExit;

	GUI::CButton		m_btnBrightAdd;
	GUI::CButton		m_btnChromaAdd;
	GUI::CButton		m_btnSaturationAdd;
	GUI::CButton		m_btnContrastAdd;

	GUI::CButton		m_btnBrightDec;
	GUI::CButton		m_btnChromaDec;
	GUI::CButton		m_btnSaturationDec;
	GUI::CButton		m_btnContrastDec;

protected:// 变量
	unsigned long		m_color;						//保存原始现场的颜色信息值
	unsigned long		m_pColorChange;					//保存临时改变的现场颜色值。
														//在该界面点击确认时把数据返回给上级目录，当直接退出时用来比较和m_color的差异，并恢复现场颜色到m_color中存储的状态
	//int					m_maxChnn;
	//int					m_curSel;
	CMessageMan*		m_pMsgMan;

	//int					m_outsideFlag;					//外面调节色彩的标记
	//bool				m_outModifyAll;					//外面调色彩的时候是否改变了all

	int					m_lastSliderValue[4];			//上一次的滑动条颜色值
};

#endif
