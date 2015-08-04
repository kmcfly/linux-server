#include "PS_Packet_Packaging.h"
#include "PS_Define.h"
#include <stdio.h>

static unsigned char PS_header[PS_Packet_Start_Code_Len] = {0x00 , 0x00 , 0x01 , 0xBA};
static unsigned char PS_System_Header[PS_Packet_Start_Code_Len] = {0x00 , 0x00 , 0x01 , 0xBB};
static unsigned char PS_Map_Header[PS_Packet_Start_Code_Len] = {0x00 , 0x00 , 0x01 , 0xBC};
static unsigned char PS_Audio_Pes_Header[PS_Packet_Start_Code_Len] = {0x00 , 0x00 , 0x01 , 0xC0};
static unsigned char PS_Video_Pes_Header[PS_Packet_Start_Code_Len] = {0x00 , 0x00 , 0x01 , 0xE0};
static unsigned char MPEG_program_end_code[PS_Packet_Start_Code_Len] = {0x00 , 0x00 , 0x01 , 0xB9};
static unsigned char DSM_CC_Header[PS_Packet_Start_Code_Len] = {0x00 , 0x00 , 0x01 , 0xF2};

CPSPackaging::CPSPackaging()
{
	m_PtsFixVideo = 3600;//大于3600的任意初始值
	m_PtsFixAudio = 3600;
}

CPSPackaging::~CPSPackaging()
{
	
}

int CPSPackaging::Packet_I_frame(const char* pSrcBuf , int nSrcLen, char* pDestBuf , int& nDestLen, LONGLONG llPts, LONGLONG llDts)
{
	if ( NULL == pSrcBuf || NULL == pDestBuf )
	{
		return PS_Error_Param;
	}

	int nTempLen = 0;
	int nTotalLen = 0;
	
	Packet_PS_header(pDestBuf ,nTempLen,llPts);
	nTotalLen += nTempLen;
	Packet_System_header(pDestBuf + nTotalLen ,nTempLen); //增加系统头
	nTotalLen += nTempLen;
	Packet_PS_map(pDestBuf + nTotalLen , nTempLen);
	nTotalLen += nTempLen;
	
	GeneratePacketsFromFrame(pSrcBuf , nSrcLen , pDestBuf + nTotalLen , nTempLen, llPts, llDts);

	nTotalLen += nTempLen;

// 	//添加MPEG_program_end_code   加不加没影响
// 	memcpy(pDestBuf + nTotalLen, MPEG_program_end_code, 4);
	//nTotalLen += 4;

	nDestLen = nTotalLen;

	return PS_Error_OK ;
}

int CPSPackaging::Packet_P_frame(const char* pSrcBuf , int nSrcLen, char* pDestBuf , int& nDestLen, LONGLONG llPts, LONGLONG llDts)
{
	if ( NULL == pSrcBuf || NULL == pDestBuf )
	{
		return PS_Error_Param;
	}
	
	int nTempLen = 0;
	int nTotalLen = 0;
	
	Packet_PS_header(pDestBuf , nTempLen , llPts);
	nTotalLen += nTempLen;

	GeneratePacketsFromFrame(pSrcBuf , nSrcLen , pDestBuf + nTotalLen , nTempLen, llPts, llDts);
	nTotalLen += nTempLen;

// 	//添加MPEG_program_end_code
// 	memcpy(pDestBuf + nTotalLen, MPEG_program_end_code, 4);
	//nTotalLen += 4;

	nDestLen = nTotalLen;

	return PS_Error_OK ;
}

int CPSPackaging::Packet_Audio_frame(const char* pSrcBuf , int nSrcLen, char* pDestBuf , int& nDestLen,LONGLONG llAudioPts)
{
	if ( NULL == pSrcBuf || NULL == pDestBuf )
	{
		return PS_Error_Param;
	}
	
	int nTempLen = 0;
	int nTotalLen = 0;

	Packet_Audio_frame_G711(pSrcBuf, nSrcLen , pDestBuf + nTotalLen , nTempLen, llAudioPts);
	nTotalLen += nTempLen;

	nDestLen = nTotalLen;

	return PS_Error_OK ;
}

