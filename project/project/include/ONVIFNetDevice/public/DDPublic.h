/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       :	Ԭʯά
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

////////////////////////////////////�߳����//////////////////////////////////////
#ifdef   WIN32

typedef  DWORD   RESULT;
typedef  __int64  LONGLONG;
typedef  FILETIME  DVRDATETIME;
typedef  HANDLE						PUB_thread_t;			//�߳�ID
typedef  LPTHREAD_START_ROUTINE		PUB_start_routine;		//�̺߳���
typedef  CRITICAL_SECTION			PUB_lock_t;				//��
typedef	 HANDLE						PUB_sem_t;				//�ź���
typedef	 DWORD						PUB_THREAD_RESULT;		//�̺߳������ؽ��
#define	 PUB_THREAD_CALL			WINAPI					//�������÷�ʽ
#define  PUB_CREATE_THREAD_FAIL		NULL					//�����߳�ʧ��
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
typedef  pthread_t					PUB_thread_t;			//�߳�ID
typedef  void *(*start_routine)(void*);
typedef  start_routine				PUB_start_routine;		//�̺߳���
typedef  pthread_mutex_t			PUB_lock_t;				//��
typedef	 sem_t						PUB_sem_t;				//�ź���
typedef	 void*						PUB_THREAD_RESULT;		//�̺߳������ؽ��
#define	 PUB_THREAD_CALL									//�������÷�ʽ
#define  PUB_CREATE_THREAD_FAIL		0						//�����߳�ʧ��
#define WINAPI *
#endif

#define PUB_THREAD_ID_NOINIT  PUB_CREATE_THREAD_FAIL    //û�г�ʼ�����߳�ID

// _PUB_DEBUG_ERROR		_PUB_DEBUG_WARN		_PUB_DEBUG_INFO Ϊ�������Լ���
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

//�����߳�
PUB_thread_t PUB_CreateThread(PUB_start_routine start_routine, void* pParam, bool *pRun);
PUB_thread_t PUB_CreateThreadEx(PUB_start_routine start_routine, void* pParam, bool *pRun, int policy);
//�����̣߳��������ȴ��߳̽�����
void PUB_ExitThread(PUB_thread_t *pThreadID, bool *pRun);

////////////////////////////////////�߳����//////////////////////////////////////

////////////////////////////////////���ߺʹ������//////////////////////////////////////
//��Ϊ����Ϊ��λ��˯�ߣ�
void PUB_Sleep(unsigned int dwMillisecond);

//��ӡ��������
void PUB_PrintError(const char* pFile, int iLine);
////////////////////////////////////���ߺʹ������//////////////////////////////////////

////////////////////////////////////�����//////////////////////////////////////
//��ʼ��������ʼ�����ǿɵݹ����
void PUB_InitLock(PUB_lock_t *pLock);

//������
void PUB_DestroyLock(PUB_lock_t *pLock);

//���ù�������������Զ���������
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

	void Lock();	//����
	void UnLock();	//����
#ifndef	WIN32
	void CondWait();
	void CondSignal();
	int TimeLock(int iMicrosecond);	//BUGS��arm-uclibc-linux-g++�ϱ��룬ִ��������������⣬���ܳ�ʱ
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
	unsigned long WaitForCond(DWORD dwMilliseconds = INFINITE);  /////�ȴ��ź� Ĭ�ϲ���Ϊ
	void SetEvent();  /////����Ϊ���ź�
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
////////////////////////////////////�����//////////////////////////////////////

////////////////////////////////////ʱ�����//////////////////////////////////////
//��Windows�µ�SYSTEMTIME��Linux�µ�timevalת����DVR�Զ���ʱ��(��1970�����ڵ�mu)
LONGLONG TimeToDVRTime(DVRDATETIME  DVRtm);
unsigned long TimeToDVRTime32(DVRDATETIME  DVRtm);
//��DVR�Զ���ʱ��ת���ɱ�׼��c��׼��tm�ṹʱ�䣨tm.yearҪ��1900,month��day��0��ʼ��
tm DVRTimeToTm(LONGLONG DVRtm);
tm DVRTime32ToTm(unsigned long DVRtm);
//��c��׼tm�ṹʱ��ת����DVR�Զ���ʱ��
LONGLONG TmToDVRTime(tm time);
unsigned long TmToDVRTime32(tm time);

#ifndef __ENVIRONMENT_WIN32__
unsigned long GetTickCount();
#endif
#ifdef WIN32

//��CTimeת����DVRʱ��
LONGLONG CTmTo64Bit(CTime time);
unsigned long CTmTo32Bit(CTime time);
//��DVRʱ��ת����CTime
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

//�õ�DVR��׼�ĵ�ǰʱ��
LONGLONG GetCurrTime();
unsigned long GetCurrTime32();
//��c��׼��tmʱ��ת����ʱ���ַ�������ʹ��strftime��׼����������CTime��Format����
LONGLONG GetZeroTimeOfDate(LONGLONG time);
unsigned long GetZeroTime32OfDate(unsigned long time);
LONGLONG GetEndTimeOfDate(LONGLONG time);
unsigned long GetEndTime32OfDate(unsigned long time);

void PrintfBit64(ULONGLONG n_num, bool noTime=true);
void PrintfBit32Time(unsigned long timeSec);
////////////////////////////////////ʱ�����//////////////////////////////////////
#endif //_DD_PUBLIC_H_
int av_log22(unsigned int v);
int get_ue_golomb1(unsigned char* buff,int &index_r);
int get_se_golomb1(unsigned char* buff,int &index_r);
int getvideosize(unsigned char* buf,int buf_size,int& width,int & height);
