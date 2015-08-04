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

//��DVR�ϸı��豸ID
void CRemoteControl::SetDVRID(const unsigned long dvrID )
{ 
	if (m_dvrID != dvrID)
	{
		m_dvrID = dvrID; 
		//�ı���DVR ID��Ҫ���³�ʼ��
		ResetRemote();
	}

}

bool CRemoteControl::RemoteControlDVR(KEY_VALUE_INFO &keyValue)
{
	if( 0 == m_dvrID )
	{
		//�ܹ���������IDΪ0��DVR
		return true;	
	}
	else
	{
		//ID����0��ʱ��Ҫ����DVRң��������
		if( !m_bRemoteControlling )
		{
			if( maxRemoteNotinputTime < ( GetCurrTime32() - m_lastInputTime ) )
			{
				//������������ʱ�䳬��5s�͸�λң����
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
				//������������ʱ�䳬��5s�����ң�����м�¼������
				m_remoteInputDVRid = 0;
				m_checkCount = 0;
			}

			//������ң��������DVRʱ
			if( remoteNotusedTime < ( GetCurrTime32() - m_lastInputTime ) )
			{
				//���볬ʱ
				ResetRemote();
				return false;
			}
			else
			{
				//�ڿ��Կ���DVRʱ���øú�����Ϊ����DVR
				//�ڿ���DVR x ʱ������ remoteCtrlCheckValue + remoteCtrlCheckValue + y + enter������DVR y�� ���x=y����x��Ȼ���Կ��ƣ����x!=y�͹رն�x�Ŀ���
				//�����ڽ���y��ʱ��x��Ȼ����ӦremoteCtrlCheckValue��y �� enter��ֱ���ܿ���y��ʱ��x�Ͳ�����Ӧң�ذ���
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
	//�����˹ؼ���8
	if( remoteCtrlCheckValue == keyValue.value )
	{
		if( remoteCtrlCheckCount != m_checkCount )
		{
			m_checkCount++;
		}
		else
		{
			//��¼������豸id���йؼ���8
			assert( -1 != RemoteValueToRealValue( keyValue.value ) );
			m_remoteInputDVRid = m_remoteInputDVRid * 10 + RemoteValueToRealValue( keyValue.value );
		}
	}
	//��������������ֵ
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
				//�����˳�������İ�����ʱ��ֻ��ENTER����Ч
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
				//��������豸ID
				m_remoteInputDVRid = ( m_remoteInputDVRid * 10 ) + RemoteValueToRealValue( keyValue.value );
			}
		}

	}
}

