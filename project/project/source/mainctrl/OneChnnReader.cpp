//COneChnnReader.cpp
#include "MessageMan.h"
#include "OneChnnReader.h"

enum
{	
	GET_NEW_FILE_FAIL,
	GET_NEW_FILE_SUC,
	RESERVER_OLD_FILE
};
//pbulic
COneChnnReader::COneChnnReader()
{
	m_currPos		= NULL;
	m_bOpenFile		= false;
	m_bReAudioFmat	= false;
	m_bReVideoFmat	= false;
	m_channel		= 0;
	m_readerType	= FILE_TYPE_FTVT;
	m_bDynSearch	= false;

	memset(&m_bitmapInfo, 0, sizeof(BITMAPINFOHEADER));
	memset(&m_waveFormat, 0, sizeof(WAVEFORMATEX));
	memset(m_filePath, 0, sizeof(m_filePath));

	m_diskOwnerIndex = 0;
	m_bRegister = false;

	m_videoTime = 0;
}

COneChnnReader::~COneChnnReader()
{

}

bool COneChnnReader::Initial(CReclogManEX *pReclogManEx, unsigned long diskOwnerIndex)
{
	m_pReclogMan = pReclogManEx;
	m_diskOwnerIndex = diskOwnerIndex;
	m_bRegister = false;
	return true;
}

void COneChnnReader::Quit()
{
	//�طŽ�����Ҫ�ͷ����йرյ�
	assert(!m_bRegister);

}

/************************************************************************
����ԭ��	long StartRead(LONGLONG startTime, LONGLONG endTime, const CHANNEL_ID_MAPPER &channelMapper)
��������	ָ��mac��ַ,ͨ����ָ����ʼʱ��ͽ���ʱ��.�������ʱ��֮���������ļ�����򿪵�һ���ļ�
			����λ����ʼʱ��Ľ�ǰ�ؼ�֡,Ϊ������ȡ������׼��.
�������	startTime ������¼��Ŀ�ʼʱ�䡣
			endTime   : ����ʱ��
			channelMapper: ����ָ��ͨ����MAC��ַ
�������	��
����ֵ		ONE_CHNN_READ_SUC���ɹ��� ONE_CHNN_READ_FAIL��ʧ��
************************************************************************/
long COneChnnReader::StartRead(unsigned long startTime, unsigned long endTime, unsigned long chnn, const char *pRecPath /* = NULL */)
{
	//��֤û�п������������Ѿ��ر�
	assert(m_fileList.IsEmpty());
	assert(NULL == m_currPos);
	assert(!m_bRegister);

	//Ŀǰ�ط�ֻ���˸ýӿڣ�������Ҫ�ṩ�Զ��������ܣ����Ը�Ϊ�Զ�������
	m_bDynSearch = true;

	if(pRecPath != NULL)
	{
		strcpy(m_recPath, pRecPath);
	}
	else
	{
		m_recPath[0] = '\0';
	}

	m_channel = chnn;
	//ȡ�õ��ļ��б��Ѵ����m_fileList��
	if (m_pReclogMan->GetFileInfo(m_channel, startTime, m_fileList, m_diskOwnerIndex, true) > 0) 
	{
		//2010-04-14 17:53:00 YSW
		//����ļ��б��ʱ�䲻��������ʱ�������죬����Ϊ����ʧ��
		if ( (m_fileList.GetTail().endTime <= startTime) || \
			(endTime <= m_fileList.GetHead().startTime))
		{
			RemoveFileList();
			return ONE_CHNN_READ_FAIL;
		}

		//2010-01-25 11:29:00 YSW
		//������ʼʱ���ٶ�λ����Ϊ����ѡ��Ŀ�ʼʱ���û��¼�����ݣ�������ϣ����ѡ����ӽ�
		//���ݿ�ʼ�طš�����SeekFrameToTime�л�����startTime�ж��Ƿ����ļ��б��ʱ�������ڣ�����
		//�������������������ͻ�������ʱ������������������
		//����ٴ����޸�ʱ�䣬�������startTime < head.StartTime�������������

		if (startTime < m_fileList.GetHead().startTime)
		{
			startTime = m_fileList.GetHead().startTime;
		}

		return SeekFrameToTime(true, startTime);
	}
	else
	{
		RemoveFileList();
		return ONE_CHNN_READ_FAIL;
	}
}

/************************************************************************
����ԭ��	long StartRead(const CMyList <FILE_INFO_EX> & fileList)
��������	�����ϲ�ָ�����ļ��б��򿪵�һ���ļ�����λ����һ���ؼ�֡��
			�б���ļ�����ͬ��һ��ͨ����
�������	fileList ��������ָ�����ļ��б�
�������	��
����ֵ	ONE_CHNN_READ_SUC���ɹ��� ONE_CHNN_READ_FAIL��ʧ��
************************************************************************/
long COneChnnReader::StartRead(const FILE_INFO_EX_LIST & fileList, const char *pRecPath /* = NULL */)
{
	assert(!fileList.IsEmpty());	//�����б������ļ���
	assert(m_fileList.IsEmpty());	//��֤����Ŀ¼�ǿյ�.
    assert(!m_bRegister);

	m_bDynSearch = false;
	if(pRecPath != NULL)
	{
		strcpy(m_recPath, pRecPath);
	}
	else
	{
		m_recPath[0] = '\0';
	}
	
	//��fileList����һ�ݵ�m_fileList��;
	POS pFilePos = fileList.GetHeadPosition();
	while (NULL != pFilePos) 
	{	
		m_fileList.AddTail(fileList.GetNext(pFilePos));
	}
	
	m_currPos = m_fileList.GetHeadPosition();
	if (GetFilePathEx(m_currPos))		//ȡ�õ�һ���ļ���·��
	{
		FILE_INFO_EX & fileInfo = m_fileList.GetAt(m_currPos);

		if (ONE_CHNN_READ_SUC == OpenFile(fileInfo.startTime,fileInfo.chnn))		//�򿪸��ļ�
		{
			if (SEEK_FRAME_OK == SeekFrame(SEEK_FIRST_KEY))		//��λ�����ļ��ĵ�һ���ؼ�֡
			{
				FILE_INFO_EX &fileInfo = m_fileList.GetHead();
				
				//ȡ�˵�һ���ļ����ͨ���ţ���Ҫ��֤�����ļ�����ͬһ��ͨ������ļ�.	
				m_channel	= fileInfo.chnn;
				m_readState = READ_STATE_ACTION;
                RegisterReadFileIndex();
				return ONE_CHNN_READ_SUC;
			}
			else
			{
				RUN_LOG(RUN_LOG_ITEM_PB_SEEK_TIME_ERR, CRunLog::RUN_Num("The state is:", m_readState), CRunLog::RUN_Num("chnn:", m_channel), m_filePath);
				RemoveFileList();
				CloseFile();
				return ONE_CHNN_READ_FAIL;
			}
		}
		else
		{
			RUN_LOG(RUN_LOG_ITEM_PB_OPEN_FILE_FAIL, CRunLog::RUN_Num("The state is:", m_readState), CRunLog::RUN_Num("chnn:", m_channel), m_filePath);
			RemoveFileList();
			return ONE_CHNN_READ_FAIL;
		}
	}
	else
	{
		RemoveFileList();
		assert(false);
		return ONE_CHNN_READ_FAIL;
	}
	
}

