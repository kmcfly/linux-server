#include "PUB_common.h"	// _WIN32 need file StdAfx.h
#include "asctable.h"
#include "scsi_dev.h"


#ifdef _WIN32
#include <winioctl.h>
#pragma warning(disable: 4244)
#pragma warning(disable: 4496)
#define poll(a,b,t)	Sleep(t)
#define getmsecs()	GetTickCount()

#define  INIT_DATA(len)							 \
{\
	memset(&p,0,sizeof(p));						\
	p.spt.Length = sizeof(p.spt);				\
	p.spt.DataIn = SCSI_IOCTL_DATA_UNSPECIFIED;	\
	p.spt.TimeOutValue = 30;					\
	p.spt.SenseInfoLength = sizeof(p.sense);	\
	p.spt.SenseInfoOffset = offsetof(SPKG,sense);\
	cmd = p.spt.Cdb;							\
	p.spt.CdbLength = len;						\
}

int CSCSI_Dev::transport_nonedata()
{
	DWORD bytes;
	int   ret = 0;

	if (DeviceIoControl (fd,IOCTL_SCSI_PASS_THROUGH_DIRECT,
		&p,sizeof(p.spt),
		&p,sizeof(p),
		&bytes,FALSE) == 0)
	{
		TRACE("Device Io Cnotrol fail,err %d",GetLastError());
		return -1;
	}

	if (p.sense[0]&0x70)
	{
		SetLastError (ERROR_GEN_FAILURE);
		m_last_err = SCSI_ERRCODE(p.sense);
		ret = SCSI_SK(m_last_err);
		if (m_last_err == 0)
			ret = -1;
		else
			SCSI_CREAM_ON_ERRNO(p.sense);
	}

	return ret;
}

int CSCSI_Dev::transport(IO_Direction dir,void *buf,size_t sz)
{
	DWORD bytes;
	int   ret = 0;

	p.spt.DataBuffer = buf;
	p.spt.DataTransferLength = (ULONG)sz;
	p.spt.DataIn = dir;

	if (DeviceIoControl (fd,IOCTL_SCSI_PASS_THROUGH_DIRECT,
		&p,sizeof(p.spt),
		&p,sizeof(p),
		&bytes,FALSE) == 0)
	{
		ret = GetLastError();
		return -1;
	}

	if (p.sense[0]&0x70)
	{
		SetLastError (ERROR_GEN_FAILURE);
		m_last_err = SCSI_ERRCODE(p.sense);
		ret = SCSI_SK(m_last_err);
		if (m_last_err == 0)
			ret = -1;
		else
			SCSI_CREAM_ON_ERRNO(p.sense);
	}

	return ret;
}

int CSCSI_Dev::associate(const char *file)
{
	char dev[32];
	sprintf(dev,"\\\\.\\%.*s",sizeof(dev)-5,file);
	fd = CreateFileA(dev,GENERIC_WRITE|GENERIC_READ,FILE_SHARE_READ|FILE_SHARE_WRITE,
		NULL,OPEN_EXISTING,0,NULL);
	if (fd != INVALID_HANDLE_VALUE)
		strcpy(dev_name,file);
	else
		return -1;
	ReadCapacity(NULL);
	return (int)fd;
}

#else
#include "scsi_dev.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/time.h>
#include <poll.h>
#include <fcntl.h>
#include <sys/ioctl.h>

#if !defined(SG_FLAG_LUN_INHIBIT)
# if defined(SG_FLAG_UNUSED_LUN_INHIBIT)
#  define SG_FLAG_LUN_INHIBIT SG_FLAG_UNUSED_LUN_INHIBIT
# else
#  define SG_FLAG_LUN_INHIBIT 0
# endif
#endif
#ifndef CHECK_CONDITION
#define CHECK_CONDITION 0x01
#endif

