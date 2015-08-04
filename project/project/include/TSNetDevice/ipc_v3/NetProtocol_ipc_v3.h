/*
2011-05-18 这段时间修改了的协议在后面增加成 _V3
*/
#ifndef _NET_PROTOCOL_H_IPC_V3_
#define _NET_PROTOCOL_H_IPC_V3_
namespace IPC_V3
{

#include "Typedef.h"
#include "ipcamera_v3.h"

#pragma pack(4)

const int HEARTBEAT_TIMEOUT = 20;		//心跳超时时间为20秒

typedef struct _pack_cmd
{
	unsigned long cmdType;//命令编号，枚举变量NET_PROTOCOL中的值
	unsigned long cmdID;//指令序号，指令发起端设定的一个数值，对应的指令返回头中的指令序号与它完全相同
	//广播命令（没有请求的状态命令）的ID约定固定为0xffffffff（在客户端产生命令ID时不允许使用这个值）
	unsigned long cmdVer;//命令版本，可以直接根据这个版本确定这个命令的格式
	unsigned long dataLen;//数据长度
}PACKCMD;

const int PACK_HEAD_LENGTH=sizeof(PACKCMD);
//占用四字节
typedef enum _NET_PROTOCOL_H_
{
	//客户端登录
	CMD_BASENUM_LOGIN		= 0x0100,
	CMD_REQUEST_LOGIN,			//客户端请求登录,返回CMD_REPLY_LONGIN_SUCC或CMD_REPLY_LONGIN_FAIL
	CMD_REQUEST_LOGOUT,			//客户端请求登出
	CMD_REQUEST_STREAM_BRIEF,	//在用户登录前获取各码流的主要信息，后面不带结构体
	CMD_END_LOGIN,

	//服务器端回复登录信息
	CMD_BASENUM_REPLY_LOGIN	= 0x01000100,
	CMD_REPLY_LONGIN_SUCC,		//允许登录，附带设备信息
	CMD_REPLY_LONGIN_FAIL,		//拒绝登录，或者登录失败
	CMD_REPLY_STREAM_BRIEF,		//回复用户各路码流的主要信息，若干个NET_GET_STREAM_BRIEF结构体，有几个可以根据长度算出来
	CMD_END_REPLY_LOGIN,

	//客户端控制（系统级）
	CMD_BASENUM_CTRL		= 0x0200,
	CMD_REQUEST_STREAM_START,	//请求开始主码流、网络副码流、音频流
	CMD_REQUEST_STREAM_STOP,	//请求停止主码流、网络副码流、音频流
	CMD_REQUEST_KEYFRAME,		//请求关键帧
	CMD_REQUEST_SHUTDOWN,		//请求关机，成功不返回任何信息，设备会断开，失败返回错误码NET_REPLY_RESULT
	CMD_REQUEST_REBOOT,			//请求重启系统，成功不返回任何信息，设备会断开，失败返回错误码NET_REPLY_RESULT
	CMD_REQUEST_CLIENT_INFO,	//查看所有连接在DVS上客户端，回复CMD_REPLY_CLIENT_INFO
	CMD_REQUEST_CHANGE_TIME,	//修改设备时间////////////////////////////////////////
	CMD_REQUEST_SEARCH_WIFI,	//搜索无线路由器，返回CMD_REPLY_SEARCH_WIFI
	CMD_REQUEST_DATA_TO_SERVER,	//把数据发给NAT服务器	
	CMD_REQUEST_CHECK_WIFI,     //检测wifi是否连接上 panjy
	CMD_REQUEST_NTP_IMMEDIATELY,//立即NTP校时
	CMD_REQUEST_MAX_VIDEO_LEN,	//设置该客户端可以发送的最大一帧视频的长度
	CMD_END_CTRL,

	//服务器端回复控制
	CMD_BASENUM_REPLY_CTRL	= 0x01000200,
	CMD_REPLY_CLIENT_INFO,		//所有客户端的信息
	CMD_REPLY_SEARCH_WIFI,		//搜索到的无线网络列表，NET_WIFI_MODEM
	CMD_REPLY_DATA_TO_CLIENT,
	CMD_REPLY_CHECK_WIFI,       //检测是否连接上wifi
	CMD_END_REPLY_CTRL,

	//客户端通道控制
	CMD_BASENUM_CHNN_CTRL	= 0x0300,	
	CMD_REQUEST_COLOR_SET,		//调整某通道的现场图像的颜色值
	CMD_REQUEST_COLOR_CANCEL,	//获取某通道的现场图像的颜色值	
	CMD_REQUEST_SET_WHITE_BALANCE,		//设置白平衡
	CMD_REQUEST_CANCE_WHITE_BALANCE,	//取消设置的白平衡
	CMD_REQUEST_SET_DYNANIC_RANGE,		//设置宽动态
	CMD_REQUEST_CANCE_DYNAMIC_RANGE,	//取消设置的宽动态
	CMD_REQUEST_SET_DENOISE,		//设置降噪
	CMD_REQUEST_CANCE_DENOISE,	//取消设置降噪
	CMD_REQUEST_SET_IRISTYPE,		//设置自动光圈
	CMD_REQUEST_CANCE_IRISTYPE,	//取消设置自动光圈
	CMD_REQUEST_SET_SHARPEN,		//设置锐化
	CMD_REQUEST_CANCE_SHARPEN,	//取消设置锐化
	CMD_REQUEST_BAD_PIXEL_CHECK,//启动坏点检测
	CMD_REQUEST_DAY_NIGHT,      //9314设置日夜模式 panjy
	CMD_REQUEST_SET_IMAGE_MIRROR = 0x0319,	//图像镜像 unsigned char
	CMD_REQUEST_SET_IMAGE_FILP,				//图像翻转 unsigned char
	CMD_REQUEST_SET_IMAGE_ROTATO,			//图像旋转 unsigned long
	CMD_REQUEST_SET_AUTOFOCUS,				//自动对焦
	CMD_REQUEST_SET_IMAGE_DISTORTION,       //畸变校正
	CMD_END_CHNN_CTRL,

	//服务器端回复通道信息
	CMD_BASENUM_REPLY_CHNN_CTRL	= 0x01000300,
	CMD_REPLY_CHNN_CTRL_COLOR,	//回复的某个通道的颜色值
	CMD_REPLY_BAD_PIXEL_CHECK,//坏点检测结束回复
	CMD_END_REPLY_CHNN_CTRL,

	//客户端配置
	CMD_BASENUM_CFG			= 0x0400,
	CMD_REQUEST_CFG_DEFAULT,	//恢复默认配置
	CMD_REQUEST_CFG_NET,		//通过多播设置网络相关的几个参数
	CMD_REQUEST_CFG_MAIL_TEST,	//测试mail配置
	CMD_REQUEST_CFG_FTP_TEST,	//测试ftp配置
	//新增的配置进入、请求、保存、离开命令
	CMD_REQUEST_CFG_ENTER,		//进入配置，每次只允许一个客户端进入配置，已有客户端正在配置时返回失败
	CMD_REQUEST_CFG_LEAVE,		//离开配置
	CMD_REQUEST_CFG_GET,		//请求配置
	CMD_REQUEST_CFG_SET,		//保存配置
	CMD_REQUEST_CFG_NET_V2,		//通过多播设置网络相关的几个参数
	CMD_REQUEST_CFG_ADD_IPHONE_TOKEN, //回复跟 NET_CTRL_FAIL
	CMD_REQUEST_CFG_DEL_IPHONE_TOKEN, //回复跟 NET_CTRL_FAIL
	CMD_REQUEST_CFG_SUPPORT_IPHONE_PUSH,//是否支持iphonepush,
	CMD_REQUEST_CFG_EXIST_IPHONE_TOKEN,//设备中是否已经存在了该令牌
	CMD_END_CFG,

	//服务器端回复配置信息
	CMD_BASENUM_REPLY_CFG		= 0x01000400,
	CMD_REPLY_CFG_SUCC,			//允许进入配置时回复成功，附带配置页面数据，对应于CMD_REQUEST_CFG_ENTER
	CMD_REPLY_CFG_FAIL,			//回复配置，附带失败码，对应于CMD_REQUEST_CFG_ENTER
	CMD_REPLY_CFG_MAIL_TEST,	//测试mail的结果
	CMD_REPLY_CFG_FTP_TEST,		//测试ftp的结果
	CMD_REPLY_CFG_DATA,			//回复配置数据,对应于CMD_REQUEST_CFG_GET
	CMD_REPLY_CFG_DEFAULT,		//回复加载默认配置命令，对应于CMD_REQUEST_CFG_DEFAULT
	CMD_REPLY_CFG_ADD_IPHONE_TOKEN,
	CMD_REPLY_CFG_DEL_IPHONE_TOKEN,
	CMD_REPLY_CFG_SUPPORT_IPHONE_PUSH,
	CMD_REPLY_CFG_EXIST_IPHONE_TOKEN,
	CMD_END_REPLY_CFG,

	//SD卡相关命令
	CMD_BASENUM_DATA		= 0x0500,
	CMD_REQUEST_DATA_SD_ALL_DATE,		//SD卡中哪些天有数据
	CMD_REQUEST_DATA_SD_SEARCH_NAME,	//按日期通道类型搜索SD卡中的内容，返回图片的文件名
	CMD_REQUEST_DATA_SD_GET_PIC,		//获取指定的图片	
	CMD_REQUEST_DISK_FORMAT,		//格式化某个磁盘
	CMD_REQUEST_DISK_POP_SDCARD,	//弹出SD卡
	CMD_END_DATA,

	//服务器端回复数据
	CMD_BASENUM_REPLY_SEARCH	= 0x01000500,
	CMD_REPLY_DATA_SD_ALL_DATE,		//SD卡中哪些天有数据
	CMD_REPLY_DATA_SD_SEARCH_NAME,	//按日期通道类型搜索SD卡中的内容，返回图片的文件名
	CMD_REPLY_DATA_SD_GET_PIC,		//获取指定的图片
	CMD_REPLY_DISK_FAIL,		//回复磁盘管理，附带失败码
	CMD_REPLY_FORMAT_FAIL,		//格式化失败
	CMD_REPLY_FORMAT_PERCENT,	//格式化进度
	CMD_REPLY_POP_SDCARD_SUCC,	//弹出SD成功
	CMD_REPLY_POP_SDCARD_FAIL,	//弹出SD失败
	CMD_END_REPLY_SEARCH,

	//客户端云台控制
	CMD_BASENUM_PTZ			= 0x0600,
	CMD_REQUEST_PTZ_START,		//请求云台控制
	CMD_REQUEST_PTZ_MOVE,		//云台控制命令
	CMD_REQUEST_PTZ_ADJUST,		//云台preset，cruise调整
	CMD_REQUEST_PTZ_STOP,		//结束云台控制
	CMD_REQUEST_PTZ_WRITECOMM,	//把客户端传来的数据直接写串口控制云台，hebingxieyi
	CMD_END_PTZ,

	//服务器回复云台预置点、巡航信息
	CMD_BASENUM_REPLY_PTZ       = 0x01000600,
	CMD_REPLY_PTZ_PRESET_INFO,      //-----应答云台预置点信息，已废除
	CMD_REPLY_PTZ_START_SUCC,       //请求开始云台返回成功
	CMD_REPLY_PTZ_START_FAIL,       //请求开始云台返回失败（附带错误原因）
	CMD_END_REPLY_PTZ,

	//对讲相关命令
	CMD_BASENUM_TALK = 0x0700,
	CMD_REQUEST_TALK_START,		    //请求开始1v1对讲
	CMD_REQUEST_TALK_STREAM,		//客户端给DVS发送对讲的音频数据
	CMD_REQUEST_TALK_STOP,			//停止1v1对讲
	CMD_END_TALK,             

	//语音对象相关回复
	CMD_BASENUM_REPLYY_TALK = 0x01000700,
	CMD_REPLY_TALK_START_SUCC,		    //回复请求开始1v1对讲成功    
	CMD_REPLY_TALK_START_FAIL,		    //回复请求开始1v1对讲失败    
	CMD_END_REPLY_TALK, 

	//客户端系统文件升级控制
	CMD_BASENUM_SYSTEM_UPDATE		= 0x0800,
	CMD_REQUEST_SYSTEM_UPDATE_DATA,		//系统文件升级包数据	
	CMD_REQUEST_UBOOT_UPDATE_DATA,		//升级uboot文件
	CMD_END_SYSTEM_UPDATE,

	//服务器端回复内核升级
	CMD_BASENUM_REPLY_SYSTEM_UPDATE	= 0x01000800,
	CMD_REPLY_SYSTEM_UPDATE_FAIL,		//回复系统文件升级失败信息
	CMD_REPLY_SYSTEM_UPDATE_PERCENT,	//回复系统文件升级进度
	CMD_END_REPLY_SYSTEM_UPDATE,

	//客户端升级控制
	CMD_BASENUM_UPDATE		= 0x0a00,
	CMD_REQUEST_UPDATE_START,	//请求软件升级
	CMD_REQUEST_UPDATE_DATA,	//软件升级包数据
	CMD_REQUEST_UPDATE_END,		//放弃升级
	CMD_END_UPDATE,

	//服务器端回复升级
	CMD_BASENUM_REPLY_UPDATE	= 0x01000a00,
	CMD_REPLY_CAN_UPDATE,		//回复表示可以升级
	CMD_REPLY_NOT_UPDATE,		//回复表示不能升级
	CMD_REPLY_UPDATE_FAIL,		//回复升级失败信息
	CMD_REPLY_UPDATE_PERCENT,	//回复升级进度
	CMD_END_REPLY_UPDATE,

	//服务器端发送状态信息
	CMD_BASENUM_REPLY_STATUS	= 0x01000b00,
	CMD_REPLY_MOTION,			//发送motion侦测状态
	CMD_REPLY_SENSOR,			//发送sensor侦测状态	
	CMD_REPLY_CHNN_NAME,
	CMD_REPLY_PTZ_PRESET_NAME,	//发送某个通道的所有预置点名称
	CMD_REPLY_PTZ_CURISE_NAME,	//发送某个通道的所有巡航线名称
	CMD_REPLY_PTZ,				//云台是否配置了协议
	CMD_REPLY_SD_INFO,			//发送SD卡的信息
	CMD_END_REPLY_STATUS,
	
	//服务器端发送视频/音频数据
	CMD_BASENUM_REPLY_STREAM	= 0x01000c00,	
	CMD_REPLY_LIVE_STREAM,		//现场数据
	CMD_REPLY_PLAY_STREAM,		//回放数据
	CMD_REPLY_TALK_STREAM,
	CMD_END_REPLY_STREAM,
}NET_PROTOCOL;

typedef enum _net_ctrl_fail
{
	NETERR_SUCCESS=0,				//成功，所有地方的成功都可以使用此值
	NETERR_FAIL=0xffffffff,			//普通失败返回值，在不清楚原因的情况下返回此值
	NETERR_LOGIN_FAIL_VERSION_ERR=1,//登陆时版本错误
	NETERR_LOGIN_FAIL_USERPWD_ERR=2,//登陆时用户名或密码错误
	NETERR_LOGIN_FAIL_OVERFLOW	 =3,//登陆网络超载，如果登陆超过一定数量后停止listen,此错误码就用不到
	NETERR_LOGIN_FAIL_REFUSE	 =4,//登陆拒绝，可能是被过滤的IP的原因
	NETERR_OPERATE_FAIL_NOAUTHORITY=5,//进操作没有权限，如配置、云台、回放
	NETERR_OPERATE_FAIL_OPERATING  =6,//只能一个人操作时返回已经有人在操作如配置、云台控制、回放
	NETERR_NO_DISK	=7,				//无磁盘，在格式化、查询事件等和磁盘相关的操作时因为没有磁盘而失败时返回此值
	NETERR_NO_EVENT = 8,			//无事件
	NETERR_FORMATERR_RECORDING	=9,	//磁盘格式化错误，正在录像
	NETERR_NOTSUPPORT=10,			//不支持的操作
	NETERR_MUST_ENTER_CFG=11,		//在没有进入配置的情况下进行入得到、保存配置的操作时返回的错误码
	NETERR_DISK_IO_ERR=12,			//磁盘读写的IO错误
	NETERR_LOGIN_FAIL_MACERR	 =13,//MAC地址不匹配，本来可以归到NETERR_LOGIN_FAIL_REFUSE
	NETERR_LOGIN_FAIL_UNSUPPORT_CLIENT=14,//登陆失败，原始版本只支持CMS登陆
	NETERR_ADD_IPHONE_TOKEN_DUPLICATE=15, //IPHONE的令牌重复了
	NETERR_DEL_IPHONE_TOKEN_NONE=16,			//没有添加过该令牌
	NETERR_NO_EXIST_IPHONE_TOKEN = 17,
	NETERR_EXIST_IPHONE_TOKEN = 18,
}NET_CTRL_FAIL;

const unsigned long NET_PROTOCOL_VER	= 0x00000003;

struct  NET_GET_STREAM_BRIEF
{
	char	videostream[320];	
};

//用户登录时用的结构体
typedef struct _net_login_info
{
	unsigned long	connectType;			//连接类型

	char			username[32];			//用户姓名
	char			password[32];			//用户密码
	char			computerName[28];		//计算机名称

	unsigned char   IP[8];					//客户端用来连接的IP地址,可以使IPV6,第一位表示IP的位数4或6
	unsigned char   MAC[6];					//网卡Mac地址
	unsigned char	resv[2];//保留字节，对齐用
	unsigned long	netprotocolver;			//客户端目前使用的网络协议版本
}NET_LOGIN_INFO, *PNET_LOGIN_INFO;

typedef enum _enum_connect_type{
	CONNECTTYPE_CTRL_CENTER,
	CONNECTTYPE_MOBILE_PHONE,
	CONNECTTYPE_IE
}ENUM_CONNECT_TYPE;

//用户登录成功时，回复用的结构体
struct NET_LOGIN_SUCCESS_REPLY
{	
	unsigned long	ConfigDataLen;				//配置结构长度

