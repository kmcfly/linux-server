
#include <assert.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>

#include <sys/types.h>	//shanchu
#include <sys/timeb.h>	//shanchu

#ifdef __ENVIRONMENT_LINUX__
#include <sys/time.h>
#include <stdarg.h>
#include "dvr_func.h"

#ifndef __USE_GNU
#define __USE_GNU
#endif
#include <sched.h>

#endif

#if defined __DVS_7114__ || defined __DVS_7720__ || defined __DVS_2834__ || defined __DVS_9910__ || defined __DVS_131__ || defined __IPC_8180_7725__
#include "CmdClient.h"
#endif

#include "PUB_common.h"
#ifndef  __ENVIRONMENT_WIN32__
#include "dirent.h"
#include "ctype.h"
#endif

#if defined(__DVR_BASIC__)
#include "Product.h"
#endif

#include "mylist.cpp"
#include "Version.h"

#ifdef __ENVIRONMENT_LINUX__
extern bool g_bCVBSChangeResolution;
#endif

#if defined(__CHIPTI__)
extern "C" time_t mktime_ti8107(struct tm * tp);
#endif

//根据关键帧，判断出该帧的宽高
//只支持16像素整数倍图像大小，非high profile的视频流图像大小检测。
typedef signed char  int8_ts;
typedef unsigned char  uint8_tu;
const uint8_tu ff_ue_golomb_vlc_codet[512] =
{
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,
	7, 7, 7, 7, 8, 8, 8, 8, 9, 9, 9, 9,10,10,10,10,11,11,11,11,12,12,12,12,13,13,13,13,14,14,14,14,
	3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
	5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
	2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};
const uint8_tu ff_golomb_vlc_lent[512] = 
{
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,
	7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
	5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,
	5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,
	3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,
	3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,
	3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,
	3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1
};
static const int8_ts ff_se_golomb_vlc_code[512] =
{
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  8, -8,  9, -9, 10,-10, 11,-11, 12,-12, 13,-13, 14,-14, 15,-15,
	4,  4,  4,  4, -4, -4, -4, -4,  5,  5,  5,  5, -5, -5, -5, -5,  6,  6,  6,  6, -6, -6, -6, -6,  7,  7,  7,  7, -7, -7, -7, -7,
	2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2,
	3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3, -3, -3, -3, -3, -3, -3, -3, -3, -3, -3, -3, -3, -3, -3, -3, -3,
	1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
	1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
};

int av_log22(unsigned int v)
{
	int n;
	static const uint8_tu ff_log2_tab[256]={
		0,0,1,1,2,2,2,2,3,3,3,3,3,3,3,3,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,
		5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,
		6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,
		6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,
		7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
		7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
		7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
		7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7
	};

	n = 0;
	if (v & 0xffff0000) {
		v >>= 16;
		n += 16;
	}
	if (v & 0xff00) {
		v >>= 8;
		n += 8;
	}
	n += ff_log2_tab[v];

	return n;
}
int get_ue_golomb1(unsigned char* buff,int &index_r)
{
	unsigned int buf;
	int log;
	int index = index_r;
	buff = buff+(index >>3);
	index = index & 7;
	buf = ((buff[0]<<index)&0xff)+(buff[1]>>(8-index));
	buf <<=8;
	buf += ((buff[1]<<index)&0xff)+(buff[2]>>(8-index));
	buf <<=8;
	buf += ((buff[2]<<index)&0xff)+(buff[3]>>(8-index));
	buf <<=8;
	buf += ((buff[3]<<index)&0xff)+(buff[4]>>(8-index));

	if(buf >= (1<<27))
	{
		buf >>= 32 - 9;
		index_r += ff_golomb_vlc_lent[buf];
		return ff_ue_golomb_vlc_codet[buf];
	}
	else
	{
		log= 2*av_log22(buf) - 31;
		buf>>= log;
		buf--;
		//  LAST_SKIP_BITS(re, gb, 32 - log);
		index_r +=32 - log;
		//      CLOSE_READER(re, gb);
		/**/
		return buf;
	}
}

int get_se_golomb1(unsigned char* buff,int &index_r)
{
	unsigned int buf;
	int log;
	int index = index_r;
	buff = buff+(index >>3);
	index = index & 7;
	buf = ((buff[0]<<index)&0xff)+(buff[1]>>(8-index));
	buf <<=8;
	buf += ((buff[1]<<index)&0xff)+(buff[2]>>(8-index));
	buf <<=8;
	buf += ((buff[2]<<index)&0xff)+(buff[3]>>(8-index));
	buf <<=8;
	buf += ((buff[3]<<index)&0xff)+(buff[4]>>(8-index));

	if(buf >= (1<<27)){
		buf >>= 32 - 9;
		index_r += ff_golomb_vlc_lent[buf];
		return ff_se_golomb_vlc_code[buf];
	}else{
		log= 2*av_log22(buf) - 31;
		buf>>= log;
		index_r +=  32 - log;
		if(buf&1) buf= -(buf>>1);
		else      buf=  (buf>>1);
		return buf;
	}
}

int getvideosize(unsigned char* buf,int buf_size,int& width,int & height)
{
	int buf_index = 0;

	for(; buf_index + 3 < buf_size; buf_index++)
	{
		if(buf[buf_index] == 0 && buf[buf_index+1] == 0 && buf[buf_index+2] == 1&& buf[buf_index+3] == 0x67)
		{
			break;
		}
	}
	if(buf_index>=buf_size-3)
	{
		return 0;
	}
	unsigned char * sps = &buf[buf_index+4];
	if(sps[0]>100)
	{
		return 0;//high profile 
	}
	int skip;
	int index=24;
	char sps_id = get_ue_golomb1(sps,index);
	index = 26;
	char poc_type = get_ue_golomb1(sps,index);
	if(poc_type == 0)
	{ //FIXME #define   
		//  sps->log2_max_poc_lsb= get_ue_golomb(&s->gb,hDec) + 4;//27
		index = 30;
	} else if(poc_type == 1)
	{//FIXME #define         //30
		//   sps->delta_pic_order_always_zero_flag= get_bits1(&s->gb);
		index ++;
		get_se_golomb1(sps,index);
		get_se_golomb1(sps,index);
		int poc_cycle_length= get_ue_golomb1(sps,index);

		if(poc_cycle_length >= 255)
		{
			//   av_log(h->s.avctx, AV_LOG_ERROR, "poc_cycle_length overflow %u\n", tmp);
			return -1;
		}

		for(int i=0; i<poc_cycle_length; i++)
			get_se_golomb1(sps,index);/**/
	}
	else if(poc_type != 2)
	{

		return -1;
	}
	int ref= get_ue_golomb1(sps,index);//29 
	index ++;
	width= get_ue_golomb1(sps,index) + 1;//33
	width*=16;
	height= get_ue_golomb1(sps,index) + 1;
	height*=16;
	return 1;
}

//返回编译时间
unsigned long GetBuildDate()
{
	const char *pDate = __DATE__;
	const char *pTime = __TIME__;

	struct tm tmTime;
	memset(&tmTime, 0, sizeof(tm));

	//year
	tmTime.tm_year = atoi(pDate+7) - 1900;
	
	//mon
	if(strncmp(pDate,"Jan",3)==0)
	{
		tmTime.tm_mon = 0;
	}
	if(strncmp(pDate,"Feb",3)==0)
	{
		tmTime.tm_mon = 1;	
	}
	if(strncmp(pDate,"Mar",3)==0)
	{
		tmTime.tm_mon = 2;	
	}
	if(strncmp(pDate,"Apr",3)==0)
	{
		tmTime.tm_mon = 3;	
	}
	if(strncmp(pDate,"May",3)==0)
	{
		tmTime.tm_mon = 4;	
	}
	if(strncmp(pDate,"Jun",3)==0)
	{
		tmTime.tm_mon = 5;		
	}
	if(strncmp(pDate,"Jul",3)==0)
	{
		tmTime.tm_mon = 6;	
	}
	if(strncmp(pDate,"Aug",3)==0)
	{
		tmTime.tm_mon = 7;	
	}
	if(strncmp(pDate,"Sep",3)==0)
	{
		tmTime.tm_mon = 8;	
	}
	if(strncmp(pDate,"Oct",3)==0)
	{
		tmTime.tm_mon = 9;	
	}
	if(strncmp(pDate,"Nov",3)==0)
	{
		tmTime.tm_mon = 10;	
	}
	if(strncmp(pDate,"Dec",3)==0)
	{
		tmTime.tm_mon = 11;		
	}

	//mday
	tmTime.tm_mday  = atoi(pDate+4);

	//hour
	tmTime.tm_hour	= atoi(pTime);

	//min
	tmTime.tm_min	= atoi(pTime+3);

	//sec
	tmTime.tm_sec	= atoi(pTime+6);

	return TmToDVRTime32(tmTime);
}
/*
*
* 返回值说明：在Window下返回的是线程句柄，NULL表示线程创建失败。在Linux下返回的是线程ID，NULL表示线程创建失败。
*
*/
HANDLE ComCreateThread(unsigned long int* threadid, void* arg, LPTHREAD_START_ROUTINE  start_routine)
{	
	HANDLE ThreadHandle;
#ifdef __ENVIRONMENT_WIN32__
	ThreadHandle = CreateThread(0, 0, start_routine, (LPVOID)arg, 0, (LPDWORD)threadid);
#else
	int    Error;
/*	pthread_attr_t attr;
	int size = PTHREAD_STACK_MIN + 0x1c000;

	pthread_attr_init(&attr);
	pthread_attr_setstacksize( &attr, size );*/

	Error = pthread_create(threadid, 0, start_routine, arg);
	if(!Error)
		ThreadHandle = *threadid;
	else
		ThreadHandle = 0;

//	pthread_attr_destroy( &attr );
#endif
	return ThreadHandle;
}

/*
*
* 输入参数说明：在Window下传入的是线程句柄。在Linux下传入的是线程ID。
*
*/

void ComExitThread(HANDLE ThreadHandle)
{
#ifdef __ENVIRONMENT_WIN32__
	if(ThreadHandle)
	{
		WaitForSingleObject(ThreadHandle, INFINITE);
		CloseHandle(ThreadHandle);
		ThreadHandle = NULL;
	}
#else
	pthread_join(ThreadHandle, NULL);
#endif ///
}

//输入参数：start_routine 线程的执行函数
//			pParam  线程执行时传入的参数
//			pRun    线程是否执行的bool量的指针，如果为NULL，则不理会此参数
//return value: 成功：返回线程ID    *pRun = true
//				失败：返回PUB_CREATE_THREAD_FAIL  *pRun的值不变
PUB_thread_t PUB_CreateThread(PUB_start_routine start_routine, void* pParam, bool *pRun)
{	
	PUB_thread_t threadID;
	if (NULL != pRun) 
	{
		*pRun = true;
	}
#ifdef __ENVIRONMENT_WIN32__
	threadID = CreateThread(0, 0, start_routine, (LPVOID)pParam, 0, NULL);
	if ( (PUB_CREATE_THREAD_FAIL == threadID) && (NULL != pRun) ) {
		*pRun = false;
	}
#elif defined __ENVIRONMENT_LINUX__
	int iRet = 0;
	if((iRet = pthread_create(&threadID, 0, start_routine, pParam)) != 0){
		threadID = PUB_CREATE_THREAD_FAIL;
		if (NULL != pRun)
		{
			*pRun = false;
		}		
	}
	errno = iRet;	//线程创建不会设置错误码，而是直接返回错误码
#endif
	return threadID;
}

