#ifndef _REMOTE_KEY_VALUE_H_
#define _REMOTE_KEY_VALUE_H_

//遥控器的按键值, 同为公司遥控器
const unsigned long KEY_REMOTE_POWER    = 0x8c;
const unsigned long KEY_REMOTE_INFO     = 0x88;
const unsigned long KEY_REMOTE_REC      = 0x80;

const unsigned long KEY_REMOTE_1        = 0x8d;
const unsigned long KEY_REMOTE_2        = 0x89;
const unsigned long KEY_REMOTE_3        = 0x85;
const unsigned long KEY_REMOTE_4        = 0x81;
const unsigned long KEY_REMOTE_5        = 0xcf;
const unsigned long KEY_REMOTE_6        = 0xcb;
const unsigned long KEY_REMOTE_7        = 0xc7;
const unsigned long KEY_REMOTE_8        = 0xc3;
const unsigned long KEY_REMOTE_9        = 0xce;
const unsigned long KEY_REMOTE_0        = 0xca;
const unsigned long KEY_REMOTE_11       = 0xc6;
const unsigned long KEY_REMOTE_12       = 0xc2;
const unsigned long KEY_REMOTE_13       = 0xcd;
const unsigned long KEY_REMOTE_14       = 0xc9;
const unsigned long KEY_REMOTE_15       = 0xc5;
const unsigned long KEY_REMOTE_16       = 0xc1;

const unsigned long KEY_REMOTE_DIVIDE_1 = 0xcc;
const unsigned long KEY_REMOTE_DIVIDE_2 = 0xc8;
const unsigned long KEY_REMOTE_DIVIDE_3 = 0xc4;
const unsigned long KEY_REMOTE_DIVIDE_4 = 0xc0;

const unsigned long KEY_REMOTE_MENU     = 0x8f;
const unsigned long KEY_REMOTE_SEARCH   = 0x83;

const unsigned long KEY_REMOTE_UP       = 0x87;
const unsigned long KEY_REMOTE_LEFT     = 0x8e;
const unsigned long KEY_REMOTE_RIGHT    = 0x8a;
const unsigned long KEY_REMOTE_DOWN     = 0x8b;
const unsigned long KEY_REMOTE_ENTER    = 0x86;

const unsigned long KEY_REMOTE_SR       = 0xdc;
const unsigned long KEY_REMOTE_SF       = 0xd8;
const unsigned long KEY_REMOTE_DEC      = 0xd4;
const unsigned long KEY_REMOTE_ADD      = 0xd0;

const unsigned long KEY_REMOTE_PLAY     = 0xdd;
const unsigned long KEY_REMOTE_REW      = 0xd9;
const unsigned long KEY_REMOTE_FF       = 0xd5;
const unsigned long KEY_REMOTE_STOP     = 0xd1;

const unsigned long KEY_REMOTE_AUDIO    = 0xde; 
const unsigned long KEY_REMOTE_SEQ      = 0xda;
const unsigned long KEY_REMOTE_BACKUP   = 0xd6;

const unsigned long KEY_REMOTE_PTZ         = 0xd2;
const unsigned long KEY_REMOTE_ZOOM_IN     = 0xdf;
const unsigned long KEY_REMOTE_FOCUS_ADD   = 0xdb;
const unsigned long KEY_REMOTE_IRIS_ADD    = 0xd7;
const unsigned long KEY_REMOTE_SPEED_ADD   = 0xd3;

const unsigned long KEY_REMOTE_ZOOM_OUT    = 0x9c;
const unsigned long KEY_REMOTE_FOCUS_DEC   = 0x98;
const unsigned long KEY_REMOTE_IRIS_DEC    = 0x94;
const unsigned long KEY_REMOTE_SPEED_DEC   = 0x90;


//同为2012年的遥控器
const unsigned long KEY_REMOTE_TR12_CUSTOMER_ID = 0x00FF;			//遥控器的客户码

const unsigned long KEY_REMOTE_TR12_REC			= 0x50;
const unsigned long KEY_REMOTE_TR12_POWER		= 0x51;

const unsigned long KEY_REMOTE_TR12_1			= 0x55;
const unsigned long KEY_REMOTE_TR12_2			= 0x17;
const unsigned long KEY_REMOTE_TR12_3			= 0x54;

