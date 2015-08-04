#ifndef _TRANSFER_SIZE_H
#define _TRANSFER_SIZE_H

#define SIZE_1KB			0x400
#define SIZE_2KB			0x800
#define SIZE_4KB			0x1000
#define SIZE_8KB			0x2000
#define SIZE_16KB			0x4000
#define SIZE_32KB			0x8000      //32768
#define SIZE_64KB			0x10000    //65536
#define SIZE_128KB			0x20000 
#define SIZE_256KB			0x40000
#define SIZE_512KB			0x80000 

#define SIZE_1MB			0x100000
#define SIZE_2MB			0x200000
#define SIZE_2d5MB			0x280000
#define SIZE_3MB			0x300000
#define SIZE_3d5MB			0x380000
#define SIZE_4MB			0x400000
#define SIZE_4d5MB			0x480000
#define SIZE_8MB			0x800000
#define SIZE_16MB			0x1000000
#define SIZE_32MB			0x2000000

#define SIZE_DVD_BLOCK		2048
#define SIZE_HDD_BLOCK1		512
#define SIZE_HDD_BLOCK2		4096  

#define SIZE_GB(x)			x<<30 
#define SIZE_MB(x)			x<<20 
#define SIZE_KB(x)			x<<10 

#define BYTE_TO_GB(x)		x>>30
#define BYTE_TO_MB(x)		x>>20 
#define BYTE_TO_KB(x)       x>>10 


#define	SIZE_ROUND_UP(X, BLK_SZIE)	 (((X) + (BLK_SZIE - 1)) & ~(BLK_SZIE - 1))
#define	SIZE_ROUND_ALIGN(X, BLK_SZIE) (((X)) & ~(BLK_SZIE - 1))					//对齐到512字节，多出部分截取
#define	BLK_COUNT(X,BLK_SZIE)	(((X) / BLK_SZIE) + (((X)%BLK_SZIE)?1:0))

#endif	//_TRANSFER_SIZE_H