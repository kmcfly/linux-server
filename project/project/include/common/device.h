/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : Ԭʯά
** Date         : 2009-06-30
** Name         : device.h
** Version      : 1.0
** Description  :
** Modify Record:
1: ����ļ�����ʹ�ã���־������Ҳ��һ����������ݱ�Ӧ����һ�µġ�����
  �����ڽṹ��DISPLAY_ATTRIB�����仯ʱ����ʱû�м�ʱ֪ͨ��־�߽���ͬ���޸ģ�
  ��֮ʱ������ƣ����ݲ���ͬ����һ���ˡ�����б仯�����໥֪ͨ�Է��޸ġ���2011040916��
***********************************************************************/
#ifndef _DEVICE_H_
#define _DEVICE_H_

//��Ʒ���Ͷ���
typedef enum _product_type
{
	TD_DEVICE_NONE = 0,
	TD_2304SE_S = 100,      //�����ڸò�Ʒ
	TD_2308SE_S,            //�����ڸò�Ʒ
	TD_2316SE_S,            //3515����16CIF
	TD_2324SE_S,            //3520����24·

	TD_2304SE_B = 120,
	TD_2308SE_B,
	TD_2316SE_B,

	TD_2304CDS = 150,       //3515����costdown4·,�г�����2304SS
	TD_2308CDS,             //3515����costdown8·,�г�����2308SS
	TD_2316CDS,             //3515��1108A����16·���г�����2316SS

	TD_2304SS_B = 160,		//3521 4·D1��û�е�Ƭ��
	TD_2308SS_B,			//3521 8·D1��û�е�Ƭ��
	TD_2316SS_B,			//3521 16·D1��û�е�Ƭ��

	TD_2304SS_SL = 165,		//TI 4·D1
	TD_2308SS_SL,			//TI 8·D1

	TD_2304SL_C = 170,		//3515A	4·960H
	TD_2308SL_C,			//3520D	8·960H

	TD_2304HS = 175,        //3515����costdown4·��D1ʵʱ��Ʒ

	TD_2304SS_C = 180,		//3520A����4·ʵʱ960H
	TD_2308SS_C,			//3520A����8X15F960H,8X30FD1
	TD_2316SS_C,			//3520A����16X7F960H,16X30FCIF
	TD_2304SS_SL_3515A,			//3515A���ģ��õ���3520D�İ�

	TD_2304SE_C = 185,		//3520A����4·ʵʱ960H
	TD_2308SE_C,			//3520A����8X15F960H,8X30FD1
	TD_2316SE_C,			//3521����16X7F960H,16X30FD1

	TD_2304SS_SDI = 190,	//3520D SDIX1+CVBSX3
	TD_2308SS_SDI,			//3521  SDIX2+CVBSX6

	TD_2304SE	= 200,      //3515����4·
	TD_2308SE,              //3515����8·
	TD_2316SE,              //3515����16·

	TD_2304ME	= 300,      //3515����4·
	TD_2308ME,              //3515����8·
	TD_2316ME,              //3520����16·ʵʱCIF

	TD_2304ME_B		= 310,	//
	TD_2308ME_B,			//
	TD_2316ME_B,			//

	TD_2304ME_BH	= 320,	//
	TD_2308ME_BH	= 321,	//
	TD_2316ME_BH	= 322,	//

	TD_2304ME_A = 350,      //fullhanƽ̨���Ĳ�Ʒ,Ŀǰû�У���Ԥ����
	TD_2308ME_A = 351,      //fullhanƽ̨���Ĳ�Ʒ, 2D1+6CIF
	TD_2316ME_A = 352,      //fullhanƽ̨���Ĳ�Ʒ,	2D1+14CIF


	TD_2304ME_C = 360,		//3520A ����960H
	TD_2308ME_C,
	TD_2316ME_C,

	TD_2304HE	= 400,      //�����ڸò�Ʒ
	TD_2308HE,              //�����ڸò�Ʒ
	TD_2316HE,              //�����ڸò�Ʒ

	TD_2404MD	= 500,      //3520����4·
	TD_2408MD_4A,           //3520����8·����Ƶ4·�������й��ò�Ʒ��Ŀǰ�Ѿ�û���ˣ����ǲ���ɾ��
	TD_2416MD_4A,           //3520����16·����Ƶ4·�������й��ò�Ʒ��Ŀǰ�Ѿ�û���ˣ����ǲ���ɾ��
	TD_2432MD_4A,           //�����ڸò�Ʒ
	TD_2408MD,              //3520����8·,��Ƶ8·
	TD_2416MD,              //3520����16·,��Ƶ16·
	TD_2424MD,              //3520����24·,��Ƶ16·
	TD_2432MD,              //3520����32·,��Ƶ16·

	TD_2404MD_B	= 520,     //3531����4·D1
	TD_2408MD_B,           //3531����4·D1+4·CIF
	TD_2416MD_B,           //3531����4·D1+12·CIF
	TD_2424MD_B,           //3531����16·D1+8·CIF
	TD_2432MD_B,           //3531����16·D1+16·CIF

	TD_2504ME	= 600,       //�����ڸò�Ʒ
	TD_2508ME,               //�����ڸò�Ʒ 
	TD_2516ME,               //�����ڸò�Ʒ

	TD_2504HE	= 700,        //3520��D1ʵʱ
	TD_2508HE,                //3520��D1ʵʱ
	TD_2516HE,                //3520��D1ʵʱ
	TD_2504HE_M,              //3515��D1ʵʱ���г�����2304HE

	//TD_2504HC_B = 710,	//FH 4D1
	TD_2508HC_B	= 711,		//3531 8D1
	TD_2516HC_B,			//3531 16D1

	TD_2504HE_B = 720,		//FH 4D1, FH �� TD_2504HC_B��Ʒ�������Ʒ��ͬһ��ID	
	TD_2508HE_B,
	TD_2516HE_B,			//3531ƽ̨����16D1��Ʒ
	TD_2508HE_B_3521,		//3521ƽ̨����8D1��Ʒ
	TD_2524HE_B,
	TD_2532HE_B,			//3531ƽ̨����32D1��Ʒ

	TD_2504HE_L = 725,		//3515 4D1
	TD_2508HE_L,			//FH 8D1
	TD_2516HE_L,			//3531 16D1

	TD_2504HE_B_N100 = 730,
	TD_2508HE_B_N100,
	TD_2516HE_B_N100,		//3531ƽ̨����16D1��Ʒ, 100M������û�е�Ƭ��,������TD_2516HE_Bһ��, HS-B

	TD_2508HE_SDI = 735,	//3521 SDIX2+CVBSX6  720P
	TD_2512HE_SDI,			//3531 SDIX4+CVBSX8  720P
	TD_2524HE_SDI,			//3531 SDIX4+CVBSX20

	TD_2504HC	= 750,        //3520��D1ʵʱ
	TD_2508HC,                //3520��D1ʵʱ
	TD_2516HC,                //3520��D1ʵʱ
	
	TD_2504HS_BL= 760,		  //3520A
	TD_2508HS_BL,			  //3520A
	TD_2516HS_BL,			  //3520A

	TD_2532HD_SDI = 770,		//3531 SDIX4+CVBSX28

	TD_2504MD	= 800,         //�����ڸò�Ʒ ����ֶβ�������
	TD_2508MD,                 //�����ڸò�Ʒ ����ֶβ�������
	TD_2516MD,                 //�����ڸò�Ʒ ����ֶβ�������	

	TD_2504HD	= 900,         //3520��D1ʵʱ
	TD_2508HD_4A,              //�����ڸò�Ʒ 
	TD_2516HD_4A,              //�����ڸò�Ʒ 
	TD_2508HD,                 //3520��D1ʵʱ
	TD_2516HD,                 //3520��D1ʵʱ

	TD_2504HD_C = 910,
	TD_2508HD_C,
	TD_2516HD_C,
	TD_2524HD_C,
	TD_2532HD_C,

	TD_2504FD	= 1000,        //3520��D1ʵʱ
	TD_2508FD_4A,              //�����ڸò�Ʒ
	TD_2516FD_4A,              //�����ڸò�Ʒ
	TD_2508FD,                 //3520��D1ʵʱ
	TD_2516FD,                 //3520��D1ʵʱ
	TD_2516FD_IP,              //2516FD����hybrid��Ʒ

	TD_NXP04    = 1050,        //Ԥ��nxpƽ̨�Ĳ�Ʒ
	TD_NXP08,                  //Ԥ��nxpƽ̨�Ĳ�Ʒ
	TD_2516FD_S,               //nxpƽ̨�Ĳ�Ʒ

	//
	TD_2604LE	= 10000,       //3531/3532ƽ̨
	TD_2608LE,
	TD_2616LE,				   //�����г��ϵ�2516HS_C
	TD_2508HE_C_3521,
	TD_2524HE_C,			   //��2524HE_B����ļ���
	TD_2532HE_C,			   //��2532HE_B����ļ���

	TD_2604LD_S	= 10100,       //3520  960Hƽ̨
	TD_2608LD_S,
	TD_2616LD_S,

	TD_2604LD	= 10200,       //3531/3532 960Hƽ̨
	TD_2608LD,
	TD_2616LD,
	TD_2616LD_IP,
	

	TD_2704HD	= 10600,       //���� 2708XDF

	TD_2704XE_M = 10640,
	TD_2708XE_M,
	TD_2716XE_M,
	TD_2708XE_MH,

	TD_2708XE_P = 10648,

	TD_2704XE_L = 10650,       //2704XEL
	TD_2708XE_L,               //2708XEL
	TD_2716XE_L,               //2716XEL

	TD_2704XD_L = 10660,     //2704XDL
	TD_2708XD_L,             //2708XDL
	TD_2716XD_L,             //2716XDL

	TD_2704XD_M = 10670,     //2704XDM
	TD_2708XD_M,             //2708XDM
	TD_2716XD_M,             //2716XDM

	TD_2704XD_P = 10680,     //2704XDP, 3531 SDI
	TD_2708XD_P,             //2708XDP
	TD_2716XD_P,             //2716XDP

	TD_2704XE_S	= 10690,	//3531 720P
	TD_2708XE_S,
	TD_2716XE_S,

	TD_5104C    = 10700,       //����
	TD_5108C,

	TD_2704XD_S = 10710,
	TD_2708XD_S,
	TD_2716XD_S,

	TD_2704XS_SL = 10720,	//3520A
	TD_2708XS_SL,			//3531
	TD_2716XS_SL,			//3531

	TD_2704AS_SL = 10725,	//8283 720 15
	TD_2708AS_SL,			//8287 720 15
	TD_2716AS_SL,			//xxxx 720 15
	
	TD_2704TS_S = 10730,	//3520A
	TD_2708TS_S,			//
	TD_2716TS_S,			//

	TD_2704TS_PL = 10740,	//3521
	TD_2708TS_PL,			//
	TD_2716TS_PL,			//

	TD_2704AS_S = 10745,	//3520A 720 30
	TD_2708AS_S,			//3521  720 30
	TD_2716AS_S,			//3531  720 30


	TD_2704TD_PL = 10750,	//
	TD_2708TD_PL,			//3531
	TD_2716TD_PL,			//3531
	TD_2732TD_PL,			//3531

	TD_2704TD_P = 10755,	//3520A 720 30
	TD_2708TD_P,			//3521  720 30
	TD_2716TD_P,			//3531  720 30

	TD_2704TE_M = 10760,
	TD_2708TE_M,
	TD_2716TE_M,

	TD_2704AE_SL = 10765,	
	TD_2708AE_SL,			
	TD_2716AE_SL,			//3531

	TD_2004AL = 10770,

	TD_2008AS = 10775,

	TD_2704AS_SL_A = 10780,
	TD_2708AS_SL_A,
	TD_2716AS_SL_A,

	TD_2704AS_S_A = 10785,
	TD_2708AS_S_A,
	TD_2716AS_S_A,

	TD_2704AS_PL = 10790,	//3521
	TD_2708AS_PL,
	TD_2716AS_PL,

	TD_2704AC_PL = 10795,
	TD_2708AC_PL,
	TD_2716AC_PL,
	
	TD_2704AE_PL = 10800,	//3521
	TD_2708AE_PL,
	TD_2716AE_PL,

	TD_2804NS_A   = 20000,
	TD_2808NS_A,
	TD_2816NS_A,
	TD_2824NS_A,
	TD_2832NS_A,

	TD_2804NE_L = 20010,
	TD_2808NE_A = 20012,	//ԭ2816NE-L����Ϊ2808NE-A
	TD_2816NE_A,
	TD_2832NE_A,
	TD_2816NE_B,
	TD_2832NE_B,
	TD_2824NE_A,

	TD_2804ND_A = 20020,
	TD_2808ND_A,
	TD_2816ND_A,
	TD_2824ND_A,
	TD_2832ND_A,

	TD_2804PS_A = 20030,
	TD_2808PS_A,
	TD_2816PS_A,

	TD_2804PE_A = 20040,
	TD_2808PE_A,
	TD_2816PE_A,
	TD_2824PE_A,
	TD_2832PE_A,

	TD_2804PD_A = 20050,
	TD_2808PD_A,
	TD_2816PD_A,
	TD_2824PD_A,
	TD_2832PD_A,

	TD_2804NS_SL = 20060,
	TD_2808NS_SL,
	TD_2816NS_SL,

	TD_2804NS_CL = 20065,
	TD_2808NS_CL,
	TD_2816NS_CL,

	TD_2804NE_B = 20070,
	TD_2808NE_B,

	TD_2804NE_AM = 20080,
	TD_2808NE_AM,
	TD_2816NE_AM,
	TD_2824NE_AM,
	TD_2832NE_AM,

	TD_2804ND_AM = 20090,
	TD_2808ND_AM,
	TD_2816ND_AM,
	TD_2824ND_AM,
	TD_2832ND_AM,

	TD_2804PS_AL = 20100,
	TD_2808PS_AL,
	TD_2816PS_AL,

	TD_2804PE_AL = 20110,
	TD_2808PE_AL,
	TD_2816PE_AL,
	TD_2824PE_AL,
	TD_2832PE_AL,

	TD_2808PD_A16 = 20120,
	TD_2816PD_A16,
	TD_2824PD_A16,
	TD_2832PD_A16,

	TD_7004SB	= 21000,       //�г��Ͻ���7004
	TD_7008SB,                 //�г��Ͻ���7008

	TD_S201		= 30000,			//�洢


	TD_2816NS_C		= 50000,	//3535 16·NVR,		֧��������Ʒ��28040816NS_C		һ�����ܣ���ԭ��Ӳ���汾����ӣ�A��B��C��D��E��F�ֱ����4,8,16,24,32,64·�豸
	TD_2804NS_C,
	TD_2808NS_C,

	TD_3004D1_4P	= 50005,	//3520D
	TD_3008D1_8P,

	TD_2832PD_C16	= 50010,	//3535 16+16·NVR,	֧��һ����Ʒ��2832PD_C16		һ�����ܣ���ԭ��Ӳ���汾����ӣ�A��B��C��D��E��F�ֱ����4,8,16,24,32,64·�豸
	
	TD_2832NE_C		= 50020,	//3535 32·NVR��	֧��������Ʒ��28081632NE_C		һ�����ܣ���ԭ��Ӳ���汾����ӣ�A��B��C��D��E��F�ֱ����4,8,16,24,32,64·�豸
	TD_2808NE_C,
	TD_2816NE_C,

	TD_2816ND_C		= 50030,	//3535 32·NVR��	֧��������Ʒ��281632ND_C		һ�����ܣ���ԭ��Ӳ���汾����ӣ�A��B��C��D��E��F�ֱ����4,8,16,24,32,64·�豸
	TD_2832ND_C,

	TD_2804PS_C4	= 50040,	//3535 4 + 4·NVR��	֧��������Ʒ��280408PS_C4		һ�����ܣ���ԭ��Ӳ���汾����ӣ�A��B��C��D��E��F�ֱ����4,8,16,24,32,64·�豸
	TD_2808PS_C4,

	TD_2808PS_C8	= 50050,	//3535 8 + 8·NVR��	֧��������Ʒ��280816PS_C8		һ�����ܣ���ԭ��Ӳ���汾����ӣ�A��B��C��D��E��F�ֱ����4,8,16,24,32,64·�豸
	TD_2816PS_C8,

	TD_2816PE_C8	= 50060,	//3535 24 + 8·NVR��֧��������Ʒ��281632PE_C8		һ�����ܣ���ԭ��Ӳ���汾����ӣ�A��B��C��D��E��F�ֱ����4,8,16,24,32,64·�豸
	TD_2832PE_C8,
}PRODUCT_TYPE;

