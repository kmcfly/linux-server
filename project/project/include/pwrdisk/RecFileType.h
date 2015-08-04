/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : Ԭʯά
** Date         : 2008-06-24
** Name         : RecFileType.h
** Version      : 1.0
** Description  :
** Modify Record:
1:
***********************************************************************/
#ifndef _REC_FILE_TYPE_H__
#define _REC_FILE_TYPE_H__

#ifdef __ENVIRONMENT_WIN32__
#include <vfw.h>
#else
#include "lvfw.h"
#endif

#include "Typedef.h"

/*
���巵��ֵ�ĺ���
*/
enum _read_file_ret
{
	ERR_READ_FILE_OK,
	ERR_READ_FILE_NO_OPEN,
	ERR_READ_FILE_SEEK_TO_END,
	ERR_READ_FILE_SEEK_TO_BEGIN,
	ERR_READ_FILE_PARAM_INVALID,
	ERR_READ_FILE_SEEK_FAILED,
	ERR_READ_FILE_READ_FAILED,
	ERR_READ_FILE_BUF_SMALL,
	ERR_READ_FILE_NO_AUDIO,
	ERR_READ_FILE_EXCEPTION
};

#pragma pack(4)
typedef struct _rec_file_header
{
	unsigned long hFlag;			//����Ϣͷ���
	unsigned long hLen;				//����Ϣͷ����

	unsigned long frameInfoLen;		//REC_FRAME_INFO�ṹ�峤�ȣ����ڵ��ýṹ�巢���仯ʱ����ȡģ���ܼ��ݸ��ְ汾���ļ���s

	unsigned long fileFlag;			//�ļ���ǡ���"FTVT"
	unsigned long fileLen;			//�ļ��ܳ���
	unsigned long writeFlg;			//д��ǣ����Ϊ 0x5a5a ��Ϊ�����������ļ�

	unsigned long indexNum;			//д�������֡����
	unsigned long videoNum;			//��Ƶ֡����
	unsigned long audioNum;			//��Ƶ֡����
	unsigned long textNum;			//�ı�֡����

	unsigned long videoFormatAddr;	//��Ƶ��ʽλ��
	unsigned long audioFormatAddr;	//��Ƶ��ʽλ��
	unsigned long indexAddr;		//������λ��
	unsigned long dataAddr;			//������λ��

	LONGLONG	startTime;			//��ʼʱ��
	LONGLONG	endTime;			//����ʱ��
}REC_FILE_HEADER;

typedef struct _index_info 
{
	unsigned long	flag;				//��Ӧ��֡���
	unsigned long	index;				//��Ӧ��������
	unsigned long	position;			//��֡���ļ��е�λ��
	LONGLONG		time;				//֡ʱ��
}INDEX_INFO;

typedef struct _rec_frame_info
{
	unsigned long flag;				//��Ӧ�ı��
	unsigned long prevPos;			//ǰһ֡��λ��
	unsigned long nextPos;			//��һ֡��λ��
	unsigned long timeStampFlag;	//ʱ������
	unsigned long length;			//���ݳ���
	LONGLONG	  time;				//ʱ��
}REC_FRAME_INFO;

inline unsigned long RecFrameInfoChsum(REC_FRAME_INFO  * pFrameInfo)
{
	unsigned long  sum = 0;
	unsigned long  uxor = 0;
	unsigned long  *ptime32=(unsigned long *)(&pFrameInfo->time);

	sum+= pFrameInfo->flag;
	uxor ^= pFrameInfo->flag;

	sum+= pFrameInfo->prevPos;
	uxor^= pFrameInfo->prevPos;

	sum+= pFrameInfo->nextPos;
	uxor^= pFrameInfo->nextPos;

	sum+= pFrameInfo->length;
	uxor^= pFrameInfo->length;

	sum+= ptime32[0];
	uxor^= ptime32[0];

	sum+= ptime32[1];
	uxor^= ptime32[1];
	sum= sum ^uxor;

	return sum;
}


typedef struct _rec_buf_info
{
	unsigned char *pData;		//�������׵�ַָ��
	unsigned long bufLen;		//����������
	unsigned long dataLen;		//�Ѿ���������ݳ���
	unsigned long filePos;		//����������ʼ��������ļ���λ��
	unsigned long currPos;		//��ǰλ��
}REC_BUF_INFO;

//����д��ϱ����ļ���
typedef struct _rec_file_event_info
{
	unsigned long chnn;
	unsigned long type;	
	unsigned long audio;
	unsigned long reserve1;
	LONGLONG      startTime;
	LONGLONG      endTime;
	unsigned long width;
	unsigned long height;
	unsigned long reserve2[14];	
}REC_FILE_EVENT_INFO;

#pragma pack()

typedef enum _seek_mode
{
	SEEK_PREV_VFRM,
	SEEK_NEXT_VFRM,
	SEEK_FIRST_KEY,
	SEEK_LAST_KEY,
	SEEK_PREV_KEY,
	SEEK_NEXT_KEY,
}SEEK_MODE;

const unsigned long FILE_FLAG		    = mmioFOURCC ('F', 'T', 'V', 'T');
const unsigned long HEADER_FLAG		    = mmioFOURCC ('F', 'H', 'D', 'R');
const unsigned long HEADER_FLAG_COMBINE = mmioFOURCC ('F', 'H', 'D', 'C');

const unsigned long I_VIDEO_FLAG	= mmioFOURCC ('0', '0', 'd', 'b');
const unsigned long B_VIDEO_FLAG	= mmioFOURCC ('0', '0', 'd', 'c');
const unsigned long I_AUDIO_FLAG	= mmioFOURCC ('0', '0', 'w', 'b');
const unsigned long I_TEXT_FLAG		= mmioFOURCC ('0', '0', 't', 'x');

const unsigned long WRITE_ING_FLAG	= 0x5a5a;
const unsigned long WRITE_END_FLAG	= 0x0000;

const unsigned long WRITE_FLAG_POS		= 4 * sizeof(long);
const unsigned long INDEX_INFO_POS		= sizeof (REC_FILE_HEADER) + 32;

const unsigned long MAX_REC_FILE_EVENT_INFO = 10000;


#define REC_WRITE_DO_BUFFER 1
#define REC_READ_DO_BUFFER 1

typedef enum _write_frame_ret
{
	//�ɹ����ͳɹ������ļ�����
	WRITE_FRAME_SUCC,

	//����Ϊʧ�ܵ����
	WRITE_FRAME_NEED_VIDEO_FORMAT,
	WRITE_FRAME_NEED_AUDIO_FORMAT,
	WRITE_FRAME_FULL,		//�ļ��Ѿ�д��
	WRITE_FRAME_FAIL,		//����ԭ��ʧ��
}WRITE_FRAME_RET;

const unsigned long AUDIO_BUF_NUM = 20;//������Ƶ����Ŀ
const unsigned long MAX_WRITE_BUF_LEN = 256*1024;	//��������С
//д���ݻ����������4���ֽ���0,������һ��д���ļ�,
//ռ��֡��ʶ��λ�ã������ж��ļ�����
const unsigned long BLANK_TAIL_LEN = sizeof(unsigned long);	

#endif //_REC_FILE_TYPE_H__
