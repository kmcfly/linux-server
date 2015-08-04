
#ifndef _DVRDVSDEFINE_
#define _DVRDVSDEFINE_
#include "Typedef.h"
#include "mylist.h"

#ifdef __ENVIRONMENT_WIN32__
#pragma pack(4)
#endif

//2010-01-26 14:59:00 YSW
//保证该文件里的结构体保持4字节对齐
#include "device.h"

typedef enum _dvr_system_status
{
	DVR_SYSTEM_RUNING,		//运行中
	DVR_SYSTEM_REBOOT,		//系统重启
	DVR_SYSTEM_REDRAW,		//重新初始化界面
	DVR_SYSTEM_LOGOUT,		//需要退出到登录界面
	DVR_SYSTEM_EXIT			//退出系统
}DVR_SYSTEM_STATUS;

//视频流类型,要将此改掉
typedef enum _TVT_ENUM_VIDEO_STREAM_TYPE
{
	VIDEO_STREAM_NONE		= 0x00000000,		//无码流
	VIDEO_STREAM_SLOW_NET	= 0x00000001,		//慢码流
	VIDEO_STREAM_FAST_NET	= 0x00000002,		//快码流
	VIDEO_STREAM_PLAYBACK	= 0x00000004,		//回放流
}TVT_ENUM_VIDEO_STREAM_TYPE;

typedef enum _DDNS_SERVER_TYPE
{
	DDNS_NONE =0,
	DDNS_88IP,
	DDNS_DNS2P,
	DDNS_MEIBU,
	DDNS_DYNDNS,
	DDNS_NOIPDNS,
	DDNS_DVRDYDNS,
	DDNS_MINTDNS,
	DDNS_MYQSEE, //专用于US02的DDNS,采用MINTDNS,服务器myq-see.com
	DDNS_DVRLIS, 
	DDNS_EASTERNDNS,
	DDNS_NEWDDNS,//专用于US09的DDNS,采用MINTDNS,服务器www.newddns.com
	DDNS_VISIONICA,
	DDNS_TVT_IP_SERVER,////同为公司的IP Server服务器 
	DDNS_END,
}DDNS_SERVER_TYPE;

//外部键盘类型
typedef enum _keyboard_type
{
	KEYBOARD_1UA	= 0x00, 
	KEYBOARD_1UB,
	KEYBOARD_1UC,
	KEYBOARD_1UD,
	KEYBOARD_1UE,
	KEYBOARD_1UF,
	KEYBOARD_1UG,
	KEYBOARD_1US,  //1US旧版
	KEYBOARD_1UT,
	KEYBOARD_SUA,
	KEYBOARD_SUB,
	KEYBOARD_SUC,
	KEYBOARD_1UTW,
	KEYBOARD_1USS,  //1US新版 
	KEYBOARD_15U,  //1.5U按键板 
	KEYBOARD_1UV,
	KEYBOARD_1UH,
	KEYBOARD_1UI,	
	KEYBOARD_2UA  = 0x30,
	KEYBOARD_2UB,
	KEYBOARD_2UC,
	KEYBOARD_2UD,
	KEYBOARD_TWOEM = 0x60,
	KEYBOARD_MY1003K,
}KEYBOARD_TYPE;

typedef enum _video_out_device_type
{
	VIDEO_OUT_DEVICE_HDMI	= 0x0001,
	VIDEO_OUT_DEVICE_VGA	= 0x0002,
	VIDEO_OUT_DEVICE_CVBS	= 0x0004
}VIDEO_OUT_DEVICE_TYPE;

typedef enum _transparency
{
	TRANSPARENCY_100	= 0x00,
	TRANSPARENCY_75		= 0x40,
	TRANSPARENCY_50		= 0x80,
	TRANSPARENCY_25		= 0xdc,
	TRANSPARENCY_0		= 0xff,
}TRANSPARENCY;

typedef enum _play_speed
{
	PLAY_SPEED_1_32X = 0,
	PLAY_SPEED_1_16X,
	PLAY_SPEED_1_8X,	//1/8
	PLAY_SPEED_1_4X,	//1/4
	PLAY_SPEED_1_2X,	//1/2
	PLAY_SPEED_1X,		//1
	PLAY_SPEED_2X,
	PLAY_SPEED_4X,
	PLAY_SPEED_8X,
	PLAY_SPEED_16X,
	PLAY_SPEED_32X,
	PLAY_SPEED_NONE,
}PLAY_SPEED;

typedef enum _image_quality
{
	IMAGE_QUALITY_LOWEST	= 0x01,
	IMAGE_QUALITY_LOWER		= 0x02,
	IMAGE_QUALITY_LOW		= 0x03,
	IMAGE_QUALITY_MEDIUM	= 0x04,
	IMAGE_QUALITY_HEIGHTER	= 0x05,
	IMAGE_QUALITY_HEIGHTEST	= 0x06
}IMAGE_QUALITY;

//VGA分辨率，既可以用于保存也可以用于支持功能参数,'
//注意：要按位表示
typedef enum _vga_resolution
{
	VGA_640X480		= 0x0001,//(640<<16)+(480),
	VGA_720X480		= 0x0002,//(720<<16)+(480),
	VGA_720X576		= 0x0004,//(720<<16)+(576),
	VGA_800X600		= 0x0008,//(800<<16)+(600),
	VGA_1024X768	= 0x0010,//(1024<<16)+(768),
	VGA_1280X960	= 0x0020,//(1280<<16)+(960),
	VGA_1280X1024	= 0x0040,//(1280<<16)+(1024),
	VGA_1920X1080	= 0x0080,//(1920<<16)+(1080)
}VGA_RESOLUTION;

typedef struct _mac
{
	unsigned char mac [8];
}MAC;

//记录年月日的结构体
typedef struct _dvr_date
{
	unsigned char  mday;
	unsigned char  month;
	unsigned short year;
	unsigned long  time;
}DVR_DATE;

//显示年月日的模式
typedef enum _date_mode
{
	DATE_MODE_YMD	= 0x01,		//年月日模式
	DATE_MODE_MDY	= 0x02,		//月日年模式
	DATE_MODE_DMY	= 0x03		//日月年模式
}DATE_MODE;

//显示时间格式
typedef enum _time_mode
{
	TIME_MODE_12	= 0x01,		//12小时
	TIME_MODE_24	= 0x02,		//24小时
}TIME_MODE;

typedef enum _daylight_type
{
	DAYLIGHT_TYPE_WEEK,
	DAYLIGHT_TYPE_DAY,
}DAYLIGHT_TYPE;


typedef enum _time_zone_name
{
	TIME_ZONE_GMT_D12=0,
	TIME_ZONE_GMT_D11,
	TIME_ZONE_GMT_D10,
	TIME_ZONE_GMT_D9,
	TIME_ZONE_GMT_D8,
	TIME_ZONE_GMT_D7,
	TIME_ZONE_GMT_D6,
	TIME_ZONE_GMT_D5,
	TIME_ZONE_GMT_D4_30,
	TIME_ZONE_GMT_D4,
	TIME_ZONE_GMT_D3_30,
	TIME_ZONE_GMT_D3,
	TIME_ZONE_GMT_D2,
	TIME_ZONE_GMT_D1,
	TIME_ZONE_GMT,
	TIME_ZONE_GMT_A1,
	TIME_ZONE_GMT_A2,
	TIME_ZONE_GMT_A3,
	TIME_ZONE_GMT_A3_30,
	TIME_ZONE_GMT_A4,
	TIME_ZONE_GMT_A4_30,
	TIME_ZONE_GMT_A5,
	TIME_ZONE_GMT_A5_30,
	TIME_ZONE_GMT_A5_45,
	TIME_ZONE_GMT_A6,
	TIME_ZONE_GMT_A6_30,
	TIME_ZONE_GMT_A7,
	TIME_ZONE_GMT_A8,
	TIME_ZONE_GMT_A9,	
	TIME_ZONE_GMT_A9_30,
	TIME_ZONE_GMT_A10,
	TIME_ZONE_GMT_A11,
	TIME_ZONE_GMT_A12,
	TIME_ZONE_GMT_A13,
}TIMEZOMENAME;

