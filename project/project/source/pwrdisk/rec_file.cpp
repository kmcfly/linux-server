/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : 陈涛
** Date         : 2012-9-1
** Name         : RecFileType.h
** Version      : 1.0
** Description  :
   该类的功能，主要是实现类似文件操作APIfopen,fwrite,fread,fclose,fseek 的接口。
** Modify Record:

***********************************************************************/
#include "PUB_common.h"
#include "rec_file.h"
#include "fsinfo.h"

#include "rec_task.h"
CRecord_task g_rec_task;	//全局录像任务管理对象

#define RET_DEAL(ret)												\
	if((ret == SENSE_MEDIUM_ERR) || (ret == SENSE_HARDWARE_ERR))	\
{																\
	if(ret == SENSE_MEDIUM_ERR)									\
	ret = ERR_BAD_SETOR;									\
		else														\
		ret = ERR_HDD_HARDWARE;									\
}else															\
{																\
	ret = ERR_WRITE_FAIL;										\
}

uint32_t CRecord_file::m_FIFO_SIZE = SIZE_512KB;
uint32_t CRecord_file::m_IP_FIFO_SIZE = SIZE_1MB;			//IPC通道，主码流的缓冲大小
uint32_t CRecord_file::m_IP_MIN_FIFO_SIZE = SIZE_256KB;		//IPC通道，第三码流的缓冲大小

CRecord_file::CRecord_file()
{
	m_scsi_dev = NULL;				//文件所属的硬盘
	m_data_size = 128 << 20;		//默认文件总大小	128MB
	m_lba_start = 0;				//文件起始LBA
	m_cur_wt_lba = m_lba_start;		//当前文件正在定入的LBA
#ifdef _WIN32
	m_rw_trans_sec = 128;			//每次传输的大小 Windows上面只能够支持128K
#else
#if defined(__CHIP3535__)
	m_rw_trans_sec = 1024;			//每次传输的大小  Linux 系统支持 512K,256K
#else
	m_rw_trans_sec = 512;;			//每次传输的大小  Linux 系统支持 512K,256K
#endif
#endif
	m_bstop = false;
	m_status = 0;
	m_plock = NULL;					//默认初始化空
	m_access_mod = -1;

	m_align_buf = new unsigned char[SIZE_HDD_BLOCK1];			//默认给足 512 大小，
	memset(m_align_buf,0,SIZE_HDD_BLOCK1);

	m_cache_buf = new unsigned char[2*SIZE_HDD_BLOCK1];			//小批量缓冲数据
	memset(m_cache_buf, 0, 2*SIZE_HDD_BLOCK1);
	

	m_phd_info = NULL;
	m_pfifo_buf = NULL;
	m_channel = 31;
	m_fifo_leaf = 0;

}

CRecord_file::~CRecord_file()
{
	if(m_align_buf)
	{
		delete [] m_align_buf;
	}
	if(m_phd_info)
	{	//注意清除头信息缓冲的顺序
		delete [] m_phd_info->buf;
		delete m_phd_info;
	}
	if(m_pfifo_buf)
	{
		m_pfifo_buf->close();
		delete m_pfifo_buf;
	}

	if (m_cache_buf)
	{
		delete [] m_cache_buf;
		m_cache_buf = NULL;
	}
}

int CRecord_file::fopen(const char * dev_hdd,const char * file_name,int access_mod, FILE_BUF_TYPE fileBufType/*=FILE_BUF_TYPE_LOCAL*/)
{
	uint64_t file_lba;
	CFSInfoTable * pfsInfo = CFSInfoTable::getSingleton();
    if(dev_hdd == NULL)
    {
        TRACE_ERR("Device name is NULL.\n");
        return ERR_NO_DEVICE;
    }
	//TRACE("CH %d  Open dev %s file name %s. \n",m_channel,dev_hdd,file_name);
	m_scsi_dev = pfsInfo->SearchScsiDev(dev_hdd);
    if(m_scsi_dev == NULL)
    {
		TRACE_ERR("No device.\n");
        return ERR_NO_DEVICE;
    }

	file_lba = pfsInfo->SearchFileOnDev(dev_hdd,file_name,&m_data_size);

	//获取硬盘扇区大小，并根据扇区大小配置相关对齐缓冲
	m_sec_size = m_scsi_dev->get_sec_size();
	if(m_sec_size >= SIZE_4KB)
	{	//支持更大扇区块的硬盘
		TRACE("A large block size \n");
		delete [] m_align_buf;
		m_align_buf =  new unsigned char[m_sec_size];
		memset(m_align_buf,0,m_sec_size);

		delete[] m_cache_buf;
		m_cache_buf = new unsigned char[m_sec_size*2];	//小批量缓冲数据	
		memset(m_cache_buf,0,2*m_sec_size);
	}

#ifdef _WIN32
	//在linux上面不会出现关闭文件句柄的情况
	m_scsi_dev->Open();
#endif

	if(file_lba <= 0)
	{	//保护MBR，可能被意外修改
		TRACE("Can not read write MBR.\n");
		return ERR_LBA_ACCES_MBR;
	}

	if(file_lba + (m_data_size/m_sec_size) > m_scsi_dev->get_capacity())
	{	//错误，录像文件已经 超出磁盘的总大小
		TRACE("More than the total size of the disk.file lba:%lld  file size sec:%d disk lba:%lld\n",
			file_lba,(m_data_size/m_sec_size),m_scsi_dev->get_capacity());
		return ERR_LBA_OVERFLOW;
	}

	//查找录像文件的LBA
	m_lba_start  = file_lba;								//文件起始LBA
	m_cur_rd_lba = m_cur_wt_lba = m_lba_start;				//当前文件正在定入的LBA
	m_cur_offset = 0;										//当前读写指针
	m_plock = m_scsi_dev->get_lock();
	m_bstop = false;
	m_status = FILE_NORMAL;
	m_bsuspense = false;
	m_access_mod = access_mod;
	m_cache_pos = 0;
	
	if(access_mod == SCSI_WRONLY)
	{	//专门录像才会开启FIFO
		if(m_pfifo_buf == NULL)
		{
			m_pfifo_buf = new CFIFO_buf();
			TRACE_DBG("LBA %lld FIFO buf open OK, FIFO size %d \n",m_lba_start,CRecord_file::m_FIFO_SIZE);
		}
		
		if (FILE_BUF_TYPE_LOCAL == fileBufType)
		{
			m_pfifo_buf->open(CRecord_file::m_FIFO_SIZE);	//开辟好FIFO缓冲,后面两参数为高低警戒线，
		}
		else if (FILE_BUF_TYPE_IP == fileBufType)
		{
			m_pfifo_buf->open(CRecord_file::m_IP_FIFO_SIZE);	//开辟好FIFO缓冲,后面两参数为高低警戒线，
		}
		else if (FILE_BUF_TYPE_MIN == fileBufType)
		{
			m_pfifo_buf->open(CRecord_file::m_IP_MIN_FIFO_SIZE);	//开辟好FIFO缓冲,后面两参数为高低警戒线，
		}
		else
		{
			m_pfifo_buf->open(CRecord_file::m_FIFO_SIZE);	//开辟好FIFO缓冲,后面两参数为高低警戒线，
		}
	}
	else
	{	//对回放的通道进行计划
		g_rec_task.SetPlayNum(m_scsi_dev,1);
	}
	TIME_CNT_CURRENT(m_lastWtTime);// 记录打开 文件的时间
	return 0;
}


