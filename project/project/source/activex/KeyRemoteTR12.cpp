#include "KeyRemoteTR12.h"
#include <stdio.h>

unsigned char CKeyRemoteTR12::s_videoInputNum=0;

CKeyRemoteTR12::CKeyRemoteTR12()
{

}

CKeyRemoteTR12::~CKeyRemoteTR12()
{

}

unsigned long CKeyRemoteTR12::FuncProc( bool isDown, unsigned long value)
{
	switch(value) 
	{
	case KEY_REMOTE_TR12_REC:
		{
			return KEY_VIRTUAL_RECORD;
		}
	case KEY_REMOTE_TR12_POWER:
		{
			return KEY_VIRTUAL_POWER;
		}
	case KEY_REMOTE_TR12_1:
		{
			return KEY_VIRTUAL_1;
		}
	case KEY_REMOTE_TR12_2:
		{
			return KEY_VIRTUAL_2;
		}
	case KEY_REMOTE_TR12_3:
		{
			return KEY_VIRTUAL_3;
		}
	case KEY_REMOTE_TR12_4:
		{
			return KEY_VIRTUAL_4;
		}
	case KEY_REMOTE_TR12_5:
		{
			return KEY_VIRTUAL_5;
		}
	case KEY_REMOTE_TR12_6:
		{
			return KEY_VIRTUAL_6;
		}
	case KEY_REMOTE_TR12_7:
		{
			return KEY_VIRTUAL_7;
		}
	case KEY_REMOTE_TR12_8:
		{
			return KEY_VIRTUAL_8;
		}
	case KEY_REMOTE_TR12_9:
		{
			return KEY_VIRTUAL_9;
		}
	case KEY_REMOTE_TR12_10:
		{
			return KEY_VIRTUAL_A_A;
		}
	case KEY_REMOTE_TR12_0:
		{
			return KEY_VIRTUAL_0;
		}
	//case KEY_REMOTE_TR12_FN:
	//	{
	//		return ;
	//	}
	case KEY_REMOTE_TR12_SPLIT:	//多画面
		{
			return KEY_VIRTUAL_SPLIT;
		}
	case KEY_REMOTE_TR12_GROUP:
		{
			return KEY_VIRTUAL_GROUP;
		}
	case KEY_REMOTE_TR12_SEQ:
		{
			return KEY_VIRTUAL_SEQ;
		}
	case KEY_REMOTE_TR12_AUDIO:
		{
			return KEY_VIRTUAL_AUDIO;
		}
	case KEY_REMOTE_TR12_CHOUTPUT:
		{
				return KEY_VIRTUAL_REDRAW;
		}
	case KEY_REMOTE_TR12_UP:
		{
			return KEY_VIRTUAL_UP;
		}
	case KEY_REMOTE_TR12_DOWN:
		{
			return KEY_VIRTUAL_DOWN;
		}
	case KEY_REMOTE_TR12_LEFT:
		{
			return KEY_VIRTUAL_LEFT;
		}
	case KEY_REMOTE_TR12_RIGHT:
		{
			return KEY_VIRTUAL_RIGHT;
		}
	case KEY_REMOTE_TR12_ENTER:
		{
			return KEY_VIRTUAL_ENTER;
		}
	case KEY_REMOTE_TR12_MENU:
		{
			return KEY_VIRTUAL_MENU;
		}
	case KEY_REMOTE_TR12_ESC:
		{
			return KEY_VIRTUAL_ESC;
		}
	case KEY_REMOTE_TR12_FOCUS_ADD:
		{
			return KEY_VIRTUAL_FOCUS_ADD;
		}
	case KEY_REMOTE_TR12_FOCUS_DEC:
		{
			return KEY_VIRTUAL_FOCUS_DEC;
		}
	case KEY_REMOTE_TR12_IRIS_ADD:
		{
			return KEY_VIRTUAL_IRIS_ADD;
		}
	case KEY_REMOTE_TR12_PTZ:
		{
			return KEY_VIRTUAL_PTZ;
		}
	case KEY_REMOTE_TR12_IRIS_DEC:
		{
			return KEY_VIRTUAL_IRIS_DEC;
		}
	case KEY_REMOTE_TR12_ZOOM_ADD:
		{
			return KEY_VIRTUAL_ZOOM_ADD;
		}
	case KEY_REMOTE_TR12_ZOOM_DEC:
		{
			return KEY_VIRTUAL_ZOOM_DEC;
		}
	case KEY_REMOTE_TR12_PRESET:
		{
			return KEY_VIRTUAL_PRESET;
		}
	case KEY_REMOTE_TR12_CRUISE:
		{
			return KEY_VIRTUAL_PTZ_CRUISE;
		}
	case KEY_REMOTE_TR12_BRUSH:
		{
			return KEY_VIRTUAL_BRUSH;
		}
	case KEY_REMOTE_TR12_LAMP:
		{
			return KEY_VIRTUAL_LAMP;
		}
	case KEY_REMOTE_TR12_TRACK:
		{
			return KEY_VIRTUAL_TRACK;
		}
	case KEY_REMOTE_TR12_EXIT:
		{
			return KEY_VIRTUAL_EXIT;
		}
	//case KEY_REMOTE_TR12_SCHEDULE:	//布防
	//	{
	//		return ;
	//	}
	case KEY_REMOTE_TR12_INFO:
		{
			return KEY_VIRTUAL_INFO;
		}
	case KEY_REMOTE_TR12_PLAY:		//播放
		{
			return KEY_VIRTUAL_PLAY;
		}
	case KEY_REMOTE_TR12_STOP:		//暂停
		{
			return KEY_VIRTUAL_STOP;
		}
	case KEY_REMOTE_TR12_SNAP:		//抓图
		{
			return KEY_VIRTUAL_LIVE_SNAP;
		}
	case KEY_REMOTE_TR12_SEARCH:		//搜索
		{
			return KEY_VIRTUAL_SEARCH;
		}
	case KEY_REMOTE_TR12_PRE_SECTION:			//单帧后退
		{
			return KEY_VIRTUAL_PRE_SECTION;
		}
	case KEY_REMOTE_TR12_NEXT_SECTION:			//单帧前进
		{
			return KEY_VIRTUAL_NEXT_SECTION;
		}
	case KEY_REMOTE_TR12_CLIP:		//剪辑
		{
			return KEY_VIRTUAL_CUT;
		}
	case KEY_REMOTE_TR12_BACKUP:		//备份
		{
			return KEY_VIRTUAL_BACKUP;
		}
	case KEY_REMOTE_TR12_REW:			//快退
		{
			return KEY_VIRTUAL_REW;
		}
	case KEY_REMOTE_TR12_FF:			//快进
		{
			return KEY_VIRTUAL_FF;
		}
	case KEY_REMOTE_TR12_BIG_PIC:		//电子放大
		{
			return KEY_VIRTUAL_ZOOM;
		}
	case KEY_REMOTE_TR12_PIP:			//画中画
		{
			return KEY_VIRTUAL_PIP;
		}
	default:
		{
			break;
		}
	}

	return 0;
}

