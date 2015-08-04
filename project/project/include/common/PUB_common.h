// PUB_common.h: interface for the CPUB_common class.
//
//////////////////////////////////////////////////////////////////////

#ifndef  __PUB_COMMON_H__
#define  __PUB_COMMON_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef   __ENVIRONMENT_WIN32__
#elif defined __ENVIRONMENT_LINUX__
#else 
	#error    UNKNOWN ENVIRONMENT
#endif

#include <assert.h> 
#include "dvrdvsdef.h"
#include <iostream>
#include <fstream>
#include <string.h>
#include <list>
using namespace std;

#ifdef __ENVIRONMENT_WIN32__
#include "stdafx.h"
#include <io.h>
#include "afxsock.h"
#include "winbase.h"
#include <vfw.h>

typedef  DWORD   RESULT;
typedef  __int64  LONGLONG;
typedef  FILETIME  DVRDATETIME;

#else //////Linux下面
#include <sys/syscall.h>
#include <pthread.h> 
#include <time.h> 
#include <string.h>
#include <sys/time.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <signal.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "lvfw.h"
#define  WINAPI        *
#define  printErrorLocation(m)		{perror( m ); printf("%s():%s:%d\n", __FUNCTION__, __FILE__, __LINE__); }

typedef  timeval       DVRDATETIME;
typedef  void          RESULT;
typedef  void*         LPVOID;
typedef  unsigned long int HANDLE;
typedef  long long     LONGLONG;
typedef  RESULT WINAPI (*LPTHREAD_START_ROUTINE)(LPVOID lpParameter);
#endif

#ifdef   __ENVIRONMENT_WIN32__
#include <winbase.h>
typedef  HANDLE						PUB_thread_t;			//线程ID
typedef  LPTHREAD_START_ROUTINE		PUB_start_routine;		//线程函数
typedef  CRITICAL_SECTION			PUB_lock_t;				//锁
typedef	 HANDLE						PUB_sem_t;				//信号量
typedef	 DWORD						PUB_THREAD_RESULT;		//线程函数返回结果
#define	 PUB_THREAD_CALL			WINAPI					//函数调用方式
#define  PUB_CREATE_THREAD_FAIL		NULL					//创建线程失败
#define  printf                     TRACE
#define  snprintf                   _snprintf
#elif defined __ENVIRONMENT_LINUX__
typedef	 unsigned long long			ULONGLONG;				
typedef  pthread_t					PUB_thread_t;			//线程ID
typedef  void *(*start_routine)(void*);
typedef  start_routine				PUB_start_routine;		//线程函数
typedef  pthread_mutex_t			PUB_lock_t;				//锁
typedef	 sem_t						PUB_sem_t;				//信号量
typedef	 void*						PUB_THREAD_RESULT;		//线程函数返回结果
#define	 PUB_THREAD_CALL									//函数调用方式
#define  PUB_CREATE_THREAD_FAIL		0						//创建线程失败
#endif

typedef  int (*MESSAGE_CALLBACK)(void* pObject, int iMsg, void *pParam1, void *pParam2);
typedef  int (*MESSAGE_CALLBACK_EX)(void* pObject, int module, int iMsg, void *pParam1, void *pParam2);

typedef struct __manual_rec_status_  
{
	ULONGLONG authCH;
	bool bStart;
}MANUALRECSTATUS;

#define PUB_THREAD_ID_NOINIT  PUB_CREATE_THREAD_FAIL    //没有初始化的线程ID

// _PUB_DEBUG_ERROR		_PUB_DEBUG_WARN		_PUB_DEBUG_INFO 为三个调试级别
#ifdef	__ENVIRONMENT_LINUX__
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
#elif defined __ENVIRONMENT_WIN32__
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

#define SERVER_FIFO_NAME   "/mnt/mtd/DVRSERVER_FIFO"
#define CLIENT_FIFO_NAME   "/mnt/mtd/DVRCLIENT_FIFO"
HANDLE ComCreateThread(unsigned long int* threadid, void* arg, LPTHREAD_START_ROUTINE  start_routine);
void ComExitThread(HANDLE ThreadHandle);

#define  HAS_TIME_ZONE 1

////sleep以毫秒为单位
//void DVRSleep(int ms);
inline void DVRSleep(int ms)//毫秒
{
#ifdef __ENVIRONMENT_WIN32__
	Sleep(ms);
#else
	usleep(ms*1000);
#endif
}

unsigned long GetBuildDate();

inline void FileFlush(FILE *pFile)
{
	fflush(pFile);
#ifdef __ENVIRONMENT_WIN32__
//	_commit(_fileno(pFile));
#else
	fsync(fileno(pFile));
#endif
}

int DVRSystem(const char *cmdstring);

