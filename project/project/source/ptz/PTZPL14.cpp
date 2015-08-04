#include "PTZPL14.h"

//#define DDBUG
#define DBUG_MSG(function, cmd) \
{\
	printf(function);\
	for(int i=0; i<COMMANDLEN; i++)\
	{\
		printf("%x ",cmd[i]);\
	}\
	printf("\n");\
};


CPTZPL14::CPTZPL14()
{

}
CPTZPL14::~CPTZPL14()
{

}

void CPTZPL14::AutoScan(int nGroupID)
{

}
bool CPTZPL14::ZoomOut()
{
	m_ucCommand[0] = 0x4E;
	m_ucCommand[1] = 0x00;
	m_ucCommand[2] = m_lDevAddress;
	m_ucCommand[3] = 0x00;
	m_ucCommand[4] = 0x01;
	m_ucCommand[5] = 0x00;
	m_ucCommand[6] = 0x80;
	m_ucCommand[7] = 0x80;
	m_ucCommand[8] = 0x40 - GetSpeed();;
	m_ucCommand[9] = GetCheckSum();
#ifdef DDBUG
	DBUG_MSG("Zoom In:",m_ucCommand);
	return true;
#else
	return (CSerialPort::Instance())->WriteToPort(m_ucCommand,COMMANDLEN,m_Baudrate);
#endif
}
bool CPTZPL14::ZoomIn()
{
	m_ucCommand[0] = 0x4E;
	m_ucCommand[1] = 0x00;
	m_ucCommand[2] = m_lDevAddress;
	m_ucCommand[3] = 0x00;
	m_ucCommand[4] = 0x01;
	m_ucCommand[5] = 0x00;
	m_ucCommand[6] = 0x80;
	m_ucCommand[7] = 0x80;
	m_ucCommand[8] = 0xBF + GetSpeed();;
	m_ucCommand[9] =GetCheckSum();
#ifdef DDBUG
	DBUG_MSG("Zoom Out:",m_ucCommand);
	return true;
#else
	return (CSerialPort::Instance())->WriteToPort(m_ucCommand,COMMANDLEN,m_Baudrate);
#endif
}

bool CPTZPL14::Near()
{
	m_ucCommand[0] = 0x4E;
	m_ucCommand[1] = 0x00;
	m_ucCommand[2] = m_lDevAddress;
	m_ucCommand[3] = 0x00;
	m_ucCommand[4] = 0x13;
	m_ucCommand[5] = 0x00;
	m_ucCommand[6] = 0x80;
	m_ucCommand[7] = 0x80;
	m_ucCommand[8] = 0x80;
	m_ucCommand[9] =GetCheckSum();
#ifdef DDBUG
	DBUG_MSG("Near:",m_ucCommand);
	return true;
#else
	return (CSerialPort::Instance())->WriteToPort(m_ucCommand,COMMANDLEN,m_Baudrate);
#endif
}

bool CPTZPL14::Far()
{
	m_ucCommand[0] = 0x4E;
	m_ucCommand[1] = 0x00;
	m_ucCommand[2] = m_lDevAddress;
	m_ucCommand[3] = 0x00;
	m_ucCommand[4] = 0x12;
	m_ucCommand[5] = 0x00;
	m_ucCommand[6] = 0x80;
	m_ucCommand[7] = 0x80;
	m_ucCommand[8] = 0x80;
	m_ucCommand[9] =GetCheckSum();
#ifdef DDBUG
	DBUG_MSG("far:",m_ucCommand);
	return true;
#else
	return (CSerialPort::Instance())->WriteToPort(m_ucCommand,COMMANDLEN,m_Baudrate);
#endif
}

