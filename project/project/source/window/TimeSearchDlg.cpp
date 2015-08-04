/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : ysw
** Date         : 2009-08-01
** Name         : TimeSearchDlg.cpp
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/
#include "TimeSearchDlg.h"
#include "NetProtocol.h"
#include "FuncCustom.h"

extern NET_LOGIN_SUCCESS_REPLY *g_p_login_succ;
//////////////////////////////////////////////////////////////////////////
CTimeSearchDlg::CTimeSearchDlg()
{
	m_selType = TIME_SEARCH_SEL_TYPE_1X1;
	m_selChnn = 0x1;
	m_channelNum = 0;
	m_videoOuputNum = 0;
	m_diskOwnerIndex = 0;

	m_authCH = 0;
	m_scheduleCH = 0;
}

CTimeSearchDlg::~CTimeSearchDlg()
{   

}
  
void CTimeSearchDlg::SetInitialInfo(unsigned long channelNum, unsigned long videoOutNum, unsigned long diskOwnerIndex, ULONGLONG authCH)
{
	m_channelNum = channelNum;
	m_videoOuputNum = videoOutNum;
	m_diskOwnerIndex = diskOwnerIndex;
	m_authCH	= authCH;

	//默认按照最大的模式来设置播放参数
	if (m_videoOuputNum == 1)
	{
		m_selType = TIME_SEARCH_SEL_TYPE_1X1;
	}
	if (m_videoOuputNum <= 4)
	{
		m_selType = TIME_SEARCH_SEL_TYPE_2X2;
	}
	else if (m_videoOuputNum <= 6)
	{
		m_selType = TIME_SEARCH_SEL_TYPE_2X3;
	}
	else if (m_videoOuputNum <= 9)
	{
		m_selType = TIME_SEARCH_SEL_TYPE_3X3;
	}
	else
	{
		m_selType = TIME_SEARCH_SEL_TYPE_4X4;
	}

	m_selChnn = (m_authCH & ALL_CHNN_MASK(m_videoOuputNum));
}