typedef struct _osd_info
{
	unsigned short x;			//
	unsigned short y;			//
	unsigned long	bEnable;	//是否显示？
}OSD_INFO;

typedef struct _cover_info
{
	unsigned short x;
	unsigned short y;
	unsigned short cx;
	unsigned short cy;

	unsigned long color;
	unsigned long bCover;
}COVER_AREA;

const unsigned char MAX_COVER_AREA_NUM = 3;
typedef struct _camera_cover_info
{
	COVER_AREA area[MAX_COVER_AREA_NUM];
}CAMERA_COVER_INFO;

typedef enum _display_dev_mode
{
	DISPLAY_DEV_MINOR = 0,
	DISPLAY_DEV_MAJOR,
}DISPLAY_DEV_MODE;

typedef struct _dispaly_dev_info 
{
	unsigned short  devId;			//设备编号
	unsigned short  videoType;		//设备类型VIDEO_OUT_TYPE

	unsigned short	bValid;			//该显示设备是否接入
	unsigned short	devMode;		//主输出，或辅助输出DISPLAY_DEV_MODE

	unsigned short	width;			//设备的分辨率
	unsigned short	height;			//
}DISPLAY_DEV_INFO;

typedef struct _daylight_info
{
	unsigned char InMonth;		//第几月进入DST
	unsigned char InMday;		//第几日进入DST（日期方式有效）
	unsigned char OutMonth;		//第几月出DST
	unsigned char OutMday;		//第几日出DST（日期方式有效）

	unsigned char InWeekIndex;	//第几周进入DST（周方式有效）
	unsigned char InWday;		//星期几进入DST（周方式有效）
	unsigned char OutWeekIndex;	//第几周出DST（周方式有效）
	unsigned char OutWday;		//星期几出DST（周方式有效）

	unsigned short InYear;		//进入DST的年份，不定有用，但是放在此，刚好可以对齐结构体，所以保留。
	unsigned short OutYear;		//出DST的年份，不定有用，但是放在此，刚好可以对齐结构体，所以保留。

	unsigned short enable;		//是否开启夏令时功能
	unsigned short type;		//夏令时设置方式：周或者日期方式

	unsigned long InSecond;		//进入DST一天内的秒偏移（0～86399），可转换成时分秒
	unsigned long OutSecond;	//出DST一天内的秒偏移（0～86399），可转换成时分秒

	unsigned long offSet;		//夏令时的偏移量（0～86399）秒。

}DAYLIGHT_INFO;

typedef struct _zone_time_info
{
	unsigned short UseZone; //是否使用时区,时间
	unsigned short UseTime;

	unsigned char TimeZone; //时区
	unsigned char hour;
	unsigned char min;
	unsigned char sec;

	unsigned char mday;
	unsigned char month;
	unsigned short year;//2008~2025
}ZONE_TIME_INFO;

typedef enum _view_split_mode
{
	VIEW_SPLIT_1X1	= 0x00010000,	//1X1

	VIEW_SPLIT_2X2	= 0x00020000,	//2X2
	
	VIEW_SPLIT_1A2	= 0x00040000,	//2X3
	VIEW_SPLIT_2X3,

	VIEW_SPLIT_1A5	= 0x00080000,	//3X3
	VIEW_SPLIT_3X3,
	
	VIEW_SPLIT_1A7	= 0x00100000,	//4X4
	VIEW_SPLIT_1A12,
	VIEW_SPLIT_4X4,

	VIEW_SPLIT_2A6	= 0x00200000,	//4X6
	VIEW_SPLIT_4X6,

	VIEW_SPLIT_1A9	= 0x00400000,	//5X5
	VIEW_SPLIT_4A9,
	VIEW_SPLIT_1A16,
	VIEW_SPLIT_4A16,
	VIEW_SPLIT_5X5,

	VIEW_SPLIT_1A11	= 0x00800000,	//6X6
	VIEW_SPLIT_1A20,
	VIEW_SPLIT_4A20,
	VIEW_SPLIT_6X6,
}VIEW_SPLIT_MODE;

inline unsigned char SplitModeToNum(VIEW_SPLIT_MODE mode)
{
	unsigned char num = 1;
	switch(mode)
	{
	case VIEW_SPLIT_2X2:
		num = 4;
		break;
	case VIEW_SPLIT_2X3:
		num = 6;
		break;
	case VIEW_SPLIT_1A5:
		num = 6;
		break;
	case VIEW_SPLIT_3X3:
		num = 9;
		break;
	case VIEW_SPLIT_1A7:
		num = 8;
		break;
	case VIEW_SPLIT_1A12:
		num = 13;
		break;
	case VIEW_SPLIT_4X4:
		num = 16;
		break;
	case VIEW_SPLIT_4X6:
		num = 24;
		break;
	case VIEW_SPLIT_5X5:
		num = 25;
		break;
	case VIEW_SPLIT_6X6:
		num = 36;
		break;
	default:
		num = 1;
		break;
	}

	return num;
}

typedef struct _zoom_info
{
	unsigned short x;
	unsigned short y;
	unsigned short width;
	unsigned short height;
}ZOOM_INFO;

//报警类型
typedef enum _alarm_type
{
	ALARM_TYPE_NULL         = 0x0000,
	ALARM_TYPE_MONTION      = 0x0001,
	ALARM_TYPE_SENSOR       = 0x0002,
	ALARM_TYPE_VIDEOLOSS    = 0x0004,
	ALARM_TYPE_SPEED        = 0x0008,

	ALARM_TYPE_DISK_FULL    = 0x0010,
	ALARM_TYPE_IP_CONFLICT  = 0x0020,
	ALARM_TYPE_DISCONNET    = 0x0040,
	ALARM_TYPE_DISK_WARNING   = 0x0080,

	ALARM_TYPE_TEST         = 0xffffffff,
}ALARM_TYPE;

//图片的属性
typedef struct _snap_pic_prority
{
	unsigned short  diskIndex;
	unsigned short  nLogPos;

	unsigned long   nPos;
	LONGLONG        snapTime;
	
	unsigned short  nChannel;   //通道号
	unsigned short  nType;      //报警类型	
	unsigned short  bLock;      //是否锁定
	unsigned short  bFill;      //是否已填充
	unsigned long   nLen;
}SNAP_PIC_PRORITY;

//由于其他报警的处理方式是相通的，所以采用模拟通道的做法来统一处理。
//这样就可以使用AlarmIn作为基类，再派生一个其他报警处理类就可以了。
//所以所有类型必须从零递增，不能间断，后面需新增也只能从尾部加入。
typedef enum _other_alarm_type
{
	OTHER_ALARM_TYPE_DISK_FULL,
	OTHER_ALARM_TYPE_IP_CONFLICT,
	OTHER_ALARM_TYPE_DISCONNET,
	OTHER_ALARM_TYPE_DISK_WARNING,
	OTHER_ALARM_TYPE_NUM
}OTHER_ALARM_TYPE;

//录像数据类型
typedef enum _record_type
{
	RECORD_TYPE_NULL		= 0x0000,

	RECORD_TYPE_MANUAL		= 0x0001,		//手动录像
	RECORD_TYPE_SCHEDULE	= 0x0002,		//排程录像
	RECORD_TYPE_MOTION		= 0x0004,		//移动侦测录像
	RECORD_TYPE_SENSOR		= 0x0008,		//传感器报警录像

	RECORD_TYPE_END			= 0x0010,		//表示结束，必须在最后
	RECORD_TYPE_ALL			= 0xffff
}RECORD_TYPE;

//FILE_INFO_EX和RECLOG_INFO_EX中的type的从0-16位用于记录录像类型。
//第32位用于记录是否锁定。
//第31位用于表示记录只读属性，注意：只有在搜索数据时，才只是在搜索到的结果记录中，给该位赋予一定的意义。
//第30位用于表示记录正在写，注意：只有在搜索数据时，才只是在搜索到的结果记录中，给该位赋予一定的意义。
//第29位用于表示记录正在被读，注意：只有在搜索数据时，才只是在搜索到的结果记录中，给该位赋予一定的意义。
//第28位用于表示记录非正式，注意：只有在搜索数据时，才只是在搜索到的结果记录中，给该位赋予一定的意义。

