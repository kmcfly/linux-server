//////////////////////////////////////////////////////////////////////
#ifndef _KEY_LOCAL_2U_H_
#define _KEY_LOCAL_2U_H_
#include "LocalKeyValue.h"
#include "KeyValue.h"

class CKeyLocal2U 
{
public:
	CKeyLocal2U();
	virtual ~CKeyLocal2U();

	static unsigned char s_videoInputNum;
protected:
	static unsigned long TranKeyLocal(KEY_MODE mod, bool isDown, unsigned long value);

private:
	static unsigned long UIProc( bool isDown, unsigned long value);
	static unsigned long PtzProc( bool isDown, unsigned long value);
	static unsigned long PlaybkProc( bool isDown, unsigned long value);
	static unsigned long FuncProc( bool	isDown, unsigned long value);

};

#endif // _KEY_LOCAL_2U_H_
