/******************************************************************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : 袁石维
** Date         : 2008-07-28
** Name         : ConfigDefine.h
** Version      : 1.0
** Description  :
** Modify Record:
**************************************************************************************************************/
#ifndef _NET_CONFIG_DEFINE_H_
#define _NET_CONFIG_DEFINE_H_
#include "Typedef.h"
//一般情况下不可改变此值，具体是否要更改此值需要讨论后确定
const unsigned long		NCFG_VERSION					=	0x0003;

#if defined(__ENVIRONMENT_WIN32__) || defined(__TDFH__) || defined(__TDNXP__) || defined(__CHIP3531__) || defined(__CHIP3535__) || defined(__CHIP3521__) || defined(__CHIP3520A__) || defined(__CHIPTI__) || defined(__CHIP3520D__) || defined(__CHIPGM__) 
#pragma pack(4)
#endif

typedef struct _ncfg_item_head
{
	unsigned short	itemID;		//ID
	unsigned short	num;		//元素数目，比如多少个通道，每个通道对应一个元素
	unsigned short	len;		//数据总长度
	unsigned short	subLen;		//每个元素的长度
}NCFG_ITEM_HEAD;


typedef struct _ncfg_block_head
{
	unsigned long	biSize;		//本结构体长度
	unsigned long	ItemNum;	//元素数目
	unsigned long	netcfgver;	//当前定义的版本号，设备端固定传输NCFG_VERSION，客户端根据需要动态判断
}NCFG_BLOCK_HEAD;

typedef struct _ncfg_item_head_ex
{
	NCFG_ITEM_HEAD	item_head;
	unsigned long	start_pos;		//文件中偏移
}NCFG_ITEM_HEAD_EX;		

typedef struct _ncfg_block_info
{
	NCFG_ITEM_HEAD_EX itemHead;
	unsigned long	 off;				//缓存中偏移
}NCFG_BLOCK_INFO;