#define RECORD_FILE_IS_LOCK(type)       (0x80000000 & (type))
#define RECORD_FILE_EN_LOCK(type)       ((type) |= (0x80000000))
#define RECORD_FILE_EN_UNLOCK(type)     ((type) &= (0x7fffffff))

#define RECORD_FILE_IS_ONLYREAD(type)   (0x40000000 & (type))
#define RECORD_FILE_EN_ONLYREAD(type)   ((type) |= (0x40000000))

#define RECORD_FILE_IS_WRITING(type)    (0x20000000 & (type))
#define RECORD_FILE_EN_WRITING(type)    ((type) |= (0x20000000))

#define RECORD_FILE_IS_READING(type)    (0x10000000 & (type))
#define RECORD_FILE_EN_READING(type)    ((type) |= (0x10000000))

#define RECORD_FILE_IS_UNOFFICAL(type)    (0x08000000 & (type))
#define RECORD_FILE_EN_UNOFFICAL(type)    ((type) |= (0x08000000))

#define RECORD_TYPE_IS_NORMAL(type) (0x0f & type)

//获取录像类型的总数
inline unsigned char RecTypeNum()
{
	for (unsigned char i=0; i<16; ++i)
	{
		if ((0x01 << i) & RECORD_TYPE_END)
		{
			return i;
		}
	}

	return 0;
}

//获取某中录像类型的排列序号
inline unsigned char RecTypeIndex(RECORD_TYPE type)
{
	unsigned char num = RecTypeNum();

	for (unsigned char i=0; i<num; ++i)
	{
		if ((0x01 << i) & type)
		{
			return i;
		}
	}

	return num;
}

typedef struct _encode_info
{
	unsigned char	resolution;		//图像分辨率
	unsigned char	rate;			//编码帧率
	unsigned char	encodeType;		//编码类型：定码流和变码流
	unsigned char	quality;		//编码质量
	unsigned long	lBitStream;		//编码码率下限
	unsigned long	hBitStream;		//编码码率上限
}ENCODE_INFO;

//	以下三个扩展结构体定义应用在XDVR3.0版本上的。
typedef struct _file_info_ex
{
	unsigned short	deviceID;		//网络设备ID
	unsigned short	cameraID;		//网络设备通道ID

	unsigned short	fileIndex;		//文件序号
	unsigned short	diskIndex;		//磁盘序号

	unsigned long	type;			//类型组合
	unsigned long	chnn;			//通道号
	unsigned long	startTime;		//开始时间
	unsigned long	endTime;		//结束时间
}FILE_INFO_EX;

typedef struct _reclog_info_ex 
{
	unsigned short	deviceID;		//网络设备ID
	unsigned short	cameraID;		//网络设备通道ID

	unsigned short	fileIndex;		//文件序号
	unsigned short	diskIndex;		//磁盘序号

	unsigned long	type;			//类型组合
	unsigned long	chnn;			//通道号
	unsigned long	startTime;		//开始时间
	unsigned long	endTime;		//结束时间
}RECLOG_INFO_EX;

typedef struct _section_info_ex
{
	unsigned long	startTime;		//开始时间
	unsigned long	endTime;		//结束时间
}SECTION_INFO_EX;

typedef CMyList <RECLOG_INFO_EX>		RECLOG_INFO_EX_LIST;
typedef CMyList <FILE_INFO_EX>			FILE_INFO_EX_LIST;
typedef CMyList <SECTION_INFO_EX>		SECTION_INFO_EX_LIST;
typedef CMyList <DVR_DATE>				DVR_DATA_LIST;

//

//事件日志
typedef enum _event_type
{
    EVENT_TYPE_MOTION   = 0x0001,		//移动侦测
    EVENT_TYPE_SENSOR   = 0x0002,		//传感器报警
    EVENT_TYPE_V_LOSS   = 0x0004,		//视频丢失
	EVENT_TYPE_V_COVER	= 0x0008,		//视频遮挡
}EVENT_TYPE;

typedef struct _event_log
{
	unsigned short chnn;			//事件对应的通道
	unsigned short type;			//事件类型：EVENT_TYPE

	unsigned long startTime;		//事件产生的开始时间
	unsigned long endTime;			//事件的结束时间
}EVENT_LOG, *P_EVENT_LOG;

typedef CMyList <EVENT_LOG> EVENT_LOT_LIST;

typedef struct _operator_log
{
	unsigned short	contentID;		//日志的组ID
	unsigned short	type;			//日志的类型：LOG_TYPE

	unsigned long	IP;				//产生该日志时用户IP
	unsigned long	time;			//该日志发生时间
	char			name [36];		//该日志对应地用户名称
}OPERATOR_LOG, *P_OPERATOR_LOG;

typedef CMyList <OPERATOR_LOG> OPERATOR_LOG_LIST;

