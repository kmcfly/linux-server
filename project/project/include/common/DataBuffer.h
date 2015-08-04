/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : YSW
** Date         : 2012-03-14
** Name         : DataBuffer.h
** Version      : 1.0
** Description  :
** Modify Record:
1:
***********************************************************************/
#ifndef __DATA_BUFFER_H__
#define __DATA_BUFFER_H__
//#include "TVT_PubCom.h"
#include "PUB_common.h"

const unsigned int DEFAULT_BUFFER_LENGTH	= 2*1024*1024;

class CDataBuffer
{
public:
	CDataBuffer();
	~CDataBuffer();

	int Initial(unsigned int bufLength = DEFAULT_BUFFER_LENGTH);
	void Quit();

	int GetBuffer(unsigned char **ppBuf, unsigned int & bufLength);
	void UnlockBuffer();
	void UsedBuffer(unsigned char *pBuf, unsigned int bufLength);
	void ReleaseBuf(unsigned char *pBuf, unsigned int bufLength);

protected:
	
private:
	typedef struct  
	{
		unsigned int	disable;
		unsigned int	start;
		unsigned int	end;
	}BUFFER_BLOCK;
	
	///
	unsigned char	*m_pBuffer;
	unsigned int	m_bufLength;
	unsigned int	m_bLocked;

	std::list<BUFFER_BLOCK>		m_freeList;
	std::list<BUFFER_BLOCK>		m_usedList;
};
#endif //__DATA_BUFFER_H__
