
#ifndef _CAVIWRITEREX_H
#define _CAVIWRITEREX_H

#ifndef __ENVIRONMENT_WIN32__
#include <sys/time.h>
#include <unistd.h>
#endif

#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "Typedef.h"

#ifdef __ENVIRONMENT_WIN32__
#include <vfw.h>
#else
#include "lvfw.h"
#endif

#include "avitypes.h"

const BYTE AVI_WRITE_BACKUP = 0;
const BYTE AVI_WRITE_NOAMAL = 1;

//////////////////////////////////////////////////////////////////////////
class CAVIWriter
{
public:
	CAVIWriter();
	virtual ~CAVIWriter();
	
	//四个标准的读写函数
	bool Open(const char *szFilename,BITMAPINFOHEADER *bmpInfo,
				WAVEFORMATEX *wFormat,bool isPal, AVI_TYPE aviType, BYTE type=AVI_WRITE_NOAMAL);
	int WriteData(BYTE *pData, int Len, LONGLONG time, bool bIsVideo=true, bool bKeyFrame=false, bool bTimeStamp=false);
	void Close(void);
	int GetFileLength(void);	
	LONGLONG GetFirstFrameTime(void);
	int FileCheck(void);
	void Flush();

// virtual bool Repaire(const char *szFilename);

private:
	int AVIWriteNoSize(BYTE *pCKHeader, int Len1, BYTE *pData, int Len2, bool bKeyFrame);
	int AVIWriteDefSize(BYTE *pCKHeader, int Len1, BYTE *pData, int Len2, bool bKeyFrame);
private:
	AVI_TYPE	m_aviType;					//AVI文件类型
	bool		m_bFileOpen;
	bool		m_bDataArrived;
	int			m_nAudioType;

	AVIMAINHEADER	m_aviHeader;	//
	AVISTREAMHEADER	m_strvhdr;		//视频流头信息
	AVISTREAMHEADER	m_strahdr;		//音频流头信息
	AVISTREAMHEADER	m_strvTimehdr;	//视频时间流头信息
	AVISTREAMHEADER	m_straTimehdr;	//音频时间流头信息
	LONGLONG		m_TextFormat;	//文本流格式

	FILE			*m_pFile;		//文件句柄
	unsigned long	m_bufVideoNum;	//缓存视频帧数
	unsigned long	m_bufDataLen;	//缓存数据总长度
	unsigned long	m_bufFramesNum;	//缓存帧数（包括时间，音频，用于确定INDEX的数目）
	BYTE			*m_pData;		//数据缓存

	AVIINDEXENTRY	*m_pIndexData;	//INDEX缓存区
	unsigned long	m_nMaxIndex;	//INDEX缓存区可容纳的索引数
	unsigned long	m_indexNum;		//写入的INDEX总数

	unsigned long   m_maxBufSize;
	unsigned long   m_maxBufFmNum;
	unsigned long	m_maxBufIndexNum;

	unsigned long	m_curWritePos;	//记录当前文件写偏移

	LONGLONG		m_firstFrameTime;//第一帧数据的时间
	unsigned long	m_nPreFrameIndex;//写入的前一帧数据的帧序号

	int				m_FileStatus;		//定长文件当前状态

//////////////////////////////////////////////////////////////////////////
//	文件中的各种标记
private:
	bool UpdateFrameIndex(unsigned long bufFramesNum );
	int WriteTimeToFile(LONGLONG time,bool timestamp);
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
};

#endif 
