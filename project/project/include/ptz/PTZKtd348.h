#ifndef __PTZKTD348_H__
#define __PTZKTD348_H__

#include "PTZProtocol.h"

#undef  COMMANDLEN
#define COMMANDLEN 18

class CPTZKtd348 : public CPTZProtocol  
{
public:

	CPTZKtd348();
	virtual ~CPTZKtd348();
	void Initial();

	bool ZoomIn();
	bool ZoomOut();
	bool Near();
	bool Far();
	bool IrisClose();
	bool IrisOpen();

	bool Up();
	bool Down();
	bool Right();
	bool Stop();
	bool Left();

	bool LeftDown(){return false;}
	bool LeftUp(){return false;}
	bool RightDown(){return false;}
	bool RightUp(){return false;}

	bool PresetGo(unsigned char ucPresetID){return false;}
	bool PresetSet(unsigned char ucPresetID){return false;}

private:
	static PUB_THREAD_RESULT PUB_THREAD_CALL WriteCmdThread(void * pParam);
	void WriteCmdProc();
private:
	unsigned char   m_ucCommand[COMMANDLEN];
	bool            m_bWriteProc;
	bool			m_bSendCmd;
	PUB_thread_t    m_thread;
	CPUB_Lock       m_lock;
};

#endif

