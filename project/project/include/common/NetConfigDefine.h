/******************************************************************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : Ԭʯά
** Date         : 2008-07-28
** Name         : ConfigDefine.h
** Version      : 1.0
** Description  :
** Modify Record:
**************************************************************************************************************/
#ifndef _NET_CONFIG_DEFINE_H_
#define _NET_CONFIG_DEFINE_H_
#include "Typedef.h"
//һ������²��ɸı��ֵ�������Ƿ�Ҫ���Ĵ�ֵ��Ҫ���ۺ�ȷ��
const unsigned long		NCFG_VERSION					=	0x0003;

#if defined(__ENVIRONMENT_WIN32__) || defined(__TDFH__) || defined(__TDNXP__) || defined(__CHIP3531__) || defined(__CHIP3535__) || defined(__CHIP3521__) || defined(__CHIP3520A__) || defined(__CHIPTI__) || defined(__CHIP3520D__) || defined(__CHIPGM__) 
#pragma pack(4)
#endif

typedef struct _ncfg_item_head
{
	unsigned short	itemID;		//ID
	unsigned short	num;		//Ԫ����Ŀ��������ٸ�ͨ����ÿ��ͨ����Ӧһ��Ԫ��
	unsigned short	len;		//�����ܳ���
	unsigned short	subLen;		//ÿ��Ԫ�صĳ���
}NCFG_ITEM_HEAD;


typedef struct _ncfg_block_head
{
	unsigned long	biSize;		//���ṹ�峤��
	unsigned long	ItemNum;	//Ԫ����Ŀ
	unsigned long	netcfgver;	//��ǰ����İ汾�ţ��豸�˹̶�����NCFG_VERSION���ͻ��˸�����Ҫ��̬�ж�
}NCFG_BLOCK_HEAD;

typedef struct _ncfg_item_head_ex
{
	NCFG_ITEM_HEAD	item_head;
	unsigned long	start_pos;		//�ļ���ƫ��
}NCFG_ITEM_HEAD_EX;		

typedef struct _ncfg_block_info
{
	NCFG_ITEM_HEAD_EX itemHead;
	unsigned long	 off;				//������ƫ��
}NCFG_BLOCK_INFO;

