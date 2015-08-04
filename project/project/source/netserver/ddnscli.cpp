//DDNSClientMod.cpp
#include <sys/types.h>
#include <sys/stat.h>

#include <sys/time.h>

#include <stdarg.h>  //only included in implementation file
#include <stdio.h>   //only included in implementation file

#include <unistd.h>  // for gethostname
#include <netdb.h>   // for gethostbyname

// For GetMACAddress
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#include <sys/ioctl.h>  // For SIOCGIFCONF on Linux

#include <sys/socket.h>
//#include <sys/sockio.h>  // For SIOCGIFCONF on SunOS
#include <net/if.h>
#include <netinet/in.h>   //For IPPROTO_IP
#ifdef CMAKE_HAVE_NET_IF_DL_H
#include <net/if_dl.h>
#endif
#if defined(CMAKE_HAVE_NET_IF_ARP_H) && defined(__sun)
// This is absolutely necessary on SunOS
#include <net/if_arp.h>
#endif
#include <stdio.h>
#include <unistd.h>
#include <string.h>
//#include <stropts.h>


#include <netinet/in.h>
#include <netinet/if_ether.h>
#include <arpa/inet.h>

#include <time.h>

#include "ddnscli.h"
#include "md5.h"

#define REGISTER_HOST_PATH		"/ddns/RegisterHost.php"
#define UPDATE_HOST_PATH		"/ddns/UpdateHost.php"
#define REMOVE_HOST_PATH		"/ddns/RemoveHost.php"
#define REGISTER_USER_PATH		"/ddns/tCU.php"

#define DDNS_ANONYID	"anonymous"
#define DDNS_ANONYPW	"WeLcOmEdDnS"

#define DDNS_WEBPORT	80

#define VM_ETHERNET_MAC	"005056"

#ifndef BOOL
#define BOOL int
#endif 

#ifndef UINT
#define UINT unsigned int
#endif 

#ifndef PHOSTENT
#define PHOSTENT struct hostent*
#endif

#ifndef FALSE
#define FALSE 0
#endif 

#ifndef TRUE 
#define TRUE 1
#endif 

#ifndef PULONG 
#define PULONG long int*
#endif

#ifndef SOCKADDR_IN
#define SOCKADDR_IN struct sockaddr_in 
#endif
#ifndef SOCKADDR
#define SOCKADDR struct sockaddr
#endif

#ifndef SOCKET_ERROR 
#define SOCKET_ERROR -1
#endif

#define HOST_ADDR	"www.autoipset.com"

typedef struct USER_INFO
{
	char szID[32];
	char szPw[32];
}_stUserInfo,*_pstUserInfo;

typedef struct PRODUCT_INFO
{
	int nProductType;
	char szModel[32];
	char szSoftVer[32];
	time_t tmLastUpdate;
}_stProductInfo,*_pstProductInfo;

typedef struct NET_INFO
{
	char szServer[64];
	char szMacAddress[32];
	char szHostname[64];
	char szLocalIP[32];
	BOOL bNetStatus;
	UINT nWebPort;
	UINT nListenPort;
}_stNetInfo,*_pstNetInfo;


_stUserInfo		g_tUser;
_stProductInfo		g_tProduct;
_stNetInfo		g_tNet;
char			g_szDebug[256]="";
long 			g_dwDebugSize=0;
double 			g_start,g_end;



////////////////////////////////////////////
extern int GetMacAddr(unsigned char *byMac,char *szLocalIP);
////////////////////////////////////////////
BOOL			UpdateIP (char *szMac);
BOOL			RegisterHostname(char *szMac);
BOOL			RemoveHostname(char *szMac);
void			Error(char *lpszFunction);
void			Cleanup();
const char *		MD5_crypt(char *strData,int nLen);
void			WriteDebug(char *strDebug);
int				ParseResult(char *szResponse);
BOOL			IsValidPassword(char *szPasswd);
BOOL			IsValidHostname(char *szHostname);
////////////////////////////////////////////
////////////////////////////////////////////


