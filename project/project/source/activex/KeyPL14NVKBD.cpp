#include "KeyPL14NVKBD.h"
#include "PL14_NVKBD30_70.h"
#include "LocalKeyValue.h"
#include "stdio.h"

CKeyPL14NVKBD::CKeyPL14NVKBD()
{

}

CKeyPL14NVKBD::~CKeyPL14NVKBD()
{

}

unsigned long CKeyPL14NVKBD::TranKeyPL14NVKBD(KEY_MODE mod, unsigned long value)
{
	unsigned long controlValue = value;
	unsigned long controlParam = 0;
	GetControlParam(controlValue, controlParam);

	switch(mod) 
	{
	case KEY_MODE_PTZ:
		return PtzProc( controlValue, controlParam );

	case KEY_MODE_PLAYBACK:
		return PlaybkProc( controlValue, controlParam );

	case KEY_MODE_FUNC:
		return FuncProc( controlValue, controlParam );

	case KEY_MODE_UI:
		return UIProc( controlValue, controlParam );

	default:
		return 0;
	}	
}

unsigned long CKeyPL14NVKBD::GetControlParam(unsigned long &controlValue, unsigned long &controlParam)
{
	controlParam = 0;
	unsigned long value = controlValue & 0xffffff00;
	switch (value)
	{
	case PL14_NVKBD_GROUP:
		{
			controlParam = (controlValue & 0xff);
			controlValue = value;
			return 0;
			break;
		}
	case PL14_NVKBD_PLAY_FF:
		{
			controlValue = value;
			return 0;
			break;
		}
	case PL14_NVKBD_PALY_REW:
		{
			controlValue = value;
			return 0;
			break;
		}
	case PL14_NVKBD_SEQ:
		{
			controlValue = value;
			return 0;
			break;
		}
	case PL14_NVKBD_FLY:
		{
			controlParam = (controlValue & 0xff);
			controlValue = value;
			return 0;
			break;
		}
	case PL14_NVKBD_FLY_IN:
		{
			controlParam = (controlValue & 0xff);
			controlValue = value;
			return 0;
			break;
		}
	case PL14_NVKBD_NUM:
		{
			controlParam = (controlValue & 0xff);
			controlValue = value;
			return 0;
			break;
		}
	default:
		{
			return 0;
			break;
		}
	}

	return 1;
}

unsigned long CKeyPL14NVKBD::GetInputNumber(KEY_MODE mode, unsigned long controlParam)
{
	if (0x01 == controlParam)
	{
		return KEY_VIRTUAL_1;
	}

	if (0x02 == controlParam)
	{
		return KEY_VIRTUAL_2;
	}

	if (0x03 == controlParam)
	{
		return KEY_VIRTUAL_3;
	}

	if (0x04 == controlParam)
	{
		return KEY_VIRTUAL_4;
	}

	if (0x05 == controlParam)
	{
		return KEY_VIRTUAL_5;
	}

	if (0x06 == controlParam)
	{
		return KEY_VIRTUAL_6;
	}

	if (0x07 == controlParam)
	{
		return KEY_VIRTUAL_7;
	}

	if (0x08 == controlParam)
	{
		return KEY_VIRTUAL_8;
	}

	if (0x09 == controlParam)
	{
		return KEY_VIRTUAL_9;
	}

	if (0x0a == controlParam)
	{
		return KEY_VIRTUAL_10;
// 		if (KEY_MODE_FUNC == mode)
// 		{
// 			return KEY_VIRTUAL_10;	
// 		}
// 		else if (KEY_MODE_UI == mode)
// 		{
// 			return KEY_VIRTUAL_0;
// 		}
// 		else if (KEY_MODE_PTZ == mode)
// 		{
// 			return KEY_VIRTUAL_10;
// 		}
// 		else if (KEY_MODE_PLAYBACK == mode)
// 		{
// 			return KEY_VIRTUAL_10;
// 		}
// 		else
// 		{
// 			printf("%s:%s:%d, Running at here\n", __FUNCTION__, __FILE__, __LINE__);
// 			return KEY_VIRTUAL_0;
// 		}
	}

	if (0x0b == controlParam)
	{
		return KEY_VIRTUAL_11;
	}

	if (0x0c == controlParam)
	{
		return KEY_VIRTUAL_12;
	}


	if (0x0d == controlParam)
	{
		return KEY_VIRTUAL_13;
	}

	if (0x0e == controlParam)
	{
		return KEY_VIRTUAL_14;
	}

	if (0x0f == controlParam)
	{
		return KEY_VIRTUAL_15;
	}

	if (0x10 == controlParam)
	{
		return KEY_VIRTUAL_16;
	}

	if (0x11 == controlParam)
	{
		return KEY_VIRTUAL_17;
	}

	if (0x12 == controlParam)
	{
		return KEY_VIRTUAL_18;
	}

	if (0x13 == controlParam)
	{
		return KEY_VIRTUAL_19;
	}

	if (0x14 == controlParam)
	{
		return KEY_VIRTUAL_20;
	}


	if (0x15 == controlParam)
	{
		return KEY_VIRTUAL_21;
	}

	if (0x16 == controlParam)
	{
		return KEY_VIRTUAL_22;
	}

	if (0x17 == controlParam)
	{
		return KEY_VIRTUAL_23;
	}

	if (0x18 == controlParam)
	{
		return KEY_VIRTUAL_24;
	}

	if (0x19 == controlParam)
	{
		return KEY_VIRTUAL_25;
	}

	if (0x1a == controlParam)
	{
		return KEY_VIRTUAL_26;
	}

	if (0x1b == controlParam)
	{
		return KEY_VIRTUAL_27;
	}

	if (0x1c == controlParam)
	{
		return KEY_VIRTUAL_28;
	}


	if (0x1d == controlParam)
	{
		return KEY_VIRTUAL_29;
	}

	if (0x1e == controlParam)
	{
		return KEY_VIRTUAL_30;
	}

	if (0x1f == controlParam)
	{
		return KEY_VIRTUAL_31;
	}

	if (0x20 == controlParam)
	{
		return KEY_VIRTUAL_32;
	}

	return 0;
}

