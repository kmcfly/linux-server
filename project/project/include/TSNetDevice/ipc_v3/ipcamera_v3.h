#ifndef _DVRDVSDEFINE_
#define _DVRDVSDEFINE_
#include "Typedef.h"

#pragma pack(4)


const char * const		DEFAULT_MAIL_SUBJECT = "AUTOMAIL IP NOTICE";	//默认AUTOMAIL的邮件主题
const char * const		DEFAULT_MAIL_TEST_SUBJECT = "AUTOMAIL TEST MAIL";	
const char * const 		DEFAULT_MAIL_TEST_CONTENT = "Automail test";
const char * const		VLOSS_ALARM_MAIL_SUBJECT = "VIDEO LOSS ALARM NOTICE";
const char * const		MOTION_ALARM_MAIL_SUBJECT = "MOTION ALARM NOTICE";
const char * const		SENSOR_ALARM_MAIL_SUBJECT = "SENSOR ALARM NOTICE";

//各种保持时间的，"总是"的定义，以前有用0和0xffffffff两种值，现在统一
const unsigned long HOLDTIME_ALWAYS=0xffffffff;
//设备用户名，密码最大长度，目前DVR DVS 客户端 CMS界面 第三方界面输入都采用此值，不要更改此值,要更改就需统一处理
const unsigned long  DEV_USER_NAME_LEN      = 16;
const unsigned long  DEV_PASSWORD_LEN       = 16;

enum IPCAMERA_ENCODE_LEVEL
{
	IPCAMERA_BASELINE,
	IPCAMERA_MAINPROFILE,
	IPCAMERA_HIGHPROFILE
};
//视频制式定义
typedef enum _video_format
{
	VIDEO_FORMAT_NTSC	= 0x00,
	VIDEO_FORMAT_PAL	= 0x01
}VIDEO_FORMAT;

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
	DDNS_MYQSEE,			 //专用于US02的DDNS,采用MINTDNS,服务器www.myq-see.com
	DDNS_EASTERNDNS,		//专用于US09的DDNS,采用MINTDNS,服务器www.easterndns.com
	DDNS_3322				//希网服务器www.3322.org
}DDNS_SERVER_TYPE;

typedef enum _dvr_video_size
{
	DVRVIDEO_SIZE_NONE	= 0x0000,	//NONE
	DVRVIDEO_SIZE_QCIF	= 0x0001,	//QCIF
	DVRVIDEO_SIZE_CIF	= 0x0002,	//CIF
	DVRVIDEO_SIZE_HD1	= 0x0003,	//HD1
	DVRVIDEO_SIZE_D1	= 0x0004,	//D1

	DVRVIDEO_SIZE_QQVGA	= 0x0005,	//QQVGA 
	DVRVIDEO_SIZE_QVGA	= 0x0006,	//QVGA
	DVRVIDEO_SIZE_VGA	= 0x0007,	//VGA
	DVRVIDEO_SIZE_XVGA	= 0x0008,	//XVGA 1280*960

	DVRVIDEO_SIZE_720P	= 0x0009,  	//1280*720
	DVRVIDEO_SIZE_1080P = 0x000A,	//1920*1080
	
	DVRVIDEO_SIZE_UXVGA = 0x000B,	//UXVGA 1600*1200

	DVRVIDEO_SIZE_NVR_INNER = 0x000C,//  NVR inner use 640*480
	DVRVIDEO_SIZE_14M = 0x000D,      //   1280*1024 1.4M
	DVRVIDEO_SIZE_QXGA  = 0x000E,    // 2048*1536
	DVRVIDEO_SIZE_8M  = 0x000F,    // 3264*2448

	DVRVIDEO_SIZE_16_9_3M = 0x0010,  //  2304*1296
	DVRVIDEO_SIZE_5M = 0x0011, //  2592*1944
	DVRVIDEO_SIZE_4M = 0x0012, //  2688*1520
	DVRVIDEO_SIZE_2K = 0x0013, //  2560*1440

}DVR_VIDEO_SIZE;

const ULONGLONG MAX_TIME		= ((LONGLONG)0x7fffffff<<32) + (0xffffffff);

typedef struct _mac
{
	unsigned char mac [8];
}MAC;


