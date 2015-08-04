#ifndef _KEY_REMOTE_TR12_H_
#define _KEY_REMOTE_TR12_H_
#include "RemoteKeyValue.h"
#include "KeyValue.h"

class CKeyRemoteTR12
{
public:
	CKeyRemoteTR12();
	virtual ~CKeyRemoteTR12();	

	static unsigned char s_videoInputNum;
protected:
	static unsigned long TranKeyRemoteTR12(KEY_MODE mod, bool isDown, unsigned long value);

private:
	static unsigned long UIProc(bool isDown, unsigned long value);
	static unsigned long PtzProc(bool isDown, unsigned long value);
	static unsigned long PlaybkProc(bool isDown, unsigned long value);
	static unsigned long FuncProc(bool isDown, unsigned long value);
};

#endif // _KEY_REMOTE_TR12_H_
