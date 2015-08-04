/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       :	Ԭʯά
** Date         : 2008-12-11
** Name         : ReclogEx.h
** Version      : 1.0
** Description  :
** Modify Record:
***********************************************************************/
#ifndef _RECLOG_EX_H_
#define _RECLOG_EX_H_

#include "PUB_common.h"
#include "mylist.cpp"

typedef struct  _reclog_file_head_info
{
	unsigned short	headFlag;			//�ļ�ͷ����ʶ
	unsigned short	version;			//���ļ��İ汾��
	unsigned short	isNormalClose;		//�Ƿ������ر�
	unsigned short	chnnNum;			//��¼��ͨ����Ŀ

	unsigned short	fileRecSecNum;		//¼�����Ŀ
	unsigned short	fileLogNum;			//��������¼����Ŀ������ΪԤ���洢�ռ��С��
	
	unsigned long	fileNum;			//��¼���ļ���Ŀ

	unsigned long	fileInfoOffset;
	unsigned long	sectionOffset;
	unsigned long	reclogoffset;
}RECLOG_FILE_HEAD_INFO;

typedef struct _file_file_info
{
	unsigned short	deviceID;		//�����豸ID
	unsigned short	cameraID;		//�����豸ͨ��ID

	unsigned long	type;			//¼���¼��������
	unsigned long	chnn;			//¼��ͨ��
	unsigned long	startTime;		//��ʼʱ��
	unsigned long	endTime;		//����ʱ��
	unsigned long	status;			//��ʾ�ļ�״̬
}FILE_FILE_INFO;

typedef enum _file_status
{
	FILE_STATUS_NULL,					//δʹ�õ��ļ�
	FILE_STATUS_WRITING		= 0x01,		//����д���ļ�
	FILE_STATUS_END			= 0x02,		//��ʾ�����������ļ�
	FILE_STATUS_UNOFFICIAL	= 0x04,		//��ʾ���ļ��Ƿ���ʽ�ģ������޸�ʱ����ֳ�ͻ��
}FILE_STATUS;

const unsigned long RECLOG_TYPE_MASK	= 0x7fffffff;	//¼�����͵�ģ�壬�����ֻ��31λ���ã����λ������
const unsigned long RECLOG_END			= 0x80000000;

typedef struct _file_reclog_info
{
	unsigned long	type;			//¼���¼����ͣ����λΪ���ʾ��ʼ�����λΪ1��ʾ����
	unsigned long	time;			//��¼ʱ��
}FILE_RECLOG_INFO;

typedef struct _date_info
{
	unsigned long	time;
	CMyList <unsigned short> *pFileList;
}DATE_INFO, *P_DATE_INFO;

typedef struct _channel_use_file_info
{
	unsigned long  channel;
	CMyList <unsigned short> *pFileList;
}CHANNEL_USE_FILE_INFO;

typedef struct _rec_status
{
	unsigned long	type;

	unsigned long	section_index;
	unsigned long	reclog_index;
	unsigned long   remainReclogNum; //���ڼ�¼�����õ��¼�����Ŀ��
}REC_STATUS, *P_REC_STATUS;

const unsigned long	MAX_TIME_OFFSET		= 60*1; //�����޶���ǰʱ��ǰMAX_TIME_OFFSET���ڵ����ݲ��ܱ�����������
const unsigned long MAX_SECTION_NUM		= 80;	//¼��������Ŀ(�޸Ĵ�����ʱע��m_pCleanDataBuf��
const unsigned long MAX_RECLOG_NUM		= 160;	//�¼���¼�����Ŀ,��Ϊһ���¼���Ҫд��ʼ�ͽ���������¼������Ϊ�ε�����(�޸Ĵ�����ʱע��m_pCleanDataBuf����	

const unsigned long FILE_INFO_OFFSET	= sizeof(RECLOG_FILE_HEAD_INFO) + 32;
const unsigned long RECLOG_OFFSET		= 0;

const unsigned long RECLOG_EX_FILE_VER	= 0x0001;

const unsigned long REC_INFO_BUF_LEN	= 1440;
const unsigned char MAX_READ_FILE_NUM	= 10;

