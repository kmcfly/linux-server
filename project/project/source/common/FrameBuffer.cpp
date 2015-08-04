/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : 袁石维
** Date         : 2007-12-18
** Name         : FrameBuffer.cpp
** Version      : 1.0
** Description  :
** Modify Record:
1:
***********************************************************************/
#include "FrameBuffer.h"
#include "mylist.h"
#include "NetProtocol.h"
#include "mylist.cpp"

unsigned long CFrameBuffer::m_currIndex = 0;
CPUB_Lock CFrameBuffer::m_indexLock;

//public
CFrameBuffer::CFrameBuffer(NET_STREAM_TYPE type, ULONG len, ULONG time) : m_bKeyFrame(true), m_type(type), \
			m_maxDataLen(len), m_maxDataTime((LONGLONG)(time)*1000000)
{
	memset(&m_fristFrameTime, 0, sizeof(LONGLONG));
	m_dataLen = 0;
}

CFrameBuffer::~CFrameBuffer()
{
}

bool CFrameBuffer::AddFrame(CFrameData *pFrameData, bool & bKeyFrame)
{
	assert(NULL != pFrameData);

	if (FRAME_TYPE_NONE == pFrameData->GetFrameInfo().frameType)
	{
		return false;
	}

	if (NET_STREAM_VIDEO == m_type)
	{		
		if((FRAME_TYPE_VIDEO != pFrameData->GetFrameInfo().frameType) && 
			(FRAME_TYPE_VIDEO_FORMAT != pFrameData->GetFrameInfo().frameType) &&
			(FRAME_TYPE_JPEG != pFrameData->GetFrameInfo().frameType))
		{
			bKeyFrame = m_bKeyFrame;
			return false;
		}
		else
		{
			bool ret = AddToList(pFrameData, bKeyFrame);
			RemoveFrame();
			bKeyFrame = m_bKeyFrame;
			return ret;
		}
	}

	if (NET_STREAM_AUDIO == m_type)
	{		
		if((FRAME_TYPE_VIDEO == pFrameData->GetFrameInfo().frameType) || (FRAME_TYPE_VIDEO_FORMAT == pFrameData->GetFrameInfo().frameType))
		{
			return false;
		}
		else
		{		
			bool ret = AddToList(pFrameData, bKeyFrame);
			RemoveFrame();
			return ret;
		}
	}
	
	if (NET_STREAM_PLAYBACK == m_type)
	{
	/*	if((FRAME_TYPE_VIDEO_FORMAT == pFrameData->GetFrameInfo().frameType) || (FRAME_TYPE_AUDIO_FORMAT == pFrameData->GetFrameInfo().frameType))
		{
			return false;
		}
		else
		{*/
			return AddToList(pFrameData, bKeyFrame);//	回放数据不删除
	//	}
	}

	assert(false);
	return false;
}

