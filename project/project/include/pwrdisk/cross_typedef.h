
/*
	cross_typedef.h
*/
#ifndef _CORSS_TYPEDEF_H
#define _CORSS_TYPEDEF_H

#include "transfer_size.h"
#ifdef _WIN32
/* bsd */
typedef unsigned char			u_char;
typedef unsigned short			u_short;
typedef unsigned int			u_int;
typedef unsigned long			u_long;

/* sysv */
typedef unsigned char			uchar;
typedef unsigned short			ushort;
typedef unsigned int			uint;
typedef unsigned long			ulong;

typedef		unsigned char		u_int8_t;
typedef		char				int8_t;
typedef		unsigned short		u_int16_t;
typedef		short				int16_t;
typedef		unsigned int		u_int32_t;
typedef		int					int32_t;

typedef		unsigned char		uint8_t;
typedef		unsigned short		uint16_t;
typedef		unsigned int		uint32_t;

typedef		unsigned long long	uint64_t;
typedef		unsigned long long	u_int64_t;
typedef		long long			int64_t;

typedef		unsigned char		uchar_t;
typedef		unsigned long long __le64;

#define TRACE_ERR printf
#define TRACE_DBG printf;
#define TRACE_PRI printf
#define SLEEP(ms) Sleep(ms);
#define TIME_CNT_CURRENT(start_tm) 	start_tm = GetTickCount()
#define TIME_DIFF_MS(start_tm,stop_tm) (stop_tm - start_tm)		//以微秒为单位，这里时间不准
#define TIME_DIFF_US(start_tm,stop_tm) (stop_tm - start_tm)
typedef DWORD pub_time_t;
#pragma comment(lib,"libcommon.lib")
#else
#include <stdint.h>
#include "trace_define.h"
#define SLEEP(ms) usleep(ms*1000)
#define TIME_CNT_CURRENT(start_tm)	gettimeofday(&start_tm, NULL)
#define TIME_DIFF_MS(start_tm,stop_tm) (((stop_tm.tv_sec- start_tm.tv_sec)*1000+stop_tm.tv_usec/1000) - (start_tm.tv_usec/1000))	//以毫秒为单位
#define TIME_DIFF_US(start_tm,stop_tm) (stop_tm.tv_usec - start_tm.tv_usec)			//以微秒为单位
typedef struct timeval pub_time_t;
#endif

#define  DEV_LOCK 1
#if DEV_LOCK
#ifdef  _WIN32
#define  LOCK_DEV(plock)  EnterCriticalSection(plock)
#define  UNLOCK_DEV(plock)  LeaveCriticalSection(plock)
#else
#define  LOCK_DEV(plock) pthread_mutex_lock(plock)
#define  UNLOCK_DEV(plock) pthread_mutex_unlock(plock)
#endif
#else
#define  LOCK_DEV(plock)
#define  UNLOCK_DEV(plock)
#endif
#include "PUB_common.h"
#endif //definde _CORSS_TYPEDEF_H

