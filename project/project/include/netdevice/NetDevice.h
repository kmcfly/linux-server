/***********************************************************************
** Copyright (C) Tongwei Video Technology Co.,Ltd. All rights reserved.
** Author       : Ԭʯά
** Date         : 2009-09-10
** Name         : NetDevice.h
** Version      : 1.0
** Description  :
** Modify Record:
1:
***********************************************************************/
#ifndef _NET_DEVICE_H_
#define _NET_DEVICE_H_
/************************************************************************
1��	���ļ���������DVR�豸����IP�����豸ʱ�Ľӿڹ淶��
2��	Ŀ����ͨ���򵥹淶�ؽӿڣ�ʵ�ֶԶ���IP�豸�ļ��ݣ�����ܻ���ͳһ�ı�׼
����á�
3��	IP�豸��������Ƶ���֧��RTSP�����Ƿ�Ϳ���ͳһ���뷽ʽʵ�ֶ�����Ƶ�Ĵ���
�����������������Ҫ��ÿ���豸ʵ�ֽӿ��ࡣ
4��	������Ƶ��Ŀ��ƺ�ͨ���������Բ���XML�淶�����������Ͳ���Ҫ��¶��������
ϸ�ڣ����ﵽ���ݵ�Ŀ�ġ�
5��	���ǵ��Ժ�Ҫ�����������ҵ��豸����ʵ�ֻ��Ż�ͨ�����Խӿ��ϾͲ�����������
�ķ�ʽ������
6��	ÿ���豸ֻ���ڿ���һ���̡߳�
************************************************************************/

#include <string>
#include <list>

#include "NetDeviceTypedef.h"
#include "NetDeviceDefine.h"
#include "IPDataBufferMan.h"

class CNetDevice
{
public:
	CNetDevice()
	{
		m_reConnectCount = -1;
		m_bUseBaseLine = false;
	}
	virtual ~CNetDevice()
	{
		m_reConnectCount = -1;
	}

	//////////////////////////////////////////////////////////////////////////
	/************************************************************************
	�ӿڹ�����������ʼ���豸�ӿڡ�
	�������������
	1��chnn��	��������豸��ͨ���ţ�
	2��IP��		���豸�������ַ��
	3��user��	���ڵ�¼���û����ƣ�
	4��netPort����¼�˿�
	5��httpPort��IE���Ӷ˿�
	6��ctrlPort�� ���ƶ˿�
	4��password�����ڵ�¼�����룻
	5��mac:		�����豸�����������ַ�����ڵ�¼��֤ʹ�á�
	����ֵ���������سɹ�����ʧ�ܡ�
	ע: �ڸýӿ��ڲ�����Ҫ���Ե�¼��IP�豸�����ݵ�¼�Ľ�����ؽ����
	ע: �ڽӿ��ڲ�, �����ظ���ʼ��
	ע: �����豸�ӿ�, ������������״̬����Ӧ����
	************************************************************************/
	virtual bool Initial(nd_in long deviceId,						//
						 nd_in long chnn,							//
						 nd_in char *pServer,						//
						 nd_in unsigned short netPort,				//
						 nd_in unsigned short httpPort,				//
						 nd_in unsigned short ctrlPort,				//
						 nd_in const std::string & user,			//
						 nd_in const std::string & password,		//
						 nd_in const ND_MAC & mac,					//
						 nd_in const char* pEther="eth0",
						 nd_in long sensorNum=0) = 0;				//
	

	/************************************************************************
	�ӿڹ������� : ����ʼ��
	************************************************************************/
	virtual void Quit() = 0;

	/************************************************************************
	�Ƿ���Ҫ������ʱ���������豸�����ò���
	************************************************************************/
	virtual bool IfNeedInitNetConfig(){return true;}

	/************************************************************************
	�ӿڹ������� : �������ݻ��������
	1����������NVR����.
	2���յ�����ͼ������(H264)��������.
	************************************************************************/
	virtual void SetDataBufferMan(nd_in CIPDataBufferMan *pDataBufferMan) = 0;

	/************************************************************************
	�ӿڹ������� : �����Ƿ�ʹ��baseline����
	************************************************************************/
	virtual void SetUseBaseLine(bool bUseBaseLine){m_bUseBaseLine = bUseBaseLine;}

	/***********************************************************************
	�޸��豸�󶨵�ͨ����
	************************************************************************/
	virtual void SetBindChannel(nd_in long chnn) = 0;
	virtual long GetBindChannel() = 0;

	virtual void SetLocalVideoNum(long localNum) =0;
	virtual unsigned int GetLocalVideoNum() =0;

	/***********************************************************************
	�޸��豸�󶨵Ĵ�����ͨ����
	************************************************************************/
	virtual void SetBindSensorChannel(long sensorChnn)=0;
	virtual long GetBindSensorChannel()=0;

	/***********************************************************************
	�õ��������豸��״̬,�Ͽ�������
	************************************************************************/
	virtual long GetDevStatus() = 0;
	