unsigned long CKeyRemoteTR12::PlaybkProc( bool	isDown, unsigned long value)
{
	switch(value) 
	{
	case KEY_REMOTE_TR12_REC:
		{
			return KEY_VIRTUAL_RECORD;
		}
	case KEY_REMOTE_TR12_POWER:
		{
			return KEY_VIRTUAL_POWER;
		}
	case KEY_REMOTE_TR12_1:
		{
			return KEY_VIRTUAL_1;
		}
	case KEY_REMOTE_TR12_2:
		{
			return KEY_VIRTUAL_2;
		}
	case KEY_REMOTE_TR12_3:
		{
			return KEY_VIRTUAL_3;
		}
	case KEY_REMOTE_TR12_4:
		{
			return KEY_VIRTUAL_4;
		}
	case KEY_REMOTE_TR12_5:
		{
			return KEY_VIRTUAL_5;
		}
	case KEY_REMOTE_TR12_6:
		{
			return KEY_VIRTUAL_6;
		}
	case KEY_REMOTE_TR12_7:
		{
			return KEY_VIRTUAL_7;
		}
	case KEY_REMOTE_TR12_8:
		{
			return KEY_VIRTUAL_8;
		}
	case KEY_REMOTE_TR12_9:
		{
			return KEY_VIRTUAL_9;
		}
	case KEY_REMOTE_TR12_10:
		{
			return KEY_VIRTUAL_A_A;
		}
	case KEY_REMOTE_TR12_0:
		{
			return KEY_VIRTUAL_0;
		}
		//case KEY_REMOTE_TR12_FN:
		//	{
		//		return ;
		//	}
	case KEY_REMOTE_TR12_SPLIT:	//多画面
		{
			return KEY_VIRTUAL_SPLIT;
		}
	case KEY_REMOTE_TR12_GROUP:
		{
			return KEY_VIRTUAL_GROUP;
		}
	case KEY_REMOTE_TR12_SEQ:
		{
			return KEY_VIRTUAL_SEQ;
		}
	case KEY_REMOTE_TR12_AUDIO:
		{
			return KEY_VIRTUAL_AUDIO;
		}
	case KEY_REMOTE_TR12_CHOUTPUT:
		{
			return KEY_VIRTUAL_REDRAW;
		}
	case KEY_REMOTE_TR12_UP:
		{
			return KEY_VIRTUAL_UP;
		}
	case KEY_REMOTE_TR12_DOWN:
		{
			return KEY_VIRTUAL_DOWN;
		}
	case KEY_REMOTE_TR12_LEFT:
		{
			return KEY_VIRTUAL_LEFT;
		}
	case KEY_REMOTE_TR12_RIGHT:
		{
			return KEY_VIRTUAL_RIGHT;
		}
	case KEY_REMOTE_TR12_ENTER:
		{
			return KEY_VIRTUAL_ENTER;
		}
	case KEY_REMOTE_TR12_MENU:
		{
			return KEY_VIRTUAL_HIDE;
		}
	case KEY_REMOTE_TR12_ESC:
		{
			return KEY_VIRTUAL_ESC;
		}
	case KEY_REMOTE_TR12_FOCUS_ADD:
		{
			return KEY_VIRTUAL_FOCUS_ADD;
		}
	case KEY_REMOTE_TR12_FOCUS_DEC:
		{
			return KEY_VIRTUAL_FOCUS_DEC;
		}
	case KEY_REMOTE_TR12_IRIS_ADD:
		{
			return KEY_VIRTUAL_IRIS_ADD;
		}
	case KEY_REMOTE_TR12_PTZ:
		{
			return KEY_VIRTUAL_PTZ;
		}
	case KEY_REMOTE_TR12_IRIS_DEC:
		{
			return KEY_VIRTUAL_IRIS_DEC;
		}
	case KEY_REMOTE_TR12_ZOOM_ADD:
		{
			return KEY_VIRTUAL_ZOOM_ADD;
		}
	case KEY_REMOTE_TR12_ZOOM_DEC:
		{
			return KEY_VIRTUAL_ZOOM_DEC;
		}
	case KEY_REMOTE_TR12_PRESET:
		{
			return KEY_VIRTUAL_PRESET;
		}
	case KEY_REMOTE_TR12_CRUISE:
		{
			return KEY_VIRTUAL_PTZ_CRUISE;
		}
	case KEY_REMOTE_TR12_BRUSH:
		{
			return KEY_VIRTUAL_BRUSH;
		}
	case KEY_REMOTE_TR12_LAMP:
		{
			return KEY_VIRTUAL_LAMP;
		}
	case KEY_REMOTE_TR12_TRACK:
		{
			return KEY_VIRTUAL_TRACK;
		}
	case KEY_REMOTE_TR12_EXIT:
		{
			return KEY_VIRTUAL_EXIT;
		}
		//case KEY_REMOTE_TR12_SCHEDULE:	//布防
		//	{
		//		return ;
		//	}
	case KEY_REMOTE_TR12_INFO:
		{
			return KEY_VIRTUAL_INFO;
		}
	case KEY_REMOTE_TR12_PLAY:		//播放
		{
			return KEY_VIRTUAL_PLAY;
		}
	case KEY_REMOTE_TR12_STOP:		//暂停
		{
			return KEY_VIRTUAL_STOP;
		}
	case KEY_REMOTE_TR12_SNAP:		//抓图
		{
			return KEY_VIRTUAL_PB_SNAP;
		}
	case KEY_REMOTE_TR12_SEARCH:		//搜索
		{
			return KEY_VIRTUAL_SEARCH;
		}
	case KEY_REMOTE_TR12_PRE_SECTION:			//单帧后退
		{
			return KEY_VIRTUAL_PRE_SECTION;
		}
	case KEY_REMOTE_TR12_NEXT_SECTION:			//单帧前进
		{
			return KEY_VIRTUAL_NEXT_SECTION;
		}
	case KEY_REMOTE_TR12_CLIP:		//剪辑
		{
			return KEY_VIRTUAL_CUT;
		}
	case KEY_REMOTE_TR12_BACKUP:		//备份
		{
			return KEY_VIRTUAL_BACKUP;
		}
	case KEY_REMOTE_TR12_REW:			//快退
		{
			return KEY_VIRTUAL_REW;
		}
	case KEY_REMOTE_TR12_FF:			//快进
		{
			return KEY_VIRTUAL_FF;
		}
	case KEY_REMOTE_TR12_BIG_PIC:		//电子放大
		{
			return KEY_VIRTUAL_ZOOM;
		}
	case KEY_REMOTE_TR12_PIP:			//画中画
		{
			return KEY_VIRTUAL_PIP;
		}
	default:
		{
			break;
		}
	}

	return 0;
}

