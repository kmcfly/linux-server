#include "KeyLocal1US.h"

unsigned char CKeyLocal1US::s_videoInputNum=0;

CKeyLocal1US::CKeyLocal1US()
{
}

CKeyLocal1US::~CKeyLocal1US()
{

}

unsigned long CKeyLocal1US::TranKeyLocal1US(KEY_MODE mod, bool isDown, unsigned long value)
{
	switch(mod) 
	{
	case  KEY_MODE_PTZ:
		return PtzProc(isDown,value);

	case  KEY_MODE_PLAYBACK:
		return PlaybkProc(isDown,value);

	case  KEY_MODE_FUNC:
		return FuncProc(isDown,value);

	case  KEY_MODE_UI:
		return UIProc(isDown,value);

	default:
		return 0;
	}	
}

unsigned long CKeyLocal1US::FuncProc( bool isDown, unsigned long value)
{
	switch(value) 
	{

	case US1_KEY_RECORD:
		return KEY_VIRTUAL_RECORD;
	case US1_KEY_PLAY:
		return KEY_VIRTUAL_PLAY;
	case US1_KEY_REW:
		return KEY_VIRTUAL_REW;
	case US1_KEY_FF:
		return KEY_VIRTUAL_FF;
	case US1_KEY_STOP:
		return ( isDown )? KEY_VIRTUAL_REDRAW : KEY_VIRTUAL_ESC;
	case US1_KEY_INFO:
		return KEY_VIRTUAL_INFO;

	case US1_KEY_ENTER:
		return KEY_VIRTUAL_ENTER;
	case US1_KEY_SEARCH:
		return KEY_VIRTUAL_SEARCH;


	case US1_KEY_LEFT:
		{
			if (s_videoInputNum <= 4)
			{
				return KEY_VIRTUAL_LEFT;
			}
			else if (s_videoInputNum <= 9)
			{
				return KEY_VIRTUAL_2X2;
			}
			else
			{
				return KEY_VIRTUAL_3X3;
			}
		}
		return KEY_VIRTUAL_3X3;	
	case US1_KEY_RIGHT:
		{
			if (s_videoInputNum <= 4)
			{
				return KEY_VIRTUAL_RIGHT;
			}
			else if (s_videoInputNum <= 9)
			{
				return KEY_VIRTUAL_3X3;
			}
			else
			{
				return KEY_VIRTUAL_4X4;
			}
		}
	case US1_KEY_DOWN:
		{
			if (s_videoInputNum <= 4)
			{
				return KEY_VIRTUAL_2X2;
			}
			else if (s_videoInputNum <= 9)
			{
				return KEY_VIRTUAL_1X1;
			}
			else
			{
				return KEY_VIRTUAL_2X2;
			}
		}
	case US1_KEY_UP:
		return KEY_VIRTUAL_1X1;

	case US1_KEY_POWER:
		return KEY_VIRTUAL_POWER;
	default:
		break;
	}
	return 0;

}

