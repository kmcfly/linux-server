#include "PTZHy.h"

#ifdef	LJ_DEBUG
#include "user_config.h"
#endif

CPTZHy::CPTZHy()
{
	m_Baudrate = SBR_9600;
}

CPTZHy::~CPTZHy()
{

}

bool CPTZHy::Stop()
{						//05 08 3A 89 A5 01 22 00 86 00
	printf("Enter thr Stop Command\n");
	m_ucCommand[0] = 0x05;
	m_ucCommand[1] = 0x08;
	m_ucCommand[2] = 0x3A;
	m_ucCommand[3] = 0x89;
	m_ucCommand[4] = 0xA5;
	m_ucCommand[5] = m_lDevAddress;//0x01;//	0x00;//		
	m_ucCommand[6] = 0x20 + m_cmd;			//Stop
	m_ucCommand[7] = 0x00;
	m_ucCommand[8] = GetCheckSum();
	m_ucCommand[9] = 0x00;
	return (CSerialPort::Instance())->WriteToPort(m_ucCommand,COMMANDLEN,m_Baudrate);
}

bool CPTZHy::Left()
{						//05 08 3A 89 A5 00 04 00 A1 00 
	m_ucCommand[0] = 0x05;
	m_ucCommand[1] = 0x08;
	m_ucCommand[2] = 0x3A;
	m_ucCommand[3] = 0x89;
	m_ucCommand[4] = 0xA5;
	m_ucCommand[5] = m_lDevAddress;//0x00;//	0x01;//		
	m_ucCommand[6] = 0x04;		//Left
	m_ucCommand[7] = 0x00;
	m_ucCommand[8] = GetCheckSum();//0x9F;//0xA0;//
	m_ucCommand[9] = 0x00;
	m_cmd = 0x04;
	return (CSerialPort::Instance())->WriteToPort(m_ucCommand,COMMANDLEN,m_Baudrate);
}

bool CPTZHy::Right()
{						//05 08 3A 89 A5 01 05 00 A1 00
	m_ucCommand[0] = 0x05;
	m_ucCommand[1] = 0x08;
	m_ucCommand[2] = 0x3A;
	m_ucCommand[3] = 0x89;
	m_ucCommand[4] = 0xA5;
	m_ucCommand[5] = m_lDevAddress;//0x01;//0x00;//			
	m_ucCommand[6] = 0x05;		//Right
	m_ucCommand[7] = 0x00;
	m_ucCommand[8] = GetCheckSum();//0xA0;0xA0;//0xA1;//
	m_ucCommand[9] = 0x00;
	m_cmd = 0x05;
	return (CSerialPort::Instance())->WriteToPort(m_ucCommand,COMMANDLEN,m_Baudrate);
}

bool CPTZHy::Up()
{						//05 08 3A 89 A5 01 02 00 A6 00
	printf("ENDTER hy UP\n");
	m_ucCommand[0] = 0x05;
	m_ucCommand[1] = 0x08;
	m_ucCommand[2] = 0x3A;
	m_ucCommand[3] = 0x89;
	m_ucCommand[4] = 0xA5;
	m_ucCommand[5] = m_lDevAddress;//0x01;//0x00;	//		
	m_ucCommand[6] = 0x02;			//Up
	m_ucCommand[7] = 0x00;
	m_ucCommand[8] = GetCheckSum();//0xA7;0xA5;//0xA6;//
	m_ucCommand[9] = 0x00;
	m_cmd = 0x02;
	return (CSerialPort::Instance())->WriteToPort(m_ucCommand,COMMANDLEN,m_Baudrate);
}

bool CPTZHy::Down()
{						//05 08 3A 89 A5 01 03 00 A7 00
	m_ucCommand[0] = 0x05;
	m_ucCommand[1] = 0x08;
	m_ucCommand[2] = 0x3A;
	m_ucCommand[3] = 0x89;
	m_ucCommand[4] = 0xA5;
	m_ucCommand[5] = m_lDevAddress;//0x01;//0x00;//			
	m_ucCommand[6] = 0x03;			//Down
	m_ucCommand[7] = 0x00;
	m_ucCommand[8] = GetCheckSum();//0xA6;0xA6;//0xA7;//
	m_ucCommand[9] = 0x00;
	m_cmd = 0x03;
	return (CSerialPort::Instance())->WriteToPort(m_ucCommand,COMMANDLEN,m_Baudrate);
}

bool CPTZHy::Near()		//05 08 3A 89 A5 01 07 00 A3 00 
{
	m_ucCommand[0] = 0x05;
	m_ucCommand[1] = 0x08;
	m_ucCommand[2] = 0x3A;
	m_ucCommand[3] = 0x89;
	m_ucCommand[4] = 0xA5;
	m_ucCommand[5] = m_lDevAddress;//0x00;			
	m_ucCommand[6] = 0x07;			//Down
	m_ucCommand[7] = 0x00;
	m_ucCommand[8] = GetCheckSum();//0xA3;
	m_ucCommand[9] = 0x00;
	m_cmd = 0x07;
	return (CSerialPort::Instance())->WriteToPort(m_ucCommand,COMMANDLEN,m_Baudrate);
}

