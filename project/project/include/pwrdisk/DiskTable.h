#ifndef _DISK_PARTION_H 
#define  _DISK_PARTION_H

#include "scsi_dev.h"

#include <vector>
using namespace std;


typedef struct _mbrpart {
	unsigned char active;
	unsigned char shead; // CHS start value, not used by program
	unsigned char ssect; // CHS start value, not used by program
	unsigned char scyl; // CHS start value, not used by program
	unsigned char type;
	unsigned char ehead; // CHS end value, not used by program
	unsigned char esect; // CHS end value, not used by program
	unsigned char ecyl; // CHS end value, not used by program
	uint32_t start;
	uint32_t len;
}mbrpart_s;

#pragma pack(push)
#pragma pack(1)
#define MBR_N_PRI_PARTITIONS	4
typedef struct _DosRawTable {
	char			boot_code [440];
	uint32_t                mbr_signature;	/* really a unique ID */
	uint16_t                Unknown;
	mbrpart_s		partitions [MBR_N_PRI_PARTITIONS];
	uint16_t		magic;
}mbrtable_s;
#pragma pack(pop)

typedef struct __guid
{
	uint32_t data1;
	uint16_t data2;
	uint16_t data3;
	uint64_t data4;    
}guid_s;

typedef struct _gptpart{	
	guid_s type;
	guid_s uuID;
	uint64_t start;
	uint64_t end;
	uint64_t flags;
	char name[72];
}gptpart_s;


typedef struct _gpthdr
{
	unsigned char magic[8];
	unsigned char version[4];
	uint32_t hdrlen;
	uint32_t hdrsum;
	uint32_t pad;
	uint64_t this_hdr;
	uint64_t other_hdr;
	uint64_t data_start;
	uint64_t data_end;
	guid_s guid;
	uint64_t first_entry;
	uint32_t entry_cnt;
	uint32_t entry_len;
	uint32_t part_sum;
}gpthdr_s;

enum FSTYPE
{	
	FSTYPE_NONE  =  0x00,
	FSTYPE_FAT32 =  0x0B,
	FSTYPE_NTFS  =  0x07,
};

typedef vector<gptpart_s> VEC_DISKPART;
typedef vector<mbrpart_s> VEC_MBRPART;

class CDiskTable 
{
public:
	CDiskTable(void);
	~CDiskTable(void);
protected:
	CSCSI_Dev* m_pdev;
	unsigned char * m_part_buf;	//分区信息的缓冲， 包含GPT和MBR
	VEC_DISKPART m_vec_gptpart;	//GPT分区向量表
	VEC_MBRPART  m_vec_mbrpart;	//MBR分区
	bool m_isGPT;				//是否为GPT分区 
protected:
	int AssignMBR(unsigned char* data_buf);				//分析MBR分区
	int AssignGPT(unsigned char* data_buf);				//分析GPT分区
public:	
	int Init(CSCSI_Dev* pDev);
	uint64_t GetTotalSec();	
	int GetPartNum();		//获取分区的个数	
	uint64_t GetPartitionSec(int part_no);		//获取总扇区数								//获取某个分区的
	int GetBootInfo(int part_no,unsigned char* boot_buf,uint64_t* start_lba);//获取某个分区的启动扇区信息及起始扇区LBA
							
public:
	bool isGPT()
	{
		return m_isGPT;
	}
#if SUPPORT_GPT_PART
public:	//扩展有支持分区功能
 	int InitPart();
	int SetPartAuto(int part); //根据硬盘的大小等分成几个分区
	int SetPart(uint64_t part1_size,uint64_t part2_size,uint64_t part3_size,uint64_t part4_size ) ;
#endif
};


#define DEF_SECTOR_SIZE    512		//默认磁盘分区单个扇区所占的字节数。
#define SECTOR_4K		   4096    //物理的4K扇区

#define GPT_HDR_START      1	//主GPT的起始地址	以512字节为单位 
#define GPT_TABLE_START	   2    //第二个扇区是GPT的首地址 扇区大小默认为512字节
#define GPT_PARTTION	   34   //第一个GPT分区的起始扇区地址
#define GTP_BAK_SIZE	   33   //备分GPT占用了33个扇区，32个单位扇区存储GPT分区表，再1个扇区存储GPT头
#define MAX_GPT_PART_NUM   128  //最大支持的GPT分区数目
#define GPT_TABLE_LEN      32   //分区表所占
#define GPT_PERPART_SIZE   128   //每个分区表项所占的字节数

#define GPT_MAGIC {0x45, 0x46, 0x49, 0x20, 0x50, 0x41, 0x52, 0x54} // "EFI PART"

#define MBR_MAGIC				0xAA55
#define PARTITION_MAGIC_MAGIC	0xf6f6

#endif //_DISK_PARTION_H