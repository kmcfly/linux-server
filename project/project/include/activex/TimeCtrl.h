#ifndef _TIME_CTRL_H_
#define _TIME_CTRL_H_

#include "Wnd.h"
#include "NumEditCtrl.h"
#include "Static.h"
#include "Button.h"
#include <vector>

namespace GUI
{
	//////////////////////////////////////////////////////////////////////////
	//由于默认按钮,绘制方式无法和此类其他控件绘制
	//风格协调,特改写绘制部分
	class CApmButton : public GUI::CButton
	{
	public:
		CApmButton();
		virtual ~CApmButton();

	protected:
		virtual void OnPaint(PAINT_STATUS status);
	};

	//////////////////////////////////////////////////////////////////////////
	////
	//////////////////////////////////////////////////////////////////////////
	class CTimeCtrl : public GUI::CWnd
	{
	public:
		CTimeCtrl();
		virtual ~CTimeCtrl();

		//低16位属于日期,高16位属于时间
		typedef enum _time_mode_
		{
			MODE_H24		= 0x00010000,		//24小时制
			MODE_H12		= 0x00020000,		//12小时制

			CHAR_MINUS		= 0x02000000,		// '-'
			CHAR_COLON		= 0x01000000,		// ':'
			CHAR_DOT		= 0x04000000,		// '.'
		}TIME_MODE;

		typedef enum _time_section_
		{
			//支持组合 [时:分] [分:秒] [时:分:秒]
			SECTION_HOUR	= 0x00010000,		//小时
			SECTION_MIN		= 0x00020000,		//分钟
			SECTION_SEC		= 0x00040000,		//秒
			SECTION_SYM1	= 0x00080000,		//第一分割符
			SECTION_SYM2	= 0x00100000,		//第二分割符
			SECTION_APM		= 0x00200000,		//[AM/PM]

			SECTION_NOTIFY_NO	= 0x01000000,	//模式改变后,是否能得到通知修改界面:默认不通知
			SECTION_INPUT_NOR	= 0x02000000,	//是否可以点击、键盘的普通输入:默认可以使用

			SECTION_NUM		= 0x00270000,		//数字区域
			SECTION_NULL	= 0x00000000,		//
			SECTION_ALL		= ~(0x0),
		}TIME_SECTION;

		typedef enum _time_size_
		{
			WIDTH_HOUR		= 26,
			WIDTH_MIN		= 26,
			WIDTH_SEC		= 26,
			WIDTH_SYM		= 6,
			WIDTH_APM		= 28,

			//组合 [小时:分钟]   [分钟:秒钟]
			WIDTH_HM		= WIDTH_HOUR + WIDTH_MIN + WIDTH_SYM + WIDTH_APM+ 10,
			//默认全长度
			WIDTH_HMS		= WIDTH_HOUR + WIDTH_MIN + WIDTH_SEC + 2 * WIDTH_SYM + WIDTH_APM +14,
			HEIGHT			= 24,
		}TIME_SIZE;

	public:
		//////////////////////////////////////////////////////////////////////////

		//设置、获取时间控件显示格式
		static void SetTimeMode(unsigned long mode,bool bRefresh=true);
		static unsigned long GetTimeMode();
		static unsigned long TimeMode(unsigned long value);

	public:
		static std::string GetTime32Text(unsigned long ultm, unsigned long section = SECTION_ALL);
		static void GetTime32Text(std::string & str, unsigned long ultm, unsigned long section = SECTION_ALL);

	public:
		//将时间转换成字符串,可以单独设定日期或时间;[年][月][日][时][分][秒]
		//不使用时区与夏令时信息,仅使用tm结构中各对应字段
		//必须使用完整tm结构才可以正确输出
		//static std::string GetTimeText(const tm& time, unsigned long section = SECTION_ALL);
		static void GetTimeText(std::string & str, const tm& time, unsigned long section = SECTION_ALL);
		static void GetTimeText(std::string & str,const tm& time,unsigned long dateMode,unsigned long timeMode,unsigned long section = SECTION_ALL);

