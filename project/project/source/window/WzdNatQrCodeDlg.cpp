/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : ZXX
** Date         : 2013-11-25
** Name         : WzdNatQrCodeDlg.cpp
** Version      : 1.0
** Description  : 穿透向导页面二维码对话框
** Modify Record:
***********************************************************************/

#include "WzdNatQrCodeDlg.h"
#include "MessageMan.h"
#include "QRCode.h"

CWzdNatQrCodeDlg::CWzdNatQrCodeDlg()
{
	m_pMaix = NULL;
	m_drawStartX = 0;
	m_drawStartY = 0;
	m_devIndex   = 0;
	m_toprow = 0;
}

CWzdNatQrCodeDlg::~CWzdNatQrCodeDlg()
{

}

void CWzdNatQrCodeDlg::SetWzdInfo(NAT_WIZARD_INFO* pWzdInfo)
{
	m_pWzdInfo = pWzdInfo;
}

void CWzdNatQrCodeDlg::ShowWzdData(bool bRefresh)
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
	switch(m_devIndex)
	{
	case DEV_ANDROID:
	case DEV_APPLE:
		{
			if(DEV_ANDROID == m_devIndex)
			{
				// strTip.append("Network Setup for Android:");
				strTip.append(m_pStrTable->String(E_STR_ID_NAT_WZD_QR_00));
				strTip.append("\\n");
				// strTip.append("Phone: Download QT View from Android Store");
				strTip.append(m_pStrTable->String(E_STR_ID_NAT_WZD_QR_01));
				strTip.append("\\n");
				// strTip.append("Tablet: Download QT View HD from Android Store");
				strTip.append(m_pStrTable->String(E_STR_ID_NAT_WZD_QR_02));
				strTip.append("\\n");
			}
			else if(DEV_APPLE == m_devIndex)
			{
				// strTip.append("Network Setup for Apple iOS:");
				strTip.append(m_pStrTable->String(E_STR_ID_NAT_WZD_QR_03));
				strTip.append("\\n");
				// strTip.append("iPhone: Download QT View from App Store");
				strTip.append(m_pStrTable->String(E_STR_ID_NAT_WZD_QR_04));	
				strTip.append("\\n");
				// strTip.append("iPad: Download QT View HD from App Store");
				strTip.append(m_pStrTable->String(E_STR_ID_NAT_WZD_QR_05));
				strTip.append("\\n");
			}
			else
			{
				assert(false);
			}
			// strTip.append(" 1. Launch QT View app and click on the Bar Code Icon at the bottom.");
			strTip.append(m_pStrTable->String(E_STR_ID_NAT_WZD_QR_06));
			strTip.append("\\n");
			// strTip.append(" 2. Scan this QR Code");
			strTip.append(m_pStrTable->String(E_STR_ID_NAT_WZD_QR_07));
			strTip.append("\\n");
			// strTip.append(" 3. Enter the Username: admin");
			strTip.append(m_pStrTable->String(E_STR_ID_NAT_WZD_QR_08));
			strTip.append("\\n");
			// strTip.append(" 4. Enter your Password ");
			strTip.append(m_pStrTable->String(E_STR_ID_NAT_WZD_QR_09));
			strTip.append("\\n");
			// strTip.append("If you did not change it at the start of this wizard, the default password is 123456 and this should be changed as soon as you finish this wizard to protect your system.");
			strTip.append(m_pStrTable->String(E_STR_ID_NAT_WZD_QR_0a));
			strTip.append("\\n");
			// strTip.append("Click Prev to setup another device. Or, click Next to continue.");
			strTip.append(m_pStrTable->String(E_STR_ID_NAT_WZD_QR_0b));
			strTip.append("\\n");

			char user[36] = "admin";
			unsigned long len  =0;
			CQRCode codeMan;
			QrCodeOutputData qrMaix;
			qrMaix.dataMatrix =NULL;
			qrMaix.width = 0;

			char str[128]={0};
			snprintf(str,128,"<sn>%02X%02X%02X%02X%02X%02X</sn>\n",networkStatusInfo.MAC[0],networkStatusInfo.MAC

				[1],networkStatusInfo.MAC[2],networkStatusInfo.MAC[3],networkStatusInfo.MAC[4],networkStatusInfo.MAC[5]);

			snprintf(str+strlen(str),128-strlen(str), "<user>%s</user>\n",user);
			snprintf(str+strlen(str),128-strlen(str), "<ip>%03d.%03d.%03d.%03d</ip>\n",(networkStatusInfo.IP&0xff), 

				(networkStatusInfo.IP>>8)&0xff, (networkStatusInfo.IP>>16)&0xff, (networkStatusInfo.IP>>24)&0xff);
			snprintf(str+strlen(str),128-strlen(str), "<port>%d</port>\n",networkStatusInfo.serverPort);
			codeMan.GetQRCodeMatrixData((unsigned char *)str,strlen(str),&qrMaix);
			SetMaix((unsigned char *)qrMaix.dataMatrix,qrMaix.width,qrMaix.width);
			codeMan.FreeQrCodeMatrix(&qrMaix);
			
			m_phoneRichTextCtrl.SetText(strTip, false);
			unsigned short maxTextRow = m_phoneRichTextCtrl.GetTextRow()-1;//文本显示需要多少行
			if (800 >= m_width)//800x600以上分辨率不需要滚动条
			{
				m_scroll_vPhone.SetRange(0,maxTextRow);
				m_scroll_vPhone.SetCurPos(0,false);
			}
		}
		break;
	default:
		{
			break;
		}
	}
}

