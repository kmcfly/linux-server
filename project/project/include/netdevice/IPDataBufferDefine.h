
#ifndef _DATA_BLOCK_DEFINE_H
#define _DATA_BLOCK_DEFINE_H

enum _ext_block_status
{
	IP_BUFFER_FREE,
	IP_BUFFER_READING,
};

enum DEC_CAPABILITY
{
	DEC_CAP_D1,
	DEC_CAP_SVGA,
	DEC_CAP_XVGA,
	DEC_CAP_720P,
};

typedef enum _frame_type_common
{
	COM_FRAME_TYPE_NONE				= 0x00,
	COM_FRAME_TYPE_VIDEO			= 0x01,
	COM_FRAME_TYPE_AUDIO			= 0x02,
	COM_FRAME_TYPE_VIDEO_FORMAT		= 0x03,
	COM_FRAME_TYPE_AUDIO_FORMAT		= 0x04,
	COM_FRAME_TYPE_EVENT			= 0x05,
	COM_FRAME_TYPE_TEXT				= 0x06,
	COM_FRAME_TYPE_TALK_AUDIO		= 0x07,
	COM_FRAME_TYPE_TALK_AUDIO_FORMAT= 0x08,
	COM_FRAME_TYPE_END				= 0x09,
	COM_FRAME_TYPE_FILE_HEAD		= 0x10,
	COM_FRAME_TYPE_FILE_INFO		= 0x11,
	COM_FRAME_TYPE_JPEG				= 0x12,
}FRAME_TYPE_COMMON;


typedef struct _dvr_ip_in_frm_info
{
	int flag0;
	int flag1;
	int channel;
	int enc_stream_type;
	int data_type;
	int len;
	int vir_len;
	int keyframe;
	int width;
	int height;
	unsigned long long cur_time;
	unsigned long long relative_time;
	int offset_pos;
	int flag2;
}DVR_IP_IN_FRM_INFO;

#endif
