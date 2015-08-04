#include "PTZPelcoD.h"

#ifdef	LJ_DEBUG
#include "user_config.h"
#endif
CPTZPelcoD::CPTZPelcoD()
{	

}

CPTZPelcoD::~CPTZPelcoD()
{
}

bool CPTZPelcoD::Left()
{
	m_ucCommand[0] = 0xFF;
	m_ucCommand[1] = m_lDevAddress;
	m_ucCommand[2] = 0;
	m_ucCommand[3] = 0x04;
	m_ucCommand[4] = GetPanSpeed();
	m_ucCommand[5] = 0;
	m_ucCommand[6] = GetCheckSum();
	return (CSerialPort::Instance())->WriteToPort(m_ucCommand,COMMANDLEN,m_Baudrate);
}

BYTE CPTZPelcoD::GetCheckSum()
{
	BYTE CheckSum=0;
	for(int i = 1;i < 6;i++)
	{
		CheckSum += m_ucCommand[i];
	}
	return CheckSum;
}

bool CPTZPelcoD::Stop()
{
	m_ucCommand[0] = 0xFF;
	m_ucCommand[1] = m_lDevAddress;
	m_ucCommand[2] = 0;
	m_ucCommand[3] = 0;
	m_ucCommand[4] = 0;
	m_ucCommand[5] = 0;
	m_ucCommand[6] = GetCheckSum();
	return (CSerialPort::Instance())->WriteToPort(m_ucCommand,COMMANDLEN,m_Baudrate);
}

void CPTZPelcoD::AutoScan(int nGroupID)
{
	m_ucCommand[0] = 0xFF;
	m_ucCommand[1] = m_lDevAddress;
	m_ucCommand[2] = 0;
	m_ucCommand[3] = 0x07;	//根据协议0x10->0x07  <1>
	m_ucCommand[4] = 0;
	m_ucCommand[5] = 0x63;
	m_ucCommand[6] = GetCheckSum();
	(CSerialPort::Instance())->WriteToPort(m_ucCommand,COMMANDLEN,m_Baudrate);
}

bool CPTZPelcoD::Right()
{
	m_ucCommand[0] = 0xFF;
	m_ucCommand[1] = m_lDevAddress;
	m_ucCommand[2] = 0;
	m_ucCommand[3] = 0x02;
	m_ucCommand[4] = GetPanSpeed();
	m_ucCommand[5] = 0;
	m_ucCommand[6] = GetCheckSum();
	return (CSerialPort::Instance())->WriteToPort(m_ucCommand,COMMANDLEN,m_Baudrate);
}


bool CPTZPelcoD::Down()
{
	m_ucCommand[0] = 0xFF;
	m_ucCommand[1] = m_lDevAddress;
	m_ucCommand[2] = 0;
	m_ucCommand[3] = 0x10;
	m_ucCommand[4] = 0;
	m_ucCommand[5] = GetTiltSpeed();
	m_ucCommand[6] = GetCheckSum();
	return (CSerialPort::Instance())->WriteToPort(m_ucCommand,COMMANDLEN,m_Baudrate);
}


bool CPTZPelcoD::Up()
{
	m_ucCommand[0] = 0xFF;
	m_ucCommand[1] = m_lDevAddress;
	m_ucCommand[2] = 0;
	m_ucCommand[3] = 0x08;
	m_ucCommand[4] = 0;
	m_ucCommand[5] = GetTiltSpeed();
	m_ucCommand[6] = GetCheckSum();
	return (CSerialPort::Instance())->WriteToPort(m_ucCommand,COMMANDLEN,m_Baudrate);
}

bool CPTZPelcoD::IrisOpen()
{
	m_ucCommand[0] = 0xFF;
	m_ucCommand[1] = m_lDevAddress;
	m_ucCommand[2] = 0x02;
	m_ucCommand[3] = 0;
	m_ucCommand[4] = 0;
	m_ucCommand[5] = 0;
	m_ucCommand[6] = GetCheckSum();
	return (CSerialPort::Instance())->WriteToPort(m_ucCommand,COMMANDLEN,m_Baudrate);
}


