#include "KeyRemote.h"

unsigned char CKeyRemote::s_videoInputNum=0;

CKeyRemote::CKeyRemote()
{

}

CKeyRemote::~CKeyRemote()
{

}

unsigned long CKeyRemote::FuncProc( bool isDown, unsigned long value)
{
	switch(value) 
	{
	case KEY_REMOTE_POWER:
		return KEY_VIRTUAL_POWER;
	case KEY_REMOTE_MENU:
		return KEY_VIRTUAL_MENU;
	case KEY_REMOTE_PTZ:
		return KEY_VIRTUAL_PTZ;
	case KEY_REMOTE_BACKUP:
		return KEY_VIRTUAL_BACKUP;
	case KEY_REMOTE_INFO:
		return KEY_VIRTUAL_INFO;
	case KEY_REMOTE_SEARCH:
		return KEY_VIRTUAL_SEARCH;
	case KEY_REMOTE_REC:
		return KEY_VIRTUAL_RECORD;
	case KEY_REMOTE_AUDIO:
		return KEY_VIRTUAL_AUDIO;
	case KEY_REMOTE_SEQ:
		return KEY_VIRTUAL_SEQ;
	case KEY_REMOTE_PLAY:
		return KEY_VIRTUAL_PLAY;
	case KEY_REMOTE_REW:
		return KEY_VIRTUAL_REW;
	case KEY_REMOTE_FF:
		return KEY_VIRTUAL_FF;
	case KEY_REMOTE_STOP:
		return KEY_VIRTUAL_ESC;
	case KEY_REMOTE_SR:
		return KEY_VIRTUAL_SR;
	case KEY_REMOTE_SF:
		return KEY_VIRTUAL_SF;

	case KEY_REMOTE_1:
		return KEY_VIRTUAL_1;
	case KEY_REMOTE_2:
		return KEY_VIRTUAL_2;
	case KEY_REMOTE_3:
		return KEY_VIRTUAL_3;
	case KEY_REMOTE_4:
		return KEY_VIRTUAL_4;
	case KEY_REMOTE_5:
		return KEY_VIRTUAL_5;
	case KEY_REMOTE_6:
		return KEY_VIRTUAL_6;
	case KEY_REMOTE_7:
		return KEY_VIRTUAL_7;
	case KEY_REMOTE_8:
		return KEY_VIRTUAL_8;
	case KEY_REMOTE_9:
		return KEY_VIRTUAL_9;
	case KEY_REMOTE_0:
		{
			if (s_videoInputNum <= 16)
			{
				return KEY_VIRTUAL_10;
			}
			else
			{
				return KEY_VIRTUAL_A_A;
			}
		}		
	case KEY_REMOTE_11:
		return KEY_VIRTUAL_11;
	case KEY_REMOTE_12:
		return KEY_VIRTUAL_12;
	case KEY_REMOTE_13:
		return KEY_VIRTUAL_13;
	case KEY_REMOTE_14:
		return KEY_VIRTUAL_14;
	case KEY_REMOTE_15:
		return KEY_VIRTUAL_15;
	case KEY_REMOTE_16:
		return KEY_VIRTUAL_16;

	case KEY_REMOTE_DIVIDE_1:
		{
			if (s_videoInputNum <= 16)
			{
				return KEY_VIRTUAL_1X1;
			}
			else
			{
				return KEY_VIRTUAL_2X2;
			}
		}		
	case KEY_REMOTE_DIVIDE_2:
		{
			if (s_videoInputNum <= 16)
			{
				return KEY_VIRTUAL_2X2;
			}
			else
			{
				return KEY_VIRTUAL_3X3;
			}
		}		
	case KEY_REMOTE_DIVIDE_3:
		{
			if (s_videoInputNum <= 16)
			{
				return KEY_VIRTUAL_3X3;
			}
			else
			{
				return KEY_VIRTUAL_4X4;
			}
		}		
	case KEY_REMOTE_DIVIDE_4:
		{
			if (s_videoInputNum <= 16)
			{
				return KEY_VIRTUAL_4X4;
			}
			else if (s_videoInputNum <= 25)
			{
				return KEY_VIRTUAL_5X5;
			}
			else
			{
				return KEY_VIRTUAL_6X6;
			}
		}
	case KEY_REMOTE_LEFT:
		return KEY_VIRTUAL_LEFT;
	case KEY_REMOTE_RIGHT:
		return KEY_VIRTUAL_RIGHT;
	case KEY_REMOTE_DOWN:
		return KEY_VIRTUAL_DOWN;
	case KEY_REMOTE_UP:
		return KEY_VIRTUAL_UP;
	case KEY_REMOTE_ENTER:
		return KEY_VIRTUAL_ENTER;

	case KEY_REMOTE_ZOOM_IN:
		return KEY_VIRTUAL_ZOOM_ADD;
	case KEY_REMOTE_ZOOM_OUT:
		return KEY_VIRTUAL_ZOOM_DEC;
	case KEY_REMOTE_FOCUS_ADD:
		return KEY_VIRTUAL_FOCUS_ADD;
	case KEY_REMOTE_FOCUS_DEC:
		return KEY_VIRTUAL_FOCUS_DEC;
	case KEY_REMOTE_IRIS_ADD:
		return KEY_VIRTUAL_IRIS_ADD;
	case KEY_REMOTE_IRIS_DEC:
		return KEY_VIRTUAL_IRIS_DEC;
	case KEY_REMOTE_SPEED_ADD:
		return KEY_VIRTUAL_SPEED_ADD;
	case KEY_REMOTE_SPEED_DEC:
		return KEY_VIRTUAL_SPEED_DEC;

	case KEY_REMOTE_ADD:
		return KEY_VIRTUAL_ADD;
	case KEY_REMOTE_DEC:
		return KEY_VIRTUAL_DEC;
	default:
		{}
	}
	return 0;
}

