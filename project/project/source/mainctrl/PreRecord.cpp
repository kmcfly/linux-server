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
#include "PreRecord.h"
#include "mylist.cpp"

CPreRecord::CPreRecord() : m_dataLen(0), 
m_maxDataLen(PRE_RECORD_MAX_LEN), 
m_fristFrameTime(0), 
m_maxDataTime(PRE_RECORD_MAX_TIME*1000000),
m_bCanRecord(false)
{
	m_oldMaxDataTime = m_maxDataTime;

}

CPreRecord::~CPreRecord()
{
	RemoveAll();
}

void CPreRecord::Initial(unsigned char channel, char *pPath)
{
	m_dataLen = 0;
	m_maxDataLen = PRE_RECORD_MAX_LEN;
	m_fristFrameTime = 0;
	m_maxDataTime = PRE_RECORD_MAX_TIME*1000000;
	m_oldMaxDataTime = m_maxDataTime;
	m_bCanRecord = false;
	assert(m_frameList.GetCount() == 0);

	m_bCanRecord = m_prerecordDataImp.Initial(channel, pPath);

	return;
}

bool CPreRecord::IsCanPreRecord()
{
	return m_bCanRecord;
}

bool CPreRecord::AddFrame(CFrameData * pFrameData, bool & bKeyFrame, bool bDelete /* = true */)
{
	assert(NULL != pFrameData);

	if (!m_bCanRecord)
	{
		//bKeyFrame = false;
		return false;
	}

	//�ȼ����б�
	if (!AddToList(pFrameData, bKeyFrame))
	{		
		return false;
	}
	
	RefreshFirstTime();

	if (!bDelete)
	{
		return true;
	}

	//Ȼ���ٽ���ɾ������
	bKeyFrame = false;
	LONGLONG time = pFrameData->GetFrameInfo().time;

	//2010-02-25 15:16:00 YSW
	//���ݳ��ȳ�����󻺴���
	//�ۻ�ʱ�䳬��ָ��ʱ�䳤��
	//����֡����ǰʱ��ĳ��ȳ���ָ��ʱ�䳤��
	//����Ϊ����ɾ����������������
	while ((m_maxDataLen < m_dataLen) || (m_maxDataTime < (time - m_fristFrameTime)) || (m_maxDataTime < (GetCurrTime() - m_fristFrameTime)))
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

CFrameData * CPreRecord::GetFrame()
{
	if (m_frameList.IsEmpty())
	{
		return NULL;
	}
	else
	{
		CFrameData *pNode = m_frameList.GetHead();
		unsigned char *pDataTemp = NULL;
		if (pNode->GetFrameInfo().pData != NULL)
		{
			//��ʾ����GetFrame������û�е���RemoveHead.����Ǵ��ڵģ�����Ԥ¼�������Ҫ��30������ݣ�д���ļ����п���ֻд��һ֡���ݣ��ļ������ˣ�
			//��RecordProc�У����������û�е���RemoveHead
			printf("#######################%s,%d,chnn:%d,GetFrame two, because of change file\n",__FILE__,__LINE__,pNode->GetFrameInfo().channel);
			return pNode;
		}
		else
		{
			if (m_prerecordDataImp.GetData(&pDataTemp, pNode->GetFrameInfo().length))
			{
				pNode->GetFrameInfo().pData = pDataTemp;
				return pNode;
			}
			else
			{			
				RemoveAll();			
				m_bCanRecord = false;
				return NULL;
			}
		}
	}
}

CFrameData * CPreRecord::RemoveHead()
{
	if (m_frameList.IsEmpty())
	{
		return NULL;
	}
	else
	{
		CFrameData *pNode = m_frameList.RemoveHead();
		assert(pNode->GetFrameInfo().pData != NULL);
		pNode->GetFrameInfo().pData = NULL;
		if (m_prerecordDataImp.DeleteData(pNode->GetFrameInfo().length))
		{
			m_dataLen -= pNode->GetFrameInfo().length;
			RefreshFirstTime();
			delete pNode;
			pNode = NULL;
			return pNode;
		}
		else
		{
			delete pNode;
			pNode = NULL;
			RemoveAll();
			m_bCanRecord = false;
			return NULL;
		}		
	}
}

void CPreRecord::RemoveAll()
{
	CFrameData *pNode = NULL;

	while (m_frameList.GetCount() > 0)
	{
		pNode = m_frameList.RemoveHead();	
		//assert(pNode->GetFrameInfo().pData == NULL);
		pNode->GetFrameInfo().pData = NULL;
		delete pNode;
		pNode = NULL;
	}
	m_prerecordDataImp.DeleteAllData();
	m_dataLen = 0;
	m_fristFrameTime = 0;
}

bool CPreRecord::CurKeyFrame()
{
	if (m_frameList.IsEmpty())
	{
		return false;
	}
	CFrameData *pNode = m_frameList.GetHead();
	if (1 == pNode->GetFrameInfo().keyFrame)
	{
		return true;
	}
	else
	{
		return false;
	}
}
unsigned long CPreRecord::GetCount()
{
	return m_frameList.GetCount();
}

void CPreRecord::SetPreRecTime(unsigned long time)
{
	m_oldMaxDataTime = m_maxDataTime;
	m_maxDataTime = (time+3)*1000000;
}

void CPreRecord::RecoverPreRecTime()
{
	if ((m_oldMaxDataTime > 0)  && (m_oldMaxDataTime <= (PRE_RECORD_MAX_TIME+3)*1000000))
	{
		m_maxDataTime = m_oldMaxDataTime;
	}
	else
	{

	}
}

//private
bool CPreRecord::AddToList(CFrameData * pFrameData, bool & bKeyFrame)
{
	//����Ϊ�գ���Ϊ�ǹؼ�֡���򷵻�
	if (m_frameList.IsEmpty() && (!pFrameData->GetFrameInfo().keyFrame || (FRAME_TYPE_AUDIO == pFrameData->GetFrameInfo().frameType)))
	{
		//bKeyFrame = true;
		return false;
	}
	else
	{
		//�����ݼ����б�β����
		if (m_frameList.IsEmpty())
		{
			assert(0 == m_dataLen);
		}
		
		CFrameData *pFrameDataTemp = new CFrameData(pFrameData->GetFrameInfo());
		
		pFrameDataTemp->GetFrameInfo().pData = NULL;
		m_frameList.AddTail(pFrameDataTemp);
		int ret = m_prerecordDataImp.InsertData(pFrameData->GetFrameInfo().pData, pFrameData->GetFrameInfo().length);
		if (ret >= 0)
		{
			m_dataLen += pFrameData->GetFrameInfo().length;
			return true;
		}
		else if (-2 == ret)
		{		
			RemoveAll();
			bKeyFrame = true;
			printf("%s,%d,chnn:%d,get buffer file failed\n",__FILE__,__LINE__,pFrameData->GetFrameInfo().channel);
			return false;			
		}	
		else
		{
			RemoveAll();
			m_bCanRecord = false;
			return false;
		}
	}
}

void CPreRecord::RemoveFrame()
{
	bool bHead = true;
	
	POS prevPos = NULL;
	POS pos = m_frameList.GetHeadPosition();
	int chnn = 0;
	while (NULL != pos)
	{
		prevPos = pos;
		CFrameData *pNode = m_frameList.GetNext(pos);
		if (!bHead && pNode->GetFrameInfo().keyFrame && (FRAME_TYPE_VIDEO == pNode->GetFrameInfo().frameType))
		{
			break;
		}
		else
		{
			//�ͷ�buf
			m_frameList.RemoveAt(prevPos);
			m_dataLen -=pNode->GetFrameInfo().length;
			//assert(pNode->GetFrameInfo().pData == NULL);
			pNode->GetFrameInfo().pData = NULL;			
			if (m_prerecordDataImp.DeleteDataEx(pNode->GetFrameInfo().length))
			{
				chnn = pNode->GetFrameInfo().channel;
				delete pNode;
				pNode = NULL;
			}
			else
			{	
				delete pNode;
				pNode = NULL;
				RemoveAll();
				m_bCanRecord = false;
				break;
			}	
			
			bHead = false;
		}
	}
//	printf("%s,%d,chnn:%d,remove:%d\n",__FILE__,__LINE__,chnn,count);
	
	RefreshFirstTime();
}

void CPreRecord::RefreshFirstTime()
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

void CPreRecord::ResetPreRecordPath(char * path, bool &bPreRecord)
{
	if (NULL == path)
	{
		RemoveAll();	
		m_prerecordDataImp.CleanPreRecordPath();
		m_bCanRecord = false;
	}
	else
	{
		RemoveAll();
		m_bCanRecord = m_prerecordDataImp.ResetPreRecordPath(path);
	}
	bPreRecord = m_bCanRecord;
}
//end
