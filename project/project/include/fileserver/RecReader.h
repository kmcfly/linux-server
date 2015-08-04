/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       :刘浩
** Date         : 2008-07-08
** Name         : RecReader.h
** Version      : 1.0
** Description  :
** Modify Record:
1:
***********************************************************************/
#ifndef _REC_READER_H_
#define _REC_READER_H_

#include "Typedef.h"
#include "RecFileType.h"
#include "PUB_common.h"

#ifdef __ENVIRONMENT_WIN32__
#include <afxmt.h>
#endif

class CRecReader
{
public:
	//interface
	CRecReader ();
	~CRecReader();

	bool Open(const char * pFilePath,FILE_BUF_TYPE fileType=FILE_BUF_TYPE_LOCAL, bool bForBackup=false);
	bool Close();

	unsigned long GetFrameNum(FRAME_TYPE type);
	unsigned long GetVideoInfo(BITMAPINFOHEADER *pBitmapInfo);
	unsigned long GetAudioInfo(WAVEFORMATEX *pWaveInfo);
	void GetTimeRange(unsigned long &startTime, unsigned long &endTime);

	FRAME_TYPE GetCurrFrameType();
	unsigned long GetCurrFrameLen()
	{
		return m_curFrame.length;
	}
	LONGLONG GetCurrFrameTime()
	{
		return m_curFrame.time;
	}
	unsigned long GetCurrFrame(FRAME_INFO_EX & frameInfo);

	unsigned long SeekVideoFrame(SEEK_MODE mode, unsigned long seekNum = 1);
	unsigned long SeekNextFrame();
	unsigned long SeekKeyFrameToTime(LONGLONG time);

	unsigned long RECFtell();
	bool RefreshIndex();

	static bool Repair(const char *pFilePath,  LONGLONG *pEndTime);

protected:
	//methods

	//variable

private:
	//methods
	bool ExtractInfoFromFile();
	int RECFseek(FILE *pFile, long offset, int seek);
	size_t RECFread(void *pBuf, size_t len, size_t count, FILE *pFile);

	static bool IsValidDataFlag(unsigned long flag);


	//variable
	FILE				*m_pFile;
	bool				m_bFileNormal;		//文件是否正常结束

	REC_FILE_HEADER		m_fileHeader;		//文件头信息
	BITMAPINFOHEADER	m_bitmapInfo;		//视频格式
	WAVEFORMATEX		m_waveInfo;			//音频格式
	INDEX_INFO			*m_pIndexInfo;		//关键帧索引

	long				m_curIndex;			//当前关键帧索引序号

	REC_FRAME_INFO		m_curFrame;			//当前帧信息

	REC_BUF_INFO		m_bufInfo;
	unsigned long m_cutSize;
};

#endif //_REC_READER_H_