typedef enum cfg_item_id
{
	//�˷�ΧID�洢ϵͳ�������õ�����
	NCFG_ITEM_SYSTEM_BASE=0x0100,
	NCFG_ITEM_MANUAL_REC,				//�ֶ�¼��ULONGLONG
	NCFG_ITEM_DEVICE_ID,				//�豸ID��unsigned long
	NCFG_ITEM_DEVICE_NAME,				//�豸���ƣ�NCFG_INFO_SHORT_NAME
	NCFG_ITEM_VIDEO_FORMAT,				//��Ƶ��ʽ��unsigned long ,��Ƶ��ʽ
	NCFG_ITEM_AUTHORIZATION,			//�Ƿ�Ȩ�޼��, unsigned long
	NCFG_ITEM_LIVE_AUDIO_ENABLE,		//�ֳ���Ƶ��unsigned long 
	NCFG_ITEM_LIVE_AUDIO_CHNN,			//�ֳ���Ƶͨ��,unsigned long--ͨ����Ŀ��ʾ�Զ�
	NCFG_ITEM_LIVE_AUDIO_VOLUME,		//ϵͳ����
	NCFG_ITEM_AUDIO_BROADCAST_ENABLE,	//�Ƿ����������㲥, unsigned long
	NCFG_ITEM_DATE_FORMAT,				//���ڸ�ʽ��unsigned long
	NCFG_ITEM_HOUR_FORMAT,				//ʱ���ʽ��12H����24H��unsigned long
	NCFG_ITEM_LANGUAGE,					//���ԣ�unsigned long
	NCFG_ITEM_TIME_ZONE,				//ʱ����long : g_zoneName �±�
	NCFG_ITEM_TIME_SYNC,				//�Ƿ�����ʱ��ͬ����unsigned long
	NCFG_ITEM_TIME_SERVER,				//����ʱ��ͬ����������ַ��NCFG_INFO_LONG_NAME
	NCFG_ITEM_LIVE_TIME_INFO,			//DVR�ֳ�ʱ���Ƿ���ʾ, OSD_INFO
	NCFG_ITEM_SCRENN_SAVE_ENABLE,		//�Ƿ�������, unsigned long
	NCFG_ITEM_SCRENN_SAVE_TIME,			//����ʱ��, unsigned long
	NCFG_ITEM_REC_RECYCLE,				//�Ƿ񸲸�¼��unsigned long
	NCFG_ITEM_VGA_RESOLUTION,			//��ʾ���ֱ��ʣ�unsigned long���ο�dvrdvsdef.h�е�VGA_RESOLUTIONö��
	NCFG_ITEM_VGA_REFRESH,			    //��ʾ����Ļˢ����,VGA_SUPPORT_REFRSHHZ[n]
	NCFG_ITEM_USER,						//�û���Ϣ��NCFG_INFO_USER [MAX_USER_NUM]
	NCFG_ITEM_DAYLIGHT_INFO,			//����ʱ��Ϣ��DAYLIGHT_INFO
	NCFG_ITEM_MAX_CLIENT_NUM,			//��������û���Ŀ�����ܳ�����Ʒ��������ֵ��unsigned long
	NCFG_ITEM_TRANSPARENCY,				//�˵�͸���ȡ�unsigned long
	NCFG_ITEM_SHOW_WIZZARD,				//�����Ƿ���ʾ�򵼡�unsigned long
	NCFG_ITEM_DISK_ALARM_FREE,			//���̱����ռ䣨���ǵ���������ռ䱨�����������԰��������ʾ��������,unsigned long
	NCFG_ITEM_NTP_PORT,					//NTP�������˿ڣ�unsigned long
	NCFG_ITEM_NTP_PERIOD,				//Уʱ�����unsigned long
	NCFG_ITEM_LIVE_REC_STATUS,			//�ֳ��Ƿ���ʾ¼��״̬��unsigned long
	NCFG_ITEM_DODWELL,					//�Ƿ���̨,unsigned long
	NCFG_ITEM_TOOLBAR_POS,				//������λ��,NCFG_INFO_TOOLBAR_POS
	NCFG_ITEM_CHANN_VISIBLE,			//δ��¼ ͨ���Ƿ�ɼ� unsigned long(1�ǲ��ɼ���0�ɼ�)
	
	NCFG_ITEM_SEARCH_GROUP_ENABLE,      //ͨ�����������
	NCFG_ITEM_SEARCH_GROUP_ITEM,		//ͨ������(����ҳ����) NCFG_INFO_GROUP_ITEM[MAX_GROUP_ITEM_COUNT] ---added by wn --2011.7.4
	NCFG_ITEM_CVBS_VO_ADJUST,			//����CVBS������еļ���������ڣ�0�� 0��λ��ʱ����ֻ��汻�ڵ��������
	NCFG_ITEM_CVBS_TO_SPOT,				//CVBS��ΪSPOT�� NCFG_CVBS_INFO
	NCFG_ITEM_ENCODE_960H,				//����960H�ֱ���
	NCFG_ITEM_SYSTEM_END,
	
	//ͨ������
	NCFG_ITEM_CHANNEL_BASE=0x0200,
	NCFG_ITEM_CAMERA_NAME,				//��������ƣ�NCFG_INFO_SHORT_NAME[VIDEO_INPUT_NUM]
	NCFG_ITEM_SHOW_CAM_NAME,			//DVR�ֳ��Ƿ���ʾ��������ƣ� unsigned long [VIDEO_INPUT_NUM]
	NCFG_ITEM_PTZ_SERIAL,               //ÿ��ͨ������̨��Э�顢��ַ�������� PTZ_SERIAL_INFO[VIDEO_INPUT_NUM]
	NCFG_ITEM_VIEW_SPLIT_MODE,			//��ʾ�ֳ�Ԥ��ʱ�ķָ�ģʽ��������ֻ��ѡ��һ�֡�unsigned long
	NCFG_ITEM_VIEW_DWELL_TIME,			//������̨��ʱ���������ʾ�������顣unsigned long[32]
	NCFG_ITEM_VIEW_SPLIT_1X1,			//1X1�ָ���Ϣ:unsigned long[32*1*1+2] [������][��Ч����][��һ��...][...]
	NCFG_ITEM_VIEW_SPLIT_2X2,			//unsigned long [8*2*2+2] ������>��������,Ϊ���ݸ��ַָ�Ԥ���ռ�
	NCFG_ITEM_VIEW_SPLIT_2X3,			//unsinged long [11*2*3+2] 
	NCFG_ITEM_VIEW_SPLIT_3X3,			//unsigned long [6*3*3+2]
	NCFG_ITEM_VIEW_SPLIT_4X4,			//unsigned long [4*4*4+2]
	NCFG_ITEM_VIEW_SPLIT_5X5,			//unsigned long [4*5*5+2]
	NCFG_ITEM_VIEW_SPLIT_6X6,			//unsigned long [3*6*6+2]
	NCFG_ITEM_SPOT_SPLIT_MODE,			//��ʾSPOTԤ��ʱ�ķָ�ģʽ��Ŀǰֻ�ܶ�Ϊһ�ָunsigned long
	NCFG_ITEM_SPOT_DWELL_TIME,			//SPOT����̨ʱ�䣬���ʾ����̨��unsigned long
	NCFG_ITEM_SPOT_SPLIT_1X1,			//unsigned long[32*1*1+2]
	NCFG_ITEM_VIDEO_COLOR,				//��Ƶɫ�ʣ�unsigned long[VIDEO_INPUT_NUM]
	NCFG_ITEM_TIME_STAMP_INFO,			//����ʱ�����Ϣ, OSD_INFO[VIDEO_INPUT_NUM]
	NCFG_ITEM_CAM_NAME_STAMP_INFO,		//����ͨ��������Ϣ, OSD_INFO[VIDEO_INPUT_NUM]
	NCFG_ITEM_CAM_COVER_INFO,			//ͼ���ڵ���Ϣ, CAMERA_COVER_INFO[VIDEO_INPUT_NUM]
	NCFG_ITEM_VIEW_SPLIT_4X6,			//unsigned long [4*4*6+2]  �ָ�ģʽ
	NCFG_ITEM_PTZ_CURISE_TYPE,			//ͨ��Ѳ������������unsigned long [VIDEO_INPUT_NUM]
	NCFG_ITEM_CAM_ROI_INFO,             //ROI    VIDEO_CONFIG_ROI_INFO[NET_VIDEO_INPUT_NUM]
	NCFG_ITEM_CAM_MIRROR,				//ͼ���� unsigned char
	NCFG_ITEM_CAM_FILP,				    //ͼ��ת unsigned char
	NCFG_ITEM_CAM_ROTATO,               // ͼ����ת(����ģʽ) IMAGE_ROTATO
	NCFG_ITEM_CAM_LENS_DISTORTION,		// ��ͷ���� LENS_DISTORTION_CONFIG
	NCFG_ITEM_CAM_TIME_FOCUS_MODE,			//�Խ�ģʽ unsigned char
	NCFG_ITEM_CAM_IRCHANGE_FOCUS,			//��ҹ�л�ʱ�Խ� unsigned char
	NCFG_ITEM_CAM_INTERVAL_TIME,			//�Խ����ʱ�� unsigned long
	NCFG_ITEM_CHANNEL_END,

	NCFG_ITEM_CHANNEL_REC_BASE=0x0300,   //ͨ��¼������
	NCFG_ITEM_REC_ENABLE,				//¼���ܿ��أ�unsigned long [VIDEO_INPUT_NUM]
	NCFG_ITEM_PRE_ALARM_REC_TIME,		//Ԥ¼��ʱ�䣨�룩��unsigned long[VIDEO_INPUT_NUM]
	NCFG_ITEM_POST_ALARM_REC_TIME,		//����¼�񱣳�ʱ�䣨�룩��unsigned long[VIDEO_INPUT_NUM]
	NCFG_ITEM_REC_HOLD_TIME,			//¼�����ݱ����ڣ��룩��unsigned long[VIDEO_INPUT_NUM]
	NCFG_ITEM_REC_AUDIO,				//��ͨ���Ƿ�¼��Ƶ��unsigned long [VIDEO_INPUT_NUM]
	NCFG_ITEM_REC_REDUNDANT,            //��ͨ���Ƿ�����¼��unsigned long [VIDEO_INPUT_NUM]
	NCFG_ITEM_CHANNEL_REC_END,

	//�������
	NCFG_ITEM_ENCODE_BASE=0x0400,
	NCFG_ITEM_ENCODE_MAJOR,				//������(�ֶ�����ʱ)���������ENCODE_INFO [VIDEO_INPUT_NUM]
	NCFG_ITEM_ENCODE_MINOR,				//���������������ENCODE_INFO [VIDEO_INPUT_NUM]
	NCFG_ITEM_ENCODE_PHONE,				//�ֻ��������������ENCODE_INFO [VIDEO_INPUT_NUM]
	NCFG_ITEM_ENCODE_MAJOR_EVENT,		//������(�¼�)���������ENCODE_INFO [VIDEO_INPUT_NUM]
	NCFG_ITEM_JPEG_PERIOD,				//JPEGץͼ�����unsigned long [VIDEO_INPUT_NUM]
	NCFG_ITEM_JPEG_SIZE,                //JPEGץͼ�ֱ��ʣ�unsigned long [VIDEO_INPUT_NUM]
	NCFG_ITEM_JPEG_QUALITY,             //JPEGץͼ���ʣ�unsigned long [VIDEO_INPUT_NUM]
	NCFG_ITEM_JPEG_SNAP_NUM,            //JPEGץͼ������unsigned long [VIDEO_INPUT_NUM]
	NCFG_ITEM_ENCODE_MAJOR_SPEC,        //¼��ķֱ��ʵ��������ã�������2432�ϣ�1,5,9,13,17,21,25,29�������ó�D1��CIF��������ͨ��ֻ����CIF
	NCFG_ITEM_ENCODE_MAJOR_EX,			//���������豸�������������������ENCODE_INFO_EX
	NCFG_ITEM_IPC_SUPPORT_ENCODE,		//NCFG_IPC_SUPPORT_ENCODE
	NCFG_ITEM_ENCODE_MINOR_EX,
	NCFG_ITEM_BITRANGE_FOR_VIDEOSIZE,		//������ÿ���ֱ��ʵ�������Χ
	NCFG_ITEM_MAJOR_CHANNEL_RECORD_PARAM_EX,	//�����dvrÿ��ͨ����¼���������
	NCFG_ITEM_ENCODE_END,

	//�ƻ�,���¼ƻ�һ��Ҫ����ϸ�����紫��������¼��ƻ��ʹ��������ƻ���
	NCFG_ITEM_SCHEDULE_BASE=0x0500,
	NCFG_ITEM_REC_SCH_SCHEDULE_WEEK,	//�ų�¼���ܼƻ��� WEEK_SCHEDULE[VIDEO_INPUT_NUM]
	NCFG_ITEM_REC_SCH_MOTION_WEEK,		//�ƶ����¼���ܼƻ��� WEEK_SCHEDULE[VIDEO_INPUT_NUM]
	NCFG_ITEM_REC_SCH_SENSOR_WEEK,		//����������¼���ܼƻ���WEEK_SCHEDULE[VIDEO_INPUT_NUM]
	NCFG_ITEM_SENSOR_IN_SCH_WEEK,		//����������ܼƻ��� WEEK_SCHEDULE[SENSOR_INPUT_NUM]
	NCFG_ITEM_ALARMOUT_SCH_WEEK,		//��������ܼƻ�WEEK_SCHEDULE[ALARMOUT_NUM]
	NCFG_ITEM_MOTION_SCH_WEEK,			//�ƶ�����ܼƻ�WEEK_SCHEDULE[VIDEO_INPUT_NUM]
	NCFG_ITEM_NET_ALARM_SCH_WEEK,		//���籨���ܼƻ�WEEK_SCHEDULE[VIDEO_INPUT_NUM]
	NCFG_ITEM_SCHEDULE_END,

	//���繦��
	NCFG_ITEM_NET_BASE=0x0600,
	NCFG_ITEM_UPNP_ENABLE,				//�Ƿ�����UPNP����, unsigned long
	NCFG_ITEM_UPNP_FRIENDLY_NAME,		//UPNP�豸��ʾ������, NCFG_INFO_LONG_NAME
	NCFG_ITEM_HTTP_PORT,				//HTTP �˿�, unsigned long
	NCFG_ITEM_SERVER_PORT,				//�������˿ڣ� unsigned long
	NCFG_ITEM_NETWORK_ADDR_INFO,		//�����ַ�����Ϣ��NETWORK_ADDR_INFO
	NCFG_ITEM_DNS_1,					//unsigned long
	NCFG_ITEM_DNS_2,					//unsigned long
	NCFG_ITEM_DDNS_INFO,				//DDNS��Ϣ��DDNS_INFO
	NCFG_ITEM_PPPOE_INFO,				//PPPoE�������Ϣ��PPPOE_INFO
	NCFG_ITEM_EMAIL_SEND_INFO,			//�ʼ����ܷ��ͷ���Ϣ,MAIL_SEND_INFO
	NCFG_ITEM_EMAIL_RECV_INFO,			//�ʼ����ܽ��շ���Ϣ,MAIL_RECV_INFO[MAX_MAIL_RECV_NUM]
	NCFG_ITEM_EMAIL_PIC_ACC,			//�ʼ�����ͼƬ���������ʾ�����ͣ�����Ϊ����ͼƬ��Ŀ
	NCFG_ITEM_MULTI_NET_ADDR,			//����ಥ��ַ
	NCFG_ITEM_DDNS_UPDATE_CYCLE,		//ddns���������˸��µ����ڣ���λΪ��
	NCFG_ITEM_BAN_ALLOW_TYPE,			//�ڰ��������ͣ�unsigned long
	NCFG_ITEM_BAN_ALLOW_LIST,			//�ڰ������б�, IP_SECTION[MAX_BAN_ALLOW_LIST_NUM]
	NCFG_ITEM_HRSST_SERVER_IP,          //����ͨ������IP��ַ
	NCFG_ITEM_HRSST_SERVER_PORT,        //����ͨ�������˿�
	NCFG_ITEM_HXHT_NET_PARAM,			//���Ż�ͨ����������ϢHXHT_NET_PARAM_INFO
	NCFG_ITEM_NET_WIRELESS,				//�������� MDVR_WIRELESS_CFG
	NCFG_ITEM_AUTO_REPORT,				//�Զ��ϱ�  NET_AUTO_REPORT
	NCFG_ITEM_MINT_DDNS_SERVER_NAME,	//mint ddns ���ͷ�����������
	NCFG_ITEM_USE_3G,				    //ҳ���Ǳ���ipҳ�滹������3Gҳ��
	NCFG_ITEM_USE_IPV4_IPV6,		//IPV4��IPV6�Ƿ�����, USE_IPV4_IPV6
	NCFG_ITEM_NETWORK_ADDR_INFO_IPV6,		//IPV6������Ϣ, NETWORK_ADDR_INFO_IPv6
	NCFG_ITEM_DNS_1_IPV6,					//unsigned char[40]
	NCFG_ITEM_DNS_2_IPV6,					//unsigned cahr[40]
	NCFG_ITEM_NET_YZ_PARAM,                 //YUANZHENG_PARAM;
	NCFG_ITEM_NET_YZ_SNAP_PARAM,            //YUANZHENG_SNAP_PARAM;Զ��ץͼ��������
	NCFG_ITEM_NET_SHANGHAI_DAILY_IMAGE,					//�Ϻ�ƽ̨�ճ�ͼ�����ã�SNAP_INFOEX[100];
	NCFG_ITEM_NET_SHANGHAI_PRE_ALARM_SNAP_TIME,			//�Ϻ�ƽ̨Ԥץͼʱ�䣨�룩��unsigned long[VIDEO_INPUT_NUM]
	NCFG_ITEM_NET_SHANGHAI_POST_ALARM_SNAP_TIME,		//�Ϻ�ƽ̨����ץͼ����ʱ�䣨�룩��unsigned long[VIDEO_INPUT_NUM]
	NCFG_ITEM_NET_SHANGHAI_MOTIONUPLOAD_ENABLE,			//�Ϻ�ƽ̨�ƶ�����ϴ�����	
	NCFG_ITEM_NET_SHANGHAI_PTENABLE,					//�Ϻ�ƽ̨��������
	NCFG_ITEM_NET_SHANGHAI_AGENTID,						//�Ϻ�ƽ̨����ʱ��Ľ���ɣ�
	NCFG_ITEM_NET_SHANGHAI_IP,							//�Ϻ�ƽ̨����ʱ��Ľ���IP
	NCFG_ITEM_NET_SHANGHAI_PORT,						//�Ϻ�ƽ̨����ʱ��Ľ���˿�
	NCFG_ITEM_NET_FTP,									//FTP������Ϣ
	NCFG_ITEM_NAT_SERVER,								//NAT������������Ϣ
	NCFG_ITEM_PUSH_SERVER,								//���ͷ�����������Ϣ
	NCFG_ITEM_SWITCH_INFO,								//POE-NVR��switch��Ϣ,NET_SWITCH_INFO
	NCFG_ITEM_USE_PUSH,									//�Ƿ�ʹ�����͹���
	NCFG_ITEM_NET_END=0x06ff,

	//������������
	NCFG_ITEM_ALARM_TRIGER_BASE=0x0700,
	NCFG_ITEM_BUZZER_ENABLE,			//���������أ�unsigned long
	NCFG_ITEM_BUZZER_HOLD_TIME,			//����������ʱ�䣬unsigned long,
	NCFG_ITEM_ALARM_OUT_NAME,			//�̵���������ƣ�NCFG_INFO_SHORT_NAME[SENSOR_OUTPUT_NUM]
	NCFG_ITEM_ALARM_OUT_HOLD_TIME,		//�̵����������ʱ�䣬unsigned long [SENSOR_OUTPUT_NUM]
	NCFG_ITEM_ALARM_OUT_TYPE,			//�̵�������豸����
	NCFG_ITEM_ALARM_TRIGER_END,

	NCFG_ITEM_ALARM_TRIGER_SENSOR_BASE=0x0800,
	NCFG_ITEM_SENSOR_IN_ENABLE,			//���������أ�unsigned long [SENSOR_INPUT_NUM]
	NCFG_ITEM_SENSOR_IN_TYPE,			//�������豸���ͣ�unsigned long [SENSOR_INPUT_NUM]
	NCFG_ITEM_SENSOR_IN_NAME,			//���������ƣ�NCFG_INFO_SHORT_NAME [SENSOR_INPUT_NUM]
	NCFG_ITEM_SENSOR_IN_HOLD_TIME,		//����������ʱ��, unsigned long [SENSOR_INPUT_NUM]
	NCFG_ITEM_SENSOR_IN_BUZZ,           //����������������, unsigned long [SENSOR_INPUT_NUM]
	NCFG_ITEM_SENSOR_IN_TO_ALARM_OUT,	//�������������������ULONGLONG [SENSOR_INPUT_NUM];
	NCFG_ITEM_SENSOR_IN_TO_RECORD,		//����������¼��ULONGLONG [SENSOR_INPUT_NUM];
	NCFG_ITEM_SENSOR_TO_BIG_VIEW,		//�������������棬ULONGLONG[VIDEO_INPUT_NUM]
	NCFG_ITEM_SENSOR_IN_TO_EMAIL_ENABLE,//�������ܹ�����EMAIL��unsigned long
	NCFG_ITEM_SENSOR_TO_EMAIL_ADDR,		//����������MAIL��ַ�б�unsigned long
	NCFG_ITEM_SENSOR_IN_TO_FTP_ENABLE,	//��������������FTP��unsigned long
	NCFG_ITEM_SENSOR_IN_TO_FTP_ADDR,	//����������FTP��ַ�б���MAIL����ʽһ��unsigned long[n] 
	NCFG_ITEM_SENSOR_IN_TO_PTZ,			//������������̨������TO_PTZ [SENSOR_INPUT_NUM][VIDEO_INPUT_NUM]
	NCFG_ITEM_SENSOR_IN_TO_SNAP,		//����������ץͼ��ͨ��ULONGLONG [SENSOR_INPUT_NUM]
	NCFG_ITEM_SENSOR_TO_FTP_SNAP,		//����������FTPץͼ��ULONGLONG[VIDEO_INPUT_NUM]
	NCFG_ITEM_SENSOR_TO_EMAIL_SNAP,		//����������EMAILץͼ��ULONGLONG[VIDEO_INPUT_NUM]
	NCFG_ITEM_ALARM_TRIGER_SENSOREND,

	NCFG_ITEM_ALARM_TRIGER_MOTION_BASE=0x0900,	
	NCFG_ITEM_MOTION_ENABLE,			//�ƶ���⿪�أ�unsigned long [VIDEO_INPUT_NUM]
	NCFG_ITEM_MOTION_AREA_INFO,         //�������Ϣ��MOTION_DETECT_INFO[VIDEO_INPUT_NUM]
	NCFG_ITEM_MOTION_HOLD_TIME,			//�ƶ���Ᵽ��ʱ��,unsigned long [VIDEO_INPUT_NUM]
	NCFG_ITEM_MOTION_TO_ALARM_OUT,		//�ƶ���ⴥ�����������ULONGLONG [VIDEO_INPUT_NUM]
	NCFG_ITEM_MOTION_TO_RECORD,			//�ƶ���ⴥ��¼��ULONGLONG [VIDEO_INPUT_NUM]
	NCFG_ITEM_MOTION_TO_BIG_VIEW,		//�ƶ���ⴥ�����棬ULONGLONG[VIDEO_INPUT_NUM]
	NCFG_ITEM_MOTION_TO_EMAIL_ENABLE,	//�ƶ�����ܹ�����EMAIL��unsigned long
	NCFG_ITEM_MOTION_TO_EMAIL_ADDR,		//�ƶ���ⴥ��MAIL��ַ�б�unsigned long
	NCFG_ITEM_MOTION_TO_PTZ,			//�ƶ���ⴥ����̨��TO_PTZ [VIDEO_INPUT_NUM][VIDEO_INPUT_NUM]
	NCFG_ITEM_MOTION_TO_FTP_ENABLE,		//�ƶ���ⴥ��FTP��unsigned long
	NCFG_ITEM_MOTION_TO_FTP_ADDR,		//�ƶ���ⴥ��FTP��ַ�б���MAIL����ʽһ��unsigned long[n] 
	NCFG_ITEM_MOTION_TO_BUZZ,           //�ƶ���ⴥ��BUZZ unsigned long[VIDEO_INPUT_NUM]
	NCFG_ITEM_MOTION_TO_SNAP,           //�ƶ���ⴥ��ץ�� ULONGLONG[VIDEO_INPUT_NUM]
	NCFG_ITEM_MOTION_TO_FTP_SNAP,		//�ƶ���ⴥ��FTPץͼ��ULONGLONG[VIDEO_INPUT_NUM]
	NCFG_ITEM_MOTION_TO_EMAIL_SNAP,		//�ƶ���ⴥ��EMAILץͼ��ULONGLONG[VIDEO_INPUT_NUM]
	NCFG_ITEM_HXHT_MOTION_AREA_INFO,	//���Ż�ͨ�ƶ��������ã�HXHT_MOTION_AREA_INFO[VIDEO_INPUT_NUM]
	NCFG_ITEM_ALARM_TRIGER_MOTIONEND,
	
	NCFG_ITEM_ALARM_TRIGER_VLOSS_BASE=0x0a00,
	NCFG_ITEM_VL_TO_ALARM_OUT,			//��Ƶ��ʧ�������������ULONGLONG [VIDEO_INPUT_NUM]
	NCFG_ITEM_VL_TO_RECORD,				//��Ƶ��ʧ����¼��ULONGLONG [VIDEO_INPUT_NUM]
	NCFG_ITEM_VL_TO_BIG_VIEW,			//��Ƶ��ʧ�������棬ULONGLONG[VIDEO_INPUT_NUM]
	NCFG_ITEM_VL_TO_EMAIL_ENABLE,		//��Ƶ��ʧ�ܹ�����MAIL unsigned long
	NCFG_ITEM_VL_TO_EMAIL_ADDR,			//��Ƶ��ʧ����MAIL��ַ�б�unsigned long
	NCFG_ITEM_VL_TO_PTZ,				//��Ƶ��ʧ������̨��TO_PTZ [VIDEO_INPUT_NUM][VIDEO_INPUT_NUM]
	NCFG_ITEM_VL_TO_FTP_ENABLE,			//��Ƶ��ʧ����FTP��unsigned long
	NCFG_ITEM_VL_TO_FTP_ADDR,			//��Ƶ��ʧ����FTP��ַ�б���MAIL����ʽһ��unsigned long[n] 
	NCFG_ITEM_VL_TO_BUZZ,               //��Ƶ��ʧ����BUZZ unsigned long[VIDEO_INPUT_NUM]
	NCFG_ITEM_VL_TO_SNAP,               //��Ƶ��ʧ����ץ�� ULONGLONG[VIDEO_INPUT_NUM]
	NCFG_ITEM_VL_TO_FTP_SNAP,		    //��Ƶ��ʧ����FTPץͼ��ULONGLONG[VIDEO_INPUT_NUM]
	NCFG_ITEM_VL_TO_EMAIL_SNAP,		    //��Ƶ��ʧ����EMAILץͼ��ULONGLONG[VIDEO_INPUT_NUM]
	NCFG_ITEM_ALARM_TRIGER_VLOSSEND,

	NCFG_ITEM_ALARM_TRIGER_SHELTER_BASE=0x0b00,
	NCFG_ITEM_SHELTER_TO_ALARM_OUT,		//�ڵ��������������ULONGLONG [VIDEO_INPUT_NUM]
	NCFG_ITEM_SHELTER_TO_RECORD,		//�ڵ�����¼��ULONGLONG [VIDEO_INPUT_NUM]
	NCFG_ITEM_SHELTER_TO_BIG_VIEW,		//�ڵ��������棬ULONGLONG[VIDEO_INPUT_NUM]
	NCFG_ITEM_SHELTER_TO_EMAIL_ENABLE,	//�ڵ��ܹ�����EMAIL��unsigned long
	NCFG_ITEM_SHELTER_TO_EMAIL_ADDR,	//�ڵ�����EMAIL��ַ��,unsigned long
	NCFG_ITEM_SHELTER_TO_FTP_ENABLE,	//�ڵ�����FTP��unsigned long
	NCFG_ITEM_SHELTER_TO_BUZZ,          //�ڵ�����BUZZ unsigned long[VIDEO_INPUT_NUM]
	NCFG_ITEM_SHELTER_TO_PTZ,			//�ڵ�������̨��TO_PTZ [VIDEO_INPUT_NUM][VIDEO_INPUT_NUM]
	NCFG_ITEM_SHELTER_TO_SNAP,          //�ڵ�����ץ�� ULONGLONG[VIDEO_INPUT_NUM]
	NCFG_ITEM_SHELTER_TO_FTP_SNAP,		//�ڵ�����FTPץͼ��ULONGLONG[VIDEO_INPUT_NUM]
	NCFG_ITEM_SHELTER_TO_EMAIL_SNAP,	//�ڵ�����EMAILץͼ��ULONGLONG[VIDEO_INPUT_NUM]
	NCFG_ITEM_ALARM_TRIGER_SHELTEREND,
	
	//
	NCFG_ITEM_SCH_HOLIDAY_BASE = 0x0c00,
	NCFG_ITEM_REC_SCH_SCHEDULE_HOLIDAY,	//�ų�¼����ռƻ��� HOLIDAY_SCHEDULE [MAX_HOLIDAY_NUM][VIDEO_INPUT_NUM]
	NCFG_ITEM_REC_SCH_MOTION_HOLIDAY,	//�ƶ����¼����ռƻ�, HOLIDAY_SCHEDULE [MAX_HOLIDAY_NUM][VIDEO_INPUT_NUM]
	NCFG_ITEM_REC_SCH_SENSOR_HOLIDAY,	//����������¼����ռƻ�, HOLIDAY_SCHEDULE [MAX_HOLIDAY_NUM][VIDEO_INPUT_NUM]
	NCFG_ITEM_SENSOR_IN_SCH_HOLIDAY,	//�����������ռƻ�HOLIDAY_SCHEDULE [MAX_HOLIDAY_NUM][SENSOR_INPUT_NUM]
	NCFG_ITEM_ALARMOUT_SCH_HOLIDAY,		//����������ռƻ�HOLIDAY_SCHEDULE [MAX_HOLIDAY_NUM][ALARMOUT_NUM]
	NCFG_ITEM_MOTION_SCH_HOLIDAY,		//�ƶ������ռƻ�HOLIDAY_SCHEDULE [MAX_HOLIDAY_NUM][VIDEO_INPUT_NUM]
	NCFG_ITEM_NET_ALARM_SCH_HOLIDAY,
	NCFG_ITEM_SCH_HOLIDAY_END,

	/////�������//////////////////////////////////////////////////////////////
	NCFG_ITEM_MOBILE_BASE = 0x0d00,
	NCFG_ITEM_COMPANY_NAME,					//���ӹ�˾����, NCFG_INFO_LONG_NAME
	NCFG_ITEM_DRIVER_NAME,					//˾������, NCFG_INFO_SHORT_NAME
	NCFG_ITEM_CAR_NUMBER,					//��������, NCFG_INFO_SHORT_NAME
	NCFG_ITEM_SHUT_DOWN_DELAY,				//�ػ���ʱ, unsigned long
	NCFG_ITEM_NET_SERVER,					//�����������MDVR_NETSERVER_CFG
	NCFG_ITEM_MOBILE_END,

	NCFG_ITEM_SPEED_BASE = 0x0d40,
	NCFG_ITEM_SPEED_SOURCE,					//�ٶ���Դ��unsigned long
	NCFG_ITEM_SPEED_UNIT,					//�ٶȵ�λ��unsigned long
	NCFG_ITEM_SPEED_END,

	NCFG_ITEM_HIGH_SPEED_BASE = 0xd50,
	NCFG_ITEM_HIGH_SPEED_ENABLE,			//������⿪�أ�unsigned long
	NCFG_ITEM_HIGH_SPEED_VALUE,				//��������ֵ��MDVR_SPEED
	NCFG_ITEM_HIGH_SPEED_HOLD_TIME,			//���ٴ�����������ʱ��, unsigned long 
	NCFG_ITEM_HIGH_SPEED_BUZZ,				//���ٴ���������, unsigned long
	NCFG_ITEM_HIGH_SPEED_TO_ALARM_OUT,		//���ٴ������������ULONGLONG
	NCFG_ITEM_HIGH_SPEED_TO_RECORD,			//���ٴ���¼��ULONGLONG 
	NCFG_ITEM_HIGH_SPEED_BIG_VIEW,			//���ٴ������棬ULONGLONG
	NCFG_ITEM_HIGH_SPEED_TO_EMAIL_ENABLE,	//�����ܹ�����EMAIL��unsigned long
	NCFG_ITEM_HIGH_SPEED_TO_EMAIL_ADDR,		//���ٴ���MAIL��ַ�б�unsigned long
	NCFG_ITEM_HIGH_SPEED_TO_FTP_ENABLE,		//���ٴ�������FTP��unsigned long
	NCFG_ITEM_HIGH_SPEED_TO_FTP_ADDR,		//���ٴ���FTP��ַ�б���MAIL����ʽһ��unsigned long[n] 
	NCFG_ITEM_HIGH_SPEED_TO_PTZ,			//���ٴ�����̨������TO_PTZ[VIDEO_INPUT_NUM]
	NCFG_ITEM_HIGH_SPEED_TO_SNAP,			//���ٴ���ץͼ��ͨ��ULONGLONG
	NCFG_ITEM_HIGH_SPEED_TO_FTP_SNAP,		//���ٴ���FTPץͼ��ULONGLONG
	NCFG_ITEM_HIGH_SPEED_TO_EMAIL_SNAP,		//���ٴ���EMAILץͼ��ULONGLONG
	NCFG_ITEM_HIGH_SPEED_END,

	NCFG_ITEM_LOW_SPEED_BASE = 0xd70,
	NCFG_ITEM_LOW_SPEED_ENABLE,				//������⿪�أ� unsigned long
	NCFG_ITEM_LOW_SPEED_VALUE,				//��������ֵ��MDVR_SPEED
	NCFG_ITEM_LOW_SPEED_HOLD_TIME,			//�͸��ٴ�����������ʱ��, unsigned long [SENSOR_INPUT_NUM]	
	NCFG_ITEM_LOW_SPEED_BUZZ,				//���ٴ���������, unsigned long
	NCFG_ITEM_LOW_SPEED_TO_ALARM_OUT,		//���ٴ������������ULONGLONG
	NCFG_ITEM_LOW_SPEED_TO_RECORD,			//���ٴ���¼��ULONGLONG
	NCFG_ITEM_LOW_SPEED_BIG_VIEW,			//���ٴ������棬ULONGLONG
	NCFG_ITEM_LOW_SPEED_TO_EMAIL_ENABLE,	//�����ܹ�����EMAIL��unsigned long
	NCFG_ITEM_LOW_SPEED_TO_EMAIL_ADDR,		//���ٴ���MAIL��ַ�б�unsigned long
	NCFG_ITEM_LOW_SPEED_TO_FTP_ENABLE,		//���ٴ�������FTP��unsigned long
	NCFG_ITEM_LOW_SPEED_TO_FTP_ADDR,		//���ٴ���FTP��ַ�б���MAIL����ʽһ��unsigned long[n] 
	NCFG_ITEM_LOW_SPEED_TO_PTZ,				//���ٴ�����̨������TO_PTZ[VIDEO_INPUT_NUM]
	NCFG_ITEM_LOW_SPEED_TO_SNAP,			//���ٴ���ץͼ��ͨ�� ULONGLONG
	NCFG_ITEM_LOW_SPEED_TO_FTP_SNAP,		//���ٴ���FTPץͼ��ULONGLONG
	NCFG_ITEM_LOW_SPEED_TO_EMAIL_SNAP,		//���ٴ���EMAILץͼ��ULONGLONG
	NCFG_ITEM_LOW_SPEED_END,

	NCFG_ITEM_WALLOP_BASE = 0x0d90,
	NCFG_ITEM_WALLOP_INFO_X,				//�����X��MDVR_WALLOP_STATE
	NCFG_ITEM_WALLOP_INFO_Y,				//�����Y��MDVR_WALLOP_STATE
	NCFG_ITEM_WALLOP_INFO_Z,				//�����Z��MDVR_WALLOP_STATE
	NCFG_ITEM_WALLOP_BUZZ,					//���������������, unsigned long
	NCFG_ITEM_WALLOP_TO_ALARM_OUT,			//������������������ULONGLONG
	NCFG_ITEM_WALLOP_TO_RECORD,				//���������¼��ULONGLONG
	NCFG_ITEM_WALLOP_BIG_VIEW,				//������������棬ULONGLONG
	NCFG_ITEM_WALLOP_TO_EMAIL_ENABLE,		//������ܹ�����EMAIL��unsigned long
	NCFG_ITEM_WALLOP_TO_EMAIL_ADDR,			//���������MAIL��ַ�б�unsigned long[n]
	NCFG_ITEM_WALLOP_TO_FTP_ENABLE,			//�������������FTP��unsigned long
	NCFG_ITEM_WALLOP_TO_FTP_ADDR,			//���������FTP��ַ�б���MAIL����ʽһ��unsigned long[n] 
	NCFG_ITEM_WALLOP_TO_PTZ,				//�����������̨������TO_PTZ [VIDEO_INPUT_NUM]
	NCFG_ITEM_WALLOP_TO_SNAP,				//���������ץͼ��ͨ�� ULONGLONG
	NCFG_ITEM_WALLOP_TO_FTP_SNAP,			//���������FTPץͼ��ULONGLONG
	NCFG_ITEM_WALLOP_TO_EMAIL_SNAP,			//���������EMAILץͼ��ULONGLONG
	NCFG_ITEM_WALLOP_END,

	NCFG_ITEM_WIRELESS_BASE = 0x0e00,
	NCFG_ITEM_WIRELESS_WIFI,				//WIFI���ã�WIFI_CONFIG
	NCFG_ITEM_WIRELESS_COMM,				//����ͨ�����ã�MDVR_WIRELESS_CFG
	NCFG_ITEM_WIRELESS_END,
	/////�������-end//////////////////////////////////////////////////////////////

	NCFG_ITEM_EXCEPTION_BASE = 0x0f00,
	NCFG_ITEM_DISK_FULL_BUZZER,				//�������������������� unsigned long
	NCFG_ITEM_DISK_FULL_TO_ALARM_OUT,		//����������������� ULONGLONG
	NCFG_ITEM_DISK_FULL_TO_EMAIL,			//�����������ʼ� unsigned long
	NCFG_ITEM_IP_CONFLICT_BUZZER,			//�����ַ��ͻ�������������� unsigned long
	NCFG_ITEM_IP_CONFLICT_TO_ALARM_OUT,		//�����ַ��ͻ����������� ULONGLONG
	NCFG_ITEM_IP_CONFLICT_TO_EMAIL,			//�����ַ��ͻ�����ʼ� unsigned long
	NCFG_ITEM_DISCONNECT_BUZZER,			//����Ͽ��������������� unsigned long
	NCFG_ITEM_DISCONNECT_TO_ALARM_OUT,		//����Ͽ������������ ULONGLONG
	NCFG_ITEM_DISCONNECT_TO_EMAIL,			//����Ͽ������ʼ� unsigned long
	NCFG_ITEM_DISK_WARNING_BUZZER,          //Ӳ��˥�������������� unsigned long
	NCFG_ITEM_DISK_WARNING_TO_ALARM_OUT,    //Ӳ��˥������������� ULONGLONG
	NCFG_ITEM_DISK_WARNING_TO_EMAIL,        //Ӳ��˥�������ʼ� unsigned long
	NCFG_ITEM_DISK_DISCONNECT_BUZZER,		//Ӳ�̵��ߴ���������
	NCFG_ITEM_DISK_DISCONNECT_TO_ALARM_OUT,	//Ӳ�̵��ߴ���������� ULONGLONG
	NCFG_ITEM_DISK_DISCONNECT_TO_EMAIL,		//Ӳ�̵��ߴ����ʼ� unsigned long
	NCFG_ITEM_EXCEPTION_END,

	//�����������
	NCFG_ITEM_GB28181_BASE = 0x1000,
	NCFG_ITEM_GB28181_SERVER_INFO,				//sip ���������ã���Ӧ �ṹ��GB28181_SIP_SERVER
	NCFG_ITEM_GB28181_CHANNEL_ID,				//channelID��ͨ�������ã���Ӧ�ṹ�� GB28181_CHANNEL_ID
	NCFG_ITEM_GB28181_ALARM_ID,					//alarmid���澯id�����ã���Ӧ�ṹ��GB28181_ALARM_ID
	NCFG_ITEM_GB28181_END,

	//��̨�������ã�Ѳ���ߣ�Ԥ�õ�ȣ�ͨ������ӵ���ID����˿ں�Э��ŵ���NCFG_ITEM_PTZ_SERIAL����
	NCFG_ITEM_PTZ_INFO_BASE=0xf800,
	NCFG_ITEM_PTZ_INFO_PRESET_INFO_CHBASE=0xf800,//Ԥ�õ�����ͨ������������64��ͨ��ID��λ��
	NCFG_ITEM_PTZ_INFO_PRESET_INFO_CHEND =NCFG_ITEM_PTZ_INFO_PRESET_INFO_CHBASE+64,

	NCFG_ITEM_PTZ_INFO_CRUISE_INFO_CHBASE=NCFG_ITEM_PTZ_INFO_PRESET_INFO_CHEND,//Ѳ��������ͨ������������64��ͨ��ID��λ��
	NCFG_ITEM_PTZ_INFO_CRUISE_INFO_CHEND =NCFG_ITEM_PTZ_INFO_CRUISE_INFO_CHBASE+64,
	NCFG_ITEM_PTZ_INFO_END=NCFG_ITEM_PTZ_INFO_CRUISE_INFO_CHEND+1,
	
	///ֻ����Ϣ������������󳤶����ƣ�֧��ѡ��ķ�Χ,�汾��Ϣ��
	NCFG_ITEM_ONLYREAD_BASE=0xfc00,
	NCFG_ITEM_DEVICE_NAME_MAX_LEN,      //�豸���Ƴ����޶�, unsigned long,[ֻ��]
	NCFG_ITEM_TIME_SERVER_MAX_LEN,      //����ʱ��ͬ����������ַ��󳤶�, unsigned long,[ֻ��]
	NCFG_ITEM_UPNP_FRIENDLY_NAME_MAX_LEN,//UPNP�豸��ʾ��������󳤶�, unsigned long,[ֻ��]
	NCFG_ITEM_CAMERA_NAME_MAX_LEN,      //ͨ��������󳤶�, unsigned long,[ֻ��]
	NCFG_ITEM_SENSOR_IN_NAME_MAX_LEN,   //������������󳤶�,unsigned long,[ֻ��]
	NCFG_ITEM_ALARM_OUT_NAME_MAX_LEN,   //�̵������������󳤶�, unsigned long,[ֻ��]
	NCFG_ITEM_PPPOE_USER_NAME_MAX_LEN,  //PPPoE�û�����󳤶ȣ�unsigned long,[ֻ��]
	NCFG_ITEM_PPPOE_PASSWORD_MAX_LEN,   //PPPoE������󳤶�, unsigned long,[ֻ��]
	NCFG_ITEM_DDNS_USER_NAME_MAX_LEN,   //DDNS�û�����󳤶�, unsigned long,[ֻ��]
	NCFG_ITEM_DDNS_PASSWORD_MAX_LEN,    //DDNS������󳤶�, unsigned long,[ֻ��]
	NCFG_ITEM_DDNS_DOMAIN_MAX_LEN,		//DDNS����������󳤶�, unsigned long,[ֻ��]
	NCFG_ITEM_WIRELESS_SSID_MAX_LEN,    //����SSID��󳤶�, unsigned long,[ֻ��]
	NCFG_ITEM_WIRELESS_KEY_MAX_LEN,     //��Կ��󳤶�, unsigned long,[ֻ��]


	NCFG_ITEM_TIME_STAMP_SUPPORT_POS,	//ʱ���֧�ֵ�λ�÷�ʽ��unsigned long ����ʱ�����ʹ�ô�ID(0x01 << pos��MASK),[ֻ��]
	NCFG_ITEM_SUPPORT_VIDEOFORMAT,		//֧�ֵ���Ƶ��ʽ,unsigned long,[ֻ��]
	NCFG_ITEM_SUPPORT_QUADMODE,			//֧�ֵķָ�ģʽ[ֻ��],�ڼ�λΪ1�ʹ���֧����һ���ָ�ģʽ��unsigned long,[ֻ��]
	NCFG_ITEM_USER_MAXNUM,				//֧���û����������unsigned long[ֻ��]
	NCFG_ITEM_SUPPORT_LANGUAGE,
	NCFG_ITEM_SUPPORT_MAXCLIENTNUM,     //֧�ֵ�ͬʱ���ߵ�����ͻ��˸�����
	NCFG_ITEM_HOLIDAY_MAX_NUM,			//����֧�ֵļ����ų����ڵ�����������ر��Ƿ�ѭ����ʽʱ���������ӣ�unsigned long
	NCFG_ITEM_DEVUSER_NAME_MAX_LEN,		//�豸�û�����󳤶ȣ�unsigned long[ֻ��]
	NCFG_ITEM_DEVUSER_PWD_MAX_LEN,		//�豸�û�������󳤶ȣ�unsigned long[ֻ��]
	NCFG_ITEM_VGA_SUPPORT_RESOLUTION,	//VGAоƬ֧�ֵķֱ���MASK��unsigned long���ο�dvrdvsdef.h�е�VGA_RESOLUTIONö��[ֻ��]
	NCFG_ITEM_VGA_SUPPORT_REFRESH,		//VGA֧�ֵ���Ļˢ���ʣ�ÿ�ֱַ����¶�������ͬ[ֻ��]
	NCFG_ITEM_DDNS_SERVER_INFO,			//DDNS�����б�֧�ֵ�DDNS����[ֻ��]

	NCFG_ITEM_MAX_PRE_ALARM_TIME,		//Ԥ¼������ʱ�䣨�룩
	NCFG_ITEM_ENCODE_MAJOR_SUPPORT_PROPERTY,//��ʾ֧�ֱַ��ʵĸ������ٸ����ɸ�NCFG_INFO_SUPPORT_FRAME_RATE[ֻ��]
	NCFG_ITEM_MAJOR_SUPPORT_QULITY_LEVELNUM,	
	NCFG_ITEM_MAJOR_SUPPORT_ENCODEMODE,
	NCFG_ITEM_MAJOR_SUPPORT_BITRATE_RANGE,
	NCFG_ITEM_ENCODE_MINOR_SUPPORT_PROPERTY,//��ʾ֧�ֱַ��ʵĸ������ٸ����ɸ�NCFG_INFO_SUPPORT_FRAME_RATE[ֻ��]
	NCFG_ITEM_MINOR_SUPPORT_QULITY_LEVELNUM,
	NCFG_ITEM_MINOR_SUPPORT_ENCODEMODE,
	NCFG_ITEM_MINOR_SUPPORT_BITRATE_RANGE,
	NCFG_ITEM_ENCODE_SIZE_MAJOR_ALL_CH,		//�������ֱ����Ƿ�֧������ͨ��һ������ unsigned long
	NCFG_ITEM_ENCODE_SIZE_MINOR_ALL_CH,		//�������ֱ����Ƿ�֧������ͨ��һ������ unsigned long
	NCFG_ITEM_ENCODE_SIZE_PHONE_ALL_CH,		//�ֻ������ֱ����Ƿ�֧������ͨ��һ������ unsigned long
	NCFG_ITEM_DEVICE_SUPPORT_REDUNDANCY,    //�豸�Ƿ�֧������¼��

	NCFG_ITEM_CRUISE_PRESET_MAXNUM,		//ÿ��Ѳ���ߵ����Ԥ�õ�����[ֻ��]
	NCFG_ITEM_CHNN_PRESET_MAX_NUM,		//ÿ��ͨ�������Ԥ�õ�����[ֻ��]
	NCFG_ITEM_CHNN_CRUISE_MAX_NUM,		//ÿ��ͨ�������Ѳ��������[ֻ��]
	NCFG_ITEM_PTZ_SUPPORT_INFO,			//֧�ֵ���̨Э����Ϣ
	
	NCFG_ITEM_DEFAULT_COLOR,			//�豸Ĭ��ɫ��ֵ
	
	//MDVR ������Ŀֻ����Ϣ
	NCFG_ITEM_SPEED_VALUE_MUX_LEN,		//�����ٶ�ֵ����󳤶�

	//cost�汾��������
	NCFG_ITEM_SUPPORT_SYNC_TIME,          //�Ƿ�֧������Уʱ.1-֧�֡�0-��֧��
	NCFG_ITEM_SUPPORT_DST,                //�Ƿ�֧������ʱ����.1-֧�֡�0-��֧��
	NCFG_ITEM_SUPPORT_MASK_AREA,          //�Ƿ�֧���ڵ�����.1-֧�֡�0-��֧��
	NCFG_ITEM_SUPPORT_MAIN_MONITOR_SPORT, //�Ƿ�֧�����豸����̨����,1-֧�֡�0-��֧��
	NCFG_ITEM_MAJOR_HAVE_ENCODE_AND_MAXBIT, //¼���Ƿ�֧�ֱ��뷽ʽ���������.1-֧�֡�0-��֧��
	NCFG_ITEM_MINOR_HAVE_ENCODE_AND_MAXBI,  //�����Ƿ�֧�ֱ��뷽ʽ���������.1-֧�֡�0-��֧��
	NCFG_ITEM_SPPORT_SIMPLE_USER_MANAGE,    //�Ƿ��Ǽ򵥵��û�����ģʽ.1-�򵥡�0-��ϸ.
	
	//�ʼ��е�ֻ����
	NCFG_ITEM_EMAIL_IMG_NUM_RANGE,       //email���ý����У�ͼƬ������ȡֵ��Χ��Ĭ��ֵ
	NCFG_ITEM_EMAIL_SNAP_TIME_RANGE,    //email���ý����У�ץȡͼƬʱ������ȡֵ��Χ��Ĭ��ֵ

	//�ڰ�������Ŀ
	NCFG_ITEM_BAN_ALLOW_NUM,			//�ڰ������б���


	NCFG_ITEM_SNAP_SIZE_RANGE,               //ץͼ�����У�ͼƬ�Ļ��ʵķֱ���
	NCFG_ITEM_SNAP_QULITY_LEVELNUM,   //ץͼ�����У�ͼƬ�Ļ��ʵ�ȡֵ��Χ��Ĭ��ֵ
    NCFG_ITEM_SNAP_IMG_NUM_RANGE,     //ץͼ�����У�ͼƬ������ȡֵ��Χ��Ĭ��ֵ
	NCFG_ITEM_SNAP_TIME_RANGE,        //ץͼ�����У�ץȡͼƬʱ������ȡֵ��Χ��Ĭ��ֵ
	NCFG_ITEM_NO_ADD_ONLYREAD,      //�Ժ�Ҫ�����ֻ�����ˡ����Ҫ���ֻ����ͷŵ������

	NCFG_ITEM_ONLYREAD_END,
}NCFG_ITEM_ID;