//ucFrameType
const char	IPCAMERA_FRAME_TYPE_VIDEO	= 0;
const char	IPCAMERA_FRAME_TYPE_AUDIO	= 1;
const char	IPCAMERA_FRAME_TYPE_PIC	= 2;

//ucEncodeType
const char	IPCAMERA_VIDEO_ENCODE_TYPE_H264 = 0;
const char	IPCAMERA_VIDEO_ENCODE_TYPE_MGP4 = 1;
const char	IPCAMERA_AUDIO_ENCODE_TYPE_RAW = 32;
const char	IPCAMERA_AUDIO_ENCODE_TYPE_G726 = 33;
const char	IPCAMERA_AUDIO_ENCODE_TYPE_G711 = 34;
const char	IPCAMERA_AUDIO_ENCODE_TYPE_ADPCM = 35;
const char	IPCAMERA_PIC_ENCODE_TYPE_JPG = 64;
const char	IPCAMERA_PIC_ENCODE_TYPE_BMP = 65;

struct IPCAMERA_FRAME_INFO
{
	//声音  视频  图片    IPCAMERA_FRAME_TYPE_LIVE_VIDEO
	unsigned char		ucFrameType;
	//编码方式，声音，视频，图片的都不一样
	unsigned char		ucEncodeType;
	//流序号，从0开始
	unsigned char		ucStreamIndex;
	//0 不是关键帧  1 是关键帧
	unsigned char		ucKeyFrame;	
	
	//帧序号，一直递增，加到溢出再从0开始递增
	unsigned long		frameIndex;

	unsigned long       width;
	unsigned long       height;
	
	//IPCamera的本地时间（即IPCamera本地某时区的时分秒），客户端用处理UTC时间的函数处理，得到的时分秒直接显示，
	LONGLONG			time;	  

	//相对时间，在改变设备时间时不会变化，是一直递增的时间
	LONGLONG			relativeTime;

	unsigned long		length;
	unsigned char      *pData;
};	//用于本地使用

//记录年月日的结构体
typedef struct _dvr_date
{
	unsigned short year;
	unsigned char  month;
	unsigned char  mday;
}DVR_DATE;
//显示年月日的模式
typedef enum _date_mode
{
	DATE_MODE_YMD	= 0x01,		//年月日模式
	DATE_MODE_MDY	= 0x02,		//月日年模式
	DATE_MODE_DMY	= 0x03		//日月年模式
}DATE_MODE;

//显示位置如通道名称、现场时间、时间戳在图像中的位置
typedef enum _disp_pos
{
	DISP_TOP_RIGHT		= 1,	//顶右
	DISP_TOP_LEFT		= 2,	//顶左
	DISP_BOTTOM_RIGHT	= 3,	//低右
	DISP_BOTTOM_LEFT	= 4,	//底左
	DISP_CUSTOM_DEFINE	= 32,	//自定义
}DISP_POS;

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
typedef struct _n_holiday_schedule
{
	unsigned char		month;
	unsigned char		day;
	unsigned short		year;	
	DATE_SCHEDULE		date;  //在一天里的计划表，24小时，每个小时每一分钟用一个bit表示
}N_HOLIDAY_SCHEDULE;

const unsigned long MAX_MAIL_RECV_CNT         = 8;

typedef struct _email_recv_item
{	
	unsigned long  dwIndex;
	char  szRecvAddr[64];
}EMAIL_RECV_ITEM;

typedef struct _email_info
{
	unsigned long	authMethod;
	unsigned short	nPort;
	unsigned char	chSSL;	//SSL_MODE_NONE 不需要SSL  SSL_MODE_SSL SSL  SSL_MODE_TLS TLS
	unsigned char	noused[1];
	char server		[132];
	char address	[132];
	char userName	[132];
	char password	[36];
	EMAIL_RECV_ITEM stRecvList[MAX_MAIL_RECV_CNT];
}EMAIL_INFO;

const unsigned char	SSL_MODE_NONE = 0;	
const unsigned char	SSL_MODE_SSL = 1;
const unsigned char	SSL_MODE_TLS = 2;

enum VIDEO_ENCODE_MODE
{
	VIDEO_ENCODE_MODE_VBR	=0x01,//变码流
	VIDEO_ENCODE_MODE_CBR =0x02//固定码流
};
//三级用户权限
typedef enum _user_group_
{
	USER_GROUP_NONE		= 0x00,
	USER_GROUP_ADMIN	= 0x01,
	USER_GROUP_ADVANCE	= 0x02,
	USER_GROUP_NORMAL	= 0x04
}USER_GROUP;

