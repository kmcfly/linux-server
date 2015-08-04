/*
* Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
* Author       : xianshiwei
* Date         : 2015-05-27
* Name         : PtzAhd.h
* Version      : 1.0
* Description  : 

* Modify Record:

*/
#ifndef __PTZ_AHD___
#define __PTZ_AHD___

#include "PTZCoaxial.h"

#undef	COMMANDLEN
#define COMMANDLEN 8

#undef	COMMANDLEN_AHD
#define COMMANDLEN_AHD 4

class CPtzAhd
:public CPTZCoaxial{
public:
	CPtzAhd();
	virtual ~ CPtzAhd();

	virtual bool Left();
	virtual bool Right();
	virtual bool Up();
	virtual bool Down();
	virtual bool LeftDown();
	virtual bool LeftUp();
	virtual bool RightDown();
	virtual bool RightUp();

	virtual bool Near();
	virtual bool Far();
	virtual bool ZoomOut();
	virtual bool ZoomIn();
	virtual bool IrisOpen();
	virtual bool IrisClose();

	virtual bool Stop();

#if 0
	virtual bool PresetGo(BYTE ucPresetID) = 0;
	virtual bool PresetSet(BYTE ucPresetID) = 0;
	virtual bool PresetClear(BYTE ucPresetID);

	virtual bool RunCruise(BYTE uCruiseId);
	virtual bool StopCruise();				
	virtual bool DeleteCruise(BYTE uCruiseId);
	virtual bool EnterCruiseMode(BYTE uCruiseId);		
	virtual bool ConfigCruise(BYTE ucPresetID, BYTE speed, BYTE waitTime);
	virtual bool LeaveCruiseMode(BYTE uCruiseId);				
	virtual bool ConfigureCruiseDisable();

	virtual bool Record_Track();
	virtual bool Stop_Record_Track();
	virtual bool Start_Track();
	virtual bool Stop_Track();

	virtual bool Auto_Scan();
	virtual bool Stop_Auto_Scan();

	virtual bool PopPTZMenu();

	virtual bool SetAuxiliary(BYTE auxNum);
	virtual bool ClearAuxiliary(BYTE auxNum);

#endif

};
#endif