typedef CMyList <DATE_INFO> DATE_INFO_LIST;
/*********************************************************************************
�ļ��ṹ��
				�ļ�ͷ����RECLOG_FILE_HEAD_INFO

				����Ϣ����FILE_RECLOG_INFO X MAX_SECTION_NUM
					.
					.		file num
					.
				����Ϣ����FILE_RECLOG_INFO X MAX_SECTION_NUM

				��¼��Ϣ����FILE_RECLOG_INFO X MAX_RECLOG_NUM
					.
					.		file num
					.
				��¼��Ϣ����FILE_RECLOG_INFO X MAX_RECLOG_NUM
*********************************************************************************/

/*********************************************************************************
��Ʊ����һЩ����˼�룺
		1�������һ������ʵ�����ڹ���һ�����̷�����¼���¼���ݿ��ļ���
		2����ʼ��ʱ���������¼��ͨ����Ŀ����һ���ļ��б����飬�����ļ�����ʱ���� ��
		�ķ�ʽ�����б�
		3����ʼ��ʱ����δʹ�õ��ļ�����һ��δ���ļ��б�
		4����ʼ��ʱ��������¼��ʹ�õ��ļ���ͨ����Ӧ����������һ����Ӧ���飬Ԫ����Ŀ
		�����ļ���Ŀ��
		5����¼��ǰ¼��״̬��
		6����ʼ��ʱ�������޸��ļ������ǳ�ʼ����������ṩ��ȡ���޸��ļ����б��ⲿ
		�����ļ��б����޸�¼���ļ��������ݿ�Ĵ�������ÿ���쳣�ļ����࣬���Բ���ȥ���
		�ļ��Ƿ��������쳣��ֱ�Ӱ���ʱ�������������ļ����뷵���б�
		7�����Ҫ���������б���Ҫ�������޸������󣬵�����ؽӿڴ���������Ĭ��û�������б�
		8�������б��м���һ���µĽڵ�ʱ���Ӹ����Ͻ�������һ�μ���Ľڵ�ǳ��ӽ�������
		�޸���list�࣬�����˼�¼�������ڵ��������ʽڵ����ز������ṩ��ȡ�ӿڣ�����
		�Ż��б����Ч�ʣ�
*********************************************************************************/
class CReclogEx
{
public:
	CReclogEx ();
	~CReclogEx ();
	
	//����һ���µ����ݿ��ļ�
	static bool CreateLogFile(const char *pFilePath, unsigned long fileNum, unsigned char videoInputNum ,bool bIsBackUpCreated = false);

	//��ʼ���˳�һ�����е����ݿ��ļ�
	bool Initial (unsigned char diskIndex, const char *pFilePath = NULL);
	void Quit();
	bool CreateDateList();

	//һЩ��ѯ�ķ���
	unsigned long GetDateInfo(CMyList <DVR_DATE> &list);

	unsigned long GetSectionInfo(ULONGLONG chnnbBits, unsigned long startTime, unsigned long endTime, \
								 SECTION_INFO_EX_LIST *pList, unsigned char chnnNum, bool bRepeat, bool bOfficial);

	unsigned long GetFileInfo(unsigned long chnn, unsigned long startTime, unsigned long endTime, \
								FILE_INFO_EX_LIST & list, bool bOnlyRead, bool bRepeat, bool bOfficial);
	unsigned long GetFileInfo(ULONGLONG chnnbBits, unsigned long startTime, unsigned long endTime, \
								FILE_INFO_EX_LIST *pList, unsigned char chnnNum, bool bOnlyRead, bool bRepeat, bool bOfficial);
	unsigned long GetFileInfo(unsigned long chnn, unsigned long time, \
								FILE_INFO_EX_LIST &list, bool bOnlyRead, bool bRepeat, bool bOfficial);

	unsigned long GetReclog(unsigned long chnn, unsigned long type, unsigned long startTime, unsigned long endTime, \
							RECLOG_INFO_EX_LIST &list, bool bOnlyRead, bool bRepeat, bool bOfficial);
	unsigned long GetReclog(ULONGLONG chnnbBits, unsigned long type, unsigned long startTime, unsigned long endTime, \
		                    RECLOG_INFO_EX_LIST *pList, unsigned char chnnNum, bool bOnlyRead, bool bRepeat, bool bOfficial);

	bool          GetInvalidFile (unsigned long chnn, unsigned short &fileIndex);
	bool          FindRecycleFile(FILE_INFO_EX &fileInfo);
	unsigned long GetFreeFileNum();
	unsigned long GetChannelNum(bool bIsBackUpCreated = false);
	bool          IsBeWoring();
	bool          IsBeReading();
	bool          GetOneFileAllInfo(unsigned short fileIndex, unsigned char *pFileHeadInfo, 
		                            unsigned char *pFileSectionInfo, unsigned char *pFileReclogInfo);
	


