/*
* Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
* Author       : xian shi wei
* Date         : 2014-10-25
* Name         : CfgCameraParamDlg.h
* Version      : 1.0
* Description  : 
* Modify Record:
1:
*/

#ifndef __CFG_CAMERA__PARAM_DLG_H__
#define __CFG_CAMERA__PARAM_DLG_H__

#include "Dialog.h"
#include "ListCtrl.h"
#include "CheckCtrl.h"
#include "CfgDlg.h"

#include <map>

#include "SetCamParaDlg.h"

using namespace GUI;

class CCfgCameraParamDlg : public CCfgDlg{
public:
	CCfgCameraParamDlg():m_pCamPara(NULL),m_pCamParaChange(NULL),m_buffLen(0){};
	~CCfgCameraParamDlg(){};

protected:
    void OnInitial();
	void OnDestroy();

	void SetTip();
	void OnChangeFocus(unsigned long oldID, unsigned long newID);

public:
	void UpdateView(bool bRefresh);
	unsigned long RecoverChange();

protected:
	bool ShowData(NCFG_ITEM_ID cfgID, const BYTE* pData, int len, bool bRefresh);
	bool SetData(NCFG_ITEM_ID cfgID, BYTE*& pData, int& len)  const;

protected:
DECLARE_MSG_MAP()
	unsigned long OnClickList(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);
	unsigned long OnClickListAll(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);
	DLG_END_VALUE PopupCamParaDlg( int x, int y, int cx, int cy, int chnn );
	/*unsigned long  OnCheck();*/

protected:// UI
	GUI::CListCtrl		m_cfgCamParamList;
	GUI::CListCtrl		m_cfgCamParamListAll;

	GUI::CStatic		m_scAll;
	GUI::CCheckCtrl		m_RecStatus;

	std::map<int, int>  m_rowChnnMap;
	CSetCamParaDlg		m_cfgSetParaDlg;
    unsigned int        m_buffLen;

	enum LIST
	{
		CAM_CHNN,
		CAM_PARA,
		CAM_FLIP,
		CAM_MIRROR,
		CAM_ROTARO,
		CAM_DISTORTION,
		CAM_FOCUS,
	};

private:
	//界面上无控件对应值(比如按钮)时,使用内存区域替代
	//IsChange() 函数的调用链中,会在SetData时读取本内存数据
	CAMERA_PARAMTER *m_pCamPara;
	CAMERA_PARAMTER *m_pCamParaChange;   //存放SetCamParaDlg.cpp界面操作后的数据，用于恢复时和配置文件中数据对比
				
};

#endif