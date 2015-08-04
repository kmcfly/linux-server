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
#ifndef _PRIMARY_REC_READER_H_
#define _PRIMARY_REC_READER_H_

#include "PUB_common.h"
#include "Typedef.h"
#include "RecFileType.h"
#include "rec_file.h"
#include "device.h"

#ifdef __ENVIRONMENT_WIN32__
#include <afxmt.h>
#endif

class CPrimaryRecReader
{
public:
	//interface
	CPrimaryRecReader ();
	~CPrimaryRecReader();

	bool Open(const char * pFilePath,FILE_BUF_TYPE fileType = FILE_BUF_TYPE_LOCAL, bool bForBackup=false);
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
//	bool RefreshIndex();

	static bool Repair(const char *pFilePath,  LONGLONG *pEndTime);
;
protected:
	//methods

	//variable

private:
	//methods
	bool ExtractKeyInfoFromFile();
	bool ExtractInfoFromFile();
	int RECFseek(long offset, int seek);
	size_t RECFread(void *pBuf, size_t len, size_t count);

	static bool IsValidDataFlag(unsigned long flag);

	
	//variable
	CRecord_file		*m_pRec;

	bool				m_bFileNormal;		//文件是否正常结束

	REC_FILE_HEADER		m_fileHeader;		//文件头信息
	BITMAPINFOHEADER	m_bitmapInfo;		//视频格式
	WAVEFORMATEX		m_waveInfo;			//音频格式
	INDEX_INFO			*m_pIndexInfo;		//关键帧索引

	long				m_curIndex;			//当前关键帧索引序号

	REC_FRAME_INFO		m_curFrame;			//当前帧信息

	REC_BUF_INFO		m_bufInfo;
	unsigned long m_cutSize;

	//第一次进入回放打开文件时，只需获取关键帧的信息，这样进入回放的速度会更快
	//之后进入回放可以每次查询实时写入的数据帧。
	unsigned long m_bKeyInfoGeted; //文件关键信息是否获取过
};

#endif //_PRIMARY_REC_READER_H_

