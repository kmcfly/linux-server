#ifndef _SCROLL_CTRL_H_
#define _SCROLL_CTRL_H_

#include "Wnd.h"
#include "Button.h"
#include "SliderCtrl.h"

namespace GUI
{
	class CScrollCtrl : public GUI::CWnd
	{
	public:
		CScrollCtrl();
		virtual ~CScrollCtrl();

		virtual bool Create( unsigned long ctrlID, 
			unsigned long x, 
			unsigned long y,
			unsigned long len,
			CWnd* pParent,
			int xFocus,
			int yFocus,
			bool bHorizontal = true);

		void SetRange(int min, int max) 
		{
			m_slider.SetRange(min, max);
		}

		int GetRangeMax() const 
		{
			return m_slider.GetRangeMax();
		}

		int GetRangeMin() const
		{
			return m_slider.GetRangeMin();
		}

		void SetCurPos(int pos, bool bRefresh)
		{
			m_slider.SetCurPos(pos, bRefresh);
		}

		int GetPos() const
		{
			return m_slider.GetPos();
		}

		void NextPos()
		{
			m_slider.OnAddKey();
		}

		void PrevPos()
		{
			m_slider.OnDecKey();
		}

		void SetIgnoreStep(bool bIgnore = false)
		{ 
			m_slider.SetIgnoreStep(bIgnore);
		}

	protected:
		DECLARE_MSG_MAP()
		virtual void OnInitial();
		virtual void OnPaint(PAINT_STATUS status);

		///unsigned long OnSliderChange();
		unsigned long OnBtnFirst();
		unsigned long OnBtnSecond();

	private:
		GUI::CButton m_btnFirst;
		GUI::CButton m_btnSecond;
		GUI::CSliderCtrl m_slider;
		bool m_bHorizontal;

		
	};
}

#endif

