#include "PUB_common.h"
#include "Fat32Part.h"
#include "dvrdvsdef.h"

#ifdef _WIN32
#include <winioctl.h>
#endif

CFat32Part::CFat32Part(void)
{
	m_root_lba = 2048;
	m_rec_fsec = 128 * 2048;
	m_base_rec_lba = 0;
	m_rec_fsize = SIZE_MB(MAX_RECORD_FILE_LEN_M);
#ifdef _WIN32
	strcpy(m_drive_path,"\\\\.\\Z:"); 
	m_drive_fd = (HANDLE)-1;
#endif
	
}

CFat32Part::~CFat32Part(void)
{

}

int CFat32Part::Init(CSCSI_Dev * pdev,fat_boot_sector* pfat_boot,uint64_t base_lba)
{
	int ret = 0;
	int i,file_idx;
	uint64_t lba_cnt = 0,lba_finfo = 0;
	unsigned char data_buf[4096] = {0} ;
	msdos_dir_entry root_info;
	int buf_offset = 0;

	m_base_rec_lba = 0;
	m_root_lba = 0;
	memcpy(&m_boot_info,pfat_boot,sizeof(fat_boot_sector));

	//判断是否为文件系统
	if(m_boot_info.boot_sign != 0xAA55 )
	{	
		TRACE_ERR("Boot sign error.\n");
		return -1;
	}
	if(pdev == NULL)
	{
		return ERR_NO_DEVICE;
	}

	m_pdev = pdev;

	//保存好基本的文件系统LBA
	m_base_lba = base_lba;
	//计算根目录的起始地址
	m_base_data_lba = m_boot_info.reserved + m_boot_info.fats * m_boot_info.fat32_length + m_base_lba;
	m_root_lba = m_base_data_lba + (m_boot_info.root_cluster - 2) * m_boot_info.sec_per_clus ;

	//读取根目录表
	ret = m_pdev->Read(m_root_lba,data_buf,4096);
	if(ret < 0)
	{
		return -1;
	}

	for(i = 0 ; i < (4096/DIR_INFO_SIZE); i++)
	{
		memcpy(&root_info,data_buf + buf_offset,DIR_INFO_SIZE);
					//去掉扩展名，形成字符串
		if(m_base_rec_lba )
		{	
			lba_cnt += m_rec_fsize/m_pdev->get_sec_size();
			lba_finfo = m_base_data_lba +  ((uint32_t)root_info.starthi<<16|(uint32_t)root_info.start - 2) * m_boot_info.sec_per_clus ;;
			if(strstr((char*)root_info.name,"dat") && lba_cnt!= lba_finfo)
			{	
				file_idx = atoi((char*)root_info.name);
				if(file_idx > 1)
					return -1;
			}
		}

		if((root_info.attr & ATTR_ARCH && strstr((char*)root_info.name,"00000")))
		{	
			root_info.name[8] = 0x00;	
			file_idx = atoi((char*)root_info.name);//转换成整型数，看是不是0000000
			if(file_idx == 0x00)			//最开始的录像文件LBA
			{
				m_base_rec_lba = m_base_data_lba + ((uint32_t)root_info.starthi<<16|(uint32_t)root_info.start - 2) * m_boot_info.sec_per_clus ;;
				if (SIZE_MB(MAX_RECORD_FILE_LEN_M) != root_info.size)
				{
					TRACE_ERR("The file size=%dM, thr disk info=%d\n", MAX_RECORD_FILE_LEN_M, root_info.size);
				}
				m_rec_fsize = root_info.size;
				lba_cnt = m_base_rec_lba;
				m_rec_fsec = m_rec_fsize/m_pdev->get_sec_size();
			}
		}

		buf_offset += DIR_INFO_SIZE;
	}

	
	return 0;
}

uint64_t CFat32Part::GetFreeSecotr()
{
	int ret;
	unsigned char data_buf[SIZE_4KB] = {0};
	fat_boot_fsinfo fsinfo;
	//正确的FAT32文件系统后，之后再去读取
	ret = m_pdev->Read(m_base_lba,data_buf,SIZE_4KB);
	if(ret)
	{
		return 0;
	}
	memcpy(&fsinfo,data_buf + 1*512,512);
	return   fsinfo.free_clusters * m_boot_info.sec_per_clus;
}
#ifdef _WIN32

/*
   简单的预分配文件操作
*/
int CFat32Part::AssignFile(uint32_t f_size,uint32_t f_num,char* fname_head, char * fname_ext)
{
	FILE* frec = NULL;
	char file_name[60];
	uint32_t f_idx;
	int ret;
	for(f_idx = 0; f_idx < f_num; f_idx++)
	{
		if(fname_head == NULL)
			sprintf(file_name,"%s%08d.dat",m_drive_path+4,f_idx);
		else		
			sprintf(file_name,"%s%s%05d.%s",m_drive_path+4,fname_head,f_idx,fname_ext);
			
		frec = fopen(file_name,"wb+");
		if(frec == NULL)
		{
			return -1;
		}
		ret = fseek(frec,f_size-1,SEEK_SET);
		if(ret < 0)
		{
			fclose(frec);

			return ret;
		}
		if(fwrite("1",1,1,frec) != 1)
		{
			fclose(frec);
			return f_idx;
		}
		fclose(frec);
	}

	return f_idx+1;
}
#endif
int CFat32Part::ScanFileInfo(VEC_FILE_INFO* pvec_finfo)
{
	msdos_dir_entry finfo;
	unsigned char info_buf[0x10000]= {0};
	uint32_t buf_offset = 0;
	int ret;

	ret = m_pdev->Read(m_root_lba,info_buf,0x10000);
	if(ret < 0)
	{
		return 0;
	}

	do
	{
		memcpy(&finfo,info_buf + buf_offset,DIR_INFO_SIZE);
		if(finfo.attr & ATTR_ARCH)
		{
			pvec_finfo->push_back(finfo);
		}
		buf_offset += DIR_INFO_SIZE;
		if(buf_offset > 0x10000)
		{
			ret = m_pdev->Read(m_root_lba,info_buf,0x10000);
			if(ret < 0)
			{
				return (int)pvec_finfo->size();
			}
			buf_offset = 0;
		}

	}while(finfo.name[0] != 0x00);

	return (int)pvec_finfo->size();
}

