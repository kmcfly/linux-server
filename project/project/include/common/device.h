/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : 袁石维
** Date         : 2009-06-30
** Name         : device.h
** Version      : 1.0
** Description  :
** Modify Record:
1: 这个文件我们使用，刘志高哪里也有一个，这个两份本应该是一致的。但是
  由于在结构体DISPLAY_ATTRIB发生变化时，当时没有即时通知刘志高进行同步修改，
  随之时间的推移，两份不能同步到一起了。如果有变化，就相互通知对方修改。（2011040916）
***********************************************************************/
#ifndef _DEVICE_H_
#define _DEVICE_H_

//产品类型定义
typedef enum _product_type
{
	TD_DEVICE_NONE = 0,
	TD_2304SE_S = 100,      //不存在该产品
	TD_2308SE_S,            //不存在该产品
	TD_2316SE_S,            //3515做的16CIF
	TD_2324SE_S,            //3520做的24路

	TD_2304SE_B = 120,
	TD_2308SE_B,
	TD_2316SE_B,

	TD_2304CDS = 150,       //3515做的costdown4路,市场叫做2304SS
	TD_2308CDS,             //3515做的costdown8路,市场叫做2308SS
	TD_2316CDS,             //3515，1108A做的16路，市场叫做2316SS

	TD_2304SS_B = 160,		//3521 4路D1，没有单片机
	TD_2308SS_B,			//3521 8路D1，没有单片机
	TD_2316SS_B,			//3521 16路D1，没有单片机

	TD_2304SS_SL = 165,		//TI 4路D1
	TD_2308SS_SL,			//TI 8路D1

	TD_2304SL_C = 170,		//3515A	4路960H
	TD_2308SL_C,			//3520D	8路960H

	TD_2304HS = 175,        //3515做的costdown4路，D1实时产品

	TD_2304SS_C = 180,		//3520A做的4路实时960H
	TD_2308SS_C,			//3520A做的8X15F960H,8X30FD1
	TD_2316SS_C,			//3520A做的16X7F960H,16X30FCIF
	TD_2304SS_SL_3515A,			//3515A做的，用的是3520D的包

	TD_2304SE_C = 185,		//3520A做的4路实时960H
	TD_2308SE_C,			//3520A做的8X15F960H,8X30FD1
	TD_2316SE_C,			//3521做的16X7F960H,16X30FD1

	TD_2304SS_SDI = 190,	//3520D SDIX1+CVBSX3
	TD_2308SS_SDI,			//3521  SDIX2+CVBSX6

	TD_2304SE	= 200,      //3515做的4路
	TD_2308SE,              //3515做的8路
	TD_2316SE,              //3515做的16路

	TD_2304ME	= 300,      //3515做的4路
	TD_2308ME,              //3515做的8路
	TD_2316ME,              //3520做的16路实时CIF

	TD_2304ME_B		= 310,	//
	TD_2308ME_B,			//
	TD_2316ME_B,			//

	TD_2304ME_BH	= 320,	//
	TD_2308ME_BH	= 321,	//
	TD_2316ME_BH	= 322,	//

	TD_2304ME_A = 350,      //fullhan平台做的产品,目前没有，做预留用
	TD_2308ME_A = 351,      //fullhan平台做的产品, 2D1+6CIF
	TD_2316ME_A = 352,      //fullhan平台做的产品,	2D1+14CIF


	TD_2304ME_C = 360,		//3520A 慢速960H
	TD_2308ME_C,
	TD_2316ME_C,

	TD_2304HE	= 400,      //不存在该产品
	TD_2308HE,              //不存在该产品
	TD_2316HE,              //不存在该产品

	TD_2404MD	= 500,      //3520做的4路
	TD_2408MD_4A,           //3520做的8路，音频4路，曾经有过该产品，目前已经没有了，但是不能删除
	TD_2416MD_4A,           //3520做的16路，音频4路，曾经有过该产品，目前已经没有了，但是不能删除
	TD_2432MD_4A,           //不存在该产品
	TD_2408MD,              //3520做的8路,音频8路
	TD_2416MD,              //3520做的16路,音频16路
	TD_2424MD,              //3520做的24路,音频16路
	TD_2432MD,              //3520做的32路,音频16路

	TD_2404MD_B	= 520,     //3531做的4路D1
	TD_2408MD_B,           //3531做的4路D1+4路CIF
	TD_2416MD_B,           //3531做的4路D1+12路CIF
	TD_2424MD_B,           //3531做的16路D1+8路CIF
	TD_2432MD_B,           //3531做的16路D1+16路CIF

	TD_2504ME	= 600,       //不存在该产品
	TD_2508ME,               //不存在该产品 
	TD_2516ME,               //不存在该产品

	TD_2504HE	= 700,        //3520，D1实时
	TD_2508HE,                //3520，D1实时
	TD_2516HE,                //3520，D1实时
	TD_2504HE_M,              //3515，D1实时，市场叫做2304HE

	//TD_2504HC_B = 710,	//FH 4D1
	TD_2508HC_B	= 711,		//3531 8D1
	TD_2516HC_B,			//3531 16D1

	TD_2504HE_B = 720,		//FH 4D1, FH 的 TD_2504HC_B产品和这个产品用同一个ID	
	TD_2508HE_B,
	TD_2516HE_B,			//3531平台做的16D1产品
	TD_2508HE_B_3521,		//3521平台做的8D1产品
	TD_2524HE_B,
	TD_2532HE_B,			//3531平台做的32D1产品

	TD_2504HE_L = 725,		//3515 4D1
	TD_2508HE_L,			//FH 8D1
	TD_2516HE_L,			//3531 16D1

	TD_2504HE_B_N100 = 730,
	TD_2508HE_B_N100,
	TD_2516HE_B_N100,		//3531平台做的16D1产品, 100M网卡，没有单片机,其他和TD_2516HE_B一样, HS-B

	TD_2508HE_SDI = 735,	//3521 SDIX2+CVBSX6  720P
	TD_2512HE_SDI,			//3531 SDIX4+CVBSX8  720P
	TD_2524HE_SDI,			//3531 SDIX4+CVBSX20

	TD_2504HC	= 750,        //3520，D1实时
	TD_2508HC,                //3520，D1实时
	TD_2516HC,                //3520，D1实时
	
	TD_2504HS_BL= 760,		  //3520A
	TD_2508HS_BL,			  //3520A
	TD_2516HS_BL,			  //3520A

	TD_2532HD_SDI = 770,		//3531 SDIX4+CVBSX28

	TD_2504MD	= 800,         //不存在该产品 这个字段不能用了
	TD_2508MD,                 //不存在该产品 这个字段不能用了
	TD_2516MD,                 //不存在该产品 这个字段不能用了	

	TD_2504HD	= 900,         //3520，D1实时
	TD_2508HD_4A,              //不存在该产品 
	TD_2516HD_4A,              //不存在该产品 
	TD_2508HD,                 //3520，D1实时
	TD_2516HD,                 //3520，D1实时

	TD_2504HD_C = 910,
	TD_2508HD_C,
	TD_2516HD_C,
	TD_2524HD_C,
	TD_2532HD_C,

	TD_2504FD	= 1000,        //3520，D1实时
	TD_2508FD_4A,              //不存在该产品
	TD_2516FD_4A,              //不存在该产品
	TD_2508FD,                 //3520，D1实时
	TD_2516FD,                 //3520，D1实时
	TD_2516FD_IP,              //2516FD做的hybrid产品

	TD_NXP04    = 1050,        //预留nxp平台的产品
	TD_NXP08,                  //预留nxp平台的产品
	TD_2516FD_S,               //nxp平台的产品

	//
	TD_2604LE	= 10000,       //3531/3532平台
	TD_2608LE,
	TD_2616LE,				   //就是市场上的2516HS_C
	TD_2508HE_C_3521,
	TD_2524HE_C,			   //用2524HE_B主板改加密
	TD_2532HE_C,			   //用2532HE_B主板改加密

	TD_2604LD_S	= 10100,       //3520  960H平台
	TD_2608LD_S,
	TD_2616LD_S,

	TD_2604LD	= 10200,       //3531/3532 960H平台
	TD_2608LD,
	TD_2616LD,
	TD_2616LD_IP,
	

	TD_2704HD	= 10600,       //就是 2708XDF

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

	TD_5104C    = 10700,       //车载
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
	TD_2808NE_A = 20012,	//原2816NE-L更名为2808NE-A
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

	TD_7004SB	= 21000,       //市场上叫做7004
	TD_7008SB,                 //市场上叫做7008

	TD_S201		= 30000,			//存储


	TD_2816NS_C		= 50000,	//3535 16路NVR,		支持三个产品，28040816NS_C		一个加密，在原来硬件版本后添加：A、B，C，D，E，F分别代表4,8,16,24,32,64路设备
	TD_2804NS_C,
	TD_2808NS_C,

	TD_3004D1_4P	= 50005,	//3520D
	TD_3008D1_8P,

	TD_2832PD_C16	= 50010,	//3535 16+16路NVR,	支持一个产品，2832PD_C16		一个加密，在原来硬件版本后添加：A、B，C，D，E，F分别代表4,8,16,24,32,64路设备
	
	TD_2832NE_C		= 50020,	//3535 32路NVR，	支持三个产品，28081632NE_C		一个加密，在原来硬件版本后添加：A、B，C，D，E，F分别代表4,8,16,24,32,64路设备
	TD_2808NE_C,
	TD_2816NE_C,

	TD_2816ND_C		= 50030,	//3535 32路NVR，	支持两个产品，281632ND_C		一个加密，在原来硬件版本后添加：A、B，C，D，E，F分别代表4,8,16,24,32,64路设备
	TD_2832ND_C,

	TD_2804PS_C4	= 50040,	//3535 4 + 4路NVR，	支持两个产品，280408PS_C4		一个加密，在原来硬件版本后添加：A、B，C，D，E，F分别代表4,8,16,24,32,64路设备
	TD_2808PS_C4,

	TD_2808PS_C8	= 50050,	//3535 8 + 8路NVR，	支持两个产品，280816PS_C8		一个加密，在原来硬件版本后添加：A、B，C，D，E，F分别代表4,8,16,24,32,64路设备
	TD_2816PS_C8,

	TD_2816PE_C8	= 50060,	//3535 24 + 8路NVR，支持两个产品，281632PE_C8		一个加密，在原来硬件版本后添加：A、B，C，D，E，F分别代表4,8,16,24,32,64路设备
	TD_2832PE_C8,
}PRODUCT_TYPE;

