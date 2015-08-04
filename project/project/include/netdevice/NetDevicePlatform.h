#ifndef _NET_DEVICE_PLATFORM_H_
#define _NET_DEVICE_PLATFORM_H_

//////////////////////////////////////////////////////////////////////////
/*
 * Win32宏 : __ENVIRONMENT_WIN32__
 * Linux宏 : __ENVIRONMENT_LINUX__
*/
//////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus   
#define EXTERN_C           extern   "C"   
#else   
#define EXTERN_C           extern   
#endif   

#ifdef __ENVIRONMENT_WIN32__

#ifdef NETDEVICE_EXPORT
#define ND_API __declspec(dllexport)
#else
#define ND_API __declspec(dllimport)
#endif

#else

#define ND_API

#endif

//参数输入, 输出 类型
#define nd_in
#define nd_out
#define nd_in_out

//////////////////////////////////////////////////////////////////////////
#ifdef __ENVIRONMENT_WIN32__
#pragma pack(4)
#endif

//标准定义
#include "dvrdvstypedef.h"
#include "dvrdvsdefine.h"
#include "dvrdvsconfig.h"

//云台
#include "miss/PTZ.h"

//Net Device 专有定义
#include "NetDeviceTypedef.h"
#include "NetDeviceDefine.h"

//接口文件
#include "NetDevice.h"

#ifdef	__ENVIRONMENT_WIN32__
#pragma pack()
#endif

//Debug
#ifdef __ENVIRONMENT_WIN32__
#ifndef printf
#define printf					TRACE
#endif
#endif

#ifndef NDEBUG
#define nd_printf(_nd_printf)					(_nd_printf)
#define nd_assert(_nd_expression, _nd_printf)	((!(_nd_expression)) ? (_nd_printf), assert(_nd_expression) : ((void)0))
#else
#define nd_printf(_nd_printf)					((void)0)
#define nd_assert(_nd_expression, _nd_printf)	((void)0)
#endif

//////////////////////////////////////////////////////////////////////////

//初始化, 退出库
EXTERN_C ND_API int			nd_init();
EXTERN_C ND_API int			nd_quit();

//获取库信息,成功返回0
EXTERN_C ND_API int			nd_get_info(nd_out ND_LIB_INFO* p_lib_info);

//搜索局域网设备，成功返回当前搜索到的设备数目
EXTERN_C ND_API int			nd_get_discovered_device_num();
//获取搜索到的设备信息，成功返回设备数目
EXTERN_C ND_API int			nd_get_discovered_device_info(nd_out ND_DEVICE_INFO* p_device_info, nd_in int deviceNum);

//设置搜索到的设备IP地址信息，无返回
EXTERN_C ND_API void		nd_set_discovered_device_info(nd_in ND_DEVICE_NETWORK_INFO * pNetworkInfo);

//定时处理
EXTERN_C ND_API void		nd_on_timer(nd_in unsigned long current_time);

//创建, 释放 接口对象
EXTERN_C ND_API CNetDevice* nd_create();
EXTERN_C ND_API void		nd_destroy(nd_in CNetDevice* p_net_device);


//////////////////////////////////////////////////////////////////////////

#endif
//end
