#include "CDOperationMan.h"


CDOperationMan* CDOperationMan::m_pCdOperIns = NULL;

//
CDOperationMan::CDOperationMan()
{
	
	m_progressFile = -1;
	m_pPrecent = NULL;
}

//
CDOperationMan::~CDOperationMan()
{

}



CDOperationMan* CDOperationMan::Instance()
{
	if (NULL == m_pCdOperIns) 
	{
		m_pCdOperIns = new CDOperationMan;
		assert(NULL != m_pCdOperIns);
	}
	return m_pCdOperIns;
}




//cmdPath是刻录用到的执行程序如，mkisofs growisofs等，存放的目录。
bool CDOperationMan::Initial(const char *cmdPath)
{
	assert(NULL != cmdPath);

     memset(m_cmdPath, 0, sizeof(m_cmdPath));
	 strcpy(m_cmdPath, cmdPath);
	 int len = strlen(cmdPath);	 
	 if ('/' == m_cmdPath[len - 1])
	 {
		 m_cmdPath[len - 1] = '\0';
	 }

	char szCmd[128] = {0};
	sprintf(szCmd, "rm -rf %s", CDROM_MOUNT_DIR);
	DVRSystem(szCmd);

	sprintf(szCmd, "mkdir %s -p", PROGRESSPATH);
	DVRSystem(szCmd);

#if defined(__X_LINUX__)
	m_progressFile = open(PROGRESSFILE, O_RDWR);
#else
	m_progressFile = open(PROGRESSFILE, O_RDWR|O_CREAT);
#endif
	
	if(m_progressFile >= 0)
	{
		printf("%s,%d\n", __FILE__,__LINE__);
		ftruncate(m_progressFile, sizeof(*m_pPrecent));

#if defined(__CHIP3521__) || defined(__CHIP3520A__) || defined(__CHIP3520D__)
		//3521的产品不支持对文件进行MAP_SHARED操作，这导致进行DVD格式化时不能正常显示格式化进度。应当去掉刻录功能
		m_pPrecent = (long *)mmap(NULL,sizeof(*m_pPrecent),PROT_READ|PROT_WRITE,MAP_SHARED,m_progressFile,0);
#else
		m_pPrecent = (long *)mmap(NULL,sizeof(*m_pPrecent),PROT_READ|PROT_WRITE,MAP_SHARED,m_progressFile,0);
#endif
		if (MAP_FAILED == m_pPrecent)
		{
			printf("(%s %d): Map file failed!",__FILE__,__LINE__);
			return false;
		}
		else
		{
			*m_pPrecent=0;
			
		}
	}

	
	
	return true;
}

//
void CDOperationMan::Quit()
{
	if(m_pPrecent != NULL)
	{
		munmap((void*)m_pPrecent, sizeof(*m_pPrecent));
		m_pPrecent=NULL;
	}
	
	unlink(PROGRESSFILE);
	if (m_progressFile >= 0) 
	{
		close (m_progressFile);
		m_progressFile = -1;
	}


}

