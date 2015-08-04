/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       :	袁石维
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
	unsigned short	headFlag;			//文件头部标识
	unsigned short	version;			//该文件的版本号
	unsigned short	isNormalClose;		//是否正常关闭
	unsigned short	chnnNum;			//记录的通道数目

	unsigned short	fileRecSecNum;		//录像段数目
	unsigned short	fileLogNum;			//四种类型录像数目（以上为预留存储空间大小）
	
	unsigned long	fileNum;			//记录的文件数目

	unsigned long	fileInfoOffset;
	unsigned long	sectionOffset;
	unsigned long	reclogoffset;
}RECLOG_FILE_HEAD_INFO;

typedef struct _file_file_info
{
	unsigned short	deviceID;		//网络设备ID
	unsigned short	cameraID;		//网络设备通道ID

	unsigned long	type;			//录像事件类型组合
	unsigned long	chnn;			//录像通道
	unsigned long	startTime;		//开始时间
	unsigned long	endTime;		//结束时间
	unsigned long	status;			//表示文件状态
}FILE_FILE_INFO;

typedef enum _file_status
{
	FILE_STATUS_NULL,					//未使用的文件
	FILE_STATUS_WRITING		= 0x01,		//正在写的文件
	FILE_STATUS_END			= 0x02,		//表示正常结束的文件
	FILE_STATUS_UNOFFICIAL	= 0x04,		//表示该文件是非正式的，即是修改时间后又冲突的
}FILE_STATUS;

const unsigned long RECLOG_TYPE_MASK	= 0x7fffffff;	//录像类型的模板，即最多只有31位可用，最高位不能用
const unsigned long RECLOG_END			= 0x80000000;

typedef struct _file_reclog_info
{
	unsigned long	type;			//录像事件类型：最高位为零表示开始，最高位为1表示结束
	unsigned long	time;			//记录时间
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
	unsigned long   remainReclogNum; //用于记录，可用的事件段数目。
}REC_STATUS, *P_REC_STATUS;

const unsigned long	MAX_TIME_OFFSET		= 60*1; //用来限定当前时间前MAX_TIME_OFFSET秒内的数据不能被搜索出来。
const unsigned long MAX_SECTION_NUM		= 80;	//录像段最大数目(修改此数字时注意m_pCleanDataBuf）
const unsigned long MAX_RECLOG_NUM		= 160;	//事件记录最大数目,因为一个事件需要写开始和结束两条记录，所以为段的两倍(修改此数字时注意m_pCleanDataBuf）。	

const unsigned long FILE_INFO_OFFSET	= sizeof(RECLOG_FILE_HEAD_INFO) + 32;
const unsigned long RECLOG_OFFSET		= 0;

const unsigned long RECLOG_EX_FILE_VER	= 0x0001;

const unsigned long REC_INFO_BUF_LEN	= 1440;
const unsigned char MAX_READ_FILE_NUM	= 10;

typedef CMyList <DATE_INFO> DATE_INFO_LIST;
/*********************************************************************************
文件结构：
				文件头——RECLOG_FILE_HEAD_INFO

				段信息——FILE_RECLOG_INFO X MAX_SECTION_NUM
					.
					.		file num
					.
				段信息——FILE_RECLOG_INFO X MAX_SECTION_NUM

				记录信息——FILE_RECLOG_INFO X MAX_RECLOG_NUM
					.
					.		file num
					.
				记录信息——FILE_RECLOG_INFO X MAX_RECLOG_NUM
*********************************************************************************/

/*********************************************************************************
设计本类的一些基本思想：
		1、本类的一个对象实例用于管理一个磁盘分区的录像记录数据库文件；
		2、初始化时，按照其记录的通道数目建立一个文件列表数组，并把文件按照时间升 序
		的方式加入列表；
		3、初始化时，把未使用的文件加入一个未用文件列表；
		4、初始化时，把正在录像使用的文件和通道对应起来，建立一个对应数组，元素数目
		等于文件数目；
		5、记录当前录像状态；
		6、初始化时，不再修复文件，而是初始化完后，向外提供获取需修复文件的列表，外部
		根据文件列表做修复录像文件及本数据库的处理。由于每次异常文件不多，所以不再去检查
		文件是否真正有异常，直接把在时间上排在最后的文件加入返回列表；
		7、如果要建立日期列表，需要在做了修复动作后，调用相关接口创建，否则默认没有日期列表；
		8、当往列表中加入一个新的节点时，从概率上讲其和最近一次加入的节点非常接近，所以
		修改了list类，增加了记录最近插入节点和最近访问节点的相关参数并提供获取接口，用于
		优化列表操作效率；
*********************************************************************************/
class CReclogEx
{
public:
	CReclogEx ();
	~CReclogEx ();
	
