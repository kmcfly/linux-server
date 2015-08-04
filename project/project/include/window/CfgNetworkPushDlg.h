
#ifndef __CFG_NETWORK_PUSH_DLG_H__
#define __CFG_NETWORK_PUSH_DLG_H__

#include "ListCtrl.h"
#include "CfgDlg.h"

using namespace GUI;

class CCfgNetworkPushDlg : public CCfgDlg
{
public:
	CCfgNetworkPushDlg();
	~CCfgNetworkPushDlg();

	void DoOther();
	void EndOther();

protected:
	void OnInitial();

protected: 
	bool ShowData(NCFG_ITEM_ID cfgID, const BYTE* pData, int len, bool bRefresh);
	bool SetData(NCFG_ITEM_ID cfgID, BYTE*& pData, int& len)  const;

	DECLARE_MSG_MAP()
	unsigned long OnClickList(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);

protected:
	GUI::CListCtrl	m_cfgNatList;

	enum LIST_ROW
	{
		ROW_USE_PUSH,
		ROW_PUSH_SERVER_IP ,
		ROW_PUSH_SERVER_PORT ,
		ROW_COUNT,
	};
};
#endif