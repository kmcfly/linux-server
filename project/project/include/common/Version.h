#ifndef _VERSION_H_
#define _VERSION_H_
 
//此文件的有两个作用:
//第一用途：确定软件的版本号，级别，软件所属的升级包ID，这些信息都会记录
//到可执行文件里。用于升级的检查。
//通过编译开关确定：此软件的所属的升级包
//第二，在生成升级包校验文件时，会用从这里面得到软件的版本号，级别，软件所属的升级包ID
#ifdef __PACKID_1__
#define DVR_PACK_ID        "DvrPackID=1"
#define DVR_SDK_VERSION		"1.0.0.0"
#else
#ifdef __PACKID_2__
#define DVR_PACK_ID        "DvrPackID=2"
#define DVR_SDK_VERSION		"1.0.4.2"
#else
#ifdef __PACKID_3__
#define DVR_PACK_ID        "DvrPackID=3"
#define DVR_SDK_VERSION		"1.0.4.2"
#else
#ifdef __PACKID_4__
#define DVR_PACK_ID        "DvrPackID=4"
#define DVR_SDK_VERSION		"1.0.2.1"
#else
#ifdef __PACKID_5__
#define DVR_PACK_ID        "DvrPackID=3"
#define DVR_SDK_VERSION		"1.0.6.0.H"
#else
#ifdef __PACKID_6__
#define DVR_PACK_ID        "DvrPackID=4"
#define DVR_SDK_VERSION		"1.0.4.0"
#else
#ifdef __PACKID_7__
#define DVR_PACK_ID        "DvrPackID=3"
#define DVR_SDK_VERSION		"1.0.6.0.F"
#else
#ifdef __PACKID_8__
#define DVR_PACK_ID        "DvrPackID=8"
#define DVR_SDK_VERSION		"1.0.6.0.32"
#else
#ifdef __PACKID_9__
#define DVR_PACK_ID        "DvrPackID=9"
#define DVR_SDK_VERSION		"1.0.6.0.HDMI"
#else
#ifdef __PACKID_10__
#define DVR_PACK_ID        "DvrPackID=10"
#define DVR_SDK_VERSION		"FH"
#else
#ifdef __PACKID_11__
#define DVR_PACK_ID        "DvrPackID=11"
#define DVR_SDK_VERSION		"NXP"
#else
#ifdef __PACKID_12__
#define DVR_PACK_ID        "DvrPackID=12"
#define DVR_SDK_VERSION		"3531"
#else
#ifdef __PACKID_13__
#define DVR_PACK_ID        "DvrPackID=13"
#define DVR_SDK_VERSION		"1.0.6.0.960"
#else
#ifdef __PACKID_14__
#define DVR_PACK_ID        "DvrPackID=14"
#define DVR_SDK_VERSION		"3531SDI"
#else
#ifdef __PACKID_15__
#define DVR_PACK_ID        "DvrPackID=15"
#define DVR_SDK_VERSION		"3521"
#else
#ifdef __PACKID_16__
#define DVR_PACK_ID        "DvrPackID=16"
#define DVR_SDK_VERSION		"3520A"
#else
#ifdef __PACKID_17__
#define DVR_PACK_ID        "DvrPackID=17"
#define DVR_SDK_VERSION		"TI"
#else
#ifdef __PACKID_20__
#define DVR_PACK_ID        "DvrPackID=20"
#define DVR_SDK_VERSION		"1.0.6.0.N2"
#else
#ifdef __PACKID_21__		//3520A做的NVR
#define DVR_PACK_ID        "DvrPackID=21"
#define DVR_SDK_VERSION		"1.0.6.0.N3A"
#else
#ifdef __PACKID_22__
#define DVR_PACK_ID        "DvrPackID=22"
#define DVR_SDK_VERSION		"3520D"
#else
#ifdef __PACKID_24__		//3521做的NVR
#define DVR_PACK_ID        "DvrPackID=24"
#define DVR_SDK_VERSION		"1.0.6.0.N4"
#else
#ifdef __PACKID_25__
#define DVR_PACK_ID        "DvrPackID=25"
#define DVR_SDK_VERSION		"1.0.6.0.N32"
#else
#ifdef __PACKID_27__		//3521做的NVR
#define DVR_PACK_ID        "DvrPackID=27"
#define DVR_SDK_VERSION		"GM8210"
#else
#define DVR_PACK_ID        "DvrPackID=X"
#define DVR_SDK_VERSION		"x.x.x.x"
#endif	//__PACKID_27__
#endif	//__PACKID_25__
#endif	//__PACKID_24__
#endif	//__PACKID_22__
#endif //__PACKID_21__
#endif //__PACKID_20__
#endif	//__PACKID_17__
#endif	 //__PACKID_16__
#endif   //__PACKID_15__
#endif   //__PACKID_14__
#endif   //__PACKID_13__
#endif   //__PACKID_12__
#endif   //__PACKID_11__
#endif   //__PACKID_10__
#endif   //__PACKID_9__
#endif   //__PACKID_8__
#endif   //__PACKID_7__
#endif  //__PACKID_6__
#endif  //__PACKID_5__ 
#endif  //__PACKID_4__
#endif  //__PACKID_3__ 
#endif //__PACKID_2__
#endif //__PACKID_1__

//客户端版本的子版本定义
#if defined(__CUSTOM_ZNV__)
	#if defined(__ZNV_CN__)
		#if defined(__ZNV_SC__)
			#define DVR_SUB_CUSTOM_ID	"SC"
		#else
			#define DVR_SUB_CUSTOM_ID	"CN"
		#endif
	#elif defined(__ZNV_WNRL__)
		#define DVR_SUB_CUSTOM_ID	"WN"
	#else
		#define DVR_SUB_CUSTOM_ID	"IN"
	#endif
