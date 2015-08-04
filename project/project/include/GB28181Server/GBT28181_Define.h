/******************************************************************************

filename:GBT28181_Define.h

describe:各种定义头文件

copyright:

author:龙亮

date:2013/03/11

******************************************************************************/
#pragma once
#include "dvrdvsdef.h"
#include <deque>
using namespace std;

#define WAITTIME		50				//SIP事件监听时间间隔
#define LIVETIME		(90 * 1000)		//保活时间
#define HEARTBEATTIME	(20 * 1000)		//心跳间隔<30s

/************************************************************************/
/*                                 xml                                  */
/************************************************************************/

enum XML_PROTOCOL_TYPE
{
	PROTOCOL_INVALID = -1,

	PROTOCOL_BASIC = 0x00000000,

	PROTOCOL_REQUEST = 0x00810000,							//请求命令
	PROTOCOL_CONTROL = 0x00818100,							//CONTROL请求
	PROTOCOL_QUERY = 0x00818200,							//QUERY请求命令	
	PROTOCOL_NOTIFY = 0x00818300,							//NOTIFY命令
	PROTOCOL_RESPONSE = 0x00820000,							//应答命令

	PROTOCOL_CONTROL_PTZ     = 0x00818101,                   //云台控制
	PROTOCOL_CONTROL_TELEBOOT = 0x00818102,                  //远程重启
	PROTOCOL_CONTROL_RECORD  =   0x00818103,                  //录像
	PROTOCOL_CONTROL_GUARD  = 0x00818104,                     //布防
	PROTOCOL_CONTROL_ALARM  =  0x00818105,                     //告警复位

	PROTOCOL_QUERY_DEVICESTATUS = 0x00818201,				//设备状态查询请求
	PROTOCOL_QUERY_CATALOG = 0x00818202,					//设备目录信息查询请求
	PROTOCOL_QUERY_DEVICEINFO = 0x00818203,					//设备信息查询请求
	PROTOCOL_QUERY_RECORDINFO = 0x00818204,					//文件目录检索请求
	PROTOCOL_QUERY_ALARM = 0x00818205,						//报警查询

	PROTOCOL_NOTIFY_KEEPALIVE = 0x00818301,					//状态信息报送
	PROTOCOL_NOTIFY_ALARM = 0x00818302,						//报警通知
	PROTOCOL_NOTIFY_FILENED = 0x00818303,                   //回放或者下载结束上报

	PROTOCOL_RESPONSE_DEVICECONTROL = 0x00820001,			//设备控制应答
	PROTOCOL_RESPONSE_ALARM = 0x00820002,					//报警通知应答
	PROTOCOL_RESPONSE_CATALOG = 0x00820003,					//设备目录信息查询应答
	PROTOCOL_RESPONSE_DEVICEINFO = 0x00820004,				//设备信息查询应答
	PROTOCOL_RESPONSE_DEVICESTATUS = 0x00820005,			//设备状态信息查询应答
	PROTOCOL_RESPONSE_RECORDINFO = 0x00820006,				//文件目录检索应答
};

class CBasicProtocol
{
public:
	CBasicProtocol() : m_ProtocolType(PROTOCOL_BASIC)
	{
		m_dwSN=0;
		m_ProtocolType=PROTOCOL_BASIC;
	};

	/*virtual*/ ~CBasicProtocol(){};
	void Clear(){m_ProtocolType = PROTOCOL_BASIC;};

	XML_PROTOCOL_TYPE GetProtocolType(){return m_ProtocolType;};

public:
	XML_PROTOCOL_TYPE m_ProtocolType;

	DWORD m_dwSN;						//<!-- 命令序列号（必选） -->
};

/************************************************************************/
/*                                 控制                                 */
/************************************************************************/
class CControlProtocol : public CBasicProtocol
{
public:
	CControlProtocol()
	{
		memset(m_szDeviceID,0,sizeof(m_szDeviceID));
		memset(m_szPTZCmd,0,sizeof(m_szPTZCmd));
		m_bTeleBoot	  =  false;
		m_bRecord	  =  false;
		m_bGuard	  =  false;
		m_bResetAlarm =  false;
		m_ProtocolType = PROTOCOL_CONTROL;
	};
	/*virtual*/ ~CControlProtocol(){};
	void Clear()
	{
		memset(m_szDeviceID,0,sizeof(m_szDeviceID));
		memset(m_szPTZCmd,0,sizeof(m_szPTZCmd));
		m_bTeleBoot	  =  false;
		m_bRecord	  =  false;
		m_bGuard	  =  false;
		m_bResetAlarm =  false;
		CBasicProtocol::Clear();
		m_ProtocolType = PROTOCOL_CONTROL;
	};

	CControlProtocol(const CControlProtocol& Protocol)
	{
		Clear();
		memcpy(m_szDeviceID, Protocol.m_szDeviceID,min(sizeof(m_szDeviceID)-1, strlen(Protocol.m_szDeviceID)));
		memcpy(m_szPTZCmd, Protocol.m_szPTZCmd, min(sizeof(m_szPTZCmd)-1,strlen(Protocol.m_szPTZCmd)));

		m_bTeleBoot = Protocol.m_bTeleBoot;
		m_bRecord = Protocol.m_bRecord;
		m_bGuard = Protocol.m_bGuard;
		m_bResetAlarm = Protocol.m_bResetAlarm;
		m_ControlType = Protocol.m_ControlType;
	}

	CControlProtocol& operator=(const CControlProtocol& Protocol)
	{
		if (this == &Protocol)
		{
			return *this;
		}

		Clear();
		memcpy(m_szDeviceID, Protocol.m_szDeviceID, min(sizeof(m_szDeviceID)-1,strlen(Protocol.m_szDeviceID)));
		memcpy(m_szPTZCmd, Protocol.m_szPTZCmd, min(sizeof(m_szPTZCmd)-1,strlen(Protocol.m_szPTZCmd)));

		m_bTeleBoot = Protocol.m_bTeleBoot;
		m_bRecord = Protocol.m_bRecord;
		m_bGuard = Protocol.m_bGuard;
		m_bResetAlarm = Protocol.m_bResetAlarm;
		m_ControlType = Protocol.m_ControlType;
		return *this;
	}
 /*
	enum CONTROL_TYPE
	{
		CONTROL_TYPE_PTZCmd = 0x01,
		CONTROL_TYPE_TeleBoot = 0x02,
		CONTROL_TYPE_RecordCmd = 0x04,
		CONTROL_TYPE_GuardCmd = 0x08,
		CONTROL_TYPE_AlarmCmd = 0x10,
	};
  */
public:
	char m_szDeviceID[32];				//<!-- 目标设备编码（必选） -->
	char m_szPTZCmd[32];				//<!-- 球机/云台控制命令（可选，控制码应符合附录A中的A.3中的规定) -->
	bool m_bTeleBoot;					//<!-- 远程启动控制命令（可选） -->
	bool m_bRecord;						//<!-- 录像控制命令（可选） -->
	bool m_bGuard;						//<!-- 报警布防/撤防命令（可选） -->
	bool m_bResetAlarm;					//<!-- 报警复位命令（可选） -->
	//list m_listInfo					//<!-- 扩展信息，可多项 -->

	XML_PROTOCOL_TYPE m_ControlType;
};

/************************************************************************/
/*                                 查询                                 */
/************************************************************************/
//设备状态查询请求
class CQueryDeviceStatusProtocol : public CBasicProtocol
{
public:
	CQueryDeviceStatusProtocol()
	{
		memset(m_szDeviceID,0,sizeof(m_szDeviceID)); 
		m_ProtocolType = PROTOCOL_QUERY_DEVICESTATUS;
	};

	/*virtual*/ ~CQueryDeviceStatusProtocol(){};

	void Clear()
	{
		memset(m_szDeviceID,0,sizeof(m_szDeviceID));
		CBasicProtocol::Clear();
		m_ProtocolType = PROTOCOL_QUERY_DEVICESTATUS;
	};

	CQueryDeviceStatusProtocol(const CQueryDeviceStatusProtocol& Protocol)
	{
		Clear();
		memcpy(m_szDeviceID, Protocol.m_szDeviceID, min(sizeof(m_szDeviceID)-1,strlen(Protocol.m_szDeviceID)));
	}

