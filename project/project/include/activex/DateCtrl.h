#ifndef _DATE_CTRL_H_
#define _DATE_CTRL_H_

#include "Wnd.h"
#include "NumEditCtrl.h"
#include "Static.h"
#include "Button.h"
#include "CalendarDlg.h"
#include "Calendar.h"
#include <vector>

namespace GUI
{
	class CDateCtrl : public GUI::CWnd
	{
	public:
		CDateCtrl();
		virtual ~CDateCtrl();

		//低16位属于日期,高16位属于时间
		typedef enum _date_mode_
		{
			MODE_YMD		= 0x0001,		//年月日
			MODE_MDY		= 0x0002,		//月日年
			MODE_DMY		= 0x0004,		//日月年

			CHAR_MINUS		= 0x0100,		// '-'
			CHAR_SLANT		= 0x0200,		// '/'
			CHAR_DOT			= 0x0400,		// '.'
		}DATE_MODE;

		typedef enum _date_section_
		{
			SECTION_YEAR	= 0x0001,			//年
			SECTION_MON	= 0x0002,			//月
			SECTION_DAY	= 0x0004,			//日
			SECTION_SYM1	= 0x0008,			//第一分割符
			SECTION_SYM2 = 0x0010,			//第二分割符
			SECTION_BTN	= 0x0020,			//日历按钮

			SECTION_NOTIFY_NO	= 0x0100,		//模式改变后,是否能得到通知修改界面:默认不通知
			SECTION_INPUT_NOR	= 0x0200,		//是否可以点击、键盘的普通输入:默认不可以使用
			SECTION_INPUT_CAL	= 0x0400,		//是否可以使用日历输入:默认可以使用

			SECTION_NUM	= 0x0007,			//数字区域
			SECTION_NULL	= 0x0000,			//
			SECTION_ALL		= ~(0x0),
		}DATE_SECTION;

		typedef enum _date_size_
		{
			WIDTH_YEAR		= 46,
			WIDTH_MON		= 26,
			WIDTH_DAY		= 26,
			WIDTH_SYM		= 6,
			WIDTH_BTN		= 24,

			WIDTH_YMD			= WIDTH_YEAR + WIDTH_MON + WIDTH_DAY + WIDTH_SYM * 2 + 14,
			WIDTH_YMD_BTN	= WIDTH_YEAR + WIDTH_MON + WIDTH_DAY + WIDTH_SYM * 2 + WIDTH_BTN + 16,
			HEIGHT				= 24,
		}DATE_SIZE;

		//////////////////////////////////////////////////////////////////////////

		//设置、获取日期控件显示格式
		static void SetDateMode(unsigned long mode,bool bRefresh=true);
		static unsigned long GetDateMode();
		static unsigned long DateMode(unsigned long value);
		//////////////////////////////////////////////////////////////////////////

	protected:
		//设置、获取日期,内部值都从0开始;外部值按tm结构标准
		CDateCtrl& SetDate (int value, unsigned long section, bool bRefresh);
		CDateCtrl& GetDate (int& value, unsigned long section);

	public:
		////设置、获取标准格式日期
		//void SetDate(tm time,bool bRefresh);
		//tm GetDate(); //注意此接口只填充了[年:月:日],其他默认值0,非完整tm结构

		//设置、获取32位DVR日期
		//void SetDVRDate(unsigned long time, bool bRefresh);
		//unsigned long GetDVRDate();

		//直接设置、获取年月日
		void AddToNotifyList();
		void SetDate(int year,int mon,int day,bool bRefresh);
		void GetDate(int&year, int& mon, int& day);
		int GetYear();//[2008~2025]
		int GetMonth();//[0~11]
		int GetDay();//[1~31+-]

		//刷新显示位置区域
		void RefreshWndPos(bool bRefresh=true);

		//刷新数据到界面
		void RefreshView(unsigned long section=SECTION_NUM,bool bRefresh=true);

		//刷新界面数据到存储区
		void RefreshData(unsigned long section=SECTION_NUM);

	public:
		virtual bool Create(  unsigned long ctrlID, unsigned long x, unsigned long y,unsigned long cx, unsigned long cy,
										CWnd* pParent,int xFocus,int yFocus,unsigned long section=SECTION_ALL);

	protected:
		virtual void OnInitial();
		virtual void OnPaint(PAINT_STATUS status);
		virtual void OnDestroy();
		virtual void OnChangeFocus(unsigned long oldID, unsigned long newID);

	protected:
		//自动计算各个部分的位置
		void AutoCalculateSize(unsigned long cx,unsigned long cy);

		//检查数据合法性
		void CheckDate(int& year, int& mon, int& day);

	//protected:
	//	//设置绘制属性
	//	void SetDrawBg(bool draw=true);
	//	void SetDrawBorder(bool draw=true);

	protected:
		DECLARE_MSG_MAP()
		unsigned long OnChangeYear();
		unsigned long OnChangeMonth();
		unsigned long OnChangeDay();
		unsigned long OnBtnCalendar();

	protected:
		GUI::CNumEditCtrl			m_neNum[3];			//年,月,日 数字显示区
		GUI::CStatic						m_scSymbol[2];		// - / . 符号显示区
		GUI::CButton					m_btnCalendar;		//弹出日历 Button
		
		CCalendar						m_calendar;				//日历
		GUI::CCalendarDlg			m_calDlg;					//日历对话框

		GUI::CRect						m_wndRect[6];			//年-月-日[btn],,,区域

	protected:
		int										m_year;					//年
		int										m_month;					//月[0~11]
		int										m_day;						//日[0~30+-1]

	private:
		bool									m_bInitRange;			//是否初始化了控件范围
		int										m_preDayRangeMax;		//前一次日期最大范围
		unsigned long					m_ctrlSection;			//本类的个性化设置,创建后不可更改

		//如下方式在CDateTimeCtrl中使用
		//bool									m_bDrawBg;			//是否绘制背景
		//bool									m_bDrawBorder;		//是否绘制边框

	private:
		static unsigned long			s_date_mode;			//日期控件显示模式
		static std::vector<CDateCtrl*>	s_vec_datectrl;	//存储已经创建了的控件,修改显示模式时,将直接被通知修改
	};
}

#endif
