#include "KeyLocal2U.h"
#include <stdio.h>


unsigned char CKeyLocal2U::s_videoInputNum=0;

CKeyLocal2U::CKeyLocal2U()
{
}

CKeyLocal2U::~CKeyLocal2U()
{

}

unsigned long CKeyLocal2U::TranKeyLocal(KEY_MODE mod, bool isDown, unsigned long value)
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

unsigned long CKeyLocal2U::FuncProc( bool isDown, unsigned long value)
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
	case U2_KEY_10:
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
	case U2_KEY_SINGLE:
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
	case U2_KEY_QUAD:
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
	case U2_KEY_NINE:
		{
			if(4>=s_videoInputNum)
			{
				//��·��Ƶ���룬 2UA��2UB������·��
				return KEY_VIRTUAL_ADD;
			}
			else if (s_videoInputNum <= 16)
			{
				//��·����ʮ��·��Ƶ����
				return KEY_VIRTUAL_3X3;
			}
			else
			{
				return KEY_VIRTUAL_4X4;
			}
		}
	case U2_KEY_SIXTEEN:
		{
			if(4>=s_videoInputNum)
			{
				//��·��Ƶ���룬 2UA��2UB������·��
				return KEY_VIRTUAL_DEC;
			}
			else if(9>=s_videoInputNum)
			{
				//8·��Ƶ����
				return KEY_VIRTUAL_AUTO;
			}
			else if (s_videoInputNum <= 12)
			{
				//ʮ��·��Ƶ����
				return KEY_VIRTUAL_4X4;
			}
			else if (s_videoInputNum <= 16)
			{
				//ʮ��·��Ƶ����
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
	case U2_KEY_INFO:
		return KEY_VIRTUAL_INFO;
	case U2_KEY_AUDIO:
		return KEY_VIRTUAL_AUDIO;
	case U2_KEY_LEFT:
		return KEY_VIRTUAL_LEFT;
	case U2_KEY_RIGHT:
		return KEY_VIRTUAL_RIGHT;
	case U2_KEY_DOWN:
		return KEY_VIRTUAL_DOWN;
	case U2_KEY_UP:
		return KEY_VIRTUAL_UP;
	//case U2_KEY_LEFT:
	//	return KEY_VIRTUAL_3X3;
	//case U2_KEY_RIGHT:
	//	return KEY_VIRTUAL_4X4;
	//case U2_KEY_DOWN:
	//	return KEY_VIRTUAL_2X2;
	//case U2_KEY_UP:
	//	return KEY_VIRTUAL_1X1;

	case U2_KEY_ENTER:
		return KEY_VIRTUAL_ENTER;
	case U2_KEY_PTZ:
		return KEY_VIRTUAL_PTZ;
	case U2_KEY_BACKUP:
		return KEY_VIRTUAL_BACKUP;
	case U2_KEY_MENU:
		return KEY_VIRTUAL_MENU;
	case U2_KEY_RECORD:
		return KEY_VIRTUAL_RECORD;
	case U2_KEY_SEARCH:
		return KEY_VIRTUAL_SEARCH;
	case U2_KEY_PLAY:
		return KEY_VIRTUAL_PLAY;
	case U2_KEY_REW:
		return KEY_VIRTUAL_REW;
	case U2_KEY_FF:
		return KEY_VIRTUAL_FF;
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

unsigned long CKeyLocal2U::PlaybkProc( bool isDown,unsigned long value)
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
	case U2_KEY_10:
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
	case U2_KEY_SINGLE:
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
	case U2_KEY_QUAD:
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
	case U2_KEY_NINE:
		{
			if(4>=s_videoInputNum)
			{
				//��·��Ƶ���룬 2UA��2UB������·��
				return KEY_VIRTUAL_ADD;
			}
			else if (s_videoInputNum <= 16)
			{
				//��·����ʮ��·��Ƶ����
				return KEY_VIRTUAL_3X3;
			}
			else
			{
				return KEY_VIRTUAL_4X4;
			}
		}
	case U2_KEY_SIXTEEN:
		{
			if(4>=s_videoInputNum)
			{
				//��·��Ƶ���룬 2UA��2UB������·��
				return KEY_VIRTUAL_DEC;
			}
			else if(9>=s_videoInputNum)
			{
				//8·��Ƶ����
				return KEY_VIRTUAL_AUTO;
			}
			else if (s_videoInputNum <= 16)
			{
				//ʮ��·��Ƶ����
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
	case U2_KEY_INFO:
		return KEY_VIRTUAL_INFO;
	case U2_KEY_AUDIO:
		return KEY_VIRTUAL_AUDIO;
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
	case U2_KEY_PTZ:
		return KEY_VIRTUAL_PTZ;
	case U2_KEY_BACKUP:
		return KEY_VIRTUAL_BACKUP;
	case U2_KEY_MENU:
		//PALYBACK ʱ��Ϊ���غ���ʾ�Ի����� zhl 2010-03-05
		return KEY_VIRTUAL_HIDE;
	case U2_KEY_RECORD:
		return KEY_VIRTUAL_RECORD;
	case U2_KEY_SEARCH:
		return KEY_VIRTUAL_SEARCH;
	case U2_KEY_PLAY:
		return KEY_VIRTUAL_PLAY;
	case U2_KEY_REW:
		return KEY_VIRTUAL_REW;
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

unsigned long CKeyLocal2U::PtzProc( bool isDown,unsigned long value)
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
	case U2_KEY_10:
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
	case U2_KEY_SINGLE:
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
	case U2_KEY_QUAD:
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
	case U2_KEY_NINE:		
		return KEY_VIRTUAL_ADD;
	case U2_KEY_SIXTEEN:
		return KEY_VIRTUAL_DEC;
	case U2_KEY_INFO:
		return KEY_VIRTUAL_INFO;
	case U2_KEY_AUDIO:
		return KEY_VIRTUAL_AUDIO;
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
	case U2_KEY_PTZ:
		return KEY_VIRTUAL_HIDE;
	case U2_KEY_BACKUP:
		return KEY_VIRTUAL_BACKUP;
	case U2_KEY_MENU:
		return KEY_VIRTUAL_MENU;
	case U2_KEY_RECORD:
		return KEY_VIRTUAL_ZOOM_ADD;
	case U2_KEY_SEARCH:
		return KEY_VIRTUAL_ZOOM_DEC;
	case U2_KEY_PLAY:
		return KEY_VIRTUAL_FOCUS_ADD;
	case U2_KEY_REW:
		return KEY_VIRTUAL_FOCUS_DEC;
	case U2_KEY_FF:
		return KEY_VIRTUAL_FF;
	case U2_KEY_STOP:
		return KEY_VIRTUAL_ESC;
	//case U2_KEY_STOP:
	//	return KEY_VIRTUAL_IRIS_DEC;
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

unsigned long CKeyLocal2U::UIProc( bool isDown, unsigned long value)
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
	case U2_KEY_10:
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
	case U2_KEY_SINGLE:
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
	case U2_KEY_QUAD:
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
	case U2_KEY_NINE:
		return KEY_VIRTUAL_ADD;
	case U2_KEY_SIXTEEN:
		return KEY_VIRTUAL_DEC;
	case U2_KEY_INFO:
		return KEY_VIRTUAL_INFO;
	case U2_KEY_AUDIO:
		return KEY_VIRTUAL_AUDIO;
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
	case U2_KEY_PTZ:
		return KEY_VIRTUAL_PTZ;
	case U2_KEY_BACKUP:
		return KEY_VIRTUAL_BACKUP;
	case U2_KEY_MENU:
		return KEY_VIRTUAL_MENU;
	case U2_KEY_RECORD:
		return KEY_VIRTUAL_RECORD;
	case U2_KEY_SEARCH:
		return KEY_VIRTUAL_SEARCH;
	case U2_KEY_PLAY:
		return KEY_VIRTUAL_PLAY;
	case U2_KEY_REW:
		return KEY_VIRTUAL_REW;
	case U2_KEY_FF:
		return KEY_VIRTUAL_FF;
	//case U2_KEY_STOP:
	//	return KEY_VIRTUAL_STOP;
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