//
int CDOperationMan::GetDeviceDiskInfo(DVR_DISK_INFO_LIST &cdInfoList)
{
	FILE *file = NULL;
	char line[128]={0};
	char getstr[20]={0};
	int  ret = 0;

	file = fopen(CDROM_INFO_PATH_NAME, "rb");

	if(NULL == file)
	{
		printf("open cdrom/info file err!\n");
		return ret;
	}

	while(fgets(line, 128, file))
	{
		if(strstr(line, "drive name:"))
		{
			if(sscanf(line,"drive name:%s", getstr))
			{
				memset(m_cdPath, 0, sizeof(m_cdPath));
				sprintf(m_cdPath, "/dev/%s", getstr);
				printf("(%s,%d)length of name:%d\n",__FILE__,__LINE__,strlen(getstr));
				if (strlen(getstr) > 0)
				{
					ret = 1;
				}
			}

			break;
		}
		memset(line, 0, sizeof(line));
	}

	fclose(file);

	if (0 == ret)
	{
		return ret;
	}

	CD_TYPE       cdType;
	DVR_DISK_INFO cdInfo;
	memset(&cdInfo, 0 ,sizeof(cdInfo));
	cdInfo.diskType      = NET_DISK_TYPE_CDDVD;
	cdInfo.diskInterface = NET_DISK_CDDVD_INTERFACE;
	strcpy(cdInfo.diskname, m_cdPath);
	strcpy(cdInfo.mountdir, CDROM_MOUNT_DIR);
	
	cdType = GetCDType(m_cdPath);
	if ((CD_TYPE_NODISK == cdType) || (CD_TYPE_UNKNOWN == cdType))
	{
		cdInfo.totalSpace = 0;		
	}
	else if ((CD_TYPE_CDR == cdType) || (CD_TYPE_CDRW == cdType))
	{
		cdInfo.totalSpace = CD_DISK_SIZE;
	}
	else
	{
		cdInfo.totalSpace = DVD_DISK_SIZE;//4.7*1024
	}
	cdInfo.freeSpace = cdInfo.totalSpace;

	if ((CD_TYPE_UNKNOWN == cdType) || (CD_TYPE_NODISK == cdType))
	{
		cdInfo.diskStatus = NET_DISK_NO_CD;
	}
	else 
	{
		cdInfo.diskStatus = NET_DISK_NORMAL;
	}

	cdInfoList.AddTail(cdInfo);	 

	return ret;
}

//
int CDOperationMan::Format( )				//对可写碟片格式化
{
	int ret = 0;
	char szCmd[128] = {0};

	unsigned int scsiNum;
	unsigned int ch;
	unsigned int id;
	unsigned int lun;

	if (NULL != m_pPrecent)
	{
		*m_pPrecent = 0;
	}
	
	//获得碟片类型
	CD_TYPE cdType = GetCDType(m_cdPath);
	if ((CD_TYPE_UNKNOWN == cdType)
	   ||(CD_TYPE_NODISK == cdType))
	{
		return -1;
	}

	if (0 != GetDevicePara(&scsiNum, &ch, &id, &lun))
	{
		return -1;
	}
	
	switch(cdType)
	{	                      
	case CD_TYPE_CDR:           //只读光盘, 只写一次, 不能格式化                      
	case CD_TYPE_DVDR:
	case CD_TYPE_DVDR_PLUSE:
	case CD_TYPE_DVDRAM:
		ret = 0;
		break;
	case CD_TYPE_CDRW:
		sprintf(szCmd, "%s/%s blank=fast dev=%s:%d,%d,%d", m_cmdPath, CDFORMAT, m_cdPath, ch, id, lun);		
		ret = DVRSystem(szCmd);		
		break;
	case CD_TYPE_DVDRW:
	case CD_TYPE_DVDRW_PLUSE:
		//DVRSystem("mkdir /nfsdir");	//创建一个目录, 因为在 ./dvd+rw-format 中调用 mkstemp("/nfsdir/dvd+rw.XXXXXX") 生成一个临时目录
		sprintf(szCmd, "%s/%s -force %s", m_cmdPath, DVDFORMAT, m_cdPath);
		ret = DVRSystem(szCmd);
		break;
	default:
		ret = -1;
		break;
	}

	return ret;	
}

//
int CDOperationMan::Write(char *pFileName, WRITE_TYPE mark)
{
	int ret = 0;
	char szCmd[128] = {0};
	unsigned int scsiNum;
	unsigned int ch;
	unsigned int id;
	unsigned int lun;

	//获得碟片类型
	CD_TYPE cdType = GetCDType(m_cdPath);
	if ((CD_TYPE_UNKNOWN == cdType)
		||(CD_TYPE_NODISK == cdType))
	{
		return -1;
	}

	if (0 != GetDevicePara(&scsiNum, &ch, &id, &lun))
	{
		return -1;
	}

	printf("%s,%d:%s,%s\n", __FILE__,__LINE__,m_cdPath,m_cmdPath);

	switch(cdType)
	{
	case CD_TYPE_CDR:
	case CD_TYPE_CDRW:		
		sprintf(szCmd, "%s/%s -J -R -f %s	| %s/%s	speed=16 fs=6m ts=2m	dev=%s:%d,%d,%d -", 
			            m_cmdPath, WRITECD, pFileName, 
			            m_cmdPath, CDFORMAT, m_cdPath, ch, id, lun);
		
		
		break;		
	case CD_TYPE_DVDR:
	case CD_TYPE_DVDR_PLUSE:
	case CD_TYPE_DVDRW:
	case CD_TYPE_DVDRW_PLUSE:
		sprintf(szCmd, "%s/%s -Z  %s -J -R -f	 %s", m_cmdPath, WRITEDVD, m_cdPath, pFileName);		
		break;		
	default:
		printf("(%s  %d):Unknow type of disk, Write disk failed!\n",__FILE__, __LINE__);
		return -1;
	}
	
	printf("szcmd:%s\n", szCmd);

	ret = DVRSystem(szCmd);	
	return ret;
}

