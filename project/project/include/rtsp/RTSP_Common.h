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
	short	year;			//��
	short	month;			//��
	short	mday;			//ĳ�µڼ���

	short	hour;			//ʱ
	short	minute;			//��
	short	second;			//��

	int	microsecond;	//΢��
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
	int	seconds;		//������
	int	microsecond;	//΢��

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
		unsigned int   position;	//�ڻ�����е�λ��
		unsigned char   *pData;		//������ָ�루TVT_DATA_FRAME + DATA��
	};
	unsigned int   length;			//��������С��sizeof(TVT_DATA_FRAME) + frame.length��

	unsigned int	deviceID;		//�豸���
	unsigned int   cameraID;		//��������

	unsigned int	freameID;		//֡���
	unsigned int	streamID;		//�����

	struct  
	{
		unsigned short		width;		//���
		unsigned short		height;		//�߶�

		unsigned short		bKeyFrame;	//�Ƿ�Ϊ�ؼ�֡
		unsigned short		encodeType;	//��������

		unsigned int		length;		//���ݳ���
		unsigned int		streamType;	//�����ͣ���TVT_STREAM_TYPE

		unsigned int		channel;	//ͨ�����

		RTSP_LOCAL_TIME		localTime;	//��ʽ��ʱ��
		RTSP_DATE_TIME		time;		//��׼ʱ��
	}frame;

	unsigned int	checkBits;      //У��λ

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

	RTSP_STREAM_TYPE_VIDEO_1		= 0x01,			//��Ƶ1��������Ӧ��������
	RTSP_STREAM_TYPE_VIDEO_2		= 0x02,			//��Ƶ2��������Ӧ��������
	RTSP_STREAM_TYPE_VIDEO_3		= 0x03,			//��Ƶ3���������ã�
	RTSP_STREAM_TYPE_AUDIO		= 0x04,			//��Ƶ
	RTSP_STREAM_TYPE_VOICE		= 0x05,			//�Խ���Ƶ
	RTSP_STREAM_TYPE_PICTURE		= 0x06,			//ͼƬ
	RTSP_STREAM_TYPE_TEXT		= 0x07,			//�ı�����
	RTSP_STREAM_TYPE_MOTION		= 0x08,			//�ƶ�������ݣ���ʾ�ƶ��������

	RTSP_FRAME_TYPE_END
}RTSP_STREAM_TYPE;


int RTSPGetTime();
RTSP_DATE_TIME	RTSPGetCurrTime();
RTSP_LOCAL_TIME RTSPGetLocalTime();



#endif