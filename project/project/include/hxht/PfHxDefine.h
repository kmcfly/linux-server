/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : liwenxie
** Date         : 2010-03-30
** Name         : PfHxDefine.h
** Version      : 1.0
** Description  : 
				此头文件里定义的结构体定义的功能都要求设备主程序支持，
				结构体的使用方式在CHxPull及CHxPush。
			//  对设备的要求:
			//	其中一些功能/参数，设备没有提供，需要设备进行完善的:
			//	1. 图像遮挡，遮挡区域，这个可以延伸到图像遮挡报警一项
			//	2. 图像叠加字幕OSD，设备只提供了时间的叠加，需要提供标题的叠加，
			//	    同时提供时间和标题叠加位置的参数，最好能提供Point类型的位置
			//	3. 语音，提供语音的编解码功能，编码压缩上传，本地解码播放
			//	4. PTZ控制，提供云台控制，预置点，自动归位，自动巡航等功能；
			//	5. 云台控制提供透传功能；
			//	6. 云台参数，需要提供数据位，停止位，校验位，流控方式的功能；
***********************************************************************/
#ifndef _PFHXDEFINE_H
#define _PFHXDEFINE_H




enum _HX_OPTYPE_E
{
	TIME_OP 	= 1,
	MANUAL_OP	= 2,
};

//用于标识数据帧类型
enum _HX_FRAME_TYPE_E
{
	REAL_VIDEO = 1,		//实时视频数据
	REAL_AUDIO = 2,		//实时音频数据
	VOD_VIDEO = 3,		//回放视频数据
	VOD_AUDIO = 4,		//回放音频数据
};

//用于表示点坐标
typedef struct _PfPos
{
	int nX;
	int nY;
}PfPos;

//用于表示矩形
typedef struct _PfRect
{
	int nLeft;		//最左边x值
	int nTop;		//最上面y值
	int nWidth;		//矩形宽值
	int nHeight;	//矩形高值
}PfRect;

//表示时间
typedef struct _PfDateTime
{
	int nYear;
	int nMonth;
	int nDay;
	int nHour;
	int nMinute;
	int nSecond;
}PfDateTime;


//用于接入互信互通的配置参数
//此参数需要永久保存在flash内，随着系统一起加载
//用于初始化互信互通接入程序
typedef struct _hx_net_init_info_
{
	char szDeviceID[32];		//设备ID
	char szStoreID[32];			//存储设备ID
	char szLocalIP[16];
	char szCsgIP[16];			//接入服务器IP
	unsigned short nCsgPort;	//设备注册到接入服务器的端口
	unsigned short nStorePort;	//存储注册到接入服务器的存储端口
	unsigned short nMsgPort;	//设备侦听的消息端口
	unsigned short nVideoPort;	//设备侦听的视频端口
	unsigned short nAudioPort;	//设备侦听的音频端口
	unsigned short nUpdatePort;	//设备侦听的更新端口
	unsigned short nWebPort;	//设备侦听的Web端口
	unsigned short dwMaxSensorIn;	//传感器个数
	unsigned long  dwMaxSensorOut; //报警输出最大个数

	unsigned char	dwMaxVideo;
	unsigned char	dwMaxAudio;
	unsigned char	dwMaxLink;
	unsigned char	dwDeviceType;

	unsigned long	dwSoftVer;
	unsigned long	dwBuildTime;

	char			deviceName [20];
	char			szMac[8];//只用了前6个字节
}HX_NET_INIT_INFO;


//注册后HXHT设备返回的会话属性
typedef struct _hx_net_message_info_
{
	char szMessageID[32];
	char szName[64];
	int  heartCycle;
	char szSysRebootTime[32];
	int sysRebootCycle;
	char szDispatchId[32];
	char szPlatformTel[32];
	char configServer[48];
	char curDateTime[48];
}HX_NET_MESSAGE_INFO;

//存储注册的相关信息
typedef struct _hx_net_store_info_
{
	char	szStoreID[64];
	int		stortHeartCycle;
	int		maxInputNum;
	int		maxOutputNum;
	char	szFtpUser[256];
	char	szFtpPass[256];
	int		ftpPort;
}HX_NET_STORE_INFO;

//视频编码参数
typedef struct _PfViEncCfg
{
	int nFrameRate;			//帧率
	int nIFrameInterval;	//I帧间隔
	int nQuality;			//图像质量
	int nBitRate;			//码率
	int nFormat;			//分辨率
}PfViEncCfg;

//视频图像参数
typedef struct _PfViParam
{
	int nBrightness;		//亮度
	int nContrast;			//对比度
	int nSaturation;		//饱和度
	int nHue;				//色度
}PfViParam;