/*
    封装PS_header，一般标准为14个字节，代码为软封装，不需要考虑字节对齐，为了扩展性可以补齐字节，方便移植
*/
static int first = 0;
int i;
int CPSPackaging::Packet_PS_header(char* pDestBuf, int& nLen ,LONGLONG llVideoPts)
{
	unsigned long tScrExt=0;
	LONGLONG  m_nScrb = llVideoPts + m_PtsFixVideo - 3600;

	if ( NULL == pDestBuf)
	{
		return PS_Error_Param;
	}
	
	memcpy( pDestBuf, PS_header , PS_Packet_Start_Code_Len);

	char temp = 0x00;

	temp = (m_nScrb>>30) & 0x07;
	pDestBuf[4]= (temp<<3) | ((m_nScrb>>28) & 0x03) | 0x44;

	pDestBuf[5]= m_nScrb>>20;

	temp = m_nScrb>>15 & 0x1f;
	pDestBuf[6]= (temp<<3) | ((m_nScrb>>13) & 0x03) | 0x04;
	pDestBuf[7]= m_nScrb >> 5;

	//system_clock_reference_extension 为scr%300
	tScrExt=m_nScrb%300;
	
	pDestBuf[8]=(((m_nScrb&0xf8)<<3) | (tScrExt>>7) & 0x3) | 0x04;
	pDestBuf[9]=((tScrExt&0x7f<<1)|0x01);
				
	pDestBuf[10]=(MUX_RATE>>14)&0xff;
	pDestBuf[11]=(MUX_RATE>>6)&0xff;
	pDestBuf[12]=(((MUX_RATE<<2)&0xfc)|0x03);

	pDestBuf[13] = 0xF8;
	nLen = PS_Header_Len;
			
	return PS_Error_OK;
}

int CPSPackaging::Packet_System_header(char* pDestBuf, int& nLen)
{
	if ( NULL == pDestBuf)
	{
		return PS_Error_Param;
	}

	memcpy( pDestBuf, PS_System_Header , PS_Packet_Start_Code_Len);

	pDestBuf[4] = 0x00;	
	pDestBuf[5] = 0x0C;

	pDestBuf[6] = 0x80 | (MUX_RATE>>15);
	pDestBuf[7] = MUX_RATE>>7;

	pDestBuf[8] = 0x01 | MUX_RATE<<1;//rate_bound >= mutex rate
	pDestBuf[9] = 0xFE;//audio_bound = 

	pDestBuf[10] = 0xE1;
	pDestBuf[11] = 0x7F;    

    pDestBuf[12] = 0xE0;//stream_id  = h.262
	pDestBuf[13] = 0xE0;//P-STD_buffer_bound_scale = 1

    //pDestBuf[14] = 0xE8;
	pDestBuf[14] = 0xD8;//P-STD_buffer_size_bound
	pDestBuf[15] = 0xC0;//stream_id  = 音频流编号

	pDestBuf[16] = 0xC0;///P-STD_buffer_bound_scale = 0
	pDestBuf[17] = 0x20;
    
    nLen = 18;  
	return PS_Error_OK;
}

int CPSPackaging::Packet_PS_map(char* pDestBuf, int& nLen)
{
	if ( NULL == pDestBuf)
	{
		return PS_Error_Param;
	}

	memcpy( pDestBuf, PS_Map_Header , PS_Packet_Start_Code_Len);

	pDestBuf[4] = 0x00;	
	pDestBuf[5] = 0x18;

	pDestBuf[6] = 0xE1;
	pDestBuf[7] = 0xFF;

	pDestBuf[8] = 0x00;
	pDestBuf[9] = 0x00;

	pDestBuf[10] = 0x00;
	pDestBuf[11] = 0x08;

	//element_info
	pDestBuf[12] = 0x1B;   //H.264

	pDestBuf[13] = 0xE0;
	pDestBuf[14] = 0x00;
	pDestBuf[15] = 0x06;

	//avc timming and hrd descriptor
	pDestBuf[16] = 0x0a;
	pDestBuf[17] = 0x04;
	pDestBuf[18] = 0x65;
	pDestBuf[19] = 0x6e;
	pDestBuf[20] = 0x67;
	pDestBuf[21] = 0x00;

	pDestBuf[22] = 0x90;
	pDestBuf[23] = 0xc0;
	pDestBuf[24] = 0x00;
	pDestBuf[25] = 0x00;

	pDestBuf[26] = 0x23;
	pDestBuf[27] = 0xb9;
	pDestBuf[28] = 0x0f;
	pDestBuf[29] = 0x3d;

	nLen = 30;      
	return PS_Error_OK;
}
int CPSPackaging::PacketAudioMap(char* pDestBuf, int& nLen,int Compression)
{
	if ( NULL == pDestBuf)
	{
		return PS_Error_Param;
	}

	memcpy( pDestBuf, PS_Map_Header , PS_Packet_Start_Code_Len);

	pDestBuf[4] = 0x00;	
	pDestBuf[5] = 0x18;

	pDestBuf[6] = 0xE1;
	pDestBuf[7] = 0xFF;

	pDestBuf[8] = 0x00;
	pDestBuf[9] = 0x00;

	pDestBuf[10] = 0x00;
	pDestBuf[11] = 0x08;

	//element_info
	pDestBuf[12] = 0x90;   //G711

	pDestBuf[13] = 0xE0;
	pDestBuf[14] = 0x00;
	pDestBuf[15] = 0x06;

	//avc timming and hrd descriptor
	pDestBuf[16] = 0x0a;
	pDestBuf[17] = 0x04;
	pDestBuf[18] = 0x65;
	pDestBuf[19] = 0x6e;
	pDestBuf[20] = 0x67;
	pDestBuf[21] = 0x00;

	pDestBuf[22] = 0x90;
	pDestBuf[23] = 0xc0;
	pDestBuf[24] = 0x00;
	pDestBuf[25] = 0x00;

	pDestBuf[26] = 0x23;
	pDestBuf[27] = 0xb9;
	pDestBuf[28] = 0x0f;
	pDestBuf[29] = 0x3d;

	nLen = 30;           
	return PS_Error_OK;
}

