

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
#include "JpegDecode.h"

CJpegDecode::CJpegDecode()
{
	ImgWidth = 0 ;
	ImgHeight = 0;

	SampRate_Y_H = 0;
	SampRate_Y_V = 0;
	SampRate_U_H = 0;
	SampRate_U_V = 0;
	SampRate_V_H = 0;
	SampRate_V_V = 0;
	
	H_YtoU = 0;
	V_YtoU = 0;
	H_YtoV = 0;
	V_YtoV = 0;
	Y_in_MCU = 0;
	U_in_MCU = 0;
	V_in_MCU = 0;
    
	lpJpegBuf = NULL;
	lp = NULL;
	lpPtr = NULL;
	
	memset(qt_table, 0, sizeof(qt_table));
	comp_num = 0;
	memset(comp_index, 0, sizeof(comp_index));
	YDcIndex = 0;
	YAcIndex = 0;
	UVDcIndex = 0;
	UVAcIndex = 0;
	HufTabIndex = 0;
	YQtTable = NULL;
	UQtTable = NULL;
	VQtTable = NULL;
	MyAnd[0] = 0;
	MyAnd[1] = 1;
	MyAnd[2] = 3;
	MyAnd[3] = 7;
	MyAnd[4] = 0x0f;
	MyAnd[5] = 0x1f;
	MyAnd[6] = 0x3f;
	MyAnd[7] = 0x7f;
	MyAnd[8] = 0xff;


	memset(code_pos_table, 0, sizeof(code_pos_table));
	memset(code_len_table, 0, sizeof(code_len_table));
	memset(code_value_table, 0, sizeof(code_value_table));
	memset(huf_max_value, 0, sizeof(huf_max_value));
	memset(huf_min_value, 0, sizeof(huf_min_value));
	
	BitPos = 0;
	CurByte = 0;
	rrun = 0;
	vvalue = 0;
	memset(MCUBuffer, 0, sizeof(MCUBuffer));
	memset(BlockBuffer, 0, sizeof(BlockBuffer));
	memset(QtZzMCUBuffer, 0, sizeof(QtZzMCUBuffer));
	
	
	ycoef = 0;
	ucoef = 0;
	vcoef = 0;
	IntervalFlag = 0;
	interval = 0;
	
	memset(Y, 0, sizeof(Y));
	memset(U, 0, sizeof(U));
	memset(V, 0, sizeof(V));
	sizei = 0; 
	sizej = 0;
	restart = 0;
	memset(iclip, 0, sizeof(iclip));
	LineBytes = 0;
}

CJpegDecode::~CJpegDecode()
{

}



UINT32 CJpegDecode::LoadJpegFile( UINT8 *pImg, unsigned char *pJpegBufSrc, int &retWidth, int &retHeight, int &retLen)
{
	UINT32 ImgSize;
	int funcret;
	lpJpegBuf = pJpegBufSrc;

	if (*(volatile unsigned char *)lpJpegBuf != 0xFF || *(volatile unsigned char *)(lpJpegBuf+1) != 0xD8)
	{
		printf("addr:%#x,:%2x,%2x\n",lpJpegBuf,*(volatile unsigned char *)lpJpegBuf,*(volatile unsigned char *)(lpJpegBuf+1));
		return HI_ERROR;
	}


	InitTable();

	funcret = InitTag();
	if(funcret != FUNC_OK)    /* ��Jpeg�ļ������ڴ� */
	{
		printf("InitTag error\n");
		return HI_ERROR;
	}

#if 0
	LineBytes = ((ImgWidth * 16 + 31) >> 5) * 4;
#else
	if (1)
	{
		/* modified by x100808
		* hi35xx 16 bytes align, 8 pixel, stride use byte
		*/
		int mod = ImgWidth%8;
		if (mod != 0)
		{
			LineBytes = (ImgWidth + (8-mod)) * 2;
		}
		else
		{
			LineBytes = ImgWidth * 2;
		}
	}
#endif

	ImgSize = LineBytes * ImgHeight;       /* �仯���ͼ��ռ� */
	lpPtr = pImg;    /* ����任��Ĵ洢�ռ� */

	if((SampRate_Y_H == 0) || (SampRate_Y_V == 0))
	{
		return HI_ERROR;
	}

	printf("<<imgwidth=%d, imgheight=%d, linebytes=%d>>\n", ImgWidth, ImgHeight, LineBytes);

    retWidth = ImgWidth;
	retHeight =  ImgHeight;
	retLen = ImgSize;

	funcret = Decode();
	if(funcret == FUNC_OK)      /* ����ɹ� */
	{
		printf("decode success!!!!\n");
		return HI_OK;
	}
	else
	{
		printf("JPEG Decode ERROR.\n");
		return HI_ERROR;
	}
}