//////////////////////////////////////////////////////////////////////////
void CTimeSearchDlg::OnInitial()
{
	assert(m_channelNum > 0);
	GUI::CDialog::OnInitial();

	ULONGLONG CH = 0;
	int localVideoInputNum = 0, netVideoInputNum = 0;
	int maxChnn = 0, effectChnn = 0;

	IsSupportCH(CH, &effectChnn, &localVideoInputNum, &netVideoInputNum);

	maxChnn = localVideoInputNum + netVideoInputNum;

	//////////////////////////////////////////////////////////////////////////
	unsigned short xLeft = m_length[DLG_OFFSET_SUB_LEFT], yTop = m_length[DLG_OFFSET_SUB_TOP];
	unsigned short x = xLeft, y = yTop;
	unsigned short cx = 50, cy = 40;
	unsigned short xOff = 15;
	char sz[64] = {0};

	unsigned char iTop = 0;
	if (1 < m_videoOuputNum)
	{
		iTop = 1;
	}

	if (4 < m_videoOuputNum)
	{
		iTop = 2;
	}

	if (6 < m_videoOuputNum)
	{
		iTop = 3;
	}

	if (9 < m_videoOuputNum)
	{
		iTop = 4;
	}

	maxChnn = m_channelNum;
	int iMax[7] = {1,4,6,8,16,24,32};
	for ( int i = 0; i < 5; i++ )
	{
		if( maxChnn < iMax[i] || i > iTop) 
		{
			break;
		}
		m_btnChnn[i].Create( GUI::CWnd::GetRandWndID(), x, y, cx, cy, this, i, 0 );
		x += cx;
	}

	m_btnPaly.Create( GUI::CWnd::GetRandWndID(), x, y, cx, cy, this, 5, 0 );
	x += cx;

	{
		y -= 7;
		x += xOff;
		m_scStartTm.Create( GUI::CWnd::GetRandWndID(), x, y, 100, m_length[TIME_CTRL_HEIGHT], this);
		m_scStartTm.SetCaption(m_pStrTable->String(E_STR_ID_START_TIME), false );

		y += 24;
		m_dateStart.Create(GetRandWndID(),x,y,m_length[DATE_CTRL_WIDTH],m_length[DATE_CTRL_HEIGHT],this,6, 0);

		x += m_length[DATE_CTRL_WIDTH]+2;
		m_tmStart.Create( GUI::CWnd::GetRandWndID(), x, y, m_length[TIME_CTRL_WIDTH], m_length[TIME_CTRL_HEIGHT], this, 7, 0);
		
		tm tmpTm = DVRTime32ToTm(GetZeroTime32OfDate(GetCurrTime32()));

		m_dateStart.SetDate(tmpTm.tm_year+1900,tmpTm.tm_mon,tmpTm.tm_mday,false);
		m_tmStart.SetTime(tmpTm.tm_hour,tmpTm.tm_min,tmpTm.tm_sec,false);
	}

	x = xLeft;
	y = cy + 20;

	cy = m_rect.Height() - y - m_length[DLG_OFFSET_SUB_BOTTOM];
	int maxRow = (cy - m_length[SCHEDULE_HEADER_HEIGHT])/m_length[SCHEDULE_ITEM_HEIGHT];
	if( maxRow < maxChnn )
	{
		maxRow -= 1;
	}
	else
	{
		maxRow = maxChnn;
	}

	int tileWidth = 36;

	m_scheduleList.Create(GetRandWndID(), x, y, m_rect.Width() - 2 * m_length[DLG_OFFSET_SUB_LEFT], cy, this, 0, 1, tileWidth, NULL, maxRow);

	m_scheduleCH = 0;
	for (unsigned char chnn = 0; chnn < maxChnn; ++chnn)
	{
		snprintf(sz, 64, "%d", chnn + 1 );
		m_scheduleList.AddItem(chnn, sz, false);
		m_scheduleCH |= ((ULONGLONG)(0x01) << chnn);
	}
	m_scheduleList.SetShowDotFlag();

	// 1X1
	m_btnChnn[0].SetStateImage(CHNN_SELECT_NO, PAINT_STATUS_NORMAL, BMP_BTN_SEARCH_1X1);
	m_btnChnn[0].SetStateImage(CHNN_SELECT_NO, PAINT_STATUS_DISABLE, BMP_BTN_SEARCH_1X1_N);
	m_btnChnn[0].SetStateImage(CHNN_SELECT_NO, PAINT_STATUS_FOCUS, BMP_BTN_SEARCH_1X1_F);
	m_btnChnn[0].SetStateImage(CHNN_SELECT_YES, PAINT_STATUS_NORMAL, BMP_BTN_SEARCH_1X1_SEL);
	m_btnChnn[0].SetStateImage(CHNN_SELECT_YES, PAINT_STATUS_DISABLE, BMP_BTN_SEARCH_1X1_SEL);
	m_btnChnn[0].SetStateImage(CHNN_SELECT_YES, PAINT_STATUS_FOCUS, BMP_BTN_SEARCH_1X1_SEL);
	// 2X2
	m_btnChnn[1].SetStateImage(CHNN_SELECT_NO, PAINT_STATUS_NORMAL, BMP_BTN_SEARCH_2X2);
	m_btnChnn[1].SetStateImage(CHNN_SELECT_NO, PAINT_STATUS_DISABLE, BMP_BTN_SEARCH_2X2_N);
	m_btnChnn[1].SetStateImage(CHNN_SELECT_NO, PAINT_STATUS_FOCUS, BMP_BTN_SEARCH_2X2_F);
	m_btnChnn[1].SetStateImage(CHNN_SELECT_YES, PAINT_STATUS_NORMAL, BMP_BTN_SEARCH_2X2_SEL);
	m_btnChnn[1].SetStateImage(CHNN_SELECT_YES, PAINT_STATUS_DISABLE, BMP_BTN_SEARCH_2X2_SEL);
	m_btnChnn[1].SetStateImage(CHNN_SELECT_YES, PAINT_STATUS_FOCUS, BMP_BTN_SEARCH_2X2_SEL);

	// 2X3
	m_btnChnn[2].SetStateImage(CHNN_SELECT_NO, PAINT_STATUS_NORMAL, BMP_BTN_SEARCH_2X3);
	m_btnChnn[2].SetStateImage(CHNN_SELECT_NO, PAINT_STATUS_DISABLE, BMP_BTN_SEARCH_2X3_N);
	m_btnChnn[2].SetStateImage(CHNN_SELECT_NO, PAINT_STATUS_FOCUS, BMP_BTN_SEARCH_2X3_F);
	m_btnChnn[2].SetStateImage(CHNN_SELECT_YES, PAINT_STATUS_NORMAL, BMP_BTN_SEARCH_2X3_SEL);
	m_btnChnn[2].SetStateImage(CHNN_SELECT_YES, PAINT_STATUS_DISABLE, BMP_BTN_SEARCH_2X3_SEL);
	m_btnChnn[2].SetStateImage(CHNN_SELECT_YES, PAINT_STATUS_FOCUS, BMP_BTN_SEARCH_2X3_SEL);

	// 3X3
	m_btnChnn[3].SetStateImage(CHNN_SELECT_NO, PAINT_STATUS_NORMAL, BMP_BTN_SEARCH_3X3);
	m_btnChnn[3].SetStateImage(CHNN_SELECT_NO, PAINT_STATUS_DISABLE, BMP_BTN_SEARCH_3X3_N);
	m_btnChnn[3].SetStateImage(CHNN_SELECT_NO, PAINT_STATUS_FOCUS, BMP_BTN_SEARCH_3X3_F);
	m_btnChnn[3].SetStateImage(CHNN_SELECT_YES, PAINT_STATUS_NORMAL, BMP_BTN_SEARCH_3X3_SEL);
	m_btnChnn[3].SetStateImage(CHNN_SELECT_YES, PAINT_STATUS_DISABLE, BMP_BTN_SEARCH_3X3_SEL);
	m_btnChnn[3].SetStateImage(CHNN_SELECT_YES, PAINT_STATUS_FOCUS, BMP_BTN_SEARCH_3X3_SEL);

	//4X4
	m_btnChnn[4].SetStateImage(CHNN_SELECT_NO, PAINT_STATUS_NORMAL, BMP_BTN_SEARCH_4X4);
	m_btnChnn[4].SetStateImage(CHNN_SELECT_NO, PAINT_STATUS_DISABLE, BMP_BTN_SEARCH_4X4_N);
	m_btnChnn[4].SetStateImage(CHNN_SELECT_NO, PAINT_STATUS_FOCUS, BMP_BTN_SEARCH_4X4_F);
	m_btnChnn[4].SetStateImage(CHNN_SELECT_YES, PAINT_STATUS_NORMAL, BMP_BTN_SEARCH_4X4_SEL);
	m_btnChnn[4].SetStateImage(CHNN_SELECT_YES, PAINT_STATUS_DISABLE, BMP_BTN_SEARCH_4X4_SEL);
	m_btnChnn[4].SetStateImage(CHNN_SELECT_YES, PAINT_STATUS_FOCUS, BMP_BTN_SEARCH_4X4_SEL);

	//
	m_btnPaly.SetImage(PAINT_STATUS_NORMAL, BMP_BTN_SEARCH_PLAY, false);
	m_btnPaly.SetImage(PAINT_STATUS_DISABLE, BMP_BTN_SEARCH_PLAY, false);
	m_btnPaly.SetImage(PAINT_STATUS_FOCUS, BMP_BTN_SEARCH_PLAY_F, false);

	{
		//处理分割按钮的状态
		for (unsigned char i=0; i<iTop; ++i)
		{
			m_btnChnn[i].SetCurState(CHNN_SELECT_NO, false);
		}
		
		//选择取大通道数目对应的按钮为默认
		m_btnChnn[iTop].SetCurState(CHNN_SELECT_YES, false);


		//把不支持的数目对应的按钮给不使能
		//for (unsigned char i=iTop+1; i<5; ++i)
		//{
		//	m_btnChnn[i].SetCurState(CHNN_SELECT_NO, false);
		//	m_btnChnn[i].SetEnable(false, false);
		//}
	}

	{
		//2010-02-08 12:20:00 YSW
		//默认把能支持的最大通道数目组合设为选择状态。
		for (unsigned char chnn = 0; chnn < m_videoOuputNum; ++chnn)
		{
			if (m_scheduleCH & ((ULONGLONG)(0x01) << chnn))
			{
				m_scheduleList.SetMarkItem(chnn, true, false);
			}
		}
	}
}