bool CFrameBuffer::GetFrame(unsigned long & dataID, FRAME_INFO_EX **ppFrameInfo, BYTE *pData, ULONG &bufLen, ULONG offset, bool & bSameID)
{
	assert (NULL != ppFrameInfo);
	assert(NULL != pData);

	if (m_frameList.IsEmpty())
	{
		return false;
	}

	m_listLock.Lock();
	if (m_frameList.IsEmpty())
	{
		m_listLock.UnLock();
		return false;
	}
	else
	{
		FRAME_DATA_INFO &head = m_frameList.GetHead();
		FRAME_INFO_EX frameInfo = head.pFrameData->GetFrameInfo();

		if (FRAME_TYPE_NONE != frameInfo.frameType)
		{
			assert (NULL != pData);

			//如果不是4的整数倍，减小到4的整数倍先
			bufLen = bufLen - bufLen % 4;

			assert(NULL == frameInfo.pData);
			assert(0 == frameInfo.length);

			if (dataID != head.dataID)
			{
				//如果是上一个分包只剩小于(2*sizeof(unsigned long) + sizeof(FRAME_INFO_EX))的长度时
				//后面的分包丢掉了，则会出现这个情况
				//if (bufLen <= (2*sizeof(unsigned long) + sizeof(FRAME_INFO_EX)))
				if (bufLen <= (8 + sizeof(FRAME_INFO_EX))) 
				{
					m_listLock.UnLock();
					return false;
				}

				dataID = head.dataID;

				unsigned long protocol = head.pFrameData->GetProtocol();
				PACKCMD *pHead = (PACKCMD*)(pData);
				pHead->cmdVer = NET_PROTOCOL_VER;
				pHead->cmdID = 0xffffffff;
				pHead->cmdType = protocol;

				//变成4的整数倍长度
				unsigned long dwResidual = frameInfo.length % 4;
				unsigned long lenTmp = frameInfo.length + ((dwResidual > 0 ) ? (4 - dwResidual) : 0 );

				unsigned long len = sizeof(FRAME_INFO_EX) + lenTmp;
				pHead->dataLen = len;

				unsigned long headLen = sizeof(PACKCMD);
				FRAME_INFO_EX * pFrameInfo = (FRAME_INFO_EX *)(pData + headLen);
				* pFrameInfo = frameInfo;
				//memcpy((pData + headLen), &frameInfo, sizeof(FRAME_INFO_EX));

				unsigned long subLen = bufLen - (headLen + sizeof(FRAME_INFO_EX));

				if ((subLen > 0) && (lenTmp > 0))
				{
					unsigned long dataLen = (subLen > lenTmp) ? lenTmp : subLen;
					unsigned long cpyLen = (subLen > frameInfo.length) ? frameInfo.length : subLen;
					memcpy(pData + (headLen + sizeof(FRAME_INFO_EX)), frameInfo.pData, cpyLen);
					bufLen = headLen + sizeof(FRAME_INFO_EX) + dataLen;
				}
				else
				{
					bufLen = headLen + sizeof(FRAME_INFO_EX);
				}

				*ppFrameInfo = (FRAME_INFO_EX *)(pData + headLen);

				if (bufLen >= (headLen + sizeof(FRAME_INFO_EX) + frameInfo.length)) //如果数据读取完毕，则从链表中弹出
				{
					m_dataLen -= frameInfo.length;

					head.pFrameData->Dec();
					m_frameList.RemoveHead();
				}

				bSameID = false;

				m_listLock.UnLock();
				return true;
			}
			else
			{
				unsigned long headLen = (sizeof(PACKCMD) + sizeof(FRAME_INFO_EX));

				offset -= headLen;

				unsigned long dataLen = (bufLen > (frameInfo.length - offset)) ? (frameInfo.length - offset) : bufLen;
				memcpy(pData, (frameInfo.pData + offset), dataLen);

				//变成4的整数倍长度
				unsigned long dwResidual = dataLen % 4;
				bufLen = dataLen + ((dwResidual > 0 ) ? (4 - dwResidual) : 0 );

				if ((offset + dataLen) >= frameInfo.length)	//如果数据读取完毕，则从链表中弹出
				{
					m_dataLen -= frameInfo.length;

					head.pFrameData->Dec();
					m_frameList.RemoveHead();
				}

				bSameID = true;

				m_listLock.UnLock();
				return true;
			}
		}
		else
		{
			head.pFrameData->Dec();
			m_frameList.RemoveHead();

			m_listLock.UnLock();
			return false;
		}
	}
}

void CFrameBuffer::RemoveAll()
{
	m_listLock.Lock();
	
	FRAME_DATA_INFO node;

	while (m_frameList.GetCount() > 0)
	{
		node = m_frameList.RemoveHead();
		
		//释放buf
		node.pFrameData->Dec();
	}

	m_dataLen = 0;
	m_fristFrameTime = 0;
	m_bKeyFrame = true;

	m_listLock.UnLock();
}
//protected

