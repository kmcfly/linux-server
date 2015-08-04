#ifndef __MESSAGE_ADAPTER__
#define __MESSAGE_ADAPTER__

#include "PUB_common.h"
#include "MessageMan.h"
#include "DevDefine.h"

#include <vector>

class CAPIMsgDistributorInterface
{
public:
	virtual int GetMessage(MESSAGE_DATA &msgData) = 0;
public:
	void SetClientID(unsigned long clientID) {m_ClientID = clientID;}
protected:
	bool TimeConvert(const NETNS::DEV_DATE& recDate, const NETNS::DEV_TIME& recTime, unsigned long &time);
	bool ConvertTime(unsigned long time, NETNS::DEV_DATE& recDate, NETNS::DEV_TIME& recTime);
	void PackCommonPlaybackMsg(MESSAGE_DATA &msgData, unsigned long channel, unsigned long streamID, unsigned long cmdType, \
		unsigned long searchType, unsigned long dataType, unsigned long startTime = 0, unsigned long endTime = 0 );
	bool RequestPlayBack(unsigned long channel, unsigned long streamID, unsigned long startTime, unsigned long endTime);
	bool StopPlayBack(unsigned long streamID);
	int ParseUser(const char *pData, char *pUser, char *pPassWd);

protected:
	unsigned long			m_ClientID;
};

class CAPIMsgDistributor
{
public:
	CAPIMsgDistributor();
	~CAPIMsgDistributor();

	int AddReceiver(CAPIMsgDistributorInterface * pObj);

private:
	static PUB_THREAD_RESULT PUB_THREAD_CALL MessageThread(void *pParam);
	void MessageProc();

	void Distribute(MESSAGE_DATA &msgData);
private:
	PUB_thread_t			m_hMessageThread;
	bool					m_bMessageProcRun;

	std::vector<CAPIMsgDistributorInterface*> m_vec;
};

#endif