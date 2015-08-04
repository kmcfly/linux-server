/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : ZXX
** Date         : 2012-09-24
** Name         : WzdUS02GetPortDlg.cpp
** Version      : 1.0
** Description  : 向导管理对话框
** Modify Record:
***********************************************************************/
#include "WzdUS02GetPortDlg.h"

CWzdUS02GetPortDlg ::CWzdUS02GetPortDlg()
{

}

CWzdUS02GetPortDlg ::~CWzdUS02GetPortDlg()
{

}

void CWzdUS02GetPortDlg::ShowWzdData(bool bRefresh)
{
	if(1 == m_pWzdInfo->ShowWzd)
	{
		m_checkCtrl.SetCheck(true,false);
	}
	else
	{
		m_checkCtrl.SetCheck(false,false);
	}
}

void CWzdUS02GetPortDlg::SetWzdInfo(US02_WzdInfo* pWzdInfo)
{
	m_pWzdInfo = pWzdInfo;
}

void CWzdUS02GetPortDlg ::InitCtrlInfo()
{
	string str;
	str.append(m_pStrTable->String(E_STR_ID_US02_WZD_EIGHTTIP01));
	char portArr[256]={0};
	snprintf(portArr,256," %d",m_pWzdInfo->httpPort);
	str.append(portArr);
	str.append(m_pStrTable->String(E_STR_ID_US02_WZD_EIGHTTIP02));
	memset(portArr,0,sizeof(portArr));
	snprintf(portArr,256," %d",m_pWzdInfo->serverPort);
	str.append(portArr);
	str.append(m_pStrTable->String(E_STR_ID_US02_WZD_EIGHTTIP03));
	m_showStatic.SetCaption(str,false);

	unsigned short x = m_length[DLG_OFFSET_SUB_LEFT];
	unsigned short y = m_rect.Height() - 3 * m_length[DLG_OFFSET_SUB_TOP];

	unsigned short cx = m_rect.Width() -  2 * x;
	unsigned short cy = m_length[CHECK_HEIGHT];

	m_checkCtrl.Create(GetRandWndID(),x,y,cx,cy,this,0,1);
	m_checkCtrl.SetCaption(m_pStrTable->String(E_STR_ID_US02_WZD_OPENAGAIN),false,false);
}

void CWzdUS02GetPortDlg ::OnInitial()
{
	CWzdUS02PageDlg::OnInitial();
	unsigned long x = m_length[DLG_OFFSET_SUB_LEFT];
	unsigned long y = m_length[DLG_OFFSET_SUB_TOP];

	unsigned long cx = m_rect.Width() -  2 * x;
	unsigned long cy = m_length[STATIC_HEIGHT];
	m_showStatic.Create(GetRandWndID(),x,y,cx,cy,this);

	x = m_length[DLG_OFFSET_SUB_LEFT];
	y = m_rect.Height() - 3 * m_length[DLG_OFFSET_SUB_TOP];

	cx = m_rect.Width() -  2 * x;
	cy = m_length[CHECK_HEIGHT];

	m_checkCtrl.Create(GetRandWndID(),x,y,cx,cy,this,0,1);
	m_checkCtrl.SetCaption(m_pStrTable->String(E_STR_ID_US02_WZD_OPENAGAIN),false,false);
}

void CWzdUS02GetPortDlg ::OnDestroy()
{

}

BEGIN_MSG_MAP(CWzdUS02GetPortDlg, CWzdUS02PageDlg)
	ON_MSG(m_checkCtrl.GetID(), KEY_VIRTUAL_ENTER, OnCheck)
END_MSG_MAP()

unsigned long  CWzdUS02GetPortDlg::OnCheck()
{
	if(m_checkCtrl.IsCheck())
	{
		m_pWzdInfo->ShowWzd = 1;
	}
	else
	{
		m_pWzdInfo->ShowWzd = 0;
	}
	return KEY_VIRTUAL_STOP;
}

//end
