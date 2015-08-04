/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : L
** Date         : 2009-01-12
** Name         : SetPositionDlg.h 
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/

#ifndef _SET_POSITION_DLG_
#define _SET_POSITION_DLG_

#include "Dialog.h"
#include "Static.h"
#include "Rect.h"

using namespace GUI;

//////////////////////////////////////////////////////////////////////////
class CSetPtItem : public GUI::CWnd
{
public:
	CSetPtItem();
	virtual ~CSetPtItem();

#ifdef __SUPPORT_OSDTWOLINES__
	 virtual bool Create( unsigned long ctrlID, unsigned long x, unsigned long y, unsigned long cx, \
				   		  unsigned long cy, bool bCamName, CWnd* pParent, const char* pCaption = NULL);
#else
	virtual bool Create( unsigned long ctrlID, unsigned long x, unsigned long y, unsigned long cx, \
						 unsigned long cy, CWnd* pParent, const char* pCaption = NULL);
#endif

	void OnIdle();

	void SetDrawBlink(bool draw) { m_bDrawBorder = draw; }
	bool SetBlink(bool enable);
	bool IsBlink() { return m_bBlink; }

	void OnDrawText();
	void OnDrawBlink(bool erase = true);

protected:
	virtual void OnPaint(PAINT_STATUS status);

	bool m_bBlink;			//������ҫ

	bool m_bDrawBorder;		//�Ƿ������ҫ�߿�
	unsigned long m_count;	//��ʱ

#ifdef __SUPPORT_OSDTWOLINES__
	bool m_bCamName;      //�Ƿ���ͨ������
#endif
};

//////////////////////////////////////////////////////////////////////////

class CSetPositionDlg : public GUI::CDialog
{
public:
	CSetPositionDlg();
	~CSetPositionDlg();

	enum ITEM_SEL
	{
		ITEM_NULL = 0,
		ITEM_CAM,
		ITEM_TIME,
	};

public:
	void SeEnableCamera(int x, int y, int cx, int cy, const char* pCamera=NULL, bool enable=true);
	void SetEnableTime(int x, int y, int cx, int cy, const char* pTime=NULL, bool enable=true);

	void GetPosCamera(int& x, int& y);
	void GetPosTime(int& x, int& y);

	bool IsModify() { return m_bModify;}

	void OnDrawItem(CSetPtItem* pItem);

protected:
	void OnInitial();
	void OnDestroy();
	void OnPaint(PAINT_STATUS status);
	void EndDialog(DLG_END_VALUE result);
	void OnIdle();
	unsigned long OnFocusProcEx(unsigned long keyID, unsigned long xPoint, unsigned long yPoint);

protected:
	//(x1,y1) (x2,y2) ��������
	bool IsAlmostEqual(unsigned long x1, unsigned long y1, unsigned long x2, unsigned long y2, unsigned long distance);
	//(x1,y1) (x2,y2) �������ܴ�
	bool IsAlmostAbove(unsigned long x1, unsigned long y1, unsigned long x2, unsigned long y2, unsigned long distance);

	//�����޸�
	void SetModify(bool modify = true);

	void SetSelItem(ITEM_SEL sel, bool bRefresh);
	void UpdateSelItem(bool bRefresh);

protected:
	DECLARE_MSG_MAP()
	unsigned long OnEnter(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);
	unsigned long OnMouseMove(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);

protected:
	CSetPtItem m_scCamera;
	CSetPtItem m_scTime;

protected:
	bool m_bSetCamera;			//��������ͨ����
	bool m_bSetTime;			//��������ʱ��

	unsigned long m_x_enter,m_y_enter;	//��������x,yֵ
	unsigned long m_offset_len, m_offset_pos;

	bool m_bDragCamera;			//�����϶�ͨ�����
	bool m_bDragTime;			//�����϶�ʱ����
	bool m_bFirstDrag;			//��һ���϶����

	GUI::CRect m_rectCamera;	//ͨ���� ���ھ��ο�
	GUI::CRect m_rectTime;		//ʱ�� ���ھ��ο�

	ITEM_SEL m_selItem;			//��ǰѡ�е�

	bool m_bModify;				//�޸ı��
};

#endif
