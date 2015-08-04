#ifndef __MP4_FILE_H__
#define __MP4_FILE_H__

#include "string.h"
//#include "TVT_PubDefine.h"
#include "mp4writer.h"
#include "mp4reader.h"
///////////////////////////////////////////////////////////
//前置定义, 避免头文件引用混乱
class Mp4Writer;
class Mp4Reader;
class H264HeaderParser;
///////////////////////////////////////////////////////////


typedef enum _mp4_enum_mode
{
	MP4_MODE_READ			= 0,		//只读
	MP4_MODE_WRITE,						//只写
}MP4_ENUM_MODE;


class CMp4File
{
public:
	CMp4File(ENUM_MP4_WRITE_DESTYPE desType = ENUM_MP4_DESTYPE_LOCAL);
	~CMp4File();

	//打开, 关闭文件
	bool Open(const char* path, MP4_ENUM_MODE mode, unsigned long fileSize = DEFAULT_DATA_CNT);//fileSize此参数只在以ftp写方式打开时有效
	void Close();


	//写文件接口***********************************************************************************
	//params:
	/*
			timeStamp				: 时间戳
			frameType				: 帧类型
			pData					: 数据
			dataLen					: 数据长度
			width					: 宽
			height					: 高
			keyFrame				: 非0 为关键帧
	**********************************************************************************************/
	long Write(INT64 timeStamp, long frameType, unsigned char* pData, long dataLen, long width, long height, long keyFrame);
	UINT GetDataCnt();//写了多少字节
	long Write(FRAME_INFO_EX* pFrame, unsigned long frameNum = 1);

	long Read(BYTE* pBuf, UINT* pLength, INT64* pTimestamp, long long* pTimeScale, bool *pbFrame, long frameType, UINT sampleID);
	//以读的方式打开后获取信息方法
	long GetSampleCount(bool bVedio);
	long GetWidth();
	long GetHeight();

    //*************************************************************************************************

protected:
	bool					m_bOpen;		//是否已经打开文件
	bool					m_bStart;		//是否可以开始写, 或读
	bool                    m_bFrameWrited; //是否已经有数据写入了

	Mp4Reader*				m_pReader;		//读文件器
	Mp4Writer*				m_pWriter;		//写文件器

	H264HeaderParser*		m_pH264Parser;	//h264头解析器
	ENUM_MP4_WRITE_DESTYPE  m_desType;      //是写到ftp还是本地

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
