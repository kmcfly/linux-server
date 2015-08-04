#ifndef _KEY_MOUSE_H_
#define _KEY_MOUSE_H_
#include "MouseKeyValue.h"
#include "KeyValue.h"

class CKeyMouse
{
public:
	CKeyMouse();
	virtual ~CKeyMouse();

protected:

	static unsigned long TranKeyMouse(KEY_MODE mod, unsigned long value);
private:
	static unsigned long UIProc(unsigned long value);
	static unsigned long PtzProc(unsigned long value);
	static unsigned long PlaybkProc(unsigned long value);
	static unsigned long FuncProc(unsigned long value);
};

#endif  ///_KEY_MOUSE_H_















