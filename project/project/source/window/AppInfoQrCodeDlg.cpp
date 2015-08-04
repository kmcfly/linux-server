#include "AppInfoQrCodeDlg.h"
#include "CtrlID.h"
#include <fcntl.h>
#include <stdio.h>

#include "PUB_common.h"

#ifdef __ENVIRONMENT_LINUX__
#include <unistd.h>
#include <stdlib.h>
#include <netdb.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "NetInterface.h"

#endif

const char *g_pNoneAppInfo		= "None app info download link";
const char *g_pErrorAppInfo		= "Error app info";
const char *g_pConnectWaitStr	= "Connecting,please waiting......";


CAppInfoQrCodeDlg *CAppInfoQrCodeDlg::Instance()
{
	static CAppInfoQrCodeDlg s_AppInfoInstance;
	return &s_AppInfoInstance;
}

CAppInfoQrCodeDlg::CAppInfoQrCodeDlg()
{
	m_last_scroll_pos = 0xff;
	m_bPanitFlag =false;
	m_RequestAppinfoRet = false;

	m_hConnectThread = PUB_THREAD_ID_NOINIT;
	m_bConnecting = false;
	m_qrMaix.dataMatrix = NULL;
}

CAppInfoQrCodeDlg::~CAppInfoQrCodeDlg()
{

}

void CAppInfoQrCodeDlg::OnDestroy()
{
	GUI::CDialog::OnDestroy();
	//if (PUB_THREAD_ID_NOINIT != m_hConnectThread)
	//{
	//	PUB_ExitThread(&m_hConnectThread, &m_bConnecting);
	//}
	m_xVectorAppInfo.clear();
	m_qrCode.FreeQrCodeMatrix(&m_qrMaix);
}


void CAppInfoQrCodeDlg::OnInitial()
{		
	string getStr;
	for(unsigned int i=0;i<17;i++)
	{
		if(GetCustomEncrypt(i,getStr)==false)
			printf("########error: get out of the str.....__%s__%d\n",__FILE__,__LINE__);
//		printf("get str = %s\n",getStr.c_str());
	}
	GUI::CDialog::OnInitial();
	SetImage(BMP_MEM_DLG_LARGE, false);
	SetCaption("App DownLoad QRCODE", false);

	//////////////////////////////////////////////////////////////////////////
	m_stcConnectTips.Create( GUI::CWnd::GetRandWndID(),20,m_rect.Height()/10,240, m_length[STATIC_HEIGHT], this );
	m_stcConnectTips.SetAlignText(GUI::ALIGN_TEXT_CENTER);

	DispalyErrorDetails(g_pConnectWaitStr);
	m_bPanitFlag = false;

//	m_xVectorAppInfo.clear();
	m_qrMaix.dataMatrix = NULL;
	m_last_scroll_pos = 0xff;
	DisplayAppinfoQrcode();

}
BEGIN_MSG_MAP(CAppInfoQrCodeDlg, GUI::CDialog)
	ON_MSG_EX(m_scroll_v.GetID(), KEY_VIRTUAL_DEC,	OnScrollchange)
	ON_MSG_EX(m_scroll_v.GetID(), KEY_VIRTUAL_ADD,	OnScrollchange)
	ON_MSG_EX(m_scroll_v.GetID(), KEY_VIRTUAL_MSG_DEFAULT_TAG,	OnScrollchange)
	ON_MSG_EX(m_scroll_v.GetID(), KEY_VIRTUAL_DRAG,	OnScrollchange)
	ON_MSG_EX(m_scroll_v.GetID(), KEY_VIRTUAL_BLUP,	OnScrollchange)
END_MSG_MAP()

unsigned long CAppInfoQrCodeDlg::OnScrollchange(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)
{
	m_scroll_cur_pos = m_scroll_v.GetPos();
	printf("m_scroll_cur_pos = %d \n",m_scroll_cur_pos);
	if(m_last_scroll_pos != m_scroll_cur_pos)
	{
		m_last_scroll_pos = m_scroll_cur_pos;
		char *pTestStr = NULL;
		m_qrCode.FreeQrCodeMatrix(&m_qrMaix);
		if(GetQRCodeMatrixData((unsigned char *)(unsigned char *)m_xVectorAppInfo.at(m_scroll_cur_pos).QrDownUrl.c_str(),strlen(m_xVectorAppInfo.at(m_scroll_cur_pos).QrDownUrl.c_str()),&m_qrMaix) ==false)
			return KEY_VIRTUAL_MSG_STOP;
			string str= m_xVectorAppInfo.at(m_scroll_cur_pos).osPlateform;
		str += "   ";
		str += m_xVectorAppInfo.at(m_scroll_cur_pos).appPlateForm;
		m_stcOSPlateform.SetCaption(str.c_str(),false);
		
		char appNameBuff[512]={0};
		const char *pStr = m_xVectorAppInfo.at(m_scroll_cur_pos).AppName.c_str();
		ChangeAppName(pStr,appNameBuff);
			
		m_stcAppName.SetCaption(appNameBuff,false);
		Repaint();
	}
	return KEY_VIRTUAL_MSG_STOP; 
}

