/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : 
** Date         : 2013-09-10
** Version      : 1.0
** Description  : 消息推送
** Modify Record:
1:
***********************************************************************/
#ifndef __PUSH_DEFINE_H__
#define __PUSH_DEFINE_H__

#include "dvrdvsdef.h"
#include "PUB_common.h"

#pragma pack(4)

const unsigned long		IPHONE_TOKEN_LEN = 32;
const unsigned short	PNTS_SERVER_PORT = 6036;
const unsigned long		MAX_MSG_LEN = 50;
const unsigned long		MAX_IP_LEN = 25;
const unsigned long     MAX_PROVIDER_ID_LEN = 20; //初步定为30，后面根据实际情况更改

const char DVR_PUSH_VERSION[8] = "1.0.0";

/////推送数据种类
typedef enum __push_data_type__
{
	DATA_TYPE_ALARM		= 0,		//报警信息
	DATA_TYPE_OPERATE	= 1,		//操作信息
	DATA_TYPE_FILE		= 2,		//文件
}PUSH_DATA_TYPE;


//推送报警类型定义
typedef enum __push_alarm_type__
{
	PUSH_ALARM_VIDEOLOSS,		//视频丢失报警
	PUSH_ALARM_MOTION,			//动态侦测报警
	PUSH_ALARM_SENSOR,			//传感器报警
	PUSH_ALARM_DISKFULL,		//磁盘满报警
	PUSH_ALARM_DISKWARNING,		//磁盘衰减报警
}PUSH_ARARM_TYPE;

//操作事件类型定义
typedef enum __push_operate_type__
{
	PUSH_OPERATE_LOGIN,			//登陆
	PUSH_OPERATE_FORMAT_DISK,	//格式化硬盘
	PUSH_OPERATE_DELETE_FILE,	//删文件
	PUSH_OPERATE_CHANGE_PASS,	//修改密码
	PUSH_OPERATE_BACKUP,		//备份
}PUSH_OPERATE_TYPE;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////DVR和手机客户端和PNTS/////////////////////////////////////////
/*
带有推送功能的手机客户端ID
*/
typedef enum __push_client_type__
{
    PUSH_CLIENT_TVT_BEGIN = 0,
	PUSH_CLIENT_IOS_TVT_FREE,				//SuperCam
	PUSH_CLIENT_IOS_TVT_PRO,				//SuperCamPro
	PUSH_CLIENT_IOS_TVT_PROHD,			//SuperCamHD
	PUSH_CLIENT_IOS_TVT_CMS_HD,          //SuperCamCMS
	PUSH_CLIENT_ANDROID_TVT_PRO = 50,	//NONE
	PUSH_CLIENT_TVT_END,
    
	PUSH_CLIENT_PENGANTAI_BEGIN = 100,
	PUSH_CLIENT_IOS_PENGANTAI_FREE,				//SuperLive
	PUSH_CLIENT_IOS_PENGANTAI_IPC,				//SuperLiveIPC
	PUSH_CLIENT_IOS_PENGANTAI_PRO,				//SuperLivePro
	PUSH_CLIENT_IOS_PENGANTAI_HD,				//SuperLiveHD
	PUSH_CLIENT_IOS_PENGANTAI_CMS,				//SuperCMS
	PUSH_CLIENT_IOS_PENGANTAI_SUPERCLIENT,    //SuperClient
	PUSH_CLIENT_IOS_PENGANTAI_SUPERCLIENTM,	//SuperClientM
	PUSH_CLIENT_IOS_PENGANTAI_NVMS,				//NVMS
	PUSH_CLIENT_ANDROID_PENGANTAI_PRO = 150, //SuperLivePro
	PUSH_CLIENT_ANDROID_PENGANTAI_HD,         //SuperLiveHD
	PUSH_CLIENT_ANDROID_PENGANTAI_CMS_HD,   //SuperCMS
	PUSH_CLIENT_ANDROID_PENGANTAI_IPC,				//SuperLiveIPC
	PUSH_CLIENT_ANDROID_PENGANTAI_SUPERCLIENT,    //SuperClient
	PUSH_CLIENT_ANDROID_PENGANTAI_SUPERCLIENTM,	//SuperClientM
	PUSH_CLIENT_ANDROID_PENGANTAI_NVMS,				//NVMS
	PUSH_CLIENT_PENGANTAI_END,
    
	PUSH_CLIENT_US02_BEGIN = 200,
    PUSH_CLIENT_IOS_US02_PRO,					//QTView
	PUSH_CLIENT_IOS_US02_HD,					//QTViewHD
	PUSH_CLIENT_ANDROID_US02_PRO = 250,		//QTView
	PUSH_CLIENT_ANDROID_US02_HD,				//QTViewHD
	PUSH_CLIENT_US02_END,
    
	PUSH_CLIENT_US09_BEGIN = 300,
	PUSH_CLIENT_IOS_US09_PRO,					//EZLIVE
	PUSH_CLIENT_ANDROID_US09_PRO = 350,		//EZLIVE
	PUSH_CLIENT_US09_END,
    
	PUSH_CLIENT_US61_BEGIN = 400,
	PUSH_CLIENT_IOS_US61_PRO,						//BroView
	PUSH_CLIENT_IOS_US61_CMS_HD,					//BroCMS
    PUSH_CLIENT_ANDROID_US61_PRO = 450,			//BroView
	PUSH_CLIENT_US61_END,
    
	PUSH_CLIENT_RU04_BEGIN = 500,
	PUSH_CLIENT_IOS_RU04_PRO,					//VideoPhone
	PUSH_CLIENT_ANDROID_RU04_PRO = 550,		//GrizzlyVideoPhone
	PUSH_CLIENT_RU04_END,
    
	PUSH_CLIENT_IL03_BEGIN = 600,
	PUSH_CLIENT_IOS_IL03_PRO,					//Provison Cam
	PUSH_CLIENT_IOS_IL03_HD,					//Provison PAD
	PUSH_CLIENT_ANDROID_IL03_PRO=650,		//Provison Cam
	PUSH_CLIENT_ANDROID_IL03_HD,				//Provison PAD
	PUSH_CLIENT_IL03_END,
}PUSH_CLIENT_TYPE;

