/***********************************************************************
** Copyright   Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : 
** Date         : 2007-09-3
** Name         : Disk_config.h
** Version      : 1.0
** Description  :
** Modify Record:
1:
***********************************************************************/

#ifndef _DISK_CONFIG_H_
#define _DISK_CONFIG_H_
#include "Typedef.h"
#include <stdio.h>
#include <stdlib.h>
#include <linux/unistd.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <ctype.h>
#include <time.h>
#include <sys/utsname.h>
#include <sys/ioctl.h>
#include <mntent.h>
#include <sys/stat.h>
#include <sys/mount.h>
#include "asm/types.h"
//#include <asm/byteorder.h>//define __u8//typedef unsigned char __u8;
#include <assert.h>
#include <dirent.h>
#include <sys/vfs.h> 
#include <sys/types.h>
#include <scsi/sg.h>
#include "mylist.cpp"
#include "dvrdvsdef.h"
#if defined(__VERY_BIG_DISK__)
#include <sys/ioctl.h>
#include <fcntl.h>
#include "uuid.h"
#endif


///////////////////////////////
#if __BYTE_ORDER == __BIG_ENDIAN //(1)

#ifdef __le16_to_cpu //(2)

#define CF_LE_W	__le16_to_cpu
#define CF_LE_L	__le32_to_cpu
#define CT_LE_W	__cpu_to_le16
#define CT_LE_L	__cpu_to_le32
#else //(2)
#define CF_LE_W(v) ((((v) & 0xff) << 8) | (((v) >> 8) & 0xff))
#define CF_LE_L(v) (((unsigned)(v)>>24) | (((unsigned)(v)>>8)&0xff00) | \
               (((unsigned)(v)<<8)&0xff0000) | ((unsigned)(v)<<24))
#define CT_LE_W(v) CF_LE_W(v)
#define CT_LE_L(v) CF_LE_L(v)
#endif /* defined(__le16_to_cpu) */  //(2)
    
#else //(1)

#define CF_LE_W(v) (v)
#define CF_LE_L(v) (v)
#define CT_LE_W(v) (v)
#define CT_LE_L(v) (v)

#endif /* __BIG_ENDIAN */  //(1)


#define SG_CHECK_CONDITION    0x02
#define SG_DRIVER_SENSE        0x08

#define SG_ATA_16        0x85
#define SG_ATA_16_LEN        16

#define SG_ATA_LBA48        1
#define SG_ATA_PROTO_NON_DATA    ( 3 << 1)
#define SG_ATA_PROTO_PIO_IN    ( 4 << 1)
#define SG_ATA_PROTO_PIO_OUT    ( 5 << 1)
#define SG_ATA_PROTO_DMA    ( 6 << 1)
#define SG_ATA_PROTO_UDMA_IN    (11 << 1) /* not yet supported in libata */
#define SG_ATA_PROTO_UDMA_OUT    (12 << 1) /* not yet supported in libata */

#define ATA_USING_LBA        (1 << 6)

enum 
{
	ATA_OP_READ_PIO                 = 0x20,
	ATA_OP_READ_PIO_ONCE            = 0x21,
	ATA_OP_READ_LONG                = 0x22,
	ATA_OP_READ_LONG_ONCE           = 0x23,
	ATA_OP_READ_PIO_EXT             = 0x24,
	ATA_OP_READ_DMA_EXT             = 0x25,
	ATA_OP_WRITE_PIO                = 0x30,
	ATA_OP_WRITE_LONG               = 0x32,
	ATA_OP_WRITE_LONG_ONCE          = 0x33,
	ATA_OP_WRITE_PIO_EXT            = 0x34,
	ATA_OP_WRITE_DMA_EXT            = 0x35,
	ATA_OP_READ_VERIFY_ONCE         = 0x41,
	ATA_OP_WRITE_UNC_EXT            = 0x45,
	ATA_OP_STANDBYNOW2              = 0x94,
	ATA_OP_SETIDLE2                 = 0x97,
	ATA_OP_CHECKPOWERMODE2          = 0x98,
	ATA_OP_SLEEPNOW2                = 0x99,
	ATA_OP_PIDENTIFY                = 0xa1,
	ATA_OP_READ_NATIVE_MAX          = 0xf8,
	ATA_OP_READ_NATIVE_MAX_EXT      = 0x27,
	ATA_OP_SMART                    = 0xb0,
	ATA_OP_READ_DMA                 = 0xc8,
	ATA_OP_WRITE_DMA                = 0xca,
	ATA_OP_DOORLOCK                 = 0xde,
	ATA_OP_DOORUNLOCK               = 0xdf,
	ATA_OP_STANDBYNOW1              = 0xe0,
	ATA_OP_SETIDLE1                 = 0xe3,
	ATA_OP_SET_MAX                  = 0xf9,
	ATA_OP_SET_MAX_EXT              = 0x37,
	ATA_OP_CHECKPOWERMODE1          = 0xe5,
	ATA_OP_SLEEPNOW1                = 0xe6,
	ATA_OP_FLUSHCACHE               = 0xe7,
	ATA_OP_FLUSHCACHE_EXT           = 0xea,
	ATA_OP_IDENTIFY                 = 0xec,
	ATA_OP_SETFEATURES              = 0xef,
	ATA_OP_SECURITY_SET_PASS        = 0xf1,
	ATA_OP_SECURITY_UNLOCK          = 0xf2,
	ATA_OP_SECURITY_ERASE_PREPARE   = 0xf3,
	ATA_OP_SECURITY_ERASE_UNIT      = 0xf4,
	ATA_OP_SECURITY_FREEZE_LOCK     = 0xf5,
	ATA_OP_SECURITY_DISABLE         = 0xf6,
};

