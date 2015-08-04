#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mount.h>
#include <dirent.h>
#include <signal.h>
#include <sys/socket.h>
#include <linux/types.h>
#include <linux/netlink.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <assert.h>
#include "Netpnp.h"
#include <iostream>

using std::cout;
using std::endl;
using std::hex;


CNetPnp::CNetPnp(int protocol)
:m_netlinkProtocol(protocol)
{
	m_netlinkSock = -1;
	m_pSerachInfo = new BROADCAST_SEARCH_INFO;
	m_pSendNetlinkHdr = (struct nlmsghdr *) new unsigned char[NLMSG_SPACE(MAX_MSGSIZE)];
	m_pRecvNetlinkHdr = (struct nlmsghdr *) new unsigned char[NLMSG_SPACE(MAX_MSGSIZE)];

	m_recvMsgCount = 0;

}


CNetPnp::~CNetPnp()
{
	if (m_pSerachInfo)
	{
		delete m_pSerachInfo;
	}
	
	if (m_pSendNetlinkHdr)
	{
		delete [] ((unsigned char *)m_pSendNetlinkHdr);
	}

	if (m_pRecvNetlinkHdr)
	{
		delete [] ((unsigned char *)m_pRecvNetlinkHdr);
	}
	
	
}


int  CNetPnp::Init()  
{  
	m_netlinkSock = socket(AF_NETLINK, SOCK_RAW,m_netlinkProtocol);  // 使用netlink监听底层传递上来的消息 
	if ( m_netlinkSock < 0 )
	{
		return -1;
	}

	/***取消非阻塞模式***/
	/*int iSave;
	iSave = fcntl(m_netlinkSock,F_GETFL);
	fcntl(m_netlinkSock, F_SETFL, iSave | O_NONBLOCK);*/
	
	memset(&m_netlinkSrcAddr, 0, sizeof(struct sockaddr_nl));  
	m_netlinkSrcAddr.nl_family = AF_NETLINK;  
	m_netlinkSrcAddr.nl_pid = getpid();  
	m_netlinkSrcAddr.nl_groups = 0;  
      
	bind(m_netlinkSock, (struct sockaddr*)&m_netlinkSrcAddr, sizeof(struct sockaddr_nl)); 
	if(bind(m_netlinkSock, (struct sockaddr*)&m_netlinkSrcAddr, sizeof(struct sockaddr_nl)) != 0)
	{
		close(m_netlinkSock);
		return -1;
	}

	memset(&m_netlingDstAddr, 0, sizeof(struct sockaddr_nl));  
	m_netlingDstAddr.nl_family = AF_NETLINK;  
	m_netlingDstAddr.nl_pid = 0;							  
	m_netlingDstAddr.nl_groups = 0;						

	return 0;
}   


int CNetPnp::Exit()
{
	close(m_netlinkSock);
	return 0;
}


int CNetPnp::StartSearch()
{
	memset(m_pSerachInfo,0,sizeof(BROADCAST_SEARCH_INFO));
	m_pSerachInfo->cmdCode = BROADCAST_CODE_CONNECT_START;
	printf("POE StartSearch %d!\n",m_pSerachInfo->cmdCode);
	return SendMessage(m_pSerachInfo);
}


int CNetPnp::EndSearch()
{
	memset(m_pSerachInfo,0,sizeof(BROADCAST_SEARCH_INFO));
	m_pSerachInfo->cmdCode = BROADCAST_CODE_CONNECT_STOP;
	printf("POE EndSearch %d!\n",m_pSerachInfo->cmdCode);
	return SendMessage(m_pSerachInfo);
}

int CNetPnp::GetPortNumByMac( unsigned char* macAddr, int mac_len )
{
	memset( m_pSerachInfo,0,sizeof(BROADCAST_SEARCH_INFO));

	m_pSerachInfo->cmdCode = BROADCAST_CODE_MAC_PORT;
	memcpy( m_pSerachInfo->MacAddr, macAddr, mac_len );
	printf( "3 == %02x,%02x,%02x,%02x,%02x,%02x--%d\n", macAddr[0], macAddr[1], macAddr[2], macAddr[3], macAddr[4], macAddr[5], (int)m_pSerachInfo->cmdCode );


	return SendMessage( m_pSerachInfo );
}