typedef enum __client_os_type__
{
	CLIENT_OS_IOS		= 0,			//苹果客户端
	CLIENT_OS_ANDROID	= 1,			//安卓客户端
}CLIENT_OS_TYPE;

//DVR杩斿洖瀹㈡埛绔槸鍚︽帴鏀跺埌tokenId
typedef enum _send_token_info__
{
    SEND_TOKEN_SUCCED
}SEND_TOKEN_INFO;


///////////////////////////////////////////////DVR和和PNTS/////////////////////////////////////////


/////provider鎺ㄩ�佹暟鎹殑鍥炲
typedef enum __push_data_reply__
{
	PUSH_SUCCESS		= 0,		//推送成功
	PUSH_FAIL	= 1,				//推送失败

}PUSH_DATA_REPLY;


/////PNTS发送给provider的命令
typedef enum __push_msg_type__
{
	PUSH_CMD_REQUEST_PUSH_MSG		= 0x100,				//provider请求PNTS的推送消息，DVR_PUSH_MSG
	PUSH_CMD_REQUEST_PROVIDER_INFO,							//请求provider的信息


	PUSH_CMD_PUSH_MSG		= 0x200,					//推送消息，只能是PROVIDER--》PNTS
	PUSH_CMD_PROVIDER_INFO,								//PROVIDER设备相关信息,PROVIDER_TYPE_INFO PROVIDER--》PNTS

	PUSH_DEVICE_TOKEN_INFO = 0x300,							//ios每次连接到provider，provider主动发给PNTS,DEVICE_TOKEN_INFO_PNTS
    PUSH_OFFLINE_TOKEN_INFO,                                //当ios端掉线后，provider主动发给PNTS,DEVICE_TOKEN_INFO_PNTS
}PUSH_MSG_TYPE;

/*
provider发送给PNTS服务器的推送消息格式:PUSH_MSG_HEADER|PUSH_MSG_INFO|DVR_PUSH_MSG

PNTS服务器给provider推送消息的回复信息格式:PUSH_MSG_HEADER|PUSH_MSG_INFO|PUSH_DATA_REPLY(int型)

PNTS服务器给provider请求信息格式:PUSH_MSG_HEADER|PUSH_MSG_INFO
provider给PNTS服务器请求消息的回复信息格式:PUSH_MSG_HEADER|PUSH_MSG_INFO|(某个结构体)

provider和PNTS之间的信息交互过程：
1. provider与PNTS连接后，PNTS发送PUSH_CMD_REQUEST_PROVIDER_INFO命令消息给provider，provider回复PUSH_CMD_REPLY_PROVIDER_INFO
   命令加PROVIDER_TYPE_INFO结构体给PNTS。

2. ios客户端每次连接到provider后，provider发送PUSH_DEVICE_TOKEN_INFO命令加DEVICE_TOKEN_INFO_PNTS结构体给PNTS.
3. ios客户端掉线后，provider发送PUSH_OFFLINE_TOKEN_INFO命令加DEVICE_TOKEN_INFO_PNTS结构体给PNTS。
4. provider和PNTS之间的心跳机制：
   每隔20s发送一次心跳消息。
 
 
 1：DVR上线后上传设备属性信息，比如序列号、厂家ID、IP等；DVR状态变化时需要上传新的设备属性
 2：实时更新前端token相关状态
 3：短连接发送推送信息、每次连接后要持续一定时间、两次连续连接之间有一定间隔
 4：消息中不包含设备相关属性信息，比如说IP和序列号；
 5：提示字符限制在50个
 6：服务器地址可配（可选）
 7：老设备升级新软件（带推送功能）后不应当有推送功能；>>>>>>待定
 8：先在US02上支持

*/

typedef struct __push_msg_header__
{
	char startFlag[4];
	unsigned long dataLen;
}PUSH_MSG_HEADER;