void CTimeSearchDlg::OnPostInitial()
{
	//处理一开界面，就把当前的数据分布图绘制出来。
#ifndef __TD2316SES_SPEC__
	MESSAGE_DATA msgData;
	NET_DATA_SEARCH searchCond;

	unsigned long startTime = GetZeroTime32OfDate(GetCurrTime32());
	unsigned long endTime = GetEndTime32OfDate(startTime);

	searchCond.searchType  = DATA_SEARCH_TYPE_TIME;
	searchCond.videoCH     = m_authCH;
	searchCond.startTime   = startTime;
	searchCond.endTime     = endTime;
	searchCond.dataOwnerby = m_diskOwnerIndex;

	memset(&msgData, 0, sizeof(MESSAGE_DATA));
	CMessageMan::Instance()->SendMsgDataToServer(0, LOCAL_DEVICE_ID, CMD_REQUEST_DATA_SEARCH, (unsigned char *)&searchCond, sizeof(NET_DATA_SEARCH), msgData);

	assert ((sizeof(PACKCMD) + sizeof(NET_DATA_INFO)) <= msgData.dataLen);
	NET_SECTION_INFO *pSectionInfo = (NET_SECTION_INFO *)(msgData.pData + sizeof(PACKCMD) + sizeof(NET_DATA_INFO));

	tm starTm, endTm;
	unsigned long num = (msgData.dataLen - sizeof(PACKCMD) - sizeof(NET_DATA_INFO))/sizeof(NET_SECTION_INFO);

	for (unsigned long i = 0; i < num; ++i)
	{
		if (pSectionInfo[i].starTime < startTime)
		{
			pSectionInfo[i].starTime = startTime;
		}
		
		//
		if (endTime < pSectionInfo[i].endTime)
		{
			pSectionInfo[i].endTime = endTime;
		}

		starTm = DVRTime32ToTm(pSectionInfo[i].starTime);
		endTm  = DVRTime32ToTm(pSectionInfo[i].endTime);
		m_scheduleList.AddData(pSectionInfo[i].chnnnel, starTm.tm_hour, starTm.tm_min, starTm.tm_sec, endTm.tm_hour, endTm.tm_min, endTm.tm_sec);		
	}

	CMessageMan::Instance()->ReleaseMsgBuff(msgData);
#endif
}
//////////////////////////////////////////////////////////////////////////

