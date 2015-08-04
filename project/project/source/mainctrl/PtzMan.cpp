
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
// 构造、初始化、退出
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
	Action函数参数例用说明:
	调整云台:
	删除巡航点:			Action(通道号, PTZCMD_CRUISE_DEL,	巡航点ID,			无效,  无效);

	运行巡航点:			Action(通道号, PTZCMD_CRUISE_RUN,	巡航点ID,			无效,  无效);

	停止巡航:				Action(通道号, PTZCMD_CRUISE_STOP,	巡航点ID,		无效,  无效);

	到达某预置点:		Action(通道号, PTZCMD_PRESETGO,	预置点ID,			无效,  无效);

	保存预置点:			Action(通道号, PTZCMD_PRESETSET,	预置点ID,			无效,  无效);

	配置巡航点:			SetCurisePoint(通道号, 巡航点ID, 巡航点系列,巡航点数量);

	记录轨迹：			Action(通道号，PTZCMD_MOVE,		PTZCOMMAND_TRACK_RECORD,速度);
	停止记录轨迹：	Action(通道号, PTZCMD_MOVE,			PTZCOMMAND_TRACK_STOP_RECORD);

	开始轨迹：			Action(通道号, PTZCMD_MOVE,			PTZCOMMAND_TRACK_RUN,速度)；
	停止轨迹：			Action(通道号, PTZCMD_MOVE,			PTZCOMMAND_TRACK_STOP_RUN);
	自动扫描：			Action(通道号, PTZCMD_PRESETGO,			0X99);
	停止自动扫描：	Action(通道号，PTZCMD_PRESETGO,			0X96);
