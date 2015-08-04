/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : liwenxie
** Date         : 2010-03-30
** Name         : HxGlobal.h
** Version      : 1.0
** Description  : 
***********************************************************************/
#ifndef _HXGLOBAL_H
#define _HXGLOBAL_H

#include "PUB_common.h"
#include "SWL_Public.h"

//#include "PfHxPublic.h"

const char TVTHX_ENCODER_MODEL[]	= "TVTDigital_H264";

enum _TVTHXPTZ_CMD
{
	PTZ_UP			= 1,
	PTZ_DOWN		= 2,
	PTZ_LEFT		= 3,
	PTZ_RIGHT		= 4,
	IRIS_UP			= 5,
	IRIS_DOWN		= 6,
	ZOOM_UP			= 7,
	ZOOM_DOWN		= 8,
	FOCUS_UP		= 9,
	FOCUS_DOWN		= 10,
};


#define AUDIO_STREAM				97
#define VIDEO_H264_STREAM			96


#define TVTHX_SUPPORT_PB			1		//支持前端回放 0 不支持     
#define TVTHX_SUPPORT_SD			0		//前端支持sd卡 0 不支持(dvs版本应该关闭该宏)     
#define TVTHX_SUPPORT_SCHEME		1		//前端支持预案(根据3.0协议版本开始支持, IPC版本应该关闭该宏) 
#define TVTHX_SUPPORT_VIDEO2		1		//支持第二版视频协议 0 不支持 (应该一直设为1,除非协议出现更高的版本)
#define TVTHX_SUPPORT_NAT			0		//前端设备支持NAT穿越 0 不支持
#define TVTHX_SUPPORT_PTZ_LOCK		1		//前端支持云台锁定 0 不支持(IPC版本应该关闭该宏)
#define TVTHX_SUPPORT_MAINSTREAM	1		//缺省码流，必须支持
#define TVTHX_SUPPORT_STREAM1		0		//子码流1
#define TVTHX_SUPPORT_STREAM2		0		//子码流2
#define TVTHX_SUPPORT_STREAM3		0		//子码流3


#define TVTHX_MSG_VERSION			257


#define TVTHX_MAX_MSG_SIZE			(3 * 1024)
#define TVTHX_KA_MSG_SIZE			(1.5 * 1024)
#define TVTHX_OK_FLAG				0

#ifdef __RELEASE__
	#define TVTHX_MAX_TCP_TIMEO			20000		//超时时间由30000改成3000  by lwx 2010-04-7
#else
	#define TVTHX_MAX_TCP_TIMEO			100		
#endif

#ifdef __RELEASE__
	#define TVTHX_MAX_REPLAY_WAIT_TIME		36		//DVR请求命令的应答超时时间(s)，如果大于6分钟没有收到数据就需要从新注册登录
#else
	#define TVTHX_MAX_REPLAY_WAIT_TIME		20		
#endif


#ifdef __RELEASE__
#define TVTHX_MAX_AVSEND_WAIT_TIME		120		//发送音视频超时时间
#else
#define TVTHX_MAX_AVSEND_WAIT_TIME		120		
#endif

#define TVTHX_MAX_VIDEO_PACK_SIZE	(9   * 1024)		//视频打包的大小
#define TVTHX_CMD_BUF_LEN			(256 * 1024)
#define TVTHX_REG_MSG_SIZE			(8   * 1024)

//设备基础配置
//#define TVTHX_MAX_AUDIO				4
//#define TVTHX_MAX_CAM				16	//一个设备支持最大通道数
#define TVTHX_CAM_VIDEO_CON			4	//一路支持的最大视频连接数
#define TVTHX_MAX_ALARM_IN			2	//报警输入设备数
#define TVTHX_MAX_ALARM_OUT			2	//报警输出设备数
#define TVTHX_MAX_VMASK_AREA		4
#define TVTHX_MAX_VMOTION_AREA		4

//设备连接方式
#define TVTHX_DEV_LINK_LAN			1
#define TVTHX_DEV_LINK_ADSL			2
#define TVTHX_DEV_LINK_OTHER		3
#define IS_COMMAND_MSG(x)			((TVTHX_SEND_VIDEO_DATA_MSG != x) && (0 != x))