#ifdef _WIN32
int CRecord_file::fopen_scsidev(const CSCSI_Dev* pdev,uint64_t file_lba,int32_t file_size,int access_mod)
{
	if(pdev == NULL)
		return ERR_NO_DEVICE;

	m_scsi_dev = (CSCSI_Dev*)pdev;	//绑定设备
    m_sec_size = m_scsi_dev->get_sec_size();

#ifdef _WIN32
	//在linux上面不会出现关闭文件句柄的情况
	m_scsi_dev->Open();
#endif

	memset(m_align_buf,0,512);
	if(file_lba == 0)
	{
        TRACE("Can not read write MBR.\n");
		return ERR_LBA_ACCES_MBR;
	}

	if(file_lba + (file_size/m_sec_size) > m_scsi_dev->get_capacity())
	{
		TRACE("More than the total size of the disk.file lba:%lld  file size sec:%d disk lba:%lld\n",
                       file_lba,(file_size/m_sec_size),m_scsi_dev->get_capacity());
		return ERR_LBA_OVERFLOW;
	}

	//查找录像文件的LBA
	m_lba_start  = file_lba;								//文件起始LBA
	m_cur_rd_lba = m_cur_wt_lba = m_lba_start;				//当前文件正在定入的LBA
	m_cur_offset = 0;										//当前读写指针
	m_plock = m_scsi_dev->get_lock();
	m_bstop = false;
	m_status = FILE_NORMAL;
	m_bsuspense = false;
	m_access_mod = access_mod;

	if(access_mod == SCSI_WRONLY)
	{	//专门录像才会开启FIFO
		if(m_pfifo_buf == NULL)
		{
			m_pfifo_buf = new CFIFO_buf();
		}
		m_pfifo_buf->open(CRecord_file::m_FIFO_SIZE);	//开辟好FIFO缓冲,后面两参数为高低警戒线，
	}
	else
	{
		g_rec_task.SetPlayNum(m_scsi_dev,1);
	}
	TIME_CNT_CURRENT(m_lastWtTime);// 记录打开 文件的时间
	return 0;
}
#endif

int CRecord_file::close()
{
	//TRACE("CH %d Start close record file.\n",m_channel);

	//先将文件头及I帧索引信息写入到文件头
	int wait_count = 500;	// 超时5秒失败（500 * 10 ms）
	int leaf_size = 0;
	if(m_access_mod == SCSI_WRONLY)
	{
		//Flush_HeadInfo(true);
		m_bstop =true; //设置停止录像 ，等待FIFO中数据写完成后退出
		while( m_status != FILE_WRITE_FAIL)
		{	/*
			等待读写IO完成
			*/
			wait_count--;
			if(wait_count <= 0)
			{
				TRACE_ERR("Time out for close rec file. CH %d m_status %d \n ",m_channel, m_status);
				break;
			}
			if(m_status == FILE_DEVICE_LOST)
			{
				TRACE_ERR("No device %s  LBA %lld \n",m_scsi_dev->get_device_name(),m_lba_start);
				break;
			}

			if((leaf_size = m_pfifo_buf->GetDataSize()) == 0 && (m_phd_info->changed_part == 0))
			{
				//TRACE("CH %d LBA %lld dev %s End all data write.\n", m_channel,m_lba_start,m_scsi_dev->get_device_name());
				break;
			}
			DVRSleep(10);
			m_bstop = true;
		}
	}
	else
	{
		g_rec_task.SetPlayNum(m_scsi_dev,-1);
	}
	m_data_size = 0;
	m_status = 0;
	m_access_mod = -1;

	g_rec_task.DelRecordTask(m_scsi_dev,m_lba_start,this);
	//TRACE("CH %d End close record file.wait_count %d \n",m_channel,wait_count);
    return 0;
}

//data_size： 限制为小于录像的m_data_size，
int CRecord_file::Read(unsigned char * data_buf, uint32_t data_size)
{
	uint32_t read_size;
	uint32_t trans_sec;
	uint32_t data_in = 0;

	unsigned char * pbuf = data_buf;
	uint32_t remain_size = 0;
	uint32_t align_size = 0;

	int ret = 0;
	int retry_count = 0;
	uint32_t last_notidy_io = 0;

	if(m_scsi_dev == NULL)
	{
		TRACE("Read data-- NO DEV.\n");
		return ERR_NO_DEVICE;
	}

	if(data_buf == NULL)
	{
		TRACE("Read data-- Buf Empty.\n");
		return ERR_DATA_BUF_EMPYT;
	}

	if(data_size > m_data_size)
	{
		TRACE("Data size more then file size.\n");
		return ERR_DATA_OVER_FILE;
	}

	if(m_access_mod == SCSI_WRONLY)
	{
		TRACE("The file is WRONLY.\n");
		return ERR_ACCESS_MOD;
	}

	m_status = FILE_READING;

	if(m_cur_offset + data_size > m_data_size)
	{
		TRACE("Data size is over the file size.\n");
		return ERR_DATA_OVER_FILE;
	}

	trans_sec = m_rw_trans_sec;
	read_size = m_rw_trans_sec * m_sec_size;
	m_cur_rd_lba = m_cur_offset/m_sec_size + m_lba_start;
	remain_size = m_cur_offset&(m_sec_size-1);

	if(data_size + remain_size < m_sec_size)
		align_size = data_size;
	else
		align_size = m_sec_size - remain_size;

	//锁住磁盘设备
	LOCK_DEV(m_plock);

	if(remain_size) //移动数据指针后，出现未对齐的情况
	{
		ret = m_scsi_dev->Read_try(m_cur_rd_lba,1,m_align_buf,m_sec_size);
		if(ret)
		{
			if(ret == SENSE_MEDIUM_ERR || ret == SENSE_HARDWARE_ERR)
			{
				TRACE("Read error lba: %lld transfer: 1.\n",m_cur_rd_lba);
				if(ret == SENSE_MEDIUM_ERR)
					ret =  ERR_BAD_SETOR;
				else
					ret =  ERR_HDD_HARDWARE;
				goto ERR_OUT;
			}
			else
			{
				ret = ERR_READ_FAIL;
				TRACE("Read error lba: %llu transfer: 1.\n",m_cur_rd_lba);
				goto ERR_OUT;
			}
		}

		memcpy(pbuf,m_align_buf + remain_size,align_size);

		if(data_size + remain_size >= m_sec_size)
			m_cur_rd_lba++;	//将当前读写LBA加1
		data_size -= align_size;
		pbuf += align_size;
		data_in += align_size;
	}

	while(data_size)
	{
		if(data_size < read_size)
		{
			if(data_size < m_sec_size)
			{
				last_notidy_io = data_size;
				break;
			}
			if(last_notidy_io)
			{
				break;
			}
			last_notidy_io = data_size%m_sec_size;
			read_size = data_size - last_notidy_io;
			trans_sec = data_size/m_sec_size;
		}
RETRY:
		ret = m_scsi_dev->Read(m_cur_rd_lba,trans_sec,pbuf,read_size);

		if(ret)
		{
			if(ret == SENSE_MEDIUM_ERR || ret == SENSE_HARDWARE_ERR)
			{
				TRACE("Read error lba: %llu transfer: %d.\n",m_cur_rd_lba,trans_sec);
				if(ret == SENSE_MEDIUM_ERR)
					ret = ERR_BAD_SETOR;
				else
					ret = ERR_HDD_HARDWARE;
				goto ERR_OUT;
			}
			else
			{
				TRACE("Read error lba: %llu transfer: %d.\n",m_cur_rd_lba,trans_sec);
				if(retry_count > 3)
				{
					m_cur_offset += data_in;
					ret = ERR_READ_FAIL;
					goto ERR_OUT;
				}
				retry_count++;
				goto RETRY;
			}
		}
		data_size -= read_size;
		pbuf += read_size;
		m_cur_rd_lba += trans_sec;
		data_in += read_size;
	}

	m_cur_offset += data_in;
	if(last_notidy_io)	//如果存在非对齐的情况，需要另外做处理
	{
		ret = m_scsi_dev->Read_try(m_cur_rd_lba,1,m_align_buf,m_sec_size);
		if(ret)
		{
			if(ret == SENSE_MEDIUM_ERR || ret == SENSE_HARDWARE_ERR)
			{
				TRACE_ERR("Read error lba: %llu transfer: %d.\n",m_cur_rd_lba,1);
				if(ret == SENSE_MEDIUM_ERR)
					ret = ERR_BAD_SETOR;
				else
					ret = ERR_HDD_HARDWARE;
			}
			else
			{
				ret = ERR_READ_FAIL;
				TRACE_ERR("Read error lba: %llu transfer: %d.\n",m_cur_rd_lba,1);
				goto ERR_OUT;
			}
		}
		memcpy(pbuf,m_align_buf,last_notidy_io);
		data_in += last_notidy_io;
		m_cur_offset += last_notidy_io;
	}
	//互斥解锁
	UNLOCK_DEV(m_plock);

	m_status = FILE_NORMAL;
	if(m_access_mod == SCSI_RDWR)
		m_cur_wt_lba = m_cur_rd_lba;

	return data_in;

ERR_OUT:
	//互斥解锁
	UNLOCK_DEV(m_plock);
	m_status = ERR_READ_FAIL;
	return ret;
}

