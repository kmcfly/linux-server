#include "KeyMouse.h"

CKeyMouse::CKeyMouse()
{
}

CKeyMouse::~CKeyMouse()
{
}

unsigned long CKeyMouse::FuncProc(unsigned long value)
{
	switch(value)
	{
	case MOUSE_KEY_MOUSE_MOVE:
		return KEY_VIRTUAL_MOVE;
	case MOUSE_KEY_LEFT_DOWN:
		return KEY_VIRTUAL_ENTER;
	case MOUSE_KEY_LEFT_DOWN_MOVE:
		return KEY_VIRTUAL_DRAG;
	case MOUSE_KEY_LEFT_UP:
		return KEY_VIRTUAL_BLUP;
	case MOUSE_KEY_LEFT_DBCLICK:
		return KEY_VIRTUAL_DBCLICK;
	case MOUSE_KEY_RIGHT_DOWN:
		return KEY_VIRTUAL_EXIT;
	case MOUSE_KEY_WHEEL_FORWARD:
		return KEY_VIRTUAL_ADD;
	case MOUSE_KEY_WHEEL_BACK:
		return KEY_VIRTUAL_DEC;
	default:
		return 0;
	}
}

unsigned long CKeyMouse::UIProc(unsigned long value)
{
	switch(value)
	{
	case MOUSE_KEY_MOUSE_MOVE:
		return KEY_VIRTUAL_MOVE;
	case MOUSE_KEY_LEFT_DOWN:
		return KEY_VIRTUAL_ENTER;
	case MOUSE_KEY_LEFT_DOWN_MOVE:
		return KEY_VIRTUAL_DRAG;
	case MOUSE_KEY_LEFT_UP:
		return KEY_VIRTUAL_BLUP;
	case MOUSE_KEY_LEFT_DBCLICK:
		return KEY_VIRTUAL_DBCLICK;
	case MOUSE_KEY_RIGHT_DOWN:
		return KEY_VIRTUAL_EXIT;
	case MOUSE_KEY_WHEEL_FORWARD:
		return KEY_VIRTUAL_ADD;
	case MOUSE_KEY_WHEEL_BACK:
		return KEY_VIRTUAL_DEC;
	default:
		return 0;
	}
}

unsigned long CKeyMouse::PtzProc(unsigned long value)
{
	switch(value)
	{
	case MOUSE_KEY_MOUSE_MOVE:
		return KEY_VIRTUAL_MOVE;
	case MOUSE_KEY_LEFT_DOWN:
		return KEY_VIRTUAL_ENTER;
	case MOUSE_KEY_LEFT_DOWN_MOVE:
		return KEY_VIRTUAL_DRAG;
	case MOUSE_KEY_LEFT_UP:
		return KEY_VIRTUAL_BLUP;
	case MOUSE_KEY_LEFT_DBCLICK:
		return KEY_VIRTUAL_DBCLICK;
	case MOUSE_KEY_RIGHT_DOWN:
		return KEY_VIRTUAL_HIDE;
		//return KEY_VIRTUAL_EXIT;
	case MOUSE_KEY_WHEEL_FORWARD:
		return KEY_VIRTUAL_ADD;
	case MOUSE_KEY_WHEEL_BACK:
		return KEY_VIRTUAL_DEC;
	default:
		return 0;
	}
}

unsigned long CKeyMouse::PlaybkProc(unsigned long value)
{
	switch(value)
	{
	case MOUSE_KEY_MOUSE_MOVE:
		return KEY_VIRTUAL_MOVE;
	case MOUSE_KEY_LEFT_DOWN:
		return KEY_VIRTUAL_ENTER;
	case MOUSE_KEY_LEFT_DOWN_MOVE:
		return KEY_VIRTUAL_DRAG;
	case MOUSE_KEY_LEFT_UP:
		return KEY_VIRTUAL_BLUP;
	case MOUSE_KEY_LEFT_DBCLICK:
		return KEY_VIRTUAL_DBCLICK;
	case MOUSE_KEY_RIGHT_DOWN:
		return KEY_VIRTUAL_HIDE;
		//return KEY_VIRTUAL_EXIT;
	case MOUSE_KEY_WHEEL_FORWARD:
		return KEY_VIRTUAL_ADD;
	case MOUSE_KEY_WHEEL_BACK:
		return KEY_VIRTUAL_DEC;
	default:
		return 0;
	}
}

unsigned long CKeyMouse::TranKeyMouse(KEY_MODE mod, unsigned long value)
{
	switch(mod) 
	{
	case KEY_MODE_PTZ:
		return PtzProc(value);

	case KEY_MODE_PLAYBACK:
		return PlaybkProc(value);

	case KEY_MODE_FUNC:
		return FuncProc(value);

	case KEY_MODE_UI:
		return UIProc(value);

	default:
		break;
	}	
	return 0;
}
