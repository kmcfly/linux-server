#ifndef _PL14_NVKBD30_70_H_
#define _PL14_NVKBD30_70_H_
/*
*PL14�ͻ�Ҫ��Ŀ��Ƽ���
*ÿ����������ݳ���10���ֽ�
*���ݸ�ʽ����
*|��ʼ�ֽ�|����ID|�豸ID|�ֽ�|У���ֽ�|
*4Eccdd10 25 00808080ss  
*cc- controller ID ( 00h : FFh)  
*dd- device ID ( 00h : FFh )  
*ss- check sum (00h : FFh )  
*ss = byte1 + byte2 +byte3+byte4+byte5+byte6+byte7+byte8+byte9 
*��4���ֽڱ�ʾ�豸���ͣ�00������ͷ���ƣ����߿ؼ�������10��DVR���ƣ�20�Ǿ�����ƣ��ü���������DVRʱ��4λ�ֽڹ̶���0x10.
*��5���ֽڱ�ʾ���ܣ�������N-CONTROL��˵�������������������������25��ζ��ֹͣ�Ĺ��ܡ�
*��6���ֽڱ�ʾ���ܵĲ���
*��7,8,9���ֽڱ�ʾ�Կ����pan tilt��zoom�Ŀ��ƣ�80�����Եģ����ı��κζ���������DVRʱ��7.8.9λ���ֽڹ̶���80
*
*/

const unsigned long PL14_NVKBD_FIXED_NUMBER = 0x808080;
const char	PL14_NVKBD_START_CHAR	= 0x4e;
const char  DEVICE_ID_POS	= 2;
//���ּ�
const unsigned long PL14_NVKBD_NUM	= 0x101000;

//���ܼ�
const unsigned long PL14_NVKBD_ENTER	= 0x102000;
const unsigned long PL14_NVKBD_ESC		= 0x102100;			//CTRL + ESC
const unsigned long PL14_NVKBD_MENU		= 0x102200;

//¼��Ͳ��ſ���
const unsigned long PL14_NVKBD_RECORD	= 0x104900;			//¼���ֹͣ

const unsigned long PL14_NVKBD_PLAY_STOP= 0x104a00;
const unsigned long PL14_NVKBD_PLAY_SF	= 0x104001;			//������һ֡
const unsigned long PL14_NVKBD_PLAY_SR	= 0x104101;			//������һ֡
const unsigned long PL14_NVKBD_PLAY_FF	= 0x104200;			//���
const unsigned long PL14_NVKBD_PALY_REW	= 0x104300;			//����
const unsigned long PL14_NVKBD_PLAY		= 0x104400;			//���ź���ͣ

//
const unsigned long PL14_NVKBD_SPOT		= 0x101400;			//��̨
const unsigned long PL14_NVKBD_GROUP	= 0x101300;			//����+GROUP
const unsigned long PL14_NVKBD_SEQ		= 0x101100;			//
const unsigned long PL14_NVKBD_BACKUP	= 0x102500;
const unsigned long PL14_NVKBD_ENTER_PTZ= 0x102600;			//����PTZ
const unsigned long PL14_NVKBD_SEARCH	= 0x104700;

const unsigned long PL14_NVKBD_UP		= 0x103700;
const unsigned long PL14_NVKBD_DOWN		= 0x103600;
const unsigned long PL14_NVKBD_LEFT		= 0x103500;
const unsigned long PL14_NVKBD_RIGHT	= 0x103400;

//�ָ�
const unsigned long PL14_NVKBD_U_1X1	= 0x103700;
const unsigned long PL14_NVKBD_D_2X2	= 0x103600;
const unsigned long PL14_NVKBD_L_3X3	= 0x103500;
const unsigned long PL14_NVKBD_R_4X4	= 0x103400;

//+/-
const unsigned long PL14_NVKBD_ADD		= 0x102200;
const unsigned long PL14_NVKBD_DEC		= 0x102500;

const unsigned long PL14_NVKBD_SEL		= 0x102300;
//����
const unsigned long PL14_NVKBD_FLY		= 0x106000;
const unsigned long PL14_NVKBD_FLY_IN	= 0x106100;

#endif /*_PL14_NVKBD30_70_H_*/

