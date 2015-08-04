/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : l
** Date         : 2009-08-12
** Name         : CfgLiveDlg.h
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/

#ifndef _CFG_LIVE_DLG_H_
#define _CFG_LIVE_DLG_H_

#include "Dialog.h"
#include "ListCtrl.h"
#include "CfgDlg.h"
#include "CfgLiveColorDlg.h"
#include "CheckCtrl.h"

#include <map>

using namespace GUI;

class CCfgLiveDlg : public CCfgDlg
{
public:
	CCfgLiveDlg();
	~CCfgLiveDlg();

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
	DLG_END_VALUE PopupLiveColorDlg( int x, int y, int cx, int cy, int chnn );
	unsigned long  OnCheck();

protected:// UI
	GUI::CListCtrl		m_cfgLiveList;
	GUI::CListCtrl		m_cfgLiveListAll;
	CCfgLiveColorDlg	m_cfgLiveColorDlg;

	GUI::CStatic		m_scAll;
	GUI::CCheckCtrl		m_liveRecStatus;

	std::map<int, int>  m_rowChnnMap;

	enum LIST_COL
	{
		COL_CHNN,
		COL_CAMERA_NAME,
		//COL_COVER,
		COL_SHOW_NAME,
		COL_COLOR,
	};

protected:
	//�������޿ؼ���Ӧֵ(���簴ť)ʱ,ʹ���ڴ��������
	//IsChange() �����ĵ�������,����SetDataʱ��ȡ���ڴ�����
	unsigned long* m_colorChnn;
	unsigned long* m_pColorChnnChange;						//���CfgLiveColorDlg.cpp�������������ݣ����ڻָ�ʱ�������ļ������ݶԱ�
};

#endif
