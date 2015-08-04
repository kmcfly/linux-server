
#ifndef _PRINTF_CODE_DLG_H_
#define _PRINTF_CODE_DLG_H_

#include "Dialog.h"
#include "Button.h"
#include "Static.h"
#include "RichTextCtrl.h"
using namespace GUI;

class CPrintQRCodeDlg : public GUI::CDialog
{
public:
	CPrintQRCodeDlg();
	~CPrintQRCodeDlg();
	void SetMaix(unsigned char * pMaix,unsigned long w,unsigned long h);

	void SetDesc(char * desc);
	void SetDescUrl(char * descUrl){strcpy(m_descUrl, descUrl);}
protected:
	void OnInitial();
	void OnPostInitial();
	void OnPaint(PAINT_STATUS status);
	void OnDestroy();
	unsigned long OnOK();
private:
	void SetDraw(unsigned char pMaix,unsigned long w,unsigned long h);
private:
	unsigned char * m_pMaix;
	unsigned long m_maxW;
	unsigned long m_maxH;

	//QR相关数据显示
	GUI::CStatic m_stcDesc;
	char m_desc[64];
	
	GUI::CStatic m_stcDescURL;
	char m_descUrl[128];
};

#endif