unsigned long CKeyRemote::PlaybkProc( bool	isDown, unsigned long value)
{
	switch(value) 
	{
	case KEY_REMOTE_POWER:
		return KEY_VIRTUAL_POWER;
	case KEY_REMOTE_MENU:
		return KEY_VIRTUAL_HIDE;
	case KEY_REMOTE_PTZ:
		return KEY_VIRTUAL_PTZ;
	case KEY_REMOTE_INFO:
		return KEY_VIRTUAL_INFO;
	case KEY_REMOTE_SEARCH:
		return KEY_VIRTUAL_SEARCH;
	case KEY_REMOTE_REC:
		return KEY_VIRTUAL_RECORD;
	case KEY_REMOTE_AUDIO:
		return KEY_VIRTUAL_AUDIO;
	case KEY_REMOTE_SEQ:
		return KEY_VIRTUAL_SEQ;
	case KEY_REMOTE_PLAY:
		return KEY_VIRTUAL_PLAY;
	case KEY_REMOTE_REW:
		return KEY_VIRTUAL_REW;
	case KEY_REMOTE_FF:
		return KEY_VIRTUAL_FF;
	case KEY_REMOTE_STOP:
		return KEY_VIRTUAL_STOP;
	case KEY_REMOTE_SR:
		return KEY_VIRTUAL_SR;
	case KEY_REMOTE_SF:
		return KEY_VIRTUAL_SF;

	case KEY_REMOTE_1:
		return KEY_VIRTUAL_1;
	case KEY_REMOTE_2:
		return KEY_VIRTUAL_2;
	case KEY_REMOTE_3:
		return KEY_VIRTUAL_3;
	case KEY_REMOTE_4:
		return KEY_VIRTUAL_4;
	case KEY_REMOTE_5:
		return KEY_VIRTUAL_5;
	case KEY_REMOTE_6:
		return KEY_VIRTUAL_6;
	case KEY_REMOTE_7:
		return KEY_VIRTUAL_7;
	case KEY_REMOTE_8:
		return KEY_VIRTUAL_8;
	case KEY_REMOTE_9:
		return KEY_VIRTUAL_9;
	case KEY_REMOTE_0:
		{
			if (s_videoInputNum <= 16)
			{
				return KEY_VIRTUAL_10;
			}
			else
			{
				return KEY_VIRTUAL_A_A;
			}
		}	
	case KEY_REMOTE_11:
		return KEY_VIRTUAL_11;
	case KEY_REMOTE_12:
		return KEY_VIRTUAL_12;
	case KEY_REMOTE_13:
		return KEY_VIRTUAL_13;
	case KEY_REMOTE_14:
		return KEY_VIRTUAL_14;
	case KEY_REMOTE_15:
		return KEY_VIRTUAL_15;
	case KEY_REMOTE_16:
		return KEY_VIRTUAL_16;

	case KEY_REMOTE_DIVIDE_1:
		return KEY_VIRTUAL_1X1;
	case KEY_REMOTE_DIVIDE_2:
		return KEY_VIRTUAL_2X2;
	case KEY_REMOTE_DIVIDE_3:
		return KEY_VIRTUAL_3X3;
	case KEY_REMOTE_DIVIDE_4:
		return KEY_VIRTUAL_4X4;

	case KEY_REMOTE_LEFT:
		return KEY_VIRTUAL_LEFT;
	case KEY_REMOTE_RIGHT:
		return KEY_VIRTUAL_RIGHT;
	case KEY_REMOTE_DOWN:
		return KEY_VIRTUAL_DOWN;
	case KEY_REMOTE_UP:
		return KEY_VIRTUAL_UP;
	case KEY_REMOTE_ENTER:
		return KEY_VIRTUAL_ENTER;

	case KEY_REMOTE_ZOOM_IN:
		return KEY_VIRTUAL_ZOOM_ADD;
	case KEY_REMOTE_ZOOM_OUT:
		return KEY_VIRTUAL_ZOOM_DEC;
	case KEY_REMOTE_FOCUS_ADD:
		return KEY_VIRTUAL_FOCUS_ADD;
	case KEY_REMOTE_FOCUS_DEC:
		return KEY_VIRTUAL_FOCUS_DEC;
	case KEY_REMOTE_IRIS_ADD:
		return KEY_VIRTUAL_IRIS_ADD;
	case KEY_REMOTE_IRIS_DEC:
		return KEY_VIRTUAL_IRIS_DEC;
	case KEY_REMOTE_SPEED_ADD:
		return KEY_VIRTUAL_SPEED_ADD;
	case KEY_REMOTE_SPEED_DEC:
		return KEY_VIRTUAL_SPEED_DEC;

	case KEY_REMOTE_ADD:
		return KEY_VIRTUAL_ADD;
	case KEY_REMOTE_DEC:
		return KEY_VIRTUAL_DEC;
	default:
		{}
	}
	return 0;
}