bool CPTZPL14::IrisClose()
{
	m_ucCommand[0] = 0x4E;
	m_ucCommand[1] = 0x00;
	m_ucCommand[2] = m_lDevAddress;
	m_ucCommand[3] = 0x00;
	m_ucCommand[4] = 0x11;
	m_ucCommand[5] = 0x00;
	m_ucCommand[6] = 0x80;
	m_ucCommand[7] = 0x80;
	m_ucCommand[8] = 0x80;
	m_ucCommand[9] =GetCheckSum();
#ifdef DDBUG
	DBUG_MSG("IrisClose:",m_ucCommand);
	return true;
#else
	return (CSerialPort::Instance())->WriteToPort(m_ucCommand,COMMANDLEN,m_Baudrate);
#endif
}
bool CPTZPL14::IrisOpen()
{
	m_ucCommand[0] = 0x4E;
	m_ucCommand[1] = 0x00;
	m_ucCommand[2] = m_lDevAddress;
	m_ucCommand[3] = 0x00;
	m_ucCommand[4] = 0x10;
	m_ucCommand[5] = 0x00;
	m_ucCommand[6] = 0x80;
	m_ucCommand[7] = 0x80;
	m_ucCommand[8] = 0x80;
	m_ucCommand[9] =GetCheckSum();
#ifdef DDBUG
	DBUG_MSG("IrisOpen:",m_ucCommand);
	return true;
#else
	return (CSerialPort::Instance())->WriteToPort(m_ucCommand,COMMANDLEN,m_Baudrate);
#endif
}

bool CPTZPL14::Up()
{
	m_ucCommand[0] = 0x4E;
	m_ucCommand[1] = 0x00;
	m_ucCommand[2] = m_lDevAddress;
	m_ucCommand[3] = 0x00;
	m_ucCommand[4] = 0x01;
	m_ucCommand[5] = 0x00;
	m_ucCommand[6] = 0x80;
	m_ucCommand[7] = 0xBF + GetSpeed();
	m_ucCommand[8] = 0x80;
	m_ucCommand[9] =GetCheckSum();
#ifdef DDBUG
	DBUG_MSG("Up:",m_ucCommand);
	return true;
#else
	return (CSerialPort::Instance())->WriteToPort(m_ucCommand,COMMANDLEN,m_Baudrate);
#endif
}

bool CPTZPL14::Down()
{
	m_ucCommand[0] = 0x4E;
	m_ucCommand[1] = 0x00;
	m_ucCommand[2] = m_lDevAddress;
	m_ucCommand[3] = 0x00;
	m_ucCommand[4] = 0x01;
	m_ucCommand[5] = 0x00;
	m_ucCommand[6] = 0x80;
	m_ucCommand[7] = 0x40 - GetSpeed();
	m_ucCommand[8] = 0x80;
	m_ucCommand[9] =GetCheckSum();
#ifdef DDBUG
	DBUG_MSG("Down:",m_ucCommand);
	return true;
#else
	return (CSerialPort::Instance())->WriteToPort(m_ucCommand,COMMANDLEN,m_Baudrate);
#endif
}

bool CPTZPL14::Right()
{
	m_ucCommand[0] = 0x4E;
	m_ucCommand[1] = 0x00;
	m_ucCommand[2] = m_lDevAddress;
	m_ucCommand[3] = 0x00;
	m_ucCommand[4] = 0x01;
	m_ucCommand[5] = 0x00;
	m_ucCommand[6] = 0xBF + GetSpeed();
	m_ucCommand[7] = 0x80;
	m_ucCommand[8] = 0x80;
	m_ucCommand[9] =GetCheckSum();
#ifdef DDBUG
	DBUG_MSG("Right:",m_ucCommand);
	return true;
#else
	return (CSerialPort::Instance())->WriteToPort(m_ucCommand,COMMANDLEN,m_Baudrate);
#endif
}

bool CPTZPL14::Left()
{
	m_ucCommand[0] = 0x4E;
	m_ucCommand[1] = 0x00;
	m_ucCommand[2] = m_lDevAddress;
	m_ucCommand[3] = 0x00;
	m_ucCommand[4] = 0x01;
	m_ucCommand[5] = 0x00;
	m_ucCommand[6] = 0x40 - GetSpeed();
	m_ucCommand[7] = 0x80;
	m_ucCommand[8] = 0x80;
	m_ucCommand[9] =GetCheckSum();
#ifdef DDBUG
	DBUG_MSG("Left:",m_ucCommand);
	return true;
#else
	return (CSerialPort::Instance())->WriteToPort(m_ucCommand,COMMANDLEN,m_Baudrate);
#endif
}

