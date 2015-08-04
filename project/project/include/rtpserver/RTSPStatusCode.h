//////////////////////////////////////////////////////////////////
#ifndef _RTSP_STATUS_CODE_H_
#define _RTSP_STATUS_CODE_H_

//#include "TVT_PubCom.h"
#include "PUB_common.h"

class CRTSPStatusCode
{
public:
	static CRTSPStatusCode *Instance();
	bool Initial();
	void Quit();

	~CRTSPStatusCode();
	const char *GetStatusCodeReason(int statusCode);
protected:
private:
	typedef struct _status_code_info_
	{
		int		statusCode;
		string	reason;
	}STATUS_CODE_INFO;

	CRTSPStatusCode();

private:
	bool		m_hasInitial;
	list<STATUS_CODE_INFO> m_statusInfoList;
};
#endif

//////////////////////////////////////////////////////////////////
