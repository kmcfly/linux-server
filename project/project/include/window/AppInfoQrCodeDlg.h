#ifndef _APP_INFO_QRCODE_DLG_H
#define _APP_INFO_QRCODE_DLG_H

#include "Wnd.h"
#include "Dialog.h"
#include "ListCtrl.h"
#include "ScrollCtrl.h"
#include "QRCode.h"
#include "LocalDevice.h"

#include "AppInfoServManage.h"
using namespace GUI;


class CAppInfoQrCodeDlg:public GUI::CDialog
{
	private:
		CAppInfoQrCodeDlg();
		~CAppInfoQrCodeDlg();
	public:
		static CAppInfoQrCodeDlg *Instance();

		bool SetAppInfoQrCodeStr(VECTOR_APP_INFO vectAppinfo){m_xVectorAppInfo = vectAppinfo; return true;};
	protected:
		DECLARE_MSG_MAP()
		virtual void OnInitial();
		virtual void OnDestroy();
		void OnPaint(PAINT_STATUS status);
		unsigned long OnScrollchange(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);		
	private:
		bool  GetQRCodeMatrixData(unsigned char *str,int strSize,QrCodeOutputData *outParam);
		void  DrawQrCodeRect(unsigned char *pMixData,unsigned long QrWidth,unsigned long QrHieght);
		
		void 	DispalyErrorDetails(const char *pDetails);
		int		ChangeStr(const char pSrcStr);
		bool	ChangeAppName(const char *pSrcStr,char *pDestStr);

		void DisplayAppinfoQrcode();
	private:
		GUI::CListCtrl		m_AppInfoList;
		GUI::CScrollCtrl 	m_scroll_v;

		int					m_scroll_cur_pos;
		int 				m_last_scroll_pos;
		CQRCode 			m_qrCode;
		QrCodeOutputData	m_qrMaix;
		bool 				m_bPanitFlag;

		GUI::CStatic		m_stcConnectTips;
		GUI::CStatic		m_stcOSPlateform;
		GUI::CStatic		m_stcAppPlateform;
		GUI::CStatic		m_stcAppName;
		GUI::CStatic		m_stcQrDownUrl;
		string 				m_stcOSStr;

		VECTOR_APP_INFO		m_xVectorAppInfo;
		bool 				m_RequestAppinfoRet;

		bool 				m_bConnecting;
		PUB_thread_t 		m_hConnectThread;

		TOKEN_SERVER_INFO 	m_tokenServerInfo;

		string 				m_strAppInfo;
};




#endif
