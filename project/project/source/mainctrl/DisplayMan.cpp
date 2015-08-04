
#include "DisplayMan.h"
#include "MessageMan.h"
#include "LocalDevice.h"
#include "MessageMan.h"
#include "IPCDecDisplayMan.h"
#include "NetDeviceManager.h"
#include "FuncCustom.h"
extern VGA_RESOLUTION	g_resolution;

extern ULONGLONG g_net_channel;
extern unsigned long g_adjustCvbsVO;
extern VIDEO_FORMAT g_video_format;

CDisplayMan* CDisplayMan::Instance()
{
	static CDisplayMan s_DisplayMan;
	return &s_DisplayMan;
}

CDisplayMan::DISPLAY_INFO::DISPLAY_INFO()
{
	record = 0;
	num_attrib = 0;
	time_out = 0;

	for (int i = 0; i < DISPLAY_LAYER_MAX; i++)
	{
		layer[i].type = i;
		
		layer[i].z_order = i;
		layer[i].signal_format = 0;
		layer[i].num_chnn = 0;
		layer[i].split_mode = 0;
		memset(layer[i].area_chnn, 0xFF, sizeof(unsigned char) * DISPLAY_INDEX_MAX);
	}

	memset(display_attrib, 0, sizeof(DISPLAY_ATTRIB) * MAX_CHNN_NUM);
}

bool CDisplayMan::DISPLAY_INFO::operator!=(const CDisplayMan::DISPLAY_INFO& display) const
{
	return (0 == memcmp(this, &display, sizeof(DISPLAY_INFO))) ? false : true;
}

CDisplayMan::CDisplayMan() : m_max_history(8)
, m_max_stack(1)
{
	m_splitMode = 0;
	m_dwellTime = 5;

	m_bHaveSpot = true;
	m_dwellTimeSpot = 0;
	m_splitDataLenSpot = 0;
	m_pSplitDataSpot = NULL;

	m_pTmpData = NULL;
	m_tmpDataLen = 0;

	m_bInit = false;
	m_bInitResolution = false;
	m_bFullChannelDisplay = false;

	m_activeData = -1;
	for(int i=0; i<SPLIT_COUNT; i++)
	{
		m_splitDataLen[i] = 0;
		m_pSplitData[i] = NULL;
	}

	m_dwellTimeDataLen = 0;
	m_pDwellTimeData = NULL;

	m_tm_wait = 0;
	m_time_out = 30;

	memset(m_bVisibleChnn, 0xFF, sizeof(m_bVisibleChnn));
	memset(&m_dispDev, 0, sizeof(DISPLAY_DEV));

	m_posPercentX = 62;
	m_posPercentY = 53;
	m_posPercentCx = 33;
	m_posPercentCy = 33;

	memset(m_prev_well_chnnData, 0, sizeof(unsigned long) * DISPLAY_INDEX_MAX);
	m_bDisplayChanged = false;
}

CDisplayMan::~CDisplayMan()
{

}

bool CDisplayMan::Initial(unsigned char localVideoInputNum,unsigned char videoInputNum, unsigned char videoOutNum,unsigned char netVideoInputNum)
{
	m_lock.Lock();

	m_bEnable = false;
	m_bPause = false;
	m_group = 0;
	m_tmCount = 0;

	m_bHaveSpot = CLocalDevice::Instance()->HaveSpot();
	m_tmCountSpot = 0;
	m_groupSpot = 0;

	////
	m_localVideoInputNum = localVideoInputNum;
	m_videoInputNum = videoInputNum;
	m_videoOutNum = videoOutNum;
	m_netVideoInputNum = netVideoInputNum;

	m_bInit = true;

	m_lock.UnLock();

	return true;
}

void CDisplayMan::Quit()
{
	m_lock.Lock();

	m_bEnable = false;
	m_bPause = false;
	m_group = 0;
	m_tmCount = 0;

	for(int i=0; i<SPLIT_COUNT; i++)
	{
		m_splitDataLen[i] = 0;
		if(NULL != m_pSplitData[i])
		{
			delete [] m_pSplitData[i];
			m_pSplitData[i] = NULL;
		}
	}

	m_dwellTimeDataLen = 0;
	if(NULL != m_pDwellTimeData)
	{
		delete [] m_pDwellTimeData;
		m_pDwellTimeData = NULL;
	}

	m_tmCountSpot = 0;
	m_groupSpot = 0;

	if(m_pSplitDataSpot)
	{
		delete [] m_pSplitDataSpot;
		m_pSplitDataSpot = NULL;
	}
	m_splitDataLenSpot = 0;

	m_activeData = -1;

	if(m_pTmpData)
	{
		delete [] m_pTmpData;
		m_pTmpData = NULL;
	}
	m_tmpDataLen = 0;

	m_display_history.RemoveAll();

	while(!m_display_stack.empty())
	{
		m_display_stack.pop();
	}

	m_lock.UnLock();
}

void CDisplayMan::SetDisplayDevInfo(const DISPLAY_DEV_INFO* pDispDevInfo, int num)
{
	assert(num > 0);
	assert(num <= DISPLAY_DEV_MAX);

	m_lock.Lock();

	memset(&m_dispDev, 0, sizeof(DISPLAY_DEV));

	m_dispDev.num = static_cast<unsigned long>(num);
	memcpy(&m_dispDev.devInfo, pDispDevInfo, sizeof(DISPLAY_DEV_INFO) * num);

	for(int i = 0; i < num; i++)
	{
		if((0 != m_dispDev.devInfo[i].bValid) && (DISPLAY_DEV_MAJOR == m_dispDev.devInfo[i].devMode))
		{
			m_dispDev.pMajorDev = &m_dispDev.devInfo[i];
		}
	}

	assert(m_dispDev.pMajorDev);

	m_bInitResolution = true;

	m_lock.UnLock();
}

void CDisplayMan::SetUnInitial()
{
	m_lock.Lock();

	//清空残留的记录
	m_display_history.RemoveAll();

	while(!m_display_stack.empty())
	{
		m_display_stack.pop();
	}

	//复位跳台
	m_bEnable = false;

#ifndef __ENVIRONMENT_LINUX__
	//清空显示屏幕
	memset(&m_display_info, 0, sizeof(DISPLAY_INFO));
	ChangeDisplayFormat(m_dispDev.pMajorDev, m_display_info);
#endif

	//复位设备信息
	memset(&m_dispDev, 0, sizeof(DISPLAY_DEV));

	m_bInitResolution = false;
	m_lock.UnLock();
}

unsigned long CDisplayMan::ChangeDisplay(unsigned long* pSplitMode, unsigned long splitLen, unsigned long* pChnn, int chnnLen, unsigned long timeout, unsigned short bRec, unsigned long bNotify, bool bFilter, bool bCmd)
{
	/*
		通道参数格式 unsigned long[(层<<24)(信号<<16)(区域编号<<8)(通道)]
	*/
	unsigned long ret = ERR_OK;

	if (!m_bFullChannelDisplay && !bCmd)
	{
		return ret;
	}
		
	// 处理报警大画面引起的跳台失败
	m_bDisplayChanged = true;

	m_lock.Lock();
	ret = ChangeDisplayEx(pSplitMode, splitLen, pChnn, chnnLen, timeout, bRec, bNotify, bFilter);
	m_lock.UnLock();


	if (bCmd)
	{
		m_bFullChannelDisplay = true;
	}

	return ret;
}

