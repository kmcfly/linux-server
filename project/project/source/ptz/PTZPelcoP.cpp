
#include "PTZPelcoP.h"

CPTZPelcoP::CPTZPelcoP()
{
	m_Baudrate = SBR_9600;
}

CPTZPelcoP::~CPTZPelcoP()
{
}

bool CPTZPelcoP::Stop()
{
	m_ucCommand[0] = 0xA0;
	m_ucCommand[1] = m_lDevAddress-1;
	m_ucCommand[2] = 0;
	m_ucCommand[3] = 0;
	m_ucCommand[4] = 0;
	m_ucCommand[5] = 0;
	m_ucCommand[6] = 0xAF;
	m_ucCommand[7] = GetCheckSum();
	//printf("The PTZ stopped,FILE:%s, LINE:%d \n", __FILE__,__LINE__);
	return (CSerialPort::Instance())->WriteToPort(m_ucCommand,COMMANDLEN,m_Baudrate);
}

bool CPTZPelcoP::Left()
{
	m_ucCommand[0] = 0xA0;
	m_ucCommand[1] = m_lDevAddress-1;
	m_ucCommand[2] = 0;
	m_ucCommand[3] = 0x04;
	m_ucCommand[4] = GetPanSpeed();
	m_ucCommand[5] = 0;
	m_ucCommand[6] = 0xAF;
	m_ucCommand[7] = GetCheckSum();
	return (CSerialPort::Instance())->WriteToPort(m_ucCommand,COMMANDLEN,m_Baudrate);
}

bool CPTZPelcoP::Right()
{
	m_ucCommand[0] = 0xA0;
	m_ucCommand[1] = m_lDevAddress-1;
	m_ucCommand[2] = 0;
	m_ucCommand[3] = 0x02;
	m_ucCommand[4] = GetPanSpeed();
	m_ucCommand[5] = 0;
	m_ucCommand[6] = 0xAF;
	m_ucCommand[7] = GetCheckSum();
	return (CSerialPort::Instance())->WriteToPort(m_ucCommand,COMMANDLEN,m_Baudrate);
	
}

bool CPTZPelcoP::Up()
{
	m_ucCommand[0] = 0xA0;
	m_ucCommand[1] = m_lDevAddress-1;
	m_ucCommand[2] = 0;
	m_ucCommand[3] = 0x08;
	m_ucCommand[4] = 0;
	m_ucCommand[5] = GetTiltSpeed();
	m_ucCommand[6] = 0xAF;
	m_ucCommand[7] = GetCheckSum();
	return (CSerialPort::Instance())->WriteToPort(m_ucCommand,COMMANDLEN,m_Baudrate);
}

bool CPTZPelcoP::Down()
{
	m_ucCommand[0] = 0xA0;
	m_ucCommand[1] = m_lDevAddress-1;
	m_ucCommand[2] = 0;
	m_ucCommand[3] = 0x10;
	m_ucCommand[4] = 0;
	m_ucCommand[5] = GetTiltSpeed();
	m_ucCommand[6] = 0xAF;
	m_ucCommand[7] = GetCheckSum();
	return (CSerialPort::Instance())->WriteToPort(m_ucCommand,COMMANDLEN,m_Baudrate);
}

bool CPTZPelcoP::Near()
{
	m_ucCommand[0] = 0xA0;
	m_ucCommand[1] = m_lDevAddress-1;
	m_ucCommand[2] = 0x02;
	m_ucCommand[3] = 0;
	m_ucCommand[4] = 0;
	m_ucCommand[5] = 0;
	m_ucCommand[6] = 0xAF;
	m_ucCommand[7] = GetCheckSum();
	return (CSerialPort::Instance())->WriteToPort(m_ucCommand,COMMANDLEN,m_Baudrate);
}

bool CPTZPelcoP::Far()
{
	m_ucCommand[0] = 0xA0;
	m_ucCommand[1] = m_lDevAddress-1;
	m_ucCommand[2] = 0x01;
	m_ucCommand[3] = 0;
	m_ucCommand[4] = 0;
	m_ucCommand[5] = 0;
	m_ucCommand[6] = 0xAF;
	m_ucCommand[7] = GetCheckSum();
	return (CSerialPort::Instance())->WriteToPort(m_ucCommand,COMMANDLEN,m_Baudrate);
}

