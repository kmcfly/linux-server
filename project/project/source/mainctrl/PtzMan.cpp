
#include "PtzMan.h"

#ifndef __ENVIRONMENT_WIN32__
#include "PTZNULL.h"
#include "PTZNeon.h"
#include "PTZDscp.h"
#include "PTZLilin.h"
#include "PTZMinking.h"
#include "PTZPelcoD.h"
#include "PTZPelcoP.h"
#include "PTZVisca.h"
#include "PTZSamsung.h"
#include "PTZRm110.h"
#include "PTZHy.h"
#include "PTZPL14.h"
#if defined(__PTZ_GE__)
#include "PTZKtd348.h"
#endif
#include "PTZTvi.h"
#include "PtzAux.h"
#endif

//////////////////////////////////////////////////////////////////////////

const unsigned long PTZ_ERR_OK_ULONG = static_cast<unsigned long>(PTZ_ERR_OK);

//////////////////////////////////////////////////////////////////////////
// ���졢��ʼ�����˳�
//////////////////////////////////////////////////////////////////////////

CPtzMan* CPtzMan::Instance()
{
	static CPtzMan s_ptzMan;
	return &s_ptzMan;
}

CPtzMan::CPtzMan() : m_bInit(false)
{

}

CPtzMan::~CPtzMan()
{

}

bool CPtzMan::Initial()
{
	assert(m_ptzChnnState.size() == 0 );
	assert(m_cfgSerialInfoMap.size() == 0);
	assert(m_cfgPresetMap.size() == 0);
	assert(m_protocolMap.size() == 0);

	m_lock.Lock();
	m_bInit = true;
	m_idPtzCtrl = INVALID_CLIENT_ID;
	m_lock.UnLock();

	return true;
}

void CPtzMan::Quit()
{
	m_ptzChnnState.clear();
	m_cfgSerialInfoMap.clear();

	for ( PtzPresetMap::iterator iter = m_cfgPresetMap.begin(); iter != m_cfgPresetMap.end(); iter++ )
	{
		if( iter->second)
		{
			delete iter->second;
			iter->second = NULL;
		}
	}
	m_cfgPresetMap.clear();

	for ( PtzCuriseMap::iterator iter = m_cfgCuriseMap.begin(); iter != m_cfgCuriseMap.end(); iter++ )
	{
		if( iter->second )
		{
			delete iter->second;
			iter->second = NULL;
		}
	}
	m_cfgCuriseMap.clear();

	for ( PtzProtocolMap::iterator iter = m_protocolMap.begin(); iter != m_protocolMap.end(); iter++ )
	{
		if( iter->second )
		{
			delete iter->second;
			iter->second = NULL;
		}
	}
	m_protocolMap.clear();

	m_lock.Lock();
	m_bInit = false;
	m_idPtzCtrl = INVALID_CLIENT_ID;
	m_lock.UnLock();
}

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////

