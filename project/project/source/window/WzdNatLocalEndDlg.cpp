#include "WzdNatLocalEndDlg.h"

#define IPADDRESS_FIELD(ip, field) static_cast<BYTE>(((ip>>((field)*8))<<24)>>24)

CWzdNatLocalEndDlg::CWzdNatLocalEndDlg()
{

}

CWzdNatLocalEndDlg::~CWzdNatLocalEndDlg()
{

}

void CWzdNatLocalEndDlg::SetWzdInfo(NAT_WIZARD_INFO* pWzdInfo)
{
	m_pWzdInfo = pWzdInfo;
}

void CWzdNatLocalEndDlg::ShowWzdData(bool bRefresh)
{
	if(1 == m_pWzdInfo->ShowWzd)
	{
		m_checkCtrl.SetCheck(true,false);
	}
	else
	{
		m_checkCtrl.SetCheck(false,false);
	}
	SetTipInfo();
}

void CWzdNatLocalEndDlg::GetWzdData(bool bNext)
{

}
void CWzdNatLocalEndDlg::OnInitial()
{
	CWzdNatPageDlg::OnInitial();
	unsigned long x = m_length[DLG_OFFSET_SUB_LEFT];
	unsigned long y = m_length[DLG_OFFSET_SUB_TOP];
	unsigned long height = m_rect.Height() - 2 * y - m_length[CHECK_HEIGHT];
	unsigned long cx = m_rect.Width() -  2 * x -2;
	unsigned long cy = m_rect.Height() - 2 * y - m_length[CHECK_HEIGHT]-2-m_length[CHECK_HEIGHT];

	m_richTextCtrl.Create(GetRandWndID(), x, y, cx, cy, this);
	

	x = m_length[DLG_OFFSET_SUB_LEFT];
	y += cy;

	x = m_length[DLG_OFFSET_SUB_LEFT];
	y = m_length[DLG_OFFSET_SUB_TOP]+height;
	cx = m_rect.Width() -  2 * x;
	cy = m_length[CHECK_HEIGHT];
	m_checkCtrl.Create(GetRandWndID(),x,y,cx,cy,this,0,0);
	m_checkCtrl.SetCaption(m_pStrTable->String(E_STR_ID_US02_WZD_OPENAGAIN),false,false);
}

void CWzdNatLocalEndDlg::OnDestroy()
{

}

void CWzdNatLocalEndDlg::SetTipInfo()
{
	std::string strTip;
	char ipArr[32]={0};
	GetStaticIp(m_pWzdInfo->Addr_Info.staticIP,ipArr);
	// strTip.append("The local IP address for this DVR is http://");
	strTip.append(m_pStrTable->String(E_STR_ID_US02_NEW_WZD_LOCAL_END_00));
	strTip.append("//");
	strTip.append(ipArr);
	strTip.append(":");
	char port[5]={0};
	snprintf(port,sizeof(port),"%d",m_pWzdInfo->httpPort);
	strTip.append(port);
	strTip.append("\\n");
	// strTip.append("if you will ONLY be connecting to your DVR through your local network, and not over the internet, ");
	// strTip.append("use the local IP address above to connect to your DVR on a computer connected to the same ");
	// strTip.append("network by entering it into the address bar in an Explorer (PC) or Safari (Mac) browser window. ");
	// strTip.append("You may also enter it into the QT View app on a mobile device using your WiFi network. ");
	// strTip.append("This address will not work when you are away from the building where your DVR is located. ");
	strTip.append(m_pStrTable->String(E_STR_ID_US02_NEW_WZD_LOCAL_END_01));
	strTip.append(m_pStrTable->String(E_STR_ID_US02_NEW_WZD_LOCAL_END_02));
	strTip.append(m_pStrTable->String(E_STR_ID_US02_NEW_WZD_LOCAL_END_03));
	strTip.append("\\n");
	// strTip.append("Clicking Exit will end this wizard. ");
	strTip.append(m_pStrTable->String(E_STR_ID_US02_NEW_WZD_LOCAL_END_04));
	m_richTextCtrl.SetText(strTip, false);
}

void CWzdNatLocalEndDlg::GetStaticIp(unsigned long ip,char *pIP)
{
	char str[32]= {0};

	unsigned char ch;
	for(int i = 0; i < 4; i++)
	{
		ch = IPADDRESS_FIELD(ip, i);
		if(3 != i)
		{
			snprintf(str+strlen(str), 32, "%d.", ch);
		}
		else
		{
			snprintf(str+strlen(str), 32, "%d", ch);
		}
	}
	strcpy(pIP,str);
}

BEGIN_MSG_MAP(CWzdNatLocalEndDlg, CWzdNatPageDlg)
	ON_MSG(m_checkCtrl.GetID(), KEY_VIRTUAL_ENTER, OnCheck)
END_MSG_MAP()
unsigned long CWzdNatLocalEndDlg::OnCheck()
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