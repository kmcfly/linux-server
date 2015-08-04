/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : zhl
** Date         : 2010-06-18
** Name         : KeyTWOEM485.cpp
** Version      : 1.0
** Description  : 把台湾OEM的485键盘键值转换为虚拟键值。
				参看了2.0版本中卢芬写的台湾键盘代码相关内容。
** Modify Record:
1:
***********************************************************************/

#include "KeyTWOEM485.h"
#include "LocalKeyValue.h"

CKeyTWOEM485::CKeyTWOEM485()
{

}

CKeyTWOEM485::~CKeyTWOEM485()
{

}


unsigned long CKeyTWOEM485::TranKeyTWOEM485(KEY_MODE mod, bool isDown, unsigned long value)
{
	switch(mod) 
	{
	case KEY_MODE_PTZ:
		return PtzProc(isDown,value);

	case KEY_MODE_PLAYBACK:
		return PlaybkProc(isDown,value);

	case KEY_MODE_FUNC:
		return FuncProc(isDown,value);

	case KEY_MODE_UI:
		return UIProc(isDown,value);

	default:
		return 0;
	}	
}


unsigned long CKeyTWOEM485::PtzProc( bool isDown,unsigned long value)
{
	switch(value) 
	{
	case TWOEM485_KEY_0:
		return KEY_VIRTUAL_0;
	case TWOEM485_KEY_1:
		return KEY_VIRTUAL_1;
	case TWOEM485_KEY_2:
		return KEY_VIRTUAL_2;
	case TWOEM485_KEY_3:
		return KEY_VIRTUAL_3;
	case TWOEM485_KEY_4:
		return KEY_VIRTUAL_4;
	case TWOEM485_KEY_5:
		return KEY_VIRTUAL_5;
	case TWOEM485_KEY_6:
		return KEY_VIRTUAL_6;
	case TWOEM485_KEY_7:
		return KEY_VIRTUAL_7;
	case TWOEM485_KEY_8:
		return KEY_VIRTUAL_8;
	case TWOEM485_KEY_9:
		return KEY_VIRTUAL_9;

	case TWOEM485_KEY_10:
		return KEY_VIRTUAL_10;
	case TWOEM485_KEY_11:
		return KEY_VIRTUAL_11;	
	case TWOEM485_KEY_12:
		return KEY_VIRTUAL_12;
	case TWOEM485_KEY_13:
		return KEY_VIRTUAL_13;
	case TWOEM485_KEY_14:
		return KEY_VIRTUAL_14;
	case TWOEM485_KEY_15:
		return KEY_VIRTUAL_15;
	case TWOEM485_KEY_16:
		return KEY_VIRTUAL_16;

	case TWOEM485_KEY_2X2:
		return KEY_VIRTUAL_2X2;
	case TWOEM485_KEY_3X3:
		return KEY_VIRTUAL_3X3;
	case TWOEM485_KEY_4X4:
		return KEY_VIRTUAL_4X4;

	case TWOEM485_KEY_BACKUP:
		return KEY_VIRTUAL_BACKUP;
	case TWOEM485_KEY_ZOOM:
		return KEY_VIRTUAL_ZOOM;

	case TWOEM485_KEY_ADD:
		return KEY_VIRTUAL_ADD;
	case TWOEM485_KEY_DEC:
		return KEY_VIRTUAL_DEC;

	case TWOEM485_KEY_MENU:
		return KEY_VIRTUAL_HIDE;
	case TWOEM485_KEY_RECORD:
		return KEY_VIRTUAL_RECORD;
	case TWOEM485_KEY_PLAY:
		return KEY_VIRTUAL_PLAY;
	case TWOEM485_KEY_ESC:
		return KEY_VIRTUAL_ESC;
	case TWOEM485_KEY_ENTER:
		return KEY_VIRTUAL_ENTER;

	case TWOEM485_KEY_UP:
		return KEY_VIRTUAL_PTZ_UP;
	case TWOEM485_KEY_DOWN:
		return KEY_VIRTUAL_PTZ_DOWN;
	case TWOEM485_KEY_LEFT:
		return KEY_VIRTUAL_PTZ_LEFT;
	case TWOEM485_KEY_RIGHT:
		return KEY_VIRTUAL_PTZ_RIGHT;

	case TWOEM485_KEY_AUTO:
		return KEY_VIRTUAL_SEQ;

	default :
		{
			return 0;
		}
	}	
}