typedef enum _product_sub_id
{
	TD_TVT_IN = 0,	//国际标准版
	TD_TVT_CN = 1,	//国内标准版
	TD_TVT_SP = 2, //特殊版
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
	TD_TVT_ZNV,    //中兴力维
	TD_TVT_CNB,		//CNB版本
}PRODUCT_SUB_ID;

//视频制式定义
typedef enum _video_format
{
	VIDEO_FORMAT_NTSC	= 0x00,
	VIDEO_FORMAT_PAL	= 0x01,
	VIDEO_FORMAT_NONE	= 0xff,		//无效的格式
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
	PLAY_FRAME_NO_SHOW			= 0x01, //此帧不显示
	PLAY_FRAME_SHOW				= 0x02, //此帧可显示
	PLAY_FRAME_ALL_END			= 0x04, //数据读取结束了，后面再没有数据
	PLAY_FRAME_SEC_END			= 0x08, //此事件段结束了
	PLAY_FRAME_TIME_STAMP		= 0x10, //此帧数据有时间戳，抓图的时候注意要屏蔽打时间的功能
	LIVE_FRAME_FIRST_STREAM		= 0x20, //此帧为现场主码流
	LIVE_FRAME_SECOND_STREAM	= 0x40, //此帧为现场子码流
	PLAY_FRAME_FF				= 0x80,	//当前帧用于快进
	LIVE_FRAME_JPEG				= 0x100,
	LIVE_FRAME_TALK				= 0x200
}FRAME_ATTRIB;