//data_size： 限制为小于录像的m_data_size，
int CRecord_file::Write(unsigned char * data_buf,uint32_t data_size)
{
	uint32_t write_size;
	uint32_t trans_sec;
	uint32_t data_out = 0;

	unsigned char * pbuf = data_buf;
	uint32_t last_notidy_io = 0;

	int ret = 0;
	int retry_count = 0;

	uint32_t remain_size = 0;
	uint32_t align_size = 0;

	if(m_scsi_dev == NULL)
		return ERR_NO_DEVICE;

	if(data_size > m_data_size)
	{
		TRACE_ERR("Data size is too big.\n");
		return ERR_DATA_OVER_FILE;
	}
	if(data_buf == NULL)
		return ERR_DATA_BUF_EMPYT;

	if(m_access_mod == SCSI_RDONLY)
	{
		TRACE_ERR("The file is WRONLY.\n");
		return ERR_ACCESS_MOD;
	}

	if(m_status == ERR_WRITE_FAIL)	//使用FIFO
	{
		TRACE_ERR("The record is already write fail.cur offset %d. data_size %d \n",m_cur_offset,data_size);
		return ERR_WRITE_FAIL;
	}
	m_status = FILE_WRITTING;
	/*
	  将不对齐的数据，进行提前处理
	*/
	if(data_size + m_cur_offset > m_data_size )
	{
		TRACE_ERR("The file is over write data_size %d offset %d. m_data_size \n",data_size,m_cur_offset,m_data_size);
		return ERR_DATA_OVER_FILE;
	}

	trans_sec = m_rw_trans_sec;
	write_size = m_rw_trans_sec * m_sec_size;
	m_cur_wt_lba = m_cur_offset/m_sec_size + m_lba_start;
	remain_size = m_cur_offset&(m_sec_size-1);
	if(data_size + remain_size < m_sec_size)
		align_size = data_size;
	else
		align_size = m_sec_size - remain_size;

	//LOCK_DEV(m_plock);

	if(remain_size) //移动数据指针后，出现未对齐的情况
	{
		ret = m_scsi_dev->Read_try(m_cur_wt_lba,1,m_align_buf,m_sec_size);
		if(ret)
		{
			if(ret == SENSE_MEDIUM_ERR || ret == SENSE_HARDWARE_ERR)
			{
				TRACE_ERR("Read error lba: %llu transfer: %d.\n",m_cur_wt_lba,1);
				if(ret == SENSE_MEDIUM_ERR)
					ret = ERR_BAD_SETOR;
				else
					ret = ERR_HDD_HARDWARE;
			}
			else
			{	sperr("Read",ret);
			}

			goto ERR_OUT;
		}
		if(remain_size + align_size > m_sec_size)
		{
			printf("remain size %d align size %d .\n",remain_size,align_size );
		}
		memcpy(m_align_buf + remain_size,pbuf,align_size);
		ret = m_scsi_dev->Write_try(m_cur_wt_lba,1,m_align_buf,m_sec_size);
		if(ret)
		{
			if(ret == SENSE_MEDIUM_ERR || ret == SENSE_HARDWARE_ERR)
			{
				TRACE_ERR("Read error lba: %llu transfer: %d.\n",m_cur_wt_lba,1);
				if(ret == SENSE_MEDIUM_ERR)
					ret = ERR_BAD_SETOR;
				else
					ret =  ERR_HDD_HARDWARE;
			}
			else
			{
				sperr("Write",ret);
			}
			goto ERR_OUT;
		}
		if(data_size + remain_size >= m_sec_size)
			m_cur_wt_lba++;	//将当前读写LBA加1
		data_size -= align_size;
		pbuf += align_size;
		data_out += align_size;
	}

	while(data_size)
	{
		if(data_size < write_size)
		{
			if(data_size < m_sec_size)
			{
				last_notidy_io = data_size;
				break;
			}
			if(last_notidy_io)
			{
				break;
			}
			last_notidy_io = data_size%m_sec_size;
			write_size = data_size - last_notidy_io;
			trans_sec = data_size/m_sec_size;
		}

RETRY:
		ret = m_scsi_dev->Write(m_cur_wt_lba,trans_sec,pbuf,write_size);
		if(ret)
		{
			if(ret == SENSE_MEDIUM_ERR || ret == SENSE_HARDWARE_ERR)
			{
				TRACE_ERR("Write error lba: %llu transfer: %d.\n",m_cur_wt_lba,trans_sec);
				if(ret == SENSE_MEDIUM_ERR)
					ret = ERR_BAD_SETOR;
				else
					ret = ERR_HDD_HARDWARE;
				goto ERR_OUT;
			}
			sperr("write",m_scsi_dev->get_last_err());
			if(retry_count > 3)
			{
				m_cur_offset += data_out;
				goto ERR_OUT;
			}
			retry_count++;
			goto RETRY;
		}

		data_size -= write_size;
		pbuf += write_size;
		m_cur_wt_lba += trans_sec;
		data_out += write_size;
	}

	m_cur_offset += data_out;
	if(last_notidy_io)	//如果存在非对齐的情况，需要另外做处理
	{
		ret = m_scsi_dev->Read_try(m_cur_wt_lba,1,m_align_buf,m_sec_size);
		if(ret)
		{
			if(ret == SENSE_MEDIUM_ERR || ret == SENSE_HARDWARE_ERR)
			{
				TRACE_ERR("Read error lba: %llu transfer: %d.\n",m_cur_wt_lba,1);
				if(ret == SENSE_MEDIUM_ERR)
					ret = ERR_BAD_SETOR;
				else
					ret = ERR_HDD_HARDWARE;
			}
			else
			{	sperr("Read",m_scsi_dev->get_last_err());
			}
			goto ERR_OUT;
		}
		memcpy(m_align_buf,pbuf,last_notidy_io);
		ret = m_scsi_dev->Write_try(m_cur_wt_lba,1,m_align_buf,m_sec_size);
		if(ret)
		{
			if(ret == SENSE_MEDIUM_ERR || ret == SENSE_HARDWARE_ERR)
			{
				TRACE_ERR("Read error lba: %llu transfer: %d.\n",m_cur_wt_lba,1);
				if(ret == SENSE_MEDIUM_ERR)
					ret = ERR_BAD_SETOR;
				else
					ret = ERR_HDD_HARDWARE;
			}
			else
			{
				sperr("Write",m_scsi_dev->get_last_err());
			}
			goto ERR_OUT;
		}
		data_out += last_notidy_io;
		m_cur_offset += last_notidy_io;
	}
	//互斥解锁

	//UNLOCK_DEV(m_plock);	//互斥解锁
	if(m_access_mod == SCSI_RDWR)
		m_cur_rd_lba = m_cur_wt_lba;
	m_status = FILE_NORMAL;
	TIME_CNT_CURRENT(m_lastWtTime);
	return data_out;

ERR_OUT:
	//互斥解锁
	//UNLOCK_DEV(m_plock);	//互斥解锁

	m_status = ERR_WRITE_FAIL;
	return ret;
}

