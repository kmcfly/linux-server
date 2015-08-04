#ifndef _PTZPL14_H_
#define _PTZPL14_H_

#include "PTZProtocol.h"
#undef  COMMANDLEN
#define COMMANDLEN 10
#define PL14_OPEN_MENU_PRESET_NUM 95
#define PL14_CLOSE_MENU_PRESET_NUM 96

class CPTZPL14 : public CPTZProtocol  
{
public:

	CPTZPL14();
	virtual ~CPTZPL14();
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
	BYTE GetCheckSum();
	BYTE GetSpeed();
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
	virtual bool PresetClear(BYTE ucPresetID);

	//zhoufeng 2007-12-11 添加巡航功能
	bool EnterCruiseMode(BYTE uCruiseId);
	bool ConfigCruise(BYTE ucPresetID, BYTE speed, BYTE waitTime);
	bool LeaveCruiseMode(BYTE uCruiseId);
	bool RunCruise(BYTE uCruiseId);		//运行巡航
	bool StopCruise();					//停止运行巡航

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
	//弹出云台自带系统菜单
	bool PopPTZMenu();
	bool ClosePTZMenu();
protected:
	BYTE m_ucCommand[COMMANDLEN];

	
};


#endif