//按位保存。最多不能超过32个
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

//按位保存，最多只能为32个。
typedef enum _video_encode_mode
{
	VIDEO_ENCODE_MODE_VBR	=0x01,//变码流
	VIDEO_ENCODE_MODE_CBR	=0x02//固定码流
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
	unsigned long		keyFrame;	//0 不是关键帧

	unsigned long		frameType;
	unsigned long		length;
	unsigned long       width;
	unsigned long       height;

	unsigned char      *pData;

	unsigned long       channel;
	unsigned long       bufIndex;
	unsigned long		frameIndex;//帧索引,快进模式时，2表示2倍速，4表示4倍速
	unsigned long		frameAttrib;
	unsigned long		streamID;
	LONGLONG			time;	   //在帧绝对时间，年月日时分秒浩渺，在改变设备时间时会变化
	LONGLONG			relativeTime;//相对时间，在改变设备时间时不会变化，是个连续的时间
}FRAME_INFO_EX;	//用于本地使用

typedef struct _display_attrib
{
	unsigned short bShow;	//0表示不显示，1表示显示现场，2表示显示回放
	unsigned short chnn;	//通道号
	unsigned short xPos;	//左上角横坐标
	unsigned short yPos;	//左上角纵坐标
	unsigned short cx;		//显示区域宽度（像素）
	unsigned short cy;		//显示区域高度（像素）
	unsigned short zOrder;	//Z轴显示顺序
	unsigned short index;	//区域编号(仅APP使用)
}DISPLAY_ATTRIB;

