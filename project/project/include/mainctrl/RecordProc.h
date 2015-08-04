/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : Ԭʯά
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
	RECORD_MODE_MAIN       = 0x01,  //��¼��ģʽ���õ�0λ��ʾ
	RECORD_MODE_REDUNDANCY = 0x02,  //����¼��ģʽ���õ�1λ��ʾ
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
	unsigned long startType; //���ڼ�¼��ǰ¼����ļ�������¼����Щ���͵����ݣ����磺����¼���ƶ����ֶ�¼��
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
	unsigned long startType; //���ڼ�¼��ǰ¼����ļ�������¼����Щ���͵����ݣ����磺����¼���ƶ����ֶ�¼��
}REDU_RECORD_WRITER;


typedef enum _record_write_status
{
	RECORD_WRITE_SUCC,			//д��ɹ�
	RECORD_WRITE_NO_FILE,		//���ļ���д
	RECORD_WRITE_NO_RECORDING,	//��¼��״̬
	RECORD_WRITE_NO_BITMAPINFO,	//��bitmap��Ϣ
	RECORD_WRITE_NO_WAVEFORMAT,	//��wave format��Ϣ
	RECORD_WRITE_FAIL,			//д��ʧ��
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
	//��reclogManע���ע��¼���ļ�������
	void RegisterFileIndex(FILE_INFO_EX &fileInfo);
	void CancelFileIndex(FILE_INFO_EX &fileInfo); //CancelFileIndexһ��Ҫ�ŵ�WriteReclogEnd������á�
	                                              //ԭ��WriteReclogEnd����д���Ƕν�����Ϣ��CancelFileIndexд�����ļ�������Ϣ��
	                                              //Ҫ��д�����Ϣ֮����д�ļ�����

	//�����ͨ��
	unsigned long        m_channel;

	//������־
	CReclogManEX		*m_pReclogManEx;
	CMessageMan			*m_pMsgMan;

	//�ļ����·��
	char				*m_pFilePath;

	//¼�����ͣ����ֶ�¼�񣬶�ʱ¼��
	unsigned long       m_startType;
	unsigned long       m_currType;

    //¼��ģʽ����¼�񣨼�������¼�񣩻�������¼�񣬻������߹��档
	bool                m_bRecycleRecord;      //�Ƿ������ˣ���������
    RECORD_WRITER       m_mainRecWriter;       //������¼��
	REDU_RECORD_WRITER  m_reduRecWriter;       //��������¼��

	//����Ԥ¼��
	CPreRecord			m_preDiskRecord;
	CPreMemRecord       m_preMemRecord;

    //�����ʽ����
	BITMAPINFOHEADER	*m_pBitmapInfo;
	WAVEFORMATEX		*m_pWaveFormat;

	//
	LONGLONG			m_lastFrameTime;

	unsigned long       m_preRecordType;

	unsigned long m_actualPrimaryStreamChnnNum; //ʵ�ʵı���ͨ��������ͨ����������Ŀ
};
#endif //_RECORD_PROC_H_