unsigned long CKeyPL14NVKBD::GetSplitMode(unsigned long channelCount)
{
	if (1 == channelCount)
	{
		return KEY_VIRTUAL_1X1;
	}

	if (1 < channelCount && 5 > channelCount)
	{
		return KEY_VIRTUAL_2X2;
	}

	if (4 < channelCount && 7 > channelCount)
	{
		return KEY_VIRTUAL_2X3;
	}

	if (6 < channelCount && 10 > channelCount)
	{
		return KEY_VIRTUAL_3X3;
	}

	if (9 < channelCount && 17 > channelCount)
	{
		return KEY_VIRTUAL_4X4;
	}

	if ( 16 < channelCount && 26 > channelCount)
	{
		return KEY_VIRTUAL_5X5;
	}

	if (25 < channelCount && 37 > channelCount)
	{
		return KEY_VIRTUAL_6X6;
	}

	return 0;
}

unsigned long CKeyPL14NVKBD::GetFlyCommand(KEY_MODE mode, unsigned long controlParam)
{
	if (0x80 == controlParam)
	{
		return KEY_VIRTUAL_FLY_MEDIUM;
	}

	if (0x40 == controlParam)
	{
		switch(mode) 
		{
		case KEY_MODE_PTZ:
		case KEY_MODE_PLAYBACK:
		case KEY_MODE_UI:
			return KEY_VIRTUAL_FLY_LEFT_1;
		default:
			return 0;
		}
	}

	if (0x36 == controlParam)
	{
		switch(mode) 
		{
		case KEY_MODE_PTZ:
		case KEY_MODE_PLAYBACK:
		case KEY_MODE_FUNC:
		case KEY_MODE_UI:
		return KEY_VIRTUAL_FLY_LEFT_2;
		default:
			return 0;
		}
	}

	if (0x2c == controlParam)
	{
		switch(mode) 
		{
		case KEY_MODE_PTZ:
		case KEY_MODE_PLAYBACK:
		//case KEY_MODE_FUNC:
		case KEY_MODE_UI:
			{
				return KEY_VIRTUAL_FLY_LEFT_3;
				break;
			}
		
		default:
			return 0;
		}
	}

	if (0x21 == controlParam)
	{
		switch(mode) 
		{
		case KEY_MODE_PTZ:
		case KEY_MODE_PLAYBACK:
		case KEY_MODE_FUNC:
		case KEY_MODE_UI:
			return KEY_VIRTUAL_FLY_LEFT_4;
		default:
			return 0;
		}
	}

	if (0x16 == controlParam)
	{
		switch(mode) 
		{
		case KEY_MODE_PTZ:
		case KEY_MODE_PLAYBACK:
		//case KEY_MODE_FUNC:
		case KEY_MODE_UI:
			return KEY_VIRTUAL_FLY_LEFT_5;
		default:
			return 0;
		}
	}

	if (0x0b == controlParam)
	{
		switch(mode) 
		{
		case KEY_MODE_PTZ:
		case KEY_MODE_PLAYBACK:
		case KEY_MODE_FUNC:
		case KEY_MODE_UI:
			return KEY_VIRTUAL_FLY_LEFT_6;
		default:
			return 0;
		}
	}

	if (0x00 == controlParam)
	{
		switch(mode) 
		{
		case KEY_MODE_PTZ:
		case KEY_MODE_PLAYBACK:
		//case KEY_MODE_FUNC:
			return KEY_VIRTUAL_FLY_LEFT_7;
		case KEY_MODE_UI:
			return KEY_VIRTUAL_LEFT;
		default:
			return 0;
		}
	}

	if (0xbf == controlParam)
	{
		switch(mode) 
		{
		case KEY_MODE_PTZ:
		case KEY_MODE_PLAYBACK:
		//case KEY_MODE_FUNC:
		case KEY_MODE_UI:
			return KEY_VIRTUAL_FLY_RIGHT_1;
		default:
			return 0;
		}
	}

	if (0xca == controlParam)
	{
		switch(mode) 
		{
		case KEY_MODE_PTZ:
		case KEY_MODE_PLAYBACK:
		case KEY_MODE_FUNC:
		case KEY_MODE_UI:
			return KEY_VIRTUAL_FLY_RIGHT_2;

		default:
			return 0;
		}
	}

	if (0xd5 == controlParam)
	{
		switch(mode) 
		{
		case KEY_MODE_PTZ:
		case KEY_MODE_PLAYBACK:
		//case KEY_MODE_FUNC:
		case KEY_MODE_UI:
			{
				return KEY_VIRTUAL_FLY_RIGHT_3;
			}
		default:
			return 0;
		}
	}

	if (0xe0 == controlParam)
	{
		switch(mode) 
		{
		case KEY_MODE_PTZ:
		case KEY_MODE_PLAYBACK:
		case KEY_MODE_FUNC:
		case KEY_MODE_UI:
			return KEY_VIRTUAL_FLY_RIGHT_4;

		default:
			return 0;
		}
	}

	if (0xeb == controlParam)
	{
		switch(mode) 
		{
		case KEY_MODE_PTZ:
		case KEY_MODE_PLAYBACK:
		//case KEY_MODE_FUNC:
		case KEY_MODE_UI:
			return KEY_VIRTUAL_FLY_RIGHT_5;

		default:
			return 0;
		}
	}

	if (0xf5 == controlParam)
	{
		switch(mode) 
		{
		case KEY_MODE_PTZ:
		case KEY_MODE_PLAYBACK:
		case KEY_MODE_FUNC:
		case KEY_MODE_UI:
			return KEY_VIRTUAL_FLY_RIGHT_6;

		default:
			return 0;
		}
	}

	if (0xff == controlParam)
	{
		switch(mode) 
		{
		case KEY_MODE_PTZ:
		case KEY_MODE_PLAYBACK:
		//case KEY_MODE_FUNC:
			return KEY_VIRTUAL_FLY_RIGHT_7;
		case KEY_MODE_UI:
			return KEY_VIRTUAL_RIGHT;
		default:
			return 0;
		}
	}


	return 0;
}

