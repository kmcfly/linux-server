/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : ZHL
** Date         : 2010-05-28
** Name         : CKeyLocal1UTW.cpp
** Version      : 1.0
** Description  :̨��01(TW01)�ͻ�ר��ǰ��嶨�塣ʹ��1Uϵ�еļ�ֵ
** Modify Record:
1:
***********************************************************************/


/***********(1UTW)���̲���-ǰ��嶨��(˿ӡͼ)*************************************************************
																		CH

RECORD	SEARCH	MENU/+	BACKUP/-	STOP(ESC)					REW		ENTER		FF

																		SPLIT
************************************************************************************************************/


#include "KeyLocal1UTW.h"

unsigned char CKeyLocal1UTW::s_videoInputNum;

CKeyLocal1UTW::CKeyLocal1UTW()
{
}

CKeyLocal1UTW::~CKeyLocal1UTW()
{

}

unsigned long CKeyLocal1UTW::TranKeyLocal1UTW(KEY_MODE mod, bool isDown, unsigned long value)
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

/***********(1UTW)���̲���-ǰ��幦��(FUNCTION)**********************************************************
																				CH

RECORD	SEARCH	MENU	BACKUP	ESC										LEFT	ENTER	RIGHT	

																				SPLIT
************************************************************************************************************/
unsigned long CKeyLocal1UTW::FuncProc( bool isDown, unsigned long value)
{
	switch(value) 
	{
	case U1_KEY_UP:
		return KEY_VIRTUAL_GROUP;
	case U1_KEY_DOWN:
		return KEY_VIRTUAL_SPLIT;
	case U1_KEY_RIGHT:
		return KEY_VIRTUAL_RIGHT;
	case U1_KEY_LEFT:
		return KEY_VIRTUAL_LEFT;
	case U1_KEY_ENTER:
		return KEY_VIRTUAL_ENTER;

	case U1_KEY_RECORD:
		return KEY_VIRTUAL_RECORD;
	case U1_KEY_SEARCH:
		return KEY_VIRTUAL_SEARCH;

	case U1_KEY_ADD:
		return isDown ? KEY_VIRTUAL_LCD_ON_OFF : KEY_VIRTUAL_MENU;
	case U1_KEY_DEC:
		return KEY_VIRTUAL_BACKUP;
	case U1_KEY_STOP:
		return isDown ? KEY_VIRTUAL_REDRAW : KEY_VIRTUAL_ESC;

	default :
		return 0;
	}
}

/***********(1UTW)���̲���-ǰ��幦��(PALY)************************************************************************
																				CH

(RECORD)	(SEARCH)	HIDE	(BACKUP)	(STOP)ESC					REW		PLAY		FF

																				SPLIT
��ģʽ�´������⣺�ڻطſ��ƽ���ȷ����ťΪPLAY���ܣ�������RECORD������ȷ������
************************************************************************************************************/
unsigned long CKeyLocal1UTW::PlaybkProc( bool isDown,unsigned long value)
{
	switch(value) 
	{

	case U1_KEY_UP:
		return KEY_VIRTUAL_GROUP;
	case U1_KEY_DOWN:
		return KEY_VIRTUAL_SPLIT;
	case U1_KEY_RIGHT:
		return KEY_VIRTUAL_FF;
	case U1_KEY_LEFT:
		return KEY_VIRTUAL_REW;
	case U1_KEY_ENTER:
		return KEY_VIRTUAL_PLAY;

	case U1_KEY_RECORD:
		return KEY_VIRTUAL_ENTER;

	case U1_KEY_ADD:
		return  isDown ? KEY_VIRTUAL_LCD_ON_OFF : KEY_VIRTUAL_HIDE;

	case U1_KEY_STOP:
		return isDown ? KEY_VIRTUAL_REDRAW : KEY_VIRTUAL_ESC;

	default :
		return 0;
	}
}


/***********(1UTW)���̲���-ǰ��幦��(PTZ)************************************************************************
																					PTZ-UP

(RECORD)	(SEARCH)	HIDE	(BACKUP)	(STOP)ESC					PTZ-LEFT	ENTER	PTZ-RIGHT

																					PTZ-DOWN
************************************************************************************************************/
unsigned long CKeyLocal1UTW::PtzProc( bool isDown,unsigned long value)
{
	switch(value) 
	{
	case U1_KEY_UP:
		return KEY_VIRTUAL_PTZ_UP;
	case U1_KEY_DOWN:
		return KEY_VIRTUAL_PTZ_DOWN;
	case U1_KEY_RIGHT:
		return KEY_VIRTUAL_PTZ_RIGHT;
	case U1_KEY_LEFT:
		return KEY_VIRTUAL_PTZ_LEFT;
	case U1_KEY_ENTER:
		return KEY_VIRTUAL_ENTER;

	case U1_KEY_RECORD:
		return KEY_VIRTUAL_ENTER;

	case U1_KEY_ADD:
		return isDown ? KEY_VIRTUAL_LCD_ON_OFF : KEY_VIRTUAL_HIDE;

	case U1_KEY_STOP:
		return isDown ? KEY_VIRTUAL_REDRAW : KEY_VIRTUAL_ESC;

	default :
		{
			return 0;
		}
	}
}


/***********(1UTW)���̲���-ǰ��幦��(UI)************************************************************************
																			UP

RECORD	SEARCH	(MENU/)+	(BACKUP/)-	(STOP)ESC					LEFT	ENTER	RIGHT

																			DOWN
************************************************************************************************************/
unsigned long CKeyLocal1UTW::UIProc( bool isDown, unsigned long value)
{
	switch(value) 
	{
	case U1_KEY_UP:
		return KEY_VIRTUAL_UP;
	case U1_KEY_DOWN:
		return KEY_VIRTUAL_DOWN;
	case U1_KEY_RIGHT:
		return KEY_VIRTUAL_RIGHT;
	case U1_KEY_LEFT:
		return KEY_VIRTUAL_LEFT;
	case U1_KEY_ENTER:
		return KEY_VIRTUAL_ENTER;

	case U1_KEY_RECORD:
		return KEY_VIRTUAL_RECORD;
	case U1_KEY_SEARCH:
		return KEY_VIRTUAL_SEARCH;

	case U1_KEY_ADD:
		return isDown ? KEY_VIRTUAL_LCD_ON_OFF : KEY_VIRTUAL_ADD;
	case U1_KEY_DEC:
		return KEY_VIRTUAL_DEC;
	
	case U1_KEY_STOP:
		return isDown ? KEY_VIRTUAL_REDRAW : KEY_VIRTUAL_ESC;
	default :
		{
			return 0;
		}
	}
}

