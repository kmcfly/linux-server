//�����������ͣ�Ϊ��ƽ̨�����������
#ifndef _DVRTYPEDEFINE_
#define _DVRTYPEDEFINE_

#ifdef __ENVIRONMENT_WIN32__ ////////////////////�����Windowsƽ̨
#include "stdafx.h"

//����PACKED ��Ҫ���ڽ����Windows�������һ��ʹ��#pragma pack(n)��Linux��һ��ʹ��__attribute__((packed))
//�ڴ˽ṹ��Ҫд���ļ������ڿ�ƽ̨֮�����ʱ����Ҫ���¶��壬ע��__attribute__((packed))ֻ���ڵ��ֽڶ���
#define PACKED
#else ////////////////////linuxƽ̨
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

//������Ϊ��������Ķ���ʹ�÷���
typedef	signed char			SBYTE;

#include <sys/time.h>
typedef timeval             DVRDATETIME;

#endif ////////#ifdef __ENVIRONMENT_WIN32__ //�����Windowsƽ̨

#endif ////////#ifndef _DVRTYPEDEFINE_