typedef enum _product_sub_id
{
	TD_TVT_IN = 0,	//���ʱ�׼��
	TD_TVT_CN = 1,	//���ڱ�׼��
	TD_TVT_SP = 2, //�����
	TD_TVT_US02,	//US02
	TD_TVT_RU04,	//RU04
	TD_TVT_BR27,	//BR27
	TD_TVT_TW01,	//TW01
	TD_TVT_GB14,	//GB14
	TD_TVT_IN23,	//IN23
	TD_TVT_US47,	//US47
	TD_TVT_IN25,    //IN25,ADI
	TD_TVT_PL14,    //PL14
	TD_TVT_IL03,    //IL03
	TD_TVT_LV01,    //LV01
	TD_TVT_CBC,     //CBC
	TD_TVT_GB31,    //GB31
	TD_TVT_ZNV,    //������ά
	TD_TVT_CNB,		//CNB�汾
}PRODUCT_SUB_ID;

//��Ƶ��ʽ����
typedef enum _video_format
{
	VIDEO_FORMAT_NTSC	= 0x00,
	VIDEO_FORMAT_PAL	= 0x01,
	VIDEO_FORMAT_NONE	= 0xff,		//��Ч�ĸ�ʽ
}VIDEO_FORMAT;

typedef enum _frame_type
{
	FRAME_TYPE_NONE				= 0x00,
	FRAME_TYPE_VIDEO			= 0x01,
	FRAME_TYPE_AUDIO			= 0x02,
	FRAME_TYPE_VIDEO_FORMAT		= 0x03,
	FRAME_TYPE_AUDIO_FORMAT		= 0x04,
	FRAME_TYPE_EVENT			= 0x05,
	FRAME_TYPE_TEXT				= 0x06,
	FRAME_TYPE_TALK_AUDIO		= 0x07,
	FRAME_TYPE_TALK_AUDIO_FORMAT= 0x08,
	FRAME_TYPE_END				= 0x09,
	FRAME_TYPE_FILE_HEAD		= 0x10,
	FRAME_TYPE_FILE_INFO		= 0x11,
	FRAME_TYPE_JPEG				= 0x12,
}FRAME_TYPE;