typedef enum _tvthx_msg_cmd_
{
	TVTHX_MSG_CMD_NONE					= 0,
	//TVTHX_KEEP_ALIVE_REQ_MSG			= 2,
	TVTHX_KEEP_ALIVE_REQ_MSG			= 2,
	TVTHX_KEEP_ALIVE_RET_MSG			= 3,	

	TVTHX_START_VIDEO_REQ_MSG			= 4,
	TVTHX_START_VIDEO_RET_MSG			= 5,

	TVTHX_DEVICE_STATUS_REPORT_REQ_MSG	= 20,
	TVTHX_DEVICE_STATUS_REPORT_RET_MSG	= 21,

	TVTHX_CAMERA_STATUS_REPORT_REQ_MSG	= 22,
	TVTHX_CAMERA_STATUS_REPORT_RET_MSG	= 23,

	TVTHX_FAULT_REPORT_REQ_MSG			= 1100,
	TVTHX_FAULT_REPORT_RET_MSG			= 1101,

	TVTHX_REMOTE_SNAP_REQ_MSG			= 1048,	//手动抓拍请求
	TVTHX_REMOTE_SNAP_RET_MSG			= 1049,	//手动抓拍应答

	TVTHX_REG_REQ_MSG					= 2002,	//注册请求
	TVTHX_REG_RET_MSG					= 2003,	//注册应答
	TVTHX_GET_NETMANAGER_REQ_MSG		= 2004, //下载网管配置请求
	TVTHX_GET_NETMANAGER_RET_MSG		= 2005, //下载网管配置应答

	TVTHX_VIDEO_DISCONN_REQ_MSG			= 2012,	//断开视频连接请求
	TVTHX_VIDEO_DISCONN_RET_MSG			= 2013,	//断开视频连接应答


	TVTHX_STORE_REG_MSG					= 4002,	//存储注册请求
	TVTHX_STORE_RET_MSG					= 4003,	//存储注册应答

	TVTHX_V_DOWNLOAD_TYPE_REQ_MSG		= 5004,	//下载方式查询
	TVTHX_V_DOWNLOAD_TYPE_RET_MSG		= 5005,	//下载方式应答
	TVTHX_CTRL_PTZ_REQ_MSG				= 5006,	//控制云镜请求
	TVTHX_CTRL_PTZ_RET_MSG				= 5007,	//控制云镜应答
	TVTHX_VIDEO_ACCREDIT_REQ_MSG1		= 5010,	//第一版视频请求
	TVTHX_VIDEO_ACCREDIT_REQ_MSG2		= 6002,	//第二版视频请求
	TVTHX_VIDEO_ACCREDIT_RET_MSG1		= 5011,	//第一版视频请求
	TVTHX_VIDEO_ACCREDIT_RET_MSG2		= 6003,	//第二版视频请求

	TVTHX_SET_VIDEO_PARA_REQ_MSG		= 5032,	//设置视频参数请求
	TVTHX_SET_VIDEO_PARA_RET_MSG		= 5033,	//设置视频参数应答
	TVTHX_GET_VIDEO_PARA_REQ_MSG		= 5034,	//获取视频参数请求
	TVTHX_GET_VIDEO_PARA_RET_MSG		= 5035,	//获取视频参数应答

	TVTHX_GET_PTZ_STATUS_REQ_MSG		= 5042,	//获取云镜状态请求
	TVTHX_GET_PTZ_STATUS_RET_MSG		= 5043,	//获取云镜状态应答
	TVTHX_DEV_CONFIG_REQ_MSG			= 5046,	//设置DVS参数请求
	TVTHX_DEV_CONFIG_RET_MSG			= 5047,	//设置DVS参数应答

	TVTHX_SET_ALARM_OUTPUT_REQ_MSG		= 5064,	//设置报警输出状态请求
	TVTHX_SET_ALARM_OUTPUT_RET_MSG		= 5065,	//设置报警输出状态应答
	TVTHX_GET_ALARM_OUTPUT_REQ_MSG		= 5066,	//获取报警输出状态请求
	TVTHX_GET_ALARM_OUTPUT_RET_MSG		= 5067,	//获取报警输出状态应答

	TVTHX_SET_MOTION_GUARD_REQ_MSG		= 5080,	//设置移动侦测布防时间请求
	TVTHX_SET_MOTION_GUARD_RET_MSG		= 5081,	//设置移动侦测布防时间应答
	TVTHX_GET_MOTION_GUARD_REQ_MSG		= 5082,	//查询移动侦测布防时间
	TVTHX_GET_MOTION_GUARD_RET_MSG		= 5083,	//回复移动侦测布防时间
	TVTHX_SET_ALARMIN_GUARD_REQ_MSG		= 5084,	//设置报警输入布防时间请求
	TVTHX_SET_ALARMIN_GUARD_RET_MSG		= 5085,	//设置报警输入布防时间应答
	TVTHX_GET_ALARMIN_GUARD_REQ_MSG		= 5086,	//查询报警输入布防时间
	TVTHX_GET_ALARMIN_GUARD_RET_MSG		= 5087,	//回复报警输入布防时间

	TVTHX_SET_ALARMOUT_GUARD_REQ_MSG	= 5096,	//设置报警输入布防时间请求
	TVTHX_SET_ALARMOUT_GUARD_RET_MSG	= 5097,	//设置报警输入布防时间应答
	TVTHX_GET_ALARMOUT_GUARD_REQ_MSG	= 5098,	//查询报警输入布防时间
	TVTHX_GET_ALARMOUT_GUARD_RET_MSG	= 5099,	//回复报警输入布防时间

	TVTHX_SET_SCHEME_REQ_MSG			= 5208,//设置联动报警
	TVTHX_SET_SCHEME_RET_MSG			= 5209,//联动报警的回复

	TVTHX_DEL_SCHEME_REQ_MSG			= 5210,//删除联动报警
	TVTHX_DEL_SCHEME_RET_MSG			= 5211,//删除联动报警的回复

	TVTHX_REPORT_ALARM_REQ_MSG			= 5214,	//报警通知请求
	TVTHX_REPORT_ALARM_RET_MSG			= 5215,	//报警通知应答

	TVTHX_MANUAL_STOP_SCHEME_MSG		= 5216,

	TVTHX_MANUAL_START_TRACK_REQ_MSG	= 5218,
	TVTHX_MANUAL_START_TRACK_RET_MSG	= 5219,

	TVTHX_MANUAL_START_SCHEME_REQ_MSG	= 5220,
	TVTHX_MANUAL_START_SCHEME_RET_MSG	= 5221,

	TVTHX_START_SCHEME_REQ_MSG			= 5214, 


	TVTHX_AUDIO_ACCREDIT_REQ_MSG		= 6110,	//接入向DVS发起语音呼叫
	TVTHX_AUDIO_ACCREDIT_RET_MSG		= 6111,	//DVS对语音呼叫应答

	TVTHX_AUDIO_CONN_REQ_MSG			= 6112,
	TVTHX_AUDIO_CONN_RET_MSG			= 6113,
	TVTHX_SEND_AUDIO_DATA_MSG			= 6114,

	TVTHX_EXT_REQ_MSG					= 6200,
	TVTHX_EXT_RET_MSG					= 6201,

	TVTHX_VIDEO_CONN_REQ_MSG			= 8000,	//客户端向设备请求视频
	TVTHX_VIDEO_CONN_RET_MSG			= 8001,
	TVTHX_MANUFACTURER_RET_MSG 			= 8002,
	TVTHX_SEND_VIDEO_DATA_MSG			= 8004,

	TVTHX_UPDATE_CHECK_REQ_MSG			= 8010,	//版本验证
	TVTHX_UPDATE_CHECK_RET_MSG			= 8011,	//版本验证回复
	TVTHX_UPDATE_MSG_REQ_MSG			= 8012,	//更新通知
	TVTHX_UPDATE_MSG_RET_MSG			= 8013,	//更新通知回复

	TVTHX_VIDEO_DOWNLOAD_REQ_MSG		= 8308,	//录像下载请求
	TVTHX_VIDEO_DOWNLOAD_REP_MSG		= 8309,	//录像下载回应

	TVTHX_FIRST_VIDEO_DOWNLOAD_REQ_MSG	= 8300, //首个录像文件下载请求
	TVTHX_FIRST_VIDEO_DOWNLOAD_RET_MSG	= 8301,	//首个录像文件下载的应答
	TVTHX_NEXT_VIDEO_DOWNLOAD_REQ_MSG	= 8302,	//后续文件下载请求
	TVTHX_NEXT_VIDEO_DOWNLOAD_RET_MSG	= 8303,	//后续文件下载的应答
	TVTHX_VIDEO_DOWNLOAD_KEEPING_MSG	= 8304,	//录像下载时客户端发的保持连接消息
	
}TVTHX_MSG_CMD;

