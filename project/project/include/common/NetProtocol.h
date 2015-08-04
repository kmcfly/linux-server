/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : Ԭʯά
** Date         : 2007-12-05
** Name         : NetProtocol.h
** Version      : 1.0
** Description  :
** Modify Record:
1:
***********************************************************************/
#ifndef _NET_PROTOCOL_H_
#define _NET_PROTOCOL_H_
#include "Typedef.h"
#include "dvrdvsdef.h"
#include "PTZ.h"
#include "Product.h"

#if defined(__ENVIRONMENT_WIN32__) || defined(__TDFH__) || defined(__TDNXP__) || defined(__CHIP3531__) || defined(__CHIP3535__) || defined(__CHIP3521__) || defined(__CHIP3520A__) || defined(__CHIPTI__) || defined(__CHIP3520D__) || defined(__CHIPGM__) 
#pragma pack(4)
#endif
/*/////////////////////////////////////////////////////////////////
//�汾������� 
//1���豸��һֱֻ������һ���汾��֧�֣������豸�˸�����
//2���ͻ���һֱ���ֶ�֮ǰ�ľɰ汾��֧��
*//////////////////////////////////////////////////////////////////


typedef struct _pack_cmd
{
	unsigned long cmdType;//������
	unsigned long cmdID;//ָ����ţ�ָ�����趨��һ����ֵ����Ӧ��ָ���ͷ�е�ָ�����������ȫ��ͬ
	//�㲥���û�������״̬�����IDԼ���̶�Ϊ0xffffffff���ڿͻ��˲�������IDʱ������ʹ�����ֵ��
	unsigned long cmdVer;//����汾������ֱ�Ӹ�������汾ȷ���������ĸ�ʽ
	unsigned long dataLen;//���ݳ���
}PACKCMD;