//输入参数：start_routine 线程的执行函数
//			pParam  线程执行时传入的参数
//			pRun    线程是否执行的bool量的指针，如果为NULL，则不理会此参数
//			policy  线程调度优先级
//			return value: 成功：返回线程ID    *pRun = true
//				失败：返回PUB_CREATE_THREAD_FAIL  *pRun的值不变
PUB_thread_t PUB_CreateThreadEx(PUB_start_routine start_routine, void* pParam, bool *pRun, int policy)
{	
	PUB_thread_t threadID;
	if (NULL != pRun) 
	{
		*pRun = true;
	}
#ifdef __ENVIRONMENT_WIN32__
	threadID = CreateThread(0, 0, start_routine, (LPVOID)pParam, 0, NULL);
	if ( (PUB_CREATE_THREAD_FAIL == threadID) && (NULL != pRun) ) {
		*pRun = false;
	}
#elif defined __ENVIRONMENT_LINUX__
	int iRet = 0;
	pthread_attr_t thread_attr;
	struct sched_param thread_param;

	pthread_attr_init(&thread_attr);
	pthread_attr_setschedpolicy(&thread_attr, SCHED_RR);
	thread_param.sched_priority = policy;
	pthread_attr_setschedparam(&thread_attr, &thread_param);

	iRet = pthread_create(&threadID, &thread_attr, start_routine, pParam);
	if (0 != iRet)
	{
		threadID = PUB_CREATE_THREAD_FAIL;
		if (NULL != pRun)
		{
			*pRun = false;
		}		
	}

	errno = iRet;	//线程创建不会设置错误码，而是直接返回错误码
#endif
	return threadID;
}
//ThreadID 线程ID
//pRun     线程是否执行的bool量的指针
//return value: 退出线程后 *pThreadID = PUB_THREAD_ID_NOINIT，*pRun = false
void PUB_ExitThread(PUB_thread_t *pThreadID, bool *pRun)
{
	assert(NULL != pThreadID);
	if (PUB_THREAD_ID_NOINIT == *pThreadID)
	{
		return;
	}

	if (NULL != pRun) 
	{
		*pRun = false;
	}	
#ifdef    __ENVIRONMENT_WIN32__
	WaitForSingleObject(*pThreadID, INFINITE);
	CloseHandle(*pThreadID);
#elif defined __ENVIRONMENT_LINUX__
	if(pthread_self() == *pThreadID)
		CThreadManager::ExitThread(*pThreadID);
	else
		pthread_join(*pThreadID, NULL);
#endif
	*pThreadID = PUB_THREAD_ID_NOINIT;
}

//以为毫秒为单位的睡眠，
//BUGS：
//这里忽略了liunx下usleep是会返回非0的，并且没有了linux下不能睡眠微秒了
//linux下:SUSv2 version usleep 被信号中断，或者设置休息的时间大于一秒则会返回-1
//		  BSD version   usleep 无返回值
//		  dwMillisecond超过6分钟左右好像有问题 估计unsigned int不支持这么大的数字
void PUB_Sleep(unsigned int dwMillisecond)
{
#if defined(__TDFH__) || defined(__TDNXP__) || defined(__CHIPTI__) || defined(__CHIPGM__)
	if (dwMillisecond < 2)
	{
		dwMillisecond = 2;
	}
#endif

#ifdef         __ENVIRONMENT_WIN32__
	if (0 == dwMillisecond) {
		Sleep(10);
	}
	else{
		Sleep(dwMillisecond);
	}
	
#elif defined  __ENVIRONMENT_LINUX__
	int iSec = dwMillisecond / 1000;
	int	iMicSec = (dwMillisecond % 1000) * 1000;

	//大于一秒的时间用这个睡眠
	if (iSec > 0) 
	{
		do 
		{
			iSec = sleep(iSec);
		} while(iSec > 0); 
	}

	if(0 != usleep(iMicSec))
	{
		if (EINTR == errno) 
		{
			//printf("the usleep Interrupted by a signal. pid = %d\n", getpid());
		}
		else if (EINVAL == errno) 
		{
			assert(false);
			printf("the usleep param is not smaller than 1000000");
		}
	}
//	usleep(dwMillisecond*1000);
#endif
}

//初始化锁，初始化后是可嵌套的锁
void PUB_InitLock(PUB_lock_t *pLock)
{
	assert(NULL != pLock);
#ifdef         __ENVIRONMENT_WIN32__
	InitializeCriticalSection(pLock);
#elif defined  __ENVIRONMENT_LINUX__
	pthread_mutexattr_t mutexAttr;
    pthread_mutexattr_init(&mutexAttr);
	pthread_mutexattr_settype(&mutexAttr, PTHREAD_MUTEX_RECURSIVE);
	pthread_mutex_init(pLock, &mutexAttr);
	pthread_mutexattr_destroy(&mutexAttr);
#endif
}

//销毁锁
void PUB_DestroyLock(PUB_lock_t *pLock)
{
	assert(NULL != pLock);
#ifdef         __ENVIRONMENT_WIN32__
	DeleteCriticalSection(pLock);
#elif defined  __ENVIRONMENT_LINUX__
	pthread_mutex_destroy(pLock);
#endif
}

CPUB_LockAction::CPUB_LockAction(PUB_lock_t* pLock)
{
	assert(NULL != pLock);
	m_pLock = pLock;
#ifdef         __ENVIRONMENT_WIN32__
	EnterCriticalSection(pLock);
#elif defined  __ENVIRONMENT_LINUX__
	pthread_mutex_lock(pLock);
#endif
}

CPUB_LockAction::~CPUB_LockAction()
{
#ifdef         __ENVIRONMENT_WIN32__
	LeaveCriticalSection(m_pLock);
#elif defined  __ENVIRONMENT_LINUX__
	pthread_mutex_unlock(m_pLock);
#endif
}
CLock::CLock()
{
#ifndef __ENVIRONMENT_WIN32__
//	if (0 == pthread_mutexattr_init(&m_MutexAttr))//
	{
//		if (0 == pthread_mutexattr_settype(&m_MutexAttr, PTHREAD_MUTEX_RECURSIVE_NP))
		{
			pthread_mutex_init(&m_MutexLock,NULL);//
		}
	}
#else
	InitializeCriticalSection(&m_MutexLock);
#endif
}

CLock::~CLock()
{
#ifndef __ENVIRONMENT_WIN32__
	pthread_mutex_destroy(&m_MutexLock);
#else
	DeleteCriticalSection(&m_MutexLock);
#endif ////
}

void CLock::Lock()
{
#ifndef __ENVIRONMENT_WIN32__
	pthread_mutex_lock(&m_MutexLock);
#else
	EnterCriticalSection(&m_MutexLock);
#endif ////
}
// #ifdef __ENVIRONMENT_WIN32__
// 
// #include "StdAfx.h"
// #include "winbase.h"
// #endif
bool CLock::TryLock()
{
	bool Ret = false;
#ifndef __ENVIRONMENT_WIN32__
	if (pthread_mutex_trylock(&m_MutexLock) != EBUSY)	//尝试加锁成功.
	{
		Ret = true;
	}
#else
//	if (TryEnterCriticalSection(&m_MutexLock))		//尝试加锁成功.
//	{
//		Ret =true;
//	}	
	ASSERT(FALSE); ////////以免给别人带来烦恼 先去掉
#endif
	return Ret;
}


void CLock::Unlock()
{
#ifndef __ENVIRONMENT_WIN32__
	pthread_mutex_unlock(&m_MutexLock);	
#else
	LeaveCriticalSection(&m_MutexLock);
#endif /////
}
CPUB_Lock::CPUB_Lock()
{
#ifdef         __ENVIRONMENT_WIN32__
	InitializeCriticalSection(&m_Lock);
#elif defined  __ENVIRONMENT_LINUX__
	pthread_mutexattr_t mutexAttr;
    pthread_mutexattr_init(&mutexAttr);
	pthread_mutexattr_settype(&mutexAttr, PTHREAD_MUTEX_RECURSIVE);
	pthread_mutex_init(&m_Lock, &mutexAttr);
	pthread_mutexattr_destroy(&mutexAttr);
	pthread_cond_init(&m_cond, NULL);
#endif
}

CPUB_Lock::~CPUB_Lock()
{
#ifdef         __ENVIRONMENT_WIN32__
	DeleteCriticalSection(&m_Lock);
#elif defined  __ENVIRONMENT_LINUX__
	pthread_cond_destroy(&m_cond);
	pthread_mutex_destroy(&m_Lock);
#endif
}

// void CPUB_Lock::Lock()
// {
// #ifdef         __ENVIRONMENT_WIN32__
// 	EnterCriticalSection(&m_Lock);
// #elif defined  __ENVIRONMENT_LINUX__
// 	pthread_mutex_lock(&m_Lock);
// #endif
// }

// void CPUB_Lock::UnLock()
// {
// #ifdef         __ENVIRONMENT_WIN32__
// 	LeaveCriticalSection(&m_Lock);
// #elif defined  __ENVIRONMENT_LINUX__
// 	pthread_mutex_unlock(&m_Lock);
// #endif
// }

#ifdef	__ENVIRONMENT_LINUX__
void CPUB_Lock::CondWait()
{
#ifdef         __ENVIRONMENT_WIN32__
	;
#elif defined  __ENVIRONMENT_LINUX__
	pthread_cond_wait(&m_cond, &m_Lock);
#endif
}

void CPUB_Lock::CondSignal()
{
#ifdef         __ENVIRONMENT_WIN32__
	;
#elif defined  __ENVIRONMENT_LINUX__
	pthread_cond_signal(&m_cond);
#endif
}

//return value: 0 成功 -1 加锁失败
int CPUB_Lock::TryLock()
{
	if(0 == pthread_mutex_trylock(&m_Lock))
	{
		return 0;
	}
	return -1;
}

//return value:	0 成功得到锁 1 超时 2 其他错误
//BUGS:这个在uclibc上好像没有用，或者是自己就写了一个bug
int CPUB_Lock::TimeLock(int iMicrosecond)
{
#ifdef         __ENVIRONMENT_WIN32__
	;
#elif defined  __ENVIRONMENT_LINUX__
	struct timeval     timeNow;
	struct timespec    Timeout;
	gettimeofday(&timeNow, NULL);
	Timeout.tv_sec = timeNow.tv_sec;
	Timeout.tv_nsec = (timeNow.tv_usec + iMicrosecond) * 1000;              
	int iRet = pthread_mutex_timedlock(&m_Lock, &Timeout);
	if (0 == iRet) 
	{
		return 0;
	}
	else if (-1 == iRet) 
	{
		if (ETIMEDOUT == errno) 
		{
			return 1;
		}
		else
		{
			assert(false);
			return 2;
		}
	}
	return 0;
#endif
}
#endif

//打印致命错误
void PUB_PrintError(const char* pFile, int iLine)
{
	char szErrorSource[200] = {0};
	sprintf(szErrorSource, "%s %d ", pFile, iLine);
	perror(szErrorSource);
}

CPUB_Sem::CPUB_Sem()
{
#ifdef			__ENVIRONMENT_WIN32__
	m_Sem = CreateSemaphore(NULL, 0, 1, NULL);
#elif defined	__ENVIRONMENT_LINUX__
	sem_init(&m_Sem, 0, 0);
#endif
}

CPUB_Sem::~CPUB_Sem()
{
#ifdef			__ENVIRONMENT_WIN32__
	CloseHandle(m_Sem);
#elif defined	__ENVIRONMENT_LINUX__
	sem_destroy(&m_Sem);
#endif
}

void CPUB_Sem::Wait()
{
#ifdef			__ENVIRONMENT_WIN32__
	WaitForSingleObject(m_Sem, INFINITE); 
#elif defined	__ENVIRONMENT_LINUX__
	sem_wait(&m_Sem);
#endif
}

void CPUB_Sem::Post()
{
#ifdef			__ENVIRONMENT_WIN32__
	ReleaseSemaphore(m_Sem, 1, NULL); 
#elif defined	__ENVIRONMENT_LINUX__
	sem_post(&m_Sem);
#endif
}

//判断ullNum的第iIndex位是否为1
//iIndex 为0到63
bool PUB_HasBit64(const ULONGLONG& ullNum, int iIndex)
{
	assert( (0 <= iIndex) && (63 >= iIndex));
	const unsigned long *pTmp = reinterpret_cast<const unsigned long *>(&ullNum);
	if (31 >= iIndex)
	{
		if( (*pTmp) & (0x01<<iIndex))
		{
			return true;
		}
	}
	else
	{
		pTmp += 1;
		iIndex -= 31;
		if( (*pTmp) & (0x01<<iIndex))
		{
			return true;
		}
	}
	return false;
}