unsigned long CKeyTWOEM485::UIProc( bool isDown, unsigned long value)
{
	switch(value) 
	{
	case TWOEM485_KEY_0:
		return KEY_VIRTUAL_0;
	case TWOEM485_KEY_1:
		return KEY_VIRTUAL_1;
	case TWOEM485_KEY_2:
		return KEY_VIRTUAL_2;
	case TWOEM485_KEY_3:
		return KEY_VIRTUAL_3;
	case TWOEM485_KEY_4:
		return KEY_VIRTUAL_4;
	case TWOEM485_KEY_5:
		return KEY_VIRTUAL_5;
	case TWOEM485_KEY_6:
		return KEY_VIRTUAL_6;
	case TWOEM485_KEY_7:
		return KEY_VIRTUAL_7;
	case TWOEM485_KEY_8:
		return KEY_VIRTUAL_8;
	case TWOEM485_KEY_9:
		return KEY_VIRTUAL_9;

	case TWOEM485_KEY_10:
		return KEY_VIRTUAL_10;
	case TWOEM485_KEY_11:
		return KEY_VIRTUAL_11;	
	case TWOEM485_KEY_12:
		return KEY_VIRTUAL_12;
	case TWOEM485_KEY_13:
		return KEY_VIRTUAL_13;
	case TWOEM485_KEY_14:
		return KEY_VIRTUAL_14;
	case TWOEM485_KEY_15:
		return KEY_VIRTUAL_15;
	case TWOEM485_KEY_16:
		return KEY_VIRTUAL_16;

	case TWOEM485_KEY_2X2:
		return KEY_VIRTUAL_2X2;
	case TWOEM485_KEY_3X3:
		return KEY_VIRTUAL_3X3;
	case TWOEM485_KEY_4X4:
		return KEY_VIRTUAL_4X4;

	case TWOEM485_KEY_MENU:
		return KEY_VIRTUAL_MENU;
	case TWOEM485_KEY_RECORD:
		return KEY_VIRTUAL_RECORD;
	case TWOEM485_KEY_PLAY:
		return KEY_VIRTUAL_PLAY;
	case TWOEM485_KEY_ESC:
		return KEY_VIRTUAL_ESC;
	case TWOEM485_KEY_ENTER:
		return KEY_VIRTUAL_ENTER;

	case TWOEM485_KEY_BACKUP:
		return KEY_VIRTUAL_BACKUP;
	case TWOEM485_KEY_ZOOM:
		return KEY_VIRTUAL_ZOOM;

	case TWOEM485_KEY_ADD:
		return KEY_VIRTUAL_ADD;
	case TWOEM485_KEY_DEC:
		return KEY_VIRTUAL_DEC;


	case TWOEM485_KEY_UP:
		return KEY_VIRTUAL_UP;
	case TWOEM485_KEY_DOWN:
		return KEY_VIRTUAL_DOWN;
	case TWOEM485_KEY_LEFT:
		return KEY_VIRTUAL_LEFT;
	case TWOEM485_KEY_RIGHT:
		return KEY_VIRTUAL_RIGHT;

	case TWOEM485_KEY_AUTO:
		return KEY_VIRTUAL_SEQ;

	default :
		{
			return 0;
		}
	}

}

unsigned long CKeyTWOEM485::PlaybkProc( bool isDown,unsigned long value)
{
	switch(value) 
	{
	case TWOEM485_KEY_0:
		return KEY_VIRTUAL_0;
	case TWOEM485_KEY_1:
		return KEY_VIRTUAL_1;
	case TWOEM485_KEY_2:
		return KEY_VIRTUAL_2;
	case TWOEM485_KEY_3:
		return KEY_VIRTUAL_3;
	case TWOEM485_KEY_4:
		return KEY_VIRTUAL_4;
	case TWOEM485_KEY_5:
		return KEY_VIRTUAL_5;
	case TWOEM485_KEY_6:
		return KEY_VIRTUAL_6;
	case TWOEM485_KEY_7:
		return KEY_VIRTUAL_7;
	case TWOEM485_KEY_8:
		return KEY_VIRTUAL_8;
	case TWOEM485_KEY_9:
		return KEY_VIRTUAL_9;

	case TWOEM485_KEY_10:
		return KEY_VIRTUAL_10;
	case TWOEM485_KEY_11:
		return KEY_VIRTUAL_11;	
	case TWOEM485_KEY_12:
		return KEY_VIRTUAL_12;
	case TWOEM485_KEY_13:
		return KEY_VIRTUAL_13;
	case TWOEM485_KEY_14:
		return KEY_VIRTUAL_14;
	case TWOEM485_KEY_15:
		return KEY_VIRTUAL_15;
	case TWOEM485_KEY_16:
		return KEY_VIRTUAL_16;

	case TWOEM485_KEY_2X2:
		return KEY_VIRTUAL_2X2;
	case TWOEM485_KEY_3X3:
		return KEY_VIRTUAL_3X3;
	case TWOEM485_KEY_4X4:
		return KEY_VIRTUAL_4X4;

	case TWOEM485_KEY_MENU:
		return KEY_VIRTUAL_HIDE;
	case TWOEM485_KEY_RECORD:
		return KEY_VIRTUAL_RECORD;
	case TWOEM485_KEY_PLAY:
		return KEY_VIRTUAL_PLAY;
	case TWOEM485_KEY_ESC:
		return KEY_VIRTUAL_ESC;
	case TWOEM485_KEY_ENTER:
		return KEY_VIRTUAL_ENTER;

	case TWOEM485_KEY_BACKUP:
		return KEY_VIRTUAL_BACKUP;
	case TWOEM485_KEY_ZOOM:
		return KEY_VIRTUAL_ZOOM;

	case TWOEM485_KEY_ADD:
		return KEY_VIRTUAL_ADD;
	case TWOEM485_KEY_DEC:
		return KEY_VIRTUAL_DEC;

	case TWOEM485_KEY_UP:
		return KEY_VIRTUAL_UP;
	case TWOEM485_KEY_DOWN:
		return KEY_VIRTUAL_DOWN;
	case TWOEM485_KEY_LEFT:
		return KEY_VIRTUAL_LEFT;
	case TWOEM485_KEY_RIGHT:
		return KEY_VIRTUAL_RIGHT;

	case TWOEM485_KEY_AUTO:
		return KEY_VIRTUAL_SEQ;

	default :
		{
			return 0;
		}
	}
}


