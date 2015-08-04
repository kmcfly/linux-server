/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : 袁石维
** Date         : 2009-06-02
** Name         : ConfigBlock.cpp
** Version      : 1.0
** Description  :
** Modify Record:
1:
***********************************************************************/
#include "ConfigBlock_V4.h"
#include <assert.h>

#ifdef __ENVIRONMENT_WIN32__
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#endif
/************************************************************************/
/*							public										*/
/************************************************************************/
CConfigBlock_v4::CConfigBlock_v4() : m_pData(NULL), m_dataLen(0)
{
	m_pData = NULL;
	m_dataLen = 0;
}

CConfigBlock_v4::~CConfigBlock_v4()
{

}

void CConfigBlock_v4::CreateBlock(unsigned char *pData, unsigned long dataLen)
{
	assert (NULL != pData);
	assert (dataLen > 0);
	assert (0 == m_itemList.size());
	assert (NULL == m_pData);
	assert (0 == m_dataLen);

	DD_CONFIG_BLOCK_HEAD *pBlockHead = reinterpret_cast<DD_CONFIG_BLOCK_HEAD *>(pData);
	assert (pBlockHead->ItemNum > 0);
	m_dataLen += sizeof(DD_CONFIG_BLOCK_HEAD);

	DD_CONFIG_ITEM_HEAD *pItemHead = reinterpret_cast<DD_CONFIG_ITEM_HEAD *>(pData + sizeof(DD_CONFIG_BLOCK_HEAD));
	m_dataLen += (pBlockHead->ItemNum * sizeof(DD_CONFIG_ITEM_HEAD));

	assert (m_dataLen <= dataLen);

	unsigned long pos = 0;
	DD_CONFIG_ITEM_HEAD_EX itemHeadEx;
	for (unsigned short i=0; i<pBlockHead->ItemNum; ++i)
	{
		itemHeadEx.item_head = pItemHead[i];
		itemHeadEx.start_pos = pos;
		pos += pItemHead[i].len;
		m_dataLen += pItemHead[i].len;

		m_itemList.push_back(itemHeadEx);
	}

	assert (m_dataLen <= dataLen);
	assert (pos == (m_dataLen - sizeof(DD_CONFIG_BLOCK_HEAD) - (pBlockHead->ItemNum * sizeof(DD_CONFIG_ITEM_HEAD))));

	if (pos > 0)
	{
		if (m_pData != NULL)
		{
			delete []m_pData;
			m_pData = NULL;
		}
		m_pData = new unsigned char [pos];
		assert (NULL != m_pData);

		memcpy(m_pData, pData+sizeof(DD_CONFIG_BLOCK_HEAD)+(pBlockHead->ItemNum * sizeof(DD_CONFIG_ITEM_HEAD)), pos);
	} 
	else
	{
		if (m_pData != NULL)
		{
			delete []m_pData;
		}
		m_pData = NULL;
	}
}

void CConfigBlock_v4::DestroyBlock()
{
	if (NULL != m_pData)
	{
		delete [] m_pData;
		m_pData = NULL;
	}
	
	m_dataLen = 0;
	m_itemList.clear();
}

bool CConfigBlock_v4::GetConfig(DD_CONFIG_ITEM_ID itemID, unsigned char **ppData, unsigned long &dataLen) const
{
	std::list<DD_CONFIG_ITEM_HEAD_EX>::const_iterator it;
	for (it=m_itemList.begin(); it!=m_itemList.end(); ++it)
	{
		if (itemID == (*it).item_head.itemID)
		{
			*ppData = m_pData + (*it).start_pos;
			dataLen = (*it).item_head.len;

			return true;
		}
	}

	return false;
}

void CConfigBlock_v4::SetConfigEx(DD_CONFIG_ITEM_ID itemID, const unsigned char *pData, unsigned long dataLen, unsigned long oldLen)
{
	//重构数据
	int new_len = m_dataLen - sizeof(DD_CONFIG_BLOCK_HEAD) - sizeof(DD_CONFIG_ITEM_HEAD) * m_itemList.size() - oldLen + dataLen;
	assert(new_len > 0);	
	BYTE* pNewData = new BYTE[new_len];

	int pos_offset = 0;
	std::list<DD_CONFIG_ITEM_HEAD_EX>::iterator it;
	for (it=m_itemList.begin(); it!=m_itemList.end(); ++it)
	{
		if (it->item_head.itemID == itemID)
		{
			it->item_head.len = static_cast<unsigned short>(dataLen);
			pos_offset = dataLen - oldLen;
			memcpy(pNewData + it->start_pos, pData, dataLen);
		} 
		else if (pos_offset != 0)
		{
			memcpy(pNewData + it->start_pos + pos_offset, m_pData + it->start_pos, it->item_head.len);
			it->start_pos += pos_offset;
		}
		else
		{
			memcpy(pNewData + it->start_pos, m_pData + it->start_pos, it->item_head.len);
		}
	}

	delete [] m_pData;
	m_pData = pNewData;
	m_dataLen += (dataLen - oldLen);
}

void CConfigBlock_v4::SetConfig(DD_CONFIG_ITEM_ID itemID, const unsigned char *pData, unsigned long dataLen)
{
	assert (NULL != pData);
	assert (dataLen > 0);

	std::list<DD_CONFIG_ITEM_HEAD_EX>::iterator it;
	for (it=m_itemList.begin(); it!=m_itemList.end(); ++it)
	{
		if (itemID == (*it).item_head.itemID)
		{
			if (dataLen == (*it).item_head.len)
			{
				memcpy(m_pData + (*it).start_pos, pData, dataLen);
			}
			else
			{
				assert(false);
				//SetConfigEx(itemID, pData, dataLen, (*it).item_head.len);
			}

			break;
		}
	}
}


bool CConfigBlock_v4::GetSubItemCfg(DD_CONFIG_ITEM_ID itemID, unsigned char **ppData, unsigned long dataPos, unsigned long &dataLen)
{
	std::list<DD_CONFIG_ITEM_HEAD_EX>::const_iterator it;
	for (it=m_itemList.begin(); it!=m_itemList.end(); ++it)
	{
		if (itemID == (*it).item_head.itemID)
		{
			*ppData = m_pData + it->start_pos + dataPos;
			dataLen = it->item_head.len - dataPos;
			return true;
		}
	}

	return false;
}


void CConfigBlock_v4::SetSubItemCfg(DD_CONFIG_ITEM_ID itemID, const unsigned char *pData, unsigned long dataPos, unsigned long dataLen)
{
	assert (NULL != pData);
	assert (dataLen > 0);

	std::list<DD_CONFIG_ITEM_HEAD_EX>::iterator it;
	for (it=m_itemList.begin(); it!=m_itemList.end(); ++it)
	{
		if (itemID == (*it).item_head.itemID)
		{
			if (dataLen <= (*it).item_head.len)
			{
				memcpy(m_pData + (*it).start_pos + dataPos, pData, dataLen);
			}
			else
			{
				assert(false);
				//SetConfigEx(itemID, pData, dataLen, (*it).item_head.len);
			}

			break;
		}
	}
}


std::list<DD_CONFIG_ITEM_HEAD_EX> & CConfigBlock_v4::GetConfigItemList()
{
	return m_itemList;
}

const std::list<DD_CONFIG_ITEM_HEAD_EX> & CConfigBlock_v4::GetConfigItemList() const
{
	return m_itemList;
}

int CConfigBlock_v4::GetItemCount() const
{
	return static_cast<int>(m_itemList.size());
}
/************************************************************************/
/*							protected									*/
/************************************************************************/


/************************************************************************/
/*							private										*/
/************************************************************************/

//end
