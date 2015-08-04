/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : L
** Date         : 2009-08-29
** Name         : CfgUserAuthorityDlg.h
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/

#ifndef _CFG_USER_AUTHORITY_DLG_
#define _CFG_USER_AUTHORITY_DLG_

#include "NetConfigDefine.h"
#include "CfgDlg.h"
#include "ScrollDialog.h"
#include "CheckCtrl.h"

using namespace GUI;

class CCfgUserAuthorityDlg : public GUI::CScrollDialog
{
public:
	CCfgUserAuthorityDlg();
	~CCfgUserAuthorityDlg();

public:
	void ShowInfoUserAuth( const NCFG_INFO_USER& infoUser, bool bRefresh );
	bool GetInfoUserAuth( NCFG_INFO_USER& infoUser );

	void SetMaxChnn(int max, ULONGLONG effectCH = INVALID_INDEX(ULONGLONG));

protected:
	void OnInitial();
	void OnPaint(PAINT_STATUS status);

protected:
	void SetAuthChnn(int baseID,ULONGLONG auth,bool bRefresh=false);
	void GetAuthChnn(int baseID,ULONGLONG& auth);

	void SetAuthCommon(unsigned long userRight,bool bRefresh=false);
	void GetAuthCommon(unsigned long& userRight);

	void CheckUIEnable(bool bRefresh);

protected:
	DECLARE_MSG_MAP()
	unsigned long OnClickScrollWnd(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);

	void OnClickChnn(int base,int index,bool bRefresh);
	void OnClickGroup(int base,int index,bool bRefresh);

protected:
	enum WND_ID
	{
		ID_CH_NONE			= 0,
		ID_CH_HAVE			= 16,

		ID_CH_LIVE_GROUP	= 32,
		ID_CH_LIVE			= 40,

		ID_CH_RECORD_GROUP	= 72,
		ID_CH_RECORD		= 80,

		ID_CH_PLAYBACK_GROUP= 112,
		ID_CH_PLAYBACK		= 120,

		ID_CH_BACKUP_GROUP	= 152,
		ID_CH_BACKUP		= 160,

		ID_CH_PTZ_GROUP		= 192,
		ID_CH_PTZ			= 200,

		ID_CH_REMOTE_GROUP	= 232,
		ID_CH_REMOTE		= 240,
	};

protected:
	int					m_maxChnn;
	ULONGLONG			m_effectCH;		//有效通道
};

#endif
