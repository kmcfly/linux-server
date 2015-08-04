

#ifndef PTZPELCOD_H
#define PTZPELCOD_H



#include "PTZProtocol.h"
#undef  COMMANDLEN
#define COMMANDLEN 7

class CPTZPelcoD : public CPTZProtocol  
{
public:

	CPTZPelcoD();
	virtual ~CPTZPelcoD();
	void AutoScan(int nGroupID);

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
	BYTE m_ucCommand[COMMANDLEN];
	BYTE GetCheckSum();
	bool Left();

	//hby 增加四个方向
	bool LeftDown();
	bool LeftUp();
	bool RightDown();
	bool RightUp();
	
	//hby 使用PELCO_D 自己的巡航功能
//	bool Command(BYTE type,BYTE cmd,BYTE ptzaddr,int baudrate,BYTE speed,BYTE waitTime);

	virtual bool PresetGo(BYTE ucPresetID);
	virtual bool PresetSet(BYTE ucPresetID);

	//zhoufeng 2007-12-11 添加巡航功能
	bool RunCruise(BYTE uCruiseId);		//运行巡航
	bool StopCruise();					//停止运行巡航
	bool DeleteCruise(BYTE uCruiseId);  //删除巡航运轨迹线
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

	//控制辅助开关
	bool SetAuxiliary(BYTE auxNum);
	bool ClearAuxiliary(BYTE auxNum);

private:
	BYTE GetTiltSpeed();
	BYTE GetPanSpeed();
};

#endif