int CRecord_file::Seek(int32_t offset,int whence)
{
	switch(whence)
	{
	case SCSI_SEEK_END:
		m_cur_offset = m_data_size;
		break;

	case SCSI_SEEK_CUR:
		if(offset < 0)
		{
			if((uint32_t)abs(offset) > m_cur_offset )
				goto ERR_OUT;
			m_cur_offset = m_cur_offset - abs(offset);
		}
		else
		{
			if(m_cur_offset + offset > m_data_size)
				goto ERR_OUT;
			m_cur_offset = m_cur_offset + offset;
		}
		break;

	case SCSI_SEEK_SET:

		if(offset < 0)
			goto ERR_OUT;
		else if((uint32_t)offset > m_data_size )
			goto ERR_OUT;
		else
		{
			m_cur_offset =(uint32_t)offset;
		}
		break;

	default:
		TRACE_ERR("Error param.\n");
		goto ERR_OUT;
		break;
	}

	if(m_access_mod == SCSI_RDONLY)
	{
		m_cur_rd_lba = m_cur_offset/m_sec_size + m_lba_start;
	}
	else if(m_access_mod == SCSI_WRONLY)
	{
		m_cur_wt_lba = m_cur_offset/m_sec_size + m_lba_start;
	}
	else //同时读写
	{
		m_cur_rd_lba = m_cur_wt_lba = m_cur_offset/m_sec_size + m_lba_start;
	}

	return 0;
ERR_OUT:
	return ERR_SEEK_FAIL;
}


	/*
		Seek(SET) + Read 合并函数,
	*/
