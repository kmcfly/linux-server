// HeapDataManager.h: interface for the CHeapDataManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HEAPDATAMANAGER_H__17159B15_1775_4C3B_8DBE_B1958B781D74__INCLUDED_)
#define AFX_HEAPDATAMANAGER_H__17159B15_1775_4C3B_8DBE_B1958B781D74__INCLUDED_

#include "NatCommon.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CNatHeapDataManager  
{
public:
	CNatHeapDataManager(int iEachMemorySize, int iHeapCount);
	virtual ~CNatHeapDataManager();

	void* GetMemory();
	//ע�⣺�������������������ڴ棬ʹ��������ӿ��ͷţ�
	//���ͷŵ�ʱ��֪���д������������ʱ����ֳ�һ��Ī������Ĵ���
	void ReleaseMemory(void *pData);

    int GetHeapCount();

    int GetEachMemorySize();
private:
    const int m_iEachMemorySize;
	const int	m_iHeapCount;
	char		*m_pData;
	int			m_iUsed;
	char		**m_ppHeadPoints;
	PUB_lock_t	m_lock;
};

class CNatHeapDataPointer
{
public:
    static CNatHeapDataPointer* NewMemory(CNatHeapDataManager *manager);
public:
    void* GetMemory() const;
    //��ȡ���ݳ���
    int GetLength() const;

    void SetLength(int iLen);

    int GetMaxLength() const;

    unsigned long Add();

    unsigned long Dec();
private:
    CNatHeapDataPointer(CNatHeapDataManager* manager, void* memory);
    ~CNatHeapDataPointer();
private:
    CNatHeapDataManager* m_manager;
    void* m_memory;
    CPUB_Lock m_dataLock;
    unsigned long m_dwShared;	//�����˼���
    int m_length;
};

#endif // !defined(AFX_HEAPDATAMANAGER_H__17159B15_1775_4C3B_8DBE_B1958B781D74__INCLUDED_)