//云台参数
typedef enum _hxht_ptz_cmd_
{
	TVTHX_PTZ_CMD_STOP				= 0,		//早期版本		
	TVTHX_PTZ_CMD_UP				= 1,
	TVTHX_PTZ_CMD_DOWN				= 2,
	TVTHX_PTZ_CMD_LEFT				= 3,
	TVTHX_PTZ_CMD_RIGHT				= 4,
	TVTHX_PTZ_CMD_IRIS_OPEN		= 5,
	TVTHX_PTZ_CMD_IRIS_CLOSE		= 6,
	TVTHX_PTZ_CMD_ZOOM_OUT			= 8,
	TVTHX_PTZ_CMD_ZOMM_IN			= 7,
	TVTHX_PTZ_CMD_FOCUS_FAR			= 9,
	TVTHX_PTZ_CMD_FOCUS_NEAR		= 10,
	TVTHX_PTZ_CMD_AUTO_SCAN_START	= 11,
	TVTHX_PTZ_CMD_AUTO_SCAN_STOP	= 12,
	TVTHX_PTZ_CMD_LIGHT_ON			= 13,
	TVTHX_PTZ_CMD_LIGHT_OFF			= 14,
	TVTHX_PTZ_CMD_BRUSH_ON			= 15,
	TVTHX_PTZ_CMD_BRUSH_OFF			= 16,
	TVTHX_PTZ_CMD_ACTION			= 8200,	  //新版本				
	TVTHX_PTZ_CMD_STOP_ACTION		= 8202,
	TVTHX_PTZ_CMD_AUTO_SCAN			= 8204,
	TVTHX_PTZ_CMD_SET_PRESET		= 8206,
	TVTHX_PTZ_CMD_CLR_PRESET		= 8208,
	TVTHX_PTZ_CMD_CALL_PRESET		= 8210,
	TVTHX_PTZ_CMD_CLR_ALL_PRESET	= 8212,
	TVTHX_PTZ_CMD_IRIS_CRL			= 8300,
	TVTHX_PTZ_CMD_ZOOM_CRL			= 8302,
	TVTHX_PTZ_CMD_FOCUS_CRL			= 8304,
	TVTHX_PTZ_CMD_IRIS_CRL_STOP		= 8306,
	TVTHX_PTZ_CMD_ZOOM_CRL_STOP		= 8308,
	TVTHX_PTZ_CMD_FOCUS_CRL_STOP	= 8310,
	TVTHX_PTZ_CMD_ASSISTANT_CRL		= 8400,
	TVTHX_PTZ_CMD_RESUME_FAULT		= 8402, 
	TVTHX_PTZ_CMD_GET_RIGHT			= 8500,
}HXHT_PTZ_CMD;



