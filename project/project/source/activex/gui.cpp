#include "gui.h"

//普通文本绘制
unsigned long COLOR_TEXT_NORMAL = GUI_RGB(200, 200, 200);
unsigned long COLOR_TEXT_TITLE = GUI_RGB(255, 255, 255);
unsigned long COLOR_TEXT_FOCUS = GUI_RGB(255, 255, 0);
unsigned long COLOR_TEXT_DISABLE = GUI_RGB(128, 128, 128);

//绘制文本时候带描边和描边颜色
unsigned long COLOR_TEXT_BORDER = GUI_RGB(28, 28, 28);
unsigned long COLOR_TEXT_BORDER_ENABLE = 0xFFFFFFFF;

unsigned long COLOR_DLG_NORMAL_BK = GUI_RGB(32, 36, 41);//
unsigned long COLOR_DLG_CHILD_BK = GUI_RGB(32, 36, 41);
unsigned long COLOR_DLG_BORDER_LEFT = GUI_RGB(128, 128, 128);
unsigned long COLOR_DLG_BORDER_BOTTOM = GUI_RGB(20, 20, 20);
unsigned long COLOR_SELECT = GUI_RGB(57, 111, 203);
unsigned long COLOR_DISABLE = GUI_RGB(66, 74, 85);
unsigned long COLOR_EDIT = GUI_RGB(66, 74, 85);

//unsigned long COLOR_SCHEDULE_BLACK = GUI_RGB(60, 60, 60);
//unsigned long COLOR_SCHEDULE_WHITE = GUI_RGB(196, 196, 196);
//unsigned long COLOR_SCHEDULE_DATA = GUI_RGB(57, 111, 203);

//
unsigned long COLOR_LINE_BLACK = GUI_RGB(20, 20, 20);
unsigned long COLOR_LINE_WHITE = GUI_RGB(255, 255, 255);
unsigned long COLOR_LINE_GRAY = GUI_RGB(128, 128, 128);
unsigned long COLOR_LINE_LIVE = GUI_RGB(255, 255, 255);

//日历
unsigned long COLOR_CAL_TODAYBTN_BK = GUI_RGB(100, 100, 255);
unsigned long COLOR_BTN_DEFAULT_MARK = GUI_RGB(128, 128, 255);
unsigned long COLOR_BTN_BK = GUI_RGB(66, 74, 85);

unsigned long COLOR_SCROLL_BK = GUI_RGB(32, 36, 41);

unsigned long COLOR_MENU_BK = GUI_RGB(25, 28, 40);
unsigned long COLOR_MENU_BK_FOCUS = GUI_RGB(192, 192, 192);

unsigned long COLOR_BLACK = GUI_RGB(0, 0, 0);

//语言的从左至右、从右至左的习惯
bool g_langRightOrder = false;

//end