typedef enum cfg_item_id
{
	//此范围ID存储系统基本配置的杂项
	NCFG_ITEM_SYSTEM_BASE=0x0100,
	NCFG_ITEM_MANUAL_REC,				//手动录像，ULONGLONG
	NCFG_ITEM_DEVICE_ID,				//设备ID，unsigned long
	NCFG_ITEM_DEVICE_NAME,				//设备名称，NCFG_INFO_SHORT_NAME
	NCFG_ITEM_VIDEO_FORMAT,				//视频格式，unsigned long ,视频制式
	NCFG_ITEM_AUTHORIZATION,			//是否权限检查, unsigned long
	NCFG_ITEM_LIVE_AUDIO_ENABLE,		//现场音频，unsigned long 
	NCFG_ITEM_LIVE_AUDIO_CHNN,			//现场音频通道,unsigned long--通道数目表示自动
	NCFG_ITEM_LIVE_AUDIO_VOLUME,		//系统音量
	NCFG_ITEM_AUDIO_BROADCAST_ENABLE,	//是否开启了语音广播, unsigned long
	NCFG_ITEM_DATE_FORMAT,				//日期格式，unsigned long
	NCFG_ITEM_HOUR_FORMAT,				//时间格式：12H或者24H。unsigned long
	NCFG_ITEM_LANGUAGE,					//语言，unsigned long
	NCFG_ITEM_TIME_ZONE,				//时区，long : g_zoneName 下标
	NCFG_ITEM_TIME_SYNC,				//是否网络时间同步，unsigned long
	NCFG_ITEM_TIME_SERVER,				//网络时间同步服务器地址，NCFG_INFO_LONG_NAME
	NCFG_ITEM_LIVE_TIME_INFO,			//DVR现场时间是否显示, OSD_INFO
	NCFG_ITEM_SCRENN_SAVE_ENABLE,		//是否开启屏保, unsigned long
	NCFG_ITEM_SCRENN_SAVE_TIME,			//屏保时间, unsigned long
	NCFG_ITEM_REC_RECYCLE,				//是否覆盖录像，unsigned long
	NCFG_ITEM_VGA_RESOLUTION,			//显示器分辨率，unsigned long，参考dvrdvsdef.h中的VGA_RESOLUTION枚举
	NCFG_ITEM_VGA_REFRESH,			    //显示器屏幕刷新率,VGA_SUPPORT_REFRSHHZ[n]
	NCFG_ITEM_USER,						//用户信息，NCFG_INFO_USER [MAX_USER_NUM]
	NCFG_ITEM_DAYLIGHT_INFO,			//夏令时信息，DAYLIGHT_INFO
	NCFG_ITEM_MAX_CLIENT_NUM,			//最大网络用户数目，不能超出产品定义的最大值。unsigned long
	NCFG_ITEM_TRANSPARENCY,				//菜单透明度。unsigned long
	NCFG_ITEM_SHOW_WIZZARD,				//开机是否显示向导。unsigned long
	NCFG_ITEM_DISK_ALARM_FREE,			//磁盘报警空间（考虑到可能有零空间报警的需求，所以把最大数表示不报警）,unsigned long
	NCFG_ITEM_NTP_PORT,					//NTP服务器端口，unsigned long
	NCFG_ITEM_NTP_PERIOD,				//校时间隔，unsigned long
	NCFG_ITEM_LIVE_REC_STATUS,			//现场是否显示录像状态，unsigned long
	NCFG_ITEM_DODWELL,					//是否跳台,unsigned long
	NCFG_ITEM_TOOLBAR_POS,				//工具条位置,NCFG_INFO_TOOLBAR_POS
	NCFG_ITEM_CHANN_VISIBLE,			//未登录 通道是否可见 unsigned long(1是不可见，0可见)
	
	NCFG_ITEM_SEARCH_GROUP_ENABLE,      //通道分组的启用
	NCFG_ITEM_SEARCH_GROUP_ITEM,		//通道分组(搜索页面下) NCFG_INFO_GROUP_ITEM[MAX_GROUP_ITEM_COUNT] ---added by wn --2011.7.4
	NCFG_ITEM_CVBS_VO_ADJUST,			//调整CVBS输出，有的监视器输出在（0， 0）位置时会出现画面被遮挡的情况。
	NCFG_ITEM_CVBS_TO_SPOT,				//CVBS作为SPOT用 NCFG_CVBS_INFO
	NCFG_ITEM_ENCODE_960H,				//编码960H分辨率
	NCFG_ITEM_SYSTEM_END,
	
	//通道配置
	NCFG_ITEM_CHANNEL_BASE=0x0200,
	NCFG_ITEM_CAMERA_NAME,				//摄像机名称，NCFG_INFO_SHORT_NAME[VIDEO_INPUT_NUM]
	NCFG_ITEM_SHOW_CAM_NAME,			//DVR现场是否显示摄像机名称， unsigned long [VIDEO_INPUT_NUM]
	NCFG_ITEM_PTZ_SERIAL,               //每个通道的云台的协议、地址、波特率 PTZ_SERIAL_INFO[VIDEO_INPUT_NUM]
	NCFG_ITEM_VIEW_SPLIT_MODE,			//表示现场预览时的分割模式，各种中只能选择一种。unsigned long
	NCFG_ITEM_VIEW_DWELL_TIME,			//分组跳台的时间间隔，零表示跳过本组。unsigned long[32]
	NCFG_ITEM_VIEW_SPLIT_1X1,			//1X1分割信息:unsigned long[32*1*1+2] [总组数][有效组数][第一组...][...]
	NCFG_ITEM_VIEW_SPLIT_2X2,			//unsigned long [8*2*2+2] 总组数>可用组数,为兼容各种分割预留空间
	NCFG_ITEM_VIEW_SPLIT_2X3,			//unsinged long [11*2*3+2] 
	NCFG_ITEM_VIEW_SPLIT_3X3,			//unsigned long [6*3*3+2]
	NCFG_ITEM_VIEW_SPLIT_4X4,			//unsigned long [4*4*4+2]
	NCFG_ITEM_VIEW_SPLIT_5X5,			//unsigned long [4*5*5+2]
	NCFG_ITEM_VIEW_SPLIT_6X6,			//unsigned long [3*6*6+2]
	NCFG_ITEM_SPOT_SPLIT_MODE,			//表示SPOT预览时的分割模式，目前只能定为一分割。unsigned long
	NCFG_ITEM_SPOT_DWELL_TIME,			//SPOT的跳台时间，零表示不跳台。unsigned long
	NCFG_ITEM_SPOT_SPLIT_1X1,			//unsigned long[32*1*1+2]
	NCFG_ITEM_VIDEO_COLOR,				//视频色彩：unsigned long[VIDEO_INPUT_NUM]
	NCFG_ITEM_TIME_STAMP_INFO,			//叠加时间戳信息, OSD_INFO[VIDEO_INPUT_NUM]
	NCFG_ITEM_CAM_NAME_STAMP_INFO,		//叠加通道名称信息, OSD_INFO[VIDEO_INPUT_NUM]
	NCFG_ITEM_CAM_COVER_INFO,			//图像遮挡信息, CAMERA_COVER_INFO[VIDEO_INPUT_NUM]
	NCFG_ITEM_VIEW_SPLIT_4X6,			//unsigned long [4*4*6+2]  分割模式
	NCFG_ITEM_PTZ_CURISE_TYPE,			//通道巡航线类型配置unsigned long [VIDEO_INPUT_NUM]
	NCFG_ITEM_CAM_ROI_INFO,             //ROI    VIDEO_CONFIG_ROI_INFO[NET_VIDEO_INPUT_NUM]
	NCFG_ITEM_CAM_MIRROR,				//图像镜像 unsigned char
	NCFG_ITEM_CAM_FILP,				    //图像翻转 unsigned char
	NCFG_ITEM_CAM_ROTATO,               // 图像旋转(走廊模式) IMAGE_ROTATO
	NCFG_ITEM_CAM_LENS_DISTORTION,		// 镜头畸变 LENS_DISTORTION_CONFIG
	NCFG_ITEM_CAM_TIME_FOCUS_MODE,			//对焦模式 unsigned char
	NCFG_ITEM_CAM_IRCHANGE_FOCUS,			//日夜切换时对焦 unsigned char
	NCFG_ITEM_CAM_INTERVAL_TIME,			//对焦间隔时间 unsigned long
	NCFG_ITEM_CHANNEL_END,

	NCFG_ITEM_CHANNEL_REC_BASE=0x0300,   //通道录像配置
	NCFG_ITEM_REC_ENABLE,				//录像总开关，unsigned long [VIDEO_INPUT_NUM]
	NCFG_ITEM_PRE_ALARM_REC_TIME,		//预录像时间（秒），unsigned long[VIDEO_INPUT_NUM]
	NCFG_ITEM_POST_ALARM_REC_TIME,		//报警录像保持时间（秒），unsigned long[VIDEO_INPUT_NUM]
	NCFG_ITEM_REC_HOLD_TIME,			//录像数据保持期（秒），unsigned long[VIDEO_INPUT_NUM]
	NCFG_ITEM_REC_AUDIO,				//此通道是否录音频，unsigned long [VIDEO_INPUT_NUM]
	NCFG_ITEM_REC_REDUNDANT,            //此通道是否冗余录像，unsigned long [VIDEO_INPUT_NUM]
	NCFG_ITEM_CHANNEL_REC_END,

	//编码参数
	NCFG_ITEM_ENCODE_BASE=0x0400,
	NCFG_ITEM_ENCODE_MAJOR,				//主码流(手动、定时)编码参数，ENCODE_INFO [VIDEO_INPUT_NUM]
	NCFG_ITEM_ENCODE_MINOR,				//子码流编码参数，ENCODE_INFO [VIDEO_INPUT_NUM]
	NCFG_ITEM_ENCODE_PHONE,				//手机码流编码参数，ENCODE_INFO [VIDEO_INPUT_NUM]
	NCFG_ITEM_ENCODE_MAJOR_EVENT,		//主码流(事件)编码参数，ENCODE_INFO [VIDEO_INPUT_NUM]
	NCFG_ITEM_JPEG_PERIOD,				//JPEG抓图间隔，unsigned long [VIDEO_INPUT_NUM]
	NCFG_ITEM_JPEG_SIZE,                //JPEG抓图分辨率，unsigned long [VIDEO_INPUT_NUM]
	NCFG_ITEM_JPEG_QUALITY,             //JPEG抓图画质，unsigned long [VIDEO_INPUT_NUM]
	NCFG_ITEM_JPEG_SNAP_NUM,            //JPEG抓图张数，unsigned long [VIDEO_INPUT_NUM]
	NCFG_ITEM_ENCODE_MAJOR_SPEC,        //录像的分辨率的特殊配置，比如在2432上，1,5,9,13,17,21,25,29可以配置成D1或CIF，而其他通道只能是CIF
	NCFG_ITEM_ENCODE_MAJOR_EX,			//兼容网络设备编码的主码流参数配置ENCODE_INFO_EX
	NCFG_ITEM_IPC_SUPPORT_ENCODE,		//NCFG_IPC_SUPPORT_ENCODE
	NCFG_ITEM_ENCODE_MINOR_EX,
	NCFG_ITEM_BITRANGE_FOR_VIDEOSIZE,		//主码流每个分辨率的码流范围
	NCFG_ITEM_MAJOR_CHANNEL_RECORD_PARAM_EX,	//混合型dvr每个通道的录像参数配置
	NCFG_ITEM_ENCODE_END,

	//计划,以下计划一定要看仔细，比如传感器报警录像计划和传感器侦测计划。
	NCFG_ITEM_SCHEDULE_BASE=0x0500,
	NCFG_ITEM_REC_SCH_SCHEDULE_WEEK,	//排程录像周计划， WEEK_SCHEDULE[VIDEO_INPUT_NUM]
	NCFG_ITEM_REC_SCH_MOTION_WEEK,		//移动侦测录像周计划， WEEK_SCHEDULE[VIDEO_INPUT_NUM]
	NCFG_ITEM_REC_SCH_SENSOR_WEEK,		//传感器报警录像周计划，WEEK_SCHEDULE[VIDEO_INPUT_NUM]
	NCFG_ITEM_SENSOR_IN_SCH_WEEK,		//传感器侦测周计划， WEEK_SCHEDULE[SENSOR_INPUT_NUM]
	NCFG_ITEM_ALARMOUT_SCH_WEEK,		//报警输出周计划WEEK_SCHEDULE[ALARMOUT_NUM]
	NCFG_ITEM_MOTION_SCH_WEEK,			//移动侦测周计划WEEK_SCHEDULE[VIDEO_INPUT_NUM]
	NCFG_ITEM_NET_ALARM_SCH_WEEK,		//网络报警周计划WEEK_SCHEDULE[VIDEO_INPUT_NUM]
	NCFG_ITEM_SCHEDULE_END,

	//网络功能
	NCFG_ITEM_NET_BASE=0x0600,
	NCFG_ITEM_UPNP_ENABLE,				//是否启动UPNP功能, unsigned long
	NCFG_ITEM_UPNP_FRIENDLY_NAME,		//UPNP设备显示的名字, NCFG_INFO_LONG_NAME
	NCFG_ITEM_HTTP_PORT,				//HTTP 端口, unsigned long
	NCFG_ITEM_SERVER_PORT,				//网络服务端口， unsigned long
	NCFG_ITEM_NETWORK_ADDR_INFO,		//网络地址相关信息，NETWORK_ADDR_INFO
	NCFG_ITEM_DNS_1,					//unsigned long
	NCFG_ITEM_DNS_2,					//unsigned long
	NCFG_ITEM_DDNS_INFO,				//DDNS信息，DDNS_INFO
	NCFG_ITEM_PPPOE_INFO,				//PPPoE的相关信息，PPPOE_INFO
	NCFG_ITEM_EMAIL_SEND_INFO,			//邮件功能发送方信息,MAIL_SEND_INFO
	NCFG_ITEM_EMAIL_RECV_INFO,			//邮件功能接收方信息,MAIL_RECV_INFO[MAX_MAIL_RECV_NUM]
	NCFG_ITEM_EMAIL_PIC_ACC,			//邮件附带图片附件，零表示不发送，数字为具体图片数目
	NCFG_ITEM_MULTI_NET_ADDR,			//网络多播地址
	NCFG_ITEM_DDNS_UPDATE_CYCLE,		//ddns功能向服务端更新的周期，单位为天
	NCFG_ITEM_BAN_ALLOW_TYPE,			//黑白名单类型，unsigned long
	NCFG_ITEM_BAN_ALLOW_LIST,			//黑白名单列表, IP_SECTION[MAX_BAN_ALLOW_LIST_NUM]
	NCFG_ITEM_HRSST_SERVER_IP,          //手视通服务器IP地址
	NCFG_ITEM_HRSST_SERVER_PORT,        //手视通服务器端口
	NCFG_ITEM_HXHT_NET_PARAM,			//互信互通网络配置信息HXHT_NET_PARAM_INFO
	NCFG_ITEM_NET_WIRELESS,				//无线网络 MDVR_WIRELESS_CFG
	NCFG_ITEM_AUTO_REPORT,				//自动上报  NET_AUTO_REPORT
	NCFG_ITEM_MINT_DDNS_SERVER_NAME,	//mint ddns 类型服务器的域名
	NCFG_ITEM_USE_3G,				    //页面是本地ip页面还是无线3G页面
	NCFG_ITEM_USE_IPV4_IPV6,		//IPV4和IPV6是否启用, USE_IPV4_IPV6
	NCFG_ITEM_NETWORK_ADDR_INFO_IPV6,		//IPV6配置信息, NETWORK_ADDR_INFO_IPv6
	NCFG_ITEM_DNS_1_IPV6,					//unsigned char[40]
	NCFG_ITEM_DNS_2_IPV6,					//unsigned cahr[40]
	NCFG_ITEM_NET_YZ_PARAM,                 //YUANZHENG_PARAM;
	NCFG_ITEM_NET_YZ_SNAP_PARAM,            //YUANZHENG_SNAP_PARAM;远征抓图参数配置
	NCFG_ITEM_NET_SHANGHAI_DAILY_IMAGE,					//上海平台日常图像配置，SNAP_INFOEX[100];
	NCFG_ITEM_NET_SHANGHAI_PRE_ALARM_SNAP_TIME,			//上海平台预抓图时间（秒），unsigned long[VIDEO_INPUT_NUM]
	NCFG_ITEM_NET_SHANGHAI_POST_ALARM_SNAP_TIME,		//上海平台报警抓图保持时间（秒），unsigned long[VIDEO_INPUT_NUM]
	NCFG_ITEM_NET_SHANGHAI_MOTIONUPLOAD_ENABLE,			//上海平台移动侦测上传启用	
	NCFG_ITEM_NET_SHANGHAI_PTENABLE,					//上海平台接入启用
	NCFG_ITEM_NET_SHANGHAI_AGENTID,						//上海平台连接时候的接入ＩＤ
	NCFG_ITEM_NET_SHANGHAI_IP,							//上海平台连接时候的接入IP
	NCFG_ITEM_NET_SHANGHAI_PORT,						//上海平台连接时候的接入端口
	NCFG_ITEM_NET_FTP,									//FTP配置信息
	NCFG_ITEM_NAT_SERVER,								//NAT服务器配置信息
	NCFG_ITEM_PUSH_SERVER,								//推送服务器配置信息
	NCFG_ITEM_SWITCH_INFO,								//POE-NVR的switch信息,NET_SWITCH_INFO
	NCFG_ITEM_USE_PUSH,									//是否使用推送功能
	NCFG_ITEM_NET_END=0x06ff,

	//报警联动功能
	NCFG_ITEM_ALARM_TRIGER_BASE=0x0700,
	NCFG_ITEM_BUZZER_ENABLE,			//蜂鸣器开关，unsigned long
	NCFG_ITEM_BUZZER_HOLD_TIME,			//蜂鸣器保持时间，unsigned long,
	NCFG_ITEM_ALARM_OUT_NAME,			//继电器输出名称，NCFG_INFO_SHORT_NAME[SENSOR_OUTPUT_NUM]
	NCFG_ITEM_ALARM_OUT_HOLD_TIME,		//继电器输出保持时间，unsigned long [SENSOR_OUTPUT_NUM]
	NCFG_ITEM_ALARM_OUT_TYPE,			//继电器输出设备类型
	NCFG_ITEM_ALARM_TRIGER_END,

	NCFG_ITEM_ALARM_TRIGER_SENSOR_BASE=0x0800,
	NCFG_ITEM_SENSOR_IN_ENABLE,			//传感器开关，unsigned long [SENSOR_INPUT_NUM]
	NCFG_ITEM_SENSOR_IN_TYPE,			//传感器设备类型，unsigned long [SENSOR_INPUT_NUM]
	NCFG_ITEM_SENSOR_IN_NAME,			//传感器名称，NCFG_INFO_SHORT_NAME [SENSOR_INPUT_NUM]
	NCFG_ITEM_SENSOR_IN_HOLD_TIME,		//传感器保持时间, unsigned long [SENSOR_INPUT_NUM]
	NCFG_ITEM_SENSOR_IN_BUZZ,           //传感器触发蜂鸣器, unsigned long [SENSOR_INPUT_NUM]
	NCFG_ITEM_SENSOR_IN_TO_ALARM_OUT,	//传感器触发报警输出，ULONGLONG [SENSOR_INPUT_NUM];
	NCFG_ITEM_SENSOR_IN_TO_RECORD,		//传感器触发录像，ULONGLONG [SENSOR_INPUT_NUM];
	NCFG_ITEM_SENSOR_TO_BIG_VIEW,		//传感器触发大画面，ULONGLONG[VIDEO_INPUT_NUM]
	NCFG_ITEM_SENSOR_IN_TO_EMAIL_ENABLE,//传感器能够触发EMAIL，unsigned long
	NCFG_ITEM_SENSOR_TO_EMAIL_ADDR,		//传感器触发MAIL地址列表，unsigned long
	NCFG_ITEM_SENSOR_IN_TO_FTP_ENABLE,	//传感器触发触发FTP，unsigned long
	NCFG_ITEM_SENSOR_IN_TO_FTP_ADDR,	//传感器触发FTP地址列表，和MAIL处理方式一样unsigned long[n] 
	NCFG_ITEM_SENSOR_IN_TO_PTZ,			//传感器触发云台联动，TO_PTZ [SENSOR_INPUT_NUM][VIDEO_INPUT_NUM]
	NCFG_ITEM_SENSOR_IN_TO_SNAP,		//传感器触发抓图的通道ULONGLONG [SENSOR_INPUT_NUM]
	NCFG_ITEM_SENSOR_TO_FTP_SNAP,		//传感器触发FTP抓图，ULONGLONG[VIDEO_INPUT_NUM]
	NCFG_ITEM_SENSOR_TO_EMAIL_SNAP,		//传感器触发EMAIL抓图，ULONGLONG[VIDEO_INPUT_NUM]
	NCFG_ITEM_ALARM_TRIGER_SENSOREND,

	NCFG_ITEM_ALARM_TRIGER_MOTION_BASE=0x0900,	
	NCFG_ITEM_MOTION_ENABLE,			//移动侦测开关，unsigned long [VIDEO_INPUT_NUM]
	NCFG_ITEM_MOTION_AREA_INFO,         //区域的信息，MOTION_DETECT_INFO[VIDEO_INPUT_NUM]
	NCFG_ITEM_MOTION_HOLD_TIME,			//移动侦测保持时间,unsigned long [VIDEO_INPUT_NUM]
	NCFG_ITEM_MOTION_TO_ALARM_OUT,		//移动侦测触发报警输出，ULONGLONG [VIDEO_INPUT_NUM]
	NCFG_ITEM_MOTION_TO_RECORD,			//移动侦测触发录像，ULONGLONG [VIDEO_INPUT_NUM]
	NCFG_ITEM_MOTION_TO_BIG_VIEW,		//移动侦测触发大画面，ULONGLONG[VIDEO_INPUT_NUM]
	NCFG_ITEM_MOTION_TO_EMAIL_ENABLE,	//移动侦测能够触发EMAIL，unsigned long
	NCFG_ITEM_MOTION_TO_EMAIL_ADDR,		//移动侦测触发MAIL地址列表，unsigned long
	NCFG_ITEM_MOTION_TO_PTZ,			//移动侦测触发云台，TO_PTZ [VIDEO_INPUT_NUM][VIDEO_INPUT_NUM]
	NCFG_ITEM_MOTION_TO_FTP_ENABLE,		//移动侦测触发FTP，unsigned long
	NCFG_ITEM_MOTION_TO_FTP_ADDR,		//移动侦测触发FTP地址列表，和MAIL处理方式一样unsigned long[n] 
	NCFG_ITEM_MOTION_TO_BUZZ,           //移动侦测触发BUZZ unsigned long[VIDEO_INPUT_NUM]
	NCFG_ITEM_MOTION_TO_SNAP,           //移动侦测触发抓拍 ULONGLONG[VIDEO_INPUT_NUM]
	NCFG_ITEM_MOTION_TO_FTP_SNAP,		//移动侦测触发FTP抓图，ULONGLONG[VIDEO_INPUT_NUM]
	NCFG_ITEM_MOTION_TO_EMAIL_SNAP,		//移动侦测触发EMAIL抓图，ULONGLONG[VIDEO_INPUT_NUM]
	NCFG_ITEM_HXHT_MOTION_AREA_INFO,	//互信互通移动区域配置，HXHT_MOTION_AREA_INFO[VIDEO_INPUT_NUM]
	NCFG_ITEM_ALARM_TRIGER_MOTIONEND,
	
	NCFG_ITEM_ALARM_TRIGER_VLOSS_BASE=0x0a00,
	NCFG_ITEM_VL_TO_ALARM_OUT,			//视频丢失触发报警输出，ULONGLONG [VIDEO_INPUT_NUM]
	NCFG_ITEM_VL_TO_RECORD,				//视频丢失触发录像，ULONGLONG [VIDEO_INPUT_NUM]
	NCFG_ITEM_VL_TO_BIG_VIEW,			//视频丢失触发大画面，ULONGLONG[VIDEO_INPUT_NUM]
	NCFG_ITEM_VL_TO_EMAIL_ENABLE,		//视频丢失能够触发MAIL unsigned long
	NCFG_ITEM_VL_TO_EMAIL_ADDR,			//视频丢失触发MAIL地址列表，unsigned long
	NCFG_ITEM_VL_TO_PTZ,				//视频丢失触发云台，TO_PTZ [VIDEO_INPUT_NUM][VIDEO_INPUT_NUM]
	NCFG_ITEM_VL_TO_FTP_ENABLE,			//视频丢失触发FTP，unsigned long
	NCFG_ITEM_VL_TO_FTP_ADDR,			//视频丢失触发FTP地址列表，和MAIL处理方式一样unsigned long[n] 
	NCFG_ITEM_VL_TO_BUZZ,               //视频丢失触发BUZZ unsigned long[VIDEO_INPUT_NUM]
	NCFG_ITEM_VL_TO_SNAP,               //视频丢失触发抓拍 ULONGLONG[VIDEO_INPUT_NUM]
	NCFG_ITEM_VL_TO_FTP_SNAP,		    //视频丢失触发FTP抓图，ULONGLONG[VIDEO_INPUT_NUM]
	NCFG_ITEM_VL_TO_EMAIL_SNAP,		    //视频丢失触发EMAIL抓图，ULONGLONG[VIDEO_INPUT_NUM]
	NCFG_ITEM_ALARM_TRIGER_VLOSSEND,

	NCFG_ITEM_ALARM_TRIGER_SHELTER_BASE=0x0b00,
	NCFG_ITEM_SHELTER_TO_ALARM_OUT,		//遮挡触发报警输出，ULONGLONG [VIDEO_INPUT_NUM]
	NCFG_ITEM_SHELTER_TO_RECORD,		//遮挡触发录像，ULONGLONG [VIDEO_INPUT_NUM]
	NCFG_ITEM_SHELTER_TO_BIG_VIEW,		//遮挡触发大画面，ULONGLONG[VIDEO_INPUT_NUM]
	NCFG_ITEM_SHELTER_TO_EMAIL_ENABLE,	//遮挡能够触发EMAIL，unsigned long
	NCFG_ITEM_SHELTER_TO_EMAIL_ADDR,	//遮挡触发EMAIL地址表,unsigned long
	NCFG_ITEM_SHELTER_TO_FTP_ENABLE,	//遮挡触发FTP，unsigned long
	NCFG_ITEM_SHELTER_TO_BUZZ,          //遮挡触发BUZZ unsigned long[VIDEO_INPUT_NUM]
	NCFG_ITEM_SHELTER_TO_PTZ,			//遮挡触发云台，TO_PTZ [VIDEO_INPUT_NUM][VIDEO_INPUT_NUM]
	NCFG_ITEM_SHELTER_TO_SNAP,          //遮挡触发抓拍 ULONGLONG[VIDEO_INPUT_NUM]
	NCFG_ITEM_SHELTER_TO_FTP_SNAP,		//遮挡触发FTP抓图，ULONGLONG[VIDEO_INPUT_NUM]
	NCFG_ITEM_SHELTER_TO_EMAIL_SNAP,	//遮挡触发EMAIL抓图，ULONGLONG[VIDEO_INPUT_NUM]
	NCFG_ITEM_ALARM_TRIGER_SHELTEREND,
	
	//
	NCFG_ITEM_SCH_HOLIDAY_BASE = 0x0c00,
	NCFG_ITEM_REC_SCH_SCHEDULE_HOLIDAY,	//排程录像假日计划， HOLIDAY_SCHEDULE [MAX_HOLIDAY_NUM][VIDEO_INPUT_NUM]
	NCFG_ITEM_REC_SCH_MOTION_HOLIDAY,	//移动侦测录像假日计划, HOLIDAY_SCHEDULE [MAX_HOLIDAY_NUM][VIDEO_INPUT_NUM]
	NCFG_ITEM_REC_SCH_SENSOR_HOLIDAY,	//传感器报警录像假日计划, HOLIDAY_SCHEDULE [MAX_HOLIDAY_NUM][VIDEO_INPUT_NUM]
	NCFG_ITEM_SENSOR_IN_SCH_HOLIDAY,	//传感器侦测假日计划HOLIDAY_SCHEDULE [MAX_HOLIDAY_NUM][SENSOR_INPUT_NUM]
	NCFG_ITEM_ALARMOUT_SCH_HOLIDAY,		//报警输出假日计划HOLIDAY_SCHEDULE [MAX_HOLIDAY_NUM][ALARMOUT_NUM]
	NCFG_ITEM_MOTION_SCH_HOLIDAY,		//移动侦测假日计划HOLIDAY_SCHEDULE [MAX_HOLIDAY_NUM][VIDEO_INPUT_NUM]
	NCFG_ITEM_NET_ALARM_SCH_HOLIDAY,
	NCFG_ITEM_SCH_HOLIDAY_END,

	/////车载相关//////////////////////////////////////////////////////////////
	NCFG_ITEM_MOBILE_BASE = 0x0d00,
	NCFG_ITEM_COMPANY_NAME,					//车队公司名称, NCFG_INFO_LONG_NAME
	NCFG_ITEM_DRIVER_NAME,					//司机名称, NCFG_INFO_SHORT_NAME
	NCFG_ITEM_CAR_NUMBER,					//机车号码, NCFG_INFO_SHORT_NAME
	NCFG_ITEM_SHUT_DOWN_DELAY,				//关机延时, unsigned long
	NCFG_ITEM_NET_SERVER,					//网络服务器，MDVR_NETSERVER_CFG
	NCFG_ITEM_MOBILE_END,

	NCFG_ITEM_SPEED_BASE = 0x0d40,
	NCFG_ITEM_SPEED_SOURCE,					//速度来源，unsigned long
	NCFG_ITEM_SPEED_UNIT,					//速度单位，unsigned long
	NCFG_ITEM_SPEED_END,

	NCFG_ITEM_HIGH_SPEED_BASE = 0xd50,
	NCFG_ITEM_HIGH_SPEED_ENABLE,			//高速侦测开关，unsigned long
	NCFG_ITEM_HIGH_SPEED_VALUE,				//高速门限值，MDVR_SPEED
	NCFG_ITEM_HIGH_SPEED_HOLD_TIME,			//高速触发报警保持时间, unsigned long 
	NCFG_ITEM_HIGH_SPEED_BUZZ,				//高速触发蜂鸣器, unsigned long
	NCFG_ITEM_HIGH_SPEED_TO_ALARM_OUT,		//高速触发报警输出，ULONGLONG
	NCFG_ITEM_HIGH_SPEED_TO_RECORD,			//高速触发录像，ULONGLONG 
	NCFG_ITEM_HIGH_SPEED_BIG_VIEW,			//高速触发大画面，ULONGLONG
	NCFG_ITEM_HIGH_SPEED_TO_EMAIL_ENABLE,	//高速能够触发EMAIL，unsigned long
	NCFG_ITEM_HIGH_SPEED_TO_EMAIL_ADDR,		//高速触发MAIL地址列表，unsigned long
	NCFG_ITEM_HIGH_SPEED_TO_FTP_ENABLE,		//高速触发触发FTP，unsigned long
	NCFG_ITEM_HIGH_SPEED_TO_FTP_ADDR,		//高速触发FTP地址列表，和MAIL处理方式一样unsigned long[n] 
	NCFG_ITEM_HIGH_SPEED_TO_PTZ,			//高速触发云台联动，TO_PTZ[VIDEO_INPUT_NUM]
	NCFG_ITEM_HIGH_SPEED_TO_SNAP,			//高速触发抓图的通道ULONGLONG
	NCFG_ITEM_HIGH_SPEED_TO_FTP_SNAP,		//高速触发FTP抓图，ULONGLONG
	NCFG_ITEM_HIGH_SPEED_TO_EMAIL_SNAP,		//高速触发EMAIL抓图，ULONGLONG
	NCFG_ITEM_HIGH_SPEED_END,

	NCFG_ITEM_LOW_SPEED_BASE = 0xd70,
	NCFG_ITEM_LOW_SPEED_ENABLE,				//低速侦测开关， unsigned long
	NCFG_ITEM_LOW_SPEED_VALUE,				//低速门限值，MDVR_SPEED
	NCFG_ITEM_LOW_SPEED_HOLD_TIME,			//低高速触发报警保持时间, unsigned long [SENSOR_INPUT_NUM]	
	NCFG_ITEM_LOW_SPEED_BUZZ,				//低速触发蜂鸣器, unsigned long
	NCFG_ITEM_LOW_SPEED_TO_ALARM_OUT,		//低速触发报警输出，ULONGLONG
	NCFG_ITEM_LOW_SPEED_TO_RECORD,			//低速触发录像，ULONGLONG
	NCFG_ITEM_LOW_SPEED_BIG_VIEW,			//低速触发大画面，ULONGLONG
	NCFG_ITEM_LOW_SPEED_TO_EMAIL_ENABLE,	//低速能够触发EMAIL，unsigned long
	NCFG_ITEM_LOW_SPEED_TO_EMAIL_ADDR,		//低速触发MAIL地址列表，unsigned long
	NCFG_ITEM_LOW_SPEED_TO_FTP_ENABLE,		//低速触发触发FTP，unsigned long
	NCFG_ITEM_LOW_SPEED_TO_FTP_ADDR,		//低速触发FTP地址列表，和MAIL处理方式一样unsigned long[n] 
	NCFG_ITEM_LOW_SPEED_TO_PTZ,				//低速触发云台联动，TO_PTZ[VIDEO_INPUT_NUM]
	NCFG_ITEM_LOW_SPEED_TO_SNAP,			//低速触发抓图的通道 ULONGLONG
	NCFG_ITEM_LOW_SPEED_TO_FTP_SNAP,		//低速触发FTP抓图，ULONGLONG
	NCFG_ITEM_LOW_SPEED_TO_EMAIL_SNAP,		//低速触发EMAIL抓图，ULONGLONG
	NCFG_ITEM_LOW_SPEED_END,

	NCFG_ITEM_WALLOP_BASE = 0x0d90,
	NCFG_ITEM_WALLOP_INFO_X,				//冲击力X，MDVR_WALLOP_STATE
	NCFG_ITEM_WALLOP_INFO_Y,				//冲击力Y，MDVR_WALLOP_STATE
	NCFG_ITEM_WALLOP_INFO_Z,				//冲击力Z，MDVR_WALLOP_STATE
	NCFG_ITEM_WALLOP_BUZZ,					//冲击力触发蜂鸣器, unsigned long
	NCFG_ITEM_WALLOP_TO_ALARM_OUT,			//冲击力触发报警输出，ULONGLONG
	NCFG_ITEM_WALLOP_TO_RECORD,				//冲击力触发录像，ULONGLONG
	NCFG_ITEM_WALLOP_BIG_VIEW,				//冲击力触发大画面，ULONGLONG
	NCFG_ITEM_WALLOP_TO_EMAIL_ENABLE,		//冲击力能够触发EMAIL，unsigned long
	NCFG_ITEM_WALLOP_TO_EMAIL_ADDR,			//冲击力触发MAIL地址列表，unsigned long[n]
	NCFG_ITEM_WALLOP_TO_FTP_ENABLE,			//冲击力触发触发FTP，unsigned long
	NCFG_ITEM_WALLOP_TO_FTP_ADDR,			//冲击力触发FTP地址列表，和MAIL处理方式一样unsigned long[n] 
	NCFG_ITEM_WALLOP_TO_PTZ,				//冲击力触发云台联动，TO_PTZ [VIDEO_INPUT_NUM]
	NCFG_ITEM_WALLOP_TO_SNAP,				//冲击力触发抓图的通道 ULONGLONG
	NCFG_ITEM_WALLOP_TO_FTP_SNAP,			//冲击力触发FTP抓图，ULONGLONG
	NCFG_ITEM_WALLOP_TO_EMAIL_SNAP,			//冲击力触发EMAIL抓图，ULONGLONG
	NCFG_ITEM_WALLOP_END,

	NCFG_ITEM_WIRELESS_BASE = 0x0e00,
	NCFG_ITEM_WIRELESS_WIFI,				//WIFI配置，WIFI_CONFIG
	NCFG_ITEM_WIRELESS_COMM,				//无线通信配置，MDVR_WIRELESS_CFG
	NCFG_ITEM_WIRELESS_END,
	/////车载相关-end//////////////////////////////////////////////////////////////

	NCFG_ITEM_EXCEPTION_BASE = 0x0f00,
	NCFG_ITEM_DISK_FULL_BUZZER,				//磁盘满触发蜂鸣器报警 unsigned long
	NCFG_ITEM_DISK_FULL_TO_ALARM_OUT,		//磁盘满触发报警输出 ULONGLONG
	NCFG_ITEM_DISK_FULL_TO_EMAIL,			//磁盘满触发邮件 unsigned long
	NCFG_ITEM_IP_CONFLICT_BUZZER,			//网络地址冲突触发蜂鸣器报警 unsigned long
	NCFG_ITEM_IP_CONFLICT_TO_ALARM_OUT,		//网络地址冲突触发报警输出 ULONGLONG
	NCFG_ITEM_IP_CONFLICT_TO_EMAIL,			//网络地址冲突触发邮件 unsigned long
	NCFG_ITEM_DISCONNECT_BUZZER,			//网络断开触发蜂鸣器报警 unsigned long
	NCFG_ITEM_DISCONNECT_TO_ALARM_OUT,		//网络断开触发报警输出 ULONGLONG
	NCFG_ITEM_DISCONNECT_TO_EMAIL,			//网络断开触发邮件 unsigned long
	NCFG_ITEM_DISK_WARNING_BUZZER,          //硬盘衰减开触发蜂鸣器 unsigned long
	NCFG_ITEM_DISK_WARNING_TO_ALARM_OUT,    //硬盘衰减触发报警输出 ULONGLONG
	NCFG_ITEM_DISK_WARNING_TO_EMAIL,        //硬盘衰减触发邮件 unsigned long
	NCFG_ITEM_DISK_DISCONNECT_BUZZER,		//硬盘掉线触发蜂鸣器
	NCFG_ITEM_DISK_DISCONNECT_TO_ALARM_OUT,	//硬盘掉线触发报警输出 ULONGLONG
	NCFG_ITEM_DISK_DISCONNECT_TO_EMAIL,		//硬盘掉线触发邮件 unsigned long
	NCFG_ITEM_EXCEPTION_END,

	//国标配置相关
	NCFG_ITEM_GB28181_BASE = 0x1000,
	NCFG_ITEM_GB28181_SERVER_INFO,				//sip 服务器配置，对应 结构体GB28181_SIP_SERVER
	NCFG_ITEM_GB28181_CHANNEL_ID,				//channelID，通道等配置，对应结构体 GB28181_CHANNEL_ID
	NCFG_ITEM_GB28181_ALARM_ID,					//alarmid，告警id等配置，对应结构体GB28181_ALARM_ID
	NCFG_ITEM_GB28181_END,

	//云台参数配置，巡航线，预置点等，通道概念加到了ID里面端口和协议放到了NCFG_ITEM_PTZ_SERIAL里面
	NCFG_ITEM_PTZ_INFO_BASE=0xf800,
	NCFG_ITEM_PTZ_INFO_PRESET_INFO_CHBASE=0xf800,//预置点名字通道基数，保留64个通道ID的位置
	NCFG_ITEM_PTZ_INFO_PRESET_INFO_CHEND =NCFG_ITEM_PTZ_INFO_PRESET_INFO_CHBASE+64,

	NCFG_ITEM_PTZ_INFO_CRUISE_INFO_CHBASE=NCFG_ITEM_PTZ_INFO_PRESET_INFO_CHEND,//巡航线名字通道基数，保留64个通道ID的位置
	NCFG_ITEM_PTZ_INFO_CRUISE_INFO_CHEND =NCFG_ITEM_PTZ_INFO_CRUISE_INFO_CHBASE+64,
	NCFG_ITEM_PTZ_INFO_END=NCFG_ITEM_PTZ_INFO_CRUISE_INFO_CHEND+1,
	
	///只读信息，包括名字最大长度限制，支持选项的范围,版本信息等
	NCFG_ITEM_ONLYREAD_BASE=0xfc00,
	NCFG_ITEM_DEVICE_NAME_MAX_LEN,      //设备名称长度限定, unsigned long,[只读]
	NCFG_ITEM_TIME_SERVER_MAX_LEN,      //网络时间同步服务器地址最大长度, unsigned long,[只读]
	NCFG_ITEM_UPNP_FRIENDLY_NAME_MAX_LEN,//UPNP设备显示的名字最大长度, unsigned long,[只读]
	NCFG_ITEM_CAMERA_NAME_MAX_LEN,      //通道名称最大长度, unsigned long,[只读]
	NCFG_ITEM_SENSOR_IN_NAME_MAX_LEN,   //传感器名称最大长度,unsigned long,[只读]
	NCFG_ITEM_ALARM_OUT_NAME_MAX_LEN,   //继电器输出名称最大长度, unsigned long,[只读]
	NCFG_ITEM_PPPOE_USER_NAME_MAX_LEN,  //PPPoE用户名最大长度，unsigned long,[只读]
	NCFG_ITEM_PPPOE_PASSWORD_MAX_LEN,   //PPPoE密码最大长度, unsigned long,[只读]
	NCFG_ITEM_DDNS_USER_NAME_MAX_LEN,   //DDNS用户名最大长度, unsigned long,[只读]
	NCFG_ITEM_DDNS_PASSWORD_MAX_LEN,    //DDNS密码最大长度, unsigned long,[只读]
	NCFG_ITEM_DDNS_DOMAIN_MAX_LEN,		//DDNS域名名称最大长度, unsigned long,[只读]
	NCFG_ITEM_WIRELESS_SSID_MAX_LEN,    //无线SSID最大长度, unsigned long,[只读]
	NCFG_ITEM_WIRELESS_KEY_MAX_LEN,     //密钥最大长度, unsigned long,[只读]


	NCFG_ITEM_TIME_STAMP_SUPPORT_POS,	//时间戳支持的位置方式，unsigned long 三种时间戳都使用此ID(0x01 << pos的MASK),[只读]
	NCFG_ITEM_SUPPORT_VIDEOFORMAT,		//支持的视频制式,unsigned long,[只读]
	NCFG_ITEM_SUPPORT_QUADMODE,			//支持的分割模式[只读],第几位为1就代表支持哪一个分割模式，unsigned long,[只读]
	NCFG_ITEM_USER_MAXNUM,				//支持用户最大数量，unsigned long[只读]
	NCFG_ITEM_SUPPORT_LANGUAGE,
	NCFG_ITEM_SUPPORT_MAXCLIENTNUM,     //支持的同时在线的网络客户端个数。
	NCFG_ITEM_HOLIDAY_MAX_NUM,			//设置支持的假日排程日期的最大数量，特别是非循环方式时可能需增加，unsigned long
	NCFG_ITEM_DEVUSER_NAME_MAX_LEN,		//设备用户名最大长度，unsigned long[只读]
	NCFG_ITEM_DEVUSER_PWD_MAX_LEN,		//设备用户密码最大长度，unsigned long[只读]
	NCFG_ITEM_VGA_SUPPORT_RESOLUTION,	//VGA芯片支持的分辨率MASK，unsigned long，参考dvrdvsdef.h中的VGA_RESOLUTION枚举[只读]
	NCFG_ITEM_VGA_SUPPORT_REFRESH,		//VGA支持的屏幕刷新率，每种分辨率下都有所不同[只读]
	NCFG_ITEM_DDNS_SERVER_INFO,			//DDNS下拉列表支持的DDNS类型[只读]

	NCFG_ITEM_MAX_PRE_ALARM_TIME,		//预录像的最大时间（秒）
	NCFG_ITEM_ENCODE_MAJOR_SUPPORT_PROPERTY,//表示支持分辨率的个数，再跟若干个NCFG_INFO_SUPPORT_FRAME_RATE[只读]
	NCFG_ITEM_MAJOR_SUPPORT_QULITY_LEVELNUM,	
	NCFG_ITEM_MAJOR_SUPPORT_ENCODEMODE,
	NCFG_ITEM_MAJOR_SUPPORT_BITRATE_RANGE,
	NCFG_ITEM_ENCODE_MINOR_SUPPORT_PROPERTY,//表示支持分辨率的个数，再跟若干个NCFG_INFO_SUPPORT_FRAME_RATE[只读]
	NCFG_ITEM_MINOR_SUPPORT_QULITY_LEVELNUM,
	NCFG_ITEM_MINOR_SUPPORT_ENCODEMODE,
	NCFG_ITEM_MINOR_SUPPORT_BITRATE_RANGE,
	NCFG_ITEM_ENCODE_SIZE_MAJOR_ALL_CH,		//主码流分辨率是否支持所有通道一起配置 unsigned long
	NCFG_ITEM_ENCODE_SIZE_MINOR_ALL_CH,		//子码流分辨率是否支持所有通道一起配置 unsigned long
	NCFG_ITEM_ENCODE_SIZE_PHONE_ALL_CH,		//手机码流分辨率是否支持所有通道一起配置 unsigned long
	NCFG_ITEM_DEVICE_SUPPORT_REDUNDANCY,    //设备是否支持冗余录像

	NCFG_ITEM_CRUISE_PRESET_MAXNUM,		//每个巡航线的最大预置点数量[只读]
	NCFG_ITEM_CHNN_PRESET_MAX_NUM,		//每个通道的最大预置点数量[只读]
	NCFG_ITEM_CHNN_CRUISE_MAX_NUM,		//每个通道的最大巡航线数量[只读]
	NCFG_ITEM_PTZ_SUPPORT_INFO,			//支持的云台协议信息
	
	NCFG_ITEM_DEFAULT_COLOR,			//设备默认色彩值
	
	//MDVR 车载项目只读信息
	NCFG_ITEM_SPEED_VALUE_MUX_LEN,		//机车速度值的最大长度

	//cost版本的限制项
	NCFG_ITEM_SUPPORT_SYNC_TIME,          //是否支持网络校时.1-支持。0-不支持
	NCFG_ITEM_SUPPORT_DST,                //是否支持夏令时功能.1-支持。0-不支持
	NCFG_ITEM_SUPPORT_MASK_AREA,          //是否支持遮挡功能.1-支持。0-不支持
	NCFG_ITEM_SUPPORT_MAIN_MONITOR_SPORT, //是否支持主设备的跳台功能,1-支持。0-不支持
	NCFG_ITEM_MAJOR_HAVE_ENCODE_AND_MAXBIT, //录像是否支持编码方式和最大码流.1-支持。0-不支持
	NCFG_ITEM_MINOR_HAVE_ENCODE_AND_MAXBI,  //网络是否支持编码方式和最大码流.1-支持。0-不支持
	NCFG_ITEM_SPPORT_SIMPLE_USER_MANAGE,    //是否是简单的用户管理模式.1-简单。0-详细.
	
	//邮件中的只读项
	NCFG_ITEM_EMAIL_IMG_NUM_RANGE,       //email配置界面中，图片张数的取值范围和默认值
	NCFG_ITEM_EMAIL_SNAP_TIME_RANGE,    //email配置界面中，抓取图片时间间隔的取值范围和默认值

	//黑白名单数目
	NCFG_ITEM_BAN_ALLOW_NUM,			//黑白名单列表长度


	NCFG_ITEM_SNAP_SIZE_RANGE,               //抓图配置中，图片的画质的分辨率
	NCFG_ITEM_SNAP_QULITY_LEVELNUM,   //抓图配置中，图片的画质的取值范围和默认值
    NCFG_ITEM_SNAP_IMG_NUM_RANGE,     //抓图配置中，图片张数的取值范围和默认值
	NCFG_ITEM_SNAP_TIME_RANGE,        //抓图配置中，抓取图片时间间隔的取值范围和默认值
	NCFG_ITEM_NO_ADD_ONLYREAD,      //以后不要在添加只读项了。如果要添加只读项，就放到配置里。

	NCFG_ITEM_ONLYREAD_END,
}NCFG_ITEM_ID;