//音频编码参数
typedef struct _PfAuEncCfg
{
	int nChannels;			//语音通道数
	int wBitsPerSample;		//采样深度
	int nSamplesPerSec;		//采样速度
	int nCompressedFormat;	//压缩格式
}PfAuEncCfg;

//音频解码参数
typedef struct _PfAuDecCfg
{
	int nChannels;			//语音通道数
	int wBitsPerSample;		//采样深度
	int nSamplesPerSec;		//采样速度
	int nCompressedFormat;	//压缩格式
}PfAuDecCfg;


//图像遮挡参数
typedef struct _PfImageCover
{
	char nEnable;		//启用
	int nNum;			//实际遮挡区域个数
	PfRect strRect[8];	//遮挡区域
}PfImageCover;

//图像叠加OSD参数
typedef struct _PfImageOSD
{
	char nEnable;		//是否启用
	char nTime;			//是否叠加时间OSD
	char nTitle;			//是否叠加标题OSD
	char nBitRate;		//是否叠加码率OSD
	char szTitle[32];	//标题内容
	PfPos TimePos;		//时间OSD位置
	PfPos TitlePos;		//标题OSD位置
	PfPos BitRatePos;	//码率OSD位置
}PfImageOSD;

//移动侦测参数
typedef struct _PfMoveDetect
{
	char nEnable;		//是否启用
	char nDetectGrade;	//灵敏度
	short nNum;			//实际区域数
	PfRect strRect[8];	//移动侦测区域
}PfMoveDetect;

//网络参数
typedef struct _PfNet
{
	char nWifiEnable;
	char szIP[16];		//IP地址
	char szNetMask[16];	//子网掩码
	char szGateWay[16];	//网关
}PfNet;

//485串口参数
typedef struct _Pf485Param
{
	int nSpeed;		//波特率
	int nData;		//数据位
	int nStop;		//停止位
	int nCheck;		//校验位
	int nControl;	//流控方式
}Pf485Param;

//报警输出参数
typedef struct _PfAlarmOut
{
	int nEnable;	//启用报警输出
}PfAlarmOut;





//设置硬盘策略
typedef struct _PfDiskInfo
{
	char nExist;			//是否存在硬盘
	char nRecordType;	//满停/循环写入
	char nRecord;		//是否支持录像
	char nSnap;			//是否支持抓拍
	unsigned long nDiskSpace;	//磁盘空间大小
}PfDiskInfo;


//录像参数
typedef struct _PfRecordCfg
{
	int nType;		//定时录像或者手动录像(HX_OP_E)
	int nFileLen;	//文件长度
	char szTimeCfg[7][24];	//配置定时录像时间(手动录像此项无效)
}PfRecordCfg;

//抓拍参数
typedef struct _PfSnapCfg
{
	int nType;		//定时抓拍或者手动抓拍(HX_OP_E)
	char szTimeCfg[7][24];	//配置定时抓拍时间(手动抓拍此项无效)
}PfSnapCfg;



//录像查询
typedef struct _PfFileQuery
{
	int nChn;			//通道号
	int nFileType;		//文件类型  录像/抓拍
	int nRecordType;	//定时/手动/报警
	char szFileName[64];	//文件名
	unsigned long lBegin;	//开始时间
	unsigned long lEnd;		//结束时间
}PfFileQuery;

//文件信息
typedef struct _PfFileInfo
{
	int nFileType;		//文件类型，录像/抓拍
	int nRecordtype;	//定时/手动/报警
	char szFileName[32];	//文件类型
	unsigned long lFileTime;//记录时间
}PfFileInfo;

//查询结果
typedef struct _PfQueryResult
{
	int nFileNum;			//实际返回记录个数
	int nRemainNum;			//仍然有多少个记录
	PfFileInfo FileInfo[20];	//文件信息
}PfQueryResult;

//云台控制
typedef struct _PfPTZControl
{
	int nOp;	//操作命令
}PfPTZControl;

//预置点描述
typedef struct _PfPresetPoint
{
	int nPresetNo;
	char szPresetName[16];
}PfPresetPoint;

//预置点信息
typedef struct _PfPreset
{
	int nPresetNum;			//预置点个数
	PfPresetPoint Point[10];//预置点
}PfPreset;



//设备工作状态，包括报警状态，设备硬件状态
typedef struct _PfDvsStatus
{
	char nAlarmIn;			//是否输入报警(红外)
	char nMotionAlarm;		//是否移动侦测报警
	char nCoverAlarm;		//是否遮挡报警
	char nVLostAlarm;		//是否视频丢失报警
	int nCPUUseRatio;		//CPU使用率
	int nMemUseRatio;		//内存使用率
	int nDiskSpaceRatio;	//磁盘空间使用率
	int nDiskSpaceBalance;
	int nProcNum;			//当前系统进程数
	unsigned long nCurTime;	//系统当前时间
}PfDvsStatus;
















#endif //_PFHXDEFINE_H