/*
	Action������������˵��:
	������̨:
	ɾ��Ѳ����:			Action(ͨ����, PTZCMD_CRUISE_DEL,	Ѳ����ID,			��Ч,  ��Ч);

	����Ѳ����:			Action(ͨ����, PTZCMD_CRUISE_RUN,	Ѳ����ID,			��Ч,  ��Ч);

	ֹͣѲ��:				Action(ͨ����, PTZCMD_CRUISE_STOP,	Ѳ����ID,		��Ч,  ��Ч);

	����ĳԤ�õ�:		Action(ͨ����, PTZCMD_PRESETGO,	Ԥ�õ�ID,			��Ч,  ��Ч);

	����Ԥ�õ�:			Action(ͨ����, PTZCMD_PRESETSET,	Ԥ�õ�ID,			��Ч,  ��Ч);

	����Ѳ����:			SetCurisePoint(ͨ����, Ѳ����ID, Ѳ����ϵ��,Ѳ��������);

	��¼�켣��			Action(ͨ���ţ�PTZCMD_MOVE,		PTZCOMMAND_TRACK_RECORD,�ٶ�);
	ֹͣ��¼�켣��	Action(ͨ����, PTZCMD_MOVE,			PTZCOMMAND_TRACK_STOP_RECORD);

	��ʼ�켣��			Action(ͨ����, PTZCMD_MOVE,			PTZCOMMAND_TRACK_RUN,�ٶ�)��
	ֹͣ�켣��			Action(ͨ����, PTZCMD_MOVE,			PTZCOMMAND_TRACK_STOP_RUN);
	�Զ�ɨ�裺			Action(ͨ����, PTZCMD_PRESETGO,			0X99);
	ֹͣ�Զ�ɨ�裺	Action(ͨ���ţ�PTZCMD_PRESETGO,			0X96);
*/
unsigned long CPtzMan::Action(unsigned long channel, unsigned char cmdtype, unsigned long ptzParam, unsigned char speed, unsigned char waiteTime, const unsigned char* ptrParamEx)
{
	/*
		ִ�����в������
		��������ʱδ�õ�
	*/
	unsigned long ret = 0;
	bool bProcessed = false;

	bool bEnd = false;
	unsigned long type = GetCmdType(cmdtype, ptzParam, bEnd);			//��̨����һ��״̬
	unsigned long state = GetPtzState(channel);				//��̨�ĵ�ǰ״̬
	bool bLock = state & static_cast<unsigned long>(PTZ_ST_LOCK);

	if( bLock )
	{
		ret = PTZ_ERR_CHNN_LOCK;
	}
	else
	{
		//�Ƿ�����ͨ�� && ��̨���ڳ�ʼ״̬
		if(GetPtzEnable(channel))
		{
			// * �����������������͵�����
			ret = PTZ_ERR_CHNN_UN_INTERRUPT;

			bool bSetTrack = false;

			if(static_cast<unsigned char>(PTZ_CMD_RESET) == cmdtype)
			{
				if(ResetPtz(channel, state))
				{
					ret = 0;
				}
			}
			else
			{
				//��ˢ�͵ƹ�ȵĸ������ز�����Ӧ���رյ�ǰ���ڽ��е���̨������
				//��������Ҳ����ֹͣ��ǰ����ˢ�͵ƹ�״̬
				if(((PTZ_CMD_SET_AUXILIARY == cmdtype) || (PTZ_CMD_CLEAR_AUXILIARY == cmdtype)) || ResetPtz(channel, state))
				{
					// * ����Ѳ����
					if( !bProcessed && static_cast<unsigned long>(PTZ_CMD_CRUISE_CFG) == cmdtype )
					{
						// [Ѳ���߱��unsigned long��Ѳ���ߵ����unsigned long��Ѳ���ߵ���ϢPTZ_CURISE_POINT*]
						unsigned long* pValue = reinterpret_cast<unsigned long*>((unsigned char*)ptrParamEx);
						unsigned long index = *pValue++;
						assert(index<PTZ_MAX_CURISE_NUM);
						unsigned long count = *pValue++;
						assert(count<=PTZ_MAX_CURISE_PT_NUM);

						if( count > 0 )
						{
							//����Ѳ����
							PTZ_CURISE_POINT* pCurisePt = reinterpret_cast<PTZ_CURISE_POINT*>(pValue);
							unsigned long retCurise = ProcessAction(channel, PTZ_CMD_ENTER_CURISE_MODE, index);
							if( 0 == retCurise )
							{
								for ( int i = 0; i < count; i++ )
								{
									retCurise = ProcessAction(channel, PTZ_CMD_CRUISE_SET, (unsigned long)pCurisePt);
									pCurisePt++;
								}
								retCurise = ProcessAction(channel, PTZ_CMD_LEAVE_CURISE_MODE, index);
							}
							ret = retCurise;
						}
						else
						{
							//ɾ��Ѳ����
							ret = ProcessAction(channel, PTZ_CMD_CRUISE_DEL, index);
						}

						bEnd = true;
						bProcessed = true;
					}

					// * �ա���ͨ
					if(!bProcessed)
					{
						if((PTZ_CMD_PRESET_GO != cmdtype) || ((PTZ_CMD_PRESET_GO == cmdtype) && PresetIsEnable(channel, ptzParam)))
						{
							//����Ԥ�õ����ʱֱ��ִ�У���Ԥ�õ����ʱ�鿴��Ԥ�õ��Ƿ�ɱ�ִ��
							ret =  ProcessAction(channel, cmdtype, ptzParam, speed, waiteTime, (unsigned char*)ptrParamEx);
							bProcessed = true;
						}
						else if( !PresetIsEnable( channel, ptzParam ) )
						{
							ret = PTZ_ERR_PRESET_DISABLE;
							bProcessed = true;
						}
					}
				}
				else
				{
					// * ���ù켣[���ù켣�п�������ͨ����]
					bSetTrack = (static_cast<unsigned long>(PTZ_ST_CFG_TRACK) & state) &&
								((static_cast<unsigned long>(PTZ_ST_NULL) == type) || (static_cast<unsigned long>(PTZ_ST_NORMAL) == type)||
								 (static_cast<unsigned long>(PTZ_ST_BRUSH_ON) == type) || (static_cast<unsigned long>(PTZ_ST_LAMP_ON) == type));
					if( (static_cast<unsigned long>(PTZ_ST_CFG_TRACK) & state) && ((state & type) || bSetTrack) )
					{
						ret =  ProcessAction(channel, cmdtype, ptzParam, speed, waiteTime, (unsigned char*)ptrParamEx);
					}
				}
			}

			if( ( 0 == ret ) && !bSetTrack  )
			{
				//����״̬
				unsigned long state_now = static_cast<unsigned long>(PTZ_ST_NULL);

				if( !bEnd )
				{
					state_now |= type;
					state_now |= GetPtzState(channel);
				}
				else
				{
					state_now |= GetPtzState(channel);
					state_now &= ~type;
				}
				
				SetPtzState(channel, state_now);
			}
		}
		else
		{
			ret = PTZ_ERR_CHNN_UN_ENABLE;
		}
	}

	return ret;
}

void CPtzMan::SetSerialInfo(unsigned long channel, unsigned char enable, unsigned char addr, unsigned char protocol, unsigned char rate, unsigned long speed)
{
	PtzSerialInfo serial;
	memset(&serial, 0, sizeof(PtzSerialInfo));
	serial.last_cmd_type = 0xFF;

	serial.channel = channel;
	serial.enable = enable;
	serial.addr = addr;
	serial.protocol = protocol;
	serial.baud_rate = rate;
	serial.speed = speed;

	PtzSerialInfoMap::iterator iter = m_cfgSerialInfoMap.find(serial.channel);
	if( iter != m_cfgSerialInfoMap.end() )
	{
		serial.last_cmd_type = (iter->second).last_cmd_type;
		serial.last_process_time = (iter->second).last_process_time;
		memcpy(&(iter->second), &serial, sizeof(PtzSerialInfo));
	}
	else
	{
		m_cfgSerialInfoMap.insert( std::make_pair(serial.channel, serial) );
	}
#ifdef __ENVIRONMENT_LINUX__
	CFakeCurise::Instance()->SetSerialInfo(channel, enable, addr, protocol, rate);
#endif
}