bool CPTZPelcoD::IrisClose()
{
	m_ucCommand[0] = 0xFF;
	m_ucCommand[1] = m_lDevAddress;
	m_ucCommand[2] = 0x04;
	m_ucCommand[3] = 0;
	m_ucCommand[4] = 0;
	m_ucCommand[5] = 0;
	m_ucCommand[6] = GetCheckSum();
	return (CSerialPort::Instance())->WriteToPort(m_ucCommand,COMMANDLEN,m_Baudrate);
}

bool CPTZPelcoD::Far()
{
	m_ucCommand[0] = 0xFF;
	m_ucCommand[1] = m_lDevAddress;
	m_ucCommand[2] = 0x00;
	m_ucCommand[3] = 0x80;
	m_ucCommand[4] = 0;
	m_ucCommand[5] = 0;
	m_ucCommand[6] = GetCheckSum();
	return (CSerialPort::Instance())->WriteToPort(m_ucCommand,COMMANDLEN,m_Baudrate);
}

bool CPTZPelcoD::Near()
{
	m_ucCommand[0] = 0xFF;
	m_ucCommand[1] = m_lDevAddress;
	m_ucCommand[2] = 0x01;
	m_ucCommand[3] = 0;
	m_ucCommand[4] = 0;
	m_ucCommand[5] = 0;
	m_ucCommand[6] = GetCheckSum();
	return (CSerialPort::Instance())->WriteToPort(m_ucCommand,COMMANDLEN,m_Baudrate);
}

bool CPTZPelcoD::ZoomOut()
{
	m_ucCommand[0] = 0xFF;
	m_ucCommand[1] = m_lDevAddress;
	m_ucCommand[2] = 0;
	m_ucCommand[3] = 0x20;//<2>	0x40->0x20
	m_ucCommand[4] = 0;
	m_ucCommand[5] = 0;
	m_ucCommand[6] = GetCheckSum();
	return (CSerialPort::Instance())->WriteToPort(m_ucCommand,COMMANDLEN,m_Baudrate);
}


bool CPTZPelcoD::ZoomIn()
{
	m_ucCommand[0] = 0xFF;
	m_ucCommand[1] = m_lDevAddress;
	m_ucCommand[2] = 0;
	m_ucCommand[3] = 0x40;//<2>	0x20->0x40
	m_ucCommand[4] = 0;
	m_ucCommand[5] = 0;
	m_ucCommand[6] = GetCheckSum();
	return (CSerialPort::Instance())->WriteToPort(m_ucCommand,COMMANDLEN,m_Baudrate);
}

/*
 * hby 增加四个方向
 */
bool CPTZPelcoD::LeftDown()
{
	m_ucCommand[0] = 0xFF;
	m_ucCommand[1] = m_lDevAddress;
	m_ucCommand[2] = 0;
	m_ucCommand[3] = 0x14;
	m_ucCommand[4] = GetPanSpeed();
	m_ucCommand[5] = GetTiltSpeed();
	m_ucCommand[6] = GetCheckSum();
	
	return (CSerialPort::Instance())->WriteToPort(m_ucCommand,COMMANDLEN,m_Baudrate);
}
bool CPTZPelcoD::LeftUp()
{
	m_ucCommand[0] = 0xFF;
	m_ucCommand[1] = m_lDevAddress;
	m_ucCommand[2] = 0;
	m_ucCommand[3] = 0x0C;
	m_ucCommand[4] = GetPanSpeed();
	m_ucCommand[5] = GetTiltSpeed();
	m_ucCommand[6] = GetCheckSum();
	
	return (CSerialPort::Instance())->WriteToPort(m_ucCommand,COMMANDLEN,m_Baudrate);
}
bool CPTZPelcoD::RightDown()
{
	m_ucCommand[0] = 0xFF;
	m_ucCommand[1] = m_lDevAddress;
	m_ucCommand[2] = 0;
	m_ucCommand[3] = 0x12;
	m_ucCommand[4] = GetPanSpeed();
	m_ucCommand[5] = GetTiltSpeed();
	m_ucCommand[6] = GetCheckSum();
	
	return (CSerialPort::Instance())->WriteToPort(m_ucCommand,COMMANDLEN,m_Baudrate);
}
bool CPTZPelcoD::RightUp()
{
	m_ucCommand[0] = 0xFF;
	m_ucCommand[1] = m_lDevAddress;
	m_ucCommand[2] = 0;
	m_ucCommand[3] = 0x0A;
	m_ucCommand[4] = GetPanSpeed();
	m_ucCommand[5] = GetTiltSpeed();
	m_ucCommand[6] = GetCheckSum();

	return (CSerialPort::Instance())->WriteToPort(m_ucCommand,COMMANDLEN,m_Baudrate);
}