	unsigned long	PTZPresetNum;				//预置点个数
	unsigned long	PTZCruiseNum;				//巡航线个数
	unsigned long   PTZPresetNumForCruise;		//巡航线支持的最大预置点个数

	//预置点名字最大长度，每个产品可能不同，在编辑界面控制,预置点保存和获得都按通道、序号来就只能放到这了
	unsigned long	PresetNameMaxLen;

	//巡游名字最大长度，每个产品可能不同，在编辑界面控制,巡游保存和获得都按通道、序号来就只能放到这了
	unsigned long	CruiseNameMaxLen;

	unsigned char	bSupportPTZ;				//是否支持PTZ功能			1 支持  0 不支持
	unsigned char	videoFormat;				//当前制式   0 VIDEO_FORMAT_PAL  1 VIDEO_FORMAT_NTSC
	unsigned char	sensorInNum;				//Sensor In 数量
	unsigned char	alarmOutNum ;				//报警输出数量

	unsigned char	ucStreamCount;				//视频有几路流	
	unsigned char	bSupportSnap;			//产品是否支持SD卡抓图功能  1 支持  0 不支持
	unsigned char	noused;
	unsigned char	ucLiveAudioStream;			//是否有音频的现场流	1 有，0 没有

	unsigned char	ucTalkAudioStream;			//是否有音频对讲		1 有，0 没有
	//IPCAMERA_AUDIO_ENCODE_TYPE_RAW   IPCAMERA_AUDIO_ENCODE_TYPE_G726
	unsigned char	audioEncodeType;			//音频编码格式	
	unsigned char	audioBitWidth;				//音频位宽
	unsigned char	audioChannel;				//几通道
	unsigned long	dwAudioSample;				//音频的采样率