unsigned long CKeyLocal1US::PlaybkProc( bool isDown,unsigned long value)
{
	switch(value) 
	{

	case US1_KEY_RECORD:
		return KEY_VIRTUAL_RECORD;
	case US1_KEY_PLAY:
		return KEY_VIRTUAL_PLAY;
	case US1_KEY_REW:
		return KEY_VIRTUAL_REW;
	case US1_KEY_FF:
		return KEY_VIRTUAL_FF;
	case US1_KEY_STOP:
		return ( isDown )? KEY_VIRTUAL_REDRAW : KEY_VIRTUAL_ESC;
	case US1_KEY_INFO:
		return KEY_VIRTUAL_INFO;

	case US1_KEY_ENTER:
		return KEY_VIRTUAL_ENTER;
	case US1_KEY_SEARCH:
		return KEY_VIRTUAL_SEARCH;

	case US1_KEY_LEFT:
		{
			if (s_videoInputNum <= 4)
			{
				return KEY_VIRTUAL_LEFT;
			}
			else if (s_videoInputNum <= 9)
			{
				return KEY_VIRTUAL_2X2;
			}
			else
			{
				return KEY_VIRTUAL_3X3;
			}	
		}
	case US1_KEY_RIGHT:
		{
			if (s_videoInputNum <= 4)
			{
				return KEY_VIRTUAL_RIGHT;
			}
			else if (s_videoInputNum <= 9)
			{
				return KEY_VIRTUAL_3X3;
			}
			else
			{
				return KEY_VIRTUAL_4X4;
			}	
		}
	case US1_KEY_DOWN:
		{
			if (s_videoInputNum <= 4)
			{
				return KEY_VIRTUAL_2X2;
			}
			else if (s_videoInputNum <= 9)
			{
				return KEY_VIRTUAL_1X1;
			}
			else
			{
				return KEY_VIRTUAL_2X2;
			}
		}
	case US1_KEY_UP:
		return KEY_VIRTUAL_1X1;

	case US1_KEY_POWER:
		return KEY_VIRTUAL_POWER;
	default:
		break;
	}
	return 0;

}


unsigned long CKeyLocal1US::PtzProc( bool isDown,unsigned long value)
{
	switch(value) 
	{
	case US1_KEY_RECORD:
		return KEY_VIRTUAL_RECORD;
	case US1_KEY_PLAY:
		return KEY_VIRTUAL_PLAY;
	case US1_KEY_REW:
		return KEY_VIRTUAL_REW;
	case US1_KEY_FF:
		return KEY_VIRTUAL_FF;
	case US1_KEY_STOP:
		return ( isDown )? KEY_VIRTUAL_REDRAW : KEY_VIRTUAL_ESC;
	case US1_KEY_INFO:
		return KEY_VIRTUAL_INFO;

	case US1_KEY_ENTER:
		return KEY_VIRTUAL_ENTER;
	case US1_KEY_SEARCH:
		return KEY_VIRTUAL_SEARCH;

	case US1_KEY_LEFT:
		return KEY_VIRTUAL_PTZ_LEFT;
	case US1_KEY_RIGHT:
		return KEY_VIRTUAL_PTZ_RIGHT;
	case US1_KEY_DOWN:
		return KEY_VIRTUAL_PTZ_DOWN;
	case US1_KEY_UP:
		return KEY_VIRTUAL_PTZ_UP;

	case US1_KEY_POWER:
		return KEY_VIRTUAL_POWER;
	default:
		break;
	}
	return 0;

}

unsigned long CKeyLocal1US::UIProc( bool isDown, unsigned long value)
{
	switch(value) 
	{
	case US1_KEY_RECORD:
		return KEY_VIRTUAL_RECORD;
	case US1_KEY_PLAY:
		return KEY_VIRTUAL_PLAY;
	case US1_KEY_REW:
		return KEY_VIRTUAL_REW;
	case US1_KEY_FF:
		return KEY_VIRTUAL_FF;
	case US1_KEY_STOP:
		return ( isDown )? KEY_VIRTUAL_REDRAW : KEY_VIRTUAL_ESC;
	case US1_KEY_INFO:
		return KEY_VIRTUAL_INFO;

	case US1_KEY_ENTER:
		return KEY_VIRTUAL_ENTER;
	case US1_KEY_SEARCH:
		return KEY_VIRTUAL_SEARCH;

	case US1_KEY_LEFT:
		return KEY_VIRTUAL_LEFT;
	case US1_KEY_RIGHT:
		return KEY_VIRTUAL_RIGHT;
	case US1_KEY_DOWN:
		return KEY_VIRTUAL_DOWN;
	case US1_KEY_UP:
		return KEY_VIRTUAL_UP;

	case US1_KEY_POWER:
		return KEY_VIRTUAL_POWER;
	default:
		break;
	}
	return 0;

}