unsigned long CKeyRemoteTR12::PtzProc( bool	isDown,unsigned long value)
{
	switch(value) 
	{
	case KEY_REMOTE_TR12_REC:
		{
			return KEY_VIRTUAL_RECORD;
		}
	case KEY_REMOTE_TR12_POWER:
		{
			return KEY_VIRTUAL_POWER;
		}
	case KEY_REMOTE_TR12_1:
		{
			return KEY_VIRTUAL_1;
		}
	case KEY_REMOTE_TR12_2:
		{
			return KEY_VIRTUAL_2;
		}
	case KEY_REMOTE_TR12_3:
		{
			return KEY_VIRTUAL_3;
		}
	case KEY_REMOTE_TR12_4:
		{
			return KEY_VIRTUAL_4;
		}
	case KEY_REMOTE_TR12_5:
		{
			return KEY_VIRTUAL_5;
		}
	case KEY_REMOTE_TR12_6:
		{
			return KEY_VIRTUAL_6;
		}
	case KEY_REMOTE_TR12_7:
		{
			return KEY_VIRTUAL_7;
		}
	case KEY_REMOTE_TR12_8:
		{
			return KEY_VIRTUAL_8;
		}
	case KEY_REMOTE_TR12_9:
		{
			return KEY_VIRTUAL_9;
		}
	case KEY_REMOTE_TR12_10:
		{
			return KEY_VIRTUAL_A_A;
		}
	case KEY_REMOTE_TR12_0:
		{
			return KEY_VIRTUAL_0;
		}
		//case KEY_REMOTE_TR12_FN:
		//	{
		//		return ;
		//	}
	case KEY_REMOTE_TR12_SPLIT:	//多画面
		{
			return KEY_VIRTUAL_SPLIT;
		}
	case KEY_REMOTE_TR12_GROUP:
		{
			return KEY_VIRTUAL_GROUP;
		}
	case KEY_REMOTE_TR12_SEQ:
		{
			return KEY_VIRTUAL_SEQ;
		}
	case KEY_REMOTE_TR12_AUDIO:
		{
			return KEY_VIRTUAL_AUDIO;
		}
	case KEY_REMOTE_TR12_CHOUTPUT:
		{
			return KEY_VIRTUAL_REDRAW;
		}
	case KEY_REMOTE_TR12_UP:
		{
			return KEY_VIRTUAL_PTZ_UP;
		}
	case KEY_REMOTE_TR12_DOWN:
		{
			return KEY_VIRTUAL_PTZ_DOWN;
		}
	case KEY_REMOTE_TR12_LEFT:
		{
			return KEY_VIRTUAL_PTZ_LEFT;
		}
	case KEY_REMOTE_TR12_RIGHT:
		{
			return KEY_VIRTUAL_PTZ_RIGHT;
		}
	case KEY_REMOTE_TR12_ENTER:
		{
			return KEY_VIRTUAL_ENTER;
		}
	case KEY_REMOTE_TR12_MENU:
		{
			return KEY_VIRTUAL_MENU;
		}
	case KEY_REMOTE_TR12_ESC:
		{
			return KEY_VIRTUAL_ESC;
		}
	case KEY_REMOTE_TR12_FOCUS_ADD:
		{
			return KEY_VIRTUAL_FOCUS_ADD;
		}
	case KEY_REMOTE_TR12_FOCUS_DEC:
		{
			return KEY_VIRTUAL_FOCUS_DEC;
		}
	case KEY_REMOTE_TR12_IRIS_ADD:
		{
			return KEY_VIRTUAL_IRIS_ADD;
		}
	case KEY_REMOTE_TR12_PTZ:
		{
			return KEY_VIRTUAL_HIDE;
		}
	case KEY_REMOTE_TR12_IRIS_DEC:
		{
			return KEY_VIRTUAL_IRIS_DEC;
		}
	case KEY_REMOTE_TR12_ZOOM_ADD:
		{
			return KEY_VIRTUAL_ZOOM_ADD;
		}
	case KEY_REMOTE_TR12_ZOOM_DEC:
		{
			return KEY_VIRTUAL_ZOOM_DEC;
		}
	case KEY_REMOTE_TR12_PRESET:
		{
			return KEY_VIRTUAL_PRESET;
		}
	case KEY_REMOTE_TR12_CRUISE:
		{
			return KEY_VIRTUAL_PTZ_CRUISE;
		}
	case KEY_REMOTE_TR12_BRUSH:
		{
			return KEY_VIRTUAL_BRUSH;
		}
	case KEY_REMOTE_TR12_LAMP:
		{
			return KEY_VIRTUAL_LAMP;
		}
	case KEY_REMOTE_TR12_TRACK:
		{
			return KEY_VIRTUAL_TRACK;
		}
	case KEY_REMOTE_TR12_EXIT:
		{
			return KEY_VIRTUAL_EXIT;
		}
		//case KEY_REMOTE_TR12_SCHEDULE:	//布防
		//	{
		//		return ;
		//	}
	case KEY_REMOTE_TR12_INFO:
		{
			return KEY_VIRTUAL_INFO;
		}
	case KEY_REMOTE_TR12_PLAY:		//播放
		{
			return KEY_VIRTUAL_PLAY;
		}
	case KEY_REMOTE_TR12_STOP:		//暂停
		{
			return KEY_VIRTUAL_STOP;
		}
	case KEY_REMOTE_TR12_SNAP:		//抓图
		{
			return KEY_VIRTUAL_LIVE_SNAP;
		}
	case KEY_REMOTE_TR12_SEARCH:		//搜索
		{
			return KEY_VIRTUAL_SEARCH;
		}
	case KEY_REMOTE_TR12_PRE_SECTION:			//单帧后退
		{
			return KEY_VIRTUAL_PRE_SECTION;
		}
	case KEY_REMOTE_TR12_NEXT_SECTION:			//单帧前进
		{
			return KEY_VIRTUAL_NEXT_SECTION;
		}
	case KEY_REMOTE_TR12_CLIP:		//剪辑
		{
			return KEY_VIRTUAL_CUT;
		}
	case KEY_REMOTE_TR12_BACKUP:		//备份
		{
			return KEY_VIRTUAL_BACKUP;
		}
	case KEY_REMOTE_TR12_REW:			//快退
		{
			return KEY_VIRTUAL_REW;
		}
	case KEY_REMOTE_TR12_FF:			//快进
		{
			return KEY_VIRTUAL_FF;
		}
	case KEY_REMOTE_TR12_BIG_PIC:		//电子放大
		{
			return KEY_VIRTUAL_ZOOM;
		}
	case KEY_REMOTE_TR12_PIP:			//画中画
		{
			return KEY_VIRTUAL_PIP;
		}
	default:
		{
			break;
		}
	}

	return 0;
}

