/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       :����
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

	bool				m_bFileNormal;		//�ļ��Ƿ���������

	REC_FILE_HEADER		m_fileHeader;		//�ļ�ͷ��Ϣ
	BITMAPINFOHEADER	m_bitmapInfo;		//��Ƶ��ʽ
	WAVEFORMATEX		m_waveInfo;			//��Ƶ��ʽ
	INDEX_INFO			*m_pIndexInfo;		//�ؼ�֡����

	long				m_curIndex;			//��ǰ�ؼ�֡�������

	REC_FRAME_INFO		m_curFrame;			//��ǰ֡��Ϣ

	REC_BUF_INFO		m_bufInfo;
	unsigned long m_cutSize;

	//��һ�ν���طŴ��ļ�ʱ��ֻ���ȡ�ؼ�֡����Ϣ����������طŵ��ٶȻ����
	//֮�����طſ���ÿ�β�ѯʵʱд�������֡��
	unsigned long m_bKeyInfoGeted; //�ļ��ؼ���Ϣ�Ƿ��ȡ��
};

#endif //_PRIMARY_REC_READER_H_

