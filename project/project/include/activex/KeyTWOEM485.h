/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : zhl
** Date         : 2010-06-18
** Name         : KeyTWOEM485.h
** Version      : 1.0
** Description  : 
** Modify Record:
1:
***********************************************************************/


#ifndef _KEY_TWOEM485_H_
#define _KEY_TWOEM485_H_

#include "KeyValue.h"


class CKeyTWOEM485
{
public:
	CKeyTWOEM485();
	virtual ~CKeyTWOEM485();

	static unsigned char s_videoInputNum;
protected:
	static unsigned long TranKeyTWOEM485(KEY_MODE mod, bool isDown, unsigned long value);

private:
	static unsigned long UIProc( bool isDown, unsigned long value);
	static unsigned long PtzProc( bool isDown, unsigned long value);
	static unsigned long PlaybkProc( bool isDown, unsigned long value);
	static unsigned long FuncProc( bool	isDown, unsigned long value);
};

#endif // _KEY_TWOEM485_H_