int CJpegDecode::InitTag(void)
{
	UINT32 finish;
	UINT8 id;
	int llength,i,j,k,huftab1,huftab2,huftabindex,ccount;
	UINT8 hf_table_index,qt_table_index,comnum;
	UINT8 *lptemp;
    int  temp2;
	finish = HI_ERROR;
	lp = lpJpegBuf + 2;   /* ���������ֽ�SOI(0xFF��0xD8 Start of Image) */
	while(finish != HI_OK)
	{
		id = *(lp + 1);   /* ȡ����λ�ֽ�(��λ��ǰ����λ�ں�) */
		lp += 2;       /* ����ȡ�����ֽ� */
		switch(id)
		{
		case M_APP0:
			llength = MAKEWORD(*(lp + 1),*lp);  /* MAKEWORD ת��Motorlora �� intel ���ݸ�ʽ */
			lp += llength;  /* Skip JFIF segment marker */
			break;
		case M_DQT:
                temp2 = 2;
			llength = MAKEWORD(*(lp + 1),*lp); /* (����������) */
			qt_table_index = *(lp + 2) & 0x0f;/* ��������Ϣbit 0..3: QT ��(0..3, �������) */
			/* bit 4..7: QT ����, 0 = 8 bit, ���� 16 bit */
			lptemp = lp + 3;       /* n �ֽڵ� QT, n = 64*(����+1) */
			if(llength < 80)
			{
				for(i = 0;i < 64;i++)
				{
					qt_table[qt_table_index][i] = (int)(*(lptemp++));
				}
			}
			else
			{
                    #if 1
                    while(temp2 < llength)
                    {
                 //       printf("qt_table_index  %d\n",qt_table_index);
                        for(i = 0;i < 64;i++)
                        {
                            qt_table[qt_table_index][i] = (int)(*(lptemp++));
                        }
                        qt_table_index = (*(lptemp++)) & 0x0f;
                        if(qt_table_index > 3)
                        {
                  //          printf("qt_table_index err %d\n",qt_table_index);
                            break;
                        }
                        temp2 += 65;
                    }
                    #else
				for(i = 0;i < 64;i++)
				{
					qt_table[qt_table_index][i] = (int)(*(lptemp++));
				}
				qt_table_index = (*(lptemp++)) & 0x0f;
				for(i = 0;i < 64;i++)
				{
					qt_table[qt_table_index][i] = (int)(*(lptemp++));
				}
                    #endif
			}
			lp += llength; /* ���������� */
			break;
		case M_SOF0:      /* ֡��ʼ (baseline JPEG 0xFF,0xC0) */
			llength = MAKEWORD(*(lp + 1),*lp);         /* ���� (���ֽ�, ���ֽ�), 8+components*3 */
			ImgHeight = MAKEWORD(*(lp + 4),*(lp + 3));/* ͼƬ�߶� (���ֽ�, ���ֽ�), �����֧�� DNL �ͱ��� >0 */
			ImgWidth = MAKEWORD(*(lp + 6),*(lp + 5)); /* ͼƬ���� (���ֽ�, ���ֽ�), �����֧�� DNL �ͱ��� >0 */
			comp_num = *(lp + 7);                   /* components ����(1 byte), �Ҷ�ͼ�� 1, YCbCr/YIQ ��ɫͼ�� 3, CMYK ��ɫͼ�� 4 */
			if((comp_num != 1) && (comp_num != 3))
			{
				return FUNC_FORMAT_ERROR;
			}
			if(comp_num == 3)
			{
				comp_index[0] = *(lp + 8);          /* component id (1 = Y, 2 = Cb, 3 = Cr, 4 = I, 5 = Q) */
				SampRate_Y_H = (*(lp + 9)) >> 4;      /* ˮƽ����ϵ�� */
				SampRate_Y_V = (*(lp + 9)) & 0x0f;    /* ˮƽ����ϵ�� */
				YQtTable = (int *)qt_table[*(lp + 10)]; /* ͨ����������ȡ����������ַ */
				/*  ShowMessage(IntToStr(YQtTable^)); */

				comp_index[1] = *(lp  + 11);         /* component id */
				SampRate_U_H = (*(lp + 12)) >> 4;     /* ˮƽ����ϵ�� */
				SampRate_U_V = (*(lp + 12)) & 0x0f;   /* ˮƽ����ϵ�� */
				UQtTable = (int *)qt_table[*(lp + 13)];  /* ͨ����������ȡ����������ַ */

				comp_index[2] = *(lp + 14);         /* component id */
				SampRate_V_H = (*(lp  + 15)) >> 4;     /* ˮƽ����ϵ�� */
				SampRate_V_V = (*(lp + 15)) & 0x0f;   /* ˮƽ����ϵ�� */
				VQtTable = (int *)qt_table[*(lp + 16)];   /* ͨ����������ȡ����������ַ	*/
			}
			else
			{
				comp_index[0] = *(lp + 8);
				SampRate_Y_H = (*(lp + 9)) >> 4;
				SampRate_Y_V = (*(lp + 9)) & 0x0f;
				YQtTable = (int *)qt_table[*(lp + 10)];  /* �Ҷ�ͼ����������һ�� */

				comp_index[1] = *(lp + 8);
				SampRate_U_H = 1;
				SampRate_U_V = 1;
				UQtTable = (int *)qt_table[*(lp + 10)];

				comp_index[2] = *(lp + 8);
				SampRate_V_H = 1;
				SampRate_V_V = 1;
				VQtTable = (int *)qt_table[*(lp + 10)];
			}
			lp += llength;
			break;
		case M_DHT:  /* ���� Huffman Table(0xFF,0xC4) */
			llength = MAKEWORD(*(lp + 1),*lp);       /* ���� (���ֽ�, ���ֽ�) */
			if (llength < 0xd0)                /* Huffman Table��Ϣ (1 byte) */
			{
				huftab1 = (int)(*(lp + 2)) >> 4;     /* huftab1=0,1(HT ����,0 = DC 1 = AC) */
				huftab2 = (int)(*(lp + 2)) & 0x0f;   /* huftab2=0,1(HT ��  ,0 = Y  1 = UV) */
				huftabindex = huftab1 * 2 + huftab2;           /* 0 = YDC 1 = UVDC 2 = YAC 3 = UVAC */
				lptemp = lp + 3;
				for(i = 0;i < 16;i++)                     /* 16 bytes: ������ 1..16 ����ķ����� */
				{
					code_len_table[huftabindex][i] = (int)(*(lptemp++));/* �볤Ϊ */
					/* i�����ָ��� */
				}
				j = 0;
				for(i = 0;i < 16;i++)             /* �ó�HT���������ֵĶ�Ӧֵ */
				{
					if(code_len_table[huftabindex][i] != 0)
					{
						k = 0;
						while(k < code_len_table[huftabindex][i])
						{
							code_value_table[huftabindex][k + j] = (int)(*(lptemp++));
							k++;
						}
						j += k;
					}
				}
				i = 0;
				while(code_len_table[huftabindex][i] == 0)  /* ȥ������ķ�����Ϊ�� */
				{
					i++;
				}
				for(j = 0;j < i;j++)
				{
					huf_min_value[huftabindex][j] = 0;            /* �볤Ϊj����С���� */
					huf_max_value[huftabindex][j] = 0;            /* �볤Ϊj��������� */
				}
				huf_min_value[huftabindex][i] = 0;
				huf_max_value[huftabindex][i] = code_len_table[huftabindex][i] - 1;
				for(j = i + 1;j < 16;j++)
				{                                              /* �볤Ϊj����С������������� */
					huf_min_value[huftabindex][j] = (huf_max_value[huftabindex][j - 1] + 1) << 1;
					huf_max_value[huftabindex][j] = huf_min_value[huftabindex][j] + code_len_table[huftabindex][j] - 1;
				}
				code_pos_table[huftabindex][0] = 0; /* ����ʼλ�� */
				for(j = 1;j < 16;j++)
				{
					code_pos_table[huftabindex][j] = code_len_table[huftabindex][j - 1] + code_pos_table[huftabindex][j - 1];
				}
				lp += llength;
			}
			else
			{
				hf_table_index = *(lp + 2);
				lp += 2;
				while(hf_table_index != 0xff)
				{
					huftab1 = (int)hf_table_index >> 4;     /* huftab1=0,1 */
					huftab2 = (int)hf_table_index & 0x0f;   /* huftab2=0,1 */
					huftabindex = huftab1 * 2 + huftab2;
					lptemp = lp;
					lptemp++;
					ccount = 0;
					for(i = 0;i < 16;i++)
					{
						code_len_table[huftabindex][i] = (int)(*(lptemp++));
						ccount = ccount + code_len_table[huftabindex][i];
					}
					ccount = ccount + 17;
					j = 0;
					for(i = 0;i < 16;i++)
					{
						if(code_len_table[huftabindex][i] != 0)
						{
							k = 0;
							while(k < code_len_table[huftabindex][i])
							{
								code_value_table[huftabindex][k + j] = (int)(*(lptemp++));
								k++;
							}
							j = j + k;
						}
					}
					i = 0;
					while(code_len_table[huftabindex][i] == 0)
					{
						i++;
					}
					for(j = 0;j < i;j++)
					{
						huf_min_value[huftabindex][j] = 0;
						huf_max_value[huftabindex][j] = 0;
					}
					huf_min_value[huftabindex][i] = 0;
					huf_max_value[huftabindex][i] = code_len_table[huftabindex][i] - 1;
					for(j = i + 1;j < 16;j++)
					{
						huf_min_value[huftabindex][j] = (huf_max_value[huftabindex][j - 1] + 1) << 1;
						huf_max_value[huftabindex][j] = huf_min_value[huftabindex][j] + code_len_table[huftabindex][j] - 1;
					}
					code_pos_table[huftabindex][0] = 0;
					for(j = 1;j < 16;j++)
					{
						code_pos_table[huftabindex][j] = code_len_table[huftabindex][j - 1] + code_pos_table[huftabindex][j - 1];
					}
					lp += ccount;
					hf_table_index = *lp;
				}
			}
			break;
		case M_DRI:
			llength = MAKEWORD(*(lp + 1),*lp);
			restart = MAKEWORD(*(lp + 3),*(lp + 2));
			lp += llength;
			break;
		case M_SOS:    /* ɨ���п�ʼ0xff, 0xda (SOS) */
			llength = MAKEWORD(*(lp + 1),*lp);/* ���� (���ֽ�, ���ֽ�), */
			comnum = *(lp + 2);               /* ɨ��������������� (1 byte), ���� >:= 1 , <:=4 (�����Ǵ���) ͨ���� 3 */
			if(comnum != comp_num)       /* ������ 6+2*(ɨ���������������) */
			{
				return FUNC_FORMAT_ERROR;
			}
			lptemp = lp + 3;
			for(i = 0;i < comp_num;i++)
			{                  /* ÿ�������Ϣ */
				if(*lptemp == comp_index[0])
				{   /* 1 byte :Component id */
					YDcIndex = (*(lptemp + 1)) >> 4;   /* Y ʹ�õ� Huffman �� */
					YAcIndex = ((*(lptemp + 1)) & 0x0f) + 2;
				}
				else
				{
					UVDcIndex = (*(lptemp + 1)) >> 4;   /* U,V */
					UVAcIndex = ((*(lptemp + 1)) & 0x0f) + 2;
				}
				lptemp += 2;
			}
			lp += llength;
			finish = HI_OK;
			break;
		case M_EOI:
			return FUNC_FORMAT_ERROR;
			break;
		default:
			if((id & 0xf0) != 0xd0)
			{
				llength = MAKEWORD(*(lp + 1),*lp);
				lp += llength;
			}
			else
			{
				lp += 2;
			}
			break;
		}
	}
	return FUNC_OK;
}