int DDNS_InitDDNSInfo(int nProductType, char *szModel, char *szSoftVer)
{
	g_tProduct.tmLastUpdate = 0;
	int nEorrCode = DE_SUCCESS;
	unsigned char byMac[6];
	int i;
	char szTemp[32]="";

	g_tProduct.nProductType = nProductType;
	
	if(szModel !=NULL && strlen(szModel)>0)
		sprintf(g_tProduct.szModel,"%s",szModel);

	if(szSoftVer !=NULL && strlen(szSoftVer)>0)
	sprintf(g_tProduct.szSoftVer,"%s",szSoftVer);

	if((nEorrCode = GetMacAddr(byMac,g_tNet.szLocalIP)) == DE_SUCCESS)
	{
		sprintf(g_tNet.szMacAddress,"%02X%02X%02X%02X%02X%02X",
			byMac[0],byMac[1],byMac[2],byMac[3],byMac[4],byMac[5]);
		 
	}else
		return DE_DISCONNECTED;
	
	return nEorrCode;
}


int DDNS_SetDDNSServer(char *szServer)
{
	int bReturn = DE_NOT_SERVER;
	if(szServer != NULL && strlen(szServer))
	{
		sprintf(g_tNet.szServer,"%s",szServer);
		bReturn = DE_SUCCESS;
	}
	return bReturn;
}

int DDNS_RegisterDDNSInfo(char *szHostName, char *szUserID, char *szPassword,
								  int nWebPort,  int nSessionPort)
{
	int bReturn =DE_SUCCESS;

	if(szHostName && strlen(szHostName) > 0)
		strncpy(g_tNet.szHostname,szHostName,64);
	else
		return DE_NULL_HOSTNAME;

	if(!IsValidHostname(szHostName))
		return DE_NOT_ALPHABET_NUM;

	if(szPassword && strlen(szPassword) > 0)
	{
		if(!IsValidPassword(szPassword))
		{
			printf("not usable password %s\n",szPassword);
			return DE_PW_DENY_CHAR;
		}
		sprintf(g_tUser.szPw,"%s",MD5_crypt(szPassword,strlen(szPassword)));
	}

	if(szUserID && strlen(szUserID) > 0 ){
		strncpy(g_tUser.szID,szUserID,32);
	}else{
		sprintf(g_tUser.szID,"%s",DDNS_ANONYID);
		sprintf(g_tUser.szPw,"%s",MD5_crypt(DDNS_ANONYPW,strlen(DDNS_ANONYPW)));
	}

	g_tNet.nWebPort = nWebPort;
	g_tNet.nListenPort = nSessionPort;

	return RegisterHostname(g_tNet.szMacAddress);
}

int DDNS_UpdateDDNSInfo()
{
	if(g_tNet.szMacAddress == NULL || strlen(g_tNet.szMacAddress) == 0)
		return FALSE;	
	return UpdateIP(g_tNet.szMacAddress);
}

int DDNS_RemoveDDNSInfo()
{
	if(g_tNet.szMacAddress == NULL || strlen(g_tNet.szMacAddress) == 0)
		return FALSE;	
	return RemoveHostname(g_tNet.szMacAddress);
}


void DDNS_ClearDDNSInfo()
{
	memset(&g_tUser,0,sizeof(g_tUser));
	memset(&g_tProduct,0,sizeof(g_tProduct));
}


