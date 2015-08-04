////////////////////////////////////////////////////////////////////////
#ifndef __MEM_POOL_H__
#define __MEM_POOL_H__
#include <stdio.h>
#include <string.h>
typedef struct _mem_use_
{
	char * pMem;
	bool bUsed;
}MEM_USE;



typedef enum _mem_type_
{
	MEM_REC_READ = 0,
	MEM_NET_PB	 = 1,
	MEM_TYPE_END = 2,
}MEM_TYPE;

typedef struct _mem_pool_
{
	MEM_USE * pMemUse;
	unsigned long count;
	unsigned long memLen;
	MEM_TYPE memType;
}MEM_POOL;

class CMemPool
{
public:
	static CMemPool * Instance();
	~CMemPool();
	bool Initial(unsigned long totalMemLen);
	bool AddPool(MEM_TYPE memType, unsigned long length, unsigned long count);
	void Quit();
	void * GetMem(MEM_TYPE memType, unsigned long length);
	void ReleaseMem(void * pMem);
protected:
private:
	CMemPool();
private:
	static CMemPool * m_pMemPool;
	unsigned long m_totalMemLen;
	unsigned long m_usedMemLen;
	MEM_POOL		m_memPool[10];
};
#endif  /*__MEM_POOL_H__*/

