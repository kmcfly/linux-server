#ifndef _SCHEDULE_LIST_CTRL_
#define _SCHEDULE_LIST_CTRL_

#include <map>
#include <vector>
#include "Wnd.h"
#include "ScheduleCtrl.h"

namespace GUI
{
	class CScheduleListCtrl : public GUI::CWnd
	{
	public:
		CScheduleListCtrl();
		virtual ~CScheduleListCtrl();

		enum { SHOW_ALL_ITEM = -1 };

		virtual bool Create( unsigned long ctrlID, 
			unsigned long x, 
			unsigned long y,
			unsigned long cx, 
			unsigned long cy,
			CWnd* pParent,
			int xFocus,
			int yFocus,
			int titleWidth,
			const char* pCaption = NULL,
			int pageSize = SHOW_ALL_ITEM,
			bool bVisible = true);

		bool AddItem(int item, const char* pStr, bool bRefresh);

		bool AddData(int item, 
			BYTE start_hour,
			BYTE start_min,
			BYTE start_second,
			BYTE end_hour,
			BYTE end_min,
			BYTE end_second);

		void ClearData(int item, bool bRefresh);
		void RemoveAllItem(bool bRefresh);
		void RepaintData(int item);

		//移动到最前页面
		void MoveFirst(bool bRefresh);

		//测试(x,y)坐标,所处的大概时间值,有数据范围影响
		bool HitTestTime(unsigned long xPos,unsigned long yPos, BYTE& hour, BYTE& min, BYTE& sec);

		//测试(x,y)坐标,所处的时间
		bool HitTest(unsigned long xPos, unsigned long yPos, int& item, BYTE& hour, BYTE& min, BYTE& sec);

		void SetMarkItem(int item, bool bMark, bool bRefresh = true);

		void SetShowDotFlag();
		void DrawDotFlag(unsigned long xPos, unsigned long yPos, bool bRefresh = false);
	protected:
		DECLARE_MSG_MAP()
		virtual void OnInitial();
		virtual void OnDestroy();
		virtual void OnPaint(PAINT_STATUS status);
		virtual void OnErase(PAINT_STATUS status);

		virtual void OnChangeFocus(unsigned long oldID, unsigned long newID);

		unsigned long OnClickBtnFirst();
		unsigned long OnClickBtnPrev();
		unsigned long OnClickBtnNext();
		unsigned long OnClickBtnLast();

		void SetCurPage(int curPage,bool bRefresh=true);//设置当前页面
		int  GetPageCount() const;//获取页数
		void SetPageSize(int size);//设置每页行数
		void SetPageInfo();//设置行标签信息
		void CreateCtrlBtn();

		typedef std::map<int, GUI::CScheduleCtrl *> DATA_CTRL_MAP;

		unsigned long m_dotXPos;
		DATA_CTRL_MAP m_dataCtrlMap;
		int m_title_width;
		int m_hour_width;
		int m_pageSize;//设置每也显示的项数,SHOW_ALL_ITEMW为全部显示
		int m_curPage;//当前页，m_pageSize 为 SHOW_ALL_ITEMW无效
		enum
		{
			PAGECTRL_STATIC,
			PAGECTRL_BTN_FIRST,
			PAGECTRL_BTN_PREV,
			PAGECTRL_BTN_NEXT,
			PAGECTRL_BTN_LAST,

			PAGECTRL_COUNT,
		};
		GUI::CWnd * m_pWndPageCtrl[PAGECTRL_COUNT];//页面控制 m_pageSize 为 SHOW_ALL_ITEMW无效

	private:
	};
}

#endif
