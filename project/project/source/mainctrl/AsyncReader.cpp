/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : xzw
** Date         : 2011-6-28
** Name         : AsyncReader.cpp
** Version      : 3.1.9
** Description  : 
** Modify Record:
1:
***********************************************************************/
#include "AsyncReader.h"


CAsyncReader::CAsyncReader()
{
	m_readType = READ_TYPE_NULL;
	m_pFilePath = NULL;
	m_pReader = NULL;
	m_maxCHNum = 0;
	m_chnnBits	= 0;
	m_currReaderID = -1;
	m_showTime = MAX_TIME;
	m_bRealPause = false;
}

CAsyncReader::~CAsyncReader()
{

}

bool CAsyncReader::Initial(long maxCHNum, CReclogManEX *pReclogManEx, unsigned long diskOwnerIndex)
{
	assert(maxCHNum > 0);
	assert(m_pReader == NULL);

	m_pFilePath = new char [256];

	m_maxCHNum = maxCHNum;
	m_pReader = new LOCAL_READER[maxCHNum];
	for(int i = 0; i < maxCHNum; i++)
	{
		m_pReader[i].reader.Initial(pReclogManEx, diskOwnerIndex);
		m_pReader[i].bSwitch = false;
		m_pReader[i].time = 0;
		m_pReader[i].chnn = INVALID_INDEX(unsigned long);
		m_pReader[i].bforward =  true;
		m_pReader[i].baseFrameTime = 0;
		m_pReader[i].baseShowTime = 0;
		m_pReader[i].lastReadFrameTime=0;
		m_pReader[i].speed = 1;
		m_pReader[i].bPause = false;
		m_pReader[i].bPlayNext = false;
	}

	m_chnnBits = 0;
	return true;
}
void CAsyncReader::Quit()
{
	m_chnnBits = 0;

	if(m_pReader != NULL)
	{
		for(int i = 0; i < m_maxCHNum; i++)
		{
			m_pReader[i].reader.Quit();
		}

		delete [] m_pReader;
		m_pReader = NULL;
	}

	if (m_pFilePath != NULL)
	{
		delete [] m_pFilePath;
		m_pFilePath = NULL;
	}	
}