	public:
		//设置、获取时间
		CTimeCtrl& SetTime(int value, unsigned long section, bool bRefresh);
		CTimeCtrl& GetTime(int& value, unsigned long section);

		//设置、获取标准格式时间
		//void SetTime(tm time,bool bRefresh);
		//tm GetTime();//注意此接口只填充了[时:分:秒],其他默认值0,非完整tm结构

		//设置、获取32位DVR时间
		//void SetDVRTime(unsigned long time, bool bRefresh);
		//unsigned long GetDVRTime();//返回[时:分:秒]的绝对时间长度

		//直接设置、获取时分秒
		void AddToNotifyList();
		void SetTime(unsigned long secTime,bool bRefresh);//[0,86399]一天内的偏移
		void SetTime(int hour,int minute,int second,bool bRefresh);
		void GetTime(int& hour, int& minute, int& second);
		unsigned long GetTime();//[0,86399]一天内的偏移
		int GetHour();
		int GetMinute();
		int GetSecond();

		//刷新显示位置区域
		void RefreshWndPos(bool bRefresh=true);

		//刷新数据到界面
		void RefreshView(unsigned long section=SECTION_NUM,bool bRefresh=true);

		//刷新界面数据到存储区
		void RefreshData(unsigned long section=SECTION_NUM);

		//设置能否使用滚轮,-,+号
		void EnableAddDec(bool enable = true);

		void Dump();

	public:
		virtual bool Create(unsigned long ctrlID, unsigned long x, unsigned long y,unsigned long cx, unsigned long cy,
									CWnd* pParent,int xFocus,int yFocus,unsigned long section=SECTION_ALL);

	protected:
		virtual void OnInitial();
		virtual void OnDestroy();
		virtual void OnPaint(PAINT_STATUS status);
		virtual void OnChangeFocus(unsigned long oldID, unsigned long newID);

	protected:
		//自动计算各个部分的位置,暂不支持动态计算
		void AutoCalculateSize(unsigned long cx,unsigned long cy);

		//检查数据合法性
		void CheckTime(int& hour, int& min, int& sec);

		//当时间制式改变时
		void OnTimeModeChange(bool bRefresh);

	//protected:
	//	//设置绘制属性
	//	void SetDrawBg(bool draw=true);
	//	void SetDrawBorder(bool draw=true);

	protected:
		DECLARE_MSG_MAP()
		unsigned long OnChangeHour();
		unsigned long OnChangeMinute();
		unsigned long OnChangeSecond();
		unsigned long OnChangeAPM();

		unsigned long OnInputNumHour(unsigned long keyID);

	protected:
		GUI::CNumEditCtrl			m_neNum[3];			//时:分:秒
		GUI::CStatic				m_scSymbol[2];		//':' '-' '.' 符号显示区域
		GUI::CRect					m_wndRect[6];		//时-分-秒,,,区域

		GUI::CApmButton				m_btnApm;			//[AM/PM]暂时替代

	protected:
		int							m_hour;				//小时[0,23],记录真实值
		int							m_minute;			//分钟[0,59]
		int							m_second;			//秒钟[0,59]
		bool						m_bAM;				//是否处于上午

	private:
		bool						m_bInitRange;		//是否初始化了控件范围
		int							m_preHourRangeMax;	//前一次设定的小时控件最大变化范围
		unsigned long				m_ctrlSection;		//本类的个性化设置,创建后不可更改

		//如下方式在CDateTimeCtrl中使用
		//bool						m_bDrawBg;			//是否绘制背景
		//bool						m_bDrawBorder;		//是否绘制边框

	private:
		static unsigned long			s_time_mode;	//时间控件显示模式
		static std::vector<CTimeCtrl*>	s_vec_timectrl;	//存储已经创建了的控件,修改显示模式时,将直接被通知修改
	};
}

#endif

