/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : Ԭʯά
** Date         : 2007-10-11
** Name         : PreRecord.cpp
** Version      : 1.0
** Description  :
** Modify Record:
1:
***********************************************************************/
#include "PreMemRecord.h"
#include "mylist.cpp"

CPreMemRecord::CPreMemRecord() : m_dataLen(0), m_maxDataLen(PRE_RECORD_MAX_LEN), m_fristFrameTime(0), m_maxDataTime(PRE_RECORD_MAX_TIME*1000000)
{
}

CPreMemRecord::~CPreMemRecord()
{
}

bool CPreMemRecord::AddFrame(CFrameData * pFrameData, bool & bKeyFrame)
{
	assert(NULL != pFrameData);

	//�ȼ����б�
	if (!AddToList(pFrameData))
	{
		//bKeyFrame = true;
		return false;
	}
	
	RefreshFirstTime();

	//Ȼ���ٽ���ɾ������
	bKeyFrame = false;
	LONGLONG time = pFrameData->GetFrameInfo().time;

	//2010-02-25 15:16:00 YSW
	//���ݳ��ȳ�����󻺴���
	//�ۻ�ʱ�䳬��ָ��ʱ�䳤��
	//����֡����ǰʱ��ĳ��ȳ���ָ��ʱ�䳤��
	//����Ϊ����ɾ����������������
	LONGLONG llCurTime = GetCurrTime();
	
	while ((m_maxDataLen < m_dataLen) || (m_maxDataTime < (time - m_fristFrameTime)) || (m_maxDataTime < (llCurTime - m_fristFrameTime)))
	{
		RemoveFrame();

		//bKeyFrame = true;

		if (m_frameList.IsEmpty())
		{
			break;
		}
		else
		{
			time = m_frameList.GetTail()->GetFrameInfo().time;
		}
	}

	return true;
}

CFrameData * CPreMemRecord::GetFrame()
{
	if (m_frameList.IsEmpty())
	{
		return NULL;
	}
	else
	{
		CFrameData *pNode = m_frameList.GetHead();

		return pNode;
	}
}

CFrameData * CPreMemRecord::RemoveHead()
{
	if (m_frameList.IsEmpty())
	{
		return NULL;
	}
	else
	{
		CFrameData *pNode = m_frameList.RemoveHead();
		
		pNode->Dec();

		m_dataLen -= pNode->GetFrameInfo().length;

		RefreshFirstTime();

		return pNode;
	}
}

void CPreMemRecord::RemoveAll()
{
	CFrameData *pNode = NULL;

	while (m_frameList.GetCount() > 0)
	{
		pNode = m_frameList.RemoveHead();
		
		//�ͷ�buf
		pNode->Dec();
	}

	m_dataLen = 0;
	m_fristFrameTime = 0;
}

bool CPreMemRecord::RemoveSection()
{
//	if (50 <= m_frameList.GetCount())
	{
		RemoveFrame();
	}
	
	bool ret = false;
	if (m_frameList.IsEmpty())
	{
		ret = true;
	}

	return ret;
}

unsigned long CPreMemRecord::GetCount()
{
	return m_frameList.GetCount();
}

void CPreMemRecord::SetPreRecTime(unsigned long time)
{
#if defined(__CUSTOM_SHANGHAI__)
	time += 4;
#endif
	m_maxDataTime = time * 1000000;
}

void CPreMemRecord::SetPreRecDataLen(unsigned long maxPreRecDataLen)
{
	m_maxDataLen = maxPreRecDataLen;
	printf("..................................................m_maxDataLen = %d\n", m_maxDataLen);
}
//private
bool CPreMemRecord::AddToList(CFrameData * pFrameData)
{
	//����Ϊ�գ���Ϊ�ǹؼ�֡���򷵻�
	if (m_frameList.IsEmpty() && (!pFrameData->GetFrameInfo().keyFrame || (FRAME_TYPE_AUDIO == pFrameData->GetFrameInfo().frameType)))
	{
		return false;
	}
	else
	{
		//�����ݼ����б�β����
		if (m_frameList.IsEmpty())
		{
			assert(0 == m_dataLen);
		}

		m_frameList.AddTail(pFrameData);

		pFrameData->Add();
		
		m_dataLen += pFrameData->GetFrameInfo().length;

		return true;
	}
}

void CPreMemRecord::RemoveFrame()
{
	bool bHead = true;
	
	POS prevPos = NULL;
	POS pos = m_frameList.GetHeadPosition();
	CFrameData *pNode = NULL;
	
	while (NULL != pos)
	{
		prevPos = pos;
		pNode = m_frameList.GetNext(pos);

		if (!bHead && pNode->IsKeyFrame() && (FRAME_TYPE_VIDEO == pNode->FrameType()))
		{
			break;
		}
		else
		{
			//�ͷ�buf
			m_frameList.RemoveAt(prevPos);
			m_dataLen -= pNode->FrameLen();
			pNode->Dec();
			
			bHead = false;
		}
	}
	
	RefreshFirstTime();
}

void CPreMemRecord::RefreshFirstTime()
{
	//����ͷ֡ʱ��
	if (m_frameList.IsEmpty())
	{
		m_fristFrameTime = 0;
		assert(0 == m_dataLen);
	}
	else
	{
		CFrameData *pHead = m_frameList.GetHead();
		m_fristFrameTime = pHead->GetFrameInfo().time;
	}
}
//end


