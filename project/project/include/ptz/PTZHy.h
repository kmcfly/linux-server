
#ifndef PTZHY_H
#define PTZHY_H

#include "PTZProtocol.h"
#undef	COMMANDLEN
#define COMMANDLEN 10

class CPTZHy : public CPTZProtocol
{
public:
	bool PresetPrepare();
	CPTZHy();
	virtual ~CPTZHy();

	BYTE m_ucCommand[COMMANDLEN];
	BYTE GetCheckSum();
	
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

	//获取该云台协议地址范围 <1>
//	virtual void GetDevAddrRange(int* min, int *max){*min = 0x00, *max = 0x1F;}

	virtual bool PresetGo(BYTE ucPresetID);
	virtual bool PresetSet(BYTE ucPresetID);

	//查询是否支持巡航
	bool IsSupportCruise();

protected:
	
private:
	BYTE m_cmd;
	BYTE GetTiltSpeed();
	BYTE GetPanSpeed();
};

#endif

