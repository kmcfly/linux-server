#ifndef _FAT32_FS_H
#define _FAT32_FS_H

#ifdef _WIN32
#include "cross_typedef.h"
#else
#include <sys/types.h>
#include <stdint.h>
#endif
/*
 * The MS-DOS filesystem constants/structures
 */

#define SECTOR_SIZE	512		/* sector size (bytes) */
#define SECTOR_BITS	9		/* log2(SECTOR_SIZE) */
#define MSDOS_DPB	(MSDOS_DPS)	/* dir entries per block */
#define MSDOS_DPB_BITS	4		/* log2(MSDOS_DPB) */
#define MSDOS_DPS	(SECTOR_SIZE / sizeof(struct msdos_dir_entry))
#define MSDOS_DPS_BITS	4		/* log2(MSDOS_DPS) */
#define MSDOS_LONGNAME	256		/* maximum name length */
#define BOOTCODE_FAT32_SIZE	420


#define MSDOS_ROOT_INO	 1	/* The root inode number */
#define MSDOS_FSINFO_INO 2	/* Used for managing the FSINFO block */

#define MSDOS_DIR_BITS	5	/* log2(sizeof(struct msdos_dir_entry)) */

/* directory limit */
#define FAT_MAX_DIR_ENTRIES	(65536)
#define FAT_MAX_DIR_SIZE	(FAT_MAX_DIR_ENTRIES << MSDOS_DIR_BITS)

#define ATTR_NONE	0	/* no attribute bits */
#define ATTR_RO		1	/* read-only */
#define ATTR_HIDDEN	2	/* hidden */
#define ATTR_SYS	4	/* system */
#define ATTR_VOLUME	8	/* volume label */
#define ATTR_DIR	16	/* directory */
#define ATTR_ARCH	32	/* archived */


/* attribute bits that are copied "as is" */
#define ATTR_UNUSED	(ATTR_VOLUME | ATTR_ARCH | ATTR_SYS | ATTR_HIDDEN)
/* bits that are used by the Windows 95/Windows NT extended FAT */
#define ATTR_EXT	(ATTR_RO | ATTR_HIDDEN | ATTR_SYS | ATTR_VOLUME)

#define CASE_LOWER_BASE	8	/* base is lower case */
#define CASE_LOWER_EXT	16	/* extension is lower case */

#define DELETED_FLAG	0xe5	/* marks file as deleted when in name[0] */
#define IS_FREE(n)	(!*(n) || *(n) == DELETED_FLAG)

#define FAT_LFN_LEN	255	/* maximum long name length */
#define MSDOS_NAME	11	/* maximum name length */
#define MSDOS_SLOTS	21	/* max # of slots for short and long names */
#define MSDOS_DOT	".          "	/* ".", padded to MSDOS_NAME chars */
#define MSDOS_DOTDOT	"..         "	/* "..", padded to MSDOS_NAME chars */

#define FAT_FIRST_ENT(s, x)	((MSDOS_SB(s)->fat_bits == 32 ? 0x0FFFFF00 : \
	MSDOS_SB(s)->fat_bits == 16 ? 0xFF00 : 0xF00) | (x))

/* start of data cluster's entry (number of reserved clusters) */
#define FAT_START_ENT	2

/* maximum number of clusters */
#define MAX_FAT12	0xFF4
#define MAX_FAT16	0xFFF4
#define MAX_FAT32	0x0FFFFFF6
#define MAX_FAT(s)	(MSDOS_SB(s)->fat_bits == 32 ? MAX_FAT32 : \
	MSDOS_SB(s)->fat_bits == 16 ? MAX_FAT16 : MAX_FAT12)

/* bad cluster mark */
#define BAD_FAT12	0xFF7
#define BAD_FAT16	0xFFF7
#define BAD_FAT32	0x0FFFFFF7

/* standard EOF */
#define EOF_FAT12	0xFFF
#define EOF_FAT16	0xFFFF
#define EOF_FAT32	0x0FFFFFFF

#define FAT_ENT_FREE	(0)
#define FAT_ENT_BAD	(BAD_FAT32)
#define FAT_ENT_EOF	(EOF_FAT32)

