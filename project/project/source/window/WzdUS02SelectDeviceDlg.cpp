/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : ZXX
** Date         : 2012-10-17
** Name         : WzdUS02SelectDeviceDlg.cpp
** Version      : 1.0
** Description  : 向导页面对话框
** Modify Record:
***********************************************************************/

#include "WzdUS02SelectDeviceDlg.h"

CWzdUS02SelectDeviceDlg::CWzdUS02SelectDeviceDlg()
{
	m_curMobileSelect = 0;
}

CWzdUS02SelectDeviceDlg::~CWzdUS02SelectDeviceDlg()
{

}

void CWzdUS02SelectDeviceDlg::SetWzdInfo(US02_WzdInfo* pWzdInfo)
{
	m_pWzdInfo = pWzdInfo;
}

void CWzdUS02SelectDeviceDlg::GetWzdData(bool bNext)
{
	m_pWzdInfo->MobileDevice = m_curMobileSelect;
}

void CWzdUS02SelectDeviceDlg::ShowWzdData(bool bRefresh)
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

void CWzdUS02SelectDeviceDlg::OnInitial()
{
	CWzdUS02PageDlg::OnInitial();
	unsigned long x = m_length[DLG_OFFSET_SUB_LEFT];
	unsigned long y = m_length[DLG_OFFSET_SUB_TOP];

	unsigned long cx = m_rect.Width() -  2 * x;
	
	m_richTip.Create(GetRandWndID(),x,y,cx,3*m_length[STATIC_HEIGHT],this);
	string str;
	str.append(m_pStrTable->String(E_STR_ID_US02_WZD_TENTIP01));
	str.append("\\n");
	str.append(m_pStrTable->String(E_STR_ID_US02_WZD_TENTIP02));
	m_richTip.SetText(str,false);

	y += 3*m_length[STATIC_HEIGHT]+5;
	m_rdApple.Create(GetRandWndID(),x,y,m_length[BTN_WIDTH],m_length[RADIO_HEIGHT],this,0,1,&m_rdGroup);
	m_rdApple.SetData(ROW_APPLE);
	m_rdApple.SetCaption(m_pStrTable->String(E_STR_ID_US02_WZD_APPLE),false,false);
	m_rdGroup.AddRadio(&m_rdApple);

	y += m_length[RADIO_HEIGHT]+10;
	m_rdAndroid.Create(GetRandWndID(),x,y,m_length[BTN_WIDTH],m_length[RADIO_HEIGHT],this,0,2,&m_rdGroup);
	m_rdAndroid.SetData(ROW_ANDROID);
	m_rdAndroid.SetCaption(m_pStrTable->String(E_STR_ID_US02_WZD_ANDROID),false,false);
	m_rdGroup.AddRadio(&m_rdAndroid);
	m_rdApple.SetCheck(true,false);
	m_curMobileSelect = ROW_APPLE;

	y += 2*m_length[BTN_HEIGHT];
	unsigned long cy = m_rect.Height() -  2 * m_length[DLG_OFFSET_SUB_TOP] - y;
	m_richAnotherTip.Create(GetRandWndID(),x,y,cx,cy,this);
	str.clear();
	str.append(m_pStrTable->String(E_STR_ID_US02_WZD_TENTIP03));
	m_richAnotherTip.SetText(str,false);

	x = m_length[DLG_OFFSET_SUB_LEFT];
	y = m_rect.Height() - 3 * m_length[DLG_OFFSET_SUB_TOP];

	cx = m_rect.Width() -  2 * x;
	cy = m_length[CHECK_HEIGHT];

	m_checkCtrl.Create(GetRandWndID(),x,y,cx,cy,this,0,1);
	m_checkCtrl.SetCaption(m_pStrTable->String(E_STR_ID_US02_WZD_OPENAGAIN),false,false);
}

void CWzdUS02SelectDeviceDlg::OnDestroy()
{

}

BEGIN_MSG_MAP(CWzdUS02SelectDeviceDlg, CWzdUS02PageDlg)
	ON_MSG(m_rdApple.GetID(),KEY_VIRTUAL_ENTER,OnClickRadio)
	ON_MSG(m_rdAndroid.GetID(),KEY_VIRTUAL_ENTER,OnClickRadio)
	ON_MSG(m_checkCtrl.GetID(), KEY_VIRTUAL_ENTER, OnCheck)
END_MSG_MAP()

unsigned long CWzdUS02SelectDeviceDlg::OnClickRadio()
{
	GUI::CRadioCtrl* pRadio = m_rdGroup.GetCheckRadio();
	assert(pRadio);
	switch ( static_cast<RADIO_ROW>(pRadio->GetData()) )
	{
	case ROW_APPLE:
		m_curMobileSelect = MOBILE_APPLE;
		break;
	case ROW_ANDROID:
		m_curMobileSelect = MOBILE_ANDROID;
		break;
	default:
		assert(false);
	}
	return KEY_VIRTUAL_MSG_CONTINUE;
}

unsigned long  CWzdUS02SelectDeviceDlg::OnCheck()
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
