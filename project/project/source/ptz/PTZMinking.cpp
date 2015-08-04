
#include "PTZMinking.h"

#ifdef	LJ_DEBUG
#include "user_config.h"
#endif

CPTZMinking::CPTZMinking()
{
	m_CurrentCruiseId = 0xFF;
}

CPTZMinking::~CPTZMinking()
{

}

BYTE CPTZMinking::GetCheckSum()
{
	BYTE CheckSum=0;
	long sum = 0;
	for(int i=1;i<9;i++)
	{
		sum += ucCommand[i];
	}
	CheckSum = (BYTE)(0xffff-sum);
	return CheckSum;
}

bool CPTZMinking::Left()
{
	ucCommand[0] = 0xA0;
	ucCommand[1] = m_lDevAddress;
	ucCommand[2] = 0;
	ucCommand[3] = 0x00;
	ucCommand[4] = 0x04;
	ucCommand[5] = GetPanSpeed();
	ucCommand[6] = 0;
	ucCommand[7] = 0;
	ucCommand[8] = 0xFF;
	ucCommand[9] = 0xAF;
	ucCommand[10] = GetCheckSum();
	return (CSerialPort::Instance())->WriteToPort(ucCommand,COMMANDLEN,m_Baudrate);
}

bool CPTZMinking::Right()
{
	ucCommand[0] = 0xA0;
	ucCommand[1] = m_lDevAddress;
	ucCommand[2] = 0;
	ucCommand[3] = 0x00;
	ucCommand[4] = 0x02;
	ucCommand[5] = GetPanSpeed();
	ucCommand[6] = 0;
	ucCommand[7] = 0;
	ucCommand[8] = 0xFF;
	ucCommand[9] = 0xAF;
	ucCommand[10] = GetCheckSum();
	return (CSerialPort::Instance())->WriteToPort(ucCommand,COMMANDLEN,m_Baudrate);
}

bool CPTZMinking::Up()
{
	ucCommand[0] = 0xA0;
	ucCommand[1] = m_lDevAddress;
	ucCommand[2] = 0;
	ucCommand[3] = 0x00;
	ucCommand[4] = 0x08;
	ucCommand[5] = 0;
	ucCommand[6] = GetTiltSpeed();
	ucCommand[7] = 0;
	ucCommand[8] = 0xFF;
	ucCommand[9] = 0xAF;
	ucCommand[10] = GetCheckSum();
	return (CSerialPort::Instance())->WriteToPort(ucCommand,COMMANDLEN,m_Baudrate);
}

bool CPTZMinking::Down()
{
	ucCommand[0] = 0xA0;
	ucCommand[1] = m_lDevAddress;
	ucCommand[2] = 0;
	ucCommand[3] = 0x00;
	ucCommand[4] = 0x10;
	ucCommand[5] = 0;
	ucCommand[6] = GetTiltSpeed();
	ucCommand[7] = 0;
	ucCommand[8] = 0xFF;
	ucCommand[9] = 0xAF;
	ucCommand[10] = GetCheckSum();
	return (CSerialPort::Instance())->WriteToPort(ucCommand,COMMANDLEN,m_Baudrate);
}

bool CPTZMinking::Near()
{
	ucCommand[0] = 0xA0;
	ucCommand[1] = m_lDevAddress;
	ucCommand[2] = 0;
	ucCommand[3] = 0x02;//0x01->0x02	<1>
	ucCommand[4] = 0x00;
	ucCommand[5] = 0;
	ucCommand[6] = 0;
	ucCommand[7] = 0x44;
	ucCommand[8] = 0xFF;
	ucCommand[9] = 0xAF;
	ucCommand[10] = GetCheckSum();
	return (CSerialPort::Instance())->WriteToPort(ucCommand,COMMANDLEN,m_Baudrate);
}

bool CPTZMinking::Far()
{
	ucCommand[0] = 0xA0;
	ucCommand[1] = m_lDevAddress;
	ucCommand[2] = 0;
	ucCommand[3] = 0x01;//0x02->0x01	<1>
	ucCommand[4] = 0x00;
	ucCommand[5] = 0;
	ucCommand[6] = 0;
	ucCommand[7] = 0x44;
	ucCommand[8] = 0xFF;
	ucCommand[9] = 0xAF;
	ucCommand[10] = GetCheckSum();
	return (CSerialPort::Instance())->WriteToPort(ucCommand,COMMANDLEN,m_Baudrate);
}

