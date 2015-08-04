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

	unsigned char				*m_pReadCursor;		//数据读取到的位置(调用ReleaseBuffer会改变此变量)
	unsigned char				*m_pWriteCursor;	//数据已写到的位置(调用GetBuffer会改变此变量)
	unsigned char				*m_pUnused;			//在缓冲区的末尾，后面的空间已不够所需要请求的大小

	bool						m_bWaitForQuit;
};

#endif