#define  INIT_DATA(len)					  \
{\
	memset(&m_sg_io, 0x00, sizeof(struct sg_io_hdr));\
	memset(cmd, 0x00, 16);		    \
	m_sg_io.interface_id = 'S';			  \
	m_sg_io.mx_sb_len = 18;		  \
	m_sg_io.cmdp = (unsigned char *)cmd;    \
	m_sg_io.sbp	= _sense;               \
	m_sg_io.flags = SG_FLAG_LUN_INHIBIT | SG_FLAG_DIRECT_IO;\
	m_sg_io.timeout = 5000;					  \
	m_sg_io.cmd_len = len;				  \
}

static inline long getmsecs()
{
	struct timeval tv;
	gettimeofday (&tv,NULL);
	return tv.tv_sec*1000+tv.tv_usec/1000;
}
int CSCSI_Dev::transport_nonedata()
{
	m_ret = 0;
	m_sg_io.dxfer_direction =  IO_NONE;

	if (ioctl (fd,SG_IO,&m_sg_io))
	{
		printf("Last error %d %s",errno,strerror(errno));
		if(errno == 9)
		{	//出现文件句柄错误时  9 Bad file descriptorLast 尝试重新open设备
			Open();
		}
		m_last_err = errno;
		return -1;
	}
	if ((m_sg_io.info&SG_INFO_OK_MASK) != SG_INFO_OK)
	{
		errno = EIO;
		m_ret=-1;
		if (m_sg_io.masked_status&CHECK_CONDITION)
		{
			m_last_err = SCSI_ERRCODE(_sense);
			m_ret = SCSI_SK(m_last_err);
			if (m_last_err==0)
				m_ret=-1;
			else
				SCSI_CREAM_ON_ERRNO(_sense);
			m_ret = SCSI_SK(m_last_err);
		}
	}
	return m_ret;
}

int CSCSI_Dev::transport(IO_Direction dir,void *buf,size_t sz)
{
	m_ret = 0;

	m_sg_io.dxfer_direction =  dir;
	m_sg_io.dxferp = buf;
	m_sg_io.dxfer_len = sz;

	if (ioctl (fd,SG_IO,&m_sg_io))
	{
		printf("Last error %d %s",errno,strerror(errno));
		if(errno == 9)
		{	//出现文件句柄错误时  9 Bad file descriptorLast 尝试重新open设备
			Open();
		}
		m_last_err = errno;
		return -1;
	}

	if ((m_sg_io.info&SG_INFO_OK_MASK) != SG_INFO_OK)
	{
		errno = EIO;
		m_ret=-1;
		if (m_sg_io.masked_status&CHECK_CONDITION)
		{
			m_last_err = SCSI_ERRCODE(_sense);
			m_ret = SCSI_SK(m_last_err);
			if (m_last_err == 0)
				m_ret=-1;
			else
				SCSI_CREAM_ON_ERRNO(_sense);

		}
	}

	return m_ret;
}
int CSCSI_Dev::associate(const char *device)
{
    struct stat sb;

    /*
     * O_RDWR is expected to provide for none set-root-uid
     * execution under Linux kernel 2.6[.8]. Under 2.4 it
     * falls down to O_RDONLY...
     */
    if ((fd = open(device, O_RDWR | O_NONBLOCK)) < 0 &&
            (fd = open(device, O_RDONLY | O_NONBLOCK)) < 0)
    {
        printf("open[%s] error: %d, %s\n", device, errno, strerror(errno));
        return -1;
    }
    if (fstat(fd, &sb) < 0)
    {
        printf("fstat error: %d, %s\n", errno, strerror(errno));
        goto ERR_OUT;
    }

    if (!S_ISBLK(sb.st_mode))
    {
        printf("not S_ISBLK\n");
        errno = ENOTBLK;
        goto ERR_OUT;
    }

    ReadCapacity(NULL);
    strcpy(dev_name, device);
    dev_no = dev_name[strlen(dev_name)-1];
    printf("Open device %s is ok.Devno %c .\n",device,dev_no);

    return fd;

ERR_OUT:
    close(fd);
    return -1;
}
#endif

