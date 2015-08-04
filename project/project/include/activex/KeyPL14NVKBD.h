#ifndef _KEY_PL14_NVKBD_H_
#define _KEY_PL14_NVKBD_H_

#include "KeyValue.h"

class CKeyPL14NVKBD
{
public:
	CKeyPL14NVKBD();
	virtual ~CKeyPL14NVKBD();

protected:
	static unsigned long TranKeyPL14NVKBD(KEY_MODE mod, unsigned long value);

private:
	static unsigned long GetControlParam(unsigned long &controlValue, unsigned long &controlParam);
	static unsigned long GetInputNumber(KEY_MODE mode, unsigned long controlParam);
	static unsigned long GetSplitMode(unsigned long channelCount);
	static unsigned long GetFlyCommand(KEY_MODE mode, unsigned long controlParam);
	static unsigned long GetFlyInCommand(KEY_MODE mode, unsigned long controlParam);

	static unsigned long UIProc( unsigned long value, unsigned long param = 0 );
	static unsigned long PtzProc( unsigned long value, unsigned long param = 0 );
	static unsigned long PlaybkProc( unsigned long value, unsigned long param = 0 );
	static unsigned long FuncProc( unsigned long value, unsigned long param = 0 );
};
#endif /*_KEY_PL14_NVKBD_H_*/
