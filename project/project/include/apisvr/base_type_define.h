
#ifndef __BASE_TYPE_DEFINE_
#define __BASE_TYPE_DEFINE_

#include <stdio.h>
#include <stdlib.h>

#ifdef WIN32
#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <gdiplus.h>
#include <MMSystem.h>
#include <winsock2.h>
#include <mmreg.h>
#include <io.h>
#include "Ws2tcpip.h"

#else
#include <assert.h>
#include <string.h>
#include <typeinfo>
#include <netinet/in.h>  
#include <sys/socket.h> 
#include <algorithm>
#include <string>
// inet_addr
#include <arpa/inet.h>
#endif

#if defined(min)
#undef min 
#endif

#if defined(max)
#undef max 
#endif

#ifndef _DEBUG
#define DEBUG_NEW  new
#endif

#ifdef WIN32
#define  snprintf _snprintf 
#endif

typedef char			tchar, tint8;/////8bit signed
typedef unsigned char	tbyte, tuchar, tuint8; ///////8bit unsigned
typedef short			tint16; ////16bit  signed
typedef unsigned short  tuint16; ////16bit  unsigned
typedef int				tint32; /////32 bit  signed
typedef unsigned int	tuint32; /////32 bit unsigned
#ifdef WIN32
typedef __int64			tint64; //////64bit  signed
typedef unsigned __int64   tuint64; ////64bit  unsigned
#else
typedef long long    tint64; //////64bit  signed
typedef unsigned long long   tuint64; ////64bit  unsigned
#endif
/************************************************************************************
 *基本类型的扩展类型
************************************************************************************/
#if defined(OS_IS_64BIT)
    typedef tuint64     tuint_ptr;
    typedef tint64      tint_ptr;
#else
    typedef tuint32     tuint_ptr;
    typedef tint32      tint_ptr;
#endif /////

#ifndef WIN32
/************************************************************************************
 *扩展类型,  不能随便添加
************************************************************************************/
//typedef tuint_ptr            HANDLE;
typedef void *               HWND;
typedef void               * LPVOID, *HDC, VOID;
typedef tuint64              ULONGLONG;
typedef tint64               LONGLONG;
typedef tuint32              COLORREF;
typedef tuint8               BYTE;

#define DEBUG_NEW  new
#ifndef ERROR_SUCCESS
#define ERROR_SUCCESS 0L
#endif

#define         FALSE          0
#define         TRUE           1

#define         IN
#define         OUT


#define        MAXWORD      0xffff
#define        WM_USER      0x2000

#define        INVALID_HANDLE_VALUE   -1
#define        INVALID_SOCKET   (tint32)(~0)

#define   stricmp   strcasecmp
#define		_atoi64		atoll
#define _strtoui64 strtoull

/* _countof helper */
#if !defined(_countof)
#if !defined(__cplusplus)
#define _countof(_Array) (sizeof(_Array) / sizeof(_Array[0]))
#else
extern "C++"
{
	template <typename _CountofType, size_t _SizeOfArray>
	char (*__countof_helper(_CountofType (&_Array)[_SizeOfArray]))[_SizeOfArray];
#define _countof(_Array) sizeof(*__countof_helper(_Array))
}
#endif
#endif


#define INFINITE 0xFFFFFFFF
//Signaled
#define WAIT_OBJECT_0	0x00000000L
#define WAIT_ABANDONED	0x00000080L
#define WAIT_TIMEOUT	0x00000102L
#define WAIT_FAILED ((tuint32)0xFFFFFFFF)

#define RGB(r,g,b)          ((COLORREF)(((BYTE)(r)|((tuint16)((BYTE)(g))<<8))|(((tuint32)(BYTE)(b))<<16)))
#define MAKELONG(a, b)      ((tint32)(((tuint16)(((tuint32)(a)) & 0xffff)) | ((tuint32)((tuint16)(((tuint32)(b)) & 0xffff))) << 16))
#define MAKEFOURCC(ch0, ch1, ch2, ch3)                              \
	((tuint32)(tuint8)(ch0) | ((tuint32)(tuint8)(ch1) << 8) |   \
	((tuint32)(tuint8)(ch2) << 16) | ((tuint32)(tuint8)(ch3) << 24 ))


