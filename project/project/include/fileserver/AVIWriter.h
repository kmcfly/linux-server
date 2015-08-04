
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
	
	//�ĸ���׼�Ķ�д����
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
	AVI_TYPE	m_aviType;					//AVI�ļ�����
	bool		m_bFileOpen;
	bool		m_bDataArrived;
	int			m_nAudioType;

	AVIMAINHEADER	m_aviHeader;	//
	AVISTREAMHEADER	m_strvhdr;		//��Ƶ��ͷ��Ϣ
	AVISTREAMHEADER	m_strahdr;		//��Ƶ��ͷ��Ϣ
	AVISTREAMHEADER	m_strvTimehdr;	//��Ƶʱ����ͷ��Ϣ
	AVISTREAMHEADER	m_straTimehdr;	//��Ƶʱ����ͷ��Ϣ
	LONGLONG		m_TextFormat;	//�ı�����ʽ

	FILE			*m_pFile;		//�ļ����
	unsigned long	m_bufVideoNum;	//������Ƶ֡��
	unsigned long	m_bufDataLen;	//���������ܳ���
	unsigned long	m_bufFramesNum;	//����֡��������ʱ�䣬��Ƶ������ȷ��INDEX����Ŀ��
	BYTE			*m_pData;		//���ݻ���

	AVIINDEXENTRY	*m_pIndexData;	//INDEX������
	unsigned long	m_nMaxIndex;	//INDEX�����������ɵ�������
	unsigned long	m_indexNum;		//д���INDEX����

	unsigned long   m_maxBufSize;
	unsigned long   m_maxBufFmNum;
	unsigned long	m_maxBufIndexNum;

	unsigned long	m_curWritePos;	//��¼��ǰ�ļ�дƫ��

	LONGLONG		m_firstFrameTime;//��һ֡���ݵ�ʱ��
	unsigned long	m_nPreFrameIndex;//д���ǰһ֡���ݵ�֡���

	int				m_FileStatus;		//�����ļ���ǰ״̬

//////////////////////////////////////////////////////////////////////////
//	�ļ��еĸ��ֱ��
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