void CJpegDecode::InitTable(void)
{
	int i,j;
	sizei = 0;
	sizej = 0;
	ImgWidth = 0;
	ImgHeight = 0;
	rrun = 0;
	vvalue = 0;
	BitPos = 0;
	CurByte = 0;
	IntervalFlag = HI_ERROR;
	restart = 0;
	for(i = 0;i < 3;i++)
	{
		for(j = 0;j < 64;j++)
		{
			qt_table[i][j] = 0;           /* ������ */
		}
	}
	comp_num = 0;
	HufTabIndex = 0;
	for(i = 0;i < 3;i++)
	{
		comp_index[i] = 0;
	}
	for(i = 0;i < 4;i++)
	{
		for(j = 0;j < 16;j++)
		{
			code_len_table[i][j] = 0;
			code_pos_table[i][j] = 0;
			huf_max_value[i][j] = 0;
			huf_min_value[i][j] = 0;
		}
	}
	for(i = 0;i < 4;i++)
	{
		for(j = 0;j < 256;j++)
		{
			code_value_table[i][j] = 0;
		}
	}
	for(i = 0;i< 640;i++)
	{
		MCUBuffer[i] = 0;
		QtZzMCUBuffer[i] = 0;
	}
	for(i = 0;i < 64;i++)
	{
		Y[i] = 0;
		U[i] = 0;
		V[i] = 0;
		BlockBuffer[i] = 0;
	}
	ycoef = 0;
	ucoef = 0;
	vcoef = 0;
}

