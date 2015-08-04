#ifndef _RECORD_TASK_H
#define _RECORD_TASK_H

#include "scsi_dev.h"
#include "fifo_buf.h"
#include "rec_file.h"
#include "PUB_common.h"
#include <map>
#include <vector>
using namespace std;

typedef map<uint64_t, CRecord_file*> MAP_RECORD_FILE;
typedef struct 
{
	CSCSI_Dev * dev;		//��Ӧ�Ĵ����豸
	PUB_thread_t thd_id;	//�߳�ID
	bool      thd_start;	//�߳̿���
	bool wait;
	int replay_num;			//�طŵ��ļ�����
	uint64_t	full_lba;	//��ǰ�����Ѿ����˵�LBA
	MAP_RECORD_FILE mp_rec;;//¼���ļ���
	CPUB_Lock	devRecLock;
}dev_rec_s,*pdev_rec_s;

typedef std::vector<dev_rec_s*>  VEC_DEV_REC; //�豸�б�

class CRecord_task
{

protected:

	VEC_DEV_REC m_vec_task;		//

public:
	int AddRecordTask(CSCSI_Dev* pDev,uint64_t lba,CRecord_file* pRec);		//����һ��¼������
	int DelRecordTask(CSCSI_Dev* pDev,uint64_t lba,CRecord_file* pRec);		//ɾ��¼������

	inline void SetPlayNum(CSCSI_Dev* pDev,int count)
	{
		for(int i = 0; i<(int)m_vec_task.size() ;i++)
		{
			if(m_vec_task[i]->dev == pDev)
			{	
				m_vec_task[i]->replay_num += count;	//count ����Ϊ����
				if(m_vec_task[i]->replay_num < 0)
				{
					m_vec_task[i]->replay_num = 0;
				}
				return;	
			}
		}
	}
};

#endif //_RECORD_TASK_H
