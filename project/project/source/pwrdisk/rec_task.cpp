/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : ����
** Date         : 2012-9-1
** Name         : rec_task.cpp
** Version      : 1.0
** Description  :
   ����Ĺ��ܣ���Ҫ��ʵ��¼������̡߳�
** Modify Record:

***********************************************************************/
#include "PUB_common.h"
#include "rec_task.h"
//¼�������

/*
	����һ���߳�ȥ¼�����һ��Ӳ��
	ʹ����MAP���������Ա�֤¼���ļ��ڴ�������λ���Ǵ�С���� LBA ������Ч�������Ѱ����ʱ��
	д�����ݹؼ�˼�룬�����ٴο����ڴ棬����CPUռ���ʡ���FIFO���ڴ��ַ����
	��������
	1. �ж�¼���Ƿ�����������������Ҫ����ǰ¼���ļ���FIFO���嵱�е�����ȫ��ˢ�µ����̡�
	2. ˢ��I֡������Ϣ
	3. д��¼������

*/
//#define TIME_LOOK

static int DevRecThd(void * arg)//���һ��Ӳ�̵�д���߳�
{
	dev_rec_s* pRecTask = (dev_rec_s*)arg;
	CPUB_Lock * pDevRecLock = &pRecTask->devRecLock;
	CRecord_file * pRec = NULL;
	MAP_RECORD_FILE* p_mp_rec;
	MAP_RECORD_FILE::iterator it_rec,it_rec_bak;
	MAP_RECORD_FILE::reverse_iterator  it_rrec;

	uint32_t cache_water = 90;
	uint32_t max_water = 85;					//Ĭ�����ˮλ��ֻҪ�����ˮλ��һ����Ҫй��


	int    cache_time = 5000;					//FIFO���屣�����ʱ�� ��λms

	unsigned char* pbuf = NULL;
	unsigned char* pTailBuf = NULL;
	CFIFO_buf* pFifo = NULL;
	int count = 0;
	int ret = 0;

#ifdef TIME_LOOK
	pub_time_t tm_start,tm_stop;
#endif
	
	pub_time_t tm_last_flush,tm_current;
	int32_t wait_count = 20;					//Ĭ��10��
	int sleep_all_ch = 4;

	p_mp_rec = &pRecTask->mp_rec;

	//TRACE_DBG("DevRec thread start. sleep time rec %d  ,\n",sleep_all_ch);

	//¼��ѭ���߳�
	/*
		�ؼ�˼��
		1.�����ļ���LBA��С����˳��ȥд�룬�����Ͽ������̶ȵļ��ٴ��̴�Χ��ת��������ʱ
		2.¼������д���������������������д�룬������Ϊ�������������� ��ȡ--�޸�--д��
		3.�߳�������һЩ��ʱ���ֳ�һЩʱ��Ƭ�������ϲ��߳����������ݵ�FIFO���壬��Ϊ���̵�д�����ݵ��ٶ�Զ����16·¼��ͨ������������������
		4.��ʱˢ��I֡����
		5.����д������ͨ����Ӧ�ü�ʱɾ����ǰͨ��¼��
		6.ֻҪ�ϲ�ֹͣ¼�񣬾�Ӧ������������ˢ�µ���������
		7.ʱ�䳬��5000ms�ͱ���д������
	*/
	TIME_CNT_CURRENT(tm_last_flush);//��¼�ʼִ�е�ʱ��
	while(true)
	{
		//Ŀǰ���16·дӲ��
		pDevRecLock->Lock();
		
		if((int)p_mp_rec->size() == 0)
		{
			//��û��¼���ļ���ʱ��Ҳ���˳��̣߳������ظ������̡߳�
			pDevRecLock->UnLock();

			DVRSleep(500); //

			pRecTask->thd_start = false;
			continue;
		}
		pRecTask->thd_start = true;
#ifdef TIME_LOOK
		TIME_CNT_CURRENT(tm_start);
#endif
		for(it_rec = p_mp_rec->begin(); it_rec !=  p_mp_rec->end(); )
		{
			pRec = it_rec->second;				//��ȡ¼���ļ���
			ret = pRec->Flush_FIFOData(cache_time,cache_water);
			if(ret != 0)
			{
				//����ֵ����
				if(errno == 2 || errno == 19)
				{
					//�豸��ʧ
					pRec->set_resualt(FILE_DEVICE_LOST);
				}
				else
				{
					pRec->set_resualt(FILE_WRITE_FAIL);	//�ļ�д��ʧ��
				}

				it_rec_bak = it_rec;
				it_rec++;
				p_mp_rec->erase(it_rec_bak);
				continue;
			}

			if(pRec->is_closed() || (it_rec->first != pRec->get_lba_start()))
			{
				it_rec_bak = it_rec;
				it_rec++;
				p_mp_rec->erase(it_rec_bak);
				continue;
			}

			//�ҳ����������Ѿ������ģ�����������д��
			if(pRecTask->full_lba)
			{
				wait_count = 2;
				pDevRecLock->UnLock();
				goto REVERSE_FLUSH;
			}

			it_rec++;
		}
		pDevRecLock->UnLock();

#ifdef TIME_LOOK
		TIME_CNT_CURRENT(tm_stop);
		if((++count % 60 == 0) ||(ret = TIME_DIFF_MS(tm_start,tm_stop))>= 600 )
		{
			TRACE("%s, Rec-loop %ld ms  Map size %d thdID %d  CH ",pRecTask->dev->get_device_name(),ret,p_mp_rec->size(),pRecTask->thd_id);
			for (it_rec = p_mp_rec->begin();it_rec != p_mp_rec->end();it_rec++)
			{
				TRACE("%d ", it_rec->second->get_channel());
			}
			TRACE("Cache_water %d wait %d \n",cache_water,pRecTask->wait);
		}
#endif

		while(wait_count--)
		{
			SLEEP(sleep_all_ch);//

			//���������һ·ͨ���ģ�fifo��ˮλ����60ʱ��ǰ������ʱѭ��,����й����������Ƶ��ʧ
			bool waterFull = false;

			pDevRecLock->Lock();
			for(it_rec = p_mp_rec->begin(); it_rec !=  p_mp_rec->end(); it_rec++)
			{
				if (it_rec->second->is_stop_rec() || it_rec->second->FlushDataOK(max_water))
				{
					waterFull = true;
				//	TRACE("CH %d 1 Water is too high %d  \n",it_rec->second->get_channel(),it_rec->second->GetFifoBuf()->GetCurWarter());
					break;
				}
			}
			pDevRecLock->UnLock();

			if (waterFull)
			{
				break;
			}
		}

		wait_count = 10;

		//����й��ˮλ������40%�������ֶ�·�ط�ʱ��·60,��·70,�������Լ���ѭ����д�������ͬʱҲ���ǿ��Լ���Ѱ����ʱ��
		//�����������ܻ�һ���׶ˣ����طŵ�·��Խ�࣬�����������Խ�󣬴��������ᵼ�µĶ�ʧ��������Ҳ��Խ��ʵʱ�洢��
REVERSE_FLUSH:
		//�������
		pDevRecLock->Lock();
		for(it_rrec = p_mp_rec->rbegin(); it_rrec !=  p_mp_rec->rend(); it_rrec++)
		{
			pRec = it_rrec->second;				//��ȡ¼���ļ���

			if(pRec->is_closed() || (it_rrec->first != pRec->get_lba_start()))
			{
                continue;
			}

			ret = pRec->Flush_FIFOData(cache_time,cache_water);
			if(ret != 0)
			{
				///����ֵ����
				if(errno == 2 || errno == 19)
				{
					//�豸��ʧ
					pRec->set_resualt(FILE_DEVICE_LOST);
				}
				else
				{	//�ļ�д��ʧ��
					pRec->set_resualt(FILE_WRITE_FAIL);
				}
			}
		}
		pDevRecLock->UnLock();

		while(wait_count--)
		{
			SLEEP(sleep_all_ch);//

			//���������һ·ͨ���ģ�fifo��ˮλ����cache_waterʱ��ǰ������ʱѭ��,����й����������Ƶ֡�ڵײ����ۻ�̫��
			bool bWaterFull = false;
			pDevRecLock->Lock();
			for(it_rec = p_mp_rec->begin(); it_rec !=  p_mp_rec->end(); it_rec++)
			{
				if (it_rec->second->is_stop_rec() || it_rec->second->FlushDataOK(max_water))
				{
					bWaterFull = true;
					//TRACE("CH %d 2 Water is too high %d  \n",it_rec->second->get_channel(),it_rec->second->GetFifoBuf()->GetCurWarter());
					break;
				}
			}
			pDevRecLock->UnLock();

			if (bWaterFull)
			{
				break;
			}
		}
		wait_count = 10;

		if((pRecTask->replay_num == 0) || (pRecTask->replay_num  < 0) )
		{
			cache_water = 50;
			max_water = 80;

			if(pRecTask->replay_num  < 0)
				pRecTask->replay_num  = 0;
		}
		else
		{
			if(pRecTask->replay_num == 1)
			{
				cache_water = 60;
				max_water = 85;
			}
			else if(pRecTask->replay_num >=1 && pRecTask->replay_num  < 4)
			{
				cache_water = 65;
				max_water = 87;
			}
			else if (pRecTask->replay_num >= 4)
			{
				cache_water = 50;
				max_water = 80;
			}
		}

		//ÿ��30Sǿ��ͬ��Ӳ�̵Ļ���
		TIME_CNT_CURRENT(tm_current);
		if((ret = TIME_DIFF_MS(tm_last_flush,tm_current)) >= 30000 || (ret < 0) )
		{
			//TRACE_DBG("Flush data to HDD.Diff time %d \n",ret);
			pRecTask->dev->FlushCache();
			TIME_CNT_CURRENT(tm_last_flush);
		}

	}

	
	pRecTask->thd_id = (PUB_thread_t)0;
	TRACE_DBG("DevRec thread END.\n");

	return 0;
}


