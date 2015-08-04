/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : 袁石维
** Date         : 2009-05-12
** Name         : StringDefine.h
** Version      : 1.0
** Description  :
** Modify Record:
1:
***********************************************************************/
#ifndef _STRING_DEFINE_H_
#define _STRING_DEFINE_H_

typedef enum _enum_string_id
{
	E_STR_ID_NULL			= 0x0000,

	/************************************************************************/
	/*									通用								*/
	/************************************************************************/
	//@section 通用
	//@enum 通用
	E_STR_ID_APPLY = 0x0001,			//Apply
	E_STR_ID_EXIT,						//Exit
	E_STR_ID_OK,						//OK
	E_STR_ID_CANCEL,					//Cancel
	E_STR_ID_YES,						//Yes
	E_STR_ID_NO,						//No

	//@enum 一些名词
	E_STR_ID_SYSTEM	= 0x0020,			//System
	E_STR_ID_TIME,						//Time
	E_STR_ID_FILE,						//File
	E_STR_ID_ALL,						//All
	E_STR_ID_TYPE,						//Type
	E_STR_ID_CHANNEL,					//Channel
	E_STR_ID_CH,						//CH
	E_STR_ID_NAME,						//Name
	E_STR_ID_HOUR,						//Hour
	E_STR_ID_MINUTE,					//Minute
	E_STR_ID_MENU,						//Menu
	E_STR_ID_INFORMATION,				//Information
	E_STR_ID_ENABLE,					//Enable
	E_STR_ID_DISABLE,					//Disable
	E_STR_ID_TRIGGER,					//Trigger
	E_STR_ID_AUDIO,						//Audio
	E_STR_ID_VOLUME,					//Volume
	E_STR_ID_MUTE,						//Mute
	E_STR_ID_VIDEO_OUTPUT,				//Video Output
	E_STR_ID_RESOLUTION,				//Resolution
	E_STR_ID_RECYCLE,					//Recycle
	E_STR_ID_PROGRESS,					//Progress
	E_STR_ID_PERCENT,					//Percent
	E_STR_ID_LIVE,						//Live
	E_STR_ID_DEFAULT,					//Default
	E_STR_ID_APPLY_SETTINGS_TO,			//Apply Settings To
	E_STR_ID_WRITING,					//Writing
	E_STR_ID_LOCKED,					//Locked
	E_STR_ID_VERSION,					//Version
	E_STR_ID_BASIC,						//Basic
	E_STR_ID_INDEX,						//Index
	E_STR_ID_STREAM,					//Stream
	E_STR_ID_NONE,						//None
	E_STR_ID_GENERAL,					//General
	E_STR_ID_ID,						//ID
	E_STR_ID_AUTO,						//Auto
	E_STR_ID_TRANSPARENCY,				//Transparency
	E_STR_ID_SIZE,						//Size
	E_STR_ID_FOLDER,					//Folder
	E_STR_ID_RECORD_EX,					//Record
	E_STR_ID_REMAINING,					//Remaining
	E_STR_ID_UNLOCKED,					//Unlock
	E_STR_ID_HOURS,                     //[Hours]
	E_STR_ID_MINUTES,                   //[Minutes]

	//@enum 一些动词
	E_STR_ID_PLAY = 0x0100,				//PLAY
	E_STR_ID_FF,						//FF
	E_STR_ID_REW,						//REW
	E_STR_ID_STOP,						//Stop
	E_STR_ID_PAUSE,						//Pause
	E_STR_ID_START,						//Start
	E_STR_ID_ADD,						//Add
	E_STR_ID_DELETE,					//Delete
	E_STR_ID_SEARCH,					//Search
	E_STR_ID_MODIFY,					//Modify
	E_STR_ID_LOCK,						//Lock
	E_STR_ID_SAVE,						//Save
	E_STR_ID_COPY,						//Copy
	E_STR_ID_SETTING,					//Setting
	E_STR_ID_IMPORT,					//Import
	E_STR_ID_EXPORT,					//Export
	E_STR_ID_DESCRIPTION,				//Description
	E_STR_ID_ALWAYS,					//Always
	E_STR_ID_TOP,						//Top
	E_STR_ID_UP,						//UP
	E_STR_ID_DOWN,						//Down
	E_STR_ID_BOTTOM,					//Bottom

	E_STR_ID_CLEAR_ALL,					//Delete All
	E_STR_ID_RECOVER_DEFAULT,			//Recover Default
	E_STR_ID_CREATE_TIME,				//Create Time
	E_STR_ID_RELAY_NAME,				//Relay Name
	E_STR_ID_CONFIG_NAME,				//Config Name
	E_STR_ID_CONFIG,					//Config
	E_STR_ID_DISK,						//Disk
	E_STR_ID_SHUT_DOWN,					//Shut Down
	E_STR_ID_READING,					//Reading...
	E_STR_ID_FORMAT_DOT,				//Formating...
	E_STR_ID_UNFORMAT,					//Unformatted
	E_STR_ID_TEST,						//Test
	E_STR_ID_PREV,						//Prev
	E_STR_ID_NEXT,						//Next
	E_STR_ID_FINISH,					//Finish
	E_STR_ID_SHOW_WZD_NEXT_TIME,		//Show Wizard Next Time
	E_STR_ID_SHOW_WZD,					//Startup Wizard
	E_STR_ID_SAVE_NOW,					//Save Now
	E_STR_ID_REGISTER,					//Register
	E_STR_ID_NEW,						//New
	E_STR_ID_SNAP,						//Snap
	E_STR_ID_SNAP_NUM,					//Snap Number
	E_STR_ID_SEARCH_IMAGES,				//Image
	E_STR_ID_NO_IMAGE,					//No Image
	E_STR_ID_SAVE_ALL,                  //Save All
	E_STR_ID_SNAP_OK,                   //Snap OK.
	E_STR_ID_SNAP_FAILED,               //Snap Failed!
	E_STR_ID_BROWSE,                    //Browse
	E_STR_ID_SNAP_CAPACITY,             //Capacity To Save Picture
	E_STR_ID_SNAP_ALL_SIZE,             //Total Number
	E_STR_ID_SNAP_USED_SIZE,            //Number of Used
	E_STR_ID_NOT_DISPLAY,               //No Image When Logout
	E_STR_ID_ADJUST_CVBS_VO,            //Adjust CVBS video out
	E_STR_ID_POSTEXT_SEARCH,		    //POS Text 



	/************************************************************************/
	/*									专业术语                            */
	/************************************************************************/
	//@section 专业术语
	//@enum 业务
	E_STR_ID_RECORD	= 0x0200,			//Record
	E_STR_ID_SCHEDULE,					//Schedule
	E_STR_ID_ALARM,						//Alarm
	E_STR_ID_MOTION,					//Motion
	E_STR_ID_SENSOR,					//Sensor
	E_STR_ID_NETWORK,					//Network
	E_STR_ID_PTZ,						//P.T.Z
	E_STR_ID_BACKUP,					//Backup
	E_STR_ID_LOG,						//Log
	E_STR_ID_EVENT,						//Event
	E_STR_ID_PTZ_CONTROL,				//P.T.Z Control
	E_STR_ID_REMOTE_LOGON,				//Remote Login
	E_STR_ID_DATA_MANAGER,				//File Management
	E_STR_ID_SETUP,						//Setup
	E_STR_ID_CONFIG_OPERATE,			//Config Operate
	E_STR_ID_CONFIG_MANAGER,			//Config Management
	E_STR_ID_PLAYBACK,					//Playback
	E_STR_ID_HOLIDAY_SCHEDULE,			//Holiday
	E_STR_ID_REC_SCHEDULE,				//Schedule
	E_STR_ID_REC_MOTION,				//Motion
	E_STR_ID_REC_SENSOR,				//Sensor
	E_STR_ID_ADVANCE,					//Advanced
	E_STR_ID_MANUAL,					//Manual
	E_STR_ID_CLEAR_ALARM,				//Clear

	//@enum 检索 & 回放 & 备份
	E_STR_ID_TIME_SEARCH = 0x0240,		//Time Search
	E_STR_ID_EVENT_SEARCH,				//Event Search
	E_STR_ID_FILE_MANAGER,				//File Management
	E_STR_ID_HOLD_TIME,					//Holding Time[S]
	E_STR_ID_START_TIME,				//Start Time
	E_STR_ID_END_TIME,					//End Time
	E_STR_ID_AVI,						//AVI
	E_STR_ID_DVR,						//DVR
	E_STR_ID_STORAGE_TYPE,				//Storage Media
	E_STR_ID_BACKUP_PLAYER,				//Backup Player
	E_STR_ID_FILES_NUM,					//The Number Of Files
	E_STR_ID_SAVE_FILE_TYPE,			//Save File Type

	//@enum 视频 & 图像 & 录像
	E_STR_ID_VIDEO_FORMAT = 0x0380,		//Video Format
	E_STR_ID_FRAME_RATE,				//Frame Rate
	E_STR_ID_TIME_STAMP,				//Time Stamp
	E_STR_ID_PRE_RECORD_TIME,			//Pre-alarm Record Time[S]
	E_STR_ID_POST_RECORD_TIME,			//Post-alarm Record Time[S]
	E_STR_ID_NTSC,						//NTSC
	E_STR_ID_PAL,						//PAL
	E_STR_ID_RATE,						//Rate
	E_STR_ID_RECYCLE_RECORD,			//Recycle Record
	E_STR_ID_RECORD_TIME,				//Time
	E_STR_ID_RECORD_STREAM,				//Record Bitrate
	E_STR_ID_RECORD_HOLD_TIME,			//Expire[Days]
	E_STR_ID_REC_ENCODE_TYPE,			//Encode
	E_STR_ID_REC_MAX_BIT_STREAM,		//Max Bitrate
	E_STR_ID_REC_VBR,					//VBR
	E_STR_ID_REC_CBR,					//CBR
	E_STR_ID_REC_REDUNDANT,				//Redundancy
	E_STR_ID_RECORDING,					//Recording...
	E_STR_ID_REC_NEVER,					//Never
	E_STR_ID_FPS,						//fps
	E_STR_ID_960H_MODE,					//960H MODE

	//@enum 设备 & 系统
	E_STR_ID_DEV_NAME = 0x0400,			//Device Name
	E_STR_ID_DEV_ID,					//Device ID
	E_STR_ID_PASSWORD_CHECK,			//Password Check

	//@enum 语言
	E_STR_ID_LANGUAGE = 0x0420,			//Language

	E_STR_ID_POS_NAME = 0x0430,			//POS Name
	E_STR_ID_POS_ENABLE,				//Enable
	E_STR_ID_POS_CONNECTION,			//Connection Type
	E_STR_ID_POS_NET,				//Network
	E_STR_ID_POS_SERIAL,				//RS232
	E_STR_ID_POS_BASIC,				//Basic
	E_STR_ID_POS_PROTOCOL,				//Protocol
	E_STR_ID_POS_GENERIC,				//Generic
	E_STR_ID_POS_CHANNEL,				//Support Channel

	//@enum 日期 & 时间 相关
	E_STR_ID_DATE_FORMAT = 0x0480,		//Date Format
	E_STR_ID_MM_DD_YY,					//MM-DD-YY
	E_STR_ID_DD_MM_YY,					//DD-MM-YY
	E_STR_ID_YY_MM_DD,					//YY-MM-DD
	E_STR_ID_TIME_ZONE,					//Time Zone
	E_STR_ID_NTP_SERVER,				//NTP Server
	E_STR_ID_SYNC_TIME_WITH_NTP,		//Sync Time With NTP Server
	E_STR_ID_DATE_AND_TIME,				//Date & Time
	E_STR_ID_TIME_FORMAT,				//Time Format
	E_STR_ID_TIME_HOUR_12,				//12 Hour
	E_STR_ID_TIME_HOUR_24,				//24 Hour
	E_STR_ID_DST,						//DST
	E_STR_ID_DST_FULL,					//Daylight Saving Time
	E_STR_ID_DATE,						//Date
	E_STR_ID_UPDATE_NOW,				//Update Now
	E_STR_ID_JANUARY,					//January
	E_STR_ID_FEBRUARY,					//February
	E_STR_ID_MARCH,						//March
	E_STR_ID_APRIL,						//April
	E_STR_ID_MAY,						//May
	E_STR_ID_JUNE,						//June
	E_STR_ID_JULY,						//July
	E_STR_ID_AUGUST,					//August
	E_STR_ID_SEPTEMBER,					//September
	E_STR_ID_OCTOBER,					//October
	E_STR_ID_NOVEMBER,					//November
	E_STR_ID_DECEMBER,					//December
	E_STR_ID_DST_WEEK_1ST,				//The 1st
	E_STR_ID_DST_WEEK_2ND,				//The 2nd
	E_STR_ID_DST_WEEK_3RD,				//The 3rd
	E_STR_ID_DST_WEEK_4TH,				//The 4th
	E_STR_ID_DST_WEEK_5TH,				//The 5th
	E_STR_ID_DST_WEEK_LAST,				//The Last
	E_STR_ID_DST_TIME_OFFSET,			//Time Offset [Hours]
	E_STR_ID_FROM,						//From
	E_STR_ID_UNTIL,						//Until
	E_STR_ID_MODE,						//Mode
	E_STR_ID_SYSTEM_DATE,				//System Date
	E_STR_ID_SYSTEM_TIME,				//System Time

	//@enum 现场 & 色彩
	E_STR_ID_MASK = 0x0500,				//Mask
	E_STR_ID_CAMERA_NAME,				//Camera Name
	E_STR_ID_SHOW_TIME,					//Show System Time
	E_STR_ID_DWELL_TIME,				//Dwell Time[S]
	E_STR_ID_SPOT,						//Spot
	E_STR_ID_COLOR,						//Color
	E_STR_ID_BRIGHTNESS,				//Brightness
	E_STR_ID_HUE,						//Hue
	E_STR_ID_SATURATION,				//Saturation
	E_STR_ID_CONTRAST,					//Contrast
	E_STR_ID_VIDEO_LOSS,				//Video Loss
	E_STR_ID_SHOW_NAME,					//Show Name
	E_STR_ID_HOST_MONITOR,				//Main Monitor
	E_STR_ID_SPLIT_MODE,				//Display Mode
	E_STR_ID_STAMP,						//Stamp
	E_STR_ID_POSITION,					//Position
	E_STR_ID_MASK_AREA,					//Mask Area
	E_STR_ID_SCREENSAVER,				//Logout After [Minutes]
	E_STR_ID_LIVE_REC_STATUS,           //Recording Status
	E_STR_ID_CAM_PARA,					//Cam Parameter
	E_STR_ID_TEXT,				//Text

	//图像相关操作， 镜像， 翻转 ，畸变校正
    E_STR_ID_IMAGE_MIRROR_ON = 0x0530,  //Mirror On
	E_STR_ID_IMAGE_MIRROR_OFF,          //Mirror Off
    E_STR_ID_IMAGE_FILP_ON,             //Flip On
	E_STR_ID_IMAGE_FILP_OFF,            //Flip Off
	E_STR_ID_IMAGE_ROTATO,              //Rotato
	E_STR_ID_IMAGE_DISTORTION,          //Distortion
	E_STR_ID_IMAGE_AUTO_FOCUS,          //Auto Focus
	E_STR_ID_IMAGE_ROI,                 //ROI
	E_STR_ID_CAMERA_PARAMTER,           //Cam Parameter

	//@enum 画质
	E_STR_ID_VIDEO_QUALITY = 0x0540,	//Video Quality
	E_STR_ID_HIGHEST,					//Highest
	E_STR_ID_HIGHER,					//Higher
	E_STR_ID_MEDIUM,					//Medium
	E_STR_ID_LOW,						//Low
    E_STR_ID_LOWER,                     //Lower
	E_STR_ID_LOWEST,					//Lowest
	E_STR_ID_QUALITY,					//Quality
	
    //
	E_STR_ID_CAM_FOCUS_MODE = 0x0560,   //Focus Mode
	E_STR_ID_CAM_AUTO_FOCUS,			//Auto Focus
	E_STR_ID_CAM_MANUAL_FOCUS,			//Manual Focus
	E_STR_ID_CAM_FOCUS_TIME_INTERVAL,   //Focus Time Interval
	E_STR_ID_CAM_ALWAYS_KEEP,			//Always Keep
	E_STR_ID_CAM_ONE_MINUTE,			//1 Minute
	E_STR_ID_CAM_FIVE_MINUTE,			//5 Minutes
	E_STR_ID_CAM_TEN_MINUTE,			//10 Minutes
	E_STR_ID_CAM_HALF_HOUR,				//30 Minutes
	E_STR_ID_CAM_MANUAL_ZOOM,           //     ZOOM    

	//@enum 排程 & 星期
	E_STR_ID_SUNDAY = 0x0580,			//Sunday
	E_STR_ID_MONDAY,					//Monday
	E_STR_ID_TUESDAY,					//Tuesday
	E_STR_ID_WEDNESDAY,					//Wednesday
	E_STR_ID_THURSDAY,					//Thursday
	E_STR_ID_FRIDAY,					//Friday
	E_STR_ID_SATURDAY,					//Saturday
	E_STR_ID_HOLIDAY,					//Holiday
	E_STR_ID_WEEK,						//Week
	E_STR_ID_HOLIDAY_SETTING,			//Holiday Setting

	E_STR_ID_SUN,						//Su
	E_STR_ID_MON,						//Mo
	E_STR_ID_TUE,						//Tu
	E_STR_ID_WED,						//We
	E_STR_ID_THU,						//Th
	E_STR_ID_FRI,						//Fr
	E_STR_ID_SAT,						//Sa
	E_STR_ID_WEEK_DAYS,					//Week Days

	//@enum 报警
	E_STR_ID_DEVICE_TYPE = 0x0600,		//Device Type
	E_STR_ID_SENSOR_NC,					//NC
	E_STR_ID_SENSOR_NO,					//NO
	E_STR_ID_ALARM_IN_HOLD_TIME,		//Alarm In Hold Time[S]
	E_STR_ID_ALARM_OUT_HOLD_TIME,		//Alarm Out Hold Time[S]
	E_STR_ID_BUZZER_HOLD_TIME,			//Buzzer Hold Time[S]
	E_STR_ID_BUZZER,					//Buzzer
	E_STR_ID_TO_RECORD,					//To Record
	E_STR_ID_TO_PTZ_CH,					//To P.T.Z CH
	E_STR_ID_TO_ALARM_OUT,				//To Alarm Out
	E_STR_ID_ALARM_NAME,				//Alarm Name
	E_STR_ID_MANUAL_ALARM,				//Manual Alarm
	E_STR_ID_ALL_TRIGGER,				//All Trigger
	E_STR_ID_OTHER_ALARM,				//Other Alarm
	E_STR_ID_ALARM_OUT,					//Alarm Out
	E_STR_ID_ALARM_HANDING,				//Alarm Handling
	E_STR_ID_NET_ALARM,					//Network Alarm
	E_STR_ID_ALARM_BASIC,				//Alarm
	E_STR_ID_TO_PTZ,					//To P.T.Z
	E_STR_ID_FULL_SCREEN,				//Show Full Screen
	E_STR_ID_ALARM_NO,					//No.
	E_STR_ID_DISK_ALARM,				//Disk Shortage Alarm
	E_STR_ID_ALARM_TYPE,				//Alarm Type
	E_STR_ID_DISK_FULL,					//Disk Full
	E_STR_ID_DISCONNECT,				//Disconnect
	E_STR_ID_PRODUCT_MODEL,				//Product Model

	
	//@enum 移动侦测
	E_STR_ID_AREA = 0x0640,				//Area
	E_STR_ID_CLEAR,						//Clear
	E_STR_ID_CUSTOM,					//Custom
	E_STR_ID_SENSITIVITY,				//Sensitivity

	//@enum 网络
	E_STR_ID_HTTP_PORT = 0x0680,		//HTTP Port
	E_STR_ID_SERVER_PORT,				//Server Port
	E_STR_ID_DHCP,						//DHCP
	E_STR_ID_IP,						//IP
	E_STR_ID_NET_MASK,					//Subnet Mask
	E_STR_ID_GATEWAY,					//Gateway
	E_STR_ID_PREFERRED_DNS_SERVER,		//Preferred DNS Server
	E_STR_ID_ALTERNATE_DNS_SERVER,		//Alternate DNS Server
	E_STR_ID_PPPOE,						//PPPoE
	E_STR_ID_NETWORK_STREAM,			//Sub-stream
	E_STR_ID_IP_AUTO,					//Obtain an IP address automatically
	E_STR_ID_IP_ADDRESS,				//IP Address
	E_STR_ID_LOCAL_IP,					//Local IP
	E_STR_ID_MAX_NETWORK_USERS,			//Max Online Users
	E_STR_ID_CONNECTING,				//Connecting...
	E_STR_ID_UNCONNECTED,				//Unconnected
	E_STR_ID_CONNECTED,					//Connected
	E_STR_ID_NETWORKAPPROACH,			//Networking Approach
	E_STR_ID_STATIC_IP,					//Static IP
	E_STR_ID_EMAIL,						//Email
	E_STR_ID_OTHERSETTINGS,				//Other Settings
	E_STR_ID_UPDATE_CYCLE,				//DDNS Update [Hours]
	E_STR_ID_UPNP,						//UPnP
	E_STR_ID_NAT,						//NAT
	E_STR_ID_NET_SERVER,				//NAT Server
	E_STR_ID_PUSH,						//PUSH
	E_STR_ID_PUSH_SERVER,				//PUSH Server
	E_STR_ID_SWITCHIP,					//Switch Ip
	E_STR_ID_SWITCHSUBNETMASK,			//Switch SubNetMask
	E_STR_ID_NET_ENABLE,				//NAT Enable
	E_STR_ID_PUSH_ENABLE,				//PUSH Enable

	//@enum DDNS
	E_STR_ID_SERVER = 0x0700,			//Server
	E_STR_ID_DOMAIN,					//Domain
	E_STR_ID_DDNS,						//DDNS
	E_STR_ID_DDNS_SERVER,				//DDNS Server
	E_STR_ID_HOST_DOMAIN,				//Host Domain
	E_STR_ID_DDNS_TYPE,                 //DDNS Type
	E_STR_ID_SUBDOMAIN,					//Sub Domain
	//@enum Mail
	E_STR_ID_SMTP_SER = 0x0725,			//SMTP Server
	E_STR_ID_SLL_CHECK,					//SSL Check
	E_STR_ID_SEND_NAME,					//Send Address
	E_STR_ID_RECV_EMAIL_ADDR,			//Receive Address
	E_STR_ID_ADVANCED_CFG,              //Advanced
	E_STR_ID_IMG_NUM,                   //Attaching Image
	E_STR_ID_SNAP_INTERVAL,             //Snap Time Interval[S]
	E_STR_ID_IMG_NONE,                  //None

	//@enum hxht
	E_STR_ID_MEGA_EYES = 0x0730,		//Mega Eyes
	E_STR_ID_VS_DEVICE_ID,				//Video Server ID
	E_STR_ID_VS_DATA_PORT,				//Video Server Data Port
	E_STR_ID_CS_IP,						//Connect Server IP
	E_STR_ID_CS_MESSAGE_PORT,			//Connect Server Message Port
	E_STR_ID_CS_STORE_PORT,				//Connect Server Store Port

	/************************************************************************/
	/*                         云台相关                                     */
	/************************************************************************/
	//@section 云台相关
	//@enum 云台
	E_STR_ID_PRESET = 0x0740,			//Preset
	E_STR_ID_CRUISE,					//Cruise
	E_STR_ID_TRACK,						//Track
	E_STR_ID_AUTO_SCAN,					//Auto Scan
	E_STR_ID_CRUISE_PRESET,				//Cruise Preset
	E_STR_ID_STOP_CRUISE,				//Stop Cruise
	E_STR_ID_PREVIEW,					//Preview
	E_STR_ID_PROTOCOL,					//Protocol
	E_STR_ID_BAUD_RATE,					//Baud Rate
	E_STR_ID_ADDRESS,					//Address
	E_STR_ID_SERIAL_PORT,				//Serial Port
	E_STR_ID_SPEED,						//Speed
	E_STR_ID_ZOOM,						//Zoom
	E_STR_ID_FOCUS,						//Focus
	E_STR_ID_IRIS,						//Iris
	E_STR_ID_START_TRACK,				//Start Track
	E_STR_ID_STOP_TRACK,				//Stop Track
	E_STR_ID_START_RECORD_TRACK,		//Start Record
	E_STR_ID_STOP_RECORD_TRACK,			//Stop Record
	E_STR_ID_START_AUTOSCAN,			//Start Auto Scan
	E_STR_ID_STOP_AUTOSCAN,				//Stop Auto Scan
	E_STR_ID_START_CRUISE,				//Start Cruise
	E_STR_ID_VIRTUAL_CRUISE,            //Simulative Cruise

	
	//@section ftp服务器相关
	E_STR_ID_FTP_SERVICE = 0x0780,		//FTP Service
	E_STR_ID_FTP_TYPE,					//FTP Type
	E_STR_ID_FTP_ANONYMITY,				//Anonymity Login
	E_STR_ID_FTP_REMOTE_DIR,			//Remote Directory
	E_STR_ID_FTP,						//FTP
	E_STR_ID_FTP_UPDATE_VIDEO,			//Update Video
	E_STR_ID_FTP_BEYOND_UPLOAD_CHANNEL, //Beyond the upload channel number limit
	E_STR_ID_FTP_FILE_LEN,				//Upload File Length[M]

	/************************************************************************/
	/*                         用户相关                                     */
	/************************************************************************/
	//@section 用户相关
	//@enum 用户
	E_STR_ID_USER_NAME = 0x0800,		//User Name
	E_STR_ID_USER_TYPE,					//User Type
	E_STR_ID_PASSWORD,					//Password
	E_STR_ID_NEW_PASSWORD,				//New Password
	E_STR_ID_COMFIRM_PASSWORD,			//Confirm Password
	E_STR_ID_LOGIN,						//Login
	E_STR_ID_LOGOUT,					//Logout
	E_STR_ID_NORMAL,					//Normal
	E_STR_ID_ADVANCED_USER,				//Advanced
	E_STR_ID_AUTHORITY,					//Authority
	E_STR_ID_BIND_PC_MAC_ADDR,			//Binding PC MAC Address
	E_STR_ID_PC_MAC_ADDR,				//PC MAC Address
	E_STR_ID_ADMIN,						//Admin
	E_STR_ID_SET_PASSWORD,				//Set Password
	E_STR_ID_MODIFY_PASSWORD,			//Change Password
	E_STR_ID_OLD_PASSWORD,				//Old Password
	E_STR_ID_USERS,						//Users
	E_STR_ID_AUTH_LOG_VIEW,				//Log Search
	E_STR_ID_AUTH_SYSTEM_SETUP,			//System Setup
	E_STR_ID_AUTH_SHUT_DOWN,			//Shut Down
	E_STR_ID_AUTH_AUDIO_TALK,			//Two Way Audio
	E_STR_ID_AUTH_DATA_MAN,				//File Management
	E_STR_ID_AUTH_DISK_MAN,				//Disk Management
	E_STR_ID_AUTH_REMOTE_LOGON,			//Remote Login
	E_STR_ID_AUTH_LIVE_VIEW	,			//Live View
	E_STR_ID_AUTH_RECORD,				//Manual Record
	E_STR_ID_AUTH_PLAYBACK,				//Playback
	E_STR_ID_AUTH_BACKUP,				//Backup
	E_STR_ID_AUTH_PTZ_CONTROL,			//P.T.Z Control
	E_STR_ID_AUTH_REMOTE_VIEW,			//Remote Live View
	E_STR_ID_NO_AHTHORITY,				//Not Authorized

	/************************************************************************/
	/*                         工具、高级配置                               */
	/************************************************************************/
	//@section 工具、高级配置
	//@enum 工具、高级配置
	E_STR_ID_LOAD_DEFAULT = 0x0900,		//Load Default

	//@enum 磁盘管理
	E_STR_ID_DISK_MANAGER,				//Disk Management
	E_STR_ID_NUMBER,					//No.
	E_STR_ID_SIZE_GB,					//Size[GB]
	E_STR_ID_FREE_GB,					//Free[GB]
	E_STR_ID_STATUS,					//Status
	E_STR_ID_FORMAT,					//Format
	E_STR_ID_OTHER,						//External
	E_STR_ID_NO_FORMAT,					//Unformatted
	E_STR_ID_PROPERTY,					//Properties
	E_STR_ID_READ_ONLY,					//Read Only
	E_STR_ID_READ_WRITE,				//Read & Write
	E_STR_ID_REDUNDANCY,				//Redundancy
	E_STR_ID_OWNER,						//Sources
	E_STR_ID_REFRESH,					//Refresh
	E_STR_ID_REFRESH_DOT,				//Refresh...
	E_STR_ID_THIS,						//Local
	E_STR_ID_NORMAL_DISK,				//Normal
	E_STR_ID_DISK_COUNT,				//Disk Count
	E_STR_ID_DISK_IDENT,				//Identifier


	//@enum 升级
	E_STR_ID_UPGRADE,					//Upgrade
	E_STR_ID_DATE_MODIFIED,				//Date Modified

	//@enum 设备
	E_STR_ID_DEVICE,					//Device
	E_STR_ID_PORT,						//Port
	E_STR_ID_IPCAM,						//IP CAMERA
	E_STR_ID_THIS_DEVICE,				//This Device

	E_STR_ID_BLACKLIST,					//Block List
	E_STR_ID_WHITELIST,					//Allow List
	E_STR_ID_IP_FROM,					//IP From
	E_STR_ID_IP_TO,						//To
	E_STR_ID_BLACK_WHITE_LIST,			//Block / Allow List
	E_STR_ID_DISK_CLEAN_UP,				//Disk Cleanup

	//@enum 磁盘smart相关
	E_STR_ID_DISK_MODEL,               //Model:
	E_STR_ID_DISK_SN,                  //S/N:
	E_STR_ID_DISK_FIRMWARE,            //Firmware:
	E_STR_ID_DISK_HEALTH,              //Health Status:
	E_STR_ID_DISK_SMART_INFO,          //S.M.A.R.T. Information
	E_STR_ID_SM_ID,                    //ID
	E_STR_ID_SM_CURR,                  //Current
	E_STR_ID_SM_WORST,                 //Worst
	E_STR_ID_SM_TH,                    //Threshold
	E_STR_ID_SM_RAW,                   //RawValue
	E_STR_ID_SM_STA,                   //Status
	E_STR_ID_SM_STA_OK,                //ok
	E_STR_ID_SM_STA_WAR,               //warning
    E_STR_ID_SM_STA_CRI,               //critical
	E_STR_ID_H01_NAME,                 //Read Error Rate
	E_STR_ID_H03_NAME,                 //Spin-Up Time
	E_STR_ID_H04_NAME,                 //Start/Stop Count
	E_STR_ID_H05_NAME,                 //Reallocated Sectors Count
	E_STR_ID_H07_NAME,                 //Seek Error Rate
	E_STR_ID_H09_NAME,                 //Power-On Hours
	E_STR_ID_H0A_NAME,                 //Spin Retry Count
	E_STR_ID_H0C_NAME,                 //Power Cycle Count
	E_STR_ID_HC2_NAME,                 //Temperature
	E_STR_ID_HC3_NAME,                 //Hardware ECC Recovered
	E_STR_ID_HC5_NAME,                 //Current Pending Sector Count
	E_STR_ID_HC6_NAME,                 //Uncorrectable Sector Count
	E_STR_ID_HC7_NAME,                 //UltraDMA CRC Error Count
	E_STR_ID_DEF_NAME,                 //Unknown
	E_STR_ID_DISK_GROUP_ID,            //Group ID
	E_STR_ID_DISK_GROUP,               //Disk Group
	E_STR_ID_DISK_GROUP_REC,           //Disk Group Record Config
	E_STR_ID_DISK_STATUS,              //Disk Status


	/************************************************************************/
	/*								系统信息								*/
	/************************************************************************/
	//@section 系统信息
	//@enum 系统信息
	E_STR_ID_HARDWARE_VERSION = 0x0a00,		//Hardware Version
	E_STR_ID_MCU_VERSION,					//MCU Version
	E_STR_ID_KERNEL_VERSION,				//Kernel Version
	E_STR_ID_FIRMWARE_VERSION,				//Firmware Version
	E_STR_ID_LAUNCH_DATE,					//Launch Date
	E_STR_ID_MAC,							//MAC
	E_STR_ID_ONLINE,						//Online

	//@enum 操作
	E_STR_ID_LOG_SYSTEM_CONTROL,			//Operation
	E_STR_ID_LOG_SYSTEM_SETUP,				//Setup
	E_STR_ID_LOG_PLAYBACK,					//Playback
	E_STR_ID_LOG_BACKUP,					//Backup
	E_STR_ID_LOG_SEARCH,					//Search
	E_STR_ID_LOG_VIEW_INFORMATION,			//Check Information
	E_STR_ID_LOG_ERROR,						//Error


	//@section 标题、菜单
	//@enum 对话框标题
	E_STR_ID_TITLE_SYSTEM	= 0x0b00,		//SYSTEM
	E_STR_ID_TITLE_SEARCH,					//SEARCH
	E_STR_ID_TITLE_BACKUP,					//BACKUP
	E_STR_ID_TITLE_INFORMATION,				//INFORMATION
	E_STR_ID_TITLE_EVENT_LIST,				//EVENT LIST
	E_STR_ID_TITLE_LOG_LIST,				//LOG LIST
	E_STR_ID_TITLE_NETWORK_INFO,			//NETWORK
	E_STR_ID_TITLE_ONLINE_USER_LIST,		//ONLINE USER LIST
	E_STR_ID_TITLE_BASIC,					//BASIC
	E_STR_ID_TITLE_LIVE,					//LIVE
	E_STR_ID_TITLE_RECORD,					//RECORD
	E_STR_ID_TITLE_SCHEDULE,				//SCHEDULE
	E_STR_ID_TITLE_ALARM,					//ALARM
	E_STR_ID_TITLE_TRIGGER,					//TRIGGER
	E_STR_ID_TITLE_NETWORK,					//NETWORK
	E_STR_ID_TITLE_PTZ,						//P.T.Z
	E_STR_ID_TITLE_USER_MAN,				//USER MANAGEMENT
	E_STR_ID_TITLE_ONLINE_USER,				//ONLINE USER
	E_STR_ID_TITLE_ADVANCED,				//ADVANCED
	E_STR_ID_TITLE_ADVANCE_SETUP,			//ADVANCE SETUP
	E_STR_ID_TITLE_UPGRADE,					//UPGRADE
	E_STR_ID_TITLE_DISK_MAN,				//DISK MANAGEMENT
	E_STR_ID_TITLE_DEVICE_MAN,				//DEVICE MANAGEMENT
	E_STR_ID_TITLE_ADD_DEVICE,				//ADD DEVICE
	E_STR_ID_TITLE_MODIFY_DEVICE,			//MODIFY DEVICE
	E_STR_ID_TITLE_SUCCEED,					//SUCCEED
	E_STR_ID_TITLE_ERROR,					//ERROR
	E_STR_ID_TITLE_WARNING,					//WARNING
	E_STR_ID_TITLE_FAILED,					//FAILED
	E_STR_ID_TITLE_LOGIN,					//LOGIN
	E_STR_ID_TITLE_MAIN_MENU,				//MAIN MENU
	E_STR_ID_TITLE_HOLIDAY_SCHEDULE,		//HOLIDAY SCHEDULE
	E_STR_ID_TITLE_ALARM_OUT,				//ALARM OUT
	E_STR_ID_TITLE_ADD_USER,				//ADD USER
	E_STR_ID_TITLE_MODIFY_USER,				//MODIFY USER
	E_STR_ID_TITLE_PRESET,					//PRESET
	E_STR_ID_TITLE_CRUISE,					//CRUISE
	E_STR_ID_TITLE_CRUISE_PRESET,			//CRUISE PRESET
	E_STR_ID_TITLE_MANUAL_ALARM,			//MANUAL ALARM
	E_STR_ID_TITLE_MOTION,					//MOTION
	E_STR_ID_TITLE_OTHER_ALARM,				//OTHER ALARM
	E_STR_ID_TITLE_SENSOR,					//SENSOR
	E_STR_ID_TITLE_VIDEO_LOSS,				//VIDEO LOSS
	E_STR_ID_TITLE_BACKUP_INFO,				//BACKUP INFORMATION
	E_STR_ID_TITLE_NET_ALARM,				//NETWORK ALARM
	E_STR_ID_TITLE_SELECT_DISK,				//SELECT DISK
	E_STR_ID_TITLE_WIZZARD,					//WIZARD
	E_STR_ID_TITLE_EXPORTLOG,				//EXPORT LOG
	E_STR_ID_TITLE_SETUP,					//SETUP
	E_STR_ID_TITLE_DATE_AND_TIME,			//DATE & TIME
	E_STR_ID_TITLE_MAIN_MONITOR,			//MAIN MONITOR
	E_STR_ID_TITLE_SPOT,					//SPOT
	E_STR_ID_TITLE_MASK,					//MASK
	E_STR_ID_TITLE_RECORD_BITRATE,			//RECORD BITRATE
	E_STR_ID_TITLE_TIME,					//TIME
	E_STR_ID_TITLE_STAMP,					//STAMP
	E_STR_ID_TITLE_RECYCLE_RECORD,			//RECYCLE RECORD
	E_STR_ID_TITLE_SUB_STREAM,				//SUB-STREAM
	E_STR_ID_TITLE_EMAIL,					//EMAIL
	E_STR_ID_TITLE_ALARM_HANDING,			//ALARM HANDING
	E_STR_ID_TITLE_BUZZER,					//BUZZER
	E_STR_ID_TITLE_OTHERSETTINGS,			//OTHER SETTINGS
	E_STR_ID_TITLE_SERIAL_PORT,				//SERIAL PORT
	E_STR_ID_TITLE_IMPORT_EXPORT,			//IMPORT / EXPORT
	E_STR_ID_TITLE_BLACK_WHITELIST,         //BLOCK / ALLOW LIST
	E_STR_ID_TITLE_HXHT_NETWORK_PARAM,		//MEGA EYES
	E_STR_ID_TITLE_POS,				//POS

	//@enum 菜单标题
	//@enum 主菜单
	E_STR_ID_MENU_CONFIG	= 0x0c00,		//Setup
	E_STR_ID_MENU_SEARCH,					//Search
	E_STR_ID_MENU_BACKUP,					//Backup
	E_STR_ID_MENU_INFO,						//Information
	E_STR_ID_MENU_MANUAL_ALARM,				//Manual Alarm
	E_STR_ID_MENU_DISK_MAN,					//Disk Management
	E_STR_ID_MENU_UPGRADE,					//Upgrade
	E_STR_ID_MENU_IP_CAMERA,				//IP Camera
	E_STR_ID_MENU_SHUT_DOWN,				//Shut Down
	E_STR_ID_MENU_LOGOFF,					//Logoff

	//@enum 配置菜单
	E_STR_ID_MENU_CFG_BASIC = 0x0c20,		//Basic
	E_STR_ID_MENU_CFG_LIVE,					//Live
	E_STR_ID_MENU_CFG_RECORD,				//Record
	E_STR_ID_MENU_CFG_SCHEDULE,				//Schedule
	E_STR_ID_MENU_CFG_ALARM,				//Alarm
	E_STR_ID_MENU_CFG_NETWORK,				//Network
	E_STR_ID_MENU_CFG_USERS,				//Users
	E_STR_ID_MENU_CFG_PTZ,					//P.T.Z
	E_STR_ID_MENU_CFG_ADVANCED,				//Advanced
	E_STR_ID_MENU_CFG_RESET,				//Reset
	E_STR_ID_MENU_CFG_POS,                                  //POS

	//@enum 报警配置菜单
	E_STR_ID_MENU_CFG_SENSOR = 0x0c40,		//Sensor
	E_STR_ID_MENU_CFG_MOTION,				//Motion
	E_STR_ID_MENU_CFG_VIDEO_LOSS,			//Video Loss
	E_STR_ID_MENU_CFG_OTHER_ALARM,			//Other Alarm
	E_STR_ID_MENU_CFG_ALARM_OUT,			//Alarm Out
	E_STR_ID_MENU_CFG_NET_ALARM,			//Network Alarm

	//@enum 信息菜单
	E_STR_ID_MENU_INFO_SYSTEM = 0x0c60,		//System
	E_STR_ID_MENU_INFO_EVENT,				//Event
	E_STR_ID_MENU_INFO_LOG,					//Log
	E_STR_ID_MENU_INFO_NETWORK,				//Network
	E_STR_ID_MENU_INFO_ONLINE_USER,			//Online Users

	//@enum 菜单项
	E_STR_ID_MENU_CHNN_SINGLE = 0x0c80,		//Single
	E_STR_ID_MENU_CHNN_MULTI,				//Multi
	E_STR_ID_MENU_DWELL,					//Dwell
	E_STR_ID_MENU_AUDIO,					//Audio
	E_STR_ID_MENU_PTZ,						//PTZ
	E_STR_ID_MENU_RECORD,					//Record
	E_STR_ID_MENU_PLAYBACK,					//Playback
	E_STR_ID_MENU_MAIN,						//Main Menu
	E_STR_ID_MENU_DIGITAL_ZOOM,				//E-Zoom
	E_STR_ID_MENU_PIP,                      //PIP

	/************************************************************************/
	/*								日志描述								*/
	/************************************************************************/
	//@enum 系统控制类日志
	E_STR_ID_LOG_BOOT = 0x0d00,				//Turn On the System
	E_STR_ID_LOG_SHUTDOWN,					//Shut Down the System
	E_STR_ID_LOG_REBOOT,					//Reboot the System
	E_STR_ID_LOG_FORMAT_SUCC,				//Format the Disk Successfully!
	E_STR_ID_LOG_FORMAT_FAIL,				//Failed To Format the Disk!
	E_STR_ID_LOG_UPGARDE_SUCC,				//System Upgrade Successfully!
	E_STR_ID_LOG_UPGRADE_FAIL,				//Upgrade Of the System Is Failed!
	E_STR_ID_LOG_CLEAR_ALARM,				//Clear the Alarm
	E_STR_ID_LOG_OPEN_ALARM,				//Enable Alarm
	E_STR_ID_LOG_MANUAL_START,				//Enable Manual Recording
	E_STR_ID_LOG_MANUAL_STOP,				//Disable Manual Recording
	E_STR_ID_LOG_PTZ_ENTER,					//Start P.T.Z Operation
	E_STR_ID_LOG_PTZ_CTRL,					//Operate P.T.Z
	E_STR_ID_LOG_PTZ_EXIT,					//Stop P.T.Z Operation
	E_STR_ID_LOG_AUDIO_CH_CHAE,				//Change Audio Channel
	E_STR_ID_LOG_VOLUME_ADJUST,				//Adjust Volume
	E_STR_ID_LOG_MUTE_ENABLE,				//Enable Mute
	E_STR_ID_LOG_MUTE_DISABLE,				//Disable Mute
	E_STR_ID_LOG_DWELL_ENABLE,				//Enable Loopout
	E_STR_ID_LOG_DWELL_DISABLE,				//Disable Loopout
	E_STR_ID_LOG_LOG_ON,					//Logon
	E_STR_ID_LOG_LOG_OFF,					//Logoff
	E_STR_ID_LOG_MODIFY_TIME,				//Modify System Time
	E_STR_ID_LOG_MANUAL_SNAP_SUC,           //Succeed to snap picture.
	E_STR_ID_LOG_MANUAL_SNAP_FAI,           //Failed to snap picture.

	//@enum 系统配置类日志
	E_STR_ID_LOG_CHGE_VIDEO_FORMAT = 0x0d40,	//Change Video Format
	E_STR_ID_LOG_CHGE_MONITOR,					//Change Video Output
	E_STR_ID_LOG_CHGE_LANGUAGE,					//Change Language
	E_STR_ID_LOG_CHGE_NET_USER_NUM,				//Change Limit Of Online Users
	E_STR_ID_LOG_CHGE_TIME_ZONE,				//Change Time Zone
	E_STR_ID_LOG_MANUAL_SYNC_NTP,				//Synchronize With Time Server Manually
	E_STR_ID_LOG_NTP_ON,						//Enable Auto Sync With Time Server
	E_STR_ID_LOG_NTP_OFF,						//Disable Auto Sync With Time Server
	E_STR_ID_LOG_CHGE_NTP_SERVER,				//Modify Address Of Time Server
	E_STR_ID_LOG_CHGE_DST,						//Modify DST Settings
	E_STR_ID_LOG_PASSWORD_ON,					//Enable Password Check
	E_STR_ID_LOG_PASSWORD_OFF,					//Disable Password Check

	E_STR_ID_LOG_CHGE_CAM_NAME,					//Change Camera Name
	E_STR_ID_LOG_MODIFY_COLOR,					//Adjust Color Settings
	E_STR_ID_LOG_CHGE_HOST_MONITOR,				//Adjust Display Settings Of Main Monitor
	E_STR_ID_LOG_CHGE_SPOT,						//Adjust Display Settings Of Spot Monitor
	E_STR_ID_LOG_CHGE_OSD,						//Adjust Settings For Labels On Screen

	E_STR_ID_LOG_CHGE_LOCAL_ENCODE,				//Adjust Recording(main-stream) Parameters
	E_STR_ID_LOG_CHGE_REC_VIDEO_SWITCH,			//Change Settings To Switch On/Off Recording
	E_STR_ID_LOG_CHGE_REC_AUDIO_SWITCH,			//Change Settings To Switch On/Off Audio Recording
	E_STR_ID_LOG_CHGE_REC_REDU_SWITCH,			//Change Settings To Enable/Disable Redundant Recording
	E_STR_ID_LOG_CHGE_PRE_TIME,					//Change Pre-alarm Time
	E_STR_ID_LOG_CHGE_POST_TIME,				//Change Post-alarm Time
	E_STR_ID_LOG_CHGE_HOLD_TIME,				//Change Expire Of Record

	E_STR_ID_LOG_CHGE_SCH_SCHEDULE,				//Modify Schedule Of Auto Recording
	E_STR_ID_LOG_CHGE_SCH_MOTION,				//Modify Schedule Of Recording Triggered By Motion
	E_STR_ID_LOG_CHGE_SCH_ALARM,				//Modify Schedule Of Recording Triggered By Sensor

	E_STR_ID_LOG_CHGE_SENSOR_SWITCH,			//Change Settings To Switch On/Off Sensor
	E_STR_ID_LOG_CHGE_SENSOR_TYPE,				//Change Type Of Sensor Devices
	E_STR_ID_LOG_CHGE_SENSOR_TRIGGER,			//Change Settings To Handle Sensor Alarm
	E_STR_ID_LOG_CHGE_SENSOR_SCH,				//Modify Schedule Of Sensor Alarm Triggering

	E_STR_ID_LOG_CHGE_MOTION_SWITCH,			//Change Settings To Switch On/Off Motion Detection
	E_STR_ID_LOG_CHGE_MOTION_SENS,				//Change Sensitivity Of Motion Detection
	E_STR_ID_LOG_CHGE_MOTION_AREA,				//Modify Area For Motion Detection
	E_STR_ID_LOG_CHGE_MOTION_TRIGGER,			//Change Settings To Handle Motion Detection
	E_STR_ID_LOG_CHGE_MOTION_SCH,				//Modify Schedule Of Motion Detection

	E_STR_ID_LOG_CHGE_VL_TRIGGER,				//Modify Settings To Handle Video Loss

	E_STR_ID_LOG_CHGE_RELAY_SWITCH,				//Change Settings To Switch On/Off Alarm Out
	E_STR_ID_LOG_CHGE_RELAY_SCH,				//Modify Schedule Of Alarm Out

	E_STR_ID_LOG_BUZZER_ON,						//Enable Buzzer Alarm
	E_STR_ID_LOG_BUZZER_OFF,					//Disable Buzzer Alarm
	E_STR_ID_LOG_CHGE_BUZZER_SCH,				//Modify Schedule Of Buzzer Alarm

	E_STR_ID_LOG_CHGE_HTTP_PORT,				//Change HTTP port
	E_STR_ID_LOG_CHGE_SER_PORT,					//Change Server Port
	E_STR_ID_LOG_CHGE_IP,						//Set Static IP
	E_STR_ID_LOG_CHGE_DHCP_SUCC,				//Succeed To Obtain IP Automaticly
	E_STR_ID_LOG_CHGE_DHCP_FAIL,				//Failed To Obtain IP Automaticly
	E_STR_ID_LOG_CHGE_PPPOE,					//Set PPPoE
	E_STR_ID_LOG_CHGE_DDNS,						//Set DDNS

	E_STR_ID_LOG_CHGE_NET_ENCODE,				//Change Settings Of Sub-stream

	E_STR_ID_LOG_CHGE_SERIAL,					//Change Settings Of Series Port Of P.T.Z
	E_STR_ID_LOG_MODIFY_PRESET,					//Modify P.T.Z Preset
	E_STR_ID_LOG_MODIFY_CRUISE,					//Modify Cruise Of P.T.Z
	E_STR_ID_LOG_MODIFY_TRACK,					//Modify Track Of P.T.Z

	E_STR_ID_LOG_ADD_USER,						//Add User
	E_STR_ID_LOG_MODIFY_USER,					//Change User Authorisation 
	E_STR_ID_LOG_DELETE_USER,					//Delete User
	E_STR_ID_LOG_CHGE_PASSWORD,					//Change User Password

	E_STR_ID_LOG_LOAD_DEFAULT,					//Load Default Config
	E_STR_ID_LOG_IMPORT_CONFIG,					//Import Config
	E_STR_ID_LOG_EXPORT_CONFIG,					//Export Config
	E_STR_ID_LOG_CHGE_SEND_EMAIL,               //Change Settings Of Outbox
	E_STR_ID_LOG_CHG_RECV_EMAIL,                //Change Settings Of Inbox
	E_STR_ID_LOG_CHG_SNAP_SETTING,              //Change Settings of Snap

	E_STR_ID_LOG_STORAGE_NET,					//Set storage network config
	E_STR_ID_LOG_STORAGE_NAME,					//Set storage name
	E_STR_ID_LOG_STORAGE_ID,					//Set storage ID

	//@enum 回放控制类日志
	E_STR_ID_LOG_PB_PLAY  = 0x0e00,				//Play
	E_STR_ID_LOG_PB_PAUSE,						//Pause
	E_STR_ID_LOG_PB_RESUME,						//Resume
	E_STR_ID_LOG_PB_FF,							//FF
	E_STR_ID_LOG_PB_REW,						//REW
	E_STR_ID_LOG_PB_STOP,						//Stop

	//@enum 备份类日志
	E_STR_ID_LOG_BACKUP_START = 0x0e10,			//Start Backup
	E_STR_ID_LOG_BACKUP_COMPLETE,				//Backup Finished
	E_STR_ID_LOG_BACKUP_CANCEL,					//Cancel Backup
	E_STR_ID_LOG_BACKUP_FAIL,					//Backup Failed

	//@enum 检索类日志
	E_STR_ID_LOG_SEARCH_TIME = 0x0e20,			//Time Search
	E_STR_ID_LOG_SEARCH_EVENT,					//Event Search
	E_STR_ID_LOG_SEARCH_FILE_MAN,				//File Management
	E_STR_ID_LOG_DELETE_FILE,					//Delete the File
	E_STR_ID_LOG_LOCK_FILE,						//Lock the File
	E_STR_ID_LOG_ULOCK_FILE,					//Unlock the File
	E_STR_ID_LOG_DELETE_PICTURE,				//Delete the picture
	E_STR_ID_LOG_LOCK_PICTURE,					//Lock the picture
	E_STR_ID_LOG_ULOCK_PICTURE,					//Unlock the picture
	E_STR_ID_LOG_SEARCH_TEXT,					//Text Search

	//@enum 查看系统信息日志
	E_STR_ID_LOG_VIEW_SYSTEM = 0x0e30,			//Check System Info
	E_STR_ID_LOG_VIEW_EVENT,					//Check Event
	E_STR_ID_LOG_VIEW_LOG,						//Check Log
	E_STR_ID_LOG_VIEW_NETWORK,					//Check Network Status
	E_STR_ID_LOG_VIEW_ON_LINE_USERS,			//Check Online Users
	E_STR_ID_LOG_EXPORT_LOG,					//Export Log
	E_STR_ID_LOG_EXPORT_EVENT,					//Export Event

	//@enum 系统异常类日志
	E_STR_ID_LOG_IP_CONFLICT  = 0x0e40,			//IP Conflict
	E_STR_ID_LOG_NETWORK_ERR,					//Network Error
	E_STR_ID_LOG_DDNS_ERR,						//DDNS Error
	E_STR_ID_LOG_DISK_IO_ERR,					//DISK I/O Error
	E_STR_ID_LOG_UNKNOWN_OFF,					//Unexpected Power Off
	E_STR_ID_LOG_UNKNOWN_ERR,					//Unknown Error
	E_STR_ID_LOG_DISK_WARNING,                  //Disk Attenuation
	E_STR_ID_LOG_DISK_DISCONNECT,               //Disk Lost
	E_STR_ID_LOG_DISK_ALARM,					//Disk Alarm
	/*************************************************************************/
	//-----------------------车载相关页面新增部分-----------------------------//
	/*************************************************************************/
	//@section 网络配置页面--WIFI
	//@enum /wifi
	E_STR_ID_WIFI_ENABLE = 0x0e80,				//WIFI Enable
	E_STR_ID_WIFI_CHANNEL,						//Channel
	E_STR_ID_WIFI_SSID,							//SSID
	E_STR_ID_WIFI_SIGNAL,						//Signal
	E_STR_ID_WIFI_ENCRYPT,						//Encrypt State
	E_STR_ID_NETWORK_WIFI,						//WIFI Setup
	E_STR_ID_SEARCH_SIGNAL,						//Search Signal
	E_STR_ID_WIFI_PASSWORD,						//Input Password
	E_STR_ID_WIFI_USE_STATE,					//Use State
	E_STR_ID_SIGNAL_NONE,						//None
	E_STR_ID_SIGNAL_WEAKER,						//Weaker
	E_STR_ID_SIGNAL_WEAK,						//Weak
	E_STR_ID_SIGNAL_GENERAL,					//General
	E_STR_ID_SIGNAL_BATTER,						//Batter
	E_STR_ID_SIGNAL_GOOD,						//Good
	E_STR_ID_SIGNAL_UNLOCK,						//Not Encrypt
	E_STR_ID_SIGNAL_LOCK,						//Encrypt
	E_STR_ID_SIGNAL_USING,						//Using
	E_STR_ID_SIGNAL_UNUSE,						//Unuse
	E_STR_ID_SIGNAL_USED,						//Used
	E_STR_ID_TITLE_IP_SET,						//WIFI IP
	E_STR_ID_MESSAGE_CONNECT_SUCC,				//Connect Succeed
	E_STR_ID_MESSAGE_CONNECT_FAIL,				//Connect Fail

	//@enum/3G
	E_STR_ID_WIRELESS_ENABLE	= 0x0f10,		//Wireless Enable
	E_STR_ID_WIRELESS_TYPE,						//Wireless TYPE
	E_STR_ID_TYPE_EVDO,							//EVDO
	E_STR_ID_TYPE_GPRS,							//GPRS
	E_STR_ID_NETWORK_WIRELESS,					//Wireless Network
	E_STR_ID_WIRELESS_CONNECT,                  //Connect Point
	E_STR_ID_WIRELESS_APN,                      //APN Number
	E_STR_ID_WIRELESS_IP,                       //Wireless IP

	//@section 基本配置页面--机车信息
	//@enum /机车信息
	E_STR_ID_DRIVER_NAME = 0x0f30,				//Driver Name
	E_STR_ID_DRIVER_NUM,						//Driver Number
	E_STR_ID_COMPANY_NAME,						//Company Name
	E_STR_ID_SHUTDOWN_DELAY,					//Shutdown Delay
	E_STR_ID_DRIVERINFO,						//Driver Info
	E_STR_ID_NETSERVER_PORT,					//Network Server Port
	E_STR_ID_NETSERVER_IP,						//Network Server IP
	E_STR_ID_NETWORK_SERVER,					//Network Server


	//@section 速度报警页面
	//@enum /基本信息
	E_STR_ID_SPEED_SOURCE = 0xf3a,				//Speed Source
	E_STR_ID_SPEED_UNIT,						//Speed Unit
	E_STR_ID_SPEED_FOR_GPS,						//GPS
	E_STR_ID_SPEED_FOR_CAR,						//CAR
	E_STR_ID_HIGH_SPEED,						//High Speed
	E_STR_ID_SPEED_VALUE,						//Speed Value
	E_STR_ID_LOW_SPEED,							//Low Speed
	E_STR_ID_SPEED_UNIT_KM,						//KM/H
	E_STR_ID_SPEED_UNIT_MN,						//MN/H
	E_STR_ID_ALARMSPEED,						//SPEED ALARM
	E_STR_ID_MENU_SPEED_ALARM,					//Speed Alarm
	//@section 车载部分标题
	//@enum 标题
	E_STR_ID_TITLE_DRIVERINFO = 0xf80,			//DRIVER INFO
	E_STR_ID_TITLE_WIFI_IP_SET,					//WIFI IP
	E_STR_ID_TITLE_ALARMSPEED,					//SPEED ALARM
	E_STR_ID_TITLE_WIFI_SETUP,					//WIFI SETUP
	E_STR_ID_TITLE_NETWORK_WIRELESS,			//WIRELESS NETWORK
	E_STR_ID_TITLE_NETWORK_SERVER,				//NETWORK SERVER

	/************************************************************************/
	/*								提示消息								*/
	/************************************************************************/
	//@section 提示消息
	//@enum 提示消息
	E_STR_ID_MESSAGE_01 = 0x1000,		//Video format changed, system restarting...
	E_STR_ID_MESSAGE_02,				//xxxx
	E_STR_ID_MESSAGE_03,				//Some records will be lost due to change of time. Continue or not?
	E_STR_ID_MESSAGE_04,				//xxxx
	E_STR_ID_MESSAGE_05,				//xxxx
	E_STR_ID_MESSAGE_06,				//Are you sure to shut down the system?
	E_STR_ID_MESSAGE_07,				//No device or no valid file.
	E_STR_ID_MESSAGE_08,				//Backup failed!
	E_STR_ID_MESSAGE_09,				//Backup completed.
	E_STR_ID_MESSAGE_10,				//Are you sure to remove the file or directory?
	E_STR_ID_MESSAGE_11,				//The directory has exist!
	E_STR_ID_MESSAGE_12,				//Locked file can't be recovered and deleted, are you sure to lock ?
	E_STR_ID_MESSAGE_13,				//Can not be imported into a different model.
	E_STR_ID_MESSAGE_14,				//Can not be imported into a model with different version of firmware.
	E_STR_ID_MESSAGE_15,				//Another user is importing or exporting the configuration.
	E_STR_ID_MESSAGE_16,				//All datas in the disk will be deleted after formatting. Continue or not?
	E_STR_ID_MESSAGE_17,				//All system settings would be changed to factory default after upgrading,are you sure to proceed?
	E_STR_ID_MESSAGE_18,				//Files under writing/playback/read-only/locked can not be deleted. Are you sure to proceed?
	E_STR_ID_MESSAGE_19,				//The hostname is blocked by server due to abuse. Login the server to unblock it.
	E_STR_ID_MESSAGE_20,				//The hostname specified does not exist in the user's account.
	E_STR_ID_MESSAGE_21,				//The hostname specified is not a fully-qualified domain name.
	E_STR_ID_MESSAGE_22,				//Too many hosts (more than 20) specified in an update.
	E_STR_ID_MESSAGE_23,				//IP address or subnet mask is incorrect. Please input again.
	E_STR_ID_MESSAGE_24,				//Upgrade failed.
	E_STR_ID_MESSAGE_25,				//Register account fail!
	E_STR_ID_MESSAGE_26,				//Someone is configuring the system. Try again later.
	E_STR_ID_MESSAGE_27,				//The IP address is not availbale now. Try another.
	E_STR_ID_MESSAGE_28,				//The port number has been used!
	E_STR_ID_MESSAGE_29,				//The wireless device is disconnected.
	E_STR_ID_MESSAGE_30,				//The wireless device has no UIM card.
	E_STR_ID_MESSAGE_31,				//DVR will reboot after importing configuration.Continue or not? 
	E_STR_ID_MESSAGE_32,				//Reset all setting, System will reboot.
	E_STR_ID_MESSAGE_33,				//Failed to format disk. System will reboot.
	E_STR_ID_MESSAGE_34,				//Please select channel!
	E_STR_ID_MESSAGE_35,				//Please select event type.
	E_STR_ID_MESSAGE_36,				//No data.
	E_STR_ID_MESSAGE_37,				//User name or password is incorrect. Please try again.
	E_STR_ID_MESSAGE_38,				//Save the Changes?
	E_STR_ID_MESSAGE_39,				//Can not delete administrator!
	E_STR_ID_MESSAGE_40,				//Are you sure to delete the user?
	E_STR_ID_MESSAGE_41,				//Please choose an user.
	E_STR_ID_MESSAGE_42,				//Can't change authorisation of administrator.
	E_STR_ID_MESSAGE_43,				//Can not add more users.
	E_STR_ID_MESSAGE_44,				//Incorrect MAC, please try again.
	E_STR_ID_MESSAGE_45,				//Succeed to format hard disk.System will reboot.
	E_STR_ID_MESSAGE_46,				//Failed to play.Maybe no data in this day or no disk!
	E_STR_ID_MESSAGE_47,				//Time error!
	E_STR_ID_MESSAGE_48,				//No. of P.T.Z preset is no more than
	E_STR_ID_MESSAGE_49,				//No. of Cruise is no more than
	E_STR_ID_MESSAGE_50,				//No file selected!
	E_STR_ID_MESSAGE_51,				//Wrong time settings!
	E_STR_ID_MESSAGE_52,				//Please choose an IP Camera!
	E_STR_ID_MESSAGE_53,				//Are you sure to delete these IP Camera?
	E_STR_ID_MESSAGE_54,				//Wrong device name. Please try another.
	E_STR_ID_MESSAGE_55,				//Too many IP Camera.
	E_STR_ID_MESSAGE_56,				//DDNS updates successfully.
	E_STR_ID_MESSAGE_57,				//User name or password of DDNS is incorrect.
	E_STR_ID_MESSAGE_58,				//Can not connect the network.
	E_STR_ID_MESSAGE_59,				//Failed to update DDNS. Unknown error!
	E_STR_ID_MESSAGE_60,				//No disc inserted.
	E_STR_ID_MESSAGE_61,				//No free disk space.
	E_STR_ID_MESSAGE_62,				//At lest 2 disks are required. Please change settings and try again.
	E_STR_ID_MESSAGE_63,				//Are you sure to change disk property?
	E_STR_ID_MESSAGE_64,				//Changing disk property. Please wait...
	E_STR_ID_MESSAGE_65,				//Failed to change disk property. System will reboot.
	E_STR_ID_MESSAGE_66,				//No signal.
	E_STR_ID_MESSAGE_67,				//Recording...
	E_STR_ID_MESSAGE_68,				//Failed to format the disk!
	E_STR_ID_MESSAGE_69,				//Formatting finished.
	E_STR_ID_MESSAGE_70,				//Device errors. Refresh it.
	E_STR_ID_MESSAGE_71,				//No IP camera found.
	E_STR_ID_MESSAGE_72,				//No HDD.
	E_STR_ID_MESSAGE_73,				//The time has been successfully synchronized.
	E_STR_ID_MESSAGE_74,				//An error occurred while time synchronizing.
	E_STR_ID_MESSAGE_75,				//Need test ok before save.
	E_STR_ID_MESSAGE_76,				//Test failure, please re-config.
	E_STR_ID_MESSAGE_77,				//You are not authorized to playback!
	E_STR_ID_MESSAGE_78,				//You are not authorized to backup files!
	E_STR_ID_MESSAGE_79,				//You are not authorized to check information!
	E_STR_ID_MESSAGE_80,				//You are not authorized to do the operation!
	E_STR_ID_MESSAGE_81,				//No audio!
	E_STR_ID_MESSAGE_82,				//You are not authorized to control P.T.Z!
	E_STR_ID_MESSAGE_83,				//You are not authorized to shut down the system!
	E_STR_ID_MESSAGE_84,				//You are not authorized to configure this system!
	E_STR_ID_MESSAGE_85,				//You are not authorized to manage the disk!
	E_STR_ID_MESSAGE_86,				//Are you sure to log off?
	E_STR_ID_MESSAGE_87,				//Update of DDNS is time out.
	E_STR_ID_MESSAGE_88,				//Error: User Name, Password, or Host Domain is incorrect.
	E_STR_ID_MESSAGE_89,				//No account is inputed!
	E_STR_ID_MESSAGE_90,				//No host domain is inputed!
	E_STR_ID_MESSAGE_91,				//Checking disk.
	E_STR_ID_MESSAGE_92,				//Failed to control P.T.Z!
	E_STR_ID_MESSAGE_93,				//No valid PTZ protocol found!
	E_STR_ID_MESSAGE_94,				//The Channel is locked.
	E_STR_ID_MESSAGE_95,				//Stop other operation first.
	E_STR_ID_MESSAGE_96,				//No config found!
	E_STR_ID_MESSAGE_97,				//The channel is disabled!
	E_STR_ID_MESSAGE_98,				//Channel is wait...
	E_STR_ID_MESSAGE_99,				//Cruise Preview.
	E_STR_ID_MESSAGE_100,				//No preset on the cruise.
	E_STR_ID_MESSAGE_101,				//Failed to preview the cruise.
	E_STR_ID_MESSAGE_102,				//Found when system checking:
	E_STR_ID_MESSAGE_103,				//Set time zone and time.
	E_STR_ID_MESSAGE_104,				//Another user is configuring remotely. Please try configuration later.
	E_STR_ID_MESSAGE_105,				//Only-readed or writing file can't be locked.
    E_STR_ID_MESSAGE_106,				//Are you sure to unlock file?
	E_STR_ID_MESSAGE_107,				//The preset is disabled!
	E_STR_ID_MESSAGE_108,				//No cruise!
	E_STR_ID_MESSAGE_109,				//Reboot is required to upgrade firmware.It takes about 1min, with blank screen
	E_STR_ID_MESSAGE_110,				//IP address is not valid, please re-config!
	E_STR_ID_MESSAGE_111,				//DDNS config is not valid, please re-config!
	E_STR_ID_MESSAGE_112,				//Another user is setting P.T.Z. Please try later.
	E_STR_ID_MESSAGE_113,				//Can not save.Invalid region.
	E_STR_ID_MESSAGE_114,				//You can save now.
	E_STR_ID_MESSAGE_115,				//Error.Invalid region.
	E_STR_ID_MESSAGE_116,				//Test Email Success
	E_STR_ID_MESSAGE_117,				//Error: Unable to connect to e-mail account.
	E_STR_ID_MESSAGE_118,				//Unknown error!
	E_STR_ID_MESSAGE_119,               //You must restart system before change take effect, yes or no?
	E_STR_ID_MESSAGE_120,               //Someone is formating the disk. Try again later.
	E_STR_ID_MESSAGE_121,               //Someone is backupping files or playbacking. Try again later.
	E_STR_ID_MESSAGE_122,               //Are you sure to delete it or them?
	E_STR_ID_MESSAGE_123,               //Picture under read-only/locked can not be deleted. Are you sure to proceed?
	E_STR_ID_MESSAGE_124,               //Please select a hard disk.
	E_STR_ID_MESSAGE_125,               //Only one hard disk can be browsed at a time.
	E_STR_ID_MESSAGE_126,               //Disk(s) has been regrouped. Please re-assign the channels for the disk group(s).
    E_STR_ID_MESSAGE_127,               //Please confirm you want to upgrade?
	E_STR_ID_MESSAGE_128,               //Important !\nWrite down your new password and keep it in a safe place. Lost passwords will require assistance from Technical Support.
	E_STR_ID_MESSAGE_129,				//Can not format the External storage device!
	E_STR_ID_MESSAGE_130,				//Can not delete locked file!
	E_STR_ID_MESSAGE_131,				//Domain name update successfully
	E_STR_ID_MESSAGE_132,				//Domain must be character or num.
	E_STR_ID_MESSAGE_133,				//Domain length is not allowed(should be between 3 to 16 words).
	E_STR_ID_MESSAGE_134,				//Failed to register DDNS. Unknown error!
	E_STR_ID_MESSAGE_135,				//Domain name already exist!
	E_STR_ID_MESSAGE_136,				//Host Domain deleted successfully!
	E_STR_ID_MESSAGE_137,				//Host Domain deleted fail!
	E_STR_ID_MESSAGE_138,				//Host Domain register successfully!
	E_STR_ID_MESSAGE_139,				//Host Domain replace successfully!
	E_STR_ID_MESSAGE_140,				//Before you enable nat server,make sure you have changed your admin password to avoid Security risks!Do you want to enable nat server?
	E_STR_ID_MESSAGE_141,				//No enough capabilities on digital access! Apply failed!

	//@section 可以接受简单排版的提示消息
	//@enum 提示消息
	E_STR_ID_TYPESET_MSG_01 = 0x1300,	//Format disk following the steps:\n
	E_STR_ID_TYPESET_MSG_02,			//  1- Click "Menu"-"Disk Management;\n
	E_STR_ID_TYPESET_MSG_03,			//  2- Choose unformatted disk, and then click "format";\n
	E_STR_ID_TYPESET_MSG_04,			//  3- Please wait...\n
	E_STR_ID_TYPESET_MSG_05,			//Change disk property following the steps:\n
	E_STR_ID_TYPESET_MSG_06,			//  2.Chose the disk and click "Property" combo box;\n
	E_STR_ID_TYPESET_MSG_07,			//  3.Click "Apply".\n
	E_STR_ID_TYPESET_MSG_08,			//Functions listed below are not active:\n
	E_STR_ID_TYPESET_MSG_09,			//  1- Can not enable recording;\n
	E_STR_ID_TYPESET_MSG_10,			//  2- Can not play back;\n
	E_STR_ID_TYPESET_MSG_11,			//  3- Can not write log.\n
	E_STR_ID_TYPESET_MSG_12,			//Welcome! Please set time zone and time first. Or do it later by click "skip" or "finish".\n
	E_STR_ID_TYPESET_MSG_13,			//Finish setting following the steps:\n
	E_STR_ID_TYPESET_MSG_14,			//  Step 1: choose time zone;
	E_STR_ID_TYPESET_MSG_15,			//  Step 2: set date and time;
	E_STR_ID_TYPESET_MSG_16,			//  Step 3: click "Modify" to activate the change.
	E_STR_ID_TYPESET_MSG_17,			//Click "Yes" to continue or "Cancel" to stop.\n

	//@section Tip提示
	E_STR_ID_TIP_00 = 0x1500,			//xxxx
	E_STR_ID_TIP_00_01,					//Save config successfully.
	E_STR_ID_TIP_00_02,					//Failed to save config!
	E_STR_ID_TIP_00_03,					//Load default successfully.
	E_STR_ID_TIP_00_04,					//Failed to load default!
	E_STR_ID_TIP_00_05,					//Did not change config.
	E_STR_ID_TIP_00_06,					//Continue or not?
	E_STR_ID_TIP_00_07,					//Unknown error.

	
	//@enum tip基本配置页面
	//34时区 from http://en.wikipedia.org/wiki/List_of_time_zones,windows系统
	E_STR_ID_TIP_01 = 0x1600,			//[GMT-12:00]Dateline West
	E_STR_ID_TIP_01_01,					//[GMT-11:00]Midway island,Samoa
	E_STR_ID_TIP_01_02,					//[GMT-10:00]Hawaii
	E_STR_ID_TIP_01_03,					//[GMT-9:00]Alaska
	E_STR_ID_TIP_01_04,					//[GMT-8:00]Tijuana,PST[United States,Canada]
	E_STR_ID_TIP_01_05,					//[GMT-7:00]La Paz,MST[United States,Canada]
	E_STR_ID_TIP_01_06,					//[GMT-6:00]CST[United States,Canada],Central America
	E_STR_ID_TIP_01_07,					//[GMT-5:00]EST[United States,Canada]
	E_STR_ID_TIP_01_08,					//[GMT-4:30]Caracas
	E_STR_ID_TIP_01_09,					//[GMT-4:00]AST[Canada]
	E_STR_ID_TIP_01_10,					//[GMT-3:30]Newfoundland
	E_STR_ID_TIP_01_11,					//[GMT-3:00]Brasilia,Greenland
	E_STR_ID_TIP_01_12,					//[GMT-2:00]Mid-Atlantic
	E_STR_ID_TIP_01_13,					//[GMT-1:00]Cape Verde Islands,Azores
	E_STR_ID_TIP_01_14,					//[GMT]Greenwich Mean Time,WET
	E_STR_ID_TIP_01_15,					//[GMT+1:00]CET,Western Africa
	E_STR_ID_TIP_01_16,					//[GMT+2:00]EET
	E_STR_ID_TIP_01_17,					//[GMT+3:00]Moscow Time
	E_STR_ID_TIP_01_18,					//[GMT+3:30]Tehran
	E_STR_ID_TIP_01_19,					//[GMT+4:00]Caucasia
	E_STR_ID_TIP_01_20,					//[GMT+4:30]Kabul
	E_STR_ID_TIP_01_21,					//[GMT+5:00]Tashkent,Yekaterinburg,Islamabad
	E_STR_ID_TIP_01_22,					//[GMT+5:30]New Delhi
	E_STR_ID_TIP_01_23,					//[GMT+5:45]Kathmandu
	E_STR_ID_TIP_01_24,					//[GMT+6:00]Novosibirsk,Astana,Dhaka
	E_STR_ID_TIP_01_25,					//[GMT+6:30]Yangon
	E_STR_ID_TIP_01_26,					//[GMT+7:00]Bangkok,Hanoi,Jakarta
	E_STR_ID_TIP_01_27,					//[GMT+8:00]Beijing,Hong Kong,Taibei,Singapore
	E_STR_ID_TIP_01_28,					//[GMT+9:00]Tokyo,Seoul,Yakutsk
	E_STR_ID_TIP_01_29,					//[GMT+9:30]Adelaide,Darwin
	E_STR_ID_TIP_01_30,					//[GMT+10:00]AEST
	E_STR_ID_TIP_01_31,					//[GMT+11:00]Solomon Island,New Caledonia
	E_STR_ID_TIP_01_32,					//[GMT+12:00]Wellington,Marshall Islands
	E_STR_ID_TIP_01_33,					//[GMT+13:00]Nukualofa
	E_STR_ID_TIP_01_34,					//Change system time.
	E_STR_ID_TIP_01_35,					//Synchronize with NTP now.
	E_STR_ID_TIP_01_36,					//ID range[0~65535]
	E_STR_ID_TIP_01_37,					//Change the video format,system will reboot.
	E_STR_ID_TIP_01_38,					//Language or video output is changed.Please login again.
	E_STR_ID_TIP_01_39,					//Set for all channels.
	E_STR_ID_TIP_01_40,					//xxxx
	E_STR_ID_TIP_01_41,					//xxxx
	E_STR_ID_TIP_01_42,					//xxxx
	E_STR_ID_TIP_01_43,					//xxxx
	E_STR_ID_TIP_01_44,					//xxxx
	E_STR_ID_TIP_01_45,					//xxxx
	E_STR_ID_TIP_01_46,					//xxxx
	E_STR_ID_TIP_01_47,					//Synchoronize system time automaticly.
	E_STR_ID_TIP_01_48,					//xxxx
	E_STR_ID_TIP_01_49,					//xxxx

	//@enum tip现场配置页面
	E_STR_ID_TIP_02 = 0x1700,			//xxxx
	E_STR_ID_TIP_02_01,					//Mask area setting, max 3 areas.
	E_STR_ID_TIP_02_02,					//Modify live video color
	E_STR_ID_TIP_02_03,					//Do not repeat choosing the same channel!
	E_STR_ID_TIP_02_04,					//xxxx
	E_STR_ID_TIP_02_05,					//Modify live video color for all channels.
	E_STR_ID_TIP_02_06,					//Max Group:
	E_STR_ID_TIP_02_07,					//Current Group:
	E_STR_ID_TIP_02_08,					//Drag and locate captions
	E_STR_ID_TIP_02_09,					//xxxx
	E_STR_ID_TIP_02_10,					//xxxx
	E_STR_ID_TIP_02_11,					//xxxx
	E_STR_ID_TIP_02_12,					//xxxx
	E_STR_ID_TIP_02_13,					//xxxx
	E_STR_ID_TIP_02_14,					//xxxx
	E_STR_ID_TIP_02_15,					//Previous group
	E_STR_ID_TIP_02_16,					//Next group
	E_STR_ID_TIP_02_17,					//Dwell time range[0~60] second

	//@enum tip录像配置页面
	E_STR_ID_TIP_03 = 0x1800,			//xxxx
	E_STR_ID_TIP_03_01,					//Are you sure to change recycle recording setting?
	E_STR_ID_TIP_03_02,					//Exceeding system capacity,framerate adjusted automatically.
	E_STR_ID_TIP_03_03,					//xxxx
	E_STR_ID_TIP_03_04,					//xxxx
	E_STR_ID_TIP_03_05,					//xxxx
	E_STR_ID_TIP_03_06,					//xxxx
	E_STR_ID_TIP_03_07,					//xxxx
	E_STR_ID_TIP_03_08,					//xxxx
	E_STR_ID_TIP_03_09,					//xxxx
	E_STR_ID_TIP_03_10,					//xxxx
	E_STR_ID_TIP_03_11,					//xxxx
	E_STR_ID_TIP_03_12,					//xxxx
	E_STR_ID_TIP_03_13,					//xxxx
	E_STR_ID_TIP_03_14,					//xxxx
	E_STR_ID_TIP_03_15,					//xxxx
	E_STR_ID_TIP_03_16,					//xxxx
	E_STR_ID_TIP_03_17,					//Valid time of record, from 1 to 60 days.
	E_STR_ID_TIP_03_18,					//xxxx
	E_STR_ID_TIP_03_19,					//xxxx
	E_STR_ID_TIP_03_20,					//xxxx
	E_STR_ID_TIP_03_21,					//xxxx
	E_STR_ID_TIP_03_22,					//xxxx
	E_STR_ID_TIP_03_23,					//xxxx
	E_STR_ID_TIP_03_24,					//xxxx
	E_STR_ID_TIP_03_25,					//xxxx

	//@enum tip排程配置页面
	E_STR_ID_TIP_04 = 0x1900,			//xxxx
	E_STR_ID_TIP_04_01,					//Double-click and setup schedule
	E_STR_ID_TIP_04_02,					//Copy channel
	E_STR_ID_TIP_04_03,					//to channel
	E_STR_ID_TIP_04_04,					//to all channel
	E_STR_ID_TIP_04_05,					//OK.
	E_STR_ID_TIP_04_06,					//Click and setup holiday schedule.

	//@enum tip网络配置页面
	E_STR_ID_TIP_05 = 0x2100,			//Register Account
	E_STR_ID_TIP_05_01,					//Test network.
	E_STR_ID_TIP_05_02,					//Test DDNS.
	E_STR_ID_TIP_05_03,					//More than system capacity.Automatically adjust the reasonable value
	E_STR_ID_TIP_05_04,					//The dial-up param error.
	E_STR_ID_TIP_POEIP_CHECK,			//Switch IP and main IP can't in the same network.

	//@enum tip用户配置页面
	E_STR_ID_TIP_06 = 0x2130,			//xxxx
	E_STR_ID_TIP_06_01,					//No user name is inputed or it exists already.Try another.
	E_STR_ID_TIP_06_02,					//User name or password is incorrect.Please try again.
	E_STR_ID_TIP_06_03,					//Passwords are not the same.Try again.
	E_STR_ID_TIP_06_04,					//No user information found.
	E_STR_ID_TIP_06_05,					//Failed to save password.
	E_STR_ID_TIP_06_06,					//xxxx
	E_STR_ID_TIP_06_07,					//User can only change their password when log in.

	//@enum toolbar
	E_STR_ID_TOOLBAR_TIP = 0x2200,			//xxxx
	E_STR_ID_TOOLBAR_TIP_MENU,					//Menu
	E_STR_ID_TOOLBAR_TIP_1X1,					//1 Cam
	E_STR_ID_TOOLBAR_TIP_2X2,					//4 Cam
	E_STR_ID_TOOLBAR_TIP_2X3,					//6 Cam
	E_STR_ID_TOOLBAR_TIP_8X,					//8 Cam
	E_STR_ID_TOOLBAR_TIP_3X3,					//9 Cam
	E_STR_ID_TOOLBAR_TIP_13X,					//13 Cam
	E_STR_ID_TOOLBAR_TIP_4X4,					//16 Cam
	E_STR_ID_TOOLBAR_TIP_32X,					//25 Cam
	E_STR_ID_TOOLBAR_TIP_64X,					//36 Cam
	E_STR_ID_TOOLBAR_TIP_PIP,					//PiP
	E_STR_ID_TOOLBAR_TIP_SEQUENCE,					//Sequence
	E_STR_ID_TOOLBAR_TIP_COLOR,					//Color
	E_STR_ID_TOOLBAR_TIP_ZOOM,					//Zoom
	E_STR_ID_TOOLBAR_TIP_AUDIO,					//Audio
	E_STR_ID_TOOLBAR_TIP_PTZ,					//PTZ
	E_STR_ID_TOOLBAR_TIP_SNAP,					//Snap
	E_STR_ID_TOOLBAR_TIP_RECORD,					//Record
	E_STR_ID_TOOLBAR_TIP_PLAYBACK,					//Playback
	E_STR_ID_TOOLBAR_TIP_MOVE,					//Move
	E_STR_ID_TOOLBAR_TIP_CAM_PARAM,             //Cam Parameter

	//@enum player
	E_STR_ID_PLAYER_TIP_PLAY = 0x2300,			//Play
	E_STR_ID_PLAYER_TIP_PAUSE,					//Pause
	E_STR_ID_PLAYER_TIP_STOP,					//Stop
	E_STR_ID_PLAYER_TIP_FRAME_ADVANCE,			//Frame Adv
	E_STR_ID_PLAYER_TIP_FAST_FORWARD,			//Fast Fwd
	E_STR_ID_PLAYER_TIP_REWIND,					//Rewind
	E_STR_ID_PLAYER_TIP_LIVE_CAMERA,			//Live Cam
	E_STR_ID_PLAYER_TIP_START_CLIP,				//Start Clip
	E_STR_ID_PLAYER_TIP_END_CLIP,				//End Clip
	E_STR_ID_PLAYER_TIP_BACKUP,					//Backup
	E_STR_ID_PLAYER_TIP_VOLUME,					//Volume
	E_STR_ID_PLAYER_TIP_HIDE_CONTROLS,			//Hide Controls
	E_STR_ID_PLAYER_TIP_EXIT,					//Exit
	E_STR_ID_PLAYER_TIP_PREVIOUS_RECORD,		//Previous Record
	E_STR_ID_PLAYER_TIP_NEXT_RECORD,			//Next Record
	


	//@section US02向导专用提示
	//@enum US02向导
	E_STR_ID_US02_WZD_STARTUP = 0x3000,	//Q-See Start Up Wizard
	E_STR_ID_US02_WZD_STARTUPFOR,		//Start Up Wizard for:
	E_STR_ID_US02_WZD_OPENAGAIN,		//Open Wizard Next Time
	E_STR_ID_US02_WZD_FIRSTTIP01,		//1. Language
	E_STR_ID_US02_WZD_FIRSTTIP02,		//2. Date-Time Setup
	E_STR_ID_US02_WZD_FIRSTTIP03,		//3. Network / Internet Setup
	E_STR_ID_US02_WZD_FIRSTTIP04,		//4. Mobile Devices Connection
	E_STR_ID_US02_WZD_FIRSTTIP05,		//5. DDNS setup
	E_STR_ID_US02_WZD_SECONDTIP01,		//Select Language: 
	E_STR_ID_US02_WZD_NETWORKSETUP,		//Network Setup
	E_STR_ID_US02_WZD_FIFTHTIP01,		//The step will be to setup the network connection so you can view your cameras over your network or the Internet.
	E_STR_ID_US02_WZD_FIFTHTIP02,		//Please connect the Network Cable from the DVR to the Router.
	E_STR_ID_US02_WZD_FIFTHTIP03,		//NOTE: Please ensure that there is only one router in your network.
	E_STR_ID_US02_WZD_FIFTHTIP04,		//If there are multiple routers, this wizard will not work. 
	E_STR_ID_US02_WZD_FIFTHTIP05,		//Please consult Section 1.7 of the Remote Monitoring Guide on the CD.
	E_STR_ID_US02_WZD_NETWORKSTATUS,	//Network Status
	E_STR_ID_US02_WZD_GETIPADDRESS,		//Getting IP Address
	E_STR_ID_US02_WZD_DVRIPADDRESS,		//DVR IP address 
	E_STR_ID_US02_WZD_DONE,				//Done
	E_STR_ID_US02_WZD_FAILED,			//FAILED
	E_STR_ID_US02_WZD_SIXTIP01,			//Note: To connect to the DVR from a Local PC, please use Internet Explorer.
	E_STR_ID_US02_WZD_SIXTIP02,			//Use the Safari browser for Macintosh.
	E_STR_ID_US02_WZD_SIXTIP03,			//Enter this IP address in the address bar:
	E_STR_ID_US02_WZD_Note,				//Note: 
	E_STR_ID_US02_WZD_SIXERRTIP01,		//1. Please check the Network cable connection
	E_STR_ID_US02_WZD_SIXERRTIP02,		//2. Please check if the Router has DHCP enabled.
	E_STR_ID_US02_WZD_SIXERRTIP03,		//3. Please check the Remote Monitoring Guide on the CD
	E_STR_ID_US02_WZD_CHECKUPNPSUCC,	//Checking Router for UPNP Function......OK
	E_STR_ID_US02_WZD_CHECKUPNPFAIL,	//Checking Router for UPNP Function......Failed
	E_STR_ID_US02_WZD_SEVENTIP01,		//UPNP:Universal Plug and Play.
	E_STR_ID_US02_WZD_SEVENTIP02,		//UPNP allows the DVR and your router to discover and communicate with each other on your Network so that you can access your DVR from outside of the network through the router.
	E_STR_ID_US02_WZD_SEVENERRTIP01,	//1. Please check if your router support UPnP and it has been enabled.
	E_STR_ID_US02_WZD_SEVENERRTIP02,	//2. If you have a 2 Wire  router or multiple routers, this Wizard will not work.
	E_STR_ID_US02_WZD_SEVENERRTIP03,	//Please consult the Remote Monitoring Guide on the CD.
	E_STR_ID_US02_WZD_EIGHTTIP01,		//Openning http port
	E_STR_ID_US02_WZD_EIGHTTIP02,		//and server port
	E_STR_ID_US02_WZD_EIGHTTIP03,		//....success
	E_STR_ID_US02_WZD_CHECKIPSUCC,		//Checking Internet IP address....success
	E_STR_ID_US02_WZD_CHECKIPERROR,		//Checking Internet IP address....ERROR
	E_STR_ID_US02_WZD_NINETIP01,		//Internet IP address is 
	E_STR_ID_US02_WZD_NINETIP02,		//Note: To connect to the DVR from outside the Netowrk/Internet on a PC,please use Internet Explorer.
	E_STR_ID_US02_WZD_TENTIP01,			//To Connect from Mobile Device: 
	E_STR_ID_US02_WZD_TENTIP02,			//Select your Mobile Device:
	E_STR_ID_US02_WZD_APPLE,			//Apple
	E_STR_ID_US02_WZD_ANDROID,			//Android
	E_STR_ID_US02_WZD_TENTIP03,			//For other Mobile Devices : Blackberry, Symbian and Windows Phone,Please look for the instructions on the Remote Monitoring Guide.
	E_STR_ID_US02_WZD_APPTIP01,			//Apple Mobile Device:
	E_STR_ID_US02_WZD_APPTIP02,			//Iphone: download QT View from Appstore
	E_STR_ID_US02_WZD_APPTIP03,			//Ipad: Download QT View HD from Appstore
	E_STR_ID_US02_WZD_ELEVENTTIP04,		//Enter this informations:
	E_STR_ID_US02_WZD_ELEVENTTIP05,		//Server
	E_STR_ID_US02_WZD_ELEVENTTIP06,		//Account:admin
	E_STR_ID_US02_WZD_ELEVENTTIP07,		//Password: 123456
	E_STR_ID_US02_WZD_ELEVENTTIP08,		//Account and Password above is default,if you change the password,please use your own password when enter the information in the App.
	E_STR_ID_US02_WZD_ANDROIDTIP01,		//Android Mobile Device:
	E_STR_ID_US02_WZD_ANDROIDTIP02,		//Android Phone: download QT View from Google Play
	E_STR_ID_US02_WZD_ANDROIDTIP03,		//Android Tablet: Download QT View HD from Google Play
	E_STR_ID_US02_WZD_DDNS01,			//DDNS Setup.
	E_STR_ID_US02_WZD_DDNS02,			//Unless you have paid for a static IP from your service provider, address can change and will cause remote connection issues.
	E_STR_ID_US02_WZD_DDNS03,			// To avoid this problem, Q-See offers a free DDNS service. 
	E_STR_ID_US02_WZD_DDNS04,			//Please go to www.myq-see.com  using a computer on the same network as the DVR and create a new account Once you have completed your registration, write down the User Name, Password and Domain Name.
	E_STR_ID_US02_WZD_DDNS05,			// Enter that information here:  
	E_STR_ID_US02_WZD_13_01,			//You can use your new domain name to connect to your DVR from outside of your local network using the Internet, Enter the domain name shown below when you are using a computer or mobile device. 
	E_STR_ID_US02_WZD_13_02,			//Please note that you will still need to use your local IP address when connecting from within your network.
	E_STR_ID_US02_WZD_14_01,			//Q-See Startup Wizard is finished.
	E_STR_ID_US02_WZD_14_02,			//Please disable the Startup Wizard by unchecking the "Open Wizard Next Time" box below so that it does not run again the next time  when you start your system.


	//@section US02新向导提示
	//@enum US02新向导
	E_STR_ID_US02_NEW_WZD_START_00 = 0x3500,		//Welcome.
	E_STR_ID_US02_NEW_WZD_START_01,		//Start Up Wizard for:
	E_STR_ID_US02_NEW_WZD_START_02,		//1. Language
	E_STR_ID_US02_NEW_WZD_START_03,		//2. Date and Time
	E_STR_ID_US02_NEW_WZD_START_04,		//3. Password
	E_STR_ID_US02_NEW_WZD_START_05,		//4. Local Connection and Remote Access
	E_STR_ID_US02_NEW_WZD_TIME_01,		//Auto Sync Time (Internet connection required)
	E_STR_ID_US02_NEW_WZD_TIME_02,		//Internet Time Server
	E_STR_ID_US02_NEW_WZD_PASSWORD_00,	//Before continuing, it is strongly recommended that you change your Admin default password from  "123456" to protect your system.
	E_STR_ID_US02_NEW_WZD_PASSWORD_01,	//If you do not wish to change your password at this time, please press Next.
	E_STR_ID_US02_NEW_WZD_PASSWORD_02,	//IMPORTANT! Please keep your new password in a safe location. Lost passwords will require assistance from Q-See Tech Support.
	E_STR_ID_US02_NEW_WZD_PASSWORD_03,	//Do not find user name,please Enter again.
	E_STR_ID_US02_NEW_WZD_PASSWORD_04,	//The confirm password dosen't match the new password,please Enter again.
	E_STR_ID_US02_NEW_WZD_PASSWORD_05,	//Original password error
	E_STR_ID_US02_NEW_WZD_ACCESS_TIP_00,	//The next step will take you through the Remote Access setup process.
	E_STR_ID_US02_NEW_WZD_ACCESS_TIP_01,	//Connect the network cable from the DVR to the Router.
	E_STR_ID_US02_NEW_WZD_NETWORK_00,		//Checking Network IP address......Success
	E_STR_ID_US02_NEW_WZD_NETWORK_01,		//Your DVR is connected to your network.
	E_STR_ID_US02_NEW_WZD_NETWORK_02,		//The local IP address for this DVR is
	E_STR_ID_US02_NEW_WZD_NETWORK_03,		//If you will be connecting to your DVR away from your local network (over the Internet), please click Next to continue with the Q-See Scan N'VIEW.
	E_STR_ID_US02_NEW_WZD_NETWORK_04,		//If you will ONLY be connecting to your DVR through your local network, select Local Connection Only below and then click Next to continue. Otherwise, just click Next to continue.
	E_STR_ID_US02_NEW_WZD_NETWORK_05,		//Local Connection with Remote Access
	E_STR_ID_US02_NEW_WZD_NETWORK_06,		//Local Connection
	E_STR_ID_US02_NEW_WZD_LOCAL_END_00,		//The local IP address for this DVR is http:
	E_STR_ID_US02_NEW_WZD_LOCAL_END_01,		//if you will ONLY be connecting to your DVR through your local network, and not over the internet,use the local IP address above to connect to your DVR on a computer connected to the same network by entering it into the address bar in the Explorer (PC) or Safari (Mac) browser window.
	E_STR_ID_US02_NEW_WZD_LOCAL_END_02,		//You may also enter it into the QT View app on a mobile device using your WiFi network.
	E_STR_ID_US02_NEW_WZD_LOCAL_END_03,		//This address will not work when you are away from the building where your DVR is located.
	E_STR_ID_US02_NEW_WZD_LOCAL_END_04,		//Clicking Exit will end this wizard.
	E_STR_ID_US02_NEW_WZD_SELECT_DEV_00,	//Mobile Setup
	E_STR_ID_US02_NEW_WZD_SELECT_DEV_01,	//Computer Setup
	E_STR_ID_US02_NEW_WZD_SELECT_DEV_02,	//Q-See Scan N'VIEW
	E_STR_ID_US02_NEW_WZD_SELECT_DEV_03,	//The next step will be to set up the network connection so you can view your cameras over your network and the Internet.
	E_STR_ID_US02_NEW_WZD_SELECT_DEV_04,	//Choose whether you want to connect via Mobile Devices or Computer.
	E_STR_ID_US02_NEW_WZD_QRCODE_00,		//Mobile Devices Setup for Android and Apple:
	E_STR_ID_US02_NEW_WZD_QRCODE_01,		//Phone: Download QT View App from Play Store (Android) or App Store (Apple)
	E_STR_ID_US02_NEW_WZD_QRCODE_02,		//Tablet: Download QT View HD App from Play Store (Android) or App Store (Apple)
	E_STR_ID_US02_NEW_WZD_QRCODE_03,		//1.Launch QT View app and click on the Bar Code Icon at the bottom
	E_STR_ID_US02_NEW_WZD_QRCODE_04,		//2.Scan this QR Code
	E_STR_ID_US02_NEW_WZD_QRCODE_05,		//3.Enter the Username: admin
	E_STR_ID_US02_NEW_WZD_QRCODE_06,		//4.Enter the DVR Password
	E_STR_ID_US02_NEW_WZD_PC_TIP_00,		//Network Setup for Mac or PC
	E_STR_ID_US02_NEW_WZD_PC_TIP_01,		//To view your camera remotely from your computer:
	E_STR_ID_US02_NEW_WZD_PC_TIP_02,		//1. Go to QTView.com using Internet Explorer (PC) or Safari (Mac).
	E_STR_ID_US02_NEW_WZD_PC_TIP_03,		//2. When you reach the login screen enter this Serial Number:
	E_STR_ID_US02_NEW_WZD_PC_TIP_04,		//3. Enter the Username: admin
	E_STR_ID_US02_NEW_WZD_PC_TIP_05,		//4. Enter your Password
	E_STR_ID_US02_NEW_WZD_PC_TIP_06,		//If you did not change it at the start of this wizard, the default password is 123456 and this should be changed as soon as you finish this wizard to protect your system.
	E_STR_ID_US02_NEW_WZD_PC_TIP_07,		//Click Prew to setup another device. Or, click Next to continue.


	//@section NAT向导提示
	//@enum NAT向导
	E_STR_ID_NAT_WZD_START_00 = 0x4000,     //Welcome.
	E_STR_ID_NAT_WZD_START_01,     //This Start Up Wizard will get your system online quickly and easily so you can access it using your computer or mobile device.
	E_STR_ID_NAT_WZD_START_02,     //You will also be able to choose your preferred language and set the date and time.
	E_STR_ID_NAT_WZD_START_03,     //If you are unable to use the Peer-to-Peer method in this wizard due to network firewalls or other issues, you may follow the instructions on the included Remote Monitoring Quick Start poster to take your system online through other methods.
	E_STR_ID_NAT_WZD_START_04,     //Please note that remote monitoring is only available if your system is connected to the Internet.
	E_STR_ID_NAT_WZD_START_05,     //If you will not be operating your system with an Internet connection, you may exit this wizard after setting the system's date and time.
	E_STR_ID_NAT_WZD_START_06,     //Please connect the Network Cable from the DVR to your router before clicking Next.
	E_STR_ID_NAT_WZD_LANGUAGE_00,	//IMPORTANT!
	E_STR_ID_NAT_WZD_LANGUAGE_01,	//If you change the language selection, your system will need to restart.
	E_STR_ID_NAT_WZD_LANGUAGE_02,	//You can then continue with the setup.
	E_STR_ID_NAT_WZD_LANGUAGE_03,	//Please make your settings in all three tabs above, before clicking Next.
	E_STR_ID_NAT_WZD_LANGUAGE_04,	//Select Language:
	E_STR_ID_NAT_WZD_PASSWORD_00,	//Before continuing, it is strongly recommended that you change your Admin default password from  "123456" to protect your system. 
	E_STR_ID_NAT_WZD_PASSWORD_01,	//If you do not wish to change your password at this time, please press Next.
	E_STR_ID_NAT_WZD_PASSWORD_02,	//IMPORTANT! Please keep your new password in a safe location. Lost passwords will require assistance 
	E_STR_ID_NAT_WZD_PASSWORD_03,	//from Q-See Tech Support.	
	E_STR_ID_NAT_WZD_PASSWORD_04,	//Do not find user name,please Enter again
	E_STR_ID_NAT_WZD_PASSWORD_05,	//Origian password error,please Enter again
	E_STR_ID_NAT_WZD_PASSWORD_06,	//The confirm password dosen't match the new password,please Enter again
	E_STR_ID_NAT_WZD_NETWORK_00,	//Network Status
	E_STR_ID_NAT_WZD_NETWORK_01,	//Getting IP Address
	E_STR_ID_NAT_WZD_NETWORK_02,	//DVR IP Address
	E_STR_ID_NAT_WZD_NETWORK_03,	//Checking Network IP address......Failure
	E_STR_ID_NAT_WZD_NETWORK_04,	//Your DVR is not connected to your network.
	E_STR_ID_NAT_WZD_NETWORK_05,	//TROUBLESHOOTING:
	E_STR_ID_NAT_WZD_NETWORK_06,	//Please ensure that the Network Cable is properly connecting your DVR to your Router.
	E_STR_ID_NAT_WZD_NETWORK_07,	//Please make sure that your Router has DHCP enabled.
	E_STR_ID_NAT_WZD_NETWORK_08,	//(Consult your service provider or your router's manual).
	E_STR_ID_NAT_WZD_NETWORK_09,	//Alternative connection methods may be found on the Remote Monitoring Quick Start poster.
	E_STR_ID_NAT_WZD_NETWORK_0a,	//Additional resources may be found in Section 1.7 of the Remote Monitoring Guide on the CD.
	E_STR_ID_NAT_WZD_NETWORK_0b,	//Click on Prev to go back and retry your connection.
	E_STR_ID_NAT_WZD_NETWORK_0c,	//Local Connection with Remote Access
	E_STR_ID_NAT_WZD_NETWORK_0d,	//Local Connection
	E_STR_ID_NAT_WZD_NETWORK_0e,	//Checking Network IP address......Success
	E_STR_ID_NAT_WZD_NETWORK_0f,	//Your DVR is connected to your network.
	E_STR_ID_NAT_WZD_NETWORK_10,	//The local IP address for this DVR is
	E_STR_ID_NAT_WZD_NETWORK_11,	//http://
	E_STR_ID_NAT_WZD_NETWORK_12,	//If you will be connecting to your DVR away from your local network (over the Internet), please click Next to continue with the Q-See Scan N'VIEW.
	E_STR_ID_NAT_WZD_NETWORK_13,	//If you will ONLY be connecting to your DVR through your local network, select Local Connection Only below and then click Next to continue. Otherwise, just click Next to continue.
	E_STR_ID_NAT_WZD_NETWORK_14,	//FAILED
	E_STR_ID_NAT_WZD_NETWORK_15,	//SUCCESS	
	E_STR_ID_NAT_WZD_P2P_00,	//Peer-to-Peer setup
	E_STR_ID_NAT_WZD_P2P_01,	//The next step will be to set up the network connection so you can view your cameras over your network and the Internet.
	E_STR_ID_NAT_WZD_P2P_02,	//Choose whether you want to connect via computer, Apple or Android mobile device.
	E_STR_ID_NAT_WZD_P2P_03,	//You will be able to come back to this window to choose another option.
	E_STR_ID_NAT_WZD_P2P_04,	//Computer
	E_STR_ID_NAT_WZD_P2P_05,	//Android
	E_STR_ID_NAT_WZD_P2P_06,	//Apple	
	E_STR_ID_NAT_WZD_QR_00,		//Network Setup for Android:
	E_STR_ID_NAT_WZD_QR_01,		//Phone: Download QT View from Android Store
	E_STR_ID_NAT_WZD_QR_02,		//Tablet: Download QT View HD from Android Store
	E_STR_ID_NAT_WZD_QR_03,		//Network Setup for Apple iOS:
	E_STR_ID_NAT_WZD_QR_04,		//iPhone: Download QT View from App Store
	E_STR_ID_NAT_WZD_QR_05,		//iPad: Download QT View HD from App Store
	E_STR_ID_NAT_WZD_QR_06,		//1. Launch QT View app and click on the Bar Code Icon at the bottom.
	E_STR_ID_NAT_WZD_QR_07,		//2. Scan this QR Code
	E_STR_ID_NAT_WZD_QR_08,		//3. Enter the Username: admin
	E_STR_ID_NAT_WZD_QR_09,		//4. Enter your Password
	E_STR_ID_NAT_WZD_QR_0a,		//If you did not change it at the start of this wizard, the default password is 123456 and this should be changed as soon as you finish this wizard to protect your system.
	E_STR_ID_NAT_WZD_QR_0b,		//Click Prev to setup another device. Or, click Next to continue.	
	E_STR_ID_NAT_WZD_PC_TIP_00,		//Network Setup for Mac or PC
	E_STR_ID_NAT_WZD_PC_TIP_01,		//To view your camera remotely from your computer:
	E_STR_ID_NAT_WZD_PC_TIP_02,		//1. Go to QTView.com using Internet Explorer (PC) or Safari (Mac).
	E_STR_ID_NAT_WZD_PC_TIP_03,		//2. When you reach the login screen enter this Serial Number:
	E_STR_ID_NAT_WZD_PC_TIP_04,		//3. Enter the Username: admin
	E_STR_ID_NAT_WZD_PC_TIP_05,		//4. Enter your Password
	E_STR_ID_NAT_WZD_PC_TIP_06,		//If you did not change it at the start of this wizard, the default password is 123456 and this should be changed as soon as you finish this wizard to protect your system.
	E_STR_ID_NAT_WZD_PC_TIP_07,		//Click Prev to setup another device. Or, click Next to continue.	
	E_STR_ID_NAT_WZD_END_00,		//The Q-See Start Up Wizard is finished.
	E_STR_ID_NAT_WZD_END_01,		//Please disable the Start Up Wizard by unchecking the "Open Wizard Next Time" box in the lower left corner, below, so it does not run again the next time you restart the DVR.
	E_STR_ID_NAT_WZD_END_02,		//The QR code and other networking information may be found in the Information window.
	E_STR_ID_NAT_WZD_END_03,		//Open Wizard Next Time




	//@section 其他
	//@enum 其他
	E_STR_ID_PLEASE_WAIT = 0x5000,		//Please wait ...
	E_STR_ID_SAFELY_TURN_OFF,			//You can safely turn off the power now.
	E_STR_ID_SYSTEM_RESTART_MINUTES,	//System will reboot in seconds.
	E_STR_ID_HARD_DISK_ERROR,			//Hard disk IO error.
	E_STR_ID_EMAIL_SUBJECT1,            //Alarm Message From Device %s.
	E_STR_ID_EMAIL_SUBJECT2,            //Test Email Message From Device %s.
	E_STR_ID_EMAIL_CH_ID,               //Channel ID
	E_STR_ID_EMAIL_CH_NAME,				//Camera Name
	E_STR_ID_EMAIL_ALARM_TYPE,          //Alarm Type
	E_STR_ID_EMAIL_ALARM_TIME,          //Time
	E_STR_ID_EMAIL_MOTION_ALARM,        //Motion Alarm
	E_STR_ID_EMAIL_SENSOR_ALARM,        //Sensor Alarm
	E_STR_ID_EMAIL_LOSS_ALARM,          //Video loss Alarm
	E_STR_ID_EMAIL_TEST_ALARM,          //Test SMTP Settings
	E_STR_ID_EMAIL_DISKFULL_ALARM,      //Disk Full Alarm
	E_STR_ID_EMAIL_IPCONF_ALARM,        //IP Conflict Alarm
	E_STR_ID_EMAIL_DISCONNET_ALARM,     //Disconnection Alarm
	E_STR_ID_EMAIL_OTHER_ALARM,         //Other Alarm
	E_STR_ID_IE_LOG_TIP_FIRST,			//First
	E_STR_ID_IE_LOG_TIP_PRE,			//Pre
	E_STR_ID_IE_LOG_TIP_NEXT,			//Next
	E_STR_ID_IE_LOG_TIP_LAST,			//Last
	E_STR_ID_IE_LOG_TIP_RECORD,			//Records
	E_STR_ID_IE_LOG_TIP_ISTHEFIRST,		//It is the first page!
	E_STR_ID_IE_LOG_TIP_ISTHELAST,		//It is the last page!
	E_STR_ID_NETWORK_TYPE,              //Network Type
	E_STR_ID_WIRED_NETWORK,             //Wired Network
	E_STR_ID_WIRELESS_NETWORK,          //Wireless Network
	E_STR_ID_CHANGE_NET_TYPE,           //System will reboot due to change the network type. Continue or not ?
	E_STR_ID_SEQUENCE,                  //Sequence
	E_STR_ID_AUDIO_EDIT,                //Audio Edit
	E_STR_ID_SEQ_USED,                  //The sequence has been used.
	E_STR_ID_NO_EXCEED,                 //The total size cannot exceed 512 MB.
	E_STR_ID_EDIT,                      //Edit
	E_STR_ID_KEEP,                      //Keep
	E_STR_ID_VIDEO_AUDIO_EDIT,          //Video and Audio Editing Information
	E_STR_ID_BAK_GENERATED,             //Generated on the %s at %s by %s
	E_STR_ID_BAK_FILENAME,              //File Name
	E_STR_ID_BAK_RECORDER,              //Recorder Type
	E_STR_ID_BAK_MAC,                   //MAC Address
	E_STR_ID_BAK_CHID,                  //Camera No.
	E_STR_ID_BAK_CHNAME,                //Camera Name
	E_STR_ID_BAK_START,                 //Start Time
	E_STR_ID_BAK_END,                   //End Time 
	E_STR_ID_BAK_TIMELENG,              //Duration 
	E_STR_ID_BAK_SIZE,                  //Size
	E_STR_ID_BAK_OPERLOG,               //Operation Log
	E_STR_ID_BAK_EVENTLOG,              //Event Log
	E_STR_ID_SEARCH_GROUP,              //Group
	E_STR_ID_SEARCH_GROUP_NAME,         //Group Name:
	E_STR_ID_SEARCH_SEL_CH,             //Select Channels:
	E_STR_ID_SEARCH_DEL_GROUP,          //Are you sure to delete the group?
	E_STR_ID_SEARCH_INPUT_NAME,         //Please input the group name.
	E_STR_ID_SEARCH_INPUT_AGAIN,        //The group name already exists. Try another.
	E_STR_ID_SENSOR_NAME,               //Sensor Name
	E_STR_ID_VIEW_MENU,					//To view menu:\nPush and hold <ESC> button until\nyou hear a beep.
	E_STR_ID_CHANGE_SWITCHIP,           //System will reboot due to change the switch ip. Continue or not ?
	//@enum		IPCamera

	E_STR_ID_IPCAM_CONNECTING = 0x5500,	//Connecting...
	E_STR_ID_IPCAM_CONNECTED,			//Connected
	E_STR_ID_IPCAM_DISCONNECTED,		//Disconnected
	E_STR_ID_IPCAM_MANUFACTURER,		//Manufacturer
	E_STR_ID_IPCAM_PRODUCT_MODEL,		//Product Model
	E_STR_ID_IPCAM_CONTROL_PORT,		//Control Port
	E_STR_ID_IPCAM_NO_PRODUCT_SEL,		//No product is selelected!
	E_STR_ID_IPCAM_IP_INVALID,			//The IP is not valid!
	E_STR_ID_IPCAM_USER_NAME_EMPTY,		//User name shouldn't be empty!


	E_STR_ID_LOG_IPCAM_ADD,				//Add IPCamera Device
	E_STR_ID_LOG_IPCAM_SETUP,			//Setup IPCamera Device
	E_STR_ID_LOG_IPCAM_DELETE,			//Delete IPCamera Device
	E_STR_ID_LOG_IPCAM_CONNECTED,		//IPCamera Device Connected
	E_STR_ID_LOG_IPCAM_DISCONNECTED,	//IPCamera Device Disconnected


	E_STR_ID_REMAINING_ANALOG,			//Remaining(Analog)
	E_STR_ID_REMAINING_DIGITAL,			//Remaining(Digital)
	E_STR_ID_ALL_ANALOG,				//All(Analog)
	E_STR_ID_AUTH_DIGITAL_MAN,			//Digital Manage
	E_STR_ID_DEVICE_NET_ADDRESS,		//Device NetAddress
	E_STR_ID_LOCAL_NET_MASK,			//Local Subnet Mask
	E_STR_ID_LOCAL_GATEWAY,				//Local Gateway

	//无字符串数据
	E_STR_ID_RBT,	//Record Quality
	E_STR_ID_RESOLUTION_01,			//Best (960H) 960x480  -  Better (D1) 720x480  -  Good (CIF) 320x240
	E_STR_ID_RESOLUTION_02,			//Best (960H) 960x480  -  Better (D1) 704x576  -  Good (CIF) 352x288
	E_STR_ID_PRE_RECORD_TIME_01,		//Notes: Time camera starts recoding before motion happened.
	E_STR_ID_POST_RECORD_TIME_01,			//Notes: Time camera continue recording after motion stop.
	E_STR_ID_REC_ENCODE_TYPE_01,			//CBR:Constant bit rate    VBR:Variable bit rate
	E_STR_ID_FPS_01,						//Frame per Second
	E_STR_ID_NTP_SERVER_01,			//Internet Time Server
	E_STR_ID_TIP_01_04_01,					//[GMT-8:00]Pacific Time [US & Canada]
	E_STR_ID_TIP_01_47_01,					//Auto Sync Time (Internet connection required)
	E_STR_ID_TIP_02_01_01,					//Mask: Privacy screen to eliminate an area from viewing. 3 area maximum.

	E_STR_ID_RECORD_QUALITY_BEST,		//Best (960H)
	E_STR_ID_RECORD_QUALITY_BETTER,		//Better (D1)
	E_STR_ID_RECORD_QUALITY_GOOD,		//Good (CIF)

	E_STR_ID_RECORD_SNAP,			//Attach snapshots to your alert emails.
	E_STR_ID_NETWORK_EMAIL,			//Set up email notifications.
	E_STR_ID_MOTION_SCHEDULE,		//Schedule motion - trigerred notifications.

	E_STR_ID_TIP_04_01_01,			//Schedule continuous recording. Double click to enter start/end times.
	E_STR_ID_TIP_04_01_02,			//Schedule motion recording. Double click to enter start/end times.

	//@enum 配置 菜单  补充
	E_STR_ID_MENU_CFG_SCHEDULE_01,		//Schedule Recording

	//@section 标题、菜单
	//@enum 对话框标题  补充
	E_STR_ID_TITLE_UPGRADE_01,			//Update

	//@enum 升级 补充
	E_STR_ID_UPGRADE_01,			//Update

	E_STR_ID_ITEM_NUM = 0x5FFF,			//结尾
}ENUM_STRING_ID;
#endif //_STRING_DEFINE_H_

