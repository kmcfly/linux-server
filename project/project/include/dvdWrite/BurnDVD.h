#ifndef __BURN_DVD_H__
#define __BURN_DVD_H__

#define LINK_FILE_DIR "/nfsdir/burn/"
#include "FileManager.h"
#include "DiscDriveMan.h"
#ifndef WIN32
#include <stdlib.h>
#endif

class CBurnDVD
{
public:
	CBurnDVD(void);
	virtual ~CBurnDVD(void);

	bool Initial(CFileManager* pFileManager, unsigned long timeWait = 0);
	void Quit();

	bool Start();
	void Stop();

	//
	bool InitialDriver();
	//新光盘预处理
	bool PreDealNewDisc();

private:
	static PUB_THREAD_RESULT PUB_THREAD_CALL BurnThread(void* lpParameter);
	void BurnProcess();
	bool Burn();
	//重新插入新光盘
	void ChgDisc();
	//弹出光盘
	void EjectDisc();
	//扫描烧录链表，返回等待时间
	long ScanList(bool bQuery = true);
	//设置与删除烧录文件软链接
	bool SetBurnLink(std::list<FileAttr*>& fileList);
	bool ClearBurnLink();
	//调用下面方法前，需加锁
	bool Format();

private:
	std::list<FileAttr*>         m_burnFileList;
	CFileManager                *m_pFileManager;
	CDiscDriveMan               *m_pDiscMan;
	bool                         m_bNewDisc;      //是否新光盘
	bool                         m_bFormatted;    //是否格式过
	unsigned long long           m_leaveSpace;    //光盘剩余容量
	int                          m_discType;      //光盘类型: 19 -> DVD-RW; 26 -> DVD+RW

	PUB_thread_t                 m_threadID;
	CPUB_Lock                    m_lock;
	bool                         m_bCreateThread;

};

#endif
