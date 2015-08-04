#ifndef _NET_DEVICE_TYPEDEF_
#define _NET_DEVICE_TYPEDEF_

#ifdef __ENVIRONMENT_WIN32__
#pragma pack(4)
#endif

enum NET_DEVICE_STATUS
{
	ND_STATUS_OK,				//��������
	ND_STATUS_DISCONNECT,		//�����Ѿ��Ͽ�
};

enum NET_DEVICE_STATUS_CONNECT
{
	ND_SC_READY,				//����
	ND_SC_ERROR,				//���ӶϿ���ʱ

	ND_SC_BUSY			= 100,	//��æ
	ND_SC_GET_CFG,				//��ȡ����
	ND_SC_SET_CFG,				//��������
	ND_SC_CTRL_COLOR,			//����ɫ��
	ND_SC_CTRL_PTZ				//������̨
};

enum NET_DEVICE_CTRL_COLOR
{
	ND_CTRL_COLOR_NULL,			//����ɫ��
	ND_CTRL_COLOR_GET,			//��ȡ
	ND_CTRL_COLOR_SET,			//����[ֻ���ͼ��]
	ND_CTRL_COLOR_SAVE,			//���ò����浽����
	ND_CTRL_COLOR_GET_DEFAULT,	//��ȡĬ��
	ND_CTRL_COLOR_SET_DEFAULT	//����Ĭ��
};
enum NET_DEVICE_CTRL_OTHER
{
	ND_CTRL_OTHER_MIRROR,
	ND_CTRL_OTHER_FLIP,
	ND_CTRL_OTHER_ROTATO,
	ND_CTRL_OTHER_AUTOFOCUS,
	ND_CTRL_OTHER_DISTORTION,
    ND_CTRL_OTHER_ROI,
};


//////////////////////////////////////////////////////////////////////////
// ����ֵ����, ������ͨ�ô���
// ����ֵ����NVR���������

enum NET_DEVICE_ERR_COMMON
{
	ND_ERR_OK				= 0,	//���� "Ok"
	ND_ERR_UNKNOWN			= 1,	//δ֪���� "Unknown"

	ND_ERR_CUSTOM			= 1000	//����Ϊ�����Զ��� "Custom"
};

#ifdef	__ENVIRONMENT_WIN32__
#pragma pack()
#endif

#endif
//end
