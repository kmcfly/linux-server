#include "WzdNewNatPCTipDlg.h"
#include "MessageMan.h"
CWzdNewNatPCTipDlg::CWzdNewNatPCTipDlg()
{

}

CWzdNewNatPCTipDlg::~CWzdNewNatPCTipDlg()
{

}

void CWzdNewNatPCTipDlg::SetWzdInfo(NAT_WIZARD_INFO* pWzdInfo)
{
	m_pWzdInfo = pWzdInfo;
}

void CWzdNewNatPCTipDlg::ShowWzdData(bool bRefresh)
{
	if(1 == m_pWzdInfo->ShowWzd)
	{
		m_checkCtrl.SetCheck(true,false);
	}
	else
	{
		m_checkCtrl.SetCheck(false,false);
	}

	//读取系统信息并组装字符串
	NETWORK_STATUS_INFO networkStatusInfo;
	memset( &networkStatusInfo, 0, sizeof(NETWORK_STATUS_INFO));
#ifdef __ENVIRONMENT_LINUX__
	MESSAGE_DATA msgData;
	CMessageMan::Instance()->SendMsgDataToServer(LOCAL_CLIENT_ID, LOCAL_DEVICE_ID, CMD_REQUEST_NETWORK_INFO, NULL, 0, msgData);
	networkStatusInfo = *reinterpret_cast<NETWORK_STATUS_INFO *>(msgData.pData+sizeof(PACKCMD));
	CMessageMan::Instance()->ReleaseMsgBuff(msgData);
#endif
	string strTip;
	strTip.clear();
	// strTip.append("Network Setup for Mac or PC");
	strTip.append(m_pStrTable->String(E_STR_ID_US02_NEW_WZD_PC_TIP_00));
	strTip.append("\\n");
	// strTip.append("To view your camera remotely from your computer:");
	strTip.append(m_pStrTable->String(E_STR_ID_US02_NEW_WZD_PC_TIP_01));
	strTip.append("\\n");
	// strTip.append("1. Go to QTView.com using Internet Explorer (PC) or Safari (Mac).");
	strTip.append(m_pStrTable->String(E_STR_ID_US02_NEW_WZD_PC_TIP_02));
	strTip.append("\\n");

	//读取mac地址作为序列号
	// strTip.append("2. When you reach the login screen enter this Serial Number:");
	strTip.append(m_pStrTable->String(E_STR_ID_US02_NEW_WZD_PC_TIP_03));
	char serialbuf[128]={0};
	snprintf(serialbuf,sizeof(serialbuf),"%02X%02X%02X%02X%02X%02X",networkStatusInfo.MAC[0],networkStatusInfo.MAC[1],networkStatusInfo.MAC[2],networkStatusInfo.MAC[3],networkStatusInfo.MAC[4],networkStatusInfo.MAC[5]);
	strTip.append(serialbuf);
	strTip.append("\\n");
	// strTip.append("3. Enter the Username: admin");
	strTip.append(m_pStrTable->String(E_STR_ID_US02_NEW_WZD_PC_TIP_04));
	strTip.append("\\n");
	// strTip.append("4. Enter your Password ");
	strTip.append(m_pStrTable->String(E_STR_ID_US02_NEW_WZD_PC_TIP_05));
	strTip.append("\\n");
	// strTip.append("If you did not change it at the start of this wizard, the default password is 123456 and this should be changed as soon as you finish this wizard to protect your system.");
	strTip.append(m_pStrTable->String(E_STR_ID_US02_NEW_WZD_PC_TIP_06));
	strTip.append("\\n");
	// strTip.append("Click Prew to setup another device. Or, click Next to continue.");
	strTip.append(m_pStrTable->String(E_STR_ID_US02_NEW_WZD_PC_TIP_07));
	
	m_richTextCtrl.SetText(strTip, false);
	
	unsigned short maxTextRow = m_richTextCtrl.GetTextRow()-1;//文本显示需要多少行
	if (800 >= m_width)//800x600以上分辨率不需要滚动条
	{
		m_scroll_v.SetRange(0,maxTextRow);
		m_scroll_v.SetCurPos(0,false);
	}
}

void CWzdNewNatPCTipDlg::GetWzdData(bool bNext)
{

}

void CWzdNewNatPCTipDlg::OnLeavePage()
{

}