bool CPTZMinking::ZoomIn()
{
	ucCommand[0] = 0xA0;
	ucCommand[1] = m_lDevAddress;
	ucCommand[2] = 0;
	ucCommand[3] = 0x00;
	ucCommand[4] = 0x40;
	ucCommand[5] = 0;
	ucCommand[6] = 0;
	ucCommand[7] = 0x44;
	ucCommand[8] = 0xFF;
	ucCommand[9] = 0xAF;
	ucCommand[10] = GetCheckSum();
	return (CSerialPort::Instance())->WriteToPort(ucCommand,COMMANDLEN,m_Baudrate);
}

bool CPTZMinking::ZoomOut()
{
	ucCommand[0] = 0xA0;
	ucCommand[1] = m_lDevAddress;
	ucCommand[2] = 0;
	ucCommand[3] = 0x00;
	ucCommand[4] = 0x20;
	ucCommand[5] = 0;
	ucCommand[6] = 0;
	ucCommand[7] = 0x44;
	ucCommand[8] = 0xFF;
	ucCommand[9] = 0xAF;
	ucCommand[10] = GetCheckSum();
	return (CSerialPort::Instance())->WriteToPort(ucCommand,COMMANDLEN,m_Baudrate);
}

bool CPTZMinking::IrisOpen()
{
	ucCommand[0] = 0xA0;
	ucCommand[1] = m_lDevAddress;
	ucCommand[2] = 0;
	ucCommand[3] = 0x10;
	ucCommand[4] = 0x00;
	ucCommand[5] = 0;
	ucCommand[6] = 0;
	ucCommand[7] = 0;
	ucCommand[8] = 0xFF;
	ucCommand[9] = 0xAF;
	ucCommand[10] = GetCheckSum();
	return (CSerialPort::Instance())->WriteToPort(ucCommand,COMMANDLEN,m_Baudrate);
}

bool CPTZMinking::IrisClose()
{
	ucCommand[0] = 0xA0;
	ucCommand[1] = m_lDevAddress;
	ucCommand[2] = 0;
	ucCommand[3] = 0x08;
	ucCommand[4] = 0x00;
	ucCommand[5] = 0;
	ucCommand[6] = 0;
	ucCommand[7] = 0;
	ucCommand[8] = 0xFF;
	ucCommand[9] = 0xAF;
	ucCommand[10] = GetCheckSum();
	return (CSerialPort::Instance())->WriteToPort(ucCommand,COMMANDLEN,m_Baudrate);
}

/*
 * hby 增加四个方向
 */
bool CPTZMinking::LeftDown()
{
	return true;
}
bool CPTZMinking::LeftUp()
{
	return true;
}
bool CPTZMinking::RightDown()
{
	return true;
}
bool CPTZMinking::RightUp()
{
	return true;
}

bool CPTZMinking::PresetGo(BYTE ucPresetID)
{
	ucCommand[0] = 0xA0;
	ucCommand[1] = m_lDevAddress;
	ucCommand[2] = 0X01;
	ucCommand[3] = 0;
	ucCommand[4] = 0x07;
	ucCommand[5] = ucPresetID;
	ucCommand[6] = 0xFF;
	ucCommand[7] = 0xFF;
	ucCommand[8] = 0xFF;
	ucCommand[9] = 0xAF;
	ucCommand[10] = GetCheckSum();
	return (CSerialPort::Instance())->WriteToPort(ucCommand,COMMANDLEN,m_Baudrate);
}



bool CPTZMinking::PresetSet(BYTE ucPresetID)
{
	ucCommand[0] = 0xA0;
	ucCommand[1] = m_lDevAddress;
	ucCommand[2] = 0X01;
	ucCommand[3] = 0;
	ucCommand[4] = 0x03;
	ucCommand[5] = ucPresetID;
	ucCommand[6] = 0xFF;
	ucCommand[7] = 0xFF;
	ucCommand[8] = 0xFF;
	ucCommand[9] = 0xAF;
	ucCommand[10] = GetCheckSum();
	return (CSerialPort::Instance())->WriteToPort(ucCommand,COMMANDLEN,m_Baudrate);
}