unsigned long CDisplayMan::ChangeDisplay(unsigned long x, unsigned long y, unsigned long timeout, unsigned short bRec, unsigned long bNotify)
{
	unsigned long ret = ERR_OK;

	// 处理报警大画面引起的跳台失败
	m_bDisplayChanged = true;

	m_lock.Lock();
	if(m_bInit && m_bInitResolution)
	{
		if(((DISPLAY_FREEZE == m_time_out) && (DISPLAY_FREEZE == timeout)) || (DISPLAY_FREEZE != m_time_out))
		{
			if(m_display_history.GetCount() > 0)
			{
				//提供回退机制,切换的是上一次操作场景
				POS pos_header = m_display_history.GetHeadPosition();
				DISPLAY_INFO lately_old_disp = m_display_history.GetAt(pos_header);

				if(lately_old_disp != m_display_info)
				{
					//当前的画面没有被记录,直接回退到历史最后状态
					ret = ChangeDisplayFormat(m_dispDev.pMajorDev, lately_old_disp);
					if(ret == ERR_OK)
					{
						m_tm_wait = 0;
						m_time_out = timeout;
						m_display_info = lately_old_disp;
					}

					//printf("To back lastest chnn. Point x=%d,y=%d,split mode=0x%x.\n", x, y, m_display_info.split_mode);
				}
				else
				{
					unsigned long hitChnn[MAX_CHNN_NUM] = {0};

					int hits = HitTest(x, y, m_display_info, hitChnn, MAX_CHNN_NUM);
					if((hits > 0) && (hits < MAX_CHNN_NUM))
					{
						unsigned char z_order = 0;
						unsigned char hit_chnn = 0;
						unsigned char signal_chnn = 0;
						for (int i = 0; i < hits; i++)
						{
							if((DISPLAY_GET_LAYER(hitChnn[i]) == DISPLAY_LAYER_ANALOG) ||
								(DISPLAY_GET_LAYER(hitChnn[i]) == DISPLAY_LAYER_NET))
							{
								z_order = DISPLAY_GET_LAYER(hitChnn[i]);
								hit_chnn = DISPLAY_GET_CHNN(hitChnn[i]);
								signal_chnn = DISPLAY_GET_SIGNAL(hitChnn[i]);
								break;
							}
						}
						unsigned char validChnn = (signal_chnn == DISPLAY_SIGNAL_NET)?m_localVideoInputNum:0 ;

						if((z_order > 0) && (z_order <= DISPLAY_LAYER_MAX) && (hit_chnn < m_videoInputNum) && (m_bVisibleChnn[hit_chnn+validChnn] > 0))
						{
							//如果当前画面有记录,
							//偌处于多画面时,则进入相应的单画面;
							//偌处于单画面时,则检查倒数第二历史记录;偌本单画面是从倒数第二历史记录而来,
							//则回到倒数第二历史记录;偌不是,则回到最大画面
							if(m_display_info.layer[z_order].split_mode == VIEW_SPLIT_1X1)
							{
								//从单画面回退到上次操作的多画面
								POS pos = pos_header;
								m_display_history.GetNext(pos);
								DISPLAY_INFO second_old_disp = m_display_history.GetAt(pos);
								if((m_display_history.GetCount() >= 2) &&
									(second_old_disp.layer[z_order].split_mode != VIEW_SPLIT_1X1) &&
									(second_old_disp.time_out == lately_old_disp.time_out))
								{
									//是从上次历史记录而来
									ret = ChangeDisplayFormat(m_dispDev.pMajorDev, second_old_disp);
									if(ERR_OK == ret)
									{
										m_tm_wait = 0;
										m_time_out = timeout;
										m_display_info = second_old_disp;
									}

									//printf("To back laster chnn. Point x=%d,y=%d,split mode=0x%x.\n", x, y, m_display_info.split_mode);
								}
								else
								{
									//全画面
									unsigned long chnnValue[DISPLAY_INDEX_MAX] = {0};
									for(int i = 0; i < m_videoInputNum; i++)
									{
										if(i < m_localVideoInputNum)
										{
											chnnValue[i] = DISPLAY_PACK(DISPLAY_LAYER_ANALOG, DISPLAY_SIGNAL_ANALOG, i, i);
										}
										else
										{
											chnnValue[i] = DISPLAY_PACK(DISPLAY_LAYER_NET, DISPLAY_LAYER_NET, i, i - m_localVideoInputNum);
										}
									}
									unsigned char videoInputNum = IsSupportHybridDisplay()?m_videoInputNum:m_localVideoInputNum;
									ret = ChangeDisplayEx(NULL, 0, chnnValue, videoInputNum, m_display_info.time_out, m_display_info.record, 1);
	
								}
							}
							else
							{
								//单画面
								const DISPLAY_LAYER& layer = m_display_info.layer[z_order];
								unsigned long chnnValue = DISPLAY_PACK(layer.type, layer.signal_format, 0, hit_chnn);
								
								unsigned long splitMode[DISPLAY_LAYER_MAX] = {0};
								ret = ChangeDisplayEx(splitMode, DISPLAY_LAYER_MAX, &chnnValue, 1, m_display_info.time_out, m_display_info.record, 1);
							
								if((ERR_OK == ret) && (m_display_history.GetCount() >= 2))
								{
									m_display_history.RemoveHead();
								}
							}
						}
						else
						{
							printf("Ignore.Invalid pt.\n");
							ret = ERR_INVALID_PT;
						}
					}
				}
			}
			else
			{
				ret = ERR_HISTORY_EMPTY;
			}
		}
		else
		{
			printf("Freezed ChangeDisplay.\n");
			ret = ERR_FREEZE;//被冻结
		}
	}
	else
	{
		printf("Warning.Ignore ChangeDisplay behavior.Init CDisplayMan or Resolution first.\n");
		ret = ERR_UNINITIAL;
	}

	m_lock.UnLock();

	return ret;
}

