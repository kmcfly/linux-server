#ifndef SCSI_COMMAND_H
#define SCSI_COMMAND_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/ioctl.h> 
#if defined(__TDFH__) || defined(__TDNXP__)
#include <sys/stat.h>
#endif
#include <mntent.h>
#include <sys/wait.h>
#include <sys/utsname.h>
#define CDROM_PACKET_SIZE       12
#define CGC_DATA_WRITE          1
#define CGC_DATA_READ           2
#define CGC_DATA_NONE           3
#define CDROM_SEND_PACKET       0x5393

typedef unsigned char       __u8;

struct request_sense {
#if defined(__BIG_ENDIAN_BITFIELD)
        __u8 valid              : 1;
        __u8 error_code         : 7;
#elif defined(__LITTLE_ENDIAN_BITFIELD)
        __u8 error_code         : 7;
        __u8 valid              : 1;
#endif
        __u8 segment_number;
#if defined(__BIG_ENDIAN_BITFIELD)
        __u8 reserved1          : 2;
        __u8 ili                : 1;
        __u8 reserved2          : 1;
        __u8 sense_key          : 4;
#elif defined(__LITTLE_ENDIAN_BITFIELD)
        __u8 sense_key          : 4;
        __u8 reserved2          : 1;
        __u8 ili                : 1;
        __u8 reserved1          : 2;
#endif
        __u8 information[4];
        __u8 add_sense_len;
        __u8 command_info[4];
        __u8 asc;
        __u8 ascq;
        __u8 fruc;
        __u8 sks[3];
        __u8 asb[46];
};

struct cdrom_generic_command
{
        unsigned char           cmd[CDROM_PACKET_SIZE];
        unsigned char           *buffer;
        unsigned int            buflen;
        int                     stat;
        struct request_sense    *sense;
        unsigned char           data_direction;
        int                     quiet;
        int                     timeout;
        void                    *reserved[1];    /* unused, actually */
};

#define CREAM_ON_ERRNO_NAKED(s)				\
    switch ((s)[12])					\
    {	case 0x04:	errno=EAGAIN;	break;		\
	case 0x20:	errno=ENODEV;	break;		\
	case 0x21:	if ((s)[13]==0)	errno=ENOSPC;	\
			else		errno=EINVAL;	\
			break;				\
	case 0x30:	errno=EMEDIUMTYPE;	break;	\
	case 0x3A:	errno=ENOMEDIUM;	break;	\
    }

#define CREAM_ON_ERRNO(s)	do { CREAM_ON_ERRNO_NAKED(s) } while(0)
#define	FATAL_START(er)	(0x80|(er))
#define	SK(errcode)	(((errcode)>>16)&0xF)
#define	ASC(errcode)	(((errcode)>>8)&0xFF)
#define ASCQ(errcode)	((errcode)&0xFF)
#define ERRCODE(s)	((((s)[2]&0x0F)<<16)|((s)[12]<<8)|((s)[13]))

void sperror (const char *cmd, int err);


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

typedef enum __cgc_direction__
{	
	NONE=CGC_DATA_NONE,	// 3
	READ=CGC_DATA_READ,	// 2
	WRITE=CGC_DATA_WRITE	// 1
} Direction;


#ifdef SG_IO
static const int Dir_xlate [4] = {
		0,			// implementation-dependent...
		SG_DXFER_TO_DEV,	// 1,CGC_DATA_WRITE
		SG_DXFER_FROM_DEV,	// 2,CGC_DATA_READ
		SG_DXFER_NONE	};	// 3,CGC_DATA_NONE

static const class USE_SG_IO 
{
private:
    int	yes_or_no;
public:
    USE_SG_IO()	
	{ 
		struct utsname buf;
		 uname (&buf);
		 yes_or_no=(strcmp(buf.release,"2.5.43")>=0);
	}

    ~USE_SG_IO(){}
    operator int()	const	{ return yes_or_no; }
    int operator[] (Direction dir)	const	{ return Dir_xlate[dir]; }
} use_sg_io;
#endif

class Scsi_Command {
	private:
		int fd,autoclose;
		char *filename;
		struct cdrom_generic_command cgc;
		union 
		{
			struct request_sense	s;
			unsigned char		u[18];
		} _sense;

	#ifdef SG_IO
		struct sg_io_hdr		sg_io;
	#else
		struct { int cmd_len,timeout; }	sg_io;
	#endif
		
	public:
		Scsi_Command();
		Scsi_Command(int f);
		Scsi_Command(void*f);
		~Scsi_Command();
		int Associate (const char *file,const struct stat *ref=NULL);
		unsigned char &operator[] (size_t i);
		unsigned char &operator() (size_t i);
		unsigned char *Sense();
		void Timeout(int i);

	#ifdef SG_IO
		size_t Residue()			{ return use_sg_io?sg_io.resid:0; }
	#else
		size_t Residue()			{ return 0; }
	#endif
		
		int Transport(Direction dir=NONE,void *buf=NULL,size_t sz=0);
		int Umount(int f=-1);
		int Is_reload_needed () { return 1;}
		int Open()
		{
			if(fd == -1)
			{
				if ((fd=open (filename,O_RDWR|O_NONBLOCK)) < 0 &&
					(fd=open (filename,O_RDONLY|O_NONBLOCK)) < 0)
				{
					return -1;
				}
				
				return fd;
			}
			else
			{
				return fd;
			}
		}
		void Close()
		{
			close(fd);
			fd = -1;
		}
};

#endif