//高位两字节对应contentID，地位两字节对应type。
typedef enum _log_type
{
	//系统控制(Control)
	LOG_TYPE_SYSTEM_CTRL		= 0x00010000,
	LOG_TYPE_BOOT,							//系统开机
	LOG_TYPE_SHUTDOWN,						//系统关机
	LOG_TYPE_REBOOT,						//系统重启
	LOG_TYPE_FORMAT_SUCC,					//格式化磁盘成功
	LOG_TYPE_FORMAT_FAIL,					//格式化磁盘失败
	LOG_TYPE_UPGRADE_SUCC,					//升级成功
	LOG_TYPE_UPGRADE_FAIL,					//升级失败
	LOG_TYPE_CLEAR_ALARM,					//清除报警
	LOG_TYPE_OPEN_ALARM,					//开启报警
	LOG_TYPE_MANUAL_START,					//开启手动录像
	LOG_TYPE_MANUAL_STOP,					//停止手动录像
	LOG_TYPE_PTZ_ENTER,						//开始云台控制
	LOG_TYPE_PTZ_CTRL,						//云台操作
	LOG_TYPE_PTZ_EXIT,						//结束云台控制
	LOG_TYPE_AUDIO_CH_CHANGE,				//改变现场音频通道
	LOG_TYPE_VOLUME_ADJUST,					//调节音量
	LOG_TYPE_MUTE_ENABLE,					//开启静音
	LOG_TYPE_MUTE_DISENABLE,				//关闭静音
	LOG_TYPE_DWELL_ENABLE,					//开启轮循
	LOG_TYPE_DWELL_DISENABLE,				//关闭轮循
	LOG_TYPE_LOG_IN,						//登录
	LOG_TYPE_LOG_OFF,						//登出
	LOG_TYPE_CHANGE_TIME,					//修改系统时间
	LOG_TYPE_MANUAL_SNAP_SUCC,               //手动抓图成功
	LOG_TYPE_MANUAL_SNAP_FAIL,               //手动抓图失败

	//系统配置(Setup)
	LOG_TYPE_CONFIG			= 0x00020000,
	LOG_TYPE_CHGE_VIDEO_FORMAT,				//改变视频制式
	LOG_TYPE_CHGE_VGA_RESOLUTION,			//改变显示器分辨率
	LOG_TYPE_CHGE_LANGUAGE,					//调整语言
	LOG_TYPE_CHGE_NET_USER_NUM,				//调整网络用户数目
	LOG_TYPE_CHGE_TIME_ZONE,				//调整时区
	LOG_TYPE_NTP_MANUAL,					//手动网络校时
	LOG_TYPE_NTP_ON,						//开启自动网络校时
	LOG_TYPE_NTP_OFF,						//关闭自动网络校时
	LOG_TYPE_CHGE_NTP_SERVER,				//修改网络时间服务器地址
	LOG_TYPE_CHGE_DST,						//调整夏令时设置
	LOG_TYPE_PASSWD_ON,						//开启操作密码
	LOG_TYPE_PASSWD_OFF,					//关闭操作密码

	LOG_TYPE_CHGE_CAM_NAME,					//调整通道名称
	LOG_TYPE_MODIFY_COLOR,					//调整图像色彩
	LOG_TYPE_CHGE_HOST_MONITOR,				//调整主监视器画面设置
	LOG_TYPE_CHGE_SPOT,						//调整辅助输出画面设置
	LOG_TYPE_CHGE_OSD,						//调整字符叠加设置

	LOG_TYPE_CHGE_LOCAL_ENCODE,				//调整录像流编码参数
	LOG_TYPE_CHGE_REC_VIDEO_SWITCH,			//调整录像开关设置
	LOG_TYPE_CHGE_REC_AUDIO_SWITCH,			//调整录制音频开关设置
	LOG_TYPE_CHGE_REC_REDU_SWITCH,			//调整冗余录像开关设置
	LOG_TYPE_CHGE_REC_PRE_TIME,				//调整景前录像时间
	LOG_TYPE_CHGE_REC_POST_TIME,			//调整景后录像时间
	LOG_TYPE_CHGE_REC_HOLD_TIME,			//调整录像数据过期时间

	LOG_TYPE_CHGE_SCH_SCHEDULE,				//调整定时录像计划
	LOG_TYPE_CHGE_SCH_MOTION,				//调整移动侦测录像计划
	LOG_TYPE_CHGE_SCH_ALARM,				//调整报警录像计划

	LOG_TYPE_CHGE_SENSOR_SWITCH,			//调整报警输入开关设置
	LOG_TYPE_CHGE_SENSOR_TYPE,				//调整报警输入设备类型
	LOG_TYPE_CHGE_SENSOR_TRIGGER,			//调整报警输入处理方式设置
	LOG_TYPE_CHGE_SENSOR_SCH,				//调整报警输入侦测计划

	LOG_TYPE_CHGE_MOTION_SWITCH,			//调整移动侦测开关设置
	LOG_TYPE_CHGE_MOTION_SENS,				//调整移动侦测灵敏度
	LOG_TYPE_CHGE_MOTION_AREA,				//调整移动侦测区域设置
	LOG_TYPE_CHGE_MOTION_TRIGGER,			//调整移动侦测处理方式
	LOG_TYPE_CHGE_MOTION_SCH,				//调整移动侦测计划

	LOG_TYPE_CHGE_VL_TRIGGER,				//调整视频丢失处理方式设置

	LOG_TYPE_CHGE_RELAY_SWITCH,				//调整报警输出开关设置
	LOG_TYPE_CHGE_RELAY_SCH,				//调整报警输出计划

	LOG_TYPE_BUZZER_ON,						//开启声音报警设备
	LOG_TYPE_BUZZER_OFF,					//关闭声音报警设备
	LOG_TYPE_CHGE_BUZZER_SCH,				//调整声音报警计划

	LOG_TYPE_CHGE_HTTP_PORT,				//修改HTTP服务器端口
	LOG_TYPE_CHGE_SER_PORT,					//修改网络服务器端口
	LOG_TYPE_CHGE_IP,						//设置网络地址
	LOG_TYPE_DHCP_SUCC,						//自动获取网络地址成功
	LOG_TYPE_DHCP_FAIL,						//自动获取网络地址失败
	LOG_TYPE_CHGE_PPPOE,					//设置PPPoE
	LOG_TYPE_CHGE_DDNS,						//设置DDNS
	LOG_TYPE_NET_STREAM_CFG,				//调整网络流编码设置
	
	LOG_TYPE_CHGE_SERIAL,					//调整云台串口设置
	LOG_TYPE_PRESET_MODIFY,					//调整预置点
	LOG_TYPE_CRUISE_MODIFY,					//调整巡航线
	LOG_TYPE_TRACK_MODIFY,					//调整轨迹

	LOG_TYPE_USER_ADD,						//增加用户
	LOG_TYPE_USER_MODIFY,					//调整用户权限
	LOG_TYPE_USER_DELETE,					//删除用户
	LOG_TYPE_CHANGE_PASSWD,					//修改用户密码

	LOG_TYPE_LOAD_DEFAULT,					//恢复默认配置
	LOG_TYPE_IMPORT_CONFIG,					//导入配置
	LOG_TYPE_EXPORT_CONFIG,					//导出配置

	LOG_TYPE_CHGE_IMAGE_MASK,				//图像遮挡
	LOG_TYPE_RECYCLE_REC_ON,				//开启循环录像
	LOG_TYPE_RECYCLE_REC_OFF,				//关闭循环录像
	LOG_TYPE_CHGE_DISK_ALARM,				//调整磁盘报警空间

	LOG_TYPE_CHGE_SEND_EMAIL,				//设置Email 发送人信息
	LOG_TYPE_CHGE_RECV_EMAIL,				//设置Email 接收人信息
	LOG_TYPE_CHGE_SNAP_SETTING,             //调整抓图配置

	//录像回放(Playback)
	LOG_TYPE_PLAYBACK		= 0x00040000,
	LOG_TYPE_PLAYBACK_PLAY,					//播放
	LOG_TYPE_PLAYBACK_PAUSE,				//暂停
	LOG_TYPE_PLAYBACK_RESUME,				//恢复播放
	LOG_TYPE_PLAYBACK_FF,					//快进
	LOG_TYPE_PLAYBACK_REW,					//快退
	LOG_TYPE_PLAYBACK_STOP,					//停止
	LOG_TYPE_PLAYBACK_NEXT_SECTION,			//下一段
	LOG_TYPE_PLAYBACK_PREV_SECTION,			//上一段

	//数据备份(Backup)
	LOG_TYPE_BACKUP			= 0x00080000,
	LOG_TYPE_BACKUP_START,					//开始备份
	LOG_TYPE_BACKUP_COMPLETE,				//备份完成
	LOG_TYPE_BACKUP_CANCEL,					//放弃备份
	LOG_TYPE_BACKUP_FAIL,					//备份失败

	//录像检索(Search)
	LOG_TYPE_SEARCH			= 0x00100000,
	LOG_TYPE_SEARCH_TIME,					//按时间检索
	LOG_TYPE_SEARCH_EVENT,					//按事件检索
	LOG_TYPE_SEARCH_FILE_MAN,				//文件管理
	LOG_TYPE_DELETE_FILE,					//删除文件
	LOG_TYPE_LOCK_FILE,						//锁定文件
	LOG_TYPE_UNLOCK_FILE,					//解锁文件
	LOG_TYPE_DELETE_PICTURE,                //删除图片
	LOG_TYPE_LOCK_PICTURE,                  //锁定图片
	LOG_TYPE_UNLOCK_PICTURE,                //解锁图片


	//查看信息(View information)
	LOG_TYPE_VIEW_INFO		= 0x00200000,
	LOG_TYPE_VIEW_SYSTEM,					//查看系统信息
	LOG_TYPE_VIEW_EVENT,					//查看事件
	LOG_TYPE_VIEW_LOG,						//查看日志
	LOG_TYPE_VIEW_NETWORK,					//查看网络状态
	LOG_TYPE_VIEW_ONLINE_USER,				//查看在线用户
	LOG_TYPE_VIEW_EXPORT_LOG,				//导出日志
	LOG_TYPE_VIEW_EXPORT_EVENT,				//导出事件

	//异常信息(Error)
	LOG_TYPE_ERROR_INFO		= 0x00400000,
	LOG_TYPE_IP_CONFLICT,					//网络地址冲突
	LOG_TYPE_NETWORK_ERR,					//网络异常
	LOG_TYPE_DDNS_ERR,						//DDNS错误
	LOG_TYPE_DISK_IO_ERR,					//磁盘读写错误
	LOG_TYPE_UNKNOWN_OFF,					//异常断电
	LOG_TYPE_UNKNOWN_ERR,					//未知错误	
	LOG_TYPE_ERR_9A9A9A9A,
	LOG_TYPE_ERR_9A000001,
	LOG_TYPE_ERR_9A000002,
	LOG_TYE_DISK_WARNING,                  //磁盘衰减报警
}LOG_TYPE;

inline unsigned short GetContentID(LOG_TYPE type)
{
	return (type & 0xffff0000) >> 16;
}

