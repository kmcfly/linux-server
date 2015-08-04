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

	//���ȡ��Ƶ�ļ��Ͷ�ȡ��־�ļ��й�
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

	//��ȡ�ļ�������
	LONGLONG         m_startTime;
	LONGLONG         m_endTime;
	BACKUP_DATA_TYPE m_dataType;

	//���ڱ�ʾ��ȡ״̬�ı仯
	int              m_status;

	//�뱸�ݽ����й�
	unsigned long    m_allTime;	
	unsigned long    m_offSetTime;
	unsigned long    m_oneFileBaseTime;
	unsigned long    m_curOffSetTimeofOneFile;

	//
	bool             m_bMustBackupAudioFormat;


};





















#endif
