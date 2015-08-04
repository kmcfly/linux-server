/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : Ԭʯά
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

// const unsigned long AUDIO_BUF_NUM = 20;//������Ƶ����Ŀ
// const unsigned long MAX_WRITE_BUF_LEN = 256*1024;	//��������С
// //д���ݻ����������4���ֽ���0,������һ��д���ļ�,
// //ռ��֡��ʶ��λ�ã������ж��ļ�����
// const unsigned long BLANK_TAIL_LEN = sizeof(unsigned long);				
#ifndef _WIN32
#include <sys/select.h>
#endif

class CRecWriter
{
public:
// 	typedef enum _write_frame_ret
// 	{
// 		//�ɹ����ͳɹ������ļ����� 
// 		WRITE_FRAME_SUCC,
// 
// 		//����Ϊʧ�ܵ����
// 		WRITE_FRAME_NEED_VIDEO_FORMAT,
// 		WRITE_FRAME_NEED_AUDIO_FORMAT,
// 		WRITE_FRAME_FULL,		//�ļ��Ѿ�д��
// 		WRITE_FRAME_FAIL,		//����ԭ��ʧ��
// 	}WRITE_FRAME_RET;
public:
	//interface
	CRecWriter ();
	~CRecWriter ();

	bool Open (const char *pPath, unsigned long fileSizeM, bool bCombined, bool bBackup=false);
	void Close ();

	void StopRecrod(){;}
	void ClearAudioBuffer();//����������Ƶ֡

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

	//�ļ�д�α�ֵ
	unsigned long m_indexPos;

	unsigned long m_IndexNum;
	unsigned long m_videoNum;
	unsigned long m_audioNum;
	unsigned long m_textNum;

	//��¼�ļ���֡����ʼ����ֹʱ��
	LONGLONG	m_startTime;
	LONGLONG	m_endTime;

	REC_FILE_HEADER m_fileHeader;
	REC_FRAME_INFO	m_frameInfo;	//����д��ÿһ֡ʱ�ã�����ÿдһ������һ���ڴ�
	INDEX_INFO		m_indexInfo;
	unsigned char	*m_pDataEndFlag;
	REC_BUF_INFO	m_bufInfo;

	unsigned long	m_vpFrameNum;	//P֡���������I֡���̫С�����ܰ�ÿ���ؼ�֡д���¼
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

