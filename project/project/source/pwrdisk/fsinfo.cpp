
#include "PUB_common.h"
#include "fsinfo.h"

CFSInfoTable::CFSInfoTable(void)
{
	devTable = 0;

}

CFSInfoTable::~CFSInfoTable(void)
{
	if (NULL != CFSInfoTable::_Instance)
	{
		delete CFSInfoTable::_Instance;
		CFSInfoTable::_Instance = NULL;
	}
}


// Initialize static member variable
CFSInfoTable* CFSInfoTable::_Instance = NULL;

// get Singleton here
CFSInfoTable* CFSInfoTable::getSingleton()
{
	if (NULL == _Instance)
	{
		_Instance = new CFSInfoTable;
		_Instance->Init();
	}

	return _Instance;
}

int CFSInfoTable::Init()
{
	int dev_idx;
	int part_idx;
	char dev_name[20];
	char dev_no = 0;

	CSCSI_Dev scsi_dev;
	CSCSI_Dev * pdev;
	CDiskTable disk_table;
	CFat32Part data_part;
	DISK_DATAPART disk_part;
	unsigned char boot_buf[512];
	uint64_t start_lba;
	CFat32Part fat_part;
	/*
		扫描系统中存在的磁盘设备
	*/
	m_vec_dev.clear();
	for (dev_idx = 0; dev_idx < 26; dev_idx++ )
	{
#ifdef _WIN32
		sprintf(dev_name, "\\\\.\\PhysicalDrive%d", dev_idx);
#else
		dev_no = 'a'+dev_idx;
		sprintf(dev_name,"/dev/sd%c",dev_no);
#endif
		if(scsi_dev.associate(dev_name) >= 0)
		{
			m_vec_dev.push_back(scsi_dev);
			m_vec_dev[dev_idx].init_devlock();
		}
		devTable = devTable| (1<< dev_idx) ;
	}

	/*
		读取每个设备的分区表
	*/
	for(dev_idx = 0 ; dev_idx < (int)m_vec_dev.size();dev_idx++)
	{
	    pdev =  &m_vec_dev[dev_idx];
		disk_part.vec_dataPart.clear();	//清除临时分区容器
	    if(disk_table.Init(pdev))
        {
            continue;
		}
		disk_part.pdev = pdev;

		for(part_idx = 0; part_idx < disk_table.GetPartNum();part_idx++)
		{
			if(disk_table.GetBootInfo(part_idx,boot_buf,&start_lba))
			{
				continue;
			}
			if(0 == fat_part.Init(pdev,(fat_boot_sector*)boot_buf,start_lba))
			{
				disk_part.vec_dataPart.push_back(fat_part);
			}
		}
		m_vec_fat.push_back(disk_part);
	}

	return (int)m_vec_dev.size();
}

int64_t CFSInfoTable::SearchFileOnDev(const char* dev,const char * fileName, uint32_t * pfSize)	//返回文件在硬盘上面的LBA地址
{
	int64_t lba = -1,start_lba = -1;
	int idx;
	bool bRet;
	DISK_DATAPART disk_part;
	unsigned char boot_buf[512] = {0};
	CFat32Part * pfat = NULL;
	CFat32Part tmpFat;
	int file_idx;
	char dev_name[20]={0};
	int part_no = 0;	//分区编号
	char last_ch;
	size_t dev_len;
    int vec_num;

	dev_len = strlen(dev);
	last_ch = dev[dev_len-1];

	if( last_ch >= 0x30 && last_ch < 0x39 ) //表示数字
	{
		part_no = last_ch - 0x30 - 1;	//分区编号
		if(dev_len < 20)
			memcpy(dev_name,dev,dev_len-1);
	}
	else
	{
		memcpy(dev_name,dev,dev_len);
	}

	file_idx = atoi(fileName);

    vec_num = (int)m_vec_fat.size();
	//寻找设备及相关的录像分区
//	TRACE_DBG("Dev %s Device name %s vec dev size %d. fileName %s \n ", dev,dev_name,vec_num,fileName);
	for(idx = 0; idx < vec_num; idx++)
	{
        if(strstr(dev_name,m_vec_fat[idx].pdev->get_device_name()))
		{
			if(part_no <(int) m_vec_fat[idx].vec_dataPart.size())
			{
				pfat = &m_vec_fat[idx].vec_dataPart[part_no];
				break;
			}
		}
	}

	//假如没有找到
	if(pfat == NULL)
	{
		CSCSI_Dev* pdev = NULL;
		CSCSI_Dev scsi_dev;
		CDiskTable disk_table;
		bool bFound = false;

		for(idx = 0 ; idx < (int)m_vec_dev.size(); idx ++)
		{
			if(strstr(dev_name,m_vec_dev[idx].get_device_name()))
			{
				pdev = &m_vec_dev[idx];
				bFound = true;
				break;
			}
		}

		if(!bFound)
		{
			if(scsi_dev.associate(dev_name) < 0)
			{
				goto ERR_OUT;
			}
			m_vec_dev.push_back(scsi_dev);
			pdev = &m_vec_dev[m_vec_dev.size()-1];
			pdev->init_devlock();
		}

		if(disk_table.Init(pdev))
		{
			goto ERR_OUT;
		}

		disk_part.pdev = pdev;
		for(idx = 0; idx < disk_table.GetPartNum();idx++)
		{
			if(disk_table.GetBootInfo(idx,boot_buf,(uint64_t*)&start_lba))
			{
				continue;
			}
			if(0 == tmpFat.Init(pdev,(fat_boot_sector*)boot_buf,start_lba))
			{
				disk_part.vec_dataPart.push_back(tmpFat);
			}
		}
		m_vec_fat.push_back(disk_part);

		if(part_no >= (int)m_vec_fat[m_vec_fat.size()-1].vec_dataPart.size())
		{
			goto ERR_OUT;
		}
		pfat = &m_vec_fat[m_vec_fat.size()-1].vec_dataPart[part_no];
	}

	if(NULL != pfat)
	{
		bRet = pfat->FileNameToLBA(fileName,(uint64_t*)&lba);
		*pfSize = pfat->GetRecFileSize();
	}
ERR_OUT:
//	TRACE_DBG("FileName %s  LBA %lld\n ", fileName,lba);
	return lba;
}

