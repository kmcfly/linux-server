/*****************************************************************
Copyright (C),2009-2012, tongwei video Tech. Co., Ltd.

File name: RemoteControl.h

Author: TVT-ZHL-146  Version:1.0	Date:20091124

Description:
			1:������������ң�����Ƿ���Կ���ָ��DVR.
			2:ң��������2��8+DVR id +ȷ����ɿ��Ƹ�DVR��
			3:������ң���������豸��ʱ�䳬��3���Ӿ��Զ��ر�ң��������
			4:�ڿ���ң�����׶�,�κ�һ������(�������һ��8������ȷ�����κ�һ������)��,�������
			  5sû�н�������,֮ǰ���������ݾͱ����,����Ҫ���¿���ң��������
			5:

Others:

Function List:

History:

1.Date:

Author:

Modification:	
2.�ڿ�����ң��������DVR��ʱ����DVR���޸���DVR ID����ʱӦ�����ܿ��Ƹ�DVR 11.19
******************************************************************/

#ifndef REMOTE_CONTROL_H_
#define REMOTE_CONTROL_H_

#include "RemoteKeyValue.h"
#include "dvrdvsdef.h"
#include "PUB_common.h"

#ifdef __REMOTE_TR12__
const unsigned long	remoteCtrlCheckValue	= KEY_REMOTE_TR12_8;				//ʹ�ܺ�ȡ��ң�������ܵ�ʱ��ʹ�õĹؼ���ֵ
const unsigned long	remoteCtrlEnterValue	= KEY_REMOTE_TR12_ENTER;			//ʹ�ܺ�ȡ��ң�������ܵ�ʱ��ʹ�õĹؼ���ֵ
#else
const unsigned long	remoteCtrlCheckValue	= KEY_REMOTE_8;						//ʹ�ܺ�ȡ��ң�������ܵ�ʱ��ʹ�õĹؼ���ֵ
const unsigned long	remoteCtrlEnterValue	= KEY_REMOTE_ENTER;					//ʹ�ܺ�ȡ��ң�������ܵ�ʱ��ʹ�õĹؼ���ֵ
#endif

const unsigned long	remoteCtrlCheckCount	= 2;								//ʹ�ܺ�ȡ��ң����ʱҪ����Ĺؼ���ֵ�Ĵ���
const unsigned long	maxRemoteNotinputTime	= 6;								//�ڿ���ң�������ܽ׶�,�ȴ��´�ң������ֵ�ĳ�ʱʱ��, s
const unsigned long	remoteNotusedTime		= 3 * 60; 							//ң������Ч��ʱ�������ʱ��û������Ӧ�Զ��ر�ң�ع���

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
	

	int m_dvrID;									//�洢����DVR�豸��
	int m_remoteInputDVRid;							//ң���������DVR id
	bool m_bRemoteControlling;						//Ϊ���ʾң�������ڿ��Ƹ�DVR��Ϊ�ٱ�ʾң������û���Ƹ�DVR
	unsigned long m_lastInputTime;					//�ϴ�ң�������ʱ��
	unsigned long m_checkCount;						//��¼���뿪��ң����ʱ����8�ĸ���
};
#endif