enum 
{
    SG_CDB2_TLEN_NODATA    = 0 << 0,
	SG_CDB2_TLEN_FEAT    = 1 << 0,
	SG_CDB2_TLEN_NSECT    = 2 << 0,
	
	SG_CDB2_TLEN_BYTES    = 0 << 2,
	SG_CDB2_TLEN_SECTORS    = 1 << 2,

    SG_CDB2_TDIR_TO_DEV    = 0 << 3,
    SG_CDB2_TDIR_FROM_DEV    = 1 << 3,

    SG_CDB2_CHECK_COND    = 1 << 5,
};

#define READ_ATTRIBUTES         0xD0
#define READ_THRESHOLDS         0xD1 
#define SMART_CMD               0xB0 


#define JPEG_FILE_NUM          1             //保存jpeg图片的文件个数


#define DISK_PRODUCT_NAME      "product="
#define DISK_ID                "id="
#define DISK_ACCESS_PROPERTY   "property="

#define MAX_PATH                256
#define NAME_LENGTH             20
#define BOOT_LENGTH             512
#define DEFAULT_SECTOR_SIZE     512
//#define BLKGETSIZE            _IO(0x12,96)    /* return device size */
#if defined(__VERY_BIG_DISK__)
//#define BLKRRPART  _IO(0x12,95) /* re-read partition table */
//#define BLKGETSIZE _IO(0x12,96) /* return device size */
//#define BLKFLSBUF  _IO(0x12,97) /* flush buffer cache */
#define BLKSSZGET  _IO(0x12,104) /* get block device sector size */
#define BLKSSZGET  _IO(0x12,104) /* get block device physical sector size */
#define BLKBSZGET  _IOR(0x12,112,size_t) /* get block device logical sector size */
#define BLKGETLASTSECT  _IO(0x12,108) /* get last sector of block device */
#define BLKSETLASTSECT  _IO(0x12,109) /* set last sector of block device */
#define BLKBSZSET  _IOW(0x12,113,size_t)
#define BLKGETSIZE64  _IOR(0x12,114,size_t)    
#endif


#define HDIO_GETGEO		        0x0301	/* get device geometry */
//#define BLKRRPART             _IO(0x12,95)/* re-read partition table */
#define DEFAULT_FAT_NUM         2
#define DBR_RESERVED_SECTORS    32
#define MSDOS_FAT32_SIGN        "FAT32   " 
#define EXTENDED                0x05 //扩展分区标准
#define FAT32                   0x0b //FAT32分区格式
#define BOOTCODE_FAT32_SIZE	    420
#define BOOTCODE_SIZE		    448
#define BOOT_SIGN               0xAA55 
//#define BLOCK_SIZE 
#define SECTORS_PER_BLOCK       ( BLOCK_SIZE / DEFAULT_SECTOR_SIZE )
#define MAX_CLUST_32            ((1 << 28) - 16) 
#define ATTR_VOLUME             8 
#define DPT_PARTTION            4  //分区表的最大项数

#define PROC_PARTITIONS         "/proc/partitions" //设备信息文件
#define MAX_PARTTION            16 //支持的最大分区数



#define FIXEDDISK_BEGIN_MOUNT_DIR  0
#define FIXEDDISK_END_MOUNT_DIR    31
#define FIXEDDISK_MAX_MOUNT_DIR    32