typedef enum _frame_attrib
{
	PLAY_FRAME_NO_SHOW			= 0x01, //��֡����ʾ
	PLAY_FRAME_SHOW				= 0x02, //��֡����ʾ
	PLAY_FRAME_ALL_END			= 0x04, //���ݶ�ȡ�����ˣ�������û������
	PLAY_FRAME_SEC_END			= 0x08, //���¼��ν�����
	PLAY_FRAME_TIME_STAMP		= 0x10, //��֡������ʱ�����ץͼ��ʱ��ע��Ҫ���δ�ʱ��Ĺ���
	LIVE_FRAME_FIRST_STREAM		= 0x20, //��֡Ϊ�ֳ�������
	LIVE_FRAME_SECOND_STREAM	= 0x40, //��֡Ϊ�ֳ�������
	PLAY_FRAME_FF				= 0x80,	//��ǰ֡���ڿ��
	LIVE_FRAME_JPEG				= 0x100,
	LIVE_FRAME_TALK				= 0x200
}FRAME_ATTRIB;

//��λ���档��಻�ܳ���32��
typedef enum _video_size
{
	DVRVIDEO_SIZE_NONE	 = 0x0000,
	DVRVIDEO_SIZE_QCIF	 = 0x0001,	//QCIF
	DVRVIDEO_SIZE_CIF	 = 0x0002,	//CIF
	DVRVIDEO_SIZE_HD1	 = 0x0004,	//HD1
	DVRVIDEO_SIZE_D1	 = 0x0008,	//D1
    DVRVIDEO_SIZE_HD1080 = 0x0010,	//1920X1080
	DVRVIDEO_SIZE_HD720  = 0x0020,	//1280X720
	DVRVIDEO_SIZE_960H   = 0x0040,
	DVRVIDEO_SIZE_VGA    = 0x0080,
	DVRVIDEO_SIZE_QVGA   = 0x0100,

	DVRVIDEO_SIZE_960P   = 0x0200,	//(1280 X 960)
	DVRVIDEO_SIZE_SXGA   = 0x0400,	//(1280 X 1024)
	DVRVIDEO_SIZE_3M     = 0x0800,	//(2048 X 1536)

	DVRVIDEO_SIZE_16_9_3M     = 0x1000,	//(2304 X 1296)
	DVRVIDEO_SIZE_4M          = 0x2000,	//(2688 X 1520)
	DVRVIDEO_SIZE_5M          = 0x4000,	//(2592 X 1944)

}VIDEO_SIZE;