#define        UNREFERENCED_PARAMETER(P)     \
{ \
	(P) = (P);\
}\

#include <assert.h>
#define ASSERT(X)      assert(X)


typedef struct tagRECT
{
	tint32    left;
	tint32    top;
	tint32    right;
	tint32    bottom;
} RECT, *PRECT,  *LPRECT;
#endif

#ifndef WIN32
typedef struct _GUID {
	unsigned int  Data1;
	unsigned short Data2;
	unsigned short Data3;
	unsigned char  Data4[ 8 ];
} GUID;
const GUID GUID_NULL={0,0,0,{0,0,0,0,0,0,0,0}};
typedef const GUID &     REFGUID;
#endif

__inline bool operator < (const GUID& guidOne, const GUID& guidOther)
{
	return (memcmp(&guidOne, &guidOther, sizeof(GUID)) < 0);
}

__inline bool operator > (const GUID& guidOne, const GUID& guidOther)
{
	return (memcmp(&guidOne, &guidOther, sizeof(GUID)) > 0);
}

#ifndef WIN32
__inline bool operator == (const GUID& guidOne, const GUID& guidOther)
{
	return (memcmp(&guidOne, &guidOther, sizeof(GUID)) == 0);
}

__inline bool operator != (const GUID& guidOne, const GUID& guidOther)
{
	return (memcmp(&guidOne, &guidOther, sizeof(GUID)) != 0);
}
#endif

#ifdef WIN32
typedef DWORD TRHEAD_RETVALUE;
#else
typedef void* TRHEAD_RETVALUE;

typedef TRHEAD_RETVALUE (*PTHREAD_START_ROUTINE)(
	LPVOID lpThreadParameter
	);
typedef PTHREAD_START_ROUTINE LPTHREAD_START_ROUTINE;
#endif


#ifndef WIN32
typedef struct _FILETIME {
	tuint32 dwLowDateTime;
	tuint32 dwHighDateTime;
} FILETIME, *PFILETIME, *LPFILETIME;
#endif

const FILETIME ZERO_FILE_TIME={0,0};
__inline bool operator < (const FILETIME &leftTime, const FILETIME &rightTime)
{
	const ULONGLONG *pLeftTime = (const ULONGLONG*)&leftTime;
	const ULONGLONG *pRightTime = (const ULONGLONG*)&rightTime;
	return ((*pLeftTime) < (*pRightTime));
}

__inline bool operator > (const FILETIME &leftTime, const FILETIME &rightTime)
{
	const ULONGLONG *pLeftTime = (const ULONGLONG*)&leftTime;
	const ULONGLONG *pRightTime = (const ULONGLONG*)&rightTime;
	return ((*pLeftTime) > (*pRightTime));
}

__inline bool operator == (const FILETIME &leftTime, const FILETIME &rightTime)
{
	return ((leftTime.dwHighDateTime== rightTime.dwHighDateTime) && 
		(leftTime.dwLowDateTime==rightTime.dwLowDateTime));
}

__inline bool operator != (const FILETIME &leftTime, const FILETIME &rightTime)
{
	return !(leftTime == rightTime);
}

__inline FILETIME & operator += (FILETIME &ftTime, const ULONGLONG &ullTime)
{
	ULONGLONG *pftTime = (ULONGLONG *)(&ftTime);
	*pftTime += ullTime;
	return ftTime;
}

__inline FILETIME & operator -= (FILETIME &ftTime, const ULONGLONG &ullTime)
{
	ULONGLONG *pftTime = (ULONGLONG *)(&ftTime);
	*pftTime -= ullTime;
	return ftTime;
}

__inline LONGLONG operator - (const FILETIME &ftTime1, const FILETIME &ftTime2)
{
	const LONGLONG *pllTime1 = (const LONGLONG *)(&ftTime1);
	const LONGLONG *pllTime2 = (const LONGLONG *)(&ftTime2);

	return *pllTime1 - *pllTime2;
}

#ifndef WIN32
#define BI_RGB        0L
#define BI_RLE8       1L
#define BI_RLE4       2L
#define BI_BITFIELDS  3L
#define BI_JPEG       4L
#define BI_PNG        5L
#endif

#endif ///////#ifndef __BASE_TYPE_DEFINE_


