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
	SENSE_MEDIUM_ERR      = 0x03,	//����ý�����
	SENSE_HARDWARE_ERR    = 0x04,	//�����豸Ӳ������
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
	CRITICAL_SECTION m_lock;		//������
#else //_linux
	unsigned char cmd[16];			//cdb����
	unsigned char _sense[18];		//������Ϣ����
	struct sg_io_hdr	m_sg_io;	//Linuxͨ��sg IO �ṹ����
	int	  fd;						//�豸���
	pthread_mutex_t  m_lock; 		//������
#endif
	int    autoclose;		//�Ƿ��Զ��رվ��
	char  dev_name[20];		//�豸��
	char  dev_no;			//�豸�� ���һ���ַ�

	uint64_t m_capacity;	//���̵���������������
	uint32_t m_sec_size;	//���̵�������С
	bool is_2TB_big;		//�Ƿ�Ϊ2TB���ϵ�Ӳ��
	int m_ret;				//����ֵ
	int m_last_err;			//������ֵ
	int m_retry_times;		//���Դ���

protected:
	int transport_nonedata();//�˺�����ʾ�������������
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
	int Open();								//�ڲ����豸
#endif
	void Close();
	int wait_for_unit(volatile int *progress);
	int pioneer_start();
	int pioneer_stop(volatile int *progress);

	int init_devlock();
public://SCSI�豸ͨ�����֧��Ӳ������̵�
	int test_unit_ready();							//�����豸�Ƿ��Ѿ�׼���á�
	int inquiry(unsigned char* inqBuf ,int bufSize);//������豸����Ӧ����
	int start_stop_unit(int unitType,unsigned char isImmed);
	int request_sense(unsigned char * buf,int bufSize);	 //��ȡ��Ӧ��Ϣ
	int synchronize_cache(unsigned char isImmed); //ͬ��Ӳ�̻��嵽Ӳ����Ƭ����
	int seek_10(unsigned int lba);

	int read_capacity_10(unsigned char* buf, uint32_t buf_size);
	int read_capacity_16(unsigned char* buf, uint32_t buf_size);

	int write_10(uint32_t lba,uint32_t sec_count,unsigned char* data_buf,uint32_t data_size);
	int read_10(uint32_t lba,uint32_t sec_count,unsigned char* data_buf,uint32_t data_size);

	int write_16(uint64_t lba,uint32_t sec_count,unsigned char* data_buf,uint32_t data_size);
	int read_16(uint64_t lba,uint32_t sec_count,unsigned char* data_buf,uint32_t data_size);

public:	//�������½ӿڲ��ÿ���LBA��2TB��С�����⡣
	int Write(uint64_t lba,uint32_t sec_count,unsigned char* data_buf,uint32_t data_size);	//Write
	int Read(uint64_t lba,uint32_t sec_count,unsigned char* data_buf,uint32_t data_size);	//Read
	
	//���Բ��ö��뵽512��4096
	int Write(int64_t lba,unsigned char* data_buf,uint32_t data_size);	//д��̶��ֽ� 
	int Read(int64_t lba,unsigned char* data_buf,uint32_t data_size);	//��ȡ�̶��ֽ� 
	
	uint64_t ReadCapacity(uint32_t * psec_size);												//�����ܹ����ٸ�����,�������������Сָ����ͬ����ȡ��������С

	//��������������ʧ�ܺ��Զ�����Ĭ�ϵĴ���
	int Read_try(uint64_t lba, uint32_t sec_count,unsigned char* data_buf,uint32_t data_size);
	int Write_try(uint64_t lba,uint32_t sec_count,unsigned char* data_buf,uint32_t data_size);
	bool FlushCache();

public:
	char* get_device_name()	//��ȡ��ǰ�豸������
	{
		return dev_name;
	}

	char get_dev_no()	//��ȡ��ǰ�豸������
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

	inline uint32_t get_capacity_gB() //��ȡ���̱������ 1000 = 1kB  1000 kB= 1 mB  1000 mB= 1gB
	{
		return (uint32_t)((m_capacity*512)/1000000000);
	}

	inline uint64_t get_capacity()	//��ȡ���̵���������
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

//����
enum ERR_TYPE
{	
	//��������
	ERR_NO_DEVICE		=   -2010,	//�豸Ϊ��
	ERR_LBA_OVERFLOW	=   -2009,  //������ַ������Ӳ�̵��������
	ERR_LBA_ACCES_MBR	=	-2008,	//�Ƿ�����������
	ERR_NO_RECFILE		=   -2007,	//¼���ļ�ָ��Ϊ��
	ERR_ACCESS_MOD		=	-2006,	//����ķ��ʷ�ʽ
	ERR_DATA_BUF_EMPYT	=	-2005,	//���ݻ���Ϊ��
	ERR_DATA_OVER_FILE	=	-2004,	//�����������ļ���С
	//IO����
	ERR_BAD_SETOR		=   -1005,	//����Ӳ�̻���
	ERR_HDD_HARDWARE	=   -1004,	//����Ӳ��Ӳ������
	ERR_WRITE_FAIL		=	-1003,	//дʧ��
	ERR_READ_FAIL		=	-1002,	//��дʧ��
	ERR_SEEK_FAIL		=   -1001,	//�ƶ�����ָ��ʧ��
	//
	NO_EER = 0,						//����û�д���
};


#define SCSI_SEEK_SET	0	/* seek relative to beginning of file */
#define SCSI_SEEK_CUR	1	/* seek relative to current file position */
#define SCSI_SEEK_END	2	/* seek relative to end of file*/

#define	 SCSI_RDONLY    0x00  //ֻ��ģʽ
#define  SCSI_WRONLY	0x01  //ֻдģʽ
#define  SCSI_RDWR      0x02  //��дģʽ

#define SIZE_SECTOR_2TB	 0xFFFFFFFF

#endif	// _SCSI_DEV_H