//判断是否每一位都为0，全为0则返回true
bool PUB_IsEmptyBit64(const ULONGLONG& ullNum)
{
	const unsigned long *pTmp = reinterpret_cast<const unsigned long *>(&ullNum);
	unsigned long dwTmp = 0;
	if(0 != memcmp(pTmp, &dwTmp, sizeof(unsigned long)))
	{
		return false;
	}
	if(0 != memcmp(pTmp + 1, &dwTmp, sizeof(unsigned long)))
	{
		return false;
	}
	return true;
}

//把ullNum的第iIndex位设为1
void PUB_SetBit64(ULONGLONG *pllNum, int iIndex)
{
	assert( (0 <= iIndex) && (63 >= iIndex));
	unsigned long *pTmp = reinterpret_cast<unsigned long *>(pllNum);
	if (31 >= iIndex)
	{
		*pTmp |= (0x01<<iIndex);
	}
	else
	{
		pTmp += 1;
		iIndex -= 31;
		*pTmp |= (0x01<<iIndex);
	}
}

//把ullNum的第iIndex位清0
void PUB_ClearBit64(ULONGLONG *pllNum, int iIndex)
{
	assert( (0 <= iIndex) && (63 >= iIndex));
	unsigned long *pTmp = reinterpret_cast<unsigned long *>(pllNum);
	if (31 >= iIndex)
	{		
		*pTmp &= ~((0x01)<<iIndex);
	}
	else
	{
		pTmp += 1;
		iIndex -= 31;
		*pTmp &= ~((0x01)<<iIndex);
	}
}

//判断ulNum的第iIndex位是否为1
//iIndex 为0到31
bool PUB_HasBit32(unsigned long ulNum, int iIndex)
{
	assert( (0 <= iIndex) && (31 >= iIndex));
	if((ulNum) & (0x01<<iIndex))
	{
		return true;
	}
	
	return false;
}

//把*pNum的第iIndex位设为1
void PUB_SetBit32(unsigned long *pNum, int iIndex)
{
	assert( (0 <= iIndex) && (31 >= iIndex));
	*pNum |= (0x01<<iIndex);
}

//把*pNum的第iIndex位清0
void PUB_ClearBit32(unsigned long *pNum, int iIndex)
{
	assert( (0 <= iIndex) && (31 >= iIndex));
	*pNum &= ~( (0x01)<<iIndex );
}

#if defined __DVS_7114__ || defined __DVS_7720__ || defined __DVS_2834__ || defined __DVS_9910__ || defined __DVS_131__ || defined __IPC_8180_7725__
//把double类型转换为long long int型
typedef union
{
  double value;
  struct
  {
    u_int32_t msw;
    u_int32_t lsw;
  } parts;
} ieee_double_shape_type;

#define EXTRACT_WORDS(ix0,ix1,d)				\
do {								\
  ieee_double_shape_type ew_u;					\
  ew_u.value = (d);						\
  (ix0) = ew_u.parts.msw;					\
  (ix1) = ew_u.parts.lsw;					\
} while (0)

long long PUB_llround(double x)
{
  int32_t j0;
  u_int32_t i1, i0;
  long long int result;
  int sign;

  EXTRACT_WORDS (i0, i1, x);
  j0 = ((i0 >> 20) & 0x7ff) - 0x3ff;
  sign = (i0 & 0x80000000) != 0 ? -1 : 1;
  i0 &= 0xfffff;
  i0 |= 0x100000;

  if (j0 < 20)
    {
      if (j0 < 0)
	return j0 < -1 ? 0 : sign;
      else
	{
	  i0 += 0x80000 >> j0;

	  result = i0 >> (20 - j0);
	}
    }
  else if (j0 < (int32_t) (8 * sizeof (long long int)) - 1)
    {
      if (j0 >= 52)
	result = (((long long int) i0 << 32) | i1) << (j0 - 52);
      else
	{
	  u_int32_t j = i1 + (0x80000000 >> (j0 - 20));
	  if (j < i1)
	    ++i0;

	  if (j0 == 20)
	    result = (long long int) i0;
	  else
	    result = ((long long int) i0 << (j0 - 20)) | (j >> (52 - j0));
	}
    }
  else
    {
      /* The number is too large.  It is left implementation defined
	 what happens.  */
      return (long long int) x;
    }

  return sign * result;
}
#endif

/*
*
* 输入参数说明：在Window下传入的是FILETIME结构体，而且精确到us。在Linux下传入的是timeval，也是精确到us。
* 返回值说明：在Window下和Linux下返回的值都是自1970年0:0:0开始的绝对时间,数量级是us。
*
*/
LONGLONG TimeToDVRTime(DVRDATETIME  DVRtm)   
{
#ifdef __ENVIRONMENT_WIN32__
	LONGLONG  AdjustTime = 116444736000000000;
	LONGLONG  Temptm = *((LONGLONG *)&DVRtm) ;
	Temptm -= AdjustTime;
	Temptm /= 10;
#else
	LONGLONG  Temptm = (LONGLONG)DVRtm.tv_sec*1000000 + (LONGLONG)DVRtm.tv_usec;
#endif
	return Temptm;
}

unsigned long TimeToDVRTime32(DVRDATETIME  DVRtm)   
{
#ifdef __ENVIRONMENT_WIN32__
	LONGLONG  AdjustTime = 116444736000000000;
	LONGLONG  Temptm = *((LONGLONG *)&DVRtm) ;
	Temptm -= AdjustTime;
	Temptm /= 10;
#else
	LONGLONG  Temptm = (LONGLONG)DVRtm.tv_sec*1000000 + (LONGLONG)DVRtm.tv_usec;
#endif

	return (unsigned long)(Temptm / 1000000);
}

//从标准tm到LONGLONG的转换
LONGLONG TmToDVRTime(tm time)  
{
#ifdef __ENVIRONMENT_WIN32__
	LONGLONG temp;
	if(time.tm_isdst == 0)
	{
		CTime ctime(time.tm_year+1900, time.tm_mon+1, time.tm_mday, time.tm_hour, time.tm_min, time.tm_sec);
		temp = CTmTo64Bit(ctime);
	}
	else
	{
		CTime ctime(time.tm_year+1900, time.tm_mon+1, time.tm_mday, time.tm_hour, time.tm_min, time.tm_sec,time.tm_isdst);
		temp = CTmTo64Bit(ctime);
	}
	return temp;
#else

	time_t tt=mktime(&time);
	LONGLONG temp=(LONGLONG)tt*1000000;
	return temp;
#endif
}

unsigned long TmToDVRTime32(tm time)  
{
#ifdef __ENVIRONMENT_WIN32__
	unsigned long temp;

	if(time.tm_isdst == 0)
	{
		CTime ctime(time.tm_year+1900, time.tm_mon+1, time.tm_mday, time.tm_hour, time.tm_min, time.tm_sec);
		temp = CTmTo32Bit(ctime);
	}
	else
	{
		CTime ctime(time.tm_year+1900, time.tm_mon+1, time.tm_mday, time.tm_hour, time.tm_min, time.tm_sec,time.tm_isdst);
		temp = CTmTo32Bit(ctime);
	}

	return temp;
#else
	time.tm_isdst = -1;						//tm_isdat = -1时mktime()转换当前时间到GMT时间的方式为:
											//1: time 在DST时间内时localtime - timezone - dstoffset 2: time不在DST时间内时localtime - timezone
											
											//tm_isdst = 0 时mktime()转换当前时间到GMT时间的方式为:localtime - timezone 
											
											//tm_isdst = 1 时mktime()转换当前时间到GMT时间的方式为:localtime - timezone - dstoffset
	unsigned long retValue = 0;
#if defined(__CHIPTI__ )
	retValue = (unsigned long)(mktime_ti8107(&time));
#else
	retValue = (unsigned long)(mktime(&time));
#endif
	
	return retValue;
#endif
}

#ifdef __ENVIRONMENT_WIN32__
LONGLONG CTmTo64Bit(CTime time)
{
#ifdef HAS_TIME_ZONE
	return ((LONGLONG)time.GetTime())*1000000;
#else
	LONGLONG   RetTime;
	SYSTEMTIME sysTime;
	FILETIME fileTime;

	time.GetAsSystemTime(sysTime);
	SystemTimeToFileTime(&sysTime, &fileTime);

	RetTime = TimeToDVRTime(fileTime);

	return RetTime; 
#endif
}

unsigned long CTmTo32Bit(CTime time)
{
#ifdef HAS_TIME_ZONE
	return time.GetTime();
#else
	SYSTEMTIME sysTime;
	FILETIME fileTime;

	time.GetAsSystemTime(sysTime);
	SystemTimeToFileTime(&sysTime, &fileTime);

	return TimeToDVRTime32(fileTime);
#endif
}

CTime _64BitToCTm(LONGLONG time)
{
#ifdef HAS_TIME_ZONE
	return CTime(time/1000000);
#else
	LONGLONG  AdjustTime = 116444736000000000;
	time *= 10;
	time += AdjustTime;
	FILETIME TempTime = *((FILETIME *)&time);
	SYSTEMTIME sysTime;
	FileTimeToSystemTime(&TempTime, &sysTime);
	return CTime(sysTime);
#endif
}

CTime _32BitToCTm(unsigned long time)
{
#ifdef HAS_TIME_ZONE
	return CTime(time);
#else
	LONGLONG  AdjustTime = 116444736000000000;
	LONGLONG tTime = (LONGLONG)(time) * 1000000;
	tTime *= 10;
	tTime += AdjustTime;

	FILETIME TempTime = *((FILETIME *)&tTime);
	SYSTEMTIME sysTime;
	FileTimeToSystemTime(&TempTime, &sysTime);

	return CTime(sysTime);
#endif
}
#endif
/*
*
* 输入参数说明：在Window下和Linux下值都是自1970年0:0:0开始的绝对时间，精确到us的
* 返回值说明：在Window和Liunx下返回的都是tm类型的结构体，而且只精确到s。
*
*/


tm DVRTimeToTm(LONGLONG DVRtm)
{
#ifdef __ENVIRONMENT_WIN32__
	static CPUB_Lock  s_TimeLock;
	s_TimeLock.Lock();
	CTime ctime = _64BitToCTm(DVRtm);
	struct tm DisplayTime;
	ctime.GetLocalTm(&DisplayTime);
	s_TimeLock.UnLock();
	return DisplayTime;
#else

	time_t SecNum = (long)(DVRtm / 1000000);
	struct tm tmTime;
	struct tm * ptime = localtime_r(&SecNum, &tmTime);

	return tmTime;
#endif
}

tm DVRTime32ToTm(unsigned long DVRtm)
{
#ifdef __ENVIRONMENT_WIN32__
	static CPUB_Lock  s_TimeLock;
	s_TimeLock.Lock();
	CTime ctime = _32BitToCTm(DVRtm);
	struct tm DisplayTime;
	ctime.GetLocalTm(&DisplayTime);
	s_TimeLock.UnLock();
	return DisplayTime;
#else

	//ptime; 
	time_t      SecNum = DVRtm;

	//s_TimeLock.Lock();
	struct tm tmTime;
	struct tm * ptime = localtime_r(&SecNum, &tmTime);
	//DisplayTime = *ptime;
	//s_TimeLock.UnLock();

	//DisplayTime.tm_year = DisplayTime.tm_year;
	return tmTime;
#endif
}

#ifndef __ENVIRONMENT_WIN32__
// #include "dirent.h"
// #include "ctype.h"
//设置DNS
void DNSSet(const char *pifname,char *defaultdns)
{
	assert(NULL != pifname);
	char sz[512];
	sprintf(sz,"udhcpc -i %s -s /usr/share/udhcpc/setdns.script",pifname);
	killProcessByName(sz);
	if(defaultdns)
	{
		memset(sz,0,sizeof(sz));
#if defined(__CHIPGM__)
		if (TD_2704AS_SL == CProduct::Instance()->ProductType())
		{
			sprintf(sz,"echo nameserver %s >> /tmp/dns.conf",defaultdns);
		}
		else
#endif
		{
			sprintf(sz,"echo nameserver %s >> /etc/resolv.conf",defaultdns);	
		}
		DVRSystem(sz);
	}
	memset(sz,0,sizeof(sz));
	sprintf(sz,"udhcpc -i %s -s /usr/share/udhcpc/setdns.script &",pifname);
	DVRSystem(sz);
}

