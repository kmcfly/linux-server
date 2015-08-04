#ifndef	_SCSI_DEV_H
#define _SCSI_DEV_H

#include "cross_typedef.h"

#ifdef _WIN32
#include <ntddscsi.h>
#include <windows.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "win32err.h"

#include <locale.h>

typedef enum
{
	IO_NONE = SCSI_IOCTL_DATA_UNSPECIFIED,
	IO_READ = SCSI_IOCTL_DATA_IN,
	IO_WRITE = SCSI_IOCTL_DATA_OUT
} IO_Direction;

typedef struct
{
	SCSI_PASS_THROUGH_DIRECT	spt;
	unsigned char		sense[18];
} SPKG;

#else //_Linux

#include <unistd.h>
#include <scsi/sg.h>
#include <stdint.h>
#ifndef EMEDIUMTYPE
#define EMEDIUMTYPE	EINVAL
#endif
#ifndef	ENOMEDIUM
#define	ENOMEDIUM	ENODEV
#endif
typedef enum
{
	IO_NONE=SG_DXFER_NONE,	// 3
	IO_READ=SG_DXFER_FROM_DEV,	// 2
	IO_WRITE=SG_DXFER_TO_DEV// 1
} IO_Direction;
#endif

#define SCSI_CREAM_ON_ERRNO_NAKED(s)				\
	switch ((s)[12])					\
	{	case 0x04:	errno=EAGAIN;	break;		\
	case 0x20:	errno=ENODEV;	break;		\
	case 0x21:	if ((s)[13]==0)	errno=ENOSPC;	\
			else		errno=EINVAL;	\
			break;				\
	case 0x30:	errno=EMEDIUMTYPE;	break;	\
	case 0x3A:	errno=ENOMEDIUM;	break;	\
	}
#define SCSI_CREAM_ON_ERRNO(s)	do { SCSI_CREAM_ON_ERRNO_NAKED(s) } while(0)

#define SCSI_ERRCODE(s)	((((s)[2]&0x0F)<<16)|((s)[12]<<8)|((s)[13]))
#define	SCSI_SK(errcode)	(((errcode)>>16)&0xF)
#define	SCSI_ASC(errcode)	(((errcode)>>8)&0xFF)
#define SCSI_ASCQ(errcode)	((errcode)&0xFF)

void sperr(const char *cmd,int err);
void sperror_bysense (const char *cmd,unsigned char *sense);


typedef enum
{
	SENSE_NO_SENSE		  = 0x00,
	SENSE_RECOVERED_ERR   = 0x01,
	SENSE_NOT_READY       = 0x02,
	SENSE_MEDIUM_ERR      = 0x03,	//磁盘媒介错误
	SENSE_HARDWARE_ERR    = 0x04,	//磁盘设备硬件错误
	SENSE_ILLEGAL_REQUEST = 0x05,
	SENSE_UINT_AITENTION  = 0x06,
	SENSE_DATA_PROTECT    = 0x07,
	SENSE_BLANK_CHECK	  = 0x08,
	SENSE_VENDOR_SPECIFIC = 0x09 ,
	SENSE_COPY_ABORTED	  = 0x0A,
	SENSE_ABORTED_COMMAND = 0x0B,
	SENSE_OBSOLETE		  = 0x0C,
	SENSE_VOLUME_OVERFLOW = 0x0D,
	SENSE_MISCOMPARE	  = 0x0E,
	SENSE_RESERVED		  = 0x0F,
}ERR_SENSE_KEY_E;


