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
 * ��͸��������Ϣ����Ҫ��Ϊ��͸�������б���
 */
typedef struct __nat_server_list_info__
{
    char                szAddr[64];     // ��������ַ
    unsigned short      sPort;          // ��͸�˿ں�
    unsigned short      sTestPort;      // ��͸���Զ˿ں� 
}NAT_SERVER_LIST_INFO;


#define NAT_SERVER_PORT			8989 
#define NAT_SERVER_TEST_PORT	8993

/**
 * �豸�汾����
 */
enum NAT_DEVICE_VER_TYPE
{
	NAT_DEVICE_VER_3			= 3,			/**< 3.0�豸���� */
	NAT_DEVICE_VER_4			= 4,			/**< 4.0�豸���� */
	NAT_DEVICE_VER_IPC			= 10001,		/**< IPC�豸���� */
	NAT_DEVICE_VER_NVMS_9000	= 20001			/**< NVMS9000*/ 
};

#endif//__NAT_PUBLIC_TYPES_H__