unsigned long CKeyPL14NVKBD::GetFlyInCommand(KEY_MODE mode, unsigned long controlParam)
{
	if (0x00 == controlParam)
	{
		//printf("%s:%s:%d, flay in L\n", __FUNCTION__, __FILE__, __LINE__);
		switch(mode) 
		{
		case KEY_MODE_PTZ:
			return KEY_VIRTUAL_FLFLY_IN_LEFT;
		case KEY_MODE_PLAYBACK:
			return KEY_VIRTUAL_SR;
		case KEY_MODE_FUNC:
			return KEY_VIRTUAL_LEFT;
		case KEY_MODE_UI:
			return KEY_VIRTUAL_UP;
		default:
			return 0;
		}
	}

	if (0xff == controlParam)
	{
		//printf("%s:%s:%d, flay in R\n", __FUNCTION__, __FILE__, __LINE__);
		switch(mode) 
		{
		case KEY_MODE_PTZ:
			return KEY_VIRTUAL_FLFLY_IN_RIGHT;
		case KEY_MODE_PLAYBACK:
			return KEY_VIRTUAL_SF;
		case KEY_MODE_FUNC:
			return KEY_VIRTUAL_RIGHT;
		case KEY_MODE_UI:
			return KEY_VIRTUAL_DOWN;

		default:
			return 0;
		}
	}

	return 0;
}


