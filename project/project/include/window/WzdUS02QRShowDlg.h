#ifndef _WZDPRINTF_CODE_DLG_H_
#define _WZDPRINTF_CODE_DLG_H_

#include "Dialog.h"
#include "WzdUS02PageDlg.h"
#include "RichTextCtrl.h"
#include "CheckCtrl.h"
using namespace GUI;


class CWzdUS02QRShowDlg:public CWzdUS02PageDlg
{
public:
	CWzdUS02QRShowDlg();
	~CWzdUS02QRShowDlg();
	void SetMaix(unsigned char * pMaix,unsigned long w,unsigned long h);
	void SetTipDisk(const std::string& str);
	void SetNoteText(const std::string& str);
	void SetWzdInfo(US02_WzdInfo * pWzdInfo);
	void ShowWzdData(bool bRefresh);
protected:
	void OnInitial();
	void OnPaint(PAINT_STATUS status);
	void OnDestroy();
	DECLARE_MSG_MAP()
	unsigned long OnCheck();
	virtual void InitCtrlInfo();
private:
	void SetDraw(unsigned char pMaix,unsigned long w,unsigned long h);
private:
	unsigned char * m_pMaix;
	unsigned long m_maxW;
	unsigned long m_maxH;

	GUI::CRichTextCtrl	m_richTextCtrl;
	GUI::CRichTextCtrl	m_noterichTextCtrl;
	GUI::CCheckCtrl		m_checkCtrl;				//下次是否显示向导
	US02_WzdInfo	    * m_pWzdInfo;
	int					m_drawStartX;//绘图开始X坐标
	int					m_drawStartY;//绘图开始Y坐标
};

#endif
