/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : L
** Date         : 2011-05-05
** Name         : DynamicLinkLib.cpp
** Version      : 1.0
** Description  :
** Modify Record:
1:
***********************************************************************/

#include "DynamicLinkLib.h"

CDynamicLinkLib::CDynamicLinkLib()
{
	m_handle = NULL;
}

CDynamicLinkLib::~CDynamicLinkLib()
{
	if(IsOpen())
	{
		Close();
	}
}

bool CDynamicLinkLib::Open(const char* filename)
{
	if(IsOpen())
	{
		Close();
	}

#if defined(__ENVIRONMENT_WIN32__) || defined(WIN32)
	m_handle = ::LoadLibrary(filename);
#else
	m_handle = dlopen(filename, RTLD_LAZY);
#endif

	if(m_handle)
	{
		//printf("(%s.%d)Load library ok, file = %s.\n", __FILE__, __LINE__, filename);
		return true;
	}
	else
	{
		//printf("(%s.%d)Load library error, file = %s.\n", __FILE__, __LINE__, filename);
		return false;
	}
}

bool CDynamicLinkLib::IsOpen()
{
	return (NULL != m_handle);
}

void CDynamicLinkLib::Close()
{
	if(m_handle)
	{
#if defined(__ENVIRONMENT_WIN32__) || defined(WIN32)
		::FreeLibrary(m_handle);
#else
		dlclose(m_handle);
#endif
		m_handle = NULL;
	}
}

void* CDynamicLinkLib::GetProcAddress(const char* procname)
{
#if defined(__ENVIRONMENT_WIN32__) || defined(WIN32)
	return (void*)::GetProcAddress(m_handle, procname);
#else
	return (void*)dlsym(m_handle, procname);
#endif
}

//end
