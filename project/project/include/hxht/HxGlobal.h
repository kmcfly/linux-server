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


#define TVTHX_SUPPORT_PB			1		//֧��ǰ�˻ط� 0 ��֧��     
#define TVTHX_SUPPORT_SD			0		//ǰ��֧��sd�� 0 ��֧��(dvs�汾Ӧ�ùرոú�)     
#define TVTHX_SUPPORT_SCHEME		1		//ǰ��֧��Ԥ��(����3.0Э��汾��ʼ֧��, IPC�汾Ӧ�ùرոú�) 
#define TVTHX_SUPPORT_VIDEO2		1		//֧�ֵڶ�����ƵЭ�� 0 ��֧�� (Ӧ��һֱ��Ϊ1,����Э����ָ��ߵİ汾)
#define TVTHX_SUPPORT_NAT			0		//ǰ���豸֧��NAT��Խ 0 ��֧��
#define TVTHX_SUPPORT_PTZ_LOCK		1		//ǰ��֧����̨���� 0 ��֧��(IPC�汾Ӧ�ùرոú�)
#define TVTHX_SUPPORT_MAINSTREAM	1		//ȱʡ����������֧��
#define TVTHX_SUPPORT_STREAM1		0		//������1
#define TVTHX_SUPPORT_STREAM2		0		//������2
#define TVTHX_SUPPORT_STREAM3		0		//������3


#define TVTHX_MSG_VERSION			257


#define TVTHX_MAX_MSG_SIZE			(3 * 1024)
#define TVTHX_KA_MSG_SIZE			(1.5 * 1024)
#define TVTHX_OK_FLAG				0

#ifdef __RELEASE__
	#define TVTHX_MAX_TCP_TIMEO			20000		//��ʱʱ����30000�ĳ�3000  by lwx 2010-04-7
#else
	#define TVTHX_MAX_TCP_TIMEO			100		
#endif

#ifdef __RELEASE__
	#define TVTHX_MAX_REPLAY_WAIT_TIME		36		//DVR���������Ӧ��ʱʱ��(s)���������6����û���յ����ݾ���Ҫ����ע���¼
#else
	#define TVTHX_MAX_REPLAY_WAIT_TIME		20		
#endif


#ifdef __RELEASE__
#define TVTHX_MAX_AVSEND_WAIT_TIME		120		//��������Ƶ��ʱʱ��
#else
#define TVTHX_MAX_AVSEND_WAIT_TIME		120		
#endif

#define TVTHX_MAX_VIDEO_PACK_SIZE	(9   * 1024)		//��Ƶ����Ĵ�С
#define TVTHX_CMD_BUF_LEN			(256 * 1024)
#define TVTHX_REG_MSG_SIZE			(8   * 1024)

//�豸��������
//#define TVTHX_MAX_AUDIO				4
//#define TVTHX_MAX_CAM				16	//һ���豸֧�����ͨ����
#define TVTHX_CAM_VIDEO_CON			4	//һ·֧�ֵ������Ƶ������
#define TVTHX_MAX_ALARM_IN			2	//���������豸��
#define TVTHX_MAX_ALARM_OUT			2	//��������豸��
#define TVTHX_MAX_VMASK_AREA		4
#define TVTHX_MAX_VMOTION_AREA		4