	/***********************************************************************
	�鿴�����豸�Ƿ���������
	************************************************************************/
	virtual bool IsReConnectting()
	{
		if (0 <= m_reConnectCount)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	/************************************************************************
	�õ���ǰ����״̬, ND_SC_BUSY����, ��Ϊ���ܴ����������                                                                    
	************************************************************************/
	virtual long IsReady() = 0;

	/************************************************************************
	�ӿڹ���������������Ƶ����Ƶ��������
	�������������
	1��video1��������MASK��
	2��video2��������MASK��
	3��video3���ֻ�����MASK��
	4��video4����Ƶ��MASK��
	����ֵ���������سɹ�����ʧ�ܡ�
	ע����Ӧ��λΪ�㣬��ʾֹͣ��
	************************************************************************/
	virtual bool RequestStream( nd_in ULONGLONG video1,
								nd_in ULONGLONG video2,
								nd_in ULONGLONG video3,
								nd_in ULONGLONG audio) = 0;

	/************************************************************************
	�ӿڹ�������������ͼƬ��������
	�������������
	1��channlMask: ͼƬ��ͨ��MASK��
	����ֵ���������سɹ�����ʧ�ܡ�
	ע����Ӧ��λΪ�㣬��ʾֹͣ��
	************************************************************************/
	virtual bool RequestJpegStream(nd_in ULONGLONG channlMask){return true;}

	/************************************************************************
	�ӿڹ���������������Ƶ�ؼ�֡
	�������������
	1��video1��������MASK��
	2��video2��������MASK��
	3��video3���ֻ�����MASK��
	����ֵ���������سɹ�����ʧ�ܡ�
	ע����Ӧ��λΪ�㣬��ʾֹͣ��
	************************************************************************/
	virtual bool RequestKeyFrame(nd_in ULONGLONG video1, 
								 nd_in ULONGLONG video2,
								 nd_in ULONGLONG video3)
	{
		return true;
	}


	/************************************************************************
	�ӿڹ�����������̨���ƽӿڡ�
	�������������
	************************************************************************/
	virtual bool PTZControl(nd_in long chnn,
							nd_in unsigned long cmdtype,
							nd_in unsigned long ptzParam,
							nd_in unsigned long speed,
							nd_in unsigned long waiteTime) = 0;


	/************************************************************************
	�ӿڹ�����������ȡͼ��ɫ����Ϣ��
	************************************************************************/
	virtual bool ColorControl(nd_in long chnn,
							  nd_in unsigned long cmdtype,
							  nd_in_out DD_VIDEO_COLOR& videoColor) = 0;
    /*
    function:
	���񣬷�ת������ģʽ������У�����Զ��Խ���ROI(����Ȥ��)
	*/
	virtual bool OtherControl(nd_in long chnn,
							  nd_in unsigned long cmdtype,
							  nd_in unsigned char *pChar) = 0;


	/************************************************************************
	�ӿڹ����������豸�ı�����Ϣ��
	�������������
	1��alarm��������Ϣ
	����ֵ�������ޡ�
	************************************************************************/
	virtual ULONGLONG GetMotionAlarm() = 0;
	virtual ULONGLONG GetSensorAlarm() = 0;
	virtual ULONGLONG GetVlossAlarm() = 0;
	virtual ULONGLONG GetOtherAlarm() = 0;


	/************************************************************************
	�ӿڹ������� : ��������
	1��
	************************************************************************/
	virtual long SetConfig( nd_in long chnn,
							nd_in unsigned long itemID,
							nd_in const unsigned char* pData,
							nd_in unsigned long dataLen,
							nd_in bool bImmediately) = 0;

	virtual long FlushConfig() = 0;

	/************************************************************************
	����ʱ��
	************************************************************************/
	virtual long SetDateTime(nd_in const DD_TIME& ddTm) = 0;

	/************************************************************************
	��ȡ�豸֧�ֲ�����Ϣ, videoIndexȡֵ0��1��2��ʾ�ĸ�����
	************************************************************************/
	virtual long GetDevInfo(nd_in long videoIndex, nd_out ND_DEV_INFO& ndDevInfo) = 0;
	virtual long GetEncodeInfo(char * pVideoEncodeInfo, int iVideoEncodeInfoLen) = 0;

	/************************************************************************
	�ӿڹ������� : ������ 
	1��������Ϊ�������, ������Ϊ���ش���
	************************************************************************/
	virtual bool GetLastError(nd_out std::string& strErr) = 0;
	virtual long GetLastError() = 0;
	virtual void SetLastError(nd_in long err) = 0;

	/************************************************************************
	ÿ���ӵ���һ�Σ�֪ͨ��ǰʱ�䣬ģ����ô���Ϊ��ʱ��
	************************************************************************/
	virtual void OnTimer(unsigned long curTime) = 0;

	/***********************************************************************
	�������糬ʱʱ��,��λms
	***********************************************************************/
	virtual void SetNetTimeout(unsigned long msTimeout) = 0;

	/***********************************************************************
	��ȡ�豸�����Ƿ����300��
	***********************************************************************/
	virtual unsigned long GetDevResolutionVal( int* width, int* height ) = 0;

	//��ʱͬ��IPC��ʱ��
	virtual int ResyncIPCSystemTime(int sync_flag) = 0;

protected:
	int m_reConnectCount;
	bool m_bUseBaseLine;
};

#endif //_NET_DEVICE_H_

//end