enum 
{
	NCFG_ENUM_SUPPORT_VIDEOFORMAT_PAL	= 0x0001,
	NCFG_ENUM_SUPPORT_VIDEOFORMAT_NTSC	= 0x0002
};

//CBRģʽ�£�ÿ�ֱַ��ʵ�������Χ
typedef struct _ncfg_info_bitraterang
{
	unsigned long low;//�������
	unsigned long high;//�������
}NCFG_INFO_BITRATE_RANG;


//�û��ģ�Ŀǰ֧�ֳ������ӾͶ���������ӣ�Ȩ�޺�ͨ��ȥ����,����ı�ͻ�ID������ID��ʵ��
typedef struct _ncfg_info_user
{
	unsigned long   bindMac;	//0 ���� 1��
	unsigned long   authority;
	unsigned long   group;

	//һ��Ϊ�������ͨ����Ȩ�ޣ�����λΪ1�������Ȩ�ޣ�0�����߱�Ȩ�ޡ�
	ULONGLONG		authLiveCH;			//�ֳ�Ԥ��ͨ��
	ULONGLONG		authRecordCH;		//�ֶ�¼�����
	ULONGLONG		authPlaybackCH;		//�����ͻط�
	ULONGLONG		authBackupCH;		//����
	ULONGLONG		authPTZCtrlCH;		//�������
	ULONGLONG		authRemoteViewCH;	//Զ��Ԥ��

	char            name[36];
	char            password[36];
	unsigned char   MAC[8];
}NCFG_INFO_USER;