const unsigned long KEY_REMOTE_TR12_4			= 0x59;
const unsigned long KEY_REMOTE_TR12_5			= 0x1B;
const unsigned long KEY_REMOTE_TR12_6			= 0x58;

const unsigned long KEY_REMOTE_TR12_7			= 0x5D;
const unsigned long KEY_REMOTE_TR12_8			= 0x1F;
const unsigned long KEY_REMOTE_TR12_9			= 0x5C;

const unsigned long KEY_REMOTE_TR12_10			= 0x4D;
const unsigned long KEY_REMOTE_TR12_0			= 0x0F;
const unsigned long KEY_REMOTE_TR12_FN			= 0x4C;

const unsigned long KEY_REMOTE_TR12_SPLIT		= 0x49;		//多画面,在多画面之间切换
const unsigned long KEY_REMOTE_TR12_GROUP		= 0x0B;
const unsigned long KEY_REMOTE_TR12_SEQ			= 0x48;

const unsigned long KEY_REMOTE_TR12_AUDIO		= 0x45;
const unsigned long KEY_REMOTE_TR12_CHOUTPUT	= 0x44;		//切换分辨率，CVBS->VGA->HDMI

const unsigned long KEY_REMOTE_TR12_UP			= 0x07;
const unsigned long KEY_REMOTE_TR12_DOWN		= 0x13;
const unsigned long KEY_REMOTE_TR12_LEFT		= 0x41;
const unsigned long KEY_REMOTE_TR12_RIGHT		= 0x40;
const unsigned long KEY_REMOTE_TR12_ENTER		= 0x03;

const unsigned long KEY_REMOTE_TR12_MENU		= 0x5E;
const unsigned long KEY_REMOTE_TR12_EXIT		= 0x1C;

const unsigned long KEY_REMOTE_TR12_FOCUS_ADD	= 0x5A;
const unsigned long KEY_REMOTE_TR12_FOCUS_DEC	= 0x56;
const unsigned long KEY_REMOTE_TR12_IRIS_ADD	= 0x1E;
const unsigned long KEY_REMOTE_TR12_PTZ			= 0x1A;
const unsigned long KEY_REMOTE_TR12_IRIS_DEC	= 0x16;
const unsigned long KEY_REMOTE_TR12_ZOOM_ADD	= 0x18;
const unsigned long KEY_REMOTE_TR12_ZOOM_DEC	= 0x14;

const unsigned long KEY_REMOTE_TR12_PRESET		= 0x52;
const unsigned long KEY_REMOTE_TR12_CRUISE		= 0x12;
const unsigned long KEY_REMOTE_TR12_BRUSH		= 0x11;
const unsigned long KEY_REMOTE_TR12_LAMP		= 0x10;

const unsigned long KEY_REMOTE_TR12_TRACK		= 0x4E;
const unsigned long KEY_REMOTE_TR12_ESC			= 0x0E; 
const unsigned long KEY_REMOTE_TR12_SCHEDULE	= 0x0D;			//布防
const unsigned long KEY_REMOTE_TR12_INFO		= 0x0C;

const unsigned long KEY_REMOTE_TR12_PLAY		= 0x4A;			//播放、暂停
const unsigned long KEY_REMOTE_TR12_STOP		= 0x0A;			//停止播放
const unsigned long KEY_REMOTE_TR12_SNAP		= 0x09;			//抓图
const unsigned long KEY_REMOTE_TR12_SEARCH		= 0x08;			//搜索

const unsigned long KEY_REMOTE_TR12_PRE_SECTION	= 0x46;			//单帧后退
const unsigned long KEY_REMOTE_TR12_NEXT_SECTION= 0x06;			//单帧前进
const unsigned long KEY_REMOTE_TR12_CLIP		= 0x05;			//剪辑
const unsigned long KEY_REMOTE_TR12_BACKUP		= 0x04;			//备份

const unsigned long KEY_REMOTE_TR12_REW			= 0x42;			//快退
const unsigned long KEY_REMOTE_TR12_FF			= 0x02;			//快进
const unsigned long KEY_REMOTE_TR12_BIG_PIC		= 0x01;			//电子放大
const unsigned long KEY_REMOTE_TR12_PIP			= 0x00;			//画中画
#endif// _REMOTE_KEY_VALUE_H_

