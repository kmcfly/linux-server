// BufferPool.h: interface for the CBufferPool class.
//
//////////////////////////////////////////////////////////////////////

//˵���������α��д�α����ʱ��������Ϊ��
//���������Ϊm_nBufferSize-1

#ifndef _BUFFER_POOL_H
#define _BUFFER_POOL_H

#include "DDPublic.h"
#include <string.h>

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

#endif
