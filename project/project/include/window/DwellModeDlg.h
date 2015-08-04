/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : L
** Date         : 2009-10-16
** Name         : DwellModeDlg.h 
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/

#ifndef _LIVE_DWELL_DLG_H_
#define _LIVE_DWELL_DLG_H_

#include "Dialog.h"
#include "Button.h"
#include "ComboCtrl.h"
#include "Static.h"
#include "DwellModeDlgNXN.h"
#include "NetConfigDefine.h"

using namespace GUI;

class CDwellModeDlg : public GUI::CDialog
{
public:
	CDwellModeDlg();
	~CDwellModeDlg();

	enum DLG_PAGE
	{
		PAGE_1X1 = 0,
		PAGE_2X2,
		PAGE_2X3,
		PAGE_3X3,
		PAGE_4X4,
		PAGE_5X5,
		PAGE_6X6,

		PAGE_MAX,
	};

	CDwellModeDlgNXN* GetPage(int index);
	CDwellModeDlgNXN* GetCurPage();
	GUI::CRect GetPageRect();
	int GetCurPageIndex() {return m_curPage;}

	void ShowPage( int index, bool bRefresh );

	void SetMaxChnn(int maxChnn, int localChnnNum,int netChannNum);
	void SetSpot(bool bSopt=false){m_bSpot = bSopt;}

	int GetMaxGroup();

	//�趨�޸İ󶨵�����,ҳ�������޸�ֱ���޸Ķ�Ӧ����ֵ
	void SetCfgData(unsigned long* pData,unsigned long* pDwell, int dwellLen);

	//���ϼ����巢���޸����ݶ���
	void SendModifyCfgData(NCFG_ITEM_ID cfgID);

	//���ϼ����ڷ����л�ҳ�涯��
	void SendModifyGroupInfo(bool bRefresh);

	//�ж�ͬһҳ���ϵ�ͨ���Ų��ظ��Ͳ�ȫ�ޡ�
	unsigned long 	CheckDwell();

public:
	void SetCfgDwellTime(NCFG_ITEM_ID dwelltime ) { m_cfgDwellTime = dwelltime; }
	void SetCfgSplitBase(NCFG_ITEM_ID split) { m_cfgSplitBase = split; }

protected:
	void ShowData(int index,int group,bool bRefresh);

	int GetPageMax(int index);

	std::string GetGroupTip(ENUM_STRING_ID preID);

protected:
	void OnInitial();
	void OnDestroy();
	void SetTip();
	void OnPaint(PAINT_STATUS status);
	void OnChangeFocus(unsigned long oldID, unsigned long newID);

	unsigned long OnEnter(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);
	unsigned long OnMouseDrag(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);
	unsigned long OnMouseLeftUp(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);

protected:
	DECLARE_MSG_MAP()
	unsigned long OnBtnPre();
	unsigned long OnBtnNext();
	unsigned long OnDwellTime();
	unsigned long OnClickScrollWnd(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);

protected:
	CDwellModeDlgNXN		m_dwellModeDlg;

	GUI::CButton			m_btnPre;
	GUI::CButton			m_btnNext;

	GUI::CComboCtrl			m_cbDwellTime;
	GUI::CStatic			m_scDwellTime;
private:
	GUI::CRect				m_rectNXN;
	int						m_curPage;		//��ǰ��ʾ��ҳ��
	int						m_group;		//��ǰ��

protected:
	int						m_maxChnn;		//���ͨ����
	int						m_localChnnNum;	//����ͨ����
	int 					m_netChannNum;	//����ͨ����
	int						m_dwellTimeLen; //
	unsigned long*			m_pData;		//���õ���������
	unsigned long*			m_pDwellTime;	//���õ���������

protected:
	NCFG_ITEM_ID			m_cfgDwellTime;	//��̨ʱ��ID
	NCFG_ITEM_ID			m_cfgSplitBase;	//�ָ�ģʽ


	//������
	bool			m_bDrag;
	bool			m_bFirstDrag;

	unsigned long	m_enterPosX;
	unsigned long	m_enterPosY;
	bool			m_bSpot;

	int 			m_indexCurItem[128];
};


#endif