//显示位置如通道名称、现场时间、时间戳在图像中的位置
typedef enum _disp_pos
{
	DISP_TOP_RIGHT		= 1,	//顶右
	DISP_TOP_LEFT		= 2,	//顶左
	DISP_BOTTOM_RIGHT	= 3,	//低右
	DISP_BOTTOM_LEFT	= 4,	//底左
	DISP_TOP_MIDDLE		= 5,	//顶中
	DISP_BOTTOM_MIDDLE	= 6,	//底中
	DISP_CENTER_LEFT	= 7,	//中左
	DISP_CENTER_MIDDLE	= 8,	//中中
	DISP_CENTER_RIGHT	= 9,	//中右
	DISP_CUSTOM_DEFINE	=32,	//自定义位置
}DISP_POS;//

//日排程结构，24代表24小时，ULONGLONG的每一位代表每一分钟的状态
typedef struct _date_schedule
{
	ULONGLONG hour [24];
}DATE_SCHEDULE;

//周排程结构，7表示每周7天的排程
typedef struct _week_schedule
{
	DATE_SCHEDULE week[7];
}WEEK_SCHEDULE;

//假日排程，目前有两种实现方式，一种是每年重复的假日排程方式，一种是指定日期的假日排程方式,year在前一种方式无用
typedef struct _holiday_schedule
{
	unsigned char		month;
	unsigned char		day;
	unsigned short		year;	
	DATE_SCHEDULE		date;  //在一天里的计划表，24小时，每个小时每一分钟用一个bit表示
}HOLIDAY_SCHEDULE;


//三级用户权限，每级用户具有默认权限，但是可以向下调节具体权限（不能向上调节）。
typedef enum _user_group_
{
	USER_GROUP_NONE		= 0x00,	//
	USER_GROUP_ADMIN	= 0x01,	//管理员，拥有所有的权限
	USER_GROUP_ADVANCE	= 0x02,	//高级用户，默认具有：基本、录像、配置、回放、备份、数据管理、磁盘管理，云台控制，远程登录，及全通道权限
	USER_GROUP_NORMAL	= 0x04	//一般用户，默认具有：基本、录像、回放、备份、云台控制、远程登录，及全通道权限
}USER_GROUP;

typedef enum _user_authority
{
	AUTH_BASIC			= 0x0001,		//基本权限

	//通道权限
	AUTH_LIVE			= 0x0002,
	AUTH_RECORD			= 0x0004,		//录像权限，即开启和停止手动录像的权利
	AUTH_PLAYBACK		= 0x0008,		//回放权限，包括数据检索及回放控制
	AUTH_BACKUP			= 0x0010,		//备份权限，每个通道单独控制。
	AUTH_PTZ_CTRL		= 0x0020,		//云台控制权限，和云台配置权限有区别
	AUTH_REMOTE_VIEW	= 0x0040,		//远程预览，及远程查看该通道的现场画面。

	//系统权限
	AUTH_LOG_VIEW		= 0x0080,		//查看系统日志
	AUTH_SYS_SETUP		= 0x0100,		//系统配置
	AUTH_SHUTDOWN		= 0x0200,		//关机或者重启
	AUTH_AUDIO_TALK		= 0x0400,		//语音对讲
	AUTH_DATA_MAN		= 0x0800,		//数据管理：即可以锁定或者删除录像文件
	AUTH_DISK_MAN		= 0x1000,		//磁盘管理，可以设定磁盘属性，可以格式化磁盘。
	AUTH_REMOTE_LOGON	= 0x2000,		//远程登录
	AUTH_ADVANCE_MAN	= 0x4000,		//高级管理：包括用户管理、回复配置、导入或者导出配置等
	AUTH_NET_DEVICE_MAN	= 0x8000,		//

	AUTH_ALL			= 0xffffffff	//全部权限
}USER_AUTHORITY;


//unsigned long 中的后28位，用于记录语言。按位来记录语言.这样只能有28中语言
typedef enum _language_
{
	LANGUAGE_ENGLISH       = 0x0000001,  
	LANGUAGE_CHINESE_S     = 0x0000002,
	LANGUAGE_CHINESE_B     = 0x0000004,
	LANGUAGE_PORTUGUESE    = 0x0000008,
	LANGUAGE_GREECE        = 0x0000010,
	LANGUAGE_SPAISH        = 0x0000020,
	LANGUAGE_RUSSIAN       = 0x0000040,	
	LANGUAGE_NORWAY        = 0x0000080,
	LANGUAGE_TURKEY        = 0x0000100,
	LANGUAGE_ITALY         = 0x0000200,
	LANGUAGE_CZECH         = 0x0000400,
	LANGUAGE_GERMAN        = 0x0000800,
	LANGUAGE_HEBREW        = 0x0001000,
	LANGUAGE_JAPANESE      = 0x0002000,	
	LANGUAGE_FRENCH        = 0x0004000,
	LANGUAGE_POLISH        = 0x0008000,
	LANGUAGE_BULGARIAN     = 0x0010000,
	LANGUAGE_INDONESIA     = 0x0020000,
	LANGUAGE_RUSSIAN_D     = 0x0040000,
	LANGUAGE_THAI          = 0x0080000,

	LANGUAGE_ALL           = 0xFFFFFFF,
}LANGUAGE;
#define  MAX_LANGUAGE_PACK_NUM 28


typedef enum _getip_mode
{
	GETIP_MODE_STATICIP = 0,	//静态IP	
	GETIP_MODE_DHCP		= 1,	//动态IP
}GETIP_MODE;

typedef enum _client_status
{
	CLIENT_STATUS_NULL		= 0x0,
	CLIENT_STATUS_LIVE		= 0x0001,
	CLIENT_STATUS_PTZ		= 0x0002,
	CLIENT_STATUS_SEARCH	= 0x0004,
	CLIENT_STATUS_PB		= 0x0008,
	CLIENT_STATUS_BACKUP	= 0x0010,
	CLIENT_STATUS_INFO		= 0x0020,
	CLIENT_STATUS_SETUP		= 0x0040,
}CLIENT_STATUS;

typedef struct _ddns_info
{
	unsigned char	bEnable;	//是否启用DDNS功能
	unsigned char	bUseDomain;	//是否使用域名信息？
	unsigned short	type;		//使用哪种协议

	char	user[132];			//用户名称，
	char	password[132];		//用户密码
	char	domain[132];		//对应的域名信息。
}DDNS_INFO;

typedef struct _pppoe_info
{
	unsigned long	bEnable;	//是否启用PPPoE功能
	char	user[132];			//用户名称，
	char	password[132];		//用户密码
}PPPOE_INFO;

typedef struct _network_addr_info
{
	unsigned long bDHCP;	//
	unsigned long staticIP;	//
	unsigned long gateway;	//
	unsigned long netMask;	//
}NETWORK_ADDR_INFO;

typedef struct _hxht_net_param_info_
{
	char videoDeviceID[132];		//DVR在接入服务器中的设备ID,由接入服务器分配，DVR向接入服务器注册时使用
	unsigned short	usVDevDataPort;	//DVR的数据端口，包括音频、视频、历史视频、会话端口等
	unsigned short	notUsed;

	unsigned long	csIP;			//接入服务器的IP
	unsigned short	csMessagePort;	//接入服务器消息端口 默认使用6005
	unsigned short	csStortPort;	//接入服务器存储注册端口 默认使用6004
}HXHT_NET_PARAM_INFO;

typedef struct _motion_detect_info
{
	unsigned long sensitivity;  //移动侦测灵敏度
	MOTION_AREA_INFO areaInfo;	//移动侦测的区域信息。
}MOTION_DETECT_INFO;

typedef struct hxht_motion_rect
{
	unsigned short		xStart;
	unsigned short		yStart;
	unsigned short		width;
	unsigned short		height;
	unsigned char		bEnable;
	unsigned char		resv[3];
}HXHT_MOTION_RECT;

typedef struct _hxht_motion_area_info
{
	unsigned long sensitivity;  //移动侦测灵敏度
	HXHT_MOTION_RECT	areaRect[32];
}HXHT_MOTION_AREA_INFO;

