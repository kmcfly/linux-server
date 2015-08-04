#ifndef _PTZ_H
#define _PTZ_H
#include "dvrdvsdef.h"
#define MAX_PRESET_NUM   255                            /*���Ԥ�õ���*/
/*direction  */
#define    PTZ_CTRL_DIRECTION_UP                  0x08                        /*��̨���ֽ�6�������ٶ�ֵ���Ϸ����˶�*/
#define    PTZ_CTRL_DIRECTION_DOWN             0x04                       /*��̨���ֽ�6�������ٶ�ֵ���·����˶�*/
#define    PTZ_CTRL_DIRECTION_LEFT                0x02                      /*��̨���ֽ�5�������ٶ�ֵ�������˶�*/
#define    PTZ_CTRL_DIRECTION_RIGHT              0x01                     /*��̨���ֽ�5�������ٶ�ֵ���ҷ����˶�*/
#define    PTZ_CTRL_STOP_ALL                           0x00                      /*PTZ�����в�����ֹͣ*/
#define    PTZ_CTRL_DIRECTION_LEFT_UP          0x0A                        /*��̨���ֽ�5�������ٶ�ֵ�������˶���ͬʱ���ֽ�6���ٶ������˶�*/
#define    PTZ_CTRL_DIRECTION_LEFT_DOWN           0x06                     /*��̨���ֽ�5�������ٶ�ֵ�������˶���ͬʱ���ֽ�6���ٶ������˶�*/
#define    PTZ_CTRL_DIRECTION_RIGHT_UP           0x09                       /*��̨���ֽ�5�������ٶ�ֵ���ҷ����˶���ͬʱ���ֽ�6���ٶ������˶�*/
#define    PTZ_CTRL_DIRECTION_RIGHT_DOWN           0x05						/*��̨���ֽ�5�������ٶ�ֵ���ҷ����˶���ͬʱ���ֽ�6���ٶ������˶�*/
/*shot control*/

#define    PTZ_CTRL_ZOOM_OUT                           0x10                     /*��ͷ���ֽ�7��4λ����ֵ�䱶�Ŵ�*/
#define    PTZ_CTRL_ZOOM_IN                        0x20                        /*��ͷ���ֽ�7��4λ����ֵ�䱶��С*/

#define    PTZ_CTRL_APERTURE_SMALL               0x48                 /*��ͷ���ֽ�6����ֵ��С��Ȧ*/
#define    PTZ_CTRL_APERTURE_LARGE               0x44                /*��ͷ���ֽ�6����ֵ�Ŵ��Ȧ*/

#define    PTZ_CTRL_FOCUS_NEAR                       0x42                         /*��ͷ���ֽ�5����ֵ�۽���*/
#define    PTZ_CTRL_FOCUS_FAR                          0x41                       /*��ͷ���ֽ�5����ֵ�۽�Զ*/

#define    PTZ_CTRL_STOP_SHOT_ALL                   0x40            /*ֹͣ��ͷ�����в���*/

/*preset*/
#define   PTZ_CTRL_PRESET_SET                           0x81                        /*����Ԥ��λ*/
#define   PTZ_CTRL_PRESET_CALL                         0x82                   /*����Ԥ��λ*/
#define   PTZ_CTRL_PRESET_CLEAR                       0x83         /*���һ��Ԥ��λ*/

/*cruise*/
#define    PTZ_CTRL_CRUISE_ADD                        0x84          /*����һ��Ѳ����*/
#define   PTZ_CTRL_CRUISE_CLEAR                     0x85         /*���һ��Ѳ����*/
#define   PTZ_CTRL_CRUISE_SPEED                     0x86          /*����Ѳ�����ٶ�*/
#define   PTZ_CTRL_CRUISE_TIME                       0x87             /*����Ѳ��ͣ��ʱ��*/
#define   PTZ_CTRL_CRUISE_START                    0x88              /*��ʼѲ��*/

#define  PTZ_CTRL_AUTO_SCAN_START                        0x89    /*��ʼ�Զ�ɨ��*/
#define  PTZ_CTRL_AUTO_SCAN_SPEED                         0x8A        /*�����Զ�ɨ���ٶ�*/

class CPTZControl
{
public:
	static CPTZControl* Instance();

	CPTZControl();
	
	//PTZ�������
	int ControlParsePtz(char *PtzStr, unsigned  long nChannelNum);

	/*����PTZ��ͨ��*/
	void SetPtzChannel(unsigned  long  ulChnn);

private:

	//ʹ�õ����������ù��캯��˽��
	~CPTZControl();

	int ParsePtzCommand(unsigned char *buf);

	//����PTZ��������
	void SendPtzCtrlCmd(PTZ_CTRL *pCtrl);

	/*������������*/
	void ParseCombinedCode(unsigned char *buf);

	/*��������룬�����˶����� �������ϣ�����*/
	void ParseCombinedDirection(unsigned char *buf);

	/*��������룬FI����*/
	void ParseCombinedFI(unsigned char *buf);

	/*���ַ���ת��Ϊ16�����������У���*/
	int ParsePtzStr(  char *PtzStr,  unsigned char  *buf);

	// ���ν��ַ�ת��������
	int StrToInt( char *string ,unsigned char *pBuf);
	
	//�ж�8λ����ĳһλ�Ƿ�Ϊ0��iIndexȡֵΪ0-7
	bool HasBit8(unsigned char c, int iIndex);

	//��1-255ת����1-8���ٶ�
	unsigned long SpeedAreaConvert(unsigned char speed);  

	void PtzCtrLock()   {m_PtzLock.Lock();}
	void PtzCtrUnLock() {m_PtzLock.UnLock();}

private:	
	unsigned  long				chnn;                                 //PTZͨ��
	CGb28181Server				*m_pGb28181Server;			//����Gb28181Server�ӿ�
	CPUB_Lock					m_PtzLock;			//m_PtzLock lock
};
#endif