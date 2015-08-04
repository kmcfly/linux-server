#ifndef _BACKUP_WRITER_H_
#define _BACKUP_WRITER_H_

#include "AVIWriter.h"
#include "RecWriter.h"
#include "AVIWriter.h"
#include "ReclogEx.h"
#include <string>
#include <vector>
using namespace std;

typedef struct dvr_writer
{
	CRecWriter       dvrWriter;
	CReclogEx        writeReclog;
	unsigned short	 fileIndex;
	BITMAPINFOHEADER bmphdr;
	WAVEFORMATEX     wavex;
	bool			 hasOpen;
	LONGLONG         backupFirstFrameTime;
	LONGLONG         backupLastFrameTime;

	FILE_FILE_INFO   fileInfo;
	SECTION_INFO_EX  fileSectionInfo[MAX_SECTION_NUM];
	FILE_RECLOG_INFO fileReclogInfo[MAX_RECLOG_NUM];
	unsigned long    reclogIndex;
}DVRWRITER;


typedef struct _std_writer
{
	CAVIWriter       stdAviWriter;
	LONGLONG	     stdFrmIndex;
	BITMAPINFOHEADER bmphdr;
	WAVEFORMATEX     *pWavex;
	LONGLONG         lasttime;
	bool             hasAudio;
	bool			 hasOpen;
	FILE_FILE_INFO   fileInfo;
	VIDEO_FORMAT     format;
	LONGLONG         firstTime;
	unsigned long preStdVideoIndex;		//ǰһ����֡���
	unsigned long preStdAudioIndex;		//ǰһ��Ƶ֡���
	unsigned long offStdVideoIndex;		//ƫ�Ƶ�����֡��Ŀ
	unsigned long offStdAudioIndex;		//ƫ�Ƶ���Ƶ֡��Ŀ
}STDWRITER;

class CBackupWriter
{
public:
	enum
	{
		BACKUP_WRITER_ERROR_SUC = 0,
		BACKUP_WRITER_ERROR_OPEN_FAIL,
		BACKUP_WRITER_ERROR_WRITER_FAIL,
	};

	//
	CBackupWriter();

	//
	~CBackupWriter();

	//
	//int Start(char *pBackupPath, BACKUP_DATA_TYPE dataType, unsigned char channelNum);
	int Start(char *pBackupPath, BACKUP_DATA_TYPE dataType);

	//
	int WriteFrame(FRAME_INFO_EX &frameInfo);

	//
	void Stop();
 
	void SetAviFileNameVec(vector<std::string> *pAviNameVec);
	
protected:
private:


	//
	int DVRBackup(FRAME_INFO_EX &frameInfo);

	//
	int AVIBackup(FRAME_INFO_EX &frameInfo);

	//
	void CalculatePath(char* pPath, unsigned short fileIndex, bool bFile);

	//
	void CalculateFormat(STDWRITER &stdWriter);

	//
	bool WriteReclogInfo(DVRWRITER &dvrWriter);
	void AddToReclogList(RECLOG_INFO_EX &reclog, RECLOG_INFO_EX_LIST &list);
	POS GetReclogPrevNodePos(RECLOG_INFO_EX_LIST &list, unsigned long time);

	//ת����������Ƶ��ʽ
	void TranslateAudioFormat(unsigned char *pData, unsigned long len);

	//д��Ƶ֡�����ݲ�ͬ�ı����ʽ������д�������ݶ�ʧʱ��Ҫ�����֡
	int WriteAudioFrame(const FRAME_INFO_EX &frameInfo);
   
	DVRWRITER		m_dvrWriter;

	STDWRITER		m_stdWriter;



	char *m_pBackupPath;
	char *m_pBackupPathName;


	BACKUP_DATA_TYPE m_dataType;
	//unsigned char m_channelNum;
	int m_nWidth;
	int m_nHeight;

	vector<std::string> *m_paviNameVec;//avi���ļ����������

};



























#endif

