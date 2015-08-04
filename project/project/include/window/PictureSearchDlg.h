/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : xzw
** Date         : 2010-10-14
** Name         : PictureSearchDlg.h
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/
#ifndef _PICTURE_SEARCH_DLG_H_
#define _PICTURE_SEARCH_DLG_H_

#include "gui.h"
#include "Wnd.h"
#include "Button.h"
#include "Dialog.h"
#include "Rect.h"
#include "MessageMan.h"

using namespace GUI;

class CPictureSearchDlg : public GUI::CDialog
{
public:
	CPictureSearchDlg(void);
	~CPictureSearchDlg(void);
	void SetInitialInfo(unsigned long channelNum, unsigned long diskOwnerIndex);

	void OnSearch(unsigned long startTime, unsigned long endTime, ULONGLONG chnnBits, unsigned long dataOwnerby);

	bool GetPlaybackInfo(unsigned long &playbackStarTime, unsigned long &playbackEndTime, unsigned long &playbackPlayTime, ULONGLONG &chnnBits);
	
protected:
	void OnInitial();
	void OnDestroy();
	void OnPaint(PAINT_STATUS status);

	DECLARE_MSG_MAP()
	unsigned long OnEnter(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);
	unsigned long OnMouseDrag(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);
	unsigned long OnMouseLeftUp(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);
	unsigned long OnDBClick(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);
	unsigned long OnClickLock();
	unsigned long OnClickDel();
	unsigned long OnClickSave();
	unsigned long OnClickSaveAll();
	unsigned long OnClickFirst();
	unsigned long OnClickPrev();
	unsigned long OnClickNext();
	unsigned long OnClickEnd();

	void SetCurSnapPic(int index, bool bRefresh = false);
	bool IsAlmostAbove(unsigned long x1, unsigned long y1, unsigned long x2, unsigned long y2, unsigned long distance);

	void SetBtnLockCaption(bool bLock, bool bRefresh = false);
	void SetBtnReadOnly(bool bReadOnly, bool bRefresh);

private:
	void GetDisplayPicInfo(unsigned char * pRealImgData, int &realDataLen, int &realW, int &realH);
	bool SetDisplayPicParam(int realW, int realH,int &displayH,int &displayW);//设置图像显示需要的一些参数
	int m_imgDisplayW;
	int m_imgDisplayH;

protected:
	GUI::CButton m_btnDelete;
	GUI::CButton m_btnLock;
	GUI::CButton m_btnSave;
	GUI::CButton m_btnSaveAll;


	GUI::CButton	m_btnFirst;
	GUI::CButton	m_btnPrev;
	GUI::CButton	m_btnNext;
	GUI::CButton	m_btnEnd;

	unsigned long m_channelNum;
	unsigned long m_diskOwnerIndex;

	//获取到的抓图信息
	int m_iSnapPicMaxLen;
	int m_iSnapPicLen;
	SNAP_PIC_PRORITY* m_pSnapPicPrority;

	//某一图片信息
	int m_iImgLen;
	int m_iImgWidth;
	int m_iImgHeight;
	int m_iImgMaxBuffLen;
	unsigned char* m_pImgData;

	//当前选择的某张图
	int m_curSnapPic;

	bool m_bStartDrag;
	bool m_bFirstDrag;
	unsigned long m_x_enter,m_y_enter;	//点击进入的x,y值

	GUI::CRect m_rectShowPic;			//显示图片的区域

	NET_DATA_SEARCH m_searchCond;

	//回放条件
	unsigned long m_playbackStarTime;
	unsigned long m_playbackEndTime;
	unsigned long m_playbackPlayTime;
	ULONGLONG     m_chnnBits;
};

#endif

//end
