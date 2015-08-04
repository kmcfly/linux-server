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
#ifndef _PRIMARY_REC_WRITER_H_
#define _PRIMARY_REC_WRITER_H_

#include "PUB_common.h"
#include "RecFileType.h"
#include "mylist.cpp"
#include "device.h"
#include "rec_file.h"


class CPrimaryRecWriter
{
public:
	//interface
	CPrimaryRecWriter ();
	~CPrimaryRecWriter ();

	bool Open(const char *pPath,unsigned long fileSizeM, bool bCombined);
	void Close();
	void StopRecrod();

	unsigned long WriteData(const FRAME_INFO_EX & frame);

	void SetFileBufType(FILE_BUF_TYPE fileBufType){m_fileBufType = fileBufType;}

	bool FFlush();

	static bool Repair(const char *pFilePath, LONGLONG *pEndTime);

	int WriteFileEventInfo(REC_FILE_EVENT_INFO *pEventInfo, int num);
#ifdef _WIN32
	CRecord_file * GetRecFile()
	{
		return m_pRec;
	}
#endif

protected:
	//methods

	//variable

private:
	//methods
	LONGLONG GetAudioTime();
	LONGLONG GetTextTime();
#ifdef _WIN32
public:
#endif
	unsigned long WriteIndex(unsigned long index, unsigned long pos, LONGLONG time);
	unsigned long WriteFrame(const FRAME_INFO_EX & frame);
	unsigned long OnWriteOneVideo(const FRAME_INFO_EX & frame);

	inline bool IsValidDataFlag(unsigned long flag){return ((flag == I_VIDEO_FLAG) || (flag == B_VIDEO_FLAG) || (flag == I_AUDIO_FLAG) || (flag == I_TEXT_FLAG));};

	static void FFlushEx(FILE * pFile);
	//variable
	bool	m_bHasVideo;
	bool	m_bHasAudio;

	CRecord_file *m_pRec;

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
	unsigned long   m_filePos;
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
	FILE_BUF_TYPE	m_fileBufType;
	
	static unsigned long m_openIndex;
	unsigned long	m_headspace;		//预留空间
};

#endif //_PRIMARY_REC_WRITER_H_