const int PACK_HEAD_LENGTH=sizeof(PACKCMD);
//ռ�����ֽ�
typedef enum _NET_PROTOCOL_H_
{
	//�ͻ��˵�¼
	CMD_BASENUM_LOGIN		= 0x0100,
	CMD_REQUEST_LOGIN,			//�ͻ��������¼
	CMD_REQUEST_LOGOUT,			//�ͻ�������ǳ�
	CMD_END_LOGIN,

	//�������˻ظ���¼��Ϣ
	CMD_BASENUM_REPLY_LOGIN	= 0x010000,
	CMD_REPLY_LONGIN_SUCC,		//�����¼�������豸��Ϣ
	CMD_REPLY_LONGIN_FAIL,		//�ܾ���¼�����ߵ�¼ʧ��
	CMD_END_REPLY_LOGIN,

	//�ͻ��˿��ƣ�ϵͳ����
	CMD_BASENUM_CTRL		= 0x0200,
	CMD_REQUEST_STREAM_START,			//�ͻ�������ʼ�����������縱��������Ƶ��
	CMD_REQUEST_STREAM_CHANGE,			//�ͻ�������ı������������縱��������Ƶ��
	CMD_REQUEST_STREAM_STOP,			//�ͻ�������ֹͣ�����������縱��������Ƶ��
	CMD_REQUEST_KEYFRAME,				//����ͨ���ؼ�֡������������ͨ������ϣ��������������
	CMD_REQUEST_SHUTDOWN,				//�ͻ�������ػ�
	CMD_REQUEST_REBOOT,					//�ͻ�����������ϵͳ
	CMD_REQUEST_MANUAL_REC,				//�ͻ��������ֶ�¼��
	CMD_REQUEST_CHANGE_TIME,			//����ͻ��������޸�ϵͳʱ��
	CMD_REQUEST_L_CHANGE_ZONE_TIME,		//���ؿͻ��������޸�ϵͳ[ʱ����ʱ��]
	CMD_REQUEST_SYN_TIME_NOW,			//���������ں�NTP����������ʱ��ͬ��
	CMD_REQUEST_CHANGE_DISPLAY_MODE,	//�����л��ֳ����ط���ʾģʽ���絥���棬4����
	CMD_REQUEST_GET_DISPLAY_INFO,		//�����ѯ��ʾ״̬
	CMD_REQUEST_CONTROL_LIVE_AUDIO,		//����ϵͳ�ֳ�����
	CMD_REQUEST_MODIFY_AUDIO_VOLUME,	//����ϵͳ����
	CMD_REQUEST_DISK_NUM_STATUS,		//����ϵͳӲ��������״̬������Ӳ�̣�����Ӳ�̽����
	CMD_REQUEST_DELETE_OVERDUEFILES,	//ɾ�������ļ�
	CMD_REQUEST_DODWELL_START,			//����ʼ��̨
	CMD_REQUEST_DODWELL_STOP,			//���������̨
	CMD_REQUEST_OPEN_ALARM,				//�ֶ���������
	CMD_REQUEST_CLEAN_ALARM,			//�ֶ�������б���
	CMD_REQUEST_MODIFY_MAC,             //�����޸�mac��ַ
	CMD_REQUEST_MODIFY_HARDWARE_VER,    //�����޸�Ӳ���汾��
	CMD_REQUEST_MODIFY_PROD_SUB_ID,     //�����޸Ĳ�Ʒ�Ӱ汾��
	CMD_REQUEST_MODIFY_BOOT_IMAGE,      //�޸Ŀ�������
	CMD_REQUEST_MODIFY_DDNS_TYPE,       //�޸Ĳ�ͬ��˾��֧�ֵ�DDNS����
	CMD_REQUEST_MODIFY_CRYPTO_INFO,     //�޸Ļ���֧�ֵ���������
	CMD_REQUEST_EXFACTORY_SETUP,		//����ָ���������
	CMD_REQUEST_RESTOR_ADMIN_PASS,      //����ָ�admin�û�������.
	CMD_REQUEST_SET_KEYBOARD_NAME,      //���û�����֧�ֵİ�����
	CMD_REQUEST_EVICT_SOMEONE,			//����ĳ���ͻ���
	CMD_REQUEST_ZOOM,					//���ӷŴ�, ZOOM_INFO
	CMD_REQUEST_L_CHANGE_TIME,          //���ؿͻ��������޸�ʱ��
	CMD_REQUEST_JPEG_START,				//
	CMD_REQUEST_JPEG_CHANGE,			//
	CMD_REQUEST_JPEG_STOP,				//
	CMD_REQUEST_SET_FUNC,
	CMD_REQUEST_GET_DEV_COLOR,
	CMD_REQUEST_DEV_COLOR_SET,
	CMD_REQUEST_DEV_DEFA_COLOR,
	CMD_REQUEST_MANUAL_SNAP,
	CMD_REQUEST_DISK_SMART_INFO,
	CMD_REQUEST_SET_NETWORK,			//�����޸�������Ϣ
	CMD_REQUEST_YUANZHENG_MANUAL_REC,   //Զ�����ֶ�¼��
	CMD_REQUEST_SET_YUANZHENG_PARAM,    //Զ������������
	CMD_REQUEST_SET_BURN_INFO,			//���ÿ�¼ͨ����Ϣ
	CMD_REQUEST_SET_BURN_REC,			//��ʼ��¼
	CMD_REQUEST_MODIFY_COMPANY_NAME,	//�޸ĳ�������
	CMD_REQUEST_GB81_REC_CTRL,			//�������¼��
	CMD_END_CTRL,

	//�������˻ظ�����
	CMD_BASENUM_REPLY_CTRL	= 0x020000,
	CMD_REPLY_CTRL_SUCC,			//���Ƴɹ�
	CMD_REPLY_CTRL_FAIL,			//����ʧ��
	CMD_REPLY_STREAM_CHANGE,		//�ı�����
	CMD_REPLY_CHANAGE_TIME_SUCC,	//�ظ��޸�ϵͳʱ��ɹ�
	CMD_REPLY_CHANAGE_TIME_FAIL,	//�ظ��޸�ϵͳʱ��ʧ��
	CMD_REPLY_CHANAGE_DISPLAY_SUCC,	//�ظ��ı��ֳ�ͨ���ɹ�
	CMD_REPLY_SYN_TIME_NOW_SUCC,	//����ͬ��ʱ��ɹ�
	CMD_REPLY_SYN_TIME_NOW_FAIL,	//����ͬ��ʱ��ʧ��
	CMD_REPLY_DEV_COLOR,
	CMD_REPLY_DISK_SMART_SUCC,
	CMD_REPLY_DISK_SMART_FAIL,
	CMD_END_REPLY_CTRL,

	//�ͻ���ͨ������
	CMD_BASENUM_CHNN_CTRL	= 0x0300,
	CMD_REQUEST_COLOR_GET,		//��ȡĳͨ�����ֳ�ͼ�����ɫֵ
	CMD_REQUEST_COLOR_SET,		//����ĳͨ�����ֳ�ͼ�����ɫֵ
	CMD_REQUEST_COLOR_DEFAULT,	//�ط�ĳͨ����ɫֵΪĬ��ֵ
	CMD_REQUEST_COLOR_SAVE,		//����ĳͨ�����ֳ�ͼ����ɫֵ
	CMD_REQUEST_CHNN_NAME,		//����ĳ��ͨ��������	

	CMD_REQUEST_SET_IMAGE_MIRROR = 0x0319,	//ͼ���� unsigned char
	CMD_REQUEST_SET_IMAGE_FILP,		        //ͼ��ת unsigned char
	CMD_REQUEST_SET_IMAGE_ROTATO,        	//ͼ����ת unsigned long
	CMD_REQUEST_SET_AUTOFOCUS,	            //AF,���ڽ����Zoom  AF_CONTROL
	CMD_REQUEST_SET_IMAGE_DISTORTION,       //����У�� NCFG_ITEM_LENS_DISTORTION
	CMD_REQUEST_SET_IMAGE_ROI,

	CMD_REQUEST_SAVE_IMAGE_MIRROR,
	CMD_REQUEST_SAVE_IMAGE_FILP,
	CMD_REQUEST_SAVE_IMAGE_ROTATO,
	CMD_REQUEST_SAVE_IMAGE_DISTORTION,
	CMD_REQUEST_SAVE_IMAGE_ROI,

	CMD_REQUEST_GET_IMAGE_MIRROR,
	CMD_REQUEST_GET_IMAGE_FILP,
	CMD_REQUEST_GET_IMAGE_ROTATO,
	CMD_REQUEST_GET_IMAGE_DISTORTION,
	CMD_REQUEST_GET_IMAGE_ROI,
	CMD_REQUEST_SET_CAM_PARA_LOCAL_NUM,
	CMD_REQUEST_CHANGE_CVBS,
	CMD_END_CHNN_CTRL,

	//�������˻ظ�ͨ����Ϣ
	CMD_BASENUM_REPLY_CHNN_CTRL	= 0x030000,
	CMD_REPLY_CHNN_CTRL_COLOR,	//�ظ���ĳ��ͨ������ɫֵ
	CMD_REPLY_COLOR_SAVE_SUCC,	//����ĳ��ͨ������ɫֵ�ɹ�
	CMD_REPLY_COLOR_SAVE_FAIL,	//����ĳ��ͨ����ɫֵʧ��
	CMD_REPLY_SAVE_IMAGE_MIRROR_SUCC,
	CMD_REPLY_SAVE_IMAGE_FILP_SUCC,
	CMD_REPLY_SAVE_IMAGE_ROTATO_SUCC,
	CMD_REPLY_SAVE_IMAGE_DISTORTION_SUCC,
	CMD_REPLY_SAVE_IMAGE_ROI_SUCC,
	CMD_REPLY_SAVE_IMAGE_MIRROR_FAIL,
	CMD_REPLY_SAVE_IMAGE_FILP_FAIL,
	CMD_REPLY_SAVE_IMAGE_ROTATO_FAIL,
	CMD_REPLY_SAVE_IMAGE_DISTORTION_FAIL,
	CMD_REPLY_SAVE_IMAGE_ROI_FAIL,
	CMD_REPLY_GET_IMAGE_MIRROR,
	CMD_REPLY_GET_IMAGE_FILP,
	CMD_REPLY_GET_IMAGE_ROTATO,
	CMD_REPLY_GET_IMAGE_DISTORTION,
	CMD_REPLY_GET_IMAGE_ROI,
	CMD_REPLY_SET_AUTOFOCUS_SUCC,
	CMD_REPLY_SET_AUTOFOCUS_FAIL,
	CMD_END_REPLY_CHNN_CTRL,

	//�ͻ�������
	CMD_BASENUM_CFG			= 0x0400,
	//���ý��롢���󡢱��桢�뿪����
	CMD_REQUEST_CFG_ENTER,		//��������
	CMD_REQUEST_CFG_LEAVE,		//�뿪����
	CMD_REQUEST_CFG_GET,		//��������
	CMD_REQUEST_CFG_SET,		//��������
	CMD_REQUEST_CFG_DEF_DATA,	//����Ĭ���������ݣ���������ݺ�GETһ����ֻ�Ƿ��صĽ��ΪĬ������
	CMD_REQUEST_CFG_DEFAULT,	//�ظ�Ĭ������
	CMD_REQUEST_CFG_MODIFY_PASS,//�����޸����룬�޸�����ǰ��Ҫ��������

	//����һЩ��������صĿ���Э��
	CMD_REQUEST_CFG_NET,		//ͨ���ಥ����������صļ�������
	CMD_REQUEST_CFG_IMPORT,		//������ȫ��������Ϣ������ȫ��������Ϣ
	CMD_REQUEST_CFG_EXPORT,		//���󵼳�ȫ��������Ϣ

	CMD_END_CFG,

	//�������˻ظ�������Ϣ
	CMD_BASENUM_REPLY_CFG		= 0x040000,
	CMD_REPLY_CFG_SUCC,			//�����������ʱ�ظ��ɹ�
	CMD_REPLY_CFG_DATA,			//�ظ���������
	CMD_REPLY_CFG_DEFAULT,		//�ظ��ָ�Ĭ��������Ϣ
	CMD_REPLY_CFG_FAIL,			//�ظ�����ʧ�ܣ�����ʧ����
	CMD_REPLY_CFG_MODIFY_PASS_SUCC, //�ظ��޸�����ɹ�
	CMD_REPLY_CFG_MODIFY_PASS_FAIL, //�ָ��޸�����ʧ��
	CMD_REPLY_CFG_IMPORT_SUCC,		//�������óɹ�
	CMD_REPLY_CFG_IMPORT_FAIL,		//��������ʧ��
	CMD_REPLY_CFG_EXPROT_SUCC,		//�������óɹ�
	CMD_REPLY_CFG_EXPROT_FAIL,		//��������ʧ��

	CMD_END_REPLY_CFG,

	//�ͻ������ݿ���
	CMD_BASENUM_DATA		= 0x0500,
	CMD_REQUEST_DATA_SEARCH,	//��������
	CMD_REQUEST_DATA_READ,		//�����ȡ����
	CMD_REQUEST_DATA_PAUSE,		//��ͣ��ȡ����
	CMD_REQUEST_DATA_RESUME,    //�ָ�����״̬
	CMD_REQUEST_DATA_FF,		//���
	CMD_REQUEST_DATA_REW,		//����
	CMD_REQUEST_DATA_STOP,		//ֹͣ��ȡ����
	CMD_REQUEST_DATA_LOCK,		//�����ļ�
	CMD_REQUEST_DATA_UNLOCK,	//�����ļ�
	CMD_REQUEST_DATA_DELETE,	//ɾ���ļ�
	CMD_REQUEST_DATA_INDEX,		//�ظ��Ѿ������֡INDEX
	CMD_REQUEST_LAST_STREAM_ERROR,//���һ�������ֳ���ط���ʱ������Ϣ
	CMD_REQUEST_CHANGE_AUDIO,
	CMD_REQUEST_PIC_LOCK,
	CMD_REQUEST_PIC_UNLOCK,
	CMD_REQUEST_PIC_DELETE,	
	CMD_REQUEST_DISK_PIC_INFO,
	CMD_REQUEST_DEL_DISK_PIC,
	CMD_END_DATA,

	//�������˻ظ�����
	CMD_BASENUM_REPLY_SEARCH	= 0x050000,
	CMD_REPLY_DATA_INFO,			//�ظ�������������ݣ���������
	CMD_REPLY_DATA_NULL,			//�ظ���������ʾ������
	CMD_REPLY_LAST_STREAM_ERROR,	//
	CMD_REPLY_DISK_PIC_INFO,
	CMD_END_REPLY_SEARCH,

	//�ͻ��˲�ѯ�¼�
	CMD_BASENUM_SYS_STATUS		= 0x0600,
	CMD_REQUEST_EVENT_INFO,			//�����ѯ�¼�
	CMD_REQUEST_LOG_INFO,			//�����ѯ�������쳣�¼�
	CMD_REQUEST_CLIENT_INFO,		//�鿴�����������豸�Ͽͻ�����Ϣ
	CMD_REQUEST_FIRST_ENCODE_INFO,	//��ѯ������������Ϣ
	CMD_REQUEST_SECOND_ENCODE_INFO,	//��ѯ������������Ϣ
	CMD_REQUEST_THREE_ENCODE_INFO,	//��ѯ�ֻ���������Ϣ
	CMD_REQUEST_NETWORK_INFO,		//��ѯ����״̬����
	CMD_REQUEST_LOGIN_USER_INFO,	//��ѯ��¼�û�����Ϣ(��������)
	CMD_REQUEST_EXPORT_LOG,			//���󵼳���־
	CMD_REQUEST_EXPORT_EVENT,		//���󵼳��¼��б�
	CMD_REQUEST_WIRELESS_INFO,		//��������������Ϣ
	CMD_REQUEST_DTNETWORK,			//��ȡ��̬ip
	CMD_REQUEST_FIRST_SUPPORT_INFO,	//����֧�ֵ�������������Ϣ
	CMD_REQUEST_FIRST_CHECK_ENC_INFO,//�����������������Ƿ񳬹�����
	CMD_REQUEST_NAT_INFO,			//��ѯNAT��Ϣ
	CMD_REQUEST_SECOND_SUPPORT_INFO, //����֧�ֵ�������������Ϣ
	CMD_REQUEST_SECOND_CHECK_ENC_INFO,//�����������������Ƿ񳬹�����
	CMD_REQUEST_APP_DOWNLOAD_LINK_INFO,//����phone app ������Ϣ
	CMD_REQUEST_TOKEN_SERVER_INFO,		//����token server ��Ϣ
	CMD_END_SYS_STATUS,

	//�������˻ظ���ѯ�¼�
	CMD_BASENUM_REPLY_SYS_STATUS	= 0x060000,
	CMD_REPLY_EVENT_INFO,			//��ѯ�¼��������¼��б�
	CMD_REPLY_LOG_INFO,				//��ѯ�������쳣�¼��������б�
	CMD_REPLY_CLIENT_INFO,			//���пͻ��˵���Ϣ
	CMD_REPLY_FIRST_ENCODE_INFO,	//�ظ��������ı��������Ϣ
	CMD_REPLY_SECOND_ENCODE_INFO,	//�ظ����������������Ϣ
	CMD_REPLY_THREE_ENCODE_INFO,	//�ظ��ֻ������������Ϣ
	CMD_REPLY_NETWORK_INFO,			//�ظ�����״̬����
	CMD_REPLY_LOGIN_USER_INFO,		//�ظ���¼�û�����Ϣ
	CMD_REPLY_SYS_STATUS_FAIL,		//��ѯ��Ϣʧ��,����ʧ����
	CMD_REPLY_WIRELESS_INFO,		//�ظ���ѯ����״̬��Ϣ
	CMD_REPLY_DTNETWORK,			//�ظ���̬ip
	CMD_REPLY_FIRST_SUPPORT_INFO,	//��Ӧ������������Ϣ
	CMD_REPLY_FIRST_CHECK_ENC_INFO, //��Ӧ��������������Ƿ񳬹�����
	CMD_REPLY_SECOND_SUPPORT_INFO,  //��Ӧ������������Ϣ
	CMD_REPLY_SECOND_CHECK_ENC_INFO,//��Ӧ��������������Ƿ񳬹�����

	CMD_REPLY_APP_DOWNLOAD_LINK_INFO_SUCC,//����app������Ϣ�ɹ�
	CMD_REPLY_APP_DOWNLOAD_LINK_INFO_FAIL,//����app������Ϣʧ��
	CMD_REPLY_TOKEN_SERVER_INFO_SUCC,	 //����token server ��Ϣ�ɹ�
	CMD_REPLY_TOKEN_SERVER_INFO_FAIL,	 //����token server ��Ϣʧ��
	CMD_END_REPLY_SYS_STATUS,

	//�ͻ�����̨����
	CMD_BASENUM_PTZ			= 0x0700,
	CMD_REQUEST_PTZ_START,			//������̨����
	CMD_REQUEST_PTZ_CTRL,			//��̨��������
	CMD_REQUEST_PTZ_PRESET_INFO,	//Ԥ�õ���Ϣ
	CMD_REQUEST_PTZ_CRUISE_INFO,	//Ѳ������Ϣ
	CMD_REQUEST_PTZ_TRACK_INFO,		//�켣��Ϣ
	CMD_REQUEST_PTZ_STOP,			//������̨����
	CMD_REQUEST_PTZ_STATUS,			//������̨״̬
	CMD_REQUEST_PTZ_CRUISE_PREVIEW,	//����Ѳ����Ԥ��
	CMD_REQUEST_PTZ_CRUISE_PREVIEW_RECOVER,		//�ָ�Ԥ����Ѳ����
	CMD_REQUEST_PTZ_STOP_CURRENT_STATUS,		//ֹͣ��̨�ĵ�ǰ״̬����Ҫ��ֹͣ�Զ�ɨ�衢Ѳ�����켣
	
	CMD_END_PTZ,

	//�������ظ���̨Ԥ�õ㡢Ѳ����Ϣ
	CMD_BASENUM_REPLY_PTZ       = 0x70000,
	CMD_REPLY_PTZ_START_SUCC,       //����ʼ��̨���سɹ�
	CMD_REPLY_PTZ_START_FAIL,       //����ʼ��̨����ʧ�ܣ���������ԭ��
	CMD_REPLY_PTZ_SET_SUCC,			//������̨�����ɹ�	
	CMD_REPLY_PTZ_SET_FAIL,			//������̨����ʧ��
	CMD_REPLY_PTZ_PRESET_INFO,      //Ӧ����̨Ԥ�õ���Ϣ
	CMD_REPLY_PTZ_CURISE_INFO,      //Ӧ����̨Ѳ������Ϣ
	CMD_REPLY_PTZ_TRACK_INFO,		//Ӧ����̨�켣����Ϣ
	CMD_REPLY_PTZ_SEND_CMD_SUC,		//����̨��������ɹ�
	CMD_REPLY_PTZ_SEND_CMD_FAIL,	//����̨��������ʧ��
	CMD_REPLY_PTZ_STATUS_SUC,		//������̨״̬�ɹ�
	CMD_REPLY_PTZ_STATUS_FAIL,		//������̨״̬ʧ��
	CMD_REPLY_PTZ_CRUISE_PREVIEW_SUC,//Ѳ����Ԥ���ɹ�
	CMD_REPLY_PTZ_CRUISE_PREVIEW_FAIL,//Ѳ����Ԥ��ʧ��
	CMD_END_REPLY_PTZ,

	//�ͻ��˴��̹���
	CMD_BASENUM_DISK		= 0x0800,
	CMD_REQUEST_DISK_START,	                //������̹���
	CMD_REQUEST_DISK_MOUNT_REMOVABLE,       //�����ƶ��豸�����ҹ���
	CMD_REQUEST_DISK_GET_REMOVABLE_PARTION, //��ȡ�ƶ��豸��ÿ����������Ϣ
	CMD_REQUEST_DISK_UNMOUNT_REMOVABLE,     //ж���ƶ��豸
	CMD_REQUEST_DISK_GET_CD_INFO,           //�����������Ϣ
	CMD_REQUEST_DISK_STOP,			        //�������̹���
	CMD_REQUEST_DISK_FORMAT_START,		    //��ʽ��ĳ������
	CMD_REQUEST_DISK_FORMAT_STOP,			//ֹͣ��ʽ��ĳ������
	CMD_REQUEST_DISK_CHANGE_PROPERTY,		//�޸Ĵ�������
	CMD_REQUEST_DISK_GET_OWNER,				//��ȡ����������Ϣ
	CMD_REQUEST_DISK_SET_SLEEP,				//���ô�������
	CMD_REQUEST_DISK_FULL_ALARM,            //����������
	CMD_REQUEST_DISK_FORMAT_PERCENT,        //�����ʽ������
	CMD_END_DISK,

	//�������˻ظ����̹���
	CMD_BASENUM_REPLY_DISK		= 0x80000,
	CMD_REPLY_DISK_INFO,				//�ظ����̹���ɹ�������������Ϣ
	CMD_REPLY_DISK_FAIL,				//�ظ����̹�������ʧ����
	CMD_REPLY_DISK_MOUNT_REMOVABLE_SUC,	//�ֶ�ж�ش��̳ɹ�
	CMD_REPLY_DISK_MOUNT_REMOVABLE_FAIL,//�ֶ�ж�ش���ʧ��
	CMD_REPLY_FORMAT_FAIL,				//��ʽ��ʧ��
	CMD_REPLY_FORMAT_PERCENT,			//��ʽ������
	CMD_REPLY_FORMAT_SUCC,				//��ʽ�����̳ɹ�
	CMD_REPLY_DISK_CHANGE_PROPERTY_SUC,	//���ô������Գɹ�
	CMD_REPLY_DISK_CHANGE_PROPERTY_FAIL,//���ô�������ʧ��
    CMD_REPLY_DISK_GET_OWNER_SUC,		//�޸Ĵ��������ɹ�
	CMD_END_REPLY_DISK,

	//�ƶ��������
	CMD_BASENUM_MOTION_AREA = 0X0900,
	CMD_REQUEST_MOTION_AREA_GET,		//�����ȡ�ƶ��������
	CMD_REQUEST_MOTION_AREA_SET,		//���󱣴��ƶ��������
	CMD_END_MOTION_AREA,             

	//�������ظ��ƶ��������
	CMD_BASENUM_REPLY_MOTION_AREA   = 0X90000,
	CMD_REPLY_MOTION_AREA_GET_SUCC,     //�ظ��ƶ����������Ϣ
	CMD_REPLY_MOTION_AREA_SET_SUCC,     //�ظ������ƶ��������ɹ�
	CMD_REPLY_MOTION_AREA_GET_FAIL,     //�ظ���ȡ�ƶ����������Ϣʧ��
	CMD_REPLY_MOTION_AREA_SET_FAIL,     //�ظ������ƶ��������ʧ��
	CMD_END_REPLY_MOTION_AREA,

	//�ͻ�����������
	CMD_BASENUM_UPDATE		= 0x0a00,
	CMD_REQUEST_UPDATE_START,		//�����������
	CMD_REQUEST_UPDATE_DATA,		//�ϴ����������
	CMD_REQUEST_UPDATE_CANCEL,		//��������
	CMD_REQUEST_UPDATE_ORIGIN,
	CMD_REQUEST_UPDATE_UPDATE,      //�ϴ������ݰ��󣬽�������.
	CMD_END_UPDATE,

	//�������˻ظ�����
	CMD_BASENUM_REPLY_UPDATE	= 0x0a0000,
	CMD_REPLY_CAN_UPDATE,		//�ظ���ʾ��������
	CMD_REPLY_NOT_UPDATE,		//�ظ���ʾ��������
	CMD_REPLY_UPDATE_FAIL,		//�ظ�����ʧ����Ϣ
	CMD_REPLY_UPDATE_PERCENT,	//�ظ���������
	CMD_END_REPLY_UPDATE,

	//�Խ��������
	CMD_BASENUM_TALK			= 0x0b00,
	CMD_REQUEST_TALK_START,		    //����ʼ�Խ�
	CMD_REQUEST_TALK_STREAM,		//�ͻ��˸��豸���ͶԽ�����Ƶ����
	CMD_REQUEST_TALK_STOP,			//ֹͣ�Խ�
	CMD_END_TALK,             

	//����������ػظ�
	CMD_BASENUM_REPLYY_TALK		= 0x0b0000,
	CMD_REPLY_TALK_START_SUCC,		//�ظ�����ʼ�Խ��ɹ�    
	CMD_REPLY_TALK_START_FAIL,		//�ظ�����ʼ�Խ�ʧ��    
	CMD_END_REPLY_TALK, 

	//���ؿͻ��˻طſ���
	CMD_BASENUM_LOCAL_CLIENT_PLAYBACK_CTRL = 0x0c00,
	CMD_REQUEST_LOCAL_CLIENT_PLAYBACK_PLAY,		//���ػطŲ���
    CMD_REQUEST_LOCAL_CLIENT_PLAYBACK_STOP,		//���ػط�ֹͣ
	CMD_REQUEST_LOCAL_CLIENT_PLAYBACK_PAUSE,	//���ػط���ͣ
	CMD_REQUEST_LOCAL_CLIENT_PLAYBACK_RESUM,	//���ػطŻָ�
	CMD_REQUEST_LOCAL_CLIENT_PLAYBACK_CHG_CHNN,	//���ػط��л�ͨ��
	CMD_REQUEST_LOCAL_CLIENT_PLAYBACK_CHG_TIME,	//���ػط��л�ʱ��
	CMD_REQUEST_LOCAL_CLIENT_PLAYBACK_NEXTFRAME,//���ػط���һ֡
	CMD_REQUEST_LOCAL_CLIENT_PLAYBACK_FF,		//���ػطſ��
	CMD_REQUEST_LOCAL_CLIENT_PLAYBACK_REW,		//���ػطſ���
	CMD_REQUEST_LOCAL_CLIENT_PLAYBACK_CHG_AUDIO,//���ػط��л���Ƶ
	CMD_REQUEST_LOCAL_CLIENT_PLAYBACK_CLICK,	//���ػطŵ���л�����ͨ��
	CMD_REQUEST_LOCAL_CLIENT_PLAYBACK_LIVE,		//���ػط�ʱ��ʾ�ֳ�
	CMD_REQUEST_LOCAL_CLIENT_PLAYBACK_CAM_NAME,	//���ػط�ʱ��ʾͨ������
	CMD_REQUEST_LOCAL_CLIENT_PLAYBACK_HIT,      //�жϱ��ػطŵ�����ǻطŻ��滹���ֳ�����
	CMD_REQUEST_LOCAL_CLIENT_PLAYBACK_CHOSECHN, //���ػط�ѡ��ͨ��	
	CMD_REQUEST_LOCAL_CLIENT_PLAYBACK_CHNINIT,//���ػطſ�ʼʱѡ�������Ϊ0ͨ��
	CMD_REQUEST_LOCAL_CLIENT_PLAYBACK_UNINIT,//���ػطŽ���ʱȡ��ѡ���

	//�������˻ظ����ؿͻ��˻طŵ�����
	CMD_BASENUM_REPLY_LOCAL_CLIENT_PLAYBACK_CTRL = 0x0c0000,
	CMD_REPLY_LOCAL_CLIENT_PLAYBACK_PLAY_SUC,		//���ػطſ����ɹ�
	CMD_REPLY_LOCAL_CLIENT_PLAYBACK_PLAY_FAIL,		//���ػطſ���ʧ��
	CMD_REPLY_LOCAL_CLIENT_PLAYBACK_STOP_SUC,		//���ػط�ֹͣ�ɹ�
	CMD_REPLY_LOCAL_CLIENT_PLAYBACK_PAUSE_SUC,		//���ػط���ͣ�ɹ�
	CMD_REPLY_LOCAL_CLIENT_PLAYBACK_RESUM_SUC,		//���ػطŻָ��ɹ�
	CMD_REPLY_LOCAL_CLIENT_PLAYBACK_CHG_CHNN_SUCC,	//���ػط��л�ͨ���ɹ�
	CMD_REPLY_LOCAL_CLIENT_PLAYBACK_CHG_CHNN_FAIL,	//���ػط��л�ͨ��ʧ��
	CMD_REPLY_LOCAL_CLIENT_PLAYBACK_CHG_TIME_SUCC,	//���ػط��л�ʱ��ɹ�
	CMD_REPLY_LOCAL_CLIENT_PLAYBACK_CHG_TIME_FAIL,	//���ػط��л�ʱ��ʧ��
	CMD_REPLY_LOCAL_CLIENT_PLAYBACK_NEXTFRAME_SUC,	//���ػط���һ֡�ɹ�
	CMD_REPLY_LOCAL_CLIENT_PLAYBACK_FF_SUC,			//���ػطſ���ɹ�
	CMD_REPLY_LOCAL_CLIENT_PLAYBACK_REW_SUC,		//���ػطſ��˳ɹ�
	CMD_REPLY_LOCAL_CLIENT_PLAYBACK_CAM_NAME,       //���ػط�ʱ��ʾͨ������
	CMD_REPLY_LOCAL_CLIENT_PLAYBACK_HIT_SUC,        //���ػطŵ�����ǻطŻ���
	CMD_REPLY_LOCAL_CLIENT_PLAYBACK_HIT_FAIL,       //���ػطŵ�������ֳ�����
	CMD_REPLY_LOCAL_CLIENT_PLAYBACK_CLICKIVE_SUC,   //���ػطŵ�������ֳ���ť�ɹ�
	CMD_REPLY_LOCAL_CLIENT_PLAYBACK_CHOSE_CHNN_SUCC,
	CMD_REPLY_LOCAL_CLIENT_PLAYBACK_CHOSE_CHNN_FAIL,
	CMD_REPLY_LOCAL_CLIENT_PLAYBACK_INIT_CHNN_SUCC,
	CMD_REPLY_LOCAL_CLIENT_PLAYBACK_UNINIT_CHNN_SUCC,
	//���ؿͻ��˱��ݿ���
	CMD_BASENUM_LOCAL_CLIENT_BACKUP_CTRL = 0x0d00,
	CMD_REQUEST_LOCAL_CLIENT_BACKUP_STARTREAD,		//���ر��ݿ�ʼ��ȡ
	CMD_REQUEST_LOCAL_CLIENT_BACKUP_BACKUPING,		//���ر��ݿ�ʼ����
	CMD_REQUEST_LOCAL_CLIENT_BACKUP_STOP,			//���ر���ֹͣ

	//�������˻ظ����ؿͻ��˱��ݵ�����
	CMD_BASENUM_REPLY_LOCAL_CLIENT_BACKUP_CTRL = 0x0d0000,
	CMD_REPLY_LOCAL_CLIENT_BACKUP_STARTREAD_SUC,	//���ر��ݿ�����ȡ�ɹ�
	CMD_REPLY_LOCAL_CLIENT_BACKUP_STARTREAD_FAIL,	//���ر��ݿ�����ȡʧ��
	CMD_REPLY_LOCAL_CLIENT_BACKUP_BACKUPING_SUC,	//���ر��ݳɹ�
	CMD_REPLY_LOCAL_CLIENT_BACKUP_BACKUPING_FAIL,	//���ر���ʧ��
	CMD_REPLY_LOCAL_CLIENT_BACKUP_STOP_SUC,

	//�����豸����
	CMD_BASENUM_NET_DEVICE_LOG_CTRL = 0x0e00,
	CMD_REQUEST_CFG_ENTER_IPCAM,//��������豸����ҳ������
	CMD_REQUEST_CFG_LEAVE_IPCAM,//�����뿪�豸����ҳ������
	CMD_REQUEST_NET_DEVICE_LOG_MAX,		//���������豸��������ֵ
	CMD_REQUEST_NET_DEVICE_LOG_GET,		//���������豸��Ϣ
	CMD_REQUEST_NET_DEVICE_LOG_SET,		//�������������豸��Ϣ, ����Ϊlong + IP_DEVICE_INFO * N
	CMD_REQUEST_NET_DEVICE_SEARCH,		//�������������豸
	CMD_REQUEST_NET_DEVICE_INFO,		//���������ϵ������豸��Ϣ
	CMD_REQUEST_SET_NET_DEVICE_INFO,	//�����������豸��Ϣ
	CMD_REQUEST_SUPPORT_DEVICE,			//��ȡNVR֧�ֵ��豸����

	//IPC�����źŽ�������
	CMD_BASENUM_REPLY_NET_DEVICE_LOG_CTRL = 0x0e0000,
	CMD_REPLY_NET_DEVICE_LOG_MAX,			//�ظ������豸��������ֵ,����Ϊlong
	CMD_REPLY_NET_DEVICE_LOG_GET_SUCC,		//�ظ����������豸��Ϣ�ɹ�������Ϊlong + IP_DEVICE_INFO * N
	CMD_REPLY_NET_DEVICE_LOG_GET_FAIL,		//�ظ����������豸��Ϣʧ��
	CMD_REPLY_NET_DEVICE_LOG_SET_SUCC,		//�ظ����������豸��Ϣ�ɹ�,û�и��Ӳ���
	CMD_REPLY_NET_DEVICE_LOG_SET_FAIL,		//�ظ����������豸��Ϣʧ��
	CMD_REPLY_NET_DEVICE_SEARCH,			//�ظ����������豸,����Ϊlong + DISCOVER_DEVICE_INFO * N
	CMD_REPLY_NET_DEVICE_INFO,				//�ظ������豸��Ϣ
	CMD_REPLY_NET_SUPPORT_DEVICE,			//�ظ�NVR֧�ֵ��豸����,����Ϊlong + NVR_SUPPORT_MANUFACTURER * N + NVR_SUPPORT_PRODUCT * M


	//����������
	CMD_BASENUM_NETPARAMCHECK_CHECK	= 0x0f00,
	CMD_REQUEST_NETPARAMCHECK_PPPOE,		//���PPPOE��������ȷ��
	CMD_REQUEST_NETPARAMCHECK_DDNS,			//���DDNS��������ȷ��
	CMD_REQUEST_NETPARAMCHECK_DHCP,			//��̬����̬����IP��ַ����Ч��
	CMD_REQUEST_NETPARAMCHECK_EMAIL,		//���EMAIL �����ĵ���ȷ��
	CMD_REQUEST_SEND_EMAIL,                 //������email
	CMD_REQUEST_NETPARAMCHECK_WIRELESS,     //���3G��������ȷ��
	CMD_REQUEST_NETPARAMCHECK_UPNP,			//���UPNP�Ƿ��
	CMD_REQUEST_ROUTERIP,					//UPNP��ȡ·����ip
	CMD_REQUEST_REMOVEDDNS,					//IN23ɾ��DDNS
	CMD_END_NETPARAMCHECK,

	CMD_BASENUM_REPLY_NETPARAMCHECK_CHECK	= 0x0f0000,
	CMD_REPLY_NETPARAMCHECK_PPPOE_SUCC,		//���ؼ��PPPOE��������ȷ��
	CMD_REPLY_NETPARAMCHECK_PPPOE_FAIL,		//���ؼ��PPPOE������ʧ��
	CMD_REPLY_NETPARAMCHECK_DDNS_SUCC,		//���ؼ��DDNS��������ȷ��
	CMD_REPLY_NETPARAMCHECK_DDNS_FAIL,		//���ؼ��DDNS������ʧ��
	CMD_REPLY_NETPARAMCHECK_DHCP_SUCC,		//���ؾ�̬����̬���������ַ����Ч��
	CMD_REPLY_NETPARAMCHECK_DHCP_FAIL,		//���ؾ�̬����̬���������ַ��ʧ��
	CMD_REPLY_NETPARAMCHECK_EMAIL_SUCC,		//���ؼ��EMAIL��������ȷ��
	CMD_REPLY_NETPARAMCHECK_EMAIL_FAIL,		//���ؼ��EMAIL������ʧ��
	CMD_REPLY_NETPARAMCHECK_WIRELESS_SUCC,  //���ؼ��3G�������ӳɹ�
	CMD_REPLY_NETPARAMCHECK_WIRELESS_FAIL,  //���ؼ��3G��������ʧ��
	CMD_REPLY_NETPARAMCHECK_UPNP_SUCC,		//���ؼ��upnpӳ��ɹ�
	CMD_REPLY_NETPARAMCHECK_UPNP_FAIL,		//���ؼ��upnpӳ��ʧ��
	CMD_REPLY_GETROUTERIP_SUCC,				//UPNP��ȡ·����IP�ɹ�
	CMD_REPLY_GETROUTERIP_FAIL,				//UPNP��ȡ·����IPʧ��
	CMD_REPLY_REMOVEDDNS_SUCC,				//IN23ɾ��DDNS�ɹ�
	CMD_REPLY_REMOVEDDNS_FAIL,				//IN23ɾ��DDNSʧ��
	CMD_REPLY_IN23_DOMAIN_ALREADY_USE,		//IN23�����Ѿ���ʹ��
	CMD_REPLY_IN23_DOMAIN_CREATE_SUCC,		//IN23���������ɹ�
	CMD_REPLY_IN23_DOMAIN_REPLACE_SUCC,		//IN23���������ɹ�
	CMD_END__REPLY_NETPARAMCHECK,

#ifdef  __MOBILE_DVR__
	//������ؽ�������
	CMD_REQUEST_NET_WIFI_SEARCH = 0x1000,	//��������wifi�ź�
	CMD_REQUEST_NET_WIFI_CONNECT,			//������wifi�����Ƿ�ɹ�

	CMD_REPLY_NET_WIFI_SEARCH = 0x100000,	//�ظ�����wifi�ź�
	CMD_REPLY_NET_WIFI_SEARCH_SUCC,			//�ظ������ɹ�
	CMD_REPLY_NET_WIFI_SEARCH_FALL,			//�ظ�����ʧ��
	CMD_REPLY_NET_WIFI_CONNECT_SUCC,		//�ظ����ӳɹ�
	CMD_REPLY_NER_WIFI_CONNECT_FAIL,		//�ظ�����ʧ��

#endif
	//����iOS���ͱ�����Ϣ������
	CMD_BASENUM_PUSH_NOTIFICATION		= 0x1100,
	CMD_REQUEST_ADD_IPHONE_TOKEN,		//���ý���������Ϣ��iOS���豸����
	CMD_END_PUSH_NOTIFICATION,

	CMD_REQUEST_CHANNEL_CRRENT_VIDEO_ENCODE_INFO = 0x1200,		//NVR���ͻ�������ĳ��ͨ����ǰ֧�ֵı�����Ϣ
	CMD_REPLY_CHANNEL_CRRENT_VIDEO_ENCODE_INFO,					//NVR���豸�˻�Ӧ��ǰͨ���ı�����Ϣ

	CMD_REQUEST_MODIFY_CHANNEL_VIDEO_ENCODE_INFO = 0x1300,		//NVR���ͻ�����������ĳ��ͨ���ı�����Ϣ
	CMD_REPLY_MODIFY_CHANNEL_VIDEO_ENCODE_INFO_SUCC,			//NVR���豸�����óɹ�
	CMD_REPLY_MODIFY_CHANNEL_VIDEO_ENCODE_INFO_FAIL,			//NVR���豸������ʧ��

	CMD_REQUEST_NET_CHANNEL_SUPPORT_VIDEO_ENCODE_INFO= 0x1400,	//IE/CMS�ȿͻ�����������ͨ����ǰ֧�ֵı�����Ϣ
	CMD_REPLY_NET_CHANNEL_SUPPORT_VIDEO_ENCODE_INFO,			//IE/CMS�ȶ˻�Ӧ��ǰ����ͨ���ı�����Ϣ

	CMD_REQUEST_NET_CHANNEL_VIDEO_DEF_BITSTREAM,				//IE/CMS�ȿͻ�����������ͨ����ǰ�ֱ���Ĭ�ϵ�����ֵ
	CMD_REPLY_NET_CHANNEL_VIDEO_DEF_BITSTREAM,					//IE/CMS�ȶ˻�Ӧ����ͨ����ǰ�ֱ���Ĭ�ϵ�����ֵ

	CMD_BASENUM_REPLY_PUSH_NOTIFICATION	= 0x110000,
	CMD_REPLY_ADD_IPHONE_TOKEN,
	CMD_END_REPLY_NOTIFICATION,

	//�������˷���״̬��Ϣ
	CMD_BASENUM_REPLY_STATUS	= 0x9000000,
	CMD_REPLY_VIDEO_LOSS,		//������Ƶ��ʧ״̬
	CMD_REPLY_MOTION,			//�����ƶ����״̬
	CMD_REPLY_SENSOR,			//���ͱ�������״̬
	CMD_REPLY_REC_STATUS,		//��������ͨ����¼��״̬
	CMD_REPLY_SHUT_DOWN,		//���ͱ�ʾϵͳ���ػ�
	CMD_REPLY_REBOOT,			//���ͱ�ʾϵͳ������
	CMD_REPLY_IP_CHANGE,		//���ͱ�ʾ�����ַ�����ģ������µ������ַ
	CMD_REPLY_CHNN_NAME,		//����ĳ��ͨ��������
	CMD_END_REPLY_STATUS,

	//�������˷�����Ƶ/��Ƶ����
	CMD_BASENUM_REPLY_STREAM	= 0xa000000,
	CMD_REPLY_DATA_STREAM,		//��Ƶ/��Ƶ����
	CMD_END_REPLY_STREAM,

	//���¼�������
	CMD_REQUEST_EFFECT_NETWORK	= 0xb000000,

	//Ҫ��ͻ��������Ͽ�������
	CMD_REPLY_DISCONNECT		= 0xfffffffd,

	//û��Ȩ�޵�ͳһ�ظ�
	CMD_REPLY_NO_AUTH			= 0xfffffffe,

	//���ش�ֵ��ʾ��������֧�ֶ�Ӧ�Ĺ���
	CMD_REPLY_NO_SUPPORT		= 0xffffffff,

}NET_PROTOCOL;