#define REMOVEABLEDISK_BEGIN_MOUNT_DIR 32
#define REMOVEABLEDISK_END_MOUNT_DIR   95
#define REMOVEABLEDISK_MAX_MOUNT_DIR   64

#define CD_BEGIN_MOUNT_DIR 36

#define ESATA_BEGIN_MOUNT_DIR 48


//同一种类型设备允许的最大数量
#define A_TYPE_MAX_NUM      16

//目前支持的usb设备的个数
#define USB_DISK_MAX_NUM     1








//分区信息结构
typedef struct _part_info
{
	BYTE   partid;              //分区所挂载目录的序列号.如某个分区挂载到/mnt/00的00
	BYTE   partformat;          //分区格式（FAT16、FAT32、NTFS、EXT3、EXT等）
	ULONG  partsize;            //分区大小（单位M）
	LONG   reclogsize;          //此分区录像LOG文件大小（单位BYTE）
	LONG   eventlogsize;        //此分区操作LOG文件大小(单位BYTE)
	LONG   datafilenum;         //此分区视音频数据文件数量
	LONG   usedfilenum;         //视音频文件中被使用的数量
	LONG   freefilenum;         //视音频文件中空闲文件数量
	LONG   usedspace;           //此分区已经使用空间（单位M）
	LONG   freespace;           //此分区剩余空间（单位M）
	char   devnod[NAME_LENGTH];    //分区设备节点.如:/dev/sda1
	char   partpath[MAX_PATH];  //访问分区的决定路径
}PART_INFO;

//typedef CMyList <PART_INFO> PART_INFO_LIST;

//设备信息结构
typedef struct _disk_info
{
	BYTE diskOwnedBy;        //隶属于本DVR还是其他的DVR
	BYTE diskid;             //磁盘索引号（磁盘管理模块统一分配）
	BYTE diskType;           //对应于DISK_TYPE；NET_DISK_TYPE_FIXEDDISK，NET_DISK_TYPE_REMOVEABLE，NET_DISK_TYPE_CDDVD
	BYTE diskInterfacetype;  //对应于DISK_INTERFACE_TYPE；NET_DISK_IDE_INTERFACE，NET_DISK_SATA_INTERFACE
	BYTE interfaceindex;     //接口序号(所在USB、IDE、SATA的序号)
	BYTE diskStatus;         //是否已经挂载
	BYTE diskProperty;       //表示硬盘的属性，只读或可读可写
	                         //true(1) -- 有效，false(0) -- 无效
	                         //true(1) -- 有效，false(0) -- 无效
	ULONG disksize;          //磁盘大小（单位M）
	ULONG freesize;          //磁盘剩余空间
	ULONG diskpara;          //磁盘的其它参数，除光盘外其它磁盘暂时还没有
	                         //意义;光盘的四个参数直接顺序占用四个BYTE
	char diskname[NAME_LENGTH]; //磁盘名称:如/dev/sda
	BYTE partitionNum;      //可用分区数
	PART_INFO partitionInfo[16];
}DISK_INFO;

typedef CMyList <DISK_INFO> DISK_INFO_LIST;

typedef CMyList <int> ESATA_INDEX_LIST;
//分区表中分区项的结构
struct partition 
{
//	unsigned char boot_ind;         /* 0x80 - active */
	unsigned char boot_ind:7;
	unsigned char lock_status:1;
	unsigned char head;             /* starting head */
	unsigned char sector;           /* starting sector */
	unsigned char cyl;              /* starting cylinder */
	unsigned char sys_ind;          /* What partition type */
	unsigned char end_head;         /* end head */
	unsigned char end_sector;       /* end sector */
	unsigned char end_cyl;          /* end cylinder */
	unsigned char start4[4];        /* starting sector counting from 0 */
	unsigned char size4[4];         /* nr of sectors in partition */
};


#define pt_offset(b, n)	 ((struct partition *)((b) + 0x1be  \
                           + (n) * sizeof(struct partition)))

#if defined(__VERY_BIG_DISK__)
#define mbr_pt_offset(b, n)	 ((struct __mbr_partition *)((b) + 0x1be  \
                           + (n) * sizeof(struct __mbr_partition)))
#endif

struct hd_geometry 
{
      unsigned char heads;
      unsigned char sectors;
      unsigned short cylinders;
      unsigned long start;
};

struct msdos_volume_info 
{
	__u8		drive_number;	/* BIOS drive number */
	__u8		RESERVED;	/* Unused */
	__u8		ext_boot_sign;	/* 0x29 if fields below exist (DOS 3.3+) */
	__u8		volume_id[4];	/* Volume ID number */
	__u8		volume_label[11];/* Volume label */
	__u8		fs_type[8];	/* Typically FAT12 or FAT16 */
} __attribute__ ((packed));


