/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : ZXX
** Date         : 2013-11-26
** Name         : WzdNatSelectDevDlg.cpp
** Version      : 1.0
** Description  : 穿透向导设备选择页面对话框
** Modify Record:
***********************************************************************/

#include "WzdNatSelectDevDlg.h"

CWzdNatSelectDevDlg::CWzdNatSelectDevDlg()
{
	m_devSelectIndex = DEV_ANDROID;
}

CWzdNatSelectDevDlg::~CWzdNatSelectDevDlg()
{

}

void CWzdNatSelectDevDlg::SetWzdInfo(NAT_WIZARD_INFO* pWzdInfo)
{
	m_pWzdInfo = pWzdInfo;
}

void CWzdNatSelectDevDlg::ShowWzdData(bool bRefresh)
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

void CWzdNatSelectDevDlg::GetWzdData(bool bNext)
{

}
void CWzdNatSelectDevDlg::OnLeavePage()
{

}

void CWzdNatSelectDevDlg::SetTipInfo(const std::string& str)
{
	m_richTextCtrl.SetText(str, false);
}

void CWzdNatSelectDevDlg::SetDevSelectIndex(int devIndex)
{
	m_devSelectIndex = devIndex;
}

int	 CWzdNatSelectDevDlg::GetDevSelectIndex()
{
	return m_devSelectIndex;
}

void CWzdNatSelectDevDlg::OnInitial()
{
	unsigned long x = m_length[DLG_OFFSET_SUB_LEFT];
	unsigned long y = m_length[DLG_OFFSET_SUB_TOP];

	unsigned long cx = m_rect.Width() -  2 * x;
	unsigned long cy = (m_rect.Height() - 2 * y)/2+15;
	unsigned long height = m_rect.Height() - 2 * y -m_length[CHECK_HEIGHT];
	m_richTextCtrl.Create(GetRandWndID(), x, y, cx, cy, this);
	

	y = y+cy+cy/2 - 3*(m_length[RADIO_HEIGHT]+2);
	m_rdDevAndroid.Create(GetRandWndID(),x + m_length[BTN_WIDTH],y,2*m_length[BTN_WIDTH],m_length[RADIO_HEIGHT],this,0,1,&m_rdGroup);
	y += m_length[RADIO_HEIGHT]+5;
	m_rdDevApple.Create(GetRandWndID(),x + m_length[BTN_WIDTH],y,2*m_length[BTN_WIDTH],m_length[RADIO_HEIGHT],this,0,2,&m_rdGroup);
	y += m_length[RADIO_HEIGHT]+5;
	m_rdDevPc.Create(GetRandWndID(),x + m_length[BTN_WIDTH],y,2*m_length[BTN_WIDTH],m_length[RADIO_HEIGHT],this,0,3,&m_rdGroup);
	m_rdGroup.AddRadio(&m_rdDevAndroid);
	m_rdGroup.AddRadio(&m_rdDevApple);
	m_rdGroup.AddRadio(&m_rdDevPc);
	m_rdDevAndroid.SetCaption(m_pStrTable->String(E_STR_ID_US02_WZD_ANDROID),false,false);
	m_rdDevAndroid.SetData(DEV_ANDROID);
	m_rdDevApple.SetCaption(m_pStrTable->String(E_STR_ID_US02_WZD_APPLE),false,false);
	m_rdDevApple.SetData(DEV_APPLE);
	m_rdDevPc.SetCaption("Computer",false,false);
	m_rdDevPc.SetData(DEV_PC);
	switch(m_devSelectIndex)
	{
	case DEV_ANDROID:
		{
			m_rdDevAndroid.SetCheck(true,false);
		}
		break;
	case DEV_APPLE:
		{
			m_rdDevApple.SetCheck(true,false);
		}
		break;
	case DEV_PC:
		{
			m_rdDevPc.SetCheck(true,false);
		}
	default:
		{
			assert(false);
			m_rdDevAndroid.SetCheck(true,false);
		}
		break;
	}

	x = m_length[DLG_OFFSET_SUB_LEFT];
	y = m_length[DLG_OFFSET_SUB_TOP]+height;
	cx = m_rect.Width() -  2 * x;
	cy = m_length[CHECK_HEIGHT];
	m_checkCtrl.Create(GetRandWndID(),x,y,cx,cy,this,0,4);
	m_checkCtrl.SetCaption(m_pStrTable->String(E_STR_ID_US02_WZD_OPENAGAIN),false,false);
}

void  CWzdNatSelectDevDlg::OnDestroy()
{

}

void CWzdNatSelectDevDlg::InitCtrlInfo()
{

}



BEGIN_MSG_MAP(CWzdNatSelectDevDlg, CWzdNatPageDlg)
	ON_MSG(m_checkCtrl.GetID(), KEY_VIRTUAL_ENTER, OnCheck)
	ON_MSG(m_rdDevAndroid.GetID(),KEY_VIRTUAL_ENTER,OnClickRadioCtrl)
	ON_MSG(m_rdDevApple.GetID(),KEY_VIRTUAL_ENTER,OnClickRadioCtrl)
	ON_MSG(m_rdDevPc.GetID(),KEY_VIRTUAL_ENTER,OnClickRadioCtrl)
END_MSG_MAP()

unsigned long CWzdNatSelectDevDlg::OnClickRadioCtrl()
{
	GUI::CRadioCtrl* pRadioCtrl = m_rdGroup.GetCheckRadio();
	DEV_SELECT_INDEX click_dev= static_cast<DEV_SELECT_INDEX>(pRadioCtrl->GetData());

	switch(click_dev)
	{
	case DEV_ANDROID:
		{
			m_devSelectIndex = DEV_ANDROID;
			m_rdDevAndroid.SetCheck(true,false);
		}
		break;
	case DEV_APPLE:
		{
			m_devSelectIndex = DEV_APPLE;
			m_rdDevApple.SetCheck(true,false);
		}
		break;
	case DEV_PC:
		{
			m_devSelectIndex = DEV_PC;
			m_rdDevPc.SetCheck(true,false);
		}
		break;
	default:
		{
			assert(false);
			m_devSelectIndex = DEV_ANDROID;
			m_rdDevAndroid.SetCheck(true,false);
		}
		break;
	}
	return KEY_VIRTUAL_MSG_CONTINUE;
}

unsigned long  CWzdNatSelectDevDlg::OnCheck()
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