	CQueryDeviceStatusProtocol& operator=(const CQueryDeviceStatusProtocol& Protocol)
	{
		if (this == &Protocol)
		{
			return *this;
		}

		Clear();
		memcpy(m_szDeviceID, Protocol.m_szDeviceID, min(sizeof(m_szDeviceID)-1,strlen(Protocol.m_szDeviceID)));
		return *this;
	}

public:
	char m_szDeviceID[32];				//<!-- 目标设备的设备/区域、安全防范视频监控联网系统编码（必选） -->
};

//设备目录信息查询请求
class CQueryCatalogProtocol : public CBasicProtocol
{
public:
	CQueryCatalogProtocol()
	{
		memset(m_szDeviceID,0,sizeof(m_szDeviceID));
		memset(m_szStartTime,0,sizeof(m_szStartTime));
		memset(m_szEndTime,0,sizeof(m_szEndTime));

		m_ProtocolType = PROTOCOL_QUERY_CATALOG;
	};

	/*virtual*/ ~CQueryCatalogProtocol(){};
	void Clear()
	{
		memset(m_szDeviceID,0,sizeof(m_szDeviceID));
		memset(m_szStartTime,0,sizeof(m_szStartTime));
		memset(m_szEndTime,0,sizeof(m_szEndTime));

		CBasicProtocol::Clear(); 
		m_ProtocolType = PROTOCOL_QUERY_CATALOG;
	};

	CQueryCatalogProtocol(const CQueryCatalogProtocol& Protocol)
	{
		Clear();

		memcpy(m_szDeviceID, Protocol.m_szDeviceID, min(sizeof(m_szDeviceID)-1,strlen(Protocol.m_szDeviceID)));
		memcpy(m_szStartTime, Protocol.m_szStartTime, min(sizeof(m_szStartTime)-1,strlen(Protocol.m_szStartTime)));
		memcpy(m_szEndTime, Protocol.m_szEndTime, min(sizeof(m_szEndTime)-1,strlen(Protocol.m_szEndTime)));
	}

	CQueryCatalogProtocol& operator=(const CQueryCatalogProtocol& Protocol)
	{
		if (this == &Protocol)
		{
			return *this;
		}

		Clear();

		memcpy(m_szDeviceID, Protocol.m_szDeviceID, min(sizeof(m_szDeviceID)-1,strlen(Protocol.m_szDeviceID)));
		memcpy(m_szStartTime, Protocol.m_szStartTime, min(sizeof(m_szStartTime)-1,strlen(Protocol.m_szStartTime)));
		memcpy(m_szEndTime, Protocol.m_szEndTime, min(sizeof(m_szEndTime)-1,strlen(Protocol.m_szEndTime)));
		return *this;
	}

public:
	char m_szDeviceID[32];				//<!-- 目标设备的设备/区域/安全防范视频监控联网系统/区域编码（必选） -->
	char m_szStartTime[64];				//<!-- 增加设备的起始时间（可选）空表示不限 -->
	char m_szEndTime[64];				//<!-- 增加设备的终止时间（可选）空表示到当前时间-->
};

//设备信息查询请求
class CQueryDeviceInfoProtocol : public CBasicProtocol
{
public:
	CQueryDeviceInfoProtocol()
	{
		memset(m_szDeviceID,0,sizeof(m_szDeviceID));
		m_ProtocolType = PROTOCOL_QUERY_DEVICEINFO;
	};

	/*virtual*/ ~CQueryDeviceInfoProtocol(){};

	void Clear()
	{
		memset(m_szDeviceID,0,sizeof(m_szDeviceID));
		CBasicProtocol::Clear(); 
		m_ProtocolType = PROTOCOL_QUERY_DEVICEINFO;
	};

	CQueryDeviceInfoProtocol(const CQueryDeviceInfoProtocol& Protocol)
	{
		Clear();
		memcpy(m_szDeviceID, Protocol.m_szDeviceID, min(sizeof(m_szDeviceID)-1,strlen(Protocol.m_szDeviceID)));
	}

	CQueryDeviceInfoProtocol& operator=(const CQueryDeviceInfoProtocol& Protocol)
	{
		if (this == &Protocol)
		{
			return *this;
		}

		Clear();
		memcpy(m_szDeviceID, Protocol.m_szDeviceID, min(sizeof(m_szDeviceID)-1,strlen(Protocol.m_szDeviceID)));
		return *this;
	}

public:
	char m_szDeviceID[32];				//<!-- 目标设备的设备编码（必选） -->
	         
};

//文件目录检索请求
class CQueryRecordInfoProtocol : public CBasicProtocol
{
public:
	CQueryRecordInfoProtocol()
	{	
		memset(m_szDeviceID,0,sizeof(m_szDeviceID));
		memset(m_szStartTime,0,sizeof(m_szStartTime));
		memset(m_szEndTime,0,sizeof(m_szEndTime));
		memset(m_szFilePath,0,sizeof(m_szFilePath));
		memset(m_szAddress,0,sizeof(m_szAddress));
		memset(m_szType,0,sizeof(m_szType));
		memset(m_szRecorderID,0,sizeof(m_szRecorderID))	;
		m_iSecrecy = 0;

		m_ProtocolType = PROTOCOL_QUERY_RECORDINFO;
	};

	/*virtual*/ ~CQueryRecordInfoProtocol(){};

	void Clear()
	{
		memset(m_szDeviceID,0,sizeof(m_szDeviceID));
		memset(m_szStartTime,0,sizeof(m_szStartTime));
		memset(m_szEndTime,0,sizeof(m_szEndTime));
		memset(m_szFilePath,0,sizeof(m_szFilePath));
		memset(m_szAddress,0,sizeof(m_szAddress));
		memset(m_szType,0,sizeof(m_szType));
		memset(m_szRecorderID,0,sizeof(m_szRecorderID))	;
		m_iSecrecy = 0;
		
		CBasicProtocol::Clear();
		m_ProtocolType = PROTOCOL_QUERY_RECORDINFO;
	};

	CQueryRecordInfoProtocol(const CQueryRecordInfoProtocol& Protocol)
	{
		Clear();


		memcpy(m_szDeviceID, Protocol.m_szDeviceID, min(sizeof(m_szDeviceID)-1,strlen(Protocol.m_szDeviceID)));
		memcpy(m_szStartTime, Protocol.m_szStartTime, min(sizeof(m_szStartTime)-1,strlen(Protocol.m_szStartTime)));
		memcpy(m_szEndTime, Protocol.m_szEndTime, min(sizeof(m_szEndTime)-1,strlen(Protocol.m_szEndTime)));
		memcpy(m_szFilePath, Protocol.m_szFilePath, min(sizeof(m_szFilePath)-1,strlen(Protocol.m_szFilePath)));
		memcpy(m_szAddress, Protocol.m_szAddress, min(sizeof(m_szAddress)-1,strlen(Protocol.m_szAddress)));
		memcpy(m_szType, Protocol.m_szType, min(sizeof(m_szType)-1,strlen(Protocol.m_szType)));
		memcpy(m_szRecorderID, Protocol.m_szRecorderID, min(sizeof(m_szRecorderID)-1,strlen(Protocol.m_szRecorderID)));

		m_iSecrecy = Protocol.m_iSecrecy;
	
	}