struct msdos_boot_sector//512个字节
{
  __u8	        boot_jump[3];	/* Boot strap short or near jump */
  __u8          system_id[8];	/* Name - can be used to special case
				   partition manager volumes */
  __u8          sector_size[2];	/* bytes per logical sector */
  __u8          cluster_size;	/* sectors/cluster */
  __u16         reserved;	/* reserved sectors */
  __u8          fats;		/* number of FATs */
  __u8          dir_entries[2];	/* root directory entries */
  __u8          sectors[2];	/* number of sectors */
  __u8          media;		/* media code (unused) */
  __u16         fat_length;	/* sectors/FAT */
  __u16         secs_track;	/* sectors per track */
  __u16         heads;		/* number of heads */
  __u32         hidden;		/* hidden sectors (unused) */
  __u32         total_sect;	/* number of sectors (if sectors == 0) */
  union 
  {
	  struct 
	  {
		  msdos_volume_info vi;//26个字节
		  __u8	boot_code[BOOTCODE_SIZE];//448个字节+26=474
	  } __attribute__ ((packed)) _oldfat;
	  struct 
	  {
		  __u32	fat32_length;	/* sectors/FAT */
		  __u16	flags;		/* bit 8: fat mirroring, low 4: active fat */
		  __u8	version[2];	/* major, minor filesystem version */
		  __u32	root_cluster;	/* first cluster in root directory */
		  __u16	info_sector;	/* filesystem info sector */
		  __u16	backup_boot;	/* backup boot sector */
		  __u16	reserved2[6];	/* Unused */
		  msdos_volume_info vi;
		  __u8	boot_code[BOOTCODE_FAT32_SIZE];//420个字节+54=474
	  } __attribute__ ((packed)) _fat32;
  } __attribute__ ((packed)) fstype;
  __u16		boot_sign;
} __attribute__ ((packed));

#define fat32	fstype._fat32

struct msdos_dir_entry //目录项结构,32个字节
  {
    char	name[8], ext[3];	/* name and extension */
    __u8        attr;			/* attribute bits */
    __u8	lcase;			/* Case for base and extension */
    __u8	ctime_ms;		/* Creation time, milliseconds */
    __u16	ctime;			/* Creation time */
    __u16	cdate;			/* Creation date */
    __u16	adate;			/* Last access date */
    __u16	starthi;		/* high 16 bits of first cl. (FAT32) */
    __u16	time, date, start;	/* time, date and first cluster */
    __u32	size;			/* file size (in bytes) */
  } __attribute__ ((packed));

struct fat32_fsinfo //32个字节
{
  __u32		reserved1;	/* Nothing as far as I can tell */
  __u32		signature;	/* 0x61417272L */
  __u32		free_clusters;	/* Free cluster count.  -1 if unknown */
  __u32		next_cluster;	/* Most recently allocated cluster.
				 * Unused under Linux. */
  __u32		reserved2[4];
};

typedef struct 
{
	WORD wGenConfig;
	WORD wNumCyls;
	WORD wReserved;
	WORD wNumHeads;
	WORD wBytesPerTrack;
	WORD wBytesPerSector;
	WORD wSectorsPerTrack;
	WORD wVendorUnique[3];
	BYTE sSerialNumber[20];
	WORD wBufferType;
	WORD wBufferSize;
	WORD wECCSize;
	BYTE sFirmwareRev[8];
	BYTE sModelNumber[39];
	WORD wMoreVendorUnique;
	WORD wDoubleWordIO;
	WORD wCapabilities;
	WORD wReserved1;
	WORD wPIOTiming;
	WORD wDMATiming;
	WORD wBS;
	WORD wNumCurrentCyls;
	WORD wNumCurrentHeads;
	WORD wNumCurrentSectorsPerTrack;
	WORD ulCurrentSectorCapacity;
	WORD wMultSectorStuff;
	DWORD ulTotalAddressableSectors;
	WORD wSingleWordDMA;
	WORD wMultiWordDMA;
	BYTE bReserved[127];
}ST_IDSECTOR;




