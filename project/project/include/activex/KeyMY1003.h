/*****************************************************************
 Copyright (C),2009-2012, tongwei video Tech. Co., Ltd.

 File name: KeyMY1003.h

 Author: TVT-ZHL-146  Version: 1.0	Date:2009-08-14

 Description:

 Others:

 Function List:

 History:
 
 	1.Date:

	  Author:

	  Modification:	
	2.
******************************************************************/

#ifndef _KEY_MY1003_H
#define _KEY_MY1003_H

#include "KeyValue.h"

const unsigned long KEY_MY1003_POS = 10000;

class CKeyMY1003
{
public:
	CKeyMY1003();
	virtual ~CKeyMY1003();
	static unsigned char s_videoInputNum;

protected:
	static unsigned long TranKeyMY1003(KEY_MODE mod, unsigned long value);

private:
	static unsigned long UIProc( unsigned long value );
	static unsigned long PtzProc( unsigned long value );
	static unsigned long PlaybkProc( unsigned long value );
	static unsigned long FuncProc( unsigned long value );

};

#endif // 