	//�����ݿ�����޸ĵķ���
	void RegisterRecordFile(unsigned long channel, unsigned short fileIndex);
	void CancelRecordFile(unsigned long channel, unsigned short fileIndex);
	void RegisterReadFile(unsigned long channel, unsigned short fileIndex);
	void CancelReadFile(unsigned long channel, unsigned short fileIndex);
	void GetUnfinishedFile(FILE_INFO_EX_LIST &list);
	bool Repair(const FILE_INFO_EX &  fileInfo, unsigned long endTime);
	bool DeleteFile(const FILE_INFO_EX &  fileInfo);
	bool LockFile(const FILE_INFO_EX &fileInfo, bool bLock);
	bool DeleteOverdueFiles(unsigned long *pHoldTimeD, unsigned long num); //ɾ��endDay֮ǰ���ļ���������һ��

	//�����ݿ���дһ����¼�ķ���
	bool WriteReclogStart(unsigned short fileIndex, unsigned long chnn, RECORD_TYPE type, unsigned long startTime);
	bool WriteReclogEnd(unsigned short fileIndex, unsigned long chnn, RECORD_TYPE type, unsigned long endTime);

	//�Ա��ݵ���ݶ����ݿ���в����ķ�����
	bool OpenLogFileForBackup(char *pFilePath);
	void CloseLogFileForBackup();
	bool WriteFileStartForBackup(unsigned short fileIndex, unsigned long chnn);
	bool WriteFileEndForBackup();
	bool WriteReclogStartForBackup(unsigned short fileIndex, unsigned long chnn, RECORD_TYPE type, unsigned long startTime);
	bool WriteReclogEndForBackup(unsigned short fileIndex, unsigned long chnn, RECORD_TYPE type, unsigned long endTime);
	bool WriteFileInfoForBackup(unsigned long fileIndex, FILE_FILE_INFO *pFileInfo);
	bool WriteSectionForBackup(unsigned long fileIndex, unsigned long chnn, unsigned long time, bool bStart);
	bool WriteTypeForBackup(unsigned long fileIndex, unsigned long chnn, RECORD_TYPE type, unsigned long time, bool bStart);
	bool WriteOneFileAllInfo(unsigned short fileIndex, unsigned char *pFileHeadInfo, 
		                     unsigned char *pFileSectionInfo, unsigned char *pFileReclogInfo);
	bool RepairForBackup(unsigned char *pFileHeadInfo, unsigned char *pFileSectionInfo, 
		                 unsigned char *pFileReclogInfo, FILE_INFO_EX fileInfo);


protected:
private:	
	//��ʼ���˳��õ��ĺ���
	void          AddToFileListForInitial(FILE_INFO_EX & fileInfo, FILE_INFO_EX_LIST & list, bool bTime);
	unsigned long ReadFileInfo (FILE_FILE_INFO *pData, unsigned long bufLen, unsigned long num, unsigned short fileIndex);
	unsigned long AddToDateList(const DVR_DATE &date, CMyList <DVR_DATE> &list);
	void          AddToDateList(unsigned long chnn, unsigned short fileIndex, unsigned long startTime, unsigned long endTime);

