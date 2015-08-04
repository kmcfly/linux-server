
#ifndef PTZSAMSUNG_H
#define PTZSAMSUNG_H

#include "PTZProtocol.h"
#undef	COMMANDLEN
#define COMMANDLEN 9

class CPTZSamsung : public CPTZProtocol
{
public:
	bool PresetPrepare();
	CPTZSamsung();
	virtual ~CPTZSamsung();

	BYTE m_ucCommand[COMMANDLEN];
	BYTE GetCheckSum();
	
	bool Stop();
	bool Left();
	bool Right();
	bool Up();
	bool Down();
//hby 
	bool LeftDown();
	bool LeftUp();
	bool RightDown();
	bool RightUp();
	
	bool Near();
	bool Far();
	bool ZoomOut();
	bool ZoomIn();
	bool IrisOpen();
	bool IrisClose();
	//获取该云台协议地址范围 <1>
	virtual void GetDevAddrRange(int* min, int *max){*min = 0x00, *max = 0x1F;}
	virtual bool PresetGo(BYTE ucPresetID);
	virtual bool PresetSet(BYTE ucPresetID);

	//zhoufeng 2007-12-11 添加巡航功能
	bool RunCruise(BYTE uCruiseId);		//运行巡航
	bool StopCruise();					//停止运行巡航
	bool DeleteCruise(BYTE uCruiseId); 	 //删除巡航运轨迹线
	bool EnterCruiseMode(BYTE uCruiseId);							//进入巡航配置模式
	bool ConfigCruise(BYTE ucPresetID, BYTE speed, BYTE waitTime);	//设置巡航速度
	bool LeaveCruiseMode(BYTE ucPresetID);							//退出巡航配置模式
//zhl113 2009-07-08添加云台轨迹和自动巡航功能
	bool Record_Track();
	bool Stop_Record_Track();
	bool Start_Track();
	bool Stop_Track();
//zhl-113 2009--7-9 添加自动扫描功能
	bool Auto_Scan();
	bool Stop_Auto_Scan();

	//查询是否支持巡航
	bool IsSupportCruise();

	//控制辅助开关， 辅助开关用来控制雨刷、灯光、和摄像头辅助开关
	bool SetAuxiliary(BYTE auxNum);
	bool ClearAuxiliary(BYTE auxNum);
	
protected:
	
private:
	BYTE GetTiltSpeed();
	BYTE GetPanSpeed();
};

#endif