/*************************************************************************
����˳��: Initialize_Fast_IDCT() :��ʼ��
DecodeMCUBlock()       Huffman Decode
IQtIZzMCUComponent()   ����������DCT
GetYUV()               Get Y U V
StoreBuffer()          YUV to RGB
*************************************************************************/
int CJpegDecode::Decode(void)
{
	int funcret;

	Y_in_MCU = SampRate_Y_H * SampRate_Y_V;     /* YDU YDU YDU YDU */
	U_in_MCU = SampRate_U_H * SampRate_U_V;     /* cRDU */
	V_in_MCU = SampRate_V_H * SampRate_V_V;     /* cBDU */
	H_YtoU = SampRate_Y_H / SampRate_U_H;
	V_YtoU = SampRate_Y_V / SampRate_U_V;
	H_YtoV = SampRate_Y_H / SampRate_V_H;
	V_YtoV = SampRate_Y_V / SampRate_V_V;
	Initialize_Fast_IDCT();
	funcret = DecodeMCUBlock();
	//printf("funcret_decodemcublock = %x\n",funcret);
	while(funcret == FUNC_OK)
	{                                          /* After Call DecodeMCUBUBlock() */

		interval++;                             /* The Digital has been Huffman Decoded and */
		if((restart != 0) && ((interval % restart) == 0))   /* be stored in MCUBuffer(YDU,YDU,YDU,YDU */
		{
			IntervalFlag = HI_OK;                          /* UDU,VDU) Every DU := 8*8 */
		}
		else
		{
			IntervalFlag = HI_ERROR;
		}
		IQtIZzMCUComponent(0);   /* ������ and IDCT The Data in QtZzMCUBuffer */
		IQtIZzMCUComponent(1);
		IQtIZzMCUComponent(2);
		GetYUV(0);                    /* �õ�Y cR cB */
		GetYUV(1);
		GetYUV(2);

		StoreBuffer();               /* To RGB */

		sizej = sizej + (UINT32)(SampRate_Y_H << 3);
		if(sizej >= ImgWidth)
		{
			sizej = 0;
			sizei = sizei + (UINT32)(SampRate_Y_V << 3);
		}
		if ((sizej == 0) && (sizei >= ImgHeight))
		{
			break;
		}

		funcret = DecodeMCUBlock();

	}

	return funcret;
}

/**********************************************************************
��� QtZzMCUBuffer ���� Y[] U[] V[]
**********************************************************************/
void CJpegDecode::GetYUV(int flag)
{
	int H,VV,i,j,k,hk;
	int temp;
	int *buf,*tempbuf,*pQtZzMCU;
	switch(flag)
	{
	case 0:                             /* ���ȷ��� */
		H = SampRate_Y_H;
		VV = SampRate_Y_V;
		buf = Y;
		pQtZzMCU = QtZzMCUBuffer;
		break;
	case 1:                             /* ��ɫ���� */
		H = SampRate_U_H;
		VV = SampRate_U_V;
		buf = U;
		pQtZzMCU = QtZzMCUBuffer;
		pQtZzMCU = pQtZzMCU + (Y_in_MCU << 6);
		break;
	case 2:                            /* ��ɫ���� */
		H = SampRate_V_H;
		VV = SampRate_V_V;
		buf = V;
		pQtZzMCU = QtZzMCUBuffer;
		pQtZzMCU = pQtZzMCU + ((Y_in_MCU + U_in_MCU) << 6);
		break;
	default:
		H = 0;
		VV = 0;
		buf = NULL;
		pQtZzMCU = NULL;
		break;
	}
	for(i = 0;i < VV;i++)
	{
		for(j = 0;j < H;j++)
		{
			for(k = 0;k < 8;k++)
			{
				for(hk = 0;hk < 8;hk++)
				{
					temp = ((((i << 3) + k) * SampRate_Y_H) << 3) + (j << 3) + hk;
					tempbuf = buf;
					tempbuf = tempbuf + temp;
					*tempbuf = (int)(*(pQtZzMCU++));

				}
			}
		}
	}
}