void CPtzMan::SetPresetConfig(unsigned long channel, const PTZ_PRESET* pPreset, unsigned long count)
{
	assert(count <= PTZ_MAX_PRESET_NUM);
	assert(pPreset);
	
	//����Ƿ�������Ԥ�õ����
	unsigned long realCount = count;
	if( count > PTZ_MAX_PRESET_NUM )
	{
		realCount = PTZ_MAX_PRESET_NUM;
		printf("(%s) overflow max preset num.set count=%d.max count=%d.real count=%d.\n", __FUNCTION__, count, PTZ_MAX_PRESET_NUM, realCount);
		assert(false);
	}

	//������û���ù���ͨ��,�����޸�;û���������
	PtzPresetMap::iterator iter = m_cfgPresetMap.find(channel);
	if( iter != m_cfgPresetMap.end() )
	{
		assert(channel == iter->second->channel);
		iter->second->channel = channel;
		iter->second->count = realCount;
		memcpy(iter->second->preset, pPreset, sizeof(PTZ_PRESET) * realCount);
	}
	else
	{
		PtzPreset* pNewPreset = new PtzPreset;
		memset(pNewPreset, 0, sizeof(PtzPreset));

		pNewPreset->channel = channel;
		pNewPreset->count = realCount;
		memcpy(pNewPreset->preset, pPreset, sizeof(PTZ_PRESET) * realCount);

		m_cfgPresetMap.insert( std::make_pair(channel, pNewPreset) );
	}
}

unsigned long CPtzMan::GetPresetConfig(unsigned long channel, PTZ_PRESET** pOutPreset, unsigned long& outCount)
{
	PtzPresetMap::iterator iter = m_cfgPresetMap.find(channel);
	if( iter != m_cfgPresetMap.end() )
	{
		*pOutPreset = iter->second->preset;
		outCount = iter->second->count;

		return PTZ_ERR_OK;
	}
	else
	{
		*pOutPreset = NULL;
		outCount = 0;

		return PTZ_ERR_CHNN_UN_SET;
	}
}

bool CPtzMan::PresetIsEnable( unsigned long chnn, unsigned int presetNum )
{
	PTZ_PRESET *pPreset = NULL;
	unsigned long  presetCount;

	if( PTZ_ERR_OK == GetPresetConfig( chnn, &pPreset, presetCount ) )
	{
		assert( NULL != pPreset );
		assert( 0 < presetCount );
		if( presetCount > presetNum )
		{
			return pPreset[presetNum].enable;
		}
		else
		{
			return false;
		}

	}
	else
	{
		return false;
	}



}

void CPtzMan::SetCruiseConfig(unsigned long channel, unsigned long index, const PTZ_CURISE* pCurise, const PTZ_CURISE_POINT* pCurisePt, unsigned long count)
{
	assert(count <= PTZ_MAX_CURISE_PT_NUM);
	assert(index < PTZ_MAX_CURISE_NUM);

	//����Ƿ�������Ѳ���ߵ����
	unsigned long realCount = count;
	if( count > PTZ_MAX_CURISE_PT_NUM )
	{
		realCount = PTZ_MAX_CURISE_PT_NUM;
		printf("(%s) overflow max cruise point.set count=%d.max count=%d.real count=%d.\n", __FUNCTION__, count, PTZ_MAX_CURISE_PT_NUM, realCount);
		assert(false);
	}

	//������Ѳ��������,
	unsigned long realIndex = index;
	if( index >= PTZ_MAX_CURISE_NUM )
	{
		realIndex = PTZ_MAX_CURISE_NUM - 1;
		printf("(%s) overflow max curise num.index=%d.real index=%d.\n", __FUNCTION__, index, realIndex);
		assert(false);
	}

	PtzCuriseMap::iterator iter = m_cfgCuriseMap.find(channel);
	if( iter != m_cfgCuriseMap.end() )
	{
		assert(channel == iter->second->channel);
		iter->second->channel = channel;

		if( pCurise && pCurisePt )
		{
			if(realIndex +1 > iter->second->count )
			{
				iter->second->count = realIndex +1;
			}

			PtzOneCurise& curise = iter->second->curise[realIndex];
			curise.symbol = 0x7;
			memcpy(&curise.header, pCurise, sizeof(PTZ_CURISE));
			assert(curise.header.index == realIndex);
			curise.header.index = realIndex;
			memcpy(curise.curise_pt, pCurisePt, sizeof(PTZ_CURISE_POINT) * realCount);
			curise.count = realCount;
		}
		else
		{
			PtzOneCurise& curise = iter->second->curise[realIndex];
			assert(curise.header.index == realIndex);
			curise.symbol = 0;
			curise.header.index = realIndex;
			curise.count = 0;
		}
	}
	else
	{
		PtzCurise* pNewCurise = new PtzCurise;
		memset(pNewCurise, 0, sizeof(PtzCurise));

		pNewCurise->channel = channel;

		if( pCurise && pCurisePt )
		{
			if(realIndex +1 > pNewCurise->count )
			{
				pNewCurise->count = realIndex +1;
			}

			PtzOneCurise& curise = pNewCurise->curise[realIndex];
			curise.symbol = 0x7;
			memcpy(&curise.header, pCurise, sizeof(PTZ_CURISE));
			assert(curise.header.index == realIndex);
			curise.header.index = realIndex;
			memcpy(curise.curise_pt, pCurisePt, sizeof(PTZ_CURISE_POINT) * realCount);
			curise.count = realCount;
		}

		m_cfgCuriseMap.insert( std::make_pair(channel, pNewCurise) );
	}
}