enum
{
	HXAV_TYPE_VIDEO = 0,
	HXAV_TYPE_AUDIO = 1,

	HXAV_TYPE_NUM
};

enum
{
	VIDEO_MAIN_STREAM	= 1,
	VIDEO_SUB_STREAM	= 2
};

typedef enum _STREAM_TYPE_E
{
	MAIN_STREAM		= 0,
	SUB_STREAM1 	= 1,
	SUB_STREAM2 	= 2,
	SUB_STREAM3 	= 3,
}STREAM_TYPE_E;

typedef enum _DEV_TYPE_E
{
	TYPE_USER		= 1,
	TYPE_TERMINAL	= 2,
	TYPE_STORE		= 4,
	TYPE_CAMERA		= 5,
	TYPE_ALARM		= 6,
	TYPE_DISPLAY	= 7,
	TYPE_ALARMOUT	= 8,
	TYPE_ANALYSE	= 20,
}DEV_TYPE_E;

typedef enum _STREAM_STATUS_E
{
	STREAM_SLEEP	= 1,
	STREAM_ACTIVE	= 2,
	STREAM_READY	= 3,	//Stream对象在匹配Ticket成功后的状态
	STREAM_RUN		= 4		//设备发送视频开始消息后,Stream对象在这个状态下发送视频
}STREAM_STATUS_E;
typedef enum _video_param_type_
{
	VIDEO_PARAM_TYPE_MAJOR		= 1,		//主要视频参数
	VIDEO_PARAM_TYPE_PICQU		= 2,		//图像质量
	VIDEO_PARAM_TYPE_MAXSTREAM	= 3,		//最大码流
	VIDEO_PARAM_TYPE_PICTYPE	= 4,		//图像格式
	VIDEO_PARAM_TYPE_BLOCKAREA	= 5,		//遮挡区域
	VIDEO_PARAM_TYPE_OSD		= 6,		//OSD和logo
	VIDEO_PARAM_TYPE_COLOR		= 7,		//色彩
	VIDEO_PARAM_TYPE_MOVEBLOCK	= 8,		//移动侦测区域
}VIDEO_PARAM_TYPE;