/********************************************************************************
��������ְ�RGB��ʽ�洢 lpbmp (BGR),(BGR) ......���Y[] U[] V[] ����lpPtr
*********************************************************************************/
void CJpegDecode::StoreBuffer(void)
{
	int i,j;
	UINT8 *lprgb;
	UINT8 R,G,B;
	int yy,uu,vv,rr,gg,bb;
	int TempSamp1,TempSamp2;
	TempSamp1 = SampRate_Y_V << 3;
	TempSamp2 = SampRate_Y_H << 3;
	int *pY;
	int *pU;
	int *pV;

	for(i = 0;i < TempSamp1;i++)
	{                                  /* sizei��ʾ�� sizej ��ʾ�� */
		if((sizei + i) < ImgHeight)
		{
			/*lpbmp = lpPtr + (UINT32)((ImgHeight - sizei - (UINT32)(i) - 1) * LineBytes + sizej * 3);*/
			lprgb = lpPtr + ( (sizei + i) * LineBytes + (sizej << 1) );

			pY=Y+i*TempSamp2;// (i << 3) * SampRate_Y_H;
			pU=U+(i / V_YtoU)*TempSamp2;// << 3) * SampRate_Y_H;
			pV=V+(i / V_YtoV)*TempSamp2;// << 3) * SampRate_Y_H;

			for(j = 0;j < TempSamp2;j++)
			{
				if((sizej + j) < ImgWidth)
				{
					yy = *pY++;

					uu = pU[ j/H_YtoU ];  /* �ڲ� */

					vv = pV[ j/H_YtoV];

					rr = ((yy << 8) + 18 * uu + 367 * vv) >> 8;
					gg = ((yy << 8) - 159 * uu - 220 * vv) >> 8;
					bb = ((yy << 8) + 411 * uu - 29 * vv) >> 8;
					R = (UINT8)(rr);
					G = (UINT8)(gg);
					B = (UINT8)(bb);
					if((rr && 0xffffff00) != 0)
					{
						if (rr > 255)
						{
							R = 255;
						}
						else
						{
							if(rr < 0)
							{
								R = 0;
							}
						}
					}

					if((gg && 0xffffff00) != 0)
					{
						if(gg > 255)
						{
							G = 255;
						}
						else
						{
							if(gg < 0)
							{
								G = 0;
							}
						}
					}

					if((bb && 0xffffff00) != 0)
					{
						if(bb > 255)
						{
							B = 255;
						}
						else
						{
							if(bb < 0)
							{
								B = 0;
							}
						}
					}

					*lprgb++ = (UINT8)(B >> 3) + (UINT8)((G << 2) & 0xe0);
					*lprgb++ = (UINT8)(G >> 6) + (UINT8)((R >> 1) & 0x7c);
				}
				else
				{
					break;
				}
			}
		}
		else
		{
			break;
		}
	}
}

/******************************************************************************
Huffman Decode   MCU ���� MCUBuffer  ���Blockbuffer[  ]
*******************************************************************************/
int CJpegDecode::DecodeMCUBlock(void)
{
	int *lpMCUBuffer;
	int i,j;
	int funcret=0;
	int tempX;
	if(IntervalFlag == HI_OK)
	{
		lp += 2;
		ycoef = 0;   /* ��ֵ��λ */
		ucoef = 0;
		vcoef = 0;
		BitPos = 0;
		CurByte = 0;
	}
	//printf("comp_num = %x\n", comp_num);
	switch(comp_num)
	{                             /* comp_num ָͼ�����ͣ���ɫͼ���Ҷ�ͼ��*/

	case 3:   /* ��ɫͼ */
		lpMCUBuffer = MCUBuffer;
		tempX = SampRate_Y_H * SampRate_Y_V;
		for(i = 0;i < tempX;i++)   /* Y */
		{
			funcret = HufBlock(YDcIndex,YAcIndex);   /* ����4 * (8*8) */
			if(funcret != FUNC_OK)
			{
				printf("funcret1 = %x\n", funcret);
				return funcret;
			}
			BlockBuffer[0] = BlockBuffer[0] + ycoef;   /* ֱ�������ǲ�ֵ������Ҫ�ۼӡ�*/
			ycoef = BlockBuffer[0];
			for(j = 0;j < 64;j++)
			{
				*lpMCUBuffer++ = BlockBuffer[j];
			}
		}
		tempX = SampRate_U_H * SampRate_U_V;
		for(i = 0;i < tempX;i++)    /* U */
		{
			funcret = HufBlock(UVDcIndex,UVAcIndex);
			if(funcret != FUNC_OK)
			{
				printf("funcret2 = %x\n", funcret);
				return funcret;
			}
			BlockBuffer[0] = BlockBuffer[0] + ucoef;
			ucoef = BlockBuffer[0];
			for(j = 0;j < 64;j++)
			{
				*lpMCUBuffer++ = BlockBuffer[j];
			}
		}
		tempX = SampRate_V_H * SampRate_V_V;
		for(i = 0;i < tempX;i++)  /* V */
		{
			funcret = HufBlock(UVDcIndex,UVAcIndex);
			if(funcret != FUNC_OK)
			{

				printf("funcret3 = %x\n", funcret);
				return funcret;
			}
			BlockBuffer[0] = BlockBuffer[0] + vcoef;
			vcoef = BlockBuffer[0];
			for(j = 0;j < 64;j++)
			{
				*lpMCUBuffer++ = BlockBuffer[j];
			}
		}
		break;
	case 1:   /* Gray Picture */
		lpMCUBuffer = MCUBuffer;
		funcret = HufBlock(YDcIndex,YAcIndex);
		if(funcret != FUNC_OK)
		{
			return funcret;
		}
		BlockBuffer[0] = BlockBuffer[0] + ycoef;
		ycoef = BlockBuffer[0];
		for(j = 0;j < 64;j++)
		{
			*lpMCUBuffer++ = BlockBuffer[j];
		}
		for(i = 0;i < 128;i++)
		{
			*lpMCUBuffer++ = 0;
		}
		break;
	default:
		// printf("comp_num1 = %x\n", comp_num);
		return FUNC_FORMAT_ERROR;
	}
	return FUNC_OK;
}