unsigned long CDisplayMan::ChangeDisplay(unsigned long x1, unsigned long y1, unsigned long x2, unsigned long y2, unsigned long timeout, unsigned short bRec, unsigned long bNotify)
{
	/*
		通过两点, 改变画面次序
	*/
	unsigned long ret = ERR_OK;

	// 处理报警大画面引起的跳台失败
	m_bDisplayChanged = true;

	m_lock.Lock();
	if(m_bInit && m_bInitResolution)
	{
		if(((DISPLAY_FREEZE == m_time_out) && (DISPLAY_FREEZE == timeout)) || (DISPLAY_FREEZE != m_time_out))
		{
			unsigned long dataChnnFirst = 0, dataChnnSecond = 0;

			unsigned long hit[DISPLAY_INDEX_MAX];

			memset(hit, 0, sizeof(hit));
			int len = HitTest(x1, y1, m_display_info, hit, DISPLAY_INDEX_MAX);
			
			if(len > 0)
			{
				for(int i = 0; i < len; i++)
				{
					dataChnnFirst = hit[i]; //取处于最上层
				}
			}
			else
			{
				ret = ERR_INVALID_PT;
			}

			if(ERR_OK == ret)
			{
				memset(hit, 0, sizeof(hit));
				len = HitTest(x2, y2, m_display_info, hit, DISPLAY_INDEX_MAX);
			
				if(len > 0)
				{
					for(int i = 0; i < len; i++)
					{
						dataChnnSecond = hit[i]; //取处于最上层
					}
				}
				else
				{
					//目标处于无效区域, 则认为在同一个层交换
					int index = 0; 
					
					if(DISPLAY_LAYER_MINI != DISPLAY_GET_LAYER(dataChnnFirst))
					{
						index = AreaIndex(m_dispDev.pMajorDev->width, m_dispDev.pMajorDev->height, VIEW_SPLIT_MODE(m_display_info.layer[DISPLAY_GET_LAYER(dataChnnFirst)].split_mode), x2, y2, 0, 0);
					}
					else
					{
						index = AreaIndex(m_dispDev.pMajorDev->width, m_dispDev.pMajorDev->height, VIEW_SPLIT_MODE(m_display_info.layer[DISPLAY_GET_SIGNAL(dataChnnFirst)].split_mode), x2, y2, 0, 0);
					}

					if(index < 0)
					{
						index = 0;
						printf("Error.Hit pt x=%d,y=%d.\n", x2, y2);
					}

					dataChnnSecond = DISPLAY_PACK(DISPLAY_GET_LAYER(dataChnnFirst), DISPLAY_GET_SIGNAL(dataChnnFirst), index, DISPLAY_INVALID);
				}

				if(dataChnnFirst != dataChnnSecond)
				{
					DISPLAY_INFO dispInfo = m_display_info;
					dispInfo.time_out = timeout;
					dispInfo.record = bRec;

					unsigned char layerFirst = DISPLAY_GET_LAYER(dataChnnFirst);
					unsigned char indexFirst = DISPLAY_GET_INDEX(dataChnnFirst);

					unsigned char layerSecond = DISPLAY_GET_LAYER(dataChnnSecond);
					unsigned char indexSecond = DISPLAY_GET_INDEX(dataChnnSecond);

					if(DISPLAY_INVALID != dispInfo.layer[layerFirst].area_chnn[indexFirst])
					{
						dispInfo.layer[layerFirst].num_chnn--;
					}

					if(DISPLAY_INVALID != dispInfo.layer[layerSecond].area_chnn[indexSecond])
					{
						dispInfo.layer[layerSecond].num_chnn--;
					}

					dispInfo.layer[layerFirst].area_chnn[indexFirst] = DISPLAY_INVALID;
					dispInfo.layer[layerSecond].area_chnn[indexSecond] = DISPLAY_INVALID;


					if((layerFirst != DISPLAY_LAYER_MINI) && (layerSecond != DISPLAY_LAYER_MINI))
					{
						//不与缩微图交换, 存在不同层间交换通道
						dispInfo.layer[layerFirst].area_chnn[indexSecond] = DISPLAY_GET_CHNN(dataChnnFirst);
						dispInfo.layer[layerSecond].area_chnn[indexFirst] = DISPLAY_GET_CHNN(dataChnnSecond);
						
						if(DISPLAY_INVALID != DISPLAY_GET_CHNN(dataChnnFirst))
						{
							dispInfo.layer[layerFirst].num_chnn++;
						}

						if(DISPLAY_INVALID != DISPLAY_GET_CHNN(dataChnnSecond))
						{
							dispInfo.layer[layerSecond].num_chnn++;
						}
					}
					else
					{
						//缩微图本身的信号格式 决定它回到哪个图层
						if(layerFirst == DISPLAY_LAYER_MINI)
						{
							dispInfo.layer[layerFirst].area_chnn[indexFirst] = DISPLAY_GET_CHNN(dataChnnSecond);
							dispInfo.layer[layerFirst].signal_format = DISPLAY_GET_SIGNAL(dataChnnSecond);

							if(DISPLAY_INVALID != dispInfo.layer[layerFirst].area_chnn[indexFirst])
							{
								dispInfo.layer[layerFirst].num_chnn++;
							}

							unsigned char signal_format = DISPLAY_GET_SIGNAL(dataChnnFirst);
							dispInfo.layer[signal_format].area_chnn[indexSecond] = DISPLAY_GET_CHNN(dataChnnFirst);
							if(DISPLAY_INVALID != dispInfo.layer[signal_format].area_chnn[indexSecond])
							{
								dispInfo.layer[signal_format].num_chnn++;
							}
						}
						else if(layerSecond == DISPLAY_LAYER_MINI)
						{
							dispInfo.layer[layerSecond].area_chnn[indexSecond] = DISPLAY_GET_CHNN(dataChnnFirst);
							dispInfo.layer[layerSecond].signal_format = DISPLAY_GET_SIGNAL(dataChnnFirst);

							if(DISPLAY_INVALID != dispInfo.layer[layerSecond].area_chnn[indexSecond])
							{
								dispInfo.layer[layerSecond].num_chnn++;
							}

							unsigned char signal_format = DISPLAY_GET_SIGNAL(dataChnnSecond);
							dispInfo.layer[signal_format].area_chnn[indexFirst] = DISPLAY_GET_CHNN(dataChnnSecond);
							if(DISPLAY_INVALID != dispInfo.layer[signal_format].area_chnn[indexFirst])
							{
								dispInfo.layer[signal_format].num_chnn++;
							}
						}
						else
						{
							printf("Error.Hit pt x1=%d,y1=%d. x2=%d,y2=%d.\n", x1, y1, x2, y2);
							assert(false);
						}
					}

					dispInfo.num_attrib = 0;
					for(int i = 0; i < DISPLAY_LAYER_MAX; i++)
					{
						dispInfo.num_attrib += dispInfo.layer[i].num_chnn;
					}
					dispInfo.num_attrib -= dispInfo.layer[DISPLAY_LAYER_MINI].num_chnn;

					//计算dispInfo.display_attrib
					memset(dispInfo.display_attrib, 0 , sizeof(dispInfo.display_attrib));
					CalculateDisplayInfo(m_dispDev.pMajorDev, dispInfo);

					//改变设备显示
					ret = ChangeDisplayFormat(m_dispDev.pMajorDev, dispInfo, bNotify);

					if(ERR_OK == ret)
					{
						//判断是否进入冻结状态.保存当前显示
						if((DISPLAY_FREEZE == timeout) && (DISPLAY_FREEZE != m_time_out))
						{
							PushDisplay();
						}

						m_tm_wait = 0;
						m_time_out = timeout;
						m_display_info = dispInfo;
					}
				}
			}
		}
		else
		{
			printf("Freezed ChangeDisplay.\n");
			ret = ERR_FREEZE;//被冻结
		}
	}
	else
	{
		printf("Warning.Ignore ChangeDisplay behavior.Init CDisplayMan or Resolution first.\n");
		ret = ERR_UNINITIAL;
	}
	m_lock.UnLock();

	return ret;
}

void CDisplayMan::SetMiniWndPos(unsigned long percentX, unsigned long percentY, unsigned long percentCx, unsigned long percentCy)
{
	m_lock.Lock();

	unsigned long per_x = (percentX > 100) ? m_posPercentX : percentX;
	unsigned long per_y = (percentY > 100) ? m_posPercentY : percentY;
	unsigned long per_cx = (percentCx > 100) ? m_posPercentCx : percentCx;
	unsigned long per_cy = (percentCy > 100) ? m_posPercentCy : percentCy;

	if(per_x + per_cx > 100)
	{
		per_x = 100 - per_cx;
	}

	if(per_y + per_cy > 100)
	{
		per_y = 100 - per_cy;
	}

	if((m_posPercentX == per_x) && (m_posPercentY == per_y) && (m_posPercentCx == per_cx) && (m_posPercentCy == per_cy))
	{
		//无变化
		m_lock.UnLock();
		return;
	}

	m_posPercentX = per_x;
	m_posPercentY = per_y;
	m_posPercentCx = per_cx;
	m_posPercentCy = per_cy;

	if(m_bInit && m_bInitResolution)
	{
		unsigned short oldRecord = m_display_info.record;
		DISPLAY_INFO dispInfo = m_display_info;
		dispInfo.record = DISPLAY_UNRECORD;

		//计算dispInfo.display_attrib
		memset(dispInfo.display_attrib, 0 , sizeof(dispInfo.display_attrib));
		CalculateDisplayInfo(m_dispDev.pMajorDev, dispInfo);

		//改变设备显示
		unsigned long ret = ChangeDisplayFormat(m_dispDev.pMajorDev, dispInfo, false);

		if(ERR_OK == ret)
		{
			m_tm_wait = 0;
			m_display_info = dispInfo;
			m_display_info.record = oldRecord;
		}
	}

	m_lock.UnLock();
}

unsigned long CDisplayMan::GetDisplayInfo(NET_DISPLAY_INFO& outDisplay, unsigned short x, unsigned short y, bool bUsePt)
{
	unsigned long count = 0;

	m_lock.Lock();

	outDisplay.rec = m_display_info.record;
	outDisplay.time_out = m_display_info.time_out;

	for(int i = 0; i < DISPLAY_LAYER_MAX; i++)
	{
		outDisplay.split_mode[i] = m_display_info.layer[i].split_mode;
	}

	if(bUsePt)
	{
		outDisplay.type = NET_DISPLAY_REPLY_HITS;
		outDisplay.data_len = HitTest(x, y, m_display_info, outDisplay.data, sizeof(outDisplay.data) / sizeof(unsigned long));
		count = outDisplay.data_len;
	}
	else
	{
		outDisplay.type = NET_DISPLAY_REPLY_ALL;
		unsigned char validChnn = 0;
		unsigned char validSignal = 0;
		for(int i = 0; i < DISPLAY_LAYER_MAX; i++)
		{
			if(DISPLAY_SIGNAL_NULL != m_display_info.layer[i].signal_format)
			{
				for (int j = 0; j < DISPLAY_INDEX_MAX; j++)
				{
					unsigned char chnn = m_display_info.layer[i].area_chnn[j];
					unsigned char Channel[DISPLAY_INDEX_MAX];
					bool DwellFlag = true;
					if((DISPLAY_INVALID != chnn) && (chnn < DISPLAY_INDEX_MAX))
					{
						Channel[count] = chnn;
						outDisplay.data[count++] = DISPLAY_PACK(m_display_info.layer[i].type, m_display_info.layer[i].signal_format, j, chnn);
						validChnn = count;
						validSignal = i;
					}
					else
					{
						if((DISPLAY_INVALID == chnn)&&((g_net_channel >> j) & 0x01)&&(count < SplitModeToNum(static_cast<VIEW_SPLIT_MODE>(m_display_info.layer[validSignal].split_mode))))
						{
							for(unsigned char temp = 0;temp<count;temp ++)
							{
								if(Channel[temp] == j-m_localVideoInputNum)
									DwellFlag =false;

							}
							if(DwellFlag)
							{
								outDisplay.data[count++] = DISPLAY_PACK(DISPLAY_LAYER_NET, DISPLAY_SIGNAL_NET, validChnn++, j-m_localVideoInputNum);
							}
						}
					}
				}
			}
		}
		outDisplay.data_len = count;
	}

	m_lock.UnLock();

	return count;
}

