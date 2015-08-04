#include "KeyLocal1U.h"

unsigned char CKeyLocal1U::s_videoInputNum;

CKeyLocal1U::CKeyLocal1U()
{
}

CKeyLocal1U::~CKeyLocal1U()
{

}

unsigned long CKeyLocal1U::TranKeyLocal(KEY_MODE mod, bool isDown, unsigned long value)
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

unsigned long CKeyLocal1U::FuncProc( bool isDown, unsigned long value)
{
	switch(value) 
	{
	case U1_KEY_1:
		return KEY_VIRTUAL_1;
	case U1_KEY_2:
		return KEY_VIRTUAL_2;
	case U1_KEY_3:
		return KEY_VIRTUAL_3;
	case U1_KEY_4:
		return KEY_VIRTUAL_4;
	case U1_KEY_5:
		return KEY_VIRTUAL_5;
	case U1_KEY_6:
		return KEY_VIRTUAL_6;
	case U1_KEY_7:
		return KEY_VIRTUAL_7;
	case U1_KEY_8:
		return KEY_VIRTUAL_8;
	case U1_KEY_9:
		return KEY_VIRTUAL_9;
	case U1_KEY_DEC:
		return KEY_VIRTUAL_BACKUP;
	case U1_KEY_10:
		if(s_videoInputNum <=16)
		{
			return KEY_VIRTUAL_10_A;
		}
		else
		{
			return KEY_VIRTUAL_A_A;
		}
	case U1_KEY_UP:
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
	case U1_KEY_DOWN:
		{
			if (s_videoInputNum == 8)
			{
				return KEY_VIRTUAL_1X1;
			}
			else if (s_videoInputNum <= 16)
			{
				return KEY_VIRTUAL_2X2;
			}
			else
			{
				return KEY_VIRTUAL_3X3	;
			}
		}
	case U1_KEY_RIGHT:
		{
			if (s_videoInputNum <= 4)
			{
				return KEY_VIRTUAL_RIGHT;
			}
			else if (s_videoInputNum <= 9)
			{
				return KEY_VIRTUAL_3X3;
			}
			else if (s_videoInputNum <= 12)
			{
				return KEY_VIRTUAL_4X4;
			}
			else if (s_videoInputNum <= 16)
			{
				return KEY_VIRTUAL_4X4;
			}
			else if(s_videoInputNum <= 25)
			{
				return KEY_VIRTUAL_5X5;
			}
			else
			{
				return KEY_VIRTUAL_6X6;
			}
		}
	case U1_KEY_LEFT:
		{
			if (s_videoInputNum <= 4)
			{
				return KEY_VIRTUAL_LEFT;
			}
			else if (s_videoInputNum <= 9)
			{
				return KEY_VIRTUAL_2X2;
			}
			else if (s_videoInputNum <= 16)
			{
				return KEY_VIRTUAL_3X3;
			}
			else
			{
				return KEY_VIRTUAL_4X4;
			}
		}
	case U1_KEY_REW:
		return KEY_VIRTUAL_REW;
	case U1_KEY_ENTER:
		return KEY_VIRTUAL_ENTER;
	case U1_KEY_FF:
		return KEY_VIRTUAL_PTZ;
	case U1_KEY_ADD:
		return isDown ? KEY_VIRTUAL_LCD_ON_OFF : KEY_VIRTUAL_MENU;
	case U1_KEY_RECORD:
		return KEY_VIRTUAL_RECORD;
	case U1_KEY_SEARCH:
		return KEY_VIRTUAL_SEARCH;
	case U1_KEY_PLAY:
		return KEY_VIRTUAL_PLAY;
	case U1_KEY_STOP:
		return isDown ? KEY_VIRTUAL_REDRAW : KEY_VIRTUAL_ESC;
	case U1_KEY_POWER:
		return KEY_VIRTUAL_POWER;
	default :
		{
			if ((KEY_VIRTUAL_10 <= value) && (value <= KEY_VIRTUAL_16))
			{
				return value;
			}
			else
			{
				return 0;
			}
		}
	}
}

