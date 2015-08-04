
#ifndef PTZTVI_H
#define PTZTVI_H

//#include "PTZProtocol.h"
#include"PTZCoaxial.h"

#undef	COMMANDLEN
#define COMMANDLEN 8


class CPTZTvi : public CPTZCoaxial
{
public:
	CPTZTvi();
	CPTZTvi(unsigned long chnn);
	virtual ~CPTZTvi();

	bool SetCurChnn(unsigned long chnn);
	unsigned long GetCurChnn();

	BYTE m_ucCommand[COMMANDLEN];
	int  m_chnn;
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

	bool PresetDelete(BYTE ucPresetID);

	//获取该云台协议地址范围 <1>
	virtual void GetDevAddrRange(int* min, int *max){*min = 0x00, *max = 0x1F;}

	virtual bool PresetGo(BYTE ucPresetID);
	virtual bool PresetSet(BYTE ucPresetID);


	bool MotorStop();
	bool SetCameraStatus(BYTE camStatus,BYTE camVersion);
	bool GetCameraStatus(BYTE* pCamStatus);

	bool StopCruise();					//停止运行巡航
#if 0	
	bool RunCruise(BYTE uCruiseId);		//运行巡航
	bool StopCruise();					//停止运行巡航
	bool DeleteCruise(BYTE uCruiseId);  //删除巡航运轨迹线
	bool EnterCruiseMode(BYTE uCruiseId);							//进入巡航配置模式
	bool ConfigCruise(BYTE ucPresetID, BYTE speed, BYTE waitTime);	//设置巡航速度
	bool LeaveCruiseMode(BYTE ucPresetID);							//退出巡航配置模式

	bool Record_Track();
	bool Stop_Record_Track();
	bool Start_Track();
	bool Stop_Track();
	bool Auto_Scan();
	bool Stop_Auto_Scan();

	//查询是否支持巡航
	bool IsSupportCruise();

	bool SetAuxiliary(BYTE auxNum);
	bool ClearAuxiliary(BYTE auxNum);
#endif

	//OSD菜单控制
	bool MenuOn();
	bool MenuLeft();
	bool MenuRight();
	bool MenuUp();
	bool MenuDown();

protected:
	
private:
	BYTE GetTiltSpeed();
	BYTE GetPanSpeed();
};

#endif