int CRecord_file::Read_pos(uint32_t offset,unsigned char * data_buf, uint32_t data_size)
{
	uint32_t read_size;
	uint32_t trans_sec;
	uint32_t data_in = 0;
	uint64_t cur_rw_lba;

	unsigned char * pbuf = data_buf;
	uint32_t remain_size = 0;
	uint32_t align_size = 0;

	int ret = 0;
	int retry_count = 0;
	uint32_t last_notidy_io = 0;

	if(m_scsi_dev == NULL)
	{
		TRACE_ERR("Read data-- NO DEV.\n");
		return ERR_NO_DEVICE;
	}

	if(data_buf == NULL)
	{
		TRACE_ERR("Read data-- Buf Empty.\n");
		return ERR_DATA_BUF_EMPYT;
	}

	if(data_size > m_data_size)
	{
		TRACE_ERR("Data size more then file size.\n");
		return ERR_DATA_OVER_FILE;
	}

	if(m_access_mod == SCSI_WRONLY)
	{
		TRACE_ERR("The file is WRONLY.\n");
		return ERR_ACCESS_MOD;
	}

	m_status = FILE_READING;

	if (offset >= m_data_size)
	{
		TRACE_ERR("Data size %d add offset=%d is over the file size %d.\n", data_size, offset, m_data_size);
		return ERR_DATA_OVER_FILE;
	}

	if(offset + data_size > m_data_size)
	{
		TRACE_ERR("Data size %d add offset=%d is over the file size %d.\n", data_size, offset, m_data_size);

		//data_size = (offset + data_size) - m_data_size;
		data_size = m_data_size - offset;
	}

	if(offset + data_size > m_data_size)
	{
		TRACE_ERR("Data size %d add offset=%d is over the file size %d.\n", data_size, offset, m_data_size);
		return ERR_DATA_OVER_FILE;
	}

	trans_sec = m_rw_trans_sec;
	read_size = m_rw_trans_sec * m_sec_size;
	cur_rw_lba = offset/m_sec_size + m_lba_start;
	remain_size = offset&(m_sec_size-1);

	if(data_size + remain_size < m_sec_size)
		align_size = data_size;
	else
		align_size = m_sec_size - remain_size;

	//锁住磁盘设备
	LOCK_DEV(m_plock);

	if(remain_size) //移动数据指针后，出现未对齐的情况
	{
		ret = m_scsi_dev->Read_try(cur_rw_lba,1,m_align_buf,m_sec_size);
		if(ret)
		{
			if(ret == SENSE_MEDIUM_ERR || ret == SENSE_HARDWARE_ERR)
			{
				TRACE_ERR("Read error lba: %lld transfer: 1.\n",m_cur_rd_lba);
				if(ret == SENSE_MEDIUM_ERR)
					ret =  ERR_BAD_SETOR;
				else
					ret =  ERR_HDD_HARDWARE;
				goto ERR_OUT;
			}
			else
			{
				TRACE_ERR("Read error lba: %llu transfer: 1.\n",m_cur_rd_lba);
				goto ERR_OUT;
			}
		}

		memcpy(pbuf,m_align_buf + remain_size,align_size);

		if(data_size + remain_size >= m_sec_size)
			cur_rw_lba++;	//将当前读写LBA加1
		data_size -= align_size;
		pbuf += align_size;
		data_in += align_size;
	}

	while(data_size)
	{
		if(data_size < read_size)
		{
			if(data_size < m_sec_size)
			{
				last_notidy_io = data_size;
				break;
			}
			if(last_notidy_io)
			{
				break;
			}
			last_notidy_io = data_size%m_sec_size;
			read_size = data_size - last_notidy_io;
			trans_sec = data_size/m_sec_size;
		}
RETRY:
		ret = m_scsi_dev->Read(cur_rw_lba,trans_sec,pbuf,read_size);
		if(ret)
		{
			if(ret == SENSE_MEDIUM_ERR || ret == SENSE_HARDWARE_ERR)
			{
				TRACE_ERR("Read error lba: %llu transfer: %d.\n", m_cur_rd_lba,trans_sec);
				if(ret == SENSE_MEDIUM_ERR)
					ret = ERR_BAD_SETOR;
				else
					ret = ERR_HDD_HARDWARE;
				goto ERR_OUT;
			}
			else
			{
				TRACE_ERR("Read error lba: %llu transfer: %d.\n", m_cur_rd_lba,trans_sec);
				if(retry_count > 3)
				{
					offset += data_in;
					goto ERR_OUT;
				}
				retry_count++;
				goto RETRY;
			}
		}
		data_size -= read_size;
		pbuf += read_size;
		cur_rw_lba += trans_sec;
		data_in += read_size;
	}

	if(last_notidy_io)	//如果存在非对齐的情况，需要另外做处理
	{
		ret = m_scsi_dev->Read_try(cur_rw_lba,1,m_align_buf,m_sec_size);
		if(ret)
		{
			if(ret == SENSE_MEDIUM_ERR || ret == SENSE_HARDWARE_ERR)
			{
				TRACE_ERR("Read error lba: %llu transfer: %d.\n",m_cur_rd_lba,1);
				if(ret == SENSE_MEDIUM_ERR)
					ret = ERR_BAD_SETOR;
				else
					ret = ERR_HDD_HARDWARE;

				goto ERR_OUT;
			}
			else
			{
				TRACE_ERR("Read error lba: %llu transfer: %d.\n",m_cur_rd_lba,1);
				goto ERR_OUT;
			}
		}
		memcpy(pbuf,m_align_buf,last_notidy_io);
		data_in += last_notidy_io;
	}
	//互斥解锁
	UNLOCK_DEV(m_plock);

	m_status = FILE_NORMAL;

	return data_in;

ERR_OUT:
	//互斥解锁
	UNLOCK_DEV(m_plock);
	m_status = ERR_READ_FAIL;
	return ret;
}
/*
	Seek(SET) + Write 合并函数，针对写入的非对齐情况，会进行 读取--修改--再写入操作 单个扇区的数据

*/
int CRecord_file::Write_pos(uint32_t offset,unsigned char * data_buf, uint32_t data_size)
{
	uint32_t write_size;
	uint32_t trans_sec;
	uint32_t data_out = 0;

	unsigned char * pbuf = data_buf;
	uint32_t last_notidy_io = 0;
	uint64_t cur_wt_lba = -1;
	int ret = 0;
	int retry_count = 0;

	uint32_t remain_size = 0;
	uint32_t align_size = 0;

	if(m_scsi_dev == NULL)
		return ERR_NO_DEVICE;

	if(data_size > m_data_size)
		return ERR_DATA_OVER_FILE;

	if(data_buf == NULL)
		return ERR_DATA_BUF_EMPYT;

	if(m_access_mod == SCSI_RDONLY)
	{
		TRACE_ERR("The file is WRONLY.\n");
		return ERR_ACCESS_MOD;
	}

	m_status = FILE_WRITTING;
	/*
	  将不对齐的数据，进行提前处理
	*/
	if(data_size + offset > m_data_size )
	{
		TRACE_ERR("Data_size is ERR_DATA_OVER_FILE.\n");
		return ERR_DATA_OVER_FILE;
	}

	trans_sec = m_rw_trans_sec;
	write_size = m_rw_trans_sec * m_sec_size;
	cur_wt_lba = offset/m_sec_size + m_lba_start;
	remain_size = offset&(m_sec_size-1);
	if(data_size + remain_size < m_sec_size)
		align_size = data_size;
	else
		align_size = m_sec_size - remain_size;

	LOCK_DEV(m_plock);

	if(remain_size) //移动数据指针后，出现未对齐的情况
	{
		ret = m_scsi_dev->Read_try(cur_wt_lba,1,m_align_buf,m_sec_size);
		if(ret)
		{
			if(ret == SENSE_MEDIUM_ERR || ret == SENSE_HARDWARE_ERR)
			{
				TRACE_ERR("Read error lba: %llu transfer: %d.\n",cur_wt_lba,1);
				if(ret == SENSE_MEDIUM_ERR)
					ret =  ERR_BAD_SETOR;
				else
					ret = ERR_HDD_HARDWARE;
			}
			else
			{	sperr("Read",ret);
				ret = ERR_WRITE_FAIL;
			}
			goto ERR_OUT;
		}
		if(remain_size + align_size > m_sec_size)
		{
			printf("remain size %d align size %d .\n",remain_size,align_size );
		}
		memcpy(m_align_buf + remain_size,pbuf,align_size);

		ret = m_scsi_dev->Write_try(cur_wt_lba,1,m_align_buf,m_sec_size);
		if(ret)
		{
			if(ret == SENSE_MEDIUM_ERR || ret == SENSE_HARDWARE_ERR)
			{
				TRACE_ERR("Read error lba: %llu transfer: %d.\n",cur_wt_lba,1);
				if(ret == SENSE_MEDIUM_ERR)
					ret =  ERR_BAD_SETOR;
				else
					ret = ERR_HDD_HARDWARE;
			}
			else
			{
				sperr("Write",ret);
				ret = ERR_WRITE_FAIL;
			}
			goto ERR_OUT;
		}
		if(data_size + remain_size >= m_sec_size)
			cur_wt_lba++;	//将当前读写LBA加1
		data_size -= align_size;
		pbuf += align_size;
		data_out += align_size;
	}

	while(data_size)
	{
		if(data_size < write_size)
		{
			if(data_size < m_sec_size)
			{
				last_notidy_io = data_size;
				break;
			}
			if(last_notidy_io)
			{
				break;
			}
			last_notidy_io = data_size%m_sec_size;
			write_size = data_size - last_notidy_io;
			trans_sec = data_size/m_sec_size;
		}

RETRY:
		ret = m_scsi_dev->Write(cur_wt_lba,trans_sec,pbuf,write_size);
		if(ret)
		{
			if(ret == SENSE_MEDIUM_ERR || ret == SENSE_HARDWARE_ERR)
			{
				TRACE_ERR("Read error lba: %llu transfer: %d.\n",cur_wt_lba,1);
				if(ret == SENSE_MEDIUM_ERR)
					ret = ERR_BAD_SETOR;
				else
					ret = ERR_HDD_HARDWARE;
				goto ERR_OUT;
			}

			sperr("write",ret);
			if(retry_count > 3)
			{
				ret = ERR_READ_FAIL;
				goto ERR_OUT;
			}
			retry_count++;
			goto RETRY;
		}
		data_size -= write_size;
		pbuf += write_size;
		cur_wt_lba += trans_sec;
		data_out += write_size;
	}

	if(last_notidy_io)	//如果存在非对齐的情况，需要另外做处理
	{
		ret = m_scsi_dev->Read_try(cur_wt_lba,1,m_align_buf,m_sec_size);
		if(ret)
		{
			if(ret == SENSE_MEDIUM_ERR || ret == SENSE_HARDWARE_ERR)
			{
				TRACE_ERR("Read error lba: %llu transfer: %d.\n",cur_wt_lba,1);
				if(ret == SENSE_MEDIUM_ERR)
					ret =  ERR_BAD_SETOR;
				else
					ret = ERR_HDD_HARDWARE;
			}
			else
			{	sperr("Read",ret);
				ret = ERR_WRITE_FAIL;
			}
			goto ERR_OUT;
		}

		memcpy(m_align_buf,pbuf,last_notidy_io);

		ret = m_scsi_dev->Write_try(cur_wt_lba,1,m_align_buf,m_sec_size);
		if(ret)
		{
			if(ret == SENSE_MEDIUM_ERR || ret == SENSE_HARDWARE_ERR)
			{
				TRACE_ERR("Read error lba: %llu transfer: %d.\n",cur_wt_lba,1);
				if(ret == SENSE_MEDIUM_ERR)
					ret = ERR_BAD_SETOR;
				else
					ret = ERR_HDD_HARDWARE;
			}
			else
			{
				sperr("Write",ret);
				ret = ERR_WRITE_FAIL;
			}
			goto ERR_OUT;
		}
		data_out += last_notidy_io;
	}
	//互斥解锁
	UNLOCK_DEV(m_plock);

	if(m_access_mod == SCSI_RDWR)
		m_cur_rd_lba = cur_wt_lba;
	m_status = FILE_NORMAL;
	return data_out;

ERR_OUT:
	//互斥解锁
	UNLOCK_DEV(m_plock);

	m_status = ERR_WRITE_FAIL;
	return ERR_WRITE_FAIL;
}