/*
功能还没有通过调试，可能不正确
*/

int64_t CFSInfoTable::SearchFileOnScsiDev(const char * fileName,CSCSI_Dev* pdev)//返回文件在硬盘上面的LBA地址
{
	int64_t lba = -1;
	CFat32Part* pfat ;
	DISK_DATAPART disk_part;
	int part_no = 0;	//分区编号
    int vec_num;
	int idx;

	vec_num = (int)m_vec_fat.size();
	for(idx = 0; idx < vec_num; idx++)
	{
		if(pdev == m_vec_fat[idx].pdev)
		{
			if(part_no <(int) m_vec_fat[idx].vec_dataPart.size())
			{
				pfat = &m_vec_fat[idx].vec_dataPart[part_no];
				break;
			}
		}
	}

	pfat->FileNameToLBA(fileName,(uint64_t*)&lba);

	return lba;
}

CSCSI_Dev* CFSInfoTable::SearchScsiDev(const char* dev)
{
	int idx;
	CSCSI_Dev scsi_dev;

	char dev_name[20] ={0};
	char last_ch;
	size_t dev_len;

	dev_len = strlen(dev);
	last_ch = dev[dev_len-1];

	if( last_ch >= 0x30 && last_ch <= 0x39 ) //表示数字
	{
		memcpy(dev_name,dev,dev_len-1);
	}
	else
	{
		memcpy(dev_name,dev,dev_len);
	}

	//TRACE_DBG("Dev %s Device name %s vec dev size %d.\n", dev,dev_name,(int)m_vec_dev.size());
	for(idx = 0; idx < (int)m_vec_dev.size(); idx++)
	{
        if(strstr(m_vec_dev[idx].get_device_name(), dev_name))
		{
		//	TRACE_DBG("Find device idx %d.\n",idx);
			return &m_vec_dev[idx];
		}
	}
	TRACE_ERR(" 1 Can not find device . dev_name %s \n",dev_name);
	if(scsi_dev.associate(dev_name) >= 0)
	{
		m_vec_dev.push_back(scsi_dev);
		return &m_vec_dev[m_vec_dev.size()-1];
	}
	TRACE_ERR(" 2 Can not find device . dev_name %s \n",dev_name);
	return NULL;
}

CFat32Part* CFSInfoTable::GetPart(char disk_no, int part_no)
{
	CFat32Part* pFat = NULL;
	int disk_num = (int) m_vec_fat.size();
	int idx = 0;
	char dev_name[20];

#ifdef _WIN32
	if(disk_no > 16)
		return NULL;
	sprintf(dev_name, "\\\\.\\PhysicalDrive%d", disk_no);
#else
	if(disk_no>='a' && disk_no <= 'z')
	{
		sprintf(dev_name,"/dev/sd%c",disk_no);
	}
	else
	{
		return NULL;
	}
#endif

	if(part_no)
		part_no--;
	for(idx = 0; idx < disk_num; idx++)
	{
        if(strstr(dev_name,m_vec_fat[idx].pdev->get_device_name()))
		{
			if(part_no <(int) m_vec_fat[idx].vec_dataPart.size())
			{
				pFat = &m_vec_fat[idx].vec_dataPart[part_no];
				break;
			}
		}
	}

	return pFat;
}
#ifdef _WIN32
void CFSInfoTable::SetMapFat(char volume,CFat32Part* pfat)
{
	m_map_fat[volume] = pfat;
}

CFat32Part* CFSInfoTable::GetPart(char volume)
{
	return m_map_fat[volume];
}
#endif
