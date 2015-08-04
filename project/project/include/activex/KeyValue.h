#ifndef _KEY_VALUE_H_
#define _KEY_VALUE_H_

/****************************************************************************************************
*
*			                Mode type define
*
****************************************************************************************************/
enum KEY_MODE
{
	KEY_MODE_UI			= 0x01,
	KEY_MODE_PTZ,
	KEY_MODE_PLAYBACK,
	KEY_MODE_FUNC,
};

/****************************************************************************************************
*
*			                virtual KeyValue range	0x1000-0x10FF
*
****************************************************************************************************/
//数字键
const unsigned long KEY_VIRTUAL_0        = 0x1000;
const unsigned long KEY_VIRTUAL_1        = 0x1001;
const unsigned long KEY_VIRTUAL_2        = 0x1002;
const unsigned long KEY_VIRTUAL_3        = 0x1003;
const unsigned long KEY_VIRTUAL_4        = 0x1004;
const unsigned long KEY_VIRTUAL_5        = 0x1005;
const unsigned long KEY_VIRTUAL_6        = 0x1006;
const unsigned long KEY_VIRTUAL_7        = 0x1007;
const unsigned long KEY_VIRTUAL_8        = 0x1008;
const unsigned long KEY_VIRTUAL_9        = 0x1009;
const unsigned long KEY_VIRTUAL_10       = 0x100a;
const unsigned long KEY_VIRTUAL_11       = 0x100b;
const unsigned long KEY_VIRTUAL_12       = 0x100c;
const unsigned long KEY_VIRTUAL_13       = 0x100d;
const unsigned long KEY_VIRTUAL_14       = 0x100e;
const unsigned long KEY_VIRTUAL_15       = 0x100f;
const unsigned long KEY_VIRTUAL_16       = 0x1010;
const unsigned long KEY_VIRTUAL_17       = 0x1011;
const unsigned long KEY_VIRTUAL_18       = 0x1012;
const unsigned long KEY_VIRTUAL_19       = 0x1013;
const unsigned long KEY_VIRTUAL_20       = 0x1014;
const unsigned long KEY_VIRTUAL_21       = 0x1015;
const unsigned long KEY_VIRTUAL_22       = 0x1016;
const unsigned long KEY_VIRTUAL_23       = 0x1017;
const unsigned long KEY_VIRTUAL_24       = 0x1018;
const unsigned long KEY_VIRTUAL_25       = 0x1019;
const unsigned long KEY_VIRTUAL_26       = 0x101a;
const unsigned long KEY_VIRTUAL_27       = 0x101b;
const unsigned long KEY_VIRTUAL_28       = 0x101c;
const unsigned long KEY_VIRTUAL_29       = 0x101d;
const unsigned long KEY_VIRTUAL_30       = 0x101e;
const unsigned long KEY_VIRTUAL_31       = 0x101f;
const unsigned long KEY_VIRTUAL_32       = 0x1020;

//现场画面
const unsigned long KEY_VIRTUAL_1X1		= 0x1021;
const unsigned long KEY_VIRTUAL_2X2		= 0x1022;
const unsigned long KEY_VIRTUAL_2X3		= 0x1023;
const unsigned long KEY_VIRTUAL_3X3		= 0x1024;
const unsigned long KEY_VIRTUAL_4X4		= 0x1025;
const unsigned long KEY_VIRTUAL_5X5		= 0x1026;
const unsigned long KEY_VIRTUAL_6X6		= 0x1027;

const unsigned long KEY_VIRTUAL_1X1_EX	= 0x1040;
const unsigned long KEY_VIRTUAL_2X2_EX	= 0x1041;
const unsigned long KEY_VIRTUAL_2X3_EX	= 0x1042;
const unsigned long KEY_VIRTUAL_3X3_EX	= 0x1043;
const unsigned long KEY_VIRTUAL_4X4_EX	= 0x1044;
const unsigned long KEY_VIRTUAL_5X5_EX	= 0x1045;
const unsigned long KEY_VIRTUAL_6X6_EX	= 0x1046;

const unsigned long KEY_VIRTUAL_SPLIT	= 0x1047;
const unsigned long KEY_VIRTUAL_GROUP	= 0x1048;
const unsigned long KEY_VIRTUAL_PIP     =0x1049;//画中画
const unsigned long KEY_VIRTUAL_SPLITEX = 0x1050;
const unsigned long KEY_VIRTUAL_GROUPEX = 0x1051;
const unsigned long KEY_VIRTUAL_LIVE_SNAP	= 0x1052;

