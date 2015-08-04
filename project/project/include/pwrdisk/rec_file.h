#ifndef _RECORD_FILE_H
#define _RECORD_FILE_H

#include "scsi_dev.h"
#include "fifo_buf.h"
#include "dvrdvsdef.h"

enum HEAD_CHANGE_PART
{
	CHANGED_HEAD      = 0x01,
	CHANGED_INDEX     = 0x02,
	CHANGED_VIDEO_FMT = 0x04,
	CHANGED_AUDIO_FMT = 0x08,
	CHANGED_NULL	  = 0x00,
};

class CRecord_file
{
public:
	CRecord_file();
	~CRecord_file();

protected:
	CSCSI_Dev* m_scsi_dev;				//�ļ�������Ӳ��
	CFIFO_buf*  m_pfifo_buf;			//FIFO Buf ���ڻ���¼�񣬶���¼�����ݵ���������������
	uint32_t   m_sec_size;				//��ǰ�ļ����������������
	unsigned long m_channel;			//��Ƶͨ��
	int m_access_mod;					//��д����
	uint64_t   m_lba_start;				//�ļ���ʼLBA
	uint32_t   m_data_size;				//�ļ��ܴ�С

	uint32_t   m_rw_trans_sec;			//ÿ�δ�����������		������Ƴɾ�̬����
	uint32_t   m_cur_offset;			//��ǰ��λ�ã��ֽ���
	uint64_t   m_cur_wt_lba;			//��ǰ����д���LBA
	uint64_t   m_cur_rd_lba;			//��ǰ���ڶ�ȡ��LBA
	int  m_status;						//0:��ʾ���� 1����ʾ����д�룬 2�����ڶ�ȡ С��0Ϊ������
	bool m_bstop;						//�Ƿ�ֹͣ¼��

	//���ݻ��������
	unsigned char*	m_align_buf;		//����BUF����׼�䱸,������ǰӲ�������Ĵ�С
	typedef struct  //ֻ��д��ʱ���õ������ⲻ��Ҫ���ڴ濪����
	{
		uint32_t buf_size;				//�����С
		uint32_t kFrame_offset;			//�ؼ�֡��������ʼλ��
		uint32_t idx_size;				//���������Ĵ�С��
		uint32_t write_offset;			//��ǰ�����Ѿ�д�������λ��
		uint32_t data_offset;			//�������֡offset
		uint16_t keyFrame_cnt;			//�ؼ�֡����
		uint16_t changed_part;			//�ı�Ĳ��֣�16bit,ʹ�õ�4λ�� λ��1 �ļ�ͷ����2 I֡���� 3 ��Ƶ��ʽ����  4 ��ʽ����
		uint32_t last_frameoOffset;		//��Ƶ�ؼ�֡��λ��
		unsigned char*  buf;			//�ļ��ؼ���Ϣ���� �����ļ�ͷ�� �ؼ�֡����
	}RecInfo,* pRecInfo;
	//���¼��д���ȥʹ��
	pRecInfo m_phd_info;
	pub_time_t m_lastWtTime;			//���д�����ݵ�ʱ��
#ifdef _WIN32
	CRITICAL_SECTION *m_plock;				//������ָ��
#else
	pthread_mutex_t  *m_plock;				//������ָ��
#endif
	static uint32_t m_FIFO_SIZE;		//FIFO�Ĵ�С Ĭ��Ϊ 4M��
	static uint32_t m_IP_FIFO_SIZE;		//IPCͨ�����������Ļ����С
	static uint32_t m_IP_MIN_FIFO_SIZE;	//IPCͨ�������������Ļ����С

	//ר���ڻ������ݣ�����δβ��д��β֡
	unsigned char*	m_cache_buf;		//����BUF����׼�䱸,������ǰӲ�������Ĵ�С
	uint32_t		m_cache_pos;		//�Ѿ�����õ�������
public:	//�ǹؼ����ݣ�ֱ������Ϊ�����������
	uint32_t   m_fifo_leaf;				//���FIFO��ʣ������ݴ�С
	bool	   m_bsuspense;				//���������ӣ�ʣ�������ޱ仯����Ϊ��ʱֹͣ¼��Ӧ������������д�뵽����

public:
	//ʵ�������ļ������������5��API fopen, fclose,fwrite,fread,fseek.
	int fopen(const char * dev_hdd,const char * file_name,int access_mod, FILE_BUF_TYPE fileBufType=FILE_BUF_TYPE_LOCAL);
	int close();
	//��д¼��ӿ�
	int Read(unsigned char * data_buf, uint32_t data_size);//data_size�� ����ΪС��¼���m_data_size��
	int Write(unsigned char * data_buf,uint32_t data_size);//data_size�� ����ΪС��¼���m_data_size��
	int Seek(int32_t offset,int whence);

	static void SetBufSize(uint32_t size)
	{
		CRecord_file::m_FIFO_SIZE = size;
		CRecord_file::m_IP_FIFO_SIZE = size;
	}

	static void SetBufSize(uint32_t size, FILE_BUF_TYPE fileBufType)
	{
		if (FILE_BUF_TYPE_LOCAL == fileBufType)
		{
			CRecord_file::m_FIFO_SIZE = size;
		}
		else if (FILE_BUF_TYPE_IP == fileBufType)
		{
			CRecord_file::m_IP_FIFO_SIZE = size;
		}
		else if (FILE_BUF_TYPE_MIN == fileBufType)
		{
			CRecord_file::m_IP_MIN_FIFO_SIZE = size;
		}
		else
		{
			CRecord_file::m_FIFO_SIZE = size;
		}
	}