long COneChnnReader::StartRead(unsigned long playTime, unsigned long chnn, const char *pRecPath /* = NULL */)
{
	//��֤û�п������������Ѿ��ر�
	assert(m_fileList.IsEmpty());
	assert(NULL == m_currPos);

	m_bDynSearch = true;

	if(pRecPath != NULL)
	{
		strcpy(m_recPath, pRecPath);
	}
	else
	{
		m_recPath[0] = '\0';
	}

	m_channel = chnn;
	//ȡ�õ��ļ��б��Ѵ����m_fileList��
	if (m_pReclogMan->GetFileInfo(m_channel, playTime, m_fileList, m_diskOwnerIndex, true) > 0) 
	{
		//2010-04-14 17:53:00 YSW
		//����ļ��б��ʱ�䲻��������ʱ�������죬����Ϊ����ʧ��
		if ( (m_fileList.GetTail().endTime <= playTime) || \
			(GetZeroTimeOfDate(playTime) <= m_fileList.GetHead().startTime))
		{
			RemoveFileList();
			return ONE_CHNN_READ_FAIL;
		}

		//2010-01-25 11:29:00 YSW
		//������ʼʱ���ٶ�λ����Ϊ����ѡ��Ŀ�ʼʱ���û��¼�����ݣ�������ϣ����ѡ����ӽ�
		//���ݿ�ʼ�طš�����SeekFrameToTime�л�����playTime�ж��Ƿ����ļ��б��ʱ�������ڣ�����
		//�������������������ͻ�������ʱ������������������
		//����ٴ����޸�ʱ�䣬�������playTime < head.StartTime�������������

		if (playTime < m_fileList.GetHead().startTime)
		{
			playTime = m_fileList.GetHead().startTime;
		}

		return SeekFrameToTime(true, playTime);
	}
	else
	{
		RUN_LOG(RUN_LOG_ITEM_PB_GET_FILE_FAIL, CRunLog::RUN_Time("The time is:", playTime), CRunLog::RUN_Num("chnn:", chnn), "Open file fail");
		RemoveFileList();
		return ONE_CHNN_READ_FAIL;
	}
}

 
/************************************************************************
����ԭ��	void StopRead();
��������	����ļ��б�ֹͣ��ȡ���ر��Ѵ򿪵�¼���ļ���
�������	��
�������	��
����ֵ		��
************************************************************************/
void COneChnnReader::StopRead()
{
	//ɾ���ļ��б�
	RemoveFileList();

	//ֹͣ��ȡ���ر�¼���ļ���������ر�����λ������
	CloseFile();
    assert(!m_bRegister);
	m_readState = READ_STATE_STOP;
}


/************************************************************************
����ԭ��	long GetFrame(FRAME_INFO_EX &frameInfo)
��������	ȡ�õ�ǰ֡.
�������	��
�������	frameInfo  :���һ֡��֡��Ϣ.
����ֵ		ONE_CHNN_READ_SUC���ɹ��� ONE_CHNN_READ_FAIL��ʧ�� ONE_CHNN_READ_SLEEP:˯��״̬
************************************************************************/
long COneChnnReader::GetFrame(FRAME_INFO_EX &frameInfo)
{
	assert(m_bOpenFile);
	assert(NULL != frameInfo.pData);
	
	if (!m_bOpenFile || (NULL == frameInfo.pData))
	{
		return ONE_CHNN_READ_FAIL;
	}

	//��ȡ״̬��Ϊ�״̬ʱ,ֱ�ӷ���ʧ��.
	if (READ_STATE_ACTION != m_readState )
	{
		return ONE_CHNN_READ_SLEEP;
	}
	
	frameInfo.channel	= m_channel;
	
	//�´��ļ�����Ҫ����Ƶ����Ƶ��ʽ�ͳ�ȥ
	if (m_bReVideoFmat)  //��Ƶ��ʽ
	{
		frameInfo.frameType = FRAME_TYPE_VIDEO_FORMAT;
		frameInfo.length = sizeof(BITMAPINFOHEADER);
		memcpy(frameInfo.pData, &m_bitmapInfo, frameInfo.length);		
		m_bReVideoFmat = false;		
		return ONE_CHNN_READ_SUC;
	}
	
	if (m_bReAudioFmat)//��Ƶ��ʽ
	{
		frameInfo.frameType = FRAME_TYPE_AUDIO_FORMAT;
		frameInfo.length = sizeof(WAVEFORMATEX);
		memcpy(frameInfo.pData, &m_waveFormat, frameInfo.length);
		m_bReAudioFmat = false;
		return ONE_CHNN_READ_SUC;
	}
	
	//��ȡһ֡.
	if (ERR_READ_FILE_OK == GetCurrFrame(frameInfo))
	{
		frameInfo.width		= m_bitmapInfo.biWidth;
		frameInfo.height	= m_bitmapInfo.biHeight;
		return ONE_CHNN_READ_SUC;
	}
	else
	{
		return ONE_CHNN_READ_FAIL;
	}
}

/************************************************************************
����ԭ��	LONGLONG GetFrameTime ()
��������	ȡ�õ�ǰ֡��ʱ�䡣
�������	��
�������	��
����ֵ		���ص�ǰ֡��ʱ��
************************************************************************/
LONGLONG COneChnnReader::GetFrameTime()
{
	if (m_readerType == FILE_TYPE_FTVT)
	{
		m_videoTime = m_cRecReader.GetCurrFrameTime();
	}
	else
	{
		m_videoTime = m_cAviReader.GetCurrFrameTime();
	}

	return 	m_videoTime;
}

