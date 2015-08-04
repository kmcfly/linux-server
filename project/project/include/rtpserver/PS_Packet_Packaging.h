 #ifndef  _PS_PACKET_PACKAGING_H_
#define  _PS_PACKET_PACKAGING_H_
#include "NetProtocol.h"
#include "FrameData.h"
#include "FrameBuffer.h"
#include "dvrdvsdef.h"
#include "NetConfigDefine.h"
#ifdef __ENVIRONMENT_LINUX__
#include "sys/ioctl.h"
#include "net/if.h"
#include "NetInterface.h"
#endif
/************************************************************************
      PS�������ı��ṹ����˵��   
	  I Frame    PS_Header | PS_Map | PES |.......|PES
	  P Frame    PS_Header | PES | .......|PES
	  Audio Frame   PS_Header | (PS_Map) | PES  (��ƵΪͳһ��ʽ����װ��һ��PES���漴��)

	  �������������ޱȱ�֤Ϊһ֡һ֡���������ݣ�֧��H.264��mpeg4


	  �������ñ����ǵ��̵߳��ã������ڲ�δ�����������Ҫ���̵߳��ã����ⲿ����
************************************************************************/

typedef std::list<int> Nal_list;

typedef enum _trick_mode_type_
{
	TRICK_MODE_FAST_FORWORD = 0x00,
	TRICK_MODE_SLOWDOWN = 0x01,
	TRICK_MODE_FREEZE = 0x02,
	TRICK_MODE_FAST_REVERSE = 0x03,
	TRICK_MODE_SLOW_REVERSE = 0x04,
	TRICK_MODE_INVALID = 0xff,
}TRICK_MODE_TYPE;

typedef enum _dsm_cc_retrieve_flag_
{
	//DSM_CC_JUMP,
	DSM_CC_PLAY = 1,
	DSM_CC_PAUSE,
	DSM_CC_RESUME,
	DSM_CC_STOP,
}DSM_CC_RETRIEVE_FLAG;

//��װ����Ϊ��׼��PS��
class CPSPackaging
{
public:
	CPSPackaging();
	virtual ~CPSPackaging();

public:  //����

	//I֡��װ����������pDestBuf�������
	int Packet_I_frame(const char* pSrcBuf , int nSrcLen, char* pDestBuf , int& nDestLen, LONGLONG llPts, LONGLONG llDts);
 
	int Packet_P_frame(const char* pSrcBuf , int nSrcLen, char* pDestBuf , int& nDestLen, LONGLONG llPts, LONGLONG llDts);

	int Packet_Audio_frame(const char* pSrcBuf , int nSrcLen, char* pDestBuf , int& nDestLen, LONGLONG llAudioPts);

	int Packet_DSM_CC(char* pDestBuf , int &nDestLen, LONGLONG Pts, DSM_CC_RETRIEVE_FLAG flag, bool bIsFastMode = false, bool bIsStorage = false);

private: //����
	int Packet_PS_header(char* pDestBuf, int& nLen ,LONGLONG llVideoPts);                    //��װPS_Header
	int Packet_PS_map(char* pDestBuf, int& nLen);											     //��װPS_MAP
	int Packet_System_header(char* pDestBuf, int& nLen);                                         //��װSystem_Header
	int Packet_Video_frame(const char* pSrcBuf, int nSrcLen, char* pDestBuf, int& nLen , LONGLONG llPts, LONGLONG llDts, bool bFirst = true);      //��װ��Ƶ֡
	int Packet_Audio_frame_G711(const char* pSrcBuf , int nSrcLen , char* pDestBuf, int& nLen, LONGLONG llAudioPts);  //��װ��������Ƶ��ʽ
	int PacketAudioMap(char* pDestBuf, int& nLen,int Compression);
	int GeneratePacketsFromFrame(const char* pSrcBuf , int nSrcLen , char* pDestBuf , int& nLen, LONGLONG llPts, LONGLONG llDts); //���NAL��
	int time_code(char* pDestBuf, LONGLONG pts, bool bInfiniteFlag = false);

private:
	LONGLONG		m_PtsFixVideo;
	LONGLONG		m_PtsFixAudio;
};

#endif