//功能控制
const unsigned long KEY_VIRTUAL_AUDIO       = 0x2030;
const unsigned long KEY_VIRTUAL_INFO		= 0x2031;
const unsigned long KEY_VIRTUAL_PTZ			= 0x2032;
const unsigned long KEY_VIRTUAL_MENU		= 0x2033;
const unsigned long KEY_VIRTUAL_BACKUP      = 0x2034;
const unsigned long KEY_VIRTUAL_RECORD      = 0x2035;
const unsigned long KEY_VIRTUAL_SEARCH      = 0x2036;
const unsigned long KEY_VIRTUAL_SEQ			= 0x2037;
const unsigned long KEY_VIRTUAL_ARM			= 0x2038;
const unsigned long KEY_VIRTUAL_LOG			= 0x2039;
const unsigned long KEY_VIRTUAL_ALARM		= 0x203a;

//回放控制
const unsigned long KEY_VIRTUAL_PLAY		= 0x3040;
const unsigned long KEY_VIRTUAL_REW			= 0x3041;
const unsigned long KEY_VIRTUAL_FF			= 0x3042;
const unsigned long KEY_VIRTUAL_STOP		= 0x3043;
const unsigned long KEY_VIRTUAL_SF			= 0x3044;
const unsigned long KEY_VIRTUAL_SR			= 0x3045;
const unsigned long KEY_VIRTUAL_PB_ZOOM_ADD	= 0x3046;
const unsigned long KEY_VIRTUAL_PB_ZOOM_DEC	= 0x3047;
const unsigned long KEY_VIRTUAL_PB_SNAP		= 0x3048;
const unsigned long KEY_VIRTUAL_PLAY_EX		= 0x3049;
const unsigned long KEY_VIRTUAL_PRE_SECTION	= 0x3050;
const unsigned long KEY_VIRTUAL_NEXT_SECTION= 0x3051;
const unsigned long KEY_VIRTUAL_CUT			= 0x3052;


//一般功能控制
const unsigned long KEY_VIRTUAL_ESC			= 0x4050;
const unsigned long KEY_VIRTUAL_ENTER       = 0x4051;
const unsigned long KEY_VIRTUAL_UP			= 0x4052;
const unsigned long KEY_VIRTUAL_DOWN		= 0x4053;
const unsigned long KEY_VIRTUAL_LEFT		= 0x4054;
const unsigned long KEY_VIRTUAL_RIGHT       = 0x4055;
const unsigned long KEY_VIRTUAL_DRAG		= 0x4056;
const unsigned long KEY_VIRTUAL_EXIT		= 0x4057;
const unsigned long KEY_VIRTUAL_ADD			= 0x4058;
const unsigned long KEY_VIRTUAL_DEC			= 0x4059;
const unsigned long KEY_VIRTUAL_MOVE		= 0x405a;	//表示鼠标移动
const unsigned long KEY_VIRTUAL_DBCLICK		= 0x405b;	//表示鼠标左键双击
const unsigned long KEY_VIRTUAL_BLUP		= 0x405C;	//鼠标左键弹起

//飞梭虚拟健
const unsigned long KEY_VIRTUAL_FLY_LEFT_1	= 0x5001;
const unsigned long KEY_VIRTUAL_FLY_LEFT_2	= 0x5002;
const unsigned long KEY_VIRTUAL_FLY_LEFT_3	= 0x5003;
const unsigned long KEY_VIRTUAL_FLY_LEFT_4	= 0x5004;
const unsigned long KEY_VIRTUAL_FLY_LEFT_5	= 0x5005;
const unsigned long KEY_VIRTUAL_FLY_LEFT_6	= 0x5006;
const unsigned long KEY_VIRTUAL_FLY_LEFT_7	= 0x5007;
const unsigned long KEY_VIRTUAL_FLY_LEFT_8	= 0x5008;

const unsigned long KEY_VIRTUAL_FLY_MEDIUM	= 0x5017;

const unsigned long KEY_VIRTUAL_FLY_RIGHT_1	= 0x5021;
const unsigned long KEY_VIRTUAL_FLY_RIGHT_2	= 0x5022;
const unsigned long KEY_VIRTUAL_FLY_RIGHT_3	= 0x5023;
const unsigned long KEY_VIRTUAL_FLY_RIGHT_4	= 0x5024;
const unsigned long KEY_VIRTUAL_FLY_RIGHT_5	= 0x5025;
const unsigned long KEY_VIRTUAL_FLY_RIGHT_6	= 0x5026;
const unsigned long KEY_VIRTUAL_FLY_RIGHT_7	= 0x5027;
const unsigned long KEY_VIRTUAL_FLY_RIGHT_8	= 0x5028;

