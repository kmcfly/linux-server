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
	unsigned char * m_part_buf;	//������Ϣ�Ļ��壬 ����GPT��MBR
	VEC_DISKPART m_vec_gptpart;	//GPT����������
	VEC_MBRPART  m_vec_mbrpart;	//MBR����
	bool m_isGPT;				//�Ƿ�ΪGPT���� 
protected:
	int AssignMBR(unsigned char* data_buf);				//����MBR����
	int AssignGPT(unsigned char* data_buf);				//����GPT����
public:	
	int Init(CSCSI_Dev* pDev);
	uint64_t GetTotalSec();	
	int GetPartNum();		//��ȡ�����ĸ���	
	uint64_t GetPartitionSec(int part_no);		//��ȡ��������								//��ȡĳ��������
	int GetBootInfo(int part_no,unsigned char* boot_buf,uint64_t* start_lba);//��ȡĳ������������������Ϣ����ʼ����LBA
							
public:
	bool isGPT()
	{
		return m_isGPT;
	}
#if SUPPORT_GPT_PART
public:	//��չ��֧�ַ�������
 	int InitPart();
	int SetPartAuto(int part); //����Ӳ�̵Ĵ�С�ȷֳɼ�������
	int SetPart(uint64_t part1_size,uint64_t part2_size,uint64_t part3_size,uint64_t part4_size ) ;
#endif
};


#define DEF_SECTOR_SIZE    512		//Ĭ�ϴ��̷�������������ռ���ֽ�����
#define SECTOR_4K		   4096    //�����4K����

#define GPT_HDR_START      1	//��GPT����ʼ��ַ	��512�ֽ�Ϊ��λ 
#define GPT_TABLE_START	   2    //�ڶ���������GPT���׵�ַ ������СĬ��Ϊ512�ֽ�
#define GPT_PARTTION	   34   //��һ��GPT��������ʼ������ַ
#define GTP_BAK_SIZE	   33   //����GPTռ����33��������32����λ�����洢GPT��������1�������洢GPTͷ
#define MAX_GPT_PART_NUM   128  //���֧�ֵ�GPT������Ŀ
#define GPT_TABLE_LEN      32   //��������ռ
#define GPT_PERPART_SIZE   128   //ÿ������������ռ���ֽ���

#define GPT_MAGIC {0x45, 0x46, 0x49, 0x20, 0x50, 0x41, 0x52, 0x54} // "EFI PART"

#define MBR_MAGIC				0xAA55
#define PARTITION_MAGIC_MAGIC	0xf6f6

#endif //_DISK_PARTION_H