bool CPTZPL14::Stop()
{
	m_ucCommand[0] = 0x4E;
	m_ucCommand[1] = 0x00;
	m_ucCommand[2] = m_lDevAddress;
	m_ucCommand[3] = 0x00;
	m_ucCommand[4] = 0x01;
	m_ucCommand[5] = 0x00;
	m_ucCommand[6] = 0x80;
	m_ucCommand[7] = 0x80;
	m_ucCommand[8] = 0x80;
	m_ucCommand[9] =GetCheckSum();
#ifdef DDBUG
	DBUG_MSG("Stop:",m_ucCommand);
	return true;
#else
	//因为云台不灵，需要连发3次相同指令
	(CSerialPort::Instance())->WriteToPort(m_ucCommand,COMMANDLEN,m_Baudrate);
	(CSerialPort::Instance())->WriteToPort(m_ucCommand,COMMANDLEN,m_Baudrate);
	return (CSerialPort::Instance())->WriteToPort(m_ucCommand,COMMANDLEN,m_Baudrate);
#endif
}


BYTE CPTZPL14::GetSpeed()
{
	return (m_Speed * 2 - 1) * 4;
}

BYTE CPTZPL14::GetCheckSum()
{
	BYTE sum =0x00;
	
	for(int i = 0; i < 9; i++)
	{
		sum += m_ucCommand[i];
	}
	
	return sum;
}

bool CPTZPL14::PresetGo(BYTE ucPresetID)
{
	if (PL14_OPEN_MENU_PRESET_NUM == (ucPresetID + 1))
	{
		return PopPTZMenu();
	}
	else if (PL14_CLOSE_MENU_PRESET_NUM == (ucPresetID + 1))
	{
		return ClosePTZMenu();
	}
	else
	{
		m_ucCommand[0] = 0x4E;
		m_ucCommand[1] = 0x00;
		m_ucCommand[2] = m_lDevAddress;
		m_ucCommand[3] = 0x00;
		m_ucCommand[4] = 0x30;
		m_ucCommand[5] = ucPresetID + 1;
		m_ucCommand[6] = 0x80;
		m_ucCommand[7] = 0x80;
		m_ucCommand[8] = 0x80;
		m_ucCommand[9] =GetCheckSum();
#ifdef DDBUG
		DBUG_MSG("PresetGo:",m_ucCommand);
		return true;
#else
		return (CSerialPort::Instance())->WriteToPort(m_ucCommand,COMMANDLEN,m_Baudrate);
#endif
	}
}
bool CPTZPL14::PresetSet(BYTE ucPresetID)
{
	m_ucCommand[0] = 0x4E;
	m_ucCommand[1] = 0x00;
	m_ucCommand[2] = m_lDevAddress;
	m_ucCommand[3] = 0x00;
	m_ucCommand[4] = 0x31;
	m_ucCommand[5] = ucPresetID;
	m_ucCommand[6] = 0x80;
	m_ucCommand[7] = 0x80;
	m_ucCommand[8] = 0x80;
	m_ucCommand[9] =GetCheckSum();
#ifdef DDBUG
	DBUG_MSG("PresetSet:",m_ucCommand);
	return true;
#else
	return (CSerialPort::Instance())->WriteToPort(m_ucCommand,COMMANDLEN,m_Baudrate);
#endif
}
bool CPTZPL14::PresetClear(BYTE ucPresetID)
{
	m_ucCommand[0] = 0x4E;
	m_ucCommand[1] = 0x00;
	m_ucCommand[2] = m_lDevAddress;
	m_ucCommand[3] = 0x00;
	m_ucCommand[4] = 0x32;
	m_ucCommand[5] = ucPresetID;
	m_ucCommand[6] = 0x80;
	m_ucCommand[7] = 0x80;
	m_ucCommand[8] = 0x80;
	m_ucCommand[9] =GetCheckSum();
#ifdef DDBUG
	DBUG_MSG("PresetClear:",m_ucCommand);
	return true;
#else
	return (CSerialPort::Instance())->WriteToPort(m_ucCommand,COMMANDLEN,m_Baudrate);
#endif
}

//添加巡航功能