void CWzdNewNatPCTipDlg::OnInitial()
{
	m_bSelect = false;

	unsigned long x = m_length[DLG_OFFSET_SUB_LEFT];
	unsigned long y = m_length[DLG_OFFSET_SUB_TOP];

	unsigned long cx = m_rect.Width() -  2 * x - CScrollCtrl::m_length[SCROLL_WIDTH] -2;
	unsigned long cy = m_rect.Height() - 2 * y - m_length[CHECK_HEIGHT]-2;
	
	m_richTextCtrl.Create(GetRandWndID(), x, y, cx, cy, this);
	if (800 >= m_width)//800x600以上分辨率不需要滚动条
	{
		m_richTextCtrl.SetScrollShow(true);
		m_scroll_v.Create(GetRandWndID(), x+cx + 2,y,cy,this,0,1,false);

	}

	cx = m_rect.Width() -  2 * x;
	cy = (m_rect.Height() - 2 * y)/2+50;
	unsigned long height = m_rect.Height() - 2 * y -m_length[CHECK_HEIGHT];
	m_richTextCtrl.Create(GetRandWndID(), x, y, cx, cy, this);
	

	/*y = y+cy+cy/2 - 3*(m_length[RADIO_HEIGHT]+2);*/
	y = m_length[DLG_OFFSET_SUB_TOP] + ((m_rect.Height() - m_length[DLG_OFFSET_SUB_TOP])/2 + 15)/2*3 - 3*(m_length[RADIO_HEIGHT] + 2); ;
	m_rdDevMobile.Create(GetRandWndID(),x + m_length[BTN_WIDTH],y,3*m_length[BTN_WIDTH],m_length[RADIO_HEIGHT],this,0,1);
	

	m_rdDevMobile.SetCaption(m_pStrTable->String(E_STR_ID_US02_NEW_WZD_SELECT_DEV_00) /*"Mobile Setup"*/,false,false);
	m_rdDevMobile.SetData(NEXT_MOBILE);

	

	x = m_length[DLG_OFFSET_SUB_LEFT];
	y = m_length[DLG_OFFSET_SUB_TOP];

	cx = m_rect.Width() -  2 * x - CScrollCtrl::m_length[SCROLL_WIDTH] -2;
	cy = m_rect.Height() - 2 * y - m_length[CHECK_HEIGHT]-2;

	m_checkCtrl.Create(GetRandWndID(),x,y+cy,cx,m_length[CHECK_HEIGHT],this,0,2);
	m_checkCtrl.SetCaption(m_pStrTable->String(E_STR_ID_US02_WZD_OPENAGAIN),false,false);

}

void CWzdNewNatPCTipDlg::InitCtrlInfo()
{

}


void CWzdNewNatPCTipDlg::OnDestroy()
{

}

BEGIN_MSG_MAP(CWzdNewNatPCTipDlg, CWzdNatPageDlg)
	ON_MSG(m_checkCtrl.GetID(), KEY_VIRTUAL_ENTER, OnCheck)
	ON_MSG_EX(m_scroll_v.GetID(), KEY_VIRTUAL_MSG_DEFAULT_TAG, OnScrollchange)
	ON_MSG_EX(m_scroll_v.GetID(),KEY_VIRTUAL_ENTER, OnScrollchange)
	ON_MSG_EX(m_scroll_v.GetID(),KEY_VIRTUAL_DRAG, OnScrollchange)
	ON_MSG_EX(m_scroll_v.GetID(),KEY_VIRTUAL_BLUP, OnScrollchange)

	ON_MSG(m_rdDevMobile.GetID(),KEY_VIRTUAL_BLUP,OnClickRadioCtrl)
END_MSG_MAP()

unsigned long  CWzdNewNatPCTipDlg::OnCheck()
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

unsigned long CWzdNewNatPCTipDlg::OnScrollchange(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{	
	int pos = m_scroll_v.GetPos();
	if (pos != m_toprow)
	{
		m_toprow = pos;
		m_richTextCtrl.SetCurRow(m_toprow);
	}
	Repaint();

	return KEY_VIRTUAL_MSG_STOP; 
}



unsigned long CWzdNewNatPCTipDlg::OnClickRadioCtrl()
{


			if(m_rdDevMobile.IsCheck())
			{
 				m_bSelect = true;
			}
			
			else 
			{

				m_bSelect = false;
			}

	return KEY_VIRTUAL_MSG_CONTINUE;
}

//end