bool CPTZPelcoP::ZoomOut()
{
	m_ucCommand[0] = 0xA0;
	m_ucCommand[1] = m_lDevAddress-1;
	m_ucCommand[2] = 0;
	m_ucCommand[3] = 0x20;//0x40->0x20	<1>
	m_ucCommand[4] = 0;
	m_ucCommand[5] = 0;
	m_ucCommand[6] = 0xAF;
	m_ucCommand[7] = GetCheckSum();
	return (CSerialPort::Instance())->WriteToPort(m_ucCommand,COMMANDLEN,m_Baudrate);
}

bool CPTZPelcoP::ZoomIn()
{
	m_ucCommand[0] = 0xA0;
	m_ucCommand[1] = m_lDevAddress -1;
	m_ucCommand[2] = 0;
	m_ucCommand[3] = 0x40;	//0x20->0x40	<1>
	m_ucCommand[4] = 0;
	m_ucCommand[5] = 0;
	m_ucCommand[6] = 0xAF;
	m_ucCommand[7] = GetCheckSum();
	return (CSerialPort::Instance())->WriteToPort(m_ucCommand,COMMANDLEN,m_Baudrate);
}

bool CPTZPelcoP::IrisOpen()
{
	m_ucCommand[0] = 0xA0;
	m_ucCommand[1] = m_lDevAddress-1;
	m_ucCommand[2] = 0x04;
	m_ucCommand[3] = 0;
	m_ucCommand[4] = 0;
	m_ucCommand[5] = 0;
	m_ucCommand[6] = 0xAF;
	m_ucCommand[7] = GetCheckSum();
	return (CSerialPort::Instance())->WriteToPort(m_ucCommand,COMMANDLEN,m_Baudrate);
}

bool CPTZPelcoP::IrisClose()
{
	m_ucCommand[0] = 0xA0;
	m_ucCommand[1] = m_lDevAddress-1;
	m_ucCommand[2] = 0x08;
	m_ucCommand[3] = 0;
	m_ucCommand[4] = 0;
	m_ucCommand[5] = 0;
	m_ucCommand[6] = 0xAF;
	m_ucCommand[7] = GetCheckSum();
	return (CSerialPort::Instance())->WriteToPort(m_ucCommand,COMMANDLEN,m_Baudrate);
}

/*
 * hby 增加四个方向
 */
bool CPTZPelcoP::LeftDown()
{
	m_ucCommand[0] = 0xA0;
	m_ucCommand[1] = m_lDevAddress-1;
	m_ucCommand[2] = 0;
	m_ucCommand[3] = 0x14;
	m_ucCommand[4] = GetPanSpeed();
	m_ucCommand[5] = GetTiltSpeed();
	m_ucCommand[6] = 0xAF;
	m_ucCommand[7] = GetCheckSum();
	return (CSerialPort::Instance())->WriteToPort(m_ucCommand,COMMANDLEN,m_Baudrate);
}
bool CPTZPelcoP::LeftUp()
{
	m_ucCommand[0] = 0xA0;
	m_ucCommand[1] = m_lDevAddress-1;
	m_ucCommand[2] = 0;
	m_ucCommand[3] = 0x0C;
	m_ucCommand[4] = GetPanSpeed();
	m_ucCommand[5] = GetTiltSpeed();
	m_ucCommand[6] = 0xAF;
	m_ucCommand[7] = GetCheckSum();
	return (CSerialPort::Instance())->WriteToPort(m_ucCommand,COMMANDLEN,m_Baudrate);
}
bool CPTZPelcoP::RightDown()
{
	m_ucCommand[0] = 0xA0;
	m_ucCommand[1] = m_lDevAddress-1;
	m_ucCommand[2] = 0;
	m_ucCommand[3] = 0x12;
	m_ucCommand[4] = GetPanSpeed();
	m_ucCommand[5] = GetTiltSpeed();
	m_ucCommand[6] = 0xAF;
	m_ucCommand[7] = GetCheckSum();
	return (CSerialPort::Instance())->WriteToPort(m_ucCommand,COMMANDLEN,m_Baudrate);
}
bool CPTZPelcoP::RightUp()
{
	m_ucCommand[0] = 0xA0;
	m_ucCommand[1] = m_lDevAddress-1;
	m_ucCommand[2] = 0;
	m_ucCommand[3] = 0x0A;
	m_ucCommand[4] = GetPanSpeed();
	m_ucCommand[5] = GetTiltSpeed();
	m_ucCommand[6] = 0xAF;
	m_ucCommand[7] = GetCheckSum();
	return (CSerialPort::Instance())->WriteToPort(m_ucCommand,COMMANDLEN,m_Baudrate);
}

