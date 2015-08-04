

#ifndef PTZLILIN_H
#define PTZLILIN_H

#include "PTZProtocol.h"
#undef	COMMANDLEN
#define COMMANDLEN 3

class CPTZLilin : public CPTZProtocol  
{
public:

	bool PresetSet(BYTE ucPresetID);
	CPTZLilin();
	virtual ~CPTZLilin();
	bool PresetGo(BYTE ucPresetID);
	BYTE m_ucCommand[COMMANDLEN];
    int m_curSpeed;
    int m_curDwell;
	bool Stop();
	bool Left();
	bool Right();
	bool Up();
	bool Down();
	bool Near();
	bool Far();
	bool ZoomOut();
	bool ZoomIn();
	bool IrisOpen();
	bool IrisClose();
	
	//hby 增加四个方向
	bool LeftDown();
	bool LeftUp();
	bool RightDown();
	bool RightUp();

	virtual void GetDevAddrRange(int* min, int *max){*min = 0x01, *max = 0x40;}//<1>
private:
	BYTE GetPanSpeed();
	BYTE GetTiltSpeed();
};

#endif