#define FAT_FSINFO_SIG1	0x41615252
#define FAT_FSINFO_SIG2	0x61417272
#define IS_FSINFO(x)	(le32_to_cpu((x)->signature1) == FAT_FSINFO_SIG1 \
			 && le32_to_cpu((x)->signature2) == FAT_FSINFO_SIG2)


struct msdos_volume_info {
	unsigned char		drive_number;	/* BIOS drive number */
	unsigned char		RESERVED;	/* Unused */
	unsigned char		ext_boot_sign;	/* 0x29 if fields below exist (DOS 3.3+) */
	unsigned char		volume_id[4];	/* Volume ID number */
	unsigned char		volume_label[11];/* Volume label */
	unsigned char		fs_type[8];	/* Typically FAT12 or FAT16 */
};

struct fat_boot_sector {
	unsigned char	ignored[3];	/* Boot strap short or near jump */
	unsigned char	system_id[8];	/* Name - can be used to special case
				   partition manager volumes */
	unsigned char	sector_size[2];	/* bytes per logical sector */
	unsigned char	sec_per_clus;	/* sectors/cluster */
	uint16_t	reserved;	/* reserved sectors */
	unsigned char	fats;		/* number of FATs */
	unsigned char	dir_entries[2];	/* root directory entries */
	unsigned char	sectors[2];	/* number of sectors */
	unsigned char	media;		/* media code */
	uint16_t	fat_length;	/* sectors/FAT */
	uint16_t	secs_track;	/* sectors per track */
	uint16_t	heads;		/* number of heads */
	uint32_t	hidden;		/* hidden sectors (unused) */
	uint32_t	total_sect;	/* number of sectors (if sectors == 0) */

	/* The following fields are only used by FAT32 */
	uint32_t	fat32_length;	/* sectors/FAT */
	uint16_t	flags;		/* bit 8: fat mirroring, low 4: active fat */
	unsigned char	version[2];	/* major, minor filesystem version */
	uint32_t	root_cluster;	/* first cluster in root directory */
	uint16_t	info_sector;	/* filesystem info sector */
	uint16_t	backup_boot;	/* backup boot sector */
	uint16_t	reserved2[6];	/* Unused */
	msdos_volume_info volume_info;
	unsigned char	boot_code[BOOTCODE_FAT32_SIZE];
	uint16_t	boot_sign;
};

struct fat_boot_fsinfo {
	uint32_t   signature1;	/* 0x41615252L */
	uint32_t   reserved1[120];	/* Nothing as far as I can tell 480 bytes*/
	uint32_t   signature2;	/* 0x61417272L */
	uint32_t   free_clusters;	/* Free cluster count.  -1 if unknown */
	uint32_t   next_cluster;	/* Most recently allocated cluster */
	uint32_t   reserved2[4];
};

struct msdos_dir_entry {
	unsigned char	name[MSDOS_NAME];/* name and extension */
	unsigned char	attr;		/* attribute bits */
	unsigned char    lcase;		/* Case for base and extension */
	unsigned char	ctime_cs;	/* Creation time, centiseconds (0-199) */
	uint16_t	ctime;		/* Creation time */
	uint16_t	cdate;		/* Creation date */
	uint16_t	adate;		/* Last access date */
	uint16_t	starthi;	/* High 16 bits of cluster in FAT32 */
	uint16_t	time,date,start;/* time, date and first cluster */
	uint32_t	size;		/* file size (in bytes) */
};

/* Up to 13 characters of the name */
struct msdos_dir_slot {
	unsigned char    id;		/* sequence number for slot */
	unsigned char    name0_4[10];	/* first 5 characters in name */
	unsigned char    attr;		/* attribute byte */
	unsigned char    reserved;	/* always 0 */
	unsigned char    alias_checksum;	/* checksum for 8.3 alias */
	unsigned char    name5_10[12];	/* 6 more characters in name */
	uint16_t   start;		/* starting cluster number, 0 in long slots */
	unsigned char    name11_12[4];	/* last 2 characters in name */
};


#endif /* _FAT32_FS_H */