bool CPTZPelcoP::PresetGo(BYTE ucPresetID)
{

	m_ucCommand[0] = 0xA0;
	m_ucCommand[1] = m_lDevAddress-1;
	m_ucCommand[2] = 0;
	m_ucCommand[3] = 0x07;
	m_ucCommand[4] = 0;
	m_ucCommand[5] = ucPresetID+1;
	//printf("the preset value:%d\n", (int)m_ucCommand[5]);
	m_ucCommand[6] = 0xAF;
	m_ucCommand[7] = GetCheckSum();
	return (CSerialPort::Instance())->WriteToPort(m_ucCommand,COMMANDLEN,m_Baudrate);
}

bool CPTZPelcoP::PresetPrepare()
{
	m_ucCommand[0] = 0xA0;
	m_ucCommand[1] = m_lDevAddress-1;
	m_ucCommand[2] = 0;
	m_ucCommand[3] = 0x07;
	m_ucCommand[4] = 0;
	m_ucCommand[5] = 0x60;//??you must do like that
	m_ucCommand[6] = 0xAF;
	m_ucCommand[7] = GetCheckSum();
	return (CSerialPort::Instance())->WriteToPort(m_ucCommand,COMMANDLEN,m_Baudrate);
}

bool CPTZPelcoP::PresetSet(BYTE ucPresetID)
{
	//A0H	地址码	00H	03H	00H	预置点号	AFH	校验码
	m_ucCommand[0] = 0xA0;
	m_ucCommand[1] = m_lDevAddress-1;
	m_ucCommand[2] = 0;
	m_ucCommand[3] = 0x03;
	m_ucCommand[4] = 0;
	m_ucCommand[5] = ucPresetID+1;
	m_ucCommand[6] = 0xAF;
	m_ucCommand[7] = GetCheckSum();
	return (CSerialPort::Instance())->WriteToPort(m_ucCommand,COMMANDLEN,m_Baudrate);
}


BYTE CPTZPelcoP::GetCheckSum()
{
	BYTE CheckSum=0;
	for(int i=0;i<7;i++)
	{
		CheckSum ^= m_ucCommand[i];
	}
	return CheckSum;
}

BYTE CPTZPelcoP::GetPanSpeed()
{
	return m_Speed*8-1;//本来为0-63，这个结果为7-63(够用)
}

BYTE CPTZPelcoP::GetTiltSpeed()
{
	return m_Speed*8-1;//本来为0-63，这个结果为7-63(够用)
}

bool CPTZPelcoP::RunCruise(BYTE uCruiseId)
{
	//A0 01 00 4F 01 00 AF 40 
	//A0 01 00 4F 01 01 AF 41 
	//A0 01 00 4F 01 02 AF 42 
	m_ucCommand[0] = 0xA0;
	m_ucCommand[1] = m_lDevAddress-1;
	m_ucCommand[2] = 0;
	m_ucCommand[3] = 0x4F;
	m_ucCommand[4] = 1;
	m_ucCommand[5] = uCruiseId;
	m_ucCommand[6] = 0xAF;
	m_ucCommand[7] = GetCheckSum();
	return (CSerialPort::Instance())->WriteToPort(m_ucCommand,COMMANDLEN,m_Baudrate);
}

bool CPTZPelcoP::StopCruise()
{
	//A0 01 00 51 00 00 AF 5F 
	m_ucCommand[0] = 0xA0;
	m_ucCommand[1] = m_lDevAddress-1;
	m_ucCommand[2] = 0;
	m_ucCommand[3] = 0x51;
	m_ucCommand[4] = 0;
	m_ucCommand[5] = 0;
	m_ucCommand[6] = 0xAF;
	m_ucCommand[7] = GetCheckSum();
	return (CSerialPort::Instance())->WriteToPort(m_ucCommand,COMMANDLEN,m_Baudrate);
}

bool CPTZPelcoP::DeleteCruise(BYTE uCruiseId)
{
	//A0 01 00 4D 00 00 AF 43 
	//A0 01 00 4D 00 01 AF 42 
	m_ucCommand[0] = 0xA0;
	m_ucCommand[1] = m_lDevAddress-1;
	m_ucCommand[2] = 0;
	m_ucCommand[3] = 0x4D;
	m_ucCommand[4] = 0;
	m_ucCommand[5] = uCruiseId;
	m_ucCommand[6] = 0xAF;
	m_ucCommand[7] = GetCheckSum();
	return (CSerialPort::Instance())->WriteToPort(m_ucCommand,COMMANDLEN,m_Baudrate);
}

