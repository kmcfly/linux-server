#include "PUB_common.h"
#include "fifo_buf.h"

CFIFO_buf::CFIFO_buf()
{
	m_buf_size = 0;	//缓冲大小
	m_bOpened = false;			//是否已经被打开过
	m_pidx = m_cidx = 0;		//产生的数据 index
    m_bOverWrite = false;
	m_buf = NULL;		    //缓冲
}

CFIFO_buf::~CFIFO_buf()
{
	if(m_buf)
	{
		free(m_buf);
	}
}
/******************************************************************************
 * 函数功能: 创建FIFO
 * 输入参数: size: FIFO大小,单位bytes,为优化运算,2的倍数,大小范围1K-2G
 * 输出参数: 无
 * 返回值  : <0-失败, 0表示成功
 *****************************************************************************/
int CFIFO_buf::open(uint32_t size)
{
	if((size == 0) || (size % 2) != 0)
	{
		printf("size error.\n");
		return FIFO_ERR_SIZE;
	}

	m_bOverWrite = false;
	m_pidx = 0;
	m_cidx = 0;		//产生的数据 index

	if(m_bOpened)
	{
		if(size != m_buf_size)
		{
			free(m_buf);	//大于不相等的情况下，释放内存，
		}
		else
		{
			return FIFO_ERR_OPENED;
		}
	}

	m_buf = (unsigned char* )malloc(size);
	memset(m_buf, 0, size);

	m_bOpened = true;
	m_buf_size = size;

	return 0;
}

int CFIFO_buf::close()
{
	if(m_buf)
	{
		free(m_buf);
		m_buf = NULL;
	}

	m_bOpened = false;
	m_buf_size = 0;
	m_bOverWrite = false;

	return 0;
}


/******************************************************************************
 * 函数功能: 从FIFO中读取数据,该函数一次读取长度size数据,当FIFO中数据不足时将返回错误信息
 * 输入参数: data: 缓冲区指针
 *           size: 读取大小
 *           bPeek: 是否清除FIFO中已读数据,0-清除,非0-不清除
 * 输出参数: 无
 * 返回值  : >0-读出数据大小,<0-错误代码
 *****************************************************************************/
int CFIFO_buf::read(unsigned char * buf, uint32_t size, bool bPeek)
{
	if(0 == size || NULL == buf)
	{
		return FIFO_ERR_PARAM;
	}

	if (GetDataSize() < size) // 缓存数据长度不足
	{
		return FIFO_ERR_NOTENOUGH;
	}

	m_lock.Lock();
	
	uint32_t len = m_buf_size - m_cidx;
	if(len < size)
	{
		memcpy(buf, m_buf + m_cidx, len);
		memcpy(buf + len, m_buf, size - len);
	}
	else
	{
		memcpy(buf,m_buf + m_cidx,size);
	}

	if(0 == bPeek)
	{
		ConsumeBuf(size);
	}

	m_lock.UnLock();
	
	return size;
}

/*
 消费掉一些数据
*/
int CFIFO_buf::ConsumeBuf(uint32_t size)
{
	m_lock.Lock();

	m_cidx += size;
	if(m_cidx >= m_buf_size )
	{
		m_cidx = m_cidx %m_buf_size;
		m_bOverWrite = false;
	}

	m_lock.UnLock();

	return 0;
}

/*
	生产了一些数据
*/
int CFIFO_buf::ProductBuf(uint32_t size)
{
	m_lock.Lock();

	m_pidx += size;
	if(m_pidx >= m_buf_size)
	{
		m_pidx = m_pidx % m_buf_size;
		m_bOverWrite = true;
	}

	m_lock.UnLock();

	return 0;
}

unsigned char* CFIFO_buf::GetBufOffset(uint32_t size, uint32_t * leaf_size,unsigned char** tail_buf)
{
	if (0 == size)
	{
		return NULL;
	}

	m_lock.Lock();
	uint32_t len = m_buf_size - m_cidx;
	if(len < size)
	{
		*leaf_size = len;		//	memcpy(buf + len, m_buf, size - len);
		*tail_buf = m_buf + m_cidx;

		m_lock.UnLock();

		return m_buf;
	}
	else
	{
		*leaf_size = 0;
		tail_buf = NULL;
		m_lock.UnLock();

		return m_buf + m_cidx;
	}
}

uint32_t CFIFO_buf::GetDataSize()
{
	uint32_t data_size = 0;

	m_lock.Lock();

	if(m_bOverWrite)
	{
		//分成 前后两段， [head data] [free][end data]
		data_size = m_pidx + (m_buf_size - m_cidx);
	}
	else
	{
		data_size = m_pidx - m_cidx;
	}

	m_lock.UnLock();

	return data_size;
}

uint32_t CFIFO_buf::GetFreeSize()
{
	return  m_buf_size - GetDataSize()-1;	
}

/******************************************************************************
 * 函数功能: 写入数据到FIFO,该函数一次写入长度size数据,当FIFO中缓冲不足时将返回错误信息
 * 输入参数: data: 缓冲区指针
 *           size: 待写数据长度
 * 输出参数: 无
 * 返回值  : >0-写入数据大小,<0-错误代码
 *****************************************************************************/
int CFIFO_buf::write(unsigned char * buf, uint32_t size)
{
	uint32_t len = GetFreeSize();

	if (0 == len || size > len ) // 缓存数据长度不足
	{
		return FIFO_ERR_NOTENOUGH;
	}

	m_lock.Lock();

	if((len = m_buf_size - m_pidx) < size)
	{
		memcpy(m_buf + m_pidx, buf, len);
		memcpy(m_buf, buf + len, size - len);
	}
	else
	{
		memcpy(m_buf + m_pidx, buf, size);
	}

	ProductBuf(size);

	m_lock.UnLock();

	return size;
}

