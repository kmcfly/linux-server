// NatUserDefine.h: interface for the custom define for nat user.
//
//////////////////////////////////////////////////////////////////////

#ifndef _NAT_USER_DEFINE_H_
#define _NAT_USER_DEFINE_H_

/************************************************************************/
/* This header file is used for custom define.                          */
/* All of the public headers must include this header first.            */
/************************************************************************/

/**
 * DVRƽ̨��Ԥ��������
 * NAT_TVT_DVR_4_0 ��ʾDVR4.0ƽ̨
 * NAT_TVT_DVR_3_0 ��ʾDVR3.0ƽ̨
 *
 * Ĭ�ϣ�����������ã���NAT_TVT_DVR_3_0
 */
//#define NAT_TVT_DVR_4_0
// or 
// #define NAT_TVT_DVR_3_0

#ifdef NAT_TVT_DVR_4_0
// declare the platform for nat 
#  if defined(WIN32) || defined(_WIN32)
#     ifndef __ENVIRONMENT_WIN32__
#         define __ENVIRONMENT_WIN32__
#     endif
#  else 
#     ifndef __ENVIRONMENT_LINUX__
#         define __ENVIRONMENT_LINUX__
#     endif
#  endif
#endif//NAT_TVT_DVR_4_0

/**
 * ��͸�豸��Ϣ�Ƿ�ʹ����չ�棺���壬��ʾ��չ�棻�����壬��ʾ��׼�档
 * ��׼�棬���豸��ע����Ϣ��ʹ��ͨ�õ��ֶ���Ϣ��
 * ��չ�棬���豸��ע����Ϣ��������Ҫ��Ϣ�����豸���кţ���������Ϣ��Ҫʹ������XML��ʽ��װ����ExtraInfo����
 */
#undef __NAT_DEVICE_CONFIG_EX__

/**
 * ��͸ģ���Ƿ���Ҫ���庯��PUB_IsTimeOutEx()
 */
#define __NAT_PUB_IsTimeOutEx__

#endif//_NAT_USER_DEFINE_H_