unsigned long CPtzMan::GetCruiseConfig(unsigned long channel, unsigned long index, PTZ_CURISE** pOutCurise, PTZ_CURISE_POINT** pOutCurisePt, unsigned long& outCount)
{
	assert(index < PTZ_MAX_CURISE_NUM);

	PtzCuriseMap::iterator iter = m_cfgCuriseMap.find(channel);
	if( iter != m_cfgCuriseMap.end() &&
		index < iter->second->count &&
		(iter->second->curise[index].symbol & 0x1) )
	{
		*pOutCurise = &(iter->second->curise[index].header);
		*pOutCurisePt = iter->second->curise[index].curise_pt;
		outCount = iter->second->curise[index].count;
		return PTZ_ERR_OK;
	}
	else
	{
		*pOutCurise = NULL;
		*pOutCurisePt = NULL;
		outCount = 0;
		return PTZ_ERR_CHNN_UN_SET;
	}
}

bool CPtzMan::GetPtzEnable(unsigned long channel)
{
	bool enable = false;

	m_lock.Lock();
	PtzSerialInfoMap::iterator iter = m_cfgSerialInfoMap.find(channel);
	if( iter != m_cfgSerialInfoMap.end() )
	{
		if( iter->second.enable != 0 )
		{
			enable = true;
		}
	}
	m_lock.UnLock();

	return enable;
}

unsigned long CPtzMan::GetPtzState(unsigned long channel)
{
	unsigned long outState = PTZ_ST_NULL;

	m_lock.Lock();
	std::map<unsigned long, unsigned long>::iterator iter = m_ptzChnnState.find(channel);
	if( iter != m_ptzChnnState.end() )
	{
		outState = iter->second;
	}
	m_lock.UnLock();

	return outState;
}

void CPtzMan::SetPtzState(unsigned long channel, unsigned long state)
{
	m_lock.Lock();
	std::map<unsigned long, unsigned long>::iterator iter = m_ptzChnnState.find(channel);
	if( iter != m_ptzChnnState.end() )
	{
		iter->second = state;
	}
	else
	{
		m_ptzChnnState.insert( std::make_pair(channel, state) );
	}
	m_lock.UnLock();
}

bool CPtzMan::IsPtzLock(unsigned long channel)
{
	bool lock = false;
	m_lock.Lock();
	unsigned long state = GetPtzState(channel);
	if( state & static_cast<unsigned long>(PTZ_ST_LOCK) )
	{
		lock = true;
	}
	m_lock.UnLock();

	return lock;
}

void CPtzMan::SetPtzLock(unsigned long channel, bool lock)
{
	m_lock.Lock();
	unsigned long state = GetPtzState(channel);
	if( lock )
	{
		state |= static_cast<unsigned long>(PTZ_ST_LOCK);
	}
	else
	{
		state &= (~static_cast<unsigned long>(PTZ_ST_LOCK));
	}
	SetPtzState(channel, state);
	m_lock.UnLock();
}
/*****************************************************************************************

������	SetCruisePreviewInfo( const char *pData )
���ܣ�	����ĳͨ��Ԥ����Ѳ���㵽��̨��
����:	
		pData,��Ű���ͨ���š�Ѳ���ߺš�Ѳ��Ԥ�õ��������Ϣ
		���ݸ�ʽΪ
		
		ͨ����		  | Ѳ���ߺ�		|	Ԥ�õ����	| �����Ԥ�õ���Ϣ
		unsigned long | unsigned long	| unsigned long	| PTZ_CURISE_POINT .......

*******************************************************************************************/
bool CPtzMan::SetCruisePreviewInfo( const unsigned char *pData )
{
	//printf("%s:%s:%d, ��Ҫ����Ԥ����Ѳ���㵽��̨\n", __FILE__, __FUNCTION__, __LINE__);

	const unsigned long *pIndex = reinterpret_cast< const unsigned long *>( pData );
	unsigned long chnn = *pIndex;												//ͨ����
	unsigned long curiseIndex = *( ++pIndex );									//Ѳ���ߺ�
	unsigned long pointCount = *( ++pIndex );									//Ѳ�������
	const PTZ_CURISE_POINT *pCurisePoint = reinterpret_cast< const PTZ_CURISE_POINT * >( pData + sizeof( unsigned long ) * 3 );		//Ѳ����Ŀ�ʼλ��

	PTZ_CURISE *pCfgPtzCurise = NULL;
	PTZ_CURISE_POINT *pCfgCurisePoint = NULL;
	unsigned long cfgCuriseCount;
	bool bNewCruise = false;

	//�ȼ��ҪԤ����Ѳ�����Ƿ�������е�һ�������һ���Ͳ�������дѲ������Ϣ����̨��
	if( PTZ_ERR_OK == GetCruiseConfig( chnn, curiseIndex, &pCfgPtzCurise, &pCfgCurisePoint, cfgCuriseCount ) )
	{
		//��Ѳ���߲�Ϊ��
		//printf("%s:%s:%d, ͨ��%d�ĵ�%d��Ѳ�����Ѿ�����\n", __FILE__, __FUNCTION__, __LINE__, chnn, curiseIndex );
		assert( NULL != pCfgPtzCurise );
		assert( NULL != pCfgCurisePoint );
		assert( pCfgPtzCurise->presetNum == cfgCuriseCount );
		assert( curiseIndex == pCfgPtzCurise->index );

		if( curiseIndex != pCfgPtzCurise->index )
		{
			return false;
		}

		if( pointCount == cfgCuriseCount )
		{
			//Ԥ�õ����һ����Ƚ�ÿ��Ԥ�õ�������Ϣ�Ƿ�һ��
			if( 0 == memcmp( pCurisePoint, pCfgCurisePoint, sizeof( PTZ_CURISE_POINT ) * cfgCuriseCount ) )
			{
				//֮ǰ���õ�Ѳ����Ϣ������ҪԤ����һ�����Ͳ�������дѲ������Ϣ����̨
				//printf("%s:%s:%d, Ԥ����Ѳ������Ϣ�������е�Ѳ������Ϣһ�£�����Ҫ����дԤ�õ㵽��̨\n", __FILE__, __FUNCTION__, __LINE__);
				return true;
			}

		}

		//printf("%s:%s:%d, Ԥ����Ѳ������Ϣ�������е�Ѳ������Ϣ��һ�£���Ҫ����дԤ�õ㵽��̨\n", __FILE__, __FUNCTION__, __LINE__);

	}
	else
	{
		//����Ѳ����Ϊ��
		bNewCruise = true;
		//printf("%s:%s:%d, ͨ��%d�ĵ�%d��Ѳ���߻�û������\n", __FILE__, __FUNCTION__, __LINE__, chnn, curiseIndex );
	}

	//����Ԥ����Ѳ���㵽��̨��
	if( PTZ_ERR_OK != Action( chnn, PTZ_CMD_CRUISE_CFG, 0, 0, 0, pData+sizeof( unsigned long ) ) )
	{
		//����Ԥ�õ㵽��̨ʧ��
		return false;
	}

	//ˢ��Ԥ����Ѳ������Ϣ����̨��Ҫ����m_curiseIsPreview�����λ��Ϊ�Ѿ�Ԥ����
	if ( !bNewCruise )
	{
		//ֻ�д����������У�����Ѳ������Ϣ���ı��Ѳ���߱�Ԥ���������
		m_curiseIsPreview[chnn][curiseIndex] = 0xff;
	}

	
	return true;
}