//报警时触发摄像头移动
typedef struct _to_ptz
{
	unsigned char	ucToPreset;				//报警时，移动到哪一个预制点0 -- 127	 0xff 表示不需要移动
	unsigned char	ucBackPreset;			//报警过后返回哪个预制点0 -- 127		 0xff 表示不返回
	char            cIsEnablePreset;        //启用到达某个预置点
	char            cIsEnableBack;          //启用结束时返回某个点	
	unsigned char	ucToCruise;				//报警时,触发哪条巡航线0 -- 16		 0xff 表示不触发
	char            cIsEnableCruise;        //启用巡航线功能
	char			cIsEnableTrack;			//触发轨迹功能
	unsigned char	ucToTrack;				//报警时,触发哪条巡航线0 -- 3		 0xff 表示不触发
}TO_PTZ;
//////////////////////////////////////////////////////////////////////////
typedef struct _net_client_info
{
	unsigned short	port;			//客户端远程机的端口号（主机字节序）
	unsigned short	status;			//表示该用户当前的操作状态

	unsigned long	clientType;		//客户端的类型（控制中心、手机、IE）
	unsigned long	IP;				//连接的客户端的IP地址

	char			User[36];		//用户名 
}NET_CLIENT_INFO;

//2010-06-10 10:15:00 YSW
//为了客户端对新旧版本的兼容，特增加这个结构体，由客户端判定长度来决定使用
//NET_CLIENT_INFO或者NET_CLIENT_INFO_EX结构体。
typedef struct _net_client_info_ex
{
	unsigned short	port;			//客户端远程机的端口号（主机字节序）
	unsigned short	status;			//表示该用户当前的操作状态

	unsigned long	clientType;		//客户端的类型（控制中心、手机、IE）
	unsigned long	IP;				//连接的客户端的IP地址

	char			User[36];		//用户名 
	unsigned long	clientID;		//客户端ID，为了和早前客户端兼容，特放在最后。
}NET_CLIENT_INFO_EX;

typedef struct _stream_info
{
	unsigned char	bAudio;			//是否带有音频
	unsigned char	bCameraName;	//是否带有通道名称
	unsigned char	bTimeStamp;		//是否带有时间戳
	unsigned char	bVBR;			//
	
	unsigned char	frameRate;		//帧率
	unsigned char	quality;		//画质
	unsigned char	resolution;		//分辨率
	unsigned char	bKeepRate;		//保持流畅性

	unsigned short	timeStampXPos;
	unsigned short	timeStampYPos;
	unsigned short	cameraNameXPos;
	unsigned short	cameraNameYPos;

	unsigned short	preRecTime;
	unsigned short	postRecTime;
	unsigned short	dataHoldTime;
	unsigned short	unused;
}STREAM_INFO;

typedef struct _network_status_info
{
	unsigned char	bDHCP;			//是否是动态获取网络地址，0表示静态IP
	unsigned char	bPPPoE;			//是否开启了PPPoE
	unsigned char	bDDNS;			//是否开启了DDNS, 0x1表示开启了DDNS功能但不一定该服务能用， 0x11表示此时可以用域名访问DVR了
	unsigned char	bWiFi;			//是否开启了无线网络

	unsigned short	httpPort;		//HTTP服务器端口	
	unsigned short	serverPort;		//业务服务器端口

	unsigned long	IP;				//当前网络地址
	unsigned long	subMask;		//子网掩码
	unsigned long	gateway;		//网关
	unsigned long	DNS1;			//主DNS服务器
	unsigned long	DNS2;			//次DNS服务器

	unsigned long   netstatus;		//网络的状态。0表示没有连接到internet，1表示连接到了internet。
									//为1的时候表示网口处于up状态并且插入了网线，其他情况下为0

	char	MAC[8];			//设备物理地址
}NETWORK_STATUS_INFO;

typedef enum _ban_allow_type
{
	BAN_ALLOW_TYPE_BAN,
	BAN_ALLOW_TYPE_ALLOW
}BAN_ALLOW_TYPE;

typedef struct _ip_section
{
	unsigned long enable;		//零表示此段无效
	unsigned long start;		//开始地址
	unsigned long end;			//结束地址
}IP_SECTION;

//备份相关
typedef enum _call_back_module
{
	CALL_BACK_MODULE_DISK_MAN = 1,
	CALL_BACK_MODULE_BACKUP_MAN = 2,

}CALL_BACK_MODULE;


typedef enum _call_back_msg
{
	CALL_BACK_MSG_PROGRESS = 1,
	CALL_BACK_MSG_FINISH   = 2,
	CALL_BACK_MSG_FAIL     = 3,
}CALL_BACK_MSG;

//与备份相关
typedef enum _backup_data_type
{
	NET_BACKUP_DATA_TYPE_DVR,
	NET_BACKUP_DATA_TYPE_AVI,	
}BACKUP_DATA_TYPE;

///////////////////////////////硬盘相关/////////////////////////////////////////
typedef enum _disk_num_status
{
	NET_DISK_HAVE_READEWRITE_DISK = 0x01,
	NET_DISK_HAVE_REDUN_DISK      = 0x02,
	NET_DISK_HAVE_ONLYREADE_DISK  = 0x04,
	NET_DISK_HAVE_UNFORMATED_DISK = 0x08,
}DISK_NUM_STATUS;

typedef enum _disk_owner
{
	NET_DISK_OWNED_BY_THIS    = 0,
	NET_DISK_OWNED_BY_OTHER_1 = 1,
}DISK_OWNER;

//设备类型
typedef enum _disk_type
{
	NET_DISK_TYPE_FIXEDDISK = 0,     //固定录像硬盘，不支持热拔出
	NET_DISK_TYPE_REMOVEABLE = 1,    //热拔插设备
	NET_DISK_TYPE_CDDVD,			 //光驱，不支持热拔出
}DISK_TYPE;

//设备接口类型
typedef enum _disk_interface_type
{
	NET_DISK_IDE_INTERFACE = 0,
	NET_DISK_SATA_INTERFACE,
	NET_DISK_USB_INTERFACE,
	NET_DISK_REMDISK_INTERFACE,
	NET_DISK_ESATA_INETERFACE,
	NET_DISK_SD_INTERFACE,
	NET_DISK_CDDVD_INTERFACE,
}DISK_INTERFACE_TYPE;

//录像硬盘会用到:NET_DISK_NORMAL,   NET_DISK_NO_FORMAT
//分别表示：     正常（可以使用），      新硬盘
//光驱会用到：NET_DISK_NORMAL，NET_DISK_NO_CD
//分别表示：  正常，            没有放光盘
//热拔插设备会用到：NET_DISK_IS_MOUNT， NET_DISK_NO_MOUNT
//分别表示：        成功接入并且正在使用，  成功接入但是还没有使用
typedef enum _disk_status
{	
	NET_DISK_NORMAL,				//正常 
	NET_DISK_NO_FORMAT,				//未格式化
	NET_DISK_NO_CD,                 //没有放光盘
	NET_DISK_IS_MOUNT,              //对热拔插设备来说，表示已经挂载，即正在使用中
	NET_DISK_NO_MOUNT,              //对热拔插设备来说，没有挂载，表示成功接入但是还没有被使用
}DISK_STATUS;

typedef enum _disk_property
{
	NET_DISK_READ_WRITE = 0,  //可正常读写
	NET_DISK_ONLY_READ,   //只能读，不能写
	NET_DISK_REDUNDANCY   //冗余
}DISK_PROPERTY;

