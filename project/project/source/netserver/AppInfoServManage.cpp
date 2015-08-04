#include "AppInfoServManage.h"

#include "PUB_common.h"
#include "LocalDevice.h"
#include "StringEncrypt.h"

#ifdef __ENVIRONMENT_LINUX__
#include <unistd.h>
#include <stdlib.h>
#include <netdb.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "NetInterface.h"

#endif

const char *g_pAppInfoServerIp 	= "app.tvt.net.cn";

const char *g_pRetCode			= "RetCode";
const char *g_posPlateFormName 	= "OSPlatform";
const char *g_pAppPlateFormName = "AppPlatform";
const char *g_pAppName			= "AppName";
const char *g_pQrDownUrl		= "QrDownUrl";

const char *g_pTokenServerAddress = "Address";
const char *g_pTokenServerPort	  = "Port";

const unsigned long CONNECT_TIME_OUT = 33000;

CAppInfoServeManage::CAppInfoServeManage()
{

}
CAppInfoServeManage::~CAppInfoServeManage()
{
	
}



int CAppInfoServeManage::ConnectServer(int connectFd, struct sockaddr *pServerSockAddr ,int sockAddrLen, unsigned long timeOut)
{
	//int iSave = fcntl(connectFd,F_GETFL);
	//fcntl(connectFd, F_SETFL, iSave | O_NONBLOCK);


	if( 0 != connect( connectFd, pServerSockAddr, sockAddrLen) )
	{
		timeval timeOutVal;
		timeOutVal.tv_sec = timeOut / 1000;
		timeOutVal.tv_usec = (timeOut % 1000) * 1000;

		fd_set readdSet;
		fd_set writeSet;
		FD_ZERO(&readdSet);
		FD_ZERO(&writeSet);
		FD_SET(connectFd, &readdSet);
		FD_SET(connectFd, &writeSet);
		if(0 < select(connectFd+1, &readdSet, &writeSet, NULL, &timeOutVal))
		{
			//要从新设置为阻塞模式
			//NoBlockIO=0;
	//		fcntl(connectFd, F_SETFL, iSave );

			return 0;
		}
		perror("connect() to the appinfo server");

		return -1;
	}
	return 0;
}

EAppServerStatus CAppInfoServeManage::GetMessageFromServer(int sockfd, char *pbuf, int &len)
{
	assert( 0 < sockfd );
	int i = 0,  retval;
	int recvLen = 0;
	struct timeval timeout;
	fd_set ReadSet;

	for( i=0; i<10; i++ )
	{
		// FD_SET()应该在每次循环时都被调用， select()超时时readSet会被清空
		FD_ZERO(&ReadSet);
		FD_SET( sockfd, &ReadSet );

		//把超时时间改为1秒， 防止网络太慢时服务端返回的应答抓不到。
		//在公司服务器(www.dvrdydns.com)上遇到过这种情况：超时时间设置为10000微妙，通过外网一致得不到服务器响应，内网却可以
		//当延长超时时间后就正常了。
		timeout.tv_sec  = 3;
		timeout.tv_usec = 0;

		retval = select(sockfd+1, &ReadSet, NULL, NULL, &timeout );
		if( 0 == retval)
		{
			if( 9 == i )
			{
				return REQUEST_APP_QR_TIME_OUT;
			}
			continue;
		}
		else if( -1 == retval )
		{
			if( EINTR == errno )
				continue;
			else
				return REQUEST_WAIT_ERROR;
		}

		if( FD_ISSET( sockfd, &ReadSet ) )
		{
			if ( recvLen > len)
			{
				printf("%s:%s:%d, recvLen=%d, len=%d", __FUNCTION__, __FILE__ ,__LINE__, recvLen, len);
				assert(false);

				return REQUEST_APP_QR_STR_ERROR;
			}

			retval = recv( sockfd, pbuf + recvLen, len - recvLen, 0 );
			if( -1 == retval )
			{
				return REQUEST_WAIT_ERROR;
			}
			if( strstr(pbuf, "\r\n\r\n"))
			{
				recvLen += retval;
				pbuf[recvLen] = '\0';
				len = recvLen;
				return REQUEST_APP_QR_OK;
			}

			recvLen += retval;

		}
	}

	return REQUEST_WAIT_ERROR;
}

