#ifndef _MY1003_KEYVALUE_H
#define _MY1003_KEYVALUE_H

#define BUTTN_DOWN	0x00
#define BUTTN_UP	0x01

//左路按键
const unsigned long MY1003_KEY_SINGLE				= 0x10;				//单画面
const unsigned long MY1003_KEY_QUAD					= 0x14;				//四画面
const unsigned long MY1003_KEY_NINE					= 0x18;				//九画面
const unsigned long MY1003_KEY_SIXTEEN				= 0x1C;				//十六画面

const unsigned long MY1003_KEY_PTZ					= 0x11;				//
const unsigned long MY1003_KEY_BACKUP    			= 0x12;				//
const unsigned long MY1003_KEY_MENU					= 0x13;				//
const unsigned long MY1003_KEY_SEQ					= 0x15;				//
const unsigned long MY1003_KEY_INFO					= 0x16;				//
const unsigned long MY1003_KEY_SEARCH    			= 0x17;				//
const unsigned long MY1003_KEY_AUDIO      			= 0x19;				
const unsigned long MY1003_KEY_ADD					= 0x1A;
const unsigned long MY1003_KEY_RECORD    			= 0x1B;
const unsigned long MY1003_KEY_POWER				= 0x1D;
const unsigned long MY1003_KEY_DEC					= 0x1E;
const unsigned long MY1003_KEY_PLAY					= 0x1F;

//中路按键
// 1-10	
const unsigned long MY1003_KEY_1     				= 0x20;
const unsigned long MY1003_KEY_2     				= 0x21;
const unsigned long MY1003_KEY_3     				= 0x22;
const unsigned long MY1003_KEY_4     				= 0x23;
const unsigned long MY1003_KEY_5     				= 0x24;
const unsigned long MY1003_KEY_6     				= 0x25;
const unsigned long MY1003_KEY_7     				= 0x26;
const unsigned long MY1003_KEY_8     				= 0x27;
const unsigned long MY1003_KEY_9     				= 0x28;
const unsigned long MY1003_KEY_ESC					= 0x29;
const unsigned long MY1003_KEY_0	     			= 0x2a;				//0和10+在一起
const unsigned long MY1003_KEY_ENTER       			= 0x2b;

const unsigned long MY1003_KEY_10      				= 0x100A;
const unsigned long MY1003_KEY_11       			= 0x100B;
const unsigned long MY1003_KEY_12       			= 0x100C;
const unsigned long MY1003_KEY_13       			= 0x100D;
const unsigned long MY1003_KEY_14       			= 0x100E;
const unsigned long MY1003_KEY_15       			= 0x100F;
const unsigned long MY1003_KEY_16       			= 0x1010;


//右路按键值
const unsigned long MY1003_KEY_SET_PRESET			= 0x30;
const unsigned long MY1003_KEY_FOCUS_ADD   			= 0x31;
const unsigned long MY1003_KEY_FOCUS_DEC   			= 0x32;
const unsigned long MY1003_KEY_DEL_PRESET			= 0x33;
const unsigned long MY1003_KEY_ZOOM_ADD    			= 0x34;
const unsigned long MY1003_KEY_ZOOM_DEC    			= 0x35;
const unsigned long MY1003_KEY_SHOW_PRESET			= 0x36;
const unsigned long MY1003_KEY_IRIS_ADD    			= 0x37;
const unsigned long MY1003_KEY_IRIS_DEC    			= 0x38;
const unsigned long MY1003_KEY_AUTO					= 0x39;
const unsigned long MY1003_KEY_WIPER				= 0x3A;
const unsigned long MY1003_KEY_LIGHT				= 0x3B;

//遥感键值
const unsigned long MY1003_KEY_UP					= 0x40;
const unsigned long MY1003_KEY_DOWN					= 0x41;
const unsigned long MY1003_KEY_LEFT					= 0x42;
const unsigned long MY1003_KEY_RIGHT				= 0x43;

//PTZ模式下键值
const unsigned long MY1003_KEY_PTZ_UP				= 0x40;
const unsigned long MY1003_KEY_PTZ_DOWN				= 0x41;
const unsigned long MY1003_KEY_PTZ_LEFT				= 0x42;
const unsigned long MY1003_KEY_PTZ_RIGHT			= 0x43;
const unsigned long MY1003_KEY_PTZ_LEFTUP			= 0x44;
const unsigned long MY1003_KEY_PTZ_LEFTDOWN			= 0x45;
const unsigned long MY1003_KEY_PTZ_RIGHTUP			= 0x46;
const unsigned long MY1003_KEY_PTZ_RIGHTDOWN		= 0x47;

const unsigned long MY1003_KEY_PTZ_SPEED_ADD		= 0x48;
const unsigned long MY1003_KEY_PTZ_SPEED_DEC		= 0x49;

const unsigned long MY1003_KEY_PTZ_ZERO				= 0x00;
const unsigned long MY1003_KEY_PTZ_STOP				= 0xFF;


//回放模式
const unsigned long MY1003_KEY_REW					= 0x42;
const unsigned long MY1003_KEY_FF					= 0x43;
const unsigned long MY1003_KEY_STOP					= 0x29;


#endif