typedef enum _net_ctrl_fail
{
	NETERR_SUCCESS					= 0,	//�ɹ������еط��ĳɹ�������ʹ�ô�ֵ
	NETERR_LOGIN_FAIL_VERSION_ERR	= 1,	//��½ʱ�汾����
	NETERR_LOGIN_FAIL_USERPWD_ERR	= 2,	//��½ʱ�û������������
	NETERR_LOGIN_FAIL_OVERFLOW		= 3,	//��½���糬�أ������½����һ��������ֹͣlisten,�˴�������ò���
	NETERR_LOGIN_FAIL_REFUSE		= 4,	//��½�ܾ��������Ǳ����˵�IP��ԭ��
	NETERR_OPERATE_FAIL_NOAUTHORITY	= 5,	//������û��Ȩ�ޣ������á���̨���ط�
	NETERR_OPERATE_FAIL_OPERATING	= 6,	//ֻ��һ���˲���ʱ�����Ѿ������ڲ��������á���̨���ơ��ط�
	NETERR_NO_DISK					= 7,	//�޴��̣��ڸ�ʽ������ѯ�¼��Ⱥʹ�����صĲ���ʱ��Ϊû�д��̶�ʧ��ʱ���ش�ֵ
	NETERR_NO_EVENT					= 8,	//���¼�
	NETERR_FORMATERR_RECORDING		= 9,	//���̸�ʽ����������¼��
	NETERR_NOTSUPPORT				= 10,	//��֧�ֵĲ���
	NETERR_MUST_ENTER_CFG			= 11,	//��û�н������õ�����½�����õ����������õĲ���ʱ���صĴ�����
	NETERR_DISK_IO_ERR				= 12,	//���̶�д��IO����
	NETERR_MODIFY_PASS_ERR			= 13,	//�޸�����ʱ,���������
	NETERR_MODIFY_PASS_UFUSER		= 14,	//�޸�����ʱ,�ڵ�¼�û���������û���ҵ���Ӧ���û�
	NETERR_MODIFY_PASS_ERR_SAVE		= 15,	//�޸�����ʱ,�������뵽�����ļ�ʧ��
	NETERR_FORMAT_DISK_FORMATING    = 16,   //��ʽ��Ӳ��ʧ�ܣ��������ڸ�ʽ��
	NETERR_FORMAT_DISK_BACKUP_PLAY  = 17,   //��ʽ��Ӳ��ʧ�ܣ��������ڱ��ر��ݻ������ڱ��ػط�
	NETERR_UPDATE_FAIL_NO_DISK      = 18,   //Զ������ʧ�ܣ�ԭ��û��Ӳ��
	NETERR_UPDATE_FAIL_FORMAT_DISK  = 19,   //Զ������ʧ�ܣ�ԭ�����ڸ�ʽӲ��Ӳ��
	NETERR_UPDATE_FAIL_UPDATING     = 20,   //Զ������ʧ�ܣ�ԭ����������
	NETERR_UPDATE_FAIL_INVAID_FILE  = 21,   //Զ������ʧ�ܣ���Ч���ļ�
	NETERR_FAIL_NO_AUTHORITY        = 22,   //û��Ȩ��

	
	NETERR_NETPARAMCHECK_OK			= 500,	//��Ϣͨ����������֤
	NETERR_NETPARAMCHECK_ENAMEPWD,			//�û��������������
	NETERR_NETPARAMCHECK_EHOSTDOMAIN,		//dydnds.com���û����������������������
	NETERR_NETPARAMCHECK_ETIMEOUT,			//��ʱ��meibuʹ��UDP���ӣ���ÿ�θ���ʱ��һ���з���
	NETERR_NETPARAMCHECK_ECONNECT,			//����δ����,δ������
	NETERR_NETPARMACHECK_ENETUNREACHD,		//���粻�ɴ�
	NETERR_NETPARMACHECK_EHOSTDOMAIN_EMPTY,	//��������Ϊ��
	NETERR_NETPARAMCHECK_EUNKNOWN,			//δ֪����	
	NETERR_NETPARAMCHECK_ABUSE,				//dyndns.com��ע���������block
	NETERR_NETPARAMCHECK_NOHOST,			//dyndns.com���Ҳ����������
	NETERR_NETPARAMCHECK_NOTFQDN,			//dyndns.com���������������Ϲ���
	NETERR_NETPARAMCHECK_NUMHOST,			//dyndns.com��̫���������update
	NETERR_NETPARAMCHECK_REGISTER_SUCC,		//��dynamic.visionica.comע��ɹ�
	NETERR_NETPARAMCHECK_REGISTER_FAIL,		//��dynamic.visionica.comע��ʧ��
	NETERR_NETPARAMCHECK_DOMAIN_ALREADY_USE = 515, //IN23���Զ�ע�ᣬ�����Ѿ���ʹ�õ��µ�ע��ʧ��
	NETERR_NETPARAMCHECK_DOMAIN_CREATE_OK,			//IN23���������ɹ�
	NETERR_NETPARAMCHECK_DOMAIN_REPLACE_OK,			//IN23���������ɹ�

	NETERR_NETPARAMCHECK_EIPCONFLICT= 600,	//��̬���õ�IP��ͻ
	NETERR_NETPARAMCHECK_EIPORSUBMASK,		//ip��ַ������,���������������
	NETERR_NETPARAMCHECK_ENONICDEVICE,		//û��ָ����NIC�豸
	NETERR_NETPARAMCHECK_EUSERNAMEEMPTY,	//�û���Ϊ��
	NETERR_NETPARAMCHECK_ENODEVICE,			//�����豸δ����
	NETERR_NETPARMACHECK_ENOCARD,			//û��UIM��
	NETERR_NETPARMACHECK_ENOSIGNAL,			//û�������ź�
	NETERR_NETPARAMCHECK_EPARAM,			//���Ų�������

	NETERR_STREAM_OK				= 1000,	//������������
	NETERR_STREAM_NO_ATH,					//û����ӦȨ��
	NETERR_STREAM_NO_CAPABILITY,			//���ܲ���

	NETERR_EXPORT_CONFIG_OK				= 1100,
	NETERR_EXPORT_CONFIG_ERR,							//����
	NETERR_EXPORT_CONFIG_DIRERR,						//Ŀ¼�����ڻ���û�з���Ȩ��
	NETERR_EXPORT_CONFIG_FILEERR,						//û��ѡ���ļ�
	NETERR_EXPORT_CONFIG_DVRIDERR,						//��ƷID��ƥ��
	NETERR_EXPORT_CONFIG_FIRMWAREERR,					//�̼��汾��ƥ��
	NETERR_EXPORT_CONFIG_OVERONEUSER,					//ͬʱ���������ϵ��û��ڵ���͵���
	NETERR_EXPORT_CONFIG_VERSION,						//�����ļ��м�¼�İ汾�ͱ��ذ汾��һ��

	NETERR_FAIL	= 0xffffffff,				//��ͨʧ�ܷ���ֵ���ڲ����ԭ�������·��ش�ֵ
}NET_CTRL_FAIL;