/***************************************************************************************************

������	RecoverCruisePreviewInfo( unsigned long chnn )
���ܣ�	��ͨ��chnn��Ԥ����Ѳ������Ϣ�ָ���m_cfgCuriseMap�и�ͨ����״̬
������	
		chnn Ҫ�ָ�Ѳ�����ͨ��[0, MAX_CHNN_NUM-1]��
		��chnn����(MAX_CHNN_NUM-1)ʱ�ָ�����ͨ��Ԥ������Ѳ����

****************************************************************************************************/
bool CPtzMan::RecoverCruisePreviewInfo( unsigned long chnn )
{
	//printf("%s:%s:%d, �ָ�Ԥ����Ѳ���㵽��̨\n", __FILE__, __FUNCTION__, __LINE__);
	
	if( chnn >= MAX_CHNN_NUM )
	{
		//�ָ�����Ԥ����Ѳ���ߵ�ͨ��
		for( int i=0; i< MAX_CHNN_NUM; i++ )
		{
			RecovreOneChnnCurisePreview( i );
		}
	}
	else
	{
		//ֻ�ָ�һ��Ԥ����Ѳ���ߵ�ͨ��
		RecovreOneChnnCurisePreview( chnn );
	}

	return true;
}

void CPtzMan::EnterPtz(unsigned long clientID)
{
	m_lock.Lock();
	m_idPtzCtrl = clientID;
	m_lock.UnLock();
}

void CPtzMan::ExitPtz()
{
	StopPTZConfig();

	m_lock.Lock();
	m_idPtzCtrl = INVALID_CLIENT_ID;
	m_lock.UnLock();
}


/*********************************************************************************************************************
Function:ExitPtz()
Description:
	1���˳�ÿ��ͨ����PTZ���ÿ��ƣ����˳�Ԥ�õ㡢Ѳ���ߡ��켣����ʱҪ���øú�������ֹ�ͻ���ռ��PTZ���ö����²��ܴ��ֳ�����PTZ�Ĺ켣��Ѳ������
	2���ͻ������쳣�˳�ʱҪֹͣ��̨�����ò�����Ѳ����Ԥ�õ㡢�켣��������������ֹͣ
Date:
	2010-06-01 zhl
************************************************************************************************************************/
void CPtzMan::StopPTZConfig()
{
	for(unsigned long channel=0; channel<MAX_CHNN_NUM; channel++)
	{
		if(GetPtzEnable(channel))
		{
			unsigned long status = GetPtzState(channel);				//��ͨ����̨�ĵ�ǰ״̬

			//if( PTZ_ST_CFG_PRESET & status ) 
			//{

			//}

			if( PTZ_ST_CFG_TRACK & status )
			{
				ProcessAction(channel, PTZ_CMD_TRACK_DEL, 0);
				status &= static_cast<unsigned long>(~PTZ_ST_CFG_TRACK);
				SetPtzState(channel, status);
			}

			if( PTZ_ST_CFG_CRUISE & status ) 
			{
				ProcessAction(channel, PTZ_CMD_LEAVE_CURISE_MODE, 0);
				status &= static_cast<unsigned long>(~PTZ_ST_CFG_CRUISE);
				SetPtzState(channel, status);
			}
		}
	}
}


bool CPtzMan::WantToConfigPTZ(unsigned char cmdtype)
{
	switch(cmdtype)
	{
		case PTZ_CMD_PRESET_SET:			//����Ԥ�õ�
			return true;
		case PTZ_CMD_PRESET_DEL:			//ɾ��Ԥ�õ�
			return true;

		case PTZ_CMD_CRUISE_CFG:			//����Ѳ����,�൱��ִ��Enter��Set��Leave��������
		case PTZ_CMD_ENTER_CURISE_MODE:		//����Ѳ��ģʽ,�˺��������Ѳ����Ԥ�õ�
		case PTZ_CMD_CRUISE_SET:			//����Ѳ���ߵ�Ԥ�õ�
		case PTZ_CMD_LEAVE_CURISE_MODE:		//�˳�����Ѳ��
			return true;
		case PTZ_CMD_TRACK_SET:			//��ʼ����켣
			return true;
		case PTZ_CMD_TRACK_DEL:			//ֹͣ����켣
			return true;
		default:
			return false;
	}
}

unsigned long CPtzMan::PtzClient()
{
	return m_idPtzCtrl;
}

