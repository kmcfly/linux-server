/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author		: L
** Date			: 2009-10-19
** Name			: DwellModeDlgNXN.h
** Version		: 1.0
** Description	: 
** Modify Record:
1:
***********************************************************************/

#ifndef _DWELL_MODE_DLG_NXN_H_
#define _DWELL_MODE_DLG_NXN_H_

#include "Wnd.h"
#include "Dialog.h"
#include "ScrollDialog.h"
#include "ComboCtrl.h"

using namespace GUI;

class CDwellModeDlgNXN : public GUI::CScrollDialog
{
public:
	CDwellModeDlgNXN();
	~CDwellModeDlgNXN();

	enum LAYOUT_TYPE
	{
		TYPE_1X1 = 0,
		TYPE_2X2,
		TYPE_2X3,
		TYPE_3X3,
		TYPE_4X4,
		TYPE_5X5,
		TYPE_6X6,
	};

public:
	void SetLayoutType(LAYOUT_TYPE type, bool bRefresh);

	void SetMaxChnn(int max, int localNum) {m_maxChnn=max;m_localChnnNum=localNum;}
	void SetChnnData(unsigned long* pChnn, int numChnn, bool bRefresh = false);
protected:
	void InitComboChnn(GUI::CComboCtrl& combo );
	void InitCombo2Chnn(GUI::CComboCtrl& combo );

	//布局窗口
	void AutoLayout(LAYOUT_TYPE type);
	void AutoSetNXN(int row, int col);

protected:
	void OnInitial();
	void OnChangeFocus(unsigned long oldID, unsigned long newID);
	void OnPaint(PAINT_STATUS status);

protected:
	GUI::CComboCtrl*	m_comboChnn[72];

private:
	LAYOUT_TYPE			m_layoutType;
	int					m_maxChnn;
	int					m_localChnnNum;	//本地通道数
};

#endif