long CAsyncReader::StartRead(FILE_INFO_EX_LIST *pFileList, ULONGLONG & outCH, const char *pFilePath /* = NULL */)
{
	if(pFileList == NULL)
	{
		assert(false);
		return MR_RET_NOTHING_TO_READ;
	}

	if(pFileList->IsEmpty())
	{
		return MR_RET_NOTHING_TO_READ;
	}

	FILE_INFO_EX_LIST	*pFileListCH = new FILE_INFO_EX_LIST [m_maxCHNum];
	unsigned long	*pIndexToCH = new unsigned long [m_maxCHNum];

	if ((NULL == pFileListCH) || (NULL == pIndexToCH))
	{
		assert (false);
		return MR_RET_UNKNOWN_ERROR;
	}

	//��ͳ��ͨ����Ŀ
	ULONGLONG chnnBits = 0;
	POS pos = pFileList->GetHeadPosition();
	while (NULL != pos)
	{
		FILE_INFO_EX & node = pFileList->GetNext(pos);
		chnnBits |= (static_cast<ULONGLONG>(0x01) << node.chnn);
	}

	//Ȼ���������ϵ��ͨ����INDEX��Ӧ��
	unsigned char index = 0, chnn = 0;
	do 
	{
		if (0x01 & chnnBits)
		{
			pIndexToCH [index] = chnn;
			++ index;
		}

		chnnBits = (chnnBits >> 1);
		++ chnn;
	} while(0 != chnnBits);

	//ȡ����СΪƥ����Ŀ
	unsigned char num = (index < m_maxCHNum) ? index : m_maxCHNum;
	
	//
	m_startTime = MAX_TIME;
	m_endTime = 0;
	outCH = 0;

	unsigned char i = 0;
	pos = pFileList->GetHeadPosition();
	while (NULL != pos)
	{
		FILE_INFO_EX & node = pFileList->GetNext(pos);

		m_startTime = (((LONGLONG)(node.startTime)*1000000) < m_startTime) ? ((LONGLONG)(node.startTime)*1000000) : m_startTime;
		m_endTime = (((LONGLONG)(node.endTime)*1000000) < m_endTime) ? m_endTime : ((LONGLONG)(node.endTime)*1000000);

		//���Ѿ������˵Ķ�Ӧ��ϵ���ҵ���������ļ������б������һ���ļ���
		for (i=0; i<num; ++i)
		{
			if (pIndexToCH[i] == node.chnn)
			{
				pFileListCH [i].AddTail(node);
				break;
			}
		}
	}

	for (i = 0; i<num; ++i)
	{
		//���õ�ͨ����ȡģ�飬ע��Ϊ���ļ��б��ȡ�Ľӿ�
		if (ONE_CHNN_READ_FAIL == m_pReader[i].reader.StartRead(pFileListCH[i], pFilePath))
		{
			m_pReader[i].bSwitch = false;
		}
		else
		{
			outCH |= ((ULONGLONG)0x01 << CHANNEL_ID(pIndexToCH[i]));//�Ǽ�����������ͨ��
			m_pReader[i].bSwitch = true;
			m_pReader[i].chnn = pIndexToCH[i];
		}

		pFileListCH[i].RemoveAll();
	}

	//
	delete [] pFileListCH;
	delete [] pIndexToCH;
	
	//�������ͨ���Ƿ���SLEEP״̬��������򷵻�ʧ�ܡ�
	if (CheckSleep())
	{
		outCH = 0;

		m_readType = READ_TYPE_NULL;
		m_chnnBits = outCH;

		return MR_RET_NOTHING_TO_READ;
	}
	else
	{
		m_readType = READ_BY_FILE_LIST;
		m_chnnBits = outCH;

		return MR_RET_OK;
	}
}

//Ҫ��ͨ��ӳ���б�ӳ�������������
long CAsyncReader::StartRead(unsigned long startTime, unsigned long endTime, \
							 const CHANNEL_LIST &chnnList, ULONGLONG & outCH, const char *pFilePath /* = NULL */)
{
	if(startTime >= endTime)
	{
		assert(false);
		return MR_RET_PARAM_ERROR;
	}

	if(chnnList.GetCount() > m_maxCHNum)
	{
		return MR_RET_TOO_MANY_CHANNELS;
	}

	//���õ�ͨ����ȡģ�飬ע��Ϊ��ʱ�俪ʼ�Ľӿڡ�
	outCH = 0;
	unsigned char i = 0;
	unsigned long chnn = 0;
	POS pos = chnnList.GetHeadPosition();
	while(pos)
	{
		chnn = chnnList.GetNext(pos);

		if (m_pReader[i].reader.StartRead(startTime, endTime, chnn, pFilePath))
		{
			outCH |= ((ULONGLONG)0x01 << chnn);
			m_pReader[i].bSwitch = true;
			m_pReader[i].chnn = chnn;			
		}
		else
		{
			m_pReader[i].bSwitch = false;
		}

		++ i;
	}
	
	if(outCH == 0)
	{
		return MR_RET_NOTHING_TO_READ;
	}

	//�������ͨ���Ƿ���SLEEP״̬��������򷵻�ʧ�ܡ�
	if (CheckSleep())
	{
		outCH = 0;

		m_readType = READ_TYPE_NULL;
		m_startTime = 0;
		m_endTime = 0;
		m_chnnBits = outCH;

		return MR_RET_NOTHING_TO_READ;
	}
	else
	{
		m_readType = READ_BY_START_END;
		m_startTime = (LONGLONG)(startTime)*1000000;
		m_endTime = (LONGLONG)(endTime)*1000000;
		m_chnnBits = outCH;

		if (NULL != pFilePath)
		{
			strcpy(m_pFilePath, pFilePath);
		}

		return MR_RET_OK;
	}
}

