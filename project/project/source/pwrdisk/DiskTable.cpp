#include "PUB_common.h"
#include "DiskTable.h"

#ifdef SUPPORT_GPT_PART
#include "efi_crc32.h"
#include "fs_endian.h"
#endif
#ifdef _WIN32
#pragma warning(disable: 4996)
#endif

#define NULL_GUID {0x00000000, 0x0000, 0x0000, 0x0000000000000000}

#ifdef SUPPORT_GPT_PART
// C7 D2 76 E2 64 B4 70 47
// 81 C1 BF B4 71 4A 98 02
#define DISK_GUID {le32_to_cpu(0xE276D2C7), le16_to_cpu(0xB464),\
	le16_to_cpu(0x4770), le64_to_cpu(0x81C1BFB4714A9802)}

//// #define DISK_GUID {le32_to_cpu(0x53f56307), le16_to_cpu(0xb6bf),\
//// 	le16_to_cpu(0x11d0), le64_to_cpu(0x94F200A0C91EFB8B)}

#define EFI_SYS_GUID {le32_to_cpu(0xC12A7328), le16_to_cpu(0xF81F),\
	le16_to_cpu(0x11D2), le64_to_cpu(0xBA4B00A0C93EC93BULL)}
#define MS_DATA_GUID {le32_to_cpu(0xEBD0A0A2), le16_to_cpu(0xB9E5),\
	le16_to_cpu(0x4433), le64_to_cpu(0x87C068B6B72699C7ULL)}
#define MS_META_GUID {le32_to_cpu(0x5808C8AA), le16_to_cpu(0x7E8F),\
	le16_to_cpu(0x42E0), le64_to_cpu(0x85D2E1E90434CFB3ULL)}
#define MS_DYN_GUID {le32_to_cpu(0xAF9B60A0), le16_to_cpu(0x1431),\
	le16_to_cpu(0x4F62), le64_to_cpu(0xBC683311714A69ADULL)}
#define MS_WINRE_GUID {le32_to_cpu(0xDE94BBA4), le16_to_cpu(0x06D1),\
	le16_to_cpu(0x4D40), le64_to_cpu(0xA16ABFD50179D6ACULL)}
#define LINUX_SWAP_GUID {le32_to_cpu(0x0657FD6D), le16_to_cpu(0xA4AB),\
	le16_to_cpu(0x43C4), le64_to_cpu(0x84E50933C84B4F4FULL)}
#define LINUX_DATA_GUID {le32_to_cpu(0xEBD0A0A2), le16_to_cpu(0xB9E5),\
	le16_to_cpu(0x4433), le64_to_cpu(0x87C068B6B72699C7ULL)}
#define LINUX_RAID_GUID {le32_to_cpu(0xA19D880F), le16_to_cpu(0x05FC),\
	le16_to_cpu(0x4D3B), le64_to_cpu(0xA006743F0F84911EULL)}
#define LINUX_LVM_GUID {le32_to_cpu(0xE6D6D379), le16_to_cpu(0xF507),\
	le16_to_cpu(0x44C2), le64_to_cpu(0xA23C238F2A3DF928ULL)}
#define APPLE_HFS_GUID {le32_to_cpu(0x48465300), le16_to_cpu(0x0000),\
	le16_to_cpu(0x11AA), le64_to_cpu(0xAA1100306543ECACULL)}
#define APPLE_UFS_GUID {le32_to_cpu(0x55465300), le16_to_cpu(0x0000),\
	le16_to_cpu(0x11AA), le64_to_cpu(0xAA1100306543ECACULL)}
#define APPLE_BOOT_GUID {le32_to_cpu(0x426F6F74), le16_to_cpu(0x0000),\
	le16_to_cpu(0x11AA), le64_to_cpu(0xAA1100306543ECACULL)}
#define SUN_BOOT_GUID {le32_to_cpu(0x6A82CB45), le16_to_cpu(0x1DD2),\
	le16_to_cpu(0x11B2), le64_to_cpu(0x99A6080020736631ULL)}
#define SUN_ROOT_GUID {le32_to_cpu(0x6A85CF4D), le16_to_cpu(0x1DD2),\
	le16_to_cpu(0x11B2), le64_to_cpu(0x99A6080020736631ULL)}

#define MBR2GUID(x) {le32_to_cpu(0x1575DA16), le16_to_cpu(0xF2E2),\
	le16_to_cpu(0x40DE), (le64_to_cpu(0xB715C6E376663B00ULL + x))}
#endif

CDiskTable::CDiskTable(void)
{
	m_part_buf = NULL;
	m_isGPT = false;
}

CDiskTable::~CDiskTable()
{
	if(m_part_buf)
		free(m_part_buf);
}

int CDiskTable::Init(CSCSI_Dev* pDev)
{
	int fd;
	int ret = 0;

	//if(pDev->is_closed())
	{
		fd = (int)pDev->Open();
		if(fd == -1)
		{
			return -1;
		}
	}

	m_vec_gptpart.clear();
	m_vec_mbrpart.clear();
	m_isGPT = false;
	m_pdev = pDev;
	m_part_buf = (unsigned char*)malloc(SECTOR_4K);

	//读取前面多个扇区，以备分析其中的分区表头信息
	ret = m_pdev->Read(0,m_part_buf,SECTOR_4K);
	if(ret < 0)
	{
		printf("Read data error.\n");
		goto ERR_OUT;
	}

	if(AssignMBR(m_part_buf) > 0)
	{
		free(m_part_buf);
		m_part_buf =NULL;
		return 0;
	}
	else if(AssignGPT(m_part_buf) < 0)
	{
		goto ERR_OUT;
	}

	if(m_part_buf)
	{
		free(m_part_buf);
		m_part_buf =NULL;
	}
	return 0;

ERR_OUT:
	if(m_part_buf)
	{
		free(m_part_buf);
		m_part_buf = NULL;
	}
	return ret;
}