unsigned long CKeyPL14NVKBD::UIProc( unsigned long value, unsigned long param/* = 0*/ )
{	
	switch (value)
	{
	case PL14_NVKBD_NUM:
		{
			return GetInputNumber(KEY_MODE_UI, param);
			break;
		}

		//功能键
	case PL14_NVKBD_ENTER:
		{
			return KEY_VIRTUAL_ENTER;
			break;
		}
	case PL14_NVKBD_ESC:
		{
			return KEY_VIRTUAL_ESC;
			break;
		}
	case PL14_NVKBD_MENU:
		{
			return KEY_VIRTUAL_MENU;
			break;
		}

		//录像和播放控制
	case PL14_NVKBD_RECORD:
		{
			return KEY_VIRTUAL_RECORD;
			break;
		}
	case PL14_NVKBD_PLAY_STOP:
		{
			return KEY_VIRTUAL_STOP;
			break;
		}
	case PL14_NVKBD_PLAY_SF:
		{
			return KEY_VIRTUAL_PLAY;
			break;
		}

	case PL14_NVKBD_PLAY_SR:
		{
			return KEY_VIRTUAL_SR;
			break;
		}
	case PL14_NVKBD_PLAY_FF:
		{
			return KEY_VIRTUAL_FF;
			break;
		}

	case PL14_NVKBD_PALY_REW:
		{
			return KEY_VIRTUAL_REW;
			break;
		}
	case PL14_NVKBD_PLAY:
		{
			return KEY_VIRTUAL_PLAY;
			break;
		}

	case PL14_NVKBD_SPOT:
		{
			//return KEY_VIRTUAL_SEQ;
			return 0;
			break;
		}
	case PL14_NVKBD_GROUP:
		{
			return GetSplitMode(param);
			break;
		}

	case PL14_NVKBD_SEQ:
		{
			return KEY_VIRTUAL_SEQ;
			break;
		}
	case PL14_NVKBD_BACKUP:
		{
			return KEY_VIRTUAL_BACKUP;
			break;
		}
	case PL14_NVKBD_ENTER_PTZ:
		{
			return KEY_VIRTUAL_PTZ;
			break;
		}
	case PL14_NVKBD_SEARCH:
		{
			return KEY_VIRTUAL_SEARCH;
			break;
		}

	case PL14_NVKBD_UP:
		{
			return KEY_VIRTUAL_UP;
			break;
		}
	case PL14_NVKBD_DOWN:
		{
			return KEY_VIRTUAL_DOWN;
			break;
		}
	case PL14_NVKBD_LEFT:
		{
			return KEY_VIRTUAL_LEFT;
			break;
		}
	case PL14_NVKBD_RIGHT:
		{
			return KEY_VIRTUAL_RIGHT;
			break;
		}

// 	case PL14_NVKBD_U_1X1:
// 		{
// 			return KEY_VIRTUAL_1X1;
// 			break;
// 		}
// 	case PL14_NVKBD_D_2X2:
// 		{
// 			return KEY_VIRTUAL_2X2;
// 			break;
// 		}
// 	case PL14_NVKBD_L3X3:
// 		{
// 			return KEY_VIRTUAL_3X3;
// 			break;
// 		}
// 	case PL14_NVKBD_R_4X4:
// 		{
// 			return KEY_VIRTUAL_4X4;
// 			break;
// 		}

		//+/-
// 	case PL14_NVKBD_ADD:
// 		{
// 			return KEY_VIRTUAL_ADD;
// 			break;
// 		}
// 	case PL14_NVKBD_DEC:
// 		{
// 			return KEY_VIRTUAL_DEC;
// 			break;
// 		}
	case PL14_NVKBD_SEL:
		{
			return KEY_VIRTUAL_AUDIO;
			break;
		}

	case PL14_NVKBD_FLY:
		{
			return GetFlyCommand(KEY_MODE_UI, param);
			break;
		}
	case PL14_NVKBD_FLY_IN:
		{
			return GetFlyInCommand(KEY_MODE_UI, param);
			break;
		}
	default:
		{
			return 0;
			break;
		}
	}
}

