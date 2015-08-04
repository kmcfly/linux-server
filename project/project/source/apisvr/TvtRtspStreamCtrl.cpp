#include "TvtRtspStreamCtrl.h"
#include "MessageMan.h"
#include "MarkupSTL.h"

#define MAX_TVT_RTSP_NAME_LEN 128

CTvtRtspStreamCtrl::CTvtRtspStreamCtrl()
{
	m_StreamIDBuf.reset();
	m_stream_id_number = 2;
}

CTvtRtspStreamCtrl::~CTvtRtspStreamCtrl()
{

}

tuint32 CTvtRtspStreamCtrl::OpenLiveStream( const char* pUser, tuint32 dwChnID ,tint32 streamIndex ,tuint32 &errCode )
{
	int stream_id = SimIEuserLogin( pUser, dwChnID, streamIndex, errCode );
	if( 0 == errCode )
	{
		return stream_id;
	}
	return errCode;
}

bool CTvtRtspStreamCtrl::RequestKeyFrame( tuint32 dwChnID ,tint32 streamIndex )
{

	return true;
}

void CTvtRtspStreamCtrl::CloseLiveStream( tuint32 streamID )
{
	SimIEuserLogout( streamID );
}

tuint32 CTvtRtspStreamCtrl::OpenPlayStream( const char* pUser, tuint32 dwChnID, const NETNS::DEV_DATE& recDate, const NETNS::DEV_TIME& recTime, tuint32 dwTimeLen ,tuint32 &errCode )
{
	printf("OpenPlayStream %lu\n", GetTickCount());

	char pUserName[128] = {0};
	char pPassWd[128] = {0};

	if (ParseUser(pUser, pUserName, pPassWd) < 0)
	{
		errCode = STREAM_CTRL_ERRCODE_INVALID_USER;
		return STREAM_CTRL_ERRCODE_INVALID_USER;
	}	

	unsigned long startTime = 0, endTime = 0;
	if (!TimeConvert(recDate, recTime, startTime))
	{
		return STREAM_CTRL_ERRCODE_INVALID_USER;
	}
	
	if (dwTimeLen <= 0)
	{
		dwTimeLen = 100;//TODO
	}
	endTime = startTime + dwTimeLen;

	unsigned long streamID = 0;
	m_Lock.Lock();
	for (int i=0; i<64; i++)
	{
		if (!m_StreamIDBuf.test(i))
		{
			streamID = i + 1;
			break;
		}
	}

	if (streamID <= 0)
	{
		m_Lock.UnLock();
		errCode = STREAM_CTRL_ERRCODE_INVALID_CH;
		return STREAM_CTRL_ERRCODE_INVALID_CH;
	}

	m_StreamIDBuf.set(streamID-1);
	m_Lock.UnLock();

	RequestPlayBack(dwChnID, streamID, startTime, endTime);

	return streamID;
}

void CTvtRtspStreamCtrl::ClosePlayStream( tuint32 streamID )
{
	printf("ClosePlayStream %lu\n", streamID);
	if (streamID < 1)
	{
		return;
	}
	m_Lock.Lock();
	if (m_StreamIDBuf.test(streamID-1))
	{
		StopPlayBack(streamID);
		m_StreamIDBuf.reset(streamID-1);
	}
	m_Lock.UnLock();
}

int CTvtRtspStreamCtrl::GetMessage( MESSAGE_DATA &msgData )
{
	return 0;
}

int CTvtRtspStreamCtrl::ParseIpAndUserName(const char *pData, char *pUser, char *pPassWd, char* pIpAddr)
{
	m_XmlUserParser.SetDoc(pData);
	do 
	{
		if (!m_XmlUserParser.FindChildElem("username"))
		{
			break;
		}
		m_XmlUserParser.IntoElem();
		strncpy( pUser, m_XmlUserParser.GetData().c_str(), MAX_TVT_RTSP_NAME_LEN );
		m_XmlUserParser.OutOfElem();

		if (!m_XmlUserParser.FindChildElem("password"))
		{
			break;
		}
		m_XmlUserParser.IntoElem();
		strncpy( pPassWd, m_XmlUserParser.GetData().c_str(), MAX_TVT_RTSP_NAME_LEN );
		m_XmlUserParser.OutOfElem();

		if (!m_XmlUserParser.FindChildElem("ip"))
		{
			break;
		}
		m_XmlUserParser.IntoElem();
		strncpy( pIpAddr, m_XmlUserParser.GetData().c_str(), MAX_TVT_RTSP_NAME_LEN );
		m_XmlUserParser.OutOfElem();

		return 0;

	} while(0);

	return -1;
}

