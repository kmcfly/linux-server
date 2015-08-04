/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       :	袁石维
** Date         : 2010-12-30
** Name         : DDPublic.h
** Version      : 1.0
** Description  :
** Modify Record:
***********************************************************************/
#ifndef _DD_PUBLIC_H_
#define _DD_PUBLIC_H_

#include "dvrdvstypedef.h"
#include <assert.h> 
#include <iostream>
#include <fstream>
#include <string>
#include <list>


using namespace std;

////////////////////////////////////线程相关//////////////////////////////////////
#ifdef   WIN32

typedef  DWORD   RESULT;
typedef  __int64  LONGLONG;
typedef  FILETIME  DVRDATETIME;
typedef  HANDLE						PUB_thread_t;			//线程ID
typedef  LPTHREAD_START_ROUTINE		PUB_start_routine;		//线程函数
typedef  CRITICAL_SECTION			PUB_lock_t;				//锁
typedef	 HANDLE						PUB_sem_t;				//信号量
typedef	 DWORD						PUB_THREAD_RESULT;		//线程函数返回结果
#define	 PUB_THREAD_CALL			WINAPI					//函数调用方式
#define  PUB_CREATE_THREAD_FAIL		NULL					//创建线程失败
#define  printf                     TRACE
#define  snprintf                   _snprintf

#else
//#include "lvfw.h"
#include <semaphore.h>
#include <errno.h>
#include <sys/time.h>
typedef  void						RESULT;
typedef  timeval					DVRDATETIME;
typedef	 unsigned long long			ULONGLONG;				
typedef  pthread_t					PUB_thread_t;			//线程ID
typedef  void *(*start_routine)(void*);
typedef  start_routine				PUB_start_routine;		//线程函数
typedef  pthread_mutex_t			PUB_lock_t;				//锁
typedef	 sem_t						PUB_sem_t;				//信号量
typedef	 void*						PUB_THREAD_RESULT;		//线程函数返回结果
#define	 PUB_THREAD_CALL									//函数调用方式
#define  PUB_CREATE_THREAD_FAIL		0						//创建线程失败
#define WINAPI *
#endif

#define PUB_THREAD_ID_NOINIT  PUB_CREATE_THREAD_FAIL    //没有初始化的线程ID

// _PUB_DEBUG_ERROR		_PUB_DEBUG_WARN		_PUB_DEBUG_INFO 为三个调试级别
#ifndef	WIN32
#if		defined _PUB_DEBUG_ERROR || defined _PUB_DEBUG_WARN || defined _PUB_DEBUG_INFO
#include  <stdio.h>
#define   PUB_PRINTF_ERROR(format,argc...) do{printf(format, ##argc);exit(0);}while(0);
#if		defined _PUB_DEBUG_WARN || defined _PUB_DEBUG_INFO
#define	  PUB_PRINTF_WARN(format,argc...) printf(format, ##argc)
#if defined _PUB_DEBUG_INFO
#define	  PUB_PRINTF_INFO(format,argc...) printf(format, ##argc)
#else
#define   PUB_PRINTF_INFO(format,argc...)
#endif
#else
#define   PUB_PRINTF_INFO(format,argc...)
#define   PUB_PRINTF_WARN(format,argc...)
#endif
#else
#define   PUB_PRINTF_INFO(format,argc...)
#define   PUB_PRINTF_WARN(format,argc...)
#define   PUB_PRINTF_ERROR(format,argc...)
#endif
#elif defined WIN32
#if		defined _PUB_DEBUG_ERROR || defined _PUB_DEBUG_WARN || defined _PUB_DEBUG_INFO
#include  <stdio.h>
#define   PUB_PRINTF_ERROR TRACE
#if		defined _PUB_DEBUG_WARN || defined _PUB_DEBUG_INFO
#define	  PUB_PRINTF_WARN TRACE
#if defined _PUB_DEBUG_INFO
#define	  PUB_PRINTF_INFO TRACE
#else
#define   PUB_PRINTF_INFO
#endif
#else
#define   PUB_PRINTF_INFO
#define   PUB_PRINTF_WARN
#endif
#else
#define   PUB_PRINTF_INFO
#define   PUB_PRINTF_WARN
#define   PUB_PRINTF_ERROR
#endif
#endif


#ifdef WIN32
#define strcasecmp _stricmp
#endif

//创建线程
PUB_thread_t PUB_CreateThread(PUB_start_routine start_routine, void* pParam, bool *pRun);
PUB_thread_t PUB_CreateThreadEx(PUB_start_routine start_routine, void* pParam, bool *pRun, int policy);
//销毁线程（会阻塞等待线程结束）
void PUB_ExitThread(PUB_thread_t *pThreadID, bool *pRun);

