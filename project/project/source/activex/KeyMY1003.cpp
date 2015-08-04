/*****************************************************************
 Copyright (C),2009-2012, tongwei video Tech. Co., Ltd.

 FileName:

 Author: TVT-ZHL-146  Version:	Date:

 Description:

 Version:

 Function List:

 History:
 
 	<author>  <time>  <version> <desc>
******************************************************************/


#include "KeyMY1003.h"
#include "LocalKeyValue.h"
#include "MY1003KeyValue.h"
#include <stdio.h>

unsigned char CKeyMY1003::s_videoInputNum = 0;

//public:
CKeyMY1003::CKeyMY1003()
{

}
CKeyMY1003::~CKeyMY1003()
{

}

//protected:
unsigned long CKeyMY1003::TranKeyMY1003(KEY_MODE mod, unsigned long value)
{
	//printf("%s():%s:%d\n", __FUNCTION__, __FILE__, __LINE__);

	switch(mod) 
	{
	case KEY_MODE_PTZ:
		return PtzProc( value );

	case KEY_MODE_PLAYBACK:
		return PlaybkProc( value );

	case KEY_MODE_FUNC:
		return FuncProc( value );

	case KEY_MODE_UI:
		return UIProc( value );

	default:
		return 0;
	}	
}

//private:
unsigned long CKeyMY1003::UIProc( unsigned long value )
{
	//������ֵ��һ�ֽ��Ǽ���,�ڶ��ֽ��ǲ����Ĳ���
	unsigned long valueTemp = (value & 0x000000ff);
	unsigned long parameter = ( value >> 8 );

	//printf( "%s():%s:%d, the MY1003 keyvalue=0x%08x, parameter=0x%08x\n", __FUNCTION__, __FILE__, __LINE__, valueTemp, parameter );	

	if( BUTTN_UP == parameter )
	{
		//ҡ��ֹͣʱ�����������벻����
		return 0;
	}
		switch(valueTemp)
	{
		//��·����
		case MY1003_KEY_0:
			return KEY_VIRTUAL_0;

		case MY1003_KEY_1:
			return KEY_VIRTUAL_1;
		
		case MY1003_KEY_2:
			return KEY_VIRTUAL_2;
	
		case MY1003_KEY_3:
			return KEY_VIRTUAL_3;
	
		case MY1003_KEY_4:
			return KEY_VIRTUAL_4;
	
		case MY1003_KEY_5:
			return KEY_VIRTUAL_5;
	
		case MY1003_KEY_6:
			return KEY_VIRTUAL_6;
	
		case MY1003_KEY_7:
			return KEY_VIRTUAL_7;
	
		case MY1003_KEY_8:
			return KEY_VIRTUAL_8;
	
		case MY1003_KEY_9:
			return KEY_VIRTUAL_9;

		case MY1003_KEY_ESC:
			return KEY_VIRTUAL_ESC;
			
		case MY1003_KEY_ENTER:
			return KEY_VIRTUAL_ENTER;

	//��·����
		case MY1003_KEY_SINGLE:
			{
					return KEY_VIRTUAL_1X1;
			}
		case  MY1003_KEY_QUAD:
			{
					return KEY_VIRTUAL_2X2;
			}
		case MY1003_KEY_NINE:
			{
				if (s_videoInputNum <= 5)
				{
					return false;
				}
				else
				{
					return KEY_VIRTUAL_3X3;
				}
			}
		case  MY1003_KEY_SIXTEEN:
			{
				if (s_videoInputNum <= 9)
				{
					return false;
				}
				else
				{
					return KEY_VIRTUAL_4X4;
				}
			}

		case MY1003_KEY_PTZ:
			return KEY_VIRTUAL_PTZ;
			
		case  MY1003_KEY_BACKUP:
			return KEY_VIRTUAL_BACKUP;
			
		case  MY1003_KEY_MENU:
			return KEY_VIRTUAL_MENU;


		case  MY1003_KEY_SEQ:
			return KEY_VIRTUAL_SEQ;

		case  MY1003_KEY_INFO:
			return KEY_VIRTUAL_INFO;
			
		case MY1003_KEY_SEARCH:
			return KEY_VIRTUAL_SEARCH;
			
		case MY1003_KEY_AUDIO:
			return KEY_VIRTUAL_AUDIO;
			
		case  MY1003_KEY_ADD:
			return KEY_VIRTUAL_ADD;
			
 		case MY1003_KEY_RECORD:
			return KEY_VIRTUAL_RECORD;
			
		case MY1003_KEY_POWER:			//�ػ�
			return KEY_VIRTUAL_POWER;
			
		case MY1003_KEY_DEC:
			return KEY_VIRTUAL_DEC;
			
 		case MY1003_KEY_PLAY:
			return KEY_VIRTUAL_PLAY;

		//��·��������UIģʽ����·�����������ã�ҡ�����������������������ƶ�

		//case MY1003_KEY_PRESET:
			//return KEY_VIRTUAL_SF;

		//case MY1003_KEY_FOCUS_ADD:
		//	return KEY_VIRTUAL_FOCUS_ADD;

		//case MY1003_KEY_FOCUS_DEC:
		//	return KEY_VIRTUAL_FOCUS_DEC;
			
		//case MY1003_KEY_DEL:
		//	return KEY_VIRTUAL_DEL;

		//case MY1003_KEY_ZOOM_ADD:
		//	return KEY_VIRTUAL_ZOOM_ADD;
			
		//case MY1003_KEY_ZOOM_DEC:
		//	return KEY_VIRTUAL_ZOOM_DEC;
			
		//case MY1003_KEY_SHOT:
		//	return KEY_VIRTUAL_SHOT;

		//case MY1003_KEY_IRIS_ADD:
		//	return KEY_VIRTUAL_IRIS_ADD;
			
		//case MY1003_KEY_IRIS_DEC:
		//	return KEY_VIRTUAL_IRIS_DEC;
			
		//case MY1003_KEY_AUTO:
		//	return KEY_VIRTUAL_AUTO;

		//case MY1003_KEY_WIPER:
			//return KEY_VIRTUAL_WIPER;
			
		//case MY1003_KEY_LIGHT:
			//return KEY_VIRTUAL_LIGHT;


//����ҡ�˲���,��UIģʽ��ҡ��ʵ���ƶ�(��������)���㹦��,��ʱ��ֻ���������0�ļ�ֵ
		case MY1003_KEY_UP:
			return KEY_VIRTUAL_UP;
			
		case MY1003_KEY_DOWN:
			return KEY_VIRTUAL_DOWN;
			
		case  MY1003_KEY_LEFT:
			return KEY_VIRTUAL_LEFT;
			
		case MY1003_KEY_RIGHT:
			return KEY_VIRTUAL_RIGHT;
		
		default :
			break;
	
	}
	return 0;
}
unsigned long CKeyMY1003::PtzProc( unsigned long value )
{
	unsigned long valueTemp = ( value & 0x000000ff );
	unsigned long parameter = ( value >> 8 );

	//zhl 2010-03-09,��PTZģʽץȡ���İ������������ת��Ϊ��ֹ̨ͣ���������Ԥ�õ㡢����Ԥ�õ㡢���Ԥ�õ�1ʱparameterҲΪ1����ʱ���ܷ���ֹͣ��̨����
	if( ( BUTTN_UP == parameter ) && ( MY1003_KEY_SHOW_PRESET != valueTemp ) && ( MY1003_KEY_SET_PRESET != valueTemp ) && ( MY1003_KEY_DEL_PRESET != valueTemp ) )
	{
		//printf("%s():%s:%d, will stop the ptz\n", __FUNCTION__, __FILE__, __LINE__);
		return KEY_VIRTUAL_PTZ_STOP;
	}

	switch(valueTemp)
	{
		case MY1003_KEY_0:
			{
				if (s_videoInputNum <= 16)
				{
					return KEY_VIRTUAL_10_A;
				}
				else
				{
					return KEY_VIRTUAL_A_A;
				}
			}
		case MY1003_KEY_1:
			return KEY_VIRTUAL_1;
		
		case MY1003_KEY_2:
			return KEY_VIRTUAL_2;
	
		case MY1003_KEY_3:
			return KEY_VIRTUAL_3;
	
		case MY1003_KEY_4:
			return KEY_VIRTUAL_4;
	
		case MY1003_KEY_5:
			return KEY_VIRTUAL_5;
	
		case MY1003_KEY_6:
			return KEY_VIRTUAL_6;
	
		case MY1003_KEY_7:
			return KEY_VIRTUAL_7;
	
		case MY1003_KEY_8:
			return KEY_VIRTUAL_8;
	
		case MY1003_KEY_9:
			return KEY_VIRTUAL_9;

		case MY1003_KEY_POWER:
			return KEY_VIRTUAL_POWER;

		case  MY1003_KEY_ADD:
			return KEY_VIRTUAL_ADD;

		case  MY1003_KEY_DEC:
			return KEY_VIRTUAL_DEC;

		case  MY1003_KEY_MENU:
			return KEY_VIRTUAL_MENU;

		case MY1003_KEY_PTZ:
			return KEY_VIRTUAL_HIDE;
			
		case MY1003_KEY_PTZ_UP:
			return KEY_VIRTUAL_PTZ_UP;

		case MY1003_KEY_PTZ_DOWN:
			return KEY_VIRTUAL_PTZ_DOWN;
			
		case MY1003_KEY_PTZ_LEFT:
			return KEY_VIRTUAL_PTZ_LEFT;

		case MY1003_KEY_PTZ_RIGHT:
			return KEY_VIRTUAL_PTZ_RIGHT;

		case MY1003_KEY_PTZ_LEFTUP:
			return KEY_VIRTUAL_PTZ_LEFTUP;

		case MY1003_KEY_PTZ_LEFTDOWN:
			return KEY_VIRTUAL_PTZ_LEFTDOWN;

		case MY1003_KEY_PTZ_RIGHTUP:
			return KEY_VIRTUAL_PTZ_RIGHTUP;

		case MY1003_KEY_PTZ_RIGHTDOWN:
			return KEY_VIRTUAL_PTZ_RIGHTDOWN;

		case MY1003_KEY_PTZ_STOP:
			return KEY_VIRTUAL_PTZ_STOP;

		case MY1003_KEY_PTZ_SPEED_DEC:
			return KEY_VIRTUAL_SPEED_DEC;
			
		case MY1003_KEY_PTZ_SPEED_ADD:
			return KEY_VIRTUAL_SPEED_ADD;
			
		case MY1003_KEY_ESC:
			return KEY_VIRTUAL_ESC;

		case MY1003_KEY_ENTER:
			return KEY_VIRTUAL_ENTER;

//����Ԥ�õ�
		case MY1003_KEY_SET_PRESET:
			return KEY_VIRTUAL_PTZ_SET_PRESET;
			
//����Ԥ�õ�			
		case MY1003_KEY_SHOW_PRESET:
			return KEY_VIRTUAL_PRESET;
//���Ԥ�õ�			
		//case MY1003_KEY_DEL_PRESET:
		//	return KEY_VIRTUAL_DEL_PRESET;

		case MY1003_KEY_FOCUS_ADD:
			return KEY_VIRTUAL_FOCUS_ADD;

		case MY1003_KEY_FOCUS_DEC:
			return KEY_VIRTUAL_FOCUS_DEC;


		case MY1003_KEY_ZOOM_ADD:
			return KEY_VIRTUAL_ZOOM_ADD;
			
		case MY1003_KEY_ZOOM_DEC:
			return KEY_VIRTUAL_ZOOM_DEC;

		case MY1003_KEY_IRIS_ADD:
			return KEY_VIRTUAL_IRIS_ADD;
			
		case MY1003_KEY_IRIS_DEC:
			return KEY_VIRTUAL_IRIS_DEC;
			
		case MY1003_KEY_AUTO:
			return KEY_VIRTUAL_AUTO;

		case MY1003_KEY_WIPER:
			return KEY_VIRTUAL_BRUSH;

		case MY1003_KEY_LIGHT:
			return KEY_VIRTUAL_LAMP;

			
		default:
			break;
	}

	return 0;
}
unsigned long CKeyMY1003::PlaybkProc( unsigned long value )
{
	unsigned long valueTemp = ( value & 0x000000ff );
	unsigned long parameter = ( value >> 8 );

	if( BUTTN_UP == parameter )
	{
		return 0;
	}

	switch(valueTemp)
	{
	    case MY1003_KEY_0:
			{
				if (s_videoInputNum <= 16)
				{
					return KEY_VIRTUAL_10_A;
				}
				else
				{
					return KEY_VIRTUAL_A_A;
				}
			}
		case MY1003_KEY_1:
			return KEY_VIRTUAL_1;
		
		case MY1003_KEY_2:
			return KEY_VIRTUAL_2;
	
		case MY1003_KEY_3:
			return KEY_VIRTUAL_3;
	
		case MY1003_KEY_4:
			return KEY_VIRTUAL_4;
	
		case MY1003_KEY_5:
			return KEY_VIRTUAL_5;
	
		case MY1003_KEY_6:
			return KEY_VIRTUAL_6;
	
		case MY1003_KEY_7:
			return KEY_VIRTUAL_7;
	
		case MY1003_KEY_8:
			return KEY_VIRTUAL_8;
	
		case MY1003_KEY_9:
			return KEY_VIRTUAL_9;

		case MY1003_KEY_ESC:			// 3.0��ESC��ֻ���˳���
			return KEY_VIRTUAL_ESC;		//��ESC������Ϊ�ط�ʱ��STOP����
			
		case MY1003_KEY_ENTER:
			return KEY_VIRTUAL_ENTER;

	//����໭��
		case MY1003_KEY_SINGLE:
			{
					return KEY_VIRTUAL_1X1;
			}
		case  MY1003_KEY_QUAD	:
			{
					return KEY_VIRTUAL_2X2;
			}
		case MY1003_KEY_NINE:
			{
				if (s_videoInputNum <= 5)
				{
					return false;
				}
				else
				{
					return KEY_VIRTUAL_3X3;
				}
			}			
		case  MY1003_KEY_SIXTEEN:
			{
				if (s_videoInputNum <= 9)
				{
					return false;
				}
				else
				{
					return KEY_VIRTUAL_4X4;
				}
			}			
		//case TW485_KEY_MENU:
		//	return KEY_VIRTUAL_MENU;

	//���������·���ܼ�
		//case MY1003_KEY_PTZ:
		//	return KEY_VIRTUAL_PTZ;
			
		case  MY1003_KEY_MENU:
			//�ط�ʱMENU������Ϊ���غ���ʾ�طŶԻ���
			return KEY_VIRTUAL_HIDE;
		//	return KEY_VIRTUAL_MENU;

		//case  MY1003_KEY_SEQ:
		//	return KEY_VIRTUAL_SEQ;

		//case  MY1003_KEY_INFO:
		//	return KEY_VIRTUAL_INFO;
			
		//case MY1003_KEY_SEARCH:
		//	return KEY_VIRTUAL_SEARCH;
			
		case MY1003_KEY_AUDIO:
			return KEY_VIRTUAL_AUDIO;
			
		case  MY1003_KEY_FF:
				return KEY_VIRTUAL_FF;
			
 		//case MY1003_KEY_RECORD:
		//	return KEY_VIRTUAL_RECORD;
			
		case MY1003_KEY_POWER:
			return KEY_VIRTUAL_POWER;
			
		case MY1003_KEY_REW:
			return KEY_VIRTUAL_REW;
			
 		case MY1003_KEY_PLAY:
			return KEY_VIRTUAL_PLAY;
			
		default:
			break;

			
	}

	return 0;
}
unsigned long CKeyMY1003::FuncProc( unsigned long value )
{
	unsigned long valueTemp = ( value & 0x000000ff );
	unsigned long parameter = ( value >> 8 );

	if( BUTTN_UP == parameter )
	{
		return 0;
	}

	//printf("%s():%s:%d, the keyvalue=0x%08x, parameter=0x%08x\n", __FUNCTION__, __FILE__,__LINE__, value, parameter );
	switch(valueTemp)
	{
	//0-9
		case MY1003_KEY_0:			
			{
				if (s_videoInputNum <= 16)
				{
					return KEY_VIRTUAL_10_A;
				}
				else
				{
					return KEY_VIRTUAL_A_A;
				}
			}
		case MY1003_KEY_1:
			return KEY_VIRTUAL_1;
		
		case MY1003_KEY_2:
			return KEY_VIRTUAL_2;
	
		case MY1003_KEY_3:
			return KEY_VIRTUAL_3;
	
		case MY1003_KEY_4:
			return KEY_VIRTUAL_4;
	
		case MY1003_KEY_5:
			return KEY_VIRTUAL_5;
	
		case MY1003_KEY_6:
			return KEY_VIRTUAL_6;
	
		case MY1003_KEY_7:
			return KEY_VIRTUAL_7;
	
		case MY1003_KEY_8:
			return KEY_VIRTUAL_8;
	
		case MY1003_KEY_9:
			return KEY_VIRTUAL_9;

		case MY1003_KEY_ESC:
			return KEY_VIRTUAL_ESC;
			
		case MY1003_KEY_ENTER:
			return KEY_VIRTUAL_ENTER;

		//����໭��
		case MY1003_KEY_SINGLE:
			{
					return KEY_VIRTUAL_1X1;
			}
		case  MY1003_KEY_QUAD	:
			{
					return KEY_VIRTUAL_2X2;
			}
		case MY1003_KEY_NINE:
			{
				if (s_videoInputNum <= 5)
				{
					return false;
				}
				else
				{
					return KEY_VIRTUAL_3X3;
				}
			}			
		case  MY1003_KEY_SIXTEEN:
			{
				if (s_videoInputNum <= 9)
				{
					return false;
				}
				else
				{
					return KEY_VIRTUAL_4X4;
				}
			}			
			
		//���������·���ܼ�
		case MY1003_KEY_PTZ:
			return KEY_VIRTUAL_PTZ;
			
		case  MY1003_KEY_BACKUP:
			return KEY_VIRTUAL_BACKUP;
			
		case  MY1003_KEY_MENU:
			return KEY_VIRTUAL_MENU;

		case  MY1003_KEY_SEQ:
			return KEY_VIRTUAL_SEQ;

		case  MY1003_KEY_INFO:
			return KEY_VIRTUAL_INFO;
			
		case MY1003_KEY_SEARCH:
			return KEY_VIRTUAL_SEARCH;
			
		case MY1003_KEY_AUDIO:
			return KEY_VIRTUAL_AUDIO;
			
		case  MY1003_KEY_ADD:
			return KEY_VIRTUAL_ADD;
			
 		case MY1003_KEY_RECORD:
			return KEY_VIRTUAL_RECORD;
			
		case MY1003_KEY_POWER:
			return KEY_VIRTUAL_POWER;
			
		case MY1003_KEY_DEC:
			return KEY_VIRTUAL_DEC;
			
 		case MY1003_KEY_PLAY:
			return KEY_VIRTUAL_PLAY;

//���������·���ܼ�
//����Ԥ�õ�
		case MY1003_KEY_SET_PRESET:
			return KEY_VIRTUAL_PTZ_SET_PRESET;

//����Ԥ�õ�			
		case MY1003_KEY_SHOW_PRESET:
			return KEY_VIRTUAL_PRESET;
//���Ԥ�õ�			
		//case MY1003_KEY_DEL_PRESET:
		//	return KEY_VIRTUAL_DEL_PRESET;

		case MY1003_KEY_FOCUS_ADD:
			return KEY_VIRTUAL_FOCUS_ADD;

		case MY1003_KEY_FOCUS_DEC:
			return KEY_VIRTUAL_FOCUS_DEC;

		case MY1003_KEY_ZOOM_ADD:
			return KEY_VIRTUAL_ZOOM_ADD;

		case MY1003_KEY_ZOOM_DEC:
			return KEY_VIRTUAL_ZOOM_DEC;

		case MY1003_KEY_IRIS_ADD:
			return KEY_VIRTUAL_IRIS_ADD;

		case MY1003_KEY_IRIS_DEC:
			return KEY_VIRTUAL_IRIS_DEC;

		case MY1003_KEY_AUTO:
			return KEY_VIRTUAL_AUTO;

		//case MY1003_KEY_WIPER:
			//return KEY_VIRTUAL_SPEED_ADD;

		//case MY1003_KEY_LIGHT:
			//return KEY_VIRTUAL_SPEED_DEC;



		//����ҡ�˲���
		case MY1003_KEY_UP:
			return KEY_VIRTUAL_UP;
			
		case MY1003_KEY_DOWN	:
			return KEY_VIRTUAL_DOWN;
			
		case  MY1003_KEY_LEFT:
			return KEY_VIRTUAL_LEFT;
			
		case MY1003_KEY_RIGHT:
			return KEY_VIRTUAL_RIGHT;

		case MY1003_KEY_PTZ_LEFTUP:
			return KEY_VIRTUAL_PTZ_LEFTUP;

		case MY1003_KEY_PTZ_LEFTDOWN:
			return KEY_VIRTUAL_PTZ_LEFTDOWN;
			
		case MY1003_KEY_PTZ_RIGHTUP:
			return KEY_VIRTUAL_PTZ_RIGHTUP;

		case MY1003_KEY_PTZ_RIGHTDOWN:
			return KEY_VIRTUAL_PTZ_RIGHTDOWN;

		case MY1003_KEY_PTZ_ZERO:
			return KEY_VIRTUAL_PTZ_STOP;
			
		case MY1003_KEY_PTZ_SPEED_DEC:
			return KEY_VIRTUAL_SPEED_DEC;
			
		case MY1003_KEY_PTZ_SPEED_ADD:
			return KEY_VIRTUAL_SPEED_ADD;

		default :
			//printf("MODE FUNC default, FILE:%s, LINE:%d\n", __FILE__, __LINE__);
			break;
	
	}
	return 0;
}