#ifdef _USRDLL
class __declspec(dllexport) CSCSI_Dev
#else
class CSCSI_Dev
#endif
{
protected:
#ifdef _WIN32
	HANDLE fd;
	SPKG   p;
	unsigned char * cmd;			//cbd
	CRITICAL_SECTION m_lock;		//互斥锁
#else //_linux
	unsigned char cmd[16];			//cdb缓冲
	unsigned char _sense[18];		//错误信息缓冲
	struct sg_io_hdr	m_sg_io;	//Linux通用sg IO 结构对象
	int	  fd;						//设备句柄
	pthread_mutex_t  m_lock; 		//互斥锁
#endif
	int    autoclose;		//是否自动关闭句柄
	char  dev_name[20];		//设备名
	char  dev_no;			//设备名 最后一个字符

	uint64_t m_capacity;	//磁盘的总容量，扇区数
	uint32_t m_sec_size;	//磁盘的扇区大小
	bool is_2TB_big;		//是否为2TB以上的硬盘
	int m_ret;				//返回值
	int m_last_err;			//最后错误值
	int m_retry_times;		//重试次数

protected:
	int transport_nonedata();//此函数表示不带缓冲的命令
	int transport(IO_Direction dir,void *buf,size_t sz);
public:
	CSCSI_Dev();
	CSCSI_Dev(void*f);
	~CSCSI_Dev();

public:
	int associate(const char *file);
#ifdef _WIN32
	HANDLE Open();
#else
	int Open();								//内部打开设备
#endif
	void Close();
	int wait_for_unit(volatile int *progress);
	int pioneer_start();
	int pioneer_stop(volatile int *progress);

	int init_devlock();
public://SCSI设备通用命令，支持硬盘与光盘等
	int test_unit_ready();							//检验设备是否已经准备好。
	int inquiry(unsigned char* inqBuf ,int bufSize);//最基本设备的响应命令
	int start_stop_unit(int unitType,unsigned char isImmed);
	int request_sense(unsigned char * buf,int bufSize);	 //获取响应信息
	int synchronize_cache(unsigned char isImmed); //同步硬盘缓冲到硬盘盘片上面
	int seek_10(unsigned int lba);

	int read_capacity_10(unsigned char* buf, uint32_t buf_size);
	int read_capacity_16(unsigned char* buf, uint32_t buf_size);

	int write_10(uint32_t lba,uint32_t sec_count,unsigned char* data_buf,uint32_t data_size);
	int read_10(uint32_t lba,uint32_t sec_count,unsigned char* data_buf,uint32_t data_size);

	int write_16(uint64_t lba,uint32_t sec_count,unsigned char* data_buf,uint32_t data_size);
	int read_16(uint64_t lba,uint32_t sec_count,unsigned char* data_buf,uint32_t data_size);

public:	//调用如下接口不用考虑LBA超2TB大小的问题。
	int Write(uint64_t lba,uint32_t sec_count,unsigned char* data_buf,uint32_t data_size);	//Write
	int Read(uint64_t lba,uint32_t sec_count,unsigned char* data_buf,uint32_t data_size);	//Read
	
	//可以不用对齐到512或4096
	int Write(int64_t lba,unsigned char* data_buf,uint32_t data_size);	//写入固定字节 
	int Read(int64_t lba,unsigned char* data_buf,uint32_t data_size);	//读取固定字节 
	
	uint64_t ReadCapacity(uint32_t * psec_size);												//返回总共多少个扇区,如果传入扇区大小指定，同样获取到扇区大小

	//以下两个函数会失败后自动重试默认的次数
	int Read_try(uint64_t lba, uint32_t sec_count,unsigned char* data_buf,uint32_t data_size);
	int Write_try(uint64_t lba,uint32_t sec_count,unsigned char* data_buf,uint32_t data_size);
	bool FlushCache();

public:
	char* get_device_name()	//获取当前设备的名称
	{
		return dev_name;
	}

	char get_dev_no()	//获取当前设备的名称
	{	
		if(dev_no)
			return dev_no;
		else
			return dev_name[strlen(dev_name) - 1];
	}

	bool is_closed()
	{
#ifdef _WIN32
		if(fd ==(HANDLE)-1)
#else
		if(fd == -1)
#endif
			return true;
		else
			return false;
	}

	inline void set_timeout(int i)
	{
#ifdef _WIN32
		p.spt.TimeOutValue = i;
#else
		m_sg_io.timeout = i;
#endif
	}

	inline unsigned char &operator()(size_t i)
	{
#ifdef _WIN32
		return p.sense[i];
#else
		return _sense[i];
#endif
	}

	inline unsigned char *get_sense()
	{
#ifdef _WIN32
		return p.sense;
#else
		return _sense;
#endif
	}

	inline bool is_bigger_2TB()
	{
		return is_2TB_big;
	}

	inline uint32_t get_capacity_gB() //获取磁盘标称容量 1000 = 1kB  1000 kB= 1 mB  1000 mB= 1gB
	{
		return (uint32_t)((m_capacity*512)/1000000000);
	}

	inline uint64_t get_capacity()	//获取磁盘的总扇区数
	{
		return m_capacity;
	}

	inline uint32_t get_sec_size()
	{
		if(m_sec_size == 0)
			return 512;
		return m_sec_size;
	}
	inline int get_last_err()
	{
		return m_last_err;
	}
#ifdef _WIN32
	inline CRITICAL_SECTION* get_lock()
	{
		return &m_lock;
	}
#else
	inline pthread_mutex_t* get_lock()
	{
		return &m_lock;
	}
#endif
};
#define	UNIT_STOP   0x00
#define	UNIT_START  0x01
#define	UNIT_EJECT  0x02
#define	UNIT_LOAD   0x03

//错误
enum ERR_TYPE
{	
	//参数错误
	ERR_NO_DEVICE		=   -2010,	//设备为空
	ERR_LBA_OVERFLOW	=   -2009,  //扇区地址超过了硬盘的最大扇区
	ERR_LBA_ACCES_MBR	=	-2008,	//非法访问零扇区
	ERR_NO_RECFILE		=   -2007,	//录像文件指针为空
	ERR_ACCESS_MOD		=	-2006,	//错误的访问方式
	ERR_DATA_BUF_EMPYT	=	-2005,	//数据缓冲为空
	ERR_DATA_OVER_FILE	=	-2004,	//数据量超过文件大小
	//IO错误
	ERR_BAD_SETOR		=   -1005,	//出现硬盘坏道
	ERR_HDD_HARDWARE	=   -1004,	//出现硬盘硬件故障
	ERR_WRITE_FAIL		=	-1003,	//写失败
	ERR_READ_FAIL		=	-1002,	//读写失败
	ERR_SEEK_FAIL		=   -1001,	//移动数据指针失败
	//
	NO_EER = 0,						//正常没有错误
};


#define SCSI_SEEK_SET	0	/* seek relative to beginning of file */
#define SCSI_SEEK_CUR	1	/* seek relative to current file position */
#define SCSI_SEEK_END	2	/* seek relative to end of file*/

#define	 SCSI_RDONLY    0x00  //只读模式
#define  SCSI_WRONLY	0x01  //只写模式
#define  SCSI_RDWR      0x02  //读写模式

#define SIZE_SECTOR_2TB	 0xFFFFFFFF

#endif	// _SCSI_DEV_H