enum 
{
	NCFG_ENUM_SUPPORT_VIDEOFORMAT_PAL	= 0x0001,
	NCFG_ENUM_SUPPORT_VIDEOFORMAT_NTSC	= 0x0002
};

//CBR模式下，每种分辨率的码流范围
typedef struct _ncfg_info_bitraterang
{
	unsigned long low;//最低码流
	unsigned long high;//最低码流
}NCFG_INFO_BITRATE_RANG;


//用户的，目前支持成这样子就定义成这样子，权限和通道去掉了,如果改变就换ID或增加ID来实现
typedef struct _ncfg_info_user
{
	unsigned long   bindMac;	//0 不绑定 1绑定
	unsigned long   authority;
	unsigned long   group;

	//一下为各组针对通道的权限，比特位为1代表具有权限，0代表不具备权限。
	ULONGLONG		authLiveCH;			//现场预览通道
	ULONGLONG		authRecordCH;		//手动录像控制
	ULONGLONG		authPlaybackCH;		//检索和回放
	ULONGLONG		authBackupCH;		//备份
	ULONGLONG		authPTZCtrlCH;		//云天控制
	ULONGLONG		authRemoteViewCH;	//远程预览

	char            name[36];
	char            password[36];
	unsigned char   MAC[8];
}NCFG_INFO_USER;

