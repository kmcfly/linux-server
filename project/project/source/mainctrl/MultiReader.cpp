// MultiReader.cpp: implementation of the CMultiReader class.
//
/*////////////////////////////////////////////////////////////////////
���ļ�¼:
2010-01-18 09:43 (YSW)    
�޸�ǰֻ��һ��StartRead��SetCurrChannel�ӿڼ��������ͨ���Ƿ񶼴��ں�
SLEEP״̬������SetTime��û�м�飬��������λ��ĳ���㣨��������ͨ����û��
���ݣ�����Ҳ�᷵�سɹ������ǻ�ȡ���ݻ�ʧ�ܣ����������Ӽ��Sleep�ķ���������
�������ͨ���Ƿ񶼴���Sleep״̬��

/////////////////////////////////////////////////////////////////////*/
#include "MultiReader.h"
#include <omp.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMultiReader::CMultiReader()
{
	m_readType = READ_TYPE_NULL;
	m_pFilePath = NULL;
	m_pReader = NULL;
	m_maxCHNum = 0;
	m_chnnBits	= 0;
}

CMultiReader::~CMultiReader()
{

}

bool CMultiReader::Initial(long maxCHNum, CReclogManEX *pReclogManEx, unsigned long diskOwnerIndex)
{
	assert(maxCHNum > 0);
	assert(m_pReader == NULL);

	m_pFilePath = new char [256];
	if (NULL == m_pFilePath)
	{
		printf("%s:%s:%d, new m_pFilePath\n", __FUNCTION__, __FILE__, __LINE__);
		fflush(stderr);
		return false;
	}
	else
	{
		memset(m_pFilePath, 0, 256);
	}

	m_maxCHNum = maxCHNum;
	m_pReader = new LOCAL_READER[maxCHNum];
	for(int i = 0; i < maxCHNum; i++)
	{
		m_pReader[i].reader.Initial(pReclogManEx, diskOwnerIndex);
		m_pReader[i].bSwitch = false;
		m_pReader[i].time = 0;
		m_pReader[i].chnn = INVALID_INDEX(unsigned long);
	}

	m_chnnBits = 0;
	return true;
}
void CMultiReader::Quit()
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

	delete [] m_pFilePath;
	m_pFilePath = NULL;
}

