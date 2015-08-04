//定义数据类型，为跨平台开发服务服务
#ifndef _DVRTYPEDEFINE_
#define _DVRTYPEDEFINE_

#ifdef __ENVIRONMENT_WIN32__ ////////////////////如果是Windows平台
#include "stdafx.h"

//定义PACKED 主要用于解决在Windows解决对奇一般使用#pragma pack(n)而Linux下一般使用__attribute__((packed))
//在此结构体要写入文件或者在跨平台之间访问时才需要以下定义，注意__attribute__((packed))只用于单字节对齐
#define PACKED
#else ////////////////////linux平台
#define PACKED __attribute__((packed))

typedef unsigned long       DWORD;
typedef unsigned long       ULONG;

typedef long                LONG;
typedef unsigned long long  ULONGLONG;
typedef int                 BOOL;
typedef unsigned char       BYTE;
typedef unsigned short      WORD;
typedef int                 INT;
typedef unsigned int        UINT;
typedef long long           LONGLONG; 

//以下是为了驱动库的定义使用方便
typedef	signed char			SBYTE;

#include <sys/time.h>
typedef timeval             DVRDATETIME;

#endif ////////#ifdef __ENVIRONMENT_WIN32__ //如果是Windows平台

#endif ////////#ifndef _DVRTYPEDEFINE_





