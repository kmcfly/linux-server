#ifndef _KEY_PROC_H_
#define _KEY_PROC_H_

#include "KeyLocal1U.h"
#include "KeyLocal1US.h"
#include "KeyLocal1UTW.h"
#include "KeyLocal2U.h"
#include "KeyLocal2UD.h"
#include "KeyRemote.h"
#include "KeyMouse.h"
#include "KeyMY1003.h"
#include "KeyTWOEM485.h"
#include "KeyPL14NVKBD.h"
#include "KeyTWSY.h"
#include "KeyRemoteTR12.h"

#include "gui.h"

class CKeyProc:public CKeyRemote, public CKeyLocal1U, public CKeyLocal1US,public CKeyLocal1UTW, public CKeyLocal2U, public CKeyLocal2UD, public CKeyMouse, public CKeyMY1003, public CKeyTWOEM485, public CKeyPL14NVKBD, public CKeyTWSY, public CKeyRemoteTR12
{
public:
	~CKeyProc();
	CKeyProc();
	
	inline static void SetKeyboardType(KEYBOARD_TYPE type) {m_keyboardType = type;}
	static void SetVideoInputNum(unsigned char videoInputNum);

	static unsigned long TranslateKey(KEY_MODE mod, KEY_VALUE_INFO key);	
	static unsigned long CheckKey(unsigned long value);

	//
private:
	static KEYBOARD_TYPE m_keyboardType;
	static unsigned char m_svideoInputNum;
};


#endif  //_KEY_PROC_H_