typedef struct __push_msg_info__
{
	unsigned long isRequest;	//为1表示请求，为0表示回应
	unsigned long msgType;		//PUSH_MSG_TYPE
	unsigned long msgLen;		//
}PUSH_MSG_INFO;

/*
设备令牌结构体定义，用于客户端在连接的时候发送给设备端
*/
typedef struct __device_token_info__
{
	unsigned long		length;				//DEVICE_TOKEN_INFO数据长度
	char				version[8];			//版本
	CLIENT_OS_TYPE		clientOSType;		//客户端操作系统类型
	PUSH_CLIENT_TYPE	pushClientType;		//客户端类型
	unsigned long		tokenLen;
	char  				token[68];			//设备令牌
	char                OSVersion[8];    //系统版本号
	char                softVersion[8];   //软件版本号
	char				language[16];		//系统语言
	char				Locale[16];			//区域
	char			    latitude[16];			//纬度
	char				longitude[16];			//经度
}DEVICE_TOKEN_INFO;

/*
璁惧浠ょ墝鎵╁睍缁撴瀯浣撳畾涔夛紝鐢ㄤ簬瀹㈡埛绔湪杩炴帴鍒皃rovider鏃讹紝provider鍙戠粰PNTS
*/
typedef struct __device_token_info_pnts__
{
	DEVICE_TOKEN_INFO deviceTokenInfo;
	char providerId[MAX_PROVIDER_ID_LEN];				//provider的标识Id
    char userName[36];                                  //用户名
    //int  sound;                                         //推送消息的声音
}DEVICE_TOKEN_INFO_PNTS;

/*
provider基本信息结构体定义，用于provider在连接到PNTS时，PNTS请求provider的信息
*/

typedef struct _provider_type_info
{
    //unsigned long headFlag; ////取值为HEADFLAG
    unsigned long devicetype; /////////设备类型 DVS  IPCAMERA  DVR  SUPERDVR
    unsigned long producttype;  /////产品类型
    unsigned long netprotrocolver; ///////////设备对应的网络协议版本
    unsigned long configver; ///////////设备对应的配置版本
    char providerId[MAX_PROVIDER_ID_LEN]; ////////provider的标识Id
    unsigned long encryptType;	//数据加密方式，0 表示没有加密方式
    unsigned long encryptParam;
    unsigned long ulReserve[6];				/////保留信息，以便以后扩充
    unsigned long dwLG;					//支持哪些语言 0 简体中文 + 英文  1 繁体中文 + 英文
    unsigned long dwUDPDefaultVideoStream;	//用于穿透默认的流
	unsigned char companyInfo[32];      //供应商标识
	unsigned long areaId;         //所在地区
    char serverIP[MAX_IP_LEN];				//provider的IP
	int  serverPort;				        //provider的port

}PROVIDER_TYPE_INFO, *LPPROVIDER_TYPE_INFO;

/*
推送消息结构体定义，provider需要推送消息时发给PNTS
*/
typedef struct __provider_push_msg__
{
	//char providerId[MAX_PROVIDER_ID_LEN];       //provider的标识Id
	char msg[MAX_MSG_LEN];					//消息体
	int  msgLen;					//消息体的长度
    int  badage;                   
	//char serverIP[MAX_IP_LEN];				//provider的IP
	//int  serverPort;				//provider的port
    //char userName[36];                //用户名
    //char password[36];                //密码
	int  msgType;					//推送消息类型(PUSH_DATA_TYPE,0,1,2)
	int  msgValue;					//消息具体的值(PUSH_ARARM_TYPE/PUSH_OPERATE_TYPE)
    int  paramLen;                  //推送消息参数的长度，应当根据msgType和msgValue解析参数的含义
	unsigned long time;				//UTC时间
}PROVIDER_PUSH_MSG, *LPPROVIDER_PUSH_MSG;


/*
{
	"aps":
	{
		"alert":"2011/9/15 02:00:55 Motion Alarm",
		"sound":"default",
        "badage":"1"
	},
	"URL":"192.168.3.154:80"
	"UTC":"1316052055",
	"T":"1"							//PUSH_DATA_TYPE,0,1,2
	"V":"3"							//具体的值
	"bHParam":"len"                  //附带参数的长度
	"P":"0x123456"					//附带参数，用二进制表示，视频丢失、传感器、移动侦测时用来存放通道信息；磁盘满时用来存放磁盘剩余空间，衰减时可用来存放衰减相关信息；操作时用来存放操作的附带参数
    "PSN":"providerId"              //providerId,长度MAX_PROVIDER_ID_LEN
    "AU":"user"
    
}
*/
//IPPORT&USER&PASS&UTC&MAC&T&V&HP&P
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma pack()

#endif

/*
CMD_REQUEST_CFG_ADD_IPHONE_TOKEN			//添加设备令牌
发送命令加设备令牌结构体 DEVICE_TOKEN_INFO
服务器回复CMD_REPLY_CFG_ADD_IPHONE_TOKEN	，附加添加设备令牌结果ADD_TOKEN_STATUS
*/
