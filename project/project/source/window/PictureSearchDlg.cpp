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
#include "PictureSearchDlg.h"
#include "ImageList.h"
#include "TimeCtrl.h"
#include "SnapMan.h"
#include "DrawManager.h"

#define  SNAP_PIC_SAVE_PATH  "/mnt/sapicu"

CPictureSearchDlg::CPictureSearchDlg(void)
{
	m_iSnapPicMaxLen = 0;
	m_iSnapPicLen = 0;
	m_pSnapPicPrority = NULL;

	m_curSnapPic = -1;

	m_iImgMaxBuffLen = 0;
	m_iImgLen = 0;
	m_iImgWidth = 0;
	m_iImgHeight = 0;
	m_pImgData = NULL;

	m_x_enter = 0;
	m_y_enter = 0;

	m_bStartDrag = false;

	m_playbackStarTime = 0;
	m_playbackEndTime  = 0;
	m_playbackPlayTime = 0;
	m_chnnBits         = 0;

	m_imgDisplayW = 0;
	m_imgDisplayH = 0;
}

CPictureSearchDlg::~CPictureSearchDlg(void)
{

}

void CPictureSearchDlg::SetInitialInfo(unsigned long channelNum, unsigned long diskOwnerIndex)
{
	m_channelNum = channelNum;
	m_diskOwnerIndex = diskOwnerIndex;
}

void CPictureSearchDlg::OnInitial()
{
	assert(m_channelNum > 0);
	GUI::CDialog::OnInitial();

	//大、小屏幕分开布局页面
	bool bBigScreen = ((GUI::CWnd::m_width >= 1024) && (GUI::CWnd::m_height >= 768)) ? true : false;

	unsigned short x = m_length[DLG_OFFSET_SUB_LEFT], y = m_length[DLG_OFFSET_SUB_TOP];
	unsigned short width = m_rect.Width() - 2 * m_length[DLG_OFFSET_SUB_LEFT];
	unsigned short height = m_rect.Height() - m_length[DLG_OFFSET_SUB_TOP] - m_length[DLG_OFFSET_SUB_BOTTOM] - m_length[BTN_HEIGHT];
	unsigned short listHeight = ((height - m_length[LIST_HEAD_HEIGHT] - 30/*LIST_CTRL_BOTTOM_HEIGHT*/)/m_length[LIST_EX_ITEM_HEIGHT])*m_length[LIST_EX_ITEM_HEIGHT] + m_length[LIST_HEAD_HEIGHT] + 30/*LIST_CTRL_BOTTOM_HEIGHT*/;

	//锁定、删除按钮
	y = m_length[DLG_OFFSET_SUB_TOP] + listHeight + 4;
	m_btnDelete.Create(GUI::CWnd::GetRandWndID(), x, y, m_length[BTN_WIDTH], m_length[BTN_HEIGHT], this, 0, 1, m_pStrTable->String(E_STR_ID_DELETE));

	x += m_length[BTN_WIDTH] + 10;// width * 4 / 32;
	m_btnLock.Create(GUI::CWnd::GetRandWndID(), x, y, m_length[BTN_WIDTH], m_length[BTN_HEIGHT], this, 1, 1, m_pStrTable->String(E_STR_ID_LOCK));

	x += m_length[BTN_WIDTH] + 10;// width * 4 / 32;
	m_btnSave.Create(GUI::CWnd::GetRandWndID(), x, y, m_length[BTN_WIDTH], m_length[BTN_HEIGHT], this, 2, 1, m_pStrTable->String(E_STR_ID_SAVE));

	x += m_length[BTN_WIDTH] + 10;// width * 4 / 32;
	m_btnSaveAll.Create(GUI::CWnd::GetRandWndID(), x, y, m_length[BTN_WIDTH], m_length[BTN_HEIGHT], this, 3, 1, m_pStrTable->String(E_STR_ID_SAVE_ALL));

	//
	y = m_length[DLG_OFFSET_SUB_TOP] + listHeight + 4;
	x = m_rect.Width() - m_length[DLG_OFFSET_SUB_LEFT] - 44 * 4;

	unsigned short btnWidth = 40;
	unsigned short btnHeight = 24;
	m_btnFirst.Create(GetRandWndID(), x, y, btnWidth, btnHeight, this, 4, 1);
	
	x += btnWidth + 4;
	m_btnPrev.Create(GetRandWndID(), x, y, btnWidth, btnHeight, this, 5, 1);
	
	x += btnWidth + 4;
	m_btnNext.Create(GetRandWndID(), x, y, btnWidth, btnHeight, this, 6, 1);
	
	x += btnWidth + 4;
	m_btnEnd.Create(GetRandWndID(), x, y, btnWidth, btnHeight, this, 7, 1);

	m_btnFirst.SetImage(PAINT_STATUS_NORMAL,BMP_LISTEX_BTN_LEFTMOST,false);
	m_btnFirst.SetImage(PAINT_STATUS_FOCUS,BMP_LISTEX_BTN_LEFTMOST_FOCUS,false);
	m_btnFirst.SetImage(PAINT_STATUS_DISABLE,BMP_LISTEX_BTN_LEFTMOST_DISABLE,false);

	m_btnPrev.SetImage(PAINT_STATUS_NORMAL,BMP_LISTEX_BTN_LEFT,false);
	m_btnPrev.SetImage(PAINT_STATUS_FOCUS,BMP_LISTEX_BTN_LEFT_FOCUS,false);
	m_btnPrev.SetImage(PAINT_STATUS_DISABLE,BMP_LISTEX_BTN_LEFT_DISABLE,false);

	m_btnNext.SetImage(PAINT_STATUS_NORMAL,BMP_LISTEX_BTN_RIGHT,false);
	m_btnNext.SetImage(PAINT_STATUS_FOCUS,BMP_LISTEX_BTN_RIGHT_FOCUS,false);
	m_btnNext.SetImage(PAINT_STATUS_DISABLE,BMP_LISTEX_BTN_RIGHT_DISABLE,false);

	m_btnEnd.SetImage(PAINT_STATUS_NORMAL,BMP_LISTEX_BTN_RIGHTMOST,false);
	m_btnEnd.SetImage(PAINT_STATUS_FOCUS,BMP_LISTEX_BTN_RIGHTMOST_FOCUS,false);
	m_btnEnd.SetImage(PAINT_STATUS_DISABLE,BMP_LISTEX_BTN_RIGHTMOST_DISABLE,false);

	//

	unsigned short pic_cx = 352;
	unsigned short pic_cy = 288;
	unsigned short pic_x = 0, pic_y = 0;

	if(bBigScreen)
	{
		//居中
		pic_x = m_rect.m_left + m_length[DLG_OFFSET_SUB_LEFT] + (width - pic_cx) / 2;
		pic_y = m_rect.m_top + m_length[DLG_OFFSET_SUB_TOP] + (listHeight - pic_cy - m_length[STATIC_HEIGHT]) /  2;
	}
	else
	{
		pic_x = m_rect.m_left + m_length[DLG_OFFSET_SUB_LEFT] + (width - pic_cx) / 2;;
		pic_y = m_rect.m_top + 2;
	}
	m_imgDisplayW = pic_cx;
	m_imgDisplayH = pic_cy;
	m_rectShowPic.SetRect(pic_x, pic_y, pic_x + pic_cx, pic_y + pic_cy);

	////
	assert(NULL == m_pImgData);
#if defined(__CHIP3531__) || defined(__DVR_ULTIMATE__) || defined(__CHIPGM__) 
	m_iImgMaxBuffLen = 5 * 1024 * 1024; //
#else
	m_iImgMaxBuffLen = 1024 * 1024; //256K
#endif
	m_pImgData = new unsigned char[m_iImgMaxBuffLen];
	memset(m_pImgData, 0, m_iImgMaxBuffLen);
	m_curSnapPic = -1;

	memset(&m_searchCond, 0, sizeof(m_searchCond));

	m_playbackStarTime = 0;
	m_playbackEndTime  = 0;
	m_playbackPlayTime = 0;
	m_chnnBits         = 0;	

	m_bStartDrag = false;
	m_bFirstDrag = false;
}

