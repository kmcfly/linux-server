/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : Ԭʯά
** Date         : 2010-01-19
** Name         : RunLogItemDef.h
** Version      : 1.0
** Description  :
** Modify Record:
1:
***********************************************************************/
#ifndef _RUN_LOG_ITEM_DEF_H_
#define _RUN_LOG_ITEM_DEF_H_

typedef enum _run_log_item_id
{
	//�ط���
	RUN_LOG_ITEM_PB_BASE			= 0x00010000,
	RUN_LOG_ITEM_PB_OPEN_FILE_FAIL,					//���ļ�ʧ��
	RUN_LOG_ITEM_PB_SEEK_TIME_ERR,					//��λ��ĳ��ʱ������
	RUN_LOG_ITEM_PB_GET_FRAME_FAIL,					//��ȡ֡ʧ��
	RUN_LOG_ITEM_PB_EXIT,							//�ط��˳�
	RUN_LOG_ITEM_PB_GET_NEXT_FILE_FAIL,				//��ȡ������һ���ļ�ʧ��
	RUN_LOG_ITEM_PB_GET_PREV_FILE_FAIL,				//��ȡ������һ���ļ�ʧ��
	RUN_LOG_ITEM_PB_GET_FILE_FAIL,					//ͨ��Reclog��ȡ�ļ���ĿΪ��
	RUN_LOG_ITEM_PB_FILE_NULL,						//�ļ��б�Ϊ��
	RUN_LOG_ITEM_PB_OPEN_ALL_FILE_FAIL,				//�����е��ļ�ʧ��
	RUN_LOG_ITEM_PB_FILE_REWRITE,					//�ļ��������ˣ����ܴ򿪸��ļ�
	RUN_LOG_ITEM_PB_ADD_TO_READ_LIST,
	RUN_LOG_ITEM_PB_PUT_OUT_READ_LIST,

	//¼����
	RUN_LOG_ITEM_REC_BASE			= 0x00020000,	
	RUN_LOG_ITEM_REC_OPEN_FILE_SUCC,				//���ļ��ɹ�
	RUN_LOG_ITEM_REC_OPEN_FILE_FAIL,
	RUN_LOG_ITEM_REC_WRITE_START,
	RUN_LOG_ITEM_REC_WRITE_END,
	RUN_LOG_ITEM_REC_ACTION_TRUE,
	RUN_LOG_ITEM_REC_ACTION_FALSE,
	RUN_LOG_ITEM_REC_NO_FILE,
	RUN_LOG_ITEM_REC_ADD_REC_LIST,
	RUN_LOG_ITEM_REC_PUT_OUT_REC_LIST,

	//������
	RUN_LOG_ITEM_BK_BASE			= 0x00040000,
	RUN_LOG_ITEM_BK_NO_DISK,

	//������
	RUN_LOG_ITEM_NET_BASE			= 0x00080000,
	RUN_LOG_ITEM_NET_IP_COLLISION
}RUN_LOG_ITEM_ID;
#endif //_RUN_LOG_ITEM_DEF_H_
