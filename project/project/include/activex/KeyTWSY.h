/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : xiaozhiwen
** Date         : 2011-04-12
** Name         : KeyTWSY.h
** Version      : 3.0
** Description  :
** Modify Record:
***********************************************************************/
#ifndef  __KEYTWSY_H__
#define __KEYTWSY_H__

#include "KeyValue.h"

class CKeyTWSY
{
public:
	CKeyTWSY();
	virtual ~CKeyTWSY();

protected:
	static unsigned long TranKeyTWSY(KEY_MODE mod, unsigned long isDown, unsigned long value);

private:
	static unsigned long UIProc(unsigned long value, unsigned long isDown);
	static unsigned long PtzProc(unsigned long value, unsigned long isDown);
	static unsigned long PlaybackProc(unsigned long value, unsigned long isDown);
	static unsigned long FuncProc(unsigned long value, unsigned long isDown);

};


#endif