inline int GetDVRVideoIndex(VIDEO_SIZE vs)
{
	switch (vs)
	{
	case DVRVIDEO_SIZE_NONE:
		return -1;
	case DVRVIDEO_SIZE_QCIF:
		return 1;
	case DVRVIDEO_SIZE_CIF:
		return 2;
	case DVRVIDEO_SIZE_HD1:
		return 3;
	case DVRVIDEO_SIZE_D1:
		return 5;
	case DVRVIDEO_SIZE_HD1080:
		return 10;
	case DVRVIDEO_SIZE_HD720:
		return 7;
	case DVRVIDEO_SIZE_960H:
		return 6;
	case DVRVIDEO_SIZE_VGA:
		return 4;
	case DVRVIDEO_SIZE_QVGA:
		return 0;
	case DVRVIDEO_SIZE_960P:
		return 8;
	case DVRVIDEO_SIZE_SXGA:
		return 9;
	case DVRVIDEO_SIZE_3M:
		return 11;
	case DVRVIDEO_SIZE_16_9_3M:
		return 12;
	case DVRVIDEO_SIZE_4M:
		return 13;
	case DVRVIDEO_SIZE_5M:
		return 14;
	default:
		return -1;
	}
}

typedef struct __video_size_bitrate_info__
{
	VIDEO_SIZE		videoSize;
	unsigned long	bitRate;
}VIDEO_SIZE_BITRATE_INFO;


