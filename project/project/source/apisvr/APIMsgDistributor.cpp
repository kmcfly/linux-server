#include "APIMsgDistributor.h"
#include "APIStreamDispatch.h"


CAPIMsgDistributor::CAPIMsgDistributor()
{
	m_hMessageThread = PUB_CreateThread(MessageThread, this, &m_bMessageProcRun);
	if(PUB_CREATE_THREAD_FAIL == m_hMessageThread)
	{
		m_bMessageProcRun = false;
		printf("%s %d err\n", __FILE__, __LINE__);
	}
}

CAPIMsgDistributor::~CAPIMsgDistributor()
{
	if(m_bMessageProcRun)
	{
		PUB_ExitThread(&m_hMessageThread, &m_bMessageProcRun);
	}
}

PUB_THREAD_RESULT PUB_THREAD_CALL CAPIMsgDistributor::MessageThread( void *pParam )
{
	CAPIMsgDistributor *pThis = (CAPIMsgDistributor *)pParam;
	pThis->MessageProc();

#ifndef WIN32
	pid_t pid = getpid();
	pid_t tid = syscall(__NR_gettid);
	printf("%s, %d, pid = %d, tid=%d\n", __FILE__, __LINE__, pid, tid);
#endif

	return 0;
}


void CAPIMsgDistributor::MessageProc()
{
	MESSAGE_DATA msgData;
	while(m_bMessageProcRun)
	{
		if (!CMessageMan::Instance()->GetMsgFromServer(msgData, true, API_SERVER))
		{
			PUB_Sleep(100);
			continue;
		}

		Distribute(msgData);

		CMessageMan::Instance()->ReleaseMsgBuff(msgData);
		PUB_Sleep(10);
	}
}

int CAPIMsgDistributor::AddReceiver( CAPIMsgDistributorInterface * pObj )
{
	m_vec.push_back(pObj);

	return true;
}

void CAPIMsgDistributor::Distribute(MESSAGE_DATA &msgData)
{
	for (int i=0; i<m_vec.size(); i++)
	{
		m_vec[i]->GetMessage(msgData);
	}
}


bool CAPIMsgDistributorInterface::TimeConvert( const NETNS::DEV_DATE& recDate, const NETNS::DEV_TIME& recTime, unsigned long &time )
{
	if (recDate.wYear < 1900 || recDate.wMonth < 1 || recDate.wMonth > 12)
	{
		return false;
	}
	tm t;
	t.tm_year = recDate.wYear - 1900;
	t.tm_mon = recDate.wMonth - 1;
	t.tm_mday = recDate.wDay;
	t.tm_hour = recTime.wHour;
	t.tm_min = recTime.wMinute;
	t.tm_sec = recTime.wSecond;
	time = TmToDVRTime32(t);

	return true;
}

bool CAPIMsgDistributorInterface::ConvertTime( unsigned long time, NETNS::DEV_DATE& recDate, NETNS::DEV_TIME& recTime )
{
	tm t = DVRTime32ToTm(time);
	recDate.wYear = t.tm_year + 1900;
	recDate.wMonth = t.tm_mon + 1;
	recDate.wDay = t.tm_mday;
	recTime.wHour = t.tm_hour;
	recTime.wMinute = t.tm_min;
	recTime.wSecond = t.tm_sec;
	
	return true;
}

void CAPIMsgDistributorInterface::PackCommonPlaybackMsg( MESSAGE_DATA &msgData, unsigned long channel, unsigned long streamID, \
													   unsigned long cmdType, unsigned long searchType, unsigned long dataType, unsigned long startTime, unsigned long endTime )
{
	ULONGLONG ullChannel  = 0;

	PUB_SetBit64(&ullChannel, channel);

	msgData.clientID = m_ClientID;
	msgData.deviceID = 0;
	msgData.dataLen = sizeof(PACKCMD)+sizeof(NET_DATA_SEARCH);
	msgData.pData = new char [msgData.dataLen];
	memset(msgData.pData, 0, msgData.dataLen);

	PACKCMD *pPackCmd = (PACKCMD *)msgData.pData;
	pPackCmd->cmdType = cmdType;
	pPackCmd->cmdID   = 0;
	pPackCmd->cmdVer = NET_PROTOCOL_VER;
	pPackCmd->dataLen = sizeof(NET_DATA_SEARCH);

	NET_DATA_SEARCH *pdataInfo = reinterpret_cast<NET_DATA_SEARCH *>(msgData.pData + sizeof(PACKCMD));

	memset(pdataInfo,0,sizeof(NET_DATA_SEARCH));

	pdataInfo->searchType     = searchType;
	pdataInfo->videoCH        =  ullChannel;
	pdataInfo->audioCH        =  ullChannel;
	pdataInfo->startTime      = startTime;
	pdataInfo->endTime        = endTime;
	pdataInfo->dataType       = dataType;
	pdataInfo->RecordTypeMASK = 0xff;
	pdataInfo->dataOwnerby    = 0;
	pdataInfo->streamID = streamID;
}

bool CAPIMsgDistributorInterface::RequestPlayBack( unsigned long channel, unsigned long streamID, unsigned long startTime, unsigned long endTime )
{
	MESSAGE_DATA msgData;

	PackCommonPlaybackMsg(msgData, channel, streamID, CMD_REQUEST_DATA_READ, DATA_SEARCH_TYPE_TIME, NET_DATA_SEARCH_PLAYBACK);
	NET_DATA_SEARCH *pdataInfo = reinterpret_cast<NET_DATA_SEARCH *>(msgData.pData + sizeof(PACKCMD));
	pdataInfo->startTime = startTime;
	pdataInfo->endTime = endTime;

	CMessageMan::Instance()->PutMsgToServer(msgData, API_SERVER);

	CAPIStreamDispatch::Instance()->StartPlayBack(streamID, 1);

	printf("RequestPlayBack %lu\n", streamID);

	return true;

}


bool CAPIMsgDistributorInterface::StopPlayBack( unsigned long streamID )
{
	MESSAGE_DATA msgData;

	PackCommonPlaybackMsg(msgData, 0, streamID, CMD_REQUEST_DATA_STOP, DATA_SEARCH_TYPE_TIME, NET_DATA_SEARCH_PLAYBACK);
	CMessageMan::Instance()->PutMsgToServer(msgData, API_SERVER);


	return true;
}

int CAPIMsgDistributorInterface::ParseUser( const char *pData, char *pUser, char *pPassWd )
{

	return 0;
}