unsigned long CKeyPL14NVKBD::PtzProc( unsigned long value, unsigned long param/* = 0*/ )
{
	switch (value)
	{
	case PL14_NVKBD_NUM:
		{
			return GetInputNumber(KEY_MODE_PTZ, param);
			break;
		}

		//功能键
	case PL14_NVKBD_ENTER:
		{
			return KEY_VIRTUAL_ENTER;
			break;
		}
	case PL14_NVKBD_ESC:
		{
			return KEY_VIRTUAL_ESC;
			break;
		}
	case PL14_NVKBD_MENU:
		{
			return KEY_VIRTUAL_MENU;
			break;
		}

		//录像和播放控制
	case PL14_NVKBD_RECORD:
		{
			return KEY_VIRTUAL_RECORD;
			break;
		}
	case PL14_NVKBD_PLAY_STOP:
		{
			return KEY_VIRTUAL_STOP;
			break;
		}
	case PL14_NVKBD_PLAY_SF:
		{
			return KEY_VIRTUAL_SF;
			break;
		}

	case PL14_NVKBD_PLAY_SR:
		{
			return KEY_VIRTUAL_SR;
			break;
		}
	case PL14_NVKBD_PLAY_FF:
		{
			return KEY_VIRTUAL_FF;
			break;
		}

	case PL14_NVKBD_PALY_REW:
		{
			return KEY_VIRTUAL_REW;
			break;
		}
	case PL14_NVKBD_PLAY:
		{
			return KEY_VIRTUAL_PLAY;
			break;
		}

	case PL14_NVKBD_SPOT:
		{
			//return KEY_VIRTUAL_SEQ;
			return 0;
		}
	case PL14_NVKBD_GROUP:
		{
			return GetSplitMode(param);
			break;
		}

	case PL14_NVKBD_SEQ:
		{
			return KEY_VIRTUAL_SEQ;
			break;
		}
	case PL14_NVKBD_BACKUP:
		{
			return KEY_VIRTUAL_BACKUP;
			break;
		}
	case PL14_NVKBD_ENTER_PTZ:
		{
			return KEY_VIRTUAL_PTZ;
			break;
		}
	case PL14_NVKBD_SEARCH:
		{
			return KEY_VIRTUAL_SEARCH;
			break;
		}

	case PL14_NVKBD_UP:
		{
			return KEY_VIRTUAL_UP;
			break;
		}
	case PL14_NVKBD_DOWN:
		{
			return KEY_VIRTUAL_DOWN;
			break;
		}
	case PL14_NVKBD_LEFT:
		{
			return KEY_VIRTUAL_LEFT;
			break;
		}
	case PL14_NVKBD_RIGHT:
		{
			return KEY_VIRTUAL_RIGHT;
			break;
		}

	//case PL14_NVKBD_U_1X1:
	//	{
	//		return KEY_VIRTUAL_1X1;
	//		break;
	//	}
	//case PL14_NVKBD_D_2X2:
	//	{
	//		return KEY_VIRTUAL_2X2;
	//		break;
	//	}
	//case PL14_NVKBD_L_3X3:
	//	{
	//		return KEY_VIRTUAL_3X3;
	//		break;
	//	}
	//case PL14_NVKBD_R_4X4:
	//	{
	//		return KEY_VIRTUAL_4X4;
	//		break;
	//	}

	//	//+/-
	//case PL14_NVKBD_ADD:
	//	{
	//		return KEY_VIRTUAL_ADD;
	//		break;
	//	}
	//case PL14_NVKBD_DEC:
	//	{
	//		return KEY_VIRTUAL_DEC;
	//		break;
	//	}

	case PL14_NVKBD_SEL:
		{
			return KEY_VIRTUAL_AUDIO;
			break;
		}

	case PL14_NVKBD_FLY:
		{
			return GetFlyCommand(KEY_MODE_PTZ, param);
			break;
		}
	case PL14_NVKBD_FLY_IN:
		{
			return GetFlyInCommand(KEY_MODE_PTZ, param);
			break;
		}
	default:
		{
			return 0;
			break;
		}
	}
}