long CAsyncReader::StartRead(unsigned long startTime, const CHANNEL_LIST &chnnList, ULONGLONG & outCH, const char *pFilePath)
{
	if(chnnList.GetCount() > m_maxCHNum)
	{
		return MR_RET_TOO_MANY_CHANNELS;
	}

	//���õ�ͨ������ʼ��ʱ����ӿ�
	outCH = 0;
	unsigned char i = 0;
	unsigned long chnn = 0;
	POS pos = chnnList.GetHeadPosition();
	while(pos)
	{
		chnn = chnnList.GetNext(pos);

		if (m_pReader[i].reader.StartRead(startTime, chnn, pFilePath))
		{
			outCH |= ((ULONGLONG)0x01 << CHANNEL_ID(chnn));
			m_pReader[i].bSwitch = true;
			m_pReader[i].chnn = chnn;
		}
		else
		{
			m_pReader[i].bSwitch = false;
		}

		++ i;
	}

	//�������ͨ���Ƿ���SLEEP״̬��������򷵻�ʧ�ܡ�
	if (CheckSleep())
	{
		outCH = 0;

		m_readType = READ_TYPE_NULL;
		m_startTime = 0;
		m_endTime = 0;
		m_chnnBits = outCH;

		return MR_RET_NOTHING_TO_READ;
	}
	else
	{
		m_readType = READ_BY_START;
		m_startTime = (LONGLONG)(startTime)*1000000;
		m_endTime = MAX_TIME;
		m_chnnBits = outCH;

		if (NULL != pFilePath)
		{
			strcpy(m_pFilePath, pFilePath);
		}

		return MR_RET_OK;
	}
}

void CAsyncReader::StopRead()
{
	if (m_pReader != NULL)
	{
		for(int i = 0; i < m_maxCHNum; i++)
		{
			m_pReader[i].bSwitch = false;
			m_pReader[i].time = 0;
			m_pReader[i].reader.StopRead();
		}
	}
	m_readType = READ_TYPE_NULL;
}

long CAsyncReader::SetCurTime(bool bForward, unsigned long time, ULONGLONG & outCH, unsigned long chnn)
{
	for (long index=0; index<m_maxCHNum; ++index)
	{
		if (chnn == m_pReader[index].chnn )
		{
			if (m_pReader[index].bSwitch)
			{
				if (ONE_CHNN_READ_FAIL != m_pReader[index].reader.SeekFrameToTime(bForward, time))
				{
					outCH = m_chnnBits;
				}
				else
				{
					m_pReader[index].bSwitch = false;
					m_pReader[index].chnn	 = INVALID_INDEX(unsigned long);
					outCH = m_chnnBits & (~(ULONGLONG)0x01 << CHANNEL_ID(m_pReader[index].chnn));
				}
			}
		}
	}

	//�������ͨ���Ƿ���SLEEP״̬��������򷵻�ʧ�ܡ�
	if (CheckSleep())
	{
		outCH = 0;

		m_readType = READ_TYPE_NULL;
		m_startTime = 0;
		m_endTime = 0;
		m_chnnBits = outCH;

		return MR_RET_NOTHING_TO_READ;
	}
	else
	{
		m_chnnBits = outCH;

		return MR_RET_OK;
	}
}

