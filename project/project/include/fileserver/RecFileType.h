/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : 袁石维
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
定义返回值的含义
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
	unsigned long hFlag;			//该信息头标记
	unsigned long hLen;				//该信息头长度

	unsigned long frameInfoLen;		//REC_FRAME_INFO结构体长度，用于当该结构体发生变化时，读取模块能兼容各种版本的文件。s

	unsigned long fileFlag;			//文件标记——"FTVT"
	unsigned long fileLen;			//文件总长度
	unsigned long writeFlg;			//写标记，如果为 0x5a5a 则为非正常结束文件

	unsigned long indexNum;			//写入的索引帧总数
	unsigned long videoNum;			//视频帧总数
	unsigned long audioNum;			//音频帧总数
	unsigned long textNum;			//文本帧总数

	unsigned long videoFormatAddr;	//视频格式位置
	unsigned long audioFormatAddr;	//音频格式位置
	unsigned long indexAddr;		//索引区位置
	unsigned long dataAddr;			//数据区位置

	LONGLONG	startTime;			//开始时间
	LONGLONG	endTime;			//结束时间
}REC_FILE_HEADER;

typedef struct _index_info 
{
	unsigned long	flag;				//对应的帧标记
	unsigned long	index;				//对应的索引号
	unsigned long	position;			//该帧在文件中的位置
	LONGLONG		time;				//帧时间
}INDEX_INFO;

typedef struct _rec_frame_info
{
	unsigned long flag;				//对应的标记
	unsigned long prevPos;			//前一帧的位置
	unsigned long nextPos;			//后一帧的位置
	unsigned long timeStampFlag;	//时间戳标记
	unsigned long length;			//数据长度
	LONGLONG	  time;				//时间
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
	unsigned char *pData;		//缓冲区首地址指针
	unsigned long bufLen;		//缓冲区长度
	unsigned long dataLen;		//已经缓冲的数据长度
	unsigned long filePos;		//缓冲数据起始点相对于文件的位置
	unsigned long currPos;		//当前位置
}REC_BUF_INFO;

//用在写组合备份文件里
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
	//成功，和成功但是文件将满
	WRITE_FRAME_SUCC,

	//以下为失败的情况
	WRITE_FRAME_NEED_VIDEO_FORMAT,
	WRITE_FRAME_NEED_AUDIO_FORMAT,
	WRITE_FRAME_FULL,		//文件已经写满
	WRITE_FRAME_FAIL,		//其他原因失败
}WRITE_FRAME_RET;

const unsigned long AUDIO_BUF_NUM = 20;//缓冲音频的数目
const unsigned long MAX_WRITE_BUF_LEN = 256*1024;	//缓冲区大小
//写数据缓冲区的最后4个字节置0,随数据一起写入文件,
//占据帧标识的位置，用以判断文件结束
const unsigned long BLANK_TAIL_LEN = sizeof(unsigned long);	

#endif //_REC_FILE_TYPE_H__