unsigned long CKeyPL14NVKBD::PlaybkProc( unsigned long value, unsigned long param/* = 0*/ )
{
	switch (value)
	{
	case PL14_NVKBD_NUM:
		{
			return GetInputNumber(KEY_MODE_PLAYBACK, param);
			break;
		}
		//功能键
	case PL14_NVKBD_ENTER:
		{
			return KEY_VIRTUAL_ENTER;
			break;
		}
	case PL14_NVKBD_ESC:
		{
			return KEY_VIRTUAL_ESC;
			break;
		}
	case PL14_NVKBD_MENU:
		{
			return KEY_VIRTUAL_MENU;
			break;
		}

		//录像和播放控制
	case PL14_NVKBD_RECORD:
		{
			return KEY_VIRTUAL_RECORD;
			break;
		}
	case PL14_NVKBD_PLAY_STOP:
		{
			return KEY_VIRTUAL_STOP;
			break;
		}
	case PL14_NVKBD_PLAY_SF:
		{
			return KEY_VIRTUAL_PLAY;
			break;
		}

	case PL14_NVKBD_PLAY_SR:
		{
			return KEY_VIRTUAL_SR;
			break;
		}
	case PL14_NVKBD_PLAY_FF:
		{
			return KEY_VIRTUAL_FF;
			break;
		}

	case PL14_NVKBD_PALY_REW:
		{
			return KEY_VIRTUAL_REW;
			break;
		}
	case PL14_NVKBD_PLAY:
		{
			return KEY_VIRTUAL_SF;
			break;
		}

	case PL14_NVKBD_SPOT:
		{
			//return KEY_VIRTUAL_SEQ;
			return 0;
		}
	case PL14_NVKBD_GROUP:
		{
			return GetSplitMode(param);
			break;
		}

	case PL14_NVKBD_SEQ:
		{
			return KEY_VIRTUAL_SEQ;
			break;
		}
	case PL14_NVKBD_BACKUP:
		{
			return KEY_VIRTUAL_BACKUP;
			break;
		}
	case PL14_NVKBD_ENTER_PTZ:
		{
			return KEY_VIRTUAL_PTZ;
			break;
		}
	case PL14_NVKBD_SEARCH:
		{
			return KEY_VIRTUAL_SEARCH;
			break;
		}

	case PL14_NVKBD_UP:
		{
			return KEY_VIRTUAL_UP;
			break;
		}
	case PL14_NVKBD_DOWN:
		{
			return KEY_VIRTUAL_DOWN;
			break;
		}
	case PL14_NVKBD_LEFT:
		{
			return KEY_VIRTUAL_LEFT;
			break;
		}
	case PL14_NVKBD_RIGHT:
		{
			return KEY_VIRTUAL_RIGHT;
			break;
		}

// 	case PL14_NVKBD_U_1X1:
// 		{
// 			return KEY_VIRTUAL_1X1;
// 			break;
// 		}
// 	case PL14_NVKBD_D_2X2:
// 		{
// 			return KEY_VIRTUAL_2X2;
// 			break;
// 		}
// 	case PL14_NVKBD_L_3X3:
// 		{
// 			return KEY_VIRTUAL_3X3;
// 			break;
// 		}
// 	case PL14_NVKBD_R_4X4:
// 		{
// 			return KEY_VIRTUAL_4X4;
// 			break;
// 		}
// 
// 		//+/-
// 	case PL14_NVKBD_ADD:
// 		{
// 			return KEY_VIRTUAL_ADD;
// 			break;
// 		}
// 	case PL14_NVKBD_DEC:
// 		{
// 			return KEY_VIRTUAL_DEC;
// 			break;
// 		}


	case PL14_NVKBD_SEL:
		{
			return KEY_VIRTUAL_AUDIO;
			break;
		}
	case PL14_NVKBD_FLY:
		{
			return GetFlyCommand(KEY_MODE_PLAYBACK, param);
			break;
		}
	case PL14_NVKBD_FLY_IN:
		{
			return GetFlyInCommand(KEY_MODE_PLAYBACK, param);
			break;
		}

	default:
		{
			return 0;
			break;
		}
	}
}