bool  CAppInfoQrCodeDlg::GetQRCodeMatrixData(unsigned char *str,int strSize,QrCodeOutputData *outParam)
{
	return m_qrCode.GetQRCodeMatrixData((unsigned char *)str,strlen((char *)str),outParam);
}
void  CAppInfoQrCodeDlg::DrawQrCodeRect(unsigned char *pMixData,unsigned long QrWidth,unsigned long QrHieght)
{
	
	unsigned short width = m_rect.Width() - 2 * m_length[DLG_OFFSET_LEFT];
	unsigned short height = m_rect.Height() - m_length[DLG_OFFSET_TOP] - m_length[DEFAULT_BTN_BOTTOM_YPOS] - m_length[DLG_OFFSET_BOTTOM];
	unsigned long x = m_rect.m_left+m_length[DLG_OFFSET_LEFT] + width/2-4*(QrWidth+1);
	unsigned long y = m_rect.m_top+m_length[DLG_OFFSET_TOP] + height/2-4*(QrWidth+1);
	
	if(pMixData != NULL)
	{
		GUI::CWnd::DrawRect(x,y,8*(QrHieght+1),8*(QrHieght+1),COLOR_LINE_WHITE);
		for(unsigned long w =0;w<QrWidth;++w)
		{
			for(unsigned long h =0;h<QrHieght;++h)
			{
				if(0x01 & (*(pMixData+w*QrWidth+h)))
				{
					//ºÚ¿é
					GUI::CWnd::DrawRect(x+8*(h%QrHieght)+4, y+8*(w%QrWidth)+4, 8, 8, COLOR_LINE_BLACK);
				}
				else
				{	//°×¿é
					GUI::CWnd::DrawRect(x+8*(h%QrHieght)+4, y+8*(w%QrWidth)+4, 8, 8, COLOR_LINE_WHITE);
				}
			}
		}
	}
}
void CAppInfoQrCodeDlg::OnPaint(PAINT_STATUS status)
{
	GUI::CDialog::OnPaint(status);	
	if(m_bPanitFlag)
	{
		DrawQrCodeRect(m_qrMaix.dataMatrix,m_qrMaix.width,m_qrMaix.width);
		
	}
}

void 	CAppInfoQrCodeDlg::DispalyErrorDetails(const char *pDetails)
{
	m_stcConnectTips.SetCaption(pDetails,false);
}
int	CAppInfoQrCodeDlg::ChangeStr(const char pSrcStr)
{
	int result=0;

	if ((pSrcStr>='0') && (pSrcStr<='9'))   
		result=pSrcStr-48;//×Ö·û0µÄASCIIÖµÎª48
	else if ((pSrcStr>='a') && (pSrcStr<='f'))   
		result=pSrcStr-'a'+10;   
	else if ((pSrcStr>='A') && (pSrcStr<='F'))   
		result=pSrcStr-'A'+10;   
	else   
		result=0; 

	return result;
}
bool CAppInfoQrCodeDlg::ChangeAppName(const char *pSrcStr,char *pDestStr)
{
	unsigned int j=0;
	if(strlen(pSrcStr) >510)
		return false;
	for(unsigned int i=0;i<strlen(pSrcStr);i+=2)
		{
			char Value = ChangeStr(pSrcStr[i])*16 + ChangeStr(pSrcStr[i+1]);
			pDestStr[j++] = Value;
		}

	return true;
}
void CAppInfoQrCodeDlg::DisplayAppinfoQrcode()
{
	string strAppinfo;
	if(m_xVectorAppInfo.size() == 0)
	{
		DispalyErrorDetails(g_pNoneAppInfo);
		return ;
	}
	if(GetQRCodeMatrixData((unsigned char *)m_xVectorAppInfo.at(0).QrDownUrl.c_str(),strlen(m_xVectorAppInfo.at(0).QrDownUrl.c_str()),&m_qrMaix) ==false)
		return ;
	m_bPanitFlag = true;
	
	unsigned long x = m_rect.Width()/2;
	unsigned long y = m_rect.Height()/2;

	m_stcAppName.Create( GUI::CWnd::GetRandWndID(),x+m_qrMaix.width*16/3,y, 8*(m_qrMaix.width+1), m_length[STATIC_HEIGHT], this );
	m_stcAppName.SetAlignText(GUI::ALIGN_TEXT_LEFT);
	m_stcOSPlateform.Create( GUI::CWnd::GetRandWndID(),x+m_qrMaix.width*16/3,y+m_length[STATIC_HEIGHT], 8*(m_qrMaix.width+1), m_length[STATIC_HEIGHT], this );
	m_stcOSPlateform.SetAlignText(GUI::ALIGN_TEXT_LEFT);

	string str= m_xVectorAppInfo.at(0).osPlateform;
	str += "   ";
	str += m_xVectorAppInfo.at(0).appPlateForm;
	m_stcOSPlateform.SetCaption(str.c_str(),false);

	char appNameBuff[512]={0};
	const char *pStr = m_xVectorAppInfo.at(0).AppName.c_str();
	if(ChangeAppName(pStr,appNameBuff)==false)
	{
		DispalyErrorDetails(g_pErrorAppInfo);
		return ;
	}
	DispalyErrorDetails("");
	m_stcAppName.SetCaption(appNameBuff,false);
			
	///////////////////////////////////////////////////////////
	m_scroll_v.Create(CTRLID_LIST_CTRL_SCROLL_V, m_rect.Width() - CScrollCtrl::m_length[SCROLL_WIDTH] - 4, m_length[TITLE_HEIGHT], m_rect.Height() - m_length[TITLE_HEIGHT]-8, this, 10, 35, false);
	m_scroll_v.SetRange(0,m_xVectorAppInfo.size()-1);
	m_scroll_v.SetCurPos(0,true);
	//Repaint();
}


