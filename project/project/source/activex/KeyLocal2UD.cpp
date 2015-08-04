#include "KeyLocal2UD.h"
#include <stdio.h>


unsigned char CKeyLocal2UD::s_videoInputNum=0;

CKeyLocal2UD::CKeyLocal2UD()
{
}

CKeyLocal2UD::~CKeyLocal2UD()
{

}

unsigned long CKeyLocal2UD::TranKeyLocal(KEY_MODE mod, bool isDown, unsigned long value)
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

unsigned long CKeyLocal2UD::FuncProc( bool isDown, unsigned long value)
{
	switch(value) 
	{
	case U2_KEY_0:
		{
			if (s_videoInputNum > 16)
			{
				return KEY_VIRTUAL_A_A;
			}
			else
			{
				return KEY_VIRTUAL_10_A;
			}
		}		
	case U2_KEY_1:
		return KEY_VIRTUAL_1;
	case U2_KEY_2:
		return KEY_VIRTUAL_2;
	case U2_KEY_3:
		return KEY_VIRTUAL_3;
	case U2_KEY_4:
		return KEY_VIRTUAL_4;
	case U2_KEY_5:
		return KEY_VIRTUAL_5;
	case U2_KEY_6:
		return KEY_VIRTUAL_6;
	case U2_KEY_7:
		return KEY_VIRTUAL_7;
	case U2_KEY_8:
		return KEY_VIRTUAL_8;
	case U2_KEY_9:
		return KEY_VIRTUAL_9;

	case U2_KEY_AUDIO:
		return KEY_VIRTUAL_AUDIO;
	case U2_KEY_PTZ:
		return KEY_VIRTUAL_PTZ;

	case U2_KEY_UP:
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
	case U2_KEY_DOWN:
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
	case U2_KEY_LEFT:
		{
			if(s_videoInputNum <= 4)
			{
				//四路视频输入
				return KEY_VIRTUAL_LEFT;
			}
			else if (s_videoInputNum <= 16)
			{
				//八路或者十六路视频输入
				return KEY_VIRTUAL_3X3;
			}
			else
			{
				return KEY_VIRTUAL_4X4;
			}
		}
	case U2_KEY_RIGHT:
		{
			if(s_videoInputNum <= 4)
			{
				//四路视频输入
				return KEY_VIRTUAL_RIGHT;
			}
			else if(s_videoInputNum <= 9)
			{
				//8路视频输入
				return KEY_VIRTUAL_RIGHT;
			}
			else if (s_videoInputNum <= 12)
			{
				//十六路视频输入
				return KEY_VIRTUAL_4X4;
			}
			else if (s_videoInputNum <= 16)
			{
				//十六路视频输入
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
	case U2_KEY_ENTER:
		return KEY_VIRTUAL_ENTER;

	case U2_KEY_MENU:
		return KEY_VIRTUAL_MENU;
	case U2_KEY_INFO:
		return KEY_VIRTUAL_INFO;
	case U2_KEY_BACKUP:
		return KEY_VIRTUAL_BACKUP;

	case U2_KEY_SEARCH:
		return KEY_VIRTUAL_SEARCH;
	case U2_KEY_REW:
		return KEY_VIRTUAL_REW;
	case U2_KEY_FF:
		return KEY_VIRTUAL_FF;

	case U2_KEY_RECORD:
		return KEY_VIRTUAL_RECORD;
	case U2_KEY_PLAY:
		return KEY_VIRTUAL_PLAY;
	case U2_KEY_STOP:
		return isDown ? KEY_VIRTUAL_REDRAW : KEY_VIRTUAL_ESC;;

	case U2_KEY_POWER:
		return KEY_VIRTUAL_POWER;

		//case U2_FLY_LEFT_1:
		//	return KEY_VIRTUAL_FLY_LEFT_1;
	case U2_FLY_LEFT_2:
		return KEY_VIRTUAL_FLY_LEFT_2;
		//case U2_FLY_LEFT_3:
		//return KEY_VIRTUAL_FLY_LEFT_3;
	case U2_FLY_LEFT_4:
		return KEY_VIRTUAL_FLY_LEFT_4;
		//case U2_FLY_LEFT_5:
		//return KEY_VIRTUAL_FLY_LEFT_5;
	case U2_FLY_LEFT_6:
		return KEY_VIRTUAL_FLY_LEFT_6;
		//case U2_FLY_LEFT_7:
		//return KEY_VIRTUAL_FLY_LEFT_7;
	case U2_FLY_LEFT_8:
		return KEY_VIRTUAL_FLY_LEFT_8;
	
	case U2_FLY_MEDIUM:
		return KEY_VIRTUAL_FLY_MEDIUM;
	
		//case U2_FLY_RIGHT_1:
		//return KEY_VIRTUAL_FLY_RIGHT_1;
	case U2_FLY_RIGHT_2:
		return KEY_VIRTUAL_FLY_RIGHT_2;
		//case U2_FLY_RIGHT_3:
		//return KEY_VIRTUAL_FLY_RIGHT_3;
	case U2_FLY_RIGHT_4:
		return KEY_VIRTUAL_FLY_RIGHT_4;
		//case U2_FLY_RIGHT_5:
		//return KEY_VIRTUAL_FLY_RIGHT_5;
	case U2_FLY_RIGHT_6:
		return KEY_VIRTUAL_FLY_RIGHT_6;
		//case U2_FLY_RIGHT_7:
		//return KEY_VIRTUAL_FLY_RIGHT_7;

	case U2_FLY_IN_LEFT:
		return KEY_VIRTUAL_LEFT;
	case U2_FLY_IN_RIGHT:
		return KEY_VIRTUAL_RIGHT;

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

unsigned long CKeyLocal2UD::PlaybkProc( bool isDown,unsigned long value)
{
	switch(value) 
	{
	case U2_KEY_0:
		{
			if (s_videoInputNum > 16)
			{
				return KEY_VIRTUAL_A_A;
			}
			else
			{
				return KEY_VIRTUAL_10_A;
			}
		}		
	case U2_KEY_1:
		return KEY_VIRTUAL_1;
	case U2_KEY_2:
		return KEY_VIRTUAL_2;
	case U2_KEY_3:
		return KEY_VIRTUAL_3;
	case U2_KEY_4:
		return KEY_VIRTUAL_4;
	case U2_KEY_5:
		return KEY_VIRTUAL_5;
	case U2_KEY_6:
		return KEY_VIRTUAL_6;
	case U2_KEY_7:
		return KEY_VIRTUAL_7;
	case U2_KEY_8:
		return KEY_VIRTUAL_8;
	case U2_KEY_9:
		return KEY_VIRTUAL_9;

	case U2_KEY_AUDIO:
		return KEY_VIRTUAL_ADD;
	case U2_KEY_PTZ:
		return KEY_VIRTUAL_DEC;

	case U2_KEY_UP:
		{
			if (s_videoInputNum <= 16)
			{
				//16路以上是单分割
				return KEY_VIRTUAL_1X1;
			}
			else
			{
				//16路以上是4分割
				return KEY_VIRTUAL_2X2;
			}
		}		
	case U2_KEY_DOWN:
		{
			if (s_videoInputNum <= 16)
			{
				//16路以下是4分割
				return KEY_VIRTUAL_2X2;	
			}
			else
			{
				//16路以上是9分割
				return KEY_VIRTUAL_3X3;
			}
		}		
	case U2_KEY_LEFT:
		{
			if(4>=s_videoInputNum)
			{
				//四路视频输入
				return KEY_VIRTUAL_LEFT;
			}
			else if (s_videoInputNum <= 16)
			{
				//八路或者十六路视频输入
				return KEY_VIRTUAL_3X3;
			}
			else
			{
				//16路以上是16分割
				return KEY_VIRTUAL_4X4;
			}
		}
	case U2_KEY_RIGHT:
		{
			if(4>=s_videoInputNum)
			{
				//四路视频输入
				return KEY_VIRTUAL_RIGHT;
			}
			else if(9>=s_videoInputNum)
			{
				//8路视频输入
				return KEY_VIRTUAL_RIGHT;
			}
			else if (16 >= s_videoInputNum)
			{
				//十六路视频输入
				return KEY_VIRTUAL_4X4;
			}
			else if (25 >= s_videoInputNum)
			{
				//24路时，25分割
				return KEY_VIRTUAL_5X5;
			}
			else
			{
				//32路时，36分割
				return KEY_VIRTUAL_6X6;
			}
		}
	case U2_KEY_ENTER:
		return KEY_VIRTUAL_ENTER;

	case U2_KEY_MENU:
		//PALYBACK 时作为隐藏和显示对话框用 zhl 2010-03-05
		return KEY_VIRTUAL_HIDE;
	case U2_KEY_INFO:
		return KEY_VIRTUAL_INFO;
	case U2_KEY_BACKUP:
		return KEY_VIRTUAL_BACKUP;

	case U2_KEY_SEARCH:
		return KEY_VIRTUAL_SEARCH;
	case U2_KEY_REW:
		return KEY_VIRTUAL_REW;
	case U2_KEY_FF:
		return KEY_VIRTUAL_FF;


	case U2_KEY_RECORD:
		return KEY_VIRTUAL_RECORD;
	case U2_KEY_PLAY:
		return KEY_VIRTUAL_PLAY;
	case U2_KEY_STOP:
		return KEY_VIRTUAL_ESC;

	case U2_KEY_POWER:
		return KEY_VIRTUAL_POWER;

	case U2_FLY_LEFT_1:
		return KEY_VIRTUAL_FLY_LEFT_1;
	case U2_FLY_LEFT_2:
		return KEY_VIRTUAL_FLY_LEFT_2;
	case U2_FLY_LEFT_3:
		return KEY_VIRTUAL_FLY_LEFT_3;
	case U2_FLY_LEFT_4:
		return KEY_VIRTUAL_FLY_LEFT_4;
	case U2_FLY_LEFT_5:
		return KEY_VIRTUAL_FLY_LEFT_5;
	case U2_FLY_LEFT_6:
		return KEY_VIRTUAL_FLY_LEFT_6;
	case U2_FLY_LEFT_7:
		return KEY_VIRTUAL_FLY_LEFT_7;
	case U2_FLY_LEFT_8:
		return KEY_VIRTUAL_FLY_LEFT_8;

	case U2_FLY_MEDIUM:
		return KEY_VIRTUAL_FLY_MEDIUM;

	case U2_FLY_RIGHT_1:
		return KEY_VIRTUAL_FLY_RIGHT_1;
	case U2_FLY_RIGHT_2:
		return KEY_VIRTUAL_FLY_RIGHT_2;
	case U2_FLY_RIGHT_3:
		return KEY_VIRTUAL_FLY_RIGHT_3;
	case U2_FLY_RIGHT_4:
		return KEY_VIRTUAL_FLY_RIGHT_4;
	case U2_FLY_RIGHT_5:
		return KEY_VIRTUAL_FLY_RIGHT_5;
	case U2_FLY_RIGHT_6:
		return KEY_VIRTUAL_FLY_RIGHT_6;
	case U2_FLY_RIGHT_7:
		return KEY_VIRTUAL_FLY_RIGHT_7;

	case U2_FLY_IN_LEFT:
		return KEY_VIRTUAL_FLFLY_IN_LEFT;
	case U2_FLY_IN_RIGHT:
		return KEY_VIRTUAL_FLFLY_IN_RIGHT;

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

unsigned long CKeyLocal2UD::PtzProc( bool isDown,unsigned long value)
{
	switch(value) 
	{
	case U2_KEY_0:
		{
			if (s_videoInputNum > 16)
			{
				return KEY_VIRTUAL_A_A;
			}
			else
			{
				return KEY_VIRTUAL_10_A;
			}
		}	
	case U2_KEY_1:
		return KEY_VIRTUAL_1;
	case U2_KEY_2:
		return KEY_VIRTUAL_2;
	case U2_KEY_3:
		return KEY_VIRTUAL_3;
	case U2_KEY_4:
		return KEY_VIRTUAL_4;
	case U2_KEY_5:
		return KEY_VIRTUAL_5;
	case U2_KEY_6:
		return KEY_VIRTUAL_6;
	case U2_KEY_7:
		return KEY_VIRTUAL_7;
	case U2_KEY_8:
		return KEY_VIRTUAL_8;
	case U2_KEY_9:
		return KEY_VIRTUAL_9;

	case U2_KEY_AUDIO:
		return KEY_VIRTUAL_AUDIO;
	case U2_KEY_PTZ:
		return KEY_VIRTUAL_HIDE;

	case U2_KEY_LEFT:
		return KEY_VIRTUAL_PTZ_LEFT;
	case U2_KEY_RIGHT:
		return KEY_VIRTUAL_PTZ_RIGHT;
	case U2_KEY_DOWN:
		return KEY_VIRTUAL_PTZ_DOWN;
	case U2_KEY_UP:
		return KEY_VIRTUAL_PTZ_UP;
	case U2_KEY_ENTER:
		return KEY_VIRTUAL_ENTER;

	case U2_KEY_MENU:
		return KEY_VIRTUAL_MENU;
	case U2_KEY_INFO:
		return KEY_VIRTUAL_INFO;
	case U2_KEY_BACKUP:
		return KEY_VIRTUAL_BACKUP;

	case U2_KEY_SEARCH:
		return KEY_VIRTUAL_ZOOM_DEC;
	case U2_KEY_RECORD:
		return KEY_VIRTUAL_ZOOM_ADD;

	case U2_KEY_REW:
		return KEY_VIRTUAL_FOCUS_DEC;
	case U2_KEY_PLAY:
		return KEY_VIRTUAL_FOCUS_ADD;
	
	case U2_KEY_FF:
		return KEY_VIRTUAL_FF;
	case U2_KEY_STOP:
		return KEY_VIRTUAL_ESC;

	case U2_KEY_POWER:
		return KEY_VIRTUAL_POWER;

	case U2_FLY_LEFT_1:
		return KEY_VIRTUAL_FLY_LEFT_1;
	case U2_FLY_LEFT_2:
		return KEY_VIRTUAL_FLY_LEFT_2;
	case U2_FLY_LEFT_3:
		return KEY_VIRTUAL_FLY_LEFT_3;
	case U2_FLY_LEFT_4:
		return KEY_VIRTUAL_FLY_LEFT_4;
	case U2_FLY_LEFT_5:
		return KEY_VIRTUAL_FLY_LEFT_5;
	case U2_FLY_LEFT_6:
		return KEY_VIRTUAL_FLY_LEFT_6;
	case U2_FLY_LEFT_7:
		return KEY_VIRTUAL_FLY_LEFT_7;
	case U2_FLY_LEFT_8:
		return KEY_VIRTUAL_FLY_LEFT_8;

	case U2_FLY_MEDIUM:
		return KEY_VIRTUAL_FLY_MEDIUM;

	case U2_FLY_RIGHT_1:
		return KEY_VIRTUAL_FLY_RIGHT_1;
	case U2_FLY_RIGHT_2:
		return KEY_VIRTUAL_FLY_RIGHT_2;
	case U2_FLY_RIGHT_3:
		return KEY_VIRTUAL_FLY_RIGHT_3;
	case U2_FLY_RIGHT_4:
		return KEY_VIRTUAL_FLY_RIGHT_4;
	case U2_FLY_RIGHT_5:
		return KEY_VIRTUAL_FLY_RIGHT_5;
	case U2_FLY_RIGHT_6:
		return KEY_VIRTUAL_FLY_RIGHT_6;
	case U2_FLY_RIGHT_7:
		return KEY_VIRTUAL_FLY_RIGHT_7;

	case U2_FLY_IN_LEFT:
		return KEY_VIRTUAL_FLFLY_IN_LEFT;
	case U2_FLY_IN_RIGHT:
		return KEY_VIRTUAL_FLFLY_IN_RIGHT;

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

unsigned long CKeyLocal2UD::UIProc( bool isDown, unsigned long value)
{
	switch(value) 
	{
	case U2_KEY_0:
		return KEY_VIRTUAL_0;
	case U2_KEY_1:
		return KEY_VIRTUAL_1;
	case U2_KEY_2:
		return KEY_VIRTUAL_2;
	case U2_KEY_3:
		return KEY_VIRTUAL_3;
	case U2_KEY_4:
		return KEY_VIRTUAL_4;
	case U2_KEY_5:
		return KEY_VIRTUAL_5;
	case U2_KEY_6:
		return KEY_VIRTUAL_6;
	case U2_KEY_7:
		return KEY_VIRTUAL_7;
	case U2_KEY_8:
		return KEY_VIRTUAL_8;
	case U2_KEY_9:
		return KEY_VIRTUAL_9;

	case U2_KEY_AUDIO:
		return KEY_VIRTUAL_ADD;
	case U2_KEY_PTZ:
		return KEY_VIRTUAL_DEC;

	case U2_KEY_LEFT:
		return KEY_VIRTUAL_LEFT;
	case U2_KEY_RIGHT:
		return KEY_VIRTUAL_RIGHT;
	case U2_KEY_DOWN:
		return KEY_VIRTUAL_DOWN;
	case U2_KEY_UP:
		return KEY_VIRTUAL_UP;
	case U2_KEY_ENTER:
		return KEY_VIRTUAL_ENTER;

	case U2_KEY_MENU:
		return KEY_VIRTUAL_MENU;
	case U2_KEY_INFO:
		return KEY_VIRTUAL_INFO;
	case U2_KEY_BACKUP:
		return KEY_VIRTUAL_BACKUP;

	case U2_KEY_SEARCH:
		return KEY_VIRTUAL_SEARCH;
	case U2_KEY_REW:
		return KEY_VIRTUAL_REW;
	case U2_KEY_FF:
		return KEY_VIRTUAL_FF;

	case U2_KEY_RECORD:
		return KEY_VIRTUAL_RECORD;
	case U2_KEY_PLAY:
		return KEY_VIRTUAL_PLAY;
	case U2_KEY_STOP:
		return isDown ? KEY_VIRTUAL_REDRAW : KEY_VIRTUAL_ESC;;

	case U2_KEY_POWER:
		return KEY_VIRTUAL_POWER;

	case U2_FLY_LEFT_1:
		return KEY_VIRTUAL_FLY_LEFT_1;
	case U2_FLY_LEFT_2:
		return KEY_VIRTUAL_FLY_LEFT_2;
	case U2_FLY_LEFT_3:
		return KEY_VIRTUAL_LEFT;
	case U2_FLY_LEFT_4:
		return KEY_VIRTUAL_FLY_LEFT_4;
	case U2_FLY_LEFT_5:
		return KEY_VIRTUAL_FLY_LEFT_5;
	case U2_FLY_LEFT_6:
		return KEY_VIRTUAL_FLY_LEFT_6;
	case U2_FLY_LEFT_7:
		return KEY_VIRTUAL_FLY_LEFT_7;
	case U2_FLY_LEFT_8:
		return KEY_VIRTUAL_FLY_LEFT_8;

	case U2_FLY_MEDIUM:
		return KEY_VIRTUAL_FLY_MEDIUM;

	case U2_FLY_RIGHT_1:
		return KEY_VIRTUAL_FLY_RIGHT_1;
	case U2_FLY_RIGHT_2:
		return KEY_VIRTUAL_FLY_RIGHT_2;
	case U2_FLY_RIGHT_3:
		return KEY_VIRTUAL_RIGHT;
	case U2_FLY_RIGHT_4:
		return KEY_VIRTUAL_FLY_RIGHT_4;
	case U2_FLY_RIGHT_5:
		return KEY_VIRTUAL_FLY_RIGHT_5;
	case U2_FLY_RIGHT_6:
		return KEY_VIRTUAL_FLY_RIGHT_6;
	case U2_FLY_RIGHT_7:
		return KEY_VIRTUAL_FLY_RIGHT_7;

	case U2_FLY_IN_LEFT:
		return KEY_VIRTUAL_UP;
	case U2_FLY_IN_RIGHT:
		return KEY_VIRTUAL_DOWN;
	
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