void CWzdNatQrCodeDlg::GetWzdData(bool bNext)
{

}

void CWzdNatQrCodeDlg::OnLeavePage()
{

}


void CWzdNatQrCodeDlg::SetMaix(unsigned char * pMaix,unsigned long w,unsigned long h)
{
	assert(w > 0);
	assert(h > 0);
	assert(pMaix != NULL);
	m_maxW = w;
	m_maxH = h;
	m_pMaix = new unsigned char[w*h];
	memcpy(m_pMaix,pMaix,w*h);

}

void CWzdNatQrCodeDlg::OnInitial()
{
	unsigned long x = m_length[DLG_OFFSET_SUB_LEFT];
	unsigned long y = m_length[DLG_OFFSET_SUB_TOP];

	unsigned long cx = (m_rect.Width() -  2 * x)/3*2 - CScrollCtrl::m_length[SCROLL_WIDTH] -2;
	unsigned long cy = m_rect.Height() - 2 * y - m_length[CHECK_HEIGHT]-2;
	m_drawStartX = x+cx+ CScrollCtrl::m_length[SCROLL_WIDTH] +2;
	m_drawStartY = y+cy/4;
	m_phoneRichTextCtrl.Create(GetRandWndID(), x, y, cx, cy, this);
	if (800 >= m_width)//800x600以上分辨率不需要滚动条
	{
		m_phoneRichTextCtrl.SetScrollShow(true);
		m_scroll_vPhone.Create(GetRandWndID(), x+cx + 2,y,cy,this,0,1,false);
		
	}
	
	m_checkCtrl.Create(GetRandWndID(),x,y+cy,cx,m_length[CHECK_HEIGHT],this,0,2);
	m_checkCtrl.SetCaption(m_pStrTable->String(E_STR_ID_US02_WZD_OPENAGAIN),false,false);
}

void CWzdNatQrCodeDlg::SetDevIndex(int index)
{
	m_devIndex = index;
	m_toprow = 0;
}

void CWzdNatQrCodeDlg::InitCtrlInfo()
{

}

void CWzdNatQrCodeDlg::OnPaint(PAINT_STATUS status)
{
	GUI::CDialog::OnPaint(status);
	switch(m_devIndex)
	{
	case DEV_ANDROID:
	case DEV_APPLE:
		{
			unsigned long x = m_rect.m_left+m_length[DLG_OFFSET_LEFT]+m_drawStartX;
			unsigned long y = m_rect.m_top+m_length[DLG_OFFSET_TOP]+m_drawStartY;

			if(m_pMaix != NULL)
			{
				GUI::CWnd::DrawRect(x,y,4*(m_maxH+1),4*(m_maxW+1),COLOR_LINE_WHITE);
				for(unsigned long w =0;w<m_maxW;++w)
				{
					for(unsigned long h =0;h<m_maxH;++h)
					{
						if(0x01 & (*(m_pMaix+w*m_maxW+h)))
						{
							//黑块
							GUI::CWnd::DrawRect(x+4*(h%m_maxH)+2, y+4*(w%m_maxW)+2, 4, 4, COLOR_LINE_BLACK);
						}
						else
						{	//白块
							GUI::CWnd::DrawRect(x+4*(h%m_maxH)+2, y+4*(w%m_maxW)+2, 4, 4, COLOR_LINE_WHITE);
						}
					}
				}
			}
		}
		break;
	default:
		break;
	}
	

}

void CWzdNatQrCodeDlg::OnDestroy()
{
	if(NULL != m_pMaix)
	{
		delete []m_pMaix;
		m_pMaix = NULL;
	}

}

BEGIN_MSG_MAP(CWzdNatQrCodeDlg, CWzdNatPageDlg)
	ON_MSG(m_checkCtrl.GetID(), KEY_VIRTUAL_ENTER, OnCheck)
	ON_MSG_EX(m_scroll_vPhone.GetID(), KEY_VIRTUAL_MSG_DEFAULT_TAG, OnScrollchange)
	ON_MSG_EX(m_scroll_vPhone.GetID(),KEY_VIRTUAL_ENTER, OnScrollchange)
	ON_MSG_EX(m_scroll_vPhone.GetID(),KEY_VIRTUAL_DRAG, OnScrollchange)
	ON_MSG_EX(m_scroll_vPhone.GetID(),KEY_VIRTUAL_BLUP, OnScrollchange)
END_MSG_MAP()

unsigned long  CWzdNatQrCodeDlg::OnCheck()
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

unsigned long CWzdNatQrCodeDlg::OnScrollchange(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	switch(m_devIndex)
	{
	case DEV_ANDROID:
	case DEV_APPLE:
		{
			int pos = m_scroll_vPhone.GetPos();
			if (pos != m_toprow)
			{
				m_toprow = pos;
				m_phoneRichTextCtrl.SetCurRow(m_toprow);
			}
		}
		break;
	}
	Repaint();

	return KEY_VIRTUAL_MSG_STOP; 
}

//end