unsigned long CKeyPL14NVKBD::FuncProc( unsigned long value, unsigned long param/* = 0*/ )
{
	switch (value)
	{
	case PL14_NVKBD_NUM:
		{
			return GetInputNumber(KEY_MODE_FUNC, param);
			break;
		}
		//功能键
	case PL14_NVKBD_ENTER:
		{
			return KEY_VIRTUAL_ENTER;
			break;
		}
	case PL14_NVKBD_ESC:
		{
			return KEY_VIRTUAL_ESC;
			break;
		}
 	case PL14_NVKBD_MENU:
 		{
			return KEY_VIRTUAL_MENU;
 			break;
 		}

		//录像和播放控制
	case PL14_NVKBD_RECORD:
		{
			return KEY_VIRTUAL_RECORD;
			break;
		}
	case PL14_NVKBD_PLAY_STOP:
		{
			return KEY_VIRTUAL_STOP;
			break;
		}
	case PL14_NVKBD_PLAY_SF:
		{
			return KEY_VIRTUAL_PLAY;
			break;
		}

	case PL14_NVKBD_PLAY_SR:
		{
			return KEY_VIRTUAL_SR;
			break;
		}
	case PL14_NVKBD_PLAY_FF:
		{
			return KEY_VIRTUAL_FF;
			break;
		}

	case PL14_NVKBD_PALY_REW:
		{
			return KEY_VIRTUAL_REW;
			break;
		}
	case PL14_NVKBD_PLAY:
		{
			return KEY_VIRTUAL_PLAY;
			break;
		}

	case PL14_NVKBD_SPOT:
		{
			//return KEY_VIRTUAL_SEQ;
			return 0;
		}
	case PL14_NVKBD_GROUP:
		{
			return GetSplitMode(param);
			break;
		}


	case PL14_NVKBD_SEQ:
		{
			return KEY_VIRTUAL_SEQ;
			break;
		}
	case PL14_NVKBD_BACKUP:
		{
			return KEY_VIRTUAL_BACKUP;
			break;
		}
	case PL14_NVKBD_ENTER_PTZ:
		{
			return KEY_VIRTUAL_PTZ;
			break;
		}
	case PL14_NVKBD_SEARCH:
		{
			return KEY_VIRTUAL_SEARCH;
			break;
		}

// 	case PL14_NVKBD_UP:
// 		{
// 			return KEY_VIRTUAL_UP;
// 			break;
// 		}
// 	case PL14_NVKBD_DOWN:
// 		{
// 			return KEY_VIRTUAL_DOWN;
// 			break;
// 		}
// 	case PL14_NVKBD_LEFT:
// 		{
// 			return KEY_VIRTUAL_LEFT;
// 			break;
// 		}
// 	case PL14_NVKBD_RIGHT:
// 		{
// 			return KEY_VIRTUAL_RIGHT;
// 			break;
// 		}

	case PL14_NVKBD_U_1X1:
		{
			return KEY_VIRTUAL_1X1;
			break;
		}
	case PL14_NVKBD_D_2X2:
		{
			return KEY_VIRTUAL_2X2;
			break;
		}
	case PL14_NVKBD_L_3X3:
		{
			return KEY_VIRTUAL_3X3;
			break;
		}
	case PL14_NVKBD_R_4X4:
		{
			return KEY_VIRTUAL_4X4;
			break;
		}

		//+/-
// 	case PL14_NVKBD_ADD:
// 		{
// 			return KEY_VIRTUAL_ADD;
// 			break;
// 		}
// 	case PL14_NVKBD_DEC:
// 		{
// 			return KEY_VIRTUAL_DEC;
// 			break;
// 		}

	case PL14_NVKBD_SEL:
		{
			return KEY_VIRTUAL_AUDIO;
			break;
		}
	case PL14_NVKBD_FLY:
		{
			return GetFlyCommand(KEY_MODE_FUNC, param);
			break;
		}
	case PL14_NVKBD_FLY_IN:
		{
			return GetFlyInCommand(KEY_MODE_FUNC, param);
			break;
		}

	default:
		{
			return 0;
			break;
		}
	}
}
