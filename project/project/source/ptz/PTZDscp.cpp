
#include "PTZDscp.h"
#ifdef	LJ_DEBUG
#include "user_config.h"
#endif

CPTZDscp::CPTZDscp()
{

}

CPTZDscp::~CPTZDscp()
{

}

bool CPTZDscp::Left()
{
	ucCommand[0] = m_lDevAddress;
	ucCommand[1] = 0xFF;
	ucCommand[2] = 0x18;
	ucCommand[3] = 0x01;
	ucCommand[4] = GetPanSpeed();
	ucCommand[5] = GetCheckSum();
	return (CSerialPort::Instance())->WriteToPort(ucCommand,COMMANDLEN,m_Baudrate);
	
}

bool CPTZDscp::Right()
{
	ucCommand[0] = m_lDevAddress;
	ucCommand[1] = 0xFF;
	ucCommand[2] = 0x18;
	ucCommand[3] = 0x00;
	ucCommand[4] = GetPanSpeed();
	ucCommand[5] = GetCheckSum();
	return (CSerialPort::Instance())->WriteToPort(ucCommand,COMMANDLEN,m_Baudrate);
	
}

bool CPTZDscp::Up()
{
	ucCommand[0] = m_lDevAddress;
	ucCommand[1] = 0xFF;
	ucCommand[2] = 0x18;
	ucCommand[3] = 0x02;
	ucCommand[4] = GetTiltSpeed();
	ucCommand[5] = GetCheckSum();
	return (CSerialPort::Instance())->WriteToPort(ucCommand,COMMANDLEN,m_Baudrate);
}

bool CPTZDscp::Down()
{
	ucCommand[0] = m_lDevAddress;
	ucCommand[1] = 0xFF;
	ucCommand[2] = 0x18;
	ucCommand[3] = 0x03;
	ucCommand[4] = GetTiltSpeed();
	ucCommand[5] = GetCheckSum();
	return (CSerialPort::Instance())->WriteToPort(ucCommand,COMMANDLEN,m_Baudrate);
	
}

bool CPTZDscp::Near()
{
	ucCommand[0] = m_lDevAddress;
	ucCommand[1] = 0xFF;
	ucCommand[2] = 0x25;
	ucCommand[3] = 0x00;
	ucCommand[4] = 0;
	ucCommand[5] = GetCheckSum();
	return (CSerialPort::Instance())->WriteToPort(ucCommand,COMMANDLEN,m_Baudrate);
}

bool CPTZDscp::Far()
{
	ucCommand[0] = m_lDevAddress;
	ucCommand[1] = 0xFF;
	ucCommand[2] = 0x25;
	ucCommand[3] = 0x01;
	ucCommand[4] = 0;
	ucCommand[5] = GetCheckSum();
	return (CSerialPort::Instance())->WriteToPort(ucCommand,COMMANDLEN,m_Baudrate);
	
}

bool CPTZDscp::ZoomIn()
{
	ucCommand[0] = m_lDevAddress;
	ucCommand[1] = 0xFF;
	ucCommand[2] = 0x24;
	ucCommand[3] = 0x00;
	ucCommand[4] = 0x00;
	ucCommand[5] = GetCheckSum();
	return (CSerialPort::Instance())->WriteToPort(ucCommand,COMMANDLEN,m_Baudrate);
}

bool CPTZDscp::ZoomOut()
{
	ucCommand[0] = m_lDevAddress;
	ucCommand[1] = 0xFF;
	ucCommand[2] = 0x24;
	ucCommand[3] = 0x01;
	ucCommand[4] = 0x00;
	ucCommand[5] = GetCheckSum();
	return (CSerialPort::Instance())->WriteToPort(ucCommand,COMMANDLEN,m_Baudrate);
}

bool CPTZDscp::Stop()
{
	ucCommand[0] = m_lDevAddress;
	ucCommand[1] = 0xFF;
	ucCommand[2] = 0x14;
	ucCommand[3] = 0x00;
	ucCommand[4] = 0x00;
	ucCommand[5] = GetCheckSum();
	(CSerialPort::Instance())->WriteToPort(ucCommand,COMMANDLEN,m_Baudrate);
	ucCommand[2] = 0x13;
	ucCommand[3] = 0x00;
	ucCommand[5] = GetCheckSum();
	(CSerialPort::Instance())->WriteToPort(ucCommand,COMMANDLEN,m_Baudrate);
	ucCommand[2] = 0x24;
	ucCommand[3] = 0x04;
	ucCommand[5] = GetCheckSum();
	(CSerialPort::Instance())->WriteToPort(ucCommand,COMMANDLEN,m_Baudrate);
	ucCommand[2] = 0x25;
	ucCommand[3] = 0x04;
	ucCommand[5] = GetCheckSum();
	return (CSerialPort::Instance())->WriteToPort(ucCommand,COMMANDLEN,m_Baudrate);
}

/*
 * hby 增加四个方向
 */
bool CPTZDscp::LeftDown()
{
	return true;
}
bool CPTZDscp::LeftUp()
{
	return true;
}
bool CPTZDscp::RightDown()
{
	return true;
}
bool CPTZDscp::RightUp()
{
	return true;
}


BYTE CPTZDscp::GetCheckSum()
{
	BYTE CheckSum=0;
	for(int i=0;i<5;i++)
	{
		CheckSum ^= ucCommand[i];
	}
	return CheckSum;
}

bool CPTZDscp::PresetGo(BYTE ucPresetID)
{
	ucCommand[0] = m_lDevAddress;
	ucCommand[1] = 0xFF;
	ucCommand[2] = 0x11;
	ucCommand[3] = 0x00;
	ucCommand[4] = ucPresetID+1;
	ucCommand[5] = GetCheckSum();
	return (CSerialPort::Instance())->WriteToPort(ucCommand,COMMANDLEN,m_Baudrate);
}

bool CPTZDscp::PresetSet(BYTE ucPresetID)
{
	ucCommand[0] = m_lDevAddress;
	ucCommand[1] = 0xFF;
	ucCommand[2] = 0x1D;
	ucCommand[3] = 0x00;
	ucCommand[4] = ucPresetID + 1;
	ucCommand[5] = GetCheckSum();
	return (CSerialPort::Instance())->WriteToPort(ucCommand,COMMANDLEN,m_Baudrate);
}



bool CPTZDscp::IrisOpen()
{
	return true;
}

bool CPTZDscp::IrisClose()
{
	return true;
}

BYTE CPTZDscp::GetPanSpeed()
{
	return m_Speed*8-1;//本来为0-63，这个结果为7-63(够用)
}

BYTE CPTZDscp::GetTiltSpeed()
{
	return m_Speed*8-1;//本来为0-63，这个结果为7-63(够用)
}
