

#ifndef MP4DEFINE_H
#define MP4DEFINE_H

/////////////////////////////////////////////////////////////

#ifdef WIN32
#include <windows.h>

#ifndef strcasecmp
#define strcasecmp _stricmp
#endif

#ifndef usleep
#define usleep Sleep
#endif

#endif

/////////////////////////////////////////////////////////////


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include "typedef.h"
//#include <objc/objc.h>
//#import <Cocoa/Cocoa.h>
#define max(a,b)    (((a) > (b)) ? (a) : (b))
#define min(a,b)    (((a) < (b)) ? (a) : (b))
typedef unsigned long long ULONGLONG;
typedef unsigned long ULONG;
typedef unsigned int  UINT;
typedef const char *  LPCSTR;
typedef long long	  INT64;
typedef unsigned char BYTE;
#ifndef WIN32
typedef int			  LRESULT;
#endif
typedef unsigned short      WORD;
//typedef int                 bool;
#ifndef MAX_PATH
#  define MAX_PATH			256
#endif
#ifndef FALSE
#  define FALSE               0
#endif

#ifndef  DEFAULT_DATA_CNT
#define  DEFAULT_DATA_CNT  (256 * 1024 * 1024)
#endif

#ifndef TRUE
#  define TRUE                1
#endif

//_____________________________________________________________________________
///////////////////////////////////////////////////////////////////////////////
// SmartPtr class

/*
*/
template <class T> 
class SmartPtr
{
public:
	SmartPtr() : ptr(NULL) {}
	SmartPtr(const SmartPtr<T>& p) : ptr(p.ptr) {if (ptr) ptr->AddRef(); }
	SmartPtr(T* p) : ptr(p) { if (ptr) ptr->AddRef(); }
	virtual ~SmartPtr() { operator =(NULL); }

	// Operations -------------------------------------------------
public:
	T* operator =(const SmartPtr<T>& p) {
		operator = (p.ptr);
		return ptr;
	}

	T* operator =(T* p);

	operator T* () { return ptr; }
	T* get() { return ptr; }

	bool operator == (const SmartPtr<T>& p) const { return ptr == p.ptr; }
	bool operator != (const SmartPtr<T>& p) const { return ptr != p.ptr; }
	bool operator == (const T* t) const { return ptr == t;}
	bool operator != (const T* t) const { return ptr != t;}

	T& operator *()  const { return *ptr; }
	T* operator ->() const { return ptr; }

	void Attach(T* p2) {
		if (ptr) {
			ptr->Release();
		}
		ptr = p2;
	}

	T* Detach() {
		T* pt = ptr;
		ptr = NULL;
		return pt;
	}
	// Data Members -----------------------------------------------
public:
	T* ptr;		///< The original pointer
};

// Implements -------------------------------------------------

template <class T>
T* SmartPtr<T>::operator =(T* p) 
{
	if (ptr == p) { return ptr; }
	if (ptr) { ptr->Release(); }

	ptr = p;
	if (ptr) { ptr->AddRef(); }
	return ptr;
}

class BaseObject
{
public:
	BaseObject() {
		fRefCount = 0;
	}
	virtual ~BaseObject() {
	}

	// Operations -------------------------------------------------
public:
	virtual unsigned long baseAddRef() {
		register unsigned long uRef = ++fRefCount;
		return uRef;
	}

	virtual unsigned long baseRelease() {
		unsigned long uRef = --fRefCount;
		if (uRef == 0) {
			delete this;
		}
		return uRef;
	}

	// Data members -----------------------------------------------
private:
	volatile unsigned long fRefCount;
};

#define DECLARE_BASEREF_METHODS(theClass)							\
	unsigned long AddRef() { return BaseObject::baseAddRef(); }		\
	unsigned long Release() { return BaseObject::baseRelease(); }	

#endif // MP4DEFINE_H

// End of File