unsigned long CKeyRemote::PtzProc( bool	isDown,unsigned long value)
{
	switch(value) 
	{
	case KEY_REMOTE_POWER:
		return KEY_VIRTUAL_POWER;
	case KEY_REMOTE_MENU:
		return KEY_VIRTUAL_MENU;
	case KEY_REMOTE_PTZ:
		return KEY_VIRTUAL_HIDE;
	case KEY_REMOTE_INFO:
		return KEY_VIRTUAL_INFO;
	case KEY_REMOTE_SEARCH:
		return KEY_VIRTUAL_SEARCH;
	case KEY_REMOTE_REC:
		return KEY_VIRTUAL_RECORD;
	case KEY_REMOTE_AUDIO:
		return KEY_VIRTUAL_AUDIO;
	case KEY_REMOTE_SEQ:
		return KEY_VIRTUAL_SEQ;
	case KEY_REMOTE_PLAY:
		return KEY_VIRTUAL_PLAY;
	case KEY_REMOTE_REW:
		return KEY_VIRTUAL_REW;
	case KEY_REMOTE_FF:
		return KEY_VIRTUAL_FF;
	case KEY_REMOTE_STOP:
		return KEY_VIRTUAL_ESC;
	case KEY_REMOTE_SR:
		return KEY_VIRTUAL_SR;
	case KEY_REMOTE_SF:
		return KEY_VIRTUAL_SF;

	case KEY_REMOTE_1:
		return KEY_VIRTUAL_1;
	case KEY_REMOTE_2:
		return KEY_VIRTUAL_2;
	case KEY_REMOTE_3:
		return KEY_VIRTUAL_3;
	case KEY_REMOTE_4:
		return KEY_VIRTUAL_4;
	case KEY_REMOTE_5:
		return KEY_VIRTUAL_5;
	case KEY_REMOTE_6:
		return KEY_VIRTUAL_6;
	case KEY_REMOTE_7:
		return KEY_VIRTUAL_7;
	case KEY_REMOTE_8:
		return KEY_VIRTUAL_8;
	case KEY_REMOTE_9:
		return KEY_VIRTUAL_9;
	case KEY_REMOTE_0:
		{
			if (s_videoInputNum <= 16)
			{
				return KEY_VIRTUAL_10;
			}
			else
			{
				return KEY_VIRTUAL_A_A;
			}
		}	
	case KEY_REMOTE_11:
		return KEY_VIRTUAL_11;
	case KEY_REMOTE_12:
		return KEY_VIRTUAL_12;
	case KEY_REMOTE_13:
		return KEY_VIRTUAL_13;
	case KEY_REMOTE_14:
		return KEY_VIRTUAL_14;
	case KEY_REMOTE_15:
		return KEY_VIRTUAL_15;
	case KEY_REMOTE_16:
		return KEY_VIRTUAL_16;

	case KEY_REMOTE_DIVIDE_1:
		return KEY_VIRTUAL_1X1;
	case KEY_REMOTE_DIVIDE_2:
		return KEY_VIRTUAL_2X2;
	case KEY_REMOTE_DIVIDE_3:
		return KEY_VIRTUAL_3X3;
	case KEY_REMOTE_DIVIDE_4:
		return KEY_VIRTUAL_4X4;

	case KEY_REMOTE_LEFT:
		return KEY_VIRTUAL_PTZ_LEFT;
	case KEY_REMOTE_RIGHT:
		return KEY_VIRTUAL_PTZ_RIGHT;
	case KEY_REMOTE_DOWN:
		return KEY_VIRTUAL_PTZ_DOWN;
	case KEY_REMOTE_UP:
		return KEY_VIRTUAL_PTZ_UP;
	case KEY_REMOTE_ENTER:
		return KEY_VIRTUAL_ENTER;

	case KEY_REMOTE_ZOOM_IN:
		return KEY_VIRTUAL_ZOOM_ADD;
	case KEY_REMOTE_ZOOM_OUT:
		return KEY_VIRTUAL_ZOOM_DEC;
	case KEY_REMOTE_FOCUS_ADD:
		return KEY_VIRTUAL_FOCUS_ADD;
	case KEY_REMOTE_FOCUS_DEC:
		return KEY_VIRTUAL_FOCUS_DEC;
	case KEY_REMOTE_IRIS_ADD:
		return KEY_VIRTUAL_IRIS_ADD;
	case KEY_REMOTE_IRIS_DEC:
		return KEY_VIRTUAL_IRIS_DEC;
	case KEY_REMOTE_SPEED_ADD:
		return KEY_VIRTUAL_SPEED_ADD;
	case KEY_REMOTE_SPEED_DEC:
		return KEY_VIRTUAL_SPEED_DEC;

	case KEY_REMOTE_ADD:
		return KEY_VIRTUAL_ADD;
	case KEY_REMOTE_DEC:
		return KEY_VIRTUAL_DEC;
	default:
		{}
	}
	return 0;
}

