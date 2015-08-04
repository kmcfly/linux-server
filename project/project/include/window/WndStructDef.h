#ifndef _WND_STRUCT_DEF__
#define _WND_STRUCT_DEF__

#include "dvrdvsdef.h"
#include "Product.h"
#include "NetConfigDefine.h"

//////////////////////////////////////////////////////////////////////////
// 配置、向导页面专用结构
//////////////////////////////////////////////////////////////////////////

#if defined(__ENVIRONMENT_WIN32__) || defined(__TDFH__) || defined(__TDNXP__) || defined(__CHIP3531__) || defined(__CHIP3535__) || defined(__CHIP3521__) || defined(__CHIP3520A__) || defined(__CHIPTI__) || defined(__CHIP3520D__) || defined(__CHIPGM__) 
#pragma pack(4)
#endif

//所有报警触发信息 =>> CfgTriggerDlg.h
typedef struct _alarm_trigger_info_
{
	unsigned short  Buzzer;					//触发蜂鸣器
	unsigned short	PtzSafeChnn;			//Motion事件,不能触本通道
	ULONGLONG		AlarmOut;				//触发报警输出
	ULONGLONG		Record;					//触发录像
	ULONGLONG		FullScreen;				//触发大画面报警
	TO_PTZ			ToPtz[MAX_CHNN_NUM];	//触发云台, 触发摄像头移动
	unsigned long   EmailEnable;			//是否触发EMail 报警
	unsigned long 	EmailAddr;
	char			AddrInfo[3][68];		//获取配置中前三个EMail的地址名字
	ULONGLONG		SnapCH;					//抓图通道
	ULONGLONG		ftpVideo;				//触发FTP视频上传
}ALARM_TRIGGER_INFO;

//EmailAddr:用户选择的EMail发送地址
//最低位对应发送地址1,次低位对应发送地址2,以此类推,当为1时表示用户选用此地址,为0表示没有选用
//例:如果EmailAddr = 0x03,表示他选中了第一个邮箱和第二个邮箱作为报警后的发送地址

typedef struct _wizard_info_ 
{
	//基本配置部分
	unsigned long Modified;				//是否被修改了
	unsigned long DevMaxNameLen;		//设备名长度
	unsigned long DateMode;				//日期格式
	unsigned long TimeMode;				//时间格式
	unsigned long TimeZone;				//时区
	unsigned long CurrTime;				//当前时间
	unsigned long LanguageNum;			//语言数目
	unsigned long CurrentLanguage;		//当前系统语言
	unsigned long CurrentResolution;	//当前分辨率
	unsigned long ShowWzd;				//下次是否显示向导

	//网络配置部分
	unsigned long Port;					//http端口号
	unsigned long ServerPort;			//服务端口号
	NETWORK_ADDR_INFO Addr_Info;		//地址的信息
	unsigned long DNS_MainIP;			//主DNS域名
	unsigned long DNS_SubIP;			//副DNS域名
	
	//码流相关
	NCFG_INFO_SUPPORT_FRAME_RATE *pFrameRateData; //框架数据
	unsigned long ChannelNum;
	unsigned long defauQualityLevel;			//默认质量水平
	unsigned long QualityLevelNum;				//质量位数
	unsigned long videoEncodeMode;				//视频模式
	unsigned long defaVideoEncodeMode;			//默认视频模式
	unsigned long bitStreamCount;				//位流计数
	unsigned long defaBitStreamValue;			//默认位数
	unsigned long bAllCH;
	unsigned long QualityLevelValue[32];		//画面质量
	unsigned long BitStreamValue[32];			//位流值
	NCFG_INFO_LANGUAGE Language[32];			//读取系统语言
	ENCODE_INFO_EX InfoEx[32];
	char DeviceName[36];						//设备名

	NCFG_INFO_ENCODE_SPEC_FRAME_RATE FrameRateSpec; //特殊的分辨率配置
	CHANNEL_RECP * pChRecp;
	VS_SUPBRR *pVSBit;
}WIZARD_INFO;