uint64_t CDiskTable::GetTotalSec()
{
	return m_pdev->get_capacity();
}

int CDiskTable::GetPartNum()
{
	if(m_isGPT)
		return (int)m_vec_gptpart.size();
	else
		return (int)m_vec_mbrpart.size();
}

uint64_t CDiskTable::GetPartitionSec(int part_no)//获取某个分区的
{
	uint64_t part_secs = 0;
	if(m_isGPT)
	{
		if(part_no >= MAX_GPT_PART_NUM || part_no > (int)m_vec_gptpart.size())
		{
			printf("Error part no %u\n",part_no);
			return 0;
		}
		part_secs = m_vec_gptpart[part_no].end - m_vec_gptpart[part_no].start;
	}
	else
	{
		if(part_no >= MAX_GPT_PART_NUM || part_no > (int)m_vec_mbrpart.size())
		{
			printf("Error part no %u\n",part_no);
			return 0;
		}
		part_secs = m_vec_mbrpart[part_no].len;
	}

	return part_secs;
}

int CDiskTable::GetBootInfo(int part_no,unsigned char* boot_buf,uint64_t* start_lba)//获取某个分区的启动扇区信息及起始扇区LBA
{
	unsigned char tmp_buf[SECTOR_4K];
	int ret;
	uint64_t start;
    int part_num;
	if(m_isGPT)
    {
        start = m_vec_gptpart[part_no].start;
        part_num = m_vec_gptpart.size();
    }
	else
	{
	    start = m_vec_mbrpart[part_no].start;
	    part_num = m_vec_mbrpart.size();
	}
	if(start_lba)
		*start_lba = start;

	if(part_no > part_num)
	{
		return -1;
	}
	ret = m_pdev->Read(start,tmp_buf,SECTOR_4K);
	if(ret < 0)
	{
		printf("Read data fail.\n");
		return ERR_READ_FAIL;
	}
	memcpy(boot_buf,tmp_buf,DEF_SECTOR_SIZE);

	return 0;
}

int CDiskTable::AssignMBR(unsigned char* data_buf)			//读取MBR分区
{
	int i;
	mbrtable_s * pmbrtable = (mbrtable_s*)data_buf;
	mbrpart_s mbrpart;
	unsigned char gpt_name[8] = GPT_MAGIC;
	if(data_buf == NULL)
		return -1;

	if(pmbrtable->magic != MBR_MAGIC)
		return -1;

	m_vec_mbrpart.clear();
	for(i = 0 ; i < MBR_N_PRI_PARTITIONS; i++)
	{
		memcpy(&mbrpart,data_buf+446+16*i,sizeof(mbrpart_s));
		if(mbrpart.type == 0xEE) //保护分区
		{
			gpthdr_s * pgpt_hdr = (gpthdr_s *)(data_buf+512);
			if(memcmp(pgpt_hdr->magic,gpt_name,8) == 0)//确认是GPT分区则退出
			{
				printf("The table is GPT.\n");
				m_isGPT = true;
				return -1;
			}
		}

		m_vec_mbrpart.push_back(mbrpart);
	}

	return (int)m_vec_mbrpart.size();
}

int CDiskTable::AssignGPT(unsigned char* data_buf)		//读取GPT分区
{
	int ret = 0;
	unsigned char gpt_name[8] = GPT_MAGIC;
	gpthdr_s * pgpt_hdr;
	gptpart_s gpt_part;
	guid_s null_guid =  NULL_GUID;// NULL_GUID;
	int idx;

	if(data_buf == NULL)
	{
		return -1;
	}
#ifdef _WIN32
	unsigned char part_buf[0x4000] = {0};
#endif
	pgpt_hdr = (gpthdr_s*)(data_buf+512);
	//省去分析MBR,直接读取GPT表头

	if(memcmp(gpt_name,pgpt_hdr->magic,8)!= 0)
	{
		printf("Not GPT parttion.\\n");
		ret = -1;
		goto ERR_OUT;
	}

	//检查表头的校验和
	m_vec_gptpart.clear();
	//分析剩余的 4K - 1K = 3K, 3*4 = 12;
	for(idx = 0; idx < 24; idx++)
	{
		memcpy(&gpt_part,m_part_buf +2*DEF_SECTOR_SIZE + idx * GPT_PERPART_SIZE,sizeof(gptpart_s));
		if(memcmp(&gpt_part.type,(void *)&null_guid,16) != 0) //
		{
			m_vec_gptpart.push_back(gpt_part);
		}
	}
	return 0;
#ifdef _WIN32	//假如在windows 测试，则支持扫描更多的分区, 嵌入式平台下面的只需要四个以内的分区。
	ret = m_pdev->Read_try(SECTOR_4K/m_pdev->get_sec_size(),0x4000/m_pdev->get_sec_size(),part_buf,0x4000);
	if(ret == 0)//正常的情况
	{
		for(idx = 24; idx < MAX_GPT_PART_NUM; idx++)
		{
			memcpy(&gpt_part,part_buf+ (idx - 24) * GPT_PERPART_SIZE,sizeof(gptpart_s));
			if(memcmp(&gpt_part.type,(void *)&null_guid,16) != 0) //
			{
				m_vec_gptpart.push_back(gpt_part);
			}
		}
	}
#endif
	return 0;
ERR_OUT:
	return ret;
}