int CTvtRtspStreamCtrl::GetClientIDByUserName( const char* pUser, NET_LOGIN_INFO* pLoginInfo,tuint32 &errCode )
{
	char pUserName[MAX_TVT_RTSP_NAME_LEN] = {0};
	char pPassWd[MAX_TVT_RTSP_NAME_LEN] = {0};
	char pIpAddr[MAX_TVT_RTSP_NAME_LEN] = {0};
	int client_id_value = -1;
	bool bFindUser = false;

	if (ParseIpAndUserName(pUser, pUserName, pPassWd, pIpAddr ) < 0)
	{
		errCode = STREAM_CTRL_ERRCODE_INVALID_CH;
		return -1;
	}	

	memset( pLoginInfo, 0, sizeof(NET_LOGIN_INFO));
	strncpy( pLoginInfo->username, pUserName, 36);
	strncpy( pLoginInfo->password, pPassWd, 36 );
	//strncpy(loginInfo.MAC, pIpAddr, 6 );
	pLoginInfo->IP = inet_addr( pIpAddr );
	snprintf( (char*)pLoginInfo->MAC, 6, "%d", pLoginInfo->IP );
	pLoginInfo->connectType = CONNECTTYPE_IE;

	if( 1 )
	{
		std::map<int, NET_LOGIN_INFO>::iterator iter = m_indexUserInfo.begin();
		NET_LOGIN_INFO stItemLoginInfo;
		while(iter != m_indexUserInfo.end())
		{
			memset( &stItemLoginInfo, 0, sizeof(NET_LOGIN_INFO) );
			stItemLoginInfo = iter->second;
			client_id_value = iter->first;
			if( 0 == memcmp( pLoginInfo->username, stItemLoginInfo.username, 35 )
				&& 0 == memcmp( pLoginInfo->password, stItemLoginInfo.password, 35) )
			{
				bFindUser = true;
				printf( "find exister user \n" );
				break;
			}
			iter++;
		}
	}

	errCode = 0;
	if( bFindUser )
	{
		return client_id_value;
	}

	return -1;
}


void CTvtRtspStreamCtrl::SimIePostToServer(unsigned long clientID, unsigned long deviceID, long cmdType, unsigned char *pData, unsigned long dataLen)
{
	assert(clientID == LOCAL_CLIENT_ID);
	PACKCMD cmd;
	MESSAGE_DATA msgData;

	cmd.cmdType	= cmdType;
	cmd.cmdVer	= NET_PROTOCOL_VER;
	cmd.dataLen	= dataLen;

	msgData.clientID = clientID;
	msgData.deviceID = deviceID;
	msgData.dataLen = sizeof(PACKCMD) + dataLen;

	msgData.pData = new char [msgData.dataLen];

	memcpy(msgData.pData, &cmd, sizeof(PACKCMD));

	if ((NULL != pData) && (0 < dataLen))
	{
		memcpy(msgData.pData + sizeof(PACKCMD), pData, dataLen);
	}

	CMessageMan::Instance()->PutMsgToServer(msgData, API_SERVER );

}



