#ifndef __HX_CYCLE_BUFFER_H__
#define __HX_CYCLE_BUFFER_H__

class CHxCycleBuffer
{
public:
	CHxCycleBuffer(void);
	~CHxCycleBuffer(void);

	bool Initial(long bufferSize);
	bool Quit();

	unsigned char *GetBuffer(long bufferLen);
	void ReleaseBuffer(unsigned char *pBuffer, long bufferLen);

	bool WaitForQuit();

private:
	unsigned char				*m_pBuffer;
	long						m_bufferSize;

	unsigned char				*m_pReadCursor;		//���ݶ�ȡ����λ��(����ReleaseBuffer��ı�˱���)
	unsigned char				*m_pWriteCursor;	//������д����λ��(����GetBuffer��ı�˱���)
	unsigned char				*m_pUnused;			//�ڻ�������ĩβ������Ŀռ��Ѳ�������Ҫ����Ĵ�С

	bool						m_bWaitForQuit;
};

#endif