typedef struct _ncfg_info_short_name
{
	char	name [36];
}NCFG_INFO_SHORT_NAME;

typedef struct _ncfg_info_long_name
{
	char	name [132];
}NCFG_INFO_LONG_NAME;


typedef struct _ncfg_info_support_language
{
	unsigned long languageId;
	char          name[64];
}NCFG_INFO_LANGUAGE;

typedef struct _ddns_server_info 
{
	unsigned char  ucDDNSID; /////��Ӧ�ķ�����ID   ID����0��Ӧ�ķ��������Ʋ���Ч
	unsigned char  supportproperty;//֧�ֵĳ��û�����������������Ե�MASK,NCFG_ENUM_DDNS_SUPPORT_PROPERTY
	unsigned char  noused[2];
	char           szDDNSServerName[64];  ////DDNS�������ĵ�ַ
}DDNS_SERVER_INFO, *LPDDNS_SERVER_INFO;

/*
typedef struct _ncfg_info_support_frame_rate
{
	unsigned long	dwResolution;	//ʲô�ֱ���
	unsigned long	dwRate;			//֧����Щ֡��
	char			strResolutionName[16];//�ֱ�������
}NCFG_INFO_SUPPORT_FRAME_RATE;
*/
typedef struct _ncfg_info_support_frame_rate
{
	unsigned long dwResolution;				//֧����Щ�ֱ���
	unsigned long totalRatePalOnCif; 	    //pal�Ƶ���֡�ʣ���CIF���㡣
	unsigned long chnnRatePalOnCif;         //pal�ƣ���ͨ�������֡��,��CIF���㡣
	unsigned long totalRateNtscOnCif;		//NTSC�Ƶ���֡�ʣ���CIF���㡣
	unsigned long chnnRateNtscOnCif;        //ntsc�ƣ���ͨ�������֡�ʣ���CIF����
    unsigned long defaResolution;           //��Ʒ��Ĭ�Ϸֱ��� 
	unsigned long defauChnnRateNumPal;      //��pal�£���Ĭ����ʽ�£�Ĭ��ÿ��ͨ��֡�ʵ���ֵ
	unsigned long defauChnnRateNumNtsc;
}NCFG_INFO_SUPPORT_FRAME_RATE;