long CMultiReader::StartRead(FILE_INFO_EX_LIST *pFileList, ULONGLONG & outCH, const char *pFilePath /* = NULL */)
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
	int num = (index < m_maxCHNum) ? index : m_maxCHNum;
	
	//
	m_startTime = MAX_TIME;
	m_endTime = 0;
	outCH = 0;

	int i = 0;
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

	ULONGLONG outCHT[128] = {0};

	#pragma omp parallel for num_threads(num)
	for (i = 0; i<num; ++i)
	{
		//���õ�ͨ����ȡģ�飬ע��Ϊ���ļ��б��ȡ�Ľӿ�
		if (ONE_CHNN_READ_FAIL == m_pReader[i].reader.StartRead(pFileListCH[i], pFilePath))
		{
			m_pReader[i].bSwitch = false;
		}
		else
		{
			outCHT[i] = ((ULONGLONG)0x01 << CHANNEL_ID(pIndexToCH[i]));//�Ǽ�����������ͨ��
			m_pReader[i].bSwitch = true;
			m_pReader[i].chnn = pIndexToCH[i];
		}

		pFileListCH[i].RemoveAll();
	}

	for (i = 0; i<num; ++i)
	{
		outCH |= outCHT[i];
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
long CMultiReader::StartRead(unsigned long startTime, unsigned long endTime, \
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
#if 0
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
#endif

	ULONGLONG outCHT[128] = {0};
	
#if 1
	int iCount = chnnList.GetCount();

	#pragma omp parallel for num_threads(iCount)
	for(int i=0; i<iCount; i++)
	{
		POS pos = chnnList.GetHeadPosition();
		int j=0;
		int channel = 0;

		while(pos)
		{
			if(i==j)
			{
				break;
			}
			channel = chnnList.GetNext(pos);
			j++;
			
		}

		channel = chnnList.GetAt(pos);
		if (m_pReader[i].reader.StartRead(startTime, endTime, channel, pFilePath))
		{
			outCHT[i] = ((ULONGLONG)0x01 << channel);
			m_pReader[i].bSwitch = true;
			m_pReader[i].chnn = channel;
		}
		else
		{
			m_pReader[i].bSwitch = false;
		}
	}
#endif
	
	for(int i=0; i<iCount; i++)
	{
		outCH |= outCHT[i];
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

long CMultiReader::StartRead(unsigned long startTime, const CHANNEL_LIST &chnnList, ULONGLONG & outCH, const char *pFilePath)
{
	if(chnnList.GetCount() > m_maxCHNum)
	{
		return MR_RET_TOO_MANY_CHANNELS;
	}

	//���õ�ͨ������ʼ��ʱ����ӿ�
	outCH = 0;
	unsigned char i = 0;
	unsigned long chnn = 0;

#if 0
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
#endif

	ULONGLONG outCHT[128] = {0};

#if 1
	int iCount = chnnList.GetCount();
	
	#pragma omp parallel for num_threads(iCount)
	for(int i=0;i<iCount;i++)
	{
		POS pos = chnnList.GetHeadPosition();
		int j=0;
		int channel = 0;
		while(pos)
		{
			if(i==j)
			{
				break;
			}
			channel = chnnList.GetNext(pos);
			j++;
			
		}
		channel = chnnList.GetAt(pos);

		if (m_pReader[i].reader.StartRead(startTime, channel, pFilePath))
		{
			outCHT[i] = ((ULONGLONG)0x01 << CHANNEL_ID(channel));
			m_pReader[i].bSwitch = true;
			m_pReader[i].chnn = channel;
		}
		else
		{
			m_pReader[i].bSwitch = false;
		}
	}
#endif

	for(int i=0; i<iCount; i++)
	{
		outCH |= outCHT[i];
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

void CMultiReader::StopRead()
{
	for(int i = 0; i < m_maxCHNum; i++)
	{
		m_pReader[i].bSwitch = false;
		m_pReader[i].time = 0;
		m_pReader[i].reader.StopRead();
	}

	m_readType = READ_TYPE_NULL;
}

long CMultiReader::SetCurTime(bool bForward, unsigned long time, ULONGLONG & outCH)
{
	ULONGLONG outCHArray[64];
	memset(outCHArray,0,sizeof(outCHArray));

	outCH = 0;
	#pragma omp parallel for num_threads(m_maxCHNum)
	for (long index=0; index<m_maxCHNum; ++index)
	{
		if (m_pReader[index].bSwitch)
		{
			if (ONE_CHNN_READ_FAIL != m_pReader[index].reader.SeekFrameToTime(bForward, time))
			{
				outCHArray[index] |= (ULONGLONG)0x01 << CHANNEL_ID(m_pReader[index].chnn);
 			}
			else
			{
				m_pReader[index].bSwitch = false;
				m_pReader[index].chnn	 = INVALID_INDEX(unsigned long);
			}
		}
	}

	for (long index=0; index<m_maxCHNum; ++index)
	{
		outCH |= outCHArray[index];
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
long CMultiReader::SetCurChannel(ULONGLONG chnnBits, ULONGLONG & outCH)
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
			if (ONE_CHNN_READ_FAIL == m_pReader[i].reader.SeekFrameToTime(true, static_cast<unsigned long>(m_lastTime/1000000)))
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


	//��ΪopenMP���ж�ȡ�ļ��������ӿ�ط�ʱ�л��ط�ͨ�����ٶȡ����������˴���ı���������ж����ԣ������������������ʼ����
	ULONGLONG outCHArray[64];
	unsigned char chnnNumNew = 0;
	unsigned char chnnArray[64];
	unsigned char readerArray[64];
	memset(outCHArray,0,sizeof(outCHArray));
	memset(chnnArray,0,sizeof(chnnArray));
	memset(readerArray,0,sizeof(readerArray));
	while (0 != chnnBits)
	{
		ULONGLONG CH = chnnBits;
		while ((0 != CH) && !(CH & 0x01))
		{
			CH = (CH >> 1);
			++ (chnnArray[chnnNumNew]);
		}
		chnnBits &= ~((ULONGLONG)(0x01) << (chnnArray[chnnNumNew]));

		readerArray[chnnNumNew] = INVALID_INDEX(unsigned char);
		for (unsigned char i=0; i<m_maxCHNum; ++i)	//ע��Ҫ���������Դͨ����
		{
			if (!m_pReader[i].bSwitch)
			{
				readerArray[chnnNumNew] = i;//����Դ���á�
				m_pReader[i].bSwitch = true;
				m_pReader[i].chnn	= chnnArray[chnnNumNew];
				m_pReader[i].time	= m_startTime;
				break;
			}
		}
		if (readerArray[chnnNumNew] == INVALID_INDEX(unsigned char))
		{
			return MR_RET_TOO_MANY_CHANNELS;
		}

		chnnNumNew++;
	}

	//���0 != chnnBits������ζ���µ�֮ǰδ������ͨ����
#pragma omp parallel for num_threads(chnnNumNew)
	for(int chnnArrayIndex = 0;chnnArrayIndex<chnnNumNew;++chnnArrayIndex) //˵�����µ�ͨ�����룬��Ҫ����
	{
		unsigned char chnn = chnnArray[chnnArrayIndex];
		unsigned char index = readerArray[chnnArrayIndex];

		if (index != INVALID_INDEX(unsigned char))
		{
			if (READ_BY_START_END == m_readType)
			{
				if (ONE_CHNN_READ_FAIL != m_pReader[index].reader.StartRead(static_cast<unsigned long>(m_lastTime/1000000), 
					static_cast<unsigned long>(m_endTime/1000000), chnn, m_pFilePath))
				{
					//m_pReader[index].bSwitch = true;
					//m_pReader[index].chnn = chnn;
					//m_pReader[index].time = m_startTime;

					outCHArray[chnnArrayIndex] |= ((ULONGLONG)0x01 << CHANNEL_ID(m_pReader[index].chnn));				
				}
			}
			else if (READ_BY_START == m_readType)
			{
				if (ONE_CHNN_READ_FAIL != m_pReader[index].reader.StartRead(static_cast<unsigned long>(m_lastTime/1000000), chnn, m_pFilePath))
				{
					//m_pReader[index].bSwitch = true;
					//m_pReader[index].chnn = chnn;
					//m_pReader[index].time = m_lastTime;

					outCHArray[chnnArrayIndex] |= ((ULONGLONG)0x01 << CHANNEL_ID(m_pReader[index].chnn));
				}
			}
			else
			{
				//��ȡ�ļ��б����ʽ���򲻴��µ�ͨ��
			}
		}
	}

	for (unsigned char chnnArrayIndex = 0;chnnArrayIndex<chnnNumNew;++chnnArrayIndex)
	{
		outCH |= outCHArray[chnnArrayIndex];
	}
	//���ˣ�OpenMP���������


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

long CMultiReader::GetNextFrameType(FRAME_INFO_EX &frameInfo, bool bForward)
{
	int index = GetNextReadChannel(bForward);
	if (index == -1)//����ͨ��û�пɲ�����
	{
		return MR_RET_NOTHING_TO_READ;
	}

	frameInfo.frameType = m_pReader[index].reader.GetFrameType();
	frameInfo.channel	= CHANNEL_ID(m_pReader[index].chnn);
	return MR_RET_OK;
}
/******************************************************************************

�������ܣ��õ���һ֡����
����˵����bVideoOnly - �Ƿ�ֻ�Ƕ�ȡ��Ƶ����
		  pFrameInfo - ����Ҫ��ȡ��һ֡����Ϣ�����ݣ�����ʱ��length��pData�������
		  bForward - �Ƿ����������򲥷�
����ֵ���ɹ�����MR_RET_OK
		���򷵻�MR_RET_NOTHING_TO_READ����ʾ�������ݿɶ�

*******************************************************************************/
long CMultiReader::GetNextFrame(FRAME_INFO_EX &frameInfo, bool bForward /* = true */, bool bVideoOnly /* = false */)
{
	int index = GetNextReadChannel(bForward);
	if (index == -1)//����ͨ��û�пɲ�����
	{
		return MR_RET_NOTHING_TO_READ;
	}

	if(ONE_CHNN_READ_SUC != m_pReader[index].reader.GetFrame(frameInfo))
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
				m_lastTime = frameInfo.time;//��¼���������һ֡��ʱ��
			}
			
			if(SEEK_FRAME_OK != m_pReader[index].reader.SeekFrame(SEEK_NEXT_VFRM))
			{
				frameInfo.frameAttrib = PLAY_FRAME_SEC_END;
			}
		}
		
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
long CMultiReader::GetKeyFrame(FRAME_INFO_EX &frameInfo, long nStepFrame /* = 1 */, bool bForward /* = true */)
{
	int index = GetNextReadChannel(bForward);

	if (index == -1)//����ͨ��û�пɲ�����
	{
		return MR_RET_NOTHING_TO_READ;
	}

	//���ܵ�һ�ε����������ʱ���õ��ĵ�ǰ֡����Ƶ�ؼ�֡������Ҫ����һ�β��ҹؼ�֡���ٶ�
	while(true)
	{
		while ((index != -1) && (ONE_CHNN_READ_SUC != m_pReader[index].reader.GetFrame(frameInfo)))
		{
			index = GetNextReadChannel(bForward);
		}
		
		if (index == -1)//����ͨ��û�пɲ�����
		{
			return MR_RET_NOTHING_TO_READ;
		}
		else
		{
			if (FRAME_TYPE_VIDEO == frameInfo.frameType)
			{
				frameInfo.frameAttrib = PLAY_FRAME_SHOW;
				if (FRAME_TYPE_VIDEO == frameInfo.frameType)
				{
					m_lastTime = frameInfo.time;//��¼���������һ֡��ʱ��
				}
			}
			
			if((FRAME_TYPE_VIDEO_FORMAT != frameInfo.frameType) && (FRAME_TYPE_AUDIO_FORMAT != frameInfo.frameType))
			{
				SEEK_MODE seekMode = bForward ? SEEK_NEXT_KEY : SEEK_PREV_KEY;
				if(SEEK_FRAME_OK != m_pReader[index].reader.SeekFrame(seekMode, nStepFrame))
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
	}
	
	return MR_RET_OK;
}
bool CMultiReader::HasAudioData(long channel)
{
//	return m_pReader[channel].reader.HasAudioData();
	return true;
}

//���ڼ������ͨ���Ƿ񶼴���READ_STATE_BEHIND_SLEEP״̬��
bool CMultiReader::CheckSleep()
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

						Protected Functions

******************************************************************************/

/******************************************************************************

�������ܣ��õ���һ��Ҫ��ȡ���ݵ�ͨ��(����ʱ��˳��)
����˵����bForwardRead��TRUE��ʾ���򲥷ţ�FALSE��ʾ���򲥷�
����ֵ���ҵ���һ��Ҫ��ȡ��ͨ��������ͨ���ţ����򷵻�-1

******************************************************************************/
int CMultiReader::GetNextReadChannel(bool bForwardRead)
{
	int nextReadChannel = -1;
	if(bForwardRead)
	{
		LONGLONG EarlistTime = MAX_TIME;
		for(int i = 0; i < m_maxCHNum; i++)
		{
			if(m_pReader[i].bSwitch)
			{
				//ֻ�д��ڻ״̬��ͨ���ͷ��򲥷��н����˯�߲Ų���ʱ������
				if((m_pReader[i].reader.GetReadState() == READ_STATE_ACTION) \
					|| (m_pReader[i].reader.GetReadState() == READ_STATE_FORWARD_SLEEP))
				{
					m_pReader[i].time = m_pReader[i].reader.GetFrameTime();
					if(m_pReader[i].time < EarlistTime)
					{
						EarlistTime = m_pReader[i].time;
						nextReadChannel = i;
					}
				}
			}//end if(m_pReader[i].bSwitch)
		}
	}//end if(bForward)
	else
	{
		LONGLONG LatestTime = 0;
		for(int i = 0; i < m_maxCHNum; i++)
		{
			if(m_pReader[i].bSwitch)
			{
				//ֻ�д��ڻ״̬��ͨ���������򲥷��н����˯�߲Ų���ʱ������
				if((m_pReader[i].reader.GetReadState() == READ_STATE_ACTION) \
					|| (m_pReader[i].reader.GetReadState() == READ_STATE_BEHIND_SLEEP))
				{
					m_pReader[i].time = m_pReader[i].reader.GetFrameTime();
					if(m_pReader[i].time > LatestTime)
					{
						LatestTime = m_pReader[i].time;
						nextReadChannel = i;
					}
				}
			}//end if(m_pReader[i].bSwitch)
		}
	}//end if(!bForward)

	//�����ڲ����н����˯��
	if((nextReadChannel != -1) && \
		((m_pReader[nextReadChannel].reader.GetReadState() == READ_STATE_FORWARD_SLEEP) \
		|| (m_pReader[nextReadChannel].reader.GetReadState() == READ_STATE_BEHIND_SLEEP)) )
	{
		m_pReader[nextReadChannel].reader.WakeUp();  
	}

	return nextReadChannel;
}