//从Windows下的SYSTEMTIME或Linux下的timeval转化成DVR自定义时间(从1970到现在的mu)
LONGLONG TimeToDVRTime(DVRDATETIME  DVRtm);
unsigned long TimeToDVRTime32(DVRDATETIME  DVRtm);
//从DVR自定义时间转化成标准的c标准的tm结构时间（tm.year要加1900,month、day从0开始）
tm DVRTimeToTm(LONGLONG DVRtm);
tm DVRTime32ToTm(unsigned long DVRtm);
//从c标准tm结构时间转化成DVR自定义时间
LONGLONG TmToDVRTime(tm time);
unsigned long TmToDVRTime32(tm time);
//得到DVR标准的当前时间
LONGLONG GetCurrTime();
unsigned long GetCurrTime32();
//从c标准的tm时间转化成时间字符串可以使用strftime标准函数类似于CTime的Format方法
#ifdef __ENVIRONMENT_WIN32__

//从CTime转换成DVR时间
LONGLONG CTmTo64Bit(CTime time);
unsigned long CTmTo32Bit(CTime time);
//从DVR时间转换成CTime
CTime _64BitToCTm(LONGLONG time);
CTime _32BitToCTm(unsigned long time);
#endif

void Release (FILE **pFile, unsigned long num, ...);

LONGLONG GetZeroTimeOfDate(LONGLONG time);
unsigned long GetZeroTime32OfDate(unsigned long time);
LONGLONG GetEndTimeOfDate(LONGLONG time);
unsigned long GetEndTime32OfDate(unsigned long time);

void PrintfBit64(ULONGLONG n_num, bool noTime=true);
void PrintfBit32Time(unsigned long timeSec);
BYTE GetChnnForbits(ULONGLONG chnnbits);

bool IsValidIPCfg(DWORD dwIP,DWORD dwSubnetMask,DWORD dwGateWay);

#ifndef __ENVIRONMENT_WIN32__
void RestartHttpSvr(unsigned short HTTPPort, unsigned short serverPort, const char *pPath);
long killProcessByName(char *pidName, const char *pParam = NULL);
bool IsProcessRunning(char *pidName, const char *pParam = NULL);
void DHCPSet(const char *pifname);
void DNSSet(const char *pifname,char *defaultdns=NULL);
#endif

class CTimeInterval
{
public:
	CTimeInterval();
	virtual ~CTimeInterval();
	void	Start();
	int		GetCurInterval();//结束计时，返回和执行Start时的时间差,单位毫秒
	int		Restart();//结束计时，返回和执行Start时的时间差,单位毫秒
private:
	LONGLONG m_starttime;

	double m_dfFreq;  /////////////CPU的时钟频率 

	LONGLONG m_QPart1,m_QPart2;

};

/**********************************************************************************
*
*                          class CWaitEvent       
*
**********************************************************************************/

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

#ifdef __ENVIRONMENT_WIN32__
	HANDLE           m_Event;
#else
	void Gettimespec(struct timespec* ptimespec, int offset);
	pthread_mutex_t  m_lock;
	pthread_cond_t   m_Event;
#endif
};

//创建线程
PUB_thread_t PUB_CreateThread(PUB_start_routine start_routine, void* pParam, bool *pRun);
PUB_thread_t PUB_CreateThreadEx(PUB_start_routine start_routine, void* pParam, bool *pRun, int policy);
//销毁线程（会阻塞等待线程结束）
void PUB_ExitThread(PUB_thread_t *pThreadID, bool *pRun);

//打印致命错误
void PUB_PrintError(const char* pFile, int iLine);

//以为毫秒为单位的睡眠，
void PUB_Sleep(unsigned int dwMillisecond);

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

	//void Lock();	//加锁
	inline void Lock()
	{
#ifdef         __ENVIRONMENT_WIN32__
		EnterCriticalSection(&m_Lock);
#elif defined  __ENVIRONMENT_LINUX__
		pthread_mutex_lock(&m_Lock);
#endif
	}
	
	//void UnLock();	//解锁
	inline void UnLock()
	{
#ifdef         __ENVIRONMENT_WIN32__
		LeaveCriticalSection(&m_Lock);
#elif defined  __ENVIRONMENT_LINUX__
		pthread_mutex_unlock(&m_Lock);
#endif
	}
#ifdef	__ENVIRONMENT_LINUX__
	void CondWait();
	void CondSignal();
	int TimeLock(int iMicrosecond);	//BUGS在arm-uclibc-linux-g++上编译，执行这个函数有问题，不能超时
	int TryLock();
#endif
private:
	PUB_lock_t m_Lock;
#ifdef	__ENVIRONMENT_LINUX__
	pthread_cond_t m_cond;
#endif
};

//时间仓促，随便写了一下lj
class CPUB_Sem
{
public:
	CPUB_Sem();
	~CPUB_Sem();

	void Wait();
	void Post();

private:
	PUB_sem_t m_Sem;
};

