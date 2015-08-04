#ifndef _SLIDER_CTRL_H_
#define _SLIDER_CTRL_H_
#include <map>
#include "Wnd.h"
#include "DrawManager.h"

namespace GUI
{
	class CSliderCtrl : public GUI::CWnd
	{
	public:
		CSliderCtrl();
		virtual ~CSliderCtrl();

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
			m_max = max;
			m_min = min;
		}

		int GetRangeMax() const 
		{
			return m_max;
		}

		int GetRangeMin() const
		{
			return m_min;
		}

		void SetCurPos(int pos, bool bRefresh)
		{
			if (pos != m_curPos)
			{
				//如果位置没有发生改变，则需要处理什么。
				assert(pos >= m_min);
				assert(pos <= m_max);
				assert(m_max > m_min);
				m_curPos = pos;

				if (bRefresh)
				{
					Repaint();
				}
			}
		}

		int GetPos() const
		{
			return m_curPos;
		}

		void SetSliderBoxImage(BITMAP_ID normal, BITMAP_ID focus, BITMAP_ID disable);
		void SetBKColor(unsigned long color) { m_bkColor = color; }

		void SetIgnoreStep(bool bIgnore = false) { m_bIgnore = bIgnore; }

		unsigned long OnLeftKey();
		unsigned long OnRightKey();
		unsigned long OnUpKey();
		unsigned long OnDownKey();
		unsigned long OnDecKeyEx();
		unsigned long OnAddKeyEx();
		unsigned long OnDecKey();
		unsigned long OnAddKey();

	protected:
		DECLARE_MSG_MAP()
		virtual void OnInitial();
		virtual void OnDestroy();
		virtual void OnPaint(PAINT_STATUS status);

		unsigned long OnDragStart(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);
		unsigned long OnDragIng(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);
		unsigned long OnDragEnd(unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);

		

		void SetPos(int x);
		int GetSliderBoxPos() const;

		int CalculateBoxLen() const;

		unsigned long SendChange() const;

	private:
		typedef std::map<PAINT_STATUS, BITMAP_ID> SLIDER_IMAGE_MAP;
		SLIDER_IMAGE_MAP m_imageMap;

		double m_max;
		double m_min;
		unsigned long m_bkColor;
		unsigned long m_curPos;
		unsigned long m_boxLen;//滑块长度
		int m_bkHeight;//背景图片高度
		int m_clickPos;//点击滑块时的位置
		bool m_bHorizontal;//是否水平模式

		bool m_bIgnore;		//是否忽略部分滑动消息
	};
}

#endif