bool CPTZPelcoD::PresetGo(BYTE ucPresetID)
{
	m_ucCommand[0] = 0xFF;
	m_ucCommand[1] = m_lDevAddress;
	m_ucCommand[2] = 0;
	m_ucCommand[3] = 0x07;
	m_ucCommand[4] = 0;
	m_ucCommand[5] = ucPresetID+1;
	m_ucCommand[6] = GetCheckSum();
	return (CSerialPort::Instance())->WriteToPort(m_ucCommand,COMMANDLEN,m_Baudrate);
}

bool CPTZPelcoD::PresetSet(BYTE ucPresetID)
{
	m_ucCommand[0] = 0xFF;
	m_ucCommand[1] = m_lDevAddress;
	m_ucCommand[2] = 0;
	m_ucCommand[3] = 0x03;
	m_ucCommand[4] = 0;
	m_ucCommand[5] = ucPresetID+1;
	m_ucCommand[6] = GetCheckSum();
	return (CSerialPort::Instance())->WriteToPort(m_ucCommand,COMMANDLEN,m_Baudrate);
	
}

BYTE CPTZPelcoD::GetPanSpeed()
{
	return m_Speed*8-1;//本来为0-63,这个结果为7-63(够用)
}

BYTE CPTZPelcoD::GetTiltSpeed()
{
	return m_Speed*8-1;//本来为0-63,这个结果为7-63(够用)
}



bool CPTZPelcoD::RunCruise(BYTE uCruiseId)
{
	//FF 02 00 4F 01 00 52 
	//FF 02 00 4F 01 01 53 
	//FF 02 00 4F 01 02 54
	//FF 02 00 4F 01 03 55
	m_ucCommand[0] = 0xFF;
	m_ucCommand[1] = m_lDevAddress;
	m_ucCommand[2] = 0;
	m_ucCommand[3] = 0x4F;
	m_ucCommand[4] = 1;
	m_ucCommand[5] = uCruiseId;
	m_ucCommand[6] = GetCheckSum();
	return (CSerialPort::Instance())->WriteToPort(m_ucCommand,COMMANDLEN,m_Baudrate);
}

bool CPTZPelcoD::StopCruise()
{
	//FF 02 00 51 00 00 53
	m_ucCommand[0] = 0xFF;
	m_ucCommand[1] = m_lDevAddress;
	m_ucCommand[2] = 0;
	m_ucCommand[3] = 0x51;
	m_ucCommand[4] = 0;
	m_ucCommand[5] = 0;
	m_ucCommand[6] = GetCheckSum();
	return (CSerialPort::Instance())->WriteToPort(m_ucCommand,COMMANDLEN,m_Baudrate);	
}


bool CPTZPelcoD::DeleteCruise(BYTE uCruiseId)
{
	//FF 02 00 4D 00 00 4F 
	//FF 02 00 4D 00 01 50
	//FF 02 00 4D 00 02 51
	//FF 02 00 4D 00 03 52
	m_ucCommand[0] = 0xFF;
	m_ucCommand[1] = m_lDevAddress;
	m_ucCommand[2] = 0;
	m_ucCommand[3] = 0x4D;
	m_ucCommand[4] = 0;
	m_ucCommand[5] = uCruiseId;
	m_ucCommand[6] = GetCheckSum();
	return (CSerialPort::Instance())->WriteToPort(m_ucCommand,COMMANDLEN,m_Baudrate);
}

