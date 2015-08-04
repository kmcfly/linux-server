//////////////////////////////////////////////////////////////////////
#ifndef _KEY_LOCAL_1UTW01_H_
#define _KEY_LOCAL_1UTW01_H_
#include "LocalKeyValue.h"
#include "KeyValue.h"

class CKeyLocal1UTW
{
public:
	CKeyLocal1UTW();
	virtual ~CKeyLocal1UTW();

	static unsigned char s_videoInputNum;
protected:
	static unsigned long TranKeyLocal1UTW(KEY_MODE mod, bool isDown, unsigned long value);

private:
	static unsigned long UIProc( bool isDown, unsigned long value);
	static unsigned long PtzProc( bool isDown, unsigned long value);
	static unsigned long PlaybkProc( bool isDown, unsigned long value);
	static unsigned long FuncProc( bool	isDown, unsigned long value);
};

#endif // _KEY_LOCAL_1UTW_H_