typedef struct _ncfg_info_short_name
{
	char	name [36];
}NCFG_INFO_SHORT_NAME;

typedef struct _ncfg_info_long_name
{
	char	name [132];
}NCFG_INFO_LONG_NAME;


typedef struct _ncfg_info_support_language
{
	unsigned long languageId;
	char          name[64];
}NCFG_INFO_LANGUAGE;

typedef struct _ddns_server_info 
{
	unsigned char  ucDDNSID; /////对应的服务器ID   ID大于0对应的服务器名称才有效
	unsigned char  supportproperty;//支持的除用户名密码外的其它属性的MASK,NCFG_ENUM_DDNS_SUPPORT_PROPERTY
	unsigned char  noused[2];
	char           szDDNSServerName[64];  ////DDNS服务器的地址
}DDNS_SERVER_INFO, *LPDDNS_SERVER_INFO;

/*
typedef struct _ncfg_info_support_frame_rate
{
	unsigned long	dwResolution;	//什么分辨率
	unsigned long	dwRate;			//支持哪些帧率
	char			strResolutionName[16];//分辨率名字
}NCFG_INFO_SUPPORT_FRAME_RATE;
*/
typedef struct _ncfg_info_support_frame_rate
{
	unsigned long dwResolution;				//支持哪些分辨率
	unsigned long totalRatePalOnCif; 	    //pal制的总帧率，用CIF计算。
	unsigned long chnnRatePalOnCif;         //pal制，单通道的最大帧率,用CIF计算。
	unsigned long totalRateNtscOnCif;		//NTSC制的总帧率，用CIF计算。
	unsigned long chnnRateNtscOnCif;        //ntsc制，单通道的最大帧率，用CIF计算
    unsigned long defaResolution;           //产品的默认分辨率 
	unsigned long defauChnnRateNumPal;      //在pal下，在默认制式下，默认每个通道帧率的数值
	unsigned long defauChnnRateNumNtsc;
}NCFG_INFO_SUPPORT_FRAME_RATE;

