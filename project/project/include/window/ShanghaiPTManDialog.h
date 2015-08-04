/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : zxx
** Date         : 2012-12-30
** Name         : ShanghaiPTManDialog.h
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/
#ifndef __SHANGHAI_PTMAN_DIALOG_H__
#define __SHANGHAI_PTMAN_DIALOG_H__

#include "TabCtrl.h"
#include "CfgDlg.h"
#include "CfgConnectSGPTDlg.h"
#include "CfgRecordSnapDlg.h"
#include "ShanghaiCfgDailyImageDialog.h"
#include "ShanghaiAlarmImageDialog.h"
#include "CharCodeTable.h"
#include <string>
using namespace GUI;

class CShanghaiPTManDialog: public CCfgDlg
{
public:
	CShanghaiPTManDialog();
	~CShanghaiPTManDialog();
protected:
	enum PAGE_ITEM
	{
		PAGE_NET = 0,	//ƽ̨��������
		PAGE_SNAPCFG,	//ƽ̨ץͼ����
		PAGE_DAILYIMAGE,//ƽ̨�ճ�ͼ������
		PAGE_ALARMIMAGE,//ƽ̨����ͼ������
	};

	virtual void OnInitial();	
	
	void OnPostInitial();

	void OnDestroy();

	void GetItemList(CFGITEM_LIST& itemList) const;

	CCfgDlg * GetPage(PAGE_ITEM item);

	unsigned long OnPreCommand(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);
	
	unsigned long OnApply();

	unsigned long CheckValue();

	bool EnterConfig();

	void ExitConfig() const;
protected:
	DECLARE_MSG_MAP()
	unsigned long OnClickNET();
	unsigned long OnClickRecordSnap();
	unsigned long OnClickDailyImage();
	unsigned long OnClickAlarmImage();

private:
	string ChangeChinese(const char *pstr);
private:
	GUI::CTabCtrl	  m_tabCtrl;

	unsigned long	   m_initPageMask;			//��¼�Ѿ���ʼ����ҳ��
	PAGE_ITEM		   m_curPage;

	CCfgConnectSGPTDlg			 m_PTNETCfgDialog;			//ƽ̨����
	CCfgRecordSnapDlg			 m_cfgRecordSnapDlg;		//ץͼ����
	CShanghaiCfgDailyimageDialog m_shanghaiDailyImageDialog;//�ճ�ͼ��
	CShanghaiAlarmImageDialog	 m_shanghaiAlarmimageDialog;//����ͼ��
	CCharCodeTable* m_codeTable;
};

#endif