CSCSI_Dev::CSCSI_Dev(void)
{
#ifdef _WIN32
	fd = INVALID_HANDLE_VALUE;
#else
	 fd = -1;
#endif
	autoclose = 1;
	is_2TB_big = false;
	m_capacity = 0;
	m_sec_size = 512;
	m_retry_times = 3;
	memset(dev_name,0,20);
}

CSCSI_Dev::CSCSI_Dev(void*f)
{
#ifdef _WIN32
	fd = f;
#else
	fd = (int)f;
#endif
	autoclose = 0;
	memset(dev_name,0,20);
}

CSCSI_Dev::~CSCSI_Dev()
{
#ifdef _WIN32
	DWORD junk;
	if (fd != INVALID_HANDLE_VALUE && autoclose)
	{
		if (autoclose > 1)
		{
			DeviceIoControl(fd,FSCTL_UNLOCK_VOLUME,
				NULL,0,NULL,0,&junk,NULL);
			CloseHandle (fd),fd = INVALID_HANDLE_VALUE;
		}
	}
#else
	if(autoclose && fd != -1)
		Close();
#endif
}

#ifdef _WIN32
HANDLE CSCSI_Dev::Open()
{
	autoclose = 0;
	if(fd == (HANDLE)-1)
	{
		fd = CreateFileA(dev_name,GENERIC_WRITE|GENERIC_READ,FILE_SHARE_READ|FILE_SHARE_WRITE,
			NULL,OPEN_EXISTING,0,NULL);
	}
	return fd;
}
#else
int CSCSI_Dev::Open()
{
	autoclose = 0;
	if ((fd=open (dev_name,O_RDWR|O_NONBLOCK)) < 0 &&
		(fd=open (dev_name,O_RDONLY|O_NONBLOCK)) < 0)
		return 0;
	return fd;
}
#endif


void CSCSI_Dev::Close()
{
#ifdef _WIN32
	autoclose = 0;
	CloseHandle(fd);
	fd = INVALID_HANDLE_VALUE;
#else
	close(fd);
	fd = -1;
#endif
}


int CSCSI_Dev::init_devlock()	//获得一个设备锁	//初始化线程锁
{
#ifdef _WIN32
	//初始化线程锁互斥锁
	InitializeCriticalSection(&m_lock);
#else
	pthread_mutexattr_t mutexAttr;
	pthread_mutexattr_init(&mutexAttr);
	pthread_mutexattr_settype(&mutexAttr, PTHREAD_MUTEX_RECURSIVE);
	pthread_mutex_init(&m_lock, &mutexAttr);
	pthread_mutexattr_destroy(&mutexAttr);
#endif
	return 0;
}

void sperr (const char *cmd,int err)
{
	int saved_errno=errno;
	const char *msg;

	if (err == -1)
	{
		fprintf (stderr,":-( unable to %s: ",cmd);
	}
	else if ((msg = ASC_lookup(err))!=NULL)
	{
		fprintf (stderr,":-[ %s failed with SK=%Xh/%s]: ",cmd,SCSI_SK(err),msg);
	}
	else
	{
		fprintf (stderr,":-[ %s failed with SK=%Xh/ASC=%02Xh/ACQ=%02Xh]: ",
			 cmd,SCSI_SK(err),SCSI_ASC(err),SCSI_ASCQ(err));
		errno = saved_errno, perror (NULL);
	}
}

void sperror_bysense(const char *cmd,unsigned char *sense)
{
	int saved_errno = errno;
	int err = SCSI_ERRCODE(sense);

	if (err == 0)
	{
		fprintf (stderr,":-( unable to %s: ",cmd);
	}
	else
	{
		if ((err == 0x20407 || err == 0x20408) && sense[15]&0x80)
		{	fprintf (stderr,":-[ %s failed with SK=%Xh/ASC=%02Xh/ACQ=%02Xh@%.1f%%]: ",
			 cmd,SCSI_SK(err),SCSI_ASC(err),SCSI_ASCQ(err),
			 100.0*(sense[16]<<8|sense[17])/65536.0);
		}
		else
		{   errno = saved_errno;
			sperr (cmd,err);
			return;
		}
	}
	errno = saved_errno, perror (NULL);
}