	//��ѯ�õ��ĺ���
	POS	GetPrevNodePos(FILE_INFO_EX_LIST &list, unsigned long time);
	POS	GetPrevNodePosEx(FILE_INFO_EX_LIST &list, unsigned long index, unsigned long no);
	POS	GetNodePos(FILE_INFO_EX_LIST &list, unsigned long time,unsigned long endTime=0);
	POS	GetNodePosEx(FILE_INFO_EX_LIST &list, unsigned long index);
#if 0
	POS GetNextNodePos(FILE_INFO_EX_LIST &list, unsigned long time);
	POS	GetNextNodePosEx(FILE_INFO_EX_LIST &list, unsigned long index, unsigned long no);
	POS	GetNodePosEx(FILE_INFO_EX_LIST &list, unsigned long time);
#endif
	void          AddToFileListForSearch(FILE_INFO_EX fileInfo, FILE_INFO_EX_LIST & list, bool bOnlyRead, bool bRepeat);
	unsigned long GetFileList(ULONGLONG chnnbBits, unsigned long startTime, unsigned long endTime, FILE_INFO_EX_LIST & list);
	void          AddFileIndexToFileList(FILE_INFO_EX & fileInfo, FILE_INFO_EX_LIST & list);
	POS           GetReclogPrevNodePos(RECLOG_INFO_EX_LIST &list, unsigned long time);
	unsigned long AddToReclogList(FILE_RECLOG_INFO *pData, unsigned long type, \
		                          unsigned long startTime, unsigned long endTime,
								  RECLOG_INFO_EX_LIST &list, 
								  const FILE_INFO_EX &  fileInfo, bool bOnlyRead, bool bRepeat);
	void          AddToReclogList(RECLOG_INFO_EX reclog, RECLOG_INFO_EX_LIST &list, bool bOnlyRead, bool bRepeat);
	POS	          GetSectionPrevNodePos(SECTION_INFO_EX_LIST &list, unsigned long time);
	void          AddToSectionList(const SECTION_INFO_EX & section, SECTION_INFO_EX_LIST &list, bool bRepeat);
	unsigned long ReadSection (SECTION_INFO_EX *pData, unsigned long bufLen, unsigned long num, unsigned short fileIndex);
	unsigned long ReadReclog (FILE_RECLOG_INFO *pData, unsigned long bufLen, unsigned long num, unsigned short fileIndex);
	bool          IsFileBeUsingForRecord(unsigned long channel, unsigned short fileIndex);
	bool          IsFileBeUsingForRead(unsigned long channel, unsigned short fileIndex);
   
	//�޸����ݿ��õ��ĺ���
	bool DeleteFileInfo(unsigned long fileIndex);
	void PutOutDateList(const FILE_INFO_EX & fileInfo);
	void AddToInvalidFileList(CMyList <unsigned short> & list, unsigned short fileIndex);
 
	//��Ӽ�¼�õ��ĺ���
	bool WriteFileInfo(unsigned long fileIndex, FILE_FILE_INFO *pFileInfo);
	bool WriteSection(unsigned long fileIndex, unsigned long chnn, unsigned long time, bool bStart);
	bool WriteType(unsigned long fileIndex, unsigned long chnn, RECORD_TYPE type, unsigned long time, bool bStart);
	bool WriteFileInfoEnd(unsigned long fileIndex);
	
	void UpdateDateList(unsigned long chnn, unsigned short fileIndex, unsigned long startTime, unsigned long endTime);

	//ͨ�û��Ĺ��ܺ���
	void AddToList(unsigned short index, CMyList <unsigned short> & list);

	//������������ʱ�������ʱ�䣬�����������Ķ��ǷǷ�ʱ��
	static unsigned long m_minTime, m_maxTime;

	//����ķ������
	unsigned char	m_diskIndex;
	
	//·��
	FILE	*m_pFile;
	char	*m_pFilePath;

	//�����Ѿ�ʹ�õ��ļ�
	RECLOG_FILE_HEAD_INFO		m_headInfo;
	FILE_INFO_EX_LIST			*m_pFileInfoList;
	DATE_INFO_LIST				*m_pDateInfoList;

	//δʹ���ļ��б�
	CMyList <unsigned short>	m_invalidFileList;	
	
	//δ����ļ��б�
	CMyList <FILE_INFO_EX>		m_unFinishedFileList;	

	//���ڹ�������¼����ļ�
	REC_STATUS					*m_pRecStatus;
	CHANNEL_USE_FILE_INFO       *m_pChannelRecordFile; //��¼ÿ��ͨ���Ѿ�������¼����ļ���ע�⣺ͬһʱ�̣�һ��ͨ���������ֻ��ʹ��һ���ļ���

	//�������ڶ�ȡ���ļ�
	CHANNEL_USE_FILE_INFO       *m_pChannelReadFile;

	//�Ա��ݵ���ݶ����ݿ���в���ʱ�õ��ı�����
	FILE                        *m_pFileForBackup;
	char                        *m_pFilePathForBackup;
	REC_STATUS                  m_recStatusForBackup;
	RECLOG_FILE_HEAD_INFO		m_headInfoForBackup;
	FILE_INFO_EX                m_fileInfoForBackup;
	
	//һ��������������д��¼ʱ�������ݡ�
	//������ָ��Ļ�����ֻ���������㣬���Զ�������ã����Ƕ�λ��̬��Ա��
	static unsigned char		*m_pCleanDataBuf;
};
#endif //_RECLOG_EX_H_