typedef struct _ncfg_ipc_support_encode_
{
	unsigned long dwResolution;
	unsigned long maxChnnRate;
	unsigned long maxChnnRateOnVGA;
	unsigned long maxTotalRateOnVGA;
	unsigned long maxTotalBitRate;
	unsigned long defaResolution;
	unsigned long defaChnnRate;
	unsigned long defaBitRate;
	unsigned long bLimitByBitRate;	//0:������ȡ����֡�ʣ�1��������ȡ��������
}NCFG_IPC_SUPPORT_ENCODE;

typedef struct _ncfg_info_spec_frame_rate_ex
{
	unsigned long bSupport;                  //�Ƿ�֧�������¼������
	ULONGLONG     videoSizeCIFChnnBits;      //������
	ULONGLONG     videoSizeD1ChnnBits;       //��Щͨ���������ó�ʵʱ��D1��ͨ������
	ULONGLONG     res1;
	ULONGLONG     res2;
	ULONGLONG     res3;
	ULONGLONG     res4;
}NCFG_INFO_ENCODE_SPEC_FRAME_RATE;

//typedef struct _ncfg_info_time_pos
//{
//	unsigned long postype;
//	unsigned long customdefine_X;
//	unsigned long customdefine_Y;
//}
//NCFG_INFO_TIME_POS;