bool CPTZPelcoD::EnterCruiseMode(BYTE uCruiseId)
{
	//FF 02 00 47 00 00 49 
	//FF 02 00 47 00 01 4A 
	//FF 02 00 47 00 02 4B
	m_ucCommand[0] = 0xFF;
	m_ucCommand[1] = m_lDevAddress;
	m_ucCommand[2] = 0;
	m_ucCommand[3] = 0x47;
	m_ucCommand[4] = 0;
	m_ucCommand[5] = uCruiseId;
	m_ucCommand[6] = GetCheckSum();
	return (CSerialPort::Instance())->WriteToPort(m_ucCommand,COMMANDLEN,m_Baudrate);
}

bool CPTZPelcoD::ConfigCruise(BYTE ucPresetID, BYTE speed, BYTE waitTime)
{
	//FF 02 07 49 06 10 68 
	//FF 02 10 49 00 20 7B
	m_ucCommand[0] = 0xFF;
	m_ucCommand[1] = m_lDevAddress;
	if ( (0==ucPresetID) && (0==speed) && (0==waitTime) ) 
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
	m_ucCommand[6] = GetCheckSum();
	return (CSerialPort::Instance())->WriteToPort(m_ucCommand,COMMANDLEN,m_Baudrate);

}

bool CPTZPelcoD::LeaveCruiseMode(BYTE ucPresetID)
{
	//FF 02 00 4B 00 00 4D  
	//FF 02 00 4B 00 00 4D
	//FF 02 00 4B 00 00 4D  
	m_ucCommand[0] = 0xFF;
	m_ucCommand[1] = m_lDevAddress;
	m_ucCommand[2] = 0;
	m_ucCommand[3] = 0x4B;
	m_ucCommand[4] = 0;
	m_ucCommand[5] = 0;
	m_ucCommand[6] = GetCheckSum();
	return (CSerialPort::Instance())->WriteToPort(m_ucCommand,COMMANDLEN,m_Baudrate);
}

bool CPTZPelcoD::IsSupportCruise()
{
	printf("CPTZPelcoD support cruise\n");
	return true;
}

bool CPTZPelcoD::Record_Track()
{
	//ff 01 00 1f 00 00  45
	m_ucCommand[0] = 0xFF;
	m_ucCommand[1] = m_lDevAddress;
	m_ucCommand[2] = 0;
	m_ucCommand[3] = 0x1f;
	m_ucCommand[4] = 0;
	m_ucCommand[5] = 0;
	m_ucCommand[6] = GetCheckSum();

	return (CSerialPort::Instance())->WriteToPort(m_ucCommand,COMMANDLEN,m_Baudrate);


}

bool CPTZPelcoD::Stop_Record_Track()
{
	//ff 01 00 0x21 00 00 45
	m_ucCommand[0] = 0xFF;
	m_ucCommand[1] = m_lDevAddress;
	m_ucCommand[2] = 0;
	m_ucCommand[3] = 0x21;
	m_ucCommand[4] = 0;
	m_ucCommand[5] = 0;
	m_ucCommand[6] = GetCheckSum();

	return (CSerialPort::Instance())->WriteToPort(m_ucCommand,COMMANDLEN,m_Baudrate);


}

bool CPTZPelcoD::Start_Track()
{
	//FF 01 00 0x23 00 00 
	m_ucCommand[0] = 0xFF;
	m_ucCommand[1] = m_lDevAddress;
	m_ucCommand[2] = 0;
	m_ucCommand[3] = 0x23;
	m_ucCommand[4] = 0;
	m_ucCommand[5] = 0;
	m_ucCommand[6] = GetCheckSum();

	return (CSerialPort::Instance())->WriteToPort(m_ucCommand,COMMANDLEN,m_Baudrate);

}