FRAME_TYPE COneChnnReader::GetFrameType()
{
	if(m_bReVideoFmat)
	{
		return FRAME_TYPE_VIDEO_FORMAT;
	}
	else if(m_bReAudioFmat)
	{
		return FRAME_TYPE_AUDIO_FORMAT;
	}
	else if(m_readerType == FILE_TYPE_FTVT)
	{
		return m_cRecReader.GetCurrFrameType();
	}
	else
	{
		return m_cAviReader.GetCurrFrameType();
	}
}

/************************************************************************
����ԭ��	long SeekFrame(SEEK_MODE mode, unsigned long num = 1);
��������	��λ��һ֡����һ֡����һ���ؼ�֡�����һ���ؼ�֡����ǰ�����������ؼ�֡��
�������	mode : ȡֵ��Χ	SEEK_PREV_VFRM,	SEEK_NEXT_VFRM,	SEEK_FIRST_KEY,SEEK_LAST_KEY,\
				   SEEK_PREV_KEY, SEEK_NEXT_KEY,
			num : ��ǰ��������ؼ�֡����Ŀ��
�������	��
����ֵ	SEEK_FRAME_OK���ɹ���SEEK_FRAME_FAIL��ʧ�� SEEK_FRAME_SLEEP: ˯��״̬��

�޸ļ�¼��
		2009-12-30-20:54:00��YSW��
		�����˶�̬�����ļ�������ɳ���������¼�Ƶ��ļ���
************************************************************************/
long COneChnnReader::SeekFrame(SEEK_MODE mode, unsigned long num /* = 1 */)
{
	//���Ѵ��ļ��ж�λ
	unsigned long ret = SeekVideoFrame(mode, num);
	if (ERR_READ_FILE_OK == ret)
	{
		return SEEK_FRAME_OK;
	}
	else if ((mode == SEEK_NEXT_VFRM) || (mode == SEEK_NEXT_KEY) || (mode ==SEEK_FIRST_KEY))
	{
		//ȡ����һ���ļ�
		if (GET_NEW_FILE_SUC == GetNextFile())			//ȡ�����ļ�
		{
			//��λ����һ���ؼ�֡
			return SeekFrame(SEEK_FIRST_KEY);
		}
		else			//sleep
		{
			if (m_bDynSearch)
			{
				//2010-02-25 11:20:00 YSW
				//�����ȡ��һ���ļ����ɹ�����������SLEEP״̬������Ϊϵͳһ�����Ǵ���¼��״̬�У����ڻطŹ�����¼�����ݷ����˱仯��
				//����ĳ��ʱ���ط��������һ��ʱ���¼���ļ�����������Щ�ļ���������һ���µ�¼���ļ���
				//��ʱ����������֧����ô��������£�Ӧ�����½����������Ը����ļ���Ϣ��
				//�ѵ�ǰ�ļ��б�ɾ������SeekFrameToTime�оͻ����������ļ������ٴζ�λ��
				RemoveFileList();

				if (ONE_CHNN_READ_FAIL != SeekFrameToTime(true, (m_videoTime/1000000)+1))
				{
					return SEEK_FRAME_OK;
				}
			}
			
			//2010-01-25 17:32:00 YSW
			//GetNextFile�ڴ��ļ��б��е��ļ�ʱ������ȫ�����ܴ򿪣����������ȫ�������ǣ����ߴ��̳����ˣ�
			//GetNextFile�ڲ��ڴ�������£����ǰ��ļ��б����գ�������ʧ�ܣ�ͬʱ��m_readState = READ_STATE_STOP��
			//����Ƕ�̬����������ȥ����Ķ����У�ͬ��Ҳ����һ�������������Ƕ�̬��������
			//��Ҫ�ڴ��ж��÷�����ȷ��״̬��

			return (READ_STATE_STOP != m_readState) ? SEEK_FRAME_SLEEP : SEEK_FRAME_FAIL;
		}
		
	}
	else if ((mode == SEEK_PREV_VFRM) || (mode == SEEK_PREV_KEY) || (mode == SEEK_LAST_KEY))
	{
		//ȡ����һ���ļ�
		if (GET_NEW_FILE_SUC == GetPrevFile())			//ȡ�����ļ�
		{
			//��λ�����һ���ؼ�֡
			return SeekFrame(SEEK_LAST_KEY);
		}
		else		//sleep
		{
			if (m_bDynSearch)
			{
				//2010-02-25 11:20:00 YSW
				//������GetNextFile��֧.
				RemoveFileList();

				if (ONE_CHNN_READ_FAIL != SeekFrameToTime(false, (m_videoTime/1000000)-1))
				{
					return SEEK_FRAME_OK;
				}
			}

			//2010-01-25 17:32:00 YSW
			//GetPrevFile�ڴ��ļ��б��е��ļ�ʱ������ȫ�����ܴ򿪣����������ȫ�������ǣ����ߴ��̳����ˣ�
			//GetPrevFile�ڲ��ڴ�������£����ǰ��ļ��б����գ�������ʧ�ܣ�ͬʱ��m_readState = READ_STATE_STOP��
			//����Ƕ�̬����������ȥ����Ķ����У�ͬ��Ҳ����һ�������������Ƕ�̬��������
			//��Ҫ�ڴ��ж��÷�����ȷ��״̬��

			return (READ_STATE_STOP != m_readState) ? SEEK_FRAME_SLEEP : SEEK_FRAME_FAIL;
		}
	}
	else
	{
		RUN_LOG(RUN_LOG_ITEM_PB_SEEK_TIME_ERR, CRunLog::RUN_Time("The mode is:", mode), CRunLog::RUN_Num("chnn:", CHANNEL_ID(m_channel)), "seek fail");
		return SEEK_FRAME_FAIL;
	}
	
}

