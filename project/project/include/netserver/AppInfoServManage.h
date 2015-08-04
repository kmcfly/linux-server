#ifndef _APPINFO_SERVER_MANAGE_H_
#define _APPINFO_SERVER_MANAGE_H_

#include <iostream> 
#include <string.h>
#include <vector>
using namespace std;


typedef enum{
	REQUEST_APP_QR_OK =0,
	REQUEST_APP_QR_TIME_OUT,
	REQUEST_APP_QR_STR_ERROR,
	REQUEST_WAIT_ERROR,

	REQUEST_RETURN_CNT
}EAppServerStatus;

typedef enum{
	REQUEST_APP_LINK_INFO=0,
	REQUEST_TOKEN_SERVER_INFO,

	CMD_REQUEST_CNT
}ERequestType;

typedef struct 
{
	char osPlateform[20];
	char appPlateForm[20];
	char AppName[40];
	char QrDownUrl[120];
}APP_DOWNLOAD_INFO;

typedef struct
{
	char tokenServerIp[80];
	char tokenServerPort[12];
}TOKEN_SERVER_INFO;

typedef struct 
{
	string osPlateform;
	string appPlateForm;
	string AppName;
	string QrDownUrl;
}RESP_APP_INFO;

typedef vector<RESP_APP_INFO> VECTOR_APP_INFO;

class CAppInfoServeManage
{
	public:
		CAppInfoServeManage();
		~CAppInfoServeManage();
		bool  RequestServerInfo(string &OutStr,ERequestType eRequestType);

		bool GetAppDownlinkInfo(string Instr,VECTOR_APP_INFO &VectorAppInfo);
		bool GetTokenServerInfo(string Instr,TOKEN_SERVER_INFO &TokenServerInfo);
	private:
		int ConnectServer(int connectFd, struct sockaddr *pServerSockAddr ,int sockAddrLen, unsigned long timeOut);

		EAppServerStatus	GetMessageFromServer(int sockfd, char *pbuf, int &len);

		bool bCreateSendStr(ERequestType eRequestType,string &OutStr);

		bool DecodeServerStr(string Instr,ERequestType eRequestType,void *pOutParm);
		bool	FormatAppChildStr(string &strApp,RESP_APP_INFO &RespAppInfo);
		bool 	FormatTokenServerStr(string strApp,TOKEN_SERVER_INFO *ptokenServerInfo);
		bool 	FormatStr(string &strApp,const char *pSearchStr,string &strValue);
};
#endif