//设置动态IP
void DHCPSet(const char *pifname)
{
	assert(NULL != pifname);
	char sz[512];
	sprintf(sz,"udhcpc -i %s -s /usr/share/udhcpc/setip.script",pifname);
	killProcessByName(sz);
	sprintf(sz,"udhcpc -i %s -s /usr/share/udhcpc/setip.script &",pifname);
	DVRSystem(sz);
}

//启动或者重启HTTP服务
void RestartHttpSvr(unsigned short HTTPPort, unsigned short serverPort, const char *pPath)
{
	assert (NULL != pPath);

	char starthttpstr[100]	=	{0};
	
	FILE *pFile=fopen(pPath, "w");

	if(pFile)
	{
		sprintf(starthttpstr,"NetPort=%d\n", serverPort);

		if (1 != fwrite(starthttpstr, strlen(starthttpstr), 1, pFile))
		{
			assert (false);
		}
		
		memset(starthttpstr,0,sizeof(starthttpstr));
		sprintf(starthttpstr,"ServerType=%d", 5);//3定义为使用TD24XX TD2316系列协议,5为新协议

		if (1 != fwrite(starthttpstr, strlen(starthttpstr), 1, pFile))
		{
			assert (false);
		}

		fclose(pFile);
	}

//	char curdir[100];

//	getcwd(curdir, sizeof(curdir));

	memset(starthttpstr, 0, sizeof(starthttpstr));

	sprintf(starthttpstr,"cd /mnt/mtd; ./httpd -h ./WebPage/ -p %d", HTTPPort);
	
	DVRSystem(starthttpstr);
}

//杀掉含有名称的
#define READ_BUF_SIZE 200
//BUGS 简单改了一下，现在只支持pParam和命令后面跟的第一个参数进行比较
long killProcessByName(char *pidName, const char *pParam)
{
	DIR *dir;
	struct dirent *next;	
	long sum=0;

	dir = opendir("/proc");
	if (!dir)
	{
//		fprintf(stderr, "Cannot open /proc\n");
		return -1;
    }
    
	while ((next = readdir(dir)) != NULL)
	{
		FILE *status;
		char filename[READ_BUF_SIZE] = {0};
		char buffer[READ_BUF_SIZE] = {0};
		
		/* Must skip ".." since that is outside /proc */
		if (strcmp(next->d_name, "..") == 0)
			continue;
		/* If it isn't a number, we don't want it */
		if (!isdigit(*next->d_name))
			continue;
		sprintf(filename, "/proc/%s/cmdline", next->d_name);
		if (! (status = fopen(filename, "rb")) )
		{
			continue;
		}
		/* Read first line in /proc/?pid?/status */
		if (fgets(buffer, READ_BUF_SIZE-1, status) == NULL)
		{
			fclose(status);
			continue;
		}
		
		if (strstr(buffer, pidName) !=NULL) 
		{
			if (NULL != pParam)
			{
				char *pbuffer = buffer + strlen(buffer) + 1;
// 				char paramBuff[1048] = {0};
// 				fread(paramBuff, 1, sizeof(paramBuff) - 1, status);
//				printf("\n\n\n\n#################  %s %d \n%s %s\n\n\n\n\n ",__FILE__, __LINE__, pbuffer, pParam);
// 				
				if(strstr(pbuffer, pParam) == NULL)
				{
//					printf("\n\n\n\n#################  %s %d\n\n\n\n\n ",__FILE__, __LINE__);
					fclose(status);
					continue;
				}
			}

			int procid=strtol(next->d_name, NULL, 0);
			char cmdline[100];
#if defined __DVS_7114__ || defined __DVS_7720__ || defined __DVS_2834__
			sprintf(cmdline,"kill -s SIGKILL %d",procid);
#else
			sprintf(cmdline,"kill -SIGKILL %d",procid);
#endif

#if defined __DVS_7114__ || defined __DVS_7720__ || defined __DVS_2834__ || defined __DVS_9910__ || defined __DVS_131__ || defined __IPC_8180_7725__
			CCmdClient::Instance()->execute(cmdline);
#else
			DVRSystem(cmdline);
#endif
			sum++;
		}
		fclose(status);
	}
	closedir(dir);
	return sum;
}

//检测进程pidName是否在运行
bool IsProcessRunning(char *pidName, const char *pParam )
{
	DIR *pDIR = NULL;
	struct dirent *pDirent;
	char filename[100];
	char cmdline[200];
	FILE *pCmdLinefp = NULL;

	pDIR =  opendir("/proc");

	assert( NULL != pDIR );

	while( NULL != ( pDirent = readdir( pDIR ) ) )
	{
		if( !isdigit( *( pDirent->d_name ) ) )
		{
			continue;
		}

		sprintf( filename, "/proc/%s/cmdline", pDirent->d_name);
		if( NULL != ( pCmdLinefp = fopen( filename, "rb")))
		{
			if ( NULL != fgets( cmdline, 199, pCmdLinefp ) )
			{
				if( NULL != strstr(cmdline, pidName ) )
				{
					char *pParamStart = cmdline + strlen( cmdline ) + 1;
					if(  ( NULL != pParam ) && ( NULL == strstr( pParamStart, pParam ) ) )
					{
						fclose( pCmdLinefp );
						continue;
					}

					fclose( pCmdLinefp );
					closedir(pDIR);
					return true;
				}
			}

			fclose( pCmdLinefp );
		}

	}

	closedir(pDIR);
	return false;
}
#endif

CTimeInterval::CTimeInterval()
{
#ifdef __ENVIRONMENT_WIN32__
	LARGE_INTEGER litmp; 
	QueryPerformanceFrequency(&litmp);
	m_dfFreq = (double)litmp.QuadPart;// 获得计数器的时钟频率	
	m_QPart1 = 0;
	m_QPart2 = 0;
#else
	m_starttime=0;
#endif
}

CTimeInterval::~CTimeInterval()
{
}

void CTimeInterval::Start()
{
#ifdef __ENVIRONMENT_WIN32__ 
	LARGE_INTEGER litmp; 
	QueryPerformanceCounter(&litmp);
	m_QPart1 = litmp.QuadPart;// 获得初始值
#else
	struct timeb tb;
	ftime(&tb);	
	m_starttime=tb.time*1000;
	m_starttime+=tb.millitm;
#endif
	
}

int CTimeInterval::GetCurInterval() //结束计时，返回和执行Start时的时间差
{
#ifdef __ENVIRONMENT_WIN32__ 
	LARGE_INTEGER litmp;
	QueryPerformanceCounter(&litmp); 
	m_QPart2 = litmp.QuadPart;// 获得结束值
	if(!m_QPart1)  ////////////没有调用开始就会返回－1
		return -1;
	double	count = (double)(m_QPart2-m_QPart1);		
	double  dfTim = count / m_dfFreq;// 获得对应的时间值，单位为秒
	return  (int)(dfTim*1000) ;  ////
#else
	struct timeb tb;
	ftime(&tb);	
	LONGLONG curtime=tb.time*1000;
	curtime+=tb.millitm;
	int timeInter=(int)(curtime-m_starttime);
	return timeInter;
#endif

}

int CTimeInterval::Restart() //结束计时，返回和执行Start时的时间差
{
	int timeInter=GetCurInterval();
	Start();
	return timeInter;
}

/****************************************************************************
*
*                               class CWaitEvent
*
****************************************************************************/
CWaitEvent::CWaitEvent()
{

}

CWaitEvent::~CWaitEvent()
{

}

void CWaitEvent::CreateWaitEvent()
{
#ifdef __ENVIRONMENT_WIN32__
	m_Event = CreateEvent(NULL, true, false, NULL);  
#else
	pthread_mutex_init(&m_lock, NULL);
	pthread_cond_init(&m_Event, NULL);
#endif
}

unsigned long CWaitEvent::WaitForCond(DWORD dwMilliseconds)
{
#ifdef __ENVIRONMENT_WIN32__
	unsigned long ret = WaitForSingleObject(m_Event, dwMilliseconds);
	ResetEvent(m_Event);

	return ret;
#else
	if(dwMilliseconds == INFINITE)
	{
		pthread_mutex_lock(&m_lock);
		pthread_cond_wait(&m_Event, &m_lock);
		pthread_mutex_unlock(&m_lock);
	}
	else
	{
		struct timespec ts;
		Gettimespec(&ts, dwMilliseconds);
		pthread_mutex_lock(&m_lock);
		pthread_cond_timedwait(&m_Event, &m_lock, &ts);
		pthread_mutex_unlock(&m_lock);
	}

	return 0;
#endif	
}

void CWaitEvent::SetEvent()
{
#ifdef	__ENVIRONMENT_WIN32__ 
	::SetEvent(m_Event);
#else
	pthread_cond_signal(&m_Event);
#endif
}

void CWaitEvent::PulseEvent()
{
#ifdef __ENVIRONMENT_WIN32__
	::PulseEvent(m_Event);  
#else
	pthread_cond_broadcast(&m_Event);
#endif
}

#ifndef __ENVIRONMENT_WIN32__
void CWaitEvent::Gettimespec(struct timespec* ptimespec, int offset) 
{ 
	struct timeval   now; 
	struct timespec  *ptime = (struct timespec *)ptimespec; 
	
	gettimeofday(&now, NULL); 
	ptime->tv_sec = now.tv_sec; 
	
	int tmp = now.tv_usec + offset * 1000; ////tmp是us级别的
	ptime->tv_sec = ptime->tv_sec + (tmp/(1000 * 1000));
	ptime->tv_nsec = (tmp % (1000 * 1000)) * 1000; 
} 
#endif

void CWaitEvent::Close()
{
#ifdef  __ENVIRONMENT_WIN32__
	CloseHandle(m_Event);
	m_Event = NULL;
#else
	pthread_cond_destroy(&m_Event);
	pthread_mutex_destroy(&m_lock);
#endif
}

//ysw
/***********************************************************************/

/********************************************************************************
功能描述：
考虑到操作文件的过程中，需要对IO的返回值进行处理，但是其结构要么嵌套
过多，要么就是显示返回的地方过多。一般会选择返回式结构，但是每一个返回
的地方都需要关闭文件及释放资源，且动作基本一样。最大的不同就是需要释放的
资源个数不定，所以不能用宏来做，只能选择用这种不定参数的函数来做。
原理介绍：
采用把指针的指针给传入，释放后并修改其指针值为空。
第一个参数固定为文件句柄的指针，如果不为空则关闭文件并修改句柄值。
第二个参数国定为其后参数的个数，这个参数且不可出错。
由于后面的都是需要释放的指针，且没有指向对象的类型，所以不定参数的指针
指向的对象不能是自定义类，必须是基本类型。
**********************************************************************************/
void Release (FILE **pFile, unsigned long num, ...)
{
	va_list arg_ptr;
	void **ptr = NULL;

	if ((NULL != pFile) && (NULL != *pFile))
	{
		fclose (*pFile);
		*pFile = NULL;
	}

	va_start(arg_ptr, num);

	for (unsigned long i=0; i<num; ++i)
	{
		ptr = (void **)va_arg(arg_ptr, void **);

		if (NULL != ptr)
		{
			delete [] *ptr;
			*ptr = NULL;
		}
	}

	va_end(arg_ptr);
}


LONGLONG GetCurrTime()
{
	LONGLONG time = 0;
#ifdef __ENVIRONMENT_WIN32__
	SYSTEMTIME systemtime;
	FILETIME  filetime;
	GetSystemTime(&systemtime);
	if (SystemTimeToFileTime(&systemtime, &filetime))
	{
		time = TimeToDVRTime(filetime);
	}
#else
	timeval tp;
	if (0 == gettimeofday(&tp, NULL))
		time = TimeToDVRTime(tp);
#endif
	return time;
}//GetCurrTime end.

unsigned long GetCurrTime32()
{
	unsigned long time = 0;

#ifdef __ENVIRONMENT_WIN32__
	SYSTEMTIME systemtime;
	FILETIME  filetime;
	GetSystemTime(&systemtime);
	if (SystemTimeToFileTime(&systemtime, &filetime))
	{
		time = TimeToDVRTime32(filetime);
	}
#else
	timeval tp;
	if (0 == gettimeofday(&tp, NULL))
		time = TimeToDVRTime32(tp);
#endif

	return time;
}