int CDisplayMan::HitChnn(unsigned short x, unsigned short y, unsigned long* pHits, int bufflen)
{
	int count = 0;

	m_lock.Lock();

	count = HitTest(x, y, m_display_info, pHits, bufflen);

	m_lock.UnLock();

	return count;
}

unsigned long CDisplayMan::FreezeDisplay()
{
	unsigned long ret = ERR_OK;

	m_lock.Lock();

	if(m_bInit && m_bInitResolution)
	{
		PushDisplay();
		m_time_out = -1;
	}
	else
	{
		ret = ERR_UNINITIAL;
	}

	m_lock.UnLock();

	return ret;
}

unsigned long CDisplayMan::PopDisplay()
{
	unsigned long ret= ERR_OK;
	m_lock.Lock();
	if(m_bInit && m_bInitResolution)
	{
		if(!m_display_stack.empty())
		{
			DISPLAY_INFO disp;
			disp = m_display_stack.top();
			ret = ChangeDisplayFormat(m_dispDev.pMajorDev, disp);

			if(ret == ERR_OK)
			{
				m_display_stack.pop();
				m_tm_wait = 0;
				m_time_out = disp.time_out;
				m_display_info = disp;

				//[2010-07-19 L]出栈的显示信息在历史记录中写两份
				//以免双击切换检查切换的无效通道
				if(0 != disp.record)
				{
					if(m_display_history.GetCount() >= m_max_history)
					{
						m_display_history.RemoveTail();
					}
					m_display_history.AddHead(disp);
				}
			}
		}
		else
		{
			//assert(false);	//只支持一次冻结
			ret = ERR_STACK_EMPTY;
		}
	}
	else
	{
		printf("Warning.Ignore ChangeDisplay behavior.Init CDisplayMan or Resolution first.\n");
		ret = ERR_UNINITIAL;
	}
	m_lock.UnLock();

	return ret;
}

void CDisplayMan::PushDisplay()
{
	//m_lock.Lock();
	if(m_bInit && m_bInitResolution)
	{
		assert(DISPLAY_FREEZE != m_time_out); //确保处于非冻结状态

		//[2010-07-19 L]压栈的显示信息必须可以写记录
		//遗漏情况:大画面报警、回放、鼠标双击 -- 黑屏[切换的是回放通道]
		unsigned short bRec = m_display_info.record;
		m_display_info.record = 0x1;
		m_display_stack.push(m_display_info);
		m_display_info.record = bRec;
		assert(m_display_stack.size() <= m_max_stack);	
	}
	else
	{
		printf("Warning.Ignore ChangeDisplay behavior.Init CDisplayMan or Resolution first.\n");
	}
	//m_lock.UnLock();
}

int CDisplayMan::HitTest(unsigned short x, unsigned short y, const DISPLAY_INFO& dispinfo, unsigned long* pHit, int len)
{
	int hit = 0;

	for(int i = 0; i < dispinfo.num_attrib; i++)
	{
		if( (dispinfo.display_attrib[i].bShow != DISPLAY_NONE) &&
			(dispinfo.display_attrib[i].xPos <= x) &&
			(dispinfo.display_attrib[i].yPos <= y) &&
			(dispinfo.display_attrib[i].xPos + dispinfo.display_attrib[i].cx >= x) &&
			(dispinfo.display_attrib[i].yPos + dispinfo.display_attrib[i].cy >= y))
		{
			if(hit < len)
			{
				unsigned long z_order = dispinfo.display_attrib[i].zOrder;
				unsigned long signal_format = dispinfo.display_attrib[i].bShow;
				unsigned long index = dispinfo.display_attrib[i].index;
				unsigned long chnn = dispinfo.display_attrib[i].chnn;

				pHit[hit++] = (z_order << 24) | (signal_format << 16) | (index << 8) | chnn;
			}
		}
	}

	return hit;
}

void CDisplayMan::CalculateDisplayInfo(const DISPLAY_DEV_INFO* pDevInfo, DISPLAY_INFO& inOutDisplay)
{
	int off_attrib = 0;

	for(int i = 0; i < DISPLAY_LAYER_MAX; i++)
	{
		int layer_num_chnn = 0;
		for (int j = 0; j < DISPLAY_INDEX_MAX; j++)
		{
			if(layer_num_chnn >= inOutDisplay.layer[i].num_chnn)
			{
				break;
			}

			unsigned char chnn = inOutDisplay.layer[i].area_chnn[j];
			if((0 != inOutDisplay.layer[i].signal_format) && (DISPLAY_INVALID != chnn) && (DISPLAY_INDEX_MAX > chnn))
			{
				if(DISPLAY_LAYER_MINI != i)
				{
					if (VIDEO_OUT_CVBS == pDevInfo->videoType && g_adjustCvbsVO)
					{
						unsigned short w = 0;
						unsigned short h = 0;
						GetCVBSResolution(g_video_format, w, h, true);
						unsigned short offsetW = (pDevInfo->width - w) / 2;
						unsigned short offsetH = (pDevInfo->height - h) / 2;
						offsetW = 0;
						offsetH = 0;
						//printf("%s:%s:%d, w=%d, h=%d, width=%d, height=%d\n", __FUNCTION__, __FILE__, __LINE__, w, h, pDevInfo->width, pDevInfo->height);
						AreaInfoEx(offsetW, offsetH, w, h, static_cast<VIEW_SPLIT_MODE>(inOutDisplay.layer[i].split_mode), inOutDisplay.display_attrib[off_attrib], j);
					}
					else
					{
						AreaInfo(pDevInfo->width, pDevInfo->height, static_cast<VIEW_SPLIT_MODE>(inOutDisplay.layer[i].split_mode), inOutDisplay.display_attrib[off_attrib], j);
					}
				}
				else
				{
					if (VIDEO_OUT_CVBS == pDevInfo->videoType && g_adjustCvbsVO)
					{
						unsigned short w = 0;
						unsigned short h = 0;
						GetCVBSResolution(g_video_format, w, h, true);

						unsigned short offsetW = (pDevInfo->width - w) / 2;
						unsigned short offsetH = (pDevInfo->height - h) / 2;

						//缩微图层,独立计算
						unsigned long x = (double(m_posPercentX * w) / 100 + double(0.5));//pDevInfo->width * 2 / 3 - 40;
						unsigned long y = (double(m_posPercentY * h) / 100 + double(0.5));//pDevInfo->height * 2 / 3 - 68;

						unsigned long cx = (double(m_posPercentCx * w) / 100 + double(0.5));//pDevInfo->width / 3;
						unsigned long cy = (double(m_posPercentCy * h) / 100 + double(0.5));//pDevInfo->height / 3;
#if defined(__CHIPTI__)
						//TI产品画中画要求8像素对齐
						x += 7;
						x &= ~7;

						cx += 7;
						cx &= ~7;
#endif

						offsetW = 0;
						offsetH = 0;
						//printf("%s:%s:%d, w=%d, h=%d, width=%d, height=%d\n", __FUNCTION__, __FILE__, __LINE__, w, h, pDevInfo->width, pDevInfo->height);
						AreaInfoEx(offsetW, offsetH, cx, cy, static_cast<VIEW_SPLIT_MODE>(inOutDisplay.layer[i].split_mode), inOutDisplay.display_attrib[off_attrib], j, x, y);
					}
					else
					{
						//缩微图层,独立计算
						unsigned long x = (double(m_posPercentX * pDevInfo->width) / 100 + double(0.5));//pDevInfo->width * 2 / 3 - 40;
						unsigned long y = (double(m_posPercentY * pDevInfo->height) / 100 + double(0.5));//pDevInfo->height * 2 / 3 - 68;

						unsigned long cx = (double(m_posPercentCx * pDevInfo->width) / 100 + double(0.5));//pDevInfo->width / 3;
						unsigned long cy = (double(m_posPercentCy * pDevInfo->height) / 100 + double(0.5));//pDevInfo->height / 3;
#if defined(__CHIPTI__)
						//TI产品画中画要求8像素对齐
						x += 7;
						x &= ~7;

						cx += 7;
						cx &= ~7;
#endif
						AreaInfo(cx, cy, static_cast<VIEW_SPLIT_MODE>(inOutDisplay.layer[i].split_mode), inOutDisplay.display_attrib[off_attrib], j, x, y);
					}
				}

				inOutDisplay.display_attrib[off_attrib].bShow = inOutDisplay.layer[i].signal_format;
				inOutDisplay.display_attrib[off_attrib].chnn = chnn;
				inOutDisplay.display_attrib[off_attrib].zOrder = inOutDisplay.layer[i].z_order;
				inOutDisplay.display_attrib[off_attrib].index = j;

				off_attrib++;
				layer_num_chnn++;
			}
		}
	}

	inOutDisplay.num_attrib = off_attrib;
}