typedef struct _ncfg_info_toolbar_pos
{
	unsigned short	style;			//0�ײ�, 1����, 3����[��(x,y)ֵ]
	unsigned short	status;			//0�̶�[���Զ�����], 1�̶�[�Զ�����], 2�༭[������]
	unsigned short  preDelayTime;	//������ڹ��������Զ���ʾ�ĵȴ�ʱ��
	unsigned short  postDelayTime;	//����뿪���������Զ����صĵȴ�ʱ��
	unsigned short	xPos;			//�ο���ʼ����, �ٷֱ�, ����100 ����
	unsigned short	yPos;

	unsigned short	ItemIndex[32];	//���ܰ�ť�Ų�����
}NCFG_INFO_TOOLBAR_POS;

//#if defined (__CUSTOM_BR27__)
	typedef struct _ncfg_info_group_item_
	{
		char szItem[32];
		ULONGLONG chanbits;
	}NCFG_INFO_GROUP_ITEM;
	typedef struct _ncfg_info_group_item_enableInfo
	{
		unsigned long enable;//��������ʹ��
		unsigned long curItem;//��ǰ�ķ������
	}NCFG_INFO_GROUP_ENABLEINFO;
//#endif
typedef struct _ptz_protocol_info 
{
	unsigned long  ucprotocolID;		//��Ӧ��Э������ID
	unsigned long  supportproperty;		//֧�ֵĳ�ID��������֮����������Ե�MASK,�����Ƿ�֧�ֹ켣����������
	char           szProtocolName[64];  ////Э������
}PTZ_PROTOCOL_INFO;