//添加云台轨迹和自动巡航功能
bool CPTZPL14::Record_Track()
{
	m_ucCommand[0] = 0x4E;
	m_ucCommand[1] = 0x00;
	m_ucCommand[2] = m_lDevAddress;
	m_ucCommand[3] = 0x00;
	m_ucCommand[4] = 0x39;
	m_ucCommand[5] = 0x01;
	m_ucCommand[6] = 0x80;
	m_ucCommand[7] = 0x80;
	m_ucCommand[8] = 0x80;
	m_ucCommand[9] =GetCheckSum();
#ifdef DDBUG
	DBUG_MSG("Record_Track:",m_ucCommand);
	return true;
#else
	return (CSerialPort::Instance())->WriteToPort(m_ucCommand,COMMANDLEN,m_Baudrate);
#endif
}
bool CPTZPL14::Stop_Record_Track()
{
	m_ucCommand[0] = 0x4E;
	m_ucCommand[1] = 0x00;
	m_ucCommand[2] = m_lDevAddress;
	m_ucCommand[3] = 0x00;
	m_ucCommand[4] = 0x3a;
	m_ucCommand[5] = 0x01;
	m_ucCommand[6] = 0x80;
	m_ucCommand[7] = 0x80;
	m_ucCommand[8] = 0x80;
	m_ucCommand[9] =GetCheckSum();
#ifdef DDBUG
	DBUG_MSG("Stop_Record_Track:",m_ucCommand);
	return true;
#else
	return (CSerialPort::Instance())->WriteToPort(m_ucCommand,COMMANDLEN,m_Baudrate);
#endif
}
bool CPTZPL14::Start_Track()
{
	m_ucCommand[0] = 0x4E;
	m_ucCommand[1] = 0x00;
	m_ucCommand[2] = m_lDevAddress;
	m_ucCommand[3] = 0x00;
	m_ucCommand[4] = 0x36;
	m_ucCommand[5] = 0x01;
	m_ucCommand[6] = 0x80;
	m_ucCommand[7] = 0x80;
	m_ucCommand[8] = 0x80;
	m_ucCommand[9] =GetCheckSum();
#ifdef DDBUG
	DBUG_MSG("Start_Track:",m_ucCommand);
	return true;
#else
	return (CSerialPort::Instance())->WriteToPort(m_ucCommand,COMMANDLEN,m_Baudrate);
#endif
}
bool CPTZPL14::Stop_Track()
{
	return Stop();
}
// 添加自动扫描功能
bool CPTZPL14::Auto_Scan()
{
	m_ucCommand[0] = 0x4E;
	m_ucCommand[1] = 0x00;
	m_ucCommand[2] = m_lDevAddress;
	m_ucCommand[3] = 0x00;
	m_ucCommand[4] = 0x3b;
	m_ucCommand[5] = 0x01;
	m_ucCommand[6] = 0x80;
	m_ucCommand[7] = 0x80;
	m_ucCommand[8] = 0x80;
	m_ucCommand[9] =GetCheckSum();
#ifdef DDBUG
	DBUG_MSG("Auto_Scan:",m_ucCommand);
	return true;
#else
	return (CSerialPort::Instance())->WriteToPort(m_ucCommand,COMMANDLEN,m_Baudrate);
#endif
}
bool CPTZPL14::Stop_Auto_Scan()
{
	return Stop();
}
//查询是否支持巡航
bool CPTZPL14::IsSupportCruise()
{
	return true;
}


bool CPTZPL14::LeftDown()
{
	m_ucCommand[0] = 0x4E;
	m_ucCommand[1] = 0x00;
	m_ucCommand[2] = m_lDevAddress;
	m_ucCommand[3] = 0x00;
	m_ucCommand[4] = 0x01;
	m_ucCommand[5] = 0x00;
	m_ucCommand[6] = 0x40 - GetSpeed();
	m_ucCommand[7] = 0x40 - GetSpeed();
	m_ucCommand[8] = 0x80;
	m_ucCommand[9] =GetCheckSum();
#ifdef DDBUG
	DBUG_MSG("Left:",m_ucCommand);
	return true;
#else
	return (CSerialPort::Instance())->WriteToPort(m_ucCommand,COMMANDLEN,m_Baudrate);
#endif
}

