/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : hz
** Date         : 2009-08-01
** Name         : MotionAreDlg.h 
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/
#ifndef _MOTION_AREA_DLG_H_
#define _MOTION_AREA_DLG_H_

#include <vector>
#include "Dialog.h"
#include "ComboCtrl.h"
#include "SliderCtrl.h"
#include "Static.h"
#include "MotionArePanelDlg.h"
#include "ConfigEx.h"

using namespace GUI;

class CMotionAreDlg : public GUI::CDialog
{
public:
	CMotionAreDlg();
	~CMotionAreDlg();

	void SetAreaInfo(const MOTION_DETECT_INFO& motionDetectInfo, int chnn = 0);
	void GetAreaInfo(MOTION_DETECT_INFO& motionDetectInfo) const;

	bool IsTested(bool bRefresh);

	bool SetMotionArea(const MOTION_DETECT_INFO& motion);

protected:
	DECLARE_MSG_MAP()
	void OnInitial();
	void OnDestroy();
	void OnPaint(PAINT_STATUS status);
	void OnIdle();
	void EndDialog(DLG_END_VALUE result);

	unsigned long OnClick(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);
	unsigned long OnMouseDrag(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);
	unsigned long OnDoubleClick(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);
	unsigned long OnClickPanel();

	void DrawMotion(bool draw, bool bRefresh);
private:
	CMotionArePanelDlg m_panelDlg;

	struct CELL 
	{
		int x;
		int y;
	};
	typedef std::vector<CELL> CELL_LIST;
	CELL_LIST m_cellList;

	int m_cellCount_x;
	int m_cellCount_y;
	int m_cell_cx;
	int m_cell_cy;

	//bool m_bStartDrag;
	CELL m_startDragCell;
	bool m_bAddCell;

	unsigned long m_sensitivity;						//motion检测的灵敏度

	MOTION_DETECT_INFO* m_pMtInfoSor; //源motion数据

	int m_chnn;
	bool m_bTested;
	bool m_bModify;

	unsigned char *m_pImgMotion;
	bool m_bDrawMotion;
	bool m_bReDraw;
	bool m_bMotion;

	void DrawCell(const CELL & cell, bool bImmediate);
	void DrawCellDel(const CELL& cell, bool bImmediate);
	void DrawCellList();
	CELL GetCell(int xPos, int yPos) const;
	bool IsCellInList(const CELL& cell) const;
	void AddCell(const CELL& cell);
	void DelCell(const CELL& cell);
	void HitCell(const CELL& cell);

	void TestReDraw(const CELL& cell);
};

#endif//_MOTION_AREA_DLG_H_