const unsigned long		NET_PROTOCOL_VER				= 0x00000004;
#if defined(__STREAM_SERVER_RTP__)
const unsigned char		NET_PLAYBACK_DATA_FRAME_NUM		= 7;
#else
const unsigned char		NET_PLAYBACK_DATA_FRAME_NUM		= 25;
#endif

//////////////////////////////////////////////////////////////////////////
typedef enum _enum_connect_type{
	CONNECTTYPE_LOCAL,
	CONNECTTYPE_CTRL_CENTER,
	CONNECTTYPE_MOBILE_PHONE,
	CONNECTTYPE_IE,
	CONNECTTYPE_VOD,	//video-on-demand��Ƶ�㲥
	CONNECTTYPE_ECMS,   //֧����������
	CONNECTTYPE_MOBILE_PHONE_HIGH,		//�����ֻ������Ǹ����������豸����������������IE��һ���������������豸D1��������D1�����ϴ�3֡��D1�����º�IE����ʽһ��
	CONNECTTYPE_ACTIVE_CONNECT = 100,	//��������
}ENUM_CONNECT_TYPE;

//�û���¼ʱ�õĽṹ��
typedef struct _net_login_info
{
	unsigned long	connectType;			//��������,ENUM_CONNECT_TYPE
	unsigned long   IP;					//�ͻ����������ӵ�IP��ַ

	char			username[36];			//�û�����
	char			password[36];			//�û�����
	char			computerName[28];		//���������

	unsigned char   MAC[6];					//����Mac��ַ
	unsigned char	resv[2];				//�����ֽڣ�������
	unsigned long	netprotocolver;			//�ͻ���Ŀǰʹ�õ�����Э��汾
}NET_LOGIN_INFO, *PNET_LOGIN_INFO;