typedef enum _video_stream_type
{
	VIDEO_STREAM_TYPE_NONE      = 0x00,
	VIDEO_STREAM_TYPE_PRIMARY	= 0x01,
	VIDEO_STREAM_TYPE_NETWORK	= 0x02,
	VIDEO_STREAM_TYPE_PHONE		= 0x03,
	VIDEO_STREAM_TYPE_AUDIO		= 0x04
}VIDEO_STREAM_TYPE;

//��λ���棬���ֻ��Ϊ32����
typedef enum _video_encode_mode
{
	VIDEO_ENCODE_MODE_VBR	=0x01,//������
	VIDEO_ENCODE_MODE_CBR	=0x02//�̶�����
}VIDEO_ENCODE_MODE;

typedef enum _osd_type
{
	OSD_CAMERA_NAME,
	OSD_TIME_STAMP,
	OSD_TEXT_INFO,
}OSD_TYPE;

typedef struct _osd_attrib
{
	unsigned short	xPos;		//
	unsigned short	yPos;		//
	unsigned short	cx;			//
	unsigned short	cy;			//
	unsigned char	*pData;		//
	unsigned long	length;		//
	unsigned long	second;
	unsigned long	usecond;
}OSD_ATTRIB;

typedef struct _frame_info_ex
{
	unsigned long		keyFrame;	//0 ���ǹؼ�֡

	unsigned long		frameType;
	unsigned long		length;
	unsigned long       width;
	unsigned long       height;

	unsigned char      *pData;

	unsigned long       channel;
	unsigned long       bufIndex;
	unsigned long		frameIndex;//֡����,���ģʽʱ��2��ʾ2���٣�4��ʾ4����
	unsigned long		frameAttrib;
	unsigned long		streamID;
	LONGLONG			time;	   //��֡����ʱ�䣬������ʱ������죬�ڸı��豸ʱ��ʱ��仯
	LONGLONG			relativeTime;//���ʱ�䣬�ڸı��豸ʱ��ʱ����仯���Ǹ�������ʱ��
}FRAME_INFO_EX;	//���ڱ���ʹ��

