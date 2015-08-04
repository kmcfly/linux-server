#include "PTZVisca.h"

#ifdef	LJ_DEBUG
#include "user_config.h"
#endif
DWORD CPTZVisca::m_dwAddrAssigned = 0;//��̨��ַ�Զ������־,ÿ�������Ͽ��Է���һ��

CPTZVisca::CPTZVisca()
{

}

CPTZVisca::~CPTZVisca()
{

}

bool CPTZVisca::Stop()
{
	bool good = false;
	m_ucCommand[0] = 0x80 | m_lDevAddress;
	m_ucCommand[1] = 0x20;	//����0
	m_ucCommand[2] = 0xFF;
	good = (CSerialPort::Instance())->WriteToPort(m_ucCommand,3);

	m_ucCommand[0] = 0x80 | m_lDevAddress;
	m_ucCommand[1] = 0x21;	//����1
	m_ucCommand[2] = 0xFF;
	good &= (CSerialPort::Instance())->WriteToPort(m_ucCommand,3);

	//ֹͣ���һ������(�Ƿ��Ҫ��)
	switch(m_dwLastState)
	{
	case PTZ_CMD_LEFT:
	case PTZ_CMD_RIGHT:
	case PTZ_CMD_UP:
	case PTZ_CMD_DOWN:
		//8x 01 06 01 VV WW 03 03 FF  Pan-tiltDrive Stop
		m_ucCommand[0] = 0x80 | m_lDevAddress;
		m_ucCommand[1] = 0x01;
		m_ucCommand[2] = 0x06;
		m_ucCommand[3] = 0x01;
		m_ucCommand[4] = GetPanSpeed();
		m_ucCommand[5] = GetTiltSpeed();
		m_ucCommand[6] = 0x03;
		m_ucCommand[7] = 0x03;
		m_ucCommand[8] = 0xFF;
		good &= (CSerialPort::Instance())->WriteToPort(m_ucCommand,9);
		break;

	case PTZ_CMD_NEAR:
	case PTZ_CMD_FAR:
		//8x 01 04 08 00 FF CAM_Focus Stop
		m_ucCommand[0] = 0x80 | m_lDevAddress;
		m_ucCommand[1] = 0x01;
		m_ucCommand[2] = 0x04;
		m_ucCommand[3] = 0x08;
		m_ucCommand[4] = 0x00;
		m_ucCommand[5] = 0xFF;
		good &= (CSerialPort::Instance())->WriteToPort(m_ucCommand,6);
		break;

	case PTZ_CMD_ZOOM_OUT:
	case PTZ_CMD_ZOOM_IN:
		//8x 01 04 07 00 FF	CAM_Zoom Stop
		m_ucCommand[0] = 0x80 | m_lDevAddress;
		m_ucCommand[1] = 0x01;
		m_ucCommand[2] = 0x04;
		m_ucCommand[3] = 0x07;
		m_ucCommand[4] = 0x00;
		m_ucCommand[5] = 0xFF;
		good &= (CSerialPort::Instance())->WriteToPort(m_ucCommand,6);
		break;

	case PTZ_CMD_IRIS_OPEN:
	case PTZ_CMD_IRIS_CLOSE:
		break;
	default:
		break;
	}

	return good;
}

bool CPTZVisca::IrisClose()
{
	//8x 01 04 0B 03 FF	CAM_Iris Down
	m_ucCommand[0] = 0x80 | m_lDevAddress;
	m_ucCommand[1] = 0x01;
	m_ucCommand[2] = 0x04;
	m_ucCommand[3] = 0x0B;
	m_ucCommand[4] = 0x03;
	m_ucCommand[5] = 0xFF;
	return (CSerialPort::Instance())->WriteToPort(m_ucCommand,6);

}