//DDNS֧���������Ե�ȡֵ��Χ���������������Ͷ�����
typedef enum _ncfg_enum_ddns_support_property
{
	NCFG_ENUM_DDNS_SUPPORT_DOMAIN1	=0x01,//֧������1��Ҳ�п�����Ҫ֧����������	
}
NCFG_ENUM_DDNS_SUPPORT_PROPERTY;

//NCFG_ITEM_PTZ_INFO_CRUISE_INFO_CHBASE
typedef struct _ncfg_info_cruise_head
{
	unsigned long index;
	unsigned long cruisepointnum;//Ѳ��������
	char  name[36];
	unsigned long offset;//������ÿ��Ѳ������Ե�һ��NCFG_INFO_CRUISE_HEAD��ͷ��ƫ��
}
NCFG_INFO_CRUISE_HEAD;

const BYTE CRUISE_NAME_MAX_LEN =32;
//Ѳ������Ϣͷ��ֻ����һ��ͨ����
typedef struct _ptz_cruise_info_head
{
	unsigned long cruiseFlag;//��ʼֹͣ��� 
	unsigned long cruisepointnum;//Ѳ��������
	unsigned long cruiseIndex;//Ѳ�������
	char  name[CRUISE_NAME_MAX_LEN+4];
}CRUISE_INFO_HEAD;