typedef struct _ncfg_ipc_support_encode_
{
	unsigned long dwResolution;
	unsigned long maxChnnRate;
	unsigned long maxChnnRateOnVGA;
	unsigned long maxTotalRateOnVGA;
	unsigned long maxTotalBitRate;
	unsigned long defaResolution;
	unsigned long defaChnnRate;
	unsigned long defaBitRate;
	unsigned long bLimitByBitRate;	//0:总能力取决于帧率，1：总能力取决于码率
}NCFG_IPC_SUPPORT_ENCODE;

typedef struct _ncfg_info_spec_frame_rate_ex
{
	unsigned long bSupport;                  //是否支持特殊的录像配置
	ULONGLONG     videoSizeCIFChnnBits;      //无意义
	ULONGLONG     videoSizeD1ChnnBits;       //哪些通道可以配置成实时的D1的通道掩码
	ULONGLONG     res1;
	ULONGLONG     res2;
	ULONGLONG     res3;
	ULONGLONG     res4;
}NCFG_INFO_ENCODE_SPEC_FRAME_RATE;

//typedef struct _ncfg_info_time_pos
//{
//	unsigned long postype;
//	unsigned long customdefine_X;
//	unsigned long customdefine_Y;
//}
//NCFG_INFO_TIME_POS;

