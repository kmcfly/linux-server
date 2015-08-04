/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : 袁石维
** Date         : 2008-07-30
** Name         : CruiseConfig.h
** Version      : 1.0
** Description  :
** Modify Record:
20081201		调整
***********************************************************************/
#ifndef _CRUISE_CONFIG_H_
#define _CRUISE_CONFIG_H_

#include "PUB_common.h"
#include "NetConfigDefine.h"
#include "PTZ.h"
#include "mylist.cpp"

typedef struct _cruise_cfg_head
{
	unsigned long version;
	unsigned long headLen;
	unsigned long channelNum;
	unsigned long fileLength;
}CRUISE_CFG_HEAD;

typedef struct _cruise_cfg_item_head_ex
{
	NCFG_ITEM_HEAD	item_head;	//这里的ID记录的是通道的值
	unsigned long	start_pos;
}CRUISE_INDEX;	//how many cruises in ch

typedef struct _cruisePoint_index
{
	NCFG_ITEM_HEAD	item_head;//这里的ID记录巡航线在通道中的索引
	char			name[36];	//巡航线的名字
	unsigned long	start_pos;
}CRUISE_POINT_INDEX;//how many preset point in cruise 

const unsigned long CRUISE_FILE_VER = 20080730;
const unsigned long CRUISE_DATA_POS = sizeof(CRUISE_CFG_HEAD) + 32;

typedef struct _cruise
{
	unsigned long	index;	
	char			name[36];	//巡航线的名字
	CMyList <PTZ_CURISE_POINT> *pointList;//此值由CCruiseConfig类来申请和释放，外部可以直接引用。
}CRUISE;

class CCruiseConfig
{
public:
	//interface
	CCruiseConfig ();
	~CCruiseConfig ();

	bool Initial (unsigned char chnnNum, const char *pFilePath = NULL);
	void Quit();

	bool GetCruiseList(CMyList<CRUISE> &curise, unsigned char chnn);
	bool ModifyCruiseList(CMyList<CRUISE> &curise, unsigned char chnn);
	bool LoadDefault(unsigned char chnnNum);
protected:
	//methods

	//variable

private:
	//methods
	void RemoveCruiseList();
	bool DefaultConfig(unsigned char chnnNum);		//product default Argument when first exec.
	bool LoadConfig();		//if the PresetConfig.dat File is exist ,then load it.
	bool UpdateData();		//if list is changed ,rewrite the data into file.
	bool LoadCruiseIndex(const CRUISE_INDEX & cruiseIndex);	//load cruise index ;
	bool LoadCruisePoint(const CRUISE_POINT_INDEX & pointIndex, unsigned long chnn);	//load cruise point data;
	bool WriteOneCruise(const CRUISE & cruise, unsigned long pos);
	//variable
	FILE *m_pFile;
	char *m_pFileName;
	CRUISE_CFG_HEAD m_cfgHead;
	CMyList <CRUISE> *m_pCruiseList;//[VIDEO_INPUT_NUM];
};

#endif //_CRUISE_CONFIG_H_