unsigned long CKeyRemoteTR12::UIProc( bool	isDown,unsigned long value)
{
	switch(value) 
	{
	case KEY_REMOTE_TR12_REC:
		{
			return KEY_VIRTUAL_RECORD;
		}
	case KEY_REMOTE_TR12_POWER:
		{
			return KEY_VIRTUAL_POWER;
		}
	case KEY_REMOTE_TR12_1:
		{
			return KEY_VIRTUAL_1;
		}
	case KEY_REMOTE_TR12_2:
		{
			return KEY_VIRTUAL_2;
		}
	case KEY_REMOTE_TR12_3:
		{
			return KEY_VIRTUAL_3;
		}
	case KEY_REMOTE_TR12_4:
		{
			return KEY_VIRTUAL_4;
		}
	case KEY_REMOTE_TR12_5:
		{
			return KEY_VIRTUAL_5;
		}
	case KEY_REMOTE_TR12_6:
		{
			return KEY_VIRTUAL_6;
		}
	case KEY_REMOTE_TR12_7:
		{
			return KEY_VIRTUAL_7;
		}
	case KEY_REMOTE_TR12_8:
		{
			return KEY_VIRTUAL_8;
		}
	case KEY_REMOTE_TR12_9:
		{
			return KEY_VIRTUAL_9;
		}
	case KEY_REMOTE_TR12_10:
		{
			return KEY_VIRTUAL_A_A;
		}
	case KEY_REMOTE_TR12_0:
		{
			return KEY_VIRTUAL_0;
		}
		//case KEY_REMOTE_TR12_FN:
		//	{
		//		return ;
		//	}
	case KEY_REMOTE_TR12_SPLIT:	//多画面
		{
			return KEY_VIRTUAL_SPLIT;
		}
	case KEY_REMOTE_TR12_GROUP:
		{
			return KEY_VIRTUAL_GROUP;
		}
	case KEY_REMOTE_TR12_SEQ:
		{
			return KEY_VIRTUAL_SEQ;
		}
	case KEY_REMOTE_TR12_AUDIO:
		{
			return KEY_VIRTUAL_AUDIO;
		}
	case KEY_REMOTE_TR12_CHOUTPUT:
		{
			return KEY_VIRTUAL_REDRAW;
		}
	case KEY_REMOTE_TR12_UP:
		{
			return KEY_VIRTUAL_UP;
		}
	case KEY_REMOTE_TR12_DOWN:
		{
			return KEY_VIRTUAL_DOWN;
		}
	case KEY_REMOTE_TR12_LEFT:
		{
			return KEY_VIRTUAL_LEFT;
		}
	case KEY_REMOTE_TR12_RIGHT:
		{
			return KEY_VIRTUAL_RIGHT;
		}
	case KEY_REMOTE_TR12_ENTER:
		{
			return KEY_VIRTUAL_ENTER;
		}
	case KEY_REMOTE_TR12_MENU:
		{
			return KEY_VIRTUAL_MENU;
		}
	case KEY_REMOTE_TR12_ESC:
		{
			return KEY_VIRTUAL_ESC;
		}
	case KEY_REMOTE_TR12_FOCUS_ADD:
		{
			return KEY_VIRTUAL_FOCUS_ADD;
		}
	case KEY_REMOTE_TR12_FOCUS_DEC:
		{
			return KEY_VIRTUAL_FOCUS_DEC;
		}
	case KEY_REMOTE_TR12_IRIS_ADD:
		{
			return KEY_VIRTUAL_IRIS_ADD;
		}
	case KEY_REMOTE_TR12_PTZ:
		{
			return KEY_VIRTUAL_PTZ;
		}
	case KEY_REMOTE_TR12_IRIS_DEC:
		{
			return KEY_VIRTUAL_IRIS_DEC;
		}
	case KEY_REMOTE_TR12_ZOOM_ADD:
		{
			return KEY_VIRTUAL_ZOOM_ADD;
		}
	case KEY_REMOTE_TR12_ZOOM_DEC:
		{
			return KEY_VIRTUAL_ZOOM_DEC;
		}
	case KEY_REMOTE_TR12_PRESET:
		{
			return KEY_VIRTUAL_PRESET;
		}
	case KEY_REMOTE_TR12_CRUISE:
		{
			return KEY_VIRTUAL_PTZ_CRUISE;
		}
	case KEY_REMOTE_TR12_BRUSH:
		{
			return KEY_VIRTUAL_BRUSH;
		}
	case KEY_REMOTE_TR12_LAMP:
		{
			return KEY_VIRTUAL_LAMP;
		}
	case KEY_REMOTE_TR12_TRACK:
		{
			return KEY_VIRTUAL_TRACK;
		}
	case KEY_REMOTE_TR12_EXIT:
		{
			return KEY_VIRTUAL_EXIT;
		}
		//case KEY_REMOTE_TR12_SCHEDULE:	//布防
		//	{
		//		return ;
		//	}
	case KEY_REMOTE_TR12_INFO:
		{
			return KEY_VIRTUAL_INFO;
		}
	case KEY_REMOTE_TR12_PLAY:		//播放
		{
			return KEY_VIRTUAL_PLAY;
		}
	case KEY_REMOTE_TR12_STOP:		//暂停
		{
			return KEY_VIRTUAL_STOP;
		}
	case KEY_REMOTE_TR12_SNAP:		//抓图
		{
			return KEY_VIRTUAL_LIVE_SNAP;
		}
	case KEY_REMOTE_TR12_SEARCH:		//搜索
		{
			return KEY_VIRTUAL_SEARCH;
		}
	case KEY_REMOTE_TR12_PRE_SECTION:			//单帧后退
		{
			return KEY_VIRTUAL_PRE_SECTION;
		}
	case KEY_REMOTE_TR12_NEXT_SECTION:			//单帧前进
		{
			return KEY_VIRTUAL_NEXT_SECTION;
		}
	case KEY_REMOTE_TR12_CLIP:		//剪辑
		{
			return KEY_VIRTUAL_CUT;
		}
	case KEY_REMOTE_TR12_BACKUP:		//备份
		{
			return KEY_VIRTUAL_BACKUP;
		}
	case KEY_REMOTE_TR12_REW:			//快退
		{
			return KEY_VIRTUAL_REW;
		}
	case KEY_REMOTE_TR12_FF:			//快进
		{
			return KEY_VIRTUAL_FF;
		}
	case KEY_REMOTE_TR12_BIG_PIC:		//电子放大
		{
			return KEY_VIRTUAL_ZOOM;
		}
	case KEY_REMOTE_TR12_PIP:			//画中画
		{
			return KEY_VIRTUAL_PIP;
		}
	default:
		{
			break;
		}
	}

	return 0;
}

unsigned long CKeyRemoteTR12::TranKeyRemoteTR12(KEY_MODE mod, bool isDown, unsigned long value)
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