bool CPTZPelcoP::EnterCruiseMode(BYTE uCruiseId)
{
	//A0 01 00 47 00 00 AF 49 
	//A0 01 00 47 00 02 AF 4B
	m_ucCommand[0] = 0xA0;
	m_ucCommand[1] = m_lDevAddress-1;
	m_ucCommand[2] = 0;
	m_ucCommand[3] = 0x47;
	m_ucCommand[4] = 0;
	m_ucCommand[5] = uCruiseId;
	m_ucCommand[6] = 0xAF;
	m_ucCommand[7] = GetCheckSum();
	return (CSerialPort::Instance())->WriteToPort(m_ucCommand,COMMANDLEN,m_Baudrate);
}

bool CPTZPelcoP::ConfigCruise(BYTE ucPresetID, BYTE speed, BYTE waitTime)
{
	//A0 01 07 49 06 10 AF 56 
	//A0 01 10 49 00 20 AF 77 
	m_ucCommand[0] = 0xA0;
	m_ucCommand[1] = m_lDevAddress-1;
	if ( (0==ucPresetID) && (0==speed) && (0==waitTime)) 
	{
		m_ucCommand[2] = ucPresetID;
	}
	else
	{
		m_ucCommand[2] = ucPresetID+1;
	}

	m_ucCommand[3] = 0x49;
	m_ucCommand[4] = 9 - speed;
	m_ucCommand[5] = waitTime;
	m_ucCommand[6] = 0xAF;
	m_ucCommand[7] = GetCheckSum();
	return (CSerialPort::Instance())->WriteToPort(m_ucCommand,COMMANDLEN,m_Baudrate);
}

bool CPTZPelcoP::LeaveCruiseMode(BYTE ucPresetID)
{
	//A0 01 00 4B 00 00 AF 45
	m_ucCommand[0] = 0xA0;
	m_ucCommand[1] = m_lDevAddress-1;
	m_ucCommand[2] = 0;
	m_ucCommand[3] = 0x4B;
	m_ucCommand[4] = 0;
	m_ucCommand[5] = 0;
	m_ucCommand[6] = 0xAF;
	m_ucCommand[7] = GetCheckSum();
	return (CSerialPort::Instance())->WriteToPort(m_ucCommand,COMMANDLEN,m_Baudrate);
}

bool CPTZPelcoP::IsSupportCruise()
{
	printf("CPTZPelcoP support cruise\n");
	return true;
}

bool CPTZPelcoP::Record_Track()
{
	//A0 01 00 4B 00 00 AF 45
	//printf("Here record the track,FILE:%s,LINE:%d\n",__FILE__,__LINE__);
	m_ucCommand[0] = 0xA0;
	m_ucCommand[1] = m_lDevAddress-1;
	m_ucCommand[2] = 0;
	m_ucCommand[3] = 0x1f;
	m_ucCommand[4] = 0;
	m_ucCommand[5] = 0;
	m_ucCommand[6] = 0xAF;
	m_ucCommand[7] = GetCheckSum();
	return (CSerialPort::Instance())->WriteToPort(m_ucCommand,COMMANDLEN,m_Baudrate);


}

bool CPTZPelcoP::Stop_Record_Track()
{
	//printf("Here stop the track,FILE:%s,LINE:%d\n",__FILE__,__LINE__);
	//A0 01 00 21 00 00 AF 45
	m_ucCommand[0] = 0xA0;
	m_ucCommand[1] = m_lDevAddress-1;
	m_ucCommand[2] = 0;
	m_ucCommand[3] = 0x21;
	m_ucCommand[4] = 0;
	m_ucCommand[5] = 0;
	m_ucCommand[6] = 0xAF;
	m_ucCommand[7] = GetCheckSum();
	return (CSerialPort::Instance())->WriteToPort(m_ucCommand,COMMANDLEN,m_Baudrate);


}
bool CPTZPelcoP::Start_Track()
{
	//A0 01 00 23 00 00 AF 45
	m_ucCommand[0] = 0xA0;
	m_ucCommand[1] = m_lDevAddress-1;
	m_ucCommand[2] = 0;
	m_ucCommand[3] = 0x23;
	m_ucCommand[4] = 0;
	m_ucCommand[5] = 0;
	m_ucCommand[6] = 0xAF;
	m_ucCommand[7] = GetCheckSum();
	return (CSerialPort::Instance())->WriteToPort(m_ucCommand,COMMANDLEN,m_Baudrate);
	
}