int CSCSI_Dev::test_unit_ready()
{
	//init_data();
	INIT_DATA(6)
	// Wait until it returns either ready or not ready/medium not present
	cmd[0] = 0x00;	// Test unit ready
	cmd[5] = 0;

	return transport_nonedata();
}

int CSCSI_Dev::inquiry(unsigned char* inqBuf ,int bufSize)
{
	//init_data();
	INIT_DATA(6)

	cmd[0] = 0x12;	// INQUIRY
	cmd[4] = 36;
	cmd[5] = 0;

	return transport(IO_READ, inqBuf, 36);
}

int CSCSI_Dev::synchronize_cache(unsigned char isImmed)
{
	//init_data();
	INIT_DATA(6)
	cmd[0] = 0x35;		// FLUSH CACHE
	if (!isImmed)
	{	/*If Immed (Immediate) is set to zero, status shall be returned only after the operation is completed.
	    If Immed is set to one, status shall be returned as soon as the CDB has been validated.*/
		cmd[1] = 0x02;		// "IMMED"
	}
	cmd[9] = 0;

	return transport_nonedata();
}

int CSCSI_Dev::seek_10(unsigned int lba)
{
	INIT_DATA(10)

	cmd[0] = 0x2B; // SEEK 10

	cmd[2] = lba>>24;
	cmd[3] = lba>>16;
	cmd[4] = lba>>8;
	cmd[5] = lba;

	return transport_nonedata();
}

int CSCSI_Dev::request_sense(unsigned char * buf,int bufSize)
{
	INIT_DATA(6)

	cmd[0] = 0x03;	// REQUEST SENSE
	cmd[4] = bufSize;
	cmd[5] = 0;

	return transport(IO_READ,buf,bufSize);
}

int CSCSI_Dev::start_stop_unit(int unitType,unsigned char isImmed)
{
	INIT_DATA(6)

	cmd[0] = 0x1B;	// START/STOP UNIT
	if (isImmed)
	{
	  cmd[1] = 1;
	}
	cmd[4] = unitType;	// "Load"-
	cmd[5] = 0;

	return transport_nonedata();
}

int  CSCSI_Dev::wait_for_unit(volatile int *progress)
{
#ifdef _WIN32
	unsigned char *sense = p.sense, sensebuf[18];
#else
	unsigned char *sense = _sense, sensebuf[18];
#endif

	int err;
	long msecs = 1000;

	while (1)
	{
		if (msecs > 0)
		{
			poll(NULL, 0, msecs);
		}
		msecs = getmsecs();
		if (!(err = test_unit_ready()))
		{
			break;
		}

		// I wish I could test just for sense.valid, but (at least)
		// hp dvd100i returns 0 in valid bit at this point:-( So I
		// check for all bits...
		if ((sense[0] & 0x70) == 0)
		{
			printf("unable to TEST UNIT READY");
			return -1;
		}
		else if (sense[12] == 0x3A) // doesn't get any further than "no media"
		{
			return err;
		}

		while (progress)
		{
			if (sense[15] & 0x80)
			{
				*progress = sense[16] << 8 | sense[17];
				break;
			}
			// MMC-3 (draft) specification says that the unit should
			// return progress indicator in key specific bytes even
			// in reply to TEST UNIT READY. I.e. as above! But (at
			// least) hp dvd100i doesn't do that and I have to fetch
			// it separately:-(
			if ((err = request_sense(sensebuf, sizeof(sensebuf))))
			{
				sperr("REQUEST SENSE", m_last_err);
				return err;
			}
			if (sensebuf[15] & 0x80)
			{
				*progress = sensebuf[16] << 8 | sensebuf[17];
			}
			break;
		}
		msecs = 1000 - (getmsecs() - msecs);
	}

	return 0;
}