unsigned long CKeyRemote::UIProc( bool	isDown,unsigned long value)
{
	switch(value) 
	{
	case KEY_REMOTE_POWER:
		return KEY_VIRTUAL_POWER;
	case KEY_REMOTE_MENU:
		return KEY_VIRTUAL_MENU;
	case KEY_REMOTE_PTZ:
		return KEY_VIRTUAL_PTZ;
	case KEY_REMOTE_BACKUP:
		return KEY_VIRTUAL_BACKUP;
	case KEY_REMOTE_INFO:
		return KEY_VIRTUAL_INFO;
	case KEY_REMOTE_SEARCH:
		return KEY_VIRTUAL_SEARCH;
	case KEY_REMOTE_REC:
		return KEY_VIRTUAL_RECORD;
	case KEY_REMOTE_AUDIO:
		return KEY_VIRTUAL_AUDIO;
	case KEY_REMOTE_SEQ:
		return KEY_VIRTUAL_SEQ;
	case KEY_REMOTE_PLAY:
		return KEY_VIRTUAL_PLAY;
	case KEY_REMOTE_REW:
		return KEY_VIRTUAL_REW;
	case KEY_REMOTE_FF:
		return KEY_VIRTUAL_FF;
	case KEY_REMOTE_STOP:
		return KEY_VIRTUAL_ESC;
	case KEY_REMOTE_SR:
		return KEY_VIRTUAL_SR;
	case KEY_REMOTE_SF:
		return KEY_VIRTUAL_SF;

	case KEY_REMOTE_1:
		return KEY_VIRTUAL_1;
	case KEY_REMOTE_2:
		return KEY_VIRTUAL_2;
	case KEY_REMOTE_3:
		return KEY_VIRTUAL_3;
	case KEY_REMOTE_4:
		return KEY_VIRTUAL_4;
	case KEY_REMOTE_5:
		return KEY_VIRTUAL_5;
	case KEY_REMOTE_6:
		return KEY_VIRTUAL_6;
	case KEY_REMOTE_7:
		return KEY_VIRTUAL_7;
	case KEY_REMOTE_8:
		return KEY_VIRTUAL_8;
	case KEY_REMOTE_9:
		return KEY_VIRTUAL_9;
	case KEY_REMOTE_0:
		return KEY_VIRTUAL_0;
	case KEY_REMOTE_11:
		return KEY_VIRTUAL_11;
	case KEY_REMOTE_12:
		return KEY_VIRTUAL_12;
	case KEY_REMOTE_13:
		return KEY_VIRTUAL_13;
	case KEY_REMOTE_14:
		return KEY_VIRTUAL_14;
	case KEY_REMOTE_15:
		return KEY_VIRTUAL_15;
	case KEY_REMOTE_16:
		return KEY_VIRTUAL_16;

	case KEY_REMOTE_DIVIDE_1:
		return KEY_VIRTUAL_1X1;
	case KEY_REMOTE_DIVIDE_2:
		return KEY_VIRTUAL_2X2;
	case KEY_REMOTE_DIVIDE_3:
		return KEY_VIRTUAL_3X3;
	case KEY_REMOTE_DIVIDE_4:
		return KEY_VIRTUAL_4X4;

	case KEY_REMOTE_LEFT:
		return KEY_VIRTUAL_LEFT;
	case KEY_REMOTE_RIGHT:
		return KEY_VIRTUAL_RIGHT;
	case KEY_REMOTE_DOWN:
		return KEY_VIRTUAL_DOWN;
	case KEY_REMOTE_UP:
		return KEY_VIRTUAL_UP;
	case KEY_REMOTE_ENTER:
		return KEY_VIRTUAL_ENTER;

	case KEY_REMOTE_ZOOM_IN:
		return KEY_VIRTUAL_ZOOM_ADD;
	case KEY_REMOTE_ZOOM_OUT:
		return KEY_VIRTUAL_ZOOM_DEC;
	case KEY_REMOTE_FOCUS_ADD:
		return KEY_VIRTUAL_FOCUS_ADD;
	case KEY_REMOTE_FOCUS_DEC:
		return KEY_VIRTUAL_FOCUS_DEC;
	case KEY_REMOTE_IRIS_ADD:
		return KEY_VIRTUAL_IRIS_ADD;
	case KEY_REMOTE_IRIS_DEC:
		return KEY_VIRTUAL_IRIS_DEC;
	case KEY_REMOTE_SPEED_ADD:
		return KEY_VIRTUAL_SPEED_ADD;
	case KEY_REMOTE_SPEED_DEC:
		return KEY_VIRTUAL_SPEED_DEC;

	case KEY_REMOTE_ADD:
		return KEY_VIRTUAL_ADD;
	case KEY_REMOTE_DEC:
		return KEY_VIRTUAL_DEC;
	default:
		{}
	}
	return 0;
}

unsigned long CKeyRemote::TranKeyRemote(KEY_MODE mod, bool isDown, unsigned long value)
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
		break;
	}	
	return 0;

}