unsigned long CDisplayMan::ChangeDisplayEx(unsigned long* pSplitMode, unsigned long splitLen, unsigned long* pChnn, int chnnLen, unsigned long timeout/* = 30*/, unsigned short bRec/* = 0x1*/, unsigned long bNotify/* = 0*/, bool bFilter/* = false*/)
{
	unsigned long ret = ERR_OK;
	if(m_bInit && m_bInitResolution)
	{
		if(((DISPLAY_FREEZE == m_time_out) && (DISPLAY_FREEZE == timeout)) || (DISPLAY_FREEZE != m_time_out))
		{
			DISPLAY_INFO dispInfo;
			dispInfo.time_out = timeout;
			dispInfo.record = bRec;

			//初始化结构参数
			unsigned char num_chnn[DISPLAY_LAYER_MAX] = {0};
			unsigned long total_chnn = 0;
			unsigned long index_chnn_max[DISPLAY_LAYER_MAX] = {0};
			for(int i = 0; i < chnnLen; i++)
			{
				unsigned long value = pChnn[i];
				for (int j = 0; j < DISPLAY_LAYER_MAX; j++)
				{
					if(dispInfo.layer[j].type == DISPLAY_GET_LAYER(value))
					{
						unsigned char index = DISPLAY_GET_INDEX(value);
						unsigned char chnn = DISPLAY_GET_CHNN(value);
						if((DISPLAY_INVALID != index) && (DISPLAY_INDEX_MAX > index) &&
							(DISPLAY_INVALID != chnn) && (DISPLAY_INDEX_MAX > chnn))
						{
							dispInfo.layer[j].area_chnn[index] = chnn;
							num_chnn[j]++;
							total_chnn++;

							if(index > index_chnn_max[j])
							{
								index_chnn_max[j] = index;
							}
						}

						dispInfo.layer[j].signal_format = DISPLAY_GET_SIGNAL(value);
					}
				}
			}

			//分割模式
			total_chnn -= num_chnn[DISPLAY_LAYER_MINI];
			for (int i = 0; i < DISPLAY_LAYER_MAX; i++)
			{
				unsigned long split_mode = 0;

				//缩微图层独立算通道，其他合在一起
				unsigned long maxChnn = total_chnn;
				if(i >= DISPLAY_LAYER_MINI)
				{
					maxChnn = num_chnn[DISPLAY_LAYER_MINI];
				}

				if(i < splitLen)
				{
					split_mode = CheckSplitMode(pSplitMode[i], maxChnn);
				}
				else
				{
					split_mode = CheckSplitMode(0, maxChnn);
				}

				dispInfo.layer[i].split_mode = split_mode;
				dispInfo.layer[i].num_chnn = num_chnn[i];

				//2010-11-22 外部传来的分割模式和分割数据有错误[不匹配]
				unsigned long split_max_num = SplitModeToNum(static_cast<VIEW_SPLIT_MODE>(split_mode));
				if((index_chnn_max[i] + 1) > split_max_num)
				{
					//说明此时有超过分割模式对应的通道,需要重新调整超过分割模式之外的通道
					for(int k = split_max_num; k < DISPLAY_INDEX_MAX; k++)
					{
						unsigned char value_chnn = dispInfo.layer[i].area_chnn[k];
						dispInfo.layer[i].area_chnn[k] = DISPLAY_INVALID;
						if(DISPLAY_INDEX_MAX > value_chnn)
						{
							for(int n = 0; n < split_max_num; n++)
							{
								if(dispInfo.layer[i].area_chnn[n] >= DISPLAY_INDEX_MAX)
								{
									dispInfo.layer[i].area_chnn[n] = value_chnn;
									break;
								}
							}
						}
					}
				}
			}
			assert(dispInfo.layer[DISPLAY_LAYER_MINI].num_chnn <= 1);


			//计算dispInfo.display_attrib
			CalculateDisplayInfo(m_dispDev.pMajorDev, dispInfo);
			if(0 == dispInfo.num_attrib)
			{
				UpdateInvalidDwellTime();
				
			}
			//改变设备显示
			ret = ChangeDisplayFormat(m_dispDev.pMajorDev, dispInfo, bNotify, bFilter);

			if(ERR_OK == ret)
			{
				//判断是否进入冻结状态.保存当前显示
				if((DISPLAY_FREEZE == timeout) && (DISPLAY_FREEZE != m_time_out))
				{
					PushDisplay();
				}

				m_tm_wait = 0;
				m_time_out = timeout;
				m_display_info = dispInfo;

				//特殊处理,现场过来的1通道,保证单击通道回最大通道,向记录中多放一个
				if(((VIEW_SPLIT_1X1 == dispInfo.layer[DISPLAY_LAYER_ANALOG].split_mode) && (DISPLAY_UNNOTIFY != bRec)) ||
					((VIEW_SPLIT_1X1 == dispInfo.layer[DISPLAY_LAYER_NET].split_mode) && (DISPLAY_UNNOTIFY != bRec)))
				{
					if( m_display_history.GetCount() >= m_max_history )
					{
						m_display_history.RemoveTail();
					}
					m_display_history.AddHead(dispInfo);
				}
			}
		}
		else
		{
			printf("Freezed ChangeDisplay.\n");
			ret = ERR_FREEZE;//被冻结
		}
	}
	else
	{
		printf("Warning.Ignore ChangeDisplay behavior.Init CDisplayMan or Resolution first.\n");
		ret = ERR_UNINITIAL;
	}

	return ret;
}