//FIFO 缓存功能接口
//
/*
	异步IO操作，会使用到FIFO操作，将数据送入到FIFO当中，写入线程会不断去执行写入数据
	*/
int CRecord_file::Write_FIFO( unsigned char * data_buf, uint32_t data_size)
{
	int ret;
	int wait_count = 300;			//根据底层编解码设计 4秒一个I帧，如果超过这个时间应该丢掉该帧数据
	uint32_t free_space = 0;
	// static int check_cnt = 0;
	//假如已经FIFO中出现写失败的情况
	if(m_status == FILE_WRITE_FAIL|| m_status == FILE_DEVICE_LOST)
	{	//应对文件中途写入失败，再次进行FIFO缓冲时直接返回失败
		TRACE_ERR("Write FIFO fail\n");
		return -1;
	}

	for(int idx = 0 ; idx < 3; idx++)
	{
		ret = m_pfifo_buf->write(data_buf,data_size);
		if(ret == data_size)
		{
			break;
		}
		if(ret == FIFO_ERR_NOTENOUGH )
		{
			while(wait_count)	//底层编解码芯片最多可以缓存8秒钟数据，所可以这里设计最多等5秒钟
			{
				if(wait_count == 0)
				{	//超时之后跳出循环，丢弃该当前数据帧
					TRACE_ERR("CH %%d Wait time too long.Free space %u data size %u\n",m_channel,free_space,data_size);
					return FIFO_ERR_TIMEOUT;
				}

				//假如已经FIFO中出现写失败的情况
				if(m_status == FILE_WRITE_FAIL|| m_status == FILE_DEVICE_LOST)
				{	//应对文件中途写入失败，再次进行FIFO缓冲时直接返回失败

					TRACE_ERR("Write FIFO fail ,m_status %d error %d %s .\n",m_status,m_scsi_dev->get_last_err(),strerror(m_scsi_dev->get_last_err()));
					return -1;
				}

				DVRSleep(10);

				free_space = m_pfifo_buf->GetFreeSize();
				if(free_space >= data_size)
				{
					//TRACE_ERR("CH %02d FIFO_ERR_NOTENOUGH free_space %d data_size %d wait_count %d \n",m_channel,free_space,data_size,wait_count);
					break;
				}

				wait_count--;
			}
		}
	}

	return data_size;
}


int CRecord_file::Flush_FIFOData(int time_out, int cache_water)
{
	int ret = 0;
	int write_size;
	unsigned char* pbuf = NULL;
	unsigned char* pTailBuf = NULL;
	uint32_t leaf_size = 0;
	int data_size = 0;
	unsigned char endFrameBuf[1024]= {0};		//结束帧

	if(!(m_bstop||m_bsuspense || IsTimeOut(time_out)|| m_pfifo_buf->GetCurWarter() >= cache_water))
	{
		return 0;
	}

	m_bsuspense = false;
	
	write_size = data_size = m_pfifo_buf->GetDataSize();
// 	if((0 == m_phd_info->changed_part) && (0 == data_size))
// 	{
// 		return 0;
// 	}

	LOCK_DEV(m_plock);
	if(0 != m_phd_info->changed_part)
	{
		ret = Flush_HeadInfo(m_bstop);
		if(ret)
		{
			TRACE_ERR("Updata key frame idx OK. ret %d Rec-CH %lu\n", ret,get_channel());
			goto ERR_REC;
		}
	}

	if(0 != data_size)
	{
		pbuf = m_pfifo_buf->GetBufOffset(data_size,&leaf_size,&pTailBuf);
		if(NULL != pTailBuf)
		{
			ret = WriteCachePos(m_cur_offset,pTailBuf,leaf_size);
			if(ret != leaf_size)
			{
				TRACE("2 Write data fail.ret %d  leaf_size %d \n",ret, leaf_size);
				goto ERR_REC;
			}

			if(leaf_size > data_size)
			{
				TRACE_ERR("Error leaf size.\n");
			}
			write_size = data_size - leaf_size;
		}

		if(write_size == 0)
		{
			TRACE(" 1 Write data fail.write_size %03d ret %d \n",write_size,ret);
		}
		if(NULL != pbuf &&  0 != write_size)
		{
			//ret = Write(pbuf,write_size);
			ret = WriteCachePos(m_cur_offset,pbuf,write_size);
			if(ret != write_size)
			{
				TRACE(" 1 Write data fail.write_size %03d ret %d \n",write_size,ret);
				goto ERR_REC;
			}
		}
		m_pfifo_buf->ConsumeBuf(data_size);

	}

	UNLOCK_DEV(m_plock);
	return 0;

ERR_REC:
	UNLOCK_DEV(m_plock);
	return ret;
}

int CRecord_file::SetHeadData(HEAD_CHANGE_PART change_part, uint32_t offset,unsigned char * data_buf,uint32_t data_size)
{
	if(NULL == m_phd_info)
	{
		assert(false);
		printf("%s:%s:%d, m_phd_info=NULL\n", __FUNCTION__, __FILE__, __LINE__);
		return ERR_DATA_BUF_EMPYT;
	}
	if(offset + data_size > m_phd_info->buf_size )
	{
		return ERR_DATA_OVER_FILE;
	}
	//将头信息缓冲起来
	memcpy(m_phd_info->buf + offset,data_buf,data_size);
	m_phd_info->changed_part |= change_part;	//标记好已经改变的部分
	return data_size;
}

//关键索引信息缓冲
int CRecord_file::Set_KeyFrameIdx(unsigned char* idx_buf,uint32_t idx_size,uint32_t framePos)
{
	if(m_phd_info)
	{
		unsigned char * pbuf = m_phd_info->buf + m_phd_info->keyFrame_cnt* 20 +m_phd_info->kFrame_offset;
		memcpy(pbuf,idx_buf,idx_size);
		m_phd_info->keyFrame_cnt++;
		m_phd_info->data_offset += idx_size;
		
		//如果上次的关键帧索引信息没有被写入就不更新位置信息，否则在帧率低时会出现帧索引写不到文件的情况，这会导致
		if (!(m_phd_info->changed_part & CHANGED_INDEX))
		{
			m_phd_info->changed_part |= CHANGED_INDEX;
			m_phd_info->last_frameoOffset = framePos;
		}

		return 0;
	}
	else
	{
		assert(false);
		printf("%s:%s:%d, m_phd_info=NULL\n", __FUNCTION__, __FILE__, __LINE__);
	}

	return -1;
}

