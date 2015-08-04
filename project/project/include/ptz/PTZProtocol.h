#ifndef PTZPROTOCOL_H
#define PTZPROTOCOL_H

//#include "Config.h"
#include "PTZ.h"
#include "Typedef.h"
#include "dvrdvsdef.h"
#include "PUB_common.h"
#include "SerialPort.h"
//#include "..\RECORD\avitypes.h"	// Added by ClassView
class CPTZProtocol  
{
public:

	CPTZProtocol();
	virtual ~CPTZProtocol();

	/////////////
	virtual bool Command(BYTE cmd,BYTE ptzaddr,int baudrate,BYTE speed, unsigned long ptzPram, BYTE waitTime = 0);
	bool SetCurisePoint(BYTE ptzaddr,int baudrate, BYTE uCruiseId,PTZ_CURISE_POINT *pCurisePoint,BYTE curisePointNum);
	void GetDevAddrRange(int &min,int &max){min = 0;max = 255;}

	bool WriteBuffer(const BYTE* pBuffer, int len);
	
	//查询是否支持巡航
	virtual bool IsSupportCruise();
protected:
	virtual bool Left()	= 0;
	virtual bool Right()	= 0;
	virtual bool Up()	= 0;
	virtual bool Down()	= 0;
	virtual bool LeftDown() = 0;
	virtual bool LeftUp() = 0;
	virtual bool RightDown() = 0;
	virtual bool RightUp() = 0;

	virtual bool Near()	= 0;
	virtual bool Far()	= 0;
	virtual bool ZoomOut()	= 0;
	virtual bool ZoomIn()	= 0;
	virtual bool IrisOpen()	= 0;
	virtual bool IrisClose()= 0;

	virtual bool Stop() = 0;

#if 1
	virtual bool PresetGo(BYTE ucPresetID) = 0;		//到某个预置点
	virtual bool PresetSet(BYTE ucPresetID) = 0;	//将目前位置设置为ucPresetID的预置点
	virtual bool PresetClear(BYTE ucPresetID);  //清楚某个预置点。
#endif
	//zhoufeng 2007-12-11 添加巡航功能
	virtual bool RunCruise(BYTE uCruiseId);		//运行巡航
	virtual bool StopCruise();					//停止运行巡航
	virtual bool DeleteCruise(BYTE uCruiseId);		//删除巡航运轨迹线
	virtual bool EnterCruiseMode(BYTE uCruiseId);							//进入巡航配置模式
	virtual bool ConfigCruise(BYTE ucPresetID, BYTE speed, BYTE waitTime);	//设置巡航速度
	virtual bool LeaveCruiseMode(BYTE uCruiseId);							//退出巡航配置模式
	virtual bool ConfigureCruiseDisable();


	//zhl113  2009-07-07 添加云台轨迹和自动扫描功能
	virtual bool Record_Track();
	virtual bool Stop_Record_Track();
	virtual bool Start_Track();
	virtual bool Stop_Track();

	//zhl-113 2009-07-9 添加自动扫描功能
	virtual bool Auto_Scan();
	virtual bool Stop_Auto_Scan();

	//添加弹出云台自带系统菜单功能
	virtual bool PopPTZMenu();
	
	//控制辅助开关， 辅助开关用来控制雨刷、灯光、和摄像头辅助开关
	virtual bool SetAuxiliary(BYTE auxNum);
	virtual bool ClearAuxiliary(BYTE auxNum);

	PORT_BAUDRATE	m_Baudrate;//波特率
	long			m_Speed;
	DWORD			m_dwLastState;
	long			m_lDevAddress;
	
private:
	BYTE		m_LastCmd;
	LONGLONG	m_LastTime;
};

#endif

