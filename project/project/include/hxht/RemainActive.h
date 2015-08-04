/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : zhl
** Date         : 2010-07-28
** Name         : RemainActive.h
** Version      : 1.0
** Description  :
				

** Modify Record:

***********************************************************************/

#ifndef _REMAIN_ACTIVE_H_
#define _REMAIN_ACTIVE_H_

#include "HxGlobal.h"
#include "HxMsgMan.h"

class CRemainActive
{
public:
	static CRemainActive *Instance();
	~CRemainActive();

protected:
private:
	CRemainActive();
	

};
#endif /*_REMAIN_ACTIVE_H_*/

