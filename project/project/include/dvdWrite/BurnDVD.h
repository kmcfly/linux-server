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
	//�¹���Ԥ����
	bool PreDealNewDisc();

private:
	static PUB_THREAD_RESULT PUB_THREAD_CALL BurnThread(void* lpParameter);
	void BurnProcess();
	bool Burn();
	//���²����¹���
	void ChgDisc();
	//��������
	void EjectDisc();
	//ɨ����¼�������صȴ�ʱ��
	long ScanList(bool bQuery = true);
	//������ɾ����¼�ļ�������
	bool SetBurnLink(std::list<FileAttr*>& fileList);
	bool ClearBurnLink();
	//�������淽��ǰ�������
	bool Format();

private:
	std::list<FileAttr*>         m_burnFileList;
	CFileManager                *m_pFileManager;
	CDiscDriveMan               *m_pDiscMan;
	bool                         m_bNewDisc;      //�Ƿ��¹���
	bool                         m_bFormatted;    //�Ƿ��ʽ��
	unsigned long long           m_leaveSpace;    //����ʣ������
	int                          m_discType;      //��������: 19 -> DVD-RW; 26 -> DVD+RW

	PUB_thread_t                 m_threadID;
	CPUB_Lock                    m_lock;
	bool                         m_bCreateThread;

};

#endif