bool CPTZVisca::IrisOpen()
{
	//8x 01 04 0B 02 FF	CAM_Iris Up
	m_ucCommand[0] = 0x80 | m_lDevAddress;
	m_ucCommand[1] = 0x01;
	m_ucCommand[2] = 0x04;
	m_ucCommand[3] = 0x0B;
	m_ucCommand[4] = 0x02;
	m_ucCommand[5] = 0xFF;
	return (CSerialPort::Instance())->WriteToPort(m_ucCommand,6);

}

bool CPTZVisca::ZoomOut()
{
	//8x 01 04 07 2Z FF	CAM_Zoom Tele
	m_ucCommand[0] = 0x80 | m_lDevAddress;
	m_ucCommand[1] = 0x01;
	m_ucCommand[2] = 0x04;
	m_ucCommand[3] = 0x07;
	m_ucCommand[4] = 0x20 | 4;//4Ϊzoom speed
	m_ucCommand[5] = 0xFF;
	return (CSerialPort::Instance())->WriteToPort(m_ucCommand,6);

}

bool CPTZVisca::ZoomIn()
{
	//8x 01 04 07 3Z FF	CAM_Zoom Wide
	m_ucCommand[0] = 0x80 | m_lDevAddress;
	m_ucCommand[1] = 0x01;
	m_ucCommand[2] = 0x04;
	m_ucCommand[3] = 0x07;
	m_ucCommand[4] = 0x30 | 4;
	m_ucCommand[5] = 0xFF;
	return (CSerialPort::Instance())->WriteToPort(m_ucCommand,6);

}

bool CPTZVisca::Far()
{
	//8x 01 04 08 02 FF	CAM_Focus Far
	m_ucCommand[0] = 0x80 | m_lDevAddress;
	m_ucCommand[1] = 0x01;
	m_ucCommand[2] = 0x04;
	m_ucCommand[3] = 0x08;
	m_ucCommand[4] = 0x02;
	m_ucCommand[5] = 0xFF;
	return (CSerialPort::Instance())->WriteToPort(m_ucCommand,6);

}

bool CPTZVisca::Near()
{
	//8x 01 04 08 03 FF	CAM_Focus Near
	m_ucCommand[0] = 0x80 | m_lDevAddress;
	m_ucCommand[1] = 0x01;
	m_ucCommand[2] = 0x04;
	m_ucCommand[3] = 0x08;
	m_ucCommand[4] = 0x03;
	m_ucCommand[5] = 0xFF;
	return (CSerialPort::Instance())->WriteToPort(m_ucCommand,6);

}

bool CPTZVisca::Down()
{
	//8x 01 06 01 VV WW 03 02 FF	Pan-tiltDrive Down
	m_ucCommand[0] = 0x80 | m_lDevAddress;
	m_ucCommand[1] = 0x01;
	m_ucCommand[2] = 0x06;
	m_ucCommand[3] = 0x01;
	m_ucCommand[4] = GetPanSpeed();
	m_ucCommand[5] = GetTiltSpeed();
	m_ucCommand[6] = 0x03;
	m_ucCommand[7] = 0x02;
	m_ucCommand[8] = 0xFF;

	return (CSerialPort::Instance())->WriteToPort(m_ucCommand,9);

}

bool CPTZVisca::Up()
{
	//8x 01 06 01 VV WW 03 01 FF	Pan-tiltDrive Up
	m_ucCommand[0] = 0x80 | m_lDevAddress;
	m_ucCommand[1] = 0x01;
	m_ucCommand[2] = 0x06;
	m_ucCommand[3] = 0x01;
	m_ucCommand[4] = GetPanSpeed();
	m_ucCommand[5] = GetTiltSpeed();
	m_ucCommand[6] = 0x03;
	m_ucCommand[7] = 0x01;
	m_ucCommand[8] = 0xFF;

	return (CSerialPort::Instance())->WriteToPort(m_ucCommand,9);

}

