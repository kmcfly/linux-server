#ifndef _GUI_H_
#define _GUI_H_

#ifdef __ENVIRONMENT_WIN32__
#pragma warning (disable : 4244)
#pragma warning (disable : 4018)
#pragma warning (disable : 4267)
#pragma warning (disable : 4996)
#pragma warning (disable : 4800)
#endif

#include "PUB_common.h"
#include "KeyValue.h"
#include "CtrlID.h"

#define GUI_RGB(r,g,b)          (\
								(unsigned long)\
								(\
								(\
								(\
								(BYTE)(b)|\
								((WORD)((BYTE)(g))<<8)\
								)|\
								(((DWORD)(BYTE)(r))<<16)\
								)|\
								(((DWORD)(BYTE)(0xff))<<24)\
								)\
								)

#define R_VALUE(rgb) ((rgb >> 16) & 0xff)
#define G_VALUE(rgb) ((rgb >> 8)  & 0xff)
#define B_VALUE(rgb) (rgb & 0xff)
//////////////////////////////////////////////////////////////////////////
enum
{
	FONT_WIDTH,
	RECT_WIDTH,
	RECT_HEIGHT,
	TITLE_HEIGHT,
	BTN_WIDTH,
	BTN_HEIGHT,
	STATIC_HEIGHT,
	EDIT_HEIGHT,
	COMBO_ARROW_WIDTH,
	COMBO_WIDTH,
	COMBO_HEIGHT,
	CHECK_WIDTH,
	CHECK_HEIGHT,
	LIST_ITEM_HEIGHT,
	LIST_HEAD_HEIGHT,
	LIST_EX_ITEM_HEIGHT,
	RADIO_WIDTH,
	RADIO_HEIGHT,
	SCROLL_WIDTH,	//就是表示宽度，不管是垂直还是水平式的，用于表示不变长的长度。
	SLIDER_WIDTH,	//就是表示宽度，不管是垂直还是水平式的，用于表示不变长的长度。
	TABLE_HEIGHT,
	FLEX_BTN_WIDTH,
	FLEX_BTN_HEIGHT,
	CALENDAR_WIDTH,
	CALENDAR_HEIGHT,
	CALENDAR_BTN_WIDTH,
	CALENDAR_BTN_HEIGHT,
	CALENDARDLG_WIDTH,
	CALENDARDLG_HEIGHT,
	TIME_CTRL_WIDTH,
	TIME_CTRL_HEIGHT,
	DATE_CTRL_WIDTH,
	DATE_CTRL_HEIGHT,
	IP_CTRL_HEIGHT,
	SCHEDULE_ITEM_HEIGHT,
	SCHEDULE_HEADER_HEIGHT,
	ANCHOR_WIDTH,
	ANCHOR_HEIGHT,

	TITLE_EXIT_BTN_RIGHT_XPOS,
	TITLE_EXIT_BTN_YPOS,
	TITLE_EXIT_BTN_WIDTH,
	TITLE_EXIT_BTN_HEIGHT,

	TITLE_CAPTION_XPOS,
	TITLE_CAPTION_YPOS,

	DEFAULT_BTN_RIGHT_XPOS,
	DEFAULT_BTN_BOTTOM_YPOS,
	DEFAULT_BTN_OFFSET,//默认按钮之间的间隔

	STATUS_INFO_XPOS,
	STATUS_INFO_BOTTOM_YPOS,

	EDIT_XPOS,
	EDIT_YPOS,
	TABLE_XPOS,
	TABLE_YPOS,
	STATIC_XPOS,
	STATIC_YPOS,
	LIST_ITEM_XPOS,
	LIST_ITEM_YPOS,
	COMBO_XPOS,
	COMBO_YPOS,

	MSG_PICTURE_XPOS,
	MSG_PICTURE_YPOS,
	MSG_TEXT_XPOS,
	MSG_TEXT_YPOS,

	// 对话框偏移量参考值

	DLG_OFFSET_LEFT,		//客户区域距离窗口左边偏移，左右对称
	DLG_OFFSET_TOP,			//客户区域距离窗口顶部偏移
	DLG_OFFSET_BOTTOM,		//客户区域距离底部偏移
	DLG_OFFSET_SUB_LEFT,	//Tab控件中包含的子窗口，客户区域距离窗口左边偏移，左右对称
	DLG_OFFSET_SUB_TOP,		//Tab控件中包含的子窗口，客户区域距离窗口顶部偏移
	DLG_OFFSET_SUB_BOTTOM,	//Tab控件中包含的子窗口，客户区域距离底部偏移