//�û���¼�ɹ�ʱ���ظ��õĽṹ��
typedef struct _net_login_success_reply
{
	unsigned long	authority;		//�û�����Ȩ�޻����û�����
	ULONGLONG		authLiveCH;		//�ֶ�¼�����
	ULONGLONG		authRecordCH;		//�ֶ�¼�����
	ULONGLONG		authPlaybackCH;		//�����ͻط�
	ULONGLONG		authBackupCH;		//����
	ULONGLONG		authPTZCtrlCH;		//�������
	ULONGLONG		authRemoteViewCH;	//Զ��Ԥ��
	PRODUCT_INFO	productInfo;	//��Ʒ��Ϣ
}NET_LOGIN_SUCCESS_REPLY;
/**************************************************��¼����Э��***************************************************
			�ͻ���										��������

1��	������������										�ظ�Э��汾�ţ�NET_PROTOCOL_INFO�����߾ܾ�����

2��	ȷ���ظ��źţ�Э��汾�Ƿ���ݣ����󣬷���			��֤��¼�û��������뼰������Ϣ�ظ��豸��Ϣ����
	��¼��Ϣ���� NET_LOGIN_INFO ����.					�ܾ���¼���� NET_LOGIN_SUCCESS_REPLY ����.			

3��	ȷ����¼�ɹ�����ʧ�ܡ������¼�ɹ����Իظ���
	�豸��Ϣ��һЩ��Ҫ�Ĵ���

	�ͻ��˷������ݸ�ʽ��	�����ֶ� - ��Ϣ��NET_LOGIN_INFO ��

	�������˻ظ����ݸ�ʽ��	
			a)�ظ���������:		�����ֶ� - ��Ϣ�ֶΣ�NET_PROTOCOL_INFO��
			b)�ظ���֤�ɹ�:		�����ֶ� - ��Ϣ�ֶΣ�NET_LOGIN_SUCCESS_REPLY��
			c)�ظ���֤ʧ��:		�����ֶ� - ʧ����Ϣ��
*****************************************************************************************************************/