typedef enum _frame_codec_type
{
	HX_VIDEO_IFRAME	= 1,
	HX_VIDEO_BFRAME, 
	HX_VIDEO_PFRAME,
	HX_AUDIO_FRAME
}FRAME_CODE_TYPE;

typedef enum _pack_serial_type
{
	HX_PACK_WHOLE_FRAME = 1,
	HX_PACK_FIRST,
	HX_PACK_MIDDLE,
	HX_PACK_LAST,
}PACK_SERIAL_TYPE;

typedef struct _TVTHX_ID_INFO
{
	char    front_device_id[32];
	char    front_device_transaction_id[32];//前端设备(视频服务器)业务id
	char    camera_id[4][32];//前端设备(摄像头)id	
	char    camera_transaction_id[4][32];//前端设备(摄像头)业务id
	char    alarm_in_dev_id[4][32];		//告警输入id
	char    alarm_out_dev_id[4][32];     //告警输出id
	char    alarm_in_transaction_id[4][32];		//告警输入id
	char    alarm_out_transaction_id[4][32];     //告警输出id	
	char    store_dev_id[64];							//存储模块id
	char    store_transaction_id[64];				    //存储模块业务id
	char    front_device_naming[128];
	char    camera_naming[4][128];
	char    alarm_in_naming[4][128];
	char    alarm_out_naming[4][128];
	char    center_id[32];                               //服务器的中心id
}TVTHX_ID_INFO, *PTVTHX_ID_INFO;

typedef struct _TVTHX_REBOOT_INFO
{
	unsigned long  reboot_hour;
	unsigned long  reboot_min;
	unsigned long  reboot_cycle;
}TVTHX_REBOOT_INFO, *PTVTHX_REBOOT_INFO;

typedef enum _hx_video_version_type_
{
	HX_VIDEO_VERSION_TYPE_ONE		= 1,
	HX_VIDEO_VERSION_TYPE_TWO		= 2,
}HX_VIDEO_VERSION;

typedef struct _video_request_info_
{
	int chnnNum;				//请求的通道
	HX_VIDEO_VERSION version;				
	int streamType;
	char devDataID[64];
	char ticket[32];
	char clientIP[32];
	int  clientPort;
	SWL_socket_t csSockTaldfd;
	SWL_socket_t clSockTaldfd;
}VIDEO_REQUEST_INFO;

//typedef struct _TVTHX_REMOTE_STORE_INFO
//{
//	char				remote_ip[16];
//	unsigned short		remote_port;
//	char				ftp_user[32];
//	char				ftp_psw[32];
//	int				model;
//}TVTHX_REMOTE_STORE_INFO, *PTVTHX_REMOTE_STORE_INFO;




//------------------消息头---------------//
typedef struct _tvthx_msg_head_
{
	unsigned long	version;
	unsigned long	commlen;
	unsigned long	commnum;
	unsigned long	commserial;

}TVTHX_MSG_HEAD;

typedef struct _TVTHX_REQUEST_MSG_HEAD
{
	unsigned long	version;
	unsigned long	commlen;
	unsigned long	commnum;
	unsigned long	commserial;
	char			sid[32];
	char			did[32];
}TVTHX_REQUEST_MSG_HEAD, *PTVTHX_REQUEST_MSG_HEAD;