bool CPTZMinking::Stop()
{
	ucCommand[0] = 0xA0;
	ucCommand[1] = m_lDevAddress;
	ucCommand[2] = 0;
	ucCommand[3] = 0x00;
	ucCommand[4] = 0x00;
	ucCommand[5] = 0;
	ucCommand[6] = 0;
	ucCommand[7] = 0;
	ucCommand[8] = 0xFF;
	ucCommand[9] = 0xAF;
	ucCommand[10] = GetCheckSum();
	return (CSerialPort::Instance())->WriteToPort(ucCommand,COMMANDLEN,m_Baudrate);
}



BYTE CPTZMinking::GetPanSpeed()
{
	return m_Speed*8-1;//本来为0-63，这个结果为7-63(够用)
}

BYTE CPTZMinking::GetTiltSpeed()
{
	return m_Speed*8-1;//本来为0-63，这个结果为7-63(够用)
}

bool CPTZMinking::RunCruise(BYTE uCruiseId)
{
	//A0 02 02 00 21 00 00 FF FF AF DC ==>cruise 1
	//A0 02 02 00 21 01 00 FF FF AF DB ==>cruise 2

	ucCommand[0] = 0xA0;
	ucCommand[1] = m_lDevAddress;
	ucCommand[2] = m_lDevAddress;
	ucCommand[3] = 0;
	ucCommand[4] = 0x21;
	ucCommand[5] = uCruiseId;
	ucCommand[6] = 0;
	ucCommand[7] = 0xFF;
	ucCommand[8] = 0xFF;
	ucCommand[9] = 0xAF;
	ucCommand[10] = GetCheckSum();
	return (CSerialPort::Instance())->WriteToPort(ucCommand,COMMANDLEN,m_Baudrate);
}

bool CPTZMinking::StopCruise()
{
	//A0 02 02 00 2F FF FF FF FF AF D0
	//A0 42 42 00 2F FF FF FF FF AF 50 
	ucCommand[0] = 0xA0;
	ucCommand[1] = m_lDevAddress;
	ucCommand[2] = m_lDevAddress;
	ucCommand[3] = 0;
	ucCommand[4] = 0x2F;
	ucCommand[5] = 0xFF;
	ucCommand[6] = 0xFF;
	ucCommand[7] = 0xFF;
	ucCommand[8] = 0xFF;
	ucCommand[9] = 0xAF;
	ucCommand[10] = GetCheckSum();
	return (CSerialPort::Instance())->WriteToPort(ucCommand,COMMANDLEN,m_Baudrate);
}

bool CPTZMinking::DeleteCruise(BYTE uCruiseId)
{
	//A0 02 02 00 2B 01 02 FF FF AF CF ==>del 3 
	//A0 02 02 00 2B 01 03 FF FF AF CE ==>del 4
	ucCommand[0] = 0xA0;
	ucCommand[1] = m_lDevAddress;
	ucCommand[2] = m_lDevAddress;
	ucCommand[3] = 0;
	ucCommand[4] = 0x2B;
	ucCommand[5] = 1;
	ucCommand[6] = uCruiseId;
	ucCommand[7] = 0xFF;
	ucCommand[8] = 0xFF;
	ucCommand[9] = 0xAF;
	ucCommand[10] = GetCheckSum();
	return (CSerialPort::Instance())->WriteToPort(ucCommand,COMMANDLEN,m_Baudrate);
}

bool CPTZMinking::EnterCruiseMode(BYTE uCruiseId)
{
	//A0 42 42 00 23 00 80 FF FF AF DA 
	//A0 42 42 00 23 01 80 FF FF AF D9 
	//A0 42 42 00 23 02 80 FF FF AF D8 
	//A0 42 42 00 23 03 80 FF FF AF D7 
	m_CurrentCruiseId = uCruiseId;
	ucCommand[0] = 0xA0;
	ucCommand[1] = m_lDevAddress;
	ucCommand[2] = m_lDevAddress;
	ucCommand[3] = 0;
	ucCommand[4] = 0x23;
	ucCommand[5] = uCruiseId;
	ucCommand[6] = 0x80;
	ucCommand[7] = 0xFF;
	ucCommand[8] = 0xFF;
	ucCommand[9] = 0xAF;
	ucCommand[10] = GetCheckSum();
	return (CSerialPort::Instance())->WriteToPort(ucCommand,COMMANDLEN,m_Baudrate);
}