typedef struct _ncfg_info_toolbar_pos
{
	unsigned short	style;			//0底部, 1顶部, 3任意[按(x,y)值]
	unsigned short	status;			//0固定[不自动隐藏], 1固定[自动隐藏], 2编辑[不隐藏]
	unsigned short  preDelayTime;	//鼠标置于工具条上自动显示的等待时间
	unsigned short  postDelayTime;	//鼠标离开工具条后自动隐藏的等待时间
	unsigned short	xPos;			//参考起始坐标, 百分比, 大于100 居中
	unsigned short	yPos;

	unsigned short	ItemIndex[32];	//功能按钮排布次序
}NCFG_INFO_TOOLBAR_POS;

//#if defined (__CUSTOM_BR27__)
	typedef struct _ncfg_info_group_item_
	{
		char szItem[32];
		ULONGLONG chanbits;
	}NCFG_INFO_GROUP_ITEM;
	typedef struct _ncfg_info_group_item_enableInfo
	{
		unsigned long enable;//搜索分组使能
		unsigned long curItem;//当前的分组序号
	}NCFG_INFO_GROUP_ENABLEINFO;
//#endif
typedef struct _ptz_protocol_info 
{
	unsigned long  ucprotocolID;		//对应的协议类型ID
	unsigned long  supportproperty;		//支持的除ID、波特率之外的其它属性的MASK,比如是否支持轨迹等特殊属性
	char           szProtocolName[64];  ////协议名称
}PTZ_PROTOCOL_INFO;