	DLG_OFFSET_X,			//客户区域，窗体元素参考间距X
	DLG_OFFSET_Y,			//客户区域，窗体元素参考间距Y

	LENGTH_NUM
};

extern unsigned long COLOR_TEXT_NORMAL;
extern unsigned long COLOR_TEXT_TITLE;
extern unsigned long COLOR_TEXT_FOCUS;
extern unsigned long COLOR_TEXT_DISABLE;

extern unsigned long COLOR_TEXT_BORDER;
extern unsigned long COLOR_TEXT_BORDER_ENABLE;

extern unsigned long COLOR_DLG_NORMAL_BK;
extern unsigned long COLOR_DLG_CHILD_BK;
extern unsigned long COLOR_DLG_BORDER_LEFT;
extern unsigned long COLOR_DLG_BORDER_BOTTOM;
extern unsigned long COLOR_SELECT;
extern unsigned long COLOR_DISABLE;
extern unsigned long COLOR_EDIT;

//extern unsigned long COLOR_SCHEDULE_BLACK;
//extern unsigned long COLOR_SCHEDULE_WHITE;
//extern unsigned long COLOR_SCHEDULE_DATA;
extern unsigned long COLOR_LINE_BLACK;
extern unsigned long COLOR_LINE_WHITE;
extern unsigned long COLOR_LINE_GRAY;
extern unsigned long COLOR_LINE_LIVE;

extern unsigned long COLOR_DLG_IMAGE_BK;
extern unsigned long COLOR_CAL_BTN_BK;
extern unsigned long COLOR_CAL_TODAYBTN_BK;
extern unsigned long COLOR_BTN_DEFAULT_MARK;
extern unsigned long COLOR_BTN_BK;
extern unsigned long COLOR_SCROLL_BK;

extern unsigned long COLOR_MENU_BK;
extern unsigned long COLOR_MENU_BK_FOCUS;
extern unsigned long COLOR_BLACK;

extern bool g_langRightOrder;

//////////////////////////////////////////////////////////////////////////
namespace GUI
{
	struct MESSAGE 
	{   
		MESSAGE_TYPE type; 
		unsigned long ctrlID;//处理消息的窗口ID
		unsigned long message;//消息值
		int x;//光标位置 x
		int y;//光标位置 y
	};

	typedef unsigned long (* MESSAGE_FUNC)(void* pParam);
	typedef unsigned long (* MESSAGE_FUNC_EX)(const MESSAGE& msg, void* pParam);

	struct MSG_MAP
	{ 	
		MESSAGE msg; //消息
		MESSAGE_FUNC func;//消息响应函数
		MESSAGE_FUNC_EX funcEx;//消息响应函数扩展，传递光标位置
	};

	//enum DISPATH_RESULT
	//{
	//	DISPATH_CONTINUE,//父窗口继续处理消息
	//	DISPATH_STOP,//父窗口停止处理消息
	//};

	enum WINDOW_STYLE
	{
		WND_CHILD				= 0x0001,	//子窗口,非模式窗口
		WND_POPUP				= 0x0002,	//弹出窗口, 模式窗口
		WND_DISABLE_FOCUS		= 0x0004,	//不接受焦点
		WND_NO_IMAGE_BK			= 0x0008,	//无图片背景，需要手工绘制背景
		WND_DEFAULT_BK			= 0x0010,	//默认背景
		WND_VISIBLE				= 0x0020,	//可见
		WND_NO_TITLE			= 0x0040,	//无标题对话框
		WND_CENTER_HORIZONTAL	= 0x0080,	//水平居中
		WND_CENTER_VERTICAL		= 0x0100,	//垂直居中
		WND_NO_BORDER			= 0x0200,	//没有边框
	};
	//默认控件
	enum DEFAULT_CTRL
	{	
		//按钮
		DEFAULT_BTN_OK = 0x01,//默认OK按钮 对应 CTRLID_DEFAULT_BTN_OK
		DEFAULT_BTN_CANCEL = 0x02,//默认取消按钮 对应 CTRLID_DEFAULT_BTN_CANCEL
		DEFAULT_BTN_APPLY = 0x04,//默认应用按钮 对应 CTRLID_DEFAULT_BTN_APPLY
		//图标
		DEFAULT_ICON_ERROR = 0x100,
		DEFAULT_ICON_WARNING = 0x200,
		DEFAULT_ICON_QUESTION = 0x400,
	};