bool CAppInfoServeManage::bCreateSendStr(ERequestType eRequestType,string &OutStr)
{
	//provider code 
	string providerCodeStr;
	unsigned long productSubId = CProduct::Instance()->ProductSubID()/*+1*/;
	GetCustomEncrypt(productSubId, providerCodeStr);
	//char providerCodeStr[20]={"PAT"};
	//MAC
	char lenMacstr[64]={0};
	char macstr[64]={0};
	CLocalDevice::GetMacFromFlash(lenMacstr, 50);
	#ifdef __ENVIRONMENT_WIN32__
	strcpy(lenMacstr,"00:18:ae:34:df:70");
	#endif
	unsigned int j=0;
	for(unsigned int i=0;i<strlen(lenMacstr);i++)
	{
		if(lenMacstr[i]!=':')
		{
			macstr[j++]=lenMacstr[i];
		}
	}
	//devicetype
	char DeviceTypeStr[10]={0};
	#ifdef __ENVIRONMENT_WIN32__
	strcpy(DeviceTypeStr,"DVR");
	#else
	switch((CProduct::Instance()->GetDeviceType()))
	{
		case 0:
			strcpy(DeviceTypeStr,"DVR");
			break;
		case 1:
			strcpy(DeviceTypeStr,"NVR");
			break;
		case 2:
			strcpy(DeviceTypeStr,"HYBIRD");
			break;
		default:
			break;
	}
	#endif
	//product type
	char productTypeStr[30]={0};
	PRODUCT_TYPE productTypeTemp = CProduct::Instance()->ProductType();
	sprintf(productTypeStr,"%d",productTypeTemp);
	//SoftVer
	char SoftVerStr[25]={0};
	#ifdef __ENVIRONMENT_WIN32__
	strcpy(SoftVerStr,"3.0");
	#endif
	if(CLocalDevice::GetKernelVersion(SoftVerStr,20)==false)
	{
		memset(SoftVerStr,0,sizeof(SoftVerStr));
	}
	
	char strCmdBuf[1024]={0};
	char *pIp = "192.168.8.181";
	switch(eRequestType)
	{
		case REQUEST_APP_LINK_INFO:
		sprintf(strCmdBuf,"GET /GetQrDownInfo.aspx?ProviderCode=%s&DevMac=%s&DeviceType=%s&ProductType=%s&SoftVer=%s HTTP/1.1\r\n"
				"Host: %s\r\n"
				"Connection: keep-alive\r\n"
				"Cache-Control: max-age=0\r\n"
				"Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8\r\n"
				"User-Agent: Mozilla/5.0 (Windows NT 6.1) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/36.0.1985.143 Safari/537.36\r\n"
				"Accept-Encoding: gzip,deflate,sdch\r\n"
				"Accept-Language: zh-CN,zh;q=0.8\r\n\r\n",providerCodeStr.c_str(),macstr,DeviceTypeStr,productTypeStr,SoftVerStr,g_pAppInfoServerIp);	
		break;
		case REQUEST_TOKEN_SERVER_INFO:
		
		sprintf(strCmdBuf,"GET /GetPushServerInfo.aspx?ProviderCode=%s&DeviceType=%s&ProductType=%s&SoftVer=%s HTTP/1.1\r\n"
				"Host: %s\r\n"
				"Connection: keep-alive\r\n"
				"Cache-Control: max-age=0\r\n"
				"Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8\r\n"
				"User-Agent: Mozilla/5.0 (Windows NT 6.1) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/36.0.1985.143 Safari/537.36\r\n"
				"Accept-Encoding: gzip,deflate,sdch\r\n"
				"Accept-Language: zh-CN,zh;q=0.8\r\n\r\n",providerCodeStr.c_str(),DeviceTypeStr,productTypeStr,SoftVerStr,g_pAppInfoServerIp);	
		break;
		default:
		return false;
		break;
	}
	OutStr.clear();
	OutStr +=strCmdBuf;
	return true;
}
bool CAppInfoServeManage::GetAppDownlinkInfo(string Instr,VECTOR_APP_INFO &VectorAppInfo)
{

	bool bRet = DecodeServerStr(Instr,REQUEST_APP_LINK_INFO,&VectorAppInfo);

	return bRet;
}
bool CAppInfoServeManage::GetTokenServerInfo(string Instr,TOKEN_SERVER_INFO &TokenServerInfo)
{
	
	bool bRet = DecodeServerStr(Instr,REQUEST_TOKEN_SERVER_INFO,&TokenServerInfo);

	char *pTokenServerIp = const_cast<char *>(TokenServerInfo.tokenServerIp);
	
	StringEncrypt::Cutcode(pTokenServerIp,StringEncrypt::DefaultKey);
	
	char *pTokenServerPort = const_cast<char *>(TokenServerInfo.tokenServerPort);
	
	StringEncrypt::Cutcode(pTokenServerPort,StringEncrypt::DefaultKey);
	return bRet;
}