bool CPTZHy::Far()	//05 08 3A 89 A5 01 06 00 A2 00		
{
	m_ucCommand[0] = 0x05;
	m_ucCommand[1] = 0x08;
	m_ucCommand[2] = 0x3A;
	m_ucCommand[3] = 0x89;
	m_ucCommand[4] = 0xA5;
	m_ucCommand[5] = m_lDevAddress;//0x00;			
	m_ucCommand[6] = 0x06;			//Down
	m_ucCommand[7] = 0x00;
	m_ucCommand[8] = 0xA2;
	m_ucCommand[9] = 0x00;
	m_cmd = 0x06;
	return (CSerialPort::Instance())->WriteToPort(m_ucCommand,COMMANDLEN,m_Baudrate);
}

bool CPTZHy::ZoomOut()		//05 08 3A 89 A5 01 09 00 AD 00
{
	m_ucCommand[0] = 0x05;
	m_ucCommand[1] = 0x08;
	m_ucCommand[2] = 0x3A;
	m_ucCommand[3] = 0x89;
	m_ucCommand[4] = 0xA5;
	m_ucCommand[5] = m_lDevAddress;//0x00;			
	m_ucCommand[6] = 0x09;			//Down
	m_ucCommand[7] = 0x00;
	m_ucCommand[8] = 0xAD;
	m_ucCommand[9] = 0x00;
	m_cmd = 0x09;
	return (CSerialPort::Instance())->WriteToPort(m_ucCommand,COMMANDLEN,m_Baudrate);
}

bool CPTZHy::ZoomIn()			//05 08 3A 89 A5 01 08 00 AC 00
{
	m_ucCommand[0] = 0x05;
	m_ucCommand[1] = 0x08;
	m_ucCommand[2] = 0x3A;
	m_ucCommand[3] = 0x89;
	m_ucCommand[4] = 0xA5;
	m_ucCommand[5] = m_lDevAddress;//0x00;			
	m_ucCommand[6] = 0x08;			//Down
	m_ucCommand[7] = 0x00;
	m_ucCommand[8] = 0xAC;
	m_ucCommand[9] = 0x00;
	m_cmd = 0x08;
	return (CSerialPort::Instance())->WriteToPort(m_ucCommand,COMMANDLEN,m_Baudrate);
}

bool CPTZHy::IrisOpen()		//05 08 3A 89 A5 01 0A 00 AE 00
{
	m_ucCommand[0] = 0x05;
	m_ucCommand[1] = 0x08;
	m_ucCommand[2] = 0x3A;
	m_ucCommand[3] = 0x89;
	m_ucCommand[4] = 0xA5;
	m_ucCommand[5] = m_lDevAddress;//0x00;			
	m_ucCommand[6] = 0x0A;			//Down
	m_ucCommand[7] = 0x00;
	m_ucCommand[8] = 0xAE;
	m_ucCommand[9] = 0x00;
	m_cmd = 0x0A;
	return (CSerialPort::Instance())->WriteToPort(m_ucCommand,COMMANDLEN,m_Baudrate);
}

bool CPTZHy::IrisClose()		//05 08 3A 89 A5 01 0B 00 AF 00
{
	m_ucCommand[0] = 0x05;
	m_ucCommand[1] = 0x08;
	m_ucCommand[2] = 0x3A;
	m_ucCommand[3] = 0x89;
	m_ucCommand[4] = 0xA5;
	m_ucCommand[5] = m_lDevAddress;//0x00;			
	m_ucCommand[6] = 0x0B;			//Down
	m_ucCommand[7] = 0x00;
	m_ucCommand[8] = 0xAF;
	m_ucCommand[9] = 0x00;
	m_cmd = 0x0B;
	return (CSerialPort::Instance())->WriteToPort(m_ucCommand,COMMANDLEN,m_Baudrate);
}

/*
 * hby 增加四个方向
 */
bool CPTZHy::LeftDown()
{
	return true;
}

bool CPTZHy::LeftUp()
{
	return true;
}

bool CPTZHy::RightDown()
{
	return true;
}

bool CPTZHy::RightUp()
{
	return true;
}

bool CPTZHy::PresetGo(BYTE ucPresetID)
{
	ucPresetID = ucPresetID; //防止编译不过
	printf("HY do not support PresetGo\n");
	return true;
}

bool CPTZHy::PresetSet(BYTE ucPresetID)
{
	ucPresetID = ucPresetID; //防止编译不过
	printf("HY do not support PresetGo\n");
	return true;
}

BYTE CPTZHy::GetCheckSum()
{
	BYTE CheckSum=0;
	for(BYTE Index=4; Index<7; Index++)
	{
		CheckSum ^= m_ucCommand[Index];
	}
	return CheckSum;
}

bool CPTZHy::IsSupportCruise()
{
	return true;
}
