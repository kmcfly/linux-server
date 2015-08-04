#ifndef _PL14_NVKBD30_70_H_
#define _PL14_NVKBD30_70_H_
/*
*PL14客户要求的控制键盘
*每个命令的数据长度10个字节
*数据格式如下
*|开始字节|控制ID|设备ID|字节|校验字节|
*4Eccdd10 25 00808080ss  
*cc- controller ID ( 00h : FFh)  
*dd- device ID ( 00h : FFh )  
*ss- check sum (00h : FFh )  
*ss = byte1 + byte2 +byte3+byte4+byte5+byte6+byte7+byte8+byte9 
*第4个字节表示设备类型，00是摄像头控制（或者控件？），10是DVR控制，20是矩阵控制，用键盘来控制DVR时第4位字节固定是0x10.
*第5个字节表示功能：在我们N-CONTROL的说明里面有描述，在这种情况下25意味着停止的功能。
*第6个字节表示功能的参数
*第7,8,9个字节表示对快球的pan tilt和zoom的控制，80是中性的，不改变任何东西，控制DVR时，7.8.9位的字节固定是80
*
*/

const unsigned long PL14_NVKBD_FIXED_NUMBER = 0x808080;
const char	PL14_NVKBD_START_CHAR	= 0x4e;
const char  DEVICE_ID_POS	= 2;
//数字键
const unsigned long PL14_NVKBD_NUM	= 0x101000;

//功能键
const unsigned long PL14_NVKBD_ENTER	= 0x102000;
const unsigned long PL14_NVKBD_ESC		= 0x102100;			//CTRL + ESC
const unsigned long PL14_NVKBD_MENU		= 0x102200;

//录像和播放控制
const unsigned long PL14_NVKBD_RECORD	= 0x104900;			//录像和停止

const unsigned long PL14_NVKBD_PLAY_STOP= 0x104a00;
const unsigned long PL14_NVKBD_PLAY_SF	= 0x104001;			//播放下一帧
const unsigned long PL14_NVKBD_PLAY_SR	= 0x104101;			//播放上一帧
const unsigned long PL14_NVKBD_PLAY_FF	= 0x104200;			//快进
const unsigned long PL14_NVKBD_PALY_REW	= 0x104300;			//快退
const unsigned long PL14_NVKBD_PLAY		= 0x104400;			//播放和暂停

//
const unsigned long PL14_NVKBD_SPOT		= 0x101400;			//跳台
const unsigned long PL14_NVKBD_GROUP	= 0x101300;			//数字+GROUP
const unsigned long PL14_NVKBD_SEQ		= 0x101100;			//
const unsigned long PL14_NVKBD_BACKUP	= 0x102500;
const unsigned long PL14_NVKBD_ENTER_PTZ= 0x102600;			//进入PTZ
const unsigned long PL14_NVKBD_SEARCH	= 0x104700;

const unsigned long PL14_NVKBD_UP		= 0x103700;
const unsigned long PL14_NVKBD_DOWN		= 0x103600;
const unsigned long PL14_NVKBD_LEFT		= 0x103500;
const unsigned long PL14_NVKBD_RIGHT	= 0x103400;

//分割
const unsigned long PL14_NVKBD_U_1X1	= 0x103700;
const unsigned long PL14_NVKBD_D_2X2	= 0x103600;
const unsigned long PL14_NVKBD_L_3X3	= 0x103500;
const unsigned long PL14_NVKBD_R_4X4	= 0x103400;

//+/-
const unsigned long PL14_NVKBD_ADD		= 0x102200;
const unsigned long PL14_NVKBD_DEC		= 0x102500;

const unsigned long PL14_NVKBD_SEL		= 0x102300;
//飞梭
const unsigned long PL14_NVKBD_FLY		= 0x106000;
const unsigned long PL14_NVKBD_FLY_IN	= 0x106100;

#endif /*_PL14_NVKBD30_70_H_*/