//该结构可以用于保存硬盘的整体信息。
typedef struct _dvr_disk_info
{
	unsigned char   diskOwnedBy;    //隶属于本DVR还是其他的DVR
	unsigned char	diskType;		//磁盘类型，USB，CD/DVD，录像硬盘 对应于DISK_TYPE
	unsigned char   diskInterface;  //磁盘接口类型，SATA,IDE,USB, CD/DVD 对于与DISK_INTERFACE_TYPE
	unsigned char	diskStatus;		//磁盘状态，no format，normal，，对于与DISK_STATUS。
	unsigned char   diskProperty;   //
    char            diskname[36];   //磁盘的名称如/dev/sda
	char            mountdir[36];   //磁盘挂载目录

	unsigned long	totalSpace;		//磁盘总容量，单位为 M
	unsigned long	freeSpace;		//剩余空间，单位为 M
	unsigned char   partitionNum;   //分区数
	unsigned long   diskpara;       //不同设备，代表的意义不同。目前如果是录像硬盘，且这个硬盘的Normal，那么这个参数保存的是第一个分区的第一个挂载目录
	unsigned char   disktemp;       //临时变量
	unsigned char	reserved;
}DVR_DISK_INFO;

typedef CMyList<DVR_DISK_INFO> DVR_DISK_INFO_LIST;

//保存设备的中的硬盘隶属于那些DVR
typedef struct _dvr_disk_owner
{
	BYTE  dvrIndex;        //
	BYTE  dvrId[8];        //
	BYTE  dvrPara;         //
	BYTE  diskNum;
}DVR_DISK_OWNER;
typedef CMyList<DVR_DISK_OWNER> DVR_DISK_OWNER_LIST;

typedef struct _net_smart_info_item_
{
	unsigned char  nIndex;            //ID
	unsigned char  nCurrent;          //当前值
	unsigned char  nWorest;           //最坏值
	unsigned char  nThreshold;        //阀值
	unsigned long  nRawValue;         //数据
	unsigned long  nStatus;           //0-ok,1-warning,2-critical
}SMART_INFO_ITEM;

typedef struct _disk_smart_info_
{
	unsigned char   szModelNumber[48];
	unsigned char   szSerialNumber[48];
	unsigned char   szFirmwareRev[48];
	SMART_INFO_ITEM smartInfo[20];
	unsigned long   diskHealth;
}NET_DISK_SMART_INFO;


typedef struct _disk_health
{
	char                szDiskName[48];
	unsigned char       bSleep;
	NET_DISK_SMART_INFO diskSmartInfo;  
}DISK_HEALTH_INFO;
typedef CMyList<DISK_HEALTH_INFO> DISK_HEALTH_INFO_LIST;

//用于日志模块和reclog模块的初始化
typedef struct _disk_log_info
{
	unsigned char diskOwnedBy;
	unsigned char diskProperty;
	unsigned char diskIndex;
}DISK_LOG_INFO;
///////////////////////////////硬盘相关/////////////////////////////////////////

///////////////////////////////云台相关/////////////////////////////////////////
//云台巡航点
typedef struct _ptz_curise_point
{
	unsigned char   preset;		//预置点序号
	unsigned char   dwellSpeed;	//运转速度
	unsigned short  dwellTime;	//停留时间（秒）
}PTZ_CURISE_POINT;

typedef struct _ptz_curise
{
	unsigned short	index;		//序号
	unsigned short	presetNum;	//预置点数目
	char			name[36];	//巡航线的名称
}PTZ_CURISE;

typedef struct _ptz_preset
{
	unsigned short		index;		//预置点序号
	unsigned short		enable;		//预置点是否已经被启用， 非零为已经被启用
	char				name[36];	//预置点名称
}PTZ_PRESET;

typedef struct _ptz_ctrl
{
	unsigned long	chnn;			//哪个通道
	unsigned long	speed;			//控制速度
	unsigned long	cmdType;		//命令类型
	unsigned long	cmdValue;		//命令参数：预置点序号，巡航线序号，轨迹序号等
}PTZ_CTRL;

typedef struct _net_ptz_info
{
	unsigned long   chnn;
	unsigned long   presetNum;
	unsigned long   curiseNum;
	unsigned long	curisePointNum;
}NET_PTZ_INFO;

//云台串口信息
typedef struct _ptz_serial_info
{
	unsigned char	baudRate;		//波特率0-13分别表示110，300，…..921600的波特率
	unsigned char	protocol;	//协议ID，0为没有配置云台
	unsigned char	devAddr;		//云台设备ID
	unsigned char	bEnable;		//表示是否启用云台功能
}PTZ_SERIAL_INFO;
///////////////////////////////云台相关/////////////////////////////////////////

//Sensro 类型
typedef enum _SENSOR_TYPE
{
	SENSOR_TYPE_NO,	
	SENSOR_TYPE_NC,				
}SENSOR_STATUS;

//邮件功能 发件人信息结构
typedef struct _mail_send_info
{
	unsigned long	bSSL;
	unsigned long	port;
	char			server[68];
	char			name[68];
	char			passwd[36];
}MAIL_SEND_INFO;

//邮件功能 接收人信息结构
typedef struct _mail_recv_info 
{
	char recvname[68];
}MAIL_RECV_INFO;
///////////////////////////////车载相关/////////////////////////////////////////
typedef enum _speed_source_type
{
	SPEED_SOURCE_GPS = 1,	//GPS
	SPEED_SOURCE_CAR		//机车
}SPEED_SOURCE_TYPE;

typedef enum _speed_unit_type
{
	SPEED_UNIT_KM_H = 1,	//千米/时
	SPEED_UNIT_MN_H			//海里/时
}SPEED_UNIT_TYPE;

typedef struct _mdvr_speed
{
	char speed [12];		//表示速度，因为可能有小数，而控件上不能输入小数，所以采用字符串的办法。
}MDVR_SPEED;

//冲击力配置的共有状态
typedef struct _mdvr_wallop_state
{
	unsigned long	enable;			//是否启用默认不启用0:不启用1:启用
	float			threshold;		//其门限值
	double 			reference;		//其参考值
}MDVR_WALLOP_STATE;

typedef enum _wifi_ip_mode
{
	WIFI_IP_STATIC,
	WIFI_IP_DECP
}WIFI_IP_MODE;

typedef enum _wifi_auth_mode
{
	WIFI_AUTH_OPEN = 1,
	WIFI_AUTH_SHARE,
	WIFI_AUTH_WPA2_PSK,
	WIFI_AUTH_WPA_PSK
}WIFI_AUTH_MODE;

typedef enum _wifi_encryp_type
{
	WIFI_ENCRYP_TKIP = 1,
	WIFI_ENCRYP_AES,
	WIFI_ENCRYP_NONE,
	WIFI_ENCRYP_WEP,
}WIFI_ENCRYP_TYPE;

//wifi 配置  //这是一个数组

typedef struct _wifi_config
{
	unsigned long	enable;				//无线网络开关
	unsigned long	IPMode;				//0	是静态IP  1 表示动态IP

	unsigned long	staticIP;			//无线网络ip
	unsigned long	subMask;			//子网掩码
	unsigned long	gateway;			//
	unsigned long	DNSServer;			//

	unsigned long	channel;			//信道	0--11 0表示自动

	unsigned long	authMode;			//认证模式1:OPEN 2:SHARE 3:WPA2-PSK 4:WPA-PSK
	unsigned long	encrypType;			//加密类型1:TKIP 2:AES 3:NONE 4:WEP 
	unsigned long	defaultKeyID;		//密码编号

	unsigned long	rate;				//频段
	unsigned long	mode;				//模式

	char			ssid[32];			//SSID

	char			key1[36];			//密码1
	//char			key2[36];			//密码2

}WIFI_CONFIG;

//网络服务器配置
typedef struct mdvr_netserver_cfg
{	
	unsigned long   serverIP;		//服务器IP
	unsigned long	serverPort;		//服务器端口
}MDVR_NETSERVER_CFG;

//无线网络中拨号参数设置
typedef struct mdvr_dialup_cfg
{
	char	points[132];			//拔号接入点
	char	name[132];				//用户名
	char	password[36];			//密码
	char	serviceNumber[36];		//数据业务号
}MDVR_DIALUP_CFG;

typedef enum _wireless_type
{
	WIRELESS_TYPE_EVDO = 1,
	WIRELESS_TYPE_GPRS
}WIRELESS_TYPE;

typedef enum _wireless_conn_mode
{
	WIRELESS_CONN_LONG = 1,
	WIRELESS_CONN_ACTIVE
}WIRELESS_CONN_MODE;

