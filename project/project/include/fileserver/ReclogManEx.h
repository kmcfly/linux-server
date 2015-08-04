/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       :Ԭʯά
** Date         : 2008-12-11
** Name         : ReclogManEx.h
** Version      : 1.0
** Description  :
** Modify Record: 
����ĺ���GetSectionInfo GetFileInfo GetReclog���и����⣺
������ĺ�����������ʱ�����������һ���ļ���������ļ��Ѿ����Ѿ��ѵ����ļ��У�
���Ǿ���Ϊ����ļ�������¼������ģ��Ͳ��ڷŵ������Ľ���С�
����ͳ��ڣ�����жϳ��������ļ���һ�¡����ڲ�ȡ�ķ����ǣ���������ļ��Ŀ�ʼʱ��һ�£�
����Ϊ��ͬһ���ļ�������жϲ��Ͻ������磺����޸�ʱ��Ҳ���Բ���������ʼʱ����ȵ��ļ���
�����ȹص�����¼���ܡ�
***********************************************************************/
#ifndef _RECLOG_MAN_EX_H_
#define _RECLOG_MAN_EX_H_

#include "ReclogEx.h"
//
//typedef struct _reclogex_disk_status 
//{
//	unsigned char diskOwnedBy;
//	unsigned char diskIndex;
//	unsigned char diskProperty;
//}RECLOGEX_DISK_STATUS;

typedef struct _reclogex_status
{
	bool           bAction;
	unsigned char  diskOwnedBy;
	unsigned char  diskIndex;
	unsigned char  diskProperty;
	CReclogEx      *pReclog;
}RECLOGEX_STATUS, *PRECLOGEX_STATUS;

typedef CMyList<RECLOGEX_STATUS> RECLOGEX_STATUS_LIST;

class CReclogManEX
{
public:
	CReclogManEX ();
	~CReclogManEX ();

	//��ʼ���˳�
	bool Initial (DISK_LOG_INFO *pDiskStatus, unsigned char diskNum, bool bSupportRedun, const char * pFilePath = NULL);
	void Quit ();
	void CreateDateList();

	//���������	
	bool AddDisk(DISK_LOG_INFO *pDiskStatus, unsigned char num);//������num��������ע�⣺�·����ı�ţ�����ǰ�Ļ������������
	                                                            //�˺�����Ҫ���ڸ�ʽ���µ�¼��Ӳ�̵�ʱ��
	bool ActionDisk(DISK_LOG_INFO *pDiskStatus, unsigned char num, bool bAction);//��startDiskIndex��ʼ��num�����������ڼ��������״̬��
	                                                                             //�˺�����Ҫ���ڸ�ʽ���Ѿ�����¼���Ӳ��
	bool ActionAllDisk(bool bAction);//ʹ���еķ������ڼ��������״̬

	void SetRecycleRecord(bool bEnable);

	//һЩ��ѯ�ķ���	
	bool          GetInvalidFile (unsigned long chnn, unsigned short &diskIndex, unsigned short &fileIndex, unsigned char fromeType);
	bool          FindRecycleFile(FILE_INFO_EX &fileInfo, unsigned char fromeType);
	
	unsigned long GetSectionInfo(ULONGLONG chnnbBits, unsigned long startTime, unsigned long endTime, \
		                         SECTION_INFO_EX_LIST *pList, unsigned char chnnNum, unsigned char diskOwnedBy, bool bOfficial);

	unsigned long GetFileInfo(unsigned long chnn, unsigned long startTime, unsigned long endTime,\
		FILE_INFO_EX_LIST & list, unsigned char diskOwnedBy, bool bOfficial);
	unsigned long GetFileInfo(ULONGLONG chnnbBits, unsigned long startTime, unsigned long endTime, \
		                      FILE_INFO_EX_LIST *pList, unsigned char chnnNum, unsigned char diskOwnedBy, bool bOfficial);

