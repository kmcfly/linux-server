#ifndef _DDNS_CLIENT_H_
#define _DDNS_CLIENT_H_

#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <netdb.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

enum DDNS_IPUPDATE_STATUS
{
	DDNS_IPUPDATE_OK = 0,
	DDNS_IPUPDATE_ENAMEPWD,
	DDNS_IPUPDATE_EHOSTDOMAIN,							//用户名、密码或主机域名错误(dyndns.com)
	DDNS_IPUPDATE_ETIMEOUT,								//连接超时
	DDNS_IPUPDATE_ECONNECT,								
	DDNS_IPUPDATE_ENETUNREACHD,							//网络不可达
	DDNS_IPUPDATE_EHOSTDOMAIN_EMPTY,					//主机域名为空
	DDNS_IPUPDATE_EUNKNOWN,
	DDNS_IPUPDATE_ABUSE,								//dyndns.com上注册的域名被block
	DDNS_IPUPDATE_NOHOST,								//dyndns.com上找不到相关域名
	DDNS_IPUPDATE_NOTFQDN,								//主机域名不符合规则
	DDNS_IPUPDATE_NUMHOST,								//太多的主机在update
	DDNS_IPUPDATE_REGISTER_SUCC,						//向visionica服务器注册成功
	DDNS_IPUPDATE_IP_ERROR,								//输入的IP错误
	DDNS_IPUPDATE_REGISTER_FAIL,						//注册失败
	DDNS_AUTOREG_DOMAIN_ALREADY_USE,					//IN23域名已经被使用
	DDNS_AUTOREG_DOMAIN_CREATE_OK,						//IN23域名建立成功
	DDNS_AUTOREG_DOMAIN_REPLACE_OK,						//IN23域名更换成功
};


#define BUFSIZE 4096

const unsigned long CONNECT_TIME_OUNT = 33000;			//连接超时时间为33S,33s内发送了三次连接

class CDdnsClient
{
public:
	CDdnsClient();
	virtual ~CDdnsClient();
	
	bool SetDDNSServerName(const char *pServerName);
	virtual	void SetDeviceName(const char *pDeviceName){;}
	virtual	void SetHttpPort(unsigned short httpPort){;}
	virtual	void SetServerPort(unsigned short serverPort){;}
	virtual void SetMac(const char * pMac){;}
	virtual	unsigned long DdnsIPupdate( const char *pUserName, const char *pPassword, const char *pmyDomainName );
	virtual bool RemoveDDNS();//只有IN23才需要删除DDNS
	void base64Encode(char *intext, char *output);
	DDNS_IPUPDATE_STATUS GetMessageFromServer(int sockfd, char *pbuf, int &len);

	//获得设备外网IP地址
	int GetExternalIp(char *pIPAddr );
	int ConnectServer(int connectFd, struct sockaddr *pServerSockAddr ,int sockAddrLen, unsigned long timeOut);

protected:
	char m_DdnsServerName[132];
	char m_szExternalIp[16];
private:

};
#endif