//刷新文件头信息，I帧索引，及视频格式，音频格式信息
//
//	将缓冲好的关键帧索引刷新到磁盘上
//	保证好扇区首尾对齐，最大程度的减少不对齐所导致的性能降低

int CRecord_file::Flush_HeadInfo(bool bEndRecord)
{
	int ret = 0;

	int wt_size;	//写入大小
	uint32_t start_addr,end_addr;
	unsigned char * pbuf;
	uint64_t write_lba;

	//当出现文件头信息改变时，即时刷新头信息进去
	if(m_phd_info == NULL)
	{
		return 0;
	}
	//LOCK_DEV(m_plock);

	if( m_phd_info->changed_part & CHANGED_HEAD ||
		bEndRecord)
	{
		ret = m_scsi_dev->Write_try(m_lba_start,1,m_phd_info->buf,m_sec_size);	//整个扇区写入，前面已经清理了整个扇区的信息
		if(ret)
		{
			if(ret == SENSE_MEDIUM_ERR || ret == SENSE_HARDWARE_ERR)
			{
				//出现物理坏道
				TRACE_ERR("Write error lba: %llu transfer: %d.\n",m_cur_wt_lba,1);
				if(ret == SENSE_MEDIUM_ERR)
					ret = ERR_BAD_SETOR;
				else
					ret = ERR_HDD_HARDWARE;
			}
			else
			{
				sperr("Write",ret);
				ret = ERR_WRITE_FAIL;
			}
		}
		m_phd_info->changed_part &= ~ CHANGED_HEAD;
		//TRACE_DBG("Flush the head data.\n");
	}
	//只要存在视频格式和音频格式信息改变时，即时刷新
	if( m_phd_info->changed_part & CHANGED_VIDEO_FMT ||
		m_phd_info->changed_part & CHANGED_AUDIO_FMT)
	{
		write_lba = m_lba_start + BLK_COUNT(m_phd_info->buf_size - m_sec_size,m_sec_size);
		pbuf = m_phd_info->buf + m_phd_info->buf_size - m_sec_size;
		//文件未端写入
		ret = m_scsi_dev->Write_try(write_lba,1,pbuf,m_sec_size);	//整个扇区写入，前面已经清理了整个扇区的信息
		if(ret)
		{
			if(ret == SENSE_MEDIUM_ERR || ret == SENSE_HARDWARE_ERR)
			{
				//出现物理坏道
				if(ret == SENSE_MEDIUM_ERR)
					ret = ERR_BAD_SETOR;
				else
					ret = ERR_HDD_HARDWARE;
			}
			else
			{
				sperr("Write",ret);
				ret = ERR_WRITE_FAIL;
			}
		}
		//TRACE_DBG("Flush the %s format data.\n",m_phd_info->changed_part & CHANGED_VIDEO_FMT? "video":"audio");
		m_phd_info->changed_part &= ~CHANGED_VIDEO_FMT;// 清除掉
		m_phd_info->changed_part &= ~CHANGED_AUDIO_FMT;//
	}

	//为了减少磁盘寻道，可以考虑将索引写写入的频率降低
	if( (m_phd_info->changed_part & CHANGED_INDEX)&&
	    ((m_cur_offset >= m_phd_info->last_frameoOffset)|| bEndRecord)
	  )
	{   //考虑到关键帧每隔4秒种会产生一次，所以必要及时刷新到磁盘
		//计算出相关的地址
		start_addr = SIZE_ROUND_ALIGN(m_phd_info->write_offset,m_sec_size);
		//end_addr = SIZE_ROUND_UP(m_phd_info->data_offset,m_sec_size);
		uint32_t cur_data_offset = m_phd_info->data_offset;
		end_addr = SIZE_ROUND_UP(cur_data_offset,m_sec_size);
		wt_size = end_addr-start_addr;
		write_lba = m_lba_start + BLK_COUNT(start_addr,m_sec_size);
		pbuf = m_phd_info->buf + start_addr;

		ret = m_scsi_dev->Write(write_lba,pbuf,wt_size);
		if(ret)
		{
			if(ret == SENSE_MEDIUM_ERR || ret == SENSE_HARDWARE_ERR)
			{	//出现物理坏道
				TRACE("Write error lba: %llu transfer: %d.\n",m_cur_wt_lba,(wt_size/m_sec_size));
				if(ret == SENSE_MEDIUM_ERR)
					ret = ERR_BAD_SETOR;
				else
					ret = ERR_HDD_HARDWARE;
			}
			else
			{
				sperr("Write",ret);
				ret = ERR_WRITE_FAIL;
			}
		}
		//结束文件写入
#if 0
		if(m_phd_info->keyFrame_cnt % 2 )
			TRACE_DBG("Flush the Idx data.	write_offset %d data_offset %d keyFrame_cnt %d",
							m_phd_info->write_offset,m_phd_info->data_offset,m_phd_info->keyFrame_cnt);
#endif
		//m_phd_info->write_offset = m_phd_info->data_offset;
		m_phd_info->write_offset = cur_data_offset;
		m_phd_info->changed_part &= ~CHANGED_INDEX ; //清除索引改变标志位

	}

//	UNLOCK_DEV(m_plock);

	return ret;
}


void CRecord_file::Start_FlushData()
{
	g_rec_task.AddRecordTask(m_scsi_dev,m_lba_start,this);
}
int CRecord_file::InitHead(unsigned char * head_buf,uint32_t head_size,uint32_t idx_size,uint32_t clear_start, uint32_t clear_end)
{
	int ret;
	int write_size;

	if(m_phd_info == NULL)
	{
		m_phd_info = new RecInfo;
		memset(m_phd_info,0,sizeof(RecInfo));
		m_phd_info->buf_size = SIZE_ROUND_UP(clear_end,m_sec_size);
		m_phd_info->buf = new unsigned char[m_phd_info->buf_size];	//这里申请的缓冲包含头信息，索引以及视频帧格式，音频帧格式
		memset(m_phd_info->buf,0,m_phd_info->buf_size);
	}
	else
	{
		m_phd_info->changed_part = 0;
		memset(m_phd_info->buf,0,m_phd_info->buf_size);
	}

	m_phd_info->keyFrame_cnt = 0;
	m_phd_info->changed_part |= 0x1;
	m_phd_info->data_offset = m_phd_info->write_offset = m_phd_info->kFrame_offset = clear_start;

	//这部分数据是需要直接 写入到文件当中的。
	memcpy(m_phd_info->buf, head_buf,head_size);
	write_size = m_phd_info->buf_size;	 //对齐到扇区的整数倍,未满磁盘扇区数，会补齐
  //  TRACE("++Start init Head buf size %d \n",m_phd_info->buf_size);
	LOCK_DEV(m_plock);
	//从录像文件头开始一次写完
	ret = m_scsi_dev->Write(m_lba_start,m_phd_info->buf,write_size);
	UNLOCK_DEV(m_plock);
	if(ret)
	{
		if(ret == SENSE_MEDIUM_ERR || ret == SENSE_HARDWARE_ERR)
		{
			//出现物理坏道
			TRACE_ERR("Write error lba: %llu transfer: %d.\n",m_lba_start,(write_size/m_sec_size));
			if(ret == SENSE_MEDIUM_ERR)
				ret = ERR_BAD_SETOR;
			else
				ret = ERR_HDD_HARDWARE;
		}
		else
		{	sperr("Read",ret);
			ret = ERR_READ_FAIL;
		}
	}
	//TRACE("--InitHead OK. m_lba_start %lld write_size %d ,\n",m_lba_start,write_size);
	//写入成功
	return ret;
}

