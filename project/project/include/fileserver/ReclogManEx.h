/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       :袁石维
** Date         : 2008-12-11
** Name         : ReclogManEx.h
** Version      : 1.0
** Description  :
** Modify Record: 
下面的函数GetSectionInfo GetFileInfo GetReclog，有个问题：
用上面的函数进行搜索时，如果搜索到一个文件，而这个文件已经在已经搜到的文件中，
我们就认为这个文件是冗余录像产生的，就不在放到搜索的结果中。
问题就出在：如何判断出这两个文件是一致。现在采取的方法是：这个两个文件的开始时间一致，
就认为是同一个文件。这个判断不严谨。比如：如果修改时间也可以产生两个开始时间相等的文件。
所以先关掉冗余录像功能。
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

	//初始和退出
	bool Initial (DISK_LOG_INFO *pDiskStatus, unsigned char diskNum, bool bSupportRedun, const char * pFilePath = NULL);
	void Quit ();
	void CreateDateList();

	//激活和休眠	
	bool AddDisk(DISK_LOG_INFO *pDiskStatus, unsigned char num);//新增加num个分区。注意：新分区的编号，在以前的基础上向后延续
	                                                            //此函数主要用在格式化新的录像硬盘的时候。
	bool ActionDisk(DISK_LOG_INFO *pDiskStatus, unsigned char num, bool bAction);//从startDiskIndex开始的num个分区，处于激活或休眠状态。
	                                                                             //此函数主要用在格式化已经用于录像的硬盘
	bool ActionAllDisk(bool bAction);//使所有的分区处于激活或休眠状态

	void SetRecycleRecord(bool bEnable);

	//一些查询的方法	
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

	void          GetUsingPartitionMask(ULONGLONG &heightMask, ULONGLONG &lowMask);//获取正在使用的分区掩码。lowMask用于表示第1个到第64个分区的情况。heightMask用于表示第65个到第128个分区的情况
	unsigned long GetDiskFreeSpace(unsigned char startDiskIndex, unsigned char num);//统计从startDiskIndex分区开始的，num个分区的剩余空间
	bool          GetWorkDiskFreeFileNum(unsigned char startDiskIndex, unsigned char num, unsigned long &freeFileNum); ////统计从startDiskIndex分区开始的，num个分区的剩余文件个数，放到freeFileNum中。函数的返回值，代表几个分区是否为本机可录像的硬盘。
	unsigned long GetDiskChannelNum(unsigned char diskIndex , bool bIsBackUpCreated = false);
	bool          CheckDiskAlarm(unsigned long freeFileNum);
    bool          CheckReadDisk();

	

	//向数据库中写一条记录的方法
	bool WriteLogStart(const FILE_INFO_EX &fileInfo);
	bool WriteLogEnd(const FILE_INFO_EX &fileInfo);
	
	//对数据库进行修改的方法
	void RegisterRecordFile(unsigned channel, unsigned short diskIndex, unsigned short fileIndex);
	void CancelRecordFile(unsigned channel, unsigned short diskIndex, unsigned short fileIndex);
	void RegisterReadFile(unsigned channel, unsigned short diskIndex, unsigned short fileIndex);
	void CancelReadFile(unsigned channel, unsigned short diskIndex, unsigned short fileIndex);
	void GetUnfinishedFile(FILE_INFO_EX_LIST &list);
	bool Repair(const FILE_INFO_EX &fileInfo, unsigned long endTime);
	bool DeleteFile(const FILE_INFO_EX &fileInfo);   //只删除fileInfo
	bool DeleteFileEx(const FILE_INFO_EX &fileInfo); //除删除fileInfo文件外，还删除冗余文件
    bool LockFile(const FILE_INFO_EX &fileInfo, bool bLock);
	bool LockFileEx(const FILE_INFO_EX &fileInfo, bool bLock);	
	bool GetOneFileAllInfo(unsigned short diskIndex, unsigned short fileIndex,
		                   unsigned char *pFileHeadInfo, unsigned char *pFileSectionInfo, unsigned char *pFileReclogInfo);
	void SetFileHoldTimeD(const unsigned long *pHoldTimeD, unsigned long num); //设置录像文件保留天数
	bool DeleteOverdueFiles( );
	void SetDiskAlarmSize(unsigned long diskSize);
protected:
private:
	
	//录像文件所在目录，如、/mnt
	char			*m_pFilePath;

	//
	RECLOGEX_STATUS_LIST	m_reclogList;

	//
	unsigned long  *m_pFileHoldTimeD; //录像文件保留天数
	unsigned long  m_FileHoldChnnNum;

	//
	unsigned long m_diskFreeAlarmSizeM; //

	//
	bool          m_bSupportRedunRec; //是否支持冗余录像

	//
	bool          m_bRecycleRecord;

	//
	CPUB_Lock		m_lock;	
};
#endif //_RECLOG_MAN_EX_H_
