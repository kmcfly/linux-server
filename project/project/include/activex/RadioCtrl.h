#ifndef _RADIO_CTRL_H_
#define _RADIO_CTRL_H_
#include <list>
#include "CheckCtrl.h"

namespace GUI
{
	class CRadioGroup;
	class CRadioCtrl : public GUI::CCheckCtrl
	{
	public:
		CRadioCtrl();
		virtual ~CRadioCtrl();

		virtual bool Create( unsigned long ctrlID, 
			unsigned long x, 
			unsigned long y,
			unsigned long cx, 
			unsigned long cy,
			CWnd* pParent, 
			unsigned long xFocus, 
			unsigned long yFocus, 
			CRadioGroup* pRadioGroup/* = NULL*/);

		virtual void SetCheck(bool bCheck, bool bRefresh = true);
		void AddGroup(CRadioGroup* pRadioGroup); 

	protected:
		virtual void OnPaint(PAINT_STATUS status);
		virtual unsigned long OnCommand (unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);
	private:
		CRadioGroup* m_pRadioGroup;
	};

	typedef std::list<CRadioCtrl*> RADIO_LIST;
	
	class CRadioGroup
	{	
	public:
		CRadioGroup();
		~CRadioGroup();

		void AddRadio(GUI::CRadioCtrl* pRadioCtrl);
		CRadioCtrl* GetCheckRadio();

		void Clear();

	protected:
		RADIO_LIST m_radioList;
	private:
	};
}

#endif