/*****************************************************************
Huffman Decode ��8*8�� DU   ���� Blockbuffer[ ] ��� vvalue
******************************************************************/
int CJpegDecode::HufBlock(UINT8 dchufindex,UINT8 achufindex)
{
	int count,i;
	int funcret;

	count = 0;
	/* dc */
	HufTabIndex = dchufindex;
	funcret = DecodeElement();  /* Read Byte Dc */
	if(funcret != FUNC_OK)
	{
		return funcret;
	}
	BlockBuffer[count++] = vvalue;/* �����ֱ��ϵ�� */

	/* ac */
	HufTabIndex = achufindex;
	while(count < 64)
	{         /* 63 Bytes AC */
		funcret = DecodeElement();
		if(funcret != FUNC_OK)
		{
			return funcret;
		}
		if((rrun == 0) && (vvalue == 0))
		{
			for(i = count;i < 64;i++)
			{
				BlockBuffer[i] = 0;
			}
			count = 64;
		}
		else
		{
			for(i = 0;i < rrun;i++)    /* ǰ����� */
			{
				BlockBuffer[count++] = 0;
			}
			BlockBuffer[count++] = vvalue;/* �����ֵ */
		}
	}
	return FUNC_OK;
}

/**************************************************************************
Huffman ����  ÿ��Ԫ��   ���� vvalue ��� ���ļ�ReadByte
***************************************************************************/
int CJpegDecode::DecodeElement(void)
{
	int thiscode,tempcode;
	UINT16 temp,valueex;
	int codelen;
	UINT8 hufexbyte,runsize,tempsize,sign;
	UINT8 newbyte,lastbyte;

	if(BitPos >= 1)             /* BitPosָʾ��ǰ����λ�� */
	{
		BitPos--;
		thiscode = (UINT8)CurByte >> BitPos; /* ȡһ������ */
		CurByte = CurByte & MyAnd[BitPos]; /* ���ȡ�ߵı���λ */
	}
	else                                        /* ȡ����һ���ֽ������� */
	{                                          /* ��ȡ */
		lastbyte = ReadByte(); /* ����һ���ֽ� */
		BitPos--;                     /* and[]:=0x0,0x1,0x3,0x7,0xf,0x1f,0x2f,0x3f,0x4f */
		newbyte = CurByte & MyAnd[BitPos];  //
		thiscode = lastbyte >> 7;
		CurByte = newbyte;
	}
	codelen = 1;
	/* ��Huffman���е�����ƥ�䣬ֱ���ҵ�Ϊֹ */
	while((thiscode < huf_min_value[HufTabIndex][codelen - 1]) ||
		(code_len_table[HufTabIndex][codelen - 1] == 0) ||
		(thiscode > huf_max_value[HufTabIndex][codelen - 1]))
	{
		if(BitPos >= 1)       /* ȡ����һ���ֽڻ��� */
		{
			BitPos--;
			tempcode = (UINT8)CurByte >> BitPos;
			CurByte = CurByte & MyAnd[BitPos];
		}
		else
		{
			lastbyte = ReadByte();
			BitPos--;
			newbyte = CurByte & MyAnd[BitPos];
			tempcode = (UINT8)lastbyte >> 7;
			CurByte = newbyte;
		}
		thiscode = (thiscode << 1) + tempcode;
		codelen++;
		if(codelen > 16)
		{
			return FUNC_FORMAT_ERROR;
		}
	}  //while
	temp = thiscode - huf_min_value[HufTabIndex][codelen - 1] + code_pos_table[HufTabIndex][codelen - 1];
	hufexbyte = (UINT8)code_value_table[HufTabIndex][temp];
	rrun = (int)(hufexbyte >> 4);  /* һ���ֽ��У�����λ����ǰ�����ĸ�����*/
	runsize = hufexbyte & 0x0f;      /* ����λΪ�����ֵĳߴ� */
	if(runsize == 0)
	{
		vvalue = 0;
		return FUNC_OK;
	}
	tempsize = runsize;
	if(BitPos >= runsize)
	{
		BitPos = BitPos - runsize;
		valueex = (UINT8)CurByte >> BitPos;
		CurByte = CurByte & MyAnd[BitPos];
	}
	else
	{
		valueex = CurByte;
		tempsize = tempsize - BitPos;
		while(tempsize > 8)
		{
			lastbyte = ReadByte();
			valueex = (valueex << 8) + (UINT8)lastbyte;
			tempsize = tempsize - 8;
		}  //while
		lastbyte = ReadByte();
		BitPos = BitPos - tempsize;
		valueex = (valueex << tempsize) + (lastbyte >> BitPos);
		CurByte = lastbyte & MyAnd[BitPos];
	}  //else
	sign = valueex >> (runsize - 1);
	if(sign != 0)
	{
		vvalue = valueex;             /* �������ֵ */
	}
	else
	{
		valueex = valueex ^ 0xffff;
		temp = 0xffff << runsize;
		vvalue = -(int)(valueex ^ temp);
	}
	return FUNC_OK;
}