//�����ų�ͷ��Ϣ
typedef struct _ncfg_info_holiday_sche_head
{
	unsigned long holidaynum;//��������
	unsigned long offset;//������ÿ���ų�����Ե�һ��NCFG_INFO_HOLIDAY_SCHE_HEAD��ͷ��ƫ��
}
NCFG_INFO_HOLIDAY_SCHE_HEAD;

typedef struct _ncfg_cvbs__{
	unsigned long last;
	unsigned long current;
}NCFG_CVBS_INFO;

inline unsigned char ViewSplitToIndex(NCFG_ITEM_ID viewSplitID)
{
	unsigned char index = 0;
	switch(viewSplitID)
	{
	case NCFG_ITEM_VIEW_SPLIT_1X1:
		index = 0;
		break;
	case NCFG_ITEM_VIEW_SPLIT_2X2:
		index = 1;
		break;
	case NCFG_ITEM_VIEW_SPLIT_2X3:
		index = 2;
		break;
	case NCFG_ITEM_VIEW_SPLIT_3X3:
		index = 3;
		break;
	case NCFG_ITEM_VIEW_SPLIT_4X4:
		index = 4;
		break;
	case NCFG_ITEM_VIEW_SPLIT_4X6:
		index = 5;
		break;
	case NCFG_ITEM_VIEW_SPLIT_5X5:
		index = 6;
		break;
	case NCFG_ITEM_VIEW_SPLIT_6X6:
		index = 7;
		break;
	default:
		index = 0;
		break;
	}

	return index;
}

#if defined(__ENVIRONMENT_WIN32__) || defined(__TDFH__) || defined(__TDNXP__) || defined(__CHIP3531__) || defined(__CHIP3535__) || defined(__CHIP3521__) || defined(__CHIP3520A__) || defined(__CHIPTI__) || defined(__CHIP3520D__) || defined(__CHIPGM__) 
#pragma pack()
#endif


#endif //_NET_CONFIG_DEFINE_H_


