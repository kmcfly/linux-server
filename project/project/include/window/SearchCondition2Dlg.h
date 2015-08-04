#ifndef _SEARCH_CONDITION_2_DLG_H_
#define _SEARCH_CONDITION_2_DLG_H_

#include "Button.h"
#include "CheckCtrl.h"
#include "Dialog.h"
#include "CalendarCtrl.h"
#include "ScrollDialog.h"

using namespace GUI;

class CSearchCondition2Dlg : public GUI::CScrollDialog
{
public:
	CSearchCondition2Dlg();
	~CSearchCondition2Dlg();

	void SetInitialInfo(unsigned long channelNum, ULONGLONG authCH);
	unsigned long Time();
	ULONGLONG CH();	
	void MarkDay(unsigned short year, unsigned char month, unsigned char day, unsigned long color = COLOR_BTN_DEFAULT_MARK, bool bRefresh = true);
protected:
	void OnInitial();
	void OnDestroy();
	void OnPaint(PAINT_STATUS status);

private:
	DECLARE_MSG_MAP()
	unsigned long OnClickScrollWnd(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);
	unsigned long OnClickChnn( unsigned long ctrlID );
	unsigned long OnClickGroup( unsigned long ctrlID );
protected:
	unsigned long DoClickChnn( unsigned long index, bool bRefresh );
private:

	unsigned long				m_channelNum;	
	ULONGLONG					m_authCH;

protected:
	enum WND_ID
	{
		ID_CALENDAR		= 0,
		ID_GROUP		= 10,
		ID_CH			= 20,
	};
};



#endif
