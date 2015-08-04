// BufferPool.h: interface for the CBufferPool class.
//
//////////////////////////////////////////////////////////////////////

//说明：当读游标和写游标相等时，缓冲区为空
//缓冲池容量为m_nBufferSize-1

#if !defined(AFX_BUFFERPOOL_H__1A2C96A5_5A15_41DC_AE8B_136AC25AE80C__INCLUDED_)
#define AFX_BUFFERPOOL_H__1A2C96A5_5A15_41DC_AE8B_136AC25AE80C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PUB_common.h"

class CBufferPool  
{
public:
	CBufferPool();
	virtual ~CBufferPool();

	bool Init(long size);
	void Destroy();
	void Empty();

	//基本属性操作
	long GetTotalSize();
	long GetDataSize();
	long GetFreeSize();
	bool IncreaseSize(long size);

	bool PushData(const void *pData, long len);
	long PopData(char *pData, long len);	
	char *GetFreeBuffer(long len);
	char *GetDataBuffer(long len);
	char *GetBufferHead();
	char *GetBufferTail();
	void FreeBuffer(long len);
	void FillBuffer(long len);

private:
	long		m_nBufferSize;			//缓冲池占用空间大小
	char		*m_pBuffer;				//缓冲池数据
	long		m_iReadCursor;			//缓冲池读游标
	long		m_iWriteCursor;			//当前缓冲池写游标
	PUB_lock_t	m_lock;	
};

#endif // !defined(AFX_BUFFERPOOL_H__1A2C96A5_5A15_41DC_AE8B_136AC25AE80C__INCLUDED_)