/************************************************************************************
������MCU�е�ÿ�����   ��� MCUBuffer ���� QtZzMCUBuffer
*************************************************************************************/
void CJpegDecode::IQtIZzMCUComponent(int flag)
{
	int H,VV,i,j;
	int *pQtZzMCUBuffer,*tempbuf1;
	int *pMCUBuffer,*tempbuf2;

	switch(flag)
	{
	case 0:
		H = SampRate_Y_H;
		VV = SampRate_Y_V;
		pMCUBuffer = MCUBuffer;  /* Huffman Decoded */
		pQtZzMCUBuffer = QtZzMCUBuffer;
		break;
	case 1:
		H = SampRate_U_H;
		VV = SampRate_U_V;
		pMCUBuffer = MCUBuffer;
		pMCUBuffer += Y_in_MCU << 6;
		pQtZzMCUBuffer = QtZzMCUBuffer;
		pQtZzMCUBuffer += Y_in_MCU << 6;
		break;
	case 2:
		H = SampRate_V_H;
		VV = SampRate_V_V;
		pMCUBuffer = MCUBuffer;
		pMCUBuffer += (Y_in_MCU + U_in_MCU) << 6;
		pQtZzMCUBuffer = QtZzMCUBuffer;
		pQtZzMCUBuffer += (Y_in_MCU + U_in_MCU) << 6;
		break;
	default:
		H = 0;
		pQtZzMCUBuffer = NULL;
		pMCUBuffer = NULL;
		VV = 0;
		break;
	}
	for(i = 0;i < VV;i++)
	{
		for(j = 0;j < H;j++)
		{
			tempbuf2 = pMCUBuffer;
			tempbuf2 += (i * H + j) << 6;
			tempbuf1 = pQtZzMCUBuffer;
			tempbuf1 += (i * H + j) << 6;
			IQtIZzBlock(tempbuf2,tempbuf1,flag);      /* 8*8DU */
		}
	}
}

/* Ҫ�������� */
/**********************************************************************************
������ 8*8 DU
***********************************************************************************/
void CJpegDecode::IQtIZzBlock(int *s,int *d,int flag)
{
	int i,j,tag;
	int *pQt,*temp1,*temp3;
	int buffer2[8][8];
	int *temp2;
	int offset;

	switch(flag)
	{
	case 0:                /* ���� */
		pQt = YQtTable;      /* ��������ַ */
		/* ShowMessage(IntTostr(YQtTable^)); */
		offset = 128;
		break;
	case 1:                /* �� */
		pQt = UQtTable;
		offset = 0;
		break;
	case 2:               /* �� */
		pQt = VQtTable;
		offset = 0;
		break;
	default:
		pQt = NULL;
		offset = 0;
		break;
	}

	for(i = 0;i < 8;i++)
	{
		for(j = 0;j < 8;j++)
		{
			tag = Zig_Zag[i][j];
			temp1 = s;
			temp1 = temp1 + tag;
			temp3 = pQt;
			temp3 = temp3 + tag;
			buffer2[i][j] = (int)((*temp1) * (*temp3));
		}
	}
	Fast_IDCT(&buffer2[0][0]);    /* ��DCT */
	for(i = 0;i < 8;i++)
	{
		for(j = 0;j < 8;j++)
		{
			temp2 = d;
			temp2 = temp2 + (i * 8 +j);
			*temp2 = buffer2[i][j] + offset;
		}
	}
}

void CJpegDecode::Fast_IDCT(int *block)
{
	int i;
	for(i = 0;i < 8;i++)
	{
		idctrow(block + 8 * i);
	}
	for(i = 0;i < 8;i++)
	{
		idctcol(block + i);
	}
}

UINT8 CJpegDecode::ReadByte(void)
{
	UINT8 i;

	i = *lp; /* lp Ϊ�������ʼλ�� */
	lp = lp + 1;
	if(i == 0xff)
	{
		lp = lp + 1;
	}
	BitPos = 8;
	CurByte = i;
	return i;
}

void CJpegDecode::Initialize_Fast_IDCT(void)
{
	int i;

	for(i = -512;i < 512;i++)
	{
		if(i < -256)
		{
			iclip[512 + i] = -256;
		}
		if(i > 255)
		{
			iclip[512 + i] = 255;
		}
		if(( i >= -256) && (i <= 255))
		{
			iclip[512 + i] = i;
		}
	}
}