/******************************************************************************

�������ܣ����õ�ǰ��ȡ���ݵ�ͨ��
����˵����chnnBits - Ҫ��ȡ���ݵ�ͨ������
����ֵ��

******************************************************************************/
long CAsyncReader::SetCurChannel(ULONGLONG chnnBits, ULONGLONG & outCH)
{
	//�ļ��б����ʽ����֧���л�ͨ���� 
	if (READ_BY_FILE_LIST == m_readType)
	{
		outCH = m_chnnBits;
		return MR_RET_OK;
	}

	outCH = 0;

	assert (0 != chnnBits);
	if (0 == chnnBits)
	{
		return MR_RET_PARAM_ERROR;
	}

	if (chnnBits == m_chnnBits)
	{
		outCH = m_chnnBits;
		return MR_RET_OK;
	}

	//�ȼ���Ѿ��򿪵�ͨ�������޸���ز�����
	for (unsigned char i=0; i<m_maxCHNum; ++i)
	{
		if (m_pReader[i].bSwitch && (((ULONGLONG)0x01 << CHANNEL_ID(m_pReader[i].chnn)) & chnnBits))
		{
			chnnBits &= ~((ULONGLONG)0x01 << CHANNEL_ID(m_pReader[i].chnn));
			outCH |= ((ULONGLONG)0x01 << CHANNEL_ID(m_pReader[i].chnn));

			//��λ����ǰʱ���ǰ�ؼ�֡���Ա㼰ʱ����
			if (ONE_CHNN_READ_FAIL == m_pReader[i].reader.SeekFrameToTime(true, static_cast<unsigned long>(m_pReader[i].lastReadFrameTime/1000000)))
			{
				m_pReader[i].bSwitch = false;
				m_pReader[i].chnn	 = INVALID_INDEX(unsigned long);
				outCH &= ~((ULONGLONG)0x01 << CHANNEL_ID(m_pReader[i].chnn));
			}
		}
		else if (m_pReader[i].bSwitch)
		{
			m_pReader[i].bSwitch = false;
			m_pReader[i].chnn = INVALID_INDEX(unsigned long);
			m_pReader[i].time = 0;
			m_pReader[i].reader.StopRead();
		}
	}

	//��������㣬���ʾ���ѡ�����������ͨ�����Ӽ���
	if (0 == chnnBits)
	{
		m_chnnBits = outCH;
		return MR_RET_OK;
	}

	//assert (m_startTime <= m_lastTime);

	//���0 != chnnBits������ζ���µ�֮ǰδ������ͨ����
	while (0 != chnnBits)	//˵�����µ�ͨ�����룬��Ҫ����
	{
		//Ѱ�ҵ���Ӧ��ͨ����
		unsigned char chnn = 0;
		ULONGLONG CH = chnnBits;
		while ((0 != CH) && !(CH & 0x01))
		{
			CH = (CH >> 1);
			++ chnn;
		}

		chnnBits &= ~((ULONGLONG)(0x01) << chnn);

		unsigned char index = INVALID_INDEX(unsigned char);
		for (unsigned char i=0; i<m_maxCHNum; ++i)	//ע��Ҫ���������Դͨ����
		{
			if (!m_pReader[i].bSwitch)
			{
				index = i;//����Դ���á�
				break;
			}
		}

		if (index != INVALID_INDEX(unsigned char))
		{
			if (READ_BY_START_END == m_readType)
			{
				if (ONE_CHNN_READ_FAIL != m_pReader[index].reader.StartRead(static_cast<unsigned long>(m_pReader[index].lastReadFrameTime/1000000), 
					static_cast<unsigned long>(m_endTime/1000000), chnn, m_pFilePath))
				{
					m_pReader[index].bSwitch = true;
					m_pReader[index].chnn	= chnn;
					m_pReader[index].time	= m_startTime;

					outCH |= ((ULONGLONG)0x01 << CHANNEL_ID(m_pReader[index].chnn));				
				}
			}
			else if (READ_BY_START == m_readType)
			{
				if (ONE_CHNN_READ_FAIL != m_pReader[index].reader.StartRead(static_cast<unsigned long>(m_pReader[index].lastReadFrameTime/1000000), chnn, m_pFilePath))
				{
					m_pReader[index].bSwitch = true;
					m_pReader[index].chnn	= chnn;
					m_pReader[index].time	= m_pReader[index].lastReadFrameTime;

					outCH |= ((ULONGLONG)0x01 << CHANNEL_ID(m_pReader[index].chnn));
				}
			}
			else
			{
				//��ȡ�ļ��б����ʽ���򲻴��µ�ͨ��
			}
		}
		else
		{
			return MR_RET_TOO_MANY_CHANNELS;
		}
	}

	//�������ͨ���Ƿ���SLEEP״̬��������򷵻�ʧ�ܡ�
	if (CheckSleep())
	{
		outCH = 0;

		m_readType = READ_TYPE_NULL;
		m_startTime = 0;
		m_endTime = 0;
		m_chnnBits = outCH;

		return MR_RET_NOTHING_TO_READ;
	}
	else
	{
		m_chnnBits = outCH;

		return MR_RET_OK;
	}
}

