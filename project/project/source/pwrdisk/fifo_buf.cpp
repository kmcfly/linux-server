#include "PUB_common.h"
#include "fifo_buf.h"

CFIFO_buf::CFIFO_buf()
{
	m_buf_size = 0;	//�����С
	m_bOpened = false;			//�Ƿ��Ѿ����򿪹�
	m_pidx = m_cidx = 0;		//���������� index
    m_bOverWrite = false;
	m_buf = NULL;		    //����
}

CFIFO_buf::~CFIFO_buf()
{
	if(m_buf)
	{
		free(m_buf);
	}
}
/******************************************************************************
 * ��������: ����FIFO
 * �������: size: FIFO��С,��λbytes,Ϊ�Ż�����,2�ı���,��С��Χ1K-2G
 * �������: ��
 * ����ֵ  : <0-ʧ��, 0��ʾ�ɹ�
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
	m_cidx = 0;		//���������� index

	if(m_bOpened)
	{
		if(size != m_buf_size)
		{
			free(m_buf);	//���ڲ���ȵ�����£��ͷ��ڴ棬
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
 * ��������: ��FIFO�ж�ȡ����,�ú���һ�ζ�ȡ����size����,��FIFO�����ݲ���ʱ�����ش�����Ϣ
 * �������: data: ������ָ��
 *           size: ��ȡ��С
 *           bPeek: �Ƿ����FIFO���Ѷ�����,0-���,��0-�����
 * �������: ��
 * ����ֵ  : >0-�������ݴ�С,<0-�������
 *****************************************************************************/
int CFIFO_buf::read(unsigned char * buf, uint32_t size, bool bPeek)
{
	if(0 == size || NULL == buf)
	{
		return FIFO_ERR_PARAM;
	}

	if (GetDataSize() < size) // �������ݳ��Ȳ���
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
 ���ѵ�һЩ����
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
	������һЩ����
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
		//�ֳ� ǰ�����Σ� [head data] [free][end data]
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
 * ��������: д�����ݵ�FIFO,�ú���һ��д�볤��size����,��FIFO�л��岻��ʱ�����ش�����Ϣ
 * �������: data: ������ָ��
 *           size: ��д���ݳ���
 * �������: ��
 * ����ֵ  : >0-д�����ݴ�С,<0-�������
 *****************************************************************************/
int CFIFO_buf::write(unsigned char * buf, uint32_t size)
{
	uint32_t len = GetFreeSize();

	if (0 == len || size > len ) // �������ݳ��Ȳ���
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

