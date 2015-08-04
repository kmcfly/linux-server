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
#include "ConfigBlock.h"
#include <assert.h>
/************************************************************************/
/*							public										*/
/************************************************************************/
CConfigBlock::CConfigBlock() : m_pData(NULL), m_dataLen(0)
{
	m_pData = NULL;
	m_dataLen = 0;
}

CConfigBlock::~CConfigBlock()
{

}

void CConfigBlock::CreateBlock(unsigned char *pData, unsigned long dataLen)
{
	assert (NULL != pData);
	assert (dataLen > 0);
	assert (0 == m_itemList.size());
	assert (NULL == m_pData);
	assert (0 == m_dataLen);

	NCFG_BLOCK_HEAD *pBlockHead = reinterpret_cast<NCFG_BLOCK_HEAD *>(pData);
	assert (pBlockHead->ItemNum > 0);
	m_dataLen += sizeof(NCFG_BLOCK_HEAD);

	NCFG_ITEM_HEAD *pItemHead = reinterpret_cast<NCFG_ITEM_HEAD *>(pData + sizeof(NCFG_BLOCK_HEAD));
	m_dataLen += (pBlockHead->ItemNum * sizeof(NCFG_ITEM_HEAD));

	assert (m_dataLen <= dataLen);

	unsigned long pos = 0;
	NCFG_ITEM_HEAD_EX itemHeadEx;
	for (unsigned short i=0; i<pBlockHead->ItemNum; ++i)
	{
		itemHeadEx.item_head = pItemHead[i];
		itemHeadEx.start_pos = pos;
		pos += pItemHead[i].len;
		m_dataLen += pItemHead[i].len;

		m_itemList.push_back(itemHeadEx);
	}

	assert (m_dataLen <= dataLen);
	assert (pos == (m_dataLen - sizeof(NCFG_BLOCK_HEAD) - (pBlockHead->ItemNum * sizeof(NCFG_ITEM_HEAD))));

	if (pos > 0)
	{
		m_pData = new unsigned char [pos];
		assert (NULL != m_pData);

		memcpy(m_pData, pData+sizeof(NCFG_BLOCK_HEAD)+(pBlockHead->ItemNum * sizeof(NCFG_ITEM_HEAD)), pos);
	} 
	else
	{
		m_pData = NULL;
	}
}

void CConfigBlock::DestroyBlock()
{
	if (NULL != m_pData)
	{
		delete [] m_pData;
		m_pData = NULL;
	}
	
	m_dataLen = 0;
	m_itemList.clear();
}

bool CConfigBlock::GetConfig(NCFG_ITEM_ID itemID, unsigned char **ppData, unsigned long &dataLen) const
{
	std::list<NCFG_ITEM_HEAD_EX>::const_iterator it;
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

void CConfigBlock::SetConfigEx(NCFG_ITEM_ID itemID, const unsigned char *pData, unsigned long dataLen, unsigned long oldLen)
{
	//2010-03-03 09:30:00 YSW
	//巡航线可能为空
	if (!((NCFG_ITEM_PTZ_INFO_CRUISE_INFO_CHBASE <= itemID) && (itemID < NCFG_ITEM_PTZ_INFO_CRUISE_INFO_CHEND)))
	{
		assert (NULL != pData);
		assert (dataLen > 0);
	}

	//重构数据
	int new_len = m_dataLen - sizeof(NCFG_BLOCK_HEAD) - sizeof(NCFG_ITEM_HEAD) * m_itemList.size() - oldLen + dataLen;
	assert(new_len > 0);	
	BYTE* pNewData = new BYTE[new_len];

	int pos_offset = 0;
	std::list<NCFG_ITEM_HEAD_EX>::iterator it;
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

void CConfigBlock::SetConfig(NCFG_ITEM_ID itemID, const unsigned char *pData, unsigned long dataLen)
{
	assert (NULL != pData);
	assert (dataLen > 0);

	std::list<NCFG_ITEM_HEAD_EX>::iterator it;
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


bool CConfigBlock::GetSubItemCfg(NCFG_ITEM_ID itemID, unsigned char **ppData, unsigned long dataPos, unsigned long &dataLen)
{
	std::list<NCFG_ITEM_HEAD_EX>::const_iterator it;
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


void CConfigBlock::SetSubItemCfg(NCFG_ITEM_ID itemID, const unsigned char *pData, unsigned long dataPos, unsigned long dataLen)
{
	assert (NULL != pData);
	assert (dataLen > 0);

	std::list<NCFG_ITEM_HEAD_EX>::iterator it;
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


std::list<NCFG_ITEM_HEAD_EX> & CConfigBlock::GetConfigItemList()
{
	return m_itemList;
}

const std::list<NCFG_ITEM_HEAD_EX> & CConfigBlock::GetConfigItemList() const
{
	return m_itemList;
}

int CConfigBlock::GetItemCount() const
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
