
#ifndef PTZMINIKING_H
#define PTZMINIKING_H


#include "PTZProtocol.h"
#undef COMMANDLEN
#define COMMANDLEN 11

class CPTZMinking : public CPTZProtocol  
{
public:
	virtual bool PresetSet(BYTE ucPresetID);
	virtual bool PresetGo(BYTE ucPresetID);
	CPTZMinking();
	virtual ~CPTZMinking();

	bool Stop();
	
	bool IrisOpen();
	bool IrisClose();
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
	
	BYTE GetCheckSum();
	BYTE ucCommand[COMMANDLEN];
	virtual void GetDevAddrRange(int* min, int *max){*min = 0x00, *max = 0x1F;}

	//zhoufeng 2007-12-11 添加巡航功能
	bool RunCruise(BYTE uCruiseId);		//运行巡航
	bool StopCruise();					//停止运行巡航
	bool DeleteCruise(BYTE uCruiseId);  //删除巡航运轨迹线
	bool EnterCruiseMode(BYTE uCruiseId);							//进入巡航配置模式
	bool ConfigCruise(BYTE ucPresetID, BYTE speed, BYTE waitTime);	//设置巡航速度
	bool LeaveCruiseMode(BYTE uCruiseId);							//退出巡航配置模式

	//查询是否支持巡航
	bool IsSupportCruise();

	//辅助开关控制
	bool SetAuxiliary(BYTE auxNum);
	bool ClearAuxiliary(BYTE auxNum);

private:
	BYTE GetTiltSpeed();
	BYTE GetPanSpeed();
	BYTE m_CurrentCruiseId;
};

#endif