int CFat32Part::ScanFileLBA(VEC_FILE_LBA* pvec_flba)
{
	RECORD_LBA file_lba;
	msdos_dir_entry finfo;
	unsigned char info_buf[0x10000]= {0};
	uint32_t buf_offset = 0;
	int ret;

	ret = m_pdev->Read(m_root_lba,info_buf,0x10000);
	if(ret < 0)
	{
		return 0;
	}

	do	//获取个扫描根目录表。
	{
		memcpy(&finfo,info_buf + buf_offset,DIR_INFO_SIZE);
		if(finfo.attr & ATTR_ARCH)
		{
			memcpy(file_lba.name,finfo.name,MSDOS_NAME);

			file_lba.lba = m_base_data_lba + ((uint32_t)finfo.starthi<<16 | (uint32_t)finfo.start  - 2) * m_boot_info.sec_per_clus;
			pvec_flba->push_back(file_lba);
		}
		buf_offset += DIR_INFO_SIZE;
		if(buf_offset > 0x10000)
		{
			ret = m_pdev->Read(m_root_lba,info_buf,0x10000);
			if(ret < 0)
			{
				return (int)pvec_flba->size();
			}
			buf_offset = 0;
		}
	}while(finfo.name[0] != 0x00);

	return (int)pvec_flba->size();
}

int CFat32Part::VecFileInfoToFileLBA(VEC_FILE_INFO* pvec_finfo,VEC_FILE_LBA* pvec_flba)
{
	RECORD_LBA file_lba;
	msdos_dir_entry* pinfo;
	int f_num;
	int f_idx;

	pvec_flba->clear();
	f_num = (int)pvec_finfo->size();

	for (f_idx = 0; f_idx < f_num; f_idx++)
	{
		pinfo = &((*pvec_finfo)[f_idx]);
		memcpy(file_lba.name,(*pvec_finfo)[f_idx].name,MSDOS_NAME);
		file_lba.name[MSDOS_NAME] = 0x00;
		file_lba.lba = m_base_data_lba + ((uint32_t)pinfo->starthi<<16 | (uint32_t)pinfo->start  - 2) * m_boot_info.sec_per_clus;
		pvec_flba->push_back(file_lba);
	}
	return (int)pvec_flba->size();
}

bool CFat32Part::FileNameToLBA(const char* f_name,uint64_t* plba)
{
	int file_idx;
	int i;
	char buf[9] ={0};
	if(f_name == NULL || plba == NULL)
		return false;

	for(i = 0 ; i < 8; i++)
	{
		if( 0x30 <= f_name[i] && f_name[i] <= 0x39)
		{	
			memcpy(buf,f_name+i,8);
			break;
		}
	}
	//此方法依然需要改进
	file_idx = atoi(buf);
	*plba = m_base_rec_lba + file_idx * (m_rec_fsec);

	return true;
}
#ifdef _WIN32
int CFat32Part::lcok_volume()
{
	DWORD ret;
	

	m_drive_fd = CreateFileA(m_drive_path,GENERIC_READ|GENERIC_WRITE,FILE_SHARE_READ|FILE_SHARE_WRITE,
			NULL,OPEN_EXISTING,0,NULL);
	if (m_drive_fd == INVALID_HANDLE_VALUE)
	{	
		ret = GetLastError();
		TRACE("Open volume fail lasst error %d.\n ",ret);
		return -1;
	}


	if (!DeviceIoControl(m_drive_fd, FSCTL_LOCK_VOLUME, NULL, 0, NULL, 0, &ret, NULL)) 
	{
		ret = GetLastError();
		TRACE("Lock volume fail lasst error %d.\n ",ret);
		return -1;
	}

	return 0; 
}

int	CFat32Part::ulock_volume()
{
	DWORD ret;

	if (!DeviceIoControl(m_drive_fd, FSCTL_UNLOCK_VOLUME, NULL, 0, NULL, 0, &ret, NULL)) 
	{	
		ret = GetLastError();
		TRACE("FSCTL_UNLOCK_VOLUME fail lasst error %d.\n ",ret);
		return -1;
	}

	CloseHandle(m_drive_fd);
	m_drive_fd = (HANDLE)-1;
	return 0; 
}

int CFat32Part::dis_umount_volume()
{
	DWORD ret;
	if (!DeviceIoControl(m_drive_fd, FSCTL_DISMOUNT_VOLUME, NULL, 0, NULL, 0, &ret, NULL)) 
	{
		ret = GetLastError();
		TRACE("FSCTL_DISMOUNT_VOLUME fail lasst error %d.\n ",ret);
		return -1;
	}
	return 0; 
}
#endif