//无线网络配置
typedef struct mdvr_wireless_cfg
{
	unsigned short	enable;			//是否开启无线网络功能 1:开启，0：关闭
	unsigned short 	type;			//无线模块类型1:EVDO 2:GPRS
	unsigned short 	mode;			//连接模式1:长连模式2:激活模式
	unsigned short  reserved;		//预留

	char			number1[16];	//号码1
	char			number2[16];	//号码2
	char			number3[16];	//号码3

	MDVR_DIALUP_CFG	dialup;			//拨号设置
}MDVR_WIRELESS_CFG;

typedef struct _net_wifi_modem
{
	unsigned long	dwChannel;		//信道	
	char			ssid[32];		//ssid
	unsigned long	dwAuthMode;
	unsigned long   dwEncrypType;
	unsigned long	dwSignal;		//0:无信号 1:很弱 2:弱 3:一般 4:好 5:很好
	unsigned long	dwSignalUseState; //信号有没有使用过 0:没有使用 1:正在使用 2:曾经使用
}NET_WIFI_MODEM;

///////////////////////////////车载相关/////////////////////////////////////////

//下面枚举了功能描述，都是些可能支持可能不支持的功能。
//注意，如果增加新的项，只能紧跟着向后添加，不够数值，保持值的递增。
typedef enum _func_type
{
	FUNC_REMOTE_UPGRADE,			//远程升级
	FUNC_MANUAL_ALARM,				//手动报警
	FUNC_VIEW_VERSION,				//查看系统版本信息
	FUNC_VIEW_EVENT,				//查看事件
	FUNC_VIEW_LOG,					//查看日志
	FUNC_VIEW_NETWORK_STATUS,		//查看网络状态
	FUNC_VIEW_ONLINE_USER,			//查看在线用户
	FUNC_VIEW_DISK_INFO,			//查看磁盘信息
	FUNC_DISK_MAN,					//磁盘管理
	FUNC_DISK_GROUP,				//磁盘分组功能
	FUNC_EXPORT_LOG,				//导出日志
	FUNC_IMPORT_SETUP,				//导入配置
	FUNC_EXPROT_SETUP,				//导出配置
	FUNC_PTZ_CTRL,					//云台控制
	FUNC_TIME_SEARCH,				//按时间搜索
	FUNC_EVENT_SEARCH,				//按事件搜索
	FUNC_FILE_MAN,					//文件管理
	FUNC_EVICT_SOMEONE,				//驱逐人
	FUNC_TALK_WAY,					//语音对讲
	FUNC_MANUAL_REC,				//手动录像（远程控制）
	FUNC_REMOTE_REBOOT,				//远程重启
	FUNC_REMOTE_SHUTDOWN,			//远程关机
	FUNC_SETUP,						//远程配置
	FUNC_BACKUP,					//远程备份
	FUNC_PLAYBACK,					//远程回放
}_FUNC_TYPE;

//注意一下宏的channe必须为32位的
//产品ID，针对连接的设备取的ID。
#define	PRODUCT_ID(channel) (((channel) & 0xffff0000) >> 16)

//网络设备的camera通道，及设备上的实际通道
#define	CAMERA_ID(channel) (((channel) & 0x0000ff00) >> 8)

//逻辑通道，比如本地camera占用0-7通道，而网络设备占用8-11通道
#define	CHANNEL_ID(channel) ((channel) & 0x000000ff)

//根据通道数目，返回全通道模板
#define ALL_CHNN_MASK(chnnNum) (((ULONGLONG)0x01 << (chnnNum)) - 1)

//无效索引
#define INVALID_INDEX(type) (static_cast<type>(~(type)0x0))

//返回日志类型组
#define LOG_CONTENT_MASK(type) (type & 0xffff0000)

//把通道掩码循环右移
#define CH_ROL(X, MASK, mask, n) \
	X <<= n; \
	if (0 == (X & MASK)) \
	{ \
		X = mask; \
	}
//把通道掩码循环左移
#define CH_LOL(X, MASK, mask, n) \
	X >>= n; \
	if (0 == (X & MASK)) \
	{ \
		X = mask; \
	}

#define IP_ADDRESS(ip1, ip2, ip3, ip4) ((ip1) + (ip2<<8) + (ip3<<16) + (ip4<<24))

const unsigned long ONE_DAY_SECOND_NUM	= 86400;
const unsigned long INVALID_CLIENT_ID	= (~0x0);
const unsigned long LOCAL_DEVICE_ID		= 0;
const unsigned long NET_DEVICE_ID		= 0xffff;	//当操作数字通道时，网络设备统一用这个ID
const unsigned long LOCAL_CLIENT_ID		= 0;
const unsigned long HOLDTIME_ALWAYS		= (~0x0);
const ULONGLONG		MAX_TIME			= ((LONGLONG)0x7fffffff<<32) + (0xffffffff);

//[层(unsigned char)] [信号制式(unsigned char)] [区域编号(unsigned char)] [通道(unsigned char)]
#define DISPLAY_PACK(LAYER_TYPE, SIGNAL_FORMAT, DISP_INDEX, CHNN_VALUE) (static_cast<unsigned long>(((LAYER_TYPE & 0xFF) << 24) | ((SIGNAL_FORMAT & 0xFF) << 16) | ((DISP_INDEX & 0xFF) << 8) |(CHNN_VALUE & 0xFF)))

//[unsigned long]
#define DISPLAY_GET_LAYER(DISP_VALUE) (static_cast<unsigned short>((DISP_VALUE >> 24) & 0xFF))
#define DISPLAY_GET_SIGNAL(DISP_VALUE) (static_cast<unsigned short>((DISP_VALUE >> 16) & 0xFF))
#define DISPLAY_GET_INDEX(DISP_VALUE) (static_cast<unsigned char>((DISP_VALUE >> 8) & 0xFF))
#define DISPLAY_GET_CHNN(DISP_VALUE) (static_cast<unsigned char>((DISP_VALUE & 0xFF)))



#define SNAP_PIC_IS_ONLYREAD(type)   (0x8000 & (type))
#define SNAP_PIC_EN_ONLYREAD(type)   ((type) |= (0x8000))

#ifndef NEED_VIDEO_ENCODE_INFO
#define NEED_VIDEO_ENCODE_INFO
typedef struct _chn_video_encode_info_
{
	unsigned int	channel;					//通道号
	unsigned long	iLength;					//视频参数数据长度
	unsigned char	*pVideoEncodeInfo;			//当前的编码信息地址
	unsigned char	bValid;						//是否可以修改
	unsigned char	reserver[7];				//保留
}CHN_VIDEO_ENCODE_INFO, *PCHN_VIDEO_ENCODE_INFO;


typedef struct _chn_video_encode_info_detail_
{
	unsigned int	stream;					//0:主码流 1:子码流
	unsigned char	resolution[12];			//获取时表示当前分辨率字符串，设置时表示需要设置的分辨率字符串，举例：352*288。要有结束符
	unsigned long	lBitRateStream[20];		//获取时表示当前编码码率数组，设置时，只有lBitStream[0]有效，适用于CBR
	unsigned int	rate;					//获取时表示为当前分辨率的帧率最大值，设置时表示需要设置的帧率
	unsigned int	encodeType;				//获取时表示当前分辨率的编码类型1：定码流；2变码流 3：定码流/变码流都支持
	//设置时表示当前分辨率的编码类型1：定码流；2变码流
	unsigned int	quality;				//获取时表示当前编码质量的分类数量，设置时为范围内的一个固定值，适用于VBR
	unsigned char	bCrrentUse;				//获取时表示当前是否为当前使用，设置时这里无意义
	unsigned char	reserver[3];
}CHN_VIDEO_ENCODE_INFO_DETAIL, *PCHN_VIDEO_ENCODE_INFO_DETAIL;

#endif

#if defined(__CHIP3520A__) || defined(__CHIP3535__)
#if defined(__ENVIRONMENT_LINUX__) && !defined(__X_LINUX__)
#include "hi_mem.h"
#endif
#endif

#ifdef	__ENVIRONMENT_WIN32__
#pragma pack()
#endif

#endif //_DVRDVSDEFINE_
