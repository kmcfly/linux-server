#ifndef _BACKUP_READER_H_
#define _BACKUP_READER_H_

#if defined(__PRIMARY_WRDISK__)
#include "PrimaryRecReader.h"
#else
#include "RecReader.h"
#endif

#include "ReclogManEx.h"

class CBackupReader
{
public:

	enum
	{
		BACKUP_READER_ERROR_SUC = 0,
		BACKUP_READER_ERROR_NO_INIT,
		BACKUP_READER_ERROR_READ_FAILD,
		BACKUP_READER_ERROR_READ_FINISH,
	};

	//
	CBackupReader();

	//
	~CBackupReader();


	//
	int Start(FILE_INFO_EX_BACKUP *pFileInfoBackup, unsigned long fileNum, char *pSrcFilePath, 
 		      BACKUP_DATA_TYPE dataType,CReclogManEX *pReclogMan,bool bMustAudioFormat);
	//
	int GetFrame(FRAME_INFO_EX &frameInfo);

	//
	int GetOffSetTime()
	{
		return ((m_offSetTime+m_curOffSetTimeofOneFile)	* 100)/m_allTime;
	}

	//
	void Stop();
protected:
private:
    //
	int OpenFile(unsigned long fileIndex);

	//
	void CloseFile();

	//
	int GetOneFrame(FRAME_INFO_EX &frameInfo);

	//与读取视频文件和读取日志文件有关
	FILE_INFO_EX_BACKUP     *m_pFileInfoBackup;
	unsigned long    m_fileNum;
	char             *m_pSrcFilePath;
	unsigned long    m_fileIndex;
	char             *m_pSrcFilePathName;
	BITMAPINFOHEADER m_bitmapInfo;
	WAVEFORMATEX     m_waveFormat;
	bool             m_bReVideoFmat;
	bool             m_bReAudioFmat;
	bool             m_bReadFileEnd;
	CReclogManEX     *m_pReclogMan;
#if defined(__PRIMARY_WRDISK__)
	CPrimaryRecReader *m_pRecReader;
#else
	CRecReader       *m_pRecReader;
#endif
	bool             m_bOpenFile;
	unsigned char    m_channelNum;

	//读取文件的条件
	LONGLONG         m_startTime;
	LONGLONG         m_endTime;
	BACKUP_DATA_TYPE m_dataType;

	//用于表示读取状态的变化
	int              m_status;

	//与备份进度有关
	unsigned long    m_allTime;	
	unsigned long    m_offSetTime;
	unsigned long    m_oneFileBaseTime;
	unsigned long    m_curOffSetTimeofOneFile;

	//
	bool             m_bMustBackupAudioFormat;


};





















#endif