typedef struct _disk_init_info
{
	//下面的参数用于格式化和启动磁盘管理模块。
	BYTE dvrId[8];   //DVR机器的序列号：每一个DVR机器应该有一个唯一的标识，此处也借用
	                 //mac地址，因为每个机器的mac地址都不相同。此参数的目的是：判断硬盘
	                 //是否是本机器上的，格式化时，会把此参数写到硬盘里。
	BYTE productId;  //产品标识，是2408还2508等等。此参数的目的是：判断硬盘是否属于
	                 //本产品的，格式化时会把此参数写到硬盘里。
	char szDiskMountPath[512]; //设备挂载路径；
	
	//下面的参数用于启动磁盘管理模块
	BOOL bHaveEsata; //本机器是否带esata功能。
	BYTE nEsataInterface; //Esata所在的接口序号.该参数的取值范围为1 -- *
	BYTE nPciToEsataNum;   //pci转的esata个数
	BYTE pciToEsataIndex[8];//pci转的esata的编号。
	char szUsbToEsataPath[128];//usb转esata的路径


	//下面的参数用于格式化
	BYTE partitionNum; //硬盘格式化成几个分区；目前支持4个
	unsigned long fileSizeM;    //硬盘格式化后，创建的录像文件大小，单位是MB.目前支持下面的参数值
	                   //0-表示不创建文件.64-表示录像文件为64M。128-表示录像文件为128M。
	BYTE videoInputNum; 
	
}DISK_INIT_INFO;


#if defined(__VERY_BIG_DISK__)
//刚好512个字节
struct __gpt_header
{   
	ULONGLONG 	signature;   
	
  	__u32 		revision;   
	__u32 		headerSize;   
	__u32 		headerCRC;   
	__u32 		reserved;   
	
	ULONGLONG 	currentLBA;   
	ULONGLONG 	backupLBA;   
	ULONGLONG 	firstUsableLBA;   
	ULONGLONG 	lastUsableLBA;  
	
	uuid_t  	diskGUID;   
	
	ULONGLONG 	partitionEntriesLBA;   

	//下面几个值，是应该没有错误的。因为打印出的值，和理论一致。
	__u32 		numParts;   
	__u32 		sizeOfPartitionEntries;   
	__u32 		partitionEntriesCRC;   
  	__u8 		reserved2[420];
} __attribute__ ((packed));

//刚好128个字节，分区表项结构
struct __partition_table
{   
	uuid_t 		partitionType;
	uuid_t 		uniqueGUID;
	
	ULONGLONG 	firstLBA;
	ULONGLONG 	lastLBA;
	ULONGLONG 	attributes;

	__u8		partitonName[72];

} __attribute__ ((packed));

struct __mbr_partition
{
  	__u8	diskStatus;
  	__u8   	firstSector[3];
  	__u8   	partitionType;
  	__u8   	lastSector[3];
	__u32   firstLBA;
	__u32   lengthLBA;

} __attribute__ ((packed));

struct __disk_mbr 
{
  	__u8 	code[440];
	__u32 	diskSignature;
	__u16 	nulls;
	struct __mbr_partition partitions[4];
	__u16 	MBRSignature; 
	
} __attribute__ ((packed));
#endif

//
//typedef struct _disk_stats
//{
//	char diskname[NAME_LENGTH]; //磁盘名称:如/dev/sda
//	bool bIsUse;                //是否可用
//  	                        //true(1) -- 有效，false(0) -- 无效
//	ULONG disksize;          //磁盘大小（单位M）
//	BYTE partitionNum;      //分区数	
//}DISK_STATS;
//
//typedef CMyList <DISK_STATS> DISK_STATS_LIST;


//typedef unsigned long (*FORMATCALLBACK)(int percent,  void* pPara);

//////////////////////////////////////////////////////////////////////
////
////
////
////
//////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////





#pragma pack(push, 1)	
typedef	struct	_DRIVEATTRIBUTE {
	BYTE	bAttrID;		// Identifies which attribute
	WORD	wStatusFlags;	// see bit definitions below
	BYTE	bAttrValue;		// Current normalized value
	BYTE	bWorstValue;	// How bad has it ever been?
	BYTE	bRawValue[6];	// Un-normalized value
	BYTE	bReserved;		// ...
} DRIVEATTRIBUTE, *PDRIVEATTRIBUTE, *LPDRIVEATTRIBUTE;

//---------------------------------------------------------------------
// The following structure defines the structure of a Warranty Threshold
// Obsoleted in ATA4!
//---------------------------------------------------------------------
typedef	struct	_ATTRTHRESHOLD {
	BYTE	bAttrID;			// Identifies which attribute
	BYTE	bWarrantyThreshold;	// Triggering value
	BYTE	bReserved[10];		// ...
} ATTRTHRESHOLD, *PATTRTHRESHOLD, *LPATTRTHRESHOLD;
#pragma pack(pop)




#endif


