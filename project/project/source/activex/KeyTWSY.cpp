/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : xiaozhiwen
** Date         : 2011-04-12
** Name         : KeyTWSY.cpp
** Version      : 3.0
** Description  :
** Modify Record:
***********************************************************************/
#include "stdio.h"
#include "KeyTWSY.h"
#include "TWSYKeyValue.h"


CKeyTWSY::CKeyTWSY()
{

}

CKeyTWSY::~CKeyTWSY()
{

}

unsigned long CKeyTWSY::TranKeyTWSY(KEY_MODE mod, unsigned long isDown, unsigned long value)
{
	switch (mod)
	{
	case KEY_MODE_UI:
		return UIProc(value, isDown);
	case KEY_MODE_PTZ:
		return PtzProc(value, isDown);
	case KEY_MODE_PLAYBACK:
		return PlaybackProc(value, isDown);
	case KEY_MODE_FUNC:
		return FuncProc(value, isDown);
	default:
		return 0;
	}
}

unsigned long CKeyTWSY::UIProc(unsigned long value, unsigned long isDown)
{
	if(isDown == 0x01)
	{
		return 0;
	}
	switch(value)
	{
		//中路按键
	case	TWSY_KEY_0:
		return	KEY_VIRTUAL_0;
	case	TWSY_KEY_1:
		return	KEY_VIRTUAL_1;
	case	TWSY_KEY_2:
		return	KEY_VIRTUAL_2;
	case	TWSY_KEY_3:
		return	KEY_VIRTUAL_3;
	case	TWSY_KEY_4:
		return	KEY_VIRTUAL_4;
	case	TWSY_KEY_5:
		return	KEY_VIRTUAL_5;
	case	TWSY_KEY_6:
		return	KEY_VIRTUAL_6;
	case	TWSY_KEY_7:
		return	KEY_VIRTUAL_7;
	case	TWSY_KEY_8:
		return	KEY_VIRTUAL_8;
	case	TWSY_KEY_9:
		return	KEY_VIRTUAL_9;
	case	TWSY_KEY_DOUBLE:
		return	KEY_VIRTUAL_A_A;

		//左路按键
	case	TWSY_KEY_MENU:
		return	KEY_VIRTUAL_MENU;
	case	TWSY_KEY_RECORD:
		return	KEY_VIRTUAL_RECORD;
	case	TWSY_KEY_PTZ:
		return	KEY_VIRTUAL_PTZ;
	case	TWSY_KEY_AUDIO:
		return	KEY_VIRTUAL_AUDIO;
	case	TWSY_KEY_DEWELL:
		return	KEY_VIRTUAL_SEQ;
	case	TWSY_KEY_SNAP:
		return	KEY_VIRTUAL_LIVE_SNAP;
	case	TWSY_KEY_ZOOM:
		return	KEY_VIRTUAL_ZOOM;

		//右路按键
	case	TWSY_KEY_PLAY:
		return	KEY_VIRTUAL_PLAY;
	case	TWSY_KEY_EXIT:	
		return	KEY_VIRTUAL_EXIT;
	case	TWSY_KEY_SEARCH:
		return	KEY_VIRTUAL_SEARCH;
	case	TWSY_KEY_BACKCUP:
		return	KEY_VIRTUAL_BACKUP;
	case	TWSY_KEY_LOG:
		return	KEY_VIRTUAL_LOG;
	case	TWSY_KEY_FORTIFY:
		return KEY_VIRTUAL_ALARM;

		//摇杆按键
	case	TWSY_KEY_VF_ADD:
		return	KEY_VIRTUAL_SPLIT;
	case	TWSY_KEY_VF_DEC:
		return  KEY_VIRTUAL_SPLITEX;
	case	TWSY_KEY_SWITCH_PRE:
		return  KEY_VIRTUAL_GROUPEX;
	case	TWSY_KEY_SWITCH_FORWORD:
		return	KEY_VIRTUAL_GROUP;
	case	TWSY_KEY_ACK:
		return  KEY_VIRTUAL_ENTER;

	case	TWSY_KEY_UP:
		return	KEY_VIRTUAL_UP;
	case	TWSY_KEY_DOWN:
		return	KEY_VIRTUAL_DOWN;
	case	TWSY_KEY_LEFT:
		return	KEY_VIRTUAL_LEFT;
	case	TWSY_KEY_RIGHT:
		return	KEY_VIRTUAL_RIGHT;
	default:
		return 0;
	}
}