int CNetPnp::SearchDevice()
{	
	memset(m_pSerachInfo,0,sizeof(BROADCAST_SEARCH_INFO));
	
	m_msgCountLock.Lock();
	m_pSerachInfo->cmdCode = BROADCAST_CODE_SEARCH;
	m_pSerachInfo->msg_cnt = m_recvMsgCount;
	m_recvMsgCount = 0;
	m_msgCountLock.UnLock();

	for(int i=0;i<BROADCAST_MAC_ADDR_LEN;i++)
	{
		m_pSerachInfo->MacAddr[i] = 11*i;
	}

	if (BROADCAST_CODE_CONNECT_STOP == m_pSerachInfo->cmdCode)
	{
		printf("POE Serach %d!\n",m_pSerachInfo->cmdCode);
	}

	return SendMessage(m_pSerachInfo);
}

int CNetPnp::GetDevice(BROADCAST_SEARCH_INFO &searchInfo)
{
	memset(&searchInfo,0,sizeof(BROADCAST_SEARCH_INFO));
	if (RecvMessage(&searchInfo) == 0)
	{
		if( BROADCAST_CODE_RESPONSE_MAC_PORT != searchInfo.cmdCode )
		{
			m_msgCountLock.Lock();
			m_recvMsgCount++;
			m_msgCountLock.UnLock();
		}
		return 0;	
	}
	else
	{
		return -1;
	}
}

unsigned long CNetPnp::GetMsgCount()
{
	unsigned long count = 0;
	m_msgCountLock.Lock();
	count = m_recvMsgCount;
	m_msgCountLock.UnLock();
	return count;
}

int CNetPnp::SendMessage(BROADCAST_SEARCH_INFO *pSearchInfo)  
{  
	memset(m_pSendNetlinkHdr, 0, NLMSG_SPACE(MAX_MSGSIZE)); 
	memcpy((BROADCAST_SEARCH_INFO *)NLMSG_DATA(m_pSendNetlinkHdr), pSearchInfo,sizeof(BROADCAST_SEARCH_INFO));  

	m_pSendNetlinkHdr->nlmsg_len   = NLMSG_SPACE(MAX_MSGSIZE);  
	m_pSendNetlinkHdr->nlmsg_pid   = getpid();  
	m_pSendNetlinkHdr->nlmsg_flags = 0; 

	struct iovec iov;
	struct msghdr nl_msg;

	memset(&iov,0,sizeof(iov));
	memset(&nl_msg,0,sizeof(nl_msg));

	iov.iov_base = (void *)m_pSendNetlinkHdr;  
	iov.iov_len  = m_pSendNetlinkHdr->nlmsg_len;

	nl_msg.msg_name    = (void *)&m_netlingDstAddr;  
	nl_msg.msg_namelen = sizeof(m_netlingDstAddr);  
	nl_msg.msg_iov     = &iov;  
	nl_msg.msg_iovlen  = 1;  

	
	int ret = sendmsg(m_netlinkSock, &nl_msg, 0);
	return 0;
} 


int CNetPnp::RecvMessage(BROADCAST_SEARCH_INFO *pSearchInfo)  
{  
	memset(m_pRecvNetlinkHdr, 0, NLMSG_SPACE(MAX_MSGSIZE));       //清空消息缓冲 

	m_pRecvNetlinkHdr->nlmsg_len   = NLMSG_SPACE(MAX_MSGSIZE);  
	m_pRecvNetlinkHdr->nlmsg_pid   = getpid();  
	m_pRecvNetlinkHdr->nlmsg_flags = 0; 

	struct iovec iov;
	struct msghdr nl_msg;

	memset(&iov,0,sizeof(iov));
	memset(&nl_msg,0,sizeof(nl_msg));

	iov.iov_base = (void *)m_pRecvNetlinkHdr;  
	iov.iov_len  = m_pRecvNetlinkHdr->nlmsg_len;  

	nl_msg.msg_name = (void *)&m_netlingDstAddr;  
	nl_msg.msg_namelen = sizeof(m_netlingDstAddr);  
	nl_msg.msg_iov = &iov;  
	nl_msg.msg_iovlen = 1;  

	if (recvmsg(m_netlinkSock, &nl_msg, 0) <= 0)		//接收消息
	{
		return -1;
	}

	BROADCAST_SEARCH_INFO *pTempSearchInfo = NULL;
	pTempSearchInfo = (BROADCAST_SEARCH_INFO *)NLMSG_DATA(m_pRecvNetlinkHdr);
	memcpy(pSearchInfo,pTempSearchInfo,sizeof(BROADCAST_SEARCH_INFO));

	return 0;
} 


