#ifndef _KEY_REMOTE_H_
#define _KEY_REMOTE_H_
#include "RemoteKeyValue.h"
#include "KeyValue.h"

class CKeyRemote  
{
public:
	CKeyRemote();
	virtual ~CKeyRemote();	

	static unsigned char s_videoInputNum;
protected:
	static unsigned long TranKeyRemote(KEY_MODE mod, bool isDown, unsigned long value);

private:
	static unsigned long UIProc(bool isDown, unsigned long value);
	static unsigned long PtzProc(bool isDown, unsigned long value);
	static unsigned long PlaybkProc(bool isDown, unsigned long value);
	static unsigned long FuncProc(bool isDown, unsigned long value);
};

#endif // _KEY_REMOTE_H_
