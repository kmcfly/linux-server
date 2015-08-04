/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : lgj
** Date         : 2010-09-07
** Name         : SnapMan..h
** Version      : 1.0
** Description  : �����û�ץͼ��
** Modify Record:
1:
***********************************************************************/
#ifndef __CSNAPMAN_H__
#define __CSNAPMAN_H__
#include "PUB_common.h"
#include "mylist.cpp"


//#define  MAX_PICTURE_IN_ONE_FILE (4000)
#if defined(__CHIP3531__) || defined(__DVR_ULTIMATE__) || defined(__CHIPGM__) 
	#if defined(__SUPPORT_SNAPD1__)		//3531D1ץͼ��16·ʹ�õ��ڴ��СΪ6M��32·ʹ�õ��ڴ��СΪ12M
		#define  MAX_PICTURE_SIZE        (256*1024)
		#define  MAX_DELETE_OLD          (100)
	#elif defined(__SUPPORT_HONGDI__)	//3531SDIץͼ��16·ʹ�õ��ڴ��СΪ32M��32·ʹ�õ��ڴ��СΪ64M
		#define  MAX_PICTURE_SIZE        (512*1024)
		#define  MAX_DELETE_OLD          (100)
	#else								//����3531��֧��d1ץͼ��16·ʹ�õ��ڴ��СΪ6M��32·ʹ�õ��ڴ��СΪ12M
		#define  MAX_PICTURE_SIZE        (256*1024)
		#define  MAX_DELETE_OLD          (100)
	#endif
#else
#define  MAX_PICTURE_SIZE        (20*1024)
#define  MAX_DELETE_OLD          (100)
#endif
typedef enum _sort_type
{
	SORT_BY_TIME,     //Ĭ������ʱ��
	SORT_BY_CHANNEL,
	SORT_BY_NAME,
	SORT_BY_TRIG_TYPE
}SORT_TYPE;

typedef enum _send_type
{
	SEND_TYPE_NET,
	SEND_TYPE_MAIL
}SEND_TYPE;
typedef struct _picture_man_info
{
	char szVersion[16];
	unsigned int nFileNum;
	unsigned int nMaxPct;
	char szReserve[12];
}PICTURE_MAN_INFO, *PPICTURE_MAN_INFO;

typedef struct _picture_info
{
	LONGLONG       time;
	unsigned short nChannel;   //ͨ����
	unsigned short nType;      //��������     //�����һλ����ʾ�Ƿ�����ֻ����Ӳ�̡�ֻ���ڲ�ѯʱ����Ч
	int            nLen;	   //buf����
	bool           bLock;      //�Ƿ�����
	bool           bFill;      //�Ƿ������

}PICTURE_INFO, *PPICTURE_INFO;

typedef struct _snap_file_info_base
{
	unsigned int nFileName;
	unsigned int nFill;
}SNAP_FILE_BASE;

typedef struct _snap_log_info
{
	SNAP_FILE_BASE fileBase;
	PPICTURE_INFO pstPctInfo; 
}SNAP_LOG_INFO;

typedef struct _snap_disk_info
{
	DISK_LOG_INFO  stDisk;
	SNAP_LOG_INFO  *pLogInfo;
	bool           bAction;
	unsigned int   nLogNum;
	unsigned int   nMaxPct;
}SNAP_DISK_INFO;

typedef struct _out_picture_info
{
	unsigned char diskIndex;
	unsigned int  nLogPos;
	unsigned int  nPos;
	PICTURE_INFO  attribute;
}OUT_PICTURE_INFO;

typedef CMyList<OUT_PICTURE_INFO> OUT_PICTURE_INFO_LIST;

class CSnapMan
{
public:
	
	~CSnapMan(void);

	static CSnapMan *GetInstance();

	//�ļ�·������Log�����ļ������������ļ�����ʼ�š�
	static bool CreateLogFile(const char *pFilePath, unsigned long fileNum, unsigned long fileBegineNum, int nMaxPic = 4000);

	bool Initial(DISK_LOG_INFO *pDiskStatus, int diskNum, const char * pFilePath);
	void Quit();

	bool AddDisk(DISK_LOG_INFO *pDiskStatus, unsigned char diskNum);

	bool ActionDisk(DISK_LOG_INFO *pDiskStatus, unsigned char num, bool bAction);
	bool ActionAllDisk(bool bAction);
	
	//��ʼ��,�������̻�·����������ʼ���̺ţ�����������Ŀ¼����С��
	//bool InitialSanp(char *basePath, int baseNum, int subAreaNum, int subAreaSize);

