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
	CSCSI_Dev* m_scsi_dev;				//文件所属的硬盘
	CFIFO_buf*  m_pfifo_buf;			//FIFO Buf 用于缓冲录像，对齐录像数据到扇区的整数倍的
	uint32_t   m_sec_size;				//当前文件所在物理磁盘扇区
	unsigned long m_channel;			//视频通道
	int m_access_mod;					//读写控制
	uint64_t   m_lba_start;				//文件起始LBA
	uint32_t   m_data_size;				//文件总大小

	uint32_t   m_rw_trans_sec;			//每次传输扇区个数		可以设计成静态变量
	uint32_t   m_cur_offset;			//当前的位置，字节数
	uint64_t   m_cur_wt_lba;			//当前正在写入的LBA
	uint64_t   m_cur_rd_lba;			//当前正在读取的LBA
	int  m_status;						//0:表示正常 1：表示正在写入， 2：正在读取 小于0为错误码
	bool m_bstop;						//是否停止录像

	//数据缓冲区设计
	unsigned char*	m_align_buf;		//对齐BUF，标准配备,分区当前硬盘扇区的大小
	typedef struct  //只在写入时会用到，避免不必要的内存开销。
	{
		uint32_t buf_size;				//缓冲大小
		uint32_t kFrame_offset;			//关键帧索引的起始位置
		uint32_t idx_size;				//单个索引的大小。
		uint32_t write_offset;			//当前缓冲已经写入的数据位置
		uint32_t data_offset;			//最后索引帧offset
		uint16_t keyFrame_cnt;			//关键帧计数
		uint16_t changed_part;			//改变的部分，16bit,使用低4位， 位置1 文件头部，2 I帧索引 3 视频格式部分  4 格式部分
		uint32_t last_frameoOffset;		//视频关键帧的位置
		unsigned char*  buf;			//文件关键信息缓冲 包含文件头和 关键帧索引
	}RecInfo,* pRecInfo;
	//针对录像写入才去使用
	pRecInfo m_phd_info;
	pub_time_t m_lastWtTime;			//最后写入数据的时间
#ifdef _WIN32
	CRITICAL_SECTION *m_plock;				//互斥锁指针
#else
	pthread_mutex_t  *m_plock;				//互斥锁指针
#endif
	static uint32_t m_FIFO_SIZE;		//FIFO的大小 默认为 4M，
	static uint32_t m_IP_FIFO_SIZE;		//IPC通道，主码流的缓冲大小
	static uint32_t m_IP_MIN_FIFO_SIZE;	//IPC通道，第三码流的缓冲大小

	//专用于缓冲数据，补齐未尾及写入尾帧
	unsigned char*	m_cache_buf;		//对齐BUF，标准配备,分区当前硬盘扇区的大小
	uint32_t		m_cache_pos;		//已经缓冲好的数据量
public:	//非关键数据，直接设置为开，方便访问
	uint32_t   m_fifo_leaf;				//最后FIFO中剩余的数据大小
	bool	   m_bsuspense;				//超过三秒钟，剩余数据无变化，视为暂时停止录像，应该立即将数据写入到磁盘

public:
	//实现类似文件操作最基本的5个API fopen, fclose,fwrite,fread,fseek.
	int fopen(const char * dev_hdd,const char * file_name,int access_mod, FILE_BUF_TYPE fileBufType=FILE_BUF_TYPE_LOCAL);
	int close();
	//读写录像接口
	int Read(unsigned char * data_buf, uint32_t data_size);//data_size： 限制为小于录像的m_data_size，
	int Write(unsigned char * data_buf,uint32_t data_size);//data_size： 限制为小于录像的m_data_size，
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

public: //特殊用途接口
	//初始化录像文件头以及清理一些相关数据
	int InitHead(unsigned char * head_buf,uint32_t head_size,uint32_t idx_size,uint32_t clear_start, uint32_t clear_end);
	//索引
	int SetHeadData(HEAD_CHANGE_PART change_part, uint32_t offset,unsigned char * data_buf,uint32_t data_size);
	//复合型函数Read 一定程度上减少函数调用带来的开销
	/*
		Seek(SET) + Read 合并函数,
	*/
	int Read_pos(uint32_t offset,unsigned char * data_buf, uint32_t data_size);

	/* 复合型函数Write
		Seek(SET) + Write 合并函数，针对写入的非对齐情况，会进行 读取--修改--再写入操作 单个扇区的数据
	*/
	int Write_pos(uint32_t offset,unsigned char * data_buf, uint32_t data_size);

	//
	/*
		异步IO操作，会使用到FIFO操作，将数据送入到FIFO缓冲当中，而录像写入线程会调用Flush_FIFOData不断去执行写入数据
	*/
	int Write_FIFO( unsigned char * data_buf, uint32_t data_size);
	
	//按磁道顺序去刷新FIFO缓冲数据
	int Flush_FIFOData(int time_out, int cache_water);
	
	//关键索引信息缓冲
	int Set_KeyFrameIdx(unsigned char* idx_buf,uint32_t idx_size,uint32_t frmaePos);

	//刷新文件头及索引
	/*
		将缓冲好的关键帧索引刷新到磁盘上
		保证好扇区首尾对齐
	*/
	int  Flush_HeadInfo(bool bEndRecord);
	void Start_FlushData();
	bool IsTimeOut(int timeOutMS); //是否超指定时间没有写入数据

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
	//是否为同一个磁盘 
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
	//应用Win32平台上面的测试程序增强功能， 在嵌入式Linux尽量考虑生成的可执行代码精简
	int fopen_scsidev(const CSCSI_Dev* pdev,uint64_t file_lba,int32_t file_size,int access_mod);//打开录像文件
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
#define FILE_DEVICE_LOST	 4	//设备已经丢失

#endif //_RECORD_FILE_H