bool CPTZMinking::ConfigCruise(BYTE ucPresetID, BYTE speed, BYTE waitTime)
{
	ucCommand[0] = 0xA0;
	ucCommand[1] = m_lDevAddress;
	ucCommand[2] = m_lDevAddress;
	ucCommand[3] = 0;
	ucCommand[4] = 0x23;
	ucCommand[5] = m_CurrentCruiseId;
	if ( (0==ucPresetID) && (0==speed) && (0==waitTime)) 
	{
		return true;
	}
	else
	{
		ucCommand[6] = ucPresetID;
	}
	ucCommand[7] = speed;
	ucCommand[8] = waitTime;
	ucCommand[9] = 0xAF;
	ucCommand[10] = GetCheckSum();
	return (CSerialPort::Instance())->WriteToPort(ucCommand,COMMANDLEN,m_Baudrate);
}

bool CPTZMinking::LeaveCruiseMode(BYTE uCruiseId)
{
	ucCommand[0] = 0xA0;
	ucCommand[1] = m_lDevAddress;
	ucCommand[2] = m_lDevAddress;
	ucCommand[3] = 0;
	ucCommand[4] = 0x23;
	ucCommand[5] = uCruiseId;
	ucCommand[6] = 0x40;
	ucCommand[7] = 0xFF;
	ucCommand[8] = 0xFF;
	ucCommand[9] = 0xAF;
	ucCommand[10] = GetCheckSum();
	return (CSerialPort::Instance())->WriteToPort(ucCommand,COMMANDLEN,m_Baudrate);
}

bool CPTZMinking::IsSupportCruise()
{
	printf("CPTZMinking support cruise\n");
	return true;
}


//辅助开关控制
bool CPTZMinking::SetAuxiliary(BYTE auxNum)
{
	if ((AUXILIARY_START >= auxNum) && (AUXILIARY_END <= auxNum))
	{
		assert(false);
		return false;
	}

	ucCommand[0] = 0xA0;
	ucCommand[1] = m_lDevAddress;
	ucCommand[2] = m_lDevAddress;
	ucCommand[3] = 0x80;
	ucCommand[4] = 0x01;

	switch(auxNum)
	{
	case AUXILIARY_BRUSH:
		{
			ucCommand[5] = 0x0C;
		}
		break;
	case AUXILIARY_LAMP:
		{
			ucCommand[5] = 0x0F;
		}
		break;
	case AUXILIARY_VIDEO_POWER:
		{
			ucCommand[5] = 0x0E;
		}
		break;
	default:
		{
			return false;
		}
		break;
	}

	ucCommand[6] = 0x01;
	ucCommand[7] = 0xFF;
	ucCommand[8] = 0xFF;
	ucCommand[9] = 0xAF;
	ucCommand[10] = GetCheckSum();
	return (CSerialPort::Instance())->WriteToPort(ucCommand,COMMANDLEN,m_Baudrate);
}

bool CPTZMinking::ClearAuxiliary(BYTE auxNum)
{
	if ((AUXILIARY_START >= auxNum) && (AUXILIARY_END <= auxNum))
	{
		assert(false);
		return false;
	}
	ucCommand[0] = 0xA0;
	ucCommand[1] = m_lDevAddress;
	ucCommand[2] = m_lDevAddress;
	ucCommand[3] = 0x80;
	ucCommand[4] = 0x01;

	switch(auxNum)
	{
	case AUXILIARY_BRUSH:
		{
			ucCommand[5] = 0x0D;
		}
		break;
	case AUXILIARY_LAMP:
		{
			ucCommand[5] = 0x0F;
		}
		break;
	case AUXILIARY_VIDEO_POWER:
		{
			ucCommand[5] = 0x0E;
		}
		break;
	default:
		{
			return false;
		}
		break;
	}
	ucCommand[6] = 0x00;
	ucCommand[7] = 0xFF;
	ucCommand[8] = 0xFF;
	ucCommand[9] = 0xAF;
	ucCommand[10] = GetCheckSum();
	return (CSerialPort::Instance())->WriteToPort(ucCommand,COMMANDLEN,m_Baudrate);
}


