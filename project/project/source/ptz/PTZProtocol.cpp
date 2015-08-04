#include "PTZProtocol.h"

CPTZProtocol::CPTZProtocol()
{
	m_dwLastState	= 0;
	m_lDevAddress	= 0;
	m_Baudrate		= SBR_115200;
	m_LastCmd=0;
	m_LastTime=0;
}

CPTZProtocol::~CPTZProtocol()
{
}

bool CPTZProtocol::SetCurisePoint(BYTE ptzaddr,int baudrate, BYTE uCruiseId,PTZ_CURISE_POINT *pCurisePoint,BYTE curisePointNum)
{
	bool ret =false;

	m_lDevAddress=ptzaddr;
	m_Baudrate=(PORT_BAUDRATE)baudrate;
	DeleteCruise(uCruiseId);

	ret = EnterCruiseMode(uCruiseId);
	for(int i=0;i<curisePointNum;i++)
	{
		//if(INVALID_PRESET != pCurisePoint[i].preset)
		if(0xff != pCurisePoint[i].preset)
			ConfigCruise(pCurisePoint[i].preset,pCurisePoint[i].dwellSpeed,pCurisePoint[i].dwellTime);
	}
	ret = LeaveCruiseMode(uCruiseId);

	return ret;
}
bool CPTZProtocol::Command(BYTE cmd,BYTE ptzaddr, int baudrate,BYTE speed,unsigned long  ptzPram,BYTE waitTime)
{
	bool ret =false;
	m_lDevAddress=ptzaddr;
	m_Speed=speed;
	m_Baudrate=(PORT_BAUDRATE)baudrate;

	switch(cmd)
	{
		case PTZ_CMD_STOP:
			ret = Stop();
			break;
		case PTZ_CMD_LEFT:
			ret = Left();
			break;
		case PTZ_CMD_RIGHT:
			ret = Right();						
			break;
		case PTZ_CMD_UP:
			ret = Up();
			break;
		case PTZ_CMD_DOWN:
			ret = Down();
			break;
		case PTZ_CMD_LEFT_UP:
			ret = LeftUp();
			break;
		case PTZ_CMD_LEFT_DOWN:
			ret = LeftDown();
			break;
		case PTZ_CMD_RIGHT_UP:
			ret = RightUp();
			break;
		case PTZ_CMD_RIGHT_DOWN:
			ret = RightDown();
			break;
		case PTZ_CMD_NEAR:
			ret = Near();
			break;
		case PTZ_CMD_FAR:
			ret = Far();
			break;
		case PTZ_CMD_ZOOM_OUT:
			ret = ZoomOut();
			break;
		case PTZ_CMD_ZOOM_IN:
			ret = ZoomIn();
			break;
		case PTZ_CMD_IRIS_OPEN:
			ret = IrisOpen();
			break;
		case PTZ_CMD_IRIS_CLOSE:
			ret = IrisClose();
			break;
		case PTZ_CMD_TRACK_SET:
			ret = Record_Track();
			break;
		case PTZ_CMD_TRACK_DEL:
			ret = Stop_Record_Track();
			break;
		case PTZ_CMD_TRACK_START:
			ret = Start_Track();
			break;
		case PTZ_CMD_TRACK_STOP:
			ret = Stop_Track();
			break;
		case PTZ_CMD_AUTO_SCAN_START:
			ret = Auto_Scan();
			break;
		case PTZ_CMD_AUTO_SCAN_STOP:
			ret = Stop_Auto_Scan();
			break;
		case PTZ_CMD_PRESET_SET:
			ret = PresetSet(ptzPram);
			break;
		case PTZ_CMD_PRESET_GO:
		{
			//????????????,??2?????????1
			if((cmd == 0) && (m_LastCmd ==0))
			{
				unsigned long curTime=GetTickCount();	
				unsigned long times=curTime-m_LastTime;
				if((times > 0) && (times < 3000))
				{
					//?????????????1, ???
				}
				else
				{
					ret = PresetGo( ptzPram );
					m_LastTime = GetTickCount();
				}
			}
			else
			{
				ret = PresetGo( ptzPram );
				m_LastTime = GetTickCount();
			}
			break;
		}
		case PTZ_CMD_ENTER_CURISE_MODE:
			ret = EnterCruiseMode( ptzPram );
			break;
		case PTZ_CMD_LEAVE_CURISE_MODE:
			ret = LeaveCruiseMode( ptzPram );
			break;
		case PTZ_CMD_CRUISE_SET:
			{
				PTZ_CURISE_POINT *pPTZCurisePoint = (PTZ_CURISE_POINT*)ptzPram;
				ret = ConfigCruise(pPTZCurisePoint->preset, pPTZCurisePoint->dwellSpeed,pPTZCurisePoint->dwellTime);
				break;
			}
		case PTZ_CMD_CRUISE_RUN:
			ret = RunCruise( ptzPram );
			break;
		case PTZ_CMD_CRUISE_STOP:
			ret = StopCruise();
			break;
		case PTZ_CMD_CRUISE_DEL:
			ret = DeleteCruise(ptzPram);
			break;
		case PTZ_CMD_POP_PTZ_MENU:
			ret = PopPTZMenu();
			break;

		case PTZ_CMD_SET_AUXILIARY:
			ret = SetAuxiliary((BYTE)ptzPram);
			break;
		case PTZ_CMD_CLEAR_AUXILIARY:
			ret = ClearAuxiliary((BYTE)ptzPram);
			break;
		default:
			break;
		
	}

	m_dwLastState = cmd;
	m_LastCmd=cmd;

	return ret;
}