	//Tip
	enum TIP_TYPE
	{
		TIP_NULL			= 0x00,		//不使用Tip
		TIP_FOCUS			= 0x01,		//使用焦点信息Tip
		TIP_DFOCUS		= 0x02,		//使用动态焦点信息Tip
		TIP_MSG				= 0x04,		//使用普通信息Tip
		TIP_ERR				= 0x08,		//使用错误信息Tip
		TIP_ALL				= ~(0x0),	//使用所有类型Tip

		TIP_COUNT			= 4
	};

	enum WND_TYPE
	{
		WND_TYPE_STATIC,//静态控件
		WND_TYPE_BUTTON,//按钮
		WND_TYPE_CHECK,//复选框
		WND_TYPE_RADIO,//单选框
		WND_TYPE_COMBO,//组合框
		WND_TYPE_COMBO_ITEM,//组合框菜单项
		WND_TYPE_COMBO_MENU,//组合框菜单
		WND_TYPE_COMBO_EX,//下拉式组合框
		WND_TYPE_LIST_BOX,//简单列表控件
		WND_TYPE_LIST_CTRL,//列表控件，关于LIST_BOX和LIST_CTRL的区别,下面讨论
		WND_TYPE_LIST_CTRL_EX,//列表控件，不同上面控件的是，只支持静态文本显示，且没有拖动条，但是有翻页功能，用于显示报表
		WND_TYPE_LIST_HEADER_CTRL,//列表头控件
		WND_TYPE_TAB,//选项卡控件
		WND_TYPE_TAB_BTN,//选项卡控件按钮
		WND_TYPE_CALENDAR,//日历
		WND_TYPE_SCROLL,//滚动条
		WND_TYPE_PROGRESS, //进度条
		WND_TYPE_SCHEDULE,//排程控件,数据分布
		WND_TYPE_SCHEDULE_LIST,//数据浏览视图控件
		WND_TYPE_SLIDER, //滑块
		WND_TYPE_MENU,//菜单
		WND_TYPE_TREE,//树
		WND_TYPE_EDIT,//编辑框
		WND_TYPE_NUM_EDIT,//数字编辑框
		WND_TYPE_RICH_TEXT,//多文本显示框
		WND_TYPE_IP,// IP地址控件
		WND_TYPE_TIME,//时间控件
		WND_TYPE_DATE,//日期控件
		WND_TYPE_DIALOG,//对话框
		WND_TYPE_MESSAGE_BOX, //消息对话框
		WND_TYPE_KEYBOARD,//键盘
		WND_TYPE_KEYBOARD_NUM,//数字键盘
		WND_TYPE_PICTURE,//图片控件，显示图片
		WND_TYPE_GROUP,//组合框控件
		WND_TYPE_SCROLL_WND,//滚动窗口
		WND_TYPE_SCROLL_DIALOG,//滚动对话框
		WND_TYPE_INPUT_PINYIN,//简体拼音输入
		WND_TYPE_ANCHOR,//锚
		WND_TYPE_IPV6,			//IPv6控件
		WND_TYPE_HEX_NUM_EDIT,		//十六进制输入框
		WND_TYPE_KEYBOARD_HEX_NUM,		//十六进制键盘
		WND_TYPE_EDITSTATIC,				//br27专用编辑框，别的不可以使用
		WND_TYPE_INVALID = ~(0x0)//无效窗口类型
	};

	enum DLG_END_VALUE
	{
		DLG_ESC = 0x00, 
		DLG_OK,
		DLG_CANCEL,
		DLG_YES,
		DLG_NO,
		DLG_RETRY,
		DLG_ABORT,
		DLG_IGNORE
	};

	//默认焦点位置
	enum DEFUALT_FOCUS
	{
		DEFUALT_FOCUS_NONE,
		DEFUALT_FOCUS_TOP_LEFT,
		DEFUALT_FOCUS_TOP_RIGHT,
		DEFUALT_FOCUS_BOTTOM_LEFT,
		DEFUALT_FOCUS_BOTTOM_RIGHT,
	};

	//绘制状态属性
	enum PAINT_STATUS
	{
		PAINT_STATUS_NULL		= 0x00,
		PAINT_STATUS_NORMAL		= 0x01,
		PAINT_STATUS_FOCUS		= 0x02,
		PAINT_STATUS_DISABLE	= 0x04,
	};