	//USER_GROUP_NONE		USER_GROUP_ADMIN	USER_GROUP_ADVANCE	USER_GROUP_NORMAL
	unsigned long	UserRight;					//用户网络权限或者用户级别	
	unsigned long	softwareVer;				//Server端软件版本
	unsigned long	buildDate;					//Server端软件创建日期

	unsigned char	MAC[6];						//ipcamera的网卡物理地址
	char	        deviceName[34];				//设备名称

	unsigned long	nCustomerID;			//客户号，根据此确定客户端的客户订制特性，0为中性

	//这几个颜色是给NVR保留的
	unsigned char	defBrightness;
	unsigned char	defContrast;
	unsigned char 	defHue;
	unsigned char	defSaturation;

	unsigned char noused1[100];		//以后功能可用此未用字节
};

//CMD_REQUEST_STREAM_START	CMD_REQUEST_STREAM_STOP
struct NET_REQUEST_STREAM_CTRL
{
	unsigned char	ucStreamType;	//流类型，视频 声音 图片
	unsigned char	ucStreamIndex;	// 流序号，从0开始
	unsigned char	noused[2];
};

//	CMD_REQUEST_KEYFRAME
struct NET_REQUEST_I_FRAME
{
	unsigned char	ucStreamIndex;	// 流序号，从0开始
	unsigned char	noused[3];
};

typedef struct _net_chnn_color
{
	unsigned long brightness;
	unsigned long contrast;
	unsigned long saturation;
	unsigned long hue;
}NET_CHNN_COLOR;

typedef struct _net_chnn_white_balance
{
	unsigned char ucMode;// 0 自动 1 手动 2 户外 3 室内
	unsigned int red;	//红色增益
	unsigned int blue;	//蓝色增益	
}NET_CHNN_WHITE_BALANCE;

typedef struct _net_chnn_dynamic_range
{
	unsigned char ucChn;
	unsigned char ucMode;// 0 关闭 1 开启
	unsigned char ucValue;
	unsigned char unused;
}NET_CHNN_DYNAMIC_RANGE;

typedef struct _net_denoise
{
	unsigned char ucMode;// 0 关闭 1 开启
	unsigned char value;
	unsigned char unused[2];
}NET_DENOISE;

typedef struct _net_iristype
{
	unsigned char ucMode;// 0 关闭 1 开启
	unsigned char unused[3];
}NET_IRISTYPE;

typedef struct _net_sharpen
{
	unsigned char ucMode;// 0 关闭 1 开启
	unsigned char value;
	unsigned char unused[2];
}NET_SHARPEN;

typedef struct _net_day_night //panjy
{
	unsigned char ucMode; //color b/w auto
	unsigned char ucDelay; //auto模式下的延迟时间 0-255, default 4
	unsigned char ucDayTh; //日阈值 0-255 default 5
	unsigned char ucNightTh; //0 - 255 default 3
}NET_DAY_NIGHT;

typedef struct _net_chnn_name
{
	unsigned long	nameLen;
	char			name [36];
}NET_CHNN_NAME;

typedef struct _net_cfg_info
{
	unsigned long	len;			//配置数据的长度
	unsigned long	cfgType;		//配置类型
}NET_CFG_INFO;

typedef struct _net_data_search
{
	unsigned long		searchType;	//搜索类型
	unsigned long		streamID;	//要求发送的stream ID
	ULONGLONG       	videoCH;	//视频通道条件
	ULONGLONG           audioCH;    //音频通道条件
	LONGLONG			startTime;	//开始时间
	LONGLONG			endTime;	//结束时间

	unsigned long		dataType; //数据类型 是用于回放还是备份：NET_DATA_SEARCH_TYPE
	unsigned char		RecordTypeMASK;//录像类型组合
	unsigned char		reserveData[3];//对齐成4BYTE
}NET_DATA_SEARCH;

typedef struct _net_data_info
{
	unsigned long	dataType;		//数据类型
	unsigned long	end;			//1 表示最后一批
	unsigned long   index;          //索引
	unsigned long	dataNum;		//数据个数
}NET_DATA_INFO;

typedef struct _net_ptz_ctrl
{
	unsigned long	speed;			//控制速度
	unsigned long	cmdType;		//命令类型
	unsigned long	cmdPara;		//命令参数
}NET_PTZ_CTRL;

typedef struct _net_ptz_info
{
	unsigned long   presetNum;
	unsigned long   presetNameLen;
	unsigned long   curiseNum;
	unsigned long   curiseNameLen;
	unsigned long	curisePointNum;
}NET_PTZ_INFO;

typedef struct _net_status
{
	unsigned long chnn;		
	unsigned long status;//用位表示各通道的状态
}NET_STATUS;

typedef struct _net_reply
{
	DWORD	dwResult;
}
NET_REPLY_RESULT;

typedef struct _net_reply_ex
{
	DWORD dwResult;
	char	szMessage[1020];
}
NET_REPLY_RESULT_EX;

//dwOtherStatus
const unsigned long STATUS_CONFIG_BASE = 0x00000001;	// 正在配置基本配置
const unsigned long STATUS_TALKING = 0x00000002;		// 正在对讲
const unsigned long STATUS_CONFIG_PTZ = 0x00000004;		// 正在配置云台
const unsigned long STATUS_OPERATE_SD = 0x00000008;		// 正在操作SD卡
const unsigned long STATUS_UPDATING = 0x00000010;		// 正在升级

typedef struct _net_echo_client_info
{
	unsigned long	dwIP;				//连接的客户端的IP地址

	unsigned short	wPort;				//客户端远程机的端口号（主机字节序）
	unsigned short	Unused;				

	//	char			szUserName[DVS_USER_NAME_LEN];	//用户名
	char			szUserName[16];	//用户名
	unsigned long	dwUserType;	
	unsigned long	dwOtherStatus;		//	STATUS_CONFIG_BASE  STATUS_TALKING  STATUS_CONFIG_PTZ 
}NET_ECHO_CLIENT_INFO;

//CMD_REQUEST_CLIENT_INFO	 CMD_REPLY_ALL_CLIENT_INFO
typedef struct _net_all_client_info
{
	unsigned long			dwCount;
	//	NET_ECHO_CLIENT_INFO	clientInfo[MAX_CLIENT_LINK];
	NET_ECHO_CLIENT_INFO	clientInfo[8];
}NET_ALL_CLIENT_INFO;

typedef struct _net_sdcard_info
{
	unsigned long	hasSDCard;	//0 没有SD卡，1 有SD卡,但是未初始化 2 有SD卡，能正常使用
	unsigned long	capability; //SD卡的总容量，以MByte为单位
	unsigned long	usedRec;	//使用了的录象空间，以MByte为单位
	unsigned long	spareRec;	//剩余的录象空间，以MByte为单位
	unsigned long	usedPic;	//使用了的图片空间，以MByte为单位
	unsigned long	sparePic;	//剩余的图片空间，以MByte为单位
}NET_SDCARD_INFO;

//CMD_REQUEST_PTZ_WRITECOMM
typedef struct _net_write_comm
{
	unsigned char			byChnn;
	unsigned char			Unused[3];
	unsigned long			dwLen;		//这里必须填4的整数倍，和手机软件兼容
	unsigned char			szContent[1];		//这里必须使用4的整数倍，和手机软件兼容
}NET_WRITE_COMM;

//CMD_REQUEST_CHANGE_TIME
typedef struct _net_change_time
{		
	LONGLONG		llTime;
}NET_CHANGE_TIME;

typedef struct _net_automail_info
{
	char	szDestMail[64];				//目的邮箱的地址 例如：aaa@hotmail.com
	char	szSrcMail[64];				//发送邮箱的地址 例如：bbb@163.com
	char	szUserName[64];				//邮箱用户名
	char	szMailPwd[64];				//邮箱密码
	char	szMailServer[64];			//邮箱服务器地址 例如 smtp.163.com
	unsigned char ucSSL;
	char	noUsed;
	unsigned short nPort;
}NET_AUTOMAIL_INFO;

typedef struct _net_ftp_info
{
	char		szHostName[64];			//服务器地址或IP
	unsigned short  nPort;				//FTP服务端口号,一般为21
	char		szUnused[2];
	char		szFtpUserName[64];		//FTP用户名
	char		szFtpPasswd[64];		//FTP用户密码
	char		szPath[64];
}NET_FTP_INFO;

typedef struct _net_cfg_page_info
{
	DWORD  pageIndex;  //////////对应的页索引信息
}NET_CFG_PAGE_INFO;

typedef struct _net_pic_name
{
	char	name[40];
}NET_PIC_NAME;

typedef struct _net_manual_rec_info
{
	unsigned long bStartRec; //0 关闭手动录像，非0开启手动录像
	unsigned long dwChannel; //开启关闭手动录像的通道号
}NET_MANUAL_REC_INFO;

typedef struct _net_iphone_push
{
        unsigned long ifsupport;//0不支持iphonepush,    1支持
        unsigned long type;//0位表示移动侦测,第1位表示传感器

}NET_IPHONE_PUSH;

#pragma pack()

}

#endif //_NET_PROTOCOL_H_