//DDNS支持其它属性的取值范围，可以随其它类型而增加
typedef enum _ncfg_enum_ddns_support_property
{
	NCFG_ENUM_DDNS_SUPPORT_DOMAIN1	=0x01,//支持域名1，也有可能需要支持两个域名	
}
NCFG_ENUM_DDNS_SUPPORT_PROPERTY;

//NCFG_ITEM_PTZ_INFO_CRUISE_INFO_CHBASE
typedef struct _ncfg_info_cruise_head
{
	unsigned long index;
	unsigned long cruisepointnum;//巡航点数量
	char  name[36];
	unsigned long offset;//所含的每个巡航线相对第一个NCFG_INFO_CRUISE_HEAD开头的偏移
}
NCFG_INFO_CRUISE_HEAD;

const BYTE CRUISE_NAME_MAX_LEN =32;
//巡航线信息头，只含有一个通道的
typedef struct _ptz_cruise_info_head
{
	unsigned long cruiseFlag;//开始停止标记 
	unsigned long cruisepointnum;//巡航点数量
	unsigned long cruiseIndex;//巡航线序号
	char  name[CRUISE_NAME_MAX_LEN+4];
}CRUISE_INFO_HEAD;

//假日排程头信息
typedef struct _ncfg_info_holiday_sche_head
{
	unsigned long holidaynum;//假日数量
	unsigned long offset;//所含的每个排程日相对第一个NCFG_INFO_HOLIDAY_SCHE_HEAD开头的偏移
}
NCFG_INFO_HOLIDAY_SCHE_HEAD;

