/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : Ԭʯά
** Date         : 2009-06-02
** Name         : ConfigBlock.h
** Version      : 1.0
** Description  :
** Modify Record:
1:
***********************************************************************/
#ifndef _CONFIG_BLOCK_V4_H_
#define _CONFIG_BLOCK_V4_H_

#include "dvrdvsconfig.h"
#include <list>
#include <string.h>

class CConfigBlock_v4
{
public:
	CConfigBlock_v4();
	~CConfigBlock_v4();
	
	//////////////////////////////////////////////////////////////////////////
	void CreateBlock(unsigned char *pData, unsigned long dataLen);
	void DestroyBlock();

	inline const unsigned char *Data() const{return m_pData;};
	inline unsigned char *Data(){return m_pData;};
	inline unsigned long	DataLen(){return m_dataLen;};
	//////////////////////////////////////////////////////////////////////////
	bool GetConfig(DD_CONFIG_ITEM_ID itemID, unsigned char **ppData, unsigned long &dataLen) const;
	void SetConfig(DD_CONFIG_ITEM_ID itemID, const unsigned char *pData, unsigned long dataLen);

	/////////////////////////////////////////////////////////////////////////
	bool GetSubItemCfg(DD_CONFIG_ITEM_ID itemID, unsigned char **ppData, unsigned long dataPos, unsigned long &dataLen);
	void SetSubItemCfg(DD_CONFIG_ITEM_ID itemID, const unsigned char *pData, unsigned long dataPos, unsigned long dataLen);

	void SetConfigEx(DD_CONFIG_ITEM_ID itemID, const unsigned char *pData, unsigned long dataLen, unsigned long oldLen);
	std::list<DD_CONFIG_ITEM_HEAD_EX> &GetConfigItemList();
	const std::list<DD_CONFIG_ITEM_HEAD_EX> &GetConfigItemList() const;
	int  GetItemCount() const;

protected:
	
private:
	unsigned char	*m_pData;
	unsigned long	m_dataLen;
	std::list<DD_CONFIG_ITEM_HEAD_EX> m_itemList;
};
#endif //_CONFIG_BLOCK_H_
