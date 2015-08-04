#include "RTPStreamDispatch.h"
#include "MessageMan.h"
#include <bitset>


CRTPStreamDispatch::CRTPStreamDispatch(void)
{
	m_bRunning = false;
	m_hThread = PUB_THREAD_ID_NOINIT;
}

CRTPStreamDispatch::~CRTPStreamDispatch(void)
{

}

bool CRTPStreamDispatch::Initial()
{
	return true;
}

void CRTPStreamDispatch::Quit()
{

}

bool CRTPStreamDispatch::Start()
{
	if(m_bRunning)
	{
		return true;
	}

	m_hThread = PUB_CreateThread(DispatchStreamThread, this, &m_bRunning);

	if(PUB_CREATE_THREAD_FAIL == m_hThread)
	{
		m_bRunning = false;
		return false;
	}

 	CServerRTPMan::Instance()->Initial(8);
 	CServerRTPMan::Instance()->Start();
 
 	printf("CServerRTPMan::Instance()->Start()\n");
	
	return true;
}

void CRTPStreamDispatch::Stop()
{
	CServerRTPMan::Instance()->Stop();
	if(m_bRunning)
	{
		PUB_ExitThread(&m_hThread, &m_bRunning);
	}
}

PUB_THREAD_RESULT PUB_THREAD_CALL CRTPStreamDispatch::DispatchStreamThread(void *pParam)
{
	CRTPStreamDispatch *pThis = (CRTPStreamDispatch *)pParam;
	pThis->DispatchStreamProc();

	return 0;
}


void CRTPStreamDispatch::DispatchStreamProc()
{
	CMessageMan *pMessageMan = CMessageMan::Instance();
	CServerRTPMan *pServerRTPMan = CServerRTPMan::Instance();
	long streams[4] = {LIVE_FRAME_FIRST_STREAM, LIVE_FRAME_SECOND_STREAM, LIVE_FRAME_TALK, LIVE_FRAME_JPEG};

	CFrameData *pFrameData;
	CFrameData *pPBFrameData;
	
	bitset<TVT_MAX_CAMERA_NUM> multiCH;
	long maxGetFrameCount = 0;


//   	RTPMediaInfo pMediaInfo;
//   	pMediaInfo.SSRC = 1234;
//   	pMediaInfo.SessionID = 123456;
//   	pMediaInfo.Channel = 1;
//   	pMediaInfo.LocalRTPPort = 7000;
//   	pMediaInfo.LocalRTCPPort = 7001;
//   	pMediaInfo.MediaType = 98;
//   	pMediaInfo.RemoteIp = htonl((unsigned long)192<<24 | (unsigned long)168<<16 | (unsigned long)3<<8 | (unsigned long)201);
//   	pMediaInfo.RemoteRTPPort = 6000;
//   	pMediaInfo.RemoteRTCPPort = 6001;
//   
//   	CServerRTPMan::Instance()->StartTransRTP(&pMediaInfo);

	unsigned short ch = 0;

	while(m_bRunning)
	{
		while(pMessageMan->GetPBFrame(&pPBFrameData, RTP_NET_SERVER))
		{
			//printf("GetFrameData IsPlayBack\n");
			pServerRTPMan->GetFrameData(pPBFrameData, true);

			pPBFrameData->Dec();
		}

		for(int i = 0; i < 4; i++)
		{
			maxGetFrameCount = 0;
			multiCH.reset();
			while(pMessageMan->GetFrame(&pFrameData, streams[i], RTP_NET_SERVER))
			{
				//分发到rtpserver中去
				if (0 == i)
				{
					pServerRTPMan->GetFrameData(pFrameData);
				}
			

				if(multiCH.test(pFrameData->GetFrameInfo().channel))
				{
					maxGetFrameCount++;
					multiCH.reset();
				}

				multiCH.set(pFrameData->GetFrameInfo().channel);

				pFrameData->Dec();	

				if(maxGetFrameCount > 25)
				{
					break;
				}
			}
		}

		PUB_Sleep(1);
	}
}

