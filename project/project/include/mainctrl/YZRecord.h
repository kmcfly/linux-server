/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : zxx
** Date         : 2012-03-21
** Name         : YZRecord.h
** Version      : 1.0
** Description  :
** Modify Record:
1:
***********************************************************************/
#ifndef _YZ_RECORD_H_
#define _YZ_RECORD_H_

#include "Product.h"
#include "PUB_common.h"
#include "RecordMan.h"
#include "MANUALDVRTimer.h"
#include "LogMan.h"
#include "UserMan.h"
enum YZ_REC_CMD//客户端发来的命令
{
	YZ_REC_STOP, //远征录像停止
	YZ_REC_START,//远征录像开启

};

class CYZRecord
{
public:
	static CYZRecord * Instance();
	~CYZRecord();
	bool Initial (unsigned char videoInputNum,CRecordMan *pRecMan);
	void Quit();
	bool start();//开启线程
	void stop();//停止线程
	//参数：chnn-通道号
	//     recTime-录像延迟时间
	//	   recStatus-录像状态
	//	   clientID-消息的客户端id号
	void StartRec(unsigned char chnn,unsigned short recTime,unsigned char recStatus,unsigned long clientID);//开启录像
	void StopRec(unsigned char chnn,unsigned char recStatus);//停止录像
private:
	CYZRecord ();
	

	bool CreateYZCheckTimerThd();//建立一个线程检查多通道录像是否到期
	void CloseYZTimerThd();//退出线程
	static	RESULT WINAPI YZTimerThread(LPVOID lpParameter);
	void YZTimer();
private:
	enum YZ_REC_STATUS
	{
		YZ_REC_ING,    //远征录像中
		YZ_REC_END,  //远征录像结束
	};
	
	int	*m_pYzRecStatus; //记录远征录像的当前状态， （多通道）
	PUB_thread_t	m_checkYzTimer;//检查定时器是否到时线程
	bool			m_bYzTimer;	
	CRecordMan		*m_pRecMan;
	CMANUALDVRTimer *m_pmaunalDVRTimer;//远征录像定时器
	CPUB_Lock m_lock;
	unsigned char m_videoInputNum;
	unsigned long m_clientID;
};

#endif
