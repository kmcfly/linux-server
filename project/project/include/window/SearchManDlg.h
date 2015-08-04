/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : ysw
** Date         : 2009-08-01
** Name         : SearchManDlg.h
** Version      : 1.0
** Description  : 
** Modify Record:zxx
1:
***********************************************************************/
#ifndef _SEARCH_MAN_DLG_H_
#define _SEARCH_MAN_DLG_H_
#include "Button.h"
#include "TabCtrl.h"
#include "Dialog.h"
#include "TimeSearchDlg.h"
#include "EventSearchDlg.h"
#include "FileManDlg.h"
#include "PictureSearchDlg.h"
#include "SearchCondition2Dlg.h"
#include "SearchConditionDlg.h"
#include "SearchCdn2TimeDlg.h"
#include "SearchCdnTimeDlg.h"
#if defined(__GROUY_SEARCH__)
	#include "SearchGroupConditionDlg.h"
	#include "SearchGroupScrollConditionDlg.h"
	#include "SearchGroupTimeDlg.h"
	#include "SearchGroupScrollTimeDlg.h"
#endif

using namespace GUI;

class CSearchManDlg : public GUI::CDialog
{
public:
	CSearchManDlg();
	~CSearchManDlg();
	void SetInitialInfo(unsigned long diskOwnerIndex, unsigned long channelNum);

protected:
	DECLARE_MSG_MAP()
	void OnInitial();
	unsigned long OnPreCommand(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);

	void CreateFoldCtrl();	//����չ�����۵� �ķ�ʽ�����ӿؼ�
	void CreateUnfoldCtrl();//����չ�����۵� �ķ�ʽ�����ӿؼ�

	unsigned long OnPlayback();

#if defined(__GROUY_SEARCH__)
	void EndDialog(DLG_END_VALUE result);
#endif

	void OnPostInitial();
private:
	//////////////////////////////////////////////////////////////////////////
	unsigned long OnBtnFlex();
	unsigned long OnSearch();
	void SetCalendarInfo();
	//////////////////////////////////////////////////////////////////////////
	bool				m_bFlexSearchConditionDlg;
	unsigned short		m_searchConditionDlgWidth;
	GUI::CTabCtrl		m_tabCtrl;
	GUI::CButton		m_btnFlex;
	GUI::CButton		m_btnSearch;
	CTimeSearchDlg		m_timeSearchDlg;
	CEventSearchDlg		m_eventSearchDlg;
	CFileManDlg			m_fileManDlg;
	CPictureSearchDlg	m_pictureSearchDlg;



#if defined(__GROUY_SEARCH__)
	CSearchGroupConditionDlg m_searchGroupConditionDlg;//1024x768�ֱ������� br27ģʽ�����ڶԻ���
	CSearchGroupTimeDlg m_searchGroupTimeDlg;//1024x768�ֱ������� br27ģʽ��ʱ��Ի���
	CSearchGroupScrollConditionDlg m_searchGroupScrollConditionDlg;//800x600�ֱ������� br27ģʽ���������ڶԻ���
	CSearchGroupScrollTimeDlg m_searchGroupScrollTimeDlg;//800x600�ֱ����£�br27ģʽ������ʱ��Ի���
#else
	CSearchCondition2Dlg m_searchCondition2Dlg;
	CSearchConditionDlg m_searchConditionDlg;
	CSearchCdnTimeDlg	m_searchCdnTimtDlg;
	CSearchCdn2TimeDlg  m_searchCdn2TimtDlg;
#endif

	unsigned long       m_diskOwnerIndex;
	unsigned long       m_channelNum;

	bool				m_bUnfold;

	enum PAGE_ITEM
	{
		PAGE_TIME_SEARCH,
		PAGE_EVENT_SEARCH,
		PAGE_FILE_MAN,
		PAGE_PIC_SEARCH,
	};
#ifdef __TD2316SES_SPEC__
	//zxx�¼�
private:
	CFBInterface *m_pWait;
#endif
	bool m_bFlexOut;//800*600�µ�չ������,Ĭ��Ϊ����(false),չ��Ϊtrue
};
#endif //_SEARCH_MAN_DLG_H_
