#ifndef _NETPNP_HEADER_
#define _NETPNP_HEADER_

#ifndef __ENVIRONMENT_WIN32__
#include <sys/socket.h>
#include <linux/netlink.h>
#endif
#include "PUB_common.h"

#define MAX_MSGSIZE 128
#define NETLINK_TVT 26

#define BROADCAST_MAC_ADDR_LEN 6
#define BROADCAST_PASSWD_LEN   28


enum BROADCAST_COMMAND_CODE
{
	BROADCAST_CODE_CONNECT_STOP,
	BROADCAST_CODE_CONNECT_START,
	BROADCAST_CODE_SEARCH,
	BROADCAST_CODE_RESPONSE_SEARCH,
	BROADCAST_CODE_PHY_LINK_STATUS,
	BROADCAST_CODE_MAC_PORT,
	BROADCAST_CODE_RESPONSE_MAC_PORT,
};


typedef struct _broadcast_search_info_
{	
	unsigned long	head;		//��FLAG_MULTIHEAD
	unsigned long	ver;		//�ṹ��Ϣ�汾
	unsigned long	cmdCode;	//BROADCAST_COMMAND_CODE
	unsigned char	MacAddr[BROADCAST_MAC_ADDR_LEN];
	unsigned short	macChannel;
	unsigned long	deviceType;
	unsigned long	productType;
	char			szPasswd[BROADCAST_PASSWD_LEN];   //����IP����������ȷ��admin���룬����base64����
	unsigned long	msg_cnt;	                      //������Ϣ�ĸ���
	unsigned int	PhyLinkStatus;	                  //PHY������״̬
}BROADCAST_SEARCH_INFO;

typedef struct _ipc_onvif_mac_phy_port
{
	unsigned char	MacAddr[BROADCAST_MAC_ADDR_LEN];
	unsigned short	macChannel;
}IPC_ONVIF_MAC_PHY_PORT;


class CNetPnp
{
public:
	CNetPnp(int protocol = NETLINK_TVT);
	~CNetPnp();

	int Init();
	int Exit();

	int StartSearch();
	int EndSearch();

	int SearchDevice();
	int GetDevice(BROADCAST_SEARCH_INFO &searchInfo);
	
	unsigned long GetMsgCount();

	int GetPortNumByMac( unsigned char* macAddr, int mac_len );

private:
	int SendMessage(BROADCAST_SEARCH_INFO *pSearchInfo);
	int RecvMessage(BROADCAST_SEARCH_INFO *pSerachInfo);

private:  

#ifndef __ENVIRONMENT_WIN32__
	struct sockaddr_nl m_netlinkSrcAddr;
	struct sockaddr_nl m_netlingDstAddr;

	struct nlmsghdr *m_pSendNetlinkHdr;	//������Ϣ����
	struct nlmsghdr *m_pRecvNetlinkHdr; //������Ϣ����
#endif

	int  m_netlinkSock;
	int  m_netlinkProtocol;         //�Զ���netlinkЭ���

	BROADCAST_SEARCH_INFO *m_pSerachInfo;

	unsigned long m_recvMsgCount;  //��һ���������豸��Ϣ����
	CPUB_Lock     m_msgCountLock;  //����m_recvMsgCount
};




#endif