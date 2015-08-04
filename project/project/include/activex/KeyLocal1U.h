//////////////////////////////////////////////////////////////////////
#ifndef _KEY_LOCAL_1U_H_
#define _KEY_LOCAL_1U_H_
#include "LocalKeyValue.h"
#include "KeyValue.h"

class CKeyLocal1U 
{
public:
	CKeyLocal1U();
	virtual ~CKeyLocal1U();

	static unsigned char s_videoInputNum;
protected:
	static unsigned long TranKeyLocal(KEY_MODE mod, bool isDown, unsigned long value);

private:
	static unsigned long UIProc( bool isDown, unsigned long value);
	static unsigned long PtzProc( bool isDown, unsigned long value);
	static unsigned long PlaybkProc( bool isDown, unsigned long value);
	static unsigned long FuncProc( bool	isDown, unsigned long value);
};

#endif // _KEY_LOCAL_1U_H_
