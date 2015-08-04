/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : 袁石维
** Date         : 2008-06-24
** Name         : RecWriter.h
** Version      : 1.0
** Description  :
** Modify Record:
1:
***********************************************************************/
#ifndef _REC_WRITER_H_
#define _REC_WRITER_H_

#include "PUB_common.h"
#include "RecFileType.h"
#include "mylist.cpp"

// const unsigned long AUDIO_BUF_NUM = 20;//缓冲音频的数目
// const unsigned long MAX_WRITE_BUF_LEN = 256*1024;	//缓冲区大小
// //写数据缓冲区的最后4个字节置0,随数据一起写入文件,
// //占据帧标识的位置，用以判断文件结束
// const unsigned long BLANK_TAIL_LEN = sizeof(unsigned long);				
#ifndef _WIN32
#include <sys/select.h>
#endif

class CRecWriter
{
public:
// 	typedef enum _write_frame_ret
// 	{
// 		//成功，和成功但是文件将满 
// 		WRITE_FRAME_SUCC,
// 
// 		//以下为失败的情况
// 		WRITE_FRAME_NEED_VIDEO_FORMAT,
// 		WRITE_FRAME_NEED_AUDIO_FORMAT,
// 		WRITE_FRAME_FULL,		//文件已经写满
// 		WRITE_FRAME_FAIL,		//其他原因失败
// 	}WRITE_FRAME_RET;
public:
	//interface
	CRecWriter ();
	~CRecWriter ();

	bool Open (const char *pPath, unsigned long fileSizeM, bool bCombined, bool bBackup=false);
	void Close ();

	void StopRecrod(){;}
	void ClearAudioBuffer();//清除缓存的音频帧

	unsigned long WriteData(const FRAME_INFO_EX & frame);

	bool FFlush();

	static bool Repair(const char *pFilePath, LONGLONG *pEndTime);

	int WriteFileEventInfo(REC_FILE_EVENT_INFO *pEventInfo, int num);
protected:
	//methods

	//variable

private:
	//methods
	LONGLONG GetAudioTime();
	LONGLONG GetTextTime();

	unsigned long WriteIndex(unsigned long index, unsigned long pos, LONGLONG time);
	unsigned long WriteFrame(const FRAME_INFO_EX & frame);
	unsigned long OnWriteOneVideo(const FRAME_INFO_EX & frame);

	inline bool IsValidDataFlag(unsigned long flag){return ((flag == I_VIDEO_FLAG) || (flag == B_VIDEO_FLAG) || (flag == I_AUDIO_FLAG) || (flag == I_TEXT_FLAG));};

	static void FFlushEx(FILE * pFile);
	inline size_t RecFWrite(const void *ptr, size_t size, size_t nmemb, FILE *stream);
	//variable
	bool	m_bHasVideo;
	bool	m_bHasAudio;

	FILE		*m_pFile;

	//文件写游标值
	unsigned long m_indexPos;

	unsigned long m_IndexNum;
	unsigned long m_videoNum;
	unsigned long m_audioNum;
	unsigned long m_textNum;

	//记录文件中帧的起始及终止时间
	LONGLONG	m_startTime;
	LONGLONG	m_endTime;

	REC_FILE_HEADER m_fileHeader;
	REC_FRAME_INFO	m_frameInfo;	//用于写入每一帧时用，避免每写一次申请一次内存
	INDEX_INFO		m_indexInfo;
	unsigned char	*m_pDataEndFlag;
	REC_BUF_INFO	m_bufInfo;

	unsigned long	m_vpFrameNum;	//P帧数量，如果I帧间隔太小，则不能把每个关键帧写入记录
	bool			m_bNeedWriteKey;
	CMyList <FRAME_INFO_EX>	m_audioList;
	CMyList <FRAME_INFO_EX>	m_textList;


	unsigned long m_MAX_REC_SUB_OFF_LEN;
	unsigned long m_MAX_REC_FILE_LEN;
	unsigned long m_MAX_KEY_FRAME_NUM;
	unsigned long m_VIDEO_FORMAT_POS;
	unsigned long m_AUDIO_FORMAT_POS;
	unsigned long m_REC_FILE_EVENT_INFO_POS;
	unsigned long m_FRAME_DATA_POS;
	unsigned long m_FILE_BASIC_LEN;

	bool          m_bCombined;
	static unsigned long m_lastFFlushTime;
	bool			m_bBuckUp;

	fd_set		m_fdset;

	int			m_fd;
};

#endif //_REC_WRITER_H_