	CQueryRecordInfoProtocol& operator=(const CQueryRecordInfoProtocol& Protocol)
	{
		if (this == &Protocol)
		{
			return *this;
		}

		Clear();

		memcpy(m_szDeviceID, Protocol.m_szDeviceID, min(sizeof(m_szDeviceID)-1,strlen(Protocol.m_szDeviceID)));
		memcpy(m_szStartTime, Protocol.m_szStartTime, min(sizeof(m_szStartTime)-1,strlen(Protocol.m_szStartTime)));
		memcpy(m_szEndTime, Protocol.m_szEndTime, min(sizeof(m_szEndTime)-1,strlen(Protocol.m_szEndTime)));
		memcpy(m_szFilePath, Protocol.m_szFilePath, min(sizeof(m_szFilePath)-1,strlen(Protocol.m_szFilePath)));
		memcpy(m_szAddress, Protocol.m_szAddress, min(sizeof(m_szAddress)-1,strlen(Protocol.m_szAddress)));
		memcpy(m_szType, Protocol.m_szType, min(sizeof(m_szType)-1,strlen(Protocol.m_szType)));
		memcpy(m_szRecorderID, Protocol.m_szRecorderID, min(sizeof(m_szRecorderID)-1,strlen(Protocol.m_szRecorderID)));

		m_iSecrecy = Protocol.m_iSecrecy;

		return *this;
	}

public:
	char m_szDeviceID[32];				//<!-- 目录设备/安全防范视频监控联网系统/区域编码（必选） -->
	char m_szStartTime[64];				//<!-- 录像起始时间（可选）空表示不限 -->
	char m_szEndTime[64];				//<!-- 增加录像终止时间（可选）空表示到当前时间-->
	char m_szFilePath[256];				//<!-- 文件路径名 （可选）-->
	char m_szAddress[256];				//<!-- 录像地址（可选 支持不完全查询） -->
	int m_iSecrecy;						//<!-- 保密属性（可选）缺省为0；0：不涉密，1：涉密-->
	char m_szType[10];					//<!-- 录像产生类型（可选）time或alarm或manual或all -->
	char m_szRecorderID[32];			//<!-- 录像触发者ID（可选）-->
};

//报警查询
class CQueryAlarmProtocol : public CBasicProtocol
{
public:
	CQueryAlarmProtocol()
	{
		memset(m_szDeviceID,0,sizeof(m_szDeviceID));
		memset(m_szStartAlarmPriority,0,sizeof(m_szStartAlarmPriority));
		memset(m_szEndAlarmPriority,0,sizeof(m_szEndAlarmPriority));
		memset(m_szAlarmMethod,0,sizeof(m_szAlarmMethod));
		memset(m_szStartAlarmTime,0,sizeof(m_szStartAlarmTime));
		memset(m_szEndAlarmTime,0,sizeof(m_szEndAlarmTime));

		m_ProtocolType = PROTOCOL_QUERY_ALARM;
	};
	/*virtual*/ ~CQueryAlarmProtocol(){};
	void Clear()
	{
		memset(m_szDeviceID,0,sizeof(m_szDeviceID));
		memset(m_szStartAlarmPriority,0,sizeof(m_szStartAlarmPriority));
		memset(m_szEndAlarmPriority,0,sizeof(m_szEndAlarmPriority));
		memset(m_szAlarmMethod,0,sizeof(m_szAlarmMethod));
		memset(m_szStartAlarmTime,0,sizeof(m_szStartAlarmTime));
		memset(m_szEndAlarmTime,0,sizeof(m_szEndAlarmTime));

		CBasicProtocol::Clear(); 
		m_ProtocolType = PROTOCOL_QUERY_ALARM;
	};

	CQueryAlarmProtocol(const CQueryAlarmProtocol& Protocol)
	{
		Clear();


		memcpy(m_szDeviceID, Protocol.m_szDeviceID, min(sizeof(m_szDeviceID)-1,strlen(Protocol.m_szDeviceID)));
		memcpy(m_szStartAlarmPriority, Protocol.m_szStartAlarmPriority, min(sizeof(m_szStartAlarmPriority)-1,strlen(Protocol.m_szStartAlarmPriority)));
		memcpy(m_szEndAlarmPriority, Protocol.m_szEndAlarmPriority, min(sizeof(m_szEndAlarmPriority)-1,strlen(Protocol.m_szEndAlarmPriority)));
		memcpy(m_szAlarmMethod, Protocol.m_szAlarmMethod, min(sizeof(m_szAlarmMethod)-1,strlen(Protocol.m_szAlarmMethod)));
		memcpy(m_szStartAlarmTime, Protocol.m_szStartAlarmTime, min(sizeof(m_szStartAlarmTime)-1,strlen(Protocol.m_szStartAlarmTime)));
		memcpy(m_szEndAlarmTime, Protocol.m_szEndAlarmTime, min(sizeof(m_szEndAlarmTime)-1,strlen(Protocol.m_szEndAlarmTime)));


	}

	CQueryAlarmProtocol& operator=(const CQueryAlarmProtocol& Protocol)
	{
		if (this == &Protocol)
		{
			return *this;
		}

		Clear();

		memcpy(m_szDeviceID, Protocol.m_szDeviceID, min(sizeof(m_szDeviceID)-1,strlen(Protocol.m_szDeviceID)));
		memcpy(m_szStartAlarmPriority, Protocol.m_szStartAlarmPriority, min(sizeof(m_szStartAlarmPriority)-1,strlen(Protocol.m_szStartAlarmPriority)));
		memcpy(m_szEndAlarmPriority, Protocol.m_szEndAlarmPriority, min(sizeof(m_szEndAlarmPriority)-1,strlen(Protocol.m_szEndAlarmPriority)));
		memcpy(m_szAlarmMethod, Protocol.m_szAlarmMethod, min(sizeof(m_szAlarmMethod)-1,strlen(Protocol.m_szAlarmMethod)));
		memcpy(m_szStartAlarmTime, Protocol.m_szStartAlarmTime, min(sizeof(m_szStartAlarmTime)-1,strlen(Protocol.m_szStartAlarmTime)));
		memcpy(m_szEndAlarmTime, Protocol.m_szEndAlarmTime, min(sizeof(m_szEndAlarmTime)-1,strlen(Protocol.m_szEndAlarmTime)));
		return *this;
	}

public:
	//char m_szDeviceID[11];				//<!-- 报警设备编码或报警中心编码（10位）（必选）-->
	char m_szDeviceID[32];
	char m_szStartAlarmPriority[10];	//<!-- 报警起始级别（可选），0为全部，1为一级警情，2为二级警情，3为三级警情，4为四级警情-->
	char m_szEndAlarmPriority[10];		//<!-- 报警终止级别（可选），0为全部，1为一级警情，2为二级警情，3为三级警情，4为四级警情-->
	char m_szAlarmMethod[10];			//<!-- 报警方式条件（可选），取值0为全部，1为电话报警，2为设备报警，3为短信报警，4为GPS报警，
										//5为视频报警，6为设备故障报警，7其它报警；可以为直接组合如12为电话报警或设备报警-->
	char m_szStartAlarmTime[64];		//<!-- 增加设备的起始时间（可选）空表示不限 -->
	char m_szEndAlarmTime[64];			//<!-- 增加设备的终止时间（可选）空表示到当前时间-->
};

/************************************************************************/
/*                                 通知                                 */
/************************************************************************/
//状态信息报送
class CNotifyKeepAliveProtocol : public CBasicProtocol
{
public:
	CNotifyKeepAliveProtocol()
	{
		memset(m_szDeviceID,0,sizeof(m_szDeviceID));
		m_bStatus = 0;
		m_ProtocolType = PROTOCOL_NOTIFY_KEEPALIVE;
	};

	/*virtual*/ ~CNotifyKeepAliveProtocol(){};
	void Clear()
	{
		memset(m_szDeviceID,0,sizeof(m_szDeviceID));
		m_bStatus = 0;

		CBasicProtocol::Clear(); 
		m_ProtocolType = PROTOCOL_NOTIFY_KEEPALIVE;
	};

	CNotifyKeepAliveProtocol(const CNotifyKeepAliveProtocol& Protocol)
	{
		Clear();

		memcpy(m_szDeviceID, Protocol.m_szDeviceID, min(sizeof(m_szDeviceID)-1,strlen(Protocol.m_szDeviceID)));

		m_bStatus = Protocol.m_bStatus;
	}

	CNotifyKeepAliveProtocol& operator=(const CNotifyKeepAliveProtocol& Protocol)
	{
		if (this == &Protocol)
		{
			return *this;
		}

		Clear();

		memcpy(m_szDeviceID, Protocol.m_szDeviceID, min(sizeof(m_szDeviceID)-1,strlen(Protocol.m_szDeviceID)));
		m_bStatus = Protocol.m_bStatus;
		return *this;
	}

public:
	char m_szDeviceID[32];				//<!-- 源设备的设备/系统编码（必选） -->
	bool m_bStatus;						//<!-- 是否正常工作（必选） -->
};