	enum
	{
		INVALID_ID = ~(0x0),//无效ID
		CENTER_POS = ~(0x0),//中间位置、坐标
		MAX_SCREEN_X = ~(0x0),
		MAX_SCREEN_Y = ~(0x0)
	};
	//对齐方式
	enum ALIGN_TEXT
	{
		ALIGN_TEXT_LEFT,
		ALIGN_TEXT_CENTER,
		ALIGN_TEXT_RIGHT,
	};
}

//////////////////////////////////////////////////////////////////////////
//消息响应
////声明消息响应
#define DECLARE_MSG_MAP()\
	unsigned long OnCommand (unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);

////消息响应开始
#define BEGIN_MSG_MAP(theClass, baseClass) \
	unsigned long theClass::OnCommand (unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)\
{\
	unsigned long retKeyID = keyID; \
	if (KEY_VIRTUAL_MSG_STOP == baseClass::OnCommand (ctrlID, keyID, xPos, yPos)) return KEY_VIRTUAL_MSG_STOP;

////消息响应结束
#define END_MSG_MAP() \
	return retKeyID; }

////响应函数的返回值：false，父窗口不在处理；true，父窗口将继续处理

////一个控件，对应一个按键的响应
////响应函数为 bool Func(); 
#define ON_MSG(_ctrlID, _keyID, Func) \
	if ((ctrlID == _ctrlID) && (keyID == _keyID)) \
	{ \
		retKeyID = Func();\
		if (KEY_VIRTUAL_MSG_STOP == retKeyID) return KEY_VIRTUAL_MSG_STOP;\
	}\
	else if ((GUI::CTRLID_NULL == _ctrlID) && (keyID == _keyID))\
	{\
		retKeyID = Func();\
		if (KEY_VIRTUAL_MSG_STOP == retKeyID) return KEY_VIRTUAL_MSG_STOP;\
}
////带有鼠标位置
////响应函数为 bool Func(unsigned long xPos, unsigned long yPos); 
#define ON_MSG_EX(_ctrlID, _keyID, Func) \
	if ((ctrlID == _ctrlID) && (keyID == _keyID)) \
	{ \
		retKeyID = Func(ctrlID, keyID, xPos, yPos);\
		if(KEY_VIRTUAL_MSG_STOP == retKeyID) return KEY_VIRTUAL_MSG_STOP;\
	}\
	else if ((ctrlID == _ctrlID) && (keyID & KEY_VIRTUAL_MSG_DEFAULT_TAG))\
	{\
		retKeyID = Func(ctrlID, keyID, xPos, yPos);\
		if (KEY_VIRTUAL_MSG_STOP == retKeyID) return KEY_VIRTUAL_MSG_STOP;\
	}\
	else if ((GUI::CTRLID_NULL == _ctrlID) && (keyID == _keyID))\
	{\
		retKeyID = Func(ctrlID, keyID, xPos, yPos);\
		if (KEY_VIRTUAL_MSG_STOP == retKeyID) return KEY_VIRTUAL_MSG_STOP;\
}

////多个控件对应一个按键的响应
////响应函数为 bool Func(unsigned long ctrlID); 
#define ON_MSG_CTRL_RANGE(firstCtrl, lastCtrl, _keyID, Func) \
	if ( (ctrlID >= firstCtrl) && (ctrlID <= lastCtrl) && (keyID == _keyID))\
	{\
	retKeyID = Func(ctrlID);\
	if(KEY_VIRTUAL_MSG_STOP == retKeyID) return KEY_VIRTUAL_MSG_STOP;\
}
////一个控件对应多个按钮的响应
////响应函数为 bool Func(unsigned long keyID); 
#define ON_MSG_KEY_RANGE(_ctrlID, firstKey, lastKey, Func) \
	if ((ctrlID == _ctrlID) && (keyID >= firstKey) && (keyID <= lastKey)) \
	{ \
		retKeyID = Func(keyID);\
		if(KEY_VIRTUAL_MSG_STOP == retKeyID) return KEY_VIRTUAL_MSG_STOP;\
	}\
	else if ((GUI::CTRLID_NULL == _ctrlID)&& (keyID >= firstKey) && (keyID <= lastKey)) \
	{\
		retKeyID = Func(keyID);\
		if(KEY_VIRTUAL_MSG_STOP == retKeyID) return KEY_VIRTUAL_MSG_STOP;\
}
//////////////////////////////////////////////////////////////////////////
#endif//_GUI_H_