	static uint32_t GetBufSize(FILE_BUF_TYPE fileBufType=FILE_BUF_TYPE_LOCAL)
	{
		if (FILE_BUF_TYPE_LOCAL == fileBufType)
		{
			return CRecord_file::m_FIFO_SIZE;
		}
		else if (FILE_BUF_TYPE_IP == fileBufType)
		{
			return CRecord_file::m_IP_FIFO_SIZE;
		}
		else if (FILE_BUF_TYPE_MIN == fileBufType)
		{
			return CRecord_file::m_IP_MIN_FIFO_SIZE;
		}
		else
		{
			return CRecord_file::m_FIFO_SIZE;
		}
	}

public: //������;�ӿ�
	//��ʼ��¼���ļ�ͷ�Լ�����һЩ�������
	int InitHead(unsigned char * head_buf,uint32_t head_size,uint32_t idx_size,uint32_t clear_start, uint32_t clear_end);
	//����
	int SetHeadData(HEAD_CHANGE_PART change_part, uint32_t offset,unsigned char * data_buf,uint32_t data_size);
	//�����ͺ���Read һ���̶��ϼ��ٺ������ô����Ŀ���
	/*
		Seek(SET) + Read �ϲ�����,
	*/
	int Read_pos(uint32_t offset,unsigned char * data_buf, uint32_t data_size);

	/* �����ͺ���Write
		Seek(SET) + Write �ϲ����������д��ķǶ������������� ��ȡ--�޸�--��д����� ��������������
	*/
	int Write_pos(uint32_t offset,unsigned char * data_buf, uint32_t data_size);

	//
	/*
		�첽IO��������ʹ�õ�FIFO���������������뵽FIFO���嵱�У���¼��д���̻߳����Flush_FIFOData����ȥִ��д������
	*/
	int Write_FIFO( unsigned char * data_buf, uint32_t data_size);
	
	//���ŵ�˳��ȥˢ��FIFO��������
	int Flush_FIFOData(int time_out, int cache_water);
	
	//�ؼ�������Ϣ����
	int Set_KeyFrameIdx(unsigned char* idx_buf,uint32_t idx_size,uint32_t frmaePos);

	//ˢ���ļ�ͷ������
	/*
		������õĹؼ�֡����ˢ�µ�������
		��֤��������β����
	*/
	int  Flush_HeadInfo(bool bEndRecord);
	void Start_FlushData();
	bool IsTimeOut(int timeOutMS); //�Ƿ�ָ��ʱ��û��д������

	bool FlushDataOK(uint32_t cache_water)
	{
		if(m_bstop || (m_pfifo_buf->GetCurWarter() >= cache_water))
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	int WriteCachePos(uint32_t offset, unsigned char * pBuf,uint32_t dataSize);
public:
	//�Ƿ�Ϊͬһ������ 
	bool isSameDisk(CSCSI_Dev * pDev)
	{
		if(pDev == m_scsi_dev)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	void set_resualt(int ret)
	{
		m_status = ret;
	}

	void stop()
	{
		m_bstop = true;
	}

	bool is_closed()
	{
		if(m_access_mod == -1)
		{
			return true;
		}
		return false;
	}

	uint32_t get_cur_offset()
	{
		return m_cur_offset;
	}

	uint32_t is_stop_rec()
	{
		return m_bstop;
	}

	uint64_t get_cur_rw_lba()
	{
		if(m_access_mod == 0)
			return m_cur_rd_lba;
		else
			return m_cur_wt_lba;
	}

	uint32_t get_sec_size()
	{
		return m_sec_size;
	}

	CFIFO_buf* GetFifoBuf()
	{
		return	m_pfifo_buf;
	}

	uint64_t get_lba_start()
	{
		return m_lba_start;
	}

	unsigned long get_channel()
	{
		return m_channel;
	}

	void set_channel(unsigned long channel)
	{
		m_channel = channel;
	}

#ifdef _WIN32 
	//Ӧ��Win32ƽ̨����Ĳ��Գ�����ǿ���ܣ� ��Ƕ��ʽLinux�����������ɵĿ�ִ�д��뾫��
	int fopen_scsidev(const CSCSI_Dev* pdev,uint64_t file_lba,int32_t file_size,int access_mod);//��¼���ļ�
public:

	uint64_t get_lba_end()
	{
		return m_lba_start + m_data_size/m_sec_size;
	}

	uint32_t get_data_size()
	{
		return m_data_size;
	}

	uint32_t get_data_secotr()
	{
		return	m_data_size/m_sec_size;
	}

	unsigned char * get_align_buf()
	{
		return m_align_buf;
	}

	uint32_t get_free_size()
	{
		return m_data_size - m_cur_offset;
	}
#endif
};

#define FILE_NORMAL			 0
#define FILE_WRITTING		 1
#define FILE_READING		 2
#define FILE_WRITE_FAIL		 3
#define FILE_DEVICE_LOST	 4	//�豸�Ѿ���ʧ

#endif //_RECORD_FILE_H