int  CSCSI_Dev::pioneer_stop( volatile int *progress)
{
	int err;
#ifdef _WIN32
	unsigned char *sense= p.sense;
#else
	unsigned char *sense= _sense;
#endif
	// Pioneer units tend to just fall through wait_for_unit() and
	// report "OP IN PROGRESS" on next non-TEST UNIT READY command.
	// This behaviour is explicitly discouraged in MMC, but what one
	// can do? Well, whenever appropriate one can deploy STOP UNIT
	// as "barrier" command with all units...
	while (1)
	{
		if ((err = start_stop_unit(UNIT_STOP, 0x01)) == 0x20407)
		{
			if (progress && sense[15] & 0x80)
			{
				*progress = sense[16] << 8 | sense[17];
			}
			poll(NULL, 0, 333);
			continue;
		}
		break;
	}

	return err;
}

int CSCSI_Dev::pioneer_start()
{
	int err;

	// Pioneer units tend to just fall through wait_for_unit() and
	// report "OP IN PROGRESS" on next non-TEST UNIT READY command.
	// This behaviour is explicitly discouraged in MMC, but what one
	// can do? Well, whenever appropriate one can deploy STOP UNIT
	// as "barrier" command with all units...
	while (1)
	{
		if ((err = start_stop_unit(UNIT_START, 0x01)) == 0x20407)
		{
			poll(NULL, 0, 333);
			continue;
		}
		break;
	}

	return err;
}

/*
Read capacity
bit→
↓byte	7	6	5	4	3	2	1	0
0	Operation code = 25h
1	LUN	Reserved	RelAdr
2	LBA (MSB)
LBA (LSB)
3
4
5
6	Reserved
7	Reserved
8	Reserved	PMI
9	Control
The special control fields in the CDB have the following meaning:
RelAdr - indicates that the logical block address (LBA) value is relative (only used with linked commands).
PMI - partial medium indicator:
0 - return value for the last LBA
1 - return value for the last LBA after which a substantial delay in data transfer will be encountered (e.g., the current track or cylinder)
The target will return capacity data structured as follows:
bit→
↓byte	7	6	5	4	3	2	1	0
0–3	Returned LBA
4–7	Block length in bytes

*/
int  CSCSI_Dev::read_capacity_10(unsigned char* buf, uint32_t buf_size)
{
	INIT_DATA(10)
	cmd[0] = 0x25; /* read capacity*/

	return transport(IO_READ,buf,buf_size);
}

int  CSCSI_Dev::read_capacity_16(unsigned char* buf, uint32_t buf_size)
{
	INIT_DATA(16)
	cmd[0] = 0x9E; /* read capacity 16*/
	cmd[1] = 0x10;
	cmd[12] = buf_size>>8;
	cmd[13] = buf_size;

	return transport(IO_READ,buf,buf_size);
}
/*
Write (10)

The Write (10) command was introduced to cope with 32-bit LBAs. It also included four other enhancements.
Disable Page Out (DPO) allows the initiator to warn the target that the data being written is unlikely to be read back soon and so is not worth keeping in the target's data cache.
Force Unit Access (FUA) tells the target to immediately send the data to the media surface and to not buffer it through a cache.
The EBP bit tells the target that it may skip the erase process when writing data.
The RelAdr bit is used to indicate that the LBA value is relative (only used with linked commands). The CDB structure is[1]:
bit→
↓byte	7	6	5	4	3	2	1	0
0	         Operation code = 2Ah
1		WRPROTECT	DPO	FUA	Reserved	FUA_NV	Obsolete
2–5			LBA
6		Reserved	Group Number
7–8	Transfer length
9		Control
*/

int  CSCSI_Dev::write_10(uint32_t lba,uint32_t sec_count,unsigned char* data_buf,uint32_t data_size)
{
	INIT_DATA(10)

	cmd[0] = 0x2A; /* WRITE (10) */

	cmd[2] = lba>>24; /* Logical Block Addrss */
	cmd[3] = lba>>16;
	cmd[4] = lba>>8;
	cmd[5] = lba;

	cmd[7] = sec_count>>8;
	cmd[8] = sec_count;

	return  transport(IO_WRITE,data_buf,data_size);
}