//
long CDOperationMan::GetPercent()			//获得备份的进度
{
	if(m_pPrecent != NULL)
	{
		return *m_pPrecent;
	}
	return 0;
}

void CDOperationMan::SetPercentZero()
{
	if (m_pPrecent != NULL)
	{
		*m_pPrecent = 0;
	}
}



CD_TYPE CDOperationMan::GetCDType(char *cdPathName)
{
	unsigned int  mmc_profile = 0;
	unsigned char formats[128]= {0};
	unsigned int  err = 0;
	CD_TYPE       ret = CD_TYPE_UNKNOWN;

	Scsi_Command scsiCmd; 

	if (!scsiCmd.Associate(cdPathName)) 
	{
		printf("(%s %d): associate device %s failed\n", __FILE__, __LINE__, cdPathName);
		return ret;
	}

	scsiCmd[0] = 0x46;
	scsiCmd[8] = 8;
	scsiCmd[9] = 0;
	if ((err=scsiCmd.Transport(READ,formats,8)))
	{	
		return ret;
	}
	scsiCmd.~Scsi_Command();

	mmc_profile = formats[6]<<8|formats[7];
	switch(mmc_profile) 
	{
	case 0x00:
		printf("Attention: No disk in device\n");
		ret = CD_TYPE_NODISK;
		break;
	case 0x09:
		printf("Attention: the disk is CD-R\n");
		ret = CD_TYPE_CDR;
		break;
	case 0x0a:
		printf("Attention: the disk is CD-RW\n");
		ret = CD_TYPE_CDRW;
		break;
	case 0x11:
		printf("Attention: the disk is DVD-R\n");
		ret = CD_TYPE_DVDR;
		break;
	case 0x12:
		printf("Attention: the disk is DVD-RAM\n");
		ret = CD_TYPE_DVDRAM;
		break;
	case 0x13:
	case 0x14:
		printf("Attention: the disk is DVD-RW\n");
		ret = CD_TYPE_DVDRW;
		break;
	case 0x1A:
		printf("Attention: the disk is DVD+RW\n");
		ret = CD_TYPE_DVDRW_PLUSE;
		break;
	case 0x1B:
		printf("Attention: the disk is DVD+R\n");
		ret = CD_TYPE_DVDR_PLUSE;
		break;
	default:
		printf("Attention: Unknow type of cd or dvd, type = %x\n",mmc_profile);
		ret = CD_TYPE_UNKNOWN;
		break;
	}

	return ret;

}






int CDOperationMan::GetDevicePara(unsigned int *pScsiNum, unsigned int *pCH, unsigned int *pID, unsigned int *pLun)
{
	char line[100] = {0};
	char buffer[20] = {0};
	

	FILE * fp = fopen(PROC_SCSI_SCSI,  "r");
	if (NULL == fp) 
	{
		return -1;
	}

	int ret = -1;
	while (fgets(line,100,fp)) 
	{
		if (strstr (line, "Channel") != NULL)
		{
			int ret = sscanf(line,"%s %[^0-9]%d %s %d %s %d %s %d\n",buffer, buffer, pScsiNum, buffer, pCH, buffer, pID, buffer, pLun);
			if (9!=ret) 
			{
				break;
			}
		}

		if (strstr (line, "CD-ROM") != NULL)
		{
			ret = 0;
			break;
		}
	}


	fclose(fp);
	return ret;
}