typedef struct _net_livestream_ctrl_info
{
	unsigned long	streamID;		//��ID

	ULONGLONG		fastVideoCH;	//��λ��ʾ��ͨ����״̬_������
	ULONGLONG		secondVideoCH;	//��λ��ʾ��ͨ����״̬_������
	ULONGLONG		threeVideoCH;	//��λ��ʾ��ͨ����״̬_�ֻ���
	ULONGLONG		audioCH;		//��λ��ʾ��ͨ����״̬_��Ƶ��
}NET_LIVESTREAM_CTRL_INFO;

typedef struct _net_jpeg_stream_info
{
	unsigned long	streamID;
	ULONGLONG		jpegCH;
}NET_JPEG_STREAM_INFO;

typedef struct _net_display_mode_info
{
	ULONGLONG  live_mask;     //��λ��ʾ��ͨ�����ֳ���ʾ
	ULONGLONG  pb_mask;       //��λ��ʾ��ͨ���Ļط���ʾ
}NET_DISPLAY_MODE_INFO;

typedef enum NET_DISPLAY_INFO_TYPE
{
	//����CMD_REQUEST_CHANGE_DISPLAY_MODE, �ظ�CMD_REPLY_CTRL_SUCC
	NET_DISPLAY_CHANGE_DATA_PACK	= 0x0000,	//
	NET_DISPLAY_CHANGE_XY_PT,
	NET_DISPLAY_FREEZE,
	NET_DISPLAY_UNFREEZE,
	NET_DISPLAY_EXCHANGE_TWO_PT,
	NET_DISPLAY_SET_MINIWND_POS,
	NET_DISPLAY_FILTER,

	//����CMD_REQUEST_GET_DISPLAY_INFO, �ظ�CMD_REPLY_CTRL_SUCC
	NET_DISPLAY_GET_INFO			= 0x0100,	//
	NET_DISPLAY_GET_INFO_XY,

	NET_DISPLAY_REPLY_ALL			= 0x0200,
	NET_DISPLAY_REPLY_HITS,
}NET_DISPLAY_INFO_TYPE;

enum DISPLAY_SIGNAL_TYPE
{
	DISPLAY_SIGNAL_NULL = 0,
	DISPLAY_SIGNAL_ANALOG,
	DISPLAY_SIGNAL_DIGIT,
	DISPLAY_SIGNAL_NET,
};

enum DISPLAY_LAYER_TYPE
{
	DISPLAY_LAYER_NULL = 0,
	DISPLAY_LAYER_ANALOG,
	DISPLAY_LAYER_DIGIT,
	DISPLAY_LAYER_NET,
	DISPLAY_LAYER_MINI,
};

typedef struct _net_display_info
{
	unsigned short	type;			//�������� NET_DISPLAY_INFO_TYPE
	unsigned short	rec;			//�����Ƿ񱻼�¼[�û��������طš���̨����¼; ���汨����������̨������ɫ�ʲ�����¼]
	unsigned short	time_out;		//��ʱ,0��ʾʹ��Ĭ��ֵ, -1��ʾ�Զ��᷽ʽ����
	unsigned short	data_len;		//��Ч���ݳ���[data]

	unsigned short	x;				//x����
	unsigned short	y;				//y����

	unsigned long	split_mode[8];	//ÿ����ķָ�ģʽ
	unsigned long	data[64];		//DISPLAY_PACK[��<<24] [�ź�����<<16] [������<<8] [ͨ��(unsigned char)]
}NET_DISPLAY_INFO;

//�����ֳ���Ƶ�ṹ��
typedef enum NET_LIVEAUDIO_TYPE
{
	NET_LIVE_AUDIO_OPEN,
	NET_LIVE_AUDIO_MUTE,
	NET_LIVE_AUDIO_AUTO,
	NET_LIVE_AUDIO_RECOVER, //�ڽ���ط�ʱ����ر��ֳ��������ô�TYPE���Իָ���ԭ����״̬��
}NET_LIVEAUDIO_TYPE;
typedef struct _net_liveaudio_ctrl_info
{
	unsigned long type;             //��������
	unsigned long audiochnnel;      //��Ƶͨ����
}NET_LIVEAUDIO_CTRL_INFO;

/**********************************************���ƣ�ϵͳ��������Э��********************************************
			�ͻ���										��������

1��					������������						���ܵ�����󣬽��д����ٷ��ش�����

ע��a) CMD_REQUEST_STREAM_START:	������Ҫ��������Ϣ��Ϣ���� NET_LIVESTREAM_CTRL_INFO������ʾ�����ͨ����
	b) CMD_REQUEST_MANUAL_REC:		�������Ҫ�ظ�ʲô����Ϊ�豸��ظ�¼��״̬��
	c) CMD_REQUEST_KEYFRAME:		�������Ҫ�ظ�ʲô.
	d) CMD_REQUEST_SHUTDOWN ��CMD_REQUEST_REBOOT��������ܲ�������ظ���Ȩ�ޡ�
	e) CMD_REQUEST_CHANGE_TIME:		�����ɹ�����ʧ��
	f) CMD_REQUEST_CHANGE_DISPLAY_MODE :�л��ֳ����ط���ʾģʽ���絥���棬4���档�ظ�CMD_REPLY_CHANAGE_DISPLAY_SUCC��
	g) CMD_REQUEST_EVICT_SOMEONE:����ĳ���ͻ���

�ͻ��˷������ݸ�ʽ:	
	a) ��������:		�����ֶ� - ͨ����Ϣ(NET_LIVESTREAM_CTRL_INFO)
	b) �����ֶ�¼��:	�����ֶ� - ͨ����Ϣ(ULONGLONG)
	c) ����ؼ�֡:		�����ֶ� - ͨ����Ϣ(ULONGLONG)
	d) �����޸�ʱ��:	�����ֶ� - ʱ��(ULONGLONG)
    e) �����л���ʾģʽ �����ֶ� - �ֳ��ͻط���ʾ��Ϣ��NET_DISPLAY_INFO        ��
	f) 
	g) ��������ĳ�ˣ�	�����ֶ� - �ͻ��������ַ��unsigned long)
�������˻ظ����ݸ�ʽ:	
	a) ����ʱ��ɹ�:	�����ֶ� - CMD_REPLY_CHANAGE_TIME_SUCC
	b) ����ʱ��ʧ��:	�����ֶ� - CMD_REPLY_CHANAGE_TIME_FAIL
*****************************************************************************************************************/

typedef struct _net_chnn_color
{
	unsigned short	chnn;	//ͨ����
	unsigned short	hide;	//ͨ������
	unsigned long	color;	//ͨ����ɫֵ((brightness<<24) + (contrast<<16) + (saturation<<8) + hue);
}NET_CHNN_COLOR;

typedef struct _net_chnn_mirror
{
	unsigned short chnn;
	unsigned char  mirror; 
}NET_CHNN_MIRROR;

typedef struct _net_chnn_flip
{
	unsigned short chnn;
	unsigned char  flip; 
}NET_CHNN_FLIP;

typedef struct _net_chnn_rotato
{
	unsigned short chnn;
	unsigned long  rotato; 
}NET_CHNN_ROTATO;

typedef struct _net_chnn_lensDistortion
{
	unsigned short chnn;
	LENS_DISTORTION_CONFIG lensDistortion; 
}NET_CHNN_LENS_DISTORTION;

typedef struct _net_chnn_name
{
	unsigned short	chnn;
	unsigned short	nameLen;
	char			name [36];
}NET_CHNN_NAME;

typedef struct _net_camera_cover_info
{
	unsigned long chnn;				//ͨ��
	CAMERA_COVER_INFO coverInfo;
}NET_CAMERA_COVER_INFO;

/*************************************************ͨ����������Э��***********************************************
			�ͻ���										��������

1��			������������								���ܵ�����󣬽��д����ٷ��ش�����

ע��a��CMD_REQUEST_KEYFRAME: ���ظ���
	b��CMD_REQUEST_COLOR_GET���ظ���ɫֵ��NET_CHNN_COLOR����
	d��CMD_REQUEST_COLOR_SET��������ɫֵ��NET_CHNN_COLOR����
	e��CMD_REQUEST_COLOR_DEF���ظ���ɫֵ��NET_CHNN_COLOR����

�ͻ��˷������ݸ�ʽ��	�����ֶ� - ��Ϣ��NET_CHNN_COLOR��
						�����ֶ� - ��

�������˻ظ����ݸ�ʽ��	�����ֶ� - ��Ϣ��NET_CHNN_COLOR��
						�����ֶ� - ��Ϣ��NET_CHNN_NAME��
*****************************************************************************************************************/

typedef struct _net_cfg_info
{
	unsigned long	len;			//�������ݵĳ���
	unsigned long	cfgType;		//��������
}NET_CFG_INFO;
/**********************************************��������Э��*****************************************************
		�ͻ���										��������

1��		��������CMD_REQUEST_CFG_ENTER��			���ܵ����󣬴���󷵻�������Ϣ���߾ܾ�����

2��		�����������ݣ�CMD_REQUEST_CFG_GET��			����������ݣ�������Ҫ�����������ݡ�

3��		�����������ݣ�CMD_REQUEST_CFG_SET��			����������ݣ����������ݣ��ظ��ɹ�����ʧ�ܡ�

3��		�뿪���ã�CMD_REQUEST_CFG_LEAVE��			�����˳����á�

ע��a����Բ�ͬ���������󣬻ظ���ͬ��������Ϣ��

�ͻ��˷������ݸ�ʽ:	
	a) �����ֶ� - ���ݳ��� - �������� - Ҫ���������ݵ���Ϣ(�г��������ID)
	b) �����ֶ� - ���ݳ��� - �������� - ��������

�������˻ظ����ݸ�ʽ��	
	a) �����ֶ� - ���ݳ��� - �������� - ��������
	b) �����ֶ� - ʧ����Ϣ��
*****************************************************************************************************************/
enum NET_DATA_SEARCH_TYPE
{
	NET_DATA_SEARCH_PLAYBACK,
	NET_DATA_SEARCH_BACKUP,
};

typedef enum _data_search_type
{
	DATA_SEARCH_TYPE_YEAR       = 0x00,     //
	DATA_SEARCH_TYPE_TIME       = 0x01,		//��ʱ������
	DATA_SEARCH_TYPE_EVENT      = 0x02,		//�����¼�
	DATA_SEARCH_TYPE_FILE       = 0x03,		//�����ļ�
	DATA_SEARCH_TYPE_PIC        = 0x04,     //����ͼƬ
}DATA_SEARCH_TYPE;