void CPictureSearchDlg::OnDestroy()
{
	if(m_pSnapPicPrority)
	{
		delete [] m_pSnapPicPrority;
		m_pSnapPicPrority = NULL;
	}
	m_iSnapPicMaxLen = 0;
	m_iSnapPicLen = 0;
	m_curSnapPic = -1;

	if(m_pImgData)
	{
		delete [] m_pImgData;
		m_pImgData = NULL;
	}
	m_iImgMaxBuffLen = 0;
	m_iImgLen = 0;
	m_iImgWidth = 0;
	m_iImgHeight = 0;

	m_playbackStarTime = 0;
	m_playbackEndTime  = 0;
	m_playbackPlayTime = 0;
	m_chnnBits         = 0;

	GUI::CDialog::OnDestroy();
}

void CPictureSearchDlg::OnPaint(PAINT_STATUS status)
{
	GUI::CDialog::OnPaint(status);

	bool bBigScreen = ((GUI::CWnd::m_width >= 1024) && (GUI::CWnd::m_height >= 768)) ? true : false;

	unsigned short x = m_rect.m_left + m_length[DLG_OFFSET_SUB_LEFT], y = m_rect.m_top + m_length[DLG_OFFSET_SUB_TOP];
	unsigned short width = m_rect.Width() - 2 * m_length[DLG_OFFSET_SUB_LEFT] - 2;
	unsigned short height = m_rect.Height() - m_length[DLG_OFFSET_SUB_TOP] - m_length[DLG_OFFSET_SUB_BOTTOM] - m_length[BTN_HEIGHT];
	unsigned short listHeight = ((height - m_length[LIST_HEAD_HEIGHT] - 30/*LIST_CTRL_BOTTOM_HEIGHT*/)/m_length[LIST_EX_ITEM_HEIGHT])*m_length[LIST_EX_ITEM_HEIGHT] + m_length[LIST_HEAD_HEIGHT] + 30/*LIST_CTRL_BOTTOM_HEIGHT*/ - 1;

	//绘制线条
	if(bBigScreen)
	{
		GUI::CWnd::DrawLine(x, y, x, y + listHeight, COLOR_LINE_GRAY, 2);
		GUI::CWnd::DrawLine(x, y, x + width, y, COLOR_LINE_GRAY, 2);
		GUI::CWnd::DrawLine(x + width, y, x + width, y + listHeight, COLOR_LINE_GRAY, 2);
		GUI::CWnd::DrawLine(x, y + listHeight, x + width, y + listHeight, COLOR_LINE_GRAY, 1);
	}

	//绘制页数
	char print_str[512] = {0};
	//char pageInfo [128] = {0};
	if(m_curSnapPic >= 0)
	{
		if (SNAP_PIC_IS_ONLYREAD(m_pSnapPicPrority[m_curSnapPic].nType))
		{
			snprintf(print_str, 128, "%d/%d %s", m_curSnapPic + 1, m_iSnapPicLen, m_pStrTable->String(E_STR_ID_READ_ONLY));
		}		
		/*else if (m_pSnapPicPrority[m_curSnapPic].bLock)
		{
			snprintf(pageInfo, 128, "%d/%d %s", m_curSnapPic + 1, m_iSnapPicLen, m_pStrTable->String(E_STR_ID_LOCK));
		}*/
		else
		{
			snprintf(print_str, 128, "%d/%d", m_curSnapPic + 1, m_iSnapPicLen);
		}
	}
	else
	{
		snprintf(print_str, 128, "%d/%d", 0, m_iSnapPicLen);
	}
	
	unsigned short cx = 0, cy = 0;
	TextSize(print_str, strlen(print_str), cx, cy);
	if(bBigScreen)
	{
		GUI::CWnd::DrawText(print_str, strlen(print_str), x + 5, y + listHeight - m_length[LIST_EX_ITEM_HEIGHT] + 2, cx, 30 - 4);
	}
	else
	{
		GUI::CWnd::DrawText(print_str, strlen(print_str), x + 5, m_rectShowPic.m_bottom - m_length[STATIC_HEIGHT], cx, 30 - 4);
	}

	//绘制图片
	if(m_iImgLen > 0)
	{
		CDrawManager drawMan;
		drawMan.CopyImage(m_pImgData, m_iImgLen, true);
		GUI::CWnd::DrawImage(drawMan.GetData(), drawMan.GetDataLen(), m_rectShowPic.m_left, m_rectShowPic.m_top, m_iImgWidth, m_iImgWidth, m_iImgHeight);	
	}
	else
	{
		unsigned short cx = 0, cy = 0;
		const char* pNoImage = m_pStrTable->String(E_STR_ID_NO_IMAGE);
		TextSize(pNoImage, strlen(pNoImage), cx, cy);
		GUI::CWnd::DrawText(pNoImage, strlen(pNoImage), m_rectShowPic.m_left + (m_rectShowPic.Width() - cx) / 2, m_rectShowPic.m_top + (m_rectShowPic.Height() - cy) / 2, cx, cy, COLOR_TEXT_FOCUS);
	}

	GUI::CWnd::DrawRectBorder(m_rectShowPic.m_left - 1, m_rectShowPic.m_top - 1, m_rectShowPic.Width() + 2, m_rectShowPic.Height() + 2, COLOR_SELECT, 1, 1);

	//绘制字符提示	
	if((m_curSnapPic >= 0) && (m_curSnapPic < m_iSnapPicLen))
	{
		//抓图时间
		std::string str_snap_time;
		CTimeCtrl::GetTimeText(str_snap_time, DVRTimeToTm(m_pSnapPicPrority[m_curSnapPic].snapTime));

		//提示字符
		snprintf(print_str, 512, "%s : %.2d  %s", m_pStrTable->String(E_STR_ID_CH), (m_pSnapPicPrority[m_curSnapPic].nChannel + 1), str_snap_time.c_str());
		
		unsigned short print_x = 0, print_y = 0;
		unsigned short print_cx = 0, print_cy = 0;
		GUI::CWnd::TextSize(print_str, strlen(print_str), print_cx, print_cy);

		if(m_rectShowPic.Width() >= print_cx)
		{
			print_x = m_rectShowPic.m_left + (m_rectShowPic.Width() - print_cx - 24) / 2;
		}
		else
		{
			print_x = m_rectShowPic.m_left - (print_cx - m_rectShowPic.Width() - 24) / 2;
		}

		print_y = bBigScreen ? (m_rectShowPic.m_bottom + 4) : (m_rectShowPic.m_bottom - m_length[STATIC_HEIGHT]);

		GUI::CWnd::DrawText(print_str, strlen(print_str), print_x, print_y, print_cx, print_cy);
		if (m_pSnapPicPrority[m_curSnapPic].bLock)
		{
			GUI::CWnd::DrawImage(BMP_LOCK, print_x - 26, print_y - 50);
		}
		
	}
}

