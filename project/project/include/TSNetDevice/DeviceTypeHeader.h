

/////////////////////////DeviceTypeHeader.h
/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : 张国军
** Date         : 2008-04-26
** Name         : DeviceTypeHeader.h
** Version      : 1.0
** Description  :
** Modify Record:
1:2008-1-14 csg 控制中心所有支持的设备类型的定义,说明控制中心兼容多种设备类型的处理方式。
		
***********************************************************************/

/*	
	此模块主要目标是定义，是考虑到所有控制中心支持的设备类型必须统一定义

*/


#ifndef	_DEVICE_TYPE_H_
#define _DEVICE_TYPE_H_

namespace DEVICE_TYPE_DEF
{
  	/************************************************************************/                                                                  
	/* 	设备类型信息的数据头	*/
	/************************************************************************/
	const unsigned long DEVICE_TYPE_HEADFLAG = ( (unsigned long)(unsigned char)('h') | ( (unsigned long)(unsigned char)('e') << 8 ) |	\
		( (unsigned long)(unsigned char)('a') << 16 ) | ( (unsigned long)(unsigned char)('d') << 24 ) );		
		


  	/************************************************************************/                                                                  
	/* 
		此设备类型定义的标准为:  设备类型_系列_通道数量
		设备类型
			DVS/IPCAMERA/DVR/DVRCARD/CMS/DECODER(解码器或解码卡)
		系列:
			DVS: 11系列
			IPCAMERA: 11、12系列
			DVR:20、23、24系列
			DVRCARD：30、31、33、41系列	 */
	/************************************************************************/
	typedef enum _tvt_product_type_
	{
		TVT_PRODUCT_DVR,			//嵌入式硬盘录像机
		TVT_PRODUCT_DVS,			//嵌入式视频服务器
		TVT_PRODUCT_IPC,			//嵌入式网络摄像机
		TVT_PRODUCT_NVR,			//嵌入式网络硬盘录像机
		TVT_PRODUCT_MDVR,			//嵌入式车载硬盘录像机
		TVT_PRODUCT_DECODEC,		//嵌入式解码器
		TVT_PRODUCT_STORAGE,		//存储器
		TVT_PRODUCT_INTELLIGENCE,    //智能分析模块
		TVT_PRODUCT_NETKEYBOARD,    //网络键盘
		TVT_PRODUCT_CMS,			//单机版控制中心
		TVT_PRODUCT_ECMS,			//专业版控制平台
		TVT_PRODUCT_CLIENT,			//网络客户端
		TVT_PRODUCT_MOBILE_APP,		//移动客户端
	}TVT_PRODUCT_TYPE;

	//////////嵌入式DVR的产品类型定义
	enum PRODUCT_DVR
	{
		TD2304L=0,//原来使用的是0，一直必须是0
		TD2304=3,
		TD2308,
		TD2316,
		TD2404,
		TD2408,
		TD2416,
		TD2304M,//9
		TD2308M,
		TD2316M,
		TD2504=21,
		TD2508,
		TD2516,
		TD2316C,
		TD5004,		//四路车载机
		TD5008,		//八路车载机
	};  

	////DVS的产品类型定义
	enum PRODUCT_DVS
	{		
		TS_1101_VSA,
		TS_1104,
		TS_1101_VSB,
		TS_1201,
		TS_1202,
		TS_1204,
	};

	///////IPCamera的产品类型定义
	enum PRODUCT_IPCAMERA
	{
		TC_1118,
		TC_1218	,
		TC_201CAB,//30万像素
		TC_211CAB,//100万像素
		TC_301CAB,//CCD	
		TC_211HCAB,//130万像素
		TC_100,//只指装CCD的模组，其它再定义
		TC_201L,//201 8M FLASH的产品类型定义，避免互相升级
		TC_612M,//211的枪机产品,参数和211完全一样
		TC_221,//8M FLASH,200万像素,1600X1200分辨率
		TC_622,//和TC_221参数一样,枪机产品,TC622M
		TC_20l_G3,//重庆移动定制产品，CMS、SDK、客户端都不需支持，此产品直接接入平台的
		TC_202,//20100112内核、armboot同TC100
		TC_562,//G版本500MHZ的cpu，类型同TC100

		//下面产品的方案用的是海思的
		TC_612F=1000,
		TC_622F,
		TD_9322,
		TD_9321,
		TD_9422,
		TD_9421,
		TD_9522,
		TD_9521,
		TD_9322D,
		HI_END= 1099,

		//下面的产品是8126的芯片
		TD_9321L = 1100,		//cmos 720P
		TD_9314,				//CCD 30万
		TD_9421L,
		TD_9411,
	};  

	/////SuperDVR的产品类型定义
	enum  PRODUCT_SUPERDVR
	{
		DVR_CARD_TDNULL,
		DVR_CARD_TD3004,
 		DVR_CARD_TD3016,
		DVR_CARD_TD3116,
		DVR_CARD_TD3216,
		DVR_CARD_TD3108,
		DVR_CARD_TD3008,
		DVR_CARD_TD3316,
		DVR_CARD_TD3101,
		DVR_CARD_TD3104,
		DVR_CARD_TD4104,
		DVR_CARD_TD3316V1,
		DVR_CARD_TD4116,
		DVR_CARD_TD4108,
		DVR_CARD_TD4408,
		DVR_CARD_TD3304,
		DVR_CARD_TD4404,
		DVR_CARD_TD4308,
		DVR_CARD_TD4316,
		DVR_CARD_TD4304,
		DVR_CARD_TD4404L,
		DVR_CARD_TD4408L,
		DVR_CARD_TD4416L,
		DVR_CARD_TD3304NC
	};

	enum PRODUCT_DECODER
	{
		DECODER_1101D
	};
	
	/************************************************************************/
	/*   控制中心连接上设备后会先收到下面的结构体信息进行解析，通过设备类型创建相应的  
	服务对象，同时会把这64个字节的信息直接通过参数传递给服务对象*/                                                                     
	/************************************************************************/	
	typedef struct _net_device_type_info
	{
		unsigned long headFlag; ////取值为HEADFLAG
		unsigned long devicetype; /////////设备类型 DVS  IPCAMERA  DVR  SUPERDVR
		unsigned long producttype;  /////产品类型
		unsigned long netprotrocolver; ///////////设备对应的网络协议版本
		unsigned long configver; ///////////设备对应的配置版本
		unsigned long devID; //////
		unsigned long encryptType;	//数据加密方式，0 表示没有加密方式
		unsigned long encryptParam;	
		unsigned long ulReserve[7];/////保留信息，以便以后扩充
		unsigned long dwUDPDefaultVideoStream;	//用于穿透默认的流
	}NETDEVICE_TYPE_INFO, *LPNETDEVICE_TYPE_INFO;

}  ////////////namespace DEVICE_TYPE_DEF end

//IPCamera 的加密方式
//encryptType 为1.0的时候，为最简单的只对用户名和用户密码进行加密，
//encryptParam 为加密用到的参数，1.0的时候就做简单的异或运算

//huxinhua add begin
//struct for iptool to update u-boot and kernel
typedef struct _ipcamera_version_check_info
{
    unsigned long checkflag;       //DEVICE_TYPE_HEADFLAG
    unsigned long packagetype;    // u-boot 1, kernel 2, update package 3(not use now)
    unsigned long devicetype;     // IPCAMERA    DVS    DVR   ...
    unsigned long producttype;    // 9322  9321 .....
}IPCAMERA_VERSION_CHECK_INFO;
//huxinhua add end

#endif /////#ifndef _DEVICE_TYPE_H_