long CAsyncReader::GetNextFrameType(FRAME_INFO_EX &frameInfo, unsigned long &readerID,bool &bneedchange)
{
	int ret=MR_RET_OK;//����ֵ
	bool bpause=false;
	m_currReaderID = GetNextReadChannel(bneedchange,ret);
	
	if (m_currReaderID == -1)//����ͨ��û�пɲ�����
	{
		if(ret == MR_RET_ALLCHNN_PAUSE)
		{
			return ret;
		}
		else
		{
			return MR_RET_NOTHING_TO_READ;
		}
		
	}

	frameInfo.frameType = m_pReader[m_currReaderID].reader.GetFrameType();
	frameInfo.channel	= CHANNEL_ID(m_pReader[m_currReaderID].chnn);
	readerID = m_currReaderID;
	return ret;
}
/******************************************************************************

�������ܣ��õ���һ֡����
����˵����bVideoOnly - �Ƿ�ֻ�Ƕ�ȡ��Ƶ����
		  pFrameInfo - ����Ҫ��ȡ��һ֡����Ϣ�����ݣ�����ʱ��length��pData�������
		  bForward - �Ƿ����������򲥷�
����ֵ���ɹ�����MR_RET_OK
		���򷵻�MR_RET_NOTHING_TO_READ����ʾ�������ݿɶ�

*******************************************************************************/
long CAsyncReader::GetNextFrame(FRAME_INFO_EX &frameInfo, unsigned long &readerID)
{
	if (m_currReaderID == -1)//����ͨ��û�пɲ�����
	{
		return MR_RET_NOTHING_TO_READ;
	}

	
	
	if(ONE_CHNN_READ_SUC != m_pReader[m_currReaderID].reader.GetFrame(frameInfo))
	{
		return MR_RET_NOTHING_TO_READ;
	}
	else
	{ 
        if ((FRAME_TYPE_VIDEO == frameInfo.frameType) || (FRAME_TYPE_AUDIO == frameInfo.frameType))
		{
			frameInfo.frameAttrib = PLAY_FRAME_SHOW;

			if (FRAME_TYPE_VIDEO == frameInfo.frameType)
			{

			}
			if(SEEK_FRAME_OK != m_pReader[m_currReaderID].reader.SeekFrame(SEEK_NEXT_VFRM))
			{
				frameInfo.frameAttrib = PLAY_FRAME_SEC_END;
			}
		}
		readerID = m_currReaderID;		
		
		return MR_RET_OK;
 	}
}