bool CRecord_file::IsTimeOut(int timeOutMS)
{
	pub_time_t cntTime;
	int timeDiff = 0;	//时间差

	TIME_CNT_CURRENT(cntTime);
	timeDiff = TIME_DIFF_MS(m_lastWtTime,cntTime);

	if(timeDiff >= timeOutMS || timeDiff < 0)
	{
		//TRACE("CH %d timeount. timeDiff %d data size: %d cur-offset %d \n",m_channel,timeDiff, m_pfifo_buf->GetDataSize(),m_cur_offset);
		return true;
	}
	else
	{
		return false;
	}
}

int CRecord_file::WriteCachePos(uint32_t offset, unsigned char * pBuf,uint32_t dataSize)
{
	int ret = 0;
	int retSize = dataSize;
	uint64_t curLBA =  offset/m_sec_size + m_lba_start;


	if( NULL == m_scsi_dev )
	{
		return ERR_NO_DEVICE;
	}

	if(NULL == pBuf)
	{
		return ERR_DATA_BUF_EMPYT;
	}

	if(SCSI_RDONLY == m_access_mod)
	{
		TRACE_ERR("The file is WRONLY.\n");
		return ERR_ACCESS_MOD;
	}

	m_status = FILE_WRITTING;

	if((dataSize + m_cur_offset)> m_data_size )
	{
		TRACE_ERR("Data_size is ERR_DATA_OVER_FILE.\n");
		return ERR_DATA_OVER_FILE;
	}

	//三个小流程的数据
	uint32_t lenHead = 0;
	uint32_t lenBody = 0;
	uint32_t lenTail = 0;
	uint32_t remainSize = offset&(m_sec_size-1);

	if(0 != remainSize)
	{
		if(dataSize <= (m_sec_size - remainSize))
		{
			lenHead = dataSize;
		}
		else
		{
			lenHead = m_sec_size - remainSize;
		}
	}

	unsigned char * pDestBuf = pBuf;
	if(0 != lenHead)
	{
		if((m_cur_offset == offset) && (0 != m_cache_pos))
		{
			//缓冲处理
			assert(m_cache_pos == remainSize);
			memcpy(m_cache_buf + m_cache_pos, pBuf, lenHead);

			pDestBuf = m_cache_buf;
			m_cache_pos = remainSize + lenHead;
		}
		else
		{
			assert(false);
			TRACE_ERR("m_cur_offset %d != Offset %d m_cache_pos %d \n",m_cur_offset,offset,m_cache_pos);
			m_cache_pos = 0;
			ret = m_scsi_dev->Read_try(curLBA,1,m_cache_buf,m_sec_size);
			if(ret)
			{
				m_status = FILE_WRITE_FAIL;
				return ret;
			}

			memcpy(m_cache_buf + remainSize, pBuf, lenHead);

			pDestBuf = m_cache_buf;

			m_cache_pos = remainSize + lenHead;
		}

		if((dataSize + remainSize) <= m_sec_size)
		{	//注意执行到这里的时候，数据会全部写入完成，之后会退出函数
			//小于一个扇区的情况下，多写入一个扇区，保证尾帧全部是零
			if(m_cache_pos < m_sec_size)
			{
				memset(m_cache_buf + m_cache_pos,0, m_sec_size - m_cache_pos);
			}
			else
			{
				m_cache_pos = 0;
			}

			int transferCnt = 2;

			if((m_data_size - (retSize + offset)) >= m_sec_size)
			{
				transferCnt = 2;
			}
			else
			{
				transferCnt = 1;
			}

			ret = m_scsi_dev->Write_try(curLBA,transferCnt,pDestBuf,m_sec_size * transferCnt);
			if(ret)
			{
				m_status = FILE_WRITE_FAIL;
				return ret;
			}
			//这里已经提前结束写入数据
			TRACE("CH %ld little data size %d cache pos %d write done.\n",m_channel,m_cache_pos,retSize);
			m_cur_offset = offset + retSize;
			m_status = FILE_NORMAL;
			return retSize;
		}
		else
		{
			ret = m_scsi_dev->Write_try(curLBA,1,pDestBuf,m_sec_size);
			if(ret)
			{
				m_status = FILE_WRITE_FAIL;
				return ret;
			}

			curLBA++;
			dataSize -= lenHead;
			m_cache_pos = 0;
		}
	}

	assert(0 == m_cache_pos);

	pDestBuf = pBuf + lenHead;
	lenTail = dataSize & (m_sec_size-1);
	lenBody = dataSize - lenTail;

	if(0 != lenBody)
	{
#if 0
		ret = WriteAlign(curLBA,pDestBuf,lenBody);
		if(ret)
		{
			UNLOCK_DEV(m_plock);
			m_status = FILE_WRITE_FAIL;
			return ret;
		}
#endif		
		uint32_t ioSecs = m_rw_trans_sec;
		uint32_t ioSize = m_rw_trans_sec * m_sec_size;
		uint32_t leafSize = 0;

		uint32_t transCnt = lenBody / ioSize;
		leafSize = lenBody - (transCnt * ioSize);

		for (int i = 0 ; i < transCnt;i++)
		{
			ret = m_scsi_dev->Write_try(curLBA,ioSecs,pDestBuf,ioSize);
			if(ret)
			{
				m_status = FILE_WRITE_FAIL;
				return ret;
			}
			curLBA += ioSecs;
			pDestBuf += ioSize;
		}

		if(leafSize >= m_sec_size)
		{
			ioSecs = leafSize/m_sec_size;
			ioSize = ioSecs * m_sec_size;

			ret = m_scsi_dev->Write_try(curLBA,ioSecs,pDestBuf,ioSize);
			if(ret)
			{
				RET_DEAL(ret);
				m_status = FILE_WRITE_FAIL;
				return ret;
			}
			curLBA += ioSecs;	//更新LBA
		}

		pDestBuf =pBuf + lenHead + lenBody;
	}

	//如果这里已经把数据全部写完成
	if(0 == lenTail)
	{	//这里就是需要刷一个帧数据进去
		if((m_data_size - (retSize + offset)) >= m_sec_size)
		{
			ret = m_scsi_dev->Write_try(curLBA,1,m_cache_buf + m_sec_size, m_sec_size);
			if(ret)
			{
				m_status = FILE_WRITE_FAIL;
				return ret;
			}
		}
	}
	else //lenTail != 0  未尾还有数据
	{	//不对齐的情况下写入两个扇区
		memcpy(m_cache_buf,pDestBuf,lenTail);
		m_cache_pos = lenTail;
		memset(m_cache_buf + m_cache_pos,0,m_sec_size - m_cache_pos);

		int transferCnt = 2;

		if((m_data_size - (retSize + offset)) >= m_sec_size)
		{
			transferCnt = 2;
		}
		else
		{
			transferCnt = 1;
		}
		ret = m_scsi_dev->Write_try(curLBA,transferCnt,m_cache_buf,m_sec_size * transferCnt);
		if(ret)
		{
			m_status = FILE_WRITE_FAIL;
			return ret;
		}
	}
	//成功结束写入
	m_cur_offset = offset + retSize;
	m_status = FILE_NORMAL;

	TIME_CNT_CURRENT(m_lastWtTime);		// 记录文件的时间　　这一行加了没有

	return retSize;
}