bool CPTZPL14::LeftUp()
{
	m_ucCommand[0] = 0x4E;
	m_ucCommand[1] = 0x00;
	m_ucCommand[2] = m_lDevAddress;
	m_ucCommand[3] = 0x00;
	m_ucCommand[4] = 0x01;
	m_ucCommand[5] = 0x00;
	m_ucCommand[6] = 0x40 - GetSpeed();
	m_ucCommand[7] = 0xBF + GetSpeed();
	m_ucCommand[8] = 0x80;
	m_ucCommand[9] =GetCheckSum();
#ifdef DDBUG
	DBUG_MSG("Left:",m_ucCommand);
	return true;
#else
	return (CSerialPort::Instance())->WriteToPort(m_ucCommand,COMMANDLEN,m_Baudrate);
#endif
}
bool CPTZPL14::RightDown()
{
	m_ucCommand[0] = 0x4E;
	m_ucCommand[1] = 0x00;
	m_ucCommand[2] = m_lDevAddress;
	m_ucCommand[3] = 0x00;
	m_ucCommand[4] = 0x01;
	m_ucCommand[5] = 0x00;
	m_ucCommand[6] = 0xBF + GetSpeed();
	m_ucCommand[7] = 0x40 - GetSpeed();
	m_ucCommand[8] = 0x80;
	m_ucCommand[9] =GetCheckSum();
#ifdef DDBUG
	DBUG_MSG("Left:",m_ucCommand);
	return true;
#else
	return (CSerialPort::Instance())->WriteToPort(m_ucCommand,COMMANDLEN,m_Baudrate);
#endif
}
bool CPTZPL14::RightUp()
{
	m_ucCommand[0] = 0x4E;
	m_ucCommand[1] = 0x00;
	m_ucCommand[2] = m_lDevAddress;
	m_ucCommand[3] = 0x00;
	m_ucCommand[4] = 0x01;
	m_ucCommand[5] = 0x00;
	m_ucCommand[6] = 0xBF + GetSpeed();
	m_ucCommand[7] = 0xBF + GetSpeed();
	m_ucCommand[8] = 0x80;
	m_ucCommand[9] =GetCheckSum();
#ifdef DDBUG
	DBUG_MSG("Left:",m_ucCommand);
	return true;
#else
	return (CSerialPort::Instance())->WriteToPort(m_ucCommand,COMMANDLEN,m_Baudrate);
#endif
}
bool CPTZPL14::RunCruise(BYTE uCruiseId)
{
	m_ucCommand[0] = 0x4E;
	m_ucCommand[1] = 0x00;
	m_ucCommand[2] = m_lDevAddress;
	m_ucCommand[3] = 0x00;
	m_ucCommand[4] = 0x33;
	m_ucCommand[5] = uCruiseId;
	m_ucCommand[6] = 0x80;
	m_ucCommand[7] = 0x80;
	m_ucCommand[8] = 0x80;
	m_ucCommand[9] =GetCheckSum();
#ifdef DDBUG
	DBUG_MSG("RunCruise:",m_ucCommand);
	return true;
#else
	return (CSerialPort::Instance())->WriteToPort(m_ucCommand,COMMANDLEN,m_Baudrate);
#endif
}
bool CPTZPL14::StopCruise()
{
	return Stop();
}

bool CPTZPL14::EnterCruiseMode(BYTE uCruiseId)
{
	return true;
}

bool CPTZPL14::ConfigCruise(BYTE ucPresetID, BYTE speed, BYTE waitTime)
{
	return true;
}

bool CPTZPL14::LeaveCruiseMode(BYTE uCruiseId)
{
	return true;
}

bool CPTZPL14::PopPTZMenu()
{
	m_ucCommand[0] = 0x4E;
	m_ucCommand[1] = 0x00;
	m_ucCommand[2] = m_lDevAddress;
	m_ucCommand[3] = 0x00;
	m_ucCommand[4] = 0x50;
	m_ucCommand[5] = 0x00;
	m_ucCommand[6] = 0x80;
	m_ucCommand[7] = 0x80;
	m_ucCommand[8] = 0x80;
	m_ucCommand[9] = GetCheckSum();
#ifdef DDBUG
	DBUG_MSG("Pop PTZ Menu:",m_ucCommand);
	return true;
#else
	return (CSerialPort::Instance())->WriteToPort(m_ucCommand,COMMANDLEN,m_Baudrate);
#endif
}


bool CPTZPL14::ClosePTZMenu()
{
	m_ucCommand[0] = 0x4E;
	m_ucCommand[1] = 0x00;
	m_ucCommand[2] = m_lDevAddress;
	m_ucCommand[3] = 0x00;
	m_ucCommand[4] = 0x51;
	m_ucCommand[5] = 0x00;
	m_ucCommand[6] = 0x80;
	m_ucCommand[7] = 0x80;
	m_ucCommand[8] = 0x80;
	m_ucCommand[9] = GetCheckSum();
#ifdef DDBUG
	DBUG_MSG("Pop PTZ Menu:",m_ucCommand);
	return true;
#else
	return (CSerialPort::Instance())->WriteToPort(m_ucCommand,COMMANDLEN,m_Baudrate);
#endif
}