int CRecord_task::AddRecordTask(CSCSI_Dev* pDev,uint64_t lba,CRecord_file* pRec)		//����һ��¼������
{
	//����ǲ����µĴ����豸
	int num;
	dev_rec_s* dev_rec_mp = NULL;
	dev_rec_s* pdev_rec_mp = NULL;
	MAP_RECORD_FILE::iterator it_rec;
	VEC_DEV_REC::iterator it_dev;
	num = (int)m_vec_task.size();

	for(it_dev = m_vec_task.begin() ;it_dev != m_vec_task.end();it_dev++)
	{
		if(strstr(pDev->get_device_name(),((dev_rec_s*)*it_dev)->dev->get_device_name()))
		{
			pdev_rec_mp =(dev_rec_s*) (*it_dev);
			goto ADD_RECORD_FILE ;
		}
	}
	dev_rec_mp = new dev_rec_s();
	dev_rec_mp->dev = pDev;
	dev_rec_mp->thd_start =false;
	dev_rec_mp->mp_rec[lba] = pRec;
	dev_rec_mp->full_lba = 0;
	m_vec_task.push_back(dev_rec_mp);
	//δ�ҵ�������£���Ҫ�����µ��߳�ȥд��¼��
	TRACE_ERR("Create thread for record.\n");
	pdev_rec_mp	= dev_rec_mp;

	if(pDev->test_unit_ready())
	{
		pDev->Open();
		pDev->ReadCapacity(NULL);
	}

	pdev_rec_mp->thd_id = PUB_CreateThread((PUB_start_routine)DevRecThd, (void *)pdev_rec_mp, NULL);
	pdev_rec_mp->wait = true;
	pdev_rec_mp->replay_num = 0;
	return 1;

ADD_RECORD_FILE:
	//������¼����뵽¼���б���
	pdev_rec_mp->devRecLock.Lock();
	pdev_rec_mp->thd_start  = true;
	pdev_rec_mp->mp_rec[lba] = pRec;
	pdev_rec_mp->devRecLock.UnLock();
	
	if((PUB_thread_t)0 == pdev_rec_mp->thd_id)
	{	//�����̳߳����������������Ҫ��������ˢ���̡߳�
		TRACE_ERR("Re-cread thhread to flush fifo data. dev %s \n",dev_rec_mp->dev->get_device_name());
		pdev_rec_mp->thd_id = PUB_CreateThread((PUB_start_routine)DevRecThd, (void *)pdev_rec_mp, NULL);
		pdev_rec_mp->wait = true;
		pdev_rec_mp->replay_num = 0;
	}
#if 0
	TRACE("Add record.Record file LBA %lld, Record num %d  .pRec %d \n",pRec->get_lba_start(),pdev_rec_mp->mp_rec.size(),pRec);
	if((int)pdev_rec_mp->mp_rec.size() && pRec->get_channel() == 15 )
	{
		for(it_rec = pdev_rec_mp->mp_rec.begin() ;it_rec !=  pdev_rec_mp->mp_rec.end(); it_rec++)
		{
			TRACE("CH %02d LBA %ld \n",it_rec->second->get_channel(),it_rec->second->get_lba_start());
		}
	}
#endif

	return 0;
}

int CRecord_task::DelRecordTask(CSCSI_Dev* pDev,uint64_t lba,CRecord_file* pRec)		//ɾ��¼������
{
	VEC_DEV_REC::iterator it_devTask;
	MAP_RECORD_FILE::iterator it_rec;

	for(it_devTask = m_vec_task.begin(); it_devTask != m_vec_task.end();it_devTask++)
	{
		if(((*it_devTask))->dev == pDev)
		{
			(*it_devTask)->devRecLock.Lock();

			MAP_RECORD_FILE* pMP_rec = &((*it_devTask))->mp_rec;
			for (it_rec = pMP_rec->begin();it_rec != pMP_rec->end(); it_rec++)
			{
				if((lba == it_rec->first) && (it_rec->second == pRec))
				{
					TRACE_DBG("Found CH %d the rec-file.LBA %lld cur-offset %u \n",pRec->get_channel(),pRec->get_lba_start(),pRec->get_cur_offset());
					pMP_rec->erase(it_rec);
					break;
				}
			}

			(*it_devTask)->devRecLock.UnLock();
			return 0;
		}
	}

	return -1;
}
