#ifndef _BACKUP_CUT_WRITER_H_
#define _BACKUP_CUT_WRITER_H_

#include "RecWriter.h"

typedef struct dvr_cut_writer
{
	CRecWriter       dvrWriter;
	unsigned short	 fileIndex;
	BITMAPINFOHEADER bmphdr;
	WAVEFORMATEX     wavex;
	bool			 hasOpen;
	bool             bHasWriteVideoFormat;
	bool             bHasWriteAudioFormat;
	LONGLONG         VideoFrameTime;
	LONGLONG         firstWriteFrameTime;
	LONGLONG         lastWriteFrameTime;
	LONGLONG         firstBackupFrameTime;
	int              backupFileIndex;
}DVRCUTWRITER;

class CBackupCutWriter
{
public:
	enum
	{
		BACKUP_CUT_WRITER_ERROR_SUC = 0,
		BACKUP_CUT_WRITER_ERROR_OPEN_FAIL,
		BACKUP_CUT_WRITER_ERROR_WRITER_FAIL,
	};

	//
	CBackupCutWriter();

	//
	~CBackupCutWriter();

	//
	int Start(char *pBackupPath, REC_FILE_EVENT_INFO *pRecFileEventInfo, unsigned long num);

	//
	int WriteFrame(FRAME_INFO_EX &frameInfo);

	//
	void WriteFileEventInfo();

	//
	void Stop();

	
protected:
private:


	//
	int DVRBackup(FRAME_INFO_EX &frameInfo);


	//
	void CalculatePath(char* pPath, unsigned short fileIndex, bool bFile);


   
	DVRCUTWRITER		m_dvrWriter;



	char *m_pBackupPath;
	char *m_pBackupPathName;


	//unsigned char m_channelNum;
	int m_nWidth;
	int m_nHeight;

	LONGLONG m_startWriteFrameTime;

	REC_FILE_EVENT_INFO *m_pRecFileEventInfo;
	unsigned long        m_eventInfoNum;


};



























#endif