typedef struct _wzd_US02_info_
{
	unsigned long		Modified;				//是否被修改了
	
	//语言相关
	unsigned long		LanguageNum;			//语言数目
	unsigned long		CurrentLanguage;		//当前系统语言
	
	//时间
	unsigned long		DateMode;				//日期格式
	unsigned long		TimeMode;				//时间格式
	unsigned long		TimeZone;				//时区
	unsigned long		CurrTime;				//当前时间
	unsigned long		ShowWzd;				//下次是否显示向导
	unsigned long		sync;					//是否和服务器时间同步
	unsigned long		httpPort;				//网络端口
	unsigned long		serverPort;				//服务端口
	NCFG_INFO_LONG_NAME timeServer;			    //时间服务器地址
	unsigned long		UpDateWzdTime;			//是否延长向导时间，时间同步的时候修改
	//夏令时（DST）
	DAYLIGHT_INFO		dstInfo;				//夏令时信息
	NETWORK_ADDR_INFO   Addr_Info;				//地址信息
	
	NCFG_INFO_LANGUAGE  Language[32];			//读取系统支持语言

	char				RouterIp[32];			//获取的路由ip
	unsigned short		MobileDevice;			//选取的移动设备型号
	//DDNS设置
	unsigned long		maxUserLen;				//DDNS用户名最大长度
	unsigned long		maxPassLen;				//DDNS密码最大长度
	unsigned long		maxDomainLen;			//DDNS域名最大长度
	DDNS_INFO			ddnsInfo;				//DDNS设置
	unsigned long		ddnsCycle;				//DDNS循环周期
	unsigned char		ddnsServerName[132];	//DDNS服务器名
	DDNS_SERVER_INFO*	pDdnsSeverInfo;			//配置中的DDNS_SERVER_INFO信息
	int					ddnsServerCount;		//总共拥有的DDNS_SERVER_INFO个数
}US02_WzdInfo;

typedef struct _nat_wizard_info_ 
{
	unsigned long		Modified;				//是否被修改了
	
	//语言相关
	unsigned long		LanguageNum;			//语言数目
	unsigned long		CurrentLanguage;		//当前系统语言
	
	//时间
	unsigned long		DateMode;				//日期格式
	unsigned long		TimeMode;				//时间格式
	unsigned long		TimeZone;				//时区
	unsigned long		CurrTime;				//当前时间
	unsigned long		ShowWzd;				//下次是否显示向导
	unsigned long		sync;					//是否和服务器时间同步
	unsigned long		httpPort;				//网络端口
	unsigned long		serverPort;				//服务端口
	NCFG_INFO_LONG_NAME timeServer;			    //时间服务器地址
	unsigned long		UpDateWzdTime;			//是否延长向导时间，时间同步的时候修改

	//用户信息相关只读项目
	unsigned long		userNameMaxLen;			//读取的用户名最大长度
	unsigned long		passwordMaxLen;			//读取的密码最大长度
	//夏令时（DST）
	DAYLIGHT_INFO		dstInfo;				//夏令时信息
	NETWORK_ADDR_INFO   Addr_Info;				//地址信息
	
	NCFG_INFO_LANGUAGE  Language[32];			//读取系统支持语言

	char				RouterIp[32];			//获取的路由ip
	unsigned short		MobileDevice;			//选取的移动设备型号
	NCFG_INFO_USER		userInfo[MAX_USER_NUM]; //读取所有的用户信息
	NAT_SERVER_INFO		natInfo;				//穿透的信息
}NAT_WIZARD_INFO;

typedef struct _other_alarm_info_
{
	unsigned long	DiskAlarmFree;			//剩余磁盘容量报警

	unsigned long	Buzzer;					//触发蜂鸣器
	unsigned long	Email;					//是否触发EMail 报警
	ULONGLONG		AlarmOut;				//触发报警输出
}OTHER_ALARM_INFO;

typedef struct _wnd_startup_info_
{
	NCFG_INFO_TOOLBAR_POS	ToolbarPos;			//toolbar的位置
	unsigned long			DoDwell;			//是否跳台
	unsigned long			SplitMode;			//跳台分割模式
	unsigned long			SplitData[8][128];	//分割数据
}WND_STARTUP_INFO;

typedef struct  _wnd_back_door_info_
{
	unsigned char BootImage;	//开机画面
	unsigned char Logo;			//显示Logo
	unsigned char DDNS;			//DDNS
	unsigned char Keyboard;		//键盘
	unsigned char Mac;			//MAC
	unsigned char Hardware;		//硬件版本
	unsigned char CompanyID;	//厂家ID
	unsigned char Lang;			//语言
	unsigned char PTZ;			//修改PTZ
	unsigned char CompanyName;	//厂家名称
	unsigned char Reserve[2];	//对齐
}WND_BACK_DOOR_INFO;

#if defined(__ENVIRONMENT_WIN32__) || defined(__TDFH__) || defined(__TDNXP__) || defined(__CHIP3531__) || defined(__CHIP3535__) || defined(__CHIP3521__) || defined(__CHIP3520A__) || defined(__CHIPTI__) || defined(__CHIP3520D__) || defined(__CHIPGM__) 
#pragma pack()
#endif

#endif
