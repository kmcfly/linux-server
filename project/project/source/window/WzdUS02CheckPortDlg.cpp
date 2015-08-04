/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : ZXX
** Date         : 2012-09-19
** Name         : WzdUS02CheckUpnpDlg.cpp
** Version      : 1.0
** Description  : 向导管理对话框
** Modify Record:
***********************************************************************/
#include "WzdUS02CheckPortDlg.h"

CWzdUS02CheckPortDlg::CWzdUS02CheckPortDlg()
{

}

CWzdUS02CheckPortDlg::~CWzdUS02CheckPortDlg()
{

}

void CWzdUS02CheckPortDlg::SetWzdInfo(US02_WzdInfo* pWzdInfo)
{
	m_pWzdInfo = pWzdInfo;
}

void CWzdUS02CheckPortDlg::ShowWzdData(bool bRefresh)
{
	string str;
	str.append("Opening http port ");
	char portArr[256] = {0};
	snprintf(portArr,256,"%d",m_pWzdInfo->httpPort);
	str.append(portArr);
	str.append(" and server port ");
	memset(portArr,0,sizeof(portArr));
	snprintf(portArr,256,"%d",m_pWzdInfo->serverPort);
	str.append(portArr);
	str.append(".....success");
}
void CWzdUS02CheckPortDlg::OnInitial()
{
	CWzdUS02PageDlg::OnInitial();
	unsigned long x = m_length[DLG_OFFSET_SUB_LEFT];
	unsigned long y = m_length[DLG_OFFSET_SUB_TOP];

	unsigned long cx = m_rect.Width() -  2 * x;
	unsigned long cy = m_length[STATIC_HEIGHT];
	m_static.Create(GetRandWndID(),x,y,cx,cy,this);
}

void CWzdUS02CheckPortDlg::OnDestroy()
{

}
//end
