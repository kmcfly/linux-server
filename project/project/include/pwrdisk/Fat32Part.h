#ifndef _FAT32_PART_H
#define _FAT32_PART_H 
#include "fat32_fs.h"
#include "scsi_dev.h"

#include <vector>
using namespace std;

typedef vector<msdos_dir_entry> VEC_FILE_INFO;

typedef struct
{
	char name[12];
	uint64_t lba;
}RECORD_LBA,*PRECORD_LBA;
typedef vector<RECORD_LBA> VEC_FILE_LBA;

class CFat32Part
{
public:

	CFat32Part(void);
public:
	~CFat32Part(void);
protected:
	CSCSI_Dev * m_pdev;			//���������Ĵ����豸
	fat_boot_sector	m_boot_info;//FAT����������Ϣ

	uint64_t m_base_lba;		//FAT32�����Ļ���LBA
	uint64_t m_base_data_lba;	//FAT32����������������ʼLBA
	uint64_t m_root_lba;		//��Ŀ¼�ļ���LBA
	uint64_t m_base_rec_lba;	//��һ��¼���ļ�����ʼLBA
	uint32_t m_rec_fsize;		//Ԥ¼�ļ���С
	uint32_t m_rec_fsec;		//Ԥ¼�ļ���ռ�õ�����

public:
	int Init(CSCSI_Dev * pdev,fat_boot_sector* pfat_boot,uint64_t base_lba);
	uint64_t GetFreeSecotr();
	int AssignFile(uint32_t f_size,uint32_t f_num,char* fname_head,char * fname_ext);
	int ScanFileInfo(VEC_FILE_INFO* pvec_finfo);
	int ScanFileLBA(VEC_FILE_LBA* pvec_flba);
	int VecFileInfoToFileLBA(VEC_FILE_INFO* pvec_finfo,VEC_FILE_LBA* pvec_flba);
	bool FileNameToLBA(const char* f_name,uint64_t* plba);	//���ļ���ת��LBA��������Ԥ�����ļ�
	
#ifdef _WIN32  //Win32 �����������;
protected:
	char  m_drive_path[8];	//
	HANDLE m_drive_fd;		//���
	char  m_part_name[20];		//�豸��+������ 
public:
	int lcok_volume();
	int	ulock_volume();
	int dis_umount_volume();

public:
	void SetDrivePath(char * drive_path)
	{
		m_drive_path[4] = drive_path[0];
	}

	void SetPhysicalPart(char * drive)
	{
		strcpy(m_part_name,drive);
	}

	char* GetPhysicalPart()	//��ȡWindows����ķ������豸��  \\.\X:
	{
		 return m_part_name;
	}

	char* GetDriverPath()
	{
		static char drive_path[4] = "Z:\\";
		drive_path[0] = m_drive_path[4];	
		return (char*)drive_path;
	}
#endif

public: 
	uint64_t GetBaseLba()
	{
		return m_base_lba;
	}
	uint64_t GetTotalSector()
	{
		return m_boot_info.total_sect;
	}
	uint64_t GetFileLBA(msdos_dir_entry* pfile_info)
	{
		uint32_t cluster_no = (uint32_t)pfile_info->starthi << 16 |  (uint32_t)pfile_info->start;
		return  m_base_data_lba + (cluster_no - 2) * m_boot_info.sec_per_clus;
	}
	uint32_t GetRecFileSize()
	{
		return m_rec_fsize;
	}
};

#define  FAT_BOOT_SIZE  512
#define  DIR_INFO_SIZE 32 
#endif //_FAT32_PART_H
