
/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       :刘浩
** Date         : 2008-08-28
** Name         : AVIReaderEx.h
** Version      : 2.0
** Description  :interface for the AVIReader class.This version of avi file's 
				reading operation is mainly designed for compatibility with new
				video player. This version modified almost all the interface functions.
				
** Modify Record:
1:
***********************************************************************/
#ifndef AVIREADER_EX_H
#define AVIREADER_EX_H

#ifndef WIN32
#include <sys/time.h>
#include <unistd.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include <errno.h>
#include <malloc.h>
#include <assert.h>
#include "Typedef.h"
//#include "PUB_Define.h"
#include "RecFileType.h"
#include "PUB_common.h"


#ifdef WIN32
#include <vfw.h>
#else
#include "lvfw.h"
#endif

#include "avitypes.h"


typedef struct _avi_read_state
{
	//视频读取状态参数
	 LONGLONG			videoTime;
	 unsigned long		videoFrmSize;
	 bool				videoKeyFlag;
	//音频读取状态参数
	 LONGLONG			audioTime;
	 unsigned long		audioFrmSize;
	 bool		audioKeyFlag;
}AVI_READ_STATE, *LPAVI_READ_STATE;

const unsigned long AVI_READ_BUF_SUB = 1 * 1024;
const unsigned long	AVI_READ_BUF_LEN = (1 * 1024 + AVI_READ_BUF_SUB);
//const unsigned long AVI_READ_BUF_LEN = 704*576*2;
const unsigned long AVI_READ_END_POS = AVIFILELENGTH - MAXFRAMES*sizeof(AVIINDEXENTRY);

#define AVI_READER_FILE_NAME 0
/*
当没有正常结束时，需要刷新索引时新帧数的最小值
*/
#define REFRESHIDX_MINIDXNUM	100

class CAVIReader  
{
public:

	CAVIReader();
	virtual ~CAVIReader();

	bool Open(const char * pFilePath);
	bool Close();

	unsigned long GetFrameNum(FRAME_TYPE type);
	unsigned long GetVideoInfo(BITMAPINFOHEADER *pBitmapInfo);
	unsigned long GetAudioInfo(WAVEFORMATEX *pWaveInfo);

	FRAME_TYPE GetCurrFrameType();
	unsigned long GetCurrFrameLen();
	LONGLONG GetCurrFrameTime();
	unsigned long GetCurrFrame(FRAME_INFO_EX & frameInfo);

	unsigned long SeekVideoFrame(SEEK_MODE mode, unsigned long seekNum = 1);
	unsigned long SeekNextAudioFrame();
	unsigned long SeekNextFrame();		//定位并读取音频及视频状态
	unsigned long SeekKeyFrameToTime(LONGLONG time);

	bool RefreshIndex(){}
	static bool Repair(const char *pstrFilename, bool bDefSize,  LONGLONG *pEndTime);

protected:
	void ReadHeaderInfo();
	void ReadIndexItem();
	void ReadIndexItemEx();
	unsigned long ReadCurVideoFrmInfo();
	unsigned long ReadCurAudioFrmInfo();
	int FrameTimetoFrameIndex(int StreamType,LONGLONG tval);//得到指定时间的帧索引

protected:
	FILE				*m_pFile;
	bool				m_bFileOpen;
	AVI_READ_STATE		m_curReadState;			//当前读状态参数
	FRAME_TYPE			m_preReadType;			//上一次读取的帧类型

	AVIMAINHEADER		m_aviHeader;
	AVISTREAMHEADER		m_videoHeader;
	AVISTREAMHEADER		m_audioHeader;
	AVISTREAMHEADER		m_vTimeHeader;
	AVISTREAMHEADER		m_aTimeHeader;
	BITMAPINFOHEADER	m_bmpInfoHeader;
	WAVEFORMATEX		m_waveFormat;
	
	bool				m_bEndNormal;

	AVIINDEXENTRY		*m_pVideoIndex;
	AVIINDEXENTRY		*m_pAudioIndex;
	long				m_nVideoFrames;		//视频数据总帧数
	long				m_nAudioFrames;		//音频数据总帧数
	LONG				m_aCurIndex;
	LONG				m_vCurIndex;

	LONG				m_nIndexOffset;
//	文件中的各种标记
private:
	unsigned long	m_flagRIFF[3];		//RIFF....AVI.
	unsigned long	m_flagList[2];		//LIST....
	unsigned long	m_flagHeadList[1];	//hdrl
	unsigned long	m_flagStrList[4][3];	//LIST....strl
	unsigned long	m_flagStrFormat[2];	//strf....
	unsigned long	m_flagTotalFrame[6];//LIST[size = 16]odmldmlh[size = 4][totalFrame]
	unsigned long	m_flagJunk[2];		//JUNK....
	unsigned long	m_flagMovie[3];		//LIST....movi
	unsigned long	m_flagDataBlock[2];	//
	unsigned long	m_flagIndex[2];		//idxl....


	unsigned long	m_FileVer;
	BYTE			m_TimeBufLen;
	BYTE	m_ReadBuf [AVI_READ_BUF_LEN];
	size_t  m_bufHeadOffset;
	size_t  m_bufTailOffset;
	size_t  m_currOffset;
	
	int         AVIFseek(FILE *pFile, long offset, int seek);
	size_t      AVIFread(void *pBuf, size_t len, size_t count, FILE *pFile);

	bool    m_bCanRead;
	HANDLE  m_read_handle;
	unsigned long Thread_read_ID;
	bool    m_thread_read;
	CWaitEvent m_ReadEvent;
	CLock      m_ReadLock;

	bool CreateReadThd();
	void CloseReadThd();

	void ReadProc();
	static RESULT WINAPI thread_read(void * arg);
};

#endif

