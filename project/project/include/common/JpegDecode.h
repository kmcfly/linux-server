/******************************************************************************

Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

******************************************************************************
File Name     :
Version       : Initial Draft
Author        : Hisilicon multimedia software group
Created       : 2005/08/25
Last Modified :
Description   :

Function List :

History       :
1.Date        : 2005/08/25
Author      : y43135
Modification: Created file

******************************************************************************/



//#include <common.h>
//#include <command.h>
#include <malloc.h>
#include <stdio.h>

//#include <devices.h>
//#include <version.h>
//#include <net.h>
#if !defined(__TDFH__) && !defined(__TDNXP__) && !defined(__CHIP3531__) && !defined(__CHIP3535__) && !defined(__CHIP3521__) && !defined(__CHIP3520A__) && !defined(__CHIPTI__) && !defined(__CHIP3520D__) && !defined(__CHIPGM__) 
#include <asm/io.h>
//#include <asm/arch/platform.h>
#include <asm/sizes.h>
#endif
//#include <config.h>
#include<string.h>

#define HI_OK 0
#define HI_ERROR 1
typedef unsigned int UINT32;
typedef unsigned char UINT8;
typedef unsigned short UINT16;

#define MAKEWORD(a, b)      ((UINT16)((unsigned char)(a)) | ((UINT16)((unsigned char)(b)) << 8))

#define FUNC_OK  0
#define FUNC_MEMORY_ERROR  1
#define FUNC_FILE_ERROR   2
#define FUNC_FORMAT_ERROR  3

#define M_SOF0  0xc0
#define M_DHT   0xc4
#define M_EOI   0xd9
#define M_SOS   0xda
#define M_DQT   0xdb
#define M_DRI   0xdd
#define M_APP0  0xe0

#define W1  2841   /* 2048*sqrt(2)*cos(1*pi/16) */
#define W2  2676   /* 2048*sqrt(2)*cos(2*pi/16) */
#define W3  2408   /* 2048*sqrt(2)*cos(3*pi/16) */
#define W5  1609   /* 2048*sqrt(2)*cos(5*pi/16) */
#define W6  1108   /* 2048*sqrt(2)*cos(6*pi/16) */
#define W7  565    /* 2048*sqrt(2)*cos(7*pi/16) */

const int Zig_Zag[8][8]=
{
	{0,1,5,6,14,15,27,28},
	{2,4,7,13,16,26,29,42},
	{3,8,12,17,25,30,41,43},
	{9,11,18,24,31,40,44,53},
	{10,19,23,32,39,45,52,54},
	{20,22,33,38,46,51,55,60},
	{21,34,37,47,50,56,59,61},
	{35,36,48,49,57,58,62,63}
};

class CJpegDecode
{
public:
	CJpegDecode();
	~CJpegDecode();

	UINT32 LoadJpegFile( UINT8 *pImg, unsigned char *pJpegBufSrc, int &retWidth, int &retHeight, int &retLen);

private:

	int InitTag(void);
	void InitTable(void);
	int Decode(void);
	int DecodeMCUBlock(void);
	int HufBlock(UINT8 dchufindex,UINT8 achufindex);
	int DecodeElement(void);
	void IQtIZzMCUComponent(int flag);
	void IQtIZzBlock(int *s,int *d,int flag);
	void GetYUV(int flag);
	void StoreBuffer(void);
	UINT8 ReadByte(void);
	void Initialize_Fast_IDCT(void);
	void Fast_IDCT(int * block);
	void idctrow(int * blk);
	void idctcol(int * blk);


	UINT32 ImgWidth ,ImgHeight;

	int SampRate_Y_H,SampRate_Y_V,SampRate_U_H,SampRate_U_V,SampRate_V_H,SampRate_V_V;
	int H_YtoU,V_YtoU,H_YtoV,V_YtoV,Y_in_MCU,U_in_MCU,V_in_MCU;
	//UINT8 *lpJpegBuf=0x35000000,*lp,*lpPtr;
	UINT8 *lpJpegBuf,*lp,*lpPtr;
	int qt_table[3][64];
	int comp_num;
	UINT8 comp_index[3];
	UINT8 YDcIndex,YAcIndex,UVDcIndex,UVAcIndex,HufTabIndex;
	int *YQtTable,*UQtTable,*VQtTable;
	UINT8 MyAnd[9];//={0,1,3,7,0x0f,0x1f,0x3f,0x7f,0xff};
	int code_pos_table[4][16],code_len_table[4][16];
	UINT16 code_value_table[4][256];
	UINT16 huf_max_value[4][16],huf_min_value[4][16];
	int BitPos,CurByte,rrun,vvalue;
	int MCUBuffer[640],BlockBuffer[64];
	int QtZzMCUBuffer[640];
	int ycoef,ucoef,vcoef,IntervalFlag,interval;
	int Y[256],U[256],V[256];
	int sizei,sizej;
	int restart;
	int iclip[1024];
	int LineBytes;

};