typedef struct _net_data_search
{
	unsigned long		searchType;	//�������ͣ� DATA_SEARCH_TYPE
	unsigned long		streamID;	//Ҫ���͵�stream ID

	ULONGLONG       	videoCH;	//��Ƶͨ������
	ULONGLONG           audioCH;    //��Ƶͨ������

	unsigned long		startTime;	//��ʼʱ��
	unsigned long		endTime;	//����ʱ��

	unsigned long		dataType;	 //�������� �����ڻطŻ��Ǳ��ݣ�NET_DATA_SEARCH_TYPE

	unsigned long		RecordTypeMASK;//¼���������

	unsigned long       dataOwnerby;  //Ҫ�������������ڱ��豸��Ӳ���ϣ����������豸��Ӳ�̵��ϡ�
}NET_DATA_SEARCH;

typedef struct _net_hx_data_search
{
	unsigned long		chnnNum;	//����Ƶͨ��
	unsigned long		startTime;	//��ʼʱ��
	unsigned long		endTime;	//����ʱ��

	unsigned long       dataOwnerby;  //Ҫ�������������ڱ��豸��Ӳ���ϣ����������豸��Ӳ�̵��ϡ�
}NET_HX_DATA_SEARCH;


typedef struct _net_data_info
{
	unsigned long	dataType;		//��������
	unsigned long	end;			//1 ��ʾ���һ��
	unsigned long   index;          //����
	unsigned long	dataNum;		//���ݸ���
}NET_DATA_INFO;

typedef struct _net_section_info
{
	unsigned long   chnnnel;
	unsigned long   starTime;
	unsigned long   endTime;
}NET_SECTION_INFO;


typedef enum _net_play_speed
{
	NET_PLAY_SPEED_1_32X = 1,
	NET_PLAY_SPEED_1_16X,
	NET_PLAY_SPEED_1_8X,	//1/8
	NET_PLAY_SPEED_1_4X,	//1/4
	NET_PLAY_SPEED_1_2X,	//1/2
	NET_PLAY_SPEED_1X,		//1
	NET_PLAY_SPEED_2X,
	NET_PLAY_SPEED_4X,
	NET_PLAY_SPEED_8X,
	NET_PLAY_SPEED_16X,
	NET_PLAY_SPEED_32X,
}NET_PLAY_SPEED;
typedef struct _playback_speed_info
{
	unsigned long chnn;
	unsigned long speed;
}PLAYBACK_SPEED_INFO;

typedef struct _playback_changetime_info
{
	unsigned long chnn;
	unsigned long time;
	bool bsync;//�Ƿ���ͬ��ģʽ
	bool bpause;//�Ƿ�����ͣ״̬������ͣ״̬��Ҫˢ�»طš�||��ͼ��
}PLAYBACK_CHANGETIME_INFO;

typedef struct _nfcg_lens_distortion_config__
{
	unsigned char  ucLensDistortion;        // 0 no, 1 yes
	unsigned char  ucLensDistortionValue;   // 0 - 255
	char            noUse[2];
    unsigned short chnn;
}LENS_DISTORTION;


/*************************************************���ݿ�������Э��***********************************************
			�ͻ���										��������

++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
1��	������������										�ظ��������ݣ����߾ܾ������������糬�أ�

2��	���ܵ��ظ����ݣ�����ˢ�´���

++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
1��	���Ͷ�ȡ��������������NET_DATA_SEARCH��			�ظ���Ƶ/��Ƶ���ݻ���ʧ����

2��	������Ƶ/��Ƶ���ݣ��طŻ���߱���

3��	������ͣ����										��ͣ��������

4��	���Ϳ������										�����ٶȶ�ȡ�ؼ�֡������

5��	���Ϳ�������										�����ٶȶ�ȡ�ؼ�֡������

6��	����ֹͣ����										ֹͣ��ȡ�������ٷ��Ͷ��У����ٶ�ȡ�߳�
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
1��	����������������ļ���Ϣ							�����ļ��б�������������

2��	���ͽ�����������ļ���Ϣ							�����ļ��б����н�������

3��	����ɾ����������ļ���Ϣ							�����ļ��б�����ɾ������

++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

ע��a����Բ�ͬ���������󣬻ظ���ͬ����Ϣ��
	b��NET_DATA_INFO�ṹ�����ڷ���ǰ�ͽ��ܺ�ʹ�á�
	c���ظ���Ƶ/��Ƶ����ʱ�ĸ�ʽ�����������˷�����Ƶ/��Ƶ���ݡ�����Э�顣

�ͻ��˷������ݸ�ʽ��	
	a�����������������ֶ� - ����������NET_DATA_SEARCH��������������Ҫʹ����������
	b����ʼ��ȡ�������ֶ� - ��ȡ������NET_DATA_SEARCH�������ڿ�ʼ��ȡ���ݣ���ʱȡstreamID
	c�����������������ֶ� - ��������ȡ��������Ҫֹͣ����ID��
	d��ɾ�������������ֶ� - �ļ����� - ����

�������˻ظ����ݸ�ʽ��	
	a�������ɹ��������ֶ� - ������Ϣ��NET_DATA_INFO��- ���ݣ������ظ�
	b������ʧ�ܣ������ֶ� - ʧ����Ϣ��
*****************************************************************************************************************/
typedef struct _net_event_search
{
	unsigned long		type;		//EVENT_TYPE���

	unsigned long		startTime;	//��ʼʱ��
	unsigned long		endTime;	//����ʱ��

	ULONGLONG	        chnnBits;	//ͨ�����
}NET_EVENT_SEARCH;

typedef struct _net_event_info
{
	unsigned long	end;			//��ʾ���һ����
	unsigned long	eventNum;		//��ʾԪ����Ŀ��EVENT_LOG X n
}NET_EVENT_INFO;

typedef struct _net_log_search
{
	unsigned long		content;	//LOG_TYPE���(ע��Ϊÿһ������ϣ�

	unsigned long		startTime;	//��ʼʱ��
	unsigned long		endTime;	//����ʱ��
}NET_LOG_SEARCH;

typedef struct _net_log_info
{
	unsigned long	end;			//��ʾ���һ����
	unsigned long	logNum;			//��ʾԪ����Ŀ��OPERATOR_LOG X n
}NET_LOG_INFO;
/**********************************************��ѯ�¼�����Э��***************************************************
				�ͻ���										��������

1��		��������CMD_REQUEST_EVENT_INFO��					���ܵ����󣬴���󷵻ؽ��

2��		��������CMD_REQUEST_LOG_INFO��					���ܵ����󣬴���󷵻ؽ��

3��		��������CMD_REQUEST_CLIENT_INFO��					���ܵ����󣬴���󷵻ؽ��

4��		��������CMD_REQUEST_FIRST_ENCODE_INFO��			���ܵ����󣬴���󷵻ؽ��

5��		��������CMD_REQUEST_SECOND_ENCODE_INFO��			���ܵ����󣬴���󷵻ؽ��

6��		��������CMD_REQUEST_THREE_ENCODE_INFO��			���ܵ����󣬴���󷵻ؽ��

7��		��������CMD_REQUEST_NETWORK_INFO��				���ܵ����󣬴���󷵻ؽ��

�ͻ��˷������ݸ�ʽ��	
	a���¼�����������		�����ֶ� - ����������NET_EVENT_SEARCH��
	b����־����������		�����ֶ� - ����������NET_LOG_SEARCH��
	c���ͻ�����Ϣ������		�����ֶ�
	d������������״̬������	�����ֶ�
	e������������״̬������	�����ֶ�
	f���ֻ�������״̬������	�����ֶ�
	g������״̬������		�����ֶ�

�������˻ظ����ݸ�ʽ��	
	a����ѯ�¼��ɹ���		�����ֶ� - �¼���Ϣ��NET_EVENT_INFO��- ���� X N
	b����ѯ��־�ɹ���		�����ֶ� - �¼���Ϣ��NET_LOG_INFO��- ���� X N
	c����ѯ�ͻ��˳ɹ���		�����ֶ� - Ԫ����Ŀ��N�����ֽڣ� - �¼���Ϣ��NET_CLIENT_INFO_EX��- ���� X N
	d����ѯ�������ɹ���		�����ֶ� - Ԫ����Ŀ��N�����ֽڣ� - �¼���Ϣ��STREAM_INFO��- ���� X N
	e����ѯ�������ɹ���		�����ֶ� - Ԫ����Ŀ��N�����ֽڣ� - �¼���Ϣ��STREAM_INFO��- ���� X N
	f����ѯ�ֻ����ɹ���		�����ֶ� - Ԫ����Ŀ��N�����ֽڣ� - �¼���Ϣ��STREAM_INFO��- ���� X N
	g����ѯ����״̬�ɹ���	�����ֶ� - ���ݣ�NETWORK_STATUS_INFO��
	h����ѯʧ�ܣ�			�����ֶ� - ʧ����Ϣ��
*****************************************************************************************************************/

/**********************************************��̨��������Э��***************************************************
�ͻ���										��������

1��		��������CMD_REQUEST_PTZ_START��				���ܵ����󣬴���󷵻ض�Ӧ��������Ϣ���߾ܾ�����

2��		���ƣ�CMD_REQUEST_PTZ_CTRL��					���ܿ���������ݲ�ͬ���������ͽ��д���

3��		����Ԥ�õ���Ϣ��CMD_REQUEST_PTZ_PRESET_INFO��	���ܿ�����������ظ�

4��		����Ѳ������Ϣ��CMD_REQUEST_PTZ_CRUISE_INFO��	���ܿ�����������ظ�

5��		����켣��Ϣ��CMD_REQUEST_PTZ_TRACK_INFO��		���ܿ�����������ظ�

6��		������̨���ƣ�CMD_REQUEST_PTZ_STOP��		�����������

ע��a��CMD_REQUEST_PTZ_START����Щ�豸����û�н�����̨�豸����������ԭ��ܾ����ƣ���Ҫ����ʧ���롣
	b��CMD_REQUEST_PTZ_CTRL�������ֶ�������̨���˶���
	d��CMD_REQUEST_PTZ_STOP����������̨����ʱ�����͸����������Ա���һЩ��Ҫ�Ĵ���

�ͻ��˷������ݸ�ʽ��	
	a��CMD_REQUEST_PTZ_CTRL - ���Ʋ�����PTZ_CTRL��
	b��CMD_REQUEST_PTZ_CTRL - ���Ʋ�����PTZ_CTRL��PTZ_CMD_PRESET_SET��PTZ_CMD_CRUISE_SET��- ���ݣ���ע�⣩
	c��CMD_REQUEST_PTZ_PRESET_INFO - ͨ���ţ����ֽڣ�
	d��CMD_REQUEST_PTZ_CRUISE_INFO - ͨ���ţ����ֽڣ�
	e��CMD_REQUEST_PTZ_TRACK_INFO - ͨ���ţ����ֽڣ�

ע��Ѳ�������ݵĸ�ʽ��	PTZ_CURISE - PTZ_CURISE_POINT X N

�������˻ظ����ݸ�ʽ��	
	a�������ֶ� - ʧ����Ϣ��
	b��CMD_REPLY_PTZ_PRESET_INFO - Ԥ�õ���Ŀ�����ֽڣ�- PTZ_PRESET X N
	c��CMD_REPLY_PTZ_CURISE_INFO - Ѳ������Ŀ�����ֽڣ�- ��PTZ_CURISE - ��PTZ_CURISE_POINT X N���� X N
	d��CMD_REPLY_PTZ_TRACK_INFO - �켣����Ŀ�����ֽڣ�
*****************************************************************************************************************/


