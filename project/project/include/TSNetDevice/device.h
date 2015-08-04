/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : 袁石维
** Date         : 2009-06-30
** Name         : device.h
** Version      : 1.0
** Description  :
** Modify Record:
1:
***********************************************************************/
#ifndef _DEVICE_H_
#define _DEVICE_H_

//产品类型定义
typedef enum _product_type
{
	TD_2304SE_S = 100,
	TD_2308SE_S,
	TD_2316SE_S,

	TD_2304SE	= 200,
	TD_2308SE,
	TD_2316SE,

	TD_2304ME	= 300,
	TD_2308ME,
	TD_2316ME,

	TD_2304HE	= 400,
	TD_2308HE,
	TD_2316HE,

	TD_2404MD	= 500,
	TD_2408MD_4A,
	TD_2416MD_4A,
	TD_2432MD_4A,
	TD_2408MD,
	TD_2416MD,
	TD_2424MD,
	TD_2432MD,

	TD_2504ME	= 600,
	TD_2508ME,
	TD_2516ME,

	TD_2504HE	= 700,
	TD_2508HE,
	TD_2516HE,
	TD_2504HE_M,

	TD_2504MD	= 800,
	TD_2508MD,
	TD_2516MD,

	TD_2504HD	= 900,
	TD_2508HD_4A,
	TD_2516HD_4A,
	TD_2508HD,
	TD_2516HD,

	TD_2504FD	= 1000,
	TD_2508FD_4A,
	TD_2516FD_4A,
	TD_2508FD,
	TD_2516FD,

	//
	TD_2604ME	= 10100,
	TD_2608ME,
	TD_2616ME,

	TD_2604HE	= 10200,
	TD_2608HE,
	TD_2616HE,

	TD_2604MD	= 10300,
	TD_2608MD,
	TD_2616MD,

	TD_2604HD	= 10400,
	TD_2608HD,
	TD_2616HD,

	TD_2604FD	= 10500,
	TD_2608FD,
	TD_2616FD,
}PRODUCT_TYPE;

typedef enum _product_sub_id
{
	TD_TVT_IN = 0,	//国际标准版
	TD_TVT_CN = 1,	//国内标准版
	TD_TVT_SP = 2, //特殊版
	TD_TVT_US02,	//US02
	TD_TVT_RU04,	//RU04
	TD_TVT_BR27,	//BR27
	TD_TVT_TW01,	//TW01
	TD_TVT_GB14,	//GB14
	TD_TVT_IN23,	//IN23
	TD_TVT_US47,	//US47
	TD_TVT_IN25,    //IN25,ADI
	TD_TVT_PL14,    //PL14
}PRODUCT_SUB_ID;

//视频制式定义
typedef enum _video_format
{
	VIDEO_FORMAT_NTSC	= 0x00,
	VIDEO_FORMAT_PAL	= 0x01
}VIDEO_FORMAT;

typedef enum _frame_type
{
	FRAME_TYPE_NONE				= 0x00,
	FRAME_TYPE_VIDEO			= 0x01,
	FRAME_TYPE_AUDIO			= 0x02,
	FRAME_TYPE_VIDEO_FORMAT		= 0x03,
	FRAME_TYPE_AUDIO_FORMAT		= 0x04,
	FRAME_TYPE_EVENT			= 0x05,
	FRAME_TYPE_TEXT				= 0x06,
	FRAME_TYPE_TALK_AUDIO		= 0x07,
	FRAME_TYPE_TALK_AUDIO_FORMAT= 0x08,
	FRAME_TYPE_END				= 0x09,
	FRAME_TYPE_FILE_HEAD		= 0x10,
	FRAME_TYPE_FILE_INFO		= 0x11,
	FRAME_TYPE_JPEG				= 0x12,
}FRAME_TYPE;

typedef enum _frame_attrib
{
	PLAY_FRAME_NO_SHOW			= 0x01, //此帧不显示
	PLAY_FRAME_SHOW				= 0x02, //此帧可显示
	PLAY_FRAME_ALL_END			= 0x04, //数据读取结束了，后面再没有数据
	PLAY_FRAME_SEC_END			= 0x08, //此事件段结束了
	PLAY_FRAME_TIME_STAMP		= 0x10, //此帧数据有时间戳，抓图的时候注意要屏蔽打时间的功能
	LIVE_FRAME_FIRST_STREAM		= 0x20, //此帧为现场主码流
	LIVE_FRAME_SECOND_STREAM	= 0x40, //此帧为现场子码流
	PLAY_FRAME_FF				= 0x80,	//当前帧用于快进
	LIVE_FRAME_JPEG				= 0x100,
	LIVE_FRAME_TALK				= 0x200
}FRAME_ATTRIB;

//按位保存。最多不能超过32个
typedef enum _video_size
{
	DVRVIDEO_SIZE_QCIF	= 0x0001,	//QCIF
	DVRVIDEO_SIZE_CIF	= 0x0002,	//CIF
	DVRVIDEO_SIZE_HD1	= 0x0004,	//HD1
	DVRVIDEO_SIZE_D1	= 0x0008,	//D1
	DVRVIDEO_SIZE_QVGA	= 0x0010,	//QVGA
	DVRVIDEO_SIZE_VGA	= 0x0020,	//VGA
	DVRVIDEO_SIZE_XVGA	= 0x0040,	//XVGA
	DVRVIDEO_SIZE_QQVGA	= 0x0080,	//QQVGA
}VIDEO_SIZE;

