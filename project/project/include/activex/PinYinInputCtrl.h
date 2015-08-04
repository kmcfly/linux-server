#ifndef _PIN_YIN_INPUT_CTRL_H_
#define _PIN_YIN_INPUT_CTRL_H_

#include "Wnd.h"
#include "EditCtrl.h"
#include "Button.h"
#include "PinYinInput.h"

namespace GUI
{
	class CPinYinInputCtrl : public GUI::CWnd
	{
	public:
		CPinYinInputCtrl();
		virtual ~CPinYinInputCtrl();

		virtual bool Create(unsigned long ctrlID, 
							unsigned long x, 
							unsigned long y,
							unsigned long cx, 
							unsigned long cy,
							CWnd* pParent,
							int xFocus,
							int yFocus,
							bool bVisible = true);


		//ÉèÖÃÆ´Òô, ch>=0:Ìí¼Ó»òÉ¾³ý×Ö·û;·µ»ØÊÇ·ñ´¦ÔÚ±à¼­Æ´Òô×´Ì¬
		bool SetPinYin(char ch, bool bAdd, bool bRefresh = true);
		void ClearPinYin(bool bRefresh = true);
		std::string GetPinYin(const char ch);

		bool IsInPinYin();

		int GetHitNum(unsigned long xPos, unsigned long yPos);
	protected:
		void OnInitial();
		void OnPaint(PAINT_STATUS status);

	protected:
		DECLARE_MSG_MAP()
		unsigned long OnBtnPrev();
		unsigned long OnBtnNext();

	protected:
		GUI::CEditCtrl m_editPinYin;
		GUI::CEditCtrl m_editCnChar;

		GUI::CButton m_btnPrev;
		GUI::CButton m_btnNext;

		bool m_bInPinYin;

	protected:
		static CPinYinInput* m_pPinYinInput;
	};
}

#endif
