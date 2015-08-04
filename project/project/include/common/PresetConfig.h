/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : 袁石维
** Date         : 2008-07-29
** Name         : PresetConfig.h
** Version      : 1.0
** Description  :
** Modify Record:
20081201		袁石维
20090504
***********************************************************************/
#ifndef _PRESET_CONFIG_H_
#define _PRESET_CONFIG_H_

/*****************************************************************************
							文件结构

							 文件头信息--	PRESET_CFG_HEAD
										+ 32 bytes
							INDEX	--	PRESET_INDEX [VIDEO_INPUT_NUM]
							预置点	--	PTZ_PRESET
							预置点	--	PTZ_PRESET
										.
										.
										.
							预置点	--	PTZ_PRESET
*****************************************************************************/
#include "PUB_common.h"
#include "NetConfigDefine.h"
#include "PTZ.h"
#include "mylist.cpp"
#include "Product.h"

typedef struct _preset_cfg_head
{
	unsigned long version;
	unsigned long headLen;
	unsigned long fileLength;
	unsigned long chnnNum;
}PRESET_CFG_HEAD;

typedef struct _pre_cfg_item_head_ex
{
	NCFG_ITEM_HEAD	item_head;
	unsigned long	start_pos;
}PRESET_INDEX;

const unsigned long PRESET_FILE_VER = 20080729;
const unsigned long PRESET_DATA_POS = sizeof(PRESET_CFG_HEAD) + 32;
const unsigned long ONE_CHNN_PRESET_LEN = sizeof( PTZ_PRESET ) * MAX_PRESET_NUM;						//一个通道中所有预置点的长度(BYTE)

class CPresetConfig
{
public:
	//interface
	CPresetConfig ();
	~CPresetConfig ();

	bool Initial (unsigned char chnnNum, const char *pFilePath = NULL);
	void Quit();

	bool GetPresetList (CMyList <PTZ_PRESET> &list, unsigned char chnn);
	bool ModifyPreset(CMyList <PTZ_PRESET> &list, unsigned char chnn);
	bool LoadDefault(unsigned char chnnNum);
	
protected:
	//methods

	//variable

private:
	//methods
	bool DefaultConfig(unsigned char chnnNum);	//product default Argument when first exec.
	bool LoadConfig();		//if the PresetConfig.dat File is exist ,then load it.
	bool UpdateData( CMyList <PTZ_PRESET> &list, unsigned char chnn );		//if list is changed ,rewrite the data into file.
	//variable
	FILE *m_pFile;
	char *m_pFileName;
	CMyList <PTZ_PRESET> *m_pPresetList;
	PRESET_CFG_HEAD  m_cfgHead;
};

#endif //_PRESET_CONFIG_H_