bool CAppInfoServeManage::RequestServerInfo(string &OutStr,ERequestType eRequestType)
{
	
	struct sockaddr_in ser_addr;
	ser_addr.sin_family = AF_INET;
	ser_addr.sin_port =  htons(80);
//	ser_addr.sin_addr.s_addr = inet_addr( "192.168.8.181" );//server IP
	struct hostent *host;
	#if 1
	if((host = gethostbyname(g_pAppInfoServerIp))!= NULL)
	{
		memcpy(&(ser_addr.sin_addr),host->h_addr,4);
	}
	else
		return false;
	#endif
	memset(ser_addr.sin_zero,0, sizeof(ser_addr.sin_zero));

	int sockfd = socket(AF_INET, SOCK_STREAM, 0);

	if( -1 == sockfd )
	{
		printf("########create socket error.....%s__%d\n",__FILE__,__LINE__);
		return false;
	}

	
	//link to host
	if(ConnectServer(sockfd, (struct sockaddr*)&ser_addr, sizeof(struct sockaddr), CONNECT_TIME_OUT) != 0)
	{
		perror("connect()");

#ifndef WIN32
		close(sockfd);
#else
		closesocket(sockfd);
#endif
		return false;
	}
	string sendStr;
	sendStr.clear();
	bool bRet = bCreateSendStr(eRequestType,sendStr);
	if(!bRet)
	{
		printf("#### create send str error......%d....%s....\n",__LINE__,__FILE__);
		return false;
	}
	if(send(sockfd, sendStr.c_str(), sendStr.length(), 0) == -1)
	{
		perror("send() data to server");
#ifndef WIN32
		close(sockfd);
#else
		closesocket(sockfd);
#endif
		return false;
	}

	// receive
	char receiveBuf[1000] = {0};
	int len = sizeof(receiveBuf)-1;

	int ret = GetMessageFromServer(sockfd, receiveBuf, len);
	receiveBuf[999]=0;
	printf("receiveBuf = %s\n",receiveBuf);
	#ifndef WIN32
	close(sockfd);
#else
	closesocket(sockfd);
#endif

	if(ret != REQUEST_APP_QR_OK)
	{
		return false;
	}
	OutStr.clear();
	OutStr+= receiveBuf;
	return true;
}

bool CAppInfoServeManage::DecodeServerStr(string strApp,ERequestType eRequestType,void *pOutParm)
{
	string blockstr;
	char *pCurPos =(char *)strApp.c_str(); //
	pCurPos = strstr(pCurPos,g_pRetCode);
	if(pCurPos)
		{
			pCurPos +=strlen(g_pRetCode)+2;
			unsigned int RetCode = *pCurPos -'0';
			switch(RetCode)
				{
					case 0:
					{
					while(*pCurPos != ']')
						{
							blockstr.clear();
							pCurPos = strstr(pCurPos,"{");
							if(pCurPos == NULL)
								return true;
							while(*pCurPos != '}')
								{
									blockstr += *pCurPos;
									pCurPos++;
									if(*pCurPos == '\0')
										return true;
								}
							blockstr +='}';
							//一组链接
							
							switch(eRequestType)
							{
								case REQUEST_APP_LINK_INFO:
								{
									RESP_APP_INFO RespAppinfo;
									if(FormatAppChildStr(blockstr,RespAppinfo)==true)
										reinterpret_cast<VECTOR_APP_INFO *>(pOutParm)->push_back(RespAppinfo);
									break;
								}
								case REQUEST_TOKEN_SERVER_INFO:
								{
									FormatTokenServerStr(blockstr,reinterpret_cast<TOKEN_SERVER_INFO *>(pOutParm));
									break;
								}
								default:
								break;
							}
							
							if(*(pCurPos+1)==']')
								return true;
							if(*pCurPos == '\0')
								return true;
							
						}
					break;
					}
					case 1:
						//没有填客户编码
						break;
					case 2:
						//服务器没有客户信息
						break;
					default:
						break;
				}
		}
	return false;
}
bool CAppInfoServeManage::FormatAppChildStr(string &strApp,RESP_APP_INFO &RespAppInfo)
{

	FormatStr(strApp,g_posPlateFormName,RespAppInfo.osPlateform);
	FormatStr(strApp,g_pAppPlateFormName,RespAppInfo.appPlateForm);
	FormatStr(strApp,g_pAppName,RespAppInfo.AppName);
	FormatStr(strApp,g_pQrDownUrl,RespAppInfo.QrDownUrl);

	return true;
		
}
bool CAppInfoServeManage::FormatTokenServerStr(string strApp,TOKEN_SERVER_INFO *ptokenServerInfo)
{
	string str;
	str.clear();
	FormatStr(strApp,g_pTokenServerAddress,str);
	if(str.length() < sizeof(ptokenServerInfo->tokenServerIp))
	{
		memcpy(ptokenServerInfo->tokenServerIp,str.c_str(),str.length());
	}
	str.clear();
	FormatStr(strApp,g_pTokenServerPort,str);
	if(str.length() < sizeof(ptokenServerInfo->tokenServerPort))
	{
		memcpy(ptokenServerInfo->tokenServerPort,str.c_str(),str.length());
	}

	return true;
}

bool CAppInfoServeManage::FormatStr(string &strApp,const char *pSearchStr,string &strValue)
{
	unsigned int StepCnt = 0;
	char *pCurPos=NULL;
	pCurPos = strstr((char *)strApp.c_str(),pSearchStr);
	if(pCurPos == NULL)
		return false;
	pCurPos += strlen(pSearchStr)+1;
	while(*pCurPos != '"')
	{
		pCurPos++;
		if(*pCurPos == '\0')
			break;
	}
	pCurPos++;
	while(*pCurPos != '"')
	{
		strValue += *pCurPos;
		pCurPos ++;
		if(*pCurPos == '\0')
			break;
	}
	return true;
}

