
#include "PTZLilin.h"

#ifdef	LJ_DEBUG
#include "user_config.h"
#endif

CPTZLilin::CPTZLilin()
{
	m_curDwell=0;
	m_curSpeed=0;
}

CPTZLilin::~CPTZLilin()
{

}

bool CPTZLilin::Stop()
{
	m_ucCommand[0] = m_lDevAddress;
	m_ucCommand[1] = 0x00;
	m_ucCommand[2] = 0xFF;
	return (CSerialPort::Instance())->WriteToPort(m_ucCommand,COMMANDLEN,m_Baudrate);
}

bool CPTZLilin::Left()
{	
	m_ucCommand[0] = m_lDevAddress;
	m_ucCommand[1] = 0x02;
	m_ucCommand[2] = GetPanSpeed();
	return (CSerialPort::Instance())->WriteToPort(m_ucCommand,COMMANDLEN,m_Baudrate);
}

bool CPTZLilin::Right()
{
	m_ucCommand[0] = m_lDevAddress;
	m_ucCommand[1] = 0x01;
	m_ucCommand[2] = GetPanSpeed();
	return (CSerialPort::Instance())->WriteToPort(m_ucCommand,COMMANDLEN,m_Baudrate);
}

bool CPTZLilin::Up()
{
	m_ucCommand[0] = m_lDevAddress;
	m_ucCommand[1] = 0x04;
	m_ucCommand[2] = GetTiltSpeed();
	return (CSerialPort::Instance())->WriteToPort(m_ucCommand,COMMANDLEN,m_Baudrate);
}

bool CPTZLilin::Down()
{
	m_ucCommand[0] = m_lDevAddress;
	m_ucCommand[1] = 0x08;
	m_ucCommand[2] =GetTiltSpeed();
	return (CSerialPort::Instance())->WriteToPort(m_ucCommand,COMMANDLEN,m_Baudrate);
}

bool CPTZLilin::Near()
{
	m_ucCommand[0] = m_lDevAddress;
	m_ucCommand[1] = 0x40;
	m_ucCommand[2] = 0xFF;
	return (CSerialPort::Instance())->WriteToPort(m_ucCommand,COMMANDLEN,m_Baudrate);
}

bool CPTZLilin::Far()
{
	BYTE m_ucCommand[COMMANDLEN];
	m_ucCommand[0] = m_lDevAddress;
	m_ucCommand[1] = 0x80;
	m_ucCommand[2] = 0xFF;
	return (CSerialPort::Instance())->WriteToPort(m_ucCommand,COMMANDLEN,m_Baudrate);
}

bool CPTZLilin::ZoomOut()//wide
{
	m_ucCommand[0] = m_lDevAddress;
	m_ucCommand[1] = 0x10;	//0x10->0x20	<1>
	m_ucCommand[2] = 0xFF;
	return (CSerialPort::Instance())->WriteToPort(m_ucCommand,COMMANDLEN,m_Baudrate);
}

bool CPTZLilin::ZoomIn()
{
	m_ucCommand[0] = m_lDevAddress;
	m_ucCommand[1] = 0x20;	//0x20->0x10	<1>
	m_ucCommand[2] = 0xFF;
	return (CSerialPort::Instance())->WriteToPort(m_ucCommand,COMMANDLEN,m_Baudrate);
	
}

bool CPTZLilin::IrisOpen()
{
	m_ucCommand[0] = m_lDevAddress;
	m_ucCommand[1] = 0x00;
	m_ucCommand[2] = 0x00;
	return (CSerialPort::Instance())->WriteToPort(m_ucCommand,COMMANDLEN,m_Baudrate);
}

bool CPTZLilin::IrisClose()
{
	m_ucCommand[0] = m_lDevAddress;
	m_ucCommand[1] = 0x00;
	m_ucCommand[2] = 0x01;
	return (CSerialPort::Instance())->WriteToPort(m_ucCommand,COMMANDLEN,m_Baudrate);
}

/*
 * hby 增加四个方向
 */
bool CPTZLilin::LeftDown()
{
	m_ucCommand[0] = m_lDevAddress;
	m_ucCommand[1] = 0x0A;
	m_ucCommand[2] = GetPanSpeed();
	return (CSerialPort::Instance())->WriteToPort(m_ucCommand,COMMANDLEN,m_Baudrate);

}
bool CPTZLilin::LeftUp()
{
	m_ucCommand[0] = m_lDevAddress;
	m_ucCommand[1] = 0x06;
	m_ucCommand[2] = GetPanSpeed();
	return (CSerialPort::Instance())->WriteToPort(m_ucCommand,COMMANDLEN,m_Baudrate);

}
bool CPTZLilin::RightDown()
{
	m_ucCommand[0] = m_lDevAddress;
	m_ucCommand[1] = 0x09;
	m_ucCommand[2] = GetPanSpeed();
	return (CSerialPort::Instance())->WriteToPort(m_ucCommand,COMMANDLEN,m_Baudrate);

}
bool CPTZLilin::RightUp()
{
	m_ucCommand[0] = m_lDevAddress;
	m_ucCommand[1] = 0x05;
	m_ucCommand[2] = GetPanSpeed();
	return (CSerialPort::Instance())->WriteToPort(m_ucCommand,COMMANDLEN,m_Baudrate);
}


bool CPTZLilin::PresetGo(BYTE ucPresetID)
{
	if(ucPresetID ==0)
	{
		m_ucCommand[0] = 0;
		m_ucCommand[1] = 0;
		m_ucCommand[2] = 0x00;
		(CSerialPort::Instance())->WriteToPort(m_ucCommand,COMMANDLEN,m_Baudrate);
		PUB_Sleep(20);
		(CSerialPort::Instance())->WriteToPort(m_ucCommand,COMMANDLEN,m_Baudrate);
		PUB_Sleep(20);
	}
	m_ucCommand[0] = 0x40+m_lDevAddress;
	m_ucCommand[1] = ucPresetID;
	m_ucCommand[2] = 0x00;
	return (CSerialPort::Instance())->WriteToPort(m_ucCommand,COMMANDLEN,m_Baudrate);
}

bool CPTZLilin::PresetSet(BYTE ucPresetID)
{
	m_ucCommand[0] = 0x80+m_lDevAddress;
	m_ucCommand[1] = m_curDwell;
	m_ucCommand[2] = 128;//速度1-255
	return (CSerialPort::Instance())->WriteToPort(m_ucCommand,COMMANDLEN,m_Baudrate);
}


BYTE CPTZLilin::GetPanSpeed()
{
	if(m_Speed == 1)
		return 0x01|0x80;
	else
		return (m_Speed-1)|0x80;//低3位为PAN(LR)速度,3-5为TILT速度，7位为0，8位为1
}

BYTE CPTZLilin::GetTiltSpeed()
{
	if(m_Speed == 1)
		return ((0x01<<3)|0x80);
	else
		return ((m_Speed-1)<<3)|0x80;//低3位为PAN(LR)速度,3-5为TILT速度，7位为0，8位为1
}