BOOL UpdateIP (char *szMac)
{
	int	bRet = DE_SUCCESS;

	//
	// Connect to..
	//
	
	SOCKADDR_IN 	addr;
	PHOSTENT	host;
	int 		sockfd;
	char		szDNS[128]="";
	time_t		ltime;

	time(&ltime);

	if((ltime - g_tProduct.tmLastUpdate) < 300)
		return DE_UPDATE_AFTER_5MIN;

	g_tProduct.tmLastUpdate = ltime;

	sprintf(szDNS,"%s",g_tNet.szServer);

	if(szMac && strlen(szMac) > 0)
		sprintf(g_tNet.szMacAddress,"%s",szMac);	

	if(strlen(g_tNet.szMacAddress)<=0)
		return DE_NO_MACADDRESS;
	
	if ((sockfd = socket (AF_INET, SOCK_STREAM, 0)) < 0)
	{
		printf("socket failed");
		return DE_DISCONNECTED;
	}

	if((host = gethostbyname (szDNS)) == NULL)
	{
		printf("gethostbyname failed");
		bRet = DE_FAILED_HOSTBYNAME;
		goto cleanup;
	}

	memset (&addr, 0, sizeof(SOCKADDR_IN));

	addr.sin_family = AF_INET;
	addr.sin_port = htons(DDNS_WEBPORT);
	addr.sin_addr.s_addr = *(PULONG)(*host->h_addr_list);	

	if (connect (sockfd, (struct sockaddr *)&addr, sizeof(struct sockaddr)))
	{
		bRet = DE_DISCONNECTED;
		goto cleanup;
	}
	
	//
	// Compose HTTP. 
	//

	UINT len, hdr;
	char header[512];
	char body[512];


	len = sprintf (body,

			"----------------------------7d51bb2150306\r\n"
			"Content-Disposition: form-data; name=\"User_id\"\r\n"
			"\r\n"
			"%s\r\n"
			"----------------------------7d51bb2150306\r\n"
			"Content-Disposition: form-data; name=\"domain\"\r\n"
			"\r\n"
			"%s.\r\n"
			"----------------------------7d51bb2150306\r\n"
			"Content-Disposition: form-data; name=\"MacAddress\"\r\n"
			"\r\n"
			"%s\r\n"
			"----------------------------7d51bb2150306\r\n"
			"Content-Disposition: form-data; name=\"PrivateIP\"\r\n"
			"\r\n"
			"%s\r\n"
			,
			g_tUser.szID,g_tNet.szServer,g_tNet.szMacAddress,g_tNet.szLocalIP);
		
	body[len] ='\0';

	len += sprintf (body+len,
			"\r\n"
			"----------------------------7d51bb2150306\r\n");

	hdr =  sprintf (header,

			"POST %s HTTP/1.0\r\n"
			"Content-Type: multipart/form-data; boundary="
			"--------------------------7d51bb2150306\r\n"
			"Host: %s\r\n"
			"Content-Length: %u\r\n"
			"Pragma: no-cache\r\n"
			"\r\n"

			,UPDATE_HOST_PATH, g_tNet.szServer, len);
	

	if (send (sockfd, header, hdr, 0) == SOCKET_ERROR)
	{
		bRet = DE_FAILED_SEND;
		goto cleanup;
	}
	
	if (send (sockfd, body, len, 0)   == SOCKET_ERROR)
	{
		bRet = DE_FAILED_SEND;
		goto cleanup;
	}

	//
	// Recv reply.
	//

	char reply[512];
	char *s;
	size_t rc;


	if ((rc =recv (sockfd, reply, sizeof(reply), 0)))
	{
		reply[rc] = '\0';
		if ((s = strrchr (reply, '\n')) == NULL)
		{
			goto cleanup;
		}	

		++s;		
	}
	
	bRet = ParseResult(s);
	//
	// Cleanup.
	//

cleanup:

	close(sockfd);

	g_start = clock();
	
	return bRet;
}

BOOL RegisterHostname(char *szMac)
{
	int			bRet = DE_SUCCESS;

	//
	// Connect to..
	//
	
	SOCKADDR_IN	addr;
	int		sockfd;
	PHOSTENT	host;
	char	szDNS[128]="";

	sprintf(szDNS,"www.%s",g_tNet.szServer);

	if(strlen(szMac)>0)
		sprintf(g_tNet.szMacAddress,"%s",szMac);

	if(strlen(g_tNet.szMacAddress)<=0)
		return DE_NOT_FOUND_ADAPTER;
	
	if ((sockfd = socket (AF_INET, SOCK_STREAM, 0)) < 0)
	{
		Error("socket");
		return DE_DISCONNECTED;
	}

	if((host = gethostbyname (szDNS)) == NULL)
	{
		Error("gethostbyname");
		bRet = DE_FAILED_HOSTBYNAME;
		goto cleanup;
	}

	memset (&addr, 0, sizeof(SOCKADDR_IN));

	addr.sin_family = AF_INET;
	addr.sin_port = htons(80);
	addr.sin_addr.s_addr = *(PULONG)(*host->h_addr_list);	

	if (connect (sockfd, (struct sockaddr *)&addr, sizeof(struct sockaddr)))
	{
//		Error("connect");
		bRet = DE_DISCONNECTED;
		goto cleanup;
	}
	
	//
	// Compose HTTP. 
	//

	UINT len, hdr;
	char header[512];
	char body[2048];

	len = sprintf (body,
			"----------------------------7d51bb2150306\r\n"
			"Content-Disposition: form-data; name=\"INFO\"\r\n"
			"\r\n"
			"%s::%s::%s::%s::%s.::%s::%s::%d::%s::%d::%d::\r\n"
			,
			g_tNet.szMacAddress,
			g_tNet.szHostname,
			g_tUser.szID,
			g_tUser.szPw,
			g_tNet.szServer,
			g_tNet.szLocalIP,
			g_tProduct.szSoftVer,
			g_tProduct.nProductType,
			g_tProduct.szModel,
			g_tNet.nWebPort,
			g_tNet.nListenPort
			);

  
	body[len] = '\0';
	len += sprintf (body+len,
			"\r\n"
			"----------------------------7d51bb2150306\r\n");
//			Error(body);

	hdr =  sprintf (header,

			"POST %s HTTP/1.0\r\n"
			"Content-Type: multipart/form-data; boundary="
			"--------------------------7d51bb2150306\r\n"
			"Host: %s\r\n"
			"Content-Length: %u\r\n"
			"Pragma: no-cache\r\n"
			"\r\n"

			,REGISTER_HOST_PATH, g_tNet.szServer, len);
	

	if (send (sockfd, header, hdr, 0) == SOCKET_ERROR)
	{
		Error("send");
		bRet = DE_FAILED_SEND;
		goto cleanup;
	}
	
	if (send (sockfd, body, len, 0)   == SOCKET_ERROR)
	{
		Error("send");
		bRet = DE_FAILED_SEND;
		goto cleanup;
	}

	//
	// Recv reply.
	//

	char reply[512];
	char *s;
	size_t rc;


	if ((rc =recv (sockfd, reply, sizeof(reply), 0)))
	{
		reply[rc] = '\0';
		
		if ((s = strrchr (reply, '\n')) == NULL)
		{
			goto cleanup;
		}	
		
		++s;
	}

	bRet = ParseResult(s);

	//
	// Cleanup.
	//

cleanup:

	close(sockfd);
	
	return bRet;
}

