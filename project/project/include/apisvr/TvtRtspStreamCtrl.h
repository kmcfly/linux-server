#ifndef __TVT_RTSP_STREAM_CTRL__
#define __TVT_RTSP_STREAM_CTRL__

#include "NetServerSDK.h"
#include "MessageMan.h"
#include "APIMsgDistributor.h"
#include <bitset>
#include "MarkupSTL.h"

//errCode����Ҫ�ŵ�NetServerSDK��
enum STREAM_CTRL_ERRCODE
{
	STREAM_CTRL_ERRCODE_INVALID_USER		= -1,
	STREAM_CTRL_ERRCODE_INVALID_CH			= -2,
	
};

class CTvtRtspStreamCtrl: public CStreamCtrl, public CAPIMsgDistributorInterface
{
public:
	CTvtRtspStreamCtrl();
	virtual ~CTvtRtspStreamCtrl();

	//���ֳ���, -1(0xffffffff):ʧ�ܣ�errCode�洢����ԭ��pUser�洢���û��� ���� IP��ַ
	tuint32 OpenLiveStream(const char* pUser, tuint32 dwChnID ,tint32 streamIndex ,tuint32 &errCode);

	//�ֳ���ʱ�ؼ�֡
	virtual bool RequestKeyFrame(tuint32 dwChnID ,tint32 streamIndex);

	//�ر��ֳ���streamIDΪOpenLiveStream�ķ���ֵ
	void CloseLiveStream( tuint32 streamID);

	//�򿪻ط���, -1(0xffffffff):ʧ�ܣ�errCode�洢����ԭ��pUser�洢���û��� ���� IP��ַ
	tuint32 OpenPlayStream(const char* pUser, tuint32 dwChnID, const NETNS::DEV_DATE& recDate, const NETNS::DEV_TIME& recTime, tuint32 dwTimeLen ,tuint32 &errCode);

	//�رջط���streamIDΪOpenPlayStream�ķ���ֵ
	void ClosePlayStream( tuint32 streamID);
public:
	virtual int GetMessage(MESSAGE_DATA &msgData);


private:
	int SimIEuserLogin(const char* pUser, tuint32 dwChnID ,tint32 streamIndex ,tuint32 &errCode);
	int SimIEuserLogout(tuint32 streamID);
	int ParseIpAndUserName(const char *pData, char *pUser, char *pPassWd, char* pIpAddr );
	int GetClientIDByUserName( const char* pUser, NET_LOGIN_INFO* pLoginInfo,tuint32 &errCode );
	void SimIePostToServer(unsigned long clientID, unsigned long deviceID, long cmdType, unsigned char *pData, unsigned long dataLen);

private:
	std::bitset<64>			m_StreamIDBuf;
	CPUB_Lock				m_Lock;

	/////////////////////////////////////////////////////////
	CMarkupSTL				m_XmlUserParser;

	std::map<int, NET_LOGIN_INFO>		m_indexUserInfo;
	CPUB_Lock                           m_userLockCtrl;

	typedef struct _RTSP_USER_STREAM_INFO
	{
		int client_id;
		int chnn_id;
		int stream_id;
	}RTSP_USER_STREAM_INFO;

	unsigned int                                 m_stream_id_number;
	std::map<unsigned int, RTSP_USER_STREAM_INFO>                  m_userChnnStreamInfo;
	//////////////////////////////////////////////////////////////////

};

#endif