void CPictureSearchDlg::SetCurSnapPic(int index, bool bRefresh)
{
	if(index >= m_iSnapPicLen)
	{
		return;
	}

	if(index >= 0)
	{
		//获取pic
		m_curSnapPic = index;
#if defined (__ENVIRONMENT_LINUX__)
		memset(m_pImgData, 0, sizeof(unsigned char)*m_iImgMaxBuffLen);
		if (CSnapMan::GetInstance()->GetPictureData(m_pSnapPicPrority[m_curSnapPic], m_pImgData, m_iImgWidth, m_iImgHeight, m_iImgLen))
		{
			GetDisplayPicInfo(m_pImgData, m_iImgLen, m_iImgWidth, m_iImgHeight);
			for(int i = 0; i < m_iImgLen; i += 2)
			{
				unsigned short* pShort = (unsigned short*)(m_pImgData + i);
				
				*pShort = (0x8000) | (*pShort);
			}			
		}
		else
		{
			m_iImgWidth = 0;
			m_iImgHeight = 0;
			m_iImgLen = 0;
		}

		m_playbackStarTime = (m_pSnapPicPrority[m_curSnapPic].snapTime)/1000000;
		m_playbackEndTime  = GetEndTime32OfDate(m_playbackStarTime);
		m_playbackPlayTime = m_playbackStarTime;
		m_chnnBits         = ((ULONGLONG)(0x01)) << m_pSnapPicPrority[m_curSnapPic].nChannel;

#else
		GUI::CImage* pImg = GUI::CImageList::Instance()->GetImage(static_cast<BITMAP_ID>(BMP_BTN_SEARCH_1X1 + m_curSnapPic % 30));
		memcpy(m_pImgData, pImg->GetDate(), pImg->Size());
		m_iImgWidth = pImg->Width();
		m_iImgHeight = pImg->Height();
		m_iImgLen = pImg->Size();
#endif
	}
	else
	{
		m_iImgLen = 0;
		m_curSnapPic = -1;
		m_playbackStarTime = 0;
		m_playbackEndTime  = 0;
		m_playbackPlayTime = 0;
		m_chnnBits         = 0;
	}

	//lock btn
	if(m_curSnapPic >= 0)
	{
		SetBtnLockCaption(m_pSnapPicPrority[m_curSnapPic].bLock ? true : false, false);
	}
	else
	{
		SetBtnLockCaption(false, false);
	}

	//
	if((m_curSnapPic >= 0) && (SNAP_PIC_IS_ONLYREAD(m_pSnapPicPrority[m_curSnapPic].nType)))
	{
		SetBtnReadOnly(true, false);
	}
	else
	{
		SetBtnReadOnly(false, false);
	}


	if(bRefresh)
	{
		Repaint();
	}
}

