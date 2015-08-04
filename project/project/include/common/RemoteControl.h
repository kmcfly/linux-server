/*****************************************************************
Copyright (C),2009-2012, tongwei video Tech. Co., Ltd.

File name: RemoteControl.h

Author: TVT-ZHL-146  Version:1.0	Date:20091124

Description:
			1:该类用来控制遥控器是否可以控制指定DVR.
			2:遥控器输入2个8+DVR id +确定便可控制该DVR。
			3:当不用遥控器操作设备的时间超过3分钟就自动关闭遥控器功能
			4:在开启遥控器阶段,任何一次输入(从输入第一个8到输入确定的任何一次输入)后,如果超过
			  5s没有接着输入,之前的输入数据就被清空,就需要从新开启遥控器功能
			5:

Others:

Function List:

History:

1.Date:

Author:

Modification:	
2.在可以用遥控器控制DVR的时候在DVR上修改了DVR ID，此时应当不能控制该DVR 11.19
******************************************************************/

#ifndef REMOTE_CONTROL_H_
#define REMOTE_CONTROL_H_

#include "RemoteKeyValue.h"
#include "dvrdvsdef.h"
#include "PUB_common.h"

#ifdef __REMOTE_TR12__
const unsigned long	remoteCtrlCheckValue	= KEY_REMOTE_TR12_8;				//使能和取消遥控器功能的时候使用的关键键值
const unsigned long	remoteCtrlEnterValue	= KEY_REMOTE_TR12_ENTER;			//使能和取消遥控器功能的时候使用的关键键值
#else
const unsigned long	remoteCtrlCheckValue	= KEY_REMOTE_8;						//使能和取消遥控器功能的时候使用的关键键值
const unsigned long	remoteCtrlEnterValue	= KEY_REMOTE_ENTER;					//使能和取消遥控器功能的时候使用的关键键值
#endif

const unsigned long	remoteCtrlCheckCount	= 2;								//使能和取消遥控器时要输入的关键键值的次数
const unsigned long	maxRemoteNotinputTime	= 6;								//在开启遥控器功能阶段,等待下次遥控器键值的超时时间, s
const unsigned long	remoteNotusedTime		= 3 * 60; 							//遥控器有效的时候如果长时间没有输入应自动关闭遥控功能

class CRemoteControl
{
public:
	static CRemoteControl *Instance();
	~CRemoteControl();

	void SetDVRID(const unsigned long dvrID);
	bool RemoteControlDVR(KEY_VALUE_INFO &keyValue);

private:
	CRemoteControl();
	void ResetRemote();
	void CheckControlID( KEY_VALUE_INFO &keyValue, bool bOpen=true );
	int  RemoteValueToRealValue( const unsigned long &RemoteValue );	
	

	int m_dvrID;									//存储本机DVR设备号
	int m_remoteInputDVRid;							//遥控器输入的DVR id
	bool m_bRemoteControlling;						//为真表示遥控器正在控制该DVR，为假表示遥控器还没控制该DVR
	unsigned long m_lastInputTime;					//上次遥控输入的时间
	unsigned long m_checkCount;						//记录输入开锁遥控器时输入8的个数
};
#endif


