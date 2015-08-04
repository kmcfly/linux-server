#ifndef PTZDSCP_H
#define PTZDSCP_H

#include "PTZProtocol.h"
#undef COMMANDLEN
#define COMMANDLEN 6

class CPTZDscp : public CPTZProtocol  
{
public:
	bool PresetSet(BYTE ucPresetID);
	bool PresetGo(BYTE ucPresetID);
	CPTZDscp();
	virtual ~CPTZDscp();
	bool IrisClose();
	bool IrisOpen();
	BYTE GetCheckSum();
	bool Stop();
	bool ZoomOut();
	bool ZoomIn();
	bool Far();
	bool Near();
	bool Down();
	bool Up();
	bool Right();
	bool Left();
	
	//hby 增加四个方向
	bool LeftDown();
	bool LeftUp();
	bool RightDown();
	bool RightUp();
	
	BYTE ucCommand[COMMANDLEN];
private:
	BYTE GetTiltSpeed();
	BYTE GetPanSpeed();

};

#endif