bool CPictureSearchDlg::IsAlmostAbove(unsigned long x1, unsigned long y1, unsigned long x2, unsigned long y2, unsigned long distance)
{
	unsigned long x = (x1>=x2)?(x1-x2):(x2-x1);
	unsigned long y = (y1>=y2)?(y1-y2):(y2-y1);

	if(((x*x) + (y*y)) >= distance)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void CPictureSearchDlg::SetBtnLockCaption(bool bLock, bool bRefresh)
{
	if(bLock)
	{
		m_btnLock.SetCaption(m_pStrTable->String(E_STR_ID_UNLOCKED), bRefresh);
	}
	else
	{
		m_btnLock.SetCaption(m_pStrTable->String(E_STR_ID_LOCK), bRefresh);
	}
}


void CPictureSearchDlg::SetBtnReadOnly(bool bReadOnly, bool bRefresh)
{
	if(bReadOnly)
	{
		m_btnLock.SetCaption(m_pStrTable->String(E_STR_ID_LOCKED), bRefresh);
		m_btnLock.SetEnable(false, bRefresh);
		m_btnDelete.SetEnable(false, bRefresh);
	}
	else
	{
		m_btnLock.SetEnable(true, bRefresh);
		m_btnDelete.SetEnable(true, bRefresh);
	}
}

void CPictureSearchDlg::GetDisplayPicInfo(unsigned char * pRealImgData, int &realDataLen, int &realW, int &realH)
{
	int displayH = 0;
	int displayW = 0;
	bool bD1 = SetDisplayPicParam(realW,realH,displayH,displayW);
	
	if(!bD1)//非D1截图不需要压缩画面
	{
		return;
	}
	
	unsigned short * pImgTemp = new unsigned short [1024 * 1024];
	if (NULL != pImgTemp)
	{
		memset(pImgTemp, 0, 1024 * 1024);
	}

	int xwDiv = realW / displayW;
	int ywDiv = 0;
	int xw = 0;
	int yw = 0;
	if (0 != realW % displayW)
	{
		ywDiv = xwDiv + 1;
		yw = ywDiv * realW - displayW * xwDiv * ywDiv;
		xw = realW - yw;
	}
	else
	{
		xw = realW;
	}

	int xhDiv = realH / displayH;
	int yhDiv = 0;
	int xh = 0;
	int yh = 0;

	if (0 != realH % displayH)
	{
		yhDiv = xhDiv + 1;
		yh = yhDiv * realH - displayH * xhDiv * yhDiv;
		xh = realH - yh;
	}
	else
	{
		xh = realH;
	}

	int pos = 0;
	int h = 0;
	for (h = 0; h < xh; h++)
	{
		if (0 == (h % xhDiv))
		{
			int w = 0;
			for (w=0; w<xw; w++)
			{
				if (0 == w % xwDiv)
				{
					unsigned short * p = (unsigned short *)(pRealImgData + h * realW * 2 + w * 2);
					pImgTemp[pos] = * p;
					pos++;
				}
			}

			for (; w<realW; w++)
			{
				if (0 == w % ywDiv)
				{
					unsigned short * p = (unsigned short *)(pRealImgData + h * realW * 2 + w * 2);
					pImgTemp[pos] = * p;
					pos++;
				}
			}
		}
	}

	for (; h < realH; h++)
	{
		if (0 == (h % yhDiv))
		{
			int w = 0;
			for (w=0; w<xw; w++)
			{
				if (0 == w % xwDiv)
				{
					unsigned short * p = (unsigned short *)(pRealImgData + h * realW * 2 + w * 2);
					pImgTemp[pos] = * p;
					pos++;
				}
			}

			for (; w<realW; w++)
			{
				if (0 == w % ywDiv)
				{
					unsigned short * p = (unsigned short *)(pRealImgData + h * realW * 2 + w * 2);
					pImgTemp[pos] = * p;
					pos++;
				}
			}
		}
	}

	realW = displayW;
	realH = displayH;
	realDataLen = realH * realW * 2;
	memcpy(pRealImgData, pImgTemp, realDataLen);
	delete [] pImgTemp;
	pImgTemp = NULL;

}

bool CPictureSearchDlg::SetDisplayPicParam(int realW, int realH,int &displayH,int &displayW)
{
	unsigned short height = m_rect.Height() - m_length[DLG_OFFSET_SUB_TOP] - m_length[DLG_OFFSET_SUB_BOTTOM] - m_length[BTN_HEIGHT];
	unsigned short width = m_rect.Width() - 2 * m_length[DLG_OFFSET_SUB_LEFT];
	unsigned short listHeight = ((height - m_length[LIST_HEAD_HEIGHT] - 30)/m_length[LIST_EX_ITEM_HEIGHT])*m_length[LIST_EX_ITEM_HEIGHT] + m_length[LIST_HEAD_HEIGHT] + 30;
	

	long pic_x = 0;
	long pic_y = 0;
	
	long pic_cx = 352;
	long pic_cy = 288;
	

	if (realW <=  pic_cx || realH <=  pic_cy)//cif图像不拉伸
	{
		//cif抓图边框
		pic_cx = realW;
		pic_cy = realH;
		//默认图片居中
		pic_x = m_rect.m_left + m_length[DLG_OFFSET_SUB_LEFT] + (width - pic_cx) / 2;
		pic_y = m_rect.m_top + m_length[DLG_OFFSET_SUB_TOP] + (listHeight - pic_cy - m_length[STATIC_HEIGHT]) /  2;
		m_imgDisplayW = pic_cx;
		m_imgDisplayH = pic_cy;
		m_rectShowPic.SetRect(pic_x, pic_y, pic_x + pic_cx, pic_y + pic_cy);
		return false;
	}
	else
	{
		pic_cx = width - m_length[DLG_OFFSET_SUB_LEFT];
		pic_cy = height - 2*m_length[BTN_HEIGHT]-10;
		if(realW <  pic_cx || realH <  pic_cy)//图像大于cif小于d1
		{
			pic_cx = realW;
			pic_cy = realH;
			//默认图片居中
			pic_x = m_rect.m_left + m_length[DLG_OFFSET_SUB_LEFT] + (width - pic_cx) / 2;
			pic_y = m_rect.m_top + m_length[DLG_OFFSET_SUB_TOP] + (listHeight - pic_cy - m_length[STATIC_HEIGHT]) /  2;
			m_imgDisplayW = pic_cx;
			m_imgDisplayH = pic_cy;
			m_rectShowPic.SetRect(pic_x, pic_y, pic_x + pic_cx, pic_y + pic_cy);
			return false;
		}
		else
		{
			pic_cx = 352;
			pic_cy = 240;
			//默认图片居中
			pic_x = m_rect.m_left + m_length[DLG_OFFSET_SUB_LEFT] + (width - pic_cx) / 2;
			pic_y = m_rect.m_top + m_length[DLG_OFFSET_SUB_TOP] + (listHeight - pic_cy - m_length[STATIC_HEIGHT]) /  2;
			m_imgDisplayH = pic_cy;
			displayW = pic_cx;
			displayH = pic_cy;
			m_rectShowPic.SetRect(pic_x, pic_y, pic_x + pic_cx, pic_y + pic_cy);
			return true;
		}
	}
	return false;
}

void CPictureSearchDlg::OnSearch(unsigned long startTime, unsigned long endTime, ULONGLONG chnnBits, unsigned long dataOwnerby)
{
	//添加搜索处理
	MESSAGE_DATA msgData;
	m_searchCond.searchType = DATA_SEARCH_TYPE_PIC;
	m_searchCond.videoCH = chnnBits;
	m_searchCond.startTime = startTime;
	m_searchCond.endTime = endTime;
	m_searchCond.dataOwnerby = dataOwnerby;

	m_playbackStarTime = 0;
	m_playbackEndTime  = 0;
	m_playbackPlayTime = 0;
	m_chnnBits         = 0;

	int retNum = 0;
	
#if defined (__ENVIRONMENT_LINUX__)
	CMessageMan::Instance()->SendMsgDataToServer(LOCAL_DEVICE_ID, LOCAL_DEVICE_ID, CMD_REQUEST_DATA_SEARCH, (unsigned char *)&m_searchCond, sizeof(NET_DATA_SEARCH), msgData);

	SNAP_PIC_PRORITY *pDate = (SNAP_PIC_PRORITY *)(msgData.pData + sizeof(PACKCMD) + sizeof(NET_DATA_INFO));
	retNum = (msgData.dataLen - sizeof(PACKCMD) - sizeof(NET_DATA_INFO))/sizeof(SNAP_PIC_PRORITY);
	if (0 == retNum)
	{
		MessageBox(m_pStrTable->String(E_STR_ID_MESSAGE_36), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK | DEFAULT_ICON_WARNING);
		CMessageMan::Instance()->ReleaseMsgBuff(msgData);	
		return;
	}
#else
	retNum = 20;
#endif

	if(m_iSnapPicMaxLen < retNum)
	{
		if(m_pSnapPicPrority)
		{
			delete [] m_pSnapPicPrority;
			m_pSnapPicPrority = NULL;
		}
		m_iSnapPicMaxLen = retNum + 1;
		m_pSnapPicPrority = new SNAP_PIC_PRORITY[m_iSnapPicMaxLen];
		memset(m_pSnapPicPrority, 0, sizeof(SNAP_PIC_PRORITY) * m_iSnapPicMaxLen);
	}
	
#if defined (__ENVIRONMENT_LINUX__)
	memcpy(m_pSnapPicPrority, pDate, sizeof(SNAP_PIC_PRORITY)*retNum);	
	CMessageMan::Instance()->ReleaseMsgBuff(msgData);	
#else
	for(int i = 0; i < retNum; i++)
	{
		m_pSnapPicPrority[i].snapTime = GetCurrTime();
		m_pSnapPicPrority[i].nChannel = i % 16;
		m_pSnapPicPrority[i].bLock = i % 2;
	}
#endif

	m_iSnapPicLen = retNum;

	if(m_iSnapPicLen > 0)
	{
		SetCurSnapPic(0, true);
	}
	else
	{
		SetCurSnapPic(-1, true);
	}
}

bool CPictureSearchDlg::GetPlaybackInfo(unsigned long &playbackStarTime, unsigned long &playbackEndTime, unsigned long &playbackPlayTime, ULONGLONG &chnnBits)
{
	if ((m_playbackPlayTime > 0) && (m_chnnBits > 0))
	{
		playbackStarTime   = m_playbackStarTime;
		playbackEndTime    = m_playbackEndTime;
		playbackPlayTime   = m_playbackPlayTime;
		chnnBits           = m_chnnBits;
		return true;
	}
	return false;
}


BEGIN_MSG_MAP(CPictureSearchDlg, GUI::CDialog)
	ON_MSG_EX(GetID(), KEY_VIRTUAL_ENTER, OnEnter)
	ON_MSG_EX(GetID(), KEY_VIRTUAL_DRAG, OnMouseDrag)
	ON_MSG_EX(GetID(), KEY_VIRTUAL_BLUP, OnMouseLeftUp)
	ON_MSG_EX(GetID(), KEY_VIRTUAL_DBCLICK, OnDBClick)
	ON_MSG(m_btnLock.GetID(), KEY_VIRTUAL_ENTER, OnClickLock)
	ON_MSG(m_btnDelete.GetID(), KEY_VIRTUAL_ENTER, OnClickDel)
	ON_MSG(m_btnSave.GetID(), KEY_VIRTUAL_ENTER, OnClickSave)	
    ON_MSG(m_btnSaveAll.GetID(), KEY_VIRTUAL_ENTER, OnClickSaveAll)	
	ON_MSG(m_btnFirst.GetID(), KEY_VIRTUAL_ENTER, OnClickFirst)
	ON_MSG(m_btnPrev.GetID(), KEY_VIRTUAL_ENTER, OnClickPrev)
	ON_MSG(m_btnNext.GetID(), KEY_VIRTUAL_ENTER, OnClickNext)
	ON_MSG(m_btnEnd.GetID(), KEY_VIRTUAL_ENTER, OnClickEnd)
END_MSG_MAP()

unsigned long CPictureSearchDlg::OnEnter(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	m_x_enter = xPos;
	m_y_enter = yPos;

	if((m_iSnapPicLen > 0) && m_rectShowPic.PtInRect(xPos, yPos))
	{
		m_bStartDrag = true;
		m_bFirstDrag = true;
	}

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CPictureSearchDlg::OnMouseDrag(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	//拖动切换图片
	if((m_iSnapPicLen > 0) && m_bStartDrag && m_bFirstDrag)
	{
		unsigned long offset_len = ((m_width / 30) * (m_width / 30)) + ((m_height / 30) * (m_height / 30));
		
		if(IsAlmostAbove(xPos, yPos, m_x_enter, m_y_enter, offset_len))
		{
			if(xPos > m_x_enter)
			{
				//下一张
				OnClickNext();
			}
			else
			{
				//上一张
				OnClickPrev();
			}

			m_bFirstDrag = false;
		}
	}

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CPictureSearchDlg::OnMouseLeftUp(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	m_bStartDrag = false;
	m_bFirstDrag = false;

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CPictureSearchDlg::OnDBClick(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
    
	if(m_rectShowPic.PtInRect(xPos, yPos))
	{
		if ((m_iSnapPicLen > 0) && (m_curSnapPic >= 0))
		{
			return KEY_VIRTUAL_PLAY;
		}
	}

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CPictureSearchDlg::OnClickLock()
{
	if ((m_iSnapPicLen > 0) && (m_curSnapPic >= 0))
	{
		MESSAGE_DATA msgData;
		if (m_pSnapPicPrority[m_curSnapPic].bLock)
		{			
			CMessageMan::Instance()->SendMsgDataToServer(LOCAL_DEVICE_ID, LOCAL_DEVICE_ID, CMD_REQUEST_PIC_UNLOCK, (unsigned char *)(&(m_pSnapPicPrority[m_curSnapPic])), sizeof(SNAP_PIC_PRORITY), msgData);
			m_pSnapPicPrority[m_curSnapPic].bLock = false;
		}
		else
		{
			CMessageMan::Instance()->SendMsgDataToServer(LOCAL_DEVICE_ID, LOCAL_DEVICE_ID, CMD_REQUEST_PIC_LOCK, (unsigned char *)(&(m_pSnapPicPrority[m_curSnapPic])), sizeof(SNAP_PIC_PRORITY), msgData);
			m_pSnapPicPrority[m_curSnapPic].bLock = true;
		}
		CMessageMan::Instance()->ReleaseMsgBuff(msgData);

		SetBtnLockCaption(m_pSnapPicPrority[m_curSnapPic].bLock, true);
		Repaint();
	}	

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CPictureSearchDlg::OnClickDel()
{
	if ((m_iSnapPicLen > 0) && (m_curSnapPic >= 0))
	{
		MESSAGE_DATA msgData;
		if (SNAP_PIC_IS_ONLYREAD(m_pSnapPicPrority[m_curSnapPic].nType))
		{
			return KEY_VIRTUAL_MSG_STOP;
		}

		if (m_pSnapPicPrority[m_curSnapPic].bLock)
		{
			MessageBox(m_pStrTable->String(E_STR_ID_MESSAGE_130), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK | DEFAULT_BTN_CANCEL | DEFAULT_ICON_WARNING);
			return KEY_VIRTUAL_MSG_STOP;
		}

		//删除前提示
		if (DLG_OK != MessageBox(m_pStrTable->String(E_STR_ID_MESSAGE_122), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK | DEFAULT_BTN_CANCEL | DEFAULT_ICON_WARNING))
		{
			//放弃则直接返回
			return KEY_VIRTUAL_MSG_STOP;
		}

		CMessageMan::Instance()->SendMsgDataToServer(LOCAL_DEVICE_ID, LOCAL_DEVICE_ID, CMD_REQUEST_PIC_DELETE, (unsigned char *)(&(m_pSnapPicPrority[m_curSnapPic])), sizeof(SNAP_PIC_PRORITY), msgData);
		CMessageMan::Instance()->ReleaseMsgBuff(msgData);

		m_playbackStarTime = 0;
		m_playbackEndTime  = 0;
		m_playbackPlayTime = 0;
		m_chnnBits         = 0;
	
		int retNum = 0;

#if defined (__ENVIRONMENT_LINUX__)
		CMessageMan::Instance()->SendMsgDataToServer(LOCAL_DEVICE_ID, LOCAL_DEVICE_ID, CMD_REQUEST_DATA_SEARCH, (unsigned char *)&m_searchCond, sizeof(NET_DATA_SEARCH), msgData);

		SNAP_PIC_PRORITY *pDate = (SNAP_PIC_PRORITY *)(msgData.pData + sizeof(PACKCMD) + sizeof(NET_DATA_INFO));
		retNum = (msgData.dataLen - sizeof(PACKCMD) - sizeof(NET_DATA_INFO))/sizeof(SNAP_PIC_PRORITY);		
#else
		retNum = 20;
#endif

		if(m_iSnapPicMaxLen < retNum)
		{
			if(m_pSnapPicPrority)
			{
				delete [] m_pSnapPicPrority;
				m_pSnapPicPrority = NULL;
			}
			m_iSnapPicMaxLen = retNum + 1;
			m_pSnapPicPrority = new SNAP_PIC_PRORITY[m_iSnapPicMaxLen];
			memset(m_pSnapPicPrority, 0, sizeof(SNAP_PIC_PRORITY) * m_iSnapPicMaxLen);
		}

#if defined (__ENVIRONMENT_LINUX__)
		if (retNum > 0)
		{
			memcpy(m_pSnapPicPrority, pDate, sizeof(SNAP_PIC_PRORITY)*retNum);	
		}		
		CMessageMan::Instance()->ReleaseMsgBuff(msgData);	
#else
		for(int i = 0; i < retNum; i++)
		{
			m_pSnapPicPrority[i].snapTime = GetCurrTime();
			m_pSnapPicPrority[i].nChannel = i % 16;
			m_pSnapPicPrority[i].bLock = i % 2;
		}
#endif

		m_iSnapPicLen = retNum;

		if(m_iSnapPicLen > 0)
		{
			int curIndex = m_curSnapPic;
			if (curIndex == m_iSnapPicLen)
			{
				curIndex--;
			}
			SetCurSnapPic(curIndex, true);
		}
		else
		{
			SetCurSnapPic(-1, true);
		}		
	}
	
	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CPictureSearchDlg::OnClickSave()
{
#ifdef __ENVIRONMENT_LINUX__
	if ((m_iSnapPicLen > 0) && (m_curSnapPic >= 0))
	{
		MESSAGE_DATA msgData;
		memset(&msgData, 0, sizeof(MESSAGE_DATA));
		PACKCMD *pCmd = NULL;
		unsigned long cmdType;

		DVR_DISK_INFO *pPartionInfo = NULL;
		int partionNum = 0;

		//查找所有的热拔插设备的分区信息
		CMessageMan::Instance()->SendMsgDataToServer(0, LOCAL_DEVICE_ID, CMD_REQUEST_DISK_GET_REMOVABLE_PARTION, NULL, 0, msgData);
		pPartionInfo = (DVR_DISK_INFO *)(msgData.pData + sizeof(PACKCMD));
		partionNum = (msgData.dataLen - sizeof(PACKCMD))/sizeof(DVR_DISK_INFO);
		if (0 == partionNum)
		{
			CMessageMan::Instance()->ReleaseMsgBuff(msgData);
			MessageBox(m_pStrTable->String(E_STR_ID_MESSAGE_07),m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK | DEFAULT_ICON_WARNING);
			GetRootWnd()->Repaint();
			return KEY_VIRTUAL_MSG_STOP;
		}

		bool bSaveOk = false;
		for (int index = 0; index < partionNum; ++index)
		{
			char szCmd[512] = {0};
			snprintf(szCmd, 512, "umount %s", SNAP_PIC_SAVE_PATH);
			DVRSystem(szCmd);
			snprintf(szCmd, 512, "mkdir -p %s", SNAP_PIC_SAVE_PATH);
			DVRSystem(szCmd);

			snprintf(szCmd, 512, "mount -t vfat %s %s/", pPartionInfo[index].diskname, SNAP_PIC_SAVE_PATH);
			if (DVRSystem(szCmd) != 0)
			{
				continue;				
			}

			snprintf(szCmd, 512, "mkdir -p %s/dvrpicture", SNAP_PIC_SAVE_PATH);
			if (DVRSystem(szCmd) != 0)
			{
				snprintf(szCmd, 512, "umount %s", SNAP_PIC_SAVE_PATH);
				DVRSystem(szCmd);
				continue;
			}

			tm strTime = DVRTimeToTm(m_pSnapPicPrority[m_curSnapPic].snapTime);
			snprintf(szCmd, 512, "%s/dvrpicture/CH%02d_%04d_%02d_%02d_%02d_%02d_%02d.jpg", SNAP_PIC_SAVE_PATH,  m_pSnapPicPrority[m_curSnapPic].nChannel+1,
				strTime.tm_year+1900, strTime.tm_mon+1, strTime.tm_mday, strTime.tm_hour, strTime.tm_min, strTime.tm_sec);
			FILE *pFile = fopen(szCmd, "wb");
			if (NULL == pFile)
			{
				snprintf(szCmd, 512, "umount %s", SNAP_PIC_SAVE_PATH);
				DVRSystem(szCmd);
				continue;
			}

			if (!CSnapMan::GetInstance()->WritePictureToFile(m_pSnapPicPrority[m_curSnapPic], pFile))
			{
				fclose(pFile);
				pFile = NULL;
				snprintf(szCmd, 512, "umount %s", SNAP_PIC_SAVE_PATH);
				DVRSystem(szCmd);
				continue;				
			}

			fclose(pFile);
			pFile = NULL;
			snprintf(szCmd, 512, "umount %s", SNAP_PIC_SAVE_PATH);
			DVRSystem(szCmd);
			bSaveOk = true;
			break;
		}

		CMessageMan::Instance()->ReleaseMsgBuff(msgData);

		if (bSaveOk)
		{		
			char str[512] = {0};
			snprintf(str, 512, "%s (%d/%d)", m_pStrTable->String(E_STR_ID_MESSAGE_09), 1, 1);
			MessageBox(str,m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK | DEFAULT_ICON_WARNING);
		}
		else
		{
			MessageBox(m_pStrTable->String(E_STR_ID_MESSAGE_07),m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK | DEFAULT_ICON_WARNING);
		}

		GetRootWnd()->Repaint();		
	}
#endif
	return KEY_VIRTUAL_MSG_STOP;

}


unsigned long CPictureSearchDlg::OnClickSaveAll()
{
#ifdef __ENVIRONMENT_LINUX__
	if (m_iSnapPicLen > 0)
	{
		MESSAGE_DATA msgData;
		memset(&msgData, 0, sizeof(MESSAGE_DATA));
		PACKCMD *pCmd = NULL;
		unsigned long cmdType;

		DVR_DISK_INFO *pPartionInfo = NULL;
		int partionNum = 0;

		//查找所有的热拔插设备的分区信息
		CMessageMan::Instance()->SendMsgDataToServer(0, LOCAL_DEVICE_ID, CMD_REQUEST_DISK_GET_REMOVABLE_PARTION, NULL, 0, msgData);
		pPartionInfo = (DVR_DISK_INFO *)(msgData.pData + sizeof(PACKCMD));
		partionNum = (msgData.dataLen - sizeof(PACKCMD))/sizeof(DVR_DISK_INFO);
		if (0 == partionNum)
		{
			GetRootWnd()->Repaint();
			MessageBox(m_pStrTable->String(E_STR_ID_MESSAGE_07),m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK | DEFAULT_ICON_WARNING);
			return KEY_VIRTUAL_MSG_STOP;
		}

		bool bSaveOk = false;
		int snapPicIndex = 0;
		for (int index = 0; index < partionNum; ++index)
		{
			char szCmd[512] = {0};
			snprintf(szCmd, 512, "umount %s", SNAP_PIC_SAVE_PATH);
			DVRSystem(szCmd);
			snprintf(szCmd, 512, "mkdir -p %s", SNAP_PIC_SAVE_PATH);
			DVRSystem(szCmd);

			snprintf(szCmd, 512, "mount -t vfat %s %s/", pPartionInfo[index].diskname, SNAP_PIC_SAVE_PATH);
			if (DVRSystem(szCmd) != 0)
			{
				continue;
			}

			snprintf(szCmd, 512, "mkdir -p %s/dvrpicture", SNAP_PIC_SAVE_PATH);
			if (DVRSystem(szCmd) != 0)
			{
				snprintf(szCmd, 512, "umount %s", SNAP_PIC_SAVE_PATH);
				DVRSystem(szCmd);
				continue;
			}

			snapPicIndex = 0;		
			for (snapPicIndex = 0; snapPicIndex < m_iSnapPicLen; ++snapPicIndex)
			{
				tm strTime = DVRTimeToTm(m_pSnapPicPrority[snapPicIndex].snapTime);
				snprintf(szCmd, 512, "%s/dvrpicture/CH%02d_%04d_%02d_%02d_%02d_%02d_%02d.jpg", SNAP_PIC_SAVE_PATH,  m_pSnapPicPrority[snapPicIndex].nChannel+1,
					strTime.tm_year+1900, strTime.tm_mon+1, strTime.tm_mday, strTime.tm_hour, strTime.tm_min, strTime.tm_sec);

				FILE *pFile = fopen(szCmd, "wb");
				if (NULL == pFile)
				{
					break;
				}

				if (!CSnapMan::GetInstance()->WritePictureToFile(m_pSnapPicPrority[snapPicIndex], pFile))
				{
					fclose(pFile);
					pFile = NULL;
					break;			
				}

				fclose(pFile);
				pFile = NULL;
			}

			if (0 == snapPicIndex)
			{
				snprintf(szCmd, 512, "umount %s", SNAP_PIC_SAVE_PATH);
				DVRSystem(szCmd);
				continue;
			}
			else
			{
				snprintf(szCmd, 512, "umount %s", SNAP_PIC_SAVE_PATH);
				DVRSystem(szCmd);
				bSaveOk = true;
				break;
			}
		}
		CMessageMan::Instance()->ReleaseMsgBuff(msgData);
		
		if (bSaveOk)
		{
			char str[512] = {0};
			snprintf(str, 512, "%s (%d/%d)", m_pStrTable->String(E_STR_ID_MESSAGE_09), snapPicIndex, m_iSnapPicLen);
			MessageBox(str,m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK | DEFAULT_ICON_WARNING);
		}
		else
		{
			MessageBox(m_pStrTable->String(E_STR_ID_MESSAGE_07),m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK | DEFAULT_ICON_WARNING);
		}	
		GetRootWnd()->Repaint();
	}
#endif
	return KEY_VIRTUAL_MSG_STOP;

}

unsigned long CPictureSearchDlg::OnClickFirst()
{
	if(m_iSnapPicLen > 0)
	{
		SetCurSnapPic(0, true);
	}

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CPictureSearchDlg::OnClickPrev()
{
	int index = m_curSnapPic - 1;

	if((m_iSnapPicLen > 0) && (index >= 0))
	{
		SetCurSnapPic(index, true);
	}

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CPictureSearchDlg::OnClickNext()
{
	int index = m_curSnapPic + 1;

	if((m_iSnapPicLen > 0) && (index < m_iSnapPicLen))
	{
		SetCurSnapPic(index, true);
	}

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CPictureSearchDlg::OnClickEnd()
{
	if(m_iSnapPicLen > 0)
	{
		SetCurSnapPic(m_iSnapPicLen - 1, true);
	}

	return KEY_VIRTUAL_MSG_STOP;
}

//end