//一个NAL送一次这个函数，进行一次打包
int CPSPackaging::Packet_Video_frame(const char* pSrcBuf, int nSrcLen, char* pDestBuf, int& nLen , LONGLONG llPts, LONGLONG llDts, bool bFirst)
{

	if ( NULL == pSrcBuf || NULL == pDestBuf )
	{
		return PS_Error_Param;
	}

	int nTempLen = 0;
	unsigned short PESPacketLen = 0;
	LONGLONG VideoPts = llPts + m_PtsFixVideo;

	memcpy( pDestBuf , PS_Video_Pes_Header , PS_Packet_Start_Code_Len);

	
	pDestBuf[6] = 0x80;//PES_priority = 0

	pDestBuf[7] = 0xc0;//PTS_DTS_flags = 10?  改为11
	
	
	pDestBuf[8] = 0x0a;
 	if ( bFirst )
 	{
		pDestBuf[9]=((0x0E&(VideoPts>>29))|0x31);
		pDestBuf[10] = ((VideoPts>>22)&(0xFF));
		pDestBuf[11]=(((VideoPts>>14)&(0xFE))|0x01);
		pDestBuf[12]=((VideoPts>>7)&(0xFF));
		pDestBuf[13] = (((VideoPts<<1)&0xFE)|0x01);

		VideoPts -= 3600;
		pDestBuf[14]=((0x0E&(VideoPts>>29))|0x11);
		pDestBuf[15] = ((VideoPts>>22)&(0xFF));
		pDestBuf[16]=(((VideoPts>>14)&(0xFE))|0x01);
		pDestBuf[17]=((VideoPts>>7)&(0xFF));
		pDestBuf[18] = (((VideoPts<<1)&0xFE)|0x01);	
 	}
 	else
 	{
		pDestBuf[9]  = 0x31;
		pDestBuf[10] = 0x00;
		pDestBuf[11] = 0x01;
		pDestBuf[12] = 0x00;
		pDestBuf[13] = 0x01;

		pDestBuf[14]  = 0x11;
		pDestBuf[15] = 0x00;
		pDestBuf[16] = 0x01;
		pDestBuf[17] = 0x00;
		pDestBuf[18] = 0x01;
 	}

	nTempLen = 19;
	
	PESPacketLen = nTempLen - 6 + nSrcLen;
	//PES包的长度
	pDestBuf[4] = (PESPacketLen >> 8) & 0xFF;
	pDestBuf[5] = PESPacketLen & 0xFF;

	memcpy(pDestBuf + nTempLen , pSrcBuf , nSrcLen);

	nLen = nTempLen + nSrcLen;

	return PS_Error_OK;
}

int CPSPackaging::Packet_Audio_frame_G711(const char* pSrcBuf , int nSrcLen , char* pDestBuf, int& nLen, LONGLONG llAudioPts)
{
	 int nTempLen = 0;
	 unsigned short PESPacketLen = 0;
	 LONGLONG  AudioPts=0;

	if ( NULL == pSrcBuf || NULL == pDestBuf )
	{
		return PS_Error_Param;
	}
	
	AudioPts = llAudioPts + m_PtsFixAudio;

	memcpy( pDestBuf , PS_Audio_Pes_Header , PS_Packet_Start_Code_Len);
	
	pDestBuf[6] = 0x80;
	
	pDestBuf[7] = 0xc0;//PTS_DTS_flags = 10?  改为11

	
	pDestBuf[8] = 0x0a;


	pDestBuf[9]=((0x0E&(AudioPts>>29))|0x31);
	pDestBuf[10] = ((AudioPts>>22)&(0xFF));
	pDestBuf[11]=(((AudioPts>>14)&(0xFE))|0x01);
	pDestBuf[12]=((AudioPts>>7)&(0xFF));
	pDestBuf[13] = (((AudioPts<<1)&0xFE)|0x01);

	AudioPts -= 3600;
	pDestBuf[14]=((0x0E&(AudioPts>>29))|0x11);
	pDestBuf[15] = ((AudioPts>>22)&(0xFF));
	pDestBuf[16]=(((AudioPts>>14)&(0xFE))|0x01);
	pDestBuf[17]=((AudioPts>>7)&(0xFF));
	pDestBuf[18] = (((AudioPts<<1)&0xFE)|0x01);	


	nTempLen = 19;
	
	PESPacketLen = nTempLen - 6 + nSrcLen;
	//PES包的长度
	pDestBuf[4] = (PESPacketLen >> 8) & 0xFF;
	pDestBuf[5] = PESPacketLen & 0xFF;

	memcpy(pDestBuf + nTempLen , pSrcBuf , nSrcLen);
	nLen = nTempLen + nSrcLen;
	
	return PS_Error_OK;
}


