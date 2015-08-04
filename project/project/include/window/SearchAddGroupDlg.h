/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : wn
** Date         : 2011-10-01
** Name         : SearchAddGroupDlg
** Version      : 1.0
** Description  : 
** Modify Record:
** Modify       :zxx
1:  ע�⣺ ͨ�����������洫��
***********************************************************************/
#if defined(__GROUY_SEARCH__)
#ifndef _SEARCH_ADD_GROUP_DLG_H_
#define _SEARCH_ADD_GROUP_DLG_H_

#include "Dialog.h"
#include "EditCtrl.h"
#include "CheckCtrl.h"
#include "Product.h"
#include "NetConfigDefine.h"
#include "Button.h"
#include <list>

using namespace GUI;

typedef std::list<NCFG_INFO_GROUP_ITEM> GROUP_ITEM_LIST;

class CSearchAddGroupDlg : public GUI::CDialog
{
public:
	CSearchAddGroupDlg();
	~CSearchAddGroupDlg();
    void SetInitialInfo(unsigned long chnnNum, ULONGLONG authCH,GROUP_ITEM_LIST *pcfgGroup);
	//������༭ʱ����Ҫ�ĺ���
	//����:bool benable ---��ȡ�Ƿ��±༭��ť,����Ҫ�༭����false
	//    string curStr ---���������һ��ҳ��ѡ��ķ�����������Ҫ�༭����""
	//    int nItem     ---�����combo�ĵ�ǰ��,����Ҫ�༭����0
	void GetEditEnable(bool bIsEdit,string curStr,int nItem);
	//������Ҫˢ�µ�combo��ǰֵ
	int SendCurnItem();
protected:
	DECLARE_MSG_MAP()
	void OnInitial();
	void OnPostInitial();
	unsigned long OnOK();//���沢�˳�
	unsigned long OnCancel();//�˳�
	void OnDestroy();
	void OnPaint(PAINT_STATUS status);
	unsigned long OnClickEdit();
	unsigned long OnClickChnn( unsigned long ctrlID );
	unsigned long OnClickGroup( unsigned long ctrlID );
	unsigned long DoClickChnn( unsigned long index, bool bRefresh );
private:
	GUI::CEditCtrl m_editAdd;
	GUI::CCheckCtrl	*m_pCKCH, *m_pCKGroup;
	GUI::CStatic  m_static;
	GUI::CStatic  m_selectStatic;

	std::string m_curStr;
	unsigned long m_chnnNum;//ͨ����Ŀ
	bool m_bIsEdit;//�Ƿ��Ǳ༭ҳ��
	ULONGLONG m_authCH;
	int m_nItem;//��һ��ҳ��combo�ؼ���ֵ
	
	GROUP_ITEM_LIST *m_pcfgList;
private:
	void SetChannel(const ULONGLONG chanbits);
	ULONGLONG GetCurChannel() const;
	bool AddGroup();//�޸�ҳ��ֵ��ʱ���ж��Ƿ����ӷ���
	int GetLineNum();
	int GetGroupNum();
};

#endif
#endif
