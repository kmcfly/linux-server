#ifndef PTZVISCA_H
#define PTZVISCA_H

#include "PTZProtocol.h"
#undef	COMMANDLEN
#define	COMMANDLEN	16
#define MAXPRESET_VISCA	6	//VISCA协议当前支持的预置点数目

class CPTZVisca : public CPTZProtocol  
{
public:
	virtual int GetMaxPresetNum(){return MAXPRESET_VISCA;}
	bool PresetSet(BYTE ucPresetID);
	CPTZVisca();
	virtual ~CPTZVisca();
	bool Initial(CSerialPort *pSPort,int devAddr);

	bool AddrAssign();
	static DWORD m_dwAddrAssigned;
	bool Stop();
	bool PresetGo(BYTE ucPresetID);
	bool IrisClose();
	bool IrisOpen();
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

	virtual void GetDevAddrRange(int* min, int *max){*min = 1, *max = 7;}
	BYTE m_ucCommand[COMMANDLEN];
private:
	BYTE GetPanSpeed();
	BYTE GetTiltSpeed();
};

#endif
