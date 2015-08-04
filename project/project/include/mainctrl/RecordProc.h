/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : 袁石维
** Date         : 2007-10-08
** Name         : RecordProc.h
** Version      : 1.0
** Description  :
** Modify Record:
1:
***********************************************************************/
#ifndef _RECORD_PROC_H_
#define _RECORD_PROC_H_

#include "ReclogManEx.h"
#include "PreRecord.h"
#include "PreMemRecord.h"
#include "MessageMan.h"

#if defined(__PRIMARY_WRDISK__)
#include "PrimaryRecWriter.h"
#else
#include "RecWriter.h"
#endif

typedef enum _record_mode
{  
	RECORD_MODE_NULL       = 0x0,
	RECORD_MODE_MAIN       = 0x01,  //主录像模式。用第0位表示
	RECORD_MODE_REDUNDANCY = 0x02,  //冗余录像模式。用第1位表示
}RECORD_MODE;

typedef struct _record_writer
{
	bool          bAction;
	bool          bNeedNewFile;
	bool          bOpen;
#if defined(__PRIMARY_WRDISK__)
	CPrimaryRecWriter    recWriter;
#else
	CRecWriter    recWriter;
#endif
	FILE_INFO_EX  fileInfo;
	unsigned long startType; //用于记录当前录像的文件，正在录制哪些类型的数据，比如：正在录制移动和手动录像。
}RECORD_WRITER;

typedef struct _redu_record_writer
{
	bool          bAction;
	bool          bNeedNewFile;
	bool          bOpen;
#if defined(__PRIMARY_WRDISK__)
	CPrimaryRecWriter    *pRecWriter;
#else
	CRecWriter    *pRecWriter;
#endif
	FILE_INFO_EX  fileInfo;
	unsigned long startType; //用于记录当前录像的文件，正在录制哪些类型的数据，比如：正在录制移动和手动录像。
}REDU_RECORD_WRITER;


typedef enum _record_write_status
{
	RECORD_WRITE_SUCC,			//写入成功
	RECORD_WRITE_NO_FILE,		//无文件可写
	RECORD_WRITE_NO_RECORDING,	//非录像状态
	RECORD_WRITE_NO_BITMAPINFO,	//无bitmap信息
	RECORD_WRITE_NO_WAVEFORMAT,	//无wave format信息
	RECORD_WRITE_FAIL,			//写入失败
}RECORD_WRITE_STATUS;

class CRecordProc
{
public:
	//interface
	CRecordProc();
	~CRecordProc();

	bool Initial(unsigned long preRecordType, unsigned long channel, CReclogManEX *pReclogManEx, const char *pFilePath = NULL, char *pPrerecordPath = NULL, CMessageMan *pMsgMan = NULL);
	void Quit();
	bool IsCanPreDiskRecord();

	void Reset();

	RECORD_WRITE_STATUS DateProc(CFrameData & frameInfo, bool & bKeyFrame);
	
	bool Record(RECORD_TYPE type);
	bool StopRecord(RECORD_TYPE type);
	void FlushRecDataToDisk();
	void OpenRecordFile(BITMAPINFOHEADER *pBitmap, WAVEFORMATEX *pWave);
	void RemoveData();
	void SetPreRecTime(unsigned long time);
	void SetPreRecDataLen(unsigned long maxPreRecDataLen);
	void RecoverPreDiskRecTime();

	void SetRecycleRecord(bool bEnable);
	void SetRedundancyRecord(bool bEnable);

	void ResetPreDiskRecordPath(int prerecordIndex, bool &bPreRecord);
protected:
	//methods

	//variable

private:
	//methods
	void WriteReclogEnd(FILE_INFO_EX &fileInfo, unsigned long startType);
	bool WriteReclogStart(FILE_INFO_EX &fileInfo, unsigned char type, LONGLONG startTime, unsigned long &startType);
#if defined(__PRIMARY_WRDISK__)
	bool GetFile(unsigned long channel, unsigned short &diskIndex, unsigned short &fileIndex, unsigned char type, CPrimaryRecWriter *recWriter);
#else
	bool GetFile(unsigned long channel, unsigned short &diskIndex, unsigned short &fileIndex, unsigned char type, CRecWriter *recWriter);
#endif
	//向reclogMan注册和注销录像文件的索引
	void RegisterFileIndex(FILE_INFO_EX &fileInfo);
	void CancelFileIndex(FILE_INFO_EX &fileInfo); //CancelFileIndex一定要放到WriteReclogEnd后面调用。
	                                              //原因：WriteReclogEnd函数写的是段结束信息。CancelFileIndex写的是文件结束信息。
	                                              //要先写完段信息之后，再写文件结束

	//负责的通道
	unsigned long        m_channel;

	//负责日志
	CReclogManEX		*m_pReclogManEx;
	CMessageMan			*m_pMsgMan;

	//文件存放路径
	char				*m_pFilePath;

	//录像类型：如手动录像，定时录像
	unsigned long       m_startType;
	unsigned long       m_currType;

    //录像模式：主录像（即非冗余录像）还是冗余录像，还是两者共存。
	bool                m_bRecycleRecord;      //是否配置了：覆盖特性
    RECORD_WRITER       m_mainRecWriter;       //负责：主录像
	REDU_RECORD_WRITER  m_reduRecWriter;       //负责：冗余录像

	//负责预录像
	CPreRecord			m_preDiskRecord;
	CPreMemRecord       m_preMemRecord;

    //保存格式变量
	BITMAPINFOHEADER	*m_pBitmapInfo;
	WAVEFORMATEX		*m_pWaveFormat;

	//
	LONGLONG			m_lastFrameTime;

	unsigned long       m_preRecordType;

	unsigned long m_actualPrimaryStreamChnnNum; //实际的本地通道和网络通道主码流数目
};
#endif //_RECORD_PROC_H_