//�豸���ӷ�ʽ
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

	TVTHX_REMOTE_SNAP_REQ_MSG			= 1048,	//�ֶ�ץ������
	TVTHX_REMOTE_SNAP_RET_MSG			= 1049,	//�ֶ�ץ��Ӧ��

	TVTHX_REG_REQ_MSG					= 2002,	//ע������
	TVTHX_REG_RET_MSG					= 2003,	//ע��Ӧ��
	TVTHX_GET_NETMANAGER_REQ_MSG		= 2004, //����������������
	TVTHX_GET_NETMANAGER_RET_MSG		= 2005, //������������Ӧ��

	TVTHX_VIDEO_DISCONN_REQ_MSG			= 2012,	//�Ͽ���Ƶ��������
	TVTHX_VIDEO_DISCONN_RET_MSG			= 2013,	//�Ͽ���Ƶ����Ӧ��


	TVTHX_STORE_REG_MSG					= 4002,	//�洢ע������
	TVTHX_STORE_RET_MSG					= 4003,	//�洢ע��Ӧ��

	TVTHX_V_DOWNLOAD_TYPE_REQ_MSG		= 5004,	//���ط�ʽ��ѯ
	TVTHX_V_DOWNLOAD_TYPE_RET_MSG		= 5005,	//���ط�ʽӦ��
	TVTHX_CTRL_PTZ_REQ_MSG				= 5006,	//�����ƾ�����
	TVTHX_CTRL_PTZ_RET_MSG				= 5007,	//�����ƾ�Ӧ��
	TVTHX_VIDEO_ACCREDIT_REQ_MSG1		= 5010,	//��һ����Ƶ����
	TVTHX_VIDEO_ACCREDIT_REQ_MSG2		= 6002,	//�ڶ�����Ƶ����
	TVTHX_VIDEO_ACCREDIT_RET_MSG1		= 5011,	//��һ����Ƶ����
	TVTHX_VIDEO_ACCREDIT_RET_MSG2		= 6003,	//�ڶ�����Ƶ����

	TVTHX_SET_VIDEO_PARA_REQ_MSG		= 5032,	//������Ƶ��������
	TVTHX_SET_VIDEO_PARA_RET_MSG		= 5033,	//������Ƶ����Ӧ��
	TVTHX_GET_VIDEO_PARA_REQ_MSG		= 5034,	//��ȡ��Ƶ��������
	TVTHX_GET_VIDEO_PARA_RET_MSG		= 5035,	//��ȡ��Ƶ����Ӧ��

	TVTHX_GET_PTZ_STATUS_REQ_MSG		= 5042,	//��ȡ�ƾ�״̬����
	TVTHX_GET_PTZ_STATUS_RET_MSG		= 5043,	//��ȡ�ƾ�״̬Ӧ��
	TVTHX_DEV_CONFIG_REQ_MSG			= 5046,	//����DVS��������
	TVTHX_DEV_CONFIG_RET_MSG			= 5047,	//����DVS����Ӧ��

	TVTHX_SET_ALARM_OUTPUT_REQ_MSG		= 5064,	//���ñ������״̬����
	TVTHX_SET_ALARM_OUTPUT_RET_MSG		= 5065,	//���ñ������״̬Ӧ��
	TVTHX_GET_ALARM_OUTPUT_REQ_MSG		= 5066,	//��ȡ�������״̬����
	TVTHX_GET_ALARM_OUTPUT_RET_MSG		= 5067,	//��ȡ�������״̬Ӧ��

	TVTHX_SET_MOTION_GUARD_REQ_MSG		= 5080,	//�����ƶ���Ⲽ��ʱ������
	TVTHX_SET_MOTION_GUARD_RET_MSG		= 5081,	//�����ƶ���Ⲽ��ʱ��Ӧ��
	TVTHX_GET_MOTION_GUARD_REQ_MSG		= 5082,	//��ѯ�ƶ���Ⲽ��ʱ��
	TVTHX_GET_MOTION_GUARD_RET_MSG		= 5083,	//�ظ��ƶ���Ⲽ��ʱ��
	TVTHX_SET_ALARMIN_GUARD_REQ_MSG		= 5084,	//���ñ������벼��ʱ������
	TVTHX_SET_ALARMIN_GUARD_RET_MSG		= 5085,	//���ñ������벼��ʱ��Ӧ��
	TVTHX_GET_ALARMIN_GUARD_REQ_MSG		= 5086,	//��ѯ�������벼��ʱ��
	TVTHX_GET_ALARMIN_GUARD_RET_MSG		= 5087,	//�ظ��������벼��ʱ��

	TVTHX_SET_ALARMOUT_GUARD_REQ_MSG	= 5096,	//���ñ������벼��ʱ������
	TVTHX_SET_ALARMOUT_GUARD_RET_MSG	= 5097,	//���ñ������벼��ʱ��Ӧ��
	TVTHX_GET_ALARMOUT_GUARD_REQ_MSG	= 5098,	//��ѯ�������벼��ʱ��
	TVTHX_GET_ALARMOUT_GUARD_RET_MSG	= 5099,	//�ظ��������벼��ʱ��

	TVTHX_SET_SCHEME_REQ_MSG			= 5208,//������������
	TVTHX_SET_SCHEME_RET_MSG			= 5209,//���������Ļظ�

	TVTHX_DEL_SCHEME_REQ_MSG			= 5210,//ɾ����������
	TVTHX_DEL_SCHEME_RET_MSG			= 5211,//ɾ�����������Ļظ�

	TVTHX_REPORT_ALARM_REQ_MSG			= 5214,	//����֪ͨ����
	TVTHX_REPORT_ALARM_RET_MSG			= 5215,	//����֪ͨӦ��

	TVTHX_MANUAL_STOP_SCHEME_MSG		= 5216,

	TVTHX_MANUAL_START_TRACK_REQ_MSG	= 5218,
	TVTHX_MANUAL_START_TRACK_RET_MSG	= 5219,

	TVTHX_MANUAL_START_SCHEME_REQ_MSG	= 5220,
	TVTHX_MANUAL_START_SCHEME_RET_MSG	= 5221,

	TVTHX_START_SCHEME_REQ_MSG			= 5214, 


	TVTHX_AUDIO_ACCREDIT_REQ_MSG		= 6110,	//������DVS������������
	TVTHX_AUDIO_ACCREDIT_RET_MSG		= 6111,	//DVS����������Ӧ��

	TVTHX_AUDIO_CONN_REQ_MSG			= 6112,
	TVTHX_AUDIO_CONN_RET_MSG			= 6113,
	TVTHX_SEND_AUDIO_DATA_MSG			= 6114,

	TVTHX_EXT_REQ_MSG					= 6200,
	TVTHX_EXT_RET_MSG					= 6201,

	TVTHX_VIDEO_CONN_REQ_MSG			= 8000,	//�ͻ������豸������Ƶ
	TVTHX_VIDEO_CONN_RET_MSG			= 8001,
	TVTHX_MANUFACTURER_RET_MSG 			= 8002,
	TVTHX_SEND_VIDEO_DATA_MSG			= 8004,

	TVTHX_UPDATE_CHECK_REQ_MSG			= 8010,	//�汾��֤
	TVTHX_UPDATE_CHECK_RET_MSG			= 8011,	//�汾��֤�ظ�
	TVTHX_UPDATE_MSG_REQ_MSG			= 8012,	//����֪ͨ
	TVTHX_UPDATE_MSG_RET_MSG			= 8013,	//����֪ͨ�ظ�

	TVTHX_VIDEO_DOWNLOAD_REQ_MSG		= 8308,	//¼����������
	TVTHX_VIDEO_DOWNLOAD_REP_MSG		= 8309,	//¼�����ػ�Ӧ

	TVTHX_FIRST_VIDEO_DOWNLOAD_REQ_MSG	= 8300, //�׸�¼���ļ���������
	TVTHX_FIRST_VIDEO_DOWNLOAD_RET_MSG	= 8301,	//�׸�¼���ļ����ص�Ӧ��
	TVTHX_NEXT_VIDEO_DOWNLOAD_REQ_MSG	= 8302,	//�����ļ���������
	TVTHX_NEXT_VIDEO_DOWNLOAD_RET_MSG	= 8303,	//�����ļ����ص�Ӧ��
	TVTHX_VIDEO_DOWNLOAD_KEEPING_MSG	= 8304,	//¼������ʱ�ͻ��˷��ı���������Ϣ
	
}TVTHX_MSG_CMD;

