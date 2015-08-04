#ifndef __DISPATCHER_MAN_H__
#define __DISPATCHER_MAN_H__

#include "BurnDVD.h"
#include "FileManager.h"
#include "RecChnn.h"
#include "FrameData.h"
#include <list>


using namespace std;

class CDispatcherMan
{
public:
	static CDispatcherMan * Instance();
	virtual ~CDispatcherMan(void);

	bool Initial(unsigned long chnnTotalCnt = 0/*, unsigned long chnnLimitCnt  = 0*/);
	void Quit();

	//test
	void test();

	bool Start();
	void Stop();

	//�޸�ϵͳʱ��ʱ����Ҫ��ͣģ��
	void Active(bool bActive);
	//
	void RecChnnStart(long chnnNum);
	void RecChnnStop(long chnnNum);

	void RecStartAll();
	void RecStopAll();

	//ֹͣ¼�񣨲����Ѿ�¼���������¼�����̣�
	void DisableAllChnn();
	//����¼��ͨ��.Return: 0, spare(Success);1, busy(record or burn)(fail);2, error(fail);
	long ReSetRecChnn(char* p = NULL, int num = 0);

	void WriteData(CFrameData& framedata);
	void RemoveAllDate();

private:
	CDispatcherMan(void);
	static PUB_THREAD_RESULT PUB_THREAD_CALL DataDispatcherThread(void* lpParameter);
	void DataDispatcher();
	void DataProc(CFrameData& framedata);
	void Clear();
	//��ȡͨ��״̬��0, spare;1, busy(record or burn);
	long GetChnnStatus();
	//��������¼��ͨ������ǰ����ͨ��ȫ��ֹͣ¼��
	void SetRecEnable(char* p = NULL, int num = 0);

private:
	CFileManager          *m_pFileManager;
	CRecChnn              *m_pRecChnn;
	CBurnDVD              *m_pBurnDVD;

	unsigned long          m_fileSize;
	unsigned long          m_chnn;
	unsigned long          m_chnnLimit;     //��¼���ͨ�������Ŀ
	bool                   m_bActive;
	std::list<CFrameData*> m_pframeList;

	PUB_thread_t           m_threadID;
	CPUB_Lock              m_lock;
	bool                   m_bCreateThread;
};

#endif
