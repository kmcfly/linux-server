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
	DDNS_IPUPDATE_EHOSTDOMAIN,							//�û����������������������(dyndns.com)
	DDNS_IPUPDATE_ETIMEOUT,								//���ӳ�ʱ
	DDNS_IPUPDATE_ECONNECT,								
	DDNS_IPUPDATE_ENETUNREACHD,							//���粻�ɴ�
	DDNS_IPUPDATE_EHOSTDOMAIN_EMPTY,					//��������Ϊ��
	DDNS_IPUPDATE_EUNKNOWN,
	DDNS_IPUPDATE_ABUSE,								//dyndns.com��ע���������block
	DDNS_IPUPDATE_NOHOST,								//dyndns.com���Ҳ����������
	DDNS_IPUPDATE_NOTFQDN,								//�������������Ϲ���
	DDNS_IPUPDATE_NUMHOST,								//̫���������update
	DDNS_IPUPDATE_REGISTER_SUCC,						//��visionica������ע��ɹ�
	DDNS_IPUPDATE_IP_ERROR,								//�����IP����
	DDNS_IPUPDATE_REGISTER_FAIL,						//ע��ʧ��
	DDNS_AUTOREG_DOMAIN_ALREADY_USE,					//IN23�����Ѿ���ʹ��
	DDNS_AUTOREG_DOMAIN_CREATE_OK,						//IN23���������ɹ�
	DDNS_AUTOREG_DOMAIN_REPLACE_OK,						//IN23���������ɹ�
};


#define BUFSIZE 4096

const unsigned long CONNECT_TIME_OUNT = 33000;			//���ӳ�ʱʱ��Ϊ33S,33s�ڷ�������������

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
	virtual bool RemoveDDNS();//ֻ��IN23����Ҫɾ��DDNS
	void base64Encode(char *intext, char *output);
	DDNS_IPUPDATE_STATUS GetMessageFromServer(int sockfd, char *pbuf, int &len);

	//����豸����IP��ַ
	int GetExternalIp(char *pIPAddr );
	int ConnectServer(int connectFd, struct sockaddr *pServerSockAddr ,int sockAddrLen, unsigned long timeOut);

protected:
	char m_DdnsServerName[132];
	char m_szExternalIp[16];
private:

};
#endif
