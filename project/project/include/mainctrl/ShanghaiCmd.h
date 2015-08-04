/***********************************************************************
** Copyright   Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : zxx
** Date         : 2012-11-28
** Name         : ShanghaiCmd.h
** Version      : 1.0
** Description  :
** Modify Record:
1:
***********************************************************************/

#ifndef _SHANGHAI_CMD_H_
#define _SHANGHAI_CMD_H_

#include "uuid.h"

typedef struct _snap_info_ex_
{
	unsigned long bEnable;			//是否有效
	ULONGLONG	  chnnbit;			//按位的通道号
	unsigned long startTime;		//开始时间
}SNAP_INFOEX;

typedef struct _oper_info_
{
	unsigned long		operType;//消息的操作类型
	unsigned long		chnn;	//消息通道
	char			    triggerTime[20];//触发时间
	unsigned char		*pNote;//报警附带的说明文字
	int					textLen;//附带的说明文字
	unsigned char		*pPic;//报警附带的图片信息
	unsigned long		picLen;//图片长度

	uuid_t 				uniqueGUID;//全球唯一GUID;
}OPER_INFO;

//上海平台新加

typedef enum _shanghai_cmd_type_
{
	//图像部分
	ALARMUPLOAD_IMAGE = 1,		//报警联动图像
	KEEPUPLOAD_IMAGE,		//保养上传图像
	DAILYUPLOAD_IMAGE,		//日常上传图像
	TESTUPLOAD_IMAGE,		//测试上传图像
	ONDEMANDUPLOAD_IMAGE,	//按需上传图像
	//故障报修部分
	OTHERSYS_ERR = 11,		//其他系统故障
	AROUNDALARM_ERR,		//周界报警故障
	NETALARM_ERR,			//联网保健故障
	LOCALALARM_ERR,			//本地报警故障
	VIDEOMONITOR_ERR,		//视频监控故障
	BUILDINTERCOM_ERR,		//楼宇对讲故障
	ENTRANCECONTROL_ERR,	//出入口控制故障
	ELECTRIALPATROL_ERR,	//电子巡更故障
	//日志部分
	DVRSYSSTART,			//DVR系统启动
	DVRSYSEXIT,				//DVR系统退出
	DVRSYSERREXIT,			//DVR系统异常退出
	DVRCFGSET,				//DVR参数设置
	DVRCFGSAVE,				//DVR参数保存
	DVRSYSVIDEOLOSS,		//DVR视频丢失
	DVRSYSMOTION,			//DVR移动侦测
	DVROUTSIDEALARM,		//DVR外部触发
	DVRSYSCANCLEALARM,		//DVR系统解除报警
	DVRSYSFAULTEXIT,		//DVR系统非法退出

	DVRREPAIRASSIGN,		//维修签到
	DVRKEEPASSIGN,			//保养签到
	DVRLOCALREPLAY,			//本地回放
	DVRNETREPLAY,			//远程回放

	CHECKUPLOAD_IMAGE,		//验收上传图像

	DVRDISKERR =41,			//DVR磁盘错误
	DVRSYSHEARTOUT,			//系统心跳超时
	DVRSYSHEARTRECOVER,		//系统心跳恢复
	DVRVIDEOOTHER,			//视频其他事件


}SHANGHAI_CMD_TYPE;
#endif
//end