typedef struct _motion_area_info
{
	unsigned short gridWidth;	//每行有多少个栈格
	unsigned short gridHeight;	//每列有多少个栈格
	unsigned char  pData[640];	//((1280 / 16) * (1024 / 16)) / 8[sizeof(unsigned char)] = 640
}MOTION_AREA_INFO;

//结构体
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
	E_MOUSE_KEY_MOUSE_MOVE		=0x01,	//鼠标移动
	E_MOUSE_KEY_LEFT_DOWN		=0x02,	//左键按下
	E_MOUSE_KEY_LEFT_DOWN_MOVE	=0x03,	//左键按下拖动
	E_MOUSE_KEY_LEFT_UP			=0x04,	//左键弹起
	E_MOUSE_KEY_LEFT_DBCLICK	=0x08,	//左键双击
	E_MOUSE_KEY_RIGHT_DOWN		=0x10,	//右键按下
	E_MOUSE_KEY_RIGHT_DOWN_MOVE	=0x11,	//右键按下拖动
	E_MOUSE_KEY_RIGHT_UP		=0x20,	//右键弹起
	E_MOUSE_KEY_RIGHT_DBCLICK 	=0x40,	//右键双击
	E_MOUSE_KEY_LEFTRIGHT_DOWN	=0x80,	//同时按下左右键
	E_MOUSE_KEY_WHEEL_FORWARD	=0x100,	//向前滑动
	E_MOUSE_KEY_WHEEL_BACK		=0x200,	//向后滑动
	E_MOUSE_KEY_WHEEL_DOWN		=0x400,	//滑轮按下
	E_MOUSE_KEY_WHEEL_UP		=0x800	//滑轮弹起
}MOUSE_KEY_VALUE;

//消息类型
typedef enum _message_type 
{
	KEY_TYPE_LOCAL		= 0x0001,	//机器键盘
	KEY_TYPE_REMOTE		= 0x0002,	//遥控键盘
	KEY_TYPE_MOUSE		= 0x0004,	//鼠标
	KEY_TYPE_MY1003		= 0x0008,	//名扬1003K键盘
	KEY_TYPE_VIRTUAL	= 0x0010,	//虚拟键，在特殊时候使用	
	KEY_TYPE_TWOEM		= 0x0011,	//台湾OEM厂商485外接键盘
	KEY_TYPE_PL14_NVKBD30_70 = 0x0012, //PL14客户，NV-KBD30/70
	KEY_TYPE_TWSY		= 0x0013,   //同为系统控制键盘
	KEY_TYPE_REMOTE_TR12= 0x0014,	//同为12年的遥控器TR12

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
