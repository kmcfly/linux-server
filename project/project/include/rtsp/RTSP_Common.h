#ifndef __RTSP_COMMON_H__
#define __RTSP_COMMON_H__

#include <assert.h> 

#ifdef WIN32
#include "time.h"
#include "winsock.h"
#else
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#endif

typedef struct _rtsp_local_time_
{
	short	year;			//年
	short	month;			//月
	short	mday;			//某月第几天

	short	hour;			//时
	short	minute;			//分
	short	second;			//秒

	int	microsecond;	//微秒
}RTSP_LOCAL_TIME;

typedef struct _onvif_public_utc_time_
{
	int uYear;
	int uMonth;
	int uDay;
	int uHour;
	int uMointue;
	int uSeconds;
}ONVIF_PUBLIC_UTC_TIME;

typedef struct _onvif_init_head_input_param_
{

	ONVIF_PUBLIC_UTC_TIME stDevUtcTime;

}ONVIF_INIT_INPUT_PARAM;

typedef enum _onvif_ctrl_dev_use_type_
{
	ONVIF_CTRL_DEV_USED_NORMAL,
	ONVIF_CTRL_DEV_USED_UTC_TIEM,
}ONVIF_CTRL_DEV_USE_TYPE;

typedef struct _rtsp_date_time_
{
	int	seconds;		//总秒数
	int	microsecond;	//微秒

	_rtsp_date_time_();
	_rtsp_date_time_ & operator=(tm & time);
	_rtsp_date_time_ & operator=(const timeval & time);
	_rtsp_date_time_ & operator=(const _rtsp_local_time_ & time);
	tm TM() const;
	timeval TimeVal() const;
	_rtsp_local_time_ LocalTime() const;

	_rtsp_date_time_ & operator+=(int usec);
	_rtsp_date_time_ & operator-=(int usec);

	long long Dec(const _rtsp_date_time_ & time);
	int operator-(const _rtsp_date_time_ & time);

	bool operator!=(const _rtsp_date_time_ & time) const;
	bool operator==(const _rtsp_date_time_ & time) const;
	bool operator<(const _rtsp_date_time_ & time) const;
	bool operator<=(const _rtsp_date_time_ & time) const;
	bool operator>(const _rtsp_date_time_ & time) const;
	bool operator>=(const _rtsp_date_time_ & time) const;
}RTSP_DATE_TIME;

typedef struct _rtsp_data_frame_
{
	union
	{
		unsigned int   position;	//在缓冲池中的位置
		unsigned char   *pData;		//数据区指针（TVT_DATA_FRAME + DATA）
	};
	unsigned int   length;			//缓冲区大小（sizeof(TVT_DATA_FRAME) + frame.length）

	unsigned int	deviceID;		//设备编号
	unsigned int   cameraID;		//摄像机编号

	unsigned int	freameID;		//帧编号
	unsigned int	streamID;		//流编号

	struct  
	{
		unsigned short		width;		//宽度
		unsigned short		height;		//高度

		unsigned short		bKeyFrame;	//是否为关键帧
		unsigned short		encodeType;	//编码类型

		unsigned int		length;		//数据长度
		unsigned int		streamType;	//流类型，见TVT_STREAM_TYPE

		unsigned int		channel;	//通道编号

		RTSP_LOCAL_TIME		localTime;	//格式化时间
		RTSP_DATE_TIME		time;		//标准时间
	}frame;

	unsigned int	checkBits;      //校验位

	bool operator!=(const _rtsp_data_frame_ & frame) const;
	bool operator==(const _rtsp_data_frame_ & frame) const;
}RTSP_DATA_FRAME;

typedef struct _rtsp_get_ipc_used_info
{
	int cmd;

	int uYear;
	int uMonth;
	int uDay;
	int uHour;
	int uMointue;
	int uSeconds;
}RTSP_GET_IPC_USED_INFO;

#define RTSP_FUNC_PROC_TYPE_REF_COUNT 1

typedef struct _rtsp_func_proc_callback_info
{
	int func_type;

	int channel;
	int sub_channel;
	int ref_count;

}RTSP_FUNC_PROC_CALLBACK_INFO;

typedef enum _rtsp_stream_type_
{
	RTSP_STREAM_TYPE_NONE		= 0x00,

	RTSP_STREAM_TYPE_VIDEO_1		= 0x01,			//视频1码流（对应主码流）
	RTSP_STREAM_TYPE_VIDEO_2		= 0x02,			//视频2码流（对应子码流）
	RTSP_STREAM_TYPE_VIDEO_3		= 0x03,			//视频3码流（备用）
	RTSP_STREAM_TYPE_AUDIO		= 0x04,			//音频
	RTSP_STREAM_TYPE_VOICE		= 0x05,			//对讲音频
	RTSP_STREAM_TYPE_PICTURE		= 0x06,			//图片
	RTSP_STREAM_TYPE_TEXT		= 0x07,			//文本数据
	RTSP_STREAM_TYPE_MOTION		= 0x08,			//移动侦测数据（表示移动侦测结果）

	RTSP_FRAME_TYPE_END
}RTSP_STREAM_TYPE;


int RTSPGetTime();
RTSP_DATE_TIME	RTSPGetCurrTime();
RTSP_LOCAL_TIME RTSPGetLocalTime();



#endif