typedef struct _display_attrib
{
	unsigned short bShow;	//0��ʾ����ʾ��1��ʾ��ʾ�ֳ���2��ʾ��ʾ�ط�
	unsigned short chnn;	//ͨ����
	unsigned short xPos;	//���ϽǺ�����
	unsigned short yPos;	//���Ͻ�������
	unsigned short cx;		//��ʾ�����ȣ����أ�
	unsigned short cy;		//��ʾ����߶ȣ����أ�
	unsigned short zOrder;	//Z����ʾ˳��
	unsigned short index;	//������(��APPʹ��)
}DISPLAY_ATTRIB;

typedef struct _motion_area_info
{
	unsigned short gridWidth;	//ÿ���ж��ٸ�ջ��
	unsigned short gridHeight;	//ÿ���ж��ٸ�ջ��
	unsigned char  pData[640];	//((1280 / 16) * (1024 / 16)) / 8[sizeof(unsigned char)] = 640
}MOTION_AREA_INFO;

//�ṹ��
typedef struct _key_value_info
{
	unsigned long	type;
	unsigned long	isDown;
	unsigned long	value;
	unsigned long	xPos;
	unsigned long	yPos;
}KEY_VALUE_INFO;

typedef enum _mouse_key_value
{
	E_MOUSE_KEY_MOUSE_MOVE		=0x01,	//����ƶ�
	E_MOUSE_KEY_LEFT_DOWN		=0x02,	//�������
	E_MOUSE_KEY_LEFT_DOWN_MOVE	=0x03,	//��������϶�
	E_MOUSE_KEY_LEFT_UP			=0x04,	//�������
	E_MOUSE_KEY_LEFT_DBCLICK	=0x08,	//���˫��
	E_MOUSE_KEY_RIGHT_DOWN		=0x10,	//�Ҽ�����
	E_MOUSE_KEY_RIGHT_DOWN_MOVE	=0x11,	//�Ҽ������϶�
	E_MOUSE_KEY_RIGHT_UP		=0x20,	//�Ҽ�����
	E_MOUSE_KEY_RIGHT_DBCLICK 	=0x40,	//�Ҽ�˫��
	E_MOUSE_KEY_LEFTRIGHT_DOWN	=0x80,	//ͬʱ�������Ҽ�
	E_MOUSE_KEY_WHEEL_FORWARD	=0x100,	//��ǰ����
	E_MOUSE_KEY_WHEEL_BACK		=0x200,	//��󻬶�
	E_MOUSE_KEY_WHEEL_DOWN		=0x400,	//���ְ���
	E_MOUSE_KEY_WHEEL_UP		=0x800	//���ֵ���
}MOUSE_KEY_VALUE;

