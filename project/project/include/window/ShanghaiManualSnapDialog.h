/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : zxx
** Date         : 2013-1-5
** Name         : ShanghaiManualSnapDialog.h
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/

#ifndef _SHANGHAIMANUAL_SNAPDIALOG_H_
#define _SHANGHAIMANUAL_SNAPDIALOG_H_


#include "Dialog.h"
#include "Static.h"
#include "CheckCtrl.h"
#include "EditCtrl.h"
#include "Button.h"
#include "CharCodeTable.h"
using namespace GUI;
class CShanghaiManualSnapDialog: public GUI::CDialog
{
public:
	CShanghaiManualSnapDialog();
	~CShanghaiManualSnapDialog();
	void SetInitialInfo(unsigned long channelNum,unsigned long choseType);
	bool OnClickBtnUpload();
protected:
	void OnInitial();
protected:
	unsigned long DoClickChnn( unsigned long index, bool bRefresh );
protected:
	DECLARE_MSG_MAP()
	unsigned long OnClickChnn( unsigned long ctrlID );
	unsigned long OnClickGroup( unsigned long ctrlID );
private:
	string ChangeChinese(const char *pstr);
private:
	GUI::CStatic	    m_Tip;
	GUI::CCheckCtrl		*m_pCKChnn;
	GUI::CCheckCtrl		*m_pCKGroup;
	GUI::CStatic		m_policeTip;
	GUI::CEditCtrl		m_policeNumEdit;
	GUI::CButton		m_btnUpload;

	unsigned long       m_channelNum;
	unsigned long		m_choseType;

	CCharCodeTable      *m_codeTable; 

	enum chose_type
	{
		type_TEST,
		type_KEEP,
		type_CHECK,
	};
private:
	ULONGLONG CH();
	
};

#endif

