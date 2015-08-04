#ifndef _SCHEDULE_CTRL_H_
#define _SCHEDULE_CTRL_H_

#include <vector>
#include "Wnd.h"

namespace GUI
{
	class CScheduleCtrl : public GUI::CWnd
	{
	public:
		CScheduleCtrl();
		virtual ~CScheduleCtrl();

		virtual bool Create( unsigned long ctrlID, 
			unsigned long x, 
			unsigned long y,
			unsigned long cx, 
			unsigned long cy,
			CWnd* pParent,
			int xFocus,
			int yFocus,
			const char* pCaption = NULL,
			const char captionWidth = 40,
			bool bVisible = true);

		void AddData(BYTE start_hour,
			BYTE start_min,
			BYTE start_second,
			BYTE end_hour,
			BYTE end_min,
			BYTE end_second);

		void ClearData(bool bRefresh);

		//调整输入的时间,即贴边缘
		void AdjustTime(BYTE& hour,BYTE& min,BYTE& sec);
		//设置标题颜色
		void SetTitilColor(unsigned long color, bool bRefresh = true);
	protected:
		virtual void OnInitial();
		virtual void OnDestroy();
		virtual void OnPaint(PAINT_STATUS status);

	protected:
		unsigned long ToCalendar(BYTE hour,BYTE min,BYTE sec);
		void ToHms(unsigned long cal, BYTE& hour,BYTE& min,BYTE& sec);

		struct DATA_NODE
		{
			BYTE start_hour;
			BYTE start_min;
			BYTE start_second;

			BYTE end_hour;
			BYTE end_min;
			BYTE end_second;
		};
		typedef std::vector<DATA_NODE> DATA_NODE_LIST;

		DATA_NODE_LIST m_dataList;
	
		GUI::CRect m_captionRect;

		int m_caption_width;
		int m_hour_width;
		unsigned long m_titleColor;

	private:
	};
}

#endif