void CJpegDecode::idctrow(int *blk)
{
	int x0, x1, x2, x3, x4, x5, x6, x7, x8;

	//intcut
	x1 = blk[4] << 11;
	x2 = blk[6];
	x3 = blk[2];
	x4 = blk[1];
	x5 = blk[7];
	x6 = blk[5];
	x7 = blk[3];
	if ((x1 || x2 || x3 || x4 || x5 || x6 || x7) == 0)
	{
		blk[1] = blk[0] << 3;
		blk[2] = blk[0] << 3;
		blk[3] = blk[0] << 3;
		blk[4] = blk[0] << 3;
		blk[5] = blk[0] << 3;
		blk[6] = blk[0] << 3;
		blk[7] = blk[0] << 3;
		blk[0] = blk[0] << 3;
		return;
	}
	x0 = (blk[0] << 11) + 128; /* for proper rounding in the fourth stage */
	/* first stage */
	x8 = W7 * (x4 + x5);
	x4 = x8 + (W1 - W7) * x4;
	x5 = x8 - (W1 + W7) * x5;
	x8 = W3 * (x6 + x7);
	x6 = x8 - (W3 - W5) * x6;
	x7 = x8 - (W3 + W5) * x7;
	/* second stage */
	x8 = x0 + x1;
	x0 = x0 - x1;
	x1 = W6 * (x3 + x2);
	x2 = x1 - (W2 + W6) * x2;
	x3 = x1 + (W2 - W6) * x3;
	x1 = x4 + x6;
	x4 = x4 - x6;
	x6 = x5 + x7;
	x5 = x5 - x7;
	/* third stage */
	x7 = x8 + x3;
	x8 = x8 - x3;
	x3 = x0 + x2;
	x0 = x0 - x2;

	x2 = (181 * (x4 + x5) + 128) >> 8;
	x4 = (181 * (x4 - x5) + 128) >> 8;
	/* fourth stage */

	blk[0] = (x7 + x1) >> 8;
	blk[1] = (x3 + x2) >> 8;
	blk[2] = (x0 + x4) >> 8;
	blk[3] = (x8 + x6) >> 8;
	blk[4] = (x8 - x6) >> 8;
	blk[5] = (x0 - x4) >> 8;
	blk[6] = (x3 - x2) >> 8;
	blk[7] = (x7 - x1) >> 8;
}

void CJpegDecode::idctcol(int * blk)
{
	int x0, x1, x2, x3, x4, x5, x6, x7, x8;
	int Temp;

	/* intcut */

	x1 = blk[8 * 4] << 8;
	x2 = blk[8 * 6];
	x3 = blk[8 * 2];
	x4 = blk[8 * 1];
	x5 = blk[8 * 7];
	x6 = blk[8 * 5];
	x7 = blk[8 * 3];
	if ((x1 | x2 | x3 | x4 | x5 | x6 | x7) == 0)
	{
		Temp = (blk[8 * 0] + 32) >> 6;
		/*      if((blk[8 * 0] + 32) < 0)
		{
		Temp = ((blk[8 * 0] + 32) >> 6) | ((!(int)(0)) << (32 - 6));
		}
		else
		{
		Temp = (blk[8 * 0] + 32) >> 6;
		}*/
		blk[8 * 1] = iclip[512 + Temp];
		blk[8 * 2] = iclip[512 + Temp];
		blk[8 * 3] = iclip[512 + Temp];
		blk[8 * 4] = iclip[512 + Temp];
		blk[8 * 5] = iclip[512 + Temp];
		blk[8 * 6] = iclip[512 + Temp];
		blk[8 * 7] = iclip[512 + Temp];
		blk[8 * 0] = iclip[512 + Temp];
		return;
	}
	x0 = (blk[8 * 0] << 8) + 8192;
	/* first stage */
	x8 = W7 * (x4 + x5) + 4;

	x4 = (x8 + (W1 - W7) * x4) >> 3;
	x5 = (x8 - (W1 + W7) * x5) >> 3;
	x8 = W3 * (x6 + x7) + 4;

	x6 = (x8 -(W3 - W5) * x6) >> 3;
	x7 = (x8 - (W3 + W5) * x7) >> 3;
	/* second stage */
	x8 = x0 + x1;
	x0 = x0 - x1;
	x1 = W6 * (x3 + x2) + 4;

	x2 = (x1 - (W2 + W6) * x2) >> 3;
	x3 = (x1 + (W2 - W6) * x3) >> 3;

	x1 = x4 + x6;
	x4 = x4 - x6;
	x6 = x5 + x7;
	x5 = x5 - x7;
	/* third stage */
	x7 = x8 + x3;
	x8 = x8 - x3;
	x3 = x0 + x2;
	x0 = x0 - x2;

	x2 = (181 * (x4 + x5) + 128) >> 8;
	x4 = (181 * (x4 - x5) + 128) >> 8;
	/* fourth stage */
	Temp = (x7 + x1) >> 14;

	blk[0] = iclip[512 + Temp];
	Temp = (x3 + x2) >> 14;

	blk[8] = iclip[512 + Temp];
	Temp = (x0 + x4) >> 14;

	blk[16] = iclip[512 + Temp];
	Temp = (x8 + x6) >> 14;

	blk[24] = iclip[512 + Temp];
	Temp = (x8 - x6) >> 14;

	blk[32] = iclip[512 + Temp];
	Temp = (x0 - x4) >> 14;

	blk[40] = iclip[512 + Temp];
	Temp = (x3 - x2) >> 14;

	blk[48] = iclip[512 + Temp];
	Temp = (x7 - x1) >> 14;

	blk[56] = iclip[512 + Temp];

}

