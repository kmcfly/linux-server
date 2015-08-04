/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : zxx
** Date         : 2012-11-19
** Name         : ShanghaiCfgDailySelectDialog.h
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/

#ifndef __SHANGHAI_CFG_DAILYSELECT_DIALOG_H_
#define __SHANGHAI_CFG_DAILYSELECT_DIALOG_H_

#include "Dialog.h"
#include "Static.h"
#include "TimeCtrl.h"
#include "CheckCtrl.h"
#include "Button.h"
#include "ComboCtrl.h"
#include "EditCtrl.h"
#include "CharCodeTable.h"
#include <string>
using namespace GUI;

class CShanghaiCfgDailySelectDialog: public GUI::CDialog
{
public:
	CShanghaiCfgDailySelectDialog();
	~CShanghaiCfgDailySelectDialog();
	void SetInitialInfo(unsigned long channelNum);
	
	void SetStartTime(tm time);
	unsigned long TimeStart();
	ULONGLONG CH();	
protected:
	void OnInitial();
	void OnPaint(PAINT_STATUS status);
protected:
	DECLARE_MSG_MAP()
	unsigned long OnClickChnn( unsigned long ctrlID );
	unsigned long OnClickGroup( unsigned long ctrlID );
protected:
	unsigned long DoClickChnn( unsigned long index, bool bRefresh );
private:
	string ChangeChinese(const char *pstr);
private:
	GUI::CStatic		m_scStart;
	GUI::CTimeCtrl		m_timeStart;
	GUI::CCheckCtrl		*m_pCKChnn;
	GUI::CCheckCtrl		*m_pCKGroup;
	unsigned long       m_channelNum;

	enum COMBO_ROW
	{
		ROW_OPER,
		ROW_KEEP,
		ROW_CHECK,
	};
	CCharCodeTable* m_codeTable; 
	
};

#endif
//end