//报警通知
class CNotifyAlarmProtocol : public CBasicProtocol
{
public:
	CNotifyAlarmProtocol()
	{
		memset(m_szDeviceID,0,sizeof(m_szDeviceID));
		memset(m_szAlarmPriority,0,sizeof(m_szAlarmPriority));
		memset(m_szAlarmMethod,0,sizeof(m_szAlarmMethod));
		memset(m_szAlarmTime,0,sizeof(m_szAlarmTime));
		memset(m_szAlarmDescription,0,sizeof(m_szAlarmDescription));

		m_fLongitude =0.0;
		m_fLatitude =0.0;

		m_ProtocolType = PROTOCOL_NOTIFY_ALARM;
	};

	/*virtual*/ ~CNotifyAlarmProtocol(){};
	void Clear()
	{
		memset(m_szDeviceID,0,sizeof(m_szDeviceID));
		memset(m_szAlarmPriority,0,sizeof(m_szAlarmPriority));
		memset(m_szAlarmMethod,0,sizeof(m_szAlarmMethod));
		memset(m_szAlarmTime,0,sizeof(m_szAlarmTime));
		memset(m_szAlarmDescription,0,sizeof(m_szAlarmDescription));
		m_fLongitude =0.0;
		m_fLatitude =0.0;

		CBasicProtocol::Clear();
		m_ProtocolType = PROTOCOL_NOTIFY_ALARM;
	};

	CNotifyAlarmProtocol(const CNotifyAlarmProtocol& Protocol)
	{
		Clear();

		memcpy(m_szDeviceID, Protocol.m_szDeviceID, min(sizeof(m_szDeviceID)-1,strlen(Protocol.m_szDeviceID)));
		memcpy(m_szAlarmPriority, Protocol.m_szAlarmPriority, min(sizeof(m_szAlarmPriority)-1,strlen(Protocol.m_szAlarmPriority)));
		memcpy(m_szAlarmMethod, Protocol.m_szAlarmMethod, min(sizeof(m_szAlarmMethod)-1,strlen(Protocol.m_szAlarmMethod)));

		memcpy(m_szAlarmTime, Protocol.m_szAlarmTime, min(sizeof(m_szAlarmTime)-1,strlen(Protocol.m_szAlarmTime)));
		memcpy(m_szAlarmDescription, Protocol.m_szAlarmDescription, min(sizeof(m_szAlarmDescription)-1,strlen(Protocol.m_szAlarmDescription)));
	

		m_fLongitude = Protocol.m_fLongitude;
		m_fLatitude = Protocol.m_fLatitude;
	}

	CNotifyAlarmProtocol& operator=(const CNotifyAlarmProtocol& Protocol)
	{
		if (this == &Protocol)
		{
			return *this;
		}

		Clear();

		memcpy(m_szDeviceID, Protocol.m_szDeviceID, min(sizeof(m_szDeviceID)-1,strlen(Protocol.m_szDeviceID)));
		memcpy(m_szAlarmPriority, Protocol.m_szAlarmPriority, min(sizeof(m_szAlarmPriority)-1,strlen(Protocol.m_szAlarmPriority)));
		memcpy(m_szAlarmMethod, Protocol.m_szAlarmMethod, min(sizeof(m_szAlarmMethod)-1,strlen(Protocol.m_szAlarmMethod)));

		memcpy(m_szAlarmTime, Protocol.m_szAlarmTime, min(sizeof(m_szAlarmTime)-1,strlen(Protocol.m_szAlarmTime)));
		memcpy(m_szAlarmDescription, Protocol.m_szAlarmDescription, min(sizeof(m_szAlarmDescription)-1,strlen(Protocol.m_szAlarmDescription)));

		m_fLongitude = Protocol.m_fLongitude;
		m_fLatitude = Protocol.m_fLatitude;
		return *this;
	}

public:
	//char m_szDeviceID[11];				//<!-- 报警设备编码或报警中心编码（10位）（必选）-->
	char m_szDeviceID[32];
	char m_szAlarmPriority[10];		//<!-- 报警级别（必选），1为一级警情，2为二级警情，3为三级警情，4为四级警情-->
	char m_szAlarmMethod[10];			//<!-- 报警方式（必选），取值1为电话报警，2为设备报警，3为短信报警，4为GPS报警，5为视频报警，6为设备故障报警，7其它报警-->
		
	char	m_szAlarmTime[64];           //<!--报警时间（必选）-->
	char m_szAlarmDescription[512];		//<!--报警内容描述（可选）-->
	double m_fLongitude;				//<!-- 经纬度信息可选 -->
	double m_fLatitude;					//<!-- 经纬度信息可选 -->
	//list m_listInfo					//<!-- 扩展信息，可多项 -->
};


//状态信息报送
class CNotifyFileEndProtocol : public CBasicProtocol
{
public:
	CNotifyFileEndProtocol()
	{
		memset(m_szDeviceID,0,sizeof(m_szDeviceID));
		m_ProtocolType = PROTOCOL_NOTIFY_FILENED;
	};

	/*virtual*/ ~CNotifyFileEndProtocol(){};
	void Clear()
	{
		memset(m_szDeviceID,0,sizeof(m_szDeviceID));
		memset(m_szNotifyType,0,sizeof(m_szNotifyType));
		CBasicProtocol::Clear(); 
		m_ProtocolType = PROTOCOL_NOTIFY_KEEPALIVE;
	};

	CNotifyFileEndProtocol(const CNotifyFileEndProtocol& Protocol)
	{
		Clear();

		memcpy(m_szDeviceID, Protocol.m_szDeviceID, min(sizeof(m_szDeviceID)-1,strlen(Protocol.m_szDeviceID)));
	    memcpy(m_szNotifyType, Protocol.m_szNotifyType, min(sizeof(m_szNotifyType)-1,strlen(Protocol.m_szNotifyType)));
	}

	CNotifyFileEndProtocol& operator=(const CNotifyFileEndProtocol& Protocol)
	{
		if (this == &Protocol)
		{
			return *this;
		}

		Clear();

		memcpy(m_szDeviceID, Protocol.m_szDeviceID, min(sizeof(m_szDeviceID)-1,strlen(Protocol.m_szDeviceID)));
		memcpy(m_szNotifyType, Protocol.m_szNotifyType, min(sizeof(m_szNotifyType)-1,strlen(Protocol.m_szNotifyType)));
		return *this;
	}

public:
	char m_szDeviceID[32];				//<!-- 源设备的设备/系统编码（必选） -->
	char m_szNotifyType[8];					//<!-- 通知类型 -->
};


/************************************************************************/
/*                                 应答                                 */
/************************************************************************/
//设备控制应答
class CResponseDeviceControlProtocol : public CBasicProtocol
{
public:
	CResponseDeviceControlProtocol()
	{
		memset(m_szDeviceID,0,sizeof(m_szDeviceID));
		m_bResult = 0;
		m_ProtocolType = PROTOCOL_RESPONSE_DEVICECONTROL;
	};

	/*virtual*/ ~CResponseDeviceControlProtocol(){};

	void Clear()
	{
		memset(m_szDeviceID,0,sizeof(m_szDeviceID));
		m_bResult = 0;

		CBasicProtocol::Clear(); 
		m_ProtocolType = PROTOCOL_RESPONSE_DEVICECONTROL;
	};

	CResponseDeviceControlProtocol(const CResponseDeviceControlProtocol& Protocol)
	{
		Clear();
		memcpy(m_szDeviceID, Protocol.m_szDeviceID, min(sizeof(m_szDeviceID)-1,strlen(Protocol.m_szDeviceID)));
		m_bResult = Protocol.m_bResult;
	}

	CResponseDeviceControlProtocol& operator=(const CResponseDeviceControlProtocol& Protocol)
	{
		if (this == &Protocol)
		{
			return *this;
		}

		Clear();
		memcpy(m_szDeviceID, Protocol.m_szDeviceID, min(sizeof(m_szDeviceID)-1,strlen(Protocol.m_szDeviceID)));
		m_bResult = Protocol.m_bResult;
		return *this;
	}

public:
	char m_szDeviceID[32];				//<!-- 目标设备/区域/系统编码（必选） -->
	bool m_bResult;						//<!-- 执行结果标志（必选） -->
};

//报警通知应答
class CResponseAlarmProtocol : public CBasicProtocol
{
public:
	CResponseAlarmProtocol()
	{
		memset(m_szDeviceID,0,sizeof(m_szDeviceID));
		m_bResult = 0;

		m_ProtocolType = PROTOCOL_RESPONSE_ALARM;
	};

