#include "PTZRm110.h"

#ifdef	LJ_DEBUG
#include "user_config.h"
#endif

CPTZRm110::CPTZRm110()
{
	m_Baudrate = SBR_9600;
}

CPTZRm110::~CPTZRm110()
{

}

/*  hby
 *  �����ʽ:  ����9Byte, COMMANDLEN = 7  �ر�ע��ֹͣ����Ĳ�ͬ
 *  ��HIK��DVR�����ֹͣ���������������
 * ���� m_ucCommand[1] = 0x48��ʱ���� ��,��,��,�ҵ�ֹͣ����
 *	    m_ucCommand[1] = 0x4A��ʱ���� ��������ͷ�Ŀ��Ƶ�ֹͣ����
 *    ����,���Ե�ʱ����0x48��0x4A��������Ϊ�������ҵ�ֹͣ����,
 *    ����,ֹͣ�����Ϊ0x4A.
 
 *   ע��2:
 *      RM110Э�鲻�ܹ������ٶ�.
 */

bool CPTZRm110::Stop()		// F6 4A 00 01 00 01 4C 
{
	m_ucCommand[0] = 0xF6;
	m_ucCommand[1] = 0x4A;
	m_ucCommand[2] = 0x00;
	m_ucCommand[3] = m_lDevAddress;
	m_ucCommand[4] = 0x00;			
	m_ucCommand[5] = 0x01;			//Stop
	m_ucCommand[6] = GetCheckSum();
	return (CSerialPort::Instance())->WriteToPort(m_ucCommand,COMMANDLEN,m_Baudrate);
}

bool CPTZRm110::Left()			// F6 45 00 01 09 01 50
{
	m_ucCommand[0] = 0xF6;
	m_ucCommand[1] = 0x45;
	m_ucCommand[2] = 0x00;
	m_ucCommand[3] = m_lDevAddress;
	m_ucCommand[4] = 0x09;			
	m_ucCommand[5] = 0x01;			//Stop
	m_ucCommand[6] = GetCheckSum();
	return (CSerialPort::Instance())->WriteToPort(m_ucCommand,COMMANDLEN,m_Baudrate);
}

bool CPTZRm110::Right()		//F6 46 00 01 09 01 51
{
	m_ucCommand[0] = 0xF6;
	m_ucCommand[1] = 0x46;
	m_ucCommand[2] = 0x00;
	m_ucCommand[3] = m_lDevAddress;
	m_ucCommand[4] = 0x09;			
	m_ucCommand[5] = 0x01;			//Stop
	m_ucCommand[6] = GetCheckSum();
	return (CSerialPort::Instance())->WriteToPort(m_ucCommand,COMMANDLEN,m_Baudrate);
}

bool CPTZRm110::Up()			//F6 47 00 01 09 01 52
{
	m_ucCommand[0] = 0xF6;
	m_ucCommand[1] = 0x47;
	m_ucCommand[2] = 0x00;
	m_ucCommand[3] = m_lDevAddress;
	m_ucCommand[4] = 0x09;			
	m_ucCommand[5] = 0x01;			//Stop
	m_ucCommand[6] = GetCheckSum();
	return (CSerialPort::Instance())->WriteToPort(m_ucCommand,COMMANDLEN,m_Baudrate);
}

bool CPTZRm110::Down()		//F6 48 00 01 09 01 53
{
	m_ucCommand[0] = 0xF6;
	m_ucCommand[1] = 0x48;
	m_ucCommand[2] = 0x00;
	m_ucCommand[3] = m_lDevAddress;
	m_ucCommand[4] = 0x09;			
	m_ucCommand[5] = 0x01;			//Stop
	m_ucCommand[6] = GetCheckSum();
	return (CSerialPort::Instance())->WriteToPort(m_ucCommand,COMMANDLEN,m_Baudrate);
}

bool CPTZRm110::Near()	////�۽��� ====> ����+  F6 4E 00 02 09 01 5A
{
	m_ucCommand[0] = 0xF6;
	m_ucCommand[1] = 0x4E;
	m_ucCommand[2] = 0x00;
	m_ucCommand[3] = m_lDevAddress;
	m_ucCommand[4] = 0x09;			
	m_ucCommand[5] = 0x01;			//Stop
	m_ucCommand[6] = GetCheckSum();
	return (CSerialPort::Instance())->WriteToPort(m_ucCommand,COMMANDLEN,m_Baudrate);
}

bool CPTZRm110::Far()		//�۽�Զ =====> ����-  F6 4D 00 02 09 01 59
{
	m_ucCommand[0] = 0xF6;
	m_ucCommand[1] = 0x4D;
	m_ucCommand[2] = 0x00;
	m_ucCommand[3] = m_lDevAddress;
	m_ucCommand[4] = 0x09;			
	m_ucCommand[5] = 0x01;			//Stop
	m_ucCommand[6] = GetCheckSum();
	return (CSerialPort::Instance())->WriteToPort(m_ucCommand,COMMANDLEN,m_Baudrate);
}

bool CPTZRm110::ZoomOut()//��д ====> ����+  F6 4B 00 02 09 01 57
{
	m_ucCommand[0] = 0xF6;
	m_ucCommand[1] = 0x4B;
	m_ucCommand[2] = 0x00;
	m_ucCommand[3] = m_lDevAddress;
	m_ucCommand[4] = 0x09;			
	m_ucCommand[5] = 0x01;			//Stop
	m_ucCommand[6] = GetCheckSum();
	return (CSerialPort::Instance())->WriteToPort(m_ucCommand,COMMANDLEN,m_Baudrate);
}

