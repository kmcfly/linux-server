//////////////////////////////////////////////////////////////////////
#ifndef _KEY_LOCAL_1US_H_
#define _KEY_LOCAL_1US_H_
#include "LocalKeyValue.h"
#include "KeyValue.h"

class CKeyLocal1US
{
public:
	CKeyLocal1US();
	virtual ~CKeyLocal1US();

	static unsigned char s_videoInputNum;
protected:
	static unsigned long TranKeyLocal1US(KEY_MODE mod, bool isDown, unsigned long value);

private:
	static unsigned long UIProc( bool isDown, unsigned long value);
	static unsigned long PtzProc( bool isDown, unsigned long value);
	static unsigned long PlaybkProc( bool isDown, unsigned long value);
	static unsigned long FuncProc( bool	isDown, unsigned long value);
};

#endif // _KEY_LOCAL_1U_H_
