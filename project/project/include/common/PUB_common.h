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

#else //////Linux����
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
typedef  HANDLE						PUB_thread_t;			//�߳�ID
typedef  LPTHREAD_START_ROUTINE		PUB_start_routine;		//�̺߳���
typedef  CRITICAL_SECTION			PUB_lock_t;				//��
typedef	 HANDLE						PUB_sem_t;				//�ź���
typedef	 DWORD						PUB_THREAD_RESULT;		//�̺߳������ؽ��
#define	 PUB_THREAD_CALL			WINAPI					//�������÷�ʽ
#define  PUB_CREATE_THREAD_FAIL		NULL					//�����߳�ʧ��
#define  printf                     TRACE
#define  snprintf                   _snprintf
#elif defined __ENVIRONMENT_LINUX__
typedef	 unsigned long long			ULONGLONG;				
typedef  pthread_t					PUB_thread_t;			//�߳�ID
typedef  void *(*start_routine)(void*);
typedef  start_routine				PUB_start_routine;		//�̺߳���
typedef  pthread_mutex_t			PUB_lock_t;				//��
typedef	 sem_t						PUB_sem_t;				//�ź���
typedef	 void*						PUB_THREAD_RESULT;		//�̺߳������ؽ��
#define	 PUB_THREAD_CALL									//�������÷�ʽ
#define  PUB_CREATE_THREAD_FAIL		0						//�����߳�ʧ��
#endif

typedef  int (*MESSAGE_CALLBACK)(void* pObject, int iMsg, void *pParam1, void *pParam2);
typedef  int (*MESSAGE_CALLBACK_EX)(void* pObject, int module, int iMsg, void *pParam1, void *pParam2);

typedef struct __manual_rec_status_  
{
	ULONGLONG authCH;
	bool bStart;
}MANUALRECSTATUS;

#define PUB_THREAD_ID_NOINIT  PUB_CREATE_THREAD_FAIL    //û�г�ʼ�����߳�ID

// _PUB_DEBUG_ERROR		_PUB_DEBUG_WARN		_PUB_DEBUG_INFO Ϊ�������Լ���
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

////sleep�Ժ���Ϊ��λ
//void DVRSleep(int ms);
inline void DVRSleep(int ms)//����
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

//��Windows�µ�SYSTEMTIME��Linux�µ�timevalת����DVR�Զ���ʱ��(��1970�����ڵ�mu)
LONGLONG TimeToDVRTime(DVRDATETIME  DVRtm);
unsigned long TimeToDVRTime32(DVRDATETIME  DVRtm);
//��DVR�Զ���ʱ��ת���ɱ�׼��c��׼��tm�ṹʱ�䣨tm.yearҪ��1900,month��day��0��ʼ��
tm DVRTimeToTm(LONGLONG DVRtm);
tm DVRTime32ToTm(unsigned long DVRtm);
//��c��׼tm�ṹʱ��ת����DVR�Զ���ʱ��
LONGLONG TmToDVRTime(tm time);
unsigned long TmToDVRTime32(tm time);
//�õ�DVR��׼�ĵ�ǰʱ��
LONGLONG GetCurrTime();
unsigned long GetCurrTime32();
//��c��׼��tmʱ��ת����ʱ���ַ�������ʹ��strftime��׼����������CTime��Format����
#ifdef __ENVIRONMENT_WIN32__

//��CTimeת����DVRʱ��
LONGLONG CTmTo64Bit(CTime time);
unsigned long CTmTo32Bit(CTime time);
//��DVRʱ��ת����CTime
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
	int		GetCurInterval();//������ʱ�����غ�ִ��Startʱ��ʱ���,��λ����
	int		Restart();//������ʱ�����غ�ִ��Startʱ��ʱ���,��λ����
private:
	LONGLONG m_starttime;

	double m_dfFreq;  /////////////CPU��ʱ��Ƶ�� 

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
	unsigned long WaitForCond(DWORD dwMilliseconds = INFINITE);  /////�ȴ��ź� Ĭ�ϲ���Ϊ
	void SetEvent();  /////����Ϊ���ź�
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

//�����߳�
PUB_thread_t PUB_CreateThread(PUB_start_routine start_routine, void* pParam, bool *pRun);
PUB_thread_t PUB_CreateThreadEx(PUB_start_routine start_routine, void* pParam, bool *pRun, int policy);
//�����̣߳��������ȴ��߳̽�����
void PUB_ExitThread(PUB_thread_t *pThreadID, bool *pRun);

//��ӡ��������
void PUB_PrintError(const char* pFile, int iLine);

//��Ϊ����Ϊ��λ��˯�ߣ�
void PUB_Sleep(unsigned int dwMillisecond);

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

	//void Lock();	//����
	inline void Lock()
	{
#ifdef         __ENVIRONMENT_WIN32__
		EnterCriticalSection(&m_Lock);
#elif defined  __ENVIRONMENT_LINUX__
		pthread_mutex_lock(&m_Lock);
#endif
	}
	
	//void UnLock();	//����
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
	int TimeLock(int iMicrosecond);	//BUGS��arm-uclibc-linux-g++�ϱ��룬ִ��������������⣬���ܳ�ʱ
	int TryLock();
#endif
private:
	PUB_lock_t m_Lock;
#ifdef	__ENVIRONMENT_LINUX__
	pthread_cond_t m_cond;
#endif
};

//ʱ��ִ٣����д��һ��lj
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
	bool TryLock();	//�ɹ���������true,�����߳�������ʧ�ܷ���false.
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

//�ж�ullNum�ĵ�iIndexλ�Ƿ�Ϊ1
bool PUB_HasBit64(const ULONGLONG& ullNum, int iIndex);

//�ж��Ƿ�ÿһλ��Ϊ0��ȫΪ0�򷵻�true
bool PUB_IsEmptyBit64(const ULONGLONG& ullNum);

//��ullNum�ĵ�iIndexλ��Ϊ1
void PUB_SetBit64(ULONGLONG *pllNum, int iIndex);

//��ullNum�ĵ�iIndexλ��0
void PUB_ClearBit64(ULONGLONG *pllNum, int iIndex);

//�ж�ulNum�ĵ�iIndexλ�Ƿ�Ϊ1
//iIndex Ϊ0��31
bool PUB_HasBit32(unsigned long ulNum, int iIndex);

//��*pNum�ĵ�iIndexλ��Ϊ1
void PUB_SetBit32(unsigned long *pNum, int iIndex);

//��*pNum�ĵ�iIndexλ��0
void PUB_ClearBit32(unsigned long *pNum, int iIndex);

#if defined __DVS_7114__ || defined __DVS_7720__ || defined __DVS_2834__ || defined __DVS_9910__ || defined __DVS_131__ || defined __IPC_8180_7725__
//��double����ת��Ϊlong long int��
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