bool CPTZVisca::Right()
{
	//8x 01 06 01 VV WW 02 03 FF	Pan-tiltDrive Right
	m_ucCommand[0] = 0x80 | m_lDevAddress;
	m_ucCommand[1] = 0x01;
	m_ucCommand[2] = 0x06;
	m_ucCommand[3] = 0x01;
	m_ucCommand[4] = GetPanSpeed();
	m_ucCommand[5] = GetTiltSpeed();
	m_ucCommand[6] = 0x02;
	m_ucCommand[7] = 0x03;
	m_ucCommand[8] = 0xFF;

	return (CSerialPort::Instance())->WriteToPort(m_ucCommand,9);

}

bool CPTZVisca::Left()
{
	//8x 01 06 01 VV WW 01 03 FF	Pan-tiltDrive Left
	m_ucCommand[0] = 0x80 | m_lDevAddress;
	m_ucCommand[1] = 0x01;
	m_ucCommand[2] = 0x06;
	m_ucCommand[3] = 0x01;
	m_ucCommand[4] = GetPanSpeed();
	m_ucCommand[5] = GetTiltSpeed();
	m_ucCommand[6] = 0x01;
	m_ucCommand[7] = 0x03;
	m_ucCommand[8] = 0xFF;

	return (CSerialPort::Instance())->WriteToPort(m_ucCommand,9);
}

/*
 * hby �����ĸ�����
 */
bool CPTZVisca::LeftDown()
{
	return true;
}
bool CPTZVisca::LeftUp()
{
	return true;
}
bool CPTZVisca::RightDown()
{
	return true;
}
bool CPTZVisca::RightUp()
{
	return true;
}


bool CPTZVisca::PresetGo(BYTE ucPresetID)
{
	//����ֻ֧��6��Ԥ�õ�(0~5)
	if(ucPresetID > 5)
	{
		return false;
	}

	//8x 01 04 3F 02 0Z FF	CAM_Memory Recall
	m_ucCommand[0] = 0x80 | m_lDevAddress;
	m_ucCommand[1] = 0x01;
	m_ucCommand[2] = 0x04;
	m_ucCommand[3] = 0x3F;
	m_ucCommand[4] = 0x02;
	m_ucCommand[5] = ucPresetID;
	m_ucCommand[6] = 0xFF;

	return (CSerialPort::Instance())->WriteToPort(m_ucCommand,7);

}

bool CPTZVisca::PresetSet(BYTE ucPresetID)
{
	//����ֻ֧��6��Ԥ�õ�(0~5)
	if(ucPresetID > 5)
	{
		return false;
	}

	//8x 01 04 3F 01 0Z FF	CAM_Memory Set
	m_ucCommand[0] = 0x80 | m_lDevAddress;
	m_ucCommand[1] = 0x01;
	m_ucCommand[2] = 0x04;
	m_ucCommand[3] = 0x3F;
	m_ucCommand[4] = 0x01;
	m_ucCommand[5] = ucPresetID;
	m_ucCommand[6] = 0xFF;

	return (CSerialPort::Instance())->WriteToPort(m_ucCommand,7);

}

bool CPTZVisca::Initial(CSerialPort *pSPort,int devAddr)
{
	int		mask = 0;
	bool	ret = true;

	m_lDevAddress = devAddr;

//ffff	mask = 1 << (m_pSerialPort->GetPortno()-1);
	//�ô�����û�н�����̨��ַ���Զ�����
	if(!(m_dwAddrAssigned & mask))
	{
		AddrAssign();
		m_dwAddrAssigned |= mask;
	}

	return ret;
}


bool CPTZVisca::AddrAssign()
{
	//88 30 01 FF	AddressSet broadcast
	m_ucCommand[0] = 0x88;
	m_ucCommand[1] = 0x30;
	m_ucCommand[2] = 0x01;
	m_ucCommand[3] = 0xFF;

	return (CSerialPort::Instance())->WriteToPort(m_ucCommand,4);

}

BYTE CPTZVisca::GetTiltSpeed()
{
	return ((m_Speed+1)*3)/2;//1-14��Χ֮�ھͿ�����
}

BYTE CPTZVisca::GetPanSpeed()
{
	return m_Speed*2;//��1-18��Χ֮�ھͿ�����
}