int CPSPackaging::GeneratePacketsFromFrame(const char* pSrcBuf , int nSrcLen , char* pDestBuf , int& nLen, LONGLONG llPts, LONGLONG llDts)
{	
	int	nTotalLen = 0; 
	int nPacketNalLen = 0;
	int rest_len = nSrcLen;
	int nNalCount = 0;
	int nOldIndex = 0;
	int nPacketCount = 0;


	//只有最后一个NAL有可能存在大于60K的情况
	while( nSrcLen - nOldIndex > 60*1024)
	{
		//printf("Pes nSrcLen - nOldIndex > 60*1024 : %lu\n", nSrcLen - nOldIndex);
		if ( nPacketCount == 0 )
		{
			Packet_Video_frame(pSrcBuf + nOldIndex , 60*1024 , pDestBuf + nTotalLen , nPacketNalLen, llPts, llDts, true);	
		}
		else
		{
			Packet_Video_frame(pSrcBuf + nOldIndex , 60*1024 , pDestBuf + nTotalLen , nPacketNalLen , llPts, llDts, false);
		}
		nOldIndex += 60*1024;
		nTotalLen += nPacketNalLen;	
		nPacketCount++;
	}
	
	if ( nPacketCount == 0)
	{
		Packet_Video_frame(pSrcBuf + nOldIndex , nSrcLen - nOldIndex , pDestBuf + nTotalLen , nPacketNalLen, llPts, llDts, true);
	}
	else
	{
		Packet_Video_frame(pSrcBuf + nOldIndex , nSrcLen - nOldIndex , pDestBuf + nTotalLen , nPacketNalLen , llPts, llDts, false);
	}

	nTotalLen += nPacketNalLen;
	nPacketCount++;
	nLen = nTotalLen;	
	return PS_Error_OK;
}


int CPSPackaging::Packet_DSM_CC(char* pDestBuf , int &nDestLen, LONGLONG Pts, DSM_CC_RETRIEVE_FLAG flag,  bool bIsFastMode, bool bIsStorage)
{
	if (!pDestBuf || flag > DSM_CC_STOP)
	{
		return -1;
	}

	memcpy( pDestBuf , DSM_CC_Header , PS_Packet_Start_Code_Len);

	unsigned short iter = 0;

//comman id
	pDestBuf[6] = 0x01;
//control
	pDestBuf[7] = 0x40 | ((0x01 & bIsStorage)<<5);//retrieve flag == 1
	pDestBuf[8] = 0x01;

	pDestBuf[9] = (0x01 << (7-flag));
	pDestBuf[10] = 0x01;
	iter = 10;

	if (flag == DSM_CC_PLAY)//不支持jump flag
	{
		pDestBuf[++iter] = (0x01 &bIsFastMode) << 7;//只支持前向播放
		iter += time_code(&pDestBuf[++iter], Pts, false);
		iter -= 1;
	}

	if (bIsStorage)
	{
		pDestBuf[++iter] = 0x01 & (flag == DSM_CC_STOP);
	}
	
	//packet len
	pDestBuf[4] = (iter+1-6) >> 8;
	pDestBuf[5] = (iter+1-6);

	nDestLen = iter+1;

	return 0;
}

int CPSPackaging::time_code(char* pDestBuf, LONGLONG pts, bool bInfiniteFlag)
{
	int len = 0;
	if (bInfiniteFlag)
	{
		pDestBuf[0] = 0x01;
		len = 1;
	}
	else
	{
		len = 6;
		pDestBuf[0] = 0x00;
		pDestBuf[1]=((0x0E&(pts>>29)) | 0x01);
		pDestBuf[2] = ((pts>>22)&(0xFF));
		pDestBuf[3]=(((pts>>14)&(0xFE))|0x01);
		pDestBuf[4]=((pts>>7)&(0xFF));
		pDestBuf[5] = (((pts<<1)&0xFE)|0x01);
	}

	return len;
}