class CLock
{

public:
	CLock();
	void Lock();
	bool TryLock();	//成功加锁返回true,其他线程已锁或失败返回false.
	void Unlock();
	virtual ~CLock();
protected:
private:
#ifndef __ENVIRONMENT_WIN32__
	pthread_mutex_t    m_MutexLock;
	pthread_mutexattr_t m_MutexAttr;// = PTHREAD_MUTEX_RECURSIVE_NP;
#else
	CRITICAL_SECTION   m_MutexLock;
#endif

};

//判断ullNum的第iIndex位是否为1
bool PUB_HasBit64(const ULONGLONG& ullNum, int iIndex);

//判断是否每一位都为0，全为0则返回true
bool PUB_IsEmptyBit64(const ULONGLONG& ullNum);

//把ullNum的第iIndex位设为1
void PUB_SetBit64(ULONGLONG *pllNum, int iIndex);

//把ullNum的第iIndex位清0
void PUB_ClearBit64(ULONGLONG *pllNum, int iIndex);

//判断ulNum的第iIndex位是否为1
//iIndex 为0到31
bool PUB_HasBit32(unsigned long ulNum, int iIndex);

//把*pNum的第iIndex位设为1
void PUB_SetBit32(unsigned long *pNum, int iIndex);

//把*pNum的第iIndex位清0
void PUB_ClearBit32(unsigned long *pNum, int iIndex);

#if defined __DVS_7114__ || defined __DVS_7720__ || defined __DVS_2834__ || defined __DVS_9910__ || defined __DVS_131__ || defined __IPC_8180_7725__
//把double类型转换为long long int型
long long PUB_llround (double x);
#endif

#ifndef __ENVIRONMENT_WIN32__
class CThreadManager
{
public:
	CThreadManager();
	~CThreadManager();
	static void ExitThread(HANDLE handle);
	static RESULT WINAPI ThreadManagerProc(LPVOID lpParameter);
	void ThreadManagerFunc();
private:
	bool m_ThreadManagerExit;
	HANDLE m_ThreadManagerThread;
	CMyList<HANDLE> m_ThreadList;
	CPUB_Lock m_ThreadListLock;
};

char *DVRStrToUper(char *pStr);
unsigned long GetTickCount();
LONGLONG GetTickCount64();
#endif

bool IsTimeOut(unsigned long dwStartTime, unsigned long dwDisTime, unsigned long *pCurTime = NULL);
bool GetKernelverionEx(unsigned long *pVersion);
void AreaInfo(unsigned short width, unsigned short height, VIEW_SPLIT_MODE mode, DISPLAY_ATTRIB & display, unsigned char index, unsigned short left = 0, unsigned short top = 0);
void AreaInfoEx(unsigned short width, unsigned short height, unsigned short realWidth, unsigned short realHeight, VIEW_SPLIT_MODE mode, DISPLAY_ATTRIB & display, unsigned char index, unsigned short left = 0, unsigned short top = 0);
int AreaIndex(unsigned short width, unsigned short height, VIEW_SPLIT_MODE mode, unsigned short x, unsigned short y, unsigned short cx, unsigned short cy);

void GetVGAResolution(VGA_RESOLUTION resolution, unsigned short & width, unsigned short & height);
void GetVGAResolution(VGA_RESOLUTION resolution, char *pName);
void GetCVBSResolution(VIDEO_FORMAT videoFormat, unsigned short & width, unsigned short & height, bool adjust=false);

unsigned long UsedRate(unsigned long rate, VIDEO_SIZE videoSize);
unsigned long RealRate(unsigned long rate, VIDEO_SIZE videoSize);
unsigned long UsedRateEx(unsigned long rate, VIDEO_SIZE videoSize);
unsigned long RealRateEx(unsigned long rate, VIDEO_SIZE videoSize);


bool CheckVersion(char *pCheck);
bool FindStringInFile(const char *pFilePath,const char *pFindContent);
bool SetUpdateStatus(bool bSucc);

int av_log22(unsigned int v);
int get_ue_golomb1(unsigned char* buff,int &index_r);
int get_se_golomb1(unsigned char* buff,int &index_r);
int getvideosize(unsigned char* buf,int buf_size,int& width,int & height);
void GetURLAndPort(char *inputUrl,char *outputUrl,char *outputUrlEnd,unsigned short &port);
int GetThisThreadId();
bool BindThreadToCpu(int pid, int whichCpu);
int GetNumberOfCpu();

#if defined(__VERY_BIG_DISK__)
ssize_t Readn(int fd, char *vptr, size_t n);
ssize_t Writen(int fd, const char *vptr, size_t n);
void GetIdFromPath(const char * pFilePath, unsigned long *diskID, unsigned long *fileID);
#define FILE_NAME_LEN 96
unsigned long GetCRC_Value ( char *block, unsigned int length);
#endif
bool GetCustomEncrypt(unsigned int productId,string &outStr);

#endif // !defined(AFX_PUB_COMMON_H__5047E65D_1AF9_4F44_8F9F_411E9C95BCF7__INCLUDED_)