long COneChnnReader::GetFrameLen()
{
	if (m_readerType == FILE_TYPE_FTVT)
	{
		return m_cRecReader.GetCurrFrameLen();
	}
	else
	{
		return m_cAviReader.GetCurrFrameLen();
	}

}
/************************************************************************
����ԭ��	long SeekFrameToTime(bool bForward, LONGLONG time);
��������	��ʱ�䶨λ��Ƶ֡
�������	time  ��Ҫ��λ��Ƶ֡��ʱ�䡣
			bForward ��ǰ��ȡ��������ǰ�������
�������	��
����ֵ		ONE_CHNN_READ_SUC���ɹ���ONE_CHNN_READ_FAIL��ʧ��; 
			ONE_CHNN_READ_SLEEP: ǰ��û���ļ�,sleep ״̬.
************************************************************************/
long COneChnnReader::SeekFrameToTime(bool bForward, unsigned long time)
{
	if (m_fileList.IsEmpty())
	{
		assert (NULL == m_currPos);
		assert (READ_STATE_STOP == m_readState);

		if (m_bDynSearch)
		{
			if (0 == m_pReclogMan->GetFileInfo(m_channel, time, m_fileList, m_diskOwnerIndex, true))
			{
				return ONE_CHNN_READ_FAIL;
			}
			//else �������ļ���������һ���ȶԻ��ڡ�
		}
		else
		{
			return ONE_CHNN_READ_FAIL;
		}
	}
	else
	{
		//time�����б��ʱ�������ڣ������Զ�����ģʽ����ɾ��ԭ�����ļ����ٴ�����������
		//��������Զ�ģʽ������ʹ��ԭ�����ļ����ȶԡ�
		if (((time < m_fileList.GetHead().startTime) || (m_fileList.GetTail().endTime < time)) && m_bDynSearch)
		{
			RemoveFileList();

			RUN_LOG(RUN_LOG_ITEM_PB_FILE_NULL, CRunLog::RUN_Time("The time is:", time), CRunLog::RUN_Num("chnn:", m_channel), "playback all files in list,and search file again");
			
			//��������������ʽ��Ȼ�ɹ�������䷵���㣬���ʾû���κ��ļ����ڡ���ʱ���Խ�״̬��Ϊֹͣ״̬��
			if (0 == m_pReclogMan->GetFileInfo(m_channel, time, m_fileList, m_diskOwnerIndex, true))
			{
				assert (false);
				CloseFile();
				m_readState = READ_STATE_STOP;
				return ONE_CHNN_READ_FAIL;
			}
			//�������ļ��������ȶԻ��ڡ�
		}
	}

	unsigned long startTime = m_fileList.GetHead().startTime;
	unsigned long endTime = m_fileList.GetTail().endTime;
   
	if (time < startTime)//��ʱ������ǰ����Ȼֻ�����SLEEP��״̬
	{
		POS pos = m_fileList.GetHeadPosition();
		while (NULL != pos)
		{
			if (m_currPos == pos)//���ļ��Ѿ���
			{
				unsigned long seekRet = SeekKeyFrameToTime(time);
				assert(ERR_READ_FILE_SEEK_TO_END != seekRet);
				if ((ERR_READ_FILE_OK == seekRet) || (ERR_READ_FILE_SEEK_TO_BEGIN == seekRet))	//
				{
					m_readState = READ_STATE_FORWARD_SLEEP;
					return ONE_CHNN_READ_SLEEP;
				}
				//���ɹ�����������һ���ļ���
			}
			else
			{
				if (GetFilePathEx(pos))		//ȡ�õ�һ���ļ���·��
				{
					CloseFile();

					FILE_INFO_EX & fileInfo = m_fileList.GetAt(pos);
					if (ONE_CHNN_READ_SUC == OpenFile(fileInfo.startTime,fileInfo.chnn))		//�򿪸��ļ�
					{
						unsigned long seekRet = SeekKeyFrameToTime(time);
						assert(ERR_READ_FILE_SEEK_TO_END != seekRet);
						if ((ERR_READ_FILE_OK == seekRet) || (ERR_READ_FILE_SEEK_TO_BEGIN == seekRet))	//
						{
							m_currPos	= pos;
							m_readState = READ_STATE_FORWARD_SLEEP;
							RegisterReadFileIndex();
							return ONE_CHNN_READ_SLEEP;
						}
					}
				}
			}

			m_fileList.GetNext(pos);
		}
	}
	else if ((startTime <= time) && (time < endTime))//��ʱ��������
	{
		//2010-01-25 15:31:00 YSW
		//(time < endTime)������ȣ���Ϊ��ĳ���ļ�����ʱ�������ŻῪʼ��һ���ļ�������
		//���ǲ�����Ϊ��λ�����Էǳ��п���ǰ���ļ��Ľ���ʱ����ں�һ���ļ��Ŀ�ʼʱ�䡣

		/*
		��ͷ��ʼ�Һ��ʵ��ļ��ڵ㣨��ǰʽ���������ļ��б�
		��������ǰ������󣬶���Ҫ�ҵ�����time��һ���ؼ�֡�����Կ��Բ���
		����Сʱ����ļ���ʼ����������������������ǰ������Ҫ��
		*/
		POS tempPos = NULL;
		POS pos = m_fileList.GetHeadPosition();
		while (NULL != pos)
		{
			FILE_INFO_EX & node = m_fileList.GetAt(pos);
			
			//2010-01-25 15:31:00 YSW
			//if (time <= node.endTime)
			//�����ǲ���ĳ���ļ��Ŀ�ʼʱ����Ϊ�طŵ����ʱ��ʱ��������ط���λ�Ļ������(time <= node.endTime)
			//��������ǰ����ļ������ж�λ���ܶ�λ�����һ���ؼ�֡�������Գ����˵��һ���¼��طţ�ȴ�ط���
			//ǰ��һ���ļ��������
			if (time < node.endTime)
			{
				tempPos	= pos;
				break;
			}
			m_fileList.GetNext(pos);
		}

		//����ҵ����ǵ�ǰ�Ѿ��򿪵��ļ������Ƚ���seek������
		if ((NULL != tempPos) && (tempPos == m_currPos))
		{
			if (bForward)
			{
				unsigned long seekRet = SeekKeyFrameToTime(time);
				//assert(ERR_READ_FILE_SEEK_TO_END != seekRet); �˴��Ǵ��ڶ�λ�����һ֡�����
				if ((ERR_READ_FILE_SEEK_TO_END == seekRet) || (ERR_READ_FILE_OK == seekRet) || (ERR_READ_FILE_SEEK_TO_BEGIN == seekRet))
				{
					m_readState = READ_STATE_ACTION;
					return ONE_CHNN_READ_SUC;
				}
			}
			else
			{
				unsigned long seekRet = SeekKeyFrameToTime(time);
				assert(ERR_READ_FILE_SEEK_TO_BEGIN != seekRet);
				if ((ERR_READ_FILE_OK == seekRet) || (ERR_READ_FILE_SEEK_TO_END == seekRet))	//
				{
					m_readState = READ_STATE_ACTION;
					return ONE_CHNN_READ_SUC;
				}
			}
		}
	
		m_currPos = tempPos;
		if (bForward)
		{
			POS pos = m_currPos;
			while (NULL != pos)
			{
				if (GetFilePathEx(pos))		//ȡ�õ�һ���ļ���·��
				{
					CloseFile();

					FILE_INFO_EX & fileInfo = m_fileList.GetAt(pos);
					if (ONE_CHNN_READ_SUC == OpenFile(fileInfo.startTime,fileInfo.chnn))		//�򿪸��ļ�
					{
						unsigned long seekRet = SeekKeyFrameToTime(time);
						//assert(ERR_READ_FILE_SEEK_TO_END != seekRet); �˴��Ǵ��ڶ�λ�����һ֡�����
					    if ((ERR_READ_FILE_SEEK_TO_END == seekRet) || (ERR_READ_FILE_OK == seekRet) || (ERR_READ_FILE_SEEK_TO_BEGIN == seekRet))	//
						{
							m_currPos	= pos;
							m_readState = READ_STATE_ACTION;
							RegisterReadFileIndex();
							return ONE_CHNN_READ_SUC;
						}
					}
				}

				m_fileList.GetNext(pos);
			}

			pos = m_currPos;
			while (NULL != pos)
			{
				if (GetFilePathEx(pos))		//ȡ�õ�һ���ļ���·��
				{
					CloseFile();

					FILE_INFO_EX & fileInfo = m_fileList.GetAt(pos);
					if (ONE_CHNN_READ_SUC == OpenFile(fileInfo.startTime,fileInfo.chnn))		//�򿪸��ļ�
					{
						unsigned long seekRet = SeekKeyFrameToTime(time);
						assert(seekRet != ERR_READ_FILE_SEEK_TO_BEGIN);
						if ((ERR_READ_FILE_OK == seekRet) || (ERR_READ_FILE_SEEK_TO_END == seekRet))	//
						{
							m_currPos	= pos;
							m_readState = READ_STATE_BEHIND_SLEEP;
							RegisterReadFileIndex();
							return ONE_CHNN_READ_SLEEP;
						}
					}
				}

				m_fileList.GetPrev(pos);
			}
		}
		else	//!bForward
		{
			pos = m_currPos;
			while (NULL != pos)
			{
				if (GetFilePathEx(pos))		//ȡ�õ�һ���ļ���·��
				{
					CloseFile();

					FILE_INFO_EX & fileInfo = m_fileList.GetAt(pos);
					if (ONE_CHNN_READ_SUC == OpenFile(fileInfo.startTime,fileInfo.chnn))		//�򿪸��ļ�
					{
						unsigned long seekRet = SeekKeyFrameToTime(time);
						//assert(ERR_READ_FILE_SEEK_TO_BEGIN != seekRet); �˴��Ǵ��ڶ�λ����һ֡����������磬ֻ��һ���ļ�time���ڵ�һ֡��ʱ�䡣
						if ((ERR_READ_FILE_SEEK_TO_BEGIN == seekRet) || (ERR_READ_FILE_OK == seekRet) || (ERR_READ_FILE_SEEK_TO_END == seekRet))
						{
							m_currPos	= pos;
							m_readState = READ_STATE_ACTION;
							RegisterReadFileIndex();
							return ONE_CHNN_READ_SUC;
						}
					}
				}

				m_fileList.GetPrev(pos);
			}

			POS pos = m_currPos;
			while (NULL != pos)
			{
				if (GetFilePathEx(pos))		//ȡ�õ�һ���ļ���·��
				{
					CloseFile();

					FILE_INFO_EX & fileInfo = m_fileList.GetAt(pos);
					if (ONE_CHNN_READ_SUC == OpenFile(fileInfo.startTime,fileInfo.chnn))		//�򿪸��ļ�
					{
						unsigned long seekRet = SeekKeyFrameToTime(time);
						assert(ERR_READ_FILE_SEEK_TO_END != seekRet);
						if ((ERR_READ_FILE_OK == seekRet) || (ERR_READ_FILE_SEEK_TO_BEGIN == seekRet))	//
						{
							m_currPos	= pos;
							m_readState = READ_STATE_FORWARD_SLEEP;
							RegisterReadFileIndex();
							return ONE_CHNN_READ_SLEEP;
						}
					}
				}

				m_fileList.GetNext(pos);
			}
		}
	}
	else//��ʱ������󣬵�Ȼֻ����ǰSLEEP��״̬
	{
		POS pos = m_fileList.GetTailPosition();
		while (NULL != pos)
		{
			if (m_currPos == pos)//���ļ��Ѿ���
			{
				unsigned long seekRet = SeekKeyFrameToTime(time);
				assert(ERR_READ_FILE_SEEK_TO_BEGIN != seekRet);
				if ((ERR_READ_FILE_OK == seekRet) || (ERR_READ_FILE_SEEK_TO_END == seekRet))	//
				{
					m_readState = READ_STATE_BEHIND_SLEEP;
					return ONE_CHNN_READ_SLEEP;
				}
				//���ɹ���������ǰһ���ļ���
			}
			else
			{
				if (GetFilePathEx(pos))		//ȡ�õ�һ���ļ���·��
				{
					CloseFile();

					FILE_INFO_EX & fileInfo = m_fileList.GetAt(pos);
					if (ONE_CHNN_READ_SUC == OpenFile(fileInfo.startTime,fileInfo.chnn))		//�򿪸��ļ�
					{
						unsigned long seekRet = SeekKeyFrameToTime(time);
						assert(ERR_READ_FILE_SEEK_TO_BEGIN != seekRet);
						if ((ERR_READ_FILE_OK == seekRet) || (ERR_READ_FILE_SEEK_TO_END == seekRet))	//
						{
							m_currPos	= pos;
							m_readState = READ_STATE_BEHIND_SLEEP;
							RegisterReadFileIndex();
							return ONE_CHNN_READ_SLEEP;
						}
					}
				}
			}

			m_fileList.GetPrev(pos);
		}

		
	}

	//�޸�ǰ��
	/*
	//����Ҳ������ʵ��ļ�������������������������ǲ��ܴ��κ�һ���ļ�����ת��ֹͣ״̬��
	assert (false);
	*/
	//�޸ĺ�ԭ��������Ժ�˵��������.//����û���ҵ��ļ�������������ļ��б���ֻ��һ������¼����ļ����ʹ��ڴ򲻿�����ļ������������Ӳ�̴�����ô�ʹ򲻿��ļ�
    //assert (false);
	RUN_LOG(RUN_LOG_ITEM_PB_OPEN_ALL_FILE_FAIL, CRunLog::RUN_Time("The time is:", time), CRunLog::RUN_Num("chnn:", m_channel), "open all file List faild");

	RemoveFileList();
	CloseFile();
	m_readState = READ_STATE_STOP;
	return ONE_CHNN_READ_FAIL;
}