unsigned long CDisplayMan::ChangeDisplayFormat(const DISPLAY_DEV_INFO* pDevInfo, DISPLAY_INFO& display, unsigned long bNotify, bool bFilter)
{
	/*
		修改主设备显示信息
		参数: 设备信息, 显示参数, 是否通知刷新, 是否过滤输出无变化的请求
	*/
	unsigned long ret = ERR_INVALID_CHNN;

	for (int i = 0; i < DISPLAY_LAYER_MAX; i++)
	{
		if(display.layer[i].num_chnn > 0)
		{
			ret = ERR_OK;
			break;
		}
	}

	//如果输出无变化,则跳出,过滤请求;
	//[2011-02-14 L],不判断来源则导致,用户要求刷新得不到响应
	//仅跳台时,做检查
	if(bFilter && (!(m_display_info != display)))
	{
		//如果过滤、显示信息一样
		//则什么也不做, 即不去修改物理设备
		//如果返回ERR_DISP_DATA_SAME错误,可能造成后续的逻辑错误
	}
	else
	{
		if(ERR_OK == ret)
		{
			unsigned long splitMode[DISPLAY_LAYER_MAX] = {0};
			for(int k = 0; k < DISPLAY_LAYER_MAX; k++)
			{
				splitMode[k] = display.layer[k].split_mode;
			}

			//FD 产品要求先修改VGA, 再修改CVBS
			if(VIDEO_OUT_CVBS == pDevInfo->videoType)
			{
				for (int i = 0; i < m_dispDev.num; i++)
				{
					if(VIDEO_OUT_CVBS != m_dispDev.devInfo[i].videoType)
					{
						DISPLAY_INFO dispInfo = display;
						memset(dispInfo.display_attrib, 0 , sizeof(dispInfo.display_attrib));

						CalculateDisplayInfo(&(m_dispDev.devInfo[i]), dispInfo);

						CLocalDevice::Instance()->ChangeDisplayMode(&(m_dispDev.devInfo[i]), dispInfo.display_attrib, dispInfo.num_attrib, splitMode, DISPLAY_LAYER_MAX, m_bVisibleChnn, bFilter);
					}
				}

				CLocalDevice::Instance()->ChangeDisplayMode(pDevInfo, display.display_attrib, display.num_attrib, splitMode, DISPLAY_LAYER_MAX, m_bVisibleChnn, bFilter);
			}
			else
			{	
				CLocalDevice::Instance()->ChangeDisplayMode(pDevInfo, display.display_attrib, display.num_attrib, splitMode, DISPLAY_LAYER_MAX, m_bVisibleChnn, bFilter);
				for (int i = 0; i < m_dispDev.num; i++)
				{
					if(VIDEO_OUT_CVBS == m_dispDev.devInfo[i].videoType)
					{
						DISPLAY_INFO dispInfo = display;
						memset(dispInfo.display_attrib, 0 , sizeof(dispInfo.display_attrib));
						
						CalculateDisplayInfo(&(m_dispDev.devInfo[i]), dispInfo);
						
						CLocalDevice::Instance()->ChangeDisplayMode(&(m_dispDev.devInfo[i]), dispInfo.display_attrib, dispInfo.num_attrib, splitMode, DISPLAY_LAYER_MAX, m_bVisibleChnn, bFilter);
					}
				}
			}
		}
		else
		{
			DISPLAY_ATTRIB DispEx;
			DispEx.bShow =DISPLAY_HIDE;
			DispEx.chnn=0;
			DispEx.xPos=0;
			DispEx.yPos=0;
			GetVGAResolution(g_resolution, DispEx.cx, DispEx.cy);
			#ifdef __ENVIRONMENT_LINUX__
			ext_change_display_format(&DispEx, 1);
			#endif
		}
	}

	if((ERR_OK == ret) && (DISPLAY_UNRECORD != display.record) && (DISPLAY_DEV_MAJOR == pDevInfo->devMode))
	{
		//修改成功,添加到记录列表
		if(m_display_history.GetCount() >= m_max_history)
		{
			m_display_history.RemoveTail();
		}
		m_display_history.AddHead(display);
	}

	if((ERR_OK == ret) && (DISPLAY_DEV_MAJOR == pDevInfo->devMode))
	{
		//修改显示成功则,通知修改刷新画面
		bool bChange = false;
		bChange = (0 == memcmp(m_display_info.display_attrib, display.display_attrib, sizeof(DISPLAY_ATTRIB) * MAX_CHNN_NUM)) ? false : true;

		if(bChange || bNotify)
		{
			NotifyRefreshDisplay(display);
		}
	}

#ifndef NDEBUG
	if(ERR_INVALID_CHNN == ret)
	{
		printf("Ignore change display request : all channels are invalid.\n");
	}
#endif

	return ret;
}

bool CDisplayMan::IsInvalidSplitMode(unsigned long splitMode)
{
	if((splitMode >= VIEW_SPLIT_1A11) && (splitMode <= VIEW_SPLIT_6X6))
	{
		return false;
	}

	if((splitMode >= VIEW_SPLIT_1A9) && (splitMode <= VIEW_SPLIT_5X5))
	{
		return false;
	}

	if((splitMode >= VIEW_SPLIT_2A6) && (splitMode <= VIEW_SPLIT_4X6))
	{
		return false;
	}

	if((splitMode >= VIEW_SPLIT_1A7) && (splitMode <= VIEW_SPLIT_4X4))
	{
		return false;
	}

	if((splitMode >= VIEW_SPLIT_1A5) && (splitMode <= VIEW_SPLIT_3X3))
	{
		return false;
	}

	if((splitMode >= VIEW_SPLIT_1A2) && (splitMode <= VIEW_SPLIT_2X3))
	{
		return false;
	}

	if(splitMode == VIEW_SPLIT_2X2)
	{
		return false;
	}

	if(splitMode == VIEW_SPLIT_1X1)
	{
		return false;
	}

	return true;
}

unsigned long CDisplayMan::CheckSplitMode(unsigned long splitMode, int numChnn)
{
	unsigned long retMode = 0x0;

	if(IsInvalidSplitMode(splitMode))
	{
		int split[] = {1, 4, 6, 9, 16, 25, 36};
		unsigned long split_mode[] = {VIEW_SPLIT_1X1, VIEW_SPLIT_2X2, VIEW_SPLIT_2X3, VIEW_SPLIT_3X3, VIEW_SPLIT_4X4, VIEW_SPLIT_5X5, VIEW_SPLIT_6X6};

		//由于存着10个通道要显示，这是应该是16分割画面，所以要从大至小地匹配
		//原来的算法是从小到大匹配，导致10通道却返回9分割。
		int num = (sizeof(split)/sizeof(int) - 1);
		for(int i = num; i >= 0; --i)
		{
			if(numChnn <= split[i])
			{
				retMode = split_mode[i];
			}
		}
	}
	else
	{
		if(1 == numChnn)
		{
			//SDK不支持只有一个通道的多分割模式
			retMode = VIEW_SPLIT_1X1;
		}
		else
		{
			retMode = splitMode;
		}
	}

	return retMode;
}

void CDisplayMan::NotifyRefreshDisplay(const DISPLAY_INFO& display)
{
	CMessageMan::Instance()->RefreshDisplay(0x1);
}

//////////////////////////////////////////////////////////////////////////
//SPOT,处理过程
//////////////////////////////////////////////////////////////////////////

void CDisplayMan::SetEnable(bool enable)
{
	m_lock.Lock();
	m_bEnable = enable;
	if(enable)
	{
		m_group = 0;
		m_dwellTime = 5;
		m_tmCount = -1;
		m_tm_wait = -1;

		m_bPause = false;

		m_bDisplayChanged = true;
	}
	printf("Set do dwell enable=%d.\n",enable);
	m_lock.UnLock();
}

bool CDisplayMan::IsEnable()
{
	bool enable;
	m_lock.Lock();
	enable = m_bEnable;
	m_lock.UnLock();
	return enable;
}

void CDisplayMan::Pause()
{
	m_lock.Lock();
	m_bPause = true;
	m_group -= 1;	//回退一组
	m_lock.UnLock();
}

bool CDisplayMan::IsPause()
{
	bool bPause;
	m_lock.Lock();
	bPause = m_bPause;
	m_lock.UnLock();
	return bPause;
}

void CDisplayMan::Resume()
{
	m_lock.Lock();
	m_bPause = false;
	m_tmCount = -1;//使超时,立即刷新数据一次
	m_tm_wait = -1;
	m_lock.UnLock();
}

void CDisplayMan::SetSplitMode(unsigned long splitMode )
{
	m_lock.Lock();
	if( m_splitMode != splitMode )
	{
		m_splitMode = splitMode;
		AttachActiveData();
		CalculateVisibleChnn();

		//立即刷新
		m_group = 0;
		m_dwellTime = 5;
		m_tmCount = -1;
		m_tm_wait = -1;
	}
	m_lock.UnLock();
}

void CDisplayMan::SetDwellTime(const unsigned long* pDwellTime, int len)
{
	m_lock.Lock();
	
	if(len > m_dwellTimeDataLen)
	{
		if(m_pDwellTimeData)
		{
			delete [] m_pDwellTimeData;
			m_pDwellTimeData = NULL;
		}

		m_dwellTimeDataLen = len;

		m_pDwellTimeData = new unsigned long[m_dwellTimeDataLen + 1];
	}

	memset(m_pDwellTimeData, 0, sizeof(unsigned long) * (m_dwellTimeDataLen + 1));
	memcpy(m_pDwellTimeData, pDwellTime, sizeof(unsigned long) * len);

	//立即刷新
	m_group = 0;
	m_dwellTime = 5;
	m_tmCount = -1;
	m_tm_wait = -1;

	m_lock.UnLock();
}

void CDisplayMan::SetSplitData(SPLIT split, const unsigned long* pData,int len)
{
	assert(split < SPLIT_COUNT);

	m_lock.Lock();

	if(NULL != m_pSplitData[split])
	{
		delete m_pSplitData[split];
		m_pSplitData[split] = NULL;
	}
	m_pSplitData[split] = new unsigned long[len];
	m_splitDataLen[split] = len;
	memcpy(m_pSplitData[split],pData,sizeof(unsigned long)*len);

	//调整临时数据区大小,保证临时数据区能容纳最大数据
	if(len > m_tmpDataLen)
	{
		if( m_pTmpData )
		{
			delete [] m_pTmpData;
			m_pTmpData = NULL;
		}
		m_tmpDataLen = len;
		m_pTmpData = new unsigned long[m_tmpDataLen];
	}

	CalculateVisibleChnn();

	//立即刷新
	m_group = 0;
	m_dwellTime = 5;
	m_tmCount = -1;
	m_tm_wait = -1;

	m_lock.UnLock();
}