unsigned long CKeyLocal1U::PlaybkProc( bool isDown,unsigned long value)
{
	switch(value) 
	{
	case U1_KEY_1:
		return KEY_VIRTUAL_1;
	case U1_KEY_2:
		return KEY_VIRTUAL_2;
	case U1_KEY_3:
		return KEY_VIRTUAL_3;
	case U1_KEY_4:
		return KEY_VIRTUAL_4;
	case U1_KEY_5:
		return KEY_VIRTUAL_5;
	case U1_KEY_6:
		return KEY_VIRTUAL_6;
	case U1_KEY_7:
		return KEY_VIRTUAL_7;
	case U1_KEY_8:
		return KEY_VIRTUAL_8;
	case U1_KEY_9:
		return KEY_VIRTUAL_9;
	//case U1_KEY_DEC:
	//	return KEY_VIRTUAL_DEC;
	case U1_KEY_10:
		if(s_videoInputNum <=16)
		{
			return KEY_VIRTUAL_10_A;
		}
		else
		{
			return KEY_VIRTUAL_A_A;
		}
	case U1_KEY_UP:
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
	case U1_KEY_DOWN:
		{
			if (s_videoInputNum == 8)
			{
				return KEY_VIRTUAL_1X1;
			}
			else if (s_videoInputNum <= 16)
			{
				return KEY_VIRTUAL_2X2;
			}
			else
			{
				return KEY_VIRTUAL_3X3	;
			}
		}
	case U1_KEY_RIGHT:
		{
			if (s_videoInputNum <= 4)
			{
				return KEY_VIRTUAL_RIGHT;
			}
			else if (s_videoInputNum <= 9)
			{
				return KEY_VIRTUAL_3X3;
			}
			else if (s_videoInputNum <= 16)
			{
				return KEY_VIRTUAL_4X4;
			}
			else if(s_videoInputNum <= 25)
			{
				return KEY_VIRTUAL_5X5;
			}
			else
			{
				return KEY_VIRTUAL_6X6;
			}
		}
	case U1_KEY_LEFT:
		{
			if (s_videoInputNum <= 4)
			{
				return KEY_VIRTUAL_LEFT;
			}
			else if (s_videoInputNum <= 9)
			{
				return KEY_VIRTUAL_2X2;
			}
			else if (s_videoInputNum <= 16)
			{
				return KEY_VIRTUAL_3X3;
			}
			else
			{
				return KEY_VIRTUAL_4X4;
			}
		}
	case U1_KEY_REW:
		return KEY_VIRTUAL_REW;
	case U1_KEY_ENTER:
		return KEY_VIRTUAL_ENTER;
	case U1_KEY_FF:
		return KEY_VIRTUAL_FF;
	case U1_KEY_ADD:
		return isDown ? KEY_VIRTUAL_LCD_ON_OFF : KEY_VIRTUAL_HIDE;
	case U1_KEY_RECORD:
		return KEY_VIRTUAL_RECORD;
	case U1_KEY_SEARCH:
		return KEY_VIRTUAL_SEARCH;
	case U1_KEY_PLAY:
		return KEY_VIRTUAL_PLAY;
	case U1_KEY_STOP:
		return isDown ? KEY_VIRTUAL_REDRAW : KEY_VIRTUAL_ESC;
	case U1_KEY_POWER:
		return KEY_VIRTUAL_POWER;
	default :
		{
			if ((KEY_VIRTUAL_10 <= value) && (value <= KEY_VIRTUAL_16))
			{
				return value;
			}
			else
			{
				return 0;
			}
		}
	}
}