/*
Read (10)

The Read(10) (also called 'read extended' in 1987) command was introduced to cope with 32-bit LBAs (2048 GiB) and uses a 16-bit Transfer Length field.
It also included three performance-related enhancements. Disable Page Out (DPO) allows the initiator to warn the target that the data being read is unlikely to be requested again soon
and so is not worth keeping in the target's data cache. Force Unit Access (FUA) tells the target to fetch the data from the media surface and to not use a cached copy.
The RelAdr bit is used to indicate that the LBA value is relative (only used with linked commands). The '2' in the operation code indicates a "class 2" command, and the '8' is the real 'read' opcode.
This command was present in a 1982 working document and as such it is likely to have been present from the first SCSI command definitions, showing remarkable foresight. The CDB structure is:
bit→
↓byte	7	6	5	4	3	2	1	0
0	Operation code = 28h
1	LUN	DPO	FUA	Reserved	RelAdr
2–5	LBA
6	Reserved
7–8	Transfer length
9	Control
*/
int  CSCSI_Dev::read_10(uint32_t lba,uint32_t sec_count,unsigned char* data_buf,uint32_t data_size)
{
	INIT_DATA(10)

	cmd[0] = 0x28;	// READ(10)

	cmd[2] = lba>>24;
	cmd[3] = lba>>16;
	cmd[4] = lba>>8;
	cmd[5] = lba;		// LBA#16, volume descriptor set

	cmd[7] = sec_count>>8;
	cmd[8] = sec_count;

	return transport(IO_READ,data_buf,data_size);
}

/*
Write (16)

The CDB structure is[1]:
bit→
↓byte	7	6	5	4	3	2	1	0
0	Operation code = 8Ah
1	WRProtect	DPO	FUA	Reserved	FUA_NV	Obsolete
2–9	LBA
10-13	Transfer Length
14	Restricted for MMC-6	Reserved	Group Number
15	Control
*/
int  CSCSI_Dev::write_16(uint64_t lba,uint32_t sec_count,unsigned char* data_buf,uint32_t data_size)
{
	INIT_DATA(16)

	cmd[0] = 0x8A; /* WRITE (16) */
#if 0	// no set ,for fast to IO. 最大支持256TB硬盘，当硬盘超这个大小需要	cmd[3] = lba>>48;cmd[4] = lba>>40;打开

	cmd[2] = lba>>56; /* Logical Block Addrss */
	cmd[3] = lba>>48;
	cmd[4] = lba>>40;
#endif
	cmd[5] = lba>>32;
	cmd[6] = lba>>24;
	cmd[7] = lba>>16;
	cmd[8] = lba>>8;
	cmd[9] = lba;
#if 0	// no set ,for fast to IO.
	cmd[10] = sec_count>>24;
	cmd[11] = sec_count>>16;
#endif
	cmd[12] = sec_count>>8;
	cmd[13] = sec_count;

	return transport(IO_WRITE,data_buf,data_size);
}

/*
The Read(16) command is similar to the Read(12) command except that it has a 64-bit LBA field and is thus capable of addressing 8 388 608 PiB (ATA-6 is capable of max 128 PiB).
The Transfer Length field is 32 bits long. The CDB structure is:
bit→
↓byte	7	6	5	4	3	2	1	0
0	Operation code = 88h
1	RDPROTECT	DPO	FUA	Reserved	FUA_NV	Reserved
2–9	LBA
10-13	Transfer length
14	Restricted for MMC-4	Reserved	Group number
15	Control
*/
int  CSCSI_Dev::read_16(uint64_t lba,uint32_t sec_count,unsigned char* data_buf,uint32_t data_size)
{
	INIT_DATA(16)

	cmd[0] = 0x88; /* READ (16) */
#if 0 //no set ,for fast to IO.
	cmd[2] = lba>>56; /* Logical Block Addrss */
	cmd[3] = lba>>48;
	cmd[4] = lba>>40;
#endif
	cmd[5] = lba>>32;
	cmd[6] = lba>>24;
	cmd[7] = lba>>16;
	cmd[8] = lba>>8;
	cmd[9] = lba;
#if 0
	cmd[10] = sec_count>>24;
	cmd[11] = sec_count>>16;
#endif
	cmd[12] = sec_count>>8;
	cmd[13] = sec_count;

	return transport(IO_READ,data_buf,data_size);;
}