////////////////////////////////////线程相关//////////////////////////////////////

////////////////////////////////////休眠和错误相关//////////////////////////////////////
//以为毫秒为单位的睡眠，
void PUB_Sleep(unsigned int dwMillisecond);

//打印致命错误
void PUB_PrintError(const char* pFile, int iLine);
////////////////////////////////////休眠和错误相关//////////////////////////////////////

////////////////////////////////////锁相关//////////////////////////////////////
//初始化锁，初始化后是可递归的锁
void PUB_InitLock(PUB_lock_t *pLock);

//销毁锁
void PUB_DestroyLock(PUB_lock_t *pLock);

//利用构造和析构函数自动加锁解锁
class CPUB_LockAction
{
public:
	explicit CPUB_LockAction(PUB_lock_t* pLock);
	~CPUB_LockAction();

private:
	PUB_lock_t* m_pLock;
};

class CPUB_Lock
{
public:
	CPUB_Lock();
	~CPUB_Lock();

	void Lock();	//加锁
	void UnLock();	//解锁
#ifndef	WIN32
	void CondWait();
	void CondSignal();
	int TimeLock(int iMicrosecond);	//BUGS在arm-uclibc-linux-g++上编译，执行这个函数有问题，不能超时
	int TryLock();
#endif
private:
	PUB_lock_t m_Lock;
#ifndef	WIN32
	pthread_cond_t m_cond;
#endif
};

#define INFINITE            0xFFFFFFFF// Infinite timeout

class CWaitEvent
{
public:
	CWaitEvent();
	~CWaitEvent();
	void CreateWaitEvent();
	unsigned long WaitForCond(DWORD dwMilliseconds = INFINITE);  /////等待信号 默认参数为
	void SetEvent();  /////设置为有信号
	void PulseEvent();
	void Close();
private:

#ifdef WIN32
	HANDLE           m_Event;
#else
	void Gettimespec(struct timespec* ptimespec, int offset);
	pthread_mutex_t  m_lock;
	pthread_cond_t   m_Event;
#endif
};
////////////////////////////////////锁相关//////////////////////////////////////

////////////////////////////////////时间相关//////////////////////////////////////
//从Windows下的SYSTEMTIME或Linux下的timeval转化成DVR自定义时间(从1970到现在的mu)
LONGLONG TimeToDVRTime(DVRDATETIME  DVRtm);
unsigned long TimeToDVRTime32(DVRDATETIME  DVRtm);
//从DVR自定义时间转化成标准的c标准的tm结构时间（tm.year要加1900,month、day从0开始）
tm DVRTimeToTm(LONGLONG DVRtm);
tm DVRTime32ToTm(unsigned long DVRtm);
//从c标准tm结构时间转化成DVR自定义时间
LONGLONG TmToDVRTime(tm time);
unsigned long TmToDVRTime32(tm time);

#ifndef __ENVIRONMENT_WIN32__
unsigned long GetTickCount();
#endif
#ifdef WIN32

//从CTime转换成DVR时间
LONGLONG CTmTo64Bit(CTime time);
unsigned long CTmTo32Bit(CTime time);
//从DVR时间转换成CTime
CTime _64BitToCTm(LONGLONG time);
CTime _32BitToCTm(unsigned long time);
#endif

#ifndef WIN32
class CThreadOnvifManager
{
public:
	CThreadOnvifManager();
	~CThreadOnvifManager();
	static void ExitThread(PUB_thread_t handle);
	static RESULT WINAPI ThreadManagerProc(LPVOID lpParameter);
	void ThreadManagerFunc();
private:
	bool m_ThreadManagerExit;
	PUB_thread_t m_ThreadManagerThread;
	list<PUB_thread_t> m_ThreadList;
	CPUB_Lock m_ThreadListLock;
};
#endif

//得到DVR标准的当前时间
LONGLONG GetCurrTime();
unsigned long GetCurrTime32();
//从c标准的tm时间转化成时间字符串可以使用strftime标准函数类似于CTime的Format方法
LONGLONG GetZeroTimeOfDate(LONGLONG time);
unsigned long GetZeroTime32OfDate(unsigned long time);
LONGLONG GetEndTimeOfDate(LONGLONG time);
unsigned long GetEndTime32OfDate(unsigned long time);

void PrintfBit64(ULONGLONG n_num, bool noTime=true);
void PrintfBit32Time(unsigned long timeSec);
////////////////////////////////////时间相关//////////////////////////////////////
#endif //_DD_PUBLIC_H_
int av_log22(unsigned int v);
int get_ue_golomb1(unsigned char* buff,int &index_r);
int get_se_golomb1(unsigned char* buff,int &index_r);
int getvideosize(unsigned char* buf,int buf_size,int& width,int & height);