typedef enum _getip_mode
{
	GETIP_MODE_STATICIP = 0,	//静态IP	
	GETIP_MODE_DHCP		= 1,	//动态IP
}
GETIP_MODE;

typedef enum _image_quality
{
	IMAGE_QUALITY_LOWEST	= 0x01,
	IMAGE_QUALITY_LOWER		= 0x02,
	IMAGE_QUALITY_MEDIUM	= 0x03,
	IMAGE_QUALITY_HEIGHTER	= 0x04,
	IMAGE_QUALITY_HEIGHTEST	= 0x05
}IMAGE_QUALITY;

typedef struct _net_wifi_modem
{
	unsigned long	dwChannel;
	char			ssid[36];//最大长度32
	unsigned long	dwAuthMode;
	unsigned long   dwEncrypType;
	unsigned long	dwSignal;		// 0 无信号 1 很弱 2 弱  3 一般 4 好  5 很好
}NET_WIFI_MODEM;

const unsigned long		SMTP_SUBJECT_MAX_LEN		= 64;	//邮件主题的最大长度
const unsigned long		SMTP_CONTENT_MAX_LEN		= 256;	//邮件内容的最大长度

const unsigned long     FTP_SERVER_NAME_MAX_LEN      = 32;   //FTP服务器名字的最大长度
const unsigned long     FTP_SERVER_HOST_NAME_MAX_LEN = 128;   //FTP服务器地址的最大长度
const unsigned long     FTP_USERNAME_MAX_LEN    = 64;   //FTP用户名的最大长度
const unsigned long     FTP_PASSWD_MAX_LEN      = 64;   //FTP用户密码的最大长度
const unsigned long		FTP_PATH_LEN			= 128;	//FTP存放文件的路径

typedef struct _ftp_server
{
	unsigned long	dwServerID;									//用来唯一标示是哪一个FTP服务器 0 表示没有配置
	char			szServerName[FTP_SERVER_NAME_MAX_LEN+4];	//FTP服务器名字，用户随便填，自己好记就行
	char			szHostName[FTP_SERVER_HOST_NAME_MAX_LEN+4];	//可以是网址，也可以是IP地址
	unsigned short  nPort;										//FTP服务端口号,一般为21
	char			szUnused[2];
	char			szFtpUserName[FTP_USERNAME_MAX_LEN+4];		//FTP用户名
	char			szFtpPasswd[FTP_PASSWD_MAX_LEN+4];			//FTP用户密码
	char			szPath[FTP_PATH_LEN+4];						//存放文件的目录
}FTP_SERVER;

const BYTE PRESET_NAME_MAX_LEN =32;

//预置点信息，只含有一个通道的
typedef struct _ptz_preset_info
{
	BYTE preseIndex;//预置点位置序号
	BYTE noused[3];//对齐用
	char  name[PRESET_NAME_MAX_LEN+4]; //预置点名字
}
PRESET_INFO;

const BYTE CRUISE_NAME_MAX_LEN =32;
//巡航线信息头，只含有一个通道的
typedef struct _ptz_cruise_info_head
{
	unsigned long cruiseFlag;//开始停止标记 
	unsigned long cruisepointnum;//巡航点数量
	unsigned long cruiseIndex;//巡航线序号
	char  name[CRUISE_NAME_MAX_LEN+4];
}
CRUISE_INFO_HEAD;

typedef	struct _ptz_cruise_point
{
	unsigned char	cruisePoint;	//预置点
	unsigned char	cruiseSpeed;	//巡航时到每个预支点的速度(1-8)
	unsigned short	cruiseStopTime;	//巡航路径的每个的预置点的停留时间(1-999秒)
}PTZ_CRUISE_POINT;

typedef enum _camera_freq_define
{
	ENUM_CAMERA_FREQ_50HZ=0,
	ENUM_CAMERA_FREQ_60HZ=1,
	NCFG_ENUM_CAMERA_FREQ_OUTDOOR=2
}NCFG_ENUM_CAMERA_FREQ;//

#pragma pack()

#endif