BEGIN_MSG_MAP(CTimeSearchDlg, GUI::CDialog)
	ON_MSG(m_btnChnn[0].GetID(), KEY_VIRTUAL_ENTER, OnBtnChnn1X1)
	ON_MSG(m_btnChnn[1].GetID(), KEY_VIRTUAL_ENTER, OnBtnChnn2X2)
	ON_MSG(m_btnChnn[2].GetID(), KEY_VIRTUAL_ENTER, OnBtnChnn2X3)
	ON_MSG(m_btnChnn[3].GetID(), KEY_VIRTUAL_ENTER, OnBtnChnn3X3)
	ON_MSG(m_btnChnn[4].GetID(), KEY_VIRTUAL_ENTER, OnBtnChnn4X4)
	ON_MSG(m_btnPaly.GetID(),    KEY_VIRTUAL_ENTER, OnBtnPlayback)
	ON_MSG_EX(m_scheduleList.GetID(),KEY_VIRTUAL_ENTER,OnClickSchedule)

	ON_MSG(CTRLID_NULL, KEY_VIRTUAL_PLAY, OnBtnPlayback)
	ON_MSG(CTRLID_NULL, KEY_VIRTUAL_1X1, OnBtnChnn1X1)
	ON_MSG(CTRLID_NULL, KEY_VIRTUAL_2X2, OnBtnChnn2X2)
	ON_MSG(CTRLID_NULL, KEY_VIRTUAL_2X3, OnBtnChnn2X3)
	ON_MSG(CTRLID_NULL, KEY_VIRTUAL_3X3, OnBtnChnn3X3)
	ON_MSG(CTRLID_NULL, KEY_VIRTUAL_4X4, OnBtnChnn4X4)
END_MSG_MAP()


