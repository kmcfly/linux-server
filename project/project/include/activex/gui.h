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
	SCROLL_WIDTH,	//���Ǳ�ʾ��ȣ������Ǵ�ֱ����ˮƽʽ�ģ����ڱ�ʾ���䳤�ĳ��ȡ�
	SLIDER_WIDTH,	//���Ǳ�ʾ��ȣ������Ǵ�ֱ����ˮƽʽ�ģ����ڱ�ʾ���䳤�ĳ��ȡ�
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
	DEFAULT_BTN_OFFSET,//Ĭ�ϰ�ť֮��ļ��

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

	// �Ի���ƫ�����ο�ֵ

	DLG_OFFSET_LEFT,		//�ͻ�������봰�����ƫ�ƣ����ҶԳ�
	DLG_OFFSET_TOP,			//�ͻ�������봰�ڶ���ƫ��
	DLG_OFFSET_BOTTOM,		//�ͻ��������ײ�ƫ��
	DLG_OFFSET_SUB_LEFT,	//Tab�ؼ��а������Ӵ��ڣ��ͻ�������봰�����ƫ�ƣ����ҶԳ�
	DLG_OFFSET_SUB_TOP,		//Tab�ؼ��а������Ӵ��ڣ��ͻ�������봰�ڶ���ƫ��
	DLG_OFFSET_SUB_BOTTOM,	//Tab�ؼ��а������Ӵ��ڣ��ͻ��������ײ�ƫ��

	DLG_OFFSET_X,			//�ͻ����򣬴���Ԫ�زο����X
	DLG_OFFSET_Y,			//�ͻ����򣬴���Ԫ�زο����Y

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
		unsigned long ctrlID;//������Ϣ�Ĵ���ID
		unsigned long message;//��Ϣֵ
		int x;//���λ�� x
		int y;//���λ�� y
	};

	typedef unsigned long (* MESSAGE_FUNC)(void* pParam);
	typedef unsigned long (* MESSAGE_FUNC_EX)(const MESSAGE& msg, void* pParam);

	struct MSG_MAP
	{ 	
		MESSAGE msg; //��Ϣ
		MESSAGE_FUNC func;//��Ϣ��Ӧ����
		MESSAGE_FUNC_EX funcEx;//��Ϣ��Ӧ������չ�����ݹ��λ��
	};

	//enum DISPATH_RESULT
	//{
	//	DISPATH_CONTINUE,//�����ڼ���������Ϣ
	//	DISPATH_STOP,//������ֹͣ������Ϣ
	//};

	enum WINDOW_STYLE
	{
		WND_CHILD				= 0x0001,	//�Ӵ���,��ģʽ����
		WND_POPUP				= 0x0002,	//��������, ģʽ����
		WND_DISABLE_FOCUS		= 0x0004,	//�����ܽ���
		WND_NO_IMAGE_BK			= 0x0008,	//��ͼƬ��������Ҫ�ֹ����Ʊ���
		WND_DEFAULT_BK			= 0x0010,	//Ĭ�ϱ���
		WND_VISIBLE				= 0x0020,	//�ɼ�
		WND_NO_TITLE			= 0x0040,	//�ޱ���Ի���
		WND_CENTER_HORIZONTAL	= 0x0080,	//ˮƽ����
		WND_CENTER_VERTICAL		= 0x0100,	//��ֱ����
		WND_NO_BORDER			= 0x0200,	//û�б߿�
	};
	//Ĭ�Ͽؼ�
	enum DEFAULT_CTRL
	{	
		//��ť
		DEFAULT_BTN_OK = 0x01,//Ĭ��OK��ť ��Ӧ CTRLID_DEFAULT_BTN_OK
		DEFAULT_BTN_CANCEL = 0x02,//Ĭ��ȡ����ť ��Ӧ CTRLID_DEFAULT_BTN_CANCEL
		DEFAULT_BTN_APPLY = 0x04,//Ĭ��Ӧ�ð�ť ��Ӧ CTRLID_DEFAULT_BTN_APPLY
		//ͼ��
		DEFAULT_ICON_ERROR = 0x100,
		DEFAULT_ICON_WARNING = 0x200,
		DEFAULT_ICON_QUESTION = 0x400,
	};

	//Tip
	enum TIP_TYPE
	{
		TIP_NULL			= 0x00,		//��ʹ��Tip
		TIP_FOCUS			= 0x01,		//ʹ�ý�����ϢTip
		TIP_DFOCUS		= 0x02,		//ʹ�ö�̬������ϢTip
		TIP_MSG				= 0x04,		//ʹ����ͨ��ϢTip
		TIP_ERR				= 0x08,		//ʹ�ô�����ϢTip
		TIP_ALL				= ~(0x0),	//ʹ����������Tip

		TIP_COUNT			= 4
	};

	enum WND_TYPE
	{
		WND_TYPE_STATIC,//��̬�ؼ�
		WND_TYPE_BUTTON,//��ť
		WND_TYPE_CHECK,//��ѡ��
		WND_TYPE_RADIO,//��ѡ��
		WND_TYPE_COMBO,//��Ͽ�
		WND_TYPE_COMBO_ITEM,//��Ͽ�˵���
		WND_TYPE_COMBO_MENU,//��Ͽ�˵�
		WND_TYPE_COMBO_EX,//����ʽ��Ͽ�
		WND_TYPE_LIST_BOX,//���б�ؼ�
		WND_TYPE_LIST_CTRL,//�б�ؼ�������LIST_BOX��LIST_CTRL������,��������
		WND_TYPE_LIST_CTRL_EX,//�б�ؼ�����ͬ����ؼ����ǣ�ֻ֧�־�̬�ı���ʾ����û���϶����������з�ҳ���ܣ�������ʾ����
		WND_TYPE_LIST_HEADER_CTRL,//�б�ͷ�ؼ�
		WND_TYPE_TAB,//ѡ��ؼ�
		WND_TYPE_TAB_BTN,//ѡ��ؼ���ť
		WND_TYPE_CALENDAR,//����
		WND_TYPE_SCROLL,//������
		WND_TYPE_PROGRESS, //������
		WND_TYPE_SCHEDULE,//�ų̿ؼ�,���ݷֲ�
		WND_TYPE_SCHEDULE_LIST,//���������ͼ�ؼ�
		WND_TYPE_SLIDER, //����
		WND_TYPE_MENU,//�˵�
		WND_TYPE_TREE,//��
		WND_TYPE_EDIT,//�༭��
		WND_TYPE_NUM_EDIT,//���ֱ༭��
		WND_TYPE_RICH_TEXT,//���ı���ʾ��
		WND_TYPE_IP,// IP��ַ�ؼ�
		WND_TYPE_TIME,//ʱ��ؼ�
		WND_TYPE_DATE,//���ڿؼ�
		WND_TYPE_DIALOG,//�Ի���
		WND_TYPE_MESSAGE_BOX, //��Ϣ�Ի���
		WND_TYPE_KEYBOARD,//����
		WND_TYPE_KEYBOARD_NUM,//���ּ���
		WND_TYPE_PICTURE,//ͼƬ�ؼ�����ʾͼƬ
		WND_TYPE_GROUP,//��Ͽ�ؼ�
		WND_TYPE_SCROLL_WND,//��������
		WND_TYPE_SCROLL_DIALOG,//�����Ի���
		WND_TYPE_INPUT_PINYIN,//����ƴ������
		WND_TYPE_ANCHOR,//ê
		WND_TYPE_IPV6,			//IPv6�ؼ�
		WND_TYPE_HEX_NUM_EDIT,		//ʮ�����������
		WND_TYPE_KEYBOARD_HEX_NUM,		//ʮ�����Ƽ���
		WND_TYPE_EDITSTATIC,				//br27ר�ñ༭�򣬱�Ĳ�����ʹ��
		WND_TYPE_INVALID = ~(0x0)//��Ч��������
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

	//Ĭ�Ͻ���λ��
	enum DEFUALT_FOCUS
	{
		DEFUALT_FOCUS_NONE,
		DEFUALT_FOCUS_TOP_LEFT,
		DEFUALT_FOCUS_TOP_RIGHT,
		DEFUALT_FOCUS_BOTTOM_LEFT,
		DEFUALT_FOCUS_BOTTOM_RIGHT,
	};

	//����״̬����
	enum PAINT_STATUS
	{
		PAINT_STATUS_NULL		= 0x00,
		PAINT_STATUS_NORMAL		= 0x01,
		PAINT_STATUS_FOCUS		= 0x02,
		PAINT_STATUS_DISABLE	= 0x04,
	};

	enum
	{
		INVALID_ID = ~(0x0),//��ЧID
		CENTER_POS = ~(0x0),//�м�λ�á�����
		MAX_SCREEN_X = ~(0x0),
		MAX_SCREEN_Y = ~(0x0)
	};
	//���뷽ʽ
	enum ALIGN_TEXT
	{
		ALIGN_TEXT_LEFT,
		ALIGN_TEXT_CENTER,
		ALIGN_TEXT_RIGHT,
	};
}