/******************************************************************************

�������ܣ��õ�һ�ؼ�֡
����˵����nStepFrame - ȡֵ1��2��... �ֱ��ʾ�õ���һ�ؼ�֡������һ֡�ؼ�֡...
		  bForward����ȡ�ؼ�֡�ķ���true��ʾ��������õ���һ�ؼ�֡��false��ʾ������
		  pFrameInfo������Ҫ��ȡ�Ĺؼ�֡����Ϣ�����ݣ�����ʱ��length��pData�������
����ֵ���ɹ�����MR_RET_OK
		���򷵻�MR_RET_NOTHING_TO_READ����ʾ�������ݿɶ�

*******************************************************************************/
long CAsyncReader::GetKeyFrame(FRAME_INFO_EX &frameInfo, long nStepFrame, bool bForward, unsigned long &readerID)
{
	if (m_currReaderID == -1)//����ͨ��û�пɲ�����
	{
		return MR_RET_NOTHING_TO_READ;
	}

	//���ܵ�һ�ε����������ʱ���õ��ĵ�ǰ֡����Ƶ�ؼ�֡������Ҫ����һ�β��ҹؼ�֡���ٶ�
	while(true)
	{
		if (ONE_CHNN_READ_SUC !=m_pReader[m_currReaderID].reader.GetFrame(frameInfo) )
		{
			return MR_RET_NOTHING_TO_READ;
		}
		
		if (FRAME_TYPE_VIDEO == frameInfo.frameType)
		{
			frameInfo.frameAttrib = PLAY_FRAME_SHOW;			
		}
		
		if((FRAME_TYPE_VIDEO_FORMAT != frameInfo.frameType) && (FRAME_TYPE_AUDIO_FORMAT != frameInfo.frameType))
		{
			SEEK_MODE seekMode = bForward ? SEEK_NEXT_KEY : SEEK_PREV_KEY;
			if(SEEK_FRAME_OK != m_pReader[m_currReaderID].reader.SeekFrame(seekMode, nStepFrame))
			{
				frameInfo.frameAttrib = PLAY_FRAME_SEC_END;
			}

			if((frameInfo.frameType == FRAME_TYPE_VIDEO) && frameInfo.keyFrame)
			{
				break;
			}
		}
		else
		{
			break;
		}			
	}	
	
	readerID = m_currReaderID;	
	return MR_RET_OK;
}
bool CAsyncReader::HasAudioData(long channel)
{
//	return m_pReader[channel].reader.HasAudioData();
	return true;
}

//���ڼ������ͨ���Ƿ񶼴���READ_STATE_BEHIND_SLEEP״̬��
bool CAsyncReader::CheckSleep()
{
	//������п����ɹ���ͨ�����ǣ����ʱ���֮ǰ�����ݣ�֮��û�����ݣ���ô����Ϊû������
	unsigned char readSucNum = 0;
	unsigned char behindSleepNum = 0;
	for (unsigned char i = 0; i < m_maxCHNum; ++ i)
	{
		if (m_pReader[i].bSwitch)
		{
			++ readSucNum;

			if (READ_STATE_BEHIND_SLEEP == m_pReader[i].reader.GetReadState())
			{
				++ behindSleepNum;
			}
		}
	}

	if (0 == readSucNum)
	{
		return true;//û�������ɹ���Ҳ������Ϊ�Ƕ���SLEEP��
	}
	else
	{
		if (readSucNum == behindSleepNum)
		{
			for (unsigned char i = 0; i < m_maxCHNum; ++ i)
			{
				if (m_pReader[i].bSwitch)
				{
					m_pReader[i].reader.StopRead();
					m_pReader[i].bSwitch = false;
				}
			}

			return true;	//����ͨ������SLEEP���򷵻���ȷ��
		}

		return false;	//ֻҪ��һ��ͨ������SLEEP���򷵻�ʧ�ܡ�
	}
}

/******************************************************************************

�������ܣ���ȡ��һ֡
����˵����frameInfo ֡��Ϣ����
����ֵ�� �ɹ�����MR_RET_OK��ʧ�ܷ���ʧ��ԭ��

******************************************************************************/
long CAsyncReader::GetFrame(FRAME_INFO_EX &frameinfo, unsigned long &readerID)
{

	if (false == m_pReader[m_currReaderID].bforward)
	{
		
		return GetKeyFrame(frameinfo, 1, false, readerID);
	}
	else if (true == m_pReader[m_currReaderID].bJumping)
	{
		
		return GetKeyFrame(frameinfo, 1, true, readerID);
	}
	else
	{
		return GetNextFrame(frameinfo, readerID);
	}
}


