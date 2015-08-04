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
#ifndef _SCHEDULE_CONFIG_CONFIG_H_
#define _SCHEDULE_CONFIG_CONFIG_H_

#include "PUB_common.h"
#include "NetConfigDefine.h"
#include "mylist.cpp"

const long HOLIDAY_SCH_ITEM_NUM = 7;//rec, motion_rec, sensor_rec, motion_alarm, sensor_alarm, net_alarm, alarm_out

typedef struct _shedule_cfg_head
{
	unsigned long version;
	unsigned long headLen;
	unsigned long channelNum;
	unsigned long sensorInNum;
	unsigned long sensorOutNum;
	unsigned long idNum;
	unsigned long fileLength;
}SCHEDULE_CFG_HEAD;

typedef struct _schedule_item_id_head
{
	NCFG_ITEM_HEAD	item_head;	//其中的ID记录item ID。
	unsigned long	start_pos;
}SCHEDULE_ITEM_ID_HEAD;

typedef struct _schedule_chnn_head
{
	NCFG_ITEM_HEAD	item_head;  //其中的ID记录通道号，从0开始。
	unsigned long	start_pos;
}SCHEDULE_CHNN_HEAD;


const unsigned long SCHEDULE_FILE_VER = 20080730;
const unsigned long SCHEDULE_DATA_POS = sizeof(SCHEDULE_CFG_HEAD)/* + 32*/;

typedef struct _schedule_node
{
	unsigned long	chnn;
	unsigned long	itemID;
	CMyList <HOLIDAY_SCHEDULE> *pHolidayList;//此值由CScheduleConfig类来申请和释放，外部可以直接引用。
}SCHEDULE_NODE;


class CScheduleConfig
{
public:
	//interface
	CScheduleConfig ();
	~CScheduleConfig ();

	bool Initial (unsigned char chnnNum, unsigned char sensorInNum, unsigned char sensorOutNum, const char *pFilePath = NULL);
	void Quit();

	bool GetHolidayScheduleList(CMyList<SCHEDULE_NODE> &scheduleList, unsigned long itemID);
	bool ModifyHolidayScheduleList(CMyList<SCHEDULE_NODE> &scheduleList);
	bool LoadDefault(unsigned char chnnNum, unsigned char sensorInNum, unsigned char sensorOutNum);
protected:
	//methods

	//variable

private:
	//methods
	void RemoveScheduleList();
	bool DefaultConfig(unsigned char chnnNum, unsigned char sensorInNum, unsigned char sensorOutNum);
	bool LoadConfig();
	bool LoadInsertToList(SCHEDULE_CHNN_HEAD &ChnnHead, int itemID, int itemIdHeadIndex);
	bool UpdateData();
	//variable
	FILE *m_pFile;
	char *m_pFileName;
	SCHEDULE_CFG_HEAD m_cfgHead;
	CMyList <SCHEDULE_NODE> *m_pScheduleList;
};

#endif //_SCHEDULE_CONFIG_CONFIG_H_