//////////////////////////////////////////////////////////////////////////
//��Ϣ��Ӧ
////������Ϣ��Ӧ
#define DECLARE_MSG_MAP()\
	unsigned long OnCommand (unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos);

////��Ϣ��Ӧ��ʼ
#define BEGIN_MSG_MAP(theClass, baseClass) \
	unsigned long theClass::OnCommand (unsigned long ctrlID, unsigned long keyID, unsigned long xPos, unsigned long yPos)\
{\
	unsigned long retKeyID = keyID; \
	if (KEY_VIRTUAL_MSG_STOP == baseClass::OnCommand (ctrlID, keyID, xPos, yPos)) return KEY_VIRTUAL_MSG_STOP;

////��Ϣ��Ӧ����
#define END_MSG_MAP() \
	return retKeyID; }

////��Ӧ�����ķ���ֵ��false�������ڲ��ڴ���true�������ڽ���������

////һ���ؼ�����Ӧһ����������Ӧ
////��Ӧ����Ϊ bool Func(); 
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
////�������λ��
////��Ӧ����Ϊ bool Func(unsigned long xPos, unsigned long yPos); 
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

////����ؼ���Ӧһ����������Ӧ
////��Ӧ����Ϊ bool Func(unsigned long ctrlID); 
#define ON_MSG_CTRL_RANGE(firstCtrl, lastCtrl, _keyID, Func) \
	if ( (ctrlID >= firstCtrl) && (ctrlID <= lastCtrl) && (keyID == _keyID))\
	{\
	retKeyID = Func(ctrlID);\
	if(KEY_VIRTUAL_MSG_STOP == retKeyID) return KEY_VIRTUAL_MSG_STOP;\
}
////һ���ؼ���Ӧ�����ť����Ӧ
////��Ӧ����Ϊ bool Func(unsigned long keyID); 
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