typedef struct _TVTHX_RETURN_MSG_HEAD
{
	unsigned long version;
	unsigned long commlen;
	unsigned long commnum;
	unsigned long commserial;
	unsigned long succesflag;	
}TVTHX_RETURN_MSG_HEAD, *PTVTHX_RETURN_MSG_HEAD;

typedef struct _TVTHX_EXT_VIDEO_INFO
{
	unsigned short frameNo;
	unsigned char  frameType;
	unsigned char  continuePackNo;
	struct timeval	timeV;
}TVTHX_EXT_VIDEO_INFO, *PTVTHX_EXT_VIDEO_INFO;

//------------------消息头--------------//



typedef struct _TVTHX_KEEPALIVE_S
{
	unsigned int		keepalive_cycle;
	unsigned int		last_keepalive_time;
}TVTHX_KEEPALIVE_S;


typedef struct _TVTHX_STORE_S
{
	unsigned int		store_heartbeat_cycle;
	unsigned int		last_keepalive_time;
}TVTHX_STORE_S;


//typedef struct _TVTHX_STREAM_HEAD
//{
//	char			szCompany[32];
//	unsigned char	nAudioCoder;
//	unsigned char	nVideoCoder;
//	unsigned char	nFrameRate;
//	unsigned char	nSize;			//结构体大小
//	unsigned int	nWidth;			//视频宽
//	unsigned int	nHeight;		//视频高
//	unsigned long	nCompression;	//视频压缩格式
//	unsigned char	nReserved1;
//	unsigned char	nReserved2;
//}TVTHX_STREAM_HEAD;

typedef struct _TVTHX_STREAM_HEAD
{
	char			szCompany[32];
	unsigned long	nSize;			//结构体大小
	unsigned long	nWidth;			//视频宽
	unsigned long	nHeight;		//视频高
	unsigned long	nCompression;	//视频压缩格式
	unsigned long	waveFormat;		//音频格式（WAVE_FORAMT_PCM为标准原始采样音频格式）
	unsigned short	waveBitPerSample;//音频量化等级
	unsigned short	waveChannels;	//声道个数
	unsigned long	waveSample;		//音频采样频率
}TVTHX_STREAM_HEAD;

// typedef struct _FRAME_HEAD
// {
// 	unsigned char avType;  //包类型 HXAV_TYPE_VIDEO HXAV_TYPE_AUDIO 两种类型
// 	unsigned char frameType; //帧类型	FRAME_CODE_TYPE
// 	unsigned char packageFlag;//包类型	PACK_SERIAL_TYPE
// 	unsigned char size;  //结构体大小
// 	unsigned int  nDataLen;
// 	unsigned char reserved[4];//保留
// }FRAME_HEAD;


typedef struct _FRAME_HEAD
{
	unsigned long	frameType; //帧类型 FRAME_CODE_TYPE
	unsigned long	packageFlag;//包类型 PACK_SERIAL_TYPE
	unsigned long	nPackLen;
	unsigned long	nFrameLen;
	unsigned long	width;
	unsigned long	height;
	unsigned long	dataPos;	//分片时存放分片在数据中的开始位置
	LONGLONG		frameTime;
	unsigned long	frameIndex;
}FRAME_HEAD;

typedef enum _TVTHX_WORK_STATUS
{
	TVTHX_INIT,
	TVTHX_REG,
	TVTHX_ON,
	TVTHX_OFF,
	TVTHX_ERR,
	TVTHX_QUIT,
}TVTHX_WORK_STATUS;

typedef enum _hx_cmd_type_
{
	HX_CMD_TYPE_REQUEST_FROM_DVR	= 0,					//从DVR发出的请求
	HX_CMD_TYPE_REPLAY_FROM_DVR,							//DVR的应答信息
	HX_CMD_TYPE_REQUEST_FROM_NET,							//网络到达的请求
	HX_CMD_TYPE_REPLAY_FROM_NET,							//网络到达的应答
	HX_CMD_TYPE_VIDEO,
	HX_CMD_TYPE_AUDIO,
}HX_CMD_TYPE;

