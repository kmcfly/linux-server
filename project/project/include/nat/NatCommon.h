// NatCommon.h: interface for the common of nat.
//
//////////////////////////////////////////////////////////////////////

#ifndef _NAT_COMMON_H_
#define _NAT_COMMON_H_

#include "NatUserDefine.h"
#include "stdio.h"

#ifdef   __ENVIRONMENT_WIN32__

#define __printf printf

#endif

#ifdef NAT_TVT_DVR_4_0
#include "TVT_PubCom.h"
#else
#include "PUB_common.h"
#endif
#include "SWL_Public.h"
#include <stdio.h>
#include <stdlib.h>

#include <assert.h>
#include "NatBaseTypes.h"
#include "NatLogger.h"


#ifdef NAT_TVT_DVR_4_0
#  ifdef __ENVIRONMENT_WIN32__
#    define Nat_GetHostByName gethostbyname
#    define Nat_ReleaseHost(host)
#  else
#    define Nat_GetHostByName GetHostbyname
#    define Nat_ReleaseHost(host) ReleaseHost(host)
#  endif
#else
#  ifdef __ENVIRONMENT_WIN32__
#    define Nat_GetHostByName gethostbyname
#    define Nat_ReleaseHost(host)
#  else
#    include "NetInterface.h"
#    define Nat_GetHostByName CNetInterface::Instance()->GetHostByName
#    define Nat_ReleaseHost(host) CNetInterface::Instance()->ReleaseHost(host)
#  endif
#endif//NAT_TVT_DVR_4_0

bool Nat_ParseIpByName(const char* addr, unsigned long &ip);

unsigned long Nat_GetTickCount();

void Nat_SRand(unsigned long seed);

void Nat_SRand();

int Nat_Rand();

unsigned short PUB_GetRandomPort();

#ifdef __NAT_PUB_IsTimeOutEx__
inline bool PUB_IsTimeOutEx(unsigned long dwStartTime, unsigned long dwDisTime, unsigned long  dwNowTime)
{
	unsigned long dwDeltaTime = 0;
	if(dwNowTime < dwStartTime)
	{
		//越界处理
		dwDeltaTime = (0xFFFFFFFF - dwStartTime) + dwNowTime;
	}
	else
	{
		dwDeltaTime = dwNowTime - dwStartTime;
	}
	return (dwDeltaTime >= dwDisTime);
}
#endif

inline bool PUB_UNSIGNED_LONG_LT(unsigned long a, unsigned long b)
{
	return (a < b);
}

inline bool PUB_UNSIGNED_LONG_LEQ(unsigned long a, unsigned long b)
{
	return (a <= b);
}

inline bool PUB_UNSIGNED_LONG_GT(unsigned long a, unsigned long b)
{
	return (a > b);
}

inline bool PUB_UNSIGNED_LONG_GEQ(unsigned long a, unsigned long b)
{
	return (a >= b);
}

char* Nat_inet_ntoa( unsigned long ip);

unsigned long GetRandomConnectionId();

#ifdef WIN32
#	ifndef strncasecmp
#		define strncasecmp _strnicmp
#	endif//strncasecmp
#	ifndef strcasecmp
#		define strcasecmp _stricmp
#	endif//strcasecmp
#endif//WIN32

#ifdef WIN32
#undef vsnprintf

inline int Nat_vsnprintf(char *dest, size_t size, const char *format, va_list ap) 
{
	if (size == 0)        // not even room for a \0?
		return -1;          // not what C99 says to do, but what windows does
	dest[size-1] = '\0';
	return _vsnprintf(dest, size-1, format, ap);
}
#define vsnprintf Nat_vsnprintf

#undef snprintf
inline int Nat_snprintf(char *dest, size_t size, const char *format, ...) 
{
	va_list __nat_ap;
	va_start(__nat_ap, format);
	const int __nat_ret = _vsnprintf(dest, size - 1, format, __nat_ap);
	va_end(__nat_ap);
	return __nat_ret;
}
#define snprintf Nat_snprintf
#endif//WIN32

inline int Nat_StrCopy(char* dest, int destSize, const char* src)
{
	return snprintf(dest, destSize, "%s", src);
}

typedef CPUB_Lock CNatLock;

class CNatScopeLock
{
public:
    explicit CNatScopeLock(CNatLock *lock);
    virtual ~CNatScopeLock();
private:
    CNatLock* m_lock;
};

#endif