#include "CScsi.h"

#ifdef	LJ_DEBUG
#include "user_config.h"
#endif

#if defined(__ENVIRONMENT_LINUX__)
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#endif

Scsi_Command::Scsi_Command()	
{ 
	fd=-1;
	autoclose=1; 
	filename=NULL; 
}

Scsi_Command::Scsi_Command(int f)	
{ 
	fd=f;
	autoclose=0; 
	filename=NULL; 
}

Scsi_Command::Scsi_Command(void*f)
{ 
	fd=(long)f;
	autoclose=0; 
	filename=NULL; 
}

Scsi_Command::~Scsi_Command()	
{ 
	if (fd>=0 && autoclose) 
	{
		close(fd);
		fd=-1;
	}
	if (filename) 
	{
		free(filename);
		filename=NULL;
	}
}

int Scsi_Command::Associate (const char *file,const struct stat *ref)
{ 
	struct stat sb;
	if ((fd=open (file,O_RDWR|O_NONBLOCK)) < 0 &&
	    (fd=open (file,O_RDONLY|O_NONBLOCK)) < 0)	return 0;
	if (fstat(fd,&sb) < 0)				return 0;
	if (!S_ISBLK(sb.st_mode))	{ errno=ENOTBLK;return 0; }

	if (ref && (!S_ISBLK(ref->st_mode) || ref->st_rdev!=sb.st_rdev))
	{   errno=ENXIO; return 0;   }

	filename=strdup(file);

	return 1;
}

unsigned char& Scsi_Command::operator[] (size_t i)
{	
	if (i==0)
	{   
		memset(&cgc,0,sizeof(cgc)), memset(&_sense,0,sizeof(_sense));
	    cgc.quiet = 1;
	    cgc.sense = &_sense.s;
#ifdef SG_IO
	    if (use_sg_io)
	    {
			memset(&sg_io,0,sizeof(sg_io));
			sg_io.interface_id= 'S';
			sg_io.mx_sb_len	= sizeof(_sense);
			sg_io.cmdp	= cgc.cmd;
			sg_io.sbp	= _sense.u;
			sg_io.flags	= SG_FLAG_LUN_INHIBIT|SG_FLAG_DIRECT_IO;
	    }
#endif
	}
	sg_io.cmd_len = i+1;
	return cgc.cmd[i];
}


unsigned char& Scsi_Command::operator()(size_t i)	
{ 
	return _sense.u[i]; 
}

unsigned char* Scsi_Command::Sense()		
{ 
	return _sense.u;    
}

void Scsi_Command::Timeout(int i)			
{ 
	cgc.timeout=sg_io.timeout=i*1000; 
}

int Scsi_Command::Transport(Direction dir,void *buf,size_t sz)
{ 
	int ret = 0;

	#ifdef SG_IO
	#define KERNEL_BROKEN 0
		if (use_sg_io)
		{   sg_io.dxferp		= buf;
			sg_io.dxfer_len		= sz;
			sg_io.dxfer_direction	= use_sg_io[dir];
			if (ioctl (fd,SG_IO,&sg_io)) return -1;

	#if !KERNEL_BROKEN
			if ((sg_io.info&SG_INFO_OK_MASK) != SG_INFO_OK)
	#else
			if (sg_io.status)
	#endif
			{	errno=EIO; ret=-1;
	#if !KERNEL_BROKEN
			if (sg_io.masked_status&CHECK_CONDITION)
	#endif
		{   ret = ERRCODE(sg_io.sbp);
		    if (ret==0) ret=-1;
		    else	CREAM_ON_ERRNO(sg_io.sbp);
		}
	    }
	    return ret;
	}
	else
	#undef KERNEL_BROKEN
	#endif
	{   cgc.buffer		= (unsigned char *)buf;
	    cgc.buflen		= sz;
	    cgc.data_direction	= dir;
	    if (ioctl (fd,CDROM_SEND_PACKET,&cgc))
	    {	ret = ERRCODE(_sense.u);
		if (ret==0) ret=-1;
	    }
	}
	return ret;
}

int Scsi_Command::Umount(int f)
{ 
	struct stat    fsb,msb;
	struct mntent *mb;
	FILE          *fp;
	pid_t          pid,rpid;
	int            ret=0,rval;

	if (f==-1) f=fd;
	if (fstat (f,&fsb) < 0)				return -1;
	if ((fp=setmntent ("/proc/mounts","r"))==NULL)	return -1;

	while ((mb=getmntent (fp))!=NULL)
	{   if (stat (mb->mnt_fsname,&msb) < 0) continue; // corrupted line?
	    if (msb.st_rdev == fsb.st_rdev)
	    {	ret = -1;
		if ((pid = fork()) == (pid_t)-1)	break;
		if (pid == 0) execl ("/bin/umount","umount",mb->mnt_dir,NULL);
		while (1)
		{   rpid = waitpid (pid,&rval,0);
		    if (rpid == (pid_t)-1)
		    {	if (errno==EINTR)	continue;
			else			break;
		    }
		    else if (rpid != pid)
		    {	errno = ECHILD;
			break;
		    }
		    if (WIFEXITED(rval))
		    {	if (WEXITSTATUS(rval) == 0) ret=0;
			else			    errno=EBUSY; // most likely
			break;
		    }
		    else
		    {	errno = ENOLINK;	// some phony errno
			break;
		    }
		}
		break;
	    }
	}
	endmntent (fp);

	return ret;
}

void sperror (const char *cmd, int err)
{
	int saved_errno = errno;

    if (err==-1)
	{
//		fprintf (stderr,":-( unable to %s: ",cmd);
	}
    else
	{
//		fprintf (stderr,":-[ %s failed with SK=%Xh/ASC=%02Xh/ACQ=%02Xh]: ", cmd,SK(err),ASC(err),ASCQ(err));
	}
	
    errno	=	saved_errno, perror (NULL);
}



