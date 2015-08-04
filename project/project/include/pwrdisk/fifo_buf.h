#ifndef _FIFO_H_
#define _FIFO_H_

#include "cross_typedef.h"

class CFIFO_buf
{
public:
	CFIFO_buf();
	~CFIFO_buf();

protected:
	bool m_bOpened;			//是否已经被打开过
	bool m_bOverWrite;
	uint32_t m_pidx;		//产生的数据product index
	uint32_t m_cidx;	    //消耗的数据consume index

	uint32_t m_buf_size;	//缓冲大小
	unsigned char*    m_buf;		    //缓冲
	CPUB_Lock m_lock;
public:

	int open(uint32_t size);
	int read(unsigned char * buf, uint32_t size,  bool bPeek);
	int write(unsigned char * buf, uint32_t data_size);
	int close();

	//增强功能，节省内存空间同时，也提高读写效率
	//获取内存地址，直接将内存地址返回给上层使用，免去对内存拷贝的时间
	unsigned char* GetBufOffset(uint32_t size, uint32_t * leaf_size,unsigned char ** tail_buf);
	//消费一部分数据
	int ConsumeBuf(uint32_t size);
	//生产了一部分数据
	int ProductBuf(uint32_t size);
	uint32_t GetDataSize();
	uint32_t GetFreeSize();
public:
	uint32_t GetFIFOSize()
	{
		return m_buf_size;
	}

	bool IsEmpty()
	{
		return (m_pidx == m_cidx);
	}

	uint32_t GetCurWarter()	//获取当前警戒位
	{
		return  (GetDataSize() * 100) / m_buf_size;
	}

	uint32_t GetCurProduct()
	{
		return m_pidx;
	}

	uint32_t GetCurConsume()
	{
		return m_cidx;
	}
};

 typedef enum
{
	FIFO_ERR_SIZE      = -3005,
	FIFO_ERR_NOTENOUGH = -3004,
	FIFO_ERR_NODATA    = -3002,
	FIFO_ERR_OPENED    = -3001,
	FIFO_ERR_NOOPEN	   = -3000,
	FIFO_ERR_PARAM     = -2009,
	FIFO_ERR_TIMEOUT   = -2008,

	NO_FIFO_EER = 0,
}ERR_FIFO_E;

#endif /* _FIFO_H_ */