bool CPTZRm110::ZoomIn()//��� =====> ����-  F6 4C 00 02 09 01 58
{
	m_ucCommand[0] = 0xF6;
	m_ucCommand[1] = 0x4C;
	m_ucCommand[2] = 0x00;
	m_ucCommand[3] = m_lDevAddress;
	m_ucCommand[4] = 0x09;			
	m_ucCommand[5] = 0x01;			//Stop
	m_ucCommand[6] = GetCheckSum();
	return (CSerialPort::Instance())->WriteToPort(m_ucCommand,COMMANDLEN,m_Baudrate);
}

bool CPTZRm110::IrisOpen()//��Ȧ�Ŵ�  F6 49 00 02 09 01 55
{
	m_ucCommand[0] = 0xF6;
	m_ucCommand[1] = 0x49;
	m_ucCommand[2] = 0x00;
	m_ucCommand[3] = m_lDevAddress;
	m_ucCommand[4] = 0x09;			
	m_ucCommand[5] = 0x01;			//Stop
	m_ucCommand[6] = GetCheckSum();
	return (CSerialPort::Instance())->WriteToPort(m_ucCommand,COMMANDLEN,m_Baudrate);
}

bool CPTZRm110::IrisClose()//��Ȧ��С F6 4A 00 02 09 01 56
{
	m_ucCommand[0] = 0xF6;
	m_ucCommand[1] = 0x4A;
	m_ucCommand[2] = 0x00;
	m_ucCommand[3] = m_lDevAddress;
	m_ucCommand[4] = 0x09;			
	m_ucCommand[5] = 0x01;			//Stop
	m_ucCommand[6] = GetCheckSum();
	return (CSerialPort::Instance())->WriteToPort(m_ucCommand,COMMANDLEN,m_Baudrate);
}

/*
 * hby �����ĸ�����
 */
bool CPTZRm110::LeftDown()
{
	return true;
}

bool CPTZRm110::LeftUp()
{
	return true;
}

bool CPTZRm110::RightDown()
{
	return true;
}

bool CPTZRm110::RightUp()
{
	return true;
}


bool CPTZRm110::PresetGo(BYTE ucPresetID)
{
	return true;
}

bool CPTZRm110::PresetSet(BYTE ucPresetID)
{
	return true;
}

BYTE CPTZRm110::GetCheckSum()
{
	BYTE CheckSum=0;
	for(int i=1;i<6;i++)
	{
		CheckSum += m_ucCommand[i];
	}
	return CheckSum;
}

// ��� ��
// F6 5E 00 01 01 01 61 
// ��� ��
// F6 5E 00 01 01 00 60 
// 
// �� ��
// F6 5E 00 01 01 00 60 
// �� ��
// F6 5E 00 01 01 00 60
//������������
//��ˢF6 00 00 01 00 01 02 F6 00 00 01 00 01 02 
//�ƹ�F6 00 00 01 00 01 02 F6 00 00 01 00 01 02 
bool CPTZRm110::SetAuxiliary(BYTE auxNum)
{
	if ((AUXILIARY_START >= auxNum) && (AUXILIARY_END <= auxNum))
	{
		assert(false);
		return false;
	}

	m_ucCommand[0] = 0xF6;
	switch(auxNum)
	{
	case AUXILIARY_BRUSH:
		{
			m_ucCommand[1] = 0x00;
			m_ucCommand[5] = 0x01;
		}
		break;
	case AUXILIARY_LAMP:
		{
			m_ucCommand[1] = 0x00;
			m_ucCommand[5] = 0x01;
		}
		break;
	//case AUXILIARY_VIDEO_POWER:
	//	break;
	default:
		{
			return false;
		}
		break;
	}
	m_ucCommand[2] = 0x00;
	m_ucCommand[3] = m_lDevAddress;
	m_ucCommand[4] = 0x00;
	m_ucCommand[6] = GetCheckSum();
	return (CSerialPort::Instance())->WriteToPort(m_ucCommand,COMMANDLEN,m_Baudrate);
}

bool CPTZRm110::ClearAuxiliary(BYTE auxNum)
{
	if ((AUXILIARY_START >= auxNum) && (AUXILIARY_END <= auxNum))
	{
		assert(false);
		return false;
	}

	m_ucCommand[0] = 0xF6;
	switch(auxNum)
	{
	case AUXILIARY_BRUSH:
		{
			m_ucCommand[1] = 0x00;
			m_ucCommand[5] = 0x01;
		}
		break;
	case AUXILIARY_LAMP:
		{
			m_ucCommand[1] = 0x00;
			m_ucCommand[5] = 0x01;
		}
		break;
		//case AUXILIARY_VIDEO_POWER:
		//	break;
	default:
		{
			return false;
		}
		break;
	}
	
	m_ucCommand[2] = 0x00;
	m_ucCommand[3] = m_lDevAddress;
	m_ucCommand[4] = 0x00;			
	m_ucCommand[6] = GetCheckSum();
	return (CSerialPort::Instance())->WriteToPort(m_ucCommand,COMMANDLEN,m_Baudrate);
}