int RemoveHostname(char *szMac)
{
	int		bRet = DE_SUCCESS;

	if(!g_tNet.szServer || strlen(g_tNet.szServer) <=0)
		return DE_NOT_SERVER;
	//
	// Connect to..
	//
	SOCKADDR_IN	addr;
	int		sockfd;
	PHOSTENT	host;

	char	szDNS[128]="";

	sprintf(szDNS,"www.%s",g_tNet.szServer);

	if(szMac && strlen(szMac) > 0)
		sprintf(g_tNet.szMacAddress,"%s",szMac);	

	if ((sockfd = socket (AF_INET, SOCK_STREAM, 0)) < 0)
	{
		printf("socket failed");
		return DE_DISCONNECTED;
	}

	if((host = gethostbyname (szDNS)) == NULL)
	{
		printf("gethostbyname failed");
		bRet = DE_FAILED_HOSTBYNAME;
		goto cleanup;
	}

	memset (&addr, 0, sizeof(SOCKADDR_IN));

	addr.sin_family = AF_INET;
	addr.sin_port = htons(DDNS_WEBPORT);
	addr.sin_addr.s_addr = *(PULONG)(*host->h_addr_list);	

	if (connect (sockfd, (struct sockaddr *)&addr, sizeof(struct sockaddr)))
	{
		bRet = DE_DISCONNECTED;
		goto cleanup;
	}
	
	//
	// Compose HTTP. 
	//

	UINT len, hdr;
	char header[512];
	char body[512];


	len = sprintf (body,
			"----------------------------7d51bb2150306\r\n"
			"Content-Disposition: form-data; name=\"MacAddress\"\r\n"
			"\r\n"
			"%s\r\n"
			,
			g_tNet.szMacAddress);
		
	body[len] = '\0';

	len += sprintf (body+len,
			"\r\n"
			"----------------------------7d51bb2150306\r\n");

	hdr =  sprintf (header,

			"POST %s HTTP/1.0\r\n"
			"Content-Type: multipart/form-data; boundary="
			"--------------------------7d51bb2150306\r\n"
			"Host: %s\r\n"
			"Content-Length: %u\r\n"
			"Pragma: no-cache\r\n"
			"\r\n"

			,REMOVE_HOST_PATH, g_tNet.szServer, len);
	

	if (send (sockfd, header, hdr, 0) == SOCKET_ERROR)
	{
		bRet =  DE_FAILED_SEND;
		goto cleanup;
	}
	
	if (send (sockfd, body, len, 0)   == SOCKET_ERROR)
	{
		bRet =  DE_FAILED_SEND;
		goto cleanup;
	}

	//
	// Recv reply.
	//

	char reply[512];
	char *s;
	size_t rc;


	if ((rc =recv (sockfd, reply, sizeof(reply), 0)))
	{
		reply[rc] = '\0';
		printf("%s\n",reply);
		if ((s = strrchr (reply, '\n')) == NULL)
		{
			goto cleanup;
		}	

		++s;		
	}

	bRet = ParseResult(s);

	//
	// Cleanup.
	//

cleanup:

	close(sockfd);
	
	return bRet;
}



