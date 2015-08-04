
#ifndef _PTZ_MAN_H_
#define _PTZ_MAN_H_

#include "PUB_common.h"
#include "dvrdvsdef.h"
#include "PTZ.h"
#include <map>

#include "Product.h"
#ifdef __ENVIRONMENT_WIN32__
#include "PtzProtocolWin32.h"
#else
#include "PTZProtocol.h"
#include "FakeCurise.h"
#endif

//////////////////////////////////////////////////////////////////////////
// * [2/4/2010]
// * 云台管理
//////////////////////////////////////////////////////////////////////////
// * 以下常量定义需要大于等于所有产品支持的最大个数或条数
const unsigned long PTZ_MAX_PRESET_NUM = 128;
const unsigned long PTZ_MAX_CURISE_PT_NUM = 32;
const unsigned long PTZ_MAX_CURISE_NUM = 8;
//////////////////////////////////////////////////////////////////////////

class CPtzMan
{
public:
	~CPtzMan ();
	static CPtzMan* Instance();
	bool Initial();
	void Quit();

	enum PtzCmdState
	{
		PTZ_CMD_WAITING = 0,
		PTZ_CMD_SUCC,
		PTZ_CMD_FAIL,
	};

	struct PtzCmdPack
	{
		unsigned long channel;			//本命令控制的通道
		unsigned long cmd_type;			//命令类型
		unsigned long speed;			//速度
		unsigned long wait_time;		//云台命令完成后,云台需要等待时间
		unsigned long param;			//命令参数
		unsigned long start_time;		//命令发起的时间戳
		unsigned long end_time;			//命令完成后的时间戳
		unsigned long process_state;	//命令处理的状态:待处理,已完成,处理失败
		unsigned char* ptr_param_ex;	//扩展命令参数,某些特定命令需要传递额外数据
	};

public:
	//基本控制 [通道、命令类型、命令参数、速率、等待时间] [返回错误代码]
	unsigned long Action(unsigned long channel, unsigned char cmdtype, unsigned long ptzParam, unsigned char speed = 5, unsigned char waiteTime = 0, const unsigned char* ptrParamEx = NULL);

	// * 配置设置读取相关,配置始终与配置文件中信息保持一致

	//云台配置串口信息 [通道、是否启用、地址、协议、波特率、速率]
	void SetSerialInfo(unsigned long channel, unsigned char enable, unsigned char addr, unsigned char protocol, unsigned char rate, unsigned long speed = 5);

	//预置点配置 [通道、预置点、预置点个数]
	void SetPresetConfig(unsigned long channel, const PTZ_PRESET* pPreset, unsigned long count);
	//获取预置点配置 [通道、输出预置点、输出预置点个数] [返回错误代码]
	unsigned long GetPresetConfig(unsigned long channel, PTZ_PRESET** pOutPreset, unsigned long& outCount);
	bool PresetIsEnable( unsigned long chnn, unsigned int presetNum );
	//巡航线配置 [通道、巡航线编号、巡航线头、巡航线点、巡航线点个数] 
	void SetCruiseConfig(unsigned long channel, unsigned long index, const PTZ_CURISE* pCurise, const PTZ_CURISE_POINT* pCurisePt, unsigned long count);
	//获取巡航线配置 [通道、巡航线编号、输出巡航线信息、输出巡航线点信息、输出巡航点个数] [返回错误代码]
	unsigned long GetCruiseConfig(unsigned long channel, unsigned long index, PTZ_CURISE** pOutCurise, PTZ_CURISE_POINT** pOutCurisePt, unsigned long& outCount);

	//获取某通道云台是否启用
	bool GetPtzEnable(unsigned long channel);
	//获取某通道云台状态 [通道] [返回通道状态]
	unsigned long GetPtzState(unsigned long channel);
	//设置某通道云台状态 [通道、状态]
	void SetPtzState(unsigned long channel, unsigned long state);

	//设置、获取锁定
	bool IsPtzLock(unsigned long channel);
	void SetPtzLock(unsigned long channel, bool lock);
	
	//设置巡航线预览的预置点信息到云台
	bool SetCruisePreviewInfo( const unsigned char *pData );
	//恢复通道chnn预览的巡航线预置点信息
	bool RecoverCruisePreviewInfo( unsigned long chnn );

	//云台锁定
	void EnterPtz(unsigned long clientID = LOCAL_DEVICE_ID);
	void ExitPtz();
	unsigned long PtzClient();
	