	unsigned long GetReclog(unsigned long chnn, unsigned long type, unsigned long startTime, unsigned long endTime, \
		RECLOG_INFO_EX_LIST &list, unsigned char diskOwnedBy, bool bOfficial);
	unsigned long GetReclog(ULONGLONG chnnbBits, unsigned long type, unsigned long startTime, unsigned long endTime, 
		                    RECLOG_INFO_EX_LIST *pList, unsigned char chnnNum, unsigned char diskOwnedBy, bool bOfficial);

	unsigned long GetDateInfo(CMyList <DVR_DATE> &list, unsigned char diskOwnedBy);
	unsigned long GetFileInfo(unsigned long chnn, unsigned long time,\
		FILE_INFO_EX_LIST &list, unsigned char diskOwnedBy, bool bOfficial);

	void          GetUsingPartitionMask(ULONGLONG &heightMask, ULONGLONG &lowMask);//��ȡ����ʹ�õķ������롣lowMask���ڱ�ʾ��1������64�������������heightMask���ڱ�ʾ��65������128�����������
	unsigned long GetDiskFreeSpace(unsigned char startDiskIndex, unsigned char num);//ͳ�ƴ�startDiskIndex������ʼ�ģ�num��������ʣ��ռ�
	bool          GetWorkDiskFreeFileNum(unsigned char startDiskIndex, unsigned char num, unsigned long &freeFileNum); ////ͳ�ƴ�startDiskIndex������ʼ�ģ�num��������ʣ���ļ��������ŵ�freeFileNum�С������ķ���ֵ�������������Ƿ�Ϊ������¼���Ӳ�̡�
	unsigned long GetDiskChannelNum(unsigned char diskIndex , bool bIsBackUpCreated = false);
	bool          CheckDiskAlarm(unsigned long freeFileNum);
    bool          CheckReadDisk();

	

	//�����ݿ���дһ����¼�ķ���
	bool WriteLogStart(const FILE_INFO_EX &fileInfo);
	bool WriteLogEnd(const FILE_INFO_EX &fileInfo);
	
	//�����ݿ�����޸ĵķ���
	void RegisterRecordFile(unsigned channel, unsigned short diskIndex, unsigned short fileIndex);
	void CancelRecordFile(unsigned channel, unsigned short diskIndex, unsigned short fileIndex);
	void RegisterReadFile(unsigned channel, unsigned short diskIndex, unsigned short fileIndex);
	void CancelReadFile(unsigned channel, unsigned short diskIndex, unsigned short fileIndex);
	void GetUnfinishedFile(FILE_INFO_EX_LIST &list);
	bool Repair(const FILE_INFO_EX &fileInfo, unsigned long endTime);
	bool DeleteFile(const FILE_INFO_EX &fileInfo);   //ֻɾ��fileInfo
	bool DeleteFileEx(const FILE_INFO_EX &fileInfo); //��ɾ��fileInfo�ļ��⣬��ɾ�������ļ�
    bool LockFile(const FILE_INFO_EX &fileInfo, bool bLock);
	bool LockFileEx(const FILE_INFO_EX &fileInfo, bool bLock);	
	bool GetOneFileAllInfo(unsigned short diskIndex, unsigned short fileIndex,
		                   unsigned char *pFileHeadInfo, unsigned char *pFileSectionInfo, unsigned char *pFileReclogInfo);
	void SetFileHoldTimeD(const unsigned long *pHoldTimeD, unsigned long num); //����¼���ļ���������
	bool DeleteOverdueFiles( );
	void SetDiskAlarmSize(unsigned long diskSize);
protected:
private:
	
	//¼���ļ�����Ŀ¼���硢/mnt
	char			*m_pFilePath;

	//
	RECLOGEX_STATUS_LIST	m_reclogList;

	//
	unsigned long  *m_pFileHoldTimeD; //¼���ļ���������
	unsigned long  m_FileHoldChnnNum;

	//
	unsigned long m_diskFreeAlarmSizeM; //

	//
	bool          m_bSupportRedunRec; //�Ƿ�֧������¼��

	//
	bool          m_bRecycleRecord;

	//
	CPUB_Lock		m_lock;	
};
#endif //_RECLOG_MAN_EX_H_
