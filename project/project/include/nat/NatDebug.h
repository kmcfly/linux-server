// NatDebug.h: interface for the Debug.
//
//////////////////////////////////////////////////////////////////////

#ifndef _NAT_DEBUG_H_
#define _NAT_DEBUG_H_

/**
 * 每个实现(.cpp)文件都必须引用该NatDebug.h，且是最后一个引用（#include)！
 */


#ifdef _DEBUG

#ifdef __ENVIRONMENT_WIN32__

#ifndef _CRTDBG_MAP_ALLOC
#define _CRTDBG_MAP_ALLOC
#endif

#include <crtdbg.h>

#ifndef DEBUG_NEW
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif//DEBUG_NEW

#define new DEBUG_NEW

#define DEBUG_INIT  _CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF)

#endif//__ENVIRONMENT_WIN32__

#endif //_DEBUG

#endif//_NAT_DEBUG_H_