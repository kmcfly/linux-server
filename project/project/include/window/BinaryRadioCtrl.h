#ifndef _BINARY_RADIO_CTRL_H_
#define _BINARY_RADIO_CTRL_H_
#include <list>
#include "CheckCtrl.h"

namespace GUI
{
	class CBinaryRadioCtrl : public GUI::CCheckCtrl
	{
	public:
		CBinaryRadioCtrl();
		virtual ~CBinaryRadioCtrl();

		virtual bool Create( unsigned long ctrlID, 
			unsigned long x, 
			unsigned long y,
			unsigned long cx, 
			unsigned long cy,
			CWnd* pParent, 
			unsigned long xFocus, 
			unsigned long yFocus);

		virtual void SetCheck(bool bCheck, bool bRefresh = true);

	protected:
		virtual void OnPaint(PAINT_STATUS status);
		virtual unsigned long OnCommand (unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);
	private:

	};

}
#endif