//private
bool CFrameBuffer::AddToList(CFrameData *pFrameData, bool &bKeyFrame)
{
	m_listLock.Lock();

	if (NET_STREAM_VIDEO == m_type)
	{
		if (m_bKeyFrame && (!pFrameData->GetFrameInfo().keyFrame && (FRAME_TYPE_VIDEO == pFrameData->GetFrameInfo().frameType)))
		{
			m_listLock.UnLock();
			return false;
		}
		
		if (m_bKeyFrame && (FRAME_TYPE_VIDEO == pFrameData->GetFrameInfo().frameType))
		{
			m_bKeyFrame = false;
		}
	}
	
	pFrameData->Add();
	FRAME_DATA_INFO node;
	node.pFrameData = pFrameData;
	
	m_indexLock.Lock();
	do 
	{
		++ m_currIndex;
	}while(INVALID_DATA_ID == m_currIndex);

	node.dataID		= m_currIndex;
	m_indexLock.UnLock();

	m_frameList.AddTail(node);
	m_dataLen += pFrameData->GetFrameInfo().length;

	RefreshFirstTime();

	m_listLock.UnLock();
	return true;
}

void CFrameBuffer::RemoveFrame()
{
	assert((NET_STREAM_VIDEO == m_type) || (NET_STREAM_AUDIO == m_type));
	
	m_listLock.Lock();

	LONGLONG tailTime = 0;
	POS tailPos = m_frameList.GetTailPosition();
	while (NULL != tailPos)
	{
		FRAME_DATA_INFO &node = m_frameList.GetPrev(tailPos);
		if ((FRAME_TYPE_VIDEO == node.pFrameData->GetFrameInfo().frameType) \
			|| (FRAME_TYPE_AUDIO == node.pFrameData->GetFrameInfo().frameType) \
			|| (FRAME_TYPE_JPEG == node.pFrameData->GetFrameInfo().frameType))
		{
			tailTime = node.pFrameData->GetFrameInfo().time;
			break;
		}
	}

	if (0 == tailTime)
	{
		tailTime = m_fristFrameTime;
	}

	//然后再进行删除动作
	while ((m_maxDataLen < m_dataLen) || (m_maxDataTime < (tailTime - m_fristFrameTime)) || (m_maxDataTime < (tailTime - GetCurrTime())))
	{
		bool bHead = true;
		
		POS prevPos = NULL;
		POS pos = m_frameList.GetHeadPosition();
		while (NULL != pos)
		{
			prevPos = pos;
			FRAME_DATA_INFO &node = m_frameList.GetNext(pos);
			
			//格式不删除
			if ((FRAME_TYPE_VIDEO == node.pFrameData->GetFrameInfo().frameType) \
				|| (FRAME_TYPE_AUDIO == node.pFrameData->GetFrameInfo().frameType) \
				|| (FRAME_TYPE_TALK_AUDIO == node.pFrameData->GetFrameInfo().frameType) \
				|| (FRAME_TYPE_JPEG == node.pFrameData->GetFrameInfo().frameType))
			{	
				if (!bHead && 
					((FRAME_TYPE_AUDIO == node.pFrameData->GetFrameInfo().frameType) 
					|| (FRAME_TYPE_TALK_AUDIO == node.pFrameData->GetFrameInfo().frameType)
					|| (FRAME_TYPE_JPEG == node.pFrameData->GetFrameInfo().frameType)
					|| (node.pFrameData->GetFrameInfo().keyFrame && (FRAME_TYPE_VIDEO == node.pFrameData->GetFrameInfo().frameType))))
				{
					break;
				}
				else
				{
					//释放buf
					m_dataLen -= node.pFrameData->GetFrameInfo().length;
					node.pFrameData->Dec();
					m_frameList.RemoveAt(prevPos);

					bHead = false;
				}
			}
		}
		RefreshFirstTime();
		if (0 == m_fristFrameTime)	//意味着没有数据了
		{
			m_bKeyFrame = true;
			break;
		}
	}

	m_listLock.UnLock();
}

void CFrameBuffer::RefreshFirstTime()
{
	m_fristFrameTime = 0;
	POS pos = m_frameList.GetHeadPosition();
	while (NULL != pos)
	{		
		FRAME_DATA_INFO &head = m_frameList.GetNext(pos);
		if ((FRAME_TYPE_VIDEO_FORMAT != head.pFrameData->GetFrameInfo().frameType) && (FRAME_TYPE_AUDIO_FORMAT != head.pFrameData->GetFrameInfo().frameType))
		{
			m_fristFrameTime = head.pFrameData->GetFrameInfo().time;
			break;
		}
	}
}
//end