/************************************************************************
����ԭ��	long  GetReadState();
��������	ȡ�õ�ǰ��ͨ������״̬.
�������	��
�������	��
����ֵ		���ص�ǰ��ͨ������״̬.
************************************************************************/
long COneChnnReader::GetReadState()
{
	return m_readState;
}

/************************************************************************
����ԭ��	long WakeUp()
��������	��λ�ô�û������ʱ������ص������ݵ�����,���¶�λ���ٽ��Ĺؼ�֡.
�������	��
�������	��
����ֵ		ONE_CHNN_READ_SUC���ɹ��� ONE_CHNN_READ_FAIL��ʧ��
************************************************************************/
long COneChnnReader::WakeUp()
{
	assert (m_readState != READ_STATE_ACTION );	//�ɶ�ȡ״̬���ܵ��øú���
	assert (m_readState != READ_STATE_STOP);	//��ʼ״̬���ܵ��øú���
	
	switch(m_readState)
	{
	case READ_STATE_FORWARD_SLEEP:
		m_readState = READ_STATE_ACTION;
		return ONE_CHNN_READ_SUC;
		break;
	case READ_STATE_BEHIND_SLEEP:
		m_readState = READ_STATE_ACTION;
		return ONE_CHNN_READ_SUC;
		break;
	default:
		break;
	}
	return ONE_CHNN_READ_FAIL;
	
}