	/*virtual*/ ~CResponseAlarmProtocol(){};

	void Clear()
	{
		memset(m_szDeviceID,0,sizeof(m_szDeviceID));
		m_bResult = 0;

		CBasicProtocol::Clear();
		m_ProtocolType = PROTOCOL_RESPONSE_ALARM;
	};

	CResponseAlarmProtocol(const CResponseAlarmProtocol& Protocol)
	{
		Clear();
		memcpy(m_szDeviceID, Protocol.m_szDeviceID, min(sizeof(m_szDeviceID)-1,strlen(Protocol.m_szDeviceID)));
		m_bResult = Protocol.m_bResult;
	}

	CResponseAlarmProtocol& operator=(const CResponseAlarmProtocol& Protocol)
	{
		if (this == &Protocol)
		{
			return *this;
		}

		Clear();
		memcpy(m_szDeviceID, Protocol.m_szDeviceID, min(sizeof(m_szDeviceID)-1,strlen(Protocol.m_szDeviceID)));
		m_bResult = Protocol.m_bResult;
		return *this;
	}

public:
	char m_szDeviceID[32];				//<!-- 目标设备/区域/系统编码（必选） -->
	bool m_bResult;						//<!-- 执行结果标志（必选） -->
};

//设备目录信息查询应答
class CCatalogItem
{
public:
	CCatalogItem()
	{
		Clear();
	};
	/*virtual*/ ~CCatalogItem(){};
	
	void Clear()
	{
		memset(m_szDeviceID,0,sizeof(m_szDeviceID));

		memset(m_szName,0,sizeof(m_szName));
		memset(m_szManufacturer,0,sizeof(m_szManufacturer));
		memset(m_szModel,0,sizeof(m_szModel));
		memset(m_szOwner,0,sizeof(m_szOwner));

		memset(m_szCivilCode,0,sizeof(m_szCivilCode));
		memset(m_szBlock,0,sizeof(m_szBlock));
		memset(m_szAddress,0,sizeof(m_szAddress));
		memset(m_szParental,0,sizeof(m_szParental));

		memset(m_szParentID,0,sizeof(m_szParentID));
		memset(m_szSafetyWay,0,sizeof(m_szSafetyWay));
		memset(m_szRegisterWay,0,sizeof(m_szRegisterWay));
		memset(m_szCertNum,0,sizeof(m_szCertNum));

		memset(m_szCertifiable,0,sizeof(m_szCertifiable));
		memset(m_szErrCode,0,sizeof(m_szErrCode));
		memset(m_szEndTime,0,sizeof(m_szEndTime));
		memset(m_szIPAddress,0,sizeof(m_szIPAddress));

		memset(m_szPassword,0,sizeof(m_szPassword));

		m_bSecrecy=false;
		m_bStatus =false;
		m_dwPort  = 0;
		m_fLongitude = 0.0;
		m_fLatitude = 0.0;
	
	}
	CCatalogItem(const CCatalogItem& Protocol)
	{
		Clear();	//push_back

		memcpy(m_szDeviceID, Protocol.m_szDeviceID, min(sizeof(m_szDeviceID)-1,strlen(Protocol.m_szDeviceID)));
		memcpy(m_szName, Protocol.m_szName, min(sizeof(m_szName)-1,strlen(Protocol.m_szName)));
		memcpy(m_szManufacturer, Protocol.m_szManufacturer, min(sizeof(m_szManufacturer)-1,strlen(Protocol.m_szManufacturer)));
		memcpy(m_szModel, Protocol.m_szModel, min(sizeof(m_szModel)-1,strlen(Protocol.m_szModel)));
		memcpy(m_szOwner, Protocol.m_szOwner, min(sizeof(m_szOwner)-1,strlen(Protocol.m_szOwner)));
		memcpy(m_szCivilCode, Protocol.m_szCivilCode, min(sizeof(m_szCivilCode)-1,strlen(Protocol.m_szCivilCode)));
		memcpy(m_szBlock, Protocol.m_szBlock, min(sizeof(m_szBlock)-1,strlen(Protocol.m_szBlock)));
		memcpy(m_szAddress, Protocol.m_szAddress, min(sizeof(m_szAddress)-1,strlen(Protocol.m_szAddress)));
		memcpy(m_szParental, Protocol.m_szParental, min(sizeof(m_szParental)-1,strlen(Protocol.m_szParental)));
		memcpy(m_szSafetyWay, Protocol.m_szSafetyWay, min(sizeof(m_szSafetyWay)-1,strlen(Protocol.m_szSafetyWay)));
		memcpy(m_szRegisterWay, Protocol.m_szRegisterWay, min(sizeof(m_szRegisterWay)-1,strlen(Protocol.m_szRegisterWay)));
		memcpy(m_szCertNum, Protocol.m_szCertNum, min(sizeof(m_szCertNum)-1,strlen(Protocol.m_szCertNum)));
		memcpy(m_szCertifiable, Protocol.m_szCertifiable, min(sizeof(m_szCertifiable)-1,strlen(Protocol.m_szCertifiable)));
		memcpy(m_szErrCode, Protocol.m_szErrCode, min(sizeof(m_szErrCode)-1,strlen(Protocol.m_szErrCode)));
		memcpy(m_szEndTime, Protocol.m_szEndTime, min(sizeof(m_szEndTime)-1,strlen(Protocol.m_szEndTime)));
		memcpy(m_szIPAddress, Protocol.m_szIPAddress, min(sizeof(m_szIPAddress)-1,strlen(Protocol.m_szIPAddress)));
		memcpy(m_szPassword, Protocol.m_szPassword, min(sizeof(m_szPassword)-1,strlen(Protocol.m_szPassword)));

		m_bSecrecy = Protocol.m_bSecrecy;
		m_dwPort = Protocol.m_dwPort;
		m_bStatus = Protocol.m_bStatus;
		m_fLongitude = Protocol.m_fLongitude;
		m_fLatitude = Protocol.m_fLatitude;
	}

