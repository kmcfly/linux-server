#ifndef  _PS_DEFINE_H_
#define  _PS_DEFINE_H_

#ifdef WIN32
#include <windows.h>
#endif

#include <time.h>

//变量
#define  PS_Packet_Start_Code_Len       4       
#define  PS_Header_Len                  14      //不考虑字节对齐PS_Header
#define  PS_Header_Len_aligned          16      //考虑字节对齐下的PS_Header

#define MAX_PSBUFFER_SIZE 614400
//#define MUX_RATE 6150
#define MUX_RATE 5242//2048*1024/8/50


#define MAX_AUDIO_LEN 1024
#define MAX_VIDEO_LEN 1024*1024

#define TRY_STREAM_LEN  64

#define  CHECK_PS_HEADER(X)  if ( X[0] != (char)0x00 || X[1] != (char)0x00 || X[2] != (char)0x01 || X[3] != (char)0xBA ) \
						{\
							return PS_Error_Stream;\
						}\

#define  SAFE_DELETE_ARRAY(X)  if( NULL != X) { delete[] X ; X = NULL;}

//方法
#define OFFSET(x) (90000/x)
#define UNITS_IN_TICKET(x)  (90000/(x*2))

//错误返回
#define  PS_Error_OK              0
#define  PS_Error_Param          -1
#define  PS_Error_Stream         -2
#define  PS_Error_Format         -3
#define  PS_Error_H264           -4
#define  PS_Error_Pes_Len        -5

#endif