	bool WantToConfigPTZ(unsigned char cmdtype);

protected:
	CPtzMan();

protected:
	//处理命令
	unsigned long ProcessAction(unsigned long channel, unsigned char cmdtype, unsigned long ptzParam, unsigned char speed = 5, unsigned char waiteTime = 0, unsigned char* ptrParamEx = NULL);

	//处理命令
	unsigned long ProcessCommand(PtzCmdPack& cmd);

	//检查某个通道,是否可以再次执行命令 [通道] [当前时间]
	unsigned long CanChnnProcess(unsigned long chnn, unsigned long timetip);

	//检查某命令是否可以被执行
	unsigned long CanProcess(const PtzCmdPack& cmd);

	//获取命令的大分类
	unsigned long GetCmdType(unsigned char cmdtype, unsigned long param, bool& bEnd);

	//停止云台的当前状态
	bool ResetPtz(unsigned long chnn, unsigned long& curState);

protected:
	//获取某协议处理操作类
	CPTZProtocol* GetPtzProtocol(unsigned long protocol,unsigned long chnn);
	
	//恢复一个通道的预览过的巡航点
	bool RecovreOneChnnCurisePreview( unsigned long chnn );
	
	//客户端异常退出时清除PTZ配置状态
	void StopPTZConfig();

protected:
	// * 内部使用结构体定义

	//云台串口配置、协议控制相关
	struct PtzSerialInfo
	{
		unsigned char enable;				//表示是否启用云台功能
		unsigned char addr;					//云台设备ID
		unsigned char protocol;				//协议ID，0为没有配置云台
		unsigned char baud_rate;			//波特率0-13分别表示110，300，…..921600的波特率
		unsigned long channel;				//通道
		unsigned long speed;				//云台默认速度
		unsigned long last_cmd_type;		//最后一次执行的命令类型
		unsigned long last_process_time;	//最后一次执行命令的时间
		unsigned long wait_time;			//最后一次执行命令需要等待的时间
	};

	struct PtzPreset
	{
		unsigned long channel;					//通道
		unsigned long count;					//预置点使用个数,最大PTZ_MAX_PRESET_NUM个
		PTZ_PRESET preset[PTZ_MAX_PRESET_NUM];	//预置点数据
	};

	struct PtzOneCurise
	{
		unsigned long symbol;			//是本巡航线标记 : 位表示[1是否设置了此巡航线、2是否启用本巡航线、3是否为真实巡航线]
		unsigned long count;			//巡航点使用个数,最大PTZ_MAX_CURISE_PT_NUM个
		PTZ_CURISE header;				//巡航线头信息
		PTZ_CURISE_POINT curise_pt[PTZ_MAX_CURISE_PT_NUM]; //巡航线点信息
	};

	struct PtzCurise
	{
		unsigned long channel;					//通道
		unsigned long count;					//通道巡航线使用条数,最大PTZ_MAX_CURISE_NUM条
		PtzOneCurise curise[PTZ_MAX_CURISE_NUM];//每条巡航线信息
	};

	typedef std::map<unsigned long, PtzSerialInfo>		PtzSerialInfoMap;	//[通道,串口配置信息]
	typedef std::map<unsigned long, PtzPreset*>			PtzPresetMap;		//[通道,预置点配置信息]
	typedef std::map<unsigned long, PtzCurise*>			PtzCuriseMap;		//[通道,巡航线配置信息]
	typedef std::map<unsigned long, CPTZProtocol*>		PtzProtocolMap;		//[协议编号,协议控制类]

protected:

	bool						m_bInit;						//是否初始化本类

	std::map<unsigned long, unsigned long> m_ptzChnnState;		//本类各个通道所处于的状态 [通道,状态标记]
	PtzSerialInfoMap			m_cfgSerialInfoMap;				//配置相关通道的串口信息 [通道,串口配置信息]
	PtzPresetMap				m_cfgPresetMap;					//配置存储的预置点信息 [通道,预置点配置信息]
	PtzCuriseMap				m_cfgCuriseMap;					//配置存储的巡航线信息 [通道,巡航线配置信息]
	unsigned char				m_curiseIsPreview[MAX_CHNN_NUM][PTZ_MAX_CURISE_NUM];		//存放某通道的巡航线是否被预览过,为0表示未被预览，不为0表示被预览过	
	PtzProtocolMap				m_protocolMap;					//协议族支持的控制方式 [协议编号,协议控制类]

	//
	CPUB_Lock					m_lock;
	unsigned long				m_idPtzCtrl;		//锁定进入进入云台控制
#ifdef __ENVIRONMENT_LINUX__
	CFakeCurise					*m_pFakeCurise;
#endif
};

#endif