	CCatalogItem& operator=(const CCatalogItem& Protocol)
	{
		if (this == &Protocol)
		{
			return *this;
		}

		Clear();

		memcpy(m_szDeviceID, Protocol.m_szDeviceID, min(sizeof(m_szDeviceID)-1,strlen(Protocol.m_szDeviceID)));
		memcpy(m_szName, Protocol.m_szName, min(sizeof(m_szName)-1,strlen(Protocol.m_szName)));
		memcpy(m_szManufacturer, Protocol.m_szManufacturer, min(sizeof(m_szManufacturer)-1,strlen(Protocol.m_szManufacturer)));
		memcpy(m_szModel, Protocol.m_szModel, min(sizeof(m_szModel)-1,strlen(Protocol.m_szModel)));
		memcpy(m_szOwner, Protocol.m_szOwner, min(sizeof(m_szOwner)-1,strlen(Protocol.m_szOwner)));
		memcpy(m_szCivilCode, Protocol.m_szCivilCode, min(sizeof(m_szCivilCode)-1,strlen(Protocol.m_szCivilCode)));
		memcpy(m_szBlock, Protocol.m_szBlock, min(sizeof(m_szBlock)-1,strlen(Protocol.m_szBlock)));
		memcpy(m_szAddress, Protocol.m_szAddress, min(sizeof(m_szAddress)-1,strlen(Protocol.m_szAddress)));
		memcpy(m_szParental, Protocol.m_szParental, min(sizeof(m_szParental)-1,strlen(Protocol.m_szParental)));
		memcpy(m_szSafetyWay, Protocol.m_szSafetyWay, min(sizeof(m_szSafetyWay)-1,strlen(Protocol.m_szSafetyWay)));
		memcpy(m_szRegisterWay, Protocol.m_szRegisterWay, min(sizeof(m_szRegisterWay)-1,strlen(Protocol.m_szRegisterWay)));
		memcpy(m_szCertNum, Protocol.m_szCertNum, min(sizeof(m_szCertNum)-1,strlen(Protocol.m_szCertNum)));
		memcpy(m_szCertifiable, Protocol.m_szCertifiable, min(sizeof(m_szCertifiable)-1,strlen(Protocol.m_szCertifiable)));
		memcpy(m_szErrCode, Protocol.m_szErrCode, min(sizeof(m_szErrCode)-1,strlen(Protocol.m_szErrCode)));
		memcpy(m_szEndTime, Protocol.m_szEndTime, min(sizeof(m_szEndTime)-1,strlen(Protocol.m_szEndTime)));
		memcpy(m_szIPAddress, Protocol.m_szIPAddress, min(sizeof(m_szIPAddress)-1,strlen(Protocol.m_szIPAddress)));
		memcpy(m_szPassword, Protocol.m_szPassword, min(sizeof(m_szPassword)-1,strlen(Protocol.m_szPassword)));

		m_bSecrecy = Protocol.m_bSecrecy;
		m_dwPort = Protocol.m_dwPort;
		m_bStatus = Protocol.m_bStatus;
		m_fLongitude = Protocol.m_fLongitude;
		m_fLatitude = Protocol.m_fLatitude;
		return *this;
	}

public:
	char m_szDeviceID[32];				//<!-- 设备/区域/系统编码（必选） -->
	char m_szName[128];					//<!-- 设备/区域/系统名称（必选） -->
	char m_szManufacturer[64];			//<!-- 当为设备时，设备厂商（必选） -->
	char m_szModel[64];					//<!-- 当为设备时，设备型号（必选） -->
	char m_szOwner[64];					//<!-- 当为设备时，设备归属（必选） -->
	char m_szCivilCode[64];				//<!-- 行政区域（必选） -->
	char m_szBlock[64];					//<!-- 警区（可选） -->
	char m_szAddress[64];				//<!-- 当为设备时，安装地址（必选） -->
	char m_szParental[10];				//<!-- 当为设备时，是否有子设备（必选）1有，0没有 -->
	char m_szParentID[32];				//<!-- 父设备/区域/系统ID（可选，有父设备需要填写） -->
	char m_szSafetyWay[10];				//<!-- 信令安全模式（可选）缺省为0； 0：不采用；2：S/MIME签名方式；3：S/MIME加密签名同时采用方式；4：数字摘要方式-->
	char m_szRegisterWay[10];			//<!-- 注册方式（必选）缺省为1； 1： 符合sip3261标准的认证注册模式；2： 基于口令的双向认证注册模式；3： 基于数字证书的双向认证注册模式-->
	char m_szCertNum[64];				//<!-- 证书序列号（有证书的设备必选） -->
	char m_szCertifiable[10];			//<!-- 证书有效标识（有证书的设备必选）缺省为0；证书有效标识： 0：无效 1：有效-->
	char m_szErrCode[32];				//<!-- 无效原因码（有证书切证书无效的设备必选） -->
	char m_szEndTime[64];				//<!-- 证书终止有效期（有证书的设备必选） -->
	bool m_bSecrecy;					//<!-- 保密属性（必选）缺省为0；0：不涉密，1：涉密-->
	char m_szIPAddress[16];				//<!-- 设备/区域/系统IP地址（可选）-->
	DWORD m_dwPort;						//<!-- 设备/区域/系统端口（可选）-->
	char m_szPassword[64];				//<!-- 设备口令（可选）-->
	bool m_bStatus;						//<!-- 设备状态（必选） -->
	double m_fLongitude;				//<!-- 经度（可选） -->
	double m_fLatitude;					//<!-- 纬度（可选） -->
};
class CResponseCatalogProtocol : public CBasicProtocol
{
public:
	CResponseCatalogProtocol()
	{
		memset(m_szDeviceID,0,sizeof(m_szDeviceID));
		memset(m_szSumNum,0,sizeof(m_szSumNum));
		m_bResult  =false;

		m_ResponseCatalogType = RESPONSECATALOG_TYPE_1;
		m_ProtocolType = PROTOCOL_RESPONSE_CATALOG;
	};
	/*virtual*/ ~CResponseCatalogProtocol(){};

	void Clear()
	{
		m_deqItem.clear(); 

		memset(m_szDeviceID,0,sizeof(m_szDeviceID));
		memset(m_szSumNum,0,sizeof(m_szSumNum));
		m_bResult  =false;

		CBasicProtocol::Clear(); 
		m_ProtocolType = PROTOCOL_RESPONSE_CATALOG;
	};
	void Delete()
	{
		deque<CCatalogItem*>::iterator it = m_deqItem.begin();
		for (; it != m_deqItem.end(); it++)
		{
			CCatalogItem* pItem = *it;
			if (pItem)
			{
				delete pItem;
				pItem = NULL;
			}
		}
		m_deqItem.clear();
	};

	CResponseCatalogProtocol(const CResponseCatalogProtocol& Protocol)
	{
		Clear();
		
		memcpy(m_szDeviceID, Protocol.m_szDeviceID, min(sizeof(m_szDeviceID)-1,strlen(Protocol.m_szDeviceID)));
		memcpy(m_szSumNum, Protocol.m_szSumNum, min(sizeof(m_szSumNum)-1,strlen(Protocol.m_szSumNum)));

		m_deqItem = Protocol.m_deqItem;
		m_bResult = Protocol.m_bResult;
		m_ResponseCatalogType = Protocol.m_ResponseCatalogType;
	}

	CResponseCatalogProtocol& operator=(const CResponseCatalogProtocol& Protocol)
	{
		if (this == &Protocol)
		{
			return *this;
		}

		Clear();

		memcpy(m_szDeviceID, Protocol.m_szDeviceID, min(sizeof(m_szDeviceID)-1,strlen(Protocol.m_szDeviceID)));
		memcpy(m_szSumNum, Protocol.m_szSumNum, min(sizeof(m_szSumNum)-1,strlen(Protocol.m_szSumNum)));

		m_deqItem = Protocol.m_deqItem;
		m_bResult = Protocol.m_bResult;
		m_ResponseCatalogType = Protocol.m_ResponseCatalogType;
		return *this;
	}

	enum RESPONSECATALOG_TYPE
	{
		RESPONSECATALOG_TYPE_1,								//设备目录信息查询应答
		RESPONSECATALOG_TYPE_2,								//目录信息查询收到应答
		RESPONSECATALOG_TYPE_3 = RESPONSECATALOG_TYPE_2		//目录收到应答
	};

public:
	char m_szDeviceID[32];				//<!-- 目标设备/区域/系统的编码（必选） -->
	char m_szSumNum[10];				//<!-- 查询结果总数（必选） -->
	deque<CCatalogItem*> m_deqItem;		//<!-- 设备目录项列表,Num表示目录项个数 -->
	//list m_listInfo					//<!-- 扩展信息，可多项 -->
	bool m_bResult;						//<!-- 执行结果标志（必选） -->

	RESPONSECATALOG_TYPE m_ResponseCatalogType;
};

//设备信息查询应答
class CResponseDeviceInfoProtocol : public CBasicProtocol
{
public:
	CResponseDeviceInfoProtocol()
	{
		memset(m_szDeviceID,0,sizeof(m_szDeviceID));
		memset(m_szManufacturer,0,sizeof(m_szManufacturer));
		memset(m_szModel,0,sizeof(m_szModel));
		memset(m_szFirmware,0,sizeof(m_szFirmware));
		memset(MaxCamera,0,sizeof(MaxCamera));
		memset(MaxALarm,0,sizeof(MaxALarm));
		memset(BuildTime,0,sizeof(BuildTime));
		memset(deviceName,0,sizeof(deviceName));

		m_bResult = false;
		m_dwChannel = 0;

		m_ProtocolType = PROTOCOL_RESPONSE_DEVICEINFO;
	};

	/*virtual*/ ~CResponseDeviceInfoProtocol(){};

	void Clear()
	{
		memset(m_szDeviceID,0,sizeof(m_szDeviceID));
		memset(m_szManufacturer,0,sizeof(m_szManufacturer));
		memset(m_szModel,0,sizeof(m_szModel));
		memset(m_szFirmware,0,sizeof(m_szFirmware));
		memset(MaxCamera,0,sizeof(MaxCamera));
		memset(MaxALarm,0,sizeof(MaxALarm));
		memset(BuildTime,0,sizeof(BuildTime));
		memset(deviceName,0,sizeof(deviceName));

		m_bResult = false;
		m_dwChannel = 0;

		CBasicProtocol::Clear();
		m_ProtocolType = PROTOCOL_RESPONSE_DEVICEINFO;
	};