unsigned long CKeyTWSY::PtzProc(unsigned long value, unsigned long isDown)
{
	if(isDown == 1)
	{
		return KEY_VIRTUAL_PTZ_STOP;
	}
	switch(value)
	{
		//中路按键
	case	TWSY_KEY_MENU:
		return KEY_VIRTUAL_HIDE;
	case	TWSY_KEY_0:
		return	KEY_VIRTUAL_0;
	case	TWSY_KEY_1:
		return	KEY_VIRTUAL_1;
	case	TWSY_KEY_2:
		return	KEY_VIRTUAL_2;
	case	TWSY_KEY_3:
		return	KEY_VIRTUAL_3;
	case	TWSY_KEY_4:
		return	KEY_VIRTUAL_4;
	case	TWSY_KEY_5:
		return	KEY_VIRTUAL_5;
	case	TWSY_KEY_6:
		return	KEY_VIRTUAL_6;
	case	TWSY_KEY_7:
		return	KEY_VIRTUAL_7;
	case	TWSY_KEY_8:
		return	KEY_VIRTUAL_8;
	case	TWSY_KEY_9:
		return	KEY_VIRTUAL_9;
	case	TWSY_KEY_DOUBLE:
		return	KEY_VIRTUAL_A_A;
	////case	TWSY_KEY_FUNC:
	////	return	KEY_VIRTUAL_;
		//左路按键
	case	TWSY_KEY_IRIS_DEC:
		return	KEY_VIRTUAL_IRIS_DEC;
	case	TWSY_KEY_IRIS_ADD:
		return  KEY_VIRTUAL_IRIS_ADD;
	case	TWSY_KEY_PRESET:
		return	KEY_VIRTUAL_PRESET;
	case	TWSY_KEY_CRUISE:
		return	KEY_VIRTUAL_PTZ_CRUISE;
	case	TWSY_KEY_SCREEN_WIPER:
		return	KEY_VIRTUAL_BRUSH;
	case	TWSY_KEY_LIGHT:
		return	KEY_VIRTUAL_LAMP;
		//右路按键
	case	TWSY_KEY_TRACK:
		return	KEY_VIRTUAL_TRACK;
	case	TWSY_KEY_STOP:
		return	KEY_VIRTUAL_EXIT;

	//	//摇杆按键
	case	TWSY_KEY_ACK:
		return	KEY_VIRTUAL_ENTER;
	case	TWSY_KEY_ZOOM_IN:
		return	KEY_VIRTUAL_ZOOM_ADD;
	case	TWSY_KEY_ZOOM_OUT:
		return	KEY_VIRTUAL_ZOOM_DEC;
	case	TWSY_KEY_FOCUS_ADD:
		return	KEY_VIRTUAL_FOCUS_ADD;
	case	TWSY_KEY_FOCUS_DEC:
		return	KEY_VIRTUAL_FOCUS_DEC;
	case	TWSY_KEY_UP:
		return	KEY_VIRTUAL_PTZ_UP;
	case	TWSY_KEY_RIGHT_UP:
		return	KEY_VIRTUAL_PTZ_RIGHTUP;
	case	TWSY_KEY_DOWN:
		return	KEY_VIRTUAL_PTZ_DOWN;
	case	TWSY_KEY_RIGHT_DOWN:
		return	KEY_VIRTUAL_PTZ_RIGHTDOWN;
	case	TWSY_KEY_LEFT:
		return	KEY_VIRTUAL_PTZ_LEFT;
	case	TWSY_KEY_LEFT_DOWN:
		return	KEY_VIRTUAL_PTZ_LEFTDOWN;
	case	TWSY_KEY_RIGHT:
		return	KEY_VIRTUAL_PTZ_RIGHT;
	case	TWSY_KEY_LEFT_UP:
		return	KEY_VIRTUAL_PTZ_LEFTDOWN;
	default:
		return 0;
	}
}