typedef enum _video_status_									//参看HXHT实时视频连接过程
{
	VIDEO_STATUS_NONE				= 0,					//没有视频
	VIDEO_STATUS_REQ_VIDEO			= 1,					//客户端视频请求，DVR不会检测到该状态
	VIDEO_STATUS_TELL_SERVER		= 2,					//接入服务器通知DVR
	VIDEO_STATUS_CONNECT_CLIENT		= 3,					//服务端连客户端，在DVR主动连接时存在该状态
	VIDEO_STATUS_REP_CONNECT_CLIENT = 4,					//DVR回复客户端的连接
	VIDEO_STATUS_REP_TELL_SERVER	= 5,					//2的回应
	VIDEO_STATUS_REP_VIDEO			= 6,					//
	VIDEO_STATUS_CLT_CT_DVR			= 7,					//客户端连接DVR的视频端口，只有被动连接有这个状态					
	VIDEO_STATUS_REP_CLT_CT_DVR		= 8,					//7的回应
	VIDEO_STATUS_START_VIDEO		= 9,					//DVR通知接入服务器开始视频传输
	VIDEO_STATUS_REP_START_VIDEO	= 10,					//接入服务器对9的回应
	VIDEO_STATUS_SND_VIDEO_HEAD		= 11,					//DVR发送视频头到客户端
	VIDEO_STATUS_VIDEO_STREAM		= 12,					//视频流	
	VIDEO_STATUS_REQ_STOP_VIDEO		= 13,					//DVR请求接入服务器停止视频
	VIDEO_STATUS_REP_STOP_VIDEO		= 14,					//接入服务器回复
	VIDEO_STATUS_REP_CLIENT_STOP_REQ = 15,					//客户端请求视频停止

	//历史视频请求时的状态
	VIDEO_STATUS_DL_REQ_VIDEO		= 16,
	VIDEO_STATUS_HISTORY_PLAY_HEAD	= 17,
	VIDEO_STATUS_HISTORY_PLAYING	= 18,
	VIDEO_STATUS_HISTORY_NEXT_FILE	= 19,
	VIDEO_STATUS_HISTORY_END		= 20,
}VIDEO_STATUS;


typedef struct _hx_cmd_data
{
	int				hxMsgVer;									//互信互通消息版本
	unsigned long	cmdType;									//消息的种类
	unsigned long	cmdNum;										//互信互通消息类型， HX_CMD_TYPE中的值
	unsigned long	clientPort;									//客户端端口
	char			clientIP[24];								//客户端IP
	SWL_socket_t	fromSocketFd;								//消息的来源， 来自哪个套接口，回应消息时就向这个套接口发送数据
	int				cmdGenerateTime;							//消息产生的时间，被动消息时是接收时间，主动消息时是发送时间，主动消息时把该时间存放在指令序号中
																//在主动消息回复中的指令序号和这个时间一样的，可以用这个关键字来查找请求的消息
	int				videoStatus;								//记录视频的状态
	bool			isTalk;
	int				chnnNum;
	char			ticket[32];
	char			notused[120];
	char			*pData;										//消息数据，格式为TVTHX_RETURN_MSG_HEAD+xml内容，或者_TVTHX_RETURN_MSG_HEAD+xml内容
	unsigned long	buffLen;									//缓冲区长度
	unsigned long	dataLen;									//*pData中数据的长度 
	unsigned long	dataPos;									//在接收时表示已经接收到的数据，发送时表示已经发送的数据
	char			*pRecData;
	unsigned long	recBufLen;
	unsigned long	recDataLen;
	unsigned long	recDataPos;
	LONGLONG		lastSRtime;									//上次发送或接收数据时间
	unsigned long	hxGetSetConfig;								//每个客户端一个CHxGetSetConfig类来处理配置信息
	unsigned long	clientID;									//客户端ID
	unsigned long	pHxAVDataSendMan;							//每个客户端绑定一个音视频服务
}HX_CMD_DATA;


typedef struct _hx_dvr_req_cmd_info_
{
	TVTHX_MSG_CMD cmdNum;
	SWL_socket_t  clientSockfd;
	char *pParam;
	unsigned long dataLen;
}HX_DVR_REQ_CMD_INFO;

