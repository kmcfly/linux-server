/***********************************************************************
** Copyright   Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : zxx
** Date         : 2012-11-28
** Name         : ShanghaiCmd.h
** Version      : 1.0
** Description  :
** Modify Record:
1:
***********************************************************************/

#ifndef _SHANGHAI_CMD_H_
#define _SHANGHAI_CMD_H_

#include "uuid.h"

typedef struct _snap_info_ex_
{
	unsigned long bEnable;			//�Ƿ���Ч
	ULONGLONG	  chnnbit;			//��λ��ͨ����
	unsigned long startTime;		//��ʼʱ��
}SNAP_INFOEX;

typedef struct _oper_info_
{
	unsigned long		operType;//��Ϣ�Ĳ�������
	unsigned long		chnn;	//��Ϣͨ��
	char			    triggerTime[20];//����ʱ��
	unsigned char		*pNote;//����������˵������
	int					textLen;//������˵������
	unsigned char		*pPic;//����������ͼƬ��Ϣ
	unsigned long		picLen;//ͼƬ����

	uuid_t 				uniqueGUID;//ȫ��ΨһGUID;
}OPER_INFO;

//�Ϻ�ƽ̨�¼�

typedef enum _shanghai_cmd_type_
{
	//ͼ�񲿷�
	ALARMUPLOAD_IMAGE = 1,		//��������ͼ��
	KEEPUPLOAD_IMAGE,		//�����ϴ�ͼ��
	DAILYUPLOAD_IMAGE,		//�ճ��ϴ�ͼ��
	TESTUPLOAD_IMAGE,		//�����ϴ�ͼ��
	ONDEMANDUPLOAD_IMAGE,	//�����ϴ�ͼ��
	//���ϱ��޲���
	OTHERSYS_ERR = 11,		//����ϵͳ����
	AROUNDALARM_ERR,		//�ܽ籨������
	NETALARM_ERR,			//������������
	LOCALALARM_ERR,			//���ر�������
	VIDEOMONITOR_ERR,		//��Ƶ��ع���
	BUILDINTERCOM_ERR,		//¥��Խ�����
	ENTRANCECONTROL_ERR,	//����ڿ��ƹ���
	ELECTRIALPATROL_ERR,	//����Ѳ������
	//��־����
	DVRSYSSTART,			//DVRϵͳ����
	DVRSYSEXIT,				//DVRϵͳ�˳�
	DVRSYSERREXIT,			//DVRϵͳ�쳣�˳�
	DVRCFGSET,				//DVR��������
	DVRCFGSAVE,				//DVR��������
	DVRSYSVIDEOLOSS,		//DVR��Ƶ��ʧ
	DVRSYSMOTION,			//DVR�ƶ����
	DVROUTSIDEALARM,		//DVR�ⲿ����
	DVRSYSCANCLEALARM,		//DVRϵͳ�������
	DVRSYSFAULTEXIT,		//DVRϵͳ�Ƿ��˳�

	DVRREPAIRASSIGN,		//ά��ǩ��
	DVRKEEPASSIGN,			//����ǩ��
	DVRLOCALREPLAY,			//���ػط�
	DVRNETREPLAY,			//Զ�̻ط�

	CHECKUPLOAD_IMAGE,		//�����ϴ�ͼ��

	DVRDISKERR =41,			//DVR���̴���
	DVRSYSHEARTOUT,			//ϵͳ������ʱ
	DVRSYSHEARTRECOVER,		//ϵͳ�����ָ�
	DVRVIDEOOTHER,			//��Ƶ�����¼�


}SHANGHAI_CMD_TYPE;
#endif
//end
