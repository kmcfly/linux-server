#ifndef _NET_CONFIG_DEFINE_V3_H_
#define _NET_CONFIG_DEFINE_V3_H_

namespace IPC_V3
{

//一般情况下不可改变此值，具体是否要更改此值需要讨论后确定
const unsigned long		NCFG_VERSION					=	0x01;

#pragma pack(4)

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

typedef enum _config_page_index
{
	///////////系统配置
	PAGE_BASENUM_SYS_CONFIG = 0x1000,
	PAGE_SYS_CONFIG ,  ///////系统配置 主索引
	PAGE_SYS_BASIC_CONFIG , /////系统基本配置， 子索引
	PAGE_SYS_TIME_CONFIG ,  ////////系统时间配置
	PAGE_SYS_SDCARD_MANAGER_CONFIG,  ////SD卡管理、
	PAGE_SYS_DEFAULT_CONFIG , //////系统恢复缺省值的配置
	PAGE_SYS_DISK_MANAGER_CONFIG , ////磁盘管理
	PAGE_SYS_REBOOT , //////系统重起
	PAGE_END_SYS_CONFIG ,


	////////////通道配置
	PAGE_BASENUM_CAM_CONFIG = 0x2000,
	PAGE_CAM_CONFIG ,  //////通道配置主索引
	PAGE_CAM_BASIC_CONFIG ,	 ///////////通道基本配置
	PAGE_CAM_IMAGE_CONFIG ,//////////通道图像配置，色彩调节
	PAGE_CAM_PTZ_CONFIG , //////////通道云台配置
	PAGE_CAM_MOTION_CONFIG , //////////通道Motion区域配置
	PAGE_CAM_NET_IMAGE_CONFIG , //////////网络图像配置
	PAGE_CAM_RECORD_CONFIG ,//////////通道录像配置
	PAGE_CMA_RECORD_SCHEDULE_CONFIG, //////////录像排程配置
	PAGE_CAM_MAP_REMOTE_CHNN_CONFIG, //////////映射远程设备通道配置
	PAGE_CAM_SET_OSD_MASK,	//////////视频遮挡的配置
	PAGE_CAM_AUDIO_PARA,    //////////音频配置
	PAGE_CAM_VIDEO_ROI,     ////////// ROI配置
	PAGE_CAM_CONFIG_END,  


	/////////报警配置
	PAGE_BASENUM_ALARM_CONFIG = 0x3000,
	PAGE_ALARM_CONFIG,  /////////报警配置主索引
	PAGE_ALARM_BASIC_CONFIG,//////////报警基本配置
	PAGE_ALARM_MOTION_CONFIG, //////////Motion报警配置
	PAGE_ALARM_MOTION_SCHEDULE_CONFIG, //////////Motion排程配置
	PAGE_ALARM_SENSOR_CONFIG,  //////////Sensor报警配置
	PAGE_ALARM_SENSOR_SCHEDULE_CONFIG, //////////Sensor排程配置
	PAGE_ALARM_IN_OTHER_CONFIG, //////其他报警输入
	PAGE_ALARM_IN_OTHER_SCHEDULE_CONFIG, ////其他报警输入排程
	PAGE_ALARM_OUT_CONFIG, //////////报警输出配置
	PAGE_ALARM_OUT_SCHEDULE_CONFIG, //////////报警输出排程配置
	PAGE_ALARM_CONFIG_END,



	/////////////////网络配置
	PAGE_BASENUM_NET_CONFIG = 0x4000,
	PAGE_NET_CONFIG,  ///////////主索引
	PAGE_NET_BASIC_CONFIG, //////////网路基本配置
	PAGE_NET_IP_CONFIG, //////////IP配置
	PAGE_NET_WIRELESS_CONFIG,//////////无线配置
	PAGE_NET_DDNS_CONFIG,//////////DDNS配置
	PAGE_NET_IP_CHANGE_NOTIFY,//////////IP变化通知
	PAGE_NET_RTSP_CONFIG,//////////RTSP配置
	PAGE_NET_UPNP_CONFIG,//////////UPNP配置
	PAGE_NET_CHUANTOU,	//网络穿透
	PAGE_NET_ECMS_INFO,
	PAGE_NET_BONJOUR_CONFIG,
	PAGE_NET_CONFIG_END,


	/////////////////用户配置
	PAGE_BASENUM_USER_CONFIG = 0x5000,
	PAGE_USER_CONFIG, ////主索引
	PAGE_USER_CONFIG_END,


	///////////////////通知配置
	PAGE_BASENUM_NOTIFY_CONFIG = 0x6000,
	PAGE_NOTIFY_CONFIG, /////主索引
	PAGE_NOTIFY_FTP_CONFIG,//////////FTP配置
	PAGE_NOTIFY_MAIL_CONFIG, //////////Mail配置
	PAGE_NOTIFY_CONFIG_END,


	///////////////////////高级配置
	PAGE_BASENUM_ADVANCED_CONFIG = 0x7000,
	PAGE_ADVANCED_CONFIG, /////主索引
	PAGE_ADVANCED_SAFE_CONFIG, //////////安全配置
	PAGE_ADVANCED_UPGRADE_CONFIG, //////升级
	PAGE_SYS_IMPORT_EXPORT_CONFIG,///倒入导出配置
	PAGE_LOG_QUERY	,			//日志查询
	PAGE_ONVIF_CONFIG,			//ONVIF客户端类型
	PAGE_SPVMN_CONFIG,
	PAGE_ADVANCED_CONFIG_END,
	//////////
	PAGE_BASENUM_PTZ_CONFIG	=0x8000,
	PAGE_PTZ_CONFIG,//主索引
	PAGE_PTZ_PRESET_CONFIG,//预置点配置
	PAGE_PTZ_CRUISE_CONFIG,//巡航线配置
	PAGE_PTZ_UPDATE_CONFIG, ////云台升级
	PAGE_BASENUM_PTZ_CONFIG_END,
}CONFIG_PAGE_INDEX;  /////////索引

typedef enum cfg_item_id
{
	NCFG_ITEM_NULL=0x0000,		//用来替代处理一些函数中不处理的配置项

//此范围ID存储系统基本配置的杂项
	NCFG_ITEM_SYSTEM_BASE=0x100,		
	NCFG_ITEM_DEVICE_NAME,				//设备名称，NCFG_INFO_SHORT_NAME
	NCFG_ITEM_VIDEO_FORMAT,				//视频格式，unsigned char ,视频制式	
	NCFG_ITEM_DATE_FORMAT,				//日期格式，unsigned char
	NCFG_ITEM_TIME_SYNC,				//是否网络时间同步，unsigned char
	NCFG_ITEM_TIME_ZONE,				//时区，long
	NCFG_ITEM_TIME_SERVER,				//网络时间同步服务器地址，LONG_NAME
	NCFG_ITEM_SEND_MAIL_SERVER,			//MAIL设置包括服务器地址和接收MAIL地址列表,N_EMAIL_INFO
	NCFG_ITEM_FTP_SERVER,               //FTP参数，结构定义采用类似MAIL的方式
	NCFG_ITEM_USER,						//用户信息，USER [MAX_USER_NUM]
	NCFG_ITEM_FTP_UPLOAD_PIC,			//负数  FTP报警几秒上传一张图片   正数FTP 报警一秒上传几张图片  long
	NCFG_ITEM_LIVE_SUPPORT_PTZ,			//现场是否支持PTZ界面控制，unsigned char
	NCFG_ITEM_TIME_CITY,				//在哪个城市 int
	NCFG_ITEM_DAYLIGHT,					//是否是夏令时 unsigned char
	NCFG_ITEM_SHOW_DEV_NAME,//是否显示设备名称.1 显示;0 不显示  ,unsigned long
	NCFG_ITEM_DEV_NAME_POS,//设备名称显示位置  1 顶右 2 顶左 3 底右 4 底左,  unsigned long
	NCFG_ITEM_TIME_SYNC_EX, // NCFG_SYNCHRONIZE_INFO
	NCFG_ITEM_TIME_ZONE_EX, // NCFG_TIMEZONE_INFO
	NCFG_ITEM_DEV_NAME_EX,  // NCFG_INFO_DEV_NAME_EX
	NCFG_ITEM_SYSTEM_END,
	
//通道配置
	NCFG_ITEM_CHANNEL_BASE=0x200,
	NCFG_ITEM_PTZ_CONFIG,               //每个通道的云台的协议、地址、波特率 NCFG_INFO_PTZ_CONFIG
	NCFG_ITEM_CHANNEL_BASE_END,

	NCFG_ITEM_CHANNEL_COLOR_BASE=0x250, //通道色彩配置
	NCFG_ITEM_COLOR_BRIGHTNESS,          //亮度   unsigned char
	NCFG_ITEM_COLOR_CONTRAST,            //对比度  unsigned char
	NCFG_ITEM_COLOR_SATURATION,          //饱和度  unsigned char
	NCFG_ITEM_COLOR_HUE,                 //灰度    unsigned char
	NCFG_ITEM_IMAGE_MIRROR,				//图像镜像 unsigned char
	NCFG_ITEM_IMAGE_FILP,				//图像翻转 unsigned char
	NCFG_ITEM_WHITE_BALANCE,			//白平衡  NCFG_WHITE_BALANCE
	NCFG_ITEM_IMAGE_DYNAMIC_RANGE,		//宽动态 NCFG_DYNAMIC_RANGE
	NCFG_ITEM_DENOISE,                     //降噪
	NCFG_ITEM_IRISTYPE,                     //自动聚焦
	NCFG_ITEM_SHARPEN,                      //锐化
	NCFG_ITEM_PICTURE_BIND_CHN,           //图片绑定到哪个通道
	NCFG_ITEM_DAY_NIGHT_MODE,                  //IR-Cut 模式,unsigned long
	NCFG_ITEM_JPEG_LIVE_CHN,       // 图片流绑定到哪个通道,char,0 CIF,1 D1
	NCFG_ITEM_CHANNEL_COLOR_END,

	NCFG_ITEM_CHANNEL_COLOR_EX_BASE=0xf30,    //panjy e00-f30 约304个
	NCFG_ITEM_WHITE_BALANCE_9321L,         //panjy 8126 9321L白平衡
	NCFG_ITEM_WHITE_BALANCE_9314,          //panjiyong 8126 9314白平衡
	NCFG_ITEM_IMAGE_DYNAMIC_RANGE_9314,    //panjy 8126 9314宽动态
	NCFG_ITEM_DAY_NIGHT,                   //panjy 日夜模式
    // huxinhua add expose set
    NCFG_ITEM_EXPOSE_TYPE,  // expose type : auto  manual   unsigned long
    NCFG_ITEM_EXPOSE_VALUE, // unsigned long
    NCFG_ITEM_VIDEO_ENCODE_TYPE,    //   unsigned long [VIDEO_INPUT_NUM] NCFG_ENCODE_H264 NCFG_ENCODE_MJPEG
    // huxinhua add end
	// ysj
	NCFG_ITEM_EXPOSURE_CTRL, 			// 曝光控制
	NCFG_ITEM_FOCUS_CTRL,				// 聚焦控制
	NCFG_ITEM_DIS,						// 数字防抖 Digital image stabilization
	NCFG_ITEM_Shading,					// 暗角补偿
	NCFG_ITEM_GAMMA,					// gamma
	NCFG_ITEM_ANTI_FLICKER,				// 抗闪烁
	NCFG_ITEM_CRStrength,				// Crosstalk Removal 强度
	NCFG_ITEM_SlowFrameRate,			// 降帧率
	// ysj
	NCFG_ITEM_FOG_REDUCTION,			// 透雾 FOG_REDUCTION_CONFIG
	NCFG_ITEM_LENS_DISTORTION,			// 镜头畸变 LENS_DISTORTION_CONFIG
	NCFG_ITEM_BLC,                      // 背光补偿 unsigned char
	NCFG_ITEM_ANTI_OVEREXPOSURE,        // 防过曝 unsigned char
	NCFG_ITEM_DAY_NIGHT_SENSITIVITY,// unsigned char
	NCFG_ITEM_NIGHT_TO_DAY,         // unsigned long
	NCFG_ITEM_DAY_TO_NIGHT,         // unsigned long
	NCFG_ITEM_ROI,                  // ROI设置 
	NCFG_ITEM_VIDEO_GAIN,           // 增益 VIDEO_GAIN
	NCFG_ITEM_DIGITA_DENOISE,       // 数字降噪 DIGITA_DENOISE
	NCFG_ITEM_IMAGE_ROTATO,         // 图像旋转(走廊模式) IMAGE_ROTATO
	NCFG_ITEM_DAY_TO_NIGHT_SENSITIVITY,	// unsigned char
	NCFG_ITEM_NIGHT_TO_DAY_SENSITIVITY,	// unsigned char
	NCFG_ITEM_TIME_FOCUS_MODE,				//对焦模式 unsigned char
	NCFG_ITEM_IRCHANGE_FOCUS,			//日夜切换时对焦 unsigned char
	NCFG_ITEM_INTERVAL_TIME,			//对焦间隔时间 unsigned long
	NCFG_ITEM_CHANNEL_COLOR_EX_END,
	
	NCFG_ITEM_CHANNEL_TIME_STAMP_BASE=0x260,//通道时间戳
	NCFG_ITEM_TIME_STAMP_ENABLE,		//是否打时间戳, unsigned char[VIDEO_INPUT_NUM]
	NCFG_ITEM_TIME_STAMP_POS,			//时间戳位置, NCFG_INFO_TIME_POS[VIDEO_INPUT_NUM]	
	NCFG_ITEM_VIDEO_OUTPUT_FORMAT,  //视频输出制式
	NCFG_ITEM_GET_MAX_OSD_REGIN_COUNT,         //OSD最多可以显示几个区域
	NCFG_ITEM_SET_OSD_CONTENT,                  //每个区域的具体内容
	NCFG_ITEM_SET_OSD_MASK,                  // 设置视频遮挡的命令
	NCFG_ITEM_TIME_OSD_EX,  ///  NCFG_TIME_STAMP_CONFIG
	NCFG_ITEM_CHANNEL_TIME_STAMP_END,

	NCFG_ITEM_SCHEDULE_BASE=0x300,
	NCFG_ITEM_SENSOR_IN_SCH_WEEK,		//传感器侦测周计划， WEEK_SCHEDULE[SENSOR_INPUT_NUM]
	NCFG_ITEM_SENSOR_IN_SCH_HOLIDAY,	//传感器侦测假日计划HOLIDAY_SCHEDULE [MAX_HOLIDAY_NUM][SENSOR_INPUT_NUM]
	NCFG_ITEM_MOTION_SCH_WEEK,			//移动侦测周计划WEEK_SCHEDULE[VIDEO_INPUT_NUM]
	NCFG_ITEM_MOTION_SCH_HOLIDAY,		//移动侦测假日计划HOLIDAY_SCHEDULE [MAX_HOLIDAY_NUM][VIDEO_INPUT_NUM]
	NCFG_ITEM_SCHEDULE_END,

	NCFG_ITEM_AUDIO_BASE=0x350, 		// 音频参数
	NCFG_ITEM_AUDIO_PARA_CAPACITY,		// 用户获取设备音频的所支持项
	NCFG_ITEM_AUDIO_PARA,
	NCFG_ITEM_AUDIO_ENCODE,				// 音频编码，unsigned char
	NCFG_ITEM_AUDIO_IN_TYPE,			// 音频输入类型，unsigned char
	NCFG_ITEM_AUDIO_END, 				// 音频参数
	
//安全相关
	NCFG_ITEM_SECURITY_BASE=0x400,
	
	NCFG_ITEM_SECURITY_IPFILTER_BASE=0x410,//IP地址过滤	
	NCFG_ITEM_FILTER_IP_ENABLE,			//是否启动IP过滤, unsigned char
	NCFG_ITEM_FILTER_IP_MODE,			//过滤模式，拒绝需要过滤的地址接入或者只允许过滤的地址接入, unsigned char
	NCFG_ITEM_FILTER_IP_LIST,			//需要过滤的IP地址, NCFG_INFO_FILT_IP [MAX_FILTER_IP_NUM]
	NCFG_ITEM_SECURITY_IPFILTER_END,

	NCFG_ITEM_SECURITY_MACFILTER_BASE=0x420,//MAC地址过滤
	NCFG_ITEM_FILTER_MAC_ENABLE,		//是否启动MAC过滤, unsigned char
	NCFG_ITEM_FILTER_MAC_MODE,			//过滤模式，拒绝需要过滤的地址接入或者只允许过滤的地址接入, unsigned char
	NCFG_ITEM_FILTER_MAC_LIST,			//需要过滤的MAC地址, MAC [MAX_FILTER_MAC_NUM]
	NCFG_ITEM_SECURITY_MACFILTER_END,

	NCFG_ITEM_SECURITY_END,

//网络功能
	NCFG_ITEM_NET_BASE=0x500,
	NCFG_ITEM_UPNP_ENABLE,				//是否启动UPNP功能, unsigned char
	NCFG_ITEM_UPNP_FRIENDLY_NAME,		//UPNP设备显示的名字, LONG_NAME
	NCFG_ITEM_HTTP_PORT,				//HTTP 端口, unsigned short
	NCFG_ITEM_SERVER_PORT,				//网络服务端口， unsigned short	
	NCFG_ITEM_DNS_1,					//ULONGLONG
	NCFG_ITEM_DNS_2,					//ULONGLONG
	NCFG_ITEM_RTSP_PORT,				//unsigned short
	NCFG_ITEM_RTSP_ENABLE,				//unsigned char,是否启用RTSP服务
	NCFG_ITEM_RTSP_ENABLE_ANON,			//unsigned char,RTSP是否能够匿名登录
	NCFG_ITEM_RTSP_STREAM_NAME,			//SHORT_NAME,RTSP地址后面的标记
	NCFG_ITEM_RTSP_ENABLE_AUDIO,		//是否要开启声音
	NCFG_ITEM_UPNP_ENABLE_MAP,			//是否启动UPNP功能, unsigned char
	NCFG_ITEM_CHUANTOU_ALIAS,			//别名只能在这里获取，不能通过配置来配置
	NCFG_ITEM_BONJOUR_ENABLE,			//是否启动Bonjour功能, unsigned char
	NCFG_ITEM_BONJOUR_FRIENDLY_NAME,		//Bonjour设备显示的名字, LONG_NAME
	NCFG_ITEM_NET_BASE_END,

	NCFG_ITEM_DDNS_BASE=0x540,			//DDNS,可能后面还会加几个参数
	NCFG_ITEM_DDNS_ENABLE,              //DDNS是否开启, unsigned char
	NCFG_ITEM_DDNS_TYPE,				//DDNS类型，unsigned char
	NCFG_ITEM_DDNS_USER_NAME,			//DDNS用户名，SHORT_NAME
	NCFG_ITEM_DDNS_PASSWORD,			//DDNS密码，SHOR T_NAME
	NCFG_ITEM_DDNS_SERVER_NAME,			//DDNS服务器信息，NCFG_INFO_LONG_NAME
	NCFG_ITEM_DDNS_DOMAIN_INFO,			//DDNS域名信息，NCFG_INFO_LONG_NAME
	NCFG_ITEM_DDNS_END,

	NCFG_ITEM_NET_WIRELESS_BASE=0x550,  //无线网络相关，如果只定义成一个ID，放到前面就可以了
	NCFG_ITEM_WIRELESS_ENABLE,			//启用无线 unsigned char
	NCFG_ITEM_WIRELESS_IPMODE,			//获得IP的模式,unsigned char
	NCFG_ITEM_WIRELESS_IP,              //无线IP
	NCFG_ITEM_WIRELESS_SUB_NET_MASK,    //无线子网掩码
	NCFG_ITEM_WIRELESS_GATEWAY,         //无线网关
	NCFG_ITEM_WIRELESS_DNS,             //无线DNS
	NCFG_ITEM_WIRELESS_SSID,            //无线SSID
	NCFG_ITEM_WIRELESS_AUTHENTICATION_MODE,//无线认证模式
	NCFG_ITEM_WIRELESS_ENCRYPT_TYPE,    //加密类型
	NCFG_ITEM_WIRELESS_CHANNEL,			//信道	unsigned char 1--11
	NCFG_ITEM_WIRELESS_WHICH_KEY,		// 使用哪个密钥 unsigned char 1 2 3 4	
	NCFG_ITEM_WIRELESS_KEY1,            //密钥1
	NCFG_ITEM_WIRELESS_KEY2,            //密钥2
	NCFG_ITEM_WIRELESS_KEY3,            //密钥3
	NCFG_ITEM_WIRELESS_KEY4,            //密钥4
	NCFG_ITEM_WIRELESS_DNS2,            //无线DNS
	NCFG_ITEM_NET_WIRELESS_END,
	
	NCFG_ITEM_NET_IPNOTIFY_BASE=0x570,  //各种IP通知方式
	NCFG_ITEM_IP_CHG_TO_EMAIL_ENABLE,	//IP改变触发邮件(端口变化一样处理)，unsigned char
	NCFG_ITEM_IP_CHG_TO_EMAIL_ADDR,		//IP改变触发MAIL地址列表，unsigned char [n],跟多个EMAIL的ID
	NCFG_ITEM_IP_CHG_TO_FTP_ENABLE,     //IP改变触发FTP上传，unsigned char
	NCFG_ITEM_IP_CHG_TO_FTP_ADDR,		//IP改变触发FTP地址列表，unsigned char [n],跟多个FTP的ID
	NCFG_ITEM_IP_CHG_TO_EMAIL_ADDR_V2,
	NCFG_ITEM_NET_IPNOTIFY_END,

	NCFG_ITEM_OLD_BASE=0x5F0,
	NCFG_ITEM_GET_IP_MODE,				//获取IP模式，unsigned char
	NCFG_ITEM_STATIC_IP,				//静态IP地址，ULONGLONG（考虑64IP）
	NCFG_ITEM_GATEWAY,					//网关，ULONGLONG
	NCFG_ITEM_SUB_NET_MASK,				//子网掩码，ULONGLONG
	NCFG_ITEM_PPPOE_ENABLE,				//PPPoE是否开启拨号, unsigned char
	NCFG_ITEM_PPPOE_USER_NAME,			//PPPoE用户名,	LONG_NAME
	NCFG_ITEM_PPPOE_PASSWORD,			//PPPoE密码,	SHORT_NAME
	NCFG_ITEM_START_AUTOSEARCH_ROUTE,	//自动搜索路由器功能 unsigned char 0 关闭，非0 开启
	NCFG_ITEM_ENABLE_CHUANTOU,
	NCFG_ITEM_GET_DNS_MODE,
	NCFG_ITEM_SET_NIC_MODE,             // 设置网卡工作模式
	NCFG_ITEM_OLD_END,

	NCFG_ITEM_NET_END,
//报警联动功能
	NCFG_ITEM_ALARM_TRIGER_BASE=0x600,
	NCFG_ITEM_ALARM_OUT_NAME,			//继电器输出名称，SHORT_NAME[SENSOR_OUTPUT_NUM]
	NCFG_ITEM_ALARM_OUT_HOLD_TIME,		//继电器输出保持时间，unsigned long [SENSOR_OUTPUT_NUM]
	NCFG_ITEM_ALARM_TRIGER_BASE_END,

	//传感器
	NCFG_ITEM_ALARM_TRIGER_SENSORBASE=0x620,
	NCFG_ITEM_SENSOR_IN_ENABLE,			//传感器开关，unsigned char [SENSOR_INPUT_NUM]
	NCFG_ITEM_SENSOR_IN_TYPE,			//传感器设备类型，unsigned char [SENSOR_INPUT_NUM]
	NCFG_ITEM_SENSOR_IN_NAME,			//传感器名称，SHORT_NAME [SENSOR_INPUT_NUM]

	NCFG_ITEM_SENSOR_IN_TO_ALARM_OUT,	//传感器触发报警输出，unsigned long [SENSOR_INPUT_NUM];
	NCFG_ITEM_SENSOR_IN_TO_EMAIL_ENABLE,//传感器能够触发EMAIL，unsigned char
	NCFG_ITEM_SENSOR_IN_TO_FTP_ENABLE,	//传感器触发触发FTP，unsigned char
	NCFG_ITEM_SENSOR_IN_TO_FTP_ADDR,	//传感器触发FTP地址列表，和MAIL处理方式一样unsigned long[n] 
	NCFG_ITEM_SENSOR_IN_TO_PTZ,			//传感器触发云台预制点，TO_PTZ [VIDEO_INPUT_NUM][SENSOR_INPUT_NUM]
	NCFG_ITEM_SENSOR_IN_TO_SNAP,		//传感器触发保存SD卡图片 unsigned long[SENSORNUM]
	NCFG_ITEM_SENSOR_TO_FTP_SNAP,		//传感器触发FTP抓图，unsigned long[SENSORNUM]
	NCFG_ITEM_SENSOR_TO_EMAIL_SNAP,		//传感器触发EMAIL抓图，unsigned long[SENSORNUM]
	NCFG_ITEM_SENSOR_IN_TO_EMAIL_SUBJECT, //传感器触发邮件报警的邮件主题char[SMTP_SUBJECT_MAX_LEN]
	NCFG_ITEM_SENSOR_IN_TO_EMAIL_CONTENT, //传感器触发邮件报警的邮件内容char[SMTP_CONTENT_MAX_LEN]
	NCFG_ITEM_SENSOR_TO_EMAIL_ADDR,
	NCFG_ITEM_SENSOR_IPHONE_PUSH_SWITCH, //传感器触发IPHONE PUSH 开关
	NCFG_ITEM_SENSOR_IPHONE_PUSH_CONTENT,//内容
	NCFG_ITEM_SENSOR_HOLD_TIME,			// unsigned long
	NCFG_ITEM_SENSOR_IN_TO_SDRECORD_ENABLE,//传感器触发SD卡录像
	NCFG_ITEM_ALARM_TRIGER_SENSOREND,

	//移动侦测
	NCFG_ITEM_ALARM_TRIGER_MOTIONBASE=0x640,	
	NCFG_ITEM_MOTION_ENABLE,			//移动侦测开关，unsigned char [VIDEO_INPUT_NUM]
	NCFG_ITEM_MOTION_AREA_INFO,         //区域的信息，NCFG_INFO_MOTION_INFO[VIDEO_INPUT_NUM]
	NCFG_ITEM_MOTION_HOLD_TIME_ALLCH,	//移动侦测保持时间,unsigned long
	NCFG_ITEM_MOTION_TO_ALARM_OUT,		//移动侦测触发报警输出，unsigned long
	NCFG_ITEM_MOTION_TO_RECORD,			//移动侦测触发录像，ULONGLONG [VIDEO_INPUT_NUM]
	NCFG_ITEM_MOTION_TO_EMAIL_ENABLE,	//移动侦测能够触发EMAIL，unsigned char
	NCFG_ITEM_MOTION_TO_PTZ,			//移动侦测触发云台，TO_PTZ [VIDEO_INPUT_NUM][VIDEO_INPUT_NUM]
	NCFG_ITEM_MOTION_TO_FTP_ENABLE,		//移动侦测触发FTP，unsigned char
	NCFG_ITEM_MOTION_TO_FTP_ADDR,		//移动侦测触发FTP地址列表，和MAIL处理方式一样unsigned long[n] 
	NCFG_ITEM_MOTION_TO_BUZZ,           //移动侦测触发BUZZ unsigned char[VIDEO_INPUT_NUM]
	NCFG_ITEM_MOTION_TO_SNAP,           //移动侦测触发抓拍 unsigned long
	NCFG_ITEM_MOTION_TO_FTP_SNAP,		//移动侦测触发FTP抓图，unsigned long
	NCFG_ITEM_MOTION_TO_EMAIL_SNAP,		//移动侦测触发EMAIL抓图，unsigned long
	NCFG_ITEM_MOTION_TO_MAIL_SUBJECT,	//移动侦测触发邮件报警的主题 char[SMTP_SUBJECT_MAX_LEN]
	NCFG_ITEM_MOTION_TO_MAIL_CONTENT,	//移动侦测触发邮件报警的内容 char[SMTP_CONTENT_MAX_LEN]
	NCFG_ITEM_MOTION_TO_EMAIL_ADDR,
	NCFG_ITEM_MOTION_IPHONE_PUSH_SWITCH, //移动侦测触发IPHONE PUSH
	NCFG_ITEM_MOTION_IPHONE_PUSH_CONTENT, //内容
	NCFG_ITEM_MOTION_HOLD_TIME,				// unsigned long
	NCFG_ITEM_MOTION_TO_SDRECORD_ENABLE,	//移动侦测触发SD卡录像
	NCFG_ITEM_ALARM_TRIGER_MOTIONEND,
	
	NCFG_ITEM_ALARM_TRIGER_SHELTERBASE=0x670,
	NCFG_ITEM_SHELTER_TO_ALARM_OUT,		//遮挡触发报警输出，ULONGLONG [VIDEO_INPUT_NUM]
	NCFG_ITEM_SHELTER_TO_RECORD,		//遮挡触发录像，ULONGLONG [VIDEO_INPUT_NUM]
	NCFG_ITEM_SHELTER_TO_EMAIL_ENABLE,	//遮挡能够触发EMAIL，unsigned char
	NCFG_ITEM_SHELTER_TO_EMAIL_ADDR,	//遮挡触发MAIL地址列表，unsigned long[n]
	NCFG_ITEM_SHELTER_TO_FTP_ENABLE,	//遮挡触发FTP，unsigned char
	NCFG_ITEM_SHELTER_TO_FTP_ADDR,		//遮挡触发FTP地址列表，和MAIL处理方式一样unsigned long[n] 
	NCFG_ITEM_SHELTER_TO_BUZZ,          //遮挡触发BUZZ unsigned char[VIDEO_INPUT_NUM]
	NCFG_ITEM_SHELTER_TO_PTZ,			//遮挡触发云台，TO_PTZ [VIDEO_INPUT_NUM][VIDEO_INPUT_NUM]
	NCFG_ITEM_ALARM_TRIGER_SHELTEREND,

	NCFG_ITEM_ALARM_TRIGER_END,


//编码参数，在不支持NCFG_ITEM_ENCODE_ENCODE_TYPE时默认是支持VBR模式
	NCFG_ITEM_ENCODE_BASE=0x700,		//目前要废除的方式放到此段ID的最后
	
	NCFG_ITEM_ENCODE_RESOLUTION_ALLCH,//所有通道一个分辨时使用此值
	NCFG_ITEM_ENCODE_RESOLUTION,		//网络主流分辨率，unsigned char [VIDEO_INPUT_NUM]，每个通道不同
	NCFG_ITEM_ENCODE_VIDEO_QUALITY,		//网络主流画质, unsigned long [VIDEO_INPUT_NUM],VBR指等级(数字越大画质越好)CBR指码流
	NCFG_ITEM_ENCODE_FRAME_RATE,		//网络主流帧率，unsigned char [VIDEO_INPUT_NUM]
	NCFG_ITEM_ENCODE_ENCODE_TYPE,		//网络主流码率类型unsigned char[VIDEO_INPUT_NUM]
	NCFG_ITEM_ENCODE_I_FRAME_INTERVAL,	// unsigned long
	NCFG_ITEM_ENCODE_ENCODE_LEVEL,		// unsigned long   NCFG_ENCODE_LEVEL
    // 以下5条给NVR使用的。
    NCFG_ITEM_ENCODE_RESOLUTION_V2,         // chn(0,1,2,3;4 byte) + 分辨率(4 byte)
    NCFG_ITEM_ENCODE_VIDEO_QUALITY_V2,     // chn(0,1,2,3;4 byte) + 画质(4 byte)
    NCFG_ITEM_ENCODE_FRAME_RATE_V2,        // chn(0,1,2,3;4 byte) + 帧率(4 byte)
    NCFG_ITEM_ENCODE_ENCODE_TYPE_V2,       // chn(0,1,2,3;4 byte) + 编码类型(4 byte)
    NCFG_ITEM_ENCODE_I_FRAME_INTERVAL_V2,// chn(0,1,2,3;4 byte) + I帧间隔(4 byte)
    
    // for api
    NCFG_ITEM_ENCODE_LEVEL_EX,   // 码率上限 unsigned long [VIDEO_INPUT_NUM]
    NCFG_ITEM_ENCODE_SLICE_EX,   // 是否分片 unsigned long
	NCFG_ITEM_ENCODE_END,		
	
//云台参数配置，巡航线，预置点等，通道概念加到了ID里面端口和协议放到了NCFG_ITEM_PTZ_CONFIG里面
	NCFG_ITEM_PTZ_INFO_BASE=0x800,
	NCFG_ITEM_SPEEDDOME_PTZ, //用于区分普通ptz和高速球ptz的配置
	NCFG_ITEM_PTZ_INFO_PRESET_INFO_CHBASE=0x810,//预置点名字通道基数，保留64个通道ID的位置，PTZ_PRESET_INFO
	NCFG_ITEM_PTZ_INFO_PRESET_INFO_CHEND =NCFG_ITEM_PTZ_INFO_PRESET_INFO_CHBASE+64,

	NCFG_ITEM_PTZ_INFO_CRUISE_INFO_CHBASE=NCFG_ITEM_PTZ_INFO_PRESET_INFO_CHEND,//巡航线名字通道基数，保留64个通道ID的位置
	NCFG_ITEM_PTZ_INFO_CRUISE_INFO_CHEND =NCFG_ITEM_PTZ_INFO_CRUISE_INFO_CHBASE+64,//PTZ_CRUISE_INFO_HEAD
	NCFG_ITEM_PTZ_INFO_END=0x8FF,
	
//国标相关配置
	NCFG_ITEM_SPVMN_BASE=0x900,	
	NCFG_ITEM_SPVMN_ENABLE,				//国标开关，unsigned char
	NCFG_ITEM_SPVMN_SERVERIP,			//sip服务器ip地址，unsigned long
	NCFG_ITEM_SPVMN_SERVERSIPPORT,		//sip服务器会话端口 unsigned short
	NCFG_ITEM_SPVMN_IPCSIPPORT,			//IPC会话端口	unsigned short	
	NCFG_ITEM_SPVMN_REGPASSWD,			//注册密码		char[36]
	NCFG_ITEM_SPVMN_SERVERDOMAIN,		//sip服务器域名	char[36]
	NCFG_ITEM_SPVMN_SERVERID,			//sip服务器ID	char[36]
	NCFG_ITEM_SPVMN_IPCID,				//板子ID		char[36]
	NCFG_ITEM_SPVMN_OWNER,				//设备归属		char[36]								
	NCFG_ITEM_SPVMN_CIVILCODE,			//行政区域		char[36]
	NCFG_ITEM_SPVMN_ADDRESS,			//安装地址		char[36]
	NCFG_ITEM_SPVMN_END,

//解码器参数
	NCFG_ITEM_DECODE_BASE=0xA00,
	NCFG_ITEM_OSD_LOCALIP,				//界面是否显示当地IP, unsigned char
	NCFG_ITEM_CUR_QUADMODE,				//目前分割模式,NCFG_INFO_QUADMODE
	NCFG_ITEM_MAP_REMOTE_CHNN,			//映射远程设备通道
	NCFG_ITEM_DECODE_END,
//IPCamera设备配置项
	NCFG_ITEM_IPCAMERA_BASE=0xB00,
	NCFG_ITEM_NIGHT_VISION,				//夜视功能开关	unsigned char,NCFG_ENUM_NIGHT_VISION 
	NCFG_ITEM_FREQUENCY_HZ,				//镜头光源频率50/60赫兹/户外模式	,unsinged char NCFG_ENUM_CAMERA_FREQ		
	NCFG_ITEM_IPCAMERA_END,

//SD卡录像配置项
	NCFG_ITEM_SDRECORD_BASE=0xC00,
	NCFG_ITEM_SDRECORD_STREAMINDEX,
	NCFG_ITEM_SDRECORD_NEEDAUDIO,
	NCFG_ITEM_SDRECORD_END,
	
	NCFG_ITEM_ECMS_INFO_BASE=0xD00,
	NCFG_ITEM_ECMS_INFO_SWITCH,			//是否要IPCamera主动连接ECMS，unsigned char
	NCFG_ITEM_ECMS_INFO_ADDR,			// char[ECMS_ADDR_MAX_LEN],包括结束符
	NCFG_ITEM_ECMS_INFO_PORT,			// unsigned short
	NCFG_ITEM_ECMS_INFO_ID,				//ID unsigned long
	NCFG_ITEM_ECMS_INFO_END,
	
///只读信息，包括名字最大长度限制，支持选项的范围,版本信息等
	NCFG_ITEM_ONLYREAD_BASE=0xE00,
	NCFG_ITEM_DEVICE_NAME_MAX_LEN,      //设备名称长度限定, unsigned char,[只读]
	NCFG_ITEM_TIME_SERVER_MAX_LEN,      //网络时间同步服务器地址最大长度, unsigned char,[只读]
	NCFG_ITEM_UPNP_FRIENDLY_NAME_MAX_LEN,//UPNP设备显示的名字最大长度, unsigned char,[只读]
	NCFG_ITEM_CAMERA_NAME_MAX_LEN,      //通道名称最大长度, unsigned char,[只读]
	NCFG_ITEM_SENSOR_IN_NAME_MAX_LEN,   //传感器名称最大长度,unsigned char,[只读]
	NCFG_ITEM_ALARM_OUT_NAME_MAX_LEN,   //继电器输出名称最大长度, unsigned char,[只读]
	NCFG_ITEM_PPPOE_USER_NAME_MAX_LEN,  //PPPoE用户名最大长度，unsigned char,[只读]
	NCFG_ITEM_PPPOE_PASSWORD_MAX_LEN,   //PPPoE密码最大长度, unsigned char,[只读]
	NCFG_ITEM_DDNS_USER_NAME_MAX_LEN,   //DDNS用户名最大长度, unsigned char,[只读]
	NCFG_ITEM_DDNS_PASSWORD_MAX_LEN,    //DDNS密码最大长度, unsigned char,[只读]
	NCFG_ITEM_DDNS_DOMAIN_MAX_LEN,		//DDNS域名名称最大长度, unsigned char,[只读]
	NCFG_ITEM_WIRELESS_SSID_MAX_LEN,    //无线SSID最大长度, unsigned char,[只读]
	NCFG_ITEM_WIRELESS_KEY_MAX_LEN,     //密钥最大长度, unsigned char,[只读]
	NCFG_ITEM_SOFTWARE_VER,				//软件版本，unsigned long,[只读]
	NCFG_ITEM_SOFTWARE_BUILD_DATE,		//软件Build日期，unsigned long,[只读]
	NCFG_ITEM_KERNEL_VER,				//内核版本，unsigned long,[只读]
	NCFG_ITEM_HARD_VER,					//硬件版本，unsigned long,[只读]
	NCFG_ITEM_DISK_INFO,				//磁盘或SD卡信息，NCFG_INFO_DISK_INFO[n],[只读]
	NCFG_ITEM_TOTAL_DISK_INFO,			//DVR整个磁盘信息，磁盘使用百分比，BYTE,[只读]
	NCFG_ITEM_MAC_ADDRESS ,				//设备MAC地址，只读属性,[只读]
	NCFG_ITEM_MAX_CLIENT_COUNT,         //连接的最大客户端数量  unsigned char,[只读]
	NCFG_ITEM_PRESET_MAXNUM,            //支持的预置点的最大数量 unsigned char,[只读]
	NCFG_ITEM_CRUISE_MAXNUM,            //支持的巡航线的最大数量 unsigned char,[只读]
	NCFG_ITEM_SUPPORT_PTZRETURN,		//支持报警结束触发预知点功能,unsigned char,[只读]
	NCFG_ITEM_TIME_STAMP_SUPPORT_POS,	//时间戳支持的位置方式，unsigned long 三种时间戳都使用此ID(0x01 << pos的MASK),[只读]
	NCFG_ITEM_SUPPORT_VIDEOFORMAT,		//支持的视频制式,unsigned char,[只读]
	NCFG_ITEM_SUPPORT_QUADMODE,			//支持的分割模式[只读],第几位为1就代表支持哪一个分割模式，unsigned long,[只读]
	NCFG_ITEM_SUPPORT_BITRATE_RANGE,	//CBR模式下支持的码流范围，NCFG_INFO_BITRATE_RANG,[只读]
	NCFG_ITEM_SUPPORT_VBR_BITRATE,		//VBR模式下每种档次的画质目标码流，NCFG_INFO_CBRBITRATE[n],[只读]
	NCFG_ITEM_SUPPORT_VBR_LEVELNUM,		//VBR支持几个档次的画质，客户端支持5种档次，其它档次以1-n的数字取代下拉,unsigned char,[只读]
	NCFG_ITEM_SUPPORT_ENCODEMODE,		//支持的编码模式，是个VIDEO_ENCODE_MODE的MASK，unsigned char,[只读]
	NCFG_ITEM_USER_MAXNUM,				//支持用户最大数量，unsigned char[只读]
	NCFG_ITEM_AUDIO_INPUT_NUM,			//声音输入通道数量，unsigned char[只读]
	NCFG_ITEM_HOLIDAY_MAX_NUM,			//设置支持的假日排程日期的最大数量，特别是非循环方式时可能需增加，unsigned char
	NCFG_ITEM_DEVUSER_NAME_MAX_LEN,		//设备用户名最大长度，unsigned char[只读]
	NCFG_ITEM_DEVUSER_PWD_MAX_LEN,		//设备用户密码最大长度，unsigned char[只读]
	NCFG_ITEM_DEVTYPE_INFO,				//设备类型，DEVICE_TYPE_DEF::NETDEVICE_TYPE_INFO，[只读]
	NCFG_ITEM_PLAYBACK_MAX_NUM,			//同时回放的通道数目，unsigned char[只读]
	NCFG_ITEM_DEFAULT_COLOR_BRIGHTNESS, //缺省的亮度值,unsigned char[只读]
	NCFG_ITEM_DEFAULT_COLOR_CONTRAST,   //缺省的对比度值,unsigned char[只读]
	NCFG_ITEM_DEFAULT_COLOR_SATURATION, //缺省的饱和度值,unsigned char[只读]
	NCFG_ITEM_DEFAULT_COLOR_HUE,		//缺省的灰度值,unsigned char[只读]
	NCFG_ITEM_VGA_SUPPORT_RESOLUTION,	//VGA芯片支持的分辨率MASK，unsigned long，参考dvrdvsdef.h中的VGA_RESOLUTION枚举[只读]
	NCFG_ITEM_VGA_SUPPORT_REFRESH,		//VGA支持的屏幕刷新率，每种分辨率下都有所不同[只读]
	NCFG_ITEM_FILTER_IP_MAXNUM,			//支持IP过滤的最大IP个数unsigned char[只读]
	NCFG_ITEM_FILTER_MAC_MAXNUM,		//支持MAC过滤的最大MAC个数unsigned char[只读]
	NCFG_ITEM_DDNS_SERVER_INFO,			//DDNS下拉列表支持的DDNS类型[只读],DDNS_SERVER_INFO	
	NCFG_ITEM_LIVE_TIME_SUPPORT_POS,	//现场时间支持的位置方式unsigned char[只读]
	NCFG_ITEM_CRUISE_PRESET_MAXNUM,		//每个巡航线的最大预置点数量[只读]
	NCFG_ITEM_VIDEO_INPUT_NUM,			//视频输入通道数量unsigned char[只读]
	NCFG_ITEM_SENSOR_IN_NUM,			//传感器报警输入数量unsigned char[只读]
	NCFG_ITEM_ALARM_OUT_NUM,			//物理报警输出数量unsigned char[只读]
	NCFG_ITEM_NOTIFY_FTP_NUM,			//FTP通知服务器的最大数量unsigned char[只读]
	NCFG_EMAIL_SUBJECT_MAX_LEN,			//EMAIL主题最大长度unsigned long[只读]
	NCFG_EMAIL_CONTENT_MAX_LEN,			//EMAIL内容最大长度unsigned long[只读]
	NCFG_FTP_SNAP_PIC_RAMES_MAX,		//FTP上传图片支持的每秒多少帧取值枚举unsigned long[只读]，按位处理
	NCFG_RTSP_STREAM_NAME_MAX_LEN,		//RTSP匿名地址最大长度
	NCFG_PTZ_PRESET_NAME_MAX_LEN,		//PTZ预置点名字最大长度
	NCFG_PTZ_CRUISE_NAME_MAX_LEN,		//PTZ巡航线名字最大长度
	NCFG_PTZ_PROTOCOL_INFO,				//云台支持的协议PTZ_PROTOCOL_INFO
	NCFG_PTZ_SUPPORT_BAUDRATE,			//云台支持的波特率PTZ_BAUDRATE_INFO
	NCFG_ITEM_ENCODE_SUPPORT_PROPERTY,//表示支持的码流有哪些	
	NCFG_ITEM_DEFAULT_IMAGE_MIRROR,			//缺省的镜像图像,unsigned char[只读]
	NCFG_ITEM_DEFAULT_IMAGE_FLIP,			//缺省的翻转图像,unsigned char[只读]
	NCFG_ITEM_LIGHT_HZ_SUPPORT,			//支持的灯光模式
	NCFG_ITEM_IPCAMERA_NO,              //获取IPC序列号 panjy	
	NCFG_ITEM_MAX_BRIGHT_VALUE,			//允许设置的亮度最大值
	NCFG_ITEM_MAX_CONTRACT_VALUE,		//允许设置的对比度最大值
	NCFG_ITEM_MAX_SATURATION_VALUE,		//允许设置的饱和度最大值
	NCFG_ITEM_MAX_HUE_VALUE,			//允许设置的色度最大值

	NCFG_ITEM_MAX_WHITE_BALANCE_VALUE,	//允许设置的白平衡的最大值 NCFG_MAX_WHITE_BALANCE
	NCFG_ITEM_MAX_DYNAMIC_RANGE,		//允许设置的宽动态的值 unsigned char

//	NCFG_ITEM_MAX_WHITE_BALANCE_VALUE,	//允许设置的白平衡最大值
	NCFG_ITEM_DEFAULT_WHITE_BALANCE, //缺省的白平衡值,NCFG_WHITE_BALANCE[只读]
	NCFG_ITEM_DEFAULT_DYNAMIC_RANGE, //缺省的宽动态,NCFG_DYNAMIC_RANGE[只读]

	NCFG_ITEM_TRAVER_ONLINE,		//穿透是否在线
	NCFG_ITEM_DEFAULT_DENOISE,  //降噪默认值
	NCFG_ITEM_MAX_DENOISE,      //降噪最大值
	NCFG_ITEM_DEFAULT_IRISTYPE,//自动光圈默认值
	NCFG_ITEM_MAX_IRISTYPE,     //自动光圈最大值
	NCFG_ITEM_DEFAULT_SHARPEN,  //锐化默认值
	NCFG_ITEM_MAX_SHARPEN,      //锐化最大值

	NCFG_ITEM_FTP_USR_MAX_LEN,
	NCFG_ITEM_FTP_PWD_MAX_LEN,

	NCFG_ITEM_CFG_IMAGE_STREAM_INDEX, //配置图像时，请求哪一路码流 unsigned char

	NCFG_ITEM_DEFAULT_VIDEO_OUTPUT_FORMAT,  //视频输出默认值
	NCFG_ITEM_MAX_VIDEO_OUTPUT_FORMAT,      //视频输出最大值

	NCFG_ITEM_IPHONE_PUSH_CONTENT_MAX,

    NCFG_ITEM_MAX_ENCODE_I_FRAME_INTERVAL,//I帧间隔最大值
    NCFG_ITEM_MIN_ENCODE_I_FRAME_INTERVAL,//I帧间隔最小值
    NCFG_ITEM_DEFAULT_ENCODE_I_FRAME_INTERVAL,//I 帧间隔默认值

    NCFG_ITEM_DAY_NIGHT_MODE_DEFAULT,//IR-CUT默认值

	NCFG_ITEM_PTZ_EXIST_DEFAULT,	//云台功能是否存在
	NCFG_ITEM_ONVIF_CLIENT_INFO,
    // expose
    NCFG_ITEM_EXPOSE_TYPE_DEFAULT,
    NCFG_ITEM_EXPOSE_VALUE_MAX,
    NCFG_ITEM_EXPOSE_VALUE_DAFAULT,
    // gain
    //NCFG_ITEM_GAIN_VALUE_MAX,
    //NCFG_ITEM_GAIN_VALUE_DEFAULE,
    NCFG_ITEM_SUPPORT_VIDEO_ENCODE_TYPE, // unsigned long [VIDEO_INPUT_NUM] NCFG_ENCODE_H264 NCFG_ENCODE_MJPEG
    NCFG_ITEM_SUPPORT_ENCODE_LEVEL,      // unsigned long [VIDEO_INPUT_NUM] NCFG_ENCODE_LEVEL_BASELINE, NCFG_ENCODE_LEVEL_MAINPROFILE
	NCFG_ITEM_DEFAULT_FOG_REDUCTION,	// FOG_REDUCTION_CONFIG
	NCFG_ITEM_DEFAULT_LENS_DISTORTION,	// LENS_DISTORTION_CONFIG
	NCFG_ITEM_DEFAULT_BLC,	                // unsigned char
	NCFG_ITEM_DEFAULT_ANTI_OVEREXPOSURE,    // unsigned char
	NCFG_ITEM_DEFAULT_DAY_NIGHT_SENSITIVITY,// unsigned char
	NCFG_ITEM_DEFAULT_NIGHT_TO_DAY,         // unsigned long
	NCFG_ITEM_DEFAULT_DAY_TO_NIGHT,         // unsigned long
	NCFG_ITEM_DEFAULT_VIDEO_GAIN,           // 增益 VIDEO_GAIN
	NCFG_ITEM_DEFAULT_DIGITA_DENOISE,       // 数字降噪 DIGITA_DENOISE
	NCFG_ITEM_DEFAULT_IMAGE_ROTATO,         // 图像旋转(走廊模式) IMAGE_ROTATO
	NCFG_ITEM_DEFAULT_DAY_TO_NIGHT_SENSITIVITY,// unsigned char
	NCFG_ITEM_DEFAULT_NIGHT_TO_DAY_SENSITIVITY,// unsigned char
	NCFG_ITEM_ONLYREAD_END,	

	NCFG_ITEM_ONVIF_BASE=0xF00,
	NCFG_ONVIF_CLIENT_TYPE,
	NCFG_ONVIF_VERSION,	// unsigned long
	NCFG_ITEM_ONVIF_END,

///其它ID，既不是可以正常读取和保存的ID，也不是只读ID
	NCFG_ITEM_OTHER_BASE=0xFF0,
	NCFG_ITEM_MANUAL_SET_TIME_VALUE,	//设置时间
	NCFG_ITEM_BONJOUR_FRIENDLY_NAME_MAX_LEN,//BONJOUR设备显示的名字最大长度, unsigned char,[只读]
	NCFG_ITEM_OTHER_END,
}NCFG_ITEM_ID;


enum NCFG_ENCODE_LEVEL
{
	NCFG_ENCODE_LEVEL_BASELINE,
	NCFG_ENCODE_LEVEL_MAINPROFILE,
	NCFG_ENCODE_LEVEL_HIGHPROFILE
};

//VGA每种分辨率下支持的屏幕刷新率
typedef struct _ncfg_info_vga_support_refreshhz
{
	unsigned long resolution;//分辨率
	unsigned char refreshhz[8];//此种分辨率下支持的屏幕刷新率(单位hz)，8种，0无效
}
NCFG_INFO_VGA_SUPPORT_REFRSHHZ;

typedef struct _ncfg_whiteBalance
{
	unsigned char ucMode;// 0 自动 1 手动 
	unsigned int red;	//红色增益
	unsigned int blue;	//蓝色增益
	//unsigned char unused;
}NCFG_WHITE_BALANCE;

typedef struct _ncfg_ptzSwitch
{
	unsigned long dwPtzSwitch;
}NCFG_PTZSWITCH;

typedef struct _ncfg_max_whiteBalance
{
	unsigned int redMax;	//红色增益
	unsigned int blueMax;	//蓝色增益
	//unsigned char unused[2];
}NCFG_MAX_WHITE_BALANCE;

typedef struct _ncfg_dynamic_range
{
	unsigned char ucMode;// 0 关闭 1 开启
	unsigned char value;
	unsigned char unused[2];
}NCFG_DYNAMIC_RANGE;

typedef struct _ncfg_denoise
{
	unsigned char ucMode;// 0 关闭 1 开启
	unsigned char value;
	unsigned char unused[2];
}NCFG_DENOISE;

typedef struct _ncfg_iristype
{
	unsigned char ucMode;// 0 关闭 1 开启
	unsigned char unused[3];
}NCFG_IRISTYPE;

typedef struct _ncfg_sharpen
{
	unsigned char ucMode;// 0 关闭 1 开启
	unsigned char value;
	unsigned char unused[2];
}NCFG_SHARPEN;

typedef struct _ncfg_day_night //panjy
{
	unsigned char ucMode; //color b/w auto
	unsigned char ucDelay; //auto模式下的延迟时间 0-255, default 4
	unsigned char ucDayTh; //日阈值 0-255 default 5
	unsigned char ucNightTh; //0 - 255 default 3
}NCFG_DAY_NIGHT;

typedef enum _day_night_mode
{
	DN_AUTO  = 0x0,
	DN_COLOR = 0x1,
	DN_BW    = 0x2
}DAY_NIGHT_MODE;

typedef enum _white_balance_9314
{
	WHITE_BLANCE_9314_ATW  = 0x0,
	WHITE_BLANCE_9314_PUSH = 0x1,
	WHITE_BLANCE_9314_CRS  = 0x2
}WHITE_BALANCE_9314;

//串口信息，用到哪些就定义哪些，此处这样定义使用
typedef struct _ncfg_info_ptz_config
{
	unsigned char	ucBaudrate;		//波特率0-13分别表示110，300，…..921600的波特率
	unsigned char	ucProtocolid;	//协议ID，0为没有配置云台
	unsigned char	ucDevaddr;		//云台设备ID
	unsigned char	ucNoUsed;		//没有用，只是为了对齐
}NCFG_INFO_PTZ_CONFIG;

//CBR模式下，每种分辨率的码流范围
typedef struct _ncfg_info_bitraterang
{
	unsigned long low;//最低码流
	unsigned long high;//最低码流
}
NCFG_INFO_BITRATE_RANG;

//VBR模式下每个档次的画质的目标码流
typedef struct _ncfg_info_vbrbitrate
{
	unsigned char resolution;//视频分辨率
	unsigned long targetbitrate[10];//最大支持10个档次，可能的将来会有7档画质还是有很大可能
}
NCFG_INFO_VBRBITRATE;

//解码器配置，实际应用还不成熟,但测试版在用
const unsigned long  	N_USER_NAME_LEN				= 32;
const unsigned long  	N_PASSWORD_LEN				= 16;
typedef struct _ncfg_info_map_remote_chnn 
{
	char userName[N_USER_NAME_LEN];
	char password[N_PASSWORD_LEN];
	unsigned long    serverIP;
	unsigned short   serverPort;

	unsigned char localChnn;//本地通道
	unsigned char remoteChnn;//对应的远程设备通道

	unsigned char osdchname;//通道名字显示开关
	unsigned char osddevip;//设备IP
	unsigned char osddevname;//设备名字	
    unsigned char resv[1];
}NCFG_INFO_MAP_REMOTE_CHNN;

//用户的，目前支持成这样子就定义成这样子，权限和通道去掉了,如果改变就换ID或增加ID来实现
typedef struct _ncfg_info_user
{
	unsigned long   group;
	char            name[36];
	char            password[36];
	unsigned char   MAC[6];
	unsigned char   reserved;
	unsigned char   ucBindMac;	//0 不绑定 1绑定
}NCFG_INFO_USER;

typedef struct _ncfg_info_short_name
{
	char	name [36];
}NCFG_INFO_SHORT_NAME;

typedef struct _ncfg_info_long_name
{
	char	name [132];
}NCFG_INFO_LONG_NAME;

typedef struct _ncfg_info_superlong_name
{
	char	name [1028];
}NCFG_INFO_SUPERLONG_NAME;

typedef struct _ncfg_info_dev_name_api_
{
    unsigned int		bSwitch;
	unsigned int		x;
	unsigned int		y;	
	char		szDevName[64];		//	设备名
}NCFG_INFO_DEV_NAME_EX;

//报警时触发摄像头移动
typedef struct _ncfg_info_toptz
{
	unsigned char	ucToPreset;				//报警时，移动到哪一个预制点0 -- 127	 0xff 表示不需要移动
	unsigned char	ucBackPreset;			//报警过后返回哪个预制点0 -- 127		 0xff 表示不返回
	char            cIsEnableTo;            //启用到达某个预置点
	char            cIsEnableBack;          //启用结束时返回某个点	
	unsigned char	ucToCruise;				//报警时,触发哪条巡航线0 -- 127		 0xff 表示不触发
	char            cIsEnableCruise;        //启用巡航线功能
	char			cIsEnableLocus;			//触发轨迹功能
	unsigned char	ucNoUsed[1];
}NCFG_INFO_TOPTZ;


typedef struct _ncfg_info_motion_info_ex
{
	unsigned long motiontype;	//移动区域类型，目前固定为1,当有其它类型需要特殊处理时+1
	unsigned long gridWidth;	//每行有多少个栈格
	unsigned long gridHeight;	//共有多少行栈格
	unsigned long xPixel;		//栈格开始象素x位置
	unsigned long yPixel;		//栈格开始象素y位置
	unsigned long gridData[128];//区域数据
	unsigned long Sensitivity;  //移动侦测灵敏度
}NCFG_INFO_MOTION_INFO;

typedef struct _ddns_server_info 
{
	unsigned char  ucDDNSID; /////对应的服务器ID   ID大于0对应的服务器名称才有效
	unsigned char  supportproperty;//支持的除用户名密码外的其它属性的MASK,NCFG_ENUM_DDNS_SUPPORT_PROPERTY
	unsigned char  noused[2];
	char           szDDNSServerName[64];  ////DDNS服务器的地址
}DDNS_SERVER_INFO, *LPDDNS_SERVER_INFO;


enum NCFG_ENUM_WIRELESS_AUTH_MODE
{
	NCFG_ENUM_WIRELESS_AUTH_MODE_OPEN,
	NCFG_ENUM_WIRELESS_AUTH_MODE_SHARED,
	NCFG_ENUM_WIRELESS_AUTH_MODE_WPA_PSK,
	NCFG_ENUM_WIRELESS_AUTH_MODE_WPA2_PSK,
};

enum NCFG_ENUM_WIRELESS_ENCRYPT_TYPE
{
	NCFG_ENUM_WIRELESS_ENCRYPT_TYPE_NONE,	//	CFG_WIRELESS_AUTH_MODE_OPEN, CFG_WIRELESS_AUTH_MODE_SHARED时填
	NCFG_ENUM_WIRELESS_ENCRYPT_TYPE_WEP,	//	CFG_WIRELESS_AUTH_MODE_OPEN, CFG_WIRELESS_AUTH_MODE_SHARED时填
	NCFG_ENUM_WIRELESS_ENCRYPT_TYPE_TKIP,	//	CFG_WIRELESS_AUTH_MODE_WPA_PSK, CFG_WIRELESS_AUTH_MODE_WPA2_PSK
	NCFG_ENUM_WIRELESS_ENCRYPT_TYPE_AES,	//	CFG_WIRELESS_AUTH_MODE_WPA_PSK, CFG_WIRELESS_AUTH_MODE_WPA2_PSK
};

enum NCFG_ENUM_DISK_TYPE
{
	NCFG_ENUM_DISK_TYPE_FIXEDDISK = 0,
	NCFG_ENUM_DISK_TYPE_REMOVABLE = 1,
	NCFG_ENUM_DISK_TYPE_CD        = 2,
	NCFG_ENUM_DISK_TYPE_ESATA     = 3,
	NCFG_ENUM_DISK_TYPE_SD		  = 4
};

//采用MASK方式,状态显示优先方式：磁盘有效-->磁盘休眠或SD卡被弹出-->磁盘未格式化-->磁盘异常
enum NCFG_ENUM_DISK_STATUS
{
	NCFG_ENUM_DISK_STATUS_VALID			=0x01,		//可用的,磁盘有效正在使用
	NCFG_ENUM_DISK_STATUS_UNFORMAT		=0x02,		//没有格式化的磁盘或SD，文件格式不符合标准
	NCFG_ENUM_DISK_STATUS_INSERT_POPED	=0x04,		//插了卡，但是被弹出了
	NCFG_ENUM_DISK_STATUS_SLEEPED		=0x08,		//磁盘已休眠
	NCFG_ENUM_DISK_STATUS_EXCEPTION		=0x10		//异常或已损坏磁盘
};

//单个磁盘或SD卡信息
typedef struct _ncfg_info_disk_info
{
	unsigned long   dwType ; /////DISK_TYPE
	unsigned long   dwIndex ;   ////磁盘索引，格式化磁盘的时候要用此变量作为参数进行格式化, -1为无效
	unsigned long	dwDiskStatus;	//CFG_SD_STATUS
	unsigned long	capability; //磁盘或SD卡的总容量，以MByte为单位
	unsigned long	Spaceused;	//使用了的空间，以MByte为单位
	unsigned long	SpaceSpare;	//剩余的空间，以MByte为单位
}NCFG_INFO_DISK_INFO, *LNCFG_INFO_PDISK_INFO;

typedef struct _ncfg_info_support_frame_rateEx
{
	unsigned long	dwStreamIndex;	//第几路码流
	unsigned long	dwResolution;	//什么分辨率
	unsigned long	dwRate;			//支持哪些帧率
	char			strResolutionName[16];//分辨率名字
	unsigned short	supportDataRate[20];//支持的码流
}NCFG_INFO_SUPPORT_FRAME_RATE;

typedef struct _ncfg_info_quadmode
{
	unsigned long quadmode;			//分割模式，1，4，9，16这样的数字
	unsigned long quadchannel;		//当前分割模式的通道
}NCFG_INFO_QUADMODE;

typedef struct _ncfg_info_time_pos
{
	unsigned long postype;
	unsigned long customdefine_X;
	unsigned long customdefine_Y;
}
NCFG_INFO_TIME_POS;

/// for api
typedef struct _ncfg_time_stamp_config
{
	unsigned int	bSwitch;			// 0 不要时间戳  1需要时间戳
	unsigned int		x;					// 时间戳位置 坐标以VGA为参考
	unsigned int		y;						
	unsigned int	format;				// 时间格式	DATE_MODE 0x01--年月日模式  0x02--月日年模式 0x03--日月年模式
}NCFG_TIME_STAMP_CONFIG;

typedef enum _ncfg_enum_support_disp_pos
{
	NCFG_ENUM_DISP_SUPPORT_TOP_RIGHT		= 0x01,//顶右
	NCFG_ENUM_DISP_SUPPORT_TOP_LEFT			= 0x02,//顶左
	NCFG_ENUM_DISP_SUPPORT_BOTTOM_RIGHT		= 0x04,//低右
	NCFG_ENUM_DISP_SUPPORT_BOTTOM_LEFT		= 0x08,//底左
	NCFG_ENUM_DISP_SUPPORT_TOP_MIDDLE		= 0x10,//顶中
	NCFG_ENUM_DISP_SUPPORT_BOTTOM_MIDDLE	= 0x20,//底中
	NCFG_ENUM_DISP_SUPPORT_CENTER_LEFT		= 0x40,//中左
	NCFG_ENUM_DISP_SUPPORT_CENTER_MIDDLE	= 0x80,//中中
	NCFG_ENUM_DISP_SUPPORT_CENTER_RIGHT		= 0x100,//中右
	NCFG_ENUM_DISP_SUPPORT_CUSTOM_DEFINE	=0x80000000,//自定义位置
}
NCFG_ENUM_DISP_POS;//

//支持哪些视频制式
typedef enum _ncfg_enum_support_video_format
{
	NCFG_ENUM_SUPPORT_VIDEOFORMAT_PAL=0x01,
	NCFG_ENUM_SUPPORT_VIDEOFORMAT_NTSC=0x02
}
NCFG_ENUM_SUPPORT_VIDEO_FORMAT;

//DDNS支持其它属性的取值范围，可以随其它类型而增加
typedef enum _ncfg_enum_ddns_support_property
{
	NCFG_ENUM_DDNS_SUPPORT_DOMAIN1	=0x01,//支持域名1，也有可能需要支持两个域名	
	NCFG_ENUM_DDNS_SUPPORT_SERVERTYPE = 0x02,
}
NCFG_ENUM_DDNS_SUPPORT_PROPERTY;

//本地IP设定信息
typedef struct _ncfg_info_localnet_
{
	char getIPMode;
	unsigned char	ucNoUsed[3];
	unsigned long ipaddr;
	unsigned long netmask;
	unsigned long gateway;
}
NCFG_INFO_LOCALNET;

//PPPOE信息
typedef struct _ncfg_info_pppoe_
{
	unsigned char enable;
	unsigned char ucNoUsed[3];
	char username[132];
	char password[132];
}
NCFG_INFO_PPPOE;

typedef struct _ncfg_info_filt_ip
{
	unsigned long	dwIp;			//需要过滤的IP地址/网段
	unsigned long	ipMask;			//掩码，如果全0则是IP地址，否则根据这个掩码判断上面的网段
}NCFG_INFO_FILT_IP;


//假日排程头信息
typedef struct _ncfg_info_holiday_sche_head
{
	unsigned long holidaynum;//假日数量
	unsigned long offset;//所含的每个排程日相对第一个NCFG_INFO_HOLIDAY_SCHE_HEAD开头的偏移
}
NCFG_INFO_HOLIDAY_SCHE_HEAD;

//夜视功能定义
typedef enum _night_vision_define
{
	NCFG_ENUM_NIGHT_VISION_AUTO=0,
	NCFG_ENUM_NIGHT_VISION_ON=1,
	NCFG_ENUM_NIGHT_VISION_OFF=2,
}
NCFG_ENUM_NIGHT_VISION;//

typedef struct _camera_freq_support
{
	unsigned long dwFreq;//哪种灯光模式
	char ucName[16];//灯光模式的名字
}CAMERA_FREQ_SUPPORT;

typedef struct _ptz_protocol_info 
{
	unsigned char  ucprotocolID; /////对应的协议类型ID
	unsigned char  supportproperty;//支持的除ID、波特率之外的其它属性的MASK
	unsigned char  noused[2];
	char           szProtocolName[64];  ////协议名称
}PTZ_PROTOCOL_INFO, *LPPTZ_PROTOCOL_INFO;

typedef struct _ptz_baudrate_info 
{
	unsigned char  ucbaudrateID; /////对应的波特率值
	unsigned char  noused[3];
	char           szbaudrateName[64];  ////对应的波特率名称
}PTZ_BAUDRATE_INFO, *LPPTZ_BAUDRATE_INFO;

/// to be remove
//周模式 TZ=CST+08:30:00CST+10:00:00,M3.1.0/02:0:0,M10.1.0/02:0:0
struct NCFG_DAYLIGHT_INFO_MODE_BY_WEEK	// 周模式
{
	unsigned char	ucMonth;		//第几月 0 -- 11
	unsigned char	ucWeekOfMonth; //这个月的第几周 0 -- 4
	unsigned char	ucDayOfWeek;	//这个周的第几天	0 -- 6	0表示星期天
	unsigned char	ucHour;
	unsigned char	ucMinute;
	unsigned char	ucSecond;
	unsigned char	ucNoused[2];
};

//在板子上的格式为 TZ=PST-8:0:0PDT-7:0:0,J65/02:0:0,J270/02:0:0
struct NCFG_DAYLIGHT_INFO_MODE_BY_DAY	//日期模式
{
	unsigned char	ucMonth;		//第几月 0 -- 11
	unsigned char	ucDayOfMonth;		//这个月的第几天 0 -- 4	
	unsigned char	ucHour;
	unsigned char	ucMinute;
	unsigned char	ucSecond;
	unsigned char	ucNoused[3];
};

union NCFG_DAYLIGHT_MODE_INFO
{
	struct NCFG_DAYLIGHT_INFO_MODE_BY_WEEK	daylightModeByWeek;
	struct NCFG_DAYLIGHT_INFO_MODE_BY_DAY	daylightModeByDay;
};

struct NCFG_DAYLIGHT_INFO
{
	unsigned char		ucEnableDaylight;		//0 非夏令时 1 夏令时		
	unsigned char		ucDayLightMode;			//0 周模式	 1 日期模式
	unsigned short		nDisSecond;				//相差几秒
	NCFG_DAYLIGHT_MODE_INFO	daylightModeInfoStart;
	NCFG_DAYLIGHT_MODE_INFO	daylightModeInfoEnd;
};
///
/// for api
typedef struct
{	
	int	                dwSynchronizeType; 	// 0: 手动 1: ntp服务器校时  较时方法
	char				szNtpServer[128];	//NTP服务器地址 IP 或者 host name
}NCFG_SYNCHRONIZE_INFO;

struct NCFG_TIMEZONE_INFO
{
	char 		    TimeZone[128];      /// 时区字符串
	unsigned long	bDaylightSwitch;	/// 0 关闭夏令时 1 启用夏令时 			
};

enum NCFG_ENCODE_VIDEO_ENCODE
{
    NCFG_ENCODE_H264 = 0x01,
//    NCFG_ENCODE_MJPEG= 0x02,
};

enum NCFG_ENCODE_AUDIO_ENCODE
{
    NCFG_ENCODE_G711A = 0,
	NCFG_ENCODE_G711U = 1,
};

struct NCFG_OSD_INFO_CONFIG
{
    unsigned int uShowOSDFlag;     //  1 show, 0 hide
    int iStartX;                   //  起始位置x的坐标
    int iStartY;                   //  起始位置y的坐标
    unsigned char ucOSDBuf[64];    //  gb2312编码后的内容
    unsigned int  uDataLen;         //  ucOSDBuf 的长度，不能超过30,也就是不能超过15个字符
    unsigned int  uShowLevel;      //  显示等级, 0 low, 1 middle, 2 high
    unsigned int  uFlicker;        //  是否闪烁, 0  not flicker, 1 flicker
};


typedef struct _onvif_client_info 
{
	unsigned char  ucONVIFID; /////对应的服务器ID   ID大于0对应的服务器名称才有效
	unsigned char  noused[3];
	char           szOnvifClientName[64];  ////ONVIF客户羰的类型
}ONVIF_CLIENT_INFO, *LONVIF_CLIENT_INFO;



// --------------------------------------------------------
// ---------------------------------------视频遮挡相关START

// mask矩形的位置和尺寸 - 约定以VGA为参考
struct NCFG_RECT
{
    int 				s32X;
    int 				s32Y;
    unsigned int 		u32Width;
    unsigned int 		u32Height;
};

// 透明度 - 海思方案不支持
enum NCFG_FOSD_TRANSPARENT_e
{
	NCFG_FOSD_TRANSPARENT_PERCENT_0,
	NCFG_FOSD_TRANSPARENT_PERCENT_50,
	NCFG_FOSD_TRANSPARENT_PERCENT_75,
	NCFG_FOSD_TRANSPARENT_PERCENT_100,
};

typedef struct _ncfg_osd_mask_config
{
	bool 					bShow;
	NCFG_RECT 				rectang;
	unsigned int 			color; // 0x00RRGGBB
	NCFG_FOSD_TRANSPARENT_e	transparent;
}NCFG_OSD_MASK_CONFIG;
// ---------------------------------------视频遮挡相关END
// --------------------------------------------------------

typedef struct _nfcg_fog_reduction_config
{
	unsigned char  ucFogReduction;          // 0 no, 1 yes
	unsigned char  ucFogReductionValue;     // 0 - 255
	char            noUse[2];
}NCFG_FOG_REDUCTION_CONFIG;

typedef struct _nfcg_lens_distortion_config
{
	unsigned char  ucLensDistortion;        // 0 no, 1 yes
	unsigned char  ucLensDistortionValue;   // 0 - 255
	char            noUse[2];
}NCFG_LENS_DISTORTION_CONFIG;

enum AF_CTRL_TYPE
{
	ONETIME_FOCUS = 0,   // 一键对焦
	MANUAL_FOCUS,        // 手动对焦
	MANUAL_ZOOM,         // 手动变倍
	AUTO_FOCUS,          // 自动一直对焦
};
typedef struct _af_control_
{
	AF_CTRL_TYPE afType;
	int			  nValue;  //手动微调时，1代表+，0代表-，其他动作类型nValue就传0
}AF_CONTROL_;


typedef struct _nfcg_video_gain
{
    unsigned char  ucAnalogGainValue;
    unsigned char  ucDigitalGainValue;
    char           noUse[2];
}NCFG_VIDEO_GAIN;

typedef struct _nfcg_digita_denoise
{
    unsigned char uc2DValue;
    unsigned char uc3DValue;
    char           noUse[2];
}NCFG_DIGITA_DENOISE;

typedef struct
{
    unsigned int iEnable; //  0 close, 1 open
    int iChn;               // 区域编号 1--8
    char szAreaName[16];    // 区域名字
    unsigned int x;         // 起始点的坐标
    unsigned int y;
    unsigned int w;
    unsigned int h;
    unsigned int iLevel;  //  1 -- 10
}NCFG_VIDEO_CONFIG_ROI;


#if 0
// -----------------------------------------------------------
// 音频设置相关 ----------------------------------------------
typedef enum _AUDIO_SMP_RATE_E 
{ 
    AUDIO_SMP_RATE_8000   = 8000,    /* 8K samplerate*/
    AUDIO_SMP_RATE_12000  = 12000,   /* 12K samplerate*/    
    AUDIO_SMP_RATE_11025  = 11025,   /* 11.025K samplerate*/
    AUDIO_SMP_RATE_16000  = 16000,   /* 16K samplerate*/
    AUDIO_SMP_RATE_22050  = 22050,   /* 22.050K samplerate*/
    AUDIO_SMP_RATE_24000  = 24000,   /* 24K samplerate*/
    AUDIO_SMP_RATE_32000  = 32000,   /* 32K samplerate*/
    AUDIO_SMP_RATE_44100  = 44100,   /* 44.1K samplerate*/
    AUDIO_SMP_RATE_48000  = 48000,   /* 48K samplerate*/
    AUDIO_SMP_RATE_BUTT,
} AUDIO_SMP_RATE_E; 

typedef enum _AUDIO_BITWIDTH_E
{
    AUDIO_BITWIDTH_8   = 0,   /* 8bit width */
    AUDIO_BITWIDTH_16  = 1,   /* 16bit width*/
    AUDIO_BITWIDTH_32  = 2,   /* 32bit width*/
    AUDIO_BITWIDTH_BUTT,
} AUDIO_BITWIDTH_E;

typedef struct _NCFG_AUDIO_CFG
{
	AUDIO_SMP_RATE_E	sample;			// 采样率
	unsigned char		ucEncodeType;	// 编码方式
	AUDIO_BITWIDTH_E	bitwidth;		// 采样的位宽
	unsigned char		channel;		// 几通道
	unsigned char		volume;			// 音量 [0, 100]
}NCFG_AUDIO_CFG;
// 音频设置相关 ----------------------------------------------
// -----------------------------------------------------------
#endif
typedef enum _pic_stream_resolution_
{
	PIC_RESOLUTION_CIF = 0, 
	PIC_RESOLUTION_D1 = 1,
}ENUM_PIC_STREAM_RESOLUTION;


// isp -------------------------------------------------------
// case NCFG_ITEM_EXPOSURE_CTRL: 			// 曝光控制:
#if 0
typedef enum _EXP_OP_TYPE_E
{
	TYPE_AUTO	= 0,
	TYPE_MANUAL	= 1,
} EXP_OP_TYPE_E;

typedef struct _AE_ATTR_S
{
    /* base parameter */
    ISP_AE_MODE_E enAEMode;		/*AE mode(lownoise/framerate)(onvif)*/
    HI_U16 u16ExpTimeMax;       /*the exposure time's max and min value .(unit :line)*/
    HI_U16 u16ExpTimeMin; 
    HI_U16 u16DGainMax;         /* the Dgain's min and max value*/
    HI_U16 u16DGainMin;
    HI_U16 u16AGainMax;			/* the Again's min and max value*/
    HI_U16 u16AGainMin;
    
    HI_U8  u8ExpStep;			/*AE adjust step*/
    HI_S16 s16ExpTolerance;		/*AE adjust tolerance*/
    HI_U8  u8ExpCompensation;	/*AE compensation*/ 
    ISP_AE_FRAME_END_UPDATE_E  enFrameEndUpdateMode;
    HI_BOOL bByPassAE;
    /*AE weighting table*/
    HI_U8 u8Weight[WEIGHT_ZONE_ROW][WEIGHT_ZONE_COLUMN];
} AE_ATTR_S;
typedef struct _ME_ATTR_S
{
	HI_S32 s32AGain;       		/* sensor analog gain (unit: times) */
	HI_S32 s32DGain;       		/* sensor digital gain (unit: times) */
	HI_U32 u32ExpLine;			/* sensor exposure time (unit: line ) */

	HI_BOOL bManualExpLineEnable;
	HI_BOOL bManualAGainEnable;
	HI_BOOL bManualDGainEnable;
} ME_ATTR_S;
typedef struct _NCFG_EXPOSURE_CTRL
{
	OP_TYPE_E			ExpType;			// 曝光方式
	AE_ATTR_S stAEAttr;
	ME_ATTR_S stMEAttr;
}NCFG_EXPOSURE_CTRL;
#endif

// case NCFG_ITEM_FOCUS_CTRL:				// 聚焦控制:


// case NCFG_ITEM_DIS:						// 数字防抖 Digital image stabilization:


// case NCFG_ITEM_Shading:					// 暗角补偿:


// case NCFG_ITEM_GAMMA:					// gamma:


// case NCFG_ITEM_ANTI_FLICKER:				// 抗闪烁:


// case NCFG_ITEM_CRStrength:				// Crosstalk Removal 强度


// case NCFG_ITEM_SlowFrameRate:			// 降帧率

// isp -------------------------------------------------------
// -----------------------------------------------------------




#pragma pack()

}
#endif //_NET_CONFIG_DEFINE_H_