unsigned long CKeyTWSY::PlaybackProc(unsigned long value, unsigned long isDown)
{
	if(isDown == 0x01)
	{
		return 0;
	}
	switch(value)
	{
		//中路按键
	case	TWSY_KEY_0:
		return	KEY_VIRTUAL_0;
	case	TWSY_KEY_1:
		return	KEY_VIRTUAL_1;
	case	TWSY_KEY_2:
		return	KEY_VIRTUAL_2;
	case	TWSY_KEY_3:
		return	KEY_VIRTUAL_3;
	case	TWSY_KEY_4:
		return	KEY_VIRTUAL_4;
	case	TWSY_KEY_5:
		return	KEY_VIRTUAL_5;
	case	TWSY_KEY_6:
		return	KEY_VIRTUAL_6;
	case	TWSY_KEY_7:
		return	KEY_VIRTUAL_7;
	case	TWSY_KEY_8:
		return	KEY_VIRTUAL_8;
	case	TWSY_KEY_9:
		return	KEY_VIRTUAL_9;
	case	TWSY_KEY_DOUBLE:
		return	KEY_VIRTUAL_A_A;
	//case	TWSY_KEY_FUNC:
	//	return	KEY_VIRTUAL_;
	
	//	//右路按键
	case	TWSY_KEY_PLAY:
		return	KEY_VIRTUAL_PLAY;
	case	TWSY_KEY_EXIT:	
		return	KEY_VIRTUAL_EXIT;
	case	TWSY_KEY_REW:
		return	KEY_VIRTUAL_REW;
	case	TWSY_KEY_FF:
		return	KEY_VIRTUAL_FF;
	case	TWSY_KEY_PRE_SECTION:
		return	KEY_VIRTUAL_PRE_SECTION;
	case	TWSY_KEY_NEXT_SECTION:
		return	KEY_VIRTUAL_NEXT_SECTION;
	//	//摇杆按键
	case	TWSY_KEY_RK_PRE:
		return	KEY_VIRTUAL_PRE_SECTION;
	case	TWSY_KEY_RK_NEXT:
		return	KEY_VIRTUAL_NEXT_SECTION;
	case	TWSY_KEY_RK_REW:
		return	KEY_VIRTUAL_REW;
	case	TWSY_KEY_RK_FF:
		return	KEY_VIRTUAL_FF;
	case	TWSY_KEY_ACK:
		return  KEY_VIRTUAL_ENTER;
	default:
		return 0;
	}
}

unsigned long CKeyTWSY::FuncProc(unsigned long value, unsigned long isDown)
{
	if(isDown == 0x01)
	{
		return 0;
	}
	switch(value)
	{
		//中路按键
	case	TWSY_KEY_0:
		return	KEY_VIRTUAL_0;
	case	TWSY_KEY_1:
		return	KEY_VIRTUAL_1;
	case	TWSY_KEY_2:
		return	KEY_VIRTUAL_2;
	case	TWSY_KEY_3:
		return	KEY_VIRTUAL_3;
	case	TWSY_KEY_4:
		return	KEY_VIRTUAL_4;
	case	TWSY_KEY_5:
		return	KEY_VIRTUAL_5;
	case	TWSY_KEY_6:
		return	KEY_VIRTUAL_6;
	case	TWSY_KEY_7:
		return	KEY_VIRTUAL_7;
	case	TWSY_KEY_8:
		return	KEY_VIRTUAL_8;
	case	TWSY_KEY_9:
		return	KEY_VIRTUAL_9;
	case	TWSY_KEY_DOUBLE:
		return	KEY_VIRTUAL_A_A;

		//左路按键
	case	TWSY_KEY_MENU:
		return	KEY_VIRTUAL_MENU;
	case	TWSY_KEY_RECORD:
		return	KEY_VIRTUAL_RECORD;
	case	TWSY_KEY_PTZ:
		return	KEY_VIRTUAL_PTZ;
	case	TWSY_KEY_AUDIO:
		return	KEY_VIRTUAL_AUDIO;
	case	TWSY_KEY_DEWELL:
		return	KEY_VIRTUAL_SEQ;
	case	TWSY_KEY_SNAP:
		return	KEY_VIRTUAL_PB_SNAP;
	case	TWSY_KEY_ZOOM:
		return	KEY_VIRTUAL_ZOOM;

		//右路按键
	case	TWSY_KEY_PLAY:
		return	KEY_VIRTUAL_PLAY;
	case	TWSY_KEY_EXIT:	
		return	KEY_VIRTUAL_EXIT;
	case	TWSY_KEY_SEARCH:
		return	KEY_VIRTUAL_SEARCH;
	case	TWSY_KEY_BACKCUP:
		return	KEY_VIRTUAL_BACKUP;
	case	TWSY_KEY_LOG:
		return	KEY_VIRTUAL_LOG;
	case	TWSY_KEY_FORTIFY:
		return KEY_VIRTUAL_ALARM;

		//摇杆按键
	case	TWSY_KEY_VF_ADD:
		return	KEY_VIRTUAL_SPLIT;
	case	TWSY_KEY_VF_DEC:
		return  KEY_VIRTUAL_SPLITEX;
	case	TWSY_KEY_SWITCH_PRE:
		return  KEY_VIRTUAL_GROUPEX;
	case	TWSY_KEY_SWITCH_FORWORD:
		return	KEY_VIRTUAL_GROUP;
	case	TWSY_KEY_ACK:
		return  KEY_VIRTUAL_ENTER;

	case	TWSY_KEY_UP:
		return	KEY_VIRTUAL_UP;
	case	TWSY_KEY_DOWN:
		return	KEY_VIRTUAL_DOWN;
	case	TWSY_KEY_LEFT:
		return	KEY_VIRTUAL_LEFT;
	case	TWSY_KEY_RIGHT:
		return	KEY_VIRTUAL_RIGHT;
	default:
		return 0;
	}
}