//////////////////////////////////////////////////////////////////////////
// ��������
//////////////////////////////////////////////////////////////////////////
unsigned long CPtzMan::ProcessAction(unsigned long channel, unsigned char cmdtype, unsigned long ptzParam, const unsigned char speed, unsigned char waiteTime, unsigned char* ptrParamEx)
{
	/*
	���������������ʽ����
	������Ч����������ٴι��˴������ȴ������Դ�����������
	*/

	unsigned long ret = PTZ_ERR_OK;

	//...����

	//�������
	PtzCmdPack cmdPack;
	memset(&cmdPack, 0, sizeof(PtzCmdPack));

	cmdPack.channel = channel;
	cmdPack.cmd_type = cmdtype;
	cmdPack.param = ptzParam;
	cmdPack.speed = speed;
	cmdPack.wait_time = waiteTime;
	cmdPack.process_state = PTZ_CMD_WAITING;
	cmdPack.ptr_param_ex = ptrParamEx;
	cmdPack.start_time = GetCurrTime32();

	ret = CanProcess(cmdPack);
	if ( PTZ_ERR_OK_ULONG == ret )
	{
		unsigned long timeout = 0;
		while(1)
		{
			unsigned long bCanChnn = CanChnnProcess(cmdPack.channel, GetCurrTime32());

			if( timeout >= 0xFF )
			{
				//�������ȴ�ʱ��,�������ȴ�,ֱ�Ӵ���
				bCanChnn = PTZ_ERR_OK_ULONG;
				printf("(%s). time out.", __FUNCTION__);
			}

			if( PTZ_ERR_OK_ULONG == bCanChnn )
			{
				ret = ProcessCommand(cmdPack);
				break;
			}
			//printf("%s:%s:%d, �ȴ�ִ����һ������\n", __FUNCTION__, __FILE__, __LINE__);
			timeout += 1;
			PUB_Sleep(100);
		}
	}

	return ret;
}

unsigned long CPtzMan::ProcessCommand(PtzCmdPack& cmd)
{
	/*
		����������׶�,���������:�������г�д����֮��Ĵ������.
	���������֮ǰ��Ҫ��鱣֤������ܹ���ִ��.
	*/

	unsigned long ret = PTZ_ERR_OK;

	m_lock.Lock();

	PtzSerialInfoMap::iterator iter = m_cfgSerialInfoMap.find(cmd.channel);
	if( iter != m_cfgSerialInfoMap.end() )
	{
		CPTZProtocol* pPtz = GetPtzProtocol((iter->second).protocol,cmd.channel);
		if( pPtz )
		{
#ifdef   __ENVIRONMENT_LINUX__
			if (!CFakeCurise::Instance()->PreAction(cmd.channel, pPtz, static_cast<unsigned char>(cmd.cmd_type), cmd.param))
			{
				if( pPtz->Command(static_cast<unsigned char>(cmd.cmd_type),
					(iter->second).addr,
					(iter->second).baud_rate,
					static_cast<unsigned char>(cmd.speed),
					cmd.param,
					static_cast<unsigned char>(cmd.wait_time)) )
				{
					cmd.end_time = GetCurrTime32();
					cmd.process_state = PTZ_CMD_SUCC;
					cmd.ptr_param_ex = NULL;
					ret = PTZ_ERR_OK;
				}
				else
				{
					cmd.end_time = GetCurrTime32();
					cmd.process_state = PTZ_CMD_FAIL;
					cmd.ptr_param_ex = NULL;
					ret = PTZ_ERR_WRITE_COM; //д����ʧ��
				}
			}
			

			//���۳ɹ����,����Ϊ�����������һ�δ����
			(iter->second).last_cmd_type = cmd.cmd_type;
			(iter->second).last_process_time = cmd.end_time;
			(iter->second).wait_time = cmd.wait_time;
#endif
		}
		else
		{
			//assert(false); //û���ҵ����������Э�鼯
			ret = PTZ_ERR_UF_PROTOCOL;
		}
	}
	else
	{
		//assert(false); //û���ҵ����ͨ��������
		ret = PTZ_ERR_CHNN_UN_SET;
	}

	m_lock.UnLock();

	return ret;
}

unsigned long CPtzMan::CanChnnProcess(unsigned long chnn, unsigned long timetip)
{
	return PTZ_ERR_OK;
}

unsigned long CPtzMan::CanProcess(const PtzCmdPack& cmd)
{
	return PTZ_ERR_OK;
}