LONGLONG GetZeroTimeOfDate(LONGLONG time)
{
	tm display = DVRTimeToTm(time);

	display.tm_hour = 0;
	display.tm_sec = 0;
	display.tm_min = 0;

	return TmToDVRTime(display);
}

unsigned long GetZeroTime32OfDate(unsigned long time)
{
	tm display = DVRTime32ToTm(time);

	display.tm_hour = 0;
	display.tm_sec = 0;
	display.tm_min = 0;

	return TmToDVRTime32(display);
}

LONGLONG GetEndTimeOfDate(LONGLONG time)
{
	tm display = DVRTimeToTm(time);

	display.tm_hour = 23;
	display.tm_sec = 59;
	display.tm_min = 59;

	return TmToDVRTime(display) + 999999;
}

unsigned long GetEndTime32OfDate(unsigned long time)
{
	tm display = DVRTime32ToTm(time);

	display.tm_hour = 23;
	display.tm_sec = 59;
	display.tm_min = 59;

	return TmToDVRTime32(display);
}

//保证长度
//0:全部，1:日期，2:时分秒
bool TimeToStr(LONGLONG time, DATE_MODE mode, long format, char *pStr, unsigned long len)
{
	if (0 == format)
	{
		assert (len > 19);
		if (len <= 19)
		{
			return false;
		}
	}
	else if (1 == format)
	{
		assert (len > 10);
		if (len <= 10)
		{
			return false;
		}
	}
	else
	{
		assert (len > 9);
		if (len <= 9)
		{
			return false;
		}
	}

	memset (pStr, 0, len);
	tm tm_time = DVRTimeToTm(time);
	tm_time.tm_year += 1900;
	tm_time.tm_mon ++;

	if (2 > format)//0,1
	{
		if (DATE_MODE_YMD == mode)
		{
			sprintf(pStr, "%lu/%02lu/%02lu", tm_time.tm_year, tm_time.tm_mon, tm_time.tm_mday);
		}
		else if (DATE_MODE_MDY == mode)
		{
			sprintf(pStr, "%02lu/%02lu/%lu", tm_time.tm_mon, tm_time.tm_mday, tm_time.tm_year);
		}
		else//DATE_MODE_DMY
		{
			sprintf(pStr, "%02lu/%02lu/%lu", tm_time.tm_mday, tm_time.tm_mon, tm_time.tm_year);
		}
	}

	if (0 == format)//时分秒
	{
		pStr[10] = ' ';
		sprintf(pStr+11, "%02lu:%02lu:%02lu", tm_time.tm_hour, tm_time.tm_min, tm_time.tm_sec);
	}
	
	if (2 == format)
	{
		sprintf(pStr, "%02lu:%02lu:%02lu", tm_time.tm_hour, tm_time.tm_min, tm_time.tm_sec);
	}

	return true;
}

//noTime false 时间 true 数字
void PrintfBit64(ULONGLONG n_num, bool noTime)
{
	if (noTime)
	{
		unsigned int h = (ULONG)(0x00000000ffffffff&(n_num>>32));
		unsigned int l = (ULONG)(0x00000000ffffffff&n_num);
		printf("%8x %8x\n", h, l);
	}
	else
	{
		tm time = DVRTimeToTm(n_num);
		int ms = static_cast<int>(n_num%1000000);

		printf("%d-%d-%d %d:%d:%d ms:%d\n", time.tm_year, time.tm_mon, time.tm_mday, time.tm_hour, time.tm_min, time.tm_sec, ms);
	}
}

void PrintfBit32Time(unsigned long timeSec)
{
	tm time = DVRTime32ToTm(timeSec);
	printf("%d-%d-%d %d:%d:%d\n", time.tm_year, time.tm_mon, time.tm_mday, time.tm_hour, time.tm_min, time.tm_sec);

}

BYTE GetChnnForbits(ULONGLONG chnnbits)
{
	BYTE chnn = 0;
	for (chnn=0; chnn<4; ++chnn)
	{
		if (0x01 & (chnnbits>>chnn))
			return chnn;
	}
	return chnn;
}

bool IsValidIPCfg(DWORD dwIP,DWORD dwSubnetMask,DWORD dwGateWay)
{
//检验掩码的合法性(从左到右为1的位必须连续)
	 DWORD dwTemp = dwSubnetMask;
	 for(int i=0;i<32&&dwTemp;i++)
	 {
		 if(!(dwTemp&(0x01<<31)))		//中间出现了为断位
			 return false;
		 dwTemp<<=1;
	 }
////检验网络号
	BYTE nField0;
	nField0 = (BYTE)(dwIP>>24)&0xff;
	if(nField0 == 127)
	{
		return false;
	}
	else if(nField0>233||nField0==0)
	{
		return false;
	}
	else
	{
		if(nField0<=126)
		{
			if(~dwSubnetMask&0xff000000)
			{
				return false;
			}
			if((dwGateWay^dwIP)&0xff000000)
			{
				return false;
			}
		}
		else if(nField0<=191)
		{
			if(dwSubnetMask^0xffff0000)//must be 255.255.?.?
			{
				return false;
			}
			if((dwGateWay^dwIP)&0xffff0000)
			{
				return false;
			}
		}
		else
		{
			if(dwSubnetMask^0xffffff00)//must be 255.255.255.?
			{
				return false;
			}
			if((dwGateWay^dwIP)&0xffffff00)
			{
				return false;
			}
		}
	}
// 检验主机号
	if(!(~dwSubnetMask&dwIP))
	{
		return false;
	}
	if(!(~dwSubnetMask&dwGateWay))
	{
		return false;
	}
	if((~dwSubnetMask&dwIP)==~dwSubnetMask)
	{
		return false;
	}
	return true;
}

int DVRSystem(const char *cmdstring)
{
	static CPUB_Lock FunctionLock;
	FunctionLock.Lock();

#ifndef __ENVIRONMENT_WIN32__
	int fdserver = open(SERVER_FIFO_NAME, O_WRONLY);
	while(-1 == fdserver)
	{
		perror("Open file error:");
		printf("%s, %d, Client: Can't open fifo. \n", __FILE__, __LINE__);
		fflush(stdout);
		assert(false);
		PUB_Sleep(1000);
		fdserver = open(SERVER_FIFO_NAME, O_WRONLY);
	}

	int fdclient = open(CLIENT_FIFO_NAME, O_RDWR);
	while(-1 == fdclient)
	{
		perror("Open file error:");
		printf("%s, %d, Cann't open client fifo. \n", __FILE__, __LINE__);
		PUB_Sleep(1000);
		fdclient = open(CLIENT_FIFO_NAME, O_RDWR);
	}

	if (-1 == write(fdserver, cmdstring, strlen(cmdstring)))
	{
		close (fdserver);
		close (fdclient);

		printf("write cmd into pipo fail\n");
		fflush(stdout);
		assert(false);
		exit(1);
	}

	close(fdserver);

	char  Replaystr[10] = {0};
	if (-1 == read(fdclient, Replaystr, sizeof(Replaystr)))
	{
		close (fdserver);
		close (fdclient);

		printf("read reply from server fail\n");
		fflush(stdout);
		assert(false);
		exit(1);
	}

	close(fdclient);

	if(0 == strcmp(Replaystr, "SUCCESS"))
	{
		FunctionLock.UnLock();
		return 0;
	}
#endif

	FunctionLock.UnLock();
	return -1;
}

#ifndef __ENVIRONMENT_WIN32__
static CThreadManager g_ThreadManager;
CThreadManager::CThreadManager()
{
	m_ThreadManagerExit=false;
	m_ThreadManagerThread= PUB_CreateThread(ThreadManagerProc, this, NULL);
}
CThreadManager::~CThreadManager()
{
	m_ThreadManagerExit=true;
	pthread_join(m_ThreadManagerThread, NULL);
//	PUB_ExitThread(&m_ThreadManagerThread, NULL);
}
void CThreadManager::ExitThread(HANDLE handle)
{
	g_ThreadManager.m_ThreadListLock.Lock();
	g_ThreadManager.m_ThreadList.AddTail(handle);
	g_ThreadManager.m_ThreadListLock.UnLock();
}
RESULT WINAPI CThreadManager::ThreadManagerProc(LPVOID lpParameter)
{
	pid_t pid = getpid();
	pid_t tid = syscall(__NR_gettid);
	printf("%s, %d, pid = %d, tid=%d\n", __FILE__, __LINE__, pid, tid);

	CThreadManager *pManager=(CThreadManager*)lpParameter;
	pManager->ThreadManagerFunc();
	return 0;
}
void CThreadManager::ThreadManagerFunc()
{
	HANDLE handle=0;
	while(!m_ThreadManagerExit)
	{
		handle=0;
		m_ThreadListLock.Lock();
		if(m_ThreadList.GetCount() > 0)
			handle=m_ThreadList.RemoveHead();
		m_ThreadListLock.UnLock();
		if(handle)
			PUB_ExitThread(&handle, NULL);
		PUB_Sleep(100);
	}
}

char *DVRStrToUper(char *pStr)
{
	for(unsigned int i=0;i<strlen(pStr);i++)
	{
		pStr[i]=toupper(pStr[i]);	
	}
	return pStr;
}

unsigned long GetTickCount()
{
	struct timespec tp;
	if(!clock_gettime(CLOCK_MONOTONIC, &tp) < 0){assert(false);}
	return tp.tv_sec * 1000 + tp.tv_nsec / 1000000;
}

LONGLONG GetTickCount64()
{
	struct timespec tp;
	if(!clock_gettime(CLOCK_MONOTONIC, &tp) < 0){assert(false);}
	LONGLONG llTC = tp.tv_sec;
	llTC = llTC * 1000000 + tp.tv_nsec / 1000;
	return llTC;
}
#endif

const unsigned long MAX_UNSIGNED_32BIT_NUM = 0x80000000 + 0x7fffffff;