void CDisplayMan::SetDwellTimeSpot(unsigned long dwellTime)
{
	m_lock.Lock();
	m_dwellTimeSpot = dwellTime;
	m_lock.UnLock();
}

void CDisplayMan::SetSplitDataSpot(SPLIT split, const unsigned long* pData, int len)
{
	assert(split == SPLIT_1X1); //只支持1x1

	m_lock.Lock();

	if(NULL != m_pSplitDataSpot)
	{
		delete m_pSplitDataSpot;
		m_pSplitDataSpot = NULL;
	}
	m_pSplitDataSpot = new unsigned long[len];
	m_splitDataLenSpot = len;
	memcpy(m_pSplitDataSpot, pData, sizeof(unsigned long)*len);

	m_groupSpot = 0;

	m_lock.UnLock();
}

void CDisplayMan::AttachActiveData()
{
	if((m_splitMode >= VIEW_SPLIT_1A11) && (m_splitMode <= VIEW_SPLIT_6X6))
	{
		m_activeData =SPLIT_6X6;
		return;
	}

	if((m_splitMode >= VIEW_SPLIT_1A9) && (m_splitMode <= VIEW_SPLIT_5X5))
	{
		m_activeData = SPLIT_5X5;
		return;
	}

	if((m_splitMode >= VIEW_SPLIT_2A6) && (m_splitMode <= VIEW_SPLIT_4X6))
	{
		m_activeData = SPLIT_4X6;
		return;
	}

	if((m_splitMode >= VIEW_SPLIT_1A7) && (m_splitMode <= VIEW_SPLIT_4X4))
	{
		m_activeData = SPLIT_4X4;
		return;
	}

	if((m_splitMode >= VIEW_SPLIT_1A5) && (m_splitMode <= VIEW_SPLIT_3X3))
	{
		m_activeData = SPLIT_3X3;
		return;
	}

	if((m_splitMode >= VIEW_SPLIT_1A2) && (m_splitMode <= VIEW_SPLIT_2X3))
	{
		m_activeData = SPLIT_2X3;
		return;
	}

	if(m_splitMode == VIEW_SPLIT_2X2)
	{
		m_activeData = SPLIT_2X2;
		return;
	}

	if(m_splitMode == VIEW_SPLIT_1X1)
	{
		m_activeData = SPLIT_1X1;
		return;
	}

	assert(false);
}

void CDisplayMan::CalculateVisibleChnn()
{
	unsigned long* pValue = m_pSplitData[m_activeData];

	if(pValue)
	{
		unsigned long count = *pValue++;						//总数据组数
		unsigned long maxGroup = *pValue++;						//有效组数
		unsigned long numGroup = SplitModeToNum(static_cast<VIEW_SPLIT_MODE>(m_splitMode));	//每组个数

		memset(m_bVisibleChnn, 0, sizeof(m_bVisibleChnn));
		for(int i = 0; i < numGroup * maxGroup; i++)
		{
			if(pValue[i] < m_videoInputNum)
			{
				m_bVisibleChnn[pValue[i]] = 1;
			}
		}
	}
}

void CDisplayMan::ProcessSpot()
{
	unsigned long group = 0;
	unsigned long* pData = NULL;
	m_lock.Lock();
	pData = m_pSplitDataSpot;
	if( pData )
	{
		unsigned long count = *pData++;		//总数据组数
		unsigned long maxGroup = *pData++; //

		if( m_groupSpot >= maxGroup || m_groupSpot < 0 )
		{
			m_groupSpot = 0;
		}
		group = m_groupSpot;
		//m_groupSpot += 1;

		////SPOT
		unsigned long ret = 0;
		int missCount = 0;
		while(1)
		{
			ret = ProcessSpot1X1(group, pData);

			if(ERR_OK != ret)
			{
				group ++;
				missCount ++;
			}
			else
			{
				m_groupSpot = group + 1;
				break;
			}

			if(missCount >= 36)
			{
				m_groupSpot += 1;
				break;
			}

			if(group >= maxGroup || group < 0)
			{
				group = 0;
			}
		}
	}

	m_lock.UnLock();
}

unsigned long CDisplayMan::ProcessSpot1X1(unsigned long group, unsigned long* pData)
{
	unsigned long chnn = pData[group];

	//只有 有效通道才发送Spot请求
	if(chnn < m_localVideoInputNum)
	{
		CLocalDevice::Instance()->ChangeDisplaySpot(chnn);
		return ERR_OK;
	}
	else
	{
		return ERR_INVALID_CHNN;
	}
}

//////////////////////////////////////////////////////////////////////////
//各种跳台操作,处理过程
//////////////////////////////////////////////////////////////////////////

unsigned long CDisplayMan::ProcessDwell()
{
	assert(m_activeData>=0);
	assert(m_activeData<SPLIT_COUNT);
	assert(m_pSplitData[m_activeData]);
	assert(m_splitDataLen[m_activeData]>0);

	m_lock.Lock();
	//选择哪种跳台
	unsigned long splitMode = m_splitMode;

	//拷贝当前使用的有效数据
	memcpy(m_pTmpData,m_pSplitData[m_activeData],sizeof(unsigned long)*m_splitDataLen[m_activeData]);
	unsigned long* pData = m_pTmpData;
	unsigned long count = *pData++;			//数据总组数
	unsigned long maxGroup = *pData++;		//本数据最大有效组数

	if((m_group >= maxGroup) || (m_group < 0))
	{
		m_group = 0;
	}
	unsigned long group = m_group;
	//m_group += 1;

	unsigned long ret = 0;
	int missCount = 0;
	while(1)
	{
		unsigned long dwellTime = m_pDwellTimeData[group];

		if(dwellTime != 0)
		{
			switch (splitMode)
			{
			case VIEW_SPLIT_1X1:
				ret = Process1X1(group,pData);
				break;
			case VIEW_SPLIT_2X2:
				ret = Process2X2(group,pData);
				break;
			case VIEW_SPLIT_2X3:
				ret = Process2X3(group,pData);
				break;
			case VIEW_SPLIT_3X3:
				ret = Process3X3(group,pData);
				break;
			case VIEW_SPLIT_4X4:
				ret = Process4X4(group,pData);
				break;
			case VIEW_SPLIT_4X6:
				ret = Process4X6(group,pData);
				break;
			case VIEW_SPLIT_5X5:
				ret = Process5X5(group,pData);
				break;
			case VIEW_SPLIT_1A11:
			case VIEW_SPLIT_1A20:
			case VIEW_SPLIT_4A20:
				assert(false);
				break;
			case VIEW_SPLIT_6X6:
				ret = Process6X6(group,pData);
				break;
			default:
				assert(false);
				break;
			}
		}
		else
		{
			ret = ERR_INVALID_CHNN;
		}

		if( ERR_OK != ret )
		{
			missCount++;
			group ++;
		}
		else
		{
			break;
		}

		if( missCount >= 36 )
		{
			break;
		}

		if((group >= maxGroup) || (group < 0))
		{
			group = 0;
		}
	}

	//设置跳台等待时间, 默认5秒
	m_dwellTime = 5;
	if(ERR_OK == ret)
	{
		if(m_pDwellTimeData && (m_dwellTimeDataLen > group) && (group >= 0))
		{
			m_dwellTime = m_pDwellTimeData[group];
		}
	}

	m_group = group + 1;

	if((m_group >= maxGroup) || (m_group < 0))
	{
		m_group = 0;
	}

	m_lock.UnLock();

	return ret;
}