int CSCSI_Dev::Write_try(uint64_t lba,uint32_t sec_count,unsigned char* data_buf,uint32_t data_size)
{
	int try_idx = 0;
	int ret = -1;
	if(is_2TB_big)
	{
		for(try_idx = 0 ;try_idx < m_retry_times ;try_idx++)
		{
			if((ret = write_16(lba,sec_count,data_buf,data_size) )== 0)
				break;
			sperr("Write 16",m_last_err);
			if(ret == SENSE_MEDIUM_ERR || ret == SENSE_HARDWARE_ERR)
				return ret;
		}
	}
	else
	{
		for(try_idx = 0 ;try_idx < m_retry_times ;try_idx++)
		{
			if((ret = write_10(lba,sec_count,data_buf,data_size)) == 0)
				break;
			sperr("Write 10",m_last_err);
			if(ret == SENSE_MEDIUM_ERR || ret == SENSE_HARDWARE_ERR)
				return ret;
		}
	}

	return ret;
}

int CSCSI_Dev::Read_try(uint64_t lba,uint32_t sec_count,unsigned char* data_buf,uint32_t data_size)
{
	int try_idx = 0;
	int ret = -1;
	if(is_2TB_big)
	{
		for(try_idx = 0 ;try_idx < m_retry_times ;try_idx++)
		{if((ret = read_16(lba,sec_count,data_buf,data_size)) == 0)
				break;
			sperr("Read 16",m_last_err);
			if(ret == SENSE_MEDIUM_ERR || ret == SENSE_HARDWARE_ERR)
				return ret;

		}
	}
	else
	{
		for(try_idx = 0 ;try_idx < m_retry_times ;try_idx++)
		{
			if((ret = read_10(lba,sec_count,data_buf,data_size)) == 0)
				break;
			sperr("Read 10",m_last_err);
			if(ret == SENSE_MEDIUM_ERR || ret == SENSE_HARDWARE_ERR)
				return ret;
		}
	}

	return ret;
}

int CSCSI_Dev::Write(uint64_t lba,uint32_t sec_count,unsigned char* data_buf,uint32_t data_size)
{
	if(is_2TB_big)
		return write_16(lba,sec_count,data_buf,data_size);
	else
		return write_10(lba,sec_count,data_buf,data_size);
}

int CSCSI_Dev::Read(uint64_t lba,uint32_t sec_count,unsigned char* data_buf,uint32_t data_size)
{
	if(is_2TB_big)
		return read_16(lba,sec_count,data_buf,data_size);
	else
		return read_10(lba,sec_count,data_buf,data_size);
}

int CSCSI_Dev::Write(int64_t lba,unsigned char* data_buf,uint32_t data_size)	//写入固定字节
{
	uint32_t leaf_size = 0;
	uint32_t trans_cnt = 128;
	uint32_t trans_size = trans_cnt * m_sec_size;
	int ret;
	unsigned char* align_buf = NULL;

	while(data_size > 0)
	{
		if(data_size < trans_size )
		{
			if(leaf_size)
			{
				align_buf = (unsigned char*)malloc(m_sec_size);
				break;
			}
			if(data_size >= m_sec_size)
			{
				trans_cnt = data_size/m_sec_size;
				trans_size = trans_cnt * m_sec_size;
				leaf_size = data_size - trans_cnt * m_sec_size;
			}
			else
			{
				leaf_size = data_size;
				align_buf = (unsigned char*)malloc(m_sec_size);
				break;
			}
		}
		ret = Write_try(lba,trans_cnt,data_buf,trans_size);
		if(ret)
		{
			goto ERR_OUT;
		}
		lba += trans_cnt;
		data_size -= trans_size;
		data_buf += trans_size;
	}

	if(leaf_size)
	{
		ret = Read_try(lba,1,align_buf,m_sec_size);
		if(ret)
		{
			return ERR_WRITE_FAIL;
		}
		memcpy(align_buf,data_buf,leaf_size);
		ret = Write_try(lba,1,align_buf,m_sec_size);
		if(ret)
			goto ERR_OUT;
	}
	if(align_buf)
		free(align_buf);
	return 0;
ERR_OUT:
	if(align_buf)
		free(align_buf);
	return ERR_WRITE_FAIL;
}