typedef struct _ncfg_cvbs__{
	unsigned long last;
	unsigned long current;
}NCFG_CVBS_INFO;

inline unsigned char ViewSplitToIndex(NCFG_ITEM_ID viewSplitID)
{
	unsigned char index = 0;
	switch(viewSplitID)
	{
	case NCFG_ITEM_VIEW_SPLIT_1X1:
		index = 0;
		break;
	case NCFG_ITEM_VIEW_SPLIT_2X2:
		index = 1;
		break;
	case NCFG_ITEM_VIEW_SPLIT_2X3:
		index = 2;
		break;
	case NCFG_ITEM_VIEW_SPLIT_3X3:
		index = 3;
		break;
	case NCFG_ITEM_VIEW_SPLIT_4X4:
		index = 4;
		break;
	case NCFG_ITEM_VIEW_SPLIT_4X6:
		index = 5;
		break;
	case NCFG_ITEM_VIEW_SPLIT_5X5:
		index = 6;
		break;
	case NCFG_ITEM_VIEW_SPLIT_6X6:
		index = 7;
		break;
	default:
		index = 0;
		break;
	}

	return index;
}

#if defined(__ENVIRONMENT_WIN32__) || defined(__TDFH__) || defined(__TDNXP__) || defined(__CHIP3531__) || defined(__CHIP3535__) || defined(__CHIP3521__) || defined(__CHIP3520A__) || defined(__CHIPTI__) || defined(__CHIP3520D__) || defined(__CHIPGM__) 
#pragma pack()
#endif


#endif //_NET_CONFIG_DEFINE_H_


