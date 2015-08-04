/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : ����
** Date         : 2012-9-1
** Name         : RecFileType.h
** Version      : 1.0
** Description  :
   ����Ĺ��ܣ���Ҫ��ʵ�������ļ�����APIfopen,fwrite,fread,fclose,fseek �Ľӿڡ�
** Modify Record:

***********************************************************************/
#include "PUB_common.h"
#include "rec_file.h"
#include "fsinfo.h"

#include "rec_task.h"
CRecord_task g_rec_task;	//ȫ��¼������������

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
uint32_t CRecord_file::m_IP_FIFO_SIZE = SIZE_1MB;			//IPCͨ�����������Ļ����С
uint32_t CRecord_file::m_IP_MIN_FIFO_SIZE = SIZE_256KB;		//IPCͨ�������������Ļ����С

CRecord_file::CRecord_file()
{
	m_scsi_dev = NULL;				//�ļ�������Ӳ��
	m_data_size = 128 << 20;		//Ĭ���ļ��ܴ�С	128MB
	m_lba_start = 0;				//�ļ���ʼLBA
	m_cur_wt_lba = m_lba_start;		//��ǰ�ļ����ڶ����LBA
#ifdef _WIN32
	m_rw_trans_sec = 128;			//ÿ�δ���Ĵ�С Windows����ֻ�ܹ�֧��128K
#else
#if defined(__CHIP3535__)
	m_rw_trans_sec = 1024;			//ÿ�δ���Ĵ�С  Linux ϵͳ֧�� 512K,256K
#else
	m_rw_trans_sec = 512;;			//ÿ�δ���Ĵ�С  Linux ϵͳ֧�� 512K,256K
#endif
#endif
	m_bstop = false;
	m_status = 0;
	m_plock = NULL;					//Ĭ�ϳ�ʼ����
	m_access_mod = -1;

	m_align_buf = new unsigned char[SIZE_HDD_BLOCK1];			//Ĭ�ϸ��� 512 ��С��
	memset(m_align_buf,0,SIZE_HDD_BLOCK1);

	m_cache_buf = new unsigned char[2*SIZE_HDD_BLOCK1];			//С������������
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
	{	//ע�����ͷ��Ϣ�����˳��
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

	//��ȡӲ��������С��������������С������ض��뻺��
	m_sec_size = m_scsi_dev->get_sec_size();
	if(m_sec_size >= SIZE_4KB)
	{	//֧�ָ����������Ӳ��
		TRACE("A large block size \n");
		delete [] m_align_buf;
		m_align_buf =  new unsigned char[m_sec_size];
		memset(m_align_buf,0,m_sec_size);

		delete[] m_cache_buf;
		m_cache_buf = new unsigned char[m_sec_size*2];	//С������������	
		memset(m_cache_buf,0,2*m_sec_size);
	}

#ifdef _WIN32
	//��linux���治����ֹر��ļ���������
	m_scsi_dev->Open();
#endif

	if(file_lba <= 0)
	{	//����MBR�����ܱ������޸�
		TRACE("Can not read write MBR.\n");
		return ERR_LBA_ACCES_MBR;
	}

	if(file_lba + (m_data_size/m_sec_size) > m_scsi_dev->get_capacity())
	{	//����¼���ļ��Ѿ� �������̵��ܴ�С
		TRACE("More than the total size of the disk.file lba:%lld  file size sec:%d disk lba:%lld\n",
			file_lba,(m_data_size/m_sec_size),m_scsi_dev->get_capacity());
		return ERR_LBA_OVERFLOW;
	}

	//����¼���ļ���LBA
	m_lba_start  = file_lba;								//�ļ���ʼLBA
	m_cur_rd_lba = m_cur_wt_lba = m_lba_start;				//��ǰ�ļ����ڶ����LBA
	m_cur_offset = 0;										//��ǰ��дָ��
	m_plock = m_scsi_dev->get_lock();
	m_bstop = false;
	m_status = FILE_NORMAL;
	m_bsuspense = false;
	m_access_mod = access_mod;
	m_cache_pos = 0;
	
	if(access_mod == SCSI_WRONLY)
	{	//ר��¼��ŻῪ��FIFO
		if(m_pfifo_buf == NULL)
		{
			m_pfifo_buf = new CFIFO_buf();
			TRACE_DBG("LBA %lld FIFO buf open OK, FIFO size %d \n",m_lba_start,CRecord_file::m_FIFO_SIZE);
		}
		
		if (FILE_BUF_TYPE_LOCAL == fileBufType)
		{
			m_pfifo_buf->open(CRecord_file::m_FIFO_SIZE);	//���ٺ�FIFO����,����������Ϊ�ߵ;����ߣ�
		}
		else if (FILE_BUF_TYPE_IP == fileBufType)
		{
			m_pfifo_buf->open(CRecord_file::m_IP_FIFO_SIZE);	//���ٺ�FIFO����,����������Ϊ�ߵ;����ߣ�
		}
		else if (FILE_BUF_TYPE_MIN == fileBufType)
		{
			m_pfifo_buf->open(CRecord_file::m_IP_MIN_FIFO_SIZE);	//���ٺ�FIFO����,����������Ϊ�ߵ;����ߣ�
		}
		else
		{
			m_pfifo_buf->open(CRecord_file::m_FIFO_SIZE);	//���ٺ�FIFO����,����������Ϊ�ߵ;����ߣ�
		}
	}
	else
	{	//�Իطŵ�ͨ�����мƻ�
		g_rec_task.SetPlayNum(m_scsi_dev,1);
	}
	TIME_CNT_CURRENT(m_lastWtTime);// ��¼�� �ļ���ʱ��
	return 0;
}


#ifdef _WIN32
int CRecord_file::fopen_scsidev(const CSCSI_Dev* pdev,uint64_t file_lba,int32_t file_size,int access_mod)
{
	if(pdev == NULL)
		return ERR_NO_DEVICE;

	m_scsi_dev = (CSCSI_Dev*)pdev;	//���豸
    m_sec_size = m_scsi_dev->get_sec_size();

#ifdef _WIN32
	//��linux���治����ֹر��ļ���������
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

	//����¼���ļ���LBA
	m_lba_start  = file_lba;								//�ļ���ʼLBA
	m_cur_rd_lba = m_cur_wt_lba = m_lba_start;				//��ǰ�ļ����ڶ����LBA
	m_cur_offset = 0;										//��ǰ��дָ��
	m_plock = m_scsi_dev->get_lock();
	m_bstop = false;
	m_status = FILE_NORMAL;
	m_bsuspense = false;
	m_access_mod = access_mod;

	if(access_mod == SCSI_WRONLY)
	{	//ר��¼��ŻῪ��FIFO
		if(m_pfifo_buf == NULL)
		{
			m_pfifo_buf = new CFIFO_buf();
		}
		m_pfifo_buf->open(CRecord_file::m_FIFO_SIZE);	//���ٺ�FIFO����,����������Ϊ�ߵ;����ߣ�
	}
	else
	{
		g_rec_task.SetPlayNum(m_scsi_dev,1);
	}
	TIME_CNT_CURRENT(m_lastWtTime);// ��¼�� �ļ���ʱ��
	return 0;
}
#endif

int CRecord_file::close()
{
	//TRACE("CH %d Start close record file.\n",m_channel);

	//�Ƚ��ļ�ͷ��I֡������Ϣд�뵽�ļ�ͷ
	int wait_count = 500;	// ��ʱ5��ʧ�ܣ�500 * 10 ms��
	int leaf_size = 0;
	if(m_access_mod == SCSI_WRONLY)
	{
		//Flush_HeadInfo(true);
		m_bstop =true; //����ֹͣ¼�� ���ȴ�FIFO������д��ɺ��˳�
		while( m_status != FILE_WRITE_FAIL)
		{	/*
			�ȴ���дIO���
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

//data_size�� ����ΪС��¼���m_data_size��
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

	//��ס�����豸
	LOCK_DEV(m_plock);

	if(remain_size) //�ƶ�����ָ��󣬳���δ��������
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
			m_cur_rd_lba++;	//����ǰ��дLBA��1
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
	if(last_notidy_io)	//������ڷǶ�����������Ҫ����������
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
	//�������
	UNLOCK_DEV(m_plock);

	m_status = FILE_NORMAL;
	if(m_access_mod == SCSI_RDWR)
		m_cur_wt_lba = m_cur_rd_lba;

	return data_in;

ERR_OUT:
	//�������
	UNLOCK_DEV(m_plock);
	m_status = ERR_READ_FAIL;
	return ret;
}

//data_size�� ����ΪС��¼���m_data_size��
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

	if(m_status == ERR_WRITE_FAIL)	//ʹ��FIFO
	{
		TRACE_ERR("The record is already write fail.cur offset %d. data_size %d \n",m_cur_offset,data_size);
		return ERR_WRITE_FAIL;
	}
	m_status = FILE_WRITTING;
	/*
	  ������������ݣ�������ǰ����
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

	if(remain_size) //�ƶ�����ָ��󣬳���δ��������
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
			m_cur_wt_lba++;	//����ǰ��дLBA��1
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
	if(last_notidy_io)	//������ڷǶ�����������Ҫ����������
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
	//�������

	//UNLOCK_DEV(m_plock);	//�������
	if(m_access_mod == SCSI_RDWR)
		m_cur_rd_lba = m_cur_wt_lba;
	m_status = FILE_NORMAL;
	TIME_CNT_CURRENT(m_lastWtTime);
	return data_out;

ERR_OUT:
	//�������
	//UNLOCK_DEV(m_plock);	//�������

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
	else //ͬʱ��д
	{
		m_cur_rd_lba = m_cur_wt_lba = m_cur_offset/m_sec_size + m_lba_start;
	}

	return 0;
ERR_OUT:
	return ERR_SEEK_FAIL;
}


	/*
		Seek(SET) + Read �ϲ�����,
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

	//��ס�����豸
	LOCK_DEV(m_plock);

	if(remain_size) //�ƶ�����ָ��󣬳���δ��������
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
			cur_rw_lba++;	//����ǰ��дLBA��1
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

	if(last_notidy_io)	//������ڷǶ�����������Ҫ����������
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
	//�������
	UNLOCK_DEV(m_plock);

	m_status = FILE_NORMAL;

	return data_in;

ERR_OUT:
	//�������
	UNLOCK_DEV(m_plock);
	m_status = ERR_READ_FAIL;
	return ret;
}
/*
	Seek(SET) + Write �ϲ����������д��ķǶ������������� ��ȡ--�޸�--��д����� ��������������

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
	  ������������ݣ�������ǰ����
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

	if(remain_size) //�ƶ�����ָ��󣬳���δ��������
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
			cur_wt_lba++;	//����ǰ��дLBA��1
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

	if(last_notidy_io)	//������ڷǶ�����������Ҫ����������
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
	//�������
	UNLOCK_DEV(m_plock);

	if(m_access_mod == SCSI_RDWR)
		m_cur_rd_lba = cur_wt_lba;
	m_status = FILE_NORMAL;
	return data_out;

ERR_OUT:
	//�������
	UNLOCK_DEV(m_plock);

	m_status = ERR_WRITE_FAIL;
	return ERR_WRITE_FAIL;
}

//FIFO ���湦�ܽӿ�
//
/*
	�첽IO��������ʹ�õ�FIFO���������������뵽FIFO���У�д���̻߳᲻��ȥִ��д������
	*/
int CRecord_file::Write_FIFO( unsigned char * data_buf, uint32_t data_size)
{
	int ret;
	int wait_count = 300;			//���ݵײ�������� 4��һ��I֡������������ʱ��Ӧ�ö�����֡����
	uint32_t free_space = 0;
	// static int check_cnt = 0;
	//�����Ѿ�FIFO�г���дʧ�ܵ����
	if(m_status == FILE_WRITE_FAIL|| m_status == FILE_DEVICE_LOST)
	{	//Ӧ���ļ���;д��ʧ�ܣ��ٴν���FIFO����ʱֱ�ӷ���ʧ��
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
			while(wait_count)	//�ײ�����оƬ�����Ի���8�������ݣ������������������5����
			{
				if(wait_count == 0)
				{	//��ʱ֮������ѭ���������õ�ǰ����֡
					TRACE_ERR("CH %%d Wait time too long.Free space %u data size %u\n",m_channel,free_space,data_size);
					return FIFO_ERR_TIMEOUT;
				}

				//�����Ѿ�FIFO�г���дʧ�ܵ����
				if(m_status == FILE_WRITE_FAIL|| m_status == FILE_DEVICE_LOST)
				{	//Ӧ���ļ���;д��ʧ�ܣ��ٴν���FIFO����ʱֱ�ӷ���ʧ��

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
	unsigned char endFrameBuf[1024]= {0};		//����֡

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
	//��ͷ��Ϣ��������
	memcpy(m_phd_info->buf + offset,data_buf,data_size);
	m_phd_info->changed_part |= change_part;	//��Ǻ��Ѿ��ı�Ĳ���
	return data_size;
}

//�ؼ�������Ϣ����
int CRecord_file::Set_KeyFrameIdx(unsigned char* idx_buf,uint32_t idx_size,uint32_t framePos)
{
	if(m_phd_info)
	{
		unsigned char * pbuf = m_phd_info->buf + m_phd_info->keyFrame_cnt* 20 +m_phd_info->kFrame_offset;
		memcpy(pbuf,idx_buf,idx_size);
		m_phd_info->keyFrame_cnt++;
		m_phd_info->data_offset += idx_size;
		
		//����ϴεĹؼ�֡������Ϣû�б�д��Ͳ�����λ����Ϣ��������֡�ʵ�ʱ�����֡����д�����ļ����������ᵼ��
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

//ˢ���ļ�ͷ��Ϣ��I֡����������Ƶ��ʽ����Ƶ��ʽ��Ϣ
//
//	������õĹؼ�֡����ˢ�µ�������
//	��֤��������β���룬���̶ȵļ��ٲ����������µ����ܽ���

int CRecord_file::Flush_HeadInfo(bool bEndRecord)
{
	int ret = 0;

	int wt_size;	//д���С
	uint32_t start_addr,end_addr;
	unsigned char * pbuf;
	uint64_t write_lba;

	//�������ļ�ͷ��Ϣ�ı�ʱ����ʱˢ��ͷ��Ϣ��ȥ
	if(m_phd_info == NULL)
	{
		return 0;
	}
	//LOCK_DEV(m_plock);

	if( m_phd_info->changed_part & CHANGED_HEAD ||
		bEndRecord)
	{
		ret = m_scsi_dev->Write_try(m_lba_start,1,m_phd_info->buf,m_sec_size);	//��������д�룬ǰ���Ѿ�������������������Ϣ
		if(ret)
		{
			if(ret == SENSE_MEDIUM_ERR || ret == SENSE_HARDWARE_ERR)
			{
				//����������
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
	//ֻҪ������Ƶ��ʽ����Ƶ��ʽ��Ϣ�ı�ʱ����ʱˢ��
	if( m_phd_info->changed_part & CHANGED_VIDEO_FMT ||
		m_phd_info->changed_part & CHANGED_AUDIO_FMT)
	{
		write_lba = m_lba_start + BLK_COUNT(m_phd_info->buf_size - m_sec_size,m_sec_size);
		pbuf = m_phd_info->buf + m_phd_info->buf_size - m_sec_size;
		//�ļ�δ��д��
		ret = m_scsi_dev->Write_try(write_lba,1,pbuf,m_sec_size);	//��������д�룬ǰ���Ѿ�������������������Ϣ
		if(ret)
		{
			if(ret == SENSE_MEDIUM_ERR || ret == SENSE_HARDWARE_ERR)
			{
				//����������
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
		m_phd_info->changed_part &= ~CHANGED_VIDEO_FMT;// �����
		m_phd_info->changed_part &= ~CHANGED_AUDIO_FMT;//
	}

	//Ϊ�˼��ٴ���Ѱ�������Կ��ǽ�����дд���Ƶ�ʽ���
	if( (m_phd_info->changed_part & CHANGED_INDEX)&&
	    ((m_cur_offset >= m_phd_info->last_frameoOffset)|| bEndRecord)
	  )
	{   //���ǵ��ؼ�֡ÿ��4���ֻ����һ�Σ����Ա�Ҫ��ʱˢ�µ�����
		//�������صĵ�ַ
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
			{	//����������
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
		//�����ļ�д��
#if 0
		if(m_phd_info->keyFrame_cnt % 2 )
			TRACE_DBG("Flush the Idx data.	write_offset %d data_offset %d keyFrame_cnt %d",
							m_phd_info->write_offset,m_phd_info->data_offset,m_phd_info->keyFrame_cnt);
#endif
		//m_phd_info->write_offset = m_phd_info->data_offset;
		m_phd_info->write_offset = cur_data_offset;
		m_phd_info->changed_part &= ~CHANGED_INDEX ; //��������ı��־λ

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
		m_phd_info->buf = new unsigned char[m_phd_info->buf_size];	//��������Ļ������ͷ��Ϣ�������Լ���Ƶ֡��ʽ����Ƶ֡��ʽ
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

	//�ⲿ����������Ҫֱ�� д�뵽�ļ����еġ�
	memcpy(m_phd_info->buf, head_buf,head_size);
	write_size = m_phd_info->buf_size;	 //���뵽������������,δ���������������Ჹ��
  //  TRACE("++Start init Head buf size %d \n",m_phd_info->buf_size);
	LOCK_DEV(m_plock);
	//��¼���ļ�ͷ��ʼһ��д��
	ret = m_scsi_dev->Write(m_lba_start,m_phd_info->buf,write_size);
	UNLOCK_DEV(m_plock);
	if(ret)
	{
		if(ret == SENSE_MEDIUM_ERR || ret == SENSE_HARDWARE_ERR)
		{
			//����������
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
	//д��ɹ�
	return ret;
}

bool CRecord_file::IsTimeOut(int timeOutMS)
{
	pub_time_t cntTime;
	int timeDiff = 0;	//ʱ���

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

	//����С���̵�����
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
			//���崦��
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
		{	//ע��ִ�е������ʱ�����ݻ�ȫ��д����ɣ�֮����˳�����
			//С��һ������������£���д��һ����������֤β֡ȫ������
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
			//�����Ѿ���ǰ����д������
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
			curLBA += ioSecs;	//����LBA
		}

		pDestBuf =pBuf + lenHead + lenBody;
	}

	//��������Ѿ�������ȫ��д���
	if(0 == lenTail)
	{	//���������Ҫˢһ��֡���ݽ�ȥ
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
	else //lenTail != 0  δβ��������
	{	//������������д����������
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
	//�ɹ�����д��
	m_cur_offset = offset + retSize;
	m_status = FILE_NORMAL;

	TIME_CNT_CURRENT(m_lastWtTime);		// ��¼�ļ���ʱ�䡡����һ�м���û��

	return retSize;
}