bool CPTZProtocol::RunCruise(BYTE uCruiseId)
{
//	printf("This kind of ptz protocol do not support cruise!\n");
	return false;
}

bool CPTZProtocol::StopCruise()
{
//	printf("This kind of ptz protocol do not support cruise!\n");
	return false;
}

bool CPTZProtocol::DeleteCruise(BYTE uCruiseId)
{
//	printf("This kind of ptz protocol do not support cruise!\n");
	return false;
}

bool CPTZProtocol::EnterCruiseMode(BYTE uCruiseId)
{
//	printf("This kind of ptz protocol do not support cruise!\n");
	return false;
}

bool CPTZProtocol::ConfigCruise(BYTE ucPresetID, BYTE speed, BYTE waitTime)
{
//	printf("This kind of ptz protocol do not support cruise!\n");
	return false;
}

bool CPTZProtocol::LeaveCruiseMode(BYTE uCruiseId)
{
//	printf("This kind of ptz protocol do not support cruise!\n");
	return false;
}

bool CPTZProtocol::IsSupportCruise()
{
//	printf("CPTZProtocol default do not support cruise\n");
	return false;
}
bool CPTZProtocol::ConfigureCruiseDisable()
{
	//printf("If support cruise, CPTZProtocol default enable configure cruise!");
	return false;
}
bool CPTZProtocol::WriteBuffer(const BYTE* pBuffer, int len)
{
	return (CSerialPort::Instance())->WriteToPort(pBuffer, len, m_Baudrate);
}
bool CPTZProtocol::Record_Track()
{
	return false;
}

bool CPTZProtocol::Stop_Record_Track()
{
	return false;
}

bool CPTZProtocol::Start_Track()
{
	return false;
}

bool CPTZProtocol::Stop_Track()
{
	return false;
}

bool CPTZProtocol::Auto_Scan()
{
	return false;

}

bool CPTZProtocol::Stop_Auto_Scan()
{
	return false;
}

bool CPTZProtocol::PresetClear(BYTE ucPresetID)
{
	return false;
}

bool CPTZProtocol::PopPTZMenu()
{
	return false;
}


bool CPTZProtocol::SetAuxiliary(BYTE auxNum)
{
	return false;
}

bool CPTZProtocol::ClearAuxiliary(BYTE auxNum)
{
	return false;
}
