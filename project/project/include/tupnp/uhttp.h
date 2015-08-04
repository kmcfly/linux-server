/////////////////////////////////////////////////////////////////////////
#ifndef _U_HTTP_H_
#define _U_HTTP_H_

#include "upnpSock.h"

#define  UHTTP_BUF_LEN		4096
#define	 NEWEXTIPSTART		"<NewExternalIPAddress>"
#define  NEWEXTIPEND		"</NewExternalIPAddress>"
#define  IPLEN				26

typedef enum _header_type_
{
	HEADER_GENERAL	= 0,
	HEADER_REQUEST	= 1,
	HEADER_RESPONSE	= 2,
	HEADER_ENTITY	= 3,
}HEADER_TYPE;

typedef struct _msg_header_
{
	char *pName;
	char *pValue;
	HEADER_TYPE type;
}MSG_HEADER;

typedef std::list<MSG_HEADER *> MSG_HEADER_P_LIST;
typedef std::list<MSG_HEADER *>::iterator MSG_HEADER_P_LIST_ITER;

class CUHttp
{
public:
	CUHttp(CUpnpSock *pSock);
	~CUHttp();
	
	bool AddStartLine(const char *pMethod, const char *pRequestURI, const char *pHTTPVersion);
	bool AddMsgHeaderLine(const char *pHeaderName, const char *pHeaderValue);
	bool AddMsgHeaderLine(const char *pHeaderName, int value);
	void AddEndLine();
	bool AddBody(const char *pBody);

	int SendRecvHttp(unsigned long sTimeOut);
	int SendHttp(unsigned long sTimeOut);
	int RecvHttp(unsigned long rTimeOut);
	int GetFile(const char *pReqFilePath, const char *pHost);
	bool ResponseOK();

	int GetErrNum();
	void GetErrDes(char *pErrDes, unsigned long length);
	const char *GetHeaderValue(const char *pHeaderName, char **ppValue);

	bool HeadMsgRecved();
	int RecvBody(unsigned long rTimeOut);
	const char *GetXmlBody();
	int ContentLength();
	void GetContentType(char *pContentType);

	void GetExtIpAddress(const char *pXmlText,char *pIP);
private:
	int GetOneLine(char *pLine, unsigned long rTimeOut);
	void ClearMSGList();

private:
	CUpnpSock		*m_pSock;
	char			m_sData[UHTTP_BUF_LEN];

	//http应答信息
	char			m_rData[UHTTP_BUF_LEN * 10];
	unsigned long	m_rrPos;
	unsigned long	m_rwPos;

	char			m_version[20];		//应答开始行的版本
	char			m_errNum[20];		//应答错误码
	char			m_errDes[160];		//应答错误描述信息

	char			*m_pBodyPos;

	MSG_HEADER_P_LIST m_msgList;		//应答中的头信息
};
#endif /*_HTTP_H_*/
/////////////////////////////////////////////////////////////////////////