	//创建一个新的数据库文件
	static bool CreateLogFile(const char *pFilePath, unsigned long fileNum, unsigned char videoInputNum ,bool bIsBackUpCreated = false);

	//初始和退出一个已有的数据库文件
	bool Initial (unsigned char diskIndex, const char *pFilePath = NULL);
	void Quit();
	bool CreateDateList();

	//一些查询的方法
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
	


	//对数据库进行修改的方法
	void RegisterRecordFile(unsigned long channel, unsigned short fileIndex);
	void CancelRecordFile(unsigned long channel, unsigned short fileIndex);
	void RegisterReadFile(unsigned long channel, unsigned short fileIndex);
	void CancelReadFile(unsigned long channel, unsigned short fileIndex);
	void GetUnfinishedFile(FILE_INFO_EX_LIST &list);
	bool Repair(const FILE_INFO_EX &  fileInfo, unsigned long endTime);
	bool DeleteFile(const FILE_INFO_EX &  fileInfo);
	bool LockFile(const FILE_INFO_EX &fileInfo, bool bLock);
	bool DeleteOverdueFiles(unsigned long *pHoldTimeD, unsigned long num); //删除endDay之前的文件，包括这一天

	//向数据库中写一条记录的方法
	bool WriteReclogStart(unsigned short fileIndex, unsigned long chnn, RECORD_TYPE type, unsigned long startTime);
	bool WriteReclogEnd(unsigned short fileIndex, unsigned long chnn, RECORD_TYPE type, unsigned long endTime);

	//以备份的身份对数据库进行操作的方法。
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
	//初始和退出用到的函数
	void          AddToFileListForInitial(FILE_INFO_EX & fileInfo, FILE_INFO_EX_LIST & list, bool bTime);
	unsigned long ReadFileInfo (FILE_FILE_INFO *pData, unsigned long bufLen, unsigned long num, unsigned short fileIndex);
	unsigned long AddToDateList(const DVR_DATE &date, CMyList <DVR_DATE> &list);
	void          AddToDateList(unsigned long chnn, unsigned short fileIndex, unsigned long startTime, unsigned long endTime);

	//查询用到的函数
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
   
	//修改数据库用到的函数
	bool DeleteFileInfo(unsigned long fileIndex);
	void PutOutDateList(const FILE_INFO_EX & fileInfo);
	void AddToInvalidFileList(CMyList <unsigned short> & list, unsigned short fileIndex);
 
	//添加记录用到的函数
	bool WriteFileInfo(unsigned long fileIndex, FILE_FILE_INFO *pFileInfo);
	bool WriteSection(unsigned long fileIndex, unsigned long chnn, unsigned long time, bool bStart);
	bool WriteType(unsigned long fileIndex, unsigned long chnn, RECORD_TYPE type, unsigned long time, bool bStart);
	bool WriteFileInfoEnd(unsigned long fileIndex);
	
	void UpdateDateList(unsigned long chnn, unsigned short fileIndex, unsigned long startTime, unsigned long endTime);

	//通用化的功能函数
	void AddToList(unsigned short index, CMyList <unsigned short> & list);

	//用来定义最早时间和最晚时间，不再这个区间的都是非法时间
	static unsigned long m_minTime, m_maxTime;

	//负责的分区序号
	unsigned char	m_diskIndex;
	
	//路径
	FILE	*m_pFile;
	char	*m_pFilePath;

	//管理已经使用的文件
	RECLOG_FILE_HEAD_INFO		m_headInfo;
	FILE_INFO_EX_LIST			*m_pFileInfoList;
	DATE_INFO_LIST				*m_pDateInfoList;

	//未使用文件列表
	CMyList <unsigned short>	m_invalidFileList;	
	
	//未完成文件列表
	CMyList <FILE_INFO_EX>		m_unFinishedFileList;	

	//用于管理正在录像的文件
	REC_STATUS					*m_pRecStatus;
	CHANNEL_USE_FILE_INFO       *m_pChannelRecordFile; //记录每个通道已经打开用于录像的文件。注意：同一时刻，一个通道这个分区只能使用一个文件。

	//管理正在读取的文件
	CHANNEL_USE_FILE_INFO       *m_pChannelReadFile;

	//以备份的身份对数据库进行操作时用到的变量。
	FILE                        *m_pFileForBackup;
	char                        *m_pFilePathForBackup;
	REC_STATUS                  m_recStatusForBackup;
	RECLOG_FILE_HEAD_INFO		m_headInfoForBackup;
	FILE_INFO_EX                m_fileInfoForBackup;
	
	//一个缓冲区，用于写记录时清零数据。
	//由于它指向的缓冲区只是用来清零，所以多个对象共用，于是定位静态成员。
	static unsigned char		*m_pCleanDataBuf;
};
#endif //_RECLOG_EX_H_