	CResponseDeviceInfoProtocol(const CResponseDeviceInfoProtocol& Protocol)
	{
		Clear();
	
		m_bResult = Protocol.m_bResult;

		memcpy(m_szDeviceID, Protocol.m_szDeviceID, min(sizeof(m_szDeviceID)-1,strlen(Protocol.m_szDeviceID)));
		memcpy(m_szManufacturer, Protocol.m_szManufacturer, min(sizeof(m_szManufacturer)-1,strlen(Protocol.m_szManufacturer)));
		memcpy(m_szModel, Protocol.m_szModel, min(sizeof(m_szModel)-1,strlen(Protocol.m_szModel)));
		memcpy(m_szFirmware, Protocol.m_szFirmware, min(sizeof(m_szFirmware)-1,strlen(Protocol.m_szFirmware)));

		m_dwChannel = Protocol.m_dwChannel;
	}

	CResponseDeviceInfoProtocol& operator=(const CResponseDeviceInfoProtocol& Protocol)
	{
		if (this == &Protocol)
		{
			return *this;
		}

		Clear();

		m_bResult   = Protocol.m_bResult;
		m_dwChannel = Protocol.m_dwChannel;

		memcpy(m_szDeviceID, Protocol.m_szDeviceID, min(sizeof(m_szDeviceID)-1,strlen(Protocol.m_szDeviceID)));
		memcpy(m_szManufacturer, Protocol.m_szManufacturer, min(sizeof(m_szManufacturer)-1,strlen(Protocol.m_szManufacturer)));
		memcpy(m_szModel, Protocol.m_szModel, min(sizeof(m_szModel)-1,strlen(Protocol.m_szModel)));
		memcpy(m_szFirmware, Protocol.m_szFirmware, min(sizeof(m_szFirmware)-1,strlen(Protocol.m_szFirmware)));
		
		return *this;
	}

public:
	char m_szDeviceID[32];				//<!-- 目标设备/区域/系统的编码（必选） -->
	bool m_bResult;						//<!-- 查询结果（必选） -->
	char m_szManufacturer[64];			//<!-- 设备生产商（可选） -->
	char m_szModel[64];					//<!-- 设备型号（可选） -->
	char m_szFirmware[64];				//<!-- 设备固件版本（可选） -->
	DWORD m_dwChannel;					//<!-- 视频输入通道数（可选） -->

	char	MaxCamera[8];        //最大支持摄像机数目
	char	MaxALarm[8];         //最大告警路数
	char 	DeviceType[16];        //设备类型 dvr
	char	BuildTime[32];              //版本编译时间
	char			deviceName [32];       //设备名称
	//list m_listInfo					//<!-- 扩展信息，可多项 -->
};

//设备状态信息查询应答
class CDeviceStatusItem
{
public:
	CDeviceStatusItem()
	{
		memset(m_szDeviceID,0,sizeof(m_szDeviceID));
		memset(m_szStatus,0,sizeof(m_szStatus));
		m_lDevicetype = 0;
	};

	/*virtual*/ ~CDeviceStatusItem(){};

	CDeviceStatusItem(const CDeviceStatusItem& Protocol)
	{
		memset(m_szDeviceID,0,sizeof(m_szDeviceID));
		memset(m_szStatus,0,sizeof(m_szStatus));

		memcpy(m_szDeviceID, Protocol.m_szDeviceID, min(sizeof(m_szDeviceID)-1,strlen(Protocol.m_szDeviceID)));
		memcpy(m_szStatus, Protocol.m_szStatus, min(sizeof(m_szStatus)-1,strlen(Protocol.m_szStatus)));

	}

	CDeviceStatusItem& operator=(const CDeviceStatusItem& Protocol)
	{
		if (this == &Protocol)
		{
			return *this;
		}

		memset(m_szDeviceID,0,sizeof(m_szDeviceID));
		memset(m_szStatus,0,sizeof(m_szStatus));

		memcpy(m_szDeviceID, Protocol.m_szDeviceID, min(sizeof(m_szDeviceID)-1,strlen(Protocol.m_szDeviceID)));
		memcpy(m_szStatus, Protocol.m_szStatus, min(sizeof(m_szStatus)-1,strlen(Protocol.m_szStatus)));

		return *this;
	}

public:
	char m_szDeviceID[32];				//<!-- 报警设备编码（必选） -->
	char m_szStatus[10];				//<!-- 报警设备状态（必选） -->
	int  m_lDevicetype;                 //<-- 1：视频通道  2：报警设备
};
class CResponseDeviceStatusProtocol : public CBasicProtocol
{
public:
	CResponseDeviceStatusProtocol()
	{
		memset(m_szDeviceID,0,sizeof(m_szDeviceID));
		memset(m_szReason,0,sizeof(m_szReason));
		memset(m_szDeviceTime,0,sizeof(m_szDeviceTime));

		m_bResult =false;
		m_bOnline = false;
		m_bEncode =false;
		m_bStatus =false;
		m_bRecord =false;
		
		m_deqItem.clear();
		m_ProtocolType = PROTOCOL_RESPONSE_DEVICESTATUS;
	};

	/*virtual*/ ~CResponseDeviceStatusProtocol(){};

	void Clear()
	{
		m_deqItem.clear();
	
		memset(m_szDeviceID,0,sizeof(m_szDeviceID));
		memset(m_szReason,0,sizeof(m_szReason));
		memset(m_szDeviceTime,0,sizeof(m_szDeviceTime));

		m_bResult =false;
		m_bOnline = false;
		m_bEncode =false;
		m_bStatus =false;
		m_bRecord =false;

		CBasicProtocol::Clear(); 
		m_ProtocolType = PROTOCOL_RESPONSE_DEVICESTATUS;
	};

	void Delete()
	{
		deque<CDeviceStatusItem*>::iterator it = m_deqItem.begin();
		for (; it != m_deqItem.end(); it++)
		{
			CDeviceStatusItem* pItem = *it;
			if (pItem)
			{
				delete pItem;
				pItem = NULL;
			}
		}
		m_deqItem.clear();
	};

	CResponseDeviceStatusProtocol(const CResponseDeviceStatusProtocol& Protocol)
	{
		Clear();

		m_bResult = Protocol.m_bResult;
		m_bOnline = Protocol.m_bOnline;
		m_bStatus = Protocol.m_bStatus;
		m_bEncode = Protocol.m_bEncode;
		m_bRecord = Protocol.m_bRecord;
		m_deqItem = Protocol.m_deqItem;
		m_StatusType = Protocol.m_StatusType;

		memcpy(m_szDeviceID, Protocol.m_szDeviceID, min(sizeof(m_szDeviceID)-1,strlen(Protocol.m_szDeviceID)));
		memcpy(m_szReason, Protocol.m_szReason, min(sizeof(m_szReason)-1,strlen(Protocol.m_szReason)));
		memcpy(m_szDeviceTime, Protocol.m_szDeviceTime, min(sizeof(m_szDeviceTime)-1,strlen(Protocol.m_szDeviceTime)));
	}

	CResponseDeviceStatusProtocol& operator=(const CResponseDeviceStatusProtocol& Protocol)
	{
		if (this == &Protocol)
		{
			return *this;
		}

		Clear();

		m_bResult = Protocol.m_bResult;
		m_bOnline = Protocol.m_bOnline;
		m_bStatus = Protocol.m_bStatus;
		m_bEncode = Protocol.m_bEncode;
		m_bRecord = Protocol.m_bRecord;
		m_deqItem = Protocol.m_deqItem;
		m_StatusType = Protocol.m_StatusType;

		memcpy(m_szDeviceID, Protocol.m_szDeviceID, min(sizeof(m_szDeviceID)-1,strlen(Protocol.m_szDeviceID)));
		memcpy(m_szReason, Protocol.m_szReason, min(sizeof(m_szReason)-1,strlen(Protocol.m_szReason)));
		memcpy(m_szDeviceTime, Protocol.m_szDeviceTime, min(sizeof(m_szDeviceTime)-1,strlen(Protocol.m_szDeviceTime)));
		return *this;
	}