bool CPTZPelcoP::Stop_Track()
{
	m_ucCommand[0] = 0xA0;
	m_ucCommand[1] = m_lDevAddress-1;
	m_ucCommand[2] = 0;
	m_ucCommand[3] = 0x04;					//使用向左来停止track
	m_ucCommand[4] = 0;
	m_ucCommand[5] = 0;
	m_ucCommand[6] = 0xAF;
	m_ucCommand[7] = GetCheckSum();	
	return (CSerialPort::Instance())->WriteToPort(m_ucCommand,COMMANDLEN,m_Baudrate);;
}


bool CPTZPelcoP::Auto_Scan()
{
	bool retval;
	
	//A0 01 00 07 00 97 AF 45   开始信号(都是AO)，解码器地址，命令byte1,命令byte2,数据byte1，数据byte2，结束传输标志，校验字节
	//自动扫描不可以用时，调用预置点99能实现自动扫描，调用预置点96可以实现停止扫描，PELCO-P和PELCO-D协议
	//99 Begin Auto-scan
	//98 Begin Frame Scan
	//97 Begin Random Scan
	//96 stop auto-scan
		m_ucCommand[0] = 0xA0;
		m_ucCommand[1] = m_lDevAddress-1;
		m_ucCommand[2] = 0;
		m_ucCommand[3] = 0x07;
		m_ucCommand[4] = 0;
		m_ucCommand[5] = 97;
		//printf("the preset value %d\n", (int)m_ucCommand[5]);
	
		m_ucCommand[6] = 0xAF;
		m_ucCommand[7] = GetCheckSum();
		retval = (CSerialPort::Instance())->WriteToPort(m_ucCommand,COMMANDLEN,m_Baudrate);
		//printf("Running= %d, FILE:%s,LINE:%d\n", Running, __FILE__, __LINE__);
		return retval;
}

bool CPTZPelcoP::Stop_Auto_Scan()
{
		bool retval;
	//printf("Here start the auto scan function,FILE:%s, LINE:%d\n", __FILE__, __LINE__);
	//A0 01 00 0x07 00 0x60 AF 45   开始信号(都是AO)，解码器地址，命令byte1,命令byte2,数据byte1，数据byte2，结束传输标志，校验字节
	//自动扫描不可以用时，调用预置点99能实现自动扫描，调用预置点96可以实现停止扫描，PELCO-P和PELCO-D协议

		m_ucCommand[0] = 0xA0;
		m_ucCommand[1] = m_lDevAddress-1;
		m_ucCommand[2] = 0;
		m_ucCommand[3] = 0x07;
		m_ucCommand[4] = 0;
		m_ucCommand[5] = 96;
		//printf("the preset value %d\n", (int)m_ucCommand[5]);
		m_ucCommand[6] = 0xAF;
		m_ucCommand[7] = GetCheckSum();
		retval = (CSerialPort::Instance())->WriteToPort(m_ucCommand,COMMANDLEN,m_Baudrate);

		return retval;
}



bool CPTZPelcoP::SetAuxiliary(BYTE auxNum)
{
	if ((AUXILIARY_START >= auxNum) && (AUXILIARY_END <= auxNum))
	{
		assert(false);
		return false;
	}

	m_ucCommand[0] = 0xA0;
	m_ucCommand[1] = m_lDevAddress-1;
	m_ucCommand[2] = 0;
	m_ucCommand[3] = 0x09;
	m_ucCommand[4] = 0;
	m_ucCommand[5] = auxNum;
	m_ucCommand[6] = 0xAF;
	m_ucCommand[7] = GetCheckSum();
	return (CSerialPort::Instance())->WriteToPort(m_ucCommand,COMMANDLEN,m_Baudrate);
}

bool CPTZPelcoP::ClearAuxiliary(BYTE auxNum)
{
	if ((AUXILIARY_START >= auxNum) && (AUXILIARY_END <= auxNum))
	{
		assert(false);
		return false;
	}

	m_ucCommand[0] = 0xA0;
	m_ucCommand[1] = m_lDevAddress-1;
	m_ucCommand[2] = 0;
	m_ucCommand[3] = 0x0B;
	m_ucCommand[4] = 0;
	m_ucCommand[5] = auxNum;
	m_ucCommand[6] = 0xAF;
	m_ucCommand[7] = GetCheckSum();
	return (CSerialPort::Instance())->WriteToPort(m_ucCommand,COMMANDLEN,m_Baudrate);
}