unsigned long CDisplayMan::ProcessNXN(int n, unsigned long group, unsigned long* pData)
{
	unsigned long ret = ERR_INVALID_CHNN;
	unsigned long* pChnn = reinterpret_cast<unsigned long*>(pData + n * group);

	unsigned long chnnData[DISPLAY_INDEX_MAX];
	memset(chnnData, 0, sizeof(unsigned long) * DISPLAY_INDEX_MAX);

	int index = 0;
	int validChnn = 0;
	for (int i = 0; i < n; i++)
	{
		if(pChnn[i] < m_videoInputNum)
		{
			if(pChnn[i] < m_localVideoInputNum)
			{
				chnnData[index] = DISPLAY_PACK(DISPLAY_LAYER_ANALOG, DISPLAY_SIGNAL_ANALOG, i, pChnn[i]);
				validChnn++;
			}
			else
			{
				if((g_net_channel >> pChnn[i]) & 0x01)
				{
					chnnData[index] = DISPLAY_PACK(DISPLAY_LAYER_NET, DISPLAY_SIGNAL_NET, i, pChnn[i] - m_localVideoInputNum);
					validChnn++;
				}
			}
			index++;
		}
	}
	if(validChnn == 0)
	{
		UpdateInvalidDwellTime();
		return ERR_OK;
	}

	if( 0 == memcmp( chnnData, m_prev_well_chnnData, sizeof(unsigned long)*DISPLAY_INDEX_MAX) )
	{
		if( !m_bDisplayChanged )
		{
			return ERR_OK;
		}
		m_bDisplayChanged = false;
	}

	bool bNeedChangeNetDisplay = false;
	if (index >= m_videoInputNum)
	{
		bNeedChangeNetDisplay = true;
	}

	memcpy( m_prev_well_chnnData, chnnData, sizeof(unsigned long)*DISPLAY_INDEX_MAX );

	if(index > 0)
	{
		unsigned long splitMode[DISPLAY_LAYER_MAX] = {0};
		splitMode[DISPLAY_LAYER_ANALOG] = m_splitMode;
		splitMode[DISPLAY_LAYER_NET] = m_splitMode;

		if(1 == index)
		{
			splitMode[DISPLAY_LAYER_ANALOG] = VIEW_SPLIT_1X1;
			splitMode[DISPLAY_LAYER_NET] = VIEW_SPLIT_1X1;
		}

#ifdef __DVR_ULTIMATE__
		ULONGLONG displayCHPrev, displayCHMinorPrev;
		CIPCDecDisplayMan::Instance()->GetDisplayCH(displayCHPrev, displayCHMinorPrev);
		CIPCDecDisplayMan::Instance()->Stop();

		//处理SDK的stop标志不及时的问题。
		PUB_Sleep(20);
#endif
		ret = ChangeDisplayEx(splitMode, DISPLAY_LAYER_MAX, chnnData, index, m_display_info.time_out, DISPLAY_RECORD, DISPLAY_NOTIFY, false);
		if(ERR_OK == ret)
		{
			//ChangeDisplay默认操作会把m_tm_wait清0,能进跳台,说明不处于等待
			m_tm_wait = m_display_info.time_out;
		}

#ifdef __DVR_ULTIMATE__
		ULONGLONG displayCH, displayCHMinor;
		CIPCDecDisplayMan::Instance()->GetDisplayCH(displayCH, displayCHMinor);

		if((bNeedChangeNetDisplay && (0 == displayCH) && (0 == displayCHMinor)) || (0 == m_localVideoInputNum) && (0 == displayCH) && (0 == displayCHMinor))
		{
			displayCH = displayCHPrev;
			displayCHMinor = displayCHMinorPrev;
			CIPCDecDisplayMan::Instance()->SetDisplayCH(displayCH, displayCHMinor);
		}
		CIPCDecDisplayMan::Instance()->Start();
		
		CNetDeviceManager::Instance()->KeyFrame(displayCH, 0, 0);
		CNetDeviceManager::Instance()->RequestMinorStream(displayCHMinor);
#endif
	}

	return ret;
}

unsigned long CDisplayMan::Process1X1(unsigned long group,unsigned long* pData)
{
	return ProcessNXN(1,group,pData);
}

unsigned long CDisplayMan::Process2X2(unsigned long group,unsigned long* pData)
{
	return ProcessNXN(4,group,pData);
}

unsigned long CDisplayMan::Process2X3(unsigned long group,unsigned long* pData)
{
	return ProcessNXN(6,group,pData);
}

unsigned long CDisplayMan::Process3X3(unsigned long group,unsigned long* pData)
{
	return ProcessNXN(9,group,pData);
}

unsigned long CDisplayMan::Process4X4(unsigned long group,unsigned long* pData)
{
	return ProcessNXN(16,group,pData);
}

unsigned long CDisplayMan::Process4X6(unsigned long group,unsigned long* pData)
{
	return ProcessNXN(24,group,pData);
}

unsigned long CDisplayMan::Process5X5(unsigned long group,unsigned long* pData)
{
	return ProcessNXN(25,group,pData);
}

unsigned long CDisplayMan::Process6X6(unsigned long group,unsigned long* pData)
{
	return ProcessNXN(36,group,pData);
}

//////////////////////////////////////////////////////////////////////////
// 线程相关
//////////////////////////////////////////////////////////////////////////

bool CDisplayMan::Start()
{
	m_timer_ID = PUB_CreateThread(TimerThread, (void *)this, &m_bTimer);
	if( PUB_CREATE_THREAD_FAIL == m_timer_ID )
	{
		printf("Initial do dwell thread fail!");
		return false;
	}

	return true;
}

void CDisplayMan::Stop()
{
	if (PUB_THREAD_ID_NOINIT != m_timer_ID)
	{		
		PUB_ExitThread(&m_timer_ID, &m_bTimer);
	}
}

RESULT WINAPI CDisplayMan::TimerThread(LPVOID lpParameter)
{
#ifdef __ENVIRONMENT_LINUX__
	pid_t pid = getpid();
	pid_t tid = syscall(__NR_gettid);
	printf("%s, %d, pid = %d, tid=%d\n", __FILE__, __LINE__, pid, tid);
#endif

	assert(NULL != lpParameter);

	CDisplayMan *pThis = reinterpret_cast<CDisplayMan *>(lpParameter);

	pThis->Timer();

	return 0;
}

void CDisplayMan::Timer()
{
	bool enable = false;
	bool pause = false;
	bool process = false;

	bool spot = false;

	while (m_bTimer)
	{
		//初始不跳台
		process = false;
		spot = false;

		m_lock.Lock();
		{
			//跳台处理
			enable = m_bEnable;
			pause = m_bPause;

			if((DISPLAY_FREEZE != m_time_out) && (m_tm_wait <=  m_time_out))
			{
				m_tm_wait += 1;
			}

			if(enable && (!pause))
			{
				if((DISPLAY_FREEZE != m_time_out) && (m_tm_wait >= m_time_out))
				{
					//if(0 != m_dwellTime)
					{
						if(m_tmCount >= m_dwellTime)
						{
							m_tmCount = 0;
							process = true;
						}
						else
						{
							m_tmCount++;
						}
					}
				}
			}

			//Spot 处理
			if(0 != m_dwellTimeSpot)
			{
				if(m_tmCountSpot >= m_dwellTimeSpot)
				{
					m_tmCountSpot = 0;
					spot = true;
				}
				else
				{
					m_tmCountSpot++;
				}
			}
		}
		m_lock.UnLock();

		if(enable && (!pause) && process)
		{
			ProcessDwell();
		}

		if( spot && m_bHaveSpot)
		{
			ProcessSpot();
		}

		PUB_Sleep(1000);	//1秒,注意计时变量与此量的比率
	}
}

//////////////////////////////////////////////////////////////////////////
// 调试输出
//////////////////////////////////////////////////////////////////////////

void CDisplayMan::Dump()
{
	m_lock.Lock();

	printf("###########################.file=%s.\n",__FILE__);

	printf("-> Thread: m_bTimer=%d,m_timer_ID=%d.\n",m_bTimer,m_timer_ID);

	printf("-> Common: m_localVideoInputNum=%d,m_videoInputNum=%d,m_videoOutNum=%d,m_netVideoInputNum=%d.\n",
		m_localVideoInputNum,m_videoInputNum,m_videoOutNum,m_netVideoInputNum);

	printf("-> Dwell: m_bEnable=%d,m_bPause=%d,m_tmCount=%d,m_group=%d.\n",
		m_bEnable,m_bPause,m_tmCount,m_group);

	printf("-> Display: m_time_out=0x%x,m_tm_wait=0x%x,m_max_history=%d,m_max_stack=%d.\n",
		m_time_out,m_tm_wait,m_max_history,m_max_stack);

	printf("-> m_display_info: record=%d,chnn_num=%d,time_out=0x%x.\n",
		m_display_info.record,m_display_info.num_attrib,m_display_info.time_out);

	m_lock.UnLock();
}

void CDisplayMan::SendClickPos(unsigned long x,unsigned long y)
{
	CLocalDevice::Instance()->SetChosenPos(x,y);
	
}
void CDisplayMan::SendInitChnn(unsigned long chnn)
{
	CLocalDevice::Instance()->SendInitChnn(chnn);
}
void CDisplayMan::UpdateInvalidDwellTime()
{
	m_tmCount = m_dwellTime;
}

//end
