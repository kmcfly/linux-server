#ifndef _FSINFO_H_
#define _FSINFO_H_

#include "DiskTable.h"
#include "Fat32Part.h"
#ifdef _WIN32
#include <map>
typedef map<char,CFat32Part *> MAP_FAT;
#endif

typedef struct
{
	CSCSI_Dev * pdev;
	vector<CFat32Part> vec_dataPart;

}DISK_DATAPART; //硬盘的录像数据分区


class CFSInfoTable
{
	// private constructor
public:
	CFSInfoTable(void);
	~CFSInfoTable(void);
private:
	// static member variable
	static CFSInfoTable* _Instance;
public:
	// static member function for access static member variable
	static CFSInfoTable* getSingleton();

protected:
	int	 devTable;//可以表示最大32个磁盘使用状态，1： 表示正常使用，0：表示未使用 ，/dev/sda -/dev/sdz ;
	vector<CSCSI_Dev>	  m_vec_dev;
	vector<DISK_DATAPART> m_vec_fat;
	
#ifdef _WIN32	//win32 volume 
	MAP_FAT				  m_map_fat;
public:
	void SetMapFat(char volume,CFat32Part*);
	CFat32Part* GetPart(char volume);
#endif
	int Init();
public:
	int64_t SearchFileOnDev(const char* dev,const char * fileName, uint32_t * pfSize);	//返回文件在硬盘上面的LBA地址
	int64_t SearchFileOnScsiDev(const char * fileName,CSCSI_Dev* pdev);	//返回文件在硬盘上面的LBA地址
	CSCSI_Dev* SearchScsiDev(const char* dev);

	CFat32Part* GetPart(char disk_no, int part_no);

public://inline
	int GetUsedDevTable()
	{
		return devTable;
	}

	bool isUsedDev(int devNO)
	{
		if(devNO < 32 && (devTable & 1<<(devNO-1)))
			return true;
		else
			return false;
	}
};

#endif //_FSINFO_H_