//protected
/************************************************************************
����ԭ��	long OpenFile(unsigned long startTime)
��������	��m_filePathָ����¼���ļ�.��ȡ������Ƶ��ʽ��Ϣ��
�������	��
�������	��
����ֵ		ONE_CHNN_READ_SUC���ɹ��� ONE_CHNN_READ_FAIL��ʧ��
************************************************************************/
long COneChnnReader::OpenFile(unsigned long startTime,unsigned long channel)
{
	if(m_bOpenFile)
	{
		CloseFile();
		m_bOpenFile = false;
	}

	FILE_BUF_TYPE fileType = FILE_BUF_TYPE_LOCAL;
	if(channel<CProduct::Instance()->LocalVideoInputNum())
	{
		fileType = FILE_BUF_TYPE_LOCAL;
	}
	else if(channel<CProduct::Instance()->VideoInputNum())
	{
		fileType = FILE_BUF_TYPE_IP;
	}
	else
	{
		fileType = FILE_BUF_TYPE_MIN;
	}
		
	if (m_cRecReader.Open(m_filePath,fileType))
	{
		m_readerType = FILE_TYPE_FTVT;
	}
	else
	{
		if (m_cAviReader.Open(m_filePath))
		{
			m_readerType = FILE_TYPE_AVI;
		}
		else
		{
			return ONE_CHNN_READ_FAIL;
		}
	}
	
	//2010-01-25 14:08:00 YSW
	//startTime��ֵ�����������һ����־��¼�е��ļ��Ŀ�ʼʱ�䡣������־��¼������ļ���ĳ���¼��Ŀ�ʼʱ��
	//�����ļ���ͷ��ʱ���ǲ��Ǻ�startTime�ӽ���������ӽ������ʾ�������������ļ�����Ϣ��
	//��Ϊ¼�񸲸ǣ�����ʵ�����ݸ�д�ˣ���m_fileList�б��ֵ���Ϣ����Ӧ�ˣ����Բ��ܴ򿪸��ļ�
	//���ж�ȡ���������ɴ��ļ�ʧ�ܵ��������
	{
		unsigned long fileStartTime = 0;
		unsigned long fileEndTime = 0;
		if (FILE_TYPE_FTVT == m_readerType)
		{
			m_cRecReader.GetTimeRange(fileStartTime, fileEndTime);
			if (0 == fileEndTime)
			{
				fileEndTime = GetCurrTime32() - 60;
			}

			//2010-02-08 21:33:00 YSW
			//������һЩ֡�ʺܵ͵������ʱ�����Ƚϳ���������10�����ж���120��������ɼ��������
			if ((startTime < (fileStartTime - 120)) || (startTime >= fileEndTime))
			{
				m_cRecReader.Close();			

				RUN_LOG(RUN_LOG_ITEM_PB_FILE_REWRITE, CRunLog::RUN_Time("The time is:", startTime), CRunLog::RUN_Num("chnn:", m_channel), "file has been rewrite");
				printf("%s:%s:%d, time error\n", __FUNCTION__, __FILE__, __LINE__);
				PrintfBit64(startTime, true);
				PrintfBit64(fileStartTime, true);
				PrintfBit64(fileEndTime, true);

				return ONE_CHNN_READ_FAIL;
			}
		}
		else
		{
			m_cAviReader.SeekVideoFrame(SEEK_FIRST_KEY);
			fileStartTime = static_cast<unsigned long>(m_cAviReader.GetCurrFrameTime()/1000000);
			m_cAviReader.SeekVideoFrame(SEEK_LAST_KEY);
			fileEndTime = static_cast<unsigned long>(m_cAviReader.GetCurrFrameTime()/1000000);
			        
			if ((startTime < fileStartTime - 120) || (startTime > fileEndTime))
			{
				m_cAviReader.Close();			

				RUN_LOG(RUN_LOG_ITEM_PB_FILE_REWRITE, CRunLog::RUN_Time("The time is:", startTime), CRunLog::RUN_Num("chnn:", m_channel), "file has been rewrite");
				return ONE_CHNN_READ_FAIL;
			}
			m_cAviReader.Close();
			m_cAviReader.Open(m_filePath);						
		}		
	}
	

	//�������ˣ�ͨ���˲Ž�������Ĳ��衣
	if(GetVideoInfo(m_bitmapInfo))
	{
		m_bReVideoFmat = true;
	}
	else
	{
		//��Ȼ�Ǳ�������,��û������֡,Ӧ�ѳ��쳣,��������.
		RUN_LOG(RUN_LOG_ITEM_PB_OPEN_FILE_FAIL, CRunLog::RUN_Time("The time is:", startTime), CRunLog::RUN_Num("chnn:", m_channel), "open file fail");
		assert(false);

		return ONE_CHNN_READ_FAIL;
	}
	
	if(GetAudioInfo(m_waveFormat))
	{
		m_bReAudioFmat = true;
	}
	else
	{
		m_bReAudioFmat = false;
	}
	
	m_bOpenFile = true;

	printf("channel %d open file %s succ\n", CHANNEL_ID(m_channel), m_filePath);

	return ONE_CHNN_READ_SUC;
	
}

