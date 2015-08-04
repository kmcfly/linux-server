/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : 袁石维
** Date         : 2010-01-19
** Name         : RunLogItemDef.h
** Version      : 1.0
** Description  :
** Modify Record:
1:
***********************************************************************/
#ifndef _RUN_LOG_ITEM_DEF_H_
#define _RUN_LOG_ITEM_DEF_H_

typedef enum _run_log_item_id
{
	//回放类
	RUN_LOG_ITEM_PB_BASE			= 0x00010000,
	RUN_LOG_ITEM_PB_OPEN_FILE_FAIL,					//打开文件失败
	RUN_LOG_ITEM_PB_SEEK_TIME_ERR,					//定位到某个时间点错误
	RUN_LOG_ITEM_PB_GET_FRAME_FAIL,					//获取帧失败
	RUN_LOG_ITEM_PB_EXIT,							//回放退出
	RUN_LOG_ITEM_PB_GET_NEXT_FILE_FAIL,				//获取并打开下一个文件失败
	RUN_LOG_ITEM_PB_GET_PREV_FILE_FAIL,				//获取并打开上一个文件失败
	RUN_LOG_ITEM_PB_GET_FILE_FAIL,					//通过Reclog获取文件数目为零
	RUN_LOG_ITEM_PB_FILE_NULL,						//文件列表为空
	RUN_LOG_ITEM_PB_OPEN_ALL_FILE_FAIL,				//打开所有的文件失败
	RUN_LOG_ITEM_PB_FILE_REWRITE,					//文件被覆盖了，不能打开给文件
	RUN_LOG_ITEM_PB_ADD_TO_READ_LIST,
	RUN_LOG_ITEM_PB_PUT_OUT_READ_LIST,

	//录像类
	RUN_LOG_ITEM_REC_BASE			= 0x00020000,	
	RUN_LOG_ITEM_REC_OPEN_FILE_SUCC,				//打开文件成功
	RUN_LOG_ITEM_REC_OPEN_FILE_FAIL,
	RUN_LOG_ITEM_REC_WRITE_START,
	RUN_LOG_ITEM_REC_WRITE_END,
	RUN_LOG_ITEM_REC_ACTION_TRUE,
	RUN_LOG_ITEM_REC_ACTION_FALSE,
	RUN_LOG_ITEM_REC_NO_FILE,
	RUN_LOG_ITEM_REC_ADD_REC_LIST,
	RUN_LOG_ITEM_REC_PUT_OUT_REC_LIST,

	//备份类
	RUN_LOG_ITEM_BK_BASE			= 0x00040000,
	RUN_LOG_ITEM_BK_NO_DISK,

	//网络类
	RUN_LOG_ITEM_NET_BASE			= 0x00080000,
	RUN_LOG_ITEM_NET_IP_COLLISION
}RUN_LOG_ITEM_ID;
#endif //_RUN_LOG_ITEM_DEF_H_
