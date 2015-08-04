/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : 陈涛
** Date         : 2012-9-1
** Name         : rec_task.cpp
** Version      : 1.0
** Description  :
   该类的功能，主要是实现录像管理线程。
** Modify Record:

***********************************************************************/
#include "PUB_common.h"
#include "rec_task.h"
//录像管理类

/*
	开启一个线程去录像针对一个硬盘
	使用了MAP容器，可以保证录像文件在磁盘上面位置是从小到大 LBA 排序，有效解决磁盘寻道的时候
	写入数据关键思想，避免再次拷贝内存，减少CPU占用率。将FIFO的内存地址给出
	大致流程
	1. 判断录像是否结束，如果结束，需要将当前录像文件的FIFO缓冲当中的数据全部刷新到磁盘。
	2. 刷新I帧索引信息
	3. 写入录像数据

*/
//#define TIME_LOOK

static int DevRecThd(void * arg)//针对一个硬盘的写入线程
{
	dev_rec_s* pRecTask = (dev_rec_s*)arg;
	CPUB_Lock * pDevRecLock = &pRecTask->devRecLock;
	CRecord_file * pRec = NULL;
	MAP_RECORD_FILE* p_mp_rec;
	MAP_RECORD_FILE::iterator it_rec,it_rec_bak;
	MAP_RECORD_FILE::reverse_iterator  it_rrec;

	uint32_t cache_water = 90;
	uint32_t max_water = 85;					//默认最高水位，只要到这个水位，一定需要泄流


	int    cache_time = 5000;					//FIFO缓冲保留的最长时间 单位ms

	unsigned char* pbuf = NULL;
	unsigned char* pTailBuf = NULL;
	CFIFO_buf* pFifo = NULL;
	int count = 0;
	int ret = 0;

#ifdef TIME_LOOK
	pub_time_t tm_start,tm_stop;
#endif
	
	pub_time_t tm_last_flush,tm_current;
	int32_t wait_count = 20;					//默认10次
	int sleep_all_ch = 4;

	p_mp_rec = &pRecTask->mp_rec;

	//TRACE_DBG("DevRec thread start. sleep time rec %d  ,\n",sleep_all_ch);

	//录像循环线程
	/*
		关键思想
		1.磁盘文件按LBA从小到大顺序，去写入，理论上可以最大程度的减少磁盘大范围跳转所带来延时
		2.录像数据写入均按磁盘扇区的整数倍写入，减少因为不对齐所带来的 读取--修改--写入
		3.线程最后进行一些延时，分出一些时间片来给其上层线程来拷贝数据到FIFO缓冲，因为磁盘的写入数据的速度远大于16路录像通道所产生的数据量。
		4.定时刷新I帧索引
		5.对于写入出错的通道，应该及时删除当前通道录像。
		6.只要上层停止录像，就应该立即将数据刷新到磁盘上面
		7.时间超过5000ms就必须写入数据
	*/
	TIME_CNT_CURRENT(tm_last_flush);//记录最开始执行的时间
	while(true)
	{
		//目前最大16路写硬盘
		pDevRecLock->Lock();
		
		if((int)p_mp_rec->size() == 0)
		{
			//当没有录像文件的时候，也不退出线程，避免重复创建线程。
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
			pRec = it_rec->second;				//获取录像文件类
			ret = pRec->Flush_FIFOData(cache_time,cache_water);
			if(ret != 0)
			{
				//错误值处理
				if(errno == 2 || errno == 19)
				{
					//设备丢失
					pRec->set_resualt(FILE_DEVICE_LOST);
				}
				else
				{
					pRec->set_resualt(FILE_WRITE_FAIL);	//文件写入失败
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

			//找出缓冲现在已经的满的，立即将其它写入
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

			//当发现最后一路通道的，fifo的水位高于60时提前跳出延时循环,进行泄流，避免视频丢失
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

		//调整泄流水位，正常40%，当出现多路回放时单路60,四路70,这样可以减少循环的写入次数，同时也就是可以减少寻道的时间
		//这样做法可能会一个弊端，当回放的路数越多，缓冲的数据量越大，此如果掉电会导致的丢失的数据量也就越大。实时存储差
REVERSE_FLUSH:
		//反向遍历
		pDevRecLock->Lock();
		for(it_rrec = p_mp_rec->rbegin(); it_rrec !=  p_mp_rec->rend(); it_rrec++)
		{
			pRec = it_rrec->second;				//获取录像文件类

			if(pRec->is_closed() || (it_rrec->first != pRec->get_lba_start()))
			{
                continue;
			}

			ret = pRec->Flush_FIFOData(cache_time,cache_water);
			if(ret != 0)
			{
				///错误值处理
				if(errno == 2 || errno == 19)
				{
					//设备丢失
					pRec->set_resualt(FILE_DEVICE_LOST);
				}
				else
				{	//文件写入失败
					pRec->set_resualt(FILE_WRITE_FAIL);
				}
			}
		}
		pDevRecLock->UnLock();

		while(wait_count--)
		{
			SLEEP(sleep_all_ch);//

			//当发现最后一路通道的，fifo的水位高于cache_water时提前跳出延时循环,进行泄流，避免视频帧在底层中累积太多
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

		//每隔30S强制同步硬盘的缓存
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


int CRecord_task::AddRecordTask(CSCSI_Dev* pDev,uint64_t lba,CRecord_file* pRec)		//增加一个录像任务
{
	//检查是不是新的磁盘设备
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
	//未找到的情况下，需要创建新的线程去写入录像
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
	//开启新录像加入到录像列表当中
	pdev_rec_mp->devRecLock.Lock();
	pdev_rec_mp->thd_start  = true;
	pdev_rec_mp->mp_rec[lba] = pRec;
	pdev_rec_mp->devRecLock.UnLock();
	
	if((PUB_thread_t)0 == pdev_rec_mp->thd_id)
	{	//假如线程出现退了情况，则需要重新启动刷新线程。
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

int CRecord_task::DelRecordTask(CSCSI_Dev* pDev,uint64_t lba,CRecord_file* pRec)		//删除录像任务
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