/************************************************************************
����ԭ��	void CloseFile()
��������	�ر�¼���ļ���
�������	��
�������	��
����ֵ		��
************************************************************************/
void COneChnnReader::CloseFile()
{
//	assert(true == m_bOpenFile);
	if (m_bRegister)
	{
		assert(m_bOpenFile);
		CancelReadFileIndex();
	}

	if(m_bOpenFile)
	{
		if (m_readerType == FILE_TYPE_FTVT)
		{
			m_cRecReader.Close();
		}
		else
		{
			m_cAviReader.Close();
		}
		m_bOpenFile = false;
	}
}

/************************************************************************
����ԭ��	long GetPrevFile()
��������	ȡ����һ���ļ�
�������	��
�������	��
����ֵ		GET_NEW_FILE_SUC: ȡ�����ļ��ɹ���GET_NEW_FILE_FAIL��ȡ�����ļ�ʧ��
			RESERVER_OLD_FILE:  ǰ���Ѿ�û���ļ�,����ԭ���ļ�.
************************************************************************/
long COneChnnReader::GetPrevFile()
{
	assert (NULL != m_currPos);
	POS tmpPos = m_currPos;
	
	m_fileList.GetPrev(tmpPos);
	while (tmpPos != NULL)
	{
		if (m_bOpenFile)
		{
			CloseFile();
		}
		
		if (GetFilePathEx(tmpPos))
		{
			FILE_INFO_EX & fileInfo = m_fileList.GetAt(tmpPos);

			//�����ļ�ʧ��
			if (ONE_CHNN_READ_SUC == OpenFile(fileInfo.startTime,fileInfo.chnn))
			{
				m_currPos = tmpPos;
				RegisterReadFileIndex();
				return GET_NEW_FILE_SUC;
			}
		}
		
		//������ȡ���ļ�·��ʧ�ܻ��ߴ��ļ�ʧ�ܵ�ʱ��ȡ��һ���ļ�
		m_fileList.GetPrev(tmpPos);
	}
	
	if (!m_bOpenFile)
	{
		if (GetFilePathEx(m_currPos))
		{
			FILE_INFO_EX & fileInfo = m_fileList.GetAt(m_currPos);
			if (ONE_CHNN_READ_SUC == OpenFile(fileInfo.startTime,fileInfo.chnn))
			{
				if (ERR_READ_FILE_OK == SeekVideoFrame(SEEK_FIRST_KEY))
				{
					m_readState = READ_STATE_FORWARD_SLEEP;
					RegisterReadFileIndex();
					return RESERVER_OLD_FILE;
				}
			}
		}
		//�ָ��ֳ�ʧ��
		//assert(false);

		//2010-01-25 17:22:00��YSW��
		//�����ڴ˹����У���ǰ�ļ�Ҳ�����ǵ����������Ӧ������б�����ʧ��
		RemoveFileList();
		m_readState = READ_STATE_STOP;
		return GET_NEW_FILE_FAIL;
	}

	//2009-12-30 11:49:00�޸ģ�YSW����GetNextFileֻ����SeekFrame�����е��ã���
	//�ڵ��÷��غ��ٴε���SeekFrame(SEEK_LAST_KEY)��Ȼ�󷵻أ��ټ�֮�˴���û���ж�SeekFrame(SEEK_LAST_KEY)�ķ���ֵ
	//��ʵ����һ�������������Σ���˰Ѵ˴���ȥ����
	//SeekFrame(SEEK_LAST_KEY);
	m_readState = READ_STATE_FORWARD_SLEEP;
	return RESERVER_OLD_FILE;
}

/************************************************************************
����ԭ��	long GetNextFile()
��������	ȡ����һ���ļ�
�������	��
�������	��
����ֵ		GET_NEW_FILE_SUC: ȡ�����ļ��ɹ���GET_NEW_FILE_FAIL��ȡ�����ļ�ʧ��
			RESERVER_OLD_FILE:  �����Ѿ�û���ļ�,����ԭ���ļ�.
************************************************************************/
long COneChnnReader::GetNextFile()
{
	assert (NULL != m_currPos);
	POS tmpPos = m_currPos;
	
	m_fileList.GetNext(tmpPos);	
	while (tmpPos != NULL)
	{
		if (m_bOpenFile)
		{						
			CloseFile();
		}
		
		if (GetFilePathEx(tmpPos))
		{
			FILE_INFO_EX & fileInfo = m_fileList.GetAt(tmpPos);
			if (ONE_CHNN_READ_SUC == OpenFile(fileInfo.startTime,fileInfo.chnn))
			{
				m_currPos = tmpPos;
				RegisterReadFileIndex();
				return GET_NEW_FILE_SUC;
			}
		}
		//������ȡ���ļ�·��ʧ�ܻ��ߴ��ļ�ʧ�ܵ�ʱ��ȡ��һ���ļ�
		m_fileList.GetNext(tmpPos);
	}
	
	//��һ���䳢�Դ�һЩ�ļ���������ΪĳЩԭ��ʧ���ˣ�����Ϊδ��״̬
	//��ʱ��Ҫ�ָ��ֳ�
	if (!m_bOpenFile)
	{
		if (GetFilePathEx(m_currPos))
		{
			FILE_INFO_EX & fileInfo = m_fileList.GetAt(m_currPos);
			if (ONE_CHNN_READ_SUC == OpenFile(fileInfo.startTime,fileInfo.chnn))
			{
				if (ERR_READ_FILE_OK == SeekVideoFrame(SEEK_LAST_KEY))
				{
					m_readState = READ_STATE_BEHIND_SLEEP;
					RegisterReadFileIndex();
					return RESERVER_OLD_FILE;
				}
			}
		}
		//�ָ��ֳ�ʧ��
		//assert(false);

		//2010-01-25 17:22:00��YSW��
		//�����ڴ˹����У���ǰ�ļ�Ҳ�����ǵ����������Ӧ������б�����ʧ��
		RemoveFileList();
		m_readState = READ_STATE_STOP;
		return GET_NEW_FILE_FAIL;
	}

	//���ܵ�ǰ�ļ�Ϊ�б�������һ���ļ�����ô��δ���Դ������ļ�
	//��û�лظ��ֳ��Ĳ���������ֱ�ӽ���SLEEP״̬��

	//2009-12-30 11:49:00�޸ģ�YSW����GetNextFileֻ����SeekFrame�����е��ã���
	//�ڵ��÷��غ��ٴε���SeekFrame(SEEK_LAST_KEY)��Ȼ�󷵻أ��ټ�֮�˴���û���ж�SeekFrame(SEEK_LAST_KEY)�ķ���ֵ
	//��ʵ����һ�������������Σ���˰Ѵ˴���ȥ����
	//SeekFrame(SEEK_LAST_KEY);
	m_readState = READ_STATE_BEHIND_SLEEP;
	return RESERVER_OLD_FILE;
}