/******************************************************************************

�������ܣ�����ĳ��ͨ�����ŷ���
����˵����chnnΪͨ���ţ�bforwardΪ�Ƿ���ǰ����
����ֵ����

******************************************************************************/
void CAsyncReader::SetForward(unsigned long chnn, bool bforward)
{
	for (unsigned char i = 0; i < m_maxCHNum; ++ i)
	{
		if (m_pReader[i].chnn == chnn)
		{
			m_pReader[i].bforward = bforward;
		}
	}
}

/******************************************************************************

�������ܣ�����ĳ��ͨ�������ٶ�
����˵����chnnΪͨ���ţ�speedΪ�����ٶ�
����ֵ��  ��

******************************************************************************/
void CAsyncReader::SetPlaySpeed(unsigned long chnn, unsigned short speed)
{
	for (unsigned char i = 0; i < m_maxCHNum; ++ i)
	{
		if (m_pReader[i].chnn == chnn)
		{
			m_pReader[i].speed = speed;
		}
	}
}

/******************************************************************************

�������ܣ�����ĳ��ͨ���Ƿ�ؼ�֡����
����˵����chnnΪͨ���ţ�bJumpingΪ�Ƿ�ؼ�֡����
����ֵ����

******************************************************************************/
void CAsyncReader::SetJumping(unsigned long chnn, bool bJumping)
{
	for (unsigned char i = 0; i < m_maxCHNum; ++ i)
	{
		if (m_pReader[i].chnn == chnn)
		{
			m_pReader[i].bJumping = bJumping;
		}
	}
}

/******************************************************************************

�������ܣ�������ͣ����
����˵����chnnΪͨ���ţ� bPauseΪ�Ƿ���ͣ
����ֵ��  ��

******************************************************************************/
void CAsyncReader::SetPause(unsigned long chnn, bool bPause)
{
	for (unsigned char i = 0; i < m_maxCHNum; ++ i)
	{
		if (m_pReader[i].chnn == chnn)
		{
			m_pReader[i].bPause = bPause;
		}
	}
}


/******************************************************************************

�������ܣ���������������һ֡
����˵����chnnΪͨ���ţ�bPlayNextΪ�Ƿ񲥷���һ֡
����ֵ��  ��

******************************************************************************/
void CAsyncReader::SetPlayNext(unsigned long chnn, bool bPlayNext)
{
	for (unsigned char i = 0; i < m_maxCHNum; ++ i)
	{
		if (m_pReader[i].chnn == chnn)
		{
			m_pReader[i].bPlayNext = bPlayNext;
		}
	}
}

void CAsyncReader::SetBaseShowTime(unsigned long readerID, LONGLONG baseShowTime)
{
	m_pReader[readerID].baseShowTime = baseShowTime;
}

void CAsyncReader::SetBaseFrameTime(unsigned long readerID, LONGLONG baseFrameTime)
{
	m_pReader[readerID].baseFrameTime = baseFrameTime;
}

void CAsyncReader::SetLastReadFrameTime(unsigned long readerID, LONGLONG lastReadFrameTime)
{
	m_pReader[readerID].lastReadFrameTime = lastReadFrameTime;
}

LONGLONG CAsyncReader::GetBaseFrameTime(unsigned long readerID)
{
	return m_pReader[readerID].baseFrameTime;
}
/******************************************************************************

						Protected Functions

******************************************************************************/

/******************************************************************************
******************************************************************************/

