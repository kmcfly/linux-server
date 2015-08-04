/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : L
** Date         : 2011-05-05
** Name         : DynamicLinkLib.h
** Version      : 1.0
** Description  :
** Modify Record:
1:
***********************************************************************/

#ifndef _DYNAMIC_LINK_LIB_H_
#define _DYNAMIC_LINK_LIB_H_

#include "PUB_common.h"

#if defined(__ENVIRONMENT_WIN32__) || defined(WIN32)
#else
#include <dlfcn.h>
#endif

class CDynamicLinkLib
{
public:
	CDynamicLinkLib();
	virtual ~CDynamicLinkLib();

	bool Open(const char* filename);
	bool IsOpen();
	void Close();

	void* GetProcAddress(const char* procname);

protected:
#if defined(__ENVIRONMENT_WIN32__) || defined(WIN32)
	typedef HMODULE	DLL_HANDLE;
#else
	typedef void*	DLL_HANDLE;
#endif

	DLL_HANDLE		m_handle;
};

#endif

//end