unsigned long CTimeSearchDlg::OnBtnChnn1X1()
{
	GUI::CRect rect = m_btnChnn[0].GetWindowRect();
	unsigned short x = rect.m_left;
	unsigned short y = rect.m_bottom;

	int maxSel = 1; // min(1, 产品回放通道最大值)
	maxSel = (m_videoOuputNum > maxSel) ? maxSel : m_videoOuputNum;
	ULONGLONG selChnn = 0x1;
	if( TIME_SEARCH_SEL_TYPE_1X1 == m_selType )
	{
		selChnn = m_selChnn;
	}

	int numChnn = g_p_login_succ->productInfo.localVideoInputNum;
	unsigned long ipcMaxVideoOutNum = CProduct::Instance()->IPCMaxVideoOutNum();
	if(ipcMaxVideoOutNum >= 1)
	{
		numChnn += g_p_login_succ->productInfo.netVideoInputNum;
	}

	m_selectChnnDlg.SetChnnCount( numChnn , m_authCH, maxSel, selChnn );
	GUI::CRect chnnRect = m_selectChnnDlg.GetDlgDefaultRect();
	if( DLG_OK == m_selectChnnDlg.DoModal( this, x, y, chnnRect.Width(), chnnRect.Height() ) )
	{
		m_selType = TIME_SEARCH_SEL_TYPE_1X1;
		m_selChnn = m_selectChnnDlg.GetSelChnn();

		//2010-02-08 12:20:00 YSW
		//默认把能支持的最大通道数目组合设为选择状态。
		for (unsigned char chnn = 0; chnn < m_channelNum; ++chnn)
		{
			if (m_selChnn & (m_scheduleCH & ((ULONGLONG)(0x01) << chnn)))
			{
				m_scheduleList.SetMarkItem(chnn, true, true);
			}
			else if (m_scheduleCH & ((ULONGLONG)(0x01) << chnn))
			{
				m_scheduleList.SetMarkItem(chnn, false, true);
			}
			else
			{
				//应该不存在该项
			}
		}

		RefreshBtnSplit(m_selType);
	}

	return KEY_VIRTUAL_MSG_STOP;
}
unsigned long CTimeSearchDlg::OnBtnChnn2X2()
{
	GUI::CRect rect = m_btnChnn[1].GetWindowRect();
	unsigned short x = rect.m_left;
	unsigned short y = rect.m_bottom;

	int numChnn = g_p_login_succ->productInfo.localVideoInputNum;
	unsigned long ipcMaxVideoOutNum = CProduct::Instance()->IPCMaxVideoOutNum();
	if(ipcMaxVideoOutNum)
	{
		if (ipcMaxVideoOutNum >= 4)
		{
			numChnn += g_p_login_succ->productInfo.netVideoInputNum;
		}
	}
	if( numChnn <= 4 )
	{
		m_selType = TIME_SEARCH_SEL_TYPE_2X2;
		//m_selChnn = 0xF;
		m_selChnn = (0xF & m_authCH);

		//2010-05-23 18:11:00 YSW
		//默认把能支持的最大通道数目组合设为选择状态。
		for (unsigned char chnn = 0; chnn < m_channelNum; ++chnn)
		{
			if (m_selChnn & (m_scheduleCH & ((ULONGLONG)(0x01) << chnn)))
			{
				m_scheduleList.SetMarkItem(chnn, true, true);
			}
			else if (m_scheduleCH & ((ULONGLONG)(0x01) << chnn))
			{
				m_scheduleList.SetMarkItem(chnn, false, true);
			}
			else
			{
				//应该不存在该项
			}
		}

		RefreshBtnSplit(m_selType);

		return KEY_VIRTUAL_MSG_STOP;
	}
	int maxSel = 4; // min(4, 产品回放通道最大值)
	maxSel = (m_videoOuputNum > maxSel) ? maxSel : m_videoOuputNum;
	ULONGLONG selChnn = 0xF;
	if( TIME_SEARCH_SEL_TYPE_2X2 == m_selType ) 
	{
		selChnn = m_selChnn;
	}

	m_selectChnnDlg.SetChnnCount( numChnn , m_authCH, maxSel, selChnn );
	GUI::CRect chnnRect = m_selectChnnDlg.GetDlgDefaultRect();
	if( DLG_OK == m_selectChnnDlg.DoModal( this, x, y, chnnRect.Width(), chnnRect.Height() ) )
	{
		m_selType = TIME_SEARCH_SEL_TYPE_2X2;
		m_selChnn = m_selectChnnDlg.GetSelChnn();

		//2010-02-08 12:20:00 YSW
		//默认把能支持的最大通道数目组合设为选择状态。
		for (unsigned char chnn = 0; chnn < m_channelNum; ++chnn)
		{
			if (m_selChnn & (m_scheduleCH & ((ULONGLONG)(0x01) << chnn)))
			{
				m_scheduleList.SetMarkItem(chnn, true, true);
			}
			else if (m_scheduleCH & ((ULONGLONG)(0x01) << chnn))
			{
				m_scheduleList.SetMarkItem(chnn, false, true);
			}
			else
			{
				//应该不存在该项
			}
		}

		RefreshBtnSplit(m_selType);
	}

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CTimeSearchDlg::OnBtnChnn2X3()
{
	GUI::CRect rect = m_btnChnn[2].GetWindowRect();
	unsigned short x = rect.m_left;
	unsigned short y = rect.m_bottom;

	int numChnn = g_p_login_succ->productInfo.localVideoInputNum;
	unsigned long ipcMaxVideoOutNum = CProduct::Instance()->IPCMaxVideoOutNum();
	if( numChnn && ipcMaxVideoOutNum )
	{
		//只有本地通道
	}
	else
	{
		if(ipcMaxVideoOutNum >= 6)
		{
			numChnn += g_p_login_succ->productInfo.netVideoInputNum;
		}
	}


	int maxSel = 6; // min(6, 产品回放通道最大值)
	if( maxSel > numChnn )
	{
		maxSel = numChnn;
	}
	maxSel = (m_videoOuputNum > maxSel) ? maxSel : m_videoOuputNum;
	ULONGLONG selChnn = 0x3F;
	if( TIME_SEARCH_SEL_TYPE_2X3 == m_selType )
	{
		selChnn = m_selChnn;
	}

	m_selectChnnDlg.SetChnnCount( numChnn , m_authCH, maxSel, selChnn );
	GUI::CRect chnnRect = m_selectChnnDlg.GetDlgDefaultRect();
	if( DLG_OK == m_selectChnnDlg.DoModal( this, x, y, chnnRect.Width(), chnnRect.Height() ) )
	{
		m_selType = TIME_SEARCH_SEL_TYPE_2X3;
		m_selChnn = m_selectChnnDlg.GetSelChnn();

		//2010-02-08 12:20:00 YSW
		//默认把能支持的最大通道数目组合设为选择状态。
		for (unsigned char chnn = 0; chnn < m_channelNum; ++chnn)
		{
			if (m_selChnn & (m_scheduleCH & ((ULONGLONG)(0x01) << chnn)))
			{
				m_scheduleList.SetMarkItem(chnn, true, true);
			}
			else if (m_scheduleCH & ((ULONGLONG)(0x01) << chnn))
			{
				m_scheduleList.SetMarkItem(chnn, false, true);
			}
			else
			{
				//应该不存在该项
			}
		}

		RefreshBtnSplit(m_selType);
	}

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CTimeSearchDlg::OnBtnChnn3X3()
{
	GUI::CRect rect = m_btnChnn[3].GetWindowRect();
	unsigned short x = rect.m_left;
	unsigned short y = rect.m_bottom;

	int numChnn = g_p_login_succ->productInfo.localVideoInputNum;
	unsigned long ipcMaxVideoOutNum = CProduct::Instance()->IPCMaxVideoOutNum();
	if( numChnn && ipcMaxVideoOutNum )
	{
		//只有本地通道
	}
	else
	{
		if(ipcMaxVideoOutNum >= 9)
		{
			numChnn += g_p_login_succ->productInfo.netVideoInputNum;
		}
	}

	if( numChnn <= 8 )
	{
		m_selType = TIME_SEARCH_SEL_TYPE_3X3;
		//m_selChnn = 0xFF;
		m_selChnn = (0xFF & m_authCH);

		//2010-05-23 18:11:00 YSW
		//默认把能支持的最大通道数目组合设为选择状态。
		for (unsigned char chnn = 0; chnn < m_channelNum; ++chnn)
		{
			if (m_selChnn & (m_scheduleCH & ((ULONGLONG)(0x01) << chnn)))
			{
				m_scheduleList.SetMarkItem(chnn, true, true);
			}
			else if (m_scheduleCH & ((ULONGLONG)(0x01) << chnn))
			{
				m_scheduleList.SetMarkItem(chnn, false, true);
			}
			else
			{
				//应该不存在该项
			}
		}

		RefreshBtnSplit(m_selType);

		return KEY_VIRTUAL_MSG_STOP;
	}
	int maxSel = 9; // min(9, 产品回放通道最大值)
	if( maxSel > numChnn )
	{
		maxSel = numChnn;
	}
	maxSel = (m_videoOuputNum > maxSel) ? maxSel : m_videoOuputNum;
	ULONGLONG selChnn = (maxSel > 8) ? 0x1FF : 0xFF;
	if( TIME_SEARCH_SEL_TYPE_3X3 == m_selType ) selChnn = m_selChnn;

	m_selectChnnDlg.SetChnnCount( numChnn , m_authCH, maxSel, selChnn );
	GUI::CRect chnnRect = m_selectChnnDlg.GetDlgDefaultRect();
	if( DLG_OK == m_selectChnnDlg.DoModal( this, x, y, chnnRect.Width(), chnnRect.Height() ) )
	{
		m_selType = TIME_SEARCH_SEL_TYPE_3X3;
		m_selChnn = m_selectChnnDlg.GetSelChnn();

		//2010-02-08 12:20:00 YSW
		//默认把能支持的最大通道数目组合设为选择状态。
		for (unsigned char chnn = 0; chnn < m_channelNum; ++chnn)
		{
			if (m_selChnn & (m_scheduleCH & ((ULONGLONG)(0x01) << chnn)))
			{
				m_scheduleList.SetMarkItem(chnn, true, true);
			}
			else if (m_scheduleCH & ((ULONGLONG)(0x01) << chnn))
			{
				m_scheduleList.SetMarkItem(chnn, false, true);
			}
			else
			{
				//应该不存在该项
			}
		}

		RefreshBtnSplit(m_selType);
	}

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CTimeSearchDlg::OnBtnChnn4X4()
{
	//if (CHNN_SELECT_YES == m_btnChnn[4].GetCurState())
	//{
	//	return KEY_VIRTUAL_MSG_STOP;
	//}

	GUI::CRect rect = m_btnChnn[4].GetWindowRect();
	unsigned short x = rect.m_left;
	unsigned short y = rect.m_bottom;

	int numChnn = g_p_login_succ->productInfo.localVideoInputNum;
	unsigned long ipcMaxVideoOutNum = CProduct::Instance()->IPCMaxVideoOutNum();
	if( numChnn && ipcMaxVideoOutNum )
	{
		//只有本地通道
	}
	else
	{
		if(ipcMaxVideoOutNum >= 16)
		{
			numChnn += g_p_login_succ->productInfo.netVideoInputNum;
		}
	}

	if( numChnn <= 16 )
	{
		m_selType = TIME_SEARCH_SEL_TYPE_4X4;
		//m_selChnn = 0xFFFF;
		m_selChnn = (0xFFFF & m_authCH);

		//2010-02-08 12:20:00 YSW
		//默认把能支持的最大通道数目组合设为选择状态。
		for (unsigned char chnn = 0; chnn < m_channelNum; ++chnn)
		{
			if (m_selChnn & (m_scheduleCH & ((ULONGLONG)(0x01) << chnn)))
			{
				m_scheduleList.SetMarkItem(chnn, true, true);
			}
			else if (m_scheduleCH & ((ULONGLONG)(0x01) << chnn))
			{
				m_scheduleList.SetMarkItem(chnn, false, true);
			}
			else
			{
				//应该不存在该项
			}
		}

		RefreshBtnSplit(m_selType);

		return KEY_VIRTUAL_MSG_STOP;
	}

	int maxSel = 16; // min(16, 产品回放通道最大值)
	if( maxSel > numChnn )
	{
		maxSel = numChnn;
	}
	maxSel = (m_videoOuputNum > maxSel) ? maxSel : m_videoOuputNum;
	ULONGLONG selChnn = 0xFFFF;
	if( TIME_SEARCH_SEL_TYPE_4X4 == m_selType ) selChnn = m_selChnn;

	m_selectChnnDlg.SetChnnCount( numChnn , m_authCH, maxSel, selChnn );
	GUI::CRect chnnRect = m_selectChnnDlg.GetDlgDefaultRect();
	if( DLG_OK == m_selectChnnDlg.DoModal( this, x, y, chnnRect.Width(), chnnRect.Height() ) )
	{
		m_selType = TIME_SEARCH_SEL_TYPE_4X4;
		m_selChnn = m_selectChnnDlg.GetSelChnn();

		//2010-02-08 12:20:00 YSW
		//默认把能支持的最大通道数目组合设为选择状态。
		for (unsigned char chnn = 0; chnn < m_channelNum; ++chnn)
		{
			if (m_selChnn & (m_scheduleCH & ((ULONGLONG)(0x01) << chnn)))
			{
				m_scheduleList.SetMarkItem(chnn, true, true);
			}
			else if (m_scheduleCH & ((ULONGLONG)(0x01) << chnn))
			{
				m_scheduleList.SetMarkItem(chnn, false, true);
			}
			else
			{
				//应该不存在该项
			}
		}

		RefreshBtnSplit(m_selType);
	}

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CTimeSearchDlg::OnBtnPlayback()
{
	return KEY_VIRTUAL_PLAY;
}

unsigned long CTimeSearchDlg::OnClickSchedule(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	//printf("x pos=%d,y pos=%d\n",xPos,yPos);
	BYTE hour=0,min=0,sec=0;
	
	if( m_scheduleList.HitTestTime(xPos,yPos,hour,min,sec) )
	{
		m_tmStart.SetTime(hour,min,sec,true);
	}

	m_scheduleList.DrawDotFlag(xPos, yPos, true);

	return KEY_VIRTUAL_MSG_STOP;
}

unsigned long CTimeSearchDlg::GetPlaybackInfo(unsigned long &playbackStarTime, unsigned long &playbackEndTime, unsigned long &playbackPlayTime, ULONGLONG &chnnBits)
{
	tm tmpTm;
	memset(&tmpTm,0,sizeof(tm));
	m_dateStart.GetDate(tmpTm.tm_year,tmpTm.tm_mon,tmpTm.tm_mday);
	m_tmStart.GetTime(tmpTm.tm_hour,tmpTm.tm_min,tmpTm.tm_sec);
	tmpTm.tm_year-=1900;

	m_playbackStartTime = TmToDVRTime32(tmpTm);
	m_playbackEndTime   = GetEndTime32OfDate(m_playbackStartTime);
	m_playbackPlayTime   = m_playbackStartTime;

	if (m_playbackEndTime < m_playbackStartTime)
	{
		MessageBox(m_pStrTable->String(E_STR_ID_MESSAGE_47), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK | DEFAULT_ICON_WARNING);
		return KEY_VIRTUAL_MSG_STOP;
	}

	if (0 == m_selChnn)
	{
		MessageBox(m_pStrTable->String(E_STR_ID_MESSAGE_34), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK | DEFAULT_ICON_WARNING);
		return KEY_VIRTUAL_MSG_STOP;
	}

	playbackStarTime	= m_playbackStartTime;
	playbackEndTime		= m_playbackEndTime;
	playbackPlayTime	= m_playbackPlayTime;
	chnnBits			= m_selChnn;

	return KEY_VIRTUAL_MSG_CONTINUE;
}


//public
void CTimeSearchDlg::OnSearch(unsigned long startTime, unsigned long endTime, ULONGLONG chnnBits, unsigned long dataOwnerby, bool bRefresh)
{
	MESSAGE_DATA msgData;
	NET_SECTION_INFO *pSectionInfo = NULL;
	NET_DATA_SEARCH searchCond;
	bool bHaveData = false;
    unsigned long num = 0;
	unsigned long index = 0;
	char  szChannelName[32] = {0};
	tm    starTm;
	tm    endTm;

	unsigned long ulStartZeroTimeOfDate = GetZeroTime32OfDate(startTime);
	unsigned long ulEndTimeOfDate	= GetEndTime32OfDate(startTime);

	tm tmpTm = DVRTime32ToTm(ulStartZeroTimeOfDate);

	m_dateStart.SetDate(tmpTm.tm_year+1900,tmpTm.tm_mon,tmpTm.tm_mday,true);
	m_tmStart.SetTime(tmpTm.tm_hour,tmpTm.tm_min,tmpTm.tm_sec,true);
	m_scheduleList.RemoveAllItem(false);


	searchCond.searchType  = DATA_SEARCH_TYPE_TIME;
	searchCond.videoCH     = chnnBits;
	searchCond.startTime   = startTime;
	searchCond.endTime     = endTime;
	searchCond.dataOwnerby = dataOwnerby;

	memset(&msgData, 0, sizeof(MESSAGE_DATA));
	CMessageMan::Instance()->SendMsgDataToServer(0, LOCAL_DEVICE_ID, CMD_REQUEST_DATA_SEARCH, (unsigned char *)&searchCond, sizeof(NET_DATA_SEARCH), msgData);

	pSectionInfo = (NET_SECTION_INFO *)(msgData.pData + sizeof(PACKCMD) + sizeof(NET_DATA_INFO));
	num = (msgData.dataLen - sizeof(PACKCMD) - sizeof(NET_DATA_INFO))/sizeof(NET_SECTION_INFO);
	for (unsigned char chnn = 0; chnn < m_channelNum; ++chnn)
	{
		if (chnnBits & (ULONGLONG)0x01 << chnn)
		{
			sprintf(szChannelName, "%d", chnn + 1 );
			m_scheduleList.AddItem(chnn, szChannelName, false);
		}
	}

	m_scheduleCH = chnnBits;

	for (index = 0; index < num; ++index)
	{
		if (pSectionInfo[index].starTime < ulStartZeroTimeOfDate)
		{
			pSectionInfo[index].starTime = ulStartZeroTimeOfDate;
		}

		if (ulEndTimeOfDate < pSectionInfo[index].endTime)
		{
			pSectionInfo[index].endTime = ulEndTimeOfDate;
		}

		starTm = DVRTime32ToTm(pSectionInfo[index].starTime);
		endTm  = DVRTime32ToTm(pSectionInfo[index].endTime);

		m_scheduleList.AddData(pSectionInfo[index].chnnnel, starTm.tm_hour, starTm.tm_min, starTm.tm_sec, endTm.tm_hour, endTm.tm_min, endTm.tm_sec);
	}

	m_scheduleList.MoveFirst(false);

	CMessageMan::Instance()->ReleaseMsgBuff(msgData);
	if (0 == num)
	{
		MessageBox(m_pStrTable->String(E_STR_ID_MESSAGE_36), m_pStrTable->String(E_STR_ID_TITLE_WARNING), DEFAULT_BTN_OK  | DEFAULT_ICON_WARNING);
	}
	
	{
		//2010-02-08 12:20:00 YSW
		//默认把能支持的最大通道数目组合设为选择状态。
		for (unsigned char chnn = 0; chnn < m_channelNum; ++chnn)
		{
			if (m_selChnn & (m_scheduleCH & ((ULONGLONG)(0x01) << chnn)))
			{
				m_scheduleList.SetMarkItem(chnn, true, false);
			}
			else if (m_scheduleCH & ((ULONGLONG)(0x01) << chnn))
			{
				m_scheduleList.SetMarkItem(chnn, false, false);
			}
		}
	}

	if (bRefresh)
	{
		m_scheduleList.Repaint();
	}
}

//////////////////////////////////////////////////////////////////////////
void CTimeSearchDlg::RefreshBtnSplit(TIME_SEARCH_SEL_TYPE type)
{
	assert (type <= TIME_SEARCH_SEL_TYPE_4X4);

	for (unsigned char i=0; i<type; ++i)
	{
		m_btnChnn[i].SetCurState(CHNN_SELECT_NO, true);
	}

	m_btnChnn[type].SetCurState(CHNN_SELECT_YES, true);

	for (unsigned char i=type+1; i<5; ++i)
	{
		m_btnChnn[i].SetCurState(CHNN_SELECT_NO, true);
	}
}
//end

















