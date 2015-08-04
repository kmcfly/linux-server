/***********************************************************************
** Copyright (C)  Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : L
** Date         : 2010-03-25
** Name         : SetCoverDlg.h
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/

#ifndef _SET_COVER_DLG_H_
#define _SET_COVER_DLG_H_

#include "Dialog.h"
#include <vector>

using namespace GUI;

class CSetCoverDlg : public GUI::CDialog
{
public:
	CSetCoverDlg();
	~CSetCoverDlg();

	enum COVER_STATE
	{
		STATE_NULL = 0,
		STATE_ADD,
	};

	struct RectCover
	{
		unsigned long	old;
		COVER_AREA		area;

		GUI::CRect		rect;
		unsigned long	color;
		unsigned long	bCover;

		unsigned long	id;
	};

	void SetCoverInfo(const CAMERA_COVER_INFO& coverInfo);
	void GetCoverInfo(CAMERA_COVER_INFO& coverInfo);

	bool IsModify();

protected:
	void OnInitial();
	void OnDestroy();
	void OnPaint(PAINT_STATUS status);
	void EndDialog(DLG_END_VALUE result);

protected:
	bool RefreshCoverInfo();

protected:
	void SetModify(bool modify = true);
	//(x1,y1) (x2,y2) 距离相差不大
	bool IsAlmostEqual(unsigned long x1, unsigned long y1, unsigned long x2, unsigned long y2, unsigned long distance);
	bool HitAreaTest(unsigned long xPos, unsigned long yPos, unsigned long& idHit);
	bool FindAreaById(unsigned long id, GUI::CRect& rect);
	void DrawRectPt2(unsigned long x1, unsigned long y1, unsigned long x2, unsigned long y2, bool bDraw = true);
	void RefreshPt2(unsigned long x1, unsigned long y1, unsigned long x2, unsigned long y2);
	void DrawRectSel(const GUI::CRect& rect, bool bDraw = true, bool bImmediate = true);

	void DrawRestore(const GUI::CRect& rect, unsigned long idMiss = -1, bool bImmediate = true);
	void DrawRestore(unsigned long x1, unsigned long y1, unsigned long x2, unsigned long y2, unsigned long idMiss = -1, bool bImmediate = true);
protected:
	DECLARE_MSG_MAP()
	unsigned long OnClick(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);
	unsigned long OnDoubleClick(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);
	unsigned long OnMouseDrag(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);

protected:
	
	unsigned long			m_idSel;	//被选中的数据
	unsigned long			m_x_enter, m_y_enter;	//点击进入的x,y值
	int						m_areaMax;	//最大数据组数
	bool					m_bFirstDrag;			//第一次拖动标记
	bool					m_bModify;				//标记是否修改过
	CAMERA_COVER_INFO		m_coverInfo;//从外部接口获取的数据
	std::vector<RectCover>	m_areaVec;	//区域数据
	COVER_STATE				m_state;				//当前操作状态
	
};

#endif