void Error(char *lpszFunction) 
{ 
    char szBuf[80]; 
 
    sprintf(szBuf, "%s failed: GetLastError returned \n", 
        lpszFunction); 
 
	WriteDebug(szBuf);
} 


const char *MD5_crypt(char *strData, int nlen)
{

	uchar MD5Hash[32]="";
	char szTemp[32]="";
	static char szResult[64]="";
	int i;

	memset(szResult,'\0',sizeof(szResult));
/*
	md5_context ctx;
	md5_starts(&ctx);
	md5_update(&ctx, (uchar*)strData, nlen);
	md5_finish(&ctx, MD5Hash);
*/
	md5_csum((unsigned char *)strData,nlen,MD5Hash);
	for(i=0; i<16; ++i)
	{
		sprintf(szTemp, "%02X", MD5Hash[i]);
		strcat(szResult, szTemp);
	}
	
	return (const char *)szResult;
}

void WriteDebug(char *strDebug)
{
	/* Open for write */
	FILE *fpD=NULL;
	time_t ltime;
#if 1
	time( &ltime );
	if(g_dwDebugSize >= 1048576)
	{
		system("rm -f DDNSClient-old.txt");
		system("mv DDNSClient.txt DDNSClient-old.txt");
	}

	if( (fpD = fopen( "DDNSClient.txt", "a+" )) != NULL )
	{
		fprintf(fpD,"%s\n<< %s >>\n",ctime( &ltime ),strDebug);
		fseek(fpD,0,SEEK_END);
		g_dwDebugSize = ftell(fpD);

		 /* Close stream */
		fclose( fpD );
	}
#else
	printf(strDebug);
#endif
}


int ParseResult(char *szResponse)
{
	int nErrCode=DE_SUCCESS;

//	printf(szResponse);

	if(*szResponse == '0')		//SUCCESS
	{
		++szResponse;
		sprintf(g_szDebug,"%s",szResponse);
	}else if(*szResponse == '2')	//DB_ERROR
	{
		nErrCode = atoi(++szResponse);
		switch(nErrCode)
		{
		case DE_QUERY_FAIL:
			break;
		case DE_NO_USER:
			break;
		case DE_INSERT_DEVICE:
			break;
		case DE_UPDATE_DEVICE:
			break;
		case DE_INSERT_RR:
			break;
		case DE_UPDATE_RR:
			break;
		case DE_INSERT_SOA:
			break;
		case DE_UPDATE_SOA:
			break;
		case DE_INSERT_USER:
			break;
		case DE_UPDATE_USER:
			break;
		case DE_ALREADY_HOST:
			break;
		case DE_REMOVE_RR:
			break;
		case DE_REMOVE_DEVICE:
			break;
		case DE_NO_MACADDRESS:
			break;
		}
	}

	return nErrCode;
}

BOOL IsValidPassword(char *szPasswd)
{
	char ch;
	BOOL bTrue=TRUE;
	int i;
	char szTempPW[32]="";
	sprintf(szTempPW,"%s",szPasswd);
	for(i=0; i<strlen(szPasswd);i++)
	{
		ch = szTempPW[i];
		if(ch == '!' || ch == '&' || ch == '<' || ch == '>' || ch == '*' ||ch == '|'
			|| ch == '\t' || ch =='\n' || ch == ' ')
		{
			bTrue = FALSE;
			break;
		}

	}
	return bTrue;
}

BOOL IsValidHostname(char *szHostname)
{
	char ch;
	BOOL bTrue;
	int i;
	char szTempHost[64]="";
	sprintf(szTempHost,"%s",szHostname);
	for(i=0; i<strlen(szHostname);i++)
	{
		ch = szTempHost[i];
		if((ch >='a' && ch <='z') ||(ch >='A' && ch <='Z') || (ch>='0' && ch <='9') || ch =='-')
		{
			bTrue = TRUE;
		}else
		{
			bTrue = FALSE;
			break;
		}


	}
	return bTrue;
}

