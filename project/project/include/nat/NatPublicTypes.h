// NatPublicTypes.h: interface for the public types of nat.
//
//////////////////////////////////////////////////////////////////////
#ifndef __NAT_PUBLIC_TYPES_H__
#define __NAT_PUBLIC_TYPES_H__

#include "NatUserDefine.h"

#ifdef NAT_TVT_DVR_4_0
#include "TVT_PubCom.h"
#else
#include "PUB_common.h"
#endif//NAT_TVT_DVR_4_0

#include "SWL_Public.h"
/**
 * 穿透服务器信息，主要作为穿透服务器列表项
 */
typedef struct __nat_server_list_info__
{
    char                szAddr[64];     // 服务器地址
    unsigned short      sPort;          // 穿透端口号
    unsigned short      sTestPort;      // 穿透测试端口号 
}NAT_SERVER_LIST_INFO;


#define NAT_SERVER_PORT			8989 
#define NAT_SERVER_TEST_PORT	8993

/**
 * 设备版本类型
 */
enum NAT_DEVICE_VER_TYPE
{
	NAT_DEVICE_VER_3			= 3,			/**< 3.0设备类型 */
	NAT_DEVICE_VER_4			= 4,			/**< 4.0设备类型 */
	NAT_DEVICE_VER_IPC			= 10001,		/**< IPC设备类型 */
	NAT_DEVICE_VER_NVMS_9000	= 20001			/**< NVMS9000*/ 
};

#endif//__NAT_PUBLIC_TYPES_H__