typedef enum _video_stream_type
{
	VIDEO_STREAM_TYPE_NONE      = 0x00,
	VIDEO_STREAM_TYPE_PRIMARY	= 0x01,
	VIDEO_STREAM_TYPE_NETWORK	= 0x02,
	VIDEO_STREAM_TYPE_PHONE		= 0x03,
	VIDEO_STREAM_TYPE_AUDIO		= 0x04
}VIDEO_STREAM_TYPE;

//按位保存，最多只能为32个。
typedef enum _video_encode_mode
{
	VIDEO_ENCODE_MODE_VBR	=0x01,//变码流
	VIDEO_ENCODE_MODE_CBR	=0x02//固定码流
}VIDEO_ENCODE_MODE;

typedef enum _osd_type
{
	OSD_CAMERA_NAME,
	OSD_TIME_STAMP,
	OSD_TEXT_INFO,
}OSD_TYPE;

typedef struct _osd_attrib
{
	unsigned short	xPos;		//
	unsigned short	yPos;		//
	unsigned short	cx;			//
	unsigned short	cy;			//
	unsigned char	*pData;		//
	unsigned long	length;		//
}OSD_ATTRIB;

typedef struct _frame_info_ex
{
	unsigned long		keyFrame;	//0 不是关键帧

	unsigned long		frameType;
	unsigned long		length;
	unsigned long       width;
	unsigned long       height;

	unsigned char      *pData;

	unsigned long       channel;
	unsigned long       bufIndex;
	unsigned long		frameIndex;//帧索引,快进模式时，2表示2倍速，4表示4倍速
	unsigned long		frameAttrib;
	unsigned long		streamID;
	LONGLONG			time;	   //在帧绝对时间，年月日时分秒浩渺，在改变设备时间时会变化
	LONGLONG			relativeTime;//相对时间，在改变设备时间时不会变化，是个连续的时间
}FRAME_INFO_EX;	//用于本地使用

typedef struct _display_attrib
{
	unsigned short bShow;	//0表示不显示，1表示显示现场，2表示显示回放
	unsigned short chnn;	//通道号
	unsigned short xPos;	//左上角横坐标
	unsigned short yPos;	//左上角纵坐标
	unsigned short cx;		//显示区域宽度（像素）
	unsigned short cy;		//显示区域高度（像素）
	unsigned short zOrder;	//Z轴显示顺序
	unsigned short index;	//区域编号(仅APP使用)
}DISPLAY_ATTRIB;

typedef struct _motion_area_info
{
	unsigned short gridWidth;	//每行有多少个栈格
	unsigned short gridHeight;	//每列有多少个栈格
	unsigned char  pData[640];	//((1280 / 16) * (1024 / 16)) / 8[sizeof(unsigned char)] = 640
}MOTION_AREA_INFO;

//结构体
typedef struct _key_value_info
{
	unsigned long	type;
	unsigned long	isDown;
	unsigned long	value;
	unsigned long	xPos;
	unsigned long	yPos;
}KEY_VALUE_INFO;

typedef enum _mouse_key_value
{
	E_MOUSE_KEY_MOUSE_MOVE		=0x01,	//鼠标移动
	E_MOUSE_KEY_LEFT_DOWN		=0x02,	//左键按下
	E_MOUSE_KEY_LEFT_DOWN_MOVE	=0x03,	//左键按下拖动
	E_MOUSE_KEY_LEFT_UP			=0x04,	//左键弹起
	E_MOUSE_KEY_LEFT_DBCLICK	=0x08,	//左键双击
	E_MOUSE_KEY_RIGHT_DOWN		=0x10,	//右键按下
	E_MOUSE_KEY_RIGHT_DOWN_MOVE	=0x11,	//右键按下拖动
	E_MOUSE_KEY_RIGHT_UP		=0x20,	//右键弹起
	E_MOUSE_KEY_RIGHT_DBCLICK 	=0x40,	//右键双击
	E_MOUSE_KEY_LEFTRIGHT_DOWN	=0x80,	//同时按下左右键
	E_MOUSE_KEY_WHEEL_FORWARD	=0x100,	//向前滑动
	E_MOUSE_KEY_WHEEL_BACK		=0x200,	//向后滑动
	E_MOUSE_KEY_WHEEL_DOWN		=0x400,	//滑轮按下
	E_MOUSE_KEY_WHEEL_UP		=0x800	//滑轮弹起
}MOUSE_KEY_VALUE;

//消息类型
typedef enum _message_type 
{
	KEY_TYPE_LOCAL		= 0x0001,	//机器键盘
	KEY_TYPE_REMOTE		= 0x0002,	//遥控键盘
	KEY_TYPE_MOUSE		= 0x0004,	//鼠标
	KEY_TYPE_MY1003		= 0x0008,	//名扬1003K键盘
	KEY_TYPE_VIRTUAL	= 0x0010,	//虚拟键，在特殊时候使用	
	KEY_TYPE_TWOEM		= 0x0011,	//台湾OEM厂商485外接键盘
}MESSAGE_TYPE;

typedef enum _display_mode
{
	DISPLAY_NONE,
	DISPLAY_LIVE,
	DISPLAY_PLAYBACK,
	DISPLAY_NET,
	DISPLAY_HIDE,
	DISPLAY_INVISIBLE
}DISPLAY_MODE;

typedef enum _video_out_type
{
	VIDEO_OUT_NONE,
	VIDEO_OUT_HDMI,
	VIDEO_OUT_VGA,
	VIDEO_OUT_CVBS
}VIDEO_OUT_TYPE;




#endif //_DEVICE_H_