*/
unsigned long CPtzMan::Action(unsigned long channel, unsigned char cmdtype, unsigned long ptzParam, unsigned char speed, unsigned char waiteTime, const unsigned char* ptrParamEx)
{
	/*
		执行所有操作入口
		锁操作暂时未用到
	*/
	unsigned long ret = 0;
	bool bProcessed = false;

	bool bEnd = false;
	unsigned long type = GetCmdType(cmdtype, ptzParam, bEnd);			//云台的下一个状态
	unsigned long state = GetPtzState(channel);				//云台的当前状态
	bool bLock = state & static_cast<unsigned long>(PTZ_ST_LOCK);

	if( bLock )
	{
		ret = PTZ_ERR_CHNN_LOCK;
	}
	else
	{
		//是否启用通道 && 云台处于初始状态
		if(GetPtzEnable(channel))
		{
			// * 非锁定、非配置类型的命令
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
				//雨刷和灯光等的辅助开关操作不应当关闭当前正在进行的云台动作。
				//其他动作也不能停止当前的雨刷和灯光状态
				if(((PTZ_CMD_SET_AUXILIARY == cmdtype) || (PTZ_CMD_CLEAR_AUXILIARY == cmdtype)) || ResetPtz(channel, state))
				{
					// * 配置巡航线
					if( !bProcessed && static_cast<unsigned long>(PTZ_CMD_CRUISE_CFG) == cmdtype )
					{
						// [巡航线编号unsigned long、巡航线点个数unsigned long、巡航线点信息PTZ_CURISE_POINT*]
						unsigned long* pValue = reinterpret_cast<unsigned long*>((unsigned char*)ptrParamEx);
						unsigned long index = *pValue++;
						assert(index<PTZ_MAX_CURISE_NUM);
						unsigned long count = *pValue++;
						assert(count<=PTZ_MAX_CURISE_PT_NUM);

						if( count > 0 )
						{
							//配置巡航线
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
							//删除巡航线
							ret = ProcessAction(channel, PTZ_CMD_CRUISE_DEL, index);
						}

						bEnd = true;
						bProcessed = true;
					}

					// * 空、普通
					if(!bProcessed)
					{
						if((PTZ_CMD_PRESET_GO != cmdtype) || ((PTZ_CMD_PRESET_GO == cmdtype) && PresetIsEnable(channel, ptzParam)))
						{
							//不是预置点操作时直接执行，是预置点操作时查看该预置点是否可被执行
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
					// * 配置轨迹[配置轨迹中可以有普通操作]
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
				//设置状态
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
	
	//检查是否溢出最大预置点个数
	unsigned long realCount = count;
	if( count > PTZ_MAX_PRESET_NUM )
	{
		realCount = PTZ_MAX_PRESET_NUM;
		printf("(%s) overflow max preset num.set count=%d.max count=%d.real count=%d.\n", __FUNCTION__, count, PTZ_MAX_PRESET_NUM, realCount);
		assert(false);
	}

	//查找有没配置过此通道,有则修改;没有则新添加
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

	//检查是否溢出最大巡航线点个数
	unsigned long realCount = count;
	if( count > PTZ_MAX_CURISE_PT_NUM )
	{
		realCount = PTZ_MAX_CURISE_PT_NUM;
		printf("(%s) overflow max cruise point.set count=%d.max count=%d.real count=%d.\n", __FUNCTION__, count, PTZ_MAX_CURISE_PT_NUM, realCount);
		assert(false);
	}

	//检查溢出巡航线条数,
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

函数：	SetCruisePreviewInfo( const char *pData )
功能：	设置某通道预览的巡航点到云台中
参数:	
		pData,存放包括通道号、巡航线号、巡航预置点等数据信息
		数据格式为
		
		通道号		  | 巡航线号		|	预置点个数	| 具体的预置点信息
		unsigned long | unsigned long	| unsigned long	| PTZ_CURISE_POINT .......

*******************************************************************************************/
bool CPtzMan::SetCruisePreviewInfo( const unsigned char *pData )
{
	//printf("%s:%s:%d, 将要设置预览的巡航点到云台\n", __FILE__, __FUNCTION__, __LINE__);

	const unsigned long *pIndex = reinterpret_cast< const unsigned long *>( pData );
	unsigned long chnn = *pIndex;												//通道号
	unsigned long curiseIndex = *( ++pIndex );									//巡航线号
	unsigned long pointCount = *( ++pIndex );									//巡航点个数
	const PTZ_CURISE_POINT *pCurisePoint = reinterpret_cast< const PTZ_CURISE_POINT * >( pData + sizeof( unsigned long ) * 3 );		//巡航点的开始位置

	PTZ_CURISE *pCfgPtzCurise = NULL;
	PTZ_CURISE_POINT *pCfgCurisePoint = NULL;
	unsigned long cfgCuriseCount;
	bool bNewCruise = false;

	//先检测要预览的巡航点是否和配置中的一样，如果一样就不用重新写巡航点信息到云台中
	if( PTZ_ERR_OK == GetCruiseConfig( chnn, curiseIndex, &pCfgPtzCurise, &pCfgCurisePoint, cfgCuriseCount ) )
	{
		//该巡航线不为空
		//printf("%s:%s:%d, 通道%d的第%d条巡航线已经配置\n", __FILE__, __FUNCTION__, __LINE__, chnn, curiseIndex );
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
			//预置点个数一样则比较每个预置点的相关信息是否一样
			if( 0 == memcmp( pCurisePoint, pCfgCurisePoint, sizeof( PTZ_CURISE_POINT ) * cfgCuriseCount ) )
			{
				//之前配置的巡航信息和现在要预览的一样，就不用重新写巡航点信息到云台
				//printf("%s:%s:%d, 预览的巡航点信息和配置中的巡航点信息一致，不需要重新写预置点到云台\n", __FILE__, __FUNCTION__, __LINE__);
				return true;
			}

		}

		//printf("%s:%s:%d, 预览的巡航点信息和配置中的巡航点信息不一致，需要重新写预置点到云台\n", __FILE__, __FUNCTION__, __LINE__);

	}
	else
	{
		//该条巡航线为空
		bNewCruise = true;
		//printf("%s:%s:%d, 通道%d的第%d条巡航线还没有配置\n", __FILE__, __FUNCTION__, __LINE__, chnn, curiseIndex );
	}

	//设置预览的巡航点到云台中
	if( PTZ_ERR_OK != Action( chnn, PTZ_CMD_CRUISE_CFG, 0, 0, 0, pData+sizeof( unsigned long ) ) )
	{
		//设置预置点到云台失败
		return false;
	}

	//刷新预览的巡航点信息到云台后要设置m_curiseIsPreview中相关位置为已经预览过
	if ( !bNewCruise )
	{
		//只有存在于配置中，并且巡航点信息被改变的巡航线被预览后才设置
		m_curiseIsPreview[chnn][curiseIndex] = 0xff;
	}

	
	return true;
}

/***************************************************************************************************

函数：	RecoverCruisePreviewInfo( unsigned long chnn )
功能：	把通道chnn中预览的巡航线信息恢复到m_cfgCuriseMap中该通道的状态
参数：	
		chnn 要恢复巡航点的通道[0, MAX_CHNN_NUM-1]，
		当chnn大于(MAX_CHNN_NUM-1)时恢复所有通道预览过的巡航线

****************************************************************************************************/
bool CPtzMan::RecoverCruisePreviewInfo( unsigned long chnn )
{
	//printf("%s:%s:%d, 恢复预览的巡航点到云台\n", __FILE__, __FUNCTION__, __LINE__);
	
	if( chnn >= MAX_CHNN_NUM )
	{
		//恢复所有预览过巡航线的通道
		for( int i=0; i< MAX_CHNN_NUM; i++ )
		{
			RecovreOneChnnCurisePreview( i );
		}
	}
	else
	{
		//只恢复一个预览过巡航线的通道
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
	1：退出每个通道的PTZ配置控制，在退出预置点、巡航线、轨迹配置时要调用该函数，防止客户端占用PTZ配置而导致不能从现场控制PTZ的轨迹、巡航功能
	2：客户端在异常退出时要停止云台的配置操作（巡航、预置点、轨迹），其他操作不停止
Date:
	2010-06-01 zhl
************************************************************************************************************************/
void CPtzMan::StopPTZConfig()
{
	for(unsigned long channel=0; channel<MAX_CHNN_NUM; channel++)
	{
		if(GetPtzEnable(channel))
		{
			unsigned long status = GetPtzState(channel);				//该通道云台的当前状态

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
		case PTZ_CMD_PRESET_SET:			//设置预置点
			return true;
		case PTZ_CMD_PRESET_DEL:			//删除预置点
			return true;

		case PTZ_CMD_CRUISE_CFG:			//设置巡航线,相当于执行Enter、Set、Leave三个命令
		case PTZ_CMD_ENTER_CURISE_MODE:		//进入巡航模式,此后可以设置巡航的预置点
		case PTZ_CMD_CRUISE_SET:			//设置巡航线的预置点
		case PTZ_CMD_LEAVE_CURISE_MODE:		//退出设置巡航
			return true;
		case PTZ_CMD_TRACK_SET:			//开始保存轨迹
			return true;
		case PTZ_CMD_TRACK_DEL:			//停止保存轨迹
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
// 处理命令
//////////////////////////////////////////////////////////////////////////
unsigned long CPtzMan::ProcessAction(unsigned long channel, unsigned char cmdtype, unsigned long ptzParam, const unsigned char speed, unsigned char waiteTime, unsigned char* ptrParamEx)
{
	/*
	本程序段是阻塞方式处理
	过滤无效命令、打包命令、再次过滤打包命令、等待到可以处理、处理命令
	*/

	unsigned long ret = PTZ_ERR_OK;

	//...过滤

	//打包命令
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
				//超过最大等待时间,不继续等待,直接处理
				bCanChnn = PTZ_ERR_OK_ULONG;
				printf("(%s). time out.", __FUNCTION__);
			}

			if( PTZ_ERR_OK_ULONG == bCanChnn )
			{
				ret = ProcessCommand(cmdPack);
				break;
			}
			//printf("%s:%s:%d, 等待执行下一个命令\n", __FUNCTION__, __FILE__, __LINE__);
			timeout += 1;
			PUB_Sleep(100);
		}
	}

	return ret;
}

unsigned long CPtzMan::ProcessCommand(PtzCmdPack& cmd)
{
	/*
		进处理命令阶段,正常情况下:不容许有除写串口之外的错误出现.
	进本程序段之前需要检查保证命令包能够被执行.
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
					ret = PTZ_ERR_WRITE_COM; //写串口失败
				}
			}
			

			//不论成功与否,都认为此命令是最后一次处理过
			(iter->second).last_cmd_type = cmd.cmd_type;
			(iter->second).last_process_time = cmd.end_time;
			(iter->second).wait_time = cmd.wait_time;
#endif
		}
		else
		{
			//assert(false); //没有找到处理命令的协议集
			ret = PTZ_ERR_UF_PROTOCOL;
		}
	}
	else
	{
		//assert(false); //没有找到相关通道的设置
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
	case PTZ_CMD_LEFT:				//左
	case PTZ_CMD_RIGHT:				//右
	case PTZ_CMD_UP:					//上
	case PTZ_CMD_DOWN:				//下
	case PTZ_CMD_LEFT_UP:			//左上
	case PTZ_CMD_LEFT_DOWN:	//左下
	case PTZ_CMD_RIGHT_UP:		//右上
	case PTZ_CMD_RIGHT_DOWN:	//右下
	case PTZ_CMD_NEAR:				//拉近
	case PTZ_CMD_FAR:				//调远
	case PTZ_CMD_ZOOM_OUT:			//缩小
	case PTZ_CMD_ZOOM_IN:			//放大
	case PTZ_CMD_IRIS_OPEN:			//开启光圈
	case PTZ_CMD_IRIS_CLOSE:			//关闭光圈
	case PTZ_CMD_PRESET_GO:			//到第几个预置点
		{
			type = PTZ_ST_NORMAL;
		}
		break;
	case PTZ_CMD_PRESET_SET:			//设置预置点
	case PTZ_CMD_PRESET_DEL:			//删除预置点
		{
			type = PTZ_ST_NORMAL/*PTZ_ST_CFG_PRESET*/;
		}
		break;
	case PTZ_CMD_CRUISE_CFG:
	case PTZ_CMD_ENTER_CURISE_MODE:	//进入巡航模式,此后可以设置巡航的预置点
	case PTZ_CMD_CRUISE_SET:			//设置巡航线的预置点
	case PTZ_CMD_LEAVE_CURISE_MODE:	//退出设置巡航
	case PTZ_CMD_CRUISE_DEL:			//删除巡航线
		{
			type = PTZ_ST_CFG_CRUISE;
		}
		break;
	case PTZ_CMD_CRUISE_RUN:			//选择一个巡航线进行巡航
	case PTZ_CMD_CRUISE_STOP:		//停止巡航
		{
			type = PTZ_ST_CRUISE;
		}
		break;
	case PTZ_CMD_TRACK_START:		//开始轨迹
	case PTZ_CMD_TRACK_STOP:			//停止轨迹
		{
			type = PTZ_ST_TRACK;
		}
		break;
	case PTZ_CMD_TRACK_SET:			//开始保存轨迹
	case PTZ_CMD_TRACK_DEL:			//停止保存轨迹
		{
			type = PTZ_ST_CFG_TRACK;
		}
		break;
	case PTZ_CMD_AUTO_SCAN_START:	//开始自动扫描
	case PTZ_CMD_AUTO_SCAN_STOP:	//停止自动扫描
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
		//停止巡航
		ProcessAction(chnn, PTZ_CMD_CRUISE_STOP, 0);
		status &= static_cast<unsigned long>(~PTZ_ST_CRUISE);
		ret = true;
	}

	if ( PTZ_ST_BRUSH_ON & status)
	{
		//不停止雨刷
		ret = true;
	}

	//这里不停止灯光
	if ( PTZ_ST_LAMP_ON & status)
	{
		ret = true;
	}

	if( PTZ_ST_AUTOSCAN & status )
	{
		//停止自动扫描
		ProcessAction(chnn, PTZ_CMD_AUTO_SCAN_STOP, 0);
		status &= static_cast<unsigned long>(~PTZ_ST_AUTOSCAN);
		ret = true;
	}

	if( ( PTZ_ST_CFG_PRESET & status ) || ( PTZ_ST_CFG_TRACK & status ) || ( PTZ_ST_CFG_CRUISE & status ) )
	{
		//本部分只可能匹配 PTZ_ST_CFG_TRACK
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
		获取PTZ协议支持类,先从已创建的集合中查找;
	没找到,则新创建.
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

函数：	RecovreOneChnnCurisePreview( unsigned long chnn )
功能：	恢复通道chnn中预览过的巡航线
参数：	
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
				//该巡航线不为空
				//printf("%s:%s:%d, 通道%d的第%d条巡航线已经配置\n", __FILE__, __FUNCTION__, __LINE__, chnn, i );
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
				//该条巡航线在配置这中为空,
				assert( false );
				//printf("%s:%s:%d, 通道%d的第%d条巡航线还没有配置\n", __FILE__, __FUNCTION__, __LINE__, chnn, i );
			}
			
			int dataLen = sizeof( unsigned long ) + sizeof( unsigned long ) + sizeof( PTZ_CURISE_POINT ) * cfgCuriseCount;
			unsigned char *pData = new unsigned char [ dataLen ];
			assert( NULL != pData );
			memset( pData, 0, dataLen );

			unsigned long *pIndex = reinterpret_cast< unsigned long * >( pData );
			*pIndex = pCfgPtzCurise->index;				//巡航线号
			*(++pIndex) = cfgCuriseCount;				//预置点个数
			memcpy( pData + ( sizeof( unsigned long ) * 2 ), pCfgCurisePoint,  sizeof( PTZ_CURISE_POINT ) * cfgCuriseCount );		//具体的预置点信息

			Action( chnn, PTZ_CMD_CRUISE_CFG ,0, 0, 0, pData );

			//设置预览标记为没有预览过
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