int CSCSI_Dev::Read(int64_t lba,unsigned char* data_buf,uint32_t data_size)	//读取固定字节
{
	uint32_t leaf_size = 0;
	uint32_t trans_cnt = 128;
	uint32_t trans_size = trans_cnt * m_sec_size;
	int ret;
	unsigned char* align_buf = NULL;

	while(data_size )
	{
		if(data_size < trans_size )
		{
			if(leaf_size)
			{
				align_buf = (unsigned char*)malloc(m_sec_size);
				break;
			}
			if(data_size >= m_sec_size)
			{
				trans_cnt = data_size/m_sec_size;
				trans_size = trans_cnt * m_sec_size;
				leaf_size = data_size - trans_cnt * m_sec_size;
			}
			else
			{
				leaf_size = data_size;
				align_buf = (unsigned char*)malloc(m_sec_size);
				break;
			}
		}
		ret = Read_try(lba,trans_cnt,data_buf,trans_size);
		if(ret)
		{
			goto ERR_OUT;
		}
		lba += trans_cnt;
		data_size -= trans_size;
		data_buf += trans_size;
	}

	if(leaf_size)
	{
		ret = Read_try(lba,1,align_buf,m_sec_size);
		if(ret)
		{
			return ERR_READ_FAIL;
		}
		memcpy(data_buf,align_buf,leaf_size);
	}
	if(align_buf)
		free(align_buf);
	return 0;
ERR_OUT:
	if(align_buf)
		free(align_buf);
	return ERR_READ_FAIL;
}

uint64_t CSCSI_Dev::ReadCapacity(uint32_t * psec_size)
{
	int ret;
	unsigned char buf[32];
	memset(buf,0,32);

	ret = read_capacity_10(buf,8);
	if(ret < 0)
	{
		sperr("Read Capacity 10",m_last_err);
		return 0;
	}

	m_capacity = buf[0]<<24| buf[1]<<16| buf[2]<<8| buf[3];
	m_sec_size = buf[4]<<24| buf[5]<<16| buf[6]<<8| buf[7];
	if(psec_size)
		*psec_size = m_sec_size;

	if(((uint32_t)m_capacity) != SIZE_SECTOR_2TB)
	{
		is_2TB_big = false;
		return m_capacity;
	}
	//大于3TB硬盘,后续增加此命令
	ret = read_capacity_16(buf,32);
	if(ret < 0)
	{
		sperr("Read Capacity 16",m_last_err);
		return 0;
	}

	m_capacity = (uint64_t)buf[0]<<56| (uint64_t)buf[1]<<48| (uint64_t)buf[2]<<40| (uint64_t)buf[3]<<32|
				 buf[4]<< 24| buf[5]<<16| buf[6]<<8| buf[7];
	m_sec_size = buf[8]<<24| buf[9]<<16| buf[10]<<8| buf[11];


	if(psec_size)
		*psec_size = m_sec_size;
	is_2TB_big = true;
	return m_capacity;
}

bool CSCSI_Dev::FlushCache()
{
	LOCK_DEV(&m_lock);
	int ret = synchronize_cache(0x01);
	UNLOCK_DEV(&m_lock);

	if(0 == ret)
	{
		return true;
	}
	else
	{
		TRACE_ERR("Flusu error.err %d \n",m_last_err);
		return false;
	}
}