//开始和停止视频的参数
typedef struct  _start_stream_param_
{
	char szTicket[32];		//该通道视频传输时协商的密钥
	unsigned long devType;	//设备类型 DEV_TYPE_E
	unsigned long isStart;  //开始还是关闭视频， 0为关闭，非0为开始
	unsigned long streamType; //视频类型， 主码流还是子码流STREAM_TYPE_E
	HX_VIDEO_VERSION version;
}START_STREAM_PARAM;


typedef enum _config_type_
{
	CONFIG_TYPE_REALTIME_VIDEO	= 0,
	CONFIG_TYPE_485				= 1,
	CONFIG_TYPE_PTZ				= 2,
	CONFIG_TYPE_ALARM			= 3,
	CONFIG_TYPE_BASIC			= 4,
}CONFIG_TYPE;

typedef struct _hx_set_config_param_
{
	unsigned long configType;				//配置的类型CONFIG_TYPE中的值
	int			  chnnNum;					//要配置那个通道
	unsigned long configElement;			//配置那个元素, CONFIG_TYPE下分不同的配置元素
}HX_SET_CONFIG_PARAM;

typedef struct _hx_net_request_info_
{
	char *pReqCmdInfo;				//HXHT 网络设备请求的信息
	unsigned long reqCmdInfoLen;	//请求信息的长度

	char *pConfigData;				//是配置的请求为配置数据
	unsigned long configDataLen;	//配置数据长度

	HX_SET_CONFIG_PARAM hxSetConfigParam;			
}HX_NET_REQUEST_INFO;

typedef struct _hx_video_download_request_info_
{
	int chnnNum;				//通道号
	int beginTime;				//开始时间 GMT
	int endTime;				//结束时间 GMT
	char szDLTicket[32];		//接入服务器发来的密钥
}HX_V_DL_REQ_INFO;


typedef struct _alarm_info_
{
	int alarmType;
	int chnnNum;
}ALARM_INFO;

//设备故障相关
typedef struct _fault_info_
{
	DEV_TYPE_E	  devType;
	unsigned long chnnNum;			//故障所在的通道
	unsigned long faultType;		//故障类型
	unsigned long faultID;			//故障编号
	unsigned long faultTime;		//故障产生时间
	unsigned long lastInformCSTime;	//上次通知接入服务器该故障的时间
	unsigned long isClear;			//新故障，还是已经恢复的故障
}FAULT_INFO;

#define  FAULT_IS_CLEAR  1
#define  FAULT_IS_NOT_CLEAR  0

#ifdef __RELEASE__
	#define	 VIDEO_LOSS_INTERVAL_TIME	10 * 60		   //视频丢失的上报间隔
#else
	#define	 VIDEO_LOSS_INTERVAL_TIME	60   //视频丢失的上报间隔
#endif
typedef enum _fault_type_
{
	FAULT_TYPE_DEVICE_ALARM			= 1,	//设备报警
	FAULT_TYPE_CAPABILITY_ALRAM		= 2,	//性能告警
	FAULT_TYPE_DEAL_WITH_ERROR		= 3,	//处理出错
	FAULT_TYPE_COMMUNICATE_ALARM	= 4,	//通信告警
	FAULT_TYPE_ENTIRONMENT_ALRAM	= 5,	//环境告警
}FAULT_TYPE;

#define FAULT_ID_VIDEO_LOSS			4		//视频丢失
#define FAULT_ID_DISK_FULL			502		//磁盘空间满


typedef struct _hx_config_info_
{
	unsigned long itemID;
	unsigned long itemType;
	unsigned long channel;
	char *pData;						//设置配置时存放从HXHT网络设备发来的配置信息, 获取配置时不处理该项
	unsigned long dataLen;				//设置配置时是pData的长度，获取配置时不处理
	unsigned long dataPosInDvrConfig;	//获取或者设置配置在该配置项中的偏移
}HX_CONFIG_INFO;

typedef enum _item_type_
{
	ITEM_TYPE_RESOLUTION	= 0,
	ITEM_TYPE_FRAME_RATE	= 1,
	ITEM_TYPE_ENCODE_TYPE	= 2,
	ITEM_TYPE_QUALITY		= 3,
	ITEM_TYPE_BITSTREAM		= 4,
}ITEM_TYPE;

const int TVT_HXHT_VERSION			= 4;

#endif //_HXGLOBAL_H

