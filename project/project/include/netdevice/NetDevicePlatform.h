#ifndef _NET_DEVICE_PLATFORM_H_
#define _NET_DEVICE_PLATFORM_H_

//////////////////////////////////////////////////////////////////////////
/*
 * Win32�� : __ENVIRONMENT_WIN32__
 * Linux�� : __ENVIRONMENT_LINUX__
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

//��������, ��� ����
#define nd_in
#define nd_out
#define nd_in_out

//////////////////////////////////////////////////////////////////////////
#ifdef __ENVIRONMENT_WIN32__
#pragma pack(4)
#endif

//��׼����
#include "dvrdvstypedef.h"
#include "dvrdvsdefine.h"
#include "dvrdvsconfig.h"

//��̨
#include "miss/PTZ.h"

//Net Device ר�ж���
#include "NetDeviceTypedef.h"
#include "NetDeviceDefine.h"

//�ӿ��ļ�
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

//��ʼ��, �˳���
EXTERN_C ND_API int			nd_init();
EXTERN_C ND_API int			nd_quit();

//��ȡ����Ϣ,�ɹ�����0
EXTERN_C ND_API int			nd_get_info(nd_out ND_LIB_INFO* p_lib_info);

//�����������豸���ɹ����ص�ǰ���������豸��Ŀ
EXTERN_C ND_API int			nd_get_discovered_device_num();
//��ȡ���������豸��Ϣ���ɹ������豸��Ŀ
EXTERN_C ND_API int			nd_get_discovered_device_info(nd_out ND_DEVICE_INFO* p_device_info, nd_in int deviceNum);

//�������������豸IP��ַ��Ϣ���޷���
EXTERN_C ND_API void		nd_set_discovered_device_info(nd_in ND_DEVICE_NETWORK_INFO * pNetworkInfo);

//��ʱ����
EXTERN_C ND_API void		nd_on_timer(nd_in unsigned long current_time);

//����, �ͷ� �ӿڶ���
EXTERN_C ND_API CNetDevice* nd_create();
EXTERN_C ND_API void		nd_destroy(nd_in CNetDevice* p_net_device);


//////////////////////////////////////////////////////////////////////////

#endif
//end
