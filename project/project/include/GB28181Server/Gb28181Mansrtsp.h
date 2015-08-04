#ifndef __MANSRTSP__
#define __MANSRTSP__

/*error codes define*/
#define ERR_NOERROR          0
#define ERR_GENERIC             -1
#define ERR_NOT_FOUND       -2
#define ERR_PARSE           -3
#define ERR_ALLOC               -4
#define ERR_INPUT_PARAM         -5
#define ERR_NOT_SD          -6
#define ERR_UNSUPPORTED_PT      -7
#define ERR_EOF             -8
#define ERR_FATAL                   -9
#define ERR_CONNECTION_CLOSE        -10


#define RTSP_VER "MANSRTSP/1.0"
#define RTSP_EL "\r\n"
#define RTSP_RTP_AVP "RTP/AVP"

/*��Ϣͷ��ؼ���*/
#define HDR_CONTENTLENGTH "Content-Length"
#define HDR_ACCEPT "Accept"
#define HDR_ALLOW "Allow"
#define HDR_BLOCKSIZE "Blocksize"
#define HDR_CONTENTTYPE "Content-Type"
#define HDR_DATE "Date"
#define HDR_REQUIRE "Require"
#define HDR_TRANSPORTREQUIRE "Transport-Require"
#define HDR_SEQUENCENO "SequenceNo"
#define HDR_CSEQ "CSeq"
#define HDR_STREAM "Stream"
#define HDR_SESSION "Session"
#define HDR_TRANSPORT "Transport"
#define HDR_RANGE "Range"	
#define HDR_USER_AGENT "User-Agent"
#define HDR_SCALE "Scale"

/*RTSP  ����*/
// #define MAN_RTSP_METHOD_MAXLEN 15
// #define MAN_RTSP_METHOD_DESCRIBE "DESCRIBE"
// #define MAN_RTSP_METHOD_ANNOUNCE "ANNOUNCE"
// #define RTSP_METHOD_GET_PARAMETERS "GET_PARAMETERS"
// #define RTSP_METHOD_OPTIONS "OPTIONS"
// #define RTSP_METHOD_RECORD "RECORD"
// #define RTSP_METHOD_REDIRECT "REDIRECT"
// #define RTSP_METHOD_SETUP "SETUP"
// #define RTSP_METHOD_SET_PARAMETER "SET_PARAMETER"

#define MAN_RTSP_METHOD_PAUSE "PAUSE"
#define MAN_RTSP_METHOD_PLAY "PLAY"
#define MAN_RTSP_METHOD_TEARDOWN "TEARDOWN"


/*RTSP�ķ������ID*/
// #define RTSP_ID_DESCRIBE 0
// #define RTSP_ID_ANNOUNCE 1
// #define RTSP_ID_GET_PARAMETERS 2
// #define RTSP_ID_OPTIONS 3
// #define RTSP_ID_PAUSE 4
// #define RTSP_ID_PLAY 5
// #define RTSP_ID_RECORD 6
// #define RTSP_ID_REDIRECT 7
// #define RTSP_ID_SETUP 8
// #define RTSP_ID_SET_PARAMETER 9
#define MAN_RTSP_ID_PAUSE 4
#define MAN_RTSP_ID_PLAY 5
#define MAN_RTSP_ID_TEARDOWN 10

typedef struct  _man_rtsp_buffer_
{
	unsigned  long rtsp_cseq;
	unsigned  long session_id;
	char in_buffer[2048];
}ManRtsp_buffer;

typedef struct _play_args
{                              
	unsigned  long  start_time;                                                     
	unsigned  long end_time; 
	struct tm tm_time;       
	float fScale;
}Play_args;

typedef struct _playback_time
{                              
	unsigned  long  start_time;                                                     
	unsigned  long end_time; 
}PLAYBACK_TIME;
class CMansRTSP
{
public:
	static CMansRTSP* Instance();

	CMansRTSP();

	int MansRtspMsgProc(unsigned long ulChannel,const string & strMsgBody, string & sipMsgBodyResp);

	//���ûط�npt�Ļ���ʱ��
	void SetPlaybackTime(unsigned long ulChannel,unsigned long ulStartTime,unsigned long ulEndTime);
	
	int GetUtcTime(struct tm *t,char *b);
private:

	~CMansRTSP();

   //��ȡ��Ӧ�Ĵ�����
	char *get_staterr(int err);

    //��ȡʱ��
    void add_time_stamp(char *b, int crlf);

     //�ظ���Ϣ
	int send_reply(int err,ManRtsp_buffer *rtsp,string & strMsgBodyResp);

   //PLAY��Ϣ��Ӧ��
	void  send_play_reply(ManRtsp_buffer *rtsp,string & strMsgBodyResp);

	//PLAY��Ľ���
	int RTSP_Play(ManRtsp_buffer *rtsp,string & strMsgBodyResp);

	//PAUSE�Ľ���
	int RTSP_Pause(ManRtsp_buffer *rtsp,string & strMsgBodyResp);

    //TEARDOWN ����
	int RTSP_Teardown(ManRtsp_buffer *rtsp,string & strMsgBodyResp);

	//��ȡmethod����
	int RTSPGetMethod(ManRtsp_buffer *rtsp);
   
	//RTSP ״̬������
	int RTSP_state_machine(int m,ManRtsp_buffer *rtsp,string & strMsgBodyResp);
	
	//����ʱ�䣬��������ط�����
	void UptateDataReadTime(unsigned  long nChannelNum,unsigned  long  ulStartTime ,unsigned  long ulEndTime);

	//ֹͣ����ط�����
	void  StopDataReadTime(unsigned  long nChannelNum);
   
	//���浱ǰ��ͨ��ID
	void SetCurrentChannelID(unsigned long ulChannel);
   
	//��ȡ��ǰ��ͨ��ID
	unsigned long GetCurrentChannelID();

	void LockPlaybackCtrl() {m_PalybackLock.Lock();}
	void UnLockPlaybackCtrl() {m_PalybackLock.UnLock();}
private:	

	unsigned long               m_ulCurrentChannelID;           //��ǰ�����Ļط���ͨ��ID
	PLAYBACK_TIME	            m_PlaybackTime[16];           //����ط�ʱ����һ���·�NPT����Ի���ʱ��  
	CGb28181Server				*m_pGb28181Server;			//����Gb28181Server�ӿ�
	CSipServer					*m_pSipServer;			//��������SipServer
	CPUB_Lock					m_PalybackLock;			//playback lock
};
#endif