unsigned long CKeyLocal1U::PtzProc( bool isDown,unsigned long value)
{
	switch(value) 
	{
	case U1_KEY_1:
		return KEY_VIRTUAL_1;
	case U1_KEY_2:
		return KEY_VIRTUAL_2;
	case U1_KEY_3:
		return KEY_VIRTUAL_3;
	case U1_KEY_4:
		return KEY_VIRTUAL_4;
	case U1_KEY_5:
		return KEY_VIRTUAL_5;
	case U1_KEY_6:
		return KEY_VIRTUAL_6;
	case U1_KEY_7:
		return KEY_VIRTUAL_7;
	case U1_KEY_8:
		return KEY_VIRTUAL_8;
	case U1_KEY_9:
		return KEY_VIRTUAL_9;
	case U1_KEY_DEC:
		return KEY_VIRTUAL_DEC;
	case U1_KEY_10:
		if(s_videoInputNum <=16)
		{
			return KEY_VIRTUAL_10_A;
		}
		else
		{
			return KEY_VIRTUAL_A_A;
		}
	case U1_KEY_UP:
		return KEY_VIRTUAL_PTZ_UP;
	case U1_KEY_DOWN:
		return KEY_VIRTUAL_PTZ_DOWN;
	case U1_KEY_RIGHT:
		return KEY_VIRTUAL_PTZ_RIGHT;
	case U1_KEY_LEFT:
		return KEY_VIRTUAL_PTZ_LEFT;
	case U1_KEY_REW:
		return KEY_VIRTUAL_SPEED;
	case U1_KEY_ENTER:
		return KEY_VIRTUAL_ENTER;
	case U1_KEY_FF:
		return KEY_VIRTUAL_HIDE;
	case U1_KEY_ADD:
		return isDown ? KEY_VIRTUAL_LCD_ON_OFF : KEY_VIRTUAL_ADD;
	case U1_KEY_RECORD:
		return KEY_VIRTUAL_FOCUS;
	case U1_KEY_SEARCH:
		return KEY_VIRTUAL_ZOOM;
	case U1_KEY_PLAY:
		return KEY_VIRTUAL_IRIS;
	case U1_KEY_STOP:
		return isDown ? KEY_VIRTUAL_REDRAW : KEY_VIRTUAL_ESC;
	case U1_KEY_POWER:
		return KEY_VIRTUAL_POWER;
	default :
		{
			if ((KEY_VIRTUAL_10 <= value) && (value <= KEY_VIRTUAL_16))
			{
				return value;
			}
			else
			{
				return 0;
			}
		}
	}
}

unsigned long CKeyLocal1U::UIProc( bool isDown, unsigned long value)
{
	switch(value) 
	{
	case U1_KEY_1:
		return KEY_VIRTUAL_1;
	case U1_KEY_2:
		return KEY_VIRTUAL_2;
	case U1_KEY_3:
		return KEY_VIRTUAL_3;
	case U1_KEY_4:
		return KEY_VIRTUAL_4;
	case U1_KEY_5:
		return KEY_VIRTUAL_5;
	case U1_KEY_6:
		return KEY_VIRTUAL_6;
	case U1_KEY_7:
		return KEY_VIRTUAL_7;
	case U1_KEY_8:
		return KEY_VIRTUAL_8;
	case U1_KEY_9:
		return KEY_VIRTUAL_9;
	case U1_KEY_DEC:
		return KEY_VIRTUAL_DEC;
	case U1_KEY_10:
		return KEY_VIRTUAL_0;
	case U1_KEY_UP:
		return KEY_VIRTUAL_UP;
	case U1_KEY_DOWN:
		return KEY_VIRTUAL_DOWN;
	case U1_KEY_RIGHT:
		return KEY_VIRTUAL_RIGHT;
	case U1_KEY_LEFT:
		return KEY_VIRTUAL_LEFT;
	case U1_KEY_REW:
		return KEY_VIRTUAL_REW;
	case U1_KEY_ENTER:
		return KEY_VIRTUAL_ENTER;
	case U1_KEY_FF:
		return KEY_VIRTUAL_PTZ;
	case U1_KEY_ADD:
		return isDown ? KEY_VIRTUAL_LCD_ON_OFF : KEY_VIRTUAL_ADD;
	case U1_KEY_RECORD:
		return KEY_VIRTUAL_RECORD;
	case U1_KEY_SEARCH:
		return KEY_VIRTUAL_SEARCH;
	case U1_KEY_PLAY:
		return KEY_VIRTUAL_PLAY;
	case U1_KEY_STOP:
		return isDown ? KEY_VIRTUAL_REDRAW : KEY_VIRTUAL_ESC;
	case U1_KEY_POWER:
		return KEY_VIRTUAL_POWER;
	default :
		{
			if ((KEY_VIRTUAL_10 <= value) && (value <= KEY_VIRTUAL_16))
			{
				return value;
			}
			else
			{
				return 0;
			}
		}
	}
}