	enum STATUS_TYPE
	{
		STATUS_TYPE_Encode = 0x01,
		STATUS_TYPE_Record = 0x02
	};

public:
	char m_szDeviceID[32];				//<!-- 目标设备/区域/系统的编码（必选） -->
	bool m_bResult;						//<!-- 查询结果标志（必选） -->
	bool m_bOnline;						//<!-- 是否在线（必选） -->
	bool m_bStatus;						//<!-- 是否正常工作（必选） -->
	char m_szReason[512];				//<!-- 不正常工作原因（可选） -->
	bool m_bEncode;						//<!-- 是否编码（可选） -->
	bool m_bRecord;						//<!-- 是否录像（可选） -->
	char m_szDeviceTime[64];			//<!-- 设备时间和日期（可选） -->
	deque<CDeviceStatusItem*> m_deqItem;//<!-- 报警设备状态列表,num表示列表项个数（可选） -->
	//list m_listInfo					//<!-- 扩展信息，可多项 -->

	STATUS_TYPE m_StatusType;
};

//文件目录检索应答
class CRecordInfoItem
{
public:
	CRecordInfoItem()
	{
		Clear();
	};
	/*virtual*/ ~CRecordInfoItem(){};
	
	CRecordInfoItem(const CRecordInfoItem& Protocol)
	{
		Clear();

		m_bSecrecy = Protocol.m_bSecrecy;

		memcpy(m_szDeviceID, Protocol.m_szDeviceID, min(sizeof(m_szDeviceID)-1,strlen(Protocol.m_szDeviceID)));
		memcpy(m_szName, Protocol.m_szName, min(sizeof(m_szName)-1,strlen(Protocol.m_szName)));
		memcpy(m_szFilePath, Protocol.m_szFilePath, min(sizeof(m_szFilePath)-1,strlen(Protocol.m_szFilePath)));
		memcpy(m_szAddress, Protocol.m_szAddress, min(sizeof(m_szAddress)-1,strlen(Protocol.m_szAddress)));
		memcpy(m_szStartTime, Protocol.m_szStartTime, min(sizeof(m_szStartTime)-1,strlen(Protocol.m_szStartTime)));
		memcpy(m_szEndTime, Protocol.m_szEndTime, min(sizeof(m_szEndTime)-1,strlen(Protocol.m_szEndTime)));
		memcpy(m_szType, Protocol.m_szType, min(sizeof(m_szType)-1,strlen(Protocol.m_szType)));
		memcpy(m_szRecorderID, Protocol.m_szRecorderID, min(sizeof(m_szRecorderID)-1,strlen(Protocol.m_szRecorderID)));
		
	}

	void Clear()
	{
		memset(m_szDeviceID,0,sizeof(m_szDeviceID));
		memset(m_szName,0,sizeof(m_szName));
		memset(m_szFilePath,0,sizeof(m_szFilePath));
		memset(m_szAddress,0,sizeof(m_szAddress));
		memset(m_szStartTime,0,sizeof(m_szStartTime));
		memset(m_szEndTime,0,sizeof(m_szEndTime));
		memset(m_szType,0,sizeof(m_szType));
		memset(m_szRecorderID,0,sizeof(m_szRecorderID));

		m_bSecrecy =false;

	}

	CRecordInfoItem& operator=(const CRecordInfoItem& Protocol)
	{
		if (this == &Protocol)
		{
			return *this;
		}

		Clear();

		m_bSecrecy = Protocol.m_bSecrecy;

		memcpy(m_szDeviceID, Protocol.m_szDeviceID, min(sizeof(m_szDeviceID)-1,strlen(Protocol.m_szDeviceID)));
		memcpy(m_szName, Protocol.m_szName, min(sizeof(m_szName)-1,strlen(Protocol.m_szName)));
		memcpy(m_szFilePath, Protocol.m_szFilePath, min(sizeof(m_szFilePath)-1,strlen(Protocol.m_szFilePath)));
		memcpy(m_szAddress, Protocol.m_szAddress, min(sizeof(m_szAddress)-1,strlen(Protocol.m_szAddress)));
		memcpy(m_szStartTime, Protocol.m_szStartTime, min(sizeof(m_szStartTime)-1,strlen(Protocol.m_szStartTime)));
		memcpy(m_szEndTime, Protocol.m_szEndTime, min(sizeof(m_szEndTime)-1,strlen(Protocol.m_szEndTime)));
		memcpy(m_szType, Protocol.m_szType, min(sizeof(m_szType)-1,strlen(Protocol.m_szType)));
		memcpy(m_szRecorderID, Protocol.m_szRecorderID, min(sizeof(m_szRecorderID)-1,strlen(Protocol.m_szRecorderID)));

		return *this;
	}

public:
	char m_szDeviceID[32];				//<!-- 设备/区域编码（必选） -->
	char m_szName[128];					//<!-- 设备/区域名称（必选） -->
	char m_szFilePath[256];				//<!-- 文件路径名 （可选）-->
	char m_szAddress[256];				//<!-- 录像地址（可选） -->
	char m_szStartTime[64];				//<!-- 录像开始时间（可选）-->
	char m_szEndTime[64];				//<!-- 录像结束时间（可选）-->
	bool m_bSecrecy;					//<!-- 保密属性（必选）缺省为0；0：不涉密，1：涉密-->
	char m_szType[10];					//<!-- 录像产生类型（可选）time或alarm或manual或all -->
	char m_szRecorderID[32];			//<!-- 录像触发者ID（可选）-->
};
class CResponseRecordInfoProtocol : public CBasicProtocol
{
public:
	CResponseRecordInfoProtocol()
	{
		memset(m_szDeviceID,0,sizeof(m_szDeviceID));
		memset(m_szName,0,sizeof(m_szName));
		memset(m_szSumNum,0,sizeof(m_szSumNum));
		m_deqItem.clear();

		m_ProtocolType = PROTOCOL_RESPONSE_RECORDINFO;
	};

	/*virtual*/ ~CResponseRecordInfoProtocol(){};

	void Clear()
	{
		m_deqItem.clear(); 
		memset(m_szDeviceID,0,sizeof(m_szDeviceID));
		memset(m_szName,0,sizeof(m_szName));
		memset(m_szSumNum,0,sizeof(m_szSumNum));
		
		CBasicProtocol::Clear(); 
		m_ProtocolType = PROTOCOL_RESPONSE_RECORDINFO;
	};

	void Delete()
	{
		deque<CRecordInfoItem*>::iterator it = m_deqItem.begin();
		for (; it != m_deqItem.end(); it++)
		{
			CRecordInfoItem* pItem = *it;
			if (pItem)
			{
				delete pItem;
				pItem = NULL;
			}
		}
		m_deqItem.clear();
	};

	CResponseRecordInfoProtocol(const CResponseRecordInfoProtocol& Protocol)
	{
		Clear();
		
		memcpy(m_szDeviceID, Protocol.m_szDeviceID, min(sizeof(m_szDeviceID)-1,strlen(Protocol.m_szDeviceID)));
		memcpy(m_szName, Protocol.m_szName, min(sizeof(m_szName)-1,strlen(Protocol.m_szName)));
		memcpy(m_szSumNum, Protocol.m_szSumNum, min(sizeof(m_szSumNum)-1,strlen(Protocol.m_szSumNum)));

		m_deqItem = Protocol.m_deqItem;
	}

	CResponseRecordInfoProtocol& operator=(const CResponseRecordInfoProtocol& Protocol)
	{
		if (this == &Protocol)
		{
			return *this;
		}

		Clear();
		memcpy(m_szDeviceID, Protocol.m_szDeviceID, min(sizeof(m_szDeviceID)-1,strlen(Protocol.m_szDeviceID)));
		memcpy(m_szName, Protocol.m_szName, min(sizeof(m_szName)-1,strlen(Protocol.m_szName)));
		memcpy(m_szSumNum, Protocol.m_szSumNum, min(sizeof(m_szSumNum)-1,strlen(Protocol.m_szSumNum)));
		m_deqItem = Protocol.m_deqItem;
		return *this;
	}

public:
	char m_szDeviceID[32];				//<!-- 设备/区域编码（必选） -->
	char m_szName[128];					//<!-- 设备/区域名称（必选） -->
	char m_szSumNum[10];				//<!-- 查询结果总数（必选） -->
	deque<CRecordInfoItem*> m_deqItem;	//<!-- 文件目录项列表,Num表示目录项个数 -->
	//list m_listInfo					//<!-- 扩展信息，可多项 -->
};