#ifndef __FILE_MANAGER_H__
#define __FILE_MANAGER_H__

#define MAX_CHNN_COUNT 32
#define MAX_CHNN_LIMIT 3
#define MAX_SINGLE_FILE_LEN_RECHNN (16*1024*1024)
#define MAX_BURNLIST_EMPTY_WAIT_TIME 5
#define MAX_BURN_FILE_CNT 4
#define	MAP_FILE_PATH_NAME "/nfsdir/mapfile"

#include <list>

#include "PUB_common.h"

#ifndef WIN32
#include <sys/mman.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#endif

using namespace std;
//存储文件属性描述
typedef struct _tvt_file_attr_
{
	char            filePath[128];
	unsigned long   chnnNum;

	unsigned long   recBegTime;
	unsigned long   recEndTime;
	unsigned long   burnBegTime;
	unsigned long   burnEndTime;

	unsigned long   flag;        //文件唯一标识符
}FileAttr;

class CFileManager
{
public:
	CFileManager(void);
	virtual ~CFileManager(void);

	bool Initial(long chnnCnt = 0);
	void Quit();

	//由录像链表移到烧录链表
	void UpdateFileAttr(FileAttr* pFileAttr);
	//烧录链表到录像链表
	void UpdateBatchFilesAttr(std::list<FileAttr*>& fileList);

	bool GetRecFileByChnn(long chnnNum, FileAttr** pFileAttr);
	void GetBurnFileList(std::list<FileAttr*>& fileList);
	long QryRecFileStat();

	//获取通道状态：0, spare;1, busy(record or burn);
	long GetChnnStatus();
	//重置文件属性（并将空闲文件移到录像链表）
	bool ReSetFileAttr(char* p = NULL, int num = 0);

	void Recycle();
private:
	bool CreateFile();

	//test
	static bool Compare(FileAttr*& a, FileAttr*& b);
	void show(std::list<FileAttr*>& filelst);

private:
	FileAttr                 m_fileVector[MAX_CHNN_COUNT * 2];
	int                      m_fd[MAX_CHNN_COUNT * 2];
	std::list<FileAttr*>     m_burnList;
	std::list<FileAttr*>     m_recList;
	void                    *m_pmapFile[MAX_CHNN_COUNT * 2];
	unsigned long            m_chnnMaxCnt;       //最大可录制的通道数
	unsigned long            m_chnnActualCnt;  //实际使用的录制通道数
	CPUB_Lock                m_lock;
};

#endif