//��Ϣ����
typedef enum _message_type 
{
	KEY_TYPE_LOCAL		= 0x0001,	//��������
	KEY_TYPE_REMOTE		= 0x0002,	//ң�ؼ���
	KEY_TYPE_MOUSE		= 0x0004,	//���
	KEY_TYPE_MY1003		= 0x0008,	//����1003K����
	KEY_TYPE_VIRTUAL	= 0x0010,	//�������������ʱ��ʹ��	
	KEY_TYPE_TWOEM		= 0x0011,	//̨��OEM����485��Ӽ���
	KEY_TYPE_PL14_NVKBD30_70 = 0x0012, //PL14�ͻ���NV-KBD30/70
	KEY_TYPE_TWSY		= 0x0013,   //ͬΪϵͳ���Ƽ���
	KEY_TYPE_REMOTE_TR12= 0x0014,	//ͬΪ12���ң����TR12

}MESSAGE_TYPE;

typedef enum _display_mode
{
	DISPLAY_NONE,
	DISPLAY_LIVE,
	DISPLAY_PLAYBACK,
	DISPLAY_NET,
	DISPLAY_HIDE,
	DISPLAY_INVISIBLE
}DISPLAY_MODE;

typedef enum _video_out_type
{
	VIDEO_OUT_NONE,
	VIDEO_OUT_HDMI,
	VIDEO_OUT_VGA,
	VIDEO_OUT_CVBS
}VIDEO_OUT_TYPE;

typedef enum _pre_record_mode
{
	PRERECORD_MODE_MEMORY = 0,
	PRERECORD_MODE_DISK,
}PRERECORD_MODE;



#endif //_DEVICE_H_