#else
#define DVR_SUB_CUSTOM_ID	""
#endif

//需要手动修改,软件的版本号
#if defined(__DVR_DEBUG__)
#define DVR_SOFT_VERSION_M  "DvrSoftVersionM=3.4.4-beta"
#else
#define DVR_SOFT_VERSION_M  "DvrSoftVersionM=3.4.4-beta"
#endif

//通过编译开关确定：软件的级别
#ifndef __RELEASE__ 
#define DVR_SOFT_LEVEL     "DvrSoftVerLevel=D"
#else
#ifndef NDEBUG
#define DVR_SOFT_LEVEL     "DvrSoftVerLevel=T1"
#else
#ifdef __DVR_ULTIMATE__    
#define DVR_SOFT_LEVEL     "DvrSoftVerLevel=U"
#else
#ifdef __DVR_PROFESSIONAL__
#define DVR_SOFT_LEVEL     "DvrSoftVerLevel=P"
#else
#ifdef  __DVR_BASIC__
#define DVR_SOFT_LEVEL     "DvrSoftVerLevel=B"
#else
#ifdef __DVR_COSTDOWN__
#define DVR_SOFT_LEVEL     "DvrSoftVerLevel=C"
#else
#define DVR_SOFT_LEVEL     "DvrSoftVerLevel=X"
#endif  // __DVR_COSTDOWN__  
#endif  // __DVR_BASIC__         
#endif  // __DVR_PROFESSIONAL__           
#endif  // __DVR_ULTIMATE__
#endif  //NDEBUG
#endif  //__RELEASE__ 


#ifdef __CUSTOM_IN__
#define DVR_CUSTOM_ID 0
#else
#ifdef __CUSTOM_CN__
#define DVR_CUSTOM_ID 1
#else
#ifdef __CUSTOM_SP__
#define DVR_CUSTOM_ID 2
#else
#ifdef __CUSTOM_US02__
#define DVR_CUSTOM_ID 3
#else
#ifdef __CUSTOM_RU04__
#define DVR_CUSTOM_ID 4
#else
#ifdef __CUSTOM_BR27__
#define DVR_CUSTOM_ID 5
#else
#ifdef __CUSTOM_TW01__
#define DVR_CUSTOM_ID 6
#else
#ifdef __CUSTOM_GB14__
#define DVR_CUSTOM_ID 7
#else
#ifdef __CUSTOM_IN23__
#define DVR_CUSTOM_ID 8
#else
#ifdef __CUSTOM_US47__
#define  DVR_CUSTOM_ID 9
#else
#ifdef __CUSTOM_IN25__
#define DVR_CUSTOM_ID 10
#else
#ifdef __CUSTOM_PL14__
#define  DVR_CUSTOM_ID 11
#else
#ifdef __CUSTOM_TW01TS__
#define  DVR_CUSTOM_ID  12
#else
#ifdef __CUSTOM_IL03__
#define  DVR_CUSTOM_ID   13
#else
#ifdef __CUSTOM_LV01__
#define  DVR_CUSTOM_ID   14
#else
#ifdef __CUSTOM_SHANGHAI__
#define DVR_CUSTOM_ID   15
#else
#ifdef __CUSTOM_CBC__
#define DVR_CUSTOM_ID   16
#else
#ifdef __CUSTOM_GB31__
#define DVR_CUSTOM_ID   17
#else
#ifdef __CUSTOM_ZNV__
#define DVR_CUSTOM_ID   18
#else
#ifdef __CUSTOM_CNB__
#define DVR_CUSTOM_ID   19
#else
#define DVR_CUSTOM_ID 99
#endif	//__CUSTOM_CNB__
#endif	//__CUSTOM_ZNV__
#endif  //__CUSTOM_GB31__
#endif  //__CUSTOM_CBC__
#endif  //__CUSTOM_SHANGHAI__
#endif  //__CUSTOM_LV01__
#endif  //__CUSTOM_IL03__
#endif  //__CUSTOM_TW01TS__
#endif  //__CUSTOM_PL14__
#endif  //__CUSTOM_IN25__
#endif  //__CUSTOM_US47__
#endif  //__CUSTOM_IN23__
#endif  //__CUSTOM_GB14__
#endif  //__CUSTOM_TW01__
#endif  //__CUSTOM_BR27__
#endif  //__CUSTOM_RU04__
#endif  //__CUSTOM_US02__
#endif  //__CUSTOM_SP__
#endif  //__CUSTOM_CN__
#endif  //__CUSTOM_IN__


inline unsigned long FirmwareVersion()
{
	const char *pVer = strstr(DVR_SOFT_VERSION_M, "=");
	if (NULL != pVer)
	{
		unsigned char len = strlen(pVer);
		if (len > strlen("3.0.0"))
		{
			unsigned long version = 0;

			//3.x.x
			version |= (0xff000000 & (atoi(pVer+1) << 24));
			++ pVer;

			//x.0.x
			pVer = strstr(pVer, ".");
			version |= (0x00ff0000 & (atoi(pVer+1) << 16));
			++ pVer;

			//x.x.0
			pVer = strstr(pVer, ".");
			version |= (0x0000ffff & atoi(pVer+1));

			return version;
		}
		else
		{
			return 0;
		}
	}
	else
	{
		return 0;
	}
}

#endif //_version_h_