//是否从dwStartTime毫秒开始计算，过了dwDisTime毫秒
//BUGS: 超了两圈以上就没办法了
bool IsTimeOut(unsigned long dwStartTime, unsigned long dwDisTime, unsigned long *pCurTime)
{
	unsigned long  dwNowTime = GetTickCount();
	if (NULL != pCurTime)
	{
		*pCurTime = dwNowTime;
	}

	if (dwNowTime >= dwStartTime)
	{
		if((dwNowTime - dwStartTime) >= dwDisTime) 
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		if(((MAX_UNSIGNED_32BIT_NUM - dwStartTime) + 1 + dwNowTime) >= dwDisTime)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
}

bool GetKernelverionEx(unsigned long *pVersion)
{
	char version[12] = {0};
	FILE* pfd = fopen("/proc/sys/kernel/version","r");
	if ( NULL == pfd )
	{
		return false;
	}
	
	char str[80] = {0};
	fgets(str,80,pfd);	
	fclose(pfd);

	memcpy(version,str+1,8);
	*pVersion = ((version[0] - '0') << 28) + 
		((version[1] - '0') << 24) +
		((version[2] - '0') << 20) +
		((version[3] - '0') << 16) +
		((version[4] - '0') << 12) +
		((version[5] - '0') << 8) +
		((version[6] - '0') << 4) +
		((version[7] - '0'));
	
	return true;
}


void AreaInfo(unsigned short width, unsigned short height, VIEW_SPLIT_MODE mode, DISPLAY_ATTRIB & display, unsigned char index, unsigned short left, unsigned short top)
{
	assert ((0 < width) && (width <= 1920));
	assert ((0 < height) && (height <= 1080));

	unsigned short w = width;
	unsigned short h = height;
	unsigned short t = 1;

	unsigned char map_index[6][6] = {-1};

	switch(mode)
	{
	case VIEW_SPLIT_2X2:
		w = width / 2;
		h = height / 2;
		t = 2;
		break;
	case VIEW_SPLIT_2X3:
		w = width / 3;
		h = height / 2;
		t = 3;
		break;
	case VIEW_SPLIT_1A5:
		{
			w = width / 3;
			h = height / 3;
			t = 3;

			unsigned char map_1a5[6][6] = { 0,  0,  1,  -1, -1,  -1,
											0,  0,  2,  -1, -1,  -1,
											3,  4,  5,  -1, -1,  -1,
											-1, -1, -1, -1, -1,  -1,
											-1, -1, -1, -1, -1,  -1,
											-1, -1, -1, -1, -1,  -1 };
			memcpy(map_index, map_1a5, sizeof(map_index));
		}
		break;
	case VIEW_SPLIT_3X3:
		w = width / 3;
		h = height / 3;
		t = 3;
		break;
	case VIEW_SPLIT_1A7:
		{
			w = width / 4;
			h = height / 4;
			t = 4;

			unsigned char map_1a7[6][6] = { 0,  0,  0,  1,  -1,  -1,
											0,  0,  0,  2,  -1,  -1,
											0,  0,  0,  3,  -1,  -1,
											4,  5,  6,  7,  -1,  -1,
											-1, -1, -1, -1, -1,  -1,
											-1, -1, -1, -1, -1,  -1 };
			memcpy(map_index, map_1a7, sizeof(map_index));
		}
		break;
	case VIEW_SPLIT_1A12:
		{
			w = width / 4;
			h = height / 4;
			t = 4;

			unsigned char map_1a12[6][6] = {0,  1,  2,  3,  -1,  -1,
											4,  5,  5,  6,  -1,  -1,
											7,  5,  5,  8,  -1,  -1,
											9,  10, 11, 12, -1,  -1,
											-1, -1, -1, -1, -1,  -1,
											-1, -1, -1, -1, -1,  -1 };
			memcpy(map_index, map_1a12, sizeof(map_index));
		}
		break;
	case VIEW_SPLIT_4X4:
		{
			w = width / 4;
			h = height / 4;
			t = 4;
		}
		break;
	case VIEW_SPLIT_4X6:
		{
			w = width / 6;
			h = height / 4;
			t = 6;
		}
	case VIEW_SPLIT_5X5:
		w = width / 5;
		h = height / 5;
		t = 5;
		break;
	case VIEW_SPLIT_6X6:
		w = width / 6;
		h = height / 6;
		t = 6;
		break;
	default:
		w = width;
		h = height;
		t = 1;
		break;
	}

	switch(mode)
	{
	case VIEW_SPLIT_1A5:
	case VIEW_SPLIT_1A7:
	case VIEW_SPLIT_1A12:
		{
			int start_i = -1, start_j = -1;
			int end_i = -1, end_j = -1;

			for(int j = 0; j < t; j++)
			{
				for(int i = 0; i < t; i++)
				{
					if(map_index[j][i] == index)
					{
						start_i = (start_i >= 0) ? start_i : i;
						start_j = (start_j >= 0) ? start_j : j;

						end_i = i;
						end_j = j;
					}
				}
			}

			display.xPos	= left + (w * start_i);
			display.yPos	= top + (h * start_j);
			display.cx		= w * (end_i - start_i + 1);
			display.cy		= h * (end_j - start_j + 1);
		}
		break;
	default:
		{
			display.xPos	= left + (w * (index % t));
			display.yPos	= top + (h * (index / t));
			display.cx		= w;
			display.cy		= h;
		}
		break;
	}
}
void AreaInfoEx(unsigned short offsetWidth, unsigned short offsetHeight, unsigned short width, unsigned short height, VIEW_SPLIT_MODE mode, DISPLAY_ATTRIB & display, unsigned char index, unsigned short left/* = 0*/, unsigned short top/* = 0*/)
{
	assert ((0 < width) && (width <= 1920));
	assert ((0 < height) && (height <= 1080));

	unsigned short w = width;
	unsigned short h = height;

	unsigned short t = 1;

	unsigned char map_index[6][6] = {-1};

	switch(mode)
	{
	case VIEW_SPLIT_2X2:
		w = width / 2;
		h = height / 2;
		t = 2;
		break;
	case VIEW_SPLIT_2X3:
		w = width / 3;
		h = height / 2;
		t = 3;
		break;
	case VIEW_SPLIT_1A5:
		{
			w = width / 3;
			h = height / 3;
			t = 3;

			unsigned char map_1a5[6][6] = { 0,  0,  1,  -1, -1,  -1,
				0,  0,  2,  -1, -1,  -1,
				3,  4,  5,  -1, -1,  -1,
				-1, -1, -1, -1, -1,  -1,
				-1, -1, -1, -1, -1,  -1,
				-1, -1, -1, -1, -1,  -1 };
			memcpy(map_index, map_1a5, sizeof(map_index));
		}
		break;
	case VIEW_SPLIT_3X3:
		w = width / 3;
		h = height / 3;
		t = 3;
		break;
	case VIEW_SPLIT_1A7:
		{
			w = width / 4;
			h = height / 4;
			t = 4;

			unsigned char map_1a7[6][6] = { 0,  0,  0,  1,  -1,  -1,
				0,  0,  0,  2,  -1,  -1,
				0,  0,  0,  3,  -1,  -1,
				4,  5,  6,  7,  -1,  -1,
				-1, -1, -1, -1, -1,  -1,
				-1, -1, -1, -1, -1,  -1 };
			memcpy(map_index, map_1a7, sizeof(map_index));
		}
		break;
	case VIEW_SPLIT_1A12:
		{
			w = width / 4;
			h = height / 4;
			t = 4;

			unsigned char map_1a12[6][6] = {0,  1,  2,  3,  -1,  -1,
				4,  5,  5,  6,  -1,  -1,
				7,  5,  5,  8,  -1,  -1,
				9,  10, 11, 12, -1,  -1,
				-1, -1, -1, -1, -1,  -1,
				-1, -1, -1, -1, -1,  -1 };
			memcpy(map_index, map_1a12, sizeof(map_index));
		}
		break;
	case VIEW_SPLIT_4X4:
		{
			w = width / 4;
			h = height / 4;
			t = 4;
		}
		break;
	case VIEW_SPLIT_4X6:
		{
			w = width / 6;
			h = height / 4;
			t = 6;
		}
	case VIEW_SPLIT_5X5:
		w = width / 5;
		h = height / 5;
		t = 5;
		break;
	case VIEW_SPLIT_6X6:
		w = width / 6;
		h = height / 6;
		t = 6;
		break;
	default:
		w = width;
		h = height;
		t = 1;
		break;
	}

	switch(mode)
	{
	case VIEW_SPLIT_1A5:
	case VIEW_SPLIT_1A7:
	case VIEW_SPLIT_1A12:
		{
			int start_i = -1, start_j = -1;
			int end_i = -1, end_j = -1;

			for(int j = 0; j < t; j++)
			{
				for(int i = 0; i < t; i++)
				{
					if(map_index[j][i] == index)
					{
						start_i = (start_i >= 0) ? start_i : i;
						start_j = (start_j >= 0) ? start_j : j;

						end_i = i;
						end_j = j;
					}
				}
			}

			display.xPos	= left + (w * start_i) + offsetWidth;
			display.yPos	= top + (h * start_j) + offsetHeight;
			display.cx		= w * (end_i - start_i + 1);
			display.cy		= h * (end_j - start_j + 1);
		}
		break;
	default:
		{
			display.xPos	= left + (w * (index % t)) + offsetWidth;
			display.yPos	= top + (h * (index / t)) + offsetHeight;
			display.cx		= w;
			display.cy		= h;
		}
		break;
	}
}
int AreaIndex(unsigned short width, unsigned short height, VIEW_SPLIT_MODE mode, unsigned short x, unsigned short y, unsigned short cx, unsigned short cy)
{
	//计算矩形区域(x,y,cx,cy)所在的屏幕编号
	//返回-1,说明区域(x,y,cx,cy)无效
	//以区域中心点来做判断

	unsigned short w = width;
	unsigned short h = height;
	unsigned short t = 1;

	int map_index[6][6] = {-1};

	switch(mode)
	{
	case VIEW_SPLIT_2X2:
		w = width / 2;
		h = height / 2;
		t = 2;
		break;
	case VIEW_SPLIT_2X3:
		w = width / 3;
		h = height / 2;
		t = 3;
		break;
	case VIEW_SPLIT_1A5:
		{
			w = width / 3;
			h = height / 3;
			t = 3;

			int map_1a5[6][6] ={0,  0,  1,  -1, -1,  -1,
								0,  0,  2,  -1, -1,  -1,
								3,  4,  5,  -1, -1,  -1,
								-1, -1, -1, -1, -1,  -1,
								-1, -1, -1, -1, -1,  -1,
								-1, -1, -1, -1, -1,  -1 };
			memcpy(map_index, map_1a5, sizeof(map_index));
		}
		break;
	case VIEW_SPLIT_3X3:
		{
			w = width / 3;
			h = height / 3;
			t = 3;
		}
		break;
	case VIEW_SPLIT_1A7:
		{
			w = width / 4;
			h = height / 4;
			t = 4;

			int map_1a7[6][6]={ 0,  0,  0,  1,  -1,  -1,
								0,  0,  0,  2,  -1,  -1,
								0,  0,  0,  3,  -1,  -1,
								4,  5,  6,  7,  -1,  -1,
								-1, -1, -1, -1, -1,  -1,
								-1, -1, -1, -1, -1,  -1 };
			memcpy(map_index, map_1a7, sizeof(map_index));
		}
		break;
	case VIEW_SPLIT_1A12:
		{
			w = width / 4;
			h = height / 4;
			t = 4;

			int map_1a12[6][6]={0,  1,  2,  3,  -1,  -1,
								4,  5,  5,  6,  -1,  -1,
								7,  5,  5,  8,  -1,  -1,
								9,  10, 11, 12, -1,  -1,
								-1, -1, -1, -1, -1,  -1,
								-1, -1, -1, -1, -1,  -1 };
			memcpy(map_index, map_1a12, sizeof(map_index));
		}
		break;
	case VIEW_SPLIT_4X4:
		{
			w = width / 4;
			h = height / 4;
			t = 4;
		}
		break;
	case VIEW_SPLIT_4X6:
		{
			w = width / 6;
			h = height / 4;
			t = 6;
		}
		break;
	case VIEW_SPLIT_5X5:
		{
			w = width / 5;
			h = height / 5;
			t = 5;
		}
		break;
	case VIEW_SPLIT_6X6:
		{
			w = width / 6;
			h = height / 6;
			t = 6;
		}
		break;
	default:
		w = width;
		h = height;
		t = 1;
		break;
	}

	int index = -1;

	unsigned short center_x = x + cx / 2;
	unsigned short center_y = y + cy / 2;

	if((center_x < width) && (center_y < height))
	{
		index = (center_y / h) * t + center_x / w;
	}

	if(index >= 0)
	{
		switch(mode)
		{
		case VIEW_SPLIT_1A5:
		case VIEW_SPLIT_1A7:
		case VIEW_SPLIT_1A12:
			{
				index = map_index[index / t][index % t];
			}
			break;
		default:
			break;
		}
	}

	return index;
}

void GetVGAResolution(VGA_RESOLUTION resolution, unsigned short & width, unsigned short & height)
{
	switch(resolution)
	{
	case VGA_640X480:
		width	= 640;
		height	= 480;
		break;
	case VGA_720X480:
#ifdef __TD2316SES_SPEC__
		width   = 704;
#elif defined(__TDFH__) || defined(__CHIPTI__)
		width = 704;
#else
#ifdef __ENVIRONMENT_LINUX__
		if (g_bCVBSChangeResolution)
		{
			width = 704;
		}
		else
		{
			width	= 720;
		}
#else
		width = 720;
#endif
#endif
		height	= 480;
		break;
	case VGA_720X576:
#ifdef __TD2316SES_SPEC__
		width   = 704;
#elif defined(__TDFH__) || defined(__CHIPTI__)
		width = 704;
#else
#ifdef __ENVIRONMENT_LINUX__
		if (g_bCVBSChangeResolution)
		{
			width = 704;
		}
		else
		{
			width	= 720;
		}
#else
		width = 720;
#endif
#endif
		height	= 576;
		break;
	case VGA_800X600:
		width	= 800;
		height	= 600;
		break;
	case VGA_1024X768:
		width	= 1024;
		height	= 768;
		break;
	case VGA_1280X960:
		width	= 1280;
		height	= 960;
		break;
	case VGA_1280X1024:
		width	= 1280;
		height	= 1024;
		break;
	case VGA_1920X1080:
		width	= 1920;
		height	= 1080;
		break;
	case VGA_1440X960:
		width	= 1440;
		height	= 960;
		break;
	case VGA_1440X1152:
		width	= 1440;
		height	= 1152;
		break;
	default:
		width	= 1024;
		height	= 768;
		break;
	}
}

void GetVGAResolution(VGA_RESOLUTION resolution, char *pName)
{
	assert(pName != NULL);
	switch(resolution)
	{
	case VGA_640X480:
		strcpy(pName, "CVBS");
		break;
	case VGA_720X480:
		strcpy(pName, "720X480");
		break;
	case VGA_720X576:
		strcpy(pName, "720X576");
		break;
	case VGA_800X600:
		strcpy(pName, "800X600");
		break;
	case VGA_1024X768:
		strcpy(pName, "1024X768");
		break;
	case VGA_1280X960:
		strcpy(pName, "1280X960");
		break;
	case VGA_1280X1024:
		strcpy(pName, "1280X1024");
		break;
	case VGA_1920X1080:
		strcpy(pName, "1920X1080");
		break;
	case VGA_1440X960:
		strcpy(pName, "1440x960");
		break;
	case VGA_1440X1152:
		strcpy(pName, "1440X1152");
		break;
	default:
		assert(false);
		break;
	}
}

void GetCVBSResolution(VIDEO_FORMAT videoFormat, unsigned short & width, unsigned short & height, bool adjust/*=false*/)
{
	unsigned short w = 0;
	unsigned short h = 0;

	if (VIDEO_FORMAT_PAL == videoFormat)
	{
		GetVGAResolution(VGA_720X576, w, h);
		if (adjust)
		{
#if defined(__CHIP3531__) || defined(__CHIPGM__) 
			w = 620;
#else
			w = 640;
#endif
			h = 516;
		}

	}
	else
	{
		GetVGAResolution(VGA_720X480, w, h);
		if (adjust)
		{
#if defined(__CHIP3531__) || defined(__CHIPGM__) 
			w = 620;
#else
			w = 640;
#endif
			h = 420;
		}
	}

	width = w;
	height = h;

	//printf("%s:%s:%d, width=%d, height=%d\n", __FUNCTION__, __FILE__, __LINE__, width, height);
}


//以CIF的帧率为标准返回某个资源数目。
unsigned long UsedRate(unsigned long rate, VIDEO_SIZE videoSize)
{
	//assert (0 < rate);
#if defined(__DVR_BASIC__)
	VIDEO_SIZE videoSizeTemp = CProduct::Instance()->RealVideoSize(videoSize);
#else
	VIDEO_SIZE videoSizeTemp = videoSize;
#endif

	if (DVRVIDEO_SIZE_D1 == videoSizeTemp || DVRVIDEO_SIZE_VGA == videoSizeTemp )
	{
		return rate << 2;
	}
	else if (DVRVIDEO_SIZE_HD1 == videoSizeTemp)
	{
		return rate << 1;
	}
	else if ((DVRVIDEO_SIZE_HD720 == videoSizeTemp)/* || (DVRVIDEO_SIZE_960H == videoSizeTemp)*/)
	{
		return rate << 1;
	}
	else if ((DVRVIDEO_SIZE_HD1080 == videoSizeTemp) || (DVRVIDEO_SIZE_960H == videoSizeTemp) || (DVRVIDEO_SIZE_960P == videoSizeTemp) )
	{
		return rate <<3;
	}
	else if( DVRVIDEO_SIZE_3M == videoSizeTemp || DVRVIDEO_SIZE_SXGA == videoSizeTemp || DVRVIDEO_SIZE_16_9_3M == videoSizeTemp)
	{
		return rate*32;
	}
	else if( DVRVIDEO_SIZE_4M == videoSizeTemp )
	{
		return rate*36;
	}
	else if( DVRVIDEO_SIZE_5M == videoSizeTemp )
	{
		return rate*40;
	}
	else if ((DVRVIDEO_SIZE_CIF == videoSizeTemp) || (DVRVIDEO_SIZE_QCIF == videoSizeTemp) )
	{
		return rate;
	}
	else
	{
		//assert (false);
		return 0;
	}

	return rate;
}


//把一个以CIF的帧率为标准资源数目转换为要求的图像尺寸的帧率。
unsigned long RealRate(unsigned long rate, VIDEO_SIZE videoSize)
{
#if defined(__DVR_BASIC__)
	VIDEO_SIZE videoSizeTemp = CProduct::Instance()->RealVideoSize(videoSize);
#else
	VIDEO_SIZE videoSizeTemp = videoSize; 
#endif
	if (DVRVIDEO_SIZE_D1 == videoSizeTemp)
	{
		return (rate >> 2);
	}
	else if (DVRVIDEO_SIZE_HD1 == videoSizeTemp)
	{
		return (rate >> 1);
	}
	else if ((DVRVIDEO_SIZE_HD720 == videoSizeTemp)/* || (DVRVIDEO_SIZE_960H == videoSizeTemp)*/)
	{
		return (rate >> 1);
	}
	else if ((DVRVIDEO_SIZE_HD1080 == videoSizeTemp) || (DVRVIDEO_SIZE_960H == videoSizeTemp))
	{
		return (rate >> 3);
	}
	else if ((DVRVIDEO_SIZE_CIF == videoSizeTemp) || (DVRVIDEO_SIZE_QCIF == videoSizeTemp))
	{
		return (rate);
	}
	else
	{
		assert (false);
		return 0;
	}
}

unsigned long UsedRateEx(unsigned long rate, VIDEO_SIZE videoSize)
{
#if defined(__DVR_BASIC__)
	VIDEO_SIZE videoSizeTemp = CProduct::Instance()->RealVideoSize(videoSize);
#else
	VIDEO_SIZE videoSizeTemp = videoSize;
#endif

	if (DVRVIDEO_SIZE_D1 == videoSizeTemp)
	{
		return rate * 4;
	}
	else if (DVRVIDEO_SIZE_HD1 == videoSizeTemp)
	{
		return rate * 2;
	}
	else if (DVRVIDEO_SIZE_HD720 == videoSizeTemp)
	{
		return rate * 10;
	}
	else if (DVRVIDEO_SIZE_HD1080 == videoSizeTemp)
	{
		return rate * 20;
	}
	else if (DVRVIDEO_SIZE_960H == videoSizeTemp)
	{
		return rate  * 6;
	}
	else if ((DVRVIDEO_SIZE_CIF == videoSizeTemp) || (DVRVIDEO_SIZE_QCIF == videoSizeTemp) )
	{
		return rate;
	}
	else
	{
		assert (false);
		return 0;
	}

	return rate;
}

unsigned long RealRateEx(unsigned long rate, VIDEO_SIZE videoSize)
{
	if (DVRVIDEO_SIZE_D1 == videoSize)
	{
		return rate / 4;
	}
	else if (DVRVIDEO_SIZE_HD1 == videoSize)
	{
		return rate / 2;
	}
	else if (DVRVIDEO_SIZE_HD720 == videoSize)
	{
		return rate / 10;
	}
	else if (DVRVIDEO_SIZE_HD1080 == videoSize)
	{
		return rate / 20;
	}
	else if (DVRVIDEO_SIZE_960H == videoSize)
	{
		return rate / 6;
	}
	else if ((DVRVIDEO_SIZE_CIF == videoSize) || (DVRVIDEO_SIZE_QCIF == videoSize))
	{
		return (rate);
	}
	else
	{
		assert (false);
		return 0;
	}
}


bool SetUpdateStatus(bool bSucc)
{
	char fileName [] = "/mnt/mtd/Update.dat";
	FILE *pFile = fopen(fileName, "wb");

	if (NULL == pFile)
	{
		return false;	
	}

	if (bSucc)
	{
		if (1 != fwrite("1", 1, 1, pFile))
		{
			fclose(pFile);
			return false;
		}
	}
	else
	{
		if (1 != fwrite("0", 1, 1, pFile))
		{
			fclose(pFile);
			return false;
		}
	}

	fclose(pFile);
	return true;
}


bool CheckVersion(char *pCheck)
{
	assert(NULL != pCheck);
	char *p = NULL;

	FILE *pFile = fopen(pCheck, "rb");
	if (NULL == pFile)
	{
		return false;
	}

	//从升级包中校验文件check.dat读取升级包的ID
	char UpDvrPackID[80] = {0};
	if (80 != fread(UpDvrPackID, sizeof(char), 80, pFile))
	{
		fclose(pFile);
		return false;
	}

	//从升级包中校验文件check.dat读取版本号
	char UpDvrSoftVersionM[80] = {0};
	if (80 != fread(UpDvrSoftVersionM, sizeof(char), 80, pFile))
	{
		fclose(pFile);
		return false;
	}

	//从升级包中校验文件check.dat读取:升级包支持的软件级别
	char UpDvrSoftVerLevel[80] = {0};
	if (80 != fread(UpDvrSoftVerLevel, sizeof(char), 80, pFile))
	{
		fclose(pFile);
		return false;
	}


	//从升级包中校验文件check.dat读取:升级包支持的扩展级别个数
	char UpSzLevelNumEx[80] = {0};
	if (80 != fread(UpSzLevelNumEx, sizeof(char), 80, pFile))
	{
		fclose(pFile);
		return false;
	}
	p = NULL;
	p = strstr(UpSzLevelNumEx, "=");
	if (NULL == p)
	{
		fclose(pFile);
		return false;
	}
	const char tempLevelNumEx = *(p+1);
	int UpLevelNumEx = atoi(&tempLevelNumEx);
	if ((UpLevelNumEx < 0) || (UpLevelNumEx >= 10))
	{
		fclose(pFile);
		return false;
	}

	//从升级包中校验文件check.dat读取:升级包中软件的扩展级别
	typedef char LINECHAR[80];
	char DvrSoftVerLevelExTemp[80] = {0};
	if (80 != fread(DvrSoftVerLevelExTemp, sizeof(char), 80, pFile))
	{
		fclose(pFile);
		return false;
	}
	LINECHAR *pUpDvrSoftVerLevelEx = NULL;
	if (UpLevelNumEx > 0)
	{	
		char szLevelExTmp[80] = {0};
		pUpDvrSoftVerLevelEx = new LINECHAR[UpLevelNumEx];

		if (NULL == pUpDvrSoftVerLevelEx)
		{
			fclose(pFile);
			return false;
		}

		p = NULL;
		p = strstr(DvrSoftVerLevelExTemp, "=");
		if (NULL == p)
		{
			fclose(pFile);
			return false;
		}
		*p = '\0';
		strcpy(szLevelExTmp, DvrSoftVerLevelExTemp);
		if (strlen(p+1) <= 0)
		{
			fclose(pFile);
			return false;
		}
		int count = 0;
		char *ps = strtok(p+1, ",");
		while((ps != NULL) && (count < UpLevelNumEx))
		{
			memset(pUpDvrSoftVerLevelEx[count], 0, 80);
			snprintf(pUpDvrSoftVerLevelEx[count], 80, "%s=%s", szLevelExTmp, ps);
			count++;
			ps = strtok(NULL, ",");
		}
		if (count != UpLevelNumEx)
		{
			fclose(pFile);
			return false;
		}		
	}

	//从升级包中校验文件check.dat读取:升级是否要比较版本级别。
	char UpBCheckSoftLevel[80] = {0};
	if (80 != fread(UpBCheckSoftLevel, sizeof(char), 80, pFile))
	{
		fclose(pFile);
		return false;
	}

	//从升级包中校验文件check.dat读取:是否允许有高版本软件升级到本升级包
	char UpBHighVerToThis[80] = {0};
	if (80 != fread(UpBHighVerToThis, sizeof(char), 80, pFile))
	{
		fclose(pFile);
		return false;
	}

	//从升级包中校验文件check.dat读取:是否允许所有低于本升级包的软件升级到本升级包
	char UpBCheckLowVer[80] = {0};
	if (80 != fread(UpBCheckLowVer, sizeof(char), 80, pFile))
	{
		fclose(pFile);
		return false;
	}

	//从升级包中校验文件check.dat读取:低版本号
	char UpStartLowVersion[80] = {0};
	if (80 != fread(UpStartLowVersion, sizeof(char), 80, pFile))
	{
		fclose(pFile);
		return false;
	}

	//比较升级包的ID是否满足要求
	if (0 != strcmp(DVR_PACK_ID, UpDvrPackID))
	{
		fclose(pFile);
		return false;
	}

	//判断是否进行软件级别的比较
	p = NULL;
	p = strstr(UpBCheckSoftLevel, "=");
	if (NULL == p)
	{
		fclose(pFile);
		return false;
	}
	p++;
	if ((*p != '1') && (*p != '0'))
	{
		fclose(pFile);
		return false;
	}
	if ('1' == *p)
	{
		bool bOk = false;
		if (0 != strncmp(DVR_SOFT_LEVEL, UpDvrSoftVerLevel, 17))
		{
			if (UpLevelNumEx > 0)
			{
				for (int i = 0; i < UpLevelNumEx; ++i)
				{
					char *p1 = NULL;
					char *p2 = NULL;
					p1 = (char*)strstr(DVR_SOFT_LEVEL, "=");
					p2 = strstr(pUpDvrSoftVerLevelEx[i], "=");
					if ((p1 != NULL) && (p2 != NULL))
					{
						if (*(p1+1) == *(p2 + 1))
						{
							bOk = true;
							break;
						}
					}
				}
			}
		}
		else
		{
			bOk = true;			
		}

		if (!bOk)
		{
			fclose(pFile);
			return false;
		}
	}

	//判断是否允许高版本升级包升级到此软件
	p = NULL;
	p = strstr(UpBHighVerToThis, "=");
	if (NULL == p)
	{
		fclose(pFile);
		return false;
	}
	p++;
	if ((*p != '1') && (*p != '0'))
	{
		fclose(pFile);
		return false;
	}
	if ('0' == *p)
	{
		if (strcmp(UpDvrSoftVersionM, DVR_SOFT_VERSION_M) > 0)
		{
			fclose(pFile);
			return false;
		}
	}

	//判断是否允许低于本软件的升级包升级到本软件
	p = NULL;
	p = strstr(UpBCheckLowVer, "=");
	if (NULL == p)
	{
		fclose(pFile);
		return false;
	}
	p++;
	if ((*p != '1') && (*p != '0'))
	{
		fclose(pFile);
		return false;
	}
	if ('1' == *p)
	{
		if (strcmp(UpDvrSoftVersionM, UpStartLowVersion) < 0)
		{
			fclose(pFile);
			return false;
		}
	}

	fclose(pFile);
	return true;	
}

int GetThisThreadId()
{
#ifdef __ENVIRONMENT_WIN32__
	return GetCurrentThreadId();
#else
	return syscall(SYS_gettid);
#endif
}

bool BindThreadToCpu(int pid, int whichCpu)
{
#ifdef __ENVIRONMENT_LINUX__
	cpu_set_t mask;
	
	CPU_ZERO(&mask);
	CPU_SET(whichCpu, &mask);
	if (0 != sched_setaffinity((pid_t)pid, sizeof(mask), &mask))
	{
		fprintf(stderr, "%s %s %d\n", strerror(errno), __FILE__, __LINE__);
        	return false;
	}
	else
	{
 	   	return true;
	}
#else 
	return false;
#endif
}

int GetNumberOfCpu()
{
#ifdef __ENVIRONMENT_LINUX__
	int cpuNum;

	if ((cpuNum = sysconf(_SC_NPROCESSORS_ONLN)) == -1)
	{
		return -1;
	}
    	else
    	{
        	return cpuNum;
    	}
#else
	return -1;
#endif
}

bool FindStringInFile(const char *pFilePath,const char *pFindContent)
{
	FILE *fp=fopen(pFilePath,"r");
	if(fp == NULL)
	{
		return false;
	}

	char buff[256]={0};
	while(!feof(fp))
	{
		memset(buff,0,sizeof(buff));
		if(1!=fread(buff,sizeof(buff),1,fp))
		{
			fclose(fp);
			return false;
		}
		else
		{
			//读取正确，查找字符串
			char *pFind=NULL;
			pFind=strstr(buff,pFindContent);
			if(pFind!=NULL)
			{
				fclose(fp);
				return true;
			}
		}
	}
	//始终都没有找到，返回假
	fclose(fp);
	return false;
}


void GetURLAndPort(char *inputUrl,char *outputUrl,char *outputUrlEnd,unsigned short &port)
{
	assert(inputUrl!=NULL);
	char *pfindurlstart=NULL;
	char *pfindUrlEnd=NULL;
	char *pPort=NULL;
	int copylen=0;
	char *tmp=new char[strlen(inputUrl)+1];

	//首先查找://。
	pfindurlstart=strstr(inputUrl,"://");
	if(pfindurlstart!=NULL)//有"://"
	{
		//先打印出来
		//printf("pfindstart:   %s\n",pfindurlstart+3);
		pfindUrlEnd=strstr(pfindurlstart+strlen("://"),"/");
		if(pfindUrlEnd!=NULL)
		{
			//printf("pfindUrlEnd=%s\n",pfindUrlEnd);
			//开始和结束都有，查找冒号的存在
			copylen=pfindUrlEnd-(pfindurlstart+3);
			memset(tmp,0,sizeof(tmp));
			memcpy(tmp, pfindurlstart+3, copylen);
			tmp[copylen]='\0';
			memset(outputUrlEnd,0,sizeof(outputUrlEnd));
			strcpy(outputUrlEnd,pfindUrlEnd);

			//printf("tmp:%s\n",tmp);
		}
		else
		{
			//没有"/",复制所有的到tmp;
			copylen=strlen(inputUrl)-(pfindurlstart+3-inputUrl);
			memset(tmp,0,sizeof(tmp));
			memcpy(tmp, pfindurlstart+3, copylen);
			tmp[copylen]='\0';
			memset(outputUrlEnd,0,sizeof(outputUrlEnd));
		}


		pPort=strstr(tmp,":");
		if(pPort!=NULL)//有冒号
		{
			port=atol(pPort+1);
			copylen=pPort-tmp;
			tmp[copylen]='\0';
			memcpy(outputUrl,tmp, copylen);
			outputUrl[copylen]='\0';
		}
		else
		{
			port=80;
			strcpy(outputUrl,tmp);
		}

	}
	else//没有"://"
	{
		//直接查找"/"
		pfindUrlEnd=strstr(inputUrl,"/");
		if(pfindUrlEnd!=NULL)
		{
			copylen=pfindUrlEnd-inputUrl;
			memset(tmp,0,sizeof(tmp));
			memcpy(tmp, inputUrl, copylen);
			tmp[copylen]='\0';
			strcpy(outputUrlEnd,pfindUrlEnd);
		}
		else
		{
			memset(tmp,0,sizeof(tmp));
			copylen=strlen(inputUrl);
			strcpy(tmp,inputUrl);
			tmp[copylen]='\0';
			memset(outputUrlEnd,0,sizeof(outputUrlEnd));
		}

		pPort=strstr(tmp,":");
		if(pPort!=NULL)//有冒号
		{
			port=atol(pPort+1);
			copylen=pPort-tmp;
			memcpy(outputUrl, tmp, copylen);
			outputUrl[copylen]='\0';
		}
		else
		{
			port=80;
			strcpy(outputUrl,tmp);
		}


	}

	if(tmp!=NULL)
	{
		delete []tmp;
	}


}



#if defined(__VERY_BIG_DISK__)
/*
下面我增加了一些包裹函数

下面两个，可用于套接口和磁盘的读写，
主要目的是防范信号中断导致的读写失败。
zhouhb*/
ssize_t Readn(int fd, char *vptr, size_t n)
{
	size_t nleft;
	ssize_t nread;
	char *ptr;

	ptr = vptr;
	nleft = n;
	while (nleft > 0) 
	{
		if ( (nread = read(fd, ptr, nleft)) < 0 ) 
		{
			if (errno == EINTR)
				nread = 0; /* and call read() again */
			else
				return(-1);
		} 
		else if ( nread == 0 )
		{
			break; /* EOF */
		}
		nleft -= nread;
		ptr += nread;
	}

	return(n - nleft); /* return >= 0 */
}

ssize_t Writen(int fd, const char *vptr, size_t n)
{
	size_t nleft;
	ssize_t nwritten;
	const char *ptr;

	ptr = vptr;
	nleft = n;
	while (nleft > 0) 
	{
		if ( (nwritten = write(fd, ptr, nleft)) <= 0) 
		{
			if (errno == EINTR)
				nwritten = 0; /* and call write() again */
			else
				return(-1); /* error */
		}
		if (errno == 5)
			return(-1);
		nleft -= nwritten;
		ptr += nwritten;
	}
	return(n);
	
}

//从路径字符串，得到相应的文件号和分区号。 zhouhb
void GetIdFromPath(const char * pFilePath, unsigned long *diskID, unsigned long *fileID)
{
	char tmp1[128];
	char tmp2[128];
	char tmpPath[256];
	char *pDisk = (char *)tmp1;
	char *pFile = (char *)tmp2;
	memset(pDisk, 0, 128);
	memset(pFile, 0, 128);
	memset(tmpPath, 0, 256);
	memcpy( tmpPath, pFilePath, strlen(pFilePath) );
	
	printf( "%s:%d:: zhouhb pFilePath==%s; strlen(pFilePath)==%d; \n", __FILE__, __LINE__, pFilePath, strlen(pFilePath) );
	printf("%s:%d:: zhouhb tmpPath==%s \n",__FILE__,__LINE__, tmpPath);

	//	路径格式: /mnt/00/00000010.dat 
	char *pTmp = (char *)tmpPath;
	strsep( &pTmp,"/" );
	strsep( &pTmp,"/" );
	pDisk = strsep(&pTmp,"/");
	*diskID = atoi(pDisk);
	printf( "%s:%d:: zhouhb 11111  pDisk==%s; pTmp==%s; diskID==%d \n", __FILE__, __LINE__, pDisk, pTmp, *diskID );
	
	pFile = strsep(&pTmp,".");
	*fileID = atoi(pFile);
	
	printf( "%s:%d:: zhouhb 222  pFile==%s; fileID==%d \n", __FILE__, __LINE__, pFile, *fileID );

}

//获取CRC校验值
unsigned long GetCRC_Value ( char *block, unsigned int length)
{
	unsigned int  crc_tab[256];
	
	//首先对crc_tab初始化
	{
		unsigned long crc, poly;
		int i, j;
		
		poly = 0xEDB88320L;
		for (i = 0; i < 256; i++)
		{
		   crc = i;
		   for (j = 8; j > 0; j--)
		   {
			  if (crc & 1)
			  {
				 crc = (crc >> 1) ^ poly;
			  }
			  else
			  {
				 crc >>= 1;
			  }
		   }
		   crc_tab[i] = crc;
		}
	}

	//指示crc应该放于寄存器中
   register unsigned long crc;
   unsigned long i;

   crc = 0xFFFFFFFF;
   for (i = 0; i < length; i++)
   {
      crc = ((crc >> 8) & 0x00FFFFFF) ^ crc_tab[(crc ^ *block++) & 0xFF];
   }
   return (crc ^ 0xFFFFFFFF);
}

#endif
bool GetCustomEncrypt(unsigned int productId,string &outStr)
{
     char *pCustomName[] = {"IN","CN","SP","US02","RU04","BR27","TW01","GB14","IN23","US47",\
	 	"IN25","PL14","IL03","LV01","CBC","GB31","ZNV"};

    char *pStr = pCustomName[productId];
    outStr.clear();
    char buf[10]={0};
    int leng = sizeof(pCustomName)/sizeof(char *);
    if(productId > leng-1)
        return false;
    sprintf(buf,"%03d-",productId);
    outStr+=buf;
    for(unsigned int i=0;i<strlen(pStr);i++)
    {
        char buff[10]={0};
        sprintf(buff,"%2x",static_cast<unsigned char >(*(pStr+i)));
        outStr +=buff;
    }
    printf("get the str = %s\n",outStr.c_str());
    return true;
}

//end