bool CPTZPelcoD::Stop_Track()
{
	//FF 01 00 0x23 00 00 
	m_ucCommand[0] = 0xFF;
	m_ucCommand[1] = m_lDevAddress;
	m_ucCommand[2] = 0;
	m_ucCommand[3] = 0x04;				//使用向左命令来停止轨迹
	m_ucCommand[4] = 0;
	m_ucCommand[5] = 0;
	m_ucCommand[6] = GetCheckSum();
	
	return (CSerialPort::Instance())->WriteToPort(m_ucCommand,COMMANDLEN,m_Baudrate);

}

bool CPTZPelcoD::Auto_Scan()
{
		//ff 01 00 0x07 00 97 45   开始信号(都是AO),解码器地址,命令byte1,命令byte2,数据byte1,数据byte2,校验字节
		//自动扫描不可以用时,调用预置点99能实现自动扫描,调用预置点96可以实现停止扫描,PELCO-P和PELCO-D协议
		//99 Begin Auto-scan
		//98 Begin Frame Scan
		//97 Begin Random Scan
		//96 stop auto-scan
	
		m_ucCommand[0] = 0xFF;
		m_ucCommand[1] = m_lDevAddress;
		m_ucCommand[2] = 0;
		m_ucCommand[3] = 0x07;
		m_ucCommand[4] = 0;
		m_ucCommand[5] = 97;
		m_ucCommand[6] = GetCheckSum();
	
		return (CSerialPort::Instance())->WriteToPort(m_ucCommand,COMMANDLEN,m_Baudrate);
}

bool CPTZPelcoD::Stop_Auto_Scan()
{
		//printf("Here stop the auto scan function,FILE:%s, LINE:%d\n", __FILE__, __LINE__);
		//ff 01 00 0x07 00 00 45   开始信号(都是AO),解码器地址,命令byte1,命令byte2,数据byte1,数据byte2,校验字节
		//自动扫描不可以用时,调用预置点99能实现自动扫描,调用预置点96可以实现停止扫描,PELCO-P和PELCO-D协议

		m_ucCommand[0] = 0xFF;
		m_ucCommand[1] = m_lDevAddress;
		m_ucCommand[2] = 0;
		m_ucCommand[3] = 0x07;
		m_ucCommand[4] = 0;
		m_ucCommand[5] = 96;
		m_ucCommand[6] = GetCheckSum();
	
		return (CSerialPort::Instance())->WriteToPort(m_ucCommand,COMMANDLEN,m_Baudrate);
}

//打开辅助开关.开关号1-8
//1
//2 雨刷
//3
//4
//5
//6
//7
//8

bool CPTZPelcoD::SetAuxiliary(BYTE auxNum)
{
	if ((AUXILIARY_START >= auxNum) && (AUXILIARY_END <= auxNum))
	{
		assert(false);
		return false;
	}

	m_ucCommand[0] = 0xFF;
	m_ucCommand[1] = m_lDevAddress;
	m_ucCommand[2] = 0;
	m_ucCommand[3] = 0x09;
	m_ucCommand[4] = 0;
	m_ucCommand[5] = auxNum;
	m_ucCommand[6] = GetCheckSum();
	return (CSerialPort::Instance())->WriteToPort(m_ucCommand,COMMANDLEN,m_Baudrate);
}

bool CPTZPelcoD::ClearAuxiliary(BYTE auxNum)
{
	if ((AUXILIARY_START >= auxNum) && (AUXILIARY_END <= auxNum))
	{
		assert(false);
		return false;
	}

	m_ucCommand[0] = 0xFF;
	m_ucCommand[1] = m_lDevAddress;
	m_ucCommand[2] = 0;
	m_ucCommand[3] = 0x0B;
	m_ucCommand[4] = 0;
	m_ucCommand[5] = auxNum;
	m_ucCommand[6] = GetCheckSum();

	printf("%s:%s:%d, ptz addr = %d, clear auxNum = %d\n", __FUNCTION__, __FILE__, __LINE__, m_lDevAddress, auxNum);
	return (CSerialPort::Instance())->WriteToPort(m_ucCommand,COMMANDLEN,m_Baudrate);
}
