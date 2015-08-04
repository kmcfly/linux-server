#include "RemoteControl.h"



CRemoteControl * CRemoteControl::Instance()
{
	static CRemoteControl s_RemoteControl;
	return &s_RemoteControl;
}

CRemoteControl::CRemoteControl()
{
	m_remoteInputDVRid = 0;
	m_checkCount = 0;
	m_bRemoteControlling = false;
	m_dvrID	= 0;
	m_lastInputTime = GetCurrTime32();
}

CRemoteControl::~CRemoteControl()
{
	
}

//在DVR上改变设备ID
void CRemoteControl::SetDVRID(const unsigned long dvrID )
{ 
	if (m_dvrID != dvrID)
	{
		m_dvrID = dvrID; 
		//改变了DVR ID需要从新初始化
		ResetRemote();
	}

}

bool CRemoteControl::RemoteControlDVR(KEY_VALUE_INFO &keyValue)
{
	if( 0 == m_dvrID )
	{
		//能够控制所有ID为0的DVR
		return true;	
	}
	else
	{
		//ID不是0的时候要解锁DVR遥控器功能
		if( !m_bRemoteControlling )
		{
			if( maxRemoteNotinputTime < ( GetCurrTime32() - m_lastInputTime ) )
			{
				//如果两次输入的时间超过5s就复位遥控器
				ResetRemote();
			}

			m_lastInputTime = GetCurrTime32();

			CheckControlID(keyValue);

			return false;
		}
		else
		{
			if( maxRemoteNotinputTime < ( GetCurrTime32() - m_lastInputTime ) )
			{
				//如果两次输入的时间超过5s就清除遥控器中记录的内容
				m_remoteInputDVRid = 0;
				m_checkCount = 0;
			}

			//可以用遥控器控制DVR时
			if( remoteNotusedTime < ( GetCurrTime32() - m_lastInputTime ) )
			{
				//输入超时
				ResetRemote();
				return false;
			}
			else
			{
				//在可以控制DVR时调用该函数是为了锁DVR
				//在控制DVR x 时输入了 remoteCtrlCheckValue + remoteCtrlCheckValue + y + enter来控制DVR y， 如果x=y，则x任然可以控制，如果x!=y就关闭对x的控制
				//但是在解锁y的时候x任然能响应remoteCtrlCheckValue、y 、 enter，直到能控制y的时候x就不会响应遥控按键
				CheckControlID(keyValue, false);			

				m_lastInputTime = GetCurrTime32();

				return m_bRemoteControlling;
			}
		}
	}
}



int CRemoteControl::RemoteValueToRealValue( const unsigned long &RemoteValue )
{
#ifdef __REMOTE_TR12__
	switch( RemoteValue )
	{
	case KEY_REMOTE_TR12_0:
		return 0;
	case KEY_REMOTE_TR12_1:
		return 1;
	case KEY_REMOTE_TR12_2:
		return 2;
	case KEY_REMOTE_TR12_3:
		return 3;
	case KEY_REMOTE_TR12_4:
		return 4;
	case KEY_REMOTE_TR12_5:
		return 5;
	case KEY_REMOTE_TR12_6:
		return 6;
	case KEY_REMOTE_TR12_7:
		return 7;
	case KEY_REMOTE_TR12_8:
		return 8;
	case KEY_REMOTE_TR12_9:
		return 9;
	default: 
		return -1;
	}
#else
	switch( RemoteValue )
	{
	case KEY_REMOTE_0:
		return 0;
	case KEY_REMOTE_1:
		return 1;
	case KEY_REMOTE_2:
		return 2;
	case KEY_REMOTE_3:
		return 3;
	case KEY_REMOTE_4:
		return 4;
	case KEY_REMOTE_5:
		return 5;
	case KEY_REMOTE_6:
		return 6;
	case KEY_REMOTE_7:
		return 7;
	case KEY_REMOTE_8:
		return 8;
	case KEY_REMOTE_9:
		return 9;
	default: 
		return -1;
	}
#endif
}




void CRemoteControl::ResetRemote()
{
	m_remoteInputDVRid = 0;
	m_checkCount = 0;
	m_bRemoteControlling = false;
}



void CRemoteControl::CheckControlID( KEY_VALUE_INFO &keyValue, bool bOpen )
{
	//输入了关键字8
	if( remoteCtrlCheckValue == keyValue.value )
	{
		if( remoteCtrlCheckCount != m_checkCount )
		{
			m_checkCount++;
		}
		else
		{
			//记录输入的设备id中有关键字8
			assert( -1 != RemoteValueToRealValue( keyValue.value ) );
			m_remoteInputDVRid = m_remoteInputDVRid * 10 + RemoteValueToRealValue( keyValue.value );
		}
	}
	//输入了其他按键值
	else
	{
		if( remoteCtrlCheckCount != m_checkCount )
		{
			if( bOpen )
			{
				ResetRemote();
			}
		}
		else
		{
			if( -1 == RemoteValueToRealValue( keyValue.value ) )
			{
				//输入了除数字外的按键的时候只有ENTER才有效
				if( ( remoteCtrlEnterValue == keyValue.value ) && ( m_dvrID == m_remoteInputDVRid ) )
				{
					printf("%s():%s:%d, from here the remote will control the DVR \n", __FUNCTION__, __FILE__, __LINE__);
					ResetRemote();	
					m_bRemoteControlling = true;
				}
				else
				{
					printf("%s():%s:%d, from here the remote will cant not control the DVR \n", __FUNCTION__, __FILE__, __LINE__);
					ResetRemote();
				}
			}
			else
			{
				//输入的是设备ID
				m_remoteInputDVRid = ( m_remoteInputDVRid * 10 ) + RemoteValueToRealValue( keyValue.value );
			}
		}

	}
}

