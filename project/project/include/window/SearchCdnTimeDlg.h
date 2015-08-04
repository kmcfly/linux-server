/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : xzw
** Date         : 2011-04-25
** Name         : SearchCdnTimeDlg.h
** Version      : 3.0
** Description  : 
** Modify Record:
1:
***********************************************************************/

#ifndef _SEARCH_CDN_TIME_DLG_
#define _SEARCH_CDN_TIME_DLG_

#include "Dialog.h"
#include "ScrollDialog.h"
#include "Static.h"
#include "DateCtrl.h"
#include "TimeCtrl.h"
#include "CheckCtrl.h"
#include "Button.h"
#include "CalendarDlg.h"


using namespace GUI;

class CSearchCdnTimeDlg : public GUI::CScrollDialog
{
public:
	CSearchCdnTimeDlg();
	~CSearchCdnTimeDlg();

public:
	void SetInitialInfo(unsigned long channelNum, ULONGLONG authCH = INVALID_INDEX(ULONGLONG));

	unsigned long TimeStart();
	unsigned long TimeEnd();
	ULONGLONG CH();	

	void SetStartTime(tm time);
	void SetEndTime(tm time);

protected:
	void OnInitial();

	void OnPaint(PAINT_STATUS status);

protected:
	DECLARE_MSG_MAP()
	unsigned long OnTimeStart(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);
	unsigned long OnTimeEnd(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);
	unsigned long OnClickScrollWnd(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);
	unsigned long OnClickChnn( unsigned long ctrlID );
	unsigned long OnClickGroup( unsigned long ctrlID );

protected:
	unsigned long DoClickChnn( unsigned long index, bool bRefresh );

protected:

	//GUI::CStatic		*m_scStart;
	//GUI::CStatic		*m_scEnd;

	//GUI::CDateCtrl		*m_dateStart;
	//GUI::CTimeCtrl		*m_timeStart;
	//GUI::CDateCtrl		*m_dateEnd;
	//GUI::CTimeCtrl		*m_timeEnd;

	//GUI::CCheckCtrl		*m_pCKChnn;
	//GUI::CCheckCtrl		*m_pCKGroup;

	CCalendarDlg		m_calendarDlg;

protected:
	unsigned long       m_channelNum;
	ULONGLONG			m_authCH;

	enum WND_ID
	{
		ID_START_STATIC = 0,
		ID_END_STATIC,
		ID_DATE_START,
		ID_TIME_START,
		ID_DATE_END,
		ID_TIME_END,

		ID_GROUP = 10,
		ID_CKCH = 20,
	};
};



#endif