int CAsyncReader::GetNextReadChannel(bool &bneedchange,int &chnnstatus)
{
	int nextReadChannel = -1;
	LONGLONG	showTime= 0;
	m_showTime  = MAX_TIME;
	int Pausecount=0,bswitchcount=0;
	LONGLONG timeInterval=0;
	
	for(int i = 0; i < m_maxCHNum; i++)
	{
		if(m_pReader[i].bSwitch)
		{
			bswitchcount++;
			if (m_pReader[i].bPause == true && m_pReader[i].bPlayNext == false && m_bRealPause)
			{
				Pausecount++;
				continue;
			}

			//ֻ�д��ڻ״̬��ͨ���ͷ��򲥷��н����˯�߲Ų���ʱ������
			int teststatus = m_pReader[i].reader.GetReadState();	
			
			if((teststatus == READ_STATE_ACTION) \
				|| (teststatus == READ_STATE_FORWARD_SLEEP))
			{
				m_pReader[i].time = m_pReader[i].reader.GetFrameTime();
				
				if (m_pReader[i].time > m_pReader[i].baseFrameTime)
				{
					timeInterval = m_pReader[i].time - m_pReader[i].lastReadFrameTime;
					if (m_pReader[i].baseFrameTime != 0)
					{
						if(timeInterval>120000000)
						{
							//����ʱ��������60�룬ʱ������Ϊ0,��һ֡�ĵ�ʱ���Ϊ��ǰ֡����ʾʱ��
							if(m_pReader[i].time!=0)
							{
								m_pReader[i].baseFrameTime = m_pReader[i].time;
								bneedchange=true;
							}
							
						}
						else if(0 == timeInterval)//˵��������ǰ����ͷ��
						{
							continue;
						}
						
					}
					if (m_pReader[i].speed >> 8)
					{
						showTime = m_pReader[i].baseShowTime + (m_pReader[i].time - m_pReader[i].baseFrameTime) * (m_pReader[i].speed &(~(0x1 << 8)));
					}
					else
					{
						showTime = m_pReader[i].baseShowTime + (m_pReader[i].time - m_pReader[i].baseFrameTime) / m_pReader[i].speed;
					}					
				}
				else
				{
					timeInterval = m_pReader[i].lastReadFrameTime -m_pReader[i].time;
					if(timeInterval > 0)
					{
						if (m_pReader[i].baseFrameTime != 0)
						{
							if(timeInterval>120000000)
							{
								//����ʱ��������60�룬ʱ������Ϊ0,��һ֡�ĵ�ʱ���Ϊ��ǰ֡����ʾʱ��
								if(m_pReader[i].time!=0)
								{
									m_pReader[i].baseFrameTime = m_pReader[i].time;
									bneedchange=true;
								}

							}
						}
					}
					else if(0 == timeInterval)//˵�������ݺ��˵�ͷ��
					{
						continue;
					}
					if (m_pReader[i].speed >> 8)
					{
						showTime = m_pReader[i].baseShowTime + (m_pReader[i].baseFrameTime -m_pReader[i].time) * (m_pReader[i].speed &(~(0x1 << 8)));
					}
					else
					{
						showTime = m_pReader[i].baseShowTime + (m_pReader[i].baseFrameTime -m_pReader[i].time) / m_pReader[i].speed;
					}
				}
			}
			
			if(showTime!=0)
			{			
				if (showTime < m_showTime)
				{
					m_showTime = showTime;
					nextReadChannel = i;
					
				}
			}
			
			if(m_pReader[i].bPlayNext == true)
			{
				m_pReader[i].bPlayNext = false;
				nextReadChannel = i;
				break;
			}
			if(m_pReader[i].baseFrameTime == 0)
			{
				return i;
			}
			
		}//end if(m_pReader[i].bSwitch)
	}

	//�����ڲ����н����˯��
	if((nextReadChannel != -1) && \
		((m_pReader[nextReadChannel].reader.GetReadState() == READ_STATE_FORWARD_SLEEP) \
		|| (m_pReader[nextReadChannel].reader.GetReadState() == READ_STATE_BEHIND_SLEEP)) )
	{
		m_pReader[nextReadChannel].reader.WakeUp();  
	}
	if(bswitchcount==Pausecount)
	{
		chnnstatus = MR_RET_ALLCHNN_PAUSE;
	}
	
	return nextReadChannel;
}


