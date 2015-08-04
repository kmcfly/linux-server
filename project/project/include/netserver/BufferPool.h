// BufferPool.h: interface for the CBufferPool class.
//
//////////////////////////////////////////////////////////////////////

//˵���������α��д�α����ʱ��������Ϊ��
//���������Ϊm_nBufferSize-1

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

	//�������Բ���
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
	long		m_nBufferSize;			//�����ռ�ÿռ��С
	char		*m_pBuffer;				//���������
	long		m_iReadCursor;			//����ض��α�
	long		m_iWriteCursor;			//��ǰ�����д�α�
	PUB_lock_t	m_lock;	
};

#endif // !defined(AFX_BUFFERPOOL_H__1A2C96A5_5A15_41DC_AE8B_136AC25AE80C__INCLUDED_)