const unsigned long KEY_VIRTUAL_FLFLY_IN_LEFT		= 0x5100;
const unsigned long KEY_VIRTUAL_FLFLY_IN_RIGHT		= 0x5101;

//云台控制
const unsigned long KEY_VIRTUAL_FOCUS_ADD   = 0x6090;
const unsigned long KEY_VIRTUAL_FOCUS_DEC   = 0x6091;
const unsigned long KEY_VIRTUAL_ZOOM_ADD    = 0x6092;
const unsigned long KEY_VIRTUAL_ZOOM_DEC    = 0x6093;
const unsigned long KEY_VIRTUAL_IRIS_ADD    = 0x6094;
const unsigned long KEY_VIRTUAL_IRIS_DEC    = 0x6095;
const unsigned long KEY_VIRTUAL_SPEED_ADD   = 0x6096;
const unsigned long KEY_VIRTUAL_SPEED_DEC   = 0x6097;

const unsigned long KEY_VIRTUAL_PTZ_UP		= 0x6098;
const unsigned long KEY_VIRTUAL_PTZ_DOWN	= 0x6099;
const unsigned long KEY_VIRTUAL_PTZ_LEFT	= 0x609a;
const unsigned long KEY_VIRTUAL_PTZ_RIGHT	= 0x609b;
const unsigned long KEY_VIRTUAL_PTZ_STOP	= 0x609c;

const unsigned long KEY_VIRTUAL_FOCUS		= 0x609d;
const unsigned long KEY_VIRTUAL_ZOOM		= 0x609e;
const unsigned long KEY_VIRTUAL_IRIS		= 0x609f;
const unsigned long KEY_VIRTUAL_PRESET		= 0x60a0;
const unsigned long KEY_VIRTUAL_TRACK		= 0x60a1;
const unsigned long KEY_VIRTUAL_AUTO		= 0x60a2;
const unsigned long KEY_VIRTUAL_SPEED		= 0x60a3;

const unsigned long KEY_VIRTUAL_PTZ_LEFTUP		= 0x60a4;
const unsigned long KEY_VIRTUAL_PTZ_LEFTDOWN	= 0x60a5;
const unsigned long KEY_VIRTUAL_PTZ_RIGHTUP		= 0x60a6;
const unsigned long KEY_VIRTUAL_PTZ_RIGHTDOWN	= 0x60a7;
const unsigned long KEY_VIRTUAL_PTZ_SET_PRESET	= 0x60a8;
const unsigned long KEY_VIRTUAL_BRUSH			= 0x60a9;
const unsigned long KEY_VIRTUAL_LAMP			= 0x60aa;
const unsigned long KEY_VIRTUAL_PTZ_CRUISE		= 0x60ab;


////IPC 
//const unsigned long KEY_VIRTUAL_IPC_FOCUS_DEC	= 0x6601;
//const unsigned long KEY_VIRTUAL_IPC_FOCUS_ADD	= 0x6602;
//const unsigned long KEY_VIRTUAL_IPC_ZOOM_DEC	= 0x6603;
//const unsigned long KEY_VIRTUAL_IPC_ZOOM_ADD	= 0x6604;


//其他功能键
const unsigned long KEY_VIRTUAL_POWER		= 0x9000;
const unsigned long KEY_VIRTUAL_HIDE		= 0x9001;
const unsigned long KEY_VIRTUAL_REDRAW		= 0x9002;//重新初始化输出设备
const unsigned long KEY_VIRTUAL_LCD_ON_OFF  = 0x9003;//关闭5304的lcd屏幕
const unsigned long KEY_VIRTUAL_10_A	    = 0x9004;
const unsigned long KEY_VIRTUAL_A_A	        = 0x9005;

//刻录
const unsigned long KEY_VIRTUAL_BURN	    = 0xA000;//开始关闭刻录
const unsigned long KEY_VIRTUAL_BURN_EX	    = 0xA001;//选择刻录通道

//系统信息对话框
const unsigned long KEY_VIRTUAL_SYSINFO	    = 0xA002;

//切换语言
const unsigned long KEY_VIRTUAL_LANGUAGE    = 0xB000;

const unsigned long KEY_VIRTUAL_OVER  = 0xb000;//获得焦点
const unsigned long KEY_VIRTUAL_OUT   = 0xb001;

const unsigned long KEY_VIRTUAL_MSG_CONTINUE	= 0xFFFe;
const unsigned long KEY_VIRTUAL_MSG_STOP		= 0xFFFF;

const unsigned long KEY_VIRTUAL_MSG_DEFAULT_TAG	= 0x80000000;
#endif  //_KEY_VALUE_H_









