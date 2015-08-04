#ifndef __RTP_STREAM_DISPATCH__
#define __RTP_STREAM_DISPATCH__

#include "PUB_common.h"
#include "ServerRTPMan.h"
#include "RTSP_Common.h"
#include "NetServerSDK.h"
#include "APIMsgDistributor.h"
#include "TvtDevInterface.h"
#include "TvtRtspStreamCtrl.h"
#include "Product.h"
#include <map>



class CAPIStreamDispatch
{
public:
	static CAPIStreamDispatch* Instance();
	~CAPIStreamDispatch();

	bool Initial();
	void Quit();

	bool Restart(unsigned short httpPort, unsigned short rtspPort);
	bool Start();
	void Stop();

	bool StartPlayBack(unsigned long streamID, float scale);
	int	SearchPlayback();
private:
	CAPIStreamDispatch();
	static PUB_THREAD_RESULT PUB_THREAD_CALL DispatchStreamThread(void *pParam);
	void DispatchStreamProc();

	bool PackFrame(NETNS::RTSP_FRAME &frame, FRAME_INFO_EX &frameInfo, bool bUseRelativeTime = true);
	void RecordPbFrame(unsigned long streamID, unsigned long frameIndex, LONGLONG frameTime);
	void PBFrameFeedback();
	void SendPbFeedback(unsigned long streamID, unsigned long frameIndex);
	void PbEnd(unsigned long streamID);
private:
//线程相关
	PUB_thread_t			m_hDispatchStreamThread;
	bool					m_bDispatchStreamRun;

	bool					m_bInit;

	CAPIMsgDistributor		*m_pMsgDistributor;
	CTvtDevInterface		*m_pTvtDev;
	CTvtRtspStreamCtrl		*m_pStreamCtrl;

	PRODUCT_INFO			m_Productinfo;
	unsigned long			m_ClientID;

	struct PBFrameFb
	{
		unsigned long	lastFbTick;
		unsigned long	frameCount;
		unsigned long	lastFrameIndex;
		LONGLONG		lastFrameTime;
		LONGLONG		currFrameTime;
		unsigned long	frameTimeDiff;
		float			scale;
	};
	std::map<int , PBFrameFb>	m_PbCtrl;
	CPUB_Lock					m_PbLock;
};
#endif
