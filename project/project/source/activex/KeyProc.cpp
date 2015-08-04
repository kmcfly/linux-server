#include "KeyProc.h"

using namespace GUI;

KEYBOARD_TYPE CKeyProc::m_keyboardType = KEYBOARD_1UA;
unsigned char CKeyProc::m_svideoInputNum = 0;

//////////////////////////////////////////////////////////////////////////
CKeyProc::CKeyProc() 
{
}

CKeyProc::~CKeyProc()
{
}

void CKeyProc::SetVideoInputNum(unsigned char videoInputNum) 
{
	switch(m_keyboardType)
	{
	case KEYBOARD_1UA:
	case KEYBOARD_1UB:
	case KEYBOARD_1UC:
	case KEYBOARD_1UD:
	case KEYBOARD_1UE:
	case KEYBOARD_1UF:
	case KEYBOARD_1UG:
	case KEYBOARD_1UT:
	case KEYBOARD_SUA:
	case KEYBOARD_SUB:
	case KEYBOARD_SUC:
	case KEYBOARD_1USS:
	case KEYBOARD_1UV:
		{
			CKeyLocal1U::s_videoInputNum=videoInputNum;
		}
		break;
	case KEYBOARD_1US:
		{
			CKeyLocal1US::s_videoInputNum=videoInputNum;
		}
		break;
	case KEYBOARD_1UTW:
		{
			CKeyLocal1UTW::s_videoInputNum=videoInputNum;
		}
		break;
	case KEYBOARD_2UA:
	case KEYBOARD_2UB:
	case KEYBOARD_2UC:
		{
			CKeyLocal2U::s_videoInputNum= videoInputNum;
		}
		break;
	case KEYBOARD_2UD:
	case KEYBOARD_15U:
	case KEYBOARD_1UH:
	case KEYBOARD_1UI:	
		{
			CKeyLocal2UD::s_videoInputNum= videoInputNum;
		}
		break;
	default:
		{
			assert(false);
			CKeyLocal1U::s_videoInputNum=videoInputNum;
		}
		break;
	}

	CKeyRemote::s_videoInputNum = videoInputNum;
	CKeyMY1003::s_videoInputNum = videoInputNum;
	m_svideoInputNum = videoInputNum;
}


unsigned long CKeyProc::TranslateKey(KEY_MODE mod, KEY_VALUE_INFO key)
{
	long ret = 0;
	
	if(key.type == KEY_TYPE_LOCAL)
	{
		switch( m_keyboardType )
		{
		case KEYBOARD_1UA:
		case KEYBOARD_1UB:
		case KEYBOARD_1UC:
		case KEYBOARD_1UD:
		case KEYBOARD_1UE:
		case KEYBOARD_1UF:
		case KEYBOARD_1UG:
		case KEYBOARD_1UT:
		case KEYBOARD_SUA:
		case KEYBOARD_SUB:
		case KEYBOARD_SUC:
		case KEYBOARD_1USS:
		case KEYBOARD_1UV:
			ret = CKeyLocal1U::TranKeyLocal(mod, key.isDown, key.value);	
			break;			
		case KEYBOARD_1US:
			ret = TranKeyLocal1US(mod, key.isDown, key.value);
			break;
		case KEYBOARD_1UTW:
			ret = TranKeyLocal1UTW(mod, key.isDown, key.value);
			break;			
		case KEYBOARD_2UA:
		case KEYBOARD_2UB:
		case KEYBOARD_2UC:
			ret = CKeyLocal2U::TranKeyLocal(mod, key.isDown, key.value);
			break;
		case KEYBOARD_2UD:
		case KEYBOARD_15U:
		case KEYBOARD_1UH:
		case KEYBOARD_1UI:		
			ret = CKeyLocal2UD::TranKeyLocal(mod, key.isDown, key.value);
			break;
		default:
			break;
		}
		//长按键不处理
		if(key.isDown)
		{
			switch(ret)
			{
#if !defined(__TW01_RILI__) 
				case KEY_VIRTUAL_REDRAW:
					break;
#endif
				case KEY_VIRTUAL_LCD_ON_OFF:
					break;
				default:
					ret = 0;
					break;
			}
			
		}
	}
	else if(key.type == KEY_TYPE_REMOTE)
	{
		int temp = TranKeyRemote(mod, key.isDown, key.value);
		ret = CheckKey(temp);
	} 
	else if(key.type == KEY_TYPE_MOUSE)
	{
		ret = TranKeyMouse(mod, key.value);
	}
	else if( KEY_TYPE_MY1003 == key.type )
	{
		int temp = TranKeyMY1003( mod, key.value );
		ret = temp;  //CheckKey(temp);
	}
	else if( KEY_TYPE_TWOEM == key.type )
	{
		int temp = TranKeyTWOEM485( mod, key.isDown, key.value );
		ret = CheckKey(temp);
	}
	else if ( KEY_TYPE_PL14_NVKBD30_70 == key.type)
	{
		int temp = TranKeyPL14NVKBD( mod, key.value );
		ret = CheckKey(temp);
	}
	else if (KEY_TYPE_TWSY == key.type)
	{
		int temp = TranKeyTWSY(mod, key.isDown, key.value);
		ret = CheckKey(temp);
	}
	else if (KEY_TYPE_REMOTE_TR12 == key.type)
	{
		int temp = TranKeyRemoteTR12(mod, key.isDown, key.value);
		ret = CheckKey(temp);
	}
	else/* if(key.type == KEY_TYPE_VIRTUAL)*/
	{
		ret = key.value;
	}
	
	return ret;  //////与模式无关的键值
}



unsigned long CKeyProc::CheckKey(unsigned long value) 
{
	if (value == KEY_VIRTUAL_2X3)
	{
		if (m_svideoInputNum < 6)
		{
			return 0;
		}
		else
		{
			return value;
		}
	}
	else if (value == KEY_VIRTUAL_3X3) 
	{
		if (m_svideoInputNum < 8)
		{
			return 0;
		}
		else
		{
			return value;
		}
	}
	else if (value == KEY_VIRTUAL_4X4)
	{
		if (m_svideoInputNum < 16)
		{
			return 0;
		}
		else
		{
			return value;
		}
	}
	else if (value == KEY_VIRTUAL_5X5)
	{
		if (m_svideoInputNum < 24)
		{
			return 0;
		}
		else
		{
			return value;
		}
	}
	else if (value == KEY_VIRTUAL_6X6)
	{
		if (m_svideoInputNum < 32)
		{
			return 0;
		}
		else
		{
			return value;
		}
	}
	else
	{
		return value;
	}

}