/**********************************************���̹�������Э��***************************************************
		�ͻ���										��������

1��		��������CMD_REQUEST_DISK_START��			���ܵ����󣬴���󷵻ض�Ӧ�ش�����Ϣ���߾ܾ�����

2��		���ƣ�CMD_REQUEST_DISK_FORMAT��				��ʽ�����̣������ڿͻ��˼����¼��״̬��

3��		�������̹���CMD_REQUEST_DISK_STOP��		�����������

ע��a��CMD_REQUEST_DISK_START����Щ�豸����û�н�����̣���������ԭ��ܾ����ƣ���Ҫ����ʧ���롣
	b��CMD_REQUEST_DISK_FORMAT���ͻ�����Ҫ���¼��״̬��ֻ�д��ڷ�¼��״̬ʱ�Ÿ�ʽ����
	c��CMD_REQUEST_DISK_STOP�����������̹���ʱ�����͸����������Ա���һЩ��Ҫ�Ĵ���
	d����ʽ�ɹ�����Ҫ����CMD_PEPLY_FORMAT_SUCC���������ո���ʽ�����̵���Ϣ��

�ͻ��˷������ݸ�ʽ��	
	a�����̹���	�����ֶ� - ��
	b�����̸�ʽ����	�����ֶ� - �������� - �������
	c���˳�����	�����ֶ� - ��

�������˻ظ����ݸ�ʽ��	
	a���ظ�������Ϣ��		�����ֶ� - ������Ŀ��N�����ֽڣ� - ��Ϣ���ݣ�DVR_DISK_INFO * N��
	b���ظ����̸�ʽ��ʧ�ܣ�	�����ֶΣ�CMD_REPLY_FORMAT_SUCC��
*****************************************************************************************************************/
typedef struct _net_motion_info
{
	unsigned long chnn;         //ͨ����
	MOTION_DETECT_INFO detectInfo;	//���������Ϣ��
}NET_MOTION_INFO;


/***************************************�ƶ��������Э��*********************************************************
�ͻ���                                          ��������

1���������� (CMD_REQUEST_MOTION_AREA_GET)       ���յ�����󣬷���NETDEVICE_MOTION_INFO��������Ϣ������ʧ��
2���������� (CMD_REQUEST_MOTION_AREA_SET)       ����������Ϣ�󣬷��سɹ���ʧ��

�ͻ��˷������ݸ�ʽ:     �����ֶ� - ͨ����(unsigned long)
�����ֶ� - ������Ϣ(NET_MOTION_INFO)

�������˻ظ����ݸ�ʽ:   �����ֶ� - ������Ϣ(NET_MOTION_INFO)
�����ֶ� - ��
*****************************************************************************************************************/
typedef struct _net_update_info
{
	unsigned long totalLen;		//�����ܳ���
	unsigned long index;//���ı�Ŵ�1  ��100  ��ʾ���ȣ� 0��ʾʧ�� 
	unsigned long currPageLen;	//�ְ�����
}NET_UPDATE_INFO;
/**********************************************��������Э��***************************************************
�ͻ���										��������

1��		��������CMD_REQUEST_UPDATE_START��			���ܵ����󣬻ظ����ܻ��߾ܾ�����Ȩ�ޣ�

2��		������������CMD_REQUEST_UPDATE_DATE��						����������

3��		����������CMD_REQUEST_UPDATE_CANCEL��						�����������

ע��a�����������÷ְ�����ʽ���ͣ�������Ҫ������ϴ���
b��������������ֹͣ��������������ɾ���Ѿ����ܵ����ݡ�

�ͻ��˷������ݸ�ʽ��	�����ֶ� - ��
�����ֶ� - �ְ���Ϣ - ���ݰ�

�������˻ظ����ݸ�ʽ��	�����ֶ� - �ظ���
*****************************************************************************************************************/
typedef struct _net_status
{
	unsigned long chnn_count;	//����ͨ������		
	ULONGLONG ullstatus;			//ÿλ�������ͨ����״̬
}NET_STATUS;
/**********************************************״̬��Ϣ����Э��**************************************************
��������

�������˻ظ����ݸ�ʽ��	�����ֶ� - ״̬���ݣ�video loss, motion, sensor��
�����ֶ� - ͨ����Ŀ - ¼���������� ��n == chnn number��
�����ֶ� - ��
*****************************************************************************************************************/
typedef struct _net_rec_status
{
	unsigned long chnn_count;	//����ͨ������
	ULONGLONG ullManualRec;
	ULONGLONG ullScheduleRec;
	ULONGLONG ullMotionRec;
	ULONGLONG ullSensorRec;
}NET_REC_STATUS;

//�ط����ͣ����ļ�����ʱ��
typedef enum _enum_playback_type
{
	PLAYBACK_ON_TIME,
	PLAYBACK_ON_FILE,
}ENUM_PLAYBACK_TYPE;

//���ؿͻ��˿�ʼ�ط�ʱ�õ��ṹ��
typedef struct _local_client_playback_info
{
	unsigned long  playbackType;
	unsigned long  starTime;
	unsigned long  endTime;
	unsigned long  curTime;
	unsigned long  pFileList;
	unsigned long  splitMode;
	ULONGLONG      chnnBits;
	unsigned long  pMsgCallBack;
	unsigned long  pCallBackObj;
    unsigned long  diskOwnerIndex;
	bool		   bSync;
}LOCAL_CLINET_PLAYBACK_INFO, *PLOCAL_CLINET_PLAYBACK_INFO;

//���ؿͻ��˿�ʼ�ط�ʱ�õ��ṹ��
typedef struct _local_client_pb_chg_ch_info
{
	unsigned long	splitMode;    
	ULONGLONG		chnnBits;
}LOCAL_CLINET_PB_CHG_CH_INFO, *PLOCAL_CLINET_PB_CHG_CH_INFO;

//���ؿͻ��˿��ƻط�ʱ���������˵Ļظ����õ��ýṹ��
typedef struct _local_client_playback_ctrl
{
	unsigned long playbackctrlcmd;  //���ͻطſ�������
	unsigned long playbackreply;    //����˵Ļظ���Ϣ
}LOCAL_CLINET_PLAYBACK_CTRL_INFO;


typedef struct _curchnn_info_
{
	unsigned long clickchnn;//�����ͨ����
	bool bsync;//�Ƿ�ͬ����trueΪͬ��,falseΪ�첽
	unsigned long speed;//�����������ͣ����һ���ٶȲ���
}CURCHNNINFO;

//���ؿͻ��˿�ʼ����ʱ�õ��ṹ��
typedef struct _local_client_backup_info
{
	unsigned long    pFileInfoBackup;
	unsigned long    fileNum;
	unsigned long    startTime;
	unsigned long    endTime;
	unsigned long    pDiskName;
	unsigned long    pMountDir;
	unsigned long    diskType;
	unsigned long    dataType;
	unsigned long    callBack;
	unsigned long    pCallBackObj;
	unsigned long    bHaveVideoPlay;
	unsigned long    bCombined;
	unsigned long    diskOwnerby;
}LOCAL_CLIENT_BACKUP_INFO;

typedef struct _crytoinfo
{
	unsigned long languageBits;
	unsigned char productSubID;
}CRYTOINFO;

//����طŰ�ťʱ���õĵĽṹ��
typedef struct _struct_clicklive_
{
	NET_DISPLAY_INFO curDisp;
	ULONGLONG   chnnBits;
}LIVESTRUCT;

typedef struct _yuanzheng_manual_rec_param_
{
	unsigned char   channel;		//���ĸ�ͨ�����в���
	unsigned char   recStatus;      //1-��ʼ¼�� 0-����¼��     
	unsigned long	recTime;		//��ʾ¼��೤ʱ�䣬��λΪ��
}YUANZHENG_MANUAL_REC_PARAM;

typedef struct _yuanzheng_param_
{
	char		ipUrl[128];
	char		asCode[64];
}YUANZHENG_PARAM;

typedef struct _yuanzheng_snap_param_
{
	char		  ipUrl[128];
	unsigned long timeS;
}YUANZHENG_SNAP_PARAM;

//
typedef struct _ip_device_info_
{
	unsigned long	deviceID;
	unsigned short	channel;	//�������豸�ڱ��ط����ͨ��
	unsigned short	status;		//����״̬

	char			szEtherName[16];		//���Ϊ�գ�Ĭ��Ϊeth0
	char			szServer[64];			//�����豸IP��ַ
	unsigned short	nPort;		//�����豸�˿ں�
	unsigned short	nHttpPort;	//HTTP�˿�
	unsigned short	nCtrlPort;	//���ƶ˿ڣ�һ����nPort��ͬ
	char			szID[64];	//�����豸��ʶ(����MAC��ַ)
	char			username[36];//ע�ᵽ�����豸��ʱ���õ��û���
	char			password[36];//ע�ᵽ�����豸��ʱ���õ�����

	unsigned long	manufacturerId;	//�豸����ID
	char			manufacturerName[36];//�豸������
	char			productModel[36];//��Ʒ�ͺ�

	unsigned char	bUseDefaultCfg;
	unsigned char	bPOEDevice;
	unsigned char	resv[2];
}IP_DEVICE_INFO;

typedef struct _discover_device_info
{
	char			szAddr[64];
	unsigned long	netmask;
	unsigned long	gateway;
	unsigned short	netport;
	unsigned short	nHttpPort;
	unsigned short	nCtrlPort;
	char			szID[64];	//�����豸��ʶ
	unsigned long	manufacturerId;
	char			manufacturerName[36];
	char			productModel[36];
}DISCOVER_DEVICE_INFO;

typedef struct _nvr_support_manufacturer
{
	unsigned long	manufacturerId;
	char			manufacturerName[36];
	unsigned char	bNeedHttpPort;		//�Ƿ���ҪHTTP�˿ڽ���ͨ��
	unsigned char	bNeedCtrlPort;		//�Ƿ���Ҫר�ŵĿ��ƶ˿�ͨ��
	unsigned char	resv[2];
}NVR_SUPPORT_MANUFACTURER;

typedef struct _af_control_
{
	AF_CTRL_TYPE afType;
	int			 nValue;  //�ֶ�΢��ʱ��1����+��0����-��������������nValue�ʹ�0
}AF_CONTROL;

#if defined(__ENVIRONMENT_WIN32__) || defined(__TDFH__) || defined(__TDNXP__) || defined(__CHIP3531__) || defined(__CHIP3535__) || defined(__CHIP3521__) || defined(__CHIP3520A__) || defined(__CHIPTI__) || defined(__CHIP3520D__) || defined(__CHIPGM__) 
#pragma pack()
#endif

#endif //_NET_PROTOCOL_H_