int CTvtRtspStreamCtrl::SimIEuserLogin(const char* pUser, tuint32 dwChnID ,tint32 streamIndex ,tuint32 &errCode)
{
	NET_LOGIN_INFO loginInfo;
	int client_id_value = -1;
	int stream_id_value = -1;

	printf( " sim ie user logint %s,%d,%d\n", pUser, dwChnID, streamIndex );

	m_userLockCtrl.Lock();

	if( m_stream_id_number >= (unsigned int)-1 )
	{
		m_stream_id_number = 2;
	}
	stream_id_value = m_stream_id_number++;

	do 
	{
		if( LIVE_FRAME_SECOND_STREAM != streamIndex )
		{
			break;
		}

		client_id_value = GetClientIDByUserName( pUser, &loginInfo, errCode );
		if( 0 != errCode)
		{
			break;
		}

		if( client_id_value < 0 )
		{
			if( m_indexUserInfo.begin() != m_indexUserInfo.end() )
			{
				printf( "too much user login\n" );
				break;
			}

			client_id_value = (int)CMessageMan::Instance()->AllocNetClientID();
			m_indexUserInfo.insert( std::map<int, NET_LOGIN_INFO>::value_type( client_id_value, loginInfo) );
			printf( "current client id = %d\n", client_id_value );

			SimIePostToServer(client_id_value, LOCAL_DEVICE_ID, 
				CMD_REQUEST_LOGIN, reinterpret_cast<BYTE*>(&loginInfo), sizeof(loginInfo));
		}

		RTSP_USER_STREAM_INFO stUserStreamInfo;
		stUserStreamInfo.client_id = client_id_value;
		stUserStreamInfo.chnn_id = dwChnID;
		stUserStreamInfo.stream_id = streamIndex;
		m_userChnnStreamInfo.insert( std::map<unsigned int, RTSP_USER_STREAM_INFO>::value_type( stream_id_value, stUserStreamInfo) );

		NET_LIVESTREAM_CTRL_INFO stNetLiveStreamCtrlInfo;
		memset( &stNetLiveStreamCtrlInfo, 0, sizeof(NET_LIVESTREAM_CTRL_INFO));
		stNetLiveStreamCtrlInfo.secondVideoCH = (1<<(dwChnID));
		SimIePostToServer(client_id_value, LOCAL_DEVICE_ID, 
			CMD_REQUEST_STREAM_START, (unsigned char *)&stNetLiveStreamCtrlInfo, sizeof(NET_LIVESTREAM_CTRL_INFO));

		printf( " open user id chnn stream %d,%d,%d, stream_id_value = %d\n", client_id_value, dwChnID, streamIndex, stream_id_value );

	} while (0);


	m_userLockCtrl.UnLock();

	return stream_id_value;
}

int CTvtRtspStreamCtrl::SimIEuserLogout(tuint32 streamID)
{
	NET_LOGIN_INFO loginInfo;
	unsigned int client_id = 0;
	bool bHasLogin = false;
	int dwChnID = 0;

	printf( " sim ie user logout %d\n", streamID );

	m_userLockCtrl.Lock();

	if( m_userChnnStreamInfo.count( streamID ) > 0 )
	{
		dwChnID = m_userChnnStreamInfo[streamID].chnn_id;
		client_id = m_userChnnStreamInfo[streamID].client_id;

		NET_LIVESTREAM_CTRL_INFO stNetLiveStreamCtrlInfo;
		memset( &stNetLiveStreamCtrlInfo, 0, sizeof(NET_LIVESTREAM_CTRL_INFO));
		stNetLiveStreamCtrlInfo.secondVideoCH = (1<<(dwChnID));
		SimIePostToServer(client_id, LOCAL_DEVICE_ID, 
			CMD_REQUEST_STREAM_STOP, (unsigned char *)&stNetLiveStreamCtrlInfo, sizeof(NET_LIVESTREAM_CTRL_INFO));

		//É¾³ýµ±Ç°stream id 
		m_userChnnStreamInfo.erase( streamID );

		bHasLogin = false;
		std::map<unsigned int, RTSP_USER_STREAM_INFO>::iterator iter = m_userChnnStreamInfo.begin();
		while(iter != m_userChnnStreamInfo.end())
		{
			if( LIVE_FRAME_SECOND_STREAM == iter->second.stream_id && client_id == iter->second.client_id )
			{
				bHasLogin = true;
				break;
			}
			iter++;
		}

		if( !bHasLogin )
		{
			SimIePostToServer(client_id, LOCAL_DEVICE_ID, CMD_REQUEST_LOGOUT, NULL, 0);

			m_indexUserInfo.erase( client_id );

			printf( "close user id = %d\n", client_id );
		}
	}

	m_userLockCtrl.UnLock();

	return 0;
}