	bool IsCanSavePicture();
	//����ץͼ������,���ӡ�
	bool SaveSnapPicture(void *buf, int nLen, int type, int nChannel, LONGLONG time, unsigned short width, unsigned short height, bool bLock = false);

	//�õ�ͼƬ�ܸ���
	int GetPictureNum();

	bool GetDiskPictureNum(unsigned char diskIndex, unsigned long &allNum, unsigned long &usedNum);
	

	//�õ�����ͼƬ��
	bool GetAllPicture(OUT_PICTURE_INFO_LIST &list);

	bool GetPicture(unsigned long chnn, unsigned long type, LONGLONG startTime, \
		LONGLONG endTime, OUT_PICTURE_INFO_LIST &list);

	bool GetPicture(unsigned long *chnns, int chnnNum, unsigned long *types, int typeNum, LONGLONG startTime,\
		LONGLONG endTime, OUT_PICTURE_INFO_LIST &list, unsigned long diskOwnBy);

	//ɾ��ͼƬ��
	bool DeleteOne(OUT_PICTURE_INFO& deleteInfo);
	bool DeleteAll();
	bool DeleteOneDiskPicture(unsigned char diskIndex);
	bool DeleteOneDisk(unsigned char diskIndex);
	
	//����
	//bool SortPicture(SORT_TYPE sortType, void * result);

	//�޸����ơ�
	//bool Rename(char * oldPath, char * newName);

	bool SendPicture(OUT_PICTURE_INFO& node, SEND_TYPE type);

	bool SetLock(OUT_PICTURE_INFO& node, bool bLock);

	bool SetLockStatus(OUT_PICTURE_INFO& node, bool bLock);
	bool SetLockStatusBatch(OUT_PICTURE_INFO_LIST& outList, bool bLock);

	bool DeleteDiskLogData(SNAP_DISK_INFO &diskNode);

	bool GetPictureData(SNAP_PIC_PRORITY picNode, unsigned char *buf, int &picWidth, int &picHeight, int &picLen);
	bool GetPictureData(OUT_PICTURE_INFO &node, unsigned char *buf);
	bool WritePictureToFile(SNAP_PIC_PRORITY picNode, FILE *pFile);

	//��.dat�ļ��е���jpeg�ļ���
	bool SaveAllAsJpeg(char * ForldPath,  int nGet = 0);

	bool SaveBatchAsJpeg(char * ForldPath, OUT_PICTURE_INFO_LIST &list);

	bool GetUsingPartitionMask(ULONGLONG heightMask, ULONGLONG lowMask);

private:
	CSnapMan(void);
	bool SaveBufTofile(void*buf, int nLen, int type, int nChannel,\
			bool bLock, int nDiskIndex, int nFilePos, int nPos, LONGLONG time, unsigned short width, unsigned short height);

	bool DeleteTheOldestFromFile(int *pDiskIndex, int *pFileNum, int *pPicNum);
	bool DeleteNOldest(int *pDiskIndex, int *pFileNum, int *pPicNum);
	bool DeleteDiskLogDataProc(SNAP_DISK_INFO &diskNode);
	bool OnBatchDelete(OUT_PICTURE_INFO_LIST &list);

	bool GetOneEmptyPos(int*pnDiskIndex, int *nLogPos, int *nPos);

	int GetOneFileSize(char *fileName);

	bool UpdateSnapLogFile(int nDiskIndex, SNAP_LOG_INFO* pLogInfo, unsigned int nNum, unsigned int nOffSet, unsigned int nMaxPic);

	bool AddToListBySort(OUT_PICTURE_INFO_LIST &list, OUT_PICTURE_INFO& node, SORT_TYPE sort);

	bool IsInType(unsigned long *types,int typeNum, unsigned long type);

	bool IsInChannels(unsigned long *chnns,int chnnNum, unsigned long chann);

	bool InsertToDeleteList(int index, int nFilePos, int nPos, LONGLONG time, OUT_PICTURE_INFO_LIST &list);

	bool WriteJpegFile(FILE *fp, int nPos, int nLen, char * ForldPath, int i, int nchannel, LONGLONG time);

	CMyList<SNAP_DISK_INFO>m_stDiskList;
	char m_szBasePath[128];
	OUT_PICTURE_INFO m_stLastInsert;

	CPUB_Lock  m_lock;

	static CSnapMan   *m_pSnapMan;
};

#endif