//��̨����
typedef enum _hxht_ptz_cmd_
{
	TVTHX_PTZ_CMD_STOP				= 0,		//���ڰ汾		
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
	TVTHX_PTZ_CMD_ACTION			= 8200,	  //�°汾				
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
	STREAM_READY	= 3,	//Stream������ƥ��Ticket�ɹ����״̬
	STREAM_RUN		= 4		//�豸������Ƶ��ʼ��Ϣ��,Stream���������״̬�·�����Ƶ
}STREAM_STATUS_E;
typedef enum _video_param_type_
{
	VIDEO_PARAM_TYPE_MAJOR		= 1,		//��Ҫ��Ƶ����
	VIDEO_PARAM_TYPE_PICQU		= 2,		//ͼ������
	VIDEO_PARAM_TYPE_MAXSTREAM	= 3,		//�������
	VIDEO_PARAM_TYPE_PICTYPE	= 4,		//ͼ���ʽ
	VIDEO_PARAM_TYPE_BLOCKAREA	= 5,		//�ڵ�����
	VIDEO_PARAM_TYPE_OSD		= 6,		//OSD��logo
	VIDEO_PARAM_TYPE_COLOR		= 7,		//ɫ��
	VIDEO_PARAM_TYPE_MOVEBLOCK	= 8,		//�ƶ��������
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
	char    front_device_transaction_id[32];//ǰ���豸(��Ƶ������)ҵ��id
	char    camera_id[4][32];//ǰ���豸(����ͷ)id	
	char    camera_transaction_id[4][32];//ǰ���豸(����ͷ)ҵ��id
	char    alarm_in_dev_id[4][32];		//�澯����id
	char    alarm_out_dev_id[4][32];     //�澯���id
	char    alarm_in_transaction_id[4][32];		//�澯����id
	char    alarm_out_transaction_id[4][32];     //�澯���id	
	char    store_dev_id[64];							//�洢ģ��id
	char    store_transaction_id[64];				    //�洢ģ��ҵ��id
	char    front_device_naming[128];
	char    camera_naming[4][128];
	char    alarm_in_naming[4][128];
	char    alarm_out_naming[4][128];
	char    center_id[32];                               //������������id
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
	int chnnNum;				//�����ͨ��
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




//------------------��Ϣͷ---------------//
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

//------------------��Ϣͷ--------------//



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
//	unsigned char	nSize;			//�ṹ���С
//	unsigned int	nWidth;			//��Ƶ��
//	unsigned int	nHeight;		//��Ƶ��
//	unsigned long	nCompression;	//��Ƶѹ����ʽ
//	unsigned char	nReserved1;
//	unsigned char	nReserved2;
//}TVTHX_STREAM_HEAD;

typedef struct _TVTHX_STREAM_HEAD
{
	char			szCompany[32];
	unsigned long	nSize;			//�ṹ���С
	unsigned long	nWidth;			//��Ƶ��
	unsigned long	nHeight;		//��Ƶ��
	unsigned long	nCompression;	//��Ƶѹ����ʽ
	unsigned long	waveFormat;		//��Ƶ��ʽ��WAVE_FORAMT_PCMΪ��׼ԭʼ������Ƶ��ʽ��
	unsigned short	waveBitPerSample;//��Ƶ�����ȼ�
	unsigned short	waveChannels;	//��������
	unsigned long	waveSample;		//��Ƶ����Ƶ��
}TVTHX_STREAM_HEAD;

// typedef struct _FRAME_HEAD
// {
// 	unsigned char avType;  //������ HXAV_TYPE_VIDEO HXAV_TYPE_AUDIO ��������
// 	unsigned char frameType; //֡����	FRAME_CODE_TYPE
// 	unsigned char packageFlag;//������	PACK_SERIAL_TYPE
// 	unsigned char size;  //�ṹ���С
// 	unsigned int  nDataLen;
// 	unsigned char reserved[4];//����
// }FRAME_HEAD;


typedef struct _FRAME_HEAD
{
	unsigned long	frameType; //֡���� FRAME_CODE_TYPE
	unsigned long	packageFlag;//������ PACK_SERIAL_TYPE
	unsigned long	nPackLen;
	unsigned long	nFrameLen;
	unsigned long	width;
	unsigned long	height;
	unsigned long	dataPos;	//��Ƭʱ��ŷ�Ƭ�������еĿ�ʼλ��
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
	HX_CMD_TYPE_REQUEST_FROM_DVR	= 0,					//��DVR����������
	HX_CMD_TYPE_REPLAY_FROM_DVR,							//DVR��Ӧ����Ϣ
	HX_CMD_TYPE_REQUEST_FROM_NET,							//���絽�������
	HX_CMD_TYPE_REPLAY_FROM_NET,							//���絽���Ӧ��
	HX_CMD_TYPE_VIDEO,
	HX_CMD_TYPE_AUDIO,
}HX_CMD_TYPE;

typedef enum _video_status_									//�ο�HXHTʵʱ��Ƶ���ӹ���
{
	VIDEO_STATUS_NONE				= 0,					//û����Ƶ
	VIDEO_STATUS_REQ_VIDEO			= 1,					//�ͻ�����Ƶ����DVR�����⵽��״̬
	VIDEO_STATUS_TELL_SERVER		= 2,					//���������֪ͨDVR
	VIDEO_STATUS_CONNECT_CLIENT		= 3,					//��������ͻ��ˣ���DVR��������ʱ���ڸ�״̬
	VIDEO_STATUS_REP_CONNECT_CLIENT = 4,					//DVR�ظ��ͻ��˵�����
	VIDEO_STATUS_REP_TELL_SERVER	= 5,					//2�Ļ�Ӧ
	VIDEO_STATUS_REP_VIDEO			= 6,					//
	VIDEO_STATUS_CLT_CT_DVR			= 7,					//�ͻ�������DVR����Ƶ�˿ڣ�ֻ�б������������״̬					
	VIDEO_STATUS_REP_CLT_CT_DVR		= 8,					//7�Ļ�Ӧ
	VIDEO_STATUS_START_VIDEO		= 9,					//DVR֪ͨ�����������ʼ��Ƶ����
	VIDEO_STATUS_REP_START_VIDEO	= 10,					//�����������9�Ļ�Ӧ
	VIDEO_STATUS_SND_VIDEO_HEAD		= 11,					//DVR������Ƶͷ���ͻ���
	VIDEO_STATUS_VIDEO_STREAM		= 12,					//��Ƶ��	
	VIDEO_STATUS_REQ_STOP_VIDEO		= 13,					//DVR������������ֹͣ��Ƶ
	VIDEO_STATUS_REP_STOP_VIDEO		= 14,					//����������ظ�
	VIDEO_STATUS_REP_CLIENT_STOP_REQ = 15,					//�ͻ���������Ƶֹͣ

	//��ʷ��Ƶ����ʱ��״̬
	VIDEO_STATUS_DL_REQ_VIDEO		= 16,
	VIDEO_STATUS_HISTORY_PLAY_HEAD	= 17,
	VIDEO_STATUS_HISTORY_PLAYING	= 18,
	VIDEO_STATUS_HISTORY_NEXT_FILE	= 19,
	VIDEO_STATUS_HISTORY_END		= 20,
}VIDEO_STATUS;


typedef struct _hx_cmd_data
{
	int				hxMsgVer;									//���Ż�ͨ��Ϣ�汾
	unsigned long	cmdType;									//��Ϣ������
	unsigned long	cmdNum;										//���Ż�ͨ��Ϣ���ͣ� HX_CMD_TYPE�е�ֵ
	unsigned long	clientPort;									//�ͻ��˶˿�
	char			clientIP[24];								//�ͻ���IP
	SWL_socket_t	fromSocketFd;								//��Ϣ����Դ�� �����ĸ��׽ӿڣ���Ӧ��Ϣʱ��������׽ӿڷ�������
	int				cmdGenerateTime;							//��Ϣ������ʱ�䣬������Ϣʱ�ǽ���ʱ�䣬������Ϣʱ�Ƿ���ʱ�䣬������Ϣʱ�Ѹ�ʱ������ָ�������
																//��������Ϣ�ظ��е�ָ����ź����ʱ��һ���ģ�����������ؼ����������������Ϣ
	int				videoStatus;								//��¼��Ƶ��״̬
	bool			isTalk;
	int				chnnNum;
	char			ticket[32];
	char			notused[120];
	char			*pData;										//��Ϣ���ݣ���ʽΪTVTHX_RETURN_MSG_HEAD+xml���ݣ�����_TVTHX_RETURN_MSG_HEAD+xml����
	unsigned long	buffLen;									//����������
	unsigned long	dataLen;									//*pData�����ݵĳ��� 
	unsigned long	dataPos;									//�ڽ���ʱ��ʾ�Ѿ����յ������ݣ�����ʱ��ʾ�Ѿ����͵�����
	char			*pRecData;
	unsigned long	recBufLen;
	unsigned long	recDataLen;
	unsigned long	recDataPos;
	LONGLONG		lastSRtime;									//�ϴη��ͻ��������ʱ��
	unsigned long	hxGetSetConfig;								//ÿ���ͻ���һ��CHxGetSetConfig��������������Ϣ
	unsigned long	clientID;									//�ͻ���ID
	unsigned long	pHxAVDataSendMan;							//ÿ���ͻ��˰�һ������Ƶ����
}HX_CMD_DATA;


typedef struct _hx_dvr_req_cmd_info_
{
	TVTHX_MSG_CMD cmdNum;
	SWL_socket_t  clientSockfd;
	char *pParam;
	unsigned long dataLen;
}HX_DVR_REQ_CMD_INFO;

//��ʼ��ֹͣ��Ƶ�Ĳ���
typedef struct  _start_stream_param_
{
	char szTicket[32];		//��ͨ����Ƶ����ʱЭ�̵���Կ
	unsigned long devType;	//�豸���� DEV_TYPE_E
	unsigned long isStart;  //��ʼ���ǹر���Ƶ�� 0Ϊ�رգ���0Ϊ��ʼ
	unsigned long streamType; //��Ƶ���ͣ� ����������������STREAM_TYPE_E
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
	unsigned long configType;				//���õ�����CONFIG_TYPE�е�ֵ
	int			  chnnNum;					//Ҫ�����Ǹ�ͨ��
	unsigned long configElement;			//�����Ǹ�Ԫ��, CONFIG_TYPE�·ֲ�ͬ������Ԫ��
}HX_SET_CONFIG_PARAM;

typedef struct _hx_net_request_info_
{
	char *pReqCmdInfo;				//HXHT �����豸�������Ϣ
	unsigned long reqCmdInfoLen;	//������Ϣ�ĳ���

	char *pConfigData;				//�����õ�����Ϊ��������
	unsigned long configDataLen;	//�������ݳ���

	HX_SET_CONFIG_PARAM hxSetConfigParam;			
}HX_NET_REQUEST_INFO;

typedef struct _hx_video_download_request_info_
{
	int chnnNum;				//ͨ����
	int beginTime;				//��ʼʱ�� GMT
	int endTime;				//����ʱ�� GMT
	char szDLTicket[32];		//�����������������Կ
}HX_V_DL_REQ_INFO;


typedef struct _alarm_info_
{
	int alarmType;
	int chnnNum;
}ALARM_INFO;

//�豸�������
typedef struct _fault_info_
{
	DEV_TYPE_E	  devType;
	unsigned long chnnNum;			//�������ڵ�ͨ��
	unsigned long faultType;		//��������
	unsigned long faultID;			//���ϱ��
	unsigned long faultTime;		//���ϲ���ʱ��
	unsigned long lastInformCSTime;	//�ϴ�֪ͨ����������ù��ϵ�ʱ��
	unsigned long isClear;			//�¹��ϣ������Ѿ��ָ��Ĺ���
}FAULT_INFO;

#define  FAULT_IS_CLEAR  1
#define  FAULT_IS_NOT_CLEAR  0

#ifdef __RELEASE__
	#define	 VIDEO_LOSS_INTERVAL_TIME	10 * 60		   //��Ƶ��ʧ���ϱ����
#else
	#define	 VIDEO_LOSS_INTERVAL_TIME	60   //��Ƶ��ʧ���ϱ����
#endif
typedef enum _fault_type_
{
	FAULT_TYPE_DEVICE_ALARM			= 1,	//�豸����
	FAULT_TYPE_CAPABILITY_ALRAM		= 2,	//���ܸ澯
	FAULT_TYPE_DEAL_WITH_ERROR		= 3,	//�������
	FAULT_TYPE_COMMUNICATE_ALARM	= 4,	//ͨ�Ÿ澯
	FAULT_TYPE_ENTIRONMENT_ALRAM	= 5,	//�����澯
}FAULT_TYPE;

#define FAULT_ID_VIDEO_LOSS			4		//��Ƶ��ʧ
#define FAULT_ID_DISK_FULL			502		//���̿ռ���


typedef struct _hx_config_info_
{
	unsigned long itemID;
	unsigned long itemType;
	unsigned long channel;
	char *pData;						//��������ʱ��Ŵ�HXHT�����豸������������Ϣ, ��ȡ����ʱ���������
	unsigned long dataLen;				//��������ʱ��pData�ĳ��ȣ���ȡ����ʱ������
	unsigned long dataPosInDvrConfig;	//��ȡ�������������ڸ��������е�ƫ��
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