unsigned long CPtzMan::GetCmdType(unsigned char cmdtype, unsigned long param, bool& bEnd)
{
	unsigned long type = PTZ_ST_NULL;
	bEnd = false;

	switch ( cmdtype )
	{
	case PTZ_CMD_STOP:
	case PTZ_CMD_LEFT:				//��
	case PTZ_CMD_RIGHT:				//��
	case PTZ_CMD_UP:					//��
	case PTZ_CMD_DOWN:				//��
	case PTZ_CMD_LEFT_UP:			//����
	case PTZ_CMD_LEFT_DOWN:	//����
	case PTZ_CMD_RIGHT_UP:		//����
	case PTZ_CMD_RIGHT_DOWN:	//����
	case PTZ_CMD_NEAR:				//����
	case PTZ_CMD_FAR:				//��Զ
	case PTZ_CMD_ZOOM_OUT:			//��С
	case PTZ_CMD_ZOOM_IN:			//�Ŵ�
	case PTZ_CMD_IRIS_OPEN:			//������Ȧ
	case PTZ_CMD_IRIS_CLOSE:			//�رչ�Ȧ
	case PTZ_CMD_PRESET_GO:			//���ڼ���Ԥ�õ�
		{
			type = PTZ_ST_NORMAL;
		}
		break;
	case PTZ_CMD_PRESET_SET:			//����Ԥ�õ�
	case PTZ_CMD_PRESET_DEL:			//ɾ��Ԥ�õ�
		{
			type = PTZ_ST_NORMAL/*PTZ_ST_CFG_PRESET*/;
		}
		break;
	case PTZ_CMD_CRUISE_CFG:
	case PTZ_CMD_ENTER_CURISE_MODE:	//����Ѳ��ģʽ,�˺��������Ѳ����Ԥ�õ�
	case PTZ_CMD_CRUISE_SET:			//����Ѳ���ߵ�Ԥ�õ�
	case PTZ_CMD_LEAVE_CURISE_MODE:	//�˳�����Ѳ��
	case PTZ_CMD_CRUISE_DEL:			//ɾ��Ѳ����
		{
			type = PTZ_ST_CFG_CRUISE;
		}
		break;
	case PTZ_CMD_CRUISE_RUN:			//ѡ��һ��Ѳ���߽���Ѳ��
	case PTZ_CMD_CRUISE_STOP:		//ֹͣѲ��
		{
			type = PTZ_ST_CRUISE;
		}
		break;
	case PTZ_CMD_TRACK_START:		//��ʼ�켣
	case PTZ_CMD_TRACK_STOP:			//ֹͣ�켣
		{
			type = PTZ_ST_TRACK;
		}
		break;
	case PTZ_CMD_TRACK_SET:			//��ʼ����켣
	case PTZ_CMD_TRACK_DEL:			//ֹͣ����켣
		{
			type = PTZ_ST_CFG_TRACK;
		}
		break;
	case PTZ_CMD_AUTO_SCAN_START:	//��ʼ�Զ�ɨ��
	case PTZ_CMD_AUTO_SCAN_STOP:	//ֹͣ�Զ�ɨ��
		{
			type = PTZ_ST_AUTOSCAN;
		}
		break;
	case PTZ_CMD_RESET:
		{
			type = PTZ_ST_NULL;
		}
		break;
	case PTZ_CMD_SET_AUXILIARY:
	case PTZ_CMD_CLEAR_AUXILIARY:
		{
			if (AUXILIARY_BRUSH == param)
			{
				type = PTZ_ST_BRUSH_ON;
			}
			else if(AUXILIARY_LAMP == param)
			{
				type = PTZ_ST_LAMP_ON;
			}
			else
			{
				assert(false);
			}
		}
		break;
	default:
		assert(false);
		break;
	}

	switch ( cmdtype )
	{
	case PTZ_CMD_LEAVE_CURISE_MODE:
	case PTZ_CMD_CRUISE_STOP:
	case PTZ_CMD_TRACK_DEL:
	case PTZ_CMD_TRACK_STOP:
	case PTZ_CMD_AUTO_SCAN_STOP:
	case PTZ_CMD_RESET:
	case PTZ_CMD_CLEAR_AUXILIARY:
		{
			bEnd = true;
		}
		break;
	}

	return type;
}

