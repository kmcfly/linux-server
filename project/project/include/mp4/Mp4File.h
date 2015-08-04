#ifndef __MP4_FILE_H__
#define __MP4_FILE_H__

#include "string.h"
//#include "TVT_PubDefine.h"
#include "mp4writer.h"
#include "mp4reader.h"
///////////////////////////////////////////////////////////
//ǰ�ö���, ����ͷ�ļ����û���
class Mp4Writer;
class Mp4Reader;
class H264HeaderParser;
///////////////////////////////////////////////////////////


typedef enum _mp4_enum_mode
{
	MP4_MODE_READ			= 0,		//ֻ��
	MP4_MODE_WRITE,						//ֻд
}MP4_ENUM_MODE;


class CMp4File
{
public:
	CMp4File(ENUM_MP4_WRITE_DESTYPE desType = ENUM_MP4_DESTYPE_LOCAL);
	~CMp4File();

	//��, �ر��ļ�
	bool Open(const char* path, MP4_ENUM_MODE mode, unsigned long fileSize = DEFAULT_DATA_CNT);//fileSize�˲���ֻ����ftpд��ʽ��ʱ��Ч
	void Close();


	//д�ļ��ӿ�***********************************************************************************
	//params:
	/*
			timeStamp				: ʱ���
			frameType				: ֡����
			pData					: ����
			dataLen					: ���ݳ���
			width					: ��
			height					: ��
			keyFrame				: ��0 Ϊ�ؼ�֡
	**********************************************************************************************/
	long Write(INT64 timeStamp, long frameType, unsigned char* pData, long dataLen, long width, long height, long keyFrame);
	UINT GetDataCnt();//д�˶����ֽ�
	long Write(FRAME_INFO_EX* pFrame, unsigned long frameNum = 1);

	long Read(BYTE* pBuf, UINT* pLength, INT64* pTimestamp, long long* pTimeScale, bool *pbFrame, long frameType, UINT sampleID);
	//�Զ��ķ�ʽ�򿪺��ȡ��Ϣ����
	long GetSampleCount(bool bVedio);
	long GetWidth();
	long GetHeight();

    //*************************************************************************************************

protected:
	bool					m_bOpen;		//�Ƿ��Ѿ����ļ�
	bool					m_bStart;		//�Ƿ���Կ�ʼд, ���
	bool                    m_bFrameWrited; //�Ƿ��Ѿ�������д����

	Mp4Reader*				m_pReader;		//���ļ���
	Mp4Writer*				m_pWriter;		//д�ļ���

	H264HeaderParser*		m_pH264Parser;	//h264ͷ������
	ENUM_MP4_WRITE_DESTYPE  m_desType;      //��д��ftp���Ǳ���

	unsigned long  long     m_lTimeStamp;
	unsigned long  long     m_lTimeStampTmp;
	unsigned long  long     m_lastTimeStamp;
	unsigned long  long     m_lastFrameTime;
	unsigned long  long     m_curFrameTime;

	unsigned long  long     m_lAudioTimeStamp;
	unsigned long  long     m_lAudioTimeStampTmp;
	unsigned long  long     m_lastAudioTimeStamp;
	unsigned long  long     m_lastAudioFrameTime;
	unsigned long  long     m_curAudioFrameTime;
	bool                    m_bFirstFrame;
	unsigned long           m_lastStreamType;

};


#endif

//end
