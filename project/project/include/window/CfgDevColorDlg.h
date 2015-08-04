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
	static unsigned short GetWidth();	//������������
	static unsigned short GetHeight();	//����������߶�
public:
	static unsigned short CFG_DLG_LEFT_X;		//���봰����߾���
	static unsigned short CFG_DLG_RIGHT_X;		//���봰���ұ߽����
	static unsigned short CFG_DLG_TOP_Y;		//���봰�ڶ�������
	static unsigned short CFG_DLG_BOTTOM_Y;		//���ڵײ����
	static unsigned short CFG_DLG_OFF_X;		//����Ԫ�غ���ƫ����
	static unsigned short CFG_DLG_OFF_Y;		//����Ԫ������ƫ����

public:
	//
	void SetDevColorInfo(unsigned long devColor);

	void GetDevColorInfo(unsigned long &devColor, int &dev);

	//void RecoveryChnnColor( const unsigned long *pClrChnn, const unsigned long *pClrChnnChng, int maxChnn );
	
protected:
	virtual void OnInitial();			//��ʼ��
	virtual void OnDestroy();
	void EndDialog(DLG_END_VALUE result);

protected:
	void SetCtrlData();					//���ÿؼ�����

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

	void SelectCh(unsigned long color); //�л�ͨ����ʱ���л���ʾģʽ������ɫ�ı�

protected:// UI �ؼ�

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

protected:// ����
	unsigned long		m_color;						//����ԭʼ�ֳ�����ɫ��Ϣֵ
	unsigned long		m_pColorChange;					//������ʱ�ı���ֳ���ɫֵ��
														//�ڸý�����ȷ��ʱ�����ݷ��ظ��ϼ�Ŀ¼����ֱ���˳�ʱ�����ȽϺ�m_color�Ĳ��죬���ָ��ֳ���ɫ��m_color�д洢��״̬
	//int					m_maxChnn;
	//int					m_curSel;
	CMessageMan*		m_pMsgMan;

	//int					m_outsideFlag;					//�������ɫ�ʵı��
	//bool				m_outModifyAll;					//�����ɫ�ʵ�ʱ���Ƿ�ı���all

	int					m_lastSliderValue[4];			//��һ�εĻ�������ɫֵ
};

#endif
