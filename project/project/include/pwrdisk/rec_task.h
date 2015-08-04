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
	CSCSI_Dev * dev;		//对应的磁盘设备
	PUB_thread_t thd_id;	//线程ID
	bool      thd_start;	//线程开启
	bool wait;
	int replay_num;			//回放的文件个数
	uint64_t	full_lba;	//当前缓冲已经满了的LBA
	MAP_RECORD_FILE mp_rec;;//录像文件表
	CPUB_Lock	devRecLock;
}dev_rec_s,*pdev_rec_s;

typedef std::vector<dev_rec_s*>  VEC_DEV_REC; //设备列表

class CRecord_task
{

protected:

	VEC_DEV_REC m_vec_task;		//

public:
	int AddRecordTask(CSCSI_Dev* pDev,uint64_t lba,CRecord_file* pRec);		//增加一个录像任务
	int DelRecordTask(CSCSI_Dev* pDev,uint64_t lba,CRecord_file* pRec);		//删除录像任务

	inline void SetPlayNum(CSCSI_Dev* pDev,int count)
	{
		for(int i = 0; i<(int)m_vec_task.size() ;i++)
		{
			if(m_vec_task[i]->dev == pDev)
			{	
				m_vec_task[i]->replay_num += count;	//count 可以为负数
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