unsigned long CKeyTWOEM485::FuncProc( bool isDown, unsigned long value)
{
	switch(value) 
	{
	case TWOEM485_KEY_0:
		return KEY_VIRTUAL_0;
	case TWOEM485_KEY_1:
		return KEY_VIRTUAL_1;
	case TWOEM485_KEY_2:
		return KEY_VIRTUAL_2;
	case TWOEM485_KEY_3:
		return KEY_VIRTUAL_3;
	case TWOEM485_KEY_4:
		return KEY_VIRTUAL_4;
	case TWOEM485_KEY_5:
		return KEY_VIRTUAL_5;
	case TWOEM485_KEY_6:
		return KEY_VIRTUAL_6;
	case TWOEM485_KEY_7:
		return KEY_VIRTUAL_7;
	case TWOEM485_KEY_8:
		return KEY_VIRTUAL_8;
	case TWOEM485_KEY_9:
		return KEY_VIRTUAL_9;

	case TWOEM485_KEY_10:
		return KEY_VIRTUAL_10;
	case TWOEM485_KEY_11:
		return KEY_VIRTUAL_11;	
	case TWOEM485_KEY_12:
		return KEY_VIRTUAL_12;
	case TWOEM485_KEY_13:
		return KEY_VIRTUAL_13;
	case TWOEM485_KEY_14:
		return KEY_VIRTUAL_14;
	case TWOEM485_KEY_15:
		return KEY_VIRTUAL_15;
	case TWOEM485_KEY_16:
		return KEY_VIRTUAL_16;

	case TWOEM485_KEY_2X2:
		return KEY_VIRTUAL_2X2;
	case TWOEM485_KEY_3X3:
		return KEY_VIRTUAL_3X3;
	case TWOEM485_KEY_4X4:
		return KEY_VIRTUAL_4X4;

	case TWOEM485_KEY_MENU:
		return KEY_VIRTUAL_MENU;
	case TWOEM485_KEY_RECORD:
		return KEY_VIRTUAL_RECORD;
	case TWOEM485_KEY_PLAY:
		return KEY_VIRTUAL_PLAY;
	case TWOEM485_KEY_ESC:
		return KEY_VIRTUAL_ESC;
	case TWOEM485_KEY_ENTER:
		return KEY_VIRTUAL_ENTER;

	case TWOEM485_KEY_BACKUP:
		return KEY_VIRTUAL_BACKUP;
	case TWOEM485_KEY_ZOOM:
		return KEY_VIRTUAL_ZOOM;

	case TWOEM485_KEY_ADD:
		return KEY_VIRTUAL_ADD;
	case TWOEM485_KEY_DEC:
		return KEY_VIRTUAL_DEC;

	case TWOEM485_KEY_UP:
		return KEY_VIRTUAL_UP;
	case TWOEM485_KEY_DOWN:
		return KEY_VIRTUAL_DOWN;
	case TWOEM485_KEY_LEFT:
		return KEY_VIRTUAL_LEFT;
	case TWOEM485_KEY_RIGHT:
		return KEY_VIRTUAL_RIGHT;

	case TWOEM485_KEY_AUTO:
		return KEY_VIRTUAL_SEQ;

	default :
		{
			return 0;
		}
	}

}