/************************************************************************
����ԭ��	void COneChnnReader::RemoveFileList()
��������	ɾ���ļ�����
�������	��
�������	��
����ֵ		��
************************************************************************/	
void COneChnnReader::RemoveFileList()
{
	m_fileList.RemoveAll();
	m_currPos = NULL;
	m_readState = READ_STATE_STOP;
}


/************************************************************************
����ԭ��	bool GetFilePathEx(POS filePos)
��������	ȡ���ļ���·��
�������	filePos  ��filePosָ��m_ fileList��ĳ������λ��
�������	��
����ֵ		true���ɹ��� false��ʧ��

************************************************************************/
bool COneChnnReader::GetFilePathEx(POS filePos)
{
	assert(filePos != NULL);
	assert(!(m_fileList.IsEmpty()));
	memset(m_filePath, 0, sizeof(m_filePath));
	FILE_INFO_EX &fileInfo = m_fileList.GetAt(filePos);

#ifdef WIN32
#ifdef __ENVIRONMENT_VIDEO_PLAY__
	if (1 < strlen(m_recPath))
	{
		sprintf(m_filePath, "%s\\%08d.dat", m_recPath, fileInfo.fileIndex);
	}
	else
	{
		assert (1 == strlen(m_recPath));
		sprintf(m_filePath, "%c:\\%08d.dat", m_recPath[0]+fileInfo.diskIndex, fileInfo.fileIndex);
	}
#else
	if (1 < strlen(m_recPath))
	{
		sprintf(m_filePath, "%s\\%02d\\%08d.dat", m_recPath, fileInfo.diskIndex, fileInfo.fileIndex);
	}
	else
	{
		assert (1 == strlen(m_recPath));
		sprintf(m_filePath, "%c:\\%08d.dat", m_recPath[0]+fileInfo.diskIndex, fileInfo.fileIndex);
	}
#endif
#else
	sprintf(m_filePath,"%s/%02d/%08d.dat", m_recPath, fileInfo.diskIndex, fileInfo.fileIndex);
#endif

	return true;
}

unsigned long COneChnnReader::SeekVideoFrame(SEEK_MODE mode, unsigned long seekNum)
{
	if ((SEEK_NEXT_VFRM == mode) && (seekNum == 1))
	{
		if (m_readerType == FILE_TYPE_FTVT)
		{
			return m_cRecReader.SeekNextFrame();
		}
		else
		{
			return m_cAviReader.SeekNextFrame();
		}

	}

	if (m_readerType == FILE_TYPE_FTVT)
	{
		return m_cRecReader.SeekVideoFrame(mode, seekNum);
	}
	else
	{
		return m_cAviReader.SeekVideoFrame(mode, seekNum);
	}
}

unsigned long COneChnnReader::SeekKeyFrameToTime(unsigned long time)
{
	if (m_readerType == FILE_TYPE_FTVT)
	{
		return m_cRecReader.SeekKeyFrameToTime((LONGLONG)time*1000000);
	}
	else
	{
		return m_cAviReader.SeekKeyFrameToTime((LONGLONG)time*1000000);
	}
}

bool COneChnnReader::GetVideoInfo(BITMAPINFOHEADER &bitmapInfo)
{
	if (m_readerType == FILE_TYPE_FTVT)
	{
		if(m_cRecReader.GetFrameNum(FRAME_TYPE_VIDEO) > 0)
		{
			if (m_cRecReader.GetVideoInfo(&bitmapInfo) == ERR_READ_FILE_OK)
			{
				return true;
			}
		}
	}
	else
	{
		if(m_cAviReader.GetFrameNum(FRAME_TYPE_VIDEO) > 0)
		{
			if (m_cAviReader.GetVideoInfo(&bitmapInfo) == ERR_READ_FILE_OK)
			{
				return true;
			}
		}
	}
	return false;
}

bool COneChnnReader::GetAudioInfo(WAVEFORMATEX &waveFormat)
{
	if (m_readerType == FILE_TYPE_FTVT)
	{
		if(m_cRecReader.GetFrameNum(FRAME_TYPE_AUDIO) > 0)
		{
			if (m_cRecReader.GetAudioInfo(&waveFormat) == ERR_READ_FILE_OK)
			{
				return true;
			}
		}
	}
	else
	{
		if(m_cAviReader.GetFrameNum(FRAME_TYPE_AUDIO) > 0)
		{
			if (m_cAviReader.GetAudioInfo(&waveFormat) == ERR_READ_FILE_OK)
			{
				return true;
			}
		}
	}
	return false;	
}

unsigned long COneChnnReader::GetCurrFrame(FRAME_INFO_EX &frameInfo)
{
	if (m_readerType == FILE_TYPE_FTVT)
	{
		return m_cRecReader.GetCurrFrame(frameInfo);
	}
	else
	{
		return m_cAviReader.GetCurrFrame(frameInfo);
	}
}


void COneChnnReader::RegisterReadFileIndex( )
{
	assert(m_fileList.GetCount() > 0);
	assert(NULL != m_currPos);
	assert(!m_bRegister);

	FILE_INFO_EX &reading = m_fileList.GetAt(m_currPos);
	m_readFileInfo = reading;
	m_pReclogMan->RegisterReadFile(m_readFileInfo.chnn, m_readFileInfo.diskIndex, m_readFileInfo.fileIndex);
	m_bRegister = true;
}


void COneChnnReader::CancelReadFileIndex( )
{
	assert(m_bRegister);
	m_pReclogMan->CancelReadFile(m_readFileInfo.chnn, m_readFileInfo.diskIndex, m_readFileInfo.fileIndex);
	m_bRegister = false;
}
