bool CPtzMan::ResetPtz(unsigned long chnn, unsigned long& curState)
{
	bool ret = false;
	unsigned long status = GetPtzState( chnn );

	if( ( PTZ_ST_NULL == status ) || ( PTZ_ST_NORMAL & status ) )
	{
		ret = true;
	}

	if( PTZ_ST_TRACK & status )
	{
		ProcessAction( chnn, PTZ_CMD_TRACK_STOP, 0 );
		status &= static_cast<unsigned long>(~PTZ_ST_TRACK);
		ret = true;
	}

	if( PTZ_ST_CRUISE & status )
	{
		//ֹͣѲ��
		ProcessAction(chnn, PTZ_CMD_CRUISE_STOP, 0);
		status &= static_cast<unsigned long>(~PTZ_ST_CRUISE);
		ret = true;
	}

	if ( PTZ_ST_BRUSH_ON & status)
	{
		//��ֹͣ��ˢ
		ret = true;
	}

	//���ﲻֹͣ�ƹ�
	if ( PTZ_ST_LAMP_ON & status)
	{
		ret = true;
	}

	if( PTZ_ST_AUTOSCAN & status )
	{
		//ֹͣ�Զ�ɨ��
		ProcessAction(chnn, PTZ_CMD_AUTO_SCAN_STOP, 0);
		status &= static_cast<unsigned long>(~PTZ_ST_AUTOSCAN);
		ret = true;
	}

	if( ( PTZ_ST_CFG_PRESET & status ) || ( PTZ_ST_CFG_TRACK & status ) || ( PTZ_ST_CFG_CRUISE & status ) )
	{
		//������ֻ����ƥ�� PTZ_ST_CFG_TRACK
		ret = false;
	}

	curState = status;
	SetPtzState(chnn, curState);
	return ret;
}

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
CPTZProtocol* CPtzMan::GetPtzProtocol(unsigned long protocol,unsigned long chnn)
{
	/*
		��ȡPTZЭ��֧����,�ȴ��Ѵ����ļ����в���;
	û�ҵ�,���´���.
	*/
	assert(protocol < static_cast<unsigned long>(PROTOCOL_END));

#ifdef __ENVIRONMENT_WIN32__
	protocol = PROTOCOL_NULL;
#endif

	PtzProtocolMap::iterator iter = m_protocolMap.find(protocol);

	if( iter != m_protocolMap.end() )
	{
#ifndef __ENVIRONMENT_WIN32__
		if(PROTOCOL_TVI == static_cast<PROTOCOL_TYPE>(protocol))
		{
			/*CPTZTvi *pPtzTvi = dynamic_cast<CPTZTvi*>(iter->second);
			assert(pPtzTvi != NULL);
			if(chnn != pPtzTvi->GetCurChnn())
			{
				pPtzTvi->SetCurChnn(chnn);
			}*/
			iter->second = CPtzAux::Instance()->GetPtzPoint(chnn);
		}
#endif
		return iter->second;
	}
	else
	{
		CPTZProtocol* pPtz = NULL;

		switch (static_cast<PROTOCOL_TYPE>(protocol))
		{
#ifdef __ENVIRONMENT_WIN32__
		case PROTOCOL_NULL:
		case PROTOCOL_PELCOP:
		case PROTOCOL_PELCOD:
		case PROTOCOL_LILIN:
		case PROTOCOL_MINKING:
		case PROTOCOL_NEON:
		case PROTOCOL_STAR:
		case PROTOCOL_VIDO:
		case PROTOCOL_DSCP:
		case PROTOCOL_VISCA:
		case PROTOCOL_SAMSUNG:
		case PROTOCOL_RM110:
		case PROTOCOL_HY:
		case PROTOCOL_TVI:
			pPtz = new CPTZProtocol();
			break;
#else
		case PROTOCOL_NULL:
			pPtz = new CPTZNULL();
			break;
		case PROTOCOL_PELCOP:
			pPtz = new CPTZPelcoP();
			break;
		case PROTOCOL_PELCOD:
			pPtz = new CPTZPelcoD();
			break;
		case PROTOCOL_LILIN:
			pPtz = new CPTZLilin();
			break;
		case PROTOCOL_MINKING:
			pPtz = new CPTZMinking();
			break;
		case PROTOCOL_NEON:
			pPtz = new CPTZNeon();
			break;
		case PROTOCOL_STAR:
			pPtz = new CPTZStar();
			break;
		case PROTOCOL_VIDO:
			pPtz = new CPTZVido();
			break;
		case PROTOCOL_DSCP:
			pPtz = new CPTZDscp();
			break;
		case PROTOCOL_VISCA:
			pPtz = new CPTZVisca();
			break;
		case PROTOCOL_SAMSUNG:
			pPtz = new CPTZSamsung();
			break;
		case PROTOCOL_RM110:
			pPtz = new CPTZRm110();
			break;
		case PROTOCOL_HY:
			pPtz = new CPTZHy();
			break;
		case PROTOCOL_PL14:
			pPtz = new CPTZPL14();
			break;
#if defined(__PTZ_GE__)
		case PROTOCOL_GE:
			pPtz = new CPTZKtd348();
			break;
#endif
		case PROTOCOL_TVI:
			//pPtz = new CPTZTvi(chnn);
			pPtz = CPtzAux::Instance()->GetPtzPoint(chnn);
			break;
#endif
		default:
			assert(false);
			break;
		}

		if( NULL != pPtz )
		{
			m_protocolMap.insert( std::make_pair( protocol, pPtz ) );
		}
		else
		{
			assert( false ); 
		}
		return pPtz;
	}
}

/*************************************************************************************************

������	RecovreOneChnnCurisePreview( unsigned long chnn )
���ܣ�	�ָ�ͨ��chnn��Ԥ������Ѳ����
������	
		chnn [0,MAX_CHNN_NUM-1]

**************************************************************************************************/
bool CPtzMan::RecovreOneChnnCurisePreview( unsigned long chnn )
{
	assert( ( 0<= chnn ) && ( MAX_CHNN_NUM > chnn ) );

	for( int i=0; i<PTZ_MAX_CURISE_NUM; i++ )
	{
		if( 0 != m_curiseIsPreview[chnn][i] )
		{
			PTZ_CURISE *pCfgPtzCurise = NULL;
			PTZ_CURISE_POINT *pCfgCurisePoint = NULL;
			unsigned long cfgCuriseCount;
			if( PTZ_ERR_OK == GetCruiseConfig( chnn, i, &pCfgPtzCurise, &pCfgCurisePoint, cfgCuriseCount ) )
			{
				//��Ѳ���߲�Ϊ��
				//printf("%s:%s:%d, ͨ��%d�ĵ�%d��Ѳ�����Ѿ�����\n", __FILE__, __FUNCTION__, __LINE__, chnn, i );
				assert( NULL != pCfgPtzCurise );
				assert( NULL != pCfgCurisePoint );
				assert( pCfgPtzCurise->presetNum == cfgCuriseCount );
				assert( i == pCfgPtzCurise->index );

				if( i != pCfgPtzCurise->index )
				{
					return false;
				}

			}
			else
			{
				//����Ѳ��������������Ϊ��,
				assert( false );
				//printf("%s:%s:%d, ͨ��%d�ĵ�%d��Ѳ���߻�û������\n", __FILE__, __FUNCTION__, __LINE__, chnn, i );
			}
			
			int dataLen = sizeof( unsigned long ) + sizeof( unsigned long ) + sizeof( PTZ_CURISE_POINT ) * cfgCuriseCount;
			unsigned char *pData = new unsigned char [ dataLen ];
			assert( NULL != pData );
			memset( pData, 0, dataLen );

			unsigned long *pIndex = reinterpret_cast< unsigned long * >( pData );
			*pIndex = pCfgPtzCurise->index;				//Ѳ���ߺ�
			*(++pIndex) = cfgCuriseCount;				//Ԥ�õ����
			memcpy( pData + ( sizeof( unsigned long ) * 2 ), pCfgCurisePoint,  sizeof( PTZ_CURISE_POINT ) * cfgCuriseCount );		//�����Ԥ�õ���Ϣ

			Action( chnn, PTZ_CMD_CRUISE_CFG ,0, 0, 0, pData );

			//����Ԥ�����Ϊû��Ԥ����
			m_curiseIsPreview[chnn][i] = 0;

			if( NULL != pData)
			{
				delete [] pData;
				pData = NULL;
			}
		}
	}

	return true;
}

