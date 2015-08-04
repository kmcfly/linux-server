/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : ysw
** Date         : 2009-08-06
** Name         : NetworkStreamInfoDlg.cpp
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/
#include "NetworkStreamInfoDlg.h"
#include "NetProtocol.h"

extern NET_LOGIN_SUCCESS_REPLY *g_p_login_succ;
//////////////////////////////////////////////////////////////////////////
CNetworkStreamInfoDlg::CNetworkStreamInfoDlg()
{

}

CNetworkStreamInfoDlg::~CNetworkStreamInfoDlg()
{

}

//////////////////////////////////////////////////////////////////////////
void CNetworkStreamInfoDlg::OnInitial()
{
	GUI::CDialog::OnInitial();

	//////////////////////////////////////////////////////////////////////////
	unsigned short x = m_length[DLG_OFFSET_SUB_LEFT], y = m_length[DLG_OFFSET_SUB_TOP];
	unsigned short width = m_rect.Width() -2 * m_length[DLG_OFFSET_SUB_LEFT];
	unsigned short height = m_rect.Height() - y - m_length[DLG_OFFSET_SUB_BOTTOM];

	m_streamInfoList.Create(GUI::CWnd::GetRandWndID(), x, y, width, height, this, 0, 1);
	m_streamInfoList.AddColumn(m_pStrTable->String(E_STR_ID_CH), width * 4 / 32);
	m_streamInfoList.AddColumn(m_pStrTable->String(E_STR_ID_FRAME_RATE), width * 7 / 32);
	m_streamInfoList.AddColumn(m_pStrTable->String(E_STR_ID_VIDEO_QUALITY), width * 7 / 32);
	m_streamInfoList.AddColumn(m_pStrTable->String(E_STR_ID_RESOLUTION), width * 7 / 32);
	m_streamInfoList.AddColumn(m_pStrTable->String(E_STR_ID_STATUS), width * 7 / 32 );

	//////////////////////////////////////////////////////////////////////////
	char str[512] = {0};
	GUI::CWnd * pWnd = NULL;
	for (int ch = 0; ch < g_p_login_succ->productInfo.localVideoInputNum; ++ch)
	{
		sprintf(str, "%d", ch+1);
		m_streamInfoList.AddItem(ch, 0, str);

		sprintf(str, "%d", 30);
		m_streamInfoList.AddItem(ch, 1, str);

		//strcpy(str, "Recording");
		m_streamInfoList.AddItem(ch, 4, m_pStrTable->String(E_STR_ID_RECORDING));
	}

	m_streamInfoList.CreateListFocusMap();
}
